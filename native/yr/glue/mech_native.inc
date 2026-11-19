
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y15), appended to the mech unit: MechLocomotionClass::Movement_AI (gamemd.exe 0x005B01C0), unmatched in
// ra2-decomp. Red Alert 2's text of the same routine (mischa85/RedAlert2 src/mech.cpp, game.exe (its Movement_AI); SPDX
// GPL-3.0-or-later, Copyright 2026 OpenTS contributors, LICENSE.md's terms; THIRD_PARTY.md), with the
// YR tree's names where they differ (marked NATIVE). Yuri's Revenge's routine is not compared with it
// instruction by instruction. Behaviour code for the native build only.
// ---------------------------------------------------------------------------------------------
void MechLocomotionClass::Movement_AI(bool continue_moving) // RA2: defined here, between Force_Immediate_Destination and Mark_Head_To
{
	if (LinkedTo->PrimaryFacing.Is_Rotating()) {
		return;
	}

	if (HeadToCoord == COORD_NONE) {
		if (DestinationCoord != COORD_NONE) {

			/*
			 * A destination exists. If there is no current path then try to build
			 * one. If a path already exists, fall through to dispatch the next leg.
			 */
			if (LinkedTo->Path[0] == FACING_NONE) {

				if (LinkedTo->PathDelay != 0) {
					LinkedTo->Stop_Movement_Animation();
					return;
				}

				LinkedTo->PathDelay = Rule->PathDelay * TICKS_PER_MINUTE;

				if (!LinkedTo->Basic_Path(DestinationCoord.As_Cell(), 0, 0)) {

					if (LinkedTo->Is_In_Same_Zone(DestinationCoord)) {

						int dist = Distance(LinkedTo->Center_Coord(), DestinationCoord);

						if (dist < (int)Rule->CloseEnoughDistance && !LinkedTo->IsTethered) {
							LinkedTo->Assign_Destination(NULL);
						} else {

							if (LinkedTo->TryTryAgain != 0) {
								LinkedTo->TryTryAgain--;
							} else {

								if (LinkedTo->IsNewNavCom) {
									VocClass::PlayGlobal(Rule->ScoldSound, 1.0f, 0x2000);	// NATIVE (lane N5): gamemd.exe 0x005B03AA; TibSun's Sound_Effect is not in the YR tree
								}
								LinkedTo->IsNewNavCom = false;

								Cell dest_cell = DestinationCoord;
								if (LinkedTo->IsInPlayfield /* NATIVE: YR's name of IsLocked */) {
									Cell src_cell = LinkedTo->Destination_Coord().As_Cell();
									if (!Map.Is_Same_Cell_Zone(src_cell, dest_cell, LinkedTo->TClass->MZone, LinkedTo->Is_Moving_Onto_Bridge(), Map[dest_cell].IsUnderBridge, LinkedTo->Is_Allowed_To_Leave_Map())) {
										LinkedTo->Assign_Destination(NULL);
									}
								}

								if (LinkedTo->TarCom != NULL) {
									Cell tar_cell = LinkedTo->TarCom->Destination_Coord().As_Cell();
									if (LinkedTo->IsInPlayfield /* NATIVE: YR's name of IsLocked */) {
										Cell src_cell = LinkedTo->Destination_Coord().As_Cell();
										if (!Map.Is_Same_Cell_Zone(src_cell, tar_cell, LinkedTo->TClass->MZone, LinkedTo->Is_Moving_Onto_Bridge(), Map[tar_cell].IsUnderBridge, LinkedTo->Is_Allowed_To_Leave_Map())) {
											LinkedTo->Assign_Target(NULL);
										}
									}
								}
							}
						}

						LinkedTo->Set_Speed(0);
						Stop_Moving();
						IsMoving = false;
						return;
					}

					LinkedTo->Assign_Destination(NULL);
					return;
				}

				LinkedTo->TryTryAgain = FootClass::PATH_RETRY;
			}

			/*
			 * Dispatch the next leg of the path. A path entry of FACING_COUNT is
			 * a special tunnel transition.
			 */
			FacingType dir = LinkedTo->Path[0];
			if (dir == FACING_COUNT) {

				int tube = Map[LinkedTo->Get_Coord()].Tube;
				if (tube >= 0 && tube < Tubes.Count()) {
					LinkedTo->Mark(MARK_UP);
					LinkedTo->Clear_Occupy_Bit(LinkedTo->Get_Coord());

					TubeClass * tubeptr = Tubes[tube];

					Cell cell = tubeptr->Exit;
					HeadToCoord = Coord(cell, 0);

					memcpy(&LinkedTo->Path[0], &LinkedTo->Path[1], 92);
					LinkedTo->Path[23] = FACING_NONE;

					LinkedTo->CurrentTube = tube;
					LinkedTo->CurrentTubeDir = FACING_FIRST;

					cell = tubeptr->Enter;
					Cell visual_cell = Adjacent_Cell(cell, tubeptr->Dirs[0]);
					Coord enter_coord(cell);
					Coord visual_coord = Map[visual_cell].Cell_Coord();
					LinkedTo->LastTubeCoord = LinkedTo->Get_Coord() + (visual_coord - enter_coord);

					int start_h = Map.Get_Height_GL(LinkedTo->Get_Coord());
					cell = tubeptr->Exit;
					int count = tubeptr->Count;
					LinkedTo->LastTubeCoord.Z = start_h + (Map.Get_Height_GL(Coord(cell, 0)) - start_h) / count;
					return;
				}

				LinkedTo->Path[0] = FACING_NONE;
				HeadToCoord = COORD_NONE;
				Stop_Moving();
				LinkedTo->Assign_Destination(NULL);
				return;
			}

			Cell what = Adjacent_Cell(LinkedTo->Get_Coord().As_Cell(), dir);

			Coord head_coord = Coord(what, 0);
			head_coord.Z = Map.Get_Height_GL(head_coord);

			CellClass * what_cell = &Map[what];

			/*
			 * If the bridge status of the cell being moved into differs from this
			 * unit's bridge status, force a fresh look scan next time.
			 */
			if (LinkedTo->IsOnBridge ^ what_cell->IsUnderBridge) {
				LinkedTo->IsPlanningToLook = true;
			}

			MoveType can_enter = LinkedTo->Can_Enter_Cell(&Map[what], dir, LinkedTo->Get_Cell_Height(), 0, true);

			/*
			 * A crusher driving into an occupied destroyable cell that has crushable
			 * overlay just crushes through it -- treat as a normal (clear) move.
			 */
			if (((can_enter != MOVE_DESTROYABLE && can_enter != MOVE_FRIENDLY_DESTROYABLE) || !LinkedTo->TClass->IsCrusher || what_cell->Overlay != OVERLAY_FIRST) && can_enter != MOVE_OK) {

				LinkedTo->Stop_Movement_Animation();

				if (can_enter == MOVE_TEMP) {

					CellClass * cellptr = &Map[what];

					if (continue_moving) {
						LinkedTo->Path[0] = FACING_NONE;
						HeadToCoord = COORD_NONE;
						LinkedTo->PathDelay = 0;
						Movement_AI(false);
						return;
					}

					int dist = LinkedTo->Distance(DestinationCoord);

					if (dist < (int)Rule->CloseEnoughDistance && !LinkedTo->In_Radio_Contact()
						&& abs(DestinationCoord.Z - LinkedTo->Get_Coord().Z) < 2 * LEVEL_LEPTON_H
						&& Map[LinkedTo->Get_Coord()].Land_Type() != LAND_TUNNEL) {

						LinkedTo->Set_Speed(0);
						LinkedTo->Assign_Destination(NULL);
						LinkedTo->Path[0] = FACING_NONE;
						Stop_Moving();
						IsMoving = false;
					} else {

						bool bridge;
						if (cellptr->IsUnderBridge) {
							int z = LinkedTo->Get_Coord().Z;
							bridge = true;
							if (abs(z / LEVEL_LEPTON_H - cellptr->Height) <= 2) {
								bridge = false;
							}
						} else {
							bridge = false;
						}
						cellptr->Incoming(COORD_NONE, true, true, bridge);
					}
					return;
				}

				if (can_enter == MOVE_MOVING_BLOCK) {

					if (!LinkedTo->IsToPathAroundBlockage) {
						LinkedTo->IsToPathAroundBlockage = true;
						LinkedTo->BlockagePathDelay = Rule->BlockagePathDelay;
					}

					if (LinkedTo->PathDelay == 0) {

						bool blockage_expired = LinkedTo->IsToPathAroundBlockage && LinkedTo->BlockagePathDelay == 0;

						bool path_ok = LinkedTo->Basic_Path(DestinationCoord.As_Cell(), 0, blockage_expired ? 2 : 1);
						LinkedTo->PathDelay = Rule->PathDelay * TICKS_PER_MINUTE;

						if (!path_ok) {
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

					DirStruct face = (DirStruct &)::Direction(LinkedTo->Center_Coord(), head_coord);
					Do_Turn(face);
					return;
				}

				if (can_enter == MOVE_CLOSED_GATE) {
					Map.Try_Open_Gate(LinkedTo, what);
					LinkedTo->TryTryAgain = FootClass::PATH_RETRY;
					return;
				}

				if (can_enter != MOVE_CLOAK) {
					if (can_enter != MOVE_NO) {
						if (can_enter == MOVE_DESTROYABLE || can_enter == MOVE_FRIENDLY_DESTROYABLE) {

							ObjectClass * obj = Map[what].Cell_Object(Point2D(0, 0), false);

							if (continue_moving) {
								LinkedTo->Path[0] = FACING_NONE;
								HeadToCoord = COORD_NONE;
								LinkedTo->PathDelay = 0;
								Movement_AI(false);
								return;
							}

							if (obj != NULL) {
								if (!LinkedTo->House->Is_Ally(obj)) {
									LinkedTo->Override_Mission(MISSION_ATTACK, obj, 0);
								}
							} else {
								if (Map[what].Overlay != OVERLAY_NONE && OverlayTypes[Map[what].Overlay]->IsWall) {
									LinkedTo->Override_Mission(MISSION_ATTACK, &Map[what], 0);
								}
							}
						}

						LinkedTo->Path[0] = FACING_NONE;
						LinkedTo->Set_Speed(0);
						Stop_Moving();
						LinkedTo->IsNewNavCom = false;
						LinkedTo->IsNewNavCom = false;
						return;
					}
				} else {
					Map[what].Shimmer();
				}

				if (!continue_moving) return;

				LinkedTo->Path[0] = FACING_NONE;
				HeadToCoord = COORD_NONE;
				LinkedTo->PathDelay = 0;
				Movement_AI(false);
				return;
			}

			if (Mark_Head_To(head_coord)) {

				FacingType next_dir = LinkedTo->Path[0];
				if (next_dir != FACING_NONE && next_dir != FACING_COUNT) {
					Cell cell = Adjacent_Cell(LinkedTo->Get_Cell(), next_dir);
					OverlayType overlay = Map[cell].Overlay;
					if (overlay != OVERLAY_NONE) {
						OverlayTypeClass * otype = OverlayTypes[overlay];
						if ((LinkedTo->TClass->IsCrusher || LinkedTo->Has_Ability(ABILITY_CRUSHER)) && otype->HeapID == OVERLAY_SANDBAG_WALL) {
							LinkedTo->IsCrushing = true;
							if (LinkedTo->TClass->IsTiltsWhenCrushes) {
								LinkedTo->RockingForwardsPerFrame = -0.01f;
							}
						}
					}
				}

				if (LinkedTo->IsActive) {

					DirStruct face = (DirStruct &)::Direction(LinkedTo->Center_Coord(), HeadToCoord);
					Do_Turn(face);

					if (!LinkedTo->PrimaryFacing.Is_Rotating()) {
						LinkedTo->Set_Speed(1.0);
						LinkedTo->IsNewNavCom = false;
					}
				}
			} else {
				LinkedTo->Set_Speed(0);
				LinkedTo->IsNewNavCom = false;
			}
			return;

		} else {

			/*
			 * With no immediate destination and no overall destination, just stop
			 * moving (if the unit is still rolling) and bail out.
			 */
			if (LinkedTo->Speed > 0.0) {
				LinkedTo->Set_Speed(0.0);
				LinkedTo->IsNewNavCom = false;
				return;
			}
			LinkedTo->IsNewNavCom = false;
			return;
		}
	} else {

		if (Distance(Point2D(LinkedTo->Center_Coord()), Point2D(HeadToCoord)) < 16) {

			LinkedTo->Mark(MARK_UP);

			if (LinkedTo->Path[0] == FACING_NONE) {
				LinkedTo->Path[1] = FACING_NONE;
			}

			memcpy(&LinkedTo->Path[0], &LinkedTo->Path[1], 92);
			LinkedTo->Path[23] = FACING_NONE;

			LinkedTo->Set_Coord(HeadToCoord);
			LinkedTo->LastPathingCell = HeadToCoord.As_Cell();
			LinkedTo->HeightAGL = 0;

			Mark_Head_To(COORD_NONE);

			if (LinkedTo->Path[0] == FACING_NONE) {
				LinkedTo->Set_Speed(0);
			}

			LinkedTo->IsOccupyingCell = true;

			LinkedTo->IsToPathAroundBlockage = false;

			/*
			 * The walk is over when there is no destination left to reach, or when
			 * this cell is the destination cell and the two are close in height.
			 */
			if (DestinationCoord == COORD_NONE
				|| (LinkedTo->Get_Cell() == DestinationCoord.As_Cell()
					&& abs(LinkedTo->Destination_Coord().Z - DestinationCoord.Z) < 2 * LEVEL_LEPTON_H)) {

				LinkedTo->Assign_Destination(NULL);
				LinkedTo->Set_Speed(0.0);
				Stop_Moving();
				IsMoving = false;
			}

			LinkedTo->Per_Cell_Process(PCP_END);

			if (LinkedTo && LinkedTo->IsActive) {
				if (!LinkedTo->IsInLimbo && !LinkedTo->IsFalling) {
					FacingType next_move = LinkedTo->Path[0];
					if (next_move != FACING_NONE && next_move != FACING_COUNT) {
						Cell check_coord = Adjacent_Cell(LinkedTo->Get_Cell(), next_move);

						OverlayType overlay = Map[check_coord].Overlay;
						if (overlay != OVERLAY_NONE) {
							OverlayTypeClass * otype = OverlayTypes[overlay];
							if ((LinkedTo->TClass->IsCrusher || LinkedTo->Has_Ability(ABILITY_CRUSHER)) && otype->HeapID == OVERLAY_SANDBAG_WALL) {
								LinkedTo->IsCrushing = true;

								if (LinkedTo->TClass->IsTiltsWhenCrushes) {
									LinkedTo->RockingForwardsPerFrame = -0.01f;
								}
							}
						}
					}

					if (LinkedTo->IsActive && !LinkedTo->IsInLimbo) {
						LinkedTo->Mark(MARK_DOWN);
						LinkedTo->IsNewNavCom = false;
					}
				}
			}
			return;
		}

		LinkedTo->Set_Speed(1.0);

		int speed = LinkedTo->Current_Speed();

		DirType desired_facing = ::Direction(LinkedTo->Center_Coord(), HeadToCoord);
		Do_Turn((DirStruct &)desired_facing);

		if (LinkedTo->PrimaryFacing.Is_Rotating()) {
			return;
		}

		if (LinkedTo->Occupies_Cells()) {
			LinkedTo->Clear_Occupy_Bit(LinkedTo->Get_Coord());
			LinkedTo->IsOccupyingCell = false;

			LinkedTo->IsToPathAroundBlockage = false;

		}

		Cell old_cell = LinkedTo->Get_Cell();

		Coord new_coord = Move_Coord(LinkedTo->Get_Coord(), desired_facing, speed);
		Cell new_cell = new_coord;

		if (old_cell != new_cell) {

			LinkedTo->Mark(MARK_UP);
			LinkedTo->Set_Coord(new_coord);

			CellClass * old_cellptr = &Map[old_cell];
			CellClass * new_cellptr = &Map[new_cell];

			if (new_cellptr->Height == old_cellptr->Height - BRIDGE_CELL_HEIGHT && new_cellptr->IsUnderBridge) {
				LinkedTo->IsOnBridge = true;
			}
			if (!new_cellptr->IsUnderBridge && old_cellptr->IsUnderBridge) {
				LinkedTo->IsOnBridge = false;
			}

			LinkedTo->HeightAGL = 0;
			LinkedTo->Mark(MARK_DOWN);

			Map[LinkedTo->Get_Coord()].Trigger_Veins();

			LinkedTo->IsNewNavCom = false;
			return;
		} else {
			bool was_down = LinkedTo->IsDown;
			LinkedTo->IsDown = false;
			LinkedTo->Set_Coord(new_coord);
			LinkedTo->HeightAGL = 0;
			LinkedTo->IsDown = was_down;
			LinkedTo->IsNewNavCom = false;
			return;
		}
	}
}

