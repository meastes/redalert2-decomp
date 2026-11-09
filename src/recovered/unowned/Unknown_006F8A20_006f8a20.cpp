// void __fastcall Unknown_006F8A20(BuildingClass *building) (0x006F8A20)
// Forgets a building pending placement: null forgets both pending slots,
// otherwise the slot that holds the building (0x006F8A00 files them).
#include "shared/unknown_globals.hpp"
// @ref ?unknown_pending_building_ac1334@@3PAVBuildingClass@@A=00ac1334
// @ref ?unknown_pending_building_ac1338@@3PAVBuildingClass@@A=00ac1338
extern BuildingClass *unknown_pending_building_ac1334;     // 0x00AC1334
extern BuildingClass *unknown_pending_building_ac1338;     // 0x00AC1338 (owner +0xBA0 == 5)

void __fastcall Unknown_006F8A20(BuildingClass *building)
{
    if (building == 0) {
        unknown_pending_building_ac1334 = 0;
        unknown_pending_building_ac1338 = 0;
        return;
    }
    if (building == unknown_pending_building_ac1334) {
        unknown_pending_building_ac1334 = 0;
        return;
    }
    if (building == unknown_pending_building_ac1338) {
        unknown_pending_building_ac1338 = 0;
    }
}
