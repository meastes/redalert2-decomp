/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 * Copyright 2026 ra2-decomp contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/

#include "always.h"
#include "geometry.h" // RA2: the header statics, in the order of the unit's initializer table

#include "jumpjet.h"
#include "tracker.h" // RA2

#include "_map.h"
#include "_rules.h"
#include "building.h"
#include "cell.h"
#include "foot.h"
#include "globals.h"
#include "house.h"
#include "infantry.h" // RA2
#include "inline.h"
#include "ion.h"
#include "mouse.h"
#include "rules.h"
#include "unit.h" // RA2
#include "unittype.h" // RA2

#include "layer.hh"


/// <summary>
/// Creates a jumpjet locomotor.
/// The unit starts out sitting on the ground with no destination, turning at the rate the
/// rules call for.
/// </summary>
JumpjetLocomotionClass::JumpjetLocomotionClass(void) :
	BASECLASS(),
	HeadToCoord(COORD_NONE),
	IsMoving(false),
	CurrentState(GROUNDED),
	Facing(Rule->JumpjetTurnRate),
	CurrentSpeed(0.0),
	TargetSpeed(0.0),
	FlightLevel(0),
	CurrentWobble(0.0),
	IsLanding(false)
{
	Facing.Set_Desired(DirType(DIR_E));
	Facing.Set(DirType(DIR_E));
}



/// <summary>
/// Destroys the jumpjet locomotor.
/// </summary>
JumpjetLocomotionClass::~JumpjetLocomotionClass(void)
{
}





/// <summary>
/// Is the jumpjet under orders to move?
/// This asks whether the unit has a destination, not whether it happens to be in the air.
/// </summary>
/// <returns>bool; Does the jumpjet have somewhere to be?</returns>
boolean STDMETHODCALLTYPE JumpjetLocomotionClass::Is_Moving(void)
{
	return(IsMoving);
}


/// <summary>
/// Fetches the destination the jumpjet has been ordered to.
/// </summary>
/// <returns>Returns with the destination coordinate. Otherwise, COORD_NONE is
/// returned.</returns>
CoordStruct STDMETHODCALLTYPE JumpjetLocomotionClass::Destination(void)
{
	if (Is_Moving()) {
		return(HeadToCoord);
	} else {
		return(COORD_NONE);
	}
}


/// <summary>
/// Handles the per frame processing of the jumpjet.
/// This is the driver for the flight state machine, called by the owning object's AI. It
/// also looks after the bookkeeping that comes of a unit moving in and out of the shroud,
/// and resubmits the object to the map when its display layer changes. An ion storm will
/// bring down anything caught off the ground.
/// </summary>
boolean STDMETHODCALLTYPE JumpjetLocomotionClass::Process(void)
{
	LayerType layer = In_Which_Layer();

	if (Is_Moving() || Is_Moving_Now()) {
		Movement_AI();
		if (!LinkedTo->IsActive) {
			return(false);
		}

		if (LinkedTo->House->Ion_Sensitives_Should_Be_Offline()) { // RA2: a house call (0x51FC80) instead of the ion storm test; a flying unit crashes (not Take_Damage)
			if (CurrentState != GROUNDED && CurrentState != CRASHING) { // RA2: as above
				LinkedTo->Crash(NULL); // RA2: as above
			}
		}

		if (LinkedTo->IsRocking && CurrentState != CRASHING && CurrentState != UNKNOWN && LinkedTo->HeightAGL > 0) { // RA2: a rocking unit in the air starts to crash
			FlightLevel = -5; // RA2: as above
			CurrentState = CRASHING; // RA2: as above
			if (LinkedTo->Fetch_RTTI() == RTTI_INFANTRY) { // RA2: infantry start their tumble sequence
				((InfantryClass *)LinkedTo)->Do_Action(DO_AIR_DEATH_START, false, false); // RA2: as above
			}
		}

		switch (CurrentState) {
			case GROUNDED:
				Process_Grounded();
				break;

			case ASCENDING:
				Process_Ascent();
				break;

			case HOVERING:
				Process_Hover();
				break;

			case CRUISING:
				Process_Cruise();
				break;

			case DESCENDING:
				Process_Descent();
				break;

			case CRASHING: // RA2
				Process_Crashing(); // RA2
				break; // RA2

			case UNKNOWN: // RA2
				Process_Unknown(); // RA2: the empty state; its call is expanded to nothing, and the case keeps its own switch table entry
				break;

		}

		if (LinkedTo->IsSelected && LinkedTo->House != PlayerPtr) {
			if (Map.Is_Shrouded(LinkedTo->PositionCoord) || (Scen->Special.IsFogOfWar && Map.Is_Fogged(LinkedTo->PositionCoord))) {
				LinkedTo->Unselect();
			}
		}

		if (!LinkedTo->IsDiscoveredByPlayer && LinkedTo->House != PlayerPtr) {
			if (!Map.Is_Shrouded(LinkedTo->PositionCoord)) {
				LinkedTo->Revealed(PlayerPtr);
			}
		}
	}

	if (LinkedTo->IsActive && In_Which_Layer() != layer) { // RA2: the object may have been removed meanwhile
		Map.Submit(LinkedTo);
	}

	return(false);
}


/// <summary>
/// Assigns a new destination for the jumpjet to fly to.
/// The destination is nudged to a nearby spot the unit can actually settle into, and any
/// landing reservation it was holding is given up. A unit already on its way down will climb
/// back to cruise height to make the trip.
/// </summary>
/// <param name="to">The coordinate to fly to, or COORD_NONE to give the unit no
/// destination at all.</param>
void STDMETHODCALLTYPE JumpjetLocomotionClass::Move_To(CoordStruct to)
{
	if (HeadToCoord != COORD_NONE && CurrentState != GROUNDED && IsLanding) {
		LinkedTo->Clear_Occupy_Bit(HeadToCoord);
		IsLanding = false;
	}

	HeadToCoord = to;

	if ((Coord)to != COORD_NONE) {
		Cell cell; // RA2: the cell is searched for by two different calls
		if (LinkedTo->Fetch_RTTI() == RTTI_UNIT && !((UnitClass *)LinkedTo)->Class->BalloonHover) { // RA2: a unit that is not a balloon hoverer searches without the bridge test
			cell = Map.Nearby_Location(((Coord &)to).As_Cell(), LinkedTo->TClass->Speed, -1, MZONE_FLYER, false, Point2D(1, 1), false, false, false, false, Cell(0, 0), false); // RA2: as above, with underbridge cells not allowed
		} else {
			cell = Map.Nearby_Location(((Coord &)to).As_Cell(), LinkedTo->TClass->Speed, -1, MZONE_FLYER, Map[to].IsUnderBridge, Point2D(1, 1), false, false, false, true, Cell(0, 0), false); // RA2: as above
		}
		Coord free = Closest_Free_Spot(Coord(cell)); // RA2: the found cell is made a coordinate first
		if (free != COORD_NONE) {
			if (LinkedTo->Fetch_RTTI() != RTTI_UNIT || (!LinkedTo->TClass->BalloonHover && LinkedTo->MissionQueue != MISSION_ENTER && LinkedTo->Get_Mission() != MISSION_ENTER)) { // RA2: a unit that is entering something or hovers like a balloon keeps the destination it was given
				HeadToCoord = free; // RA2: as above
			}
			if (LinkedTo->MissionQueue != MISSION_ENTER && LinkedTo->Get_Mission() != MISSION_ENTER) { // RA2: as above
				LinkedTo->NavCom = &Map[HeadToCoord]; // RA2: as above
			}
			IsMoving = true;
			if (CurrentState == DESCENDING) {
				CurrentState = ASCENDING;
				FlightLevel = Height; // RA2: the locomotor's own Height (+0x28), not the rules value
			}
		}
	} else {
		IsMoving = false;
	}
}


/// <summary>
/// Halts the jumpjet at the nearest place it can land.
/// An airborne unit cannot simply stop where it is, so it is redirected to the closest spot
/// it could put down in. A unit with nowhere at all to land is destroyed rather than left
/// hanging in the air.
/// </summary>
void STDMETHODCALLTYPE JumpjetLocomotionClass::Stop_Moving(void)
{
	if (!ScenarioInit && IsMoving) { // RA2: Stop_Moving at 0x52F960 matches only so
		if (HeadToCoord != COORD_NONE && CurrentState != GROUNDED && IsLanding) {
			LinkedTo->Clear_Occupy_Bit(HeadToCoord);
			IsLanding = false;
		}
		Coord position = LinkedTo->Position; // RA2: as above
		Cell cell = position.As_Cell(); // RA2: as above
		Cell nearby; // RA2: as above
		nearby = Map.Nearby_Location(Cell(cell), LinkedTo->TClass->Speed, -1, LinkedTo->TClass->MZone, false, Point2D(1, 1), false, false, false, true); // RA2: as above
		if (nearby != CELL_NONE) {
			Coord nearby_coord = nearby.As_Coord();
			nearby_coord.Z = Map.Get_Height_GL(nearby_coord);
			if (Map[nearby].IsUnderBridge) {
				nearby_coord.Z += BRIDGE_LEPTON_HEIGHT;
			}
			Move_To(nearby_coord);
		} else {
			if (LinkedTo->Strength > 0) { // RA2: as above
				LinkedTo->Take_Damage(LinkedTo->Strength, 0, Rule->C4Warhead, NULL, true, true); // RA2: as above
				HeadToCoord = COORD_NONE; // RA2: as above
			}
		}
	}
}


/// <summary>
/// Sets the facing of the jumpjet.
/// The turn is immediate rather than gradual -- the flight states do their own steering
/// through the locomotor's own facing tracker.
/// </summary>
/// <param name="coord">The direction the unit should be facing.</param>
void STDMETHODCALLTYPE JumpjetLocomotionClass::Do_Turn(DirStruct coord)
{
	LinkedTo->PrimaryFacing.Set(coord);
}


/// <summary>
/// Fetches the class identifier of this locomotor.
/// The persistence machinery uses the identifier to build the right kind of locomotor back
/// when a save game is loaded.
/// </summary>
/// <returns>Returns with S_OK, or E_POINTER if there is nowhere to put the answer.</returns>
HRESULT STDMETHODCALLTYPE JumpjetLocomotionClass::GetClassID(CLSID * retval)
{
	if (retval == NULL) return(E_POINTER);
	*retval = CLSID_JumpjetLocomotion;
	return(S_OK);
}


/// <summary>
/// Loads this locomotor from a save game stream.
/// The base class reads the raw data back and this routine then re-establishes the object's
/// virtual table pointer so that it is fit to be called again.
/// </summary>
/// <returns>Returns with the result of the load, as an HRESULT.</returns>
HRESULT STDMETHODCALLTYPE JumpjetLocomotionClass::Load(IStream * stream)
{
	HRESULT result = BASECLASS::Load(stream);
	if (SUCCEEDED(result)) {
		new (this) JumpjetLocomotionClass(NoInitClass());
	}
	return(result);
}





/// <summary>
/// Determines which display layer the jumpjet belongs to.
/// The layer follows the unit's altitude, so it is checked every game frame and the object
/// is resubmitted to the map whenever the answer changes. A unit passing beneath a bridge is
/// measured from the bridge deck rather than from the ground.
/// </summary>
/// <returns>Returns with the layer this object should be drawn in.</returns>
LayerType STDMETHODCALLTYPE JumpjetLocomotionClass::In_Which_Layer(void)
{
	int height = LinkedTo->HeightAGL;
	if (!LinkedTo->IsOnBridge) {
		if (Map[LinkedTo->Get_Coord()].IsUnderBridge && height >= BRIDGE_LEPTON_HEIGHT && !LinkedTo->IsFalling) {
			height -= BRIDGE_LEPTON_HEIGHT;
		}
	}

	if (height == 0) {
		return(LAYER_GROUND);
	}
	return(height >= Height ? LAYER_TOP : LAYER_AIR); // RA2: compares with the locomotor's own Height (+0x28), not the rules value
}


/// <summary>
/// Handles the grounded flight state.
/// A landed unit that has been given somewhere to go spins up and takes off. An ion storm
/// overhead keeps it pinned to the ground instead.
/// </summary>
void JumpjetLocomotionClass::Process_Grounded(void)
{
	if (Is_Moving()) {
		LinkedTo->Set_Speed(1.0);
		Facing.Set(LinkedTo->PrimaryFacing.Current());
		CurrentSpeed = 0;
		TargetSpeed = 0;
		FlightLevel = Height; // RA2: the locomotor's own Height (+0x28), not the rules value
		if (!LinkedTo->House->Ion_Sensitives_Should_Be_Offline()) { // RA2: a house call (0x51FC80) instead of the ion storm test
			if (LinkedTo->GetLastFlightMapCoords() == CELL_NONE) { // RA2: files the unit in the aircraft tracker if it has no cell there yet
				AircraftTracker.Add(LinkedTo); // RA2: as above
			}
			CurrentState = ASCENDING;
		}
	}
}


/// <summary>
/// Handles the ascent flight state.
/// The unit climbs toward its flight level, picking up speed and turning toward its
/// destination once it is safely clear of the ground. Reaching the flight level puts it into
/// a hover.
/// </summary>
void JumpjetLocomotionClass::Process_Ascent(void)
{
	int height = LinkedTo->HeightAGL;
	if (!LinkedTo->IsOnBridge) {
		if (Map[LinkedTo->Get_Coord()].IsUnderBridge && height >= BRIDGE_LEPTON_HEIGHT) {
			height -= BRIDGE_LEPTON_HEIGHT;
		}
	}

	Cell cell = LinkedTo->Get_Cell(); // RA2: keeps the aircraft tracker's filing of the unit up to date
	if (cell != LinkedTo->LastFlightMapCoords) { // RA2: as above
		AircraftTracker.Update(LinkedTo, LinkedTo->LastFlightMapCoords, cell); // RA2: as above
	}

	if (height >= FlightLevel) {
		if (AircraftTracker.IsJumpjet(LinkedTo)) { // RA2: another jumpjet holds this cell's place, so move on to a random neighbouring cell
			int facing = Random_Pick(0, 7); // RA2: as above
			LinkedTo->Assign_Destination(&Map[cell].Adjacent_Cell((FacingType)facing), true); // RA2: as above
			CurrentState = CRUISING; // RA2: as above
		} else {
			LinkedTo->Get_Cell_Ptr()->TryAssignJumpjet(LinkedTo); // RA2: claims the cell's jumpjet place
			CurrentState = HOVERING; // RA2: as above
		}
		return; // RA2: as above
	}

	int limit; // RA2: the speed-up starts at half the flight level for a balloon hoverer unit, a quarter otherwise
	if (LinkedTo->Fetch_RTTI() == RTTI_UNIT && LinkedTo->Techno_Type_Class()->BalloonHover) { // RA2: as above
		limit = FlightLevel / 2; // RA2: as above
	} else {
		limit = FlightLevel / 4; // RA2: as above
	}
	if (height > limit) { // RA2: as above
		TargetSpeed = Speed; // RA2: the locomotor's own Speed (+0x1C), not the rules value
		Facing.Set_Desired(DirType().Direction(LinkedTo->PositionCoord, HeadToCoord));
		TechnoClass * contact = LinkedTo->Radio_Link(0); // RA2: a unit leaving a factory or barracks tells it that it has taken off
		if (contact != NULL && contact->Fetch_RTTI() == RTTI_BUILDING) { // RA2: as above
			BuildingTypeClass * type = ((BuildingClass *)contact)->Class; // RA2: as above
			if (type->IsWeaponsFactory || type->IsGDIBarracks || type->IsNODBarracks) { // RA2: as above
				LinkedTo->Unknown_Virtual_0(0x117B, 0); // RA2: as above
			}
		}
		if (cell == Cell(HeadToCoord.As_Cell()) && !LinkedTo->Techno_Type_Class()->BalloonHover) { // RA2: over its destination cell the flight level is the current height
			FlightLevel = LinkedTo->HeightAGL; // RA2: as above
			CurrentState = CRUISING; // RA2: as above
		}
	}
}


/// <summary>
/// RA2
/// </summary>
static inline void Release_Jumpjet(FootClass * const & foot)
{
	if (foot->Get_Cell_Ptr()->Jumpjet == foot) {
		foot->Get_Cell_Ptr()->TryAssignJumpjet(NULL);
	}
}


/// <summary>
/// Handles the hover flight state.
/// A hovering unit that has somewhere else to be turns toward it and cruises. One that has
/// arrived and has nothing left to shoot at begins its descent.
/// </summary>
void JumpjetLocomotionClass::Process_Hover(void)
{
	if (Is_Moving()) {
		Coord headto = HeadToCoord;
		Coord position = LinkedTo->PositionCoord;
		if (Point2D(headto) == Point2D(position)) {
			if (LinkedTo->TarCom == NULL) {
				if (LinkedTo->Techno_Type_Class()->BalloonHover) { // RA2: balloon hoverers stay up, the rest descend; both advance the planning path
					CurrentState = HOVERING; // RA2: as above
					LinkedTo->Next_Planning_Node(); // RA2: as above
				} else {
					CurrentState = DESCENDING; // RA2: as above
					LinkedTo->Next_Planning_Node(); // RA2: as above
				}
			}
		} else {
			Facing.Set_Desired(DirType().Direction(LinkedTo->PositionCoord, HeadToCoord));
			if (LinkedTo->Get_Cell_Ptr()->Jumpjet == NULL) { // RA2: gives up the jumpjet place of the cell it was last filed under, then the one of its own cell
				if (Map[LinkedTo->LastFlightMapCoords].Jumpjet == LinkedTo) { // RA2: as above
					Map[LinkedTo->LastFlightMapCoords].TryAssignJumpjet(NULL); // RA2: as above
				}
			}
			LinkedTo->Get_Cell_Ptr()->TryAssignJumpjet(NULL); // RA2: as above
			CurrentState = CRUISING; // RA2: as above
		}
	}
}


/// <summary>
/// Handles the cruise flight state.
/// The unit steers toward its destination and throttles back as it closes on it. Once there
/// it drops into a descent, or holds a hover instead while it still has something to shoot
/// at.
/// </summary>
void JumpjetLocomotionClass::Process_Cruise(void)
{
	Coord position = LinkedTo->PositionCoord;
	Cell cell = LinkedTo->Get_Cell(); // RA2: keeps the aircraft tracker's filing of the unit up to date
	if (cell != LinkedTo->LastFlightMapCoords) { // RA2: as above
		AircraftTracker.Update(LinkedTo, LinkedTo->LastFlightMapCoords, cell); // RA2: as above
	}
	DirType dir; // RA2: a named direction for the facing
	Facing.Set_Desired(dir.Direction(position, HeadToCoord)); // RA2: as above
	DirType difference = Facing.Difference(); // RA2: how far the facing still has to turn
	bool balloon = LinkedTo->Techno_Type_Class()->BalloonHover; // RA2: balloon hoverers keep their flight level

	int distance = Point2D(position).Distance_To(Point2D(HeadToCoord));
	if (distance < 20) {
		CurrentSpeed = 0;
		TargetSpeed = 0;
		position.X = HeadToCoord.X;
		position.Y = HeadToCoord.Y;
		bool down = LinkedTo->IsDown;
		LinkedTo->IsDown = false;
		LinkedTo->Set_Coord(position);
		LinkedTo->IsDown = down;
		if (LinkedTo->TarCom == NULL && !balloon) { // RA2: a balloon hoverer never descends here
			FlightLevel = 0;
			CurrentState = DESCENDING;
			LinkedTo->Next_Planning_Node(); // RA2: advances the planning path
		} else {
			if (AircraftTracker.IsJumpjet(LinkedTo)) { // RA2: another jumpjet holds the cell's place, so head for a random neighbouring cell
				int facing = Random_Pick(0, 7); // RA2: as above
				LinkedTo->Assign_Destination(&Map[cell].Adjacent_Cell((FacingType)facing), true); // RA2: as above
			} else {
				LinkedTo->Get_Cell_Ptr()->TryAssignJumpjet(LinkedTo); // RA2: claims the cell's jumpjet place
				CurrentState = HOVERING; // RA2: claims the cell and hovers, as in the ascent
			}
		}
	} else if (distance < Speed) { // RA2: the locomotor's own Speed (+0x1C), not the rules value
		TargetSpeed = Speed / 8; // RA2: integer division
		if (LinkedTo->TarCom == NULL) {
			FlightLevel = Height / 2; // RA2: the locomotor's own Height (+0x28)
		}
	} else if (distance < Speed * 2) { // RA2: as above
		TargetSpeed = Speed / 4; // RA2: integer division of the locomotor's Speed
		if (LinkedTo->TarCom == NULL) { // RA2: as above
			FlightLevel = Height / 2; // RA2: as above
		}
		if (difference.As_Dir256() > TurnRate) { // RA2: a sharp turn slows the unit to a tenth, at least 1
			TargetSpeed = Speed / 10; // RA2: as above
			if (TargetSpeed < 1.0) { // RA2: as above
				TargetSpeed = 1.0; // RA2: as above
			}
		}
	} else {
		if (distance < (Speed * 50) / TurnRate) { // RA2: close enough to start slowing down for the turn rate
			TargetSpeed = Speed / 2; // RA2: as above
			if (LinkedTo->TarCom == NULL) { // RA2: as above
				FlightLevel = (int)(Height * 0.75); // RA2: as above
			}
			if (difference.As_Dir256() > TurnRate * 5) { // RA2: as above
				TargetSpeed = Speed / 5; // RA2: as above
				if (TargetSpeed < 1.0) { // RA2: as above
					TargetSpeed = 1.0; // RA2: as above
				}
			}
		} else {
			FlightLevel = Height; // RA2: the locomotor's own Height
			TargetSpeed = Speed; // RA2: the locomotor's own Speed
		}
	}

	CellClass & destination = Map[HeadToCoord]; // RA2: over water or beach it keeps the full flight level
	if (balloon || destination.Land_Type() == LAND_WATER || destination.Land_Type() == LAND_BEACH) { // RA2: as above
		FlightLevel = Height; // RA2: as above
	}
}


/// <summary>
/// Handles the descent flight state.
/// The unit reserves the cell it is coming down into and settles onto it, becoming an
/// ordinary grounded object once it touches down. A landing spot that has been taken in the
/// meantime makes the unit give up and go looking for somewhere else to put down.
/// </summary>
void JumpjetLocomotionClass::Process_Descent(void)
{
	CellClass * cellptr = &Map[HeadToCoord];
	if (cellptr->Land_Type() == LAND_WATER || cellptr->Land_Type() == LAND_BEACH) { // RA2: no landing on water or beach unless the unit is entering something
		if (LinkedTo->MissionQueue != MISSION_ENTER && LinkedTo->Get_Mission() != MISSION_ENTER) { // RA2: as above
			if (AircraftTracker.IsJumpjet(LinkedTo)) { // RA2: another jumpjet holds the cell's place, so head for a random neighbouring cell
				int facing = Random_Pick(0, 7); // RA2: as above
				LinkedTo->Assign_Destination(&LinkedTo->Get_Cell_Ptr()->Adjacent_Cell((FacingType)facing), true); // RA2: as above
				CurrentState = CRUISING; // RA2: as above
			} else {
				LinkedTo->Get_Cell_Ptr()->TryAssignJumpjet(LinkedTo); // RA2: claims the cell's jumpjet place and hovers there at full height
				CurrentState = HOVERING; // RA2: as above
				FlightLevel = Height; // RA2: as above
			}
			return; // RA2: as above
		}
	}

	MoveType move = LinkedTo->Can_Enter_Cell(cellptr);
	int spot = CellClass::Spot_Index(HeadToCoord);
	bool landable; // RA2: the spot is free or already claimed; a unit may always land on the cell's centre spot
	if (cellptr->Is_Spot_Free(spot, cellptr->IsUnderBridge) || IsLanding) { // RA2: as above
		landable = true; // RA2: as above
	} else {
		landable = false; // RA2: as above
	}
	if (LinkedTo->Fetch_RTTI() == RTTI_UNIT && spot == 0) { // RA2: as above
		landable = true; // RA2: as above
	}

	if (LinkedTo->MissionQueue != MISSION_ENTER && LinkedTo->Get_Mission() != MISSION_ENTER) { // RA2: a unit entering something skips the landing checks
		if (!landable || (!Map[HeadToCoord].IsUnderBridge && (move > MOVE_MOVING_BLOCK || (move == MOVE_MOVING_BLOCK && !IsLanding)))) { // RA2: as above
			Stop_Moving();
			return;
		}
		if (!IsLanding) {
			IsLanding = true; // RA2: as above
			LinkedTo->Set_Occupy_Bit(HeadToCoord); // RA2: as above
		}
	}

	{
		FlightLevel = 0;

		int height = LinkedTo->HeightAGL;
		if (!LinkedTo->IsOnBridge) {
			if (Map[LinkedTo->Get_Coord()].IsUnderBridge && height >= BRIDGE_LEPTON_HEIGHT) {
				height -= BRIDGE_LEPTON_HEIGHT;
			}
		}

		if (height == 0) {
			LinkedTo->Set_Speed(0.0);
			LinkedTo->Mark(MARK_UP);
			LinkedTo->Set_Coord(HeadToCoord);

			if (LinkedTo->PositionCoord.Z > Map.Get_Height_GL(LinkedTo->PositionCoord)) {
				if (Map[LinkedTo->Get_Coord()].IsUnderBridge) {
					LinkedTo->IsOnBridge = true;
				}
			}

			LinkedTo->Mark(MARK_DOWN);
			HeadToCoord = COORD_NONE;
			IsMoving = false;
			LinkedTo->Per_Cell_Process(PCP_END);
			LinkedTo->Assign_Destination(NULL, true); // RA2: Assign_Destination comes after Per_Cell_Process

			if (LinkedTo->IsActive && !LinkedTo->IsInLimbo && !LinkedTo->IsFalling) { // RA2: no null test of LinkedTo
				LinkedTo->RemoveSight(0, 0, 0, 0); // RA2: takes back the sight, then reveals the surroundings again, instead of Look
				if (LinkedTo->IsActive) { // RA2: as above
					LinkedTo->UpdateSight(0, 0, 0, 0); // RA2: as above
				}
				if (LinkedTo->IsActive) { // RA2: as above
					Map.RevealArea3(LinkedTo->PositionCoord, LinkedTo->LastSightRange - 3, LinkedTo->LastSightRange + 3, 0); // RA2: as above
				}
				Release_Jumpjet(LinkedTo); // RA2: gives up the cell's jumpjet place and leaves the aircraft tracker
				AircraftTracker.Remove(LinkedTo); // RA2: as above
				CurrentState = GROUNDED;
				IsLanding = false;
			}
		}
	}
}



/// <summary>
/// RA2: handles the crashing state: the jumpjet sinks by JumpjetCrash, turning as it falls, and once on the ground it
/// gives up its cell's jumpjet claim, leaves the aircraft tracker, stops and is told so.
/// </summary>
void JumpjetLocomotionClass::Process_Crashing(void) // RA2: at 0x530940
{
	Coord coord = LinkedTo->Position;
	coord.Z -= Crash;
	{
		DirType dir = DirType((unsigned short)(Facing.Current().Raw + 0x2D00)); // RA2: as above
		Facing.Set_Desired(dir); // RA2: as above
	}
	Map.Get_Height_GL(coord);
	Map[coord]; // RA2: as above
	if (Map.In_Radar(coord.As_Cell())) {
		Map.Remove(LinkedTo);
		LinkedTo->Mark(MARK_UP);
		LinkedTo->Set_Coord(coord);
		LinkedTo->Mark(MARK_DOWN);
		Map.Submit(LinkedTo);
	}
	if (LinkedTo->HeightAGL <= 0) {
		if (((CellClass *)LinkedTo->Get_Cell_Ptr())->Jumpjet == LinkedTo) { // RA2: as above
			((CellClass *)LinkedTo->Get_Cell_Ptr())->TryAssignJumpjet(NULL); // RA2: as above
		}
		AircraftTracker.Remove(LinkedTo);
		CurrentState = UNKNOWN;
		TargetSpeed = 0; // RA2: as above
		LinkedTo->Unknown_Virtual_0(0x117C, 0); // RA2: as above
	}
}






/// <summary>
/// Handles the unknown flight state.
/// The state has a slot in the machine but asks for no processing of its own.
/// </summary>
void JumpjetLocomotionClass::Process_Unknown(void)
{
}


/// <summary>
/// Is the jumpjet under way at this instant?
/// A grounded or hovering unit is holding station. Anything else is in transit, whether or
/// not it has been given a destination.
/// </summary>
/// <returns>bool; Is the jumpjet in flight toward somewhere?</returns>
boolean STDMETHODCALLTYPE JumpjetLocomotionClass::Is_Moving_Now(void)
{
	if (CurrentState != GROUNDED && CurrentState != HOVERING) {
		return(true);
	}
	return(false);
}


/// <summary>
/// Handles the actual flying of the jumpjet.
/// This routine brings the unit up to its target speed, trims its altitude to suit the
/// terrain it is crossing, and then slides it along its current facing. Anything cloaked
/// underneath is made to shimmer as the unit passes over, and a firestorm wall is told when
/// it has been crossed.
/// </summary>
void JumpjetLocomotionClass::Movement_AI(void)
{
	bool need_to_mark = CurrentState != HOVERING && CurrentState != CRUISING;
	bool was_down = LinkedTo->IsDown;

	if (need_to_mark) {
		LinkedTo->Mark(MARK_UP);
	} else {
		LinkedTo->IsDown = false;
	}

	if (TargetSpeed > CurrentSpeed) {
		CurrentSpeed += Accel; // RA2: the locomotor's own Accel (+0x2C), not the rules value
		CurrentSpeed = MIN(CurrentSpeed, (double)Speed); // RA2: the locomotor's own Speed (+0x1C)
	}
	if (TargetSpeed < CurrentSpeed) {
		CurrentSpeed -= Accel * 1.5; // RA2: as above
		CurrentSpeed = MAX(CurrentSpeed, 0.0); // RA2: as above
	}

	LinkedTo->Set_Speed(CurrentSpeed / Speed); // RA2: as above

	bool at_destination = LinkedTo->Get_Cell() == HeadToCoord.As_Cell();

	if ((CurrentState == HOVERING || CurrentState == CRUISING) && !NoWobbles) { // RA2: the NoWobbles flag (+0x38) turns the wobble off
		CurrentWobble += DEG_TO_RAD(360) / (15.0 / Wobbles); // RA2: the locomotor's own Wobbles (+0x30)
	} else {
		CurrentWobble = 0;
	}

	int desired_height = fastmath::sin(CurrentWobble) * Deviation + FlightLevel; // RA2: the locomotor's own Deviation (+0x34)
	int height = LinkedTo->Height;
	int ground_height = Map.Get_Height_GL(LinkedTo->PositionCoord);

	if (Map[LinkedTo->Get_Coord()].IsUnderBridge && LinkedTo->PositionCoord.Z >= ground_height + BRIDGE_CELL_HEIGHT * LEVEL_LEPTON_H) {
		ground_height += BRIDGE_LEPTON_HEIGHT;
	}

	int height_diff = 0;
	if (CurrentState != DESCENDING && CurrentState != GROUNDED && (!at_destination || LinkedTo->TClass->BalloonHover)) { // RA2: a balloon hoverer keeps following the terrain above its destination
		height_diff = height - Desired_Flight_Level();
	} else {
		height_diff = height - ground_height;
	}

	bool moved = false;
	if (height_diff < desired_height) {
		int height_agl = LinkedTo->HeightAGL;
		if ((Map[LinkedTo->Get_Coord()].IsUnderBridge) && !LinkedTo->IsOnBridge) {
			if (LinkedTo->PositionCoord.Z >= Map.Get_Height_GL(LinkedTo->PositionCoord) + BRIDGE_LEPTON_HEIGHT) {
				height_agl -= BRIDGE_LEPTON_HEIGHT;
			}
		}
		if (height_agl == 0) {
			LinkedTo->Clear_Occupy_Bit(LinkedTo->PositionCoord);
			LinkedTo->IsOnBridge = false;
		}
		if (height_diff + Climb > desired_height) { // RA2: the climb step (+0x20) is limited so the unit does not overshoot the wanted height
			height += desired_height - height_diff; // RA2: as above
		} else {
			height = (int)(height + Climb); // RA2: as above
		}
		moved = true;
	} else if (height_diff > desired_height) { // RA2: an else-if, as the executable tests the flags of one comparison
		if (height_diff - Climb < desired_height) { // RA2: the same limit going down
			height += desired_height - height_diff; // RA2: as above
		} else {
			height = (int)(height - Climb); // RA2: as above
		}
		if (height <= ground_height) {
			height = ground_height;
		}
		height_diff = MAX(height_diff, 0);
		moved = true; // RA2: after the MAX (both branches share this store)
	}

	if (LinkedTo->Get_Cell() != HeadToCoord.As_Cell()) {
		if (height_diff < desired_height / 2) {
			CurrentSpeed = 0; // RA2: the speed drops to zero, not by a tenth
		}
		if (height_diff < desired_height / 4) {
			CurrentSpeed = 0; // RA2: as above
		}
	}

	if (moved) {
		LinkedTo->Height = height;
		if (need_to_mark) {
			LinkedTo->Mark(MARK_UP);
		}
	}

	Coord new_coord = Move_Coord(LinkedTo->PositionCoord, Facing.Current(), CurrentSpeed);
	LinkedTo->Set_Coord(new_coord);

	CellClass * cellptr = &Map[new_coord];
	BuildingClass * building = cellptr->Cell_Building();
	if (building && building->Class->IsFirestormWall && building->House->FirestormDefenseActivated) {
		building->Crossing_Firestorm(LinkedTo, true);
	}

	LinkedTo->PrimaryFacing.Set(Facing.Current());

	if (need_to_mark) {
		LinkedTo->Mark(MARK_DOWN);
	} else {
		LinkedTo->IsDown = was_down;
	}
}


/// <summary>
/// Finds the closest free spot to the coordinate specified.
/// This is the jumpjet's flavor of the map query -- the spot it returns is raised to ground
/// level, or to bridge level when the cell lies under a bridge, so that it can be landed on.
/// </summary>
/// <param name="to">The coordinate to search near.</param>
/// <returns>Returns with the free coordinate found. Otherwise, COORD_NONE is
/// returned.</returns>
Coord JumpjetLocomotionClass::Closest_Free_Spot(Coord const & to) const
{
	Coord closest = Map.Closest_Free_Spot(to);
	if (closest != COORD_NONE) {
		closest.Z = Map.Get_Height_GL(closest);
		if (Map[closest].IsUnderBridge) {
			closest.Z += BRIDGE_LEPTON_HEIGHT;
		}
	}
	return(closest);
}


/// <summary>
/// Determines the height the jumpjet should be flying at.
/// This routine keeps the unit clear of whatever it is passing over. It looks ahead to the
/// cell being moved into as well, so that the unit climbs before it arrives rather than
/// after.
/// </summary>
/// <returns>Returns with the height, in leptons, that the terrain below demands.</returns>
int JumpjetLocomotionClass::Desired_Flight_Level(void) const
{
	Coord coord = LinkedTo->PositionCoord;

	int height = Map[coord].Occupier_Height();
	if (Map[coord].IsUnderBridge) {
		height += BRIDGE_LEPTON_HEIGHT;
	}

	if (CurrentSpeed > 0) {
		coord = Adjacent_Cell(coord, (FacingType)Facing.Current().As_Dir8());
		int adjacent_height = Map[coord].Occupier_Height();
		if (Map[coord].IsUnderBridge) {
			height += BRIDGE_LEPTON_HEIGHT;
		}
		if (adjacent_height > height) {
			return(adjacent_height);
		}
		return((height + adjacent_height) / 2);
	}
	return(height);
}


/// <summary>
/// Handles the occupation bits held by the jumpjet.
/// A descending unit reserves the cell it intends to touch down on, and this routine is how
/// that reservation is given up when the object is lifted off the map.
/// </summary>
/// <param name="mark">The marking operation being performed, such as MARK_UP.</param>
void STDMETHODCALLTYPE JumpjetLocomotionClass::Mark_All_Occupation_Bits(int mark)
{
	if (mark == MARK_UP) {
		Coord headto = Head_To_Coord();
		if (headto != COORD_NONE && (CurrentState == GROUNDED || IsLanding)) {
			LinkedTo->Clear_Occupy_Bit(headto);
			IsLanding = false;
		}
	}
}


/// <summary>
/// Fetches the coordinate this jumpjet is heading toward.
/// A unit sitting on the ground has nowhere to be, so its own position stands in for the
/// destination.
/// </summary>
/// <returns>Returns with the coordinate being flown to.</returns>
CoordStruct STDMETHODCALLTYPE JumpjetLocomotionClass::Head_To_Coord(void)
{
	if (CurrentState == GROUNDED) {
		return(LinkedTo->PositionCoord);
	} else {
		return(HeadToCoord);
	}
}

/// <summary>
/// Copies the linked object type's jumpjet settings and starts its facing east.
/// </summary>
/// <param name="object">The foot object this locomotor drives.</param>
HRESULT STDMETHODCALLTYPE JumpjetLocomotionClass::Link_To_Object(void * object) // RA2: 0x0052F230, not in TibSun; ra2-decomp's twin text
{
	FootClass * foot = (FootClass *)object;
	TurnRate = foot->TClass->JumpjetTurnRate;
	Speed = foot->TClass->JumpjetSpeed;
	Climb = foot->TClass->JumpjetClimb;
	Crash = foot->TClass->JumpjetCrash;
	int height = foot->TClass->JumpjetHeight;
	Height = MAX(height, LEVEL_LEPTON_H * 2);
	Accel = foot->TClass->JumpjetAccel;
	Wobbles = foot->TClass->JumpjetWobbles;
	Deviation = foot->TClass->JumpjetDeviation;
	NoWobbles = foot->TClass->JumpjetNoWobbles;
	FacingClass turning(TurnRate);
	Facing = turning;
	Facing.Set_Desired(DIR_E);
	Facing.Set(DIR_E);
	return(BASECLASS::Link_To_Object(object));
}
