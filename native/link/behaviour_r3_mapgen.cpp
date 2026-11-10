// Behaviour row (Scout1209 r3; NOT EXACT, native/behaviour_rewrites.json): the skirmish
// Random Map's region splitting.
#define RA2_MAP_REGION_SPLIT
#define VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ADD
#define VECTOR_IMPLEMENT_DYNAMIC_DELETE_INDEX
#include "shared/rect.hpp"
#include "shared/unknown_map_generator.hpp"
#include "shared/map_region.hpp"
#include "shared/map_view.hpp"
#include "shared/cell.hpp"
#include "shared/map_gaussian.hpp"
#include "shared/random2.hpp"

unsigned __fastcall Map_Random_Range_005797a0(unsigned, unsigned);
extern DynamicVectorClass<MapRegionClass *> unknown_map_regions_a6fd28;
extern Random2Class unknown_map_random_a705c0;
double __stdcall Unknown_Cell_Score_0056dff0(Cell const &from, Cell const &to, double direction);

namespace {
UnknownMapGenerationCell &Split_Cell_Data(Cell const &cell)
{
    return unknown_map_cells_a70a24[cell.X + cell.Y * unknown_map_cell_side_84e804];
}
// The generator's map diamond (TibSun My_In_Radar; the inline test 0x0056EF20 repeats).
bool Split_In_Bounds(Cell const &cell)
{
    int x = cell.X, y = cell.Y;
    return y + x > unknown_map_min_a70a1c && x - y < unknown_map_min_a70a1c && y - x < unknown_map_min_a70a1c &&
        y + x <= unknown_map_max_a70a20;
}
// The region of a cell's generator record, -1 without records (0x0056E9A0).
int Split_Get_Region(Cell const &cell)
{
    return unknown_map_cells_a70a24 ? Split_Cell_Data(cell).RegionIndex : -1;
}
// The open list: a 1-based binary min-heap of node pointers on their float score, as the
// original's inline PriorityQueueClass<CellNode> (capacity 2 * CellCount + 10).
struct SplitNode { Cell Element; float Score; };
struct SplitHeap {
    unsigned Count = 0, Size;
    SplitNode **Nodes;
    explicit SplitHeap(unsigned size) : Size(size), Nodes(new SplitNode *[size + 1]()) {}
    ~SplitHeap() { delete[] Nodes; }
    void Insert(SplitNode *node)
    {
        unsigned index = Count + 1;
        if (index >= Size) return;                    // full: the original drops the node
        while (index > 1 && Nodes[index >> 1]->Score > node->Score) {
            Nodes[index] = Nodes[index >> 1];
            index >>= 1;
        }
        Nodes[index] = node;
        ++Count;
    }
    SplitNode *Extract_Min()
    {
        if (Count == 0) return 0;
        SplitNode *top = Nodes[1];
        Nodes[1] = Nodes[Count];
        Nodes[Count] = 0;
        --Count;
        unsigned index = 1;                           // 0x0058F090: sift down
        for (;;) {
            unsigned left = index * 2, right = left + 1, least = index;
            if (left <= Count && Nodes[left]->Score < Nodes[least]->Score) least = left;
            if (right <= Count && Nodes[right]->Score < Nodes[least]->Score) least = right;
            if (least == index) break;
            SplitNode *swap = Nodes[index];
            Nodes[index] = Nodes[least];
            Nodes[least] = swap;
            index = least;
        }
        return top;
    }
};
void Split_Clear_Spreads()
{
    int count = unknown_map_cell_side_84e804 * unknown_map_cell_side_84e804;
    for (int i = 0; i < count; ++i) unknown_map_cells_a70a24[i].unknown_3c = -1;
}
// Raises every map cell of `region` by `diff` levels (CellClass +0x11B).
void Split_Raise_Region(int region, int diff)
{
    Map.Reset_Iterator();
    for (CellClass *cell = Map.Iterate(); cell != 0; cell = Map.Iterate()) {
        if (Split_Get_Region(cell->MapCoords) == region) cell->unknown_11b = (char)(cell->unknown_11b + diff);
    }
}
}

// 0x0056EF20 MapRegionClass::Split_Region (TibSun mapgen.cpp MapRegionClass::Split_Region; the
// structure and RA2's differences from the decompile, inventory/decompiled/0056ef20.c): a region
// not marked unsplittable (+0x1A) grows a wandering boundary from a random cell of its own until
// it holds a random 1/8..1/3 of its cells, steered by Get_Angle_Score (0x0056DFF0) towards a
// direction that drifts by a normal sample times pi/8 each step (0x00579830; 0x007A5DD8). Each
// connected piece of the result (cells left at region -2 or marked -3) becomes a new region
// (0x0056D870). A new region of more than 100 cells moves to a new height picked from its
// neighbours' heights (same: 4 below / 4 above; 4 apart: either; 8 apart: the middle; with map
// types 3 and 4 a height of 0 is not offered); a smaller one takes the height of its largest
// dry (+0x14 clear) neighbour and joins it (+0x24 discardable). Every discardable region other
// than this one is deleted, the split count (+0x1C) rises and true is returned; the caller
// (Make_Ice_Regions 0x005704C0) deletes this region.
// NOT EXACT: the heap's tie order and an empty height list (the original then indexes past it;
// natively the height stays) are the only intended differences.
bool MapRegionClass::Split_Region_0056ef20()
{
    if (unknown_1a) return false;

    unsigned heap_size = (unsigned)(CellCount * 2 + 10);
    SplitNode *nodes = new SplitNode[heap_size];
    SplitHeap *queue = new SplitHeap(heap_size);
    Split_Clear_Spreads();
    for (int i = Cells.Count() - 1; i >= 0; --i) Split_Cell_Data(Cells[i]).RegionIndex = -2;

    unsigned target = Map_Random_Range_005797a0(CellCount / 8, CellCount / 3);
    unsigned seed_index = Map_Random_Range_005797a0(0, Cells.Count() - 1);
    Cell seed = Cells[seed_index];
    int node_count = 1;
    nodes[0].Element = seed;
    nodes[0].Score = 0.0f;
    Split_Cell_Data(seed).unknown_3c = -3;
    queue->Insert(&nodes[0]);

    SplitNode *node = queue->Extract_Min();
    double heading = (double)(unsigned)unknown_map_random_a705c0() * (6.283185307179586 / 4294967295.0);
    int spread = 0;
    while (node != 0 && spread < (int)target) {
        Split_Cell_Data(node->Element).RegionIndex = -3;
        for (int dir = 0; dir < 8; dir += 2) {
            Cell ncell = node->Element + unknown_adjacent_851a70[dir & 7];
            if (!Split_In_Bounds(ncell)) continue;
            UnknownMapGenerationCell &data = Split_Cell_Data(ncell);
            if (data.RegionIndex != -2 || data.unknown_3c == -3) continue;
            if (!Map[ncell].unknown_47b590()) continue;
            if (node_count >= (int)heap_size) continue;
            SplitNode *added = &nodes[node_count++];
            added->Element = ncell;
            added->Score = (float)Unknown_Cell_Score_0056dff0(seed, ncell, heading);
            data.unknown_3c = -3;
            queue->Insert(added);
        }
        ++spread;
        heading += unknown_map_gaussian_a6fd50.Next() * 0.39269908169872414;
        node = queue->Extract_Min();
    }
    Split_Clear_Spreads();
    for (SplitNode *rest = queue->Extract_Min(); rest != 0; rest = queue->Extract_Min()) {
        Split_Cell_Data(rest->Element).RegionIndex = -3;
    }
    delete[] nodes;
    delete queue;

    // Each connected piece (8-neighbours with the same mark) becomes a region.
    DynamicVectorClass<MapRegionClass *> made;
    for (int i = Cells.Count() - 1; i >= 0; --i) {
        Cell cell = Cells[i];
        int old_id = Split_Cell_Data(cell).RegionIndex;
        if (old_id >= -1) continue;
        MapRegionClass *region = new MapRegionClass(cell);
        region->Cells.Add(cell);
        region->CellCount++;
        made.Add(region);
        Split_Cell_Data(cell).RegionIndex = region->Index;
        DynamicVectorClass<Cell> open;
        open.Set_Growth_Step(100000);
        open.Add(cell);
        while (open.Count() != 0) {
            int last = open.Count() - 1;
            Cell c = open[last];
            open.Delete(last);
            for (int dir = 0; dir < 8; ++dir) {
                Cell ncell = c + unknown_adjacent_851a70[dir & 7];
                if (!Split_In_Bounds(ncell)) continue;
                UnknownMapGenerationCell &data = Split_Cell_Data(ncell);
                if (data.RegionIndex != old_id) continue;
                region->Cells.Add(ncell);
                region->CellCount++;
                data.RegionIndex = region->Index;
                open.Add(ncell);
            }
        }
    }

    for (int i = made.Count() - 1; i >= 0; --i) {
        MapRegionClass *region = made[i];
        if (region->unknown_24) continue;
        DynamicVectorClass<Cell> *border = region->Collect_Region_Boundary_0056fed0();
        int total = unknown_map_region_next_index_a70a28;
        bool *adjacent = new bool[total > 0 ? total : 1];
        for (int j = 0; j < total; ++j) adjacent[j] = false;
        for (int j = border->Count() - 1; j >= 0; --j) {
            for (int dir = 0; dir < 8; ++dir) {
                Cell ncell = (*border)[j] + unknown_adjacent_851a70[dir & 7];
                if (!Split_In_Bounds(ncell)) continue;
                int id = Split_Cell_Data(ncell).RegionIndex;
                if (id >= 0 && id != region->Index && id < total) adjacent[id] = true;
            }
        }
        delete border;

        MapRegionClass *merge_target = 0;
        int merge_height = -1, max_count = -1, min_height = -1, max_height = -1;
        for (int j = 0; j < total; ++j) {
            if (!adjacent[j]) continue;
            MapRegionClass *other = 0;
            for (int k = 0; k < unknown_map_regions_a6fd28.Count(); ++k) {
                if (unknown_map_regions_a6fd28[k]->Index == j) { other = unknown_map_regions_a6fd28[k]; break; }
            }
            if (other == 0 || other->unknown_24) continue;
            if (other->CellCount > max_count && !other->unknown_14) {
                max_count = other->CellCount;
                merge_height = other->unknown_10;
                merge_target = other;
            }
            if (min_height == -1) {
                min_height = max_height = other->unknown_10;
            } else {
                if (other->unknown_10 > max_height) max_height = other->unknown_10;
                if (other->unknown_10 < min_height) min_height = other->unknown_10;
            }
        }
        delete[] adjacent;

        DynamicVectorClass<int> heights;
        if (max_height == min_height) {
            if (min_height > 3) heights.Add(min_height - 4);
            if (max_height < 8) heights.Add(max_height + 4);
        } else if (max_height - min_height == 4) {
            heights.Add(max_height);
            heights.Add(min_height);
        } else if (max_height - min_height == 8) {
            heights.Add((max_height + min_height) / 2);
        }
        if (RandomMapGen.SeedData.MapType == 3 || RandomMapGen.SeedData.MapType == 4) {
            for (int j = heights.Count() - 1; j >= 0; --j) {
                if (heights[j] == 0) heights.Delete(j);
            }
        }

        if (region->CellCount <= 100) {
            if (merge_target != 0) {
                int old_height = region->unknown_10;
                region->unknown_10 = merge_height;
                Split_Raise_Region(region->Index, merge_height - old_height);
                for (int j = region->Cells.Count() - 1; j >= 0; --j) {
                    Cell c = region->Cells[j];
                    merge_target->Cells.Add(c);
                    merge_target->CellCount++;
                    Split_Cell_Data(c).RegionIndex = merge_target->Index;
                }
                region->unknown_24 = true;
            }
        } else if (heights.Count() > 0) {
            int old_height = region->unknown_10;
            int height = heights[Map_Random_Range_005797a0(0, heights.Count() - 1)];
            region->unknown_10 = height;
            Split_Raise_Region(region->Index, height - old_height);
        }
    }

    for (int k = unknown_map_regions_a6fd28.Count() - 1; k >= 0; --k) {
        MapRegionClass *region = unknown_map_regions_a6fd28[k];
        if (region != this && region != 0 && region->unknown_24) delete region;
    }
    ++unknown_1c;
    return true;
}
