// NON-EXACT DRAFT 0x6C5270 FUN_006c5270
// entry 0x6C5270; exact-match score 96.38 (misses_index best); plan "draft as-is"
// origin lane: Scout898 (build/class-work/Scout898/w/am0.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1069 (native/drafts.json). Not original bytes.
// double TechnoClass::unknown_6c5270(Cell const &cell) (0x006C5270)
// A fire-suppression factor (hypothesis): 1.0 unless the slot 230 weapon has
// +0x11E set; then halved for each building of an allied house on the square
// rings of radius 1 up to Rules FireSupress cells (in leptons / 256) around
// the cell, cells outside the map skipped.
#include "shared/techno.hpp"
#include "shared/rules.hpp"
#include "shared/weapon_type.hpp"
#include "shared/gscreen.hpp"
#include "shared/techno_map.hpp"
// @ref ?Unknown_004717B0@UnknownCellClass@@QAEPAVBuildingClass@@XZ=004717b0
// @ref ?Is_Ally@HouseClass@@QBE_NPBVTechnoClass@@@Z=004e5580
// @ref __real@8@3fff8000000000000000=0079a708
// @ref __real@8@3ffe8000000000000000=0079a728

double TechnoClass::unknown_6c5270(Cell const &cell)
{
    WeaponStruct *weapon = unknown_slot230();
    if (weapon == 0 || weapon->WeaponType == 0 || !weapon->WeaponType->unknown_11e) {
        return 1.0;
    }
    double factor = 1.0;
    int range = Rules->FireSupress / 256;
    for (int radius = 1; radius < range; radius++) {
        for (int x = -radius; x <= radius; x++) {
            Cell spot;
            if (cell.X + x >= Map.unknown_120 && cell.X + x < Map.unknown_120 + Map.unknown_128) {
                if (cell.Y - radius >= Map.unknown_124) {
                    spot = cell + Cell(x, -radius);
                    BuildingClass *building = ((UnknownCellClass *)&Map[spot])->Unknown_004717B0();
                    if (building != 0 && House->Is_Ally(building)) {
                        factor *= 0.5;
                    }
                }
                if (cell.Y + radius < Map.unknown_124 + Map.unknown_12c) {
                    spot = cell + Cell(x, radius);
                    BuildingClass *building = ((UnknownCellClass *)&Map[spot])->Unknown_004717B0();
                    if (building != 0 && House->Is_Ally(building)) {
                        factor *= 0.5;
                    }
                }
            }
        }
        for (int y = -radius + 1; y < radius; y++) {
            Cell spot;
            if (cell.Y + y >= Map.unknown_124 && cell.Y + y < Map.unknown_124 + Map.unknown_12c) {
                if (cell.X - radius >= Map.unknown_120) {
                    spot = cell + Cell(-radius, y);
                    BuildingClass *building = ((UnknownCellClass *)&Map[spot])->Unknown_004717B0();
                    if (building != 0 && House->Is_Ally(building)) {
                        factor *= 0.5;
                    }
                }
                if (cell.X + radius < Map.unknown_120 + Map.unknown_128) {
                    spot = cell + Cell(radius, y);
                    BuildingClass *building = ((UnknownCellClass *)&Map[spot])->Unknown_004717B0();
                    if (building != 0 && House->Is_Ally(building)) {
                        factor *= 0.5;
                    }
                }
            }
        }
    }
    return factor;
}
