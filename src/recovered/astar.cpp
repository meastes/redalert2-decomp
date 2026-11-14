// astar.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/astar.json carries the addresses and the rows that
// stay in their own files.


// Optimize_Moves 0x004290C0 is declared for this unit.
#define RA2_PATH_OPTIMIZE_MOVES
// Get_Movement_Cost 0x00427110 is declared for this unit.
#define RA2_PATH_MOVEMENT_COST
#define COM_PTR_IMPLEMENT_RELEASE
// Find_Path_Hierarchical calls MapClass::Unknown_00567D20 (shared/map_view.hpp).
#define MAP_VIEW_DECLARE_REGION_THREAT
// Header-static dynamic initializers of the unit at 0x00426E70: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x00426E70-0x00427051); the other
// drafts of this plan are emitted by these includes.
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"


// Dynamic initializer (0x00426E90, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_00426e70.cpp; no source of its own.


// Dynamic initializer (0x00426EC0, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_00426e70.cpp; no source of its own.


// Dynamic initializer (0x00426EE0, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_00426e70.cpp; no source of its own.


// Dynamic initializer (0x00426F00, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_00426e70.cpp; no source of its own.


// Dynamic initializer (0x00426F20, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_00426e70.cpp; no source of its own.


// Dynamic initializer (0x00426F40, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_00426e70.cpp; no source of its own.


// Dynamic initializer (0x00426F60, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_00426e70.cpp; no source of its own.


// Dynamic initializer (0x00426FA0, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_00426e70.cpp; no source of its own.


// Dynamic initializer (0x00426FD0, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_00426e70.cpp; no source of its own.


// Dynamic initializer (0x00427000, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_00426e70.cpp; no source of its own.


// Dynamic initializer (0x00427030, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_00426e70.cpp; no source of its own.


// Dynamic initializer (0x00427040, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_00426e70.cpp; no source of its own.


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// AStarClass::Follow_Path() (0x00427060): the cell at the end
// of count steps along a list of facings.
#ifndef VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#endif
#include "shared/path_estimator.hpp"
#include "shared/cell.hpp"
#include "shared/subzone_tracking.hpp"
#include "shared/map_view.hpp"
#include "shared/tube.hpp"

// The unit expands its DynamicVectorClass constructors and calls the VectorClass base constructors
// (0x0042B510, 0x0042B620) out of line.
template <>
inline DynamicVectorClass<unsigned int>::DynamicVectorClass(unsigned size, unsigned int const *array)
    : VectorClass<unsigned int>(size, array)
{
    GrowthStep = 10;
    ActiveCount = 0;
}
template <>
inline DynamicVectorClass<unsigned short>::DynamicVectorClass(unsigned size, unsigned short const *array)
    : VectorClass<unsigned short>(size, array)
{
    GrowthStep = 10;
    ActiveCount = 0;
}

// The unit's out-of-line copy is the COMDAT 0x0042AD50 (0x00428CF0 calls it).


Cell __fastcall AStarClass::Follow_Path(Cell const &start, int count, int const *facings)
{
    // ra2-decomp Follow_Path (YR 0x00429780) form: the exit test inside the loop.
    Cell cell = start;
    for (int index = 0; ; index++) {
        if (index >= count) {
            return cell;
        }
        cell = Next_Cell(cell, facings[index]);
    }
}


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
#include "shared/foot.hpp"
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
                on_bridge = (cellptr->unknown_140 & 0x100) && (occupier->unknown_79 || ((CellClass *)occupier->Get_Cell_Ptr())->unknown_11b - cellptr->unknown_11b > 2);
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


// AStarClass::Create_Node() (0x00427D40): takes the next search node for a cell.
#include "shared/fast_math.hpp"
#include "shared/crt_memory.hpp"

RegularOpenNode *AStarClass::Create_Node(RegularOpenNode *parent, CellClass **cell, Cell const &target, float cost)
{
    RegularOpenNode *node = &RegularOpenNodes->nodes[RegularOpenNodes->count++];
    RegularNode *entry = &RegularNodes->entries[RegularNodes->count++];
    entry->unknown_00 = cell;
    if (parent) {
        entry->unknown_08 = parent->unknown_00;
        CellClass *here = *cell;
        CellClass *from = *parent->unknown_00->unknown_00;
        entry->unknown_04 = here->unknown_11b;
        if (here->unknown_140 & 0x100) {
                        if ((from->unknown_140 & 0x100) && parent->unknown_00->unknown_04 == from->unknown_11b + 4) {
                entry->unknown_04 += 4;
            } else if (!(from->unknown_140 & 0x100) && abs(here->unknown_11b - parent->unknown_00->unknown_04 + 3) <= 1) {
                entry->unknown_04 += 4;
            }
        }
    } else {
        entry->unknown_08 = 0;
        entry->unknown_04 = *(int *)unknown_30;
    }
    node->unknown_00 = entry;
    if (parent) {
        node->unknown_04 = cost + parent->unknown_04;
        node->unknown_0c = parent->unknown_0c + 1;
    } else {
        node->unknown_04 = 0;
        node->unknown_0c = 1;
    }
    Cell const &coords = (*cell)->MapCoords;
    int dx = abs(coords.X - target.X);
    int dy = abs(coords.Y - target.Y);
    node->unknown_08 = Fast_Sqrt(dx * dx + dy * dy) + node->unknown_04;
    return node;
}


// AStarClass::Clear() (0x00427E90): starts a new search.
// Shared map-grid dimension; this pathfinding routine needs no MapRegionClass layout.
extern int unknown_map_cell_side_84e804;

void AStarClass::Clear()
{
    RegularNodes->count = 0;
    RegularOpenNodes->count = 0;
    RegularQueue->Clear();
    HierQueue->Clear();
    if (++UniqueID == 0) {
        for (int index = unknown_map_cell_side_84e804 * unknown_map_cell_side_84e804 - 1; index >= 0; index--) {
            RegularVisited[index] = 0;
            RegularBridgeVisited[index] = 0;
        }
        for (int layer = 0; layer < 3; layer++) {
            int size = Map.unknown_70[layer];
            int *first = HierOnPath[layer];
            int *second = HierOpened[layer];
            int *third = HierCosts[layer];
            for (int entry = size - 1; entry >= 0; entry--) {
                first[entry] = 0;
                second[entry] = 0;
                third[entry] = 0;
            }
        }
        UniqueID++;
    }
}


// AStarClass::Is_Visited() (0x00427F70): whether a cell index is
// marked in the current generation.

bool AStarClass::Is_Visited(int unused, bool first, int index)
{
    if (first) {
        if (RegularVisited[index] == UniqueID) return true;
        return false;
    }
    if (RegularBridgeVisited[index] == UniqueID) return true;
    return false;
}


// Not exact (RA2_NON_MATCHING): AStarClass::Find_Path_Regular at 0x00427370, best score 51.17, 2026-11-13; differs: 812 instructions in the original, 771 in the draft (shape 91.22)
#ifdef RA2_NON_MATCHING
// NON-EXACT DRAFT 0x427370 AStarClass::Find_Path_Regular (cell search) (2506 B)
// entry 0x427370; exact-match score 78.55 (lane best), this text 78.55; plan "draft, verify"
// origin lane: Scout965 (build/class-work/Scout965/keep/a0.cpp, 78.55); native adaptation Scout1103
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1103 (native/drafts.json, native M10). Not original bytes.
// Its own unit: the population macros the lane scored it under come first.
// AStarClass::Find_Path_Regular() (0x00427370): the cell search (Tiberian Sun's
// Find_Path_Regular): expands the open nodes cell by cell, eight neighbours and the tube exit, kept
// inside the subzone corridor the hierarchical search marked in +0x40 when with_hs, and builds,
// straightens and optimizes the path to the destination.

// NATIVE adaptation (Scout1103): the lane scored this draft with private
// path_estimator.hpp additions (the +0x14 queue's Insert, Regular_Extract_Min,
// Replace_Root, Regular_Heapify over RegularOpenNode; Scout965 h1/shared) and
// map_view.hpp's guarded Cells member. They are spelled here as file-local
// functions with the lane's statements; the cell slots come from the full
// MapClass (link/behaviour_m10_map.cpp).
CellClass **Native_Map_Cell_Slot(int index);   // the address of Map.Cells[index]
static inline RegularOpenNode **Native_Regular_Heap(UnknownPathEstimatorBlock *block) { return (RegularOpenNode **)block->unknown_08; }
static inline bool Native_Node_Less(RegularOpenNode const &a, RegularOpenNode const &b) { return (double)a.unknown_08 < (double)b.unknown_08; }
static inline void Native_Regular_Heapify(UnknownPathEstimatorBlock *block, int index)
{
    RegularOpenNode **heap = Native_Regular_Heap(block);
    int left = index << 1;
    int right = (index << 1) + 1;
    int smallest = left <= block->unknown_00 && Native_Node_Less(*heap[left], *heap[index]) ? left : index;
    smallest = right <= block->unknown_00 && Native_Node_Less(*heap[right], *heap[smallest]) ? right : smallest;
    while (smallest != index) {
        RegularOpenNode *temp = heap[index];
        heap[index] = heap[smallest];
        heap[smallest] = temp;
        index = smallest;
        left = index << 1;
        right = (index << 1) + 1;
        smallest = left <= block->unknown_00 && Native_Node_Less(*heap[left], *heap[index]) ? left : index;
        smallest = right <= block->unknown_00 && Native_Node_Less(*heap[right], *heap[smallest]) ? right : smallest;
    }
}
static inline bool Native_Regular_Insert(UnknownPathEstimatorBlock *block, RegularOpenNode &node)
{
    RegularOpenNode **heap = Native_Regular_Heap(block);
    unsigned index = block->unknown_00 + 1;
    unsigned parent = index >> 1;
    float score = node.unknown_08;
    if (index >= (unsigned)block->unknown_04) {
        return false;
    }
    while (index > 1) {
        if (heap[parent]->unknown_08 <= score) {
            break;
        }
        heap[index] = heap[parent];
        index = parent;
        parent = index >> 1;
    }
    heap[index] = &node;
    block->unknown_00++;
    if ((unsigned int)&node > (unsigned int)block->unknown_0c) {
        block->unknown_0c = (int)&node;
    }
    if ((unsigned int)&node < (unsigned int)block->unknown_10) {
        block->unknown_10 = (int)&node;
    }
    return true;
}
static inline RegularOpenNode *Native_Regular_Extract_Min(UnknownPathEstimatorBlock *block)
{
    if (block->unknown_00 == 0) {
        return 0;
    }
    RegularOpenNode **heap = Native_Regular_Heap(block);
    RegularOpenNode *min = heap[1];
    heap[1] = heap[block->unknown_00];
    heap[block->unknown_00] = 0;
    block->unknown_00--;
    Native_Regular_Heapify(block, 1);
    return min;
}
static inline RegularOpenNode *Native_Replace_Root(UnknownPathEstimatorBlock *block, RegularOpenNode &node)
{
    if (block->unknown_00 == 0) {
        return &node;
    }
    RegularOpenNode **heap = Native_Regular_Heap(block);
    RegularOpenNode *old_root = heap[1];
    if (Native_Node_Less(node, *old_root)) {
        return &node;
    }
    heap[1] = &node;
    Native_Regular_Heapify(block, 1);
    return old_root;
}
// The lane's header named the estimator's +0x30/+0x34 ints (TS names);
// src/shared keeps them as char unknown_30[8].
#define CurrentCellHeight (*(int *)&unknown_30[0])
#define DestCellHeight (*(int *)&unknown_30[4])

extern int unknown_map_cell_side_84e804;

PathStruct *AStarClass::Find_Path_Regular(Cell const &from, Cell const &to, FootClass *foot, int *moves, int max_loops, bool with_hs)
{
    int face;

    // The facing's step cost addend (0x007CF0DC).
    static float _facing_costs[8] = {
        0.001f,
        0.005f,
        0.002f,
        0.006f,
        0.003f,
        0.007f,
        0.004f,
        0.008f,
    };

    int tries = 0;

    CellClass **to_pptr = Native_Map_Cell_Slot(512 * to.Y + to.X);
    CellClass **from_pptr = Native_Map_Cell_Slot(512 * from.Y + from.X);

    CellClass *to_ptr = *to_pptr;
    CellClass *from_ptr = *from_pptr;

    if (!from_ptr || !to_ptr) {
        return 0;
    }

    DestCellHeight = (foot->Kind_Of() == 2 || !(to_ptr->unknown_140 & 0x100)) ? to_ptr->unknown_11b : to_ptr->unknown_11b + 4;

    CurrentCellHeight = (foot->Kind_Of() == 2 || !foot->unknown_79) ? from_ptr->unknown_11b : from_ptr->unknown_11b + 4;

    if (foot->Techno_Type_Class()->IsTrain) {
        if (from_ptr->unknown_140 & 0x100) {
            if (abs(CoordStruct(foot->Coord).Z / unknown_geometry_g - CurrentCellHeight) > 2) {
                CurrentCellHeight += 4;
            }
        }
    }

    ObjectSpeed = foot->Techno_Type_Class()->SpeedType;
    HierNodeIndex = 0;
    HierLastNodeCell = from;
    int *fine_final_ids = HierOnPath[0];
    RegularOpenNode *working_node = Create_Node(0, from_pptr, to, 0.0);

    if (from == to && CurrentCellHeight == DestCellHeight) {
        return 0;
    }

    if (Avoidance != 0) {
        Apply_Path_Collision_Avoidance(foot);
    }
    int start_index = from_ptr->MapCoords.X + unknown_map_cell_side_84e804 * from_ptr->MapCoords.Y;
    if (CurrentCellHeight > from_ptr->unknown_11b) {
        RegularBridgeVisited[start_index] = UniqueID;
        RegularBridgeMovementCosts[start_index] = 0;
    } else {
        RegularVisited[start_index] = UniqueID;
        RegularMovementCosts[start_index] = 0;
    }

    // The map cell offsets of the eight facings (0x0079C484).
    static const int _offsets[8] = {
        -512,
        -512 + 1,
        1,
        512 + 1,
        512,
        512 - 1,
        -1,
        -512 - 1
    };

    bool is_train = false;
    if (foot->Techno_Type_Class()->IsTrain) {
        is_train = true;
        for (face = 0; face <= 8; face++) {
            int facing_diff = Dir_To_8(foot->unknown_2e0.Current()) - face;
            int abs_diff = abs(facing_diff);
            if (abs_diff > 2 && abs_diff < 6 && face != 8) {
                CellClass *&neighbor = from_pptr[_offsets[face]];
                int neighbor_index = neighbor->MapCoords.X + unknown_map_cell_side_84e804 * neighbor->MapCoords.Y;
                if (CurrentCellHeight > neighbor->unknown_11b + 1) {
                    RegularBridgeVisited[neighbor_index] = UniqueID;
                    RegularBridgeMovementCosts[neighbor_index] = 0;
                } else {
                    RegularVisited[neighbor_index] = UniqueID;
                    RegularMovementCosts[neighbor_index] = 0;
                }
            }
        }
    }

    bool is_passive = foot->Kind_Of() == 1 && ((UnitClass *)foot)->Type->Passive;

    if (max_loops < 0) {
        max_loops = 0xFFFF - 8;
    }

    while (working_node && tries < max_loops) {
        CellClass **working_from = working_node->unknown_00->unknown_00;
        if (working_from == to_pptr && working_node->unknown_00->unknown_04 == DestCellHeight) {
            break;
        }
        Cell &from_id = working_from[0]->MapCoords;
        RegularOpenNode *temp_node = 0;
        int from_index = from_id.X + unknown_map_cell_side_84e804 * from_id.Y;

        for (face = 0; face <= 8; face++) {

            CellClass **neighbor_slot;
            if (face == 8) {
                int tube = (*working_from)->unknown_116;
                if (tube != -1) {
                    Cell exit_cell = unknown_TubeClass_list_008661c8[tube]->ExitCell;
                    neighbor_slot = Native_Map_Cell_Slot(512 * exit_cell.Y + exit_cell.X);
                } else {
                    static CellClass *NoneCell = 0;
                    neighbor_slot = &NoneCell;
                }
            } else {
                neighbor_slot = &working_from[_offsets[face]];
            }

            CellClass *neighbor_cell = *neighbor_slot;
            CellClass **working_to = neighbor_slot;

            if (neighbor_cell == 0) {
                continue;
            }

            Cell &neighbor_id = neighbor_cell->MapCoords;
            int node_index;
            if (face != 8) {
                node_index = from_index + unknown_path_facing_offsets_84c838[face];
            } else {
                node_index = neighbor_cell->MapCoords.X + unknown_map_cell_side_84e804 * neighbor_cell->MapCoords.Y;
            }

            bool base_level = !(neighbor_cell->unknown_140 & 0x100) || abs(CurrentCellHeight - neighbor_cell->unknown_11b) <= 1;

            int zone_index = Map.Get_Cell_Zone_Index(neighbor_id);
            char *subzones = Map.CellSubzones;

            short subzone_id = *(short *)(subzones + zone_index * 10);
            if (fine_final_ids[subzone_id] != UniqueID && base_level && !neighbor_cell->unknown_122 && with_hs) {
                continue;
            }

            if (base_level && Is_Visited(0, true, node_index) && (working_node->unknown_04 + 1.009) > RegularMovementCosts[node_index]) {
                continue;
            }

            if (!base_level && Is_Visited(0, false, node_index) && (working_node->unknown_04 + 1.009) > RegularBridgeMovementCosts[node_index]) {
                continue;
            }
            int move = foot->Can_Enter_Cell(neighbor_cell, face, CurrentCellHeight, *working_from, UseLocomotorEnterCheck);
            if (is_train && move < 7) {
                move = 0;
            }

            float movement_cost;
            if (face != 8) {
                movement_cost = Get_Movement_Cost(working_from, working_to, base_level == 0, move, foot) * MovementCostMultiplier + _facing_costs[face];
            } else {
                CellClass &from_cell = *working_from[0];
                CellClass &to_cell = *neighbor_cell;
                int dy = abs((int)from_cell.MapCoords.Y - (int)to_cell.MapCoords.Y);
                int dx = abs((int)from_cell.MapCoords.X - (int)to_cell.MapCoords.X);
                movement_cost = dx > dy ? dx : dy;
            }

            if (move < 7) {
                if (Is_Visited(0, base_level, node_index)) {
                    continue;
                }

                RegularOpenNode *new_node = Create_Node(working_node, working_to, to, movement_cost);
                if (temp_node == 0) {
                    temp_node = new_node;
                } else {
                    // The original's INC at 0x00427939 overwrites the score-test ZF.
                    // With a valid queue count, it always queues the prior temporary.
                    Native_Regular_Insert(RegularQueue, *temp_node);
                    temp_node = new_node;
                }

                if (base_level) {
                    RegularVisited[node_index] = UniqueID;
                    RegularMovementCosts[node_index] = new_node->unknown_04;
                } else {
                    RegularBridgeVisited[node_index] = UniqueID;
                    RegularBridgeMovementCosts[node_index] = new_node->unknown_04;
                }
                if (subzone_id == HierSubzonePath[0][HierNodeIndex + 1]) {
                    HierNodeIndex++;
                    HierLastNodeCell = neighbor_cell->MapCoords;
                }
            } else if (working_to == to_pptr && !is_passive) {
                if (abs(CurrentCellHeight - DestCellHeight) <= 1) {
                    goto breakout;
                }
            }
        }

        if (temp_node) {
            working_node = Native_Replace_Root(RegularQueue, *temp_node);
        } else {
            working_node = Native_Regular_Extract_Min(RegularQueue);
        }

        if (working_node) {
            CurrentCellHeight = working_node->unknown_00->unknown_04;
        }
        tries++;
    }

breakout:

    if (tries == 10000 || working_node == 0 || tries == max_loops || working_node->unknown_0c < 2) {
        if (Avoidance != 0) {
            Apply_Path_Collision_Avoidance(foot);
        }
        return 0;
    }

    PathStruct *final_path = Build_Final_Path(working_node, moves);
    Cut_Corners(final_path, foot);

    Optimize_Moves(final_path, foot);

    if (Avoidance != 0) {
        Apply_Path_Collision_Avoidance(foot);
    }
    return final_path;
}
#endif

// AStarClass::AStarClass() (0x00427FB0)
#ifndef VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#endif
#ifndef VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR
#endif

AStarClass::AStarClass()
    : unknown_00(false), IsAvoidBridges(false), unknown_02(false), IsAvoidPathCollision(true), MovementCostMultiplier(1.0f), UseLocomotorEnterCheck(true),
      RegularVisited(0), RegularBridgeVisited(0), RegularBridgeMovementCosts(0), RegularMovementCosts(0), UniqueID(-1), ObjectSpeed(-1), IsHSEnabled(true),
      Avoidance(0), HierNodeIndex(-1), HierLastNodeCell(0, 0)
{
    RegularQueue = new UnknownPathEstimatorBlock(0x10000);
    HierQueue = new UnknownPathEstimatorBlock(0x2710);
    RegularOpenNodes = new RegularOpenNodePool;
    RegularNodes = new RegularNodePool;
    RegularNodes->count = 0;
    RegularOpenNodes->count = 0;
    RegularQueue->Clear();
    HierQueue->Clear();
    for (int index = 0; index < 3; index++) {
        HierOnPath[index] = 0;
        HierOpened[index] = 0;
        HierCosts[index] = 0;
        HierBannedEdges[index].Clear();
        for (int slot = 0; slot < 500; slot++) {
            HierSubzonePath[index][slot] = 0;
        }
        HierSubzonePathCount[index] = 0;
    }
    HierNodePool = new char[0x27100];
}


// AStarClass::~AStarClass() (0x004281E0)
// Frees the estimator's blocks and tables; the three vectors at +0x74 are
// destroyed as members.
#ifndef VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR
#endif
#ifndef VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#endif

AStarClass::~AStarClass()
{
    UnknownPathEstimatorBlock *block = RegularQueue;
    if (block) {
        delete block->unknown_08;
        delete block;
    }
    RegularQueue = 0;
    block = HierQueue;
    if (block) {
        delete block->unknown_08;
        delete block;
    }
    HierQueue = 0;
    if (RegularOpenNodes) {
        delete RegularOpenNodes;
    }
    RegularOpenNodes = 0;
    if (RegularNodes) {
        delete RegularNodes;
    }
    RegularNodes = 0;
    if (RegularVisited) {
        delete RegularVisited;
        RegularVisited = 0;
    }
    if (RegularBridgeVisited) {
        delete RegularBridgeVisited;
        RegularBridgeVisited = 0;
    }
    if (RegularMovementCosts) {
        delete RegularMovementCosts;
        RegularMovementCosts = 0;
    }
    if (RegularBridgeMovementCosts) {
        delete RegularBridgeMovementCosts;
        RegularBridgeMovementCosts = 0;
    }
    for (int index = 0; index < 3; index++) {
        if (HierOnPath[index]) {
            delete HierOnPath[index];
            HierOnPath[index] = 0;
        }
        if (HierOpened[index]) {
            delete HierOpened[index];
            HierOpened[index] = 0;
        }
        if (HierCosts[index]) {
            delete HierCosts[index];
            HierCosts[index] = 0;
        }
    }
    delete HierNodePool;
}


// AStarClass::Facing_Between() (0x00428320): the facing between
// two adjacent cells.

int __fastcall AStarClass::Facing_Between(Cell const &from, Cell const &to)
{
    if (abs(to.Y - from.Y) <= 1 && abs(to.X - from.X) <= 1) {
        return unknown_path_facing_table_7cf100[to.Y - from.Y + 1][to.X - from.X + 1];
    }
    return 8;
}


// AStarClass::Build_Final_Path() (0x00428370): the path from the
// search's start to a node.

PathStruct *AStarClass::Build_Final_Path(RegularOpenNode *node, int *buffer)
{
    static PathStruct path;
    path.Cost = node->unknown_08;
    path.Length = node->unknown_0c;
    path.Overlap = 0;
    path.LastOverlap.Set(0, 0);
    path.Command = buffer;
    path.Height = unknown_path_levels_84c858;
    RegularNode *entry = node->unknown_00;
    RegularNode *parent = entry->unknown_08;
    for (int index = node->unknown_0c - 2; index >= 0; index--) {
        if (parent) {
            unknown_path_levels_84c858[index] = parent->unknown_04;
            buffer[index] = Facing_Between((*entry->unknown_00)->MapCoords, (*parent->unknown_00)->MapCoords);
        }
        entry = entry->unknown_08;
        parent = parent->unknown_08;
    }
    buffer[node->unknown_0c - 1] = -1;
    path.Start = (*entry->unknown_00)->MapCoords;
    if (path.Cost == 0) {
        path.Cost = 1;
    }
    return &path;
}


// The atexit destructor (0x004284D0, _$E) of the function-local static path in
// AStarClass::Build_Final_Path(); no source of its own.


// AStarClass::Update_Map_Dimensions() (0x004284E0): sizes the cell marks
// and the facing offsets for the map rectangle.
// Shared map-grid dimension; this pathfinding routine needs no MapRegionClass layout.
extern int unknown_map_cell_side_84e804;

void AStarClass::Update_Map_Dimensions(Rect const &size)
{
    if (RegularVisited) {
        delete RegularVisited;
        RegularVisited = 0;
    }
    if (RegularBridgeVisited) {
        delete RegularBridgeVisited;
        RegularBridgeVisited = 0;
    }
    if (RegularMovementCosts) {
        delete RegularMovementCosts;
        RegularMovementCosts = 0;
    }
    if (RegularBridgeMovementCosts) {
        delete RegularBridgeMovementCosts;
        RegularBridgeMovementCosts = 0;
    }
    unknown_map_cell_side_84e804 = size.Width + size.Height + 1;
    int count = unknown_map_cell_side_84e804 * unknown_map_cell_side_84e804;
    RegularBridgeVisited = new int[count];
    RegularVisited = new int[count];
    RegularMovementCosts = new float[count];
    RegularBridgeMovementCosts = new float[count];
    unknown_path_facing_offsets_84c838[0] = -unknown_map_cell_side_84e804;
    unknown_path_facing_offsets_84c838[1] = 1 - unknown_map_cell_side_84e804;
    unknown_path_facing_offsets_84c838[2] = 1;
    unknown_path_facing_offsets_84c838[3] = unknown_map_cell_side_84e804 + 1;
    unknown_path_facing_offsets_84c838[4] = unknown_map_cell_side_84e804;
    unknown_path_facing_offsets_84c838[5] = unknown_map_cell_side_84e804 - 1;
    unknown_path_facing_offsets_84c838[6] = -1;
    unknown_path_facing_offsets_84c838[7] = -1 - unknown_map_cell_side_84e804;
}


#include "shared/techno_type.hpp"
Cell __fastcall CellClass::Adjacent_Cell(Cell const &cell,int facing)
{
    return cell + AdjacentCell[facing&7];
}
void AStarClass::Apply_Path_Collision_Avoidance(FootClass *foot)
{
    // Four-byte view of CellClass flag word +0x140; bit18 is the predicted path mark.
    // The alias/layout spelling remains a hypothesis; original 32-bit mask is independently verified.
    struct PredictedPathFlags {
        unsigned :18;
        unsigned IsPredictedPath:1;
        unsigned :13;
    };
    if(IsAvoidPathCollision) {
    Cell current=foot->unknown_slot104();
    CellClass *currentCell=&Map[current];
    CellClass *next=&Map[CellClass::Adjacent_Cell(current,Dir_To_8(foot->unknown_2e0.Current()))];
    bool bridge=(next->unknown_140&0x100)!=0 &&
        (abs(currentCell->unknown_11b-next->unknown_11b)>3 || foot->unknown_79);
    FootClass *blocker=(FootClass*)(bridge?next->AltOccupierPtr:next->OccupierPtr);
    if(!blocker)blocker=(FootClass*)Find_Moving_Blocker(next->MapCoords,next->unknown_11b+(bridge?4:0));
    bool marked=false;
    TechnoTypeClass const *type=foot->Techno_Type_Class();
    while(blocker) {
        if(blocker->Kind_Of()==1 || blocker->Kind_Of()==15) {
            int index=0;
            Cell position=blocker->unknown_450;
            TechnoTypeClass const *other=blocker->Techno_Type_Class();
            if(Avoidance==2 || (type!=other && type->Speed>other->Speed && Map.Unknown_0055A660(position,true))) {
                if(blocker->Kind_Of()==1) {
                    if(blocker->Path[0]==-1 || blocker->Path[1]==-1) {blocker=(FootClass*)blocker->unknown_30;continue;}
                } else {
                    if(blocker->Path[0]==-1 || blocker->Path[1]==-1 || blocker->Path[2]==-1) {blocker=(FootClass*)blocker->unknown_30;continue;}
                }
                marked=true;
                while(blocker->Path[index]!=-1 && index<24) {
                    Cell nextPosition=Next_Cell(position,blocker->Path[index]);
                    position=nextPosition;
                    reinterpret_cast<PredictedPathFlags *>(&Map[position].unknown_140)->IsPredictedPath=!reinterpret_cast<PredictedPathFlags *>(&Map[position].unknown_140)->IsPredictedPath;
                    ++index;
                }
            } else {
                blocker=(FootClass*)blocker->unknown_30;
                continue;
            }
        }
        blocker=(FootClass*)blocker->unknown_30;
    }
    if(!marked && Avoidance==1) {Avoidance=0;return;}
    for(int x=-2;x<3;++x) {
        for(int y=-2;y<3;++y) {
            CellClass *neighbor=&Map[next->Fetch_CellID()+Cell(x,y)];
            if(neighbor->unknown_124) {
                Cell neighborPosition=neighbor->Fetch_CellID();
                if(neighborPosition!=current)reinterpret_cast<PredictedPathFlags *>(&neighbor->unknown_140)->IsPredictedPath=!reinterpret_cast<PredictedPathFlags *>(&neighbor->unknown_140)->IsPredictedPath;
            }
        }
    }
    reinterpret_cast<PredictedPathFlags *>(&next->unknown_140)->IsPredictedPath=!reinterpret_cast<PredictedPathFlags *>(&next->unknown_140)->IsPredictedPath;
    }
}


extern int unknown_path_level_scale_84e800;
ObjectClass *AStarClass::Find_Moving_Blocker(Cell const &cell,int level)
{
    CoordStruct destination=Cell_Coord(cell);
    destination.Z=level*unknown_path_level_scale_84e800;
    for(int y=-2;y<3;++y) {
        for(int x=-2;x<3;++x) {
            CellClass &neighbor=Map[Cell(x,y)+cell];
            bool alternate=(neighbor.unknown_140&0x100)!=0 && abs(neighbor.unknown_11b-level)>2;
            FootClass *object=(FootClass*)(alternate?neighbor.AltOccupierPtr:neighbor.OccupierPtr);
            while(object) {
                bool isFoot;
                if(object!=0)isFoot=object->AbstractFlags.IsFoot;
                if(object!=0 && isFoot) {
                    if(object->Locomotor.Checked()->Is_Moving_Here(destination))return object;
                }
                object=(FootClass*)object->unknown_30;
            }
        }
    }
    return 0;
}


// int __fastcall unknown_cell_index_428a90(Cell const &cell) (0x00428A90)
// The index of a cell in the map's isometric cell block (0x00428AC0 counts them).
#include "shared/veinhole.hpp"

int __fastcall unknown_cell_index_428a90(Cell const &cell)
{
    return (cell.X - Map.MapSize.Width - 1 + cell.Y) * Map.MapSize.Width + ((cell.X - cell.Y - 1 + Map.MapSize.Width) >> 1);
}


// int __fastcall unknown_cells_428ac0() (0x00428AC0)
// The number of cells of the map's isometric cell block: (height + 4) rows of
// twice the width (TiberiumClass sizes its growth tables with it).

int __fastcall unknown_cells_428ac0()
{
    return (Map.MapSize.Height + 4) * Map.MapSize.Width * 2;
}


void AStarClass::Cut_Corners(PathStruct *path,FootClass *foot)
{
    int maximum=path->Length;
    --maximum;
    unsigned secondDirection=(unsigned)-1;
    int *heights=path->Height;
    int *moves=path->Command;
    Cell source=path->Start;
    int firstStart=0,secondStart=0;
    bool corner=false;
    int firstLength=0,secondLength=0;
    Cell start=source;
    Cell cursor=source;
    unsigned firstDirection=(unsigned)-1;
    while(firstStart+firstLength<maximum) {
        if(secondStart+secondLength>=maximum)break;
        if(!corner) {
            unsigned direction=moves[firstStart+firstLength];
            unsigned turn=(direction-firstDirection)&7;
            if(direction==firstDirection)++firstLength;
            else if((turn==2 || turn==6) && firstDirection!=-1 && firstDirection!=8 && direction!=8) {
                corner=true;secondDirection=direction;secondLength=1;
                secondStart=firstStart+firstLength;
            } else {
                firstStart+=firstLength;firstLength=1;
                firstDirection=(direction&1)?direction:-1;
                start=cursor;
            }
            cursor=Next_Cell(cursor,direction);
        } else {
            if(moves[secondStart+secondLength]==secondDirection)++secondLength;
            else {
                firstStart+=Try_Diagonal_Shortcut(foot,moves+firstStart,heights+firstStart,firstLength,secondLength,start);
                firstLength=1;corner=false;
                cursor=start+AdjacentCell[moves[firstStart]&7];
                firstDirection=moves[firstStart];
            }
        }
    }
    if(corner)Try_Diagonal_Shortcut(foot,moves+firstStart,heights+firstStart,firstLength,secondLength,start);
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// AStarClass::Try_Diagonal_Shortcut() (0x00428CF0): replaces a corner
// of two legs of a path by a diagonal where foot can walk it (Tiberian
// Sun's Try_Diagonal_Shortcut; hypothesis). From ra2-decomp AStarClass::Try_Diagonal_Shortcut
// (YR 0x0042B420), adapted to our names (Scout803 td0, Scout849 td_d FacingType locals; Scout857 r1).
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
            is_blocked = foot->Can_Enter_Cell(scan_cell_ptr, diagonal_direction, scan_height, 0, 1) != 0
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


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// AStarClass::Optimize_Moves() (0x004290C0): replaces wandering
// runs of a found path by straight lines where foot can walk them, then packs
// out the emptied moves (Tiberian Sun's Optimize_Moves; hypothesis).
// From ra2-decomp AStarClass::Optimize_Moves (YR 0x0042B7F0), adapted to our names.


// The cell next to one toward a facing, the cell taken by value.
static inline Cell Adjacent_Cell_By_Value(Cell cell, int facing)
{
    Cell const &offset = AdjacentCell[facing & 7];
    return Cell(cell.X + offset.X, cell.Y + offset.Y);
}


// Westwood's max template (defined with the Test_Cell_Walk row of this unit).
template <class T> T max(T a, T b);

void AStarClass::Optimize_Moves(PathStruct *path, FootClass *foot)
{
    int splice_index;
    Cell splice_cell;
    int *moves = path->Command;
    int max_len = path->Length;
    max_len--;
    int scan_index = 0;
    int prev_turn_index = 0;
    Cell cursor_cell = path->Start;
    int *heights = path->Height;
    int index;
    int last_turn_index = 0;
    int max_axis;
    int max_axis_extent = 0;
    Cell path_offset(0, 0);
    Cell last_turn_cell(0, 0);
    int max_dev_x = 0;
    int max_dev_y = 0;
    Cell candidate_offset(0, 0);
    while (scan_index < max_len && scan_index < 20) {
        int move = moves[scan_index];
        int step = moves[scan_index];

        if (step == 8) {
            cursor_cell = Adjacent_Cell_By_Value(cursor_cell, 0);
            path_offset = Cell(0, 0);
            scan_index++;
            prev_turn_index = scan_index;
            max_axis_extent = 0;
            max_dev_x = 0;
            max_dev_y = 0;
            candidate_offset = Cell(0, 0);
            last_turn_index = scan_index;
            last_turn_cell = Cell(0, 0);
            continue;
        }

        if (move == -2) {
            scan_index++;
            continue;
        }

        Cell new_offset = path_offset + AdjacentCell[move & 7];
        Cell new_candidate_offset = AdjacentCell[move & 7] + candidate_offset;
        if (abs(new_candidate_offset.X) >= max_dev_x && abs(new_candidate_offset.Y) >= max_dev_y) {

            candidate_offset = new_candidate_offset;

            max_dev_x = abs(new_candidate_offset.X);
            max_dev_y = abs(new_candidate_offset.Y);

            max_axis = max(abs(new_offset.X), abs(new_offset.Y));

            cursor_cell = Adjacent_Cell_By_Value(cursor_cell, step);
            if (max_axis_extent < max_axis) {
                max_axis_extent = max_axis;
            } else {
                splice_cell = cursor_cell;
                Splice_Path(moves, scan_index, prev_turn_index, &splice_index, &splice_cell);

                Plot_Straight_Line(&moves[splice_index], scan_index - splice_index + 1, splice_cell,
                    Cell(cursor_cell - splice_cell), foot, heights[splice_index], false);
            }

            path_offset = new_offset;
            ++scan_index;
        } else {
            if (last_turn_cell == Cell(0, 0)) {
                max_dev_x = 0;
                max_dev_y = 0;
                last_turn_cell = cursor_cell;
                last_turn_index = scan_index;
                candidate_offset = Cell(0, 0);
            } else {
                prev_turn_index = last_turn_index;
                path_offset = last_turn_cell - cursor_cell;
                max_dev_x = 0;
                max_dev_y = 0;
                candidate_offset = Cell(0, 0);
                max_axis_extent = max(abs(path_offset.X), abs(path_offset.Y));
                last_turn_cell = cursor_cell;
                last_turn_index = scan_index;
            }
        }
    }
    if (last_turn_cell != Cell(0, 0)) {
        Cell diff = cursor_cell - last_turn_cell;
        max_axis = max(abs(diff.X), abs(diff.Y));
        if ((scan_index - last_turn_index - 1) > max_axis) {
            scan_index--;
            splice_cell = cursor_cell;
            Splice_Path(moves, scan_index, last_turn_index, &splice_index, &splice_cell);

            Plot_Straight_Line(&moves[splice_index], scan_index - splice_index + 1, splice_cell,
                Cell(cursor_cell - splice_cell), foot, heights[splice_index], true);
        }
    }
    index = 0;
    int new_len = 0;
    int dir = moves[index];
    while (dir != -1 && index < max_len) {
        if (dir != -2) {
            moves[new_len++] = moves[index];
        }
        dir = moves[++index];
    }
    int fill = new_len;
    while (fill < path->Length + 1) {
        moves[fill++] = -1;
    }

    path->Length = new_len + 1;
}


// AStarClass::Splice_Path() (0x00429570): walks a facing list
// backwards from index to stop until the walk turns back toward its start.


void AStarClass::Splice_Path(int const *facings, int index, int stop, int *out_index, Cell *cell)
{
    Cell offset(0, 0);
    int farthest = 0;
    int step = index;
    Cell position = *cell;
    bool returning = false;
    while (step >= stop) {
        int facing = facings[step];
        if (facing == -2) {
            step--;
            continue;
        }
        int back = (facing - 4) & 7;
        offset = AdjacentCell[back & 7] + offset;
        position = AdjacentCell[back & 7] + position;
        int distance = abs(offset.X) > abs(offset.Y) ? abs(offset.X) : abs(offset.Y);
        if (distance > farthest) {
            if (returning) {
                *out_index = step + 1;
                *cell = AdjacentCell[(back - 4) & 7] + position;
                return;
            }
            farthest = distance;
        } else {
            returning = true;
        }
        step--;
    }
    *out_index = stop;
    *cell = position;
}


// AStarClass::Plot_Straight_Line() (0x004296F0): tries the two-leg
// straight path (a diagonal then a straight run, or the reverse) to a cell
// offset and writes its facings (Tiberian Sun's Plot_Straight_Line;
// hypothesis).

// The cell next to one toward a facing (the guarded block of shared/display.hpp, shared/terrain.hpp,
// shared/infantry.hpp and shared/aircraft.hpp; those headers are too heavy for this unit). Here the
// sum is spelled member-wise through Cell's int constructor.
#ifndef RA2_ADJACENT_CELL
#define RA2_ADJACENT_CELL
#include "shared/adjacent_cell.hpp"
inline Cell Adjacent_Cell(Cell const &cell, int facing)
{
    Cell const &offset = AdjacentCell[facing & 7];
    return Cell(cell.X + offset.X, cell.Y + offset.Y);
}
#endif

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

template <class T> void swap(T &left, T &right)
{
    T temp = left;
    left = right;
    right = temp;
}

bool AStarClass::Plot_Straight_Line(int *moves, int move_count, Cell const &from, Cell const &to, FootClass *foot, int level, bool fearless)
{
    int tries = 0;
    int x = to.X;
    int y = to.Y;
    int first_dir;
    if (to.X < 0) {
        first_dir = to.Y < 0 ? 7 : 5;
    } else {
        first_dir = to.Y < 0 ? 1 : 3;
    }
    int sub_xy = x - y;
    int sum_xy = x + y;
    int second_dir;
    if (sub_xy > 0) {
        second_dir = sum_xy > 0 ? 2 : 0;
    } else {
        second_dir = sum_xy > 0 ? 4 : 6;
    }
    int y_dist = abs(y);
    int x_dist = abs(x);
    int first_dist = MIN(x_dist, y_dist);
    int second_dist = MAX(x_dist, y_dist) - first_dist;
    double avoidance = foot->unknown_4cafb0();
    int threats = 0;
    HouseClass *house = foot->House;
    bool avoid_threats = avoidance > 0.00001;
    bool blocked = false;
    while (tries < 2) {
        threats = 0;
        if (first_dist != 0) {
            int height = level;
            blocked = false;
            Cell current = from;
            int count1 = first_dist;
            int count2 = second_dist;
            while (count1 > 0 && !blocked) {
                current = Adjacent_Cell(current, first_dir);
                CellClass *cellptr = &Map[current];
                if (avoid_threats && Map.Cell_Threat(current, house) * avoidance >= 0.01) {
                    threats++;
                }
                blocked = foot->Can_Enter_Cell(cellptr, first_dir, height, 0, 1) != 0 || (cellptr->unknown_140 & 0x40000) || threats > 3 || (!fearless && threats > 0);
                count1--;
                int step_height = cellptr->unknown_11b;
                if (height - step_height == 4) {
                    height = step_height + 4;
                    if (cellptr->unknown_140 & 0x100) {
                        continue;
                    }
                }
                height = step_height;
            }
            while (count2 > 0 && !blocked) {
                current = Adjacent_Cell(current, second_dir);
                CellClass *cellptr = &Map[current];
                if (avoid_threats && Map.Cell_Threat(current, house) * avoidance >= 0.01) {
                    threats++;
                }
                blocked = foot->Can_Enter_Cell(cellptr, second_dir, height, 0, 1) != 0 || (cellptr->unknown_140 & 0x40000) || threats > 3 || (!fearless && threats > 0);
                count2--;
                int step_height = cellptr->unknown_11b;
                if (height - step_height == 4) {
                    height = step_height + 4;
                    if (cellptr->unknown_140 & 0x100) {
                        continue;
                    }
                }
                height = step_height;
            }
        }
        if (first_dist == 0 || blocked) {
            swap(second_dir, first_dir);
            swap(second_dist, first_dist);
        } else {
            int i;
            for (i = 0; i < first_dist; i++) {
                moves[i] = first_dir;
            }
            for (i = 0; i < second_dist; i++) {
                moves[first_dist + i] = second_dir;
            }
            for (i = 0; i < move_count - first_dist - second_dist; i++) {
                moves[first_dist + second_dist + i] = -2;
            }
            return true;
        }
        tries++;
    }
    return false;
}


// AStarClass::Reset() (0x00429A90): reallocates and
// clears the three layers' arrays.

void AStarClass::Reset()
{
    for (int layer = 0; layer < 3; layer++) {
        if (HierOnPath[layer]) {
            delete HierOnPath[layer];
        }
        int *array = new int[Map.unknown_70[layer]];
        HierOnPath[layer] = array;
        for (int entry = Map.unknown_70[layer] - 1; entry >= 0; entry--) {
            array[entry] = 0;
        }
        if (HierOpened[layer]) {
            delete HierOpened[layer];
        }
        array = new int[Map.unknown_70[layer]];
        HierOpened[layer] = array;
        for (entry = Map.unknown_70[layer] - 1; entry >= 0; entry--) {
            array[entry] = 0;
        }
        if (HierCosts[layer]) {
            delete HierCosts[layer];
        }
        array = new int[Map.unknown_70[layer]];
        HierCosts[layer] = array;
        for (entry = Map.unknown_70[layer] - 1; entry >= 0; entry--) {
            array[entry] = 0;
        }
    }
}


// AStarClass::Find_Path_Hierarchical() (0x00429B60): the subzone search
// (Tiberian Sun's Find_Path_Hierarchical): each layer from the coarsest
// searches the subzone graph inside the corridor the coarser layer found and
// records its subzone path in +0xBC.
#include "shared/zone_table.hpp"
// The search cost of each passability (0x0079C4A4).
extern "C" float const unknown_path_passability_scores_79c4a4[8];

bool AStarClass::Find_Path_Hierarchical(Cell const &from, Cell const &to, int zone, FootClass *foot)
{
    int *pass_table = unknown_table_7e00c4[zone];
    double threat_avoidance = foot != 0 ? foot->unknown_4cafb0() : 0.0;
    HouseClass *house = foot != 0 ? foot->House : 0;
    bool avoid_threats = threat_avoidance > 0.00001;

    for (int layer = 2; layer >= 0; layer--) {
        HierQueue->Clear();

        short *start_subzones = (short *)(Map.CellSubzones + Map.Get_Cell_Zone_Index(from) * 10);
        int start_subzone = start_subzones[layer];
        short *end_subzones = (short *)(Map.CellSubzones + Map.Get_Cell_Zone_Index(to) * 10);
        int end_subzone = end_subzones[layer];

        bool is_coarse = layer == 2;
        int *coarser_final_ids;
        if (!is_coarse) {
            coarser_final_ids = HierOnPath[layer + 1];
        } else {
            coarser_final_ids = 0;
        }

        int *final_ids = HierOnPath[layer];
        int *working_ids = HierOpened[layer];
        float *costs = (float *)HierCosts[layer];
        final_ids[start_subzone] = UniqueID;
        final_ids[end_subzone] = UniqueID;

        if (start_subzone == end_subzone) {
            if (layer == 0) {
                AStarHierarchicalNode *node = &((AStarHierarchicalNode *)HierNodePool)[0];
                node->Depth = 0;
                node->SubzoneID = start_subzone;
            }
            HierSubzonePath[layer][0] = start_subzone;
            HierSubzonePathCount[layer] = 1;
        } else {
            int node_count = 0;
            AStarHierarchicalNode *start_node = &((AStarHierarchicalNode *)HierNodePool)[0];
            start_node->ParentIndex = -1;
            start_node->SubzoneID = start_subzone;
            start_node->Score = 0.0;
            start_node->Depth = 0;

            HierQueue->Insert(*start_node);
            node_count++;
            working_ids[start_subzone] = UniqueID;
            costs[start_subzone] = 0.0;

            AStarHierarchicalNode *best_node = HierQueue->Extract_Min();
            bool no_banned_edges = HierBannedEdges[layer].Count() == 0;

            while (best_node != 0) {
                int from_subzone = best_node->SubzoneID;
                if (from_subzone == end_subzone) {
                    break;
                }

                SubzoneConnectionStruct *connection = &Map.SubzoneTracking[layer][from_subzone].Connections[0];
                int conn_count = Map.SubzoneTracking[layer][from_subzone].Connections.Count();
                for (int conn_idx = 0; conn_idx < conn_count; conn_idx++) {
                    int threat = 0;
                    int to_subzone = connection[conn_idx].unknown_00;
                    bool is_cross_block = connection[conn_idx].unknown_04;
                    int passability = Map.SubzoneTracking[layer][to_subzone].unknown_1c;
                    int to_coarser_subzone = (unsigned short)Map.SubzoneTracking[layer][to_subzone].unknown_18;

                    if (avoid_threats) {
                        threat = Map.Unknown_00567D20(house, layer, from_subzone, to_subzone) * threat_avoidance;
                    }

                    double extra_score = is_cross_block ? 0.001 : 0.0;

                    float score = unknown_path_passability_scores_79c4a4[passability] + best_node->Score + (double)threat + extra_score;

                    if ((working_ids[to_subzone] != UniqueID || costs[to_subzone] > score) && (is_coarse || coarser_final_ids[to_coarser_subzone] == UniqueID || passability == 1) && pass_table[passability] == 1) {
                        if (!no_banned_edges) {
                            unsigned short first = from_subzone;
                            unsigned short second = to_subzone;
                            if (second < first) {
                                unsigned short swap = first;
                                first = second;
                                second = swap;
                            }
                            unsigned key = ((unsigned)first << 16) | second;
                            DynamicVectorClass<unsigned int> &list = HierBannedEdges[layer];
                            bool banned = false;
                            for (int index = list.Count() - 1; index >= 0; index--) {
                                if (list[index] == key) {
                                    banned = true;
                                    break;
                                }
                            }
                            if (banned) {
                                continue;
                            }
                        }
                        AStarHierarchicalNode *new_node = &((AStarHierarchicalNode *)HierNodePool)[node_count];
                        new_node->ParentIndex = best_node - (AStarHierarchicalNode *)HierNodePool;
                        new_node->SubzoneID = to_subzone;
                        new_node->Score = score;
                        new_node->Depth = best_node->Depth + 1;
                        HierQueue->Insert(*new_node);
                        working_ids[to_subzone] = UniqueID;
                        costs[to_subzone] = score;
                        node_count++;
                    }
                }
                best_node = HierQueue->Extract_Min();
            }

            if (best_node == 0) {
                return false;
            }

            AStarHierarchicalNode *tail_node = best_node;
            while (best_node->ParentIndex != -1) {
                final_ids[best_node->SubzoneID] = UniqueID;
                best_node = &((AStarHierarchicalNode *)HierNodePool)[best_node->ParentIndex];
            }

            int path_count = tail_node->Depth + 1;
            HierSubzonePathCount[layer] = path_count;
            path_count--;
            while (path_count > 0) {
                HierSubzonePath[layer][path_count] = tail_node->SubzoneID;
                tail_node = &((AStarHierarchicalNode *)HierNodePool)[tail_node->ParentIndex];
                path_count--;
            }

            HierSubzonePath[layer][0] = tail_node->SubzoneID;
        }
    }
    return true;
}


// Hierarchical corridor selection and regular-search retries, 0x0042A1D0.
#include "shared/infantry.hpp"
#include "shared/debug_printf.hpp"
extern "C" char unknown_path_hierarchical_failure[];
extern "C" char unknown_path_regular_failure[];
extern "C" char unknown_path_without_hs[];

PathStruct *AStarClass::Find_Path(Cell const &from, Cell const &to, FootClass *foot, int *moves, int max_loops, int zone, int avoidance)
{
    IsHSEnabled = true;
    Clear();
    for (int i = 0; i < 3; ++i) {
        HierBannedEdges[i].Clear();
    }
    Avoidance = avoidance;
    CellClass *from_cell = &Map[from];
    CellClass *to_cell = &Map[to];
    int from_zone = Map.Zone(from, zone != -1 ? zone : foot->Techno_Type_Class()->MovementZone, foot->unknown_79);
    int to_zone = Map.Zone(to, zone != -1 ? zone : foot->Techno_Type_Class()->MovementZone, (Map[to].unknown_140 & 0x100) != 0);
    Cell hs_from = Map.Get_Bridge_Zone_Connection_Cell(from_cell, foot->unknown_79);
    Cell hs_to = Map.Get_Bridge_Zone_Connection_Cell(to_cell, (to_cell->unknown_140 & 0x100) != 0);
    int move_zone = zone != -1 ? zone : foot->Techno_Type_Class()->MovementZone;
    if (foot->Kind_Of() == 15 && ((InfantryClass *)foot)->Type->JumpJet) {
        move_zone = 7;
        UnknownComPtr<IPersist, &unknown_uuid_IPersist_7cf208> persist((ILocomotion *)foot->Locomotor);
        GUID id;
        persist.Checked()->GetClassID(&id);
    }
    bool with_hs;
    if (foot->Techno_Type_Class()->IsTrain) {
        with_hs = false;
    } else if (!foot->unknown_32d || foot->unknown_slot189()) {
        with_hs = false;
    } else if (!Map.Unknown_0055A660(hs_from, true) || !Map.Unknown_0055A660(hs_to, true)) {
        with_hs = false;
    } else {
        with_hs = true;
    }
    if (from_zone != to_zone) {
        if (with_hs) {
            return 0;
        }
    }
    if (with_hs) {
        if (!Find_Path_Hierarchical(hs_from, hs_to, move_zone, foot)) {
            Debug_Printf(unknown_path_hierarchical_failure, hs_from.X, hs_from.Y, hs_to.X, hs_to.Y);
            with_hs = false;
        }
    }
    PathStruct *path = 0;
    int attempt = 0;
    int tries = max_loops == -1 ? 5 : 1;
    while (true) {
        if (!with_hs) {
            Debug_Printf(unknown_path_without_hs, hs_from.X, hs_from.Y, hs_to.X, hs_to.Y);
        }
        path = Find_Path_Regular(from, to, foot, moves, max_loops, with_hs);
        if (path || !with_hs) {
            break;
        }
        int dy = abs(from.Y - to.Y);
        int dx = abs(from.X - to.X);
        if (dx <= dy) {
            dx = dy;
        }
        if (dx > 1) {
            Debug_Printf(unknown_path_regular_failure, hs_from.X, hs_from.Y, hs_to.X, hs_to.Y);
        }
        ++attempt;
        Ban_Blocked_Subzone_Edges(foot);
        Clear();
        with_hs = IsHSEnabled ? true : false;
        if (attempt >= tries) {
            break;
        }
        if (with_hs && !Find_Path_Hierarchical(hs_from, hs_to, move_zone, foot)) {
            break;
        }
    }
    return path;
}


// AStarClass::Ban_Blocked_Subzone_Edges() (0x0042A590): records, for each
// layer, the subzone pairs around the start cell.

// The cell's element of Map's +0x6C array: its subzone in each path estimator layer (a MapClass
// member inline here changes the scheduling of units that only see shared/map_view.hpp).
inline unsigned short *Cell_Subzones(Cell const &cell)
{
    return (unsigned short *)(Map.CellSubzones + Map.Get_Cell_Zone_Index(cell) * 10);
}

void AStarClass::Ban_Blocked_Subzone_Edges(FootClass *foot)
{
    unsigned short *subzones = Cell_Subzones(HierLastNodeCell);
    for (int layer = 0; layer < 3; layer++) {
        unsigned short subzone = subzones[layer];
        DynamicVectorClass<unsigned short> list;
        list.Clear();
        if (Map.Build_Reachable_Subzones(&Map[HierLastNodeCell], layer, list, (ObjectClass const *)foot)) {
            Ban_Neighborhood_Subzone_Edges(Cell_Subzones(HierLastNodeCell)[layer], layer);
            continue;
        }
        for (int index = list.Count() - 1; index >= 0; index--) {
            Ban_Subzone_Edge(subzone, list[index], layer);
        }
    }
}


// AStarClass::Subzone_Edge_Banned() (0x0042A770): whether a subzone
// pair is in a layer's list.

bool AStarClass::Subzone_Edge_Banned(unsigned short first, unsigned short second, int layer)
{
    if (second < first) {
        unsigned short swap = first;
        first = second;
        second = swap;
    }
    unsigned key = ((unsigned)first << 16) | second;
    DynamicVectorClass<unsigned int> &list = HierBannedEdges[layer];
    for (int index = list.Count() - 1; index >= 0; index--) {
        if (list[index] == key) {
            return true;
        }
    }
    return false;
}


// AStarClass::Ban_Subzone_Edge() (0x0042A7D0): records a subzone
// pair in a layer's list.

void AStarClass::Ban_Subzone_Edge(unsigned first, unsigned second, int layer)
{
    if (first == second) {
        return;
    }
    if (second < first) {
        unsigned swap = second;
        second = first;
        first = swap;
    }
    HierBannedEdges[layer].Add((first << 16) | second);
}


// AStarClass::Ban_Neighborhood_Subzone_Edges() (0x0042A840): records the subzone
// pairs around a subzone's step on a layer's +0xBC path.

void AStarClass::Ban_Neighborhood_Subzone_Edges(unsigned subzone, int layer)
{
    int count = HierSubzonePathCount[layer];
    if (count <= 1) {
        IsHSEnabled = false;
        return;
    }
    int found = -1;
    for (int index = 0; index < count; index++) {
        if (HierSubzonePath[layer][index] == subzone) {
            found = index;
            break;
        }
    }
    if (found == -1) {
        IsHSEnabled = false;
    } else {
        unsigned short second;
        unsigned short first;
        if (found == count - 1) {
            second = HierSubzonePath[layer][found];
            first = HierSubzonePath[layer][found - 1];
        } else {
            second = HierSubzonePath[layer][found + 1];
            first = HierSubzonePath[layer][found];
        }
        Ban_Subzone_Edge(second, first, layer);
        DynamicVectorClass<SubzoneConnectionStruct> &to = Map.SubzoneTracking[layer][second].Connections;
        DynamicVectorClass<SubzoneConnectionStruct> &from = Map.SubzoneTracking[layer][first].Connections;
        for (int other = to.Count() - 1; other >= 0; other--) {
            unsigned short next = to[other].unknown_00;
            if (next != first) {
                for (int index = from.Count() - 1; index >= 0; index--) {
                    if (from[index].unknown_00 == next) {
                        Ban_Subzone_Edge(first, next, layer);
                    }
                }
            }
        }
    }
}


// AStarClass::Test_Cell_Walk() (0x0042AA30): the estimated
// length of a walk between two cells (Tiberian Sun's Test_Cell_Walk;
// hypothesis).

template <class T> T max(T a, T b)
{
    return a > b ? a : b;
}

int AStarClass::Test_Cell_Walk(Cell const &from, Cell const &to, FootClass *foot, bool from_bridge, bool to_bridge, int arg6)
{
    IsHSEnabled = true;
    Clear();
    for (int index = 0; index < 3; index++) {
        HierBannedEdges[index].Clear();
    }
    CellClass *from_ptr = &Map[from];
    CellClass *to_ptr = &Map[to];
    Cell hs_from = Map.Get_Bridge_Zone_Connection_Cell(from_ptr, from_bridge);
    Cell hs_to = Map.Get_Bridge_Zone_Connection_Cell(to_ptr, to_bridge);
    if (arg6 == -1) {
        if (foot) {
            arg6 = foot->Techno_Type_Class()->MovementZone;
        } else {
            arg6 = 0;
        }
    }
    if (Find_Path_Hierarchical(hs_from, hs_to, arg6, foot)) {
        int radius = max(abs(from.X - to.X), abs(from.Y - to.Y));
        int node_count = HierSubzonePathCount[0];
        int distance = 2 * node_count - 2;
        bool add_bridge_dist = true;
        if (to_bridge && from_bridge) {
            int to_conn = Map.Zone_Connection_Index(to, 3, 0);
            int from_conn = Map.Zone_Connection_Index(from, 3, 0);
            if (to_conn == from_conn && to_conn != -1) {
                add_bridge_dist = false;
            }
        }
        if (add_bridge_dist) {
            if (to_bridge) {
                Cell bridge_end = unknown_geometry_k;
                if (node_count >= 4) {
                    bridge_end = Map.Find_Bridge_End_Cell_For_Subzone(to, 0, HierSubzonePath[0][node_count - 2]);
                }
                if (node_count < 4 || bridge_end == unknown_geometry_k) {
                    bridge_end = Map.Find_Bridge_End_Cell_For_Subzone(to, 0, HierSubzonePath[0][node_count - 1]);
                }
                if (bridge_end == unknown_geometry_k) {
                    bridge_end = hs_to;
                }
                if (bridge_end != unknown_geometry_k) {
                    distance += max(abs(to.X - bridge_end.X), abs(to.Y - bridge_end.Y));
                }
            }
            if (from_bridge) {
                Cell bridge_end = unknown_geometry_k;
                if (node_count >= 4) {
                    bridge_end = Map.Find_Bridge_End_Cell_For_Subzone(from, 0, HierSubzonePath[0][1]);
                }
                if (node_count < 4 || bridge_end == unknown_geometry_k) {
                    bridge_end = Map.Find_Bridge_End_Cell_For_Subzone(from, 0, HierSubzonePath[0][0]);
                }
                if (bridge_end == unknown_geometry_k) {
                    bridge_end = hs_from;
                }
                if (bridge_end != unknown_geometry_k) {
                    distance += max(abs(from.X - bridge_end.X), abs(from.Y - bridge_end.Y));
                }
            }
            if (distance > radius) {
                return distance;
            }
        }
        return radius;
    }
    return 0x7FFFFFFF;
}


Cell __fastcall AStarClass::Next_Cell(Cell const &cell, int facing)
{
    if (facing == 8) {
        int tube = Map[cell].unknown_116;
        if (tube != -1) {
            return Cell(unknown_TubeClass_list_008661c8[tube]->ExitCell);
        }
        return Cell(0, 0);
    }
    return cell + AdjacentCell[facing];
}


extern "C" void __cdecl release_42ae60(void*);
class Cleanup_42ae60 { unsigned char pad0[4]; unsigned int* items; int count; unsigned char pad12[1]; unsigned char owns; unsigned char pad14[2]; int capacity; public: void get(); };
void Cleanup_42ae60::get() { unsigned int* pointer=items; capacity=0; if(pointer && owns) { release_42ae60(pointer); items=0; } owns=0; count=0; }


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<unsigned int>::Add (0x0042B0F0): emitted by this unit's use of its list.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<unsigned int>::Clear (0x0042AE00; DynamicVectorClass<unsigned int> vtable 0x0079c4dc slot 3).
// DynamicVectorClass<unsigned int>::ID_pointer (0x0042AE30; DynamicVectorClass<unsigned int> vtable 0x0079c4dc slot 5).
// VectorClass<unsigned int>::Fetch (0x0042AE50; VectorClass<unsigned int> vtable 0x0079c4fc slot 6).
// VectorClass<unsigned short>::Fetch (0x0042AEB0; VectorClass<unsigned short> vtable 0x0079c534 slot 6).
// VectorClass<unsigned int>::operator equal (0x0042AEC0; VectorClass<unsigned int> vtable 0x0079c4fc slot 1).
// VectorClass<unsigned int>::Resize (0x0042AF00; VectorClass<unsigned int> vtable 0x0079c4fc slot 2).
// VectorClass<unsigned int>::Clear (0x0042AFB0; VectorClass<unsigned int> vtable 0x0079c4fc slot 3).
// VectorClass<unsigned int>::ID_object (0x0042AFE0; VectorClass<unsigned int> vtable 0x0079c4fc slot 4).
// VectorClass<unsigned int>::ID_pointer (0x0042B020; VectorClass<unsigned int> vtable 0x0079c4fc slot 5).
// DynamicVectorClass<unsigned int>::Resize (0x0042B040; DynamicVectorClass<unsigned int> vtable 0x0079c4dc slot 2).
// DynamicVectorClass<unsigned int>::ID_object (0x0042B140; DynamicVectorClass<unsigned int> vtable 0x0079c4dc slot 4).
// VectorClass<unsigned short>::operator equal (0x0042B170; VectorClass<unsigned short> vtable 0x0079c534 slot 1).
// VectorClass<unsigned short>::Resize (0x0042B1B0; VectorClass<unsigned short> vtable 0x0079c534 slot 2).
// VectorClass<unsigned short>::Clear (0x0042B260; VectorClass<unsigned short> vtable 0x0079c534 slot 3).
// VectorClass<unsigned short>::ID_object (0x0042B290; VectorClass<unsigned short> vtable 0x0079c534 slot 4).
// VectorClass<unsigned short>::ID_pointer (0x0042B2D0; VectorClass<unsigned short> vtable 0x0079c534 slot 5).
// DynamicVectorClass<unsigned short>::Resize (vtable 0x0079C554 slot 2, named by its RTTI) (0x0042B2F0).
// DynamicVectorClass<unsigned short>::ID(unsigned short const &) (vtable 0x0079C554 slot 4) (0x0042B3A0).


extern "C" unsigned char vtable_42b3d0[]; extern "C" void __cdecl release_buffer_42b3d0(void*); extern "C" void __cdecl release_object_42b3d0(void*);
class Leaf_42b3d0 { public: void* get(unsigned int flags); };
void* Leaf_42b3d0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_42b3d0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_42b3d0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_42b3d0(this); return this; }


extern "C" unsigned char vtable_42b420[]; extern "C" void __cdecl release_buffer_42b420(void*); extern "C" void __cdecl release_object_42b420(void*);
class Leaf_42b420 { public: void* get(unsigned int flags); };
void* Leaf_42b420::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_42b420; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_42b420((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_42b420(this); return this; }


extern "C" unsigned char vtable_42b470[]; extern "C" void __cdecl release_buffer_42b470(void*); extern "C" void __cdecl release_object_42b470(void*);
class Leaf_42b470 { public: void* get(unsigned int flags); };
void* Leaf_42b470::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_42b470; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_42b470((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_42b470(this); return this; }


extern "C" unsigned char vtable_42b4c0[]; extern "C" void __cdecl release_buffer_42b4c0(void*); extern "C" void __cdecl release_object_42b4c0(void*);
class Leaf_42b4c0 { public: void* get(unsigned int flags); };
void* Leaf_42b4c0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_42b4c0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_42b4c0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_42b4c0(this); return this; }


extern "C" unsigned char table_42b510[]; extern "C" void* __cdecl storage_42b510(unsigned int);
class Storage_42b510 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_42b510::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_42b510; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_42b510(requested << 2); owns=1; } return this; }


// UnknownPathEstimatorBlock::Heapify (0x0042B560)
// The out-of-line copy of the priority queue's sift-down (shared/path_estimator.hpp) that the first
// Extract_Min of AStarClass::Find_Path_Hierarchical (0x00429B60) calls; no source of its own.
