// void __stdcall TunnelLocomotionClass::Move_To(CoordStruct to) (0x006EFCD0, ILocomotion 17)
#include "shared/tunnel_locomotion.hpp"
// @ref _unknown_geometry_l=00ac0f30

void __stdcall TunnelLocomotionClass::Move_To(CoordStruct to)
{
    if (!LinkedTo->unknown_slot201() && !LinkedTo->unknown_slot202() && !LinkedTo->unknown_slot111()
        && !LinkedTo->unknown_slot112()) {
        CoordStruct from = to;
        CoordStruct copy = from;
        DestinationCoord = LinkedTo->Class_Of()->unknown_slot27(copy);
        if (from != unknown_geometry_l) {
            if (LinkedTo->unknown_slot108() < 0 && unknown_18 != 3 && unknown_18 != 5 && unknown_18 != 1
                && unknown_18 != 6 && unknown_18 != 2) {
                LinkedTo->Mark(0);
                unknown_18 = 4;
            }
        }
        if (unknown_18 == 0) {
            unknown_18 = 1;
        }
    }
}
