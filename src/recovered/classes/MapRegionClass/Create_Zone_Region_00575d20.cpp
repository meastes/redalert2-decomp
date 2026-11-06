// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// From ra2-decomp 0x00594420 Create_Zone_Region (src/mapgen.cpp, YR twin).
// void __fastcall MapRegionClass::Create_Zone_Region_00575d20(CellClass *cell_ptr, int zone_kind, int zone) (0x00575D20,
// RET4): a new region from a flood fill out of cell over the in-bounds cells that have no +0x4C occupant and lie in
// the given zone of the map.
#include "shared/map_region.hpp"
#include "shared/cell.hpp"
#include "shared/map_view.hpp"

static inline bool In_Zone_Map_Bounds(Cell const &cell)
{
    int x = cell.X;
    int y = cell.Y;
    int diagonal = y + cell.X;
    return diagonal > unknown_map_min_a70a1c &&
        x - y < unknown_map_min_a70a1c &&
        y - x < unknown_map_min_a70a1c &&
        diagonal <= unknown_map_max_a70a20;
}

void __fastcall MapRegionClass::Create_Zone_Region_00575d20(CellClass *cell_ptr, int zone_kind, int zone)
{
    MapRegionClass *region = new MapRegionClass(cell_ptr->MapCoords);
    int id = region->Index;
    DynamicVectorClass<Cell> pending;
    pending.Set_Growth_Step(20000);
    Set_Spread(cell_ptr->MapCoords, id);
    pending.Add(cell_ptr->MapCoords);
    while (pending.Count() > 0) {
        Cell cell = pending[pending.Count() - 1];
        pending.Delete(pending.Count() - 1);
        MapRegionClass::Get_Cell_Data(cell).RegionIndex = id;
        region->CellCount++;
        region->Cells.Add(cell);
        for (int dir = FACING_N; dir < FACING_COUNT; dir++) {
            CellClass &other = Map[unknown_adjacent_851a70[dir & 7] + cell];
            Cell const &next = other.MapCoords;
            if (In_Zone_Map_Bounds(other.MapCoords)) {
                UnknownMapGenerationCell &data = MapRegionClass::Get_Cell_Data(next);
                if (other.unknown_4c == 0 && Map.Zone(next, zone_kind, false) == zone && data.unknown_3c == -1) {
                    data.unknown_3c = id;
                    pending.Add(next);
                }
            }
        }
    }
}
