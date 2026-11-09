// Cell TeamTypeClass::Waypoint_Location() const (0x006BFA90)
// The cell of the team's waypoint; the "no cell" value without one.
#include "shared/team_type.hpp"
#include "shared/coord.hpp"
// @ref ?Get_Waypoint_Location@UnknownIDSource@@QBE?AVCell@@H@Z=00663180
// @ref ?unknown_id_source_a3d290@@3PAVUnknownIDSource@@A=00a3d290
// @ref _unknown_geometry_k=00ac00c8

// The "no cell" position (0x00AC00C8; hypothesis).

Cell TeamTypeClass::Waypoint_Location() const
{
    if (Waypoint == -1) {
        return unknown_geometry_k;
    }
    return unknown_id_source_a3d290->Get_Waypoint_Location(Waypoint);
}
