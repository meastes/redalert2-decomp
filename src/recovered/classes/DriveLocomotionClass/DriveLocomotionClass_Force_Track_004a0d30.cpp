// void __stdcall DriveLocomotionClass::Force_Track(int track, CoordStruct coord) (0x004A0D30, ILocomotion 27)
// Puts the unit on a track at its start; with a coordinate it heads there
// (Start_Driver) and, when that succeeds, it is the destination at full speed.
#include "shared/drive_locomotion.hpp"
#include "shared/cell.hpp"
// @ref ?unknown_476cc0@CellClass@@QAE_NPAVObjectClass@@@Z=00476cc0
// @ref __real@8@3fff8000000000000000=0079a708
// @ref _unknown_geometry_l=00852998

void __stdcall DriveLocomotionClass::Force_Track(int track, CoordStruct coord)
{
    unknown_58 = track;
    unknown_5c = 0;
    if (coord != unknown_geometry_l) {
        if (Start_Driver(CoordStruct(coord.X, coord.Y, coord.Z))) {
            unknown_34 = coord;
            unknown_50 = 1.0;
        }
    }
}
