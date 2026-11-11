// CoordStruct __stdcall TunnelLocomotionClass::Destination() (0x006EFC70, ILocomotion 5)
// The destination while moving, otherwise where the linked object is.
#include "shared/tunnel_locomotion.hpp"

CoordStruct __stdcall TunnelLocomotionClass::Destination()
{
    if (Is_Moving()) {
        return CoordStruct(DestinationCoord.X, DestinationCoord.Y, DestinationCoord.Z);
    }
    CoordStruct coord = LinkedTo->Coord;
    return coord;
}
