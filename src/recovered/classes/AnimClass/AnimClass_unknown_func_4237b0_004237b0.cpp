// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// Adopted from ra2-decomp src/anim.cpp AnimClass::Next_Flaming_Guy_Coord
// (YR 0x00425D10, ca9b8ad), on the declarations of the earlier drafts.
// CoordStruct AnimClass::unknown_func_4237b0() (0x004237B0)
// Finds a place for the animation to come down: the nearest water cell
// within five cells whose neighbours toward facings 6 and 0 are clear
// (cells to the north-west score 3 better), then a step from the
// animation's cell toward it, then a random neighbour; the zero coordinate
// for none (ra2-decomp Next_Flaming_Guy_Coord; the cell tests are water, not
// under a bridge, neighbours not under a bridge). The point length is
// TPoint2D::Length (shared/point.hpp RA2_POINT_LENGTH) and the cell centres
// are their Coord(Cell) (shared/coord.hpp RA2_COORD_FROM_CELL_INLINE); both
// macros are defined by the unit's first row 0x0041FF20.
#include "shared/fast_math.hpp"
#include "shared/anim.hpp"
#include "shared/map_view.hpp"
#include "shared/cell.hpp"
#include "shared/random.hpp"
#include "shared/id_source.hpp"
#include "shared/rect.hpp"
#include "shared/adjacent_cell.hpp"
// @ref ?unknown_sqrt_4ba410@@YANN@Z=004ba410
// @ref _unknown_geometry_k=0084c6a0
// @ref _unknown_geometry_l=0084c6d0

CoordStruct AnimClass::unknown_func_4237b0()
{
    bool any = false;
    Cell start = Coord_Cell(Position_Coord_004f3f40());
    int mindist = 10000;
    Cell mincell = unknown_geometry_k;
    for (int x = start.X - 5; x < start.X + 5; x++) {
        any = true;
        for (int y = start.Y - 5; y < start.Y + 5; y++) {
            if (Map.Unknown_0055A660(Cell(x, y), true)) {
                CellClass &cell = Map[Cell(x, y)];
                if (cell.unknown_ec == 2 && !(cell.unknown_140 & 0x100)
                    && !(cell.Adjacent_Cell(6).unknown_140 & 0x100)
                    && !(cell.Adjacent_Cell(0).unknown_140 & 0x100)) {
                    int dist = cell.MapCoords != start ? TPoint2D<int>(x - start.X, y - start.Y).Length() : 0;
                    if (x > start.X || y > start.Y) {
                        dist -= 3;
                    }
                    if (dist < mindist) {
                        mindist = dist;
                        mincell = Cell(x, y);
                    }
                }
            }
        }
    }
    if (any && mincell != unknown_geometry_k) {
        int dx = mincell.X - start.X;
        int dy = mincell.Y - start.Y;
        int step_x = 0;
        int step_y = 0;
        if (dx < 0) {
            step_x = -1;
        } else if (dx > 0) {
            step_x = 1;
        }
        if (dy < 0) {
            step_y = -1;
        } else if (dy > 0) {
            step_y = 1;
        }
        if (unknown_func_423b90(start + Cell(step_x, step_y))) {
            return CoordStruct(Cell(start.X + step_x, start.Y + step_y));
        }
        if (unknown_func_423b90(start + Cell(step_x, 0))) {
            return CoordStruct(Cell(start.X + step_x, start.Y));
        }
        if (unknown_func_423b90(start + Cell(0, step_y))) {
            return CoordStruct(Cell(start.X, start.Y + step_y));
        }
    }
    if (unknown_geometry_k.X == 0 && unknown_geometry_k.Y == 0) {
        int dir = unknown_id_source_a3d290->Random()(0, 7);
        for (int i = 0; i < 8; i++) {
            int facing = (dir + i) % 8;
            Cell const &offset = AdjacentCell[facing & 7];
            Cell newcell = start + offset;
            if (unknown_func_423b90(newcell)) {
                return CoordStruct(newcell);
            }
        }
    }
    return unknown_geometry_l;
}
