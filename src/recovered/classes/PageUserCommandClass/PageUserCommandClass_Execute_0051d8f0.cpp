// void PageUserCommandClass::Execute(int key) const -- 0x0051D8F0
// Pages a user (Queue_Event_22 0x00623600) in an internet game (mode 4).
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#include "shared/vector_class.hpp"
#include "shared/command.hpp"
#include "shared/mouse_class.hpp"
#include "shared/map_view.hpp"
#include "shared/object.hpp"
#include "shared/tab.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/queue.hpp"

void PageUserCommandClass::Execute(int key) const
{
    if (unknown_game_mode_a3d298 == 4) {
        Queue_Event_22();
    }
}
