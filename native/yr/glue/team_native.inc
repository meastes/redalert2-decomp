// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y11): TeamClass::AI (gamemd.exe 0x006E9140, 3,896 bytes) is unmatched
// in the YR tree. Written from gamemd.exe's code with the RA2 twin's text as the guide (RA2 0x006B7AD0,
// src/recovered/team.cpp); the script step's handlers are the tree's own TMission_* functions, which
// gamemd.exe calls out of line (its jump table at 0x006E9F74, 65 cases). Mission 2 (TMISSION_BERZERK)
// does nothing in YR. Behaviour code for the native build only; never part of an exact build.
// ---------------------------------------------------------------------------------------------

void TeamClass::AI(void)
{
	if (Suspended) {
		if (SuspendTimer.Value() != 0) {
			return;
		}
		Suspended = false;
	}

	if (IsAltered) {
		if (!Recalc_Strength()) {
			return;
		}
	}

	if (IsMoving && IsUnderStrength) {
		Regroup();
	}

	if (!IsMoving && (IsFullStrength || IsForcedActive)) {
		Flag_Into_Action();
	}

	if (IsReforming || IsMoving || Zone == NULL || ClosestMember == NULL) {
		Calc_Center(Zone, ClosestMember);
	}

	/*
	**	Recruit the members still missing, unless an AI-less (human) house's team is under way.
	*/
	if (!IsMoving || (!IsFullStrength && Class->IsReinforcable)) {
		if (!House->FUN_0050B730() || !IsHasBeen) {
			for (int index = 0; index < Class->TaskForce->ClassCount; index++) {
				if (Quantity[index] < Class->TaskForce->Members[index].Quantity) {
					Recruit(index);
				}
			}
		}
	}

	/*
	**	A team with no members left is done once it has been active, or (multiplayer) once it
	**	stayed unfilled too long.
	*/
	if (Member == NULL) {
		if (IsHasBeen || (Session.Type != GAME_NORMAL && Frame - CreationFrame > Rule->DissolveUnfilledTeamDelay)) {
			if (IsLeaveMap) {
				for (int index = 0; index < LogicTags.Count(); index++) {
					if (LogicTags[index]->Spring(TEVENT_LEAVES_MAP)) {
						index--;
						if (LogicTags.Count() == 0) {
							break;
						}
					}
				}
			}
			delete this;
			return;
		}
	}

	bool warping = (Member != NULL && Member->IsWarpingIn());

	if (!IsMoving || IsReforming || IsUnderStrength || warping) {
		if (IsMoving) {
			IsReforming = !Coordinate_Regroup();
		} else {
			Coordinate_Move();
		}
		return;
	}

	bool first_time = false;
	if (IsNextMission) {
		first_time = true;
		IsNextMission = false;
		Script->Next_Mission();

		for (FootClass * member = Member; member != NULL; member = member->Member) {
			if (Class->TransportsReturnOnUnload && member->Techno_Type_Class()->MaxPassengers > 0) {
				if (member->ArchivedTarget != NULL) {
					FUN_004068E0("A Transport just recieved orders to go home after unloading\n");
				}
			} else {
				member->Assign_Archive_Target(NULL);
			}
		}

		if (!Script->Has_Missions_Remaining()) {
			delete this;
			return;
		}
		Assign_Mission_Target(NULL);
		Target = NULL;
	} else if (Target == NULL) {
		Target = MissionTarget;
	}

	if (TargetNotAssigned) {
		first_time = true;
		TargetNotAssigned = false;
		Assign_Mission_Target(NULL);
		Target = NULL;
	}

	TeamMissionClass mission = Script->Get_Current_Mission();
	switch ((int)mission.Mission) {
		case 0x00: TMission_ATTACK(&mission, first_time); break;
		case 0x01: TMission_ATT_WAYPT(&mission, first_time); break;
		case 0x02: break;	// TMISSION_BERZERK: nothing in YR.
		case 0x03: TMission_MOVE(&mission, first_time); break;
		case 0x04: TMission_MOVECELL(&mission, first_time); break;
		case 0x05: TMission_GUARD(&mission, first_time); break;
		case 0x06: TMission_LOOP(&mission, first_time); break;
		case 0x07: TMission_WIN(&mission, first_time); break;
		case 0x08: TMission_UNLOAD(&mission, first_time); break;
		case 0x09: TMission_DEPLOY(&mission, first_time); break;
		case 0x0A: TMission_HOUND_DOG(&mission, first_time); break;
		case 0x0B: TMission_DO(&mission, first_time); break;
		case 0x0C: TMission_SET_GLOBAL(&mission, first_time); break;
		case 0x0D: TMission_IDLE_ANIM(&mission, first_time); break;
		case 0x0E: TMission_LOAD(&mission, first_time); break;
		case 0x0F: TMission_SPY(&mission, first_time); break;
		case 0x10: TMission_PATROL(&mission, first_time); break;
		case 0x11: TMission_SCRIPT(&mission, first_time); break;
		case 0x12: TMission_TEAMCHANGE(&mission, first_time); break;	// deletes this team
		case 0x13: TMission_PANIC(&mission, first_time); break;
		case 0x14: TMission_CHANGE_HOUSE(&mission, first_time); break;
		case 0x15: TMission_SCATTER(&mission, first_time); break;
		case 0x16: TMission_GOTO_SHROUD(&mission, first_time); break;
		case 0x17: TMission_LOSE(&mission, first_time); break;
		case 0x18: TMission_PLAY_SPEECH(&mission, first_time); break;
		case 0x19: TMission_PLAY_SOUND(&mission, first_time); break;
		case 0x1A: TMission_PLAY_MOVIE(&mission, first_time); break;
		case 0x1B: TMission_PLAY_MUSIC(&mission, first_time); break;
		case 0x1C: TMission_REDUCE_TIBERIUM(&mission, first_time); break;
		case 0x1D: TMission_BEGIN_PRODUCTION(&mission, first_time); break;
		case 0x1E: TMission_FIRE_SALE(&mission, first_time); break;
		case 0x1F: TMission_SELF_DESTRUCT(&mission, first_time); break;
		case 0x20: TMission_ION_STORM_START(&mission, first_time); break;
		case 0x21: TMission_ION_STORM_END(&mission, first_time); break;
		case 0x22: TMission_CENTER_VIEWPOINT(&mission, first_time); break;
		case 0x23: TMission_RESHROUD(&mission, first_time); break;
		case 0x24: TMission_REVEAL(&mission, first_time); break;
		case 0x25: TMission_DESTROY_MEMBERS(&mission, first_time); break;
		case 0x26: TMission_CLEAR_GLOBAL(&mission, first_time); break;
		case 0x27: TMission_SET_LOCAL(&mission, first_time); break;
		case 0x28: TMission_CLEAR_LOCAL(&mission, first_time); break;
		case 0x29: TMission_UNPANIC(&mission, first_time); break;
		case 0x2A: TMission_FORCE_FACING(&mission, first_time); break;
		case 0x2B: TMission_FULLY_LOADED(&mission, first_time); break;
		case 0x2C: TMission_UNLOAD_TRUCK(&mission, first_time); break;
		case 0x2D: TMission_LOAD_TRUCK(&mission, first_time); break;
		case 0x2E: TMission_ATTACK_BUILDING_WITH_PROPERTY(&mission, first_time); break;
		case 0x2F: TMission_MOVETO_BUILDING_WITH_PROPERTY(&mission, first_time); break;
		case 0x30: TMission_SCOUT(&mission, first_time); break;
		case 0x31: TMission_SUCCESS(&mission, first_time); break;
		case 0x32: TMission_FLASH(&mission, first_time); break;
		case 0x33: TMission_PLAY_ANIM(&mission, first_time); break;
		case 0x34: TMission_TALK_BUBBLE(&mission, first_time); break;
		case 0x35: TMission_MOVE_TO_ENEMY_APPROACH(&mission, first_time); break;
		case 0x36: TMission_MOVE_TO_BASE_FRONT(&mission, first_time); break;
		case 0x37: TMission_IRON_CURTAIN(&mission, first_time); break;
		case 0x38: TMission_CHRONO_BUILDING(&mission, first_time); break;
		case 0x39: TMission_CHRONO_ATTACK(&mission, first_time); break;
		case 0x3A: TMission_MOVETO_FRIENDLY_BUILDING_WITH_PROPERTY(&mission, first_time); break;
		case 0x3B: TMission_ATT_WAYPT_BUILDING(&mission, first_time); break;
		// gamemd.exe: the member's vtable slots 0x33C, 0x344, 0x340, 0x348, 0x34C in that case order.
		case 0x3C: TMission_ENTER_GRINDER(&mission, first_time); break;
		case 0x3D: TMission_ENTER_TANK_BUNKER(&mission, first_time); break;
		case 0x3E: TMission_ENTER_BIO_REACTOR(&mission, first_time); break;
		case 0x3F: TMission_ENTER_BATTLE_BUNKER(&mission, first_time); break;
		case 0x40: TMission_GARRISON_STRUCTURE(&mission, first_time); break;
		default: break;
	}
}
