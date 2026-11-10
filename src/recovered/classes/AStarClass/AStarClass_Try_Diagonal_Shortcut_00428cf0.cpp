// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// AStarClass::Try_Diagonal_Shortcut() (0x00428CF0): replaces a corner
// of two legs of a path by a diagonal where foot can walk it (Tiberian
// Sun's Try_Diagonal_Shortcut; hypothesis). From ra2-decomp AStarClass::Try_Diagonal_Shortcut
// (YR 0x0042B420), adapted to our names (Scout803 td0, Scout849 td_d FacingType locals; Scout857 r1).
// @ref ?unknown_4cafb0@FootClass@@QBENXZ=004cafb0
#include "shared/path_estimator.hpp"
#include "shared/foot.hpp"
#include "shared/cell.hpp"
#include "shared/map_view.hpp"
#include "shared/house_adjacent.hpp"

// The cell next to one toward a facing (the guarded block of shared/display.hpp, shared/terrain.hpp,
// shared/infantry.hpp and shared/aircraft.hpp; those headers are too heavy for this unit).


// The cell reached from cell along count facings (Westwood's Follow_Path2, the file-local wrapper).
static Cell Follow_Path2(Cell const &cell, int count, int const *path)
{
    return AStarClass::Follow_Path(cell, count, path);
}

int AStarClass::Try_Diagonal_Shortcut(FootClass *foot, int *facings, int *heights, int first_length, int second_length, Cell &corner)
{
    int first_leg_length = first_length;
    int second_leg_length = second_length;
    bool is_blocked;
    FacingType second_leg_direction = (FacingType)facings[first_leg_length];
    FacingType first_leg_direction = (FacingType)facings[0];
    FacingType diagonal_direction = FacingType((first_leg_direction + second_leg_direction) >> 1);

    if (diagonal_direction + 1 != second_leg_direction
    && diagonal_direction + 1 != first_leg_direction) {
        diagonal_direction = (FacingType)0;
    }

    if (first_leg_direction == 8 || second_leg_direction == 8) {
        int total_length = first_leg_length + second_leg_length;
        corner = Follow_Path2(corner, total_length, facings);
        return total_length;
    }

    Cell diagonal_start_cell = corner;

    int max_diagonal_length = second_leg_length;
    second_leg_length = first_leg_length < max_diagonal_length ? first_leg_length : max_diagonal_length;
    if (second_leg_length < first_leg_length) {
        diagonal_start_cell = Follow_Path(diagonal_start_cell, first_leg_length - second_leg_length, facings);
    }

    double threat_avoidance = foot->unknown_4cafb0();
    HouseClass *house = foot->House;

    while (second_leg_length > 0) {
        int moves_remaining = second_leg_length * 2;
        is_blocked = false;
        Cell scan_cell = Adjacent_Cell(diagonal_start_cell, diagonal_direction);
        int leg_total = first_leg_length + second_leg_length;
        int scan_height = heights[leg_total - moves_remaining];
        CellClass *scan_cell_ptr = &Map[scan_cell];

        while (!is_blocked && moves_remaining > 0) {
            is_blocked = foot->unknown_slot101(scan_cell_ptr, diagonal_direction, scan_height, 0, 1) != 0
            || (scan_cell_ptr->unknown_140 & 0x40000)
            || Map.Cell_Threat(diagonal_start_cell, house) * threat_avoidance >= 1.0;

            moves_remaining--;

            scan_cell = Adjacent_Cell(scan_cell, diagonal_direction);
            scan_cell_ptr = &Map[scan_cell];

            scan_height = (scan_height - scan_cell_ptr->unknown_11b == 4 && (scan_cell_ptr->unknown_140 & 0x100))
            ? scan_cell_ptr->unknown_11b + 4
            : scan_cell_ptr->unknown_11b;
        }

        if (!is_blocked) {
            int *diagonal_moves = &facings[first_leg_length - second_leg_length];
            int diagonal_count = 2 * second_leg_length;
            for (int move_index = 0; move_index < diagonal_count; move_index++) {
                diagonal_moves[move_index] = diagonal_direction;
            }

            corner = Follow_Path(corner, first_leg_length - second_leg_length, facings);
            return first_leg_length - second_leg_length;
        }

        diagonal_start_cell = Adjacent_Cell(diagonal_start_cell, first_leg_direction);
        second_leg_length--;
    }

    corner = Follow_Path2(corner, first_leg_length, facings);
    return first_leg_length;
}
