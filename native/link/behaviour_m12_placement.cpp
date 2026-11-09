// Behaviour row (Scout1111, native M12; NOT EXACT, native/behaviour_rewrites.json):
// 0x006F8A00 Unknown_006F8A00 (seed src/recovered/006f8a00.cpp), the twin of
// 0x006F8A20 in behaviour_launch.cpp: a completed building waits for placement in
// the second slot (0x00AC1338) when its type's BuildCat (+0xBA0) is 5, else in the
// first (0x00AC1334). SidebarClass::StripClass::AI 0x0067F050 calls it when the
// harness's power plant is ready. Pointer-sized slots (data/bss_globals.cpp).
#include "shared/building_class.hpp"

extern "C" BuildingClass *ra2_pending_placement_ac1334 __asm__("_DAT_00ac1334");
extern "C" BuildingClass *ra2_pending_placement_ac1338 __asm__("_DAT_00ac1338");

void __fastcall Unknown_006F8A00(BuildingClass *building)
{
    if (building->Type->BuildCat == 5) {
        ra2_pending_placement_ac1338 = building;
        return;
    }
    ra2_pending_placement_ac1334 = building;
}
