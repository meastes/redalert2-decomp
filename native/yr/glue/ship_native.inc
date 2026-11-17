// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFTS (lane Y12), appended to the ship unit: ShipLocomotionClass::Draw_Matrix and
// While_Moving and the ship locomotor's track tables. gamemd.exe's ShipLocomotionClass is a clone of
// DriveLocomotionClass (the same constructor, member layout and method order in ship.cpp and
// drive.cpp; Draw_Matrix compared instruction by instruction: identical), so this is
// glue/drive_native.inc's text (lanes Y3, Y4) under the ship's class name, generated from it; see
// there for its sources and NATIVE marks. A campaign's first Soviet mission reached the ship's
// Draw_Matrix as a link-closure trap. Behaviour code for the native build; never part of an exact
// build.
// ---------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFTS (lane Y3): functions unmatched in the YR tree, the text TibSun's
// (TibSun archive code/drive.cpp; SPDX GPL-3.0-or-later, Copyright 2025 Electronic Arts Inc. and
// OpenTS contributors, EA's GPL section 7 terms; THIRD_PARTY.md); native changes are marked
// NATIVE. Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------

Matrix3DStruct STDMETHODCALLTYPE ShipLocomotionClass::Draw_Matrix(int *key)
{
	Matrix3DStruct m;

	if (!Is_Angled()) {
		Matrix3D m1, m2;
		m1.Make_Identity();
		m2.Make_Identity();

		float val = LinkedTo->TClass->VoxelCenterY;
		float val2 = LinkedTo->TClass->VoxelCenterX;

		float fcos = fastmath::cos(LinkedTo->AngleRotatedForwards);
		float fsin = fastmath::sin(LinkedTo->AngleRotatedForwards);

		float scos = fastmath::cos(LinkedTo->AngleRotatedSideways);
		float ssin = fastmath::sin(LinkedTo->AngleRotatedSideways);

		int num = fabs(ssin) * val + fabs(fsin) * val2;

		int x_shift = (int)(val2 - (float)(int)(fcos * val2));
		int x_offset = x_shift;
		int y_shift = (int)(val - (float)(int)(scos * val));
		int y_offset = y_shift;
		if (LinkedTo->AngleRotatedForwards < 0.0f) {
			x_offset = -x_shift;
		}
		if (LinkedTo->AngleRotatedSideways > 0.0f) {
			y_offset = -y_shift;
		}

		m1.Translate_Z(num);
		m2.Translate_X(x_offset);
		m2.Translate_Y(y_offset);
		m2.Rotate_X(LinkedTo->AngleRotatedSideways);
		m2.Rotate_Y(LinkedTo->AngleRotatedForwards);

		if (key) {
			*key = -1;
		}

		Matrix3D basemtx(BASECLASS::Draw_Matrix(key));
		Matrix3D slopemtx = Get_Slope_Matrix();
		m = (Matrix3DStruct &)(((m1 * slopemtx) * basemtx) * m2);
		return(m);
	}

	if (key && *key != -1) {
		int keybase = *key << 6;
		*key = keybase;
		*key = keybase + CurrentRamp;
	}

	Matrix3D basemtx(BASECLASS::Draw_Matrix(key));
	Matrix3D slopemtx = Get_Slope_Matrix();
	m = (Matrix3DStruct &)(slopemtx * basemtx);
	return(m);
}

// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y4), appended to the drive unit: ShipLocomotionClass::While_Moving
// (bool), unmatched in ra2-decomp. TibSun's text (TibSun archive code/drive.cpp; SPDX
// GPL-3.0-or-later, Copyright 2025 Electronic Arts Inc. and OpenTS contributors, EA's GPL section
// 7 terms; THIRD_PARTY.md); native changes are marked NATIVE. Behaviour code for the native build
// only; gamemd.exe's version is not compared yet.
// ---------------------------------------------------------------------------------------------
static const double _sinking = 0.1f;		// TibSun drive.cpp's sinking deceleration
static const double _sinking_scale = 0.0015f;

bool ShipLocomotionClass::While_Moving(bool just_started)
{
	assert(LinkedTo->IsActive);

	/*
	**	Perform quick legality checks.
	*/
	if ((!IsDriving || TrackNumber == -1) && LinkedTo->Path[0] != FACING_COUNT || (IsRotating && !LinkedTo->TClass->IsTurretEquipped)) {
		SpeedAccum = 0;		// Kludge?  No speed should accumulate if movement is on hold.
		return(false);
	}
	if (LinkedTo->TClass->IsAccelerates) {
		bool not_passive = LinkedTo->RTTI != RTTI_UNIT || !((UnitClass *)LinkedTo)->Class->Passive /* NATIVE: YR's name */;
		if (TrackNumber < BACKUP_INTO_REFINERY && not_passive) {
			Coord dest_coord = DestinationCoord;
			dest_coord.Z = Map.Get_Height_GL(dest_coord) + (Map[dest_coord].IsUnderBridge ? BRIDGE_LEPTON_HEIGHT : 0);
			int distance = (LinkedTo->PositionCoord - dest_coord).Length();//LinkedTo->PositionCoord.Distance_To(dest_coord);

			bool forced_speed = false;
			double speed = LinkedTo->Speed;

			TechnoTypeClass const * tclass = LinkedTo->TClass;
			int maxspeed = LinkedTo->Get_Max_Speed();

			if (distance < tclass->SlowdownDistance) {
				forced_speed = true;
				speed = MAX(_deaccel, speed - maxspeed * tclass->DeaccelerationFactor);
			} else if (LinkedTo->IsSinking) {
				speed = MAX(_sinking, speed - maxspeed * _sinking_scale);
				forced_speed = true;
			}

			if (LinkedTo->IsCrushing) {
				TargetSpeed = MIN(TargetSpeed, 0.2);
				LinkedTo->Set_Speed(TargetSpeed);
			} else if (forced_speed) {
				LinkedTo->Set_Speed(speed);
			} else if (LinkedTo->Speed < TargetSpeed) {
				LinkedTo->Set_Speed(MIN(TargetSpeed, LinkedTo->Speed + tclass->AccelerationFactor));
			} else if (LinkedTo->Speed > TargetSpeed) {
				LinkedTo->Set_Speed(MAX(TargetSpeed, LinkedTo->Speed - maxspeed * tclass->DeaccelerationFactor));
			}

			if (LinkedTo->RTTI == RTTI_UNIT) {
				UnitClass * following = ((UnitClass *)LinkedTo)->FollowingMe;
				if (following != NULL) {
					do {
						following->Set_Speed(LinkedTo->Speed);
						following = following->FollowingMe;
					} while (following && following != following->FollowingMe);
				}
			}
		}
	} else {
		LinkedTo->Set_Speed(TargetSpeed);
	}

	/*
	**	If enough movement has accumulated so that the unit can
	**	visibly move on the map, then process accordingly.
	**	Slow the unit down if he's carrying a flag.
	*/
	int maxspeed = LinkedTo->Current_Speed();
	//if (IsFormationMove) maxspeed = FormationMaxSpeed;

	FacingType		nextface;		// Next facing queued in path.
	nextface = LinkedTo->Path[0];

	int actual;			// Working movement addition value.
	actual = SpeedAccum + (just_started ? 0 : maxspeed);

	if (nextface == FACING_COUNT && TrackNumber == -1) {
		LinkedTo->Mark(MARK_UP);
		Stop_Driver();

		TubeType tubenum = (TubeType)LinkedTo->Get_Cell_Ptr()->Tube;
		if (tubenum >= TUBE_FIRST && tubenum < Tubes.Count()) {
			TubeClass * tube = Tubes[tubenum];
			Cell c = tube->Exit;
			HeadToCoord = c.As_Coord();
			memcpy((char*)&LinkedTo->Path, (char*)&LinkedTo->Path[1], sizeof(LinkedTo->Path[0]) * (CONQUER_PATH_MAX-1));
			LinkedTo->Path[CONQUER_PATH_MAX-1] = FACING_NONE;
			LinkedTo->CurrentTube = tubenum;
			LinkedTo->CurrentTubeDir = FACING_FIRST;
			LinkedTo->LastTubeCoord = Map[Adjacent_Cell((Cell)tube->Enter, tube->Dirs[0])].Cell_Coord();
			int current_height = Map.Get_Height_GL(LinkedTo->PositionCoord);
			int count = tube->Count;
			c = tube->Exit;
			LinkedTo->LastTubeCoord.Z = current_height + (Map.Get_Height_GL(c) - current_height) / count;
			IsDriving = true;
			TrackNumber = -1;
		} else {
			LinkedTo->Path[0] = FACING_NONE;
			TrackNumber = -1;
			HeadToCoord = COORD_NONE;
			Stop_Driver();
		}
		return(false);
	}

	if (actual > ((PIXEL_LEPTON_W + PIXEL_LEPTON_H) / 2)) {
		TurnTrackType	const * track;  // Track control pointer.
		TrackType		const	* ptr;  // Pointer to coord offset values.
		int				tracknum;       // The track number being processed.
		bool				adj;        // Is a turn coming up?

		track = &TrackControl[TrackNumber];
		if (IsOnShortTrack) {
			tracknum = track->StartTrack;
		} else {
			tracknum = track->Track;
		}
		ptr = RawTracks[tracknum-1].Track;
		//nextface = LinkedTo->Path[0];

		if (nextface < FACING_NONE || nextface > FACING_COUNT) {
			LinkedTo->Path[0] = FACING_NONE;
			return(false);
		}

		/*
		**	Determine if there is a turn coming up. If there is
		**	a turn, then track jumping might occur.
		*/
		adj = false;
		if (nextface != FACING_COUNT && nextface != FACING_NONE && Dir_Facing(track->Facing) != nextface) {
			adj = true;
		}

		/*
		**	Skip ahead the number of track steps required (limited only
		**	by track length). Set the unit to the new position and
		**	flag the unit accordingly.
		*/
		//LinkedTo->Mark(MARK_UP);
		while (actual > (PIXEL_LEPTON_W + PIXEL_LEPTON_H) / 2) {
			Point2D	offset;
			Dir256	dir;

			actual -= (PIXEL_LEPTON_W + PIXEL_LEPTON_H) / 2;

			offset = ptr[TrackIndex].Offset;
			if (offset != Point2D(0, 0) || !TrackIndex) {
				if (LinkedTo->Occupies_Cells()) {
					LinkedTo->Clear_Occupy_Bit(LinkedTo->PositionCoord);
					LinkedTo->IsOccupyingCell = false;
					LinkedTo->IsToPathAroundBlockage = false;
				}

				Cell oldcell;
				if (!TrackIndex) {
					oldcell = LinkedTo->PositionCell;
				} else {
					dir = ptr[TrackIndex - 1].Facing;
					Point2D prevoffset = ptr[TrackIndex - 1].Offset;
					oldcell = Coord(Smooth_Turn(prevoffset, dir), 0).As_Cell();
				}

				dir = ptr[TrackIndex].Facing;
				Coord newcoord = Coord(Smooth_Turn(offset, dir), 0);
				Cell newcell = newcoord.As_Cell();

				if (LinkedTo->PositionCoord.As_Cell() != newcoord.As_Cell()) {
					LinkedTo->Mark(MARK_UP);
					LinkedTo->PositionCoord = newcoord;
					CellClass * oldcellptr = &Map[oldcell];
					CellClass * newcellptr = &Map[newcell];
					if (newcellptr->Height == oldcellptr->Height - BRIDGE_CELL_HEIGHT && newcellptr->IsUnderBridge) {
						LinkedTo->IsOnBridge = true;
					}
					if (!newcellptr->IsUnderBridge && oldcellptr->IsUnderBridge) {
						LinkedTo->IsOnBridge = false;
					}
					if (LinkedTo->TClass->IsTrain && !((UnitClass *)LinkedTo)->IsFollowing) {
						bool is_bridge = LinkedTo->IsOnBridge || (LinkedTo->PositionCoord.Z >= (Map.Get_Height_GL(LinkedTo->PositionCoord) + BRIDGE_LEPTON_HEIGHT));
						ObjectClass * occupier;
						if (is_bridge) {
							occupier = Map[newcell].Cell_Occupier(true);
						} else {
							occupier = Map[newcell].Cell_Occupier(false);
						}
						while (occupier != NULL) {
							ObjectClass * next = occupier->Next;
							if (!occupier->Class_Of()->IsCrushable) {
								int strength = 10000;
								occupier->Take_Damage(strength, 0, Rule->C4Warhead, NULL, true, true);
								strength = 20;
								LinkedTo->Take_Damage(strength, 0, Rule->C4Warhead, NULL, true, false);
							}
							occupier = next;
						}
					}
					if (!LinkedTo->IsActive) return(false);
					LinkedTo->Mark(MARK_DOWN);

					if (newcellptr->Overlay != OVERLAY_NONE) {
						OverlayTypeClass * otype = OverlayTypes[newcellptr->Overlay];
						if (IsRocking && (LinkedTo->TClass->IsCrusher || LinkedTo->Has_Ability(ABILITY_CRUSHER)) && otype->HeapID == OVERLAY_SANDBAG_WALL) {
							LinkedTo->IsCrushing = true;
							if (LinkedTo->TClass->IsTiltsWhenCrushes) {
								LinkedTo->RockingForwardsPerFrame = -0.02f;
							}
						}
					}
				} else {
					bool down = LinkedTo->IsDown;
					LinkedTo->IsDown = false;
					LinkedTo->PositionCoord = newcoord;
					LinkedTo->IsDown = down;
				}

				if (!LinkedTo->IsActive) {
					return(false);
				}

				bool down = LinkedTo->IsDown;
				LinkedTo->IsDown = false;
				LinkedTo->HeightAGL = 0;
				LinkedTo->IsDown = down;

				LinkedTo->PrimaryFacing.Set(dir);

				/*
				**	See if "per cell" processing is necessary.
				*/
				if (TrackIndex && RawTracks[tracknum-1].Cell == TrackIndex) {
					LinkedTo->Clear_Occupy_Bit(LinkedTo->PositionCoord);
				}

				/*
				**	The unit could "jump tracks". Check to see if the unit should
				**	do so.
				*/
				if (nextface != FACING_COUNT && nextface != FACING_NONE && adj && RawTracks[tracknum-1].Jump == TrackIndex && TrackIndex) {
					TurnTrackType const * newtrack;		// Proposed jump-to track.
					int	tnum;

					tnum = (int)(Dir_Facing(track->Facing) * FACING_COUNT) + (int)nextface;
					newtrack = &TrackControl[tnum];
					if (newtrack->Track && RawTracks[newtrack->Track-1].Entry) {
						double oldspeed = LinkedTo->Speed;

						Coord c = Adjacent_Cell(HeadToCoord, nextface);

						switch (LinkedTo->Can_Enter_Cell(&Map[c.As_Cell()], nextface, LinkedTo->Get_Cell_Height())) {
							case MOVE_OK:
							case MOVE_MOVING_BLOCK:
								if (LinkedTo->RTTI == RTTI_UNIT && !((UnitClass *)LinkedTo)->Class->Passive /* NATIVE: YR's name */) break;
								IsOnShortTrack = false;		// Shouldn't be necessary, but...
								TrackNumber = tnum;
								track = newtrack;

								tracknum = track->Track;
								TrackIndex = RawTracks[tracknum-1].Entry-1;	// Anticipate increment.
								ptr = RawTracks[tracknum-1].Track;
								adj = false;

								Stop_Driver();
								IsDriving = true;
								LinkedTo->Per_Cell_Process(PCP_END);
								IsDriving = false;
								if (LinkedTo == NULL || !LinkedTo->IsActive || LinkedTo->IsInLimbo || LinkedTo->IsFalling) return(false);
								if (Start_Driver(c)) {
									LinkedTo->Set_Speed(oldspeed);
									memcpy((char*)&LinkedTo->Path, (char*)&LinkedTo->Path[1], sizeof(LinkedTo->Path[0]) * (CONQUER_PATH_MAX-1));
									LinkedTo->Path[CONQUER_PATH_MAX-1] = FACING_NONE;
								}
								break;

							case MOVE_CLOAK:
								Map[c].Shimmer();
								break;

							case MOVE_CLOSED_GATE:
								Map.Try_Open_Gate(LinkedTo, c.As_Cell());
								break;

							case MOVE_TEMP: {
									bool bridge;
									if (!Map[c].IsUnderBridge || abs(LinkedTo->PositionCoord.Z / LEVEL_LEPTON_H - Map[c].Height) <= 2) {
										bridge = false;
									} else {
										bridge = true;
									}
									Map[HeadToCoord].Incoming(COORD_NONE, true, true, bridge);
								}
								break;
						}
					}
				}
				TrackIndex++;

			} else {
				Coord dc = HeadToCoord - LinkedTo->PositionCoord;
				int d = abs(dc.X) + abs(dc.Y);
				actual += (int)((1.0 - (double)d / 11.0) * 7.0);
				LinkedTo->IsOccupyingCell = true;
				LinkedTo->IsToPathAroundBlockage = false;

				if (HeadToCoord.As_Cell() != LinkedTo->PositionCoord.As_Cell()) {
					LinkedTo->Mark(MARK_UP);
					LinkedTo->PositionCoord = HeadToCoord;
					LinkedTo->HeightAGL = 0;
					LinkedTo->Mark(MARK_DOWN);
				} else {
					bool down = LinkedTo->IsDown;
					LinkedTo->IsDown = false;
					LinkedTo->PositionCoord = HeadToCoord;
					LinkedTo->HeightAGL = 0;
					LinkedTo->IsDown = down;
				}

				//actual = 0;
				//DestinationCoord = Head_To_Coord();
				Stop_Driver();
				TrackNumber = -1;
				TrackIndex = NULL;

				bool arrived = false;
				if (LinkedTo->NavCom != NULL && LinkedTo->PositionCell == LinkedTo->NavCom->Destination_Coord().As_Cell()) {
					if (abs(LinkedTo->Destination_Coord().Z - DestinationCoord.Z) < 2 * LEVEL_LEPTON_H) {
						arrived = true;
						DestinationCoord = COORD_NONE;
						Stop_Driver();
						IsDriving = false;
					}
				}

				/*
				**	Perform "per cell" activities.
				*/
				//LinkedTo->Mark(MARK_DOWN);
				LinkedTo->Per_Cell_Process(PCP_END);
				if (LinkedTo == NULL || !LinkedTo->IsActive || LinkedTo->IsInLimbo || LinkedTo->IsFalling) return(true);
				//LinkedTo->Mark(MARK_UP);

				if (arrived) {
					LinkedTo->NavCom = NULL;
					LinkedTo->Path[0] = FACING_NONE;
					if (LinkedTo->Mission == MISSION_MOVE && LinkedTo->Enter_Idle_Mode()) {
						return(true);
					}
				}

				if (LinkedTo->Is_Docked_For_Repair()) {
					return(true);
				}

				if (!LinkedTo->IsActive) return(false);

				break;
			}
		}
		// if (LinkedTo->IsActive) {
		// 	LinkedTo->Mark(MARK_DOWN);
		// }
	}

	/*
	**	Replace any remainder back into the unit's movement
	**	accumulator to be processed next pass.
	*/
	SpeedAccum = actual;

	if (actual > 0 && TrackNumber > -1) {
		TurnTrackType	const * track;  // Track control pointer.
		TrackType		const	* ptr;  // Pointer to coord offset values.
		int				tracknum;       // The track number being processed.

		track = &TrackControl[TrackNumber];
		if (IsOnShortTrack) {
			tracknum = track->StartTrack;
		} else {
			tracknum = track->Track;
		}
		ptr = RawTracks[tracknum-1].Track;

		Point2D	offset;
		Dir256	dir;

		offset = ptr[TrackIndex].Offset;
		if (offset != Point2D(0, 0) || !TrackIndex) {

			Coord oldcoord = LinkedTo->PositionCoord;

			dir = ptr[TrackIndex].Facing;
			Coord movement = Coord(Smooth_Turn(offset, dir) - LinkedTo->PositionCoord, 0);
			Coord stepcoord = movement + LinkedTo->PositionCoord;
			CellClass * stepcellptr = &Map[stepcoord];

			Coord partial(0, 0, 0);
			partial = Lerp(partial, movement, SpeedAccum / 7.0); /// 7 == (CELL_LEPTON / ((CELL_PIXEL_W + CELL_PIXEL_H) / 2))
			Coord partialcoord = oldcoord + partial;

			CellClass * oldcellptr = &Map[oldcoord];
			CellClass * partialcellptr = &Map[partialcoord];

			Coord poscoord = LinkedTo->PositionCoord;
			Coord coord = poscoord;

			if (partialcellptr != stepcellptr && partialcellptr != oldcellptr) {
				if (SpeedAccum > 3) {
					coord = stepcoord;
					coord.Z = poscoord.Z;
				}
			} else {
				coord = partialcoord;
				coord.Z = poscoord.Z;
			}

			CellClass * newcellptr = &Map[coord];

			if (coord.As_Cell() != LinkedTo->PositionCoord.As_Cell()) {
				LinkedTo->Mark(MARK_UP);
				LinkedTo->PositionCoord = coord;
				if (newcellptr->Height == oldcellptr->Height - BRIDGE_CELL_HEIGHT && newcellptr->IsUnderBridge) {
					LinkedTo->IsOnBridge = true;
				}
				if (!newcellptr->IsUnderBridge && oldcellptr->IsUnderBridge) {
					LinkedTo->IsOnBridge = false;
				}
				LinkedTo->Mark(MARK_DOWN);
			} else {
				bool down = LinkedTo->IsDown;
				LinkedTo->IsDown = false;
				LinkedTo->PositionCoord = coord;
				LinkedTo->IsDown = down;
			}
		}
	}

	return(false);
}

// ---------------------------------------------------------------------------------------------
// NATIVE (lane Y4): the drive locomotor's track tables. yr_data.py cannot load them from
// gamemd.exe (RawTracks holds pointers to the track arrays, which no global names), so they were
// zero and every turn track was empty. TibSun's tables (TibSun archive code/drive.cpp; same
// licence notice as above); gamemd.exe's RawTracks jump/entry/cell values (0x007E7A38) are
// TibSun's, and its track arrays have TibSun's extents.
// ---------------------------------------------------------------------------------------------
ShipLocomotionClass::TrackType const ShipLocomotionClass::Track1[24] = {
	{{0,245},(Dir256)0},
	{{0,234},(Dir256)0},
	{{0,223},(Dir256)0},
	{{0,212},(Dir256)0},
	{{0,201},(Dir256)0},
	{{0,190},(Dir256)0},
	{{0,179},(Dir256)0},
	{{0,168},(Dir256)0},
	{{0,157},(Dir256)0},
	{{0,146},(Dir256)0},
	{{0,135},(Dir256)0},
	{{0,124},(Dir256)0},		// Track jump check here.
	{{0,113},(Dir256)0},
	{{0,102},(Dir256)0},
	{{0,91},(Dir256)0},
	{{0,80},(Dir256)0},
	{{0,69},(Dir256)0},
	{{0,58},(Dir256)0},
	{{0,47},(Dir256)0},
	{{0,36},(Dir256)0},
	{{0,25},(Dir256)0},
	{{0,14},(Dir256)0},
	{{0,3},(Dir256)0},
	{{0,0},(Dir256)0}
};

ShipLocomotionClass::TrackType const ShipLocomotionClass::Track2[] = {
	{{-248,248},(Dir256)32},
	{{-240,240},(Dir256)32},
	{{-232,232},(Dir256)32},
	{{-224,224},(Dir256)32},
	{{-216,216},(Dir256)32},
	{{-208,208},(Dir256)32},
	{{-200,200},(Dir256)32},
	{{-192,192},(Dir256)32},
	{{-184,184},(Dir256)32},
	{{-176,176},(Dir256)32},
	{{-168,168},(Dir256)32},
	{{-160,160},(Dir256)32},
	{{-152,152},(Dir256)32},
	{{-144,144},(Dir256)32},
	{{-136,136},(Dir256)32},
	{{-129,129},(Dir256)32},		// Track jump check here.
	{{-120,120},(Dir256)32},
	{{-112,112},(Dir256)32},
	{{-104,104},(Dir256)32},
	{{-96,96},(Dir256)32},
	{{-88,88},(Dir256)32},
	{{-80,80},(Dir256)32},
	{{-72,72},(Dir256)32},
	{{-64,64},(Dir256)32},
	{{-56,56},(Dir256)32},
	{{-48,48},(Dir256)32},
	{{-40,40},(Dir256)32},
	{{-32,32},(Dir256)32},
	{{-24,24},(Dir256)32},
	{{-16,16},(Dir256)32},
	{{-8,8},(Dir256)32},
	{{0,0},(Dir256)32}
};

ShipLocomotionClass::TrackType const ShipLocomotionClass::Track3[] = {
	{{-256,501},(Dir256)0},
	{{-256,490},(Dir256)0},
	{{-256,479},(Dir256)0},
	{{-256,468},(Dir256)0},
	{{-256,457},(Dir256)0},
	{{-256,446},(Dir256)0},
	{{-256,435},(Dir256)0},
	{{-256,424},(Dir256)0},
	{{-256,413},(Dir256)0},
	{{-256,402},(Dir256)0},
	{{-256,391},(Dir256)0},
	{{-256,383},(Dir256)0},
	{{-256,373},(Dir256)0},		// Jump entry point here.
	{{-256,363},(Dir256)0},
	{{-254,352},(Dir256)1},
	{{-252,341},(Dir256)3},
	{{-250,332},(Dir256)4},
	{{-248,321},(Dir256)5},
	{{-245,311},(Dir256)7},
	{{-241,302},(Dir256)8},
	{{-237,292},(Dir256)9},
	{{-233,282},(Dir256)11},
	{{-229,272},(Dir256)12},
	{{-225,263},(Dir256)13},		// Center cell processing here.
	{{-220,252},(Dir256)15},
	{{-216,243},(Dir256)16},
	{{-212,236},(Dir256)17},
	{{-206,224},(Dir256)19},
	{{-202,215},(Dir256)20},
	{{-195,207},(Dir256)21},
	{{-190,198},(Dir256)23},
	{{-183,186},(Dir256)24},
	{{-179,176},(Dir256)25},
	{{-168,168},(Dir256)27},
	{{-160,160},(Dir256)28},
	{{-152,152},(Dir256)29},
	{{-144,144},(Dir256)31},
	{{-136,136},(Dir256)32},
	{{-129,129},(Dir256)32},		// Track jump check here.
	{{-120,120},(Dir256)32},
	{{-112,112},(Dir256)32},
	{{-104,104},(Dir256)32},
	{{-96,96},(Dir256)32},
	{{-88,88},(Dir256)32},
	{{-80,80},(Dir256)32},
	{{-72,72},(Dir256)32},
	{{-64,64},(Dir256)32},
	{{-56,56},(Dir256)32},
	{{-48,48},(Dir256)32},
	{{-40,40},(Dir256)32},
	{{-32,32},(Dir256)32},
	{{-24,24},(Dir256)32},
	{{-16,16},(Dir256)32},
	{{-8,8},(Dir256)32},
	{{0,0},(Dir256)32}
};

ShipLocomotionClass::TrackType const ShipLocomotionClass::Track4[] = {
	{{-256,245},(Dir256)0},
	{{-256,235},(Dir256)0},
	{{-256,224},(Dir256)0},
	{{-256,213},(Dir256)0},
	{{-255,203},(Dir256)0},
	{{-253,192},(Dir256)0},
	{{-251,181},(Dir256)1},
	{{-249,171},(Dir256)1},
	{{-246,160},(Dir256)2},
	{{-243,149},(Dir256)3},
	{{-240,139},(Dir256)4},
	{{-236,127},(Dir256)5},		// Track entry here.
	{{-232,117},(Dir256)8},
	{{-228,109},(Dir256)12},
	{{-222,99},(Dir256)16},
	{{-219,90},(Dir256)20},
	{{-213,82},(Dir256)23},
	{{-206,72},(Dir256)27},
	{{-201,64},(Dir256)32},
	{{-195,56},(Dir256)36},
	{{-186,48},(Dir256)39},
	{{-177,43},(Dir256)43},
	{{-168,36},(Dir256)47},
	{{-160,32},(Dir256)51},
	{{-147,27},(Dir256)54},
	{{-135,23},(Dir256)57},
	{{-126,20},(Dir256)60},		// Track jump here.
	{{-113,17},(Dir256)62},
	{{-104,13},(Dir256)63},
	{{-94,9},(Dir256)64},
	{{-84,6},(Dir256)64},
	{{-75,4},(Dir256)66},
	{{-64,3},(Dir256)64},
	{{-53,2},(Dir256)64},
	{{-43,1},(Dir256)64},
	{{-32,0},(Dir256)64},
	{{-21,0},(Dir256)64},
	{{-11,0},(Dir256)64},
	{{0,0},(Dir256)64}
};

ShipLocomotionClass::TrackType const ShipLocomotionClass::Track5[] = {
	{{-504,-8},(Dir256)32},
	{{-496,-16},(Dir256)32},
	{{-488,-24},(Dir256)32},
	{{-480,-32},(Dir256)32},
	{{-472,-40},(Dir256)32},
	{{-464,-48},(Dir256)32},
	{{-456,-56},(Dir256)32},
	{{-448,-64},(Dir256)32},
	{{-440,-72},(Dir256)32},
	{{-432,-80},(Dir256)32},
	{{-424,-88},(Dir256)32},
	{{-416,-96},(Dir256)32},
	{{-408,-104},(Dir256)32},
	{{-400,-112},(Dir256)32},
	{{-392,-120},(Dir256)32},
	{{-385,-127},(Dir256)32},		// Track entry here.
	{{-376,-136},(Dir256)32},
	{{-368,-143},(Dir256)32},
	{{-361,-150},(Dir256)32},
	{{-353,-158},(Dir256)32},
	{{-344,-166},(Dir256)32},
	{{-336,-173},(Dir256)35},
	{{-329,-181},(Dir256)38},
	{{-322,-188},(Dir256)41},
	{{-316,-194},(Dir256)44},
	{{-306,-199},(Dir256)47},
	{{-296,-204},(Dir256)50},
	{{-288,-208},(Dir256)53},
	{{-277,-211},(Dir256)56},
	{{-267,-212},(Dir256)59},
	{{-256,-213},(Dir256)62},
	{{-245,-212},(Dir256)66},
	{{-235,-211},(Dir256)69},
	{{-225,-208},(Dir256)72},
	{{-216,-204},(Dir256)75},
	{{-208,-199},(Dir256)78},
	{{-198,-194},(Dir256)81},
	{{-188,-188},(Dir256)84},
	{{-181,-181},(Dir256)87},
	{{-176,-173},(Dir256)90},
	{{-168,-166},(Dir256)93},
	{{-160,-158},(Dir256)96},
	{{-152,-150},(Dir256)96},
	{{-144,-143},(Dir256)96},
	{{-136,-136},(Dir256)96},
	{{-129,-129},(Dir256)96},		// Track jump check here.
	{{-120,-120},(Dir256)96},
	{{-112,-112},(Dir256)96},
	{{-104,-104},(Dir256)96},
	{{-96,-96},(Dir256)96},
	{{-88,-88},(Dir256)96},
	{{-80,-80},(Dir256)96},
	{{-72,-72},(Dir256)96},
	{{-64,-64},(Dir256)96},
	{{-56,-56},(Dir256)96},
	{{-48,-48},(Dir256)96},
	{{-40,-40},(Dir256)96},
	{{-32,-32},(Dir256)96},
	{{-24,-24},(Dir256)96},
	{{-16,-16},(Dir256)96},
	{{-8,-8},(Dir256)96},
	{{0,0},(Dir256)96}
};

ShipLocomotionClass::TrackType const ShipLocomotionClass::Track6[] = {
	{{-512,256},(Dir256)32},
	{{-504,248},(Dir256)32},
	{{-496,240},(Dir256)32},
	{{-488,232},(Dir256)32},
	{{-480,224},(Dir256)32},
	{{-472,216},(Dir256)32},
	{{-464,208},(Dir256)32},
	{{-456,200},(Dir256)32},
	{{-448,192},(Dir256)32},
	{{-440,184},(Dir256)32},
	{{-432,176},(Dir256)32},
	{{-424,168},(Dir256)32},
	{{-416,160},(Dir256)32},
	{{-408,152},(Dir256)32},
	{{-400,144},(Dir256)32},
	{{-392,136},(Dir256)32},
	{{-385,129},(Dir256)32},		// Jump entry point here.
	{{-376,120},(Dir256)32},
	{{-368,112},(Dir256)32},
	{{-360,104},(Dir256)32},
	{{-352,96},(Dir256)32},
	{{-344,88},(Dir256)32},
	{{-338,85},(Dir256)32},
	{{-328,78},(Dir256)35},
	{{-320,72},(Dir256)37},
	{{-311,66},(Dir256)40},
	{{-302,59},(Dir256)43},
	{{-294,55},(Dir256)45},
	{{-285,50},(Dir256)48},
	{{-277,43},(Dir256)51},
	{{-267,38},(Dir256)53},
	{{-258,34},(Dir256)56},
	{{-248,28},(Dir256)59},
	{{-238,25},(Dir256)61},
	{{-229,21},(Dir256)64},
	{{-218,17},(Dir256)64},
	{{-208,14},(Dir256)64},
	{{-199,11},(Dir256)64},
	{{-189,9},(Dir256)64},
	{{-178,7},(Dir256)64},
	{{-169,5},(Dir256)64},
	{{-158,3},(Dir256)64},
	{{-147,1},(Dir256)64},
	{{-137,0},(Dir256)64},
	{{-129,0},(Dir256)64},		// Track jump check here.
	{{-117,0},(Dir256)64},
	{{-107,0},(Dir256)64},
	{{-96,0},(Dir256)64},
	{{-85,0},(Dir256)64},
	{{-75,0},(Dir256)64},
	{{-64,0},(Dir256)64},
	{{-53,0},(Dir256)64},
	{{-43,0},(Dir256)64},
	{{-32,0},(Dir256)64},
	{{-21,0},(Dir256)64},
	{{-11,0},(Dir256)64},
	{{0,0},(Dir256)64}
};

ShipLocomotionClass::TrackType const ShipLocomotionClass::Track7[] = {
	{{-1,6},(Dir256)0},
	{{-2,12},(Dir256)4},
	{{-4,17},(Dir256)8},
	{{-6,24},(Dir256)12},
	{{-10,31},(Dir256)16},
	{{-13,36},(Dir256)19},
	{{-16,43},(Dir256)22},
	{{-3,48},(Dir256)23},
	{{-21,53},(Dir256)24},
	{{-24,56},(Dir256)25},
	{{-26,60},(Dir256)26},
	{{-29,64},(Dir256)27},
	{{-32,67},(Dir256)28},
	{{-35,70},(Dir256)29},
	{{-33,67},(Dir256)30},
	{{-31,64},(Dir256)30},
	{{-29,60},(Dir256)30},
	{{-27,56},(Dir256)30},
	{{-25,53},(Dir256)31},
	{{-23,48},(Dir256)31},
	{{-21,43},(Dir256)31},
	{{-19,36},(Dir256)31},
	{{-15,31},(Dir256)31},
	{{-12,24},(Dir256)32},
	{{-9,17},(Dir256)32},
	{{-6,12},(Dir256)32},
	{{-3,6},(Dir256)32},
	{{0,0},(Dir256)32}
};

ShipLocomotionClass::TrackType const ShipLocomotionClass::Track8[] = {
	{{-4,3},(Dir256)32},
	{{-9,6},(Dir256)36},
	{{-15,10},(Dir256)40},
	{{-21,12},(Dir256)44},
	{{-28,13},(Dir256)46},
	{{-36,14},(Dir256)48},
	{{-43,15},(Dir256)50},
	{{-48,16},(Dir256)52},
	{{-55,17},(Dir256)54},
	{{-62,18},(Dir256)56},
	{{-64,17},(Dir256)58},
	{{-62,16},(Dir256)60},
	{{-55,14},(Dir256)62},
	{{-49,12},(Dir256)64},
	{{-43,10},(Dir256)64},
	{{-38,8},(Dir256)64},
	{{-30,6},(Dir256)64},
	{{-23,4},(Dir256)64},
	{{-17,2},(Dir256)64},
	{{-11,1},(Dir256)64},
	{{-7,0},(Dir256)64},
	{{0,0},(Dir256)64}
};

ShipLocomotionClass::TrackType const ShipLocomotionClass::Track9[] = {
	{{2,-11},(Dir256)0},
	{{4,-21},(Dir256)2},
	{{6,-32},(Dir256)4},
	{{9,-43},(Dir256)6},
	{{12,-50},(Dir256)9},
	{{15,-56},(Dir256)11},
	{{18,-64},(Dir256)13},
	{{21,-72},(Dir256)16},
	{{18,-64},(Dir256)18},
	{{14,-56},(Dir256)20},
	{{10,-50},(Dir256)22},
	{{4,-43},(Dir256)24},
	{{0,-34},(Dir256)26},
	{{-8,-23},(Dir256)28},
	{{-14,-18},(Dir256)30},
	{{-21,-11},(Dir256)32},
	{{-31,-3},(Dir256)34},
	{{-40,2},(Dir256)36},
	{{-46,7},(Dir256)39},
	{{-53,11},(Dir256)41},
	{{-59,16},(Dir256)43},
	{{-66,19},(Dir256)45},
	{{-73,21},(Dir256)48},
	{{-66,19},(Dir256)50},
	{{-59,17},(Dir256)52},
	{{-52,11},(Dir256)54},
	{{-44,8},(Dir256)56},
	{{-33,5},(Dir256)58},
	{{-21,3},(Dir256)62},
	{{-11,1},(Dir256)64},
	{{0,0},(Dir256)64}
};

ShipLocomotionClass::TrackType const ShipLocomotionClass::Track10[] = {
	{{11,-10},(Dir256)32},
	{{21,-16},(Dir256)37},
	{{32,-21},(Dir256)42},
	{{43,-23},(Dir256)47},
	{{50,-27},(Dir256)52},
	{{56,-29},(Dir256)57},
	{{64,-32},(Dir256)60},
	{{56,-30},(Dir256)62},
	{{50,-28},(Dir256)64},
	{{42,-27},(Dir256)68},
	{{30,-26},(Dir256)70},
	{{21,-25},(Dir256)72},
	{{11,-24},(Dir256)74},
	{{0,-23},(Dir256)76},
	{{-11,-24},(Dir256)78},
	{{-21,-25},(Dir256)80},
	{{-32,-26},(Dir256)82},
	{{-43,-27},(Dir256)84},
	{{-50,-28},(Dir256)86},
	{{-59,-30},(Dir256)88},
	{{-64,-32},(Dir256)90},
	{{-59,-29},(Dir256)92},
	{{-50,-27},(Dir256)94},
	{{-43,-23},(Dir256)95},
	{{-32,-21},(Dir256)96},
	{{-21,-16},(Dir256)96},
	{{-11,-10},(Dir256)96},
	{{0,0},(Dir256)96}
};

ShipLocomotionClass::TrackType const ShipLocomotionClass::Track11[] = {
	{{0,256},DIR_SW},
	{{8,243},DIR_SW},
	{{16,229},DIR_SW},
	{{24,214},DIR_SW},
	{{32,200},DIR_SW},
	{{40,185},DIR_SW},
	{{48,171},DIR_SW},
	{{56,156},DIR_SW},
	{{64,141},DIR_SW},
	{{72,127},DIR_SW},
	{{80,113},DIR_SW},
	{{88,100},DIR_SW},
	{{96,85},DIR_SW},

	{{0,0},DIR_SW}
};

ShipLocomotionClass::TrackType const ShipLocomotionClass::Track12[] = {
	{{96,-171},DIR_SW},
	{{88,-156},DIR_SW},
	{{80,-143},DIR_SW},
	{{72,-129},DIR_SW},
	{{64,-115},DIR_SW},
	{{56,-100},DIR_SW},
	{{48,-85},DIR_SW},
	{{40,-71},DIR_SW},
	{{32,-56},DIR_SW},
	{{24,-42},DIR_SW},
	{{16,-27},DIR_SW},
	{{8,-13},DIR_SW},

	{{0,0},DIR_SW}
};

/*
**	Drive out of weapon's factory.
*/
ShipLocomotionClass::TrackType const ShipLocomotionClass::Track13[] = {
	{{-670,-68},DIR_E},
	{{-660,-67},DIR_E},
	{{-650,-66},DIR_E},
	{{-639,-65},DIR_E},
	{{-630,-64},DIR_E},
	{{-620,-63},DIR_E},
	{{-610,-62},DIR_E},
	{{-600,-61},DIR_E},
	{{-590,-60},DIR_E},
	{{-580,-59},DIR_E},
	{{-570,-58},DIR_E},
	{{-560,-57},DIR_E},
	{{-550,-56},DIR_E},
	{{-540,-55},DIR_E},
	{{-530,-54},DIR_E},
	{{-520,-53},DIR_E},
	{{-510,-52},DIR_E},
	{{-500,-51},DIR_E},
	{{-490,-50},DIR_E},
	{{-480,-49},DIR_E},
	{{-470,-48},DIR_E},
	{{-460,-47},DIR_E},
	{{-450,-46},DIR_E},
	{{-440,-45},DIR_E},
	{{-430,-44},DIR_E},
	{{-420,-43},DIR_E},
	{{-410,-42},DIR_E},
	{{-400,-41},DIR_E},
	{{-390,-40},DIR_E},
	{{-380,-39},DIR_E},
	{{-370,-38},DIR_E},
	{{-360,-37},DIR_E},
	{{-350,-36},DIR_E},
	{{-340,-35},DIR_E},
	{{-330,-34},DIR_E},
	{{-320,-33},DIR_E},
	{{-310,-32},DIR_E},
	{{-300,-31},DIR_E},
	{{-290,-30},DIR_E},
	{{-280,-29},DIR_E},
	{{-270,-28},DIR_E},
	{{-260,-27},DIR_E},
	{{-250,-26},DIR_E},
	{{-240,-25},DIR_E},
	{{-230,-24},DIR_E},
	{{-220,-23},DIR_E},
	{{-210,-22},DIR_E},
	{{-200,-21},DIR_E},
	{{-190,-20},DIR_E},
	{{-180,-19},DIR_E},
	{{-170,-18},DIR_E},
	{{-160,-17},DIR_E},
	{{-150,-16},DIR_E},
	{{-140,-15},DIR_E},
	{{-130,-14},DIR_E},
	{{-120,-13},DIR_E},
	{{-110,-12},DIR_E},
	{{-100,-11},DIR_E},
	{{-90,-10},DIR_E},
	{{-80,-9},DIR_E},
	{{-70,-8},DIR_E},
	{{-60,-7},DIR_E},
	{{-50,-6},DIR_E},
	{{-40,-5},DIR_E},
	{{-30,-4},DIR_E},
	{{-20,-3},DIR_E},
	{{-10,-2},DIR_E},
	{{0,-1},DIR_E},

	{{0,0},DIR_E}
};


/*
**	There are a limited basic number of tracks that a vehicle can follow. These
**	are they. Each track can be interpreted differently but this is controlled
**	by the TrackControl structure elaborated elsewhere.
*/
ShipLocomotionClass::RawTrackType const ShipLocomotionClass::RawTracks[13] = {
	{Track1, -1, 0, -1},
	{Track2, -1, 0, -1},
	{Track3, 37, 12, 22},
	{Track4, 26, 11, 19},
	{Track5, 45, 15, 31},
	{Track6, 44, 16, 27},
	{Track7, -1, 0, -1},
	{Track8, -1, 0, -1},
	{Track9, -1, 0, -1},
	{Track10, -1, 0, -1},
	{Track11, -1, 0, -1},
	{Track12, -1, 0, -1},
	{Track13, -1, 0, -1}
};


/***************************************************************************
**	Smooth turning control table. Given two directions in a path list, this
**	table determines which track to use and what modifying operations need
**	be performed on the track data.
*/
ShipLocomotionClass::TurnTrackType const ShipLocomotionClass::TrackControl[67] = {
	{1,	0,		DIR_N,	F_},                                                                // 0-0
	{3,	7,		DIR_NE,	F_D},                                                               //	0-1 (raw chart)
	{4,	9,		DIR_E,	F_D},                                                               //	0-2 (raw chart)
	{0,	0,		DIR_SE,	F_},                                                                // 0-3 !
	{0,	0,		DIR_S,	F_},                                                                // 0-4 !
	{0,	0,		DIR_SW,	F_},                                                                // 0-5 !
	{4,	9,		DIR_W,	(ShipLocomotionClass::TrackControlType)(F_X|F_D)},                 // 0-6
	{3,	7,		DIR_NW,	(ShipLocomotionClass::TrackControlType)(F_X|F_D)},                 // 0-7
	{6,	8,		DIR_N,	(ShipLocomotionClass::TrackControlType)(F_T|F_X|F_Y|F_D)},         // 1-0
	{2,	0,		DIR_NE,	F_},                                                                //	1-1 (raw chart)
	{6,	8,		DIR_E,	F_D},                                                               //	1-2 (raw chart)
	{5,	10,	DIR_SE,	F_D},                                                                   //	1-3 (raw chart)
	{0,	0,		DIR_S,	F_},                                                                // 1-4 !
	{0,	0,		DIR_SW,	F_},                                                                // 1-5 !
	{0,	0,		DIR_W,	F_},                                                                // 1-6 !
	{5,	10,	DIR_NW,	(ShipLocomotionClass::TrackControlType)(F_T|F_X|F_Y|F_D)},             // 1-7
	{4,	9,		DIR_N,	(ShipLocomotionClass::TrackControlType)(F_T|F_X|F_Y|F_D)},         // 2-0
	{3,	7,		DIR_NE,	(ShipLocomotionClass::TrackControlType)(F_T|F_X|F_Y|F_D)},         // 2-1
	{1,	0,		DIR_E,	(ShipLocomotionClass::TrackControlType)(F_T|F_X)},                 // 2-2
	{3,	7,		DIR_SE,	(ShipLocomotionClass::TrackControlType)(F_T|F_X|F_D)},             // 2-3
	{4,	9,		DIR_S,	(ShipLocomotionClass::TrackControlType)(F_T|F_X|F_D)},             // 2-4
	{0,	0,		DIR_SW,	F_},                                                                // 2-5 !
	{0,	0,		DIR_W,	F_},                                                                // 2-6 !
	{0,	0,		DIR_NW,	F_},                                                                // 2-7 !
	{0,	0,		DIR_N,	F_},                                                                // 3-0 !
	{5,	10,	DIR_NE,	(ShipLocomotionClass::TrackControlType)(F_Y|F_D)},                     // 3-1
	{6,	8,		DIR_E,	(ShipLocomotionClass::TrackControlType)(F_Y|F_D)},                 // 3-2
	{2,	0,		DIR_SE,	F_Y},                                                               // 3-3
	{6,	8,		DIR_S,	(ShipLocomotionClass::TrackControlType)(F_T|F_X|F_D)},             // 3-4
	{5,	10,	DIR_SW,	(ShipLocomotionClass::TrackControlType)(F_T|F_X|F_D)},                 // 3-5
	{0,	0,		DIR_W,	F_},                                                                // 3-6 !
	{0,	0,		DIR_NW,	F_},                                                                // 3-7 !
	{0,	0,		DIR_N,	F_},                                                                // 4-0 !
	{0,	0,		DIR_NE,	F_},                                                                // 4-1 !
	{4,	9,		DIR_E,	(ShipLocomotionClass::TrackControlType)(F_Y|F_D)},                 // 4-2
	{3,	7,		DIR_SE,	(ShipLocomotionClass::TrackControlType)(F_Y|F_D)},                 // 4-3
	{1,	0,		DIR_S,	F_Y},                                                               // 4-4
	{3,	7,		DIR_SW,	(ShipLocomotionClass::TrackControlType)(F_X|F_Y|F_D)},             // 4-5
	{4,	9,		DIR_W,	(ShipLocomotionClass::TrackControlType)(F_X|F_Y|F_D)},             // 4-6
	{0,	0,		DIR_NW,	F_},                                                                // 4-7 !
	{0,	0,		DIR_N,	F_},                                                                // 5-0 !
	{0,	0,		DIR_NE,	F_},                                                                // 5-1 !
	{0,	0,		DIR_E,	F_},                                                                // 5-2 !
	{5,	10,	DIR_SE,	(ShipLocomotionClass::TrackControlType)(F_T|F_D)},                     // 5-3
	{6,	8,		DIR_S,	(ShipLocomotionClass::TrackControlType)(F_T|F_D)},                 // 5-4
	{2,	0,		DIR_SW,	F_T},                                                               // 5-5
	{6,	8,		DIR_W,	(ShipLocomotionClass::TrackControlType)(F_X|F_Y|F_D)},             // 5-6
	{5,	10,	DIR_NW,	(ShipLocomotionClass::TrackControlType)(F_X|F_Y|F_D)},                 // 5-7
	{4,	9,		DIR_N,	(ShipLocomotionClass::TrackControlType)(F_T|F_Y|F_D)},             // 6-0
	{0,	0,		DIR_NE,	F_},                                                                // 6-1 !
	{0,	0,		DIR_E,	F_},                                                                // 6-2 !
	{0,	0,		DIR_SE,	F_},                                                                // 6-3 !
	{4,	9,		DIR_S,	(ShipLocomotionClass::TrackControlType)(F_T|F_D)},                 // 6-4
	{3,	7,		DIR_SW,	(ShipLocomotionClass::TrackControlType)(F_T|F_D)},                 // 6-5
	{1,	0,		DIR_W,	F_T},                                                               // 6-6
	{3,	7,		DIR_NW,	(ShipLocomotionClass::TrackControlType)(F_T|F_Y|F_D)},             // 6-7
	{6,	8,		DIR_N,	(ShipLocomotionClass::TrackControlType)(F_T|F_Y|F_D)},             // 7-0
	{5,	10,	DIR_NE,	(ShipLocomotionClass::TrackControlType)(F_T|F_Y|F_D)},                 // 7-1
	{0,	0,		DIR_E,	F_},                                                                // 7-2 !
	{0,	0,		DIR_SE,	F_},                                                                // 7-3 !
	{0,	0,		DIR_S,	F_},                                                                // 7-4 !
	{5,	10,	DIR_SW,	(ShipLocomotionClass::TrackControlType)(F_X|F_D)},                     // 7-5
	{6,	8,		DIR_W,	(ShipLocomotionClass::TrackControlType)(F_X|F_D)},                 // 7-6
	{2,	0,		DIR_NW,	F_X},                                                               // 7-7

	{11,	11,	DIR_SW,	F_},                                                            // Backup harvester into refinery.
	{12,	12,	DIR_SW,	F_},                                                            // Drive back into refinery.
	{13,	13,	DIR_SW,	F_}                                                             // Drive out of weapons factory.
};
