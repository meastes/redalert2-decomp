// void __stdcall ShipLocomotionClass::Move_To(COMCoordStruct to) (0x00675DF0, ILocomotion 17)
// Unless the linked object is busy (slots 201, 202, 111, 112), heads for
// the coordinate (raised by the bridge height on flagged cells).
#include "shared/ship_locomotion.hpp"
#include "shared/unknown_geometry.hpp"
// @ref _unknown_geometry_l=00ab91c8

void __stdcall ShipLocomotionClass::Move_To(COMCoordStruct to)
{
    if (!LinkedTo->unknown_slot201() && !LinkedTo->unknown_slot202() && !LinkedTo->unknown_slot111() && !LinkedTo->unknown_slot112()) {
        unknown_34 = CoordStruct(to.X, to.Y, to.Z);
        if (to != unknown_geometry_l) {
            to = CoordStruct(to.X, to.Y, to.Z);
            if (Loco_Cell(to)->Flags & 0x100) {
                unknown_34.Z += unknown_geometry_j;
            }
        }
    }
}
