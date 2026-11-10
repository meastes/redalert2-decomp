// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// From ra2-decomp 0x005A5360 MapGeneratorClass::Create_Urban_Area (src/mapgen.cpp, YR twin).
// Grows a paved urban area of up to size cells out from a cell (a priority-queue flood fill scored by the urban
// score), rejects it when it is too small or too sparse, trims its edge cells, marks the rest inviolate and
// scatters misc pavement tiles over it.
#include "shared/unknown_map_generator.hpp"
#include "shared/map_region.hpp"
#include "shared/map_view.hpp"
#include "shared/cell.hpp"
#include "shared/vein_heap_core.hpp"
#include "shared/tileset_index.hpp"
// @ref __real@8@3fffccccccccccccd000=007a6040
Rect __fastcall Bounding_Rectangle_00588e20(DynamicVectorClass<Cell> const &cells);
double __stdcall Randomized_Chebyshev_Distance_005872c0(Cell const &a,Cell const &b);

DynamicVectorClass<Cell> *MapGeneratorClass::Create_Urban_Area(Cell const &cell, int size)
{
    DynamicVectorClass<Cell> *cells;
    UnknownVeinNode *nodes = new UnknownVeinNode[10 * size];
    UnknownVeinHeap *queue = new UnknownVeinHeap(10 * size);
    int processed = 0;
    queue->Clear();

    Map.Reset_Iterator();
    CellClass *cptr = Map.Iterate();
    while (cptr != 0) {
        Cell c = cptr->MapCoords;
        Set_Cell_Data_Spread(c, 0);
        cptr = Map.Iterate();
    }

    int node_count = 0;
    nodes[node_count].Position = cell;
    nodes[node_count].Key = 0;
    Set_Cell_Data_Spread(cell, 1);
    node_count++;
    queue->Insert(&nodes[0]);

    UnknownVeinNode *node = queue->Remove_Min();

    cells = new DynamicVectorClass<Cell>;
    cells->Set_Growth_Step(size + 2);

    while (processed < size && node != 0) {
        Map[node->Position].IsoTileIndex = unknown_tileset_a6e0e0;
        cells->Add(node->Position);

        UnknownVeinNode *newnode = &nodes[node_count];
        for (int dir = FACING_N; dir < FACING_COUNT; dir++) {
            Cell newcell = Region_Adjacent_Cell(node->Position, dir);
            if (Hill_In_Radar(newcell)) {
                CellClass *newcellptr = &Map[newcell];
                if (newcellptr->unknown_47b590() && newcellptr->OverlayIndex == -1) {
                    if (Get_Cell_Data_Spread(newcell) != 1 && !MapRegionClass::Get_Cell_Data(newcell).unknown_45[0]) {
                        newnode->Position = newcell;
                        newnode->Key = Randomized_Chebyshev_Distance_005872c0(cell, newcell);
                        Set_Cell_Data_Spread(newcell, 1);
                        node_count++;
                        queue->Insert(newnode++);
                    }
                }
            }
        }

        processed++;
        node = queue->Remove_Min();
    }

    delete[] nodes;
    delete queue;

    Rect bounds = Bounding_Rectangle_00588e20(*cells);

    if (processed <= 100 || (double)(bounds.Width * bounds.Height) >= (double)cells->Count() * 1.6) {
        for (int j = cells->Count() - 1; j >= 0; j--) {
            Map[(*cells)[j]].IsoTileIndex = 0xFFFF;
        }
        delete cells;
        return 0;
    }

    for (int cell_index = cells->Count() - 1; cell_index >= 0; cell_index--) {
        Cell c = (*cells)[cell_index];
        for (int dir = FACING_N; dir < FACING_COUNT; dir++) {
            Cell newcell = Region_Adjacent_Cell(c, dir);
            if (Hill_In_Radar(newcell) && Map[newcell].IsoTileIndex != unknown_tileset_a6e0e0) {
                cells->Delete(cell_index);
            }
        }
    }

    for (int k = 0; k < cells->Count(); k++) {
        MapRegionClass::Get_Cell_Data((*cells)[k]).unknown_45[0] = 1;
    }

    int patch_count = cells->Count() / 3;
    for (int i = 0; i < patch_count; i++) {
        Cell base = (*cells)[Pick_Random_UInt(0, cells->Count() - 1)];
        int tile_variant = Pick_Random_UInt(0, 6);

        bool place = true;
        if (tile_variant > 3) {
            for (int m = 0; m < 2; m++) {
                for (int n = 0; n < 2; n++) {
                    Cell c = base + Cell(m, n);
                    if (!MapRegionClass::Get_Cell_Data(c).unknown_45[0] || Map[c].IsoTileIndex != unknown_tileset_a6e0e0) {
                        place = false;
                    }
                }
            }
        }

        if (place) {
            Place_Tile_Footprint_005883f0(unknown_tileset_a52ef4 + tile_variant, base, -1, -1);
        }
    }

    return cells;
}
