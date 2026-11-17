// Behaviour-written bodies for native M21 (Scout1170). NOT EXACT.
// WorldDominationTour::Map's two toggles (pass-6a decompiles 0072a6d0, 0072a720).
// address_aliases.py refuses the unowned UnknownToggle_72a6d0 bodies (a padded
// view of an RTTI class), so these follow the decompiles against wdt_map.hpp's
// load layout (the one Map::Load 0x00729510 fills): +0x54 the back-button
// rectangle, +0x64 the overlay animation and +0x68 its flag, +0x6C/+0x70 the
// off/on frames, +0x74 the second flag. The overlay shows the "on" frame only
// while both flags are set; the engine then broadcasts to its animations (the
// rectangle's address, which every slot 8 ignores, as a 32-bit value) and redraws.
#define RA2_WDT_MAP_LOAD_LAYOUT
#include "shared/wdt_map.hpp"
#include "shared/ms_anim.hpp"
#include "shared/ms_engine.hpp"
#include <stdint.h>

namespace WorldDominationTour {

// 0x0072A6D0
void Map::Set_Unknown_74(MSEngine *engine, bool value)
{
    if (unknown_6c.unknown_8 != value) {
        unknown_6c.unknown_8 = value;
        unknown_64.unknown_0->Set_Frame(value && unknown_64.unknown_4 ? unknown_6c.unknown_4 : unknown_6c.unknown_0);
        engine->Broadcast((int)(intptr_t)&unknown_54);
        engine->Redraw();
    }
}

// 0x0072A720
void Map::Set_Unknown_68(MSEngine *engine, bool value)
{
    if (unknown_64.unknown_4 != value) {
        unknown_64.unknown_4 = value;
        unknown_64.unknown_0->Set_Frame(!unknown_6c.unknown_8 || !value ? unknown_6c.unknown_0 : unknown_6c.unknown_4);
        engine->Broadcast((int)(intptr_t)&unknown_54);
        engine->Redraw();
    }
}

}  // namespace WorldDominationTour
