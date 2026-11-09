// Behaviour-written tactical-path rows (Scout1089; Tactical::Detach 0x006AAB60 is an accepted source since Scout1110;
// placement during Read_Scenario_INI detaches through 0x006ECB90). NOT EXACT;
// never original code or bytes.
#include "shared/mouse_class.hpp"
#include "shared/tactical.hpp"
#include "shared/map_view.hpp"   // extern MouseClass Map (after the full layers)
#include <stdint.h>

// 0x0049EC40 (seed src/recovered/0049ec40.cpp): the object at +0x119C when
// +0x1198 is set, else null.
ObjectClass *DisplayClass::Unknown_0049EC40()
{
    if (!unknown_1198 || unknown_119c == 0) {
        return 0;
    }
    return (ObjectClass *)unknown_119c;
}
