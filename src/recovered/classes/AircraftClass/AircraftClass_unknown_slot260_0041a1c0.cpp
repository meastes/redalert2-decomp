// Aircraft destination setter 0041A1C0: original831+1; current existing shared view.
// Complete original 831+1, vtable260, independently bounded by vtable29/41A500.
// Uses existing canonical UnknownCellClass Flags+140/unknown_e4+E4; no header overlay.
// Class/operation names remain hypotheses; original ordered calls are preserved.
#include "shared/aircraft.hpp"
#include "shared/house.hpp"
// @ref ?Is_Radio_Contact@RadioClass@@QBE_NPAVTechnoClass@@@Z=00636e30
// @ref ?Assign_Destination@FootClass@@UAEXPAVAbstractClass@@_N@Z=004c8030
// @ref ?unknown_4198b0@AircraftClass@@QAEPAVAbstractClass@@XZ=004198b0
// @ref ?unknown_6d6e50@TechnoClass@@QAEXPAVAbstractClass@@@Z=006d6e50
// @ref ?Unknown_0051FC80@HouseClass@@QAE_NXZ=0051fc80

void AircraftClass::Assign_Destination(AbstractClass *target, bool immediate)
{
    if (target) {
        if (target->In_Air()) {
            target = 0;
            goto set_destination;
        }
        if (target->What_Am_I() == 6 && (Get_Mission() == 7 || MissionQueue == 7)) {
            BuildingClass *building = static_cast<BuildingClass *>(target);
            if (!In_Radio_Contact()) {
                if (!building->Unknown_00636ED0(this)) {
                    unknown_6d6e50(target);
                    if (building->Type->unknown_1365) {
                        TechnoClass *dock = unknown_slot299(
                            (DynamicVectorClass<TechnoTypeClass *> const *)&Type->Dock, 0, 0);
                        Assign_Destination(0, true);
                        int next_mission = 2;
                        if (dock && unknown_slot150(15, dock) == 1) {
                            unknown_slot150(2, dock);
                            target = dock;
                            next_mission = 7;
                        } else {
                            target = unknown_4198b0();
                        }
                        QueueMission(next_mission, false);
                        if (unknown_slot122()) Commence();
                    }
                    if (building->Type->unknown_1349) {
                        unknown_3f8 = building;
                        target = 0;
                    }
                } else {
                    if (unknown_slot150(14, building) != 1) {
                        unknown_slot149(3);
                        if (building->Type->unknown_1349 || building->Type->unknown_134a) {
                            unknown_6d6e50(target);
                            target = 0;
                        }
                    }
                    if (!building->Type->unknown_1349 && !building->Type->unknown_134a) {
                        unknown_6d6e50(target);
                    } else if (unknown_494) {
                        unknown_6d6e50(unknown_494);
                    }
                }
            } else {
                TechnoClass *techno = target->As_Techno();
                if (techno) {
                    if (!techno->Unknown_00636ED0(this)) {
                        unknown_6d6e50(target);
                    } else if (!techno->Is_Radio_Contact(this)) {
                        if (unknown_slot149(14) == 1) return;
                        unknown_slot149(3);
                        if (building->Type->unknown_1349 || building->Type->unknown_134a) {
                            unknown_6d6e50(target);
                            target = 0;
                        }
                    }
                }
            }
        }
        UnknownCellClass *cell = reinterpret_cast<UnknownCellClass *>(Get_Cell_Ptr());
        if (!(cell->Flags & 0x100)) {
            ObjectClass *occupier = cell->unknown_e4;
            while (occupier && (occupier == this || occupier->Kind_Of() != 6)) {
                occupier = occupier->unknown_30;
            }
            if (occupier) {
                BuildingClass *building = static_cast<BuildingClass *>(occupier);
                if (building->Type->unknown_1349 || building->Type->unknown_134a) {
                    if (!Locomotor.Checked()->Is_Powered() && !House->Unknown_0051FC80()) {
                        Locomotor.Checked()->Power_On();
                    }
                    if (Contact(0) == building && target != Contact(0)) unknown_slot149(3);
                }
            }
        }
    }
set_destination:
    FootClass::Assign_Destination(target, immediate);
}
