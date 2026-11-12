// Unit AI: damage/death, deployment, falling, repair and refinery navigation.
#include "shared/unit.hpp"
#include "shared/anim.hpp"
#include "shared/cell.hpp"
#include "shared/map_view.hpp"
#include "shared/rules.hpp"
#include "shared/building_type.hpp"
#include "shared/team.hpp"
// The original caller passes House in ECX and this unit in EDX; source spelling is provisional.
BuildingClass *__fastcall Unknown_00450F90(HouseClass *house, UnitClass *unit);
// @ref ?Unknown_00450F90@@YIPAVBuildingClass@@PAVHouseClass@@PAVUnitClass@@@Z=00450f90
// @ref ?Unknown_006FAA30@UnitClass@@QAEXXZ=006faa30
// @ref ?Unknown_00709EC0@UnitClass@@QAEXXZ=00709ec0
// @ref ?AI@FootClass@@UAEXXZ=004c8ff0
// @ref ?unknown_6d6e50@TechnoClass@@QAEXPAVAbstractClass@@@Z=006d6e50
// @ref ?unknown_6cf130@TechnoClass@@QAE?AUCell@@PAVAbstractClass@@@Z=006cf130
// @ref ?Cell_Building@CellClass@@QBEPAVObjectClass@@XZ=004717b0
// @ref ?Is_Human_Player@HouseClass@@QAE_NXZ=004f52f0
// @ref ?Is_Count_Down_Complete@DelayTimerClass@@QBE_NXZ=00496b50
// @ref ?Count_Down@DelayTimerClass@@QAEXN@Z=00496bc0
// @ref ?HasAnyLink@RadioClass@@QBE_NXZ=00636f10
// @ref ?Unknown_0054AB00@MapClass@@QAE_NABUCoordStruct@@@Z=0054ab00
// @ref ?unknown_6fb890@UnitClass@@QAEXXZ=006fb890
// @ref ?unknown_6fbc90@UnitClass@@QAEXXZ=006fbc90
// @ref ?unknown_6fd3a0@UnitClass@@QAEXXZ=006fd3a0
// @ref ?unknown_6fbf40@UnitClass@@QAEXXZ=006fbf40
inline CoordStruct Unit_AI_Smoke_Position(CoordStruct const &position)
{
    CoordStruct smoke = position;
    smoke.X += 120;
    smoke.Y += 120;
    return smoke;
}
inline Cell Unit_AI_Refinery_Exit_Cell(Cell const &position)
{
    return Cell((short)(position.X + 3), (short)(position.Y + 1));
}
void UnitClass::AI()
{
    if (unknown_20c) unknown_20c->AI();
    if ((unknown_slot111() || unknown_slot112()) && Frame % 24 == 0 && Rules->unknown_2a8) {
        new AnimClass(Rules->unknown_2a8, Unit_AI_Smoke_Position(Coord), 0, 1, 0x600, 0);
    }
    if (unknown_slot112() || (unknown_slot111() && unknown_210)) {
        Locomotor.Checked()->Process();
        if (!unknown_7d) return;
    }
    if ((unknown_slot112() && unknown_20c) || unknown_slot111()) {
        if (unknown_228) unknown_slot220(0);
        if (unknown_494) {
            Assign_Destination(0, true);
            return;
        }
        return;
    }
    if (unknown_5c0 != -1) {
        if (unknown_5c0++ >= Type->MaxDeathCounter) {
            unknown_6fd3a0();
            Mark(0);
            Delete_Me();
            return;
        }
    }
    if (unknown_574 >= 0) {
        Unknown_006FAA30();
        unknown_slot267(false);
        return;
    }
    int countdown = unknown_5a8 - 1;
    if (countdown < -1) countdown = -1;
    unknown_5a8 = countdown;
    if (Type->DeploysInto) {
        for (int index = 0; index < Rules->unknown_7c8.Count(); index++) {
            if (Rules->unknown_7c8[index] == Type->DeploysInto) {
                if (House->unknown_13b && !House->Is_Human_Player() && unknown_game_mode_a3d298 != 0 && House->unknown_50.Count() == 0 && Mission != 15 && Mission != 16) {
                    QueueMission(15, false);
                }
                break;
            }
        }
    }
    if (unknown_slot122()) Commence();
    FootClass::AI();
    if (Type->CanDisguise && !Type->PermaDisguise) Unknown_00709EC0();
    if (unknown_325) {
        CoordStruct position = Coord;
        CoordStruct lowered(position.X, position.Y, position.Z - 5);
        Set_Coord(lowered);
        if (Get_Height_AGL() < -400) {
            Record_The_Kill(0);
            Delete_Me();
            return;
        }
        if ((Frame & 3) == 0) {
            int y_offset = unknown_id_source_a3d290->Random()(-170, 170);
            int x_offset = unknown_id_source_a3d290->Random()(-170, 170);
            int height = Get_Height_AGL();
            CoordStruct offset(x_offset, y_offset, -height);
            CoordStruct origin = Coord;
            offset.X = origin.X + offset.X;
            offset.Y += origin.Y;
            offset.Z += origin.Z;
            position = offset;
            new AnimClass(Rules->unknown_09c, position, 0, 1, 0x600, 0);
        }
    }
    if (unknown_7d) {
        if (Get_Mission() != 10) unknown_5ba = false;
        unknown_6fbc90();
        unknown_6fb890();
        if (Get_Mission() == 5) {
            CoordStruct position = Coord;
            if (!Map.Unknown_0054AB00(position) && unknown_32d) {
                if (Team) Team->TargetNotAssigned = true;
                unknown_slot210();
                Delete_Me();
                return;
            }
        }
        if (Type->SmallVisceroid || Type->LargeVisceroid) unknown_6fbf40();
        unknown_slot239();
        if (Type->Passengers > 0 && !unknown_2a8.Is_Count_Down_Complete() && Get_Mission() != 16 && unknown_slot149(17) != 1) {
            unknown_2a8.Count_Down(Type->DeployTime);
        }
        if (unknown_slot122()) Commence();
        if (unknown_1b8 == 2 && unknown_5b4 != -1) Do_Shimmer();
        if ((Frame & 15) == 0 && (Mission == 11 || Mission == 5) && !House->Is_Human_Player() && MissionQueue == -1 && Strength < Type->MaxStrength && (!unknown_1b0 || Mission == 11) && !Type->Harvester && !Type->Weeder) {
            BuildingClass *repair_building = Unknown_00450F90(House, this);
            if (repair_building) {
                if (!unknown_1b0) {
                    CoordStruct position = Coord;
                    unknown_6d6e50(&Map[position]);
                }
                QueueMission(7, false);
                unknown_slot220(0);
                Assign_Destination(repair_building, true);
            }
        }
        if (Type->Harvester && !House->Is_Human_Player() && Frame - unknown_ac > 300 && !Locomotor.Checked()->Is_Moving()) {
            BuildingClass *refinery = (BuildingClass *)Map[CoordStruct(Coord)].Cell_Building();
            if (refinery && refinery->Type->Refinery) {
                if (unknown_slot104() == Unit_AI_Refinery_Exit_Cell(refinery->unknown_slot104())) {
                    ObjectClass *contact = Map[CoordStruct(Coord)].Cell_Building();
                    if (HasAnyLink()) unknown_slot149(3);
                    Assign_Destination(&Map[unknown_6cf130(contact)], true);
                    QueueMission(2, true);
                }
            }
        }
    }
}
