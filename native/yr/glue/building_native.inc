
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y4), appended to the building unit: BuildingClass::Exit_Object
// (TechnoClass *, Cell), unmatched in ra2-decomp. TibSun's text of Exit_Object(TechnoClass *)
// (TibSun archive code/building.cpp; SPDX GPL-3.0-or-later, Copyright 2025 Electronic Arts Inc.
// and OpenTS contributors, EA's GPL section 7 terms; THIRD_PARTY.md); native changes are marked
// NATIVE. YR's second parameter (a preferred cell) goes to Find_Exit_Cell for vehicles only (lane Y6,
// compared with gamemd.exe 0x00443C60: infantry and the other factory get CELL_NONE). Behaviour code
// for the native build only; otherwise gamemd.exe's version is not compared.
// NATIVE (lane N3): TibSun's Assign_Mission(m) is written QueueMission(m, false), what gamemd.exe calls there
// (vtable slot 0x1E8, MissionClass::QueueMission 0x005B35E0, e.g. 0x004445F0 for the war factory's unload): the
// YR tree's ObjectClass::Assign_Mission is an empty virtual, so the war factory never unloaded and every
// mission given in these drafts was dropped. (The same in all of this file's drafts.)
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
							air->QueueMission(MISSION_MOVE, false);
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
						base->QueueMission(MISSION_MOVE, false);
					} else {
						cell = base->Nearby_Location(this);
						if (cell != CELL_NONE) {
							base->Assign_Destination(&Map[cell]);
							base->QueueMission(MISSION_MOVE, false);
						} else {
							base->Assign_Destination(NULL);
							base->QueueMission(MISSION_MOVE, false);
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
						QueueMission(MISSION_UNLOAD, false);
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

						base->QueueMission(MISSION_MOVE, false);
						base->Assign_Destination(&Map[exitcell]);

						/*
						 * When the computer produces a unit, it has the unit guard an
						 * area around the spot it should head to.
						 */
						if (!House->Is_Human_Player()) {
							base->QueueMission(MISSION_GUARD_AREA, false);

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

						base->QueueMission(MISSION_MOVE, false);
						base->Assign_Destination(&Map[exitcell]);

						/*
						 * When the computer produces infantry, it has the infantry guard
						 * an area around the spot it should head to.
						 */
						if (!House->Is_Human_Player()) {
							base->QueueMission(MISSION_GUARD_AREA, false);

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
						base->QueueMission(MISSION_HARVEST, false);
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
				if (IsOwnedByPlayer) VoxClass::Play("EVA_UnitSold");	// NATIVE (lane Y11b): YR's EVA call for TibSun's Speak
				tech->Sell_Back(1);
				QueueMission(MISSION_GUARD, false);
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
				if (IsOwnedByPlayer) VoxClass::Play("EVA_StructureSold");	// NATIVE (lane Y11b): YR's EVA call for TibSun's Speak
				QueueMission(MISSION_GUARD, false);
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
				VocClass::PlayAt(Class->AuxSound2, PositionCoord);	// NATIVE (lane Y11b): YR's sound call for TibSun's Sound_Effect
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
								infantry->QueueMission(MISSION_MOVE, false);
							} else {
								delete infantry;
							}
							ScenarioInit--;
						}
						count--;
					}
				}

				if (House->Is_Player_Control() && !IsStrange()) {	// YR (lane N4): TibSun's Considered_Vehicle is the YR tree's IsStrange (BuildingClass: Class->IsVehicle(), vtable 0x80); the tree's Considered_Vehicle is another slot (Height > -20), true for every building
					VocClass::PlayAt(Rule->SellSound, PositionCoord);	// NATIVE (lane Y11b): YR's sound call for TibSun's Sound_Effect
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
				if (IsOwnedByPlayer && Class->UndeploysInto == NULL) VoxClass::Play("EVA_StructureSold");	// NATIVE (lane Y11b): YR's EVA call for TibSun's Speak

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
								unit->QueueMission(MISSION_MOVE, false);
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
						int amount = (int)storage->Decrease_Amount(storage->Get_Amount(i), i);
						House->Give_Tiberium((float)amount, i);	// NATIVE (lane Y11b): YR credits it so (gamemd.exe 0x0044A272); TibSun's Harvested is not in the tree
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

// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y11b): BuildingClass::Drop_Debris, gamemd.exe 0x00442D90 (1,651 bytes),
// unmatched in the YR tree (a link-closure trap whenever a building is destroyed). TibSun's text
// (TibSun archive code/building.cpp, BuildingClass::Drop_Debris; attribution as above) with Yuri's
// Revenge's changes read from gamemd.exe's code: a building that absorbs units or infantry (UnitAbsorb,
// InfantryAbsorb: the bio reactor, the tank bunker) first lets out what it holds at its own position,
// facing the building's way (each one steps the foundation list on), scattering and a computer house's set to Hunt (each
// counted as the house's infantry again); a survivor that cannot be placed is deleted; the survivors
// then follow TibSun's rules (a technician from a building with buildup data, an attack on the
// destroyer unless the house can overpower it, else Move for a human house or Hunt); the scorch or crater mark is placed within half a cell of each clear cell's centre.
// Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------
void BuildingClass::Drop_Debris(AbstractClass * source)
{
	Cell cell = Get_Cell();
	Cell const * offset = Occupy_List();
	int odds = (WhomToRepay != NULL) ? 1 : 2;
	if (IsCaptured) {
		odds += 6;
	}
	int count = How_Many_Survivors();

	if (Class->UnitAbsorb || Class->InfantryAbsorb) {
		for (FootClass * foot = Cargo.Detach_Object(); foot != NULL; foot = Cargo.Detach_Object()) {
			offset++;		// gamemd.exe works out a spot in the next foundation cell, then places the unit at the building's own position
			ScenarioInit++;
			foot->IsOnBridge = IsOnBridge;
			Coord coord = Position;
			coord.Z = Get_Cell_Ptr()->Center_Coord().Z;
			bool placed = false;
			if (!IsSurvivorless) {
				DirType facing = PrimaryFacing.Current();
				if (foot->Unlimbo(coord, (Dir256)((((unsigned)facing.Raw >> 7) + 1) >> 1 & 0xFF))) {
					placed = true;
					if (!foot->CountedAsOwnedSpecial) {
						foot->House->CurInfantry++;
						foot->CountedAsOwnedSpecial = true;
					}
					foot->Absorbed = false;
					foot->Scatter(COORD_NONE, true, false);
					if (!House->FUN_0050B730()) {
						foot->QueueMission(MISSION_HUNT, false);
					}
				}
			}
			if (!placed) {
				foot->RegisterDestruction((source != NULL && (source->AbstractFlags & 1)) ? (TechnoClass *)source : NULL);
				foot->Delete_Me();
			}
			ScenarioInit--;
		}
	}

	if (count == 0) {
		return;
	}
	while (*offset != REFRESH_EOL) {
		Cell newcell = cell + *offset++;
		CellClass * cellptr = &Map[newcell];

		if (!House->IsToDie && count > 0 && Scen->RandomNumber(0, odds) == 1) {
			InfantryTypeClass const * type = Crew_Type();
			InfantryClass * infantry = (type != NULL) ? new InfantryClass(type, House) : NULL;
			if (infantry != NULL) {
				if (HasBuildupData && infantry->Class->IsNominal) {
					infantry->IsTechnician = true;
				}
				ScenarioInit++;
				Coord newcoord(newcell.X * CELL_LEPTON_W + CELL_LEPTON_W / 2, newcell.Y * CELL_LEPTON_H + CELL_LEPTON_H / 2 + 36, 0);
				newcoord = Map[newcoord].Closest_Free_Spot(newcoord, false, false, false);
				if (newcoord != COORD_NONE && infantry->Unlimbo(newcoord, DIR_N)) {
					count--;
					infantry->Strength = Scen->RandomNumber(5, infantry->Class->MaxStrength);
					infantry->Scatter(COORD_NONE, true, false);
					if (source != NULL && !House->CanOverpower(source)) {
						infantry->QueueMission(MISSION_ATTACK, false);
						infantry->Assign_Target(source);
					} else if (House->FUN_0050B730()) {
						infantry->QueueMission(MISSION_MOVE, false);
					} else {
						infantry->QueueMission(MISSION_HUNT, false);
					}
				} else {
					delete infantry;
				}
				ScenarioInit--;
			}
		}

		if (cellptr->Is_Clear_To_Move(SPEED_TRACK, true, true, -1, MZONE_NORMAL, -1, true)) {
			Coord center = newcell.As_Coord();
			center.Z = 0;
			Coord spot = Coord_Scatter(center, CELL_LEPTON_W / 2, false);
			Coord mark = spot.As_Cell().As_Coord();
			mark.Z = 0;
			if (Scen->RandomNumber(0, 99) < 50) {
				SmudgeTypeClass::Scorch_The_Ground(mark, 100, 100, false);
			} else {
				SmudgeTypeClass::Crater_The_Ground(mark, 100, 100, false);
			}
		}
	}
}

// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y11b): BuildingClass::FUN_00457DE0 (gamemd.exe 0x00457DE0, 1,029 bytes,
// unmatched in the YR tree): a garrisoned building lets its occupants out (it is destroyed, red, sold
// or abandoned). Written from gamemd.exe's code: the first cell along the outside of the foundation
// that the first occupant can enter (the right edge upwards from the bottom right corner, the bottom
// edge leftwards, the top edge rightwards from the top left corner, the left edge downwards); with
// none, the occupants die unless the caller forces them out at the foundation's bottom right cell.
// Each is placed there, its garrison orders dropped for a human house, its target cleared, scattered
// away from the building, and (hunt) taken from its team and set to Hunt; one that cannot be placed is
// deleted. Then the list is emptied and the cell's threat updated.
// ---------------------------------------------------------------------------------------------
void BuildingClass::FUN_00457DE0(bool hunt, bool force)
{
	FiringOccupantIndex = 0;
	if (GetOccupantCount() == 0) {
		return;
	}
	Cell found = CELL_NONE;
	int width = Class->Width();
	int height = Class->Height(false);
	InfantryClass * first = Occupants[0];
	Cell home = Get_Cell();
	bool ok = false;
	auto enter = [&](Cell const & c) { return first->Can_Enter_Cell(&Map[c], FACING_NONE, -1, NULL, true) == MOVE_OK; };

	Cell c(home.X + width, home.Y + height);
	if (enter(c)) {
		found = c;
		ok = true;
	} else {
		while (c.Y > home.Y - 1) {
			c.Y--;
			if (enter(c)) { found = c; ok = true; break; }
		}
	}
	c = Cell(home.X + width, home.Y + height);
	while (!ok && c.X > home.X - 1) {
		c.X--;
		if (enter(c)) { found = c; ok = true; }
	}
	c = Cell(home.X - 1, home.Y - 1);
	while (!ok && c.X < home.X + width) {
		c.X++;
		if (enter(c)) { found = c; ok = true; }
	}
	c = Cell(home.X - 1, home.Y - 1);
	while (!ok && c.Y < home.Y + height) {
		c.Y++;
		if (enter(c)) { found = c; ok = true; }
	}
	if (!ok) {
		if (!force) {
			KillOccupants(NULL);
			return;
		}
		found = Cell(home.X + width - 1, home.Y + height - 1);
	}

	Coord coord = Map[found].Center_Coord();
	ScenarioInit++;
	for (int i = Occupants.Count() - 1; i >= 0; i--) {
		InfantryClass * occupant = Occupants[i];
		if (occupant->Unlimbo(coord, DIR_N)) {
			if (occupant->House->IsHuman) {
				occupant->ShouldGarrisonStructure = false;
				occupant->ShouldEnterOccupiable = false;
			}
			occupant->Assign_Target(NULL);
			occupant->Scatter(Center_Coord(), true, true);
			if (hunt) {
				if (occupant->Team != NULL) {
					occupant->Team->Remove(occupant, -1, false);
				}
				occupant->QueueMission(MISSION_HUNT, false);
			}
		} else {
			occupant->Delete_Me();
		}
	}
	ScenarioInit--;
	Occupants.Clear();
	UpdateThreatInCell(Get_Cell_Ptr());
}

// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y12): BuildingClass::Captured, gamemd.exe 0x00448260 (4,518 bytes,
// unmatched in the YR tree; a campaign map's opening ownership changes reached it as a link-closure
// trap). Written from behaviour: Red Alert 2's exact text (src/recovered/building.cpp
// BuildingClass::Captured, game.exe 0x00444660, which names gamemd.exe 0x00448260 as its twin) and
// TibSun's (building.cpp:4212) in the YR tree's names, with gamemd.exe's YR changes: the attached bomb
// disarmed unless the building can be occupied; the multiplayer-passive cash bonus; the EVA lines
// (EVA_TechBuildingLost, the type's CaptureEvaEvent, EVA_BuildingCaptured after the radar event 10); a
// building that needs an engineer gets its engineer flag and its light source; the house's YR lists
// (construction yards, repair bays, grinders, absorbers, bunkers, occupiables, cloning vats, secret
// labs, psychic detectors, factory plants with the cost multipliers, buildings) and counters (ore
// purifiers, docks, self heal) move from the old owner to the new, Grand_Opening(true) rebuilding the
// new owner's counters as it does for every captured building. NATIVE: the light source and the
// type's animations are not re-enabled here (Grand_Opening redraws them).
// Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------
bool BuildingClass::Captured(HouseClass * newowner, bool announce)
{
	if (newowner == House) {
		return(false);
	}
	if (AttachedBomb != NULL && !Class->CanBeOccupied) {
		AttachedBomb->Disarm();
	}
	if (Class->IsCloakGenerator) {
		newowner->HasCloakGenerator = true;
	}
	if (House->Class->IsMultiplayPassive && Class->ProduceCashStartup) {
		newowner->Refund_Money(Class->ProduceCashStartup);
		CashProductionTimer = Class->ProduceCashDelay;
	}
	if (SpiedBy & (1 << newowner->Class->House)) {
		SpiedBy -= (1 << newowner->Class->House);
		if (Class->IsRadar) {
			Update_Radar_Spied();
		}
	}
	IsLeader = false;
	if (Class->IsLaserFencePost) {
		Update_Laser_Fence_Connections(0);
	}
	if (House->Is_Player_Control() || newowner->Is_Player_Control()) {
		if (!newowner->Class->IsMultiplayPassive && announce) {
			if (Class->NeedsEngineer) {
				if (House != newowner) {
					if (House->Is_Player_Control()) VoxClass::Play("EVA_TechBuildingLost");
					if (newowner->Is_Player_Control() && Class->CaptureEvaEvent != -1) VoxClass::PlayIndex(Class->CaptureEvaEvent);
				}
			} else if (RadarEventClass::Create(RADAREVENT_BUILDING_CAPTURED, Get_Cell())) {
				VoxClass::Play("EVA_BuildingCaptured");
			}
		}
		Map.Flag_To_Redraw(GS_REDRAW_DIRTY);
	}
	IsToDisplay = true;
	HasEngineer = true;

	// Cargo and docked units change sides with it; units that only touch it are sent away.
	if (TechnoClass * passenger = (TechnoClass *)Cargo.Attached_Object()) {
		passenger->Captured(newowner, true);
	}
	struct Docked { TechnoClass * Contact; bool WasTethered; };
	Docked docked[8];
	int docked_count = 0;
	for (int i = 0; i < RadioLinks.Length(); i++) {
		TechnoClass * contact = FUN_0065AD30(i);
		if (contact == NULL || !contact->Is_Foot()) continue;
		intptr_t param = 0;
		if (Transmit_Message(RADIO_NEED_TO_MOVE, param, contact) == RADIO_ROGER
			&& (Class->IsWeaponsFactory || ::Distance(contact->Center_Coord(), Docking_Coord(contact)) < 64)) {
			bool tethered = contact->IsTethered;
			contact->Captured(newowner, true);
			if (docked_count < 8) {
				docked[docked_count].Contact = contact;
				docked[docked_count].WasTethered = tethered;
				docked_count++;
			}
		} else {
			Transmit_Message(RADIO_RUN_AWAY, contact);
			Transmit_Message(RADIO_OVER_OUT, contact);
		}
	}
	if (Factory != NULL) {
		Factory->Abandon();
		delete Factory;
		Factory = NULL;
	}
	House->Active_Remove(this);

	HouseClass * oldowner = House;
	IsCaptured = true;
	oldowner->ConYards.Delete(this);
	oldowner->UnitRepairStations.Delete(this);
	oldowner->Grinders.Delete(this);
	oldowner->Absorbers.Delete(this);
	oldowner->Bunkers.Delete(this);
	oldowner->Occupiables.Delete(this);
	oldowner->CloningVats.Delete(this);
	oldowner->SecretLabs.Delete(this);
	oldowner->PsychicDetectionBuildings.Delete(this);
	if (oldowner->FactoryPlants.Delete(this)) {
		oldowner->CalculateCostMultipliers();
	}
	oldowner->Buildings.Delete(this);
	if (Class->OrePurifier && oldowner->NumOrePurifiers > 0) oldowner->NumOrePurifiers--;
	if (Class->IsHelipad) {
		oldowner->AirportDocks -= Class->NumberOfDocks;
		if (oldowner->AirportDocks < 0) oldowner->AirportDocks = 0;
	}
	if (Class->InfantryGainSelfHeal) {
		oldowner->InfantrySelfHeal -= Class->InfantryGainSelfHeal;
		if (oldowner->InfantrySelfHeal < 0) oldowner->InfantrySelfHeal = 0;
	}
	if (Class->UnitsGainSelfHeal) {
		oldowner->UnitsSelfHeal -= Class->UnitsGainSelfHeal;
		if (oldowner->UnitsSelfHeal < 0) oldowner->UnitsSelfHeal = 0;
	}
	if (Class->IsCloakGenerator) {
		Disable_Cloak_Generator();
		CurrentCloakRadius = 1;
		Cloaking_AI(true);
	}

	TechnoClass::Captured(newowner, true);

	if (Class->IsCloakGenerator && IsPowerOnline()) {	// gamemd.exe 0x00448C01: vtable slot 0x350 (lane Y14)
		Enable_Cloak_Generator();
	}
	oldowner->ToCapture = this;
	oldowner->Recalc_Center();
	House->Recalc_Center();
	if (House->ToCapture == this) {
		House->ToCapture = NULL;
	}
	if (oldowner->Is_Player_Control() && Class->IsConstructionYard) {
		Map.PendingObjectPtr = NULL;
		Map.PendingObject = NULL;
		Map.PendingHouse = HOUSE_NONE;
		Map.Set_Cursor_Shape(NULL);
	}
	House->Active_Add(this);
	IsRepairing = false;
	if (Rule->BuildConst.Is_In_List(Class)) House->ConYards.Add(this);
	if (Class->IsCanUnitRepair) House->UnitRepairStations.Add(this);
	if (Class->Grinding) House->Grinders.Add(this);
	if (Class->UnitAbsorb || Class->InfantryAbsorb) House->Absorbers.Add(this);
	if (Class->Bunker) House->Bunkers.Add(this);
	if (Class->CanBeOccupied && Class->Level > -1) House->Occupiables.Add(this);
	if (Class->Cloning) House->CloningVats.Add(this);
	if (Class->SecretLab) House->SecretLabs.Add(this);
	if (Class->PsychicDetectionRadius > 0) House->PsychicDetectionBuildings.Add(this);
	if (Class->FactoryPlant) {
		House->FactoryPlants.Add(this);
		House->CalculateCostMultipliers();
	}
	House->Buildings.Add(this);
	Grand_Opening(true);
	if (Class->IsLaserFencePost) {
		Init_Laser_Fence();
		Init_Laser_Fence_Frame();
	}
	if (House->Is_Player_Control()) {
		Map.Flag_To_Redraw(GS_REDRAW_TACTICAL);
	}
	if (SpiedBy & (1 << newowner->Class->House)) {
		SpiedBy &= ~(1 << newowner->Class->House);
	}
	Update_Anim_Appearance();
	for (int d = 0; d < docked_count; d++) {
		Transmit_Message(RADIO_HELLO, docked[d].Contact);
		if (docked[d].WasTethered) {
			IsTethered = true;
			docked[d].Contact->IsTethered = true;
		}
	}
	oldowner->IsRecalcNeeded = true;
	newowner->IsRecalcNeeded = true;
	return(true);
}

// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y12): BuildingClass::Fire_Direction, gamemd.exe 0x0044D7D0 (161 bytes,
// BuildingClass vtable 0x007E3EBC slot 194; unmatched in the YR tree, a link-closure trap the first Soviet
// mission reached). TibSun's text (TibSun archive code/building.cpp:6151; same licence notice as the drafts
// above), which gamemd.exe follows: no target, the body's facing rounded to 256 directions; a turret not
// drawn as a voxel animation, its facing rounded to 32; otherwise the aim at the target (Aim_Direction).
// ---------------------------------------------------------------------------------------------
DirType BuildingClass::Fire_Direction(void) const
{
	if (TarCom == NULL) {
		return(DirType((Dir256)PrimaryFacing.Current().Round_To_256()));
	}
	if (Is_Turret_Equipped() && !Class->IsTurretAnimAVoxel) {
		return(DirType(PrimaryFacing.Current().As_Dir32()));
	}
	return(Aim_Direction(TarCom));
}

// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y15), appended to the building unit: BuildingClass::Do_MISSION_UNLOAD,
// gamemd.exe 0x0044D880 (2,848 bytes), unmatched in ra2-decomp. Written from gamemd.exe's code read
// instruction by instruction, on the shape of the RA2 twin (game.exe 0x00449290, src/recovered/building.cpp,
// matched there): YR adds the absorbers' unload in front (a Bio Reactor or a unit absorber lets its
// first passenger out to an adjacent cell), sends a unit whose locomotor is not the drive one 4,1
// cells away (RA2: 5,1) and plays the factory's door animation by name. Behaviour code for the native
// build only; not compared with a VC6 compile.
// ---------------------------------------------------------------------------------------------
void __cdecl FUN_004068E0(char const * fmt, ...);	// the debug log (audevent.h)

int BuildingClass::Do_MISSION_UNLOAD(void)
{
	if (GetOccupantCount() > 0) {
		FUN_00457DE0(false, false);
	}

	if (Class->UnitAbsorb || Class->InfantryAbsorb) {
		if (Cargo.How_Many() > 0) {
			FootClass * passenger = Cargo.Detach_Object();
			if (passenger != NULL) {
				/*
				**	An adjacent cell the passenger can enter, starting from the building's facing:
				**	the first pass also wants the cell beyond it free (room to walk away), the
				**	second takes the adjacent cell alone; never a cell under a bridge.
				*/
				unsigned short raw = (unsigned short)(PrimaryFacing.Current().Facing + 0x7FFF);
				int start = ((raw >> 12) + 1) >> 1;
				bool roomy = true;
				bool found = false;
				Cell near_cell, far_cell;
				int dir = 0;
				for (int i = 0; i < 8; i++) {
					dir = (start + i) & 7;
					near_cell = Get_Cell() + AdjacentCell[dir];
					far_cell = near_cell + AdjacentCell[dir];
					bool far_ok = passenger->Can_Enter_Cell(&Map[far_cell], (FacingType)dir, Get_Cell_Height(), NULL, true) == MOVE_OK;
					bool near_ok = passenger->Can_Enter_Cell(&Map[near_cell], (FacingType)dir, Get_Cell_Height(), NULL, true) == MOVE_OK;
					if ((!near_ok || (!far_ok && roomy))) {
						if (roomy && i + 1 == 8) {
							roomy = false;
							i = 0;		// gamemd.exe's second pass starts again at the second direction
						}
						continue;
					}
					if (Map[near_cell].IsUnderBridge) continue;
					found = true;
					break;
				}

				bool placed = false;
				if (found) {
					Coord coord(near_cell);
					ScenarioInit++;
					if (passenger->Fetch_RTTI() == RTTI_INFANTRY) {
						coord = Map.Closest_Free_Spot(coord, false);
					} else {
						near_cell = Map.Nearby_Location(near_cell, passenger->Techno_Type_Class()->Speed, -1, MZONE_NORMAL,
							false, Point2D(1, 1), false, false, false, true, Cell(0, 0));
						coord = Coord(Cell(Map[near_cell].Cell_Coord()));
					}
					placed = passenger->Unlimbo(coord, Facing_Dir((FacingType)dir));
					ScenarioInit--;
				}

				if (placed) {
					if (Class->InfantryAbsorb) {
						VocClass::PlayAt(Rule->LeaveBioReactorSound, Center_Coord());
						if (passenger->House->IsHuman) {
							passenger->ShouldEnterAbsorber = false;
						}
						if (!passenger->CountedAsOwnedSpecial) {
							passenger->House->CurInfantry++;
							passenger->CountedAsOwnedSpecial = true;
						}
						passenger->Absorbed = false;
					}
					passenger->QueueMission(MISSION_MOVE, false);
					passenger->Assign_Destination(&Map[roomy ? far_cell : near_cell], true);
					if (Class->ExtraPower > 0) {
						House->RecalcPower = true;
					}
				} else {
					Cargo.Attach(passenger);
					passenger->Hidden();
				}
			}
			return(Current_Mission_Control().Normal_Delay() + Random_Pick(0, 2));
		}
	}

	if (Class->IsWeaponsFactory) {
		CoordStruct destination;
		destination.X = destination.Y = destination.Z = 0;
		if (!Class->Naval) {
			Cell position = Get_Cell() + Class->ExitList[10];
			destination = Coord(Cell(position.X - 1, position.Y));
		}

		switch (Status) {
			case 0: {
				TechnoClass * unit = FUN_0065AD30(0);
				if (unit != NULL) {
					unit->QueueMission(MISSION_GUARD, false);
					unit->Commence();
				}
				Door.Open_Door(Class->DeployTime);
				IsToDisplay = true;
				Status = Class->Naval ? 4 : 1;
				DestroyNthAnim((BAnimType)18);
				bool damaged = Get_Health_Ratio() <= Rule->ConditionYellow;
				char const * name = damaged ? Class->AnimData[8].AnimDamaged : Class->AnimData[8].Anim;
				if (name != NULL && *name != '\0') {
					PlayAnim(name, (BAnimType)8, damaged, false, 0);
				}
				break;
			}

			case 1:
				if (!Clear_Weapons_Factory_Bib()) {
					FUN_004068E0("Weapons factory bib clear - kicking out unit\n");
					Status = 2;
				}
				break;

			case 2:
				if (Door.Is_Door_Open()) {
					FootClass * unit = (FootClass *)FUN_0065AD30(0);
					if (unit != NULL) {
						unit->QueueMission(MISSION_MOVE, false);
						IPersistPtr persist(unit->Locomotion);
						CLSID classid;
						persist->GetClassID(&classid);
						if (classid == CLSID_TunnelLocomotion || classid == CLSID_TeleportLocomotion) {
							IPiggybackPtr piggy(unit->Locomotion);
							if (piggy != NULL && piggy->Is_Piggybacking()) {
								unit->Locomotion.Attach(NULL);
								piggy->End_Piggyback(&unit->Locomotion);
							}
							ILocomotionPtr drive;
							drive.CreateInstance(CLSID_DriveLocomotion, NULL, CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER | CLSCTX_LOCAL_SERVER);
							drive->Link_To_Object(unit);
							IPiggybackPtr carrier(drive);
							piggy = carrier;
							if (piggy != NULL) {
								piggy->Begin_Piggyback(unit->Locomotion);
								unit->Locomotion = drive;
								unit->Locomotion->Force_Track(0x42, destination);
							} else {
								int damage = unit->Strength;
								unit->Take_Damage(damage, 0, Rule->C4Warhead, NULL, true, false, NULL);
							}
						} else if (classid != CLSID_DriveLocomotion) {
							unit->Assign_Destination(&Map[Get_Cell() + Cell(4, 1)], true);
						} else {
							unit->Locomotion->Force_Track(0x42, destination);
						}
						unit->Set_Speed(0.5);
						Status = 3;
					} else {
						Door.Close_Door(Class->DeployTime);
						Status = 4;
					}
				}
				break;

			case 3:
				if (!IsTethered) {
					Door.Close_Door(Class->DeployTime);
					Status = 4;
				}
				break;

			case 4:
				if (Door.Is_Door_Closed() || Class->Naval) {
					Enter_Idle_Mode(false, true);
					IsToDisplay = true;
				}
				break;

			default:
				break;
		}
		return(Current_Mission_Control().Normal_Delay() + Random_Pick(0, 2));
	}

	if (Class->GapGenerator && Class->SuperGapRadiusInCells) {
		if (!GapSuperCharged) {
			DestroyGap();
			GapRadius = Class->SuperGapRadiusInCells;
			if (IsPowerOnline()) {
				CreateGap();
				GapSuperCharged = true;
				HasExtraPowerDrain = true;
				House->RecalcPower = true;
			} else {
				GapRadius = Class->GapRadiusInCells;
				CreateGap();
			}
		} else {
			DestroyGap();
			GapSuperCharged = false;
			HasExtraPowerDrain = false;
			GapRadius = Class->GapRadiusInCells;
			CreateGap();
			House->RecalcPower = true;
		}
	}
	QueueMission(MISSION_GUARD, false);
	return(1);
}

// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE STAND-IN (lane N3, 2026-11-19): BuildingClass::ForceCreate (gamemd.exe 0x00458A80,
// 809 bytes; BuildingClass's vtable slot 0x490), unmatched in ra2-decomp. Written from gamemd.exe's
// instructions (native/yr/tools/yr_dis.py) in the shape of the tree's matched FootClass::ForceCreate
// (0x004DF510), which uses the same virtual calls on one cell: over every cell of the building's
// foundation an overlay is cleared and the units there are lifted off the map (any building or
// non-techno there refuses the placement and puts them back); the building is placed; the units come
// down on the nearest free cells. Reached by the trigger action "create building at"
// (TActionClass::TAction_CreateBuildingAt) and HouseClass::RespawnStartingForces. Behaviour code for
// the native build only.
// ---------------------------------------------------------------------------------------------
bool BuildingClass::ForceCreate(Coord & coord, unsigned unk)
{
	Cell const * offset = Occupy_List(false);
	Cell origin = coord.As_Cell();
	DynamicVectorClass<TechnoClass *> objs;
	int index;
	while (*offset != REFRESH_EOL) {
		CellClass & cell = Map[origin + *offset++];
		if (cell.Overlay != OVERLAY_NONE) {
			cell.Overlay = OVERLAY_NONE;
			cell.OverlayData = 0;
			cell.Rubble = NULL;
		}
		for (ObjectClass * occupier = cell.Cell_Occupier(); occupier != NULL; occupier = cell.Cell_Occupier()) {
			if (!(occupier->AbstractFlags & 1) || occupier->RTTI == RTTI_BUILDING) {
				for (index = objs.Count() - 1; index >= 0; index--) {
					TechnoClass * object = objs[index];
					Coord position = object->Position;
					if (!object->Unlimbo(position, DIR_N)) {
						object->Delete_Me();
					}
				}
				objs.Clear();
				return(false);
			}
			objs.Add((TechnoClass *)occupier);
			occupier->Limbo();
		}
	}
	if (!Unlimbo(coord, (Dir256)unk)) {
		Delete_Me();
		for (index = objs.Count() - 1; index >= 0; index--) {
			TechnoClass * object = objs[index];
			Coord position = object->Position;
			if (!object->Unlimbo(position, DIR_N)) {
				object->Delete_Me();
			}
		}
		objs.Clear();
		return(false);
	}
	for (index = objs.Count() - 1; index >= 0; index--) {
		TechnoClass * object = objs[index];
		Cell current = object->Get_Cell();
		Cell nearby = Map.Nearby_Location(current, object->TClass->Speed);
		if (!object->Unlimbo(Map[nearby].Center_Coord(), DIR_N)) {
			object->Delete_Me();
		}
	}
	return(true);
}
