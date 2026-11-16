
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y4), appended to the infantry unit: InfantryClass::Assign_Destination
// (AbstractClass *, bool), YR 0x0051AA40 (1,953 bytes, unmatched in ra2-decomp). TibSun's text
// (TibSun archive code/infantry.cpp; SPDX GPL-3.0-or-later, Copyright 2025 Electronic Arts Inc. and
// OpenTS contributors, EA's GPL section 7 terms; THIRD_PARTY.md) with gamemd.exe's changes, read
// from its code and marked YR: the deploy-sequence and same-cell early returns, the rocker link
// release, the transport queue (QueueUpToEnter) and the docking reply, the jump jet's fly test that
// also honours the type's HoverAttack flag (+0x390). Behaviour code for the native build only.
// ---------------------------------------------------------------------------------------------
void InfantryClass::Assign_Destination(AbstractClass * target, bool immediate)
{
	/*
	**	YR: a human player's soldier in one of the deploy sequences (DoType 27-30) keeps it.
	*/
	if (Owner_HouseClass()->Is_Human_Player()) {
		if (Doing == (DoType)27 || Doing == (DoType)28 || Doing == (DoType)29 || Doing == (DoType)30) {
			return;
		}
	}

	/*
	**	YR: a jump jet soldier told to go to the cell it is in ignores the order.
	*/
	if (Class->JumpJet && target != NULL) {
		Cell here = Center_Coord().As_Cell();
		Cell there = target->Center_Coord().As_Cell();
		if (here == there) {
			return;
		}
	}

	/*
	**	YR: break the rocker link with the unit this one was paired with.
	*/
	if (DirectRockerLinkedUnit != NULL) {
		DirectRockerLinkedUnit->DirectRockerLinkedUnit = NULL;
		DirectRockerLinkedUnit = NULL;
	}

	/*
	**	Special flag so that infantry will start heading in the right direction immediately.
	*/
	if (Locomotion->Is_Moving() && target != NULL && Map[Center_Coord()].Is_Clear_To_Move(Class->Speed, true, false)) {
		if (Class->JumpJet || CurrentMission != MISSION_ATTACK || NavCom != target) {
			Stop_Driver();
		}
	}

	/*
	**	When telling an infantry soldier to move to a location twice, then this
	**	means that movement is more important than safety. Get up and run!
	*/
	if (House->Is_Human_Player() && target != NULL && NavCom == target && IsProne && !Class->IsFraidyCat && !Class->IsCyborg) {
		Do_Action(DO_GET_UP);
	}

	/*
	**	Handle entry logic here.
	*/
	if (Get_Mission() == MISSION_ENTER || MissionQueue == MISSION_ENTER) {

		/*
		**	If not already in radio contact (presumed with the transport), then
		**	either try to establish contact if allowed, or just move close and
		**	wait until radio contact can be established.
		*/
		if (!HasAnyLink()) {
			TechnoClass * techno = Dynamic_Cast<TechnoClass *>(target);
			if (techno != NULL) {

				/*
				**	Determine if the transport is already in radio contact. If so, then just move
				**	toward the transport and try to establish contact at a later time.
				*/
				if (techno->HasAnyLink()) {
					if (techno->Fetch_RTTI() == RTTI_UNIT || techno->Fetch_RTTI() == RTTI_BUILDING) {
						/*
						**	YR: queue up to enter the busy transport or building instead of walking to it.
						*/
						QueueUpToEnter = techno;
						ArchiveTarget = NULL;
						target = NULL;
						BASECLASS::Assign_Destination(target, immediate);
						return;
					}
					ArchiveTarget = target;
				} else {
					if (Transmit_Message(RADIO_HELLO, techno) == RADIO_ROGER) {
						RadioMessageType reply = Transmit_Message(RADIO_DOCKING);
						if (reply == RADIO_DOCKING) {
							return;		// YR
						}
						if (reply != RADIO_ROGER) {
							Transmit_Message(RADIO_OVER_OUT);
						}
					}
				}
			}
		} else {
			Path[0] = FACING_NONE;
		}
	} else {
		Path[0] = FACING_NONE;
	}

	if (target != NULL && Class->JumpJet && Locomotion->Is_Moving()) {
		IPersistPtr persist(Locomotion);
		CLSID clsid;
		persist->GetClassID(&clsid);
		if (clsid == CLSID_WalkLocomotion) {
			NavQueue.Add_Head(target);
			target = Get_Target_Cell_Ptr();
			if (target != NULL && ((CellClass *)target)->IsUnderBridge) {
				target = NULL;
			}
		}
	}

	if (Class->JumpJet && target != NULL && !Locomotion->Is_Moving()) {
		bool should_fly = Should_JumpJet_Fly(Destination_Coord().As_Cell(), target->Center_Coord().As_Cell());
		bool jumpjet = Is_JumpJet();
		if (Class->HoverAttack) {
			should_fly = true;		// YR: the type's flag at +0x390 always flies
		}
		if (jumpjet) {
			if (!should_fly) {
				IPiggybackPtr piggy(Locomotion);
				if (piggy != NULL) {
					if (piggy->Is_Piggybacking() && piggy->Is_Ok_To_End()) {
						piggy->End_Piggyback(&Locomotion);
					}
				}
				ILocomotionPtr walk(CLSID_WalkLocomotion);
				walk->Link_To_Object(this);
				piggy = IPiggybackPtr(walk);
				if (piggy != NULL) {
					piggy->Begin_Piggyback(Locomotion);
					Locomotion = walk;
				}
			}
		} else {
			if (should_fly) {
				IPiggybackPtr piggy(Locomotion);
				if (piggy != NULL) {
					if (piggy->Is_Piggybacking() && piggy->Is_Ok_To_End()) {
						piggy->End_Piggyback(&Locomotion);
					}
				}
			}
		}
	}

	BASECLASS::Assign_Destination(target, immediate);
}

// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y4): InfantryClass::Per_Cell_Process (PCPType), unmatched in
// ra2-decomp. TibSun's text (TibSun archive code/infantry.cpp; same licence notice as above);
// native changes are marked NATIVE. Behaviour code for the native build only; gamemd.exe's
// version is not compared yet.
// ---------------------------------------------------------------------------------------------
void InfantryClass::Per_Cell_Process(PCPType why)
{
	BStart(BENCH_PCP);
	CellClass * cellptr = &Map[Get_Coord()];

	if (why == PCP_END) {

		/*
		**	If the infantry unit is entering a cell that contains the building it is trying to
		**	capture, then capture it.
		*/
		if (Mission == MISSION_CAPTURE || Mission == MISSION_GUARD_AREA || Mission == MISSION_PATROL) {
			if (NavCom != NULL && NavCom->Is_Techno() && ((TechnoClass*)NavCom)->Considered_Vehicle()) {
				TechnoClass * tech = (TechnoClass *)NavCom;
				if (tech != NULL) {
					if (PositionCell == tech->PositionCell) {
						if (tech->Tag) {
							tech->Tag->Spring(TEVENT_PLAYER_ENTERED, this);
						}
						tech->Detach_All(false);
						tech->Captured(House);
						tech->EnteredByInfType = Class->HeapID;
						tech->Scatter_Incoming_Infantry();
						if (Tag && Tag->Is_To_Inherit()) {
							tech->Attach_Tag(Tag);
						}
						if (Tag) {
							Tag->Spring(TEVENT_DESTROYED_ANY, this);
						}
						Delete_Me();
						BEnd(BENCH_PCP);
						return;
					} else {
						Coord destination = Locomotion->Destination();
						if (destination == COORD_NONE || PositionCell == Cell(destination)) {
							Assign_Destination(NavCom);
							BEnd(BENCH_PCP);
							return;
						}
					}
				}
			}

			TechnoClass * tech = cellptr->Cell_Building();

			if (tech != NULL && (tech == NavCom || tech == TarCom)) {
				if (tech->Tag) {
					tech->Tag->Spring(TEVENT_PLAYER_ENTERED, this);
				}

				if (Class->IsEngineer) {
					// are we trying to repair a bridge?
					if (tech->RTTI == RTTI_BUILDING && ((BuildingClass*)tech)->Class->IsBridgeRepairHut) {
						if (House->Is_Player_Control()) Speak(VOX_BRIDGE_REPAIRED);

						bool train = false;
						for (int y = -2; y < 3; y++) {
							for (int x = -2; x < 3; x++) {
								Cell cell = (Cell)Cell(PositionCell + Cell(x, y));
								IsometricTileType ittype = Map[cell].ITType;
								if (ittype >= IsometricTileTypeClass::TrainBridgeSet && ittype < IsometricTileTypeClass::TrainBridgeSet + TRAIN_BRIDGE_COUNT) {
									train = true;
								}
							}
						}
						if (train) {
							Map.Repair_Train_Bridge(PositionCell);
						} else {
							Map.Repair_Bridge(PositionCell);
						}
						for (int i = Infantry.Count() - 1; i >= 0; i--) {
							Infantry[i]->Detach(tech, false);
						}
						tech->Scatter_Incoming_Infantry();
					}

					/*
					**	An engineer will either mega-repair a friendly or allied
					**	building or it will damage/capture an enemy building. Whether
					**	it damages or captures depends on how badly damaged the
					**	enemy building is.
					*/
					else if (House->Is_Ally(tech)) {
						tech->Renovate();
					} else {
						bool iscapturable = false;
						if (tech->RTTI == RTTI_BUILDING) {
							iscapturable = ((BuildingClass *)tech)->Class->IsCaptureable;
						}

						if (Session.Type != GAME_NORMAL && Session.Options.CrapEngineers && tech->HealthRatio > Rule->ConditionRed) {
							int maxdamage = tech->Strength - int(tech->TClass->MaxStrength * Rule->ConditionRed / 2);
							int damage = MIN((tech->TClass->MaxStrength) * ((1 - Rule->ConditionRed / 2) / 2), maxdamage);
							tech->Take_Damage(damage, 0, Rule->C4Warhead, this, true);
						} else if (iscapturable) {
							if (tech->Tag) {
								tech->Tag->Spring(TEVENT_PLAYER_ENTERED, this);
							}
							tech->House->IsThieved = true;
							if (Tag && Tag->Is_To_Inherit()) {
								tech->Attach_Tag(Tag);
							}
							tech->Captured(House);
							tech->EnteredByInfType = Class->HeapID;
							tech->Scatter_Incoming_Infantry();
						}
					}
				} else {
					if (Class->IsAgent) {
						if (House->Is_Player_Control()) Speak(VOX_BUILDING_INFILTRATED);
						((BuildingClass *)tech)->Spied_By(House);
					}
				}
				if (Tag) {
					Tag->Spring(TEVENT_DESTROYED_ANY, this);
				}
				Delete_Me();
				BEnd(BENCH_PCP);
				return;

			} else {
				if (NavCom == NULL) {
					if (CurrentMission != MISSION_GUARD_AREA) {
						Enter_Idle_Mode();
					}
					if (Map[Get_Coord()].Cell_Building()) {
						Scatter(COORD_NONE, true);
					}
				}
			}
		}

		/*
		**	Infantry entering a transport vehicle will break radio contact
		**	at attach itself to the transporter.
		*/
		TechnoClass * techno;
		if (Class->IsVehicleThief) {
			techno = Get_Cell_Ptr()->Cell_Unit(IsOnBridge);
			if (techno == NULL) techno = Get_Cell_Ptr()->Cell_Aircraft(IsOnBridge);
			if (techno == NULL) techno = Get_Cell_Ptr()->Cell_Building();
		} else {
			techno = Get_Cell_Ptr()->Cell_Building();
			if (techno == NULL) techno = Get_Cell_Ptr()->Cell_Aircraft(IsOnBridge);
			if (techno == NULL) techno = Get_Cell_Ptr()->Cell_Unit(IsOnBridge);
		}

		TechnoClass * on_target_cell = NULL;
		if (NavCom != NULL && NavCom->RTTI == RTTI_CELL) {
			CellClass * cellptr = &Map[NavCom->Center_Coord()];
			if (Class->IsVehicleThief) {
				on_target_cell = cellptr->Cell_Unit(true);
				if (on_target_cell == NULL) on_target_cell = cellptr->Cell_Aircraft(true);
				if (on_target_cell == NULL) on_target_cell = cellptr->Cell_Building();
			} else {
				on_target_cell = cellptr->Cell_Building();
				if (on_target_cell == NULL) on_target_cell = cellptr->Cell_Aircraft(true);
				if (on_target_cell == NULL) on_target_cell = cellptr->Cell_Unit(true);
			}
		}

		if (Mission == MISSION_ENTER && techno != NULL && (NavCom == techno || TarCom == techno || on_target_cell != NULL && techno == on_target_cell)) {
			if (techno->Tag) {
				techno->Tag->Spring(TEVENT_PLAYER_ENTERED, this);
			}
			if (techno->RTTI == RTTI_BUILDING) {
				if (techno == Get_Cell_Ptr()->Cell_Building() && Transmit_Message(RADIO_IM_IN) == RADIO_ROGER) {
					Limbo();
					Transmit_Message(RADIO_HELLO, techno);
					techno->Cargo.Attach(this);
					NavCom = NULL;
					BEnd(BENCH_PCP);
					return;
				}
			} else {
				if (Get_Cell() == techno->Get_Cell() && techno == NavCom) {
					if (Transmit_Message(RADIO_CAN_LOAD, techno) == RADIO_ROGER) {
						ArchiveTarget = NULL;
						Limbo();
						techno->Cargo.Attach(this);
						Hidden();
					} else {
						Assign_Destination(NULL);
						Assign_Mission(MISSION_GUARD);
						Scatter(COORD_NONE, true, true);
					}
					BEnd(BENCH_PCP);
					return;
				}
			}
		}

		/*
		**	If the infantry unit is entering a cell that contains the building it is trying to
		**	sabotage, then sabotage it.
		*/
		if (Mission == MISSION_SABOTAGE) {
			BuildingClass * building = cellptr->Cell_Building();
			if (building != NULL && building == NavCom) {
				if (building->Tag) {
					building->Tag->Spring(TEVENT_PLAYER_ENTERED, this);
				}
				if (building->Mission != MISSION_DECONSTRUCTION) {
					building->IsGoingToBlow = true;
					building->Clicked_As_Target((Rule->C4Delay * TICKS_PER_MINUTE) / 2);
					building->CountDown = Rule->C4Delay * TICKS_PER_MINUTE;
					building->WhomToRepay = this;
				}
				NavCom = NULL;
				Do_Uncloak();
				Arm = Rearm_Delay(true);
				Scatter(building->Center_Coord(), true, true);	// RUN AWAY!
				BEnd(BENCH_PCP);
				return;
			} else {
				if (&Map[Center_Coord()] == NavCom) {
					Explosion_Damage(PositionCoord, Rule->BridgeStrength, this, Rule->C4Warhead);

					Stop_Driver();
					Scatter(Adjacent_Cell(PositionCoord, (FacingType)PrimaryFacing.Current().As_Dir8()), true, true);
					Assign_Mission(MISSION_MOVE);

					if (NavCom == NULL || Map[NavCom->Center_Coord()].Land_Type() == LAND_WATER) {
						Mark(MARK_DOWN);		// Needed only so that Tanya will get destroyed by the explosion.
					}
					Explosion_Damage(PositionCoord, Rule->BridgeStrength, NULL, Rule->C4Warhead);
					Explosion_Damage(PositionCoord, Rule->BridgeStrength, NULL, Rule->C4Warhead);
					if (!IsActive) {
						BEnd(BENCH_PCP);
						return;
					}

					Mark(MARK_DOWN);
				}
			}
		}

		/*
		**	If this unit is on a teather, then cut it at this time so that
		**	the "parent" unit is free to proceed. Note that the parent
		**	unit might actually be a building.
		*/
		if (IsTethered) {
			Transmit_Message(RADIO_UNLOADED);

			/*
			**	Special voice play.
			*/
			if (Class->VoiceComment.Count() > 1) {
				Sound_Effect((VocType)Class->VoiceComment[1], PositionCoord);
			}

			/*
			**	If the cell is now full of infantry, tell them all to scatter
			**	in order to make room for more.
			*/
			if (IsOnBridge) {
				if ((cellptr->BridgeFlag.Composite & 0x01C) == 0x01C) {
					cellptr->Incoming(COORD_NONE, true, true, true);
				}
			} else {
				if ((cellptr->Flag.Composite & 0x01C) == 0x01C) {
					cellptr->Incoming(COORD_NONE, true, true, false);
				}
			}
		}

		/*
		**	When the infantry reaches the center of the cell, it may begin a new mission.
		*/
		if (MissionQueue == MISSION_NONE && NavCom == NULL && TarCom == NULL && !In_Radio_Contact()) {
			Enter_Idle_Mode();
		}
		Commence();

		if (IsPlanningToLook) {
			IsPlanningToLook = false;
			Look(false);
		} else {
			Look(true);
		}

		/*
		**	If after all is said and done, the unit finishes its move on an impassable cell, then
		**	it must presume that it is in the case of a unit driving onto a bridge that blows up
		**	before the unit completes it's move. In such a case the unit should have been destroyed
		**	anyway, so blow it up now.
		*/
		CellClass * cellptr = &Map[Get_Coord()];
		LandType land = cellptr->Land_Type();
		if (!Locomotion->Is_Moving() && !Class->IsBomber && !Has_Ability(ABILITY_C4) && (land == LAND_ROCK || land == LAND_WATER) && (!IsOnBridge || !cellptr->IsUnderBridge)) {
			int damage = Strength;
			Take_Damage(damage, 0, Rule->C4Warhead, NULL, true);
			BEnd(BENCH_PCP);
			return;
		}

		if (!Class->IsTiberiumProof && !Has_Ability(ABILITY_TIBERIUM_PROOF) && Strength > 0) {
			TiberiumType tiberium = Map[Get_Coord()].Tiberium_Type_Here();
			if (tiberium != TIBERIUM_NONE) {
				int damage = Tiberiums[tiberium]->Power / 10;
				if (damage <= 1) damage = 1;
				Coord coord = Get_Coord();
				if (Take_Damage(damage, 0, Rule->C4Warhead, NULL, true) == RESULT_DESTROYED) {
					if (Scen->IsTiberiumDeathToVisceroid) {
						UnitClass * visceroid = new UnitClass(Rule->SmallVisceroid, House_From_HousesType(HouseTypeClass::From_Name("Neutral")));
						if (visceroid) {
							Coord cell_coord = Map[coord].Cell_Coord();
							if ((Map[cell_coord].Flag.Composite & 0x020) == 0) {
								ScenarioInit++;
								visceroid->Unlimbo(coord);
								ScenarioInit--;
							}
						}
					}
				}
			}
		}
	}

	if (IsActive) {
		BASECLASS::Per_Cell_Process(why);
	}
	BEnd(BENCH_PCP);
}
