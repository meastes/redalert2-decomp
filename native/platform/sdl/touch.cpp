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
    two,         // two fingers down: a two-finger tap until they move past kTwoSlop
    two_pan,     // two fingers moved past kTwoSlop: the map follows them (Scout1281)
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
float x2_last = 0, y2_last = 0;
float pan_x = 0, pan_y = 0;
// The last finger that came down during another gesture and was ignored: when that gesture
// turns out to have been a stale touch (its lift lost), this finger starts the gesture.
uint64_t ignored_id = 0, ignored_ms = 0;
float ignored_x = 0, ignored_y = 0;
bool ignored_valid = false;   // the view movement this one-finger pan has made (undone if it becomes a two-finger tap)

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
    x2 = x2_last = x;
    y2 = y2_last = y;
    second_ms = ms;
}
// A hold decided by tick (the host's clock) that the finger's own events contradict:
// the move or lift came less than kHoldMs after the touch by the events' timestamps (a
// long frame between the touch and the next event read; Scout1281, user report: a tap
// at the start of a skirmish began a selection box). The gesture is decided again by
// the events' times.
bool hold_contradicted(uint64_t ms, const char *what)
{
    if (ms - down_ms >= kHoldMs) return false;
    fprintf(stderr, "touch: hold undone: the %s came %llu ms after the touch\n", what,
            (unsigned long long)(ms - down_ms));
    return true;
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
    // A finger id the gestures still hold: its lift never came (UIKit reuses touch ids, so
    // the next touch arrives under the same id). The old touch is forgotten first; kept, it
    // stayed the gesture's first finger, its stale position and time turned the new touch
    // into a hold, a selection box or a two-finger pan (Scout1281, user reports: a tap began
    // a selection box; the two-finger tap panned).
    for (int i = 0; i < fingers; ++i)
        if (finger_ids[i] == id) {
            fprintf(stderr, "touch: finger %llx down again with no lift: the old touch is forgotten\n", (unsigned long long)id);
            forget_finger(id, ms, out);
            if (state == State::idle && ignored_valid && ms - ignored_ms < kSecondMs) {
                bool held = false;
                for (int k = 0; k < fingers; ++k) held = held || finger_ids[k] == ignored_id;
                if (held) {
                    fprintf(stderr, "touch: finger %llx (+%llu ms) starts the gesture instead\n",
                            (unsigned long long)ignored_id, (unsigned long long)(ms - ignored_ms));
                    state = State::pending;
                    first_id = ignored_id;
                    second_down = false;
                    x0 = last_x = ignored_x;
                    y0 = last_y = ignored_y;
                    down_ms = ignored_ms;
                    wheel_rest = 0;
                    pan_x = pan_y = 0;
                }
            }
            ignored_valid = false;
            break;
        }
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
        pan_x = pan_y = 0;
        return;
    }
    if (state == State::pending && !second_down && ms - down_ms < kSecondMs) {
        fprintf(stderr, "touch: second finger +%llu ms: two-finger tap candidate\n", (unsigned long long)(ms - down_ms));
        start_two(id, x, y, ms);
        return;
    }
    // A second finger soon after the first began a pan (Scout1281; user report: the
    // two-finger tap kept panning the map): the first finger moved past kSlop before the
    // second landed. Within kSecondMs and kTwoSlop it is a two-finger tap candidate after
    // all: the pan so far is undone.
    if (state == State::pan && !second_down && ms - down_ms < kSecondMs && hypotf(last_x - x0, last_y - y0) <= kTwoSlop) {
        fprintf(stderr, "touch: second finger +%llu ms during a %.0f px pan: two-finger tap candidate (pan undone)\n",
                (unsigned long long)(ms - down_ms), hypotf(last_x - x0, last_y - y0));
        float px = last_x, py = last_y;
        edge(px, py);
        if (pan_x != 0 || pan_y != 0) out.pan(-pan_x, -pan_y, px, py);
        pan_x = pan_y = 0;
        start_two(id, x, y, ms);
        return;
    }
    // Other fingers during a gesture are ignored (remembered in case the gesture's own
    // finger proves stale).
    ignored_id = id;
    ignored_ms = ms;
    ignored_x = x;
    ignored_y = y;
    ignored_valid = true;
}

void finger_move(uint64_t id, float x, float y, uint64_t ms, Output const &out)
{
    if (state == State::two) {
        float d = id == first_id ? hypotf(x - x0, y - y0) : id == second_id ? hypotf(x - x2, y - y2) : 0;
        if (d <= kTwoSlop) return;
        // Past the tap slop: no tap any more; the two fingers pan the map from where they
        // came down (Scout1281).
        moved_two = true;
        state = State::two_pan;
        last_x = x0;
        last_y = y0;
        x2_last = x2;
        y2_last = y2;
        fprintf(stderr, "touch: two fingers moved %.0f px (+%llu ms): pan\n", d, (unsigned long long)(ms - down_ms));
    }
    if (state == State::two_pan) {
        // The map follows the fingers' middle: each finger's movement counts half while
        // both are down.
        float *lx = id == first_id ? &last_x : id == second_id ? &x2_last : nullptr;
        float *ly = id == first_id ? &last_y : id == second_id ? &y2_last : nullptr;
        if (!lx) return;
        float share = fingers >= 2 ? 0.5f : 1.0f;
        float dx = (x - *lx) * share, dy = (y - *ly) * share;
        *lx = x;
        *ly = y;
        if (!over_sidebar(x0)) {
            float px = x, py = y;
            edge(px, py);
            out.pan(-dx, -dy, px, py);
        }
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
            pan_x -= dx;
            pan_y -= dy;
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
        if (hold_contradicted(ms, "move")) {
            // A swipe, as it would have been without the hold.
            state = over_sidebar(x0) ? State::wheel : State::pan;
            dx = x - x0;
            dy = y - y0;
            if (state == State::pan) {
                float px = x, py = y;
                edge(px, py);
                out.pan(-dx, -dy, px, py);
                pan_x -= dx;
                pan_y -= dy;
            }
            break;
        }
        {
            fprintf(stderr, "touch: selection box (held %llu ms)\n", (unsigned long long)(ms - down_ms));
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
    case State::tap_wait:
        // Its first finger already lifted: an ignored finger must not cancel the waiting click.
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
    case State::two_pan:
        if (fingers == 0) {
            fprintf(stderr, "touch: two-finger pan ended (+%llu ms)\n", (unsigned long long)(ms - down_ms));
            state = State::idle;
        }
        break;
    case State::armed:
        if (id == first_id) {
            if (hold_contradicted(ms, "lift")) {
                state = State::tap_wait;   // a tap after all
                up_ms = ms;
                break;
            }
            fprintf(stderr, "touch: hold released: right click (held %llu ms)\n", (unsigned long long)(ms - down_ms));
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
        fprintf(stderr, "touch: hold (%llu ms by the clock)\n", (unsigned long long)(ms - down_ms));
    }
}

}  // namespace touch
