
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y4), appended to the astar unit: AStarClass::Find_Path_Regular,
// unmatched in ra2-decomp. TibSun's text (TibSun archive code/astar.cpp; SPDX GPL-3.0-or-later,
// Copyright 2025 Electronic Arts Inc. and OpenTS contributors, EA's GPL section 7 terms;
// THIRD_PARTY.md); native changes are marked NATIVE. Behaviour code for the native build only;
// gamemd.exe's version is not compared yet.
// ---------------------------------------------------------------------------------------------
PathStruct * AStarClass::Find_Path_Regular(Cell const & from, Cell const & to, FootClass *foot, FacingType *moves, int max_loops, bool with_hs)
{
	FacingType face;

	static float _facing_costs[FACING_COUNT] = {
		0.001f, /// FACING_N
		0.005f, /// FACING_NE
		0.002f, /// FACING_E
		0.006f, /// FACING_SE
		0.003f, /// FACING_S
		0.007f, /// FACING_SW
		0.004f, /// FACING_W
		0.008f, /// FACING_NW
	};

	int tries = 0;

	CellClass ** to_pptr = &Map.Array[MAP_CELL_W * to.Y + to.X];
	CellClass ** from_pptr = &Map.Array[MAP_CELL_W * from.Y + from.X];

	CellClass * to_ptr = *to_pptr;
	CellClass * from_ptr = *from_pptr;

	if (!from_ptr || !to_ptr) {
		return(NULL);
	}

	DestCellHeight = (foot->RTTI == RTTI_AIRCRAFT || !to_ptr->IsUnderBridge) ? to_ptr->Height : to_ptr->Height + BRIDGE_CELL_HEIGHT;

	if (foot->RTTI == RTTI_AIRCRAFT || !foot->IsOnBridge) {
		CurrentCellHeight = from_ptr->Height;
	} else {
		CurrentCellHeight = from_ptr->Height + BRIDGE_CELL_HEIGHT;
	}

	if (foot->TClass->IsTrain) {
		if (from_ptr->IsUnderBridge) {
			if (abs(foot->PositionCoord.Z / LEVEL_LEPTON_H - CurrentCellHeight) > 2) {
				CurrentCellHeight += BRIDGE_CELL_HEIGHT;
			}
		}
	}

	ObjectSpeed = foot->TClass->Speed;
	HierNodeIndex = 0;
	HierLastNodeCell = from;
	int * fine_final_ids = HierOnPath[0];
	RegularOpenNode * working_node = Create_Node(0, from_pptr, to, 0.0);

	if (from == to && CurrentCellHeight == DestCellHeight) {
		return(NULL);
	}

	if (Avoidance != AVOIDANCE_NONE) {
		Apply_Path_Collision_Avoidance(foot);
	}
	int start_index = from_ptr->CellID.X + MapCellStride * from_ptr->CellID.Y;
	if (CurrentCellHeight > from_ptr->Height) {
		RegularBridgeVisited[start_index] = UniqueID;
		RegularBridgeMovementCosts[start_index] = 0;
	} else {
		RegularVisited[start_index] = UniqueID;
		RegularMovementCosts[start_index] = 0;
	}

	static const int _offsets[FACING_COUNT] = {
		-MAP_CELL_W,
		-MAP_CELL_W + 1,
		1,
		MAP_CELL_W + 1,
		MAP_CELL_W,
		MAP_CELL_W - 1,
		-1,
		-MAP_CELL_W - 1
	};

	bool is_train = false;
	if (foot->TClass->IsTrain) {
		is_train = true;
		for (face = FACING_FIRST; face <= FACING_COUNT; face++) {
			int facing_diff = foot->PrimaryFacing.Current().As_Dir8() - face;
			int abs_diff = abs(facing_diff);
			if (abs_diff > FACING_E && abs_diff < FACING_W && face != FACING_COUNT) {
				CellClass * neighbor = from_pptr[_offsets[face]];
				int neighbor_index = neighbor->CellID.X + MapCellStride * neighbor->CellID.Y;
				if (CurrentCellHeight > neighbor->Height + 1) {
					RegularBridgeVisited[neighbor_index] = UniqueID;
					RegularBridgeMovementCosts[neighbor_index] = 0;
				} else {
					RegularVisited[neighbor_index] = UniqueID;
					RegularMovementCosts[neighbor_index] = 0;
				}
			}
		}
	}

	bool is_passive = foot->RTTI == RTTI_UNIT && ((UnitClass *)foot)->Class->Passive;	// NATIVE: YR's name of IsPassive

	if (max_loops < 0) {
		max_loops = (USHRT_MAX - 8);
	}

	while (working_node && tries < max_loops) {
		CellClass ** working_from = working_node->Node->CellSlot;
		if (working_from == to_pptr && working_node->Node->CellHeight == DestCellHeight) {
			break;
		}
		Cell & from_id = working_from[0]->CellID;
		RegularOpenNode * temp_node = NULL;
		int from_index = from_id.X + MapCellStride * from_id.Y;

		for (face = FACING_FIRST; face <= FACING_COUNT; face++) {

			CellClass ** neighbor_slot;
			if (face == TUNNEL) {
				int tube = (*working_from)->Tube;
				if (tube != -1) {
					Cell exit_cell = Tubes[tube]->Exit;
					neighbor_slot = &Map.Array[MAP_CELL_W * exit_cell.Y + exit_cell.X];
				} else {
					static CellClass * NoneCell = NULL;
					neighbor_slot = &NoneCell;
				}
			} else {
				neighbor_slot = &working_from[_offsets[face]];
			}

			CellClass * neighbor_cell = *neighbor_slot;
			CellClass ** working_to = neighbor_slot;

			if (neighbor_cell == NULL) {
				continue;
			}

			Cell & neighbor_id = neighbor_cell->CellID;
			int node_index;
			if (face != TUNNEL) {
				node_index = from_index + AStarFacingToOffset[face];
			} else {
				node_index = neighbor_cell->CellID.X + MapCellStride * neighbor_cell->CellID.Y;
			}

			bool base_level = !neighbor_cell->IsUnderBridge || abs(CurrentCellHeight - neighbor_cell->Height) <= 1;

			int zone_index = Map.Get_Cell_Zone_Index(neighbor_id);
			CellSubzoneStruct * subzones = Map.CellSubzones;

			short subzone_id = subzones[zone_index].SubzoneID[SUBZONE_FINE];
			if (fine_final_ids[subzone_id] != UniqueID && base_level && !neighbor_cell->AdjacentObjectCount && with_hs) {
				continue;
			}

			if (base_level && Is_Visited(0, true, node_index) && (working_node->MovementCost + 1.009) > RegularMovementCosts[node_index]) {
				continue;
			}

			if (!base_level && Is_Visited(0, false, node_index) && (working_node->MovementCost + 1.009) > RegularBridgeMovementCosts[node_index]) {
				continue;
			}
			MoveType move = foot->Can_Enter_Cell(neighbor_cell, face, CurrentCellHeight, *working_from, UseLocomotorEnterCheck);
			if (is_train && move < MOVE_NO) {
				move = MOVE_OK;
			}

			float movement_cost;
			if (face != TUNNEL) {
				movement_cost = Get_Movement_Cost(working_from, working_to, base_level == 0, move, foot) * MovementCostMultiplier + _facing_costs[face];
			} else {
				CellClass & from_cell = *working_from[0];
				CellClass & to_cell = *neighbor_cell;
				int dx = ((int)from_cell.CellID.X - (int)to_cell.CellID.X);
				int dy = ((int)from_cell.CellID.Y - (int)to_cell.CellID.Y);
				movement_cost = max(abs(dy), abs(dx));
			}

			if (move < MOVE_NO) {
				if (Is_Visited(0, base_level, node_index)) {
					continue;
				}

				RegularOpenNode * new_node = Create_Node(working_node, working_to, to, movement_cost);
				if (temp_node == NULL) {
					temp_node = new_node;
				} else {
					if (new_node->Score < temp_node->Score) {
						RegularQueue->Insert(*temp_node);
						temp_node = new_node;
					} else {
						RegularQueue->Insert(*new_node);
					}
				}

				if (base_level) {
					RegularVisited[node_index] = UniqueID;
					RegularMovementCosts[node_index] = new_node->MovementCost;
				} else {
					RegularBridgeVisited[node_index] = UniqueID;
					RegularBridgeMovementCosts[node_index] = new_node->MovementCost;
				}
				if (subzone_id == HierSubzonePath[SUBZONE_FINE][HierNodeIndex + 1]) {
					HierNodeIndex++;
					HierLastNodeCell = neighbor_cell->CellID;
				}
			} else if (working_to == to_pptr && !is_passive) {
				if (abs(CurrentCellHeight - DestCellHeight) <= 1) {
					goto breakout;
				}
			}
		}

		if (temp_node) {
			working_node = RegularQueue->Replace_Root(*temp_node);
		} else {
			working_node = RegularQueue->Extract_Min();
		}

		if (working_node) {
			CurrentCellHeight = working_node->Node->CellHeight;
		}
		tries++;
	}

breakout:

	if (tries == 10000 || working_node == NULL || tries == max_loops || working_node->PathLength < 2) {
		if (Avoidance != AVOIDANCE_NONE) {
			Apply_Path_Collision_Avoidance(foot);
		}
		return(NULL);
	}

	PathStruct * final_path = Build_Final_Path(working_node, moves);
	Cut_Corners(final_path, foot);

	/*
	**	Optimize the move list but only necessary if
	**	diagonal moves are allowed.
	*/
	Optimize_Moves(final_path, foot);

	if (Avoidance != AVOIDANCE_NONE) {
		Apply_Path_Collision_Avoidance(foot);
	}
	return(final_path);
}
