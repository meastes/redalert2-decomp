#include "shared/radar.hpp"
// Complete cleanup: 57ACE0..57ADF1, followed by14 NOP bytes through57ADFF.
#include "shared/unknown_map_generator.hpp"
#include "shared/map_region.hpp"
#include "shared/map_view.hpp"
#include "shared/debug_printf.hpp"
extern DynamicVectorClass<MapRegionClass *> MapRegionClass_MapRegions;
extern "C" char unknown_rmg_radar_message[];
void MapGeneratorClass::Cleanup()
{
    SeededWaterAmount=0;
    WorkingRegionID=0;
    if(RMGCellData!=0) {
        delete [] RMGCellData;
        RMGCellData=0;
    }
    for(int index=(int)((unsigned int)MapRegionClass_MapRegions.Count()-1u);index>=0;--index) {
        MapRegionClass *region=MapRegionClass_MapRegions[index];
        if(region!=0)delete region;
    }
    unknown_map_region_next_index_a70a28=0;
    Map.Overpass();
    Debug_Printf(unknown_rmg_radar_message);
    Map.RadarClass::Set_Local_Dimensions(Map.MapLocalSize);
    Map.Compute_Radar_Image();
}
