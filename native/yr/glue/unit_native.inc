
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFTS (lane Y3): functions unmatched in the YR tree, the text TibSun's
// (TibSun archive code/unit.cpp; SPDX GPL-3.0-or-later, Copyright 2025 Electronic Arts Inc. and
// OpenTS contributors, EA's GPL section 7 terms; THIRD_PARTY.md); native changes are marked
// NATIVE. Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------

void UnitClass::Assign_Destination(AbstractClass * target, bool immediate)
{
	/*
	**	Abort early if there is anything wrong with the parameters
	**	or the unit already is assigned the specified destination.
	*/
	if (target == NavCom) return;

	/*
	**	Transport vehicles must tell all passengers that are about to load, that they
	**	cannot proceed. This is accomplished with a radio message to this effect.
	*/
	if (In_Radio_Contact() && Class->Max_Passengers() > 0 && Contact_With_Whom()->Fetch_RTTI() == RTTI_INFANTRY) {
		Transmit_Message(RADIO_OVER_OUT);
	}

	/*
	 * If tethered to a carryall while sleeping, untether before moving.
	 */
	if (target != NULL && In_Radio_Contact() && Mission == MISSION_SLEEP && IsTethered && Contact_With_Whom()->RTTI == RTTI_AIRCRAFT) {
		AircraftClass * a = (AircraftClass *)Contact_With_Whom();
		if (a->Class->IsCarryall) {
			a->Assign_Destination(NULL);
			Transmit_Message(RADIO_UNTETHER);
			Transmit_Message(RADIO_OVER_OUT);
		}
	}

	BuildingClass * b = dynamic_cast<BuildingClass *>(target);

	/*
	**	Handle entry logic here.
	*/
	if (Mission == MISSION_ENTER || MissionQueue == MISSION_ENTER) {

		/*
		**	If not already in radio contact (presumed with the transport), then
		**	either try to establish contact if allowed, or just move close and
		**	wait until radio contact can be established.
		*/
		if (!In_Radio_Contact()) {
			if (b != NULL) {

				/*
				**	Determine if the transport is already in radio contact. If so, then just move
				**	toward the transport and try to establish contact at a later time.
				*/
				if (b->In_Radio_Contact()) {
					ArchiveTarget = target;

					/*
					**	HACK ALERT: The repair bay is counting on the assignment of the NavCom by this routine.
					**	The refinery must NOT have the navcom assigned by this routine.
					*/
					if (b->Class->IsCanUnitRepair) {
						target = NULL;	// NATIVE: TibSun also remembers the bay (NearbyObject); gamemd's YR code reads no building class here
					}
				} else {
					AbstractClass * oldnav = NavCom;
					if (Transmit_Message(RADIO_DOCKING, b) != RADIO_ROGER) {
						Transmit_Message(RADIO_OVER_OUT);
						if (b->Class->IsCanUnitRepair) {
							ArchiveTarget = target;
						}
					} else {
						if (b->Class->IsDockUnload && NavCom != oldnav) {
							target = NavCom;
						}
					}
				}
			} else {
				TechnoClass * techno = Dynamic_Cast<TechnoClass *>(target);
				if (techno != NULL) {

					/*
					**	Determine if the transport is already in radio contact. If so, then just move
					**	toward the transport and try to establish contact at a later time.
					*/
					if (techno->In_Radio_Contact()) {
						ArchiveTarget = target;
					} else {
						if (Transmit_Message(RADIO_HELLO, techno) == RADIO_ROGER) {
							if (Transmit_Message(RADIO_DOCKING) == RADIO_ROGER) {
								return;
							}
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

	bool useplan = false;

	/*
	 * Subterranean units prepend their navigation target onto the queue and
	 * re-target the nearest reachable cell when driving rather than burrowing.
	 */
	if (target != NULL && Class->IsSubterranean && Locomotion->Is_Moving()) {
		IPersistPtr persist(Locomotion);
		CLSID clsid;
		persist->GetClassID(&clsid);
		if (clsid == CLSID_DriveLocomotion) {
			NavQueue.Add_Head(target);
			RouteQueue.Clear();
			CellClass * tcell = Get_Target_Cell_Ptr();
			target = tcell;
			immediate = false;
			if (tcell != NULL && tcell->IsUnderBridge && !Is_Moving_Onto_Bridge()) {
				Cell nearby = Map.Nearby_Location(tcell->CellID, SPEED_TRACK, Map.Get_Cell_Zone(&tcell->CellID, Class->MZone, false), Class->MZone, false, Point2D(1, 1), false, true, false, false);
				if (nearby != CELL_NONE) {
					target = &Map[nearby];
				}
			}
		}
	}

	bool needsswap = false;

	if (target != NULL) {
		bool surface = false;

		/*
		 * A subterranean unit that is told to move immediately must decide whether
		 * it can burrow straight to the destination or has to plan a surface route.
		 */
		if (Class->IsSubterranean && immediate) {
			bool isbridge = Is_Moving_Onto_Bridge();

			Cell from = Destination_Coord().As_Cell();
			bool canburrowfrom = Map[from].Can_Burrow_Here();

			Cell to = target->Center_Coord().As_Cell();
			CellClass * tocell = &Map[to];
			bool isunderbridgeto = tocell->IsUnderBridge;
			bool canburrowto = tocell->Can_Burrow_Here();
			int height = Get_Height_AGL();

			if (isbridge) {
				if (isunderbridgeto) {
					int z1 = Map.Zone_Connection_Index(from, 3, 0);
					if (z1 == Map.Zone_Connection_Index(to, 3, 0) && z1 != -1) {
						if (!Map.ZoneConnections[z1].IsPassable) {
							Cell d1 = Map.Get_Zone_Connection_Destination(from, from);
							Cell d2 = Map.Get_Zone_Connection_Destination(to, to);
							if (d1.Distance_To(d2) <= 2) {
								needsswap = true;
							}
						} else {
							needsswap = true;
						}
					}
				}
				if (!needsswap) {
					useplan = true;
				}
			} else if (isunderbridgeto) {
				useplan = true;
			}

			if (((!canburrowfrom && height >= 0) || !canburrowto) && !needsswap || useplan) {
				target = Plan_Route(target);	// NATIVE: YR's Plan_Route takes the target
				surface = true;
			}
		}

		/*
		 * The non-subterranean (and the no-plan subterranean) immediate path clears
		 * the route queue before surfacing a tunneler that is on its way up.
		 */
		if (!surface) {
			if (immediate) {
				RouteQueue.Clear();
				surface = true;
			}
		}

		/*
		 * Surfacing tunneler keeps its current cell at the route front.
		 */
		if (surface) {
			if (Locomotion->Is_Surfacing()) {
				RouteQueue.Add_Head(target);
				target = &Map[Get_Coord()];
			}
		}

		/*
		 * If the locomotor is a tunnel locomotor and the unit is on the ground,
		 * surface the tunneler by swapping the tunnel locomotor for a drive one.
		 * (Mirrors BuildingClass weapons-factory exit, building.cpp:6236-6251.)
		 */
		if (target != NULL && !Locomotion->Is_Moving()) {
			IPersistPtr persist(Locomotion);
			CLSID clsid;
			persist->GetClassID(&clsid);
			if (clsid == CLSID_TunnelLocomotion && Get_Height_AGL() == 0) {
				Coord tc = target->Center_Coord();
				int gl = Map.Get_Height_GL(tc);
				if (tc.Z < gl) tc.Z = gl;

				bool doswap = needsswap;
				if (!doswap) {
					if (!Is_Moving_Onto_Bridge()
						&& !Map[target->Center_Coord()].IsUnderBridge
						&& Map[Destination_Coord()].Can_Burrow_Here()
						&& Map[target->Center_Coord()].Can_Burrow_Here()) {
						Cell to = target->Center_Coord().As_Cell();
						if (!Is_Route_Broken(Get_Target_Cell(), to)) {
							doswap = true;
						}
					} else {
						doswap = true;
					}
				}

				if (doswap) {
					IPiggybackPtr piggy(Locomotion);
					if (piggy != NULL && piggy->Is_Piggybacking()) {
						piggy->End_Piggyback(&Locomotion);
					}
					ILocomotionPtr walk(CLSID_DriveLocomotion);
					walk->Link_To_Object(this);
					piggy = IPiggybackPtr(walk);
					if (piggy != NULL) {
						piggy->Begin_Piggyback(Locomotion);
						Locomotion = walk;
						Locomotion->Force_New_Slope(Map[Get_Coord()].Ramp);
					}
				}
			}
		}
	} else {
		RouteQueue.Clear();
	}

	/*
	**	If the player clicked on a friendly repair facility and the repair
	**	facility is currently not involved with some other unit (radio or unloading).
	*/
	if (b != NULL && b->Class->IsCanUnitRepair) {
		if (b->In_Radio_Contact() && b->Contact_With_Whom() != this) {
			ArchiveTarget = target;
		} else {

			/*
			**	Establish radio contact protocol. If the facility responds correctly,
			**	then remain in radio contact and proceed toward the desired destination.
			*/
			if (Transmit_Message(RADIO_HELLO, b) == RADIO_ROGER) {
				if (Transmit_Message(RADIO_DOCKING) == RADIO_ROGER) {
					BASECLASS::Assign_Destination(target, immediate);
					Path[0] = FACING_NONE;
					return;
				}

				/*
				**	Failure to establish a docking relationship with the refinery.
				**	Bail & await further instructions.
				*/
				Transmit_Message(RADIO_OVER_OUT);
				target = NULL;
			}
		}
	}

	/*
	 * A weapons factory that this unit is exiting wants the unit driven to its
	 * exit cell before anything else; queue the real destination behind it.
	 */
	BuildingClass * wf = (BuildingClass *)Contact_With_Whom();
	if (wf != NULL && wf->Fetch_RTTI() == RTTI_BUILDING && wf->Class->IsWeaponsFactory) {
		Cell exitcell = wf->Get_Coord().As_Cell() + Cell(3, 1);
		if (target != &Map[exitcell]) {
			NavQueue.Clear();
			RouteQueue.Clear();
			if (target != NULL) {
				Queue_Navigation_List(target);
			}
			return;
		}
	}

	/*
	 * Harvesters and weeders automatically enter their dock building when the
	 * clicked target is that building (and the houses are allied).
	 */
	if (b != NULL && Class->Dock.Count() > 0 && b->Class == (BuildingTypeClass *)Class->Dock[0] &&
		Fetch_RTTI() == RTTI_UNIT && (Class->Harvester || Class->Weeder)	/* NATIVE: YR's names */ &&
		Mission != MISSION_UNLOAD && b->House->Is_Ally(House) && House->Is_Ally(b->House)) {

		if (Contact_With_Whom() != b && !b->In_Radio_Contact() && Transmit_Message(RADIO_HELLO, b) == RADIO_ROGER &&
			Mission != MISSION_ENTER && Mission != MISSION_HARVEST) {
			Assign_Mission(MISSION_ENTER);
			target = NULL;
		} else {
			if (target != NULL && target->Fetch_RTTI() == RTTI_BUILDING && Contact_With_Whom() != target) {
				Cell nearby = Map.Nearby_Location(b->Get_Coord().As_Cell(), SPEED_WHEEL, Map.Get_Cell_Zone(&b->Get_Coord().As_Cell(), Class->MZone, false), Class->MZone, false, Point2D(1, 1), false, true, false, false);
				if (nearby != CELL_NONE) {
					target = &Map[nearby];
				} else {
					target = NULL;
				}
			}
		}
	}

	/*
	 * If the locomotor is unpowered and we are not in an ion storm, power it
	 * back on when an adjacent friendly repair facility occupies our cell.
	 */
	if (!Locomotion->Is_Powered() && !IonStormClass::Is_Ion_Storm_Active()) {
		CellClass * cellptr = Get_Cell_Ptr();
		if (!cellptr->IsUnderBridge) {
			ObjectClass * occupier = cellptr->Cell_Occupier();
			while (occupier != NULL) {
				if (occupier != this && occupier->Fetch_RTTI() == RTTI_BUILDING && ((BuildingClass *)occupier)->Class->IsCanUnitRepair) {
					Locomotion->Power_On();
					break;
				}
				occupier = occupier->Next;
			}
		}
	}

	BASECLASS::Assign_Destination(target, immediate);
}

// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y6): UnitClass::Do_MISSION_HARVEST (gamemd.exe 0x0073E5E0, 2,528 B),
// unmatched in ra2-decomp. TibSun's Mission_Harvest text as the RA2 twin has it (src/recovered/
// unit.cpp, RA2 0x007025A0, exact there: the chrono miner's teleport, the too-far distances, the
// dock search over the type's list), with YR's changes read from gamemd.exe's code: a resource
// gatherer that keeps slaves hands the mission to its slave manager; an idle useless harvester
// goes for repair when a repair bay is reachable (RA2: when the house owns one). Not compared byte
// for byte; behaviour code for the native build only.
// ---------------------------------------------------------------------------------------------
static inline int Harvest_Distance(ObjectClass const * a, ObjectClass const * b)
{
	Coord from = a->Center_Coord();
	Coord to = b->Center_Coord();
	double x = to.X - from.X;
	double y = to.Y - from.Y;
	double z = to.Z - from.Z;
	return((int)fastmath::sqrt(x * x + y * y + z * z));
}

int UnitClass::Do_MISSION_HARVEST(void)
{
	enum { LOOKING, HARVESTING, FINDHOME, HEADINGHOME, GOINGTOIDLE };
	int i;

	if (Class->ResourceDestination && Class->ResourceGatherer && SlaveManager != NULL) {
		SlaveManager->FUN_006B0DB0();
		return(Current_Mission_Control().Normal_Delay() + Random_Pick(0, 2));
	}

	if (!Class->Harvester && !Class->Weeder) return(TICKS_PER_SECOND * 30);

	if (Class->Dock.Count() == 0 && !House->Is_Human_Player()) {
		QueueMission(MISSION_GUARD, false);
		return(1);
	}

	bool found = false;
	for (i = 0; i < Class->Dock.Count(); i++) {
		if (House->BQuantity.Value(Class->Dock[i]->HeapID) > 0) {
			found = true;
			break;
		}
	}
	if (!found) {
		QueueMission(MISSION_GUARD, false);
		return(1);
	}

	bool teleporter = Class->Teleporter;
	TypeList<BuildingTypeClass *> * docks = (TypeList<BuildingTypeClass *> *)&Class->Dock;

	switch (Status) {
		case LOOKING:
			if (!Class->Weeder && Tiberium_Load() >= 1.0) {
				Status = FINDHOME;
				return(1);
			}
			{
				bool hastarget = true;
				if (ArchiveTarget != NULL) {
					Assign_Destination(ArchiveTarget, true);
					ArchiveTarget = NULL;
					hastarget = false;
				}
				IsHarvesting = false;
				bool ok;
				if (Class->Weeder) {
					ok = Goto_Weed(Rule->TiberiumLongScan / CELL_LEPTON_W);
				} else {
					if (Locomotion) {
						CLSID clsid;
						IPersistPtr persist(Locomotion);
						persist->GetClassID(&clsid);
						if (memcmp(&clsid, &CLSID_TeleportLocomotion, sizeof(CLSID)) == 0 && NavCom != NULL) {
							Assign_Destination(NULL, true);
						}
					}
					ok = Goto_Tiberium(Rule->TiberiumLongScan / CELL_LEPTON_W, hastarget);
				}
				if (ok) {
					IsHarvesting = true;
					Animation.Set_Rate(2);
					Animation.Set_Stage(0);
					Status = HARVESTING;
					return(1);
				}
				if (NavCom == NULL) {
					if (ArchiveTarget != NULL) {
						Assign_Destination(ArchiveTarget, true);
					} else {
						Status = GOINGTOIDLE;
						IsUseless = true;
						if (Class->Harvester) {
							House->IsTiberiumShort = true;
						}
						return(TICKS_PER_SECOND * 7);
					}
				} else {
					IsUseless = false;
				}
			}
			break;

		case HARVESTING:
			if (Animation.Fetch_Rate() == 0) {
				Animation.Set_Stage(0);
				Animation.Set_Rate(Rule->HarvesterLoadRate);
			}
			if (Animation.Fetch_Stage() < 9) return(1);
			if (!Harvesting()) {
				IsHarvesting = false;
				if (Class->Harvester && Tiberium_Load() == 1.0) {
					Status = FINDHOME;
					Cell cell;
					if (Class->Harvester) {
						cell = ScanForTiberium(Rule->TiberiumShortScan / CELL_LEPTON_W, false);
					} else {
						cell = Search_For_Weed(Rule->TiberiumShortScan / CELL_LEPTON_W);
					}
					if (cell != CELL_NONE) {
						ArchiveTarget = &Map[cell];
					} else {
						ArchiveTarget = NULL;
					}
					return(1);
				}
				bool ok;
				if (Class->Harvester) {
					ok = Goto_Tiberium(Rule->TiberiumShortScan / CELL_LEPTON_W, false);
				} else {
					ok = Goto_Weed(Rule->TiberiumShortScan / CELL_LEPTON_W);
				}
				if (!ok && NavCom == NULL) {
					ArchiveTarget = NULL;
					Status = FINDHOME;
				} else {
					Status = HARVESTING;
					IsHarvesting = true;
				}
				return(1);
			}
			return(1);

		case FINDHOME:
			if (NavCom != NULL && teleporter && TryNearestDockBuilding(docks, 0, 0) != NULL) {
				AbortMotion();
			}
			if (NavCom == NULL) {
				BuildingClass * nearest = TryNearestDockBuilding(docks, 0, 0);
				if (nearest != NULL
						&& Harvest_Distance(this, nearest) <= (teleporter ? Rule->ChronoHarvTooFarDistance : Rule->HarvesterTooFarDistance) * CELL_LEPTON_W
						&& Transmit_Message(RADIO_HELLO, nearest) == RADIO_ROGER) {
					Status = HEADINGHOME;
					break;
				}
				ScenarioInit++;
				nearest = TryNearestDockBuilding(docks, 0, 1);
				ScenarioInit--;
				if (nearest != NULL && (Harvest_Distance(this, nearest) > 3 * CELL_LEPTON_W || teleporter)) {
					Cell cell = nearest->Get_Coord().As_Cell();
					Cell here = cell;
					here.X += (short)nearest->Class->QueueingCell.X;
					here.Y += (short)nearest->Class->QueueingCell.Y;
					here = Map.Nearby_Location(here, SPEED_WHEEL, -1, MZONE_NORMAL, false, Point2D(1, 1), false, false, false, true, Cell(0, 0), false, false);
					if (here != CELL_NONE) {
						Assign_Destination(&Map[here], true);
					} else {
						Assign_Destination(nearest, true);
					}
				}
			}
			break;

		case HEADINGHOME:
			QueueMission(MISSION_ENTER, false);
			return(1);

		case GOINGTOIDLE:
			if (IsUseless) {
				if (TryNearestDockBuilding((TypeList<BuildingTypeClass *> *)&Rule->RepairBay, 0, 1) != NULL) {
					QueueMission(MISSION_REPAIR, false);
				} else {
					QueueMission(MISSION_HUNT, false);
				}
			}
			{
				BuildingClass * building = Map[Get_Coord()].Cell_Building();
				if (building != NULL && (building->Class->IsRefinery || building->Class->IsWeeder)) {
					Assign_Destination(&Map[Nearby_Location(building)], true);
				}
			}
			QueueMission(MISSION_GUARD, false);
			break;
	}
	return(Current_Mission_Control().Normal_Delay() + Random_Pick(0, 2));
}

// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y15): UnitClass::Is_Route_Broken, unmatched in ra2-decomp (called by
// the route planning above). TibSun's text (TibSun archive code/unit.cpp; SPDX GPL-3.0-or-later,
// Copyright 2025 Electronic Arts Inc. and OpenTS contributors, EA's GPL section 7 terms;
// THIRD_PARTY.md); gamemd.exe's version is not compared. Behaviour code for the native build only.
// ---------------------------------------------------------------------------------------------
bool UnitClass::Is_Route_Broken(Cell const & from, Cell const & to) const
{
	if (HeightAGL >= 0 && Map.Is_Same_Cell_Zone(from, to, MZONE_NORMAL, 0, 0, 0)) {
		if (from == to) {
			return(false);
		}
		int dist = MAX(abs(to.X - from.X), abs(to.Y - from.Y));
		if (dist == 1) {
			return(false);
		}
		if (dist >= 12 || !Map.In_Local_Radar(from) || !Map.In_Local_Radar(to)) {
			return(true);
		}
		return(Search.Test_Cell_Walk(from, to, this, false, Map[to].IsUnderBridge, MZONE_NORMAL) > 15);
	}
	return(true);
}
