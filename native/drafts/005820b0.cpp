// NON-EXACT DRAFT 0x5820B0 FUN_005820b0
// entry 0x5820B0; exact-match score 90.35 (misses_index best), this text 84.09; plan "review: rewrite likely"
// origin lane: Scout638 (build/class-work/Scout638/keep/gw_m3_8409.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1069 (native/drafts.json). Not original bytes.
void Clear_Map_Cell_Traversal_Marks_00582a50();
// @ref __real@8@3ffec90fdaa22168c000=0079ca80
#include "shared/unknown_map_generator.hpp"
#include "shared/map_region.hpp"
#include "shared/map_view.hpp"
#include "shared/cell.hpp"
#include "shared/vein_heap_core.hpp"
#include "shared/random2.hpp"
#include "shared/map_gaussian.hpp"
#include "shared/fast_math.hpp"
#include "shared/crt_math.hpp"
extern "C" double __cdecl log(double);
#pragma intrinsic(log)
extern Random2Class unknown_map_random_a705c0;
extern double const angle_half_pi_79b630,angle_pi_79d158,angle_turn_79c9b8;
extern double const angle_weight_7a5cd0,jitter_weight_7a5cc8,random_unit_7a5dd0;
// @ref ?angle_half_pi_79b630@@=0079b630
// @ref ?angle_pi_79d158@@=0079d158
// @ref ?angle_turn_79c9b8@@=0079c9b8
// @ref ?angle_weight_7a5cd0@@=007a5cd0
// @ref ?jitter_weight_7a5cc8@@=007a5cc8
// @ref ?random_unit_7a5dd0@@=007a5dd0
// @ref ?unknown_map_random_a705c0@@=00a705c0
// @ref ??RRandom2Class@@QAEHXZ=00638840
bool __fastcall Generation_Cell_In_Bounds_0058da10(Cell const &);
// @ref ?unknown_gaussian_value_005798d0@@=005798d0
double __stdcall unknown_gaussian_value_005798d0(double mean,double deviation);
unsigned __fastcall Map_Random_Range_005797a0(unsigned,unsigned);

bool MapGeneratorClass::Grow_Water_Region_005820b0(int region,float scale,Rect const &bounds,Cell const &origin,bool frontier)
{
    int capacity=(int)((unsigned)LocalWidth*(unsigned)LocalHeight*2u);
    UnknownVeinNode *nodes=new UnknownVeinNode[capacity>100?capacity:100];
    UnknownVeinHeap *queue=new UnknownVeinHeap((int)((unsigned)LocalWidth*(unsigned)LocalHeight*2u)>100?(int)((unsigned)LocalWidth*(unsigned)LocalHeight*2u):100);
    int used=0;
    bool success=true;
    double angle=0.0;
    if(bounds.X!=0)angle=0.0;
    else if(bounds.Width!=512)angle=3.141592653589793;
    if(bounds.Y!=0)angle=4.71238898038469;
    else if(bounds.Height!=512)angle=1.5707963267948966;
    Clear_Map_Cell_Traversal_Marks_00582a50();
    DynamicVectorClass<Cell> *seeds=Build_Region_Border_Cell_List_00581ee0(region);
    int seed_count=seeds->Count();
    int index=0;
    if(seed_count>0) {
        UnknownVeinNode *seedNode=nodes;
        do {
        Cell position=(*seeds)[index];
        if(bounds.Is_Point_Within(Point2D(position.X,position.Y))) {
            float &seedScore=seedNode->Key;
            seedNode->Position=position;
            seedScore=Angle_Score_00582970(origin,position,angle);
            ++used;
            queue->Insert(seedNode++);
        }
        } while(++index<seed_count);
    }
    UnknownVeinNode *next;
    delete seeds;
    double logarithm=log((double)seed_count);
    if(1.0>logarithm)logarithm=1.0;
    int limit=(int)(1.0f/((float)(1.0/logarithm)*scale)*0.5f);
    limit+=Map_Random_Range_005797a0(0,limit/2);
    UnknownVeinNode *node=queue->Remove_Min();
    int step=0;
    if(limit>0) {
      while(true) {
        if(!success || !node)break;
        Cell const &position=node->Position;
        UnknownMapGenerationCell &data=MapRegionClass::Get_Cell_Data(position);
        if(data.RegionIndex==0 && Map[position].unknown_47b590()) {
            data.unknown_4b=1;
            data.RegionIndex=region;
        }
        next=&nodes[used];
        for(int direction=0;direction<8;direction+=2) {
            Cell adjacent=CellClass::Adjacent_Cell(position,direction);
            if(Generation_Cell_In_Bounds_0058da10(adjacent)) {
                if(Get_Cell_Data_Region_005818a0(adjacent)==0 && Get_Cell_Data_Spread_00581910(adjacent)!=region && bounds.Is_Point_Within(Point2D(adjacent.X,adjacent.Y)) && Map[adjacent].unknown_47b590()) {
                    next->Position=adjacent;
                    next->Key=Angle_Score_00582970(origin,adjacent,angle);
                    Set_Cell_Data_Spread_005818e0(adjacent,region);
                    ++used;
                    queue->Insert(next++);
                } else {
                    int other=UnknownMapGenerationCell::Get_Region(adjacent);
                    if(other!=0 && other!=region)success=false;
                }
            }
        }
        ++step;
        angle+=unknown_gaussian_value_005798d0(0.0,0.7853981633974483);
        node=queue->Remove_Min();
        if(step>=limit)break;
      }
    }
    if(frontier) {
        UnknownVeinNode *remaining=queue->Remove_Min();
        while(remaining) {
            if(!success)break;
            int other=UnknownMapGenerationCell::Get_Region(remaining->Position);
            if(other==0)Set_Cell_Data_Region_00581870(remaining->Position,region);
            else if(other!=region)success=false;
            remaining=queue->Remove_Min();
        }
    }
    delete[] nodes;
    delete queue;
    return success;
}
