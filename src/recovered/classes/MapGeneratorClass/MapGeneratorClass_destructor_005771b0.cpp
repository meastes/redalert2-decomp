// @ref ?Clear@?$VectorClass@PBVBuildingTypeClass@@@@UAEXXZ=004f7100
#include "shared/unknown_map_generator.hpp"
#include "shared/map_region.hpp"
#include "shared/map_preview.hpp"
extern DynamicVectorClass<MapRegionClass *> unknown_map_regions_a6fd28;
// @ref ??1MapGeneratorClass@@QAE@XZ=005771b0
// @ref ??1MapRegionClass@@QAE@XZ=0056d970
// @ref ??1MapPreviewClass@@QAE@XZ=0061d440
MapGeneratorClass::~MapGeneratorClass()
{
    MapRegionClass::Destroy_Cell_Regions();
    if(unknown_map_cells_a70a24!=0) {
        delete[] unknown_map_cells_a70a24;
        unknown_map_cells_a70a24=0;
    }
    if(MapSeeder!=0) {
        delete reinterpret_cast<MapSeedClass *>(MapSeeder);
        MapSeeder=0;
    }
    if(MapPreview!=0) {
        delete reinterpret_cast<MapPreviewClass *>(MapPreview);
        MapPreview=0;
    }
}
