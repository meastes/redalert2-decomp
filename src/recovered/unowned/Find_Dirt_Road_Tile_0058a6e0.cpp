// Select a dirt-road tile whose link faces the target and whose footprint fits.
#include "shared/coord.hpp"
#include "shared/map_generation_path.hpp"
#include "shared/random2.hpp"
#include "shared/map_random.hpp"
#include "shared/dir_type.hpp"
// @ref ?unknown_road_counts_7e1064@@=007e1064
// @ref ?unknown_road_library_7e0fe4@@=007e0fe4
// @ref ?random_unit_7a5dd0@@=007a5dd0
// @ref ?unknown_road_bias_7a5d70@@=007a5d70
extern double const unknown_road_bias_7a5d70;
// @ref __real@8@4001e000000000000000=007a05c8

extern double const random_unit_7a5dd0;
extern int unknown_road_counts_7e1064[8][4];
extern int *unknown_road_library_7e0fe4[8][4];
extern UnknownMapGenerationPath unknown_map_paths_a700a0[109];
double __stdcall Map_Clamped_Gaussian_005798f0(double mean,double deviation,double minimum,double maximum);
unsigned int __fastcall Map_Random_Range_005797a0(unsigned int minimum,unsigned int maximum);
bool __fastcall Can_Use_Tile_Footprint_005894e0(int tile,Cell const &position,unsigned char check_mark,unsigned char reject_outside);
bool __fastcall Find_Dirt_Road_Tile_0058a6e0(int facing,Cell const &target_cell,int max_connections,unsigned int &tile,Cell &origin,int &link_index)
{
    int current=1;
    int initial=1;
    if(max_connections>1) {
        double roll=Map_Clamped_Gaussian_005798f0(2.0,0.5,2.0,max_connections);
        current=roll;
        initial=current;
    }
    while(true) {
        int tile_count=unknown_road_counts_7e1064[facing][current-1];
        int *tile_list=unknown_road_library_7e0fe4[facing][current-1];
        int start_index;
        if(current==2 && (unsigned)unknown_map_random_a705c0()*random_unit_7a5dd0<unknown_road_bias_7a5d70) {
            start_index=Map_Random_Range_005797a0(7,tile_count-1);
        } else {
            start_index=Map_Random_Range_005797a0(0,tile_count-1);
        }
        for(int i=0;i<tile_count;i++) {
            UnknownMapGenerationPath &path=unknown_map_paths_a700a0[tile_list[(i+start_index)%tile_count]];
            int index;
            for(index=0;index<path.Count;index++) {
                UnknownMapGenerationPoint connection=path.Points[index];
                if(connection.unknown_04==facing) {
                    break;
                }
            }
            UnknownMapGenerationPoint link;
            link.Position=path.Points[index].Position;
            Cell position=target_cell-link.Position;
            if(Can_Use_Tile_Footprint_005894e0(path.unknown_08,position,1,1)) {
                UnknownMapGenerationPoint link2;
                link2.Position=path.Points[index].Position;
                origin=target_cell-link2.Position;
                tile=path.unknown_08;
                link_index=index;
                return true;
            }
        }
        current=(current+1)%(max_connections+1);
        if(current==0) {
            current=1;
        }
        if(current==initial) {
            tile=0xffff;
            origin=Cell(0,0);
            link_index=-1;
            return false;
        }
    }
}
