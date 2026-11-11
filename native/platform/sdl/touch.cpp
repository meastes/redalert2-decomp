// SPDX-License-Identifier: GPL-3.0-or-later
// Touch gestures to the game's mouse (touch.hpp; Scout1248).
#include "touch.hpp"

#include <math.h>
#include <stdio.h>

namespace touch {
namespace {

constexpr uint64_t kHoldMs = 400;        // a finger held still this long is a hold
// Two-finger tap (right click), forgiving (Scout1274; user report: on the Duo it often came
// out as a one-finger tap that moved the selection): a one-finger tap's left click waits
// kTapWaitMs after the lift for a second finger; the second finger may land up to
// kSecondMs after the first, even after the first has lifted (within kTapWaitMs); both
// fingers may move up to kTwoSlop and lift within kTwoLiftMs of the first touch. A second
// finger within kDoubleTapRadius of a lifted first one is a double tap instead (two left
// clicks: the game's double click selects all of a type).
constexpr uint64_t kTapWaitMs = 100;
constexpr uint64_t kSecondMs = 500;
constexpr uint64_t kTwoLiftMs = 1000;
constexpr float kTwoSlop = 40.0f;
constexpr float kDoubleTapRadius = 15.0f;
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
    tap_wait,    // a one-finger tap lifted: its left click waits kTapWaitMs for a second finger
};

State state = State::idle;
int screen_w = 800, screen_h = 600, sidebar_x = 0;
uint64_t first_id = 0, second_id = 0;
bool second_down = false;
// The fingers down, by id (a set, not a count: a lost lift is dropped by forget_finger,
// which the host calls when SDL no longer lists that finger; Scout1274).
constexpr int kMaxFingers = 10;
uint64_t finger_ids[kMaxFingers];
int fingers = 0;
float x0 = 0, y0 = 0, last_x = 0, last_y = 0, wheel_rest = 0, x2 = 0, y2 = 0;
uint64_t down_ms = 0, up_ms = 0, second_ms = 0;
bool moved_two = false;

bool add_finger(uint64_t id)
{
    for (int i = 0; i < fingers; ++i)
        if (finger_ids[i] == id) return false;
    if (fingers < kMaxFingers) finger_ids[fingers++] = id;
    return true;
}
bool remove_finger(uint64_t id)
{
    for (int i = 0; i < fingers; ++i)
        if (finger_ids[i] == id) { finger_ids[i] = finger_ids[--fingers]; return true; }
    return false;
}

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
// The waiting tap's left click goes out now.
void flush_tap(Output const &out, uint64_t ms, const char *why)
{
    if (state != State::tap_wait) return;
    fprintf(stderr, "touch: tap (held %llu ms, click %llu ms after the lift: %s)\n",
            (unsigned long long)(up_ms - down_ms), (unsigned long long)(ms - up_ms), why);
    click(out, 0, x0, y0);
    state = State::idle;
}
void start_two(uint64_t id, float x, float y, uint64_t ms)
{
    state = State::two;
    second_id = id;
    second_down = true;
    moved_two = false;
    x2 = x;
    y2 = y;
    second_ms = ms;
}

}  // namespace

void set_screen(int width, int height, int sidebar)
{
    screen_w = width > 2 * (int)kEdge ? width : 800;
    screen_h = height > 2 * (int)kEdge ? height : 600;
    sidebar_x = sidebar;
}

bool active() { return fingers > 0; }

int finger_list(uint64_t *ids, int max)
{
    int n = fingers < max ? fingers : max;
    for (int i = 0; i < n; ++i) ids[i] = finger_ids[i];
    return n;
}

void forget_finger(uint64_t id, uint64_t ms, Output const &out)
{
    // As if it lifted where it is now, without the tap or hold it would have made.
    fprintf(stderr, "touch: finger %llx has no lift from SDL: forgotten\n", (unsigned long long)id);
    if ((state == State::box) && id == first_id) {
        float px = last_x, py = last_y;
        edge(px, py);
        out.button(0, false, px, py);
    }
    remove_finger(id);
    if (id == first_id && (state == State::pending || state == State::armed || state == State::box || state == State::pan || state == State::wheel))
        state = State::idle;
    if (fingers == 0 && state != State::tap_wait) state = State::idle;
    (void)ms;
}

void finger_down(uint64_t id, float x, float y, uint64_t ms, Output const &out)
{
    add_finger(id);
    if (state == State::tap_wait) {
        // A second finger soon after a one-finger tap: a two-finger tap whose first finger
        // lifted early, unless it lands where the first did (a double tap).
        if (ms - up_ms < kTapWaitMs && ms - down_ms < kSecondMs && hypotf(x - x0, y - y0) > kDoubleTapRadius) {
            start_two(id, x, y, ms);
            return;
        }
        flush_tap(out, ms, "next touch");
    }
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
    if (state == State::pending && !second_down && ms - down_ms < kSecondMs) start_two(id, x, y, ms);
    // Other fingers during a gesture are ignored.
}

void finger_move(uint64_t id, float x, float y, uint64_t ms, Output const &out)
{
    if (state == State::two) {
        if ((id == first_id && hypotf(x - x0, y - y0) > kTwoSlop) || (id == second_id && hypotf(x - x2, y - y2) > kTwoSlop))
            moved_two = true;
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
    if (!remove_finger(id)) return;     // not a finger this gesture knows (already forgotten)
    switch (state) {
    case State::pending:
        if (id == first_id) {
            // tap: left click where the finger came down, after kTapWaitMs (tick) unless a
            // second finger lands first
            state = State::tap_wait;
            up_ms = ms;
        }
        break;
    case State::two:
        if (fingers == 0) {
            bool ok = !moved_two && ms - down_ms < kTwoLiftMs;
            fprintf(stderr, "touch: two-finger tap %s (second +%llu ms, lifted %llu ms after the first touch%s)\n",
                    ok ? "right click" : "dropped", (unsigned long long)(second_ms - down_ms),
                    (unsigned long long)(ms - down_ms), moved_two ? ", moved" : "");
            if (ok) click(out, 1, x0, y0);
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
    if (fingers == 0 && state != State::idle && state != State::tap_wait) state = State::idle;
}

void tick(uint64_t ms, Output const &out)
{
    if (state == State::tap_wait && ms - up_ms >= kTapWaitMs) flush_tap(out, ms, "no second finger");
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
