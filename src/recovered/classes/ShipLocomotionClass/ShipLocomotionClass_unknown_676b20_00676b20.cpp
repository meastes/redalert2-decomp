// void ShipLocomotionClass::unknown_676b20(CoordStruct const &headto, int mark) (0x00676B20)
// Marks (slot 58) or clears (slot 59, mark 0) the cell ahead on the current
// track and the head-to coordinate (Westwood's DriveClass::Mark_Track).
#include "shared/ship_locomotion.hpp"
// @ref ?ShipRawTracks@@3QBURawTrackStruct@@B=007aad20
// @ref ?ShipTrackControl@@3QBUTrackControlStruct@@B=007aadf0
// @ref _unknown_geometry_l=00ab91c8

void ShipLocomotionClass::unknown_676b20(CoordStruct const &headto, int mark)
{
    if (headto == unknown_geometry_l) {
        return;
    }
    if (!unknown_60 && unknown_58 != -1) {
        int track = ShipTrackControl[unknown_58].Track;
        if (track) {
            TrackType const *points = ShipRawTracks[track - 1].Track;
            int cell = ShipRawTracks[track - 1].Cell;
            if (cell > -1) {
                int facing = points[cell].Facing;
                if (unknown_5c < cell) {
                    Point2D offset = points[cell].Offset;
                    Point2D point = Smooth_Turn(offset, facing);
                    if (mark == 0) {
                        CoordStruct coord(point.X, point.Y, LinkedTo->Get_Height());
                        LinkedTo->Clear_Occupy_Bit(coord);
                    } else if (mark == 1 || mark == 3) {
                        CoordStruct coord(point.X, point.Y, LinkedTo->Get_Height());
                        LinkedTo->Set_Occupy_Bit(coord);
                    }
                }
            }
        }
    }
    if (mark == 0) {
        LinkedTo->Clear_Occupy_Bit(headto);
    } else if (mark == 1 || mark == 3) {
        LinkedTo->Set_Occupy_Bit(headto);
    }
}
