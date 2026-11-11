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
//   two-finger tap              right click
// No SDL here: platform/sdl/host.cpp converts SDL's finger events.
#pragma once
#include <stdint.h>

namespace touch {

struct Output {
    void (*move)(float x, float y);                        // the pointer moves (no button change)
    void (*button)(int button, bool down, float x, float y);   // 0 left, 1 right
    void (*pan)(float dx, float dy, float x, float y);    // view moves by dx, dy game pixels
    void (*wheel)(int notches, float x, float y);         // + up, - down (120 each)
};

// The game picture's size in game pixels and the sidebar's left edge (no sidebar: <= 0).
void set_screen(int width, int height, int sidebar_x);
void finger_down(uint64_t id, float x, float y, uint64_t ms, Output const &out);
void finger_move(uint64_t id, float x, float y, uint64_t ms, Output const &out);
void finger_up(uint64_t id, float x, float y, uint64_t ms, Output const &out);
// Called often (every event pump): a finger held still long enough becomes a hold.
void tick(uint64_t ms, Output const &out);
// Fingers are down (the host keeps the pointer where the gesture left it).
bool active();

}  // namespace touch
