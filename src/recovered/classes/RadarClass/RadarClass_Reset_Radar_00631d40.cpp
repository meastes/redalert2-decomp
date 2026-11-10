// void RadarClass::Reset_Radar() (0x00631D40)
// Rebuilds the radar: a fresh tracking table, the radar refitted to the
// map's local area (slot 29), 0x00630A00, and the +0x37B flag of every
// object in the list at 0x00A40C20 cleared.
#include "shared/radar.hpp"
#include "shared/map_view.hpp"
#include "shared/techno.hpp"
// @ref ?Map@@3VMouseClass@@A=008324e0
// @ref ?Unknown_00631C60@@YIHABURadarTrackingStruct@@@Z=00631c60
// @ref ??2@YAPAXI@Z=00782017
// @ref ??3@YAXPAX@Z=00781d3b
// @ref ?Compute_Radar_Image@RadarClass@@QAEXXZ=00630a00
// @ref ?Set_Local_Dimensions@RadarClass@@UAEXABVRect@@@Z=00630840
// @ref ?unknown_list_a40c20@@3V?$DynamicVectorClass@PAVTechnoClass@@@@A=00a40c20


void RadarClass::Reset_Radar()
{
    delete unknown_1254;
    unknown_1254 = New_Tracking_Table(256, 10);
    RadarClass *map = &Map;
    map->RadarClass::Set_Local_Dimensions(map->MapLocalSize);
    Compute_Radar_Image();
    for (int index = 0; index < unknown_list_a40c20.Count(); index++) {
        unknown_list_a40c20[index]->unknown_37b = false;
    }
}
