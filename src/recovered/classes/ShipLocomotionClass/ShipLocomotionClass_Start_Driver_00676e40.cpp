// bool ShipLocomotionClass::Start_Driver(CoordStruct const &headto) (0x00676E40)
// Heads for a coordinate: taken (and its track marked) when the object may
// stand in its cell; otherwise dropped again when +0x7D is set (Westwood's
// DriveClass::Start_Driver). Expanded where it is used; this out-of-line
// copy is never called.
#include "shared/ship_locomotion.hpp"
#include "shared/cell.hpp"
// @ref ?unknown_476cc0@CellClass@@QAE_NPAVObjectClass@@@Z=00476cc0
// @ref _unknown_geometry_l=00ab91c8

bool ShipLocomotionClass::Start_Driver(CoordStruct const &headto)
{
    Stop_Driver();
    if (headto != unknown_geometry_l) {
        unknown_40 = headto;
        unknown_63 = true;
        if (Map[headto].unknown_476cc0(LinkedTo) && !LinkedTo->unknown_76) {
            unknown_676b20(headto, 1);
            return true;
        }
        if (LinkedTo->unknown_7d) {
            unknown_40 = unknown_geometry_l;
            unknown_63 = false;
        }
    }
    return false;
}
