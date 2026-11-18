// SPDX-License-Identifier: GPL-3.0-or-later
// Touch gestures to the game's mouse (Scout1248, iOS brief step 3). The host
// feeds finger events in game pixels with their time; the gestures come out as
// the mouse and wheel input the game already takes:
//   tap                         left click
//   swipe over the map          map pan (ra2_native_scroll_map)
//   swipe over the sidebar      mouse wheel (the sidebar scroll)
//   hold ~400 ms, then drag     left button held: a selection box
//   hold ~400 ms on the map, release without moving   right click
//   hold ~400 ms on the sidebar (a cameo)              right click at once
//   hold ~400 ms on a command-bar team button          right click, then left click: the team
//                               becomes the selection (the bar's right click disbands the team and
//                               its click on an empty team assigns the selection; lane Y15)
//   two-finger tap              right click (forgiving: a one-finger tap's click waits
//                               100 ms for a second finger; Scout1274)
// No SDL here: platform/sdl/host.cpp converts SDL's finger events.
#pragma once
#include <stdint.h>

namespace touch {

struct Output {
    void (*move)(float x, float y);                        // the pointer moves (no button change)
    void (*button)(int button, bool down, float x, float y);   // 0 left, 1 right
    void (*pan)(float dx, float dy, float x, float y);    // view moves by dx, dy game pixels
    void (*wheel)(int notches, float x, float y);         // + up, - down (120 each)
    void (*zoom)(float scale, float wx, float wy);        // a pinch's step about its middle (window points; lane C16)
    int (*team_button)(float x, float y);                 // the command-bar team button (1-3) there, else 0 (lane Y15)
};

// Pinch zoom (lane C16): two fingers whose span changes become a pinch while this is on (the
// host's Zoom gestures, in play): it zooms only, never pans or clicks. Each finger's window point
// (points) is given beside its game pixel so the span does not follow the zoom it makes.
void set_pinch(bool on);
void set_window_point(float wx, float wy);

// The game picture's size in game pixels and the sidebar's left edge (no sidebar: <= 0).
void set_screen(int width, int height, int sidebar_x);
void finger_down(uint64_t id, float x, float y, uint64_t ms, Output const &out);
void finger_move(uint64_t id, float x, float y, uint64_t ms, Output const &out);
void finger_up(uint64_t id, float x, float y, uint64_t ms, Output const &out);
// Called often (every event pump): a finger held still long enough becomes a hold.
void tick(uint64_t ms, Output const &out);
// Fingers are down (the host keeps the pointer where the gesture left it).
bool active();
// The fingers the gestures hold as down (ids), and dropping one whose lift never came
// (the host compares them with SDL's own list; Scout1274).
int finger_list(uint64_t *ids, int max);
void forget_finger(uint64_t id, uint64_t ms, Output const &out);

}  // namespace touch
