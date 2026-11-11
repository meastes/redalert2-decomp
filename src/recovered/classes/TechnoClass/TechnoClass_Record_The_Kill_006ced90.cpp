// void TechnoClass::Record_The_Kill(TechnoClass *killer) (0x006CED90, slot 55)
// Records the object's destruction by the techno killer (may be null): springs
// its tag (events 6 and 4 with a killer; for non-units 7 with a killer, 0x30
// and 0x1D), gives the killer (and its +0x23C parent when the killer's type
// has +0xB18) experience for the object's cost (doubled for veterans,
// tripled for elites, none for an ally), notes the killer's owner on the
// owner (+0x53B8) and adds the points to the killer's owner (+0x5418); then
// counts the loss by kind in the killer owner's trackers and per-house
// tables and asks for a radar redraw.
#include "shared/techno.hpp"
#include "shared/techno_type.hpp"
#include "shared/house.hpp"
#include "shared/tag.hpp"
#include "shared/infantry.hpp"
#include "shared/infantry_type.hpp"
#include "shared/aircraft.hpp"
#include "shared/aircraft_type.hpp"
#include "shared/foot.hpp"
#include "shared/gscreen.hpp"
#include "shared/techno_map.hpp"
// @ref ?unknown_cell_ac0148@@3VCell@@A=00ac0148
// @ref ?Spring@TagClass@@QAE_NHPAVObjectClass@@VCell@@_NH@Z=006b3df0
// @ref ?Is_Ally@HouseClass@@QBE_NPBVTechnoClass@@@Z=004e5580
// @ref ?Is_Veteran@VeterancyClass@@QBE_NXZ=007133a0
// @ref ?Is_Elite@VeterancyClass@@QBE_NXZ=00713420
// @ref ?Made_A_Kill@VeterancyClass@@QAEXHH@Z=00713360
// @ref ?Unknown_0070C490@UnknownHouseTracker@@QAEXH@Z=0070c490

void TechnoClass::Record_The_Kill(TechnoClass *killer)
{
    int found = 0;
    int points = Techno_Type_Class()->Cost_Of(House);
    if (unknown_7d && unknown_34 != 0 && killer != 0) {
        unknown_34->Spring(6, this, unknown_cell_ac0148, 0, 0);
    }
    if (unknown_7d) {
        if (unknown_34 != 0 && killer != 0) {
            unknown_34->Spring(4, this, unknown_cell_ac0148, 0, 0);
        }
        if (unknown_7d && Kind_Of() != 1) {
            if (unknown_7d && unknown_34 != 0 && killer != 0) {
                unknown_34->Spring(7, this, unknown_cell_ac0148, 0, 0);
            }
            if (unknown_7d && unknown_34 != 0) {
                unknown_34->Spring(0x30, this, unknown_cell_ac0148, 0, 0);
            }
            if (unknown_7d && unknown_34 != 0) {
                unknown_34->Spring(0x1D, this, unknown_cell_ac0148, 0, 0);
            }
        }
    }
    if (killer != 0) {
        if (killer->House->Is_Ally(this)) {
            points = 0;
        } else if (unknown_118.Is_Veteran()) {
            points += points;
        } else if (unknown_118.Is_Elite()) {
            points *= 3;
        }
        if (killer->Techno_Type_Class()->Trainable) {
            killer->unknown_118.Made_A_Kill(killer->Techno_Type_Class()->Cost_Of(House), points);
        }
        if (killer->Techno_Type_Class()->MissileSpawn && killer->unknown_23c != 0
            && ((TechnoClass *)killer->unknown_23c)->Techno_Type_Class()->Trainable) {
            ((TechnoClass *)killer->unknown_23c)->unknown_118.Made_A_Kill(
                ((TechnoClass *)killer->unknown_23c)->Techno_Type_Class()->Cost_Of(House), points);
        }
        House->unknown_53b8 = killer->Owner();
        killer->House->unknown_5418 += points;
    }
    switch (Kind_Of()) {
    case 6:
        if (Techno_Type_Class()->unknown_202) {
            break;
        }
        if (((BuildingClass *)this)->unknown_434 != -1) {
            House->unknown_53b4++;
        }
        if (killer != 0) {
            killer->House->unknown_3aa0.Unknown_0070C490(((BuildingClass *)this)->Type->ArrayIndex);
            killer->House->unknown_5364[Owner()]++;
        }
        if (Map.Is_Player_Names()) {
            Map.Redraw_Radar(false);
        }
        break;
    case 2:
        if (killer != 0) {
            killer->House->unknown_2288.Unknown_0070C490(((AircraftClass *)this)->Type->ArrayIndex);
            found = 1;
        }
    case 15:
        if (killer != 0 && !found) {
            killer->House->unknown_2a90.Unknown_0070C490(((InfantryClass *)this)->Type->ArrayIndex);
            found = 1;
        }
    case 1:
        if (killer != 0 && !found) {
            killer->House->unknown_3298.Unknown_0070C490(((UnitClass *)this)->Type->ArrayIndex);
        }
        House->unknown_5360++;
        if (killer != 0) {
            killer->House->unknown_5310[Owner()]++;
        }
        if (Map.Is_Player_Names()) {
            Map.Redraw_Radar(false);
        }
        break;
    }
}
