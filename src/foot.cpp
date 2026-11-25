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

/* $Header: /CounterStrike/FOOT.CPP 2     3/06/97 1:46p Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : FOOT.CPP                                                     *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : April 22, 1994                                               *
 *                                                                                             *
 *                  Last Update : October 5, 1996 [JLB]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   FootClass::AI -- Handle general movement AI.                                              *
 *   FootClass::Active_Click_With -- Initiates attack or move according to target clicked on.  *
 *   FootClass::Active_Click_With -- Performs action as a result of left mouse click.          *
 *   FootClass::Adjust_Dest -- Adjust candidate movement cell to account for formation.        *
 *   FootClass::Approach_Target -- Sets the navigation computer to approach target object.     *
 *   FootClass::Assign_Destination -- Assigns specified destination to NavCom.                 *
 *   FootClass::Basic_Path -- Finds the basic path for a ground object.                        *
 *   FootClass::Body_Facing -- Set the body rotation/facing.                                   *
 *   FootClass::Can_Demolish -- Checks to see if this object can be sold back.                 *
 *   FootClass::Can_Enter_Cell -- Checks to see if the object can enter cell specified.        *
 *   FootClass::Clear_Navigation_List -- Clears out the navigation queue.                      *
 *   FootClass::Death_Announcement -- Announces the death of a unit.                           *
 *   FootClass::Debug_Dump -- Displays the status of the FootClass to the mono monitor.        *
 *   FootClass::Detach -- Detaches a target from tracking systems.                             *
 *   FootClass::Detach_All -- Removes this object from the game system.                        *
 *   FootClass::Enters_Building -- When unit enters a building for some reason.                *
 *   FootClass::FootClass -- Normal constructor for the foot class object.                     *
 *   FootClass::Greatest_Threat -- Fetches the greatest threat to this object.                 *
 *   FootClass::Handle_Navigation_List -- Processes the navigation queue.                      *
 *   FootClass::Is_Allowed_To_Leave_Map -- Checks to see if it can leave the map and the game. *
 *   FootClass::Is_On_Priority_Mission -- Checks to see if this object should be given priority*
 *   FootClass::Is_Recruitable -- Determine if this object is recruitable as a team members.   *
 *   FootClass::Likely_Coord -- Fetches the coordinate the object will be at shortly.          *
 *   FootClass::Mark -- Unit interface to map rendering system.                                *
 *   FootClass::Mission_Attack -- AI for heading towards and firing upon target.               *
 *   FootClass::Mission_Capture -- Handles the capture mission.                                *
 *   FootClass::Mission_Enter -- Enter (cooperatively) mission handler.                        *
 *   FootClass::Mission_Guard_Area -- Causes unit to guard an area about twice weapon range.   *
 *   FootClass::Mission_Hunt -- Handles the default hunt order.                                *
 *   FootClass::Mission_Move -- AI process for moving a vehicle to its destination.            *
 *   FootClass::Mission_Retreat -- Handle reatreat from map mission for mobile objects.        *
 *   FootClass::Offload_Tiberium_Bail -- Fetches the Tiberium to offload per step.             *
 *   FootClass::Override_Mission -- temporarily overrides a units mission                      *
 *   FootClass::Per_Cell_Process -- Perform action based on once-per-cell condition.           *
 *   FootClass::Queue_Navigation_List -- Add a target to the objects navigation list.          *
 *   FootClass::Receive_Message -- Movement related radio messages are handled here.           *
 *   FootClass::Rescue_Mission -- Calls this unit to the rescue.                               *
 *   FootClass::Restore_Mission -- Restores an overridden mission                              *
 *   FootClass::Sell_Back -- Causes this object to be sold back.                               *
 *   FootClass::Set_Speed -- Initiate unit movement physics.                                   *
 *   FootClass::Sort_Y -- Determine the sort coordinate for foot class objects.                *
 *   FootClass::Start_Driver -- This starts the driver heading to the destination desired.     *
 *   FootClass::Stop_Driver -- This routine clears the driving state of the object.            *
 *   FootClass::Stun -- Prepares a ground travelling object for removal.                       *
 *   FootClass::Take_Damage -- Handles taking damage to this object.                           *
 *   FootClass::Unlimbo -- Unlimbos object and performs special fixups.                        *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "always.h"
#include "geometry.h" // RA2: the header statics, in the order of the unit's initializer table
#include "rect.h"

#include "foot.h"

#include "_astar.h"
#include "_bench.h"
#include "_convert.h"
#include "_keyboar.h"
#include "_map.h"
#include "_rect.h"
#include "_rtti.h"
#include "_rules.h"
#include "_surface.h"
#include "_tactica.h"
#include "aircraft.h"
#include "anim.h"
#include "astar.h"
#include "bench.h"
#include "building.h"
#include "builtype.h"
#include "capmgr.h"
#include "bomb.h" // RA2: Active_Click_With reads the attached bomb's owner
#include "ccrand.h"
#include "cell.h"
#include "convert.h"
#include "ddist.h"
#include "dsurface.h" // RA2: Build_Hicolor_Pixel
#include "event.h" // RA2: Active_Click_With sends a detonate event
#include "goptions.h"
#include "house.h"
#include "incdec.h"
#include "infantry.h"
#include "infatype.h"
#include "inline.h"
#include "ion.h"
#include "ipiggy.h"
#include "loco.h"
#include "mono.h"
#include "partsys.h"
#include "queue.h"
#include "warhead.h" // RA2: Take_Damage reads WarheadTypeClass::Sonic
#include "parasite.h" // RA2: Detach tells the parasite that is eating this object (ParasiteImUsing->Detach)
#include "revent.h"
#include "rules.h"
#include "session.h"
#include "swizzle.h"
#include "tactical.h"
#include "team.h"
#include "tracker.h"
#include "tube.h"
#include "unit.h"
#include "unittype.h"
#include "vein.h"
#include "voc.h"
#include "vox.h"
#include "waypoint.h"
#include "weapon.h"
#include "xsurface.h"

#include "bench.hh"
#include "color.hh"
#include "tube.hh"


// RA2: the weighted cell distribution retains its vector constructor out of line.
template<>
DynamicVectorClass<DiscreteDistributionClass<CellClass>::DistributionObject<CellClass *> >::DynamicVectorClass(unsigned size, DiscreteDistributionClass<CellClass>::DistributionObject<CellClass *> const *array)
	: VectorClass<DiscreteDistributionClass<CellClass>::DistributionObject<CellClass *> >(size, array)
{
	GrowthStep = 10;
	ActiveCount = 0;
}


DynamicVectorClass<FootClass *> Feet;

/***********************************************************************************************
 * FootClass::FootClass -- Default constructor for foot class objects.                         *
 *                                                                                             *
 *    This is the default constructor for FootClass objects. It sets the foot class values to  *
 *    their default starting settings.                                                         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/23/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
FootClass::FootClass(HouseClass * house) :
	BASECLASS(house),
	CurrentPath(PATH_NONE),
	WaypointOffsetCell(0,0),
	WaypointTargetCell(0,0),
	ThreatAvoidanceCoefficient(0),
	TotalFramesWalked(0),
	IsMoveSoundPlaying(false), // RA2: +0x434 and +0x438 are cleared right after TotalFramesWalked
	MoveSoundDelay(0),
	LastPathingCell(0,0),
	LastAdjacencyCell(0,0),
	LastFlightMapCoords(0,0), // RA2: +0x458 is cleared with the other cells
	LastTubeCoord(0,0,0),
	Speed(0),
	SpeedBias(1),
	RouteQueue(0, NULL), // RA2: the vector is built out of line with (0, NULL)
	FollowTarget(NULL), // RA2: +0x490 is cleared
	NavCom(NULL),
	SuspendedNavCom(NULL),
	NavQueue(0, NULL), // RA2: as RouteQueue
	MegaMission(MISSION_NONE), // RA2: +0x4B4 starts at -1
	MegaDestination(NULL), // RA2: +0x4B8 is cleared
	MegaTarget(NULL), // RA2: +0x4BC is cleared
	HasAttackMoveTarget(false), // RA2: +0x4C1 is cleared
	Team(0),
	Member(0),
	PatrolCell(0),
	TryTryAgain(PATH_RETRY),
	HeadToCoord(COORD_NONE),
	CurrentTube(-1),
	CurrentTubeDir(0),
	NextWaypoint(0),
	IsToScatter(false),
	IsScanLimited(false),
	IsInitiated(false),
	IsNewNavCom(false),
	IsPlanningToLook(false),
	IsDeploying(false),
	IsFiring(false),
	IsOverpowering(false), // RA2: +0x57E is cleared
	ParasiteEatingMe(NULL), // RA2: +0x580 is cleared
	LastBeParasitedStartFrame(0), // RA2: +0x584 is cleared
	ParasiteImUsing(NULL), // RA2: +0x588 is cleared
	IsMoveToSkipped(false), // RA2: +0x598 is cleared
	IsRotating(false),
	IsUnloading(false),
	IsNavQueueLoop(false),
	IsScattering(false),
	IsIdle(false),
	IonBlastYDrawOffset(0),
	IsCrushing(false),
	IsOccupyingCell(true),
	IsToPathAroundBlockage(false),
	IsDroppedFromTeam(false)
{
	Path[0] = FACING_NONE;
	MoveSoundAudioController.Init(); // RA2: 0x405BF0 clears the sound handle at +0x43C
	Feet.Add(this);
	AbstractFlags |= 4; // RA2: bit 2 of the abstract flags (the one Is_Foot reads) is set between the two list adds
	TeamPtrTracker.Add(this);
}


/// <summary>
/// Constructs the object without initializing it.
/// This constructor is used by the load process, which needs the object built in place but
/// left untouched so that the saved data can be read straight over the top of it.
/// </summary>
FootClass::FootClass(NoInitClass const & x) :
	BASECLASS(x),
	RouteQueue(x),
	NavQueue(x),
	PathDelay(x),
	BaseAttackTimer(x),
	SightTimer(x), // RA2: the timer at +0x54C and the one at +0x58C are built no-init too
	BlockagePathDelay(x),
	ParalysisTimer(x)
{
}


/// <summary>
/// Destroys the object and unlinks it from the game.
/// This routine will resign the object from whatever team it belongs to and then remove it
/// from the global foot and team trackers.
/// </summary>
FootClass::~FootClass(void)
{
	if (Team != NULL && GameActive) {
		Team->Remove(this);
	}

	Feet.Delete(this);
	TeamPtrTracker.Delete(this);

	if (ParasiteImUsing != NULL) { // RA2: the parasite object this one owns is deleted (virtual deleting destructor, slot 8) and forgotten
		delete ParasiteImUsing;
		ParasiteImUsing = NULL;
	}

	MoveSoundAudioController.EndLooping(); // RA2: 0x406070, then 0x405C10, run on the sound handle at +0x43C
	MoveSoundAudioController.Release();
}


/***********************************************************************************************
 * FootClass::Set_Speed -- Initiate unit movement physics.                                     *
 *                                                                                             *
 *    This routine is used to set a unit's velocity control structure.                         *
 *    The game will then process the unit's movement during the momentum                       *
 *    physics calculation.                                                                     *
 *                                                                                             *
 * INPUT:   unit  -- Pointer to the unit to alter.                                             *
 *                                                                                             *
 *          speed -- Throttle setting (0=stop, 255=full throttle).                             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/07/1992 JLB : Created.                                                                 *
 *   09/24/1993 JLB : Revised for faster speed.                                                *
 *   04/02/1994 JLB : Revised for new system.                                                  *
 *   04/15/1994 JLB : Converted to member function.                                            *
 *   07/21/1994 JLB : Simplified.                                                              *
 *=============================================================================================*/
void FootClass::Set_Speed(double speed)
{
	if (speed >= 1.0) {
		speed = 1.0;
	} else if (speed <= 0.0) {
		speed = 0.0;
	}

	Speed = speed;
}


/***********************************************************************************************
 * FootClass::Mark -- Unit interface to map rendering system.                                  *
 *                                                                                             *
 *    This routine is the interface function for units as they relate to                       *
 *    the map rendering system. Whenever a unit's imagery changes, this                        *
 *    function is called.                                                                      *
 *                                                                                             *
 * INPUT:   mark  -- Type of image change (MARK_UP, _DOWN, _CHANGE)                            *
 *             MARK_UP  -- Unit is removed.                                                    *
 *             MARK_CHANGE -- Unit alters image but doesn't move.                              *
 *             MARK_DOWN -- Unit is overlaid onto existing icons.                              *
 *                                                                                             *
 * OUTPUT:  bool; Did the marking operation succeed? Failure could be the result of marking    *
 *                down when it is already down, or visa versa.                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/14/1991 JLB : Created.                                                                 *
 *   04/15/1994 JLB : Converted to member function.                                            *
 *   12/23/1994 JLB : Performs low level check before processing.                              *
 *=============================================================================================*/
bool FootClass::Mark(MarkType mark)
{
	if (mark == MARK_CHANGE) { // RA2: Mark at 0x4C2E60 has no assert(this != NULL);
		return(true);
	}

	if (BASECLASS::Mark(mark)) {
		if (In_Which_Layer() == LAYER_GROUND) {
			Cell cell = Get_Cell();

			switch (mark) {
				case MARK_UP:
					Map.Pick_Up(cell, this);
					break;

				case MARK_DOWN:
				case MARK_DOWN_FORCED:
					Map.Place_Down(cell, this);
					break;
			}
		}
		return(true);
	}
	return(false);
}


/// <summary>
/// Determines if the specified location shares this object's destination zone.
/// This routine is consulted before an object commits to a journey, so that it does not set
/// off toward somewhere its locomotor could never actually walk or drive to. An object with
/// no movement zone restriction always answers yes.
/// </summary>
/// <param name="coord">The location to compare against this object's destination.</param>
/// <returns>bool; Are the two locations in the same movement zone?</returns>
bool FootClass::Is_In_Same_Zone(Coord const & coord) const
{
	MZoneType zone = Techno_Type_Class()->MZone; // RA2: Is_In_Same_Zone at 0x4C2EF0 matches only so
	if (zone == MZONE_NONE) {
		return(true);
	}

	Cell cell1 = coord.As_Cell();

	if (cell1 == CELL_NONE) {
		return(false);
	}

	Cell cell2 = Destination_Coord().As_Cell();

	return(Map.Is_Same_Cell_Zone(cell2, cell1, zone, ((ObjectClass *)this)->Is_Moving_Onto_Bridge(), Map[cell1].IsUnderBridge, Is_Allowed_To_Leave_Map()));
} // RA2: Is_In_Same_Zone at 0x4C2EF0 has no return(0);


/***********************************************************************************************
 * FootClass::Basic_Path -- Finds the basic path for a ground object.                          *
 *                                                                                             *
 *    This is a common routine used by both infantry and other ground travelling units. It     *
 *    will fill in the unit's basic path to the NavCom destination.                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was a path found? A failure to find a path means either the target cannot    *
 *                be found or the terrain prohibits the unit's movement.                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool FootClass::Basic_Path(Cell cell, int path_offset, int avoidance)
{
	/// simplifies movement zone types
	static const MZoneType _simple_mzone[] = { // RA2: twelve entries, the water zones map to themselves
		MZONE_NORMAL,
		MZONE_NORMAL,
		MZONE_NORMAL,
		MZONE_AMPHIBIOUS,
		MZONE_AMPHIBIOUS,
		MZONE_AMPHIBIOUS,
		MZONE_SUBTERANNEAN,
		MZONE_INFANTRY,
		MZONE_INFANTRY,
		MZONE_FLYER,
		MZONE_WATER,
		MZONE_WATER_BEACH,
	};

	PathStruct		* path;			// Pointer to path control structure.
	int 			skip_path = false;

	if (path_offset == 0) {
		Path[0] = FACING_NONE;
	}

	if (!Is_In_Same_Zone(cell)) {
		Path[0] = FACING_NONE;
		return(false);
	}

	/*
	**	When the navigation computer is set to a location that is impassible, then
	**	find a nearby cell that can be entered and try to head toward that instead.
	**	EXCEPT when that cell is very close -- then just bail.
	*/
	int dist = Distance(cell);
	int checkdist = Team != NULL ? Team->Stray_Distance() : Rule->CloseEnoughDistance; // RA2: the stray distance is the team's call (0x6BE6A0)
	int maxdist = 0;

	bool istrain = TClass->IsTrain;

	MoveType move = Can_Enter_Cell(&Map[cell]);

	if (move == MOVE_TEMP && dist > checkdist && !istrain) {

		MZoneType mzone = _simple_mzone[TClass->MZone];
		bool check = TClass->IsSubterranean && Map[cell].Can_Burrow_Here() ? true : false;

		Cell nearby = Map.Nearby_Location(cell, TClass->Speed, Map.Get_Cell_Zone(PositionCell, mzone, IsOnBridge), mzone, IsOnBridge, Point2D(1, 1), false, true, check, true, PositionCell);
		maxdist = MAX(abs(nearby.X - cell.X), abs(nearby.Y - cell.Y));

		if (nearby != CELL_NONE && ::Distance(Coord(cell), Coord(nearby)) < dist) {

			int adist = Search.Test_Cell_Walk(nearby, cell, this, Map[nearby].IsUnderBridge, Map[cell].IsUnderBridge, check ? MZONE_NORMAL : MZONE_NONE);
			if (adist <= maxdist + 6) {
				Assign_Destination(&Map[nearby]);
				cell = nearby;
			}
		}
	}

	if (move == MOVE_NO && !istrain) {

		if (Map[cell].Cell_Building() != NULL) {

			MZoneType mzone = _simple_mzone[TClass->MZone];
			bool check = TClass->IsSubterranean && Map[cell].Can_Burrow_Here() ? true : false;

			Cell nearby = Map.Nearby_Location(cell, TClass->Speed, Map.Get_Cell_Zone(PositionCell, mzone, IsOnBridge), mzone, IsOnBridge, Point2D(1, 1), false, true, check, true, PositionCell);

			Assign_Destination(&Map[nearby]);
			cell = nearby;
		}
	}

	if (!skip_path) {
		Mark(MARK_UP);
		if (path_offset == 0) {
			Path[0] = FACING_NONE;		// Probably not necessary, but...
		}

		/*
		**	Try to find a path to the destination. If a failure occurs, then keep trying
		**	with greater determination until either a complete failure occurs, or a decent
		**	path was found.
		*/
		bool found = false;		// Found a best path yet?
		PathStruct path1;
		FacingType workpath[200*10];	// Staging area for path list.
		MoveType maxtype = MOVE_OK;

		path = Find_Path(cell, &workpath[0], ARRAY_SIZE(workpath), maxtype, path_offset, avoidance);

		if (path && path->Cost) {
			memcpy(&path1, path, sizeof(path1));
			found = true;
		}

		/*
		**	If a good path was found, then record it in the object's path
		**	list.
		*/
		if (found) {
			Fixup_Path(&path1);
			memcpy(&Path[path_offset], &workpath[0], MIN(path->Length, ARRAY_SIZE(Path) - path_offset) * sizeof(Path[0]));
		}

		Mark(MARK_DOWN);

		PathDelay = 0;

		if (RTTI == RTTI_UNIT && Path[0] != FACING_NONE) {
			UnitClass * unit = (UnitClass *)this;
			UnitClass * follower = unit->FollowingMe;
			int length = 0;
			if (path != NULL) {
				length = path->Length - 2;
			}
			Cell ncell = cell;
			if (follower != NULL && !unit->IsFollowing) {
				while (follower != NULL) {
					CellClass *cptr = &Map[ncell];
					ncell = cptr->Adjacent_Cell(FacingType((workpath[length] + FACING_180) % FACING_COUNT)).Fetch_CellID();
					AbstractClass *nav = follower->NavCom;
					if (nav == NULL || nav->Destination_Coord().As_Cell() != ncell) {
						follower->QueueMission(MISSION_MOVE, false); // RA2: QueueMission, not Assign_Mission
						follower->Assign_Destination(&Map[ncell]);
						follower->Basic_Path(ncell);
					}
					follower = follower->FollowingMe;
				}
			}
		}

		if (path != NULL) {
			LastPathingCell = PositionCell;
			return(true);
		}

		PathDelay = Rule->PathDelay * TICKS_PER_MINUTE;
	}

	/*
	**	If a basic path couldn't be determined, then abort the navigation process.
	*/
	Stop_Driver();

	Cell mycell = PositionCoord;
	maxdist = MAX(abs(mycell.X - cell.X), abs(mycell.Y - cell.Y));
	if (maxdist > 1 || (!IsOnBridge && Map[cell].IsUnderBridge)) {
		if (Team != NULL) {
			Locomotion->Lock();
			Team->Remove(this);
			Locomotion->Unlock();
		}

		Assign_Destination(NULL);
		Assign_Target(NULL);

		if (!House->Is_Human_Player()) {
			QueueMission(MISSION_GUARD_AREA, false);
			if (Session.Type != GAME_NORMAL) {
				Cell where = House->Where_To_Go(this);
				if (where != CELL_NONE) {
					Assign_Destination(&Map[where]);
				}
			}
		} else {
			QueueMission(MISSION_GUARD, false);
		}
	}

	return(false);
}



/***********************************************************************************************
 * FootClass::Mission_Move -- AI process for moving a vehicle to its destination.              *
 *                                                                                             *
 *    This simple AI script handles moving the vehicle to its desired destination. Since       *
 *    simple movement is handled directly by the engine, this routine merely waits until       *
 *    the unit has reached its destination, and then causes the unit to enter idle mode.       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the delay before calling this routine again.                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1994 JLB : Created.                                                                 *
 *   10/02/1996 JLB : Player controlled or human owned units don't scan for targets.           *
 *=============================================================================================*/
int FootClass::Do_MISSION_MOVE(void)
{
	if (NavCom == NULL && !Locomotion->Is_Moving() && MissionQueue == MISSION_NONE) {
		Enter_Idle_Mode();
		return(1);
	}

	return(Current_Mission_Control().Normal_Delay() + Random_Pick(0, 2)); // RA2: Do_MISSION_MOVE at 0x4C38D0 has no TarCom, House, Team and 2 more
}


/// <summary>
/// RA2
/// </summary>
static inline ObjectClass * To_Object(AbstractClass * a)
{
	return(a == NULL ? NULL : ((a->AbstractFlags & 2) ? (ObjectClass *)a : NULL));
}


/// <summary>
/// Handles the patrol mission state machine. The unit scans for threats while
/// traveling, breaks off to engage any threat it can reach, then returns to
/// its patrol point or resumes its archived travel destination.
/// </summary>
/// <returns>The delay in game frames before this mission should be processed again.</returns>
int FootClass::Do_MISSION_PATROL(void)
{
	int trange = Threat_Range(2);
	IsOnPatrol = true;
	int dist = (trange / CELL_LEPTON_W) + 6;

	enum {
		PATROL_FIND_TARGET,		/// scan for a target and take up the patrol
		PATROL_ENGAGE_TARGET,	/// fighting the target; approach until it is gone
		PATROL_RETURN,			/// heading back; re-scan from the patrol cell
		PATROL_RESET			/// wait until moving again before scanning anew
	};

	switch (Status) {

		case PATROL_RESET:
			if (NavCom != NULL) {
				Status = PATROL_FIND_TARGET;
				return(1);
			}
			break;

		case PATROL_FIND_TARGET: {
			AbstractClass * target = Greatest_Threat(THREAT_AREA, Get_Coord(), false);
			ObjectClass * optr = (target == NULL) ? NULL : ((target->AbstractFlags & ABSTRACT_OBJECT) ? (ObjectClass *)target : NULL); // RA2: the object flag test of the abstract, not a call
			int primary = What_Weapon_Should_I_Use(optr);

			if (optr != NULL && (In_Range(optr, primary) || Search.Test_Cell_Walk(Cell(Destination_Coord()), Cell(optr->Destination_Coord()), this, Is_Moving_Onto_Bridge(), optr->Is_Moving_Onto_Bridge(), MZONE_NONE) < dist)) {

				/*
				 * Take up patrol at the cell the target occupies. If that cell is
				 * tucked under a bridge and this unit isn't crossing onto the
				 * bridge, find a nearby clear cell instead.
				 */
				PatrolCell = Get_Target_Cell_Ptr();
				if (PatrolCell->IsUnderBridge && !Is_Moving_Onto_Bridge()) {
					Cell cell = Map.Nearby_Location(PatrolCell->CellID, TClass->Speed, Map.Get_Cell_Zone(PatrolCell->CellID, TClass->MZone, false), TClass->MZone, false, Point2D(1,1), false, true, false, false);
					if (cell != CELL_NONE) {
						PatrolCell = &Map[cell];
					} else {
						PatrolCell = NULL;
						return(60);
					}
				}

				/*
				 * Remember where to resume travel once the threat is dealt with.
				 */
				if (ArchiveTarget == NULL) {
					if (RouteQueue.Count() > 0) {
						ArchiveTarget = RouteQueue[RouteQueue.Count()-1];
					} else if (NavCom != NULL) {
						ArchiveTarget = NavCom;
					} else {
						ArchiveTarget = PatrolCell;
					}
				}

				Assign_Destination(NULL);
				Assign_Target(target);
				Status = PATROL_ENGAGE_TARGET;

			} else if (NavCom == NULL || !Locomotion->Is_Moving()) {
				Enter_Idle_Mode();
				Status = PATROL_RESET;
				return(1);
			}
			break;
		}

		case PATROL_ENGAGE_TARGET: {
			ObjectClass * optr = (TarCom == NULL) ? NULL : ((TarCom->AbstractFlags & ABSTRACT_OBJECT) ? (ObjectClass *)TarCom : NULL);
			int primary = What_Weapon_Should_I_Use(optr);

			/*
			 * Engineers shouldn't try to recapture an allied building that is
			 * still healthy enough.
			 */
			if (Is_Renovator() && optr != NULL && optr->RTTI == RTTI_BUILDING && House->Is_Ally(((TechnoClass *)optr)->House) && optr->HealthRatio > Rule->ConditionRed) {
				Assign_Target(NULL);
			} else if (optr != NULL && (In_Range(optr, primary) || Search.Test_Cell_Walk(PatrolCell->CellID, Cell(optr->Destination_Coord()), this, PatrolCell->IsUnderBridge, optr->Is_Moving_Onto_Bridge(), MZONE_NONE) < dist)) {
				Approach_Target();
				if (TarCom != NULL) {
					break;
				}
				Assign_Destination(ArchiveTarget);
				ArchiveTarget = NULL;
				PatrolCell = NULL;
				Status = PATROL_FIND_TARGET;
				return(45);
			}

			/*
			 * The target is gone or out of reach. Scan for another threat from
			 * the patrol cell before giving up the engagement.
			 */
			if (TarCom == NULL) {
				AbstractClass * target = Greatest_Threat(THREAT_AREA, PatrolCell->Center_Coord(), false);
				ObjectClass * scanptr = (target == NULL) ? NULL : ((target->AbstractFlags & ABSTRACT_OBJECT) ? (ObjectClass *)target : NULL);
				int scanweapon = What_Weapon_Should_I_Use(scanptr);
				if (scanptr != NULL && (In_Range(scanptr, scanweapon) || Search.Test_Cell_Walk(PatrolCell->CellID, Cell(scanptr->Destination_Coord()), this, PatrolCell->IsUnderBridge, scanptr->Is_Moving_Onto_Bridge(), MZONE_NONE) < dist)) {
					Assign_Target(target);
					Assign_Destination(NULL);
					return(1);
				}
			}

			/*
			 * Head for whichever is closer; the patrol cell or the archived
			 * travel destination.
			 */
			Status = PATROL_RETURN;
			Assign_Target(NULL);
			if (ArchiveTarget != NULL) {
				CellClass * tcell = Get_Target_Cell_Ptr();
				bool onbridge = Is_Moving_Onto_Bridge();
				CellClass * acell = &Map[ArchiveTarget->Center_Coord()];
				int patrolwalk = Search.Test_Cell_Walk(tcell->CellID, PatrolCell->CellID, this, onbridge, PatrolCell->IsUnderBridge, MZONE_NONE);
				if (patrolwalk < Search.Test_Cell_Walk(tcell->CellID, acell->CellID, this, onbridge, acell->IsUnderBridge, MZONE_NONE)) {
					Assign_Destination(PatrolCell);
				} else {
					Assign_Destination(ArchiveTarget);
				}
				return(1);
			}
			Status = PATROL_FIND_TARGET;
			Enter_Idle_Mode();
			Status = PATROL_RESET;
			return(1);
		}

		case PATROL_RETURN: {
			AbstractClass * target = Greatest_Threat(THREAT_AREA, PatrolCell->Center_Coord(), false);
			ObjectClass * optr = (target == NULL) ? NULL : ((target->AbstractFlags & ABSTRACT_OBJECT) ? (ObjectClass *)target : NULL); // RA2: the object flag test of the abstract, not a call
			int primary = What_Weapon_Should_I_Use(optr);

			if (optr != NULL && (In_Range(optr, primary) || Search.Test_Cell_Walk(PatrolCell->CellID, Cell(optr->Destination_Coord()), this, PatrolCell->IsUnderBridge, optr->Is_Moving_Onto_Bridge(), MZONE_NONE) < dist)) {
				Assign_Target(target);
				Assign_Destination(NULL);
				Status = PATROL_ENGAGE_TARGET;
				break;
			}

			if (NavCom == NULL) {
				Assign_Target(NULL);
				if (ArchiveTarget != NULL) {
					Assign_Destination(ArchiveTarget);
					ArchiveTarget = NULL;
					Status = PATROL_FIND_TARGET;
					PatrolCell = NULL;
				} else {
					Status = PATROL_FIND_TARGET;
					Enter_Idle_Mode();
					Status = PATROL_RESET;
					return(1);
				}
			}
			break;
		}

		default:
			break;
	}

	return(Current_Mission_Control().Normal_Delay() + Random_Pick(0, 2));
}



/***********************************************************************************************
 * FootClass::Mission_Capture -- Handles the capture mission.                                  *
 *                                                                                             *
 *    Capture missions are nearly the same as normal movement missions. The only difference    *
 *    is that the final destination is handled in a special way so that it is not marked as    *
 *    impassable. This allows the object (usually infantry) the ability to walk onto the       *
 *    object and thus capture it.                                                              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of game ticks to delay before calling this routine.        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int FootClass::Do_MISSION_CAPTURE(void)
{
	InfantryClass * inf = (this == NULL) ? NULL : (RTTI == RTTI_INFANTRY) ? (InfantryClass *)this : NULL; // RA2: Do_MISSION_CAPTURE at 0x4C41E0 matches only so

	if (TarCom != NULL && TarCom->RTTI == RTTI_BUILDING && NavCom == NULL && inf != NULL && (inf->Class->IsBomber || inf->Has_Ability(ABILITY_C4) || inf->Class->IsEngineer || inf->Class->Occupier || inf->Class->Assaulter || inf->Class->IsCapture)) {
		Assign_Destination(TarCom);
	}

	if (NavCom == NULL) {
		Enter_Idle_Mode();
		if (Map[Center_Coord()].Cell_Building()) {
			Scatter(COORD_NONE, true);
		}
	}

	if (TarCom == NULL && !House->Is_Human_Player()) {
		if (RTTI != RTTI_INFANTRY || !((InfantryClass *)this)->Class->Occupier) {
			Assign_Target(NULL);
			Assign_Destination(NULL);
			QueueMission(MISSION_HUNT, false);
		}
	}

	return(Current_Mission_Control().Normal_Delay() + Random_Pick(0, 2));
}





/// <summary>
/// The mission of an object that is being swallowed: an infantry that can blow up or capture a building it
/// targets heads into it, an object with nowhere to go idles (and gets off any building), and a computer
/// object with no target goes back to guarding.
/// </summary>
int FootClass::Do_MISSION_EATEN(void) // RA2: not in TibSun, at 0x4C4370
{
	InfantryClass * inf = (this == NULL) ? NULL : (RTTI == RTTI_INFANTRY) ? (InfantryClass *)this : NULL;

	if (TarCom != NULL && TarCom->RTTI == RTTI_BUILDING && NavCom == NULL && inf != NULL && (inf->Class->IsBomber || inf->Has_Ability(ABILITY_C4) || inf->Class->IsEngineer)) {
		Assign_Destination(TarCom);
	}

	if (NavCom == NULL) {
		Enter_Idle_Mode();
		if (Map[Center_Coord()].Cell_Building()) {
			Scatter(COORD_NONE, true);
		}
	}

	if (TarCom == NULL && !House->Is_Human_Player()) {
		Assign_Target(NULL);
		Assign_Destination(NULL);
		QueueMission(MISSION_GUARD, false);
	}

	return(Current_Mission_Control().Normal_Delay() + Random_Pick(0, 2));
}


/***********************************************************************************************
 * FootClass::Mission_Attack -- AI for heading towards and firing upon target.                 *
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
int FootClass::Do_MISSION_ATTACK(void)
{
	if (Techno_Type_Class()->HoverAttack && HeightAGL == 0) { // RA2: a hover-attacker on the ground first moves to a nearby cell
		Assign_Destination(&Map[Nearby_Location(this)], true);
	}

	if (IsOverpowering) { // RA2: an electric-assault unit looks for a target to overpower
		Coord coord = Position; // RA2: as above (the member, not the virtual Center_Coord)
		AbstractClass * target = Greatest_Threat(THREAT_RANGE, coord, false);
		if (target != NULL) {
			Assign_Target(target);
			IsOverpowering = false;
		}
	}

	if (TarCom != NULL) {
		Approach_Target();
	} else {
		Enter_Idle_Mode();
	}
	return(Current_Mission_Control().Normal_Delay() + Random_Pick(0, 2));
}






/// <summary>
/// Finds an adjacent building this object can overpower.
/// RA2: a member function (0x004C45B0). It walks the eight cells around the object in facing order and
/// returns the first building whose type is Overpowerable and whose owner is this object's owner.
/// </summary>
/// <returns>BuildingClass *; The building to overpower, or NULL.</returns>
BuildingClass * FootClass::Find_Adjacent_Overpowerable(void)
{
	for (int face = FACING_FIRST; face < FACING_COUNT; face++) {
		BuildingClass * building = Map[Adjacent_Cell(Get_Cell(), (FacingType)face)].Cell_Building();
		if (building != NULL && building->Class->Overpowerable && building->Owner_HouseClass() == Owner_HouseClass()) {
			return(building);
		}
	}
	return(NULL);
}


/***********************************************************************************************
 * FootClass::Mission_Guard -- Handles the AI for guarding in place.                           *
 *                                                                                             *
 *    Units that are performing stationary guard duty use this AI process. They will sit       *
 *    still and target any enemies that get within range.                                      *
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
int FootClass::Do_MISSION_GUARD(void)
{
	if (TarCom == NULL) {
		if (Get_Class_Weapon_Data(1)->Weapon != NULL && Get_Class_Weapon_Data(1)->Weapon->WarheadPtr->ElectricAssault) { // RA2: an electric-assault unit looks for an adjacent building to overpower
			BuildingClass * building = NULL;
			for (int face = FACING_FIRST; face < FACING_COUNT; face++) { // RA2: the adjacent cells are searched inline here
				Cell cell = Adjacent_Cell(Get_Cell(), (FacingType)face);
				BuildingClass * candidate = Map[cell].Cell_Building();
				if (candidate != NULL && candidate->Class->Overpowerable) {
					HouseClass * owner = Owner_HouseClass();
					if (candidate->Owner_HouseClass() == owner) {
						building = candidate;
						break;
					}
				}
			}
			if (building != NULL) {
				Assign_Target(building);
				IsOverpowering = true;
				QueueMission(MISSION_ATTACK, false);
			} else {
				Random_Animate();
			}
		} else {
			Random_Animate(); // RA2: the nearby-target scan is gone
		}
	} else if (Techno_Type_Class()->HoverAttack && HeightAGL == 0) { // RA2: a hover-attacker on the ground moves to a nearby cell
		Assign_Destination(&Map[Nearby_Location(this)], true);
	}

	int dtime = Current_Mission_Control().Normal_Delay();

	if (this != NULL && RTTI == RTTI_INFANTRY) { // RA2: the infantry test is inline, no cast local

		/*
		**	If this is a bomber type infantry and the current target is a building, then go into
		**	sabotage mode if not already.
		*/
		if (!House->Is_Human_Player() && (((InfantryClass *)this)->Class->IsBomber || Has_Ability(ABILITY_C4)) && Mission != MISSION_SABOTAGE) {
			if (TarCom != NULL && TarCom->RTTI == RTTI_BUILDING) {
				QueueMission(MISSION_SABOTAGE, false); // RA2: QueueMission, not Assign_Mission
			}
		}
	}

	int result = 0; // RA2: one result variable, zero when distributed fire has targets queued
	if (Arm != 0) { // RA2: an arming weapon returns its remaining time
		result = (int)Arm;
	} else if (!Techno_Type_Class()->DistributedFire || CurrentTargets.Count() <= 0) {
		result = dtime + Random_Pick(0, 2);
	}
	return(result);
}



/***********************************************************************************************
 * FootClass::Mission_Hunt -- Handles the default hunt order.                                  *
 *                                                                                             *
 *    This routine is the default hunt order for game objects. It handles searching for a      *
 *    nearby object and heading toward it. The act of targeting will cause it to attack        *
 *    the target it selects.                                                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the game tick delay before calling this routine again.                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
int FootClass::Do_MISSION_HUNT(void)
{
	if (!Techno_Type_Class()->StupidHunt && Target_Something_Nearby(PositionCoord, THREAT_NORMAL)) { // RA2: a StupidHunt type never scans for targets
		InfantryClass * infantry = RTTI == RTTI_INFANTRY ? (InfantryClass *)this : NULL;
		if (infantry != NULL && infantry->Class->IsEngineer && !infantry->Class->IsBomber && !infantry->Has_Ability(ABILITY_C4)) {
			Assign_Destination(TarCom, true); // RA2: Assign_Destination takes the follow flag explicitly, QueueMission replaces Assign_Mission
			QueueMission(MISSION_CAPTURE, false);
			if (Ready_To_Commence()) {
				Commence();
			}
		} else {
			if (infantry != NULL && (infantry->Class->IsBomber || infantry->Has_Ability(ABILITY_C4)) && TarCom != NULL && TarCom->Fetch_RTTI() == RTTI_BUILDING) {
				Assign_Destination(TarCom, true);
				QueueMission(MISSION_SABOTAGE, false);
				if (Ready_To_Commence()) {
					Commence();
				}
			} else if (infantry != NULL && infantry->Class->IsVehicleThief) {
				Assign_Destination(TarCom, true);
				QueueMission(MISSION_CAPTURE, false);
				if (Ready_To_Commence()) {
					Commence();
				}
			} else if (infantry != NULL) {
				Assign_Destination(NULL, true); // RA2: an infantry unit with nothing to do clears its destination first
				Approach_Target();
			} else {
				Approach_Target();
			}
		}
	} else if (!House->Is_Human_Player() && Session.Type == GAME_NORMAL) { // RA2: a computer-controlled unit in a campaign heads for the player's base
		Cell cell = CELL_NONE;
		cell = Cell(PlayerPtr->Center);
		if (cell != CELL_NONE) {
			Assign_Destination(&Map[cell], true);
			QueueMission(MISSION_MOVE, true);
		}
	} else {
		Random_Animate();
	}
	return(Current_Mission_Control().Normal_Delay() + Random_Pick(0, 2));
}



/***********************************************************************************************
 * FootClass::Stop_Driver -- This routine clears the driving state of the object.              *
 *                                                                                             *
 *    This is the counterpart routine to the Start_Driver function. It clears the driving      *
 *    status flags and destination coordinate record.                                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was driving stopped?                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *   12/12/1994 JLB : Greatly simplified.                                                      *
 *=============================================================================================*/
bool FootClass::Stop_Driver(void)
{
	Locomotion->Stop_Moving();
	return(false);
}


/***********************************************************************************************
 * FootClass::Start_Driver -- This starts the driver heading to the destination desired.       *
 *                                                                                             *
 *    Before a unit can move it must be started by this routine. This routine handles          *
 *    reserving the cell and setting the driving flag.                                         *
 *                                                                                             *
 * INPUT:   headto   -- The coordinate of the immediate drive destination. This is one cell    *
 *                      away from the unit's current location.                                 *
 *                                                                                             *
 * OUTPUT:  bool; Was driving initiated?                                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *   12/12/1994 JLB : Uses simple spot index finder.                                           *
 *=============================================================================================*/
bool FootClass::Start_Driver(Coord & headto)
{
	Locomotion->Move_To(headto);
	return(Locomotion->Is_Moving() != 0);
}


/***********************************************************************************************
 * FootClass::Stun -- Prepares a ground travelling object for removal.                         *
 *                                                                                             *
 *    This routine clears the units' navigation computer in preparation for removal from the   *
 *    game. This is probably called as a result of unit destruction in combat. Clearing the    *
 *    navigation computer ensures that the normal AI process won't start it moving again while *
 *    the object is undergoing any death animations.                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/23/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void FootClass::Stun(void)
{
	Assign_Destination(NULL);
	Path[0] = FACING_NONE;
	Stop_Driver();
	BASECLASS::Stun();
}


/// <summary>
/// RA2
/// </summary>
static inline bool Is_Foot_Object(AbstractClass const * object)
{
	if (object == NULL) {
		return(false);
	}
	unsigned char flag = (object->AbstractFlags >> 2) & 1;
	return(flag != 0);
}


/// <summary>
/// RA2
/// </summary>
static inline int Point_Distance(Point2D const & a, Point2D const & b)
{
	Point2D diff = a - b;
	return(diff.Length());
}


/// <summary>
/// RA2
/// </summary>
static inline void Nav_Pop(FootClass * foot)
{
	AbstractClass * target = foot->NavQueue[0];
	if (target != NULL) {
		foot->Assign_Destination(target);
		foot->NavQueue.Delete_Index(0);
		if (foot->IsNavQueueLoop) {
			foot->NavQueue.Add(target);
		}
	}
}


/// <summary>
/// RA2
/// </summary>
static inline int Flat_Distance(Point2D const & a, Point2D const & b)
{
	Point2D diff = a - b;
	double y = diff.Y;
	double x = diff.X;
	return((int)fastmath::sqrt(x*x + y*y));
}


/// <summary>
/// RA2
/// </summary>
static inline int Approach_Distance(Coord const & a, Coord const & b) // RA2: the distance between two coordinates
{
	Coord diff = a - b;
	return(diff.Length());
}


/// <summary>
/// RA2
/// </summary>
// RA2: kept as this file's own helper: Move_Coord takes an int distance, and the computation in place changes Approach_Target's code
static inline Coord Approach_Move_Coord(Coord const & start, DirType const & dir, double distance) // RA2: Move_Coord with a double distance
{
	double radians = dir.As_Radian();
	return(Coord((int)(start.X + fastmath::cos(radians) * distance), (int)(start.Y - fastmath::sin(radians) * distance), start.Z));
}


/***********************************************************************************************
 * FootClass::Approach_Target -- Sets the navigation computer to approach target object.       *
 *                                                                                             *
 *    This routine will set the navigation computer to approach the target indicated by the    *
 *    targeting computer. It is through this function that the unit nears the target so        *
 *    that weapon firing may occur.                                                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/31/1994 JLB : Created.                                                                 *
 *   12/13/1994 JLB : Made part of TechnoClass.                                                *
 *   12/22/1994 JLB : Enhanced search algorithm.                                               *
 *   05/20/1995 JLB : Always approaches if the object is off the map.                          *
 *=============================================================================================*/
void FootClass::Approach_Target(void)
{
	if (TarCom != NULL) { // RA2: no IsJellyfish (not in RA2's layout)
		int primary = What_Weapon_Should_I_Use(TarCom);

		int maxrange = Weapon_Range(primary);
		if (maxrange >= 512) { // RA2: a long range weapon keeps half a cell in hand
			maxrange -= 128;
		}
		bool inrange = In_Range(TarCom, primary);

		if (!inrange) { // RA2: an out-of-range target is only chased by units that may approach
			if (CurrentMission == MISSION_STICKY) {
				Assign_Target(NULL);
				Assign_Destination(NULL);
				return;
			}
			if (!TClass->CanApproachTarget) {
				bool pursue = false;
				if (CurrentMission == MISSION_ATTACK) pursue = true;
				if (CurrentMission == MISSION_GUARD_AREA && House->Is_Human_Player()) pursue = true;
				if ((CurrentMission != MISSION_HUNT || House->Is_Human_Player()) && !pursue) {
					Assign_Target(NULL);
					Assign_Destination(NULL);
					return;
				}
			}
		}

		bool checkbuild = false;
		if (RTTI == RTTI_UNIT && ((UnitClass *)this)->Class->IsDeployToFire) {
			if (!Map[(Coord const &)PositionCoord].Can_Build_Here()) {
				checkbuild = true;
				int dist = Distance(TarCom) + 2 * CELL_LEPTON;
				if (maxrange >= dist) {
					maxrange = dist;
				}
			}
		}

		bool flyer = (RTTI == RTTI_AIRCRAFT);

		CLSID clsid;
		IPersistPtr persist(Locomotion);
		persist->GetClassID(&clsid);
		if (clsid == CLSID_JumpjetLocomotion) {
			flyer = true;
		}

		if (NavCom != NULL) { // RA2: a navigation target that is too far from the target is dropped and found again
			if (TarCom != NULL && TClass->CanRecalcApproachTarget && !inrange) {
				int separation = Approach_Distance(NavCom->Center_Coord(), TarCom->Center_Coord());
				if (separation > Rule->ApproachTargetResetMultiplier * maxrange) {
					FollowTarget = NULL;
					NavCom = NULL;
				}
			}
		}

		if ((NavCom == NULL || In_Air()) && (!inrange || !IsLocked || checkbuild)) {

			if (RouteQueue.Count() > 0) {
				Assign_Destination(RouteQueue[0], false);
				RouteQueue.Delete_Index(0);
				return;
			}

			if (TarCom->RTTI == RTTI_CELL) {
				CellClass * cellptr = (CellClass *)TarCom;
				if (cellptr->Is_Tile_Destroyable_Cliff()) {
					Cell cell = Map.Closest_Passable_Cell(cellptr->CellID, Destination_Coord().As_Cell());
					Assign_Target(&Map[cell]);
				}
			}

			if (TarCom->RTTI == RTTI_BUILDING) {
				BuildingClass * obj = (BuildingClass *)TarCom;
				if (obj != NULL) {
					maxrange += ((obj->Class->Width() + obj->Class->Height()) * (CELL_LEPTON / 4));
				}
			}

			double adjust;
			if (RTTI == RTTI_INFANTRY) {
				adjust = (double)maxrange - 281.6;
			} else {
				adjust = (double)maxrange - 179.2;
			}
			maxrange = (int)adjust;
			maxrange = (maxrange <= 0) ? 0 : maxrange;
			if (maxrange > 0 && maxrange <= 255) {
				maxrange = 255;
			}

			Coord tcoord = TarCom->Center_Coord();
			Coord trycoord;
			Cell tcell = tcoord.As_Cell();
			Cell trycell = tcell;

			int dir = ::Direction(tcoord, Center_Coord()).As_Dir256(); // RA2: the direction from the target to this object

			bool tarbridge = false;
			if (TarCom->RTTI == RTTI_CELL) {
				tarbridge = ((CellClass *)TarCom)->IsUnderBridge;
			} else {
				AbstractClass * target = TarCom; // RA2: the object test is the abstract's flag
				if (target && (target->AbstractFlags & ABSTRACT_OBJECT)) {
					tarbridge = ((ObjectClass *)target)->IsOnBridge;
				}
			}

			MZoneType mzone = TClass->MZone;
			bool found = false;
			double floating_radius; // RA2: the search radius is a double
			int range = maxrange;
			if ((double)range > 204.8) { // RA2: the sweep only runs for a radius above one cell
				floating_radius = (double)range;
				do {
					for (int index = 0; index < 25; index++) { // RA2: 25 angles in the table at 0x7D8178
						static int _approach_angles[25] = { 0, 1, -1, 2, -2, 3, -3, 4, -4, 5, -5, 6, -6, 8, -8, 16, -16, 24, -24, 32, -32, 48, -48, 64, -64 };
						DirType angle((Dir256)(dir + _approach_angles[index]));
						trycoord = Coord(Approach_Move_Coord(tcoord, angle.Snap_To_256(), floating_radius).As_Cell(), 0);

						if (checkbuild && !Map[trycoord].Can_Build_Here()) continue;

						trycoord.Z = Map.Get_Height_GL(trycoord);
						if (Map[trycoord].IsUnderBridge) {
							trycoord.Z += BRIDGE_LEPTON_HEIGHT;
						}

						if (!In_Range(&trycoord, TarCom, Get_Class_Weapon_Data(primary)->Weapon)) continue; // RA2: the member test, not the type's

						trycell = trycoord.As_Cell();
						if (!Map.In_Local_Radar(trycell, true)) continue;

						bool clear = Map[trycell].Is_Clear_To_Move(TClass->Speed, false, false, Map.Get_Cell_Zone(Destination_Coord().As_Cell(), mzone, IsOnBridge), mzone, -1, true);
						if (!clear) { // RA2: a blocked cell is retried around its neighbors
							if (RTTI == RTTI_INFANTRY) continue;
							Coord savedcoord = trycoord;
							Cell savedcell = trycell;
							for (int adjacent = 0; adjacent < 8; adjacent++) {
								trycell = AdjacentCell[adjacent & 7] + trycell;
								trycoord = Coord(trycell);
								if (In_Range(&trycoord, TarCom, Get_Class_Weapon_Data(primary)->Weapon) && Map[trycell].Is_Clear_To_Move(TClass->Speed, false, false, Map.Get_Cell_Zone(Destination_Coord().As_Cell(), mzone, IsOnBridge), mzone, -1, true)) {
									clear = true;
									break;
								}
							}
							if (!clear) {
								trycoord = savedcoord;
								trycell = savedcell;
								continue;
							}
						}

						int dy = abs((int)trycell.Y - tcell.Y);
						int dx = abs((int)trycell.X - tcell.X);
						int maxdist = dx > dy ? dx : dy;
						CellClass * cellptr = &Map[trycell];
						if (!flyer && Search.Test_Cell_Walk(trycell, tcell, this, cellptr->IsUnderBridge, tarbridge, MZONE_NONE) > maxdist + 8) {
							Cell destcell = Destination_Coord().As_Cell();
							int destdy = abs((int)destcell.Y - trycell.Y);
							int destdx = abs((int)destcell.X - trycell.X);
							int destdist = destdx > destdy ? destdx : destdy;
							if (Search.Test_Cell_Walk(destcell, trycell, this, Is_Moving_Onto_Bridge(), cellptr->IsUnderBridge, MZONE_NONE) > destdist + 8) continue;
						}
						found = true;
						break;
					}
					if (found) break;
					range -= 256;
					floating_radius = (double)range;
				} while (floating_radius > 204.8);
			}

			if (found) {
				Assign_Destination(&Map[trycell]);
			} else if (TClass->IsHunterSeeker || (RTTI == RTTI_INFANTRY && ((InfantryClass *)this)->Class->IsVehicleThief)) {
				Assign_Destination(TarCom);
			} else {
				trycell = Map.Nearby_Location(trycell,
					TClass->Speed,
					Map.Get_Cell_Zone(Destination_Coord().As_Cell(), mzone, Is_Moving_Onto_Bridge()),
					mzone,
					Map[trycell].IsUnderBridge,
					Point2D(1, 1),
					false,
					true,
					false,
					false);
				Assign_Target(NULL);

				if (trycell != CELL_NONE) {
					Assign_Destination(&Map[trycell]);
				} else {
					Assign_Destination(NULL);
				}
			}
		}
	}
}



/// <summary>
/// RA2
/// </summary>
static inline void Guard_Navigation(FootClass * foot) // RA2: the navigation queue step is its own inline function (one more inline layer)
{
	if (!foot->House->Is_Human_Player()) {
		/*
		**	The navigation queue only needs to be processed if there is
		**	currently no navigation target for this object.
		*/
		if (foot->NavQueue.Count() > 0 && foot->NavCom == NULL && foot->NavQueue[0] == foot->ArchiveTarget) {
			AbstractClass * target = foot->NavQueue[0];

			/*
			**	Check to see if the navigation queue even exists and
			**	has at least one valid entry. If it does, then process it by
			**	assigning the object's NavCom to the first entry on the list.
			*/
			if (target != NULL) {
				foot->Assign_Destination(target, true); // RA2: the follow flag is explicit
				foot->NavQueue.Delete_Index(0);

				/*
				**	If the navigation queue is to loop (indefinately), then append the
				**	target value from the first part to the end of the queue.
				*/
				if (foot->IsNavQueueLoop) {
					foot->NavQueue.Add(target);
				}
			}
		}
	}
}



/***********************************************************************************************
 * FootClass::Mission_Guard_Area -- Causes unit to guard an area about twice weapon range.     *
 *                                                                                             *
 *    This mission routine causes the unit to scan for targets out to twice its weapon range   *
 *    from the home point. If a target was found, then it will be attacked. The unit will      *
 *    chase the target until it gets up to to its weapon range from the home position.         *
 *    In that case, it will return to home position and start scanning for another target.     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with time delay before calling this routine again.                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/23/1994 JLB : Created.                                                                 *
 *   07/27/1995 JLB : Greatly simplified.                                                      *
 *=============================================================================================*/
int FootClass::Do_MISSION_GUARD_AREA(void)
{
	Guard_Navigation(this);

	if (Is_Target_Cell(ArchiveTarget)) {
		BuildingClass * building = Map[ArchiveTarget->Center_Coord()].Cell_Building();
		if (building != NULL && House->Is_Ally(building->House) && !House->Is_Human_Player() && (RTTI != RTTI_UNIT || !static_cast<UnitClass *>(this)->Class->IsToHarvest)) {
			Cell nearby = Map.Nearby_Location((Cell)ArchiveTarget->Center_Coord(), SPEED_TRACK, Map.Get_Cell_Zone((Cell)PositionCoord), MZONE_NORMAL, false, Point2D(1, 1), false, false, false, true, Cell(0, 0), false); // RA2: the extra arguments are spelled out
			if (nearby != CELL_NONE) {
				ArchiveTarget = &Map[nearby];
			}
		}
	}

	if (RTTI == RTTI_UNIT && static_cast<UnitClass *>(this)->Class->IsToHarvest) {
		QueueMission(MISSION_HARVEST, false); // RA2: QueueMission, not Assign_Mission
		Commence();
		return(1+Random_Pick(1, 10));
	}

	/*
	**	Ensure that the archive target is valid.
	*/
	if (ArchiveTarget == NULL && MissionQueue == MISSION_NONE) {
		ArchiveTarget = &Map[(Coord const &)PositionCoord];
	}

	/*
	**	If this is a bomber type infantry and the current target is a building, then go into
	**	sabotage mode if not already.
	*/
	InfantryClass * infantry = (this == NULL) ? NULL : (RTTI == RTTI_INFANTRY) ? (InfantryClass *)this : NULL; // RA2: the infantry cast is inline
	if (!House->Is_Human_Player() && infantry != NULL && (infantry->Class->IsBomber || infantry->Has_Ability(ABILITY_C4)) && Mission != MISSION_SABOTAGE && TarCom != NULL && TarCom->Fetch_RTTI() == RTTI_BUILDING) { // RA2: the building test is inline
		QueueMission(MISSION_SABOTAGE, false);
		return(1);
	}

	/*
	**	Make sure that the unit has not strayed too far from the home position.
	**	If it has, then race back to it.
	*/
	int maxrange = (int)(Threat_Range(1) * 0.75);

	AbstractClass * archive = ArchiveTarget; // RA2: the home position's foot flag goes through a local
	bool foot;
	if (archive != NULL) {
		foot = (archive->AbstractFlags & ABSTRACT_FOOT);
	}
	if (archive != NULL && foot) { // RA2: a Foot object as home position limits the stray distance to the rules value
		maxrange = Rule->GuardModeStray;
	}

	if (ArchiveTarget != NULL) {
		if (!IsFiring && NavCom == NULL && Distance(ArchiveTarget) > maxrange) {
			Assign_Target(NULL);
			Assign_Destination(ArchiveTarget, true); // RA2: the follow flag is explicit
		}

		if (TarCom == NULL) {
			if (Can_Scan_For_Target() && TargetingTimerFinished()) { // RA2: the scan waits for the targeting timer
				Target_Something_Nearby(ArchiveTarget->Center_Coord(), THREAT_AREA);
			}
			if (TarCom != NULL) {
				return(1);
			}
			Random_Animate();
		} else {
			Approach_Target();
		}
	}

	if (TarCom == NULL && Get_Class_Weapon_Data(1)->Weapon != NULL && Get_Class_Weapon_Data(1)->Weapon->WarheadPtr->ElectricAssault) { // RA2: an electric-assault unit looks for an adjacent building to overpower
		for (int face = FACING_FIRST; face < FACING_COUNT; face++) {
			BuildingClass * candidate = Map[Adjacent_Cell(Get_Cell(), (FacingType)face)].Cell_Building(); // RA2: the adjacent cell is indexed directly
			if (candidate != NULL && candidate->Class->Overpowerable && candidate->Owner_HouseClass() == Owner_HouseClass()) {
				if (candidate != NULL) {
					Assign_Target(candidate);
					IsOverpowering = true;
					QueueMission(MISSION_ATTACK, false);
				}
				break;
			}
		}
	}

	int dtime = Current_Mission_Control().Normal_Delay();
	if (RTTI == RTTI_AIRCRAFT) {
		dtime *= 2;
	}
	return(dtime + Random_Pick(1, 5));
}



/***********************************************************************************************
 * FootClass::Unlimbo -- Unlimbos object and performs special fixups.                          *
 *                                                                                             *
 *    This routine will make sure that the home position for the foot class object gets        *
 *    reset. This is necessary since the home position may change depending on the unit's      *
 *    transition between limbo and non-limbo condition.                                        *
 *                                                                                             *
 * INPUT:   coord    -- The coordinate to unlimbo the unit at.                                 *
 *                                                                                             *
 *          dir      -- The initial direction to give the unit.                                *
 *                                                                                             *
 * OUTPUT:  bool; Was the unit unlimboed successfully?                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/23/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool FootClass::Unlimbo(Coord const & coord, Dir256 dir)
{
	/*
	**	Try to unlimbo the unit.
	*/
	if (BASECLASS::Unlimbo(coord, dir)) {

		Locomotion->Unlimbo();

		if (House->Ion_Sensitives_Should_Be_Offline() && Locomotion->Is_Ion_Sensitive()) { // RA2: the house decides (always false in RA2), no ion storm test here
			Locomotion->Power_Off();
		} else {
			Locomotion->Power_On();
		}

		/*
		**	Mobile units are always revealed to the house that owns them.
		*/
		Revealed(House);

		/*
		**	Start in a still (non-moving) state.
		*/
		Path[0] = FACING_NONE;

		Cell cell = Get_Cell(); // RA2: the virtual cell query, not the PositionCell member
		for (int face = FACING_FIRST; face < FACING_COUNT; face++) {
			Cell c = AdjacentCell[face & 7] + cell; // RA2: the table entry is the left operand of the sum, not Adjacent_Cell's cell + entry
			CellClass *cptr = &Map[c];
			cptr->AdjacentObjectCount++;
		}

		if (!In_Air()) {
			LastAdjacencyCell = cell;
		} else if (TClass->ConsideredAircraft) { // RA2: an airborne object of an aircraft-like type is tracked
			AircraftTracker.Add(this);
		}

		double avoidance = TClass->ThreatAvoidanceCoefficient;
		ThreatAvoidanceCoefficient = avoidance;
		if (TClass->SensorsSight != 0) { // RA2: a sensor-carrying type registers its sensor cells
			AddSensorsAt(CELL_NONE);
		}
		return(true);
	}
	return(false);
}



/***********************************************************************************************
 * FootClass::Take_Damage -- Handles taking damage to this object.                             *
 *                                                                                             *
 *    This routine intercepts the damage assigned to this object and if this object is         *
 *    a member of a team, it informs the team that the damage has occurred. The team may       *
 *    change it's priority or action based on this event.                                      *
 *                                                                                             *
 * INPUT:   damage      -- The damage points inflicted on the unit.                            *
 *                                                                                             *
 *          distance    -- The distance from the point of damage to the unit itself.           *
 *                                                                                             *
 *          warhead     -- The type of damage that is inflicted.                               *
 *                                                                                             *
 *          source      -- The perpetrator of the damage. By knowing who caused the damage,    *
 *                         the team know's who to "get even with".                             *
 *                                                                                             *
 * OUTPUT:  Returns with the result type of the damage.                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/30/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
ResultType FootClass::Take_Damage(int & damage, int distance, WarheadTypeClass const * warhead, TechnoClass * source, bool forced, bool no_crew)
{
	if (warhead->Sonic && ParasiteEatingMe != NULL) { // RA2: a sonic hit makes the parasite eating this object let go
		ParasiteEatingMe->ParasiteImUsing->ExitUnit();
		if (source != NULL) {
			source->Assign_Target(NULL);
		}
	}

	if (ParasiteEatingMe != NULL && source != ParasiteEatingMe && damage > ParasiteEatingMe->Techno_Type_Class()->SuppressionThreshold) { // RA2: heavy damage from someone else suppresses the parasite
		ParasiteEatingMe->ParasiteImUsing->SuppressionTimer = damage * 2 - ParasiteEatingMe->Techno_Type_Class()->SuppressionThreshold;
	}

	if (ParasiteEatingMe != NULL && damage < 0) { // RA2: repair suppresses the parasite and frees the victim
		ParasiteEatingMe->ParasiteImUsing->SuppressionTimer = 50;
		ParasiteEatingMe->ParasiteImUsing->ExitUnit();
	}

	ResultType result = BASECLASS::Take_Damage(damage, distance, warhead, source, forced, no_crew);

	if (result == RESULT_ALREADY_DESTROYED) {
		return(result);
	}

	if (result != RESULT_NONE && Team) {

		Team->Took_Damage(this, result, source);

	} else {

		if (result != RESULT_DESTROYED && result != RESULT_NONE) {

			/*
			**	Determine if the target that is currently being attacked has a weapon that can
			**	do harm to a ground based unit. This information is needed so that an appropriate
			**	response will occur when damage is taken.
			*/
//			bool tweap = false;
//			if (As_Techno(TarCom)) {
//				tweap = (As_Techno(TarCom)->TClass->PrimaryWeapon != NULL);
//			}

			if (Team != NULL && Team->Class->IsWhiner && !House->Is_Human_Player()) {
				if (source == NULL) {
					return(result);
				}
				Base_Is_Attacked(source);
			}

			/*
			**	This ensures that if a unit is in sticky mode, then it will snap out of
			**	it when it takes damage.
			*/
			if (source != NULL && Current_Mission_Control().IsNoThreat && !Current_Mission_Control().IsZombie) {
				Enter_Idle_Mode();
			}

			#if 0
			/*
			**	If this object is not part of a team and it can retaliate for the damage, then have
			**	it try to do so. This prevents it from just sitting there and taking damage.
			*/
			if (Is_Allowed_To_Retaliate(source)) {

				int primary = What_Weapon_Should_I_Use(source->As_Target());
				if (In_Range(source, primary) || !House->IsHuman) {
					Assign_Target(source->As_Target());
				}

				if (Mission == MISSION_AMBUSH) {
					Assign_Mission(MISSION_HUNT);
				}

				/*
				**	Simple retaliation cannot occur because the source of the damage
				**	is too far away. If scatter logic is enabled, then scatter now.
				*/
				if (TarCom == NULL && NavCom == NULL && Rule->IsScatter) {
					Scatter(COORD_NONE, true);
				}

			} else {

				/*
				**	If this object isn't doing anything important, then scatter.
				*/
				if (Current_Mission_Control().IsScatter && !IsTethered && !IsDriving && TarCom == NULL && NavCom == NULL && RTTI != RTTI_AIRCRAFT && RTTI != RTTI_VESSEL) {
					if (!House->IsHuman || Rule->IsScatter) {
						Scatter(COORD_NONE, true);
					}
				}
			}
			#endif
		}
	}
	return(result);
}



/***********************************************************************************************
 * FootClass::Active_Click_With -- Initiates attack or move according to target clicked on.    *
 *                                                                                             *
 *    At this level, the object is known to have the ability to attack or move to the          *
 *    target specified (in theory). Perform the attack or move as indicated.                   *
 *                                                                                             *
 * INPUT:   target   -- The target clicked upon that will precipitate action.                  *
 *                                                                                             *
 * OUTPUT:  Returns with the type of action performed.                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/06/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool FootClass::Active_Click_With(ActionType action, ObjectClass * object, bool is_waypoint)
{

	switch (action) {
		case ACTION_GUARD_AREA:
			if (Can_Player_Fire() && Can_Player_Move()) {
				if (((Fetch_RTTI() == RTTI_INFANTRY && ((InfantryClass *)this)->Class->IsBomber) || Has_Ability(ABILITY_C4))
                && object->Fetch_RTTI() == RTTI_BUILDING && !House->Is_Ally((TechnoClass const *)object)) {
					Player_Assign_Mission(MissionType(MISSION_SABOTAGE), 0, object, 0);
				} else {
					Player_Assign_Mission(MissionType(MISSION_GUARD_AREA), object, 0, 0);
				}
				return true;
			}
			break;

		case ACTION_SELF:
		case ACTION_AREA_ATTACK:
		case ACTION_DETONATE_ALL:
			Player_Assign_Mission(MissionType(MISSION_UNLOAD), 0, 0, 0);
			return true;

		case ACTION_DETONATE:
			OutList.Add(EventClass(AttachedBomb->OwnerHouse->HeapID, EventClass::DETONATE, TargetClass(this)));
			return true;

		case ACTION_ATTACK:
		case ACTION_IVAN_BOMB:
		case ACTION_ATTACK_MOVE_TAR:
		case ACTION_DEMOLISH:
			if (Can_Player_Fire()) {
				Player_Assign_Mission(MissionType(MISSION_ATTACK), object, 0, 0);
				return true;
			}
			break;

		case ACTION_ENTER:
			if (Can_Player_Move() && object && Dynamic_Cast<TechnoClass *>(object)) {
				Player_Assign_Mission(MissionType(MISSION_ENTER), 0, object, 0);
				return true;
			}
			break;

		case ACTION_CAPTURE:
			if (Can_Player_Move()) {
				Player_Assign_Mission(MissionType(MISSION_CAPTURE), 0, object, 0);
				return true;
			}
			break;

		case ACTION_EATEN:
			if (Can_Player_Move()) {
				Player_Assign_Mission(MissionType(MISSION_EATEN), 0, object, 0);
				return true;
			}
			break;

		case ACTION_SABOTAGE:
			if (Can_Player_Move()) {
				Player_Assign_Mission(MissionType(MISSION_SABOTAGE), 0, object, 0);
				return true;
			}
			break;

		case ACTION_NOMOVE:
			if (Map.Is_Shrouded(object->Center_Coord()) && !Techno_Type_Class()->IsMoveToShroud) {
				return false;
			}
        // Fall into next case.

		case ACTION_MOVE:
		case ACTION_ATTACK_MOVE_NAV:
			if (Can_Player_Move()) {
				AbstractClass * targ = object;

				Coord center = object->Center_Coord();
				if (center.Z <= Map.Get_Height_GL(center)) {
					center.Z = Map.Get_Height_GL(center);
				}

				if (((Techno_Type_Class()->IsSubterranean && !Rule->IsShroudedSubteranneanMovesAllowed) || Fetch_RTTI() == RTTI_AIRCRAFT) && Map.Is_Shrouded(center)) return true;

				MZoneType mzone = Techno_Type_Class()->MZone;
				Cell cell = object->Destination_Coord(0).As_Cell();
				bool in_radar = Map.In_Local_Radar(cell, true);
				CellClass * cptr = &Map[Destination_Coord(0)];

				if (mzone == MZONE_SUBTERANNEAN) mzone = MZONE_NORMAL;
				else if (mzone == MZONE_FLYER) mzone = MZONE_INFANTRY;

				bool moveanywhere = In_Air() || Techno_Type_Class()->IsSubterranean || (Fetch_RTTI() == RTTI_INFANTRY && ((InfantryClass *)this)->Class->JumpJet) || Fetch_RTTI() == RTTI_AIRCRAFT;

				bool ontobridge = Is_Moving_Onto_Bridge(0);
				Coord coord = Coord(cell.X * 256 + 128, cell.Y * 256 + 128, 0);
				coord.Z = Map.Get_Height_GL(coord);
				bool shrouded = Map.Is_Shrouded(coord);
				bool forcemove = !is_waypoint && (Keyboard->Down(Options.KeyForceMove1) || Keyboard->Down(Options.KeyForceMove2));

				if (moveanywhere && action == ACTION_NOMOVE) {
					Cell nearby = Map.Nearby_Location(cell, Techno_Type_Class()->Speed, -1, mzone, Map[cell].IsUnderBridge, Point2D(1, 1), 0, 1, Techno_Type_Class()->IsSubterranean && HeightAGL < 0, 1, Cell(0, 0), 0);
					if (nearby != CELL_NONE) {
						targ = &Map[nearby];
					}
				} else {
					if (!in_radar || !moveanywhere && (shrouded || action == ACTION_NOMOVE || forcemove && Fetch_RTTI() == RTTI_INFANTRY || !Map.Is_Same_Cell_Zone(cptr->CellID, cell, mzone, ontobridge, Map[cell].IsUnderBridge, false))) {
						Cell nearby = Map.Nearby_Location(cell, Techno_Type_Class()->Speed, Map.Get_Cell_Zone(cptr->CellID, mzone, ontobridge), mzone, Map[cell].IsUnderBridge, Point2D(1, 1), 0, 1, 0, 1, Cell(0, 0), 0);
						if (nearby != CELL_NONE) {
							targ = &Map[nearby];
						}
					}
				}

				if (targ != 0) {
					CellClass * from = 0;
					if (targ != object && object != 0) {
						from = &Map[object->Center_Coord().As_Cell()];
					}
					Player_Assign_Mission(MissionType(MISSION_MOVE), 0, targ, from);
				}
				return true;
			}
			break;

		case ACTION_PATROL_WAYPOINT:
		case ACTION_NO_IVAN_BOMB:
			return false;

		case ACTION_ATTACK_SUPPORT: {
			WeaponDataStruct const * weapon = Get_Class_Weapon_Data(0);
			bool heals = (weapon != 0 && weapon->Weapon != 0 && weapon->Weapon->Attack < 0);
			Player_Assign_Mission(heals ? MISSION_GUARD_AREA : MISSION_GUARD, this, 0, 0);
			return true;
		}

		default:
			return true;
	}
	return false;
}


















































































































;


#ifdef _DEBUG
/***********************************************************************************************
 * FootClass::Debug_Dump -- Displays the status of the FootClass to the mono monitor.          *
 *                                                                                             *
 *    This routine is used to output the current status of the foot class to the mono          *
 *    monitor. Through this display bugs may be tracked down or eliminated.                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/02/1994 JLB : Created.                                                                 *
 *   07/04/1995 JLB : Handles aircraft special case.                                           *
 *=============================================================================================*/
void FootClass::Debug_Dump(MonoClass * mono) const
{
	mono->Fill_Attrib(53, 13, 12, 1, IsInitiated ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(1, 18, 12, 1, IsPlanningToLook ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(53, 14, 12, 1, IsDeploying ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(53, 15, 12, 1, IsFiring ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(53, 16, 12, 1, IsRotating ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(53, 18, 12, 1, IsUnloading ? MonoClass::INVERSE : MonoClass::NORMAL);

	mono->Set_Cursor(45, 1);mono->Printf("%02X", Speed);
	if (NavCom) {
		mono->Set_Cursor(29, 5);
		mono->Printf("%08X", NavCom);
	}
	if (SuspendedNavCom) {
		mono->Set_Cursor(38, 5);
		mono->Printf("%08X", SuspendedNavCom);
	}

	if (Team) Team->Debug_Dump(mono);
	if (Group != 255) {
		mono->Set_Cursor(59, 1);mono->Printf("%d", Group);
	}

	static char	const * _p2c[9] = {"-","0","1","2","3","4","5","6","7"};
	for (int index = 0; index < MIN(12, ARRAY_SIZE(Path)); index++) {
		mono->Set_Cursor(54+index, 3);
		mono->Printf("%s", _p2c[((abs((int)Path[index]+1)) % ARRAY_SIZE(_p2c))]);
	}
	mono->Set_Cursor(72, 3);mono->Printf("%4d", (int)PathDelay);
	mono->Set_Cursor(67, 3);mono->Printf("%3d", TryTryAgain);
	if (HeadToCoord != COORD_NONE) {
		mono->Set_Cursor(54, 5);mono->Printf("%5d,%5d,%3d", HeadToCoord.X, HeadToCoord.Y, HeadToCoord.Z);
	}

	BASECLASS::Debug_Dump(mono);
}
#endif



/***********************************************************************************************
 * FootClass::Active_Click_With -- Performs action as a result of left mouse click.            *
 *                                                                                             *
 *    This routine performs the action requested when the left mouse button was clicked over   *
 *    a cell. Typically, this is just a move command.                                          *
 *                                                                                             *
 * INPUT:   action   -- The predetermined action that should occur.                            *
 *                                                                                             *
 *          cell     -- The cell number that the action should occur at.                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool FootClass::Active_Click_With(ActionType action, Cell const & cell, Cell const & from, bool is_waypoint) // RA2: a second cell, handed on as the source of the move
{
	action = What_Action(cell, false, is_waypoint);

	if (!Map.In_Local_Radar(cell) && action != ACTION_NOMOVE) {
		return(false);
	}

	switch (action) {
		case ACTION_PATROL_WAYPOINT:
			if (Can_Player_Move()) {
				Cell moveto = Move_Order(cell, false);
				Player_Assign_Mission(MISSION_PATROL, NULL, &Map[moveto]);
				break;
			}
			return(false);

		case ACTION_GUARD_AREA:
			if ((Can_Player_Fire() || Is_Renovator()) && Can_Player_Move()) {
				Player_Assign_Mission(MISSION_GUARD_AREA, &Map[cell]);
				break;
			}
			return(false);

		case ACTION_HARVEST:
			Player_Assign_Mission(MISSION_HARVEST, NULL, &Map[cell]);
			break;

		case ACTION_MOVE:
		case ACTION_ATTACK_MOVE_NAV: // RA2: the attack-move order shares the move handling
			if (AllowVoice) {
				Coord coord(cell.X * 256 + 128, cell.Y * 256 + 128, 0); // RA2: the coordinate is built out of line from the cell's center
				coord.Z = Map.Get_Height_GL(coord);
				if (Map[coord].IsUnderBridge) {
					coord.Z += BRIDGE_LEPTON_HEIGHT;
				}
				int scenid = Scen->UniqueID;
				if (Session.Type != GAME_NORMAL && Session.Type != GAME_SKIRMISH) {
					Scen->UniqueID = -3;
					((AnimTypeClass *)Rule->MoveFlash)->YSortAdjust = -5000;
				}
				AnimClass * moveflash = new AnimClass(Rule->MoveFlash, coord);
				if (Session.Type != GAME_NORMAL && Session.Type != GAME_SKIRMISH) {
					Scen->UniqueID = scenid;
					Anims.Delete(moveflash);
					MoveFlashes.Add(moveflash);
				}
			}
			// Fall into next case.

		case ACTION_NOMOVE: {
			Cell moveto = Move_Order(cell, false);
			if (moveto != CELL_NONE) {
				Player_Assign_Mission(MISSION_MOVE, NULL, &Map[moveto], &Map[from]); // RA2: the fourth argument is the second cell's map cell
				break;
			}
		}
		return(false);

		case ACTION_ATTACK:
		case ACTION_ATTACK_MOVE_TAR: { // RA2: the attack-move target order shares the attack handling
			VeinholeMonsterClass * veinhole = VeinholeMonsterClass::Get_Monster_At(cell);
			if (veinhole != NULL && !veinhole->IsDead && veinhole->Strength > 0) {
				Player_Assign_Mission(MISSION_ATTACK, veinhole);
				break;
			} else {
				Player_Assign_Mission(MISSION_ATTACK, &Map[cell]);
				break;
			}
		}
		return(false);

		/*
		**	Engineer attempting to capture bridge to repair it
		*/
		case ACTION_CAPTURE:
			if (Can_Player_Move()) {
				Player_Assign_Mission(MISSION_CAPTURE, NULL, &Map[cell]);
				break;
			}
			return(false);

		case ACTION_ENTER:
			if (Can_Player_Move()) {
				Player_Assign_Mission(MISSION_ENTER, NULL, &Map[cell]);
				break;
			}
			return(false);

		case ACTION_SABOTAGE:
			Player_Assign_Mission(MISSION_SABOTAGE, NULL, &Map[cell] );
			break;

		case ACTION_ENTER_TUNNEL: {
			TubeClass * tunnel = Map[cell].Get_Tunnel();
			TubeClass * exittunnel = Map[tunnel->Get_Exit()].Get_Tunnel();
			Cell exit = Adjacent_Cell(tunnel->Get_Exit(), Facing_Sub(exittunnel->EnterDir, FACING_180));
			Player_Assign_Mission(MISSION_MOVE, NULL, &Map[exit]);
			break;
		}
	}
	return(true);
}




/// <summary>
/// Puts the object into its idle state now that it has run out of orders.
/// This routine will end any piggyback locomotion and then look around for further work --
/// a queued route destination, a waypoint path to resume, or an archived target to head back
/// to -- before finally bringing the object to a halt.
/// </summary>
/// <param name="resume_waypoint">Should an interrupted waypoint path be resumed?</param>
/// <returns>bool; Was other work taken up instead of going idle?</returns>
bool FootClass::Enter_Idle_Mode(bool initial, bool resume_waypoint) // RA2: both arguments passed on
{
	if (!IsIdle) {
		IsIdle = true;
		TechnoClass::Enter_Idle_Mode(initial, resume_waypoint); // RA2

		if (IsToScatter) {
			IsToScatter = false;
			Scatter(COORD_NONE, true, false); // RA2: nokidding false
		}

		bool was_piggybacking = false;
		IPiggybackPtr piggy(Locomotion);
		if (piggy != NULL && piggy->Is_Ok_To_End()) {
			piggy->End_Piggyback(&Locomotion);
			was_piggybacking = true;
		}

		if (RouteQueue.Count() > 0) {
			Assign_Destination(RouteQueue[0], false);
			RouteQueue.Delete_Index(0);
			return(true);
		}

		if (CurrentMission != MISSION_PATROL || Status == 0) {
			if (CurrentPath != PATH_NONE && resume_waypoint) {
				WaypointClass * wp = PlayerPtr->Paths[CurrentPath]->Get_Waypoint(NextWaypoint);
				WaypointClass * next_wp = PlayerPtr->Paths[CurrentPath]->Get_Next_Waypoint(wp);
				Execute_Waypoint_Path(next_wp);
			}
		}

		if (was_piggybacking) {
			return(true);
		}

		if (RTTI == RTTI_INFANTRY && ArchivedTarget != NULL) { // RA2: the archived target read directly
			if (ArchivedTarget->Center_Coord().As_Cell() != Center_Coord().As_Cell()) { // RA2: only when not already there
				AbstractClass * target = ArchivedTarget;
				if (CurrentMission != MISSION_GUARD_AREA) {
					QueueMission(MISSION_MOVE, false); // RA2: queued
					Assign_Archive_Target(NULL);
				}
				Assign_Destination(target, true);
			}
		}

		Set_Speed(0);
	}

	return(false);
}


/// <summary>
/// Fetches the spot this object is heading for when it attacks: unless it is on an attack
/// mission, the object it follows or else its navigation target; otherwise the plain technology
/// object's answer.
/// </summary>
Coord FootClass::GetAttackCoordinates(void) const // RA2: not in TibSun, at 0x4C71B0
{
	if (Get_Mission() != MISSION_ATTACK) {
		if (FollowTarget != NULL) {
			return(FollowTarget->As_Coord());
		}
		if (NavCom != NULL) {
			return(NavCom->As_Coord());
		}
	}
	return(TechnoClass::GetAttackCoordinates());
}





/***********************************************************************************************
 * FootClass::Per_Cell_Process -- Perform action based on once-per-cell condition.             *
 *                                                                                             *
 *    This routine is called as this object moves from cell to cell. When the center of the    *
 *    cell is reached, check to see if any trigger should be sprung. For moving units, reduce  *
 *    the path to the distance to the target. This forces path recalculation in an effort to   *
 *    avoid units passing each other.                                                          *
 *                                                                                             *
 * INPUT:   why   -- Specifies the circumstances under which this routine was called.          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *   07/08/1995 JLB : Handles generic enter trigger event.                                     *
 *   07/16/1995 JLB : If next to a scanner and cloaked, then shimmer.                          *
 *=============================================================================================*/
void FootClass::Per_Cell_Process(PCPType why)
{
	if (why == PCP_END) {

		IsScattering = false;

		/*
		**	Clear any unloading flag if necessary.
		*/
		IsUnloading = false;

		if (TClass->SensorsSight != 0) { // RA2: a sensor unit moves its sensed cells
			RemoveSensorsAt(LastAdjacencyCell);
			AddSensorsAt(CELL_NONE);
		}

		if (LastAdjacencyCell != Cell(0, 0)) {
			if (Map.Cell_Region(LastAdjacencyCell) != Map.Cell_Region(PositionCell)) {
				Remove_Cell_Threat(&Map[LastAdjacencyCell]); // RA2: the threat moves with two out-of-line calls (0x6D95D0, 0x6D95A0)
				Add_Cell_Threat(Get_Cell_Ptr());
			}
			int face;
			for (face = FACING_FIRST; face < FACING_COUNT; face++) {
				Cell c = Adjacent_Cell(LastAdjacencyCell, (FacingType)face);
				CellClass *cptr = &Map[c];
				cptr->AdjacentObjectCount--;
			}
			LastAdjacencyCell = PositionCell;
			for (face = FACING_FIRST; face < FACING_COUNT; face++) {
				Cell c = Adjacent_Cell(LastAdjacencyCell, (FacingType)face);
				CellClass *cptr = &Map[c];
				cptr->AdjacentObjectCount++;
			}
		}

		/*
		**	If adjacent to an enemy techno that has the ability to reveal a sub,
		**	then shimmer the cloaked object.
		*/
		if (Cloak == CLOAKED) {
			for (FacingType face = FACING_N; face < FACING_COUNT; face++) {
				Cell cell = Adjacent_Cell(PositionCell, (FacingType)face); // RA2: the loop's cell is its own local

				if (Map.In_Local_Radar(cell, true)) {
					TechnoClass * techno = Map[cell].Cell_Techno(Point2D(0, 0), false, NULL);

					if (techno && !techno->House->Is_Ally(this) && (techno->TClass->IsScanner || techno->Has_Ability(ABILITY_SENSORS))) {
						Do_Shimmer();
						break;
					}
				}
			}
		}

		/*
		**	Shorten the path if the target is now within weapon range of this
		**	unit and this unit is on an attack type mission.
		*/
		int primary = What_Weapon_Should_I_Use(TarCom);
		bool inrange = false;
		if (TarCom != NULL && (TarCom->AbstractFlags & ABSTRACT_FOOT)) { // RA2: the foot flag test of the abstract, not a dynamic_cast
			inrange = In_Range(((FootClass *)TarCom)->Likely_Coord(), primary);
		}

		if ((Mission == MISSION_RESCUE || Mission == MISSION_GUARD_AREA || Mission == MISSION_ATTACK || Mission == MISSION_HUNT) && inrange && RouteQueue.Count() == 0) {
			Assign_Destination(NULL);
			Path[0] = FACING_NONE;
		}

		Cell cell = PositionCell; // RA2: a second cell local, declared after the loop
		CellClass * cellptr = &Map[cell];

		/*
		**	Trigger event associated with the player entering the cell.
		*/
		if (Cloak != CLOAKED) {
			TagClass * tag = cellptr->Tag;
			int x = cell.X;
			int y = cell.Y;
			if (((!cellptr->IsUnderBridge && !cellptr->WasUnderBridge) || IsOnBridge) && tag != NULL) {
				tag->Spring(TEVENT_PLAYER_ENTERED, this, PositionCell);
				if (RTTI == RTTI_INFANTRY && ((InfantryClass *)this)->Class->IsAgent) { // RA2: an agent also springs two more events
					tag->Spring(TEVENT_PARALYZED, this, PositionCell); // RA2: as above (event 0x35)
					tag->Spring(TEVENT_ENEMY_IN_SPOTLIGHT_REPEATING, this, PositionCell); // RA2: as above (event 0x36)
				}
			}

			/*
			**	Check for horizontal trigger crossing.
			*/
			if (cellptr->IsHorizontalLine) {
				for (int index = 0; index < Map.MapRect.Width; index++) {
					cellptr = &Map[Cell(index+Map.MapRect.X, y)];
					tag = cellptr->Tag;
					if (tag != NULL) {
						if (tag->Is_Cross_Horizontal()) {
							tag->Spring(TEVENT_CROSS_HORIZONTAL, this, PositionCell);
						}
					}
				}
			}

			/*
			**	Check for vertical trigger crossing.
			*/
			if (cellptr->IsVerticalLine) {
				for (int index = 0; index < Map.MapRect.Height; index++) {
					tag = Map[Cell(x, index+Map.MapRect.Y)].Tag;
					if (tag != NULL) {
						if (tag->Is_Cross_Vertical()) {
							tag->Spring(TEVENT_CROSS_VERTICAL, this, PositionCell);
						}
					}
				}
			}

			/*
			**	Check for zone entry trigger events.
			*/
			for (MapTriggerID = 0; MapTriggerID < MapTags.Count(); MapTriggerID++) {
				tag = MapTags[MapTriggerID];
				if (tag != NULL) {
					if (tag->Is_Enters_Zone()) {
						if (Map.Is_Same_Cell_Zone(tag->Get_Position(), Destination_Coord().As_Cell(), TClass->MZone, Map[tag->Get_Position()].IsUnderBridge, Is_Moving_Onto_Bridge())) {
							tag->Spring(TEVENT_ENTERS_ZONE, this, Destination_Coord().As_Cell());
						}
					}
				}
			}
		}

		BuildingClass * building = cellptr->Cell_Building();
		if (building != NULL) {
			if (building->Class->IsLaserFence && building->LaserFenceFrame < 8) {
				RTTIType rtti = RTTI;
				if (rtti > RTTI_NONE && (rtti <= RTTI_AIRCRAFT || rtti == RTTI_INFANTRY) && Strength > 0) {
					Take_Damage(Strength, 0, Rule->C4Warhead, building, true, true);
				}
			}
		}

		if (IsOnBridge && !Map[(Coord const &)PositionCoord].IsUnderBridge) {
			Fall_From_Height();
		}

		/*
		**	If any of these triggers cause this unit to be destroyed, then
		**	stop all further processing for this unit.
		*/
		if (!IsActive) return;

		if (!Map.In_Local_Radar((Cell const &)PositionCell, true) && Should_Delete_Off_Map()) {
			Delete_Me();
			return;
		}

		if (IsSelected && !House->Is_Player_Control() && Map.Is_Shrouded(PositionCoord)) {
			Unselect();
		}
	}

	if (CurrentPath != PATH_NONE) {
		WaypointPathClass * path = PlayerPtr->Paths[CurrentPath];
		WaypointClass * next_waypoint = path->Get_Waypoint(NextWaypoint);
		if (next_waypoint != NULL) {
			Cell wp_cell = next_waypoint->Location;
			if (Map.DraggedWaypoint == next_waypoint) {
				wp_cell = Map.DraggedWaypointCoord;
			}
			if (WaypointOffsetCell + wp_cell != WaypointTargetCell) {
				Execute_Waypoint_Path(next_waypoint);
			}
		} else {
			CurrentPath = PATH_NONE;
			NextWaypoint = NULL;
			WaypointOffsetCell = Cell(0, 0);
			WaypointTargetCell = Cell(0, 0);
		}
	}

	BASECLASS::Per_Cell_Process(why);
}



/***************************************************************************
 * FootClass::Override_Mission -- temporarily overrides a units mission    *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 * INPUT:      MissionType mission - the mission we want to override       *
 *               TARGET      tarcom  - the new target we want to override  *
 *               TARGET      navcom  - the new navigation point to override*
 *                                                                         *
 * OUTPUT:      none                                                       *
 *                                                                         *
 * WARNINGS:   If a mission is already overridden, the current mission is  *
 *               just re-assigned.                                         *
 *                                                                         *
 * HISTORY:                                                                *
 *   04/28/1995 PWG : Created.                                             *
 *=========================================================================*/
void FootClass::Override_Mission(MissionType mission, AbstractClass * tarcom, AbstractClass * navcom)
{
	SuspendedNavCom = NavCom;
	BASECLASS::Override_Mission(mission, tarcom, navcom);

	Assign_Destination(navcom);
}


/***************************************************************************
 * FootClass::Restore_Mission -- Restores an overridden mission            *
 *                                                                         *
 * INPUT:      none                                                        *
 *                                                                         *
 * OUTPUT:     none                                                        *
 *                                                                         *
 * WARNINGS:   none                                                        *
 *                                                                         *
 * HISTORY:                                                                *
 *   04/28/1995 PWG : Created.                                             *
 *=========================================================================*/
bool FootClass::Restore_Mission(void)
{
	if (BASECLASS::Restore_Mission()) {
		Assign_Destination(SuspendedNavCom);
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * FootClass::Receive_Message -- Movement related radio messages are handled here.             *
 *                                                                                             *
 *    This routine handles radio message that are related to movement. These are used for      *
 *    complex coordinated maneuvers.                                                           *
 *                                                                                             *
 * INPUT:   from     -- Pointer to the originator of this radio message.                       *
 *                                                                                             *
 *          message  -- The radio message that is being received.                              *
 *                                                                                             *
 *          param    -- The optional parameter (could be a movement destination).              *
 *                                                                                             *
 * OUTPUT:  Returns with the radio response appropriate to the message received. Usually the   *
 *          response is RADIO_ROGER.                                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/14/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
RadioMessageType FootClass::Receive_Message(RadioClass * from, RadioMessageType message, int & param)
{
	BuildingClass const * building = NULL;
	ObjectClass *object = NULL;


	switch ((int)message) { // RA2: the cases mix main's enumerators with RA2's numbers

		/*
		**	Answers if this object is located on top of a service depot.
		*/
		case RADIO_ON_DEPOT: // RA2: number 35
			building = Map[Center_Coord()].Cell_Building();
			if (building == (RadioClass const *)from) {
				return(RADIO_ROGER);
			}
			return(RADIO_NEGATIVE);

		/*
		**	Intercept the repair request and if this object is moving, then no repair
		**	is possible.
		*/
		case RADIO_REPAIR: // RA2: number 28
			if (NavCom != NULL) return(RADIO_NEGATIVE);
			break;

		/*
		**	Something bad has happened to the object in contact with. Abort any coordinated
		**	activity with this object. Basically, ... run away! Run away!
		*/
		case RADIO_RUN_AWAY:
			if (HasAnyLink()) { // RA2: the link test is HasAnyLink, the partner comes from Radio_Link(0)
				if (NavCom == Radio_Link(0)) {
					Assign_Destination(NULL);
				}
			}
			if (Mission == MISSION_SLEEP) {
				QueueMission(MISSION_GUARD, false); // RA2: QueueMission (slot 116), not Assign_Mission
				if (Ready_To_Commence()) {
					Commence();
				}
			}
			if (Mission == MISSION_ENTER) {
				QueueMission(MISSION_GUARD, false);
			}
			if (!IsRotating && NavCom == NULL) {
				Scatter(COORD_NONE, true, true);
			}
			break;

		/*
		**	Checks to see if this unit needs to move somewhere. If it is already in motion,
		**	then it doesn't need further movement instructions.
		*/
		case RADIO_NEED_TO_MOVE:
			param = (int)NavCom;
			if (NavCom == NULL || !Locomotion->Is_Moving()) {
				return(RADIO_ROGER);
			}
			return(RADIO_NEGATIVE);

		/*
		**	Radio request to move to location specified. Typically this is used
		**	for complex loading and unloading missions.
		*/
		case RADIO_MOVE_HERE:
			object = (ObjectClass *)param;
			{
				if (object != NULL && PositionCell == Cell(object->Center_Coord())) {
						return(RADIO_YEA_NOW_WHAT);
				} else {
					if (Mission == MISSION_GUARD && MissionQueue == MISSION_NONE) {
						QueueMission(MISSION_MOVE, false);
					}
					if (MissionQueue == MISSION_ENTER && Ready_To_Commence()) {
						Commence();
					}
					Assign_Destination((ObjectClass *)param);
					Shorten_Mission_Timer();
				}
			}
			return(RADIO_ROGER);

		/*
		**	Requests if this unit is trying to cooperatively load up. Typically, this occurs
		**	for passengers and when vehicles need to be repaired.
		*/
		case RADIO_TRYING_TO_LOAD:
			if (Mission == MISSION_ENTER || MissionQueue == MISSION_ENTER) {
				BASECLASS::Receive_Message(from, message, param);
				return(RADIO_ROGER);
			}
			break;
	}
	return(BASECLASS::Receive_Message(from, message, param));
}



/***********************************************************************************************
 * FootClass::Mission_Enter -- Enter (cooperatively) mission handler.                          *
 *                                                                                             *
 *    This mission handler will cooperatively coordinate the object to maneuver into the       *
 *    object it is in radio contact with. This is used by infantry when they wish to load      *
 *    into an APC as well as by vehicles when they wish to enter a repair facility.            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the number of game ticks before this routine should be called again.       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/15/1995 JLB : Created.                                                                 *
 *   09/22/1995 JLB : Modified to handle the "on hold" condition.                              *
 *=============================================================================================*/
int FootClass::Do_MISSION_ENTER(void)
{
	/*
	**	Find out who to coordinate with. If in radio contact, then this the transporter is
	**	defined. If not in radio contact, then try the archive target value to see if that
	**	is suitable.
	*/
	TechnoClass * contact = Radio_Link(0); // RA2: the out-of-line link accessor
	if (contact == NULL) {
		contact = Dynamic_Cast<TechnoClass *>(ArchiveTarget);
	}

	/*
	**	If in contact, then let the transporter handle the movement coordination.
	*/
	if (contact != NULL) {

		/*
		**	If the transport says to "bug off", then abort the enter mission. The transport may
		**	likely say all is 'ok' with the "RADIO ROGER", then try again later.
		*/
		if (Transmit_Message(RADIO_DOCKING, contact) != RADIO_ROGER && !IsTethered) {
			Transmit_Message(RADIO_OVER_OUT);
			Enter_Idle_Mode();
		} else {
			if (NavCom == NULL && RouteQueue.Count() > 0 ) {
				IPiggybackPtr piggy(Locomotion);
				if (piggy != NULL && piggy->Is_Ok_To_End()) {
					piggy->End_Piggyback(&Locomotion);
				}
				if (RouteQueue.Count() > 0) {
					Assign_Destination(RouteQueue[0], false);
					RouteQueue.Delete_Index(0);
				}
			} else if (Techno_Type_Class()->Teleporter) { // RA2: a teleporter restarts its move to the destination
				AbstractClass * target = NavCom;
				FollowTarget = NULL;
				NavCom = NULL;
				Assign_Destination(target, true);
			}
		}

	} else {

		/*
		**	Since there is no potential object to enter, then abort this
		**	mission with some default standby mission.
		*/
		if (!Move_To_Object_Nearby()) {
			if (NavCom == NULL || NavCom->RTTI != RTTI_UNIT && NavCom->RTTI != RTTI_AIRCRAFT) {
				Enter_Idle_Mode();
			}
		}
		Commence();
	}

	return(Current_Mission_Control().Normal_Delay() + Random_Pick(0, 2));
}


/// <summary>
/// Sets the object this object follows (the Follow object of a MegaMission order).
/// </summary>
void FootClass::Set_Follow_Target(AbstractClass * target) // RA2: not in TibSun, at 0x4C8020
{
	FollowTarget = target;
}





/***********************************************************************************************
 * FootClass::Assign_Destination -- Assigns specified destination to NavCom.                   *
 *                                                                                             *
 *    This routine will assign the specified target to the navigation computer. No legality    *
 *    checks are performed.                                                                    *
 *                                                                                             *
 * INPUT:   target   -- The target value to assign to the navigation computer.                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void FootClass::Assign_Destination(AbstractClass * target, bool)
{
	FollowTarget = NULL; // RA2: RA2's form
	NavCom = target;

	if (target != NULL) {
		if (ParticleSystems[0] != NULL) {
			ParticleSystems[0]->Delete_Me();
			ParticleSystems[0] = NULL;
		}

		IPersistPtr persist(Locomotion);
		GUID clsid;
		persist->GetClassID(&clsid);

		if (memcmp(&clsid, &CLSID_HoverLocomotion, sizeof(GUID)) == 0 && PathDelay.Value() == 0) {
			PathDelay = CDTimerClass<FrameTimerClass>(1);
		}

		if (IsMoveToSkipped) {
			IsMoveToSkipped = false;
		} else {
			Coord coord = NavCom->Destination_Coord(this);
			Locomotion->Move_To(coord);
		}
	} else if (Fetch_RTTI() != RTTI_AIRCRAFT || (CurrentMission != MISSION_ATTACK && MissionQueue != MISSION_ATTACK) || TarCom == NULL) {
		Locomotion->Stop_Moving();
		NavCom = NULL;
	}

	/*
	**	Presume that the easiest path is tried first. As the findpath proceeds, when
	**	a failure occurs, this threshhold will be increased until path failure
	**	cannot be prevent. At this point, all movement should cease.
	*/
	IsToPathAroundBlockage = false;
	BlockagePathDelay = CDTimerClass<FrameTimerClass>(Rule->BlockagePathDelay);
	PathDelay = CDTimerClass<FrameTimerClass>(0);
}




/***********************************************************************************************
 * FootClass::Detach_All -- Removes this object from the game system.                          *
 *                                                                                             *
 *    This routine will remove this object from the game system. This routine is called when   *
 *    this object is about to be deleted. All other objects should no longer reference this    *
 *    object in that case.                                                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void FootClass::Detach_All(bool all)
{
	if (all) {
		if (Team && !ScenarioInit) {
			Team->Remove(this);
		}
	}

	if (!all) {
		TechnoClass * whom = Radio_Link(0); // RA2: the out-of-line link accessor, not In_Radio_Contact/Contact_With_Whom's inline reads
		if (whom != NULL && !House->Is_Ally(whom)) {
			Transmit_Message(RADIO_OVER_OUT);
		}
	} else {
		Transmit_Message(RADIO_OVER_OUT);
	}

	BASECLASS::Detach_All(all);
}



/***********************************************************************************************
 * FootClass::Rescue_Mission -- Calls this unit to the rescue.                                 *
 *                                                                                             *
 *    This routine is called when the house determines that it should attack the specified     *
 *    target. This routine will determine if it can attack the target specified and if so,     *
 *    the amount of power it can throw at it. This returned power value is used to allow       *
 *    intelligent distribution of retaliation.                                                 *
 *                                                                                             *
 * INPUT:   target   -- The target that this object just might be assigned to attack and thus  *
 *                      how much power it can bring to bear should be returned.                *
 *                                                                                             *
 * OUTPUT:  Returns with the amount of power that this object can bring to bear against the    *
 *          potential target specified.                                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int FootClass::Rescue_Mission(AbstractClass * tarcom)
{
	if (tarcom == NULL) return(0);

	if (TarCom == tarcom) {
		return(-Risk());
	}

	if (TarCom != NULL && (TarCom->AbstractFlags & ABSTRACT_TECHNO) && ((TechnoClass *)TarCom)->Is_Weapon_Equipped()) { // RA2: Rescue_Mission at 0x4C82A0 matches only so
		return(0);
	}

	bool basedefense = Team != NULL && Team->Class->IsBaseDefense;
	if ((Team != NULL && !basedefense) || Get_Mission() == MISSION_HARVEST || !Risk()) {
		return(0);
	}

	int dist = Distance(tarcom) - Weapon_Range(0);
	int threat = Risk() * 1024;
	int speed = -1;
	if (dist > 0) {
		speed = MAX((unsigned)Get_Max_Speed(), (unsigned)1);

		int ratio = (speed > 0) ? MAX(dist / speed, 1) : 1;

		threat = MAX(threat/ratio, 1);
	}
	return(threat);
}


/***********************************************************************************************
 * FootClass::Death_Announcement -- Announces the death of a unit.                             *
 *                                                                                             *
 *    This routine is called when a unit (infantry, vehicle, or aircraft) is destroyed.        *
 *                                                                                             *
 * INPUT:   source   -- The perpetrator of this death.                                         *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/01/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void FootClass::Death_Announcement(TechnoClass const * ) const
{
	if (House->Is_Player_Control() && !Techno_Type_Class()->Spawned) { // RA2: RA2's form
		if (Submit_Radar_Event(RADAREVENT_UNIT_LOST, Get_Cell())) {
			Queue_Speech("EVA_UnitLost", -1, -1);
		}
	}
}



/***********************************************************************************************
 * FootClass::Greatest_Threat -- Fetches the greatest threat to this object.                   *
 *                                                                                             *
 *    This routine will return with the greatest threat (best target) for this object. For     *
 *    movable ground object, they won't automatically return ANY target if this object is      *
 *    cloaked. Otherwise, cloaking is relatively useless.                                      *
 *                                                                                             *
 * INPUT:   method   -- The request method (bit flags) to use when scanning for a target.      *
 *                                                                                             *
 * OUTPUT:  Returns with the best target to attack. If there is no target that qualifies, then *
 *          TARGET_NONE is returned.                                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1995 JLB : Created.                                                                 *
 *   07/10/1996 JLB : Handles scan range limitation.                                           *
 *=============================================================================================*/
AbstractClass * FootClass::Greatest_Threat(ThreatType method, Coord const & coord, bool onlyenemy) const
{
	if (IsScanLimited) {
		method = ThreatType(method & ~THREAT_AREA);
		method = ThreatType(method | THREAT_RANGE);
	}

	AbstractClass * target = BASECLASS::Greatest_Threat(method, coord, onlyenemy); // RA2: Greatest_Threat at 0x4C8420 matches only so

	if (IsScanLimited && target == NULL) {
		const_cast<FootClass *>(this)->IsScanLimited = false;
	}

	return(target); // RA2: Greatest_Threat at 0x4C8420 has no ThreatType, AbstractClass, IsScanLimited and 1 more
}


/***********************************************************************************************
 * FootClass::Detach -- Detaches a target from tracking systems.                               *
 *                                                                                             *
 *    This routine will detach the specified target from the tracking systems of this object.  *
 *    It will be removed from the navigation computer and any queued mission record.           *
 *                                                                                             *
 * INPUT:   target   -- The target to be removed from this object.                             *
 *                                                                                             *
 *          all      -- Is the unit really about to be eliminated? If this is true then even   *
 *                      friendly contact (i.e., radio) must be eliminated.                     *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1995 JLB : Created.                                                                 *
 *   07/24/1996 JLB : Removes target from NavQueue list.                                       *
 *=============================================================================================*/
void FootClass::Detach(AbstractClass const * target, bool all)
{
	int index;

	BASECLASS::Detach(target, all);

	if (Team == target) {
		Team = NULL;
	}

	if (ParasiteEatingMe == target && (ParasiteEatingMe->Strength == 0 || !ScenarioActive)) { // RA2: the parasite that is eating this object is forgotten
		ParasiteEatingMe = NULL;
	}

	if (ParasiteEatingMe != NULL && ParasiteEatingMe->Strength > 0) { // RA2: as above, a living eater is told as well
		ParasiteEatingMe->ParasiteImUsing->Detach(target, true);
	}

	if (target == this) {
		ParasiteEatingMe = NULL;
	}

	if (Member == target && all) {
		if (target != NULL) {
			Member = Member->Member;
		}
	}

	if (ArchiveTarget == target) {
		Assign_Archive_Target(NULL); // RA2: the setter call, not a store
	}

	if (SuspendedNavCom == target) {
		SuspendedNavCom = NULL;
	}

	/*
	**	If the navigation computer is assigned to the target, then the navigation
	**	computer must be cleared.
	*/
	if (NavCom == target) {
		bool forget = true; // RA2: a flag replaces the old single condition
		bool techno = false;
		if (!all && target != NULL) {
			techno = target->AbstractFlags & ABSTRACT_TECHNO; // RA2: the Is_Techno test is read into a local
		}
		if (!all && target != NULL && techno) {
			CellClass * cptr = &Map[target->Center_Coord()];
			if (cptr->Is_Sensed(House->HeapID)) {
				forget = false;
			}
		}
		if (Mission == MISSION_CAPTURE && RTTI == RTTI_INFANTRY && ((InfantryClass *)this)->Class->Occupier && NavCom != NULL && (NavCom->AbstractFlags & ABSTRACT_OBJECT) && ((ObjectClass *)NavCom)->IsActive && ((ObjectClass *)NavCom)->Strength > 0) { // RA2: an infantry unit capturing an occupiable building keeps its NavCom while the building lives
		} else if (forget) {
			FollowTarget = NULL; // RA2: the follow target is dropped with the NavCom
			NavCom = NULL;
		}
	}

	if (MegaTarget == target) {
		MegaDestination = &Map[Cell(MegaTarget->Center_Coord())]; // RA2: a Mega order whose target goes away keeps its cell as the destination
		MegaTarget = NULL;
	}

	/*
	**	Remove the target from the NavQueue list as well.
	*/
	for (index = 0; index < NavQueue.Count(); index++) {
		if (NavQueue[index] == target) {
			NavQueue.Delete_Index(index);
			index--;
		}
	}

	for (index = 0; index < RouteQueue.Count(); index++) {
		if (RouteQueue[index] == target) {
			RouteQueue.Delete_Index(index);
			index--;
		}
	}
}



/***********************************************************************************************
 * FootClass::Offload_Tiberium_Bail -- Fetches the Tiberium to offload per step.               *
 *                                                                                             *
 *    This routine is called when a packet/package/bail of Tiberium needs to be offloaded      *
 *    from the object. This function is overridden for those objects that can contain          *
 *    Tiberium.                                                                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of credits offloaded from the object.                      *
 *                                                                                             *
 * WARNINGS:   This routine must be called multiple times in order to completely offload the   *
 *             Tiberium. When this routine return 0, all Tiberium has been offloaded.          *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int FootClass::Offload_Tiberium_Bail(void)
{
	return(0);
}


/***********************************************************************************************
 * FootClass::Can_Enter_Cell -- Checks to see if the object can enter cell specified.          *
 *                                                                                             *
 *    This routine examines the specified cell to see if the object can enter it. This         *
 *    function is to be overridden for objects that could have the possibility of not being    *
 *    allowed to enter the cell. Typical objects at the FootClass level always return          *
 *    MOVE_OK.                                                                                 *
 *                                                                                             *
 * INPUT:   cell     -- The cell to examine.                                                   *
 *                                                                                             *
 *          facing   -- The direction that this cell might be entered from.                    *
 *                                                                                             *
 * OUTPUT:  Returns with the move check result type. This will be MOVE_OK if there is not      *
 *          blockage. There are various other values that represent other blockage types.      *
 *          The value returned will indicated the most severe reason why entry into the cell   *
 *          is blocked.                                                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
MoveType FootClass::Can_Enter_Cell(CellClass const * cell, FacingType, int cell_height, CellClass const *, bool use_locomotor_enter_check) const
{
	if (Locomotion != NULL && use_locomotor_enter_check) {
		CellStruct cellnum = (CellStruct &)cell->CellID; // RA2: Can_Enter_Cell at 0x4C86E0 matches only so
		return(Locomotion->Can_Enter_Cell(cellnum));
	}
	return(MOVE_OK);
}


/// <summary>
/// Determines if this object can step between two adjacent cells.
/// This routine is used by the Can_Enter_Cell logic to police elevation changes. The step is
/// refused unless the two cells are level, joined by a ramp, or joined by a bridge that this
/// object is permitted to travel over or under.
/// </summary>
/// <param name="current_cell">The cell being stepped out of.</param>
/// <param name="facing">The direction of travel, or FACING_NONE to merely resolve the
/// height.</param>
/// <param name="cell_height">The elevation to travel at. This is filled in with the bridge deck
/// height when the cell turns out to be spanned by one.</param>
/// <param name="onto_bridge">Set to true when the step climbs onto a bridge deck.</param>
/// <param name="adjacent_cell">The cell being stepped into, or NULL to derive it from the
/// facing.</param>
/// <returns>Returns with MOVE_OK if the step is legal, or MOVE_NO if the elevation change
/// forbids it.</returns>
MoveType FootClass::Can_Reach(CellClass const * current_cell, FacingType facing, int & cell_height, bool & onto_bridge, CellClass const * adjacent_cell) const
{
	if (adjacent_cell == NULL) {
		adjacent_cell = &Map[Adjacent_Cell(current_cell->CellID, Facing_Sub(facing, FACING_180))];
	}

	if (facing != FACING_NONE) {
		if (adjacent_cell == NULL || current_cell == NULL) {
			return(MOVE_OK);
		}

		if (cell_height == -1 && adjacent_cell->IsUnderBridge) {
			cell_height = adjacent_cell->Height + BRIDGE_CELL_HEIGHT;
			if (!current_cell->IsBridgeTraversable) {
				return(MOVE_NO);
			}
		}

		int height_difference = adjacent_cell->IsUnderBridge ? adjacent_cell->Height - current_cell->Height : cell_height - current_cell->Height; // RA2: Can_Reach at 0x4C8730 matches only so
		int current_height = current_cell->Height;

		switch (abs(height_difference)) {
			case 0:
				if ((!current_cell->IsUnderBridge || !current_cell->IsBridgeTraversable || !adjacent_cell->IsUnderBridge) && cell_height != -1 && cell_height != current_height) {
					return(MOVE_NO);
				}
				break;

			case 1:
				if (height_difference > 0) {
					if (current_cell->Ramp == 0) {
						return(MOVE_NO);
					}
				} else if (adjacent_cell->Ramp == 0) {
					return(MOVE_NO);
				}
				break;

			case BRIDGE_CELL_HEIGHT:
				if (adjacent_cell->Height == current_height - BRIDGE_CELL_HEIGHT) {
					if (cell_height != current_height) return(MOVE_NO);
					if (!adjacent_cell->IsUnderBridge) return(MOVE_NO);
				}
				if (current_height != adjacent_cell->Height - BRIDGE_CELL_HEIGHT) return(MOVE_OK);
				if (!current_cell->IsUnderBridge) return(MOVE_NO);
				if (!current_cell->IsBridgeTraversable) return(MOVE_NO);
				onto_bridge = true;
				break;

			default:
				return(MOVE_NO);
		}
	} else {
		if (cell_height == -1 && current_cell->IsUnderBridge) {
			cell_height = current_cell->Height + BRIDGE_CELL_HEIGHT;
		}
	}
	return(MOVE_OK);
}


/// <summary>
/// RA2
/// </summary>
static inline int Coord_Distance(Coord const & a, Coord const & b)
{
	Coord diff = a - b;
	return(diff.Length());
}


/***********************************************************************************************
 * FootClass::Can_Demolish -- Checks to see if this object can be sold back.                   *
 *                                                                                             *
 *    This routine determines if it is legal to sell the object back. A foot class object can  *
 *    only be sold back if it is sitting on a repair bay.                                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Was the object successfully sold back?                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/13/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool FootClass::Can_Demolish(void) const
{
	switch ((RTTIType)RTTI) {
		case RTTI_UNIT:
		case RTTI_AIRCRAFT:
			if (HasAnyLink() && // RA2: the link test is HasAnyLink and the partner comes from Radio_Link(0), not the inline accessors
				Radio_Link(0)->RTTI == RTTI_BUILDING &&
				((BuildingClass *)Radio_Link(0))->Class->IsCanUnitRepair) {

				Coord diff = Center_Coord() - Radio_Link(0)->Center_Coord(); // RA2: the difference is a named local
				double z = diff.Z; // RA2: the length loads z, y, x in this order
				double y = diff.Y;
				double x = diff.X;
				if ((int)fastmath::sqrt(x * x + y * y + z * z) < CELL_LEPTON / 2) {
					return(true);
				}
			}
			break;

		default:
			break;
	}
	return(BASECLASS::Can_Demolish());
}




/***********************************************************************************************
 * FootClass::Sell_Back -- Causes this object to be sold back.                                 *
 *                                                                                             *
 *    When an object is sold back, a certain amount of money is refunded to the owner and then *
 *    the object is removed from the game system.                                              *
 *                                                                                             *
 * INPUT:   control  -- The action to perform. The only supported action is "1", which means   *
 *                      to sell back.                                                          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/13/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void FootClass::Sell_Back(int control)
{
	if (control) { // RA2: RA2's form
		if (House->Is_Player_Control()) {
			Queue_Speech("EVA_UnitSold", -1, -1);
			VocClass::PlayGlobal(Rule->SellSound, 1.0f, 0x2000, 0);
		}
		House->Refund_Money(Refund_Amount());
		Stun();
		Limbo();
		Delete_Me();
	}
}



/***********************************************************************************************
 * FootClass::Likely_Coord -- Fetches the coordinate the object will be at shortly.            *
 *                                                                                             *
 *    This routine comes in handy when determining where a travelling object will be at        *
 *    when considering the amount of time it would take for a normal unit to travel one cell.  *
 *    Using this information, an intelligent "approach target" logic can be employed.          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the coordinate the object is at or soon will be.                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/13/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
Coord FootClass::Likely_Coord(void) const
{
	return(Target_Coord());
}


/***********************************************************************************************
 * FootClass::Adjust_Dest -- Adjust candidate movement cell to account for formation.          *
 *                                                                                             *
 *    This routine modify the specified cell if the unit is part of a formation. The           *
 *    adjustment will take into consideration the formation relative offset from the           *
 *    (presumed) center cell specified.                                                        *
 *                                                                                             *
 * INPUT:   cell  -- The cell to presume as the desired center point of the formation.         *
 *                                                                                             *
 * OUTPUT:  Returns with the cell that should be used as the actual destination. If this       *
 *          object is part of a formation, then the cell location will be appropriately        *
 *          adjusted.                                                                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/11/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
Cell FootClass::Adjust_Dest(Cell const & cell) const
{
	return(cell);
}


/***********************************************************************************************
 * FootClass::Handle_Navigation_List -- Processes the navigation queue.                        *
 *                                                                                             *
 *    This routine will process the navigation queue. If the queue is present and valid and    *
 *    there is currently no navigation target assigned to this object, then the first entry    *
 *    of the queue will be assigned. The remaining entries will move down. If the queue is     *
 *    to be processed as a circular list, then the first entry is appended to the end.         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine might end up assigning a movement destination.                     *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void FootClass::Handle_Navigation_List(void)
{
	if (NavCom == NULL && NavQueue.Count()) {
		AbstractClass * target = NavQueue[0];

		if (target != NULL) {
			Assign_Destination(target);
			NavQueue.Delete_Index(0);

			if (IsNavQueueLoop) {
				NavQueue.Add(target);
			}
		}
	}
}


/***********************************************************************************************
 * FootClass::Queue_Navigation_List -- Add a target to the objects navigation list.            *
 *                                                                                             *
 *    This routine will append the destination target to the object's NavQueue list. After     *
 *    doing so, if the object is not doing anything important, then it will be started on      *
 *    that destination. This is functionally the same as Assign_Destination, but it stores     *
 *    the target to the NavQueue first.                                                        *
 *                                                                                             *
 * INPUT:   target   -- The movement target destination to append the queue.                   *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   The queue is of finite size and any queue requests that would exceed that size  *
 *             are ignored. If there are no queue entries pending and the unit is not          *
 *             otherwise occupied, then the queue target might be carried directly into the    *
 *             NavCom.                                                                         *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void FootClass::Queue_Navigation_List(AbstractClass * target)
{
	if (target != NULL) {
		int count = NavQueue.Count();
		/*
		**	If the target is this object itself, then this indicates that the
		**	queue list is to be processed as a loop. Otherwise, just tack the
		**	navigation target to the end of the list.
		*/
		if (target == this && count > 0) {
			IsNavQueueLoop = true;
		} else {
			if (count == 0) {
				IsNavQueueLoop = false;
			}
			NavQueue.Add(target);
		}

		/*
		**	If this object isn't doing anything, then start acting on the
		**	navigation queue now.
		*/
		if (NavCom == NULL && Mission == MISSION_GUARD) {
			TechnoClass *tptr = Radio_Link(0); // RA2: the out-of-line link accessor, not Contact_With_Whom's inline array read
			if (tptr == NULL || tptr->RTTI != RTTI_BUILDING || !((BuildingClass*)tptr)->Class->IsWeaponsFactory) {
				Enter_Idle_Mode();
			}
		}
	}
}



/***********************************************************************************************
 * FootClass::Clear_Navigation_List -- Clears out the navigation queue.                        *
 *                                                                                             *
 *    This routine will clear out any values in the navigation queue. This is the preferred    *
 *    way of aborting a navigation queue for a unit. If the unit is already travelling, it     *
 *    won't be interrupted by this routine.                                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This will clear the navigation list but not the navigation computer. Thus a     *
 *             unit will still travel to its current immediate destination.                    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/30/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void FootClass::Clear_Navigation_List(void)
{
	NavQueue.Clear();
}


/***********************************************************************************************
 * FootClass::Is_Allowed_To_Leave_Map -- Checks to see if it can leave the map and the game.   *
 *                                                                                             *
 *    This routine will determine if this object has permission to leave the map and thus      *
 *    leave the game. Typical objects with this permission are transports used to drop of      *
 *    reinforcements.                                                                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Does this object have permission to travel off the map edge and leave the    *
 *                game?                                                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/05/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool FootClass::Is_Allowed_To_Leave_Map(void) const
{
	/*
	**	If the unit hasn't entered the map yet, then don't allow leave the game.
	*/
	if (!IsLocked) return(false);

	/*
	**	A unit that isn't marked as a loaner is a gift to the player. Such objects can never
	**	leave the map unless they are part of a team that gives it special permision.
	*/
	if (!TClass->IsTrain && !IsALoaner && Mission != MISSION_RETREAT && (Team == NULL || !Team->Is_Leaving_Map())) return(false);

	return(true);
}


/***********************************************************************************************
 * FootClass::Is_Recruitable -- Determine if this object is recruitable as a team members.     *
 *                                                                                             *
 *    This will examine this object to determine if it is suitable as a team recruit. Some     *
 *    objects are disqualified if they are otherwise premptively occupied.                     *
 *                                                                                             *
 * INPUT:   house -- Pointer to the house that is trying to recruit this object.               *
 *                                                                                             *
 * OUTPUT:  bool; Is this object suitable for recruitment by a team.                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/14/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool FootClass::Is_Recruitable(HouseClass const * house) const
{
	/*
	**	If not of the correct house presuasion, then recruitment is not allowed.
	*/
	if (house != NULL && house != House) {
		return(false);
	}

	/*
	**	If the object is not a playing member of the game, then don't consider it available.
	*/
	if (IsInLimbo) {
		return(false);
	}

	/*
	**	If it is already part of another team, then it is not available for
	**	general recruitment.
	*/
	if (Team != NULL) {
		return(false);
	}

	/*
	**	If it is currently in a mission the precludes recruitment into a team, then
	**	return with this information.
	*/
	if (!Is_Recruitable_Mission(Mission)) {
		return(false);
	}

	if (!IsTeamRecruitable) {
		return(false);
	}

	/*
	**	It was not disqualified for general team recruitment, so return that
	**	it is available.
	*/
	return(true);
}


/***********************************************************************************************
 * FootClass::Is_On_Priority_Mission -- Checks to see if this object should be given priority. *
 *                                                                                             *
 *    Some objects are on an important mission that must succeed. If the object is on such     *
 *    a mission, then it will be more aggressive in its movement action.                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Is this object on a priority mission?                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/30/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool FootClass::Is_On_Priority_Mission(void) const
{
	if (Mission == MISSION_ENTER) return(true);
	return(false);
}


/// <summary>
/// The unload mission: a foot object does what the plain mission does.
/// </summary>
int FootClass::Do_MISSION_UNLOAD(void) // RA2: not in TibSun, at 0x4C8D70
{
	return(BASECLASS::Do_MISSION_UNLOAD());
}





/***********************************************************************************************
 * FootClass::Mission_Retreat -- Handle reatreat from map mission for mobile objects.          *
 *                                                                                             *
 *    This will try to make this mobile object leave the map. It does this by assigning a      *
 *    movement destination that is located off the edge of the map.                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of game frames to delay before calling this routine        *
 *          again.                                                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/05/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int FootClass::Do_MISSION_RETREAT(void)
{
	enum {
		FIND_EDGE,
		TRAVELLING
	};

	switch (Status) {

		/*
		**	Find a suitable edge to travel to and then assign destination there.
		*/
		case FIND_EDGE:
			if (NavCom != NULL) {
				Status = TRAVELLING;
			} else {

				Cell cell(0,0);

				/*
				**	If this is part of a team, then pick the edge where the team as likely
				**	entered from.
				*/
				if (Team != NULL && Team->Class->Get_Origin() != CELL_NONE) {
					cell = Map.Calculated_Cell(House->Control.Edge, Team->Class->Get_Origin(), Center_Coord().As_Cell(), TClass->Speed);
				}

				/*
				**	If an edge hasn't been found, then try to find one that is not based on any
				**	team information.
				*/
				if (cell == CELL_NONE) {
					cell = Map.Calculated_Cell(House->Control.Edge, CELL_NONE, Center_Coord().As_Cell(), TClass->Speed);
				}

				assert(cell == Cell(0,0));		// An edge cell must be found!

				Assign_Destination(&Map[cell]);
				Status = TRAVELLING;
			}
			break;

		/*
		**	While travelling, monitor that all is proceeding according to plan.
		*/
		case TRAVELLING:
			if (NavCom == NULL) {
				Status = FIND_EDGE;
			}
			break;
	}

	return(Current_Mission_Control().Normal_Delay() + Random_Pick(0, 2));
}


/// <summary>
/// Determines the visual character of this object.
/// The locomotor gets first say, so that a submerging or burrowing locomotion can hide the
/// object. Failing that, the normal cloaking rules decide the matter.
/// </summary>
/// <param name="raw">Should the check be based on the unmodified cloak condition?</param>
/// <returns>Returns with the visual character to use when displaying this object.</returns>
VisualType FootClass::Visual_Character(bool raw, HouseClass const * house) const
{
	VisualType visual = VISUAL_NORMAL;

	if (Locomotion != NULL) {
		visual = Locomotion->Visual_Character(raw);
	}
	if (visual == VISUAL_NORMAL) {
		visual = BASECLASS::Visual_Character(raw, house);
	}

	return(visual);
}


/***********************************************************************************************
 * FootClass::AI -- Handle general movement AI.                                                *
 *                                                                                             *
 *    This basically just sees if this object is within weapon range of the target and if      *
 *    so, it will stop movement so that firing may commence. This prevents the occasional      *
 *    case of an attacker driving right up to the defender before firing.                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/17/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void FootClass::AI(void)
{
	BASECLASS::AI();

	if (!IsActive) { // RA2: the early exit replaces the nested block
		return;
	}
	IsIdle = false;

	if (Frame % Rule->RadApplicationDelay == 0 && !TClass->ImmuneToRadiation && !In_Air()) { // RA2: radiation damages ground units
		Cell cell = Center_Coord().As_Cell();
		int amount = Map[cell].GetRadLevel();
		if (amount > 0) {
			int damage = (int)(amount * Rule->RadLevelFactor);
			Take_Damage(damage, 0, Rule->RadSiteWarhead, NULL, false, true);
			if (!IsActive) {
				return;
			}
		}
	}

	if (!IsLocked && TClass->BalloonHover && Map.In_Local_Radar(Get_Cell_Ptr(), true)) { // RA2: a balloon-hovering object locks as soon as it is on the radar map
		IsLocked = true;
	}

	if (Locomotion->Is_Moving_Now() && In_Air() && House->Is_Ally(PlayerPtr) && SightTimer.Value() == 0) { // RA2: an airborne mover reveals a ring around it as it moves
		RemoveSight(false, false, false, NULL);
		UpdateSight(false, false, false, NULL);
		if (LastSightHeight != HeightAGL) {
			Map.RevealArea3(Coord(PositionCoord), 0, LastSightRange + 3, 0);
		} else {
			Map.RevealArea3(Coord(PositionCoord), LastSightRange - 3, LastSightRange + 3, 0);
		}
		LastSightHeight = HeightAGL;
		SightTimer = CDTimerClass<FrameTimerClass>(15);
	}

	int frame = TotalFramesWalked; // RA2: the walk frame before the locomotion runs
	if (Locomotion != NULL && !IsSinking && !IsFalling) {
		Locomotion->Process();
		if (!IsActive) {
			return;
		}
		if (((Locomotion->Is_Moving_Now() && !IsWarpingIn()) || (TarCom != NULL && TClass->HoverAttack)) && Frame % TClass->WalkRate == 0 && !IsBeingWarpedOut() && !IsWarpingIn() || // RA2: the walk and idle frame rates
			TClass->IdleRate != 0 && !Locomotion->Is_Moving_Now() && Frame % TClass->IdleRate == 0 && !IsBeingWarpedOut() && !IsWarpingIn()) {
			TotalFramesWalked++;
		}
	}

	if ((frame != TotalFramesWalked || Locomotion->Is_Moving_Now()) && !IsFalling && !IsRocking) { // RA2: the move sound plays while the object walks
		if (!IsMoveSoundPlaying && TClass->MoveSound.Count() > 0) {
			MoveSoundAudioController.Stop();
			VocClass::PlayAt(TClass->MoveSound.Pick(NonCriticalRandomNumber()), Coord(PositionCoord), &MoveSoundAudioController); // RA2: a move sound picked with the noncritical generator
			IsMoveSoundPlaying = true;
		}
		MoveSoundDelay = 3;
	} else if (IsMoveSoundPlaying) {
		if (MoveSoundDelay != 0 && !IsFalling && !IsRocking) {
			MoveSoundDelay--;
		} else {
			MoveSoundAudioController.EndLooping();
			IsMoveSoundPlaying = false;
			MoveSoundDelay = 0;
		}
	}

	if (IsFalling != IsABomb) { // RA2: the falling voice plays when the fall begins
		if (IsFalling && TClass->VoiceFalling != -1 && IsToExplode) {
			VocClass::PlayAt(TClass->VoiceFalling, Coord(PositionCoord), NULL);
		}
		IsABomb = IsFalling;
	}
	if (IsSinking != WasSinkingAlready) { // RA2: the sinking voice and sound play when the sinking begins
		if (IsSinking) {
			if (TClass->VoiceSinking != -1) {
				VocClass::PlayAt(TClass->VoiceSinking, Coord(PositionCoord), NULL);
			}
			if (TClass->SinkingSound != -1) {
				VocClass::PlayAt(TClass->SinkingSound, Coord(PositionCoord), &MoveSoundAudioController);
			} else if (Rule->SinkingSound != -1) {
				VocClass::PlayAt(Rule->SinkingSound, Coord(PositionCoord), &MoveSoundAudioController);
			}
		} else if (!IsMoveSoundPlaying) {
			MoveSoundAudioController.EndLooping();
		}
		WasSinkingAlready = IsSinking;
	}
	if (IsRocking != WasCrashingAlready) { // RA2: the crashing voice and sound play when the crash begins
		if (IsRocking) {
			MoveSoundAudioController.EndLooping();
			if (TClass->VoiceCrashing != -1 && House->Is_Player_Control()) {
				VocClass::PlayAt(TClass->VoiceCrashing, Coord(PositionCoord), NULL);
			}
			if (TClass->CrashingSound != -1) {
				VocClass::PlayAt(TClass->CrashingSound, Coord(PositionCoord), &MoveSoundAudioController);
			}
		} else if (!IsMoveSoundPlaying) {
			MoveSoundAudioController.EndLooping();
		}
		WasCrashingAlready = IsRocking;
	}
	VocClass::UpdateAt(Coord(PositionCoord), &MoveSoundAudioController); // RA2: the kept sound event follows the object

	if ((Frame & 63) == 63 && NavCom == NULL && !IsOnBridge && Get_Cell_Ptr()->Ramp == 0 && Is_On_Elevation() && HeightAGL == 0) {
		Scatter(Coord(0, 0, 0), true);
	}

	IPiggybackPtr piggy(Locomotion);
	if (piggy != NULL) {
		if (piggy->Is_Ok_To_End()) {
			piggy->End_Piggyback(&Locomotion);
		}
	}

	IonBlastYDrawOffset = 0;

	Enter_Object_Nearby();

	if (ParasiteEatingMe != NULL) { // RA2: the parasite eating this object runs its own AI
		ParasiteEatingMe->ParasiteImUsing->AI();
	}
}



/// <summary>
/// Draws the voxel body of this object.
/// This override shifts the draw position by whatever offset the locomotor asks for, so that
/// a bobbing or lurching locomotion carries the artwork along with it.
/// </summary>
void FootClass::Draw_Voxel(VoxelDataStruct const & voxeldata, int frame, int key, VoxelIndexClass * cache, Rect const & cliprect, Point2D const & point, Matrix3D const & matrix, int brightness, ShapeFlags_Type flags) const
{
	Matrix3D mtx = matrix;
	Point2D pt = point;

	if (Locomotion != NULL) {
		pt = Point2D(Locomotion->Draw_Point()) + point;
	}

	BASECLASS::Draw_Voxel(voxeldata, frame, key, cache, cliprect, pt, mtx, brightness, flags);
}





/// <summary>
/// Fetches the depth buffer bias to render this object with.
/// The locomotor's own adjustment is combined with the fudge factor for whatever the object
/// happens to be passing -- a column, a tunnel, a cliff or a bridge -- so that it sorts in
/// front of or behind the terrain as the type data asks.
/// </summary>
/// <returns>Returns with the amount to bias this object's depth by when it is drawn.</returns>
int FootClass::Get_Z_Adjust(void) const
{
	int adjust = 0;
	if (Locomotion != NULL) {
		adjust = Locomotion->Z_Adjust();
	}

	int column = TClass->ZFudgeColumn * Get_Z_Fudge_Column();
	int tunnel = TClass->ZFudgeTunnel * Get_Z_Fudge_Tunnel();
	int cliff = TClass->ZFudgeCliff * Get_Z_Fudge_Cliff();
	int bridge = 0;
	if (Is_Z_Fudge_Bridge()) {
		bridge = TClass->ZFudgeBridge;
	}

	int max_fudge = MAX(column, tunnel);
	max_fudge = MAX(max_fudge, cliff);
	max_fudge = MAX(max_fudge, bridge);

	adjust += BASECLASS::Get_Z_Adjust();
	adjust += max_fudge;
	return(adjust);
}


/// <summary>
/// Fetches the depth gradient to render this object with.
/// The locomotor decides, so that a climbing or diving locomotion can lay its object into the
/// depth buffer at the appropriate slope. Objects without a locomotor stand upright.
/// </summary>
/// <returns>Returns with the Z gradient to use, which is ZGRAD_90DEG by default.</returns>
ZGradientType FootClass::Get_Z_Gradient(void) const
{
	if (Locomotion != NULL) {
		return(Locomotion->Z_Gradient());
	}
	return(ZGRAD_90DEG);
}


/// <summary>
/// Draws the ground shadow for this object's voxel body.
/// The locomotor has the final say over whether a shadow is appropriate at all -- a burrowed
/// or submerged object casts none -- and over where the shadow should be offset to.
/// </summary>
void FootClass::Draw_Voxel_Shadow(VoxelDataStruct const & voxeldata, int layer_index, int key, VoxelIndexClass * cache, Rect const & cliprect, Point2D const & point, Matrix3D const & matrix, bool force_cache, Surface * surface, unsigned shadow_x, unsigned shadow_y) const // RA2: three more arguments
{
	if (Locomotion != NULL && Locomotion->Is_To_Have_Shadow() == (boolean)true) {
		Point2D drawpoint = point;
		if (Locomotion != NULL) {
			drawpoint = Point2D(Locomotion->Shadow_Point()) + point;
		}
		Techno_Draw_Voxel_Shadow(voxeldata, layer_index, key, cache, cliprect, drawpoint, matrix, force_cache, surface, shadow_x, shadow_y); // RA2: the callee takes the surface and the two shadow offsets (0x6D21E0)
	}
}



/// <summary>
/// Fetches the speed this object should be traveling at.
/// This is the type's maximum speed after the owner house's ground speed bias, any veteran
/// speed ability, and the object's current throttle have all been taken into account. A unit
/// that is carrying the flag is deliberately hobbled.
/// </summary>
/// <returns>Returns with the current speed of the object.</returns>
int FootClass::Current_Speed(void)
{
	int speed = (int)(Get_Max_Speed() * (double)House->Speed_Multiplier(Techno_Type_Class()) * SpeedBias); // RA2: the house's per-type speed factor (a float the exe widens to double) replaces GroundspeedBias
	if (Has_Ability(ABILITY_FASTER)) {
		speed = (int)(speed * Rule->VeteranSpeed); // RA2: VeteranSpeed is the whole factor, not 1 + VeteranSpeed
	}
	speed = (int)(speed * Speed); // RA2: as above, the product is truncated per step
	if (RTTI == RTTI_UNIT && ((UnitClass *)this)->Flagged != HOUSE_NONE) {
		speed /= 2;
	}
	return(speed);
}



/// <summary>
/// Draws this object.
/// Foot objects are rendered by their derived classes, each of which knows whether it is a
/// shape or a voxel, so this override deliberately does nothing.
/// </summary>
void FootClass::Draw_It(Point2D const &, Rect const &) const
{
}


/***********************************************************************************************
 * DriveClass::Limbo -- Prepares vehicle and then limbos it.                                   *
 *                                                                                             *
 *    This routine removes the occupation bits for the vehicle and also handles cleaning up    *
 *    any vehicle reservation bits. After this, it then proceeds with limboing the unit.       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was the vehicle limboed?                                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/22/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool FootClass::Limbo(void)
{
	if (!IsInLimbo && Techno_Type_Class() != NULL) { // RA2: only with a type
		Cell cell = LastAdjacencyCell;
		for (FacingType face = FACING_FIRST; face < FACING_COUNT; face++) {
			Cell newcell = Adjacent_Cell(cell, face);
			CellClass * cptr = &Map[newcell];
			cptr->AdjacentObjectCount--;
		}
		if (!ScenarioInit) { // RA2: not while the scenario starts
			Stop_Driver();
		}
		if (Locomotion != NULL) {
			Locomotion->Mark_All_Occupation_Bits(0);
		}
		MoveSoundAudioController.End(); // RA2: the movement sound ends
		if (Techno_Type_Class()->SensorsSight != 0) { // RA2: sensors withdrawn
			RemoveSensorsAt(CELL_NONE);
		}
		if (GetLastFlightMapCoords() != CELL_NONE) { // RA2: out of the tracker of aircraft in flight
			AircraftTracker.Remove(this);
		}
	}
	return(BASECLASS::Limbo());
}



/// <summary>
/// Loads this object from the specified stream.
/// This routine reads back the pending route and navigation orders after the base class
/// data, lets OLE recreate the locomotor, and then remaps every pointer the object holds.
/// </summary>
/// <returns>Returns with S_OK if the object was read successfully.</returns>
HRESULT STDMETHODCALLTYPE FootClass::Load(IStream * stream)
{
	Locomotion.Attach(0); // RA2: RA2's form
	HRESULT result = TechnoClass::Load(stream);
	if (SUCCEEDED(result)) {
		int count;
		result = stream->Read(&count, sizeof(count), 0);
		if (FAILED(result)) {
			return result;
		}
		new (&RouteQueue) DynamicVectorClass<AbstractClass *>;
		for (int index = 0; index < count; index++) {
			AbstractClass * pointer = 0;
			result = stream->Read(&pointer, sizeof(pointer), 0);
			if (FAILED(result)) {
				return result;
			}
			RouteQueue.Add(pointer);
		}

		result = stream->Read(&count, sizeof(count), 0);
		if (FAILED(result)) {
			return result;
		}
		new (&NavQueue) DynamicVectorClass<AbstractClass *>;
		for (index = 0; index < count; index++) {
			AbstractClass * pointer = 0;
			result = stream->Read(&pointer, sizeof(pointer), 0);
			if (FAILED(result)) {
				return result;
			}
			NavQueue.Add(pointer);
		}

		Locomotion.Detach();
		OleLoadFromStream(stream, IID_ILocomotion, (void **)&Locomotion);
		FollowTarget = 0;
		Swizzle_Pointer(&Team);
		Swizzle_Pointer(&Member);
		Swizzle_Pointer(&NavCom);
		Swizzle_Pointer(&SuspendedNavCom);
		Swizzle_Pointer(&MegaDestination);
		Swizzle_Pointer(&MegaTarget);
		Swizzle_Pointer(&PatrolCell);
		Swizzle_Pointer(&ParasiteEatingMe);
		Swizzle_Pointer(&ParasiteImUsing);
		MoveSoundAudioController.Init();
		IsMoveSoundPlaying = false;
		MoveSoundDelay = 0;
		for (index = 0; index < NavQueue.Count(); index++) {
			Swizzle_Pointer(&NavQueue[index]);
		}
		for (index = 0; index < RouteQueue.Count(); index++) {
			Swizzle_Pointer(&RouteQueue[index]);
		}
		result = S_OK;
	}
	return result;
}



/// <summary>
/// Saves this object to the specified stream.
/// This routine writes out the object's pending route and navigation orders after the base
/// class data, then hands the locomotor over to OLE so that it can persist itself.
/// </summary>
/// <returns>Returns with S_OK if the object was written successfully.</returns>
HRESULT STDMETHODCALLTYPE FootClass::Save(IStream * stream, BOOL cleardirty)
{
	HRESULT result = TechnoClass::Save(stream, cleardirty); // RA2: Save at 0x4C9FE0 matches only so
	if (SUCCEEDED(result)) {
		int index;
		int count = RouteQueue.Count();

		result = stream->Write(&count, sizeof(count), NULL);
		if (FAILED(result)) {
			return(result);
		}

		for (index = 0; index < count; index++) {
			result = stream->Write(&RouteQueue[index], sizeof(RouteQueue[index]), NULL);
			if (FAILED(result)) {
				return(result);
			}
		}

		count = NavQueue.Count();

		result = stream->Write(&count, sizeof(count), NULL);
		if (FAILED(result)) {
			return(result);
		}

		for (index = 0; index < count; index++) {
			result = stream->Write(&NavQueue[index], sizeof(NavQueue[index]), NULL);
			if (FAILED(result)) {
				return(result);
			}
		}

		IPersistStreamPtr persist(Locomotion);
		OleSaveToStream(persist, stream);

		result = S_OK;
	}
	return(result);
}


/// <summary>
/// Fetches the display layer this object belongs in.
/// The locomotor makes this decision, since it is the part that knows whether the object is
/// currently on the ground, in the air, or under it.
/// </summary>
/// <returns>Returns with the layer this object should be rendered in.</returns>
LayerType FootClass::In_Which_Layer(void) const
{
	return(Locomotion->In_Which_Layer());
}


/// <summary>
/// Sets this object's location on the map.
/// This routine will lift the object off the map before moving it and set it back down
/// afterward, so that the cells it is recorded as occupying stay correct.
/// </summary>
void FootClass::Set_Coord(Coord const & coord)
{
	if (IsDown) {
		Mark(MARK_UP);
		BASECLASS::Set_Coord(coord);
		Mark(MARK_DOWN);
	} else {
		BASECLASS::Set_Coord(coord);
	}
}


/// <summary>
/// Attaches a drop pod locomotor to this object.
/// This routine piggybacks a ballistic locomotor onto whatever the object was using, so
/// that it can be dropped from orbit. The original locomotor takes over again once the
/// descent is finished.
/// </summary>
void FootClass::Link_DropPod(void)
{
	ILocomotionPtr locomotion = Locomotion;
	ILocomotionPtr ballistic(CLSID_BallisticLocomotion);
	ballistic->Link_To_Object(this);
	IPiggybackPtr piggy(ballistic);
	piggy->Begin_Piggyback(locomotion);
	Locomotion = ballistic;

}


/// <summary>
/// Handles an idle object that may be sitting on a repair dock.
/// This routine is called when the object has run out of orders. It drops the object into
/// its idle mode, but withholds the automatic advance to the next waypoint when the object
/// is deliberately entering a building that can repair it.
/// </summary>
/// <returns>bool; Did the object pick up further orders instead of falling idle?</returns>
bool FootClass::Is_Docked_For_Repair(void)
{
	if (NavCom == NULL && TarCom == NULL) {
		TechnoClass * whom = Radio_Link(0); // RA2: the out-of-line link accessor, not Contact_With_Whom's inline array read
		BuildingClass * bptr = NULL;
		if (whom != NULL && whom->RTTI == RTTI_BUILDING) {
			bptr = (BuildingClass *)whom;
		}
		bool going_to_repair = (CurrentMission == MISSION_ENTER && bptr != NULL && bptr->Class->IsCanUnitRepair);
		return(Enter_Idle_Mode(false, going_to_repair == false));
	}
	return(false);
}



/// <summary>
/// Stops whatever animation goes with this object's movement.
/// Derived classes override this routine to halt their walking or turning artwork when the
/// object comes to rest. At this level there is nothing to stop.
/// </summary>
void FootClass::Stop_Movement_Animation(void)
{
}





/// <summary>
/// Can this object reach where the other object is headed?
/// This routine is a convenience for the zone test. It compares against the other object's
/// destination rather than where it happens to be standing at this instant.
/// </summary>
/// <returns>bool; Are the two in the same movement zone?</returns>
bool FootClass::Is_In_Same_Zone_As(ObjectClass const * object) const
{
	return(Is_In_Same_Zone(object->Destination_Coord((TechnoClass *)this).As_Cell())); // RA2: Is_In_Same_Zone_As at 0x4CA350 matches only so
}


/// <summary>
/// Adds this object's movement state to the consistency check.
/// This routine is used by the network code to prove that every machine agrees about where
/// this object is going and what it is doing on the way.
/// </summary>
void FootClass::Compute_CRC(CRCEngine & crc) const
{
	BASECLASS::Compute_CRC(crc);
	crc(ThreatAvoidanceCoefficient);
	crc(LastPathingCell.Length());
	crc(LastTubeCoord.Length());
	crc(Speed);
	crc(SpeedBias);
	if (NavCom != NULL) crc(NavCom->Fetch_ID());
	if (SuspendedNavCom != NULL) crc(SuspendedNavCom->Fetch_ID());
	if (Team != NULL) crc(Team->Fetch_ID());
	if (Member != NULL) crc(Member->Fetch_ID());
	crc((int)PathDelay);
	crc(TryTryAgain);
	crc((int)BaseAttackTimer);
	crc(HeadToCoord.Length());
	crc(CurrentTube);
	crc(CurrentTubeDir);
	crc(IsScanLimited);
	crc(IsInitiated);
	crc(IsNewNavCom);
	crc(IsPlanningToLook);
	crc(IsDeploying);
	crc(IsFiring);
	crc(IsRotating);
	crc(IsUnloading);
	crc(IsNavQueueLoop);
	crc(IsScattering);
	crc(IsIdle);
	crc(IonBlastYDrawOffset);
}


/// <summary>
/// Can this object cloak itself again?
/// Some objects are forbidden from cloaking while they are still under way, so they must
/// come to a halt before they can disappear.
/// </summary>
/// <returns>bool; Is the object allowed to recloak?</returns>
bool FootClass::Is_Allowed_To_Recloak(void) const
{
	if (BASECLASS::Is_Allowed_To_Recloak()) {
		if (!Techno_Type_Class()->IsCloakStop || !Locomotion->Is_Moving_Now()) { // RA2: Is_Allowed_To_Recloak at 0x4CA6A0 matches only so
			return(true);
		}
	}
	return(false);
}


/// <summary>
/// Fetches the location this object is traveling toward.
/// An object making its way through a tunnel reports the tunnel exit, since that is where
/// it will surface. An object with nowhere to be reports its own center.
/// </summary>
/// <returns>Returns with the coordinate being headed for.</returns>
Coord FootClass::Destination_Coord(TechnoClass *) const // RA2: the docking object
{
	if (CurrentTube >= TUBE_FIRST) {
		Cell exit = Tubes[CurrentTube]->Exit;
		return(exit.As_Coord());
	} else {
		CoordStruct tmp = Locomotion->Head_To_Coord();
		Coord head_to = tmp;
		if (head_to != COORD_NONE) {
			return(head_to);
		}
		return(Center_Coord());
	}
}


/// <summary>
/// Handles this object changing hands.
/// This routine remembers where the object was standing at the moment it was captured, so
/// that its adjacency tracking remains valid under the new owner.
/// </summary>
/// <returns>bool; Was the object captured?</returns>
bool FootClass::Captured(HouseClass * newowner, bool announce) // RA2: announce
{
	if (TechnoClass::Captured(newowner, true)) {
		if (!In_Air()) {
			LastAdjacencyCell = Get_Cell();
		}
		return(true);
	}
	return(false);
}



/// <summary>
/// Is this object considered slow?
/// Slow is meant in the slow-witted sense. A team that has IsGuardSlower set will tweak
/// itself to better guard the members that answer true -- typically the unarmed, the
/// unpromotable, and anything that would rather deploy than fight.
/// </summary>
/// <returns>bool; Is the object considered slow?</returns>
bool FootClass::Is_Considered_Slow(void)
{
	if (Get_Class_Weapon_Data(0) == NULL) {
		return(true);
	}
	if (Techno_Type_Class()->Level == -1) { // RA2: Is_Considered_Slow at 0x4CA820 matches only so
		return(true);
	}
	if (Techno_Type_Class()->DeploysInto != NULL) {
		return(true);
	}
	if (Techno_Type_Class()->MaxSpeed > 13) {
		return(false);
	}
	return(true);
}


/// <summary>
/// Handles this object being unable to make any progress.
/// This routine gives a hunting object the chance to forget its orders and look for
/// something else to do, rather than grinding away against whatever is in its path.
/// </summary>
void FootClass::On_Movement_Blocked(void)
{
	if (Get_Mission() == MISSION_HUNT) { // RA2: On_Movement_Blocked at 0x4CA880 matches only so
		Assign_Target(NULL);
		Assign_Destination(NULL);
	}
}


/// <summary>
/// Draws the line that shows this object's current order: to its target in red from the
/// firing point, or else in green from the object to its destination (the last queued one).
/// Unless forced, the line only appears for a short while after the order is given.
/// </summary>
/// <param name="force">Draw the line even after the action line timer ran out?</param>
/// <param name="dashed">Draw the line to the destination dashed?</param>
void FootClass::Draw_Action_Line(bool force, int dashed) const // RA2: two arguments; the TibSun body moved to Draw_Action_Line(void)
{
	Coord start_coord;
	Coord end_coord;

	if (TarCom != NULL || NavCom != NULL) {
		if (!force && ActionLineTimer.Value() <= 0) {
			return;
		}

		if (TarCom != NULL) {
			start_coord = Turret_Coord();
			end_coord = Predict_Target_Coord();
			TechnoClass::Draw_Action_Line(start_coord, end_coord, DSurface::Deconstruct_Hicolor_Pixel(NormalDrawer->Convert_Pixel(RED)), false, false);
			return;
		}

		start_coord = PositionCoord;
		if (RouteQueue.Count() == 0) {
			end_coord = NavCom->Center_Coord();
		} else {
			end_coord = RouteQueue[RouteQueue.Count() - 1]->Center_Coord();
		}
		if (Map.In_Radar(end_coord.As_Cell()) && Map[end_coord].IsUnderBridge) {
			end_coord.Z = BRIDGE_LEPTON_HEIGHT + Map.Get_Height_GL(end_coord);
		}
		TechnoClass::Draw_Action_Line(start_coord, end_coord, DSurface::Deconstruct_Hicolor_Pixel(NormalDrawer->Convert_Pixel(GREEN)), dashed, false);
	}
}


/// <summary>
/// Fetches the marker rectangle for an action line end point: a small block centered about the pixel.
/// </summary>
static inline Rect Action_Line_End_Rect(Point2D const & point) // RA2
{
	return(Rect(point - Point2D(2, 2), 3, 3));
}


/// <summary>
/// Fills the marker of an action line end point, clipped to the tactical map.
/// </summary>
static inline void Fill_Action_Line_End(Point2D const & point, int pixel) // RA2
{
	CompositeSurface->Fill_Rect(Intersect(TacticalRect, Action_Line_End_Rect(point)), pixel);
}


/// <summary>
/// Draws the line that shows this object's current order in the owner's color.
/// This routine marks the two ends of the order -- the firing point and its target, or the
/// object and its destination -- and joins them with a dashed line clipped to the tactical
/// map whose color pulses toward white and whose dashes crawl along it over time.
/// </summary>
void FootClass::Draw_Action_Line(void) const // RA2: TibSun's Draw_Action_Line, no longer virtual, at 0x4CAB90
{
	static bool _pattern[16] = { // RA2
		true,
		true,
		true,
		true,
		true,
		false,
		false,
		false,
		true,
		true,
		true,
		true,
		true,
		false,
		false,
		false
	};

	RGBClass white(255, 255, 255); // RA2
	RGBClass color; // RA2: the owner's color

	if (!TarCom && !NavCom) {
		return;
	}

	Coord start_coord; // RA2: no ActionLineTimer check
	Coord end_coord;

	if (TarCom != NULL) {
		start_coord = Turret_Coord();
		end_coord = Predict_Target_Coord();
	} else {
		start_coord = PositionCoord;
		if (RouteQueue.Count() == 0) {
			end_coord = NavCom->Center_Coord();
		} else {
			end_coord = RouteQueue[RouteQueue.Count() - 1]->Center_Coord();
		}
		if (Map.In_Radar(end_coord.As_Cell()) && Map[end_coord].IsUnderBridge) {
			end_coord.Z = BRIDGE_LEPTON_HEIGHT + Map.Get_Height_GL(end_coord);
		}
	}

	Point2D start_point;
	Point2D end_point;

	TacticalMap->Coord_To_Pixel(start_coord, start_point);
	TacticalMap->Coord_To_Pixel(end_coord, end_point);

	start_point.Y += TacticalRect.Y;
	end_point.Y += TacticalRect.Y;

	color = House->RemapColorRGB; // RA2
	int pixel = DSurface::Build_Hicolor_Pixel(color); // RA2
	Fill_Action_Line_End(start_point, pixel); // RA2: pixel
	Fill_Action_Line_End(end_point, pixel); // RA2: pixel

	Rect tacticalr = TacticalRect;
	if (Clip_Line_To_Rect(start_point, end_point, tacticalr)) {
		unsigned long time = timeGetTime(); // RA2: the color pulses toward white
		unsigned amount = time & 0x3FF;
		if (!(amount & 0x200)) {
			if (amount & 0x100) {
				amount ^= 0xFF;
			}
			color.Adjust(amount & 0xFF, white);
		}
		CompositeSurface->Draw_Masked_Dashed_Line(start_point, end_point, DSurface::Build_Hicolor_Pixel(color), _pattern, (-(int)time >> 5) & 15, false); // RA2: dashed
	}
}









/// <summary>
/// Fetches how strongly this object should steer around danger.
/// This routine is used by the path finder when weighting the threat of the regions it
/// passes through. A team that has been told to avoid threats forces its members to the
/// maximum avoidance.
/// </summary>
/// <returns>Returns with the avoidance coefficient, where zero ignores threats entirely.</returns>
double FootClass::Threat_Avoidance_Value(void) const
{
	if (Team != NULL && Team->Class->AvoidThreats) {
		return(1.0);
	}
	return(ThreatAvoidanceCoefficient);
}


/// <summary>
/// Should this object be quietly removed once it wanders off the map?
/// This routine is consulted as objects reach the edge of the world. A team member only
/// qualifies once its team is actually leaving, and a train is spared while its destination
/// is still within the local view.
/// </summary>
/// <returns>bool; Should the object be deleted?</returns>
bool FootClass::Should_Delete_Off_Map(void)
{
	if (!IsLocked) {
		return(false);
	}
	if (Team != NULL && !Team->Is_Leaving_Map()) {
		return(false);
	}
	if (TClass->IsTrain && NavCom != NULL && Map.In_Local_Radar(NavCom->Center_Coord())) {
		return(false);
	}
	return(true);
}


/// <summary>
/// Assigns this object to one of the player's waypoint paths.
/// This routine is used when a recorded path is handed to the object, and also to clear
/// that assignment again.
/// </summary>
/// <param name="path">The path to follow, or PATH_NONE to clear the assignment.</param>
/// <param name="index">The waypoint within the path to head for first.</param>
void FootClass::Set_Waypoint_Path(PathType path, char index)
{
	if (path != PATH_NONE) {
		CurrentPath = path;
		NextWaypoint = index;
		WaypointClass * wpc = PlayerPtr->Paths[path]->Get_Waypoint(index); // RA2: Set_Waypoint_Path at 0x4CB060 matches only so
		WaypointTargetCell = wpc->Location.As_Cell();
	} else {
		CurrentPath = PATH_NONE;
		NextWaypoint = 0;
		WaypointTargetCell = Cell(0, 0);
	}
	WaypointOffsetCell = Cell(0, 0);
}


/// <summary>
/// Carries out the order recorded at a waypoint.
/// This routine performs the same click the player would have performed on the waypoint's
/// cell, shifting the order to a nearby reachable cell when the waypoint itself is occupied
/// or cannot be walked to directly. A waypoint that cannot be acted upon at all abandons
/// the path.
/// </summary>
/// <param name="waypoint">The waypoint to act upon. A NULL waypoint abandons the path.</param>
bool FootClass::Execute_Waypoint_Path(WaypointClass * waypoint) // RA2: returns bool (the old void had no result)
{
	bool action_succeeded = false;

	if (IsOnPatrol) {
		IsOnWaypointPatrol = true;
	}

	if (waypoint != NULL) {
		PlayerPtr->Fetch_Waypoint_Data(waypoint, CurrentPath, NextWaypoint);

		Coord waypoint_coord = waypoint->Location;
		ObjectClass * occupying_object = Map[waypoint_coord].Cell_Occupier();
		TechnoClass * occupying_techno = Dynamic_Cast<TechnoClass *>(occupying_object);

		WaypointTargetCell = waypoint_coord;

		bool was_voice_allowed = AllowVoice;
		AllowVoice = false;

		Cell waypoint_cell = waypoint_coord;

		ActionType action; // RA2: one action variable serves the occupier, waypoint and adjusted queries
		bool is_direct_waypoint = true;
		bool should_click_occupier = false;

		if (occupying_techno != NULL) {
			action = What_Action(occupying_object, true);

			if (PlayerPtr->Is_Ally(occupying_techno->House)) {
				if (occupying_techno->RTTI == RTTI_BUILDING) {
					BuildingClass * building = (BuildingClass *)occupying_techno;

					bool needs_repair = (building->Class->IsCanUnitRepair &&
						Strength < (int)TClass->MaxStrength &&
						RTTI != RTTI_INFANTRY);

					bool needs_reload = (building->Class->IsCanUnitReload &&
						Ammo < TClass->MaxAmmo &&
						Ammo != -1);

					bool is_allied_building = building->House->Is_Ally(PlayerPtr);

					if ((needs_repair || needs_reload) &&
						is_allied_building &&
						action != ACTION_SELECT &&
						action != ACTION_TOGGLE_SELECT) {
						should_click_occupier = true;
					}
				}
			} else if (action != ACTION_TOGGLE_SELECT &&
				action != ACTION_SELECT &&
				action != ACTION_MOVE &&
				action != ACTION_NOMOVE) {
				should_click_occupier = true;
			}

			if (!should_click_occupier) {
				/*
				 * The waypoint cell is occupied but the occupier wasn't clicked on.
				 * Try to shift the waypoint to a reachable nearby cell, then carry
				 * on with the normal waypoint processing.
				 */
				Cell nearby_cell = Map.Nearby_Location(
					waypoint_cell,
					TClass->Speed,
					Map.Get_Cell_Zone(waypoint_cell, TClass->MZone, Map[waypoint_cell].IsUnderBridge),
					TClass->MZone,
					false,
					Point2D(1, 1),
					false,
					true,
					false,
					true,
					Cell(0, 0));

				if (nearby_cell != CELL_NONE) {
					WaypointOffsetCell = nearby_cell - waypoint_cell;
					is_direct_waypoint = false;
				}
			}
		}

		if (should_click_occupier) {
			action = What_Action(occupying_object, true);
			action_succeeded = Active_Click_With(action, occupying_object, true);
			WaypointOffsetCell = Cell(0, 0);
		} else {

			action = What_Action(waypoint_cell, false, true);
			Cell waypoint_offset = WaypointOffsetCell;
			Cell adjusted_waypoint = waypoint_cell + waypoint_offset;
			CellClass * adjusted_cell = &Map[adjusted_waypoint];
			CellClass * waypoint_map_cell = &Map[waypoint_cell];
			ActionType adjusted_action = What_Action(adjusted_waypoint, false, true);

			int waypoint_zone = Map.Get_Cell_Zone(waypoint_cell, TClass->MZone, waypoint_map_cell->IsUnderBridge); // RA2: the waypoint zone comes first
			int adjusted_zone = Map.Get_Cell_Zone(adjusted_waypoint, TClass->MZone, adjusted_cell->IsUnderBridge);
			int destination_zone = Map.Get_Cell_Zone(Destination_Coord().As_Cell(), TClass->MZone, Is_Moving_Onto_Bridge());
			MoveType entry_result = Can_Enter_Cell(adjusted_cell, FACING_NONE,
				adjusted_cell->Height + (BRIDGE_CELL_HEIGHT * adjusted_cell->IsUnderBridge), 0, true);

			int walk_distance_limit = MAX(abs((int)waypoint_offset.X), abs((int)waypoint_offset.Y));
			walk_distance_limit = MIN(5, walk_distance_limit + 3);

			if ((action == adjusted_action || !is_direct_waypoint) && // RA2: the positive test with the adjusted click first, as the original
				entry_result == MOVE_OK &&
				waypoint_zone == adjusted_zone &&
				destination_zone == waypoint_zone &&
				(!is_direct_waypoint || Search.Test_Cell_Walk(adjusted_waypoint, waypoint_cell, this,
					adjusted_cell->IsUnderBridge, waypoint_map_cell->IsUnderBridge, MZONE_NONE) <= walk_distance_limit)) {
				action_succeeded = Active_Click_With(action, adjusted_waypoint, Cell(-1, -1), true); // RA2: a second cell, (-1, -1) at 0x4CB110
				WaypointTargetCell = adjusted_waypoint;
			} else {
				action_succeeded = Active_Click_With(action, waypoint_cell, Cell(-1, -1), true);
				WaypointOffsetCell = Cell(0, 0);
			}
		}

		if (was_voice_allowed) {
			AllowVoice = true;
		}
	}

	if (!action_succeeded) {
		CurrentPath = PATH_NONE;
		NextWaypoint = 0;
		WaypointOffsetCell = Cell(0, 0);
		WaypointTargetCell = Cell(0, 0);
	}

	IsOnWaypointPatrol = false;
	return(action_succeeded); // RA2: the success flag is the return value
}




/***********************************************************************************************
 * FootClass::Tiberium_Check -- Search for and head toward nearest available Tiberium patch.   *
 *                                                                                             *
 *    This routine is used to move a harvester to a place where it can load up with            *
 *    Tiberium. It will return true only if it can start harvesting. Otherwise, it sets        *
 *    the navigation computer toward the nearest Tiberium and lets the unit head there         *
 *    automatically.                                                                           *
 *                                                                                             *
 * INPUT:   center   -- Reference to the center of the radius scan.                            *
 *                                                                                             *
 *          x,y      -- Relative offset from the center cell to perform the check upon.        *
 *                                                                                             *
 * OUTPUT:  bool; Is it located directly over a Tiberium patch?                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool FootClass::Tiberium_Check(Cell & center)
{
	if (!Map.In_Local_Radar(center)) return(false);

	if ((Session.Type != GAME_NORMAL || (!IsOwnedByPlayer || !Map.Is_Shrouded(center.As_Coord(Map.Get_Height_GL(center)))))) {
		if (!Map.Is_Same_Cell_Zone(Destination_Coord().As_Cell(), center, TClass->MZone, Is_Moving_Onto_Bridge(), false, false)) return(false);
		CellClass * cptr = &Map[center];
		if (cptr->Land_Type() == LAND_TIBERIUM && !Can_Enter_Cell(cptr)) { // RA2: Tiberium_Check at 0x4CB6D0 matches only so
			return(true);
		}
	}
	return(false);
}


/***********************************************************************************************
 * FootClass::Goto_Tiberium -- Searches for and heads toward tiberium.                         *
 *                                                                                             *
 *    This routine will cause the unit to search for and head toward nearby Tiberium. When     *
 *    the Tiberium is reached, then this routine should not be called again until such time    *
 *    as additional harvesting is required. When this routine returns false, then it should    *
 *    be called again until such time as it returns true.                                      *
 *                                                                                             *
 * INPUT:   rad = size of ring to search                                                       *
 *                                                                                             *
 * OUTPUT:  Has the unit reached Tiberium and harvesting should begin?                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/22/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool FootClass::Goto_Tiberium(int rad, bool allow_weighted)
{
	if (NavCom == NULL) {
		Cell newcell = Search_For_Tiberium(rad, allow_weighted);
		if (newcell != CELL_NONE) {
			if (newcell == Center_Coord().As_Cell()) {
				return(true);
			}
			Assign_Destination(&Map[newcell]);
		}
	}
	return(false);
}


/// <summary>
/// Finds the most valuable Tiberium patch within reach.
/// This routine is used by the harvest mission to pick somewhere to go. A computer
/// controlled harvester in a skirmish or multiplayer game defers to the weighted search
/// instead, so that the house's harvesters do not all pile onto one patch.
/// </summary>
/// <param name="rad">The limit of the search, expressed in cells from this object.</param>
/// <param name="allow_weighted">Is the weighted search allowed to be used?</param>
/// <returns>Returns with the cell of the Tiberium found. Otherwise, CELL_NONE is
/// returned.</returns>
Cell FootClass::Search_For_Tiberium(int rad, bool allow_weighted)
{
	Cell bestcell = CELL_NONE;

	// RA2: no weighted search here, whatever allow_weighted says
	Cell center = Center_Coord().As_Cell();
	if (Map[center].Land_Type() == LAND_TIBERIUM) {
		return(center);
	} else {

		/*
		**	Perform a ring search outward from the center.
		*/
		Cell cell(0,0);
		int bestvalue = -1;
		for (int radius = 1; radius < rad; radius++) {
			for (int x = -radius; x <= radius; x++) {
				cell = center + Cell(x, -radius);
				if (Tiberium_Check(cell)) {
					int value = Map[cell].Tiberium_Value();
					if (value > bestvalue) {
						bestvalue = value;
						bestcell = cell;
					}
				}

				cell = center + Cell(x, +radius);
				if (Tiberium_Check(cell)) {
					int value = Map[cell].Tiberium_Value();
					if (value > bestvalue) {
						bestvalue = value;
						bestcell = cell;
					}
				}

				cell = center + Cell(-radius, x);
				if (Tiberium_Check(cell)) {
					int value = Map[cell].Tiberium_Value();
					if (value > bestvalue) {
						bestvalue = value;
						bestcell = cell;
					}
				}

				cell = center + Cell(+radius, x);
				if (Tiberium_Check(cell)) {
					int value = Map[cell].Tiberium_Value();
					if (value > bestvalue) {
						bestvalue = value;
						bestcell = cell;
					}
				}
			}
			if (bestvalue != -1) break;
		}
	}
	return(bestcell);
}



/// <summary>
/// Finds a Tiberium patch, weighted by richness and by distance.
/// This routine picks at random from the reachable patches, favoring the rich ones close to
/// home. It is used in place of the plain search so that a house's harvesters spread out
/// across the field instead of all converging on the same patch.
/// </summary>
/// <param name="rad">The limit of the search, expressed in cells from this object.</param>
/// <returns>Returns with the cell of the Tiberium chosen. Otherwise, CELL_NONE is
/// returned.</returns>
#ifdef RA2_NON_MATCHING // not exact: 1045 bytes differ; ra2-decomp's twin text (Yuri's Revenge 0x004DD2F0) with RA2's harvester count in place of YR's CountResourceGatherers: the distribution's vector constructor 0x4CE710 is now called as in the image; left are the four Cell sums ahead of Tiberium_Check (the image computes X with lea and stores X before Y) and the lea/push pairs ahead of Add. Any archive form in place of the twin's max/MAX forms expands the constructor again (1207 B; lane O49)
Cell FootClass::Search_For_Tiberium_Weighted(int rad)
{
	DiscreteDistributionClass<CellClass> celldist;

	Cell center = Center_Coord().As_Cell();
	if (Map[center].Land_Type() == LAND_TIBERIUM) {
		return(center);
	}

	int numharv = House->AUQuantity.Value(House->Get_First_Ownable(Rule->HarvesterUnit)->HeapID); // RA2: the first harvester this house may own, not the list's first
	numharv = max(1, numharv);

	for (int radius = 1; radius < rad; radius++) {
		int share = radius * 3 / numharv; // RA2: three cells per ring step
		bool northrun = false;
		bool southrun = false;
		bool westrun = false;
		bool eastrun = false;
		int divisor = max(1, share);
		double scale = 1.0 / (double)divisor;
		for (int x = -radius; x <= radius; x++) {
			Cell cell;
			cell = Cell(x, -radius) + center;
			if (Tiberium_Check(cell)) {
				if (!northrun) {
					CellClass *cptr = &Map[cell];
					int intweight = (int)MAX(1.0, cptr->Tiberium_Value() * scale);
					northrun = true;
					celldist.Add(cptr, intweight);
				}
			} else {
				northrun = false;
			}

			cell = Cell(x, +radius) + center;
			if (Tiberium_Check(cell)) {
				if (!southrun) {
					CellClass *cptr = &Map[cell];
					int intweight = (int)MAX(1.0, cptr->Tiberium_Value() * scale);
					southrun = true;
					celldist.Add(cptr, intweight);
				}
			} else {
				southrun = false;
			}

			cell = Cell(-radius, x) + center;
			if (Tiberium_Check(cell)) {
				if (!westrun) {
					CellClass *cptr = &Map[cell];
					int intweight = (int)MAX(1.0, cptr->Tiberium_Value() * scale);
					westrun = true;
					celldist.Add(cptr, intweight);
				}
			} else {
				westrun = false;
			}

			cell = Cell(+radius, x) + center;
			if (Tiberium_Check(cell)) {
				if (!eastrun) {
					CellClass *cptr = &Map[cell];
					int intweight = (int)MAX(1.0, cptr->Tiberium_Value() * scale);
					eastrun = true;
					celldist.Add(cptr, intweight);
				}
			} else {
				eastrun = false;
			}
		}
	}

	CellClass *cellptr = celldist.Sample();
	if (cellptr != NULL) {
		return(cellptr->CellID);
	}
	return(CELL_NONE);
}
#endif






/// <summary>
/// Finds the nearest harvestable weed patch.
/// This routine is used by the weed harvesting logic to pick somewhere to go. Only patches
/// the harvester can actually reach are considered.
/// </summary>
/// <param name="rad">The limit of the search, expressed in cells from this object.</param>
/// <returns>Returns with the cell of the weed found. Otherwise, CELL_NONE is returned.</returns>
Cell FootClass::Search_For_Weed(int rad)
{
	Cell bestcell = CELL_NONE;
	Cell center = Center_Coord().As_Cell();
	if (Weed_Check(center, 0, 0)) {
		return(center);
	} else {
		Cell cell(0, 0); // RA2: Search_For_Weed at 0x4CC100 matches only so
		for (int radius = 1; radius < rad; radius++) {
			for (int x = -radius; x <= radius; x++) {
				cell = center;
				if (Weed_Check(cell, x, -radius)) {
					bestcell = cell;
				}

				cell = center;
				if (Weed_Check(cell, x, +radius)) {
					bestcell = cell;
				}

				cell = center;
				if (Weed_Check(cell, -radius, x)) {
					bestcell = cell;
				}

				cell = center;
				if (Weed_Check(cell, +radius, x)) {
					bestcell = cell;
				}
			}
			if (bestcell != CELL_NONE) break;
		}
	}
	return(bestcell);
}


/***********************************************************************************************
 * FootClass::Weed_Check -- Search for and head toward nearest available Weed patch.           *
 *                                                                                             *
 *    This routine is used to move a harvester to a place where it can load up with            *
 *    Weed. It will return true only if it can start harvesting. Otherwise, it sets            *
 *    the navigation computer toward the nearest Weed and lets the unit head there             *
 *    automatically.                                                                           *
 *                                                                                             *
 * INPUT:   center   -- Reference to the center of the radius scan.                            *
 *                                                                                             *
 *          x,y      -- Relative offset from the center cell to perform the check upon.        *
 *                                                                                             *
 * OUTPUT:  bool; Is it located directly over a Weed patch?                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool FootClass::Weed_Check(Cell & center, int x, int y)
{
	Cell cell = center + Cell(x, y);

	if (!Map.In_Local_Radar(cell)) return(false);

	center = cell;

	if ((Session.Type != GAME_NORMAL || (!IsOwnedByPlayer || !Map.Is_Shrouded(center.As_Coord(Map.Get_Height_GL(center)))))) {
		if (!Map.Is_Same_Cell_Zone(Destination_Coord().As_Cell(), center, TClass->MZone, Is_Moving_Onto_Bridge(), false, false)) return(false);
		CellClass * cptr = &Map[center];
		if (!Can_Enter_Cell(cptr) && cptr->Land_Type() == LAND_WEEDS && cptr->OverlayData >= OVERLAYDATA_FIRST_SOLID_VEIN) {
			return(true);
		}
	}
	return(false);
}


/***********************************************************************************************
 * FootClass::Goto_Weed -- Searches for and heads toward weed.                                 *
 *                                                                                             *
 *    This routine will cause the unit to search for and head toward nearby Weed. When         *
 *    the Weed is reached, then this routine should not be called again until such time        *
 *    as additional harvesting is required. When this routine returns false, then it should    *
 *    be called again until such time as it returns true.                                      *
 *                                                                                             *
 * INPUT:   rad = size of ring to search                                                       *
 *                                                                                             *
 * OUTPUT:  Has the unit reached Weed and harvesting should begin?                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/22/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool FootClass::Goto_Weed(int rad)
{
	if (NavCom == NULL) {
		Cell newcell = Search_For_Weed(rad);
		if (newcell != CELL_NONE) {
			if (newcell == Center_Coord().As_Cell()) {
				return(true);
			}
			Assign_Destination(&Map[newcell]);
		}
	}
	return(false);
}


/// <summary>
/// Is this object about to step onto a bridge?
/// An object traveling through a tunnel is never considered to be heading onto a bridge,
/// since its destination is underground rather than on the deck.
/// </summary>
/// <returns>bool; Is the object moving onto a bridge?</returns>
bool FootClass::Is_Moving_Onto_Bridge(TechnoClass *techno) const // RA2: the docking object
{
	if (CurrentTube >= 0) {
		return(false);
	}
	return ObjectClass::Is_Moving_Onto_Bridge(techno);
}



/***********************************************************************************************
 * FootClass::Is_LZ_Clear -- Determines if landing zone is free for landing.                   *
 *                                                                                             *
 *    This routine examines the landing zone (as specified by the target parameter) in order   *
 *    to determine if it is free to be landed upon. Call this routine when it is necessary     *
 *    to double check this. Typically this occurs right before a helicopter lands and also     *
 *    when determining the landing zone in the first place.                                    *
 *                                                                                             *
 * INPUT:   target   -- The target that is the "landing zone".                                 *
 *                                                                                             *
 * OUTPUT:  bool; Is the landing zone clear for landing?                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/31/94   JLB : Created.                                                                 *
 *=============================================================================================*/
bool FootClass::Is_LZ_Clear(AbstractClass * target) const
{
	if (target == 0) { // RA2: RA2's form
		return(false);
	}
	Cell cell = target->Center_Coord().As_Cell();
	if (!Map.In_Local_Radar(cell, true)) {
		return(false);
	}
	TechnoClass * techno = ((CellClass *)&Map[cell])->Cell_Techno(Point2D(0, 0), 0, 0);
	if (techno != 0) {
		if ((TechnoClass const *)this != techno && Radio_Link(0) != techno) {
			if (!Techno_Type_Class()->Spawned || (techno->SpawnManager == 0 && !techno->Techno_Type_Class()->Spawned)) {
				return(false);
			}
		}
	} else {
		if (!((CellClass *)&Map[cell])->Is_Clear_To_Move(SPEED_TRACK, 0, 0, -1, MZONE_NORMAL, -1, 1)) {
			return(false);
		}
		for (int index = 0; index < Aircraft.Count(); index++) {
			AircraftClass * aircraft = Aircraft[index];
			if (aircraft->IsActive && !aircraft->IsInLimbo && aircraft->NavCom == target && aircraft != this) {
				return(false);
			}
		}
	}
	return(true);
}



/// <summary>
/// Determines what action to perform if this object is ordered onto the cell.
/// This routine adds the shroud restriction to the base class decision. A shrouded cell can
/// only be moved toward, and only if this object is allowed to travel into the shroud at
/// all. Patrol waypoint orders survive the restriction unchanged.
/// </summary>
/// <returns>Returns with the action to perform.</returns>
ActionType FootClass::What_Action(Cell const & cell, bool check_fog, bool disallow_force) const
{
	ActionType action = BASECLASS::What_Action(cell, check_fog, disallow_force);

	Coord coord = cell;
	coord.Z = Map.Get_Height_GL(coord);
	if (Map[coord].IsUnderBridge) {
		coord.Z += BRIDGE_LEPTON_HEIGHT;
	}

	if (Map.Is_Shrouded(coord) && action != ACTION_NONE) {
		if (Techno_Type_Class()->IsMoveToShroud && Map.In_Local_Radar(cell)) { // RA2: What_Action at 0x4CC650 matches only so
			if (action != ACTION_PATROL_WAYPOINT) {
				action = ACTION_MOVE;
			}
		} else {
			action = ACTION_NOMOVE;
		}
	}

	return(action);
}


/// <summary>
/// Determines what action to perform if this object is ordered onto the target.
/// This routine adds the shroud restriction to the base class decision. A target hidden
/// under the shroud can only be moved toward, and only if this object is allowed to travel
/// into the shroud at all.
/// </summary>
/// <returns>Returns with the action to perform.</returns>
ActionType FootClass::What_Action(ObjectClass const * target, bool disallow_force) const
{
	ActionType action = BASECLASS::What_Action(target, disallow_force);

	Coord coord = target->Center_Coord();
	coord.Z = Map.Get_Height_GL(coord);
	if (Map[coord].IsUnderBridge) {
		coord.Z += BRIDGE_LEPTON_HEIGHT;
	}

	if (Map.Is_Shrouded(coord) && action != ACTION_NONE && Session.Type == GAME_NORMAL) {
		if (Techno_Type_Class()->IsMoveToShroud) { // RA2: What_Action at 0x4CC740 matches only so
			action = ACTION_MOVE;
		} else {
			action = ACTION_NOMOVE;
		}
	}

	return(action);
}


/// <summary>
/// Handles the rescue mission state machine.
/// This routine will have the object engage any threat close to the spot the mission began
/// at, then head for whatever destination its house nominates. Once it arrives, it settles
/// into a guard area mission.
/// </summary>
/// <returns>The delay in game frames before this mission should be processed again.</returns>
int FootClass::Do_MISSION_RESCUE(void)
{
	enum {
		ENGAGE_THREATS, // RA2: Do_MISSION_RESCUE at 0x4CC800 matches only so
		TRAVELLING
	};

	switch (Status) {

		case ENGAGE_THREATS:
			if (TarCom != NULL) {
				Approach_Target();
			} else {
				if (ArchiveTarget == NULL) {
					ArchiveTarget = &Map[(Coord const &)PositionCoord];
				}

				IsScanLimited = false;

				ObjectClass * threat = (ObjectClass *)Greatest_Threat(THREAT_NORMAL, ArchiveTarget->Center_Coord(), false);
				if (threat != NULL) {
					if (threat->Distance(ArchiveTarget->Center_Coord()) < Threat_Range(1) * 1.5) {
						Assign_Target(threat);
					}
				}

				if (TarCom != NULL) {
					return(1);
				} else {
					Status = TRAVELLING;
					Cell where = House->Where_To_Go(this);
					if (where != CELL_NONE) {
						Assign_Destination(&Map[where]);
					} else {
						Assign_Destination(NULL);
					}
					ArchiveTarget = NULL;
				}
			}
			break;

		case TRAVELLING:
			if (NavCom == NULL) {
				ArchiveTarget = NULL;
				QueueMission(MISSION_GUARD_AREA, false);
				Commence();
			}
			break;
	}

	return(Current_Mission_Control().Normal_Delay() + Random_Pick(0, 2));
}


/// <summary>
/// Fetches the cell this object should actually be sent to.
/// This routine will find the closest reachable cell to the one ordered, so that clicking
/// on an impassable or shrouded location still sends the object as near as it can
/// reasonably get. An object that is not allowed to move into shroud refuses the order
/// outright.
/// </summary>
/// <param name="where">The cell that was ordered as the destination.</param>
/// <param name="consider_fog">Should the fog of war be considered when deciding the action?</param>
/// <returns>Returns with the cell to head toward, or CELL_NONE if the order is refused.</returns>
Cell FootClass::Move_Order(Cell const & where, bool consider_fog)
{
	/*
	**	Find the closest same-zoned cell to where the unit currently is.
	**	This will allow the unit to come as close to the destination cell
	**	as is reasonably possible, when clicking on an impassable cell
	**	(as is likely when clicking in the shroud.)  It looks for the
	**	nearest cell using an expanding-radius box, and ignores cells
	**	off the edge of the map.
	*/

	ActionType action = What_Action(where, consider_fog, false);
	bool inradar = Map.In_Local_Radar(where);

	Coord coord = where;
	coord.Z = Map.Get_Height_GL(coord);
	if (Map[coord].IsUnderBridge) {
		coord.Z += BRIDGE_LEPTON_HEIGHT;
	}

	if (TClass->IsMoveToShroud || !Map.Is_Shrouded(coord)) {

		Cell cell = where;
		CellClass * cptr = &Map[Destination_Coord()];

		MZoneType mzone = TClass->MZone;
		if (mzone == MZONE_SUBTERANNEAN) {
			mzone = MZONE_NORMAL;
		} else if (mzone == MZONE_FLYER && House->Ion_Sensitives_Should_Be_Offline()) { // RA2: the house decides, no ion storm test
			mzone = MZONE_INFANTRY;
		}

		bool moveanywhere = (In_Air() || TClass->IsSubterranean || (RTTI == RTTI_INFANTRY && ((InfantryClass *)this)->Class->JumpJet && !House->Ion_Sensitives_Should_Be_Offline()) || RTTI == RTTI_AIRCRAFT) ? true : false; // RA2: an airborne unit also moves anywhere
		bool ontobridge = Is_Moving_Onto_Bridge();

		Coord coord2 = where.As_Coord();
		coord2.Z = Map.Get_Height_GL(coord2);
		if (Map[coord2].IsUnderBridge) {
			coord2.Z += BRIDGE_LEPTON_HEIGHT;
		}

		bool shroud = Map.Is_Shrouded(coord2);

		if (moveanywhere && action == ACTION_NOMOVE) {
			cell = Map.Nearby_Location(where, TClass->Speed, -1, mzone, Map[where].IsUnderBridge, Point2D(1, 1), false, true, TClass->IsSubterranean && HeightAGL < 0, true, Cell(0, 0), false); // RA2: the extra arguments are spelled out
		} else if (!inradar || shroud || (!moveanywhere && (!TClass->Teleporter || action == ACTION_NOMOVE) && !Map.Is_Same_Cell_Zone(cptr->CellID, cell, mzone, ontobridge, Map[where].IsUnderBridge, false))) { // RA2: a teleporter only needs the zone test for a refused move
			cell = Map.Nearby_Location(where, TClass->Speed, Map.Get_Cell_Zone(cptr->CellID, mzone, ontobridge), mzone, Map[where].IsUnderBridge, Point2D(1, 1), false, true, false, true, Cell(0, 0), false);
		}

		if (cell != CELL_NONE) {
			return(cell);
		}
	}

	return(CELL_NONE);
}



/// <summary>
/// Sends this object on to the next leg of its waypoint path.
/// This routine is used once the current waypoint has been satisfied. If the object is not
/// following a path at all, then it does nothing.
/// </summary>
void FootClass::Advance_Waypoint_Path(void)
{
	if (CurrentPath != PATH_NONE) {
		WaypointClass * wp = PlayerPtr->Paths[CurrentPath]->Get_Waypoint(NextWaypoint);
		WaypointClass * next_wp = PlayerPtr->Paths[CurrentPath]->Get_Next_Waypoint(wp);
		Execute_Waypoint_Path(next_wp);
	}
}


/// <summary>
/// Removes this object from the game.
/// This routine will detach the object from whatever team it belongs to before letting the
/// base class perform the actual deletion, so that no team is left holding a dead member.
/// </summary>
void FootClass::Delete_Me(void)
{
	if (CaptureManager != 0) { // RA2: RA2's form
		CaptureManager->FreeAll();
	}
	if (Team != 0) {
		Team->Remove(this, -1, 0);
	}
	BASECLASS::Delete_Me();
}



/// <summary>
/// Determines if this object is airborne.
/// A hover driven object is never considered to be in the air, even though it rides above
/// the terrain rather than on it.
/// </summary>
/// <returns>bool; Is the object in the air?</returns>
bool FootClass::In_Air(void) const
{
	return(BASECLASS::In_Air()); // RA2: no IPersistPtr, Locomotion, CLSID and 1 more (not in RA2's layout)
}


/// <summary>
/// RA2
/// Takes a passenger into this transport.
/// The type's enter sound plays at the transport, and a gunner type that has just taken its first
/// passenger receives its gunner.
/// </summary>
/// <param name="passenger">The object that enters this transport.</param>
void FootClass::Load_Passenger(FootClass * passenger) // RA2: 0x4CCE80
{
	Cargo.Attach(passenger); // RA2: the passenger joins the cargo
	if (passenger != NULL && TClass->EnterTransportSound != VOC_NONE) {
		VocClass::PlayIndexAtPos(TClass->EnterTransportSound, Coord(PositionCoord), 0);
	}
	if (TClass->Gunner && Cargo.How_Many() == 1) {
		ReceiveGunner(passenger);
	}
}


/// <summary>
/// RA2
/// Takes a given passenger out of this transport.
/// The type's leave sound plays at the transport, and a gunner type that has just lost its last
/// passenger loses its gunner.
/// </summary>
/// <param name="passenger">The object that leaves this transport.</param>
void FootClass::Remove_Passenger(FootClass * passenger) // RA2: 0x4CCF20
{
	Cargo.Detach(passenger); // RA2: the passenger leaves the cargo
	if (passenger != NULL && TClass->LeaveTransportSound != VOC_NONE) {
		VocClass::PlayIndexAtPos(TClass->LeaveTransportSound, Coord(PositionCoord), 0);
	}
	if (TClass->Gunner && Cargo.How_Many() == 0) {
		RemoveGunner(passenger);
	}
}


/// <summary>
/// RA2
/// Lets the first passenger out of this transport.
/// A gunner type left empty loses its gunner, and the type's leave sound plays when a passenger came out.
/// </summary>
/// <returns>Returns with the passenger that left, or NULL if there was none.</returns>
FootClass * FootClass::Unload_Passenger(void) // RA2: 0x4CCFC0
{
	FootClass * passenger = Cargo.Detach_Object(); // RA2: the first passenger leaves the cargo
	if (TClass->Gunner && Cargo.How_Many() == 0) {
		RemoveGunner(passenger);
	}
	if (passenger != NULL && TClass->LeaveTransportSound != VOC_NONE) {
		VocClass::PlayIndexAtPos(TClass->LeaveTransportSound, Coord(PositionCoord), 0);
	}
	return(passenger);
}


/// <summary>
/// RA2
/// Takes a gunner aboard.
/// A foot object has no gunner seat: the call does nothing here and exists for the types that have one.
/// </summary>
/// <param name="gunner">The object that became the gunner.</param>
void FootClass::ReceiveGunner(FootClass * gunner) // RA2: 0x4CD060, an empty virtual
{
}


/// <summary>
/// RA2
/// Takes a gunner off.
/// A foot object has no gunner seat: the call does nothing here and exists for the types that have one.
/// </summary>
/// <param name="gunner">The object that stopped being the gunner.</param>
void FootClass::RemoveGunner(FootClass * gunner) // RA2: 0x4CD070, an empty virtual
{
}


/// <summary>
/// RA2
/// Is this object frozen by a paralysis?
/// </summary>
/// <returns>bool; Is the paralysis timer still running?</returns>
bool FootClass::IsParalyzed(void) const // RA2: 0x4CD080
{
	return(ParalysisTimer.Value() != 0); // RA2: the paralysis timer (+0x58C) is read through Value(), so a run-out timer counts as zero
}


/// <summary>
/// RA2
/// Lets the owner sense the cells around this object (a sensor array effect carried by a unit).
/// Every cell within the type's SensorsSight is marked as sensed by the owner, cloaked objects standing in
/// them reconsider whether they can stay hidden, and a building of another house that can be seen is redrawn.
/// </summary>
/// <param name="cell">The cell to center on; CELL_NONE means this object's own cell.</param>
void FootClass::AddSensorsAt(Cell cell) // RA2: 0x4CD0C0
{
	int range = TClass->SensorsSight; // RA2: the range is SensorsSight (+0x538) of the type
	int range_squared = range * range;
	HousesType house = House->HeapID;
	if (cell == CELL_NONE) {
		cell = Center_Coord().As_Cell();
	}
	for (int y = -range; y < range; y++) {
		for (int x = -range; x < range; x++) {
			Cell spot = Cell(cell.X + x, cell.Y + y);
			if (x * x + y * y < range_squared) {
				CellClass * cellptr = &Map[spot];
				cellptr->Sensed_By(house);
				for (ObjectClass * occupier = cellptr->Cell_Occupier(); occupier != NULL; occupier = occupier->Next) {
					RTTIType rtti = occupier->RTTI;
					if (rtti == RTTI_UNIT || rtti == RTTI_INFANTRY || rtti == RTTI_AIRCRAFT) {
						((TechnoClass *)occupier)->Try_To_Cloak();
					}
				}
				BuildingClass * building = cellptr->Cell_Building();
				if (building != NULL && building->House != PlayerPtr && building->Visual_Character(false, NULL)) {
					building->IsToDisplay = true;
				}
			}
		}
	}
}


/// <summary>
/// RA2
/// Takes back what AddSensorsAt gave the owner: the cells that were sensed by the owner are released again.
/// </summary>
/// <param name="cell">The cell to center on; CELL_NONE means this object's own cell.</param>
void FootClass::RemoveSensorsAt(Cell cell) // RA2: 0x4CD250
{
	int range = TClass->SensorsSight; // RA2: the range is SensorsSight (+0x538) of the type
	int range_squared = range * range;
	HousesType house = House->HeapID;
	if (cell == CELL_NONE) {
		cell = Center_Coord().As_Cell();
	}
	for (int y = -range; y < range; y++) {
		for (int x = -range; x < range; x++) {
			Cell spot = Cell(cell.X + x, cell.Y + y);
			if (x * x + y * y < range_squared) {
				CellClass * cellptr = &Map[spot];
				if (cellptr->Is_Sensed(house)) {
					cellptr->Unsensed_By(house);
					for (ObjectClass * occupier = cellptr->Cell_Occupier(); occupier != NULL; occupier = occupier->Next) {
						RTTIType rtti = occupier->RTTI;
						if (rtti == RTTI_UNIT || rtti == RTTI_INFANTRY || rtti == RTTI_AIRCRAFT) {
							((TechnoClass *)occupier)->Try_To_Cloak();
						}
					}
					BuildingClass * building = cellptr->Cell_Building();
					if (building != NULL) {
						if (building->House != PlayerPtr && building->Visual_Character(false, NULL)) { // RA2: tested in a nested if here; the combined test does not match
							building->IsToDisplay = true;
						}
					}
				}
			}
		}
	}
}


/// <summary>
/// RA2
/// Makes this object invulnerable for a while.
/// An organic object cannot be protected and is killed with a C4 warhead instead; otherwise a parasite
/// that is feeding on it is shaken off, the paralysis ends and the techno's own effect starts.
/// </summary>
/// <param name="duration">How long the protection lasts.</param>
void FootClass::IronCurtain(int duration) // RA2: 0x4CD3F0
{
	if (TClass->Organic) { // RA2: organic types are not protected but killed with the C4 warhead (0x4CD3F0)
		int damage = TClass->MaxStrength;
		Take_Damage(damage, 0, Rule->C4Warhead, NULL, false, false);
		return;
	}
	if (ParasiteEatingMe != NULL) {
		ParasiteEatingMe->ParasiteImUsing->SuppressionTimer = CDTimerClass<FrameTimerClass>(50);
		ParasiteEatingMe->ParasiteImUsing->ExitUnit();
	}
	ParalysisTimer = CDTimerClass<FrameTimerClass>(0);
	TechnoClass::IronCurtain(duration);
}


/// <summary>
/// RA2
/// Sends this object plummeting out of the sky.
/// Like AircraftClass::Crash of Tiberian Sun, but for every airborne foot object: the kill is credited,
/// an attached trigger is sprung, the cargo dies with it, and a vehicle is set rocking.
/// </summary>
/// <param name="killer">The object responsible for the kill. This may be NULL.</param>
/// <returns>bool; Was the object sent into a crash?</returns>
bool FootClass::Crash(ObjectClass * killer) // RA2: 0x4CD4B0
{
	if (HeightAGL > 0) { // RA2: only an airborne object crashes
		if (Strength > 0) {
			if (killer != NULL && Tag != NULL) {
				Tag->Spring(TEVENT_FIRST_DAMAGED, this, CELL_NONE, false, NULL);
			}
			if (Tag != NULL && IsActive) {
				Tag->Spring(TEVENT_FIRST_DAMAGED_ANY, this, CELL_NONE, false, NULL);
			}
			if (Tag != NULL && IsActive && killer != NULL) {
				Tag->Spring(TEVENT_FIRST_DAMAGED_ANY, this, CELL_NONE, false, (TechnoClass *)killer);
			}
			if (!IsActive) {
				return(true);
			}
			Record_The_Kill((TechnoClass *)killer);
			if (!IsActive) {
				return(true);
			}
			Strength = 0;
		}
		IsRocking = true;
		Transmit_Message(RADIO_OVER_OUT);
		Stun();
		Kill_Cargo((TechnoClass *)killer);
		if (Fetch_RTTI() != RTTI_INFANTRY && !ScenarioInit) { // RA2: infantry does not rock
			RockingSidewaysPerFrame = Random_Double(0.10, 0.25);
			if (Random_Pick(0, 1) == 0) {
				RockingSidewaysPerFrame = -RockingSidewaysPerFrame;
			}
			RockingForwardsPerFrame = Random_Double(0.0, 0.1);
		}
		Detach_This_From_All(this, false);
		return(true);
	}
	return(false);
}


/// <summary>
/// RA2
/// The image this object is drawn with.
/// A disguised object that the player has not detected shows the image of what it imitates, and an
/// object the player cannot see clearly shows its disguise image too.
/// </summary>
/// <returns>Returns with the image data of the object or of its disguise.</returns>
void const * FootClass::Get_Image_Data(void) const // RA2: 0x4CD650
{
	CellClass * cell = &Map[Center_Coord().As_Cell()]; // RA2: the cell under the object decides whether the disguise is seen through
	if (!House->Is_Ally(PlayerPtr) && IsDisguised() && !cell->Is_Disguise_Sensed(PlayerPtr->HeapID)) {
		return(GetDisguise(false)->ImageData);
	}
	if (!IsClearlyVisibleTo(PlayerPtr)) {
		return(GetDisguise(true)->ImageData);
	}
	return(ObjectClass::Get_Image_Data());
}


/// <summary>
/// RA2
/// Looks for the nearest building of a type that this object can dock at.
/// </summary>
/// <param name="type">The building type to look for.</param>
/// <param name="unk2">Passed on to FindCloserDockBuilding.</param>
/// <param name="unk3">Passed on to FindCloserDockBuilding.</param>
/// <returns>Returns with the nearest suitable building, or NULL.</returns>
BuildingClass * FootClass::FindNearestDockBuilding(BuildingTypeClass * type, unsigned unk2, unsigned unk3) const // RA2: 0x4CD730
{
	int distance = -1; // RA2: starts with no best distance (-1)
	return(FindCloserDockBuilding(type, unk2, unk3, &distance));
}


/// <summary>
/// RA2
/// Looks for a building of one type that is closer than the one found so far.
/// Only buildings of the owner that are up, of that type, and either free to take this object or asked
/// for a free spot count; on the ground the building also has to be in this object's zone.
/// </summary>
/// <param name="type">The building type to look for.</param>
/// <param name="unk2">Unused.</param>
/// <param name="unk3">1 takes a building whatever its links; anything else needs a free link.</param>
/// <param name="distance">The distance of the best building found so far, -1 for none. Updated.</param>
/// <returns>Returns with the best building, or NULL.</returns>
BuildingClass * FootClass::FindCloserDockBuilding(BuildingTypeClass * type, unsigned unk2, unsigned unk3, int * distance) const // RA2: 0x4CD760
{
	BuildingClass * best = NULL; // RA2: the owner must have any building of the type (BQuantity)
	if (House->BQuantity.Value(type->HeapID) != 0) {
		int count = House->Buildings.Count(); // RA2: the count is read once
		for (int index = 0; index < count; index++) {
			BuildingClass * building = House->Buildings[index];
			if (building != NULL && !building->IsInLimbo && building->Class == type && (unk3 == 1 || building->HasFreeLink(this))) {
				if (Fetch_RTTI() != RTTI_AIRCRAFT) {
					Cell there = building->Center_Coord().As_Cell();
					Cell here = Destination_Coord(NULL).As_Cell();
					if (!Map.Is_Same_Cell_Zone(here, there, TClass->MZone, Is_Moving_Onto_Bridge(NULL), false, false)) {
						continue;
					}
				}
				if (((FootClass *)this)->Transmit_Message(RADIO_CAN_LOAD, building) == RADIO_ROGER) {
					int dist = Relative_Distance(building);
					if (*distance == -1 || dist < *distance || building->IsLeader) {
						best = building;
						*distance = dist;
					}
				}
			}
		}
	}
	return(best);
}


/// <summary>
/// RA2
/// Looks for the nearest building among several types that this object can dock at.
/// </summary>
/// <param name="list">The building types to look through.</param>
/// <param name="unk2">Passed on to FindCloserDockBuilding.</param>
/// <param name="unk3">Passed on to FindCloserDockBuilding.</param>
/// <returns>Returns with the nearest suitable building, or NULL.</returns>
BuildingClass * FootClass::TryNearestDockBuilding(TypeList<BuildingTypeClass *> * list, unsigned unk2, unsigned unk3) const // RA2: 0x4CD920
{
	BuildingClass * best = NULL; // RA2: no building found yet
	int best_distance = -1;
	for (int index = 0; index < list->Count(); index++) {
		int distance = -1;
		BuildingClass * found = FindCloserDockBuilding((*list)[index], unk2, unk3, &distance);
		if (found != NULL) {
			if (best == NULL || distance < best_distance || best_distance == -1 || found->IsLeader) {
				best = found;
				best_distance = distance;
			}
		}
	}
	return(best);
}


/// <summary>
/// RA2
/// Forgets where this object was heading: the follow target and the navigation target are dropped.
/// </summary>
void FootClass::Abort_Motion(void) // RA2: 0x4CD9B0
{
	FollowTarget = NULL; // RA2: drops the Follow object (+0x490)
	NavCom = NULL;
}


/// <summary>
/// RA2
/// Takes a MegaMission event for this object.
/// Only the attack move mission is handled here: the object remembers the destination (or, with no
/// destination, the target) that the event carries, as an order to carry out and to come back to after
/// it has dealt with something on the way. Any other mission clears what was remembered.
/// </summary>
/// <param name="event">The MegaMission event.</param>
/// <returns>Returns with 2 if the order was taken as a destination, 1 as a target, or the event's mission
/// if the object keeps nothing.</returns>
int FootClass::RespondMegaEventMission(EventClass * event) // RA2: 0x4CD9C0
{
	int mission = event->Data.MegaMission.Mission; // RA2: the byte at +12 of the event is its mission
	if (mission == MISSION_ATTACK_MOVE) {
		bool has_destination = event->Data.MegaMission.Destination.Is_Valid();
		bool has_target = event->Data.MegaMission.Target.Is_Valid();
		if (!CanAttackOnTheMove()) {
			return(has_target ? 1 : 2);
		}
		HasAttackMoveTarget = false;
		MegaMission = MISSION_ATTACK_MOVE;
		if (has_destination) {
			MegaDestination = event->Data.MegaMission.Destination.As_Abstract();
			return(2);
		}
		if (has_target) {
			MegaTarget = event->Data.MegaMission.Target.As_Abstract();
			return(1);
		}
		ClearMegaMissionData();
	} else {
		ClearMegaMissionData();
	}
	return(mission);
}


/// <summary>
/// RA2
/// Forgets the MegaMission this object was given.
/// </summary>
void FootClass::ClearMegaMissionData(void) // RA2: 0x4CDA80
{
	MegaMission = MISSION_NONE; // RA2: -1 (MISSION_NONE) means no mission is stored (+0x4B4)
	MegaDestination = NULL;
	MegaTarget = NULL;
	HasAttackMoveTarget = false;
}


/// <summary>
/// RA2
/// Does this object remember a MegaMission?
/// </summary>
/// <returns>bool; Is there a mission stored?</returns>
bool FootClass::HaveMegaMission(void) const // RA2: 0x4CDAA0
{
	return(MegaMission != MISSION_NONE); // RA2: -1 (MISSION_NONE) means no mission is stored (+0x4B4)
}


/// <summary>
/// RA2
/// Is this object currently dealing with something it met on its attack move?
/// </summary>
/// <returns>bool; Has the attack move been interrupted by a target?</returns>
bool FootClass::HaveAttackMoveTarget(void) const // RA2: 0x4CDAB0
{
	return(HasAttackMoveTarget); // RA2: the flag at +0x4C1
}


/// <summary>
/// RA2
/// The MegaMission this object remembers.
/// </summary>
/// <returns>Returns with the mission, or MISSION_NONE.</returns>
int FootClass::GetMegaMission(void) const // RA2: 0x4CDAC0
{
	return(MegaMission); // RA2: the mission at +0x4B4
}


/// <summary>
/// RA2
/// Where the attack move leads: the remembered destination, else the remembered target.
/// </summary>
/// <returns>Returns with the coordinate, or (-1, -1, -1) if there is neither.</returns>
Coord FootClass::GetAttackMoveCoords(void) // RA2: 0x4CDAD0
{
	if (MegaDestination != NULL) { // RA2: the destination wins over the target
		return(MegaDestination->Center_Coord());
	}
	if (MegaTarget != NULL) {
		return(MegaTarget->Center_Coord());
	}
	return(Coord(-1, -1, -1));
}


/// <summary>
/// RA2
/// Turns the remembered target into a destination: the cell of the target becomes the place to go to.
/// Nothing in the executable calls this.
/// </summary>
void FootClass::Convert_Mega_Target_To_Destination(void) // RA2: 0x4CDB60
{
	Cell cell = MegaTarget->Center_Coord().As_Cell(); // RA2: the target must be set; the executable has no null test
	MegaDestination = &Map[cell];
	MegaTarget = NULL;
}


/// <summary>
/// RA2
/// Is the remembered MegaMission an attack move?
/// </summary>
/// <returns>bool; Is it MISSION_ATTACK_MOVE?</returns>
bool FootClass::MegaMissionIsAttackMove(void) const // RA2: 0x4CDBD0
{
	return(MegaMission == MISSION_ATTACK_MOVE); // RA2: MISSION_ATTACK_MOVE is 29 in the executable
}


/// <summary>
/// RA2
/// Carries on with the remembered MegaMission: moves to the destination, or attacks the target.
/// </summary>
/// <returns>bool; Was there something to carry on with?</returns>
bool FootClass::ContinueMegaMission(void) // RA2: 0x4CDBE0
{
	bool result = true; // RA2: the result stays true unless nothing was stored
	HasAttackMoveTarget = false;
	if (MegaDestination != NULL) {
		QueueMission(MISSION_MOVE, true);
		Assign_Destination(MegaDestination, true);
	} else if (MegaTarget != NULL) {
		QueueMission(MISSION_ATTACK, true);
		Assign_Target(MegaTarget);
	} else {
		ClearMegaMissionData();
		result = false;
	}
	return(result);
}


/// <summary>
/// RA2
/// Looks for something to attack while this object is on its attack move.
/// An object heading for a destination starts attacking whatever it finds within range; one that was
/// given a target keeps to it as long as that is in the auxiliary search range.
/// </summary>
void FootClass::UpdateAttackMove(void) // RA2: 0x4CDC60
{
	if (MegaDestination != NULL) { // RA2: with a destination, the object looks around from its own position
		if (HasAttackMoveTarget) {
			if (InAuxiliarySearchRange(TarCom)) {
				return;
			}
			TarCom = NULL;
		}
		Coord coord = PositionCoord;
		if (Target_Something_Nearby(coord, THREAT_RANGE)) {
			QueueMission(MISSION_ATTACK, true);
			HasAttackMoveTarget = true;
		}
	} else {
		if (MegaTarget != NULL && InAuxiliarySearchRange(MegaTarget)) {
			return;
		}
		if (HasAttackMoveTarget && InAuxiliarySearchRange(TarCom)) {
			return;
		}
		TarCom = NULL;
		Coord coord = PositionCoord;
		if (Target_Something_Nearby(coord, THREAT_RANGE)) {
			QueueMission(MISSION_ATTACK, true);
			HasAttackMoveTarget = true;
		} else {
			TarCom = MegaTarget;
		}
	}
}


/// <summary>
/// RA2
/// Takes up the attack move again after it was interrupted.
/// </summary>
/// <returns>bool; Is the attack move going on?</returns>
bool FootClass::RefreshMegaMission(void) // RA2: 0x4CDD70
{
	bool result = false; // RA2: only an attack move is refreshed
	if (MegaMission == MISSION_ATTACK_MOVE) {
		if (HasAttackMoveTarget) {
			HasAttackMoveTarget = false;
			result = ContinueMegaMission();
		} else if (Get_Mission() == MISSION_GUARD || Get_Mission() == MISSION_GUARD) { // RA2: the executable calls Get_Mission twice
			result = ContinueMegaMission();
		} else {
			ClearMegaMissionData();
			result = false;
		}
	}
	return(result);
}


// RA2: this virtual has no work in FootClass.
void FootClass::Unknown_Virtual_4D0(void) // RA2: 0x004CA340
{
}


// RA2: the weighted-cell record vector retains this Add member out of line.
template<>
bool DynamicVectorClass<DiscreteDistributionClass<CellClass>::DIST_OBJ>::Add(DiscreteDistributionClass<CellClass>::DIST_OBJ const & object)
{
	if (ActiveCount >= Length()) {
		if ((IsAllocated || !VectorMax) && GrowthStep > 0) {
			if (!Resize(Length() + GrowthStep)) {

				/*
				**	Failure to increase the size of the vector is an error condition.
				**	Return with the error flag.
				*/
				return(false);
			}
		} else {

			/*
			**	Increasing the size of this vector is not allowed! Bail this
			**	routine with the error code.
			*/
			return(false);
		}
	}

	/*
	**	There is room for the new object now. Add it to the end of the object vector.
	*/
	(*this)[ActiveCount++] = object;
	return(true);
}
