// SPDX-License-Identifier: GPL-3.0-or-later
// Touch gestures to the game's mouse (touch.hpp; Scout1248).
#include "touch.hpp"

#include <math.h>
#include <stdio.h>

namespace touch {
namespace {

constexpr uint64_t kHoldMs = 400;        // a finger held still this long is a hold
constexpr uint64_t kTwoFingerMs = 400;   // a two-finger tap lifts within this
constexpr float kSlop = 10.0f;           // movement (game pixels) that makes a touch a swipe or a drag
constexpr float kWheelStep = 40.0f;      // finger movement per wheel notch over the sidebar
// Edge scrolling is the mouse's: a touch never leaves the pointer within this many
// pixels of the picture's edge, where the game would scroll the map on its own.
constexpr float kEdge = 10.0f;

enum class State {
    idle,
    pending,     // one finger down, not decided yet
    pan,         // swipe over the map
    wheel,       // swipe over the sidebar
    armed,       // held on the map: a drag selects, a release is a right click
    box,         // dragging the selection box (left button down)
    done,        // the gesture already acted (a hold on the sidebar): wait for the fingers to lift
    two,         // two fingers down
};

State state = State::idle;
int screen_w = 800, screen_h = 600, sidebar_x = 0;
uint64_t first_id = 0, second_id = 0;
bool second_down = false;
int fingers = 0;
float x0 = 0, y0 = 0, last_x = 0, last_y = 0, wheel_rest = 0;
uint64_t down_ms = 0;
bool moved_two = false;

float clamp(float v, float lo, float hi) { return v < lo ? lo : v > hi ? hi : v; }
void edge(float &x, float &y)
{
    x = clamp(x, kEdge, (float)screen_w - 1 - kEdge);
    y = clamp(y, kEdge, (float)screen_h - 1 - kEdge);
}
bool over_sidebar(float x) { return sidebar_x > 0 && x >= (float)sidebar_x; }
void click(Output const &out, int button, float x, float y)
{
    edge(x, y);
    out.move(x, y);
    out.button(button, true, x, y);
    out.button(button, false, x, y);
}

}  // namespace

void set_screen(int width, int height, int sidebar)
{
    screen_w = width > 2 * (int)kEdge ? width : 800;
    screen_h = height > 2 * (int)kEdge ? height : 600;
    sidebar_x = sidebar;
}

bool active() { return fingers > 0; }

void finger_down(uint64_t id, float x, float y, uint64_t ms, Output const &out)
{
    ++fingers;
    if (state == State::idle) {
        state = State::pending;
        first_id = id;
        second_down = false;
        x0 = last_x = x;
        y0 = last_y = y;
        down_ms = ms;
        wheel_rest = 0;
        return;
    }
    if (state == State::pending && !second_down && ms - down_ms < kTwoFingerMs) {
        state = State::two;
        second_id = id;
        second_down = true;
        moved_two = false;
    }
    // Other fingers during a gesture are ignored.
}

void finger_move(uint64_t id, float x, float y, uint64_t ms, Output const &out)
{
    if (state == State::two) {
        if ((id == first_id && hypotf(x - x0, y - y0) > 2 * kSlop)) moved_two = true;
        return;
    }
    if (id != first_id) return;
    float dx = x - last_x, dy = y - last_y;
    switch (state) {
    case State::pending:
        if (hypotf(x - x0, y - y0) <= kSlop) return;
        state = over_sidebar(x0) ? State::wheel : State::pan;
        dx = x - x0;
        dy = y - y0;
        // fall through: the movement so far counts
        [[fallthrough]];
    case State::pan:
        if (state == State::pan) {
            // The map follows the finger: the view moves the other way.
            float px = x, py = y;
            edge(px, py);
            out.pan(-dx, -dy, px, py);
            break;
        }
        [[fallthrough]];
    case State::wheel: {
        // Finger down pulls the list down: earlier items, as a wheel turned up.
        wheel_rest += dy;
        int notches = 0;
        while (wheel_rest >= kWheelStep) { ++notches; wheel_rest -= kWheelStep; }
        while (wheel_rest <= -kWheelStep) { --notches; wheel_rest += kWheelStep; }
        float px = x0, py = y0;
        edge(px, py);
        if (notches) out.wheel(notches, px, py);
        break;
    }
    case State::armed:
        if (hypotf(x - x0, y - y0) <= kSlop) return;
        {
            // The selection box: pressed where the hold was, then dragged.
            float px = x0, py = y0;
            edge(px, py);
            out.move(px, py);
            out.button(0, true, px, py);
            state = State::box;
        }
        [[fallthrough]];
    case State::box: {
        float px = x, py = y;
        edge(px, py);
        out.move(px, py);
        break;
    }
    default:
        break;
    }
    last_x = x;
    last_y = y;
    (void)ms;
}

void finger_up(uint64_t id, float x, float y, uint64_t ms, Output const &out)
{
    if (fingers > 0) --fingers;
    switch (state) {
    case State::pending:
        if (id == first_id) {
            click(out, 0, x0, y0);   // tap: left click where the finger came down
            state = State::idle;
        }
        break;
    case State::two:
        if (fingers == 0) {
            if (!moved_two && ms - down_ms < kTwoFingerMs + 300) click(out, 1, x0, y0);
            state = State::idle;
        }
        break;
    case State::armed:
        if (id == first_id) {
            click(out, 1, x0, y0);   // held still and released on the map: right click
            state = State::idle;
        }
        break;
    case State::box:
        if (id == first_id) {
            float px = x, py = y;
            edge(px, py);
            out.move(px, py);
            out.button(0, false, px, py);
            state = State::idle;
        }
        break;
    default:
        if (id == first_id || fingers == 0) state = State::idle;
        break;
    }
    if (fingers == 0 && state != State::idle) state = State::idle;
}

void tick(uint64_t ms, Output const &out)
{
    if (state != State::pending || ms - down_ms < kHoldMs) return;
    if (over_sidebar(x0)) {
        click(out, 1, x0, y0);       // held on a cameo: right click (cancel or put on hold)
        state = State::done;
    } else {
        float px = x0, py = y0;
        edge(px, py);
        out.move(px, py);            // the pointer shows where the hold is
        state = State::armed;
    }
}

}  // namespace touch
