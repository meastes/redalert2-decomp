#include "shared/tunnel_locomotion.hpp"
// @ref ?Rules@@3PAVRulesClass@@A=00839848
// @ref __ftol=0077f110

void TunnelLocomotionClass::Process_Descending()
{
    CoordStruct coord = LinkedTo->Coord;
    if (coord.Z > -0x100) {
        int rate = (int)(LinkedTo->unknown_slot303() * Rules->unknown_20);
        if (rate <= 5) {
            rate = 5;
        }
        coord.Z -= rate;
        if (coord.Z < -0x100) {
            coord.Z = -0x100;
        }
        LinkedTo->unknown_slot103(coord);
    } else {
        LinkedTo->Mark(0);
        unknown_18 = 4;
    }
}

