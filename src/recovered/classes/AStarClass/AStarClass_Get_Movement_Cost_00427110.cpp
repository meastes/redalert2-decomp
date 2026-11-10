// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// double AStarClass::Get_Movement_Cost(CellClass **from, CellClass **to, bool bridge, int move, FootClass *foot) (0x00427110)
// The cost of a step between two map cell slots for a move result (Tiberian
// Sun's AStarClass::Get_Movement_Cost): a moving blocker is followed along its
// path up to ten cells; predicted-path cells cost four times as much, and
// with bridges avoided a bridge step costs ten times (two times on a span).
// from ra2-decomp 0x00429830 (AStarClass::Get_Movement_Cost, src/astar.cpp)
// @ref ?unknown_path_move_costs_7cf0bc@@3PAMA=007cf0bc
// @ref ?unknown_bridge_offsets_79c420@@3QBHB=0079c420
// @ref ?unknown_bridge_offsets_79c440@@3QBHB=0079c440
// @ref ?unknown_bridge_facings_79c460@@3QAY02$$CBHA=0079c460
// @ref __real@4@40018000000000000000=0079c4cc
// @ref __real@4@4002a000000000000000=0079c4c8
// @ref __real@4@40008000000000000000=0079c4c4
#include "shared/path_estimator.hpp"
#include "shared/foot.hpp"
#include "shared/cell.hpp"
#include "shared/map_view.hpp"
#include "shared/adjacent_cell.hpp"
#include "shared/facing.hpp"

// The step cost of each move result (0x007CF0BC).
extern float unknown_path_move_costs_7cf0bc[];
// The bridge side cell offsets by facing (0x0079C420, 0x0079C440) and the facing by (dy + 1, dx + 1)
// (0x0079C460).
extern int const unknown_bridge_offsets_79c420[8];
extern int const unknown_bridge_offsets_79c440[8];
extern int const unknown_bridge_facings_79c460[3][3];

// ::Adjacent_Cell(cell, facing) as the twin calls it.
static inline Cell Path_Adjacent_Cell(Cell const &cell, int facing)
{
    Cell const &offset = AdjacentCell[facing & 7];
    return Cell(cell.X + offset.X, cell.Y + offset.Y);
}

double AStarClass::Get_Movement_Cost(CellClass **from, CellClass **to, bool bridge, int move, FootClass *foot)
{
    CellClass *from_cell = from[0];
    CellClass *to_cell = to[0];
    float cost = unknown_path_move_costs_7cf0bc[move];
    ObjectClass *next_occupier;
    ObjectClass *occupier;
    CellClass *cellptr = to_cell;
    bool on_bridge = bridge;
    if (move == 2) {
        ObjectClass *object = on_bridge ? cellptr->AltOccupierPtr : cellptr->OccupierPtr;
        occupier = object;
        int depth = 0;
        bool clear = false;
        if (Avoidance == 0) {
            for (; depth < 10; depth++) {
                if (occupier == 0) {
                    clear = true;
                    break;
                }
                clear = occupier->AbstractFlags.IsFoot;
                if (!clear) {
                    break;
                }
                int facing;
                if (((FootClass *)occupier)->unknown_468 == 0.0) {
                    facing = ((FootClass *)occupier)->Path[0];
                    if (facing == -1) {
                        clear = true;
                        break;
                    }
                } else {
                    facing = Dir_To_8(((FootClass *)occupier)->unknown_2e0.Current());
                }
                clear = false;
                cellptr = &Map[Path_Adjacent_Cell(occupier->unknown_slot104(), facing)];
                on_bridge = (cellptr->unknown_140 & 0x100) && (occupier->unknown_79 || ((CellClass *)occupier->unknown_slot105())->unknown_11b - cellptr->unknown_11b > 2);
                next_occupier = on_bridge ? cellptr->AltOccupierPtr : cellptr->OccupierPtr;
                occupier = next_occupier;
            }
        }
        if (!clear || Avoidance != 0) {
            cost = 4.0f;
        }
        if (Avoidance == 2) {
            cost = 1000.0f;
        }
    }
    if (to_cell->unknown_140 & 0x40000) {
        cost = cost * 4.0f;
    }
    if (bridge && IsAvoidBridges) {
        float multiplier = 1.0f;
        Cell step = to_cell->MapCoords - from_cell->MapCoords;
        int dir = unknown_bridge_facings_79c460[step.Y + 1][step.X + 1];
        CellClass *cell1;
        CellClass *cell2;
        int cell2_offset;
        if (to_cell->unknown_140 & 0x800) {
            cell2_offset = unknown_bridge_offsets_79c440[(dir - 4) & 7];
            cell1 = to[unknown_bridge_offsets_79c440[dir]];
            cell2 = to[cell2_offset];
        } else {
            cell2_offset = unknown_bridge_offsets_79c420[(dir - 4) & 7];
            cell1 = to[unknown_bridge_offsets_79c420[dir]];
            cell2 = to[cell2_offset];
        }
        if (!(cell1->unknown_140 & 0x100)) {
            multiplier = 10.0f;
        } else if (cell2->unknown_140 & 0x100) {
            multiplier = 2.0f;
        }
        return cost * multiplier;
    }
    return cost;
}
