
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y4), appended to the walk unit: WalkLocomotionClass::Movement_AI
// (bool), unmatched in ra2-decomp. TibSun's text (TibSun archive code/walk.cpp; SPDX
// GPL-3.0-or-later, Copyright 2025 Electronic Arts Inc. and OpenTS contributors, EA's GPL section
// 7 terms; THIRD_PARTY.md); native changes are marked NATIVE. Behaviour code for the native build
// only; gamemd.exe's version is not compared yet.
// ---------------------------------------------------------------------------------------------
void WalkLocomotionClass::Movement_AI(bool first_pass)
{
	if (HeadToCoord == COORD_NONE) {
		if (DestinationCoord != COORD_NONE) {
			if (LinkedTo->Path[0] == FACING_NONE) {
				if (LinkedTo->PathDelay != 0) {
					LinkedTo->Stop_Movement_Animation();
					return;
				}

				LinkedTo->PathDelay = Rule->PathDelay * TICKS_PER_MINUTE;

				if (!LinkedTo->Basic_Path(DestinationCoord.As_Cell(), 0, 0)) {
					IsReallyMoving = false;

					if (LinkedTo->Is_In_Same_Zone(DestinationCoord)) {
						LinkedTo->On_Movement_Blocked();

						if (Distance(LinkedTo->Center_Coord(), DestinationCoord) < Rule->CloseEnoughDistance && !LinkedTo->IsTethered) {
							LinkedTo->Assign_Destination(NULL);
						} else {
							if (LinkedTo->TryTryAgain) {
								LinkedTo->TryTryAgain--;
							} else {
								if (LinkedTo->IsNewNavCom) {
									VocClass::PlayGlobal(Rule->ScoldSound, 1.0f, 0x2000);	// NATIVE (lane Y11b): YR's sound call for TibSun's Sound_Effect
								}
								LinkedTo->IsNewNavCom = false;

								Cell dest = DestinationCoord.As_Cell();

								if (LinkedTo->IsInPlayfield /* NATIVE: YR's name of TibSun's IsLocked */) {
									Cell nav_dest = LinkedTo->Destination_Coord().As_Cell();
									if (!Map.Is_Same_Cell_Zone(nav_dest, dest, LinkedTo->TClass->MZone, LinkedTo->Is_Moving_Onto_Bridge(), Map[dest].IsUnderBridge, LinkedTo->Is_Allowed_To_Leave_Map())) {
										LinkedTo->Assign_Destination(NULL);
									}
								}

								if (LinkedTo->TarCom != NULL) {
									Cell tar_dest = LinkedTo->TarCom->Destination_Coord().As_Cell();
									if (LinkedTo->IsInPlayfield /* NATIVE: YR's name of TibSun's IsLocked */) {
										if (LinkedTo->TarCom != NULL) {
											Cell nav_dest = LinkedTo->Destination_Coord().As_Cell();
											if (!Map.Is_Same_Cell_Zone(nav_dest, tar_dest, LinkedTo->TClass->MZone, LinkedTo->Is_Moving_Onto_Bridge(), Map[tar_dest].IsUnderBridge, LinkedTo->Is_Allowed_To_Leave_Map())) {
												LinkedTo->Assign_Target(NULL);
											}
										}
									}
								}
							}
						}

						LinkedTo->Set_Speed(0.0);
						Stop_Moving();
						return;
					}

					LinkedTo->Assign_Destination(NULL);
					return;
				}

				LinkedTo->TryTryAgain = FootClass::PATH_RETRY;
				if (LinkedTo->JumpJet_To_Walk()) {
					return;
				}
			}

			FacingType dir = (FacingType)LinkedTo->Path[0];
			if (dir == FACING_COUNT) {
				int tube = Map[LinkedTo->Get_Coord()].Tube;
				if (tube >= 0 && tube < Tubes.Count()) {
					LinkedTo->Mark(MARK_UP);
					LinkedTo->Clear_Occupy_Bit(LinkedTo->Get_Coord());

					TubeClass * tube_ptr = Tubes[tube];
					Cell cell = tube_ptr->Exit;
					HeadToCoord = Coord(cell, 0);

					memcpy(&LinkedTo->Path[0], &LinkedTo->Path[1], sizeof(LinkedTo->Path[0]) * (ARRAY_SIZE(LinkedTo->Path) - 1));
					LinkedTo->Path[ARRAY_SIZE(LinkedTo->Path) - 1] = FACING_NONE;

					LinkedTo->CurrentTube = tube;
					LinkedTo->CurrentTubeDir = FACING_FIRST;

					cell = tube_ptr->Enter;
					Cell visual_cell = Adjacent_Cell(cell, tube_ptr->Dirs[0]);
					Coord enter_coord(cell);
					Coord visual_coord = Map[visual_cell].Cell_Coord();
					LinkedTo->LastTubeCoord = LinkedTo->Get_Coord() + (visual_coord - enter_coord);

					int start_h = Map.Get_Height_GL(LinkedTo->Get_Coord());
					cell = tube_ptr->Exit;
					LinkedTo->LastTubeCoord.Z = start_h + (Map.Get_Height_GL(Coord(cell, 0)) - start_h) / tube_ptr->Count;
					return;
				}

				LinkedTo->Path[0] = FACING_NONE;
				HeadToCoord = COORD_NONE;
				Stop_Moving();
				LinkedTo->Assign_Destination(NULL);
				return;
			}

			Coord coord = Adjacent_Cell(LinkedTo->Get_Coord(), dir);
			Cell cell = coord;

			if (LinkedTo->IsOnBridge ^ Map[Adjacent_Cell(LinkedTo->Get_Coord(), dir)].IsUnderBridge) {
				LinkedTo->IsPlanningToLook = true;
			}

			MoveType move = LinkedTo->Can_Enter_Cell(&Map[cell], dir, LinkedTo->Get_Cell_Height(), NULL, true);
			if (move != MOVE_OK) {
				IsReallyMoving = false;
				LinkedTo->Stop_Movement_Animation();

				if (move == MOVE_TEMP) {
					CellClass & cellptr = Map[cell];
					if (first_pass) {
						LinkedTo->Path[0] = FACING_NONE;
						HeadToCoord = COORD_NONE;
						LinkedTo->PathDelay = 0;
						Movement_AI(false);
					} else {
						int dist = LinkedTo->Center_Coord().Distance_To(DestinationCoord);
						if (dist < Rule->CloseEnoughDistance &&
							!LinkedTo->In_Radio_Contact() &&
							abs(DestinationCoord.Z - LinkedTo->Get_Coord().Z) < 2 * LEVEL_LEPTON_H &&
							Map[LinkedTo->Get_Coord()].Land_Type() != LAND_TUNNEL) {

							HeadToCoord = COORD_NONE;
							Stop_Moving();
							LinkedTo->Set_Speed(0.0);
							LinkedTo->Assign_Destination(NULL);
							LinkedTo->Path[0] = FACING_NONE;
						} else {
							bool isbridge = cellptr.IsUnderBridge && abs(LinkedTo->Get_Coord().Z / LEVEL_LEPTON_H - cellptr.Height) > 2;
							cellptr.Incoming(COORD_NONE, true, true, isbridge);
						}
					}
					return;
				}

				if (move == MOVE_MOVING_BLOCK) {
					if (!LinkedTo->IsToPathAroundBlockage) {
						LinkedTo->IsToPathAroundBlockage = true;
						LinkedTo->BlockagePathDelay = Rule->BlockagePathDelay;
					}

					if (LinkedTo->PathDelay != 0) {
						return;
					}

					bool blocked = LinkedTo->IsToPathAroundBlockage && LinkedTo->BlockagePathDelay == 0;
					bool found = LinkedTo->Basic_Path(DestinationCoord.As_Cell(), 0, blocked ? 2 : 1);
					LinkedTo->PathDelay = Rule->PathDelay * TICKS_PER_MINUTE;

					if (!found) {
						if (!LinkedTo->Is_In_Same_Zone(DestinationCoord)) {
							LinkedTo->Assign_Destination(NULL);
						} else {
							LinkedTo->On_Movement_Blocked();
						}
					} else {
						LinkedTo->JumpJet_To_Walk();
					}
					return;
				}

				if (move == MOVE_CLOSED_GATE) {
					Map.Try_Open_Gate(LinkedTo, cell);
					LinkedTo->TryTryAgain = FootClass::PATH_RETRY;
					return;
				}

				if (move == MOVE_DESTROYABLE || move == MOVE_FRIENDLY_DESTROYABLE) {
					ObjectClass * object = Map[cell].Cell_Object();
					if (first_pass) {
						LinkedTo->Path[0] = FACING_NONE;
						HeadToCoord = COORD_NONE;
						LinkedTo->PathDelay = 0;
						Movement_AI(false);
						return;
					}

					if (object != NULL) {
						if (!LinkedTo->House->Is_Ally(object)) {
							LinkedTo->Override_Mission(MISSION_ATTACK, object, NULL);
						}
					} else {
						if (Map[cell].Overlay != OVERLAY_NONE && OverlayTypes[Map[cell].Overlay]->IsWall) {
							LinkedTo->Override_Mission(MISSION_ATTACK, &Map[cell], NULL);
						}
					}
				}

				if (move != MOVE_CLOAK) {
					if (move != MOVE_NO) {
						LinkedTo->Path[0] = FACING_NONE;
						LinkedTo->Set_Speed(0.0);
						Stop_Moving();
						LinkedTo->IsNewNavCom = false;
						LinkedTo->IsNewNavCom = false;
						return;
					}
				} else {
					Map[cell].Shimmer();
				}

				if (first_pass) {
					LinkedTo->Path[0] = FACING_NONE;
					HeadToCoord = COORD_NONE;
					LinkedTo->PathDelay = 0;
					Movement_AI(false);
				}
				return;
			}

			if (Mark_Head_To(coord)) {
				IsReallyMoving = true;
				if (LinkedTo->IsActive) {
					DirStruct face = (DirStruct &)::Direction(LinkedTo->Center_Coord(), HeadToCoord);
					Do_Turn(face);
					LinkedTo->Set_Speed(1.0);
					LinkedTo->IsNewNavCom = false;
				}
			} else {
				LinkedTo->Set_Speed(0.0);
				LinkedTo->IsNewNavCom = false;
			}
			return;
		} else {
			if (LinkedTo->Speed > 0.0) {
				LinkedTo->Set_Speed(0.0);
				LinkedTo->IsNewNavCom = false;
				return;
			}
			LinkedTo->IsNewNavCom = false;
			return;
		}
	} else {

		IsReallyMoving = true;

		if (Distance(Point2D(LinkedTo->Center_Coord()), Point2D(HeadToCoord)) < 17) {
			LinkedTo->Mark(MARK_UP);

			if (LinkedTo->Path[0] == FACING_NONE) {
				LinkedTo->Path[1] = FACING_NONE;
			}

			memcpy(&LinkedTo->Path[0], &LinkedTo->Path[1], sizeof(LinkedTo->Path[0]) * (ARRAY_SIZE(LinkedTo->Path) - 1));
			LinkedTo->Path[ARRAY_SIZE(LinkedTo->Path) - 1] = FACING_NONE;

			LinkedTo->Set_Coord(HeadToCoord);
			LinkedTo->LastPathingCell = HeadToCoord.As_Cell();
			LinkedTo->Set_Height_AGL(0);
			LinkedTo->IsToPathAroundBlockage = false;

			Mark_Head_To(COORD_NONE);

			if (LinkedTo->Path[0] == FACING_NONE) {
				LinkedTo->Set_Speed(0.0);
			}

			LinkedTo->Per_Cell_Process(PCP_END);

			if (LinkedTo && LinkedTo->IsActive && !LinkedTo->IsInLimbo && !LinkedTo->IsFalling) {
				if (DestinationCoord == COORD_NONE) {
					LinkedTo->Assign_Destination(NULL);
					LinkedTo->Set_Speed(0.0);
					HeadToCoord = COORD_NONE;
					Stop_Moving();
					IsReallyMoving = false;
				} else {
					if (LinkedTo->Destination_Coord().As_Cell() == DestinationCoord.As_Cell()) {
						if (abs(LinkedTo->Destination_Coord().Z - DestinationCoord.Z) < 2 * LEVEL_LEPTON_H) {
							LinkedTo->Assign_Destination(NULL);
							LinkedTo->Set_Speed(0.0);
							HeadToCoord = COORD_NONE;
							Stop_Moving();
							IsReallyMoving = false;
						}
					}
				}

				LinkedTo->Mark(MARK_DOWN);
				LinkedTo->IsNewNavCom = false;
			}
			return;
		}

		if (LinkedTo->Is_Immobilized()) {
			IsReallyMoving = false;
			LinkedTo->IsNewNavCom = false;
			return;
		}

		LinkedTo->Set_Speed(1.0);
		int speed = LinkedTo->Current_Speed();
		LinkedTo->IsToPathAroundBlockage = false;

		DirStruct face = (DirStruct &)::Direction(LinkedTo->Center_Coord(), HeadToCoord);
		Do_Turn(face);

		Cell old_cell = LinkedTo->Get_Cell();
		Coord new_coord = Move_Coord(LinkedTo->Get_Coord(), face, speed);
		Cell new_cell = new_coord.As_Cell();

		if (old_cell != new_cell) {
			LinkedTo->Mark(MARK_UP);
			LinkedTo->Set_Coord(new_coord);

			CellClass * old_cellptr = &Map[old_cell];
			CellClass * new_cellptr = &Map[new_cell];

			if (new_cellptr->Height == old_cellptr->Height - BRIDGE_CELL_HEIGHT) {
				if (new_cellptr->IsUnderBridge) {
					LinkedTo->IsOnBridge = true;
				}
			}
			if (!new_cellptr->IsUnderBridge && old_cellptr->IsUnderBridge) {
				LinkedTo->IsOnBridge = false;
			}

			LinkedTo->Set_Height_AGL(0);
			LinkedTo->Mark(MARK_DOWN);
			Map[LinkedTo->Get_Coord()].Trigger_Veins();
			LinkedTo->IsNewNavCom = false;
		} else {
			bool was_down = LinkedTo->IsDown;
			LinkedTo->IsDown = false;
			LinkedTo->Set_Coord(new_coord);
			LinkedTo->Set_Height_AGL(0);
			LinkedTo->IsDown = was_down;
			LinkedTo->IsNewNavCom = false;
		}
	}
}
