// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// From ra2-decomp 0x0059B200 MapGeneratorClass::Seed_Land_Type2 (src/mapgen.cpp, YR twin).
// Scatters land over two halves of the map until each half meets the land target.
#include "shared/crt_math.hpp"
#include "shared/unknown_map_generator.hpp"
#include "shared/map_region.hpp"
#include "shared/map_view.hpp"
#include "shared/map_random.hpp"
#include "shared/random2.hpp"
#include "shared/cell.hpp"
int __fastcall unknown_cells_428ac0();
extern double const unknown_random_scale_7a5dd0;
extern double const unknown_land2_minimum_7a5c78,unknown_land2_maximum_7a5c80,unknown_land2_chunk_7a5c88;
// @ref ?unknown_land2_minimum_7a5c78@@=007a5c78
// @ref ?unknown_land2_maximum_7a5c80@@=007a5c80
// @ref ?unknown_land2_chunk_7a5c88@@=007a5c88
// @ref ?Cell_In_Generation_Shape_0057d290@UnknownMapGenerator@@QAE_NABVCell@@ABVRect@@_NNN@Z=0057d290
void UnknownMapGenerator::unknown_0057c9e0()
{
    unknown_308 = 1;
    int total = unknown_cells_428ac0();
    int tries = 0;
    double target = (1.0 - SeedOptions.WaterAmount * 0.01) * (unknown_land2_maximum_7a5c80 - unknown_land2_minimum_7a5c78) + unknown_land2_minimum_7a5c78;
    int maximum = (int)((double)total * unknown_land2_chunk_7a5c88 * target);

    Map.Unknown_0055A550();
    CellClass *cptr = Map.Unknown_0055A490();
    while (cptr != 0) {
        MapRegionClass::Get_Cell_Data(cptr->Fetch_CellID()).unknown_3c = 0;
        cptr = Map.Unknown_0055A490();
    }

    Rect halves[2];
    if ((unsigned int)unknown_map_random_a705c0() * unknown_random_scale_7a5dd0 < 0.5) {
        halves[0] = Rect(Map.MapLocalSize.X, Map.MapLocalSize.Y, Map.MapLocalSize.Width / 2 - 1, Map.MapLocalSize.Height);
        halves[1] = Rect(Map.MapLocalSize.X + Map.MapLocalSize.Width / 2 + 1, Map.MapLocalSize.Y, Map.MapLocalSize.Width / 2 - 1, Map.MapLocalSize.Height);
    } else {
        halves[0] = Rect(Map.MapLocalSize.X, Map.MapLocalSize.Y, Map.MapLocalSize.Width, Map.MapLocalSize.Height / 2 - 1);
        halves[1] = Rect(Map.MapLocalSize.X, Map.MapLocalSize.Y + Map.MapLocalSize.Height / 2 + 1, Map.MapLocalSize.Width, Map.MapLocalSize.Height / 2 - 1);
    }

    for (int half = 0; half < 2; half++) {
        int placed = 0;
        double current = 0.0;
        Rect bounds = halves[half];
        Point2D origin(bounds.X + bounds.Width / 2 + bounds.Height / 2 + bounds.Y + 1,
                       bounds.Y + Map.MapSize.Width + bounds.Height / 2 - bounds.Width / 2 - bounds.X);
        Point2D seed = origin;
        double x_scale;
        double y_scale;
        if (bounds.Height > bounds.Width) {
            x_scale = (double)bounds.Height / (double)bounds.Width * 1.2;
            y_scale = 1.0;
        } else {
            x_scale = 1.0;
            y_scale = (double)bounds.Width / (double)bounds.Height * 1.2;
        }
        double half_width = (double)bounds.Width * 0.5;
        double inverse_x = 1.0 / (half_width * half_width);
        double half_height = (double)bounds.Height * 0.5;
        double inverse_y = 1.0 / (half_height * half_height);

        while (current < target && tries < 100) {
            int size = (int)((target - current) * (double)total);
            size = size < maximum ? size : maximum;
            current = (double)(placed += unknown_0057d3a0(size, bounds, seed, true, Cell(origin.X, origin.Y), 0.75, true)) / (double)total;
            tries++;

            int nearest = 50000;
            Cell closest(0, 0);
            int count = unknown_map_cell_side_84e804 * unknown_map_cell_side_84e804;
            int minimum_diagonal = bounds.X * 2 - Map.MapSize.Width + 1;
            int maximum_diagonal = minimum_diagonal + bounds.Width * 2 + 2;
            int minimum_sum = Map.MapSize.Width + bounds.Y * 2 + 1;
            int maximum_sum = minimum_sum + bounds.Height * 2 + 2;
            for (int index = 0; index < count; index++) {
                UnknownMapGenerationCell &data = MapRegionClass::Get_Cell_Data(index);
                int sum = data.Position.X + data.Position.Y;
                int diagonal = data.Position.X - data.Position.Y;
                if (sum >= minimum_sum && sum <= maximum_sum && diagonal >= minimum_diagonal && diagonal <= maximum_diagonal && data.unknown_3c == 0 && Cell_In_Generation_Shape_0057d290(data.Position, bounds, true, inverse_x, inverse_y)) {
                    double dy = (double)abs(data.Position.Y - origin.Y) * y_scale;
                    double dx = (double)abs(data.Position.X - origin.X) * x_scale;
                    int distance = (int)(dy + dx);
                    if (distance < nearest) {
                        nearest = distance;
                        closest = data.Position;
                    }
                }
            }
            seed = Point2D(closest.X, closest.Y);
        }
    }
}
