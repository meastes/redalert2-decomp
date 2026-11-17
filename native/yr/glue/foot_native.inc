
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y4), appended to the foot unit: FootClass::Active_Click_With
// (ActionType, ObjectClass *, bool), YR 0x004D74E0 (2,155 bytes, unmatched in ra2-decomp).
// TibSun's text (TibSun archive code/foot.cpp; SPDX GPL-3.0-or-later, Copyright 2025 Electronic
// Arts Inc. and OpenTS contributors, EA's GPL section 7 terms; THIRD_PARTY.md) with gamemd.exe's
// cases and changes, read from its code (the same case table as the RA2 twin, src/recovered/foot.cpp
// 0x004C61A0) and marked YR: the berserk and rocker-link refusals the cell variant also makes, the
// unload, detonate and attack-move actions, the move order's original cell. Behaviour code for the
// native build only.
// ---------------------------------------------------------------------------------------------
#include "bomb.h"

bool FootClass::Active_Click_With(ActionType action, ObjectClass * object, bool is_waypoint)
{
	/*
	**	YR: as in Active_Click_With(Cell): a berserk unit, or one paired by a rocker link that
	**	is not pushy, takes no orders.
	*/
	if (TechnoClass::Berzerk) {
		return(false);
	}
	if (DirectRockerLinkedUnit != NULL && !Techno_Type_Class()->Pushy) {
		return(false);
	}

	switch ((int)action) {
		case ACTION_GUARD_AREA:
			if (Can_Player_Fire() && Can_Player_Move()) {
				if (((Fetch_RTTI() == RTTI_INFANTRY &&
						((InfantryClass *)this)->Class->IsBomber) || Has_Ability((AbilityType)14)) &&
						object->Fetch_RTTI() == RTTI_BUILDING &&
						!House->Is_Ally(object)) {

					Player_Assign_Mission(MISSION_SABOTAGE, NULL, object);
				} else {
					Player_Assign_Mission(MISSION_GUARD_AREA, object);
				}
				return(true);
			}
			break;

		case ACTION_SELF:
		case ACTION_AREA_ATTACK:		// YR: 52 and 56 unload as well
		case ACTION_DETONATE_ALL:
			Player_Assign_Mission(MISSION_UNLOAD);
			return(true);

		case ACTION_DETONATE:		// YR: the attached bomb's owner detonates it
			OutList.Add(EventClass(AttachedBomb->OwnerHouse->HeapID, EventClass::DETONATE, TargetClass(this)));
			return(true);

		case ACTION_ATTACK:
		case ACTION_IVAN_BOMB:		// YR: 53, 63, 64 and 71 attack too
		case ACTION_ATTACK_MOVE_TAR:
		case ACTION_DEMOLISH:
		case 71:
			if (Can_Player_Fire()) {
				Player_Assign_Mission(MISSION_ATTACK, object);
				return(true);
			}
			break;

		case ACTION_ENTER:
			if (Can_Player_Move() && object && Dynamic_Cast<TechnoClass *>(object)) {
				Player_Assign_Mission(MISSION_ENTER, NULL, object);
				return(true);
			}
			break;

		case ACTION_CAPTURE:
			if (Can_Player_Move()) {
				Player_Assign_Mission(MISSION_CAPTURE, NULL, object);
				return(true);
			}
			break;

		case ACTION_EATEN:		// YR: 11
			if (Can_Player_Move()) {
				Player_Assign_Mission(MISSION_EATEN, NULL, object);
				return(true);
			}
			break;

		case ACTION_SABOTAGE:
			if (Can_Player_Move()) {
				Player_Assign_Mission(MISSION_SABOTAGE, NULL, object);
				return(true);
			}
			break;

		case ACTION_NOMOVE:
			if (Map.Is_Shrouded(object->Center_Coord()) && !TClass->IsMoveToShroud) {
				return(false);
			}
			/// intentional fallthrough
		case ACTION_MOVE:
		case ACTION_ATTACK_MOVE_NAV:
			if (Can_Player_Move()) {

				AbstractClass * targ = object;

				Coord center = object->Center_Coord();
				if (center.Z <= Map.Get_Height_GL(center)) {
					center.Z = Map.Get_Height_GL(center);
				}

				if (((TClass->IsSubterranean && !Rule->IsShroudedSubteranneanMovesAllowed) || Fetch_RTTI() == RTTI_AIRCRAFT) && Map.Is_Shrouded(center)) return(true);

				MZoneType mzone = TClass->MZone;
				Cell cell = object->Destination_Coord().As_Cell();
				bool in_radar = Map.In_Local_Radar(cell);
				CellClass * cptr = &Map[Destination_Coord()];

				if (mzone == MZONE_SUBTERANNEAN) mzone = MZONE_NORMAL;
				else if (mzone == MZONE_FLYER) mzone = MZONE_INFANTRY;

				bool moveanywhere = In_Air() || TClass->IsSubterranean || (Fetch_RTTI() == RTTI_INFANTRY && ((InfantryClass*)this)->Class->JumpJet) || Fetch_RTTI() == RTTI_AIRCRAFT;

				bool ontobridge = Is_Moving_Onto_Bridge();
				Coord coord = cell.As_Coord();
				coord.Z = Map.Get_Height_GL(coord);
				bool shrouded = Map.Is_Shrouded(coord);
				bool forcemove = !is_waypoint && (Keyboard->Down(Options.KeyForceMove1) || Keyboard->Down(Options.KeyForceMove2));

				if (moveanywhere && action == ACTION_NOMOVE) {
					Cell nearby = Map.Nearby_Location(cell, TClass->Speed, -1, mzone, Map[cell].IsUnderBridge, Point2D(1, 1), false, true, TClass->IsSubterranean && HeightAGL < 0, true);
					if (nearby != CELL_NONE) {
						targ = &Map[nearby];
					}
				} else {
					/*
					**	If the destination object is not the same zone, then pick a nearby location.
					*/
					if (!in_radar || !moveanywhere && (shrouded || action == ACTION_NOMOVE || forcemove && Fetch_RTTI() == RTTI_INFANTRY || !Map.Is_Same_Cell_Zone(cptr->CellID, cell, mzone, ontobridge, Map[cell].IsUnderBridge, false))) {
						Cell nearby = Map.Nearby_Location(cell, TClass->Speed, Map.Get_Cell_Zone(&cptr->CellID, mzone, ontobridge), mzone, Map[cell].IsUnderBridge, Point2D(1, 1), false, true, false, true);
						if (nearby != CELL_NONE) {
							targ = &Map[nearby];
						}
					}
				}

				if (targ != NULL) {
					/*
					**	YR: the order carries the clicked object's cell when it moves elsewhere.
					*/
					CellClass * from = NULL;
					if (targ != object && object != NULL) {
						from = &Map[object->Center_Coord().As_Cell()];
					}
					Player_Assign_Mission(MISSION_MOVE, NULL, targ, from);
				}
				return(true);
			}
			break;

		case ACTION_PATROL_WAYPOINT:
		case ACTION_NO_IVAN_BOMB:
			return(false);

		case ACTION_ATTACK_SUPPORT: {
				WeaponDataStruct const * wdata = Get_Class_Weapon_Data(0);
				bool heals = (wdata != 0 && wdata->Weapon != 0 && wdata->Weapon->Attack < 0);
				Player_Assign_Mission(heals ? MISSION_GUARD_AREA : MISSION_GUARD, this, NULL);
			}
			return(true);

		default:
			return(true);
	}
	return(false);
}

// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y10), YR 0x004E02C0 (13 bytes, unmatched in ra2-decomp): the
// ILocomotion smart pointer's _Release, which FootClass::Load calls on its Locomotion before the
// saved locomotor is loaded: the interface's Release when it holds one (gamemd.exe: mov eax,[ecx];
// test; call [vtable+8]). Behaviour code for the native build only.
// ---------------------------------------------------------------------------------------------
void __fastcall FUN_004E02C0(void * loco)
{
	IUnknown * pointer = *(IUnknown **)loco;
	if (pointer != NULL) {
		pointer->Release();
	}
}
