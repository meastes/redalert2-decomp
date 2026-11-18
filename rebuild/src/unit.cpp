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

/* $Header: /CounterStrike/UNIT.CPP 1     3/03/97 10:26a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : UNIT.CPP                                                     *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : September 10, 1993                                           *
 *                                                                                             *
 *                  Last Update : November 3, 1996 [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Recoil_Adjust -- Adjust pixel values in direction specified.                              *
 *   UnitClass::AI -- AI processing for the unit.                                              *
 *   UnitClass::APC_Close_Door -- Closes an APC door.                                          *
 *   UnitClass::APC_Open_Door -- Opens an APC door.                                            *
 *   UnitClass::Active_Click_With -- Intercepts the active click to see if deployment is possib*
 *   UnitClass::Active_Click_With -- Performs specified action on specified cell.              *
 *   UnitClass::Approach_Target -- Handles approaching the target in order to attack it.       *
 *   UnitClass::Assign_Destination -- Assign a destination to a unit.                          *
 *   UnitClass::Blocking_Object -- Determines how a object blocks a unit                       *
 *   UnitClass::Can_Enter_Cell -- Determines cell entry legality.                              *
 *   UnitClass::Can_Fire -- Determines if turret can fire upon target.                         *
 *   UnitClass::Click_With -- Handles player map clicking while this unit is selected.         *
 *   UnitClass::Credit_Load -- Fetch the full credit value of cargo carried.                   *
 *   UnitClass::Crew_Type -- Fetches the kind of crew that this object produces.               *
 *   UnitClass::Debug_Dump -- Displays the status of the unit to the mono monitor.             *
 *   UnitClass::Desired_Load_Dir -- Determines the best cell and facing for loading.           *
 *   UnitClass::Draw_It -- Draws a unit object.                                                *
 *   UnitClass::Edge_Of_World_AI -- Check for falling off the edge of the world.               *
 *   UnitClass::Enter_Idle_Mode -- Unit enters idle mode state.                                *
 *   UnitClass::Fire_Direction -- Determines the direction of firing.                          *
 *   UnitClass::Firing_AI -- Handle firing logic for this unit.                                *
 *   UnitClass::Flag_Attach -- Attaches a house flag to this unit.                             *
 *   UnitClass::Flag_Remove -- Removes the house flag from this unit.                          *
 *   UnitClass::Goto_Clear_Spot -- Finds a clear spot to deploy.                               *
 *   UnitClass::Goto_Tiberium -- Search for and head toward nearest available Tiberium patch.  *
 *   UnitClass::Greatest_Threat -- Fetches the greatest threat for this unit.                  *
 *   UnitClass::Harvesting -- Harvests tiberium at the current location.                       *
 *   UnitClass::Init -- Clears all units for scenario preparation.                             *
 *   UnitClass::Limbo -- Limbo this unit.                                                      *
 *   UnitClass::Mission_Guard -- Special guard mission override processor.                     *
 *   UnitClass::Mission_Guard_Area -- Guard area logic for units.                              *
 *   UnitClass::Mission_Harvest -- Handles the harvesting process used by harvesters.          *
 *   UnitClass::Mission_Hunt -- This is the AI process for aggressive enemy units.             *
 *   UnitClass::Mission_Move -- Handles special move mission overrides.                        *
 *   UnitClass::Mission_Repair -- Handles finding and proceeding on a repair mission.          *
 *   UnitClass::Mission_Unload -- Handles unloading cargo.                                     *
 *   UnitClass::Offload_Tiberium_Bail -- Offloads one Tiberium quantum from the object.        *
 *   UnitClass::Ok_To_Move -- Queries whether the vehicle can move.                            *
 *   UnitClass::Overlap_List -- Determines overlap list for units.                             *
 *   UnitClass::Overrun_Square -- Handles vehicle overrun of a cell.                           *
 *   UnitClass::Per_Cell_Process -- Performs operations necessary on a per cell basis.         *
 *   UnitClass::Pip_Count -- Fetches the number of pips to display on unit.                    *
 *   UnitClass::Random_Animate -- Handles random idle animation for the unit.                  *
 *   UnitClass::Read_INI -- Reads units from scenario INI file.                                *
 *   UnitClass::Receive_Message -- Handles receiving a radio message.                          *
 *   UnitClass::Reload_AI -- Perform reload logic for this unit.                               *
 *   UnitClass::Rotation_AI -- Process any turret or body rotation.                            *
 *   UnitClass::Scatter -- Causes the unit to scatter to a nearby location.                    *
 *   UnitClass::Set_Speed -- Initiate unit movement physics.                                   *
 *   UnitClass::Shape_Number -- Fetch the shape number to use for this unit.                   *
 *   UnitClass::Should_Crush_It -- Determines if this unit should crush an object.             *
 *   UnitClass::Sort_Y -- Give Y coordinate sort value for unit.                               *
 *   UnitClass::Start_Driver -- Starts driving and reserves destination cell.                  *
 *   UnitClass::Take_Damage -- Inflicts damage points on a unit.                               *
 *   UnitClass::Tiberium_Check -- Search for and head toward nearest available Tiberium patch. *
 *   UnitClass::Tiberium_Load -- Determine the Tiberium load as a percentage.                  *
 *   UnitClass::Try_To_Deploy -- The unit attempts to "deploy" at current location.            *
 *   UnitClass::UnitClass -- Constructor for units.                                            *
 *   UnitClass::Unlimbo -- Removes unit from stasis.                                           *
 *   UnitClass::What_Action -- Determines action to perform on specified cell.                 *
 *   UnitClass::What_Action -- Determines what action would occur if clicked on object.        *
 *   UnitClass::Write_INI -- Store the units to the INI database.                              *
 *   UnitClass::delete -- Deletion operator for units.                                         *
 *   UnitClass::new -- Allocate a unit slot and adjust access arrays.                          *
 *   UnitClass::~UnitClass -- Destructor for unit objects.                                     *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#define INCLUDE_COM
#include "always.h"
#include "geometry.h" // RA2: the header statics, in the order of the unit's initializer table
#include "rect.h"

#include "unit.h"

#include "_bench.h"
#include "_convert.h"
#include "_map.h"
#include "_mixfile.h"
#include "_rect.h"
#include "_rtti.h"
#include "_rules.h"
#include "_surface.h"
#include "_tactica.h"
#include "aircraft.h"
#include "airctype.h"
#include "anim.h"
#include "astar.h"
#include "bench.h"
#include "blit.h"
#include "building.h"
#include "builtype.h"
#include "bullet.h"
#include "bullettype.h"
#include "ccrand.h"
#include "cell.h"
#include "combat.h"
#include "conquer.h"
#include "draw.h"
#include "ebolt.h" // RA2: ~EBolt, called by Save
#include "fog.h"
#include "house.h"
#include "houstype.h"
#include "ilocos.h"
#include "incdec.h"
#include "infantry.h"
#include "infatype.h"
#include "inline.h"
#include "ion.h"
#include "ipiggy.h"
#include "isotype.h"
#include "lightcon.h"
#include "mixfile.h"
#include "mono.h"
#include "overlay.h"
#include "overtype.h"
#include "parasite.h" // RA2: Can_Enter_Cell reads the parasite victim
#include "partsys.h"
#include "planning.h" // RA2: What_Action asks Planning_Allows_Event_Type
#include "psystype.h" // RA2: Do_MISSION_UNLOAD reads the behavior of the refinery's damage particle systems
#include "revent.h"
#include "rules.h"
#include "scheme.h"
#include "session.h"
#include "shapeset.h"
#include "spawnmanager.h" // RA2: Can_Fire asks the spawn manager
#include "stringtable.h" // RA2: Full_Name's tooltip labels
#include "sun.h"
#include "swizzle.h"
#include "tactical.h"
#include "tag.h"
#include "tagtype.h"
#include "team.h"
#include "tiberium.h"
#include "tracker.h"
#include "tube.h"
#include "unittype.h"
#include "voc.h"
#include "vox.h"
#include "warhead.h"
#include "weapon.h"

#include "bench.hh"
#include "color.hh"
#include "tube.hh"
#include "ftimer.h"
#include "temporal.h" // RA2

int __stdcall Is_Weapon_Present(WeaponDataStruct const * wdata); // RA2: techno.cpp, at 0x6D8640
#include "capmgr.h" // RA2: CaptureManagerClass, read by Receive_Message


namespace Unsorted { extern bool MoveFeedback; }

/*
**	RA2: the casts Assign_Destination and the teleport block use: a null pointer gives null, a wrong RTTI gives null, the
**	virtual Fetch_RTTI is called once.
*/
static inline CellClass * Unit_As_Cell(AbstractClass * object) // RA2: as above
{
	return(object == NULL ? NULL : (object->Fetch_RTTI() == RTTI_CELL ? (CellClass *)object : NULL)); // RA2: as above
}

static inline bool Unit_Is_Cell(AbstractClass const * object) // RA2: the null test stays inside the helper
{
	return(object != NULL && object->Fetch_RTTI() == RTTI_CELL); // RA2: as above
}

static inline BuildingClass * Unit_As_Building(AbstractClass * object) // RA2: as above
{
	return(object == NULL ? NULL : (object->Fetch_RTTI() == RTTI_BUILDING ? (BuildingClass *)object : NULL)); // RA2: as above
}

static inline UnitClass * Unit_As_Unit(AbstractClass * object) // RA2: as above
{
	return(object == NULL ? NULL : (object->Fetch_RTTI() == RTTI_UNIT ? (UnitClass *)object : NULL)); // RA2: as above
}


char const * const UnitClass::INI_NAME = "Units";

Rect UnitCompositeDirtyRect(RECT_NONE);



int UnitClass::Visceroid_Fire_List[FACING_COUNT] = { 100, 105, 110, 115, 120, 125, 90, 95 };


/*
**	This is the list of animation stages to use when the harvester
**	is to dump its load into the refinery. The offsets are based from the
**	start of the dump animation.
*/
int UnitClass::Harvester_Load_List[FACING_COUNT] = {4, 5, 6, 7, 0, 1, 2, 3};





/***********************************************************************************************
 * UnitClass::UnitClass -- Constructor for units.                                              *
 *                                                                                             *
 *    This constructor for units will initialize the unit into the game                        *
 *    system. It will be placed in all necessary tracking lists. The initial condition will    *
 *    be in a state of limbo.                                                                  *
 *                                                                                             *
 * INPUT:   classid  -- The type of unit to create.                                            *
 *                                                                                             *
 *          house -- The house owner of this unit.                                             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/21/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
UnitClass::UnitClass(UnitTypeClass const * type, HouseClass * house) :
	BASECLASS(house),
	FiringSyncDelay(-1), // RA2: no Reload (TibSun's use does not compile against RA2's declaration)
	Class((UnitTypeClass *)type),
	FollowingMe(NULL), // RA2: as above
	Flagged(HOUSE_NONE),
	IsFollowing(false), // RA2: as above
	IsDumping(false),
	IsHarvesting(false),
	IsCompositingToEightBitSurface(false), // RA2: no Charge (TibSun's use does not compile against RA2's declaration)
	VisceroidFacing(FACING_NONE),
	DeathCounter(-1),
	ElectricBolt(NULL), // RA2: the members after DeathCounter are cleared as well
	Deployed(false), // RA2: as above
	NonPassengerCount(0) // RA2: as above
{
	Create_ID();
	Units.Add(this);
	SecondaryFacing.Set(PrimaryFacing.Current());

	if (Class != NULL) {
		Locomotion = ILocomotionPtr(Class->Locomotor, NULL, CLSCTX_ALL);
		Locomotion->Link_To_Object(this);
	}

	if (Class != NULL) {
		PrimaryFacing.Set_ROT(Class->ROT);
		SecondaryFacing.Set_ROT(Class->ROT);
		if (Class->InitialAmmo != -1) { // RA2: a type with a starting ammunition count uses it
			Ammo = Class->InitialAmmo; // RA2: as above
		} else {
			Ammo = Class->MaxAmmo; // RA2: as above
		}
		IsCloakable = Class->IsCloakable;
		EstimatedHealth = Strength = Class->MaxStrength; // RA2: the estimate starts equal to the strength
	}

	ReloadTimer = 0; // RA2: the reload timer is restarted

	if (House != NULL) {
		House->Tracking_Add(this);
	}

	TechnoClass::Init(); // RA2: the base initializer, with the veteran checks written out below

	if (Class != NULL) { // RA2: as above
		if (House != NULL) { // RA2: as above
			TypeList<UnitTypeClass const *> & list = House->Class->VeteranUnits; // RA2: as above
			for (int index = 0; index < list.Count(); index++) { // RA2: as above
				if (list[index] == Class) { // RA2: as above
					Veterancy.Set_Veteran(true); // RA2: as above
					break; // RA2: as above
				}
			}
		}

		if (House != NULL && House->WarFactoryInfiltrated && !Class->Naval && Class->IsTrainable) { // RA2: as above
			Veterancy.Set_Veteran(true); // RA2: as above
		}

		if (Class->Gunner) { // RA2: as above
			Set_Current_Weapon(0); // RA2: as above
		}
	}

	TargetTracker.Add_Index(Fetch_ID(), this);
}



/***********************************************************************************************
 * UnitClass::~UnitClass -- Destructor for unit objects.                                       *
 *                                                                                             *
 *    This destructor will lower the unit count for the owning house as well as inform any     *
 *    other units in communication, that this unit is about to leave reality.                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/15/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: 645 bytes differ (adopted text)
UnitClass::~UnitClass(void)
{
	if (ElectricBolt != NULL) { // RA2: a live electric bolt is destroyed first
		ElectricBolt->~EBolt(); // RA2: as above
		ElectricBolt = NULL; // RA2: as above
	}

	if (GameActive && Class != NULL) {

		if (House->Can_Build(Class, false, false) == -1) {
			House->IsRecalcNeeded = true;
		}

		/*
		**	Remove this member from any team it may be associated with. This must occur at the
		**	top most level of the inheritance hierarchy because it may call virtual functions.
		*/
		if (Team != NULL) {
			Team->Remove(this, -1, false); // RA2: the removal takes its type index and idle flag
			Team = NULL;
		}

		House->Tracking_Remove(this);

		/*
		**	If there are any cargo members, delete them.
		*/
		if (ScenarioInit == 0) { // RA2: the cargo is only deleted outside scenario loading
			while (Cargo.Is_Something_Attached()) {
				delete Cargo.Detach_Object();
			}
		}

		if (Limbo()) { // RA2: a unit still carrying a flag hands it back to its house at this cell
			if (Flagged != HOUSE_NONE) { // RA2: as above
				Houses[Flagged]->Flag_Attach(Get_Cell(), false); // RA2: as above
				Flagged = HOUSE_NONE; // RA2: as above
			}
		}
	}

	Detach_This_From_All(this);
	Units.Delete(this);
	TargetTracker.Remove_Index(Fetch_ID());
	IsActive = false;
}
#endif



#ifdef _DEBUG
/***********************************************************************************************
 * UnitClass::Debug_Dump -- Displays the status of the unit to the mono monitor.               *
 *                                                                                             *
 *    This displays the current status of the unit class to the mono monitor. By this display  *
 *    bugs may be tracked down or prevented.                                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/02/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void UnitClass::Debug_Dump(MonoClass * mono) const
{
	mono->Set_Cursor(0, 0);
	mono->Set_Cursor(47, 5);mono->Printf("%02X:%02X", SecondaryFacing.Current(), SecondaryFacing.Desired());

	mono->Set_Cursor(1, 11);mono->Printf("%3d", Storage.Get_Total_Amount());
	mono->Set_Cursor(7, 11);mono->Printf("%5d", Storage.Get_Total_Value());

	mono->Fill_Attrib(66, 13, 12, 1, IsDumping ? MonoClass::INVERSE : MonoClass::NORMAL);

	BASECLASS::Debug_Dump(mono);
}
#endif



/// <summary>
/// Handles a unit's passage through a tunnel.
/// This routine walks the vehicle along the segments of the tube it entered, following the
/// tunnel's slope, and hands it back to the map at the far mouth. If the exit cell is
/// already spoken for the unit waits underground, first shooing any idle occupier away.
/// </summary>
void UnitClass::Tunnel_AI(void)
{
	Coord coord;
	bool finished = Tubes[CurrentTube]->Dirs[CurrentTubeDir] == FACING_NONE;
	if (!finished) {
		int distance = Distance(LastTubeCoord);
		TubeClass * tube = Tubes[CurrentTube];
		Get_Coord();	/// Result discarded.

		DirType direction_temp;
		DirType direction = direction_temp.Direction(Center_Coord(), LastTubeCoord);
		int speed = (int)(Class->MaxSpeed * 1.5);

		Coord exit(tube->Exit);
		Coord enter(tube->Enter);
		int height_step = tube->Count;
		int enter_height = Map.Get_Height_GL(enter);
		height_step = (Map.Get_Height_GL(exit) - enter_height) / height_step;

		if (distance <= speed) {
			CurrentTubeDir++;
			FacingType tube_dir = tube->Dirs[CurrentTubeDir];
			finished = tube_dir == FACING_NONE;
			PositionCoord = LastTubeCoord;

			if (!finished) {
				coord = Adjacent_Cell(LastTubeCoord, tube_dir);
				Coord delta = Map[coord].Cell_Coord() - Map[LastTubeCoord].Cell_Coord();
				LastTubeCoord += Coord(delta.X, delta.Y, 0);
				LastTubeCoord.Z += height_step;

				DirType dir = DirType().Direction(Center_Coord(), LastTubeCoord);
				Coord new_coord = Move_Coord(PositionCoord, dir, speed - distance);
				double divisor;
				if (tube_dir % 2) {
					divisor = CELL_LEPTON_H * M_SQRT2;
				} else {
					divisor = CELL_LEPTON_H;
				}
				new_coord.Z += (speed - distance) / divisor * height_step;
				PositionCoord = new_coord;
				return;
			}
		} else {
			FacingType tube_dir = tube->Dirs[CurrentTubeDir];
			Coord new_coord = Move_Coord(Get_Coord(), direction, speed);
			double divisor;
			if (tube_dir % 2) {
				divisor = CELL_LEPTON_H * M_SQRT2;
			} else {
				divisor = CELL_LEPTON_H;
			}
			new_coord.Z += (speed) / divisor * height_step;
			PositionCoord = new_coord;
			return;
		}
	}

	if (finished) {
		CellClass * cellptr = &Map[Get_Coord()];
		ObjectClass * occupier = cellptr->Cell_Occupier();
		bool was_down = true;
		bool was_marked = true;

		while (occupier) {
			if (occupier == this) {
				was_marked = false;
			} else {
				if (occupier->RTTI == RTTI_UNIT || occupier->RTTI == RTTI_INFANTRY) {
					if (!((FootClass *)occupier)->Locomotion->Is_Moving()) {
						Coord scatter_coord(0, 0, 0);
						occupier->Scatter(scatter_coord, true, true);
					}
					was_down = false;
				}
			}
			occupier = occupier->Next;
		}

		if (was_down) {
			TubeClass * tube = Tubes[CurrentTube];
			Cell exit_cell = tube->Exit;
			Coord exit_coord(exit_cell, LastTubeCoord.Z);
			PositionCoord = exit_coord;
			CurrentTube = TUBE_NONE;

			if (was_marked) Mark(MARK_DOWN);

			if (NavCom == cellptr) {
				Scatter(Coord(0, 0, 0), true, true);
			}

			IsPlanningToLook = true;
			Per_Cell_Process(PCP_END);
			Set_Speed(1);

			TubeClass * tube_here = Map[Get_Coord()].Get_Tunnel();
			if (tube_here != NULL) {
				PrimaryFacing.Set(DirType(DirType(tube_here->EnterDir).As_Int() - DirType(FACING_135).As_Int() - 1).Snap_To_8());
			}
		} else {
			Set_Speed(0);
		}
	}
}


/// <summary>
/// RA2
/// </summary>
static inline Coord Unit_AI_Smoke_Position(Coord const & position) // RA2: the chrono sparkle appears 120 leptons south-east of the unit
{
	Coord smoke = position; // RA2: as above
	smoke.X += 120; // RA2: as above
	smoke.Y += 120; // RA2: as above
	return smoke; // RA2: as above
}


/// <summary>
/// RA2
/// </summary>
static inline Cell Unit_AI_Refinery_Exit_Cell(Cell const & position) // RA2: the cell a harvester stands on when it has left a refinery
{
	return Cell((short)(position.X + 3), (short)(position.Y + 1)); // RA2: as above
}


/***********************************************************************************************
 * UnitClass::AI -- AI processing for the unit.                                                *
 *                                                                                             *
 *    This routine will perform the AI processing necessary for the unit. These are non-       *
 *    graphic related operations.                                                              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/31/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void UnitClass::AI(void)
{
	if (TemporalTargetingMe != NULL) { // RA2: a temporal beam on the unit runs its own AI first
		TemporalTargetingMe->AI(); // RA2: as above
	}

	if ((IsBeingWarpedOut() || IsWarpingIn()) && Frame % 24 == 0 && Rule->ChronoSparkle1 != NULL) { // RA2: a warping unit sparkles every 24 frames
		new AnimClass(Rule->ChronoSparkle1, Unit_AI_Smoke_Position(PositionCoord), 0, 1, ShapeFlags_Type(0x600), 0); // RA2: as above
	}

	if (IsWarpingIn() || (IsBeingWarpedOut() && IsImmobilized)) { // RA2: the locomotor keeps running while warping
		Locomotion->Process(); // RA2: as above
		if (!IsActive) return; // RA2: as above
	}

	if ((IsWarpingIn() && TemporalTargetingMe != NULL) || IsBeingWarpedOut()) { // RA2: a warping unit drops its orders and does nothing else
		if (TarCom != NULL) { // RA2: as above
			Assign_Target(NULL); // RA2: as above
		}
		if (NavCom != NULL) { // RA2: as above
			Assign_Destination(NULL, true); // RA2: as above
			return; // RA2: as above
		}
		return; // RA2: as above
	}

	if (DeathCounter != -1) {
		if (DeathCounter++ >= Class->MaxDeathCounter) {
			Explode();
			Mark(MARK_UP);
			Delete_Me();
			return;
		}
	}

	if (CurrentTube >= TUBE_FIRST) { // RA2: no MaxCharge (not in RA2's layout)
		Tunnel_AI();
		Update_Radar_Position(false); // RA2: as above
		return;
	}

	int countdown = FiringSyncDelay - 1; // RA2: the delay counts down to -1
	if (countdown < -1) countdown = -1; // RA2: as above
	FiringSyncDelay = countdown; // RA2: as above

	if (Class->DeploysInto != NULL) { // RA2: every construction yard type is tested, not only the first
		for (int index = 0; index < Rule->BuildConst.Count(); index++) { // RA2: as above
			if (Rule->BuildConst[index] == Class->DeploysInto) { // RA2: as above
				if (House->IsBaseBuilding && !House->Is_Human_Player() && Session.Type != GAME_NORMAL && House->ConYards.Count() == 0 && CurrentMission != MISSION_HUNT && CurrentMission != MISSION_UNLOAD) { // RA2: as above
					QueueMission(MISSION_HUNT, false); // RA2: queued, not assigned
				}
				break; // RA2: as above
			}
		}
	}

	/*
	**	Act on new orders if the unit is at a good position to do so.
	*/
	if (Ready_To_Commence()) {
		Commence();
	}
	BASECLASS::AI();

	if (Class->CanDisguise && !Class->PermaDisguise) { // RA2: a disguised unit updates its disguise
		UpdateDisguise(); // RA2: as above
	}

	if (IsSinking) {
		Coord coord = PositionCoord; // RA2: sinks straight down, 5 leptons a frame
		Set_Coord(Coord(coord.X, coord.Y, coord.Z - 5)); // RA2: as above
		if (Get_Height_AGL() < -400) { // RA2: as above
			Record_The_Kill(NULL);
			Delete_Me();
			return;
		}
		if ((Frame & 3) == 0) { // RA2: a wake every 4 frames
			int ry = Random_Pick(-170, 170); // RA2: as above
			int rx = Random_Pick(-170, 170); // RA2: as above
			int height = Get_Height_AGL(); // RA2: as above
			Coord offset(rx, ry, -height); // RA2: as above
			Coord origin = PositionCoord; // RA2: as above
			offset.X = origin.X + offset.X; // RA2: as above
			offset.Y += origin.Y; // RA2: as above
			offset.Z += origin.Z; // RA2: as above
			coord = offset; // RA2: as above
			new AnimClass(Rule->Wake, coord, 0, 1, ShapeFlags_Type(0x600), 0); // RA2: as above
		}
	}

	if (IsActive) { // RA2: the rest runs only while the unit is alive
		/*
		**	Hack check to ensure that a harvester won't harvest if it is not harvesting.
		*/
		if (Mission != MISSION_HARVEST) { // RA2: as above
			IsHarvesting = false; // RA2: as above
		}

		Firing_AI(); // RA2: the combat logic runs before the turret rotation
		Rotation_AI(); // RA2: as above

		/*
		**	Delete this unit if it finds itself off the edge of the map and it is in
		**	guard or other static mission mode.
		*/
		if (Mission == MISSION_GUARD) { // RA2: the edge of the world test is expanded inline
			Coord position = PositionCoord; // RA2: as above
			if (!Map.In_Radar(position) && IsLocked) { // RA2: as above
				if (Team != NULL) Team->IsLeaveMap = true; // RA2: as above
				Stun(); // RA2: as above
				Delete_Me(); // RA2: as above
				return; // RA2: as above
			}
		}

		if (Class->IsSmallVisceroid || Class->IsLargeVisceroid) { // RA2: as above
			Visceroid_AI(); // RA2: as above
		}

		/*
		**	Units will reload every so often if they are under the burden of
		**	being required to reload between shots.
		*/
		Reload(); // RA2: the virtual reload

		/*
		**	Transporters require special logic handled here since there isn't a MISSION_WAIT_FOR_PASSENGERS
		**	mission that they can follow. Passenger loading is merely a part of their normal operation.
		*/
		if (Class->Max_Passengers() > 0) { // RA2: as above

			/*
			**	Double check that there is a passenger that is trying to load or unload.
			**	If not, then close the door.
			*/
			if (!Door.Is_Door_Closed() && Mission != MISSION_UNLOAD && Transmit_Message(RADIO_TRYING_TO_LOAD) != RADIO_ROGER) { // RA2: as above
				APC_Close_Door(); // RA2: as above
			}
		}

		/*
		**	Don't start a new mission unless the vehicle is in the center of
		**	a cell (not driving) and the door (if any) is closed.
		*/
		if (Ready_To_Commence()) { // RA2: as above
			Commence(); // RA2: as above
		}

		/*
		**	A cloaked object that is carrying the flag will always shimmer.
		*/
		if (Cloak == CLOAKED && Flagged != HOUSE_NONE) { // RA2: as above
			Do_Shimmer(); // RA2: as above
		}

		if ((Frame & 15) == 0 && (CurrentMission == MISSION_GUARD_AREA || CurrentMission == MISSION_GUARD) && !House->Is_Human_Player() && MissionQueue == MISSION_NONE && Strength < Class->MaxStrength && (ArchiveTarget == NULL || CurrentMission == MISSION_GUARD_AREA) && !Class->IsToHarvest && !Class->IsToVeinHarvest) { // RA2: the idle repair search is one condition
			BuildingClass * repair = Find_Unit_Repair_Facility(House, this); // RA2: as above
			if (repair != NULL) { // RA2: as above
				if (ArchiveTarget == NULL) { // RA2: as above
					Coord position = PositionCoord; // RA2: as above
					Assign_Archive_Target(&Map[position]); // RA2: as above
				}
				QueueMission(MISSION_ENTER, false); // RA2: queued, not assigned
				Assign_Target(NULL); // RA2: as above
				Assign_Destination(repair, true); // RA2: as above
			}
		}

		if (Class->IsToHarvest && !House->Is_Human_Player() && Frame - CurrentMissionStartTime > 300 && !Locomotion->Is_Moving()) { // RA2: an idle computer harvester that sits on a refinery is moved off it
			BuildingClass * refinery = (BuildingClass *)Map[(Coord const &)PositionCoord].Cell_Building(); // RA2: as above
			if (refinery != NULL && refinery->Class->IsRefinery) { // RA2: as above
				if (PositionCell == Unit_AI_Refinery_Exit_Cell(refinery->PositionCell)) { // RA2: as above
					BuildingClass * contact = Map[(Coord const &)PositionCoord].Cell_Building(); // RA2: as above
					if (HasAnyLink()) Transmit_Message(RADIO_OVER_OUT); // RA2: as above
					Assign_Destination(&Map[Nearby_Location(contact)], true); // RA2: as above
					QueueMission(MISSION_MOVE, true); // RA2: as above
				}
			}
		}
	}
}




/***********************************************************************************************
 * UnitClass::Rotation_AI -- Process any turret or body rotation.                              *
 *                                                                                             *
 *    This routine will handle the rotation logic for the unit's turret (if it has one) as     *
 *    well as its normal body shape.                                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/30/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: 456 bytes differ (adopted text)
void UnitClass::Rotation_AI(void)
{
	if (TarCom != NULL && !IsRotating) {
		DirType dir = Direction(TarCom);

		if (Class->IsTurretEquipped) {
			WeaponDataStruct const * weapon = GetTurretWeapon(); // RA2: the turret turns only for a present, non-omni weapon
			if (Is_Weapon_Present(weapon) && !weapon->Weapon->OmniFire) { // RA2: as above
				if (weapon->TurretLocked) { // RA2: a locked turret stays on the body facing
					SecondaryFacing.Set_Desired(PrimaryFacing.Current()); // RA2: as above
				} else {
					SecondaryFacing.Set_Desired(dir); // RA2: as above
				}
			}
		} else {

			/*
			**	Non turret equipped vehicles will rotate their body to face the target only
			**	if the vehicle isn't currently moving or facing the correct direction. This
			**	applies only to tracked vehicles. Wheeled vehicles never rotate to face the
			**	target, since they aren't maneuverable enough.
			*/
			if (Class->Speed == SPEED_TRACK && NavCom == NULL && !Locomotion->Is_Moving() && PrimaryFacing.Current() == dir) {
				PrimaryFacing.Set_Desired(dir);
			}
		}
	}

	if (Class->TurretSpins) { // RA2: a spinning turret keeps turning eight steps at a time
		DirType direction((Dir256)(SecondaryFacing.Current().As_Dir256() + 8)); // RA2: as above
		SecondaryFacing.Set(direction); // RA2: as above
	} else {
		IsRotating = false; // RA2: the turret state is recomputed each frame
		if (Class->IsTurretEquipped) { // RA2: as above
			if (SecondaryFacing.Is_Rotating()) { // RA2: as above
				if (!Class->TurretSpins) { // RA2: as above
					IsRotating = SecondaryFacing.Is_Rotating(); // RA2: as above
				}
			} else if (TarCom == NULL) { // RA2: an idle turret follows the destination or realigns with the body
				WeaponDataStruct const * weapon = GetTurretWeapon(); // RA2: as above
				bool locked = Is_Weapon_Present(weapon) && weapon->TurretLocked; // RA2: as above
				if (NavCom != NULL && !locked) { // RA2: as above
					SecondaryFacing.Set_Desired(Direction(NavCom)); // RA2: as above
				} else {
					SecondaryFacing.Set_Desired(PrimaryFacing.Current()); // RA2: as above
				}
			}
		}
	}

	if (Class->IsTurretEquipped) { // RA2: the turret state is mirrored for others
		TurretIsRotating = SecondaryFacing.Is_Rotating(); // RA2: as above
	}
}
#endif

 // RA2: no IsRadarEquipped (not in RA2's layout)


/***********************************************************************************************
 * UnitClass::Edge_Of_World_AI -- Check for falling off the edge of the world.                 *
 *                                                                                             *
 *    When a unit leaves the map it will be eliminated. This routine checks for this case      *
 *    and eliminates the unit accordingly.                                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was the unit eliminated by this routine?                                     *
 *                                                                                             *
 * WARNINGS:   Be sure to check for the return value and if 'true' abort any further processing*
 *             of the unit since it is dead. Only call this routine once per unit per          *
 *             game logic loop.                                                                *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/30/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool UnitClass::Edge_Of_World_AI(void)
{
	if (Mission == MISSION_GUARD && !Map.In_Radar(Get_Coord()) && IsLocked) {
		if (Team != NULL) Team->IsLeaveMap = true;
		Stun();
		Delete_Me();
		return(true);
	}
	return(false);
}

/// <summary>
/// RA2: a unit that cannot fire on the move holds its reload while moving.
/// </summary>
void UnitClass::Reload(void)
{
	if (!Class->MobileFire && Locomotion->Is_Moving()) {
		if (ReloadTimer > 0) {
			ReloadTimer = ReloadTimer + 1;
		}
	}
	TechnoClass::Reload();
}






/// <summary>
/// Should this unit deploy in order to attack?
/// This routine is used to decide whether a deployable vehicle such as the tick tank is
/// better off digging in before it opens fire. A computer controlled tick tank only
/// bothers when its target warrants it and there is somewhere legal to set down.
/// </summary>
/// <returns>bool; Should the unit deploy before firing?</returns>
bool UnitClass::Deploy_To_Fire(void) const
{
	if (Class->IsDeployToFire) {
		return(true);
	}

	if (Class->DeploysInto != NULL && Class->DeploysInto->IsTickTank) {
		if (!House->Is_Human_Player() && TarCom != NULL) {
			switch (TarCom->What_Am_I()) {
				case RTTI_INFANTRY:
					return(false);

				case RTTI_UNIT:
					if (Map[Get_Coord()].Can_Build_Here()) {
						return(true);
					}
					return(false);

				case RTTI_BUILDING:
					return(false);
			}
		}
	}
	return(false);
}


/// <summary>
/// RA2
/// </summary>
static inline bool Draw_It_Coord_Pixel(Coord coord, Point2D & pixel) // RA2: the pixel of a coordinate, taken by value: the Move_Coord result is built after the radian dies and shares its slot
{
	return TacticalMap->Coord_To_Pixel(coord, pixel); // RA2: as above
}


/// <summary>
/// RA2
/// </summary>
inline Cell Adjacent_Cell_By_Value(Cell cell, FacingType dir)
{
	return(cell + AdjacentCell[(int)dir & 0x07]);
}


/***********************************************************************************************
 * UnitClass::Firing_AI -- Handle firing logic for this unit.                                  *
 *                                                                                             *
 *    This routine wil check for and perform any firing logic required of this unit.           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This should be called only once per unit per game logic loop.                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/30/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void UnitClass::Firing_AI(void)
{
	if (TarCom != NULL && Get_Class_Weapon_Data(0)->Weapon != NULL) { // RA2: the primary weapon slot is asked directly

		/*
		**	Determine which weapon can fire. First check for the primary weapon. If that weapon
		**	cannot fire, then check any secondary weapon. If neither weapon can fire, then the
		**	failure code returned is that from the primary weapon.
		*/
		int primary = What_Weapon_Should_I_Use(TarCom);
		FireErrorType ok = Can_Fire(TarCom, primary, true); // RA2: the range check is requested

		if ((ok == FIRE_OK || ok == FIRE_FACING) && Deploy_To_Fire()) {
			QueueMission(MISSION_UNLOAD, false); // RA2: queued, not assigned
			return;
		}

		switch (ok) {
			case FIRE_ILLEGAL:
				if (Combat_Damage(primary) < 0) {
					AbstractClass * target = TarCom; // RA2: only a unit target that is not damaged enough keeps being attacked
					if (target != NULL && (target->AbstractFlags & 2) && target->Fetch_RTTI() == RTTI_UNIT) { // RA2: as above
						if (((ObjectClass *)target)->HealthRatio >= Rule->ConditionGreen) { // RA2: as above
							Assign_Target(NULL); // RA2: as above
							return; // RA2: as above
						}
					} else {
						Assign_Target(NULL); // RA2: as above
						return; // RA2: as above
					}
				}
				break;

			case FIRE_OK:
				if (!Class->IsFireAnim) {
					IsFiring = false;
				}

				if (Class->IsLargeVisceroid || Class->IsSmallVisceroid) {
					Set_Stage(UnitClass::Visceroid_Fire_List[Direction(TarCom).As_Dir8()]);
					Set_Rate(5);
				}

				Fire_At(TarCom, primary);
				return; // RA2: the case returns instead of breaking

			case FIRE_FACING:
				if (Class->IsLockTurret || !Class->IsTurretEquipped) {
					if (NavCom == NULL && !Locomotion->Is_Moving()) {
						PrimaryFacing.Set_Desired(Direction(TarCom));
						SecondaryFacing.Set_Desired(PrimaryFacing.Desired());
					}
				} else {
					SecondaryFacing.Set_Desired(Direction(TarCom));
					return; // RA2: the turret case returns at once
				}
				break;

			case FIRE_CLOAKED:
				IsFiring = false;
				Do_Uncloak();
				return; // RA2: as above

			case FIRE_RANGE:
			case FIRE_MUST_DEPLOY:
				IsFiring = false; // RA2: no approach; the case only clears the firing flag
				return; // RA2: as above

			case FIRE_CANT: // RA2: a new case
				if (SpawnManager != NULL) { // RA2: a unit that cannot fire abandons its spawn manager's target
					SpawnManager->ResetTarget(); // RA2: as above
				}
				break;
		}
	}
}



/// <summary>
/// Handles the per frame logic for a visceroid.
/// This routine drives the creature's aimless wandering, sends a small visceroid off to
/// merge with any neighbor it stumbles across, and steers a wounded one onto Tiberium so
/// that it can heal.
/// </summary>
#ifdef RA2_NON_MATCHING // not exact: 34 bytes differ (adopted text)
void UnitClass::Visceroid_AI(void)
{
	Cell cell = PositionCell;
	int stage = Fetch_Stage();

	if (stage >= 90) {
		if (stage % 5 != 4) { // RA2: the long stages restart only on their last step
			return; // RA2: as above
		}
		Set_Stage(TICKS_PER_SECOND * 2 * Random_Pick(0, 2)); // RA2: as above
		Set_Rate(1); // RA2: as above
		return; // RA2: as above
	}

	if (stage % 30 == 29) {
		Set_Stage(TICKS_PER_SECOND * 2 * Random_Pick(0, 2));
	}
	Set_Rate(1);

	if (Locomotion->Is_Moving()) { // RA2: a moving visceroid does nothing else
		return; // RA2: as above
	}

	if (Class->IsSmallVisceroid) {
		int direction = 7; // RA2: the neighbors are scanned starting after the last facing
		for (int i = 0; i < FACING_COUNT; i++) {
			direction = (direction + 1) & 7; // RA2: as above
			Cell adjacent = Adjacent_Cell_By_Value(cell, (FacingType)direction); // RA2: the neighbor is cell plus the offset table entry
			CellClass * cellptr = &Map[adjacent];
			ObjectClass * occupier = cellptr->Cell_Occupier();
			if (occupier != NULL && occupier->RTTI == RTTI_UNIT && ((UnitClass *)occupier)->Class->IsSmallVisceroid) {
				UnitClass * visceroid = (UnitClass *)occupier;
				if (visceroid->NavCom != NULL || visceroid->TarCom != NULL) { // RA2: a busy neighbor ends the search
					return; // RA2: as above
				}
				visceroid->Assign_Destination(this, true); // RA2: the neighbor is sent to this unit at once
				VisceroidFacing = FACING_NONE;
				return;
			}
		}
	}

	if (NavCom != NULL || TarCom != NULL) { // RA2: no wandering while a destination or target is set
		return; // RA2: as above
	}

	FacingType dir;
	if (VisceroidFacing == FACING_NONE || Random_Pick(0, 2) == 0) { // RA2: as above
		dir = Random_Pick(FACING_FIRST, FacingType(FACING_COUNT - 1));
		VisceroidFacing = dir;
	} else {
		dir = VisceroidFacing;
	}
	Cell adjacent = Adjacent_Cell_By_Value(cell, dir); // RA2: as above
	CellClass * cellptr = &Map[adjacent];
	if (Can_Enter_Cell(cellptr, dir, Get_Cell_Height(), &Map[cell], true) == MOVE_OK) { // RA2: no healing logic; the neighbor cell is simply entered
		Assign_Destination(cellptr, true); // RA2: as above
		return; // RA2: as above
	}
	VisceroidFacing = FACING_NONE;
}
#endif



/// <summary>
/// RA2
/// </summary>
static inline bool _Has_Room(int maxpass, CargoClass const & cargo, RadioClass * from)
{
	return cargo.GetTotalSize() + from->Techno_Type_Class()->Size <= maxpass;
}


/***********************************************************************************************
 * UnitClass::Receive_Message -- Handles receiving a radio message.                            *
 *                                                                                             *
 *    This is the handler function for when a unit receives a radio                            *
 *    message. Typical use of this is when a unit unloads from a hover                         *
 *    class so that clearing of the transport is successful.                                   *
 *                                                                                             *
 * INPUT:   from     -- Pointer to the originator of the message.                              *
 *                                                                                             *
 *          message  -- The radio message received.                                            *
 *                                                                                             *
 *          param    -- Reference to an optional parameter the might be needed to return       *
 *                      information back to the originator of the message.                     *
 *                                                                                             *
 * OUTPUT:  Returns with the radio message response.                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/22/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
RadioMessageType UnitClass::Receive_Message(RadioClass * from, RadioMessageType message, int & param)
{
	TechnoClass * sender = (TechnoClass *)from; // RA2: the sender as a techno

	switch (message) {

		case RADIO_WANT_RIDE:
			if (IsBeingWarpedOut()) return(RADIO_STATIC); // RA2: a warping unit takes no passengers
			{
				CellClass & tile = Map[Destination_Coord()]; // RA2: as above
				if (tile.IsUnderBridge && !Is_Moving_Onto_Bridge()) return(RADIO_NEGATIVE); // RA2: as above
			}
			if (Mission == MISSION_UNLOAD) return(RADIO_NEGATIVE); // RA2: as above
			if (IsTethered) return(RADIO_NEGATIVE); // RA2: as above
			return(CurrentTube == TUBE_NONE ? RADIO_ROGER : RADIO_NEGATIVE); // RA2: as above

		case RADIO_HOLD_STILL:
			BASECLASS::Receive_Message(from, message, param);
			Assign_Destination(NULL, true); // RA2: as above
			Assign_Target(NULL);
			QueueMission(MISSION_SLEEP, false); // RA2: queued, not assigned
			Clear_Navigation_List();
			if (!IsTethered || Radio_Link(0) == NULL) { // RA2: as above
				Transmit_Message(RADIO_HELLO, sender); // RA2: as above
				Transmit_Message(RADIO_TETHER);
			}
			return(RADIO_ROGER);

		/*
		**	Asks if the passenger can load on this transport.
		*/
		case RADIO_CAN_LOAD:
			if (Class->MaxPassengers != 0 && from != NULL && House->Is_Ally(sender)) { // RA2: as above
				if (IsBeingWarpedOut()) return(RADIO_STATIC); // RA2: as above
				if (sender->MindControlledBy == NULL && (!(from->AbstractFlags & 4) || ((FootClass *)from)->ParasiteEatingMe == NULL)) { // RA2: a mind controlled or eaten passenger is refused
					if (sender->CaptureManager != NULL && sender->CaptureManager->Unit != NULL) return(RADIO_STATIC); // RA2: as above
					int capacity = Class->MaxPassengers; // RA2: the cargo size must fit the capacity and the size limit
					if (!(Cargo.GetTotalSize() + sender->Techno_Type_Class()->Size <= capacity)) goto no_transport_room; // RA2: the executable shares one refusal exit
					if (sender->Techno_Type_Class()->Size > Class->SizeLimit) goto no_transport_room; // RA2: as above
					return(RADIO_ROGER);
				}
			}
			return(RADIO_STATIC); // RA2: as above

		/*
		**	The refinery has told this harvester that it should begin the backup procedure
		**	so that proper unloading may take place.
		*/
		case RADIO_BACKUP_NOW:
			BASECLASS::Receive_Message(from, message, param);
			if (!IsRotating && PrimaryFacing.Current().Facing != 0x4000) { // RA2: turn to face east first
				Locomotion->Do_Turn((DirStruct &)DirType(0x4000)); // RA2: as above
				return(RADIO_ROGER); // RA2: as above
			}
			if (!Locomotion->Is_Moving()) { // RA2: as above
				TechnoClass * whom = Radio_Link(0); // RA2: as above
				if (IsTethered && whom != NULL && whom->Fetch_RTTI() == RTTI_BUILDING && Mission == MISSION_ENTER) { // RA2: as above
					Transmit_Message(RADIO_IM_IN, whom); // RA2: as above
				}
			}
			return(RADIO_ROGER);

		/*
		**	This message is sent by the passenger when it determines that it has
		**	entered the transport.
		*/
		case RADIO_IM_IN:
			{
				int capacity = Class->MaxPassengers; // RA2: the size of the cargo against the capacity
				if (Cargo.GetTotalSize() == capacity) { // RA2: as above
					APC_Close_Door(); // RA2: as above
				}
			}
			return(RADIO_ATTACH);

		/*
		**	Docking maintenance message received. Check to see if new orders should be given
		**	to the impatient unit.
		*/
		case RADIO_DOCKING:

			/*
			**	If this transport is moving, then always abort the docking request.
			*/
			if ((Locomotion->Is_Moving() || NavCom != NULL) && CurrentMission == MISSION_MOVE) { // RA2: only while on a move mission
				return(RADIO_NEGATIVE);
			}

			if (Radio_Link(0) != from) { // RA2: as above
				int capacity = Class->MaxPassengers; // RA2: as above
				if (Cargo.GetTotalSize() + sender->Techno_Type_Class()->Size <= capacity && !(sender->Techno_Type_Class()->Size > Class->SizeLimit)) { // RA2: as above
					if (!HasAnyLink()) { // RA2: as above
						Transmit_Message(RADIO_HELLO, sender); // RA2: as above
					}
				} else {
					goto no_transport_room; // RA2: as above
				}
			}

			if (NavCom == NULL) { // RA2: a passenger in another zone, or on water, is called over
				Cell cell = PositionCell; // RA2: as above
				CellClass & tile = Map[cell]; // RA2: as above
				int own_zone = Map.Get_Cell_Zone(LastAdjacencyCell, sender->Techno_Type_Class()->MZone, false); // RA2: as above
				int sender_zone = -1; // RA2: as above
				if (from != NULL && (from->AbstractFlags & 4)) { // RA2: as above
					sender_zone = Map.Get_Cell_Zone(((FootClass *)from)->LastAdjacencyCell, sender->Techno_Type_Class()->MZone, false); // RA2: as above
				}
				if (own_zone != sender_zone) { // RA2: as above
					Assign_Destination(from, true); // RA2: as above
					return(RADIO_DOCKING); // RA2: as above
				}
				if (NavCom == NULL && tile.Land_Type() == LAND_WATER) { // RA2: as above
					Assign_Destination(from, true); // RA2: as above
					return(RADIO_DOCKING); // RA2: as above
				}
			}

			if (Class->MaxPassengers > 0) { // RA2: as above
				int capacity = Class->MaxPassengers; // RA2: as above
				if (Cargo.GetTotalSize() + sender->Techno_Type_Class()->Size <= capacity && !(sender->Techno_Type_Class()->Size > Class->SizeLimit)) { // RA2: as above
					BASECLASS::Receive_Message(from, message, param); // RA2: as above

					if (!Locomotion->Is_Moving() && !IsRotating && !IsTethered && Transmit_Message(RADIO_NEED_TO_MOVE, sender) == RADIO_ROGER) { // RA2: as above
						param = (int)this;
						if (Transmit_Message(RADIO_MOVE_HERE, param, sender) != RADIO_ROGER) { // RA2: as above
							Transmit_Message(RADIO_OVER_OUT, sender); // RA2: as above
						}
					}
					return(RADIO_ROGER); // RA2: as above
				}
			}
			break;

		/*
		**	Something bad has happened to the object in contact with. Abort any coordinated
		**	activity with this object. Basically, ... run away! Run away!
		*/
		case RADIO_RUN_AWAY:
			if ((Class->IsToHarvest || Class->IsToVeinHarvest) && IsDumping) { // RA2: as above
				IsDumping = false; // RA2: as above
				Scatter(COORD_NONE, true, false); // RA2: as above
				QueueMission(MISSION_HARVEST, false); // RA2: queued, not assigned
				if (Ready_To_Commence()) { // RA2: as above
					Commence(); // RA2: as above
				}
			}
			break;

		/*
		**	When this message is received, it means that the other object
		**	has already turned its radio off. Turn this radio off as well.
		*/
		case RADIO_OVER_OUT:
			if (Mission == MISSION_RETURN) {
				QueueMission(MISSION_GUARD, false); // RA2: queued, not assigned
			}
			BASECLASS::Receive_Message(from, message, param);
			return(RADIO_ROGER);

	}
	return(BASECLASS::Receive_Message(from, message, param));

no_transport_room: // RA2: tested: separate returns repeat the epilogue; the goto form shares one refusal exit as the executable does
	return(RADIO_NEGATIVE); // RA2: as above
}




/***********************************************************************************************
 * UnitClass::Unlimbo -- Removes unit from stasis.                                             *
 *                                                                                             *
 *    This routine will place a unit into the game and out of its limbo                        *
 *    state. This occurs whenever a unit is unloaded from a transport.                         *
 *                                                                                             *
 * INPUT:   coord    -- The coordinate to make the unit appear.                                *
 *                                                                                             *
 *          dir      -- The initial facing to impart upon the unit.                            *
 *                                                                                             *
 * OUTPUT:  bool; Was the unit unlimboed successfully?  If the desired                         *
 *                coordinate is illegal, then this might very well return                      *
 *                false.                                                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/22/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool UnitClass::Unlimbo(Coord const & coord, Dir256 dir)
{
	if (FootClass::Unlimbo(coord, dir)) { // RA2: RA2's form
		SecondaryFacing.Set(dir);
		if (IsCloakable && !IsLocked) { // RA2: as above
			Cloak = CLOAKED; // RA2: as above
		}
		if (!Class->IsSmallVisceroid && !Class->IsLargeVisceroid) { // RA2: as above
			Set_Stage(0);
			Set_Rate(0);
			return(true); // RA2: as above
		}
		Set_Stage(Random_Pick(0, 29)); // RA2: as above
		Set_Rate(1); // RA2: as above
		return(true);
	}
	return(false);
}



/***********************************************************************************************
 * UnitClass::Take_Damage -- Inflicts damage points on a unit.                                 *
 *                                                                                             *
 *    This routine will inflict the specified number of damage points on                       *
 *    the given unit. If the unit is destroyed, then this routine will                         *
 *    remove the unit cleanly from the game. The return value indicates                        *
 *    whether the unit was destroyed. This will allow appropriate death                        *
 *    animation or whatever.                                                                   *
 *                                                                                             *
 * INPUT:   damage-- The number of damage points to inflict.                                   *
 *                                                                                             *
 *          distance -- The distance from the damage center point to the object's center point.*
 *                                                                                             *
 *          warhead--The type of damage to inflict.                                            *
 *                                                                                             *
 *          source   -- Who is responsible for this damage?                                    *
 *                                                                                             *
 * OUTPUT:  Returns the result of the damage process. This can range from RESULT_NONE up to    *
 *          RESULT_DESTROYED.                                                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/30/1991 JLB : Created.                                                                 *
 *   07/12/1991 JLB : Script initiated by unit destruction.                                    *
 *   04/15/1994 JLB : Converted to member function.                                            *
 *   04/16/1994 JLB : Warhead modifier.                                                        *
 *   06/03/1994 JLB : Added the source of the damage target value.                             *
 *   06/20/1994 JLB : Source is a base class pointer.                                          *
 *   11/22/1994 JLB : Shares base damage handler for techno objects.                           *
 *   06/30/1995 JLB : Lasers do maximum damage against gunboat.                                *
 *   08/16/1995 JLB : Harvester crushing doesn't occur on early missions.                      *
 *=============================================================================================*/
ResultType UnitClass::Take_Damage(int & damage, int distance, WarheadTypeClass const * warhead, TechnoClass * source, bool forced, bool no_crew)
{
	Coord animation_position; // RA2: one scratch position serves the splash and the passengers

	/*
	**	Remember if this object was selected. If it was and it gets destroyed and it has
	**	passengers that pop out, then the passengers will inherit the select state.
	*/
	bool select = (IsSelected && House->Is_Player_Control());

	/*
	**	In order for a this to be damaged, it must either be a unit
	**	with a crew or a sandworm.
	*/
	ResultType res = BASECLASS::Take_Damage(damage, distance, warhead, source, forced, no_crew); // RA2: as above

	if (res == RESULT_ALREADY_DESTROYED) {
		return(res);
	}

	if (res == RESULT_DESTROYED) {
		if (Class->DeathFrames > 0) {
			if (DeathCounter == -1) {
				DeathCounter = 0;
				Death_Announcement(source); // RA2: the announcement is made when the death counter starts
			}
			Strength = 1;
			IsActive = true;
		} else {
				if (Class->Naval && !Class->Underwater && !Class->Organic && Class->Weight >= Rule->ShipSinkingWeight) { // RA2: a heavy ship over water sinks
					Coord position = PositionCoord; // RA2: as above
					Cell cell = position.As_Cell(); // RA2: as above
					if (Map[cell].Is_Tile_With_Water() && !WarpingOut) { // RA2: as above
						Death_Announcement(source); // RA2: as above
						Strength = 1; // RA2: as above
						IsActive = true; // RA2: as above
						IsSinking = true; // RA2: as above
						Stun(); // RA2: as above
						goto remove_passengers; // RA2: a sinking ship skips the explosion
					}
				}
				Death_Announcement(source); // RA2: as above
				if (warhead == Rule->FirestormWarhead) { // RA2: as above
					int count = abs(Scen->RandomNumber % 3) + 7; // RA2: seven to nine firestorm particle systems
					while (count > 0) { // RA2: as above
						ParticleSystemClass * partsys = new ParticleSystemClass(Rule->DefaultFirestormExplosionSystem, Center_Coord(), NULL, this, COORD_NONE); // RA2: as above
						partsys->Sparks_To_Use_Random_Direction(); // RA2: as above
						count--; // RA2: as above
					}
				} else {
					if (HeightAGL <= 10 && IsToExplode) { // RA2: a wreck over water splashes
						animation_position = PositionCoord; // RA2: as above
						if (Map[animation_position].Land_Type() == LAND_WATER) { // RA2: as above
							new AnimClass(Rule->Wake, (animation_position = PositionCoord), 0, 1, ShapeFlags_Type(0x600), 0); // RA2: as above
							new AnimClass(Rule->SplashList[Rule->SplashList.Count() - 1], (animation_position = PositionCoord + Coord(0, 0, 5)), 0, 1, ShapeFlags_Type(0x600), 0); // RA2: as above
							goto remove_passengers; // RA2: a splash skips the explosion
						}
					}
					Explode(); // RA2: as above
				}
		}

remove_passengers: // RA2: structured exits (do/while) moved the sinking block out of line; the goto form keeps the executable's order
		Mark(MARK_UP); // RA2: as above

		if (!Class->Crashable) { // RA2: a crashable aircraft-like unit keeps its cargo
			while (Cargo.Is_Something_Attached()) {
				FootClass * object = Unload_Passenger(); // RA2: FootClass 0x4CCFC0, not CargoClass::Detach_Object

				if (object == NULL) break;		// How can this happen?

				MoveType move = object->Can_Enter_Cell(&Map[(Coord const &)PositionCoord], FACING_NONE, -1, NULL, true); // RA2: the test comes before the scenario flag is raised
				bool can_place = move == MOVE_OK || move == MOVE_MOVING_BLOCK; // RA2: as above
				ScenarioInit++;
				object->IsOnBridge = IsOnBridge;
				animation_position = PositionCoord; // RA2: as above
				animation_position.Z = Get_Cell_Ptr()->Center_Coord().Z; // RA2: as above
				if (!forced && !IsToExplode && can_place && object->Unlimbo(animation_position, PrimaryFacing.Current().As_Dir256())) { // RA2: any passenger can run, not only infantry
					object->Scatter(COORD_NONE, true, false); // RA2: as above
					if (!House->Is_Human_Player()) { // RA2: as above
						if (Team != NULL) { // RA2: as above
							Team->Add(object); // RA2: as above
						} else {
							object->QueueMission(MISSION_HUNT, false); // RA2: as above
						}
					}
					if (select) object->Select();
				} else {
					object->Record_The_Kill(source);
					delete object;
				}

				ScenarioInit--;
			}
		}

		/*
		**	Possibly have the crew member run away.
		*/
		InfantryClass * i = NULL; // RA2: the crew is built by a shared exit, not flags
		do { // RA2: as above
			if (EnteredByInfType == INFANTRY_NONE) {
				if (no_crew || !Class->IsCrew || Class->Max_Passengers() != 0 || Random_Double(0.0, 1.0) >= Rule->CrewEscape) { // RA2: as above
					break; // RA2: as above
				}
			}
			if (EnteredByInfType != INFANTRY_NONE) { // RA2: as above
				i = new InfantryClass(InfantryTypes[EnteredByInfType], House);
			} else {
				InfantryTypeClass const * crew = Crew_Type();
				if (crew == NULL) break; // RA2: as above
				i = new InfantryClass(crew, House); // RA2: as above
			}
			if (i != NULL) {
				i->IsOnBridge = IsOnBridge;
				if (i->Unlimbo(PositionCoord, DIR_N)) {
					i->Strength = Random_Pick(5, (int)i->Class->MaxStrength/2);
					i->Scatter(COORD_NONE, true, false); // RA2: as above
					if (!House->Is_Human_Player()) {
						i->QueueMission(MISSION_HUNT, false); // RA2: queued, not assigned
					} else {
						i->QueueMission(MISSION_GUARD, false); // RA2: as above
					}
					if (select) i->Select();
					if (Tag != NULL && Tag->Is_To_Inherit()) i->Attach_Tag(Tag);
				} else {
					delete i;
				}
			}
		} while (false); // RA2: as above

		/*
		**	If this is a truck, there is a possibility that a crate will drop out
		**	if the scenario so indicates and there is room.
		*/
		if (Class->IsCarriesCrate && (Scen->IsTruckCrate && !Class->IsTrain || Scen->IsTrainCargo && Class->IsTrain)) { // RA2: as above
			Cell cell = Map.Nearby_Location(PositionCell, SPEED_TRACK, -1, MZONE_NORMAL, false, Point2D(1,1), true, false, false, true, Cell(0, 0), false); // RA2: the crate is placed with Place_Crate
			if (cell != CELL_NONE && Session.Options.Goodies) { // RA2: only when the crates option is on
				Map.Place_Crate(cell, (CrateType)20); // RA2: crate type 20, the truck's crate, outside the CrateType list
			}
		}

		/*
		**	Finally, delete the vehicle.
		*/
		if (Class->Crashable) { // RA2: a crashable unit crashes instead of vanishing
			if (!Crash(NULL)) { // RA2: as above
				Delete_Me(); // RA2: as above
			}
		} else if (!IsSinking) { // RA2: a sinking ship stays until it is under
			Delete_Me();
		}

	} else {

		if (res != RESULT_NONE) {
			if (Class->IsToHarvest && House == PlayerPtr) {
				if (Submit_Radar_Event(RADAREVENT_HARVESTER_ATTACKED, Destination_Coord().As_Cell())) {
					Queue_Speech("EVA_OreMinerUnderAttack", -1, -1); // RA2: the EVA line by index
				}
			}
		}

		/*
		**	Try to crush anyone that fires on this unit if possible. The harvester
		**	typically is the only one that will qualify here.
		*/
		if (Team == NULL && source != NULL && !IsTethered && !House->Is_Ally(source) && !House->Is_Human_Player()) {
			if (Should_Crush_It(source)) {
				Assign_Destination(source);
				QueueMission(MISSION_MOVE, false); // RA2: queued, not assigned
			} else if ((Class->IsToHarvest || Class->IsToVeinHarvest) && Pip_Count() > 0 && HealthRatio <= Rule->ConditionYellow) { // RA2: as above
				BuildingClass * building = NULL;
				for (int i = 0; i < Class->Dock.Count(); i++) { // RA2: as above
					building = FindNearestDockBuilding((BuildingTypeClass *)Class->Dock[i], 0, 0); // RA2: as above
					if (building != NULL) { // RA2: as above
						break; // RA2: as above
					}
				}
				if (building != NULL && !ContainsLink(building) && Transmit_Message(RADIO_HELLO, building) == RADIO_ROGER) { // RA2: as above
					QueueMission(MISSION_ENTER, false); // RA2: queued, not assigned
				}
			}
		}
	}
	return(res);
}




/// <summary>
/// RA2
/// </summary>
static void Shake_On_Explosion(int strength)
{
	if (strength > Rule->ShakeScreen) {
		int shakes = strength / (Rule->ShakeScreen / 2) + 3;
		Shake_The_Screen(MIN(shakes, 6));
	}
}


/// <summary>
/// Creates the explosion that accompanies this unit's destruction.
/// A loaded harvester adds a blast in proportion to the Tiberium it was carrying, and a
/// sufficiently sturdy vehicle will rock the screen on its way out.
/// </summary>
void UnitClass::Explode(void)
{
	if (Class->Explosion.Count() > 0) {
		AnimTypeClass const * anim = Class->Explosion.Pick(Scen->RandomNumber);

		if (Class->IsExploding || Has_Ability(ABILITY_EXPLODES)) {
			if (Class->MaxAmmo == -1 || Ammo > 0) {
				anim = Class->Explosion[Class->Explosion.Count() - 1];
			}
		}

		new AnimClass(anim, PositionCoord);

		// The game sums the cargo power here without applying area damage.
		if (Storage.Get_Total_Amount() > 0 && Rule->IsExplosiveHarvester && !Scen->Special.IsHarvesterImmune) {
			int power = 0;
			for (int i = 0; i < Tiberiums.Count(); i++) {
				power += Storage.Get_Amount(i) * Tiberiums[i]->Power;
			}
		} // RA2: Explode at 0x6FD3A0 has no PositionCoord, Rule, C4Warhead

		Shake_On_Explosion(Class->MaxStrength); // RA2: Explode at 0x6FD3A0 matches only so
	}

	if (Class->DestroyAnim.Count() > 0) { // RA2: as above
		AnimTypeClass const * anim = Class->DestroyAnim.Pick(Scen->RandomNumber); // RA2: as above
		new AnimClass(anim, PositionCoord); // RA2: as above
	}
}


/***********************************************************************************************
 * UnitClass::Active_Click_With -- Intercepts the active click to see if deployment is possible*
 *                                                                                             *
 *    This routine intercepts the active click operation. It check to see if this is a self    *
 *    deployment request (MCV's have this ability). If it is, then the object is initiated     *
 *    to self deploy. In the other cases, it passes the operation down to the lower            *
 *    classes for processing.                                                                  *
 *                                                                                             *
 * INPUT:   action   -- The action requested of the unit.                                      *
 *                                                                                             *
 *          object   -- The object that the mouse pointer is over.                             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/10/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool UnitClass::Active_Click_With(ActionType action, ObjectClass * object, bool is_waypoint)
{
	ActionType what = What_Action(object, is_waypoint); // RA2: asked once
	if (action != what) { // RA2: as above
		action = what; // RA2: as above
		switch (action) {
			case ACTION_ENTER:
				action = ACTION_MOVE; // RA2: only enter is turned into move here
				break;

			case ACTION_CAPTURE:
			case ACTION_SABOTAGE:
				action = ACTION_ATTACK;
				break;

			default:
				break;
		}
	}

	if (action == ACTION_HEAL || action == ACTION_GREPAIR) {
		action = ACTION_ATTACK;
	}

	/*
	**	Short circuit out if trying to tell a unit to "nomove" to itself. This bypass of the
	**	normal active click with logic prevents any disturbance to the vehicle's state. Without
	**	this bypass, a unit on a repair bay would stop repairing because it would break radio
	**	contact.
	*/
	if (object == this && (action == ACTION_NOMOVE || action == ACTION_ATTACK)) { // RA2: an attack on itself is refused as well
		return(false);
	}

	return(BASECLASS::Active_Click_With(action, object, is_waypoint));
}



/***********************************************************************************************
 * UnitClass::Active_Click_With -- Performs specified action on specified cell.                *
 *                                                                                             *
 *    This routine is called when the mouse has been clicked over a cell and this unit must    *
 *    now respond. Notice that this is merely a placeholder function that exists because there *
 *    is another function of the same name that needs to be overloaded. C++ has scoping        *
 *    restrictions when there are two identically named functions that are overridden in       *
 *    different classes -- it handles it badly, hence the existence of this routine.           *
 *                                                                                             *
 * INPUT:   action   -- The action to perform on the cell specified.                           *
 *                                                                                             *
 *          cell     -- The cell that the action is to be performed on.                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool UnitClass::Active_Click_With(ActionType action, Cell const & cell, Cell const & cell2, bool is_waypoint) // RA2: a second cell
{
	return(BASECLASS::Active_Click_With(action, cell, cell2, is_waypoint)); // RA2: as above
}


/***********************************************************************************************
 * UnitClass::Enter_Idle_Mode -- Unit enters idle mode state.                                  *
 *                                                                                             *
 *    This routine is called when the unit completes one mission but does not have a clear     *
 *    follow up mission to perform. In such a case, the unit should enter a default idle       *
 *    state. This idle state varies depending on what the current internal computer            *
 *    settings of the unit is as well as what kind of unit it is.                              *
 *                                                                                             *
 * INPUT:   initial  -- Is this called when the unit just leaves a factory or is initially     *
 *                      or is initially placed on the map?                                     *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/31/1994 JLB : Created.                                                                 *
 *   06/03/1994 JLB : Fixed to handle non-combat vehicles.                                     *
 *   06/18/1995 JLB : Allows a harvester to stop harvesting.                                   *
 *=============================================================================================*/
bool UnitClass::Enter_Idle_Mode(bool initial, bool resume_waypoint)
{
	bool res = BASECLASS::Enter_Idle_Mode(initial, resume_waypoint);

	if (HaveMegaMission() || Mission == MISSION_WAIT) { // RA2: a mega mission or a wait order keeps its state
		return(res); // RA2: as above
	}

	/*
	**	A movement mission without a NavCom would be pointless to have a radio contact since
	**	no radio coordination occurs on a just a simple movement mission.
	*/
	if (Mission == MISSION_MOVE && NavCom == NULL) { // RA2: as above
		Coord center = PositionCell.As_Coord(); // RA2: the cell center is compared component by component
		Coord coord = PositionCoord; // RA2: as above
		if (coord.X == center.X && coord.Y == center.Y && coord.Z == center.Z) { // RA2: as above
			Transmit_Message(RADIO_OVER_OUT); // RA2: as above
		}
	}

	Handle_Navigation_List();

	MissionType order; // RA2: the order to queue, chosen by the cases below
	bool reset = false; // RA2: the harvester and idle cases clear the target and the destination first
	if (NavCom != NULL) {
		order = MISSION_MOVE;
	} else if (!Class->IsToHarvest && !Class->IsToVeinHarvest) { // RA2: no IsMobileEMP and no weeder case
		if (!Is_Weapon_Equipped()) { // RA2: as above
			if (IsALoaner && Class->MaxPassengers > 0 && Cargo.Is_Something_Attached() && Team == NULL) { // RA2: a loaner with cargo unloads
				order = MISSION_UNLOAD; // RA2: as above
			} else {
				if (IsDeploying) { // RA2: a deploying unit is left alone
					return(res); // RA2: as above
				}
				if (CurrentMission == MISSION_UNLOAD && Class->DeploysInto != NULL) { // RA2: as above
					return(res); // RA2: as above
				}
				if (CurrentMission == MISSION_GUARD_AREA) { // RA2: as above
					return(res); // RA2: as above
				}
				order = MISSION_GUARD; // RA2: as above
				reset = true; // RA2: as above
			}
		} else {
			if (Mission == MISSION_GUARD || Mission == MISSION_GUARD_AREA) { // RA2: an armed unit already on guard stays
				return(res); // RA2: as above
			}
			if (Mission != MISSION_NONE && (Current_Mission_Control().IsParalyzed || Current_Mission_Control().IsZombie)) { // RA2: as above
				return(res); // RA2: as above
			}
			if (!(House->IQ >= Rule->IQGuardArea || Has_Ability(ABILITY_GUARD_AREA) || Class->DefaultToGuardArea) || Team != NULL) { // RA2: the guard area order needs IQ, the ability or the type flag
				order = MISSION_GUARD; // RA2: as above
			} else {
				order = MISSION_GUARD_AREA; // RA2: as above
			}
		}
	} else {
		if (HasAnyLink()) { // RA2: a harvester in radio contact stays
			return(res); // RA2: as above
		}
		if (Mission == MISSION_HARVEST || MissionQueue == MISSION_HARVEST) { // RA2: as above
			return(res); // RA2: as above
		}
		order = MISSION_HARVEST; // RA2: as above
		reset = true; // RA2: as above
		if (!initial && House->Is_Human_Player()) { // RA2: a human harvester standing on its own ore keeps its order
			if (Map[(Coord const &)PositionCoord].Land_Type() != (Class->IsToHarvest ? LAND_TIBERIUM : LAND_WEEDS)) { // RA2: as above
				order = MISSION_GUARD; // RA2: as above
			}
		}
	}

	if (reset) { // RA2: as above
		Assign_Target(NULL); // RA2: as above
		Assign_Destination(NULL, true); // RA2: as above
	}
	if (CurrentMission != MISSION_PATROL && CurrentMission != MISSION_GUARD_AREA && CurrentMission != MISSION_UNLOAD) { // RA2: an unload mission is never replaced
		QueueMission(order, false); // RA2: queued, not assigned
	}
	return(res);
}




/// <summary>
/// RA2
/// </summary>
static inline Cell Deploy_Origin(Cell const & cell) // RA2: the building origin for a unit cell, one cell north-west
{
	return cell + AdjacentCell[7]; // RA2: as above
}


/// <summary>
/// RA2
/// </summary>
static inline Cell Offset_Cell(Cell const & offset, Cell const & position) // RA2: a search offset added to the unit's cell
{
	Cell result; // RA2: as above
	result.X = offset.X; // RA2: as above
	result.Y = offset.Y; // RA2: as above
	result.X += position.X; // RA2: as above
	result.Y += position.Y; // RA2: as above
	return result; // RA2: as above
}


/***********************************************************************************************
 * UnitClass::Goto_Clear_Spot -- Finds a clear spot to deploy.                                 *
 *                                                                                             *
 *    This routine is used by the MCV to find a clear spot to deploy. If a clear spot          *
 *    is found, then the MCV will assign that location to its navigation computer. This only   *
 *    occurs if the MCV isn't already heading toward a spot.                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool;  Is the located at a spot where it can deploy?                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/27/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool UnitClass::Goto_Clear_Spot(void)
{
	if (!Class->DeploysInto) {
		return(true);
	}

	Mark(MARK_UP);

	bool shipyard = false; // RA2: a construction yard type that may also need a shipyard-sized area
	for (int index = 0; index < Rule->BuildConst.Count(); index++) { // RA2: the type is one of the construction yard types
		if (Rule->BuildConst[index] == Class->DeploysInto) { // RA2: as above
			if (Owner_HouseClass()->IsShipyardConYardWanted) { // RA2: the house wants its next yard placed where a shipyard fits too
				if (NavCom == NULL) { // RA2: as above
					Cell first; // RA2: the area found for the shipyard
					{
						Cell zero(0, 0); // RA2: a block local of its own
						first = Map.Nearby_Location(Get_Cell(), SPEED_FLOAT, -1, MZONE_NORMAL, false, Point2D(House->Get_First_Ownable(Rule->Shipyard)->Width() + 1, House->Get_First_Ownable(Rule->Shipyard)->Height(false) + 1), false, false, false, true, zero, false); // RA2: as above
					}
					{
						Cell zero(0, 0); // RA2: as above
						first = Map.Nearby_Location(first, SPEED_TRACK, -1, MZONE_NORMAL, false, Point2D(House->Get_First_Ownable(Rule->BuildConst)->Width() + 1, House->Get_First_Ownable(Rule->BuildConst)->Height(false) + 1), false, false, false, true, zero, false); // RA2: the yard-sized area near that one
					}
					if (first != CELL_NONE) { // RA2: as above
						Assign_Destination(&Map[first], true); // RA2: as above
					}
					Owner_HouseClass()->IsShipyardConYardWanted = false; // RA2: as above
				}
				shipyard = true; // RA2: as above
			}
			break; // RA2: as above
		}
	}

	if (!shipyard && NavCom == NULL) { // RA2: as above
		if (Class->DeploysInto->Legal_Placement(Deploy_Origin(Center_Coord().As_Cell()), NULL)) { // RA2: as above
			Mark(MARK_DOWN); // RA2: as above
			return(true); // RA2: as above
		}

		if (NavCom == NULL) { // RA2: as above
			static Cell _offsets[] = { // RA2: as above
				Cell(+0, -1), // RA2: as above
				Cell(+0, -2), // RA2: as above
				Cell(+1, -2), // RA2: as above
				Cell(-1, -2), // RA2: as above
				Cell(+0, -3), // RA2: as above
				Cell(+1, -3), // RA2: as above
				Cell(-1, -3), // RA2: as above
				Cell(+2, -3), // RA2: as above
				Cell(-2, -3), // RA2: as above
				Cell(+0, -4), // RA2: as above
				Cell(+1, -4), // RA2: as above
				Cell(-1, -4), // RA2: as above
				Cell(+2, -4), // RA2: as above
				Cell(-2, -4), // RA2: as above
//BG: Added south scanning
				Cell(+0, +1), // RA2: as above
				Cell(+0, +2), // RA2: as above
				Cell(+1, +2), // RA2: as above
				Cell(-1, +2), // RA2: as above
				Cell(+0, +3), // RA2: as above
				Cell(+1, +3), // RA2: as above
				Cell(-1, +3), // RA2: as above
				Cell(+2, +3), // RA2: as above
				Cell(-2, +3), // RA2: as above
				Cell(+0, +4), // RA2: as above
				Cell(+1, +4), // RA2: as above
				Cell(-1, +4), // RA2: as above
				Cell(+2, +4), // RA2: as above
				Cell(-2, +4), // RA2: as above

//BG: Added some token east/west scanning
				Cell(-1, +0),Cell(-2, +0),Cell(-3, +0),Cell(-4, +0), // RA2: as above

				Cell(+1, +0),Cell(+2, +0),Cell(+3, +0),Cell(+4, +0) // RA2: as above
			};

			for (int i = 0; i < ARRAY_SIZE(_offsets); i++) { // RA2: as above
				Cell	cell = Offset_Cell(_offsets[i], Get_Cell()); // RA2: the search offset added to the unit's cell
				Cell	check_cell = cell + AdjacentCell[7]; // RA2: the building origin is one cell north-west
				if (Class->DeploysInto->Legal_Placement(check_cell, NULL)) { // RA2: as above
					Assign_Destination(&Map[cell], true); // RA2: as above
					break; // RA2: as above
				}
			}
		}
	}
	Mark(MARK_DOWN);

	/*
	**	If we couldn't find a destination to go to, let's try random movement
	**	to see if that brings us to a better spot.
	*/
	if (NavCom == NULL && !House->Is_Human_Player()) {
		Scatter(COORD_NONE);
	}

	return(false);
}







/***********************************************************************************************
 * UnitClass::Try_To_Deploy -- The unit attempts to "deploy" at current location.              *
 *                                                                                             *
 *    Certain units have the ability to deploy into a building. When this routine is called    *
 *    for one of those units, it will attempt to deploy at its current location. If the unit   *
 *    is in motion to a destination or it isn't one of the special units that can deploy or    *
 *    it isn't allowed to deploy at this location for some reason it won't deploy. In all      *
 *    other cases, it will begin to deploy and once it begins only a player abort action will  *
 *    stop it.                                                                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was deployment begun?                                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/18/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool UnitClass::Try_To_Deploy(void)
{
	if (Can_Deploy_Now()) { // RA2: the type may deploy now (virtual); the flag is cleared at the end otherwise
		if (NavCom != NULL) { // RA2: a moving unit keeps its deploy flag
			return(false); // RA2: as above
		}
		if (Locomotion->Is_Moving()) { // RA2: as above
			return(false); // RA2: as above
		}
		if (Class->DeploysInto == NULL) { // RA2: as above
			return(false); // RA2: as above
		}

		/*
		**	Determine if it is legal to deploy at this location. If not, tell the
		**	player.
		*/
		Mark(MARK_UP); // RA2: as above
		Locomotion->Mark_All_Occupation_Bits(MARK_UP); // RA2: as above
		Cell ncell = PositionCell; // RA2: as above
		Cell cell; // RA2: as above
		if (Class->DeploysInto->Is_Mobile_Deployer()) { // RA2: as above
			cell = ncell; // RA2: as above
		} else {
			cell = ncell + AdjacentCell[7]; // RA2: the building origin is one cell north-west
		}
		if (!Class->DeploysInto->Legal_Placement(cell, NULL)) { // RA2: as above
			if (House->Is_Player_Control()) { // RA2: as above
				Queue_Speech("EVA_CannotDeployHere", -1, -1); // RA2: the EVA line by name
			}
			if (!House->Is_Human_Player()) { // RA2: as above
				Class->DeploysInto->Flush_For_Placement(cell, House); // RA2: as above
			}
			Locomotion->Mark_All_Occupation_Bits(MARK_DOWN);
			Mark(MARK_DOWN);
		} else {
			Locomotion->Mark_All_Occupation_Bits(MARK_DOWN); // RA2: as above
			Mark(MARK_DOWN); // RA2: as above

			/*
			**	If the unit is not facing the correct direction, then start it rotating
			**	toward the right facing, but still flag it as if it had deployed. This is
			**	because it will deploy as soon as it reaches the correct facing.
			*/
			Dir256 dfacing = Class->DeploysInto->Deploy_Facing();
			if (PrimaryFacing.Current().As_Dir256() != dfacing) {
				if (!Locomotion->Is_Moving_Now()) {
					Locomotion->Do_Turn((DirStruct &)DirType(dfacing));
				}
				Transmit_Message(RADIO_OVER_OUT);
				IsDeploying = true;
				return(true);
			}

			bool selected = IsSelected;

			/*
			**	Since the unit is already facing the correct direction, actually do the
			**	deploy logic. If for some reason this cannot occur, then don't delete the
			**	unit, just mark it as not deploying.
			*/
			Mark(MARK_UP);
			Locomotion->Mark_All_Occupation_Bits(MARK_UP);
			BuildingClass * building = new BuildingClass(Class->DeploysInto, House);
			if (building != NULL) {
				building->QueueMission(MISSION_CONSTRUCTION, false); // RA2: queued, not assigned
				if (building->Unlimbo(Coord(cell.X * CELL_LEPTON_W + CELL_LEPTON_W / 2, cell.Y * CELL_LEPTON_H + CELL_LEPTON_H / 2, 0), DIR_N)) { // RA2: the cell center, no direction

					Transmit_Message(RADIO_OVER_OUT);

					for (int i = 0; i < Technos.Count(); i++) {
						TechnoClass * techno = Technos[i];
						if (techno->TarCom != NULL && techno->TarCom->RTTI == RTTI_UNIT && techno->TarCom == this && techno->IsActive && techno != this && techno != building) { // RA2: attackers of the unit retarget the building
							if (building->Class->IsConstructionYard && techno->RTTI == RTTI_INFANTRY && ((InfantryTypeClass const *)techno->Techno_Type_Class())->IsVehicleThief) { // RA2: a construction yard is not retargeted by vehicle thieves
								techno->Assign_Target(NULL); // RA2: as above
							} else {
								techno->Assign_Target(building); // RA2: as above
							}
						}
					}

					building->ActLike = ActLike;
					building->Group = Group;
					building->Veterancy = Veterancy;

					/*
					**	Play the buildup sound for the player if this is the players
					**	MCV.
					*/
					if (building->House->Is_Player_Control()) { // RA2: as above
						VocClass::PlayAt(Rule->BuildingDrop, Center_Coord(), NULL); // RA2: positional sound, no audio controller
					} else {
						building->IsToRebuild = true;
						building->IsToRepair = true;
					}

					if (building->Class->IsArtillary || building->Class->IsTickTank) {
						building->BarrelPitch.Set(DIR_E);
					}

					building->Assign_Target(TarCom);

					/*
					**	Always reveal the construction yard to the player that owned the
					**	mobile construction vehicle.
					*/
					building->Revealed(House);

					building->IsReadyToCommence = true;

					/*
					**	When the MCV deploys, always consider production to have started
					**	for the owning house. This ensures that in multiplay, computer
					**	opponents will begin construction as soon as they start their
					**	base.
					*/
					if (!House->Is_Human_Player() && building->Class->IsConstructionYard && Session.Type != GAME_NORMAL) {
						Cell base = building->PositionCoord.As_Cell(); // RA2: as above
						House->Center = base.As_Coord(); // RA2: the base center is the cell center
						House->Ensure_Base_Nodes(); // RA2: the computer house gets its base nodes when it has none
						House->Base.Nodes[0].CellID = base; // RA2: as above
						House->Base.PlacementCenter = base; // RA2: as above
						House->IsStarted = true;
						House->IsAITriggersOn = true;
						House->IsBaseBuilding = true;
						House->Recall_Units_To_Base(); // RA2: the units are sent to a free spot by the base
					}

					/*
					**	Force the newly placed construction yard to be in the same strength
					**	ratio as the MCV that deployed into it.
					*/
					building->Strength = (int)(HealthRatio * building->Class->MaxStrength); // RA2: as above
					building->Strength = max(building->Strength, 1);
					building->EstimatedHealth = building->Strength; // RA2: the estimated health starts equal to the real one

					if (selected) building->Select();

					/*
					**	Force the MCV to drop any flag it was carrying.  This will also set
					**	the owner house's flag home cell (since the house's FlagHome is
					**	presumably 0 at this point).
					*/
					Stun();

					if (Tag != NULL) {
						building->Attach_Tag(Tag);
						Tag->AttachCount--;
						Tag = NULL;
					}

					Delete_Me();
					return(true);
				} else {

					/*
					**	Could not deploy the construction yard at this location! Just revert
					**	back to normal "just sitting there" mode and await further instructions.
					*/
					delete building;
				}
			}
			Locomotion->Mark_All_Occupation_Bits(MARK_DOWN);
			Mark(MARK_DOWN);
		}
	}
	IsDeploying = false; // RA2: as above
	return(false);
}










/***********************************************************************************************
 * UnitClass::Per_Cell_Process -- Performs operations necessary on a per cell basis.           *
 *                                                                                             *
 *    This routine will perform the operations necessary that occur when a unit is at the      *
 *    center of a cell. These operations could entail deploying into a construction yard,      *
 *    radioing a transport unit, and looking around for the enemy.                             *
 *                                                                                             *
 * INPUT:   why   -- Specifies the circumstances under which this routine was called.          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/18/1994 JLB : Created.                                                                 *
 *   06/17/1995 JLB : Handles case when building says "NO!"                                    *
 *   06/30/1995 JLB : Gunboats head back and forth now.                                        *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: 2919 bytes differ (adopted text)
void UnitClass::Per_Cell_Process(PCPType why)
{
	Cell cell = Get_Cell(); // RA2: no bench timer and no visceroid merging

	if ((why == PCP_END || why == PCP_ROTATION) && IsDeploying) { // RA2: one condition
		Try_To_Deploy(); // RA2: as above
		if (!IsActive) return; // RA2: as above
	}

	if (why == PCP_END) {
		TechnoClass * contact = Radio_Link(0); // RA2: the first radio contact, read once
		if ((Get_Mission() == MISSION_ENTER || Get_Mission() == MISSION_PATROL) && contact != NULL && contact->Fetch_RTTI() == RTTI_BUILDING) { // RA2: as above
			Coord center = Center_Coord().As_Cell().As_Coord(); // RA2: the center of this cell
			Coord contact_center = contact->Docking_Coord(this).As_Cell().As_Coord(); // RA2: the center of the cell of the docking coordinate
			if (center.As_Cell() == contact_center.As_Cell()) { // RA2: as above
				IPersistPtr persist(Locomotion);
				CLSID clsid;
				persist->GetClassID(&clsid);
				if (clsid == CLSID_HoverLocomotion && static_cast<BuildingClass *>(contact)->Class->IsCanUnitRepair && NavCom == NULL) { // RA2: as above
					NavCom = contact; // RA2: as above
				}
				if (contact == NavCom) { // RA2: as above
					BASECLASS::Per_Cell_Process(PCP_END);
					Transmit_Message(RADIO_IM_IN);
					Locomotion->Power_Off();
					return; // RA2: no bench timer
				}
			}
		}

		/*
		**	If this is a unit that is driving onto a building then the unit must enter
		**	the building as the final step.
		*/
		if (IsTethered && contact != NULL && contact->Fetch_RTTI() == RTTI_BUILDING && Get_Mission() == MISSION_ENTER) { // RA2: as above
			if (contact == Map[cell - Cell(0, 1)].Cell_Building()) { // RA2: as above
				switch (Transmit_Message(RADIO_IM_IN, contact)) { // RA2: as above
					case RADIO_ROGER: // RA2: as above
						break; // RA2: as above

					case RADIO_ATTACH: // RA2: as above
						break; // RA2: as above

					default: // RA2: as above
						Scatter(COORD_NONE, true, false); // RA2: as above
						break; // RA2: as above
				}
			}
		}

		/*
		**	Unit entering a transport vehicle will break radio contact
		**	and attach itself to the transporter.
		*/
		CellClass * current = Get_Cell_Ptr(); // RA2: the transport is whatever stands on this cell
		TechnoClass * transport = current->Cell_Building(); // RA2: as above
		if (transport == NULL) { // RA2: as above
			transport = Get_Cell_Ptr()->Cell_Aircraft(IsOnBridge); // RA2: as above
			if (transport == NULL) { // RA2: as above
				transport = Get_Cell_Ptr()->Cell_Unit(IsOnBridge); // RA2: as above
			}
		}
		if (transport == this) transport = Dynamic_Cast<TechnoClass *>(transport->Next); // RA2: as above
		if (Get_Mission() == MISSION_ENTER && transport != NULL && transport != this) { // RA2: as above
			if (Get_Cell() == transport->Get_Cell() && ContainsLink(transport) && transport->Techno_Type_Class()->MaxPassengers > 0) { // RA2: as above
				if (Transmit_Message(RADIO_CAN_LOAD, transport) == RADIO_ROGER) { // RA2: as above
					ArchiveTarget = NULL; // RA2: as above
					IsOnBridge = false; // RA2: as above
					Limbo(); // RA2: as above
					transport->Cargo.Attach(this); // RA2: as above
					Hidden(); // RA2: as above
				} else {
					Assign_Destination(NULL, true); // RA2: as above
					QueueMission(MISSION_GUARD, false); // RA2: as above
					Scatter(COORD_NONE, true, true); // RA2: as above
				}
				return; // RA2: as above
			}
		}

		/*
		**	When breaking away from a transport object or building, possibly
		**	scatter or otherwise begin normal unit operations.
		*/
		if (IsTethered && (Get_Mission() != MISSION_ENTER || // RA2: as above
				(Dynamic_Cast<TechnoClass *>(NavCom) != NULL && Radio_Link(0) != NavCom) // RA2: as above
				) &&
				Get_Mission() != MISSION_UNLOAD) { // RA2: as above

			bool arrived = NavCom == NULL;
			if (NavCom != NULL && NavCom->Fetch_RTTI() == RTTI_CELL && static_cast<CellClass *>(NavCom)->CellID == Get_Coord().As_Cell()) { // RA2: as above
				arrived = true; // RA2: as above
			}

			if (Map[Get_Coord()].Cell_Building() == NULL || arrived) { // RA2: as above
				contact = Radio_Link(0); // RA2: the contact is read again
				if (arrived || contact == NULL || contact->Fetch_RTTI() != RTTI_BUILDING || !static_cast<BuildingClass *>(contact)->Class->IsWeaponsFactory || Map[Get_Coord()].Cell_Building() != contact) { // RA2: as above
					RadioMessageType response = Transmit_Message(RADIO_UNLOADED);
					if (response == RADIO_RUN_AWAY) {
						if (NavCom != NULL && NavCom != Get_Cell_Ptr()) {
							AbstractClass * destination = NavCom; // RA2: a destination that is a techno gets the docking message
							if (destination != NULL && (destination->AbstractFlags & 1)) { // RA2: as above
								Transmit_Message(RADIO_DOCKING, (TechnoClass *)destination); // RA2: as above
							}
						} else {
							BuildingClass * building; // RA2: the contact as a building, tested by hand
							if (contact == NULL) { // RA2: as above
								building = NULL; // RA2: as above
							} else {
								building = contact->Fetch_RTTI() == RTTI_BUILDING ? (BuildingClass *)contact : NULL; // RA2: as above
							}
							if (!Class->IsToHarvest && !Class->IsToVeinHarvest) { // RA2: as above
								if (!House->Is_Human_Player() && building != NULL && building->Class->IsWeaponsFactory) { // RA2: as above
									Cell where = House->Where_To_Go(this); // RA2: as above
									if (where != CELL_NONE) { // RA2: as above
										QueueMission(MISSION_MOVE, false); // RA2: QueueMission replaces Assign_Mission
										Assign_Destination(&Map[where], true); // RA2: as above
										Commence(); // RA2: as above
										ArchiveTarget = &Map[where]; // RA2: as above
										QueueMission(MISSION_GUARD_AREA, false); // RA2: as above
									} else {
										ArchiveTarget = NULL; // RA2: as above
									}
								} else {
									AbstractClass * target = ArchivedTarget; // RA2: as above
									if (target != NULL && target != NavCom) { // RA2: as above
										Assign_Destination(target, true); // RA2: as above
									} else {
										Abort_Motion(); // RA2: as above
										Scatter(COORD_NONE, true, false); // RA2: as above
									}
								}
							} else {
								QueueMission(MISSION_HARVEST, true); // RA2: a harvester goes back to harvesting at once
							}
						}

					} else if (response != RADIO_NEGATIVE && (Class->IsToHarvest || Class->IsToVeinHarvest)) { // RA2: as above
						if (ArchivedTarget != NULL) { // RA2: as above
							QueueMission(MISSION_HARVEST, false); // RA2: as above
							Assign_Destination(ArchivedTarget, true); // RA2: as above
							ArchiveTarget = NULL; // RA2: as above
						} else if (NavCom == NULL) { // RA2: as above
							Scatter(COORD_NONE, true, false); // RA2: as above
						}
					}
				}
			}

			if (Map[Get_Coord()].Cell_Building() && NavCom == NULL && NavQueue.Count() == 0 && RouteQueue.Count() == 0) { // RA2: as above
				Scatter(COORD_NONE, true, true);
			}
		}

		/*
		**	If this is a loaner unit and is is off the edge of the
		**	map, then it gets eliminated.
		*/
		if (Get_Mission() == MISSION_GUARD && !Map.In_Radar(Get_Coord()) && IsLocked) { // RA2: no Edge_Of_World_AI; the loaner leaves the map by hand
			if (Team != NULL) { // RA2: as above
				Team->IsLeaveMap = true; // RA2: as above
			}
			Stun(); // RA2: as above
			Delete_Me(); // RA2: as above
			return; // RA2: as above
		}

		RemoveSight(false, false, false, NULL); // RA2: takes the place of Look (the sight is taken back and revealed again)
		UpdateSight(false, false, false, NULL); // RA2: as above
		Map.RevealArea3(Get_Coord(), 0, LastSightRange + 3, 0); // RA2: as above

		if (!IsDumping && Ready_To_Commence()) {
			Commence();
		}

		if (Class->IsToHarvest && Get_Mission() != MISSION_UNLOAD && CurrentMission != MISSION_ENTER && MissionQueue != MISSION_ENTER && !IsDumping) { // RA2: as above
			RadioClass * radio = Radio_Link(0); // RA2: as above
			if (radio != NULL && radio->What_Am_I() == RTTI_BUILDING && static_cast<BuildingClass *>(radio)->Class->IsRefinery) {
				Transmit_Message(RADIO_OVER_OUT);
			}
		}

		if (Class->IsToVeinHarvest && Get_Mission() != MISSION_UNLOAD && CurrentMission != MISSION_ENTER && MissionQueue != MISSION_ENTER && !IsDumping) { // RA2: as above
			RadioClass * radio = Radio_Link(0); // RA2: as above
			if (radio != NULL && radio->What_Am_I() == RTTI_BUILDING && static_cast<BuildingClass *>(radio)->Class->IsWeeder) {
				Transmit_Message(RADIO_OVER_OUT);
			}
		}

		/*
		**	Certain units require some setup time after they come to a halt.
		*/
		if (NavCom == NULL && Path[0] == FACING_NONE && !Class->MobileFire) { // RA2: a unit that cannot fire on the move rearms
			Arm = Rearm_Delay(1) / 4; // RA2: as above
		}

		CellClass * cellptr = &Map[Get_Coord()]; // RA2: as above
		LandType land = cellptr->Land_Type();
		if (!Locomotion->Is_Moving() && Can_Enter_Cell(cellptr) == MOVE_NO && (!IsOnBridge || !cellptr->IsUnderBridge) && !IsSinking) {
			if (CurrentMission != MISSION_ENTER) { // RA2: a unit still entering something is only scattered
				new AnimClass(Combat_Anim(Strength, Rule->C4Warhead, land, Get_Coord()), Get_Coord(), 0, 1, ShapeFlags_Type(SHAPE_CENTER|SHAPE_WIN_REL|SHAPE_ZGRAD), Get_Explosion_Z(Get_Coord())); // RA2: as above
				int damage = Strength; // RA2: as above
				Combat_Lighting(Center_Coord(), damage, Rule->C4Warhead, 0, 0); // RA2: the five argument form
				Take_Damage(damage, 0, Rule->C4Warhead, NULL, true); // RA2: as above
				return; // RA2: as above
			}
			Scatter(COORD_NONE, true, true); // RA2: as above
		}
	}

	/*
	**	Destroy any crushable wall that is driven over by a tracked vehicle.
	*/
	CellClass * cellptr = &Map[cell];
	if ((Class->IsCrusher || Has_Ability(ABILITY_CRUSHER)) && cellptr->Overlay != OVERLAY_NONE) {
		OverlayTypeClass const * optr = OverlayTypes[cellptr->Overlay];

		if (optr->IsCrushable) {
			Sound_Effect(optr->CrushSound, Center_Coord());
			cellptr->Reduce_Wall(-1);
			IsCrushing = false;
			RockingForwardsPerFrame += 0.02f;
		}
	}

	/*
	**	Check to see if crushing of any unfortunate infantry is warranted.
	*/
	Overrun_Square(Get_Cell(), false); // RA2: as above

	if (IsActive) { // RA2: no bench timer
		BASECLASS::Per_Cell_Process(why); // RA2: as above
	}
}
#endif



/// <summary>
/// Determines if this unit should be rendered, and marks it for redraw if so.
/// A vehicle still being disgorged by a building is held back until the building's door
/// says it may be seen, so that it does not appear before it has been let out.
/// </summary>
/// <param name="rect">The rectangle to record the unit's dirty area within.</param>
/// <param name="forced">Is this redraw forced by outside circumstances?</param>
/// <returns>bool; Was the unit rendered?</returns>
bool UnitClass::Render(Rect & rect, bool forced, bool extras_only) const
{
	if (IsTethered) {
		TechnoClass *contact = Radio_Link(0); // RA2: RA2's form
		if (contact->Fetch_RTTI() == RTTI_BUILDING && (contact->Get_Mission() == MISSION_UNLOAD || contact->MissionQueue == 16)) { // RA2: as above
			DelayTimerClass *state = &contact->Door; // RA2: as above
			if (state->Is_Count_Up_Active() || state->Is_Count_Down_Active() || state->Is_Count_Up_Complete() || state->Is_Count_Down_Complete()) { // RA2: as above
				return(false); // RA2: as above
			}
		}
	}
	return ObjectClass::Render(rect, forced, false); // RA2: as above
}



/// <summary>
/// RA2
/// </summary>
static inline bool Is_Cached(VoxelIndexClass * cache, int key)
{
	if (key != -1 && cache != NULL) {
		return((*cache)[key] != NULL);
	}
	return(false);
}


/// <summary>
/// RA2
/// </summary>
inline void Coord_Pixel(Coord coord, Point2D & pixel)
{
	TacticalMap->Coord_To_Pixel(coord, pixel);
}


/// <summary>
/// Transfers the composed unit image to the destination surface.
/// This routine is the last step of drawing a turreted vehicle. The image built up on the
/// scratch surface is blitted out with whatever cloaking or translucency effect the unit's
/// visual character calls for. A vehicle too tall to fit under a bridge goes across in two
/// pieces, so that its upper half sorts above the bridge deck.
/// </summary>
/// <param name="surface">The surface to blit the composed unit onto.</param>
/// <param name="drawpoint">The pixel position to center the unit upon.</param>
/// <param name="cliprect">The clipping rectangle to draw within.</param>
/// <param name="brightness">The lighting level to blit the unit at.</param>
void UnitClass::Unit_Blit_Voxel(Surface & surface, Point2D drawpoint, Rect cliprect, int brightness) const
{
	bool split = false; // RA2: a vehicle too big to fit under a bridge is drawn in two pieces at a bridge or a weapons factory
	if (Class->IsTooBigToFitUnderBridge) { // RA2: as above
		if (Is_Z_Fudge_Bridge() && Get_Z_Fudge_Column() == 0) { // RA2: as above
			split = true; // RA2: as above
		} else {
			TechnoClass * contact = Radio_Link(0); // RA2: as above
			if (NavCom != NULL && contact != NULL && contact->Fetch_RTTI() == RTTI_BUILDING && ((BuildingClass *)contact)->Class->IsWeaponsFactory) { // RA2: as above
				split = true; // RA2: as above
			}
		}
	}

	int draw_adjust = 0; // RA2: the shape flags follow the cloak state
	unsigned flags = 0x2800; // RA2: as above
	switch (Visual_Character(false, 0)) { // RA2: as above
		case VISUAL_INDISTINCT: // RA2: as above
			flags = 0x2802; // RA2: as above
			break; // RA2: as above

		case VISUAL_DARKEN: // RA2: as above
		case VISUAL_SHADOWY: // RA2: as above
			flags = 0x2804; // RA2: as above
			break; // RA2: as above

		case VISUAL_RIPPLE: // RA2: as above
			flags = 0x280A; // RA2: as above
			if (CloakingDevice.Fetch_Stage()) { // RA2: as above
				flags = 0x280C; // RA2: as above
			}
			draw_adjust = Get_Predator_Offset(); // RA2: as above
			break; // RA2: as above
	}
	if (IsBeingWarpedOut() || IsWarpingIn()) { // RA2: as above
		flags |= 4; // RA2: as above
	}
	if (House->Is_Player_Control() && IsDisguised()) { // RA2: as above
		flags = GetDisguiseFlags(flags); // RA2: as above
	}
	Blitter const * blitter = ColorSchemes[House->Scheme]->Converter->Blitter_From_Flags((ShapeFlags_Type)flags); // RA2: as above

	Rect destination(UnitCompositeDirtyRect.X + drawpoint.X - 128, UnitCompositeDirtyRect.Y + drawpoint.Y - 128, UnitCompositeDirtyRect.Width, UnitCompositeDirtyRect.Height); // RA2: the composite is blitted to the surface around the draw point
	if (split && UnitCompositeDirtyRect.Height > 16) { // RA2: the lower 16 rows get a separate z gradient
		Rect source = UnitCompositeDirtyRect; // RA2: as above
		destination.Height = UnitCompositeDirtyRect.Height - 16; // RA2: as above
		source.Height = destination.Height; // RA2: as above
		Bit_Blit(surface, cliprect, destination, *LogicalSurface, LogicalSurface->Get_Rect(), source, *blitter, Get_Z_Adjust() - 5, ZGRAD_GROUND, brightness, draw_adjust); // RA2: as above
		destination.Y += destination.Height; // RA2: as above
		source.Y += destination.Height; // RA2: as above
		destination.Height = 16; // RA2: as above
		source.Height = 16; // RA2: as above
		Bit_Blit(surface, cliprect, destination, *LogicalSurface, LogicalSurface->Get_Rect(), source, *blitter, Get_Z_Adjust(), ZGRAD_90DEG, brightness, draw_adjust); // RA2: as above
		return; // RA2: as above
	}
	Bit_Blit(surface, cliprect, destination, *LogicalSurface, LogicalSurface->Get_Rect(), UnitCompositeDirtyRect, *blitter, Get_Z_Adjust(), Get_Z_Gradient(), brightness, draw_adjust); // RA2: as above
}



/// <summary>
/// Draws this unit from its shape artwork.
/// This routine handles the unit types that are drawn from shapes rather than voxels --
/// visceroids, jellyfish, limpet drones and the sprite based vehicles. A turreted vehicle
/// is composed on the scratch surface first so that its turret and voxel barrel can be
/// layered in the right order, then blitted to the destination in one piece.
/// </summary>
/// <param name="xdrawpoint">The pixel position to center the unit's artwork upon.</param>
/// <param name="xcliprect">The clipping rectangle to draw within.</param>
/// <param name="brightness">The lighting level to draw the unit at.</param>
#ifdef RA2_NON_MATCHING // not exact: 2247 bytes differ (adopted text)
void UnitClass::Unit_Draw_Shape(Point2D xdrawpoint, Rect xcliprect, int brightness) const
{
	VoxelDataStruct * voxl;
	int shapenum;                // Working shape number.
	ShapeSet const * shapefile;  // Working shape file pointer.

	/*
	**	Verify the legality of the unit class.
	*/
	shapefile = (ShapeSet const *)Get_Image_Data();
	bool barrel_above_turret = true;

	if (Class->IsSmallVisceroid || Class->IsLargeVisceroid) {
		if (VisceroidsAsSnoBees) {
			if (Class->IsSmallVisceroid) {
				shapefile = (ShapeSet const *)UnitTypeClass::SmallVisceroidShapes;
			} else {
				shapefile = (ShapeSet const *)UnitTypeClass::LargeVisceroidShapes;
			}
			shapenum = PrimaryFacing.Current().As_Dir8();
			switch (shapenum) {
				case FACING_N:
				case FACING_NE:
					shapenum = 2;
					break;
				case FACING_SE:
					shapenum = 4;
					break;
				case FACING_S:
				case FACING_SW:
					shapenum = 6;
					break;
				case FACING_NW:
					shapenum = 0;
					break;
				default:
					break;
			}
			shapenum += (((unsigned int)(Frame + (Fetch_ID() >> 1)) >> 3) & 1);
		} else {
			shapenum = Fetch_Stage();
			if (shapenum >= 90) {
				shapenum -= 90;
				shapefile = (ShapeSet const *)Class->AltImageData;
			}
		}
		Draw_Object(shapefile, shapenum, xdrawpoint, xcliprect, DIR_N, 256, 0, ZGRAD_90DEG, false, brightness);
		return;
	}


	if (Class->Facings == FACING_COUNT) { // RA2: no IsLimpetDrone, IsJellyfish (not in RA2's layout)
		shapenum = Facing_Add(PrimaryFacing.Current().Round_To_8(), FACING_45);
	} else {
		shapenum = 0;
	}

	if (Locomotion->Is_Moving()) {
		shapenum = Class->StartWalkFrame + shapenum * Class->WalkFrames + TotalFramesWalked % Class->WalkFrames;
	} else {
		if (FiringSyncDelay >= 0) {
			shapenum = Class->StartFiringFrame + FiringSyncDelay / 2 + shapenum * Class->FiringFrames;
		} else {
			if (DeathCounter >= 0) {
				int deathframe = DeathCounter / Class->DeathFrameRate;
				int StartDeathFrame = Class->StartDeathFrame;
				int lastframe = Class->DeathFrames - 1;
				if (deathframe >= lastframe) {
					deathframe = lastframe;
				}
				shapenum = deathframe + StartDeathFrame;
			} else if (IsOccupyingCell) {
				if (Class->StandingFrames == 0) {
					shapenum = Class->StartWalkFrame + shapenum * Class->WalkFrames;
				} else {
					shapenum = Class->StartStandFrame + shapenum * Class->StandingFrames;
				}
			}
		}
	}

	/*
	**	If there is a turret, then it must be rendered as well. This may include
	**	firing animation if required.
	*/
	if (Class->IsTurretEquipped) {

		Point2D drawpoint = xdrawpoint;
		if (IsInTransport) {
			drawpoint.Y -= 14;
		}

		/*
		**	Actually perform the draw. Overlay an optional shimmer effect as necessary.
		*/
		Draw_Shape(*LogicalSurface, *NormalDrawer, shapefile, shapenum + shapefile->Get_Count() / 2, drawpoint, xcliprect, ShapeFlags_Type(SHAPE_DARKEN | SHAPE_CENTER | SHAPE_WIN_REL | SHAPE_ALPHA | SHAPE_ZGRAD), NULL, Get_Z_Adjust() - 2);

		Surface * old_surface = LogicalSurface;
		LogicalSurface = EightBitSurface;
		Point2D pt = Point2D(80,80);

		Matrix3D nmtx;
		Rect srect = LogicalSurface->Get_Rect();
		IsCompositingToEightBitSurface = true;
		UnitCompositeDirtyRect = RECT_NONE;

		voxl = &Class->AuxVoxel2;
		if (voxl->VoxLib != NULL && voxl->MotLib != NULL) {
			int key = -1;
			Matrix3D mtx = Locomotion->Draw_Matrix(&key);
			mtx.Translate_X(Class->TurretOffset / 8);
			double sec = SecondaryFacing.Current().As_Radian32();
			double pri = PrimaryFacing.Current().As_Radian32();
			mtx.Rotate_Z(sec - pri);

			/*
			 * A recoiling turret moves "backward".
			 */

			nmtx = mtx; // RA2: no IsInRecoilState (TibSun's use does not compile against RA2's declaration)

			Vector3 trans = mtx.Get_Translation();
			nmtx.Translate(-trans);

			Vector3 flh = Vector3(Get_Class_Weapon_Data()->FireFLH.X / -8, 0, Get_Class_Weapon_Data()->FireFLH.Z / -8);
			nmtx.Translate(-flh);

			FacingClass face = BarrelPitch;
			if (PrimaryWeapon->Bullet->IsInvisible) {
				Dir256 dir256 = face.Current().As_Dir256();
				if (dir256 > DIR_E && dir256 <= DIR_S) {
					dir256 = (Dir256)((dir256 - DIR_E) / 3 + DIR_E);
				} else {
					dir256 = (Dir256)((DIR_E - dir256) / 3 + DIR_E);
				}
				face.Set(dir256);
			}

			DirType facing = face.Current();
			nmtx.Rotate_Y(-facing.As_Radian32());
			nmtx.Translate(flh);
			nmtx.Translate(trans);
			if (SecondaryFacing.Current().As_Dir4() > 0) {
				DirType dir = SecondaryFacing.Current();
				barrel_above_turret = true;
				if (dir.As_Dir4() >= 3) {
					barrel_above_turret = false;
				}
			} else {
				barrel_above_turret = false;
			}
		}

		/// Draw the main shape
		Draw_Object(shapefile, shapenum, pt, srect, DIR_N, 256, 0, ZGRAD_GROUND, 0, brightness, NULL, 0, Point2D(0, 0), ShapeFlags_Type(SHAPE_ALPHA|SHAPE_ZGRAD));

		/*
		 * The the voxel barrel below the turret at certain angles
		 */
		voxl = &Class->AuxVoxel2;
		if (voxl->VoxLib != NULL && voxl->MotLib != NULL && !barrel_above_turret) {
			Draw_Voxel(Class->AuxVoxel2, 0, -1, 0, srect, pt, Get_Isometric_View_Matrix() * nmtx, brightness, ShapeFlags_Type(SHAPE_ZGRAD|SHAPE_ALPHA));
		}

		Dir32 d = SecondaryFacing.Current().As_Dir32();
		Draw_Object(shapefile, ((d + 4) % 32U) + 8 * Class->WalkFrames, pt, srect, DIR_N, 256, 0, ZGRAD_GROUND, false, brightness, NULL, 0, Point2D(0, 0), ShapeFlags_Type(SHAPE_NOTRANS|SHAPE_ALPHA|SHAPE_ZGRAD));

		/*
		 * The the voxel barrel above the turret at other angles
		 */
		voxl = &Class->AuxVoxel2;
		if (voxl->VoxLib != NULL && voxl->MotLib != NULL && barrel_above_turret) {
			Draw_Voxel(Class->AuxVoxel2, 0, -1, 0, srect, pt, Get_Isometric_View_Matrix() * nmtx, brightness, ShapeFlags_Type(SHAPE_NOTRANS|SHAPE_ZGRAD|SHAPE_ALPHA));
		}

		int apparent_brightness = Apparent_Brightness(brightness);
		Unit_Blit_Voxel(*old_surface, xdrawpoint, xcliprect, apparent_brightness);
		LogicalSurface->Fill_Rect(UnitCompositeDirtyRect, 0);
		LogicalSurface = old_surface;
		IsCompositingToEightBitSurface = false;

	} else {
		static int _zadj = 0;
		Draw_Object(shapefile, shapenum, xdrawpoint, xcliprect, DIR_N, 256, _zadj, Get_Z_Gradient(), 0, brightness, NULL, 0, Point2D(0, 0), SHAPE_NORMAL);
	}
}
#endif


/***********************************************************************************************
 * UnitClass::Draw_It -- Draws a unit object.                                                  *
 *                                                                                             *
 *    This routine is the one that actually draws a unit object. It displays the unit          *
 *    according to its current state flags and centered at the location specified.             *
 *                                                                                             *
 * INPUT:   x,y   -- The X and Y coordinate of where to draw the unit.                         *
 *                                                                                             *
 *          window   -- The clipping window to use.                                            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/20/1994 JLB : Created.                                                                 *
 *   06/27/1994 JLB : Takes a window parameter.                                                *
 *   08/15/1994 JLB : Removed infantry support.                                                *
 *   01/07/1995 JLB : Harvester animation support.                                             *
 *   07/08/1995 JLB : Uses general purpose draw routine.                                       *
 *=============================================================================================*/
void UnitClass::Draw_It(Point2D const & point, Rect const & cliprect) const
{
	static ShapeSet const * harvesting_shape = (ShapeSet const *)MFCD::Retrieve("OREGATH.SHP"); // RA2: the harvesting overlay is OREGATH.SHP

	Point2D adjusted_point = point;
	adjusted_point.Y -= IonBlastYDrawOffset;
	Point2D raw_point = adjusted_point;
	if (IsInTransport) {
		adjusted_point.Y += 14;
	}

	Cell cell = PositionCell;

	/*
	**	If drawing of this unit is not explicitly prohibited, then proceed
	**	with the render process.
	*/
	if (Visual_Character(false, NULL) != VISUAL_HIDDEN) {

		TacticalMap->Add_To_Selectables((ObjectClass *)this, adjusted_point);
		int height = HeightAGL;
		CellClass * cptr = &Map[(Coord const &)PositionCoord]; // RA2: as above

		int brightness; // RA2: as above
		if (((!IsOnBridge && height >= BRIDGE_LEPTON_HEIGHT) || (IsOnBridge && height >= 0)) && // RA2: as above
			(cptr->IsUnderBridge && // RA2: as above
				(cptr->IsBridgeEastWest && cptr->Adjacent_Cell(FACING_N).IsUnderBridge || // RA2: as above
				!cptr->IsBridgeEastWest && cptr->Adjacent_Cell(FACING_W).IsUnderBridge))) { // RA2: as above
			int level; // RA2: the ion storm and the nuke flash have their own level lighting
			if (IonStormClass::Is_Ion_Storm_Active()) { // RA2: as above
				level = Scen->IonLevelLight; // RA2: as above
			} else if (NukeFlash::IsFadingIn()) { // RA2: as above
				level = Scen->NukeLevelLight; // RA2: as above
			} else {
				level = Scen->LevelLight; // RA2: as above
			}
			brightness = Map[cell].Brightness + 4 * level; // RA2: as above
		} else {
			int shade = 0; // RA2: the shade is applied only to a unit on the ground
			if (Map[cell].IsOvershadowed && !In_Air()) { // RA2: as above
				shade = -500; // RA2: as above
			}
			brightness = Map[cell].Brightness + shade; // RA2: as above
		}
		brightness += Rule->ExtraUnitLight;

		if (Class->IsToHarvest && IsHarvesting && !Locomotion->Is_Moving_Now()) {
			Point2D pixel; // RA2: the pixel of the overlay
			FacingType face = PrimaryFacing.Current().As_Dir8(); // RA2: the facing as one of eight, used for the offset and the frame row
			Draw_It_Coord_Pixel(Move_Coord(Render_Coord(), DirType(face), 30), pixel); // RA2: the overlay sits 30 leptons ahead; the pixel conversion is a helper
			int offset = PrimaryFacing.Current().As_Dir8(); // RA2: the frame row is mirrored
			offset = 7 - offset; // RA2: as above
			if (TemporalTargetingMe == NULL && !IsBeingWarpedOut() && !IsWarpingIn()) { // RA2: not while being warped
				Draw_Shape(*LogicalSurface, *AnimDrawer, harvesting_shape, (TotalFramesWalked + Frame) % 15 + offset * 15, pixel, cliprect, ShapeFlags_Type(SHAPE_CENTER|SHAPE_ALPHA|SHAPE_ZGRAD), NULL, Get_Z_Adjust() - 2, ZGRAD_GROUND, brightness); // RA2: as above
			}
		}

		UnitTypeClass * oldclass = Class;
		if (Class->IsToHarvest && IsDumping && oldclass->UnloadingClass != NULL) { // RA2: the type's own unloading class
			((UnitClass *)this)->Class = oldclass->UnloadingClass; // RA2: as above
		}

		/*
		**	Actually perform the draw. Overlay an optional shimmer effect as necessary.
		*/
		ObjectTypeClass * type = Class; // RA2: a unit the local player cannot see clearly draws as its disguise
		if (!IsClearlyVisibleTo(PlayerPtr)) { // RA2: as above
			type = GetDisguise(true); // RA2: as above
		}
		if (CurrentTube == TUBE_NONE) {
			if (type->IsVoxel) { // RA2: as above
				if (type->Voxel.VoxLib != NULL) { // RA2: as above
					Unit_Draw_Voxel(adjusted_point, cliprect, brightness);
				}
			} else {
				Unit_Draw_Shape(adjusted_point, cliprect, brightness);
			}
		}

		((UnitClass *)this)->Class = oldclass;

		/*
		**	If this unit is carrying the flag, then draw that on top of everything else.
		*/
		if (Flagged != HOUSE_NONE) {
			Draw_Shape(*LogicalSurface, *ColorSchemes[Houses[Flagged]->Scheme]->Converter, (ShapeSet const *)MFCD::Retrieve("FLAGFLY.SHP"), Frame % 14, raw_point, cliprect, ShapeFlags_Type(SHAPE_CENTER|SHAPE_ALPHA), NULL, 0, ZGRAD_GROUND, brightness);
		}

		BASECLASS::Draw_It(raw_point, cliprect);

		if (TargetingLaserTimer != 0) {
			Draw_Target_Laser();
		}
	}
}




/***********************************************************************************************
 * UnitClass::Harvesting -- Harvests tiberium at the current location.                         *
 *                                                                                             *
 *    This routine is used to by the harvester to harvest Tiberium at the current location.    *
 *    When harvesting is complete, this routine will return true.                              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Is harvesting complete?                                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool UnitClass::Harvesting(void)
{
	Coord position = Get_Coord(); // RA2: RA2's form
	CellClass *cell = &Map[position]; // RA2: as above
	if (NavCom) { // RA2: as above
		return(true); // RA2: as above
	}
	if (Class->IsToHarvest && Tiberium_Load() < 1.0 && cell->Land_Type() == LAND_TIBERIUM) { // RA2: as above
		if (Class->IsToVeinHarvest) {
			cell->Reduce_Weed(); // RA2: as above
			Storage.Increase_Amount(1.0f, 0); // RA2: as above

			/*
			**	If the harvester is stopped on a non Tiberium field and the harvester // RA2: as above
			**	isn't loaded with Tiberium, then no further action can be performed // RA2: as above
			**	by this logic routine. Bail with a failure and thus cause a branch to // RA2: as above
			**	a better suited logic processor. // RA2: as above
			*/
			Set_Stage(0);
			Set_Rate(Rule->HarvesterLoadRate * 3); // RA2: as above
		} else {
			int resource = cell->Tiberium_Type_Here(); // RA2: as above
			float capacity = (float)Class->Capacity; // RA2: as above
			int available = (int)(capacity - Storage.Get_Total_Amount() > 1.0f // RA2: as above
				? 1.0f : (float)Class->Capacity - Storage.Get_Total_Amount()); // RA2: as above
			int taken = cell->Reduce_Tiberium(available); // RA2: as above
			if (taken <= 0) { // RA2: as above
				return(false);
			}
			Storage.Increase_Amount((float)taken, resource); // RA2: as above
			Set_Stage(0);
			Set_Rate(Rule->HarvesterLoadRate);
		}
		return(true); // RA2: as above
	}
	Set_Stage(0); // RA2: as above
	Set_Rate(0); // RA2: as above
	return(false); // RA2: as above
}





/***********************************************************************************************
 * UnitClass::Mission_Unload -- Handles unloading cargo.                                       *
 *                                                                                             *
 *    This is the AI control sequence for when a transport desires to unload its cargo and     *
 *    then exit the map.                                                                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the delay before calling this routine again.                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: 3346 bytes differ (adopted text)
int UnitClass::Do_MISSION_UNLOAD(void)
{
	enum {
		INITIAL_CHECK,
		MANEUVERING,
		OPENING_DOOR,
		UNLOADING,
		CLOSING_DOOR
	};
	Cell cell; // RA2: as above

	if (Class->Max_Passengers() > 0) {
		/// derives from RA's UNIT_TRUCK
		switch (Status) {
			case INITIAL_CHECK: { // RA2: as above
				if (Locomotion->Is_Moving()) {
					return(10);
				}
				Cell current = Get_Cell(); // RA2: a transport standing in water first moves to the nearest land
				CellClass & current_cell = Map[current]; // RA2: as above
				if (NavCom == NULL && current_cell.Land_Type() == LAND_WATER) { // RA2: as above
					Cell destination = Map.Nearby_Location(Get_Cell(), SPEED_WHEEL, -1, MZONE_NORMAL, false, Point2D(1, 1), false, false, false, true, Cell(0, 0), false); // RA2: as above
					Assign_Destination(&Map[destination], true); // RA2: as above
					return(10); // RA2: as above
				}
				int passengers = Cargo.How_Many(); // RA2: the passenger count is read before the facing
				int facing = Desired_Load_Dir(NULL, cell); // RA2: as above
				if (passengers && cell != CELL_NONE) { // RA2: as above
					if (Has_Turret_Weapons()) { // RA2: a turreted transport keeps its last passenger aboard
						if (passengers == 1) { // RA2: as above
							NonPassengerCount = 0; // RA2: as above
						} else {
							NonPassengerCount = 1; // RA2: as above
						}
					}
					DirType direction((unsigned short)(facing << 13)); // RA2: the load direction in 16 bit steps
					Locomotion->Do_Turn((DirStruct &)direction); // RA2: as above
					Status = MANEUVERING;
					return(1);
				}
				QueueMission(MISSION_GUARD, false); // RA2: queued, not assigned
				break;
			}

			case MANEUVERING:
				if (!IsRotating) {
					Status = UNLOADING;
					return(1);
				}
				break;

			case UNLOADING:
				if (Cargo.How_Many() > NonPassengerCount) { // RA2: the last passenger of a turreted transport stays
					FootClass * passenger = Unload_Passenger(); // RA2: the first passenger leaves (FootClass 0x4CCFC0)
					if (passenger != NULL) {
						DirType opposite((unsigned short)(PrimaryFacing.Current().Raw + 0x7fff)); // RA2: the passenger leaves backwards, the eight directions are tried from that one
						int attempt = 0; // RA2: as above
						int start = (((unsigned)opposite.Raw >> 12) + 1) >> 1; // RA2: as above
						bool prefer_far = true; // RA2: first a cell whose far neighbor is free as well
						bool placed = false;
						for (; attempt < 8; ++attempt) { // RA2: as above
							int facing = (start + attempt) & 7; // RA2: as above
							Cell near_cell = Adjacent_Cell(Get_Cell(), (FacingType)facing); // RA2: as above
							Cell far_cell = Adjacent_Cell(near_cell, (FacingType)facing); // RA2: as above
							bool far_allowed = passenger->Can_Enter_Cell(&Map[far_cell], (FacingType)facing, Get_Cell_Height(), NULL, true) == MOVE_OK; // RA2: as above
							bool near_allowed = passenger->Can_Enter_Cell(&Map[near_cell], (FacingType)facing, Get_Cell_Height(), NULL, true) == MOVE_OK; // RA2: as above
							if (!near_allowed || (!far_allowed && prefer_far)) { // RA2: as above
								if (prefer_far && attempt + 1 == 8) { // RA2: the second round accepts a free near cell only
									attempt = 0; // RA2: as above
									prefer_far = false; // RA2: as above
								}
								continue; // RA2: as above
							}
							if (Map[near_cell].IsUnderBridge) { // RA2: as above
								continue; // RA2: as above
							}
							ScenarioInit++; // RA2: as above
							Coord coord = near_cell.As_Coord(); // RA2: as above
							coord = Map.Closest_Free_Spot(coord, false); // RA2: as above
							DirType direction((unsigned short)(facing << 13)); // RA2: as above
							placed = passenger->Unlimbo(coord, direction.As_Dir256()); // RA2: as above
							ScenarioInit--; // RA2: as above
							if (placed) { // RA2: as above
								passenger->QueueMission(MISSION_MOVE, false); // RA2: queued, not assigned
								if (prefer_far) { // RA2: as above
									passenger->Assign_Destination(&Map[far_cell], true); // RA2: as above
								} else {
									passenger->Assign_Destination(&Map[near_cell], true); // RA2: as above
								}
								if (Team != NULL) { // RA2: as above
									Team->Add(passenger); // RA2: as above
								}
							}
							break; // RA2: as above
						}
						if (!placed) { // RA2: a passenger that finds no place goes back aboard
							Cargo.Attach(passenger); // RA2: as above
							passenger->Hidden(); // RA2: as above
						}
					}
				} else {
					Status = CLOSING_DOOR;
				}
				break;

			case CLOSING_DOOR:
				QueueMission(MISSION_GUARD, false); // RA2: queued, not assigned
				IsMissionUnloadStandby = true;
				break;
		}
		return(Current_Mission_Control().Normal_Delay() + Random_Pick(0, 2));
	}

	if (!Class->IsToHarvest && !Class->IsToVeinHarvest) { // RA2: the deploying types come before the harvesters
		if (Class->DeploysInto != NULL) { // RA2: as above
			/// derives from RA's UNIT_MCV
			switch (Status) { // RA2: as above
				case 2: // RA2: as above
					if (!IsDeploying) { // RA2: as above
						Enter_Idle_Mode(false, true); // RA2: as above
						Commence(); // RA2: as above
					} else if (!Try_To_Deploy() && NavCom != NULL) { // RA2: as above
						IsDeploying = false; // RA2: as above
					}
					break; // RA2: as above

				case 0: // RA2: as above
					Transmit_Message(RADIO_OVER_OUT); // RA2: as above
					Path[0] = FACING_NONE; // RA2: as above
					Status = 1; // RA2: as above
					// fall through
				case 1: // RA2: as above
					if (!Locomotion->Is_Moving()) { // RA2: as above
						Try_To_Deploy(); // RA2: as above
						if (IsActive) { // RA2: as above
							if (IsDeploying) { // RA2: as above
								Status = 2; // RA2: as above
							} else if (!House->Is_Human_Player() && Session.Type != GAME_NORMAL) { // RA2: as above
								QueueMission(MISSION_HUNT, false); // RA2: queued, not assigned
							} else {
								QueueMission(MISSION_GUARD, false); // RA2: as above
							}
						}
					} else if (MissionQueue != MISSION_NONE && MissionQueue != MISSION_UNLOAD) { // RA2: as above
						Commence(); // RA2: as above
					}
					break; // RA2: as above

				default: // RA2: as above
					break; // RA2: as above
			}
		} else {
			if (Class->IsSimpleDeployer) { // RA2: a simple deployer only guards and starts at once
				QueueMission(MISSION_GUARD, false); // RA2: as above
				Commence(); // RA2: as above
			}
			return(FootClass::Do_MISSION_UNLOAD()); // RA2: everything else is the base mission
		}
		return(Current_Mission_Control().Normal_Delay() + Random_Pick(0, 2)); // RA2: as above
	}

	/// derives from RA's UNIT_HARVESTER
	if (!HasAnyLink()) { // RA2: as above
		Enter_Idle_Mode(false, true); // RA2: as above
		IsDumping = false; // RA2: as above
		if (Locomotion->Is_Moving()) { // RA2: as above
			Stop_Driver(); // RA2: as above
		}
		if (Ready_To_Commence()) { // RA2: as above
			Commence(); // RA2: as above
		}
		return(1); // RA2: as above
	}

	if (PrimaryFacing.Current().As_Dir256() != DIR_E) { // RA2: as above
		if (!IsRotating) { // RA2: as above
			Locomotion->Do_Turn((DirStruct &)DirType(DIR_E)); // RA2: as above
		}
		return(5); // RA2: as above
	}

	if (!IsDumping) { // RA2: as above
		IsDumping = true; // RA2: as above
		Set_Stage(0); // RA2: as above
		Set_Rate(1); // RA2: as above
		if (Class->IsToHarvest) { // RA2: as above
			BuildingClass * building = Map[Get_Cell() + AdjacentCell[6]].Cell_Building(); // RA2: the refinery is west of the unit
			if (building != NULL) { // RA2: as above
				building->Begin_Anim(BANIM_PRE_PRODUCTION, building->HealthRatio <= Rule->ConditionYellow, false, 0); // RA2: as above
			}
		}
		Status = 3; // RA2: as above
	} else {
		switch (Status) { // RA2: as above
			case 4: { // RA2: as above
				if (Class->IsToVeinHarvest) { // RA2: as above
					IsDumping = false; // RA2: as above
					if (NavCom != NULL && MissionQueue != MISSION_NONE && MissionQueue != MISSION_HARVEST) { // RA2: as above
						if (Locomotion->Is_Moving()) { // RA2: as above
							Stop_Driver(); // RA2: as above
						}
						if (Ready_To_Commence()) { // RA2: as above
							Commence(); // RA2: as above
						}
					} else {
						QueueMission(MISSION_HARVEST, false); // RA2: queued, not assigned
						if (Ready_To_Commence()) { // RA2: as above
							if (HasAnyLink()) { // RA2: as above
								Transmit_Message(RADIO_OVER_OUT); // RA2: as above
							}
							Commence(); // RA2: as above
						}
					}
				} else {
					BuildingClass * building = Map[Get_Cell() + AdjacentCell[6]].Cell_Building(); // RA2: as above
					bool active = false; // RA2: as above
					if (building != NULL && building->Class->IsRefinery) { // RA2: as above
						active = building->Anim_Active(BANIM_PRODUCTION); // RA2: as above
					}
					if (active) { // RA2: as above
						return(1); // RA2: as above
					}
					IsDumping = false; // RA2: as above
					if (NavCom != NULL && MissionQueue != MISSION_NONE && MissionQueue != MISSION_HARVEST) { // RA2: as above
						if (Locomotion->Is_Moving()) { // RA2: as above
							Stop_Driver(); // RA2: as above
						}
						if (Ready_To_Commence()) { // RA2: as above
							Commence(); // RA2: as above
						}
					} else {
						QueueMission(MISSION_HARVEST, false); // RA2: as above
						if (Ready_To_Commence()) { // RA2: as above
							if (HasAnyLink()) { // RA2: as above
								Transmit_Message(RADIO_OVER_OUT); // RA2: as above
							}
							Commence(); // RA2: as above
						}
					}
				}
				break; // RA2: as above
			}

			case 3: { // RA2: as above
				BuildingClass * building = Map[Get_Cell() + AdjacentCell[6]].Cell_Building(); // RA2: as above
				if (building == NULL) { // RA2: the refinery is gone
					if (HasAnyLink()) { // RA2: as above
						Transmit_Message(RADIO_OVER_OUT); // RA2: as above
					}
					QueueMission(MISSION_HARVEST, true); // RA2: as above
					return(Current_Mission_Control().Normal_Delay() + Random_Pick(0, 2)); // RA2: as above
				} else {
					if (Fetch_Stage() >= Rule->HarvesterDumpRate * TICKS_PER_MINUTE) {
						DynamicVectorClass<ParticleSystemTypeClass const *> smoke; // RA2: the refinery's smoke systems are picked from its damage particle systems
						for (int n = building->Techno_Type_Class()->DamageParticleSystems.Count() - 1; n >= 0; --n) { // RA2: as above
							if (building->Techno_Type_Class()->DamageParticleSystems[n]->BehavesLike == PSYS_BEHAVIOR_SMOKE) { // RA2: as above
								smoke.Add(building->Techno_Type_Class()->DamageParticleSystems[n]); // RA2: as above
							}
						}
						if (building->ParticleSystems[5] == NULL && smoke.Count() > 0 && HeightAGL > -10) { // RA2: as above
							TechnoTypeClass const * type = building->Techno_Type_Class(); // RA2: as above
							Coord const & o = type->RefinerySmokeOffsetOne; // RA2: as above
							Coord offset = o; // RA2: as above
							building->ParticleSystems[5] = new ParticleSystemClass((ParticleSystemTypeClass *)smoke[Random_Pick(0, smoke.Count() - 1)], building->Get_Coord() + offset, NULL, building, COORD_NONE); // RA2: as above
						}
						if (building->ParticleSystems[6] == NULL && smoke.Count() > 0 && HeightAGL > -10) { // RA2: as above
							TechnoTypeClass const * type = building->Techno_Type_Class(); // RA2: as above
							Coord const & o = type->RefinerySmokeOffsetTwo; // RA2: as above
							Coord offset = o; // RA2: as above
							building->ParticleSystems[6] = new ParticleSystemClass((ParticleSystemTypeClass *)smoke[Random_Pick(0, smoke.Count() - 1)], building->Get_Coord() + offset, NULL, building, COORD_NONE); // RA2: as above
						}
						if (building->Anims[10] == NULL) { // RA2: as above
							building->Begin_Anim(BANIM_ACTIVE_TWO, building->HealthRatio <= Rule->ConditionYellow, false, 0); // RA2: as above
						}
						int slot = Storage.First_Used_Slot();
						HouseClass * owner = building->Owner_HouseClass(); // RA2: the refinery's owner is paid
						int purifiers = owner->NumOrePurifiers; // RA2: as above
						if (owner->Power_Fraction() < 1.0) { // RA2: as above
							purifiers = 0; // RA2: as above
						}
						if (!owner->IsHuman && Session.Type != GAME_NORMAL) { // RA2: as above
							purifiers += Rule->AIVirtualPurifiers[owner->Difficulty]; // RA2: as above
						}
						float amount = slot != -1 ? Storage.Get_Amount(slot) : 0.0f; // RA2: as above
						float bonus = (float)purifiers * Rule->PurifierBonus * amount; // RA2: as above
						float removed; // RA2: as above
						if (slot != -1 && (removed = Storage.Decrease_Amount(amount, slot)) > 0.0f) { // RA2: the whole load is paid at once
							if (Class->IsToVeinHarvest) { // RA2: as above
								House->Harvested_Weed((int)removed, slot); // RA2: as above
								Set_Stage(0); // RA2: as above
							} else {
								owner->Give_Tiberium(removed, slot); // RA2: as above
								if (bonus > 0.0f) { // RA2: as above
									owner->Give_Tiberium(bonus, slot); // RA2: as above
								}
								Set_Stage(0);
							}
						} else {
							if (building->Class->IsRefinery) { // RA2: as above
								building->Begin_Anim(BANIM_PRODUCTION, building->HealthRatio <= Rule->ConditionYellow, false, 0); // RA2: as above
							}
							Status = 4;
							if (building->ParticleSystems[5] != NULL) { // RA2: as above
								building->ParticleSystems[5]->Delete_Me(); // RA2: as above
							}
							if (building->ParticleSystems[6] != NULL) { // RA2: as above
								building->ParticleSystems[6]->Delete_Me(); // RA2: as above
							}
							if (building->Anims[10] != NULL) { // RA2: as above
								building->End_Anim(BANIM_ACTIVE_TWO); // RA2: as above
							}
						}
					}
					if (NavCom != NULL && MissionQueue != MISSION_NONE && MissionQueue != MISSION_HARVEST) { // RA2: as above
						if (building->Class->IsRefinery) { // RA2: as above
							building->Begin_Anim(BANIM_PRODUCTION, building->HealthRatio <= Rule->ConditionYellow, false, 0); // RA2: as above
						}
						Status = 4; // RA2: as above
						if (building->ParticleSystems[5] != NULL) { // RA2: as above
							building->ParticleSystems[5]->Delete_Me(); // RA2: as above
						}
						if (building->ParticleSystems[6] != NULL) { // RA2: as above
							building->ParticleSystems[6]->Delete_Me(); // RA2: as above
						}
						if (building->Anims[10] != NULL) { // RA2: as above
							building->End_Anim(BANIM_ACTIVE_TWO); // RA2: as above
						}
					}
					return(1); // RA2: as above
				}
			}

			default: // RA2: as above
				break;
		}
	}
	return(Current_Mission_Control().Normal_Delay() + Random_Pick(0, 2));
}
#endif



/// <summary>
/// RA2: the distance between two objects' centers, summed z, y, x (Do_MISSION_HARVEST's refinery distance limits)
/// </summary>
static inline Coord Unit_Coord_Delta(Coord const & a, Coord const & b) // RA2: as above
{
	return Coord(a.X - b.X, a.Y - b.Y, a.Z - b.Z); // RA2: as above
}

static inline int Unit_Length_ZYX(Coord const & coord) // RA2: as above
{
	double x = coord.X; // RA2: as above
	double y = coord.Y; // RA2: as above
	double z = coord.Z; // RA2: as above
	return((int)fastmath::sqrt(z * z + y * y + x * x)); // RA2: as above
}

static inline int Unit_Distance_ZYX(ObjectClass const * from, ObjectClass const * to) // RA2: as above
{
	return(Unit_Length_ZYX(Unit_Coord_Delta(from->Center_Coord(), to->Center_Coord()))); // RA2: as above
}


/***********************************************************************************************
 * UnitClass::Mission_Harvest -- Handles the harvesting process used by harvesters.            *
 *                                                                                             *
 *    This is the AI process used by harvesters when they are doing their harvesting action.   *
 *    This entails searching for nearby Tiberium, heading there, harvesting, and then          *
 *    returning to a refinery for unloading.                                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the delay before calling this routine again.                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1994 JLB : Created.                                                                 *
 *   06/21/1995 JLB : Force guard mode if no Tiberium found.                                   *
 *   09/28/1995 JLB : Aborts harvesting if there are no more refineries.                       *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: 6 bytes differ (adopted text)
int UnitClass::Do_MISSION_HARVEST(void)
{
	int i;

	enum {
		LOOKING,
		HARVESTING,
		FINDHOME,
		HEADINGHOME,
		GOINGTOIDLE,
	};

	/*
	**	A non-harvesting type unit will just sit still if it is given the harvest mission. This
	**	allows combat units to act "brain dead".
	*/
	if (!Class->IsToHarvest && !Class->IsToVeinHarvest) return(TICKS_PER_SECOND*30);

	if (Class->Dock.Count() == 0 && !House->Is_Human_Player()) {
		QueueMission(MISSION_GUARD, false); // RA2: queued, not assigned
		return(1);
	}

	/*
	**	If there are no more refineries, then drop into guard mode.
	*/
	bool found = false; // RA2: the search for an owned dock building is written out with a flag
	for (i = 0; i < Class->Dock.Count(); i++) { // RA2: as above
		if (House->BQuantity.Value(Class->Dock[i]->HeapID) > 0) { // RA2: as above
			found = true; // RA2: as above
			break; // RA2: as above
		}
	}
	if (!found) { // RA2: as above
		QueueMission(MISSION_GUARD, false); // RA2: as above
		return(1); // RA2: as above
	}

	bool teleporter = Class->Teleporter; // RA2: a chrono harvester uses its own distance limit and always looks for a queueing spot
	switch (Status) {

		/*
		**	Go and find a Tiberium field to harvest.
		*/
		case LOOKING:
			if (!Class->IsToVeinHarvest && Tiberium_Load() >= 1.0) { // RA2: a weed harvester has no full check here
				Status = FINDHOME; // RA2: as above
				return(1); // RA2: as above
			}

			{
				/*
				**	Look for ore where we last found some - mine the same patch
				*/
				bool hastarget = true;
				if (ArchivedTarget != NULL) { // RA2: as above
					Assign_Destination(ArchivedTarget, true); // RA2: as above
					Assign_Archive_Target(NULL); // RA2: as above
					hastarget = false;
				}
				IsHarvesting = false;
				bool ok; // RA2: as above
				if (Class->IsToVeinHarvest) {
					ok = Goto_Weed(Rule->TiberiumLongScan / CELL_LEPTON_W);
				} else {
					IPersistPtr persist(Locomotion); // RA2: a chrono harvester drops its destination before it looks for ore
					CLSID clsid; // RA2: as above
					persist->GetClassID(&clsid); // RA2: as above
					if (clsid == CLSID_TeleportLocomotion && NavCom != NULL) { // RA2: as above
						Assign_Destination(NULL, true); // RA2: as above
					}
					ok = Goto_Tiberium(Rule->TiberiumLongScan / CELL_LEPTON_W, hastarget);
				}
				if (ok) {
					IsHarvesting = true;
					Set_Rate(2);
					Set_Stage(0);
					Status = HARVESTING;
					return(1);
				}

				/*
				**	If the harvester isn't on Tiberium and it is not heading toward Tiberium, then
				**	force it to go into guard mode. This will prevent the harvester from repeatedly
				**	searching for Tiberium.
				*/
				if (NavCom == NULL) { // RA2: as above

					/*
					**	If the archive target is legal, then head there since it is presumed
					**	that the archive target points to the last place it harvested at.
					*/
					if (ArchivedTarget != NULL) { // RA2: as above
						Assign_Destination(ArchivedTarget, true); // RA2: as above
					} else {
						Status = GOINGTOIDLE; // RA2: as above
						IsUseless = true; // RA2: as above
						if (Class->IsToHarvest) { // RA2: as above
							House->IsTiberiumShort = true; // RA2: as above
						}
						return(TICKS_PER_SECOND*7); // RA2: as above
					}
				} else {
					IsUseless = false; // RA2: as above
				}
			}
			break;

		/*
		**	Harvest at current location until full or Tiberium exhausted.
		*/
		case HARVESTING:
			if (Fetch_Rate() == 0) {
				Set_Stage(0);
				Set_Rate(Rule->HarvesterLoadRate);
			}

			if (Fetch_Stage() < ARRAY_SIZE(Harvester_Load_List)+1) return(1);
			if (!Harvesting()) {
				IsHarvesting = false;
				if (Class->IsToHarvest && Tiberium_Load() == 1.0) { // RA2: only a Tiberium harvester checks for a full load here
					Status = FINDHOME;
					Cell cell;
					if (Class->IsToHarvest) {
						cell = Search_For_Tiberium(Rule->TiberiumShortScan / CELL_LEPTON_W);
					} else {
						cell = Search_For_Weed(Rule->TiberiumShortScan / CELL_LEPTON_W);
					}
					if (cell != CELL_NONE) {
						Assign_Archive_Target(&Map[cell]); // RA2: as above
					} else {
						Assign_Archive_Target(NULL); // RA2: as above
					}
					return(1); // RA2: as above
				}
				bool ok; // RA2: as above
				if (Class->IsToHarvest) { // RA2: as above
					ok = Goto_Tiberium(Rule->TiberiumShortScan / CELL_LEPTON_W, false); // RA2: as above
				} else {
					ok = Goto_Weed(Rule->TiberiumShortScan / CELL_LEPTON_W); // RA2: as above
				}
				if (!ok && NavCom == NULL)	{ // RA2: as above
					Assign_Archive_Target(NULL); // RA2: as above
					Status = FINDHOME; // RA2: as above
				} else {
					Status = HARVESTING; // RA2: as above
					IsHarvesting = true; // RA2: as above
				}
				return(1);
			}
			return(1);

		/*
		**	Find and head to refinery.
		*/
		case FINDHOME:
			if (NavCom != NULL && teleporter && TryNearestDockBuilding((TypeList<BuildingTypeClass *> *)&Class->Dock, 0, 0) != NULL) { // RA2: a chrono harvester that can dock somewhere stops its walk first
				Abort_Motion(); // RA2: as above
			}
			if (NavCom == NULL) {

				/*
				**	Find nearby refinery and head to it?
				*/
				BuildingClass * nearest = TryNearestDockBuilding((TypeList<BuildingTypeClass *> *)&Class->Dock, 0, 0); // RA2: the whole Dock list at once

				/*
				**	Since the refinery said it was ok to load, establish radio
				**	contact with the refinery and then await docking orders.
				*/
				if (((!teleporter && nearest != NULL && Unit_Distance_ZYX(this, nearest) <= Rule->HarvesterTooFarDistance * 256) || (teleporter && nearest != NULL && Unit_Distance_ZYX(this, nearest) <= Rule->ChronoHarvTooFarDistance * 256)) && Transmit_Message(RADIO_HELLO, nearest) == RADIO_ROGER) { // RA2: a refinery within the harvester's own limit is asked first
					Status = HEADINGHOME;
				} else {
					ScenarioInit++;
					nearest = TryNearestDockBuilding((TypeList<BuildingTypeClass *> *)&Class->Dock, 0, 1); // RA2: as above
					ScenarioInit--;
					if (nearest != NULL) {
						if (Distance_To(nearest) > 3 * CELL_LEPTON_W || teleporter) { // RA2: a chrono harvester always goes to the queueing spot
							Cell cell = Cell(nearest->Get_Coord()); // RA2: as above
							Cell queue = cell; // RA2: the queueing spot is the building's cell plus the type's queueing offset
							queue.X = nearest->Class->QueueingCell.X + cell.X; // RA2: as above
							queue.Y += nearest->Class->QueueingCell.Y; // RA2: as above
							queue = Map.Nearby_Location(queue, SPEED_WHEEL, -1, MZONE_NORMAL, false, Point2D(1, 1), false, false, false, true, Cell(0, 0), false); // RA2: as above
							if (!(queue == CELL_NONE)) { // RA2: as above
								Assign_Destination(&Map[queue], true); // RA2: as above
							} else {
								Assign_Destination(NULL, true); // RA2: as above
							}
						}
					}
				}
			}
			break;

		/*
		**	In communication with refinery so that it will successfully dock and
		**	unload. If, for some reason, radio contact was lost, then hunt for
		**	another refinery to unload at.
		*/
		case HEADINGHOME:
			QueueMission(MISSION_ENTER, false); // RA2: queued, not assigned
			return(1);

		/*
		**	The harvester has nothing to do. There is no Tiberium nearby and
		**	no where to go.
		*/
		case GOINGTOIDLE:
			if (IsUseless) {
				if (House->BQuantity.Value(Rule->RepairBay->HeapID) > 0) {
					QueueMission(MISSION_REPAIR, false); // RA2: as above
				} else {
					QueueMission(MISSION_HUNT, false); // RA2: as above
				}
			}
			{
				BuildingClass * bptr = Map[(Coord const &)PositionCoord].Cell_Building(); // RA2: as above
				if (bptr != NULL) { // RA2: as above
					if (bptr->Class->IsRefinery || bptr->Class->IsWeeder) { // RA2: as above
						Assign_Destination(&Map[Nearby_Location(bptr)], true); // RA2: as above
					}
				}
			}
			QueueMission(MISSION_GUARD, false); // RA2: as above
			break;

	}
	return(Current_Mission_Control().Normal_Delay() + Random_Pick(0, 2));
}
#endif



/***********************************************************************************************
 * UnitClass::Mission_Hunt -- This is the AI process for aggressive enemy units.               *
 *                                                                                             *
 *    Computer controlled units must be intelligent enough to find enemies as well as to       *
 *    attack them. This AI process will handle both the simple attack process as well as the   *
 *    scanning for enemy units to attack.                                                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the delay before calling this routine again.                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
int UnitClass::Do_MISSION_HUNT(void)
{
	BuildingTypeClass * building = Class->DeploysInto; // RA2: the deploy type is read once, then looked up in the construction yard list
	if (building != NULL) {
		bool listed = false; // RA2: an inlined search of Rule->BuildConst
		for (int index = 0; index < Rule->BuildConst.Count(); index++) { // RA2: as above
			if (Rule->BuildConst[index] == building) { // RA2: as above
				listed = true; // RA2: as above
				break; // RA2: as above
			}
		}
		if (listed || TarCom != NULL || House->Is_Human_Player()) { // RA2: any listed construction yard type, not only the first
			enum {
				FIND_SPOT,
				WAITING
			};

			switch (Status) {

				/*
				**	This stage handles locating a convenient spot, rotating to face the correct
				**	direction and then commencing the deployment operation.
				*/
				case FIND_SPOT:
					if (Goto_Clear_Spot()) {
						if (Try_To_Deploy()) {
							Status = WAITING;
						}
					}
					break;

				/*
				**	This stage watchdogs the deployment operation and if for some reason, the deployment
				**	is aborted (the IsDeploying flag becomes false), then it reverts back to hunting for
				**	a convenient spot to deploy.
				*/
				case WAITING:
					if (!IsDeploying) {
						Status = FIND_SPOT;
					}
					break;
			}
			return(Current_Mission_Control().Normal_Delay()+Random_Pick(0, 2));
		}
	}
	return(BASECLASS::Do_MISSION_HUNT()); // RA2: the base mission is the fall-through, the delay return sits inside the deploy branch
}



/***********************************************************************************************
 * UnitClass::Can_Enter_Cell -- Determines cell entry legality.                                *
 *                                                                                             *
 *    Use this routine to determine if the unit can enter the cell                             *
 *    specified and given the direction of entry specified. Typically,                         *
 *    this is used when determining unit travel path.                                          *
 *                                                                                             *
 * INPUT:   cell     -- The cell to examine.                                                   *
 *                                                                                             *
 *          facing   -- The facing that the unit would enter the specified                     *
 *                      cell. If this value is -1, then don't consider                         *
 *                      facing when performing the check.                                      *
 *                                                                                             *
 * OUTPUT:  Returns the reason why it couldn't enter the cell or MOVE_OK if movement is        *
 *          allowed.                                                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/07/1992 JLB : Created.                                                                 *
 *   04/16/1994 JLB : Converted to member function.                                            *
 *   07/04/1995 JLB : Allowed to drive on building trying to enter it.                         *
 *=============================================================================================*/
MoveType UnitClass::Can_Enter_Cell(CellClass const * cellptr, FacingType dir, int cell_height, CellClass const *cellptr2, bool use_locomotor_enter_check) const
{
	bool bridge = cellptr->IsUnderBridge && (cell_height == -1 || abs(cell_height - cellptr->Height) > 1);
	unsigned char flags = cellptr->Flag.Composite;
	bool vehicle = cellptr->Flag.Occupy.Vehicle;
	HousesType inftype = cellptr->InfType;
	TubeClass *tunnel = cellptr->Get_Tunnel();

	if (Class->MovementRestrictedTo != LAND_NONE) {
		if (cellptr->Land_Type() == LAND_TUNNEL) {
			IsometricTileTypeClass * itptr = IsometricTileTypes[cellptr->ITType];
			if (itptr->Width == 5 && itptr->Height == 3 || itptr->Width == 4 && itptr->Height == 3) {
				if (cellptr->SubTile != 2) {
					return(MOVE_NO);
				}
			} else if ((itptr->Width == 3 && itptr->Height == 4 || itptr->Width == 3 && itptr->Height == 5) && cellptr->SubTile != 6) {
				return(MOVE_NO);
			}
		}
		if (cellptr->Land_Type() != Class->MovementRestrictedTo && cellptr->Land_Type() != LAND_TUNNEL) {
			if (cellptr->Overlay < 0xED || cellptr->Overlay > 0xEE || cell_height == cellptr->Height) { // RA2: the rail bridge overlays are 0xED and 0xEE (OVERLAY_RAIL_BRIDGE1/2 are TibSun numbers)
				return(MOVE_NO);
			}
		}
	}

	if (dir == FACING_COUNT) {
		if (tunnel != NULL) {
			Cell exit = tunnel->Exit;
			if (exit != Cell(0, 0)) {
				return(MOVE_OK);
			}
		}
		return(MOVE_NO);
	}

	if (tunnel != NULL) {
		int dist = abs(dir - tunnel->EnterDir);
		if (dist > 2 && dist < 6 && dir != FACING_NONE) {
			return(MOVE_NO);
		}
	}

	/*
	 * Check the tunnel of the cell entered from (the cell in the opposite
	 * direction) so that the unit can't drive through a tunnel sideways.
	 */
	TubeClass *backtunnel = cellptr->Adjacent_Cell(Facing_Sub(dir, FACING_180)).Get_Tunnel();
	if (backtunnel != NULL) {
		int dist = abs(Facing_Sub(dir, FACING_180) - backtunnel->EnterDir);
		if (dist > FACING_90 && dist < FACING_270 && dir != FACING_NONE) {
			return(MOVE_NO);
		}
	}

	MoveType retval = Can_Reach(cellptr, dir, cell_height, bridge, cellptr2);

	if (retval == MOVE_NO) {
		return(MOVE_NO);
	}

	if (cell_height != -1 && cellptr->IsUnderBridge && cell_height == cellptr->Height + BRIDGE_CELL_HEIGHT) {
		flags = cellptr->BridgeFlag.Composite;
		vehicle = cellptr->BridgeFlag.Occupy.Vehicle;
		inftype = cellptr->BridgeInfType;
	}

	/*
	**	Moving off the edge of the map is not allowed unless
	**	this is a loaner vehicle.
	*/
	if (!ScenarioInit && !Map.In_Local_Radar(cellptr) && !Is_Allowed_To_Leave_Map() && IsLocked) {
		return(MOVE_NO);
	}

	retval = BASECLASS::Can_Enter_Cell(cellptr, dir, cell_height, cellptr2, use_locomotor_enter_check);

	if (retval == MOVE_NO) {
		return(MOVE_NO);
	}


	/*
	**	Certain vehicles can drive over walls. Check for this case and
	**	and return the appropriate flag. Other units treat walls as impassable.
	*/
	if (cellptr->Overlay != OVERLAY_NONE) {
		OverlayTypeClass const * optr = OverlayTypes[cellptr->Overlay];

		if (optr->IsCrate && !House->Is_Human_Player() && Session.Type == GAME_NORMAL) {
			return(MOVE_NO);
		}

		if (optr->IsWall) {
			if (optr->IsCrushable && (Class->IsCrusher || Has_Ability(ABILITY_CRUSHER))) { // RA2: a crusher drives over crushable walls of its allies' houses
				if (House->Is_Ally(cellptr->Owner)) { // RA2: as above
					if (retval < MOVE_FRIENDLY_DESTROYABLE) retval = MOVE_FRIENDLY_DESTROYABLE; // RA2: as above
				}
			} else if (Is_Weapon_Equipped()) { // RA2: a unit with a weapon may shoot the wall down
				WarheadTypeClass const * warhead = Get_Class_Weapon_Data(0)->Weapon->WarheadPtr; // RA2: as above
				if (warhead->IsWallDestroyer || (warhead->IsWoodDestroyer && optr->Armor == 6)) { // RA2: as above
					if (House->Is_Ally(cellptr->Owner)) { // RA2: as above
						if (retval < MOVE_FRIENDLY_DESTROYABLE) retval = MOVE_FRIENDLY_DESTROYABLE; // RA2: as above
					} else {
						if (retval < MOVE_DESTROYABLE) retval = MOVE_DESTROYABLE; // RA2: as above
					}
				} else {
					return(MOVE_NO); // RA2: as above
				}
			} else {
				return(MOVE_NO); // RA2: as above
			}
		}
	}

	/*
	**	Loop through all of the objects in the square setting a bit
	**	for how they affect movement.
	*/
	bool crushable = false;
	ObjectClass * obj = cellptr->Cell_Occupier(bridge);
	while (obj != NULL) {

		if (const_cast<UnitClass *>(this) != obj) {

			if (ParasiteImUsing != NULL && ParasiteImUsing->Victim != NULL) { // RA2: a parasite's victim cell is always enterable
				if (obj->Get_Cell() == ParasiteImUsing->Victim->Get_Cell()) { // RA2: as above
					return(MOVE_OK); // RA2: as above
				}
			}

			TechnoClass * contact = (obj->AbstractFlags & 1) ? (TechnoClass *)obj : NULL; // RA2: the Is_Techno test is inline
			if (ContainsLink(contact) && obj->Fetch_RTTI() == RTTI_BUILDING && !((BuildingClass *)obj)->Is_Impassable_Cell(cellptr)) { // RA2: a building this unit is linked to lets it through
				obj = obj->Next; // RA2: as above
				continue; // RA2: as above
			}

			if (Class->IsSmallVisceroid && obj->Fetch_RTTI() == RTTI_UNIT && ((UnitClass *)obj)->Class->IsSmallVisceroid) { // RA2: as above
				return(MOVE_OK);
			}

			if (obj->Fetch_RTTI() == RTTI_BUILDING) { // RA2: as above
				BuildingClass *bptr = (BuildingClass *)obj;

				if (bptr->Class->IsGate) {
					if (!bptr->Is_Gate_Open()) {
						if (bptr->House->Is_Ally(House)) {
							if (retval < MOVE_CLOSED_GATE) retval = MOVE_CLOSED_GATE;
						} else {
							if (!Is_Weapon_Equipped()) {
								return(MOVE_NO);
							}
							if (retval < MOVE_DESTROYABLE) retval = MOVE_DESTROYABLE;
						}
					}
					obj = obj->Next;
					continue;
				}

				if (dir != FACING_NONE && bptr->Class->IsCanUnitRepair && cellptr->Cell_Building() == bptr && !bptr->Is_Impassable_Cell(cellptr)) { // RA2: the repair bay's own cell is open to a unit not coming from nowhere
					obj = obj->Next;
					continue;
				}

				if (bptr->Class->IsInvisibleInGame) {
					obj = obj->Next; // RA2: as above
					continue;
				}
				if (bptr->Class->IsLaserFence && (bptr->LaserFenceFrame == 12 || bptr->LaserFenceFrame == 8)) {
					obj = obj->Next;
					continue;
				}
				if (bptr->Class->IsFirestormWall) {
					if (bptr->House->FirestormDefenseActivated) {
						return(MOVE_NO);
					}
					obj = obj->Next;
					continue;
				}

				if (bptr->Class->IsBibbed) {
					if (Map[Adjacent_Cell(cellptr->CellID, FACING_E)].Cell_Building() != bptr) {
						obj = obj->Next;
						continue;
					}
				} // RA2: no harvester dock exception

			}

			if (Get_Mission() == MISSION_ENTER && obj == NavCom && obj->Fetch_RTTI() == RTTI_UNIT) { // RA2: the transport this unit is entering
				return(MOVE_OK);
			}

			if (Get_Mission() == MISSION_GUARD_AREA && ArchivedTarget == obj) { // RA2: as above
				return(MOVE_NO);
			}

			FootClass * foot; // RA2: as above
			if (obj->AbstractFlags & 4) { // RA2: the Is_Foot test is inline
				foot = (FootClass *)obj; // RA2: as above
			} else {
				foot = NULL; // RA2: as above
			}

			bool is_moving = foot != NULL &&
					(foot->NavCom || foot->PrimaryFacing.Is_Rotating() || foot->Locomotion->Is_Moving());

			if (House->Is_Ally((TechnoClass *)obj)) { // RA2: as above
				if (is_moving) {
					int face = PrimaryFacing.Current().As_Dir8();
					DirType reverse = DirType((unsigned short)(((FootClass *)obj)->PrimaryFacing.Current().Raw + 0x7FFF)); // RA2: the occupier's facing turned around
					int techface = reverse.As_Dir8(); // RA2: as above
					int & tech = techface; // RA2: as meastes
					DirType direction = ::Direction(Get_Coord(), obj->Get_Coord()); // RA2: as above
					if (face == tech && Center_Coord().Distance_To(obj->Center_Coord()) <= 2 * CELL_LEPTON - 1 && direction.As_Dir8() == face) { // RA2: as above
						return(MOVE_NO);
					}
					if ((foot->IsOccupyingCell && foot->Fetch_RTTI() != RTTI_INFANTRY) || foot->Locomotion->Will_Jump_Tracks()) { // RA2: as above
						if (retval < MOVE_MOVING_BLOCK) retval = MOVE_MOVING_BLOCK;
					}
				} else {
					if (obj->Fetch_RTTI() == RTTI_BUILDING) return(MOVE_NO); // RA2: as above
					if (retval < MOVE_TEMP) retval = MOVE_TEMP; // RA2: any other idle allied object blocks only temporarily
				}
			} else {

				TechnoClass const * tech = Dynamic_Cast<TechnoClass const *>((AbstractClass const *)obj); // RA2: the const overload (0x40DBD0)
				if (tech != NULL && tech->Cloak == CLOAKED) { // RA2: as above
					if (retval < MOVE_CLOAK) retval = MOVE_CLOAK;
				} else {
					ObjectTypeClass const * otype; // RA2: as above
					if ((Class->IsCrusher || Has_Ability(ABILITY_CRUSHER)) && (otype = obj->Class_Of())->IsCrushable) { // RA2: as above
						if (!House->Is_Ally((TechnoClass *)obj)) { // RA2: as above
							crushable = true; // RA2: as above
						}
					} else {
						if (!Get_Class_Weapon_Data(0)->Weapon && !Class->IsTrain) { // RA2: a unit without a weapon cannot enter
							return(MOVE_NO); // RA2: as above
						}
						switch (obj->Fetch_RTTI()) { // RA2: as above
						case RTTI_TERRAIN: { // RA2: as above
							int which = What_Weapon_Should_I_Use(obj); // RA2: as above
							WeaponTypeClass const * weapon = Get_Class_Weapon_Data(which)->Weapon; // RA2: as above
							if (weapon == NULL || weapon->WarheadPtr == NULL || !weapon->WarheadPtr->IsWoodDestroyer) { // RA2: as above
								return(MOVE_NO); // RA2: as above
							}
						}
						default: // RA2: as above
							if (retval < MOVE_DESTROYABLE) retval = MOVE_DESTROYABLE; // RA2: as above
							break; // RA2: as above
						case RTTI_BUILDING: // RA2: as above
							if (((BuildingClass *)obj)->Class->IsBridgeRepairHut) return(MOVE_NO); // RA2: as above
							if (retval < MOVE_DESTROYABLE) retval = MOVE_DESTROYABLE; // RA2: as above
							break; // RA2: as above
						}
					}
				}
			}
		} else {
			vehicle = false;
			flags &= ~0x20;
		}

		obj = obj->Next;
	}

	if (!bridge && Ground[cellptr->Land_Type()].Cost[Class->Speed] == 0) {
		return(MOVE_NO);
	}

	if (retval == MOVE_OK && !crushable && (flags & 0x3F)) { // RA2: a cell reserved by someone else
		if (vehicle) { // RA2: as above
			retval = MOVE_MOVING_BLOCK; // RA2: as above
		} else if (inftype != HOUSE_NONE && House->Is_Ally(inftype)) { // RA2: as above
			retval = MOVE_MOVING_BLOCK; // RA2: as above
		} else if (!Class->IsCrusher && !Has_Ability(ABILITY_CRUSHER)) { // RA2: as above
			if (Class->IsTrain || (Get_Class_Weapon_Data(0)->Weapon != NULL && Get_Class_Weapon_Data(0)->Weapon->Bullet->IsAntiGround)) { // RA2: as above
				retval = MOVE_DESTROYABLE; // RA2: as above
			} else {
				return(MOVE_NO); // RA2: as above
			}
		}
	}

	if (retval == MOVE_OK && crushable && vehicle) { // RA2: as above
		if (cellptr->Cell_Unit() == NULL || !((UnitClass *)cellptr->Cell_Unit())->Class->IsCrushable) { // RA2: as above
			return(MOVE_MOVING_BLOCK); // RA2: as above
		}
	}

	return(retval);
}



/***********************************************************************************************
 * UnitClass::What_Action -- Determines what action would occur if clicked on object.          *
 *                                                                                             *
 *    Use this function to determine what action would likely occur if the specified object    *
 *    were clicked on while this unit was selected as current. This function controls, not     *
 *    only the action to perform, but indirectly controls the cursor shape to use as well.     *
 *                                                                                             *
 * INPUT:   object   -- The object that to check for against "this" object.                    *
 *                                                                                             *
 * OUTPUT:  Returns with the default action to perform. If no clear action can be determined,  *
 *          then ACTION_NONE is returned.                                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/11/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ActionType UnitClass::What_Action(ObjectClass const * object, bool disallow_force) const
{
	ActionType action = BASECLASS::What_Action(object, disallow_force);

	if (action == ACTION_TOGGLE_SELECT) { // RA2: a toggle is returned untouched
		return(ACTION_TOGGLE_SELECT); // RA2: as above
	}

	/*
	**	A unit that does damage by default only supports its friends and ignores the rest.
	*/
	if (Combat_Damage() < 0 && House->Is_Player_Control() && action != ACTION_SELF) { // RA2: this block comes first, and skips the deploy action
		if (House->Is_Ally(object)) {
			if (object->Considered_Vehicle() && object != this && object->Not_Underground()) {
				if ( object->RTTI != RTTI_AIRCRAFT || Map[object->Center_Coord()].Cell_Building() == NULL) {
					if (object->HealthRatio < Rule->ConditionGreen) {
						action = ACTION_GREPAIR;
					}
				}
			} else if ( object->RTTI != RTTI_BUILDING ) {
				action = ACTION_SELECT;
			}
		} else {
			action = ACTION_NONE; // RA2: an enemy gives no action at all
		}
	}

	/*
	**	If the unit doesn't have a weapon, but can crush the object, then consider
	**	the object as a movable location.
	*/
	if (action == ACTION_ATTACK) {
		if (!Can_Player_Fire()) { // RA2: a crusher moves over a crushable object
			if ((Class->IsCrusher || Has_Ability(ABILITY_CRUSHER)) && object->Class_Of()->IsCrushable) { // RA2: as above
				action = ACTION_MOVE; // RA2: as above
			} else {
				action = ACTION_SELECT; // RA2: as above
			}
		}
	} else if (action == ACTION_SELF) { // RA2: the deploy checks run only for a legal planning event
		if (Planning_Allows_Event_Type(action)) { // RA2: as above
			if (Class->DeploysInto != NULL) {
				Cell cell = Center_Coord().As_Cell();
				for (int index = 0; index < Rule->BuildConst.Count(); index++) { // RA2: a listed construction yard type deploys at the south-west neighbor
					if (Rule->BuildConst[index] == Class->DeploysInto) { // RA2: as above
						cell = cell + AdjacentCell[7]; // RA2: as above
						break; // RA2: as above
					}
				}

				/*
				**	The MCV will get the no-deploy cursor if it couldn't
				**	deploy at its current location.
				*/
				((UnitClass &)(*this)).Mark(MARK_UP);
				Locomotion->Mark_All_Occupation_Bits(MARK_UP);
				if (!Class->DeploysInto->Legal_Placement(cell)) {
					action = ACTION_NO_DEPLOY;
				}
				Locomotion->Mark_All_Occupation_Bits(MARK_DOWN);
				((UnitClass &)(*this)).Mark(MARK_DOWN);
			} else if (!Class->IsSimpleDeployer) { // RA2: a simple deployer needs no passenger checks
				if (Class->Max_Passengers() > 0) {
					if (Cargo.How_Many() == 0) {
						action = ACTION_NO_DEPLOY;
					} else if (!IsOnBridge) { // RA2: as above
						CellClass * cellptr = &Map[(Coord const &)PositionCoord];
						ObjectClass * passenger = Cargo.Attached_Object(); // RA2: as above
						if (passenger != NULL && Ground[cellptr->Land_Type()].Cost[passenger->TClass->Speed] < 0.01 && cellptr->Land_Type() != LAND_WATER) { // RA2: a passenger that cannot leave here blocks the deploy
							action = ACTION_NO_DEPLOY; // RA2: as above
						}
						if (Class->Speed != SPEED_HOVER && HeightAGL != 0) { // RA2: a non-hovering unit above ground cannot unload
							action = ACTION_NO_DEPLOY; // RA2: as above
						}
					}
				} else {
					action = ACTION_NONE; // RA2: a unit without passengers has no self action
				}
			}
			if (action == ACTION_SELF && !Can_Deploy_Now()) {
				action = ACTION_NO_DEPLOY;
			}
		} else {
			action = ACTION_NO_DEPLOY; // RA2: planning mode forbids the deploy
		}
	}

	TechnoClass const * techno = Dynamic_Cast<TechnoClass const *>((AbstractClass const *)object);

	/*
	**	Special return to friendly refinery action.
	*/
	if (House->Is_Player_Control() && techno != NULL && techno->House->Is_Ally(this)) { // RA2: no reverse ally test
		if (object->RTTI == RTTI_BUILDING && ((UnitClass *)this)->Transmit_Message(RADIO_CAN_LOAD, (TechnoClass*)object) == RADIO_ROGER) {
			action = ACTION_ENTER;
		}
	}

	/*
	**	Special return to friendly repair factory action.
	*/
	if (House->Is_Player_Control() && action == ACTION_SELECT) { // RA2: the repair factory test is nested differently
		if (object->RTTI == RTTI_BUILDING) { // RA2: as above
			BuildingClass * building = (BuildingClass *)object;
			if (building->Class->IsCanUnitRepair && ((UnitClass *)this)->Transmit_Message(RADIO_CAN_LOAD, building) == RADIO_ROGER && !building->HasAnyLink() && !building->Cargo.Is_Something_Attached()) { // RA2: no ally test on the building, and HasAnyLink for the radio contact
				action = ACTION_MOVE;
			}
		}
	} else if (action == ACTION_ATTACK) { // RA2: as above
		if (!Is_Weapon_Equipped()) {
			action = ACTION_ATTACK_SUPPORT;
		}
	}

	if (action != ACTION_NO_ENTER) { // RA2: an allied transport can be entered unless it is already moving
		if (House->Is_Ally(object) && House->Is_Player_Control() && Dynamic_Cast<TechnoClass const *>((AbstractClass const *)object) != NULL && action != ACTION_ATTACK && action != ACTION_GREPAIR && action != ACTION_GUARD_AREA) { // RA2: as above
			TechnoTypeClass const * type = object->Techno_Type_Class(); // RA2: as above
			if (object != this && type != NULL && type->MaxPassengers > 0) { // RA2: as above
				if (object != NULL && (object->AbstractFlags & 4) && ((FootClass const *)object)->Locomotion->Is_Moving()) { // RA2: as above
					action = ACTION_NO_ENTER; // RA2: as above
				} else {
					switch (((UnitClass *)this)->Transmit_Message(RADIO_CAN_LOAD, (TechnoClass *)object)) { // RA2: as above
						case RADIO_ROGER:
							action = ACTION_ENTER;
							break;

						case RADIO_NEGATIVE: // RA2: as above
							action = ACTION_NO_ENTER; // RA2: as above
							break;
					}
				}
			}
		}
	}

	/*
	**	If it doesn't know what to do with the object, then just
	**	say it can't move there.
	*/
	if (action == ACTION_NONE && House->Is_Player_Control()) {
		action = ACTION_NOMOVE;
	}

	if (action == ACTION_ATTACK && !Can_Attack_Now()) {
		action = ACTION_NONE;
	}

	if (action == ACTION_NOMOVE) { // RA2: a disguised object whose disguise is a unit is moved onto
		if (object->IsDisguised() && object->GetDisguiseHouse(true) == NULL) { // RA2: as above
			if (object->GetDisguise(true)->Fetch_RTTI() == RTTI_OVERLAYTYPE) { // RA2: as above
				action = ACTION_MOVE; // RA2: as above
			}
		}
	} else if (action == ACTION_ATTACK && object->Class_Of()->IsImmune) { // RA2: an immune object is not attacked
		action = ACTION_NOMOVE; // RA2: as above
	}

	return(action);
}




/***********************************************************************************************
 * UnitClass::What_Action -- Determines action to perform on specified cell.                   *
 *                                                                                             *
 *    This routine will determine what action to perform if the mouse were clicked over the    *
 *    cell specified. At the unit level, only the harvester is checked for. The lower          *
 *    classes determine the regular action response.                                           *
 *                                                                                             *
 * INPUT:   cell  -- The cell that the mouse might be clicked on.                              *
 *                                                                                             *
 * OUTPUT:  Returns with the action type that this unit will perform if the mouse were         *
 *          clicked of the cell specified.                                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ActionType UnitClass::What_Action(Cell const & cell, bool check_fog, bool disallow_force) const
{
	if (!House->Is_Player_Control()) {
		return(ACTION_NONE);
	}

	CellClass * cellptr = &Map[cell];

	ActionType action = BASECLASS::What_Action(cell, check_fog, disallow_force);

	if (Combat_Damage() < 0 && House->Is_Player_Control() && action == ACTION_ATTACK) {
		action = ACTION_ATTACK_SUPPORT;
	}

	OverlayType overlay = OVERLAY_NONE;
	if (check_fog && Scen->Special.IsFogOfWar && cellptr->FoggedObjects != NULL) {
		for (int i = 0; i < cellptr->FoggedObjects->Count(); i++) {
			FoggedObjectClass * fogged = (*cellptr->FoggedObjects)[i];
			if (fogged->CanDraw && fogged->RTTI == RTTI_OVERLAY) {
				overlay = fogged->Overlay;
			}
		}
	}

	Coord coord = cell;
	coord.Z = Map.Get_Height_GL(coord);
	if (Map[coord].IsUnderBridge) {
		coord.Z += BRIDGE_LEPTON_HEIGHT;
	}

	if (!Map.Is_Shrouded(coord)) {
		if (action == ACTION_MOVE) {
			if ((cellptr->Land_Type() == LAND_TIBERIUM || overlay != OVERLAY_NONE && OverlayTypes[overlay]->IsTiberium && check_fog) && Class->IsToHarvest) {
				return(ACTION_HARVEST);
			}
			if ((cellptr->Land_Type() == LAND_WEEDS || overlay == OVERLAY_VEINS && cellptr->OverlayData >= OVERLAYDATA_FIRST_SOLID_VEIN && check_fog) && Class->IsToVeinHarvest) {
				return(ACTION_HARVEST);
			}
		}
	}

	if (action == ACTION_MOVE) {
		if (Map[cell].Has_Tunnel()) {
			if (Map[cell].Can_Enter_Tunnel((FootClass *)this)) {
				return(ACTION_ENTER_TUNNEL);
			}
			return(ACTION_NO_ENTER_TUNNEL);
		}
		if (Deployed) { // RA2: a deployed unit cannot move there
			action = ACTION_NONE; // RA2: as above
		}
	} else {
		if (action == ACTION_ATTACK && !Can_Attack_Now()) {
			action = ACTION_NONE;
		}
	}

	return(action);
}



/***********************************************************************************************
 * UnitClass::Mission_Guard -- Special guard mission override processor.                       *
 *                                                                                             *
 *    Handles the guard mission for the unit. If the IQ is high enough and the unit is         *
 *    a harvester, it will begin to harvest automatically. An MCV might autodeploy.            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the time delay before this command is executed again.                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *   05/08/1995 JLB : Fixes gunboat problems.                                                  *
 *=============================================================================================*/
int UnitClass::Do_MISSION_GUARD(void)
{
	if (Class->IsToHarvest || Class->IsToVeinHarvest) { // RA2: harvesters decide between guarding and harvesting here
		if (!House->Is_Human_Player()) { // RA2: as above, the computer looks for an owned refinery
			for (int i = 0; i < Class->Dock.Count(); i++) { // RA2: as above
				if (House->BQuantity.Value(Class->Dock[i]->HeapID) > 0) { // RA2: as above
					if (!Class->IsToHarvest || !House->IsTiberiumShort) { // RA2: as above
						QueueMission(MISSION_HARVEST, false); // RA2: queued, as above
						return(1); // RA2: as above
					}
					break; // RA2: as above
				}
			}
		} else if (Class->Teleporter) { // RA2: a human's teleporting harvester goes back to work next to its own refinery
			CellClass * cell = Get_Cell_Ptr(); // RA2: as above
			for (FacingType facing = FACING_N; facing < FACING_COUNT; facing++) { // RA2: as above
				BuildingClass * building = cell->Adjacent_Cell(facing).Cell_Building(); // RA2: as above
				if (building != NULL && building->Class->IsRefinery && building->House == House) { // RA2: as above
					QueueMission(MISSION_HARVEST, false); // RA2: as above
					return(1); // RA2: as above
				}
			}
			if (Tiberium_Load() == 1.0 && Locomotion->Is_Moving()) { // RA2: a full load that is already moving
				QueueMission(MISSION_HARVEST, false); // RA2: as above
				return(1); // RA2: as above
			}
		}
	}

	BuildingTypeClass * building = Class->DeploysInto; // RA2: a computer-owned deployable that is a listed construction yard type
	if (building != NULL) { // RA2: as above
		bool listed = false; // RA2: an inlined search of Rule->BuildConst
		for (int index = 0; index < Rule->BuildConst.Count(); index++) { // RA2: as above
			if (Rule->BuildConst[index] == building) { // RA2: as above
				listed = true; // RA2: as above
				break; // RA2: as above
			}
		}
		if (listed && House->IsBaseBuilding && !House->Is_Human_Player()) { // RA2: as above
			QueueMission(MISSION_UNLOAD, false); // RA2: queued, as above
			return(Current_Mission_Control().Normal_Delay() + Random_Pick(0, 2)); // RA2: as above
		}
	}

	if (Class->IsToVeinHarvest && IsDroppedFromTeam) {
		IsDroppedFromTeam = false;
		QueueMission(MISSION_HARVEST, false); // RA2: queued, as YR's
	}
	return(BASECLASS::Do_MISSION_GUARD());
}



/***********************************************************************************************
 * UnitClass::Mission_Move -- Handles special move mission overrides.                          *
 *                                                                                             *
 *    This routine intercepts the normal move mission and if a gunboat is being processed,     *
 *    changes its mission to hunt. This is an attempt to keep the gunboat on the hunt mission  *
 *    regardless of what the player did.                                                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the number of ticks before this routine should be called again.            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/09/1995 JLB : Created.                                                                 *
 *   09/28/1995 JLB : Harvester stick in guard mode if no more refineries.                     *
 *=============================================================================================*/
int UnitClass::Do_MISSION_MOVE(void)
{
	IsHarvesting = false;

	if (Deployed) { // RA2: a deployed unit only queues the guard mission instead of moving
		QueueMission(MISSION_GUARD, false); // RA2: as above
		return(1); // RA2: as above
	}

	/*
	**	Always make sure that that transport door is closed if the vehicle is moving.
	*/
	if (!Door.Is_Count_Down_Complete()) { // RA2: the timer test is called out of line, not through Is_Door_Closed
		Door.Count_Down(Class->DeployTime); // RA2: as above for Close_Door
	}

	return(BASECLASS::Do_MISSION_MOVE());
}



/// <summary>
/// Handles the patrol mission for a vehicle.
/// This routine sees the transport door shut before handing the mission over to the
/// normal foot object patrol logic.
/// </summary>
/// <returns>The delay in game frames before this mission should be processed again.</returns>
int UnitClass::Do_MISSION_PATROL(void)
{
	IsHarvesting = false;

	if (!Door.Is_Door_Closed()) {
		APC_Close_Door();
	}

	return(BASECLASS::Do_MISSION_PATROL());
}


/// <summary>
/// RA2
/// </summary>
static inline DirType & _Direction(DirType & dir, ObjectClass const * from, ObjectClass const * to)
{
	return(dir.Direction(from->Center_Coord(), to->Center_Coord()));
}


/***********************************************************************************************
 * UnitClass::Desired_Load_Dir -- Determines the best cell and facing for loading.             *
 *                                                                                             *
 *    This routine examines the unit and adjacent cells in order to find the best facing       *
 *    for the transport and best staging cell for the potential passengers. This location is   *
 *    modified by adjacent cell passability and direction of the potential passenger.          *
 *                                                                                             *
 * INPUT:   passenger   -- Pointer to the potential passenger.                                 *
 *                                                                                             *
 *          moveto      -- Reference to the cell number that specifies where the potential     *
 *                         passenger should move to first.                                     *
 *                                                                                             *
 * OUTPUT:  Returns with the direction the transport should face before opening the transport  *
 *          door.                                                                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
FacingType UnitClass::Desired_Load_Dir(ObjectClass * passenger, Cell & moveto) const
{
	FacingType face = FACING_N;
	FacingType faceto;
	if (passenger != NULL) {
		DirType direction = _Direction(direction, this, passenger); // RA2: Desired_Load_Dir at 0x704730 matches only so
		faceto = (FacingType)direction.As_Dir256();
	} else {
		faceto = (FacingType)(PrimaryFacing.Current().Right_180()).As_Dir256();
	}
	int desired = (int)(signed char)faceto;

	FacingType bestdir = FACING_N;
	int bestval = -1;
	for (; face < FACING_COUNT; face++) {
		int value = 0;
		Cell cellnum = Adjacent_Cell(PositionCell, face);

		if (passenger != NULL) {
			value = (passenger->Can_Enter_Cell(&Map[cellnum], face, Get_Cell_Height()) == MOVE_OK || passenger->PositionCell == cellnum) ? 128 : -128;
		} else {
			CellClass * cell = &Map[Cell(cellnum)];
			if (Ground[cell->Land_Type()].Cost[SPEED_FOOT] == 0 || cell->Flag.Occupy.Building || cell->Flag.Occupy.Vehicle || cell->Flag.Occupy.Monolith || (cell->Flag.Composite & 0x01F) == 0x01F) {
				value = -128;
			} else {
				if (cell->Cell_Techno() && !House->Is_Ally(cell->Cell_Techno())) {
					value = -128;
				} else {
					value = 128;
				}
			}
		}

		value -= (int)abs((int)(signed char)DirType(face).As_Dir256() - desired);
		if (face == FACING_S) {
			value -= 100;
		}

		if (bestval == -1 || value > bestval) {
			bestval = value;
			bestdir = face;
		}
	}

	FacingType facing = FACING_S;
	moveto = CELL_NONE;
	if (bestval > 0) {
		moveto = Adjacent_Cell(PositionCell, bestdir);
		facing = (FacingType)Harvester_Load_List[bestdir];
	}

	if (Class->IsTrain) {
		facing = PrimaryFacing.Current().As_Dir8();
	}

	return(facing);
}


/***********************************************************************************************
 * UnitClass::Flag_Attach -- Attaches a house flag to this unit.                               *
 *                                                                                             *
 *    This routine will attach a house flag to this unit.                                      *
 *                                                                                             *
 * INPUT:   house -- The house that is having its flag attached to it.                         *
 *                                                                                             *
 * OUTPUT:  Was the house flag successfully attached to this unit?                             *
 *                                                                                             *
 * WARNINGS:   A unit can only carry one flag at a time. This might be a reason for failure    *
 *             of this routine.                                                                *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool UnitClass::Flag_Attach(HousesType house)
{
	if (house != HOUSE_NONE && Flagged == HOUSE_NONE) {
		Flagged = house;
		Mark(MARK_CHANGE);
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * UnitClass::Flag_Remove -- Removes the house flag from this unit.                            *
 *                                                                                             *
 *    This routine will remove the house flag that is attached to this unit.                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Was the flag successfully removed?                                                 *
 *                                                                                             *
 * WARNINGS:   This routine doesn't put the flag into a new location. That operation must      *
 *             be performed or else the house flag will cease to exist.                        *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool UnitClass::Flag_Remove(void)
{
	if (Flagged != HOUSE_NONE) {
		Flagged = HOUSE_NONE;
		Mark(MARK_CHANGE);
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * UnitClass::Pip_Count -- Fetches the number of pips to display on unit.                      *
 *                                                                                             *
 *    This routine is used to fetch the number of "fullness" pips to display on the unit.      *
 *    This will either be the number of passengers or the percentage full (in 1/5ths) of       *
 *    a harvester.                                                                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of pips to draw on this unit.                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int UnitClass::Pip_Count(void) const
{
	switch (Class->PipScale) { // RA2: RA2's form
	case 2: // RA2: as above
	{
		double ratio = Storage.Get_Total_Amount() / Class->Capacity; // RA2: as above
		return (int)(Class->Max_Pips() * ratio + 0.5); // RA2: as above
	}
	}
	return TechnoClass::Pip_Count(); // RA2: as above
}



/***********************************************************************************************
 * UnitClass::APC_Close_Door -- Closes an APC door.                                            *
 *                                                                                             *
 *    This routine will initiate closing of the APC door.                                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void UnitClass::APC_Close_Door(void)
{
	Door.Close_Door(Class->DeployTime);
}


/***********************************************************************************************
 * UnitClass::APC_Open_Door -- Opens an APC door.                                              *
 *                                                                                             *
 *    This routine will initiate opening of the APC door.                                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void UnitClass::APC_Open_Door(void)
{
	if (!Locomotion->Is_Moving() && !IsRotating) {
		Door.Open_Door(Class->DeployTime);
	}
}


/***********************************************************************************************
 * UnitClass::Crew_Type -- Fetches the kind of crew that this object produces.                 *
 *                                                                                             *
 *    When a unit is destroyed, a crew member might be generated. This routine will return     *
 *    with the infantry type to produce for this unit. This routine will be called for every   *
 *    survivor that is generated.                                                              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a suggested infantry type to generate as a survivor from this unit.   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/13/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
InfantryTypeClass const * UnitClass::Crew_Type(void) const
{
	return(BASECLASS::Crew_Type());
}


/***********************************************************************************************
 * UnitClass::Mission_Repair -- Handles finding and proceeding on a repair mission.            *
 *                                                                                             *
 *    This mission handler will look for a repair facility. If one is found then contact       *
 *    is established and then the normal Mission_Enter logic is performed. The repair facility *
 *    will take over the actual repair coordination process.                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the number of game frames to delay before calling this routine again.      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/02/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int UnitClass::Do_MISSION_REPAIR(void)
{
	BuildingClass * nearest = FindNearestDockBuilding((BuildingTypeClass *)Rule->RepairBay, 0, 0); // RA2: the repair bay type goes to FindNearestDockBuilding (slot 0x4B4), not TryNearestDockBuilding

	IsHarvesting = false;

	if (nearest == NULL) {
		if (House->BQuantity.Value(Rule->RepairBay->HeapID) == 0) { // RA2: only with no repair bay owned at all does the unit give up
			Enter_Idle_Mode(false, true); // RA2: as above, resuming the waypoint
		}
	} else {
		if (Transmit_Message(RADIO_HELLO, nearest) == RADIO_ROGER) {
			QueueMission(MISSION_ENTER, false); // RA2: queued, as YR's
			return(1);
		}
	}

	return(Current_Mission_Control().Normal_Delay());
}



/***********************************************************************************************
 * UnitClass::Fire_Direction -- Determines the direction of firing.                            *
 *                                                                                             *
 *    This routine will return with the facing that a projectile will travel if it was         *
 *    fired at this instant. The facing should match the turret facing for those units         *
 *    equipped with a turret. If the unit doesn't have a turret, then it will be the facing    *
 *    of the body.                                                                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the default firing direction for a projectile.                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
DirType UnitClass::Fire_Direction(void) const
{
	if (Class->IsTurretEquipped) {
		return(SecondaryFacing.Current());
	}

	return(BASECLASS::Fire_Direction());
}


/***********************************************************************************************
 * UnitClass::Can_Fire -- Determines if turret can fire upon target.                           *
 *                                                                                             *
 *    This routine determines if the turret can fire upon the target                           *
 *    specified.                                                                               *
 *                                                                                             *
 * INPUT:   target   -- The target to fire upon.                                               *
 *                                                                                             *
 *          which    -- Which weapon to use to determine legality to fire. 0=primary,          *
 *                      1=secondary.                                                           *
 *                                                                                             *
 * OUTPUT:  Returns the fire status type that indicates if firing is allowed and if not, why.  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/26/1994 JLB : Created.                                                                 *
 *   06/01/1994 JLB : Returns reason why it can't fire.                                        *
 *=============================================================================================*/
FireErrorType UnitClass::Can_Fire(AbstractClass * target, int which, bool ignoreRange) const // RA2: ignoreRange
{
	if (SpawnManager != NULL && SpawnManager->CountLaunchingSpawns() != 0) { // RA2: a unit whose spawns are still launching is busy
		return(FIRE_BUSY); // RA2: as above
	}

	FireErrorType	fire = BASECLASS::Can_Fire(target, which, ignoreRange); // RA2: the range flag is passed on

	if (fire == FIRE_OK || fire == FIRE_FACING) {
		bool buildable = Map[(Coord const &)PositionCoord].Can_Build_Here();
		if (Class->IsDeployToFire && Deploy_To_Fire() && !buildable) {
			return(FIRE_MUST_DEPLOY);
		}
	}

	if (fire == FIRE_OK) {
		if (IsTethered && Radio_Link(0)->Fetch_RTTI() == RTTI_BUILDING) { // RA2: the link is read through Radio_Link
			return(FIRE_CANT);
		}

		WeaponTypeClass const * weapon = Get_Class_Weapon_Data(which)->Weapon;

		if (Combat_Damage() < 0) {
			TechnoClass const * techno = Dynamic_Cast<TechnoClass const *>((AbstractClass const *)target);
			if (techno == NULL || !techno->Considered_Vehicle() || techno->HealthRatio >= Rule->ConditionGreen) {
				return(FIRE_ILLEGAL);
			}
		}

		/*
		**	If this unit cannot fire while moving, then bail.
		*/
		if (!Class->MobileFire && NavCom != NULL) { // RA2: a type that cannot fire on the move
			return(FIRE_MOVING); // RA2: as above
		}

		if ((weapon->UseSparkParticles || weapon->UseFireParticles) && NavCom != NULL) {
			return(FIRE_MOVING);
		}

		if (TemporalImUsing != NULL && NavCom != NULL) { // RA2: a temporal beam in use blocks firing on the move
			return(FIRE_MOVING); // RA2: as above
		}

		/*
		**	If the turret is rotating and the projectile isn't a homing type, then
		**	firing must be delayed until the rotation stops.
		*/
		if (!IsFiring && IsRotating && weapon->Bullet->ROT == 0) {
			return(FIRE_ROTATING);
		}

		if (!weapon->OmniFire && !Class->IsLargeVisceroid && !Class->IsSmallVisceroid) { // RA2: a weapon that needs facing checks the angle to the target
			DirType current; // RA2: the turret or body facing
			if (Class->IsTurretEquipped) { // RA2: as above
				current = SecondaryFacing.Current(); // RA2: as above
			} else {
				current = PrimaryFacing.Current(); // RA2: as above
			}
			int steps = weapon->Bullet->ROT ? 16 : 8; // RA2: a homing projectile tolerates a wider angle
			if (DirType(steps << 8) < current - Direction(target)) { // RA2: DirType's magnitude ordering: the turn limit against the angle to the target
				return(FIRE_FACING); // RA2: as above
			}
		}

		FireErrorType locomotion_fire = Locomotion->Can_Fire();
		if (locomotion_fire != FIRE_OK) {
			return(locomotion_fire);
		}
	}
	return(fire);
}




/***********************************************************************************************
 * UnitClass::Fire_At -- Try to fire upon the target specified.                                *
 *                                                                                             *
 *    This routine is the auto-fire logic for the turret. It will check                        *
 *    to see if firing is technically legal given the specified target.                        *
 *    If it is legal to fire, it does so. It is safe to call this routine                      *
 *    every game tick.                                                                         *
 *                                                                                             *
 * INPUT:   target   -- The target to fire upon.                                               *
 *                                                                                             *
 *          which    -- Which weapon to use when firing. 0=primary, 1=secondary.               *
 *                                                                                             *
 * OUTPUT:  bool; Did firing occur?                                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/26/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
BulletClass * UnitClass::Fire_At(AbstractClass * target, int which)
{
	BulletClass *bullet = 0; // RA2: RA2's form
	WeaponTypeClass *weapon = Get_Class_Weapon_Data(which)->Weapon; // RA2: as above
	if (!weapon) { // RA2: as above
		return(0); // RA2: as above
	}
	bool synchronized = false; // RA2: as above
	if (which == 0) { // RA2: as above
		int burst = BurstIndex % weapon->Burst; // RA2: as above
		int frame; // RA2: as above
		if (burst < 2) { // RA2: as above
			frame = Class->FiringSyncFrame[burst]; // RA2: as above
		} else { // RA2: as above
			frame = -1; // RA2: as above
		}
		if (FiringSyncDelay != -1 && frame != -1) { // RA2: as above
			synchronized = true; // RA2: as above
		} else { // RA2: as above
			synchronized = false; // RA2: as above
		}
	}
	if (!Can_Fire(target, which, 1)) { // RA2: as above
		bullet = TechnoClass::Fire_At(target, which); // RA2: as above
		if (bullet) { // RA2: as above
			if (Class->MaxAmmo > 0 && Ammo < Class->MaxAmmo && !Class->IsManualReload) { // RA2: as above
				Start_Reload_Timer(); // RA2: as above
			}
			if (!synchronized && Class->FiringFrames > 0) { // RA2: as above
				FiringSyncDelay = Class->FiringFrames * 2 - 1; // RA2: as above
			}
			if (bullet->Class->Scalable) { // RA2: as above
				bullet->InitScalable(); // RA2: as above
			}
			if (Class->DisguiseWhenStill) { // RA2: as above
				DisguiseBlinkTimer = CDTimerClass<FrameTimerClass>(weapon->DisguiseFakeBlinkTime); // RA2: as above
			}
		}
	}
	return bullet; // RA2: as above
}



/***********************************************************************************************
 * UnitClass::Class_Of -- Fetches a reference to the class type for this object.               *
 *                                                                                             *
 *    This routine will fetch a reference to the TypeClass of this object.                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with reference to the type class of this object.                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ObjectTypeClass const * UnitClass::Class_Of(void) const
{
	return(Class);
}


/***********************************************************************************************
 * UnitClass::Tiberium_Load -- Determine the Tiberium load as a percentage.                    *
 *                                                                                             *
 *    Use this routine to determine what the Tiberium load is (as a floating point percentage).*
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the current "fullness" rating for the object.                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/17/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
double UnitClass::Tiberium_Load(void) const
{
	if (!Class->IsToHarvest && !Class->IsToVeinHarvest) { // RA2: RA2's form
		return 0.0; // RA2: as above
	}
	return Storage.Get_Total_Amount() / Class->Capacity; // RA2: as above
}



/// <summary>
/// RA2
/// </summary>
static inline bool Is_Object(AbstractClass * a)
{
	return(a != NULL && (a->AbstractFlags & 2));
}


/// <summary>
/// RA2
/// </summary>
static inline ObjectClass * Object_Of(AbstractClass * a)
{
	if (a == NULL) return(NULL);
	return((a->AbstractFlags & 2) ? (ObjectClass *)a : NULL);
}


/// <summary>
/// RA2: the inline techno cast of Approach_Target; it keeps its own null test.
/// </summary>
static inline TechnoClass * Techno_Of(AbstractClass * a) // RA2: a new helper
{
	if (a == NULL) return(NULL); // RA2: as above
	return((a->AbstractFlags & 1) ? (TechnoClass *)a : NULL); // RA2: as above
}


/***********************************************************************************************
 * UnitClass::Approach_Target -- Handles approaching the target in order to attack it.         *
 *                                                                                             *
 *    This routine will check to see if the target is infantry and it can be overrun. It will  *
 *    try to overrun the infantry rather than attack it. This only applies to computer         *
 *    controlled vehicles. If it isn't the infantry overrun case, then it falls into the       *
 *    base class for normal (complex) approach algorithm.                                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/17/1995 JLB : Created.                                                                 *
 *   07/12/1995 JLB : Flamethrower tanks don't overrun -- their weapon is better.              *
 *=============================================================================================*/
void UnitClass::Approach_Target(void)
{
	/*
	**	Only if there is a legal target should the approach check occur.
	*/
	if (!House->Is_Human_Player() && TarCom != NULL && NavCom == NULL) {

		/*
		**	Special case:
		**	If this is for a unit that can crush infantry, and the target is
		**	infantry, AND the infantry is pretty darn close, then just try
		**	to drive over the infantry instead of firing on it.
		*/
		TechnoClass * target = Techno_Of(TarCom); // RA2: an inline flag-tested techno cast, not Dynamic_Cast
		if ((Class->IsCrusher || Has_Ability(ABILITY_CRUSHER)) && Distance(TarCom) < Rule->CrushDistance && target && ((TechnoTypeClass const *)(target->Class_Of()))->IsCrushable && (Class->IsAutoCrush || !House->Is_Human_Player())) {
			Assign_Destination(TarCom, true); // RA2: the destination is taken at once
			return;
		}
	}

	if (Class->BalloonHover) { // RA2: a hovering balloon type always heads straight for its target
		Assign_Destination(TarCom, true); // RA2: as above
		return; // RA2: as above
	}

	/*
	**	In the other cases, uses the more complex "get to just within weapon range"
	**	algorithm.
	*/
	BASECLASS::Approach_Target();
}



/***********************************************************************************************
 * DriveClass::Overrun_Square -- Handles vehicle overrun of a cell.                            *
 *                                                                                             *
 *    This routine is called when a vehicle enters a square or when it is about to enter a     *
 *    square (controlled by parameter). When a vehicle that can crush infantry enters a        *
 *    cell that contains infantry, then the infantry will be destroyed (regardless of          *
 *    affiliation). When a vehicle threatens to overrun a square, all occupying infantry       *
 *    will attempt to get out of the way.                                                      *
 *                                                                                             *
 * INPUT:   cell     -- The cell that is, or soon will be, entered by a vehicle.               *
 *                                                                                             *
 *          threaten -- Don't kill, but just threaten to enter the cell.                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void UnitClass::Overrun_Square(Cell const & cell, bool threaten)
{
	CellClass * cellptr = &Map[cell];
	bool isbridge = false;

	if (cellptr->IsUnderBridge) {
		isbridge = IsOnBridge || (Map[Center_Coord().As_Cell()].Height == (cellptr->Height + BRIDGE_CELL_HEIGHT));
	}

	if (Class->IsCrusher || Has_Ability(ABILITY_CRUSHER)) {
		if (threaten) {

			/*
			**	If the cell contains infantry, then they will panic when a vehicle tries
			**	drive over them. Have the infantry run away instead.
			*/
			if (isbridge) {

				/*
				**	Scattering is controlled by the game difficulty level.
				*/
				if (cellptr->BridgeFlag.Composite & 0x1F) {
					cellptr->Incoming(COORD_NONE, true, false, true);
				}

			} else {

				/*
				**	Scattering is controlled by the game difficulty level.
				*/
				if (cellptr->Flag.Composite & 0x1F) {
					cellptr->Incoming(COORD_NONE, true, false, false);
				}
			}
		} else {
			ObjectClass * object = cellptr->Cell_Occupier(isbridge);
			int crushed = false;
			while (object != NULL) {
				if (object->Class_Of()->IsCrushable && (!House->Is_Ally(object) || Class->IsTrain) && Relative_Distance(object->Center_Coord()) < 0x4000 && !object->IsFalling) { // RA2: crushable, not an ally's unless a train, close and not falling
					if (object->Fetch_RTTI() == RTTI_INFANTRY && ((InfantryClass *)object)->Class->IsVehicleThief && ((InfantryClass *)object)->NavCom == this && !Class->IsTrain) { // RA2: an infantry thief heading for this unit steals it instead
						ObjectClass * next = object->Next; // RA2: as above
						IsOwnedByPlayer = ((InfantryClass *)object)->IsOwnedByPlayer; // RA2: as above
						Detach_All(false); // RA2: as above
						Captured(((InfantryClass *)object)->House, true); // RA2: as above
						object->Delete_Me(); // RA2: as above
						object = next; // RA2: as above
					} else {
						ObjectClass * next = object->Next; // RA2: the crushed object is killed and removed
						crushed = true; // RA2: as above
						VocClass::PlayAt(object->Class_Of()->CrushSound, PositionCoord); // RA2: as above
						object->Record_The_Kill(this); // RA2: as above
						object->Mark(MARK_UP); // RA2: as above
						object->Limbo(); // RA2: as above
						object->Delete_Me(); // RA2: as above
						object = next; // RA2: as above
					}
				} else {
					object = object->Next; // RA2: as above
				}
			}
			if (crushed) { // RA2: a crush uncloaks the unit
				Do_Uncloak(); // RA2: as above
			}
		}
	}
}



/***********************************************************************************************
 * UnitClass::Assign_Destination -- Assign a destination to a unit.                            *
 *                                                                                             *
 *    This will assign the specified destination to the unit. It is presumed that doing is     *
 *    is all that is needed in order to cause the unit to move to the specified destination.   *
 *                                                                                             *
 * INPUT:   target   -- The target (location) to move to.                                      *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: 5377 bytes differ (adopted text)
void UnitClass::Assign_Destination(AbstractClass * target, bool immediate)
{
	/*
	**	Abort early if there is anything wrong with the parameters
	**	or the unit already is assigned the specified destination.
	*/
	if (Class->BalloonHover && target == NULL && NavCom != NULL && TarCom != NULL) { // RA2: a hovering balloon unit already chasing its target keeps going
		if (NavCom == TarCom) return; // RA2: as above
		if (Unit_Is_Cell(NavCom)) { // RA2: as above
			CellClass * destination = Unit_As_Cell(NavCom); // RA2: as above
			if (destination->Cell_Occupier() == TarCom) return; // RA2: as above
		}
	}
	if (target == NavCom && !UnlimboingInfantry) return; // RA2: the unlimbo flag lets the same destination through once
	UnlimboingInfantry = false; // RA2: as above
	if (Deployed) { // RA2: a deployed unit only stops
		FootClass::Abort_Motion(); // RA2: as above
		return; // RA2: as above
	}

	/*
	**	Transport vehicles must tell all passengers that are about to load, that they
	**	cannot proceed. This is accomplished with a radio message to this effect.
	*/
	if (HasAnyLink() && Class->Max_Passengers() > 0 && Radio_Link(0)->Fetch_RTTI() == RTTI_INFANTRY) { // RA2: as above
		if (target != NULL && (target->AbstractFlags & 1) && !ContainsLink((TechnoClass *)target)) { // RA2: only when the destination is a techno that is not linked
			Transmit_Message(RADIO_OVER_OUT); // RA2: as above
		}
	}

	/*
	 * If tethered to a carryall while sleeping, untether before moving.
	 */
	if (target != NULL && HasAnyLink() && Get_Mission() == MISSION_SLEEP && IsTethered && Radio_Link(0)->Fetch_RTTI() == RTTI_AIRCRAFT) { // RA2: as above
		AircraftClass * a = (AircraftClass *)Radio_Link(0); // RA2: as above
		if (a->Class->IsCarryall) {
			a->Assign_Destination(NULL, true); // RA2: as above
			Transmit_Message(RADIO_UNTETHER);
			Transmit_Message(RADIO_OVER_OUT);
		}
	}

	BuildingClass * b = Unit_As_Building(target); // RA2: the inline RTTI-tested cast, not dynamic_cast
	if (Get_Mission() == MISSION_UNLOAD || MissionQueue == MISSION_UNLOAD) { // RA2: a refinery being unloaded into loses its dump effects
		Cell unload_cell = Get_Cell() + AdjacentCell[6]; // RA2: as above
		BuildingClass * refinery = (BuildingClass *)Map[unload_cell].Cell_Building(); // RA2: as above
		if (refinery != NULL && Class->IsToHarvest) { // RA2: as above
			if (refinery->ParticleSystems[5] != NULL) refinery->ParticleSystems[5]->Delete_Me(); // RA2: as above
			if (refinery->ParticleSystems[6] != NULL) refinery->ParticleSystems[6]->Delete_Me(); // RA2: as above
			if (refinery->Anims[10] != NULL) refinery->End_Anim((BAnimType)10); // RA2: as above
		}
	}

	/*
	**	Handle entry logic here.
	*/
	if (Get_Mission() == MISSION_ENTER || MissionQueue == MISSION_ENTER) { // RA2: as above

		/*
		**	If not already in radio contact (presumed with the transport), then
		**	either try to establish contact if allowed, or just move close and
		**	wait until radio contact can be established.
		*/
		if (!HasAnyLink()) { // RA2: as above
			TechnoClass * destination_techno = Dynamic_Cast<TechnoClass *>(target); // RA2: as above
			if (destination_techno != NULL && destination_techno->Fetch_RTTI() == RTTI_UNIT) { // RA2: a unit destination is queued up behind
				QueueUpToEnter = Unit_As_Unit(target); // RA2: as above
				ArchiveTarget = NULL; // RA2: as above
				target = NULL; // RA2: as above
			} else if (b != NULL && b->Fetch_RTTI() == RTTI_BUILDING) { // RA2: as above

				/*
				**	Determine if the transport is already in radio contact. If so, then just move
				**	toward the transport and try to establish contact at a later time.
				*/
				if (b->HasAnyLink()) { // RA2: as above
					ArchiveTarget = target;

					/*
					**	HACK ALERT: The repair bay is counting on the assignment of the NavCom by this routine.
					**	The refinery must NOT have the navcom assigned by this routine.
					*/
					if (b->Class->IsCanUnitRepair) {
						if (HealthRatio < Rule->ConditionGreen) { // RA2: a damaged unit queues up for the repair bay
							target = NULL; // RA2: as above
							QueueUpToEnter = b; // RA2: as above
						} else {
							Transmit_Message(RADIO_OVER_OUT); // RA2: a healthy one just backs off
							target = NULL; // RA2: as above
						}
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
				TechnoClass * techno = (target != NULL && (target->AbstractFlags & 1)) ? (TechnoClass *)target : NULL; // RA2: the inline flag-tested techno cast
				if (techno != NULL) {

					/*
					**	Determine if the transport is already in radio contact. If so, then just move
					**	toward the transport and try to establish contact at a later time.
					*/
					if (techno->HasAnyLink()) { // RA2: as above
						ArchiveTarget = target;
					} else {
						if (Transmit_Message(RADIO_HELLO, techno) == RADIO_ROGER) {
							if (Transmit_Message(RADIO_DOCKING) != RADIO_ROGER) { // RA2: the handshake continues after a successful docking message
								Transmit_Message(RADIO_OVER_OUT); // RA2: as above
							}
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
				Cell nearby = Map.Nearby_Location(tcell->CellID, SPEED_TRACK, Map.Get_Cell_Zone(tcell->CellID, Class->MZone, false), Class->MZone, false, Point2D(1, 1), false, true, false, false);
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

			if (isbridge && isunderbridgeto) { // RA2: one condition
				int z1 = Map.Zone_Connection_Index(from, 3, 0); // RA2: as above
				if (z1 == Map.Zone_Connection_Index(to, 3, 0) && z1 != -1) { // RA2: as above
					if (!Map.ZoneConnections[z1].IsPassable) { // RA2: as above
						Cell d1 = Map.Get_Zone_Connection_Destination(from, from); // RA2: as above
						Cell d2 = Map.Get_Zone_Connection_Destination(to, to); // RA2: as above
						if ((d1 - d2).Length() <= 2) { // RA2: the cell difference is measured inline
							needsswap = true;
						}
					} else {
						needsswap = true; // RA2: as above
					}
				}
			}
			if ((isbridge || isunderbridgeto) && !needsswap) { // RA2: the plan decision is two separate tests
				useplan = true; // RA2: as above
			}

			if (((!canburrowfrom && height >= 0) || !canburrowto) && !needsswap) { // RA2: as above
				useplan = true; // RA2: as above
			}
		}

		if (useplan) { // RA2: the plan is made here, the immediate branch is separate
			target = Plan_Route(*target); // RA2: as above
			surface = true; // RA2: as above
		} else if (immediate) { // RA2: as above
			RouteQueue.Clear(); // RA2: as above
			surface = true; // RA2: as above
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

	} else {
		RouteQueue.Clear(); // RA2: as above
	}

	if (Class->Teleporter && !IsImmobilized) { // RA2: a teleporting unit may need a drive locomotor to enter a dock
		TechnoClass * contact = Radio_Link(0); // RA2: as above
		IPersistPtr persist(Locomotion); // RA2: as above
		CLSID tclsid; // RA2: as above
		persist->GetClassID(&tclsid); // RA2: as above
		bool drive = true; // RA2: as above
		if (contact != NULL) { // RA2: as above
			if (contact->Fetch_RTTI() == RTTI_BUILDING) { // RA2: as above
				BuildingClass * building = Unit_As_Building(contact); // RA2: as above
				CellClass * cell = Unit_As_Cell(target); // RA2: as above
				if (building->Class->IsDockUnload && cell != NULL && cell->Cell_Unit(false) == NULL) { // RA2: as above
					drive = false; // RA2: as above
					if (!(tclsid == CLSID_TeleportLocomotion)) { // RA2: as above // RA2: as above
						IPiggybackPtr piggy(Locomotion); // RA2: as above
						if (piggy != NULL && piggy->Is_Piggybacking() && piggy->Is_Ok_To_End()) { // RA2: as above
							piggy->End_Piggyback(&Locomotion); // RA2: as above
						} else {
							Locomotion->Stop_Moving(); // RA2: as above
							Set_Mission(MISSION_NONE); // RA2: as above
							QueueMission(MISSION_ENTER, false); // RA2: as above
							IsMoveToSkipped = true; // RA2: as above
							UnlimboingInfantry = true; // RA2: as above
						}
					}
				}
			}
		}
		if ((contact == NULL || drive == true) && !(tclsid == CLSID_DriveLocomotion)) { // RA2: as above
			IPiggybackPtr piggy(Locomotion); // RA2: as above
			if (piggy != NULL && piggy->Is_Piggybacking() && piggy->Is_Ok_To_End()) { // RA2: as above
				piggy->End_Piggyback(&Locomotion); // RA2: as above
			}
			ILocomotionPtr walk(CLSID_DriveLocomotion); // RA2: as above
			walk->Link_To_Object(this); // RA2: as above
			piggy = IPiggybackPtr(walk); // RA2: as above
			if (piggy != NULL) { // RA2: as above
				piggy->Begin_Piggyback(Locomotion); // RA2: as above
				Locomotion = walk; // RA2: as above
			}
		}
	}

	/*
	 * If the locomotor is a tunnel locomotor and the unit is on the ground,
	 * surface the tunneler by swapping the tunnel locomotor for a drive one.
	 * (Mirrors BuildingClass weapons-factory exit, building.cpp:6236-6251.)
	 */
	if (target != NULL && !Locomotion->Is_Moving()) { // RA2: as above
		IPersistPtr persist(Locomotion); // RA2: as above
		CLSID clsid; // RA2: as above
		persist->GetClassID(&clsid); // RA2: as above
		if (clsid == CLSID_TunnelLocomotion && Get_Height_AGL() == 0) { // RA2: as above
			Coord tc = target->Center_Coord(); // RA2: as above
			int gl = Map.Get_Height_GL(tc); // RA2: as above
			if (tc.Z < gl) tc.Z = gl; // RA2: as above

			bool doswap = needsswap; // RA2: as above
			if (!doswap) { // RA2: as above
				if (!Is_Moving_Onto_Bridge() // RA2: as above
					&& !Map[target->Center_Coord()].IsUnderBridge // RA2: as above
					&& Map[Destination_Coord()].Can_Burrow_Here() // RA2: as above
					&& Map[target->Center_Coord()].Can_Burrow_Here()) { // RA2: as above
					Cell to = target->Center_Coord().As_Cell(); // RA2: as above
					if (!Is_Route_Broken(Get_Target_Cell(), to)) { // RA2: as above
						doswap = true; // RA2: as above
					}
				} else {
					doswap = true; // RA2: as above
				}
			}

			if (doswap) { // RA2: as above
				IPiggybackPtr piggy(Locomotion); // RA2: as above
				if (piggy != NULL && piggy->Is_Piggybacking()) { // RA2: as above
					piggy->End_Piggyback(&Locomotion); // RA2: as above
				}
				ILocomotionPtr walk(CLSID_DriveLocomotion); // RA2: as above
				walk->Link_To_Object(this); // RA2: as above
				{
					IPiggybackPtr walk_piggy(walk); // RA2: the drive locomotor's piggyback interface is a named local
					piggy = (IPiggyback *)walk_piggy; // RA2: as above
				}
				if (piggy != NULL) { // RA2: as above
					piggy->Begin_Piggyback(Locomotion); // RA2: as above
					Locomotion = walk; // RA2: as above
					Locomotion->Force_New_Slope(Map[Get_Coord()].Ramp); // RA2: as above
				}
			}
		}
	}

	/*
	**	If the player clicked on a friendly repair facility and the repair
	**	facility is currently not involved with some other unit (radio or unloading).
	*/
	if (b != NULL && b->Class->IsCanUnitRepair) {
		if (b->HasAnyLink() && b->Radio_Link(0) != this) { // RA2: as above
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
	BuildingClass * wf = (BuildingClass *)Radio_Link(0); // RA2: as above
	if (wf != NULL && wf->Fetch_RTTI() == RTTI_BUILDING && wf->Class->IsWeaponsFactory && CurrentMission != MISSION_ENTER) { // RA2: not while entering
		Cell exitcell = wf->Get_Coord().As_Cell() + Cell(5, 1); // RA2: RA2's exit cell is (5, 1)
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
		Fetch_RTTI() == RTTI_UNIT && (Class->IsToHarvest || Class->IsToVeinHarvest) &&
		Get_Mission() != MISSION_UNLOAD) { // RA2: no ally test

		if (Radio_Link(0) != b && !b->HasAnyLink() && Transmit_Message(RADIO_HELLO, b) == RADIO_ROGER && // RA2: as above
			Get_Mission() != MISSION_ENTER && Get_Mission() != MISSION_HARVEST) { // RA2: as above
			QueueMission(MISSION_ENTER, false); // RA2: QueueMission replaces Assign_Mission
			target = NULL;
		} else {
			if (target != NULL && target->Fetch_RTTI() == RTTI_BUILDING && Radio_Link(0) != target) { // RA2: as above
				Cell nearby = Map.Nearby_Location(b->Get_Coord().As_Cell(), SPEED_WHEEL, Map.Get_Cell_Zone(b->Get_Coord().As_Cell(), Class->MZone, false), Class->MZone, false, Point2D(1, 1), false, true, false, false);
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
	if (!Locomotion->Is_Powered() && !House->Ion_Sensitives_Should_Be_Offline()) { // RA2: the house's ion test
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
#endif



/***********************************************************************************************
 * UnitClass::Greatest_Threat -- Fetches the greatest threat for this unit.                    *
 *                                                                                             *
 *    This routine will search the map looking for a good target to attack. It takes into      *
 *    consideration the type of weapon it is equipped with.                                    *
 *                                                                                             *
 * INPUT:   threat   -- The threat type to search for.                                         *
 *                                                                                             *
 * OUTPUT:  Returns with a target value of the target that this unit should pursue. If there   *
 *          is no suitable target, then TARGET_NONE is returned.                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
AbstractClass * UnitClass::Greatest_Threat(ThreatType threat, Coord const & coord, bool onlyenemy) const
{
	if (House->Is_Human_Player() && Class->IsDeployToFire) {
		return(0); // RA2: RA2's form
	}
	if (!(threat & 0x1B978)) { // RA2: as above
		if (const_cast<UnitClass *>(this)->Has_Turret_Weapons()) { // RA2: as above
			threat = ThreatType(threat | const_cast<UnitClass *>(this)->Get_Class_Weapon_Data(const_cast<UnitClass *>(this)->Get_Current_Weapon_Number())->Weapon->Allowed_Threats()); // RA2: as above
		} else { // RA2: as above
			if (const_cast<UnitClass *>(this)->Get_Class_Weapon_Data(0)->Weapon) { // RA2: as above
				threat = ThreatType(threat | const_cast<UnitClass *>(this)->Get_Class_Weapon_Data(0)->Weapon->Allowed_Threats()); // RA2: as above
			}
			if (const_cast<UnitClass *>(this)->Get_Class_Weapon_Data(1)->Weapon) { // RA2: as above
				threat = ThreatType(threat | const_cast<UnitClass *>(this)->Get_Class_Weapon_Data(1)->Weapon->Allowed_Threats()); // RA2: as above
			}
		}
	}
	return FootClass::Greatest_Threat(threat, coord, onlyenemy); // RA2: as above
}



/***********************************************************************************************
 * UnitClass::Read_INI -- Reads units from scenario INI file.                                  *
 *                                                                                             *
 *    This routine is used to read all the starting units from the                             *
 *    scenario control INI file. The units are created and placed on the                       *
 *    map by this routine.                                                                     *
 *                                                                                             *
 *    INI entry format:                                                                        *
 *      Housename, Typename, Strength, Coord, Facingnum, Missionname, Triggername              *
 *                                                                                             *
 * INPUT:   buffer   -- Pointer to the loaded scenario INI file.                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: 15 bytes differ (adopted text)
void UnitClass::Read_INI(CCINIClass const & ini)
{
	UnitClass	* unit;         // Working unit pointer.
	UnitType		classid;    // Unit class.
	char			buf[128];
	DynamicVectorClass<int> followers;
	followers.Clear(); // RA2: the follower list is cleared right after it is built

	int len = ini.Entry_Count(INI_NAME);

	for (int index = 0; index < len; index++) {
		char const * entry = ini.Get_Entry(INI_NAME, index);

		ini.Get_String(INI_NAME, entry, NULL, buf, sizeof(buf));

		int house_index = House_Index_From_Name(strtok(buf, ",")); // RA2: the house is found by index in Houses
		if (house_index != -1) { // RA2: as above
			classid = UnitTypeClass::From_Name(strtok(NULL, ","));
			HouseClass * inhousep = Houses[house_index]; // RA2: as above

			if (classid != UNIT_NONE) {

				unit = new UnitClass(UnitTypes[classid], inhousep);
				if (unit != NULL) {

					/*
					**	Read the raw data.
					*/
					int strength = atoi(strtok(NULL, ","));

					Cell cell;
					Coord coord;
					if (NewINIFormat >= 4) {
						unsigned short x = atoi(strtok(NULL, ","));
						unsigned short y = atoi(strtok(NULL, ","));
						cell = Cell(x,y);
					} else {
						int c = atoi(strtok(NULL, ","));
						cell = Cell(c % 128, c / 128); // RA2: the old packed format is split by 128 columns
					}

					coord = cell;

					Dir256 dir = (Dir256)atoi(strtok(NULL, ","));
					MissionType mission = MissionClass::Mission_From_Name(strtok(NULL, ","));

					TagTypeClass * tp = TagTypeClass::From_Name(strtok(NULL,","));
					if (tp != NULL) {
						TagClass * tt = Find_Or_Make(tp);
						if (tt != NULL) {
							unit->Attach_Tag(tt);
						}
					}

					char * token = strtok(NULL, ",");
					if (token != NULL) {
						unit->Veterancy.From_Integer(atoi(token));
					}

					token = strtok(NULL, ",");
					if (token != NULL) {
						unit->Group = atoi(token);
					}

					token = strtok(NULL, ",");
					if (token != NULL) {
						unit->IsOnBridge = atoi(token) != 0;
						if (unit->IsOnBridge) {
							coord.Z = Map.Get_Height_GL(coord) + BRIDGE_LEPTON_HEIGHT;
						}
					}

					token = strtok(NULL, ",");
					if (token != NULL) {
						followers.Add(atoi(token));
					}

					token = strtok(NULL, ",");
					if (token != NULL) {
						unit->IsTeamRecruitable = atoi(token) != 0;
					}

					token = strtok(NULL, ",");
					if (token != NULL) {
						unit->IsAutocreateRecruitable = atoi(token) != 0;
					}

					if (unit->Unlimbo(coord, dir)) {
						UnitTypeClass const * type = unit->Class; // RA2: the type is read once
						unit->Strength = (int)((double)strength * type->MaxStrength * (1.0 / 256.0)); // RA2: the health byte scaled by the maximum, multiplied as integers first
						if (unit->Strength > type->MaxStrength-3) unit->Strength = type->MaxStrength; // RA2: as above
						if (unit->Strength == 0) unit->Strength = 1;
						unit->EstimatedHealth = unit->Strength; // RA2: the estimated health starts equal to the real one
						if (Session.Type == GAME_NORMAL || unit->House->Is_Human_Player()) {
							unit->QueueMission(mission, false); // RA2: queued, not assigned
							if (unit->Ready_To_Commence()) {
								unit->Commence();
							}
						} else {
							unit->Enter_Idle_Mode(false, true); // RA2: resuming the waypoint
						}

					} else {

						/*
						**	If the unit could not be unlimboed, then this is a catastrophic error
						**	condition. Delete the unit.
						*/
						delete unit;
					}
				}
			}
		}
	}

	for (int i = 0; i < Units.Count(); i++) {
		int followerid = followers[i];
		UnitClass * unit = Units[i];
		if ((UnitType)followerid != UNIT_NONE && (unsigned)followerid < (unsigned)Units.Count()) {
			UnitClass * follower = Units[followerid];
			unit->FollowingMe = follower;
			follower->IsFollowing = true;
		} else {
			unit->FollowingMe = NULL;
		}
	}
}
#endif



/***********************************************************************************************
 * UnitClass::Write_INI -- Store the units to the INI database.                                *
 *                                                                                             *
 *    This routine will store all the unit data to the INI database.                           *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database object to store to.                         *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void UnitClass::Write_INI(CCINIClass & ini)
{
	ini.Clear(INI_NAME);

	for (int index = 0; index < Units.Count(); index++) {
		UnitClass * unit = Units[index];
		if (unit != NULL && !unit->IsInLimbo && unit->IsActive) {
			char uname[10]; // RA2: Write_INI at 0x706F60 matches only so
			char buf[128]; // RA2: as above

			sprintf(uname, "%d", index);
			sprintf(buf, "%s,%s,%d,%d,%d,%d,%s,%s,%d,%d,%d,%d,%d,%d",
				(char const *)unit->House->IniName, // RA2: as above
				(char const *)unit->Class->IniName,
				(int)(unit->HealthRatio * 256), // RA2: as above
				unit->PositionCell.X,
				unit->PositionCell.Y,
				unit->PrimaryFacing.Current().As_Dir256(),
				MissionClass::Mission_Name(unit->Get_Mission()), // RA2: as above
				(unit->Tag != NULL && unit->Tag->Class != NULL) ? (char const *)unit->Tag->Class->IniName : "None",
				unit->Veterancy.To_Integer(),
				unit->Group,
				unit->IsOnBridge,
				(unit->FollowingMe != NULL) ? Units.ID(unit->FollowingMe) : -1,
				unit->IsTeamRecruitable,
				unit->IsAutocreateRecruitable
				);
			ini.Put_String(INI_NAME, uname, buf);
		}
	}
}


/***********************************************************************************************
 * UnitClass::Credit_Load -- Fetch the full credit value of cargo carried.                     *
 *                                                                                             *
 *    This will determine the value of the cargo carried (limited to considering only gold     *
 *    and gems) and return that value. Use this to determine how 'valuable' a harvester is.    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the credit value of the cargo load of this unit (harvester).          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int UnitClass::Credit_Load(void) const
{
	float value = (float)Storage.Get_Total_Value(); // RA2: the stored total is converted to float first
	return (int)(House->Income_Multiplier() * value); // RA2: scaled by the owner's income multiplier (0x4F5B30)
}



/***********************************************************************************************
 * UnitClass::Should_Crush_It -- Determines if this unit should crush an object.               *
 *                                                                                             *
 *    Call this routine to determine if this unit should crush the object specified. The       *
 *    test for crushable action depends on proximity and ability of the unit. If a unit        *
 *    should crush the object, then it should be given a movement order to enter the cell      *
 *    where the object is located.                                                             *
 *                                                                                             *
 * INPUT:   it -- The object to see if it should be crushed.                                   *
 *                                                                                             *
 * OUTPUT:  bool; Should "it" be crushed by this unit?                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool UnitClass::Should_Crush_It(TechnoClass const * it) const
{
	/*
	**	If this unit cannot crush anything or the candidate object cannot be crushed,
	**	then it obviously should not try to crush it -- return negative answer.
	*/
	if (!Class->IsCrusher && !Has_Ability(ABILITY_CRUSHER)) return(false); // RA2: a crusher by type or by ability only
	if (it == NULL || !it->Techno_Type_Class()->IsCrushable) return(false); // RA2: as above, the candidate must be crushable

	/*
	**	Objects that are far away should really be fired upon rather than crushed.
	*/
	int distance = Center_Coord().Distance_YZX(it->Center_Coord()); // RA2: the distance sums y, z, x
	if (distance > Rule->CrushDistance) return(false); // RA2: as above

	/*
	**	Human controlled units don't automatically crush. Neither do computer controlled ones
	**	if they are at difficult setting.
	*/
	if ((House->Is_Human_Player() && !Rule->IsAutoCrush && !Class->IsAutoCrush) || House->Difficulty == DIFF_HARD) return(false);

	/*
	**	If the house IQ indicates that crushing should not be allowed, then don't
	**	suggest that crushing be done.
	*/
	if (!House->Is_Human_Player() && House->IQ < Rule->IQCrush) return(false);

	/*
	**	Don't allow crushing of spies by computer-controlled vehicles.
	*/
	return(it->IsDisguisedAs(House) ? false : true); // RA2: a target disguised as this house's unit is left alone
}



/***********************************************************************************************
 * UnitClass::Scatter -- Causes the unit to scatter to a nearby location.                      *
 *                                                                                             *
 *    This scatter logic will actually look for a nearby location rather than an adjacent      *
 *    free location. This is necessary because sometimes a unit is required to scatter more    *
 *    than one cell. A vehicle on a service depot is a prime example.                          *
 *                                                                                             *
 * INPUT:   threat   -- The coordinate that a potential threat resides. If this is a non       *
 *                      threat related scatter, then this parameter will be zero.              *
 *                                                                                             *
 *          forced   -- Should the scatter be performed even if it would be otherwise          *
 *                      inconvenient?                                                          *
 *                                                                                             *
 *          nokidding-- Should the scatter be performed even if it would otherwise be          *
 *                      illegal?                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/02/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: 1528 bytes differ (adopted text)
void UnitClass::Scatter(Coord const & threat, bool forced, bool nokidding)
{
	if (!Can_Scatter()) return;

	/*
	**	Certain missions prevent scattering regardless of whether it would be
	**	a good idea or not.
	*/
	if (!Current_Mission_Control().IsScatter && !forced) return;

	if (PrimaryFacing.Is_Rotating()) return;

	if (NavCom != NULL && !nokidding) return;

	if (threat == COORD_NONE) {
		Cell nearby = Map.Nearby_Location(Destination_Coord().As_Cell(), Class->Speed, -1, MZONE_NORMAL, IsOnBridge, Point2D(1,1), false, true);
		if (nearby != CELL_NONE) {
			Assign_Destination(&Map[nearby]);
		}
		return;
	}

	/*
	**	Certain missions prevent scattering regardless of whether it would be
	**	a good idea or not.
	*/
	if (Current_Mission_Control().IsParalyzed) return;

	if ((Fetch_RTTI() != RTTI_UNIT || !((UnitClass *)this)->IsDumping) && (NavCom == NULL || (nokidding && !IsRotating))) {
		if (TarCom == NULL || forced || Random_Pick(1, 4) == 1) {
			FacingType	toface;

			if (threat != COORD_NONE) {
				toface = Dir_Facing(::Direction(threat, PositionCoord));
				toface = Facing_Add(toface, Random_Pick(FACING_0, FACING_90)-FACING_45);
			} else {
				toface = (PrimaryFacing.Current().As_Dir8());
				toface = Facing_Add(toface, Random_Pick(FACING_0, FACING_90)-FACING_45);
			}

			Cell altcell(0, 0);
			Cell newcell(0, 0);
			Cell destcell(Destination_Coord());

			CellClass *cellptr = &Map[destcell];

			int z = cellptr->Height + Is_Moving_Onto_Bridge() ? BRIDGE_CELL_HEIGHT : 0;
			Coord destcoord = Destination_Coord();
			destcoord.Z = z * LEVEL_LEPTON_H;

			FacingType face;
			for (face = FACING_N; face < FACING_COUNT; face++) {
				FacingType newface = Facing_Add(toface, face);
				Cell checkcell = Adjacent_Cell(destcell, newface);
				CellClass *cptr = &Map[checkcell];

				if (Map.In_Local_Radar(checkcell) && Can_Enter_Cell(cptr, newface, Get_Cell_Height()) == MOVE_OK) {
					if (altcell == CELL_NONE) altcell = checkcell;
					if (CELL_NONE == Cell(0,0)) {
						Coord checkcoord = checkcell.As_Coord();
						checkcoord.Z = destcoord.Z;
						if (checkcell == TacticalMap->Coord_To_Cell(checkcoord) && !Map[checkcell].IsUnderBridge) {
							newcell = checkcell;
							break;
						}
					}
				}
			}

			Cell destination = newcell;
			if (destination == CELL_NONE) {
				destination = altcell;
			}

			if (destination != CELL_NONE) {
				Assign_Mission(MISSION_MOVE);
				Assign_Destination(&Map[destination]);
			}
		}
	}
}
#endif


/***********************************************************************************************
 * UnitClass::Limbo -- Limbo this unit.                                                        *
 *                                                                                             *
 *    This will cause the unit to go into a limbo state. If it was carrying a flag, then       *
 *    the flag will be dropped where the unit is at.                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was this unit limboed?                                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool UnitClass::Limbo(void)
{
	if (BASECLASS::Limbo()) {
		if (Flagged != HOUSE_NONE) {
			Houses[Flagged]->Flag_Attach(Get_Cell(), false); // RA2: Limbo at 0x7078F0 matches only so
			Flagged = HOUSE_NONE;
		}
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * UnitClass::Mission_Guard_Area -- Guard area logic for units.                                *
 *                                                                                             *
 *    This logic is similar to normal guard area except that APCs owned by the computer will   *
 *    try to load up with nearby infantry. This will give the computer some fake intelligence  *
 *    when playing in skirmish mode.                                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the delay to use before calling this routine again.                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int UnitClass::Do_MISSION_GUARD_AREA(void)
{
	return(BASECLASS::Do_MISSION_GUARD_AREA());
}


/// <summary>
/// Is this unit free to start moving?
/// A transport must have its door settled shut before it may drive off; one that is open
/// or still animating holds the vehicle where it is.
/// </summary>
/// <returns>bool; May the unit begin moving?</returns>
bool UnitClass::Is_Ready_To_Move(void) const
{
	if (Door.Is_Ready_To_Open()) {
		return(true);
	}

	return(false);
}


/// <summary>
/// Marks the cell at the coordinate specified as occupied by this unit.
/// A vehicle riding a bridge claims the bridge deck rather than the ground below it.
/// </summary>
/// <param name="coord">The coordinate of the cell to occupy.</param>
void UnitClass::Set_Occupy_Bit(Coord const & coord)
{
	CellClass &cell = Map[coord];
	if (Map.Get_Height_GL(coord) + BRIDGE_LEPTON_HEIGHT <= coord.Z && cell.IsUnderBridge) {
		cell.BridgeFlag.Occupy.Vehicle = true;
	} else {
		cell.Flag.Occupy.Vehicle = true;
	}
}


/// <summary>
/// Releases this unit's claim on the cell at the coordinate specified.
/// A vehicle riding a bridge gives up the bridge deck rather than the ground below it.
/// </summary>
/// <param name="coord">The coordinate of the cell to release.</param>
void UnitClass::Clear_Occupy_Bit(Coord const & coord)
{
	CellClass &cell = Map[coord];
	if (Map.Get_Height_GL(coord) + BRIDGE_LEPTON_HEIGHT <= coord.Z) { // RA2: Clear_Occupy_Bit at 0x7079E0 matches only so
		cell.BridgeFlag.Occupy.Vehicle = false;
	} else {
		cell.Flag.Occupy.Vehicle = false;
	}
}


/// <summary>
/// RA2
/// </summary>
static inline bool Is_Cell_Below(Cell const & a, Cell const & b)
{
	Cell diff = a - b;
	return(diff.X == 0 && diff.Y == 1);
}


/// <summary>
/// May this unit be handed its next order now?
/// The mission system consults this routine before starting a queued mission. A vehicle
/// that is still rolling, still unloading, or still being disgorged by a war factory is
/// left to finish what it is doing.
/// </summary>
/// <returns>bool; Is the unit ready to begin its next mission?</returns>
bool UnitClass::Ready_To_Commence(void)
{
	if (CurrentMission == MISSION_STICKY || CurrentMission == MISSION_RESCUE || IsDumping) { // RA2: RA2's form
		return(false);
	}
	if (MissionQueue != MISSION_ENTER && Locomotion->Is_Moving_Now() && HeightAGL >= 0 // RA2: as above
		&& Get_Mission() != MISSION_GUARD && (Get_Mission() != MISSION_ATTACK || TarCom != NULL) && !IsMissionUnloadStandby) { // RA2: as above
		return(false);
	}
	if (!Door.Is_Door_Closed()) {
		return(false);
	}
	TechnoClass * contact = ((RadioClass const *)this)->Radio_Link(0); // RA2: the const Radio_Link (0x636E20)
	if (contact != NULL) { // RA2: as above
		if (contact->Fetch_RTTI() == RTTI_BUILDING && ((BuildingClass *)contact)->Class->IsWeaponsFactory // RA2: as above
			&& MissionQueue != MISSION_MOVE && MissionQueue != MISSION_ENTER) { // RA2: as above
			return(false);
		}
	} else {
		BuildingClass * building = Map[Get_Coord()].Cell_Building();
		if (building != NULL && building->Class->IsWeaponsFactory // RA2: as above
			&& (PositionCoord.As_Cell() - building->PositionCoord.As_Cell() == Cell(0, 1))) { // RA2: as above
			return(false);
		}
	}
	return(true);
}






/// <summary>
/// Reads this unit back in from a save game stream.
/// This routine restores the unit's entry in the target tracker and brings the members of
/// an old format save game up to the layout the game expects today.
/// </summary>
/// <param name="stream">The stream to read this unit from.</param>
/// <returns>Returns with S_OK if the unit was read successfully.</returns>
HRESULT STDMETHODCALLTYPE UnitClass::Load(IStream * stream) // RA2: Load at 0x707C20 matches only so
{
	TargetTracker.Remove_Index(Fetch_ID()); // RA2: Load at 0x707C20 has no int size = Fetch_Object_Size(false);
	HRESULT result = BASECLASS::Load(stream);
	if (SUCCEEDED(result)) {
		ILocomotion * locomotion = Locomotion;

		new (this) UnitClass(NoInitClass()); // RA2: Load at 0x707C20 has no IsOldSaveGame, Unused1, Charge and 2 more

		Locomotion = locomotion;
		locomotion->Release();

		TargetTracker.Add_Index(Fetch_ID(), this);

		Swizzle_Pointer(&Class);
		Swizzle_Pointer(&FollowingMe);

		ElectricBolt = NULL; // RA2: as above
		result = S_OK;
	}
	return(result);
}


/// <summary>
/// Writes this unit out to the save game stream.
/// </summary>
/// <param name="stream">The stream to write this unit to.</param>
/// <param name="cleardirty">Should the unit be marked as no longer needing a save?</param>
/// <returns>Returns with S_OK if the unit was written successfully.</returns>
HRESULT STDMETHODCALLTYPE UnitClass::Save(IStream * stream, BOOL cleardirty)
{
	if (ElectricBolt) { // RA2: a live electric bolt is destroyed before the save (0x4B25C0 ~EBolt)
		ElectricBolt->~EBolt(); // RA2: as above
		ElectricBolt = 0; // RA2: as above
	}
	return(BASECLASS::Save(stream, cleardirty));
}



/// <summary>
/// Adds this unit's state to the running game state checksum.
/// The network code compares these checksums between machines in order to detect a game
/// that has fallen out of sync.
/// </summary>
/// <param name="crc">The checksum engine to submit this unit's state to.</param>
void UnitClass::Compute_CRC(CRCEngine & crc) const
{
	BASECLASS::Compute_CRC(crc);

	crc(Class->Fetch_RTTI()); // RA2: Compute_CRC at 0x707DF0 has no Reload
	crc(Class->Fetch_ID());
	if (FollowingMe != NULL) {
		crc(FollowingMe->Fetch_ID());
	}
	crc(Flagged);
	crc(IsFollowing);
	crc(IsDumping);
	crc(IsHarvesting);
	crc(IsToScatter);
}


/// <summary>
/// Removes all references this unit holds to the object specified.
/// This routine is called when an object is about to disappear, so that no dangling
/// pointer to it survives anywhere in the game.
/// </summary>
/// <param name="target">The object that is going away.</param>
/// <param name="all">Should even the passive references be severed?</param>
void UnitClass::Detach(AbstractClass const * target, bool all)
{
	BASECLASS::Detach(target, all);
	if (FollowingMe == target) {
		FollowingMe = NULL;
	}
	if (Class == target) {
		Class = NULL;
	}
}


/// <summary>
/// Records the destruction of this unit.
/// This routine springs whatever destruction events are hanging off the unit's tag before
/// handing the kill on for the usual scoring bookkeeping.
/// </summary>
/// <param name="source">The object responsible for the kill, or NULL if there was none.</param>
void UnitClass::Record_The_Kill(TechnoClass * source)
{
	if (Tag != NULL) {
		if (EnteredByInfType == INFANTRY_NONE || !Tag->Is_To_Inherit()) {
			if (source != NULL) {
				Tag->Spring(TEVENT_DESTROYED, this);
			}
			if (Tag != NULL) {
				Tag->Spring(TEVENT_DESTROYED_ANY, this);
			}
			if (Tag != NULL) {
				Tag->Spring(TEVENT_DESTROYED_ANY_X, this);
			}
		}
	}
	BASECLASS::Record_The_Kill(source);
}





/***********************************************************************************************
 * UnitClass::Mission_Attack -- AI for heading towards and firing upon target.                 *
 *                                                                                             *
 *    This AI routine handles heading to within range of the target and then firing upon       *
 *    it until it is destroyed. If the target is destroyed, then the unit will change          *
 *    missions to match its "idle mode" of operation (usually guarding).                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the delay before calling this routine again.                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
int UnitClass::Do_MISSION_ATTACK(void)
{
	return(BASECLASS::Do_MISSION_ATTACK());
}


/// <summary>
/// Plans a route from the unit's destination cell to the given target. When the journey
/// crosses movement zones (under-bridge destinations, burrowable start or end cells,
/// bridge approaches), the intermediate travel legs (bridge entry/exit cells, zone
/// connection cells, nearest reachable cells) are queued in RouteQueue ahead of the
/// target itself. The first leg is then popped and returned as the immediate destination.
/// </summary>
/// <param name="object">The desired final movement target.</param>
/// <returns>The first leg of the planned route, or NULL if no route is possible.</returns>
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
AbstractClass * UnitClass::Plan_Route(AbstractClass const & object) const
{
	DynamicVectorClass<AbstractClass *> & route = ((UnitClass *)this)->RouteQueue;
	route.Clear();

	bool onbridge = Is_Moving_Onto_Bridge();

	Cell from = Destination_Coord().As_Cell();

	bool canburrowfrom = Map[from].Can_Burrow_Here() || Get_Height_AGL() < 0;

	AbstractClass * dest = (AbstractClass *)&object;
	Cell to = dest->Center_Coord().As_Cell();

	CellClass * toptr = &Map[to];
	bool isunderbridgeto = toptr->IsUnderBridge;
	bool canburrowto = toptr->Can_Burrow_Here();

	if (onbridge) {
		if (isunderbridgeto) {
			Cell frombridge = Map.Get_Zone_Connection_Destination(from, to);
			Cell tobridge = Map.Get_Zone_Connection_Destination(to, to);

			if (Is_Route_Broken(frombridge, tobridge)) {
				Cell fromspot = Map.Nearby_Location(frombridge, SPEED_TRACK, Map.Get_Cell_Zone(frombridge), MZONE_NORMAL, false, Point2D(1, 1), false, true, true);
				Cell tospot = Map.Nearby_Location(tobridge, SPEED_TRACK, Map.Get_Cell_Zone(frombridge), MZONE_NORMAL, false, Point2D(1, 1), false, true, true);

				if (fromspot != CELL_NONE && tospot != CELL_NONE) {
					route.Add(&Map[fromspot]);
					route.Add(&Map[tospot]);

				} else if (Map.Is_Same_Cell_Zone(from, to, MZONE_NORMAL, true, true, false)) {
					if (frombridge != from) {
						route.Add(&Map[frombridge]);
					}
					route.Add(&Map[tobridge]);

				} else if (fromspot != CELL_NONE) {
					tospot = Map.Nearby_Location(to, SPEED_TRACK, -1, MZONE_NORMAL, true, Point2D(1, 1), false, false, true);
					if (tospot != CELL_NONE && fromspot != tospot) {
						route.Add(&Map[fromspot]);
						dest = &Map[tospot];
					} else {
						Cell fallback = Map.Nearby_Location(to, SPEED_TRACK, Map.Get_Cell_Zone(from, MZONE_NORMAL, true), MZONE_NORMAL, true, Point2D(1, 1), false, true, false);
						if (fallback == CELL_NONE || Get_Height_AGL() < 0) {
							return(NULL);
						}
						dest = &Map[fallback];
					}

				} else {
					Cell fallback = Map.Nearby_Location(to, SPEED_TRACK, Map.Get_Cell_Zone(from, MZONE_NORMAL, true), MZONE_NORMAL, true, Point2D(1, 1), false, true, false);
					if (fallback == CELL_NONE || Get_Height_AGL() < 0) {
						return(NULL);
					}
					dest = &Map[fallback];
				}

			} else {
				if (frombridge != from) {
					route.Add(&Map[frombridge]);
				}
				route.Add(&Map[tobridge]);
			}

		} else if (canburrowto) {
			Cell frombridge = Map.Get_Zone_Connection_Destination(from, to);

			if (Is_Route_Broken(frombridge, to)) {
				Cell fromspot = Map.Nearby_Location(frombridge, SPEED_TRACK, Map.Get_Cell_Zone(frombridge), MZONE_NORMAL, false, Point2D(1, 1), false, true, true);

				if (fromspot != CELL_NONE) {
					route.Add(&Map[fromspot]);

				} else if (Map.Is_Same_Cell_Zone(from, to, MZONE_NORMAL, true, false, false)) {
					if (frombridge != from) {
						route.Add(&Map[frombridge]);
					}

				} else {
					Cell fallback = Map.Nearby_Location(to, SPEED_TRACK, Map.Get_Cell_Zone(from, MZONE_NORMAL, true), MZONE_NORMAL, true, Point2D(1, 1), false, true, false);
					if (fallback == CELL_NONE || Get_Height_AGL() < 0) {
						return(NULL);
					}
					dest = &Map[fallback];
				}

			} else if (frombridge != from) {
				route.Add(&Map[frombridge]);
			}

		} else {
			Cell frombridge = Map.Get_Zone_Connection_Destination(from, to);

			if (Is_Route_Broken(frombridge, to)) {
				Cell fromspot = Map.Nearby_Location(frombridge, SPEED_TRACK, Map.Get_Cell_Zone(frombridge), MZONE_NORMAL, false, Point2D(1, 1), false, true, true);
				Cell tospot = Map.Nearby_Location(to, SPEED_TRACK, Map.Get_Cell_Zone(to), MZONE_NORMAL, false, Point2D(1, 1), false, true, true);

				if (fromspot != CELL_NONE && tospot != CELL_NONE) {
					route.Add(&Map[fromspot]);
					route.Add(&Map[tospot]);

				} else if (Map.Is_Same_Cell_Zone(from, to, MZONE_NORMAL, true, false, false)) {
					if (frombridge != from) {
						route.Add(&Map[frombridge]);
					}

				} else if (fromspot != CELL_NONE) {
					Cell altdest = Map.Nearby_Location(to, SPEED_TRACK, -1, MZONE_NORMAL, true, Point2D(1, 1), false, true, true);
					if (altdest != CELL_NONE && altdest != fromspot) {
						route.Add(&Map[fromspot]);
						dest = &Map[altdest];
					} else {
						Cell fallback = Map.Nearby_Location(to, SPEED_TRACK, Map.Get_Cell_Zone(from, MZONE_NORMAL, true), MZONE_NORMAL, true, Point2D(1, 1), false, true, false);
						if (fallback == CELL_NONE || Get_Height_AGL() < 0) {
							return(NULL);
						}
						dest = &Map[fallback];
					}

				} else {
					Cell fallback = Map.Nearby_Location(to, SPEED_TRACK, Map.Get_Cell_Zone(from, MZONE_NORMAL, true), MZONE_NORMAL, true, Point2D(1, 1), false, true, false);
					if (fallback == CELL_NONE || Get_Height_AGL() < 0) {
						return(NULL);
					}
					dest = &Map[fallback];
				}

			} else if (frombridge != from) {
				route.Add(&Map[frombridge]);
			}
		}

	} else if (canburrowfrom) {
		if (isunderbridgeto) {
			Cell tobridge = Map.Get_Zone_Connection_Destination(to, to);

			if (Is_Route_Broken(from, tobridge)) {
				Cell tospot = Map.Nearby_Location(tobridge, SPEED_TRACK, Map.Get_Cell_Zone(tobridge), MZONE_NORMAL, false, Point2D(1, 1), false, true, true);

				if (tospot != CELL_NONE) {
					route.Add(&Map[tospot]);

				} else if (Map.Is_Same_Cell_Zone(from, to, MZONE_NORMAL, false, true, false)) {
					if (tobridge != from) {
						route.Add(&Map[tobridge]);
					}

				} else {
					Cell altdest = Map.Nearby_Location(to, SPEED_TRACK, -1, MZONE_NORMAL, false, Point2D(1, 1), false, false, true);
					if (altdest != CELL_NONE) {
						dest = &Map[altdest];
					} else {
						Cell fallback = Map.Nearby_Location(to, SPEED_TRACK, Map.Get_Cell_Zone(from), MZONE_NORMAL, false, Point2D(1, 1), false, true, false);
						if (fallback == CELL_NONE || Get_Height_AGL() < 0) {
							return(NULL);
						}
						dest = &Map[fallback];
					}
				}

			} else if (tobridge != from) {
				route.Add(&Map[tobridge]);
			}

		} else if (!canburrowto && Is_Route_Broken(from, to)) {
			Cell tospot = Map.Nearby_Location(to, SPEED_TRACK, Map.Get_Cell_Zone(to), MZONE_NORMAL, false, Point2D(1, 1), false, true, true);

			if (tospot != CELL_NONE) {
				route.Add(&Map[tospot]);

			} else if (!Map.Is_Same_Cell_Zone(from, to, MZONE_NORMAL, true, true, false)) {
				Cell altdest = Map.Nearby_Location(to, SPEED_TRACK, -1, MZONE_NORMAL, false, Point2D(1, 1), false, false, true);
				if (altdest != CELL_NONE) {
					dest = &Map[altdest];
				} else {
					Cell fallback = Map.Nearby_Location(to, SPEED_TRACK, Map.Get_Cell_Zone(from), MZONE_NORMAL, false, Point2D(1, 1), false, false, false);
					if (fallback == CELL_NONE || Get_Height_AGL() < 0) {
						return(NULL);
					}
					dest = &Map[fallback];
				}
			}
		}

	} else if (isunderbridgeto) {
		Cell tobridge = Map.Get_Zone_Connection_Destination(to, to);

		if (Is_Route_Broken(from, tobridge)) {
			Cell fromspot = Map.Nearby_Location(from, SPEED_TRACK, Map.Get_Cell_Zone(from), MZONE_NORMAL, false, Point2D(1, 1), false, true, true);
			Cell tospot = Map.Nearby_Location(tobridge, SPEED_TRACK, Map.Get_Cell_Zone(tobridge), MZONE_NORMAL, false, Point2D(1, 1), false, true, true);

			if (fromspot != CELL_NONE && tospot != CELL_NONE) {
				route.Add(&Map[fromspot]);
				route.Add(&Map[tospot]);

			} else if (Map.Is_Same_Cell_Zone(from, to, MZONE_NORMAL, false, true, false)) {
				if (tobridge != from) {
					route.Add(&Map[tobridge]);
				}

			} else if (fromspot != CELL_NONE) {
				Cell altdest = Map.Nearby_Location(to, SPEED_TRACK, -1, MZONE_NORMAL, false, Point2D(1, 1), false, false, true);
				if (altdest != CELL_NONE && fromspot != altdest) {
					route.Add(&Map[fromspot]);
					dest = &Map[altdest];
				} else {
					Cell fallback = Map.Nearby_Location(to, SPEED_TRACK, Map.Get_Cell_Zone(from), MZONE_NORMAL, false, Point2D(1, 1), false, false, false);
					if (fallback == CELL_NONE || Get_Height_AGL() < 0) {
						return(NULL);
					}
					dest = &Map[fallback];
				}

			} else {
				Cell fallback = Map.Nearby_Location(to, SPEED_TRACK, Map.Get_Cell_Zone(from), MZONE_NORMAL, false, Point2D(1, 1), false, false, false);
				if (fallback == CELL_NONE || Get_Height_AGL() < 0) {
					return(NULL);
				}
				dest = &Map[fallback];
			}

		} else if (tobridge != from) {
			route.Add(&Map[tobridge]);
		}

	} else if (canburrowto) {
		if (Is_Route_Broken(from, to)) {
			Cell fromspot = Map.Nearby_Location(from, SPEED_TRACK, Map.Get_Cell_Zone(from), MZONE_NORMAL, false, Point2D(1, 1), false, true, true);

			if (fromspot != CELL_NONE) {
				route.Add(&Map[fromspot]);

			} else if (!Map.Is_Same_Cell_Zone(from, to, MZONE_NORMAL, false, false, false)) {
				Cell fallback = Map.Nearby_Location(to, SPEED_TRACK, Map.Get_Cell_Zone(from), MZONE_NORMAL, false, Point2D(1, 1), false, false, false);
				if (fallback == CELL_NONE || Get_Height_AGL() < 0) {
					return(NULL);
				}
				dest = &Map[fallback];
			}
		}

	} else {
		if (Is_Route_Broken(from, to)) {
			Cell fromspot = Map.Nearby_Location(from, SPEED_TRACK, Map.Get_Cell_Zone(from), MZONE_NORMAL, false, Point2D(1, 1), false, true, true);
			Cell tospot = Map.Nearby_Location(to, SPEED_TRACK, Map.Get_Cell_Zone(to), MZONE_NORMAL, false, Point2D(1, 1), false, true, true);

			if (fromspot != CELL_NONE && tospot != CELL_NONE) {
				route.Add(&Map[fromspot]);
				route.Add(&Map[tospot]);

			} else if (!Map.Is_Same_Cell_Zone(from, to, MZONE_NORMAL, true, true, false)) {
				if (fromspot != CELL_NONE) {
					Cell altdest = Map.Nearby_Location(to, SPEED_TRACK, -1, MZONE_NORMAL, false, Point2D(1, 1), false, false, true);
					if (altdest != CELL_NONE && fromspot != altdest) {
						route.Add(&Map[fromspot]);
						dest = &Map[altdest];
					} else {
						Cell fallback = Map.Nearby_Location(to, SPEED_TRACK, Map.Get_Cell_Zone(from), MZONE_NORMAL, false, Point2D(1, 1), false, false, false);
						if (fallback == CELL_NONE || Get_Height_AGL() < 0) {
							return(NULL);
						}
						dest = &Map[fallback];
					}
				} else {
					Cell fallback = Map.Nearby_Location(to, SPEED_TRACK, Map.Get_Cell_Zone(from), MZONE_NORMAL, false, Point2D(1, 1), false, false, false);
					if (fallback == CELL_NONE || Get_Height_AGL() < 0) {
						return(NULL);
					}
					dest = &Map[fallback];
				}
			}
		}
	}

	route.Add(dest);
	AbstractClass * next = RouteQueue[0];
	route.Delete_Index(0);
	return(next);
}
#endif


/// <summary>
/// Determines whether a clean direct route exists between two cells, or whether route
/// planning must insert intermediate travel legs. The route is considered broken if the
/// cells are in different movement zones, if the unit is underground, if distant cells
/// fall outside the local radar, or if a trial path walk between them fails. Identical
/// and adjacent cell pairs are never considered broken.
/// </summary>
/// <param name="from">Starting cell.</param>
/// <param name="to">Ending cell.</param>
/// <returns>true if there is no clean direct route between the cells.</returns>
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

/// <summary>
/// RA2: the notice sink. A balloon or jumpjet unit in radio contact answers notice 0x117B by attacking its archived
/// target or scattering; a Crashable one is destroyed on notice 0x117C (a balloon by its death weapon).
/// </summary>
bool UnitClass::Unknown_Virtual_0(int code, int)
{
	if ((Class->BalloonHover || Class->JumpJet) && code == 0x117B && HasAnyLink()) {
		Transmit_Message((RadioMessageType)8);
		if (ArchiveTarget != NULL && ArchiveTarget != NavCom) {
			Assign_Destination(ArchiveTarget, true);
			return(true);
		}
		Abort_Motion();
		Scatter(COORD_NONE, true, false);
		return(true);
	}
	if (Class->Crashable && code == 0x117C) {
		Set_Height_AGL(0);
		if (!Class->BalloonHover) {
			Explode();
		} else {
			FireDeathWeapon(0);
		}
		Delete_Me();
		return(true);
	}
	return(false);
}








/// <summary>
/// Changes ownership of this unit to the house specified.
/// A vehicle that has another following it takes its follower along, so that a captured
/// pair does not end up split between two houses.
/// </summary>
/// <param name="house">The house that is to take ownership of this unit.</param>
/// <returns>bool; Did ownership actually change hands?</returns>
bool UnitClass::Captured(HouseClass * house, bool announce) // RA2: announce
{
	if (house != House) {
		UnitClass * follower = FollowingMe;
		IsFollowing = NULL;

		if (follower != NULL) {
			follower->Captured(house);
			FollowingMe = follower;
			follower->IsFollowing = true;
		}

		return(BASECLASS::Captured(house));
	}
	return(false);
}



/// <summary>
/// Is this object treated as a vehicle by the rest of the game?
/// Some unit types -- visceroids and their kin -- are units only as a convenience and
/// should not be counted or handled as vehicles.
/// </summary>
/// <returns>bool; Should this object be considered a vehicle?</returns>
bool UnitClass::Considered_Vehicle(void) const
{
	return(!Class->IsNonVehicle);
}


/// <summary>
/// RA2: an IFV takes over its gunner's temporal weapon and switches to the gunner's IFVMode weapon.
/// </summary>
void UnitClass::ReceiveGunner(FootClass * gunner)
{
	TemporalClass * temporal = gunner->TemporalImUsing;
	if (temporal != NULL) {
		int rearming = 0;
		if (gunner->Arm != 0) {
			rearming = 1;
		}
		TemporalImUsing = temporal;
		temporal->Owner = this;
		gunner->TemporalImUsing = NULL;
		Set_Current_Weapon(7);
		if (rearming) {
			BurstIndex++;
			Arm = Rearm_Delay(7);
		}
	}
	Set_Current_Weapon(gunner->Techno_Type_Class()->IFVMode);
}


/// <summary>
/// RA2: an IFV hands its temporal weapon back to the leaving gunner and returns to its own weapon.
/// </summary>
void UnitClass::RemoveGunner(FootClass * gunner)
{
	if (gunner != NULL) {
		TemporalClass * temporal = TemporalImUsing;
		if (temporal != NULL) {
			int rearming = 0;
			if (Arm != 0) {
				rearming = 1;
			}
			gunner->TemporalImUsing = temporal;
			TemporalImUsing->Owner = gunner;
			TemporalImUsing = NULL;
			if (gunner->TemporalImUsing != NULL && gunner->TemporalImUsing->Target != NULL) {
				gunner->TemporalImUsing->LetGo();
			}
			if (rearming) {
				gunner->BurstIndex++;
				gunner->Arm = gunner->Rearm_Delay(0);
			}
		}
	}
	Set_Current_Weapon(0);
}


/// <summary>
/// RA2: the type an enemy sees: the disguise.
/// </summary>
ObjectTypeClass * UnitClass::GetDisguise(bool against_allies) const
{
	if (House->Is_Ally(PlayerPtr) && !against_allies) {
		return(Class);
	}
	return(Disguise);
}


/// <summary>
/// RA2: the house an enemy sees: the disguise's.
/// </summary>
HouseClass * UnitClass::GetDisguiseHouse(bool against_allies) const
{
	if (House->Is_Ally(PlayerPtr) && !against_allies) {
		return(House);
	}
	return(DisguisedAsHouse);
}


/// <summary>
/// RA2: takes the look of a type (a mirage tank's tree) with no house.
/// </summary>
void UnitClass::Disguise_As_Type(ObjectTypeClass * type)
{
	if (type != NULL) {
		Disguise = type;
		DisguisedAsHouse = NULL;
		Disguised = true;
		DisguiseCreationFrame = Frame;
		Plot_On_Radar();
	}
}


#pragma warning(push)
#pragma warning(disable: 4716)
/// <summary>
/// RA2: takes the look of a terrain object, an overlay or a disguised unit.
/// </summary>
unsigned UnitClass::DisguiseAs(AbstractClass * target)
{
	if (target != NULL) {
		ObjectClass * object = (ObjectClass *)target;
		switch (target->Fetch_RTTI()) {
			case RTTI_TERRAIN:
				Disguise = (ObjectTypeClass *)object->Class_Of();
				DisguisedAsHouse = NULL;
				return(TechnoClass::DisguiseAs(target));

			case RTTI_CELL:
				if (CanDisguiseAs(target)) {
					Disguise = OverlayTypes[((CellClass *)target)->Overlay];
					DisguisedAsHouse = NULL;
					return(TechnoClass::DisguiseAs(target));
				}
				break;

			case RTTI_UNIT:
				if (object->IsDisguised()) {
					Disguise = object->GetDisguise(true);
					DisguisedAsHouse = object->GetDisguiseHouse(true);
					TechnoClass::DisguiseAs(target);
				}
				break;
		}
	}
}
#pragma warning(pop)


/// <summary>
/// RA2: drops the disguise.
/// </summary>
void UnitClass::ClearDisguise(void)
{
	Disguised = false;
	Plot_On_Radar();
	Disguise = NULL;
	DisguisedAsHouse = NULL;
}


/// <summary>
/// RA2: whether the disguise fools this house (not allied, no disguise sensor on the cell).
/// </summary>
bool UnitClass::IsDisguisedAs(HouseClass * house) const
{
	if (IsDisguised()) {
		CellClass * cell = &Map[Center_Coord().As_Cell()];
		if (!House->Is_Ally(house) && !cell->Is_Disguise_Sensed(house->HeapID)) {
			HouseClass * disguiser = DisguisedAsHouse;
			if (disguiser == house || disguiser == NULL || house->Is_Ally(disguiser)) {
				return(true);
			}
		}
	}
	return(false);
}


/// <summary>
/// RA2: a unit starts as a veteran when its house type lists it or its house infiltrated a war factory (trainable
/// land units); a gunner type starts on its first weapon.
/// </summary>
void UnitClass::Init(void)
{
	TechnoClass::Init();
	if (Class != NULL) {
		if (House != NULL) {
			TypeList<UnitTypeClass const *> & list = House->Class->VeteranUnits;
			for (int i = 0; i < list.Count(); i++) {
				if (list[i] == Class) {
					Veterancy.Set_Veteran(true);
					break;
				}
			}
		}
		if (House != NULL && House->WarFactoryInfiltrated && !Class->Naval && Class->IsTrainable) {
			Veterancy.Set_Veteran(true);
		}
		if (Class->Gunner) {
			Set_Current_Weapon(0);
		}
	}
}


/// <summary>
/// RA2: a DisguiseWhenStill unit (the mirage tank) takes a random DefaultMirageDisguises look while it stands without
/// radio contact; moving, or enemy infantry next to it, drops the look (the latter for InfantryBlinkDisguiseTime).
/// </summary>
void UnitClass::UpdateDisguise(void)
{
	bool still = false;
	if (!IsDisguised()) {
		if (!Locomotion->Is_Moving() && Class->DisguiseWhenStill && Radio_Link(0) == NULL) {
			still = true;
		}
	}

	if (Locomotion->Is_Moving()) {
		goto clear;
	}

	if (Frame % 8 != 0) {
		Cell cell = Get_Cell();
		bool bridge = Get_Cell_Ptr()->Is_Tile_Bridge();
		for (int face = FACING_FIRST; face < FACING_COUNT; face++) {
			CellClass * adjacent = &Map[Adjacent_Cell(cell, FacingType(face))];
			InfantryClass * infantry = adjacent->Cell_Infantry(bridge);
			if (infantry != NULL && !House->Is_Ally(infantry)) {
				InfantryBlinkTimer = Rule->InfantryBlinkDisguiseTime;
				goto clear;
			}
		}
	}

	if (still) {
		if (InfantryBlinkTimer.Value() == 0) {
			int index = Random_Pick(0, Rule->DefaultMirageDisguises.Count() - 1);
			index = MIN(index, Rule->DefaultMirageDisguises.Count() - 1);
			Disguise_As_Type((ObjectTypeClass *)Rule->DefaultMirageDisguises[index]);
		}
	}
	return;

clear:
	ClearDisguise();
}



































/// <summary>
/// Fetches the displayable name of this unit.
/// </summary>
/// <returns>Returns with a pointer to the name to present to the player.</returns>
wchar_t const * UnitClass::Full_Name(void) const // RA2: wide; RA2 also names a turret passenger
{
	UnitClass * self = (UnitClass *)this; // RA2: the tooltip text and the turret tests of the base class are not const
	if (IsDisguisedAs(PlayerPtr) && GetDisguise(true)) { // RA2: a disguised unit shows its disguise's name
		if (!GetDisguiseHouse(true)) { // RA2: as above
			return(NULL); // RA2: as above
		}
		return(GetDisguise(true)->UIName); // RA2: as above
	}
	if (self->Has_Turret_Weapons() && self->Has_Turret_Tooltips()) { // RA2: a unit with a turret weapon names its passenger's role
		wchar_t const * role = NULL; // RA2: as above
		InfantryClass * passenger = (InfantryClass *)Cargo.Attached_Object(); // RA2: as above
		if (passenger != NULL && passenger->Fetch_RTTI() == RTTI_INFANTRY) { // RA2: as above
			if (passenger->Is_Renovator()) { // RA2: as above
				role = StringTable::LoadString("Tip:Repair", NULL, "D:\\Ra2\\Unit.CPP", 10328); // RA2: as above
			} else if (!passenger->Class->IsCivilian) { // RA2: as above
				role = passenger->Class_Of()->UIName; // RA2: as above
				WeaponDataStruct const * weapon = passenger->GetTurretWeapon(); // RA2: as above
				if (weapon != NULL && weapon->Weapon != NULL && weapon->Weapon->WarheadPtr != NULL && weapon->Weapon->WarheadPtr->Bullets) { // RA2: as above
					role = StringTable::LoadString("Tip:MachineGun", NULL, "D:\\Ra2\\Unit.CPP", 10341); // RA2: as above
				}
			}
		}
		if (role == NULL) { // RA2: as above
			role = StringTable::LoadString("Tip:Rocket", NULL, "D:\\Ra2\\Unit.CPP", 10349); // RA2: as above
		}
		swprintf(self->ToolTipText, L"%s %s", role, Class->UIName); // RA2: as above
		return(self->ToolTipText); // RA2: as above
	}
	return(Class->UIName); // RA2: the type's wide name
}



/// <summary>
/// Is this unit unable to move?
/// A vehicle that is playing out its death throes is immobilized on top of all the usual
/// reasons that apply to any foot object.
/// </summary>
/// <returns>bool; Is the unit prevented from moving?</returns>
bool UnitClass::Is_Immobilized(void) const // RA2: at 0x70A240 (vtable slot 201)
{
	return(DeathCounter != -1 || BASECLASS::Is_Immobilized());
}

/// <summary>
/// RA2: TechnoClass's.
/// </summary>
bool UnitClass::CanUseWaypoint(void) const
{
	return(TechnoClass::CanUseWaypoint());
}


/// <summary>
/// RA2: TechnoClass's.
/// </summary>
bool UnitClass::CanAttackOnTheMove(void) const
{
	return(TechnoClass::CanAttackOnTheMove());
}

































/// <summary>
/// Fetches the number of bytes this unit occupies in a save game.
/// This routine lets the load process cope with a save file written by an earlier version
/// of the game, where the members added since then were not yet part of the object.
/// </summary>
/// <param name="oldsave">Is the size wanted for an old format save game?</param>
/// <returns>Returns with the size of the unit, in bytes.</returns>
int UnitClass::Fetch_Object_Size(void) const // RA2: no argument
{
	return(sizeof(*this)); // RA2: no old-save delta
}


/// <summary>
/// Fetches the class identifier used by the save game persistence system.
/// </summary>
/// <param name="retval">Pointer to the buffer to fill in with the class identifier.</param>
/// <returns>Returns with S_OK, or E_POINTER if no buffer was supplied.</returns>
HRESULT STDMETHODCALLTYPE UnitClass::GetClassID(CLSID * retval)
{
	if (retval == NULL) return(E_POINTER);
	*retval = CLSID_UnitClass;
	return(S_OK);
}


/// <summary>
/// Fetches the RTTI type of this object.
/// </summary>
/// <returns>Returns with RTTI_UNIT.</returns>
RTTIType UnitClass::Fetch_RTTI(void) const
{
	return(RTTI_UNIT);
}


/// <summary>
/// Fetches the direction this unit's weapon is pointing.
/// This routine returns the turret facing for a turreted vehicle and the body facing for
/// everything else, so a caller need not know which kind of unit it is dealing with.
/// </summary>
/// <returns>Returns with the direction the unit is currently aiming.</returns>
DirType UnitClass::Turret_Facing(void) const
{
	if (Class->IsTurretEquipped) {
		return(SecondaryFacing.Current());
	}

	return(PrimaryFacing.Current()); // RA2: Turret_Facing at 0x70A2E0 matches only so
}


/// <summary>
/// Draws this unit from its voxel model.
/// This routine lays down the vehicle's shadow, its hull, and whatever turret and barrel
/// it carries. A turreted vehicle is composed on the scratch surface first so that the
/// pieces sort correctly against one another, then blitted to the screen in one piece.
/// </summary>
/// <param name="xdrawpoint">The pixel position to center the unit's model upon.</param>
/// <param name="xcliprect">The clipping rectangle to draw within.</param>
/// <param name="brightness">The lighting level to draw the unit at.</param>
void UnitClass::Unit_Draw_Voxel(Point2D xdrawpoint, Rect xcliprect, int brightness) const
{
	VoxelDataStruct *voxl;
	Point2D drawpoint = xdrawpoint;
	Point2D offset(0, 0);
	Matrix3D main_matrix;

	int frame = TotalFramesWalked / 2 % Class->Voxel.MotLib->Get_Frame_Count();

	switch (frame) {
		case 0:
		case 7:
		case 15:
			break;

		case 1:
		case 6:
		case 8:
		case 14:
			offset = Point2D(0, -1);
			break;

		default:
			offset = Point2D(0, -2);
			break;
	}

	Rect cliprect = xcliprect;
	int has_turret = false;
	ShapeFlags_Type flags = SHAPE_NORMAL;

	if (Class->IsTurretEquipped && Class->AuxVoxel.VoxLib != NULL || Class->AuxVoxel2.VoxLib != NULL &&  Class->AuxVoxel2.MotLib != NULL) {
		has_turret = true;
	}

	int key = 0;
	main_matrix = Locomotion->Shadow_Matrix(&key);

	if (!IsSinking) {
		Point2D shadow_drawpoint = xdrawpoint;
		if (Class->IsHunterSeeker) {
			Coord coord = PositionCoord;
			coord.Z = Map.Get_Height_GL(coord);
			TacticalMap->Coord_To_Pixel(coord, shadow_drawpoint);
			shadow_drawpoint += Locomotion->Shadow_Point();
		}

		if (IsInTransport) {
			shadow_drawpoint.Y -= 14;
		}

		int height = HeightAGL;
		CellClass * cellptr = &Map[Get_Coord()];

		if (IsInTransport) {
			if (!IsOnBridge && height >= BRIDGE_LEPTON_HEIGHT) {
				if (cellptr->IsUnderBridge && (cellptr->IsBridgeEastWest && cellptr->Adjacent_Cell(FACING_N).IsUnderBridge ||
					!cellptr->IsBridgeEastWest && cellptr->Adjacent_Cell(FACING_W).IsUnderBridge)) {
					shadow_drawpoint.Y -= 4 * LEVEL_PIXEL_H;
				}
			}
		}

		Draw_Voxel_Shadow(Class->Voxel, Class->ShadowIndex, key, &Class->ShadowVoxelIndex, xcliprect, shadow_drawpoint, Get_Isometric_View_Matrix() * main_matrix, true);
	}

	Surface * old_surface = LogicalSurface;
	if (has_turret) {
		LogicalSurface = EightBitSurface;
		drawpoint = Point2D(80, 80);
		flags = ShapeFlags_Type(SHAPE_ALPHA|SHAPE_ZGRAD);
		cliprect = LogicalSurface->Get_Rect();
		IsCompositingToEightBitSurface = true;
	}

	if (UseVoxelCache) {
		key = 0;
	} else {
		key = -1;
	}

	main_matrix = Locomotion->Draw_Matrix(&key);

	if (key != -1) {
		key = frame | (key << 5);
	}

	Rect rect = cliprect;
	if (!has_turret && SinkingYOffset > 0) {
		rect = Intersect(rect, Rect(0, 0, TacticalRect.Width, SinkingYOffset - TacticalMap->TacPixelY));
	}

	UnitCompositeDirtyRect = RECT_NONE;

	if (strcmp(Class->IniName, "APC") == 0 && Map[Get_Coord()].Land_Type() == LAND_WATER && !IsOnBridge && HeightAGL < LEVEL_LEPTON_H) {
		Draw_Voxel(Class->AuxVoxel, frame, -1, NULL, rect, drawpoint, Get_Isometric_View_Matrix() * main_matrix, brightness, flags);
	} else {
		Draw_Voxel(Class->Voxel, frame, key, &Class->VoxelIndex, rect, drawpoint, Get_Isometric_View_Matrix() * main_matrix, brightness, flags);
	}

	/*
	**	If there is a turret, then it must be rendered as well. This may include
	**	firing animation if required.
	*/
	if (Class->IsTurretEquipped && Class->AuxVoxel.VoxLib != NULL) {
		main_matrix.Translate_X(Class->TurretOffset / 8);
		main_matrix.Rotate_Z(SecondaryFacing.Current().As_Radian32() - PrimaryFacing.Current().As_Radian32());

		/*
		**	A recoiling turret moves "backward" one pixel.
		*/

		Matrix3D barrel_matrix = main_matrix; // RA2: no IsInRecoilState (TibSun's use does not compile against RA2's declaration)
		Vector3 vec2 = main_matrix.Get_Translation();
		barrel_matrix.Translate(-vec2);

		Vector3 flh = Vector3(-Get_Class_Weapon_Data(0)->FireFLH.X / 8, 0, -Get_Class_Weapon_Data(0)->FireFLH.Z / 8);
		barrel_matrix.Translate(-flh);
		barrel_matrix.Rotate_Y(-BarrelPitch.Current().As_Radian32());
		barrel_matrix.Translate(flh);
		barrel_matrix.Translate(vec2);

		bool draw_barrel;
		if (SecondaryFacing.Current().As_Dir4() <= 0 || SecondaryFacing.Current().As_Dir4() >= 3) {
			draw_barrel = false;
			voxl = &Class->AuxVoxel2;
			if (voxl->VoxLib != NULL && voxl->MotLib != NULL) {
				Draw_Voxel(Class->AuxVoxel2, 0, -1, &Class->VoxelIndex, cliprect, drawpoint + offset, Get_Isometric_View_Matrix() * barrel_matrix, brightness, flags);
			}
		} else {
			draw_barrel = true;
		}

		Draw_Voxel(Class->AuxVoxel, frame, -1, &Class->AuxVoxelIndex, cliprect, drawpoint + offset, Get_Isometric_View_Matrix() * main_matrix, brightness, flags);

		if (draw_barrel) {
			voxl = &Class->AuxVoxel2;
			if (voxl->VoxLib != NULL && voxl->MotLib != NULL) {
				Draw_Voxel(Class->AuxVoxel2, 0, -1, &Class->VoxelIndex, cliprect, drawpoint + offset, Get_Isometric_View_Matrix() * barrel_matrix, brightness, flags);
			}
		}
	} else {
		voxl = &Class->AuxVoxel2;
		if (voxl->VoxLib != NULL && voxl->MotLib != NULL) {
			main_matrix.Rotate_Y(-(BarrelPitch.Current().As_Radian32() - main_matrix.Get_Y_Rotation()));
			main_matrix.Translate(Get_Class_Weapon_Data(0)->FireFLH.X / 8, 0, Get_Class_Weapon_Data(0)->FireFLH.Z / 8);
			Draw_Voxel(Class->AuxVoxel2, frame, -1, &Class->VoxelIndex, cliprect, drawpoint, Get_Isometric_View_Matrix() * main_matrix, brightness, SHAPE_NORMAL);
		}
	}

	if (has_turret) {
		rect = xcliprect;
		if (SinkingYOffset > 0) {
			rect = Intersect(rect, Rect(0, 0, TacticalRect.Width, SinkingYOffset - TacticalMap->TacPixelY));
		}
		Unit_Blit_Voxel(*old_surface, xdrawpoint, rect, brightness);
		LogicalSurface->Fill_Rect(UnitCompositeDirtyRect, TBLACK);
		LogicalSurface = old_surface;
		IsCompositingToEightBitSurface = false;
	}

}


/***********************************************************************************************
 * UnitClass::Reload_AI -- Perform reload logic for this unit.                                 *
 *                                                                                             *
 *    Some units require special reload logic. The V2 rocket launcher in particular. Perform   *
 *    this reload logic with this routine.                                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Only call this routine once per unit per game logic loop.                       *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/30/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
void UnitClass::Reload_AI(void)
{
} // RA2: no IsNoFireWhileMoving (not in RA2's layout)
#endif


/// <summary>
/// Handles the per frame logic for a jellyfish.
/// This routine lets the jellyfish sting anything hostile that drifts into the cells
/// around it, and drives its animation between the idle and stinging states.
/// </summary>
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
void UnitClass::Jellyfish_AI(void)
{
	Cell pos = PositionCell;
	WeaponTypeClass const * weapon = Class->Get_Weapon(0)->Weapon;
	WarheadTypeClass const * warhead = weapon ? weapon->WarheadPtr : NULL;
	int stage = Fetch_Stage();

	bool time_to_attack = (stage % 8 == 0) || stage == 32;
	bool attacked = false;

	if (warhead != NULL && time_to_attack) {
		Cell cell = (Cell)Center_Coord();
		CellClass * cellptr = &Map[cell];
		for (int x = -1; x <= 1; x++) {
			for (int y = -1; y <= 1; y++) {
				Cell newcell = cell + Cell(x, y);
				CellClass * newcellptr = &Map[newcell];
				if (newcellptr != NULL) {
					Coord cell_crd = newcellptr->Cell_Coord();
					if (abs(cell_crd.Z - PositionCoord.Z) < 3.0 * CELL_LEPTON / 2.0) {
						ObjectClass * occupier = newcellptr->Cell_Occupier(newcellptr->IsUnderBridge && (!cellptr->IsUnderBridge || IsOnBridge));
						while (occupier != NULL) {
							ObjectClass * next = occupier->Next;
							if (occupier->IsActive && occupier->IsDown && !occupier->IsInLimbo && occupier->Is_Techno() && occupier != this) {
								TechnoClass * techno = dynamic_cast<TechnoClass *>(occupier);
								if (techno != NULL && techno != this && techno->Strength > 0) {
									UnitClass * unit = dynamic_cast<UnitClass *>(techno);
									bool visceroid = (unit != NULL && (unit->Class->IsSmallVisceroid || unit->Class->IsLargeVisceroid));

									bool invisible;
									if (unit == NULL) {
										BuildingClass * building = dynamic_cast<BuildingClass *>(techno);
										invisible = (building != NULL && (building->Class->IsInvisibleInGame));
									} else {
										invisible = false;
									}

									if (!visceroid && !invisible && !House->Is_Ally(techno)) {
										int damage = weapon->Attack * warhead->Modifier[techno->TClass->Armor];
										techno->Take_Damage(damage, 0, warhead, this);
										attacked = true;
									}
								}
							}
							occupier = next;
						}
					}
					if (attacked) {
						Sound_Effect((VocType)weapon->Sound.Pick(Scen->RandomNumber));
					}
				}
			}
		}
	}

	switch (stage - 1) {

		case 15:
			if (attacked) {
				Set_Stage(16);
				Just_Set_Rate(3);
			} else {
				Set_Stage(0);
				Just_Set_Rate(1);
			}
			break;

		case 31:
			if (!time_to_attack || attacked) {
				Set_Stage(16);
				Just_Set_Rate(3);
			}
			break;

		case 32:
			if (time_to_attack && attacked) {
				Set_Stage(16);
				Just_Set_Rate(3);
			} else {
				Set_Stage(0);
				Just_Set_Rate(1);
			}
			break;

		default:
			if (stage >= 32 || stage < 0) {
				if (attacked) {
					Set_Stage(16);
					Just_Set_Rate(3);
				} else {
					Set_Stage(0);
					Just_Set_Rate(1);
				}
			}
			break;
	}

	Locomotion->Is_Moving();
}
#endif


/***********************************************************************************************
 * UnitClass::Weed_Load -- Determine the Weed load as a percentage.                            *
 *                                                                                             *
 *    Use this routine to determine what the Weed load is (as a floating point percentage).    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the current "fullness" rating for the object.                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/17/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
double UnitClass::Weed_Load(void) const
{
	if (Class->IsToVeinHarvest) {
		return((double)Storage.Get_Total_Amount() / Class->Capacity);
	}
	return(0.0);
}
#endif


/// <summary>
/// Fetches the maximum speed this unit may travel at.
/// A limpet drone clamped to the vehicle drags its top speed down; otherwise this is the
/// normal speed for any foot object.
/// </summary>
int UnitClass::Get_Max_Speed(void) const
{
	int speed = BASECLASS::Get_Max_Speed();
	return(speed); // RA2: no LimpetType, LimpetSpeedFactor (not in RA2's layout)
}


/// <summary>
/// Discharges this unit's stored EM pulse charge.
/// This routine is used by the mobile EM pulse cannon once it has built up a full charge.
/// The pulse goes off at the vehicle's own position and the charge is spent.
/// </summary>
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
void UnitClass::EMPulse_Blast(void)
{
} // RA2: no MaxCharge, ProjectileRange (not in RA2's layout)
#endif

