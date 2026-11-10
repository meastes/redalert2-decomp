// int __stdcall TunnelLocomotionClass::Can_Enter_Cell(Cell cell) (0x006F1230, ILocomotion 7)
// Refused (7) where the cell object's 0x0047C200 fails, unless the flag at
// 0x00A40D13 is set.
#include "shared/tunnel_locomotion.hpp"
// @ref ?Map@@3VMouseClass@@A=008324e0
// @ref ?Debug_Map@@3_NA=00a40d13
// @ref ?Can_Burrow_Here@UnknownLocoCellObject@@QAE_NXZ=0047c200

int __stdcall TunnelLocomotionClass::Can_Enter_Cell(Cell cell)
{
    if (!Debug_Map) {
        Cell copy(cell.X, cell.Y);
        if (!Loco_Cell_At(copy)->Can_Burrow_Here()) {
            return 7;
        }
    }
    return 0;
}
