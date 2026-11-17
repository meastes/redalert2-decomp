// Existing real definitions required by actual Count/index/delete/embedded-destructor operations.
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#define VECTOR_IMPLEMENT_DYNAMIC_DELETE_OBJECT
#define VECTOR_IMPLEMENT_DYNAMIC_DELETE_INDEX
// Actual ECX/RET0 with ignored result; member/name/void source intent remain hypotheses.
#include "shared/unknown_map_generator.hpp"
#include "shared/map_region.hpp"
#include "shared/map_view.hpp"
#include "shared/cell.hpp"
#include "shared/tileset_index.hpp"
extern DynamicVectorClass<MapRegionClass *> MapRegionClass_MapRegions;
// @ref ?MapRegionClass_MapRegions@@3V?$DynamicVectorClass@PAVMapRegionClass@@@@A=00a6fd28
// @ref ??1MapRegionClass@@QAE@XZ=0056d970
// @ref ?Seed_Land@MapGeneratorClass@@=0057bea0
// @ref ?Seed_Land_Type0@MapGeneratorClass@@=0057c4f0
// @ref ?Seed_Land_Type1@MapGeneratorClass@@=0057c780
// @ref ?Seed_Land_Type2@MapGeneratorClass@@=0057c9e0
// @ref ?Unknown_0055C2C0@MapClass@@=0055c2c0
// R109 proves this adopted-pointer path; global dynamic identity remains qualified.
MapRegionClass::~MapRegionClass()
{
    if(unknown_00!=0) {
        delete static_cast<DynamicVectorClass<Cell> *>(unknown_00);
        unknown_00=0;
    }
    MapRegionClass_MapRegions.Delete(this);
    // Cells member destruction is automatic; no outer storage release here.
}
void MapGeneratorClass::Seed_Land()
{
    Map.Reset_Iterator();
    CellClass *cell=Map.Iterate();
    while(cell!=0) {
        cell->IsoTileIndex=IsometricTileTypeClass_WaterSet;
        cell=Map.Iterate();
    }
    switch(SeedData.MapType) {
    case 0: Seed_Land_Type0();break;
    case 1: Seed_Land_Type1();break;
    case 2: Seed_Land_Type2();break;
    }
    Map.Reset_Iterator();
    cell=Map.Iterate();
    while(cell!=0) {
        if(cell->IsoTileIndex==IsometricTileTypeClass_WaterSet) {
            bool surrounded=true;
            for(int direction=0;direction<8;direction+=2) {
                if(!cell->Adjacent_Cell(direction).Is_Tile_Clear())surrounded=false;
            }
            if(surrounded)cell->IsoTileIndex=0;
        }
        cell=Map.Iterate();
    }
    Map.Unknown_0055C2C0(0,0);
    if(RMGCellData!=0) {
        int remaining=(int)((unsigned int)MapCellStride*(unsigned int)MapCellStride);
        for(int index=0;remaining>0;--remaining,++index) {
            RMGCellData[index].RegionIndex=-1;
            RMGCellData[index].unknown_3c=-1;
        }
    }
    for(int reverse=(int)((unsigned int)MapRegionClass_MapRegions.Count()-1u);reverse>=0;--reverse) {
        delete MapRegionClass_MapRegions[reverse];
    }
    unknown_map_region_next_index_a70a28=0;
    Map.Reset_Iterator();
    cell=Map.Iterate();
    while(cell!=0) {
        if(cell->Is_Tile_Shore()) {
            for(int direction=0;direction<8;direction+=2) {
                CellClass &neighbor=cell->Adjacent_Cell(direction);
                if(neighbor.Is_Tile_Clear())neighbor.IsoTileIndex=IsometricTileTypeClass_GreenTile;
            }
        }
        cell=Map.Iterate();
    }
}
