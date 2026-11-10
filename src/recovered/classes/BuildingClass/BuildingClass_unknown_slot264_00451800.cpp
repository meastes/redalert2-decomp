// void BuildingClass::unknown_slot264() (0x00451800, slot 264)
// Calls the map's Radar_Track for each foundation offset of the type, then sets
// unknown_37b to true.
#include "shared/building.hpp"
#include "shared/gscreen.hpp"
#include "shared/building_map.hpp"
// @ref ?Map@@3VMouseClass@@A=008324e0
// @ref ?Radar_Track@RadarClass@@QAEXPAVTechnoClass@@UPoint2D@@@Z=00631910
// @ref ?Unknown_006328D0@RadarClass@@QAEPAV?$DynamicVectorClass@UPoint2D@@@@PAVBuildingTypeClass@@@Z=006328d0

void BuildingClass::unknown_slot264()
{
    DynamicVectorClass<Point2D> *offsets = Map.Unknown_006328D0(Type);
    for (int index = 0; index < offsets->Count(); index++) {
        Map.Radar_Track(this, Point_Sum((*offsets)[index], unknown_cell_1a0()));
    }
    unknown_37b = true;
}
