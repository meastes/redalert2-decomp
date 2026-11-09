// void BeaconPlacementCommandClass::Execute(int key) const -- 0x0051C5B0
// Beacon placement (0x006F64B0) outside the campaign and skirmish (game modes 0 and 5).
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#include "shared/vector_class.hpp"
#include "shared/command.hpp"
#include "shared/mouse_class.hpp"
#include "shared/map_view.hpp"
#include "shared/object.hpp"
#include "shared/tab.hpp"
// @ref ?Unknown_006F64B0@@YAXXZ=006f64b0
#include "shared/unknown_globals.hpp"

void BeaconPlacementCommandClass::Execute(int key) const
{
    if (unknown_game_mode_a3d298 != 0 && unknown_game_mode_a3d298 != 5) {
        Unknown_006F64B0();
    }
}
