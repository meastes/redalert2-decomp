// bool CellClass::Is_Bridge_Here() const (0x004788D0)
// TibSun's IsUnderBridge read as a member function (bit 8 of +0x140); Preview_Cell_Color
// 0x004710D0 expands it, TeleportLocomotionClass 0x006DFF00 takes its address.
#include "shared/cell.hpp"

bool CellClass::Is_Bridge_Here() const
{
    return IsUnderBridge;
}
