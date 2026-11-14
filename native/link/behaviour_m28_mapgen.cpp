// Behaviour rows (Scout1208, native M28; NOT EXACT, native/behaviour_rewrites.json).
// The skirmish Random Map's land seeding.
#define RA2_VEIN_HEAP_REMOVE_MIN_IMPLEMENTATION
#define RA2_MAPGEN_SHAPE_MEMBER
#include "shared/rect.hpp"
#include "shared/unknown_map_generator.hpp"
#include "shared/map_region.hpp"
#include "shared/map_view.hpp"
#include "shared/cell.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/map_gaussian.hpp"
#include "shared/map_random.hpp"
#include "shared/tileset_index.hpp"
#include "shared/fast_math.hpp"
#include "shared/com_persist.hpp"
#include "shared/vein_heap.hpp"

// MapGeneratorClass::MapSeeder's pointer (the header's int field cannot hold it natively;
// port/fixups.json points its five users here).
MapSeedClass *native_map_seeder_m28 = 0;

unsigned __fastcall Map_Random_Range_005797a0(unsigned, unsigned);
double __stdcall Direction_Weighted_Cell_Distance_0057d120(Cell const *from, Cell const *to, Rect const *area,
                                                           Cell const *origin, float xWeight, float yWeight);
bool __fastcall Generation_Cell_In_Bounds_0058da10(Cell const &position);

namespace {
UnknownMapGenerationCell &Cell_Data_m28(Cell const &cell)
{
    return unknown_map_cells_a70a24[cell.X + cell.Y * unknown_map_cell_side_84e804];
}
}

// 0x0057D3A0 MapGeneratorClass::unknown_0057d3a0 (Seed_Land's land patch grower; ra2-decomp's YR
// twin FUN_0059BBC0, called by Seed_Land_Type1/Type2 and Land_Chunks). Behaviour from the
// decompile (inventory/decompiled/0057d3a0.c; constants 1.0, 0.5, 1.2, 5.0, 2^-32 read from the
// exe's data at write time): grows at least 400 (`count` below 401 becomes 400) land cells as
// WorkingRegionID from `seed` (or a random water cell not yet in a region, up to 200 tries; 0
// when none), best-first over a min-heap of up to 8*count+2 nodes: each popped cell joins the
// region and its four straight neighbours inside the map diamond, not yet in a region, not yet
// visited and on water, inside the generation shape (0x0057D290), are scored by distance (plus
// up to 5 cells of noise; with `weighted` the direction-weighted distance 0x0057D120) and
// queued. The rest of the queue is then claimed where still water. Then Build_All_Shores for
// the region (0x0055C2C0); shore tiles of the region are reset to clear; on success the region
// id advances and the count of grown cells is returned, else the region's cells go back to
// water (tile 0x00A52580, height CellHeight) and 0 is returned.
// NOT EXACT: the start cell is picked with Map_Random_Range (the original draws raw
// Random2Class values and rejects those past the map width), so a seed gives another map than
// the original's; the two gaussian draws per step are kept for the sequence.
int MapGeneratorClass::unknown_0057d3a0(int count, Rect const &area, Point2D const &seed, bool ellipse,
                                        Cell const &origin, double weight, bool weighted)
{
    if (count < 401) {
        count = 400;
    }
    int capacity = count * 8 + 2;
    if (capacity < 101) {
        capacity = 100;
    }
    UnknownVeinNode *nodes = new UnknownVeinNode[capacity];
    UnknownVeinHeap *heap = new UnknownVeinHeap(capacity);

    Cell start;
    if (seed.X == 0 && seed.Y == 0) {
        int tries = 0;
        for (;;) {
            int x = (int)Map_Random_Range_005797a0(0, Map.MapSize.Width - 1);
            int y = (int)Map_Random_Range_005797a0(0, Map.MapSize.Height - 1);
            start = Cell((short)(x + 1 + y), (short)(Map.MapSize.Width - x + y));
            if (++tries > 199) {
                delete[] nodes;
                delete heap;
                return 0;
            }
            if (unknown_map_cells_a70a24 == 0 || Cell_Data_m28(start).RegionIndex != 0) {
                continue;
            }
            if (Map[start].unknown_47b7e0()) {
                break;
            }
        }
    } else {
        start = Cell((short)seed.X, (short)seed.Y);
    }

    float x_shape = 0.0f, y_shape = 0.0f;
    if (area != unknown_static_rect) {
        if (area.Height < area.Width) {
            x_shape = 1.0f;
            y_shape = (float)area.Width / (float)area.Height * 1.2f;
        } else {
            y_shape = 1.0f;
            x_shape = (float)area.Height / (float)area.Width * 1.2f;
        }
    }

    heap->Clear();
    int used = 1;
    nodes[0].Position = start;
    nodes[0].Key = 0.0f;
    Cell_Data_m28(start).unknown_3c = WorkingRegionID;
    heap->Insert(&nodes[0]);
    UnknownVeinNode *current = heap->Remove_Min();

    double x_weight = 1.0 / ((double)area.Width * 0.5 * (double)area.Width * 0.5);
    double y_weight = 1.0 / ((double)area.Height * 0.5 * (double)area.Height * 0.5);
    int grown = 0;
    while (grown < count && current != 0) {
        if (unknown_map_cells_a70a24 != 0) {
            Cell_Data_m28(current->Position).RegionIndex = WorkingRegionID;
        }
        Map[current->Position].IsoTileIndex = 0;
        for (int facing = 0; facing < 8; facing += 2) {
            Cell next = current->Position + AdjacentCell[facing & 7];
            int x = next.X, y = next.Y;
            if (!(unknown_map_min_a70a1c < x + y && x - y < unknown_map_min_a70a1c && y - x < unknown_map_min_a70a1c &&
                  x + y <= unknown_map_max_a70a20)) {
                continue;
            }
            UnknownMapGenerationCell &data = Cell_Data_m28(next);
            if (data.RegionIndex != 0 || data.unknown_3c == WorkingRegionID) continue;
            if (!Map[next].unknown_47b7e0() || used >= capacity) continue;
            if (!Cell_In_Generation_Shape(next, area, ellipse, x_weight, y_weight)) continue;
            UnknownVeinNode *node = &nodes[used];
            node->Position = next;
            float key;
            if (!weighted) {
                int dx = next.X - start.X, dy = next.Y - start.Y;
                double distance = Fast_Sqrt((double)(dx * dx + dy * dy));
                key = (float)((double)unknown_map_random_a705c0() * 5.0 * 2.3283064370807974e-10 + distance);
            } else {
                Cell from = start;
                key = (float)Direction_Weighted_Cell_Distance_0057d120(&from, &next, &area, &origin, x_shape, y_shape);
            }
            node->Key = key;
            ++used;
            data.unknown_3c = WorkingRegionID;
            heap->Insert(node);
        }
        unknown_map_gaussian_a6fd50.Next();
        ++grown;
        unknown_map_gaussian_a6fd50.Next();
        (void)weight;
        current = heap->Remove_Min();
    }
    while (current != 0) {
        UnknownMapGenerationCell &data = Cell_Data_m28(current->Position);
        CellClass &cell = Map[current->Position];
        if (data.RegionIndex == 0 && cell.unknown_47b7e0()) {
            cell.IsoTileIndex = 0;
            data.RegionIndex = WorkingRegionID;
        }
        ++grown;
        current = heap->Remove_Min();
    }
    delete[] nodes;
    delete heap;

    bool shores = Map.Unknown_0055C2C0(WorkingRegionID, 1);   // Build_All_Shores
    Map.Reset_Iterator();
    for (CellClass *cell = Map.Iterate(); cell != 0; cell = Map.Iterate()) {
        if (cell->IsoTileIndex >= unknown_tileset_a6cb74 && cell->IsoTileIndex < unknown_tileset_a6cb74 + 42) {
            cell->IsoTileIndex = 0;
            cell->unknown_11a = 0;
        }
    }
    if (shores) {
        ++WorkingRegionID;
        return grown;
    }
    int region = WorkingRegionID;
    Map.Reset_Iterator();
    for (CellClass *cell = Map.Iterate(); cell != 0; cell = Map.Iterate()) {
        UnknownMapGenerationCell &data = Cell_Data_m28(cell->MapCoords);
        if (data.RegionIndex == region) {
            data.RegionIndex = 0;
            data.unknown_4b = 0;
            cell->IsoTileIndex = unknown_tileset_a52580;
            cell->unknown_11a = 0;
            cell->unknown_11b = (char)CellHeight;
        }
    }
    return 0;
}

// 0x00582A50 (asm): clears every generator cell record's visit mark (+0x3C) over a walk of the
// map's cells (Grow_Water_Region 0x005820B0 calls it before each growth).
void Clear_Map_Cell_Traversal_Marks_00582a50()
{
    Map.Reset_Iterator();
    for (CellClass *cell = Map.Iterate(); cell != 0; cell = Map.Iterate()) {
        Cell_Data_m28(cell->MapCoords).unknown_3c = 0;
    }
}
