// void MapClass::Fresh_Map() (0x005499C0)
// Rebuilds the zone data for the map size: frees and reallocates the +0x64
// elements ((width + height + 1) squared, +0x68) and the +0x6C buffer (ten
// bytes per element), sizes the path estimator for the map rectangle, sets
// the three subzone tracking vectors' growth steps (width * height * 4 over
// 4, 16 and 64), runs 0x0054B2E0, 0x0054FDA0 and 0x0054EBD0 and rebuilds the
// subzone tracking (0x00563D40).
#include "shared/map.hpp"
#include "shared/path_estimator.hpp"
// @ref ??2@YAPAXI@Z=00782017
// @ref ??3@YAXPAX@Z=00781d3b
// @ref ?Search@@3UUnknownPathEstimator@@A=008315c8
// @ref ?Update_Map_Dimensions@AStarClass@@QAEXABVRect@@@Z=004284e0
// @ref ?Overpass@MapClass@@QAEHXZ=0054b2e0
// @ref ?Compute_Zone_Connections@MapClass@@QAEXXZ=0054fda0
// @ref ?Zone_Reset@MapClass@@QAEHXZ=0054ebd0

void MapClass::Fresh_Map()
{
    if (CellZones != 0) {
        delete CellZones;
        CellZones = 0;
    }
    if (CellSubzones != 0) {
        delete CellSubzones;
        CellSubzones = 0;
    }
    int size = MapSize.Width + MapSize.Height + 1;
    CellZoneCount = size * size;
    CellZones = new UnknownMapElem4[CellZoneCount];
    CellSubzones = new char[CellZoneCount * 10];
    Search.Update_Map_Dimensions(MapSize);
    for (int index = 0; index < 3; index++) {
        int scale = 1 << (index + 1);
        SubzoneTracking[index].Set_Growth_Step(MapSize.Width * MapSize.Height * 4 / (scale * scale));
    }
    Overpass();
    Compute_Zone_Connections();
    Zone_Reset();
    Reset_All_Subzones();
}
