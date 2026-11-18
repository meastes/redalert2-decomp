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

/* $Header: /CounterStrike/BUILDING.CPP 5     3/13/97 5:18p Joe_b $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : BUILDING.CPP                                                 *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : September 10, 1993                                           *
 *                                                                                             *
 *                  Last Update : October 27, 1996 [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   BuildingClass::AI -- Handles non-graphic AI processing for buildings.                     *
 *   BuildingClass::Active_Click_With -- Handles cell selection for buildings.                 *
 *   BuildingClass::Animation_AI -- Handles normal building animation processing.              *
 *   BuildingClass::Assign_Target -- Assigns a target to the building.                         *
 *   BuildingClass::Begin_Mode -- Begins an animation mode for the building.                   *
 *   BuildingClass::BuildingClass -- Constructor for buildings.                                *
 *   BuildingClass::Can_Demolish -- Can the player demolish (sell back) the building?          *
 *   BuildingClass::Can_Enter_Cell -- Determines if building can be placed down.               *
 *   BuildingClass::Can_Fire -- Determines if this building can fire.                          *
 *   BuildingClass::Can_Player_Move -- Can this building be moved?                             *
 *   BuildingClass::Captured -- Captures the building.                                         *
 *   BuildingClass::Center_Coord -- Fetches the center coordinate for the building.            *
 *   BuildingClass::Charging_AI -- Handles the special charging logic for Tesla coils.         *
 *   BuildingClass::Check_Point -- Fetches the landing checkpoint for the given flight pattern.*
 *   BuildingClass::Click_With -- Handles clicking on the map while the building is selected.  *
 *   BuildingClass::Crew_Type -- This determines the crew that this object generates.          *
 *   BuildingClass::Death_Announcement -- Announce the death of this building.                 *
 *   BuildingClass::Debug_Dump -- Displays building status to the monochrome screen.           *
 *   BuildingClass::Detach -- Handles target removal from the game system.                     *
 *   BuildingClass::Detach_All -- Possibly abandons production according to factory type.      *
 *   BuildingClass::Docking_Coord -- Fetches the coordinate to use for docking.                *
 *   BuildingClass::Draw_It -- Displays the building at the location specified.                *
 *   BuildingClass::Drop_Debris -- Drops rubble when building is destroyed.                    *
 *   BuildingClass::Enter_Idle_Mode -- The building will enter its idle mode.                  *
 *   BuildingClass::Exit_Coord -- Determines location where object will leave it.              *
 *   BuildingClass::Exit_Object -- Initiates an object to leave the building.                  *
 *   BuildingClass::Factory_AI -- Handle factory production and initiation.                    *
 *   BuildingClass::Find_Exit_Cell -- Find a clear location to exit an object from this buildin*
 *   BuildingClass::Fire_Direction -- Fetches the direction of firing.                         *
 *   BuildingClass::Fire_Out -- Handles when attached animation expires.                       *
 *   BuildingClass::Flush_For_Placement -- Handles clearing a zone for object placement.       *
 *   BuildingClass::Get_Image_Data -- Fetch the image pointer for the building.                *
 *   BuildingClass::Grand_Opening -- Handles construction completed special operations.        *
 *   BuildingClass::Greatest_Threat -- Searches for target that building can fire upon.        *
 *   BuildingClass::How_Many_Survivors -- This determine the maximum number of survivors.      *
 *   BuildingClass::Init -- Initialize the building system to an empty null state.             *
 *   BuildingClass::Limbo -- Handles power adjustment as building goes into limbo.             *
 *   BuildingClass::Mark -- Building interface to map rendering system.                        *
 *   BuildingClass::Mission_Attack -- Handles attack mission for building.                     *
 *   BuildingClass::Mission_Construction -- Handles mission construction.                      *
 *   BuildingClass::Mission_Deconstruction -- Handles building deconstruction.                 *
 *   BuildingClass::Mission_Guard -- Handles guard mission for combat buildings.               *
 *   BuildingClass::Mission_Harvest -- Handles refinery unloading harvesters.                  *
 *   BuildingClass::Mission_Missile -- State machine for nuclear missile launch.               *
 *   BuildingClass::Mission_Repair -- Handles the repair (active) state for building.          *
 *   BuildingClass::Mission_Unload -- Handles the unload mission for a building.               *
 *   BuildingClass::Pip_Count -- Determines "full" pips to display for building.               *
 *   BuildingClass::Power_Output -- Fetches the current power output from this building.       *
 *   BuildingClass::Read_INI -- Reads buildings from INI file.                                 *
 *   BuildingClass::Receive_Message -- Handle an incoming message to the building.             *
 *   BuildingClass::Remap_Table -- Fetches the remap table to use for this building.           *
 *   BuildingClass::Remove_Gap_Effect -- Stop a gap generator from jamming cells               *
 *   BuildingClass::Repair -- Initiates or terminates the repair process.                      *
 *   BuildingClass::Repair_AI -- Handle the repair (and sell) logic for the building.          *
 *   BuildingClass::Revealed -- Reveals the building to the specified house.                   *
 *   BuildingClass::Rotation_AI -- Process any turret rotation required of this building.      *
 *   BuildingClass::Sell_Back -- Controls the sell back (demolish) operation.                  *
 *   BuildingClass::Shape_Number -- Fetch the shape number for this building.                  *
 *   BuildingClass::Sort_Y -- Returns the building coordinate used for sorting.                *
 *   BuildingClass::Take_Damage -- Inflicts damage points upon a building.                     *
 *   BuildingClass::Target_Coord -- Return the coordinate to use when firing on this building. *
 *   BuildingClass::Toggle_Primary -- Toggles the primary factory state.                       *
 *   BuildingClass::Turret_Facing -- Fetches the turret facing for this building.              *
 *   BuildingClass::Unlimbo -- Removes a building from limbo state.                            *
 *   BuildingClass::Update_Buildables -- Informs sidebar of additional construction options.   *
 *   BuildingClass::Value -- Determine the value of this building.                             *
 *   BuildingClass::What_Action -- Determines action to perform if click on specified object.  *
 *   BuildingClass::What_Action -- Determines what action will occur.                          *
 *   BuildingClass::Write_INI -- Write out the building data to the INI file specified.        *
 *   BuildingClass::delete -- Deallocates building object.                                     *
 *   BuildingClass::new -- Allocates a building object from building pool.                     *
 *   BuildingClass::~BuildingClass -- Destructor for building type objects.                    *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#define INCLUDE_COM
#include "always.h"
#include "geometry.h" // RA2: the header statics, in the order of the unit's initializer table

#include "building.h"

#include "_convert.h"
#include "_keyboar.h"
#include "_map.h"
#include "_rect.h"
#include "_rules.h"
#include "_surface.h"
#include "_tactica.h"
#include "aircraft.h"
#include "anim.h"
#include "animtype.h"
#include "blight.h"
#include "bound.h"
#include "bsurface.h"
#include "builtype.h"
#include "bullet.h"
#include "bullettype.h"
#include "ccrand.h"
#include "cell.h"
#include "combat.h"
#include "conquer.h"
#include "dbgprint.h"
#include "draw.h"
#include "drive.h"
#include "dsurface.h"
#include "event.h"
#include "factory.h"
#include "fog.h"
#include "globals.h"
#include "goptions.h"
#include "house.h"
#include "houstype.h"
#include "iloco.h"
#include "ilocos.h"
#include "incdec.h"
#include "infantry.h"
#include "infatype.h"
#include "inline.h"
#include "ion.h"
#include "ipiggy.h"
#include "isotile.h"
#include "isotype.h"
#include "laser.h" // RA2
#include "light.h"
#include "lightcon.h"
#include "mono.h"
#include "overlay.h"
#include "overtype.h"
#include "partsys.h"
#include "queue.h"
#include "revent.h"
#include "rules.h"
#include "scheme.h"
#include "session.h"
#include "shapeset.h"
#include "smudtype.h"
#include "stimer.h"
#include "sun.h"
#include "super.h"
#include "suprtype.h"
#include "swizzle.h"
#include "tactical.h"
#include "tag.h"
#include "tagtype.h"
#include "techtype.h"
#include "capmgr.h" // RA2: BuildingClass::Receive_Message reads the controlled unit of an infantry's capture manager
#include "temporal.h" // RA2: BuildingClass::AI runs the temporal weapon aimed at the building
#include "tiberium.h"
#include "tracker.h"
#include "unit.h"
#include "unittype.h"
#include "vox.h"
#include "warhead.h"
#include "weapon.h"

#include "color.hh"
#include "ftimer.h"

void Record_Event_Cell(Cell const & cell); // RA2: 0x63C6C0, the ring of recent event cells
#include "side.h" // RA2: SideClass::From_Name for the garrison owner
#include "bomb.h" // RA2: Active_Click_With reads the attached bomb's owner

void Queue_Speech(char const * name, int priority, int expire); // RA2: 0x715B40, an EVA line by name
void __fastcall Cloak_Cell_Occupiers(CellClass * cellptr); // RA2: defined after its first caller

namespace Unsorted { extern int MuteSWLaunches; extern bool ArmageddonMode; }
namespace Unsorted { extern bool ArmageddonMode; }



char const * const BuildingClass::INI_NAME = "Structures";



enum SAMState {
	SAM_READY,					// Launcher can be facing any direction tracking targets.
	SAM_FIRING					// Stationary while missile is being fired.
};








/// <summary>
/// Whether one of the player's powered psychic detectors reaches the cell; never for the player's allies.
/// </summary>
bool __fastcall Psychic_Detector_Covers_Cell(HouseClass const * house, CellClass const * cell) // RA2: 0x00438A60 (anim.cpp calls it); free function of the executable
{
	if (PlayerPtr->Is_Ally(house)) return(false); // RA2: as above
	int count = PlayerPtr->PsychicDetectionBuildings.Count(); // RA2: as above
	for (int index = 0; index < count; index++) { // RA2: as above
		BuildingClass * detector = PlayerPtr->PsychicDetectionBuildings[index]; // RA2: as above
		if (detector->IsPowerOnline()) { // RA2: as above
			Coord position = detector->Center_Coord(); // RA2: as above
			Coord other = cell->Cell_Coord(); // RA2: as above
			Coord delta(position.X - other.X, position.Y - other.Y, position.Z - other.Z); // RA2: as above
			int distance = (int)fastmath::sqrt((double)delta.X * delta.X + (double)delta.Y * delta.Y + (double)delta.Z * delta.Z); // RA2: as above
			if (distance <= (detector->Class->PsychicDetectionRadius << 8)) return(true); // RA2: as above
		}
	}
	return(false); // RA2: as above
}


/// <summary>
/// Makes the named animation at the target for the house and attaches the bullet to it.
/// </summary>
AnimClass * __fastcall Create_Attached_Anim(HouseClass * house, AbstractClass * target, BulletClass * bullet, char const * anim) // RA2: 0x00438B80 (super.cpp declares it); the bullet is the first stack argument, the name the second; free function of the executable
{
	AnimType index = AnimTypeClass::From_Name(anim); // RA2: as above
	AnimTypeClass * type = AnimTypes[index]; // RA2: as above
	Coord coord = target->Center_Coord(); // RA2: as above
	AnimClass * animation = new AnimClass(type, coord, 0, 1, ShapeFlags_Type(0x600), 0); // RA2: as above
	animation->Set_Attached_Bullet(bullet); // RA2: as above
	animation->Set_Owner(house); // RA2: as above
	animation->Make_Invisible(); // RA2: sets the byte at +0x17D (IsInvisible)
	return(animation); // RA2: as above
}


/// <summary>
/// Constructs a building without initializing it.
/// This constructor is used by the load process, which is about to overwrite the object
/// wholesale with the data read from the save file.
/// </summary>
BuildingClass::BuildingClass(NoInitClass const & x) :
	BASECLASS(x),
	CountDown(x),
	PlacementDelay(x),
	GateTimer(x),
	BuildingStage(), // RA2: as above
	CashProductionTimer(x) // RA2: the executable leaves the +0x5A0 timer alone here (no store at +0x5A0/+0x5A8)
{
}


/***********************************************************************************************
 * BuildingClass::BuildingClass -- Constructor for buildings.                                  *
 *                                                                                             *
 *    This routine inserts a building into the object tracking system.                         *
 *    It is placed into a limbo state unless a location is provided for                        *
 *    it to unlimbo at.                                                                        *
 *                                                                                             *
 * INPUT:   type  -- The structure type to make this object.                                   *
 *                                                                                             *
 *          house -- The owner of this building.                                               *
 *                                                                                             *
 *          pos   -- The position to unlimbo the building. If -1 is                            *
 *                   specified, then the building remains in a limbo                           *
 *                   state.                                                                    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/21/1994 JLB : Created.                                                                 *
 *   08/07/1995 JLB : Fixed act like value to match expected value.                            *
 *=============================================================================================*/
BuildingClass::BuildingClass(BuildingTypeClass const * type, HouseClass * house) :
	BASECLASS(house),
	Class((BuildingTypeClass* )type),
	Factory(0),
	IsToRebuild(false),
	IsToRepair(false),
	IsAllowedToSell(true),
	IsReadyToCommence(false),
	IsRepairing(false),
	IsWrenchVisible(false),
	IsGoingToBlow(false),
	IsSurvivorless(false),
	IsCharging(false),
	IsCharged(false),
	IsCaptured(false),
	HasOpened(false),
	CountDown(0),
	BState(BSTATE_NONE),
	QueueBState(BSTATE_NONE),
	WhoLastHurtMe(HOUSE_FIRST),
	WhomToRepay(NULL),
	AnimToTrack(NULL),
	PsiWarnAnim(NULL), // RA2: the members the executable clears in the same order
	LastStrength(0),
	PlacementDelay(0),
	RequiresDamageFires(false), // RA2: as above
	IsOn(true),
	IsNominal(false),
	IsOverpowered(false), // RA2: as above
	SupportingPrisms(0), // RA2: as above
	HasExtraPowerBonus(false), // RA2: as above
	HasExtraPowerDrain(false), // RA2: as above
	FiringOccupantIndex(0), // RA2: as above
	WasOnline(false), // RA2: as above
	HasEngineer(false), // RA2: as above
	LastSuperWeaponIndex(-1),
	TurretIndex(-1),
	BuildingLight(NULL),
	GateTimer(0),
	LightSource(NULL),
	LaserFenceFrame(0),
	FirestormWallFrame(0),
	LastRenderRect(RECT_NONE),
	LastRenderCoord(COORD_NONE),
	LastRenderOffset(Point2D(0,0)),
	UnusedBuildingBool1(false),
	IsDamagedAnims(false),
	IsFogged(false),
	HasBuildupData(false),
	IsPoweredOn(true),
	CloakGeneratorState(0),
	CurrentCloakRadius(0),
	TranslucencyLevel(0),
	StorageFilledSlots(0), // RA2: as above
	Brightness(NORMAL_LIGHT),
	UpgradeLevel(0),
	GateFrame(-1), // RA2: as above
	PrismStage(0), // RA2: as above
	DelayBeforeFiring(0) // RA2: as above
{
	Create_ID();

	for (int i = 0; i < BUILDING_UPGRADE_MAX; i++) {
		Upgrades[i] = NULL;
	}

	PrismTargetCoords = Coord(0, 0, 0); // RA2: the prism target starts at the origin

	if (Class != NULL) {
		BarrelPitch.Set_Desired(DirType(Class->StartPitch << 8)); // RA2: the type's start pitch as a facing
	}

	memset(Anims, 0, sizeof(Anims)); // RA2: as above

	for (int index = 0; index < 8; index++) { // RA2: no damage fires yet
		DamageFireAnims[index] = NULL; // RA2: as above
	}

	BuildupSoundController.Init(); // RA2: the audio controllers are initialized
	WorkingSoundController.Init(); // RA2: as above

	BarrelPitch.Set_Desired(DirType(0x4000)); // RA2: the barrel then starts looking straight ahead
	BuildingStage.Set_Stage(0);

	Buildings.Add(this);

	Init();

	FactoryPtrTracker.Add(this);
	AnimPtrTracker.Add(this);
	TargetTracker.Add_Index(Fetch_ID(), this);

	if (Class != NULL) { // RA2: the radio links follow the number of docks
		int count = Class->NumberOfDocks; // RA2: as above
		if (count < 1) { // RA2: as above
			count = 1; // RA2: as above
		}
		SetLinkCount(count); // RA2: as above
	} else {
		SetLinkCount(1); // RA2: as above
	}
}




// RA2: empties an infantry list but keeps its capacity (an inline helper of the destructor 0x439240)
static inline void Reset_Infantry_List(DynamicVectorClass<InfantryClass *> & list) // RA2: as above
{
	int size = list.Length(); // RA2: as above
	list.Clear(); // RA2: as above
	list.Resize(size); // RA2: as above
}

/***********************************************************************************************
 * BuildingClass::~BuildingClass -- Destructor for building type objects.                      *
 *                                                                                             *
 *    This destructor for building objects will put the building in limbo if possible.         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
BuildingClass::~BuildingClass(void)
{
	WorkingSoundController.EndLooping(); // RA2: the two audio controllers are stopped and released first
	BuildupSoundController.EndLooping(); // RA2: as above
	WorkingSoundController.Release(); // RA2: as above
	BuildupSoundController.Release(); // RA2: as above

	if (LightSource != NULL) {
		LightSource->Disable();
		delete LightSource;
		LightSource = NULL;
	}

	Detach_This_From_All(this, true);
	Limbo();
	Buildings.Delete(this);
	End_Anim(BANIM_ALL);
	Class->Free_Buildup_Data();
	Remove_Damage_Fires(); // RA2: the damage fires go too

	if (GameActive && Class != NULL && Class->IsCloakGenerator) {
		CloakGeneratorState = -1; // RA2: Disable_Cloak_Generator inline, the type was checked above
		if (CurrentCloakRadius == 0) { // RA2: as above
			CurrentCloakRadius = Class->CloakRadiusInCells; // RA2: as above
		}
		IsToDisplay = true; // RA2: as above
		CurrentCloakRadius = 1;
		Cloaking_AI(true);
	}

	if (Fetch_Super_Weapon() != SUPER_NONE || Fetch_Super_Weapon2() != SUPER_NONE) { // RA2: only a building with a super weapon
		House->Update_Present_Super_Weapons(); // RA2: as above
	}

	if (GameActive) {
		if (Strength != LastStrength) {
			House->RecalcPower = true;
		}
		if (GameActive && Class) {
			if (House) {
				House->Tracking_Remove(this);
			}
			Limbo();
		}
	}

	delete Factory;
	Factory = NULL;

	AnimPtrTracker.Delete(this);
	FactoryPtrTracker.Delete(this);
	TargetTracker.Remove_Index(Fetch_ID());

	Class = NULL;
	IsActive = false;

	Reset_Infantry_List(Overpowerers); // RA2: both infantry lists are emptied but keep their capacity
	Reset_Infantry_List(Occupants); // RA2: as above
}





/// <summary>
/// Starts the damage fires at the building's DamageFireOffset points (up to eight, ending at the first unset offset),
/// cycling through the DamageFireTypes animations from a random start.
/// </summary>
void BuildingClass::Create_Damage_Fires(void) // RA2: not in TibSun, at 0x4395F0
{
	int type_count = Rule->DamageFireTypes.Count();
	if (type_count != 0) {
		int type_index = Scen->RandomNumber(0, type_count - 1);
		for (int index = 0; index < 8; index++) {
			if (Class->DamageFireOffset[index] == COORD_NONE) return; // RA2: the X and Y of COORD_NONE end the list
			if (DamageFireAnims[index] != NULL) return;
			Coord coord = Coord(TacticalMap->Pixel_To_Lepton(Class->DamageFireOffset[index]), 0);
			coord += Render_Coord();
			AnimClass * fire = new AnimClass(Rule->DamageFireTypes[type_index], coord, 0, 1, ShapeFlags_Type(SHAPE_WIN_REL|SHAPE_CENTER), 0);
			if (fire != NULL) {
				DamageFireAnims[index] = fire;
				fire->ZAdjust = ((Class->DamageFireOffset[index].Y - (Class->Height(false) + Class->Width()) * 15) * 3 >> 1) - 10;
				if (fire->ZAdjust > 0) fire->ZAdjust = 0;
				int stages = fire->Class->Stages;
				if (stages > 0) fire->Set_Stage(Scen->RandomNumber(0, stages - 1));
				type_index++;
				if (type_index >= type_count) type_index = 0;
			}
		}
	}
}


/// <summary>
/// Removes the building's damage fires.
/// </summary>
void BuildingClass::Remove_Damage_Fires(void) // RA2: not in TibSun, at 0x4397C0
{
	for (int index = 0; index < 8; index++) {
		if (DamageFireAnims[index] != NULL) {
			DamageFireAnims[index]->Delete_Me();
			DamageFireAnims[index] = NULL;
		}
	}
}








/***********************************************************************************************
 * BuildingClass::Receive_Message -- Handle an incoming message to the building.               *
 *                                                                                             *
 *    This routine handles an incoming message to the building. Messages regulate the          *
 *    various cooperative ventures between buildings and units. This might include such        *
 *    actions as coordinating the construction yard animation with the actual building's       *
 *    construction animation.                                                                  *
 *                                                                                             *
 * INPUT:   from     -- The originator of the message received.                                *
 *                                                                                             *
 *          message  -- The radio message received.                                            *
 *                                                                                             *
 *          param    -- Reference to an optional parameter that might be used to return        *
 *                      extra information to the message originator.                           *
 *                                                                                             *
 * OUTPUT:  Returns with the response to the message (typically, this is just RADIO_OK).       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/09/1994 JLB : Created.                                                                 *
 *   06/26/1995 JLB : Forces refinery load anim to start immediately.                          *
 *   08/13/1995 JLB : Uses ScenarioInit for special loose "CAN_LOAD" check.                    *
 *=============================================================================================*/
RadioMessageType BuildingClass::Receive_Message(RadioClass * from, RadioMessageType message, int & param)
{
	switch (message) {

		/*
		**	This message is received as a request to attach/load/dock with this building.
		**	Verify that this is allowed and return the appropriate response.
		*/
		case RADIO_CAN_LOAD:
			BASECLASS::Receive_Message(from, message, param);
			if (!House->Is_Ally(from)) return(RADIO_STATIC);
			if (Get_Mission() == MISSION_CONSTRUCTION || Get_Mission() == MISSION_DECONSTRUCTION || BState == BSTATE_CONSTRUCTION || (!ScenarioInit && !HasFreeLink((TechnoClass *)from))) return(RADIO_NEGATIVE); // RA2: Get_Mission, and the building can only be reached when it has a free link
			if (from->Techno_Type_Class()->MZone != MZONE_AMPHIBIOUS && ((Techno_Type_Class()->Naval && !from->Techno_Type_Class()->Naval) || (!Techno_Type_Class()->Naval && from->Techno_Type_Class()->Naval))) return(RADIO_NEGATIVE); // RA2: ships only use naval buildings and land units only land ones
			if (from->Techno_Type_Class()->BalloonHover) return(RADIO_NEGATIVE); // RA2: balloon hovers never dock
			if (!IsOn) return(RADIO_NEGATIVE);
			if (Class->IsCanUnitRepair) {
				if (from->Fetch_RTTI() == RTTI_UNIT || (from->Fetch_RTTI() == RTTI_AIRCRAFT)) { // RA2: Fetch_RTTI
					if (Transmit_Message(RADIO_ON_DEPOT, (TechnoClass *)from) != RADIO_ROGER) { // RA2: as above
						return(RADIO_ROGER);
					}
				}
				return(RADIO_NEGATIVE);
			}
			if ((Class->IsArmory || Class->IsHospital) && from->Fetch_RTTI() == RTTI_INFANTRY) { // RA2: as above
				InfantryClass * infantry = (from == NULL) ? NULL : (from->Fetch_RTTI() == RTTI_INFANTRY ? (InfantryClass *)from : NULL); // RA2: a controlled or mind-controlled infantry may not be healed (the cast tests the RTTI)
				if (infantry->CaptureManager && infantry->CaptureManager->Unit) return(RADIO_NEGATIVE); // RA2: as above
				if (infantry->MindControlledBy) return(RADIO_NEGATIVE); // RA2: as above
				return(Ammo != 0 ? RADIO_ROGER : RADIO_NEGATIVE); // RA2: no mission test
			}
			if (Class->IsHelipad) {
				if (from->What_Am_I() == RTTI_AIRCRAFT) {
					return(RADIO_ROGER);
				}
				return(RADIO_NEGATIVE);
			}
			if (Class->IsDockUnload && from->Fetch_RTTI() == RTTI_UNIT && ((UnitClass *)from)->Class->IsToHarvest && (ScenarioInit || !Cargo.Is_Something_Attached())) { // RA2: as above
				return(RADIO_ROGER);
			}
			if (Class->IsWeeder && from->Fetch_RTTI() == RTTI_UNIT && ((UnitClass *)from)->Class->IsToVeinHarvest && (ScenarioInit || !Cargo.Is_Something_Attached())) { // RA2: as above
				return(RADIO_ROGER);
			}
			return(RADIO_STATIC);

		/*
		**	This message is received when the object has attached itself to this
		**	building.
		*/
		case RADIO_IM_IN:
			if (Get_Mission() == MISSION_DECONSTRUCTION) { // RA2: as above
				return(RADIO_NEGATIVE);
			}
			if (Class->IsCanUnitRepair || Class->IsCanUnitReload || Class->IsHospital || Class->IsArmory) {
				IsReadyToCommence = true;
				QueueMission(MISSION_REPAIR, false); // RA2: QueueMission
				from->QueueMission(MISSION_SLEEP, false); // RA2: as above
				return(RADIO_ROGER);
			}
			if (Class->IsDockUnload) { // RA2: no IsWeeder
				from->QueueMission(MISSION_UNLOAD, false); // RA2: as above
				return(RADIO_ROGER);
			}
			break;

		/*
		**	Docking maneuver maintenance message. See if new order should be given to the
		**	unit trying to dock.
		*/
		case RADIO_DOCKING: {
			BASECLASS::Receive_Message(from, message, param);

			if (!IsOn) {
				return(RADIO_NEGATIVE);
			}

			if (Class->IsCanUnitRepair && ContainsLink((TechnoClass *)from) && Transmit_Message(RADIO_NEED_REPAIR, (TechnoClass *)from) == RADIO_NEGATIVE) { // RA2: as above
				return(RADIO_NEGATIVE); // RA2: as above
			}

			if (!Class->IsHospital && !Class->IsArmory) { // RA2: as above
				if (!ContainsLink((TechnoClass *)from) && HasFreeLink((TechnoClass *)from)) { // RA2: as above
					Transmit_Message(RADIO_HELLO, (TechnoClass *)from); // RA2: as above
					ContainsLink((TechnoClass *)from); // RA2: as above
				}

				bool needs_to_move = false; // RA2: as above

				if (ContainsLink((TechnoClass *)from) && (Class->IsDockUnload || Class->IsWeeder)) { // RA2: as above
					FootClass * foot = (from == NULL) ? NULL : ((from->AbstractFlags & 4) ? (FootClass *)from : NULL); // RA2: the docker, not the building's contact (the cast tests bit 2 of the abstract flags)
					CellClass * docking_cell = &Map[Docking_Coord(foot)]; // RA2: Docking_Coord takes the docker
					AbstractClass * navcom = foot->NavCom; // RA2: as above
					if (navcom != NULL && docking_cell != navcom) {
						needs_to_move = true;
					}
				}

				if (Radio_Link(0) != NULL && Class->IsCanUnitRepair && Distance_To(Radio_Link(0)) > CELL_LEPTON / 2) { // RA2: as above
					needs_to_move = true; // RA2: as above
				}

				if (Transmit_Message(RADIO_NEED_TO_MOVE, (TechnoClass *)from) == RADIO_ROGER || needs_to_move) { // RA2: as above
					param = (int)this; // RA2: as above
					if (Class->IsDockUnload || Class->IsWeeder) { // RA2: as above
						param = (int)&Map[Get_Cell() + Cell(3, 1)]; // RA2: the docking pad is at (3, 1)

						/*
						**	Tell the harvester to move to the docking pad of the building.
						*/
						if (Transmit_Message(RADIO_MOVE_HERE, param, from) == RADIO_YEA_NOW_WHAT) { // RA2: as above

							/*
							**	Since the harvester is already there, tell it to begin the backup
							**	procedure now. If it can't, then tell it to get outta here.
							*/
							Transmit_Message(RADIO_TETHER, (TechnoClass *)from); // RA2: as above
							if (Transmit_Message(RADIO_BACKUP_NOW, (TechnoClass *)from) != RADIO_ROGER) { // RA2: as above
								from->Scatter(COORD_NONE, true, true); // RA2: as above
							}
						}
					} else if (Class->IsHelipad) { // RA2: as above
						param = (int)this; // RA2: as above
						if (Transmit_Message(RADIO_MOVE_HERE, param, from) == RADIO_YEA_NOW_WHAT) { // RA2: as above
							Transmit_Message(RADIO_TETHER); // RA2: as above
						}
					}
				}
				return(RADIO_ROGER); // RA2: as above
			}

			if (!HasFreeLink((TechnoClass *)from)) { // RA2: as above
				for (int index = 0; index < RadioLinks.Length(); index++) { // RA2: as above
					TechnoClass * contact = Radio_Link(index); // RA2: as above
					if (Transmit_Message(RADIO_NEED_REPAIR, contact) == RADIO_NEGATIVE) { // RA2: as above
						Transmit_Message(RADIO_RUN_AWAY, contact); // RA2: as above
					}
				}
				if (!HasFreeLink((TechnoClass *)from)) { // RA2: as above
					return(RADIO_NEGATIVE); // RA2: as above
				}
			} else {
				param = (int)&Map[Get_Coord()]; // RA2: as above
				Transmit_Message(RADIO_MOVE_HERE, param, from); // RA2: as above
			}
			return(RADIO_ROGER);
		}

		/*
		**	If a transport or harvester is requesting permission to head toward, dock
		**	and load/unload, check to make sure that this is allowed given the current
		**	state of the building.
		*/
		case RADIO_ARE_REFINERY:
			if (!Cargo.Is_Something_Attached() && HasFreeLink((TechnoClass *)from) && !IsInLimbo && House == from->Owner_HouseClass()) { // RA2: as above
				if (Class->IsRefinery || Class->IsCanUnitRepair || Class->IsWeeder) { // RA2: as above
					return(RADIO_ROGER); // RA2: as above
				}
			}
			return(RADIO_NEGATIVE); // RA2: as above

		/*
		**	Someone is telling us that it is starting construction. This should only
		**	occur if this is a construction yard and a building was just placed on
		**	the map.
		*/
		case RADIO_BUILDING:
			QueueMission(MISSION_REPAIR, false); // RA2: QueueMission
			BASECLASS::Receive_Message(from, message, param);
			return(RADIO_ROGER);

		/*
		**	Someone is telling us that they have finished construction. This should
		**	only occur if this is a construction yard and the building that was being
		**	constructed has finished. In this case, stop the construction yard
		**	animation.
		*/
		case RADIO_COMPLETE:
			if (Get_Mission() != MISSION_DECONSTRUCTION) { // RA2: as above
				QueueMission(MISSION_GUARD, false); // RA2: QueueMission
				if (Class->IsConstructionYard) {
					End_Anim(BANIM_PRE_PRODUCTION);
					End_Anim(BANIM_IDLE); // RA2: the idle animation stops too
					Begin_Anim(BANIM_PRODUCTION, HealthRatio <= Rule->ConditionYellow, false, 0); // RA2: as above
				}
			}
			BASECLASS::Receive_Message(from, message, param);
			return(RADIO_ROGER);

		/*
		**	This message may occur unexpectedly if the unit in contact with this
		**	building is suddenly destroyed. Handle any cleanup necessary. For example,
		**	a construction yard should stop its construction animation in this case.
		*/
		case RADIO_OVER_OUT:
			Begin_Mode(BSTATE_IDLE);
			BASECLASS::Receive_Message(from, message, param);
			return(RADIO_ROGER);

		/*
		**	This message is received when an object has completely left
		**	building. Sometimes special cleanup action is required when
		**	this event occurs.
		*/
		case RADIO_UNLOADED:
			if (Class->IsCanUnitRepair && Distance_To(from) < 3 * CELL_LEPTON / 2) { // RA2: as above
				return(RADIO_ROGER); // RA2: as above
			}
			BASECLASS::Receive_Message(from, message, param);
			if (!Class->IsWeaponsFactory && !Class->IsCanUnitRepair) return(RADIO_ROGER); // RA2: as above
			return(RADIO_RUN_AWAY); // RA2: as above

		case RADIO_REDRAW:
			if (Class->IsWeaponsFactory) {
				return(RADIO_ROGER);
			}
			break;

		default:
			break;
	}

	/*
	**	Pass along the message to the default message handler in the radio itself.
	*/
	return(BASECLASS::Receive_Message(from, message, param));
}



/// <summary>
/// Renders the building if it needs to be displayed.
/// This routine will determine whether the building falls inside the region being
/// redrawn and, if so, hand it to the drawing code. It is the entry point the layer
/// render uses; the attached decorations can be requested on their own so that they may
/// be laid down in a later pass than the structure itself.
/// </summary>
/// <param name="rect">The region being redrawn. It is clipped to the tactical view
/// before use and the clipped result is handed back to the caller.</param>
/// <param name="forced">Is this redraw forced by outside circumstances?</param>
/// <param name="extras_only">Should only the attached extras be drawn?</param>
/// <returns>bool; Was anything drawn?</returns>
bool BuildingClass::Render(Rect & rect, bool forced, bool extras_only) const
{
	if (Debug_Map || !MainWindow || ((forced || IsToDisplay) && IsDown && !IsInLimbo)) {
		IsToDisplay = false;
		rect = Intersect(rect, TacticalRect);

		if (rect.Is_Overlapping(((BuildingClass *)this)->Get_Render_Rect() + TacticalRect.TopLeft)) {
			Point2D point;
			TacticalMap->Coord_To_Pixel(Render_Coord(), point);
			if (rect.X > TacticalRect.X) {
				point.X += TacticalRect.X - rect.X;
			}

			if (rect.Y > TacticalRect.Y) {
				point.Y += TacticalRect.Y - rect.Y;
			}

			if (extras_only) {
				if (!IsFogged) {
					((BuildingClass *)this)->Draw_Extras(point, rect);
				}
			} else {
				Draw_It(point, rect);
			}
			return(true);
		}
	}
	return(false);
}


/// <summary>
/// Draws the building without laying down a depth of its own.
/// This is the low level graphic routine used when the structure must be placed over
/// what has already been rendered. The shape is drawn in two bands -- the upper part
/// standing upright and the remainder lying flat on the ground -- so that it sorts
/// against the terrain the way the normal render does. A building in the middle of
/// opening up will not be drawn at all.
/// </summary>
/// <param name="xdrawpoint">The pixel location to draw the building at.</param>
/// <param name="xcliprect">The clipping rectangle to draw within.</param>
void BuildingClass::Editor_Draw_It(Point2D const & xdrawpoint, Rect const & xcliprect) const
{
	Cell cell = PositionCell;

	ShapeSet const * shapefile = (ShapeSet const *)Get_Image_Data();
	if (shapefile == NULL) return;

	if (Class->IsInvisibleInGame) return;

	if (Mission != MISSION_OPEN || Door.Is_Door_Closed()) {

		if (Mission == MISSION_UNLOAD) {
			if (Class->DeployingAnim != NULL) {
				TechnoClass * object = Factory->Get_Object(); // RA2: Editor_Draw_It at 0x43A380 matches only so
				if (object != NULL && object->Techno_Type_Class()->JumpJet) { // RA2: as above
					shapefile = Class->RoofDeployingAnim; // RA2: as above
				} else {
					shapefile = Class->DeployingAnim; // RA2: as above
				}
			}
		}

		Point2D drawpoint = xdrawpoint;
		int shapeheight = shapefile->Get_Height();
		int height = drawpoint.Y - shapeheight / 2;

		Rect cliprect = xcliprect;
		if (cliprect.Height > height) {
			cliprect.Height = height;
		}

		if (cliprect.Height > 0) {
			Techno_Draw_Object(shapefile, Shape_Number(), drawpoint, cliprect, DIR_N, 256, -2 - TacticalMap->Z_Lepton_To_Pixel(Height), ZGRAD_90DEG, false, Map[cell].Brightness, 0, 0, Point2D(0, 0), SHAPE_NORMAL); // RA2: one argument fewer
		}

		cliprect = xcliprect;
		Point2D xy = drawpoint; // RA2: as above
		cliprect.Y += height;
		cliprect.Height = shapeheight;
		xy.Y -= height; // RA2: Editor_Draw_It at 0x43A380 has no Point2D
		if (cliprect.Y + cliprect.Height > xcliprect.Y + xcliprect.Height) {
			cliprect.Height = xcliprect.Y + xcliprect.Height - cliprect.Y;
		}
		if (cliprect.Y < xcliprect.Y) {
			cliprect.Height += cliprect.Y - xcliprect.Y;
			xy.Y += cliprect.Y - xcliprect.Y;
			cliprect.Y = xcliprect.Y;
		}

		if (cliprect.Height > 0) {
			Techno_Draw_Object(shapefile, Shape_Number(), xy, cliprect, DIR_N, 256, -TacticalMap->Z_Lepton_To_Pixel(Height), ZGRAD_GROUND, false, Map[cell].Brightness, 0, 0, Point2D(0, 0), SHAPE_NORMAL); // RA2: one argument fewer
		}
	}
}


/***********************************************************************************************
 * BuildingClass::Draw_It -- Displays the building at the location specified.                  *
 *                                                                                             *
 *    This is the low level graphic routine that displays the building at the location         *
 *    specified.                                                                               *
 *                                                                                             *
 * INPUT:   x,y   -- The coordinate to draw the building at.                                   *
 *                                                                                             *
 *          window   -- The clipping window to use.                                            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/20/1994 JLB : Created.                                                                 *
 *   06/27/1994 JLB : Takes a clipping window parameter.                                       *
 *   07/06/1995 JLB : Handles damaged silos correctly.                                         *
 *=============================================================================================*/
void BuildingClass::Draw_It(Point2D const & xdrawpoint, Rect const & xcliprect) const
{
	Cell cell = Get_Cell(); // RA2: RA2's form

	/*
	**	The shape file to use for rendering depends on whether the building
	**	is undergoing construction or not.
	*/
	ShapeSet const * shape = (ShapeSet const *)Get_Image_Data(); // RA2: as above
	if (shape == NULL) return; // RA2: as above

	if (this != NULL && Fetch_RTTI() == RTTI_BUILDING && BState == BSTATE_CONSTRUCTION && Get_Mission() == MISSION_DECONSTRUCTION) { // RA2: as above
		for (int index = 0; index < 18; index++) { // RA2: as above
			if (Anims[index]) { // RA2: as above
				Anims[index]->Make_Invisible(); // RA2: as above
			}
		}
	}

	if (Class->IsInvisibleInGame) return;

	Point2D depth_point(198, 446); // RA2: as above
	int z_adjust = Class->NormalZAdjust; // RA2: as above
	bool air_product = false; // RA2: as above
	if (Get_Mission() == MISSION_UNLOAD) { // RA2: as above
		TechnoClass * product = Radio_Link(0); // RA2: as above
		if (product && (product->Techno_Type_Class()->JumpJet || product->Techno_Type_Class()->BalloonHover)) { // RA2: as above
			air_product = true; // RA2: as above
		}
	}

	if (Get_Mission() == MISSION_OPEN && (Door.Is_Count_Up_Active() || Door.Is_Count_Down_Active() || Door.Is_Count_Up_Complete())) { // RA2: as above
		int stage = (int)(Door.Percent_Complete() * Class->GateStages); // RA2: as above
		if (Door.Is_Count_Down_Active()) { // RA2: as above
			stage = Class->GateStages - stage; // RA2: as above
		}
		if (Door.Is_Count_Down_Complete()) { // RA2: as above
			stage = 0; // RA2: as above
		}
		if (Door.Is_Count_Up_Complete()) { // RA2: as above
			stage = Class->GateStages - 1; // RA2: as above
		}
		if (stage >= Class->GateStages) { // RA2: as above
			stage = Class->GateStages - 1; // RA2: as above
		}
		if (stage < 0) { // RA2: as above
			stage = 0; // RA2: as above
		}

		ShapeSet const * gate_shape = (ShapeSet const *)Get_Image_Data(); // RA2: as above
		stage += (HealthRatio <= Rule->ConditionYellow ? Class->GateStages + 1 : 0); // RA2: as above
		Techno_Draw_Object(gate_shape, stage, xdrawpoint, xcliprect, DIR_N, 256, // RA2: as above
			z_adjust - TacticalMap->Z_Lepton_To_Pixel(Get_Height()), ZGRAD_90DEG, 1, Map[cell].Brightness, 0, 0, Point2D(0, 0), SHAPE_NORMAL); // RA2: as above
		return;
	}

	if (Get_Mission() == MISSION_UNLOAD) { // RA2: as above
		if ((air_product && Class->RoofDeployingAnim) || (!air_product && Class->DeployingAnim)) { // RA2: as above
			shape = (ShapeSet const *)(air_product ? Class->RoofDeployingAnim : Class->DeployingAnim); // RA2: as above
			z_adjust = -20; // RA2: as above
		}
	}

	int mission = Get_Mission(); // RA2: as above
	if (mission != MISSION_CONSTRUCTION && mission != MISSION_DECONSTRUCTION) { // RA2: as above
		depth_point += Class->ZShapePointMove; // RA2: as above
	}

	Point2D footprint(Class->Width() * 256 - 256, Class->Height() * 256 - 256); // RA2: as above
	depth_point -= TacticalMap->Coord_To_Pixel_Absolute(footprint); // RA2: as above

	ShapeSet const * depth_shape = (ShapeSet const *)BuildingTypeClass::BuildingZShape; // RA2: as above
	if (Class->Width() >= 8) { // RA2: as above
		depth_shape = NULL; // RA2: as above
	}

	if (xcliprect.Height > 0) { // RA2: as above
		int frame = Shape_Number() < shape->Get_Count() / 2 ? Shape_Number() : shape->Get_Count() / 2; // RA2: as above
		Techno_Draw_Object(shape, frame, xdrawpoint, xcliprect, DIR_N, 256, // RA2: as above
			z_adjust - TacticalMap->Z_Lepton_To_Pixel(Get_Height()), ZGRAD_90DEG, 1, Map[cell].Brightness + Class->ExtraLight, depth_shape, 0, depth_point, SHAPE_NORMAL); // RA2: as above
	}

	/*
	**	Draw the weapon factory custom overlay graphic.
	*/
	if (Class->BibShape && BState != BSTATE_CONSTRUCTION) {
		Techno_Draw_Object(Class->BibShape, Shape_Number(), xdrawpoint, xcliprect, DIR_N, 256, // RA2: as above
			-1 - TacticalMap->Z_Lepton_To_Pixel(Get_Height()), ZGRAD_GROUND, 1, Map[cell].Brightness + Class->ExtraLight, 0, 0, Point2D(0, 0), SHAPE_NORMAL); // RA2: as above
	}

	if (Get_Mission() == MISSION_UNLOAD && ((air_product && Class->UnderRoofDoorAnim != NULL) || (!air_product && Class->UnderDoorAnim != NULL))) { // RA2: as above
		if (air_product) { // RA2: as above
			Techno_Draw_Object(Class->UnderRoofDoorAnim, HealthRatio <= Rule->ConditionYellow, xdrawpoint, xcliprect, DIR_N, 256, // RA2: as above
				-TacticalMap->Z_Lepton_To_Pixel(Get_Height()), ZGRAD_GROUND, 1, Map[cell].Brightness + Class->ExtraLight, 0, 0, Point2D(0, 0), SHAPE_NORMAL); // RA2: as above
		} else {
			Techno_Draw_Object(Class->UnderDoorAnim, HealthRatio <= Rule->ConditionYellow, xdrawpoint, xcliprect, DIR_N, 256, // RA2: as above
				-TacticalMap->Z_Lepton_To_Pixel(Get_Height()), ZGRAD_GROUND, 1, Map[cell].Brightness + Class->ExtraLight, 0, 0, Point2D(0, 0), SHAPE_NORMAL); // RA2: as above
		}
	}
}



/// <summary>
/// Draws the extra imagery that belongs on top of this building.
/// This is the second pass of the render, used for everything that must appear over the
/// structure itself -- the vehicle passing out through the factory door, the door
/// animation, and the voxel turret and barrel. It is skipped for a fogged building.
/// </summary>
/// <param name="xy">Pixel position to draw the building at.</param>
/// <param name="rect">Clipping rectangle to draw within.</param>
#ifdef RA2_NON_MATCHING // not exact: 61 bytes differ (adopted text)
void BuildingClass::Draw_Extras(Point2D & xy, Rect & rect)
{
	Cell cell = Get_Cell(); // RA2: Get_Cell

	/*
	 * If a vehicle is currently exiting through the factory door, draw it through
	 * the door opening.
	 */
	if (Get_Mission() == MISSION_UNLOAD // RA2: Get_Mission
		&& (Door.Is_Door_Opening() || Door.Is_Door_Open() || Door.Is_Door_Closed() || Door.Is_Door_Closing())
		&& IsTethered) {

		for (int index = 0; index < RadioLinks.Length(); index++) { // RA2: every radio link, not just the first
			TechnoClass * techno = ((BuildingClass const *)this)->Radio_Link(index); // RA2: the executable calls the const Radio_Link overload here
			if (techno && !techno->IsInLimbo && techno->Fetch_RTTI() != RTTI_BUILDING) { // RA2: Fetch_RTTI

				Coord coord = techno->Destination_Coord(NULL); // RA2: Destination_Coord takes the docker
				coord.Z = techno->Get_Coord().Z; // RA2: Get_Coord

				if (!MainWindow || Debug_Map || !Scen->Special.IsFogOfWar || (!Map.Is_Fogged(techno->Get_Coord()) && !Map.Is_Fogged(coord))) { // RA2: as above
					Point2D point; // RA2: as above
					TacticalMap->Coord_To_Pixel(techno->Render_Coord(), point); // RA2: as above
					techno->Draw_It(point, rect); // RA2: as above
				}
			}
		}
	}

	/*
	 * Draw the factory door animation.
	 */
	if (Get_Mission() == MISSION_UNLOAD && Class->DoorAnim != NULL) { // RA2: as above

		if (Door.Is_Door_Closed() || Door.Is_Door_Opening() || Door.Is_Door_Closing() || Door.Is_Door_Open()) {

			int shapenum = (int)(Door.Percent_Complete() * Class->DoorStages);
			if (Door.Is_Door_Closing()) {
				shapenum = Class->DoorStages - shapenum;
			}
			if (Door.Is_Door_Closed()) {
				shapenum = 0;
			}
			if (shapenum >= Class->DoorStages) {
				shapenum = Class->DoorStages - 1;
			}
			if (shapenum < 0) {
				shapenum = 0;
			}

			if (HealthRatio <= Rule->ConditionYellow && Class->IsDamagedDoor) {
				shapenum += Class->DoorStages;
			}

			int zadjust = -5 - Tactical::Z_Lepton_To_Pixel(Get_Height()); // RA2: Get_Height
			Techno_Draw_Object(Class->DoorAnim, shapenum, xy, rect, DIR_N, 256, zadjust, ZGRAD_GROUND, false, Map[cell].Brightness);

			if (Door.Is_Door_Closing() && shapenum == 0) {
				IsToDisplay = true;
			}
		}
	}

	if (Class->IsTurretAnimAVoxel) {

		bool construction = false;
		if ((CurrentMission == MISSION_CONSTRUCTION || MissionQueue == MISSION_CONSTRUCTION) && Fetch_Stage() < Class->Anims[BSTATE_CONSTRUCTION].Start + Class->Anims[BSTATE_CONSTRUCTION].Count - 1) { // RA2: Start first
			construction = true;
		}

		if (((CurrentMission != MISSION_DECONSTRUCTION || Fetch_Stage() <= 0) && !construction) || Class->IsArtillary) { // RA2: as above

			int key; // RA2: as above
			if (UseVoxelCache) { // RA2: the key is the facing in 32 steps
				key = PrimaryFacing.Current().As_Dir32(); // RA2: as above
			} else {
				key = -1; // RA2: as above
			}

			Matrix3D matrix;
			matrix.Make_Identity();

			if (Class->AuxVoxel.VoxLib != NULL) {

				matrix.Rotate_Z(PrimaryFacing.Current().As_Radian32());
				matrix.Translate_X(Class->TurretOffset / 8);

				Matrix3D barrel_matrix = matrix; // RA2: no IsInRecoilState (not in RA2's layout)
				Vector3 origin = Vector3(matrix.Get_X_Translation(), matrix.Get_Y_Translation(), matrix.Get_Z_Translation()); // RA2: as above
				barrel_matrix.Translate(-origin); // RA2: as above

				/*
				**	A recoiling turret or barrel moves "backward" and takes the voxels out of the cache.
				*/
				if (TurretRecoil.State != 0) { // RA2: TurretRecoil
					matrix.Translate_X(-TurretRecoil.TravelSoFar); // RA2: as above
					key = -1; // RA2: as above
				}
				if (BarrelRecoil.State != 0) { // RA2: BarrelRecoil
					barrel_matrix.Translate_X(-BarrelRecoil.TravelSoFar); // RA2: as above
					key = -1; // RA2: as above
				}

				barrel_matrix.Rotate_Y(-(BarrelPitch.Current().As_Radian32()));
				barrel_matrix.Translate(origin); // RA2: as above

				Point2D drawpoint = xy + Point2D(Class->AnimData[BANIM_TURRET].Location.X, Class->AnimData[BANIM_TURRET].Location.Y); // RA2: the offset is rebuilt from its two components

				VoxelDataStruct * voxl;
				bool draw_barrel;
				if (PrimaryFacing.Current().As_Dir4() <= 0 || PrimaryFacing.Current().As_Dir4() >= 3) {
					draw_barrel = false;
					voxl = &Class->AuxVoxel2;
					if (voxl->VoxLib != NULL && voxl->MotLib != NULL) {
						Draw_Voxel(Class->AuxVoxel2, 0, key, &Class->AuxVoxel2Index, rect, drawpoint, Get_Isometric_View_Matrix() * barrel_matrix, Map[cell].Brightness, SHAPE_NORMAL); // RA2: as above
					}
				} else {
					draw_barrel = true;
				}

				Draw_Voxel(Class->AuxVoxel, 0, key, &Class->AuxVoxelIndex, rect, drawpoint, Get_Isometric_View_Matrix() * matrix, Map[cell].Brightness, SHAPE_NORMAL); // RA2: as above

				if (draw_barrel) {
					voxl = &Class->AuxVoxel2;
					if (voxl->VoxLib != NULL && voxl->MotLib != NULL) {
						Draw_Voxel(Class->AuxVoxel2, 0, key, &Class->AuxVoxel2Index, rect, drawpoint, Get_Isometric_View_Matrix() * barrel_matrix, Map[cell].Brightness, SHAPE_NORMAL); // RA2: as above
					}
				}

			} else if (Class->AuxVoxel2.VoxLib != NULL && Class->AuxVoxel2.MotLib != NULL) {

				Vector3 origin = Vector3(matrix.Get_X_Translation(), matrix.Get_Y_Translation(), matrix.Get_Z_Translation()); // RA2: as above
				matrix.Translate(-origin); // RA2: as above

				matrix.Rotate_Z(PrimaryFacing.Current().As_Radian32());
				matrix.Rotate_Y(-(BarrelPitch.Current().As_Radian32()));
				matrix.Translate(origin); // RA2: as above

				Point2D drawpoint = xy + Point2D(Class->AnimData[BANIM_TURRET].Location.X, Class->AnimData[BANIM_TURRET].Location.Y); // RA2: as above
				Draw_Voxel(Class->AuxVoxel2, 0, key, &Class->AuxVoxel2Index, rect, drawpoint, Get_Isometric_View_Matrix() * matrix, Map[cell].Brightness, SHAPE_NORMAL); // RA2: as above
			}
		}

	} else if (Class->IsBarrelAnimAVoxel) {

		bool construction = false;
		if ((CurrentMission == MISSION_CONSTRUCTION || MissionQueue == MISSION_CONSTRUCTION) && Fetch_Stage() < Class->Anims[BSTATE_CONSTRUCTION].Start + Class->Anims[BSTATE_CONSTRUCTION].Count - 1) { // RA2: as above
			construction = true;
		}

		if ((CurrentMission != MISSION_DECONSTRUCTION || Fetch_Stage() <= 0) && !construction) {

			static int _dir_adjust = 28;
			static bool _make_visible = true;

			Matrix3D matrix = Get_Barrel_Matrix();

			int dir = (_dir_adjust + PrimaryFacing.Current().As_Dir32()) % (FACING_COUNT * 4);
			bool in_front = (dir <= 16);
			AnimClass * anim = Anims[BANIM_TURRET];

			if (in_front && anim != NULL) { // RA2: as above
				if (_make_visible) { // RA2: as above
					anim->Make_Visible(); // RA2: as above
				}
				anim->Render(TacticalRect, true, false); // RA2: as above
				anim->Make_Invisible(); // RA2: as above
			}

			Draw_Voxel(Class->AuxVoxel2, 0, -1, &Class->VoxelIndex, rect, xy + Point2D(Class->AnimData[BANIM_TURRET].Location.X, Class->AnimData[BANIM_TURRET].Location.Y), Get_Isometric_View_Matrix() * matrix, Map[cell].Brightness, SHAPE_NORMAL); // RA2: as above

			if (!in_front && anim != NULL) { // RA2: as above
				if (_make_visible) { // RA2: as above
					anim->Make_Visible(); // RA2: as above
				}
				anim->Render(TacticalRect, true, false); // RA2: as above
				anim->Make_Invisible(); // RA2: as above
			}
		}
	}
}
#endif



/// <summary>
/// Draws the informational overlays that sit on top of the building.
/// This routine handles the selection text and, for a factory the player has managed to
/// infiltrate, the cameo of whatever is currently being produced there.
/// </summary>
/// <param name="point">The pixel location to center the overlays about.</param>
/// <param name="cliprect">The clipping rectangle to draw within.</param>
void BuildingClass::Draw_Overlays(Point2D const & point, Rect const & cliprect) const // RA2: at 0x43B660; no repair or power-off shapes
{
	Cell cell = PositionCell;

	if (IsSelected && (House->Is_Ally(PlayerPtr) || SpiedBy & (1<<(PlayerPtr->Class->House)))) {
		Draw_Text_Overlay(point + Point2D(-10, 10), point, cliprect);
	}

	/*
	**	If this is a factory that we're spying on, show what it's producing
	*/
	if (SpiedBy & (1<<(PlayerPtr->Class->House)) && IsSelected) {

		/*
		**	Fetch the factory that is associate with this building. For computer controlled buildings, the
		**	factory pointer is integral to the building itself. For human controlled buildings, the factory
		**	pointer is part of the house structure and must be retrieved from there.
		*/
		FactoryClass * factory = NULL;
		if (House->Is_Human_Player()) {
			factory = House->Fetch_Factory(Class->ToBuild, Class->Naval, 0); // RA2: naval factories apart
		} else {
			factory = Factory;
		}

		/*
		**	If there is a factory associated with this building, then fetch any attached
		**	object under production and display its cameo image over the top of this building.
		*/
		if (factory != NULL) {
			TechnoClass * obj = factory->Get_Object();
			if (obj != NULL) {
				Draw_Shape(*LogicalSurface, *NormalDrawer, (ShapeSet const *)obj->TClass->Get_Cameo_Data(), 0, point, cliprect, ShapeFlags_Type(SHAPE_CENTER|SHAPE_WIN_REL|SHAPE_ALPHA), NULL);
			}
		}
	}
}





/// <summary>
/// Fetches the depth adjustment to draw this building with.
/// This routine is used by the render code to nudge the structure forward or back within
/// the depth buffer, so that a voxel turret or barrel sorts correctly against the
/// building it is mounted on.
/// </summary>
/// <returns>Returns with the depth bias to apply, expressed in pixels.</returns>
int BuildingClass::Get_Z_Adjust(void) const
{
	int pixel = -Tactical::Z_Lepton_To_Pixel(Height); // RA2: Get_Z_Adjust at 0x43B7A0 has no static int _barrel_z_adj = -21;

	if (Class->IsTurretAnimAVoxel) {
		return(Class->AnimData[BANIM_TURRET].ZAdjust + pixel);
	}
	return(pixel); // RA2: Get_Z_Adjust at 0x43B7A0 has no Class, IsBarrelAnimAVoxel
}


/// <summary>
/// Determines the pitch the barrel must adopt to hit the target.
/// A laser weapon merely elevates to point straight at the aim point, while the EM pulse
/// cannon has to solve for a ballistic arc that will drop its shot on the target. Any
/// other building leaves the decision to the normal barrel handling.
/// </summary>
/// <returns>Returns with the pitch to elevate the barrel to.</returns>
DirType BuildingClass::Barrel_Pitch(AbstractClass * target) const
{
	if (PrimaryWeapon->IsLaser) { // RA2: Barrel_Pitch at 0x43B7E0 has no DirType
		Techno_Type_Class(); // RA2: Barrel_Pitch at 0x43B7E0 matches only so
		if (target != NULL) {
			Coord predicted = Predict_Target_Coord();
			WeaponDataStruct const * weap = Get_Class_Weapon_Data(); // RA2: as above
			int z = weap->FireFLH.Z;
			Coord coord = Render_Coord() + Coord(0, 0, z); // RA2: as above
			Coord pt(TacticalMap->Pixel_To_Lepton(Point2D(Class->AnimData[BANIM_TURRET].Location.X, // RA2: as above
				Class->AnimData[BANIM_TURRET].Location.Y)), 0); // RA2: as above
			coord += pt;
			Coord fire = predicted - coord;

			if (fire.Z == 0) {
				return(DIR_E);
			}

			double dr;
			int d;

			d = fire.X * fire.X + fire.Y * fire.Y;
			dr = fastmath::sqrt(d);
			d = fire.Length();
			dr = dr / d;
			if (dr > 1) {
				dr = 1;
			}

			DirType dir = (int)((fastmath::asin(dr) * 16384.0) * M_2_PI);
			if (fire.Z < 0) {
				dir = DirType(DIR_S) - dir;
			}

			return(dir);
		} else {
			return(BarrelPitch.Current());
		}
	} else if (Class->IsEMPulseCannon) {
		Point3D ground_target(target->Center_Coord(), Map.Get_Height_GL(target->Center_Coord())); // RA2: as above
		Coord fcoord = Fire_Coord(0); // RA2: no offset argument
		::Direction(fcoord, ground_target); // RA2: as above
		int max_speed = Calculate_Projectile_Speed(Distance(target), Rule->Gravity);

		Coord ucoord = Turret_Coord();
		Coord disp = target->Center_Coord() - ucoord;
		int distance2 = disp.X * disp.X + disp.Y * disp.Y;

		DirType pitch;
		bool valid = Calculate_Projectile_Pitch(Should_Use_High_Arc(0), max_speed, fastmath::sqrt(distance2), disp.Z, Rule->Gravity, pitch);
		if (!valid) {
			valid = Calculate_Projectile_Pitch(Should_Use_High_Arc(0), max_speed * 10 / 8, fastmath::sqrt(distance2), disp.Z, Rule->Gravity, pitch);
		}
		return(pitch);
	} else {
		return(BASECLASS::Barrel_Pitch(target));
	}
}


/// <summary>
/// Determines the direction the turret must face to aim at the target.
/// The angle is measured from wherever the turret actually sits rather than from the
/// center of the building, so that structures with an offset turret still track their
/// target properly.
/// </summary>
/// <returns>Returns with the facing that points at the target.</returns>
DirType BuildingClass::Aim_Direction(AbstractClass * target) const
{
	Coord coord = COORD_NONE;

	if (Class->PrimaryFirePixelOffset != Point2D(0xFFFF, 0xFFFF)) {
		coord = Coord(TacticalMap->Pixel_To_Lepton(Class->PrimaryFirePixelOffset), 0);
	} else {
		Point2D pt(Class->AnimData[BANIM_TURRET].Location.X, Class->AnimData[BANIM_TURRET].Location.Y);
		if (pt != Point2D(0, 0)) {
			coord = Coord(TacticalMap->Pixel_To_Lepton(pt), 0);
		}
	}
	coord = coord + Center_Coord();
	return(::Direction(coord, target->Center_Coord()));
}


/// <summary>
/// Sets the turret animation to the frame that matches the building.
/// A charging weapon drives its turret from the charge sequence; anything else shows the
/// frame appropriate to where the turret is currently pointed.
/// </summary>
/// <returns>Returns with the animation stage selected, or zero if there was no turret to
/// update.</returns>
int BuildingClass::Set_Turret_Frame(void)
{
	int stage = 0;
	int reset_step = false;
	if (Is_Turret_Equipped()) { // RA2: no charge animation stage
		if (Anims[BANIM_TURRET] != NULL) {
			stage = BASECLASS::BodyShape[PrimaryFacing.Current().As_Dir32()]; // RA2: as above
			reset_step = true; // RA2: as above
			AnimClass * anim = Anims[BANIM_TURRET];
			if (anim != NULL) {
				anim->Set_Stage(stage);
				if (reset_step) {
					anim->Set_Step(0);
				}
			}
		}
	}
	return(stage);
}



/// <summary>
/// Sets which turret animation the building displays.
/// Buildings whose turret is drawn as an animation rather than a voxel carry a set of
/// lettered variants, one per facing group. This routine takes down the turret currently
/// showing and brings up the variant asked for.
/// </summary>
/// <param name="index">The turret variant to display, or -1 to leave the turret down.</param>
void BuildingClass::Set_Turret_Index(int index)
{
	if (index != TurretIndex && !Class->IsTurretAnimAVoxel) {
		End_Anim(BANIM_TURRET);
		if (index != -1) {
			char buffer[64]; // RA2: RA2's form
			strcpy(buffer, Class->GraphicName);
			strcat(buffer, "_");
			unsigned int len = strlen(buffer); // RA2: as above
			buffer[len] = (char)(index + 'B'); // RA2: as above
			buffer[len + 1] = '\0';
			Create_Anim(buffer, BANIM_TURRET, false, false, 0); // RA2: as above
			TurretIndex = index; // RA2: as above
		}
	}
}



/***********************************************************************************************
 * BuildingClass::Shape_Number -- Fetch the shape number for this building.                    *
 *                                                                                             *
 *    This routine will examine the current state of the building and return with the shape    *
 *    number to use. The shape number is subordinate to the building graphic image data.       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the shape number to use when rendering this building.                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int BuildingClass::Shape_Number(void) const
{
	BuildingTypeClass *type = Class; // RA2: RA2's form
	unsigned int frame = (unsigned int)Fetch_Stage(); // RA2: as above
	if (type->IsLaserFence) { // RA2: as above
		return LaserFenceFrame; // RA2: as above
	}
	if (type->IsFirestormWall) { // RA2: as above
		return FirestormWallFrame; // RA2: as above
	}

	/*
	**	The shape file to use for rendering depends on whether the building
	**	is undergoing construction or not.
	*/
	if (BState == BSTATE_CONSTRUCTION) {
		if (type->IsGate) { // RA2: as above
			frame = (unsigned int)type->Anims[BSTATE_CONSTRUCTION].Count + (unsigned int)type->Anims[BSTATE_CONSTRUCTION].Start - frame - 1; // RA2: as above
		}

		/*
		**	If the building is deconstructing, then the display frame progresses
		**	from the end to the beginning. Reverse the shape number accordingly.
		*/
		if (Get_Mission() == MISSION_DECONSTRUCTION) { // RA2: as above
			frame = (unsigned int)Class->Anims[BState].Count + (unsigned int)Class->Anims[BState].Start - frame - 1; // RA2: as above
		}
	} else {
		if (type->CanBeOccupied) { // RA2: as above
			if (HealthRatio <= Rule->ConditionRed) { // RA2: as above
				return 1; // RA2: as above
			}
			if (Occupants.Count() > 0) { // RA2: as above
				return 2; // RA2: as above
			}
			return 0; // RA2: as above
		}
		if (type->IsGate) { // RA2: as above
			/* // RA2: as above
			**	If below half strenth, then show the damage frames of the // RA2: as above
			**	building. // RA2: as above
			*/ // RA2: as above
			if (HealthRatio <= Rule->ConditionYellow) { // RA2: as above
				return (unsigned int)Class->GateStages + 1; // RA2: as above
			}
			return 0; // RA2: as above
		}
		if (HealthRatio <= Rule->ConditionYellow) {
			if (BState == BSTATE_IDLE) {
				++frame; // RA2: as above
			} else {
				type = Class; // RA2: as above
				int end = (int)((unsigned int)type->Anims[BSTATE_IDLE].Count + (unsigned int)type->Anims[BSTATE_IDLE].Start); // RA2: as above
				int other = (int)((unsigned int)type->Anims[BSTATE_ACTIVE].Count + (unsigned int)type->Anims[BSTATE_ACTIVE].Start); // RA2: as above
				if (end <= other) { // RA2: as above
					end = other; // RA2: as above
				}
				other = (int)((unsigned int)type->Anims[BSTATE_AUX1].Count + (unsigned int)type->Anims[BSTATE_AUX1].Start); // RA2: as above
				if (end <= other) { // RA2: as above
					end = other; // RA2: as above
				}
				other = (int)((unsigned int)type->Anims[BSTATE_AUX2].Count + (unsigned int)type->Anims[BSTATE_AUX2].Start); // RA2: as above
				if (end <= other) { // RA2: as above
					end = other; // RA2: as above
				}
				frame += (unsigned int)end; // RA2: as above
			}
		}
	}
	return frame; // RA2: as above
}





/***********************************************************************************************
 * BuildingClass::Mark -- Building interface to map rendering system.                          *
 *                                                                                             *
 *    This routine is used to mark the map cells so that when it renders                       *
 *    the underlying icons will also be updated as necessary.                                  *
 *                                                                                             *
 * INPUT:   mark  -- Type of image change (MARK_UP, _DOWN, _CHANGE)                            *
 *             MARK_UP  -- Building is removed.                                                *
 *             MARK_CHANGE -- Building changes shape.                                          *
 *             MARK_DOWN -- Building is added.                                                 *
 *                                                                                             *
 * OUTPUT:  bool; Did the mark operation succeed? Failure could be the result of marking down  *
 *                when the building is already marked down, or visa versa.                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/31/1994 JLB : Created.                                                                 *
 *   04/15/1994 JLB : Converted to member function.                                            *
 *   04/16/1994 JLB : Added health bar tracking.                                               *
 *   12/23/1994 JLB : Calls low level check before proceeding.                                 *
 *   01/27/1995 JLB : Special road spacer template added.                                      *
 *=============================================================================================*/
bool BuildingClass::Mark(MarkType mark)
{
	if (BASECLASS::Mark(mark)) {
		Cell cell = Get_Cell(); // RA2: through the virtual

		switch (mark) {
			case MARK_UP:
				Map.Pick_Up(cell, this);
				break;

			case MARK_DOWN:
			case MARK_DOWN_FORCED:

				if (Class->ToTile != NULL) {
					if (Can_Enter_Cell(&Map[cell], FACING_NONE, -1, NULL, true) == MOVE_OK) { // RA2: all the arguments
						int w = Class->Width();
						int h = Class->Height(false); // RA2: without the bib
						bool any_created = false;
						int x; // RA2: the loop counters are declared with the blocks that use them
						int y; // RA2: as above

						for (y = 0; y < h; y++) {
							for (x = 0; x < w; x++) {
								Cell newcell = cell + Cell(x, y);
								if (Map[newcell].Is_Clear_To_Build(Class->Speed, Class, House)) {
									if (Map[newcell].ITType != Class->ToTile->HeapID && Map[newcell].Land_Type() != LAND_ROAD) {
										CellClass *cptr = &Map[newcell];
										if (cptr->Smudge != SMUDGE_NONE) {
											SmudgeTypeClass *sptr = SmudgeTypes[cptr->Smudge];
											Cell position = cptr->CellID;
											int width = sptr->Width;
											int xx = cptr->SmudgeData % width;
											int yy = cptr->SmudgeData / width;
											position -= Cell(xx, yy);
											for (int smy = 0; smy < sptr->Height; smy++) {
												for (int smx = 0; smx < sptr->Width; smx++) {
													Map[position + Cell(smx, smy)].Smudge = SMUDGE_NONE;
													Map[position + Cell(smx, smy)].Register_As_Dirty();
												}
											}
										}
										ObjectClass *iptr = Class->ToTile->Create_One_Of(House); // RA2: an object, unlimboed through the virtual
										iptr->Unlimbo(Coord(newcell), DIR_N); // RA2: as above
										if (House == PlayerPtr) {
											Map.Sight_From(Coord(newcell), 1, PlayerPtr, false, false, false, true, false); // RA2: the sight is revealed twice, plain and with the fog
											Map.Sight_From(Coord(newcell), 1, PlayerPtr, false, false, false, true, true); // RA2: as above
										}
										any_created = true;
									}
								}
							}
						}

						for (y = 0; y < h + 2; y++) {
							for (x = 0; x < w + 2; x++) {
								Cell snewcell = cell + Cell(x - 1, y - 1);
								Map[snewcell].Register_For_Redraw();
							}
						}

						if (any_created) {
							SendToEachLink(RADIO_OVER_OUT); // RA2: to every link
							Delete_Me();
							return(true); // RA2: as above
						} else {
							BASECLASS::Mark(MARK_UP);
							return(false);
						}
					} else {
						BASECLASS::Mark(MARK_UP);
						return(false);
					}
				}
				/*
				**	Special wall logic is handled here. A building that is really a wall
				**	gets converted into an overlay wall type when it is placed down. The
				**	actual building object itself is destroyed.
				*/
				else if (Class->IsWall) {
					new OverlayClass(Class->ToOverlay, cell, House->Class->House);
					SendToEachLink(RADIO_OVER_OUT); // RA2: as above
					Delete_Me();

				} else {
					if (mark != MARK_DOWN_FORCED) {
						if (Can_Enter_Cell(&Map[cell], FACING_NONE, -1, NULL, true) != MOVE_OK) { // RA2: as above
							return(false);
						}
					}
					Map.Place_Down(cell, this);
					Set_Coord(Class->Coord_Fixup(Coord(cell))); // RA2: through the virtual
					Set_Anim_Coords();

					int w = Class->Width(); // RA2: the smudges under the footprint go as well
					int h = Class->Height(false); // RA2: as above
					int x; // RA2: as above
					int y; // RA2: as above
					for (y = 0; y < h; y++) { // RA2: as above
						for (x = 0; x < w; x++) { // RA2: as above
							Cell newcell = cell + Cell(x, y); // RA2: as above
							CellClass *cptr = &Map[newcell]; // RA2: as above
							if (cptr->Smudge != SMUDGE_NONE) { // RA2: as above
								SmudgeTypeClass *sptr = SmudgeTypes[cptr->Smudge]; // RA2: as above
								Cell position = cptr->CellID; // RA2: as above
								int width = sptr->Width; // RA2: as above
								int xx = cptr->SmudgeData % width; // RA2: as above
								int yy = cptr->SmudgeData / width; // RA2: as above
								position -= Cell(xx, yy); // RA2: as above
								for (int smy = 0; smy < sptr->Height; smy++) { // RA2: as above
									for (int smx = 0; smx < sptr->Width; smx++) { // RA2: as above
										Map[position + Cell(smx, smy)].Smudge = SMUDGE_NONE; // RA2: as above
										Map[position + Cell(smx, smy)].Register_As_Dirty(); // RA2: as above
									}
								}
							}
						}
					}

					for (y = 0; y < h + 2; y++) { // RA2: as above
						for (x = 0; x < w + 2; x++) { // RA2: as above
							Cell snewcell = cell + Cell(x - 1, y - 1); // RA2: as above
							Map[snewcell].Register_For_Redraw(); // RA2: as above
						}
					}
				}
				break;

			case MARK_CHANGE:
				Update_Anim_Appearance();
				break;

			default:
				break;
		}
		return(true);
	}
	return(false);
}




// RA2: the lepton offset of a garrison slot as a coordinate at height zero (an inline helper of KillOccupants 0x4536D0 and AI 0x43C930)
static inline void Occupant_Offset_To_Coord(Coord & coordinate, Point2D const & point) // RA2: as above
{
	int y = point.Y; // RA2: as above
	int x = point.X; // RA2: as above
	coordinate.X = x; // RA2: as above
	coordinate.Y = y; // RA2: as above
	coordinate.Z = 0; // RA2: as above
}

/***********************************************************************************************
 * BuildingClass::AI -- Handles non-graphic AI processing for buildings.                       *
 *                                                                                             *
 *    This function is to handle the AI logic for the building. The graphic logic (facing,     *
 *    firing, and animation) is handled elsewhere.                                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/31/1994 JLB : Created.                                                                 *
 *   12/26/1994 JLB : Handles production.                                                      *
 *   06/11/1995 JLB : Revamped.                                                                *
 *=============================================================================================*/
void BuildingClass::AI(void)
{
	bool working = IsPowerOnline() && Mission != MISSION_CONSTRUCTION && Mission != MISSION_DECONSTRUCTION; // RA2: a building at work plays its working sound
	if (working != WasOnline) { // RA2: as above
		if (Class->WorkingSound != -1 || Class->NotWorkingSound != -1) { // RA2: as above
			WorkingSoundController.End(); // RA2: as above
			if (working) { // RA2: as above
				VocClass::PlayAt(Class->WorkingSound, PositionCoord, &WorkingSoundController); // RA2: as above
			} else {
				VocClass::PlayAt(Class->NotWorkingSound, PositionCoord, &WorkingSoundController); // RA2: as above
			}
		}
		Update_Power_State(); // RA2: as above
		WasOnline = working; // RA2: as above
	} else {
		if (Class->WorkingSound != -1 || Class->NotWorkingSound != -1) { // RA2: as above
			VocClass::UpdateAt(PositionCoord, &WorkingSoundController); // RA2: as above
		}
	}

	bool damaged; // RA2: the damage fires burn below the red or yellow condition
	if (Class->CanBeOccupied) { // RA2: as above
		damaged = HealthRatio <= Rule->ConditionRed; // RA2: as above
	} else {
		damaged = HealthRatio <= Rule->ConditionYellow; // RA2: as above
	}
	if (damaged != RequiresDamageFires) { // RA2: as above
		if (damaged) { // RA2: as above
			Create_Damage_Fires(); // RA2: as above
		} else {
			Remove_Damage_Fires(); // RA2: as above
		}
		RequiresDamageFires = damaged; // RA2: as above
	}

	if (Class->IsTurretEquipped) { // RA2: whether the turret is turning
		TurretIsRotating = PrimaryFacing.Is_Rotating(); // RA2: as above
	} else {
		TurretIsRotating = 0; // RA2: as above
	}

	if (TemporalTargetingMe != NULL) { // RA2: the temporal weapon aimed at this building runs
		TemporalTargetingMe->AI(); // RA2: as above
	}

	if (IsBeingWarpedOut() || IsWarpingIn()) { // RA2: a building in the chronosphere only sparkles
		if (Class->MaxNumberOccupants == 0 || Rule->ChronoSparkle1 == NULL) { // RA2: as above
			if (Frame % 24 == 0 && Rule->ChronoSparkle1 != NULL) { // RA2: as above
				new AnimClass(Rule->ChronoSparkle1, PositionCoord, 0, 1, ShapeFlags_Type(0x600), 0); // RA2: as above
			}
		} else {
			for (int index = 0; index < Class->MaxNumberOccupants; index++) { // RA2: a sparkle per garrison slot
				if ((Frame + index) % 24 == 0 && Rule->ChronoSparkle1 != NULL) { // RA2: as above
					Coord location; // RA2: as above
					Occupant_Offset_To_Coord(location, TacticalMap->Pixel_To_Lepton(Class->MuzzleFlash[index])); // RA2: the offset helper
					location = Render_Coord() + location; // RA2: as above
					AnimClass * effect = new AnimClass(Rule->ChronoSparkle1, location, 0, 1, ShapeFlags_Type(0x600), 0); // RA2: as above
					effect->ZAdjust = -200; // RA2: as above
				}
			}
		}
		if (TarCom != NULL) { // RA2: as above
			Assign_Target(NULL); // RA2: as above
		}
		return; // RA2: as above
	}

	if (CashProductionTimer.Value() == 1) { // RA2: a cash producer pays out when its timer runs out
		CashProductionTimer = 0; // RA2: as above
		CashProductionTimer = Class->ProduceCashDelay; // RA2: Produce_Cash inline
		if (!House->Class->IsMultiplayPassive && IsPowerOnline()) { // RA2: as above
			int amount = Class->ProduceCashAmount; // RA2: as above
			if (amount > 0) { // RA2: as above
				House->Refund_Money(amount); // RA2: as above
			} else {
				House->Spend_Money(-amount); // RA2: as above
			}
		}
	}

	if (Class->IsSAM && TarCom != NULL && !TarCom->In_Air()) {
		Assign_Target(NULL);
	}

	/*
	**	Process building animation state changes. Transition to a following state
	**	if there is one specified and the current animation sequence has expired.
	**	This process must occur before mission AI since the mission AI relies on
	**	the bstate change to occur immediately before the MissionClass::AI.
	*/
	Animation_AI();

	/*
	**	If now is a good time to act on a new mission, then do so. This process occurs
	**	here because some outside event may have requested a mission change for the building.
	**	Such outside requests (player input) must be initiated BEFORE the normal AI process.
	*/
	if (Ready_To_Commence() && BState != BSTATE_CONSTRUCTION) {

		/*
		**	Clear the commencement flag ONLY if something actually occurred. By acting
		**	this way, a building can set the IsReadyToCommence flag before it goes
		**	to "sleep" knowing that it will wake up as soon as a new mission comes
		**	along.
		*/
		if (Commence()) {
			IsReadyToCommence = false;
		}
	}

	/*
	**	Proceed with normal logic processing. This is where the mission processing
	**	occurs. This call must be located after the animation sequence makes the
	**	transition to the next frame (see above) in order for the mission logic to
	**	act at the exact moment of graphic transition BEFORE it has a chance to
	**	be displayed.
	*/
	BASECLASS::AI();

	/*
	**	Bail if the object died in the AI routine.
	*/
	if (!IsActive) {
		return;
	}

	/*
	**	Building ammo is instantly reloaded.
	*/
	if (!Ammo && !Class->IsHospital && !Class->IsArmory) {
		Ammo = Class->MaxAmmo;
	}

	/*
	**	If now is a good time to act on a new mission, then do so. This occurs here because
	**	some AI event may have requested a mission change (usually from another mission
	**	state machine). This must occur here before it has a chance to render.
	*/
	if (Ready_To_Commence()) {

		/*
		**	Clear the commencement flag ONLY if something actually occurred. By acting
		**	this way, a building can set the IsReadyToCommence flag before it goes
		**	to "sleep" knowing that it will wake up as soon as a new mission comes
		**	along.
		*/
		if (Commence()) {
			IsReadyToCommence = false;
		}
	}

	/*
	**	If a change of animation was requested, then make the change
	**	now. The building animation system acts independently but subordinate
	**	to the mission state machine system. By performing the animation change-up
	**	here, the mission AI system is ensured of immediate visual affect when it
	**	decides to change the animation state of the building.
	*/
	if (QueueBState != BSTATE_NONE) {
		if (BState != QueueBState) {
			BState = QueueBState;
			BuildingTypeClass::AnimControlType const * ctrl = Fetch_Anim_Control();
			if (BState == BSTATE_CONSTRUCTION || BState == BSTATE_IDLE) {
				Set_Rate(Options.Normalize_Delay(ctrl->Rate));
			} else {
				Set_Rate(ctrl->Rate);
			}
			Set_Stage(ctrl->Start);
		}
		QueueBState = BSTATE_NONE;
	}

	/*
	**	If the building's strength has changed, then update the power
	**	accordingly.
	*/
	if (Strength != LastStrength) {
		House->RecalcPower = true;
		House->RecalcRadar = true;
		LastStrength = Strength;
		Adjust_House_Power(House);
	}

	/*
	**	Check to see if the destruction countdown timer is active. If so, then decrement it.
	**	When this timer reaches zero, the building is removed from the map. All the explosions
	**	are presumed to be in progress at this time.
	*/
	if (Strength == 0) {
		Remove_Damage_Fires(); // RA2: the fires go out first
		if (CountDown == 0) {
			Limbo();
			Drop_Debris(WhomToRepay);
			Delete_Me();
			Leave_Rubble(); // RA2: the ruins stay behind
		}
		return;
	}

	/*
	**	Charging logic.
	*/
	Delayed_Fire_AI(); // RA2: the delayed fire logic of the prism tower instead of charging

	if (Class->Overpowerable) { // RA2: the infantry overpowering this building are counted
		for (int index = Overpowerers.Count() - 1; index >= 0; index--) { // RA2: as above
			if (Overpowerers[index]->TarCom != this) { // RA2: as above
				Overpowerers.Delete_Index(index); // RA2: as above
			}
		}
		int count = Overpowerers.Count(); // RA2: as above
		if (count >= 3 || (House->Power_Fraction() == 1.0 && IsOn && count >= 1)) { // RA2: as above
			IsOverpowered = true; // RA2: as above
		} else {
			IsOverpowered = false; // RA2: as above
		}
	}

	if (Class->CanBeOccupied) { // RA2: the garrison follows the owner
		Update_Garrison_Owner(); // RA2: as above
	}

	/*
	**	Handle any repair process that may be going on.
	*/
	Repair_AI();

	/*
	**	For computer controlled buildings, determine what should be produced and start
	**	production accordingly.
	*/
	if (Class->ToBuild != RTTI_NONE) {
		Factory_AI();
	}

	/*
	**	Check for demolition timeout. When timeout has expired, the building explodes.
	*/
	if (IsGoingToBlow && CountDown == 0) {
		int damage = Strength; // RA2: as above
		if (Class->IsBridgeRepairHut) { // RA2: a bridge repair hut takes the bridge next to it
			bool found = false; // RA2: as above
			for (int y = -2; y < 3; y++) { // RA2: as above
				for (int x = -2; x < 3; x++) { // RA2: as above
					IsometricTileType tile = Map[Get_Cell() + Cell(x, y)].ITType; // RA2: as above
					OverlayType overlay = Map[Get_Cell() + Cell(x, y)].Overlay; // RA2: as above
					if ((tile >= IsometricTileTypeClass::TrainBridgeSet && tile < IsometricTileTypeClass::TrainBridgeSet + 16) || (overlay >= OVERLAY_LOWBRIDGE_FIRST && overlay <= OVERLAY_LOWBRIDGE_LAST)) { // RA2: as above
						found = true; // RA2: as above
					}
				}
			}
			if (found) { // RA2: as above
				Map.Destroy_Low_Bridge_Near(Get_Cell()); // RA2: as above
			} else {
				Map.Destroy_Bridge2_Near(Get_Cell()); // RA2: as above
			}
			IsGoingToBlow = false; // RA2: as above
			WhomToRepay = NULL; // RA2: as above
		} else {
			Take_Damage(damage, 0, Rule->C4Warhead, (TechnoClass *)WhomToRepay, true); // RA2: the saboteur is credited
		}
		if (!IsActive) {
			return;
		}
		Mark(MARK_CHANGE);
	}

	if (Class->IsFirestormWall && (Frame & 7) == 0)  {
		int frame = FirestormWallFrame & MAX_FIRESTORM_WALL_FRAMES;
		if (House->FirestormDefenseActivated) {
			if (frame != 10 && frame != 5 && Anims[BANIM_SPECIAL_TWO] == NULL && (Scen->RandomNumber() & MAX_FIRESTORM_WALL_FRAMES) == 0) {
				Anims[BANIM_SPECIAL_TWO] = new AnimClass(Rule->FirestormIdleAnim, PositionCoord - Coord(740,740,0), 0, 1, ShapeFlags_Type(SHAPE_WIN_REL|SHAPE_CENTER|SHAPE_TRANSLUCENT50), -10);
			}
		}
	}

	if (TarCom != NULL && !IsCloseEnoughToAttack(TarCom)) { // RA2: the target left the range
		if (TarCom->RTTI != RTTI_AIRCRAFT) { // RA2: as above
			Assign_Target(NULL); // RA2: as above
		} else if (TarCom->On_Ground()) { // RA2: as above
			Assign_Target(NULL); // RA2: as above
		}
	}
}






/***********************************************************************************************
 * BuildingClass::Unlimbo -- Removes a building from limbo state.                              *
 *                                                                                             *
 *    Use this routine to transform a building that has been held in limbo                     *
 *    state, into one that really exists on the map. Once a building as                        *
 *    been unlimboed, then it becomes a normal object in the game world.                       *
 *                                                                                             *
 * INPUT:   pos   -- The position to place the building on the map.                            *
 *                                                                                             *
 *          dir (optional) -- not used for this class                                          *
 *                                                                                             *
 * OUTPUT:  bool; Was the unlimbo successful?                                                  *
 *                                                                                             *
 * WARNINGS:   The unlimbo operation might not be successful if the                            *
 *             building could not be placed at the location specified.                         *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/16/1994 JLB : Created.                                                                 *
 *   06/07/1994 JLB : Matches virtual function format for base class.                          *
 *   05/09/1995 JLB : Handles wall placement.                                                  *
 *   06/18/1995 JLB : Checks for wall legality before placing down.                            *
 *=============================================================================================*/
bool BuildingClass::Unlimbo(Coord const & coord, Dir256 dir)
{
	bool ok = Can_Enter_Cell(&Map[coord], FACING_NONE, -1, NULL, true) == MOVE_OK; // RA2: as above

	if (ok) {
		if (Class == Rule->WallTower) {
			CellClass * cellptr = &Map[coord];
			HouseClass * owner = NULL;
			if (cellptr->Owner >= HOUSE_FIRST) {
				owner = Houses[cellptr->Owner];
			}
			if (owner == House) {
				if (cellptr->Overlay == OVERLAY_BRICK_WALL || cellptr->Overlay == OVERLAY_SANDBAG_WALL) {
					House->Sell_Wall(cellptr->CellID, true); // RA2: quietly
				}
			}
		} else if (Class->ToOverlay != NULL) {
			CellClass * cellptr = &Map[coord];
			HouseClass * owner = NULL;
			if (cellptr->Owner >= HOUSE_FIRST) {
				owner = Houses[cellptr->Owner];
			}
			if (owner == House) {
				if (cellptr->Overlay == Class->ToOverlay->HeapID) {
					House->Sell_Wall(cellptr->CellID, true); // RA2: quietly
				}
			}
		} else if (Class->IsGate) {
			Cell cell = coord.As_Cell();
			Cell const * occupy = Class->Occupy_List(true);
			while (*occupy != REFRESH_EOL) {
				CellClass * cellptr = &Map[cell + *occupy];
				HouseClass * owner = NULL;
				if (cellptr->Owner >= HOUSE_FIRST) {
					owner = Houses[cellptr->Owner];
				}
				if (owner == House) {
					if (cellptr->Overlay == OVERLAY_BRICK_WALL || cellptr->Overlay == OVERLAY_SANDBAG_WALL || cellptr->Overlay == OVERLAY_NOD_WALL) {
						House->Sell_Wall(cellptr->CellID, true); // RA2: quietly
					}
				}
				BuildingClass * building = cellptr->Cell_Building();
				if (building != NULL && building->Class->IsLaserFence && building->Owner() == Owner()) {
					Unlimbo_Laser_Fence_Helper(cellptr->Fetch_CellID());
				}
				occupy++;
			}
		}
	}

	/*
	**	If this is a wall type building, then it never gets unlimboed. Instead, it gets
	**	converted to an overlay type.
	*/
	if (Class->IsWall) {
		if (Can_Enter_Cell(&Map[coord], FACING_NONE, -1, NULL, true) == MOVE_OK) { // RA2: as above
			ObjectClass * o = Class->ToOverlay->Create_One_Of(House);
			if (o && o->Unlimbo(coord, DIR_N)) { // RA2: as above
				Map[coord].Owner = House->HeapID;
				Transmit_Message(RADIO_OVER_OUT);
				Map.Sight_From(coord, Class->SightRange, House, false, false, false, true, false); // RA2: the sight is revealed twice, plain and with the fog
				Map.Sight_From(coord, Class->SightRange, House, false, false, false, true, true); // RA2: as above
				Delete_Me();
				return(true);
			}
		}
		return(false);
	}

	if (Class->PowersUpBuilding[0] != '\0') {
		ObjectClass * object = Map.Cell_Object(coord.As_Cell(), Point2D(0, 0));
		if (object != NULL && object->RTTI == RTTI_BUILDING) {
			BuildingClass * building = (BuildingClass *)object;
			BuildingTypeClass *btype = Class;
			if (House == building->House && stricmp(btype->PowersUpBuilding, (char *)building->Class->IniName) == 0) {
				bool ok = false;
				if (btype->PowersUpToLevel != -1) {
					if (btype->PowersUpToLevel <= 0 || btype->PowersUpToLevel > BUILDING_UPGRADE_MAX) {
						return(false);
					}
				} else {
					if (building->UpgradeLevel < building->Class->Upgrades) {
						ok = true;
					}
				}
				if (!ok && building->UpgradeLevel) {
					return(false);
				}

				House->RecalcRadar = true;
				House->RecalcPower = true;
				int levels = Class->PowersUpToLevel;
				if (levels == -1) {
					levels = 1;
				}
				char * anim = building->Class->AnimData[building->UpgradeLevel].Anim;
				if (stricmp(anim, Class->GraphicName) != 0) {
					strncpy(anim, Class->GraphicName, sizeof(building->Class->AnimData[0].Anim));
				}
				while (levels) {
					building->Add_Upgrade();
					levels--;
				}
				building->Upgrades[building->UpgradeLevel - 1] = Class;
				Adjust_House_Power(House);
				House->Enable_Available_Super_Weapons();
				House->IsRecalcNeeded = true;
				if (Class->IsThreatRatingNode) {
					House->Activate_Threat_Node();
				}
				Delete_Me();
				return(true);
			}
		}

		return(false);
	}

	if (Class->IsLaserFence) {
		if ((DirType(dir << 8).As_Dir8() & 3) == FACING_E) { // RA2: the direction is a Dir256
			LaserFenceFrame = 8;
		} else {
			LaserFenceFrame = 12;
		}
	}

	if (ok && Class->IsLaserFencePost) {
		LaserFenceFrame = 0;
		BuildingClass * building = Map[coord.As_Cell()].Cell_Building();
		if (building != NULL && building->Class->IsLaserFence && Owner() == building->Owner()) {
			building->Delete_Me();
		}
	}

	/*
	**	Normal building unlimbo process.
	*/
	if (BASECLASS::Unlimbo(coord, dir)) {

		if (Class->IsCloakGenerator) {
			House->HasCloakGenerator = true;
		}

		if (!IsActive) {
			return(true);
		}

		if (Session.Type != GAME_NORMAL && !House->Is_Human_Player() && !House->Class->IsMultiplayPassive) {
			IsToRepair = true;
		}

		if (Scen->Special.IsFogOfWar) {
			if (Should_Fog()) {
				Make_Fogged(NULL, NULL, false); // RA2: all the arguments
			}
		}

		if (!Considered_Vehicle()) {
			Reserve_Base_Area(false); // RA2: as above
		}

		int width = Class->Width();
		int height = Class->Height(false); // RA2: without the bib
		Cell cell = coord.As_Cell();

		for (int y = 0; y < height + 2; y++) {
			for (int x = 0; x < width + 2; x++) {
				CellClass * cptr = &Map[Cell(x, y) + (cell - Cell(1,1))];
				cptr->AdjacentObjectCount++;
			}
		}

		if (Class->IsFirestormWall) {
			Update_FS_Wall_State();
			CellClass * cellptr = &Map[coord];
			for (int facing = FACING_N; facing < FACING_COUNT; facing += FACING_90) {
				BuildingClass * building = cellptr->Adjacent_Cell((FacingType)facing).Cell_Building();
				if (building != NULL && building->Class->IsFirestormWall) {
					building->Update_FS_Wall_State();
				}
			}
		}

		/*
		**	Recalculate the center point of the house's base.
		*/
		House->Recalc_Center();

		/*
		**	Update the total factory type, assuming this building has a factory.
		*/
		House->Active_Add(this);

		/*
		**	Possibly the sidebar will be affected by this addition.
		*/
		House->IsRecalcNeeded = true;
		LastStrength = 0;

		if ((!IsDiscoveredByPlayer && Map[coord].IsVisible) || Session.Type != GAME_NORMAL) {
			Revealed(PlayerPtr);
		} else if (Class->LightIntensity != 0) {
			if (LightSource == NULL) {
				LightSource = new LightSourceClass(Center_Coord(), Class->LightVisibility, Class->LightIntensity, Class->LightRedTint, Class->LightGreenTint, Class->LightBlueTint);
			}
			LightSource->Enable(false); // RA2: as above
		}

		if (Class->IsLaserFencePost && !ScenarioInit) {
			if (IsActive && !IsInLimbo) {
				Connect_Laser_Fence(FACING_N);
				Connect_Laser_Fence(FACING_E);
				Connect_Laser_Fence(FACING_S);
				Connect_Laser_Fence(FACING_W);
				Toggle_Laser_Fence_Post(true);
			}
		}

		if (!House->Is_Human_Player()) {
			Revealed(House);
		}

		if (IsOwnedByPlayer) {
			Map.PowerClass::IsToRedraw = true;
			Map.Flag_To_Redraw(GS_REDRAW_DIRTY); // RA2: as above
		}

		if (Class->NaturalParticleSystem != NULL && ParticleSystems[ATTACHED_PARTICLE_NATURAL] == NULL) {
			Coord sysloc = Class->NaturalParticleLocation;
			ParticleSystems[ATTACHED_PARTICLE_NATURAL] = new ParticleSystemClass(Class->NaturalParticleSystem, sysloc + PositionCoord, &Map[(Coord const &)PositionCoord], NULL);
		}

		if (Class == Rule->WallTower) {
			Cell cell = Map[coord].Fetch_CellID();
			Map[cell + Cell(1, 0)].Wall_Update(false); // RA2: as above
			Map[cell + Cell(-1, 0)].Wall_Update(false); // RA2: as above
			Map[cell + Cell(0, 1)].Wall_Update(false); // RA2: as above
			Map[cell + Cell(0, -1)].Wall_Update(false); // RA2: as above
		}

		if (Class == Rule->GDIGateOne || Class == Rule->NodGateOne) {
			Cell cell = Map[coord].Fetch_CellID();
			Map[cell + Cell(-1, 0)].Wall_Update(false); // RA2: as above
			Map[cell + Cell(3, 0)].Wall_Update(false); // RA2: as above
		}

		if (Class == Rule->GDIGateTwo || Class == Rule->NodGateTwo) {
			Cell cell = Map[coord].Fetch_CellID();
			Map[cell + Cell(0, -1)].Wall_Update(false); // RA2: as above
			Map[cell + Cell(0, 3)].Wall_Update(false); // RA2: as above
		}

		if (Class->HasSpotlight) {
			BuildingLight = new BuildingLightClass(this);
		}

		IsLeader = false; // RA2: a new building is not the primary factory until it asks
		if (Class->ToBuild != RTTI_NONE) { // RA2: as above
			Become_Primary(false); // RA2: as above
		}

		for (int i = 0; i < Rule->BuildConst.Count(); i++) { // RA2: every construction yard type adds the building to ConYards
			if (Rule->BuildConst[i] == Class) { // RA2: as above
				House->ConYards.Add(this); // RA2: as above
				break; // RA2: as above
			}
		}
		if (Class->IsCanUnitRepair) { // RA2: the house keeps lists of the special buildings
			House->UnitRepairStations.Add(this); // RA2: as above
		}
		if (Class->Cloning) { // RA2: as above
			House->CloningVats.Add(this); // RA2: as above
		}
		if (Class->PsychicDetectionRadius > 0) { // RA2: as above
			House->PsychicDetectionBuildings.Add(this); // RA2: as above
		}
		House->Buildings.Add(this); // RA2: as above

		if (!House->Is_Human_Player()) { // RA2: a computer house plans the base node
			House->Base.Mark_Node_Placed(this); // RA2: as above
		}

		if (Class->ToBuild != RTTI_NONE) {
			House->Update_Factories(Class->ToBuild, Class->Naval, 0); // RA2: with the naval flag and category 0
		}

		return(true);
	}
	return(false);
}



/// <summary>
/// RA2
/// </summary>
inline void Register_Dirty(Rect const & area)
{
	TacticalMap->Register_Dirty_Area(area, false);
}


/// <summary>
/// Handles the destruction of the building.
/// This routine performs everything that must happen the moment a structure is reduced
/// to rubble. The occupants die with it, the ground is scorched, fire and explosions are
/// strewn across the footprint, any stored tiberium spills back onto the map, the screen
/// is shaken, and the survivors are turned loose.
/// </summary>
/// <param name="source">The object responsible for the destruction.</param>
/// <param name="forced">Should the destruction be forced, leaving no survivors?</param>
/// <param name="offset">Pointer to the REFRESH_EOL terminated list of cell offsets that
/// make up the building's footprint.</param>
#ifdef RA2_NON_MATCHING // not exact: 2205 bytes differ (adopted text)
void BuildingClass::Do_Destruction(TechnoClass *last_contact, TechnoClass *source, bool forced, Cell const *offset)
{
	int shakes;
	int i;

	/*
	**	Destroy all attached objects.
	*/
	Kill_Cargo(source);

	/*
	**	Destruction of a radar facility or advanced communications
	**	center will cause the spiedby field to change...
	*/
	if (SpiedBy) {
		SpiedBy = 0;
		if (Class->IsRadar) {
			Update_Radar_Spied();
		}
	}

	if (Class->IsCloakGenerator) {
		Disable_Cloak_Generator();
		CurrentCloakRadius = 1;
		Cloaking_AI(true);
	}

	if (Class->IsLaserFencePost) {
		Update_Laser_Fence_Connections(true);
	}


	if (Class->Width() >= 2 && Class->Height() >= 2) { // RA2: no CrumbleSound (not in RA2's layout)
		if (Class->Width() > 2) {
			Random_Pick(0, Class->Width() - 2);
		}
		if (Class->Height() > 2) {
			Random_Pick(0, Class->Height() - 2);
		}
		if (Percent_Chance(50)) {
			SmudgeTypeClass::Scorch_The_Ground(PositionCoord.As_Cell(), 100, 100, true);
		} else {
			SmudgeTypeClass::Crater_The_Ground(PositionCoord.As_Cell(), 100, 100, true);
		}
	}

	while (*offset != REFRESH_EOL) {
		Coord pos;
		Coord coord(0,0,0);
		Cell cell = Render_Coord().As_Cell() + *offset++;

		/*
		**	If the building is destroyed, then lots of
		**	explosions occur.
		*/
		if (Percent_Chance(50)) {
			coord.Z = PositionCoord.Z;
			Coord ccoord = cell;
			pos = coord + Coord_Scatter(ccoord, CELL_LEPTON / 2);
			new AnimClass(Rule->SmallFire, pos, Random_Pick(0, 7), Random_Pick(1, 3));
			if (Percent_Chance(50)) {
				coord.Z = PositionCoord.Z;
				Coord ccoord = cell;
				pos = coord + Coord_Scatter(ccoord, CELL_LEPTON / 4);
				new AnimClass(Rule->LargeFire, pos, Random_Pick(0, 7), Random_Pick(1, 3));
			}
		}
		if (Class->Explosion.Count() > 0) {
			coord.Z = PositionCoord.Z;
			Coord ccoord = cell;
			pos = coord + Coord_Scatter(ccoord, CELL_LEPTON / 4);
			new AnimClass((Class->Explosion.Pick(Scen->RandomNumber())), pos, Random_Pick(0, 3), 1);
		}
	}

	if (Class->IsExploding) {
		static FacingType _facings[] = { FACING_N, FACING_E, FACING_S, FACING_W };
		for (i = 0; i < ARRAY_SIZE(_facings); i++) {
			Coord coord = Adjacent_Coord_With_Height(PositionCoord, _facings[i]);
			if (Map[coord].Overlay != OVERLAY_NONE) {
				if (OverlayTypes[Map[coord].Overlay]->IsExplosive) {
					new AnimClass(AnimTypes[AnimTypeClass::From_Name("FIRE3")], coord, Random_Pick(1, 3) + 3);
				}
			}
		}
	}

	while (Storage.Get_Total_Amount() != 0) {
		int slot = Storage.First_Used_Slot();
		Storage.Decrease_Amount(1, slot);
		House->Tiberium.Decrease_Amount(1, slot);
		Coord coord = Coord_Scatter(PositionCoord, Random_Pick(CELL_LEPTON, 3 * CELL_LEPTON), true);
		Map[coord].Place_Tiberium((TiberiumType)slot, 1);
	}

	shakes = Class->Cost_Of(House) / Rule->ShakeScreen;
	if (shakes > 0) {
		Shake_The_Screen(shakes);
	}

	if (Mission == MISSION_DECONSTRUCTION || Class->IsExploding) {
		CountDown = 0;
		Set_Rate(0);
	} else {
		CountDown = 8;
	}

	Strength = 0;

	/*
	**	A force destruction will not generate survivors.
	*/
	if (forced) {
		IsSurvivorless = true;
	}

	Drop_Debris(WhomToRepay);
}
#endif







/// <summary>
/// Leaves the building's rubble behind: the cells it covered get the rubble overlay, the
/// cell under its render coordinate remembers the building type, and a crate (money or
/// random) appears there when the type says so.
/// </summary>
void BuildingClass::Leave_Rubble(void) // RA2: not in TibSun, at 0x43EBA0
{
	ShapeSet const * shape;
	int frame;
	if (Class->Get_Rubble_Shape(&shape, &frame)) {
		Cell const * offset = Occupy_List();
		Cell origin = Render_Coord();
		while (*offset != REFRESH_EOL) {
			Cell cell = origin + *offset++;
			CellClass & cellptr = Map[cell];
			cellptr.Register_For_Redraw();
			cellptr.Overlay = OverlayType(239); // RA2: the overlay index that marks a rubble cell
			cellptr.Rubble = NULL;
			cellptr.Recalc_Attributes(-1);
			Map.Update_Cell_Zone(cellptr.CellID);
			Map.Update_Cell_Subzones(cellptr.CellID);
			TechnoClass::Remove_Target(&cellptr);
		}
		CellClass & base = Map[origin];
		base.Rubble = Class;
		Rect area = Union(base.Overlay_Render_Rect(), base.Overlay_Shadow_Render_Rect());
		TacticalMap->Register_Dirty_Area(area - TacticalRect.TopLeft);
		TacticalMap->Flag_Cell(Map[origin]);
	}
	if (Class->CrateBeneath) {
		Cell cell = Render_Coord();
		if (Class->CrateBeneathIsMoney) {
			Map.Place_Crate(cell, CRATE_MONEY);
			return;
		}
		Map.Place_Crate(cell, CrateType(20)); // RA2: past the last crate type, a random one
	}
}






/***********************************************************************************************
 * BuildingClass::Take_Damage -- Inflicts damage points upon a building.                       *
 *                                                                                             *
 *    This routine will inflict damage points upon the specified building.                     *
 *    It will handle the damage animation and building destruction. Use                        *
 *    this routine whenever a building is attacked.                                            *
 *                                                                                             *
 * INPUT:   damage   -- Amount of damage to inflict.                                           *
 *                                                                                             *
 *          distance -- The distance from the damage center point to the object's center point.*
 *                                                                                             *
 *          warhead  -- The kind of damage to inflict.                                         *
 *                                                                                             *
 *          source   -- The source of the damage. This is used to change targeting.            *
 *                                                                                             *
 *          forced   -- Is the damage forced upon the object regardless of whether it          *
 *                      is normally immune?                                                    *
 *                                                                                             *
 * OUTPUT:  true/false; Was the building destroyed?                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/21/1991     : Created.                                                                 *
 *   04/15/1994 JLB : Converted to member function.                                            *
 *   04/16/1994 JLB : Added warhead modifier to damage.                                        *
 *   06/03/1994 JLB : Added source of damage as target value.                                  *
 *   06/20/1994 JLB : Source is a base class pointer.                                          *
 *   11/22/1994 JLB : Shares base damage handler for techno objects.                           *
 *   07/15/1995 JLB : Power ratio gets adjusted.                                               *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: 1334 bytes differ (adopted text)
ResultType BuildingClass::Take_Damage(int & damage, int distance, WarheadTypeClass const * warhead, TechnoClass * source, bool forced, bool no_crew)
{
	ResultType res = RESULT_NONE;

	if (this != source /*&& !Class->IsInsignificant*/) {

		float healthratio = HealthRatio;
		int shapenum = Shape_Number();

		if (source && !Considered_Vehicle()) {
			House->LATime = Frame;
			House->LAEnemy = source->Owner_HouseClass()->HeapID;
			Base_Is_Attacked(source);
		}

		Cell const * offset = Occupy_List();

		/*
		**	Memorize who they used to be in radio contact with.
		*/
		DynamicVectorClass<TechnoClass *> contacts; // RA2: every radio link is remembered, not just the first
		for (int index = 0; index < RadioLinks.Length(); index++) { // RA2: as above
			TechnoClass * contact = Radio_Link(index); // RA2: as above
			if (contact) { // RA2: as above
				contacts.Add(contact); // RA2: as above
			}
		}

		if (Class->IsLaserFence && !forced) {
			return(RESULT_NONE);
		}

		if (Class->IsBridgeRepairHut && Class->IsImmune) {
			return(RESULT_NONE);
		}

		if (Strength != 0) {
			/*
			**	Perform the low level damage assessment.
			*/
			res = BASECLASS::Take_Damage(damage, distance, warhead, source, forced, no_crew);
			if (!IsActive) {
				return(res);
			}
			switch (res) {

				case RESULT_ALREADY_DESTROYED:
					return(res);

				case RESULT_DESTROYED: { // RA2: braces for the loop scope

					/*
					 * Units in radio contact with this building (e.g., docked on it)
					 * are destroyed along with it if they are close enough or the
					 * building is a helipad. Otherwise they are told to get out of the way.
					 */
					for (int index = 0; index < contacts.Count(); index++) { // RA2: every contact, not just the first
						TechnoClass * contact = contacts[index]; // RA2: as above
						if (Distance_To(contact) < CELL_LEPTON || Class->IsHelipad) { // RA2: a helipad always destroys its contact
							int strength = 10 * contact->Techno_Type_Class()->MaxStrength; // RA2: ten times the contact's full strength
							contact->Take_Damage(strength, 0, Rule->C4Warhead, NULL, true, true); // RA2: as above
						} else {
							Transmit_Message(RADIO_RUN_AWAY, contact); // RA2: as above
							contact->QueueUpToEnter = NULL; // RA2: QueueUpToEnter, the object to enter once nearby
						}
					}
					contacts.Delete_All(); // RA2: the list is emptied before the building dies

					if (Class->CanBeOccupied) { // RA2: occupants are ejected
						Eject_Occupants(false); // RA2: as above
					}

					if (LightSource != NULL) {
						LightSource->Disable(false);
					}

					Do_Destruction(NULL, source, forced, offset); // RA2: as above

					if (CountDown > 0) {
						Delete_Me();
						Leave_Rubble(); // RA2: leaves the rubble behind
					}

					break;
				}

				case RESULT_HALF:
					if (ParticleSystems[2] != NULL) {
						ParticleSystems[2]->SpawnFrames *= 1.5f;
					}
					// Fall into next case.

				case RESULT_MAJOR:
					if (BuildingRunAwayOnDamage) { // RA2: objects close to the building run away from it
						for (int index = 0; index < RadioLinks.Length(); index++) { // RA2: as above
							TechnoClass * contact = Radio_Link(index); // RA2: as above
							if (contact) { // RA2: as above
								int separation = Distance(contact->Center_Coord()); // RA2: as above
								if (separation < 2 * CELL_LEPTON && !Class->IsHelipad) { // RA2: as above
									Transmit_Message(RADIO_RUN_AWAY, contact); // RA2: as above
									contact->QueueUpToEnter = NULL; // RA2: as above
								}
							}
						}
					}

					if (Techno_Type_Class()->DamageSound == -1) { // RA2: the building's own damage sound is unset, so the rules' one plays
						VocClass::PlayIndexAtPos(Rule->BuildingDamageSound, Get_Coord(), 0); // RA2: as above
					}

					while (*offset != REFRESH_EOL) { // RA2: no BlowupSound (not in RA2's layout)
						Cell cell = Get_Cell() + *offset++; // RA2: the building's cell plus the occupy offset

						AnimClass * anim = NULL;

						Coord coord(cell);
						coord.Z = Map.Get_Height_GL(coord);
						/*
						**	Show pieces of fire to indicate that a significant change in
						**	damage level has occurred.
						*/
						if (warhead->IsSparky) {
							switch (Random_Pick(0, 5+Class->Height(false)+Class->Width())) { // RA2: as above
								case 0:
									break;

								case 1:
								case 2:
								case 3:
								case 4:
								case 5:
									anim = new AnimClass(Rule->OnFire[0], Coord_Scatter(coord, 3 * CELL_LEPTON / 8), 0, Random_Pick(1, 3));
									break;

								case 6:
								case 7:
								case 8:
									anim = new AnimClass(Rule->OnFire[1], Coord_Scatter(coord, 3 * CELL_LEPTON / 8), 0, Random_Pick(1, 3));
									break;

								case 9:
									anim = new AnimClass(Rule->OnFire[2], Coord_Scatter(coord, 3 * CELL_LEPTON / 8), 0, 1);
									break;

								default:
									break;
							}
						}
						/*
						**	If the animation was created, then attach it to the building.
						*/
						if (anim) {
							anim->Attach_To(this);
						}
					}
					break;

				case RESULT_NONE:
					break;

				case RESULT_LIGHT:
					break;
			}
		}

		if (!IsActive) {
			return(RESULT_DESTROYED);
		}

		if (source && res != RESULT_NONE) {

			/*
			**	If any damage occurred, then inform the house of this fact. If it is the player's
			**	house, it might announce this fact.
			*/
			if (!Class->IsInsignificant && !Considered_Vehicle()) {
				House->Attacked(this);
			}

			/*
			**	Save the type of the house that's doing the damage, so if the building burns
			**	to death credit can still be given for the kill
			*/
			WhoLastHurtMe = source->Owner_HouseClass()->HeapID;

			/*
			**	When certain buildings are hit, they "snap out of it" and
			**	return fire if they are able and allowed.
			*/
			if (CurrentMission != MISSION_DECONSTRUCTION &&
				!House->Is_Ally(source) &&
				Get_Class_Weapon_Data(0)->Weapon != NULL && // RA2: the class weapon data
				!Get_Class_Weapon_Data(0)->Weapon->Bullet->IsAntiAircraft && // RA2: as above
				(TarCom == NULL || !IsCloseEnoughToAttack(TarCom))) { // RA2: IsCloseEnoughToAttack

				if (source->Fetch_RTTI() == RTTI_AIRCRAFT || (House->Is_Human_Player() && !Rule->IsSmartDefense)) { // RA2: Fetch_RTTI
					/*
					**	Generate a random rotation effect since there is nothing else that this
					**	building can do.
					*/
					if (!PrimaryFacing.Is_Rotating()) {
						PrimaryFacing.Set_Desired(Random_Dir(DIR_N, DIR_MAX));
					}
				} else {
					Assign_Target(source); // RA2: as above
				}
			}
		}

		if (res != RESULT_NONE) {
			Set_Anim_Damage_State(HealthRatio <= Rule->ConditionYellow);
		}

		if (shapenum != Shape_Number()) {
			IsToDisplay = true;
		}
	}

	return(res);
}
#endif



/// <summary>
/// Initializes the building to its starting condition.
/// This routine brings the building up to full strength and ammunition, registers it
/// with the owning house, and settles whether it will ever be allowed to sell -- a
/// structure with no buildup animation cannot be taken down again.
/// </summary>
void BuildingClass::Init(void)
{
	BASECLASS::Init();
	if (House != NULL) {
		WhoLastHurtMe = House->Class->House;
		House->Tracking_Add(this);
	}
	if (Class != NULL) {
		EstimatedHealth = Strength = Class->MaxStrength; // RA2: the estimate starts full too
		if (Class->InitialAmmo == -1) { // RA2: InitialAmmo, MaxAmmo when unset
			Ammo = Class->MaxAmmo; // RA2: as above
		} else {
			Ammo = Class->InitialAmmo; // RA2: as above
		}
		PrimaryFacing.Set_ROT(Class->ROT);

		/*
		**	If the building could never be built, then it can never be sold either. This
		**	is due to the lack of buildup animation.
		*/
		if (Class->Get_Buildup_Data() == NULL) {
			IsAllowedToSell = false;
			return; // RA2: returns here
		}
		Class->Free_Buildup_Data(); // RA2: as above
		HasBuildupData = true; // RA2: as above
	}
}


/***********************************************************************************************
 * BuildingClass::Drop_Debris -- Drops rubble when building is destroyed.                      *
 *                                                                                             *
 *    This routine is called when a building is destroyed. It handles                          *
 *    placing the rubble down.                                                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/14/1994 JLB : Created.                                                                 *
 *   06/13/1995 JLB : Added smoke and normal infantry survivor possibility.                    *
 *   07/16/1995 JLB : Survival rate depends on if captured or sabotaged.                       *
 *=============================================================================================*/
void BuildingClass::Drop_Debris(AbstractClass * source)
{
	Cell const * offset;
	Cell cell;

	/*
	**	Generate random survivors from the destroyed building.
	*/
	cell = PositionCell;
	offset = Occupy_List();
	int odds = 2;
	if (WhomToRepay != NULL) odds -= 1;
	if (IsCaptured) odds += 6;
	int count = How_Many_Survivors();
	if (count == 0) return;
	while (*offset != REFRESH_EOL) {
		Cell	newcell;

		newcell = cell + *offset++;
		CellClass const * cellptr = &Map[newcell];

		/*
		**	Infantry could run out of a destroyed building.
		*/
		if (!House->IsToDie && count > 0) {
			InfantryClass * i = NULL;

			if (Random_Pick(0, odds) == 1) {
				i = NULL;
				const InfantryTypeClass * typ = Crew_Type();
				if (typ != NULL) i = new InfantryClass(typ, House);
				DebugString("Creating survivor type '%s' from building type '%s'\n", (char *)i->Class->GivenName, (char *)Class->GivenName);
				if (i != NULL) {
					if (HasBuildupData && i->Class->IsNominal) i->IsTechnician = true;
					ScenarioInit++;
					Coord newcoord = Coord(newcell) + Coord(0, 36, 0);
					newcoord = Map[newcoord].Closest_Free_Spot(newcoord);

					if (newcoord != COORD_NONE && i->Unlimbo(newcoord, DIR_N)) {
						DebugString("Survivor unlimbo OK\n");
						count--;
						i->Strength = Random_Pick(5, (int)i->Class->MaxStrength);
						i->Scatter(COORD_NONE, true);
						if (source != NULL && !House->Is_Ally(source)) {
							i->QueueMission(MISSION_ATTACK, false); // RA2: QueueMission, not Assign_Mission
							i->Assign_Target(source);
						} else {
							if (House->Is_Human_Player()) {
								i->QueueMission(MISSION_MOVE, false); // RA2: QueueMission, not Assign_Mission
							} else {
								i->QueueMission(MISSION_HUNT, false); // RA2: QueueMission, not Assign_Mission
							}
						}
					} else {
						delete i;
					}
					ScenarioInit--;
				}
			}
		}

		/*
		**	Smoke and fire only appear on terrestrail cells. They should not appear on
		**	rivers, clifs, or water cells.
		*/
		if (cellptr->Is_Clear_To_Move(SPEED_TRACK, true, true)) {

			/*
			**	Possibly add some smoke rising from the ashes of the building.
			*/
			/*switch (Random_Pick(0, 5)) {
				case 0:
				case 1:
				case 2:
					new AnimClass(ANIM_SMOKE_M, Coord_Scatter(newcell.As_Coord(), 5 * CELL_LEPTON / 16, false), Random_Pick(0, 5), Random_Pick(1, 2));
					break;

				default:
					break;
			}*/

			/*
			**	The building always scars the ground in some fashion.
			*/
			if (Percent_Chance(50)) {
				SmudgeTypeClass::Scorch_The_Ground(Coord_Scatter(newcell, CELL_LEPTON / 2, false).As_Cell());
			} else {
				SmudgeTypeClass::Crater_The_Ground(Coord_Scatter(newcell, CELL_LEPTON / 2, false).As_Cell());
			}
		}
	}
}



/***********************************************************************************************
 * BuildingClass::Active_Click_With -- Handles clicking on the map while the building is selected.*
 *                                                                                             *
 *    This interface routine handles when the player clicks on the map while this building     *
 *    is currently selected. This is used to assign an override target to a turret or          *
 *    guard tower.                                                                             *
 *                                                                                             *
 * INPUT:   target   -- The target that was clicked upon.                                      *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/28/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool BuildingClass::Active_Click_With(ActionType action, ObjectClass * object, bool)
{
	if (action == ACTION_ATTACK) {
		if (object != NULL) {
			Player_Assign_Mission(MISSION_ATTACK, object, NULL, NULL); // RA2: with the two extra arguments
		}
	} else if (action == ACTION_SELF) { // RA2: an else-if chain, the cases leave through the end of the function
		if (Class->Is_Factory()) {
			OutList.Add(EventClass(Owner(), EventClass::PRIMARY, TargetClass(this)));
		}
		if (Class->GapGenerator) { // RA2: a gap generator and an occupied garrison unload on the target
			Player_Assign_Mission(MISSION_UNLOAD, object, NULL, NULL); // RA2: as above
		}
		if (Occupants.Count() > 0) { // RA2: as above
			Player_Assign_Mission(MISSION_UNLOAD, object, NULL, NULL); // RA2: as above
			return(true);
		}
	} else {
		if (action == ACTION_DETONATE) { // RA2: the attached bomb is detonated
			return(OutList.Add(EventClass(AttachedBomb->OwnerHouse->HeapID, EventClass::DETONATE, TargetClass(this))), true); // RA2: as above
		} else {
			if (action != ACTION_MOVE) { // RA2: as above
				return(true); // RA2: as above
			}
			RTTIType factory = Class->ToBuild; // RA2: as above
			if (factory == RTTI_INFANTRYTYPE || factory == RTTI_UNITTYPE || factory == RTTI_AIRCRAFTTYPE) { // RA2: a barracks, a war factory or an airfield
				Cell cell = object->Center_Coord().As_Cell(); // RA2: the rally point is the clicked object's cell
				Assign_Rally_Point(cell, true); // RA2: announced
				return(true); // RA2: as above
			}
			return(OutList.Add(EventClass(Owner(), EventClass::ARCHIVE, TargetClass(this), TargetClass(object))), true); // RA2: as above
		}
	}
	return(true);
}



/***********************************************************************************************
 * BuildingClass::Active_Click_With -- Handles cell selection for buildings.                   *
 *                                                                                             *
 *    This routine really only serves one purpose -- to allow targeting of the ground for      *
 *    buildings that are equipped with weapons.                                                *
 *                                                                                             *
 * INPUT:   action   -- The requested action to perform.                                       *
 *                                                                                             *
 *          cell     -- The cell location to perform the action upon.                          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/04/1995 JLB : Created.                                                                 *
 *   10/04/1995 JLB : Handles construction yard undeploy to move logic.                        *
 *=============================================================================================*/
bool BuildingClass::Active_Click_With(ActionType action, Cell const & cell, Cell const &, bool) // RA2: a second cell
{
	if (action == ACTION_ATTACK) {
		Player_Assign_Mission(MISSION_ATTACK, &Map[cell], NULL, NULL); // RA2: with the two extra arguments
		return(true);
	}

	if (action == ACTION_MOVE) {
		bool held = false; // RA2: a construction yard that may not redeploy keeps its rally point alone
		if (Class->IsConstructionYard && (!House->Is_Human_Player() || Session.Type == GAME_NORMAL || !Session.Options.MCVRedeploy)) { // RA2: as above
			held = true; // RA2: as above
		} else if (Class->UndeploysInto != NULL || Is_Move_Override()) { // RA2: as above
			Assign_Rally_Point(cell, true); // RA2: announced
		}
		if (Class->UndeploysInto != NULL && !held) { // RA2: as above
			OutList.Add(EventClass(Owner(), EventClass::SELL, TargetClass(this)));
		}
	}

	return(true);
}


/// <summary>
/// Assigns the rally point for the units this building produces.
/// The cell asked for is nudged to somewhere the produced object could actually reach,
/// and the result is submitted to the event queue rather than stored directly, so that
/// every machine in the game agrees on where the rally point ended up.
/// </summary>
/// <param name="cell">The map cell designated as the rally point.</param>
void BuildingClass::Assign_Rally_Point(Cell const & cell, bool announce) // RA2: announce
{
	SpeedType speed = SPEED_FOOT;
	MZoneType mzone = MZONE_NORMAL;

	bool underbridge = Map[cell].IsUnderBridge;

	if (Class->ToBuild == RTTI_AIRCRAFTTYPE) {
		speed = SPEED_WINGED;
		mzone = MZONE_FLYER;
	}
	if (Class->Naval) { // RA2: a naval yard's rally point is a water cell
		speed = SPEED_AMPHIBIOUS; // RA2: as above
		mzone = MZONE_AMPHIBIOUS_CRUSHER; // RA2: as above
	}

	int zone = Map.Get_Cell_Zone(Get_Coord().As_Cell(), mzone, underbridge);

	Cell nearbyloc = Map.Nearby_Location(cell, speed, zone, mzone, underbridge);

	if (nearbyloc != CELL_NONE) {
		OutList.Add(EventClass(Owner(), EventClass::ARCHIVE, TargetClass(this), TargetClass(&Map[nearbyloc])));
	} else if (Class->IsConstructionYard && House->Is_Human_Player() && Session.Type != GAME_NORMAL && Session.Options.MCVRedeploy) {
		OutList.Add(EventClass(Owner(), EventClass::ARCHIVE, TargetClass(this), TargetClass(&Map[Center_Coord()])));
		return; // RA2: no announcement then
	}
	if (announce && House->Is_Player_Control() && !Class->IsConstructionYard) { // RA2: the new rally point is announced
		Queue_Speech("EVA_NewRallyPointEstablished", -1, -1); // RA2: as above
	}
}


/***********************************************************************************************
 * BuildingClass::Assign_Target -- Assigns a target to the building.                           *
 *                                                                                             *
 *    Assigning of a target to a building makes sense if the building is one that can attack.  *
 *    This routine would be used to assign the attack target to a turret or guard tower.       *
 *                                                                                             *
 * INPUT:   target   -- The target that was clicked on while this building was selected.       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/28/1994 JLB : Created.                                                                 *
 *   11/02/1994 JLB : Checks for range before assigning target.                                *
 *=============================================================================================*/
void BuildingClass::Assign_Target(AbstractClass * target)
{
	if (CurrentMission == MISSION_DECONSTRUCTION) goto none; // RA2: Assign_Target at 0x440450 matches only so
	if (!IsPowerOnline()) { // RA2: as above
none: // RA2: as above
		target = NULL; // RA2: as above
	} else if (target != NULL && PrimaryWeapon != NULL && !PrimaryWeapon->Bullet->IsAntiAircraft && !IsCloseEnoughToAttack(target)) { // RA2: as above
		BASECLASS::Assign_Target(NULL); // RA2: as above
		if (Class->IsTickTank || Class->IsArtillary) { // RA2: as above
			if (!House->Is_Human_Player() && !IsUnderEMP()) { // RA2: as above
				QueueMission(MISSION_DECONSTRUCTION, false); // RA2: as above
				Commence(); // RA2: as above
			}
		}
		return; // RA2: as above
	}
	BASECLASS::Assign_Target(target);
}


/***********************************************************************************************
 * BuildingClass::Exit_Object -- Initiates an object to leave the building.                    *
 *                                                                                             *
 *    This function is used to cause an object to exit the building. It is called when a       *
 *    factory produces a vehicle or other mobile object and that object needs to exit the      *
 *    building to join the ranks of a regular unit. Typically, the object is placed down on    *
 *    the map such that it overlaps the building and then it is given a movement order so that *
 *    it will move to an adjacent free cell.                                                   *
 *                                                                                             *
 * INPUT:   base  -- Pointer to the object that is to exit the building.                       *
 *                                                                                             *
 * OUTPUT:  Returns the success rating for the exit attempt;                                   *
 *             0  = complete failure (refund money please)                                     *
 *             1  = temporarily prevented (try again later please)                             *
 *             2  = successful                                                                 *
 *                                                                                             *
 * WARNINGS:   The building is placed in radio contact with the object. The object is in a     *
 *             tethered condition. This condition will be automatically broken when the        *
 *             object reaches the adjacent square.                                             *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/28/1994 JLB : Created.                                                                 *
 *   04/10/1995 JLB : Handles building production by computer.                                 *
 *   06/17/1995 JLB : Handles refinery exit.                                                   *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: no independent bound yet (00440520 has no row)
int BuildingClass::Exit_Object(TechnoClass * base)
{
	if (!base) return(0);

	/*
	**	A unit exiting a building is always considered to be "locked". That means, it
	**	will be considered as to have legally entered the visible map domain.
	*/
	base->IsLocked = true;

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
					if (air->Unlimbo(Docking_Coord(NULL), air->Pose_Dir())) { // RA2: Docking_Coord and Exit_Coord take an argument
						Transmit_Message(RADIO_HELLO, air);
						Transmit_Message(RADIO_TETHER);

						if (ArchiveTarget != NULL) {
							air->Assign_Destination(ArchiveTarget);
							air->Assign_Mission(MISSION_MOVE);
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
						base->Assign_Mission(MISSION_MOVE);
					} else {
						cell = base->Nearby_Location(this);
						if (cell != CELL_NONE) {
							base->Assign_Destination(&Map[cell]);
							base->Assign_Mission(MISSION_MOVE);
						} else {
							base->Assign_Destination(NULL);
							base->Assign_Mission(MISSION_MOVE);
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
								int retval = (bldg->Exit_Object(base));
								bldg->Factory = NULL;
								Factory = temp;
								return(retval);
							}
						}
						return(1);	// fail while we're still unloading previous
					}
					ScenarioInit++;
					if (base->Unlimbo(Exit_Coord(0), DIR_E)) { // RA2: Docking_Coord and Exit_Coord take an argument
						base->Mark(MARK_UP);
						base->Set_Coord(Exit_Coord(0)); // RA2: Docking_Coord and Exit_Coord take an argument
						base->Mark(MARK_DOWN);
						Transmit_Message(RADIO_HELLO, base);
						Transmit_Message(RADIO_TETHER);
						Assign_Mission(MISSION_UNLOAD);
						ScenarioInit--;
						return(2);
					}

				} else if (Class->ToBuild != RTTI_INFANTRYTYPE && !Class->IsHospital && !Class->IsArmory) {

					Cell exitcell = Find_Exit_Cell(base);
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

						base->Assign_Mission(MISSION_MOVE);
						base->Assign_Destination(&Map[exitcell]);

						/*
						 * When the computer produces a unit, it has the unit guard an
						 * area around the spot it should head to.
						 */
						if (!House->Is_Human_Player()) {
							base->Assign_Mission(MISSION_GUARD_AREA);

							cell = House->Where_To_Go((FootClass *)base);
							base->Assign_Archive_Target((cell != CELL_NONE && Class->ToBuild != RTTI_NONE) ? &Map[cell] : NULL);
						}

						ScenarioInit--;
						return(2);
					}

				} else {

					base->ArchiveTarget = ArchiveTarget;
					Coord exitcoord;

					Cell exitcell = Find_Exit_Cell(base);
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

						base->Assign_Mission(MISSION_MOVE);
						base->Assign_Destination(&Map[exitcell]);

						/*
						 * When the computer produces infantry, it has the infantry guard
						 * an area around the spot it should head to.
						 */
						if (!House->Is_Human_Player()) {
							base->Assign_Mission(MISSION_GUARD_AREA);

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
						base->Assign_Mission(MISSION_HARVEST);
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
#endif


/***********************************************************************************************
 * BuildingClass::Update_Buildables -- Informs sidebar of additional construction options.     *
 *                                                                                             *
 *    This routine will tell the sidebar of objects that can be built. The function is called  *
 *    whenever a building matures.                                                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/11/1994 JLB : Created.                                                                 *
 *   12/23/1994 JLB : Only updates for PLAYER buildings.                                       *
 *=============================================================================================*/
void BuildingClass::Update_Buildables(void)
{
	if (House == PlayerPtr && !IsInLimbo && IsDiscoveredByPlayer && IsOn) {
		switch (Class->ToBuild) {
			StructType i;
			UnitType u;
			InfantryType f;
			AircraftType a;

			case RTTI_BUILDINGTYPE:
				for (i = STRUCT_FIRST; i < BuildingTypes.Count(); i++) {
					if (PlayerPtr->Can_Build((const ObjectTypeClass *)BuildingTypes[i], false, true)) {
						Map.Add(RTTI_BUILDINGTYPE, i);
					}
				}
				break;

			case RTTI_UNITTYPE:
				for (u = UNIT_FIRST; u < UnitTypes.Count(); u++) {
					if (PlayerPtr->Can_Build((const ObjectTypeClass *)UnitTypes[u], false, true)) {
						Map.Add(RTTI_UNITTYPE, u);
					}
				}
				break;

			case RTTI_INFANTRYTYPE:
				for (f = INFANTRY_FIRST; f < InfantryTypes.Count(); f++) {
					if (PlayerPtr->Can_Build((const ObjectTypeClass *)InfantryTypes[f], false, true)) {
						Map.Add(RTTI_INFANTRYTYPE, f);
					}
				}
				break;

			case RTTI_AIRCRAFTTYPE:
				for (a = AIRCRAFT_FIRST; a < AircraftTypes.Count(); a++) {
					if (PlayerPtr->Can_Build((const ObjectTypeClass *)AircraftTypes[a], false, true)) {
						Map.Add(RTTI_AIRCRAFTTYPE, a);
					}
				}
				break;

			default:
				break;
		}
	}
}


/***********************************************************************************************
 * BuildingClass::Limbo -- Handles power adjustment as building goes into limbo.               *
 *                                                                                             *
 *    This routine will handle the power adjustments for the associated house when the         *
 *    building goes into limbo. This means that its power drain or production is subtracted    *
 *    from the house accumulated totals.                                                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was the building limboed?                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool BuildingClass::Limbo(void)
{
	bool threatnode = false;
	Coord coord = PositionCoord; // RA2: as above
	Cell cell = coord;
	bool res = false; // RA2: as above
	int i;

	if (!IsInLimbo && Class != NULL) { // RA2: a building without a type has nothing to remove

		Remove_Damage_Fires(); // RA2: the damage fires go first

		if (Class->OrePurifier && HasOpened) { // RA2: the purifiers and docks of the house count only opened buildings
			House->NumOrePurifiers--; // RA2: as above
			if (House->NumOrePurifiers < 0) { // RA2: as above
				House->NumOrePurifiers = 0; // RA2: as above
			}
		}

		if (Class->IsHelipad && HasOpened) { // RA2: as above
			House->AirportDocks -= Class->NumberOfDocks; // RA2: as above
			if (House->AirportDocks < 0) { // RA2: as above
				House->AirportDocks = 0; // RA2: as above
			}
		}

		if (Class->IsLaserFencePost) {
			Update_Laser_Fence_Connections(false);
		}

		if (!ScenarioInit) {
			if (Class->IsSensorArray) {
				SensorArrayDeactivate(CELL_NONE); // RA2: the sensor array virtuals
			}
			if (Class->DetectDisguise) { // RA2: the disguise detector goes as well
				DisguiseDetectorDeactivate(CELL_NONE); // RA2: as above
			}
			if (!Considered_Vehicle()) {
				Release_Base_Area();
			}
		}

		for (i = 0; i < BUILDING_UPGRADE_MAX; i++) {
			if (Upgrades[i] != NULL && Upgrades[i]->IsThreatRatingNode) {
				threatnode = true;
				break;
			}
		}

		if (!GameActive) {
			res = true;
		}

		if (!res) {
			/*
			 * A wall tower that is removed from the map will cause the wall
			 * segments next to it to recalculate their connection state.
			 */
			if (Class == Rule->WallTower) {
				for (i = 0; i < FACING_COUNT; i += FACING_COUNT / 4) {
					Cell c = Adjacent_Cell(cell, (FacingType)i);
					CellClass * cptr = &Map[c];
					cptr->Wall_Update(false);
					if (cptr->Overlay != OVERLAY_NONE && OverlayTypes[cptr->Overlay]->IsWall && cptr->OverlayData < OVERLAYDATA_WALL_DAMAGE_STAGE) {
						cptr->Reduce_Wall(200);
					}
				}
			}

			/*
			 * Gates update the wall segments that they were connected to.
			 */
			if (Class == Rule->GDIGateOne || Class == Rule->NodGateOne) {
				Map[Cell(cell - Cell(1, 0))].Wall_Update(false);
				Map[Cell(cell + Cell(3, 0))].Wall_Update(false);
			}

			if (Class == Rule->GDIGateTwo || Class == Rule->NodGateTwo) {
				Map[Cell(cell - Cell(0, 1))].Wall_Update(false);
				Map[Cell(cell + Cell(0, 3))].Wall_Update(false);
			}

			if (Class->ToTile == NULL) {
				int w = Class->Width();
				int h = Class->Height(false); // RA2: without the bib
				Cell position = Get_Cell(); // RA2: through the virtual

				for (int y = 0; y < h + 2; y++) {
					for (int x = 0; x < w + 2; x++) {
						CellClass *cptr = &Map[(position - Cell(1, 1)) + Cell(x, y)];
						cptr->AdjacentObjectCount--;
					}
				}
			}

			if (TacticalMap != NULL) {
				TacticalMap->Register_Dirty_Area(Get_Render_Rect(), false); // RA2: as above
			}

			if (BuildingLight != NULL) {
				BuildingLight->Delete_Me();
			}

			/*
			**	Update the total factory type, assuming this building has a factory.
			*/
			House->Active_Remove(this);
			House->IsRecalcNeeded = true;
			House->Recalc_Center();

			/*
			**	Update the power status of the owner's house.
			*/
			if (House == PlayerPtr) {
				Map.PowerClass::IsToRedraw = true;
				Map.Flag_To_Redraw(GS_REDRAW_DIRTY); // RA2: as above
			}

			House->Invalidate_Base_Node_Position(this);
		}
	}

	if (!res) {
		res = BASECLASS::Limbo();

		if (threatnode) {
			House->Activate_Threat_Node();
		}

		if (Class->ToBuild != RTTI_NONE) {
			House->Update_Factories(Class->ToBuild, Class->Naval, 0); // RA2: with the naval flag and category 0
		}
	}

	if (Class->IsLaserFencePost && !ScenarioInit) {
		LaserFenceFrame = 0;
	}

	return(res);
}




/***********************************************************************************************
 * BuildingClass::Turret_Facing -- Fetches the turret facing for this building.                *
 *                                                                                             *
 *    This will return the turret facing for this building. Some buildings don't have a        *
 *    visual turret (e.g., pillbox) so they return a turret facing that always faces their     *
 *    current target.                                                                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the current facing of the turret.                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
DirType BuildingClass::Turret_Facing(void) const
{
	if (!Is_Turret_Equipped() && TarCom != NULL) {
		return(::Direction(Center_Coord(), TarCom->Center_Coord()));
	}
	return(PrimaryFacing.Current());
}


/***********************************************************************************************
 * BuildingClass::Greatest_Threat -- Searches for target that building can fire upon.          *
 *                                                                                             *
 *    This routine intercepts the Greatest_Threat function so that it can add the ability      *
 *    to search for ground targets, if this isn't a SAM site.                                  *
 *                                                                                             *
 * INPUT:   threat   -- The base threat control value. Typically, it might be THREAT_RANGE     *
 *                      or THREAT_NORMAL.                                                      *
 *                                                                                             *
 * OUTPUT:  Returns with a suitable target. If none could be found, then TARGET_NONE is        *
 *          returned instead.                                                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/01/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
AbstractClass * BuildingClass::Greatest_Threat(ThreatType threat, Coord const & coord, bool onlyenemy) const
{
	if (PrimaryWeapon != NULL) {
		threat = ThreatType(threat | PrimaryWeapon->Allowed_Threats());
	}
	if (SecondaryWeapon != NULL) {
		threat = ThreatType(threat | SecondaryWeapon->Allowed_Threats());
	}
	threat = ThreatType(threat | THREAT_RANGE); // RA2: Greatest_Threat at 0x442650 has no House, ThreatType
	return(BASECLASS::Greatest_Threat(threat, coord, onlyenemy));
}




/***********************************************************************************************
 * BuildingClass::Grand_Opening -- Handles construction completed special operations.          *
 *                                                                                             *
 *    This routine is called when construction has finished. Typically, this enables           *
 *    new production options for factories.                                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/08/1995 JLB : Created.                                                                 *
 *   06/13/1995 JLB : Added helipad.                                                           *
 *=============================================================================================*/
void BuildingClass::Grand_Opening(bool captured)
{
	if (HasOpened && !captured) { // RA2: RA2's form
		return; // RA2: as above
	}

	if (Class->ProduceCashDelay) { // RA2: the cash production timer (re)starts at every opening
		CashProductionTimer = CDTimerClass<FrameTimerClass>(Class->ProduceCashDelay); // RA2: as above
	}

	if (!HasOpened) { // RA2: as above
		if (Class->IsRefinery) { // RA2: a refinery shows how full its silos are
			int storage_stage = (int)Storage.Get_Total_Amount(); // RA2: as above
			if (storage_stage != 0) { // RA2: as above
				storage_stage = ((int)Storage.Get_Total_Amount() << 2) / Class->Capacity; // RA2: as above
			}
			StorageFilledSlots = storage_stage; // RA2: as above
			if (storage_stage >= 3) { // RA2: as above
				Begin_Anim(BANIM_ACTIVE_FOUR, HealthRatio <= Rule->ConditionYellow, false, 0); // RA2: as above
			} else if (storage_stage >= 2) { // RA2: as above
				Begin_Anim(BANIM_ACTIVE_THREE, HealthRatio <= Rule->ConditionYellow, false, 0); // RA2: as above
			} else if (storage_stage >= 1) { // RA2: as above
				Begin_Anim(BANIM_ACTIVE_TWO, HealthRatio <= Rule->ConditionYellow, false, 0); // RA2: as above
			} else if (storage_stage >= 0) { // RA2: as above
				Begin_Anim(BANIM_ACTIVE_ONE, HealthRatio <= Rule->ConditionYellow, false, 0); // RA2: as above
			}
		} else {
			Begin_Anim(BANIM_IDLE, HealthRatio <= Rule->ConditionYellow, Occupants.Count() > 0, 0); // RA2: as above
			Begin_Anim(BANIM_ACTIVE_ONE, HealthRatio <= Rule->ConditionYellow, Occupants.Count() > 0, 0); // RA2: as above
			Begin_Anim(BANIM_ACTIVE_TWO, HealthRatio <= Rule->ConditionYellow, false, 0); // RA2: as above
			Begin_Anim(BANIM_ACTIVE_THREE, HealthRatio <= Rule->ConditionYellow, false, 0); // RA2: as above
			Begin_Anim(BANIM_ACTIVE_FOUR, HealthRatio <= Rule->ConditionYellow, false, 0); // RA2: as above
		}

		if (Class->OrePurifier) { // RA2: as above
			House->NumOrePurifiers++; // RA2: as above
		}

		if (Class->IsHelipad) { // RA2: as above
			House->AirportDocks += Class->NumberOfDocks; // RA2: as above
			Map.Request_Column_Redraw(3); // RA2: the sidebar strip is redrawn
		}

		if (Class->SuperWeapon != -1) { // RA2: a super weapon building shows its charge on its animations
			for (int i = 0; i < House->SuperWeapon.Count(); i++) { // RA2: as above
				SuperClass * super = House->SuperWeapon[i]; // RA2: as above
				if (super->Class->Type == Class->SuperWeapon) { // RA2: as above
					if ((super->Control.Value() / 15) * 60 <= 4) { // RA2: as above
						Begin_Anim(BANIM_SUPER_THREE, HealthRatio <= Rule->ConditionYellow, Occupants.Count() > 0, 0); // RA2: as above
					} else {
						Begin_Anim(BANIM_SUPER_ONE, HealthRatio <= Rule->ConditionYellow, Occupants.Count() > 0, 0); // RA2: as above
					}
				}
			}
		}

		if (Is_Turret_Equipped() && !Class->IsTurretAnimAVoxel) { // RA2: as above
			if (TurretIndex != -1) { // RA2: as above
				int idx = TurretIndex; // RA2: as above
				TurretIndex = -1; // RA2: as above
				Set_Turret_Index(idx); // RA2: as above
			} else {
				Begin_Anim(BANIM_TURRET, false, false, 0); // RA2: as above
			}
			if (Is_Turret_Equipped() && Anims[BANIM_TURRET] != NULL) { // RA2: as above
				unsigned int frame = BodyShape[PrimaryFacing.Current().As_Dir32()]; // RA2: as above
				AnimClass * anim = Anims[BANIM_TURRET]; // RA2: as above
				if (anim != NULL) { // RA2: as above
					anim->Set_Stage(frame); // RA2: as above
					anim->Set_Step(0); // RA2: as above
				}
			}
		}

		if (Class->LightIntensity) { // RA2: as above
			if (LightSource == NULL) { // RA2: as above
				LightSource = new LightSourceClass(Center_Coord(), Class->LightVisibility, Class->LightIntensity, Class->LightRedTint, Class->LightGreenTint, Class->LightBlueTint); // RA2: as above
			}
			LightSource->Enable(false); // RA2: as above
		}
		if (Class->IsLaserFencePost) { // RA2: as above
			Toggle_Laser_Fence_Post(false); // RA2: as above
		}
		if (Class->IsSensorArray) { // RA2: as above
			DisguiseDetectorActivate(CELL_NONE); // RA2: the sensor array and disguise detector virtuals
		}
		if (Class->DetectDisguise) { // RA2: as above
			SensorArrayActivate(CELL_NONE); // RA2: as above
		}
		IsReadyToCommence = true; // RA2: as above
	}

	if (Class->RevealToAll && !House->Is_Player_Control()) { // RA2: a building that is revealed to everyone when it opens
		if (!captured) { // RA2: as above
			UpdateSight(false, false, true, PlayerPtr); // RA2: as above
			Coord coordinate = Get_Coord(); // RA2: as above
			Map.RevealArea3(coordinate, 0, LastSightRange + 3, 0); // RA2: as above
		}
		Submit_Radar_Event(RadarEventType(12), Cell(Get_Coord())); // RA2: as above
	}

	if (!House->Is_Player_Control() && MuteSWLaunches == 0 && Session.Type != GAME_NORMAL && (Class->SuperWeapon == -1 || !SuperWeapons[Class->SuperWeapon]->Class->AuxBuilding || House->ABQuantity.Value(SuperWeapons[Class->SuperWeapon]->Class->AuxBuilding->HeapID) != 0)) { // RA2: other players hear about the detected super weapon
		switch (Class->SuperWeapon) { // RA2: as above
			case 0: // RA2: as above
				Queue_Speech("EVA_NuclearSiloDetected", -1, -1); // RA2: as above
				break; // RA2: as above
			case 1: // RA2: as above
				Queue_Speech("EVA_IronCurtainDetected", -1, -1); // RA2: as above
				break; // RA2: as above
			case 2: // RA2: as above
				Queue_Speech("EVA_WeatherDeviceReady", -1, -1); // RA2: as above
				break; // RA2: as above
			case 3: // RA2: as above
				Queue_Speech("EVA_ChronosphereDetected", -1, -1); // RA2: as above
				break; // RA2: as above
		}
	}

	if (Class->IsWeaponsFactory && Class->Naval) { // RA2: a naval factory gets a rally point in the water
		Cell result = Map.Nearby_Location(Get_Cell(), SPEED_FLOAT, -1, MZONE_WATER, false, Point2D(5, 5), false, false, false, true, Cell(0, 0), false); // RA2: as above
		result = Cell(result.X + 2, result.Y + 2); // RA2: as above
		if (result != CELL_NONE) { // RA2: as above
			Assign_Rally_Point(result, false); // RA2: as above
		}
	}

	House->IsRecalcNeeded = true; // RA2: as above

	HasOpened = true; // RA2: as above

	/*
	**	Adjust the owning house according to the power, drain, and Tiberium capacity that
	**	this building has.
	*/
	House->RecalcPower = true; // RA2: as above
	House->RecalcRadar = true; // RA2: as above

	if (Techno_Type_Class()->GapGenerator) { // RA2: a gap generator starts jamming
		CreateGap(); // RA2: as above
	}

	/*	SPECIAL CASE:
	**	Tiberium Refineries get a free harvester. Add a harvester to the
	**	reinforcement list at this time.
	*/
	if (Class->FreeUnit != NULL && !ScenarioInit && !captured && !Debug_Map && (!House->Is_Human_Player() || PurchasePrice == 0 || PurchasePrice > Class->Raw_Cost())) { // RA2: as above
		Cell origin = Cell(Center_Coord()); // RA2: as above
		Cell cell = origin + AdjacentCell[FACING_S]; // RA2: as above

		bool placed = false; // RA2: as above
		UnitClass * unit = new UnitClass(Class->FreeUnit, House); // RA2: as above
		if (unit != NULL) { // RA2: as above

			/*
			**	Try to place down the harvesters. If it could not be placed, then try
			**	to place it in a nearby location.
			*/
			if (!unit->Unlimbo(Coord(cell), (Dir256)0xC0)) { // RA2: Unlimbo takes a coordinate
				cell = Map.Nearby_Location(Cell(Get_Coord()), SPEED_WHEEL, Map.Get_Cell_Zone(Cell(Get_Coord()), unit->Class->MZone, false), unit->Class->MZone, false, Point2D(1,1), true, true, false, false, Cell(0, 0), false); // RA2: as above

				if (cell == CELL_NONE || !unit->Unlimbo(Coord(cell), (Dir256)0xA0)) { // RA2: as above
					Cell newcell = Map.Nearby_Location(Cell(Get_Coord()), SPEED_WHEEL, Map.Get_Cell_Zone(Cell(Get_Coord()), unit->Class->MZone, false), unit->Class->MZone, false, Point2D(1,1), false, true, false, false, Cell(0, 0), false); // RA2: as above

					/*
					**	If the harvester could still not be placed, then refund the money
					**	to the owner and then bail.
					*/
					if (newcell == CELL_NONE || !unit->Unlimbo(Coord(newcell), (Dir256)0xA0)) { // RA2: as above
						House->Refund_Money(unit->Class->GetRefund(House, true)); // RA2: as above
						delete unit; // RA2: as above
					} else {
						placed = true;
					}
				} else {
					placed = true;
				}
			} else {
				placed = true; // RA2: as above
			}

			if (placed) { // RA2: as above
				unit->QueueMission(MISSION_HARVEST, false); // RA2: QueueMission
				unit->Commence(); // RA2: as above
			}
		} else {

			/*
			**	If the harvester could not be created in the first place, then give
			**	the full refund price to the owning player.
			*/
			House->Refund_Money(Class->FreeUnit->GetRefund(House, true)); // RA2: as above
		}
	}

	/*
	**	Helicopter pads get a free attack helicopter.
	*/
	if (!Rule->IsSeparate && Class->IsHoverPad && !captured) { // RA2: as above
		ScenarioInit++; // RA2: as above
		AircraftClass * air = new AircraftClass(Rule->PadAircraft[0], House); // RA2: as above
		if (air) { // RA2: as above
			air->Set_Height_AGL(0); // RA2: as above
			if (air->Unlimbo(Center_Coord(), air->Pose_Dir())) { // RA2: as above
				air->QueueMission(MISSION_GUARD, false); // RA2: as above
				air->Transmit_Message(RADIO_HELLO, this); // RA2: as above
				Transmit_Message(RADIO_TETHER, air); // RA2: as above
			}
		}
		ScenarioInit--; // RA2: as above
	}
}



/***********************************************************************************************
 * BuildingClass::Repair -- Initiates or terminates the repair process.                        *
 *                                                                                             *
 *    This routine will start, stop, or toggle the repair process. When a building repairs, it *
 *    occurs incrementally over time.                                                          *
 *                                                                                             *
 * INPUT:   control  -- Determines how to control the repair process.                          *
 *                      0: Turns repair process off (if it was on).                            *
 *                      1: Turns repair process on (if it was off).                            *
 *                      -1:Toggles repair process to other state.                              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void BuildingClass::Repair(int control)
{
	switch (control) {
		case -1:
			IsRepairing = (IsRepairing == false);
			break;

		case 0:
			if (!IsRepairing) return;
			IsRepairing = false;
			break;

		case 1:
			if (IsRepairing) return;
			IsRepairing = true;
			break;
	}

	VocType soundid; // RA2: RA2's form
	if (IsRepairing) {
		if (Strength == Class->MaxStrength) {
			soundid = Rule->ScoldSound;
		} else {
			soundid = Rule->GenericClick;
			if (House->Is_Player_Control()) {
				Clicked_As_Target();
			}
			IsWrenchVisible = true;
			if (House->Is_Player_Control()) {
				Queue_Speech("EVA_Repairing", -1, -1); // RA2: as above
			}
		}
	} else {
		soundid = Rule->GenericClick;
	}
	if (House->Is_Player_Control()) {
		VocClass::PlayAt(soundid, PositionCoord); // RA2: as above
	}
}



/***********************************************************************************************
 * BuildingClass::Sell_Back -- Controls the sell back (demolish) operation.                    *
 *                                                                                             *
 *    This routine will initiate or stop the sell back process for a building. It is called    *
 *    when the player clicks on a building when the sell mode is active.                       *
 *                                                                                             *
 * INPUT:   control  -- The action to perform. 0 = turn deconstruction off, 1 = deconstruct,   *
 *                      -1 = toggle deconstruction state.                                      *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void BuildingClass::Sell_Back(int control)
{
	if (HasBuildupData) {
		bool decon = false;
		switch (control) {
			case -1:
				decon = (Get_Mission() != MISSION_DECONSTRUCTION); // RA2: RA2's form
				break;

			case 0:
				if (Get_Mission() != MISSION_DECONSTRUCTION) return; // RA2: as above
				break;

			case 1:
				if (Get_Mission() == MISSION_DECONSTRUCTION) return; // RA2: as above
				if (IsGoingToBlow) return;
				decon = true;
				break;
		}

		if (decon) {
			QueueMission(MISSION_DECONSTRUCTION, false); // RA2: as above
			Commence();
		}
		if (House->Is_Player_Control()) {
			VocClass::PlayGlobal(Rule->GenericClick, 1.0f, 0x2000, 0); // RA2: as above
		}
	} else if (Class->IsFirestormWall) { // RA2: as above
		Class->Cost_Of(House); // RA2: as above
		House->Is_Human_Player(); // RA2: as above
		Limbo(); // RA2: as above
		Delete_Me(); // RA2: as above
	}
}



/***********************************************************************************************
 * BuildingClass::What_Action -- Determines action to perform if click on specified object.    *
 *                                                                                             *
 *    This routine will determine what action to perform if the mouse was clicked on the       *
 *    object specified. This determination is used to control the mouse imagery and the        *
 *    function process when the mouse button is pressed.                                       *
 *                                                                                             *
 * INPUT:   object   -- Pointer to the object that, if clicked on, will control what action    *
 *                      is to be performed.                                                    *
 *                                                                                             *
 * OUTPUT:  Returns with the ActionType that will occur if the mouse is clicked over the       *
 *          object specified while the building is currently selected.                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: 715 bytes differ (adopted text)
ActionType BuildingClass::What_Action(ObjectClass const * object, bool disallow_force) const
{
	if (Class->IsInvisibleInGame) {
		return(ACTION_NONE);
	}

	if (object->RTTI == RTTI_BUILDING && ((BuildingClass *)object)->Class->IsInvisibleInGame) {
		return(ACTION_NONE);
	}

	ActionType action = BASECLASS::What_Action(object, disallow_force);

	if (action == ACTION_SELF) {
		int index;
		if (StunDuration == 0 && Class->Is_Factory() && PlayerPtr == House && *House->Factory_Counter(Class->ToBuild) > 1) {
			switch (Class->ToBuild) {
				case RTTI_INFANTRYTYPE:
				case RTTI_INFANTRY:
					action = ACTION_NONE;
					for (index = 0; index < Buildings.Count(); index++) {
						BuildingClass *bldg = Buildings[index];
						if (bldg != this && bldg->House == House && bldg->Class->ToBuild == RTTI_INFANTRYTYPE) {
							action = ACTION_SELF;
							break;
						}
					}
					break;

				case RTTI_NONE:
					action = ACTION_NONE;
					break;

				default:
					break;
			}

		} else {
			action = ACTION_NONE;
		}
	}

	/*
	**	Don't allow targeting of SAM sites, even if the CTRL key
	**	is held down. Also don't allow targeting if the object is too
	**	far away.
	*/
	if (action == ACTION_ATTACK && PrimaryWeapon != NULL) {
		if (!In_Range((ObjectClass *)object, 0) || !PrimaryWeapon->Bullet->IsAntiGround) {
			action = ACTION_NONE;
			action = ACTION_NONE; // RA2: no IsLimpetMine (not in RA2's layout)
		}
		if (CurrentMission == MISSION_DECONSTRUCTION) {
			action = ACTION_NONE;
		}
	}

	if (action == ACTION_MOVE || action == ACTION_NOMOVE) {
		if (!Can_Player_Move()) {
			action = ACTION_SELECT;
		} else if (Class->ToBuild == RTTI_INFANTRYTYPE || Class->ToBuild == RTTI_UNITTYPE || Class->ToBuild == RTTI_AIRCRAFTTYPE) {
			bool altdown = (Keyboard->Down(Options.KeyForceMove1) || Keyboard->Down(Options.KeyForceMove2));
			if (!altdown) {
				action = ACTION_SELECT;
			} else {
				Cell cell = object->Center_Coord().As_Cell();
				if (Class->ToBuild != RTTI_AIRCRAFTTYPE) {
					if (!Is_In_Same_Zone(cell)) {
						action = ACTION_NOMOVE;
					}
					if (!Map[cell].IsUnderBridge && Map[cell].Passability != PASSABLE_LAND) {
						action = ACTION_NOMOVE;
					}
				}
			}
		}
	}

	return(action);
}
#endif



/***********************************************************************************************
 * BuildingClass::What_Action -- Determines what action will occur.                            *
 *                                                                                             *
 *    This routine examines the cell specified and returns with the action that will be        *
 *    performed if that cell were clicked upon while the building is selected.                 *
 *                                                                                             *
 * INPUT:   cell  -- The cell to examine.                                                      *
 *                                                                                             *
 * OUTPUT:  Returns the ActionType that indicates what should occur if the mouse is clicked    *
 *          on this cell.                                                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: 436 bytes differ (adopted text)
ActionType BuildingClass::What_Action(Cell const & cell, bool check_fog, bool disallow_force) const
{
	if (Class->IsInvisibleInGame) {
		return(ACTION_NONE);
	}

	ActionType action;

	if (Class->UndeploysInto != NULL && check_fog) {
		action = BASECLASS::What_Action(cell, false, disallow_force);
	} else {
		action = BASECLASS::What_Action(cell, check_fog, disallow_force);
	}


	/*
	**	Don't allow targeting of SAM sites, even if the CTRL key
	**	is held down.
	*/
	if (action == ACTION_ATTACK && PrimaryWeapon != NULL) { // RA2: no ACTION_RALLY_TO_POINT (not in RA2's layout)
		if (!PrimaryWeapon->Bullet->IsAntiGround) {
			action = ACTION_NONE;
			action = ACTION_NONE; // RA2: no IsLimpetMine (not in RA2's layout)
		}
		if (CurrentMission == MISSION_DECONSTRUCTION) {
			action = ACTION_NONE;
		}
	}

	return(action);
}
#endif


/***********************************************************************************************
 * BuildingClass::Begin_Mode -- Begins an animation mode for the building.                     *
 *                                                                                             *
 *    This routine will start the building animating. This animation will loop indefinitely    *
 *    until explicitly stopped.                                                                *
 *                                                                                             *
 * INPUT:   bstate   -- The animation state to initiate.                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   The building graphic state will reflect the first stage of this animation the   *
 *             very next time it is rendered.                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Created.                                                                 *
 *   07/02/1995 JLB : Uses normalize animation rate where applicable.                          *
 *=============================================================================================*/
void BuildingClass::Begin_Mode(BStateType bstate)
{
	QueueBState = bstate;
	if (BState == BSTATE_NONE || bstate == BSTATE_CONSTRUCTION || ScenarioInit) {
		QueueBState = BSTATE_NONE; // RA2: cleared before BState is set
		BState = bstate;
		BuildingTypeClass::AnimControlType const * ctrl = Fetch_Anim_Control();

		if (ScenarioInit) { // RA2: the idle animation and the four active animations start together, garrisoned when occupied
			Begin_Anim(BANIM_IDLE, HealthRatio <= Rule->ConditionYellow, Occupants.Count() > 0, 0); // RA2: as above
			if (ScenarioInit) {
				Begin_Anim(BANIM_ACTIVE_ONE, HealthRatio <= Rule->ConditionYellow, Occupants.Count() > 0, 0); // RA2: as above
				if (ScenarioInit) {
					Begin_Anim(BANIM_ACTIVE_TWO, HealthRatio <= Rule->ConditionYellow, Occupants.Count() > 0, 0); // RA2: as above
					if (ScenarioInit) {
						Begin_Anim(BANIM_ACTIVE_THREE, HealthRatio <= Rule->ConditionYellow, Occupants.Count() > 0, 0); // RA2: as above
						if (ScenarioInit) {
							Begin_Anim(BANIM_ACTIVE_FOUR, HealthRatio <= Rule->ConditionYellow, Occupants.Count() > 0, 0); // RA2: as above
						}
					}
				}
			}
		}

		int rate = ctrl->Rate;
		if (Class->Normalized && bstate != BSTATE_CONSTRUCTION) { // RA2: the options' normalized delay for all but the construction state
			rate = Options.Normalize_Delay(rate); // RA2: as above
		}
		Set_Rate(rate);
		Set_Stage(ctrl->Start);
	}
}


/***********************************************************************************************
 * BuildingClass::Center_Coord -- Fetches the center coordinate for the building.              *
 *                                                                                             *
 *    This routine is used to set the center coordinate for this building.                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the coordinate for the center location for the building.              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/10/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
Coord BuildingClass::Center_Coord(void) const
{
	int h = (Class->Height(false) * (CELL_LEPTON_H / 2)) - (CELL_LEPTON_H / 2); // RA2: Center_Coord at 0x444080 matches only so
	int w = (Class->Width() * (CELL_LEPTON_W / 2)) - (CELL_LEPTON_W / 2); // RA2: as above

	return(PositionCoord + Coord(w, h));
}


/***********************************************************************************************
 * BuildingClass::Docking_Coord -- Fetches the coordinate to use for docking.                  *
 *                                                                                             *
 *    This routine will return the coordinate to use when an object wishes to dock with this   *
 *    building. Normally the docking coordinate would be the center of the building.           *
 *    Exceptions to this would be the airfield and helipad. Their docking coordinates are      *
 *    offset to match the building artwork.                                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the coordinate to head to when trying to dock with this building.     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: 399 bytes differ (adopted text)
Coord BuildingClass::Docking_Coord(TechnoClass * docker) const // RA2: the docking object
{
	if (Class->IsWeeder) {
		Cell cell = PositionCell + Cell(2, 1);
		Coord coord(cell); // RA2: the Coord constructor, not Cell::As_Coord
		coord.Z = Coord(PositionCoord).Z; // RA2: the height is read from a copy of the position
		return(coord);
	}
	if (Class->IsRefinery) {
		Coord coord = ObjectClass::Docking_Coord(docker); // RA2: the docker is passed on to ObjectClass
		return(coord + Coord(CELL_LEPTON_W/2, 0, 0));
	}
	if (!Class->IsHelipad && !Class->IsCanUnitRepair) { // RA2: other buildings dock at the center
		return(ObjectClass::Docking_Coord(docker)); // RA2: as above
	}
	switch (Class->NumberOfDocks) { // RA2: helipads and unit repair bays have docking offsets
		case 0:
			return(Center_Coord());

		case 1:
			return(Center_Coord() + Class->DockingOffsets[0]); // RA2: as above

		default:
			{
				int index = FindLinkIndex(docker); // RA2: the docker's radio link picks the offset
				if (index >= 0 && index < Class->NumberOfDocks) { // RA2: as above
					return(Center_Coord() + Class->DockingOffsets[index]); // RA2: as above
				}
			}
			return(Center_Coord());
	}
}
#endif









/// <summary>
/// Fetches how far the docking object is from the point it docks at on this building.
/// </summary>
/// <param name="docker">The object that docks with the building.</param>
/// <returns>Returns with the distance in leptons.</returns>
int BuildingClass::DistanceToDockingCoord(TechnoClass * docker) const // RA2: not in TibSun, at 0x4442B0
{
	return(Docking_Coord(docker).Distance_To(docker->Center_Coord()));
}





/// <summary>
/// Fetches the coordinate that an object should travel to.
/// This routine is used when something has been ordered to this building. A helipad
/// sends the visitor to its docking spot; every other building is approached at its
/// center.
/// </summary>
/// <returns>Returns with the coordinate to move to.</returns>
Coord BuildingClass::Destination_Coord(TechnoClass * docker) const // RA2: the docking object
{
	if (Class->IsHelipad) {
		return(Docking_Coord(docker)); // RA2: the docker's spot, on a helipad
	}
	if (Class->IsCanUnitRepair) { // RA2: as above
		return(Docking_Coord(docker)); // RA2: and on a repair depot
	}
	return(Center_Coord());
}



/***********************************************************************************************
 * BuildingClass::Can_Fire -- Determines if this building can fire.                            *
 *                                                                                             *
 *    Use this routine to see if the building can fire its weapon.                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   target   -- The target that firing upon is desired.                                *
 *                                                                                             *
 *          which    -- Which weapon to use when firing. 0=primary, 1=secondary.               *
 *                                                                                             *
 * OUTPUT:  Returns with the fire possibility code. If firing is allowed, then FIRE_OK is      *
 *          returned. Other cases will result in appropriate fire code value that indicates    *
 *          why firing is not allowed.                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/03/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
FireErrorType BuildingClass::Can_Fire(AbstractClass * target, int which, bool ignoreRange) const // RA2: ignoreRange
{
	if (Class->CanBeOccupied && Occupants.Count() == 0) { // RA2: a garrison without occupants has nothing to fire
		return(FIRE_ILLEGAL); // RA2: as above
	}

	if (Class->IsEMPulseCannon) {
		return(FIRE_CANT);
	}

	if (!IsPowerOnline()) { // RA2: Is_Powered_On is the IsPowerOnline virtual
		return(FIRE_CANT);
	}

	if (DelayBeforeFiring != 0) { // RA2: a delayed shot is pending
		return(FIRE_REARM); // RA2: as above
	}

	FireErrorType canfire = BASECLASS::Can_Fire(target, which, ignoreRange); // RA2: the range flag is passed on

	if (canfire == FIRE_OK) {

		/*
		**	Double check to make sure that the facing is roughly toward
		**	the target. If the difference is too great, then firing is
		**	temporarily postponed.
		*/
		if (Is_Turret_Equipped()) {
			DirType towards = Aim_Direction(target);
			DirType by = Class->IsTurretAnimAVoxel ? DIR_MIN : DIR_STEP_32;
			if (!PrimaryFacing.Current().Is_Complete_Turn(towards, by)) {
				return(FIRE_FACING);
			}
		}
	}
	return(canfire); // RA2: no power or charge checks
}


/// <summary>
/// Makes this factory the primary one of its kind (land or naval) for its house, unless
/// another one already is and the caller does not ask to take over from it.
/// </summary>
/// <param name="take">Should this building take the primary status from the current primary factory?</param>
/// <returns>Returns with whether this building's primary status changed.</returns>
bool BuildingClass::Become_Primary(bool take) // RA2: not in TibSun, at 0x4444B0
{
	RTTIType tobuild = Class->ToBuild;
	if (tobuild == RTTI_NONE) {
		return(false);
	}

	bool was = IsLeader;
	int count = House->Buildings.Count();
	for (int index = 0; index < count; index++) {
		BuildingClass * building = House->Buildings[index];
		if (!building->IsInLimbo && building->IsLeader && building->Class->ToBuild == tobuild) {
			if ((Class->Naval && building->Class->Naval) || (!Class->Naval && !building->Class->Naval)) {
				if (take) {
					building->IsLeader = false;
					IsLeader = true;
				}
				goto done;
			}
		}
	}
	IsLeader = true;

done:
	return(IsLeader != was);
}





/***********************************************************************************************
 * BuildingClass::Toggle_Primary -- Toggles the primary factory state.                         *
 *                                                                                             *
 *    This routine will change the primary factory state of this building. The primary         *
 *    factory is the one that units will be produced from (by default).                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Is this building NOW the primary factory?                                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/03/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool BuildingClass::Toggle_Primary(void)
{
	if (Class->ToBuild != RTTI_NONE) { // RA2: RA2's form
		if (IsLeader) { // RA2: as above
			IsLeader = false; // RA2: as above
		} else { // RA2: as above
			int count = House->Buildings.Count(); // RA2: as above
			for (int index = 0; index < count; ++index) { // RA2: as above
				BuildingClass * building = House->Buildings[index]; // RA2: as above
				if (!building->IsInLimbo && building->Class->ToBuild == Class->ToBuild) { // RA2: as above
					if ((Class->Naval && building->Class->Naval) || (!Class->Naval && !building->Class->Naval)) { // RA2: as above
						building->IsLeader = false; // RA2: as above
					}
				}
			}
			IsLeader = true; // RA2: as above
			if (House->Is_Player_Control()) { // RA2: as above
				Queue_Speech("EVA_PrimaryBuildingSelected", -1, -1); // RA2: as above
			}
		}
		Mark(MARK_CHANGE); // RA2: as above
	}
	return(IsLeader);
}



struct ReestablishRecord { // RA2: the 8-byte record Captured allocates per captured contact (contact, was-tethered flag)
	TechnoClass * Contact; // RA2: as above
	bool WasTethered; // RA2: as above
	ReestablishRecord(TechnoClass * contact, bool tethered) : Contact(contact), WasTethered(tethered) {} // RA2: as above
};

/***********************************************************************************************
 * BuildingClass::Captured -- Captures the building.                                           *
 *                                                                                             *
 *    This routine will change the owner of the building. It handles updating any related      *
 *    game systems as a result. Factories are the most prone to have great game related        *
 *    consequences when captured. This could also affect the sidebar and building ownership.   *
 *                                                                                             *
 * INPUT:   newowner -- Pointer to the house that is now the new owner.                        *
 *                                                                                             *
 * OUTPUT:  Was the capture attempt successful?                                                *
 *                                                                                             *
 * WARNINGS:   Capturing could fail if the house is already owned by the one specified or      *
 *             the building isn't allowed to be captured.                                      *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/03/1995 JLB : Created.                                                                 *
 *   07/05/1995 JLB : Fixed production problem with capturing enemy buildings.                 *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: 2366 bytes differ (adopted text)
bool BuildingClass::Captured(HouseClass * newowner, bool announce) // RA2: announce
{
	if (newowner != House) {

		if (AttachedBomb && !Class->CanBeOccupied) { // RA2: a bomb on a building that cannot be occupied is disarmed
			AttachedBomb->Disarm(); // RA2: as above
		}

		if (Class->IsCloakGenerator) {
			newowner->HasCloakGenerator = true;
		}

		if (House->Class->IsMultiplayPassive && Class->ProduceCashStartup) { // RA2: the capturing house is paid the startup cash and the cash timer restarts
			newowner->Refund_Money(Class->ProduceCashStartup); // RA2: as above
			CashProductionTimer = CDTimerClass<FrameTimerClass>(Class->ProduceCashDelay); // RA2: as above
		}

		/*
		**	Make sure the capturer isn't spying on his own building, and if
		**	it was a radar facility, update the target house's RadarSpied field.
		*/
		if (SpiedBy & (1<<(newowner->Class->House)) ) {
			SpiedBy -= (1<<(newowner->Class->House));
			if (Class->IsRadar) {
				Update_Radar_Spied();
			}
		}

		IsLeader = false;

		if (Class->IsLaserFencePost) {
			Update_Laser_Fence_Connections(0);
		}

		if (House->Is_Player_Control() || newowner->Is_Player_Control()) {
			if (!newowner->Class->IsMultiplayPassive && announce) { // RA2: EVA announces by name
				if (Class->NeedsEngineer) { // RA2: as above
					if (House != newowner) { // RA2: as above
						if (House->Is_Player_Control()) { // RA2: as above
							Queue_Speech("EVA_TechBuildingLost", -1, -1); // RA2: as above
						}
						if (newowner->Is_Player_Control()) { // RA2: as above
							Queue_Speech("EVA_TechBuildingCaptured", -1, -1); // RA2: as above
						}
					}
				} else {
					if (Submit_Radar_Event(RadarEventType(10), Get_Cell())) { // RA2: as above
						Queue_Speech("EVA_BuildingCaptured", -1, -1); // RA2: as above
					}
				}
			}
			Map.PowerClass::IsToRedraw = true;
			Map.Flag_To_Redraw(GS_REDRAW_DIRTY);
		}

		IsToDisplay = true;
		HasEngineer = true; // RA2: the capturer's engineer now stays inside

		if (Class->NeedsEngineer) { // RA2: a captured tech building powers on
			Power_On(); // RA2: as above
		}

		/*
		**	Add this building to the list of buildings captured this game. For internet stats purposes.
		*/
		if (Session.Type == GAME_INTERNET) {
			newowner->CapturedBuildings.Increment_Unit_Total (Class->HeapID); // RA2: the house trackers are members, not pointers
		}

		/*
		**	If there is something loaded, then it gets captured as well.
		*/
		TechnoClass * tech = Cargo.Attached_Object();
		if (tech) tech->Captured(newowner, true); // RA2: announce

		/*
		**	If something isn't technically attached, but is sitting on this
		**	building for another reason (e.g., helicopter on helipad), then it
		**	gets captured as well.
		*/
		DynamicVectorClass<ReestablishRecord *> reestablish; // RA2: RA2 walks every radio link and remembers the captured ones
		int i; // RA2: as above
		for (i = 0; i < RadioLinks.Length(); i++) { // RA2: as above
			TechnoClass * contact = Radio_Link(i); // RA2: as above
			if (contact) { // RA2: as above
				bool captured = false; // RA2: as above
				bool was_tethered = false; // RA2: as above
				if (!(contact->AbstractFlags & 4)) { // RA2: only units are taken along (bit 2 of the abstract flags, tested in memory)
					contact = NULL; // RA2: as above
				} else if (Transmit_Message(RADIO_NEED_TO_MOVE, contact) == RADIO_ROGER && (Class->IsWeaponsFactory || contact->Distance(Docking_Coord(contact)) < CELL_LEPTON / 4) ) { // RA2: Docking_Coord takes the docker
					was_tethered = contact->IsTethered; // RA2: as above
					contact->Captured(newowner, true); // RA2: announce
					captured = true; // RA2: as above
				}
				if (!captured) { // RA2: as above
					Transmit_Message(RADIO_RUN_AWAY, contact); // RA2: as above
					Transmit_Message(RADIO_OVER_OUT, contact); // RA2: as above
				} else {
					ReestablishRecord * record = new ReestablishRecord(contact, was_tethered); // RA2: as above
					reestablish.Add(record); // RA2: as above
				}
			}
		}

		/*
		**	Abort any computer production in progress.
		*/
		if (Factory) {
			Factory->Abandon();
			delete (FactoryClass *)Factory;
			Factory = NULL;
		}

		/*
		**	Decrement the factory counter for the original owner.
		*/
		House->Active_Remove(this);

		HouseClass * oldowner = House;
		TargetClass tocap = this;

		IsCaptured = true;
		if (Rule->BuildConst.Is_In_List(Class)) { // RA2: any construction yard type
			oldowner->ConYards.Delete(this);
		}
		if (Class->IsCanUnitRepair) { // RA2: the new house lists
			oldowner->UnitRepairStations.Delete(this); // RA2: as above
		}
		if (Class->Cloning) { // RA2: as above
			oldowner->CloningVats.Delete(this); // RA2: as above
		}
		if (Class->PsychicDetectionRadius > 0) { // RA2: as above
			oldowner->PsychicDetectionBuildings.Delete(this); // RA2: as above
		}
		oldowner->Buildings.Delete(this); // RA2: as above
		if (Class->OrePurifier) { // RA2: as above
			oldowner->NumOrePurifiers--; // RA2: as above
		}
		if (Class->IsHelipad) { // RA2: as above
			oldowner->AirportDocks -= Class->NumberOfDocks; // RA2: as above
		}

		if (Class->IsCloakGenerator) {
			Disable_Cloak_Generator();
			CurrentCloakRadius = 1;
			Cloaking_AI(true);
		}
		if (Class->GapGenerator) { // RA2: as above
			DestroyGap(); // RA2: as above
		}
		RemoveSight(false, false, false, NULL); // RA2: the old owner loses the sight

		BASECLASS::Captured(newowner, true); // RA2: announce

		if (Class->IsCloakGenerator && IsPowerOnline()) { // RA2: Is_Powered_On is the IsPowerOnline virtual
			Enable_Cloak_Generator();
		}
		if (Class->GapGenerator && IsPowerOnline()) { // RA2: as above
			CreateGap(); // RA2: as above
		}

		Become_Primary(false); // RA2: as above
		oldowner->ToCapture = this;
		oldowner->Recalc_Center();
		House->Recalc_Center();

		if (House->ToCapture == this) {
			House->ToCapture = NULL;
		}

		if (oldowner->Is_Player_Control() && Class->IsConstructionYard) { // RA2: as above
			Map.PendingObjectPtr = NULL;
			Map.PendingObject = NULL;
			Map.PendingHouse = HOUSE_NONE;
			Map.Set_Cursor_Shape(NULL);
		}

		/*
		**	Increment the factory count for the new owner.
		*/
		House->Active_Add(this);

		IsRepairing = false;
		Grand_Opening(true);

		if (Class->IsLaserFencePost) {
			Init_Laser_Fence();
			Init_Laser_Fence_Frame();
		}

		/*
		**	Perform a look operation when captured if it was the player
		**	that performed the capture.
		*/
		if (House->Is_Player_Control()) {
			UpdateSight(false, false, false, NULL); // RA2: the new owner gains the sight
			Map.Flag_To_Redraw(GS_REDRAW_TACTICAL); // RA2: as above
		}

		/*
		**	If it was spied upon by the player who just captured it, clear the
		**	spiedby flag for that house.
		*/
		if (SpiedBy & (1 << (newowner->Class->House))) {
			SpiedBy &= ~(1 << (newowner->Class->House));
		}

		Update_Anim_Appearance();

		if (Rule->BuildConst.Is_In_List(Class)) { // RA2: as above
			newowner->ConYards.Add(this);
		}
		if (Class->IsCanUnitRepair) { // RA2: as above
			newowner->UnitRepairStations.Add(this); // RA2: as above
		}
		if (Class->Cloning) { // RA2: as above
			newowner->CloningVats.Add(this); // RA2: as above
		}
		if (Class->PsychicDetectionRadius > 0) { // RA2: as above
			newowner->PsychicDetectionBuildings.Add(this); // RA2: as above
		}
		newowner->Buildings.Add(this); // RA2: as above
		if (Class->OrePurifier) { // RA2: as above
			House->NumOrePurifiers++; // RA2: as above
		}
		if (Class->IsHelipad) { // RA2: as above
			House->AirportDocks += Class->NumberOfDocks; // RA2: as above
		}

		if (Class->ToBuild != RTTI_NONE) {
			oldowner->Update_Factories(Class->ToBuild, Class->Naval, 0); // RA2: ship flag and category
			newowner->Update_Factories(Class->ToBuild, Class->Naval, 0); // RA2: as above
		}

		for (i = 0; i < reestablish.Count(); i++) { // RA2: every remembered contact greets again
			ReestablishRecord * record = reestablish[i]; // RA2: as above
			Transmit_Message(RADIO_HELLO, record->Contact); // RA2: as above
			record = reestablish[i]; // RA2: as above
			if (record->WasTethered) { // RA2: as above
				IsTethered = true;
				record = reestablish[i]; // RA2: as above
				record->Contact->IsTethered = true; // RA2: as above
			}
		}

		if (Session.Type != GAME_NORMAL && !newowner->Is_Human_Player()) {
			Sell_All_Upgrades(); // RA2: as above
		}

		oldowner->IsRecalcNeeded = true;
		newowner->IsRecalcNeeded = true;

		return(true);
	}
	return(false);
}
#endif



/***********************************************************************************************
 * BuildingClass::Sort_Y -- Returns the building coordinate used for sorting.                  *
 *                                                                                             *
 *    The coordinate value returned from this function should be used for sorting purposes.    *
 *    It has special offset adjustment applied so that vehicles don't overlap (as much).       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a coordinate value suitable to be used for sorting.                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1995 JLB : Created.                                                                 *
 *   06/19/1995 JLB : Handles buildings that come with bibs built-in.                          *
 *=============================================================================================*/
int BuildingClass::Sort_Y(void) const
{
	int y = BASECLASS::Sort_Y();
	return(y + (Class->IsTurretAnimAVoxel != 0 ? 32 : 0) - (Class->IsGate != 0 ? 16 : 0));
}


/***********************************************************************************************
 * BuildingClass::Can_Enter_Cell -- Determines if building can be placed down.                 *
 *                                                                                             *
 *    This routine will determine if the building can be placed down at the location           *
 *    specified.                                                                               *
 *                                                                                             *
 * INPUT:   cell  -- The cell to examine. This is usually the cell of the upper left corner    *
 *                   of the building if it were to be placed down.                             *
 *                                                                                             *
 * OUTPUT:  Returns with the move legality value for placement at the location specified. This *
 *          will either be MOVE_OK or MOVE_NO.                                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
MoveType BuildingClass::Can_Enter_Cell(CellClass const * cell, FacingType dir, int cell_height, CellClass const *, bool) const
{
	Cell c = cell->CellID;
	if (Class->UndeploysInto && IsDown) {
		return(Map[c].Is_Clear_To_Build(Class->Speed, (BuildingTypeClass *)Class, House) ? MOVE_OK : MOVE_NO);
	}
	return(Class->Legal_Placement(c, House) ? MOVE_OK : MOVE_NO); // RA2: Can_Enter_Cell at 0x4453B0 has no ScenarioInit, Session, Type and 4 more
}


/***********************************************************************************************
 * BuildingClass::Can_Demolish -- Can the player demolish (sell back) the building?            *
 *                                                                                             *
 *    Determines if the player can sell this building. Selling is possible if the building     *
 *    is not currently in construction or deconstruction animation.                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Can the building be demolished at this time?                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Created.                                                                 *
 *   07/01/1995 JLB : If there is no buildup data, then the building can't be sold.            *
 *   07/17/1995 JLB : Cannot sell a refinery that has a harvester attached.                    *
 *=============================================================================================*/
bool BuildingClass::Can_Demolish(void) const
{
	if (Class->IsUnsellable) return(false);

	if (HasBuildupData && BState != BSTATE_CONSTRUCTION && Mission != MISSION_DECONSTRUCTION && Mission != MISSION_CONSTRUCTION) {
		//if (*this == STRUCT_REFINERY && Is_Something_Attached()) return(false);
		return(true);
	}
	if (Class->IsFirestormWall && !House->FirestormDefenseActivated) {
		return(true);
	}
	return(false);
}


/// <summary>
/// Clears any object loitering on the weapons factory door.
/// This routine is used to make room for a vehicle that is about to drive out of the
/// factory. Whatever is sitting on the exit cell, along with anything in the cells
/// immediately around it, is told to scatter.
/// </summary>
/// <returns>bool; Was something found blocking the door and moved along?</returns>
bool BuildingClass::Clear_Weapons_Factory_Bib(void)
{
	if (Class->IsWeaponsFactory) {

		Cell exit = Class->ExitList[10]; // RA2: RA2's form
		Cell cell = PositionCell + exit; // RA2: as above
		cell = cell + Cell(-1, 0); // RA2: as above
		Cell cell2 = cell;
		Coord coord = cell2.As_Coord(); // RA2: as above
		CellClass * cellptr = &Map[cell2];

		TechnoClass * tech = cellptr->Cell_Techno(Point2D(0,0), false, this);
		if (tech != NULL) {
			DebugString("Weapons factory clearing %s from bib\n", (const char *)tech->TClass->IniName);
			cellptr->Incoming(COORD_NONE, true, true);

			for (FacingType f = FACING_FIRST; f < FACING_COUNT; f++) {
				CellClass * cptr = &cellptr->Adjacent_Cell(f);
				TechnoClass * tech = cptr->Cell_Techno(Point2D(0,0), false, this);
				if (tech != NULL) {
					DebugString("Weapons factory clearing %s from bib area\n", (const char *)tech->TClass->IniName);
					cptr->Incoming(coord, true, true);
				}
			}
			return(true);
		}
	}
	return(false);
}



/***********************************************************************************************
 * BuildingClass::Mission_Guard -- Handles guard mission for combat buildings.                 *
 *                                                                                             *
 *    Buildings that can attack are given this mission. They will wait until a suitable target *
 *    comes within range and then launch into the attack mission. Buildings that have no       *
 *    weaponry will just sit in this routine forever.                                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of game frames to delay before this routine will be called *
 *          again.                                                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: 768 bytes differ (adopted text)
int BuildingClass::Do_MISSION_GUARD(void)
{
	bool isfacility;
	bool hascontact;
	bool istechno;
	bool entermission;
	bool inrange;
	bool tomove;

	/*
	**	If this building has a weapon, then search for a target to attack. When
	**	a target is found, switch into attack mode to deal with the threat.
	*/
	if (Is_Weapon_Equipped()) {

		/*
		**	Weapon equipped buildings are ALWAYS ready to launch into another mission if
		**	they are sitting around in guard mode.
		*/
		IsReadyToCommence = true;

	} else { // RA2: no SUPER_CHEM_MISSILE (not in RA2's layout)

		if (Class->IsHasStupidGuardMode) {
			return(100);
		}

		/*
		**	This is the very simple state machine that basically does
		**	nothing. This is the mode that non weapon equipped buildings
		**	are normally in.
		*/
		enum {
			INITIAL_ENTRY,
			IDLE
		};
		switch (Status) {
			case INITIAL_ENTRY:
				Begin_Mode(BSTATE_IDLE);
				Status = IDLE;
				break;

			case IDLE:

				/*
				**	Special case to break out of guard mode if this is a repair
				**	facility and there is a customer waiting at the grease pit.
				*/
				isfacility = Class->IsCanUnitRepair || Class->IsCanUnitReload;
				hascontact = Contact_With_Whom() != 0;

				istechno = false;
				if (hascontact) {
					istechno = Contact_With_Whom()->Is_Techno();
				}

				entermission = false;
				if (istechno) {
					entermission = ((TechnoClass *)Contact_With_Whom())->Mission == MISSION_ENTER;
				}

				inrange = false;
				if (entermission) {
					inrange = Distance_To(Contact_With_Whom()) < CELL_LEPTON / 4;
				}

				tomove = false;
				if (inrange) {
					tomove = Transmit_Message(RADIO_NEED_TO_MOVE) == RADIO_ROGER;
				}

				if (isfacility && hascontact && istechno && entermission && inrange && tomove) {

					Assign_Mission(MISSION_REPAIR);
					return(1);
				}

				if (Class->IsWeaponsFactory) {
					Clear_Weapons_Factory_Bib();
				}

				break;

			default:
				break;
		}

		if (Class->IsCanUnitReload && In_Radio_Contact()) {
			if (Transmit_Message(RADIO_PREPARED) != RADIO_ROGER &&
				Transmit_Message(RADIO_NEED_TO_MOVE) == RADIO_ROGER) {
				Assign_Mission(MISSION_REPAIR);
			}
		}

		if (Class->IsCanUnitRepair) {
			return(Current_Mission_Control().Normal_Delay() + Random_Pick(0, 2));
		} else {
			return(Current_Mission_Control().Normal_Delay() * 3 + Random_Pick(0, 2));
		}
	}

	return(Current_Mission_Control().AA_Delay() + Random_Pick(0, 2));
}
#endif


/// <summary>
/// Handles the guard area mission for a building.
/// A building cannot leave its foundation, so guarding an area amounts to the same thing
/// as guarding itself. This routine defers to the normal guard mission.
/// </summary>
/// <returns>Returns with the delay in game frames before this mission should be processed
/// again.</returns>
int BuildingClass::Do_MISSION_GUARD_AREA(void)
{
	return(Do_MISSION_GUARD());
}


/***********************************************************************************************
 * BuildingClass::Mission_Construction -- Handles mission construction.                        *
 *                                                                                             *
 *    This routine will handle mission construction. When this mission is complete, the        *
 *    building will begin normal operation.                                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of game frames to delay before calling this routine        *
 *          again.                                                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int BuildingClass::Do_MISSION_CONSTRUCTION(void)
{
	enum {
		INITIAL,
		DURING
	};
	switch (Status) {
		case INITIAL:
			Begin_Mode(BSTATE_CONSTRUCTION);
			Transmit_Message(RADIO_BUILDING);
			{
				VocType sound = (VocType)Class->BuildupSound; // RA2: the type's own buildup sound, else the rules' Construction sound
				if (sound == VOC_NONE) { // RA2: as above
					sound = Rule->Construction; // RA2: as above
				}
				if (sound != VOC_NONE) { // RA2: as above
					VocClass::PlayAt(sound, PositionCoord, &BuildupSoundController); // RA2: the sound is kept in BuildupSoundController
				}
			}
			IsToDisplay = true;
			Status = DURING;
			break;

		case DURING:
			IsToDisplay = true;
			VocClass::UpdateAt(PositionCoord, &BuildupSoundController); // RA2: the kept sound follows the building
			if (IsReadyToCommence) {

				/*
				**	When construction is complete, then transmit this
				**	to the construction yard so that it can stop its
				**	construction animation.
				*/
				Transmit_Message(RADIO_COMPLETE);		// "I'm finished."
				Transmit_Message(RADIO_OVER_OUT);		// "You're free."
				Begin_Mode(BSTATE_IDLE);
				Grand_Opening();
				QueueMission(MISSION_GUARD, false); // RA2: QueueMission, not Assign_Mission
				if (!Class->IsLaserFence) {
					PrimaryFacing.Set(Class->StartFace);
				}
				Class->Free_Buildup_Data();
				BuildupSoundController.EndLooping(); // RA2: the buildup sound ends with the construction
				return(1); // RA2: returns 1 straight away
			}
			break;

		default:
			break;
	}
	return(1);
}


/// <summary>
/// Can this building be undeployed back into a vehicle?
/// A building only undeploys if its type names something to undeploy into. Mobile
/// deployers and types that always allow it agree immediately, otherwise the game
/// options decide whether the owner is permitted to pack up and move on.
/// </summary>
bool BuildingClass::Can_Be_Undeployed(void)
{
	BuildingTypeClass const * type = Class; // RA2: RA2's undeploying types
	if (type->UndeploysInto == NULL) { // RA2: as above
		return(false); // RA2: as above
	}
	if (type->IsSensorArray || type->IsTickTank || type->IsICBMLauncher || type->IsArtillary) { // RA2: as above
		return(true); // RA2: as above
	}
	if (Session.Type == GAME_NORMAL) { // RA2: as above
		return(false); // RA2: as above
	}
	if (ArchivedTarget != NULL && House->Is_Human_Player() && (Session.Type == GAME_NORMAL || Session.Options.MCVRedeploy)) { // RA2: as above
		return(true); // RA2: as above
	}
	return(false);
}



/***********************************************************************************************
 * BuildingClass::Mission_Deconstruction -- Handles building deconstruction.                   *
 *                                                                                             *
 *    This state machine is only used when the building is deconstructing as a result of       *
 *    selling.  When this mission is finished, the building will no longer exist.              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of game frames to delay before calling this routine again. *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Created.                                                                 *
 *   08/13/1995 JLB : Enable selling of units on a repair bay.                                 *
 *   08/20/1995 JLB : Scatters infantry from scattered starting points.                        *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: 3098 bytes differ (adopted text)
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

			if (Can_Be_Undeployed() && Class->Is_Mobile_Deployer()) {
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
				if (IsOwnedByPlayer) Speak(VOX_UNIT_SOLD);
				tech->Sell_Back(1);
				Assign_Mission(MISSION_GUARD);
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
				Adjust_House_Power(House);
				if (IsOwnedByPlayer) Speak(VOX_STRUCTURE_SOLD);
				Assign_Mission(MISSION_GUARD);
				return(1);
			}

			/*
			 * A juggernaut must rotate back to its starting facing before the
			 * deconstruction can proceed.
			 */

			End_Anim(BANIM_ALL); // RA2: no IsJuggernaut (not in RA2's layout)

			IsReadyToCommence = false;
			Transmit_Message(RADIO_RUN_AWAY);

			if (Class->IsLaserFencePost) {
				Toggle_Laser_Fence_Post(false);
			}


			if (Class->AuxSound2 != VOC_NONE) { // RA2: no IsJuggernaut (not in RA2's layout)
				Sound_Effect(Class->AuxSound2, PositionCoord);
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
				if (ArchiveTarget == NULL && Class->UndeploysInto == NULL) {
					int count = How_Many_Survivors();
					bool engineer = false;

					const Cell * list = Occupy_List();
					const Cell * occupy = list;
					int num_cells = 0;
					while (*occupy != REFRESH_EOL) {
						num_cells++;
						occupy++;
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
								infantry->Assign_Mission(MISSION_MOVE);
							} else {
								delete infantry;
							}
							ScenarioInit--;
						}
						count--;
					}
				}

				if (House->Is_Player_Control() && !Considered_Vehicle()) {
					Sound_Effect(Rule->SellSound, PositionCoord);
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
				if (IsOwnedByPlayer && Class->UndeploysInto == NULL) Speak(VOX_STRUCTURE_SOLD);

				/*
				**	Construction yards that deconstruct, really just revert back
				**	to an MCV.
				*/
				if (Class->UndeploysInto != NULL &&
					(Class->Is_Mobile_Deployer() ||
					Class->Can_Always_Undeploy() ||
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
						if (Class->Is_Mobile_Deployer()) {
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
							unit->ActLike = ActLike;
							unit->Group = Group;
							unit->Veterancy.Experience = Veterancy.Experience;


							/*
							**	Lift the move destination from the building and assign
							**	it to the unit.
							*/
							if (arch != NULL) { // RA2: no LimpetType, LimpetSpeedFactor (not in RA2's layout)
								unit->Assign_Destination(arch);
								unit->Assign_Mission(MISSION_MOVE);
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
					Adjust_House_Power(House);

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
						int amount = storage->Decrease_Amount(storage->Get_Amount(i), i);
						House->Give_Tiberium(amount, i); // RA2: Give_Tiberium takes the amount and the index
						i = storage->First_Used_Slot();
					}

					/*
					**	Finally, delete the building from the game.
					*/
					Delete_Me();

					Adjust_House_Power(House);
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
#endif




/// <summary>
/// Draws this prism tower's support beam to the tower that fires and starts the tower's
/// recharge.
/// </summary>
/// <param name="destination">The coordinate the support beam goes to.</param>
void BuildingClass::Fire_Prism_Support(Coord destination) // RA2: not in TibSun, at 0x446820
{
	RGBClass black;
	LaserDrawClass * laser = new LaserDrawClass(Fire_Coord(0), destination, 0, true, House->LaserColor, black, black, Rule->PrismSupportDuration, false, true, 1.0f, 0.0f);
	if (laser != NULL) {
		laser->IsHouseColor = true;
		laser->Thickness = 3;
	}
	Arm = Rule->PrismSupportDelay;
	SupportingPrisms = 0;
}


/***********************************************************************************************
 * BuildingClass::Mission_Attack -- Handles attack mission for building.                       *
 *                                                                                             *
 *    Buildings that can attack are processed by this attack mission state machine.            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of game frames to delay before calling this routine        *
 *          again.                                                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Created.                                                                 *
 *   02/22/1996 JLB : SAM doesn't lower back into ground.                                      *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: no independent bound yet (00446920 has no row)
int BuildingClass::Do_MISSION_ATTACK(void)
{
	AircraftClass *air;

	if (Class->IsSAM) {
		switch (Status) {

			/*
			**	This is the target tracking state of the launcher. It will rotate
			**	to face the current TarCom of the launcher.
			*/
			case SAM_READY:
				if ((Class->IsPowered && Class->Drain > 0 && House->Power_Fraction() < 1)) {
					return(1);
				}
				//if (TarCom == NULL || !Is_Target_Aircraft(TarCom) || As_Aircraft(TarCom)->Height == 0) {
				air = dynamic_cast<AircraftClass *>(TarCom);
				if (TarCom == NULL || !air || air->Height == 0) {
					Assign_Target(NULL);
					Status = SAM_READY;
					Assign_Mission(MISSION_GUARD);
					Commence();
					return(1);
				} else {
					if (!PrimaryFacing.Is_Rotating()) {
						if (DIR_STEP_8 >= (PrimaryFacing.Current() - Aim_Direction(TarCom))) {
							Status = SAM_FIRING;
						} else {
							PrimaryFacing.Set_Desired(Aim_Direction(TarCom));
						}
					}
				}
				return(1);

			/*
			**	The launcher is in the process of firing.
			*/
			case SAM_FIRING:
				air = dynamic_cast<AircraftClass *>(TarCom);
				//if (TarCom == NULL || !Is_Target_Aircraft(TarCom) || As_Aircraft(TarCom)->Height == 0) {
				if (TarCom == NULL || !air || air->Height == 0) {
					Assign_Target(NULL);
					Status = SAM_READY;
				} else {
					FireErrorType error = Can_Fire(TarCom, 0);
					if (error == FIRE_ILLEGAL || error == FIRE_CANT || error == FIRE_RANGE) {
						Assign_Target(NULL);
						Status = SAM_READY;
					} else {
						if (error == FIRE_FACING) {
							Status = SAM_READY;
						} else {
							if (error == FIRE_OK) {
								Fire_At(TarCom, 0);
								Fire_At(TarCom, 1);
								Status = SAM_READY;
							}
						}
					}
				}
				return(1);

			default:
				break;
		}

		return(Current_Mission_Control().AA_Delay() + Random_Pick(0, 2));

	}

	if (TarCom == NULL) {
		Assign_Target(NULL);
		Assign_Mission(MISSION_GUARD);
		Commence();
		return(1);
	}

	int primary = What_Weapon_Should_I_Use(TarCom);
	IsReadyToCommence = true;

	FireErrorType fire = Can_Fire(TarCom, primary);

	if (fire == FIRE_FACING && Is_Turret_Equipped()) {
		DirType dir = Aim_Direction(TarCom);
		DirType dir2(Class->IsTurretAnimAVoxel ? DIR_MIN : DIR_STEP_32);
		if (dir2 < (PrimaryFacing.Current() - dir) && Class->IsTurretAnimAVoxel && DIR_STEP_128 >= (PrimaryFacing.Current() - dir)) {
			PrimaryFacing.Set(dir);
			fire = Can_Fire(TarCom, primary);
		}
	}

	switch (fire) {
		case FIRE_ILLEGAL:
		case FIRE_CANT:
		case FIRE_RANGE:
		case FIRE_AMMO:
			Assign_Target(NULL);
			Assign_Mission(MISSION_GUARD);
			Commence();
			break;

		case FIRE_FACING:
			if (TarCom != NULL) {
				PrimaryFacing.Set_Desired(Aim_Direction(TarCom));
			}
			return(2);

		case FIRE_REARM:
			if (TarCom != NULL) {
				PrimaryFacing.Set_Desired(Aim_Direction(TarCom));
			}
			return(2);

		case FIRE_BUSY:
			return(1);

		case FIRE_CLOAKED:
			Do_Uncloak();
			break;

		case FIRE_OK:
			if (UpgradeLevel != 0 && Upgrades[0] != NULL) {
				if (Upgrades[0]->Is_Two_Shooter()) {
					Fire_At(TarCom, 0);
					Fire_At(TarCom, 1);
				}
			} else {
				Fire_At(TarCom, primary);
			}
			return(1);

		default:
			break;
	}
	if (TarCom != NULL) {
		PrimaryFacing.Set_Desired(Aim_Direction(TarCom));
		return(1);
	}

	return(1);
}
#endif








/***********************************************************************************************
 * BuildingClass::Mission_Harvest -- Handles refinery unloading harvesters.                    *
 *                                                                                             *
 *    This state machine handles the refinery when it unloads the harvester.                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of game frames to delay before calling this routine        *
 *          again.                                                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int BuildingClass::Do_MISSION_HARVEST(void)
{
	/// This state machine was removed in Tiberian Sun.
	#if 0
	//assert(IsActive);

	enum {
		INITIAL,					// Dock the Tiberium cannister.
		WAIT_FOR_DOCK,			// Waiting for docking to complete.
		MIDDLE,					// Offload "bails" of tiberium.
		WAIT_FOR_UNDOCK		// Waiting for undocking to complete.
	};
	switch (Status) {
		case INITIAL:
			Status = WAIT_FOR_DOCK;
			break;

		case WAIT_FOR_DOCK:
			if (IsReadyToCommence) {
				IsReadyToCommence = false;
				Status = MIDDLE;
			}
			break;

		case MIDDLE:
			if (IsReadyToCommence) {
				IsReadyToCommence = false;

				/*
				**	Force any bib squatters to scatter.
				*/
				Map[Adjacent_Cell(Center_Coord().As_Cell(), DIR_S)].Incoming(0, true, true);

				FootClass * techno = Attached_Object();
				if (techno) {
					int bail = techno->Offload_Tiberium_Bail();

					if (bail) {
						House->Harvested(bail);
						if (techno->Tiberium_Load() > 0) {
							return(1);
						}
					}
				}
				Status = WAIT_FOR_UNDOCK;
			}
			break;

		case WAIT_FOR_UNDOCK:
			if (IsReadyToCommence) {

				/*
				**	Detach harvester and go back into idle state.
				*/
				Assign_Mission(MISSION_GUARD);
			}
			break;

		default:
			break;
	}
	#endif
	return(BASECLASS::Do_MISSION_HARVEST());
}


/// <summary>
/// RA2
/// </summary>
static inline Cell Target_Cell(AbstractClass * target)
{
	return(target->Center_Coord().As_Cell());
}


/***********************************************************************************************
 * BuildingClass::Mission_Capture -- Buildings capture nothing of their own.                   *
 *=============================================================================================*/
int BuildingClass::Do_MISSION_CAPTURE(void) // RA2: 0x447240; the base class behaviour
{
	return(MissionClass::Do_MISSION_CAPTURE());
}

static inline FootClass * Techno_As_Foot(TechnoClass * techno) // RA2: the checked cast the executable expands inline wherever a radio contact is used as a foot (bit 2 of the abstract flags)
{
	return((techno == NULL) ? NULL : ((techno->AbstractFlags & 4) ? (FootClass *)techno : NULL)); // RA2: as above
}

EXTERN_C const CLSID CLSID_ShipLocomotion; // RA2: 0x007A1FF0, the ship locomotor's class id (next to CLSID_HoverLocomotion 0x007A1F80)

/***********************************************************************************************
 * BuildingClass::Mission_Repair -- Handles the repair (active) state for building.            *
 *                                                                                             *
 *    This state machine is used when the building is active in some sort of repair or         *
 *    construction mode. The construction yard will animate. The repair facility will repair   *
 *    anything that it docked on it.                                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of game frames to delay before calling this routine again. *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Created.                                                                 *
 *   06/25/1995 JLB : Handles repair facility                                                  *
 *   07/29/1995 JLB : Repair rate is controlled by power rating.                               *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: 4259 bytes differ (adopted text)
int BuildingClass::Do_MISSION_REPAIR(void)
{
	if (Class->IsConstructionYard) {
		enum {
			INITIAL,
			IDLE,
			DURING
		};
		switch (Status) {
			case INITIAL:
				Begin_Mode(BSTATE_ACTIVE);
				Begin_Anim(BANIM_PRE_PRODUCTION, HealthRatio <= Rule->ConditionYellow, false, 0); // RA2: as above
				Status = DURING;
				break;

			case DURING:
				if (!HasAnyLink()) { // RA2: HasAnyLink
					QueueMission(MISSION_GUARD, false); // RA2: QueueMission
					End_Anim(BANIM_PRE_PRODUCTION);
				}
				break;

			default:
				break;
		}
		return(1);
	}

	if (Class->IsHospital) {
		enum {
			INITIAL,
			IDLE,
			DURING
		};
		switch (Status) {
			case INITIAL:
				Status = DURING;
				IsReadyToCommence = false;
				BuildingStage.Set_Stage(0);
				BuildingStage.Set_Rate(1);
				if (Class->MaxAmmo != -1) { // RA2: only a limited-ammo hospital uses up ammo
					Ammo--; // RA2: as above
					Ammo = MAX(Ammo, 0); // RA2: as above
				}
				break;

			case DURING:
				BuildingStage.Graphic_Logic();
				if (BuildingStage.Fetch_Stage() >= (Rule->IRepairRate * TICKS_PER_MINUTE)) {
					IsReadyToCommence = false;
					BuildingStage.Set_Stage(0);
					switch (Transmit_Message(RADIO_REPAIR)) { // RA2: as above
						case RADIO_ALL_DONE: // RA2: as above
							if (House->Is_Player_Control()) { // RA2: the player hears and sees the unit leave repaired
								if (Submit_Radar_Event(RadarEventType(8), Get_Cell())) { // RA2: as above
									Queue_Speech("EVA_UnitRepaired", -1, -1); // RA2: as above
								}
								VocClass::PlayAt(Rule->HealCrateSound, Get_Coord(), 0); // RA2: as above
							}
							// Both completed treatment and an empty reply release the patient.

						case RADIO_NEGATIVE: // RA2: as above
							Exit_Object((TechnoClass *)Cargo.Detach_Object()); // RA2: as above
							QueueMission(MISSION_GUARD, false); // RA2: QueueMission
							break; // RA2: as above

						case RADIO_CANT: // RA2: as above
							Exit_Object((TechnoClass *)Cargo.Detach_Object()); // RA2: as above
							break; // RA2: as above
					}
				}
				return(1);

			default:
				break;
		}

		return(Current_Mission_Control().Normal_Delay());
	}

	if (Class->IsArmory) {
		enum {
			INITIAL,
			IDLE,
			DURING
		};
		switch (Status) {
			case INITIAL:
				Status = DURING;
				IsReadyToCommence = false;
				BuildingStage.Set_Stage(0);
				BuildingStage.Set_Rate(1);
				Ammo--;
				Ammo = MAX(Ammo, 0);
				break;

			case DURING:
				BuildingStage.Graphic_Logic();
				if (BuildingStage.Fetch_Stage() >= (Rule->IRepairRate * TICKS_PER_MINUTE)) {
					TechnoClass * contact = Radio_Link(0); // RA2: as above
					if (contact->Veterancy.Is_Dumbass()) {
						contact->Veterancy.Set_Veteran(true);
					} else {
						contact->Veterancy.Set_Elite(true);
					}
					Exit_Object((TechnoClass *)Cargo.Detach_Object()); // RA2: as above
					QueueMission(MISSION_GUARD, false); // RA2: QueueMission
				}
				break; // RA2: as above

			default:
				break;
		}

		return(Current_Mission_Control().Normal_Delay());
	}

	if (Class->IsCanUnitRepair) {
		enum {
			INITIAL,
			IDLE,
			DURING
		};
		switch (Status) {
			case INITIAL:
				{
					if (!HasAnyLink()) { // RA2: as above
						End_Anim(BANIM_PRODUCTION);
						End_Anim(BANIM_SPECIAL_TWO);
						Begin_Anim(BANIM_SPECIAL_THREE, HealthRatio <= Rule->ConditionYellow, false, 0); // RA2: as above
						Begin_Anim(BANIM_ACTIVE_ONE, HealthRatio <= Rule->ConditionYellow, false, 0); // RA2: as above
						QueueMission(MISSION_GUARD, true); // RA2: QueueMission, started at once
						return(1);
					}
					IsReadyToCommence = false;
					int distance = 100; // RA2: 100 leptons
					FootClass * tech = (FootClass *)Radio_Link(0); // RA2: as above

					/*
					**	BG: If the unit to repair is an aircraft, and the aircraft is
					**	fixed-wing, and it's landed, be much more liberal with the
					**	distance check.  Fixed-wing aircraft are very inaccurate with
					**	their landings.
					*/
					IPersistPtr persist(tech->Locomotion);
					CLSID clsid;
					persist->GetClassID(&clsid);
					if (clsid == CLSID_HoverLocomotion) { // RA2: as above
						distance = 200; // RA2: hovers and ships both
					}
					if (clsid == CLSID_ShipLocomotion) { // RA2: as above
						distance = 200; // RA2: as above
					}
					if (Transmit_Message(RADIO_NEED_TO_MOVE) == RADIO_ROGER && DistanceToDockingCoord(Radio_Link(0)) < distance) { // RA2: the docking distance
						Status = IDLE;
						return(3); // RA2: as above
					}
					if (!House->Ion_Sensitives_Should_Be_Offline()) { // RA2: as above
						tech->Locomotion->Power_On();
					}
					break;
				}

			case IDLE:
				{
					if (!HasAnyLink()) { // RA2: as above
						if (Anims[BANIM_PRODUCTION] || Anims[BANIM_SPECIAL_TWO]) {
							Begin_Anim(BANIM_SPECIAL_THREE, HealthRatio <= Rule->ConditionYellow, false, 0); // RA2: as above
							Begin_Anim(BANIM_ACTIVE_ONE, HealthRatio <= Rule->ConditionYellow, false, 0); // RA2: as above
							End_Anim(BANIM_PRODUCTION);
							End_Anim(BANIM_SPECIAL_TWO);
						}
						if (!Anims[BANIM_SPECIAL_THREE]) { // RA2: the idle state waits until the animation is gone
							IsReadyToCommence = true; // RA2: as above
							QueueMission(MISSION_GUARD, true); // RA2: as above
						}
						return(1); // RA2: as above
					}

					if (Anims[BANIM_PRODUCTION]) { // RA2: as above
						return(1);
					}

					FootClass * radio = (FootClass *)Radio_Link(0); // RA2: as above
					if (DistanceToDockingCoord(Radio_Link(0)) < 200) { // RA2: as above
						if (radio->Locomotion->Is_Powered()) {
							if (!radio->Locomotion->Is_Moving()) { // RA2: as above
								radio->Locomotion->Power_Off();
							}
							return(1); // RA2: as above
						}
						if (!radio->Locomotion->Is_Powered() && ((FootClass *)Radio_Link(0))->NavCom) { // RA2: as above
							((FootClass *)Radio_Link(0))->Abort_Motion(); // RA2: as above
						}
					}

					if (Transmit_Message(RADIO_NEED_TO_MOVE) == RADIO_ROGER) {
						TechnoClass * client = Radio_Link(0); // RA2: as above
						bool damaged = client->HealthRatio < Rule->ConditionGreen;
						bool manual_reload = client->Techno_Type_Class()->IsManualReload; // RA2: as above
						RadioMessageType msg = Transmit_Message(RADIO_REPAIR);
						bool roger = msg == RADIO_ROGER;
						bool all_done = msg == RADIO_ALL_DONE;
						if ((!damaged && !manual_reload) || (!roger && !all_done)) { // RA2: as above
							if (client->HealthRatio == Rule->ConditionGreen && !((FootClass *)client)->Locomotion->Is_Powered()) { // RA2: as above
								FootClass * mover = Techno_As_Foot(Radio_Link(0)); // RA2: the checked cast
								mover->Locomotion->Power_On(); // RA2: as above
								if (mover->ArchiveTarget != NULL && !mover->House->Is_Human_Player()) { // RA2: as above
									mover->QueueMission(MISSION_MOVE, false); // RA2: QueueMission
									mover->Assign_Destination(mover->ArchiveTarget, true); // RA2: as above
									mover->ArchiveTarget = NULL; // RA2: as above
									mover->QueueUpToEnter = NULL; // RA2: QueueUpToEnter, the object to enter once nearby
									Transmit_Message(RADIO_OVER_OUT); // RA2: as above
								} else {
									Cell exit = Find_Exit_Cell(Radio_Link(0)); // RA2: as above
									if (ArchivedTarget) { // RA2: as above
										exit = Cell(ArchivedTarget->Center_Coord()); // RA2: the archived target's cell
									}
									if (exit != CELL_NONE) { // RA2: as above
										mover->QueueMission(MISSION_MOVE, false); // RA2: as above
										mover->Assign_Destination(&Map[exit], true); // RA2: as above
										mover->ArchiveTarget = NULL;
										Transmit_Message(RADIO_OVER_OUT);
										mover->QueueUpToEnter = NULL; // RA2: as above
									}
								}
							}
						} else {

							/*
							**	If the object over the repair bay is marked as useless, then
							**	sell it back to get some money.
							*/
							if (!client->IsUseless || client->House->Is_Human_Player()) { // RA2: as above
								if (IsOwnedByPlayer) Queue_Speech("EVA_Repairing", -1, -1); // RA2: as above
								Status = DURING;
								Begin_Anim(BANIM_SPECIAL_ONE, HealthRatio <= Rule->ConditionYellow, false, 0); // RA2: as above
								End_Anim(BANIM_ACTIVE_ONE);
								End_Anim(BANIM_IDLE); // RA2: as above
								IsReadyToCommence = false;
								BuildingStage.Set_Stage(0);
								BuildingStage.Set_Rate(1);
							} else {
								client->Sell_Back(1); // RA2: as above
								Status = INITIAL; // RA2: as above
								IsReadyToCommence = true; // RA2: as above
							}
						}
					} else if (!House->Ion_Sensitives_Should_Be_Offline()) { // RA2: as above
						if (!((FootClass *)Radio_Link(0))->Locomotion->Is_Powered()) { // RA2: as above
							((FootClass *)Radio_Link(0))->Locomotion->Power_On(); // RA2: as above
						}
					}
				}
				break;

			case DURING:
				if (!HasAnyLink()) { // RA2: as above
					End_Anim(BANIM_PRODUCTION);
					End_Anim(BANIM_SPECIAL_TWO);
					Begin_Anim(BANIM_SPECIAL_THREE, HealthRatio <= Rule->ConditionYellow, false, 0); // RA2: as above
					Begin_Anim(BANIM_ACTIVE_ONE, HealthRatio <= Rule->ConditionYellow, false, 0); // RA2: as above
					Status = IDLE;
					return(1);
				}

				if (BuildingStage.Fetch_Rate() == 0) { // RA2: an unset stage rate becomes one frame
					BuildingStage.Just_Set_Rate(1); // RA2: as above
				}
				BuildingStage.Graphic_Logic();

				/*
				**	Check to see if the repair light blink has completed and the attached
				**	unit is not doing something else. If these conditions are favorable,
				**	the repair can proceed another step.
				*/
				if (BuildingStage.Fetch_Stage() >= (Rule->URepairRate * TICKS_PER_MINUTE) && Transmit_Message(RADIO_NEED_TO_MOVE) == RADIO_ROGER) {
					IsReadyToCommence = false;
					BuildingStage.Set_Stage(0);

					/*
					**	Tell the attached unit to repair one step. It will respond with how
					**	it fared.
					*/
					switch (Transmit_Message(RADIO_REPAIR)) {

						/*
						**	The repair step proceeded smoothly. Proceed normally with the
						**	repair process.
						*/
						case RADIO_ROGER:
							break;

						/*
						**	The repair operation was aborted because of some reason. Presume
						**	that the reason is because of low cash.
						*/
						case RADIO_CANT:
							{
								if (IsOwnedByPlayer && House->Available_Money() == 0) Queue_Speech("EVA_InsufficientFunds", -1, -1); // RA2: as above
								End_Anim(BANIM_PRODUCTION); // RA2: as above
								End_Anim(BANIM_SPECIAL_TWO); // RA2: as above
								Begin_Anim(BANIM_SPECIAL_THREE, HealthRatio <= Rule->ConditionYellow, false, 0); // RA2: as above
								Begin_Anim(BANIM_ACTIVE_ONE, HealthRatio <= Rule->ConditionYellow, false, 0); // RA2: as above
								Status = IDLE; // RA2: as above
								break; // RA2: as above
							}

						/*
						**	The repair step resulted in a completely repaired unit.
						*/
						default:
							{
								if (IsOwnedByPlayer && Submit_Radar_Event(RadarEventType(8), Get_Cell())) Queue_Speech("EVA_UnitRepaired", -1, -1); // RA2: as above
								End_Anim(BANIM_PRODUCTION);
								End_Anim(BANIM_SPECIAL_TWO);
								Begin_Anim(BANIM_SPECIAL_THREE, HealthRatio <= Rule->ConditionYellow, false, 0); // RA2: as above
								Begin_Anim(BANIM_ACTIVE_ONE, HealthRatio <= Rule->ConditionYellow, false, 0); // RA2: as above
								Status = IDLE;

								FootClass * foot = Techno_As_Foot(Radio_Link(0)); // RA2: as above
								if (foot->ArchiveTarget != NULL && !foot->House->Is_Human_Player()) {
									foot->QueueMission(MISSION_MOVE, false); // RA2: QueueMission
									foot->Assign_Destination(foot->ArchiveTarget, true); // RA2: as above
									foot->ArchiveTarget = NULL;
									Transmit_Message(RADIO_OVER_OUT);
									foot->QueueUpToEnter = NULL; // RA2: QueueUpToEnter, the object to enter once nearby
									return(1); // RA2: as above
								}
								Cell exit = Find_Exit_Cell(Radio_Link(0)); // RA2: as above
								if (ArchivedTarget) { // RA2: as above
									exit = Cell(ArchivedTarget->Center_Coord()); // RA2: the archived target's cell
								}
								if (exit != CELL_NONE) { // RA2: as above
									foot->QueueMission(MISSION_MOVE, false); // RA2: as above
									foot->Assign_Destination(&Map[exit], true); // RA2: as above
									Transmit_Message(RADIO_OVER_OUT); // RA2: as above
									foot->QueueUpToEnter = NULL; // RA2: as above
									return(1); // RA2: as above
								}
								break; // RA2: as above
							}
					}
				}
				return(1);

			default:
				break;
		}
		return(Current_Mission_Control().Normal_Delay());
	}

	if (Class->IsCanUnitReload) {
		bool servicing = false; // RA2: as above
		for (int index = 0; index < RadioLinks.Length(); index++) { // RA2: every radio link is serviced
			TechnoClass * contact = Radio_Link(index); // RA2: as above
			if (contact) { // RA2: as above
				bool release = false; // RA2: as above
				if (Transmit_Message(RADIO_PREPARED, contact) == RADIO_ROGER && contact->Strength == contact->Techno_Type_Class()->MaxStrength) { // RA2: as above
					release = true; // RA2: as above
				} else if (contact->Get_Mission() != MISSION_ENTER && Transmit_Message(RADIO_NEED_TO_MOVE, contact) == RADIO_ROGER) { // RA2: as above
					servicing = true; // RA2: as above
					if (contact->Get_Mission() != MISSION_SLEEP) { // RA2: as above
						contact->QueueMission(MISSION_SLEEP, false); // RA2: as above
					} else if (Transmit_Message(RADIO_RELOAD, contact) != RADIO_ROGER && Transmit_Message(RADIO_REPAIR, contact) != RADIO_ROGER) { // RA2: as above
						release = true; // RA2: as above
					}
				}
				if (release) { // RA2: as above
					contact->Enter_Idle_Mode(false, true); // RA2: as above
					contact->Set_Mission(MISSION_GUARD); // RA2: as above
					contact->Advance_Waypoint_Path(); // RA2: as above
				}
			}
		}
		if (servicing) { // RA2: as above
			return(int(Rule->ReloadRate * TICKS_PER_MINUTE)); // RA2: as above
		}
		QueueMission(MISSION_GUARD, false); // RA2: QueueMission
		return(3);
	}
	return(15); // RA2: 15 frames
}
#endif



/***********************************************************************************************
 * BuildingClass::Mission_Missile -- State machine for nuclear missile launch.                 *
 *                                                                                             *
 *    This handles the Temple of Nod launching its nuclear missile.                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of frames to delay before calling this routine again.      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/04/1995 JLB : Commented.                                                               *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: no independent bound yet (00448400 has no row)
int BuildingClass::Do_MISSION_MISSILE(void)
{
	if (Class->IsNukeSilo) {
		enum {
			INITIAL,
			DOOR_OPENING,
			LAUNCH_UP,
			LAUNCH_DOWN,
			DONE_LAUNCH
		};

		switch (Status) {

			/*
			**	The initial case is responsible for starting the door
			**	opening on the building.
			*/
			case INITIAL:
				IsReadyToCommence = false;
				Begin_Mode(BSTATE_ACTIVE);	// open the door
				Status = DOOR_OPENING;
				return(1);

			/*
			**	This polls for the case when the door is actually open and
			**	then kicks off the missile smoke.
			*/
			case DOOR_OPENING:
				if (IsReadyToCommence) {
					Begin_Mode(BSTATE_AUX1);	// hold the door open
					Status = LAUNCH_UP;
					return(14);
				}
				return(1);

			/*
			**	Once the smoke has been going for a little while this
			**	actually handles launching the missile into the air.
			*/
			case LAUNCH_UP:
				{
					//Cell center = Center_Coord().As_Cell();
					Cell center = Center_Coord();
					//Cell cell = Cell( Cell_X(center), 1);
					AbstractClass * targ = &Map[House->NukeDest];
					WeaponTypeClass const * weap = SuperWeaponTypes[LastSuperWeaponIndex]->Weapon;
					BulletTypeClass const * bullet_type = weap->Bullet;
					WarheadTypeClass const * warhead = weap->WarheadPtr;
					MPHType max_speed = weap->MaxSpeed;
					BulletClass * bullet = bullet_type->Create_Bullet(targ, this, 200, warhead, max_speed, true); // RA2: a BulletTypeClass member, no range
					if (bullet) {
						bullet->Limbo();
						Coord launch = Move_Coord(Center_Coord(), DIR_N, 5 * CELL_LEPTON / 8);
						/*
						 * The nuke takes off straight up (pitch DIR_N).
						 */
						if (!bullet->Unlimbo(launch, TVelocity3D<double>(DirType(DIR_N), DirType(DIR_N), 100))) {
							delete bullet;
							bullet = NULL;
						} else {
							if (!House->Is_Player_Control()) {
								Speak(VOX_MISSILE_LAUNCH_DETECTED);
							}
							Status = LAUNCH_DOWN;
						}
					}
				}
				return(1);

			/*
			**	Once the missile is in the air, this handles waiting for
			**	the missile to be off the screen and then launching one down
			**	over the target.
			*/
			case LAUNCH_DOWN:
				Begin_Mode(BSTATE_AUX2);	// start the door closing
				Status = DONE_LAUNCH;
				return(6);

			/*
			**	Once the missile is done launching this handles allowing
			**	the building to sit there with its door closed.
			*/
			case DONE_LAUNCH:
				Begin_Mode(BSTATE_IDLE);	// keep the door closed.
				Assign_Mission(MISSION_GUARD);
				return(4 * TICKS_PER_SECOND);
		}
	}


	if (Class->IsEMPulseCannon) { // RA2: no SUPER_CHEM_MISSILE (not in RA2's layout)
		enum {
			INITIAL,
			EM_PULSE,
			FIRE,
			DONE
		};

		switch (Status) {
			case INITIAL: {
					DirType aim_direction = Aim_Direction(&Map[House->EMPDest]);
					DirType barrel_pitch = Barrel_Pitch(&Map[House->EMPDest]);

					if (DIR_MIN < (PrimaryFacing.Current() - aim_direction)) {
						PrimaryFacing.Set_Desired(aim_direction);
					} else {
						if (DIR_MIN < (BarrelPitch.Current() - barrel_pitch)) {
							BarrelPitch.Set_Desired(barrel_pitch);
						} else {
							Status = EM_PULSE;
						}
					}
				}
				return(1);

			case EM_PULSE:
				new AnimClass(AnimTypes[Anim_From_Name("PULSBALL")], Fire_Coord(0));
				Status = FIRE;
				return(32);

			case FIRE: {
					WeaponTypeClass const * weap = PrimaryWeapon;
					CellClass * targ = &Map[House->EMPDest];
					CellClass * targ2 = &Map[House->EMPDest];

					if (!Debug_Map && House->EMPDest != CELL_NONE) {
						BarrelPitch.Set_Desired(Barrel_Pitch(targ));

						Coord fire = Fire_Coord(0);
						Map.Get_Height_GL(House->EMPDest); /// result discarded
						DirType yaw = ::Direction(fire, House->EMPDest.As_Coord());
						MPHType speed = Calculate_Projectile_Speed(Distance(targ), Rule->Gravity);

						BulletClass * bullet = weap->Bullet->Create_Bullet(targ, this, weap->Attack, weap->WarheadPtr, 3 * speed / 4, weap->IsBright); // RA2: a BulletTypeClass member, no range

						TVelocity3D<double> velocity;
						velocity.Set(0, 0, 0);
						velocity.Set_Yaw(yaw);
						velocity.Set_Speed(speed);

						Coord ucoord = Turret_Coord();
						Coord disp = targ2->Center_Coord() - ucoord;
						int distance2 = disp.X * disp.X + disp.Y * disp.Y;

						DirType pitch;
						bool valid = Calculate_Projectile_Pitch(Should_Use_High_Arc(0), speed, fastmath::sqrt(distance2), disp.Z, Rule->Gravity, pitch);
						if (!valid) {
							valid = Calculate_Projectile_Pitch(Should_Use_High_Arc(0), speed * 10 / 8, fastmath::sqrt(distance2), disp.Z, Rule->Gravity, pitch);
							if (!valid) {
								pitch = DirType(Dir256(DIR_NW + 26));
								valid = true;
							}
						}

						velocity.Set_Pitch(pitch);

						if (valid && bullet->Unlimbo(ucoord, velocity)) {
							if (Get_Class_Weapon_Data(0)->BarrelLength > 0) {
								bullet->AI();
								if (bullet->IsActive) {
									bullet->AI();
								}
							}

							if (Is_Turret_Equipped()) {
							} // RA2: no IsInRecoilState (not in RA2's layout)
							Sound_Effect((VocType)weap->Sound.Pick(NonCriticalRandomNumber), Fire_Coord(0));
							Status = DONE;
							return(1);
						} else {
							bullet->Release();
							Begin_Mode(BSTATE_IDLE);	// keep the door closed.
							Assign_Mission(MISSION_GUARD);
							return(4 * TICKS_PER_SECOND);
						}
					}
				}
				Begin_Mode(BSTATE_IDLE);	// keep the door closed.
				Assign_Mission(MISSION_GUARD);
				return(4 * TICKS_PER_SECOND);

			case DONE:
				BarrelPitch.Set_Desired(DIR_E);
				Begin_Mode(BSTATE_IDLE);	// keep the door closed.
				Assign_Mission(MISSION_GUARD);
				return(4 * TICKS_PER_SECOND);
		}
	}

	return(Current_Mission_Control().Normal_Delay());
}
#endif


/***********************************************************************************************
 * BuildingClass::Revealed -- Reveals the building to the specified house.                     *
 *                                                                                             *
 *    This routine will reveal the building to the specified house. It will handle updating    *
 *    the sidebar for player owned buildings. A player owned building that hasn't been         *
 *    revealed, is in a state of pseudo-limbo. It cannot be used for any of its special        *
 *    abilities even though it exists on the map for all other purposes.                       *
 *                                                                                             *
 * INPUT:   house -- The house that this building is being revealed to.                        *
 *                                                                                             *
 * OUTPUT:  Was this building revealed by this procedure?                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool BuildingClass::Revealed(HouseClass * house)
{
	if (BASECLASS::Revealed(house)) {
		if (!ScenarioInit) {
			House->JustBuiltStructure = Class->HeapID;
			House->IsBuiltSomething = true;
		}
		House->IsRecalcNeeded = true;
		if (!HasAnyLink() && House->Is_Human_Player() && Get_Mission() != MISSION_CONSTRUCTION && MissionQueue != MISSION_CONSTRUCTION) { // RA2: Revealed at 0x448FE0 matches only so
			Grand_Opening();
		} else {
			if (!HasAnyLink() && !House->Is_Human_Player() && house == House && Get_Mission() != MISSION_CONSTRUCTION) { // RA2: as above
				Grand_Opening();
			}
		}
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * BuildingClass::Enter_Idle_Mode -- The building will enter its idle mode.                    *
 *                                                                                             *
 *    This routine is called when the exact mode of the building isn't known. By examining     *
 *    the building's condition, this routine will assign an appropriate mission.               *
 *                                                                                             *
 * INPUT:   initial  -- This this being called during scenario init?                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool BuildingClass::Enter_Idle_Mode(bool initial, bool)
{
	MissionType mission = MISSION_GUARD;
	if (!initial || ScenarioInit || Debug_Map) {
		Begin_Mode(BSTATE_IDLE);
		mission = MISSION_GUARD;
	} else {
		Begin_Mode(BSTATE_CONSTRUCTION);
		mission = MISSION_CONSTRUCTION;
	}
	QueueMission(mission, false); // RA2: Enter_Idle_Mode at 0x4490B0 matches only so
	return(false);
}


/***********************************************************************************************
 * BuildingClass::Pip_Count -- Determines "full" pips to display for building.                 *
 *                                                                                             *
 *    This routine will determine the number of pips that should be filled in when rendering   *
 *    the building.                                                                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the number of pips to display as filled in.                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/28/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int BuildingClass::Pip_Count(void) const
{
	switch (Class->PipScale) {
		case PIPSCALE_TIBERIUM:
			if (Class->IsWeeder) {
				return(int(Class->Max_Pips() * House->Weed_Fraction()));
			}
			return(0); // RA2: Pip_Count at 0x449110 matches only so
	}
	return(BASECLASS::Pip_Count()); // RA2: Pip_Count at 0x449110 has no Class, House
}


/***********************************************************************************************
 * BuildingClass::Death_Announcement -- Announce the death of this building.                   *
 *                                                                                             *
 *    This routine is called when the building is destroyed by "unnatural" means. Typically    *
 *    as a result of combat. If the building is known to the player, then it should be         *
 *    announced.                                                                               *
 *                                                                                             *
 * INPUT:   source   -- The object most directly responsible for the building's death.         *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/04/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void BuildingClass::Death_Announcement(TechnoClass const * source) const
{
	if (source != NULL && House->Is_Player_Control()) {
		Record_Event_Cell(Destination_Coord().As_Cell()); // RA2: the ring of recent event cells, no speech
	}
}



/***********************************************************************************************
 * BuildingClass::Fire_Direction -- Fetches the direction of firing.                           *
 *                                                                                             *
 *    This routine will return with the default direction to use when firing from this         *
 *    building. This is the facing of the turret except for the case of non-turret equipped    *
 *    buildings that have a weapon (e.g., guard tower).                                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the default firing direction for this building.                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/04/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: 8 bytes differ (adopted text)
DirType BuildingClass::Fire_Direction(void) const
{
	if (TarCom == NULL) {
		return((Dir256)DirType(PrimaryFacing.Current()).Round_To_256());
	}
	if (Is_Turret_Equipped() && !Class->IsTurretAnimAVoxel) {
		return(PrimaryFacing.Current().As_Dir32());
	}
	return(Aim_Direction(TarCom));
}
#endif











/***********************************************************************************************
 * BuildingClass::Mission_Unload -- Handles the unload mission for a building.                 *
 *                                                                                             *
 *    This is the unload mission for a building. This really only applies to the weapon's      *
 *    factory, since it needs the sophistication of an unload mission due to the door          *
 *    animation.                                                                               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of game frames to delay before calling this routine        *
 *          again.                                                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int BuildingClass::Do_MISSION_UNLOAD(void)
{
	if (Occupants.Count() > 0) { // RA2: a garrisoned building empties first
		Eject_Occupants(false); // RA2: as above
	}

	if (Class->IsWeaponsFactory) {
		Coord coord(0, 0, 0); // RA2: the exit point is looked up only for a land factory
		if (!Class->Naval) { // RA2: as above
			Cell exitcell(Class->ExitList[10]); // RA2: the eleventh exit entry
			Cell position = Get_Cell() + exitcell; // RA2: as above
			coord = Coord(Cell(position.X - 1, position.Y)); // RA2: one cell to the left
		}
		enum {
			INITIAL,
			CLEAR_BIB,
			OPEN,
			LEAVE,
			CLOSE
		};
		UnitClass * unit;
		switch (Status) {
			/*
			**	Start the door opening.
			*/
			case INITIAL:
				unit = (UnitClass *)Radio_Link(0); // RA2: the first radio link
				if (unit) {
					unit->QueueMission(MISSION_GUARD, false); // RA2: queued, not assigned
					unit->Commence();
				}
				Door.Open_Door(Class->DeployTime);
				Status = Class->Naval ? CLOSE : CLEAR_BIB; // RA2: a naval yard needs no bib or exit
				IsToDisplay = true;
				End_Anim(BANIM_IDLE); // RA2: the idle animation stops
				Begin_Anim(BANIM_PRODUCTION, HealthRatio <= Rule->ConditionYellow, false, 0); // RA2: the damaged state follows the health
				break;

			/*
			**	Now that the occupants can peek out the door, they will tell
			**	everyone that could be blocking the way, that they should
			**	scatter away.
			*/
			case CLEAR_BIB:
				if (!Clear_Weapons_Factory_Bib()) {
					DebugString("Weapons factory bib clear - kicking out unit\n");
					Status = OPEN;
				}
				break;

			/*
			**	When the door is finally open and the way is clear, tell the
			**	unit to drive out.
			*/
			case OPEN:
				if (Door.Is_Door_Open()) {
					unit = (UnitClass *)Radio_Link(0); // RA2: as above
					if (unit) {
						unit->QueueMission(MISSION_MOVE, false); // RA2: as above

						IPersistPtr persist(unit->Locomotion);
						CLSID clsid;
						persist->GetClassID(&clsid);

						if (clsid == CLSID_TunnelLocomotion || clsid == CLSID_TeleportLocomotion) { // RA2: a chrono miner unit drives out like a tunneler
							IPiggybackPtr piggy(unit->Locomotion);
							if (piggy != NULL && piggy->Is_Piggybacking()) {
								piggy->End_Piggyback(&unit->Locomotion);
							}
							ILocomotionPtr walk(CLSID_DriveLocomotion);
							walk->Link_To_Object(unit);
							piggy = IPiggybackPtr(walk);
							if (piggy != NULL) {
								piggy->Begin_Piggyback(unit->Locomotion);
								unit->Locomotion = walk;
								unit->Locomotion->Force_Track(DriveLocomotionClass::OUT_OF_WEAPON_FACTORY, coord);
							} else {
								int damage = unit->Strength;
								unit->Take_Damage(damage, 0, Rule->C4Warhead, NULL, true);
							}
						} else if (clsid != CLSID_DriveLocomotion) {
							unit->Assign_Destination(&Map[Get_Cell() + Cell(5, 1)], true); // RA2: five cells right
						} else {
							unit->Locomotion->Force_Track(DriveLocomotionClass::OUT_OF_WEAPON_FACTORY, coord); // RA2: as above
						}
						unit->Set_Speed(0.5);
						Status = LEAVE;
					} else {
						Door.Close_Door(Class->DeployTime);
						Status = CLOSE;
					}
				}
				break;

			/*
			**	Wait until the unit has completely left the building.
			*/
			case LEAVE:
				if (!IsTethered) {
					Door.Close_Door(Class->DeployTime);
					Status = CLOSE;
				}
				break;

			/*
			**	Wait while the door closes.
			*/
			case CLOSE:
				if (Door.Is_Door_Closed() || Class->Naval) { // RA2: a naval yard does not wait
					Enter_Idle_Mode(false, true); // RA2: as above
					IsToDisplay = true;
				}
				break;

			default:
				break;
		}
		return(Current_Mission_Control().Normal_Delay() + Random_Pick(0, 2));
	}

	if (Class->GapGenerator && Class->SuperGapRadiusInCells) { // RA2: a gap generator toggles its super charge
		if (!GapSuperCharged) { // RA2: as above
			DestroyGap(); // RA2: as above
			GapRadius = Class->SuperGapRadiusInCells; // RA2: as above
			if (IsPowerOnline()) { // RA2: as above
				CreateGap(); // RA2: as above
				GapSuperCharged = true; // RA2: as above
				HasExtraPowerDrain = true; // RA2: as above
				House->RecalcPower = true; // RA2: as above
			} else {
				GapRadius = Class->GapRadiusInCells; // RA2: as above
				CreateGap(); // RA2: as above
			}
		} else {
			DestroyGap(); // RA2: as above
			GapSuperCharged = false; // RA2: as above
			HasExtraPowerDrain = false; // RA2: as above
			GapRadius = Class->GapRadiusInCells; // RA2: as above
			CreateGap(); // RA2: as above
			House->RecalcPower = true; // RA2: as above
		}
	}

	QueueMission(MISSION_GUARD, false); // RA2: as above
	return(1);
}











/// <summary>
/// Is some other object standing in this building's footprint?
/// This routine is used by the gate logic to find out whether anything would be
/// crushed or trapped by closing.
/// </summary>
bool BuildingClass::Is_Blocked_By_Occupier(void)
{
	Cell cell = Get_Cell();
	Cell const * list = Occupy_List();
	bool blocked = false;
	while (*list != REFRESH_EOL) {
		Cell newcell = cell + *list;
		list++;
		ObjectClass * optr = Map[newcell].Cell_Occupier();
		while (optr != NULL) {
			if (this != optr) {
				blocked = true;
				break;
			}
			optr = optr->Next;
		}
	}
	return(blocked);
}


/// <summary>
/// Handles the gate opening and closing mission.
/// This routine walks a gate through its opening, open, and closing states, playing
/// the appropriate sound and animation frame for each. The gate holds itself open
/// while something stands in the way and closes again once its timer expires. A
/// building that is not a gate is put back onto guard duty.
/// </summary>
/// <returns>The delay in game frames before this mission should be processed again.</returns>
int BuildingClass::Do_MISSION_OPEN(void)
{
	if (Class->IsGate) {
		enum {
			START_OPENING,
			OPENING,
			OPEN,
			START_CLOSING,
			CLOSING,
			CLOSED
		};
		switch (Status) {
			case START_OPENING:
				if (Door.Is_Door_Open()) {
					GateTimer = int(Class->GateCloseDelay * TICKS_PER_MINUTE);
					Status = OPEN;
					break;
				}
				if (!Door.Is_Door_Opening()) {
					if (Door.Is_Door_Closing()) {
						Door.Reverse();
					} else {
						Door.Open_Door(Class->DeployTime);
						VocClass::PlayAt(Rule->GateDownSound, PositionCoord, NULL); // RA2: Do_MISSION_OPEN at 0x4499D0 matches only so
						Rect redrawrect = Get_Render_Rect();
						TacticalMap->Register_Dirty_Area(redrawrect, false); // RA2: as above
					}
				}
				Status = OPENING;
				GateTimer = int(Class->GateCloseDelay * TICKS_PER_MINUTE);
				return(0);

			case START_CLOSING:
				Door.Close_Door(Class->DeployTime);
				Status = CLOSING;
				VocClass::PlayAt(Rule->GateUpSound, PositionCoord, NULL); // RA2: as above
				return(0);

			case OPEN:
				if (!Is_Blocked_By_Occupier()) {
					if (GateTimer.Progress() == 1.0) {
						Status = START_CLOSING;
					}
				} else {
					GateTimer = int(Class->GateCloseDelay * TICKS_PER_MINUTE);
				}
				break;

			case OPENING:
				if (Door.Is_Door_Open()) {
					Status = OPEN;
				}

			case CLOSING:
				if (Door.Is_Door_Closed()) {
					Enter_Idle_Mode();
					Status = CLOSED;
					IsToDisplay = true;
				}
				if (!Door.Is_Ready_To_Open()) {
					int stage = int(Door.Percent_Complete() * Class->GateStages);
					if (stage != GateFrame) {
						GateFrame = stage;
						IsToDisplay = true;
					}
				}
				return(0);
		}

		return(Current_Mission_Control().Normal_Delay() + Random_Pick(0, 2));
	}

	QueueMission(MISSION_GUARD, false); // RA2: as above
	return(1);
}


/***********************************************************************************************
 * BuildingClass::Power_Output -- Fetches the current power output from this building.         *
 *                                                                                             *
 *    This routine will return the current power output for this building. The power output    *
 *    is adjusted according to the damage level of the building.                               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the current power output for this building.                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int BuildingClass::Power_Output(void) const
{
	int power = Class->Power;
	if (IsBeingWarpedOut()) { // RA2: none while warped out
		return(0); // RA2: as above
	}
	if (HasExtraPowerBonus) { // RA2: the extra power bonus
		power += Class->ExtraPower; // RA2: as above
	}
	if (UpgradeLevel != 0) {
		for (int i = 0; i < BUILDING_UPGRADE_MAX; i++) {
			if (Upgrades[i] != NULL) {
				power += Upgrades[i]->Power;
			}
		}
	}
	if (power > 0 && IsOn) {
		return(int(Get_Health_Ratio() * power)); // RA2: the ratio first
	}
	return(0);
}



/// <summary>
/// Fetches the power drain of this building.
/// This routine tallies the power this building takes from its house, including the
/// drain of any upgrades plugged into it. A building that has been switched off draws
/// nothing at all.
/// </summary>
/// <returns>Returns with the amount of power drained from the owning house.</returns>
int BuildingClass::Power_Drain(void) const
{
	if (!IsBeingWarpedOut() && IsOn) { // RA2: Power_Drain at 0x449DD0 matches only so
		int drain = Class->Drain;
		if (HasExtraPowerDrain) { // RA2: as above
			drain += Class->ExtraDrain; // RA2: as above
		}
		if (UpgradeLevel != 0) {
			for (int i = 0; i < BUILDING_UPGRADE_MAX; i++) {
				if (Upgrades[i] != NULL) {
					drain += Upgrades[i]->Drain;
				}
			}
		}
		return(drain);
	}
	return(0);
}


/***********************************************************************************************
 * BuildingClass::Detach -- Handles target removal from the game system.                       *
 *                                                                                             *
 *    This routine is called when the specified target is about to be removed from the game    *
 *    system.                                                                                  *
 *                                                                                             *
 * INPUT:   target   -- The target to be removed from this building's targeting computer.      *
 *                                                                                             *
 *          all      -- Is the target about to be completely eliminated?                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void BuildingClass::Detach(AbstractClass const * target, bool all)
{
	BASECLASS::Detach(target, all);

	if (WhomToRepay == target) {
		WhomToRepay = NULL;
	}
	if (AnimToTrack == target) {
		AnimToTrack = NULL;
	}
	if (Factory == target) {
		Factory = NULL;
	}
	if (BuildingLight == target) {
		BuildingLight = NULL;
	}
	if (LightSource == target) {
		LightSource = NULL;
	}
	if (Anims[BANIM_PRODUCTION] == target) { // RA2: the idle animation starts when the production animation (slot 8, +0x474) goes away
		Begin_Anim(BANIM_IDLE, HealthRatio <= Rule->ConditionYellow); // RA2: as above
	}
	if (target != NULL && ((AbstractClass *)target)->What_Am_I() == RTTI_ANIM) { // RA2: no IsActive test
		for (int index = 0; index < 8; index++) { // RA2: a damage fire animation is just forgotten
			if (DamageFireAnims[index] == target) {
				DamageFireAnims[index] = NULL;
				return;
			}
		}
		if (((AnimClass *)target)->IsBuildingAnim) { // RA2: only building animations go through Detach_Anim
			Detach_Anim((AnimClass *)target);
			return;
		}
	}
	if (Class == target) {
		Class = NULL;
	}
	for (int index = 0; index < BUILDING_UPGRADE_MAX; index++) {
		if (Upgrades[index] == target) {
			Upgrades[index] = NULL;
		}
	}
	Overpowerers.Delete((InfantryClass *)target); // RA2: the overpowerers forget it too
}




/***********************************************************************************************
 * BuildingClass::Crew_Type -- This determines the crew that this object generates.            *
 *                                                                                             *
 *    When selling very cheap buildings (such as the silo), a technician will pop out since    *
 *    generating minigunners would be overkill -- the player could use this loophole to        *
 *    gain an advantage.                                                                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the infantry type that this building will generate as a survivor.          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/05/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
InfantryTypeClass const * BuildingClass::Crew_Type(void) const
{
	if (!IsCaptured && Percent_Chance(25) && Class->ToBuild == RTTI_BUILDINGTYPE) {
		return(Rule->Engineer);
	}
	return(BASECLASS::Crew_Type());
}


/// <summary>
/// Abandons what the factory produces when, without this building, nothing of its house can
/// build that type any more.
/// </summary>
/// <param name="factory">The factory to check; may be NULL.</param>
void BuildingClass::Abandon_Orphaned_Production(FactoryClass * factory) // RA2: not in TibSun, at 0x44A000
{
	if (factory != NULL) {
		TechnoClass * object = factory->Get_Object();
		bool limbo = IsInLimbo;
		IsInLimbo = true;
		if (object != NULL) {
			if (object->Techno_Type_Class()->Who_Can_Build_Me(true, false, false, House) == NULL) {
				House->Abandon_Production(Class->ToBuild, object->Techno_Type_Class()->Fetch_Heap_ID(), false);
			}
		}
		IsInLimbo = limbo;
	}
}





/***********************************************************************************************
 * BuildingClass::Detach_All -- Possibly abandons production according to factory type.        *
 *                                                                                             *
 *    When this routine is called, it indicates that the building is about to be destroyed     *
 *    or captured. In such a case any production it may be doing, must be abandoned.           *
 *                                                                                             *
 * INPUT:   all   -- Is the object about the be completely destroyed?                          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/05/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void BuildingClass::Detach_All(bool all)
{
	if (all) {
		/*
		**	If it is producing something, then it must be abandoned.
		*/
		if (Factory) {
			Factory->Abandon();
			delete (FactoryClass *)Factory;
			Factory = 0;
		}

		/*
		**	If the owner HouseClass is building something, and this building can
		**	build that thing, we may be the last building for that house that can
		**	build that thing; if so, abandon production of it.
		*/
		if (House) {
			if (Class->ToBuild == RTTI_BUILDINGTYPE) { // RA2: a construction yard checks all five building queues
				Abandon_Orphaned_Production(House->BuildingFactory); // RA2: as above
				Abandon_Orphaned_Production(House->Primary_Unused1); // RA2: as above
				Abandon_Orphaned_Production(House->Primary_Unused2); // RA2: as above
				Abandon_Orphaned_Production(House->Primary_Unused3); // RA2: as above
				Abandon_Orphaned_Production(House->Primary_ForDefenses); // RA2: as above
			} else {
				Abandon_Orphaned_Production(House->Fetch_Factory(Class->ToBuild, Class->Naval, 0)); // RA2: the factory of this building's kind, naval or not
			}
		}
	}

	if (!all) {
		for (int index = 0; index < RadioLinks.Length(); index++) { // RA2: every radio contact, not just the first
			TechnoClass * contact = Radio_Link(index); // RA2: as above
			if (contact != NULL && !House->Is_Ally(contact)) { // RA2: as above
				Transmit_Message(RADIO_OVER_OUT, contact); // RA2: as above
			}
		}
	} else {
		SendToEachLink(RADIO_OVER_OUT); // RA2: all links are told
	}

	BASECLASS::Detach_All(all);
}


/***********************************************************************************************
 * BuildingClass::Flush_For_Placement -- Handles clearing a zone for object placement.         *
 *                                                                                             *
 *    This routine is used to clear the way for placement of the specified object (usually     *
 *    a building). If there are friendly units blocking the placement area, they are told      *
 *    to scatter. Enemy blocking units are attacked.                                           *
 *                                                                                             *
 * INPUT:   techno   -- Pointer to the object that is desired to be placed.                    *
 *                                                                                             *
 *          cell     -- The cell that placement wants to occur at.                             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/06/1995 JLB : Created.                                                                 *
 *   09/27/1995 JLB : Revised to use type class function.                                      *
 *=============================================================================================*/
int BuildingClass::Flush_For_Placement(TechnoClass * techno, Cell const & cell)
{
	if (techno) {
		return(((BuildingTypeClass const *)techno->Class_Of())->Flush_For_Placement(cell, House));
	}
	return(-1);
}


/***********************************************************************************************
 * BuildingClass::Find_Exit_Cell -- Find a clear location to exit an object from this building *
 *                                                                                             *
 *    This routine is called when the building needs to discharge a unit. It will find a       *
 *    nearby (adjacent) cell that is clear enough for the specified object to enter. Typical   *
 *    use of this routine is when the airfield disgorges its cargo.                            *
 *                                                                                             *
 * INPUT:   techno   -- Pointer to the object that wishes to exit this building.               *
 *                                                                                             *
 * OUTPUT:  Returns with the cell number to use for object placement. If no free location      *
 *          could be found, then zero (0) is returned.                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *   02/20/1996 JLB : Added default case for exit cell calculation.                            *
 *=============================================================================================*/
Cell BuildingClass::Find_Exit_Cell(TechnoClass const * techno) // RA2: not const
{
	Cell origin = Get_Cell(); // RA2: as above
	Cell cell; // RA2: as above
	if (Class->IsGDIBarracks) {
		cell = origin + Cell(1, 2); // RA2: as above
		if (Map.In_Radar(cell) && techno->Can_Enter_Cell(&Map[cell], FACING_NONE, -1, 0, 1) == MOVE_OK) // RA2: as above
			return cell; // RA2: as above
	}
	if (Class->IsNODBarracks) {
		cell = origin + Cell(2, 2); // RA2: as above
		if (Map.In_Radar(cell) && techno->Can_Enter_Cell(&Map[cell], FACING_NONE, -1, 0, 1) == MOVE_OK) // RA2: as above
			return cell; // RA2: as above
	}
	if (Class->Naval && Class->IsWeaponsFactory) { // RA2: as above
		Cell dock = Cell(Docking_Coord((TechnoClass *)techno)); // RA2: as above
		cell = dock; // RA2: as above
		cell += Cell(1, 1); // RA2: as above
		if (Map.In_Radar(cell) && techno->Can_Enter_Cell(&Map[cell], FACING_NONE, -1, 0, 0) == MOVE_OK) // RA2: as above
			return cell; // RA2: as above
		--cell.Y; // RA2: as above
		if (Map.In_Radar(cell) && techno->Can_Enter_Cell(&Map[cell], FACING_NONE, -1, 0, 0) == MOVE_OK) // RA2: as above
			return cell; // RA2: as above
		++cell.Y; // RA2: as above
		--cell.X; // RA2: as above
		if (Map.In_Radar(cell) && techno->Can_Enter_Cell(&Map[cell], FACING_NONE, -1, 0, 0) == MOVE_OK) // RA2: as above
			return cell; // RA2: as above
	}
	Cell const *offset = Class->ExitList; // RA2: as above
	if (!offset || Class->IsHospital) { // RA2: as above
		int bottom = Class->Height(); // RA2: as above
		for (int x = -1; x <= Class->Width(); ++x) { // RA2: as above
			cell = origin + Cell(x, bottom); // RA2: as above
			if (Map.In_Radar(cell) && techno->Can_Enter_Cell(&Map[cell], FACING_NONE, -1, 0, 1) == MOVE_OK) // RA2: as above
				return cell; // RA2: as above
			cell = origin + Cell(x, -1); // RA2: as above
			if (Map.In_Radar(cell) && techno->Can_Enter_Cell(&Map[cell], FACING_NONE, -1, 0, 1) == MOVE_OK) // RA2: as above
				return cell; // RA2: as above
		}
		int right = Class->Width(); // RA2: as above
		for (int y = -1; y <= Class->Height(); ++y) { // RA2: as above
			cell = origin + Cell(right, y); // RA2: as above
			if (Map.In_Radar(cell) && techno->Can_Enter_Cell(&Map[cell], FACING_NONE, -1, 0, 1) == MOVE_OK) // RA2: as above
				return cell; // RA2: as above
			cell = origin + Cell(-1, y); // RA2: as above
			if (Map.In_Radar(cell) && techno->Can_Enter_Cell(&Map[cell], FACING_NONE, -1, 0, 1) == MOVE_OK) // RA2: as above
				return cell; // RA2: as above
		}
	} else {
		while (offset->X != 0x7fff || offset->Y != 0x7fff) { // RA2: as above
			cell = origin + *offset++; // RA2: as above
			if (Map.In_Radar(cell) && techno->Can_Enter_Cell(&Map[cell], FACING_NONE, -1, 0, 0) == MOVE_OK) // RA2: as above
				return cell; // RA2: as above
		}
	}
	return CELL_NONE; // RA2: as above
}



/***********************************************************************************************
 * BuildingClass::Can_Player_Move -- Can this building be moved?                               *
 *                                                                                             *
 *    This routine answers the question 'can this building be moved?' Typically, only the      *
 *    construction yard can be moved and it does this by undeploying back into a MCV.          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Can the building move to a new location under player control?                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/04/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool BuildingClass::Can_Player_Move(void) const
{
	if (StunDuration > 0) {
		return(false);
	}
	if (Is_Move_Override()) { // RA2: factories, cloning vats and repair depots can always be moved to
		return(true); // RA2: as above
	}
	if (Class->IsConstructionYard && (Session.Type == GAME_NORMAL || !Session.Options.MCVRedeploy || !House->Is_Human_Player())) {
		return(false);
	}
	return(Class->UndeploysInto != NULL);
}



/***********************************************************************************************
 * BuildingClass::Exit_Coord -- Determines location where object will leave it.                *
 *                                                                                             *
 *    This routine will return the coordinate where an object that wishes to leave the         *
 *    building will exit at.                                                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the coordinate that the object should be created at.                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/20/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
Coord BuildingClass::Exit_Coord(int) const // RA2: an index
{
	if (Class->ExitCoordinate != COORD_NONE) { // RA2: no weapons factory exit
		Coord coord = PositionCoord; // RA2: as above
		return(coord + Class->ExitCoordinate); // RA2: as above
	}

	return(Center_Coord());
}



/***********************************************************************************************
 * BuildingClass::Check_Point -- Fetches the landing checkpoint for the given flight pattern.  *
 *                                                                                             *
 *    Use this routine to coordinate a landing operation. The specified checkpoint is          *
 *    converted into a cell number. The landing aircraft should fly over that cell and then    *
 *    request the next check point.                                                            *
 *                                                                                             *
 * INPUT:   cp    -- The check point to convert to a cell number.                              *
 *                                                                                             *
 * OUTPUT:  Returns with the cell that the aircraft should fly over in order to complete       *
 *          that portion of the landing pattern.                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
Cell BuildingClass::Check_Point(CheckPointType cp) const
{
	int xoffset = 6; // RA2: Check_Point at 0x44AA40 matches only so
	int yoffset = 5; // RA2: as above
	Cell cell = Center_Coord().As_Cell();

	switch (cp) {
		case CHECK_STACK:
			xoffset = 0;
			break;

		case CHECK_CROSSWIND:
			yoffset = 0;
			break;

		case CHECK_DOWNWIND:
		default:
			break;
	}

	if ((cell.X - Map.MapRect.X) > Map.MapRect.Width/2)  {
		xoffset = -xoffset;
	}

	if ((cell.Y - Map.MapRect.Y) > Map.MapRect.Height/2)  {
		yoffset = -yoffset;
	}

	return(cell + Cell(xoffset, yoffset));
}


/***********************************************************************************************
 * BuildingClass::Update_Radar_Spied - set house's RadarSpied field appropriately.             *
 *                                                                                             *
 *    This routine is called when a radar facility is captured or destroyed.  It fills in the  *
 *    RadarSpied field of the house based on whether there's a spied-upon radar facility or not*
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  House->RadarSpied field gets set appropriately.                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/22/1996 BWG : Created.                                                                 *
 *=============================================================================================*/
void BuildingClass::Update_Radar_Spied(void)
{
	House->RadarSpied = 0;
	int count = House->Buildings.Count(); // RA2: Update_Radar_Spied at 0x44AB00 matches only so
	for (int index = 0; index < count; index++) { // RA2: as above
		BuildingClass * obj = House->Buildings[index]; // RA2: as above
		if (obj && !obj->IsInLimbo && obj->Class->IsRadar) { // RA2: as above
			House->RadarSpied |= obj->SpiedBy; // RA2: as above
		}
	}
	Map.Flag_To_Redraw(GS_REDRAW_ALL);
}


/***********************************************************************************************
 * BuildingClass::Read_INI -- Reads buildings from INI file.                                   *
 *                                                                                             *
 *    This is the basic scenario initialization of building function. It                       *
 *    is called when reading the scenario startup INI file and it handles                      *
 *    creation of all specified buildings.                                                     *
 *                                                                                             *
 *    INI entry format:                                                                        *
 *      Housename, Typename, Strength, Cell, Facing, Triggername                               *
 *                                                                                             *
 * INPUT:   buffer   -- Pointer to the loaded INI file data.                                   *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void BuildingClass::Read_INI(CCINIClass const & ini)
{
	BuildingClass			* b;        // Working unit pointer.
	HousesType				bhouse;     // Building house.
	HouseClass				* bhptr;    // Building house.
	StructType				classid;    // Building type.
	Cell						cell;   // Cell of building.
	char						buf[128];
	char						* trigname;	// building's trigger's name

	int i;
	StructType upgrades[ARRAY_SIZE(b->Upgrades)];

	for (i = 0; i < ARRAY_SIZE(upgrades); i++) {
		upgrades[i] = STRUCT_NONE;
	}

	int len = ini.Entry_Count(INI_NAME);
	for (int index = 0; index < len; index++) {
		char const * entry = ini.Get_Entry(INI_NAME, index);

		/*
		**	Get a building entry.
		*/
		ini.Get_String(INI_NAME, entry, NULL, buf, sizeof(buf));

		/*
		**	1st token: house name.
		*/
		bhouse = (HousesType)House_Index_From_Name(strtok(buf, ",")); // RA2: the house index is looked up by name, then taken from Houses
		if (bhouse == HOUSE_NONE) { // RA2: as above
			continue; // RA2: as above
		}
		bhptr = Houses[bhouse]; // RA2: as above

		if (bhptr == NULL) {
			continue;
		}

		if (Session.Type != GAME_NORMAL && bhptr == PlayerPtr) {
			continue;
		}

		/*
		**	2nd token: building name.
		*/
		classid = BuildingTypeClass::From_Name(strtok(NULL, ","));

		if (bhouse != HOUSE_NONE && classid != STRUCT_NONE) {
			int	strength;
			Dir256 facing;

			/*
			**	3rd token: strength.
			*/
			strength = atoi(strtok(NULL, ","));

			/*
			**	4th token: cell #.
			*/
			if (NewINIFormat >= 4) {
				Cell parsed;
				parsed.X = (short)atoi(strtok(NULL, ",")); // RA2: the coordinates are stored as shorts
				parsed.Y = (short)atoi(strtok(NULL, ",")); // RA2: as above
				cell = parsed; // RA2: as above
			} else {
				int packed = atoi(strtok(NULL, ",")); // RA2: the packed cell number is split by hand
				cell = Cell((short)(packed % 128), (short)(packed / 128)); // RA2: as above
			}

			/*
			**	5th token: facing.
			*/
			facing = (Dir256)atoi(strtok(NULL, ","));

			/*
			**	6th token: triggername (can be NULL).
			*/
			trigname = strtok(NULL, ",");

			bool sellable = false;
			char * token_pointer = strtok(NULL, ",");
			if (token_pointer) {
				sellable = atoi(token_pointer) != 0;
			}

			bool rebuild = false;
			token_pointer = strtok(NULL, ",");
			if (token_pointer) {
				rebuild = atoi(token_pointer) != 0;
			}

			bool online = true;
			token_pointer = strtok(NULL, ",");
			if (token_pointer) {
				online = atoi(token_pointer) != 0;
			}

			int upgradecount = 0;
			token_pointer = strtok(NULL, ",");
			if (token_pointer) {
				upgradecount = atoi(token_pointer);
			}

			int spotlight = LIGHT_BEHAVIOR_SWEEP;
			token_pointer = strtok(NULL, ",");
			if (token_pointer) {
				spotlight = atoi(token_pointer);
			}

			for (i = 0; i < ARRAY_SIZE(upgrades); i++) {
				token_pointer = strtok(NULL, ",");
				if (token_pointer) {
					upgrades[i] = BuildingTypeClass::From_Name(token_pointer);
				}
			}

			bool repair = false;
			token_pointer = strtok(NULL, ",");
			if (token_pointer) {
				repair = atoi(token_pointer) != 0;
			}

			bool nominal = false;
			token_pointer = strtok(NULL, ",");
			if (token_pointer) {
				nominal = atoi(token_pointer) != 0;
			}

			b = new BuildingClass(BuildingTypes[classid], bhptr);
			if (b) {

				TagTypeClass * tp = TagTypeClass::From_Name(trigname);
				if (tp) {
					TagClass * tt = Find_Or_Make(tp);
					if (tt) {
						b->Attach_Tag(tt);
					}
				}
				b->IsAllowedToSell = sellable;
				b->IsToRebuild = rebuild;
				b->IsToRepair = repair;
				b->IsNominal = nominal;

				strength = MIN(strength, 256);
				strength = b->Class->MaxStrength * (strength / 256.0);
				b->Strength = strength;
				if (b->Strength > b->Class->MaxStrength-3) b->Strength = b->Class->MaxStrength;
				b->EstimatedHealth = b->Strength; // RA2: the estimated health starts at the strength

				if (b->Unlimbo(Coord(cell), facing)) { // RA2: the cell is converted to a coordinate

					if (b->BuildingLight) {
						b->BuildingLight->Set_Behavior_Type((LightBehaviorType)spotlight);
					}

					if (online) {
						if (!b->IsOn && b->StunDuration == 0) {
							BuildingClass * building = b; // RA2: Turn_On is called through a copy of the pointer
							building->Turn_On(); // RA2: as above
						}
					} else {
						b->Turn_Off();
					}

					b->IsALemon = false;

					if (b->Class->NeedsEngineer && !b->HasEngineer) { // RA2: a building that needs an engineer starts without power
						b->Power_Off(); // RA2: as above
					}

					if (upgradecount) {
						for (i = 0; i < upgradecount; i++) {
							if (upgrades[i] != STRUCT_NONE) {
								BuildingTypeClass *utype = BuildingTypes[upgrades[i]];
								BuildingClass *u = new BuildingClass(utype, bhptr);
								u->Unlimbo(Coord(cell), facing); // RA2: as above
							}
						}
					}
					if (b->Is_Turret_Equipped()) { // RA2: no charge anim test
						b->Set_Turret_Frame();
					}
				} else {

					/*
					**	If the building could not be unlimboed on the map, then this indicates
					**	a serious error. Delete the building.
					*/
					delete b;
				}
			}
		}
	}
}


/// <summary>
/// Writes all of the buildings out to the INI file specified.
/// This routine is used when saving a scenario. Buildings that are in limbo and laser
/// fence segments are skipped, since those are recreated rather than stored.
/// </summary>
/// <param name="ini">The INI file to write the building data to.</param>
void BuildingClass::Write_All(CCINIClass & ini)
{
	ini.Clear(INI_NAME);

	for (int index = 0; index < Buildings.Count(); index++) {
		BuildingClass * building = Buildings[index];
		if (!building->IsInLimbo && !building->Class->IsLaserFence) {
			building->Write_INI(ini);
		}
	}
}


/***********************************************************************************************
 * BuildingClass::Write_INI -- Write out the building data to the INI file specified.          *
 *                                                                                             *
 *    This will store the building data (as it relates to scenario initialization) to the      *
 *    INI database specified.                                                                  *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database that the building data will be stored to.   *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void BuildingClass::Write_INI(CCINIClass & ini)
{
	char	uname[10];
	char	buf[127];

	sprintf(uname, "%d", Fetch_ID());
	int behavior;
	if (BuildingLight != NULL) {
		behavior = BuildingLight->Behavior;
	} else {
		behavior = 0;
	}

	char const * tag_name;
	if (Tag != NULL && Tag->Class != NULL) {
		tag_name = (char const *)Tag->Class->IniName;
	} else {
		tag_name = "None";
	}
	sprintf(buf, "%s,%s,%d,%d,%d,%d,%s,%d,%d,%d,%d,%d", // RA2: Write_INI at 0x44B250 has no Dir256, PrimaryFacing, Current
		(char const *)House->IniName, // RA2: Write_INI at 0x44B250 matches only so
		(char const *)Class->IniName,
		(int)(HealthRatio*256 + .5),
		Get_Cell().X,
		Get_Cell().Y,
		PrimaryFacing.Current().As_Dir256(), // RA2: as above
		tag_name,
		IsAllowedToSell,
		IsToRebuild,
		IsOn,
		UpgradeLevel,
		behavior
	);

	for (int i = 0; i < BUILDING_UPGRADE_MAX; i++) {
		sprintf(&buf[strlen(buf)], ",%s", Upgrades[i] != NULL ? (char const *)Upgrades[i]->IniName : "None");
	}

	sprintf(&buf[strlen(buf)], ",%d", IsToRepair != 0);
	sprintf(&buf[strlen(buf)], ",%d", IsNominal != 0);
	ini.Put_String(INI_NAME, uname, buf);
}


/***********************************************************************************************
 * BuildingClass::Target_Coord -- Return the coordinate to use when firing on this building.   *
 *                                                                                             *
 *    This routine will determine the "center" location of this building for purposes of       *
 *    targeting. Usually, this location is somewhere near the foundation of the building.      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the coordinate to use when firing upon this building (or trying to    *
 *          walk onto it).                                                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
Coord BuildingClass::Target_Coord(void) const
{
	Coord center = Center_Coord(); // RA2: Target_Coord at 0x44B430 matches only so
	return(Class->TargetCoordOffset + center); // RA2: as above
}


/***********************************************************************************************
 * BuildingClass::Factory_AI -- Handle factory production and initiation.                      *
 *                                                                                             *
 *    Some building (notably the computer controlled ones) can have a factory object attached. *
 *    This routine handles processing of that factory and also detecting when production       *
 *    should begin in order to initiate production.                                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Only call this routine once per building per game logic loop.                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void BuildingClass::Factory_AI(void)
{
	/*
	**	Handle any production tied to this building. Only computer controlled buildings have
	**	production attached to the building itself. The player uses the sidebar interface for
	**	all production control.
	*/
	if (Factory != NULL && Factory->Has_Completed() && PlacementDelay == 0) {
		TechnoClass * product = Factory->Get_Object();
//		FactoryClass * fact = Factory;

		switch (Exit_Object(product)) {

			/*
			**	If the object could not leave the factory, then either request
			**	a transport, place the (what must be a) building using another method, or
			**	abort the production and refund money.
			*/
			case 0:
				if (product->Fetch_RTTI() == RTTI_BUILDING && ((BuildingClass *)product)->Class->Naval) { // RA2: a naval building that cannot be placed stops the AI's naval base planning
					product->Owner_HouseClass()->IsNavalBaseAllowed = false; // RA2: as above
				}
				Factory->Abandon();
				delete (FactoryClass *)Factory;
				Factory = 0;
				break;

			/*
			**	Exiting this building is prevented by some temporary blockage. Wait
			**	a bit before trying again.
			*/
			case 1:
				PlacementDelay = int(Rule->PlacementDelay * TICKS_PER_MINUTE);
				break;

			/*
			**	The object was successfully sent from this factory. Inform the house
			**	tracking logic that the requested object has been produced.
			*/
			case 2: { // RA2: braces for the sound's coordinate copy
				Coord location = Position; // RA2: the building's own position is copied for the sound
				VocClass::PlayIndexAtPos(Class->CreateUnitSound, location, 0); // RA2: the building's create-unit sound plays when the object leaves
				House->Just_Built(product);
//				fact->Completed();
				Factory->Completed();
//				delete fact;
				delete (FactoryClass *)Factory;
				Factory = 0;
				break;
			} // RA2: as above

			default:
				break;
		}
	}

	/*
	**	Pick something to create for this factory.
	*/
	if (House->IsStarted && Mission != MISSION_CONSTRUCTION && Mission != MISSION_DECONSTRUCTION) {

		/*
		**	Buildings that produce other objects have special factory logic handled here.
		*/
		if (Class->ToBuild != RTTI_NONE) {
			if (Factory != NULL) {

				/*
				**	If production has halted, then just abort production and make the
				**	funds available for something else.
				*/
				if (PlacementDelay == 0 && !Factory->Is_Building()) {
					Factory->Abandon();
					delete (FactoryClass *)Factory;
					Factory = 0;
				}

			} else {

				/*
				**	Only look to start production if there is at least a small amount of
				**	money available. In cases where there is no practical money left, then
				**	production can never complete -- don't bother starting it.
				*/
				if (House->IsStarted && House->Available_Money() > 10) {
					TechnoTypeClass const * techno = House->Suggest_New_Object(Class->ToBuild, false);

					/*
					**	If a suitable object type was selected for production, then start
					**	producing it now.
					*/
					if (techno != NULL) {
						if (techno->Fetch_RTTI() == RTTI_UNITTYPE) { // RA2: a naval factory builds only ships and a land factory only land units
							if (Class->Naval && !techno->Naval) { // RA2: as above
								return; // RA2: as above
							}
							if (techno->Naval && !Class->Naval) { // RA2: as above
								return; // RA2: as above
							}
						}
						Factory = new FactoryClass;
						if (Factory != NULL) {
							if (!Factory->Set(*techno, *House, false)) {
								delete (FactoryClass *)Factory;
								Factory = 0;
							} else {
								House->Production_Begun(Factory->Get_Object());
								Factory->Start(false);
							}
						}
					}
				}
			}
		}
	}
}


/// <summary>
/// RA2: an empty function nothing calls.
/// </summary>
void __cdecl Building_Empty_Function(void) // RA2: not in TibSun, at 0x44B760
{
}


/// <summary>
/// Counts down a delayed shot: at zero, stage 1 fires the recorded weapon at the target (each supporting
/// prism tower raising its damage) and stage 2 sends the support beam to the recorded coordinate.
/// </summary>
void BuildingClass::Delayed_Fire_AI(void) // RA2: not in TibSun, at 0x44B770 (RA2 copies PrismTargetCoords field by field)
{
	if (PrismStage != 0) {
		if (--DelayBeforeFiring <= 0) {
			DelayBeforeFiring = 0;
			switch (PrismStage) {
				case 1:
					if (TarCom != NULL && Can_Fire(TarCom, PrismTargetCoords.X, true) == FIRE_OK) {
						BulletClass * bullet = Fire_At(TarCom, PrismTargetCoords.X);
						if (bullet != NULL && SupportingPrisms != 0) {
							bullet->DamageMultiplier = (unsigned)((Rule->PrismSupportModifier * SupportingPrisms + 100) * 256) / 100;
							SupportingPrisms = 0;
						}
					}
					break;
				case 2:
					Fire_Prism_Support(PrismTargetCoords); // RA2: as TibSun, the flat struct converts to the Coord argument
					break;
			}
			PrismStage = 0;
		}
	}
}


/// <summary>
/// Records a delayed shot (the stage, the type's delay and the coordinate) and starts the delayed-fire
/// animation in place of the idle one.
/// </summary>
void BuildingClass::Start_Delayed_Fire(int stage, Coord const & coord) // RA2: not in TibSun, at 0x44B870
{
	DelayBeforeFiring = Class->DelayedFireDelay;
	PrismStage = stage;
	memcpy(&PrismTargetCoords, &coord, sizeof(PrismTargetCoords));
	End_Anim((BAnimType)3);
	Begin_Anim((BAnimType)10, HealthRatio <= Rule->ConditionYellow, false, 0);
}


/// <summary>
/// Drops overpowerers that no longer target this building; it is overpowered with three, or with one
/// while switched on and its house at full power.
/// </summary>
void BuildingClass::Update_Overpowerers(void) // RA2: not in TibSun, at 0x44B910
{
	for (int index = Overpowerers.Count() - 1; index >= 0; index--) {
		if (((TechnoClass *)Overpowerers[index])->TarCom != this) {
			Overpowerers.Delete_Index(index);
		}
	}
	int count = Overpowerers.Count();
	if (count >= 3 || (House->Power_Fraction() == 1.0 && IsOn && count >= 1)) {
		IsOverpowered = true;
	} else {
		IsOverpowered = false;
	}
}














/***********************************************************************************************
 * BuildingClass::Repair_AI -- Handle the repair (and sell) logic for the building.            *
 *                                                                                             *
 *    This routine handle the repair animation and healing logic. It also detects when the     *
 *    (computer controlled) building should begin repair or sell itself.                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Only call this routine once per building per game logic loop.                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void BuildingClass::Repair_AI(void)
{
	if (House->IQ >= Rule->IQRepairSell && Get_Mission() != MISSION_CONSTRUCTION && Get_Mission() != MISSION_DECONSTRUCTION && Can_Repair()) { // RA2: RA2's form
		if (House->Available_Money() >= Rule->RepairThreshhold) { // RA2: as above
			if (!House->DidRepair && !IsRepairing && (IsCaptured || IsToRepair || House->Is_Human_Player())) { // RA2: as above
				House->DidRepair = true; // RA2: as above
				Repair(1); // RA2: as above
				if (!House->Is_Human_Player()) { // RA2: as above
					House->RepairTimer = Random_Pick((int)(House->RepairDelay * 225.0), (int)(House->RepairDelay * 1800.0)); // RA2: as above
				}
			}
		} else {
			if ((Session.Type != GAME_NORMAL || IsAllowedToSell) && IsTickedOff && (unsigned)House->Control.IQ >= (unsigned)Rule->IQSellBack && (unsigned)Random_Pick(0, 50) < (unsigned)House->Control.TechLevel && Tag == NULL && Class->ToBuild != RTTI_BUILDINGTYPE && HealthRatio < Rule->ConditionRed) { // RA2: as above
				Sell_Back(1); // RA2: as above
			}
		}
	}
	if (IsRepairing && Frame % (int)(Rule->RepairRate * 900.0) == 0) { // RA2: as above
		IsWrenchVisible = !IsWrenchVisible; // RA2: as above
		int cost = Class->Repair_Cost();
		int step = Class->Repair_Step();
		int old_frame = Shape_Number(); // RA2: as above

		/*
		**	Check for and expend any necessary monies to continue the repair.
		*/
		if (House->Available_Money() >= cost) {
			House->Spend_Money(cost);
			Strength += step;
			EstimatedHealth += step; // RA2: as above
			if (Strength >= Class->MaxStrength) {
				EstimatedHealth = Strength = Class->MaxStrength; // RA2: as above
				IsRepairing = false;
			}
			Set_Anim_Damage_State(HealthRatio <= Rule->ConditionYellow);
			if (HealthRatio > Rule->ConditionYellow && ParticleSystems[ATTACHED_PARTICLE_DAMAGE]) ParticleSystems[ATTACHED_PARTICLE_DAMAGE]->Delete_Me(); // RA2: as above
			if (Shape_Number() != old_frame) IsToDisplay = true; // RA2: as above
		} else {
			IsRepairing = false;
		}
	}
}



// RA2: the ore level of a silo, in quarters of the capacity rounded to nearest, clamped to 0..3 (an inline helper of 0x44BD40)
static inline int Silo_Storage_Frame(BuildingClass * building) // RA2: as above
{
	int frame = 0; // RA2: as above
	if (building->Class->Capacity > 0) { // RA2: as above
		int stored = (int)building->Storage.Get_Total_Amount(); // RA2: as above
		stored = (int)((unsigned int)stored << 2); // RA2: as above
		frame = (int)((double)stored / building->Class->Capacity + 0.5); // RA2: as above
		if (frame < 0) return(0); // RA2: as above
		if (frame > 3) return(3); // RA2: as above
	}
	return(frame); // RA2: as above
}

// RA2: the ore level of a refinery, in quarters of the capacity truncated (an inline helper of 0x44BD40)
static inline int Refinery_Storage_Stage(BuildingClass * building) // RA2: as above
{
	int stored = (int)building->Storage.Get_Total_Amount(); // RA2: as above
	if (stored != 0) { // RA2: as above
		stored = (int)building->Storage.Get_Total_Amount(); // RA2: as above
		stored = (int)((unsigned int)stored << 2) / building->Class->Capacity; // RA2: as above
	}
	return(stored); // RA2: as above
}

/***********************************************************************************************
 * BuildingClass::Animation_AI -- Handles normal building animation processing.                *
 *                                                                                             *
 *    This will process the general building animation mechanism. It detects when the          *
 *    building animation sequence has completed and flags the building to perform mission      *
 *    changes as a result.                                                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Call this routine only once per building per game logic loop.                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void BuildingClass::Animation_AI(void)
{
	bool stagechange = Graphic_Logic();
	bool toloop = false;

	Update_Anim_Appearance();

	if (Class->IsCanUnitRepair && Mission != MISSION_REPAIR && (Anims[BANIM_PRODUCTION] != NULL || Anims[BANIM_SPECIAL_TWO] != NULL) && !Class->Naval) { // RA2: not for naval yards
		Begin_Anim(BANIM_SPECIAL_THREE, !(HealthRatio > Rule->ConditionYellow)); // RA2: the damaged state follows the health
		End_Anim(BANIM_PRODUCTION);
		End_Anim(BANIM_SPECIAL_TWO);
	}

	if (Class->IsSiloDamage) {
		int stage = Silo_Storage_Frame(this); // RA2: the frame of an inline helper
		AnimClass * anim = Anims[BANIM_SPECIAL_ONE];
		if (stage != 0) {
			if (anim == NULL) {
				Begin_Anim(BANIM_SPECIAL_ONE, !(HealthRatio > Rule->ConditionYellow)); // RA2: as above
			}
			Anims[BANIM_SPECIAL_ONE]->Set_Stage(stage);
		} else {
			if (anim != NULL) {
				End_Anim(BANIM_SPECIAL_ONE);
			}
		}

	}

	if (Class->IsRefinery) { // RA2: the ore level of a refinery shows in the four active animations
		int stage = Refinery_Storage_Stage(this); // RA2: as above
		if (StorageFilledSlots != stage) { // RA2: as above
			int previous = StorageFilledSlots; // RA2: as above
			if (previous >= 3) { // RA2: as above
				End_Anim(BANIM_ACTIVE_FOUR); // RA2: as above
			} else if (previous >= 2) { // RA2: as above
				End_Anim(BANIM_ACTIVE_THREE); // RA2: as above
			} else if (previous >= 1) { // RA2: as above
				End_Anim(BANIM_ACTIVE_TWO); // RA2: as above
			} else if (previous >= 0) { // RA2: as above
				End_Anim(BANIM_ACTIVE_ONE); // RA2: as above
			}
			StorageFilledSlots = Refinery_Storage_Stage(this); // RA2: recomputed
			if (StorageFilledSlots >= 3) { // RA2: as above
				Begin_Anim(BANIM_ACTIVE_FOUR, !(HealthRatio > Rule->ConditionYellow)); // RA2: as above
			} else if (StorageFilledSlots >= 2) { // RA2: as above
				Begin_Anim(BANIM_ACTIVE_THREE, !(HealthRatio > Rule->ConditionYellow)); // RA2: as above
			} else if (StorageFilledSlots >= 1) { // RA2: as above
				Begin_Anim(BANIM_ACTIVE_TWO, !(HealthRatio > Rule->ConditionYellow)); // RA2: as above
			} else if (StorageFilledSlots >= 0) { // RA2: as above
				Begin_Anim(BANIM_ACTIVE_ONE, !(HealthRatio > Rule->ConditionYellow)); // RA2: as above
			}
		}
	}

	if (Class->SuperWeapon != SUPER_NONE && Mission != MISSION_CONSTRUCTION && Mission != MISSION_DECONSTRUCTION && !(Class->ChargedAnimTime > 990.0f)) { // RA2: the super weapon animations
		for (int index = 0; index < House->SuperWeapon.Count(); ++index) { // RA2: as above
			SuperClass * super = House->SuperWeapon[index]; // RA2: as above
			SuperWeaponType super_type = super->Class->Type; // RA2: as above
			if (super_type == Class->SuperWeapon) { // RA2: as above
				if (!(Class->ChargedAnimTime < super->Control.Value() * 0.0011111111f)) { // RA2: as above
					if (Anims[BANIM_SUPER_ONE] != NULL) { // RA2: as above
						End_Anim(BANIM_SUPER_ONE); // RA2: as above
						Begin_Anim(BANIM_SUPER_TWO, !(HealthRatio > Rule->ConditionYellow)); // RA2: as above
					}
				} else if (Anims[BANIM_SUPER_THREE] != NULL) { // RA2: as above
					End_Anim(BANIM_SUPER_THREE); // RA2: as above
					Begin_Anim(BANIM_SUPER_FOUR, !(HealthRatio > Rule->ConditionYellow)); // RA2: as above
				}
			}
		}
	}

	if (!Is_Turret_Equipped() || Mission == MISSION_CONSTRUCTION || Mission == MISSION_DECONSTRUCTION) { // RA2: no charge animation
		if (stagechange) {

			/*
			**	Check for animation end or if special case of MCV deconstructing when it is allowed
			**	to convert back into an MCV.
			*/
			BuildingTypeClass::AnimControlType const * ctrl = Fetch_Anim_Control();

			/*
			**	When the last frame of the current animation sequence is reached, flag that
			**	a new mission may be started. This must occur before the animation actually
			**	loops so that if a mission change does occur, it will have a chance to change
			**	the building graphic before the last frame is replaced by the first frame of
			**	the loop.
			*/
			if (Fetch_Stage() == ctrl->Start+ctrl->Count-1  || (ArchiveTarget == NULL && Class->UndeploysInto != NULL && Mission == MISSION_DECONSTRUCTION && Fetch_Stage() == (42-19))) {
				IsReadyToCommence = true;
			}

			/*
			**	If the animation advances beyond the last frame, then start the animation
			**	sequence over from the beginning.
			*/
			if (Fetch_Stage() >= ctrl->Start+ctrl->Count) {
				toloop = true;
			}
			IsToDisplay = true;
		} else {
			if (BState == BSTATE_NONE || Fetch_Rate() == 0) {
				IsReadyToCommence = true;
			}
		}
	}

	if (Unknown_Virtual_32C()) { // RA2: an invulnerable building redraws every frame
		IsToDisplay = true; // RA2: as above
	}

	if (Is_Turret_Equipped() && Is_Turret_Equipped() && Anims[BANIM_TURRET] != NULL) { // RA2: the turret animation follows the facing (Set_Turret_Frame inline)
		int index = PrimaryFacing.Current().As_Dir32(); // RA2: Set_Turret_Frame inline
		AnimClass * anim = Anims[BANIM_TURRET]; // RA2: as above
		if (anim != NULL) { // RA2: as above
			anim->Set_Stage(BASECLASS::BodyShape[index]); // RA2: as above
			anim->Set_Step(0); // RA2: as above
		}
	}

	/*
	**	The animation sequence has looped. Restart it and flag this loop condition.
	**	This is used to tell the mission system that the animation has completed. It
	**	also signals that now is a good time to act on any pending mission.
	*/
	if (toloop) {
		BuildingTypeClass::AnimControlType const * ctrl = Fetch_Anim_Control();
		if (BState == BSTATE_CONSTRUCTION || BState == BSTATE_IDLE) {
			Set_Rate(Options.Normalize_Delay(ctrl->Rate));
		} else {
			Set_Rate(ctrl->Rate);
		}
		Set_Stage(ctrl->Start);
		Mark(MARK_CHANGE);
	}
}




/***********************************************************************************************
 * BuildingClass::How_Many_Survivors -- This determine the maximum number of survivors.        *
 *                                                                                             *
 *    This routine is called to determine how many survivors should run from this building     *
 *    when it is either sold or destroyed. Buildings that are captured have fewer survivors.   *
 *    The number of survivors is a portion of the cost of the building divided by the cost     *
 *    of a minigunner.                                                                         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of soldiers that should run from this building.            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/04/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int BuildingClass::How_Many_Survivors(void) const
{
	if (IsSurvivorless || !Class->IsCrew) return(0);

	int divisor = (ActLike == 1) ? Rule->SovietSurvivorDivisor : Rule->AlliedSurvivorDivisor; // RA2: by side
	if (divisor == 0) return(0);
	if (IsCaptured) divisor *= 2;
	int count = Class->GetRefund(House, false) / divisor; // RA2: the refund, no survivor fraction
	return(Bound(count, 1, 5));
}


/***********************************************************************************************
 * BuildingClass::Get_Image_Data -- Fetch the image pointer for the building.                  *
 *                                                                                             *
 *    This routine will return with a pointer to the shape data for the building. The shape    *
 *    data is different than normal when the building is undergoing construction and           *
 *    disassembly.                                                                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the shape data for this building.                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void const * BuildingClass::Get_Image_Data(void) const
{
	if (BState != BSTATE_CONSTRUCTION && HasOpened) { // RA2: the image only once the building has opened
		return(Class->Get_Image_Data()); // RA2: as above
	}
	return(Class->Get_Buildup_Data()); // RA2: as above
}



/// <summary>
/// Adds an upgrade to this building.
/// This routine plugs another upgrade into the building. The building is brought back
/// to full strength as part of the process and its animations are restarted so that
/// the new hardware appears. A wall tower cycles its turret instead.
/// </summary>
/// <returns>bool; Was the upgrade accepted?</returns>
bool BuildingClass::Add_Upgrade(void)
{
	if (Strength != Class->MaxStrength) {
		Strength = Class->MaxStrength;
		Set_Anim_Damage_State(false);
		if (HealthRatio > Rule->ConditionYellow) {
			if (ParticleSystems[ATTACHED_PARTICLE_DAMAGE]) {
				ParticleSystems[ATTACHED_PARTICLE_DAMAGE]->Delete_Me();
			}
		}
		for (BAnimType i = BANIM_ACTIVE_ONE; i <= BANIM_ACTIVE_FOUR; i++) {
			if (Anims[i] == NULL) {
				Begin_Anim(i, false);
			}
		}
		IsToDisplay = true;
	}

	if (Class == Rule->WallTower) {
		int index = TurretIndex + 1;
		if (index > 2) index = 0;
		Set_Turret_Index(index);
		UpgradeLevel++;
		return(true);
	}

	if (UpgradeLevel == Class->Upgrades) {
		return(false);
	}

	UpgradeLevel++;
	Begin_Anim((BAnimType)(UpgradeLevel - 1), HealthRatio <= Rule->ConditionYellow);

	return(true);
}


/// <summary>
/// Removes the most recent upgrade from this building.
/// This routine is used when an upgrade is sold or destroyed. Its animation is
/// stopped, and any super weapon that came with it is taken away from the house.
/// </summary>
/// <returns>bool; Was an upgrade actually removed?</returns>
bool BuildingClass::Remove_Upgrade(void)
{
	if (UpgradeLevel == 0) {
		return(false);
	}

	bool super = false;
	if (Upgrades[UpgradeLevel-1] != NULL && Upgrades[UpgradeLevel-1]->SuperWeapon != SUPER_NONE) {
		super = true;
	}

	if (Upgrades[UpgradeLevel-1] != NULL && Upgrades[UpgradeLevel-1]->IsTurretEquipped) {
		End_Anim(BANIM_TURRET);
		Upgrades[UpgradeLevel-1] = NULL;
		UpgradeLevel = 0;
		TurretIndex = -1;
		if (super) House->Update_Present_Super_Weapons();
		return(true);
	}

	End_Anim((BAnimType)(UpgradeLevel-1));
	Upgrades[--UpgradeLevel] = NULL;
	if (super) House->Update_Present_Super_Weapons();
	return(true);

}


/// <summary>
/// Starts one of this building's attached animations.
/// This routine is used when the building enters a state that its type provides an
/// animation for. Slots the type leaves blank are quietly ignored, so a caller may
/// begin an animation without first checking that the building has one.
/// </summary>
/// <param name="anim">The animation slot to start.</param>
/// <param name="damaged">Should the damaged form of the animation be used?</param>
/// <param name="delay">The delay in game frames before the animation begins.</param>
void BuildingClass::Begin_Anim(BAnimType anim, bool damaged, bool garrisoned, int delay) // RA2: the garrisoned animation too
{
	char const * name; // RA2: as above
	if (damaged) {
		name = Class->AnimData[anim].AnimDamaged; // RA2: as above
	} else if (garrisoned) { // RA2: as above
		name = Class->AnimData[anim].AnimGarrisoned; // RA2: as above
	} else {
		name = Class->AnimData[anim].Anim;
	}
	if (name != NULL && name[0] != '\0') {
		Create_Anim(name, anim, damaged, garrisoned, delay); // RA2: as above
	}
}






/// <summary>
/// Moves this building's animations to follow the building.
/// This routine is used after the building's render position changes so that its
/// attached animations stay pinned to the offsets the type specifies for them.
/// </summary>
void BuildingClass::Set_Anim_Coords(void)
{
	Coord render = Render_Coord();
	for (int i = 0; i < 18; i++) { // RA2: eighteen animations
		ObjectClass * anim = Anims[i]; // RA2: an object
		if (anim != NULL) {
			anim->Set_Coord(TacticalMap->Pixel_To_Coord_Absolute(Class->AnimData[i].Location) + render); // RA2: the offset first, no named temporary
		}
	}
}



/// <summary>
/// RA2
/// </summary>
static inline void Warp_Pause(BuildingClass * building, AnimClass * anim)
{
	if (building->IsBeingWarpedOut()) {
		anim->Paused = true;
	}
}


/// <summary>
/// RA2
/// </summary>
static inline void Power_Disable(BuildingClass * building, AnimClass * anim, int index)
{
	if (!(building->IsPoweredOn && building->IsPowerOnline()) && building->Class->IsPowered && building->Class->AnimData[index].Powered) {
		anim->Disable();
	}
}


/// <summary>
/// Creates one of this building's attached animations.
/// This is the low level routine that Begin_Anim uses. The animation is placed at the
/// offset the type specifies, inherits the building's fog and translucency, and
/// replaces whatever was running in the slot -- keeping its stage so that the swap
/// does not show.
/// </summary>
/// <param name="name">The name of the animation type to create.</param>
/// <param name="anim">The animation slot to fill.</param>
/// <param name="damaged">Should the damaged form of the animation be used?</param>
/// <param name="delay">The delay in game frames before the animation begins.</param>
void BuildingClass::Create_Anim(char const * name, BAnimType anim, bool damaged, bool, int delay) // RA2: garrisoned
{
	Set_Anim_Damage_State(damaged);

	AnimType animtype = AnimTypeClass::From_Name(name);
	if (animtype != ANIM_NONE) {
		Coord coord (Render_Coord() + TacticalMap->Pixel_To_Coord_Absolute(Class->AnimData[anim].Location));
		AnimClass * animptr = new AnimClass(AnimTypes[animtype], coord, delay, 1, ShapeFlags_Type(SHAPE_CENTER|SHAPE_WIN_REL|SHAPE_ZREAD));

		animptr->ZAdjust = Class->AnimData[anim].ZAdjust;
		animptr->YSortAdjust = Class->AnimData[anim].YSort;
		animptr->IsBuildingAnim = true;

		if (IsFogged) {
			animptr->IsFogged = true;
		}

		Set_Anim_Translucency(TranslucencyLevel);

		if (Anims[anim] != NULL) {
			animptr->Set_Stage(Anims[anim]->Fetch_Stage());
			AnimClass * oldanim = Anims[anim];
			Anims[anim] = NULL;
			delete oldanim;
		}

		Anims[anim] = animptr;

		if (!IsPoweredOn && Class->AnimData[anim].Powered) { // RA2: an animation that runs with power starts disabled while the building is off
			animptr->Disable(); // RA2: as above
		}

		if (animptr->Class->IsShouldUseCellDrawer) {
			animptr->AlternativeDrawer = (ConvertClass *)GetRemapColour(); // RA2: the object's remap, not the house's color scheme
			int height = Get_Cell_Ptr()->Brightness; // RA2: the cell's brightness
			int brightness = Apparent_Brightness(height); // RA2: as above
			if (Unknown_Virtual_32C()) { // RA2: with the effect tint (iron curtain)
				brightness = GetEffectTintIntensity(brightness); // RA2: as above
			}
			animptr->AlternativeBrightness = brightness; // RA2: as above
		}

		if (anim == BANIM_TURRET && Class->IsBarrelAnimAVoxel) {
			animptr->Make_Invisible();
		}

		if (IsBeingWarpedOut()) { // RA2: a building that is being warped out freezes its animations
			animptr->Paused = true; // RA2: as above
		}
	}
}






/// <summary>
/// Handles an attached animation being removed from the game.
/// This routine is called as one of the building's animations expires so that the
/// building forgets its pointer. A repair bay takes the opportunity to start the next
/// stage of its repair animation.
/// </summary>
/// <param name="anim">The animation that is going away.</param>
void BuildingClass::Detach_Anim(AnimClass * anim)
{
	if (!IsActive) { // RA2: RA2's form
		return; // RA2: as above
	}
	for (int index = 0; index < 18; ++index) { // RA2: as above
		if (Anims[index] != anim) { // RA2: as above
			continue; // RA2: as above
		}
		Anims[index] = NULL; // RA2: as above
		AnimClass * animation = static_cast<AnimClass *>(anim); // RA2: as above
		switch (index) { // RA2: as above
		case 10: // RA2: as above
			if (Class->IsCanUnitRepair) { // RA2: as above
				if (HasAnyLink() && Get_Mission() == MISSION_REPAIR) { // RA2: as above
					Begin_Anim(BANIM_SPECIAL_TWO, HealthRatio <= Rule->ConditionYellow, false, 0); // RA2: as above
					return; // RA2: as above
				}
				Begin_Anim(BANIM_IDLE, HealthRatio <= Rule->ConditionYellow, false, 0); // RA2: as above
			} else if (Class->IsAnimDelayedFire && animation->TimeToDie) { // RA2: as above
				Begin_Anim(BANIM_ACTIVE_ONE, HealthRatio <= Rule->ConditionYellow, Occupants.Count() > 0, 0); // RA2: as above
			}
			break; // RA2: as above
		case 12: // RA2: as above
			if (Class->IsCanUnitRepair && animation->TimeToDie) { // RA2: as above
				Begin_Anim(BANIM_IDLE, HealthRatio <= Rule->ConditionYellow, false, 0); // RA2: as above
			}
			break; // RA2: as above
		case 14: // RA2: as above
			if (animation->TimeToDie) { // RA2: as above
				Begin_Anim(BANIM_SUPER_THREE, HealthRatio <= Rule->ConditionYellow, false, 0); // RA2: as above
			}
			break; // RA2: as above
		case 16: // RA2: as above
			if (animation->TimeToDie) { // RA2: as above
				Begin_Anim(BANIM_SUPER_ONE, HealthRatio <= Rule->ConditionYellow, false, 0); // RA2: as above
			}
			break; // RA2: as above
		}
		return; // RA2: as above
	}
}











/// <summary>
/// Stops one of this building's attached animations.
/// Pass BANIM_ALL to stop every animation the building is running.
/// </summary>
/// <param name="anim">The animation slot to stop, or BANIM_ALL for all of them.</param>
void BuildingClass::End_Anim(BAnimType anim)
{
	if (anim == -2) { // RA2: RA2's form
		for (int i = 0; i < 18; i++) { // RA2: as above
			AbstractClass *object = Anims[i]; // RA2: as above
			if (object) { // RA2: as above
				Anims[i] = 0; // RA2: as above
				delete object; // RA2: as above
			}
		}
	} else {
		AbstractClass *object = Anims[anim]; // RA2: as above
		if (object) { // RA2: as above
			Anims[anim] = 0; // RA2: as above
			delete object; // RA2: as above
		}
	}
}



/// <summary>
/// Hides the building animation of the given type (or all of them) without removing it.
/// </summary>
/// <param name="anim">The animation to hide, or BANIM_ALL.</param>
void BuildingClass::Hide_Anim(BAnimType anim) // RA2: not in TibSun, at 0x44D030
{
	if (anim == BANIM_ALL) {
		for (int i = 0; i < 18; i++) { // RA2: all 18 animation slots
			AnimClass * animation = Anims[i];
			if (animation != NULL) {
				animation->Make_Invisible();
			}
		}
	} else {
		AnimClass * animation = Anims[anim];
		if (animation != NULL) {
			animation->Make_Invisible();
		}
	}
}








/// <summary>
/// Switches this building's animations to their damaged form.
/// Each building animation may have a separate damaged version. This routine is used
/// as the building crosses a damage threshold so that every animation it is running is
/// restarted in the matching form.
/// </summary>
/// <param name="damaged">Should the damaged versions of the animations be used?</param>
void BuildingClass::Set_Anim_Damage_State(bool damaged)
{
	if (IsDamagedAnims != damaged) {
		IsDamagedAnims = damaged;
		for (int i = 0; i < 18; i++) { // RA2: all eighteen anims (see Anims)
			if (Anims[i] != NULL) {
				Begin_Anim((BAnimType)i, damaged);
			}
		}
	}
}


/// <summary>
/// Sets the drawer that this building's animations render with.
/// Only the animations that ask for the cell drawer are affected -- the rest keep
/// their own appearance.
/// </summary>
/// <param name="drawer">The color converter for the animations to draw with.</param>
/// <param name="brightness">The brightness level to draw the animations at.</param>
void BuildingClass::Set_Anim_Drawer(ConvertClass * drawer, int brightness)
{
	for (int index = 0; index < 18; index++) { // RA2: RA2's form
		AnimClass *anim = static_cast<AnimClass *>(Anims[index]); // RA2: as above
		if (anim != 0) { // RA2: as above
			if (anim->Class != 0 && anim->Class->IsShouldUseCellDrawer) { // RA2: as above
				anim->AlternativeDrawer = drawer;
				anim->AlternativeBrightness = brightness;
			}
			Set_Anim_Translucency(TranslucencyLevel);
		}
	}
}



/// <summary>
/// Brings this building's animations back in line with its appearance.
/// Use this routine after the lighting on the building changes, so that the
/// animations attached to it are drawn with the same brightness and translucency as
/// the building itself.
/// </summary>
void BuildingClass::Update_Anim_Appearance(void)
{
	ConvertClass * converter = (ConvertClass *)GetRemapColour(); // RA2: the remap of the object, not the house's color scheme
	Brightness = Tinted_Brightness(Get_Cell_Ptr()->Brightness); // RA2: the effect tint (iron curtain) is applied to the cell's brightness
	Set_Anim_Drawer(converter, Brightness);
	Set_Anim_Translucency(TranslucencyLevel);
}




/// <summary>
/// Sets the translucency of this building's animations.
/// This routine keeps the attached animations as transparent as the building they
/// belong to. A building that has faded from sight pushes its animations one step
/// further so that they disappear along with it.
/// </summary>
/// <param name="translucency">The translucency level to apply.</param>
void BuildingClass::Set_Anim_Translucency(int translucency)
{
	int trans = translucency;
	if (trans == 15 && Visual_Character() == VISUAL_HIDDEN) {
		trans = 16;
	}

	for (int i = 0; i < 18; i++) { // RA2: all eighteen anims (see Anims)
		if (Anims[i] != NULL) {
			Anims[i]->TranslucencyLevel = trans;
		}
	}
}

/// <summary>
/// Freezes the building's powered animations while a temporal weapon holds the building.
/// </summary>
void BuildingClass::DisableTemporal(void) // RA2: not in TibSun, at 0x44D320
{
	IsOn = false;
	for (int i = 0; i < 18; i++) {
		AnimClass * anim;
		if (Class->AnimData[i].Powered && (anim = Anims[i]) != NULL) {
			anim->Paused = true;
			anim->Unpaused = false;
			anim->PausedAnimFrame = anim->Fetch_Stage();
			anim->UnderTemporal = true;
		}
	}
}


/// <summary>
/// Lets the building's powered animations run again once the temporal weapon lets go.
/// </summary>
void BuildingClass::EnableTemporal(void) // RA2: not in TibSun, at 0x44D380
{
	IsOn = true;
	for (int i = 0; i < 18; i++) {
		AnimClass * anim;
		if (Class->AnimData[i].Powered && (anim = Anims[i]) != NULL) {
			anim->Paused = false;
			anim->Unpaused = true;
			anim->UnderTemporal = false;
		}
	}
}


/// <summary>
/// Answers an unidentified virtual query about the building.
/// This routine has never been given a purpose -- it simply answers zero.
/// </summary>
unsigned int BuildingClass::entry_380(void)
{
	/// Nothing in the game calls this routine.
	return(0);
}











/// <summary>
/// Turns this building on.
/// This routine brings the building back into service, powers it up, and tells the
/// house to recalculate its power and radar coverage. Production that depends on this
/// building is re-examined and the player is told the good news. A building that is
/// still stunned refuses to come back on.
/// </summary>
void BuildingClass::Turn_On(void)
{
	if (!IsOn && StunDuration == 0) {
		IsOn = true;
		IsToDisplay = true;
		House->RecalcPower = true;
		House->RecalcRadar = true;
		House->IsRecalcNeeded = true;
		Power_On();

		if (Class->ToBuild != RTTI_NONE) {
			House->Update_Factories(Class->ToBuild, Class->Naval, 0); // RA2: with the naval flag and category 0
		}

		if (House->Is_Player_Control()) {
			Queue_Speech("EVA_BuildingOnLine", -1, -1); // RA2: EVA speaks by name through Queue_Speech
		}
	}
}


/// <summary>
/// Turns this building off.
/// This routine takes a powered building out of service and tells the house to
/// recalculate its power and radar coverage. Production that depends on this building
/// is re-examined and the player is told the bad news.
/// </summary>
void BuildingClass::Turn_Off(void)
{
	if (IsOn && (Class->Drain > 0 || Class->IsPowered)) {
		IsOn = 0;
		IsToDisplay = 1;
		House->RecalcPower = 1;
		House->RecalcRadar = 1;
		House->IsRecalcNeeded = 1;
		Power_Off();

		if (House->Is_Player_Control()) {
			Queue_Speech("EVA_BuildingOffLine", -1, -1); // RA2: EVA speaks by name through Queue_Speech
		}

		if (Class->ToBuild != RTTI_NONE) {
			House->Update_Factories(Class->ToBuild, Class->Naval, 0); // RA2: with the naval flag and category 0
		}

		if (Class == Rule->GDIFirestormGenerator) {
			House->Lost_Firestorm_Generator();
		}
	}
}


/// <summary>
/// Puts this building back onto the power grid.
/// This routine restores the building's contribution to the house power tally and
/// wakes up everything that only runs while the building is powered -- its light
/// source, its laser fence, and its powered animations.
/// </summary>
void BuildingClass::Power_On(void)
{
	IsPoweredOn = true;
	Adjust_House_Power(House);

	if (LightSource) {
		LightSource->Enable();
	}

	if (Class->IsLaserFencePost) {
		Toggle_Laser_Fence_Post(false);
	}

	for (int i = 0; i < BANIM_COUNT; i++) {
		if (Class->AnimData[i].Powered && Anims[i] != NULL) {
			Anims[i]->Enable();
		}
	}
}


/// <summary>
/// Takes this building off of the power grid.
/// This routine withdraws the building's contribution to the house power tally and
/// shuts down everything that depends upon power -- its light source, its cloaking
/// field, its laser fence, and its powered animations.
/// </summary>
void BuildingClass::Power_Off(void)
{
	HouseClass *house = House; // RA2: RA2's form
	IsPoweredOn = false;
	int count = house->Buildings.Count(); // RA2: as above
	for (int index = 0; index < count; index++) { // RA2: as above
		BuildingClass *building = house->Buildings[index]; // RA2: as above
		if (building->IsActive && !building->IsInLimbo) { // RA2: as above
			if (building->Class->IsLaserFencePost) { // RA2: as above
				building->Toggle_Laser_Fence_Post(0); // RA2: as above
			}
			building->Update_Power_State(); // RA2: as above
		}
	}
	if (LightSource) {
		LightSource->Disable();
	}
	if (Class->IsCloakGenerator) { // RA2: as above
		CloakGeneratorState = -1; // RA2: as above
		if (!CurrentCloakRadius) { // RA2: as above
			CurrentCloakRadius = Class->CloakRadiusInCells; // RA2: as above
		}
		IsToDisplay = true; // RA2: as above
	}
	if (Class->GapGenerator && GeneratingGap) { // RA2: as above
		DestroyGap(); // RA2: as above
	}
	if (Class->IsLaserFencePost) {
		Toggle_Laser_Fence_Post(0); // RA2: as above
	}
	for (int anim = 0; anim < 18; anim++) { // RA2: as above
		if (Class->AnimData[anim].Powered && Anims[anim]) { // RA2: as above
			Anims[anim]->Disable(); // RA2: as above
		}
	}
}









/// <summary>
/// Requests that this gate be opened.
/// This routine is called by a unit that wishes to pass through. A building that is
/// not a gate agrees at once, but a real gate is put onto its opening mission and the
/// caller is told to wait until the gate reports itself open.
/// </summary>
/// <returns>bool; Is the gate open and ready to be passed through?</returns>
bool BuildingClass::Open_Gate(void)
{
	if (!Class->IsGate) {
		return(true);
	}
	if (Get_Mission() == MISSION_OPEN && !Door.Is_Door_Closing() && !Door.Is_Door_Closed()) { // RA2: Open_Gate at 0x44D7D0 matches only so
		return(Is_Gate_Open());
	}
	Set_Mission(MISSION_NONE);
	QueueMission(MISSION_OPEN, false); // RA2: as above
	Commence();
	return(false);
}


/// <summary>
/// Is this gate standing open?
/// Anything that is not a gate is considered permanently open, so that the movement
/// code may ask this question of any building it meets.
/// </summary>
bool BuildingClass::Is_Gate_Open(void) const
{
	if (!Class->IsGate) {
		return(true);
	}
	if (Get_Mission() == MISSION_OPEN && Door.Is_Door_Open()) { // RA2: Is_Gate_Open at 0x44D880 matches only so
		return(true);
	}
	return(false);
}


/// <summary>
/// Can this building be repaired?
/// A destroyed building cannot be repaired, and neither can one that is really a
/// vehicle wearing a building's clothes. Everything else falls back on the normal
/// damage test.
/// </summary>
bool BuildingClass::Can_Repair(void) const
{
	if (Strength == 0) {
		return(false);
	}

	if (!Class->ClickRepairable) { // RA2: a type flag says whether a click can repair it
		return(false);
	}

	if (Considered_Vehicle()) {
		return(false);
	}

	return(BASECLASS::Can_Repair());
}



/// <summary>
/// Can the specified upgrade be plugged into this building?
/// This routine is used when deciding whether an upgrade may be placed upon this
/// building. The upgrade must name this building as its host, the house asking must
/// own it, and there must still be room for another upgrade.
/// </summary>
/// <param name="upgrade">The upgrade building type that wishes to be installed.</param>
/// <param name="upgrader">The house that wishes to install the upgrade.</param>
bool BuildingClass::Can_Upgrade(BuildingTypeClass const * upgrade, HouseClass const * upgrader) const
{
	if (upgrader != House || stricmp(upgrade->PowersUpBuilding, Class->IniName)) {
		goto failed;
	}

	if (upgrade->PowersUpToLevel != -1) {
		if (upgrade->PowersUpToLevel <= 0 || upgrade->PowersUpToLevel > BUILDING_UPGRADE_MAX) {
			goto failed;
		}
	} else {
		if (UpgradeLevel < Class->Upgrades) {
			return(true);
		}
	}

	if (UpgradeLevel == 0) {
		return(true);
	}

failed:
	return(false);
}


/// <summary>
/// Fetches the weapon data for the weapon slot specified.
/// An upgrade plugged into the building brings its own weapon along, and that weapon
/// takes precedence over the building's own. This routine is how the combat code
/// discovers what the building really shoots with.
/// </summary>
/// <param name="which">The weapon slot to fetch the data for.</param>
/// <returns>Returns with a pointer to the weapon data that applies to this building.</returns>
WeaponDataStruct const * BuildingClass::Get_Class_Weapon_Data(int which) const
{
	if (UpgradeLevel != 0) {
		for (int i = 0; i < UpgradeLevel; i++) {
			if (Upgrades[i] != NULL) {
				WeaponDataStruct const * weapon = Upgrades[i]->Get_Weapon(which);
				if (weapon->Weapon != NULL) {
					return(weapon);
				}
			}
		}
	}
	return(BASECLASS::Get_Class_Weapon_Data(which));
}


/// <summary>
/// Does this building have a turret?
/// An upgrade can bring a turret with it, so this routine considers both the
/// building's own type and everything that has been plugged into it.
/// </summary>
bool BuildingClass::Is_Turret_Equipped(void) const
{
	if (Class->IsTurretEquipped) {
		return(true);
	}
	if (UpgradeLevel != 0) {
		for (int i = 0; i < UpgradeLevel; i++) {
			if (Upgrades[i] != NULL && Upgrades[i]->IsTurretEquipped) {
				return(true);
			}
		}
	}
	return(false);
}


/// <summary>
/// Moves the infantry to the end of the list of those overpowering this building.
/// </summary>
void BuildingClass::Rotate_Overpowerer(InfantryClass * infantry) // RA2: not in TibSun, at 0x44DA20
{
	Overpowerers.Delete(infantry);
	Overpowerers.Add(infantry);
}


/// <summary>
/// Is the target within this building's psychic detection radius?
/// </summary>
bool BuildingClass::Is_In_Psychic_Range(AbstractClass const * target) // RA2: not in TibSun, at 0x44DAA0
{
	Coord a = Center_Coord();
	Coord b = target->Center_Coord();
	return(a.Distance_To(b) <= Class->PsychicDetectionRadius << 8);
}


/// <summary>
/// Is the cell within this building's psychic detection radius?
/// </summary>
bool BuildingClass::Is_Cell_In_Psychic_Range(CellClass const * cell) // RA2: not in TibSun, at 0x44DB40
{
	Coord a = Center_Coord();
	Coord b = cell->Cell_Coord();
	return(a.Distance_To(b) <= Class->PsychicDetectionRadius << 8);
}











/// <summary>
/// Links this laser fence post up with its neighbors.
/// This routine runs a fence out in each of the four cardinal directions and then
/// switches the post on.
/// </summary>
void BuildingClass::Init_Laser_Fence(void)
{
	if (!ScenarioInit && Class->IsLaserFencePost) {
		if (IsActive && !IsInLimbo) {
			Connect_Laser_Fence(FACING_N);
			Connect_Laser_Fence(FACING_E);
			Connect_Laser_Fence(FACING_S);
			Connect_Laser_Fence(FACING_W);
			Toggle_Laser_Fence_Post(true);
		}
	}
}


/// <summary>
/// Notes which sides of this fence post carry fence, and clears those runs away.
/// Use this routine when the post is about to change state. The sides it records are
/// what allows the fence to be laid down again afterwards.
/// </summary>
/// <param name="explode">Should the fence segments be blown up rather than quietly
/// removed?</param>
void BuildingClass::Update_Laser_Fence_Connections(int explode)
{
	if (!ScenarioInit && Class->IsLaserFencePost) {
		bool do_explode = false;
		if (explode == (int)true) {
			do_explode = true;
		}
		FacingType dir = FACING_N;
		Cell origin = Center_Coord().As_Cell();

		static int _facing_bits[] = { 1 << 0, 1 << 1, 1 << 2, 1 << 3 };

		for (int i = 0; i < ARRAY_SIZE(_facing_bits); i++) {
			Cell adjacent = Adjacent_Cell(origin, dir);
			BuildingClass * building = Map[adjacent].Cell_Building();
			if (building != NULL && building->Class->IsLaserFence) {
				if (Owner() == building->Owner()) {
					if ((building->PrimaryFacing.Current().As_Axis()) == (dir & 3)) {
						LaserFenceFrame |= _facing_bits[i];
						Disconnect_Laser_Fence(dir, do_explode);
					}
				}
			}
			dir = Facing_Add(dir, FACING_90);
		}
	}
}


/// <summary>
/// Finds the friendly laser fence post lying in the direction specified.
/// This routine is used to discover which post this one should be linked with. The
/// search can be required to follow unbroken fence, so that a post beyond a gap is not
/// mistaken for a neighbor.
/// </summary>
/// <param name="dir">The direction to search in.</param>
/// <param name="connected">Must the search stop at the first break in the fence?</param>
/// <param name="xrange">How many cells to search; a negative value means this post's
/// own range.</param>
/// <returns>Returns with a pointer to the fence post found, or NULL if none.</returns>
BuildingClass * BuildingClass::Find_Laser_Fence_Post(FacingType dir, bool connected, int xrange)
{
	int range = 1;

	if (xrange < 0) {
		if (Class->IsLaserFencePost) {
			range = Class->ThreatRange >> 8;
			range = MAX(range, 1);
		}
	} else {
		range = xrange;
	}

	unsigned face = (unsigned)dir % 4;
	Cell working_cell = Center_Coord().As_Cell();
	BuildingClass * result = NULL;

	for (int i = 0; i < range; i++) {
		working_cell = Adjacent_Cell(working_cell, dir);
		BuildingClass * building = Map[working_cell].Cell_Building();

		if (building) {
			if (building->Class->IsLaserFencePost) {
				if (Owner() == building->Owner()) {
					result = building;
				}
			}

			if (!connected) break;
			if (!building->Class->IsLaserFence)	break;
			if (Owner() != building->Owner()) break;
			if ((building->PrimaryFacing.Current().As_Axis()) != (int)face) break;
		}
	}

	return(result);
}


/// <summary>
/// Initializes the laser fences for every building in play.
/// Use this routine to bring every laser fence post on the map into a connected state
/// in one sweep.
/// </summary>
void BuildingClass::Init_All_Laser_Fences(void)
{
	for (int i = 0; i < Buildings.Count(); i++) {
		BuildingClass * building = Buildings[i];
		if (building != NULL && building->Class->IsLaserFencePost) {
			building->Init_Laser_Fence();
		}
	}
}


/// <summary>
/// Builds a laser fence run toward the neighboring post.
/// This routine fills the gap between this fence post and the friendly post lying in
/// the direction specified. Fence segments are created cell by cell, and any tiberium
/// or veins caught underneath them is burned away. If a segment cannot be placed, the
/// whole run is torn down again.
/// </summary>
/// <param name="dir">The direction of the neighboring fence post to link with.</param>
void BuildingClass::Connect_Laser_Fence(FacingType dir)
{
	BuildingClass * building;

	if (!ScenarioInit && Class->IsLaserFencePost) {
		int range;
		if (Class->IsLaserFencePost) {
			range = Class->ThreatRange >> 8;
			range = MAX(range, 1);
		} else {
			range = 1;
		}

		BuildingClass * post = Find_Laser_Fence_Post(dir, false, -1);
		if (post != NULL && post->Mission != MISSION_DECONSTRUCTION) {
			int fenceid;
			for (fenceid = 0; fenceid < BuildingTypes.Count(); fenceid++) {
				if (BuildingTypes[fenceid]->IsLaserFence) {
					break;
				}
			}

			if (fenceid < BuildingTypes.Count()) {
				Cell working_cell = Center_Coord().As_Cell();
				bool linked = false; // RA2: Connect_Laser_Fence at 0x44DFC0 matches only so
				while (range > 0) {
					working_cell = Adjacent_Cell(Cell(working_cell), dir); // RA2: as above
					building = Map[working_cell].Cell_Building();
					if (building == post) { // RA2: as above
						linked = true; // RA2: as above
						break;
					}

					building = (BuildingClass *)BuildingTypes[fenceid]->Create_One_Of(Owner_HouseClass());
					if (!building->Unlimbo(Coord(working_cell), DirType((FacingType)(dir & 3)).As_Dir256())) {
						delete building;
						break;
					}
				}

				if (!linked) { // RA2: as above
					Disconnect_Laser_Fence(dir, false); // RA2: as above
					return; // RA2: as above
				}

				working_cell = Center_Coord(); // RA2: as above
				do {
					CellClass * cptr = &Map[working_cell];
					if (cptr->Tiberium_Type_Here() != TIBERIUM_NONE) {
						cptr->Reduce_Tiberium(12);
					}
					if (cptr->Overlay == OVERLAY_VEINS) {
						cptr->Reduce_Weed();
					}
					working_cell = Adjacent_Cell(working_cell, dir);
					building = Map[working_cell].Cell_Building();
				} while (building != post);
			}
		}
	}
}


/// <summary>
/// Removes the laser fence run lying in the direction specified.
/// This routine is used when a fence post loses one of its links. The segments
/// between this post and its neighbor are either quietly removed or destroyed in a
/// suitably spectacular fashion.
/// </summary>
/// <param name="dir">The direction of the fence run to remove.</param>
/// <param name="explode">Should the fence segments be destroyed rather than quietly
/// removed?</param>
void BuildingClass::Disconnect_Laser_Fence(FacingType dir, bool explode)
{
	if (Class->IsLaserFencePost) {
		int range = Class->ThreatRange >> 8;
		if (range <= 1) range = 1; // RA2: RA2's form
		unsigned axis = (unsigned)dir % 4; // RA2: as above
		Cell cell = Center_Coord().As_Cell(); // RA2: as above
		for (int index = 0; index < range; ++index) { // RA2: as above
			cell = Adjacent_Cell(cell, dir); // RA2: as above
			BuildingClass *segment = static_cast<BuildingClass *>(Map[cell].Cell_Building()); // RA2: as above
			if (segment) { // RA2: as above
				if (!segment->Class->IsLaserFence) return; // RA2: as above
				if (Owner() != segment->Owner()) return; // RA2: as above
				if ((unsigned)(segment->PrimaryFacing.Current().As_Axis() & 3) != axis) return; // RA2: as above
				if (explode) {
					Combat_Lighting(segment->Center_Coord(), segment->Strength, Rule->C4Warhead, 0, 0); // RA2: as above
					segment->Take_Damage(segment->Strength, 0, Rule->C4Warhead, 0, true, 1); // RA2: as above
				} else {
					segment->Delete_Me(); // RA2: as above
				}
			}
		}
	}
}



/// <summary>
/// Validates the laser fence segment occupying the cell specified.
/// This routine is used as something is placed onto the map beside a laser fence. The
/// run of fence leading away from the cell is followed, and unless it terminates in a
/// fence post the segment is removed rather than left dangling.
/// </summary>
/// <param name="cell">The cell holding the fence segment to check.</param>
void BuildingClass::Unlimbo_Laser_Fence_Helper(Cell const & cell)
{
	CellClass * cptr = &Map[cell];
	if (cptr != NULL) {
		BuildingClass * fence = cptr->Cell_Building();
		if (fence != NULL && fence->Class->IsLaserFence) {
			int facing = fence->PrimaryFacing.Current().As_Axis();
			BuildingClass * building = NULL;
			Cell old_cell = cell;

			Cell working_cell = Adjacent_Cell(old_cell, (FacingType)facing);
			while (working_cell.X != old_cell.X || working_cell.Y != old_cell.Y) { // RA2: Unlimbo_Laser_Fence_Helper at 0x44E440 matches only so
				building = Map[working_cell].Cell_Building();
				if (building == NULL) {
					break;
				}
				if (building->Class->IsLaserFencePost) {
					building->Disconnect_Laser_Fence(FacingType(facing | FACING_180), false);
					break;
				}
				if (!building->Class->IsLaserFence) {
					building = NULL;
					break;
				}
				old_cell = working_cell;
				working_cell = Adjacent_Cell(old_cell, (FacingType)facing);
			}
			if (building == NULL) {
				fence->Delete_Me();
			}
		}
	}
}


/// <summary>
/// Clears the laser fence connections recorded for this post.
/// Use this routine to make the post forget which of its sides carry fence, so that
/// the connections may be worked out afresh.
/// </summary>
void BuildingClass::Init_Laser_Fence_Frame(void)
{
	if (!ScenarioInit && Class->IsLaserFencePost) {
		LaserFenceFrame = 0;
	}
}


/// <summary>
/// RA2
/// </summary>
inline Cell Next_Cell(Cell const & cell, Cell const & adj) { return cell + adj; }


/// <summary>
/// Raises or lowers the laser fences strung from this post.
/// This routine is used whenever the post's power or mission state changes. A fence run
/// that reaches another live post energizes; the rest go slack. Units, aircraft and
/// infantry caught in a fence as it energizes are destroyed.
/// </summary>
/// <param name="force">Should the fence be updated even when it already looks correct?</param>
void BuildingClass::Toggle_Laser_Fence_Post(bool force)
{
	if (Class->IsLaserFencePost && IsActive && !IsInLimbo) {

		int range = Class->ThreatRange >> 8;
		range = MAX(range, 1);

		bool on = false;
		if (IsPowerOnline() && IsPoweredOn && Mission != MISSION_DECONSTRUCTION && Mission != MISSION_CONSTRUCTION) { // RA2: Toggle_Laser_Fence_Post at 0x44E5A0 matches only so
			on = true;
		}

		FacingType dir = FACING_N;
		for (int i = 0; i < FACING_COUNT / 2; i++, dir = Facing_Add(dir, FACING_90)) {

			Cell working_cell = Adjacent_Cell(Center_Coord().As_Cell(), dir);
			BuildingClass * building = Map[working_cell].Cell_Building();
			int face = dir & 3;

			if (building == NULL) continue;
			if (!building->Class->IsLaserFence) continue;
			if (Owner() != building->Owner()) continue;
			if ((building->PrimaryFacing.Current().As_Axis()) != face) continue;

			BuildingClass * first = building;
			BuildingClass * last = building;

			if (!force) {
				if (on) {
					if (building->LaserFenceFrame < 8) continue;
				} else {
					if (building->LaserFenceFrame >= 8) continue;
				}
			}

			BuildingClass * post = NULL;
			if (on) {
				post = Find_Laser_Fence_Post(dir, true, -1);
			}

			int first_frame = 12;
			int mid_frame = 12;
			int last_frame = 12;
			if (face == 2) {
				first_frame = 8;
				mid_frame = 8;
				last_frame = 8;
			}

			bool post_on = false;
			if (on && post != NULL && post->IsPowerOnline() && post->IsPoweredOn // RA2: as above
					&& post->Mission != MISSION_DECONSTRUCTION && post->Mission != MISSION_CONSTRUCTION) {

				post_on = true;
				switch (dir) {
					case FACING_N:
						first_frame = 5;
						mid_frame = 4;
						last_frame = 6;
						break;

					case FACING_S:
						first_frame = 6;
						mid_frame = 4;
						last_frame = 5;
						break;

					case FACING_E:
						first_frame = 1;
						mid_frame = 0;
						last_frame = 2;
						break;

					case FACING_W:
						first_frame = 2;
						mid_frame = 0;
						last_frame = 1;
						break;

					default:
						break;
				}
			}

			int index = 0;
			while (building != NULL) {
				if (index >= range) break;
				if (!building->Class->IsLaserFence) break;
				if (Owner() != building->Owner()) break;
				if ((building->PrimaryFacing.Current().As_Axis()) != face) break;

				last = building;
				if (building == first) {
					building->LaserFenceFrame = first_frame;
				} else {
					building->LaserFenceFrame = mid_frame;
				}
				building->IsToDisplay = true;

				if (post_on) {
					ObjectClass * occupier = Map[working_cell].Cell_Occupier();
					while (occupier != NULL) {
						RTTIType rtti = occupier->RTTI;
						if (rtti > RTTI_NONE && (rtti <= RTTI_AIRCRAFT || rtti == RTTI_INFANTRY)) {
							ObjectClass * next = occupier->Next;
							if (occupier->Strength > 0) {
								occupier->Take_Damage(occupier->Strength, 0, Rule->C4Warhead, building, true, true);
							}
							occupier = next;
						} else {
							occupier = occupier->Next;
						}
					}
				}

				index++;
				working_cell = Next_Cell(working_cell, AdjacentCell[(int)dir & 0x07]); // RA2: as above
				building = Map[working_cell].Cell_Building();
			}

			if (post_on && last != NULL) {
				if (first == last) {
					if (face == 2) {
						last->LaserFenceFrame = 3;
					} else {
						last->LaserFenceFrame = 7;
					}
				} else {
					last->LaserFenceFrame = last_frame;
				}
				last->IsToDisplay = true;
			}
		}
	}
}


/// <summary>
/// RA2
/// </summary>
static inline Coord Ground_Fire_Offset(Point2D const & point)
{
	Coord offset;
	static_cast<TPoint2D<int> &>(offset) = point;
	offset.Z = 0;
	return(offset);
}


/// <summary>
/// Fetches the coordinate that this building fires from.
/// The building type may pin the muzzle to an explicit offset; failing that, the
/// position of the turret or the voxel barrel is taken into account so that the shot
/// leaves the weapon rather than the middle of the building.
/// </summary>
/// <param name="slot">The weapon slot that is doing the firing.</param>
/// <returns>Returns with the coordinate that the projectile should appear at.</returns>
#ifdef RA2_NON_MATCHING // not exact: 381 bytes differ (adopted text)
Coord BuildingClass::Fire_Coord(int slot) const
{
	if (Class->CanBeOccupied && Occupants.Count() > 0) { // RA2: an occupied garrison fires from the occupant's muzzle flash
		Coord offset(TacticalMap->Pixel_To_Lepton(Class->MuzzleFlash[FiringOccupantIndex]), 0); // RA2: as above
		return(Render_Coord() + offset); // RA2: as above
	}
	Point2D const & pixel = Class->PrimaryFirePixelOffset; // RA2: the pixel offset is held by reference
	if (pixel != Point2D(0xFFFF, 0xFFFF)) {
		if (Class->PrimaryFireDualOffset) { // RA2: a dual offset adds the offset to the technos' own fire coord
			Coord offset(TacticalMap->Pixel_To_Lepton(pixel), 0); // RA2: as above
			return(BASECLASS::Fire_Coord(slot) + offset); // RA2: as above
		}
		Coord offset(TacticalMap->Pixel_To_Lepton(pixel), 0);
		return(Render_Coord() + offset);
	}
	if (Class->IsBarrelAnimAVoxel) {
		return(Voxel_Fire_Coord(slot, true));
	}

	Coord coord = BASECLASS::Fire_Coord(slot);
	if (Class->IsTurretAnimAVoxel) {
		Point2D pt(Class->AnimData[BANIM_TURRET].Location.X, Class->AnimData[BANIM_TURRET].Location.Y);
		coord += TacticalMap->Pixel_To_Lepton(pt);
	}
	return(coord);
}
#endif


/// <summary>
/// Fetches the coordinate that this building's weapon is mounted at.
/// The building type may pin the mounting to an explicit offset; failing that, the position
/// of the turret or the voxel barrel is taken into account, so that the shot originates from
/// the weapon rather than from the middle of the building.
/// </summary>
/// <param name="slot">The weapon slot to fetch the mounting for.</param>
/// <returns>Returns with the coordinate that the projectile should appear at.</returns>
#ifdef RA2_NON_MATCHING // not exact: 230 bytes differ (adopted text)
Coord BuildingClass::Turret_Coord(int slot) const
{
	if (Class->CanBeOccupied && Occupants.Count() > 0) { // RA2: an occupied garrison uses the occupant's muzzle flash
		Coord offset(TacticalMap->Pixel_To_Lepton(Class->MuzzleFlash[FiringOccupantIndex]), 0); // RA2: an occupied garrison uses the occupant's muzzle flash
		return(Render_Coord() + offset); // RA2: as above
	}
	Point2D const & pixel = Class->PrimaryFirePixelOffset; // RA2: the pixel offset is held by reference
	if (pixel != Point2D(0xFFFF, 0xFFFF)) {
		Coord offset(TacticalMap->Pixel_To_Lepton(pixel), 0);
		return(Render_Coord() + offset);
	}
	if (Class->IsBarrelAnimAVoxel) {
		return(Voxel_Fire_Coord(slot, false));
	}

	Coord coord = BASECLASS::Turret_Coord(slot);
	if (Class->IsTurretAnimAVoxel) {
		Point2D pt(Class->AnimData[BANIM_TURRET].Location.X, Class->AnimData[BANIM_TURRET].Location.Y);
		coord += TacticalMap->Pixel_To_Lepton(pt);
	}
	return(coord);
}
#endif


/// <summary>
/// Fetches the barrel end coordinate of a voxel barreled weapon.
/// This routine runs the type's barrel offset through the building's current barrel
/// matrix, so that a projectile appears at the end of the barrel wherever the weapon
/// happens to be aimed. A weapon that alternates barrels between shots is offset to
/// whichever barrel is its turn.
/// </summary>
/// <param name="which">The weapon slot to fetch the barrel for.</param>
/// <param name="just_fired">Should the barrel of the shot just fired be used rather than
/// the next one?</param>
/// <returns>Returns with the coordinate of the barrel end.</returns>
Coord BuildingClass::Voxel_Fire_Coord(int which, bool just_fired) const
{
	WeaponTypeClass *weapon = Get_Class_Weapon_Data(which)->Weapon;
	int burst;
	if (just_fired) {
		if (BurstIndex == 0) {
			burst = weapon->Burst - 1;
		} else {
			burst = BurstIndex - 1;
		}
	} else {
		burst = BurstIndex;
	}

	int yoffset;
	if (burst == 0) {
		yoffset = +Class->VoxelBarrelOffsetToBarrelEnd.Y;
	}
	else if (burst == 1) {
		yoffset = -Class->VoxelBarrelOffsetToBarrelEnd.Y;
	} else {
		yoffset = 0;
	}
	Vector3 vec(Class->VoxelBarrelOffsetToBarrelEnd.X, yoffset, Class->VoxelBarrelOffsetToBarrelEnd.Z);

	Vector3 v = Get_Barrel_Matrix() * vec;

	Coord coord = Coord(v.X, -v.Y, v.Z);

	coord = Render_Coord() + coord;

	Point2D lpt(Class->AnimData[BANIM_TURRET].Location.X, Class->AnimData[BANIM_TURRET].Location.Y);
	coord += TacticalMap->Pixel_To_Lepton(lpt);

	return(coord);
}


/// <summary>
/// Flags the cell at the coordinate specified as occupied by a building.
/// </summary>
/// <param name="coord">The coordinate of the cell to flag.</param>
void BuildingClass::Set_Occupy_Bit(Coord const & coord)
{
	Map[coord.As_Cell()].Flag.Occupy.Building = true;
}


/// <summary>
/// Clears the building occupation flag from the cell at the coordinate specified.
/// This routine is used as a building vacates a cell, so that the cell no longer
/// reports a building sitting upon it.
/// </summary>
/// <param name="coord">The coordinate of the cell to clear.</param>
void BuildingClass::Clear_Occupy_Bit(Coord const & coord)
{
	Map[coord.As_Cell()].Flag.Occupy.Building = false;
}


/// <summary>
/// Reads this building back in from a save game stream.
/// The building is rebuilt in place, re-registered with the target tracker, and every
/// pointer it holds is remapped from the saved identifier to the object that now lives at
/// that identity. Any light source it owned is dropped, since lights are not persisted.
/// </summary>
/// <returns>Returns with S_OK if the building was read, or the failure code from the
/// underlying stream.</returns>
HRESULT STDMETHODCALLTYPE BuildingClass::Load(IStream *stream)
{
	TargetTracker.Remove_Index(Fetch_ID());
	HRESULT result = TechnoClass::Load(stream); // RA2: RA2's form
	if (SUCCEEDED(result)) {
		int count; // RA2: as above
		new (this) BuildingClass(NoInitClass());
		TargetTracker.Add_Index(Fetch_ID(), this);
		Swizzle_Pointer(&Class);
		Swizzle_Pointer(&Factory);
		Swizzle_Pointer(&WhomToRepay);
		Swizzle_Pointer(&AnimToTrack);
		Swizzle_Pointer(&PsiWarnAnim); // RA2: as above
		Swizzle_Pointer(&BuildingLight);
		BuildupSoundController.Init(); // RA2: as above
		WorkingSoundController.Init(); // RA2: as above
		HRESULT read = stream->Read(&count, sizeof(count), 0); // RA2: as above
		if (FAILED(read)) { // RA2: as above
			return read; // RA2: as above
		}
		for (int index = 0; index < count; index++) { // RA2: as above
			InfantryClass *infantry; // RA2: as above
			read = stream->Read(&infantry, sizeof(infantry), 0); // RA2: as above
			if (FAILED(read)) { // RA2: as above
				return read; // RA2: as above
			}
			Overpowerers.Add(infantry); // RA2: as above
		}
		for (index = 0; index < count; index++) { // RA2: as above
			Swizzle_Pointer(&Overpowerers[index]); // RA2: as above
		}
		read = stream->Read(&count, sizeof(count), 0); // RA2: as above
		if (FAILED(read)) { // RA2: as above
			return read; // RA2: as above
		}
		for (index = 0; index < count; index++) { // RA2: as above
			InfantryClass *infantry; // RA2: as above
			read = stream->Read(&infantry, sizeof(infantry), 0); // RA2: as above
			if (FAILED(read)) { // RA2: as above
				return read; // RA2: as above
			}
			Occupants.Add(infantry); // RA2: as above
		}
		for (index = 0; index < count; index++) { // RA2: as above
			Swizzle_Pointer(&Occupants[index]); // RA2: as above
		}
		for (index = 0; index < 18; index++) { // RA2: as above
			Swizzle_Pointer(&Anims[index]); // RA2: as above
		}
		for (index = 0; index < 3; index++) { // RA2: as above
			Swizzle_Pointer(&Upgrades[index]); // RA2: as above
		}
		for (index = 0; index < 8; index++) { // RA2: as above
			Swizzle_Pointer(&DamageFireAnims[index]); // RA2: as above
		}
		LightSource = 0; // RA2: as above
	}
	return result; // RA2: as above
}




/// <summary>
/// Writes this building out to a save game stream.
/// The base class does the actual work of committing the object; this routine exists so
/// that the building layer has somewhere to hang any extra persistence of its own.
/// </summary>
/// <returns>Returns with S_OK if the building was written, or the failure code from the
/// underlying stream.</returns>
HRESULT STDMETHODCALLTYPE BuildingClass::Save(IStream * stream, BOOL cleardirty) // RA2: Save at 0x44F2F0 matches only so
{
	HRESULT result = BASECLASS::Save(stream, cleardirty);
	if (SUCCEEDED(result)) {
		int index; // RA2: as above
		HRESULT hr; // RA2: as above
		int count = Overpowerers.Count(); // RA2: as above

		hr = stream->Write(&count, sizeof(count), NULL); // RA2: as above
		if (FAILED(hr)) { // RA2: as above
			return(hr); // RA2: as above
		}

		for (index = 0; index < count; index++) { // RA2: as above
			hr = stream->Write(&Overpowerers[index], sizeof(Overpowerers[index]), NULL); // RA2: as above
			if (FAILED(hr)) { // RA2: as above
				return(hr); // RA2: as above
			}
		}

		count = Occupants.Count(); // RA2: as above

		hr = stream->Write(&count, sizeof(count), NULL); // RA2: as above
		if (FAILED(hr)) { // RA2: as above
			return(hr); // RA2: as above
		}

		for (index = 0; index < count; index++) { // RA2: as above
			hr = stream->Write(&Occupants[index], sizeof(Occupants[index]), NULL); // RA2: as above
			if (FAILED(hr)) { // RA2: as above
				return(hr); // RA2: as above
			}
		}
	}
	return(result);
}


/// <summary>
/// Is this building waiting to be told to get on with it?
/// A building that has finished its current animation stage sits idle until the mission
/// logic acknowledges the fact and moves it along.
/// </summary>
/// <returns>bool; Is the building ready to commence its next action?</returns>
bool BuildingClass::Ready_To_Commence(void)
{
	if (IsReadyToCommence) {
		return(true);
	}
	return(false);
}





/// <summary>
/// Adds this building's state into the game checksum.
/// The multiplayer sync check uses this routine to prove that every machine agrees about
/// the building. Only state that must stay in step across the network is contributed;
/// purely cosmetic values are left out.
/// </summary>
/// <param name="crc">The checksum engine to submit the building's state to.</param>
void BuildingClass::Compute_CRC(CRCEngine & crc) const
{
	TechnoClass::Compute_CRC(crc); // RA2: RA2's form
	crc(CountDown.Value()); // RA2: as above
	crc(CashProductionTimer.Value()); // RA2: as above
	crc((long)BState); // RA2: as above
	crc((long)QueueBState); // RA2: as above
	crc((long)WhoLastHurtMe); // RA2: as above
	if (WhomToRepay != 0) { // RA2: as above
		crc(WhomToRepay->Fetch_ID());
	}
	crc((long)LastStrength); // RA2: as above
	if (AnimToTrack != 0) { // RA2: as above
		crc(AnimToTrack->Fetch_ID());
	}
	crc(PlacementDelay.Value()); // RA2: as above
	crc((long)TurretIndex); // RA2: as above
	crc((long)LaserFenceFrame); // RA2: as above
	crc((long)FirestormWallFrame); // RA2: as above
	crc(IsOn);
	crc(IsPoweredOn);
	crc(IsToRebuild);
	crc(IsToRepair);
	crc(IsAllowedToSell);
	crc(IsReadyToCommence);
	crc(IsRepairing);
	crc(IsWrenchVisible);
	crc(IsGoingToBlow);
	crc(IsSurvivorless);
	crc(IsCharging);
	crc(IsCharged);
	crc(IsCaptured);
	crc(HasOpened);
	crc(HasEngineer); // RA2: as above
	crc(UnusedBuildingBool1);
	crc(IsDamagedAnims);
	crc((long)(unsigned short)Brightness); // RA2: as above
	crc(UpgradeLevel);
}



/// <summary>
/// Determines how visible this building appears.
/// A cloaked building is hidden outright unless the observer owns it or is sensing the
/// cell it stands on, and a building part way through cloaking is drawn progressively
/// dimmer. Anything not cloaked falls back to the normal object visibility rules.
/// </summary>
/// <param name="raw">Should the answer be given for the specified house rather than for
/// the local player?</param>
/// <param name="house">The house doing the observing.</param>
/// <returns>Returns with the visual style the building should be rendered with.</returns>
VisualType BuildingClass::Visual_Character(bool raw, HouseClass const * house) const
{
	if (TranslucencyLevel != 0) {
		if (TranslucencyLevel > 10) {
			if (raw) {
				if (house != NULL) {
					if (Map[PositionCoord.As_Cell()].Is_Sensed(house->HeapID)) {
						return(VISUAL_SHADOWY);
					}
				}
			} else {
				if (IsOwnedByPlayer || Is_Sensed_By_Player() || !MainWindow || (Session.Type != GAME_NORMAL && House != NULL && PlayerPtr != NULL && PlayerPtr->Is_Ally(House) && House->Is_Ally(PlayerPtr))) {
					return(VISUAL_SHADOWY);
				}
			}
			return(VISUAL_HIDDEN);
		}

		if (TranslucencyLevel > 5) {
			return(VISUAL_DARKEN);
		}

		return(VISUAL_INDISTINCT);
	}

	return(BASECLASS::Visual_Character(raw, house));
}

/// <summary>
/// Restarts the building's powered animations and brings back the ones that come and go with its power.
/// </summary>
void BuildingClass::Enable_Powered_Anims(void) // RA2: not in TibSun, at 0x44F730
{
	for (int index = 0; index < 18; index++) {
		if (Class->AnimData[index].Powered) {
			if (Anims[index] != NULL) {
				Anims[index]->Enable();
			}
		} else if (Class->AnimData[index].PoweredLight && Anims[index] == NULL && HasOpened) {
			if (!(Class->IsAnimDelayedFire && index == 10)) {
				Begin_Anim((BAnimType)index, HealthRatio <= Rule->ConditionYellow, false, 0);
			}
		}
	}
}


/// <summary>
/// Pauses the building's powered animations and removes the ones that come and go with its power.
/// </summary>
void BuildingClass::Disable_Powered_Anims(void) // RA2: not in TibSun, at 0x44F800
{
	for (int index = 0; index < 18; index++) {
		if (Class->AnimData[index].Powered) {
			if (Anims[index] != NULL) {
				Anims[index]->Disable();
			}
		} else if (Class->AnimData[index].PoweredLight) {
			if (Anims[index] != NULL) {
				End_Anim((BAnimType)index);
				if (index == 10 && Class->IsAnimDelayedFire && Class->AnimData[3].Powered) {
					Begin_Anim((BAnimType)3, HealthRatio <= Rule->ConditionYellow, false, 0);
				}
			}
		}
	}
}


/// <summary>
/// Brings the cloak generator, gap generator and powered animations in line with whether the building
/// has power.
/// </summary>
void BuildingClass::Update_Power_State(void) // RA2: not in TibSun, at 0x44F8E0
{
	if (IsPowerOnline()) {
		if (Class->IsCloakGenerator) {
			if (CloakGeneratorState <= 0) {
				char radius = CurrentCloakRadius;
				if (radius != Class->CloakRadiusInCells) {
					CloakGeneratorState = 1;
					if (radius == Class->CloakRadiusInCells) {
						CurrentCloakRadius = 0;
					}
					IsToDisplay = true;
				}
			}
		}
		if (Class->GapGenerator && !GeneratingGap) {
			CreateGap();
		}
		if (Class->IsPowered && Class->Drain > 0 && Class->IsCanTogglePower) {
			Enable_Powered_Anims();
		}
	} else {
		if (Class->IsCloakGenerator && CloakGeneratorState >= 0 && CurrentCloakRadius != 0) {
			CloakGeneratorState = -1;
			IsToDisplay = true;
		}
		if (Class->GapGenerator && GeneratingGap) {
			DestroyGap();
			if (GapSuperCharged) {
				GapSuperCharged = false;
				HasExtraPowerDrain = false;
				House->RecalcPower = true;
				GapRadius = Class->GapRadiusInCells;
				CreateGap();
			}
		}
		if (Class->IsPowered && Class->Drain > 0 && Class->IsCanTogglePower) {
			Disable_Powered_Anims();
		}
	}
}


/// <summary>
/// Brings a house's buildings into line with its power supply.
/// Call this routine whenever the power balance of a house changes. Buildings that need
/// power have their animations, laser fences and cloak generators started or stopped to
/// suit, so that a browning out base visibly goes dark.
/// </summary>
/// <param name="house">The house whose buildings are to be adjusted.</param>
void Adjust_House_Power(HouseClass * house)
{
	int count = house->Buildings.Count(); // RA2: RA2's form
	for (int index = 0; index < count; ++index) { // RA2: as above
		BuildingClass *building = house->Buildings[index]; // RA2: as above
		if (building->IsActive && !building->IsInLimbo) { // RA2: as above
			if (building->Class->IsLaserFencePost) // RA2: as above
				building->Toggle_Laser_Fence_Post(0); // RA2: as above
			building->Update_Power_State(); // RA2: as above
		}
	}
}





















/// <summary>
/// Starts this cloak generator's field growing.
/// The field is not thrown up at once; the cloaking logic pushes it outward a ring at a
/// time on subsequent game frames.
/// </summary>
void BuildingClass::Enable_Cloak_Generator(void)
{
	if (Class->IsCloakGenerator) {
		CloakGeneratorState = 1;
		if (CurrentCloakRadius == Class->CloakRadiusInCells) {
			CurrentCloakRadius = 0;
		}
		IsToDisplay = true;
	}
}


/// <summary>
/// Starts this cloak generator's field collapsing.
/// The field is not withdrawn at once; the cloaking logic peels it back a ring at a time
/// on subsequent game frames.
/// </summary>
void BuildingClass::Disable_Cloak_Generator(void)
{
	if (Class->IsCloakGenerator) {
		CloakGeneratorState = -1;
		if (CurrentCloakRadius == 0) {
			CurrentCloakRadius = Class->CloakRadiusInCells;
		}
		IsToDisplay = true;
	}
}


/// <summary>
/// Asks everything standing in the cell to reconsider cloaking.
/// Use this routine after the cloak coverage of a cell has changed. Only the mobile
/// occupants are given the chance; buildings look after their own cloaking.
/// </summary>
/// <param name="cellptr">Pointer to the cell whose occupants are to be prompted.</param>
void __fastcall Cloak_Cell_Occupiers(CellClass * cellptr) // RA2: Cloak_Cell_Occupiers at 0x44FB10 matches only so
{
	ObjectClass * occupier = cellptr->Cell_Occupier();
	while (occupier != NULL) {
		RTTIType rtti = occupier->RTTI;
		if (rtti == RTTI_UNIT || rtti == RTTI_INFANTRY || rtti == RTTI_AIRCRAFT) {
			((FootClass *)occupier)->Try_To_Cloak(); // RA2: as above
		}
		occupier = occupier->Next;
	}
}


/// <summary>
/// Handles the cloaking logic for this building.
/// This routine fades the building in and out of view as it cloaks and uncloaks, and it
/// grows or collapses the field of a cloak generator one ring of cells at a time. Any unit
/// standing in a cell the field reaches is asked to hide itself.
/// </summary>
/// <param name="fast">Should a collapsing field be given up in one pass rather than
/// yielding at another cloaked building?</param>
void BuildingClass::Cloaking_AI(bool fast)
{
	if (Class == NULL) {
		return;
	}

	Cell center = Cell(Center_Coord()); // RA2: RA2's form

	if (Cloak == CLOAKING) {
		if (TranslucencyLevel < 15) {
			++TranslucencyLevel; // RA2: as above
			if (TranslucencyLevel == 1 || TranslucencyLevel == 6 || TranslucencyLevel == 11) {
				IsToDisplay = true;
			}
			Set_Anim_Translucency(TranslucencyLevel);
			if (TranslucencyLevel == 15) {
				Cloak = CLOAKED;
				if (ParticleSystems[ATTACHED_PARTICLE_NATURAL] != NULL) {
					delete ParticleSystems[ATTACHED_PARTICLE_NATURAL];
					ParticleSystems[ATTACHED_PARTICLE_NATURAL] = NULL;
				}
			}
		}
	} else if (Cloak == UNCLOAKING) { // RA2: as above
		if (TranslucencyLevel >= 0) {
			if (TranslucencyLevel > 0) {
				--TranslucencyLevel; // RA2: as above
			}
			if (TranslucencyLevel == 0 || TranslucencyLevel == 5 || TranslucencyLevel == 10) {
				IsToDisplay = true;
			}
			Set_Anim_Translucency(TranslucencyLevel);
			if (TranslucencyLevel == 0) {
				Cloak = UNCLOAKED;
				if (ParticleSystems[ATTACHED_PARTICLE_NATURAL] == NULL && Class->NaturalParticleLocation != COORD_NONE) {
					Coord coord = Coord(Class->NaturalParticleLocation);
					ParticleSystems[ATTACHED_PARTICLE_NATURAL] = new ParticleSystemClass(Class->NaturalParticleSystem, // RA2: as above
						PositionCoord + coord, &Map[Get_Coord()], NULL); // RA2: as above
				}
			}
		}
	}

	if (Cloak == CLOAKED && Should_Uncloak()) {
		Do_Uncloak(false); // RA2: as above
	}

	if (Cloak == UNCLOAKED && Is_Ready_To_Cloak()) {
		Do_Cloak(false); // RA2: as above
	}

	if (CloakGeneratorState != 0 && Class->IsCloakGenerator) {
		HousesType houseid = House->HeapID;
		BSurface * cloaking_surface = CloakingSurface;
		int width = cloaking_surface->Get_Width();
		int width_half = width / 2;

		if (CloakGeneratorState > 0) {
			if (CurrentCloakRadius == Class->CloakRadiusInCells) {
				CloakGeneratorState = 0;
			} else {
				++CurrentCloakRadius; // RA2: as above
				cloaking_surface->Fill(0);
				cloaking_surface->Draw_Circle(Point2D(width_half, width_half), CurrentCloakRadius, cloaking_surface->Get_Rect(), 1);
				char * data = (char *)cloaking_surface->Lock();
				Cell origin = center - Cell(width_half, width_half);

				for (int y = 0; y < width; ++y) { // RA2: as above
					for (int x = 0; x < width; ++x) { // RA2: as above
						if (*data++ != 0) {
							Cell current = origin + Cell(x, y);
							CellClass * cellptr = &Map[current];
							if (!cellptr->Is_Cloaked(houseid)) {
								cellptr->Cloaked_By(houseid);
								Cloak_Cell_Occupiers(cellptr);
								BuildingClass * building = static_cast<BuildingClass *>(cellptr->Cell_Building()); // RA2: as above
								if (building && building->House->HeapID == houseid && Cell(building->Center_Coord()) == current) { // RA2: as above
									--CurrentCloakRadius; // RA2: as above
									return;
								}
							}
						}
					}
				}
			}
		} else if (CurrentCloakRadius == 0) { // RA2: as above
			CloakGeneratorState = 0;
		} else { // RA2: as above
			--CurrentCloakRadius; // RA2: as above
			cloaking_surface->Fill(2);
			cloaking_surface->Draw_Circle(Point2D(width_half, width_half), Class->CloakRadiusInCells, cloaking_surface->Get_Rect(), 0);
			if (CurrentCloakRadius != 0) {
				cloaking_surface->Draw_Circle(Point2D(width_half, width_half), CurrentCloakRadius, cloaking_surface->Get_Rect(), 1);
			}
			char * data = (char *)cloaking_surface->Lock();
			Cell origin = center - Cell(width_half, width_half);

			for (int y = 0; y < width; ++y) { // RA2: as above
				for (int x = 0; x < width; ++x) { // RA2: as above
					if (*data++ == 0) {
						Cell current = origin + Cell(x, y);
						CellClass * cellptr = &Map[current];
						if (cellptr->Is_Cloaked(houseid)) {
							cellptr->Uncloaked_By(houseid);
							Cloak_Cell_Occupiers(cellptr);
							BuildingClass * building = static_cast<BuildingClass *>(cellptr->Cell_Building()); // RA2: as above
							if (building && building->House->HeapID == houseid && Cell(building->Center_Coord()) == current && (char)fast == 0) { // RA2: as above
								++CurrentCloakRadius; // RA2: as above
								return; // RA2: as above
							}
						}
					}
				}
			}

			if (CurrentCloakRadius == 0) {
				for (int i = 0; i < Buildings.Count(); ++i) { // RA2: as above
					BuildingClass * building = Buildings[i];
					if (building->IsActive && building != this && building->Class->IsCloakGenerator && // RA2: as above
						building->IsPowerOnline() && building->CloakGeneratorState == 0) { // RA2: as above
						Cell diff = center - Cell(building->Center_Coord()); // RA2: as above
						int dy = diff.Y; // RA2: as above
						int dx = diff.X; // RA2: as above
						if (dy * dy + dx * dx < 4 * ((Class->CloakRadiusInCells + 2) * (Class->CloakRadiusInCells + 2))) { // RA2: as above
							if (building->Class->IsCloakGenerator) { // RA2: as above
								building->CloakGeneratorState = 1; // RA2: as above
								if (building->CurrentCloakRadius == building->Class->CloakRadiusInCells) { // RA2: as above
									building->CurrentCloakRadius = 0; // RA2: as above
								}
								building->IsToDisplay = true; // RA2: as above
							}
							if (building->CurrentCloakRadius != 0) { // RA2: as above
								--building->CurrentCloakRadius; // RA2: as above
							}
						}
					}
				}
			}
		}
	}
}



/// <summary>
/// Is this building up and running?
/// A building is only considered operational when the player has not switched it off, it
/// is not stunned, it is still standing, and the owning house is supplying enough power to
/// run it.
/// </summary>
/// <returns>bool; Is the building powered on and able to perform its function?</returns>
bool BuildingClass::IsPowerOnline(void) const // RA2: Is_Powered_On is the IsPowerOnline virtual
{
	if (!IsOn && Overpowerers.Count() < 2) { // RA2: two overpowering units keep it running
		return(false);
	}
	if (StunDuration > 0) {
		return(false);
	}
	if (Strength == 0) {
		return(false);
	}
	if (Class->IsPowered && Class->Drain > 0 && Class->IsCanTogglePower) { // RA2: as above
		if (House->Power_Fraction() < 1.0 && Overpowerers.Count() < 2) { // RA2: as above
			return(false); // RA2: as above
		}
	}
	if (Class->NeedsEngineer && !HasEngineer) { // RA2: a building that needs an engineer
		return(false);
	}
	return(true);
}



/// <summary>
/// Switches this sensor array off.
/// The sensed flag is lifted from every cell within the array's radius, so that cloaked
/// objects standing there are hidden again. The house's other sensor arrays are then
/// re-enabled, since their coverage may well have overlapped the area just given up.
/// </summary>
void BuildingClass::SensorArrayDeactivate(Cell cell) // RA2: RA2's form
{
	int range = Class->CloakRadiusInCells; // RA2: as above
	int range_squared = range * range; // RA2: as above
	HousesType house = House->HeapID; // RA2: as above
	if (cell == CELL_NONE) { // RA2: as above
		cell = Center_Coord().As_Cell(); // RA2: as above
	}
	for (int y = -range; y < range; y++) { // RA2: as above
		for (int x = -range; x < range; x++) { // RA2: as above
			Cell spot = Cell(cell.X + x, cell.Y + y); // RA2: as above
			if (x * x + y * y < range_squared) { // RA2: as above
				CellClass *object = &Map[spot]; // RA2: as above
				object->Unsensed_By(house); // RA2: as above
				for (ObjectClass *occupier = object->Cell_Occupier(); occupier != 0; occupier = occupier->Next) { // RA2: as above
					int kind = occupier->Fetch_RTTI(); // RA2: as above
					if (kind == 1 || kind == 15 || kind == 2) { // RA2: as above
						((TechnoClass *)occupier)->Try_To_Cloak(); // RA2: as above
					}
				}
			}
		}
	}
}



/// <summary>
/// Turns on a disguise detector: the cells within its SensorsSight are sensed by the owner,
/// and the units, infantry and aircraft standing in them are told they were sensed.
/// </summary>
/// <param name="cell">The cell to center on; CELL_NONE means the building's own cell.</param>
void BuildingClass::DisguiseDetectorActivate(Cell cell) // RA2: not in TibSun, at 0x450550
{
	if (!IsPowerOnline()) {
		return;
	}

	int radius = Class->SensorsSight;
	int dist = radius * radius;
	HousesType houseid = House->HeapID;
	if (cell == CELL_NONE) {
		cell = Center_Coord().As_Cell();
	}
	for (int y = -radius; y < radius; y++) {
		for (int x = -radius; x < radius; x++) {
			Cell newcell = Cell(cell.X + x, cell.Y + y);
			if (x * x + y * y < dist) {
				CellClass * cellptr = &Map[newcell];
				cellptr->Sensed_By(houseid); // RA2: Sensed_By, where the deactivation calls Disguise_Unsensed_By
				for (ObjectClass * occupier = cellptr->Cell_Occupier(); occupier != NULL; occupier = occupier->Next) {
					RTTIType rtti = occupier->RTTI;
					if (rtti == RTTI_UNIT || rtti == RTTI_INFANTRY || rtti == RTTI_AIRCRAFT) {
						((TechnoClass *)occupier)->Try_To_Cloak();
					}
				}
			}
		}
	}
}


/// <summary>
/// Turns off a disguise detector: the cells within its DetectDisguiseRange are no longer
/// disguise-sensed by the owner.
/// </summary>
/// <param name="cell">The cell to center on; CELL_NONE means the building's own cell.</param>
void BuildingClass::DisguiseDetectorDeactivate(Cell cell) // RA2: not in TibSun, at 0x4506B0
{
	int radius = Class->DetectDisguiseRange;
	int dist = radius * radius;
	HousesType houseid = House->HeapID;
	if (cell == CELL_NONE) {
		cell = Center_Coord().As_Cell();
	}
	for (int y = -radius; y < radius; y++) {
		for (int x = -radius; x < radius; x++) {
			Cell newcell = Cell(cell.X + x, cell.Y + y);
			if (x * x + y * y < dist) {
				CellClass * cellptr = &Map[newcell];
				cellptr->Disguise_Unsensed_By(houseid);
			}
		}
	}
}


/// <summary>
/// Switches this sensor array on.
/// Every cell within the array's radius is marked as sensed by the owning house, which
/// reveals cloaked objects standing there and forces any cloaked unit in the area to
/// reconsider whether it can stay hidden. A powered down array does nothing.
/// </summary>
void BuildingClass::SensorArrayActivate(Cell cell) // RA2: Enable_Sensor_Array is the virtual SensorArrayActivate
{
	if (IsPowerOnline()) { // RA2: as above
		int range = Class->DetectDisguiseRange; // RA2: as above
		int range_squared = range * range; // RA2: as above
			HousesType house = House->HeapID; // RA2: as above
		if (cell == CELL_NONE) { // RA2: as above
			cell = Center_Coord().As_Cell(); // RA2: as above
		}
		for (int y = -range; y < range; y++) { // RA2: as above
			for (int x = -range; x < range; x++) { // RA2: as above
				Cell spot = Cell(cell.X + x, cell.Y + y); // RA2: as above
				if (x * x + y * y < range_squared) { // RA2: as above
					CellClass *object = &Map[spot]; // RA2: as above
				object->Disguise_Sensed_By(house); // RA2: Disguise_Sensed_By (0x47C380)
				}
			}
		}
	}
}




















/// <summary>
/// Determines which firestorm wall shape this section should use.
/// The four cardinal neighbors are examined for other firestorm wall sections, so that the
/// wall can knit itself together into corners, tees and straight runs.
/// </summary>
/// <returns>Returns with the connection bits describing which neighbors are wall.</returns>
int BuildingClass::Get_Firestorm_Wall_Frame(void)
{
	unsigned frame = 0;
	CellClass * cellptr = &Map[Get_Coord()];
	for (int i = FACING_N; i < FACING_COUNT; i += FACING_90) {
		BuildingClass * building = cellptr->Adjacent_Cell((FacingType)i).Cell_Building();
		if (building != NULL && building->Class->IsFirestormWall) {
			if (!building->IsInLimbo && building->IsActive) {
				frame |= 1 << (i >> 1);
			}
		}
	}
	return(frame);
}


/// <summary>
/// Updates the state of this firestorm wall section.
/// The section picks the shape that matches its neighbors and whether the house has the
/// firestorm defense switched on. While the defense is active, anything sharing the wall's
/// cell -- or trying to move into one of the cells nearby -- is destroyed.
/// </summary>
void BuildingClass::Update_FS_Wall_State(void)
{
	bool active = false;

	if (!Class->IsFirestormWall) {
		return;
	}

	unsigned frame = Get_Firestorm_Wall_Frame();
	if (House->FirestormDefenseActivated) {
		frame += 32;
		active = true;
	} else {
		active = active;
	}

	CellClass * cellptr = &Map[Get_Coord()];
	if (frame != (unsigned)FirestormWallFrame) {
		FirestormWallFrame = frame;
		IsToDisplay = true;
		cellptr->Recalc_Attributes();
	}

	unsigned frame_mod_16 = frame % 16;
	if (!active || (frame_mod_16 == 10) || (frame_mod_16 == 5) || Anims[BANIM_SPECIAL_ONE] != NULL) {
		if (Anims[BANIM_SPECIAL_ONE] != NULL) {
			delete Anims[BANIM_SPECIAL_ONE];
			Anims[BANIM_SPECIAL_ONE] = NULL;
		}
	} else {
		Anims[BANIM_SPECIAL_ONE] = new AnimClass(Rule->FirestormActiveAnim, PositionCoord - Coord(CELL_LEPTON_W / 2, CELL_LEPTON_H / 2, 0), 1, 0, ShapeFlags_Type(SHAPE_WIN_REL|SHAPE_CENTER), -10);
		Anims[BANIM_SPECIAL_ONE]->IsFogged = IsFogged;
	}

	if (active) {
		ObjectClass * object = cellptr->Cell_Occupier();
		while (object != NULL) {
			if (object != this && object->Class_Of() != NULL && !object->Class_Of()->IsIgnoresFirestorm) {
				ObjectClass * next;
				switch ((RTTIType)object->RTTI) {
					case RTTI_UNIT:
					case RTTI_AIRCRAFT:
					case RTTI_INFANTRY:
						next = object->Next;
						Crossing_Firestorm(object, true);
						if (next != NULL) {
							object = next;
							continue;
						}
						break;
				}
			}
			object = object->Next;
		}

		Coord coord = cellptr->Cell_Coord();
		for (int x = -2; x < 3; x++) {
			for (int y = -2; y < 3; y++) {
				Cell cell = Cell(x, y);
				CellClass * cptr = &Map[cell + cellptr->CellID];
				if (cptr != cellptr) {
					ObjectClass * occupier = cptr->Cell_Occupier();
					while (occupier != NULL) {
						ObjectClass * next = occupier->Next;
						if (occupier->Is_Foot() && ((FootClass *)occupier)->Locomotion->Is_Moving_Here(coord) && !occupier->TClass->IsIgnoresFirestorm) {
							int damage = occupier->Strength;
							occupier->Take_Damage(damage, 0, Rule->C4Warhead, NULL, true, true);
						}
						occupier = next;
					}
				}
			}
		}
	}
}


/// <summary>
/// Handles an object caught in the firestorm wall.
/// The victim is destroyed outright and an appropriate flare is spawned, one flavor for
/// something caught in the air and another for something caught on the ground.
/// </summary>
/// <param name="do_damage">Should the object be destroyed, or only the flare shown?</param>
/// <returns>bool; Was the object caught by the firestorm?</returns>
bool BuildingClass::Crossing_Firestorm(ObjectClass * object, bool do_damage)
{
	if (object && object->Strength > 0) {
		if (do_damage) {
			object->Take_Damage(object->Strength, 0, Rule->FirestormWarhead, NULL, true, true);
		}
		if (object->HeightAGL > 100) {
			new AnimClass(Rule->FirestormAirAnim, object->PositionCoord, 0, 1, ShapeFlags_Type(SHAPE_WIN_REL|SHAPE_CENTER), -10);
		} else {
			new AnimClass(Rule->FirestormGroundAnim, PositionCoord, 0, 1, ShapeFlags_Type(SHAPE_WIN_REL|SHAPE_CENTER), -10);
		}
		return(true);
	}
	return(false);
}


/// <summary>
/// Fetches the screen rectangle this building draws into.
/// The answer is remembered between calls, so that the render pass can ask for it as often
/// as it likes without paying for the coordinate conversion each time.
/// </summary>
/// <returns>Returns with the rectangle, in screen pixels, that the building occupies.</returns>
Rect BuildingClass::Get_Render_Rect(void)
{
	Coord render = Render_Coord();
	Point2D tac(TacticalMap->TacPixelX, TacticalMap->TacPixelY);
	Rect rect;

	if (render == LastRenderCoord && tac == LastRenderOffset) {
		rect = LastRenderRect;
	} else {
		Point2D pixel;
		TacticalMap->Coord_To_Pixel(render, pixel);
		rect = Class->Get_Draw_Rect() + pixel;
		LastRenderCoord = render;
		LastRenderOffset = tac;
		LastRenderRect = rect;
	}

	return(rect);
}


/// <summary>
/// Assigns the destination for this building.
/// A building cannot travel, so the destination serves as the rally point for whatever it
/// produces. Factories and construction yards remember it as their archive target as well.
/// A building that is busy deconstructing itself ignores the order entirely.
/// </summary>
void BuildingClass::Assign_Destination(AbstractClass * target, bool immediate)
{
	if (CurrentMission != MISSION_DECONSTRUCTION) {
		if (Is_Move_Override() || Class->IsConstructionYard) {
			ArchiveTarget = target;
		}
		BASECLASS::Assign_Destination(target, immediate);
	}
}


/// <summary>
/// Can this building be given a movement order even though it cannot move?
/// A factory accepts a move order as a rally point for whatever it produces, so the cursor
/// logic must offer the move action for it in spite of the building being rooted in place.
/// </summary>
/// <returns>bool; Does a movement order to this building mean a rally point?</returns>
bool BuildingClass::Is_Move_Override(void) const
{
	RTTIType type = Class->ToBuild; // RA2: a factory of units, infantry or aircraft, a cloning vat or a repair depot
	if (type == RTTI_UNITTYPE || type == RTTI_INFANTRYTYPE || type == RTTI_AIRCRAFTTYPE || Class->Cloning || Class->IsCanUnitRepair) { // RA2: as above
		return(true); // RA2: as above
	}
	return(false); // RA2: as above
}



/// <summary>
/// Finds the nearest repair facility that will take the object.
/// Only facilities belonging to the specified house and willing to accept the object over
/// the radio are considered, and only those close enough to be worth the trip.
/// </summary>
/// <param name="house">The house whose repair facilities are to be searched.</param>
/// <param name="obj">The object that wishes to be repaired.</param>
/// <returns>Returns with a pointer to the best repair facility found. Otherwise, NULL is
/// returned.</returns>
BuildingClass * Find_Unit_Repair_Facility(HouseClass * house, TechnoClass * obj)
{
	BuildingClass * best_bld = NULL;
	int best_dist = 0x7FFFFFFF;

	static int max_dist = int(CELL_LEPTON_DIAG * 20.0);

	int count = house->UnitRepairStations.Count(); // RA2: Find_Unit_Repair_Facility at 0x450F90 matches only so
	for (int i = 0; i < count; i++) { // RA2: as above
		BuildingClass * bld = house->UnitRepairStations[i]; // RA2: as above
		if (obj->Transmit_Message(RADIO_CAN_LOAD, bld) != RADIO_STATIC) { // RA2: as above
			int dist = (bld->Get_Coord() - obj->Get_Coord()).Length(); // RA2: as above
			if (dist < best_dist && dist < max_dist) {
				best_bld = bld; // RA2: as above
				best_dist = dist;
			} // RA2: Find_Unit_Repair_Facility at 0x450F90 has no UnitRepairFacilities
		}
	}
	return(best_bld);
}


/// <summary>
/// Reserves the base area occupied by this building.
/// The computer player's base planner uses these reservations to keep its structures a
/// civilized distance apart and to know which cells lie inside the base perimeter. The
/// owning house's base rectangle is grown to take the new building in.
/// </summary>
/// <param name="skip_inner_cells">Should the base's inner cell list be left alone?</param>
void BuildingClass::Reserve_Base_Area(bool skip_inner_cells)
{
	int spacing = Rule->AIBaseSpacing;
	int width = 2 * spacing + Class->Width();
	int height = 2 * spacing + Class->Height();

	unsigned owner = 1 << House->HeapID;
	Cell top_left = PositionCoord.As_Cell() - Cell(spacing, spacing);

	for (int x = top_left.X; x < top_left.X + width; x++) {
		for (int y = top_left.Y; y < top_left.Y + height; y++) {
			Map[Cell(x, y)].OccupiedBy |= owner;
		}
	}

	Rect & base_rect = House->Base.BaseAreaRect;
	if (base_rect.X == 0) {
		base_rect.X = top_left.X;
	}
	if (base_rect.Y == 0) {
		base_rect.Y = top_left.Y;
	}
	if (top_left.X < base_rect.X) {
		int old_x = base_rect.X;
		base_rect.X = top_left.X;
		base_rect.Width += old_x - top_left.X;
	}
	if (top_left.X + width > base_rect.Width + base_rect.X) {
		base_rect.Width = top_left.X - base_rect.X + width;
	}
	if (top_left.Y < base_rect.Y) {
		int old_y = base_rect.Y;
		base_rect.Y = top_left.Y;
		base_rect.Height += old_y - top_left.Y;
	}
	if (top_left.Y + height > base_rect.Height + base_rect.Y) {
		base_rect.Height = top_left.Y - base_rect.Y + height;
	}

	if (!skip_inner_cells) {
		for (int x = top_left.X - 1; x < top_left.X + width + 2; x++) {
			for (int y = top_left.Y - 1; y < top_left.Y + height + 2; y++) {
				Cell cell = Cell(x, y);
				int mask = Map[cell].Occupation_Mask(House->HeapID);
				if (mask == (1 << FACING_COUNT) - 1) {
					House->Base.InnerCells.Delete(cell);
				} else if (mask > 0) {
					if (House->Base.InnerCells.ID(cell) == -1) {
						House->Base.InnerCells.Add(cell);
					}
				}
			}
		}
	}
}


/// <summary>
/// Releases the base area this building had reserved.
/// This routine is the counterpart of Reserve_Base_Area and is used when the building leaves
/// the map. The neighboring buildings re-reserve themselves afterwards, since they may well
/// still be claiming the cells that were just given up.
/// </summary>
void BuildingClass::Release_Base_Area(void)
{
	int spacing = Rule->AIBaseSpacing;
	int width = 2 * spacing + Class->Width();
	int height = 2 * spacing + Class->Height();

	unsigned owner = 1 << House->HeapID;
	Cell top_left = PositionCoord.As_Cell() - Cell(spacing, spacing);

	int x, y;

	for (x = top_left.X; x < top_left.X + width; x++) {
		for (y = top_left.Y; y < top_left.Y + height; y++) {
			Map[Cell(x, y)].OccupiedBy &= ~owner;
		}
	}

	for (x = top_left.X - spacing; x < top_left.X + width + spacing * 2; x++) {
		for (y = top_left.Y - spacing; y < top_left.Y + height + spacing * 2; y++) {
			BuildingClass * building = Map[Cell(x, y)].Cell_Building();
			if (building != NULL && building != this) {
				building->Reserve_Base_Area(true);
			}
		}
	}

	for (x = top_left.X - 1; x < top_left.X + width + 2; x++) {
		for (y = top_left.Y - 1; y < top_left.Y + height + 2; y++) {
			Cell cell = Cell(x, y);
			int mask = Map[cell].Occupation_Mask(House->HeapID);
			if (mask == (1 << FACING_COUNT) - 1) {
				House->Base.InnerCells.Delete(cell);
			} else if (mask > 0) {
				if (House->Base.InnerCells.ID(cell) == -1) {
					House->Base.InnerCells.Add(cell);
				}
			} else {
				House->Base.InnerCells.Delete(cell);
				Map[cell].OccupiedBy &= ~owner;
			}
		}
	}
}


/// <summary>
/// Fetches this building's worth as an anti-aircraft defense.
/// The computer player uses this rating when deciding whether its base is adequately
/// defended against aircraft. Any upgrade plugged into the building can supply the rating
/// if the building itself does not.
/// </summary>
/// <returns>Returns with the anti-aircraft defense value, or zero if it defends against no
/// aircraft at all.</returns>
int BuildingClass::Anti_Air_Defense_Value(void)
{
	if (Class->AntiAirValue > 0) {
		return(Class->AntiAirValue);
	}

	for (int i = 0; i < BUILDING_UPGRADE_MAX; i++) {
		if (Upgrades[i] != NULL && Upgrades[i]->AntiAirValue > 0) {
			return(Upgrades[i]->AntiAirValue);
			break;
		}
	}

	return(0);
}


/// <summary>
/// Fetches this building's worth as an anti-armor defense.
/// The computer player uses this rating when deciding whether its base is adequately
/// defended against vehicles. Any upgrade plugged into the building can supply the rating
/// if the building itself does not.
/// </summary>
/// <returns>Returns with the anti-armor defense value, or zero if it defends against no
/// armor at all.</returns>
int BuildingClass::Anti_Armor_Defense_Value(void)
{
	if (Class->AntiArmorValue > 0) {
		return(Class->AntiArmorValue);
	}

	for (int i = 0; i < BUILDING_UPGRADE_MAX; i++) {
		if (Upgrades[i] != NULL && Upgrades[i]->AntiArmorValue > 0) {
			return(Upgrades[i]->AntiArmorValue);
		}
	}

	return(0);
}


/// <summary>
/// Fetches this building's worth as an anti-infantry defense.
/// The computer player uses this rating when deciding whether its base is adequately
/// defended against infantry. Any upgrade plugged into the building can supply the rating
/// if the building itself does not.
/// </summary>
/// <returns>Returns with the anti-infantry defense value, or zero if it defends against
/// no infantry at all.</returns>
int BuildingClass::Anti_Infantry_Defense_Value(void)
{
	if (Class->AntiInfantryValue > 0) {
		return(Class->AntiInfantryValue);
	}

	for (int i = 0; i < BUILDING_UPGRADE_MAX; i++) {
		if (Upgrades[i] != NULL && Upgrades[i]->AntiInfantryValue > 0) {
			return(Upgrades[i]->AntiInfantryValue);
		}
	}

	return(0);
}


/// <summary>
/// Registers this building with the radar tracking list.
/// The radar keeps a record of every cell a tracked building covers, so that the radar map
/// can resolve a click back to the building that owns that cell.
/// </summary>
void BuildingClass::Radar_Track(void)
{
	const FOUNDATION_LIST & list = Map.Get_Foundation(Class);
	for (int i = 0; i < list.Count(); i++) {
		Point2D pt = list[i] + RadarPos;
		Map.Radar_Track(this, pt);
	}
	IsRadarTracked = true;
}


/// <summary>
/// Removes this building from the radar tracking list.
/// Use this routine when the building is being removed from the map or is no longer to be
/// shown on the radar. Every cell of its foundation is untracked.
/// </summary>
void BuildingClass::Radar_Untrack(void)
{
	const FOUNDATION_LIST & list = Map.Get_Foundation(Class);
	for (int i = 0; i < list.Count(); i++) {
		Point2D pt = list[i] + RadarPos;
		Map.Radar_Untrack(this, pt);
	}
	IsRadarTracked = false;
}


/// <summary>
/// Plots this building onto the radar map.
/// Every cell of the building's foundation is drawn, so that a large structure shows up on
/// the radar with its true footprint.
/// </summary>
void BuildingClass::Plot_On_Radar(void) const
{
	const FOUNDATION_LIST & list = Map.Get_Foundation(Class);
	for (int i = 0; i < list.Count(); i++) {
		Point2D pt = list[i] + RadarPos;
		Map.Radar_Pixel(pt);
	}
}





/// <summary>
/// The radius, in cells, of the indicator drawn around this building: its psychic detection radius, a gap
/// generator's (super charged or not) or a sensor array's and cloak generator's radius, else its weapon's range.
/// </summary>
int BuildingClass::GetRadialIndicatorRange(void) const // RA2: not in TibSun, at 0x451930
{
	int range = 0;
	BuildingTypeClass * type = Class; // RA2: the type is loaded once, before the first test
	int psychic = type->PsychicDetectionRadius; // RA2: as above
	if (psychic > 0) {
		range = psychic;
	} else if (type->GapGenerator) {
		range = GapSuperCharged ? type->SuperGapRadiusInCells : type->GapRadiusInCells;
	} else if (type->IsSensorArray || type->IsCloakGenerator) {
		range = type->CloakRadiusInCells;
	} else {
		WeaponDataStruct const * weapon = TechnoClass::GetTurretWeapon();
		if (Is_Weapon_Valid(weapon) && weapon->Weapon->Range > 0) { // RA2: Is_Weapon_Valid (0x6D8640, this in ecx) instead of the NULL tests
			range = weapon->Weapon->Range / 256;
		}
	}
	return range;
}


/// <summary>
/// Draws the radial range indicator for this building.
/// Sensor arrays and cloak generators show the extent of their field as an ellipse on the
/// tactical map, with a few spokes sweeping around it. Only the owning player sees it, and
/// only while the building is switched on.
/// </summary>
void BuildingClass::Draw_Radial_Indicator(DWORD sweep) // RA2: takes an argument; not const
{
	unsigned radius = GetRadialIndicatorRange(); // RA2: the range in leptons, the type's radial indicator range
	if ((int)radius > 0 && IsOn && House->Is_Player_Control()) { // RA2: only the owning player sees it, while the building is switched on
		if (Class->ConcentricRadialIndicator) { // RA2: a pulsing ellipse of the full range and a growing one inside it
			static RGBClass white(255, 255, 255); // RA2: the color the pulse fades towards
			RGBClass color = House->RemapColorRGB; // RA2: the house color, faded to white in the pulse
			unsigned phase = (timeGetTime() >> 1) & 0x3FF; // RA2: the pulse phase, 1024 steps of two milliseconds
			if (!(phase & 0x200)) { // RA2: the full ellipse is shown during the first half of the pulse
				if (!(phase & 0x100)) { // RA2: and fades in during the first quarter
					color.Adjust(~phase & 0xFF, white); // RA2: as above
				}
				Draw_Radial_Ellipse(radius, false, Center_Coord(), color, false, false); // RA2: as above
			}
			double scaled = (double)radius + 0.5; // RA2: the range in pixels, from half cells
			unsigned pixels = (unsigned)(int)(scaled / fastmath::sqrt(2.0) * 60.0); // RA2: as above
			pixels = (pixels * phase) >> 10; // RA2: the growing ellipse
			if (pixels > 32) { // RA2: it shows only once it is large enough
				Draw_Radial_Ellipse(pixels, false, Center_Coord(), House->RemapColorRGB, false, true); // RA2: as above
			}
		} else {
			Draw_Radial_Ellipse(radius, *(bool *)&sweep, Center_Coord(), House->RemapColorRGB, true, false); // RA2: the plain ellipse; the low byte of the argument is passed on as it is (no bool normalisation)
		}
	}
}



/// <summary>
/// Draws a radial indicator: an isometric ellipse (two pixels thick) around a coordinate and,
/// optionally, four sweeping spokes whose brightness trails off.
/// </summary>
/// <param name="radius">The radius, in cells unless in_pixels is set.</param>
/// <param name="sweep">Should the rotating spokes be drawn?</param>
/// <param name="center">The coordinate the indicator is centered on.</param>
/// <param name="color">The color of the indicator.</param>
/// <param name="faded">Should the ellipse be drawn at half brightness?</param>
/// <param name="in_pixels">Is the radius in pixels already?</param>
void Draw_Radial_Ellipse(unsigned radius, bool sweep, Coord center, RGBClass color, bool faded, bool in_pixels) // RA2: not in TibSun, at 0x451BF0
{
	static double _transparencies[] = { 0.05, 0.2, 0.4, 1.0 };

	int pixels;
	if (in_pixels) {
		pixels = radius;
	} else {
		pixels = (int)(((double)radius + 0.5) / fastmath::sqrt(2.0) * 60.0);
	}

	Point2D pixel;
	TacticalMap->Coord_To_Pixel(center, pixel);
	pixel += TacticalRect.TopLeft;

	Rect bounds = Rect(pixel.X - pixels, pixel.Y - pixels / 2, pixels * 2, pixels);
	Rect clipped = Intersect(bounds, TacticalRect);

	if (clipped.Is_Valid()) {
		RGBClass black(0, 0, 0);
		RGBClass shade = color;
		if (faded) {
			shade.Adjust(50, black);
		}
		LogicalSurface->Draw_Ellipse(pixel, pixels, pixels / 2, TacticalRect, DSurface::Build_Hicolor_Pixel(shade.Get_Red(), shade.Get_Green(), shade.Get_Blue()));
		LogicalSurface->Draw_Ellipse(pixel, pixels - 1, pixels / 2 - 1, TacticalRect, DSurface::Build_Hicolor_Pixel(shade.Get_Red(), shade.Get_Green(), shade.Get_Blue()));

		if (sweep) {
			double radius_x = pixels;
			double radius_y = pixels / 2;

			for (int i = 0; i < 4; i++) {

				/// The subtracted term is (angle / (2 * PI)) truncated, so this wraps
				/// the sweep angle back into a single revolution.
				double angle = (double)(Frame + i) * 0.005;
				angle = angle - (double)(int)(angle * (1.0 / DEG_TO_RAD(360))) * DEG_TO_RAD(360);
				Point2D end;

				if (fabs(angle - M_PI / 2) < 0.001) {
					end = pixel + Point2D(0, -radius_y);
				} else if (fabs(angle - 3 * M_PI / 2) < 0.001) {
					end = pixel + Point2D(0, radius_y);
				} else {

					/// Intersect the spoke with the ellipse, then put the signs
					/// back on according to which quadrant the angle lands in.
					double slope = fastmath::tan(angle);
					double radius_y_sq = radius_y * radius_y;
					double radius_x_sq = radius_x * radius_x;
					double xdist = fastmath::sqrt(1.0 / (slope * slope / radius_y_sq + 1.0 / radius_x_sq));
					double ydist = fastmath::sqrt((1.0 - xdist * xdist / radius_x_sq) * radius_y_sq);
					double xoffset = angle > M_PI / 2 && angle < 3 * M_PI / 2 ? -xdist : xdist;

					if (angle < M_PI) {
						ydist = -ydist;
					}

					end = pixel + Point2D(xoffset, ydist);
				}

				LogicalSurface->Draw_Depth_Antialiased_Line(TacticalRect, pixel - TacticalRect.TopLeft, end - TacticalRect.TopLeft, color, -500, -500, false, true, true, true, _transparencies[i]);
			}
		}
	}
}





/// <summary>
/// Flashes the building to acknowledge that it was clicked on.
/// This routine is called by the sidebar and the tactical map when the player designates
/// this building as a target. The building refreshes its animation appearance as well, so
/// that any state tied to being the current target is picked up right away.
/// </summary>
/// <param name="count">The number of times the building should flash.</param>
void BuildingClass::Clicked_As_Target(int count)
{
	if (count) {
		IsToDisplay = true;
	}

	BASECLASS::Clicked_As_Target(count);

	Update_Anim_Appearance();
}


/// <summary>
/// Adjusts the brightness that this building is drawn with.
/// A building that is flashing has its lighting pushed away from the norm, so that the
/// flash reads against both bright and dark surroundings.
/// </summary>
/// <param name="brightness">The brightness the building would otherwise be drawn with.</param>
/// <returns>Returns with the brightness to actually draw the building with.</returns>
int BuildingClass::Apparent_Brightness(int brightness) const
{
	if ((FlashCount & 2) == 2) {
		return(brightness > 1500 ? brightness - 500 : brightness + 500);
	}
	return(brightness);
}





/// <summary>
/// The brightness to draw this building at: its apparent brightness, tinted while it is under an effect.
/// </summary>
int BuildingClass::Tinted_Brightness(int brightness) // RA2: not in TibSun, at 0x4521E0
{
	int value = Apparent_Brightness(brightness);
	if (Unknown_Virtual_32C()) {
		value = GetEffectTintIntensity(value);
	}
	return(value);
}


/// <summary>
/// Should this building show up on the radar map?
/// This routine is used by the radar drawing code. A building that is known only through
/// a sensor rather than by sight is still reported as visible, but the caller is told so
/// that it can be plotted differently.
/// </summary>
/// <param name="detected">Set to DETECTED_CLOAKED when the building is merely sensed.</param>
/// <returns>bool; Should the building be plotted on the radar?</returns>
bool BuildingClass::Is_Radar_Visible(DetectedType & detected) const
{
	if (!Class->IsInvisible) {
		if (Class->IsRadarVisible) {
			return(true);
		}
		if (House->Is_Player_Control()) {
			return(IsDiscoveredByPlayer ? true : false);
		}

		int height = Class->Height() * CELL_LEPTON_H - CELL_LEPTON;
		int width = Class->Width() * CELL_LEPTON_W - CELL_LEPTON;
		bool shrouded = Map.Is_Shrouded(PositionCoord) && Map.Is_Shrouded(PositionCoord + Coord(width, height)) && MainWindow;

		if (Cloak != CLOAKED && TranslucencyLevel != 15 && !IsFogged && !shrouded) {
			return(true);
		}
		if (!Is_Sensed_By_Player()) {
			return(false);
		}
		if (!PlayerPtr->Is_Ally(House) && !IsFogged && !shrouded) {
			detected = DETECTED_CLOAKED;
		}
		return(true);
	}
	return(false);
}


/// <summary>
/// RA2
/// </summary>
static inline bool Is_Tech_Building(BuildingTypeClass const * type)
{
	int count = Rule->BuildTech.Count();
	for (int i = 0; i < count; i++) {
		if (Rule->BuildTech[i] == type) {
			return(true);
		}
	}
	return(false);
}


/// <summary>
/// Records that a spy has infiltrated this building.
/// This routine is called when a spy makes it inside. The spying house is handed whatever
/// intelligence this building type is worth -- radar coverage and power plant readings.
/// </summary>
/// <param name="house">The house that has gained the intelligence.</param>
void BuildingClass::Spied_By(HouseClass * house)
{
	bool visible = false; // RA2: whether the spying is announced (a radar event was submitted for this cell)
	if (House->Is_Player_Control() || house->Is_Player_Control()) { // RA2: as above
		if (Submit_Radar_Event(RadarEventType(9), Get_Cell())) { // RA2: as above
			visible = true; // RA2: as above
		}
	}
	BuildingTypeClass const * type = Class; // RA2: as above
	if (type->IsRadar) { // RA2: a spied radar blinds its owner's satellite
		House->Spy_Satellite_Lost(); // RA2: as above
		if (House->Is_Player_Control() && !House->SpySatActive && visible) { // RA2: as above
			Queue_Speech("EVA_RadarSabotaged", -1, -1); // RA2: as above
		}
		if (house->Is_Player_Control() && !House->SpySatActive && visible) { // RA2: as above
			Queue_Speech("EVA_BuildingInfRadarSabotaged", -1, -1); // RA2: as above
		}
	} else if (type->Power > 0) { // RA2: a spied power plant causes a blackout
		House->Begin_Power_Blackout(Rule->SpyPowerBlackout); // RA2: as above
		if (House->Is_Player_Control() && visible) { // RA2: as above
			Queue_Speech("EVA_PowerSabotaged", -1, -1); // RA2: as above
		} else if (house->Is_Player_Control() && visible) { // RA2: as above
			Queue_Speech("EVA_BuildingInfiltrated", -1, -1); // RA2: as above
			Queue_Speech("EVA_EnemyBasePoweredDown", -1, -1); // RA2: as above
		}
	} else if (type->SuperWeapon != -1) { // RA2: a spied superweapon building loses its charge
		House->SuperWeapon[type->SuperWeapon]->Reset(); // RA2: as above
		if (House->Is_Player_Control() || (house->Is_Player_Control() && visible)) { // RA2: as above
			Queue_Speech("EVA_BuildingInfiltrated", -1, -1); // RA2: as above
		}
	} else {
		for (int index = 0; index < Rule->BuildTech.Count(); index++) { // RA2: as above
			if (Rule->BuildTech[index] == type) { // RA2: a tech building gives the spy's house the other side's technology
				if (House->ActLike == 0) { // RA2: as above
					house->Side0TechInfiltrated = true; // RA2: as above
				} else {
					house->Side1TechInfiltrated = true; // RA2: as above
				}
				house->IsRecalcNeeded = true; // RA2: as above
				if (House->Is_Player_Control() && visible) { // RA2: as above
					Queue_Speech("EVA_TechnologyStolen", -1, -1); // RA2: as above
				}
				if (house->Is_Player_Control() && visible) { // RA2: as above
					Queue_Speech("EVA_BuildingInfiltrated", -1, -1); // RA2: as above
					Queue_Speech("EVA_NewTechnologyAcquired", -1, -1); // RA2: as above
				}
				goto finished; // RA2: the tests below are skipped
			}
		}
		if (type->Capacity > 0) { // RA2: a spied refinery or silo has its owner's money stolen
			int stolen = (int)(House->Available_Money() * Rule->SpyMoneyStealPercent); // RA2: as above
			House->Spend_Money(stolen); // RA2: as above
			house->Refund_Money(stolen); // RA2: as above
			if (House->Is_Player_Control() && visible) { // RA2: as above
				Queue_Speech("EVA_CashStolen", -1, -1); // RA2: as above
			}
			if (house->Is_Player_Control() && visible) { // RA2: as above
				Queue_Speech("EVA_BuildingInfCashStolen", -1, -1); // RA2: as above
			}
		} else if (type->ToBuild == RTTI_UNITTYPE) { // RA2: 0x28, a war factory
			house->WarFactoryInfiltrated = true; // RA2: as above
			house->IsRecalcNeeded = true; // RA2: as above
			if (house->Is_Player_Control()) { // RA2: as above
				IsCameoRefreshPending = true; // RA2: as above
			}
			if (House->Is_Player_Control() && visible) { // RA2: as above
				Queue_Speech("EVA_TechnologyStolen", -1, -1); // RA2: as above
			}
			if (house->Is_Player_Control() && visible) { // RA2: as above
				Queue_Speech("EVA_BuildingInfiltrated", -1, -1); // RA2: as above
				Queue_Speech("EVA_NewTechnologyAcquired", -1, -1); // RA2: as above
			}
		} else if (type->ToBuild == RTTI_INFANTRYTYPE) { // RA2: 0x10, a barracks
			house->BarracksInfiltrated = true; // RA2: as above
			house->IsRecalcNeeded = true; // RA2: as above
			if (house->Is_Player_Control()) { // RA2: as above
				IsCameoRefreshPending = true; // RA2: as above
			}
			if (House->Is_Player_Control() && visible) { // RA2: as above
				Queue_Speech("EVA_TechnologyStolen", -1, -1); // RA2: as above
			}
			if (house->Is_Player_Control() && visible) { // RA2: as above
				Queue_Speech("EVA_BuildingInfiltrated", -1, -1); // RA2: as above
				Queue_Speech("EVA_NewTechnologyAcquired", -1, -1); // RA2: as above
			}
		}
	}
finished: // RA2: the exit of the tech building case
	Mark(MARK_CHANGE);
}






/// <summary>
/// Sells this building along with everything plugged into it.
/// Every upgrade is refunded and removed before the building itself is sold back, so the
/// owner is not shortchanged for the plugs he paid for.
/// </summary>
void BuildingClass::Sell_All_Upgrades(void)
{
	char level = UpgradeLevel;
	while (level > 0) {
		int cost = Upgrades[level - 1]->GetRefund(House, true); // RA2: the refund of the upgrade, not its cost
		House->Refund_Money(cost);
		Remove_Upgrade();
		House->RecalcPower = true;
		House->RecalcRadar = true;
		Adjust_House_Power(House);
		level = UpgradeLevel;
	}
	Sell_Back(true);
}


/// <summary>
/// Is this building really a vehicle that happens to be deployed?
/// A building that can undeploy back into a unit is counted as a vehicle rather than as a
/// structure. The construction yard is the deliberate exception to this.
/// </summary>
/// <returns>bool; Should this building be treated as a vehicle?</returns>
bool BuildingClass::Considered_Vehicle(void) const
{
	if (Class->UndeploysInto == NULL) {
		return(false);
	}
	if (Class->IsConstructionYard) {
		return(false);
	}
	return(true);
}


/// <summary>
/// Fetches the super weapon that this building grants.
/// A super weapon that names an auxiliary building is only granted while the owner
/// actually has one of those buildings standing.
/// </summary>
/// <returns>Returns with the super weapon granted, or SUPER_NONE if there is none.</returns>
SuperWeaponType BuildingClass::Fetch_Super_Weapon(void) const
{
	if (Class->SuperWeapon != SUPER_NONE) {
		BuildingTypeClass const * aux = SuperWeapons[Class->SuperWeapon]->Class->AuxBuilding;
		if (aux != NULL && House->ABQuantity.Value(aux->HeapID) == 0) {
			return(SUPER_NONE);
		}
	}
	return(Class->SuperWeapon);
}


/// <summary>
/// Fetches the second super weapon that this building grants.
/// A super weapon that names an auxiliary building is only granted while the owner
/// actually has one of those buildings standing.
/// </summary>
/// <returns>Returns with the super weapon granted, or SUPER_NONE if there is none.</returns>
SuperWeaponType BuildingClass::Fetch_Super_Weapon2(void) const
{
	if (Class->SuperWeapon2 != SUPER_NONE) {
		BuildingTypeClass const * aux = SuperWeapons[Class->SuperWeapon2]->Class->AuxBuilding;
		if (aux != NULL && House->ABQuantity.Value(aux->HeapID) == 0) {
			return(SUPER_NONE);
		}
	}
	return(Class->SuperWeapon2);
}


/// <summary>
/// Scatters any infantry that were heading into this building.
/// This routine is used when the building can no longer take the infantry that were on
/// their way to it, so that they do not gather on its doorstep.
/// </summary>
void BuildingClass::Scatter_Incoming_Infantry(void) const
{
	for (int i = 0; i < Infantry.Count(); i++) {
		InfantryClass * infantry = Infantry[i];
		BuildingClass * building = Map[infantry->Destination_Coord()].Cell_Building();
		if (infantry->IsActive && building != NULL && building == this) {
			if (infantry->NavCom == NULL || infantry->NavCom == building) {
				infantry->Scatter(COORD_NONE, true, true);
			}
		}
	}
}














/// <summary>
/// Can this building cloak itself at this time?
/// A building will not bother cloaking while an enemy detector is standing alongside it,
/// since it would only be revealed again straight away.
/// </summary>
/// <returns>bool; May the building cloak now?</returns>
bool BuildingClass::Is_Ready_To_Cloak(void) const
{
	if (BASECLASS::Is_Ready_To_Cloak()) {
		Cell cell = PositionCell;
		int width = Class->Width();
		int height = Class->Height();
		for (int x = -1; x <= width; x++) {
			for (int y = -1; y <= height; y++) {
				TechnoClass * tech = Map[cell + Cell(x, y)].Cell_Techno();
				if (tech == (TechnoClass *)this) {
					tech = Map[cell + Cell(x, y)].Cell_Infantry();
				}
				if (tech != NULL && tech->House != House) {
					if (tech->TClass->IsScanner) {
						return(false);
					}
				}
			}
		}
		return(true);
	}
	return(false);
}


/// <summary>
/// Should this cloaked building reveal itself?
/// On top of the usual reasons a cloaked object decloaks, a building is given away by an
/// enemy detector standing alongside it.
/// </summary>
/// <returns>bool; Should the building drop its cloak?</returns>
bool BuildingClass::Should_Uncloak(void) const
{
	if (!BASECLASS::Should_Uncloak()) {
		Cell cell = PositionCell;
		int width = Class->Width();
		int height = Class->Height();
		for (int x = -1; x <= width; x++) {
			for (int y = -1; y <= height; y++) {
				TechnoClass * tech = Map[cell + Cell(x, y)].Cell_Techno();
				if (tech == (TechnoClass *)this) {
					tech = Map[cell + Cell(x, y)].Cell_Infantry();
				}
				if (tech != NULL && tech->House != House) {
					if (tech->TClass->IsScanner) {
						return(true);
					}
				}
			}
		}
		return(false);
	}
	return(true);
}


/// <summary>
/// Should this building be handed over to the fog of war?
/// A building only fogs once every cell it occupies has been fogged. One that is still
/// partly in view is left alone.
/// </summary>
/// <returns>bool; Is the building completely fogged over?</returns>
bool BuildingClass::Should_Fog(void) const
{
	Cell cell = PositionCell;
	Cell const * occupy = Class->Occupy_List();
	while (*occupy != REFRESH_EOL) {
		if (!Map[*occupy + cell].IsFogged) {
			return(false);
		}
		occupy++;
	}
	return(true);
}


/// <summary>
/// Hands this building over to the fog of war.
/// A fogged stand-in is created and attached to every cell of the building's footprint so
/// that the shrouded map still shows the building after the real object has passed out of
/// view. The building is deselected as part of the process.
/// </summary>
/// <param name="fogged_objects">The fogged object list the caller is building up for the
/// cell below. May be NULL.</param>
/// <param name="cellptr">The cell whose list is supplied above. May be NULL.</param>
/// <param name="fade">Should the fogged stand-in be drawn?</param>
void BuildingClass::Make_Fogged(DynamicVectorClass<FoggedObjectClass *> * fogged_objects, CellClass * cellptr, bool fade)
{
	Cell const * occupy = Class->Occupy_List();

	Unselect();
	FoggedObjectClass * fogged_object = new FoggedObjectClass(this, fade);

	Cell cell = PositionCell;
	while (*occupy != REFRESH_EOL) {
		CellClass * cptr = &Map[cell + *occupy];
		if (cellptr != NULL && cptr == cellptr) {
			fogged_objects->Add(fogged_object);
		} else {
			DynamicVectorClass<FoggedObjectClass *> * new_fogged_objects = cptr->FoggedObjects;
			if (new_fogged_objects != NULL) {
				new_fogged_objects->Add(fogged_object);
			} else {
				cptr->FoggedObjects = new DynamicVectorClass<FoggedObjectClass *>;
				cptr->FoggedObjects->Resize(1);
				cptr->FoggedObjects->Set_Growth_Step(1);
				cptr->FoggedObjects->Add(fogged_object);
			}
		}
		occupy++;
	}
}


/// <summary>
/// Puts this building under the iron curtain; a building about to blow up from planted explosives is defused.
/// </summary>
void BuildingClass::IronCurtain(int duration) // RA2: not in TibSun, at 0x452E60 (vtable slot 82)
{
	if (IsGoingToBlow) {
		IsGoingToBlow = false;
		CountDown = 0;
		WhomToRepay = NULL;
	}
	TechnoClass::IronCurtain(duration); // RA2: TechnoClass's own (0x6D86B0) is not declared yet
}


/// <summary>
/// Can the infantry garrison (an occupier) or clear (an assaulter) this building?
/// </summary>
bool BuildingClass::CanBeOccupiedBy(InfantryClass * infantry) const // RA2: not in TibSun, at 0x452EB0
{
	if (!Class->CanBeOccupied) { // RA2: as above
		return(false); // RA2: as above
	}
	if (infantry->Class->Occupier // RA2: as above
	    ? ((House == infantry->House || House->Class->IsMultiplayPassive) // RA2: as above
	       && GetOccupantCount() != Class->MaxNumberOccupants // RA2: as above
	       && !(char)Is_Red_Health() && !infantry->MindControlledBy) // RA2: as above
	    : (infantry->Class->Assaulter && !House->Is_Ally(infantry) && GetOccupantCount())) { // RA2: as above
		return(true); // RA2: as above
	}
	return(false);
}




/// <summary>
/// Puts the occupants out next to the building, on the first free cell along its edges; with none free they die.
/// </summary>
void BuildingClass::Eject_Occupants(bool hunt) // RA2: not in TibSun, at 0x452F50
{
	if (Occupants.Count() == 0) return;

	Coord destination = COORD_NONE; // RA2: as above
	Cell selected;
	memcpy(&selected, &CELL_NONE, sizeof(selected)); // RA2: as above
	int width = Class->Width();
	int height = Class->Height(false);
	bool found = false;
	Cell probe = Get_Cell();
	probe.X = (short)((unsigned int)(int)probe.X + (unsigned int)width); // RA2: as above
	probe.Y = (short)((unsigned int)(int)probe.Y + (unsigned int)height); // RA2: as above
	InfantryClass *occupant = Occupants[0]; // RA2: as above
	int entry_result = occupant->Can_Enter_Cell(&Map[probe], FACING_NONE, -1, NULL, true); // RA2: as above
	while (entry_result != 0) { // RA2: as above
		if ((int)probe.Y <= (int)Get_Cell().Y - 1) goto after_first_edge; // RA2: as above
		probe.Y = (short)(probe.Y - 1); // RA2: as above
		entry_result = occupant->Can_Enter_Cell(&Map[probe], FACING_NONE, -1, NULL, true); // RA2: as above
	}
	found = true; // RA2: as above
	selected = probe; // RA2: as above
after_first_edge: // RA2: as above
	probe = Get_Cell();
	probe.X = (short)((unsigned int)(int)probe.X + (unsigned int)width); // RA2: as above
	probe.Y = (short)((unsigned int)(int)probe.Y + (unsigned int)height); // RA2: as above
	while (!found && (int)probe.X > (int)Get_Cell().X - 1) { // RA2: as above
		probe.X = (short)(probe.X - 1); // RA2: as above
		if (occupant->Can_Enter_Cell(&Map[probe], FACING_NONE, -1, NULL, true) == 0) { // RA2: as above
			found = true;
			selected = probe;
		}
	}

	probe = Get_Cell();
	probe.X = (short)(probe.X - 1); // RA2: as above
	probe.Y = (short)(probe.Y - 1); // RA2: as above
	while (!found && (int)probe.X < (int)((unsigned int)(int)Get_Cell().X + (unsigned int)width)) { // RA2: as above
		probe.X = (short)(probe.X + 1); // RA2: as above
		if (occupant->Can_Enter_Cell(&Map[probe], FACING_NONE, -1, NULL, true) == 0) { // RA2: as above
			found = true;
			selected = probe;
		}
	}

	probe = Get_Cell();
	probe.X = (short)(probe.X - 1); // RA2: as above
	probe.Y = (short)(probe.Y - 1); // RA2: as above
	while (!found && (int)probe.Y < (int)((unsigned int)(int)Get_Cell().Y + (unsigned int)height)) { // RA2: as above
		probe.Y = (short)(probe.Y + 1); // RA2: as above
		if (occupant->Can_Enter_Cell(&Map[probe], FACING_NONE, -1, NULL, true) == 0) { // RA2: as above
			selected = probe;
			found = true;
		}
	}
	if (!found) {
		KillOccupants(NULL);
		return;
	}

	destination = Map[selected].Center_Coord();
	ScenarioInit = (int)((unsigned int)ScenarioInit + 1u); // RA2: as above
	for (int index = (int)((unsigned int)Occupants.Count() - 1u); index >= 0; --index) { // RA2: as above
		InfantryClass *member = Occupants[index]; // RA2: as above
		if (member->Unlimbo(destination, DIR_N)) {
			member->Assign_Target(NULL);
			member->Scatter(Center_Coord(), true, true);
			if (hunt) {
				TeamClass *team = member->Team; // RA2: as above
				if (team) team->Remove(member, -1, 0); // RA2: as above
				member->QueueMission(MISSION_HUNT, false);
			}
		} else {
			member->Delete_Me();
		}
	}
	ScenarioInit = (int)((unsigned int)ScenarioInit - 1u); // RA2: as above
	DynamicVectorClass<InfantryClass *> &occupants = Occupants; // RA2: as above
	int capacity = occupants.Length(); // RA2: as above
	occupants.Clear(); // RA2: as above
	occupants.Resize(capacity, 0); // RA2: as above
	UpdateThreatInCell(Get_Cell_Ptr());
}




/// <summary>
/// The number of infantry garrisoning this building.
/// </summary>
int BuildingClass::GetOccupantCount(void) const // RA2: not in TibSun, at 0x4532F0
{
	return(Occupants.Count());
}


/// <summary>
/// Hands a garrisonable civilian building to whoever holds it: abandoned, it goes back to the civilians; garrisoned
/// while civilian, it goes to the house of its first occupant.
/// </summary>
void BuildingClass::Update_Garrison_Owner(void) // RA2: not in TibSun, at 0x453300
{
	if ((char)Is_Red_Health()) { // RA2: as above
		Eject_Occupants(0); // RA2: as above
	}
	int count = Houses.Count();
	HouseClass * civilian = NULL;
	int side = SideClass::From_Name("Civilian"); // RA2: as above
	if (count > 0) { // RA2: as above
		HouseClass ** cursor = &Houses[0]; // RA2: as above
		do { // RA2: as above
			HouseClass * house = *cursor; // RA2: as above
			if (house->Class->Side == side) { // RA2: as above
				civilian = house; // RA2: as above
			}
			++cursor; // RA2: as above
		} while (--count); // RA2: as above
	}
	if (Occupants.Count() == 0) { // RA2: as above
		if (House != civilian) { // RA2: as above
			if (House->Is_Player_Control()) { // RA2: as above
				VocClass::PlayGlobal(Rule->BuildingAbandonedSound, 1.0f, 0x2000, 0); // RA2: as above
				if (Submit_Radar_Event(RadarEventType(15), Get_Cell())) { // RA2: as above
					Queue_Speech("EVA_StructureAbandoned", -1, -1); // RA2: as above
				}
			}
			Refresh_State_Animations(); // RA2: as above
			Captured(civilian, 0); // RA2: as above
		}
	}
	if (Occupants.Count() > 0 && House == civilian) { // RA2: as above
		Refresh_State_Animations();
		Captured(Occupants[0]->House, 0); // RA2: as above
		if (Occupants[0]->House->Is_Player_Control()) { // RA2: as above
			Queue_Speech("EVA_StructureGarrisoned", -1, -1);
			VocClass::PlayAt(Rule->BuildingGarrisonedSound, Center_Coord(), 0); // RA2: as above
		}
	}
}



/// <summary>
/// Restarts the idle and active animations with the names for the building's state: damaged, garrisoned or normal.
/// </summary>
void BuildingClass::Refresh_State_Animations(void) // RA2: not in TibSun, at 0x453450
{
	if (Anims[BANIM_IDLE] != NULL) { // RA2: the idle animation and the four active ones are restarted if running
		Begin_Anim(BANIM_IDLE, HealthRatio <= Rule->ConditionYellow, Occupants.Count() > 0, 0); // RA2: as above
	}
	if (Anims[BANIM_ACTIVE_ONE] != NULL) {
		Begin_Anim(BANIM_ACTIVE_ONE, HealthRatio <= Rule->ConditionYellow, Occupants.Count() > 0, 0);
	}
	if (Anims[BANIM_ACTIVE_TWO] != NULL) {
		Begin_Anim(BANIM_ACTIVE_TWO, HealthRatio <= Rule->ConditionYellow, Occupants.Count() > 0, 0);
	}
	if (Anims[BANIM_ACTIVE_THREE] != NULL) {
		Begin_Anim(BANIM_ACTIVE_THREE, HealthRatio <= Rule->ConditionYellow, Occupants.Count() > 0, 0);
	}
	if (Anims[BANIM_ACTIVE_FOUR] != NULL) {
		Begin_Anim(BANIM_ACTIVE_FOUR, HealthRatio <= Rule->ConditionYellow, Occupants.Count() > 0, 0);
	}
}


/// <summary>
/// Kills the occupants: quietly, or by the killer's assault, each with its weapon's assault animation at its window.
/// </summary>
void BuildingClass::KillOccupants(TechnoClass * killer) // RA2: not in TibSun, at 0x4536D0
{
	if (killer == NULL) { // RA2: as above
		for (int index = Occupants.Count() - 1; index >= 0; index--) { // RA2: as above
			Occupants[index]->Delete_Me(); // RA2: as above
		}
	} else {
		for (int index = Occupants.Count() - 1; index >= 0; index--) { // RA2: as above
			Coord coord; // RA2: as above
			Occupant_Offset_To_Coord(coord, TacticalMap->Pixel_To_Lepton(Class->MuzzleFlash[index])); // RA2: the offset helper
			coord = coord + Render_Coord(); // RA2: as above
			AnimClass * anim = new AnimClass(killer->Get_Class_Weapon_Data(0)->Weapon->AssaultAnim, coord, 0, 1, ShapeFlags_Type(0x600), 0); // RA2: as above
			anim->ZAdjust = -200; // RA2: as above
			killer->Record_The_Kill(Occupants[index]); // RA2: as above
			Occupants[index]->Delete_Me(); // RA2: as above
		}
	}
	int capacity = Occupants.Length(); // RA2: as above
	Occupants.Clear(); // RA2: as above
	Occupants.Resize(capacity, 0); // RA2: as above
	UpdateThreatInCell(Get_Cell_Ptr()); // RA2: as above
}





/// <summary>
/// Restarts the cash timer and pays (or charges) the house the type's ProduceCashAmount while the building is powered.
/// </summary>
void BuildingClass::Produce_Cash(void) // RA2: not in TibSun, at 0x453850
{
	CashProductionTimer = Class->ProduceCashDelay;
	if (!House->Class->IsMultiplayPassive && IsPowerOnline()) {
		int amount = Class->ProduceCashAmount;
		if (amount > 0) {
			House->Refund_Money(amount);
		} else {
			House->Spend_Money(-amount);
		}
	}
}


/// <summary>
/// Makes one of the type for this building's house and lets it out; did it get out?
/// </summary>
bool BuildingClass::Exit_New_Object(ObjectTypeClass const * type) // RA2: not in TibSun, at 0x4538D0
{
	TechnoClass * object = (TechnoClass *)type->Create_One_Of(House); // RA2: the new object in a local
	return(Exit_Object(object) != 0 ? true : false); // RA2: as above
}



























/// <summary>
/// Builds the transformation matrix for this building's voxel barrel.
/// This routine is used by the voxel draw code to place the barrel. The pivot offsets and
/// the scale come from the building type, while the rotation and pitch track the turret
/// as it aims.
/// </summary>
/// <returns>Returns with the matrix that positions the barrel on the building.</returns>
Matrix3D BuildingClass::Get_Barrel_Matrix(void) const
{
	Matrix3D matrix;
	matrix.Make_Identity();

	matrix.Translate_X(Class->VoxelBarrelOffsetToBuildingPivotPoint.X);
	matrix.Translate_Y(Class->VoxelBarrelOffsetToBuildingPivotPoint.Y);
	matrix.Translate_Z(Class->VoxelBarrelOffsetToBuildingPivotPoint.Z);

	matrix.Rotate_Z(PrimaryFacing.Current().As_Radian32());

	matrix.Translate_X(Class->VoxelBarrelOffsetToRotatePivotPoint.X);
	matrix.Translate_Y(Class->VoxelBarrelOffsetToRotatePivotPoint.Y);
	matrix.Translate_Z(Class->VoxelBarrelOffsetToRotatePivotPoint.Z);

	matrix.Rotate_Y(-(BarrelPitch.Current().As_Radian32()));

	matrix.Translate_X(Class->VoxelBarrelOffsetToPitchPivotPoint.X);
	matrix.Translate_Y(Class->VoxelBarrelOffsetToPitchPivotPoint.Y);
	matrix.Translate_Z(Class->VoxelBarrelOffsetToPitchPivotPoint.Z);

	float scale = Class->VoxelBarrelScale;
	matrix.Scale_X(scale);
	matrix.Scale_Y(scale);
	matrix.Scale_Z(scale);

	return(matrix);
}


/// <summary>
/// How full this building's storage is, in quarters of its capacity.
/// </summary>
int BuildingClass::Storage_Level(void) const // RA2: not in TibSun, at 0x453AB0
{
	if ((int)Storage.Get_Total_Amount() == 0) {
		return(0);
	}
	return((int)Storage.Get_Total_Amount() * 4 / Class->Capacity);
}


/// <summary>
/// Is the cell one of this building's impassable rows (all of its cells when the type gives no row count)?
/// </summary>
bool BuildingClass::Is_Impassable_Cell(CellClass const * cell) // RA2: not in TibSun, at 0x453AF0
{
	if (cell->Cell_Building() != this) {
		return(false);
	}
	if (Class->NumberImpassableRows == -1) {
		return(true);
	}
	int x = cell->CellID.X;
	int cx = Get_Cell().X;
	int rows = Class->NumberImpassableRows;
	return(rows + cx > x ? true : false);
}




















/// <summary>
/// RA2
/// </summary>
static inline DirType Direction_From(AbstractClass const * object, Coord const & coord)
{
	return(::Direction(object->Center_Coord(), coord));
}













































/// <summary>
/// RA2
/// </summary>
static inline bool _Exit_Free(Cell const & cell)
{
	return(Map.In_Radar(cell) && Map[cell].Cell_Occupier() == NULL && Map[cell].Is_Clear_To_Move(SPEED_FOOT, false, false));
}
































/// <summary>
/// Fetches the number of bytes this building occupies in a save file.
/// This routine is used by the load and save code so that save games written before the
/// class last grew are still read back at the size they were stored with.
/// </summary>
/// <param name="oldsave">Is the size wanted for an older save file layout?</param>
/// <returns>Returns with the size, in bytes, of this object as it is stored.</returns>
int BuildingClass::Fetch_Object_Size(void) const // RA2: no argument
{
	return(sizeof(*this)); // RA2: no older save layout
}


/// <summary>
/// Fetches the persistent class identifier for this building.
/// This routine is part of the persistence support. The save code writes this identifier
/// ahead of the object so that the loader knows what kind of object to create.
/// </summary>
/// <param name="retval">Pointer to the identifier to fill in.</param>
/// <returns>Returns with S_OK, or E_POINTER if no destination was supplied.</returns>
HRESULT STDMETHODCALLTYPE BuildingClass::GetClassID(CLSID * retval)
{
	if (retval == NULL) return(E_POINTER);
	*retval = CLSID_BuildingClass;
	return(S_OK);
}


/// <summary>
/// Determines what kind of object this is.
/// </summary>
/// <returns>Returns with RTTI_BUILDING.</returns>
RTTIType BuildingClass::Fetch_RTTI(void) const
{
	return(RTTI_BUILDING);
}


/// <summary>
/// Fetches the displayable name of this building.
/// </summary>
/// <returns>Returns with a pointer to the human readable name of this building.</returns>
wchar_t const * BuildingClass::Full_Name(void) const // RA2: wide
{
	return(Class->UIName); // RA2
}


/// <summary>
/// Fetches the type class this building was created from.
/// </summary>
/// <returns>Returns with a pointer to the building type class that holds this building's
/// static data.</returns>
ObjectTypeClass const * BuildingClass::Class_Of(void) const
{
	return(Class);
}


/// <summary>
/// Fetches the coordinate that this building is rendered about.
/// The drawing code works from the upper left corner of the building's footprint rather
/// than from the center of its cell, so the position is shifted back by half a cell.
/// </summary>
/// <returns>Returns with the coordinate to render this building at.</returns>
Coord BuildingClass::Render_Coord(void) const
{
	return(PositionCoord - Coord(CELL_LEPTON/2, CELL_LEPTON/2, 0)); // RA2: Render_Coord at 0x453C70 matches only so
}





#ifdef _DEBUG
/***********************************************************************************************
 * BuildingClass::Debug_Dump -- Displays building status to the monochrome screen.             *
 *                                                                                             *
 *    This utility function will output the current status of the building class to the        *
 *    monochrome screen. It is through this data that bugs may be fixed or detected.           *
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
void BuildingClass::Debug_Dump(MonoClass * mono) const
{
	mono->Set_Cursor(0, 0);
	mono->Fill_Attrib(66, 13, 12, 1, IsRepairing ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(66, 14, 12, 1, IsToRebuild ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(66, 15, 12, 1, IsAllowedToSell ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(66, 16, 12, 1, IsCharging ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(66, 17, 12, 1, IsCharged ? MonoClass::INVERSE : MonoClass::NORMAL);

	mono->Set_Cursor(1, 11);
	if (Factory != NULL) {
		mono->Printf("%s %d%%", Factory->Get_Object()->Class_Of()->IniName, (100*Factory->Completion())/FactoryClass::STEP_COUNT);
	}

	BASECLASS::Debug_Dump(mono);
}
#endif







/***********************************************************************************************
 * BuildingClass::Charging_AI -- Handles the special charging logic for Tesla coils.           *
 *                                                                                             *
 *    This handles the special logic required of the charging tesla coil. It requires special  *
 *    processing since its charge up is dependant upon the target and power surplus of the     *
 *    owning house.                                                                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
void BuildingClass::Charging_AI(void)
{
	if (PrimaryWeapon != NULL && PrimaryWeapon->IsElectric && BState != BSTATE_CONSTRUCTION) {
		if (TarCom != NULL && House->Power_Fraction() >= 1 && IsOn) {
			if (!IsCharged) {
				if (IsCharging) {
					BuildingStage.Graphic_Logic();
//					if (stagechange) {
						if (BuildingStage.Fetch_Stage() >= (Anims[BANIM_TURRET] != NULL ? Anims[BANIM_TURRET]->Class->LoopEnd : 12)) {
							IsCharged = true;
							IsCharging = false;
							Set_Rate(0);
							BuildingStage.Set_Rate(0);
						}
//					}
				} else if (!Arm) {
					if (Can_Fire(TarCom, 0) < FIRE_ILLEGAL || Can_Fire(TarCom, 1) < FIRE_ILLEGAL) {
						Charge_Turret();
						Sound_Effect(Rule->TeslaCharge, PositionCoord); // RA2: no TurretChargeAnimRate (not in RA2's layout)
					}
					return;
				}
			}
		} else {
			if (IsCharging || IsCharged) {
				Discharge_Turret();
			}
		}
	}
}
#endif


/// <summary>
/// Fetches the extra damage this building deals when it is destroyed.
/// Any tiberium sitting in the building's storage adds to the blast, which is why a full
/// refinery makes such a spectacular wreck.
/// </summary>
/// <returns>Returns with the collateral damage to inflict on the surroundings.</returns>
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
int BuildingClass::Get_Collateral_Damage(void) const
{
	int damage = 0;

	if (Storage.Get_Total_Amount() > 0) {
		for (int i = 0; i < Tiberiums.Count(); i++) {
			damage += Storage.Get_Amount(i) * Tiberiums[i]->Power;
		}
	}
	damage += BASECLASS::Get_Collateral_Damage();

	return(damage);
}
#endif


/// <summary>
/// Starts this building's turret charging up.
/// A building with a charge up weapon plays its turret animation while the charge builds,
/// which is what warns everyone nearby that the obelisk is about to fire. The request is
/// ignored if the turret is already charged.
/// </summary>
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
void BuildingClass::Charge_Turret(void)
{
	if (!IsCharged) {
		Begin_Anim(BANIM_TURRET, HealthRatio <= Rule->ConditionYellow);
		BuildingStage.Set_Stage(Anims[BANIM_TURRET]->Class->Start);
		Set_Turret_Frame();
		IsCharging = true;
	} // RA2: no IsTurretAnimExclusive (not in RA2's layout)
}
#endif


/// <summary>
/// Dumps whatever charge this building's turret was holding.
/// This routine is called once the charged weapon has fired, or when the charge up must
/// be abandoned. Buildings whose turret animation locks out the active animation have
/// that animation started up again here.
/// </summary>
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
void BuildingClass::Discharge_Turret(void)
{
	IsCharging = false;
	IsCharged = false;
	BuildingStage.Set_Stage(0);
	BuildingStage.Set_Rate(0);
} // RA2: no IsTurretAnimExclusive (not in RA2's layout)
#endif

