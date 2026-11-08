namespace seed_0056dba0 { int __fastcall FUN_0056dba0(short *position); }
#include "shared/map_region.hpp"
#include "shared/unknown_map_generator.hpp"
#include "shared/isometric_tile_type.hpp"
#include "shared/terrain.hpp"
#define RA2_VEIN_HEAP_REMOVE_MIN_IMPLEMENTATION
#include "shared/com_persist.hpp"
#include "shared/vein_heap.hpp"
// @ref ?Sift_Down@UnknownVeinHeap@@=0058f090
// @ref ??_GUnknownVeinHeap@@=0058db20
// @ref ??0TerrainClass@@=006e33d0
// @ref ?From_Name@TerrainTypeClass@@=006e5510
// @ref ?TerrainTypes@@=00a40320
bool __fastcall Generation_Cell_In_Bounds_0058da10(Cell const &position);
double __stdcall Randomized_Cell_Distance_005862c0(Cell const &,Cell const &);
extern char const unknown_forest_tree_format_7e1b54[];
// @ref ?unknown_forest_tree_format_7e1b54@@=007e1b54
#include "shared/random.hpp"
#include "shared/map_random.hpp"
// @ref ?unknown_map_random_a705c0@@3VRandom2Class@@A=00a705c0
// @ref ??RRandom2Class@@QAEHXZ=00638840
// @ref __real@8@3fdf8000000080000000=007a5dd0
// Random map callback: map the generator's complete unsigned word to [0,1].
double __cdecl Map_Random_Fraction_00579770();

int UnknownMapGenerator::Place_Forest_00585dc0(CellClass *origin,int count,double density)
{
    int capacity=(int)((unsigned)count*25u);
    UnknownVeinNode *nodes=new UnknownVeinNode[capacity];
    UnknownVeinHeap *heap=new UnknownVeinHeap(capacity);
    heap->Clear();
    int used=1;
    float &seedScore=nodes[0].Key;
    nodes[0].Position=Cell(origin->MapCoords);
    seedScore=0.0f;
    Cell position=Cell(origin->MapCoords);
    (*reinterpret_cast<UnknownMapGenerationCell *>(seed_0056dba0::FUN_0056dba0((short *)&position))).unknown_45[2]=1;
    heap->Insert(nodes);
    UnknownVeinNode *current=heap->Remove_Min();
    int planted=0;
    while(planted<count && current && used<capacity) {
        CellClass *cell=&Map[current->Position];
        if(cell->unknown_47b590() && cell->OccupierPtr==0 && cell->OverlayIndex==-1 && cell->unknown_ec!=3) {
            if(Map_Random_Fraction_00579770()<density) {
                char treeName[20];
                int tree=(int)Map_Random_Range_005797a0(1,25);
                sprintf(treeName,unknown_forest_tree_format_7e1b54,tree/10,tree%10);
                new TerrainClass(TerrainTypes[TerrainTypeClass::From_Name(treeName)],Cell(cell->MapCoords));
                ++planted;
            }
        }
        int facing=0;
        UnknownVeinNode *nextNode=nodes+used;
        for(;facing<8;++facing) {
            Cell next=current->Position + AdjacentCell[facing & 7];
            if(Generation_Cell_In_Bounds_0058da10(next)) {
                UnknownMapGenerationCell &data=(*reinterpret_cast<UnknownMapGenerationCell *>(seed_0056dba0::FUN_0056dba0((short *)&next)));
                if(!data.unknown_45[2] && !data.unknown_45[0] && used<capacity) {
                    nextNode->Position=next;
                    Cell originPosition=Cell(origin->MapCoords);
                    nextNode->Key=(float)Randomized_Cell_Distance_005862c0(originPosition,next);
                    data.unknown_45[2]=1;
                    ++used;
                    heap->Insert(nextNode++);
                }
            }
        }
        current=heap->Remove_Min();
    }
    delete[] nodes;
    delete heap;
    return planted;
}
// @ref __real@8@3fe3c8000000c8000000=007a6018
