// NON-EXACT DRAFT 0x427370 AStarClass::Find_Path_Regular (cell search) (2506 B)
// entry 0x427370; exact-match score 78.55 (lane best), this text 78.55; plan "draft, verify"
// origin lane: Scout965 (build/class-work/Scout965/keep/a0.cpp, 78.55); native adaptation Scout1103
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1103 (native/drafts.json, native M10). Not original bytes.
// Its own unit: the population macros the lane scored it under come first.
#define RA2_PATH_MOVEMENT_COST
#define RA2_PATH_OPTIMIZE_MOVES
// AStarClass::Find_Path_Regular() (0x00427370): the cell search (Tiberian Sun's
// Find_Path_Regular): expands the open nodes cell by cell, eight neighbours and the tube exit, kept
// inside the subzone corridor the hierarchical search marked in +0x40 when with_hs, and builds,
// straightens and optimizes the path to the destination.
// @ref _?NoneCell@?DI@??Find_Path_Regular@AStarClass@@QAEPAUUnknownPathType@@ABVCell@@0PAVFootClass@@PAHH_N@Z@4PAVCellClass@@A=0084e808
// @ref _?_facing_costs@?1??Find_Path_Regular@AStarClass@@QAEPAUUnknownPathType@@ABVCell@@0PAVFootClass@@PAHH_N@Z@4PAMA=007cf0dc
// @ref _?_offsets@?BJ@??Find_Path_Regular@AStarClass@@QAEPAUUnknownPathType@@ABVCell@@0PAVFootClass@@PAHH_N@Z@4QBHB=0079c484
// @ref __real@8@3fff8126e978d4fdf000=0079c4d0
#include "shared/path_estimator.hpp"
#include "shared/foot.hpp"
#include "shared/cell.hpp"
#include "shared/map_view.hpp"
#include "shared/tube.hpp"
#include "shared/facing.hpp"
#include "shared/crt_memory.hpp"

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
    if ((uintptr_t)&node > (uintptr_t)block->unknown_0c) {
        block->unknown_0c = (intptr_t)&node;
    }
    if ((uintptr_t)&node < (uintptr_t)block->unknown_10) {
        block->unknown_10 = (intptr_t)&node;
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
            int move = foot->unknown_slot101(neighbor_cell, face, CurrentCellHeight, *working_from, UseLocomotorEnterCheck);
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
                    if (new_node->unknown_08 < temp_node->unknown_08) {
                        Native_Regular_Insert(RegularQueue, *temp_node);
                        temp_node = new_node;
                    } else {
                        Native_Regular_Insert(RegularQueue, *new_node);
                    }
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
