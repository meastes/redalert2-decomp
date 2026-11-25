/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/

#include "always.h"
#include "geometry.h"


#include "teleport.h"
#include "map.h" // RA2
#include "cell.h" // RA2
#include "overtype.h" // RA2
#include "voc.h" // RA2
#include "unittype.h" // RA2
#include "unit.h" // RA2
#include "parasite.h" // RA2
#include "bullet.h" // RA2
#include "anim.h" // RA2
#include "_map.h"
#include "_rules.h" // RA2
#include "rules.h" // RA2

#include "foot.h"
#include "globals.h"


/// <summary>
/// Creates a teleport locomotor.
/// The locomotor starts out idle, with no destination waiting to be jumped to.
/// </summary>
TeleportLocomotionClass::TeleportLocomotionClass(void) :
	BASECLASS(),
	DestinationCoord(COORD_NONE),
	LastCoords(COORD_NONE),
	IsMoving(false),
	Unused_35(false),
	Unused_36(false),
	State(0),
	Timer(),
	Piggybacker(NULL)
{
}


/// <summary>
/// Is a teleport pending?
/// The object counts as moving from the moment a destination is handed to this
/// locomotor until the jump has actually been made.
/// </summary>
boolean STDMETHODCALLTYPE TeleportLocomotionClass::Is_Moving(void)
{
	if (IsMoving == true) {
		return(true);
	}
	return(false);
}


/// <summary>
/// Is the object standing still?
/// This is the plain opposite of Is_Moving. An object with a teleport ordered counts as
/// being on the move even though it has not gone anywhere yet.
/// </summary>
boolean TeleportLocomotionClass::Is_Stationary(void)
{
	if (Is_Moving() == false) {
		return(true);
	}
	return(false);
}


/// <summary>
/// Fetches the location this locomotor is bound for.
/// </summary>
/// <returns>Returns with the pending teleport destination, or with the object's current
/// position if no teleport has been ordered.</returns>
CoordStruct STDMETHODCALLTYPE TeleportLocomotionClass::Destination(void)
{
	if (Is_Moving()) {
		return(DestinationCoord);
	}
	return(LinkedTo->PositionCoord);
}


/// <summary>
/// Orders the object to teleport to the location specified.
/// The jump is not made here. It happens the next time this locomotor is processed.
/// </summary>
/// <param name="to">The coordinate to teleport the object to.</param>
void STDMETHODCALLTYPE TeleportLocomotionClass::Move_To(CoordStruct to)
{
	if (LinkedTo->IsUnderEMP() || LinkedTo->IsParalyzed() || LinkedTo->IsBeingWarpedOut() || LinkedTo->IsWarpingIn()) {
		LinkedTo->NavCom = 0;
		return;
	}
	Coord coord = Coord(to.X, to.Y, to.Z);
	if (LinkedTo->UnlimboingInfantry && LinkedTo->Fetch_RTTI() == RTTI_INFANTRY) {
		CellClass & cell = Map[coord];
		cell.Incoming(COORD_NONE, true, true, false);
	}
	Pick_Arrival(coord);
	if (LastCoords != COORD_NONE) {
		IsMoving = true;
		DestinationCoord = LastCoords;
	} else {
		LinkedTo->Assign_Destination(0, true);
	}
}



/// <summary>
/// Cancels any teleport that has been ordered.
/// The pending destination is forgotten, so the object stays where it is rather than
/// making the jump.
/// </summary>
void STDMETHODCALLTYPE TeleportLocomotionClass::Stop_Moving(void)
{
	DestinationCoord = COORD_NONE;
	IsMoving = false;
	Unused_36 = false;
}

/// <summary>

/// (infantry on the very spot, any other foot object) - or the warping object is, against an iron curtained
/// occupant; a building, an impassable overlay or an unusable bridge makes the warp go to the nearest free cell
/// instead (ChronoDestCoords keeps the offset within it; false). Arriving moves the occupation bits to
/// ChronoDestCoords, on the bridge when there is one.
/// </summary>
bool TeleportLocomotionClass::Clear_Arrival(Coord to, bool arrive)
{
	if (!arrive) {
		ObjectClass * obj;
		if (Map[to].IsUnderBridge) {
			obj = Map[to].Cell_Bridge_Occupier();
		} else {
			obj = Map[to].Cell_Occupier();
		}
		int overlay = Map[to].Overlay;
		bool skip = false;
		while (obj != NULL) {
			if (!obj->IsIronCurtained() && obj->Fetch_RTTI() == RTTI_INFANTRY && LinkedTo->Fetch_RTTI() == RTTI_INFANTRY) {
				if (to == obj->Center_Coord()) {
					int damage = obj->Techno_Type_Class()->MaxStrength;
					obj->Take_Damage(damage, 0, Rule->C4Warhead, NULL, true, false);
				}
			} else if (!obj->IsIronCurtained() && obj->Is_Foot()) {
				int damage = obj->Techno_Type_Class()->MaxStrength;
				obj->Take_Damage(damage, 0, Rule->C4Warhead, NULL, true, false);
			} else if (obj->IsIronCurtained()) {
				int damage = LinkedTo->Techno_Type_Class()->MaxStrength;
				LinkedTo->Take_Damage(damage, 0, Rule->C4Warhead, NULL, true, false);
			} else if (!obj->Is_Foot()) {
				skip = true;
			}
			obj = obj->Next;
		}

		if (Map[to].IsUnderBridge) {
			if (!Map[to].IsBridgeTraversable) {
				skip = true;
			}
		}

		CellClass * cptr = &Map[LinkedTo->Center_Coord().As_Cell()];
		bool bridge = cptr->Is_Bridge_Here;
		if ((overlay != -1 && !OverlayTypes[overlay]->IsTiberium && !bridge) || skip) {
			MZoneType mzone = LinkedTo->Techno_Type_Class()->MZone;
			if (mzone == MZONE_FLYER) {
				mzone = MZONE_NORMAL;
			} else if (mzone == MZONE_DESTROYER) {
				mzone = MZONE_NORMAL;
			} else if (mzone == MZONE_AMPHIBIOUS_DESTROYER) {
				mzone = MZONE_AMPHIBIOUS;
			}
			Cell nearby = Map.Nearby_Location(to.As_Cell(), SPEED_TRACK, Map.Get_Cell_Zone(cptr->CellID, mzone, bridge), mzone, Map[to.As_Cell()].IsUnderBridge, Point2D(1, 1), false, false, false, true, Cell(0, 0));
			Coord offset = to - Map[to].Center_Coord();
			Coord target = Map[nearby].Center_Coord() + offset;
			LinkedTo->ChronoDestCoords = target;
			return(false);
		}
	} else {
		if (LastCoords != COORD_NONE) {
			LinkedTo->Clear_Occupy_Bit(LastCoords);
		} else {
			LinkedTo->Clear_Occupy_Bit(Coord(LinkedTo->Get_Coord()));
		}
		LastCoords = LinkedTo->ChronoDestCoords;
		LastCoords.Z = Map.Get_Height_GL(LastCoords);
		if (Map[LastCoords].IsUnderBridge && !LinkedTo->IsOnBridge) {
			LinkedTo->IsOnBridge = true;
			LastCoords.Z += BRIDGE_LEPTON_HEIGHT;
		} else {
			LinkedTo->IsOnBridge = false;
		}
		LinkedTo->Set_Occupy_Bit(LastCoords);
	}
	if (LastCoords != COORD_NONE) {
		LinkedTo->Set_Occupy_Bit(LastCoords);
	} else {
		LinkedTo->Set_Occupy_Bit(Coord(LinkedTo->Get_Coord()));
	}
	return(true);
}




/// <summary>

/// on a bridge or road it cannot enter) sinks; anything else that cannot stand there is destroyed.
/// </summary>
void TeleportLocomotionClass::Check_Landing(Coord to)
{
	for (ObjectClass * object = Map[to].Cell_Occupier(); object != NULL; object = object->Next) {
		if (object->IsIronCurtained()) {
			int damage = LinkedTo->Techno_Type_Class()->MaxStrength;
			LinkedTo->Take_Damage(damage, 0, Rule->C4Warhead, NULL, true, false);
		}
	}
	bool kill = false;
	if (LinkedTo->Techno_Type_Class()->Naval && (Map[to].IsUnderBridge || Map[to].Land_Type() == LAND_ROAD)) {
		kill = true;
	}
	if (Map[to.As_Cell()].Land_Type() == LAND_WATER && !LinkedTo->Techno_Type_Class()->Naval && LinkedTo->Fetch_RTTI() != RTTI_INFANTRY
		&& !Map[to].IsUnderBridge && Map[to].Land_Type() != LAND_ROAD) {
		LinkedTo->IsSinking = true;
		LinkedTo->Stun();
		return;
	}
	if (LinkedTo->Can_Enter_Cell(&Map[to.As_Cell()], FACING_NONE, -1, NULL, true) == MOVE_NO || kill) {
		if (!Map[to.As_Cell()].Is_Tile_With_Water() || LinkedTo->Fetch_RTTI() == RTTI_INFANTRY) {
			int damage = LinkedTo->Techno_Type_Class()->MaxStrength;
			LinkedTo->Take_Damage(damage, 0, Rule->C4Warhead, NULL, true, false);
		} else {
			LinkedTo->IsSinking = true;
			LinkedTo->Stun();
		}
	}
}



/// <summary>

/// (the occupied one when the target it is entering or capturing is there), else the centre of the nearest cell it
/// can enter. The occupation bits move with it. False when there is nowhere to arrive.
/// </summary>
bool TeleportLocomotionClass::Pick_Arrival(Coord const & coord)
{
	if (LastCoords != COORD_NONE) {
		LinkedTo->Clear_Occupy_Bit(LastCoords);
	} else {
		LinkedTo->Clear_Occupy_Bit(LinkedTo->PositionCoord);
	}

	Coord crd = coord;

	if (coord != COORD_NONE) {
		bool isbridge = Map[crd].IsUnderBridge && LinkedTo->PositionCoord.Z > 3 * LEVEL_LEPTON_H + Map.Get_Height_GL(crd);

		if (LinkedTo->Fetch_RTTI() == RTTI_INFANTRY) {
			bool any_spot = false;
			if (LinkedTo->Get_Mission() == MISSION_CAPTURE || LinkedTo->Get_Mission() == MISSION_EATEN || LinkedTo->Get_Mission() == MISSION_ENTER || LinkedTo->Get_Mission() == MISSION_PATROL) {
				ObjectClass * unit = (ObjectClass *)LinkedTo->NavCom;
				unit = (unit != NULL && unit->Fetch_RTTI() == RTTI_UNIT) ? unit : NULL;
				if (unit != NULL && unit->Get_Cell() == coord.As_Cell()) {
					any_spot = true;
				}
				ObjectClass * aircraft = (ObjectClass *)LinkedTo->NavCom;
				aircraft = (aircraft != NULL && aircraft->Fetch_RTTI() == RTTI_AIRCRAFT) ? aircraft : NULL;
				if (aircraft != NULL && aircraft->Get_Cell() == coord.As_Cell()) {
					any_spot = true;
				}
				BuildingClass * building = (BuildingClass *)LinkedTo->NavCom;
				building = (building != NULL && ((AbstractClass *)building)->Fetch_RTTI() == RTTI_BUILDING) ? building : NULL;
				if (building != NULL && Map[coord].Cell_Building() == building) {
					any_spot = true;
				}
			}

			Coord old = LastCoords;
			LastCoords = Map[crd].Closest_Free_Spot(crd, any_spot, isbridge);
			if (LinkedTo->Can_Enter_Cell(&Map[LastCoords.As_Cell()])) {
				LastCoords = COORD_NONE;
			}
			Map[old].Center_Coord();

			if (LastCoords == COORD_NONE) {
				ObjectClass * obj = (ObjectClass *)LinkedTo->NavCom;
				if (obj != NULL && (obj->AbstractFlags & ABSTRACT_OBJECT)) {
					MZoneType mzone = LinkedTo->Techno_Type_Class()->MZone;
					Cell navcell = obj->Destination_Coord().As_Cell();
					CellClass * cellptr = &Map[LinkedTo->Destination_Coord()];
					bool onbridge = LinkedTo->Is_Moving_Onto_Bridge();
					Cell nearcell = Map.Nearby_Location(navcell, LinkedTo->Techno_Type_Class()->Speed, Map.Get_Cell_Zone(cellptr->CellID, mzone, onbridge), mzone, Map[navcell].IsUnderBridge, Point2D(1, 1), false, true);
					if (nearcell != CELL_NONE) {
						LastCoords = Map[nearcell].Closest_Free_Spot(Map[nearcell].Center_Coord(), any_spot, isbridge);
					}
				}
			}
		} else if (LinkedTo->IsImmobilized) {
			LinkedTo->Set_Occupy_Bit(LastCoords);
		} else {
			MZoneType mzone = LinkedTo->Techno_Type_Class()->MZone;
			Cell cell = coord.As_Cell();
			if (cell != CELL_NONE && LinkedTo->Can_Enter_Cell(&Map[cell])) {
				int zone = Map.Get_Cell_Zone(cell, mzone, isbridge);
				cell = Map.Nearby_Location(cell, LinkedTo->Techno_Type_Class()->Speed, zone, mzone, isbridge);
			}
			if (cell != CELL_NONE) {
				LinkedTo->Clear_Occupy_Bit(LastCoords);
				LastCoords = Coord(Map[cell].Center_Coord().As_Cell());
				LastCoords.Z = Map.Get_Height_GL(LastCoords);
				LinkedTo->Set_Occupy_Bit(LastCoords);
			}
		}
	} else {
		LastCoords = coord;
	}

	if (LastCoords != COORD_NONE) {
		LinkedTo->Set_Occupy_Bit(LastCoords);
		return(true);
	}

	LinkedTo->Set_Occupy_Bit(LinkedTo->PositionCoord);
	return(false);
}


/// <summary>

/// </summary>
void STDMETHODCALLTYPE TeleportLocomotionClass::Do_Turn(DirStruct coord)
{
	LinkedTo->PrimaryFacing.Set_Desired(coord);
}


/// <summary>
/// Performs any pending teleport.
/// This routine is called by the owning object's movement processing. The object is
/// lifted off the map, set down at its destination, and made to look around from where
/// it now stands. The whole journey is over by the time this routine returns.
/// </summary>
/// <returns>bool; Is there more movement still to process? A teleport never leaves any.</returns>
boolean STDMETHODCALLTYPE TeleportLocomotionClass::Process(void)
{
	int distance = 0;
	if (LinkedTo->WarpingOut && State == 0 && LinkedTo->TeleportState == 0) {
		Process_Delay();
		return(false);
	}
	if (State == 0 && LinkedTo->TeleportState != 0) {
		State = LinkedTo->TeleportState;
		return(false);
	}
	if (!LinkedTo->IsImmobilized) {
		if (State <= 0) {
			if (Is_Moving()) {
				if (LinkedTo->PositionCoord != LastCoords && LastCoords != COORD_NONE) {
					TechnoClass::Remove_Target(LinkedTo);
					for (int index = Bullets.Count() - 1; index >= 0; index--) {
						BulletClass * bullet = Bullets[index];
						if (bullet->TarCom == LinkedTo) {
							bullet->LoseTarget();
						}
					}
					new AnimClass(Rule->WarpOut, LinkedTo->PositionCoord, 0, 1, ShapeFlags_Type(SHAPE_CENTER | SHAPE_WIN_REL), 0);
					distance = LinkedTo->Center_Coord().Distance_YZX(LastCoords);
					Timer = 0;
					if (Rule->ChronoTrigger) {
						Timer = distance / Rule->ChronoDistanceFactor;
					}
					Timer = Timer > Rule->ChronoMinimumDelay ? Timer : CDTimerClass<FrameTimerClass>(Rule->ChronoMinimumDelay);
					if (distance < Rule->ChronoRangeMinimum) {
						Timer = Rule->ChronoMinimumDelay;
					}
					LinkedTo->WarpingOut = true;
					if (LinkedTo->Fetch_RTTI() == RTTI_UNIT && ((UnitClass *)LinkedTo)->Class->IsToHarvest) {
						Timer = 0;
						LinkedTo->WarpingOut = false;
					}
					if (LinkedTo->ParasiteEatingMe != NULL) {
						LinkedTo->ParasiteEatingMe->ParasiteImUsing->ExitUnit();
					}
					LinkedTo->Mark(MARK_UP);
					{
						FootClass * linked = LinkedTo;
						int sound = linked->Techno_Type_Class()->ChronoOutSound;
						if (sound != VOC_NONE || (sound = Rule->ChronoOutSound) != VOC_NONE) {
							VocClass::PlayAt(sound, linked->PositionCoord, NULL);
						}
					}
					LinkedTo->Set_Coord(LastCoords);
					Cell where = LastCoords.As_Cell();
					CellClass * cell = &Map[where];
					LinkedTo->Set_Coord(LastCoords);
					if (cell->IsUnderBridge) {
						LinkedTo->IsOnBridge = true;
					} else {
						LinkedTo->IsOnBridge = false;
					}
					LinkedTo->Set_Height_AGL(0);
					LinkedTo->Mark(MARK_DOWN);
					{
						FootClass * linked = LinkedTo;
						int sound = linked->Techno_Type_Class()->ChronoInSound;
						if (sound != VOC_NONE || (sound = Rule->ChronoInSound) != VOC_NONE) {
							VocClass::PlayAt(sound, linked->PositionCoord, NULL);
						}
					}
					LinkedTo->Per_Cell_Process(PCP_END);
					Stop_Moving();
					cell->Goodie_Check(LinkedTo);
					LinkedTo->Assign_Destination(0, true);
					new AnimClass(Rule->WarpOut, LinkedTo->PositionCoord, 0, 1, ShapeFlags_Type(SHAPE_CENTER | SHAPE_WIN_REL), 0);
					LinkedTo->TeleportState = 0;
					return(false);
				}
				LinkedTo->Assign_Destination(0, true);
				Stop_Moving();
				return(false);
			}
			return(false);
		}
	} else if (State == 0) {
		LinkedTo->BeingWarpedOut = true;
		Timer = 60;
		State++;
		return(false);
	}
	if (State == 1) {
		Process_Delay();
		return(false);
	}
	if (State == 2) {
		new AnimClass(Rule->WarpOut, LinkedTo->PositionCoord, 0, 1, ShapeFlags_Type(SHAPE_CENTER | SHAPE_WIN_REL), 0);
		LinkedTo->Mark(MARK_UP);
		{
			FootClass * linked = LinkedTo;
			int sound = linked->Techno_Type_Class()->ChronoOutSound;
			if (sound != VOC_NONE || (sound = Rule->ChronoOutSound) != VOC_NONE) {
				VocClass::PlayAt(sound, linked->PositionCoord, NULL);
			}
		}
		LinkedTo->WarpingOut = true;
		LinkedTo->IsImmobilized = false;
		LinkedTo->BeingWarpedOut = false;
		LinkedTo->IsOnBridge = false;
		bool ok = Clear_Arrival(LinkedTo->ChronoDestCoords, false);
		State++;
		if (ok) {
			State++;
			return(false);
		}
		return(false);
	}
	if (State == 3) {
		if (Clear_Arrival(LinkedTo->ChronoDestCoords, false)) {
			State++;
		}
		LinkedTo->ChronoLockRemaining = Rule->ChronoDelay;
		return(false);
	}
	if (State == 4) {
		Clear_Arrival(LinkedTo->ChronoDestCoords, true);
		LinkedTo->Set_Coord(LastCoords);
		LinkedTo->Set_Height_AGL(0);
		LinkedTo->Mark(MARK_DOWN);
		State++;
		return(false);
	}
	if (State == 5) {
		LinkedTo->Set_Coord(LastCoords);
		LinkedTo->Set_Height_AGL(0);
		LinkedTo->Mark(MARK_DOWN);
		{
			FootClass * linked = LinkedTo;
			int sound = linked->Techno_Type_Class()->ChronoInSound;
			if (sound != VOC_NONE || (sound = Rule->ChronoInSound) != VOC_NONE) {
				VocClass::PlayAt(sound, linked->PositionCoord, NULL);
			}
		}
		if (!Map.In_Local_Radar(LinkedTo->Get_Cell(), true)) {
			LinkedTo->IsLocked = false;
		}
		if (LinkedTo->TeleportState == 0) {
			Check_Landing(LastCoords);
		}
		if (LinkedTo->IsActive) {
			LinkedTo->Per_Cell_Process(PCP_END);
			Stop_Moving();
			LinkedTo->Assign_Archive_Target(0);
			LinkedTo->Assign_Destination(0, true);
			Timer = LinkedTo->ChronoLockRemaining;
			new AnimClass(Rule->WarpOut, LinkedTo->PositionCoord, 0, 1, ShapeFlags_Type(SHAPE_CENTER | SHAPE_WIN_REL), 0);
			State++;
			return(false);
		}
		return(false);
	}
	if (State == 6) {
		Process_Delay();
		return(false);
	}
	if (State == 7) {
		LinkedTo->WarpingOut = false;
		LinkedTo->Assign_Archive_Target(0);
		LinkedTo->Assign_Destination(0, true);
		IsMoving = false;
		LinkedTo->TeleportState = 0;
		State = 0;
	}
	return(false);
}



/// <summary>

/// </summary>
void STDMETHODCALLTYPE TeleportLocomotionClass::Process_Delay(void)
{
	if (Timer == 0) {
		LinkedTo->WarpingOut = false;
		if (State > 0) {
			State++;
		}
	}
}





/// <summary>
/// Fetches the class identifier of this locomotor.
/// This routine is used by the persistence system to record which locomotor was
/// written, so that the right one can be created when the save game is loaded.
/// </summary>
/// <param name="retval">Pointer to the class identifier to fill in.</param>
/// <returns>Returns with S_OK, or E_POINTER if no destination was supplied.</returns>
HRESULT STDMETHODCALLTYPE TeleportLocomotionClass::GetClassID(CLSID * retval)
{
	if (retval == NULL) return(E_POINTER);
	*retval = CLSID_TeleportLocomotion;
	return(S_OK);
}


/// <summary>
/// Loads this locomotor from the persist stream.
/// The base class reads the raw data back, and the locomotor is then reconstructed in
/// place so that it regains a usable virtual function table.
/// </summary>
/// <returns>Returns with the HRESULT of the load. S_OK if the locomotor was rebuilt.</returns>
HRESULT STDMETHODCALLTYPE TeleportLocomotionClass::Load(IStream * stream)
{
	HRESULT result = BASECLASS::Load(stream);
	if (SUCCEEDED(result)) {
		new (this) TeleportLocomotionClass(NoInitClass());

		bool haspiggy;
		stream->Read(&haspiggy, sizeof(haspiggy), NULL);

		if (haspiggy) {
			result = OleLoadFromStream(stream, IID_ILocomotion, (void **)&Piggybacker);
		}
	}
	return(result);
}



/// <summary>

/// </summary>
HRESULT STDMETHODCALLTYPE TeleportLocomotionClass::Save(IStream * stream, BOOL cleardirty)
{
	HRESULT result = BASECLASS::Save(stream, cleardirty);
	if (SUCCEEDED(result)) {
		bool haspiggy;
		if (Piggybacker != NULL) {
			haspiggy = true;
			stream->Write(&haspiggy, sizeof(haspiggy), NULL);
			IPersistStreamPtr ptr(Piggybacker);
			result = OleSaveToStream(ptr, stream);
		} else {
			haspiggy = false;
			stream->Write(&haspiggy, sizeof(haspiggy), NULL);
		}
	}
	return(result);
}















/// <summary>
/// RA2
/// </summary>
static inline void Chrono_Out_Sound(FootClass * object)
{
	int sound = object->Techno_Type_Class()->ChronoOutSound;
	if (sound != -1 || (sound = Rule->ChronoOutSound) != -1) {
		VocClass::PlayAt(sound, object->PositionCoord);
	}
}


/// <summary>
/// RA2
/// </summary>
static inline void Chrono_In_Sound(FootClass * object)
{
	int sound = object->Techno_Type_Class()->ChronoInSound;
	if (sound != -1 || (sound = Rule->ChronoInSound) != -1) {
		VocClass::PlayAt(sound, object->PositionCoord);
	}
}





/// <summary>
/// Fetches the display layer that the owning object belongs in.
/// A teleporting object is always on the ground. It never travels through the air on
/// the way to its destination, so it never rises out of the ground layer.
/// </summary>
/// <returns>Returns with the layer the object should be rendered in.</returns>
LayerType STDMETHODCALLTYPE TeleportLocomotionClass::In_Which_Layer(void)
{
	return(LAYER_GROUND);
}

/// <summary>

/// </summary>
HRESULT STDMETHODCALLTYPE TeleportLocomotionClass::QueryInterface(REFIID riid, LPVOID * ppvObject)
{
	HRESULT result = BASECLASS::QueryInterface(riid, ppvObject);

	if (result == E_NOINTERFACE) {
		if (riid == IID_IPiggyback) {
			*ppvObject = (IPiggyback *)this;
		}
		if (*ppvObject == NULL) {
			result = E_NOINTERFACE;
		} else {
			AddRef();
			result = S_OK;
		}
	}
	return(result);
}


/// <summary>

/// </summary>
HRESULT STDMETHODCALLTYPE TeleportLocomotionClass::Begin_Piggyback(ILocomotion * pointer)
{
	if (pointer == NULL) {
		return(E_POINTER);
	}
	if (Piggybacker == NULL) {
		Piggybacker = pointer;
		return(S_OK);
	}
	return(E_FAIL);
}


/// <summary>

/// </summary>
HRESULT STDMETHODCALLTYPE TeleportLocomotionClass::End_Piggyback(ILocomotion ** pointer)
{
	if (pointer == NULL) {
		return(E_POINTER);
	}
	if (Piggybacker != NULL) {
		*pointer = Piggybacker;
		Piggybacker.Detach();
		return(S_OK);
	}
	return(S_FALSE);
}



/// <summary>

/// </summary>
boolean STDMETHODCALLTYPE TeleportLocomotionClass::Is_Ok_To_End(void)
{
	if (!Is_Moving() && Piggybacker != NULL && !Unused_35 && !LinkedTo->IsImmobilized && State == 0) {
		return(true);
	}
	return(false);
}


/// <summary>

/// </summary>
HRESULT STDMETHODCALLTYPE TeleportLocomotionClass::Piggyback_CLSID(GUID * classid)
{
	if (classid == NULL) {
		return(E_POINTER);
	}

	if (Piggybacker != NULL) {
		IPersistPtr ptr(Piggybacker);
		if (ptr == NULL) {
			return(E_FAIL);
		}
		return(ptr->GetClassID(classid));
	}

	IPersistPtr ptr(this);
	if (ptr == NULL) {
		return(E_FAIL);
	}
	return(ptr->GetClassID(classid));
}


/// <summary>

/// </summary>
void STDMETHODCALLTYPE TeleportLocomotionClass::Mark_All_Occupation_Bits(int mark)
{
	if (mark == MARK_UP) {
		LinkedTo->Clear_Occupy_Bit((Coord)Head_To_Coord());
	}
}





















/// <summary>
/// Fetches the size of this locomotor for the save game file.
/// </summary>
/// <returns>Returns with the number of bytes this locomotor occupies.</returns>
int TeleportLocomotionClass::Fetch_Object_Size(void) const
{
	return(sizeof(*this));
}

