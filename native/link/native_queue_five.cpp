// Queue five with Shift (native addition, not in the original; lane C16, user 2026-11-14;
// the Enhancements page's "Queue five with Shift", off by default and in the Original
// preset). Written from the behaviour the design reference describes (redalert2-recomp
// docs/modern-controls.md, Queue Five); no code taken from it.
//
// A click on a build icon in the sidebar (SidebarClass::StripClass::SelectClass::Action
// 0x006809F0) adds the game's own production event (0x0E, HouseClass::Begin_Production at
// execution) to the outgoing queue OutList; a right click on one being built or queued adds
// the abandon event (0x10, HouseClass::Abandon_Production 0x004E64F0: one queued copy of the
// type is removed, else the object in production is abandoned). Native fixups route those
// adds here (port/fixups.json, sidebar.cpp). With the setting on and Shift held, the same
// event is added five times, as five clicks would add it: the events are the original's
// kinds and go through the game's own event path (a multiplayer game sends them as it sends
// clicks). The right click's first press on an object in production still puts it on hold
// (event 0x0F, once), as in the original; the abandons that follow remove five. The queue
// holds 128 events: the copies stop when it is full. Shift is the game's own key test
// (WWKeyboardClass::Down, as DisplayClass::Mouse_Left_Release reads it for the selection
// band), so the touch toolbar's Add (which holds Shift) gives a five-queue on a cameo tap.
#include "shared/event.hpp"
#include "shared/keyboard.hpp"
#include <stdio.h>

// The setting (platform/sdl/enhancements.cpp). Weak default: a build without the SDL host
// keeps the original behaviour.
extern "C" __attribute__((weak)) int ra2_enh_queue_five(void) { return 0; }

namespace {
constexpr int kQueueFive = 5;
constexpr KeyNumType kShift = 0x10;   // VK_SHIFT
}

// Adds a sidebar click's production (0x0E) or abandon (0x10) event; five copies with the
// setting on and Shift held. Answers OutList.Add's result for the first copy.
int Native_Queue_Five_Add(EventClass const &event)
{
    int added = OutList.Add(event);
    if (!added || !ra2_enh_queue_five() || WWKeyboard == 0 || !WWKeyboard->Down(kShift)) return added;
    int copies = 1;
    while (copies < kQueueFive && OutList.Add(event)) ++copies;
    fprintf(stderr, "queue-five: event %d added %d times\n", (int)event.Type, copies);
    return added;
}
