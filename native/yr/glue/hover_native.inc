
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFTS (lane Y12; in gamemd.exe the unnamed bodies after Shadow_Matrix 0x005142A0, Move_To 0x00514D90 and Reduce_Path_Length 0x005163B0; link-closure traps a Soviet campaign mission reached; not compared with gamemd.exe yet): functions unmatched in the YR tree, the text TibSun's
// (TibSun archive code/hover.cpp; SPDX GPL-3.0-or-later, Copyright 2025 Electronic Arts Inc. and
// OpenTS contributors, EA's GPL section 7 terms; THIRD_PARTY.md); native changes are marked
// NATIVE. Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------

boolean STDMETHODCALLTYPE HoverLocomotionClass::Process(void)
{
	if (Is_Moving() && Is_Moving1()) {
		Motion_AI();

		int height = LinkedTo->Height;
		int speed = LinkedTo->Current_Speed() * Acceleration;
		DirType direction = Direction(LinkedTo->Center_Coord(), HeadToCoord);

		if (speed == 0 && Facing.Current().Is_Complete_Turn(direction, 0)) {
			Start();
			speed = LinkedTo->Current_Speed() * Acceleration;
		}

		if (speed >= Point2D(LinkedTo->PositionCoord).Distance_To(HeadToCoord)) {
			LinkedTo->IsOccupyingCell = true;
			LinkedTo->IsToPathAroundBlockage = false;
			WasPushed = false;

			Start_Of_Move(0);

			if (LinkedTo->Path[0] == FACING_NONE && DestinationCoord == COORD_NONE) {
				HeadToCoord = COORD_NONE;
			}

			if (DestinationCoord != COORD_NONE) {
				MoveType ok = While_Moving(true);
				if (!LinkedTo->IsActive || LinkedTo->IsInLimbo || LinkedTo->IsFalling) return(MOVE_NO);
				if (ok == MOVE_OK && HeadToCoord == COORD_NONE) {
					/*
					**	Perform "per cell" activities.
					*/
					LinkedTo->Per_Cell_Process(PCP_END);
					if (LinkedTo == NULL || !LinkedTo->IsActive || LinkedTo->IsInLimbo || LinkedTo->IsFalling) return(false);
				}
				if (ok == MOVE_NO) {
					if (LinkedTo->CurrentTube >= TUBE_FIRST) return(false);
					Stop();
				} else if (ok != MOVE_OK) {
					Stop_Driver();
					speed = 0;
				}
			}
		}

		if (speed > 0) {
			Coord coord = LinkedTo->PositionCoord;

			if (LinkedTo->Occupies_Cells() && HeadToCoord != COORD_NONE) {
				LinkedTo->Clear_Occupy_Bit(LinkedTo->PositionCoord);
				LinkedTo->IsOccupyingCell = false;
				LinkedTo->IsToPathAroundBlockage = false;
			}

			coord = Move_Coord(coord, Facing.Current(), speed);
			if (coord.As_Cell() != LinkedTo->Get_Coord().As_Cell()) {
				LinkedTo->Mark(MARK_UP);
				LinkedTo->PositionCoord = coord;
				LinkedTo->Height = height;
				CellClass * cellptr = &Map[coord];
				if (!LinkedTo->IsOnBridge && cellptr->IsUnderBridge && LinkedTo->HeightAGL >= BRIDGE_LEPTON_HEIGHT) {
					LinkedTo->IsOnBridge = true;
				}
				if (LinkedTo->IsOnBridge == true && !cellptr->IsUnderBridge) {
					LinkedTo->IsOnBridge = false;
				}
				LinkedTo->Mark(MARK_DOWN);
			} else {
				bool wasdown = LinkedTo->IsDown;
				LinkedTo->IsDown = false;
				LinkedTo->PositionCoord = coord;
				LinkedTo->Height = height;
				LinkedTo->IsDown = wasdown;
			}
		}
	}

	if (Is_Moving_Now() && (Frame % 10) == 0) {
		if (!LinkedTo->IsOnBridge && LinkedTo->Get_Cell_Ptr()->Land_Type() == LAND_WATER) {
			if (Rule->Wake != NULL) {
				new AnimClass(Rule->Wake, LinkedTo->Get_Coord());
			}
		}
	}

	Gravity_AI();

	if (WasShoved) {
		if (Is_Powered()) {
			DirType dir = LinkedTo->PrimaryFacing.Current();
			LinkedTo->PrimaryFacing.Set(dir + Dir256(ShoveAccum));
			int delta = abs(ShoveAccum);
			if (delta >= 1) {
				delta = 1;
			}
			if (ShoveAccum < 0) {
				ShoveAccum += delta;
			} else {
				ShoveAccum -= delta;
			}
			if (ShoveAccum == 0) {
				WasShoved = false;
			}
		} else {
			WasShoved = false;
		}
		if (!WasShoved) {
			Map[LinkedTo->Get_Coord()].Trigger_Veins();
			if (Map[LinkedTo->Get_Coord()].Land_Type() == LAND_WATER) {
				if (LinkedTo->Get_Coord().Z < Map.Get_Height_GL(LinkedTo->Get_Coord()) + LEVEL_LEPTON_H) {
					LinkedTo->Fall_From_Height();
				}
			}
		}
	}

	return(Is_Moving());
}

MoveType HoverLocomotionClass::While_Moving(bool first_pass)
{
	assert(LinkedTo->IsActive);

	/*
	**	See if "per cell" processing is necessary.
	*/
	if (HeadToCoord != COORD_NONE) {
		LinkedTo->Clear_Occupy_Bit(HeadToCoord);
		HeadToCoord = COORD_NONE;
	}

	/*
	**	Perform quick legality checks.
	*/
	if (!Is_Moving() || DestinationCoord == COORD_NONE) {
		return(MOVE_OK);
	}

	FacingType		nextface;		// Next facing queued in path.
	nextface = LinkedTo->Path[0];

	if (nextface == FACING_COUNT) {
		TubeType tubenum = (TubeType)Map[LinkedTo->Get_Coord()].Tube;
		if (tubenum >= TUBE_FIRST && tubenum < Tubes.Count()) {
			LinkedTo->Mark(MARK_UP);
			LinkedTo->Clear_Occupy_Bit(LinkedTo->PositionCoord);
			TubeClass * tube = Tubes[tubenum];
			Cell exit = tube->Exit;
			HeadToCoord = exit.As_Coord();
			memcpy((char*)&LinkedTo->Path, (char*)&LinkedTo->Path[1], sizeof(LinkedTo->Path[0]) * (CONQUER_PATH_MAX-1));
			LinkedTo->LastPathingCell = HeadToCoord.As_Cell();
			LinkedTo->Path[CONQUER_PATH_MAX-1] = FACING_NONE;
			LinkedTo->CurrentTube = tubenum;
			LinkedTo->CurrentTubeDir = FACING_FIRST;
			LinkedTo->LastTubeCoord = Map[Adjacent_Cell((Cell)tube->Enter, tube->Dirs[0])].Cell_Coord() - Coord((Cell)tube->Enter) + LinkedTo->PositionCoord;
			int height = LinkedTo->Height;
			int current_height = Map.Get_Height_GL(LinkedTo->PositionCoord);
			int count = tube->Count;
			exit = tube->Exit;
			LinkedTo->LastTubeCoord.Z = height + (Map.Get_Height_GL(exit) - current_height) / count;
		} else {
			LinkedTo->Path[0] = FACING_NONE;
			HeadToCoord = COORD_NONE;
			Stop();
			LinkedTo->Assign_Destination(NULL);
		}
		return(MOVE_NO);
	}

	if (nextface != FACING_NONE) {
		HeadToCoord = Adjacent_Cell(LinkedTo->PositionCell, nextface).As_Coord();
		HeadToCoord.Z = Map.Get_Height_GL(HeadToCoord);
		if (LinkedTo->PositionCoord.Z >= HeadToCoord.Z + 2 * LEVEL_LEPTON_H + LEVEL_LEPTON_H) {
			HeadToCoord.Z += BRIDGE_LEPTON_HEIGHT;
		}

		/*
		**	Check for crate goodie finder here.
		*/
		if (!Map[HeadToCoord].Goodie_Check(LinkedTo) && !LinkedTo->IsInLimbo) {
			if (LinkedTo->IsActive && !LinkedTo->IsFalling) {
				LinkedTo->Path[0] = FACING_NONE;
				HeadToCoord = COORD_NONE;
				Stop();
			}
			return(MOVE_NO);
		}

		if (!LinkedTo->IsActive || LinkedTo->IsInLimbo || LinkedTo->IsFalling) return(MOVE_NO);

		if (LinkedTo->IsOnBridge != (int)Map[Adjacent_Cell(LinkedTo->PositionCoord, nextface)].IsUnderBridge) {
			LinkedTo->IsPlanningToLook = true;
		}

		Cell c = HeadToCoord;
		MoveType ok = LinkedTo->Can_Enter_Cell(&Map[c], nextface, LinkedTo->Get_Cell_Height());

		switch (ok) {
		case MOVE_CLOAK:
		case MOVE_NO:
			Map[HeadToCoord].Shimmer();
			if (first_pass) {
				Stop_Driver();
				LinkedTo->PathDelay = 0;
				LinkedTo->Path[0] = FACING_NONE;
				Start_Of_Move(0);
				ok = While_Moving(false);
			}
			if (ok != MOVE_OK) {
				HeadToCoord = COORD_NONE;
				Stop_Driver();
			}
			return(ok);

		case MOVE_CLOSED_GATE:
			HeadToCoord = COORD_NONE;
			Map.Try_Open_Gate(LinkedTo, c);
			Stop_Driver();
			return(ok);

		case MOVE_TEMP:
			if (first_pass) {
				Stop_Driver();
				LinkedTo->PathDelay = 0;
				LinkedTo->Path[0] = FACING_NONE;
				Start_Of_Move(0);
				ok = While_Moving(false);
			} else {
				if (LinkedTo->Center_Coord().Distance_To(DestinationCoord) < Rule->CloseEnoughDistance && abs(DestinationCoord.Z - LinkedTo->PositionCoord.Z) < 2 * LEVEL_LEPTON_H && Map[LinkedTo->Get_Coord()].Land_Type() != LAND_TUNNEL) {
					Stop_Moving();
					LinkedTo->Assign_Destination(NULL);
					return(MOVE_NO);
				}
				bool bridge;
				if (!Map[HeadToCoord].IsUnderBridge || abs(LinkedTo->PositionCoord.Z / LEVEL_LEPTON_H - Map[HeadToCoord].Height) <= 2) {
					bridge = false;
				} else {
					bridge = true;
				}
				Map[HeadToCoord].Incoming(COORD_NONE, true, true, bridge);
				HeadToCoord = COORD_NONE;
				Stop_Driver();
				return(ok);
			}
			break;

		case MOVE_OK:
			LinkedTo->Per_Cell_Process(PCP_END);
			if (LinkedTo == NULL || !LinkedTo->IsActive || LinkedTo->IsInLimbo || LinkedTo->IsFalling) return(MOVE_NO);
			if (LinkedTo->IsInPlayfield /* NATIVE: YR's name of IsLocked */) {
				LinkedTo->Set_Occupy_Bit(HeadToCoord);
			}
			memcpy((char *)&LinkedTo->Path, (char *)&LinkedTo->Path[1], sizeof(LinkedTo->Path[0]) * (CONQUER_PATH_MAX - 1));
			LinkedTo->Path[CONQUER_PATH_MAX - 1] = FACING_NONE;
			LinkedTo->LastPathingCell = HeadToCoord.As_Cell();
			return(ok);

		case MOVE_FRIENDLY_DESTROYABLE:
		case MOVE_DESTROYABLE:
			HeadToCoord = COORD_NONE;
			if (first_pass) {
				Stop_Driver();
				LinkedTo->PathDelay = 0;
				LinkedTo->Path[0] = FACING_NONE;
				Start_Of_Move(0);
				ok = While_Moving(false);
				return(ok);
			}
			if (Map[c].Cell_Object() != NULL) {
				if (!LinkedTo->House->Is_Ally(Map[c].Cell_Object())) {
					LinkedTo->Override_Mission(MISSION_ATTACK, Map[c].Cell_Object(), NULL);
				}
			} else {
				if (Map[c].Overlay != OVERLAY_NONE && OverlayTypes[Map[c].Overlay]->IsWall) {
					LinkedTo->Override_Mission(MISSION_ATTACK, &Map[c], NULL);
				}
			}
			return(ok);

		case MOVE_MOVING_BLOCK:
			HeadToCoord = COORD_NONE;
			Stop_Driver();
			if (!LinkedTo->IsToPathAroundBlockage) {
				LinkedTo->IsToPathAroundBlockage = true;
				LinkedTo->BlockagePathDelay = Rule->BlockagePathDelay;
			}
			if (LinkedTo->PathDelay == 0) {
				LinkedTo->Path[0] = FACING_NONE;
			}
			bool blocked = LinkedTo->IsToPathAroundBlockage && LinkedTo->BlockagePathDelay == 0;
			Start_Of_Move((blocked != 0) + 1);
			return(ok);
		}
		return(ok);
	}

	Start_Of_Move(0);
	return(MOVE_OK);
}

void HoverLocomotionClass::Start_Of_Move(int num)
{
	assert(LinkedTo->IsActive);

	FacingType		facing;				// Direction movement will commence.
	Cell			destcell;			// Cell of destination.
	Coord			dest;				// Destination coordinate.

	Reduce_Path_Length();

	facing = LinkedTo->Path[0];

	if (DestinationCoord != COORD_NONE) {
		if (LinkedTo->PositionCell == DestinationCoord.As_Cell() && abs(LinkedTo->Destination_Coord().Z - DestinationCoord.Z) < 2 * LEVEL_LEPTON_H) {
			Stop_Moving();
			LinkedTo->Assign_Destination(NULL);
			return;
		}
	}

	/*
	**	If the path is invalid at this point, then generate one. If
	**	generating a new path fails, then abort NavCom.
	*/
	if (facing == FACING_NONE) {
		if (DestinationCoord == COORD_NONE) {
			return;
		}

		if (LinkedTo->PositionCell == DestinationCoord.As_Cell() && abs(LinkedTo->Destination_Coord().Z - DestinationCoord.Z) <= 2 * LEVEL_LEPTON_H) {
			return;
		}

		/*
		**	If after a path search, there is still no valid path, then set the
		**	NavCom to null and let the script take care of assigning a new
		**	navigation target.
		*/
		if (LinkedTo->PathDelay != 0) {
			return;
		}

		if (!LinkedTo->Basic_Path(DestinationCoord.As_Cell(), 0, num)) {
			LinkedTo->PathDelay = Rule->PathDelay * TICKS_PER_MINUTE;

			if (!LinkedTo->Is_In_Same_Zone(DestinationCoord)) {
				LinkedTo->Assign_Destination(NULL);
				return;
			}

			/*
			**	If the unit is close enough to the target then just stop
			**	driving now. This prevents the fidgeting that would occur
			**	if they mindlessly kept trying to get to the exact location
			**	desired. This is quite necessary since it is typical to move
			**	several units with the same mouse click.
			*/
			if (!LinkedTo->Is_On_Priority_Mission() && LinkedTo->Distance(DestinationCoord) < Rule->CloseEnoughDistance && (LinkedTo->Mission == MISSION_MOVE || LinkedTo->Mission == MISSION_GUARD_AREA)) {
				LinkedTo->Assign_Destination(NULL);
				if (!LinkedTo->IsActive) return;
			} else {
				if (LinkedTo->TryTryAgain > 0) {
					LinkedTo->TryTryAgain--;
				} else {
					LinkedTo->Assign_Destination(NULL);
					if (!LinkedTo->IsActive) return;
					if (LinkedTo->IsNewNavCom) Sound_Effect(Rule->ScoldSound);
					LinkedTo->IsNewNavCom = false;
				}
			}

			/*
			**	Since the path was blocked, check to make sure that it was completely
			**	blocked. If so and it has a valid TarCom and it is out of range of the
			**	TarCom, then give this unit a range limit so that it might not pick
			**	a "can't reach" target again.
			*/
			if (!Is_Moving() && LinkedTo->TarCom != NULL && !LinkedTo->In_Range(LinkedTo->TarCom)) {
				LinkedTo->IsScanLimited = true;
				if (LinkedTo->Team != NULL) LinkedTo->Team->Scan_Limit();
				LinkedTo->Assign_Target(NULL);
			}

			/*
			**	Stop the movement, for now, and let the subsequent logic in later game
			**	frames resume movement as appropriate.
			*/
			if (HeadToCoord != COORD_NONE) {
				LinkedTo->Clear_Occupy_Bit(HeadToCoord);
				HeadToCoord = COORD_NONE;
			}
			Stop_Moving();
			return;
		}

		LinkedTo->PathDelay = Rule->PathDelay * TICKS_PER_MINUTE;

		/*
		**	If a basic path could be found, but the immediate move destination is
		**	blocked by a friendly temporary blockage, then cause that blockage
		**	to scatter.
		*/
		Cell cell = Adjacent_Cell(LinkedTo->Center_Coord().As_Cell(), LinkedTo->Path[0]);
		if (Map.In_Local_Radar(cell, true)) {
			MoveType ok = LinkedTo->Can_Enter_Cell(&Map[cell], LinkedTo->Path[0], LinkedTo->Get_Cell_Height());
			if (ok == MOVE_TEMP) {
				CellClass * cellptr = &Map[cell];
				ObjectClass * blockage = cellptr->Cell_Techno(Point2D(0, 0), LinkedTo->PositionCoord.Z > Map.Get_Height_GL(cell) + 3 * LEVEL_LEPTON_H);
				if (blockage && LinkedTo->House->Is_Ally(blockage)) {

					/*
					**	If the target can be told to get out of the way, only bother
					**	to do so if we aren't very close to the target and this
					**	object can just say "good enough" and stop here.
					*/
					if (LinkedTo->Center_Coord().Distance_To(DestinationCoord) < Rule->CloseEnoughDistance && !LinkedTo->In_Radio_Contact() &&
						abs(DestinationCoord.Z - LinkedTo->PositionCoord.Z) < 2 * LEVEL_LEPTON_H && Map[LinkedTo->Get_Coord()].Land_Type() != LAND_TUNNEL) {

						Stop_Moving();
						LinkedTo->Assign_Destination(NULL);
						return;
					} else {
						bool bridge = (cellptr->IsUnderBridge && abs(LinkedTo->PositionCoord.Z / LEVEL_LEPTON_H - cellptr->Height) > 2);
						cellptr->Incoming(COORD_NONE, true, true, bridge);
					}
				}
			}
		}

		LinkedTo->TryTryAgain = FootClass::PATH_RETRY;
		facing = LinkedTo->Path[0];
	}
}
