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

/* $Header: /CounterStrike/AIRCRAFT.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : AIRCRAFT.CPP                                                 *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : July 22, 1994                                                *
 *                                                                                             *
 *                  Last Update : November 2, 1996 [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   AircraftClass::AI -- Processes the normal non-graphic AI for the aircraft.                *
 *   AircraftClass::Active_Click_With -- Handles clicking over specified cell.                 *
 *   AircraftClass::Active_Click_With -- Handles clicking over specified object.               *
 *   AircraftClass::AircraftClass -- The constructor for aircraft objects.                     *
 *   AircraftClass::Can_Enter_Cell -- Determines if the aircraft can land at this location.    *
 *   AircraftClass::Can_Fire -- Checks to see if the aircraft can fire.                        *
 *   AircraftClass::Cell_Seems_Ok -- Checks to see if a cell is good to enter.                 *
 *   AircraftClass::Desired_Load_Dir -- Determines where passengers should line up.            *
 *   AircraftClass::Draw_It -- Renders an aircraft object at the location specified.           *
 *   AircraftClass::Draw_Rotors -- Draw rotor blades on the aircraft.                          *
 *   AircraftClass::Edge_Of_World_AI -- Detect if aircraft has exited the map.                 *
 *   AircraftClass::Enter_Idle_Mode -- Gives the aircraft an appropriate mission.              *
 *   AircraftClass::Exit_Object -- Unloads passenger from aircraft.                            *
 *   AircraftClass::Fire_At -- Handles firing a projectile from an aircraft.                   *
 *   AircraftClass::Fire_Direction -- Determines the direction of fire.                        *
 *   AircraftClass::Good_Fire_Location -- Searches for and finds a good spot to fire from.     *
 *   AircraftClass::Good_LZ -- Locates a good spot ot land.                                    *
 *   AircraftClass::In_Which_Layer -- Calculates the display layer of the aircraft.            *
 *   AircraftClass::Init -- Initialize the aircraft system to an empty state.                  *
 *   AircraftClass::Is_LZ_Clear -- Determines if landing zone is free for landing.             *
 *   AircraftClass::Landing_Takeoff_AI -- Handle aircraft take off and landing processing.     *
 *   AircraftClass::Look -- Aircraft will look if they are on the ground always.               *
 *   AircraftClass::Mission_Attack -- Handles the attack mission for aircraft.                 *
 *   AircraftClass::Mission_Enter -- Control aircraft to fly to the helipad or repair center.  *
 *   AircraftClass::Mission_Guard -- Handles aircraft in guard mode.                           *
 *   AircraftClass::Mission_Guard_Area -- Handles the aircraft guard area logic.               *
 *   AircraftClass::Mission_Hunt -- Maintains hunt AI for the aircraft.                        *
 *   AircraftClass::Mission_Move -- Handles movement mission.                                  *
 *   AircraftClass::Mission_Retreat -- Handles the aircraft logic for leaving the battlefield. *
 *   AircraftClass::Mission_Unload -- Handles unloading cargo.                                 *
 *   AircraftClass::Movement_AI -- Handles aircraft physical movement logic.                   *
 *   AircraftClass::New_LZ -- Find a good landing zone.                                        *
 *   AircraftClass::Overlap_List -- Returns with list of cells the aircraft overlaps.          *
 *   AircraftClass::Paradrop_Cargo -- Drop a passenger by parachute.                           *
 *   AircraftClass::Per_Cell_Process -- Handle the aircraft per cell process.                  *
 *   AircraftClass::Pip_Count -- Returns the number of "objects" in aircraft.                  *
 *   AircraftClass::Player_Assign_Mission -- Handles player input to assign a mission.         *
 *   AircraftClass::Pose_Dir -- Fetches the natural landing facing.                            *
 *   AircraftClass::Process_Fly_To -- Handles state machine for flying to destination.         *
 *   AircraftClass::Process_Landing -- Landing process state machine handler.                  *
 *   AircraftClass::Process_Take_Off -- State machine support for taking off.                  *
 *   AircraftClass::Read_INI -- Reads aircraft object data from an INI file.                   *
 *   AircraftClass::Receive_Message -- Handles receipt of radio messages.                      *
 *   AircraftClass::Response_Attack -- Gives audio response to attack order.                   *
 *   AircraftClass::Response_Move -- Gives audio response to move request.                     *
 *   AircraftClass::Response_Select -- Gives audio response when selected.                     *
 *   AircraftClass::Rotation_AI -- Handle aircraft body and flight rotation.                   *
 *   AircraftClass::Scatter -- Causes the aircraft to move away a bit.                         *
 *   AircraftClass::Set_Speed -- Sets the speed for the aircraft.                              *
 *   AircraftClass::Shape_Number -- Fetch the shape number to use for the aircraft.            *
 *   AircraftClass::Sort_Y -- Figures the sorting coordinate.                                  *
 *   AircraftClass::Take_Damage -- Applies damage to the aircraft.                             *
 *   AircraftClass::Unlimbo -- Removes an aircraft from the limbo state.                       *
 *   AircraftClass::What_Action -- Determines what action to perform.                          *
 *   AircraftClass::What_Action -- Determines what action to perform.                          *
 *   AircraftClass::operator delete -- Deletes the aircraft object.                            *
 *   AircraftClass::operator new -- Allocates a new aircraft object from the pool              *
 *   AircraftClass::~AircraftClass -- Destructor for aircraft object.                          *
 *   _Counts_As_Civ_Evac -- Is the specified object a candidate for civilian evac logic?       *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#define INCLUDE_COM
#include "always.h"
#include "geometry.h" // RA2: the header statics, in the order of the unit's initializer table
#include "rect.h"

#include "aircraft.h"

#include "_map.h"
#include "_rtti.h"
#include "_rules.h"
#include "_tactica.h"
#include "airctype.h"
#include "anim.h"
#include "animtype.h"
#include "building.h"
#include "builtype.h"
#include "bullet.h"
#include "bullettype.h"
#include "ccrand.h"
#include "cell.h"
#include "dbgprint.h"
#include "findmake.h"
#include "house.h"
#include "houstype.h"
#include "incdec.h"
#include "infantry.h"
#include "infatype.h"
#include "inline.h"
#include "ion.h"
#include "map.h"
#include "mono.h"
#include "partsys.h"
#include "queue.h"
#include "rules.h"
#include "scenario.h"
#include "session.h"
#include "stimer.h"
#include "sun.h"
#include "swizzle.h"
#include "tactical.h"
#include "tag.h"
#include "tagtype.h"
#include "team.h"
#include "temporal.h" // RA2: AircraftClass::AI calls TemporalClass::AI
#include "tracker.h"
#include "unit.h"
#include "waypoint.h"
#include "weapon.h"
#include "globals.h"
static inline Point3D Coord_Difference(Coord const & to, Coord const & from)
{
	return(Point3D(to.X - from.X, to.Y - from.Y, to.Z - from.Z));
}


static inline Cell Aircraft_Adjacent_Cell(Cell const & cell, FacingType dir) // RA2: an adjacent cell by value, the table entry first
{
	return(AdjacentCell[(int)dir & 0x07] + cell); // RA2: as above
}




static inline Cell Aircraft_Destination_Cell(AbstractClass * target, TechnoClass * docker) // RA2: the cell of the destination coordinate
{
	return(target->Destination_Coord(docker).As_Cell()); // RA2: as above
}






static inline bool Aircraft_Accepts_Radio(int mission) // RA2: an aircraft that retreats or paradrops answers no radio message
{
	if (mission == MISSION_RETREAT) { // RA2: as above
		return(false);
	}
	if (mission > MISSION_PATROL && mission <= MISSION_PARADROP_OVERFLY) { // RA2: as above
		return(false);
	}
	return(true);
}


static inline BuildingClass * Aircraft_Building_Destination(AbstractClass * target) // RA2: the target when it is a building
{
	return(target == NULL ? NULL : ((target->Fetch_RTTI() == RTTI_BUILDING) ? (BuildingClass *)target : NULL)); // RA2: as above
}


char const * const AircraftClass::INI_NAME = "Aircraft";

/***********************************************************************************************
 * _Counts_As_Civ_Evac -- Is the specified object a candidate for civilian evac logic?         *
 *                                                                                             *
 *    Examines the specified object to see if it qualifies to be a civilian evacuation. This   *
 *    can only occur if it is a civilian (or Tanya) and the special evacuation flag has been   *
 *    set in the scenario control structure.                                                   *
 *                                                                                             *
 * INPUT:   candidate   -- Candidate object to examine for civilian evacuation legality.       *
 *                                                                                             *
 * OUTPUT:  bool; Is the specified object considered a civilian that must be auto-evacuated?   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/24/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool Counts_As_Civ_Evac(ObjectClass const * candidate)
{
	/*
	**	If the candidate pointer is missing, then return with failure code.
	*/
	if (candidate == NULL) return(false);

	/*
	**	Only infantry objects can be considered for civilian evacuation action.
	*/
	InfantryClass const * inf = (candidate->RTTI == RTTI_INFANTRY) ? (InfantryClass const *)candidate : NULL;

	if (inf == NULL) return(false);

	/*
	**	If the infantry is not a civilian, then it isn't allowed to be a civilian evacuation.
	*/
	if (!inf->Class->IsCivilian) return(false);

	/*
	**	Technicians look like civilians, but are not considered a legal evacuation candidate.
	*/
	if (inf->IsTechnician) return(false);

	/*
	**	All tests pass, so return the success of the infantry as a civilian evacuation candidate.
	*/
	return(true);
}


/***********************************************************************************************
 * AircraftClass::AircraftClass -- The constructor for aircraft objects.                       *
 *                                                                                             *
 *    This routine is the constructor for aircraft objects. An aircraft object can be          *
 *    created and possibly placed into the game system by this routine.                        *
 *                                                                                             *
 * INPUT:   classid  -- The type of aircraft to create.                                        *
 *                                                                                             *
 *          house    -- The owner of this aircraft.                                            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/26/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
AircraftClass::AircraftClass(AircraftTypeClass const * type, HouseClass * house) :
	BASECLASS(house),
	IsToSpendAmmo(false),
	Class((AircraftTypeClass *)type),
	Passenger(false),
	IsKamikaze(false),
	DockNowHeadingTo(NULL), // RA2: the later members are initialized as well
	Unused_5B8(false), // RA2: as above
	Unused_5B9(false), // RA2: as above
	IsLockedStraight(false),
	NumParadropsLeft(5), // RA2: as above
	IsCarryallNotLanding(true), // RA2: as above
	IsReturningFromAttackRun(true) // RA2: as above
{
	Create_ID();

	if (Class != NULL) {
		Locomotion.CreateInstance(Class->Locomotor);
		Locomotion->Link_To_Object(this);
	}

	Init();

	Aircraft.Add(this);

	TargetTracker.Add_Index(Fetch_ID(), this);
}



/***********************************************************************************************
 * AircraftClass::Init -- Initialize the aircraft system to an empty state.                    *
 *                                                                                             *
 *    This routine is used to clear out the aircraft allocation system. It is called in        *
 *    preparation for a scenario load or save game load.                                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void AircraftClass::Init(void)
{
	TechnoClass::Init(); // RA2: RA2's form
	if (Class != 0) { // RA2: as above
		if (House != 0) { // RA2: as above
			DynamicVectorClass<TechnoTypeClass *> &types = (DynamicVectorClass<TechnoTypeClass *> &)House->Class->VeteranAircraft; // RA2: as above
			for (int index = 0; index < types.Count(); index++) { // RA2: as above
				if (types[index] == Class) { // RA2: as above
					Veterancy.Set_Veteran(true); // RA2: as above
					break; // RA2: as above
				}
			}
		}
		PrimaryFacing.Set_ROT(Class->ROT);
		SecondaryFacing.Set_ROT(Class->ROT);
		SecondaryFacing.Set(PrimaryFacing.Current());
		Set_Height_AGL(Class->Flight_Level()); // RA2: as above
		Ammo = Class->InitialAmmo != -1 ? Class->InitialAmmo : Class->MaxAmmo; // RA2: as above
		EstimatedHealth = Strength = Class->MaxStrength; // RA2: as above
	}
	if (House != 0) { // RA2: as above
		House->Tracking_Add(this);
	}
}



/***********************************************************************************************
 * AircraftClass::~AircraftClass -- Destructor for aircraft object.                            *
 *                                                                                             *
 *    This is the destructor for aircraft. It will limbo the aircraft if it isn't already      *
 *    and also removes the aircraft from any team it may be attached to.                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/24/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
AircraftClass::~AircraftClass(void)
{
	if (GameActive && Class) {
		if (House->Can_Build(Class, false, false) == -1) {
			House->IsRecalcNeeded = true;
		}

		Kill_Cargo(NULL);

		if (Team) {
			Team->Remove(this);
			Team = NULL;
		}

		House->Tracking_Remove(this);

		AircraftClass::Limbo();
		Class = NULL;
	}

	Detach_This_From_All(this);
	Aircraft.Delete(this);
	TargetTracker.Remove_Index(Fetch_ID());
	IsActive = false;
}


/// <summary>
/// Fetches the requested interface from this aircraft.
/// Aircraft add the fly control interface to the set that every game object supports, so
/// that the flying locomotor can interrogate them about how they wish to be flown.
/// </summary>
/// <param name="guid">The identifier of the interface being asked for.</param>
/// <param name="ppv">Pointer to the pointer to fill in with the interface.</param>
/// <returns>Returns with S_OK if the interface was supplied.</returns>
HRESULT STDMETHODCALLTYPE AircraftClass::QueryInterface(struct _GUID const &guid, void **ppv)
{
	HRESULT res = BASECLASS::QueryInterface(guid, ppv);
	if (FAILED(res)) {
		if (guid == IID_IFlyControl) {
			*ppv = (IFlyControl *)(this);
		}
		res = S_OK;
		AddRef();
	}
	return(res);
}


/// <summary>
/// Adds a reference to this aircraft.
/// </summary>
/// <returns>Returns with the new number of references outstanding.</returns>
ULONG STDMETHODCALLTYPE AircraftClass::AddRef(void)
{
	return(BASECLASS::AddRef());
}


/// <summary>
/// Releases a reference to this aircraft.
/// </summary>
/// <returns>Returns with the number of references still outstanding.</returns>
ULONG STDMETHODCALLTYPE AircraftClass::Release(void)
{
	return(BASECLASS::Release());
}


/***********************************************************************************************
 * AircraftClass::Unlimbo -- Removes an aircraft from the limbo state.                         *
 *                                                                                             *
 *    This routine is used to transition the aircraft from the limbo to the non limbo state.   *
 *    It occurs when the aircraft is placed on the map for whatever reason. When it is         *
 *    unlimboed, only then will normal game processing recognize it.                           *
 *                                                                                             *
 * INPUT:   coord -- The coordinate that the aircraft should appear at.                        *
 *                                                                                             *
 *          dir   -- The direction it should start facing.                                     *
 *                                                                                             *
 *            strength (optional) -- sets initial strength                                     *
 *                                                                                             *
 *            mission (optional) -- sets initial mission                                       *
 *                                                                                             *
 * OUTPUT:  bool; Was the aircraft unlimboed successfully?                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/26/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool AircraftClass::Unlimbo(Coord const & coord, Dir256 dir)
{
	Coord placed = coord; // RA2: RA2's form
	if (!Class->MissileSpawn) { // RA2: as above
		if (!IsALoaner && Map.In_Local_Radar(coord)) { // RA2: as above
			placed.Z = Map.Get_Height_GL(coord); // RA2: as above
		} else {
			AircraftTypeClass *type = Class; // RA2: as above
			placed.Z = Map.Get_Height_GL(coord) + type->Flight_Level(); // RA2: as above
		}
	}
	if (BASECLASS::Unlimbo(placed, dir)) { // RA2: as above
		if (!(Class->IsSelectable && Class->IsLandable) || // RA2: as above
			(Get_Class_Weapon_Data(0)->Weapon && Get_Class_Weapon_Data(0)->Weapon->IsCamera)) // RA2: as above
			IsALoaner = true;
		if (Cargo.Is_Something_Attached()) Passenger = true; // RA2: as above

		/*
		**	Forces the body of the helicopter to face the correct direction.
		*/
		SecondaryFacing.Set(dir);
		StageClass::Set_Rate(1); // RA2: as above
		Set_Stage(0);
		int altitude = Class->Flight_Level(); // RA2: as above
		if (HeightAGL == altitude) Set_Speed(1.0); // RA2: as above
		else Set_Speed(0.0); // RA2: as above
		return(true);
	}
	return(false);
}




/***********************************************************************************************
 * AircraftClass::Draw_It -- Renders an aircraft object at the location specified.             *
 *                                                                                             *
 *    This routine is used to display the aircraft object at the coordinates specified.        *
 *    The tactical map display uses this routine for all aircraft rendering.                   *
 *                                                                                             *
 * INPUT:   x,y      -- The coordinates to render the aircraft at.                             *
 *                                                                                             *
 *          window   -- The window that the coordinates are based upon.                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/26/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void AircraftClass::Draw_It(Point2D const & xpoint, Rect const & cliprect) const
{
	if (!Debug_Map && MainWindow && Scen->Special.IsFogOfWar) {
		Coord headto = (Coord)Locomotion->Head_To_Coord();
		headto.Z = PositionCoord.Z;
		if (Map.Is_Fogged(headto) && Map.Is_Fogged(PositionCoord) && !House->Is_Player_Control()) {
			return;
		}
	}

	Point2D point = xpoint;
	point += Locomotion->Draw_Point();

	if (Cargo.Is_Something_Attached() && Class->IsCarryall) {
		Cargo.Attached_Object()->Draw_It(point, cliprect);
	}

	Get_Cell(); // RA2: called, result unused

	if (Class->IsVoxel && Class->Voxel.VoxLib != NULL) {
		Coord coord = PositionCoord;
		coord.Z = Map.Get_Height_GL(coord);

		Point2D shadow;
		TacticalMap->Coord_To_Pixel(coord, shadow);
		shadow += Locomotion->Shadow_Point();

		int key = 0;
		int height = HeightAGL;

		bool occupies_cell = Occupies_Cells();
		((AircraftClass &)*this).IsOccupyingCell = false;

		CellClass * cellptr = &Map[Get_Coord()];

		bool draw_on_ground = false;
		if ((IsOnBridge || height < BRIDGE_LEPTON_HEIGHT) && (!IsOnBridge || height < 0)) {
			draw_on_ground = true;
		} else {
			if (cellptr->IsUnderBridge &&
				(cellptr->IsBridgeEastWest && cellptr->Adjacent_Cell(FACING_N).IsUnderBridge ||
				!cellptr->IsBridgeEastWest && cellptr->Adjacent_Cell(FACING_W).IsUnderBridge)) {

				((AircraftClass &)*this).HeightAGL = BRIDGE_LEPTON_HEIGHT;
				shadow.Y -= TacticalMap->Z_Lepton_To_Pixel(BRIDGE_LEPTON_HEIGHT);
			} else {
				draw_on_ground = true;
			}
		}

		if (draw_on_ground) {
			((AircraftClass &)*this).HeightAGL = 0;
		}

		/*
		**	Special manual shadow draw code.
		*/
		Matrix3D matrix;
		matrix = Locomotion->Shadow_Matrix(&key);
		if (!IsSinking) { // RA2: no shadow while sinking
			Draw_Voxel_Shadow(Class->Voxel, 0, key, &Class->ShadowVoxelIndex, cliprect, shadow, Get_Isometric_View_Matrix() * matrix, true, NULL, 0, 0); // RA2: 0, 0, 0
		}

		((AircraftClass &)*this).HeightAGL = height;
		((AircraftClass &)*this).IsOccupyingCell = occupies_cell;

		TacticalMap->Add_To_Selectables((AircraftClass *)this, point);

		int brightness;
		if (IonStormClass::Is_Ion_Storm_Active()) {
			brightness = Scen->IonLevelLight;
		} else {
			brightness = Scen->LevelLight;
		}

		brightness = (HeightAGL / (2 * LEVEL_LEPTON_H)) * brightness; // RA2: height term first
		int newbrightness = Map[coord].Brightness + Rule->ExtraAircraftLight; // RA2: cell light + extra, then the height term
		newbrightness += brightness; // RA2: Draw_It at 0x4141C0 matches only so

		/*
		**	Actually draw the root body of the unit.
		*/
		key = 0; // RA2: 0, not -1
		matrix = Locomotion->Draw_Matrix(&key);
		Draw_Voxel(Class->Voxel, 0, key, &Class->VoxelIndex, cliprect, point, Get_Isometric_View_Matrix() * matrix, newbrightness, SHAPE_NORMAL);
	}

	/*
	**	This draws any overlay graphics on the aircraft.
	*/
	BASECLASS::Draw_It(xpoint, cliprect);
}


/***********************************************************************************************
 * AircraftClass::Draw_Rotors -- Draw rotor blades on the aircraft.                            *
 *                                                                                             *
 *    This routine will draw rotor blades on the aircraft. It is presumed that the aircraft    *
 *    has already been drawn at the X and Y pixel coordinates specified.                       *
 *                                                                                             *
 * INPUT:   x,y   -- The X and Y pixel coordinates to draw the rotor blades.                   *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/26/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void AircraftClass::Draw_Rotors(Point2D const & xy, Rect const & cliprect) const
{
} // RA2: no HeightAGL (TibSun's use does not compile against RA2's declaration)





/***********************************************************************************************
 * AircraftClass::Mission_Hunt -- Maintains hunt AI for the aircraft.                          *
 *                                                                                             *
 *    Hunt AI consists of finding a target and attacking it. If there is no target assigned    *
 *    and this unit doesn't automatically hunt for more targets, then it will change           *
 *    mission to a more passive (land and await further orders) type.                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of ticks before calling this routine again.                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/26/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
int AircraftClass::Do_MISSION_HUNT(void)
{
	if (!Ammo) {
		if (Team) Team->Remove(this);
		Enter_Idle_Mode();
	} else {
		if (TarCom == NULL) {
			if (Session.Type != GAME_NORMAL) {
				Assign_Target(Greatest_Threat(THREAT_TIBERIUM, Get_Coord(), false)); // RA2: a copy of the position (0x4146C0)
			}
			if (TarCom == NULL) {
				Assign_Target(Greatest_Threat(THREAT_NORMAL, Get_Coord(), false)); // RA2: as above
			}
			if (TarCom == NULL) {
				Enter_Idle_Mode();
				return(1);
			}
		}

		QueueMission(MISSION_ATTACK, false); // RA2: QueueMission, not Assign_Mission (vtable +0x1D0 with 1, 0)
		return(1);
	}
	return(Current_Mission_Control().Normal_Delay() + Random_Pick(0, 2));
}


/***********************************************************************************************
 * AircraftClass::AI -- Processes the normal non-graphic AI for the aircraft.                  *
 *                                                                                             *
 *    This handles the non-graphic AI processing for the aircraft. This usually entails        *
 *    maintenance and other AI functions.                                                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/26/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void AircraftClass::AI(void)
{
	switch (CurrentMission) { // RA2: a paradrop run keeps the lock as well
		case MISSION_ATTACK:
		case MISSION_PARADROP_OVERFLY: // RA2: as above
			break;

		default:
			IsLockedStraight = false;
			break;
	}

	/*
	**	RA2: a chronoshifting aircraft sparkles every 24th frame, and a chrono-warping one
	**	cannot do anything else.
	*/
	if (TemporalTargetingMe != NULL) { // RA2: the executable's AI order (temporal, chrono warp)
		TemporalTargetingMe->AI(); // RA2: as above
	}
	if ((IsBeingWarpedOut() || IsWarpingIn()) && Frame % 24 == 0 && Rule->ChronoSparkle1 != NULL) { // RA2: as above
		new AnimClass(Rule->ChronoSparkle1, PositionCoord, 0, 1, (ShapeFlags_Type)0x600, 0); // RA2: as above
	}
	if (IsWarpingIn() || (IsBeingWarpedOut() && IsImmobilized)) { // RA2: as above
		Locomotion->Process(); // RA2: as above
		if (!IsActive) { // RA2: as above
			return;
		}
	}
	if ((IsWarpingIn() && TemporalTargetingMe != NULL) || IsBeingWarpedOut()) { // RA2: as above
		if (TarCom != NULL) {
			Assign_Target(NULL);
		}
		if (NavCom != NULL) {
			Assign_Destination(NULL, true);
		}
		return;
	}

	if (TarCom != NULL) {
		if (TarCom->In_Air()) {
			Assign_Target(NULL);
		} else if (TarCom != NULL) {
			unsigned char const & techno = TarCom->AbstractFlags & 1; // RA2: the Is_Techno test is read into a byte temporary
			if (techno && !House->Is_Ally(TarCom) && ((TechnoClass*)TarCom)->Cloak == CLOAKED && !((TechnoClass*)TarCom)->Is_Sensed_By_House(House)) {
				Assign_Target(NULL);
			}
		}
	}

	/*
	**	Perform any base class AI processing. If during this process, the aircraft was
	**	destroyed, then detect this and bail from this AI routine early.
	*/
	BASECLASS::AI();
	if (!IsActive) {
		return;
	}

	if (IsSinking) { // RA2: a sinking aircraft drops until it is gone
		Coord current = PositionCoord; // RA2: as above
		Set_Coord(current - Coord(0, 0, 5)); // RA2: as above
		if (HeightAGL < -400) { // RA2: as above
			Record_The_Kill(NULL); // RA2: as above
			Delete_Me(); // RA2: as above
			return;
		}
		if ((Frame & 3) == 0) { // RA2: as above
			int offset_y = Scen->RandomNumber(-170, 170); // RA2: as above
			int offset_x = Scen->RandomNumber(-170, 170); // RA2: as above
			Coord offset(offset_x, offset_y, -HeightAGL); // RA2: as above
			current = PositionCoord; // RA2: as above
			current = current + offset; // RA2: as above
			new AnimClass(Rule->Wake, current, 0, 1, (ShapeFlags_Type)0x600, 0); // RA2: as above
		}
	}

	Coord position = PositionCoord; // RA2: the trailer animation
	if (Class->Trailer != NULL && Frame % Class->SpawnDelay == 0) { // RA2: as above
		new AnimClass(Class->Trailer, position, 1, 1, (ShapeFlags_Type)0x600, 0); // RA2: as above
	}

	if (!Map.In_Local_Radar(Get_Cell(), true) && Should_Delete_Off_Map()) { // RA2: the virtual Get_Cell
		Delete_Me();
		return;
	}

	if (NavCom == &BlubCell) {
		Assign_Destination(NULL, true); // RA2: explicit second argument
		Assign_Target(NULL);
		Enter_Idle_Mode(false, true); // RA2: as above
	}

	if (TarCom == &BlubCell) {
		Assign_Destination(NULL, true); // RA2: as above
		Assign_Target(NULL);
		Enter_Idle_Mode(false, true); // RA2: as above
	}

	if (Ready_To_Commence()) {
		Commence();
	}

	if (IsToSpendAmmo) {
		if (CurrentMission != MISSION_ATTACK) {
			IsToSpendAmmo = false;
			Ammo--;
		}
	}

	if (HealthRatio < Rule->ConditionRed && HeightAGL > 0) {
		int chance = Strength != 0 ? 10 : 80; // RA2: as above
		if (Scen->RandomNumber(0, 99) < chance) { // RA2: Percent_Chance open coded
			new AnimClass(AnimTypes[AnimTypeClass::From_Name("SGRYSMK1")], PositionCoord, 0, 1, (ShapeFlags_Type)0x600, 0);
		}
	}

	if (Cargo.Is_Something_Attached() && Class->IsCarryall) {
		Cargo.Attached_Object()->PrimaryFacing = SecondaryFacing;
		Cargo.Attached_Object()->SecondaryFacing = SecondaryFacing;
		Coord passenger_position = PositionCoord; // RA2: the position is copied
		Cargo.Attached_Object()->Set_Coord(passenger_position); // RA2: as above
	}
}



/***********************************************************************************************
 * AircraftClass::Mission_Unload -- Handles unloading cargo.                                   *
 *                                                                                             *
 *    This function is used to handle finding, heading toward, landing, and unloading the      *
 *    cargo from the aircraft. Once unloading of cargo has occurred, then the aircraft follows *
 *    a different mission.                                                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the number of game ticks to delay before calling this function again.      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/31/94   JLB : Created.                                                                 *
 *=============================================================================================*/
int AircraftClass::Do_MISSION_UNLOAD(void)
{
	enum {
		SEARCH_FOR_LZ,
		FLY_TO_LZ,
		LAND_ON_LZ,
		UNLOAD_PASSENGERS,
		TAKE_OFF
	};

	switch (Status) {

		/*
		**	Search for an appropriate destination spot if one isn't already assigned.
		*/
		case SEARCH_FOR_LZ:
			if (HeightAGL == 0 && (double)PitchAngle == 0 && (NavCom == NULL || (PositionCoord == NavCom->Center_Coord()))) {
				Status = UNLOAD_PASSENGERS;
			} else {
				if (NavCom == NULL && Class->IsDropship && HeightAGL > 0) {
					BuildingClass * building = TryNearestDockBuilding((TypeList<BuildingTypeClass *> *)&Class->Dock, 0, 0); // RA2: the whole Dock list at once
					if (building != NULL) {
						Assign_Destination(building, true);
					} else {
						Assign_Destination(Good_LZ(), true);
					}
				} else if (NavCom == NULL) {
					Status = LAND_ON_LZ;
				} else if (Is_LZ_Clear(NavCom)) {
					FootClass * foot = Cargo.Attached_Object(); // RA2: no dropship scatter of the LZ occupiers
					if (foot != NULL && foot->Team && foot->Team->Class->Get_Origin() != CELL_NONE) {
						Assign_Destination(New_LZ(&Map[foot->Team->Class->Get_Origin()]), true);
					} else {
						Assign_Destination(New_LZ(&Map[Scen->Get_Waypoint_Cell(WAYPT_REINF)]), true);
						if (Team != NULL) {
							Team->Assign_Mission_Target(NavCom);
						}
					}

				} else {

					if (HeightAGL != Class->Flight_Level()) {
						Status = TAKE_OFF;
					} else {
						Status = FLY_TO_LZ;
					}
				}

			}
			break;

		/*
		**	Fly to destination.
		*/
		case FLY_TO_LZ:
			if (!Locomotion->Is_Moving()) {
				Status = LAND_ON_LZ;
			}
			if (!Is_LZ_Clear(NavCom)){
				Status = SEARCH_FOR_LZ;
			}
			break;

		/*
		**	Landing phase. Just delay until landing is complete. At that time,
		**	transition to the unloading phase.
		*/
		case LAND_ON_LZ:
			if (!Locomotion->Is_Moving()) {
				Status = UNLOAD_PASSENGERS;
			}
			return(1);

		/*
		**	Hold while unloading passengers. When passengers are unloaded the order for this
		**	transport gets changed to MISSION_RETREAT.
		*/
		case UNLOAD_PASSENGERS:
			if (!IsTethered) {
				if (Cargo.Is_Something_Attached()) {
					if (Class->IsCarryall) {
						Mark(MARK_UP);
						Drop_Off_Cargo();
						Mark(MARK_DOWN);

					} else {
						FootClass * unit = (FootClass *)Cargo.Detach_Object();

						/*
						**	First thing is to lift the transport off of the map so that the unlimbo
						**	process for the passengers is more likely to succeed.
						*/
						Map.Pick_Up(PositionCell, this);

						if (Exit_Object(unit)) {
							unit->IsInTransport = false;
						}

						/*
						**	Restore the transport back down on the map.
						*/
						Map.Place_Down(PositionCell, this);

						if (!unit->IsInTransport) {
							if (unit->Team != NULL) {
								Team->Add(unit);
							}
						}

						if (!Cargo.Is_Something_Attached()) {
							Enter_Idle_Mode();
						}
					}
				} else {

					Enter_Idle_Mode();
				}
			}
			break;

		/*
		**	Aircraft is now taking off. Once the aircraft reaches flying altitude then it
		**	will either take off or look for another landing spot to try again.
		*/
		case TAKE_OFF:
			Status = SEARCH_FOR_LZ;
			return(1);

		default:
			break;
	}
	return(Current_Mission_Control().Normal_Delay() + Random_Pick(0, 2));
}














int AircraftClass::Do_MISSION_PARADROP_APPROACH(void) // RA2: not in TibSun, at 0x415190
{
	if (TarCom == NULL) {
		Assign_Destination(NULL, true);
		QueueMission(MISSION_RETREAT, false);
		return(3);
	}

	if (NavCom == NULL) {
		Assign_Destination(TarCom, true);
		return(3);
	}

	if (Planar_Distance(TarCom) <= Rule->ParadropRadius) {
		QueueMission(MISSION_PARADROP_OVERFLY, false);
		NumParadropsLeft--;
	}
	return(3);
}


int AircraftClass::Do_MISSION_PARADROP_OVERFLY(void) // RA2: not in TibSun, at 0x415210
{
	IsLockedStraight = true;
	if (TarCom != NULL && Cargo.Is_Something_Attached()) {
		if (Planar_Distance(TarCom) > Rule->ParadropRadius) {
			IsLockedStraight = false;
			if (NumParadropsLeft > 0) {
				QueueMission(MISSION_PARADROP_APPROACH, false);
			} else {
				Assign_Target(NULL);
				Assign_Destination(NULL, true);
				QueueMission(MISSION_RETREAT, false);
			}
		} else if (Map.In_Local_Radar(PositionCoord)) {
			Paradrop_Cargo();
		}
	} else {
		IsLockedStraight = false;
		Assign_Target(NULL);
		Assign_Destination(NULL, true);
		QueueMission(MISSION_RETREAT, false);
	}
	return(5);
}


/***********************************************************************************************
 * AircraftClass::Mission_Retreat -- Handles the aircraft logic for leaving the battlefield.   *
 *                                                                                             *
 *    This mission will be followed when the aircraft decides that it is time to leave the     *
 *    battle. Typically, this occurs when a loaner transport has dropped off its load or when  *
 *    an attack air vehicle has expended its ordinance.                                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of game ticks to delay before calling this routine again.  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/19/1995 JLB : Created.                                                                 *
 *   08/13/1995 JLB : Handles aircraft altitude gain after takeoff logic.                      *
 *=============================================================================================*/
int AircraftClass::Do_MISSION_RETREAT(void)
{
	if (NavCom == NULL) { // RA2: TibSun's is a stub
		int edge = House->Control.Edge;
		if (edge < 0 || edge > 3) edge = 0; // RA2: invalid edge -> 0
		Cell cell = Map.Calculated_Cell((SourceType)edge, CELL_NONE, CELL_NONE, SPEED_WINGED, true, MZONE_NORMAL); // RA2: a cell on that edge
		if (cell != CELL_NONE) {
			Assign_Destination(&Map[cell], true); // RA2: fly there
			return(3);
		}
	} else if (NavCom == Get_Cell_Ptr()) { // RA2: arrived
		Assign_Destination(NULL, true);
	}
	return(3); // RA2: 3

} // RA2: Do_MISSION_RETREAT at 0x415300 has no Status


/***********************************************************************************************
 * AircraftClass::Exit_Object -- Unloads passenger from aircraft.                              *
 *                                                                                             *
 *    This routine is called when the aircraft is to unload a passenger. The passenger must    *
 *    be able to move under its own power. Typical situation is when a transport helicopter    *
 *    is to unload an infantry unit.                                                           *
 *                                                                                             *
 * INPUT:   unit  -- Pointer to the unit that is to be unloaded from this aircraft.            *
 *                                                                                             *
 * OUTPUT:  bool; Was the unit unloaded successfully?                                          *
 *                                                                                             *
 * WARNINGS:   The unload process is merely started by this routine. Radio contact is          *
 *             established with the unloading unit and when the unit is clear of the aircraft  *
 *             the radio contact will be broken and then the aircraft is free to pursue        *
 *             other.                                                                          *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/10/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int AircraftClass::Exit_Object(TechnoClass * unit)
{
	static FacingType _toface[FACING_COUNT] = {FACING_S, FACING_SW, FACING_SE, FACING_NW, FACING_NE, FACING_N, FACING_W, FACING_E};
	Cell	cell(0,0);

	/*
	**	Find a free cell to drop the unit off at.
	*/
	FacingType face;
	for (face = FACING_N; face < FACING_COUNT; face++) {
		cell = Aircraft_Adjacent_Cell(Get_Cell(), _toface[face]); // RA2: the virtual Get_Cell, not PositionCell; the table entry is the left operand of the sum
		if (unit->Can_Enter_Cell(&Map[cell], FACING_NONE, -1, NULL, true) == MOVE_OK) break; // RA2: explicit defaults, as above
	}

	// Should perform a check here to see if no cell could be found.

	/*
	**	If the passenger can be placed on the map, then start it moving toward the
	**	destination cell and establish radio contact with the transport. This is used
	**	to make sure that the transport waits until the passenger is clear before
	**	unloading the next passenger or taking off.
	*/
	if (unit->Unlimbo(PositionCoord, DirType(Facing_Dir(_toface[face])).As_Dir256())) { // RA2: the facing goes through As_Dir256
		unit->QueueMission(MISSION_MOVE, false); // RA2: QueueMission(MISSION_MOVE, false) in place of Assign_Mission
		unit->Assign_Destination(&Map[cell], true); // RA2: explicit second argument
		if (Transmit_Message(RADIO_HELLO, unit) == RADIO_ROGER) {
			Transmit_Message(RADIO_UNLOAD);
		}
		return(true);
	}
	return(false);
}



/***********************************************************************************************
 * AircraftClass::Paradrop_Cargo -- Drop a passenger by parachute.                             *
 *                                                                                             *
 *    Call this routine when a passenger needs to be dropped off by parachute. One passenger   *
 *    is offloaded by a call to this routine.                                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the delay time that it is safe to wait before processing any further  *
 *          paradrop actions.                                                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/26/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int AircraftClass::Paradrop_Cargo(void)
{
	FootClass * passenger = Cargo.Detach_Object();
	if (passenger) {
		Ammo--; // RA2: each drop uses one Ammo, which alternates the side the passenger leaves on
		Coord landing = Center_Coord(); // RA2: the drop point is found by projecting 128 leptons to the side
		DirType facing;
		if (Ammo & 1) {
			facing = DirType((unsigned short)(PrimaryFacing.Current().Raw - 0x3FFF)); // RA2: a quarter turn each way
		} else {
			facing = DirType((unsigned short)(PrimaryFacing.Current().Raw + 0x3FFF)); // RA2: as above
		}
		double angle = facing.As_Radian(); // RA2: as above
		int altitude = landing.Z;
		double center_y = landing.Y;
		int sample_y = (int)(center_y - fastmath::sin(angle) * 128.0); // RA2: as above
		int sample_x = (int)(fastmath::cos(angle) * 128.0 + landing.X); // RA2: as above
		landing = Coord(sample_x, sample_y, altitude); // RA2: as above
		CellClass * cell = &Map[landing];
		if (passenger->Can_Enter_Cell(cell, FACING_NONE, 0, NULL, true) == MOVE_OK) { // RA2: height 0, no shroud check
			Coord spot = cell->Closest_Free_Spot(landing, false, false); // RA2: as above
			if (spot != COORD_NONE) { // RA2: as above
				landing.X = spot.X; // RA2: only X and Y come from the free spot
				landing.Y = spot.Y; // RA2: as above
				if (passenger->Paradrop(landing)) {

					/*
					**	Play a sound effect of the parachute opening.
					*/
					VocClass::PlayAt(Rule->ChuteSound, PositionCoord, NULL); // RA2: positional sound, no audio controller
					passenger->LastAdjacencyCell = Cell(Map[landing].Center_Coord()); // RA2: the landing cell is remembered
					if (Team != NULL) {
						Team->Remove(passenger, -1, false); // RA2: no Assign_Mission afterwards, keeps the passenger's own mission
					}
					Arm = CDTimerClass<FrameTimerClass>(0); // RA2: the arm timer is restarted by hand
					NumParadropsLeft = 5; // RA2: 5 left again
					return(0); // RA2: no delay returned
				}
			}
		}
		Cargo.Attach(passenger); // RA2: the drop failed
		passenger->Hidden(); // RA2: as above
		Ammo++; // RA2: as above
	}
	return(0); // RA2: as above
}



/// <summary>
/// Fires this aircraft's weapon at the target specified.
/// An aircraft with passengers aboard paradrops them instead of firing. The projectile
/// is launched with a velocity that suits the aircraft's own motion, and a human owner
/// is shown the ground around the attack if any of it lies under shroud.
/// </summary>
/// <param name="target">The object or cell to fire upon.</param>
/// <param name="which">Which of the weapons to fire.</param>
/// <returns>Returns with a pointer to the projectile created, or NULL if the aircraft
/// did not fire.</returns>
#ifdef RA2_NON_MATCHING // not exact: 1114 bytes differ (the fifth shroud test and the coordinate difference keep their constructors out of line in the original)
BulletClass * AircraftClass::Fire_At(AbstractClass * target, int which)
{
	/*
	**	Passenger aircraft will actually paradrop their cargo instead of
	**	firing their weapon.
	*/
	if (Cargo.Is_Something_Attached()) {
		Paradrop_Cargo();
		return(0);
	}

#if OBSOLETE
	/*
	**	If the weapon is actually a camera, then perform the "snapshot" of the
	**	ground instead of normal weapon fire.
	*/
	if (PrimaryWeapon != NULL && PrimaryWeapon->IsCamera) {
		if (House->Is_Ally(PlayerPtr)) {
			Map.Sight_From(Center_Coord().As_Cell(), 9, House, false);
		}
		Ammo = 0;
		Arm = Rearm_Delay(IsSecondShot);
		return(0);
	}
#endif

	BulletClass * bullet = BASECLASS::Fire_At(target, which);

	if (bullet) {

		if (bullet->Class->ROT == 0) {
			bullet->Velocity.Set_Speed(Locomotion->Apparent_Speed());
			bullet->Velocity.Set_Pitch(DirType(0x4000)); // RA2: straight up as a DirType
			bullet->Velocity.Set_Yaw(SecondaryFacing.Current());
		}

		if (bullet->Class->ROT == 1) {
			Point3D diff = Coord_Difference(target->Center_Coord(), Center_Coord());
			TVelocity3D<double> dir(diff.X, diff.Y, diff.Z);
			bullet->Velocity.Set_Yaw(dir.Get_Yaw());
			bullet->Velocity.Set_Pitch(dir.Get_Pitch());
			bullet->Velocity.Set_Speed(PrimaryWeapon->MaxSpeed);
		}

		if (House->Is_Player_Control()) {
			if (Map.Is_Shrouded(PositionCoord) ||
				Map.Is_Shrouded(PositionCoord + Coord(2 * CELL_LEPTON_W, 2 * CELL_LEPTON_H)) ||
				Map.Is_Shrouded(PositionCoord + Coord(-2 * CELL_LEPTON_W, -2 * CELL_LEPTON_H)) ||
				Map.Is_Shrouded(PositionCoord + Coord(2 * CELL_LEPTON_W, -2 * CELL_LEPTON_H)) ||
				Map.Is_Shrouded(PositionCoord + Coord(2 * CELL_LEPTON_W, -2 * CELL_LEPTON_H)) ||
				Map.Is_Shrouded(target->Center_Coord())) {
				Map.Sight_From(PositionCoord, Rule->AttackingAircraftSightRange, House, false, false, false, true, false); // RA2: the sight is revealed twice, the second time with the last flag set
				Map.Sight_From(PositionCoord, Rule->AttackingAircraftSightRange, House, false, false, false, true, true); // RA2: as above
			}
		}
	}
	if (IsKamikaze) { // RA2: a kamikaze aircraft is gone after it fired
		Delete_Me(); // RA2: as above
	}
	return(bullet);
}
#endif



/***********************************************************************************************
 * AircraftClass::Take_Damage -- Applies damage to the aircraft.                               *
 *                                                                                             *
 *    This routine is used to apply damage to the specified aircraft. This is where any        *
 *    special crash animation will be initiated.                                               *
 *                                                                                             *
 * INPUT:   damage   -- Reference to the damage that will be applied to the aircraft.          *
 *                      This value will be filled in with the actual damage that was           *
 *                      applied.                                                               *
 *                                                                                             *
 *          distance -- Distance from the source of the explosion to this aircraft.            *
 *                                                                                             *
 *          warhead  -- The warhead type that the damage occurs from.                          *
 *                                                                                             *
 *          source   -- Pointer to the originator of the damage. This can be used so that      *
 *                      proper "thank you" can be delivered.                                   *
 *                                                                                             *
 * OUTPUT:  Returns with the result of the damage as it affects this aircraft.                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/26/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ResultType AircraftClass::Take_Damage(int & damage, int distance, WarheadTypeClass const * warhead, TechnoClass * source, bool forced, bool no_crew)
{
	ResultType res = RESULT_NONE;

	/*
	**	Apply the damage to the aircraft.
	*/
	res = BASECLASS::Take_Damage(damage, distance, warhead, source, forced, no_crew);

	/*
	**	Special action is performed if the aircraft is killed -- the cargo is destroyed
	**	as well.
	*/
	switch (res) {
		case RESULT_ALREADY_DESTROYED:
			res = RESULT_ALREADY_DESTROYED;
			break;

		case RESULT_DESTROYED:
			Death_Announcement();
			if (warhead == Rule->FirestormWarhead) {
				int count = abs(Scen->RandomNumber % 3) + 7;
				for (int i = 0; i < count; i++) {
					ParticleSystemClass * psys = new ParticleSystemClass(Rule->DefaultFirestormExplosionSystem, Center_Coord(), NULL, this);
					psys->Sparks_To_Use_Random_Direction();
				}
			} else if (Class->Explosion.Count() > 0) {
				new AnimClass(Class->Explosion.Pick(Scen->RandomNumber), Target_Coord());
			}

#if OBSOLETE
			/*
			**	Parachute a survivor if possible.
			*/
			if (Class->IsCrew && Percent_Chance(90) && Map[Center_Coord()].Is_Clear_To_Move(SPEED_FOOT, true, false)) {
				InfantryClass * infantry = new InfantryClass(INFANTRY_E1, House->Class->House);
				if (infantry != NULL) {
					if (!infantry->Paradrop(Center_Coord())) {
						delete infantry;
					}
				}
			}
#endif

			if (!Crash(source)) { // RA2: the virtual TechnoClass::Crash
				Delete_Me();
			}

			break;

		default:
		case RESULT_HALF:
			break;
	}

	return(res);
}


/***********************************************************************************************
 * AircraftClass::Mission_Move -- Handles movement mission.                                    *
 *                                                                                             *
 *    This state machine routine is used when an aircraft (usually helicopter) is to move      *
 *    from one location to another. It will handle any necessary take off and landing this     *
 *    may require.                                                                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of game frames that should elapse before this routine      *
 *          is called again.                                                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int AircraftClass::Do_MISSION_MOVE(void)
{
	if (Class->IsCarryall) {
		return(Do_MISSION_MOVE_Carryall());
	}
	return(Do_MISSION_MOVE_Normal());
}


/// <summary>
/// RA2
/// </summary>
static inline Cell Aircraft_Dest_Cell(AircraftClass * aircraft)
{
	return(aircraft->NavCom->Destination_Coord(aircraft).As_Cell());
}


/// <summary>
/// Handles the move mission for an aircraft.
/// This routine picks a suitable landing zone, gets the aircraft airborne, flies it to
/// the destination -- by way of the assigned waypoint path if there is one -- and then
/// either lands it or drops it back into idle mode.
/// </summary>
/// <returns>Returns with the delay in game frames before this mission should be
/// processed again.</returns>
int AircraftClass::Do_MISSION_MOVE_Normal(void)
{
	enum {
		VALIDATE_LZ,
		TAKE_OFF,
		FLY_TO_LZ,
		IDLE,
		LAND
	};
	switch (Status) {

		/*
		**	Double check and change LZ if necessary.
		*/
		case VALIDATE_LZ:
			if (NavCom == NULL) {
				Enter_Idle_Mode();
			} else {
				Assign_Destination(New_LZ(NavCom), true); // RA2: Do_MISSION_MOVE_Normal at 0x416010 matches only so
				Status = TAKE_OFF;
			}
			break;

		/*
		**	Take off if necessary.
		*/
		case TAKE_OFF:
			if (NavCom == NULL) {
				Enter_Idle_Mode();
				return(1);
			} else {
				Locomotion->Move_To(NavCom->Destination_Coord(this)); // RA2: as above
				Status = FLY_TO_LZ;
				return(1);
			}
			break;

		/*
		**	Fly toward target.
		*/
		case FLY_TO_LZ:
			if (CurrentPath != PATH_NONE) {
				bool proceed_to_wp = false;
				if (NavCom != NULL) {
					Coord tar_coord = NavCom->Center_Coord();
					Coord here_coord = PositionCoord;
					proceed_to_wp = Coord(here_coord.X, here_coord.Y, 0).Distance_To(Coord(tar_coord.X, tar_coord.Y, 0)) < CELL_LEPTON;
				} else {
					proceed_to_wp = true;
				}
				if (proceed_to_wp) {
					WaypointClass * wp = PlayerPtr->Paths[CurrentPath]->Get_Waypoint(NextWaypoint);
					WaypointClass * nwp = PlayerPtr->Paths[CurrentPath]->Get_Next_Waypoint(wp);
					Execute_Waypoint_Path(nwp);
				}
			}

			if (Locomotion->Is_Moving()) {
				if (NavCom == NULL || Aircraft_Dest_Cell(this) != PositionCell) { // RA2: as above
					if (NavCom != NULL && !Cell_Seems_Ok(Aircraft_Dest_Cell(this), true)) { // RA2: as above
						Status = VALIDATE_LZ;
					} else {
						Status = LAND;
					}
				} else {
					Status = IDLE;
				}
			} else {
				Status = IDLE;
			}
			return(1);

		case IDLE:
			if (!Locomotion->Is_Moving()) { // RA2: as above
				Enter_Idle_Mode(); // RA2: as above
			}
			return(1);

		/*
		**	Land on target.
		*/
		case LAND:
			if (Locomotion->Is_Moving()) {
				if ((NavCom == NULL || Aircraft_Dest_Cell(this) != PositionCell)) { // RA2: as above
					if (NavCom != NULL && !Cell_Seems_Ok(Aircraft_Dest_Cell(this), true)) { // RA2: as above
						Status = VALIDATE_LZ;
					}
				} else {
					Status = IDLE;
				}
			} else {
				Status = IDLE;
			}
			return(1);

		default:
			break;
	}

	return(Current_Mission_Control().Normal_Delay() + Random_Pick(0, 2));
}


/// <summary>
/// Sets the carried unit down on the ground below.
/// This routine is used by the carryall move mission once the aircraft has arrived over
/// its destination. Should the unit prove impossible to place, it stays aboard and the
/// carryall goes looking for somewhere else to put it.
/// </summary>
void AircraftClass::Drop_Off_Cargo(void)
{
	DebugString("Do_MISSION_MOVE_Carryall - LAND - Dropping off cargo\n");

	FootClass * unit = Cargo.Detach_Object();

	Coord coord = PositionCoord;
	coord.Z = Map.Get_Height_GL(coord);
	if (Map[coord].IsUnderBridge) {
		coord.Z += BRIDGE_LEPTON_HEIGHT;
		unit->IsOnBridge = true;
	} else {
		unit->IsOnBridge = false;
	}

	unit->Locomotion.Release();
	unit->Locomotion = ILocomotionPtr(unit->TClass->Locomotor);
	unit->Locomotion->Link_To_Object(unit);

	if (!unit->Unlimbo(coord)) {
		Cargo.Attach(unit);
	} else {
		unit->IsInTransport = false;
		unit->RemoveSight(false, false, false, NULL); // RA2: Look is taken apart: the sight is removed and added again
		unit->UpdateSight(false, false, false, NULL); // RA2: as above
		Map.RevealArea3(Coord(PositionCoord), unit->LastSightRange - 3, unit->LastSightRange + 3, 0); // RA2: as above
		Cell nearby = Nearby_Location(NULL); // RA2: explicit argument
		if (Radio_Link(0) == unit) { // RA2: the Radio_Link accessor, not Contact_With_Whom
			Transmit_Message(RADIO_UNTETHER);
			Transmit_Message(RADIO_OVER_OUT);
		}
		if (nearby != CELL_NONE) {
			Assign_Destination(&Map[nearby], true); // RA2: explicit second argument
		} else {
			Assign_Destination(NULL, true); // RA2: as above
		}
	}
}



/// <summary>
/// Handles the move mission for a carryall aircraft.
/// This routine arranges the ride by radio, flies the carryall over the unit or landing
/// zone it was sent to, and then either lifts the waiting unit aboard or sets down
/// whatever it is already carrying.
/// </summary>
/// <returns>Returns with the delay in game frames before this mission should be
/// processed again.</returns>
int AircraftClass::Do_MISSION_MOVE_Carryall(void)
{
	enum {
		VALIDATE_LZ,
		TAKE_OFF,
		FLY_TO_LZ,
		LAND
	};

	TechnoClass * target;
	Coord bridge_position; // RA2: declared first
	Coord occupant_position; // RA2: as above

	switch (Status) {

		/*
		**	Double check and change LZ if necessary.
		*/
		case VALIDATE_LZ:
			DebugString("Do_MISSION_MOVE_Carryall - VALIDATE_LZ\n");
			if (NavCom == NULL) {
				DebugString("Do_MISSION_MOVE_Carryall - VALIDATE_LZ - NavCom == NULL\n");
				Enter_Idle_Mode(false, true); // RA2: explicit arguments
				break;
			}

			target = Dynamic_Cast<TechnoClass *>(NavCom);
			if (target != NULL &&
				!Cargo.Is_Something_Attached() &&
				House->Is_Ally(target)) {
				bool const & techno_ref = (target->AbstractFlags & 1); // RA2: the Is_Techno test is read into a byte temporary
				if (!techno_ref || target->Owner_HouseClass()->Is_Ally(this)) { // RA2: as above
				if (target->Fetch_RTTI() == RTTI_UNIT) { // RA2: the virtual Fetch_RTTI
				DebugString("Do_MISSION_MOVE_Carryall - VALIDATE_LZ - target != NULL\n");
				if (Radio_Link(0) != target) { // RA2: Radio_Link(0), not Contact_With_Whom
					Transmit_Message(RADIO_OVER_OUT);
				}
				if (Transmit_Message(RADIO_HELLO, target) == RADIO_ROGER) {
					DebugString("Do_MISSION_MOVE_Carryall - VALIDATE_LZ - RADIO_HELLO got RADIO_ROGER\n");
					if (Transmit_Message(RADIO_WANT_RIDE) != RADIO_ROGER) { // RA2: RA2's message number 36
						DebugString("Do_MISSION_MOVE_Carryall - VALIDATE_LZ - RADIO_WANT_RIDE did not get RADIO_ROGER\n");
						Transmit_Message(RADIO_OVER_OUT);
						Enter_Idle_Mode(false, true); // RA2: explicit arguments
						break;
					} else {
						Transmit_Message(RADIO_HOLD_STILL);
					}
				} else {
					Assign_Destination(NULL, true); // RA2: explicit second argument
					Enter_Idle_Mode(false, true); // RA2: explicit arguments
					break;
				}
				}
				}
			}
			Assign_Destination(New_LZ(NavCom), true); // RA2: as above
			if (Team != NULL) {
				Team->Assign_Mission_Target(NavCom);
			}
			Status = TAKE_OFF;
			DebugString("Do_MISSION_MOVE_Carryall - VALIDATE_LZ - Status = TAKE_OFF\n");
			break;

		/*
		**	Take off if necessary.
		*/
		case TAKE_OFF: {
			if (NavCom == NULL) {
				Enter_Idle_Mode(false, true); // RA2: explicit arguments
				return(1);
			}
			Coord destination = NavCom->Center_Coord(); // RA2: a local
			Locomotion->Move_To(destination);
			Status = FLY_TO_LZ;
			return(1);
		}

		/*
		**	Fly toward target.
		*/
		case FLY_TO_LZ:
			if (NavCom != NULL && NavCom->Fetch_RTTI() != RTTI_CELL && Radio_Link(0) != NavCom) { // RA2: the RTTI is read directly
				DebugString("Do_MISSION_MOVE_Carryall - FLY_TO_LZ - Lost contact\n");
				Status = VALIDATE_LZ;
				return(1);
			}

			if (NavCom != NULL && NavCom->Fetch_RTTI() == RTTI_CELL && !Is_LZ_Clear(NavCom)) { // RA2: as above
				Status = VALIDATE_LZ;
				return(1);
			}

			if (Locomotion->Get_Status() == 1) {
				DebugString("Do_MISSION_MOVE_Carryall - FLY_TO_LZ - Begin landing\n");
				if (NavCom != NULL && NavCom->Fetch_RTTI() == RTTI_UNIT) { // RA2: as above
					Coord current_position = PositionCoord; // RA2: both cells are made from copies
					Cell current_cell = Cell(current_position); // RA2: as above
					Coord target_position = ((ObjectClass *)NavCom)->PositionCoord; // RA2: as above
					Cell target_cell = Cell(target_position); // RA2: as above
					if (target_cell != current_cell) { // RA2: as above
						Status = VALIDATE_LZ;
						DebugString("Do_MISSION_MOVE_Carryall - FLY_TO_LZ - Target moved\n");
						return(1);
					}
				}
				IsCarryallNotLanding = false; // RA2: the landing has begun
			}
			if (!Locomotion->Is_Moving()) {
				DebugString("Do_MISSION_MOVE_Carryall - FLY_TO_LZ - Stopped moving\n");
				Status = LAND;
			}
			return(1);

		/*
		**	Land on target.
		*/
		case LAND: {
			DebugString("Do_MISSION_MOVE_Carryall - LAND\n");
			if (Cargo.Is_Something_Attached() && QueueUpToEnter != NULL) { // RA2: QueueUpToEnter, the object to enter once nearby
				IsCarryallNotLanding = true; // RA2: keeps hovering
				return(1); // RA2: as above
			}
			if (Cargo.Is_Something_Attached()) {
				Mark(MARK_UP);
				Drop_Off_Cargo();
				Mark(MARK_DOWN);
				Status = VALIDATE_LZ;
				return(1); // RA2: as above
			}
			DebugString("Do_MISSION_MOVE_Carryall - LAND - Picking up cargo\n");
			Mark(MARK_UP);
			bridge_position = PositionCoord; // RA2: two copies of the position
			occupant_position = PositionCoord; // RA2: as above
			FootClass * unit = Map[occupant_position].Cell_Unit(Map[bridge_position].IsUnderBridge); // RA2: as above
			if (unit != NULL && unit == Radio_Link(0)) { // RA2: Radio_Link(0), not Contact_With_Whom
				DebugString("Do_MISSION_MOVE_Carryall - LAND - Got Cell_Unit\n");
				if (Transmit_Message(RADIO_NEED_TO_MOVE) == RADIO_ROGER) {
					DebugString("Do_MISSION_MOVE_Carryall - LAND - RADIO_NEED_TO_MOVE got RADIO_ROGER\n");
					unit->Limbo();
					unit->IsOnBridge = false;
					unit->IsInTransport = true;
					Cargo.Attach(unit);
				} else {
					Status = VALIDATE_LZ;
				}
			} else {
				Status = VALIDATE_LZ;
			}
			Transmit_Message(RADIO_OVER_OUT);
			Mark(MARK_DOWN);
			IsCarryallNotLanding = true; // RA2: as above
			Enter_Idle_Mode(false, true); // RA2: explicit arguments
			return(1);
		}
	}

	return(Current_Mission_Control().Normal_Delay() + Scen->RandomNumber(0, 2)); // RA2: the scenario's random number
}



/// <summary>
/// RA2
/// </summary>
static inline Cell LZ_Cell(AbstractClass * navcom, AircraftClass * aircraft)
{
	return Cell(navcom->Destination_Coord(aircraft));
}


/// <summary>
/// Handles the patrol mission for an aircraft.
/// This routine walks the aircraft along its assigned waypoint path, breaking off to
/// engage any threat it spots on the way and heading for a docking bay to rearm when it
/// runs dry.
/// </summary>
/// <returns>Returns with the delay in game frames before this mission should be
/// processed again.</returns>
int AircraftClass::Do_MISSION_PATROL(void)
{
	enum {
		VALIDATE_LZ,
		TAKE_OFF,
		FLY_TO_LZ,
		IDLE,
		LAND,
	};

	IsOnPatrol = true;

	switch (Status) {

		/*
		**	Double check and change LZ if necessary.
		*/
		case VALIDATE_LZ:
			if (NavCom == NULL) {
				Enter_Idle_Mode(false, true); // RA2: explicit arguments
			} else {
				Assign_Destination(New_LZ(NavCom), true); // RA2: as above
				if (Team != NULL) {
					Team->Assign_Mission_Target(NavCom);
				}
				Status = TAKE_OFF;
			}
			break;

		/*
		**	Take off if necessary.
		*/
		case TAKE_OFF:
			if (NavCom == NULL) {
				Enter_Idle_Mode(false, true); // RA2: as above
				return(1);
			} else {
				Locomotion->Move_To(NavCom->Destination_Coord(this)); // RA2: the destination is asked for this docker
				Status = FLY_TO_LZ;
				return(1);
			}
			break;

		/*
		**	Fly toward target.
		*/
		case FLY_TO_LZ:
			if (Ammo == 0) {
				BuildingClass * building = TryNearestDockBuilding((TypeList<BuildingTypeClass *> *)&Class->Dock, 0, 0); // RA2: the whole Dock list at once

				if (building != NULL) {
					Override_Mission(MISSION_ENTER, NULL, building);
					Status = VALIDATE_LZ;
					return(1);
				}
			}

			if (Ammo != 0) {
				AbstractClass * threat = Greatest_Threat(THREAT_AREA, PositionCoord, false); // RA2: the position goes in directly, only objects count
				if (threat != NULL && (threat->AbstractFlags & 2)) { // RA2: only objects
					Override_Mission(MISSION_ATTACK, threat, NULL);
					Status = VALIDATE_LZ;
					return(1);
				}
			}

			if (CurrentPath != PATH_NONE) {
				bool proceed_to_wp = false;
				if (NavCom != NULL) {
					Coord tar_coord = NavCom->Center_Coord();
					Coord here_coord = PositionCoord;
					proceed_to_wp = Coord(here_coord.X, here_coord.Y, 0).Distance_To(Coord(tar_coord.X, tar_coord.Y, 0)) < CELL_LEPTON;
				} else {
					proceed_to_wp = true;
				}
				if (proceed_to_wp) {
					WaypointClass * wp = PlayerPtr->Paths[CurrentPath]->Get_Waypoint(NextWaypoint);
					WaypointClass * nwp = PlayerPtr->Paths[CurrentPath]->Get_Next_Waypoint(wp);
					Execute_Waypoint_Path(nwp);
				}
			}

			if (!Locomotion->Is_Moving()) {
				Status = IDLE;
			} else {
				if (NavCom != NULL && !Cell_Seems_Ok(Aircraft_Destination_Cell(NavCom, this), true)) {
					Status = VALIDATE_LZ;
				} else {
					Status = LAND;
				}
			}
			return(1);

		case IDLE:
			Enter_Idle_Mode(false, true); // RA2: as above
			return(1);

		/*
		**	Land on target.
		*/
		case LAND:
			if (!Locomotion->Is_Moving()) {
				Status = IDLE;
			} else {
				if (NavCom != NULL) {
					if (!Cell_Seems_Ok(Aircraft_Destination_Cell(NavCom, this), true)) {
						Status = VALIDATE_LZ;
					}
				}
			}
			return(1);

		default:
			break;
	}

	return(Current_Mission_Control().Normal_Delay() + Scen->RandomNumber(0, 2)); // RA2: the scenario's random number
}



/***********************************************************************************************
 * AircraftClass::Enter_Idle_Mode -- Gives the aircraft an appropriate mission.                *
 *                                                                                             *
 *    Use this routine when the mission for the aircraft is in doubt. This routine will find   *
 *    an appropriate mission for the aircraft and dispatch it.                                 *
 *                                                                                             *
 * INPUT:   initial  -- Is this called when the unit just leaves a factory or is initially     *
 *                      or is initially placed on the map?                                     *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/05/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool AircraftClass::Enter_Idle_Mode(bool initial, bool resume_waypoint)
{
	if (Has_Suspended_Mission()) {
		Restore_Mission();
		if (CurrentMission == MISSION_PATROL) {
			Status = 0;
			IsLockedStraight = false;
		}
		return(false);
	}
	int current_mission = CurrentMission; // RA2: RA2's form
	bool mission_blocks_selection = current_mission == MISSION_RETREAT || (current_mission > MISSION_PATROL && current_mission <= MISSION_PARADROP_OVERFLY); // RA2: as above
	if (mission_blocks_selection) { // RA2: as above
		return(false);
	}
	if (MissionQueue == MISSION_PARADROP_APPROACH) { // RA2: as above
		Commence();
		return(false);
	}
	bool result = FootClass::Enter_Idle_Mode(initial, resume_waypoint); // RA2: as above
	int mission = !House->Is_Human_Player() && !Team && Is_Weapon_Equipped() ? MISSION_GUARD_AREA : MISSION_GUARD; // RA2: as above
	int landing_altitude = Landing_Altitude(); // RA2: as above
	if (In_Which_Layer() == LAYER_GROUND || HeightAGL <= landing_altitude || Class->MissileSpawn) { // RA2: as above
		/* // RA2: as above
		**	Weapon equipped helicopters that run out of ammo and were // RA2: as above
		**	brought in as reinforcements will leave the map. // RA2: as above
		*/ // RA2: as above
		if (IsALoaner) {
			if (Cargo.Is_Something_Attached()) {
				mission = Team ? MISSION_GUARD : MISSION_UNLOAD; // RA2: as above
			} else if (!Team) { // RA2: as above
				Assign_Target(0); // RA2: as above
				Assign_Destination(0, true); // RA2: as above
				mission = MISSION_RETREAT;
			}
		} else {
			Assign_Destination(0, true); // RA2: as above
			Assign_Target(0); // RA2: as above
			if (!House->Is_Human_Player() && !Team && Is_Weapon_Equipped()) { // RA2: as above
				mission = MISSION_GUARD_AREA;
			} else {
				mission = MISSION_GUARD;
			}
		}
	} else {
		if (Cargo.Is_Something_Attached()) {
			if (!IsALoaner) { // RA2: as above
				goto select_destination; // RA2: as above
			}
			if (!Team) { // RA2: as above
				mission = MISSION_UNLOAD; // RA2: as above
				Assign_Destination(Good_LZ(), true); // RA2: as above
			} else {
				mission = MISSION_GUARD;
			}
			goto choose_mission; // RA2: as above
		} else {
			if ((IsALoaner && House->Is_Human_Player()) || (!House->Is_Human_Player() && Class->MaxAmmo == 0)) { // RA2: as above
				if (Team && Team->Has_Entered_Map()) { // RA2: as above
					Team->Remove(this, -1, 0); // RA2: as above
				}
			}
			if (Get_Class_Weapon_Data(0)->Weapon) { // RA2: as above
				if (IsALoaner) {
					if (!Ammo) { // RA2: as above
						if (Team) { // RA2: as above
							Team->Remove(this, -1, 0); // RA2: as above
						}
						Assign_Target(0); // RA2: as above
						Assign_Destination(0, true); // RA2: as above
						mission = MISSION_RETREAT;
					} else if (!Team) { // RA2: as above
						mission = MISSION_HUNT; // RA2: as above
					}
				} else if ((Ammo && TarCom && Get_Mission() == MISSION_ATTACK) || MissionQueue == MISSION_ATTACK) { // RA2: as above
					mission = MISSION_ATTACK;
				} else if (!In_Air() || (NavCom && CurrentMission == MISSION_ENTER) || Class->Dock.Count() <= 0 || // RA2: as above
					(!IsLocked && Team)) { // RA2: as above
				} else {
					TechnoClass *dock = TryNearestDockBuilding((TypeList<BuildingTypeClass *> *)&Class->Dock, 0, 0); // RA2: as above
					Assign_Destination(0, true); // RA2: as above
					if (dock && Transmit_Message(RADIO_HELLO, dock) == RADIO_ROGER) { // RA2: as above
						Assign_Destination(dock, true); // RA2: as above
						mission = MISSION_ENTER; // RA2: as above
					} else {
						if (Class->IsAirportBound) { // RA2: as above
							Crash(0); // RA2: as above
							return(false); // RA2: as above
						}
						Assign_Destination(Good_LZ(), true); // RA2: as above
						mission = MISSION_MOVE; // RA2: as above
					}
				}
				goto choose_mission; // RA2: as above
			}
			if (Team) { // RA2: as above
				return(false); // RA2: as above
			}
		}
select_destination: // RA2: as above
		Assign_Destination(Good_LZ(), true); // RA2: as above
		mission = MISSION_MOVE; // RA2: as above
	}
choose_mission: // RA2: as above
	if (!Ammo && Is_Weapon_Equipped() && !HasAnyLink()) { // RA2: as above
		TechnoClass *dock = TryNearestDockBuilding((TypeList<BuildingTypeClass *> *)&Class->Dock, 0, 0); // RA2: as above
		if (dock) { // RA2: as above
			Assign_Destination(dock, true); // RA2: as above
			Assign_Target(0); // RA2: as above
			mission = MISSION_ENTER; // RA2: as above
		} else if (Class->IsAirportBound) { // RA2: as above
			Crash(0); // RA2: as above
			return(false); // RA2: as above
		}
	}
	if (HasAnyLink()) { // RA2: as above
		mission = MISSION_ENTER; // RA2: as above
	}
	QueueMission((MissionType)mission, false); // RA2: as above
	if (Ready_To_Commence()) {
		Commence();
	}
	return result; // RA2: as above
}





#ifdef _DEBUG
/***********************************************************************************************
 * AircraftClass::Debug_Dump -- Displays the status of the aircraft to the mono monitor.       *
 *                                                                                             *
 *    This displays the current status of the aircraft class to the mono monitor. By this      *
 *    display bugs may be tracked down or prevented.                                           *
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
void AircraftClass::Debug_Dump(MonoClass * mono) const
{
	mono->Set_Cursor(0, 0);
	mono->Set_Cursor(1, 11);mono->Printf("%3d", AttacksRemaining);

	BASECLASS::Debug_Dump(mono);
}
#endif


/***********************************************************************************************
 * AircraftClass::Active_Click_With -- Handles clicking over specified object.                 *
 *                                                                                             *
 *    This routine is used when the player clicks over the speicifed object. It will assign    *
 *    the appropriate mission to the aircraft.                                                 *
 *                                                                                             *
 * INPUT:   action   -- The action that was nominally determined by the What_Action function.  *
 *                                                                                             *
 *          object   -- The object over which the mouse was clicked.                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine will alter the game sequence and causes an event packet to be      *
 *             propagated to all connected machines.                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool AircraftClass::Active_Click_With(ActionType action, ObjectClass * object, bool is_waypoint)
{
	action = What_Action(object, is_waypoint);

	switch (action) {

		case ACTION_TOTE:
			return(Player_Assign_Mission(MISSION_MOVE, NULL, object)); // RA2: Active_Click_With at 0x4174B0 matches only so

		case ACTION_NOMOVE:
			return(false);

		case ACTION_ENTER:
			return(Player_Assign_Mission(MISSION_ENTER, NULL, object)); // RA2: as above

		case ACTION_SELF:
			return(Player_Assign_Mission(MISSION_UNLOAD, NULL, NULL)); // RA2: as above

		default:
			break;
	}

	return(BASECLASS::Active_Click_With(action, object, is_waypoint));
}


/***********************************************************************************************
 * AircraftClass::Active_Click_With -- Handles clicking over specified cell.                   *
 *                                                                                             *
 *    This routine is used when the player clicks the mouse of the specified cell. It will     *
 *    assign the appropriate mission to the aircraft.                                          *
 *                                                                                             *
 * INPUT:   action   -- The action nominally determined by What_Action().                      *
 *                                                                                             *
 *          cell     -- The cell over which the mouse was clicked.                             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine will affect the game sequence and causes an event object to be     *
 *             propagated to all connected machines.                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool AircraftClass::Active_Click_With(ActionType action, Cell const & cell, Cell const & cell2, bool is_waypoint)
{
	return(BASECLASS::Active_Click_With(action, cell, cell2, is_waypoint));
}


/***********************************************************************************************
 * AircraftClass::Player_Assign_Mission -- Handles player input to assign a mission.           *
 *                                                                                             *
 *    This routine is called as a result of player input with the intent to change the         *
 *    mission of the aircraft.                                                                 *
 *                                                                                             *
 * INPUT:   mission  -- The mission requested of the aircraft.                                 *
 *                                                                                             *
 *          target   -- The value to assign to the aircraft's targeting computer.              *
 *                                                                                             *
 *          dest.    -- The value to assign to the aircraft's navigation computer.             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   The mission specified will be executed at an indeterminate future game frame.   *
 *             This is controlled by net/modem propagation delay.                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool AircraftClass::Player_Assign_Mission(MissionType mission, AbstractClass * target, AbstractClass * destination, AbstractClass * nearby)
{
	return(BASECLASS::Player_Assign_Mission(mission, target, destination, nearby));
}


/***********************************************************************************************
 * AircraftClass::What_Action -- Determines what action to perform.                            *
 *                                                                                             *
 *    This routine is used to determine what action will likely be performed if the mouse      *
 *    were clicked over the object specified. The display system calls this routine to         *
 *    control the mouse shape.                                                                 *
 *                                                                                             *
 * INPUT:   target   -- Pointer to the object that the mouse is currently over.                *
 *                                                                                             *
 * OUTPUT:  Returns with the action that will occur if the mouse were clicked over the         *
 *          object specified.                                                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ActionType AircraftClass::What_Action(ObjectClass const * target, bool disallow_force) const
{
	ActionType action = BASECLASS::What_Action(target, disallow_force);

	if (action == ACTION_TOGGLE_SELECT) { // RA2: What_Action at 0x417840 matches only so
		return(action); // RA2: as above
	}

	if (action == ACTION_GUARD_AREA && Class->IsAirportBound) { // RA2: as above
		action = ACTION_NONE; // RA2: as above
	}

	if (Class->IsCarryall && House->Is_Player_Control()) {
		if (action == ACTION_SELECT || action == ACTION_NONE) {
			if (House->Is_Ally(target)) {
				bool techno; // RA2: the Is_Techno test is read into a local
				if (target != NULL) { // RA2: as above
					techno = target->AbstractFlags & 1; // RA2: as above
				}
				if (target == NULL || !techno || target->Owner_HouseClass()->Is_Ally(this)) { // RA2: as above
					if (!Cargo.Is_Something_Attached() && target->RTTI == RTTI_UNIT) {
						action = ACTION_TOTE;
					}
				}
			}
		}
	}

	if (action == ACTION_NONE) {
		action = What_Action(target->Center_Coord().As_Cell(), false, disallow_force);
	}

	if (action == ACTION_SELF) { // RA2: as above
		if (!Cargo.How_Many()) { // RA2: as above
			action = ACTION_NONE; // RA2: as above
		}
	} else if (action == ACTION_ATTACK && Get_Class_Weapon_Data(0)->Weapon == NULL) { // RA2: as above
		action = ACTION_NONE;
	}

	if (House->Is_Player_Control() && action == ACTION_SELECT && target->RTTI == RTTI_BUILDING) { // RA2: What_Action at 0x417840 has no PrimaryWeapon
		BuildingTypeClass const * type = ((BuildingClass *)target)->Class; // RA2: a repair bay or a helipad is entered
		if (type == Rule->RepairBay || type->IsHelipad) { // RA2: as above
			if (((BuildingClass *)target)->HasFreeLink(this) && !((BuildingClass *)target)->Cargo.Is_Something_Attached() && ((AircraftClass *)this)->Transmit_Message(RADIO_CAN_LOAD, (BuildingClass *)target) == RADIO_ROGER) { // RA2: as above
				action = ACTION_ENTER;
			}
		}
	}

	if (Class->IsCarryall && action == ACTION_TOTE) {
		Cell cell = target->PositionCell;
		if (cell != CELL_NONE) {
			BuildingClass * building = (BuildingClass *)Map[cell].Cell_Building();
			if (building != NULL && building->Class->IsWeaponsFactory) {
				action = ACTION_NONE;
			}
		}
	} else if (action == ACTION_NOMOVE) { // RA2: as above
		if (target->IsDisguised() && target->GetDisguiseHouse(true) == NULL && target->GetDisguise(true)->Fetch_RTTI() == RTTI_OVERLAYTYPE) { // RA2: as above
			action = ACTION_MOVE; // RA2: as above
		}
	} else if (action == ACTION_ATTACK) { // RA2: as above
		if (target->Class_Of()->IsImmune) { // RA2: as above
			action = ACTION_NOMOVE; // RA2: as above
		}
	}

	return(action);
}



/***********************************************************************************************
 * AircraftClass::What_Action -- Determines what action to perform.                            *
 *                                                                                             *
 *    This routine will determine what action would occur if the mouse were clicked over the   *
 *    cell specified. The display system calls this routine to determine what mouse shape      *
 *    to use.                                                                                  *
 *                                                                                             *
 * INPUT:   cell  -- The cell over which the mouse is currently positioned.                    *
 *                                                                                             *
 * OUTPUT:  Returns with the action that will be performed if the mouse were clicked at the    *
 *          specified cell location.                                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ActionType AircraftClass::What_Action(Cell const & cell, bool check_fog, bool disallow_force) const
{
	if (!House->Is_Player_Control()) {
		return(ACTION_NONE);
	}

	ActionType action = BASECLASS::What_Action(cell, check_fog, disallow_force);

	if (action == ACTION_ATTACK && Get_Class_Weapon_Data(0)->Weapon == NULL) { // RA2: What_Action at 0x417840 matches only so
		action = ACTION_NONE;
	}

	return(action);
}


/***********************************************************************************************
 * AircraftClass::Pose_Dir -- Fetches the natural landing facing.                              *
 *                                                                                             *
 *    Use this routine to get the desired facing the aircraft should assume when landing.      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the normal default facing the aircraft should have when landed.       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/19/1995 JLB : Created.                                                                 *
 *   03/04/1996 JLB : Fixed wing aircraft always face down the runway.                         *
 *=============================================================================================*/
Dir256 AircraftClass::Pose_Dir(void) const
{
	return(Rule->PoseDir);
}


/***********************************************************************************************
 * AircraftClass::Mission_Attack -- Handles the attack mission for aircraft.                   *
 *                                                                                             *
 *    This routine is the state machine that handles the attack mission for aircraft. It will  *
 *    handling homing in on and firing on the target in the aircraft's targeting computer.     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of game ticks to pass before this routine must be called   *
 *          again.                                                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/19/1995 JLB : Created.                                                                 *
 *   09/22/1995 JLB : Fixes brain dead helicopter for Nod scen #7.                             *
 *=============================================================================================*/
int AircraftClass::Do_MISSION_ATTACK(void)
{
	enum {
		VALIDATE_AZ,
		PICK_ATTACK_LOCATION,
		TAKE_OFF,
		FLY_TO_POSITION,
		FIRE_AT_TARGET,
		FIRE_AT_TARGET2,

		/*
		 * A strafing run is five shots -- the first is fired by FIRE_AT_TARGET, then one
		 * by each of these states, spaced by the weapon's rate of fire.
		 */
		STRAFE_SHOT2,
		STRAFE_SHOT3,
		STRAFE_SHOT4,
		STRAFE_LAST_SHOT,

		RETURN_TO_BASE
	};
	switch (Status) {

		/*
		**	Double check target and validate the attack zone.
		*/
		case VALIDATE_AZ:
			IsLockedStraight = false;

			if (TarCom == NULL) {
				Status = RETURN_TO_BASE;
			} else {
				Status = PICK_ATTACK_LOCATION;
			}
			return(1);

		/*
		**	Pick a good location to attack from.
		*/
		case PICK_ATTACK_LOCATION:
			IsLockedStraight = false;

			if (IsToSpendAmmo) {
				IsToSpendAmmo = false;
				Ammo--;
			}

			if (TarCom == NULL || Ammo == 0) {
				Status = RETURN_TO_BASE;
			} else {
				Assign_Destination(Good_Fire_Location(TarCom), true); // RA2: explicit second argument
				if (NavCom != NULL) {
					Status = FLY_TO_POSITION;
				} else {
					Status = RETURN_TO_BASE;
				}
			}
			break;

		/*
		**	Fly to attack location.
		*/
		case FLY_TO_POSITION:

			if (IsToSpendAmmo) {
				IsToSpendAmmo = false;
				Ammo--;
			}
			IsLockedStraight = false;

			if (TarCom != NULL && Ammo) {

				if (Is_Strafe()) {
					if (Planar_Distance(TarCom) < PrimaryWeapon->Range) {
						Status = FIRE_AT_TARGET;
						return(1);
					}
					Assign_Destination(TarCom, true); // RA2: as above
				} else {
					if (Is_Fighter() || !Locomotion->Is_Moving_Now()) { // RA2: a fighter fires without waiting to stop
						Status = FIRE_AT_TARGET;
						return(1);
					}
				}

				/*
				**	If the navcom was cleared mysteriously, then try to pick
				**	a new attack location. This is a likely event if the player
				**	clicks on a new target while in flight to an existing target.
				*/
				if (NavCom == NULL) {
					Status = PICK_ATTACK_LOCATION;
					return(1);
				}

				int distance = Planar_Distance(NavCom);// = Process_Fly_To(true, NavCom);

				if (distance < 2 * CELL_LEPTON) {
					SecondaryFacing.Set_Desired(Direction(TarCom));

					if (distance < 0x0010) {
						Status = FIRE_AT_TARGET;
						Assign_Destination(NULL, true); // RA2: as above
					}
				} else {
					SecondaryFacing.Set_Desired(DirType().Direction(Fire_Coord(0), NavCom->Center_Coord()));
					return(1);
				}
			} else {
				Status = RETURN_TO_BASE;
			}
			return(1);

		/*
		**	Fire at the target.
		*/
		case FIRE_AT_TARGET:
			if (TarCom == NULL || Ammo == 0) {
				Status = RETURN_TO_BASE;
				return(1);
			}

			if (!Is_Strafe()) {
				PrimaryFacing.Set_Desired(Direction(TarCom));
				SecondaryFacing.Set_Desired(Direction(TarCom));
			}
			switch (Can_Fire(TarCom, What_Weapon_Should_I_Use(TarCom), true)) { // RA2: the weapon is chosen, the range ignored
				case FIRE_CLOAKED:
					Do_Uncloak(false); // RA2: explicit argument
					break;

				case FIRE_REARM:
					break;

				case FIRE_FACING:
					if (!Ammo) {
						Status = RETURN_TO_BASE;
					} else {
						if (IsCloseEnoughToAttack(TarCom) && !Is_Strafe()) { // RA2: IsCloseEnoughToAttack in place of In_Range
							if (Is_Fighter()) { // RA2: a fighter keeps firing from here
								Status = FIRE_AT_TARGET;
							} else {
								Status = Rule->IsCurleyShuffle ? PICK_ATTACK_LOCATION : FIRE_AT_TARGET;
							}
						} else {
							Status = PICK_ATTACK_LOCATION;
						}
						if (Is_Strafe()) {
							return(45);
						}
					}
					break;

				case FIRE_OK:
					IsToSpendAmmo = true;
					Fire_At(TarCom, What_Weapon_Should_I_Use(TarCom)); // RA2: no In_Range test, the weapon is chosen
					Map[TarCom->Center_Coord()].Incoming(PositionCoord, true, false, false); // RA2: explicit arguments
					if (Is_Strafe()) {
						Status = STRAFE_SHOT2;
						IsLockedStraight = true;
						return(PrimaryWeapon->ROF);
					}
					if (Is_Fighter()) { // RA2: a fighter makes one pass per shot
						IsLockedStraight = true;
						Status = Ammo <= 0 ? RETURN_TO_BASE : PICK_ATTACK_LOCATION;
						return(PrimaryWeapon->ROF);
					}
					Status = FIRE_AT_TARGET2;
					break;

				default:
					if (!Ammo) {
						Status = RETURN_TO_BASE;
					} else {
						if (!Is_Strafe()) {
							Status = FIRE_AT_TARGET2;
						}
					}
					break;
			}
			return(1);

		/*
		**	Fire at the target.
		*/
		case FIRE_AT_TARGET2:
			if (TarCom == NULL) {
				Status = RETURN_TO_BASE;
				return(1);
			}

			PrimaryFacing.Set_Desired(Direction(TarCom));
			SecondaryFacing.Set_Desired(Direction(TarCom));
			switch (Can_Fire(TarCom, What_Weapon_Should_I_Use(TarCom), true)) { // RA2: as above
				case FIRE_CLOAKED:
					Do_Uncloak(false); // RA2: as above
					break;

				case FIRE_REARM:
					break;

				case FIRE_FACING:
					if (!Ammo) {
						Status = RETURN_TO_BASE;
					} else {
						if (!IsCloseEnoughToAttack(TarCom) || Is_Strafe()) { // RA2: as above
							Status = PICK_ATTACK_LOCATION;
						} else {
							Status = Rule->IsCurleyShuffle ? PICK_ATTACK_LOCATION : FIRE_AT_TARGET;
						}
						if (Is_Strafe()) {
							return(45);
						}
					}
					break;

				case FIRE_OK:
					Fire_At(TarCom, What_Weapon_Should_I_Use(TarCom)); // RA2: as above
					Map[TarCom->Center_Coord()].Incoming(PositionCoord, true, false, false); // RA2: as above

					if (Ammo) {
						Status = Rule->IsCurleyShuffle ? PICK_ATTACK_LOCATION : FIRE_AT_TARGET;
					} else {
						Status = RETURN_TO_BASE;
					}
					break;

				default:
					if (!Ammo) {
						Status = RETURN_TO_BASE;
					} else {
						if (!IsCloseEnoughToAttack(TarCom)) { // RA2: as above
							Status = PICK_ATTACK_LOCATION;
						} else {
							Status = Rule->IsCurleyShuffle ? PICK_ATTACK_LOCATION : FIRE_AT_TARGET;
						}
					}
					break;
			}
			break;

		case STRAFE_SHOT2:
			if (TarCom == NULL) {
				Status = RETURN_TO_BASE;
				return(1);
			}
			switch (Can_Fire(TarCom, What_Weapon_Should_I_Use(TarCom), true)) { // RA2: as above
				case FIRE_RANGE:
					Assign_Destination(TarCom, true); // RA2: as above

				case FIRE_OK:
				case FIRE_FACING:
				case FIRE_CLOAKED:
				{
					Fire_At(TarCom, What_Weapon_Should_I_Use(TarCom)); // RA2: as above
					Map[TarCom->Center_Coord()].Incoming(PositionCoord, true, false, false); // RA2: as above
					Assign_Destination(TarCom, true); // RA2: as above
					Status = STRAFE_SHOT3;
					return(PrimaryWeapon->ROF);
				}

				default:
					if (!Ammo) {
						Status = RETURN_TO_BASE;
						IsLockedStraight = false;
					}
					return(1);
			}

		case STRAFE_SHOT3:
			if (TarCom == NULL) {
				Status = RETURN_TO_BASE;
				return(1);
			}
			switch (Can_Fire(TarCom, What_Weapon_Should_I_Use(TarCom), true)) { // RA2: as above
				case FIRE_RANGE:
					Assign_Destination(TarCom, true); // RA2: as above

				case FIRE_OK:
				case FIRE_FACING:
				case FIRE_CLOAKED:
				{
					Fire_At(TarCom, What_Weapon_Should_I_Use(TarCom)); // RA2: as above
					Map[TarCom->Center_Coord()].Incoming(PositionCoord, true, false, false); // RA2: as above
					Assign_Destination(TarCom, true); // RA2: as above
					Status = STRAFE_SHOT4;
					return(PrimaryWeapon->ROF);
				}

				default:
					if (!Ammo) {
						Status = RETURN_TO_BASE;
						IsLockedStraight = false;
					}
					return(1);
			}

		case STRAFE_SHOT4:
			if (TarCom == NULL) {
				Status = RETURN_TO_BASE;
				return(1);
			}
			switch (Can_Fire(TarCom, What_Weapon_Should_I_Use(TarCom), true)) { // RA2: as above
				case FIRE_RANGE:
					Assign_Destination(TarCom, true); // RA2: as above

				case FIRE_OK:
				case FIRE_FACING:
				case FIRE_CLOAKED:
				{
					Fire_At(TarCom, What_Weapon_Should_I_Use(TarCom)); // RA2: as above
					Map[TarCom->Center_Coord()].Incoming(PositionCoord, true, false, false); // RA2: as above
					Assign_Destination(TarCom, true); // RA2: as above
					Status = STRAFE_LAST_SHOT;
					return(PrimaryWeapon->ROF);
				}

				default:
					if (!Ammo) {
						Status = RETURN_TO_BASE;
						IsLockedStraight = false;
					}
					return(1);
			}

		case STRAFE_LAST_SHOT:
			if (TarCom == NULL) {
				Status = RETURN_TO_BASE;
				return(1);
			}
			switch (Can_Fire(TarCom, What_Weapon_Should_I_Use(TarCom), true)) { // RA2: as above
				case FIRE_OK:
				case FIRE_FACING:
				case FIRE_RANGE:
				case FIRE_CLOAKED:
					Fire_At(TarCom, What_Weapon_Should_I_Use(TarCom)); // RA2: as above
					Map[TarCom->Center_Coord()].Incoming(PositionCoord, true, false, false); // RA2: as above
					Status = FLY_TO_POSITION;
					return((PrimaryWeapon->Range + 4 * CELL_LEPTON) / Class->MaxSpeed);

				default:
					if (!Ammo) {
						Status = RETURN_TO_BASE;
						IsLockedStraight = false;
					}
					return(1);
			}

		/*
		**	Fly back to landing spot.
		*/
		case RETURN_TO_BASE:

			IsLockedStraight = false;

			if (IsToSpendAmmo) {
				IsToSpendAmmo = false;
				if (Ammo > 0) { // RA2: the ammunition does not go below zero
					Ammo--;
				}
			}
			/*
			**	Break off of firing at the target if there is no more
			**	point in attacking it this mission. The player will
			**	reassign a target for the next mission.
			*/
			if (!Ammo) {
				if (IsALoaner || House->Is_Human_Player()) {
					Assign_Target(NULL);
				}
			} else if (TarCom != NULL) {
				Status = PICK_ATTACK_LOCATION;
				return(1);
			}

			IsLockedStraight = false;
			Assign_Destination(NULL, true); // RA2: as above
			IsReturningFromAttackRun = false; // RA2: idle mode is entered with the flag cleared
			Enter_Idle_Mode(false, true); // RA2: as above
			IsReturningFromAttackRun = true; // RA2: as above
			return(1);

		default:
			break;
	}

	return(Current_Mission_Control().Normal_Delay() + Scen->RandomNumber(0, 2)); // RA2: the scenario's random number
}



/***********************************************************************************************
 * AircraftClass::New_LZ -- Find a good landing zone.                                          *
 *                                                                                             *
 *    Use this routine to locate a good landing zone that is nearby the location specified.    *
 *    By using this routine it is possible to assign the same landing zone to several          *
 *    aircraft and they will land nearby without conflict.                                     *
 *                                                                                             *
 * INPUT:   oldlz -- Target value of desired landing zone (usually a cell target value).       *
 *                                                                                             *
 * OUTPUT:  Returns with the new good landing zone. It might be the same value passed in.      *
 *                                                                                             *
 * WARNINGS:   The landing zone might be a goodly distance away from the ideal if there is     *
 *             extensive blocking terrain in the vicinity.                                     *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
AbstractClass * AircraftClass::New_LZ(AbstractClass * oldlz) const
{
	if (oldlz != NULL) {
		if (Team != NULL && Team->Is_Leaving_Map()) { // RA2: a team that is leaving keeps its landing zone
			return(oldlz); // RA2: as above
		}
		if (Is_LZ_Clear(oldlz)) { // RA2: a clear landing zone whose cell is fine is kept
			Cell lzcell = oldlz->Destination_Coord().As_Cell(); // RA2: as above
			if (Cell_Seems_Ok(lzcell, false)) { // RA2: as above
				return(oldlz); // RA2: as above
			}
		}
		Coord coord = oldlz->Center_Coord();

		/*
		**	Scan outward in a series of concentric rings up to certain distance
		**	in cells.
		*/
		if (!ScenarioInit) { // RA2: no scan while the scenario is being initialised
			for (int radius = 0; radius < Rule->LZScanRadius / CELL_LEPTON_W; radius++) { // RA2: the radius is in cells
				FacingType modifier = Random_Pick(FACING_N, FACING_NW);
				Cell lastcell(0,0);

				/*
				**	Perform a radius scan out from the original center location. Try to
				**	find a cell that is allowed to be a legal LZ.
				*/
				for (FacingType facing = FACING_N; facing < FACING_COUNT; facing++) {
					Cell newcell = Move_Cell(Cell(coord), DirType(Facing_Dir(FacingType(facing+modifier))).As_Int() & -(DIR_STEP_8 << 8), radius/* * CELL_LEPTON_W*/);
					if (Map.In_Local_Radar(newcell, true)) {
						AbstractClass * newtarget = &Map[newcell];

						if (newcell != lastcell && Is_LZ_Clear(newtarget) && Cell_Seems_Ok(newcell, false)) {
							return(newtarget);
						}
						lastcell = newcell;
					}
				}
			}
		}
	}
	return(oldlz);
}



/***********************************************************************************************
 * AircraftClass::Receive_Message -- Handles receipt of radio messages.                        *
 *                                                                                             *
 *    This routine receives all radio messages directed at this aircraft. It is used to handle *
 *    all inter-object coordination. Typically, this would be for transport helicopters and    *
 *    other complex landing operations required of helicopters.                                *
 *                                                                                             *
 * INPUT:   from     -- The source of this radio message.                                      *
 *                                                                                             *
 *          message  -- The message itself.                                                    *
 *                                                                                             *
 *          param    -- An optional parameter that may be used to transfer additional          *
 *                      data.                                                                  *
 *                                                                                             *
 * OUTPUT:  Returns with the radio response from the aircraft.                                 *
 *                                                                                             *
 * WARNINGS:   Some radio messages are handled by the base classes.                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
RadioMessageType AircraftClass::Receive_Message(RadioClass * from, RadioMessageType message, int & param)
{
	AbstractClass * target;

	if (!Aircraft_Accepts_Radio(CurrentMission)) { // RA2: no radio while retreating or paradropping
		return(RADIO_STATIC); // RA2: as above
	}

	switch ((int)message) { // RA2: the message numbers are RA2's (two extra messages sit at 26 and 27)

		case RADIO_RELOAD: // RA2: RA2's number
			if (Ammo >= (Class->MaxAmmo / 2) && TarCom != NULL) {
				return(RADIO_ROGER);
			}
			return(BASECLASS::Receive_Message(from, message, param));

		case RADIO_PREPARED: // RA2: RA2's number
			if (TarCom != NULL) return(RADIO_NEGATIVE);
			if (Ammo == Class->MaxAmmo) return(RADIO_ROGER); // RA2: no height test
			return(RADIO_NEGATIVE);

		case RADIO_ALL_DONE: // RA2: RA2's number
			if (Ammo == Class->MaxAmmo) {
				return(RADIO_ROGER);
			}
			return(RADIO_NEGATIVE);

		/*
		**	Something disastrous has happened to the object in contact with. Fall back
		**	and regroup. This means that any landing process is immediately aborted.
		*/
		case RADIO_RUN_AWAY:
			QueueMission(MISSION_MOVE, false); // RA2: the aircraft heads for a good landing zone
			Assign_Destination(Good_LZ(), true); // RA2: as above
			Transmit_Message(RADIO_OVER_OUT); // RA2: as above
			break;

		/*
		**	The ground control requests that this specified landing spot be used.
		*/
		case RADIO_MOVE_HERE:
			BASECLASS::Receive_Message(from, message, param);
			target = (AbstractClass *)param;
			if (target != NULL && target->Fetch_RTTI() == RTTI_BUILDING) { // RA2: the virtual Fetch_RTTI
				if (Transmit_Message(RADIO_CAN_LOAD, Dynamic_Cast<TechnoClass *>(target)) != RADIO_ROGER) {
					return(RADIO_NEGATIVE);
				}
				QueueMission(MISSION_ENTER, false); // RA2: QueueMission in place of Assign_Mission
				Assign_Destination(target, true); // RA2: explicit second argument
			} else {
				QueueMission(MISSION_MOVE, false); // RA2: as above
				Assign_Destination(target, true); // RA2: as above
			}
			Commence();
			return(RADIO_ROGER);

		/*
		**	Ground control is requesting if the aircraft requires navigation direction.
		*/
		case RADIO_NEED_TO_MOVE:
			BASECLASS::Receive_Message(from, message, param);
			if (!Locomotion->Is_Moving() || NavCom == NULL) {
				return(RADIO_ROGER);
			}
			if (Class->IsAirportBound && NavCom->Fetch_RTTI() == RTTI_CELL && NavCom == Get_Cell_Ptr()) { // RA2: an airport-bound aircraft that is heading for its own cell is fine
				return(RADIO_ROGER); // RA2: as above
			}
			return(RADIO_NEGATIVE);

		/*
		**	This message is sent by the passenger when it determines that it has
		**	entered the transport.
		*/
		case RADIO_IM_IN:
			if (Cargo.How_Many() == Class->Max_Passengers()) {
				Door.Close_Door(Class->DeployTime);
			}

			/*
			**	If a civilian has entered the transport, then the transport will immediately
			**	fly off the map.
			*/
			if (Counts_As_Civ_Evac(from)) {
				Assign_Target(NULL); // RA2: the target and destination are cleared first
				Assign_Destination(NULL, true); // RA2: as above
				QueueMission(MISSION_RETREAT, false); // RA2: QueueMission in place of Assign_Mission
			}
			return(RADIO_ATTACH);

		/*
		**	Docking maintenance message received. Check to see if new orders should be given
		**	to the impatient unit.
		*/
		case RADIO_DOCKING:
			if (Class->Max_Passengers() > 0 && Cargo.How_Many() < Class->Max_Passengers()) {
				BASECLASS::Receive_Message(from, message, param);

				if (!Locomotion->Is_Moving()) {

					Door.Open_Door(Class->DeployTime);

					/*
					**	If the potential passenger needs someplace to go, then figure out a good
					**	spot and tell it to go.
					*/
					if (Transmit_Message(RADIO_NEED_TO_MOVE, from) == RADIO_ROGER) {

						/*
						**	Tell the potential passenger where it should go. If the passenger is
						**	already at the staging location, then tell it to move onto the transport
						**	directly.
						*/
						param = (int)this;
						if (Transmit_Message(RADIO_MOVE_HERE, param, from) != RADIO_ROGER) {
							Transmit_Message(RADIO_OVER_OUT, from);
						} else {
							Radio_Link(0)->Unselect(); // RA2: Radio_Link(0), not Contact_With_Whom
						}
					}
				}
				return(RADIO_ROGER);
			}
			break;

		/*
		**	Asks if the passenger can load on this transport.
		*/
		case RADIO_CAN_LOAD:
			if (Class->Max_Passengers() == 0 || from == NULL || !House->Is_Ally(from)) return(RADIO_STATIC);
			if (Cargo.How_Many() < Class->Max_Passengers()) {
				return(RADIO_ROGER);
			}
			return(RADIO_NEGATIVE);

		case RADIO_UNLOADED:
			if (Class->IsCarryall && Mission == MISSION_MOVE && IsTethered) {
				if ((Cargo.Is_Something_Attached() && Cargo.Attached_Object() == from) || NavCom == from) {
					return(RADIO_NEGATIVE);
				}
			}
			break;

		default:
			break;
	}

	/*
	**	Let the base class take over processing this message.
	*/
	return(BASECLASS::Receive_Message(from, message, param));
}



/***********************************************************************************************
 * AircraftClass::Desired_Load_Dir -- Determines where passengers should line up.              *
 *                                                                                             *
 *    This routine is used by the transport helicopter to determine the location where the     *
 *    infantry passengers should line up before loading.                                       *
 *                                                                                             *
 * INPUT:   object   -- The object that is trying to load up on this transport.                *
 *                                                                                             *
 *                   -- Reference to the cell that the passengers should move to before the    *
 *                      actual load process may begin.                                         *
 *                                                                                             *
 * OUTPUT:  Returns with the direction that the helicopter should face for the load operation. *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/12/1995 JLB : Created.                                                                 *
 *   07/30/1995 JLB : Revamped to scan all adjacent cells.                                     *
 *=============================================================================================*/
FacingType AircraftClass::Desired_Load_Dir(ObjectClass * object, Cell & moveto) const
{
	static FacingType _toface[FACING_COUNT] = {FACING_S, FACING_SW, FACING_SE, FACING_W, FACING_E, FACING_NW, FACING_NE, FACING_N};

	moveto = CELL_NONE;

	Cell center = Center_Coord().As_Cell();
	for (int face = FACING_N; face < FACING_COUNT; face++) { // RA2: Desired_Load_Dir at 0x418CF0 matches only so
		moveto = AdjacentCell[(int)_toface[face] & 7] + center; // RA2: as above
		if (Map.In_Radar(moveto) && (object->Center_Coord().As_Cell() == moveto || Map[moveto].Is_Clear_To_Move(SPEED_FOOT, false, false))) { // RA2: as above
			return(FACING_N); // RA2: as above
		}
	}
	return(FACING_N);
}


/***********************************************************************************************
 * AircraftClass::Can_Enter_Cell -- Determines if the aircraft can land at this location.      *
 *                                                                                             *
 *    This routine is used when the passability of a cell needs to be determined. This is      *
 *    necessary when scanning for a location that the aircraft can land.                       *
 *                                                                                             *
 * INPUT:   cell  -- The cell location to check for landing.                                   *
 *                                                                                             *
 * OUTPUT:  Returns a value indicating if the cell is a legal landing spot or not.             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/12/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
MoveType AircraftClass::Can_Enter_Cell(CellClass const * cell, FacingType, int cell_height, CellClass const *, bool) const
{
	if (Team != 0 && Team->Is_Leaving_Map() && !Map.In_Local_Radar(cell, 1)) { // RA2: RA2's form
		return(MOVE_OK); // RA2: as above
	}
	TechnoClass *occupier = (TechnoClass *)cell->Cell_Occupier(); // RA2: as above
	bool active; // RA2: as above
	if (occupier != 0) { // RA2: as above
		active = (occupier->AbstractFlags & 1); // RA2: as above
	}
	if ((occupier == 0 || !active || occupier->House->Is_Ally(House) // RA2: as above
		 || (occupier->Cloak != 2 && ScenarioInit == 0 // RA2: as above
			 && (occupier->Fetch_RTTI() != RTTI_BUILDING || !((BuildingClass *)occupier)->Class->IsInvisible))) // RA2: as above
		&& !cell->Is_Clear_To_Move(SPEED_WINGED, 0, 0, -1, MZONE_NORMAL, -1, 1)) { // RA2: as above
		return(MOVE_NO);
	}
	if (Session.Type == 0 && IsOwnedByPlayer && !IsALoaner) { // RA2: as above
		if (Map.Is_Shrouded(cell->Center_Coord())) { // RA2: as above
			return(MOVE_NO); // RA2: as above
		}
	}
	return(MOVE_OK);
}



/***********************************************************************************************
 * AircraftClass::Good_Fire_Location -- Searches for and finds a good spot to fire from.       *
 *                                                                                             *
 *    Given the specified target, this routine will locate a good spot for the aircraft to     *
 *    fire at the target.                                                                      *
 *                                                                                             *
 * INPUT:   target   -- The target that is desired to be attacked.                             *
 *                                                                                             *
 * OUTPUT:  Returns with the target location of the place that firing should be made from.     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/12/1995 JLB : Created.                                                                 *
 *   06/14/1995 JLB : Finer resolution on ring scan.                                           *
 *   11/02/1996 JLB : Bias fire position to get closer to moving objects.                      *
 *=============================================================================================*/
AbstractClass * AircraftClass::Good_Fire_Location(AbstractClass * target) const
{
	if (target != NULL) {
		if (((AircraftClass *)this)->Is_Strafe()) {
			return(target);
		}

		int range = Weapon_Range(0);
		Coord tcoord = target->Center_Coord();
		Cell bestcell = CELL_NONE;
		Cell best2cell = CELL_NONE;
		int bestval = -1;
		int best2val = -1;

		/*
		**	Try to get closer to a target that is moving.
		*/
		Coord altcoord = COORD_NONE;
		if (target->AbstractFlags & 4) { // RA2: Good_Fire_Location at 0x418F10 matches only so
			AbstractClass * alttarg = ((FootClass *)target)->NavCom; // RA2: as above
			if (alttarg != NULL) {
				altcoord = alttarg->Center_Coord();
			}
		}

		for (int r = range-CELL_LEPTON; r > CELL_LEPTON; r -= CELL_LEPTON) {
			for (int face = 0; face < DIR_MAX; face += DIR_STEP_16) {
				Coord newcoord = Move_Coord(tcoord, (Dir256)face, r);
				Cell newcell = newcoord.As_Cell();

				if (Map.In_Local_Radar(newcell) && (Session.Type != GAME_NORMAL || IsALoaner || Map[newcell].IsVisible) && Cell_Seems_Ok(newcell, true)) { // RA2: as above
					int dist;
					if (altcoord != COORD_NONE) {
						dist = Point2D(newcoord).Distance_To(Point2D(altcoord));
					} else {
						dist = Point2D(Center_Coord()).Distance_To(Point2D(newcoord));
					}
					if (bestval == -1 || dist < bestval) {
						best2val = bestval;
						best2cell = bestcell;
						bestval = dist;
						bestcell = newcell;
					}
				}
			}
			if (bestval != -1) break;
		}

		if (best2val == -1) {
			best2cell = bestcell;
		}

		/*
		**	If it found a good firing location, then return this location as
		**	a target value.
		*/
		if (bestval != -1) {
			if (Percent_Chance(50)) {
				return(&Map[bestcell]);
			} else {
				return(&Map[best2cell]);
			}
		}
	}
	return(NULL);
}


/***********************************************************************************************
 * AircraftClass::Cell_Seems_Ok -- Checks to see if a cell is good to enter.                   *
 *                                                                                             *
 *    This routine examines the navigation computers of other aircraft in order to see if the  *
 *    specified cell is safe to fly to. The intent of this routine is to avoid unnecessary     *
 *    mid-air collisions.                                                                      *
 *                                                                                             *
 * INPUT:   cell     -- The cell to examine for clear airspace.                                *
 *                                                                                             *
 *          strict   -- Should the scan consider the aircraft, that is making this check, a    *
 *                      blocking aircraft. Typically, the aircraft itself is not considered    *
 *                      a blockage -- an aircraft can always exist where it is currently       *
 *                      located. A strict check is useful for helicopters that need to move    *
 *                      around at the slightest provocation.                                   *
 *                                                                                             *
 * OUTPUT:  Is the specified cell free from airspace conflicts?                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool AircraftClass::Cell_Seems_Ok(Cell const & cell, bool strict) const
{
	if (!Map.In_Local_Radar(cell)) {
		return(true);
	}

	if (Class->Spawned) { // RA2: Cell_Seems_Ok at 0x419250 matches only so
		TechnoClass * techno = Map[cell].Cell_Techno(); // RA2: as above
		if (techno != NULL) { // RA2: as above
			if (techno->SpawnManager != NULL || techno->Techno_Type_Class()->Spawned) { // RA2: as above
				return(true); // RA2: as above
			}
		}
	}

	/*
	**	Make sure that no other aircraft are heading to the selected location. If they
	**	are, then don't consider the location as valid.
	*/
	if (Class->IsAirportBound) { // RA2: as above
		return(true); // RA2: as above
	}

	bool is_toting = (Class->IsCarryall && NavCom != NULL && NavCom->Fetch_RTTI() == RTTI_UNIT); // RA2: as above

	AbstractClass * astarget = &Map[cell];
	for (int index = 0; index < Feet.Count(); index++) {
		AircraftClass * air = (AircraftClass *)Feet[index];
		if (air && (!is_toting || NavCom != air) && (strict || air != this) && !air->IsInLimbo && air->IsDown) {
			if (air->PositionCell == cell || air->NavCom == astarget) {
				return(false);
			}
		}
	}
	return(true);
}


/***********************************************************************************************
 * AircraftClass::Mission_Enter -- Control aircraft to fly to the helipad or repair center.    *
 *                                                                                             *
 *    This routine is used when the aircraft needs to fly for either rearming or repairing.    *
 *    It tries to establish contact with the support building. Once contact is established     *
 *    the ground controller takes care of commanding the aircraft.                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the delay before this routine should be called again.                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/12/1995 JLB : Created.                                                                 *
 *   07/04/1995 JLB : Ground controller gives orders.                                          *
 *=============================================================================================*/
int AircraftClass::Do_MISSION_ENTER(void)
{
	enum {
		INITIAL,
		TAKEOFF,
		ALTITUDE,
		STACK,
		DOWNWIND,
		CROSSWIND,
		TRAVEL,
		LANDING
	};

	/*
	**	RA2: an airport-bound aircraft on the ground that is not standing on its destination
	**	building heads for that building (or none) and starts over.
	*/
	if (HeightAGL == 0 && Class->IsAirportBound && NavCom != NULL) { // RA2: not in TibSun
		if (NavCom != Get_Cell_Ptr()->Cell_Building()) { // RA2: as above
			DockNowHeadingTo = Aircraft_Building_Destination(NavCom); // RA2: as above
			Transmit_Message(RADIO_OVER_OUT); // RA2: as above
			QueueMission(MISSION_MOVE, true); // RA2: as above
			return(1); // RA2: as above
		}
	}
	if (HeightAGL > 0) { // RA2: in the air the dock being headed for follows the destination
		if (DockNowHeadingTo != NavCom && NavCom != NULL && NavCom->Fetch_RTTI() == RTTI_BUILDING) { // RA2: as above
			DockNowHeadingTo = Aircraft_Building_Destination(NavCom); // RA2: as above
		}
	}

	/*
	**	Verify that it has a valid NavCom. If it doesn't then request one from the
	**	building this building is trying to land upon. If that fails, then enter
	**	idle mode.
	*/
	if (NavCom == NULL && In_Which_Layer() != LAYER_GROUND) {
		if (Transmit_Message(RADIO_DOCKING) != RADIO_ROGER && !Move_To_Object_Nearby()) {
			Enter_Idle_Mode(false, true); // RA2: explicit arguments
			return(1);
		}
	}

	switch (Status) {
		case INITIAL:
			Status = TRAVEL; // RA2: and on as the other approach states

		case TAKEOFF:
		case ALTITUDE:
		case STACK:
		case DOWNWIND:
		case CROSSWIND:
			IsCarryallNotLanding = true; // RA2: RA2's flag where TibSun sets IsReadyToCommence
			return(1);

		case TRAVEL:
			if (Class->IsAirportBound && HeightAGL != 0) { // RA2: an airport-bound aircraft in the air asks to dock
				Transmit_Message(RADIO_DOCKING); // RA2: as above
			}
			if (!HasAnyLink()) { // RA2: HasAnyLink, not In_Radio_Contact
				if (!Move_To_Object_Nearby()) {
					if (Locomotion->Get_Status() == 1) {
						Status = LANDING;
						IsCarryallNotLanding = true; // RA2: as above
					}
					if (MissionQueue == MISSION_NONE) {
						Assign_Destination(NULL, true); // RA2: explicit second argument
						Enter_Idle_Mode(false, true); // RA2: as above
						IsCarryallNotLanding = true; // RA2: as above
						return(1);
					} else {
						IsCarryallNotLanding = true; // RA2: as above
					}
				}
				IsCarryallNotLanding = true; // RA2: as above
				return(1);
			} else {
				if (Locomotion->Get_Status() == 1 || (Class->IsAirportBound && HeightAGL == 0)) { // RA2: an airport-bound aircraft on the ground counts as landed
					Status = LANDING;
					IsCarryallNotLanding = false;
				}
				return(3);
			}

		case LANDING:
			if (Locomotion->Get_Status() == 1) {
				AbstractClass * target = NavCom; // RA2: a local copy
				if (target != NULL) {
					Coord nav = (target->Fetch_RTTI() == RTTI_BUILDING) ? ((ObjectClass *)target)->Docking_Coord(this) : NavCom->Center_Coord(); // RA2: a building is approached at its docking coordinate
					Coord pos = PositionCoord;
					int x = nav.X - pos.X;
					int y = nav.Y - pos.Y;
					x = x > 0 ? min(5, x) : max(-5, x);
					y = y > 0 ? min(5, y) : max(-5, y);
					pos.X += x; // RA2: the components are added separately
					pos.Y += y; // RA2: as above
					Set_Coord(pos); // RA2: the virtual Set_Coord
				}
				IsCarryallNotLanding = true; // RA2: as above
			} else {
				IsCarryallNotLanding = true; // RA2: as above
				switch (Transmit_Message(RADIO_IM_IN)) {
					default:
						Enter_Idle_Mode(false, true); // RA2: explicit arguments
						return(1);

					case RADIO_ATTACH:
						Limbo();
						Radio_Link(0)->Cargo.Attach(this); // RA2: Radio_Link(0), not Contact_With_Whom
						return(1);

					case RADIO_ROGER:
						break;
				}
				if (Class->IsCarryall && Cargo.Is_Something_Attached()) {
					TechnoClass * contact = Radio_Link(0); // RA2: Radio_Link(0), not Contact_With_Whom
					TechnoClass * cargo = Cargo.Detach_Object();
					Transmit_Message(RADIO_OVER_OUT);
					if (cargo->Transmit_Message(RADIO_HELLO, contact) == RADIO_ROGER) {
						Cargo.Attach((FootClass *)cargo);
						Mark(MARK_UP);
						Drop_Off_Cargo();
						Mark(MARK_DOWN);
					} else {
						Assign_Destination(Good_LZ(), true); // RA2: explicit second argument
					}
					QueueMission(MISSION_MOVE, false); // RA2: QueueMission in place of Assign_Mission
				} else {
					QueueMission(MISSION_GUARD, false); // RA2: as above
				}
			}
			break;

		default:
			break;
	}
	return(1);
}



/***********************************************************************************************
 * AircraftClass::Good_LZ -- Locates a good spot to land.                                      *
 *                                                                                             *
 *    This routine is used when helicopters need a place to land, but there are no obvious     *
 *    spots (i.e., helipad) available. It will try to land near a friendly helipad or friendly *
 *    building if there are no helipads anywhere. In the event that there are no friendly      *
 *    buildings anywhere on the map, then just land right where it is flying.                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the target location where this aircraft should land. This value may   *
 *          not be a clear cell, but the normal landing logic will resolve that problem.       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/12/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
AbstractClass * AircraftClass::Good_LZ(void) const
{
	/*
	**	Scan through all of the buildings and try to land near
	**	the helipad (if there is one) or the nearest friendly building.
	*/
	Cell bestcell = Cell(0, 0);
	int bestdist = -1;
	int index; // RA2: one index for both scans

	if (Class->Dock.Count() > 0) {
		int count = House->Buildings.Count(); // RA2: the owner's own building list, counted once
		for (index = 0; index < count; index++) { // RA2: as above
			BuildingClass * building = House->Buildings[index]; // RA2: as above

			if (building && !building->IsInLimbo) { // RA2: the owner's list holds only its own buildings
				int dist = Distance(building->Center_Coord()); // RA2: the 3D distance between the two centres
				if (building->Class == Class->Dock[0]) {
					dist /= 4;
				}
				if (bestdist == -1 || dist < bestdist) {
					Cell cell = building->PositionCell;
					cell = Map.Nearby_Location(cell, SPEED_FOOT, Map.Get_Cell_Zone(cell), MZONE_NORMAL, false, Point2D(3, 3));
					if (cell != CELL_NONE) {
						bestdist = dist;
						bestcell = cell;
					}
				}
			}
		}
	}

	if (bestdist != -1) {
		if (bestdist < CELL_LEPTON) {
			Cell cell = Map.Nearby_Location(PositionCell, SPEED_FOOT, Map.Get_Cell_Zone(PositionCell)); // RA2: the cell is a local, bestcell is kept
			return(&Map[cell]); // RA2: as above
		}
		return(&Map[bestcell]);
	}

	if (bestdist == -1) {
		for (index = 0; index < Objects.Count(); index++) { // RA2: as above
			TechnoClass * techno = Dynamic_Cast<TechnoClass *>(Objects[index]);
			if (techno && !techno->IsInLimbo && techno->House == House && techno != (AircraftClass *)this) {
				int dist = (Center_Coord() - techno->Center_Coord()).Length(); // RA2: as above
				if (bestdist == -1 || dist < bestdist) {
					Cell cell = techno->PositionCell;
					cell = Map.Nearby_Location(cell, SPEED_FOOT, Map.Get_Cell_Zone(cell));
					if (cell != CELL_NONE) {
						bestdist = dist;
						bestcell = cell;
					}
				}
			}
		}
	}

	/*
	**	Return with the suitable location if one was found.
	*/
	if (bestdist != -1) {
		return(&Map[bestcell]);
	}

	/*
	**	No good location was found. Just try to land here.
	*/
	return(&Map[(Coord const &)PositionCoord]);
}



/***********************************************************************************************
 * AircraftClass::Fire_Direction -- Determines the direction of fire.                          *
 *                                                                                             *
 *    This routine will determine what direction a projectile would take if it were fired      *
 *    from the aircraft. This is the direction that the aircraft's body is facing.             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the direction of projectile fire.                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
DirType AircraftClass::Fire_Direction(void) const
{
	return(SecondaryFacing.Current());
}


/***********************************************************************************************
 * AircraftClass::Scatter -- Causes the aircraft to move away a bit.                           *
 *                                                                                             *
 *    This routine will cause the aircraft to move away from its current location and then     *
 *    enter some idle mode. Typically this is called when the aircraft is attacked while on    *
 *    the ground.                                                                              *
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
void AircraftClass::Scatter(Coord const & , bool, bool )
{
	/*
	**	Certain missions prevent scattering regardless of whether it would be
	**	a good idea or not.
	*/
	if (!Current_Mission_Control().IsScatter) return;

	Enter_Idle_Mode();
}


/***********************************************************************************************
 * AircraftClass::Mission_Guard -- Handles aircraft in guard mode.                             *
 *                                                                                             *
 *    Aircraft don't like to be in guard mode if in flight. If this situation is detected,     *
 *    then figure out what the aircraft should be doing and go do it.                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of game frames to delay before calling this routine again. *
 *                                                                                             *
 * WARNINGS:   This routine typically calls the normal guard logic for ground units.           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1995 JLB : Created.                                                                 *
 *   10/10/1995 JLB : Hunts for harvesters that are unescorted.                                *
 *=============================================================================================*/
int AircraftClass::Do_MISSION_GUARD(void)
{
	if (HeightAGL == Class->Flight_Level()) {

		/*
		**	If part of a team, then do nothing, since the team
		**	handler will take care of giving this aircraft a
		**	mission.
		*/
		if (Team) {
			if (NavCom != NULL) {
				IsCarryallNotLanding = true; // RA2: set before the move order
				QueueMission(MISSION_MOVE, false); // RA2: QueueMission in place of Assign_Mission
			}
			return(Current_Mission_Control().Normal_Delay());
		}

		if (PrimaryWeapon == NULL) {
			Assign_Destination(&Map[(Coord const &)PositionCoord], true); // RA2: explicit second argument
			QueueMission(MISSION_MOVE, false); // RA2: as above
		} else {
			if (Team == NULL) {
				IsCarryallNotLanding = true; // RA2: as above
				Enter_Idle_Mode(false, true); // RA2: as above
			}
		}
		return(1);
	}

	/*
	**	If the aircraft cannot attack anything because of lack of ammo,
	**	abort any normal guard logic in order to look for a helipad
	**	to rearm.
	*/
	bool rearm = false; // RA2: three rules choose when to look for a helipad
	if (AircraftRearmWhenEmpty) { // RA2: as above
		rearm = (Ammo == 0 && Is_Weapon_Equipped()); // RA2: as above
	} else if (AircraftRearmWhenHalf) { // RA2: as above
		rearm = (Is_Weapon_Equipped() && Ammo < Class->MaxAmmo / 2); // RA2: as above
	} else {
		rearm = (Is_Weapon_Equipped() && Ammo < Class->MaxAmmo); // RA2: as above
	}
	if (rearm) { // RA2: as above
		if (!HasAnyLink()) { // RA2: HasAnyLink, not In_Radio_Contact
			TechnoClass * dock = TryNearestDockBuilding((TypeList<BuildingTypeClass *> *)&Class->Dock, 0, 0); // RA2: the whole Dock list at once
			if (dock != NULL) {
				QueueMission(MISSION_ENTER, false); // RA2: as above
				Assign_Destination(dock, true); // RA2: as above
				Assign_Target(NULL);
				return(1);
			}
		}
	}

	if (Ammo != -1 && Ammo < (Class->MaxAmmo / 2) && HasAnyLink()) { // RA2: as above
		if (Radio_Link(0)->Fetch_RTTI() == RTTI_BUILDING && ((BuildingClass *)Radio_Link(0))->Class->IsCanUnitReload) { // RA2: as above
			return(1);
		}
	}

	/*
	**	If the aircraft already has a target, then attack it if possible.
	*/
	if (TarCom != NULL) {
		QueueMission(MISSION_ATTACK, false); // RA2: as above
		return(1);
	}

	/*
	**	Transport helicopters don't really do anything but just sit there.
	*/
	if (!Is_Weapon_Equipped()) {
		return(TICKS_PER_SECOND*3);
	}

	/*
	**	Computer controlled helicopters will defend themselves by bouncing around
	**	and looking for a free helipad.
	*/
	if (HeightAGL == 0 && !HasAnyLink()) { // RA2: as above
		return(TICKS_PER_SECOND*3);
	}

	/*
	**	Perform a special check to hunt for harvesters that are outside of the protective
	**	shield of their base.
	*/
	if (!House->Is_Human_Player()) { // RA2: no State test
		AbstractClass * target = House->Find_Juicy_Target(PositionCoord);

		if (target != NULL) {
			Assign_Target(target);
			QueueMission(MISSION_ATTACK, false); // RA2: as above
		}
	}

	if (House->Is_Human_Player() && !In_Air()) {
		return(Current_Mission_Control().Normal_Delay() + Scen->RandomNumber(0, 2)); // RA2: the mission delay plus a random 0-2
	}

	return(BASECLASS::Do_MISSION_GUARD());
}



/***********************************************************************************************
 * AircraftClass::Mission_Guard_Area -- Handles the aircraft guard area logic.                 *
 *                                                                                             *
 *    This routine handles area guard logic for aircraft. Aircraft require special handling    *
 *    for this mode since they are to guard area only if they are in a position to do so.      *
 *    Otherwise they just defend themselves.                                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of game frames to delay before calling this routine        *
 *          again.                                                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/10/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int AircraftClass::Do_MISSION_GUARD_AREA(void)
{
	if (HeightAGL == Class->Flight_Level()) {
		if (Team == NULL) Enter_Idle_Mode(false, true); // RA2: RA2's form
		return(1);
	}
	if (Ammo == 0 && Is_Weapon_Equipped() && !HasAnyLink()) { // RA2
		Enter_Idle_Mode(false, true); // RA2: as above
	}
	if (TarCom != NULL) {
		QueueMission(MISSION_ATTACK, false); // RA2
		return(1);
	}
	return(BASECLASS::Do_MISSION_GUARD_AREA());
}



/***********************************************************************************************
 * AircraftClass::Can_Fire -- Checks to see if the aircraft can fire.                          *
 *                                                                                             *
 *    This routine is used to determine if the aircraft can fire its weapon at the target      *
 *    specified. If it cannot, then the reason why is returned.                                *
 *                                                                                             *
 * INPUT:   target   -- The target that the aircraft might fire upon.                          *
 *                                                                                             *
 *          which    -- The weapon that will be used to fire.                                  *
 *                                                                                             *
 * OUTPUT:  Returns with the reason why it can't fire or with FIRE_OK.                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/05/1996 JLB : Created.                                                                 *
 *   07/11/1996 JLB : Fixed for camera carrying aircraft.                                      *
 *=============================================================================================*/
FireErrorType AircraftClass::Can_Fire(AbstractClass * target, int which, bool ignoreRange) const // RA2: ignoreRange
{
	FireErrorType result = TechnoClass::Can_Fire(target, which, ignoreRange); // RA2: as above
	if (result == FIRE_OK) { // RA2: as above
		if (Passenger && !Cargo.Is_Something_Attached()) { // RA2: as above
			return(FIRE_AMMO); // RA2: as above
		}
		DirType facing = SecondaryFacing.Current(); // RA2: as above
		if (!const_cast<AircraftClass *>(this)->Is_Fighter()) { // RA2: as above
			short difference = (short)(facing.Raw - Direction(target).Raw); // RA2: as above
			if (abs(0x800) < abs(difference)) { // RA2: as above
				return(FIRE_FACING); // RA2: as above
			}
		}
		result = FIRE_OK; // RA2: as above
	}
	return result; // RA2: as above
}




/***********************************************************************************************
 * AircraftClass::Assign_Destination -- Assigns movement destination to the object.            *
 *                                                                                             *
 *    This routine is called when the object needs to have a new movement destination          *
 *    assigned.  Aircraft have their own version of this routine because a fixed-wing plane    *
 *    trying to land will behave poorly if given a new destination while it's landing.         *
 *                                                                                             *
 * INPUT:   destination -- The destination to assign to this object.                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/24/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void AircraftClass::Assign_Destination(AbstractClass * dest, bool immediate)
{
	if (dest != NULL) {

		/*
		 * A flying aircraft that is told to go somewhere must abort any landing
		 * approach it might be in the middle of.
		 */
		if (dest->In_Air()) {
			dest = NULL;
		} else {

			/*
			 * Special docking logic applies when this aircraft has been ordered to
			 * enter a building (a helipad or a repair facility).
			 */
			if (dest->What_Am_I() == RTTI_BUILDING && (Get_Mission() == MISSION_ENTER || MissionQueue == MISSION_ENTER)) { // RA2: the mission is read through the virtual
				BuildingClass * destination = (BuildingClass *)dest;

				if (!HasAnyLink()) { // RA2: HasAnyLink, not In_Radio_Contact

					/*
					 * The destination building is already occupied (in radio contact).
					 * Try to find an alternate docking bay to head for instead.
					 */
					if (!destination->HasFreeLink(this)) { // RA2: HasFreeLink(this) of the building
						Assign_Archive_Target(dest); // RA2: the archive target is set through Assign_Archive_Target

						if (destination->Class->IsHelipad) {
							TechnoClass * dock = TryNearestDockBuilding((TypeList<BuildingTypeClass *> *)&Class->Dock, 0, 0); // RA2: the whole Dock list at once
							Assign_Destination(NULL, true); // RA2: explicit second argument

							MissionType mission = MISSION_MOVE;
							if (dock != NULL && Transmit_Message(RADIO_CAN_LOAD, dock) == RADIO_ROGER) { // RA2: as above
								Transmit_Message(RADIO_HELLO, dock); // RA2: as above
								dest = dock; // RA2: as above
								mission = MISSION_ENTER;
							} else {
								dest = Good_LZ();
							}
							QueueMission(mission, false); // RA2: QueueMission in place of Assign_Mission
							if (Ready_To_Commence()) {
								Commence();
							}
						}

						if (destination->Class->IsCanUnitRepair) {
							QueueUpToEnter = destination; // RA2: QueueUpToEnter, the object to enter once nearby
							dest = NULL;
						}

					} else {

						/*
						 * The destination is free. Ask permission to dock; if refused,
						 * break contact and fall back on the archive target.
						 */
						if (Transmit_Message(RADIO_DOCKING, destination) != RADIO_ROGER) { // RA2: the building is passed as a TechnoClass
							Transmit_Message(RADIO_OVER_OUT); // RA2: the message-only overload
							if (destination->Class->IsCanUnitRepair || destination->Class->IsCanUnitReload) {
								Assign_Archive_Target(dest); // RA2: as above
								dest = NULL;
							}
						}

						if (destination->Class->IsCanUnitRepair || destination->Class->IsCanUnitReload) {
							if (NavCom != NULL) {
								Assign_Archive_Target(NavCom); // RA2: as above
							}
						} else {
							Assign_Archive_Target(dest); // RA2: as above
						}
					}

				} else {

					/*
					 * This aircraft is already in radio contact. If the destination
					 * building is not yet in contact, perform the docking handshake. If
					 * it is already our own contact there is nothing to do; if it is in
					 * contact with someone else, just stash it as the archive target.
					 */
					TechnoClass * building = Dynamic_Cast<TechnoClass *>(dest);
					if (building != NULL) {
						if (!building->HasFreeLink(this)) { // RA2: as above
							Assign_Archive_Target(dest); // RA2: as above
						} else if (!building->ContainsLink(this)) { // RA2: as above
							if (Transmit_Message(RADIO_DOCKING) == RADIO_ROGER) { // RA2: the message-only overload
								return; // RA2: as above
							}
							Transmit_Message(RADIO_OVER_OUT); // RA2: as above
							if (destination->Class->IsCanUnitRepair || destination->Class->IsCanUnitReload) {
								Assign_Archive_Target(dest); // RA2: as above
								dest = NULL;
							}
						}
					}
				}
			}

			/*
			 * If sitting on top of a repair or reload building, make sure power is
			 * restored to the locomotion and break radio contact with it if this
			 * aircraft is being sent somewhere other than that building.
			 */
			CellClass * cellptr = Get_Cell_Ptr();
			if (!cellptr->IsUnderBridge) {
				BuildingClass * occupier = (BuildingClass *)cellptr->Cell_Occupier();

				while (occupier != NULL) {
					if (occupier != (BuildingClass *)this && occupier->Fetch_RTTI() == RTTI_BUILDING) { // RA2: the virtual Fetch_RTTI
						break;
					}
					occupier = (BuildingClass *)occupier->Next;
				}

				if (occupier != NULL && (occupier->Class->IsCanUnitRepair || occupier->Class->IsCanUnitReload)) {
					if (!Locomotion->Is_Powered() && !House->Ion_Sensitives_Should_Be_Offline()) { // RA2: the house's ion storm check
						Locomotion->Power_On();
					}

					if (Radio_Link(0) == occupier && dest != Radio_Link(0)) { // RA2: Radio_Link(0), not Contact_With_Whom
						Transmit_Message(RADIO_OVER_OUT); // RA2: the message-only overload
					}
				}
			}
		}
	}

	BASECLASS::Assign_Destination(dest, immediate);
}



/***********************************************************************************************
 * AircraftClass::In_Which_Layer -- Calculates the display layer of the aircraft.              *
 *                                                                                             *
 *    This examines the aircraft to determine what display layer it should be located          *
 *    in. Fixed wing aircraft must always be in the top layer if they are flying even though   *
 *    they may be low to the ground.                                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the layer that this aircraft resides in.                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/20/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
LayerType AircraftClass::In_Which_Layer(void) const
{
	return(Locomotion->In_Which_Layer());
}


/***********************************************************************************************
 * AircraftClass::Look -- Aircraft will look if they are on the ground always.                 *
 *                                                                                             *
 *    Aircraft perform a look operation according to their sight range. If the aircraft is     *
 *    on the ground, then it will look a distance of one cell regardless of what its           *
 *    specified sight range is.                                                                *
 *                                                                                             *
 * INPUT:   incremental -- Is this an incremental look?                                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/23/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void AircraftClass::Look(bool incremental, bool dont_map)
{
	Coord coord; // RA2: RA2's form
	int sight = Class->SightRange; // RA2: as above
	if (HeightAGL == 0) {
		sight = 1; // RA2: as above
	}
	if (sight != 0) { // RA2: as above
		coord = PositionCoord; // RA2: as above
		Map.Sight_From(coord, sight, House, incremental, dont_map, 0, 1, 0); // RA2: as above
		coord = PositionCoord; // RA2: as above
		Map.Sight_From(coord, sight, House, incremental, dont_map, 0, 1, 1); // RA2: as above
	} else if (*(int *)&Scen->Special & 0x1000) { // RA2: as above
		sight = Rule->AircraftFogReveal; // RA2: as above
		coord = PositionCoord; // RA2: as above
		Map.Sight_From(coord, sight, House, 0, 0, 1, HeightAGL < Rule->FlightLevel / 2, 0); // RA2: as above
		coord = PositionCoord; // RA2: as above
		Map.Sight_From(coord, sight, House, 0, 0, 1, HeightAGL < Rule->FlightLevel / 2, 1); // RA2: as above
	}
}



/***********************************************************************************************
 * AircraftClass::Write_INI -- Store the units to the INI database.                            *
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
void AircraftClass::Write_INI(CCINIClass & ini)
{
	/*
	 * First, clear out all existing aircraft data from the ini file.
	 */
	ini.Clear(INI_NAME);

	/*
	 * Write the aircraft data out.
	 */
	for (int index = 0; index < Aircraft.Count(); index++) {
		AircraftClass * air = Aircraft[index];
		if (air != NULL && !air->IsInLimbo && air->IsActive) {
			char	uname[10];
			char	buf[128];

			sprintf(uname, "%d", index);

			sprintf(buf, "%s,%s,%d,%d,%d,%d,%s,%s,%d,%d,%d,%d",
				(char const *)air->House->IniName, // RA2: Write_INI at 0x41A6C0 matches only so
				(char const *)air->Class->IniName,
				(int)(air->HealthRatio * 256), // RA2: as above
				air->PositionCell.X,
				air->PositionCell.Y,
				air->PrimaryFacing.Current().As_Dir256(),
				MissionClass::Mission_Name(air->Get_Mission()), // RA2: as above
				(air->Tag != NULL) ? (char const *)air->Tag->Class->IniName : "None",
				air->Veterancy.To_Integer(),
				air->Group,
				air->IsTeamRecruitable,
				air->IsAutocreateRecruitable
				);
			ini.Put_String(INI_NAME, uname, buf);
		}
	}
}


/***********************************************************************************************
 * AircraftClass::Read_INI -- Reads aircraft object data from an INI file.                     *
 *                                                                                             *
 *    This routine is used to read the aircraft object data from the INI file buffer           *
 *    specified. This is used by the scenario loader code to interpret the INI file and        *
 *    create the specified objects therein.                                                    *
 *                                                                                             *
 * INPUT:   buffer   -- Pointer to the INI buffer.                                             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/26/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void AircraftClass::Read_INI(CCINIClass const & ini)
{
	AircraftClass	* air;      // Working unit pointer.
	AircraftType	classid;    // Unit class.
	char				buf[128];

	int counter = ini.Entry_Count(INI_NAME);

	for (int index = 0; index < counter; index++) {
		char const * entry = ini.Get_Entry(INI_NAME, index);

		ini.Get_String(INI_NAME, entry, NULL, buf, sizeof(buf));

		int house_index = House_Index_From_Name(strtok(buf, ",")); // RA2: the house is found by index in Houses
		if (house_index != -1) { // RA2: as above
			classid = AircraftTypeClass::From_Name(strtok(NULL, ","));

			if (classid != AIRCRAFT_NONE) {

				HouseClass * inhousep = Houses[house_index]; // RA2: as above

				air = new AircraftClass(AircraftTypes[classid], inhousep);
				if (air) {

					/*
					**	Read the raw data.
					*/
					int strength = atoi(strtok(NULL, ","));

					Cell cell;
					Coord coord;
					if (NewINIFormat >= 4) {
						int x = atoi(strtok(NULL, ","));
						int y = atoi(strtok(NULL, ","));
						cell = Cell((short)x, y); // RA2: x is narrowed to a short
					} else {
						int c = atoi(strtok(NULL, ","));
						cell = Cell(c % 128, c / 128); // RA2: the cell number is split by hand
					}

					coord = cell;

					Dir256 dir = (Dir256)atoi(strtok(NULL, ","));

					MissionType mission = MissionClass::Mission_From_Name(strtok(NULL, ","));

					TagTypeClass * tp = TagTypeClass::From_Name(strtok(NULL,","));
					if (tp != NULL) {
						TagClass * tt = Find_Or_Make(tp);
						if (tt != NULL) {
							air->Attach_Tag(tt);
						}
					}

					char * token = strtok(NULL, ",");
					if (token != NULL) {
						air->Veterancy.From_Integer(atoi(token));
					}

					token = strtok(NULL, ",");
					if (token != NULL) {
						air->Group = atoi(token);
					}

					token = strtok(NULL, ",");
					if (token != NULL) {
						air->IsTeamRecruitable = atoi(token) != 0;
					}

					token = strtok(NULL, ",");
					if (token != NULL) {
						air->IsAutocreateRecruitable = atoi(token) != 0;
					}

					if (air->Unlimbo(coord, dir)) {
						double fs;
						air->Strength = (fs = strength) * air->Class->MaxStrength / 256.0; // RA2: the strength is scaled first
						if (air->Strength > air->Class->MaxStrength-3) air->Strength = air->Class->MaxStrength;
						if (air->Strength == 0) air->Strength = 1;
						air->EstimatedHealth = air->Strength; // RA2: the health the owner last saw
						if (Session.Type == GAME_NORMAL || air->House->Is_Human_Player()) {
							air->QueueMission(mission, false); // RA2: the mission is queued
							air->Commence();
						} else {
							air->Enter_Idle_Mode(false, true); // RA2: explicit arguments
						}
					} else {
						/*
						**	If the aircraft could not be unlimboed, then this is a catastrophic error
						**	condition. Delete the aircraft.
						*/
						delete air;
					}
				}
			}
		}
	}
}



/// <summary>
/// Reads this aircraft back in from the save game stream.
/// The virtual table pointers are rebuilt as part of the load, the locomotor is carried
/// across intact, and the aircraft re-registers itself with the target tracker under the
/// identifier it was saved with.
/// </summary>
/// <param name="stream">The stream to read this object from.</param>
/// <returns>Returns with S_OK if the aircraft was loaded successfully.</returns>
HRESULT STDMETHODCALLTYPE AircraftClass::Load(IStream * stream)
{
	TargetTracker.Remove_Index(Fetch_ID());
	HRESULT result = BASECLASS::Load(stream);
	if (SUCCEEDED(result)) {
		ILocomotion * locomotion = Locomotion;

		new (this) AircraftClass(NoInitClass());

		Locomotion = locomotion;
		locomotion->Release();

		TargetTracker.Add_Index(Fetch_ID(), this);

		Swizzle_Pointer(&Class);
		Swizzle_Pointer(&DockNowHeadingTo); // RA2: Load at 0x41AB80 matches only so

		result = S_OK;
	}
	return(result);
}


/// <summary>
/// Writes this aircraft out to the save game stream.
/// </summary>
/// <param name="stream">The stream to write this object to.</param>
/// <param name="cleardirty">Should the object be marked as clean once written?</param>
/// <returns>Returns with S_OK if the aircraft was written successfully.</returns>
HRESULT STDMETHODCALLTYPE AircraftClass::Save(IStream * stream, int cleardirty)
{
	return(BASECLASS::Save(stream, cleardirty));
}


#ifdef _DEBUG
/***********************************************************************************************
 * AircraftClass::Debug_Dump -- Displays the status of the aircraft to the mono monitor.       *
 *                                                                                             *
 *    This displays the current status of the aircraft class to the mono monitor. By this      *
 *    display bugs may be tracked down or prevented.                                           *
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
void AircraftClass::Debug_Dump(MonoClass * mono) const
{
	mono->Set_Cursor(0, 0);
	mono->Set_Cursor(1, 11);mono->Printf("%3d", AttacksRemaining);

	BASECLASS::Debug_Dump(mono);
}
#endif



/// <summary>
/// Is this aircraft ready to move on to its next mission step?
/// An aircraft that is holding a locked heading, or that is on a sticky or rescue
/// mission, is left to finish what it is doing.
/// </summary>
/// <returns>bool; Is the aircraft ready to commence?</returns>
bool AircraftClass::Ready_To_Commence(void)
{
	if (CurrentMission == MISSION_STICKY || CurrentMission == MISSION_RESCUE || IsLockedStraight) { // RA2: RA2's form
		return(false); // RA2: as above
	}
	return(IsCarryallNotLanding ? true : false); // RA2: as above
}






/// <summary>
/// Adds this aircraft's state to the running checksum.
/// This routine is used by the network synchronization check to prove that every machine
/// agrees about the condition of this aircraft.
/// </summary>
/// <param name="crc">The checksum engine to submit this object's state to.</param>
void AircraftClass::Compute_CRC(CRCEngine & crc) const
{
	BASECLASS::Compute_CRC(crc);
	crc(Passenger);
	crc(Unused_5B8); // RA2: RA2's form
	crc(Unused_5B9); // RA2: as above
	crc((long)NumParadropsLeft); // RA2: as above
}
 // RA2: no AttacksRemaining (TibSun's use does not compile against RA2's declaration)


/// <summary>
/// Removes all references to the specified object.
/// This routine is called when an object is about to disappear, so that this aircraft is
/// not left holding a pointer to it.
/// </summary>
/// <param name="target">The object that is going away.</param>
/// <param name="all">Should even the loosest of references be severed?</param>
void AircraftClass::Detach(AbstractClass const * target, bool all)
{
	BASECLASS::Detach(target, all);
	if (DockNowHeadingTo == target) { // RA2: Detach at 0x41ADB0 matches only so
		DockNowHeadingTo = NULL; // RA2: as above
	}
	if (Class == target) {
		Class = NULL;
	}
}


/// <summary>
/// Fetches the altitude that this aircraft should settle at.
/// An empty carryall touches right down when it has been sent to enter a helipad or
/// repair bay, but otherwise a carryall holds station just above the ground so that it
/// can pick up or set down its cargo.
/// </summary>
/// <returns>Returns with the height above ground level to settle at.</returns>
LONG STDMETHODCALLTYPE AircraftClass::Landing_Altitude(void)
{
	if (Class->IsCarryall && !Cargo.Is_Something_Attached() && HasAnyLink()) { // RA2: RA2's form
		TechnoClass * tptr = ((RadioClass const *)this)->Radio_Link(0); // RA2: the const Radio_Link (0x636E20)
		if (Get_Mission() == MISSION_ENTER && tptr != NULL && tptr->Fetch_RTTI() == RTTI_BUILDING) { // RA2: as above
			BuildingTypeClass const * btype = ((BuildingClass *)tptr)->Class; // RA2: as above
			if (btype->IsCanUnitRepair || btype->IsHelipad) { // RA2: as above
				return(0); // RA2: as above
			}
		}
	}
	if ((HasAnyLink() && Class->IsCarryall) || (Cargo.Is_Something_Attached() && Class->IsCarryall)) { // RA2: as above
		return(100);
	}
	return(0);
}



/// <summary>
/// Fetches the facing that this aircraft should land at.
/// The aircraft lines itself up with whatever it is in radio contact with, since that is
/// the helipad or unit it has come to service. Failing that it holds its present heading
/// while loaded, or settles into the default parked pose.
/// </summary>
/// <returns>Returns with the facing to land at.</returns>
LONG STDMETHODCALLTYPE AircraftClass::Landing_Direction(void)
{
	if (HasAnyLink()) { // RA2: RA2's form
		TechnoClass * tptr = ((RadioClass const *)this)->Radio_Link(0); // RA2: the const Radio_Link (0x636E20)
		return(tptr->PrimaryFacing.Current().As_Dir8());
	}
	if (Cargo.Is_Something_Attached()) {
		return(SecondaryFacing.Current().As_Dir8());
	}
	return(Rule->PoseDir);
}



/// <summary>
/// Is this aircraft carrying anything?
/// The locomotor asks this so that it can fly a loaded aircraft differently from an
/// empty one.
/// </summary>
/// <returns>Returns with true if there is cargo aboard this aircraft.</returns>
BOOL STDMETHODCALLTYPE AircraftClass::Is_Loaded(void)
{
	return(Cargo.Is_Something_Attached());
}


/// <summary>
/// Should this aircraft attack by strafing?
/// The locomotor asks this in order to choose between a strafing run and an attack made
/// from a hover. Only a visible and unguided projectile is suited to strafing.
/// </summary>
/// <returns>Returns with true if the aircraft should make strafing attack runs.</returns>
LONG STDMETHODCALLTYPE AircraftClass::Is_Strafe(void)
{
	const WeaponDataStruct * data = Get_Class_Weapon_Data(0);
	if (data == NULL) {
		return(false);
	}
	WeaponTypeClass *weapon = data->Weapon;
	if (weapon == NULL) {
		return(false);
	}
	if (weapon->Bullet->ROT > 1) {
		return(false);
	}
	if (weapon->Bullet->IsInvisible) {
		return(false);
	}
	return(true);
}





/// <summary>
/// Is this aircraft a fighter?
/// </summary>
LONG STDMETHODCALLTYPE AircraftClass::Is_Fighter(void) // RA2: not in TibSun, at 0x41AF90
{
	return(Class->IsFighter != 0);
}


/// <summary>
/// Is this aircraft locked into straight and level flight?
/// The locomotor asks this so that it will not turn the aircraft while it is committed
/// to an attack run.
/// </summary>
/// <returns>Returns with true if the aircraft must hold its present heading.</returns>
LONG STDMETHODCALLTYPE AircraftClass::Is_Locked(void)
{
	return(IsLockedStraight);
}


/// <summary>
/// Starts this aircraft on its assigned mission.
/// The aircraft is released from any straight and level flight restriction before the
/// normal commence processing takes over.
/// </summary>
/// <returns>bool; Was the mission commenced?</returns>
bool AircraftClass::Commence(void)
{
	IsLockedStraight = false;
	return(BASECLASS::Commence());
}


/// <summary>
/// Should this aircraft be quietly removed once it leaves the map?
/// A loaner, or an aircraft belonging to a team that is on its way off the map, has no
/// further business in the scenario and is deleted rather than left to fly forever.
/// </summary>
/// <returns>bool; Should this aircraft be deleted when it flies off the map?</returns>
bool AircraftClass::Should_Delete_Off_Map(void)
{
	if (TarCom == 0 && IsLocked && Get_Mission() != MISSION_PARADROP_APPROACH && Get_Mission() != MISSION_PARADROP_OVERFLY) { // RA2: RA2's form
		if ((Team == 0 && IsALoaner) || (Team != 0 && Team->Is_Leaving_Map())) { // RA2: as above
			return(true);
		}
	}
	return(false);
}



/// <summary>
/// Spends a round of this aircraft's ammunition.
/// Aircraft do not lose ammunition through the normal firing sequence -- their supply is
/// accounted for by the mission code instead -- so this override does nothing at all.
/// </summary>
void AircraftClass::Reduce_Ammunition(void)
{
	//nothing
}


/// <summary>
/// Is this aircraft to be treated as a ground vehicle?
/// An aircraft sitting on the ground occupies its cell and is handled like any other
/// vehicle by the code that asks this question.
/// </summary>
/// <returns>bool; Should this aircraft be considered a vehicle?</returns>
bool AircraftClass::Considered_Vehicle(void) const
{
	return(On_Ground());
}


bool AircraftClass::In_Air(void) const // RA2: not in TibSun, at 0x41B050
{
	if (Class == Rule->V3Rocket.Type || Class == Rule->DMisl.Type) {
		return(Locomotion->Is_Moving_Now() != 0);
	}
	return(BASECLASS::In_Air());
}


bool AircraftClass::On_Ground(void) const // RA2: not in TibSun, at 0x41B0B0
{
	if (Class == Rule->V3Rocket.Type || Class == Rule->DMisl.Type) {
		return(Locomotion->Is_Moving_Now() == 0);
	}
	return(BASECLASS::On_Ground());
}


bool AircraftClass::IsNotWarpingIn(void) const // RA2: not in TibSun, at 0x41B110
{
	return(IsReturningFromAttackRun);
}


void AircraftClass::Set_Mission(MissionType mission) // RA2: not in TibSun, at 0x41B120
{
	if ((CurrentMission == MISSION_RETREAT || (CurrentMission > MISSION_PATROL && CurrentMission <= MISSION_PARADROP_OVERFLY)) && mission != MISSION_RETREAT && (mission <= MISSION_PATROL || mission > MISSION_PARADROP_OVERFLY)) { // RA2: only retreat and paradrop guarded
		return; // RA2: as above
	}
	BASECLASS::Set_Mission(mission); // RA2: as above
}











void AircraftClass::QueueMission(MissionType mission, bool start) // RA2: not in TibSun, at 0x41B160
{
	if ((CurrentMission == MISSION_RETREAT || (CurrentMission > MISSION_PATROL && CurrentMission <= MISSION_PARADROP_OVERFLY)) && mission != MISSION_RETREAT && (mission <= MISSION_PATROL || mission > MISSION_PARADROP_OVERFLY)) { // RA2: only retreat and paradrop guarded
		return; // RA2: as above
	}
	BASECLASS::QueueMission(mission, start); // RA2: as above
}











void AircraftClass::Override_Mission(MissionType mission, AbstractClass * tarcom, AbstractClass * navcom) // RA2: not in TibSun, at 0x41B1A0
{
	if ((CurrentMission == MISSION_RETREAT || (CurrentMission > MISSION_PATROL && CurrentMission <= MISSION_PARADROP_OVERFLY)) && mission != MISSION_RETREAT && (mission <= MISSION_PATROL || mission > MISSION_PARADROP_OVERFLY)) { // RA2: only retreat and paradrop guarded
		return; // RA2: as above
	}
	BASECLASS::Override_Mission(mission, tarcom, navcom); // RA2: as above
}








/// <summary>
/// An airport-bound aircraft keeps the dock it is heading for while that one still takes it; otherwise
/// the nearest suitable dock becomes its new one.
/// </summary>
BuildingClass * AircraftClass::TryNearestDockBuilding(TypeList<BuildingTypeClass *> * list, unsigned unk2, unsigned unk3) const // RA2: not in TibSun, at 0x41B1E0
{
	AircraftClass * self = (AircraftClass *)this;
	if (Class->IsAirportBound && DockNowHeadingTo != NULL) {
		if (self->Transmit_Message(RADIO_CAN_LOAD, DockNowHeadingTo) != RADIO_ROGER) {
			self->DockNowHeadingTo = NULL;
		} else {
			return(DockNowHeadingTo);
		}
	}
	self->DockNowHeadingTo = BASECLASS::TryNearestDockBuilding(list, 0, unk3);
	return(DockNowHeadingTo);
}

































/// <summary>
/// Sends this aircraft plummeting out of the sky.
/// This routine is used when an airborne aircraft is destroyed. Rather than simply
/// vanishing, the aircraft is stunned and set rocking so that it tumbles to the ground.
/// The kill is credited, any attached trigger is sprung, and the cargo dies with it.
/// RA2
/// </summary>
/// <param name="source">The object responsible for the kill. This may be NULL.</param>
/// <returns>bool; Was the aircraft sent into a crash?</returns>
void TechnoClass::ClearDisguise(void)
{
	Disguised = false;
}








/// <summary>
/// Determines how much storage this object requires.
/// This routine is used by the save/load system. Older save games were written before a
/// couple of the TechnoClass members existed, so the reported size is trimmed back to
/// what those files expect.
/// </summary>
/// <param name="oldsave">Is the size wanted for the older save game format?</param>
/// <returns>Returns with the number of bytes this object occupies.</returns>
int AircraftClass::Fetch_Object_Size(void) const
{
	return(sizeof(*this));
}


/// <summary>
/// Fetches the run time type of this object.
/// </summary>
/// <returns>Returns with RTTI_AIRCRAFT.</returns>
RTTIType AircraftClass::Fetch_RTTI(void) const
{
	return(RTTI_AIRCRAFT);
}


/// <summary>
/// Fetches the class identifier of this object.
/// This routine is part of the persistence support. The save/load machinery uses the
/// class identifier to recreate an object of the correct type when a game is restored.
/// </summary>
/// <param name="retval">Pointer to the identifier to fill in.</param>
/// <returns>Returns with S_OK, or E_POINTER if no destination was supplied.</returns>
HRESULT STDMETHODCALLTYPE AircraftClass::GetClassID(CLSID * retval)
{
	if (retval == NULL) return(E_POINTER);
	*retval = CLSID_AircraftClass;
	return(S_OK);
}


/// <summary>
/// Fetches the descriptive name of this aircraft.
/// </summary>
/// <returns>Returns with a pointer to the name given to this aircraft's type.</returns>
wchar_t const * AircraftClass::Full_Name(void) const // RA2: wide
{
	return(Class->UIName); // RA2
}


/// <summary>
/// Fetches the direction that this aircraft's weapon is pointing.
/// An aircraft carries its weapon on the secondary facing, so this is what the targeting
/// and rendering code is told when it asks about the turret.
/// </summary>
/// <returns>Returns with the current turret facing.</returns>
DirType AircraftClass::Turret_Facing(void) const
{
	return(SecondaryFacing.Current());
}


/***********************************************************************************************
 * AircraftClass::Class_Of -- Fetches a reference to the class type for this object.           *
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
ObjectTypeClass const * AircraftClass::Class_Of(void) const
{
	return(Class);
}

