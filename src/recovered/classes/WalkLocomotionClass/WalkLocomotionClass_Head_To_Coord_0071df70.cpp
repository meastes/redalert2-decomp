// CoordStruct __stdcall WalkLocomotionClass::Head_To_Coord() (0x0071DF70, ILocomotion 6)
#include "shared/walk_locomotion.hpp"
// @ref _unknown_geometry_l=00af9060

CoordStruct __stdcall WalkLocomotionClass::Head_To_Coord()
{
    if (unknown_28 != unknown_geometry_l) {
        return CoordStruct(unknown_28.X, unknown_28.Y, unknown_28.Z);
    }
    CoordStruct coord = LinkedTo->Coord;
    return coord;
}
