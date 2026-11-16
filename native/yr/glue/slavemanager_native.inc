// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFTS (lane Y6): SlaveManagerClass::FUN_006AF6C0 (gamemd.exe 0x006AF6C0, 1,688 B)
// and FUN_006AFD60 (0x006AFD60, 1,268 B), unmatched in ra2-decomp; YR-only (no TibSun or RA2 text),
// written from gamemd.exe's code. The slave miner's two state machines, run every 10 frames
// from SlaveManagerClass::AI. Behaviour code for the native build only.
// ---------------------------------------------------------------------------------------------

// The cell the slaves leave from and come back to: a deployed miner's east edge, half way down;
// an undeployed miner's own cell.
static Cell Slave_Home_Cell(TechnoClass * owner)
{
	if (owner != NULL && owner->Fetch_RTTI() == RTTI_BUILDING) {
		BuildingClass * building = As_Building(owner);
		return(owner->Get_Cell() + Cell(building->Class->Width() - 1, building->Class->Height(false) / 2));
	}
	return(owner->Get_Cell());
}

static inline bool Slave_Timer_Expired(CDTimerClass<FrameTimerClass> const & timer)
{
	return(timer.Value() == 0);
}

// Each slave's state: 0 waiting in the miner, 1 looking for ore, 2 going to ore, 3 harvesting,
// 4 going home, 5 unloading (reloading inside the miner), 6 dead (respawn timer).
void SlaveManagerClass::FUN_006AF6C0(void)
{
	for (int index = 0; index < SlaveNodes.Count(); index++) {
		SlaveControl * node = SlaveNodes[index];
		InfantryClass * slave = node->Slave;

		if (slave == NULL && node->State != 6) {
			node->Slave = NULL;
			node->State = 6;
			node->RespawnTimer = CDTimerClass<FrameTimerClass>(RegenRate);
		}

		switch (node->State) {
			case 1: {
				Cell cell = slave->ScanForTiberium(Rule->SlaveMinerSlaveScan / 256, false);
				if (cell != CELL_NONE) {
					slave->Assign_Destination(&Map[cell], true);
					slave->QueueMission(MISSION_MOVE, false);
					node->State = 2;
				} else {
					slave->Assign_Archive_Target(NULL);
					slave->Assign_Destination(&Map[Slave_Home_Cell(Owner)], true);
					slave->QueueMission(MISSION_MOVE, false);
					node->State = 4;
				}
				break;
			}

			case 2:
				if (slave->Get_Cell_Ptr()->FUN_00487DF0()) {
					slave->FUN_00522D00();
					node->State = 3;
				} else if (slave->NavCom == NULL) {
					node->State = 1;
				}
				break;

			case 3:
				if (slave->FUN_00522D30()) {
					slave->Assign_Archive_Target(slave->Get_Cell_Ptr());
					slave->Assign_Destination(&Map[Slave_Home_Cell(Owner)], true);
					slave->QueueMission(MISSION_MOVE, false);
					node->State = 4;
				} else if (!slave->Get_Cell_Ptr()->FUN_00487DF0()) {
					slave->FUN_00522D20();
					node->State = 1;
				} else if (!slave->FUN_00522FC0()) {
					node->State = 1;
				}
				break;

			case 4: {
				Cell home = Slave_Home_Cell(Owner);
				bool astray = false;
				if (slave->NavCom != NULL) {
					Cell target = slave->NavCom->Center_Coord().As_Cell();
					Cell delta(target.X - home.X, target.Y - home.Y);
					if (delta.Length() > Rule->ApproachTargetResetMultiplier) {	// gamemd.exe: Rule +0xDF8
						astray = true;
					}
				}
				if (slave->Get_Cell() == home) {
					if (slave->NavCom == NULL) {
						slave->FUN_00522D50(Owner);
						slave->Limbo();
						node->State = 5;
						node->RespawnTimer = CDTimerClass<FrameTimerClass>(ReloadRate);
						break;
					}
					if (!astray) break;
				} else if (slave->NavCom != NULL && !astray) {
					break;
				}
				slave->Assign_Destination(&Map[home], true);
				slave->QueueMission(MISSION_MOVE, false);
				break;
			}

			case 5:
				if (Slave_Timer_Expired(node->RespawnTimer)) {
					slave->Strength = slave->Class->MaxStrength;
					slave->EstimatedHealth = slave->Class->MaxStrength;	// gamemd.exe: ObjectClass +0x70
					node->State = 0;
				}
				break;

			case 6:
				if (Slave_Timer_Expired(node->RespawnTimer)) {
					CreateSlave(node);
				}
				break;

			default:
				break;
		}
	}
}

// The miner's state: 0 idle, 1 looking for a field, 2 driving there, 3 deploying failed (retry),
// 4 deploying, 5 deployed (slaves out), 6 packing up to move to a better field.
void SlaveManagerClass::FUN_006AFD60(void)
{
	UnitClass * unit = (Owner != NULL && Owner->Fetch_RTTI() == RTTI_UNIT) ? (UnitClass *)Owner : NULL;
	BuildingClass * building = (Owner != NULL && Owner->Fetch_RTTI() == RTTI_BUILDING) ? (BuildingClass *)Owner : NULL;

	switch (State) {
		case 0:
			if (building != NULL && building->CurrentMission != MISSION_DECONSTRUCTION && building->CurrentMission != MISSION_CONSTRUCTION) {
				State = 5;
				LastScanFrame = 0x7FFFFFFF;
			}
			break;

		case 1: {
			if (unit != NULL && unit->NavCom != NULL) {
				State = 2;
				LastScanFrame = 0x7FFFFFFF;
				break;
			}
			Cell cell = Owner->ScanForTiberium(Rule->SlaveMinerLongScan / 256, true);
			if (cell != CELL_NONE) {
				cell = FUN_006B0300(cell);
			}
			if (cell == CELL_NONE) {
				State = 0;
				LastScanFrame = Unsorted::CurrentFrame;
				break;
			}
			Owner->Assign_Destination(&Map[cell], true);
			Owner->QueueMission(MISSION_MOVE, false);
			State = 2;
			LastScanFrame = 0x7FFFFFFF;
			break;
		}

		case 2:
			if (unit == NULL) {
				State = 0;
				LastScanFrame = Unsorted::CurrentFrame;
				break;
			}
			if (unit->NavCom != NULL) break;
			if (unit->Try_To_Deploy()) {
				State = 4;
			} else {
				State = 3;
				RespawnTimer = CDTimerClass<FrameTimerClass>(30);
			}
			LastScanFrame = 0x7FFFFFFF;
			break;

		case 3:
			if (unit == NULL) {
				State = 0;
				LastScanFrame = Unsorted::CurrentFrame;
				break;
			}
			State = unit->Try_To_Deploy() ? 4 : 1;
			LastScanFrame = 0x7FFFFFFF;
			break;

		case 4:
			if (building != NULL && building->BState != BSTATE_CONSTRUCTION) {
				State = 5;
				LastScanFrame = 0x7FFFFFFF;
				break;
			}
			if (unit != NULL && !unit->IsDeploying && unit->CurrentMission == MISSION_GUARD) {
				State = 2;
				LastScanFrame = 0x7FFFFFFF;
			}
			break;

		case 5: {
			if (unit != NULL || Owner == NULL) {	// NATIVE: the null owner guard
				State = 0;
				LastScanFrame = Unsorted::CurrentFrame;
				break;
			}
			Cell cell = Owner->ScanForTiberium(Rule->SlaveMinerShortScan / 256, false);
			if (cell == CELL_NONE) {
				Cell field = Owner->ScanForTiberium(Rule->SlaveMinerLongScan / 256, true);
				Cell spot = FUN_006B0300(field);
				Cell d1(spot.X - field.X, spot.Y - field.Y);
				Cell here = Owner->Get_Cell();
				Cell d2(here.X - field.X, here.Y - field.Y);
				short to_spot = (short)(int)fastmath::sqrt((double)d1.X * d1.X + (double)d1.Y * d1.Y);
				short to_here = (short)(int)fastmath::sqrt((double)d2.X * d2.X + (double)d2.Y * d2.Y);
				if (Rule->SlaveMinerScanCorrection / 256 + to_spot < to_here) {
					Owner->unknown_bool_4F8 = true;
					Owner->Assign_Archive_Target(Owner->Get_Cell_Ptr());
					Owner->QueueMission(MISSION_DECONSTRUCTION, false);
					State = 6;
					LastScanFrame = 0x7FFFFFFF;
				}
			}
			if (State != 6) {
				FUN_006B04C0();
			}
			break;
		}

		case 6:
			if (unit != NULL) {
				FUN_006B0490();
				State = 1;
				LastScanFrame = 0x7FFFFFFF;
			}
			break;

		default:
			break;
	}
}
