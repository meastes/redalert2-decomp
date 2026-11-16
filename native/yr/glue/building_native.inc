
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y4), appended to the building unit: BuildingClass::Exit_Object
// (TechnoClass *, Cell), unmatched in ra2-decomp. TibSun's text of Exit_Object(TechnoClass *)
// (TibSun archive code/building.cpp; SPDX GPL-3.0-or-later, Copyright 2025 Electronic Arts Inc.
// and OpenTS contributors, EA's GPL section 7 terms; THIRD_PARTY.md); native changes are marked
// NATIVE. YR's second parameter (a preferred cell) goes to Find_Exit_Cell for vehicles only (lane Y6,
// compared with gamemd.exe 0x00443C60: infantry and the other factory get CELL_NONE). Behaviour code
// for the native build only; otherwise gamemd.exe's version is not compared.
// ---------------------------------------------------------------------------------------------
int BuildingClass::Exit_Object(TechnoClass * base, Cell preferred)	// NATIVE: YR's preferred cell passed on to Find_Exit_Cell for vehicles
{
	if (!base) return(0);

	/*
	**	A unit exiting a building is always considered to be "locked". That means, it
	**	will be considered as to have legally entered the visible map domain.
	*/
	base->IsInPlayfield /* NATIVE: YR's name of IsLocked */ = true;

	/*
	**	Find a good cell to unload the object to. The object, probably a vehicle
	**	will drive/walk to the adjacent free cell.
	*/
	Cell cell;

	switch (base->Fetch_RTTI()) {

		case RTTI_AIRCRAFT:

			House->Update_Production_Mode(RTTI_AIRCRAFT);
			House->BuildAircraft = AIRCRAFT_NONE;

			if (!In_Radio_Contact() || IonStormClass::Is_Ion_Storm_Active()) {
				AircraftClass * air = (AircraftClass *)base;

				air->Set_Height(0);
				ScenarioInit++;

				if (IonStormClass::Is_Ion_Storm_Active()) {
					Cell nearby = air->Nearby_Location(this);
					if (air->Unlimbo(Coord(nearby, 0), air->Pose_Dir())) {
						ScenarioInit--;
						return(2);
					}
				} else {
					if (air->Unlimbo(Docking_Coord(air), air->Pose_Dir())) {
						Transmit_Message(RADIO_HELLO, air);
						Transmit_Message(RADIO_TETHER);

						if (ArchiveTarget != NULL) {
							air->Assign_Destination(ArchiveTarget);
							air->Assign_Mission(MISSION_MOVE);
						}
						ScenarioInit--;
						return(2);
					}
				}
				ScenarioInit--;

			} else {

				/*
				 * The aircraft has a pad to land on, so spawn it at the edge of the
				 * visible map and let it fly in.
				 */
				int x = Map.LocalRect.X;
				cell = Cell(x, -x) + Cell(1, Map.PlayRect.Width) + Cell(Map.LocalRect.Y, Map.LocalRect.Y);

				Cell diff;
				diff = Center_Coord().As_Cell() - cell;

				if (diff.X - diff.Y > Map.LocalRect.Width) {
					cell = Cell(cell.X - 1, cell.Y) + Cell(Map.LocalRect.Width, -Map.LocalRect.Width);
				} else {
					cell = Cell(cell.X - 1, cell.Y);
				}

				Cell spawncell = cell;
				int random = Random_Pick(0, Map.LocalRect.Height);
				spawncell = spawncell + Cell(random, random);

				ScenarioInit++;
				if (base->Unlimbo(Coord(spawncell, 0), DIR_N)) {
					if (ArchiveTarget != NULL) {
						base->Assign_Destination(ArchiveTarget);
						base->Assign_Mission(MISSION_MOVE);
					} else {
						cell = base->Nearby_Location(this);
						if (cell != CELL_NONE) {
							base->Assign_Destination(&Map[cell]);
							base->Assign_Mission(MISSION_MOVE);
						} else {
							base->Assign_Destination(NULL);
							base->Assign_Mission(MISSION_MOVE);
						}
					}
					ScenarioInit--;
					return(2);
				}
				ScenarioInit--;
			}
			break;

		case RTTI_INFANTRY:
		case RTTI_UNIT:

			if (!Class->IsHospital) {

				/*
				 * If a unit is already exiting and this building doesn't allow more than
				 * one exit at a time, then refuse the exit attempt for now.
				 */
				if (!Class->IsArmory && !Class->IsWeaponsFactory && In_Radio_Contact()) {
					return(1);
				}
			}

			if (!Class->IsHospital && !Class->IsArmory) {
				House->Update_Production_Mode(base->Fetch_RTTI());

				if (base->Fetch_RTTI() == RTTI_UNIT) {
					House->BuildUnit = UNIT_NONE;
				}
				if (base->Fetch_RTTI() == RTTI_INFANTRY) {
					House->BuildInfantry = INFANTRY_NONE;
				}
			}

			if (!Class->IsRefinery && !Class->IsWeeder) {

				if (Class->IsWeaponsFactory) {

					base->ArchiveTarget = ArchiveTarget;

					if (Mission == MISSION_UNLOAD) {
						for (int index = 0; index < Buildings.Count(); index++) {
							BuildingClass * bldg = Buildings[index];
							if (bldg->House == House && bldg->Class == Class && bldg != this && bldg->Mission == MISSION_GUARD && !bldg->Factory) {
								FactoryClass * temp = Factory;
								bldg->Factory = Factory;
								Factory = NULL;
								int retval = (bldg->Exit_Object(base, CELL_NONE));	// gamemd.exe 0x00444542: no preferred cell for the other factory
								bldg->Factory = NULL;
								Factory = temp;
								return(retval);
							}
						}
						return(1);	// fail while we're still unloading previous
					}
					ScenarioInit++;
					if (base->Unlimbo(Exit_Coord(0), DIR_E)) {
						base->Mark(MARK_UP);
						base->Set_Coord(Exit_Coord(0));
						base->Mark(MARK_DOWN);
						Transmit_Message(RADIO_HELLO, base);
						Transmit_Message(RADIO_TETHER);
						Assign_Mission(MISSION_UNLOAD);
						ScenarioInit--;
						return(2);
					}

				} else if (Class->ToBuild != RTTI_INFANTRYTYPE && !Class->IsHospital && !Class->IsArmory) {

					Cell exitcell = Find_Exit_Cell(base, preferred);
					if (exitcell == CELL_NONE) {
						return(0);
					}

					Dir256 dir = ::Direction(Center_Coord(), Coord(exitcell)).As_Dir256();

					Cell to = exitcell;
					cell = Get_Cell();
					int x = cell.X;
					if (to.X >= x + Class->Width()) {
						to.X -= 1;
					} else if (to.X < cell.X) {
						to.X += 1;
					}
					int y = cell.Y;
					if (to.Y >= y + Class->Height()) {
						to.Y -= 1;
					} else if (to.Y < cell.Y) {
						to.Y += 1;
					}

					/*
					 * The barracks types have a special exit door location that the
					 * exiting object should be placed at.
					 */
					Coord exitcoord = Coord(to, 0);
					if (Class->IsGDIBarracks) {
						if (exitcell.X == x + 1 && exitcell.Y == y + 2) {
							exitcoord = exitcoord + Class->ExitCoordinate;
						}
					}
					if (Class->IsNODBarracks && exitcell.X == x + 2 && exitcell.Y == y + 2) {
						exitcoord = exitcoord + Class->ExitCoordinate;
					}

					ScenarioInit++;
					if (base->Unlimbo(exitcoord, dir)) {

						base->Assign_Mission(MISSION_MOVE);
						base->Assign_Destination(&Map[exitcell]);

						/*
						 * When the computer produces a unit, it has the unit guard an
						 * area around the spot it should head to.
						 */
						if (!House->Is_Human_Player()) {
							base->Assign_Mission(MISSION_GUARD_AREA);

							cell = House->Where_To_Go((FootClass *)base);
							base->Assign_Archive_Target((cell != CELL_NONE && Class->ToBuild != RTTI_NONE) ? &Map[cell] : NULL);
						}

						ScenarioInit--;
						return(2);
					}

				} else {

					base->ArchiveTarget = ArchiveTarget;
					Coord exitcoord;

					Cell exitcell = Find_Exit_Cell(base, CELL_NONE);	// gamemd.exe 0x004449AF: infantry ignore the preferred cell
					if (exitcell == CELL_NONE) {
						return(0);
					}

					Coord exitcellcoord(exitcell);
					Dir256 dir = ::Direction(Center_Coord(), exitcellcoord).As_Dir256();

					Cell to = exitcell;
					cell = Get_Cell();
					int x = cell.X;
					if (to.X >= x + Class->Width()) {
						to.X -= 1;
					} else if (to.X < cell.X) {
						to.X += 1;
					}
					int y = cell.Y;
					if (to.Y >= y + Class->Height()) {
						to.Y -= 1;
					} else if (to.Y < cell.Y) {
						to.Y += 1;
					}

					/*
					 * The barracks types have a special exit door location that the
					 * exiting object should be placed at.
					 */
					exitcoord = Coord(to, 0);
					if (Class->IsGDIBarracks) {
						if (exitcell.X == x + 1 && exitcell.Y == y + 2) {
							exitcoord = exitcoord + Class->ExitCoordinate;
						}
					}
					if (Class->IsNODBarracks && exitcell.X == x + 2 && exitcell.Y == y + 2) {
						exitcoord = exitcoord + Class->ExitCoordinate;
					}

					ScenarioInit++;
					if (base->Unlimbo(exitcoord, dir)) {

						if (((FootClass *)base)->NavCom != NULL) {
							base->ArchiveTarget = ((FootClass *)base)->NavCom;
						}

						base->Assign_Mission(MISSION_MOVE);
						base->Assign_Destination(&Map[exitcell]);

						/*
						 * When the computer produces infantry, it has the infantry guard
						 * an area around the spot it should head to.
						 */
						if (!House->Is_Human_Player()) {
							base->Assign_Mission(MISSION_GUARD_AREA);

							cell = House->Where_To_Go((FootClass *)base);
							if (cell != CELL_NONE && Class->ToBuild != RTTI_NONE) {
								base->ArchiveTarget = &Map[cell];
								((FootClass *)base)->Queue_Navigation_List(&Map[cell]);
							} else {
								base->Assign_Archive_Target(NULL);
							}
						}

						/*
						**	Establish radio contact so unload coordination can occur. This
						**	radio contact should always succeed.
						*/
						if (Transmit_Message(RADIO_HELLO, base) == RADIO_ROGER) {
							Transmit_Message(RADIO_UNLOAD);
						}
						ScenarioInit--;
						return(2);
					}
				}

			} else {

				/*
				 * A unit exits a refinery to the south west corner and heads off to
				 * harvest. Infantry just scatters out.
				 */
				if (base->Fetch_RTTI() == RTTI_UNIT) {

					cell = Adjacent_Cell(Center_Coord().As_Cell(), FACING_SW);

					ScenarioInit++;
					if (base->Unlimbo(Coord(Adjacent_Cell(cell, FACING_S), 0), DIR_SW)) {
						base->PrimaryFacing.Set(DirType(DIR_S));
						base->Assign_Mission(MISSION_HARVEST);
					}

				} else {
					base->Scatter(COORD_NONE, true, false);
					return(0);
				}
			}
			ScenarioInit--;
			break;

		case RTTI_BUILDING:

			if (!House->Is_Human_Player()) {

				House->Update_Production_Mode(RTTI_BUILDING);
				House->BuildStructure = STRUCT_NONE;

				/*
				**	Find the next available spot to place this newly created building. If the
				**	building could be placed at the desired location, fine. If not, then this
				**	routine will return failure. The calling routine will probably abandon this
				**	building in preference to building another.
				*/
				BuildingClass * building = (BuildingClass *)base;
				BaseNodeClass * node = House->Base.Next_Buildable(building->Class->HeapID);
				Coord placementcoord(0, 0, 0);
				BuildingTypeClass * buildingtype = building->Class;

				if (node != NULL && node->CellID != Cell(0, 0)) {

					if (buildingtype->PowersUpBuilding[0] != '\0' || House->Can_Build_Here(buildingtype, node->CellID)) {
						placementcoord = Coord(node->CellID);
					} else {

						/*
						**	Find a suitable new spot to place.
						*/
						placementcoord = Coord(House->Where_To_Place_Building(buildingtype, HouseClass::Base_Cell_Weight_By_Distance, -1));
						if (placementcoord == COORD_NONE) {
							return(0);
						}
						node->CellID = placementcoord.As_Cell();
					}

				} else {

					if (buildingtype->PowersUpBuilding[0] == '\0') {
						placementcoord = Coord(House->Where_To_Place_Building(buildingtype, HouseClass::Base_Cell_Weight_By_Distance, -1));
					} else {
						cell = House->Where_To_Place_Upgrade(buildingtype);
						if (cell != CELL_NONE) {
							placementcoord = Coord(cell);
						}
					}

					if (node != NULL && placementcoord != COORD_NONE) {
						node->CellID = placementcoord.As_Cell();
					}
				}

				if (placementcoord != COORD_NONE) {

					Cell cellnum = placementcoord.As_Cell();
					switch (((BuildingTypeClass *)base->Class_Of())->Flush_For_Placement(cellnum, House)) {

						default:
							return(0);

						case 1:
							return(1);

						case 0:
							if (base->Unlimbo(placementcoord)) {
								if (node != NULL) {
									if (building->Class->HeapID == House->BuildStructure) {
										House->BuildStructure = STRUCT_NONE;
									}
								}

								if (base->CurrentMission == MISSION_NONE && base->MissionQueue == MISSION_CONSTRUCTION) {
									base->Commence();
								}

								/*
								 * When a wall tower is placed, the next base defense node is
								 * moved to the spot the tower actually appeared at.
								 */
								if (building->Class == Rule->WallTower) {
									int index = House->Base.Nodes.ID(node) + 1;
									int count = House->Base.Nodes.Count();
									for (; index < count; index++) {
										if (House->Base.Nodes[index].Type >= STRUCT_FIRST && BuildingTypes[House->Base.Nodes[index].Type]->IsBaseDefense) {
											House->Base.Nodes[index].CellID = base->Get_Coord().As_Cell();
											break;
										}
									}
								}
								return(2);
							}
							// fall through

						case 2:
							if (node == NULL) {
								return(0);
							} else {
								int index = House->Base.Nodes.ID(node);

								BuildingTypeClass * nodetype = BuildingTypes[node->Type];
								if (!nodetype->IsWall && !nodetype->IsGate) {
									Cell placecell = placementcoord.As_Cell();
									for (int i = 0; i < House->Base.Nodes.Count(); i++) {
										if (House->Base.Nodes[i].CellID == placecell) {
											House->Base.Nodes[i].CellID = Cell(0, 0);
										}
									}
									return(0);
								}
								House->Base.Nodes.Delete_Index(index);
							}
							break;
					}

				} else {

					if (buildingtype->PowersUpBuilding[0] != '\0') {
						if (House->Base.Next_Buildable(STRUCT_NONE) == node) {
							House->Base.Nodes.Delete_Index(House->Base.Next_Buildable_Index(STRUCT_NONE));
						}
					}
				}
			}
			break;

		default:
			break;
	}

	/*
	**	Failure to exit the object results in a false return value.
	*/
	return(0);
}

// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y4): BuildingClass::Do_MISSION_DECONSTRUCTION, unmatched in
// ra2-decomp. TibSun's text (TibSun archive code/building.cpp; same licence notice as above);
// native changes are marked NATIVE. Behaviour code for the native build only. The survivors part is
// compared with gamemd.exe 0x00449C30 (lane Y6: who leaves survivors, absorbed passengers and
// occupants let out); the rest (MCV repacking) is not compared yet.
// ---------------------------------------------------------------------------------------------
int BuildingClass::Do_MISSION_DECONSTRUCTION(void)
{
	int i;

	/*
	**	Always force repair off.
	*/
	Repair(0);

	bool selected = IsSelected;

	enum {
		INITIAL,
		HOLDING,
		DURING
	};
	switch (Status) {
		case INITIAL:

			if (Can_Be_Undeployed() && Class->IsVehicle() /* NATIVE: YR's mobile deployer test */) {
				Assign_Target(NULL);
			}

			/*
			**	Special check for the repair bay which has the ability to sell
			**	whatever is on it. If there is something on the repair bay, then
			**	it will be sold. If there is nothing on the repair bay, then
			**	the repair bay itself will be sold.
			*/
			if (Class->IsCanUnitRepair && Transmit_Message(RADIO_NEED_TO_MOVE) == RADIO_ROGER && ::Distance(Center_Coord(), Contact_With_Whom()->Center_Coord()) < CELL_LEPTON / 2) {
				TechnoClass * tech = Contact_With_Whom();
				Transmit_Message(RADIO_OVER_OUT);
				if (IsOwnedByPlayer) Speak(VOX_UNIT_SOLD);
				tech->Sell_Back(1);
				Assign_Mission(MISSION_GUARD);
				return(1);
			}

			if (UpgradeLevel) {
				const BuildingTypeClass *upgrade = Upgrades[UpgradeLevel - 1];
				BuildingClass *uptr = (BuildingClass *)upgrade->Create_One_Of(House);
				House->Refund_Money(uptr->Refund_Amount());
				delete uptr;
				Remove_Upgrade();
				House->RecalcPower = true;
				House->RecalcRadar = true;
				House->IsRecalcNeeded = true;	// NATIVE: YR rechecks power through IsRecalcNeeded (TibSun: Adjust_House_Power)
				if (IsOwnedByPlayer) Speak(VOX_STRUCTURE_SOLD);
				Assign_Mission(MISSION_GUARD);
				return(1);
			}

			/*
			 * A juggernaut must rotate back to its starting facing before the
			 * deconstruction can proceed.
			 */
			// NATIVE: TibSun's juggernaut facing wait left out (YR has no juggernaut).

			DestroyNthAnim(BANIM_ALL);	// NATIVE: YR's name of End_Anim

			IsReadyToCommence = false;
			Transmit_Message(RADIO_RUN_AWAY);

			if (Class->IsLaserFencePost) {
				Toggle_Laser_Fence_Post(false);
			}

			// NATIVE: TibSun's juggernaut facing wait left out (YR has no juggernaut).

			if (Class->AuxSound2 != VOC_NONE) {
				Sound_Effect(Class->AuxSound2, PositionCoord);
			}

			Status = HOLDING;
			break;

		case HOLDING:
			Transmit_Message(RADIO_OVER_OUT);
			if (!IsTethered) {

				/*
				**	The crew will evacuate from the building. The number of crew
				**	members leaving is equal to the unrecovered cost of the building
				**	divided by 100 (the typical cost of a minigunner infantryman).
				*/
				if (ArchiveTarget == NULL || Class->UndeploysInto == NULL) {	// gamemd.exe 0x0044A309 (TibSun: &&): a sold construction yard leaves survivors
					int count = How_Many_Survivors();
					bool engineer = false;

					const Cell * list = Occupy_List();
					const Cell * occupy = list;
					int num_cells = 0;
					while (*occupy != REFRESH_EOL) {
						num_cells++;
						occupy++;
					}

					/*
					 * YR (gamemd.exe 0x0044A37F-0x0044A59E): a building that absorbs units or
					 * infantry lets them out, one per foundation cell, facing as the building.
					 */
					Cell here = Get_Cell();
					if (Class->UnitAbsorb || Class->InfantryAbsorb) {
						const Cell * spot = list;
						FootClass * passenger;
						while (Cargo.How_Many() > 0 && (passenger = Cargo.Detach_Object()) != NULL) {
							Cell exitcell = here + *spot++;
							if (*spot == REFRESH_EOL) spot = list;	// NATIVE: the original walks on past the list's end
							Coord coord(exitcell.X * CELL_LEPTON_W + CELL_LEPTON_W / 2, exitcell.Y * CELL_LEPTON_H + CELL_LEPTON_H / 2 + 36, 0);
							if (passenger->RTTI == RTTI_UNIT) {
								coord = Map[coord].Center_Coord();
							} else {
								coord = Map[coord].Closest_Free_Spot(coord, false, false, false);
							}
							ScenarioInit++;
							passenger->IsOnBridge = IsOnBridge;
							coord.Z = Get_Cell_Ptr()->Center_Coord().Z;
							if (!IsSurvivorless && passenger->Unlimbo(coord, PrimaryFacing.Current().As_Dir256())) {
								if (passenger->RTTI == RTTI_INFANTRY) {
									InfantryClass * inf = (InfantryClass *)passenger;
									if (!inf->CountedAsOwnedSpecial) {
										inf->House->CurInfantry++;
										inf->CountedAsOwnedSpecial = true;
									}
									inf->Absorbed = false;
								}
								passenger->Scatter(COORD_NONE, true, false);
								if (!House->Is_Human_Player()) {
									passenger->QueueMission(MISSION_HUNT, false);
								}
							} else {
								passenger->Limbo();
								delete passenger;
							}
							ScenarioInit--;
						}
					}

					/*
					 * YR: a garrisoned building lets its occupants out.
					 */
					if (GetOccupantCount() > 0) {
						FUN_00457DE0(false, true);
					}

					while (count) {

						/*
						**	Ensure that the player only gets ONE engineer and not from a captured
						**	construction yard.
						*/
						const InfantryTypeClass * typ = Crew_Type();
						while (typ->IsEngineer && engineer) {
							typ = Crew_Type();
						}
						if (typ->IsEngineer) engineer = true;

						InfantryClass * infantry = NULL;
						if (typ != NULL) infantry = new InfantryClass(typ, House);
						if (infantry != NULL) {
							ScenarioInit++;

							/*
							 * The exit point is biased 36 leptons south of the cell center.
							 */
							Cell newcell = PositionCell + list[Scen->RandomNumber(0, num_cells - 1)];
							Coord coord = Coord(newcell) + Coord(0, 36, 0);
							coord = Map[coord].Closest_Free_Spot(coord, false);

							if (coord != COORD_NONE && infantry->Unlimbo(coord, DIR_N)) {
								if (infantry->Class->IsNominal) infantry->IsTechnician = true;
								ScenarioInit--;
								infantry->Scatter(COORD_NONE, true);
								ScenarioInit++;
								infantry->Assign_Mission(MISSION_MOVE);
							} else {
								delete infantry;
							}
							ScenarioInit--;
						}
						count--;
					}
				}

				if (House->Is_Player_Control() && !Considered_Vehicle()) {
					Sound_Effect(Rule->SellSound, PositionCoord);
				}
				Status = DURING;
				Begin_Mode(BSTATE_CONSTRUCTION);
				//Detach_All(true);
				//Transmit_Message(RADIO_OVER_OUT);
				IsReadyToCommence = false;

				if (selected) {
					Select();
				}
				break;
			}
			//Transmit_Message(RADIO_RUN_AWAY);
			break;

		case DURING:
			if (IsReadyToCommence) {
				House->IsRecalcNeeded = true;
				Assign_Target(NULL);
				if (IsOwnedByPlayer && Class->UndeploysInto == NULL) Speak(VOX_STRUCTURE_SOLD);

				/*
				**	Construction yards that deconstruct, really just revert back
				**	to an MCV.
				*/
				if (Class->UndeploysInto != NULL &&
					(Class->IsVehicle() /* NATIVE: YR's mobile deployer test */ ||
					/* NATIVE: TibSun's Can_Always_Undeploy (limpet mines, mobile war factory) has no YR type */
					(Session.Type != GAME_NORMAL && ArchiveTarget != NULL && House->Is_Human_Player() && (Session.Type == GAME_NORMAL || Session.Options.MCVRedeploy)))
				) {

					if (Class->IsArtillary && (BarrelPitch.Current() != Class->StartPitch || PrimaryFacing.Current() != Class->StartFace)) {
						BarrelPitch.Set_Desired(Class->StartPitch);
						PrimaryFacing.Set_Desired(Class->StartFace);
						return(1);
					}

					ScenarioInit++;
					UnitClass * unit = new UnitClass(Class->UndeploysInto, House);
					ScenarioInit--;
					if (unit != NULL) {

						/*
						**	Unlimbo the MCV onto the map. The MCV should start in the same
						**	health condition that the construction yard was in.
						*/
						double ratio = HealthRatio;
						int money = Refund_Amount();
						AbstractClass * arch = ArchiveTarget;

						Coord place;
						if (Class->IsVehicle() /* NATIVE: YR's mobile deployer test */) {
							place = PositionCoord;
						} else {
							Coord adjacent = Adjacent_Cell(PositionCoord, DIR_SE);
							place = Coord_Snap(adjacent);
						}

						if (LightSource != NULL) {
							LightSource->Disable();
						}

						DynamicVectorClass<TechnoClass *> targetters;

						/*
						 * Collect everything that was targeting this building so that it
						 * can be retargeted onto the undeployed unit. Engineers that were
						 * trying to capture the building just lose their target.
						 */
						for (i = 0; i < Technos.Count(); i++) {
							TechnoClass * tptr = Technos[i];
							if (tptr->TarCom != NULL && tptr->TarCom->RTTI == RTTI_BUILDING && tptr->TarCom == this && tptr->IsActive && tptr != this && tptr != unit) {
								if (tptr->RTTI == RTTI_INFANTRY && ((InfantryTypeClass *)tptr->TClass)->IsEngineer) {
									tptr->Assign_Target(NULL);
								} else {
									targetters.Add(tptr);
								}
							}
						}

						Limbo();
						Dir256 dir = Class->Deploy_Facing();
						if (unit->Unlimbo(place, dir)) {
							unit->Strength = (int)(unit->Class_Of()->MaxStrength * ratio);
							unit->Strength = max(unit->Strength, 1);
							unit->Group = Group;
							unit->Veterancy.Experience = Veterancy.Experience;

							if (Class->IsArtillary) {	// NATIVE: YR has no juggernaut
								unit->BarrelPitch.Set(Class->StartPitch);
							}

							/*
							**	Lift the move destination from the building and assign
							**	it to the unit.
							*/
							if (arch != NULL) {
								unit->Assign_Destination(arch);
								unit->Assign_Mission(MISSION_MOVE);
							}

							if (Tag != NULL) {
								unit->Attach_Tag(Tag);
								Tag->AttachCount--;
								Tag = NULL;
							}

							if (selected) {
								unit->Select();
							}

							for (i = 0; i < targetters.Count(); i++) {
								targetters[i]->Assign_Target(unit);
							}

						} else {

							/*
							**	If, for some strange reason, the MCV could not be placed on the
							**	map, then give the player some money to compensate.
							*/
							House->Refund_Money(money);
						}
					} else {
						House->Refund_Money(Refund_Amount());
					}
					Detach_All();
					Delete_Me();
					House->IsRecalcNeeded = true;	// NATIVE: YR rechecks power through IsRecalcNeeded (TibSun: Adjust_House_Power)

				} else {

					#if 0
					/*
					**	Selling off a gap generator will cause the cells it affects
					**	to stop being jammed.
					*/
					if (*this == STRUCT_GAP) {
						Remove_Gap_Effect();
					}
					#endif

					/*
					**	A sold building still counts as a kill, but it just isn't directly
					**	attributed to the enemy.
					*/
					WhoLastHurtMe = HOUSE_NONE;
					Record_The_Kill(NULL);

					if (LightSource != NULL) {
						LightSource->Disable();
					}

					/*
					**	The player gets part of the money back for the sell.
					*/
					House->Refund_Money(Refund_Amount());
					//House->Stole(-Refund_Amount());
					Limbo();

					StorageClass *storage = &Storage;
					int i = storage->First_Used_Slot();
					while (i != -1) {
						int amount = storage->Decrease_Amount(storage->Get_Amount(i), i);
						House->Harvested(amount, TiberiumType(i));
						i = storage->First_Used_Slot();
					}

					/*
					**	Finally, delete the building from the game.
					*/
					Delete_Me();

					House->IsRecalcNeeded = true;	// NATIVE: YR rechecks power through IsRecalcNeeded (TibSun: Adjust_House_Power)
					if (Class->IsCloakGenerator) {
						Disable_Cloak_Generator();
						CurrentCloakRadius = 1;
						Cloaking_AI(true);
					}
				}
			}
			break;

		default:
			break;
	}
	return(1);
}
