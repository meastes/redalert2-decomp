// NON-EXACT DRAFT 0x576420 FUN_00576420
// entry 0x576420; exact-match score 84.24 (misses_index best), last lane score 64.79; plan "draft, verify"
// origin lane: Scout677 (build/class-work/Scout677/w/keep/start_points_576420_draft.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1061 (native/drafts.json). Not original bytes.
// Divides the starting positions between the regions large enough to hold them, largest first.
#include "shared/unknown_map_generator.hpp"
#include "shared/map_region.hpp"
#include "shared/map_view.hpp"
#include "shared/map.hpp"
#include "shared/ini_class.hpp"
#include "shared/map_random.hpp"
#include "shared/random2.hpp"
#include "shared/vector_class.hpp"
#include "shared/fast_math.hpp"
extern DynamicVectorClass<MapRegionClass *> unknown_map_regions_a6fd28;
extern double const unknown_region_scale_7a5e10,unknown_region_minimum_7a5e08;
extern double const unknown_random_scale_7a5dd0;
bool __fastcall Generation_Cell_In_Bounds_0058da10(Cell const &position);
void __fastcall Grow_Zone_Region_00575d20(CellClass *cell,int zone_type,int zone);
// @ref ?unknown_region_scale_7a5e10@@=007a5e10
// @ref ?unknown_region_minimum_7a5e08@@=007a5e08
// @ref ?Grow_Zone_Region_00575d20@@=00575d20
bool __cdecl Generate_Starting_Points_00576420()
{
    int zone_types[5]={5,5,5,0,0};
    double size=(double)unknown_map_generator_a6fd68.unknown_184*unknown_map_generator_a6fd68.unknown_180*unknown_region_scale_7a5e10;
    if(unknown_region_minimum_7a5e08>size) {
        size=unknown_region_minimum_7a5e08;
    }
    int minimum=size;
    int largest=Map.Unknown_0054EBD0();
    int zone_type=zone_types[unknown_map_generator_a6fd68.SeedOptions.MapType];
    int main_zone=((unsigned short *)Map.unknown_18[zone_type])[largest];
    MapRegionClass::Destroy_Cell_Regions();
    Map.Unknown_0055A550();
    for(CellClass *cell=Map.Unknown_0055A490();cell!=0;cell=Map.Unknown_0055A490()) {
        if(Generation_Cell_In_Bounds_0058da10(cell->MapCoords)) {
            if(MapRegionClass::Get_Cell_Data(cell->MapCoords).RegionIndex==-1 && cell->unknown_4c==0) {
                if(Map.Zone(cell->MapCoords,zone_type,false)==main_zone) {
                    Grow_Zone_Region_00575d20(cell,zone_type,main_zone);
                }
            }
        }
    }
    int count=unknown_map_regions_a6fd28.Count();
    for(int i=count-1;i>=0;i--) {
        MapRegionClass *region=unknown_map_regions_a6fd28[i];
        if(region->CellCount<minimum) {
            delete region;
        }
    }
    DynamicVectorClass<MapRegionClass *> sorted;
    IndexClass<int,MapRegionClass *> index;
    sorted.Clear();
    for(i=0;i<unknown_map_regions_a6fd28.Count();i++) {
        MapRegionClass *const &region=unknown_map_regions_a6fd28[i];
        index.Add_Index((500000-region->CellCount)*count+region->Index,region);
    }
    for(i=0;i<index.Count();i++) {
        sorted.Add(index.Fetch_By_Position(i));
    }
    int total=0;
    for(i=0;i<sorted.Count();i++) {
        total+=sorted[i]->CellCount;
    }
    int starts=unknown_map_generator_a6fd68.SeedOptions.NumPlayers;
    int running=0;
    int assigned=0;
    for(i=0;i<sorted.Count();i++) {
        MapRegionClass *region=sorted[i];
        running+=region->CellCount;
        int share;
        if(i==sorted.Count()-1) {
            share=starts-assigned;
        } else {
            share=(double)running/total*starts+0.5-assigned;
        }
        region->unknown_20=share;
        assigned+=share;
    }
    int first=0;
    for(i=0;i<sorted.Count();i++) {
        sorted[i]->Generate_Starting_Points_00576170(first);
        first+=sorted[i]->unknown_20;
    }
    if(unknown_map_generator_a6fd68.SeedOptions.MapType==0) {
        for(i=0;i<sorted.Count();i++) {
            if(sorted[i]->unknown_20==0 && unknown_map_random_a705c0()*unknown_random_scale_7a5dd0<0.5) {
                sorted[i]->Place_Neutral_Tech_Building_00576cd0();
            }
        }
    }
    return true;
}
