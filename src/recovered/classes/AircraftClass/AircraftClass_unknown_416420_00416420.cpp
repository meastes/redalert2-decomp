// Aircraft carryall passenger placement, original 00416420, complete 592+0.
// FootClass passenger identity follows the original locomotor and virtual-slot use.
#include "shared/aircraft.hpp"
#include "shared/map_view.hpp"
#include "shared/debug_printf.hpp"
// @ref ??_C@_0DG@HIJH@Do_MISSION_MOVE_Carryall?5?9?5LAND?5@=007ce62c

void AircraftClass::unknown_416420()
{
    Debug_Printf("Do_MISSION_MOVE_Carryall - LAND - Dropping off cargo\n");
    FootClass *passenger = static_cast<FootClass *>(Cargo.Detach_Object());
    CoordStruct position = Coord;
    position.Z = Map.Get_Height_GL(position);
    UnknownCellClass *cell = reinterpret_cast<UnknownCellClass *>(&Map[position]);
    if (cell->Flags & 0x100) {
        position.Z += unknown_geometry_j;
        passenger->unknown_79 = true;
    } else {
        passenger->unknown_79 = false;
    }
    {
        passenger->Locomotor.Checked()->Release();
        passenger->Locomotor.Detach();
        passenger->Locomotor = UnknownComPtr<ILocomotion, &unknown_uuid_ILocomotion_7ce600>(
            passenger->Techno_Type_Class()->Locomotor);
    }
    passenger->Locomotor.Checked()->Link_To_Object(passenger);
    if (!passenger->unknown_slot53(&position, 0)) {
        Cargo.Attach(passenger);
        return;
    }
    {
        passenger->unknown_37c = false;
        passenger->unknown_slot263(0, 0, false, 0);
        passenger->unknown_slot262(0, 0, 0, 0);
        Map.Unknown_0054A550(CoordStruct(Coord), passenger->unknown_1f4 - 3,
                            passenger->unknown_1f4 + 3, 0);
        Cell next = unknown_6cf130(0);
        if (Contact(0) == passenger) {
            unknown_slot149(25);
            unknown_slot149(3);
        }
        if (next != unknown_geometry_k) {
            Assign_Destination(&Map[next], true);
            return;
        }
        Assign_Destination(0, true);
    }
}
