// Behaviour-written Tactical::Detach (Scout1089; skirmish harness: OverlayClass
// placement during Read_Scenario_INI detaches through 0x006ECB90). NOT EXACT;
// never original code or bytes.
#include "shared/mouse_class.hpp"
#include "shared/tactical.hpp"
#include "shared/map_view.hpp"   // extern MouseClass Map (after the full layers)
#include <stdint.h>

// 0x006AAB60 Tactical::Detach, slot 10 (the row's seed src/recovered/classes/
// Tactical/006aab60.cpp; TibSun tactical.cpp Detach): forgets the object in the
// 500-entry drawn-object table (0x00ABE5F8, 12-byte entries); when it is the
// pending placement object (Map +0x118C), clears the placement cursor
// (DisplayClass 0x00499070 with no list) and +0x118C/+0x1190, and sets +0x1194
// to -1. display.hpp declares +0x118C an int (it holds an object pointer: the
// int/pointer conflict listed for src/shared), compared here as the int it holds.
void Tactical::Detach(AbstractClass *target, bool all)
{
    for (int index = 0; index < 500; index++) {
        if ((AbstractClass *)unknown_tactical_pixels_abe5f8[index].Object == target) {
            unknown_tactical_pixels_abe5f8[index].Object = 0;
        }
    }
    if ((int)(intptr_t)target == Map.unknown_118c) {
        Map.Set_Cursor_Shape(0);
        Map.unknown_118c = 0;
        Map.unknown_1190 = 0;
        Map.unknown_1194 = -1;
    }
}

// 0x0049EC40 (seed src/recovered/0049ec40.cpp): the object at +0x119C when
// +0x1198 is set, else null.
ObjectClass *DisplayClass::Unknown_0049EC40()
{
    if (!unknown_1198 || unknown_119c == 0) {
        return 0;
    }
    return (ObjectClass *)unknown_119c;
}
