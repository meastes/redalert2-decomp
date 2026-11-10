// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// From ra2-decomp 0x0059AFA0 MapGeneratorClass::Seed_Land_Type1 (src/mapgen.cpp, YR twin).
// Scatters one land mass from the middle of the map until the land target is met.
#include "shared/unknown_map_generator.hpp"
#include "shared/map_region.hpp"
#include "shared/map_view.hpp"
#include "shared/cell.hpp"
int __fastcall unknown_cells_428ac0();
extern double const unknown_land_minimum_7a5c58,unknown_land_maximum_7a5c60,unknown_land_chunk_7a5c68;
// @ref ?unknown_land_minimum_7a5c58@@=007a5c58
// @ref ?unknown_land_maximum_7a5c60@@=007a5c60
// @ref ?unknown_land_chunk_7a5c68@@=007a5c68
void MapGeneratorClass::Seed_Land_Type1()
{
    WorkingRegionID = 1;
    int total = unknown_cells_428ac0();
    int placed = 0;
    int tries = 0;
    double current = 0.0;
    double target = (1.0 - SeedData.WaterAmount * 0.01) * (unknown_land_maximum_7a5c60 - unknown_land_minimum_7a5c58) + unknown_land_minimum_7a5c58;
    int maximum = (int)((double)total * unknown_land_chunk_7a5c68 * target);
    Point2D origin(Map.MapSize.Width / 2 + Map.MapSize.Height / 2 + 1,
                   Map.MapSize.Width / 2 + Map.MapSize.Height / 2);
    Point2D seed = origin;

    Map.Reset_Iterator();
    CellClass *cptr = Map.Iterate();
    while (cptr != 0) {
        MapRegionClass::Get_Cell_Data(cptr->Fetch_CellID()).unknown_3c = 0;
        cptr = Map.Iterate();
    }

    Rect bounds = Map.MapLocalSize;
    bounds.X++;
    bounds.Y++;
    bounds.Width -= 2;
    bounds.Height -= 2;
    while (current < target && tries < 100) {
        int size = (int)((target - current) * (double)total);
        size = size < maximum ? size : maximum;
        current = (double)(placed += unknown_0057d3a0(size, bounds, seed, true, Cell(origin.X, origin.Y), 0.75, true)) / (double)total;
        tries++;

        int nearest = 50000;
        Cell closest(0, 0);
        Map.Reset_Iterator();
        cptr = Map.Iterate();
        while (cptr != 0) {
            if (MapRegionClass::Get_Cell_Data(cptr->MapCoords).unknown_3c == 0) {
                int distance = abs(cptr->MapCoords.X - origin.X) + abs(cptr->MapCoords.Y - origin.Y);
                if (distance < nearest) {
                    nearest = distance;
                    closest = cptr->MapCoords;
                }
            }
            cptr = Map.Iterate();
        }
        seed = Point2D(closest.X, closest.Y);
    }
}
