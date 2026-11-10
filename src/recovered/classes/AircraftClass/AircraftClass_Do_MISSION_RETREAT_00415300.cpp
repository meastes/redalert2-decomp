// int AircraftClass::Do_MISSION_RETREAT() (0x00415300, slot 134)
// Without +0x494: target the cell the map finds on the house's edge (+0x128,
// 0 when out of range) unless it finds none. With +0x494: clear the target
// once slot 105 answers it. Always 3.
#include "shared/aircraft.hpp"
#include "shared/map_view.hpp"
// @ref ?Map@@3VMouseClass@@A=008324e0
// @ref ?Calculated_Cell@DisplayClass@@QAE?AVCell@@HABV2@0H_NH@Z=0049a700
// @ref _unknown_geometry_k=0083c478

int AircraftClass::Do_MISSION_RETREAT()
{
    if (unknown_494 == 0) {
        int edge = House->unknown_128;
        if (edge < 0 || edge > 3) {
            edge = 0;
        }
        Cell cell = Map.Calculated_Cell(edge, unknown_geometry_k, unknown_geometry_k, 4, 1, 0);
        if (cell.X != unknown_geometry_k.X || cell.Y != unknown_geometry_k.Y) {
            Assign_Destination((AbstractClass *)&Map[cell], true);
            return 3;
        }
    } else if (unknown_494 == unknown_slot105()) {
        Assign_Destination(0, true);
    }
    return 3;
}
