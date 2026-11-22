/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 * Copyright 2026 ra2-decomp contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/

/* $Header: /CounterStrike/TECHNO.CPP 5     3/17/97 1:28a Steve_tall $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : BASE.CPP                                                     *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : May 8, 1994                                                  *
 *                                                                                             *
 *                  Last Update : November 1, 1996 [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   TechnoClass::AI -- Handles AI processing for techno object.                               *
 *   TechnoClass::Anti_Air -- Determines the anti-aircraft strength of the object.             *
 *   TechnoClass::Anti_Armor -- Determines the anti-armor strength of the object.              *
 *   TechnoClass::Anti_Infantry -- Calculates the anti-infantry strength of this object.       *
 *   TechnoClass::Area_Modify -- Determine the area scan modifier for the cell.                *
 *   TechnoClass::Assign_Destination -- Assigns movement destination to the object.            *
 *   TechnoClass::Assign_Target -- Assigns the targeting computer with specified target.       *
 *   TechnoClass::Base_Is_Attacked -- Handle panic response to base being attacked.            *
 *   TechnoClass::Can_Fire -- Determines if this techno object can fire.                       *
 *   TechnoClass::Can_Player_Fire -- Determines if the player can give this object a fire order*
 *   TechnoClass::Can_Player_Move -- Determines if the object can move be moved by player.     *
 *   TechnoClass::Can_Repair -- Determines if the object can and should be repaired.           *
 *   TechnoClass::Can_Teleport_Here -- Checks cell to see if a valid teleport destination.     *
 *   TechnoClass::Captured -- Handles capturing this object.                                   *
 *   TechnoClass::Clicked_As_Target -- Sets the flash count for this techno object.            *
 *   TechnoClass::Cloaking_AI -- Perform the AI maintenance for a cloaking device.             *
 *   TechnoClass::Combat_Damage -- Fetch the amount of damage infliced by the specified weapon.*
 *   TechnoClass::Crew_Type -- Fetches the kind of crew this object contains.                  *
 *   TechnoClass::Debug_Dump -- Displays the base class data to the monochrome screen.         *
 *   TechnoClass::Desired_Load_Dir -- Fetches loading parameters for this object.              *
 *   TechnoClass::Detach -- Handles removal of target from tracking system.                    *
 *   TechnoClass::Do_Cloak -- Start the object into cloaking stage.                            *
 *   TechnoClass::Do_Shimmer -- Causes this object to shimmer if it is cloaked.                *
 *   TechnoClass::Do_Uncloak -- Cause the stealth tank to uncloak.                             *
 *   TechnoClass::Draw_It -- Draws the health bar (if necessary).                              *
 *   TechnoClass::Draw_Pips -- Draws the transport pips and other techno graphics.             *
 *   TechnoClass::Electric_Zap -- Fires electric zap at the target specified.                  *
 *   TechnoClass::Enter_Idle_Mode -- Object enters its default idle condition.                 *
 *   TechnoClass::Evaluate_Cell -- Determine the value and object of specified cell.           *
 *   TechnoClass::Evaluate_Just_Cell -- Evaluate a cell as a target by itself.                 *
 *   TechnoClass::Evaluate_Object -- Determines score value of specified object.               *
 *   TechnoClass::Exit_Object -- Causes specified object to leave this object.                 *
 *   TechnoClass::Find_Docking_Bay -- Searches for a close docking bay.                        *
 *   TechnoClass::Find_Exit_Cell -- Finds an appropriate exit cell for this object.            *
 *   TechnoClass::Fire_At -- Fires projectile at target specified.                             *
 *   TechnoClass::Fire_Coord -- Determine the coordinate where bullets appear.                 *
 *   TechnoClass::Fire_Direction -- Fetches the direction projectile fire will take.           *
 *   TechnoClass::Get_Ownable -- Fetches the ownable bits for this object.                     *
 *   TechnoClass::Greatest_Threat -- Determines best target given search criteria.             *
 *   TechnoClass::Hidden -- Returns the object back into the hidden state.                     *
 *   TechnoClass::How_Many_Survivors -- Determine the number of survivors to escape.           *
 *   TechnoClass::In_Range -- Determines if specified target is within weapon range.           *
 *   TechnoClass::In_Range -- Determines if specified target is within weapon range.           *
 *   TechnoClass::In_Range -- Determines if the specified coordinate is within range.          *
 *   TechnoClass::Is_Allowed_To_Recloak -- Can this object recloak?                            *
 *   TechnoClass::Is_Allowed_To_Retaliate -- Checks object to see if it can retaliate.         *
 *   TechnoClass::Is_In_Same_Zone -- Determine if specified cell is in same zone as object.    *
 *   TechnoClass::Is_Players_Army -- Determines if this object is part of the player's army.   *
 *   TechnoClass::Is_Ready_To_Cloak -- Determines if this object is ready to begin cloaking.   *
 *   TechnoClass::Is_Ready_To_Random_Animate -- Determines if the object should random animate.*
 *   TechnoClass::Is_Visible_On_Radar -- Is this object visible on player's radar screen?      *
 *   TechnoClass::Is_Weapon_Equipped -- Determines if this object has a combat weapon.         *
 *   TechnoClass::Kill_Cargo -- Destroys any cargo attached to this object.                    *
 *   TechnoClass::Look -- Performs a look around (map reveal) action.                          *
 *   TechnoClass::Mark -- Handles marking of techno objects.                                   *
 *   TechnoClass::Nearby_Location -- Radiates outward looking for clear cell nearby.           *
 *   TechnoClass::Owner -- Who is the owner of this object?                                    *
 *   TechnoClass::Per_Cell_Process -- Handles once-per-cell operations for techno type objects.*
 *   TechnoClass::Pip_Count -- Fetches the number of pips to display on this object.           *
 *   TechnoClass::Player_Assign_Mission -- Assigns a mission as result of player input.        *
 *   TechnoClass::Rearm_Delay -- Calculates the delay before firing can occur.                 *
 *   TechnoClass::Receive_Message -- Handles inbound message as appropriate.                   *
 *   TechnoClass::Record_The_Kill -- Records the death of this object.                         *
 *   TechnoClass::Refund_Amount -- Returns with the money to refund if this object is sold.    *
 *   TechnoClass::Remap_Table -- Fetches the appropriate remap table to use.                   *
 *   TechnoClass::Renovate -- Heal a building to maximum                                       *
 *   TechnoClass::Response_Attack -- Handles the voice response when given attack order.       *
 *   TechnoClass::Response_Move -- Handles the voice response to a movement request.           *
 *   TechnoClass::Response_Select -- Handles the voice response when selected.                 *
 *   TechnoClass::Revealed -- Handles revealing an object to the house specified.              *
 *   TechnoClass::Risk -- Fetches the risk associated with this object.                        *
 *   TechnoClass::Select -- Selects object and checks to see if can be selected.               *
 *   TechnoClass::Set_Mission -- Forced mission set (used by editor).                          *
 *   TechnoClass::Stun -- Prepares the object for removal from the game.                       *
 *   TechnoClass::Take_Damage -- Records damage assessed to this object.                       *
 *   TechnoClass::Target_Something_Nearby -- Handles finding and assigning a nearby target.    *
 *   TechnoClass::TechnoClass -- Constructor for techno type objects.                          *
 *   TechnoClass::Techno_Draw_Object -- General purpose draw object routine.                   *
 *   TechnoClass::Threat_Range -- Returns the range to scan based on threat control.           *
 *   TechnoClass::Tiberium_Load -- Fetches the current tiberium load percentage.               *
 *   TechnoClass::Time_To_Build -- Determines the time it would take to build this.            *
 *   TechnoClass::Unlimbo -- Performs unlimbo process for all techno type objects.             *
 *   TechnoClass::Value -- Fetches the target value for this object.                           *
 *   TechnoClass::Visual_Character -- Determine the visual character of the object.            *
 *   TechnoClass::Weapon_Range -- Determines the maximum range for the weapon.                 *
 *   TechnoClass::What_Action -- Determines action to perform if cell is clicked on.           *
 *   TechnoClass::What_Action -- Determines what action to perform if object is selected.      *
 *   TechnoClass::What_Weapon_Should_I_Use -- Determines what is the best weapon to use.       *
 *   TechnoTypeClass::Cost_Of -- Fetches the cost of this object type.                         *
 *   TechnoTypeClass::Get_Cameo_Data -- Fetches the cameo image for this object type.          *
 *   TechnoTypeClass::Get_Ownable -- Fetches the ownable bits for this object type.            *
 *   TechnoTypeClass::Is_Two_Shooter -- Determines if this object is a double shooter.         *
 *   TechnoTypeClass::Raw_Cost -- Fetches the raw (base) cost of the object.                   *
 *   TechnoTypeClass::Read_INI -- Reads the techno type data from the INI database.            *
 *   TechnoTypeClass::Repair_Cost -- Fetches the cost to repair one step.                      *
 *   TechnoTypeClass::Repair_Step -- Fetches the health to repair one step.                    *
 *   TechnoTypeClass::TechnoTypeClass -- Constructor for techno type objects.                  *
 *   TechnoTypeClass::Time_To_Build -- Fetches the time to build this object.                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "always.h"
#include "rect.h" // RA2: the header statics, in the order of the unit's initializer table
#include "geometry.h"

#include "techno.h"
#include "bomb.h" // RA2: Draw_Post_Render reads the attached bomb
#include "capmgr.h" // RA2: Init makes the managers
#include "parasite.h" // RA2: as above
#include "planning.h" // RA2: the planning token
#include "planmgr.h" // RA2: ClickedEvent asks planning mode
#include "scheme.h" // RA2: GetRemapColour
#include "spawnmanager.h" // RA2: as above
#include "temporal.h" // RA2: as above

#include "_bench.h"
#include "_convert.h"
#include "_keyboar.h"
#include "_map.h"
#include "_rtti.h"
#include "_rules.h"
#include "_surface.h"
#include "_tactica.h"
#include "aircraft.h"
#include "airctype.h"
#include "anim.h"
#include "bench.h"
#include "blit.h"
#include "bound.h"
#include "bsurface.h"
#include "building.h"
#include "builtype.h"
#include "bullet.h"
#include "bullettype.h"
#include "ccrand.h"
#include "cell.h"
#include "combat.h"
#include "data.h"
#include "dbgprint.h"
#include "dialog.h"
#include "draw.h"
#include "dsurface.h"
#include "fog.h"
#include "globals.h"
#include "goptions.h"
#include "house.h"
#include "houstype.h"
#include "infantry.h"
#include "infatype.h"
#include "inline.h"
#include "ion.h"
#include "isotile.h"
#include "isotype.h"
#include "language\language.h"
#include "laser.h"
#include "lightcon.h"
#include "mono.h"
#include "overtype.h"
#include "partsys.h"
#include "psystype.h"
#include "queue.h"
#include "revent.h"
#include "rules.h"
#include "scheme.h"
#include "session.h"
#include "shapeset.h"
#include "stimer.h"
#include "sun.h"
#include "swizzle.h"
#include "tactical.h"
#include "tag.h"
#include "team.h"
#include "techtype.h"
#include "tracker.h"
#include "unit.h"
#include "unittype.h"
#include "vanim.h"
#include "vein.h"
#include "voc.h"
#include "vox.h"
#include "voxdrsys.h"
#include "warhead.h"
#include "wave.h"
#include "weapon.h"

#include "bench.hh"
#include "tube.hh"
#include "ftimer.h"
#include "trajhelp.h" // RA2
#include "ebolt.h" // RA2: Electric_Zap
#include "radbeam.h" // RA2: Rad_Zap, Rad_Eruption
#include "bomblist.h" // RA2: Limbo: BombList.Remove_Detector
#include "stringtable.h" // RA2: Draw_Text_Overlay: StringTable::LoadString
#include "dialog.h" // RA2: Draw_Text_Overlay: Simple_Text_Print
#include "dsurface.h" // RA2: Draw_Text_Overlay: Build_Hicolor_Pixel
#include "conquer.h"
static inline Rect Text_Rect(wchar_t const * text, Point2D loc, unsigned short flags, int marginx, int marginy) {Rect rect; return(*Drawing_GetTextDimensions(&rect, text, loc, flags, marginx, marginy));} // RA2: the box a text needs (Draw_Text_Overlay)
static inline bool Mouse_Hover_Flag(TechnoClass const * techno) {return(techno->IsMouseHovering);} // RA2: read accessor of the tail test in Draw_Post_Render (its site decides the temporaries)
static inline bool Techno_Is_Selected(TechnoClass const * techno) {return(techno->IsSelected);} // RA2: as above
static inline float Vector_Length(Vector3 const & v) {double y = v.Y; double z = v.Z; double x = v.X; return((float)fastmath::sqrt(x * x + z * z + y * y));} // RA2: the length of a vector, summed in doubles, x, z, y (Railgun_Beam_Damage)

namespace FileSystem { extern ConvertClass * PALETTE_PAL; }
namespace Unsorted { extern bool ArmageddonMode; }
namespace FileSystem { extern ShapeSet * PIPS_SHP; extern ConvertClass * PALETTE_PAL; }


CDTimerClass<FrameTimerClass> TechnoClass::ActionLineTimer;
bool TechnoClass::ActionLines = true;

TalkType TechnoClass::TalkBubbleType = TALK_NONE;
TechnoClass * TechnoClass::TalkBubbleOwner = NULL;
CDTimerClass<SystemTimerClass> TechnoClass::TalkBubbleTimer;


/***********************************************************************************************
 * TechnoClass::TechnoClass -- Default constructor for techno objects.                         *
 *                                                                                             *
 *    This default constructor for techno objects is used to reset all internal variables to   *
 *    their default state.                                                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/09/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
TechnoClass::TechnoClass(HouseClass * house) :
	BASECLASS(),
	Cargo(), // RA2: as above
	CurrentTurretNumber(-1), // RA2: no turret change yet
	LastTurretNumber(-1), // RA2: as above
	BehindAnim(NULL), // RA2: no animation behind the object
	CurrentWeaponNumber(0), // RA2: as above
	CurrentRanking(-1), // RA2: no ranking yet
	ActLike(HOUSE_NONE), // RA2: as above
	Veterancy(), // RA2: as above
	ArmorBias(1.0), // RA2: as above
	FirepowerBias(1.0), // RA2: as above
	IdleTimer(0),
	RadarFlashTimer(0), // RA2: as above
	TargetingTimer(45), // RA2: 45 frames until the first look for targets
	IronCurtainTimer(0), // RA2: the Iron Curtain, tint and disguise members are all cleared
	IronTintTimer(0), // RA2: as above
	Disguised(false), // RA2: as above
	DisguiseCreationFrame(0), // RA2: as above
	InfantryBlinkTimer(0), // RA2: as above
	DisguiseBlinkTimer(0), // RA2: as above
	UnlimboingInfantry(false), // RA2: as above
	ReloadTimer(0), // RA2: as above
	RadarPos(0,0), // RA2: as above
	SpiedBy(0),
	Group(-1), // RA2: as above
	ArchivedTarget(NULL),
	House(house),
	Cloak(UNCLOAKED),
	CloakingDevice(), // RA2: as above
	CloakDelay(0), // RA2: as above
	PredatorOffset(0), // RA2: as above
	IsSightRevealed(false), // RA2: as above
	LastSightCoords(COORD_NONE), // RA2: the sight reveal record starts empty
	LastSightRange(0), // RA2: as above
	LastSightHeight(0), // RA2: as above
	GapSuperCharged(false), // RA2: the gap generator, warp and temporal members are all cleared
	GeneratingGap(false), // RA2: as above
	GapRadius(0), // RA2: as above
	BeingWarpedOut(false), // RA2: as above
	WarpingOut(false), // RA2: as above
	Unused_206(false), // RA2: as above
	TemporalImUsing(NULL), // RA2: as above
	TemporalTargetingMe(NULL), // RA2: as above
	IsImmobilized(false), // RA2: as above
	TeleportState(0), // RA2: as above
	ChronoLockRemaining(0), // RA2: as above
	ChronoDestCoords(COORD_NONE), // RA2: as above
	TarCom(NULL),
	SuspendedTarCom(NULL),
	CaptureManager(NULL), // RA2: the manager pointers start empty
	MindControlledBy(NULL), // RA2: as above
	SpawnManager(NULL), // RA2: as above
	SpawnOwner(NULL), // RA2: as above
	PitchAngle(0), // RA2: as above
	Arm(0),
	ChargeTurretDelay(0), // RA2: cleared
	Ammo(-1),
	PurchasePrice(0),
	Wave(NULL),
	AngleRotatedSideways(0.0),
	AngleRotatedForwards(0.0),
	RockingSidewaysPerFrame(0.0),
	RockingForwardsPerFrame(0.0),
	EnteredByInfType(INFANTRY_NONE),
	Storage(),
	Door(),
	BarrelPitch(3),
	PrimaryFacing(), // RA2: as above
	SecondaryFacing(), // RA2: as above
	BurstIndex(0),
	TargetingLaserTimer(0), // RA2: as above
	SinkingYOffset(0),
	CountedAsOwned(false), // RA2: cleared
	IsSinking(false),
	WasSinkingAlready(false), // RA2: as above
	IsNeedingRescue(false),
	IsUseless(false), // RA2: as above
	IsTickedOff(false), // RA2: as above
	IsCloakable(false), // RA2: as above
	IsLeader(false), // RA2: as above
	IsALoaner(false), // RA2: as above
	IsLocked(false), // RA2: as above
	TurretRecoil(), // RA2: as above
	BarrelRecoil(), // RA2: as above
	IsTethered(false), // RA2: as above
	IsAlternativeTether(false), // RA2: cleared
	IsOwnedByPlayer(false), // RA2: as above
	IsDiscoveredByPlayer(false), // RA2: as above
	IsDiscoveredByComputer(false), // RA2: as above
	IsALemon(false), // RA2: as above
	Cooldown(0), // RA2: cleared
	Unused1(0), // RA2: as above
	SightIncrease(0),
	IsTeamRecruitable(true),
	IsAutocreateRecruitable(true),
	IsRadarTracked(false),
	IsInTransport(false),
	IsRocking(false),
	WasCrashingAlready(false), // RA2: the crash, hover and reselect flags and the old team start cleared
	IsOnPatrol(false),
	IsMouseHovering(false), // RA2: as above
	ShouldBeReselectOnUnlimbo(false), // RA2: as above
	OldTeam(NULL), // RA2: as above
	IsOnWaypointPatrol(false),
	RadialFireSegment(0), // RA2: the fan starts at its first segment
	CurrentTargetThreatValues(), // RA2: as above
	CurrentTargets(), // RA2: as above
	AttackedTargets(), // RA2: as above
	IsTurretRotateSoundPlaying(1), // RA2: the turret sound state starts at 1 / 0, as Load leaves it
	TurretIsRotating(0), // RA2: as above
	LastTargetScanFrame(Frame), // RA2: the frame of construction
	QueueUpToEnter(NULL), // RA2: as above
	StunDuration(0), // RA2: as above
	PlanningToken(NULL), // RA2: no planning token and no disguise yet
	Disguise(NULL), // RA2: as above
	DisguisedAsHouse(NULL) // RA2: as above
{
	if (house != NULL) {
		ActLike = house->ActLike;
	}

	TurretRotateSoundController.Init(); // RA2: the turret sound controller starts out unused
	Technos.Add(this);
	HousePtrTracker.Add(this);
	AbstractFlags |= 1; // RA2: marks the object as a techno (the flag the AbstractFlags & 1 tests read)

	for (int i = 0; i < ARRAY_SIZE(ParticleSystems); i++) {
		ParticleSystems[i] = NULL;
	}
	SoundRandomSeed = Scen->RandomNumber();
}


/// <summary>
/// Fetches the techno type class for this object.
/// Every ownable object's type derives from TechnoTypeClass, so this narrows the general type
/// pointer down to the one the combat and production logic works through.
/// </summary>
/// <returns>Returns with a pointer to this object's techno type class.</returns>
TechnoTypeClass const * TechnoClass::Techno_Type_Class(void) const
{
	return((TechnoTypeClass const *)Class_Of());
}


/// <summary>
/// Determines whether this object is allowed to scatter.
/// False while sleeping, sticky, or unloading, or if the type is a train.
/// </summary>
/// <returns>True if the object may scatter, false otherwise.</returns>
bool TechnoClass::Can_Scatter(void) const
{
	if (Mission != MISSION_SLEEP && Mission != MISSION_STICKY && Mission != MISSION_UNLOAD && !TClass->IsTrain) {
		return(true);
	}

	return(false);
}


/***************************************************************************
**	Which shape to use depending on which facing is controlled by these arrays.
*/
int const TechnoClass::BodyShape[FACING_COUNT * 4] = {28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,31,30,29};


/***********************************************************************************************
 * TechnoClass::Is_Players_Army -- Determines if this object is part of the player's army.     *
 *                                                                                             *
 *    The player's army is considered to be all those mobile units that can be selected        *
 *    and controlled by the player (they may or may not have weapons in the traditional        *
 *    sense).                                                                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Is this object part of the player's selectable controllable army?            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/23/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoClass::Is_Players_Army(void) const
{
	/*
	**	An object that is dead (or about to be) is not considered part of
	**	the player's army.
	*/
	if (Strength <= 0) {
		return(false);
	}

	/*
	**	If the object is not yet on the map or is otherwise indisposed, then
	**	don't consider it.
	*/
	if (IsInLimbo || !IsLocked) {
		return(false);
	}

	/*
	**	Buildings, although sometimes serving a combat purpose, are not part
	**	of the player's army.
	*/
	if (RTTI == RTTI_BUILDING) {
		return(false);
	}

	/*
	**	If not discoverd by the player, then don't consider it part of the
	**	player's army (yet).
	*/
	if (!IsDiscoveredByPlayer) {
		return(false);
	}

	/*
	**	If not selectable, then not really part of the player's active army.
	*/
	if (!TClass->IsSelectable) {
		return(false);
	}

	/*
	**	If not under player control, then it isn't part of the player's army.
	*/
	if (!House->Is_Player_Control()) {
		return(false);
	}
	return(true);
}


/***********************************************************************************************
 * TechnoClass::What_Weapon_Should_I_Use -- Determines what is the best weapon to use.         *
 *                                                                                             *
 *    This routine will compare the weapons this object is equipped with verses the            *
 *    candidate target object. The best weapon to use against the target will be returned.     *
 *    Special emphasis is given to weapons that can fire on the target without requiring       *
 *    this object to move within range.                                                        *
 *                                                                                             *
 * INPUT:   target   -- The candidate target to determine which weapon is best against.        *
 *                                                                                             *
 * OUTPUT:  Returns with an identifier the specifies what weapon to use against the target.    *
 *          The return value will be "0" for the primary weapon and "1" for the secondary.     *
 *                                                                                             *
 * WARNINGS:   This routine is called very frequently. It should be as efficient as possible.  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/12/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int TechnoClass::What_Weapon_Should_I_Use(AbstractClass * target) const
{
	if (Techno_Type_Class()->Has_Turret_Weapons()) { // RA2: a turret object uses the weapon of its current turret
		return(CurrentWeaponNumber != -1 ? CurrentWeaponNumber : 0); // RA2: as above
	}

	/*
	**	Without a secondary or a primary weapon there is no choice to make.
	*/
	WeaponTypeClass const * secondary = Get_Class_Weapon_Data(1)->Weapon; // RA2: RA2's rules, not the weighted warhead comparison
	if (secondary == NULL) { // RA2: as above
		return(0);
	}
	WeaponTypeClass const * primary = Get_Class_Weapon_Data(0)->Weapon; // RA2: as above
	if (primary == NULL) { // RA2: as above
		return(0);
	}
	if (secondary->NeverUse) { // RA2: as above
		return(0);
	}
	if (secondary->AreaFire && Get_Mission() == MISSION_UNLOAD) { // RA2: as above
		return(1);
	}
	if (Fetch_RTTI() == RTTI_BUILDING && ((BuildingClass const *)this)->IsOverpowered) { // RA2: as above
		return(1);
	}
	if (Fetch_RTTI() == RTTI_BUILDING && ((BuildingClass const *)this)->GetOccupantCount() > 0) { // RA2: a garrison fires the secondary only for Soviet occupants
		return(((BuildingClass const *)this)->Occupants[0]->House->ActLike == 1); // RA2: as above
	}
	if (target == NULL) { // RA2: as above
		return(0);
	}
	if (target->Fetch_RTTI() == RTTI_BUILDING && House->Is_Ally(target) // RA2: an allied building that can be overpowered
		&& secondary->WarheadPtr->ElectricAssault && ((BuildingClass const *)target)->Class->Overpowerable) { // RA2: as above
		return(1);
	}
	if (Fetch_RTTI() == RTTI_AIRCRAFT && ((AircraftClass const *)this)->IsKamikaze) { // RA2: as above
		return(1);
	}
	if (!(target->AbstractFlags & ABSTRACT_TECHNO)) { // RA2: only technos are weighed against the armor
		return(0);
	}
	ObjectClass const * object = (ObjectClass const *)target; // RA2: as above
	ArmorType armor = object->Techno_Type_Class()->Armor; // RA2: as above
	if (secondary->WarheadPtr->Modifier[armor] == 0.0) { // RA2: as above
		return(0);
	}
	armor = object->Techno_Type_Class()->Armor; // RA2: as above
	if (primary->WarheadPtr->Modifier[armor] == 0.0) { // RA2: as above
		return(1);
	}
	bool ground = object->Get_Cell_Ptr()->Land_Type() == LAND_WATER || object->Get_Cell_Ptr()->Land_Type() == LAND_BEACH; // RA2: a target on water or beach
	if (object->In_Air()) { // RA2: as above
		ground = false; // RA2: as above
	}
	if (!object->IsOnBridge && ground) { // RA2: as above
		int which = SelectNavalTargeting((AbstractClass *)object); // RA2: as above
		if (which == -1) { // RA2: as above
			which = 0; // RA2: as above
		}
		return(which); // RA2: the naval targeting answer, -1 becomes 0
	} else if (secondary->Bullet->IsAntiAircraft && object->In_Air()) { // RA2: as above
		return(1);
	}
	return(0);
}


/// <summary>
/// Which weapon this object's naval targeting picks against the target: 0 the primary, 1 the secondary,
/// -1 none.
/// </summary>
int TechnoClass::SelectNavalTargeting(AbstractClass * target) const // RA2: not in TibSun, at 0x6C15D0
{
	bool active = target != NULL && (target->AbstractFlags & ABSTRACT_TECHNO);
	if (!active) {
		return(-1);
	}
	bool underwater = ((TechnoClass *)target)->Techno_Type_Class()->Underwater;
	bool organic = ((TechnoClass *)target)->Techno_Type_Class()->Organic;
	bool hover = ((TechnoClass *)target)->Techno_Type_Class()->Speed == SPEED_HOVER;
	switch (Techno_Type_Class()->NavalTargeting) {
		case 0:
			if (underwater && ((TechnoClass *)target)->Cloak != UNCLOAKED) {
				return(-1);
			}
			return(0);
		case 1:
			return(underwater ? 1 : 0);
		case 2:
			return(underwater ? 0 : -1);
		case 3:
			return(organic ? 1 : 0);
		case 4:
			if (!hover && !organic) {
				return(1);
			}
			break;
		case 5:
			break;
		case 6:
			return(-1);
	}
	return(0);
}





/***********************************************************************************************
 * TechnoClass::How_Many_Survivors -- Determine the number of survivors to escape.             *
 *                                                                                             *
 *    This routine will determine the number of survivors that should run from this object     *
 *    when it is destroyed.                                                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the maximum number of survivors that should run from this object      *
 *          when the object gets destroyed.                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/04/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int TechnoClass::How_Many_Survivors(void) const
{
	if (TClass->IsCrew) {
		return(1);
	}
	return(0);
}


/***********************************************************************************************
 * TechnoClass::Combat_Damage -- Fetch the amount of damage infliced by the specified weapon.  *
 *                                                                                             *
 *    This routine will examine the specified weapon of this object and determine how much     *
 *    damage it could inflict -- best case.                                                    *
 *                                                                                             *
 * INPUT:   which -- Which weapon to consider. If -1 is specified, then the average of both    *
 *                   weapon types (if present) is returned.                                    *
 *                                                                                             *
 * OUTPUT:  Returns with the combat damage that could be inflicted by the specified weapon.    *
 *                                                                                             *
 * WARNINGS:   This routine could return a negative number if a medic is scanned.              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/16/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int TechnoClass::Combat_Damage(int which) const
{
	int value = 0;

	if (which == -1) { // RA2: RA2's form
		int divisor = 0; // RA2: as above

		if (Techno_Type_Class()->Has_Turret_Weapons()) { // RA2: as above
			const WeaponDataStruct * slot = Get_Class_Weapon_Data(CurrentWeaponNumber); // RA2: as above
			if (slot == NULL) { // RA2: as above
				return(0); // RA2: as above
			}
			if (slot->Weapon == NULL) { // RA2: as above
				return(0); // RA2: as above
			}
			value += slot->Weapon->Attack; // RA2: as above
			value += slot->Weapon->AmbientDamage; // RA2: as above
			divisor = 1;
		} else { // RA2: as above
			if (Get_Class_Weapon_Data(0)->Weapon) { // RA2: as above
				value += Get_Class_Weapon_Data(0)->Weapon->Attack; // RA2: as above
				value += Get_Class_Weapon_Data(0)->Weapon->AmbientDamage; // RA2: as above
				divisor = 1; // RA2: as above
			}
			if (Get_Class_Weapon_Data(1)->Weapon) { // RA2: as above
				value += Get_Class_Weapon_Data(1)->Weapon->Attack; // RA2: as above
				value += Get_Class_Weapon_Data(1)->Weapon->AmbientDamage; // RA2: as above
				++divisor; // RA2: as above
			}
		}
		if (divisor > 0) { // RA2: as above
			return(value / divisor); // RA2: as above
		}
		return(0); // RA2: as above
	} else { // RA2: as above
		if (Get_Class_Weapon_Data(which)->Weapon) { // RA2: as above
			value += Get_Class_Weapon_Data(which)->Weapon->Attack; // RA2: as above
			value += Get_Class_Weapon_Data(which)->Weapon->AmbientDamage; // RA2: as above
		}
		return(value); // RA2: as above
	}
}



/***********************************************************************************************
 * TechnoClass::Fire_Coord -- Determine the coordinate where bullets appear.                   *
 *                                                                                             *
 *    This routine will determine the coordinate to use when this object fires. The coordinate *
 *    is the location where bullets appear (or fire effects appear) when the object fires      *
 *    its weapon.                                                                              *
 *                                                                                             *
 * INPUT:   which -- Which weapon is the coordinate to be calculated for? 0 means primary      *
 *                   weapon, 1 means secondary weapon.                                         *
 *                                                                                             *
 * OUTPUT:  Returns with the coordinate that any bullets fired from the specified weapon       *
 *          should appear.                                                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
Coord TechnoClass::Fire_Coord(int which) const
{
	TechnoTypeClass const * tclass = TClass;
	WeaponDataStruct const * weapon = Get_Class_Weapon_Data(which);

	int flhx = weapon->FireFLH.X;
	int flhy = weapon->FireFLH.Y;
	int flhz = weapon->FireFLH.Z;

	Matrix3D matrix; // RA2: the weapon coordinate is taken from the locomotor's matrix, as in Turret_Coord
	double angle; // RA2: as above
	FootClass const * foot = static_cast<FootClass const *>(this); // RA2: as above

	if (foot != NULL && (AbstractFlags & ABSTRACT_FOOT) && foot->Locomotion != NULL) { // RA2: as above
		matrix = foot->Locomotion->Draw_Matrix(NULL); // RA2: as above
		angle = Turret_Facing().As_Radian32() - PrimaryFacing.Current().As_Radian32(); // RA2: as above
	} else {
		matrix.Make_Identity();
		angle = Turret_Facing().As_Radian32(); // RA2: no foot, no locomotor matrix
	}

	matrix.Translate(tclass->TurretOffset, 0, 0); // RA2: the turret offset first, then the turret turn
	matrix.Rotate_Z(angle); // RA2: as above
	matrix.Translate(flhx, flhy * (BurstIndex % 2 != 0 ? -1 : 1), flhz); // RA2: no barrel pitch or barrel length

	Vector3 firevec = matrix * Vector3(0, 0, 0);
	Coord coord(firevec.X, -firevec.Y, firevec.Z);

	return(coord + Render_Coord());
}



/// <summary>
/// Fetches the coordinate that a weapon is mounted at.
/// This is where a projectile is created, and where a firing solution is measured from. The
/// barrel is deliberately not accounted for, so the coordinate holds still as the weapon
/// elevates -- unlike Fire_Coord, which follows the muzzle. A burst weapon alternates between
/// its left and right mounting on successive shots.
/// </summary>
/// <param name="which">Which of the object's weapons to fetch the mounting for.</param>
/// <returns>Returns with the coordinate the projectile should be created at.</returns>
Coord TechnoClass::Turret_Coord(int which) const
{
	TechnoTypeClass const * tclass = TClass;
	Matrix3D matrix; // RA2: Turret_Coord at 0x6C1A70 matches only so
	double angle; // RA2: as above
	FootClass const * foot = static_cast<FootClass const *>(this); // RA2: as above

	if (foot != NULL && (AbstractFlags & ABSTRACT_FOOT) && foot->Locomotion != NULL) { // RA2: as above
		matrix = foot->Locomotion->Draw_Matrix(NULL);
		angle = Turret_Facing().As_Radian32() - PrimaryFacing.Current().As_Radian32(); // RA2: as above
	} else {
		matrix.Make_Identity();
		angle = Turret_Facing().As_Radian32(); // RA2: as above
	}

	matrix.Translate(tclass->TurretOffset, 0, 0); // RA2: as above
	matrix.Rotate_Z(angle); // RA2: as above

	Vector3 firevec = matrix * Vector3(0, 0, 0);
	Coord coord(firevec.X, -firevec.Y, firevec.Z);

	return(Render_Coord() + coord);
}


/// <summary>
/// RA2
/// </summary>
static inline void Set_Recoil(RecoilData & recoil, TurretControl const & control)
{
	recoil.Turret.Travel = control.Travel;
	recoil.Turret.CompressFrames = control.CompressFrames;
	recoil.Turret.RecoverFrames = control.RecoverFrames;
	recoil.Turret.HoldFrames = control.HoldFrames;
}


/// <summary>
/// Initializes owner-related state for the object.
/// Sets IsOwnedByPlayer based on whether the object's house is the player's house.
/// </summary>
void TechnoClass::Init(void)
{
	if (House != NULL) {
		if (PlayerPtr == House) {
			IsOwnedByPlayer = true;
		} else {
			IsOwnedByPlayer = false;
		}
	}

	if (TClass != NULL) { // RA2: 0x6C1C50 has no planning token, slave manager or airstrike
		if (TClass->Spawns != NULL) { // RA2: Init at 0x6C1C50 matches only so
			SpawnManager = new SpawnManagerClass(this, TClass->Spawns, TClass->SpawnsNumber, TClass->SpawnRegenRate, TClass->SpawnReloadRate); // RA2: as above
		}

		if (Get_Class_Weapon_Data(0)->Weapon != NULL && Get_Class_Weapon_Data(0)->Weapon->WarheadPtr->MindControl) { // RA2: as above
			CaptureManager = new CaptureManagerClass(this); // RA2: only the owner (0x467DB0)
		}

		if (RTTI != RTTI_BUILDING && Get_Class_Weapon_Data(0)->Weapon != NULL && Get_Class_Weapon_Data(0)->Weapon->WarheadPtr->Parasite) { // RA2: as above
			static_cast<FootClass *>(this)->ParasiteImUsing = new ParasiteClass(static_cast<FootClass *>(this)); // RA2: as above
		}

		if (Get_Class_Weapon_Data(0)->Weapon != NULL && Get_Class_Weapon_Data(0)->Weapon->WarheadPtr->Temporal) { // RA2: as above
			TemporalImUsing = new TemporalClass(this); // RA2: as above
			TemporalImUsing->WarpPerStep = Get_Class_Weapon_Data(0)->Weapon->Attack; // RA2: as above
		}

		if (TClass->CanDisguise && TClass->PermaDisguise) { // RA2: as above
			Disguised = true; // RA2: as above
			DisguisedAsHouse = House; // RA2: as above
			if (House->ActLike == 0) { // RA2: as above
				Disguise = (ObjectTypeClass *)Rule->AlliedDisguise; // RA2: as above
			} else { // RA2: two sides only
				Disguise = (ObjectTypeClass *)Rule->SovietDisguise; // RA2: as above
			}
		}

		Set_Recoil(TurretRecoil, TClass->TurretAnimData); // RA2: as above
		Set_Recoil(BarrelRecoil, TClass->BarrelAnimData); // RA2: as above
		IsTurretRotateSoundPlaying = 1; // RA2: as above
		TurretIsRotating = 0; // RA2: as above
	}
}


/// <summary>
/// Constructs a techno object without initializing it.
/// This constructor serves the load process, which is about to overwrite every member from
/// the save file. The object still joins the global techno list if it is not already on it.
/// </summary>
#ifdef RA2_NON_MATCHING // not exact: 93 bytes differ (RA2 timers and turret sound added; the original expands Technos.Add inline here, this text calls it)
TechnoClass::TechnoClass(NoInitClass const & x) :
	BASECLASS(x),
	FlasherClass(x),
	StageClass(x),
	Storage(x),
	Cargo(x),
	IdleTimer(x),
	RadarFlashTimer(x),
	TargetingTimer(x), // RA2: the timers RA2 adds are restored too
	IronCurtainTimer(x), // RA2: as above
	IronTintTimer(x), // RA2: as above
	InfantryBlinkTimer(x), // RA2: as above
	DisguiseBlinkTimer(x), // RA2: as above
	ReloadTimer(x), // RA2: as above
	Door(x),
	Veterancy(x),
	CloakDelay(x),
	BarrelPitch(x),
	PrimaryFacing(x),
	SecondaryFacing(x),
	Arm(x),
	TargetingLaserTimer(x)
{
	bool found = false;
	for (int i = 0; i < Technos.Count(); i++) {
		if (Technos[i] == this) {
			found = true;
			break;
		}
	}
	TurretRotateSoundController.Init(); // RA2: the turret sound is reset
	if (!found) {
		Technos.Add(this);
	}
}
#endif


/// <summary>
/// Destroys the techno object and unlinks it from the game.
/// The object gives up its house and drops out of the global techno lists that were holding
/// it.
/// </summary>
TechnoClass::~TechnoClass(void)
{
	if (PlanningToken != NULL) { // RA2: the planning token, the managers and the temporal object are given up with the techno
		PlanningToken->Destroy(); // RA2: as above
		PlanningToken = NULL; // RA2: as above
	}
	if (CaptureManager != NULL) { // RA2: as above
		delete CaptureManager; // RA2: as above
		CaptureManager = NULL; // RA2: as above
	}
	if (SpawnManager != NULL) { // RA2: as above
		delete SpawnManager; // RA2: as above
		SpawnManager = NULL; // RA2: as above
	}
	if (TemporalImUsing != NULL) { // RA2: as above
		delete TemporalImUsing; // RA2: as above
		TemporalImUsing = NULL; // RA2: as above
	}
	TurretRotateSoundController.End(); // RA2: the turret sound stops and is released
	TurretRotateSoundController.Release(); // RA2: as above
	House = NULL;
	IsOwnedByPlayer = false; // RA2: as above
	Technos.Delete(this);
	HousePtrTracker.Delete(this);
	if (BehindAnim != NULL) { // RA2: the anim behind the object is removed
		BehindAnim->Delete_Me(); // RA2: as above
		BehindAnim = NULL; // RA2: as above
	}
	AnimPtrTracker.Delete(this); // RA2: as above
}



/***********************************************************************************************
 * TechnoClass::Time_To_Build -- Determines the time it would take to build this.              *
 *                                                                                             *
 *    Use this routine to determine the amount of time it would take to build an object of     *
 *    this type.                                                                               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the time it should take (unmodified by outside factors) to build      *
 *          this object. The time is expressed in game frames.                                 *
 *                                                                                             *
 * WARNINGS:   The time will usually be modified by power status and handicap rating for the   *
 *             owning house. The value returned is merely the raw unmodified time to build.    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *   09/27/1996 JLB : Takes into account the power availability.                               *
 *=============================================================================================*/
int TechnoClass::Time_To_Build(void) const
{
	int val = Class_Of()->Time_To_Build();

	val = House->Buildtime_Multiplier(Techno_Type_Class()) * val; // RA2: the house type's build time factor, not BuildSpeedBias

	/*
	**	Adjust the time to build based on the power output of the owning house.
	*/
	float power = House->Power_Fraction(); // RA2: a float, bounded by the rules' low power speeds
	float speed = 1.0f - (1.0f - power) * Rule->LowPowerPenaltyModifier; // RA2: as above
	if ((double)Rule->MinLowPowerProductionSpeed >= speed) { // RA2: as above
		speed = Rule->MinLowPowerProductionSpeed; // RA2: as above
	}
	if (power < 1.0f && (double)Rule->MaxLowPowerProductionSpeed <= speed) { // RA2: as above
		speed = Rule->MaxLowPowerProductionSpeed; // RA2: as above
	}
	if (speed == 0.0f) { // RA2: as above
		speed = 0.01f; // RA2: as above
	}
	val = val / speed; // RA2: as above

	bool naval = false; // RA2: ships are counted by their own factories
	if (Fetch_RTTI() == RTTI_UNIT) { // RA2: as above
		TechnoTypeClass const * type = Techno_Type_Class(); // RA2: as above
		if (type != NULL) { // RA2: as above
			naval = type->Naval; // RA2: as above
		}
	}
	int divisor = House->Factory_Count(Fetch_RTTI(), naval); // RA2: as above
	float multiple = Rule->MultipleFactory; // RA2: each factory beyond the first scales the time again
	if (multiple > 0.0f) { // RA2: as above
		for (int count = divisor - 1; count > 0; count--) { // RA2: as above
			val = val * multiple; // RA2: as above
		}
	}
	if (Fetch_RTTI() == RTTI_BUILDING && ((BuildingClass *)this)->Class->IsWall) { // RA2: a wall type is told by the virtual RTTI
		return(val * Rule->WallBuildSpeedCoefficient); // RA2: the wall speed coefficient applies last
	}
	return(val);
}



/***********************************************************************************************
 * TechnoClass::Revealed -- Handles revealing an object to the house specified.                *
 *                                                                                             *
 *    When a unit moves out from under the shroud or when it gets delivered into already       *
 *    explored terrain, then it must be "revealed". Objects that are revealed may be           *
 *    announced to the player. The discovered bit updated accordingly.                         *
 *                                                                                             *
 * INPUT:   house -- The house that this object is revealed to.                                *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/02/1994 JLB : Created.                                                                 *
 *   12/27/1994 JLB : Discovered trigger event processing.                                     *
 *=============================================================================================*/
bool TechnoClass::Revealed(HouseClass * house)
{
	if (house == PlayerPtr) { // RA2: Revealed at 0x6C2440 matches only so
		if (IsDiscoveredByPlayer) { // RA2: as above
			return(false); // RA2: as above
		}
	} else {
		if (IsDiscoveredByComputer) { // RA2: as above
			return(false); // RA2: as above
		}
		IsDiscoveredByComputer = true;
	}

	if (BASECLASS::Revealed(house)) {
		if (!house->Is_Human_Player() && Get_Mission() == MISSION_AMBUSH) { // RA2: as above
			QueueMission(MISSION_HUNT, false); // RA2: as above
		}

		if (house == PlayerPtr) {
			IsDiscoveredByPlayer = true;
			House->RecalcPower = true;
			House->RecalcRadar = true;

			if (!IsOwnedByPlayer) {
				if (!ScenarioInit && Tag != NULL) {
					Tag->Spring(TEVENT_DISCOVERED, this);
				}

				House->IsDiscovered = true;
			} // RA2: Revealed at 0x6C2440 has no Look
		} else {
			IsDiscoveredByComputer = true;
		}
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * TechnoClass::Hidden -- Returns the object back into the hidden state.                       *
 *                                                                                             *
 *    This routine is called for every object that returns to the darkness shroud or when      *
 *    it gets destroyed. This also occurs when an object enters another (such as infantry      *
 *    entering a transport helicopter).                                                        *
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
void TechnoClass::Hidden(void)
{
	if (IsDiscoveredByPlayer) { // RA2: Hidden at 0x6C2520 matches only so
		if (!House->Is_Human_Player()) { // RA2: as above
			IsDiscoveredByPlayer = false; // RA2: as above
		}
	}
}


/***********************************************************************************************
 * TechnoClass::Mark -- Handles marking of techno objects.                                     *
 *                                                                                             *
 *    On the Techno-level, marking handles transmission of the redraw command to any object    *
 *    that it is 'connected' with. This only occurs during loading and unloading.              *
 *                                                                                             *
 * INPUT:   mark  -- The marking method. This routine just passes this on to base classes.     *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoClass::Mark(MarkType mark)
{
	if (BASECLASS::Mark(mark)) {
		if (IsTethered) {
			Transmit_Message(RADIO_REDRAW);
		}
		return(true);
	}
	return(false);
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


/***********************************************************************************************
 * TechnoClass::Receive_Message -- Handles inbound message as appropriate.                     *
 *                                                                                             *
 *    This routine is used to handle inbound radio messages. It only handles those messages    *
 *    that deal with techno objects. Typically, these include loading and unloading.           *
 *                                                                                             *
 * INPUT:   from     -- Pointer to the originator of the radio message.                        *
 *                                                                                             *
 *          message  -- The inbound radio message.                                             *
 *                                                                                             *
 *          param    -- Reference to optional parameter that might be used to transfer         *
 *                      more information than is possible with the simple radio message        *
 *                      type.                                                                  *
 *                                                                                             *
 * OUTPUT:  Returns with the radio response.                                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *   06/17/1995 JLB : Handles tether contact messages.                                         *
 *=============================================================================================*/
RadioMessageType TechnoClass::Receive_Message(RadioClass * from, RadioMessageType message, int & param)
{
	switch (message) {

		/*
		**	Just received instructions to attack the specified target.
		*/
		case RADIO_ATTACK_THIS: // RA2: the message numbers are RA2's (two alternative tether messages sit at 26 and 27)
			{
				WeaponDataStruct const * weapon = GetTurretWeapon(); // RA2: needs a weapon on the turret in use
				if (weapon == NULL || weapon->Weapon == NULL) { // RA2: as above
					break;
				}
				Assign_Target((AbstractClass *)param);
				QueueMission(MISSION_ATTACK, false); // RA2: queued, not assigned
				return(RADIO_ROGER);
			}

		/*
		**	Establish a tethered connection to the object in radio contact.
		*/
		case RADIO_TETHER:
			if (Fetch_RTTI() == RTTI_AIRCRAFT) { // RA2: an airport-bound aircraft takes no tether
				AircraftClass * aircraft = (this == NULL) ? NULL : ((Fetch_RTTI() == RTTI_AIRCRAFT) ? (AircraftClass *)this : NULL); // RA2: as above
				if (aircraft->Class->IsAirportBound) { // RA2: as above
					break;
				}
			}
			if (IsTethered) { // RA2: as above
				break;
			}
			IsTethered = true;
			Transmit_Message(RADIO_TETHER, from);
			return(RADIO_ROGER);

		/*
		**	Break the tethered connection to the object in radio contact.
		*/
		case RADIO_UNTETHER:
			if (!IsTethered) {
				break;
			}
			IsTethered = false;
			Transmit_Message(RADIO_UNTETHER, from);
			return(RADIO_ROGER);

		/*
		**	The alternative tether, used by RA2's docking, works the same way on its own flag.
		*/
		case RADIO_ALT_TETHER: // RA2: as above
			if (IsAlternativeTether) { // RA2: as above
				break;
			}
			IsAlternativeTether = true; // RA2: as above
			Transmit_Message(RADIO_ALT_TETHER, from); // RA2: as above
			return(RADIO_ROGER);

		case RADIO_ALT_UNTETHER: // RA2: as above
			if (!IsAlternativeTether) { // RA2: as above
				break;
			}
			IsAlternativeTether = false; // RA2: as above
			Transmit_Message(RADIO_ALT_UNTETHER, from); // RA2: as above
			return(RADIO_ROGER);

		/*
		**	A tethered unit has reached it's destination. All is
		**	clear, so radio contact can be broken.
		*/
		case RADIO_UNLOADED:
			Transmit_Message(RADIO_UNTETHER, from);
			return(Transmit_Message(RADIO_OVER_OUT, from));

		/*
		**	When this message is received, it means that the other object
		**	has already turned its radio off. Turn this radio off as well.
		*/
		case RADIO_OVER_OUT:
			if (IsTethered && ((TechnoClass *)from)->IsTethered) { // RA2: only an object tethered to a tethered sender lets go
				Transmit_Message(RADIO_UNTETHER, from); // RA2: as above
			}
			BASECLASS::Receive_Message(from, message, param);
			return(RADIO_ROGER);

		/*
		**	Request to be informed when unloaded. This message is transmitted
		**	by the transport unit to the transported unit as it is about to be
		**	unloaded. It is saying, "All is clear. Drive off now."
		*/
		case RADIO_UNLOAD:
		case RADIO_BACKUP_NOW:
		case RADIO_HOLD_STILL:
			Transmit_Message(RADIO_TETHER, from);
			BASECLASS::Receive_Message(from, message, param);
			return(RADIO_ROGER);

		/*
		**	Handle reloading one ammo point for this unit.
		*/
		case RADIO_RELOAD: // RA2: RA2's number
			if (Ammo == Techno_Type_Class()->MaxAmmo) return(RADIO_NEGATIVE); // RA2: as above
			Ammo++;
			return(RADIO_ROGER);

		/*
		**	Handle repair of this unit.
		*/
		case RADIO_REPAIR: // RA2: RA2's number

			/*
			**	Determine if this unit can be repaired becaause it is under strength. If so, then
			**	proceed with the repair process.
			*/
			if (HealthRatio < Rule->ConditionGreen) {
				int cost = Techno_Type_Class()->Repair_Cost(); // RA2: as above
				int step = Techno_Type_Class()->Repair_Step(); // RA2: as above
				if (step <= 1) { // RA2: the step is at least one
					step = 1; // RA2: as above
				}

				/*
				**	If there is sufficient money to repair the unit one step, then do so.
				**	Otherwise return with a "can't complete" radio response.
				*/
				if (House->Available_Money() >= cost) {
					if (cost != 0) {
						House->Spend_Money(cost);
					}
					Strength += step;
					EstimatedHealth += step; // RA2: the estimate follows
					bool foot; // RA2: a parasite that is eating this object is made to let go
					if (this != NULL) { // RA2: as above
						foot = (AbstractFlags & ABSTRACT_FOOT); // RA2: as above
					}
					if (this != NULL && foot && ((FootClass *)this)->ParasiteEatingMe != NULL) { // RA2: as above
						((FootClass *)this)->ParasiteEatingMe->ParasiteImUsing->SuppressionTimer = 50; // RA2: as above
						((FootClass *)this)->ParasiteEatingMe->ParasiteImUsing->ExitUnit(); // RA2: as above
					}

					if (HealthRatio > Rule->ConditionYellow || HeightAGL < -10) {
						if (ParticleSystems[ATTACHED_PARTICLE_DAMAGE]) {
							ParticleSystems[ATTACHED_PARTICLE_DAMAGE]->Delete_Me();
						}
					}

					/*
					**	Return with either an all ok or mission accomplished radio message. This
					**	lets the repairing object know if it should abort the repair control process
					**	or continue it.
					*/
					if (HealthRatio < Rule->ConditionGreen) {
						return(RADIO_ROGER);
					} else {
						int strength = Techno_Type_Class()->MaxStrength; // RA2: the estimate is set with the strength
						Strength = strength; // RA2: as above
						EstimatedHealth = strength; // RA2: as above
						return(RADIO_ALL_DONE); // RA2: RA2's number
					}
				} else {
					return(RADIO_CANT); // RA2: RA2's number
				}
			}
			return(RADIO_NEGATIVE);

		default:
			break;
	}
	return(BASECLASS::Receive_Message(from, message, param));
}









/// <summary>
/// Cloaks this object if it is standing somewhere that will hide it.
/// This routine is called as the object settles into cloaking cover. Everything that was
/// shooting at it is told to re-acquire it afterwards, so that an attacker which can still
/// sense the object does not simply lose track of it.
/// </summary>
void TechnoClass::Try_To_Cloak(void)
{
	int i;

	CellClass & cell = Map[Center_Coord().As_Cell()];
	if (cell.Is_Cloaked(House->HeapID) && Is_Ready_To_Cloak()) {
		DynamicVectorClass<TechnoClass *> targeting_me;
		for (i = Technos.Count() - 1; i >= 0; i--) {
			TechnoClass * tech = Technos[i];
			if (tech->TarCom == this) {
				if (Map[Center_Coord()].Is_Sensed(tech->House->HeapID) || tech->House == House) {
					targeting_me.Add(tech);
				}
			}
		}
		Do_Cloak();
		for (i = targeting_me.Count() - 1; i >= 0; i--) {
			targeting_me[i]->Assign_Target(this);
		}
	}
}





/***********************************************************************************************
 * TechnoClass::Per_Cell_Process -- Handles once-per-cell operations for techno type objects.  *
 *                                                                                             *
 *    This routine handles marking a game object as not a loaner. It is set only if the unit   *
 *    is not player owned and is on the regular map. This is necessary so that enemy objects   *
 *    can exist off-map but as soon as they move onto the map, are flagged so that can never   *
 *    leave it again.                                                                          *
 *                                                                                             *
 * INPUT:   why   -- Specifies the circumstances under which this routine was called.          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *   10/26/94   JLB : Handles scanner units.                                                   *
 *   12/27/1994 JLB : Checks for an processes any trigger in cell.                             *
 *=============================================================================================*/
void TechnoClass::Per_Cell_Process(PCPType why)
{
	if (why == PCP_END) {
		if (TemporalImUsing != 0 && TemporalImUsing->Target != 0) { // RA2: RA2's form
			TemporalImUsing->LetGo(); // RA2: as above
		}
		Cell cell = Center_Coord().As_Cell();
		Try_To_Cloak();
		if (Tag != 0) { // RA2: as above
			Tag->Spring(TEVENT_NEAR_WAYPOINT, this, CELL_NONE, 0, 0); // RA2: as above
		}
		if (!IsLocked && Map.In_Local_Radar(cell, true)) { // RA2: as above
			IsLocked = true; // RA2: as above
		}

		/*
		**	If this object somehow moves into mapped terrain, but is not yet
		**	discovered, then flag it to be discovered.
		*/
		if (!IsDiscoveredByPlayer && Map[cell].IsVisible) {
			Revealed(PlayerPtr);
		}
		Map[cell].Trigger_Veins();
	}
}



/// <summary>
/// Draws the decorations that appear after the object's body.
/// A selected object -- or a submerged one the player's sensors have picked up -- gets its
/// selection box and its condition indicator drawn over it. The talk bubble, when the
/// object has something to say, is drawn whether the object is selected or not.
/// </summary>
#ifdef RA2_NON_MATCHING // not exact: 110 bytes differ (adopted text)
void TechnoClass::Draw_Post_Render(Point2D const & point, Rect const & cliprect) const
{
	bool sensed_underground = false;
	if (!IsSinking) { // RA2: a sinking object draws nothing here
		CellClass * cell = Get_Cell_Ptr();
		if (!IsSelected && HeightAGL < -20 && cell != NULL && cell->Is_Sensed(PlayerPtr->HeapID)) { // RA2: one condition, the cell is tested
			sensed_underground = true;
		}

		if (BombVisible && AttachedBomb != NULL && !Map[Get_Cell()].Is_Shrouded()) { // RA2: a visible bomb is drawn on the object
			Point2D bomb_point; // RA2: as above
			TacticalMap->Coord_To_Pixel(Render_Coord(), bomb_point); // RA2: as above
			Draw_Shape(*LogicalSurface, *MouseDrawer, (ShapeSet const *)Rule->BOMBCURS_SHP, AttachedBomb->Get_Current_Flicker_Frame(), bomb_point, TacticalRect, ShapeFlags_Type(SHAPE_CENTER|SHAPE_WIN_REL|SHAPE_ALPHA), NULL, 0, ZGRAD_GROUND, 1000, NULL, 0, Point2D(0, 0)); // RA2: as above
		}

		if (this != NULL && Fetch_RTTI() == RTTI_BUILDING) { // RA2: a building being repaired shows the wrench
			BuildingClass const * bldg = Fetch_RTTI() == RTTI_BUILDING ? (BuildingClass const *)this : NULL; // RA2: as above
			if (bldg->IsRepairing && !Map[Get_Cell()].Is_Shrouded()) { // RA2: as above
				Point2D repair_point; // RA2: as above
				TacticalMap->Coord_To_Pixel(Render_Coord(), repair_point); // RA2: as above
				int delay = Options.Normalize_Delay(14) / 4; // RA2: as above
				if (delay < 2) { // RA2: as above
					delay = 2; // RA2: as above
				}
				Draw_Shape(*LogicalSurface, *MouseDrawer, (ShapeSet const *)BuildingTypeClass::WrenchShapes, ((Frame % delay) * 6) / (delay - 1), repair_point, cliprect, ShapeFlags_Type(SHAPE_CENTER|SHAPE_WIN_REL|SHAPE_ALPHA), NULL, 0, ZGRAD_GROUND, 1000, NULL, 0, Point2D(0, 0)); // RA2: as above
			}
		}

		if (!IsDisguisedAs(PlayerPtr) && Visual_Character(false, NULL) != VISUAL_HIDDEN) { // RA2: the rank pip is drawn for anything visible
			DrawVeterancyPips(point, cliprect); // RA2: as above
		}

		if (IsSelected || sensed_underground) { // RA2: as above

			if (Fetch_RTTI() == RTTI_BUILDING) { // RA2: no core defenders, the virtual RTTI

				int color = WHITE; // RA2: as above
				if (HeightAGL < -4) { // RA2: no LimpetType (not in RA2's layout)
					color = BLACK; // RA2: as above
				}

				Point3D const & lepton = Techno_Type_Class()->Lepton_Dimensions(); // RA2: the dimensions come by reference
				Coord dim = lepton; // RA2: as above
				int x = dim.X / 2; // RA2: as above
				dim.Y /= 2; // RA2: the half is kept in the dimensions
				int y = dim.Y; // RA2: as above
				color = NormalDrawer->Convert_Pixel(color); // RA2: as above
				Coord const & position = Center_Coord(); // RA2: as above
				Coord center = position; // RA2: as above

				if (Fetch_RTTI() != RTTI_INFANTRY) { // RA2: the virtual RTTI
					Draw_Double_Selection_Bracket(center + Coord(x, y, 0), center + Coord(-x, y, 0), color); // RA2: as above
					Draw_Double_Selection_Bracket(center + Coord(x, y, 0), center + Coord(x, -y, 0), color); // RA2: as above
					Draw_Double_Selection_Bracket(center + Coord(-x, y, 0), center + Coord(-x, y, dim.Z), color); // RA2: as above
					Draw_Double_Selection_Bracket(center + Coord(x, -y, 0), center + Coord(x, -y, dim.Z), color); // RA2: as above
				}

				if (Strength > 0 && (House->Is_Ally(PlayerPtr) || Rule->IsHealthBar)) { // RA2: as above
					((TechnoClass *)this)->Unknown_Virtual_3DC((unsigned)&point, (unsigned)&cliprect); // RA2: the old health bar is a virtual taking the argument addresses
				}

				if (Fetch_RTTI() != RTTI_INFANTRY) { // RA2: as above
					Draw_Single_Selection_Bracket(center + Coord(x, y, 0), center + Coord(x, y, dim.Z), color); // RA2: as above
					Draw_Single_Selection_Bracket(center + Coord(x, -y, dim.Z), center + Coord(x, y, dim.Z), color); // RA2: as above
					Draw_Single_Selection_Bracket(center + Coord(-x, y, dim.Z), center + Coord(x, y, dim.Z), color); // RA2: as above
				} else {
					Draw_Single_Selection_Bracket(center + Coord(-x, y, 0), center + Coord(x, y, 0), color); // RA2: as above
					Draw_Single_Selection_Bracket(center + Coord(-x, y, 0), center + Coord(-x, y, dim.Z), color); // RA2: as above
					Draw_Single_Selection_Bracket(center + Coord(x, -y, dim.Z), center + Coord(x, -y, 0), color); // RA2: as above
					Draw_Single_Selection_Bracket(center + Coord(x, -y, dim.Z), center + Coord(-x, -y, dim.Z), color); // RA2: as above
				}
			}

			Draw_Health_Bar(point, cliprect, sensed_underground); // RA2: as above
		}

		if (Mouse_Hover_Flag(this) && !Techno_Is_Selected(this)) { // RA2: a hovered object shows its health bar too
			if (IsDisguisedAs(PlayerPtr)) { // RA2: as above
				if (GetDisguiseHouse(true) != NULL) { // RA2: as above
					Draw_Health_Bar(point, cliprect, true); // RA2: as above
				}
			} else {
				Draw_Health_Bar(point, cliprect, true); // RA2: as above
			}
		}

		if (TalkBubbleOwner == this && TalkBubbleType > 0 && (int)TalkBubbleTimer > 0) { // RA2: as above
			Draw_Shape(*LogicalSurface, *NormalDrawer, (ShapeSet const *)ObjectTypeClass::TalkBubbleShapes, TalkBubbleType - 1, point, cliprect, ShapeFlags_Type(SHAPE_CENTER|SHAPE_WIN_REL), NULL, 0, ZGRAD_GROUND, 1000, NULL, 0, Point2D(0, 0)); // RA2: the last arguments are passed explicitly
		}
	}
}
#endif



/// <summary>
/// Draws one edge of a selection box as a pair of corner brackets.
/// Only the outer quarter at each end of the edge is drawn, so that a box assembled from
/// these is left open along its sides and does not fence in the object inside it.
/// </summary>
/// <param name="color">The color to draw the edge in. It must already be converted.</param>
void TechnoClass::Draw_Double_Selection_Bracket(Coord const & coord1, Coord const & coord2, int color)
{
	Coord c1 = (coord1 + coord1 + coord1 + coord2) / 4;
	Coord c2 = (coord2 + coord2 + coord2 + coord1) / 4;

	if (coord1.Z > c1.Z) {
		TacticalMap->Draw_3D_Line(coord1, c1, color, 0);
	} else {
		TacticalMap->Draw_3D_Line(c1, coord1, color, 0);
	}

	if (coord2.Z > c2.Z) {
		TacticalMap->Draw_3D_Line(coord2, c2, color, 0);
	} else {
		TacticalMap->Draw_3D_Line(c2, coord2, color, 0);
	}
}


/// <summary>
/// Static helper that draws only a single end-quarter tick of an edge (used for infantry).
/// Renders the outer one-quarter at the first endpoint only via the tactical map's 3D line routine.
/// </summary>
/// <param name="coord1">World coordinate of the endpoint where the tick is drawn.</param>
/// <param name="coord2">World coordinate of the far endpoint defining the edge direction.</param>
/// <param name="color">Converted pixel color used to draw the line.</param>
void TechnoClass::Draw_Single_Selection_Bracket(Coord const & coord1, Coord const & coord2, int color)
{
	Coord c1 = (coord1 + coord1 + coord1 + coord2) / 4;

	if (coord1.Z > c1.Z) {
		TacticalMap->Draw_3D_Line(coord1, c1, color, 0);
	} else {
		TacticalMap->Draw_3D_Line(c1, coord1, color, 0);
	}
}


/// <summary>
/// Draws the old style health bar.
/// Nothing is drawn. The routine survives only as the hook Draw_Post_Render still calls at
/// the point the older health bar used to occupy.
/// </summary>
void TechnoClass::Draw_Health_Bar_Old(Point2D const &, Rect const &) const
{
}











































































































#ifdef _DEBUG
/***********************************************************************************************
 * TechnoClass::Debug_Dump -- Displays the base class data to the monochrome screen.           *
 *                                                                                             *
 *    This routine is used to dump the status of the object class to the monochrome screen.    *
 *    This display can be used to track down or prevent bugs.                                  *
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
void TechnoClass::Debug_Dump(MonoClass * mono) const
{
	mono->Set_Cursor(10, 7);mono->Printf("%2d", Fetch_Rate());
	mono->Set_Cursor(14, 7);mono->Printf("%2d", Fetch_Stage());
	mono->Set_Cursor(49, 1);mono->Printf("%3d", Ammo);
	mono->Set_Cursor(71, 1);mono->Printf("$%4d", PurchasePrice);
	mono->Set_Cursor(54, 1);mono->Printf("%3d", (int)Arm);
	if (Cargo.Is_Something_Attached()) {
		mono->Set_Cursor(1, 5);mono->Printf("%08X", Cargo.Attached_Object());
	}
	if (TarCom != NULL) {
		mono->Set_Cursor(29, 3);mono->Printf("%08X", TarCom);
	}
	if (SuspendedTarCom != NULL) {
		mono->Set_Cursor(38, 3);mono->Printf("%08X", SuspendedTarCom);
	}
	if (ArchiveTarget != NULL) {
		mono->Set_Cursor(69, 5);mono->Printf("%08X", ArchiveTarget);
	}
	mono->Set_Cursor(47, 3);mono->Printf("%02X:%02X", PrimaryFacing.Current(), PrimaryFacing.Desired());
	mono->Set_Cursor(64, 1);mono->Printf("%d(%d)", Cloak, CloakingDevice);

	mono->Fill_Attrib(14, 15, 12, 1, IsUseless ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(14, 16, 12, 1, IsTickedOff ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(14, 17, 12, 1, IsCloakable ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(27, 13, 12, 1, IsLeader ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(27, 14, 12, 1, IsALoaner ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(27, 15, 12, 1, IsLocked ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(27, 16, 12, 1, IsInRecoilState ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(27, 17, 12, 1, IsTethered ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(40, 13, 12, 1, IsOwnedByPlayer ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(40, 14, 12, 1, IsDiscoveredByPlayer ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(40, 15, 12, 1, IsDiscoveredByComputer ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(40, 16, 12, 1, IsALemon ? MonoClass::INVERSE : MonoClass::NORMAL);

	BASECLASS::Debug_Dump(mono);
}
#endif



/// <summary>
/// Draws the rear portion of a selected building's three dimensional selection box.
/// Drawing these edges before the building body allows the body to sit between the rear
/// edges and the front edges drawn by Draw_Post_Render. The box turns yellow while a
/// limpet mine is attached and black while the building sits below ground.
/// </summary>
void TechnoClass::Draw_Pre_Render(Point2D const & point, Rect const & cliprect) const // RA2: ra2-decomp's twin text
{
	if (RTTI == RTTI_BUILDING && IsSelected) {
		if (RTTI != RTTI_INFANTRY) {
			int color = WHITE;
			if (HeightAGL < -4) { // RA2: no LimpetType (not in RA2's layout)
				color = BLACK;
			}
			Point3D dim = TClass->Lepton_Dimensions();
			dim.X /= 2;
			dim.Y /= 2;
			color = NormalDrawer->Convert_Pixel(color);
			Coord center = Center_Coord();
			Draw_Double_Selection_Bracket(center + Coord(-dim.X, -dim.Y, 0), center + Coord(-dim.X, -dim.Y, dim.Z), color);
			Draw_Double_Selection_Bracket(center + Coord(-dim.X, -dim.Y, 0), center + Coord(dim.X, -dim.Y, 0), color);
			Draw_Double_Selection_Bracket(center + Coord(-dim.X, -dim.Y, 0), center + Coord(-dim.X, dim.Y, 0), color);
			Draw_Double_Selection_Bracket(center + Coord(-dim.X, -dim.Y, dim.Z), center + Coord(-dim.X, dim.Y, dim.Z), color);
			Draw_Double_Selection_Bracket(center + Coord(-dim.X, -dim.Y, dim.Z), center + Coord(dim.X, -dim.Y, dim.Z), color);
		}
	}
}


/// <summary>
/// Draws this object's condition indicator.
/// Buildings and core defenders get a pip bar laid along the near edge of their footprint;
/// everything else gets the familiar select box graphic with a row of health pips beneath
/// it. An ally, or a house that has spied on the object, is also shown its cargo pips.
/// </summary>
/// <param name="sensed_underground">Should the select graphic be drawn only when the object is selected?</param>
#ifdef RA2_NON_MATCHING // not exact: no row yet (main has no accepted bound between Draw_Pre_Render and Limbo)
void TechnoClass::Draw_Health_Bar(Point2D const & xpoint, Rect const & cliprect, bool sensed_underground) const
{
	UnitClass const * unit = (RTTI == RTTI_UNIT) ? dynamic_cast<UnitClass const *>(this) : NULL;

	if (RTTI == RTTI_BUILDING) { // RA2: no core defenders

		/*
		 * Build the screen-space pip bar from the object's lepton dimensions.
		 */
		HeightAGL;

		Point3D dim = TClass->Lepton_Dimensions();

		Point3D half(dim.X / 2, dim.Y / 2, dim.Z / 2);

		/*
		 * The "dim - half" difference is computed for all three components and Z is
		 * then replaced with the full height, so the Z store below is dead.
		 */
		Coord coord = dim - half;
		coord.X = -coord.X;
		coord.Z = dim.Z;

		/*
		 * The X sign flips around the Y flips are redundant -- the paired negations
		 * are dead load/neg/neg/store sequences.
		 */
		Point2D p0 = TacticalMap->Coord_To_Pixel_Absolute(coord);
		coord.X = -coord.X;
		coord.Y = -coord.Y;
		coord.X = -coord.X;
		Point2D p1 = TacticalMap->Coord_To_Pixel_Absolute(coord);
		coord.Z = 0;
		coord.Y = -coord.Y;
		Point2D p2 = TacticalMap->Coord_To_Pixel_Absolute(coord);
		coord.X = -coord.X;
		coord.Y = -coord.Y;
		coord.X = -coord.X;

		int barlen = (p0.Y - p1.Y) / 2;

		int n = (int)(HealthRatio * (double)barlen);
		if (n <= 1) {
			n = 1;
		}
		if (n >= barlen) {
			n = barlen;
		}

		int condcolor = 1;
		if (HealthRatio <= Rule->ConditionYellow) {
			condcolor = 2;
		}
		if (HealthRatio <= Rule->ConditionRed) {
			condcolor = 4;
		}

		int ybase = 2 - 2 * barlen;

		Point2D point;
		int index;

		int yoff = 0;
		int xoff = 0;
		for (index = 0; index < n; index++) {
			point.X = xpoint.X + p0.X + 4 * barlen + 3;
			point.Y = xpoint.Y + p0.Y + ybase + 2;
			point.X -= xoff;
			point.Y -= yoff;
			Draw_Shape(*LogicalSurface, *NormalDrawer, (ShapeSet const *)ObjectTypeClass::PipShapes, condcolor, point, cliprect, ShapeFlags_Type(SHAPE_WIN_REL|SHAPE_CENTER));
			xoff += 4;
			yoff -= 2;
		}

		yoff = -2 * n;
		xoff = 4 * n;
		for (index = n; index < barlen; index++) {
			point.X = xpoint.X + p0.X + 4 * barlen + 3;
			point.Y = xpoint.Y + p0.Y + ybase + 2;
			point.X -= xoff;
			point.Y -= yoff;
			Draw_Shape(*LogicalSurface, *NormalDrawer, (ShapeSet const *)ObjectTypeClass::PipShapes, 0, point, cliprect, ShapeFlags_Type(SHAPE_WIN_REL|SHAPE_CENTER));
			xoff += 4;
			yoff -= 2;
		}

		if (House->Is_Ally(PlayerPtr) || (SpiedBy & (1<<(PlayerPtr->Class->House)))) {
			Draw_Pips(xpoint + p2, xpoint, cliprect);
		}

	} else {

		bool powerup = false;
		if (ArmorBias > 1.0 || FirepowerBias > 1.0 || (Is_Foot() && ((FootClass *)this)->SpeedBias > 1.0)) {
			powerup = true;
		}

		Point2D offset;
		int health_bar_count;

		if (RTTI == RTTI_INFANTRY) {
			if (!sensed_underground || IsSelected) {
				Draw_Shape(*LogicalSurface, *NormalDrawer, (ShapeSet const *)ObjectTypeClass::SelectShapes, powerup ? 6 : 2, xpoint, cliprect, ShapeFlags_Type(SHAPE_ALPHA|SHAPE_WIN_REL|SHAPE_CENTER));
			}
			offset = Point2D(-5, -24);
			health_bar_count = 8;
		} else {
			if (!sensed_underground || IsSelected) {
			} // RA2: no LimpetType (not in RA2's layout)
			offset = Point2D(-15, -25);
			health_bar_count = 17;
		}

		int n = (int)(HealthRatio * (double)health_bar_count);
		if (n <= 1) {
			n = 1;
		}
		if (n >= health_bar_count) {
			n = health_bar_count;
		}

		int shapenum = 9;
		if (HealthRatio <= Rule->ConditionYellow) {
			shapenum = 10;
		}
		if (HealthRatio <= Rule->ConditionRed) {
			shapenum = 11;
		}

		Point2D point;
		for (int index = 0; index < n; index++) {
			point.X = xpoint.X + offset.X;
			point.Y = offset.Y + xpoint.Y;
			point.X += 2 * index;
			Draw_Shape(*LogicalSurface, *NormalDrawer, (ShapeSet const *)ObjectTypeClass::PipShapes, shapenum, point, cliprect, ShapeFlags_Type(SHAPE_WIN_REL|SHAPE_CENTER));
		}

		if (House->Is_Ally(PlayerPtr) || (SpiedBy & (1<<(PlayerPtr->Class->House)))) {
			Draw_Pips(xpoint + Point2D(-10, +10), xpoint, cliprect);
		}
	}
}
#endif


/// <summary>
/// Performs the limbo process for a techno object.
/// This routine handles the bookkeeping an owned object must undo as it leaves the map. It
/// drops out of its house's active tracking, hands back the threat it was contributing to
/// the cell it occupied, and stops being tracked on radar.
/// </summary>
/// <returns>bool; Was the object limboed?</returns>
bool TechnoClass::Limbo(void)
{
	if (!IsInLimbo && Techno_Type_Class() != NULL) { // RA2: also needs a type
		if (Techno_Type_Class()->RevealToAll) { // RA2: the sight taken back from every house or from the owner's
			RemoveSight(false, false, true, PlayerPtr); // RA2: as above
		} else {
			RemoveSight(false, false, false, NULL); // RA2: as above
		}
		Map.RevealArea3(PositionCoord, LastSightRange - 3, LastSightRange + 3, 0); // RA2: the revealed area is covered again
		if (Techno_Type_Class()->GapGenerator) { // RA2: the gap goes with it
			DestroyGap(); // RA2: as above
		}
		if (Techno_Type_Class()->BombSight != 0) { // RA2: a bomb detector is taken off the list
			BombList.Remove_Detector(this); // RA2: as above
		}
		House->Tracking_Active_Remove(this, false);
		if (Risk() > 0) { // RA2: the risk is read once; the threat lent is taken back
			CellClass * cell; // RA2: the threat this object lent is taken back (ThreatPosed, not the risk)
			if (this != NULL && (AbstractFlags & ABSTRACT_FOOT)) { // RA2: a foot, by its flag
				cell = &Map[((FootClass *)this)->LastAdjacencyCell]; // RA2: as above
				if (cell != NULL) { // RA2: as above
					cell->Adjust_Threat(Owner(), -(int)ThreatPosed); // RA2: as above
					ThreatPosed = 0; // RA2: as above
				}
			} else {
				cell = Get_Cell_Ptr(); // RA2: as above
				if (cell != NULL) { // RA2: as above
					cell->Adjust_Threat(Owner(), -(int)ThreatPosed); // RA2: as above
					ThreatPosed = 0; // RA2: as above
				}
			}
		}
		if (IsRadarTracked) {
			Radar_Untrack();
		}
		TurretRotateSoundController.EndLooping(); // RA2: the turret sound stops
		IsTurretRotateSoundPlaying = 1; // RA2: as above
		TurretIsRotating = 0; // RA2: as above
	}
	return(BASECLASS::Limbo());
}





/***********************************************************************************************
 * TechnoClass::Unlimbo -- Performs unlimbo process for all techno type objects.               *
 *                                                                                             *
 *    This routine handles the common operation between techno objects when they are           *
 *    unlimboed. This includes revealing the map.                                              *
 *                                                                                             *
 * INPUT:   coord    -- The coordinate to unlimbo object at.                                   *
 *                                                                                             *
 *          dir (optional) -- initial facing direction for this object                         *
 *                                                                                             *
 * OUTPUT:  bool; Was the unlimbo successful?                                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/14/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoClass::Unlimbo(Coord const & coord, Dir256 dir)
{
	IsLocked = Map.In_Local_Radar(coord.As_Cell(), true); // RA2: the cell overload with the height test spelled out

	if (BASECLASS::Unlimbo(coord, dir)) {

		if (!IsActive) {
			return(true);
		}

		UpdateSight(false, false, false, NULL); // RA2: the sight is revealed at once
		Map.RevealArea3(PositionCoord, 0, LastSightRange + 3, 0); // RA2: as above, the surrounding area is uncovered
		if (Techno_Type_Class()->BombSight != 0) { // RA2: a bomb detector joins the list
			BombList.Add_Detector(this); // RA2: as above
		}
		House->Tracking_Active_Add(this, false);
		PrimaryFacing.Set(dir);
		BarrelPitch.Set(DIR_E << 8);
		BarrelPitch.Set_Desired(DirType(DIR_E) - DirType(Dir256(Techno_Type_Class()->FireAngle))); // RA2: the barrel aims at the type's fire angle
		IsTurretRotateSoundPlaying = 1; // RA2: the turret sound is not playing
		TurretIsRotating = 0; // RA2: as above
		if (Fetch_RTTI() == RTTI_INFANTRY) { // RA2: infantry are marked while they enter idle mode
			UnlimboingInfantry = true; // RA2: as above
		}
		Enter_Idle_Mode(true, true); // RA2: two arguments
		UnlimboingInfantry = false; // RA2: as above
		if (Ready_To_Commence()) {
			Commence();
		}

		SightIncrease = Get_Sight_Bonus(coord);

		if (!Map.In_Local_Radar(coord.As_Cell(), true)) { // RA2: as above
			IsDiscoveredByPlayer = false;
		}

		CellClass * here = Get_Cell_Ptr(); // RA2: the threat is only lent when there is a cell
		if (here != NULL) { // RA2: as above
			ThreatPosed = Risk(); // RA2: the risk is kept in ThreatPosed
			here->Adjust_Threat(Owner(), ThreatPosed); // RA2: as above
		}

		RadarPos = Map.Coord_To_Radar_Pixel(PositionCoord, true);

		if (ActLike == HOUSE_NONE) {
			ActLike = House->ActLike;
		}

		return(true);
	}
	return(false);
}



/// <summary>
/// Fetches the extra sight range earned by height.
/// An object raised off the ground can see further than one on it. This routine works out
/// how much further, and is used when the object is unlimboed onto the map.
/// </summary>
/// <param name="coord">The coordinate whose height the bonus is derived from.</param>
/// <returns>Returns with the amount to add to this object's sight range.</returns>
int TechnoClass::Get_Sight_Bonus(Coord const & coord)
{
	return(10 * (coord.Z / Rule->LeptonsPerSightIncrease));
}

/// <summary>
/// The extra weapon range against the given object, combined with the height between the two, for a
/// weapon whose projectile is subject to elevation; 0 unless both are on the ground.
/// </summary>
int TechnoClass::Elevation_Range_Bonus_3D(ObjectClass * object) const // RA2: not in TibSun, at 0x6C48F0
{
	if (On_Ground() && object->On_Ground()) {
		Cell here = Center_Coord().As_Cell();
		Cell there = object->Center_Coord().As_Cell();
		CellClass * source = &Map[here];
		CellClass * target = &Map[there];
		int rise;
		if (source->GetLevel() - target->GetLevel() < 0) {
			rise = 0;
		} else {
			rise = source->GetLevel() - target->GetLevel();
		}
		double scaled = rise / Rule->ElevationIncrement * Rule->ElevationIncrementBonus;
		double cap = Rule->ElevationBonusCap;
		if (scaled >= cap) {
			scaled = Rule->ElevationBonusCap;
		}
		int height = LEVEL_LEPTON_H * rise;
		int lift = (int)scaled << 8;
		return((int)fastmath::sqrt((double)(lift * lift + height * height)));
	}
	return(0);
}


/// <summary>
/// The extra weapon range against the given object for standing above it, in leptons; 0 unless both
/// are on the ground.
/// </summary>
int TechnoClass::Elevation_Range_Bonus(ObjectClass * object) const // RA2: not in TibSun, at 0x6C4A70
{
	if (On_Ground() && object->On_Ground()) {
		Cell here = Center_Coord().As_Cell();
		Cell there = object->Center_Coord().As_Cell();
		CellClass * source = &Map[here];
		CellClass * target = &Map[there];
		int rise;
		if (source->GetLevel() - target->GetLevel() < 0) {
			rise = 0;
		} else {
			rise = source->GetLevel() - target->GetLevel();
		}
		double scaled = rise / Rule->ElevationIncrement * Rule->ElevationIncrementBonus;
		double cap = Rule->ElevationBonusCap;
		if (scaled >= cap) {
			scaled = Rule->ElevationBonusCap;
		}
		return((int)scaled << 8);
	}
	return(0);
}


/// <summary>
/// RA2: helpers of the weapon range check below (RA2 inlines them; their layers decide what stays inline).
/// </summary>
static inline int Distance_3D(Coord const & a, Coord const & b)
{
	Point3D delta = a - b;
	double y = delta.Y;
	double z = delta.Z;
	double x = delta.X;
	return((int)fastmath::sqrt(x * x + z * z + y * y));
}

static inline BuildingClass * As_Building(AbstractClass * target)
{
	if (target == NULL) return NULL;
	if (target->Fetch_RTTI() == RTTI_BUILDING) {
		return((BuildingClass *)target);
	}
	return(NULL);
}

static inline int Length_3D(Point3D const & delta)
{
	return((int)fastmath::sqrt((double)delta.X * delta.X + ((double)delta.Y * delta.Y + (double)delta.Z * delta.Z)));
}

static inline int Length_XY(Point2D const & point)
{
	return((int)fastmath::sqrt((double)point.X * point.X + (double)point.Y * point.Y));
}

static inline Point2D Coord_XY(Point3D const & coord)
{
	return(Point2D(coord.X, coord.Y));
}

static inline int Bridge_Height(Coord const & coord)
{
	return(Map.Get_Height_GL(coord) + BRIDGE_LEPTON_HEIGHT);
}


/// <summary>
/// Whether the weapon fired from the given coordinate reaches the target (TibSun's TechnoTypeClass::In_Range,
/// with RA2's air and elevation range bonuses, minimum range and the bridge check).
/// </summary>
bool TechnoClass::In_Range(Coord const * coord, AbstractClass * target, WeaponTypeClass const * weapon) const // RA2: not in TibSun, at 0x6C4BB0
{
	if (target != NULL && weapon != NULL) {
		int range = weapon->Range;
		if (range == -512) {
			return(true);
		}
		if (target->In_Air()) {
			range += Techno_Type_Class()->AirRangeBonus;
		}
		BulletTypeClass const * bullet = weapon->Bullet;
		int ground_range = range;
		if (bullet->SubjectToElevation) {
			ground_range = Elevation_Range_Bonus_3D((ObjectClass *)target) + range;
		}
		Coord tcoord = target->Center_Coord();
		int minrange = weapon->MinimumRange;
		if (minrange != 0 && Distance_3D(*coord, tcoord) < minrange) {
			return(false);
		}
		if (bullet->IsArcing) {
			Point3D delta = tcoord - *coord;
			int dist = Length_XY(Coord_XY(delta));
			int arcrange = range;
			if (bullet->SubjectToElevation) {
				arcrange = Elevation_Range_Bonus((ObjectClass *)target) + range;
			}
			if (dist > arcrange) {
				return(false);
			}
			double gravity = weapon->Bullet->IsFloater ? Get_Floater_Gravity() : Rule->Gravity;
			int speed = Calculate_Projectile_Speed(arcrange, gravity);
			if (!Is_Projectile_Trajectory_Valid(speed, dist, delta.Z, gravity)) {
				return(false);
			}
			if (Map[tcoord].IsUnderBridge && tcoord.Z - coord->Z >= LEVEL_LEPTON_H * 3) {
				return(false);
			}
			bool clear = TrajectoryHelper::FindFirstImpenetrableObstacle(*coord, tcoord, weapon, House) == NULL;
			return(clear);
		} else {
			BuildingClass * building = As_Building(target);
			if (building != NULL) {
				ground_range += (building->Class->Height(false) + building->Class->Width()) * 64;
			}
			bool inrange;
			if (Fetch_RTTI() != RTTI_AIRCRAFTTYPE) { // RA2: compares with the aircraft type, so every object measures in 3D
				inrange = Length_3D(*coord - tcoord) <= ground_range;
			} else {
				inrange = Length_XY(Coord_XY(*coord) - Coord_XY(tcoord)) <= ground_range;
			}
			if (inrange) {
				if (Map[*coord].IsUnderBridge) {
					int height = Bridge_Height(*coord);
					if (coord->Z < height && tcoord.Z >= height) {
						return(false);
					}
				}
				bool clear = TrajectoryHelper::FindFirstImpenetrableObstacle(*coord, tcoord, weapon, House) == NULL;
				return(clear);
			}
		}
	}
	return(false);
}


/// <summary>
/// Whether the target is within the area-fire weapon's warhead spread (less its inset) of this object, in cells.
/// </summary>
bool TechnoClass::TriggersCellInset(AbstractClass * target) // RA2: not in TibSun, at 0x6C4F50
{
	if (IsAreaFire()) {
		WarheadTypeClass const * warhead = GetDeployWeapon()->Weapon->WarheadPtr;
		if (warhead != NULL) {
			float range = warhead->SpreadFactor;
			range -= warhead->CellInset;
			Cell here = Center_Coord().As_Cell();
			Cell there = target->Center_Coord().As_Cell();
			Cell delta = here - there;
			if ((short)delta.Length() <= range) {
				return(true);
			}
		}
	}
	return(false);
}


/// <summary>
/// Whether the weapon this object would use against the target reaches it.
/// </summary>
bool TechnoClass::IsCloseEnoughToAttack(AbstractClass * target) const // RA2: not in TibSun, at 0x6C5070
{
	return(In_Range(target, What_Weapon_Should_I_Use(target)));
}

















/***********************************************************************************************
 * TechnoClass::In_Range -- Determines if specified target is within weapon range.             *
 *                                                                                             *
 *    This routine is used to compare the distance to the specified target with the range      *
 *    of the weapon. If the target is outside of weapon range, then false is returned.         *
 *                                                                                             *
 * INPUT:   target   -- The target to check if it is within weapon range.                      *
 *                                                                                             *
 *          which    -- Which weapon to use in determining range. 0=primary, 1=secondary.      *
 *                                                                                             *
 * OUTPUT:  bool; Is the specified target within weapon range?                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/14/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoClass::In_Range(AbstractClass * target, int which) const
{
	if (target == NULL) {
		return(true);
	}

	Coord coord = Center_Coord();
	WeaponTypeClass * weapon = Get_Class_Weapon_Data(which)->Weapon; // RA2
	if (weapon != NULL && weapon->CellRangefinding) { // RA2: fires from the centre of its cell
		coord = Map[Center_Coord().As_Cell()].Center_Coord(); // RA2
	}
	if (In_Air()) {
		coord.Z = target->Center_Coord().Z;
	}

	return(In_Range(&coord, target, weapon)); // RA2
}

/// <summary>
/// Whether the target is in range of the secondary weapon when that one is never chosen on its own, else
/// of the weapon this object would use against it.
/// </summary>
bool TechnoClass::InAuxiliarySearchRange(AbstractClass * target) const // RA2: not in TibSun, at 0x6C51A0
{
	int which = GetSecondaryWeaponIndex();
	WeaponDataStruct const * wdata = Get_Class_Weapon_Data(which);
	if (wdata != NULL && wdata->Weapon != NULL && wdata->Weapon->NeverUse) {
		return(In_Range(target, which));
	} else {
		return(IsCloseEnoughToAttack(target));
	}
}


/// <summary>
/// Whether the weapon this object would use against the cell at the coordinate reaches it.
/// </summary>
bool TechnoClass::IsCloseEnoughToAttackCoords(Coord const & coord) const // RA2: not in TibSun, at 0x6C5200
{
	AbstractClass * cell = &Map[coord];
	return(In_Range(cell, What_Weapon_Should_I_Use(cell)));
}








/***********************************************************************************************
 * TechnoClass::In_Range -- Determines if the specified coordinate is within range.            *
 *                                                                                             *
 *    Use this routine to determine if the specified coordinate is within weapon range.        *
 *                                                                                             *
 * INPUT:   coord    -- The coordinate to examine against the object to determine range.       *
 *                                                                                             *
 *          which    -- The weapon to consider when determining range. 0=primary, 1=secondary. *
 *                                                                                             *
 * OUTPUT:  bool; Is the weapon within range?                                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoClass::In_Range(Coord const & coord, int which) const
{
	return(In_Range(&Map[coord], which));
}


#if 0
/***********************************************************************************************
 * TechnoClass::Draw_It -- Draws the health bar (if necessary).                                *
 *                                                                                             *
 *    This routine will draw the common elements for techno type objects. This element is      *
 *    the health bar. The main game object has already been rendered by the time this          *
 *    routine is called.                                                                       *
 *                                                                                             *
 * INPUT:   x,y   -- The coordinate of the center of the unit.                                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *   10/26/94   JLB : Knows about radar scanned cells.                                         *
 *   12/13/1994 JLB : Clips health bar against map edge.                                       *
 *   01/23/1995 JLB : Dynamic selected object rectangle.                                       *
 *=============================================================================================*/
void TechnoClass::Draw_It(int x, int y, int /*WindowNumberType*/ window) const
{
	//assert(IsActive);

	/*
	**	Tells the door logic that it has been drawn.
	*/
	((TechnoClass *)this)->Clear_Redraw_Flag();

	if (IsSelected) {
		GraphicViewPortClass draw_window(	LogicPage->Get_Graphic_Buffer(),
														WindowList[window][WINDOWX] + LogicPage->Get_XPos(),
														WindowList[window][WINDOWY] + LogicPage->Get_YPos(),
														WindowList[window][WINDOWWIDTH],
														WindowList[window][WINDOWHEIGHT]);


		/*
		**	The infantry select box should be a bit higher than normal.
		*/
		if (RTTI == RTTI_INFANTRY) {
			y -= 6;
		}

		if (RTTI == RTTI_BUILDING && ((BuildingTypeClass const &)Class_Of()).Type == STRUCT_BARRACKS) {
			y -= 5;
		}

		/*
		**	Fetch the dimensions of the object. These dimensions will be used to draw
		**	the selection box and the health bar.
		*/
		int width,height;
		Class_Of().Dimensions(width, height);

		if (Strength && (House->Is_Ally(PlayerPtr) || Rule.IsHealthBar)) {
			fixed	ratio = Health_Ratio();
			int	pwidth;		// Pixel width of bar interior.
			int	color;		// The color to give the interior of the bargraph.

			int xx = x-width/2;
			int yy = y-(height/2);

			/*
			**	Draw the outline of the bargraph.
			*/
			draw_window.Remap(xx+1, yy+1, width-1, 3-1, Map.FadingShade);
			draw_window.Draw_Rect(xx, yy, xx+width-1, yy+3, BLACK);

			/*
			**	Determine the width of the interior strength
			**	graph.
			*/
			pwidth = (width-2) * ratio;

			pwidth = Bound(pwidth, 1, width-2);

			color = LTGREEN;
			if (ratio <= Rule->ConditionYellow) {
				color = YELLOW;
			}
			if (ratio <= Rule->ConditionRed) {
				color = RED;
			}
			draw_window.Fill_Rect(xx+1, yy+1, xx+pwidth, yy+(3-1), color);
		}

		/*
		**	Draw the selected object graphic.
		*/
		if (IsSelected) {
			int lx = width/2;
			int ly = height/2;
			int dx = width/5;
			int dy = height/5;
			int fudge = (House->Is_Ally(PlayerPtr) || Rule.IsHealthBar) ? 4 : 0;
			if (What_Am_I() == RTTI_VESSEL) {
				lx = width / 2;
			}

			// Upper left corner.
			draw_window.Draw_Line(x-lx, fudge+y-ly, x-lx+dx, fudge+y-ly, WHITE);
			draw_window.Draw_Line(x-lx, fudge+y-ly, x-lx, fudge+y-ly+dy, WHITE);

			// Upper right corner.
			draw_window.Draw_Line(x+lx, fudge+y-ly, x+lx-dx, fudge+y-ly, WHITE);
			draw_window.Draw_Line(x+lx, fudge+y-ly, x+lx, fudge+y-ly+dy, WHITE);

			// Lower right corner.
			draw_window.Draw_Line(x+lx, y+ly, x+lx-dx, y+ly, WHITE);
			draw_window.Draw_Line(x+lx, y+ly, x+lx, y+ly-dy, WHITE);

			// Lower left corner.
			draw_window.Draw_Line(x-lx, y+ly, x-lx+dx, y+ly, WHITE);
			draw_window.Draw_Line(x-lx, y+ly, x-lx, y+ly-dy, WHITE);
			if (House->Is_Ally(PlayerPtr) || (SpiedBy & (1<<(PlayerPtr->Class->House)))) {
				Draw_Pips((x-lx)+5, y+ly-3, window);
			}
		}
	}
}
#endif



/***********************************************************************************************
 * TechnoClass::Area_Modify -- Determine the area scan modifier for the cell.                  *
 *                                                                                             *
 *    This routine scans around the cell specified and if there are any friendly buildings     *
 *    nearby, the multiplier return value will be reduced. If there are no friendly buildings  *
 *    nearby, then the return value will be 1. It checks to see if the primary weapon is       *
 *    supposed to perform this scan and if so, the scan will be performed. Otherwise the       *
 *    default value is quickly returned.                                                       *
 *                                                                                             *
 * INPUT:   cell  -- The cell where the potential target lies. An area around this cell will   *
 *                   be scanned for friendly buildings.                                        *
 *                                                                                             *
 * OUTPUT:  Returns with the multiplier to be multiplied by the potential target score value.  *
 *          For less opportune targets, the multiplier fraction will be less than one. For     *
 *          all other cases, it will return the default value of 1.                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/23/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
double TechnoClass::Area_Modify(Cell const & cell) const
{
//	assert(PrimaryWeapon != NULL);
	if (PrimaryWeapon == NULL || !PrimaryWeapon->IsSupressed) return(1);

	int crange = Rule->SupressRadius / CELL_LEPTON;
	double odds = 1;

	for (int radius = 1; radius < crange; radius++) {

		/*
		**	Scan the top and bottom rows of the "box".
		*/
		for (int x = -radius; x <= radius; x++) {
			Cell newcell;

			if (cell.X + x < Map.MapRect.X) continue;
			if (cell.X + x >= (Map.MapRect.X+Map.MapRect.Width)) continue;

			if ((cell.Y - radius) >= Map.MapRect.Y) {
				newcell = cell + Cell(x, -radius);
				BuildingClass const * building = Map[newcell].Cell_Building();
				if (building != NULL && House->Is_Ally(building)) {
					odds /= 2;
				}
			}

			if ((cell.Y + radius) < (Map.MapRect.Y+Map.MapRect.Height)) {
				newcell = cell + Cell(x, radius);
				BuildingClass const * building = Map[newcell].Cell_Building();
				if (building != NULL && House->Is_Ally(building)) {
					odds /= 2;
				}
			}
		}

		/*
		**	Scan the left and right columns of the "box".
		*/
		for (int y = -(radius-1); y < radius; y++) {
			Cell newcell;

			if ((cell.Y + y) < Map.MapRect.Y) continue;
			if ((cell.Y + y) >= (Map.MapRect.Y+Map.MapRect.Height)) continue;

			if ((cell.X - radius) >= Map.MapRect.X) {
				newcell = cell + Cell(-radius, y);
				BuildingClass const * building = Map[newcell].Cell_Building();
				if (building != NULL && House->Is_Ally(building)) {
					odds /= 2;
				}
			}

			if ((cell.X + radius) < (Map.MapRect.X+Map.MapRect.Width)) {
				newcell = cell + Cell(radius, y);
				BuildingClass const * building = Map[newcell].Cell_Building();
				if (building != NULL && House->Is_Ally(building)) {
					odds /= 2;
				}
			}
		}
	}
	return(odds);
}
#endif


/***********************************************************************************************
 * TechnoClass::Evaluate_Object -- Determines score value of specified object.                 *
 *                                                                                             *
 *    This routine is used to determine the score value (value as a potential target) of the   *
 *    object specified. This routine will check the specified object for all the various       *
 *    legality checks that threat scanning requires. This is the main workhorse routine for    *
 *    target searching.                                                                        *
 *                                                                                             *
 * INPUT:   method   -- The threat method requested. This is a combined bitflag value that     *
 *                      not only specifies the kind of targets to consider, but how far away   *
 *                      they are allowed to be.                                                *
 *                                                                                             *
 *          mask     -- This is an RTTI mask to use for quickly eliminating object types.      *
 *                      The mask is created outside of this routine because this routine is    *
 *                      usually called from within a loop and this value is constant in that   *
 *                      context.                                                               *
 *                                                                                             *
 *          range    -- The range at which potential target objects are rejected.              *
 *                      0  = must be within weapon range.                                      *
 *                      >0 = must be within this lepton distance.                              *
 *                      <0 = range doesn't matter.                                             *
 *                                                                                             *
 *          object   -- Pointer to the object itself.                                          *
 *                                                                                             *
 *          value    -- Reference to the value variable that this routine will fill in. The    *
 *                      higher the value the more likely this object will be selected as best. *
 *                                                                                             *
 *          zone     -- The zone restriction if any. A -1 means no zone check required.        *
 *                                                                                             *
 * OUTPUT:  Did the target pass all legality checks? If this value is returned true, then the  *
 *          value parameter will be filled in correctly.                                       *
 *                                                                                             *
 * WARNINGS:   This routine is time consuming. Don't call unless necessary.                    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/30/1995 JLB : Created.                                                                 *
 *   07/14/1995 JLB : Forces SAM site to not fire on landed aircraft.                          *
 *   09/22/1995 JLB : Zone checking enabled.                                                   *
 *   10/05/1995 JLB : Gives greater weight to designated enemy house targets.                  *
 *   02/16/1996 JLB : Added additional threat checks.                                          *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: no independent bound yet (006c5570 has no row); 2861 bytes differ against main's function-map extent; main: best 91.81, tried 10 times (order, register, frame)
bool TechnoClass::Evaluate_Object(ThreatType method, int mask, int range, TechnoClass const * object, int & value, int zone, Coord const & coord) const
{
	assert(object != NULL);

	BStart(BENCH_EVAL_OBJECT);

	bool engineer = Is_Renovator();
	if (!object) return(false);
	int which = What_Weapon_Should_I_Use((TechnoClass *)object);
	WeaponTypeClass const * weapon = Get_Class_Weapon_Data(which)->Weapon;
	if (!(method & (THREAT_CAPTURE | THREAT_OCCUPIABLE | THREAT_TECH_BUILDINGS)) &&
		Can_Fire((TechnoClass *)object, which) == FIRE_ILLEGAL) {
		return(false);
	}
	if (object->EstimatedHealth <= 0 && TClass->VHPScan == 2) return(false);
	if (weapon && weapon->WarheadPtr && (int)(weapon->WarheadPtr->Modifier[object->TClass->Armor] * 100.0) == 1) {
		return(false);
	}

	/*
	**	An object in limbo can never be a valid target.
	*/
	if (object->IsInLimbo || object->Strength == 0) {
		BEnd(BENCH_EVAL_OBJECT);
		return(false);
	}

	/*
	**	If the object is cloaked, then it isn't a legal target.
	*/
	if (object->Cloak == CLOAKED) {
		if (!Map[object->Center_Coord()].Is_Sensed(House->HeapID) && House != object->House) {
			BEnd(BENCH_EVAL_OBJECT);
			return(false);
		}
	}

	if (!object->IsLocked) {
		BEnd(BENCH_EVAL_OBJECT);
		return(false);
	}

	/*
	**	If the object is in a "harmless" state, then don't bother to consider it
	**	a threat.
	*/
	if (object->Current_Mission_Control().IsNoThreat) {
		BEnd(BENCH_EVAL_OBJECT);
		return(false);
	}

	if (object->HeightAGL < -20) {
		BEnd(BENCH_EVAL_OBJECT);
		return(false);
	}

	/*
	**	If the object is not within the desired zone, then ignore it, but only if
	**	zone checking is desired.
	*/
	Coord objectcoord = object->Center_Coord();
	if (zone != -1 && Map.Get_Cell_Zone(objectcoord.As_Cell(), TClass->MZone, object->IsOnBridge) != zone) {
		BEnd(BENCH_EVAL_OBJECT);
		return(false);
	}

	/*
	**	Friendly units are never considered a good target. Bail if this
	**	object is a friend.  Unless we're a medic, of course.  But then,
	**	only consider it a target if it's injured.
	*/
	if ((RTTI != RTTI_INFANTRY || !((InfantryClass *)this)->IsBerzerk) && !TClass->AttackFriendlies && House->Is_Ally(object)) {
		if (Combat_Damage() < 0 || engineer) {
			if (object->HealthRatio == Rule->ConditionGreen) {
				BEnd(BENCH_EVAL_OBJECT);
				return(false);
			}
			if (object->RTTI == RTTI_AIRCRAFT && RTTI == RTTI_UNIT) {
				if (object->HeightAGL > 0) {
					BEnd(BENCH_EVAL_OBJECT);
					return(false);
				}
				if (Map[object->Center_Coord()].Cell_Building()) {
					BEnd(BENCH_EVAL_OBJECT);
					return(false);
				}
			} else if (RTTI == RTTI_UNIT && !object->Considered_Vehicle()) {
				BEnd(BENCH_EVAL_OBJECT);
				return(false);
			}
		} else {
			BEnd(BENCH_EVAL_OBJECT);
			return(false);
		}
	}

	if (Scen->Special.IsHarvesterImmune) {
		if (Rule->HarvesterUnit.Is_In_List((UnitTypeClass const *)object->Class_Of())) {
			BEnd(BENCH_EVAL_OBJECT);
			return(false);
		}
	}

	/*
	**	If the object is further away than allowed, bail.
	*/
	int dist;
	if (object->In_Air()) {
		double x = object->Center_Coord().X - Center_Coord().X;
		double y = object->Center_Coord().Y - Center_Coord().Y;
		dist = (int)fastmath::sqrt(x * x + y * y);
	} else {
		dist = Distance_To(object);
	}
	if (range > 0 && dist > range) {
		BEnd(BENCH_EVAL_OBJECT);
		return(false);
	}

	if (range == 0) {
		if (!Is_Weapon_Equipped()) {
			if (dist > TClass->ThreatRange) {
				BEnd(BENCH_EVAL_OBJECT);
				return(false);
			}
		} else {
			if (!In_Range((TechnoClass *)object, which)) {
				BEnd(BENCH_EVAL_OBJECT);
				return(false);
			}
		}
	}

	/*
	**	If the object is not visible, then bail. Human controlled units
	**	are always considered to be visible.
	*/
	if (House->Is_Player_Control() && !object->IsOwnedByPlayer && !object->IsDiscoveredByPlayer && Session.Type == GAME_NORMAL && object->RTTI != RTTI_AIRCRAFT) {
		BEnd(BENCH_EVAL_OBJECT);
		return(false);
	}

	if (object->RTTI == RTTI_BUILDING && ((BuildingClass *)object)->Class->IsInvisibleInGame) {
		BEnd(BENCH_EVAL_OBJECT);
		return(false);
	}

	/*
	**	Quickly eliminate all unit types that are not allowed according to the mask
	**	value.
	*/
	RTTIType otype = object->RTTI;
	if (!((1 << otype) & mask) && (!(mask & 2) || !object->Considered_Vehicle())) {
		BEnd(BENCH_EVAL_OBJECT);
		return(false);		// Mask failure.
	}

	TechnoTypeClass const * tclass = object->TClass;
	if (!tclass->IsLegalTarget) {
		BEnd(BENCH_EVAL_OBJECT);
		return(false);		// Legality failure.
	}

	// RA2: AI engineers and occupiers may still select eligible neutral buildings.
	if ((Session.Type != GAME_NORMAL && object->House->Class->IsMultiplayPassive) ||
		(tclass->IsInsignificant && (otype != RTTI_BUILDING || object->House->Class->IsMultiplayPassive))) {
		if (House->Is_Human_Player() || otype != RTTI_BUILDING || RTTI != RTTI_INFANTRY) return(false);
		InfantryClass * infantry = (InfantryClass *)this;
		BuildingClass const * building = (BuildingClass const *)object;
		bool capture = building->Class->NeedsEngineer && infantry->Class->IsEngineer && object->House != House;
		bool occupy = infantry->Class->Occupier && building->CanBeOccupiedBy(infantry);
		if (Session.Type == GAME_NORMAL || (infantry->Team && infantry->Team->Class->IsBaseDefense) || (!capture && !occupy)) {
			return(false);
		}
	}

	if (tclass->IsTrain && RTTI == RTTI_INFANTRY && ((InfantryClass *)this)->Class->IsVehicleThief) {
		BEnd(BENCH_EVAL_OBJECT);
		return(false);
	}

	/*
	**	Never consider a spy to be a valid target, unless you're a dog
	*/

	/*
	**	Special case so that SAM site doesn't fire on aircraft that are landed.
	*/
	if (object->IsDisguisedAs(House) && !TClass->DetectDisguise) {
		if (!object->DisguiseBlinkTimer.Value() || House->Is_Human_Player()) return(false);
		if (Sim_Random_Pick(0, 99) > Rule->DisabledDisguiseDetectionPercent[House->Difficulty]) return(false);
	}

	if (method & THREAT_CIVILIANS) {
		object->Owner_HouseClass(); /// The result is unused.
		BEnd(BENCH_EVAL_OBJECT);
		return(false);
	}

	/*
	**	If the scan is limited to capturable buildings only, then bail if the examined
	**	object isn't a capturable building.
	*/
	if ((method & THREAT_CAPTURE) && (otype != RTTI_BUILDING || !((BuildingTypeClass const *)tclass)->IsCaptureable)) {
		BEnd(BENCH_EVAL_OBJECT);
		return(false);
	}

	/*
	**	SPECIAL CASE: Friendly units won't automatically fire on buildings
	**	if the building is not aggressive. That is, unless it is part of a team. A team
	**	is allowed to pick any target it so chooses.
	*/
	if ((!Is_Foot() || !((FootClass *)this)->Team) && House->Is_Human_Player() &&
		!object->Considered_Vehicle() && otype == RTTI_BUILDING) {
		WeaponDataStruct const * armed = object->GetTurretWeapon();
		if ((!armed || !armed->Weapon || !tclass->Risk) && !engineer) return(false);
	}

	if (engineer) {
		if (object->RTTI != RTTI_BUILDING || (House->Is_Ally(object->House) && (object->HealthRatio > Rule->ConditionRed || !((BuildingClass *)object)->Class->Cost_Of(House)))) {
			BEnd(BENCH_EVAL_OBJECT);
			return(false);
		}
	}

	/*
	**	If the search is restricted to Tiberium processing objects, then
	**	perform the special qualification check now.
	*/
	if (method & THREAT_TIBERIUM) {
		if (tclass->Capacity == 0) {
			BEnd(BENCH_EVAL_OBJECT);
			return(false);
		}
	}

	CellClass * cell_this = &Map[Get_Coord()];
	CellClass * cell_that = &Map[object->Get_Coord()];
	if (cell_this->IsUnderBridge && cell_that->IsUnderBridge && IsOnBridge != object->IsOnBridge) {
		BEnd(BENCH_EVAL_OBJECT);
		return(false);
	}

	value = Target_Threat((TechnoClass *)object, coord);
	if (TClass->VHPScan == 1) {
		if (object->EstimatedHealth <= 0) {
			value /= 2;
		} else if (object->EstimatedHealth <= tclass->MaxStrength / 2) {
			value *= 2;
		}
	} // RA2: no IsWebby, ProneStruggleTimer (not in RA2's layout)

	/*
	**	If the candidate object is owned by the designated enemy of this house, then
	**	give it a higher value. This will tend to gravitate attacks toward the main
	**	antagonist of this house.
	*/
	if (House->IsAllToHunt && House->Enemy != HOUSE_NONE && object->House != Houses[House->Enemy]) {
		value = 1;
	}

	/*
	**	If power plants are to be considered a greater threat, then increase
	**	their value here. Buildings that produce no power are not considered
	**	a threat.
	*/
	if ((method & THREAT_POWER) && otype == RTTI_BUILDING) {
		if (((BuildingTypeClass const *)tclass)->Power > 0) {
			value += ((BuildingTypeClass const *)tclass)->Power * 1000;
		} else {
			value = 0;
		}
	}

	/*
	**	If factories are to be considered a greater threat, then don't
	**	consider any non-factory building.
	*/
	if (method & THREAT_OCCUPIABLE) {
		if (otype != RTTI_BUILDING || ((BuildingTypeClass const *)tclass)->MaxNumberOccupants <= 0) return(false);
		value += ((BuildingTypeClass const *)tclass)->MaxNumberOccupants * 1000;
	}
	if (method & THREAT_TECH_BUILDINGS) {
		if (otype != RTTI_BUILDING || !((BuildingTypeClass const *)tclass)->NeedsEngineer) return(false);
		value += 1000;
	}

	if ((method & THREAT_FACTORIES) && otype == RTTI_BUILDING) {
		if (((BuildingTypeClass const *)tclass)->ToBuild == RTTI_NONE) {
			value = 0;
		}
	}

	/*
	**	If base defensive structures are to be considered a greater threat, then
	**	don't consider an unarmed building to be a threat.
	*/
	if (method & THREAT_BASE_DEFENSE) {
		WeaponDataStruct const * armed = GetTurretWeapon();
		if (!armed || !armed->Weapon || otype != RTTI_BUILDING ||
			(((BuildingClass const *)object)->Class->CanBeOccupied && !((BuildingClass const *)object)->GetOccupantCount())) {
			value = 0;
		}
	}

	/*
	**	Possibly cause a reduction of the target's value if it is nearby friendly
	**	structures and the primary weapon of this object is flagged for
	**	friendly fire supression special check logic.
	*/
	double areamod = Area_Modify(object->Center_Coord().As_Cell());
	if (areamod != 1) {
		value = areamod * value;
	}

	/*
	**	Lessen threat as a factor of distance.
	*/
	if (value) {
		value = MAX(1, value);
		BEnd(BENCH_EVAL_OBJECT);
		return(true);
	}
	value = 0;
	BEnd(BENCH_EVAL_OBJECT);
	return(false);
}
#endif


/***********************************************************************************************
 * TechnoClass::Evaluate_Cell -- Determine the value and object of specified cell.             *
 *                                                                                             *
 *    This routine will examine the specified cell and return with the potential target        *
 *    object it contains and the value of it. Use this routine when searching for threats.     *
 *                                                                                             *
 * INPUT:   method   -- The scan method to use for target searching.                           *
 *                                                                                             *
 *          mask     -- Prebuilt mask of object RTTI types acceptable for scanning.            *
 *                                                                                             *
 *          range    -- Scan range limit to use for elimination purposes. This ensures that    *
 *                      objects in the "corner" of a square scan get properly discarded.       *
 *                                                                                             *
 *          object   -- Pointer to object pointer to be filled in with the object at this      *
 *                      cell as a valid target.                                                *
 *                                                                                             *
 *          value    -- Reference to the value of the object in this cell. It will be set      *
 *                      according to the object's value.                                       *
 *                                                                                             *
 *          zone     -- The zone restriction if any. A -1 means no zone check required.        *
 *                                                                                             *
 * OUTPUT:  Was a valid potential target found in this cell?                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/19/1995 JLB : Created.                                                                 *
 *   09/22/1995 JLB : Zone checking enabled.                                                   *
 *=============================================================================================*/
bool TechnoClass::Evaluate_Cell(ThreatType method, int mask, Cell const & cell, int range, TechnoClass const * * object, int & value, int zone) const
{
	*object = NULL;
	value = 0;

	/*
	**	Fetch the techno object from the cell. If there is no
	**	techno object there, then bail.
	*/
	CellClass * cellptr = reinterpret_cast<CellClass *>(&Map[cell]); // RA2: RA2's form

	/*
	**	Don't consider for evaluation a cell that is not within the same zone. Only
	**	perform this check if zone checking is required.
	*/
	if (zone != -1 && Map.Get_Cell_Zone(cell, TClass->MZone, true) != zone) {
		return(false);
	}

	ObjectClass const * tentative = cellptr->Cell_Bridge_Occupier(); // RA2: as above
	TechnoClass * tech = NULL;
	if (!tentative) { // RA2: as above
		tentative = cellptr->Cell_Occupier(); // RA2: as above
	}

	while (tentative) { // RA2: as above
		if (tentative != this) {
			if (!tentative) { // RA2: as above
				tech = NULL; // RA2: as above
			} else { // RA2: as above
				if (tentative->AbstractFlags & ABSTRACT_TECHNO) { // RA2: as above
					tech = static_cast<TechnoClass *>((ObjectClass *)tentative); // RA2: as above
				} else {
					tech = NULL; // RA2: as above
				}
			}
			if (tech) {
				if (Combat_Damage() < 0) {
					if (tech->HealthRatio < Rule->ConditionGreen && House->Is_Ally(tech)) { // RA2: as above
						break;
					}
				} else {
					if (!House->Is_Ally(tech) || TClass->AttackFriendlies // RA2: as above
					   || (RTTI == RTTI_INFANTRY && (((InfantryClass*)this)->IsBerzerk // RA2: as above
					   || (((InfantryClass*)this)->Class->IsEngineer && CurrentMission == MISSION_GUARD_AREA // RA2: as above
						   && tech->HealthRatio <= Rule->ConditionRed && tech->RTTI == RTTI_BUILDING // RA2: as above
						   && static_cast<BuildingClass *>(tech)->Class->Cost_Of(tech->House) > 0)))) { // RA2: as above
						break;
					}
				}
			}
		}
		tentative = tentative->Next;
	}

	if (!tech) { // RA2: as above
		return(false);
	}
	*object = tech;

	bool result = false; // RA2: as above
	if (!House->Is_Ally(tech) || Combat_Damage() < 0 // RA2: as above
	   || (!House->Is_Human_Player() && this && RTTI == RTTI_INFANTRY // RA2: as above
		   && ((RTTI == RTTI_INFANTRY) ? (InfantryClass*)this : (InfantryClass*)0)->Class->IsEngineer)) { // RA2: as above
		result = Evaluate_Object(method, mask, range, tech, value); // RA2: as above
	}
	return(result);
}









/***********************************************************************************************
 * TechnoClass::Evaluate_Just_Cell -- Evaluate a cell as a target by itself.                   *
 *                                                                                             *
 *    This will examine the cell (as if it contained no sentient objects) and determine a      *
 *    target value to assign to it. Typically, this is only useful for wall destroyable        *
 *    weapons when dealing with enemy walls.                                                   *
 *                                                                                             *
 * INPUT:   cell  -- The cell to examine and evaluate.                                         *
 *                                                                                             *
 * OUTPUT:  Returns with the target value to assign to this cell.                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/10/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int TechnoClass::Evaluate_Just_Cell(Cell const & cell) const
{
	/*
	**	First, only computer objects are allowed to automatically scan for walls.
	*/
	if (House->Is_Human_Player()) { // RA2: Evaluate_Just_Cell at 0x6C6350 has no BStart
		return(0); // RA2: Evaluate_Just_Cell at 0x6C6350 has no BEnd
	}

	/*
	**	Even then, if the difficulty indicates that it shouldn't search for wall
	**	targets, then don't allow it to do so.
	*/
	if (!Rule->Diff[House->Difficulty].IsWallDestroyer) {
		return(0); // RA2: Evaluate_Just_Cell at 0x6C6350 has no BEnd
	}

	/*
	**	Determine if, in fact, a wall is located at this cell location.
	*/
	CellClass const * cellptr = &Map[cell];
	if (cellptr->Overlay == OVERLAY_NONE || !OverlayTypes[cellptr->Overlay]->IsWall) {
		return(0); // RA2: Evaluate_Just_Cell at 0x6C6350 has no BEnd
	}

	/*
	**	As a convenience to the target scanning logic, don't consider any wall to be
	**	a target if it isn't in range of the primary weapon.
	*/
	int primary = What_Weapon_Should_I_Use(&Map[cell]);
	if (!In_Range(cell, primary)) {
		return(0); // RA2: Evaluate_Just_Cell at 0x6C6350 has no BEnd
	}

	/*
	**	See if the object has a weapon that can damage walls.
	*/
	WeaponDataStruct const * data = GetTurretWeapon(); // RA2: Evaluate_Just_Cell at 0x6C6350 matches only so
	if (data == NULL || data->Weapon == NULL || data->Weapon->WarheadPtr == NULL) { // RA2: as above
		return(0);
	}

	/*
	**	If the weapon cannot deal with ground based targets, then don't consider
	**	this a valid cell target.
	*/
	if (data->Weapon->Bullet != NULL && !data->Weapon->Bullet->IsAntiGround) { // RA2: as above
		return(0);
	}

	/*
	**	If the primary weapon cannot destroy a wall, then don't give the cell any
	**	value as a target.
	*/
	if (!data->Weapon->WarheadPtr->IsWallDestroyer) { // RA2: as above
		return(0);
	}

	/*
	**	If this is a friendly wall, then don't attack it.
	*/
	if (cellptr->Owner == HOUSE_NONE || House->Is_Ally(Houses[cellptr->Owner])) {
		return(0); // RA2: Evaluate_Just_Cell at 0x6C6350 has no BEnd
	}

	/*
	**	Since a wall was found, then return a value adjusted according to the range the wall
	**	is from the object. The greater the range, the lesser the value returned.
	*/
	return(Weapon_Range(0) - Distance(cell)); // RA2: Evaluate_Just_Cell at 0x6C6350 has no BEnd
}


/***********************************************************************************************
 * TechnoClass::Greatest_Threat -- Determines best target given search criteria.               *
 *                                                                                             *
 *    This routine will scan game objects looking for the best target. It is used by the       *
 *    general target searching processes. The type of target scan to perform is controlled     *
 *    by the method control parameter.                                                         *
 *                                                                                             *
 * INPUT:   method   -- The method control parameter is used to control the type of target     *
 *                      scan performed. It consists of a series of bit flags (see ThreatType)  *
 *                      that are combined to form the target scan desired.                     *
 *                                                                                             *
 * OUTPUT:  Returns the target value of a suitable target. If no target was found then the     *
 *          value TARGET_NONE is returned.                                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/14/1994 JLB : Created.                                                                 *
 *   06/20/1995 JLB : Greatly optimized scan method.                                           *
 *   09/22/1995 JLB : Takes into account the zone (if necessary).                              *
 *   05/30/1996 JLB : Tighter elimination mask checking.                                       *
 *=============================================================================================*/
AbstractClass * TechnoClass::Greatest_Threat(ThreatType method, Coord const & coord, bool onlyenemy) const
{
	ObjectClass const * best_object = 0;
	int best_value = -1;
	int zone = -1;
	++TargetScan;
	if (Techno_Type_Class()->IsNoAutoFire && House->Is_Human_Player()) return 0;
	if (!(method & 1) && Fetch_RTTI() != RTTI_BUILDING && Fetch_RTTI() != RTTI_AIRCRAFT) {
		zone = Map.Get_Cell_Zone(Center_Coord().As_Cell(), Techno_Type_Class()->MZone, true);
	}
	if (Fetch_RTTI() == RTTI_INFANTRY) {
		if (Combat_Damage(-1) < 0) method = ThreatType((method & 3) | 0x4008);
		else if (static_cast<InfantryClass const *>(this)->Class->IsEngineer) method = ThreatType(method & ~0x18);
	} else if (Fetch_RTTI() == RTTI_UNIT) {
		if (Combat_Damage(-1) < 0) method = ThreatType((method & 3) | 0x4010);
	}
	int mask = 0;
	if (method & 0x100) mask = 0x8042;
	if (method & 4) mask |= 4;
	if (method & 0x1ba60) mask |= 0x40;
	if (method & 8) mask |= 0x8000;
	if (method & 0x50) mask |= 2;
	if (Techno_Type_Class()->DistributedFire) {
		CurrentTargets.Clear();
		CurrentTargetThreatValues.Clear();
	}
	if (method & 3) {
		int range = 0;
		if (method & 1) range = Threat_Range(0);
		else if (method & 2) {
			if (CurrentMission == MISSION_PATROL) range = Threat_Range(2);
			else range = Threat_Range(1);
		}
		if (Combat_Damage(-1) < 0 && CurrentMission == MISSION_GUARD) range = 512;
		int cell_range = range / 256;
		if (!range) {
			if (Techno_Type_Class()->Has_Turret_Weapons()) cell_range = Weapon_Range(CurrentWeaponNumber) / 256;
			else if (Techno_Type_Class()->Underwater && Techno_Type_Class()->Organic && Techno_Type_Class()->IsSelfHealing) cell_range = Techno_Type_Class()->ThreatRange / 256;
			else cell_range = ((Weapon_Range(0) > Weapon_Range(1)) ? Weapon_Range(0) : Weapon_Range(1)) / 256;
			++cell_range;
			cell_range += Techno_Type_Class()->AirRangeBonus / 256;
		}
		Cell center(coord.As_Cell());
		if (method & 4) {
			int air_mask = mask | 0x8002;
			AircraftTracker.Append_Radius_Bins(&Map[center], cell_range);
			TechnoClass * object = AircraftTracker.Get();
			while (object) {
				int value = 0;
				if (!House->Is_Ally(object) && (!(method & 0x4000) || House->Is_Ally(object->House)) && object->IsDown && object->In_Which_Layer() != LAYER_GROUND && (!onlyenemy || object->House->HeapID == House->Enemy) && Evaluate_Object(method, air_mask, range, object, value, -1, COORD_NONE)) {
					if (Techno_Type_Class()->DistributedFire) {
						CurrentTargets.Add((AbstractClass *)object);
						CurrentTargetThreatValues.Add(value);
					}
					if (value > best_value) { best_object = object;best_value = value; }
				}
				object = AircraftTracker.Get();
			}
		}
		if (method & 0x10) mask |= 4;
		if (!(method ^ 5)) return (ObjectClass *)best_object;
		{
			Cell best_cell(0, 0);
			int best_cell_value = 0;
			TechnoClass const * object;
			int value;
			for (int radius = 0; radius < cell_range; ++radius) {
				for (int x = -radius; x <= radius; ++x) {
					Cell scan = center + Cell(x, -radius);
					if (Map.In_Radar(scan)) {
						if (Evaluate_Cell(method, mask, scan, range, &object, value, zone) && object && (!onlyenemy || static_cast<TechnoClass const *>(object)->House->HeapID == House->Enemy) && (!(method & 0x4000) || House->Is_Ally(static_cast<TechnoClass const *>(object)->House))) {
							if (Techno_Type_Class()->DistributedFire) { CurrentTargets.Add((AbstractClass *)object);CurrentTargetThreatValues.Add(value); }
							if (best_value < value) { best_object = object;best_value = value; }
						}
						if (!best_object) {
							value = Evaluate_Just_Cell(scan);
							if (best_cell_value < value) {best_cell_value = value;best_cell = scan;}
						}
					}
					scan = center + Cell(x, radius);
					if (Map.In_Radar(scan)) {
						if (Evaluate_Cell(method, mask, scan, range, &object, value, zone) && object && (!onlyenemy || static_cast<TechnoClass const *>(object)->House->HeapID == House->Enemy) && (!(method & 0x4000) || House->Is_Ally(static_cast<TechnoClass const *>(object)->House))) {
							if (Techno_Type_Class()->DistributedFire) { CurrentTargets.Add((AbstractClass *)object);CurrentTargetThreatValues.Add(value); }
							if (best_value < value) { best_object = object;best_value = value; }
						}
						if (!best_object) {
							value = Evaluate_Just_Cell(scan);
							if (best_cell_value < value) {best_cell_value = value;best_cell = scan;}
						}
					}
				}
				for (int y = 1 - radius; y < radius; ++y) {
					Cell scan = center + Cell(-radius, y);
					if (Map.In_Radar(scan)) {
						if (Evaluate_Cell(method, mask, scan, range, &object, value, zone) && object && (!onlyenemy || static_cast<TechnoClass const *>(object)->House->HeapID == House->Enemy) && (!(method & 0x4000) || House->Is_Ally(static_cast<TechnoClass const *>(object)->House))) {
							if (Techno_Type_Class()->DistributedFire) { CurrentTargets.Add((AbstractClass *)object);CurrentTargetThreatValues.Add(value); }
							if (best_value < value) { best_object = object;best_value = value; }
						}
						if (!best_object) {
							value = Evaluate_Just_Cell(scan);
							if (best_cell_value < value) {best_cell_value = value;best_cell = scan;}
						}
					}
					scan = center + Cell(radius, y);
					if (Map.In_Radar(scan)) {
						if (Evaluate_Cell(method, mask, scan, range, &object, value, zone) && object && (!onlyenemy || static_cast<TechnoClass const *>(object)->House->HeapID == House->Enemy) && (!(method & 0x4000) || House->Is_Ally(static_cast<TechnoClass const *>(object)->House))) {
							if (Techno_Type_Class()->DistributedFire) { CurrentTargets.Add((AbstractClass *)object);CurrentTargetThreatValues.Add(value); }
							if (best_value < value) { best_object = object;best_value = value; }
						}
						if (!best_object) {
							value = Evaluate_Just_Cell(scan);
							if (best_cell_value < value) {best_cell_value = value;best_cell = scan;}
						}
					}
				}
				if (best_object) {
					if (radius == cell_range / 4 || radius == cell_range / 2) return (ObjectClass *)best_object;
				}
				if (best_cell != CELL_NONE) return &Map[best_cell];
			}
		}
	} else {
		int index;
		if (mask & 4) {
			for (index = 0; index < Aircraft.Count(); ++index) {
				TechnoClass * object = Aircraft[index];
				int value = 0;
				if (!House->Is_Ally(object) && (!onlyenemy || object->House->HeapID == House->Enemy) && (!(method & 0x4000) || House->Is_Ally(object->House)) && Evaluate_Object(method, mask, -1, object, value, -1, COORD_NONE) && value > best_value) { best_object = object;best_value = value; }
			}
		}
		if (method & 0x10) mask |= 4;
		for (index = 0; index < Technos.Count(); ++index) {
			TechnoClass * object = Technos[index];
			int value = 0;
			if ((!House->Is_Ally(object) || Combat_Damage(-1) < 0 || (!House->Is_Human_Player() && this && Fetch_RTTI() == RTTI_INFANTRY && (this && Fetch_RTTI() == RTTI_INFANTRY ? static_cast<InfantryClass const *>(this) : 0)->Class->IsEngineer)) && object->Layer == LAYER_GROUND && (!onlyenemy || object->House->HeapID == House->Enemy) && (!(method & 0x4000) || House->Is_Ally(object->House)) && Evaluate_Object(method, mask, -1, object, value, zone, coord) && value > best_value) {best_object = object;best_value = value;}
		}
	}
	return (ObjectClass *)best_object;
}









/***********************************************************************************************
 * TechnoClass::Owner -- Who is the owner of this object?                                      *
 *                                                                                             *
 *    Use this routine to examine this object and return who the owner is.                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the house number of the owner of this object.                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/09/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
HousesType TechnoClass::Owner(void) const
{
	return(House->HeapID);
}


/// <summary>
/// Returns the house that owns this techno object.
/// </summary>
/// <returns>HouseClass *; pointer to the owning house.</returns>
HouseClass * TechnoClass::Owner_HouseClass(void) const
{
	return(House);
}


/***********************************************************************************************
 * TechnoClass::Clicked_As_Target -- Sets the flash count for this techno object.              *
 *                                                                                             *
 *    Use this routine to set the flash count for the object. This flash count is the number   *
 *    of times the object will "flash". Typically it is called as a result of the player       *
 *    clicking on this object in order to make it the target of a move or attack.              *
 *                                                                                             *
 * INPUT:   count -- The number of times the object should flash.                              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/09/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void TechnoClass::Clicked_As_Target(int count)
{
	if (!IsDisguisedAs(PlayerPtr) || GetDisguiseHouse(true)) { // RA2: a disguised unit does not flash for the player
		FlashCount = count; // RA2: as above
	}
}



/// <summary>
/// Determines whether this object's voxel library is loaded and usable.
/// Returns false if the voxel library is NULL or failed to load.
/// </summary>
/// <returns>bool; is the voxel library loaded successfully?</returns>
bool TechnoClass::Is_Voxel_Loaded(void) const
{
	if (TClass->Voxel.VoxLib == NULL) {
		return(false);
	}
	if (TClass->Voxel.VoxLib->Load_Failed()) {
		return(false);
	}
	return(true);
}




/***********************************************************************************************
 * TechnoClass::AI -- Handles AI processing for techno object.                                 *
 *                                                                                             *
 *    This routine handles AI processing for techno objects. Typically, this merely dispatches *
 *    to the appropriate AI routines for the base classes.                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Make sure that this routine is only called ONCE per game tick.                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/09/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
static inline Point3D Offset_Coord(Coord const & center, Coord const & offset)
{
	return Point3D(offset.X + center.X, offset.Y + center.Y, offset.Z + center.Z);
}

void TechnoClass::AI(void)
{
	Point2D point;
	Rect window;
	if (IsMouseHovering) IsMouseHovering = false;
	Update_Iron_Tint();
	if (EstimatedHealth > Strength) EstimatedHealth = Strength;
	if ((Frame & 4) && EstimatedHealth < Strength) {
		if (EstimatedHealth + 30 < 0) EstimatedHealth = -30;
		++EstimatedHealth;
	}
	if (Techno_Type_Class()->IsTurretEquipped) {
		if (TurretIsRotating) {
			if (IsTurretRotateSoundPlaying) {
				TurretRotateSoundController.Stop();
				VocClass::PlayAt(Techno_Type_Class()->TurretRotateSound, PositionCoord, &TurretRotateSoundController);
				IsTurretRotateSoundPlaying = 0;
			}
		} else { TurretRotateSoundController.EndLooping(); IsTurretRotateSoundPlaying = 1; }
		VocClass::UpdateAt(PositionCoord, &TurretRotateSoundController);
	}
	int old_rank = CurrentRanking;
	if (old_rank != Veterancy.Remaining_Levels()) {
		if (old_rank != -1) {
			if (Veterancy.Remaining_Levels() == 0) {
				if (House->Is_Player_Control()) { VocClass::PlayAt(Rule->UpgradeEliteSound, PositionCoord, 0); Queue_Speech("EVA_UnitPromoted", -1, -1); }
				FlashCount = Rule->EliteFlashTimer;
			} else if (Veterancy.Remaining_Levels() == 1 && House->Is_Player_Control()) {
				VocClass::PlayAt(Rule->UpgradeVeteranSound, PositionCoord, 0);
				Queue_Speech("EVA_UnitPromoted", -1, -1);
			}
		}
		CurrentRanking = Veterancy.Remaining_Levels();
	}
	if (Is_Voxel_Loaded()) { Rocking_AI(); if (!IsActive) return; }
	Cell where(Center_Coord().As_Cell());
	CellClass & cell = Map[where];
	bool draw_behind;
	if (Fetch_RTTI() == RTTI_BUILDING) {
		if (static_cast<BuildingClass *>(this)->Class->CanBeHidden) draw_behind = cell.OccupyHeightsCoveringMe > 1;
		else draw_behind = false;
	} else draw_behind = cell.OccupyHeightsCoveringMe != 0;
	if (draw_behind && !(this && Fetch_RTTI() == RTTI_AIRCRAFT)) {
		Draw_Behind_Marker(point, &window);
	} else if (BehindAnim) {
		if (Fetch_RTTI() != RTTI_BUILDING) AnimPtrTracker.Delete(this);
		BehindAnim->Delete_Me();
		BehindAnim = 0;
	}
	if (!House->Is_Human_Player() && TarCom && House->Is_Ally(TarCom)) {
		bool infantry = Fetch_RTTI() == RTTI_INFANTRY;
		bool building = TarCom->Fetch_RTTI() == RTTI_BUILDING;
		bool can_enter = infantry && building && static_cast<BuildingClass *>(TarCom)->CanBeOccupiedBy((InfantryClass *)this);
		bool engineer = infantry && static_cast<InfantryClass *>(this)->Class->IsEngineer;
		bool assault = Get_Class_Weapon_Data(1)->Weapon && Get_Class_Weapon_Data(1)->Weapon->WarheadPtr->ElectricAssault && building && static_cast<BuildingClass *>(TarCom)->Class->Overpowerable;
		if (!engineer && !assault && !can_enter) Assign_Target(0);
	}
	if ((int)Frame % 16 == 0 && CurrentMission != MISSION_CAPTURE && CurrentMission != MISSION_SABOTAGE) {
		int fire_error = GetFireErrorWithoutRange(TarCom, What_Weapon_Should_I_Use(TarCom));
		if (TarCom && (fire_error == 5 || fire_error == 6)) Assign_Target(0);
	}
	if (Techno_Type_Class()->TurretRecoil) { TurretRecoil.Update(); BarrelRecoil.Update(); }
	if (Techno_Type_Class()->IsChargeTurret && Techno_Type_Class()->Has_Turret_Weapons()) {
		int turret = 0;
		if (ChargeTurretDelay > 0) {
			turret = Techno_Type_Class()->TurretCount * Arm.Value() / ChargeTurretDelay;
			if (turret >= Techno_Type_Class()->TurretCount) turret = Techno_Type_Class()->TurretCount - 1;
			else if (turret < 0) turret = 0;
		}
		CurrentTurretNumber = turret;
	}
	if (Door.Has_Ended()) Door.End_Count();
	if (Cooldown) --Cooldown;
	if (TarCom && ShouldLoseTargetNow) {
		if (CurrentMission == MISSION_SLEEP || CurrentMission == MISSION_ENTER || CurrentMission == MISSION_STOP || CurrentMission == MISSION_AMBUSH || CurrentMission == MISSION_UNLOAD || CurrentMission == MISSION_CONSTRUCTION || CurrentMission == MISSION_DECONSTRUCTION || CurrentMission == MISSION_REPAIR || CurrentMission == MISSION_MISSILE || CurrentMission == MISSION_HARMLESS || CurrentMission == MISSION_WAIT || CurrentMission == MISSION_OPEN) Assign_Target(0);
	}
	MissionClass::AI();
	if (TargetingTimer.Value() == 0) {
		if (MegaMissionIsAttackMove()) {
			UpdateAttackMove();
		} else if ((CurrentMission == MISSION_MOVE || CurrentMission == MISSION_HARVEST || CurrentMission == MISSION_GUARD) && Should_Scan_For_Target()) {
			LastTargetScanFrame = Frame;
			AbstractClass * target = TarCom;
			Coord coord = PositionCoord;
			if (Target_Something_Nearby(coord, THREAT_RANGE) && TarCom != target) ShouldLoseTargetNow = true;
		}
	}
	if (AttachedBomb && AttachedBomb->Time_To_Explode()) AttachedBomb->Detonate();
	if (!IsActive) return;
	if (Should_Self_Heal_Now()) {
		++Strength;
		if ((HealthRatio > Rule->ConditionYellow || HeightAGL < -10) && ParticleSystems[3]) ParticleSystems[3]->Delete_Me();
	}
	Cloaking_AI(0);
	if (SpawnManager) SpawnManager->AI();
	if (Cloak == UNCLOAKED && Map[Center_Coord()].Is_Cloaked(House->HeapID)) Try_To_Cloak();
	if (Cloak == CLOAKED && !Map[Center_Coord()].Is_Cloaked(House->HeapID)) Try_To_Cloak();
	if (Session.Type == GAME_NORMAL && !House->Is_Player_Control() && TarCom && !House->Is_Ally(TarCom) && Combat_Damage(-1) < 0) Assign_Target(0);
	if (TarCom && Session.Type != GAME_NORMAL && House->IsHuman && Combat_Damage(-1) < 0 && !House->Is_Ally(TarCom)) Assign_Target(0);
	if (TarCom && TarCom->Fetch_RTTI() == RTTI_AIRCRAFT) {
		ObjectClass * aircraft = static_cast<ObjectClass *>(TarCom);
		if (Fetch_RTTI() == RTTI_UNIT && Combat_Damage(-1) < 0 && (aircraft->HeightAGL > 0 || Map[aircraft->Get_Coord()].Cell_Building())) Assign_Target(0);
	}
	if (Fetch_RTTI() != RTTI_AIRCRAFT && TarCom && !Is_In_Team()) {
		FootClass * foot = 0;
		if (Is_Foot_Object(this)) foot = static_cast<FootClass *>(this);
		if (!foot || !foot->NavCom) {
			if (!Is_In_Same_Zone_As((ObjectClass *)TarCom)) {
				if (Is_Foot_Object(this)) foot->Approach_Target();
				if (!foot || !foot->NavCom) {
					int primary = What_Weapon_Should_I_Use(TarCom);
					if (!In_Range(TarCom, primary)) Assign_Target(0);
				}
			}
		}
	}
	if (Fetch_RTTI() != RTTI_BUILDING) StageClass::Graphic_Logic();
	unsigned old_flash = FlashCount;
	bool was_flash_on = (old_flash & 2) == 2;
	if (FlasherClass::Process()) Mark(MARK_CHANGE);
	if (old_flash && old_flash != FlashCount && Fetch_RTTI() == RTTI_BUILDING && was_flash_on != ((FlashCount & 2) == 2)) {
		TacticalMap->Register_Dirty_Area(Get_Render_Rect(), false);
		static_cast<BuildingClass *>(this)->Update_Anim_Appearance();
	}
	TechnoTypeClass const * type = Techno_Type_Class();
	if (type->IsDamageSparks && HealthRatio < Rule->ConditionYellow && HeightAGL > -10) {
		DynamicVectorClass<ParticleSystemTypeClass const *> sparks;
		for (int i = 0; i < type->DamageParticleSystems.Count(); ++i) {
			if (type->DamageParticleSystems[i]->BehavesLike == PSYS_BEHAVIOR_SPARK) sparks.Add(type->DamageParticleSystems[i]);
		}
		if (!ParticleSystems[1] && sparks.Count() > 0) {
			double probability = HealthRatio < Rule->ConditionRed ? Rule->ConditionRedSparkingProbability : Rule->ConditionYellowSparkingProbability;
			if (Random_Double(0.0, 1.0) < probability) {
				ParticleSystems[1] = new ParticleSystemClass(const_cast<ParticleSystemTypeClass *>(sparks[Random_Pick(0, sparks.Count() - 1)]), Offset_Coord(Center_Coord(), Techno_Type_Class()->Damage_Smoke_Offset()), 0, this, COORD_NONE);
			}
		}
	}
	Update_Radar_Position(false);
	if (StunDuration > 0 && --StunDuration == 0) {
		if (Fetch_RTTI() == RTTI_BUILDING) {
			BuildingClass * building = static_cast<BuildingClass *>(this);
			if (building && !building->Class->IsInvisibleInGame) { building->Power_On(); if (building->Class->IsRadar) House->RecalcRadar = true; }
		} else {
			bool is_foot = false;
			if (this) is_foot = (AbstractFlags & ABSTRACT_FOOT);
			if (is_foot) {
				FootClass * foot = static_cast<FootClass *>(this);
				if (foot->Locomotion) foot->Locomotion->Power_On();
				for (int i = 0; i < Anims.Count(); ++i) {
					AnimClass * anim = Anims[i];
					if (anim && anim->xObject == this && anim->Class == Rule->EMPulseSparkles) anim->Loops = 0;
				}
			}
		}
	}
}














/// <summary>
/// Adds one round once the reload timer has run out and the object is short of its full ammunition, then
/// starts the timer for the next one.
/// </summary>
void TechnoClass::Reload(void) // RA2: not in TibSun, at 0x6C8290
{
	TechnoTypeClass const * type = Techno_Type_Class();
	if (type->MaxAmmo != -1 && Ammo < type->MaxAmmo && ReloadTimer == 0) {
		Ammo++;
		Mark(MARK_CHANGE);
		Start_Reload_Timer();
	}
}

/// <summary>
/// Starts the reload timer when the object is short of its full ammunition: EmptyReload when it has none
/// left (and the type gives one), else Reload plus ReloadIncrement times the square of the full pip groups.
/// </summary>
void TechnoClass::Start_Reload_Timer(void) // RA2: not in TibSun, at 0x6C8300
{
	int maxammo = Techno_Type_Class()->MaxAmmo;
	if (Ammo < maxammo) {
		if (Ammo == 0 && Techno_Type_Class()->EmptyReload != -1) {
			ReloadTimer = Techno_Type_Class()->EmptyReload;
		} else {
			int groups = Techno_Type_Class()->PipWrap == 0 ? 1 : Ammo / Techno_Type_Class()->PipWrap;
			int delay = Techno_Type_Class()->ReloadIncrement * groups * groups;
			ReloadTimer = Techno_Type_Class()->Reload + delay;
		}
	}
}

/// <summary>
/// Starts this gap generator: shrouds the cells within its radius for everyone but the player's side and
/// counts the gap on each of them.
/// </summary>
void TechnoClass::CreateGap(void) // RA2: not in TibSun; ra2-decomp's twin text
{
	if (PlayerPtr != NULL && !GeneratingGap && IsPowerOnline()) {
		if (GapRadius == 0) {
			GapRadius = TClass->GapRadiusInCells;
		}
		GeneratingGap = true;

		Coord coord = Position;
		coord.X += (Tactical::Z_Lepton_To_Pixel(coord.Z) / -30) << 8;
		coord.Y += (Tactical::Z_Lepton_To_Pixel(coord.Z) / -30) << 8;
		Cell center = coord.As_Cell();

		int radius = GapRadius;
		for (int dx = -2 - radius; dx < radius + 2; dx++) {
			for (int dy = -2 - radius; dy < radius + 2; dy++) {
				if (dx * dx + dy * dy < (radius + 1) * (radius + 1)) {
					Cell cell = center + Cell(dx, dy);
					if (!Owner_HouseClass()->Is_Player_Control() && !House->Is_Ally(PlayerPtr) && !PlayerPtr->IsDefeated) {
						if (Map[cell].ShroudCounter != 1) {
							if (Map[cell].ShroudCounter >= 0) {
								++Map[cell].ShroudCounter;
							}
						}
						++Map[cell].GapsCoveringThisCell;
						if (Map[cell].ShroudCounter >= 1) {
							Map[cell].IsVisible = false;
							Map[cell].IsMapped = false;
						}
					}
					if (Owner_HouseClass()->Is_Player_Control() || House->Is_Ally(PlayerPtr) || PlayerPtr->IsDefeated) {
						++Map[cell].AlliedGapsCoveringThisCell;
					}
				}
			}
		}

		if (PlayerPtr != NULL) {
			PlayerPtr->IsVisionary = false;
		}
		Map.Complete_Radar_Refresh();
		Map.Flag_To_Redraw(GS_REDRAW_ALL);
	}
}

/// <summary>
/// Stops this gap generator: takes its count off the cells within its radius and reveals the ones no gap
/// covers any more when the player's spy satellite is up.
/// </summary>
void TechnoClass::DestroyGap(void) // RA2: not in TibSun; ra2-decomp's twin text
{
	if (PlayerPtr != NULL && GeneratingGap) {
		if (GapRadius == 0) {
			GapRadius = TClass->GapRadiusInCells;
		}
		GeneratingGap = false;

		Coord coord = Position;
		coord.X += (Tactical::Z_Lepton_To_Pixel(coord.Z) / -30) << 8;
		coord.Y += (Tactical::Z_Lepton_To_Pixel(coord.Z) / -30) << 8;
		Cell center = coord.As_Cell();

		int radius = GapRadius;
		for (int dx = -2 - radius; dx < radius + 2; dx++) {
			for (int dy = -2 - radius; dy < radius + 2; dy++) {
				if (dx * dx + dy * dy < (radius + 1) * (radius + 1)) {
					Cell cell = center + Cell(dx, dy);
					if (!Owner_HouseClass()->Is_Player_Control() && !House->Is_Ally(PlayerPtr) && !PlayerPtr->IsDefeated) {
						--Map[cell].GapsCoveringThisCell;
						if (PlayerPtr->SpySatActive && Map[cell].GapsCoveringThisCell <= 0) {
							--Map[cell].ShroudCounter;
							if (Map[cell].ShroudCounter <= 0) {
								Map[cell].IsMapped = true;
								Map[cell].IsVisible = true;
							}
						}
					}
					if (Owner_HouseClass()->Is_Player_Control() || House->Is_Ally(PlayerPtr) || PlayerPtr->IsDefeated) {
						--Map[cell].AlliedGapsCoveringThisCell;
					}
				}
			}
		}

		if (PlayerPtr != NULL) {
			PlayerPtr->IsVisionary = false;
		}
		Map.Complete_Radar_Refresh();
		Map.Flag_To_Redraw(GS_REDRAW_ALL);
	}
}

/// <summary>
/// RA2
/// </summary>
static inline Rect Moved_Rect(Rect const & rect, Point2D const & point)
{
	return(Rect(rect.Top_Left() + point, rect.Width, rect.Height));
}


/***********************************************************************************************
 * TechnoClass::Cloaking_AI -- Perform the AI maintenance for a cloaking device.               *
 *                                                                                             *
 *    This routine handles the cloaking device logic for this object. It will handle the       *
 *    transition effects as the object cloaks or decloaks. It will also try to start an        *
 *    object to cloak if possible.                                                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void TechnoClass::Cloaking_AI(bool)
{
	/*
	**	If this object is uncloaked, but it can be cloaked and it thinks that it
	**	is a good time do so, then begin cloaking.
	*/
	if (Cloak == UNCLOAKED) {
		if ((Is_Allowed_To_Recloak() && !IsUnderEMP() && !IsParalyzed() && !IsBeingWarpedOut() && !IsWarpingIn()) || Has_Ability(ABILITY_CLOAK)) { // RA2: the immobilizing states are tested one by one
			TechnoClass * radio = Radio_Link(0); // RA2: the link is asked for by index
			if (radio == NULL || !(radio->Fetch_RTTI() == RTTI_BUILDING && ((BuildingClass *)radio)->Class->IsWeaponsFactory)) { // RA2: as above
				CloakingDevice.Graphic_Logic();
				if (Is_Ready_To_Cloak()) {
					if (HealthRatio > Rule->ConditionRed) {
						Do_Cloak(false); // RA2: as above
					} else {
						if (Percent_Chance(4)) {
							Do_Cloak(false); // RA2: as above
						}
					}
				}
			}
		}
	} else {
		CloakingDevice.Graphic_Logic();
		if (CloakingDevice.Fetch_Stage() < 0) {
			CloakingDevice.Set_Stage(0);
		}
		switch (Cloak) {
			/*
			**	Handle the uncloaking process. Always mark to redraw
			**	the object and when cloaking is complete, stabilize into
			**	the normal uncloaked state.
			*/
			case UNCLOAKING:
			{
				Mark(MARK_CHANGE);
				switch (Visual_Character(true)) {
					case VISUAL_NORMAL:
						CloakingDevice.Set_Rate(0);
						CloakingDevice.Set_Stage(0);	// re-start the stage counter
						Cloak = UNCLOAKED;
						CloakDelay = Rule->CloakDelay * TICKS_PER_MINUTE;
						Mark(MARK_CHANGE);
						break;

					case VISUAL_INDISTINCT:
						if (Is_Ready_To_Cloak()) {
							Do_Cloak(true);
						}
				}
				break;
			}
			/*
			**	Handle the cloaking process. Always mark to redraw the object
			**	and when the cloaking process is complete, stabilize into the
			**	normal cloaked state.
			*/
			case CLOAKING:
				Mark(MARK_CHANGE);
				if (!CloakingDevice.Fetch_Rate()) {
					CloakingDevice.Set_Rate(1);
				}
				switch (Visual_Character(true)) {
					/*
					**	If badly damaged, then it can never fully cloak.
					*/
					case VISUAL_DARKEN:
						if (HealthRatio <= Rule->ConditionRed && Percent_Chance(10)) {
							Do_Uncloak(true);
						}
						break;
					case VISUAL_SHADOWY:
					case VISUAL_HIDDEN:
						Cloak = CLOAKED;
						CloakingDevice.Set_Rate(0);
						CloakingDevice.Set_Stage(0);
						Mark(MARK_CHANGE);
						/*
						**	Special check to ensure that if the unit is carrying a captured
						**	flag, it will never fully cloak.
						*/
						if (RTTI == RTTI_UNIT && ((UnitClass *)this)->Flagged != HOUSE_NONE) {
							Do_Shimmer();
						} else {
							int i;
							DynamicVectorClass<TechnoClass *> targeting_me;
							for (i = Technos.Count() - 1; i >= 0; i--) {
								TechnoClass * tech = Technos[i];
								if (tech->TarCom == this) {
									if (Map[Center_Coord()].Is_Sensed(tech->House->HeapID) || tech->House == House) {
										targeting_me.Add(tech);
									}
								}
							}
							Detach_All(false);
							for (i = targeting_me.Count() - 1; i >= 0; i--) {
								targeting_me[i]->Assign_Target(this);
							}
						}
						/*
						**	A computer controlled unit will try to scatter if possible so
						**	that it will be much harder to locate.
						*/
						if (RTTI == RTTI_UNIT && !House->Is_Human_Player()) {
							Scatter(COORD_NONE, true);
						}
						break;
				}
				break;
			/*
			**	A cloaked object will always be redrawn if it is owned by the
			**	player. This ensures that the shimmering effect will animate.
			*/
			case CLOAKED:
				if (Should_Uncloak()) {
					Do_Uncloak(false); // RA2: as above
				}
				break;
		}
	}
}



/// <summary>
/// Should this object drop its cloak?
/// An object that has lost the right to stay hidden -- because it may no longer recloak, is
/// not a cloaking type at all, or has been immobilized -- must decloak, unless it carries
/// the cloak ability outright or is standing in cloaking cover.
/// </summary>
/// <returns>bool; Should the object uncloak now?</returns>
bool TechnoClass::Should_Uncloak(void) const
{
	if (!(Is_Allowed_To_Recloak() || IsCloakable) || IsUnderEMP() || IsParalyzed() || IsBeingWarpedOut() || IsWarpingIn()) { // RA2: also a paralyzed, warping object uncloaks
		bool allowed = true; // RA2: the cloak ability, as in Is_Ready_To_Cloak
		if (Veterancy.Is_Veteran() || Veterancy.Is_Elite()) { // RA2: as above
			const TechnoTypeClass *type = Techno_Type_Class(); // RA2: as above
			if (Veterancy.Is_Veteran() && ((const bool *)&type->VeteranAbilities)[6]) { // RA2: as above
				allowed = false; // RA2: as above
			} else if (Veterancy.Is_Elite() && (((const bool *)&type->VeteranAbilities)[6] || ((const bool *)&type->EliteAbilities)[6])) { // RA2: as above
				allowed = false; // RA2: as above
			}
		}
		if (allowed && !((CellClass *)&Map[Center_Coord().As_Cell()])->Is_Cloaked(House->HeapID)) { // RA2: as above
			return(true);
		}
	}
	return(false);
}



/***********************************************************************************************
 * TechnoClass::Is_Ready_To_Cloak -- Determines if this object is ready to begin cloaking.     *
 *                                                                                             *
 *    This routine will examine this object and determine if it can and is ready and able      *
 *    to begin cloaking. It will also check to make sure it appears to be a good time to cloak *
 *    as well.                                                                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Is this unit ready and able to start cloaking?                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoClass::Is_Ready_To_Cloak(void) const
{
	if (!Is_Allowed_To_Recloak()) { // RA2: RA2's form
		bool allowed = true; // RA2: as above
		if (Veterancy.Is_Veteran() || Veterancy.Is_Elite()) { // RA2: as above
			const TechnoTypeClass *type = Techno_Type_Class(); // RA2: as above
			if (Veterancy.Is_Veteran() && ((const bool *)&type->VeteranAbilities)[6]) { // RA2: as above
				allowed = false; // RA2: as above
			} else if (Veterancy.Is_Elite() && (((const bool *)&type->VeteranAbilities)[6] || ((const bool *)&type->EliteAbilities)[6])) { // RA2: as above
				allowed = false; // RA2: as above
			}
		}
		if (allowed && !((CellClass *)&Map[Center_Coord().As_Cell()])->Is_Cloaked(House->HeapID) // RA2: as above
			&& !IsCloakable) { // RA2: as above
			return(false);
		}
	}

	/*
	**	If it is already cloaked or in the process of cloaking, then it can't start cloaking.
	*/
	if (Cloak == CLOAKED) {
		return(false);
	}
	if (Arm.Value() != 0) { // RA2: as above
		return(false);
	}
	if (TarCom != 0 && IsCloseEnoughToAttack(TarCom)) { // RA2: as above
		return(false);
	}

	/*
	**	Recloaking can only begin if the cloaking device is not already operating.
	*/
	if (RTTI != RTTI_BUILDING && CloakingDevice.Fetch_Stage() != 0) {
		return(false);
	}
	if (CloakDelay.Value() != 0) { // RA2: as above
		return(false);
	}

	/*
	**	All tests passed, so this object is allowed to begin cloaking.
	*/
	return(true);
}




/***********************************************************************************************
 * TechnoClass::Select -- Selects object and checks to see if can be selected.                 *
 *                                                                                             *
 *    This function checks to see if this techno object can be selected. If it can, then it    *
 *    is selected.                                                                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/11/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoClass::Select(void)
{
	if ((IsDiscoveredByPlayer || House->Is_Player_Control() || !MainWindow) && BASECLASS::Select()) { // RA2: RA2's form
		if (Tag) { // RA2: as above
			Tag->Spring(TEVENT_SELECTED, this, CELL_NONE, 0, 0); // RA2: as above
		}

		/*
		**	Speak a confirmation of selection.
		*/
		if (House->Is_Player_Control() && AllowVoice) {
			Response_Select();
		}
		Select_Planning_Members(); // RA2: as above
		return(true);
	}
	return(false);
}


/// <summary>
/// Why the weapon cannot fire at the target, range aside.
/// </summary>
int TechnoClass::GetFireErrorWithoutRange(AbstractClass * target, int which) const // RA2: not in TibSun, at 0x6C9240
{
	return(Can_Fire(target, which, false));
}








/***********************************************************************************************
 * TechnoClass::Can_Fire -- Determines if this techno object can fire.                         *
 *                                                                                             *
 *    This performs a simple check to make sure that this techno object can fire. At this      *
 *    level, the only thing checked for is the rearming delay.                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the fire legality control code.                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/23/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
FireErrorType TechnoClass::Can_Fire(AbstractClass * target, int which, bool check_range) const
{
	TechnoClass * techno;
	CellClass * cellptr;
	WeaponTypeClass const * weapon;
	WeaponTypeClass const * other;
	bool water;
	bool check_rearm;
	FootClass * foot;
	bool parasite;
	if (target == 0) goto ILLEGAL;
	if (IsWarpingIn()) return FIRE_REARM;
	if (IsBeingWarpedOut()) goto ILLEGAL;
	if (IsSinking) goto ILLEGAL;
	if (IsWarpingSomethingOut() && TemporalImUsing && target == TemporalImUsing->Target) return FIRE_REARM;
	techno = (target->AbstractFlags & ABSTRACT_TECHNO) ? static_cast<TechnoClass *>(target) : 0;
	cellptr = reinterpret_cast<CellClass *>(&Map[target->Center_Coord()]);
	if (techno) {
		if (techno->IsInLimbo) goto ILLEGAL;
		if (techno->IsImmobilized) goto ILLEGAL;
		if (techno->Visual_Character(true, House) == VISUAL_HIDDEN && !cellptr->Is_Sensed(House->HeapID)) {
			if (Combat_Damage(-1) > 0) goto CANT;
			if (!techno->House->Is_Ally(House)) goto CANT;
		}
	}
	if (IsFalling) goto ILLEGAL;
	if (IsUnderEMP()) {
		if (Fetch_RTTI() != RTTI_UNIT) goto CANT;
		UnitClass const * unit = static_cast<UnitClass const *>(this);
		if (!unit->Class->IsLargeVisceroid && !unit->Class->IsSmallVisceroid) goto CANT;
	}
	weapon = Get_Class_Weapon_Data(which)->Weapon;
	if (weapon != 0) {
		if (weapon->IsIonSensitive && House->Ion_Sensitives_Should_Be_Offline()) goto CANT;
		if (techno && weapon->WarheadPtr && techno->IsBeingWarpedOut() && !weapon->WarheadPtr->Temporal) goto ILLEGAL;
		if (weapon->Spawner) {
			if (Is_Z_Fudge_Bridge()) goto CANT;
			if (IsParalyzed()) goto CANT;
			if (SpawnManager->CountAliveSpawns() == 0) return FIRE_REARM;
		}
		other = Get_Class_Weapon_Data(which == 0)->Weapon;
		if (other && ((other->UseFireParticles && ParticleSystems[0]) || (other->IsRailgun && ParticleSystems[4]) || (other->UseSparkParticles && ParticleSystems[1]) || (other->IsSonic && Wave))) goto CANT;
		if (target->In_Air() && !weapon->Bullet->IsAntiAircraft) goto ILLEGAL;
		if (techno) {
			water = (techno->Get_Cell_Ptr()->Land_Type() == LAND_WATER)
                || (techno->Get_Cell_Ptr()->Land_Type() == LAND_BEACH);
			if (techno->In_Air()) water = false;
			if (techno->IsOnBridge) water = false;
			else if (water && SelectNavalTargeting(static_cast<ObjectClass *>(target)) == -1) goto ILLEGAL;
			if (target->On_Ground() && !water && Techno_Type_Class()->LandTargeting == 1) return FIRE_ILLEGAL;
		} else {
			if (!target->In_Air() && !weapon->Bullet->IsAntiGround) return FIRE_ILLEGAL;
			if (target->Fetch_RTTI() == RTTI_CELL && ((CellClass *)(target->Fetch_RTTI() == RTTI_CELL ? target : 0))->Land_Type() != LAND_WATER && ((CellClass *)(target->Fetch_RTTI() == RTTI_CELL ? target : 0))->Land_Type() != LAND_BEACH && Techno_Type_Class()->LandTargeting == 1) goto ILLEGAL;
		}
		check_rearm = true;
		if (which == 0 && this && Fetch_RTTI() == RTTI_UNIT) {
			UnitClass const * unit = static_cast<UnitClass const *>(this);
			int burst = BurstIndex % weapon->Burst;
			if (burst < 2 && unit->Class->FiringSyncFrame[burst] != -1 && unit->FiringSyncDelay != -1) {
				if (unit->FiringSyncDelay != unit->Class->FiringSyncFrame[burst]) return FIRE_REARM;
				check_rearm = false;
			}
		}
		if (check_rearm && Arm.Value() != 0) return FIRE_REARM;
		if (weapon->UseFireParticles && ParticleSystems[0]) return FIRE_REARM;
		if (weapon->IsRailgun && ParticleSystems[4]) return FIRE_REARM;
		if (weapon->UseSparkParticles && ParticleSystems[1]) return FIRE_REARM;
		if (weapon->IsSonic && Wave) return FIRE_REARM;
		if (Ammo == 0) return FIRE_AMMO;
		if (weapon->DecloakToFire && Cloak != UNCLOAKED && (Fetch_RTTI() != RTTI_AIRCRAFT || Cloak == CLOAKED)) return FIRE_CLOAKED;
		if (Techno_Type_Class()->IsHunterSeeker) return FIRE_RANGE;
		if (weapon->WarheadPtr->Parasite && this && (AbstractFlags & ABSTRACT_FOOT)) {
			foot = (target->AbstractFlags & ABSTRACT_FOOT) ? static_cast<FootClass *>(target) : 0;
			if (!static_cast<FootClass const *>(this)->ParasiteImUsing->CanInfect(foot)) return FIRE_ILLEGAL;
		}
		parasite = weapon->WarheadPtr->Parasite;
		if (parasite && (target->AbstractFlags & ABSTRACT_FOOT) && Frame < static_cast<FootClass *>(target)->LastBeParasitedStartFrame) return FIRE_ILLEGAL;
		if (techno) {
			if (parasite && techno->Unknown_Virtual_32C()) return FIRE_ILLEGAL;
			if (weapon->WarheadPtr->MindControl && !reinterpret_cast<CaptureManagerClass *>(CaptureManager)->Can_Capture(techno)) return FIRE_ILLEGAL;
			WarheadTypeClass const * warhead = weapon->WarheadPtr;
			if (warhead->Modifier[techno->Techno_Type_Class()->Armor] == 0.0) return FIRE_ILLEGAL;
			if (warhead->BombDisarm && techno->AttachedBomb == 0) return FIRE_ILLEGAL;
			if (warhead->IvanBomb && techno->AttachedBomb != 0) return FIRE_ILLEGAL;
			if (techno->IsSinking) return FIRE_ILLEGAL;
			if (IsOnBridge != techno->IsOnBridge) {
				if (Get_Cell_Ptr() && Get_Cell_Ptr()->IsUnderBridge && !In_Air()
                    && techno->Get_Cell_Ptr() && techno->Get_Cell_Ptr()->IsUnderBridge) return FIRE_ILLEGAL;
				int mine = PositionCoord.Z;
				int theirs = techno->PositionCoord.Z;
				if (weapon->WarheadPtr->Parasite && abs(mine - theirs) > LEVEL_LEPTON_H * 2) return FIRE_ILLEGAL;
			}
		}
		if (Techno_Type_Class()->Organic && IsParalyzed()) return FIRE_ILLEGAL;
		if (check_range && !In_Range(target, which)) return FIRE_RANGE;
		return FIRE_OK;
	}
CANT:
	return FIRE_CANT;
ILLEGAL:
	return FIRE_ILLEGAL;
}


/***********************************************************************************************
 * TechnoClass::Stun -- Prepares the object for removal from the game.                         *
 *                                                                                             *
 *    This routine handles cleaning up this techno object from the game system so that when    *
 *    it is subsequently removed, it doesn't leave any loose ends.                             *
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
void TechnoClass::Stun(void)
{
	Assign_Target(NULL); // RA2: clears target, then destination (TibSun's order differs)
	Assign_Destination(NULL, true); // RA2: as above
	SendToEachLink(RADIO_OVER_OUT); // RA2: out message to every link
	if (SpawnManager) { // RA2: spawner nodes are killed and the target abandoned
		SpawnManager->KillNodes(); // RA2: as above
		SpawnManager->ResetTarget(); // RA2: as above
	}
	Detach_All(true); // RA2: as above
	Unselect(); // RA2: as above
}



/***********************************************************************************************
 * TechnoClass::Assign_Target -- Assigns the targeting computer with specified target.         *
 *                                                                                             *
 *    Use this routine to set the targeting computer for this object. It checks to make sure   *
 *    that targeting of itself is prohibited.                                                  *
 *                                                                                             *
 * INPUT:   target   -- The target for this object to attack.                                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/23/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void TechnoClass::Assign_Target(AbstractClass * target)
{
	ShouldLoseTargetNow = false; // RA2: cleared on every assignment

	if (target == TarCom) return;

	if (target == NULL) {
		target = NULL;
	} else {

		/*
		**	Prevent targeting of self.
		*/
		if (target == this) {
			target = &Map[(Coord const &)PositionCoord];
		} else if (target->AbstractFlags & ABSTRACT_OBJECT) { // RA2: only objects are tested; the flag byte says object, not As_Object()

			/*
			**	Make sure that the target is not already dead.
			*/
			ObjectClass * object = (ObjectClass *)target; // RA2: as above
			if (object->IsActive == false || object->Strength == 0) { // RA2: the object flag byte, not As_Object(), says it is an object
				target = NULL;
			} else {
				bool is_foot = (target->AbstractFlags >> 2) & 1; // RA2: a sinking foot or a dying infantry is no target
				TechnoClass * techno; // RA2: assigned inside the condition below, as the executable does
				if (is_foot && (techno = (TechnoClass *)target)->IsSinking) { // RA2: as above
					target = NULL; // RA2: as above
				}
				if (is_foot && techno->Fetch_RTTI() == RTTI_INFANTRY && ((InfantryClass *)techno)->Is_Dying()) { // RA2: as above
					target = NULL; // RA2: as above
				}
			}
		}
	}

	/*
	**	Set the unit's targeting computer.
	*/
	TarCom = target;

	if (SpawnManager != NULL && target == NULL) { // RA2: a spawner forgets its target when the techno has none
		SpawnManager->SetTarget(target); // RA2: as above
	}

	if (TarCom == NULL) { // RA2: reads the field again
		BurstIndex = 0;
	}

	if (ParticleSystems[ATTACHED_PARTICLE_FIRE]) {
		if (!IsCloseEnoughToAttack(target)) { // RA2: virtual slot 213, not In_Range
			ParticleSystems[ATTACHED_PARTICLE_FIRE]->Delete_Me();
			ParticleSystems[ATTACHED_PARTICLE_FIRE] = NULL;
		}
	}
}



/***********************************************************************************************
 * TechnoClass::Rearm_Delay -- Calculates the delay before firing can occur.                   *
 *                                                                                             *
 *    This function calculates the delay between shots. It determines this from the standard   *
 *    rate of fire (ROF) of the base class and modifies it according to game speed and         *
 *    whether this is the first or second shot. All single shot attackers consider their       *
 *    shots to be "second" since the second shot is the one handled normally. The first shot   *
 *    usually gets assigned a much shorter delay time before the next shot can fire.           *
 *                                                                                             *
 * INPUT:   second   -- bool; Is this the second of a two shot salvo?                          *
 *                                                                                             *
 * OUTPUT:  Returns with the number of game frames to delay before the next shot may fire.     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/26/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
int TechnoClass::Rearm_Delay(int which) const
{
	if (Fetch_RTTI() == RTTI_BUILDING && Ammo > 1) { // RA2: the type is asked by the virtual
		return(1);
	}

	WeaponTypeClass const * weapon = Get_Class_Weapon_Data(which)->Weapon;
	if (weapon == NULL) {
		return(1);
	}

	if (weapon->IsSonic
		|| (weapon->UseSparkParticles && ParticleSystems[ATTACHED_PARTICLE_SPARK])
		|| (weapon->UseFireParticles && ParticleSystems[ATTACHED_PARTICLE_FIRE])
		|| (weapon->IsRailgun && ParticleSystems[ATTACHED_PARTICLE_RAILGUN])) {

		return(weapon->ROF);
	}

	UnitClass const * unit; // RA2: a unit's type has its own burst delays
	if (this == NULL) { // RA2: as above
		unit = NULL; // RA2: as above
	} else {
		unit = (Fetch_RTTI() == RTTI_UNIT) ? (UnitClass const *)this : NULL; // RA2: as above
	}
	if (BurstIndex < weapon->Burst) {
		int delay = -1;
		if (BurstIndex > 0 && BurstIndex <= 4 && unit) { // RA2: BurstDelay of the unit's type
			delay = unit->Class->BurstDelay[BurstIndex - 1]; // RA2: as above
		}
		return(delay != -1 ? delay : Random_Pick(3, 5)); // RA2: as above

	} else {
		int delay = weapon->ROF * House->ROFBias + Random_Pick(0, 2);

		if (Veterancy.Is_Veteran() || Veterancy.Is_Elite()) { // RA2: the ROF ability, as in Is_Ready_To_Cloak (index 4)
			TechnoTypeClass const * type = Techno_Type_Class(); // RA2: as above
			if ((Veterancy.Is_Veteran() && ((const bool *)&type->VeteranAbilities)[4]) // RA2: as above
				|| (Veterancy.Is_Elite() && (((const bool *)&type->VeteranAbilities)[4] || ((const bool *)&type->EliteAbilities)[4]))) { // RA2: as above
				delay = delay * Rule->VeteranROF; // RA2: as above
			}
		}
		if (Fetch_RTTI() == RTTI_BUILDING && ((BuildingClass const *)this)->GetOccupantCount() > 0) { // RA2: a garrison fires faster
			delay /= ((BuildingClass const *)this)->GetOccupantCount(); // RA2: as above
		}
		return(delay);
	}
}



/***********************************************************************************************
 * TechnoClass::Laser_Zap -- Fires laser zap at the target specified.                          *
 *                                                                                             *
 *    This routine is used to fire a laser zap at the target specified.                        *
 *                                                                                             *
 * INPUT:   target   -- The target to fire the zap at.                                         *
 *                                                                                             *
 *          which    -- Which weapon is this zap associated with (0=primary, 1=secondary).     *
 *                                                                                             *
 *          window      -- The clipping window to use when rendering.                          *
 *                                                                                             *
 *          source_coord   -- The coordinate that the zap is to originate from. This is an     *
 *                            override value and if not specifide, the normal fire coordinate  *
 *                            is used.                                                         *
 *                                                                                             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/30/1996 BWG : Created.                                                                 *
 *   09/30/1996 JLB : Uses standard facing conversion and distance routines.                   *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
LaserDrawClass * TechnoClass::Laser_Zap(AbstractClass * target, int which, WeaponTypeClass const * weapon, Coord const & source_coord)
{
	RGBClass half; // RA2: the house colour halved, zeroed on entry
	RGBClass const * outer; // RA2: the bolt colors are kept by pointer
	RGBClass const * inner; // RA2: as above
	int duration = weapon->LaserDuration; // RA2: as above
	if (weapon->IsHouseColor) { // RA2: a house coloured laser: inner is the house's laser colour, outer half of it
			inner = &House->LaserColor; // RA2: a house coloured laser: inner is the house's laser colour, outer half of it
		half.Set(inner->Get_Red() >> 1, inner->Get_Green() >> 1, inner->Get_Blue() >> 1); // RA2: as above
		outer = &half; // RA2: as above
	} else {
		inner = &weapon->LaserInnerColor; // RA2: the weapon's own colours
		outer = &weapon->LaserOuterColor; // RA2: as above
	}
	Coord source;
	Coord dest;
	int zadjust = 0;
	if (source_coord != COORD_NONE) {
		source = source_coord;
	} else {
		source = Fire_Coord(which);
		if (Fetch_RTTI() == RTTI_BUILDING) { // RA2: only a building's beam is kept above its own render row
			Point2D p1 = TacticalMap->Coord_To_Pixel_Absolute(source); // RA2: as above
			Point2D p2 = TacticalMap->Coord_To_Pixel_Absolute(Render_Coord()); // RA2: as above
			zadjust = p1.Y; // RA2: as above
			zadjust -= p2.Y; // RA2: as above
			if (zadjust > 0) { // RA2: as above
				zadjust = 0; // RA2: as above
			}
		}
	}
	if (target && (target->AbstractFlags & 2)) { // RA2: an object gives its target coordinate
		dest = ((ObjectClass *)target)->Target_Coord(); // RA2: as above
	} else {
		dest = target->As_Coord(); // RA2: else its own coordinate
	}
	LaserDrawClass * laser = new LaserDrawClass(source, dest, zadjust, true, *inner, *outer, weapon->LaserOuterSpread, duration, false, true, 1.0f, 0.0f); // RA2: no wave, fades
	if (laser && weapon->IsHouseColor) { // RA2: the beam remembers it is house coloured
		laser->IsHouseColor = true; // RA2: as above
	}
	return laser;
}
#endif





/// <summary>
/// Fires an electric bolt from the source coordinate at the target. A building's bolt is drawn no lower
/// than the point it starts from.
/// </summary>
EBolt * TechnoClass::Electric_Zap(AbstractClass * target, WeaponTypeClass const * weapon, Coord const & source_coord) // RA2: not in TibSun, at 0x6CA020
{
	int zadjust = 0;
	EBolt * bolt = new EBolt;
	if (bolt == NULL) {
		return(NULL);
	}

	if (RTTI == RTTI_BUILDING) {
		Point2D p1 = TacticalMap->Coord_To_Pixel_Absolute(source_coord);
		Point2D p2 = TacticalMap->Coord_To_Pixel_Absolute(Render_Coord());
		zadjust = p1.Y - p2.Y;
		if (zadjust > 0) {
			zadjust = 0;
		}
	}

	Coord dest;
	if (target != NULL && (target->AbstractFlags & 2)) {
		dest = ((ObjectClass *)target)->Target_Coord();
	} else {
		dest = target->As_Coord();
	}

	if (weapon != NULL) {
		bolt->AlternateColor = weapon->IsAlternateColor;
	}
	bolt->Fire(source_coord, dest, zadjust);
	return(bolt);
}


/// <summary>
/// Fires an electric bolt from the weapon this object would use against the target. An active unit
/// without a bolt keeps this one as its own.
/// </summary>
EBolt * TechnoClass::Electric_Zap(AbstractClass * target) // RA2: not in TibSun, at 0x6CA130
{
	int which = What_Weapon_Should_I_Use(target);
	WeaponTypeClass * weapon = Get_Class_Weapon_Data(which)->Weapon;
	Coord source = Fire_Coord(which);
	EBolt * bolt = Electric_Zap(target, weapon, source);
	if (bolt != NULL && RTTI == RTTI_UNIT && IsActive && ((UnitClass *)this)->ElectricBolt == NULL) {
		((UnitClass *)this)->ElectricBolt = bolt;
		bolt->Set_Owner(this, which);
	}
	return(bolt);
}


/// <summary>
/// Draws a rad beam (or, for the chrono kind, a chrono beam) from the primary weapon's firing point to
/// the target.
/// </summary>
void TechnoClass::Rad_Zap(AbstractClass * target, int kind) // RA2: not in TibSun, at 0x6CA1D0
{
	RadBeam * beam = RadBeam::Allocate(1);
	int zadjust = 0;
	Coord source = Fire_Coord(0);
	Coord render = Render_Coord();
	if (source.Y != render.Y) {
		Point2D p1 = TacticalMap->Coord_To_Pixel_Absolute(source);
		Point2D p2 = TacticalMap->Coord_To_Pixel_Absolute(render);
		zadjust = p1.Y - p2.Y;
	}

	Coord dest;
	bool object = target != NULL && (target->AbstractFlags & ABSTRACT_OBJECT);
	if (!object) {
		dest = target->As_Coord();
	} else {
		dest = ((ObjectClass *)target)->Target_Coord();
	}

	RGBClass color;
	switch (kind) {
		case 1:
			color = Rule->ChronoBeamColor;
			break;
		default:
			color = Rule->RadColor;
			break;
	}
	beam->Set_Source(source);
	beam->Set_Target(dest);
	beam->Set_Period(15);
	beam->Set_Amplitude(40.0);
	beam->Set_Color(color);
	beam->Set_Depth(zadjust);
}


/// <summary>
/// Throws rad beams from this object's center to random points in the eight cells around it, the given
/// number of cells away.
/// </summary>
void TechnoClass::Rad_Eruption(int spread) // RA2: not in TibSun, at 0x6CA340
{
	Coord center = Center_Coord();
	int longest = 0;
	Cell cell = center.As_Cell();
	RGBClass color = Rule->RadColor;
	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			if (x != 0 || y != 0) {
				int ox = spread * x;
				Cell offset(ox, spread);
				offset.Y *= y; // RA2: this spelling (not Cell(ox, spread * y)) keeps both multiplies the original has
				Cell spot = offset + cell;
				Coord to = Map[spot].Cell_Coord();
				int dx = NonCriticalRandomNumber(-128, 128);
				int dy = NonCriticalRandomNumber(-128, 128);
				to.X += dx;
				to.Y += dy;
				RadBeam * beam = RadBeam::Allocate(2);
				beam->Set_Color(color);
				int period = NonCriticalRandomNumber(5, 20);
				int amplitude = NonCriticalRandomNumber(100, 500);
				int delay = NonCriticalRandomNumber(0, 20);
				beam->Set_Source(center);
				beam->Set_Target(to);
				beam->Set_Period(period);
				beam->Set_Amplitude(amplitude);
				beam->Set_Phase(0.0);
				beam->Set_Start_Delay(delay);
				if (longest <= period) {
					longest = period; // RA2: computed, never used
				}
			}
		}
	}
}

















/// <summary>
/// Fetches the barrel elevation needed to hit a target.
/// This routine is used when aiming a weapon that lobs its shot, solving the ballistic arc
/// to where the target is predicted to be by the time the shot arrives. When no arc will
/// reach, a fixed fallback pitch is used rather than aiming flat.
/// </summary>
/// <param name="target">The target to aim at. If NULL, the resting fire angle is used.</param>
/// <returns>Returns with the pitch to set the barrel to.</returns>
DirType TechnoClass::Barrel_Pitch(AbstractClass * target) const
{
	DirType dir((unsigned short)(Techno_Type_Class()->FireAngle << 8)); // RA2: RA2's form
	if (target != 0) { // RA2: as above
		Coord end = Predict_Target_Coord(); // RA2: as above
		int weapon; // RA2: as above
		if (Techno_Type_Class()->Has_Turret_Weapons()) { // RA2: as above
			weapon = CurrentWeaponNumber; // RA2: as above
		} else { // RA2: as above
			weapon = 0; // RA2: as above
		}
		Coord delta = end - Turret_Coord(weapon); // RA2: as above
		WeaponTypeClass *type = Get_Class_Weapon_Data(weapon)->Weapon; // RA2: as above
		double speed = Rule->Gravity; // RA2: as above
		if (type->Bullet != 0 && type->Bullet->IsFloater) { // RA2: as above
			speed = Get_Floater_Gravity(); // RA2: as above
		}
		int distance = (int)fastmath::sqrt(pow(delta.X, 2.0) + pow(delta.Y, 2.0)); // RA2: as above
		int velocity = type->Get_Speed(distance); // RA2: as above
		if (!Calculate_Projectile_Pitch(Should_Use_High_Arc(weapon), velocity, distance, delta.Z, speed, dir)) { // RA2: as above
			dir = DIR_NW; // RA2: as above
		}
	}
	return dir; // RA2: as above
}



/// <summary>
/// The damage the weapon would do to the target: the attack scaled by both houses' and both objects'
/// biases and veterancy, then by the target's armor. Sonic and fire-particle weapons count as none.
/// </summary>
int TechnoClass::Expected_Damage(TechnoClass * target, WeaponTypeClass const * weapon) // RA2: not in TibSun, at 0x6CA6C0
{
	if (target == NULL) {
		return(0);
	}
	int damage = weapon->Attack;
	if (weapon->IsSonic || weapon->UseFireParticles) {
		return(0);
	}
	if (damage > 0) {
		damage = House->FirepowerBias * FirepowerBias * damage;
		if (Veterancy.Is_Veteran() || Veterancy.Is_Elite()) {
			TechnoTypeClass const * type = Techno_Type_Class();
			if ((Veterancy.Is_Veteran() && type->VeteranAbilities[ABILITY_FIREPOWER])
				|| (Veterancy.Is_Elite() && (type->VeteranAbilities[ABILITY_FIREPOWER] || type->EliteAbilities[ABILITY_FIREPOWER]))) {
				damage = damage * Rule->VeteranCombat;
			}
		}
		double value = damage;
		damage = value / (target->House->Armor_Multiplier(Techno_Type_Class()) * ArmorBias);
		if (target->Veterancy.Is_Veteran() || target->Veterancy.Is_Elite()) {
			TechnoTypeClass const * type = target->Techno_Type_Class();
			if ((target->Veterancy.Is_Veteran() && type->VeteranAbilities[ABILITY_STRONGER])
				|| (target->Veterancy.Is_Elite() && (type->VeteranAbilities[ABILITY_STRONGER] || type->EliteAbilities[ABILITY_STRONGER]))) {
				damage = damage / Rule->VeteranArmor;
			}
		}
		if (damage < 1) {
			damage = 1;
		}
		damage = Modify_Damage(damage, weapon->WarheadPtr, target->Class_Of()->Armor, 0);
	}
	return(damage);
}








/// <summary>
/// Fetches the marker rectangle for a target laser end point.
/// This routine is used by the target laser display to build the small block that is
/// drawn over the aim point.
/// </summary>
/// <param name="point">The screen pixel that the marker should be centered upon.</param>
/// <returns>Returns with the marker rectangle, centered about the pixel specified.</returns>
static inline Rect Target_Laser_Rect(Point2D const & point)
{
	return(Rect(point - Point2D(2, 2), 3, 3));
}


/***********************************************************************************************
 * TechnoClass::Fire_At -- Fires projectile at target specified.                               *
 *                                                                                             *
 *    This is the main projectile firing code. Buildings, units, and infantry route fire       *
 *    requests through this function.                                                          *
 *                                                                                             *
 * INPUT:   target   -- The target that the projectile is to be fired at.                      *
 *                                                                                             *
 *          which    -- Which weapon to fire.                                                  *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the projectile object that was fired. If no projectile   *
 *          could be created or there was some other illegality detected, the return value     *
 *          will be NULL.                                                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/26/1994 JLB : Created.                                                                 *
 *   07/03/1995 JLB : Moving platforms fire inaccurate projectiles.                            *
 *   02/22/1996 JLB : Handles camera "weapon" case.                                            *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: 4596 bytes differ (RA2 firing paths restored from main and the TibSun archive)
BulletClass * TechnoClass::Fire_At(AbstractClass * target, int which)
{
	WeaponTypeClass * weapon = Get_Class_Weapon_Data(which)->Weapon;
	if (weapon == 0) {
		return 0;
	}
	ObjectClass * object = target == 0 ? 0 : ((target->AbstractFlags & ABSTRACT_OBJECT) ? static_cast<ObjectClass *>(target) : 0);
	BulletTypeClass const * btype = weapon->Bullet;
	if (Debug_Map || target == 0 || (object != 0 && object->IsInLimbo)) {
		return 0;
	}
	if (weapon->Suicide) {
		int damage = TClass->MaxStrength;
		Take_Damage(damage, 0, Rule->C4Warhead, 0, true, false);
		return 0;
	}
	if ((weapon->UseFireParticles && ParticleSystems[ATTACHED_PARTICLE_FIRE] != 0) || (weapon->IsRailgun && ParticleSystems[ATTACHED_PARTICLE_RAILGUN] != 0)
		|| (weapon->UseSparkParticles && ParticleSystems[ATTACHED_PARTICLE_SPARK] != 0) || (weapon->IsSonic && Wave != 0)) {
		return 0;
	}
	// RA2: A spawn launcher assigns its target and reveals its position without creating a projectile.
	if (weapon->Spawner) {
		SpawnManager->SetTarget(target);
		if (IsOwnedByPlayer || IsDiscoveredByPlayer) {
			if (!Map.Is_Shrouded(Center_Coord()) && !Map.Is_Fogged(Center_Coord())) {
				return 0;
			}
			if (RTTI == RTTI_AIRCRAFT && IsOwnedByPlayer) {
				return 0;
			}
		}
		if (object != 0) {
			HouseClass * owner = object->Owner_HouseClass();
			if (owner != 0 && owner->Is_Player_Control() && weapon->RevealOnFire) {
				Map.RevealArea1(Center_Coord(), 3, owner, false, false, false, true, false);
			}
			Map.RevealArea3(Center_Coord(), 0, 4, 0);
		}
		return 0;
	}
	TVelocity3D<double> velocity;
	Coord target_coord;
	if (object != 0) {
		target_coord = object->Target_Coord();
	} else {
		target_coord = target->As_Coord();
	}
	Coord fire_coord = Fire_Coord(which);
	DirType dir;
	if (btype->ROT == 0 && !btype->IsDropping) {
		dir = fastmath::atan2((double)fire_coord.Y - (double)target_coord.Y, (double)target_coord.X - (double)fire_coord.X);
	} else {
		dir = Fire_Direction();
		if (btype->IsDropping) {
			fire_coord = Center_Coord();
		}
	}
	int attack = weapon->Attack;
	int firepower = attack;
	if (weapon->IsSonic || weapon->UseFireParticles) {
		firepower = 0;
	}
	if (firepower > 0) {
		firepower = (int)(House->FirepowerBias * FirepowerBias * attack);
		if (Has_Ability(ABILITY_FIREPOWER)) {
			firepower = (int)(firepower * Rule->VeteranCombat);
		}
	}
	Point2D delta = Point2D(target_coord.X, target_coord.Y) - Point2D(fire_coord.X, fire_coord.Y);
	int distance = (int)fastmath::sqrt((double)(delta.X * delta.X + delta.Y * delta.Y));
	int speed = weapon->Get_Speed(distance);
	BulletClass * bullet = weapon->Bullet->Create_Bullet(target, this, firepower, weapon->WarheadPtr, speed, weapon->IsBright);
	if (bullet != 0) {
		bullet->Set_Weapon_Type(weapon);
		bullet->Limbo();
		if (Is_Foot()) {
			if (static_cast<FootClass *>(this)->Locomotion->Is_Moving() && !TClass->JumpJet) {
				bullet->IsInaccurate = true;
			}
		}
		if (!bullet->IsInaccurate && !btype->IsInaccurate) {
			TechnoClass * techno = TarCom == 0 ? 0 : ((TarCom->AbstractFlags & ABSTRACT_TECHNO) ? static_cast<TechnoClass *>(TarCom) : 0);
			WeaponTypeClass * bullet_weapon = bullet->Get_Weapon_Type();
			if (techno != 0 && bullet_weapon != 0) {
				techno->EstimatedHealth -= Expected_Damage(techno, bullet_weapon);
			}
		}
		Coord displacement = Predict_Target_Coord() - fire_coord;
		if (btype->IsInaccurate && btype->IsArcing) {
			if (!btype->FlakScatter || btype->IsInvisible) {
				int spread = Scen->RandomNumber(Rule->BallisticScatter / 2, Rule->BallisticScatter);
				DirType facing(Random_Double(0.0, M_PI * 2));
				displacement = Move_Coord(displacement, facing, spread);
			} else {
				float length = Vector3((float)displacement.X, (float)displacement.Y, (float)displacement.Z).Length();
				int spread = Random_Pick(0, Rule->BallisticScatter);
				spread *= (int)length;
				spread /= Weapon_Range(which);
				DirType facing(Random_Double(0.0, M_PI * 2));
				displacement = Move_Coord(displacement, facing, spread);
			}
		}
		if (btype->ROT == 0 && !btype->IsDropping) {
			double angle = fastmath::atan2((double)-displacement.Y, (double)displacement.X);
			dir = DirType(angle);
		} else {
			dir = Fire_Direction();
			if (btype->IsDropping) {
				fire_coord = Center_Coord();
			}
		}
		if (speed > displacement.Length() / 2) {
			speed = displacement.Length() / 2;
		}
		// RA2: Successive radial shots advance around the current turret facing.
		int segments = TClass->RadialFireSegments;
		BulletTypeClass const * projectile = weapon->Bullet;
		if (projectile->ROT > 0 || projectile->Vertical) {
			if (segments == 0) {
				speed = 1;
			}
			bullet->MaxSpeed = weapon->MaxSpeed;
		}
		velocity.Set(100.0, 0.0, 0.0);
		velocity.Set_Speed((double)speed);
		if (segments > 0) {
			double angle = (2.0 * M_PI / segments) * RadialFireSegment - M_PI / 2.0;
			velocity.Set_Yaw(DirType(PrimaryFacing.Current().As_Radian() + angle));
			RadialFireSegment++;
			if (RadialFireSegment >= segments) {
				RadialFireSegment = 0;
			}
		} else {
			velocity.Set_Yaw(dir);
		}
		int height = displacement.Z;
		DirType pitch(DirType(DIR_E).As_Int() - 1); // RA2: one raw direction unit below level flight
		bool valid = true;
		if (bullet->Class->IsArcing) {
			double gravity = (double)Rule->Gravity;
			if (bullet->Class->IsFloater) {
				gravity = Get_Floater_Gravity();
			}
			int planar = Point2D(displacement).Length();
			valid = Calculate_Projectile_Pitch(Should_Use_High_Arc(which), speed, planar, height, gravity, pitch);
		} else if (bullet->Class->IsVoxel) {
			if (target->Center_Coord().Z < Center_Coord().Z) {
				pitch = DirType(DIR_S);
			} else {
				pitch = DirType(DIR_E);
			}
		} else {
			int abs_z = abs(height);
			if (abs_z > 200) {
				double y = 20.0;
				if (TarCom != NULL && TarCom->RTTI == RTTI_BUILDING) {
					BuildingClass * building = static_cast<BuildingClass *>(TarCom);
					Coord turret = Turret_Coord(which);
					height = building->Class->ZHeight * 200 - turret.Z;
					abs_z = abs(height);
					if ((double)abs_z < 20.0) {
						y = 0.0;
					}
				}
				double planar = (double)Point2D(displacement).Length();
				if (planar < 0.05) {
					planar = 0.05;
				}
				double angle = fastmath::atan2f((float)((double)abs_z - y), (float)planar);
				if (height < 0) {
					angle = -angle;
				}
				pitch = DirType(angle);
			}
		}
		velocity.Set_Pitch(pitch);
		if (valid) {
			if (!bullet->Unlimbo(fire_coord, velocity)) {
				bullet->Release();
				bullet = 0;
			} else {
				if (bullet->Class->IsInvisible && object != 0 && object->IsOnBridge) {
					bullet->IsOnBridge = true;
				}
				if (Is_Turret_Equipped() && TClass->TurretRecoil) {
					TurretRecoil.Fire();
					BarrelRecoil.Fire();
				}
				if (weapon->UseFireParticles && ParticleSystems[ATTACHED_PARTICLE_FIRE] == 0) {
					ParticleSystems[ATTACHED_PARTICLE_FIRE] = new ParticleSystemClass(weapon->AttachedParticleSystem, fire_coord, target, this, COORD_NONE);
				}
				if (weapon->UseSparkParticles && ParticleSystems[ATTACHED_PARTICLE_SPARK] == 0) {
					ParticleSystems[ATTACHED_PARTICLE_SPARK] = new ParticleSystemClass(weapon->AttachedParticleSystem, fire_coord, target, this, COORD_NONE);
				}
				if (weapon->IsRailgun && ParticleSystems[ATTACHED_PARTICLE_RAILGUN] == 0) {
					Coord end = Railgun_Beam_Damage(fire_coord, target, weapon);
					ParticleSystems[ATTACHED_PARTICLE_RAILGUN] = new ParticleSystemClass(weapon->AttachedParticleSystem, fire_coord, 0, this, end);
				}
				BurstIndex++;
				int delay = Rearm_Delay(which);
				ChargeTurretDelay = delay;
				Arm = delay;
				BurstIndex %= weapon->Burst;
				AnimTypeClass const * anim_type = 0;
				int anim_count = weapon->Anim.Count();
				if (anim_count == 8) {
					anim_type = weapon->Anim[(Fire_Direction().As_Dir8() + 1) % 8];
				} else if (anim_count > 0) {
					anim_type = weapon->Anim[0];
				}
				if (weapon->Sound.Count() > 0) {
					int voc = weapon->Sound.Pick((unsigned short)SoundRandomSeed);
					VocClass::PlayAt(voc, fire_coord, 0);
				}
				if (anim_type != 0) {
					AnimClass * anim = new AnimClass(anim_type, fire_coord);
					if (RTTI == RTTI_BUILDING) {
						int dy = (fire_coord - Render_Coord()).Y;
						int zadjust = dy / -4;
						anim->ZAdjust = MIN(zadjust, 0);
						BuildingClass * building = static_cast<BuildingClass *>(this);
						if (building->GetOccupantCount() > 0) {
							anim->ZAdjust = -200;
							building->FiringOccupantIndex++;
							building->FiringOccupantIndex %= building->GetOccupantCount();
						}
					}
					if (anim != 0 && RTTI != RTTI_BUILDING) {
						anim->Attach_To(this);
					}
				}
				if (weapon->IsSonic) {
					Wave = new WaveClass(fire_coord, target_coord, this, WAVE_SONIC, (TechnoClass *)target);
				}
				if (TClass->IsTargetLaser && House->Is_Player_Control()) {
					TargetingLaserTimer = 15;
				}
				if (weapon->IsLaser) {
					if (RTTI == RTTI_BUILDING) {
						WeaponTypeClass * primary = PrimaryWeapon;
						LaserDrawClass * laser = Laser_Zap(target, which, primary, COORD_NONE);
						BuildingClass * building = static_cast<BuildingClass *>(this);
						if (laser != 0 && building->Class == Rule->PrismType) {
							laser->Thickness = 3;
							if (building->SupportingPrisms > 0) {
								laser->IsSupported = true;
								laser->Thickness = 5;
							}
						}
					} else {
						LaserDrawClass * laser = Laser_Zap(target, which, weapon, COORD_NONE);
						if (laser != 0 && weapon->IsHouseColor) {
							laser->Thickness = 2;
						}
					}
				} else if (weapon->IsElectricBolt) {
					Electric_Zap(target);
				} else if (weapon->IsRadBeam) {
					WarheadTypeClass const * warhead = weapon->WarheadPtr;
					if (warhead == 0 || !warhead->Temporal) {
						Rad_Zap(target, 0);
					} else {
						Rad_Zap(target, 1);
					}
				} else if (weapon->IsRadEruption) {
					Rad_Eruption((int)weapon->WarheadPtr->SpreadFactor);
				}
				Reduce_Ammunition();
				Mark(MARK_CHANGE);
				if (!(IsOwnedByPlayer || IsDiscoveredByPlayer)
				|| ((Map.Is_Shrouded(Center_Coord()) || Map.Is_Fogged(Center_Coord())) && !(RTTI == RTTI_AIRCRAFT && IsOwnedByPlayer))) {
					if ((target->AbstractFlags & ABSTRACT_OBJECT)) {
						HouseClass * owner = static_cast<ObjectClass *>(target)->Owner_HouseClass();
						if (owner != 0 && owner->Is_Player_Control() && weapon->RevealOnFire) {
							Map.RevealArea1(Center_Coord(), 3, owner, false, false, false, true, false);
							Map.RevealArea3(Center_Coord(), 0, 4, 0);
						}
					}
				}
			}
		} else {
			bullet->Release();
			bullet = 0;
		}
	}
	// RA2: A limbo launch preserves selection and team membership for the returning firer.
	if (weapon->LimboLaunch) {
		if (TClass->ReselectIfLimboed && IsSelected) {
			HouseClass * house = Owner_HouseClass();
			if (house->Is_Player_Control() && target->RTTI == RTTI_INFANTRY) {
				ShouldBeReselectOnUnlimbo = true;
			}
		}
		if (TClass->RejoinTeamIfLimboed && Is_Foot() && static_cast<FootClass *>(this)->Team != 0 && target->RTTI == RTTI_INFANTRY) {
			OldTeam = static_cast<FootClass *>(this)->Team;
		}
		Limbo();
		if (weapon->WarheadPtr->Parasite) {
			if ((target->AbstractFlags & ABSTRACT_FOOT)) {
				static_cast<FootClass *>(target)->LastBeParasitedStartFrame = Frame + 20;
			}
			bullet->Limbo();
			bullet->Set_Bullet_Data(weapon->Bullet, target, this, firepower, weapon->WarheadPtr, weapon->MaxSpeed, weapon->IsBright);
			bullet->Unlimbo(fire_coord, velocity);
		}
	}
	if (TClass->DistributedFire && (Mission != MISSION_ATTACK || RTTI == RTTI_BUILDING)) {
		AttackedTargets.Add(target);
		Assign_Target(0);
	}
	if (weapon->FireOnce) {
		if (Is_Foot() && static_cast<FootClass *>(this)->Team != 0) {
			static_cast<FootClass *>(this)->Team->Assign_Mission_Target(0);
			static_cast<FootClass *>(this)->Team->IsNextMission = true;
		}
		Assign_Target(0);
	}
	return bullet;
}
#endif


/// <summary>
/// Draws the targeting laser line from this object's firing coordinate to its current target:
/// a small box at each endpoint connected by an animated dashed red line.
/// </summary>
void TechnoClass::Draw_Target_Laser(void) const
{
	static bool _pattern[16] = {
		true,
		false,
		true,
		false,
		true,
		false,
		true,
		false,
		true,
		false,
		true,
		false,
		true,
		false,
		true,
		false
	};

	if (TarCom != NULL) {

		Coord coord = Turret_Coord(0);
		Coord predicted = Predict_Target_Coord();

		Point2D point;
		Point2D point2;

		TacticalMap->Coord_To_Pixel(coord, point);
		TacticalMap->Coord_To_Pixel(predicted, point2);

		point.Y += TacticalRect.Y;
		point2.Y += TacticalRect.Y;

		LogicalSurface->Fill_Rect(Intersect(TacticalRect, Rect(point, 3, 3) - Point2D(2, 2)), NormalDrawer->Convert_Pixel(RED));
		LogicalSurface->Fill_Rect(Intersect(TacticalRect, Target_Laser_Rect(point2)), NormalDrawer->Convert_Pixel(RED));

		Rect tacticalr = TacticalRect;
		if (Clip_Line_To_Rect(point, point2, tacticalr)) {
			LogicalSurface->Draw_Dashed_Line(point, point2, NormalDrawer->Convert_Pixel(RED), _pattern, 7 * Frame % 16);
		}
	}
}


/***********************************************************************************************
 * TechnoClass::Player_Assign_Mission -- Assigns a mission as result of player input.          *
 *                                                                                             *
 *    This routine is called when the mission for an object needs to change as a result of     *
 *    player input. The basic operation would be to queue the event and let the action         *
 *    occur at the frame dictated by the queuing system. However, if a voice response is       *
 *    indicated, then perform it at this time. This will give a greater illusion of            *
 *    immediate response.                                                                      *
 *                                                                                             *
 * INPUT:   mission     -- The mission order to assign to this object.                         *
 *                                                                                             *
 *          target      -- The target of this object. This will be used for combat and attack. *
 *                                                                                             *
 *          destination -- The movement destination for this object.                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/22/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoClass::Player_Assign_Mission(MissionType order, AbstractClass * target, AbstractClass * destination, AbstractClass * nearby) // RA2: fourth argument, returns bool
{
	bool special = Can_Force_Order() && CanAttackOnTheMove(); // RA2: a forced attack or move becomes an attack-move
	if ((order == MISSION_MOVE || order == MISSION_ATTACK) && special) { // RA2: as above
		order = MISSION_ATTACK_MOVE; // RA2: as above
	}

	/*
	**	In planning mode the order is sent as a planned event and nothing else happens.
	*/
	if (Planning_Is_Active()) { // RA2: as above
		EventClass event(PlayerPtr->HeapID, TargetClass(this), order, target, destination, nearby); // RA2: as above
		Planning_Send_Event(event); // RA2: as above
		return(false);
	}

	if (AllowVoice) { // RA2: the order is answered with the matching voice
		if (order == MISSION_ATTACK) { // RA2: as above
			Response_Attack(); // RA2: as above
		} else if (order == MISSION_MOVE) { // RA2: as above
			Response_Move(); // RA2: as above
		} else if (order == MISSION_ENTER) { // RA2: as above
			if (TClass->VoiceEnter != VOC_NONE) { // RA2: as above
				VocClass::PlayGlobal(TClass->VoiceEnter, 1.0f, 0x2000, 0); // RA2: as above
			} else {
				Response_Move(); // RA2: as above
			}
		} else if (order == MISSION_CAPTURE) { // RA2: as above
			if (TClass->VoiceCapture != VOC_NONE) { // RA2: as above
				VocClass::PlayGlobal(TClass->VoiceCapture, 1.0f, 0x2000, 0); // RA2: as above
			} else if (TClass->VoiceEnter != VOC_NONE) { // RA2: as above
				VocClass::PlayGlobal(TClass->VoiceEnter, 1.0f, 0x2000, 0); // RA2: as above
			} else {
				Response_Move(); // RA2: as above
			}
		} else if (TClass->VoiceSpecialAttack.Count() > 0) { // RA2: as above
			TypeList<int> const & sounds = TClass->VoiceSpecialAttack; // RA2: as above
			unsigned random = NonCriticalRandomNumber(); // RA2: as above
			VocClass::PlayGlobal(sounds[random % sounds.Count()], 1.0f, 0x2000, 0); // RA2: as above
		}
	}

	{
		TargetClass dest(destination);
		TargetClass targ(target);
		Queue_Mission(TargetClass(this), order, targ, dest); // RA2: the temporaries are built in this order
	}
	return(true);
}



/// <summary>
/// Sends the player's event of the given type for this object, unless planning mode refuses it.
/// </summary>
bool TechnoClass::ClickedEvent(int event) // RA2: not in TibSun, at 0x6CC510
{
	EventClass packet(PlayerPtr->HeapID, event, TargetClass(this));
	if (Planning_Is_Active()) {
		Planning_Refuse_Event(&packet);
		return(false);
	}
	OutList.Add(packet);
	return(true);
}




/***********************************************************************************************
 * TechnoClass::What_Action -- Determines what action to perform if object is selected.        *
 *                                                                                             *
 *    This routine will examine the object specified and return with the action that will      *
 *    be performed if the mouse button were clicked over the object.                           *
 *                                                                                             *
 * INPUT:   object   -- The object that the mouse button might be clicked on.                  *
 *                                                                                             *
 * OUTPUT:  Returns with the action that will be performed if the object was clicked on.       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *   03/21/1995 JLB : Special target control for trees.                                        *
 *=============================================================================================*/
ActionType TechnoClass::What_Action(ObjectClass const * object, bool disallow_force) const
{
	if (object == NULL) { // RA2: nothing to act on
		return(ACTION_NONE); // RA2: as above
	}

	if (IsBeingWarpedOut()) { // RA2: a unit being warped out only selects
		if (Is_Weapon_Equipped() && House->Is_Player_Control() && object->Owner() != Owner()) { // RA2: as above
			return(ACTION_NONE); // RA2: as above
		}
		if (IsALoaner && IsOwnedByPlayer) { // RA2: as above
			return(ACTION_NONE); // RA2: as above
		}
		if (!object->CanBeSelected() || object->IsSelected) { // RA2: as above
			return(ACTION_NONE); // RA2: as above
		}
		return(ACTION_SELECT); // RA2: as above
	}

	if (object == this && CurrentObject.Count() == 1 && object->AttachedBomb != NULL && object->AttachedBomb->OwnerHouse->Is_Player_Control()) { // RA2: a bomb attached to this object can be detonated by its owner
		if (object->AttachedBomb->Is_Death_Bomb() == 1 && Rule->CanDetonateDeathBomb) { // RA2: as above
			return(ACTION_DETONATE); // RA2: the order of the tests follows the executable
		}
		if (object->AttachedBomb->Is_Death_Bomb() == 0 && Rule->CanDetonateTimeBomb) { // RA2: as above
			return(ACTION_DETONATE); // RA2: the order of the tests follows the executable
		}
	}

	bool altdown = !disallow_force && (Keyboard->Down(Options.KeyForceMove1) || Keyboard->Down(Options.KeyForceMove2)); // RA2: the move keys are read first (opcheck: Options+140, +144)
	bool force_fire = !disallow_force && (Keyboard->Down(Options.KeyForceAttack1) || Keyboard->Down(Options.KeyForceAttack2)); // RA2: as above
	if (!disallow_force && (Keyboard->Down(Options.KeySelect1) || Keyboard->Down(Options.KeySelect2))) { // RA2: the select (shift) keys toggle selection and cancel a force-fire
		if (force_fire) { // RA2: as above
			force_fire = false; // RA2: as above
		} else if (House->Is_Player_Control() && !IsALoaner) { // RA2: as above
			return(ACTION_TOGGLE_SELECT); // RA2: as above
		}
	}

	if (object == this && CurrentObject.Count() == 1 && House->Is_Player_Control()) { // RA2: as above
		return(ACTION_SELF); // RA2: as above
	}

	if (object == this && CurrentObject.Count() == 1 && House->Is_Ally(PlayerPtr) && Session.Type == GAME_NORMAL) { // RA2: as above
		if (this != NULL) { // RA2: the casts are tested against the pointer
			if (Fetch_RTTI() == RTTI_UNIT && ((UnitClass const *)this)->Class->Max_Passengers() > 0 && ((UnitClass const *)this)->Cargo.How_Many() > 0) { // RA2: as above
				return(ACTION_SELF); // RA2: as above
			}
			if (Fetch_RTTI() == RTTI_AIRCRAFT && ((AircraftClass const *)this)->Passenger) { // RA2: as above
				return(ACTION_SELF); // RA2: as above
			}
		}
	}

	if (House->Is_Player_Control() && House->Is_Ally((TechnoClass *)object) && Fetch_RTTI() != RTTI_AIRCRAFT && force_fire) { // RA2: a force-fire on an ally with the move keys down is a guard-area
		if (altdown) { // RA2: as above
			if (Can_Player_Move()) { // RA2: as above
				return(ACTION_GUARD_AREA); // RA2: as above
			}
		}
	}
	if (altdown && House->Is_Player_Control() && Can_Player_Move()) { // RA2: a forced move
		return(ACTION_MOVE); // RA2: as above
	}

	TechnoTypeClass const * ttype = Techno_Type_Class(); // RA2: the order of the tests follows the executable
	int primary = What_Weapon_Should_I_Use((ObjectClass *)object); // RA2: the weapon is chosen before the target is tested
	if (object->Not_Underground()) { // RA2: the order of the tests follows the executable
		WeaponTypeClass * weapon_type = Get_Class_Weapon_Data(primary)->Weapon; // RA2: as above
		bool disguised = Rule->AttackCursorOnDisguise || ((object->AbstractFlags & 1) && (object->Techno_Type_Class()->PermaDisguise || ((object->AbstractFlags & 1) && ((TechnoClass *)object)->DisguiseBlinkTimer.Value() != 0))); // RA2: as above
		bool owned = object->IsDisguisedAs(House) && !disguised; // RA2: as above
		bool capture = owned && object->GetDisguiseHouse(true) == NULL; // RA2: as above
		bool can_disguise = CanDisguiseAs((AbstractClass *)object); // RA2: as above
		bool usable = !owned || (capture && can_disguise); // RA2: as above
		bool ivan = weapon_type != NULL && weapon_type->WarheadPtr->IvanBomb && object->Techno_Type_Class() != NULL && object->Techno_Type_Class()->MaxPassengers > 0; // RA2: as above
		bool foot = false; // RA2: as above
		if (weapon_type != NULL && weapon_type->WarheadPtr->Sonic) { // RA2: as above
			foot = (object->AbstractFlags & 4); // RA2: as above
		}
		bool sonic = foot && ((FootClass *)object)->ParasiteEatingMe != NULL; // RA2: as above
		bool friendly = Techno_Type_Class()->AttackFriendlies || (Techno_Type_Class()->AttackCursorOnFriendlies && !ivan) || sonic; // RA2: as above
		bool enemy = !House->Is_Ally((TechnoClass *)object) || friendly; // RA2: as above
		bool owned_friendly = owned && friendly; // RA2: as above
		if (House->Is_Player_Control()) { // RA2: as above
			if (force_fire || (enemy && (usable || owned_friendly) && (object->Class_Of()->IsLegalTarget || can_disguise))) { // RA2: as above
				bool infiltrate = Fetch_RTTI() == RTTI_INFANTRY && ((InfantryTypeClass const *)ttype)->IsCapture; // RA2: as above
				if ((Is_Weapon_Equipped() || infiltrate) && (Can_Player_Move() || In_Range((ObjectClass *)object, primary))) { // RA2: as above
					bool special = false; // RA2: as above
					if (infiltrate) { // RA2: as above
						if ((!((InfantryClass const *)this)->Class->IsAgent && object->Fetch_RTTI() == RTTI_BUILDING && ((BuildingClass const *)object)->Class->IsCaptureable) // RA2: as above
							|| (((InfantryClass const *)this)->Class->IsAgent && object->Fetch_RTTI() == RTTI_BUILDING && ((BuildingClass const *)object)->Class->Spyable) // RA2: as above
							|| (((InfantryClass const *)this)->Class->IsBomber && object->Fetch_RTTI() == RTTI_BUILDING && ((BuildingClass const *)object)->Class->CanC4)) { // RA2: as above
							special = true; // RA2: as above
						}
					}
					if (Can_Fire((AbstractClass *)object, primary, true) != FIRE_ILLEGAL || special || force_fire) { // RA2: as above
						return(ACTION_ATTACK); // RA2: as above
					}
				}
			}
		}
	}

	if (Is_Weapon_Equipped() && House->Is_Player_Control() && object->Owner() != Owner()) { // RA2: selection of an own object
		return(ACTION_NONE); // RA2: as above
	}
	if (IsALoaner && IsOwnedByPlayer) { // RA2: as above
		return(ACTION_NONE); // RA2: as above
	}
	if (!object->CanBeSelected() || object->IsSelected || object->IsBeingWarpedOut()) { // RA2: as above
		return(ACTION_NONE); // RA2: as above
	}
	return(ACTION_SELECT); // RA2: as above
}




/***********************************************************************************************
 * TechnoClass::What_Action -- Determines action to perform if cell is clicked on.             *
 *                                                                                             *
 *    Use this routine to determine what action will be performed if the specified cell        *
 *    is clicked on. Usually this action is either a ACTION_MOVE or ACTION_NOMOVE. The action  *
 *    nomove is used to perform special case checking for nearby cells if in fact the mouse    *
 *    is clicked over the cell.                                                                *
 *                                                                                             *
 * INPUT:   cell  -- The cell to check for being clicked over.                                 *
 *                                                                                             *
 * OUTPUT:  Returns with the action that will occur if the cell is clicked on.                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *   07/10/1995 JLB : Force fire for buildings is explicitly disabled.                         *
 *=============================================================================================*/
ActionType TechnoClass::What_Action(Cell const & cell, bool check_fog, bool disallow_force) const
{
	CellClass const * cellptr = &Map[cell];
	OverlayTypeClass const * optr = NULL;

	bool ctrldown = !disallow_force && (Keyboard->Down(Options.KeyForceAttack1) || Keyboard->Down(Options.KeyForceAttack2)); // RA2: the force-attack keys are read first, then the select keys, then the move keys
	bool shiftdown = !disallow_force && (Keyboard->Down(Options.KeySelect1) || Keyboard->Down(Options.KeySelect2)); // RA2: as above
	bool altdown = !disallow_force && (Keyboard->Down(Options.KeyForceMove1) || Keyboard->Down(Options.KeyForceMove2)); // RA2: as above
	if (shiftdown && ctrldown) { // RA2: both together cancel each other
		shiftdown = false; // RA2: as above
		ctrldown = false; // RA2: as above
	}

	OverlayType overlay = OVERLAY_NONE;
	bool legal = false;
	if (IsBeingWarpedOut() || IsWarpingIn()) { // RA2: nothing to do for a unit being warped
		return(ACTION_NONE); // RA2: as above
	}
	if (Scen->Special.IsFogOfWar && check_fog) {
		if (cellptr->FoggedObjects != NULL) {
			for (int index = 0; index < cellptr->FoggedObjects->Count(); index++) {
				FoggedObjectClass * fogged = (*cellptr->FoggedObjects)[index];
				if (fogged->CanDraw) {
					if (fogged->RTTI == RTTI_OVERLAY) {
						overlay = fogged->Overlay;
					} else if (fogged->RTTI == RTTI_BUILDING && (fogged->House == NULL || !PlayerPtr->Is_Ally(fogged->House)) && (fogged->Get_Head_Record_Object_Type() == NULL || fogged->Get_Head_Record_Object_Type()->IsLegalTarget)) {
						legal = true;
					}
				}
			}
		}
	}

	if (overlay != OVERLAY_NONE) { // RA2: the fogged overlay wins, else the cell's own
		optr = OverlayTypes[overlay]; // RA2: as above
	} else if (cellptr->Overlay != OVERLAY_NONE) { // RA2: as above
		optr = OverlayTypes[cellptr->Overlay]; // RA2: as above
	}

	bool renovator = Is_Renovator();
	if (House->Is_Player_Control() && ((ctrldown && altdown) || IsOnWaypointPatrol) && Can_Player_Move() && (Can_Player_Fire() || renovator)) {
		if (PlayerPtr->Waypoint_At(cell) == NULL && !IsOnWaypointPatrol) { // RA2: the test is inverted
			return(ACTION_GUARD_AREA); // RA2: as above
		}
		return(ACTION_PATROL_WAYPOINT); // RA2: as above
	}

	WeaponDataStruct const * wdata; // RA2: the order of the tests follows the executable
	if (House->Is_Player_Control() && (wdata = GetTurretWeapon()) != NULL && wdata->Weapon != NULL) { // RA2: the turret weapon, not the primary
		int primary = What_Weapon_Should_I_Use((AbstractClass *)&Map[cell]); // RA2: chosen first
		bool destroyable = Map[cell].Is_Tile_Destroyable_Cliff();
		if (destroyable) {
			WeaponTypeClass * wtype = GetTurretWeapon()->Weapon; // RA2: as above
			if (wtype->WarheadPtr->IsFire) { // RA2: the order of the tests follows the executable
				destroyable = false;
			}
		}

		if (ctrldown || (optr != NULL && optr->IsLegalTarget) || destroyable || legal) {
			WarheadTypeClass const * warhead = GetTurretWeapon()->Weapon->WarheadPtr; // RA2: as above
			bool isally = (cellptr->Owner != HOUSE_NONE) ? !Houses[cellptr->Owner]->Is_Ally(House) : false; // RA2: as above

			VeinholeMonsterClass * monster = VeinholeMonsterClass::Get_Monster_At(cell);
			bool isvein = (monster != NULL && !monster->IsDead && monster->Strength > 0);

			if (ctrldown || optr == NULL || isvein || (isally && optr->IsWall && (warhead->IsWallDestroyer || (warhead->IsWoodDestroyer && optr->Armor == ARMOR_WOOD)))) {
				if (Can_Player_Move() || In_Range(Coord(cell.X * 256 + 128, cell.Y * 256 + 128, 0), primary)) { // RA2: as above
					return(ACTION_ATTACK);
				}
			}
		}
		if (CanDisguiseAs((AbstractClass *)cellptr) && (Can_Player_Move() || In_Range(Coord(cell.X * 256 + 128, cell.Y * 256 + 128, 0), primary))) { // RA2: a disguised unit can attack a cell too
			return(ACTION_ATTACK); // RA2: as above
		}
	}

	if (House->Is_Player_Control() && (Can_Player_Move() || Is_Move_Override())) {

		if (Map.In_Local_Radar(cell, true)) { // RA2: outside the radar, only NOMOVE
			if (shiftdown) { // RA2: as above
				return(ACTION_MOVE);
			}
			if (Is_Move_Override() && altdown) { // RA2: as above
				return(ACTION_MOVE); // RA2: as above
			}
			if (Can_Player_Move()) { // RA2: as above
				if (Can_Enter_Cell(&Map[cell], FACING_NONE, -1, 0, true) <= MOVE_CLOAK || check_fog) { // RA2: as above
					return(ACTION_MOVE); // RA2: as above
				}
				if (Techno_Type_Class()->IsSubterranean && Can_Enter_Cell(&Map[cell], FACING_NONE, -1, 0, false) <= MOVE_CLOAK) { // RA2: as above
					return(ACTION_MOVE);
				}
			}
		}
		return(ACTION_NOMOVE);
	}

	return(ACTION_NONE);
}







/***********************************************************************************************
 * TechnoClass::Can_Player_Move -- Determines if the object can move be moved by player.       *
 *                                                                                             *
 *    Use this routine to determine whether a movement order can be given to this object.      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Can this object be given a movement order by the player?                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoClass::Can_Player_Move(void) const
{
	if (!House->Is_Player_Control()) { // RA2: as above
		return(false); // RA2: as above
	}
	if (IsUnderEMP()) { // RA2: the virtual (slot 201)
		return(false); // RA2: as above
	}
	if (TClass->Spawned) { // RA2: a spawned unit is not moved by the player
		return(false); // RA2: as above
	}
	if (IsParalyzed()) { // RA2: nor a paralyzed one
		return(false); // RA2: as above
	}
	return(IsNotWarping() ? true : false); // RA2: nor one warping
}



/// <summary>
/// Determines whether this object is currently able to attack.
/// For units, returns false if it must deploy to fire while in a tube, or if it is immobilized.
/// </summary>
/// <returns>bool; can the object attack right now?</returns>
bool TechnoClass::Can_Attack_Now(void) const
{
	if (this != NULL && Fetch_RTTI() == RTTI_UNIT) { // RA2: Can_Attack_Now at 0x6CD300 matches only so
		FootClass const * foot = (FootClass const *)this; // RA2: as above
		if ((foot->CurrentTube >= 0 && foot->Deploy_To_Fire()) || IsUnderEMP()) { // RA2: as above
			return(false);
		}
	}
	return(true);
}


/// <summary>
/// Can this object deploy where it stands?
/// This routine is consulted before a deploy or undeploy order is accepted. A vehicle that
/// cannot move, that has nothing to deploy into, or that is riding a tunnel is refused, as
/// is a transport standing under a bridge. A building that is allowed to undeploy anywhere
/// overrides all of that, but nothing at all may deploy against a tunnel mouth.
/// </summary>
/// <returns>bool; Can the object deploy now?</returns>
#ifdef RA2_NON_MATCHING // not exact: no independent bound yet (006cd340 has no row); 684 bytes differ against main's function-map extent
bool TechnoClass::Can_Deploy_Now(void) const
{
	CellClass const * cell = &Map[Get_Cell()];
	if (cell && cell->Is_Near_Tunnel_NW()) return(false);
	switch (Fetch_RTTI()) {
		case RTTI_UNIT:
		{
			UnitClass const * unit = (UnitClass const *)this;
			UnitTypeClass const * type = unit->Class;
			if (IsUnderEMP() || unit->CurrentTube >= TUBE_FIRST) return(false);
			if (!type->DeploysInto && !type->Max_Passengers() && !type->IsSimpleDeployer) return(false);
			if (type->DeploysInto && type->DeploysInto->IsConstructionYard && unit->ParasiteEatingMe) return(false);
			if (type->Max_Passengers() > 0) {
				if (IsOnBridge) return(false);
				Cell center = Get_Cell();
				CellClass * here = &Map[center];
				if (here) {
					CellClass * south = &Map[Adjacent_Cell(center, FACING_S)];
					CellClass * north = &Map[Adjacent_Cell(center, FACING_N)];
					CellClass * east = &Map[Adjacent_Cell(center, FACING_E)];
					CellClass * west = &Map[Adjacent_Cell(center, FACING_W)];
					if (here->IsUnderBridge || south && south->IsUnderBridge ||
						west && west->IsUnderBridge || east && east->IsUnderBridge ||
						north && north->IsUnderBridge) return(false);
				}
				if (House->IsHuman && here->Land_Type() == LAND_WATER) return(false);
				if (Get_Cell_Ptr()->Cell_Building()) return(false);
			}
			return(true);
		}

		case RTTI_BUILDING:
		return(false);

		case RTTI_INFANTRY:
		{
			InfantryClass const * infantry = (InfantryClass const *)this;
			if (!infantry->Has_Undeploy_Delay() || (!infantry->Is_Deploying() && infantry->Doing != DO_UNDEPLOY)) {
				return(infantry->Is_Deployer());
			}
			return(false);
		}

		default:
			return(!IsUnderEMP() && Techno_Type_Class()->Max_Passengers() != 0);
	}
}
#endif


/***********************************************************************************************
 * TechnoClass::Can_Player_Fire -- Determines if the player can give this object a fire order. *
 *                                                                                             *
 *    Call this routine to determine if this object can be given a fire order by the player.   *
 *    Such objects will affect the mouse cursor accordingly -- usually causes the targeting    *
 *    cursor to appear.                                                                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Can this object be given firing orders by the player?                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoClass::Can_Player_Fire(void) const
{
	if (House->Is_Player_Control()) { // RA2: as above
		WeaponDataStruct const * weapon = GetTurretWeapon(); // RA2: the turret slot's weapon
		if (weapon != NULL && weapon->Weapon != NULL && !IsUnderEMP()) { // RA2: the virtual (slot 201)
			return(true); // RA2: as above
		}
	}
	return(false);
}



/***********************************************************************************************
 * TechnoClass::Is_Weapon_Equipped -- Determines if this object has a combat weapon.           *
 *                                                                                             *
 *    Use this routine to determine if this object is equipped with a combat weapon. Such      *
 *    determination is used by the AI system to gauge the threat potential of the object.      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Is this object equipped with a combat weapon?                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoClass::Is_Weapon_Equipped(void) const
{
	WeaponDataStruct const * weapon = GetTurretWeapon(); // RA2: the weapon of the turret's slot
	return(weapon != NULL && weapon->Weapon != NULL); // RA2: as above
}



/***********************************************************************************************
 * TechnoClass::Can_Repair -- Determines if the object can and should be repaired.             *
 *                                                                                             *
 *    Use this routine to determine if the specified object is a candidate for repair. In      *
 *    order to qualify, the object must be allowed to be repaired (in theory) and it must      *
 *    be below full strength. If these conditions are met, then it can be repaired.            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; May this unit be repaired? A return value of false may mean that the object  *
 *                is not allowed to be repaired, or it might be full strength already.         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoClass::Can_Repair(void) const
{
	if (Fetch_RTTI() != RTTI_BUILDING) { // RA2: Can_Repair at 0x6CD690 matches only so
		return(false);
	}
	return(Techno_Type_Class()->IsRepairable && Strength != Class_Of()->MaxStrength); // RA2: as above
}


/// <summary>
/// Whether this object's power is on; a plain object has none.
/// </summary>
bool TechnoClass::IsPowerOnline(void) const // RA2: not in TibSun, at 0x6CD6E0
{
	return(false);
}


/// <summary>
/// The extra weapon range this object gets from standing above the lowest ground of the map: one
/// ElevationIncrementBonus per ElevationIncrement levels up to ElevationBonusCap cells, combined with the height.
/// </summary>
int TechnoClass::Elevation_Bonus(void) // RA2: not in TibSun, at 0x6CD6F0
{
	if (In_Air()) {
		return(0);
	}
	int increment = Rule->ElevationIncrement;
	if (increment == 0) {
		return(0);
	}
	double bonus = Rule->ElevationIncrementBonus;
	Cell where = Center_Coord().As_Cell();
	CellClass * cell = &Map[where];
	int lowest = Map.Get_Max_Level();
	int diff = (cell->Height - lowest) / increment;
	double scaled = diff * bonus;
	double cap = Rule->ElevationBonusCap;
	scaled = (scaled < cap) ? scaled : Rule->ElevationBonusCap;
	int height = cell->Height * LEVEL_LEPTON_H;
	int lift = (int)scaled << 8;
	return((int)fastmath::sqrt(lift * lift + height * height));
}








/// <summary>
/// RA2
/// </summary>
static inline int Passenger_Range(TechnoClass const * transport)
{
	int range = 0x7FFFFFFF;
	TechnoClass * passenger = transport->Cargo.Attached_Object();
	if (passenger != NULL) {
		do {
			WeaponTypeClass * weapon = passenger->GetTurretWeapon()->Weapon;
			if (weapon != NULL && weapon->Range < range) {
				range = weapon->Range;
			}
			passenger = (TechnoClass *)passenger->Next;
		} while (passenger != NULL && (passenger->AbstractFlags & 4));
	}
	return(range);
}


/***********************************************************************************************
 * TechnoClass::Weapon_Range -- Determines the maximum range for the weapon.                   *
 *                                                                                             *
 *    Use this routine to determine the maximum range for the weapon indicated.                *
 *                                                                                             *
 * INPUT:   which -- Which weapon to use when determining the range. 0=primary, 1=secondary.   *
 *                                                                                             *
 * OUTPUT:  Returns with the range of the weapon (in leptons).                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int TechnoClass::Weapon_Range(int which) const
{
	assert((unsigned)which < 2);

	WeaponTypeClass const * weapon = Get_Class_Weapon_Data(which)->Weapon;

	if (weapon != NULL) {
		return(weapon->Range);
	}
	return(0);
}


/***************************************************************************
 * TechnoClass::Override_Mission -- temporarily overrides a units mission  *
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
void TechnoClass::Override_Mission(MissionType mission, AbstractClass * tarcom, AbstractClass * navcom)
{
	SuspendedTarCom = TarCom;
	MissionClass::Override_Mission(mission, tarcom, navcom); // RA2: Override_Mission at 0x6CD840 matches only so
	Assign_Target(tarcom);
}


/***************************************************************************
 * TechnoClass::Restore_Mission -- Restores an overridden mission          *
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
bool TechnoClass::Restore_Mission(void)
{
	if (MissionClass::Restore_Mission()) { // RA2: Restore_Mission at 0x6CD880 matches only so
		Assign_Target(SuspendedTarCom);
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * TechnoClass::Renovate -- Heal a building to maximum                                         *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/15/1996 BWG : Created.                                                                 *
 *=============================================================================================*/
void TechnoClass::Renovate(void)
{
	Mark(MARK_CHANGE);
	int max_strength = Techno_Type_Class()->MaxStrength; // RA2: Renovate at 0x6CD8B0 matches only so
	Strength = max_strength; // RA2: as above
	EstimatedHealth = max_strength; // RA2: as above
	if (Fetch_RTTI() == RTTI_BUILDING) { // RA2: as above
		Repair(0); // RA2: as above
		((BuildingClass *)this)->Set_Anim_Damage_State(HealthRatio <= Rule->ConditionYellow);
		VocClass::PlayAt(Rule->BuildingRepairedSound, Center_Coord(), NULL); // RA2: as above
	}
}


/***********************************************************************************************
 * TechnoClass::Captured -- Handles capturing this object.                                     *
 *                                                                                             *
 *    This routine is called when this object gets captured by the house specified. It handles *
 *    removing this object from any targeting computers and then changes the ownership of      *
 *    the object to the new house.                                                             *
 *                                                                                             *
 * INPUT:   newowner -- Pointer to the house that is now the new owner.                        *
 *                                                                                             *
 * OUTPUT:  Was the object captured? Failure would mean that it is already under control of    *
 *          the house specified.                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *   09/29/1995 JLB : Keeps track of quantity records.                                         *
 *=============================================================================================*/
bool TechnoClass::Captured(HouseClass * newowner, bool announce) // RA2: signature as in techno.h
{
	if (newowner != House) {
		Assign_Target(NULL);
		Assign_Destination(NULL, true); // RA2: as above
		Planning_Cancel_Token(this); // RA2: the object lets go of its planning token
		House->Tracking_Active_Remove(this, false);
		Record_The_Kill(NULL); // RA2: as above
		newowner->PointTotal += Techno_Type_Class()->Cost_Of(House); // RA2: as above
		House->Tracking_Remove(this);
		newowner->Tracking_Add(this);
		switch (Fetch_RTTI()) { // RA2: as above
			case RTTI_BUILDING:
				newowner->BuildingsKilled[Owner()]++; // RA2: the new owner's counters are advanced
				break;
			case RTTI_UNIT:
			case RTTI_AIRCRAFT:
			case RTTI_INFANTRY:
				newowner->UnitsKilled[Owner()]++; // RA2: as above
				break;
		}
		House->WhoLastHurtMe = newowner->Class->House;
		if (!IsInLimbo) {
			Mark(MARK_UP);
		}
		Remove_Target(this); // RA2: nobody keeps this object as a target
		if (!IsInLimbo) {
			Mark(MARK_DOWN_FORCED);
		}
		CellClass * cell = NULL; // RA2: as above
		if (!IsInLimbo) {
			bool foot; // RA2: as above
			if (this != NULL) { // RA2: the foot test by flag, with its local
				foot = (AbstractFlags & ABSTRACT_FOOT); // RA2: as above
			}
			bool moving = this != NULL && foot; // RA2: as above
			if (!moving) { // RA2: as above
				cell = &Map[Coord(PositionCoord)]; // RA2: as above
			} else {
				cell = &Map[((FootClass *)this)->LastAdjacencyCell]; // RA2: as above
			}
			if (cell != NULL) { // RA2: the threat this object lent is taken back
				cell->Adjust_Threat(Owner(), -(int)ThreatPosed); // RA2: as above
				ThreatPosed = 0; // RA2: as above
			}
		}
		Set_Owner(newowner); // RA2: as above
		if (!IsInLimbo) {
			if (cell != NULL) { // RA2: and lent again for the new owner
				cell->Adjust_Threat(Owner(), ThreatPosed = Risk()); // RA2: as above
			}
		}
		House->Tracking_Active_Add(this, true); // RA2: as above
		if (!IsInLimbo) {
			if (Fetch_RTTI() != RTTI_BUILDING || CurrentMission != MISSION_UNLOAD || !((BuildingClass *)this)->Class->IsWeaponsFactory) { // RA2: a weapons factory that is unloading keeps its order
				TechnoClass * radio; // RA2: as above
				if (Radio_Link(0) == NULL || (radio = Radio_Link(0))->Fetch_RTTI() != RTTI_BUILDING || !((BuildingClass *)radio)->Class->IsWeaponsFactory) { // RA2: as above
					Assign_Destination(NULL, true); // RA2: as above
					Assign_Target(NULL); // RA2: as above
					Enter_Idle_Mode(false, true); // RA2: as above
				}
			}
			Radar_Untrack();
			Radar_Track();
		}
		return(true);
	}
	return(false);
}



/// <summary>
/// Reassigns this object to a new owning house.
/// Updates the player-ownership flag based on whether the new owner is the local player.
/// </summary>
/// <param name="newowner">The house that will now own this object.</param>
void TechnoClass::Set_Owner(HouseClass * newowner)
{
	House = newowner;
	IsOwnedByPlayer = (newowner == PlayerPtr && PlayerPtr != NULL); // RA2: Set_Owner at 0x6CDC00 matches only so
}


/***********************************************************************************************
 * TechnoClass::Take_Damage -- Records damage assessed to this object.                         *
 *                                                                                             *
 *    This routine is called when this object has taken damage. It handles recording whether   *
 *    this object has been destroyed. If it has, then mark the appropriate kill records as     *
 *    necessary.                                                                               *
 *                                                                                             *
 * INPUT:                                                                                      *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/20/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ResultType TechnoClass::Take_Damage(int & damage, int distance, WarheadTypeClass const * warhead, TechnoClass * attacker, bool forced, bool no_crew)
{
	bool healing = damage < 0;
	if (!forced && !healing) {
		double incoming = damage;
		damage = (int)(incoming / (House->Armor_Multiplier(Techno_Type_Class()) * ArmorBias));
		if (Has_Ability(ABILITY_STRONGER)) {
			damage = (int)(damage / Rule->VeteranArmor);
		}
		if (damage < 1) damage = 1;
		if (attacker && Techno_Type_Class()->IsTypeImmune) {
			TechnoTypeClass const * attacker_type = attacker->Techno_Type_Class();
			if (Techno_Type_Class() == attacker_type && House == attacker->House) return RESULT_NONE;
		}
	}
	if (Unknown_Virtual_32C() && !forced && !healing) {
		Combat_Lighting(PositionCoord, damage * 2, warhead, 1, 1);
		damage = 0;
		return RESULT_NONE;
	}
	if (IsBeingWarpedOut()) {
		damage = 0;
		return RESULT_NONE;
	}
	if (Techno_Type_Class()->DamageReducesReadiness) {
		double fraction = (double)damage / Techno_Type_Class()->MaxStrength;
		double reduction = Techno_Type_Class()->ReadinessReductionMultiplier * fraction;
		double readiness = Ammo;
		Ammo = (int)(readiness - Techno_Type_Class()->MaxAmmo * reduction);
		if (Ammo < 0) Ammo = 0;
		Start_Reload_Timer();
	}
	if (warhead) {
		if (warhead->Radiation && Techno_Type_Class()->ImmuneToRadiation) { damage = 0; return RESULT_NONE; }
		if (warhead->PsychicDamage && Techno_Type_Class()->ImmuneToPsionics) { damage = 0; return RESULT_NONE; }
	}
	ResultType result = ObjectClass::Take_Damage(damage, distance, warhead, attacker, forced, no_crew);
	if (attacker) {
		TechnoTypeClass const * type = Techno_Type_Class();
		double incoming = damage;
		House->Add_Anger((int)(type->Raw_Cost() * (incoming / Techno_Type_Class()->MaxStrength)), attacker->House);
	}
	if (result == RESULT_DESTROYED) {
		if (Fetch_RTTI() == RTTI_BUILDING && warhead && warhead->CausesDelayKill) {
			BuildingClass * building = As_Building(this);
			if (building->Class->EligibleForDelayKill) {
				int delay = warhead->DelayKillFrames;
				int frames = (int)((double)delay +
					((warhead->DelayKillAtMax * (double)delay - delay) / (double)((int)warhead->SpreadFactor * 256)) * distance);
				if (!building->IsGoingToBlow || frames < building->CountDown.Value()) {
					building->IsGoingToBlow = true;
					building->CountDown = CDTimerClass<FrameTimerClass>(frames);
				}
				IsActive = true;
				Strength = 1;
				result = RESULT_ALREADY_DESTROYED;
			}
		}
	}
	if (result == RESULT_ALREADY_DESTROYED) return RESULT_ALREADY_DESTROYED;
	if (result != RESULT_NONE) {
		RadarFlashTimer = CDTimerClass<FrameTimerClass>(Rule->RadarCombatFlashTime);
	}
	if (result != RESULT_NONE && result != RESULT_DESTROYED && Techno_Type_Class()->CanDisguise && !Techno_Type_Class()->PermaDisguise) {
		if (IsDisguised()) ClearDisguise();
		InfantryBlinkTimer = CDTimerClass<FrameTimerClass>(damage * 2);
	}
	if (Strength == 0) result = RESULT_DESTROYED;
	switch (result) {
		case RESULT_DESTROYED:
        {
			if (Techno_Type_Class()->VoiceDie.Count() > 0 && House->Is_Player_Control()) {
				VocClass::PlayAt(Techno_Type_Class()->VoiceDie.Pick(NonCriticalRandomNumber()), PositionCoord, 0);
			}
			if (Techno_Type_Class()->DieSound.Count() > 0) {
				VocClass::PlayAt(Techno_Type_Class()->DieSound.Pick(NonCriticalRandomNumber()), PositionCoord, 0);
			}
			SendToEachLink(RADIO_OVER_OUT);
			Stun();
			if (ParticleSystems[0]) { ParticleSystems[0]->Delete_Me(); ParticleSystems[0] = 0; }
			if (!(HeightAGL <= 10 && IsToExplode && Map[Get_Coord()].Land_Type() == LAND_WATER)) {
				if (Techno_Type_Class()->MaxDebris > 0) {
					int maximum = Techno_Type_Class()->MaxDebris - 1;
					int debris = Random_Pick(Techno_Type_Class()->MinDebris, maximum);
					if (Techno_Type_Class()->DebrisTypes.Count() > 0) {
						int index = 0;
						while (debris > 0) {
							int pieces = abs(Scen->RandomNumber) % (Techno_Type_Class()->DebrisMaximums[index] + 1);
							if (pieces >= debris) pieces = debris;
							for (int n = pieces; n > 0; --n) {
								new VoxelAnimClass(Techno_Type_Class()->DebrisTypes[index], Center_Coord(), House);
							}
							debris -= pieces;
							if (++index >= Techno_Type_Class()->DebrisTypes.Count()) index = 0;
						}
					}
					if (Techno_Type_Class()->DebrisAnims.Count() > 0) {
						for (int n = debris; n > 0; --n) {
							new AnimClass(Techno_Type_Class()->DebrisAnims[Random_Pick(0, Techno_Type_Class()->DebrisAnims.Count() - 1)], Center_Coord() + Coord(0, 0, 20), 0, 1, ShapeFlags_Type(0x600), 0);
						}
					} else if (Techno_Type_Class()->DebrisTypes.Count() == 0) {
						for (; debris > 0; --debris) {
							new AnimClass(Rule->MetallicDebris[Random_Pick(0, Rule->MetallicDebris.Count() - 1)], Center_Coord() + Coord(0, 0, 20), 0, 1, ShapeFlags_Type(0x600), 0);
						}
					}
				}
				WeaponTypeClass const * weapon = Get_Class_Weapon_Data(CurrentWeaponNumber)->Weapon;
				if (Techno_Type_Class()->IsExploding || Has_Ability(ABILITY_EXPLODES) || (weapon && weapon->Suicide)) {
					Kill_Cargo(attacker);
					FireDeathWeapon(0);
				}
			}
		}
			if (AttachedBomb) AttachedBomb->Detonate();
			break;
		case RESULT_HALF:
        {
			TechnoTypeClass const * type = Techno_Type_Class();
			if (type->VoiceFeedback.Count() > 0 && NonCriticalRandomNumber(0, 99) < 30 && House->Is_Player_Control()) {
				int selection = NonCriticalRandomNumber();
				int voice = type->VoiceFeedback.Pick(selection);
				VocClass::PlayAt(voice, Center_Coord(), 0);
			}
		}
        break;
		case RESULT_NONE:
			break;
		case RESULT_MAJOR:
			break;
		default:
			if (Techno_Type_Class()->DamageSound != VOC_NONE) {
				VocClass::PlayIndexAtPos(Techno_Type_Class()->DamageSound, PositionCoord, 0);
			}
			if (Techno_Type_Class()->DamageSound != VOC_NONE) {
				VocClass::PlayIndexAtPos(Techno_Type_Class()->DamageSound, PositionCoord, 0);
			}
			if ((Techno_Type_Class()->IsToProtect || IsNeedingRescue) && !House->Is_Human_Player() && attacker) Base_Is_Attacked(attacker);
			break;
	}
	if (result != RESULT_DESTROYED) {
		if (attacker && !House->Is_Ally(attacker)) IsTickedOff = true;
		Do_Shimmer();
		if (HealthRatio > Rule->ConditionYellow) {
			if (ParticleSystems[3]) ParticleSystems[3]->Delete_Me();
		} else if (result == RESULT_HALF || result == RESULT_MAJOR) {
			DynamicVectorClass<ParticleSystemTypeClass const *> smoke_types;
			for (int index = Techno_Type_Class()->DamageParticleSystems.Count() - 1; index >= 0; --index) {
				if (Techno_Type_Class()->DamageParticleSystems[index]->BehavesLike == PSYS_BEHAVIOR_SMOKE) smoke_types.Add(Techno_Type_Class()->DamageParticleSystems[index]);
			}
			if (!ParticleSystems[3] && smoke_types.Count() > 0 && HeightAGL > -10) {
				Coord offset = Techno_Type_Class()->Damage_Smoke_Offset();
				ParticleSystems[3] = new ParticleSystemClass(const_cast<ParticleSystemTypeClass *>(smoke_types[Random_Pick(0, smoke_types.Count() - 1)]), PositionCoord + offset, 0, this, COORD_NONE);
			}
		}
		if (!healing) {
			TechnoClass * target = attacker;
			if (Is_Allowed_To_Retaliate(attacker, warhead)) {
				if (attacker == 0 && Rule->IsScatter) target = 0;
				if (target) {
					int which = What_Weapon_Should_I_Use(target);
					bool retaliate = In_Range(target, which);
					if (!retaliate) {
						if (House->Is_Human_Player()) {
							TPoint3D<int> difference = Center_Coord() - target->Center_Coord();
							double range = (int)fastmath::sqrt((double)difference.X * (double)difference.X + (double)difference.Z * (double)difference.Z + (double)difference.Y * (double)difference.Y);
							retaliate = range <= (Techno_Type_Class()->SightRange + 0.5) * 256.0;
						} else {
							retaliate = true;
						}
					}
					if (retaliate) {
						Override_Mission(MISSION_ATTACK, target, 0);
					}
				}
				FootClass * foot = (this && (AbstractFlags & ABSTRACT_FOOT) ? static_cast<FootClass *>(this) : 0);
				if (foot) {
					if (!TarCom && !foot->NavCom) {
						if (Rule->IsScatter || Has_Ability(ABILITY_SCATTER)) {
							Scatter(COORD_NONE, true, false);
						}
					}
				}
			} else {
				FootClass * foot = (this && (AbstractFlags & ABSTRACT_FOOT) ? static_cast<FootClass *>(this) : 0);
				if (foot) {
					if (Current_Mission_Control().IsScatter) {
						if (!IsTethered) {
							if (!foot->Locomotion->Is_Moving() && !TarCom && !foot->NavCom && Fetch_RTTI() != RTTI_AIRCRAFT) {
								if (!House->Is_Human_Player() || Rule->IsScatter || Has_Ability(ABILITY_SCATTER)) {
									Scatter(COORD_NONE, true, false);
								}
							}
						}
					}
				}
			}
		}
	}
	return result;
}




/***********************************************************************************************
 * TechnoClass::Record_The_Kill -- Records the death of this object.                           *
 *                                                                                             *
 *    This routine is used to record the death of this object. It will handle updating the     *
 *    owner house with the kill record as well as springing any trigger events associated with *
 *    this object's death.                                                                     *
 *                                                                                             *
 * INPUT:   source   -- Pointer to the source of this object's death (if there is a source).   *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1995 JLB : Created.                                                                 *
 *   08/23/1995 JLB : Building loss is only counted if it received damage.                     *
 *=============================================================================================*/
void TechnoClass::Record_The_Kill(TechnoClass * source)
{
	int total_recorded = 0;
	int points = TClass->Cost_Of(House);
	if (IsActive && Tag != 0 && source != 0) { // RA2: RA2's form
		Tag->Spring(TEVENT_ATTACKED, this, CELL_NONE, 0, 0); // RA2: as above
	}
	if (IsActive) { // RA2: as above
		if (Tag != 0 && source != 0) { // RA2: as above
			Tag->Spring(TEVENT_DISCOVERED, this, CELL_NONE, 0, 0); // RA2: as above
		}
		if (IsActive && RTTI != RTTI_UNIT) { // RA2: as above
			if (IsActive && Tag != 0 && source != 0) { // RA2: as above
				Tag->Spring(TEVENT_DESTROYED, this, CELL_NONE, 0, 0); // RA2: as above
			}
			if (IsActive && Tag != 0) { // RA2: as above
				Tag->Spring(TEVENT_DESTROYED_ANY, this, CELL_NONE, 0, 0); // RA2: as above
			}
			if (IsActive && Tag != 0) { // RA2: as above
				Tag->Spring(TEVENT_DESTROYED_ANY_X, this, CELL_NONE, 0, 0); // RA2: as above
			}
		}
	}
	if (source != 0) { // RA2: as above
		if (source->House->Is_Ally(this)) { // RA2: as above
			points = 0; // RA2: as above
		} else if (Veterancy.Is_Veteran()) { // RA2: as above
			points += points; // RA2: as above
		} else if (Veterancy.Is_Elite()) { // RA2: as above
			points *= 3; // RA2: as above
		}
		if (source->TClass->IsTrainable) {
			source->Veterancy.Made_A_Kill(source->TClass->Cost_Of(House), points);
		}
		if (source->TClass->MissileSpawn && source->SpawnOwner != 0 // RA2: as above
			&& ((TechnoClass *)source->SpawnOwner)->TClass->IsTrainable) { // RA2: as above
			((TechnoClass *)source->SpawnOwner)->Veterancy.Made_A_Kill( // RA2: as above
				((TechnoClass *)source->SpawnOwner)->TClass->Cost_Of(House), points); // RA2: as above
		}
		House->WhoLastHurtMe = source->Owner();

		/*
		**	Add up the score for killing this unit
		*/
		source->House->PointTotal += points;
	}
	switch ((RTTIType)RTTI) {
	case RTTI_BUILDING: // RA2: as above
		if (TClass->IsInsignificant) { // RA2: as above
			break;
		}
		if ((((BuildingClass *)this))->WhoLastHurtMe != HOUSE_NONE) { // RA2: as above
			House->BuildingsLost++; // RA2: as above
		}
		if (source != 0) { // RA2: as above
			source->House->DestroyedBuildings.Increment_Unit_Total(((BuildingClass *)this)->Class->HeapID); // RA2: as above
			source->House->BuildingsKilled[Owner()]++; // RA2: as above
		}

		/*
		**	If the map is displaying the multiplayer player names & their // RA2: as above
		** # of kills, tell it to redraw. // RA2: as above
		*/

		/*
		**	If the map is displaying the multiplayer player names & their // RA2: as above
		** # of kills, tell it to redraw. // RA2: as above
		*/
		if (Map.Is_Player_Names()) { // RA2: as above
			Map.Redraw_Radar(false); // RA2: as above
		}
		break; // RA2: as above
	case RTTI_AIRCRAFT: // RA2: as above
		if (source != 0) { // RA2: as above
			source->House->DestroyedAircraft.Increment_Unit_Total(((AircraftClass *)this)->Class->HeapID); // RA2: as above
			total_recorded = 1; // RA2: as above
		}

	//Fall through.....
	case RTTI_INFANTRY: // RA2: as above
		if (source != 0 && !total_recorded) { // RA2: as above
			source->House->DestroyedInfantry.Increment_Unit_Total(((InfantryClass *)this)->Class->HeapID); // RA2: as above
			total_recorded = 1; // RA2: as above
		}

	//Fall through.....
	case RTTI_UNIT: // RA2: as above
		if (source != 0 && !total_recorded) { // RA2: as above
			source->House->DestroyedUnits.Increment_Unit_Total(((UnitClass *)this)->Class->HeapID); // RA2: as above
		}
		House->UnitsLost++; // RA2: as above
		if (source != 0) { // RA2: as above
			source->House->UnitsKilled[Owner()]++; // RA2: as above
		}
		if (Map.Is_Player_Names()) { // RA2: as above
			Map.Redraw_Radar(false); // RA2: as above
		}
		break; // RA2: as above
	default: // RA2: as above
		break; // RA2: as above
	}
}



/***********************************************************************************************
 * TechnoClass::Nearby_Location -- Radiates outward looking for clear cell nearby.             *
 *                                                                                             *
 *    This routine is used to find a nearby location from center of this object. It can lean   *
 *    toward finding a location closest to an optional object.                                 *
 *                                                                                             *
 * INPUT:   object   -- Optional object that the finding algorithm will try to find a close    *
 *                      spot to.                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the cell that is closest to this object.                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/06/1995 JLB : Created.                                                                 *
 *   09/28/1995 JLB : Uses map scan function.                                                  *
 *=============================================================================================*/
Cell TechnoClass::Nearby_Location(TechnoClass const * techno) const
{
	SpeedType speed = Techno_Type_Class()->Speed; // RA2: Nearby_Location at 0x6CF130 matches only so
	if (speed == SPEED_WINGED) {
		speed = SPEED_TRACK;
	}
	Cell cell;
	if (techno != NULL) {
		cell = techno->Center_Coord().As_Cell();
	} else {
		cell = Center_Coord().As_Cell();
	}
	MZoneType mzone = Techno_Type_Class()->MZone; // RA2: as above
	int zone = mzone != MZONE_NONE ? Map.Get_Cell_Zone(cell, mzone, IsOnBridge) : -1;
	return(Map.Nearby_Location(cell, speed, zone, mzone, IsOnBridge));
}


/***********************************************************************************************
 * TechnoClass::Do_Uncloak -- Cause the stealth tank to uncloak.                               *
 *                                                                                             *
 *    This routine will start the stealth tank to uncloak.                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void TechnoClass::Do_Uncloak(bool silent)
{
	if (Cloak == CLOAKED || Cloak == CLOAKING) { // RA2: Do_Uncloak at 0x6CF250 has no CloakDelay, Rule
		Cloak = UNCLOAKING;
		CloakingDevice.Set_Stage(Rule->CloakingStages - 1);
		CloakingDevice.Set_Rate(Techno_Type_Class()->CloakingSpeed); // RA2: Do_Uncloak at 0x6CF250 matches only so
		CloakingDevice.Set_Step(-1);
		if (!silent) {
			VocClass::PlayAt(Rule->CloakSound, PositionCoord); // RA2: as above
		}
	}
}


/***********************************************************************************************
 * TechnoClass::Do_Cloak -- Start the object into cloaking stage.                              *
 *                                                                                             *
 *    This routine will start the object into its cloaking state.                              *
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
void TechnoClass::Do_Cloak(bool silent)
{
	if (Cloak == UNCLOAKED || Cloak == UNCLOAKING) {
		Detach_All(false);
		Cloak = CLOAKING;
		CloakingDevice.Set_Stage(0);
		CloakingDevice.Set_Rate(TClass->CloakingSpeed);
		CloakingDevice.Set_Step(1);
		if (!silent) {
			VocClass::PlayAt(Rule->CloakSound, PositionCoord); // RA2: the sound at the unit
		}
	}
}



/***********************************************************************************************
 * TechnoClass::Do_Shimmer -- Causes this object to shimmer if it is cloaked.                  *
 *                                                                                             *
 *    This routine is called when this object should shimmer. If the object is cloaked, then   *
 *    a shimmering effect (partial decloak) occurs. For objects that are not cloaked, no       *
 *    effect occurs.                                                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void TechnoClass::Do_Shimmer(void)
{
	Do_Uncloak();
}


/***********************************************************************************************
 * TechnoClass::Visual_Character -- Determine the visual character of the object.              *
 *                                                                                             *
 *    This routine will determine how this object should be drawn. Typically, this is the      *
 *    unmodified visible state, but cloaked objects have a different character.                *
 *                                                                                             *
 * INPUT:   raw   -- Should the check be based on the unmodified cloak condition of the        *
 *                   object? If false, then an object owned by the player will never become    *
 *                   completely invisible.                                                     *
 *                                                                                             *
 * OUTPUT:  Returns with the visual character to use when displaying this object.              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/07/1995 JLB : Created.                                                                 *
 *   05/27/1996 JLB : Knows about invisible objects.                                           *
 *=============================================================================================*/
VisualType TechnoClass::Visual_Character(bool raw, HouseClass const * house) const
{
	if (Techno_Type_Class()->IsInvisible && IsOwnedByPlayer) return(VISUAL_NORMAL); // RA2: Visual_Character at 0x6CF3C0 matches only so
	if (Techno_Type_Class()->IsInvisible && !IsOwnedByPlayer && !Debug_Map) return(VISUAL_HIDDEN); // RA2: as above
	if (Cloak == UNCLOAKED || Debug_Map || Fetch_RTTI() == RTTI_BUILDING) return(VISUAL_NORMAL); // RA2: as above
	if (Cloak == CLOAKED) {
		if (raw && house != NULL && Map[PositionCoord.As_Cell()].Is_Sensed(house->HeapID)) return(VISUAL_SHADOWY); // RA2: as above
		if (!raw && !MainWindow) return(VISUAL_SHADOWY);
		if (!raw && IsOwnedByPlayer) return(VISUAL_SHADOWY);
		if (!raw && Map[PositionCoord.As_Cell()].Is_Sensed(PlayerPtr->HeapID)) return(VISUAL_SHADOWY); // RA2: as above
		if (!raw && (Session.Type != GAME_NORMAL && House != NULL && PlayerPtr != NULL && PlayerPtr->Is_Ally(House) && House->Is_Ally(PlayerPtr))) return(VISUAL_SHADOWY);
		return(VISUAL_HIDDEN);
	}
	int stage = CloakingDevice.Fetch_Stage();
	if (stage <= 0) {
		return(VISUAL_NORMAL);
	}
	stage = double(stage) / Rule->CloakingStages * 256;
	if (stage < 0x0040) return(VISUAL_INDISTINCT);
	if (stage < 0x0080) return(VISUAL_DARKEN);
	if (stage < 0x00C0) return(VISUAL_SHADOWY);
	if (!raw && IsOwnedByPlayer) return(VISUAL_SHADOWY);
	if (stage < 0x00FF) return(VISUAL_RIPPLE);
	return(VISUAL_HIDDEN);
}


/// <summary>
/// Is this object under a bridge?
/// An object counts as under a bridge when its own cell is covered, or when it stands close
/// enough alongside one that the deck still passes over it. This routine feeds the draw
/// depth calculation, so that the object sorts beneath the deck. An object riding the
/// bridge itself never counts.
/// </summary>
/// <returns>bool; Is the object under a bridge?</returns>
bool TechnoClass::Is_Z_Fudge_Bridge(void) const
{
	Cell cell = Get_Cell();
	CellClass * cptr = &Map[cell];

	if (cptr != NULL && !IsOnBridge) {
		CellClass * south = &Map[Adjacent_Cell(cell, FACING_S)];
		CellClass * north = &Map[Adjacent_Cell(cell, FACING_N)];
		CellClass * east = &Map[Adjacent_Cell(cell, FACING_E)];
		CellClass * west = &Map[Adjacent_Cell(cell, FACING_W)];

		if (cptr->IsUnderBridge ||
			(south != NULL && south->IsUnderBridge && south->IsBridgeEastWest) ||
			(west != NULL && west->IsUnderBridge && !west->IsBridgeEastWest) ||
			(east != NULL && east->IsUnderBridge && !east->IsBridgeEastWest) ||
			(north != NULL && north->IsUnderBridge && north->IsBridgeEastWest)) {
			return(true);
		}
	}
	return(false);
}


/// <summary>
/// Was this object under a bridge?
/// This is the previous frame companion to Is_Z_Fudge_Bridge, and the bridge column depth
/// fudge consults the two together. An object riding the bridge itself never counts.
/// </summary>
/// <returns>bool; Was the object under a bridge?</returns>
bool TechnoClass::Was_Z_Fudge_Bridge(void) const
{
	Cell cell = Get_Cell();
	CellClass * cptr = &Map[cell];

	if (cptr != NULL && !IsOnBridge) {
		CellClass * south = &Map[Adjacent_Cell(cell, FACING_S)];
		CellClass * north = &Map[Adjacent_Cell(cell, FACING_N)];
		CellClass * east = &Map[Adjacent_Cell(cell, FACING_E)];
		CellClass * west = &Map[Adjacent_Cell(cell, FACING_W)];

		if (cptr->WasUnderBridge ||
			(south != NULL && south->WasUnderBridge && south->IsBridgeEastWest) ||
			(west != NULL && west->WasUnderBridge && !west->IsBridgeEastWest) ||
			(east != NULL && east->WasUnderBridge && !east->IsBridgeEastWest) ||
			(north != NULL && north->WasUnderBridge && north->IsBridgeEastWest)) {
			return(true);
		}
	}
	return(false);
}


/// <summary>
/// Determines how far a bridge column should bias this object's depth.
/// This routine feeds the draw depth calculation for an object that is, or has just been,
/// under a bridge, so that it sorts behind the support columns rather than in front of
/// them.
/// </summary>
/// <returns>Returns with the column fudge strength, or zero if no column is in the way.</returns>
int TechnoClass::Get_Z_Fudge_Column(void) const
{
	Cell cell = Get_Cell();
	int fudge = 0;
	CellClass * cptr = &Map[cell];

	if (cptr != NULL && (Is_Z_Fudge_Bridge() || Was_Z_Fudge_Bridge())) {
		CellClass * south = &Map[Adjacent_Cell(cell, FACING_S)];
		CellClass * east = &Map[Adjacent_Cell(cell, FACING_E)];
		CellClass * south_east = &Map[Adjacent_Cell(cell, FACING_SE)];

		if (south != NULL && east != NULL && south_east != NULL) {
			if (south->ITType != (IsometricTileType)ISOTILE_NONE && south->ITType != (IsometricTileType)ISOTILE_NONE_LEGACY) { // RA2: Get_Z_Fudge_Column at 0x6CF9D0 matches only so
				int tile = south->ITType - IsometricTileTypeClass::BridgeSet + 1;
				if (tile >= 7 && tile <= 16) { // RA2: as above
					fudge = 1;
				}
			}
			if (east->ITType != (IsometricTileType)ISOTILE_NONE && east->ITType != (IsometricTileType)ISOTILE_NONE_LEGACY) { // RA2: as above
				int tile = east->ITType - IsometricTileTypeClass::BridgeSet + 1;
				if (tile >= 7 && tile <= 16) { // RA2: as above
					fudge = 1;
				}
			}
			if (south_east->ITType != (IsometricTileType)ISOTILE_NONE && south_east->ITType != (IsometricTileType)ISOTILE_NONE_LEGACY) { // RA2: as above
				int tile = south_east->ITType - IsometricTileTypeClass::BridgeSet + 1;
				if (tile >= 7 && tile <= 16) { // RA2: as above
					fudge++;
				}
			}
		}
	}
	return(fudge);
}


/// <summary>
/// Determines whether a tunnel mouth should bias this object's depth.
/// This routine feeds the draw depth calculation, so that an object standing at a tunnel
/// entrance sorts behind the tunnel art instead of in front of it. An object riding a
/// bridge is never fudged this way.
/// </summary>
/// <returns>Returns with one when the tunnel fudge applies, otherwise zero.</returns>
int TechnoClass::Get_Z_Fudge_Tunnel(void) const
{
	int fudge = 0;
	Cell cell = PositionCell;

	if (!IsOnBridge) {
		CellClass * north = &Map[Adjacent_Cell(cell, DIR_N)];
		CellClass * west = &Map[Adjacent_Cell(cell, DIR_W)];
		Cell tuncell = CELL_NONE;
		CellClass * own = &Map[cell];

		if (north != NULL && west != NULL) {
			if (own->Has_Tunnel()) {
				tuncell = own->Fetch_CellID();
			} else if (north->Has_Tunnel()) {
				tuncell = north->Fetch_CellID();
			} else if (west->Has_Tunnel()) {
				tuncell = west->Fetch_CellID();
			}

			if (tuncell != CELL_NONE) {
				north = &Map[Adjacent_Cell(tuncell, DIR_N)];
				west = &Map[Adjacent_Cell(tuncell, DIR_W)];

				if (north != NULL && west != NULL) {
					north = &Map[Adjacent_Cell(north->Fetch_CellID(), DIR_N)];
					west = &Map[Adjacent_Cell(west->Fetch_CellID(), DIR_W)];

					if (north != NULL && west != NULL) {
						if (west->Has_Tunnel() || north->Has_Tunnel()) {
							fudge = 1;
						}
					}
				}
			}
		}
	}
	return(fudge);
}


/// <summary>
/// Determines how far a cliff should bias this object's depth.
/// This routine feeds the draw depth calculation, so that an object standing at the foot of
/// a cliff sorts behind it rather than through it. An object riding a bridge is never
/// fudged this way.
/// </summary>
/// <returns>Returns with the cliff fudge strength, or zero if no cliff is in the way.</returns>
int TechnoClass::Get_Z_Fudge_Cliff(void) const
{
	int fudge = 0;
	Cell cell = Get_Cell();
	CellClass * cptr = &Map[cell];

	if (!IsOnBridge) {
		CellClass * south_east = &Map[Adjacent_Cell(Get_Cell(), FACING_SE)];
		if (south_east != NULL) {
			if (south_east->Height - cptr->Height >= 4) {
				fudge = 2;
			}
			south_east = &Map[Adjacent_Cell(south_east->Fetch_CellID(), FACING_SE)];
			if (south_east != NULL) {
				if (south_east->Height - cptr->Height >= 4) {
					fudge = 1;
				}
			}
		}
	}
	return(fudge);
}


/// <summary>
/// Fetches this object's draw depth adjustment.
/// This routine biases the object's depth so that it sorts sensibly against whatever it is
/// standing on or next to -- a harvester backed into a refinery, a vehicle on or beside a
/// ramp, a rock or a tall tile in the way. FootClass::Get_Z_Adjust combines this with the
/// locomotor's own bias and with any bridge, tunnel or cliff fudge that applies.
/// </summary>
/// <returns>Returns with the amount to bias this object's depth by when it is drawn.</returns>
// RA2: Overlay_Is_Rock and Get_Z_Adjust are ra2-decomp's twin text
static inline bool Overlay_Is_Rock(int overlay)
{
	OverlayTypeClass * otype = OverlayTypes[overlay];
	return otype->IsARock;
}

int TechnoClass::Get_Z_Adjust(void) const
{
	CellClass *cptrs[3];
	int width;
	int height;
	static int _techno_zadj_ramp1 = -1;
	static int _techno_zadj_ramp2 = -1;
	static int _techno_zadj_rock = 3;
	int z = -TacticalMap->Z_Lepton_To_Pixel(Get_Height());
	int zadjust2 = z;
	int zadjust = z;
	Cell cell = PositionCell;
	CellClass *cptr = &Map[cell];
	if (RTTI == RTTI_UNIT) {
		UnitClass const * unit = static_cast<UnitClass const *>(this);
		if (unit->IsTethered) {
			BuildingClass * building = (BuildingClass *)unit->Radio_Link(0);
			if (building->RTTI == RTTI_BUILDING && building->Get_Mission() == MISSION_UNLOAD) {
				return zadjust2 - 3;
			}
		}
		if (unit->Class->IsToHarvest) {
			BuildingClass *building = Get_Cell_Ptr()->Cell_Building();
			if (building != 0 && building->Class->IsRefinery) {
				return zadjust2 - 14;
			}
		}
	}
	if (cptr->Ramp) {
		CellClass *cell_s = &Map[Adjacent_Cell(cell, FACING_S)];
		CellClass *cell_e = &Map[Adjacent_Cell(cell, FACING_E)];
		CellClass *cell_se = &Map[Adjacent_Cell(cell, FACING_SE)];
		if (cell_s != 0 && cell_e != 0 && cell_se != 0) {
			bool rock = false;
			if (cell_s->Overlay != OVERLAY_NONE) {
				if (Overlay_Is_Rock(cell_s->Overlay)) {
					rock = true;
				}
			} else if (cell_e->Overlay != OVERLAY_NONE) {
				if (Overlay_Is_Rock(cell_e->Overlay)) {
					rock = true;
				}
			} else if (cell_se->Overlay != OVERLAY_NONE) {
				if (Overlay_Is_Rock(cell_se->Overlay)) {
					rock = true;
				}
			}
			if (rock) {
				return _techno_zadj_rock + zadjust2 - 1;
			}
		}
	}
	CellClass *ahead_right = &Map[Adjacent_Cell(cell, Facing_Add(PrimaryFacing.Current().As_Dir8(), FACING_45))];
	CellClass *ahead_left = &Map[Adjacent_Cell(cell, Facing_Sub(PrimaryFacing.Current().As_Dir8(), FACING_45))];
	CellClass *ahead = &Map[Adjacent_Cell(cell, Facing_Add(PrimaryFacing.Current().As_Dir8(), FACING_0))];
	CellClass *behind = &Map[Adjacent_Cell(cell, Facing_Sub(PrimaryFacing.Current().As_Dir8(), FACING_180))];
	CellClass *behind_right = &Map[Adjacent_Cell(cell, Facing_Add(PrimaryFacing.Current().As_Dir8(), FACING_135))];
	CellClass *behind_left = &Map[Adjacent_Cell(cell, Facing_Sub(PrimaryFacing.Current().As_Dir8(), FACING_135))];
	bool ramp = false;
	if (ahead_right != 0 && ahead_left != 0 && ahead != 0) {
		if (ahead_right->Ramp == 0 && ahead_left->Ramp == 0 && ahead->Ramp == 0) {
			if (behind != 0 && behind_right != 0 && behind_left != 0) {
				if (behind->Ramp == 0 && behind_right->Ramp == 0 && behind_left->Ramp == 0) {
					if (cptr->Ramp) {
						return zadjust + _techno_zadj_ramp1;
					}
					if (PrimaryFacing.Current().As_Dir8() == FACING_W || PrimaryFacing.Current().As_Dir8() == FACING_N) {
						cptrs[0] = behind;
						cptrs[1] = behind_right;
						cptrs[2] = behind_left;
					} else if (PrimaryFacing.Current().As_Dir8() == FACING_S || PrimaryFacing.Current().As_Dir8() == FACING_E) {
						cptrs[0] = ahead_right;
						cptrs[1] = ahead_left;
						cptrs[2] = ahead;
					} else {
						return zadjust - 1;
					}
					if (cptr->IsOvershadowed) {
						return zadjust - 1;
					}
					for (int i = 0; i < ARRAY_SIZE(cptrs); i++) {
						if (cptrs[i]->IsOvershadowed) {
							return zadjust - 1;
						}
					}
					for (int j = 0; j < ARRAY_SIZE(cptrs); j++) {
						IsometricTileType ittype = cptrs[j]->ITType;
						int subtile;
						IsometricTileTypeClass * iptr;
						if (ittype == ISOTILE_NONE || ittype == ISOTILE_NONE_LEGACY) {
							iptr = IsometricTileTypes[TILE_CLEAR];
							subtile = 0;
						} else {
							iptr = IsometricTileTypes[ittype];
							subtile = cptrs[j]->SubTile;
						}
						iptr->Get_Tile_Pixel_Dimensions(subtile, width, height);
						if (height > 36) {
							return zadjust - 2;
						}
					}
					return zadjust - 1;
				} else {
					ramp = true;
				}
			}
		} else {
			ramp = true;
		}
	}
	if (ramp) {
		return _techno_zadj_ramp2 + zadjust;
	}
	return zadjust - 1;
}


static inline Rect Line_End_Rect(Point2D const & point)
{
	Point2D offset = point - Point2D(2, 2);
	return Rect(Point2D(offset.X, offset.Y), 3, 3);
}




/// <summary>
/// Draws an action line between two spots: end markers, an optional drop shadow, dashed or solid.
/// </summary>
void TechnoClass::Draw_Action_Line(Coord from, Coord to, RGBClass color, unsigned char dashed_arg, unsigned char shadow_arg) const
{
	Point2D start;
	Point2D end;
	TacticalMap->Coord_To_Pixel(from, start);
	TacticalMap->Coord_To_Pixel(to, end);
	start.Y += TacticalRect.Y;
	end.Y += TacticalRect.Y;
	Point2D shadow_start = start;
	shadow_start.Y += 1;
	Point2D shadow_end = end;
	shadow_end.Y += 1;
	char shadow = (char)shadow_arg;
	if (shadow) {
		int black = NormalDrawer->Convert_Pixel(0);
		CompositeSurface->Fill_Rect(Intersect(TacticalRect, Moved_Rect(Rect(-2, -2, 3, 3), shadow_start)), black);
		black = NormalDrawer->Convert_Pixel(0);
		CompositeSurface->Fill_Rect(Intersect(TacticalRect, Moved_Rect(Rect(-2, -2, 3, 3), shadow_end)), black);
	}
	Rect clip = TacticalRect;
	if (shadow && Clip_Line_To_Rect(shadow_start, shadow_end, clip)) {
		CompositeSurface->Draw_Line(shadow_start, shadow_end, NormalDrawer->Convert_Pixel(0));
	}
	int pixel = ((color.Get_Red() >> DSurface::RedLeft) << DSurface::RedRight) | ((color.Get_Blue() >> DSurface::BlueLeft) << DSurface::BlueRight) | ((color.Get_Green() >> DSurface::GreenLeft) << DSurface::GreenRight);
	CompositeSurface->Fill_Rect(Intersect(TacticalRect, Moved_Rect(Rect(-2, -2, 3, 3), start)), pixel);
	CompositeSurface->Fill_Rect(Intersect(TacticalRect, Moved_Rect(Rect(-2, -2, 3, 3), end)), pixel);
	if ((char)dashed_arg) {
		int offset = (0x7fffffff - Frame) % 15;
		if (Clip_Line_To_Rect(start, end, clip)) {
			CompositeSurface->Draw_Masked_Dashed_Line(start, end, pixel, (bool *)ActionLineDash, offset, false);
		}
	} else {
		if (Clip_Line_To_Rect(start, end, clip)) {
			CompositeSurface->Draw_Line(start, end, pixel);
		}
	}
}


/// <summary>
/// Draws a link line between two spots in 32 pieces whose brightness runs along the line over time.
/// </summary>
#ifdef RA2_NON_MATCHING // not exact: no independent bound yet (006d0990 has no row); 1033 bytes differ against main's function-map extent; main: best 86.96, tried 6 times (frame, register, inline)
void TechnoClass::Draw_Link_Line(Coord start, Coord end, RGBClass color) const // RA2: not in TibSun, at 0x6D0990
{
	RGBClass white(255, 255, 255);
	Point2D start_point;
	Point2D end_point;
	TacticalMap->Coord_To_Pixel(start, start_point);
	TacticalMap->Coord_To_Pixel(end, end_point);
	start_point.Y += TacticalRect.Y;
	end_point.Y += TacticalRect.Y;
	int pixel = DSurface::Build_Hicolor_Pixel(color);
	CompositeSurface->Fill_Rect(Intersect(Line_End_Rect(start_point), TacticalRect), pixel);
	CompositeSurface->Fill_Rect(Intersect(Line_End_Rect(end_point), TacticalRect), pixel);
	Rect clip = TacticalRect;
	int phase = (timeGetTime() >> 4) << 3;
	Coord last = start;
	for (int i = 1, j = 31, k = 8; k <= 256; i++, j--, k += 8) {
		Coord next;
		next.X = (end.X * i + start.X * j) / 32;
		next.Y = (end.Y * i + start.Y * j) / 32;
		double t = (i - 16.0) * 0.0625;
		int z = (end.Z * i + start.Z * j) / 32;
		double arc = -(t * t);
		next.Z = (int)(z + (arc + 1.0) * 512.0);
		TacticalMap->Coord_To_Pixel(last, start_point);
		TacticalMap->Coord_To_Pixel(next, end_point);
		start_point.Y += TacticalRect.Y;
		end_point.Y += TacticalRect.Y;
		if (Clip_Line_To_Rect(start_point, end_point, clip)) {
			RGBClass shade = color;
			unsigned ratio = (k - phase) & 0x1F8;
			if (ratio < 0xF9 && ratio != 0) {
				shade.Adjust(ratio, white);
			}
			CompositeSurface->Draw_Line(start_point, end_point, DSurface::Build_Hicolor_Pixel(shade));
		}
		last = next;
	}
}
#endif


/// <summary>
/// Draws a line between two spots in 64 pieces that fade from the colour to black, with end markers
/// and an optional drop shadow.
/// </summary>
void TechnoClass::Draw_Shaded_Line(Coord from, Coord to, RGBClass color, bool shadow) const
{
	RGBClass shade;
	Point2D start;
	Point2D end;
	TacticalMap->Coord_To_Pixel(from, start);
	TacticalMap->Coord_To_Pixel(to, end);
	start.Y += TacticalRect.Y;
	end.Y += TacticalRect.Y;
	int pixel = DSurface::Build_Hicolor_Pixel(color);
	int fade = DSurface::Build_Hicolor_Pixel(BlackColor);
	Point2D shadow_start = start;
	shadow_start.Y += 1;
	Point2D shadow_end = end;
	shadow_end.Y += 1;
	if (shadow) {
		CompositeSurface->Fill_Rect(Intersect(TacticalRect, Moved_Rect(Rect(-2, -2, 3, 3), shadow_start)), pixel);
		CompositeSurface->Fill_Rect(Intersect(TacticalRect, Moved_Rect(Rect(-2, -2, 3, 3), shadow_end)), fade);
	}
	Rect clip = TacticalRect;
	if (shadow && Clip_Line_To_Rect(shadow_start, shadow_end, clip)) {
		CompositeSurface->Draw_Line(shadow_start, shadow_end, NormalDrawer->Convert_Pixel(0));
	}
	CompositeSurface->Fill_Rect(Intersect(TacticalRect, Line_End_Rect(start)), fade);
	CompositeSurface->Fill_Rect(Intersect(TacticalRect, Line_End_Rect(end)), pixel);
	Coord last = from;
	Coord next;
	int i = 1;
	int j = 63;
	while (i <= 64) {
		next.X = (from.X * j + to.X * i) / 64;
		next.Y = (from.Y * j + to.Y * i) / 64;
		next.Z = (from.Z * j + to.Z * i) / 64;
		TacticalMap->Coord_To_Pixel(last, start);
		TacticalMap->Coord_To_Pixel(next, end);
		start.Y += TacticalRect.Y;
		end.Y += TacticalRect.Y;
		if (Clip_Line_To_Rect(start, end, clip)) {
			shade = color;
			int ratio = j * 4;
			shade.Adjust(ratio < 0xfe ? ratio : 0xfe, BlackColor);
			CompositeSurface->Draw_Line(start, end, DSurface::Build_Hicolor_Pixel(shade));
		}
		last = next;
		i++;
		j--;
	}
}





/// <summary>
/// Fetches the spot this object attacks: its target's while it is attacking, else its own.
/// </summary>
Coord TechnoClass::GetAttackCoordinates(void) const // RA2: not in TibSun, at 0x6D13B0
{
	if (Get_Mission() == MISSION_ATTACK && TarCom != NULL) {
		return(TarCom->As_Coord());
	}
	return(Position);
}


/// <summary>
/// Sets the planning token this object follows.
/// </summary>
void TechnoClass::Set_Planning_Token(PlanningTokenClass * token) // RA2: not in TibSun, at 0x6D1420
{
	PlanningToken = token;
}


/// <summary>
/// Fetches the planning token this object follows.
/// </summary>
PlanningTokenClass * TechnoClass::Get_Planning_Token(void) // RA2: not in TibSun, at 0x6D1430
{
	return(PlanningToken);
}


/// <summary>
/// Does this object have planning nodes to follow?
/// </summary>
bool TechnoClass::Has_Planning_Nodes(void) // RA2: not in TibSun, at 0x6D1440
{
	if (PlanningToken == NULL) {
		return(false);
	}
	return(PlanningToken->Node_Count() > 0 ? true : false);
}


/// <summary>
/// Is this object not warping in? RA2 objects never are.
/// </summary>
bool TechnoClass::IsNotWarpingIn(void) const // RA2: not in TibSun, at 0x6D1460
{
	return(true);
}


/// <summary>
/// Moves on to the next node of this object's planning token.
/// </summary>
bool TechnoClass::Next_Planning_Node(void) // RA2: not in TibSun, at 0x6D1470
{
	return(TryNextPlanningTokenNode());
}


/// <summary>
/// Fetches the colour converter this object draws with: its type's own palette, the disguise house's
/// scheme when the player cannot see through the disguise, else its house's scheme.
/// </summary>
void * TechnoClass::GetRemapColour(void) const // RA2: not in TibSun, at 0x6D1480
{
	TechnoTypeClass const * type = Techno_Type_Class();
	int index = House->Scheme;
	DynamicVectorClass<ColorScheme *> * palette = type->Palette;
	if (palette != NULL) {
		return((*palette)[index]->Converter);
	}
	if (!IsClearlyVisibleTo(PlayerPtr) && GetDisguiseHouse(true) != NULL) {
		return(ColorSchemes[GetDisguiseHouse(true)->Scheme]->Converter);
	}
	return(ColorSchemes[index]->Converter);
}


/***********************************************************************************************
 * TechnoClass::Techno_Draw_Object -- General purpose draw object routine.                     *
 *                                                                                             *
 *    This routine is used to draw the object. It will handle any remapping or cloaking        *
 *    effects required. This logic is isolated here since all techno object share the same     *
 *    render logic when it comes to remapping and cloaking.                                    *
 *                                                                                             *
 * INPUT:   shapefile   -- Pointer to the shape file that the shape will be drawn from.        *
 *                                                                                             *
 *          shapenum    -- The shape number of the object in the file to use.                  *
 *                                                                                             *
 *          x,y         -- Center pixel coordinate to use for rendering this object.           *
 *                                                                                             *
 *          window      -- The clipping window to use when rendering.                          *
 *                                                                                             *
 *          rotation    -- The rotation of the object.                                         *
 *                                                                                             *
 *          scale       -- The scaling factor to use (24.8 fixed point).                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1995 JLB : Created.                                                                 *
 *   01/11/1996 JLB : Added rotation and scaling.                                              *
 *=============================================================================================*/
void TechnoClass::Techno_Draw_Object(ShapeSet const * shapefile, int shapenum, Point2D const & xdrawpoint, Rect const & rect, Dir256 rotation, int scale, int xzadjust, ZGradientType zgrad, bool zwrite, int brightness, ShapeSet const * zshapefile, int zshapenum, Point2D zoff, ShapeFlags_Type negflags) const
{
	if (shapefile != NULL) {
		bool shadow = DrawShapeShadows;
		VisualType visual = Visual_Character(false, NULL); // RA2: both arguments passed

		unsigned flags = SHAPE_NORMAL; // RA2: the flags are kept as a plain number
		if (negflags == SHAPE_NORMAL) {
			switch (visual) { // RA2: the visual character picks the translucency directly
				case VISUAL_RIPPLE: // RA2: as above
					if (CloakingDevice.Fetch_Stage()) { // RA2: as above
						flags = SHAPE_TRANSLUCENT50; // RA2: as above
						break; // RA2: as above
					}
				case VISUAL_INDISTINCT: // RA2: as above
					flags = SHAPE_TRANSLUCENT25; // RA2: as above
					break; // RA2: as above

				case VISUAL_DARKEN:
				case VISUAL_SHADOWY:
					flags = SHAPE_TRANSLUCENT50; // RA2: as above
					break;

				case VISUAL_HIDDEN:
					return;
			}
		}

		if (IsBeingWarpedOut() || IsWarpingIn()) { // RA2: a unit being warped is translucent
			flags |= SHAPE_TRANSLUCENT50; // RA2: as above
		}

		ConvertClass * converter;
		if (Fetch_RTTI() == RTTI_BUILDING && ((BuildingClass *)this)->Class->IsTerrainPalette) { // RA2: the virtual RTTI
			CellClass * cptr = &Map[Center_Coord().As_Cell()];
			converter = cptr->Drawer;
			if (converter == NULL) {
				cptr->Init_Drawer(NULL, 0x10000, 0, 1000, 1000, 1000); // RA2: all six arguments
				converter = cptr->Drawer;
				if (converter == NULL) return;
			}
			brightness = cptr->TileBrightness;
		} else {
			if (Fetch_RTTI() == RTTI_UNIT && ((UnitClass const *)this)->IsCompositingToEightBitSurface) { // RA2: as above
				converter = EightBitDrawer;
			} else {
				converter = (ConvertClass *)GetRemapColour(); // RA2: the remap virtual replaces the house scheme lookup
			}
		}

		Point2D drawpoint = xdrawpoint;
		int zadjust = xzadjust;

		switch (Fetch_RTTI()) { // RA2: the virtual RTTI
			case RTTI_UNIT:
				if (((UnitClass *)this)->Class->IsSmallVisceroid || ((UnitClass *)this)->Class->IsLargeVisceroid) {
					shadow = false;
				}
			case RTTI_INFANTRY: // RA2: units fall into the infantry case
				if (HeightAGL == 0) {
					zadjust += Get_Z_Adjust();
				} else {
					zadjust -= TacticalMap->Z_Lepton_To_Pixel(Get_Height()); // RA2: as above
					shadow = false; // RA2: as above
				}
				break;

			case RTTI_AIRCRAFT:
				drawpoint.Y -= TacticalMap->Z_Lepton_To_Pixel(Get_Height()); // RA2: as above
				zadjust -= TacticalMap->Z_Lepton_To_Pixel(Get_Height()); // RA2: as above
				break;
		}

		if (Techno_Type_Class()->NoShadow) { // RA2: a type without shadow
			shadow = false; // RA2: as above
		}

		if (!IsClearlyVisibleTo(PlayerPtr)) { // RA2: a disguised object looks like what it imitates
			if (GetDisguise(true)->Fetch_RTTI() != RTTI_OVERLAYTYPE && GetDisguise(true)->Fetch_RTTI() != RTTI_TERRAINTYPE) { // RA2: as above
				converter = ColorSchemes[GetDisguiseHouse(true)->Scheme]->Converter; // RA2: as above
			} else {
				CellClass * cptr = (CellClass *)Get_Cell_Ptr(); // RA2: as above
				converter = cptr->Drawer; // RA2: as above
				if (converter == NULL) { // RA2: as above
					cptr->Init_Drawer(NULL, 0x10000, 0, 1000, 1000, 1000); // RA2: as above
					converter = cptr->Drawer; // RA2: as above
				}
				brightness = cptr->TileBrightness; // RA2: as above
			}
			if (converter == NULL) { // RA2: as above
				converter = ColorSchemes[PlayerPtr->Scheme]->Converter; // RA2: as above
			}
		}

		if (zgrad != ZGRAD_NONE) flags |= SHAPE_ZGRAD; // RA2: the flags are a plain number
		if (zwrite) flags |= SHAPE_ZWRITE; // RA2: the flags are a plain number
		flags |= SHAPE_ALPHA; // RA2: the flags are a plain number

		if (Fetch_RTTI() == RTTI_UNIT && ((UnitClass const *)this)->IsCompositingToEightBitSurface) { // RA2: as above
			flags |= SHAPE_NOTRANS; // RA2: the flags are a plain number
			shadow = false;
		}

		flags &= ~negflags; // RA2: the flags are a plain number

		if (Fetch_RTTI() == RTTI_UNIT && ((UnitClass const *)this)->IsCompositingToEightBitSurface) { // RA2: as above
			Rect shaperect = shapefile->Get_Rect(shapenum);
			Rect drawrect;
			drawrect.X = shaperect.X + drawpoint.X - shapefile->Get_Width() / 2;
			drawrect.Y = shaperect.Y + drawpoint.Y - shapefile->Get_Height() / 2;
			drawrect.Width = shaperect.Width;
			drawrect.Height = shaperect.Height;
			UnitCompositeDirtyRect = Union(UnitCompositeDirtyRect, drawrect);
		}

		if (House->Is_Player_Control() && IsDisguised()) { // RA2: the player's own disguised unit gets the disguise flags
			flags = GetDisguiseFlags(flags); // RA2: as above
		}

		brightness = Apparent_Brightness(brightness);
		if (Unknown_Virtual_32C()) { // RA2: an invulnerable object is tinted
			brightness = GetEffectTintIntensity(brightness); // RA2: as above
		}

		switch (visual) {
			case VISUAL_NORMAL:
				Draw_Shape(*LogicalSurface, *converter, shapefile, shapenum, drawpoint, rect, ShapeFlags_Type(flags|SHAPE_CENTER|SHAPE_WIN_REL), NULL, zadjust - 2, zgrad, brightness, zshapefile, zshapenum, zoff);
				if (shadow) {
					zadjust = -2 - TacticalMap->Z_Lepton_To_Pixel(Get_Height()); // RA2: as above
					if (IsInTransport) {
						drawpoint.Y -= 14;
					}
					flags &= ~(SHAPE_TRANSLUCENT75); // RA2: the translucency is dropped for the shadow
					Draw_Shape(*LogicalSurface, *converter, shapefile, shapenum + shapefile->Get_Count() / 2, drawpoint, rect, ShapeFlags_Type(flags|SHAPE_DARKEN|SHAPE_CENTER|SHAPE_WIN_REL), NULL, zadjust - 2, ZGRAD_GROUND, 1000, NULL, 0, Point2D(0, 0)); // RA2: the last arguments are passed explicitly
				}
				break;

			case VISUAL_INDISTINCT:
				Draw_Shape(*LogicalSurface, *converter, shapefile, shapenum, drawpoint, rect, ShapeFlags_Type(flags|SHAPE_CENTER|SHAPE_WIN_REL), NULL, zadjust - 2, zgrad, brightness, zshapefile, zshapenum, zoff);
				if (shadow) { // RA2: the local flag, not the global
					zadjust = -2 - TacticalMap->Z_Lepton_To_Pixel(Get_Height()); // RA2: as above
					flags &= ~(SHAPE_TRANSLUCENT75); // RA2: as above
					if (IsInTransport) {
						drawpoint.Y -= 14;
					}
					Draw_Shape(*LogicalSurface, *converter, shapefile, shapenum + shapefile->Get_Count() / 2, drawpoint, rect, ShapeFlags_Type(flags|SHAPE_DARKEN|SHAPE_CENTER|SHAPE_WIN_REL), NULL, zadjust - 2, ZGRAD_GROUND, 1000, NULL, 0, Point2D(0, 0)); // RA2: as above
				}
				break;

			case VISUAL_DARKEN:
			case VISUAL_RIPPLE:
				Draw_Shape(*LogicalSurface, *converter, shapefile, shapenum, drawpoint, rect, ShapeFlags_Type(flags|SHAPE_CENTER|SHAPE_WIN_REL), NULL, zadjust - 2, zgrad, brightness, zshapefile, zshapenum, zoff);
				break;

			case VISUAL_SHADOWY:
				Draw_Shape(*LogicalSurface, *converter, shapefile, shapenum, drawpoint, rect, ShapeFlags_Type(flags|SHAPE_CENTER|SHAPE_WIN_REL), NULL, zadjust - 2, zgrad, brightness, zshapefile, zshapenum, zoff);
				break;
		}
	}
}

























/// <summary>
/// Draws this object's voxel image.
/// This routine picks the shape flags that suit the object's current visual character --
/// a cloaked object ripples, a shrouded one fades -- and then draws from the voxel index
/// cache when the caller offers a key, rendering and caching the image if it is not there
/// yet. An object that is hidden outright draws nothing.
/// </summary>
/// <param name="voxeldata">The voxel and motion libraries to render from.</param>
/// <param name="key">The cache slot to draw from, or -1 to render without caching.</param>
/// <param name="cache">The voxel index cache to look the image up in and store it back to.</param>
/// <param name="negflags">Shape flags to suppress. Pass SHAPE_NORMAL to let the visual
/// character choose the flags.</param>
void TechnoClass::Draw_Voxel(VoxelDataStruct const & voxeldata, int frame, int key, VoxelIndexClass * cache, Rect const & cliprect, Point2D const & draw_point, Matrix3D const & transform, int brightness, ShapeFlags_Type negflags) const
{
	unsigned flags = 0x2000;
	if (!negflags) {
		switch (Visual_Character(false, 0)) {
			case VISUAL_RIPPLE: {
            // Reference distinguishes ripple stages0,1,and later stages.
				int stage = CloakingDevice.Fetch_Stage();
				if (stage == 0) flags = 0x200a;
				else if (stage == 1) flags = 0x200c;
				else flags = 0x200c;
				break;
			}
			case VISUAL_INDISTINCT: flags = 0x2002; break;
			case VISUAL_DARKEN: case VISUAL_SHADOWY: flags = 0x2004; break;
			case VISUAL_HIDDEN: return;
		}
	}
	if ((IsBeingWarpedOut() || IsWarpingIn()) && (Fetch_RTTI() != RTTI_UNIT || !((UnitClass const *)this)->IsCompositingToEightBitSurface)) flags |= 0x2004;
	if (House->Is_Player_Control() && IsDisguised() && (Fetch_RTTI() != RTTI_UNIT || !((UnitClass const *)this)->IsCompositingToEightBitSurface)) flags = GetDisguiseFlags(flags);
	flags = (flags | 0x800) & ~negflags;
	Rect window = cliprect;
	brightness = Apparent_Brightness(brightness);
	if (Unknown_Virtual_32C()) brightness = GetEffectTintIntensity(brightness);
	if (key != -1) {
		if (!cache || Techno_Type_Class()->DisableVoxelCache) key = -1;
		else {
			StaticBufferClass::Entry * entry = (*cache)[key];
			if (entry) {
				Techno_Blit_Voxel(*entry, draw_point, window, ShapeFlags_Type(flags), brightness);
				return;
			}
		}
	}
	if (!voxeldata.VoxLib || !voxeldata.MotLib) return;
	SurfaceRegion region = Techno_Render_Voxel_Object(voxeldata, transform, draw_point, window, frame, ShapeFlags_Type(flags), brightness);
	int height = region.Bounds.Height;
	int width = region.Bounds.Width;
	if (Fetch_RTTI() == RTTI_UNIT) UnitCompositeDirtyRect = Union(UnitCompositeDirtyRect, Rect(region.Point.X + draw_point.X, region.Point.Y + draw_point.Y, width, height));
	if (key == -1) return;
	StaticBufferClass::Entry * data = VoxelStaticBuffer.Add(*VoxelDrawSystem::Get_Surface(), region);
	if (!data || !cache->Add_Index(key, data)) {
		Class_Of()->Clear_Voxel_Indexes();
		if (!data) data = VoxelStaticBuffer.Add(*VoxelDrawSystem::Get_Surface(), region);
		cache->Add_Index(key, data);
	}
}


/// <summary>
/// Draws the voxel shadow for this object, honoring the cache key when present.
/// Skips drawing entirely if cloaked, mid-flash, or the voxel library failed to load.
/// Renders and caches the shadow when uncached, then redraws from cache.
/// </summary>
/// <param name="voxeldata">Voxel/motion library data for the object.</param>
/// <param name="layer_index">Voxel layer to render the shadow for.</param>
/// <param name="key">Cache key; -1 means do not use the cache.</param>
/// <param name="cache">Voxel index cache for the shadow entry.</param>
/// <param name="cliprect">Clipping rectangle for the shadow blit.</param>
/// <param name="point">Screen position to draw the shadow at.</param>
/// <param name="matrix">Transform matrix for the shadow.</param>
/// <param name="force_cache">Force rendering even when a cache key is supplied.</param>
void TechnoClass::Techno_Draw_Voxel_Shadow(VoxelDataStruct const & voxeldata, int layer_index, int key, VoxelIndexClass * cache, Rect const & cliprect, Point2D const & point, Matrix3D const & matrix, bool force_cache, Surface * surface, unsigned shadow_x, unsigned shadow_y) const // RA2: 0x6D21E0 takes the shadow surface and the two shadow offsets
{
	if (Cloak != UNCLOAKED || Techno_Type_Class()->NoShadow || voxeldata.VoxLib->Load_Failed()) { // RA2: a type without shadow draws none; no flash test
		return;
	}

	if (key != -1) {
		if (cache == NULL || Techno_Type_Class()->DisableShadowCache) { // RA2: a type that does not cache its shadow renders it every time
			key = -1; // RA2: as above
		} else {
			StaticBufferClass::Entry * entry = (*cache)[key]; // RA2: as above
			if (entry != NULL) { // RA2: as above
				Techno_Blit_Voxel(*entry, point, cliprect, ShapeFlags_Type(SHAPE_DARKEN|SHAPE_ZGRAD), NORMAL_LIGHT); // RA2: as above
				return; // RA2: as above
			}
			if (!force_cache) { // RA2: a missing entry is only rendered when asked to
				return; // RA2: as above
			}
		}
	}

	SurfaceRegion region = Techno_Render_Voxel_Shadow(voxeldata, matrix, point, cliprect, layer_index, ShapeFlags_Type(SHAPE_DARKEN|SHAPE_ZGRAD), key != -1, surface, shadow_x, shadow_y); // RA2: the surface and offsets are passed on
	if (key != -1) { // RA2: as above
		bool added = true; // RA2: as above
		StaticBufferClass::Entry * data = VoxelStaticBuffer.Add(*VoxelDrawSystem::Get_Surface(), region); // RA2: as above
		if (data == NULL || !cache->Add_Index(key, data)) { // RA2: as above
			added = false; // RA2: as above
			Class_Of()->Clear_Voxel_Indexes(); // RA2: as above
			if (data == NULL) { // RA2: as above
				data = VoxelStaticBuffer.Add(*VoxelDrawSystem::Get_Surface(), region); // RA2: as above
			}
		}
		if (!added) { // RA2: as above
			cache->Add_Index(key, data); // RA2: as above
		}

		Techno_Draw_Voxel_Shadow(voxeldata, layer_index, key, cache, cliprect, point, matrix, false, NULL, 0, 0); // RA2: the recursion passes no surface and no offsets
	}
}



/// <summary>
/// Renders a voxel object and blits the result to the screen.
/// This is the low level voxel routine that Draw_Voxel falls back on when there is no
/// cached image to use. The owning house's color scheme, the sinking clip and the predator
/// offset are all applied here, and the region returned is what the caller hands on to the
/// voxel cache.
/// </summary>
/// <param name="voxeldata">The voxel and motion libraries to render from.</param>
/// <returns>Returns with the surface region the object was rendered into.</returns>
#ifdef RA2_NON_MATCHING // not exact: 611 bytes differ (main-derived voxel effects)
SurfaceRegion TechnoClass::Techno_Render_Voxel_Object(VoxelDataStruct const & voxeldata, Matrix3D const & matrix, Point2D const & point, Rect const & xcliprect, int frame, ShapeFlags_Type flags, int brightness) const
{
	Matrix3D mtx = matrix;
	Matrix3D mtx2 = matrix;

	VoxelLibrary * voxlib = voxeldata.VoxLib;
	MotionLibrary * motlib = voxeldata.MotLib;

	VoxelDrawSystem::Precalculate_Light(voxlib, 0, 0, mtx2, VoxelCameraMatrix, VoxelLightSource, 3.0f); // RA2: view transform and specular strength
	VoxelDrawSystem::Reset();

	for (unsigned layer = 0; layer < voxlib->Get_Layer_Count(); layer++) {
		if (motlib != NULL) {
			Matrix3D mtx3 = motlib->Get_Layer_Matrix(layer, frame);
			mtx = mtx2 * mtx3;
		}
		VoxelDrawSystem::Prep_For_Object(voxlib, layer, 0, VoxelCameraMatrix * mtx);
	}

	SurfaceRegion region = VoxelDrawSystem::Render();
	if (IsBeingWarpedOut() || IsWarpingIn()) { // RA2: warping objects are translucent
		flags = ShapeFlags_Type(flags | SHAPE_TRANSLUCENT50);
	}
	if (House->Is_Player_Control() && IsDisguised()) { // RA2: apply the local disguise drawing flags
		flags = ShapeFlags_Type(GetDisguiseFlags(flags));
	}
	Rect cliprect = xcliprect;

	if (LogicalSurface != EightBitSurface) {
		if (SinkingYOffset > 0) {
			cliprect = Intersect(cliprect, Rect(0, 0, TacticalRect.Width, SinkingYOffset - TacticalMap->TacPixelY));
		} else if (IsSinking) {
			((TechnoClass *)this)->SinkingYOffset = (short)(TacticalMap->TacPixelY + point.Y + region.Bounds.Height + region.Point.Y); // RA2: includes the rendered origin
		}
	}

	flags = ShapeFlags_Type(flags & ~SHAPE_REMAP);
	ConvertClass * converter = NULL;
	if (Fetch_RTTI() == RTTI_UNIT && ((UnitClass const *)this)->IsCompositingToEightBitSurface) {
		converter = EightBitDrawer;
	} else {
		converter = (ConvertClass *)GetRemapColour(); // RA2: uses the object's current remap
	}

	int predoffset = 0;
	if (flags & SHAPE_PREDATOR) {
		predoffset = Get_Predator_Offset();
	}

	Blit_Block(*LogicalSurface, *converter, *VoxelDrawSystem::Get_Surface(), region.Bounds, region.Point + point, cliprect, NULL, converter->Blitter_From_Flags(flags), Get_Z_Adjust(), Get_Z_Gradient(), brightness, predoffset);

	return(region);
}
#endif


/// <summary>
/// Draws the shadow cast by one layer of a voxel object.
/// This routine serves the voxel drawing pass, which works through an object's layers and
/// asks each of them in turn for its shadow. A caller that is filling the voxel cache takes
/// the rendered region away instead and does its own drawing from it later.
/// </summary>
/// <param name="voxeldata">The voxel and motion libraries for this object.</param>
/// <param name="point">The screen position to draw the shadow at.</param>
/// <param name="layer_index">Which of the object's layers to cast the shadow for.</param>
/// <param name="cached">Is the caller keeping the shadow rather than drawing it now?</param>
/// <returns>Returns with the surface region the shadow was rendered into.</returns>
SurfaceRegion TechnoClass::Techno_Render_Voxel_Shadow(VoxelDataStruct const & voxeldata, Matrix3D const & matrix, Point2D const & point, Rect const & cliprect, int layer_index, ShapeFlags_Type flags, bool cached, Surface * surface, unsigned shadow_x, unsigned shadow_y) const // RA2: 0x6D2810 also takes the shadow surface and the two offsets and passes them on
{
	MotionLibrary * motlib = voxeldata.MotLib; // RA2: the motion library is kept
	Matrix3D combined; // RA2: the layer matrix is applied in its own block
	{
		Matrix3D section_matrix; // RA2: as above
		section_matrix = motlib->Get_Layer_Matrix(layer_index, 0); // RA2: as above
		combined = matrix * section_matrix; // RA2: as above
	}
	Matrix3D basis; // RA2: an aircraft's shadow is half size
	basis.Make_Identity(); // RA2: the basis matrix is set up first
	if (Techno_Type_Class()->ConsideredAircraft) { // RA2: as above
		basis.Scale(0.5f); // RA2: as above
	}

	VoxelDrawSystem::Reset();
	VoxelDrawSystem::Prep_For_Shadow(voxeldata.VoxLib, layer_index, 0, VoxelCameraMatrix * basis, combined, VoxelShadowLightVector, surface, shadow_x, shadow_y); // RA2: the surface and offsets of the caller
	SurfaceRegion region = VoxelDrawSystem::Render();

	if (!cached) {
		Rect r = VoxelDrawSystem::Get_Surface()->Get_Rect(); /// The result is unused.
		Point2D destination = region.Point + point; // RA2: the destination is a local
		Blit_Block(*LogicalSurface, *NormalDrawer, *VoxelDrawSystem::Get_Surface(), region.Bounds, Point2D(destination.X, destination.Y), cliprect, NULL, NormalDrawer->Blitter_From_Flags(flags), Get_Z_Adjust(), Get_Z_Gradient(), 1000, 0); // RA2: the depth gradient and the last arguments are passed explicitly
	}

	return(region);
}




/// <summary>
/// Draws a voxel image that was rendered earlier and kept.
/// This is the cheap path for an object whose appearance has not changed since the last
/// time it was drawn -- the rendering was paid for once and this routine merely puts the
/// finished image back on the screen.
/// </summary>
/// <param name="entry">The cached voxel image to draw.</param>
/// <param name="point">The screen position to draw it at.</param>
/// <param name="brightness">The brightness to draw the image with.</param>
void TechnoClass::Techno_Blit_Voxel(StaticBufferClass::Entry const & entry, Point2D const & point, Rect const & cliprect, ShapeFlags_Type flags, int brightness) const
{
	BSurface surface(entry.Width, entry.Height, 1, entry.Data);
	flags = ShapeFlags_Type(flags & ~SHAPE_REMAP);
	ConvertClass * converter;
	if (Fetch_RTTI() == RTTI_UNIT && static_cast<UnitClass const *>(this)->IsCompositingToEightBitSurface) converter = EightBitDrawer;
	else converter = (ConvertClass *)GetRemapColour();
	RLEBlitter const * blitter = converter->RLEBlitter_From_Flags(flags);
	int phase = 0;
	if (flags & 8) phase = (Fetch_ID() + (int)PredatorOffset) % 400;
	if (Fetch_RTTI() == RTTI_UNIT) {
		UnitCompositeDirtyRect = Union(UnitCompositeDirtyRect, Rect(entry.X + point.X, entry.Y + point.Y, entry.Width, entry.Height));
	}
	if (blitter) {
		RLE_Blit(*LogicalSurface, cliprect, Rect(entry.X + point.X, entry.Y + point.Y, entry.Width, entry.Height), surface, surface.Get_Rect(), surface.Get_Rect(), *blitter, Get_Z_Adjust(), Get_Z_Gradient(), brightness, phase, 0, Point2D(0, 0));
	}
}


/// <summary>
/// Determines where a sinking object should be clipped off.
/// This routine works out the screen row below which the object is no longer to be drawn,
/// so that it appears to slide beneath the surface rather than simply vanish. A few facings
/// need the waterline nudged up before they look right.
/// </summary>
/// <param name="height">Pixel height of the object's drawn image.</param>
/// <param name="y">The screen row the object is being drawn at.</param>
void TechnoClass::Calculate_Sinking_Offset(short height, int y)
{
	SinkingYOffset = TacticalMap->TacPixelY + height + y - 10;
	FacingType dir = PrimaryFacing.Current().As_Dir8();
	if (dir == FACING_N || dir == FACING_W) {
		SinkingYOffset -= 8;
	} else if (dir == FACING_NW) {
		SinkingYOffset -= 12;
	}
}




/***********************************************************************************************
 * TechnoClass::Detach -- Handles removal of target from tracking system.                      *
 *                                                                                             *
 *    This routine is called when the specified object is about to be removed from the game    *
 *    system. The target object is removed from any tracking computers that this object may    *
 *    have.                                                                                    *
 *                                                                                             *
 * INPUT:   target   -- The target object (as a target value) that is being removed from the   *
 *                      game.                                                                  *
 *                                                                                             *
 *          all      -- Is the target about to die? A false value might indicate that the      *
 *                      object is merely cloaking. In such a case, radio contact will not      *
 *                      be affected.                                                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void TechnoClass::Detach(AbstractClass const * target, bool all)
{
	BASECLASS::Detach(target, all);

	if (all) {
		Cargo.Detach((FootClass *)target);
	}

	bool clear_target = true;

	if (target == SpawnOwner) { // RA2: the spawner and the temporal warhead holder are forgotten
		SpawnOwner = NULL; // RA2: as above
	}
	if (target == TemporalTargetingMe) { // RA2: as above
		TemporalTargetingMe = NULL; // RA2: as above
	}

	bool techno; // RA2: the techno flag of the target is tested by byte, with a local
	if (!all && target != NULL) { // RA2: as above
		techno = (target->AbstractFlags & ABSTRACT_TECHNO); // RA2: as above
	}
	if (!all && target != NULL && techno) { // RA2: as above
		CellClass * cptr = &Map[target->Center_Coord()];
		if (cptr->Is_Sensed(House->HeapID)) {
			clear_target = false;
		}
	}

	/*
	**	If the targeting computer is assigned to the target, then the targeting
	**	computer must be cleared.
	*/
	if (TarCom == target && clear_target && (all || target->Owner_HouseClass() != House)) {
		if (TargetingTimer.Value() > 10 && ScenarioInit == 0) { // RA2: the next scan comes soon
			TargetingTimer = Random_Pick(4, 8); // RA2: as above
		}
		Assign_Target(NULL);
		if (Has_Suspended_Mission()) {
			Restore_Mission();
			if (Fetch_RTTI() == RTTI_AIRCRAFT && CurrentMission == MISSION_PATROL) { // RA2: the type is asked by the virtual
				Status = 0;
				((AircraftClass *)this)->IsLockedStraight = false;
			}
		}
	}

	if (SuspendedTarCom == target && clear_target) {
		SuspendedTarCom = NULL;
	}

	if (House == target) {
		House = NULL;
		IsOwnedByPlayer = false; // RA2: as above
	}

	if (IsDisguised() && GetDisguiseHouse(true) == target) { // RA2: a disguise as the detached house is dropped
		ClearDisguise(); // RA2: as above
	}

	for (int slot = 0; slot < 8; slot++) { // RA2: eight particle slots
		if (ParticleSystems[slot] == target) { // RA2: as above
			ParticleSystems[slot] = NULL; // RA2: as above
		}
	}

	if (Wave == target) {
		Wave = NULL;
	}

	if (all) {
		if (QueueUpToEnter == target) { // RA2: QueueUpToEnter, the object to enter once nearby
			QueueUpToEnter = NULL; // RA2: QueueUpToEnter, the object to enter once nearby
		}
		if (ArchivedTarget == target) { // RA2: as above
			ArchivedTarget = NULL; // RA2: as above
		}
		if (CaptureManager != NULL) { // RA2: the manager lets go of it too
			CaptureManager->Detach_Target(target); // RA2: as above
		}
	}
	if (SpawnManager != NULL) { // RA2: as above
		SpawnManager->UnlinkPointer((AbstractClass *)target); // RA2: as above
	}
	if (TemporalImUsing != NULL) { // RA2: as above
		TemporalImUsing->Detach_Techno(target); // RA2: as above
	}
	if (target == OldTeam) { // RA2: as above
		OldTeam = NULL; // RA2: as above
	}
	if (target == BehindAnim) { // RA2: as above
		BehindAnim = NULL; // RA2: as above
	}

	/*
	**	The removals take a temporary copy of the pointer, never the parameter's own slot.
	*/
	int index = -1; // RA2: the target is looked up in the current targets, its threat value goes with it
	for (int i = 0; i < CurrentTargets.Count(); i++) { // RA2: as above
		if (target == CurrentTargets[i]) { // RA2: as above
			index = i; // RA2: as above
			break; // RA2: as above
		}
	}
	if (index != -1) { // RA2: as above
		CurrentTargetThreatValues.Delete_Index(index); // RA2: as above
		CurrentTargets.Delete((AbstractClass *)target); // RA2: as above
	}
	AttackedTargets.Delete((AbstractClass *)target); // RA2: as above
}



/***********************************************************************************************
 * TechnoClass::Kill_Cargo -- Destroys any cargo attached to this object.                      *
 *                                                                                             *
 *    This routine handles the destruction of any cargo this object may contain. Typical of    *
 *    this would be when a transport helicopter gets destroyed.                                *
 *                                                                                             *
 * INPUT:   source   -- The source of the destruction of the cargo.                            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void TechnoClass::Kill_Cargo(TechnoClass * source)
{
	while (Cargo.Is_Something_Attached()) {
		Cargo.Attached_Object()->Remove_From_Team();
		FootClass * foot = Cargo.Detach_Object();
		if (foot != NULL) {
			foot->Record_The_Kill(source);
			delete foot;
		}
	}
}


/***********************************************************************************************
 * TechnoClass::Crew_Type -- Fetches the kind of crew this object contains.                    *
 *                                                                                             *
 *    This routine is called when generating survivors to this object. This routine returns    *
 *    the type of survivor to generate.                                                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the infantry type of a survivor.                                           *
 *                                                                                             *
 * WARNINGS:   This routine is designed to be called repeatedly. Once for each survivor to     *
 *             generate.                                                                       *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
InfantryTypeClass const * TechnoClass::Crew_Type(void) const
{
	/*
	**	If this object contains no crew, then there can be no
	**	crew inside, duh... return this news.
	*/
	if (!TClass->IsCrew) {
		return(NULL);
	}

	/*
	**	The normal infantry survivor is the standard issue
	**	minigunner. Certain buildings, especially neutral ones, tend to have
	**	civilians exit them instead.
	*/
	InfantryTypeClass const * crew; // RA2: the crew type depends on the house's ActLike
	if (House->ActLike == 0) { // RA2: Allied crew
		crew = Rule->AlliedCrew; // RA2: as above
	} else if (House->ActLike == 1) { // RA2: Soviet crew
		crew = Rule->SovietCrew; // RA2: as above
	} else {
		crew = Rule->Technician; // RA2: everybody else
	}
	if (House->Class->Side != SIDE_NONE) {
		if (Is_Weapon_Equipped() && Percent_Chance(15)) {
			crew = Rule->Technician; // RA2: armed objects sometimes leave a technician
		}
	} else {
		crew = Rule->Technician; // RA2: no side, technician
	}
	return(crew); // RA2: as above
}
 // RA2: no infantry (not in RA2's layout)


/***********************************************************************************************
 * TechnoClass::Value -- Fetches the target value for this object.                             *
 *                                                                                             *
 *    This routine is used to fetch the target value for this object. The greater the value    *
 *    returned, the better this object is as a target.                                         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the target value for this object.                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *   08/16/1995 JLB : Adjusted for early mission lame-out.                                     *
 *=============================================================================================*/
int TechnoClass::Value(void) const
{
	int value = 0;

	if (Rule->Diff[House->Difficulty].IsContentScan || House->IQ >= Rule->IQContentScan) {
		if (Cargo.Is_Something_Attached()) {
			FootClass * object = Cargo.Attached_Object();

			while (object != NULL) {
				value += object->Value();
				object = (FootClass *)(ObjectClass *)object->Next;
			}
		}
	}

	return(Risk() + TClass->Reward + value);
}


/***********************************************************************************************
 * TechnoClass::Threat_Range -- Returns the range to scan based on threat control.             *
 *                                                                                             *
 *    This routine will return the range to scan based on the control value specified. The     *
 *    value returned by this routine is typically used when scanning for enemies.              *
 *                                                                                             *
 * INPUT:   control  -- The range control parameter.                                           *
 *                      0  = Use weapon range (zero is returned in this special case).         *
 *                      -1 = Scan without range restrictions (-1 is returned in this case).    *
 *                      1  = Scan up to twice weapon range.                                    *
 *                                                                                             *
 * OUTPUT:  Returns with a range (or special value) that can be used in the threat scan        *
 *          process. If zero is returned, then always check threat against In_Range(). If      *
 *          -1 is returned, then no range limitation restriction exists.                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int TechnoClass::Threat_Range(int control) const
{
	if (control == -1) return(-1);

	if (control == 0) {
		bool is_renovator = Is_Renovator();
		if (TClass->ThreatRange != 0 && !is_renovator) {
			return(TClass->ThreatRange);
		}
		return(0);
	}

	int range = TClass->ThreatRange;
	if (range == 0) {
		range = MAX(Weapon_Range(0), Weapon_Range(1));
	}

	range *= 2;
	if (control == 2) {
		range = Bound(range, 7 * CELL_LEPTON, 16 * CELL_LEPTON);
	} else {
		range = Bound(range, 0, 16 * CELL_LEPTON);
	}

	return(range);
}


/***********************************************************************************************
 * TechnoClass::Is_In_Same_Zone -- Determine if specified cell is in same zone as object.      *
 *                                                                                             *
 *    This will examine the specified cell to determine if it is in the same zone as this      *
 *    object's location.                                                                       *
 *                                                                                             *
 * INPUT:   cell  -- The cell that is to be checked against this object's current location.    *
 *                                                                                             *
 * OUTPUT:  bool; Is the specified cell in the same zone as this object is?                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoClass::Is_In_Same_Zone(Coord const & coord) const
{
	MZoneType zone = TClass->MZone;

	if (zone != MZONE_NONE) {
		if (coord != COORD_NONE) {
			Cell cell1 = coord.As_Cell();
			Cell cell2 = Destination_Coord(NULL).As_Cell(); // RA2: Is_In_Same_Zone at 0x6D32E0 matches only so

			return(Map.Is_Same_Cell_Zone(cell2, cell1, zone, ((ObjectClass *)this)->Is_Moving_Onto_Bridge(NULL), Map[cell1].IsUnderBridge, Is_Allowed_To_Leave_Map())); // RA2: as above
		}
	}
	return(true);
}


/// <summary>
/// RA2
/// </summary>
static inline bool Is_Object(AbstractClass const * a)
{
	if (a == NULL) return(false);
	unsigned char flag = (a->AbstractFlags >> 1) & 1;
	return(flag != 0);
}


/***********************************************************************************************
 * TechnoClass::Base_Is_Attacked -- Handle panic response to base being attacked.              *
 *                                                                                             *
 *    This routine is called when the base is being attacked. It will pull units off of the    *
 *    field and send them back to defend the base. This routine will make taking an enemy      *
 *    base much more difficult.                                                                *
 *                                                                                             *
 * INPUT:   enemy -- Pointer to the enemy object that did the damage on the base.              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine can drastically affect the game play. The computer will probably   *
 *             call off its attacks as a result.                                               *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Commented.                                                               *
 *   10/15/1996 JLB : Alternates between guard area and attack.                                *
 *   11/01/1996 JLB : Allow recruit of guard area units in multiplay.                          *
 *=============================================================================================*/
void TechnoClass::Base_Is_Attacked(TechnoClass const * enemy)
{
	FootClass * defender[6];
	int value[ARRAY_SIZE(defender)];
	memset(defender, '\0', sizeof(defender)); // RA2: as above
	memset(value, '\0', sizeof(value));

	int count = 0;
	int weakest = 0;
	int desired = enemy->Risk() * Rule->ComputerBaseDefenseResponse;
	int risktotal = 0;

	if (enemy == NULL || House->Is_Ally(enemy) || House->Is_Human_Player() || enemy->IsInLimbo) { // RA2: an enemy in limbo is ignored too
		return;
	}

	if (Session.Type == GAME_NORMAL && Is_Weapon_Equipped()) return;

	if (enemy->Fetch_RTTI() != RTTI_INFANTRY && enemy->Fetch_RTTI() != RTTI_UNIT) { // RA2: the virtual RTTI
		return;
	}

	if (Techno_Type_Class()->IsInsignificant) { // RA2: the virtual type
		return;
	}

	bool is_foot; // RA2: the foot test is kept in a local, left unset for a null enemy
	if (enemy != NULL) { // RA2: as above
		is_foot = (enemy->AbstractFlags & ABSTRACT_FOOT); // RA2: as above
	}
	if (enemy != NULL && is_foot && ((FootClass *)enemy)->BaseAttackTimer != 0) { // RA2: as above
		return;
	}

	TeamClass::Suspend_Teams(Rule->SuspendPriority, House);

	for (int index = 0; index < Infantry.Count(); index++) { // RA2: the limit is tested inside the loop
		if (desired <= 0) { // RA2: as above
			break; // RA2: as above
		}
		InfantryClass * infantry = Infantry[index]; // RA2: as above
		if (infantry != NULL && infantry->IsActive && infantry->Owner() == Owner()) {
			bool basedefense = infantry->Team != NULL && infantry->Team->Class->IsBaseDefense;
			if ((infantry->Team == NULL || basedefense) && infantry->IsTeamRecruitable && infantry->Is_Weapon_Equipped() && infantry->IsAutocreateRecruitable && (infantry->Current_Mission_Control().IsRecruitable || Session.Type != GAME_NORMAL)) { // RA2: one condition instead of continues
				int armor = enemy->Techno_Type_Class()->Armor; // RA2: as above
				if (infantry->GetTurretWeapon()->Weapon->WarheadPtr->Modifier[armor] != 0.0) { // RA2: the turret weapon
					Cell from = Destination_Coord().As_Cell(); // RA2: as above
					Cell to = infantry->Destination_Coord().As_Cell(); // RA2: as above
					if (Map.Is_Same_Cell_Zone(to, from, infantry->Class->MZone, infantry->Is_Moving_Onto_Bridge(), false, false)) { // RA2: as above
						int threat = infantry->Rescue_Mission((AbstractClass *)enemy); // RA2: as above
						if (threat) { // RA2: as above
							if (ArchiveTarget == this) { // RA2: as above
								threat *= 100; // RA2: as above
							}
							if (threat < 0) { // RA2: as above
								desired += threat; // RA2: as above
							} else if (count < ARRAY_SIZE(defender)) { // RA2: else-if chains and own counters as the executable lays them out
								defender[count] = infantry; // RA2: as above
								value[count] = threat; // RA2: as above
								count++; // RA2: as above
							} else if (threat > weakest) { // RA2: else-if chains and own counters as the executable lays them out
								int newweakest = threat; // RA2: as above
								for (int lp = 0; lp < count; lp++) { // RA2: as above
									if (value[lp] == weakest) { // RA2: as above
										value[lp] = threat; // RA2: as above
										defender[lp] = infantry; // RA2: else-if chains and own counters as the executable lays them out
									} else if (value[lp] < newweakest) { // RA2: else-if chains and own counters as the executable lays them out
										newweakest = value[lp]; // RA2: as above
									}
								}
								weakest = newweakest; // RA2: as above
							}
						}
					}
				}
			}
		}
	}

	for (int unit_index = 0; unit_index < Units.Count(); unit_index++) { // RA2: a second counter
		if (desired <= 0) { // RA2: as above
			break; // RA2: as above
		}
		UnitClass * unit = Units[unit_index]; // RA2: else-if chains and own counters as the executable lays them out
		if (unit != NULL && unit->IsActive) { // RA2: the owner is tested next
			int unit_owner = unit->Owner(); // RA2: as above
			if (unit_owner != Owner()) { // RA2: as above
				continue;
			}
			bool basedefense = unit->Team != NULL && unit->Team->Class->IsBaseDefense; // RA2: as above
			if ((unit->Team == NULL || basedefense) && unit->IsTeamRecruitable && unit->Is_Weapon_Equipped() && unit->IsAutocreateRecruitable && (unit->Current_Mission_Control().IsRecruitable || Session.Type != GAME_NORMAL)) { // RA2: as above
				int armor = enemy->Techno_Type_Class()->Armor; // RA2: as above
				if (unit->GetTurretWeapon()->Weapon->WarheadPtr->Modifier[armor] != 0.0) { // RA2: as above
					Cell from = Destination_Coord().As_Cell(); // RA2: as above
					Cell to = unit->Destination_Coord().As_Cell(); // RA2: as above
					if (Map.Is_Same_Cell_Zone(to, from, unit->Class->MZone, unit->Is_Moving_Onto_Bridge(), false, false)) { // RA2: as above
						int threat = unit->Rescue_Mission((AbstractClass *)enemy); // RA2: as above
						if (threat) { // RA2: as above
							if (threat > 0 && ArchiveTarget == this) { // RA2: as above
								threat *= 10; // RA2: as above
							}
							if (threat < 0) { // RA2: as above
								desired += threat; // RA2: as above
							} else if (count < ARRAY_SIZE(defender)) { // RA2: else-if chains and own counters as the executable lays them out
								defender[count] = unit; // RA2: as above
								value[count] = threat; // RA2: as above
								count++; // RA2: as above
							} else if (threat > weakest) { // RA2: else-if chains and own counters as the executable lays them out
								int newweakest = threat; // RA2: as above
								for (int lp = 0; lp < count; lp++) { // RA2: as above
									if (value[lp] == weakest) { // RA2: as above
										value[lp] = threat; // RA2: as above
										defender[lp] = unit; // RA2: else-if chains and own counters as the executable lays them out
									} else if (value[lp] < newweakest) { // RA2: else-if chains and own counters as the executable lays them out
										newweakest = value[lp]; // RA2: as above
									}
								}
								weakest = newweakest; // RA2: as above
							}
						}
					}
				}
			}
		}
	}

	if (desired > 0) {
		for (int first = 0; first < count - 1; first++) { // RA2: own counters
			for (int second = first + 1; second < count; second++) { // RA2: as above
				if (value[first] < value[second]) { // RA2: else-if chains and own counters as the executable lays them out
					value[first] ^= value[second]; // RA2: else-if chains and own counters as the executable lays them out
					value[second] ^= value[first]; // RA2: else-if chains and own counters as the executable lays them out
					value[first] ^= value[second]; // RA2: else-if chains and own counters as the executable lays them out

					FootClass * temp = defender[first]; // RA2: else-if chains and own counters as the executable lays them out
					defender[first] = defender[second]; // RA2: else-if chains and own counters as the executable lays them out
					defender[second] = temp; // RA2: else-if chains and own counters as the executable lays them out
				}
			}
		}

		for (int chosen = 0; chosen < count; chosen++) { // RA2: as above
			FootClass * member = defender[chosen]; // RA2: the responder is kept in a local
			bool basedefense = member->Team != NULL && member->Team->Class->IsBaseDefense; // RA2: else-if chains and own counters as the executable lays them out
			if (Scen->RandomNumber(0, 99) >= 66 || basedefense) { // RA2: a plain random draw, the base defense test is an alternative
				member->QueueMission(MISSION_GUARD_AREA, false); // RA2: queued, not assigned
				member->ArchiveTarget = this; // RA2: else-if chains and own counters as the executable lays them out
			} else {
				member->QueueMission(MISSION_RESCUE, false); // RA2: as above
				member->ArchiveTarget = this; // RA2: else-if chains and own counters as the executable lays them out
			}
			member->Assign_Target((AbstractClass *)enemy); // RA2: else-if chains and own counters as the executable lays them out
			risktotal += member->Risk(); // RA2: else-if chains and own counters as the executable lays them out
			if (risktotal > desired) {
				break;
			}
		}
	}

	if (risktotal > desired) { // RA2: as above
		if (enemy != NULL) { // RA2: the foot test is repeated
			is_foot = (enemy->AbstractFlags & ABSTRACT_FOOT); // RA2: as above
		}
		if (enemy != NULL && is_foot) { // RA2: as above
			((FootClass *)enemy)->BaseAttackTimer = (int)(Rule->BaseDefenseDelay * 900.0); // RA2: minutes of 900 ticks
		}
	}
}




/***********************************************************************************************
 * TechnoClass::Is_Allowed_To_Retaliate -- Checks object to see if it can retaliate.           *
 *                                                                                             *
 *    This routine is called when this object has suffered some damage and it needs to know    *
 *    if it should fight back. The object that caused the damage is specifed as a parameter.   *
 *                                                                                             *
 * INPUT:   source   -- The points to the object that was the source of the damage applied     *
 *                      to this object.                                                        *
 *                                                                                             *
 * OUTPUT:  bool; Should retaliation occur?                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/19/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoClass::Is_Allowed_To_Retaliate(TechnoClass const * source, WarheadTypeClass const * warhead) const
{
	/*
	**	If there is no source of the damage, then retaliation cannot occur.
	*/
	if (source == NULL) return(false); // RA2: asked first, the veinhole case is gone (not in RA2's layout)

	if (!Techno_Type_Class()->CanRetaliate) return(false); // RA2: the type may forbid it
	if (CaptureManager != NULL && ((CaptureManagerClass *)CaptureManager)->Unit != NULL) return(false); // RA2: a mind controller never retaliates
	if (SpawnManager != NULL) return(false); // RA2: nor does a spawner

	if (House->Is_Human_Player() && TarCom != NULL) return(false);

	/*
	**	If the mission precludes retaliation, then don't retaliate.
	*/
	if (!Current_Mission_Control().IsRetaliate) return(false);

	/*
	**	If the source of the damage is an ally, then retaliation shouldn't
	**	occur either.
	*/
	if (House->Is_Ally(source)) return(false);
	if (source->IsDisguisedAs(House)) return(false); // RA2: a disguised source is not recognised

	/*
	**	Only objects that have a damaging weapon are allowed to retaliate.
	*/
	if (Combat_Damage(-1) <= 0 || !Is_Weapon_Equipped()) return(false); // RA2: the damage is asked for with -1

	/*
	**	If this is not equipped with a weapon that can attack the molester, then
	**	don't allow retaliation.
	*/
	TechnoTypeClass const * ttype = Techno_Type_Class(); // RA2: as above
	int which = What_Weapon_Should_I_Use((AbstractClass *)source);
	FireErrorType result = (FireErrorType)GetFireErrorWithoutRange((AbstractClass *)source, which); // RA2: the fire error says whether the weapon can attack it
	if (result == FIRE_ILLEGAL || result == FIRE_CANT) return(false); // RA2: as above

	/*
	**	Tanya is not allowed to retaliate against buildings in the normal sense while in guard mode. That
	**	is, unless it is owned by the computer. Normally, Tanya can't do anything substantial to a building
	**	except to blow it up.
	*/
	if (House->Is_Human_Player() && source->Fetch_RTTI() == RTTI_BUILDING) { // RA2: as above
		if (Fetch_RTTI() == RTTI_INFANTRY && ((InfantryTypeClass const *)ttype)->IsBomber) { // RA2: as above
			return(false);
		}
		if (Has_Ability(ABILITY_C4)) { // RA2: as above
			return(false);
		}
	}

	if (House->Is_Human_Player() && Fetch_RTTI() == RTTI_UNIT) { // RA2: as above
		BuildingTypeClass const * deploys_into = ((UnitClass const *)this)->Class->DeploysInto;
		if (deploys_into != NULL && deploys_into->IsArtillary) {
			return(false);
		}
	}

	/*
	**	If a human house is not allowed to retaliate automatically, then don't
	*/
	if (House->Is_Human_Player() && !Rule->IsSmartDefense && Fetch_RTTI() != RTTI_BUILDING // RA2: as above
		&& CurrentMission != MISSION_GUARD_AREA && CurrentMission != MISSION_GUARD && CurrentMission != MISSION_PATROL) { // RA2: as above
		return(false);
	}

	/*
	**	If this object is part of a team that prevents retaliation then don't allow retaliation.
	*/
	bool foot; // RA2: the foot flag is tested by byte, with a local
	if (this != NULL) { // RA2: as above
		foot = (AbstractFlags & ABSTRACT_FOOT); // RA2: as above
	}
	if (this != NULL && foot && ((FootClass const *)this)->Team != NULL && ((FootClass const *)this)->Team->Class->IsSuicide) { // RA2: as above
		return(false);
	}

	/*
	**	Compare potential threat of the current target and the potential new target. Don't retaliate
	**	if it is currently attacking the greater threat.
	*/
	if (!House->Is_Human_Player()) { // RA2: as above
		AbstractClass * target = TarCom; // RA2: as above
		bool techno; // RA2: as above
		if (target != NULL) { // RA2: as above
			techno = (target->AbstractFlags & ABSTRACT_OBJECT); // RA2: as above
		}
		if (target != NULL && techno && Target_Threat((TechnoClass *)target, COORD_NONE) > Target_Threat((TechnoClass *)source, COORD_NONE)) { // RA2: as above
			return(false);
		}
	}

	/*
	**	A parasite that is eating this object is never fought.
	*/
	bool foot2; // RA2: as above
	if (this != NULL) { // RA2: as above
		foot2 = (AbstractFlags & ABSTRACT_FOOT); // RA2: as above
	}
	if (this != NULL && foot2 && source == ((FootClass const *)this)->ParasiteEatingMe) { // RA2: as above
		return(false);
	}

	/*
	**	All checks passed, so return that retaliation is allowed.
	*/
	return(true);
}






/***********************************************************************************************
 * TechnoClass::Get_Ownable -- Fetches the ownable bits for this object.                       *
 *                                                                                             *
 *    This routine will return the ownable bits for this object. The ownable bits represent    *
 *    the houses that are allowed to own this object.                                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the ownable bits for this object.                                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int TechnoClass::Get_Ownable(void) const
{
	return(TClass->Get_Ownable());
}


/***********************************************************************************************
 * TechnoClass::Risk -- Fetches the risk associated with this object.                          *
 *                                                                                             *
 *    This routine is called when the risk value for this object needs to be determined.       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the risk value for this object.                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int TechnoClass::Risk(void) const
{
	TechnoTypeClass const *ttptr = TClass;
	if (ttptr != NULL) {
		if (Fetch_RTTI() == RTTI_BUILDING && ((BuildingClass *)this)->GetOccupantCount() > 0) { // RA2: a garrisoned building, by its occupants
			int value = Rule->ThreatPerOccupant; // RA2: as above
			return(value * ((BuildingClass *)this)->GetOccupantCount()); // RA2: as above
		}
		return(ttptr->Risk);
	}

	return(0);
}



/***********************************************************************************************
 * TechnoClass::Tiberium_Load -- Fetches the current tiberium load percentage.                 *
 *                                                                                             *
 *    This routine will return the current Tiberium load (expressed as a fixed point fraction) *
 *    that this object currently contains. Typical implementor of this function would be       *
 *    the harvester. Any object that can return a non-zero value should derive from this       *
 *    function in order to return the appropriate value.                                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the current Tiberium load expressed as a fixed point number.          *
 *          0x0000   = empty                                                                   *
 *          0x0080   = half full                                                               *
 *          0x0100   = full                                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
double TechnoClass::Tiberium_Load(void) const
{
	return(0.0);
}


/***********************************************************************************************
 * TechnoClass::Desired_Load_Dir -- Fetches loading parameters for this object.                *
 *                                                                                             *
 *    This routine is called when an object desires to load up on this object. The object      *
 *    desiring to load is specified. The cell that the loading object should move to is        *
 *    determined. The direction that this object should face is also calculated. This routine  *
 *    will be overridden by those objects that can actually load up passengers.                *
 *                                                                                             *
 * INPUT:   object   -- The object that is desiring to load up.                                *
 *                                                                                             *
 *          moveto   -- Reference to the cell that the loading object should move to before    *
 *                      the final load process occurs (this value will be filled in).          *
 *                                                                                             *
 * OUTPUT:  Returns with the direction that the transport object should face.                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
FacingType TechnoClass::Desired_Load_Dir(ObjectClass * , Cell & moveto) const
{
	moveto = CELL_NONE;
	return(FACING_N);
}


/***********************************************************************************************
 * TechnoClass::Pip_Count -- Fetches the number of pips to display on this object.             *
 *                                                                                             *
 *    This routine will return the number of pips to display on this object when the object    *
 *    is selected. The default condition is to return no pips at all. This routine is          *
 *    derived for those objects that can have pips.                                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of pips to display on this object when selected.           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int TechnoClass::Pip_Count(void) const
{
	bool valid = false;
	double maximum, current;
	switch (TClass->PipScale) {

		case PIPSCALE_PASSENGERS:
			current = Cargo.How_Many();
			maximum = TClass->Max_Passengers();
			valid = true;
			break;

		case PIPSCALE_AMMO:
			current = Ammo;
			maximum = TClass->MaxAmmo;
			valid = true;
			break;

		default:
			return(0);
	}

	if (valid) {
		int retval = TClass->Max_Pips() * (current / maximum) + 0.5;
		if (!retval && current > 0) retval = 1;
		return(retval);
	}

	return(0);
}


/***********************************************************************************************
 * TechnoClass::Fire_Direction -- Fetches the direction projectile fire will take.             *
 *                                                                                             *
 *    This routine will fetch the direction that a fired projectile will take. This is         *
 *    usually the facing of the object's weapon. This routine will be derived for the objects  *
 *    that have their weapon barrel facing a different direction than the body.                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the direction a fired projectile will take.                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
DirType TechnoClass::Fire_Direction(void) const
{
	return(Turret_Facing());
}











/***********************************************************************************************
 * TechnoClass::Response_Select -- Handles the voice response when selected.                   *
 *                                                                                             *
 *    This routine is called when a voice response to a select action is desired. This routine *
 *    should be overridden for any object that actually has a voice response.                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine can generate an audio response.                                    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void TechnoClass::Response_Select(void)
{
	if (AllowVoice) { // RA2: as above
		if (TClass->VoiceSelect.Count()) { // RA2: as above
			TypeList<int> const & sounds = TClass->VoiceSelect; // RA2: a random pick of the list
			unsigned random = NonCriticalRandomNumber(); // RA2: as above
			VocClass::PlayGlobal(sounds[random % sounds.Count()], 1.0f, 0x2000, 0); // RA2: as above
		}
	}
}



/***********************************************************************************************
 * TechnoClass::Response_Move -- Handles the voice response to a movement request.             *
 *                                                                                             *
 *    This routine is called when a voice response to a movement order is desired. This        *
 *    routine should be overridden for any object that actually has a voice response.          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This can generate an audio response.                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void TechnoClass::Response_Move(void)
{
	if (AllowVoice) { // RA2: as above
		if (TClass->VoiceMove.Count()) { // RA2: as above
			TypeList<int> const & sounds = TClass->VoiceMove; // RA2: a random pick of the list
			unsigned random = NonCriticalRandomNumber(); // RA2: as above
			VocClass::PlayGlobal(sounds[random % sounds.Count()], 1.0f, 0x2000, 0); // RA2: as above
		}
	}
}









/***********************************************************************************************
 * TechnoClass::Response_Attack -- Handles the voice response when given attack order.         *
 *                                                                                             *
 *    This routine is called when a voice response to an attack order is desired. This routine *
 *    should be overridden for any object that actually have a voice response.                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This can generate an audio response.                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void TechnoClass::Response_Attack(void)
{
	if (AllowVoice) { // RA2: as above
		if (TClass->VoiceAttack.Count()) { // RA2: as above
			TypeList<int> const & sounds = TClass->VoiceAttack; // RA2: a random pick of the list
			unsigned random = NonCriticalRandomNumber(); // RA2: as above
			VocClass::PlayGlobal(sounds[random % sounds.Count()], 1.0f, 0x2000, 0); // RA2: as above
		}
	}
}



/// <summary>
/// Whether this object may look for an opportunity target now: the scan delay has passed, it is not
/// warping something out, its type acquires targets on its own, an occupiable building has occupants,
/// and it is armed without a mind-controlled unit or (as a human player's renovator) other duties.
/// </summary>
bool TechnoClass::Can_Scan_For_Target(void) // RA2: not in TibSun, at 0x6D40C0
{
	if ((unsigned)(Frame - LastTargetScanFrame) >= (unsigned)TargetScanDelay) {
		if (!IsWarpingSomethingOut()) {
			if (Techno_Type_Class()->CanPassiveAquire) {
				if (Fetch_RTTI() != RTTI_BUILDING || !((BuildingClass *)this)->Class->CanBeOccupied || ((BuildingClass *)this)->GetOccupantCount() != 0) {
					bool busy = CaptureManager != NULL && CaptureManager->Unit != NULL;
					if ((!Is_Renovator() || !House->Is_Human_Player()) && !busy && Is_Weapon_Equipped()) {
						return(true);
					}
				}
			}
		}
	}
	return(false);
}


/// <summary>
/// Whether this object should look for an opportunity target now: always for an aggressive team member
/// moving without a target, otherwise when it may scan, is not a hovering balloon still on its way, and
/// fires on opportunity or guards without an area-fire weapon already chosen for its target.
/// </summary>
bool TechnoClass::Should_Scan_For_Target(void) // RA2: not in TibSun, at 0x6D4180
{
	if ((unsigned)(Frame - LastTargetScanFrame) < (unsigned)TargetScanDelay) {
		return(false);
	}
	bool foot;
	bool foot2;
	if (TarCom == NULL && !House->Is_Human_Player() && this != NULL) {
		if (this != NULL) foot = (AbstractFlags & ABSTRACT_FOOT) != 0;
		if (foot && ((FootClass *)this)->Team != NULL) {
			TeamTypeClass const * team_type = ((FootClass *)this)->Team->Class;
			if (!team_type->IsSuicide && team_type->IsAggressive && CurrentMission == MISSION_MOVE) {
				return(true);
			}
		}
	}
	if (!Can_Scan_For_Target()) {
		return(false);
	}
	if (CurrentMission == MISSION_MOVE && Techno_Type_Class()->BalloonHover) {
		foot2 = false;
		if (this != NULL) foot2 = (AbstractFlags & ABSTRACT_FOOT) != 0;
		if (!foot2) {
			return(false);
		}
		if (((FootClass *)this)->NavCom == Get_Cell_Ptr()) {
			if (PlanningToken == NULL) {
				return(true);
			}
			if (PlanningToken->Node_Count() <= 0) {
				return(true);
			}
		}
	}
	if (!Techno_Type_Class()->OpportunityFire) {
		if (CurrentMission != MISSION_GUARD) {
			return(false);
		}
		int which = IsNotSprayAttack();
		WeaponDataStruct const * wdata = Get_Class_Weapon_Data(which);
		bool flagged;
		if (wdata == NULL || wdata->Weapon == NULL || !wdata->Weapon->AreaFire) {
			flagged = false;
		} else {
			flagged = true;
		}
		int best = What_Weapon_Should_I_Use(TarCom);
		bool same = best == which;
		if (flagged && same) {
			return(false);
		}
	}
	return(true);
}


/// <summary>
/// Whether objects of this kind are not animation types, buildings or bullets.
/// </summary>
int __stdcall Is_Not_AnimType_Building_Bullet(RTTIType rtti) // RA2: not in TibSun, at 0x6D42F0
{
	if (rtti != RTTI_ANIMTYPE && rtti != RTTI_BUILDING && rtti != RTTI_BULLET) {
		return(1);
	}
	return(0);
}


/// <summary>
/// Looks for an opportunity target when the object should; a new target makes it drop the old one.
/// </summary>
bool TechnoClass::Scan_For_Target(void) // RA2: not in TibSun, at 0x6D4310
{
	bool result = false;
	if (Should_Scan_For_Target()) {
		LastTargetScanFrame = Frame;
		AbstractClass * target = TarCom;
		Coord coord = PositionCoord;
		result = Target_Something_Nearby(coord, THREAT_RANGE);
		if (result && TarCom != target) {
			ShouldLoseTargetNow = true;
		}
	}
	return(result);
}


/// <summary>
/// Whether the object is moving, guarding or harvesting.
/// </summary>
int TechnoClass::Is_Moving_Guarding_Or_Harvesting(void) const // RA2: not in TibSun, at 0x6D4390
{
	MissionType mission = CurrentMission;
	if (mission != MISSION_MOVE && mission != MISSION_HARVEST && mission != MISSION_GUARD) {
		return(0);
	}
	return(1);
}


/// <summary>
/// Whether the object is on a mission that keeps it from looking for targets.
/// </summary>
int TechnoClass::Is_Passive_Mission(void) const // RA2: not in TibSun, at 0x6D43B0
{
	MissionType mission = CurrentMission;
	if (mission != MISSION_SLEEP && mission != MISSION_ENTER && mission != MISSION_STOP && mission != MISSION_AMBUSH
		&& mission != MISSION_UNLOAD && mission != MISSION_CONSTRUCTION && mission != MISSION_DECONSTRUCTION
		&& mission != MISSION_REPAIR && mission != MISSION_MISSILE && mission != MISSION_HARMLESS && mission != MISSION_WAIT
		&& mission != MISSION_OPEN) {
		return(0);
	}
	return(1);
}


/// <summary>
/// RA2's DistributedFire: spreads the object's fire over its current targets, the most threatening one
/// not yet fired on first, and all of them again once each was fired on.
/// </summary>
void TechnoClass::Distribute_Fire(void) // RA2: not in TibSun, at 0x6D4400
{
	if (CurrentTargets.Count() == 0) {
		AttackedTargets.Clear();
		return;
	}
	for (int index = AttackedTargets.Count() - 1; index >= 0; index--) {
		AbstractClass * target = AttackedTargets[index];
		bool found = false;
		for (int k = 0; k < CurrentTargets.Count(); k++) {
			if (target == CurrentTargets[k]) {
				found = true;
				break;
			}
		}
		if (!found) {
			AttackedTargets.Delete_Index(index);
		}
	}
	DynamicVectorClass<AbstractClass *> targets;
	DynamicVectorClass<int> values;
	int fired = AttackedTargets.Count();
	if (fired > 0) {
		int count = CurrentTargets.Count();
		for (int i = 0; i < count; i++) {
			AbstractClass * target = CurrentTargets[i];
			bool found = false;
			for (int k = 0; k < fired; k++) {
				if (target == AttackedTargets[k]) {
					found = true;
					break;
				}
			}
			if (!found) {
				targets.Add(target);
				values.Add(CurrentTargetThreatValues[i]);
			}
		}
	}
	if (targets.Count() == 0) {
		AttackedTargets.Clear();
		int best = 0;
		AbstractClass * best_target = NULL;
		for (int i = 0; i < CurrentTargets.Count(); i++) {
			if (CurrentTargetThreatValues[i] > best) {
				best = CurrentTargetThreatValues[i];
				best_target = CurrentTargets[i];
			}
		}
		Assign_Target(best_target);
	} else {
		int best = 0;
		AbstractClass * best_target = NULL;
		for (int i = 0; i < targets.Count(); i++) {
			AbstractClass * target = targets[i];
			if (values[i] > best) {
				best_target = target;
				best = values[i];
			}
		}
		Assign_Target(best_target);
	}
}


/// <summary>
/// Whether the mission is neither guard nor sticky.
/// </summary>
int TechnoClass::Is_Not_Guard_Or_Sticky(MissionType mission) // RA2: not in TibSun, at 0x6D46B0
{
	if (mission != MISSION_GUARD && mission != MISSION_STICKY) {
		return(1);
	}
	return(0);
}
























/***********************************************************************************************
 * TechnoClass::Target_Something_Nearby -- Handles finding and assigning a nearby target.      *
 *                                                                                             *
 *    This routine will search for a nearby target and assign it to this object's TarCom.      *
 *    The method to use when scanning for a target is controlled by the parameter passed.      *
 *                                                                                             *
 * INPUT:   threat   -- The threat control parameter used to control the range searched. The   *
 *                      only values recognized are THREAT_RANGE and THREAT_AREA.               *
 *                                                                                             *
 * OUTPUT:  Was a suitable target acquired and assigned to the TarCom?                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoClass::Target_Something_Nearby(Coord const & coord, ThreatType threat)
{
	LastTargetScanFrame = Frame; // RA2: the frame of this scan is noted
	if (CurrentMission == MISSION_GUARD_AREA) { // RA2: the next scan is delayed by the rules, guarding being quicker
		int random = Random_Pick(0, 2); // RA2: as above
		TargetingTimer = Rule->GuardAreaTargetingDelay + random; // RA2: as above
	} else {
		int random = Random_Pick(0, 2); // RA2: as above
		TargetingTimer = Rule->NormalTargetingDelay + random; // RA2: as above
	}
	threat = ThreatType(threat & (THREAT_RANGE|THREAT_AREA));

	/*
	**	Determine that if there is an existing target it is still legal
	**	and within range.
	*/
	if (TarCom != NULL && ShouldLoseTargetNow) { // RA2: a target is dropped when the weapon cannot fire at it any more
		int weapon = What_Weapon_Should_I_Use(TarCom); // RA2: as above
		FireErrorType result = Can_Fire(TarCom, weapon, true); // RA2: as above
		if (result == FIRE_CANT && SpawnManager != NULL) { // RA2: a spawner only forgets its target
			SpawnManager->ResetTarget(); // RA2: as above
		} else if (result == FIRE_CANT || result == FIRE_ILLEGAL || result == FIRE_RANGE) { // RA2: as above
			Assign_Target(NULL);
		}
	}

	/*
	**	If there is no target, then try to find one and assign it as
	**	the target for this unit.
	*/
	if (TarCom == NULL) {
		AbstractClass * target = Greatest_Threat(threat, coord, false); // RA2: the threat is found first, then assigned
		if (Techno_Type_Class()->DistributedFire) { // RA2: spread over the current targets
			Distribute_Fire(); // RA2: as above
		} else {
			if (target != NULL) { // RA2: as above
				Assign_Target(target); // RA2: as above
			}
			int weapon = What_Weapon_Should_I_Use(TarCom); // RA2: the damage the shot is expected to do is booked on the target
			TechnoClass * techno = (TechnoClass *)target; // RA2: as above
			if (techno != NULL) { // RA2: as above
				techno = (techno->AbstractFlags & ABSTRACT_TECHNO) ? techno : NULL; // RA2: as above
			}
			WeaponTypeClass const * wtype = Get_Class_Weapon_Data(weapon)->Weapon; // RA2: as above
			if (techno != NULL && wtype != NULL && !wtype->Bullet->IsInaccurate) { // RA2: as above
				techno->EstimatedHealth -= Expected_Damage(techno, wtype); // RA2: as above
			}
		}
	}

	/*
	**	Return with answer to question: Does this unit now have a target?
	*/
	return(TarCom != NULL);
}



/***********************************************************************************************
 * TechnoClass::Exit_Object -- Causes specified object to leave this object.                   *
 *                                                                                             *
 *    This routine is called when there is an attached object that should detach and leave     *
 *    this object. Typical of this would be the refinery and APC.                              *
 *                                                                                             *
 * INPUT:   object   -- The object that is trying to leave this object.                        *
 *                                                                                             *
 * OUTPUT:  Was the object successfully launched from this object? Failure might indicate that *
 *          there is insufficient room to detach the specified object.                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/24/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int TechnoClass::Exit_Object(TechnoClass *)
{
	return(0);
}


/***********************************************************************************************
 * TechnoClass::Is_Ready_To_Random_Animate -- Determines if the object should random animate.  *
 *                                                                                             *
 *    This will examine this object to determine if it is time and ready to perform some       *
 *    kind of random animation.                                                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Is it time to perform an random animation?                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/19/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoClass::Is_Ready_To_Random_Animate(void) const
{
	return(IdleTimer == 0);
}





/// <summary>
/// Does nothing at this level.
/// </summary>
void TechnoClass::Set_Follow_Target(AbstractClass *) // RA2: not in TibSun, at 0x6D48D0
{
}


/***********************************************************************************************
 * TechnoClass::Assign_Destination -- Assigns movement destination to the object.              *
 *                                                                                             *
 *    This routine is called when the object needs to have a new movement destination          *
 *    assigned. This routine must be overridden since at this level, movement is not allowed.  *
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
void TechnoClass::Assign_Destination(AbstractClass *, bool)
{
}


/***********************************************************************************************
 * TechnoClass::Enter_Idle_Mode -- Object enters its default idle condition.                   *
 *                                                                                             *
 *    This routine is called when the object should intelligently revert to an idle state.     *
 *    Typically this routine is called after some mission has completed. This routine must     *
 *    be overridden by the various object types. It is located at this level merely to provide *
 *    a virtual function entry point.                                                          *
 *                                                                                             *
 * INPUT:   initial  -- Is this called when the unit just leaves a factory or is initially     *
 *                      or is initially placed on the map?                                     *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/24/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoClass::Enter_Idle_Mode(bool, bool)
{
	if (!RefreshMegaMission() && IsNotWarpingIn()) { // RA2: an idle unit takes its next planning token node
		TryNextPlanningTokenNode(); // RA2: as above
	}
	return(false);
}



/***********************************************************************************************
 * TechnoClass::Draw_Pips -- Draws the transport pips and other techno graphics.               *
 *                                                                                             *
 *    This routine is used to render the small transportation pip (occupant feedback graphic)  *
 *    used for transporter object. It will also display if the techno object is "primary"      *
 *    if necessary.                                                                            *
 *                                                                                             *
 * INPUT:   x,y   -- The pixel coordinate for the center of the first pip. Subsequent pips     *
 *                   are drawn rightward.                                                      *
 *                                                                                             *
 *          window-- The window that pip clipping is relative to.                              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/08/1995 JLB : Created.                                                                 *
 *   10/06/1995 JLB : Displays the team group number.                                          *
 *   09/10/1996 JLB : Medic hack for red pip.                                                  *
 *=============================================================================================*/
void TechnoClass::Draw_Pips(Point2D const & bottomleft, Point2D const & center, Rect const & rect) const
{
	Point2D offset(4, 2);
	Point2D xy = bottomleft + Point2D(6, -1);

	Class_Of(); // RA2: the type is fetched for each table
	ShapeSet const * pip_shapes = (ShapeSet const *)ObjectTypeClass::PipShapes; // RA2: a static table, the type call stays
	Class_Of(); // RA2: as above
	ShapeSet const * pips2 = (ShapeSet const *)ObjectTypeClass::Pip2Shapes; // RA2: as above

	bool building = Fetch_RTTI() == RTTI_BUILDING; // RA2: the virtual RTTI, kept in a local
	if (!building) { // RA2: the pip tables and tests as the executable reads them
		xy = bottomleft + Point2D(-5, 0);
		offset = Point2D(4, 0);
		pip_shapes = pips2;
	}

	if (Fetch_RTTI() == RTTI_BUILDING && Techno_Type_Class()->PipScale == PIPSCALE_TIBERIUM) { // RA2: a building with this scale draws no pips here
		return; // RA2: as above
	}

	int spawns = Techno_Type_Class()->SpawnsNumber; // RA2: carrier spawns show docked or away
	if (spawns > 0) { // RA2: as above
		int docked = SpawnManager->CountDockedSpawns(); // RA2: as above
		for (int index = 0; index < spawns; index++) { // RA2: as above
			int full = 1; // RA2: as above
			int empty = 0; // RA2: as above
			Draw_Shape(*LogicalSurface, *NormalDrawer, pip_shapes, index < docked ? full : empty, xy + offset * index, rect, ShapeFlags_Type(SHAPE_CENTER|SHAPE_WIN_REL), NULL, 0, ZGRAD_GROUND, 1000, NULL, 0, Point2D(0, 0)); // RA2: the last arguments are passed explicitly
		}
	}

	if (building) { // RA2: a garrisoned building shows its occupants
		BuildingClass const * bldg = (BuildingClass const *)this; // RA2: as above
		int occupants = bldg->Class->MaxNumberOccupants; // RA2: as above
		int count = bldg->GetOccupantCount(); // RA2: as above
		if (bldg->Class->ShowOccupantPips && occupants > 0 && count > 0) { // RA2: as above
			for (int index = 0; index < occupants; index++) { // RA2: as above
				int full = 7; // RA2: as above
				int empty = 6; // RA2: as above
				Draw_Shape(*LogicalSurface, *NormalDrawer, pip_shapes, index < count ? full : empty, xy + offset * index, rect, ShapeFlags_Type(SHAPE_CENTER|SHAPE_WIN_REL), NULL, 0, ZGRAD_GROUND, 1000, NULL, 0, Point2D(0, 0)); // RA2: as above
			}
		}
	}

	/*
	**	Transporter type objects have a different graphic representation for the pips. The
	**	pip color represents the type of occupant.
	*/
	if (Techno_Type_Class()->MaxPassengers > 0) { // RA2: the pip tables and tests as the executable reads them
		Point2D shift(0, 0); // RA2: a gunner shifts the other pips
		int pips = Class_Of()->Max_Pips(); // RA2: the pips are colored by an array of occupant sizes
		Techno_Type_Class(); // RA2: as above
		int * colors = new int[pips]; // RA2: as above
		for (int c = 0; c < pips; c++) { // RA2: as above
			colors[c] = 0; // RA2: as above
		}
		int passengers = Cargo.How_Many(); // RA2: as above
		int slot = Cargo.GetTotalSize() - 1; // RA2: as above
		if (slot > pips) { // RA2: as above
			slot = pips; // RA2: as above
		}
		ObjectClass * object = Cargo.Attached_Object(); // RA2: as above
		for (int i = 0; i < passengers && object != NULL; i++) { // RA2: as above
			if (object->Fetch_RTTI() == RTTI_INFANTRY) { // RA2: as above
				colors[slot] = ((InfantryClass *)object)->Class->Pip; // RA2: as above
			} else if (object->Fetch_RTTI() == RTTI_UNIT) { // RA2: as above
				int size = (int)(((TechnoClass *)object)->Techno_Type_Class()->Size - 1.0); // RA2: as above
				for (int extra = size; extra > 0; extra--) { // RA2: as above
					colors[slot--] = 4; // RA2: as above
				}
				colors[slot] = 5; // RA2: as above
			} else {
				colors[slot] = 1; // RA2: as above
			}
			object = object->Next; // RA2: as above
			slot--; // RA2: as above
		}
		int start = 0; // RA2: as above
		if (Techno_Type_Class()->Gunner) { // RA2: as above
			Draw_Shape(*LogicalSurface, *NormalDrawer, pip_shapes, colors[0], xy, rect, ShapeFlags_Type(SHAPE_CENTER|SHAPE_WIN_REL), NULL, 0, ZGRAD_GROUND, 1000, NULL, 0, Point2D(0, 0)); // RA2: as above
			start = 1; // RA2: as above
			shift = offset * 2; // RA2: as above
		}
		for (int j = start; j < pips; j++) { // RA2: as above
			Draw_Shape(*LogicalSurface, *NormalDrawer, pip_shapes, colors[j], xy + shift + offset * j, rect, ShapeFlags_Type(SHAPE_CENTER|SHAPE_WIN_REL), NULL, 0, ZGRAD_GROUND, 1000, NULL, 0, Point2D(0, 0)); // RA2: as above
		}
		delete colors; // RA2: as above
	} else {

		/*
		**	Display number of how many attached objects there are. This is also used
		**	to display the fullness rating for a harvester.
		*/
		int pips = Pip_Count();

		/*
		**	Check if it's a harvester, to show the right type of pips for the
		**	various minerals it could have harvested.
		*/
		if (Fetch_RTTI() == RTTI_UNIT && Techno_Type_Class()->PipScale == PIPSCALE_TIBERIUM) { // RA2: the harvester scale
			float gold = Storage.Get_Amount(0); // RA2: the four tiberium slots are read separately
			float gems = Storage.Get_Amount(2); // RA2: as above
			float ore = Storage.Get_Amount(3); // RA2: as above
			float other = Storage.Get_Amount(1); // RA2: as above
			int greenpips = (int)(Techno_Type_Class()->Max_Pips() * ((double)(ore + gems + gold) / Techno_Type_Class()->Capacity) + 0.5); // RA2: as above
			int bluepips = (int)(Techno_Type_Class()->Max_Pips() * ((double)other / Techno_Type_Class()->Capacity) + 0.5); // RA2: as above

			for (int index = 0; index < Class_Of()->Max_Pips(); index++) {
				int shape = 0; // RA2: the pip tables and tests as the executable reads them
				if (bluepips > 0) {
					shape = 5; // RA2: the pip tables and tests as the executable reads them
					bluepips--;
				} else if (greenpips > 0) {
					shape = 1; // RA2: the pip tables and tests as the executable reads them
					greenpips--;
				}
				Draw_Shape(*LogicalSurface, *NormalDrawer, pip_shapes, shape, xy + offset * index, rect, ShapeFlags_Type(SHAPE_CENTER|SHAPE_WIN_REL), NULL, 0, ZGRAD_GROUND, 1000, NULL, 0, Point2D(0, 0)); // RA2: as above
			}
		} else if (Techno_Type_Class()->PipScale == PIPSCALE_AMMO) { // RA2: the ammo scale, with optional wrapping rows
			int wrap = Techno_Type_Class()->PipWrap; // RA2: as above
			if (wrap == 0) { // RA2: as above
				int left = pips; // RA2: as above
				for (int index = 0; index < Class_Of()->Max_Pips(); index++) { // RA2: as above
					if (left > 0) { // RA2: as above
						left--; // RA2: as above
						Draw_Shape(*LogicalSurface, *NormalDrawer, pips2, 12, xy + offset * index - Point2D(0, 3), rect, ShapeFlags_Type(SHAPE_CENTER|SHAPE_WIN_REL), NULL, 0, ZGRAD_GROUND, 1000, NULL, 0, Point2D(0, 0)); // RA2: as above
					}
				}
			} else {
				int rows = Class_Of()->Max_Pips() / wrap; // RA2: as above
				for (int index = 0; index < wrap; index++) { // RA2: as above
					int frame = 13; // RA2: as above
					for (int row = rows - 1; row >= 0; row--) { // RA2: as above
						if (index + row * wrap < Ammo) { // RA2: as above
							frame = row + 14; // RA2: as above
							break; // RA2: as above
						}
					}
					Draw_Shape(*LogicalSurface, *NormalDrawer, pips2, frame, xy + offset * index - Point2D(0, 3), rect, ShapeFlags_Type(SHAPE_CENTER|SHAPE_WIN_REL), NULL, 0, ZGRAD_GROUND, 1000, NULL, 0, Point2D(0, 0)); // RA2: as above
				}
			}
		}
	}

	/*
	**	Display whether this unit is a leader unit or not.
	*/
	if (Fetch_RTTI() != RTTI_BUILDING) { // RA2: the pip tables and tests as the executable reads them
		Draw_Text_Overlay(bottomleft + Point2D(-10, 10), bottomleft, rect);
	}

	/*
	**	Display what group this unit belongs to. This corresponds to the team
	**	number assigned with the <CTRL> key.
	*/
	if (Group >= 0 && Group < 10) {
		int group = Group + 1; // RA2: the pip tables and tests as the executable reads them
		if (group == 10) group = 0;
		int yval = Fetch_RTTI() == RTTI_INFANTRY ? -36 : -33; // RA2: a fixed offset by type
		wchar_t text[10]; // RA2: the group number is wide text in a box
		swprintf(text, L"%d", group < 10 ? group : 0); // RA2: as above
		Point2D where = bottomleft + Point2D(-4, yval - 3); // RA2: as above
		Rect box = Text_Rect(text, where, 73, 2, -2); // RA2: as above
		box = Intersect(box, rect); // RA2: as above
		box.X--; // RA2: as above
		box.Width++; // RA2: as above
		RGBClass color = House->RemapColorRGB; // RA2: as above
		int pixel = DSurface::Build_Hicolor_Pixel(color.Get_Red(), color.Get_Green(), color.Get_Blue()); // RA2: as above
		LogicalSurface->Fill_Rect(box, 0); // RA2: as above
		LogicalSurface->Draw_Rect(box, pixel); // RA2: as above
		Plain_Text_Print(text, LogicalSurface, (Rect const *)&rect, &where, pixel, 0, TextPrintType(TPF_EFNT | TPF_FULLSHADOW), -1, 1); // RA2: as above
	}
}



/// <summary>
/// Draws the veteran, elite or dumbass pip at the object's corner.
/// </summary>
void TechnoClass::DrawVeterancyPips(Point2D const & point, Rect const & rect) const // RA2: not in TibSun, at 0x6D5440
{
	ObjectTypeClass const * type = Class_Of();
	void const * pips = ObjectTypeClass::PipShapes;
	int frame = -1;
	if (Veterancy.Is_Veteran()) {
		frame = 13;
	}
	if (Veterancy.Is_Elite()) {
		frame = 14;
	}
	if (Veterancy.Is_Dumbass()) {
		frame = 18;
	}
	if (frame != -1) {
		Point2D position = Point2D(point.X + 5, point.Y + 2);
		if (Fetch_RTTI() != RTTI_INFANTRY) {
			position.X += 5;
			position.Y += 4;
		}
		Draw_Shape(*LogicalSurface, *NormalDrawer, (ShapeSet const *)pips, frame, position, rect, (ShapeFlags_Type)(SHAPE_CENTER | SHAPE_WIN_REL | SHAPE_ALPHA), NULL, -2, ZGRAD_GROUND, 1000, NULL, 0, Point2D(0, 0));
	}
}





/// <summary>
/// Draws the text that is overlaid on top of the object.
/// This routine handles the power output and drain figures shown against a building that
/// generates power, and the "Primary" tag worn by a leader object. The tag is abbreviated
/// when the building is too narrow to carry the whole word.
/// </summary>
/// <param name="point2">The point the text is centered on.</param>
void TechnoClass::Draw_Text_Overlay(Point2D const & point1, Point2D const & point2, Rect const & cliprect) const
{
	if (Fetch_RTTI() == RTTI_BUILDING && ((BuildingClass const *)this)->Class->Power > 0) { // RA2: a power plant shows its output against the drain, in wide text
		if (!IsLeader) { // RA2: the primary label takes its place on a leader
			wchar_t text[128]; // RA2: as above
			swprintf(text, StringTable::LoadString("TXT_POWER_DRAIN2", NULL, "D:\\Ra2\\Techno.CPP", 13591), House->Power_Output(), House->Power_Drain()); // RA2: as above
			Rect background = Text_Rect(text, point2, 329, 4, 2); // RA2: the box the text needs
			background = Intersect(background, cliprect); // RA2: cut to the window
			RGBClass const & color = House->RemapColorRGB; // RA2: the box is drawn in the house color
			int pixel = DSurface::Build_Hicolor_Pixel(color.Get_Red(), color.Get_Green(), color.Get_Blue()); // RA2: as above
			LogicalSurface->Fill_Rect(background, 0); // RA2: as above
			LogicalSurface->Draw_Rect(background, pixel); // RA2: as above
			Simple_Text_Print(text, *LogicalSurface, cliprect, point2, pixel, 0, TextPrintType(TPF_EFNT | TPF_FULLSHADOW | TPF_CENTER), 1); // RA2: as above
		}
	}

	/*
	**	Display whether this unit is a leader unit or not.
	*/
	if (IsLeader) {
		wchar_t const * pritext = StringTable::LoadString("TXT_PRIMARY", NULL, "D:\\Ra2\\Techno.CPP", 13610); // RA2: wide text with a box
		if (Fetch_RTTI() == RTTI_BUILDING && ((BuildingClass const *)this)->Class->Width() == 1) { // RA2: as above
			pritext = StringTable::LoadString("TXT_PRI", NULL, "D:\\Ra2\\Techno.CPP", 13612); // RA2: as above
		}
		Rect background = Text_Rect(pritext, point2, 329, 4, 2); // RA2: as above
		background = Intersect(background, cliprect); // RA2: as above
		RGBClass const & color = House->RemapColorRGB; // RA2: as above
		int pixel = DSurface::Build_Hicolor_Pixel(color.Get_Red(), color.Get_Green(), color.Get_Blue()); // RA2: as above
		LogicalSurface->Fill_Rect(background, 0); // RA2: as above
		LogicalSurface->Draw_Rect(background, pixel); // RA2: as above
		Simple_Text_Print(pritext, *LogicalSurface, cliprect, point2, pixel, 0, TextPrintType(TPF_EFNT | TPF_FULLSHADOW | TPF_CENTER), 1); // RA2: as above
	}
}



/***********************************************************************************************
 * TechnoClass::Find_Exit_Cell -- Finds an appropriate exit cell for this object.              *
 *                                                                                             *
 *    This routine is called when an object would like to exit from this (presumed) transport. *
 *    A suitable cell should be returned by this routine. The specified object will probably   *
 *    be unloaded at that cell.                                                                *
 *                                                                                             *
 * INPUT:   techno   -- Pointer to the object that would like to unload. This is used to       *
 *                      determine suitability for placement.                                   *
 *                                                                                             *
 * OUTPUT:  Returns with the cell that is recommended for object exit.                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/12/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
Cell TechnoClass::Find_Exit_Cell(TechnoClass const * techno) const // RA2: as above
{
	return(Docking_Coord((TechnoClass *)techno).As_Cell()); // RA2: Docking_Coord takes the docking object
}



/***********************************************************************************************
 * TechnoClass::Refund_Amount -- Returns with the money to refund if this object is sold.      *
 *                                                                                             *
 *    This routine is used by the selling back mechanism in order to credit the owning house   *
 *    with some refund credits. The value returned is the credits to refund to the owner.      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the credits to refund to the owner.                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/13/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int TechnoClass::Refund_Amount(void) const
{
	return(Techno_Type_Class()->GetRefund(House, false)); // RA2: Refund_Amount at 0x6D5850 matches only so
} // RA2: Refund_Amount at 0x6D5850 has no return(cost);


/***********************************************************************************************
 * TechnoClass::Anti_Air -- Determines the anti-aircraft strength of the object.               *
 *                                                                                             *
 *    This routine will calculate and return the anti-aircraft strength of this object.        *
 *    Typical users of this strength value is the base defense expert system AI.               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the anti-aircraft defense value of this object. The value returned    *
 *          is an abstract number to be used for relative comparisons only.                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/02/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int TechnoClass::Anti_Air(void) const
{
	if (Is_Weapon_Equipped()) {
		WeaponTypeClass const * weapon = GetTurretWeapon()->Weapon; // RA2: the turret slot's weapon

		if (weapon->Bullet->IsAntiAircraft) { // RA2: as above
			double damage = weapon->Attack * weapon->WarheadPtr->Modifier[ARMOR_LIGHT]; // RA2: in floating point
			int value = (int)(damage * weapon->Range / weapon->ROF); // RA2: as above

			if (TClass->Is_Two_Shooter()) {
				value += value; // RA2: as above
			}
			return(value/50);
		}
	}
	return(0);
}



/***********************************************************************************************
 * TechnoClass::Anti_Armor -- Determines the anti-armor strength of the object.                *
 *                                                                                             *
 *    This routine is used to examine and calculate the anti-armor strength of this object.    *
 *    Typical user user of this would be the expert system base defense AI.                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the relative anti-armor combat value for this object. The value       *
 *          is abstract and is only to be used in relative comparisons.                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/02/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int TechnoClass::Anti_Armor(void) const
{
	if (Is_Weapon_Equipped() && GetTurretWeapon()->Weapon->Bullet->IsAntiGround) { // RA2: the turret slot's weapon
		WeaponTypeClass const * weapon = GetTurretWeapon()->Weapon; // RA2: as above
		int range = weapon->Range; // RA2: as above
		BulletTypeClass const * bullet = weapon->Bullet;
		WarheadTypeClass const * warhead = weapon->WarheadPtr;
		if (range >= 4 * CELL_LEPTON) { // RA2: as above
			range = 4 * CELL_LEPTON; // RA2: as above
		}
		double damage = weapon->Attack * warhead->Modifier[ARMOR_MEDIUM]; // RA2: in floating point, no spread factor
		int value = (int)(damage * range / weapon->ROF); // RA2: as above
		if (TClass->Is_Two_Shooter()) {
			value += value; // RA2: as above
		}
		if (bullet->IsInaccurate) {
			value /= 2;
		}
		return(value/50);
	}
	return(0);
}



/***********************************************************************************************
 * TechnoClass::Anti_Infantry -- Calculates the anti-infantry strength of this object.         *
 *                                                                                             *
 *    This routine is used to determine the anti-infantry strength of this object. The         *
 *    typical user of this routine is the expert system base defense AI.                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the anti-infantry strength of this object. The value returned is      *
 *          abstract and should only be used for relative comparisons.                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/02/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int TechnoClass::Anti_Infantry(void) const
{
	if (Is_Weapon_Equipped() && GetTurretWeapon()->Weapon->Bullet->IsAntiGround) { // RA2: the turret slot's weapon
		WeaponTypeClass const * weapon = GetTurretWeapon()->Weapon; // RA2: as above
		int range = weapon->Range; // RA2: as above
		BulletTypeClass const * bullet = weapon->Bullet;
		WarheadTypeClass const * warhead = weapon->WarheadPtr;
		if (range >= 4 * CELL_LEPTON) { // RA2: as above
			range = 4 * CELL_LEPTON; // RA2: as above
		}
		double damage = weapon->Attack * warhead->Modifier[ARMOR_NONE]; // RA2: in floating point, no spread factor
		int value = (int)(damage * range / weapon->ROF); // RA2: as above
		if (TClass->Is_Two_Shooter()) {
			value += value; // RA2: as above
		}
		if (bullet->IsInaccurate) {
			value /= 2;
		}
		return(value/50);
	}
	return(0);
}



/***********************************************************************************************
 * TechnoClass::Look -- Performs a look around (map reveal) action.                            *
 *                                                                                             *
 *    This routine will reveal the map around this object.                                     *
 *                                                                                             *
 * INPUT:   incremental -- This parameter can enable a more efficient map reveal logic.        *
 *                         If it is absolutely known that the object has only moved one        *
 *                         cell from its previous location that it performed a Look() at,      *
 *                         then set this parameter to TRUE. It will only perform the look      *
 *                         check on the perimeter cells.                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine is slow, try to call it only when necessary.                       *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/14/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void TechnoClass::Look(bool incremental, bool dontmap)
{
	if (!IsLocked) { // RA2: no assertion, a plain early return
		return;
	}
	if (House->Class->IsMultiplayPassive && Session.Type != GAME_NORMAL) { // RA2: as above
		return;
	}

	int sight_increase = Get_Coord().Z / Rule->LeptonsPerSightIncrease * 10; // RA2: as in UpdateSight
	if (sight_increase > SightIncrease) {
		incremental = false;
	}
	SightIncrease = sight_increase;

	int sight_range = (int)(Techno_Type_Class()->SightRange * (SightIncrease * 0.01 + 1.0)); // RA2: as above
	if (Veterancy.Is_Veteran() || Veterancy.Is_Elite()) { // RA2: as above
		TechnoTypeClass const * type = Techno_Type_Class(); // RA2: as above
		if ((Veterancy.Is_Veteran() && type->VeteranAbilities[ABILITY_SIGHT]) // RA2: the sight ability, as in UpdateSight
			|| (Veterancy.Is_Elite() && (type->VeteranAbilities[ABILITY_SIGHT] || type->EliteAbilities[ABILITY_SIGHT]))) { // RA2: as above
			if (Rule->VeteranSight != 0.0) { // RA2: as above
				sight_range = (int)(sight_range * Rule->VeteranSight); // RA2: as above
			}
		}
	}

	if (sight_range != 0) { // RA2: as above
		Map.RevealArea1(Get_Coord(), sight_range, House, incremental, dontmap, false, true, true); // RA2: the variant that also reveals fog and shroud (0x549C50)
	}
}



/// <summary>
/// Reveals the map around this object, as Look does, and remembers where and how far it saw so that
/// RemoveSight can take the same area back. Once revealed, it only reveals again for the given house.
/// </summary>
void TechnoClass::UpdateSight(bool incremental, bool dontmap, bool for_house, HouseClass * house) // RA2: not in TibSun, at 0x6D5C30
{
	if (!IsLocked || House->Class->IsMultiplayPassive) {
		return;
	}

	int sight_increase = Get_Coord().Z / Rule->LeptonsPerSightIncrease * 10;
	if (sight_increase > SightIncrease) {
		incremental = false;
	}
	SightIncrease = sight_increase;

	int sight_range = (int)(Techno_Type_Class()->SightRange * (SightIncrease * 0.01 + 1.0));
	if (Veterancy.Is_Veteran() || Veterancy.Is_Elite()) {
		TechnoTypeClass const * type = Techno_Type_Class();
		if ((Veterancy.Is_Veteran() && type->VeteranAbilities[ABILITY_SIGHT])
			|| (Veterancy.Is_Elite() && (type->VeteranAbilities[ABILITY_SIGHT] || type->EliteAbilities[ABILITY_SIGHT]))) {
			if (Rule->VeteranSight != 0.0) {
				sight_range = (int)(sight_range * Rule->VeteranSight);
			}
		}
	}

	if (!IsSightRevealed) {
		IsSightRevealed = true;
		LastSightCoords = PositionCoord;
		LastSightRange = sight_range;
		if (sight_range != 0) {
			if (for_house && house != NULL) {
				Map.Sight_From(Get_Coord(), sight_range, house, incremental, dontmap, false, true, false);
			} else {
				Map.Sight_From(Get_Coord(), sight_range, House, incremental, dontmap, false, true, false);
			}
		}
	} else if (for_house && sight_range != 0 && house != NULL) {
		Map.Sight_From(Get_Coord(), sight_range, house, incremental, dontmap, false, true, false);
	}
}


/// <summary>
/// Takes back the sight UpdateSight revealed at LastSightCoords; once taken back, only again for the
/// given house.
/// </summary>
void TechnoClass::RemoveSight(bool incremental, bool dontmap, bool for_house, HouseClass * house) // RA2: not in TibSun, at 0x6D5E90
{
	if (IsLocked && !House->Class->IsMultiplayPassive) {
		if (IsSightRevealed) {
			IsSightRevealed = false;
			if (LastSightRange != 0 && for_house && house != NULL) {
				Map.Sight_From(LastSightCoords, LastSightRange, house, incremental, dontmap, false, true, true);
			} else if (LastSightRange != 0) {
				Map.Sight_From(LastSightCoords, LastSightRange, House, incremental, dontmap, false, true, true);
			}
		} else if (for_house && LastSightRange != 0 && house != NULL) {
			Map.Sight_From(LastSightCoords, LastSightRange, house, incremental, dontmap, false, true, true);
		}
	}
}


/// <summary>
/// Rocks the object in reaction to a nearby jolt.
/// This routine is called when something goes off near a voxel object, and it works out
/// how hard and in which direction the object should tip. A heavy object shrugs the jolt
/// off, and one far enough away is not disturbed at all.
/// </summary>
/// <param name="coord">Where the jolt came from.</param>
/// <param name="force">How violent the jolt was.</param>
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
void TechnoClass::Rock(Coord const & coord, float force)
{
	if (TClass->Voxel.VoxLib != NULL && !TClass->Voxel.VoxLib->Load_Failed()) {
		Coord pos = PositionCoord;
		Vector3 vec1;
		vec1.X = pos.X - coord.X;
		vec1.Y = coord.Y - pos.Y;
		vec1.Z = pos.Z - coord.Z;
		Vector3 vec2 = vec1;

		float theta = PrimaryFacing.Current().As_Radian();
		float sangle = fastmath::sin(theta);
		float cangle = fastmath::cos(theta);

		float dist1 = vec1.Length();

		float scale = (0.04f - dist1 * 0.000025f) * force / TClass->Weight;

		if (fabs(dist1) >= 0.00002 && scale >= 0.01f) {
			if (scale > 0.05f) {
				scale = 0.05f;
			}

			vec2.Z = 0.0;
			vec1 = Normalize(vec2);

			dist1 = vec1.X * cangle + vec1.Y * sangle;
			vec2.X = -(vec1.Z * sangle);
			vec2.Y = +(vec1.Z * cangle);
			vec2.Z = ((vec1.X * sangle) - (vec1.Y * cangle));
			float dist2 = vec2.Length();
			if (fabs(cangle * dist1 - sangle * dist2 - vec1.X) > 0.0002 || fabs(cangle * dist2 + sangle * dist1 - vec1.Y) > 0.0002) {
				dist2 = -dist2;
			}
			RockingForwardsPerFrame = dist1 * scale / 2.0f;
			RockingSidewaysPerFrame = -(dist2 * scale);
		}
	}
}
#endif


/// <summary>
/// Handles the rocking of the object for one frame.
/// This routine tips the object a little further along whatever rocking motion it was
/// given, holds it back from tipping over entirely, and then eases it toward level again
/// so the disturbance dies away. An object that is sinking ignores all of this and simply
/// keels forward.
/// </summary>
void TechnoClass::Rocking_AI(void)
{
	bool sideways_was_positive = AngleRotatedSideways > 0.00002;
	bool sideways_was_negative = AngleRotatedSideways < -0.00002;

	if (IsSinking) {
		float roll = AngleRotatedForwards < 0.0f ? -AngleRotatedForwards : AngleRotatedForwards; // RA2: the sinking object rolls on either side, towards its facing
		if (roll < (float)DEG_TO_RAD(45)) { // RA2: as above
			int facing = PrimaryFacing.Current().As_Dir8(); // RA2: as above
			if (facing != 0 && (facing <= 5 || facing > 7)) { // RA2: as above
				AngleRotatedForwards += 0.01; // RA2: as above
			} else {
				AngleRotatedForwards -= 0.01; // RA2: as above
			}
		}
		return;
	}

	if (IsRocking) {
		AngleRotatedForwards += RockingForwardsPerFrame;
		AngleRotatedSideways += RockingSidewaysPerFrame;
		return;
	}

	if (RockingSidewaysPerFrame != 0.0f) {

		AngleRotatedSideways += RockingSidewaysPerFrame;

		float angle = (float)DEG_TO_RAD(45);

		if (AngleRotatedSideways > angle) {
			AngleRotatedSideways = angle;
			RockingSidewaysPerFrame = 0.0f;
		} else if (AngleRotatedSideways < -angle) {
			AngleRotatedSideways = -angle;
			RockingSidewaysPerFrame = 0.0f;
		}

		if (sideways_was_positive) {
			if (RockingSidewaysPerFrame > 0.0f) {
				RockingSidewaysPerFrame -= 0.002f;
			} else {
				RockingSidewaysPerFrame -= 0.0050000002374872565;
			}
		} else {
			if (RockingSidewaysPerFrame < 0.0f) {
				RockingSidewaysPerFrame += 0.002f;
			} else {
				RockingSidewaysPerFrame += 0.0050000002374872565;
			}
		}

	} else {
		AngleRotatedSideways = 0.0f;
	}

	if ((sideways_was_positive && AngleRotatedSideways < 0.00002) || (sideways_was_negative && AngleRotatedSideways > -0.00002)) {
		RockingSidewaysPerFrame = 0.0f;
		AngleRotatedSideways = 0.0f;
	}

	bool forwards_was_positive = AngleRotatedForwards > 0.00002;
	bool forwards_was_negative = AngleRotatedForwards < -0.00002;

	if (RockingForwardsPerFrame != 0.0f) {

		float roll = RockingForwardsPerFrame + AngleRotatedForwards; // RA2: the new angle is kept in a local too
		AngleRotatedForwards = roll; // RA2: as above

		float angle = (float)DEG_TO_RAD(45);

		bool foot; // RA2: the foot flag is tested by byte, with a local
		if (this != NULL) foot = (AbstractFlags & ABSTRACT_FOOT); // RA2: as above
		if (this != NULL && foot && ((FootClass *)this)->IsCrushing) { // RA2: as above
			angle = (float)DEG_TO_RAD(18);
		}

		if (roll > angle) { // RA2: the local is compared, not the field
			AngleRotatedForwards = angle;
			RockingForwardsPerFrame = 0.0f;
		} else if (roll < -angle) { // RA2: as above
			AngleRotatedForwards = -angle;
			RockingForwardsPerFrame = 0.0f;
		}

		if (forwards_was_positive) {
			if (RockingForwardsPerFrame > 0.0f) {
				RockingForwardsPerFrame -= 0.002f;
			} else {
				RockingForwardsPerFrame -= 0.0050000002374872565;
			}
		} else {
			if (RockingForwardsPerFrame < 0.0f) {
				RockingForwardsPerFrame += 0.002f;
			} else {
				RockingForwardsPerFrame += 0.0050000002374872565;
			}
		}
	} else {
		AngleRotatedForwards = 0.0f;
	}

	if ((forwards_was_positive && AngleRotatedForwards < 0.00002) || (forwards_was_negative && AngleRotatedForwards > -0.00002)) {
		RockingForwardsPerFrame = 0.0f;
		AngleRotatedForwards = 0.0f;
	}
}



/// <summary>
/// Fetches the point to aim at in order to lead the target.
/// Firing at where a moving target stands is a good way to miss it. This routine guesses
/// where the target will have got to by the time the shot arrives, so that the weapon can
/// be aimed ahead of it instead. A target that is standing still needs no such courtesy.
/// </summary>
/// <returns>Returns with the coordinate to aim at. If there is no target at all, COORD_NONE
/// is returned.</returns>
Coord TechnoClass::Predict_Target_Coord(void) const
{
	Coord coord = COORD_NONE;
	if (TarCom != 0) { // RA2: RA2's form
		coord = TarCom->As_Coord();
		if (TarCom != 0 && TarCom->Fetch_RTTI() == RTTI_UNIT) { // RA2: as above
			FootClass *unit = (FootClass *)TarCom; // RA2: as above
			if (unit != 0 && unit->Locomotion->Is_Moving()) { // RA2: as above
				int speed = unit->Current_Speed();
				int distance = Distance(TarCom);
				WeaponDataStruct const *weapon = GetTurretWeapon(); // RA2: as above
				if (weapon != 0 && weapon->Weapon != 0) { // RA2: as above
					double range = distance; // RA2: as above
					int lead = (int)(range / (weapon->Weapon->Get_Speed(distance) * 0.9) * speed); // RA2: as above
					coord = Move_Coord(coord, unit->PrimaryFacing.Current(), lead); // RA2: as above
				}
			}
		}
	}
	return coord; // RA2: as above
}



/// <summary>
/// Returns a per-object cloaking/predator animation offset.
/// Computed as (object ID + PredatorOffset) modulo 400.
/// </summary>
/// <returns>Predator effect offset in the range 0-399.</returns>
int TechnoClass::Get_Predator_Offset(void) const
{
	return((Fetch_ID() + (int)PredatorOffset) % 400);
}


/// <summary>
/// Should this object mend a little of its damage now?
/// Only objects that can heal themselves qualify, whether by their nature or by the grace
/// of veterancy, and only while they are hurt badly enough to be in the yellow. The healing
/// is doled out on a slow tick, so this routine says no far more often than it says yes.
/// </summary>
/// <returns>bool; Should the object self heal this frame?</returns>
bool TechnoClass::Should_Self_Heal_Now(void) const
{
	if (!TClass->IsSelfHealing) {
		if (!Has_Ability(ABILITY_SELF_HEAL)) {
			return(false);
		}
	}
	if ((Frame % (int)(Rule->RepairRate * TICKS_PER_MINUTE)) != 0) {
		return(false);
	}
	int health = Strength; // RA2: Should_Self_Heal_Now at 0x6D6780 matches only so
	if (health == TClass->MaxStrength) { // RA2: as above
		return(false); // RA2: as above
	}
	if (health != 0) { // RA2: as above
		return(true); // RA2: as above
	}
	return(false); // RA2: as above
}


/// <summary>
/// Loads this object back in from a save game.
/// Once the raw data has been read, every pointer the object holds is still the value it
/// had when the game was saved, so they are all swizzled back into live pointers here.
/// Save games written by an older version have their layout patched up on the way through.
/// </summary>
/// <returns>Returns with S_OK, or with the failure code from the base class.</returns>
HRESULT STDMETHODCALLTYPE TechnoClass::Load(IStream * stream)
{
	HRESULT result = RadioClass::Load(stream); // RA2: RA2's form
	if (FAILED(result)) { // RA2: as above
		return result; // RA2: as above
	}
	PlanningToken = 0; // RA2: as above
	Swizzler.Swizzle((void **)&House); // RA2: as above
	for (int index = 0; index < 8; index++) { // RA2: as above
		Swizzler.Swizzle((void **)&ParticleSystems[index]); // RA2: as above
	}
	Swizzler.Swizzle((void **)&ArchivedTarget); // RA2: as above
	Swizzler.Swizzle((void **)&QueueUpToEnter); // RA2: as above
	Swizzler.Swizzle((void **)&TarCom); // RA2: as above
	Swizzler.Swizzle((void **)&SuspendedTarCom); // RA2: as above
	Swizzler.Swizzle((void **)&CaptureManager); // RA2: as above
	Swizzler.Swizzle((void **)&OldTeam); // RA2: as above
	Swizzler.Swizzle((void **)&SpawnManager); // RA2: as above
	Swizzler.Swizzle((void **)&MindControlledBy); // RA2: as above
	Swizzler.Swizzle((void **)&SpawnOwner); // RA2: as above
	Swizzler.Swizzle((void **)&Disguise); // RA2: as above
	Swizzler.Swizzle((void **)&DisguisedAsHouse); // RA2: as above
	Swizzler.Swizzle((void **)&TemporalImUsing); // RA2: as above
	Swizzler.Swizzle((void **)&TemporalTargetingMe); // RA2: as above
	Swizzler.Swizzle((void **)&Cargo.CargoHold); // RA2: as above
	Swizzler.Swizzle((void **)&Wave); // RA2: as above
	if (BehindAnim) { // RA2: as above
		AnimPtrTracker.Delete(this); // RA2: as above
		AnimPtrTracker.Add(this); // RA2: as above
	}
	Swizzler.Swizzle((void **)&BehindAnim); // RA2: as above
	TurretRotateSoundController.Init(); // RA2: as above
	IsTurretRotateSoundPlaying = 1; // RA2: as above
	TurretIsRotating = 0; // RA2: as above
	return S_OK; // RA2: as above
}


/// <summary>
/// RA2: TechnoClass's own Save, between RadioClass's and the derived classes'. It lets the base class write its part and reports success.
/// </summary>
HRESULT STDMETHODCALLTYPE TechnoClass::Save(IStream * stream, BOOL cleardirty) // RA2: not in TibSun, at 0x6D6A80
{
	RadioClass::Save(stream, cleardirty); // RA2: the result is dropped
	return(S_OK); // RA2: as above
}






/// <summary>
/// Adds this object's state to the running game CRC.
/// This routine serves the multiplayer sync check. Every value that must agree between the
/// machines playing the game is folded in, so that a desynchronization is caught on the
/// frame it happens rather than long afterwards.
/// </summary>
void TechnoClass::Compute_CRC(CRCEngine & crc) const
{
	RadioClass::Compute_CRC(crc); // RA2: RA2's form
	crc((long)Group); // RA2: as above
	crc((long)ActLike); // RA2: as above
	crc(ArmorBias);
	crc(FirepowerBias);
	crc(IdleTimer.Value()); // RA2: as above
	crc((long)SpiedBy); // RA2: as above
	crc((long)Cloak); // RA2: as above
	crc(CloakDelay.Value()); // RA2: as above
	crc(PredatorOffset);
	crc(IsSightRevealed); // RA2: as above
	crc((long)LastSightRange); // RA2: as above
	crc(PitchAngle);
	crc(Arm.Value()); // RA2: as above
	crc((long)Ammo); // RA2: as above
	crc(ReloadTimer.Value()); // RA2: as above
	crc((long)PurchasePrice); // RA2: as above
	crc(AngleRotatedSideways);
	crc(AngleRotatedForwards);
	crc(RockingSidewaysPerFrame);
	crc(RockingForwardsPerFrame);
	crc(IsSinking);
	crc(IsNeedingRescue);
	crc(IsUseless);
	crc(IsTickedOff);
	crc(IsCloakable);
	crc(IsLeader);
	crc(IsALoaner);
	crc(IsLocked);
	crc(IsTethered); // RA2: as above
	crc(IsAlternativeTether); // RA2: as above
	if (Session.Type == GAME_NORMAL || Session.Type == GAME_SKIRMISH) { // RA2: as above
		crc(IsOwnedByPlayer);
		crc(IsDiscoveredByPlayer);
		crc(IsDiscoveredByComputer);
	}
	crc(IsALemon);
	crc((long)StunDuration); // RA2: as above
	crc((long)Cooldown); // RA2: as above
	crc((long)Unused1); // RA2: as above
	crc(SightIncrease);
	int count = CurrentTargets.Count(); // RA2: as above
	crc((long)count); // RA2: as above
	for (int index = 0; index < count; index++) { // RA2: as above
		AbstractClass *object = CurrentTargets[index]; // RA2: as above
		int value = CurrentTargetThreatValues[index]; // RA2: as above
		crc(object->Fetch_ID()); // RA2: as above
		crc((long)value); // RA2: as above
	}
	count = AttackedTargets.Count(); // RA2: as above
	crc((long)count); // RA2: as above
	for (index = 0; index < count; index++) { // RA2: as above
		crc(AttackedTargets[index]->Fetch_ID()); // RA2: as above
	}
}
 // RA2: no LimpetType (not in RA2's layout)


/***********************************************************************************************
 * TechnoClass::Is_Allowed_To_Recloak -- Can this object recloak?                              *
 *                                                                                             *
 *    Determine is this object can recloak now and returns that info. Usually the answer is    *
 *    yes, but it can be overridden be derived classes.                                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Can this object recloak now? (presumes it has the ability to cloak)          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoClass::Is_Allowed_To_Recloak(void) const
{
	if (IsCloakable) {
		return(true);
	}
	return(false);
}


/// <summary>
/// Whether a chrono weapon is warping this object out.
/// </summary>
bool TechnoClass::IsBeingWarpedOut(void) const // RA2: not in TibSun, at 0x6D6DF0
{
	return(BeingWarpedOut);
}


/// <summary>
/// Whether this object is warping in.
/// </summary>
bool TechnoClass::IsWarpingIn(void) const // RA2: not in TibSun, at 0x6D6E00
{
	return(WarpingOut);
}


/// <summary>
/// Whether this object's chrono weapon is warping something out.
/// </summary>
bool TechnoClass::IsWarpingSomethingOut(void) const // RA2: not in TibSun, at 0x6D6E10
{
	if (TemporalImUsing != NULL && TemporalImUsing->Target != NULL) {
		return(true);
	}
	return(false);
}


/// <summary>
/// Whether this object is neither warping out nor warping in.
/// </summary>
bool TechnoClass::IsNotWarping(void) const // RA2: not in TibSun, at 0x6D6E30
{
	if (!BeingWarpedOut && !WarpingOut) {
		return(true);
	}
	return(false);
}














/// <summary>
/// Sets the object's archived (saved/secondary) target.
/// </summary>
/// <param name="target">Object to store as the archived target.</param>
void TechnoClass::Assign_Archive_Target(AbstractClass * target)
{
	ArchivedTarget = target;
}


/// <summary>
/// Is this object standing on raised ground?
/// An object up a hill or on a bridge is drawn higher up the screen than its own cell, so
/// it appears to be standing in a different cell than the one it really occupies.
/// </summary>
/// <returns>bool; Is the object on raised ground?</returns>
bool TechnoClass::Is_On_Elevation(void) const
{
	Cell cell = TacticalMap->Coord_To_Cell(Get_Coord());
	return(cell != Get_Cell());
}


/// <summary>
/// Damages everything a railgun beam passes through.
/// A railgun does not merely hit what it was aimed at -- it punches a line clean through
/// the world, and anything standing close enough to that line takes the weapon's ambient
/// damage as well. The beam is stopped by the first ground high enough to be in its way,
/// and it may bring a fragile cliff down where it strikes.
/// </summary>
/// <param name="coord">Where the beam is fired from.</param>
/// <param name="abstract">What is being fired at. The beam ends at its center and it always
/// takes damage, whether or not the beam actually passed over it.</param>
/// <param name="weapon">The weapon supplying the ambient damage and the warhead.</param>
/// <returns>Returns with the point the beam came to rest at -- the blocking ground, or the
/// target's own center if nothing got in the way.</returns>
#ifdef RA2_NON_MATCHING // not exact: 19 bytes differ (adopted text; the original reserves the sqrt argument after the third product)
Coord TechnoClass::Railgun_Beam_Damage(Coord & coord, AbstractClass *abstract, WeaponTypeClass *weapon)
{
	Coord center = abstract->Center_Coord();

	bool target_found = false;

	Cell final_cell = coord.As_Cell();
	Vector3 start(coord.X, coord.Y, coord.Z);
	Vector3 end(center.X, center.Y, center.Z);

	Coord final_impact = coord;

	Vector3 delta = end - start;
	DynamicVectorClass<ObjectClass *> targets;

	for (int i = 0; i < RAILGUN_STEPS; i++) {
		final_impact = Lerp(coord, center, i * (1.0f / (RAILGUN_STEPS - 1)));

		if (final_impact.As_Cell() != final_cell) {
			final_cell = final_impact.As_Cell();
			CellClass *cptr = &Map[final_cell];

			ObjectClass * optr; // RA2: declared first, the occupier list is picked by an if/else
			if (cptr->IsUnderBridge && final_impact.Z >= LEVEL_LEPTON_H * (cptr->Height + BRIDGE_CELL_HEIGHT)) { // RA2: as above
				optr = cptr->Cell_Bridge_Occupier(); // RA2: as above
			} else {
				optr = cptr->Cell_Occupier(); // RA2: as above
			}

			while (optr != NULL) {
				if (optr != this) {
					if (optr == abstract) {
						target_found = true;
					}
					bool techno; // RA2: the flag test is kept in a local, left unset for a null object
					if (optr != NULL) { // RA2: as above
						techno = (optr->AbstractFlags & 1); // RA2: as above
					}
					if (optr != NULL && techno) { // RA2: as above
						float distance = 0.0f;
						if (optr->RTTI != RTTI_BUILDING) {
							/*
							 * Measure the perpendicular distance from the object to the beam
							 * line: cross the offset to the object with the beam direction,
							 * cross the result back with the beam direction to get a vector
							 * perpendicular to the beam, then project the offset onto it
							 * and normalize.
							 */
							Coord obj_coord = optr->PositionCoord;
							Vector3 obj(obj_coord.X, obj_coord.Y, obj_coord.Z);
							Vector3 off = obj - start;
							Vector3 cross = Vector3::Cross_Product(off, delta);
							Vector3 perp = Vector3::Cross_Product(cross, delta);
							distance = (float)fabs((perp * off) / Vector_Length(perp)); // RA2: the length comes from a helper summing in doubles
						}

						if (distance < Rule->RailgunDamageRadius || optr == abstract) {
							if (targets.Count() == 0 || targets.ID(optr) == -1) {
								targets.Add(optr);
							}
						}
					}
				}
				optr = optr->Next; // RA2: the list is walked through the object pointer
			}

			/// We hit a cell higher than the impact point, halt gathering targets.
			if (Map.Get_Height_GL(final_impact) > final_impact.Z) {
				/// If the impact was a cell and contains a collapseable cliff, try to collapse it.
				if (Map[final_impact].Is_Tile_Destroyable_Cliff()) {
					if (Percent_Chance(Rule->CollapseChance)) {
						Map.Collapse_Cliff(&Map[final_impact]);
					}
				}
				return(final_impact);
			}
		}
	}

	CellClass *cptr = (CellClass *)abstract;

	if (!target_found && abstract != NULL && (abstract->AbstractFlags & 2)) { // RA2: as above
		ObjectClass * obj = (ObjectClass *)abstract; // RA2: the abstract flag test replaces the dynamic cast

		if (targets.Count() == 0 || targets.ID(obj) == -1) { // RA2: as above
			targets.Add(obj); // RA2: as above
		}
	}

	for (int j = 0; j < targets.Count(); j++) {
		int damage = weapon->AmbientDamage;
		targets[j]->Take_Damage(damage, 0, weapon->WarheadPtr, this, false, false); // RA2: all six arguments
	}

	/// If the impact was a cell and contains a collapseable cliff, try to collapse it.
	if (abstract->Fetch_RTTI() == RTTI_CELL && cptr->Is_Tile_Destroyable_Cliff()) { // RA2: the virtual RTTI
		if (Percent_Chance(Rule->CollapseChance)) {
			Map.Collapse_Cliff(cptr);
		}
	}

	return(center);
}
#endif




/// <summary>
/// Adds this object to the radar's tracking list.
/// This routine is used so that the radar can tell what is standing at any given radar
/// pixel without having to search every object in the game.
/// </summary>
void TechnoClass::Radar_Track(void)
{
	Map.Radar_Track(this, RadarPos);
	IsRadarTracked = true;
}


/// <summary>
/// Removes this object from the radar's tracking list.
/// </summary>
void TechnoClass::Radar_Untrack(void)
{
	Map.Radar_Untrack(this, RadarPos);
	IsRadarTracked = false;
}


/// <summary>
/// Draws this object's radar pixel on the map at its radar position.
/// </summary>
void TechnoClass::Plot_On_Radar(void) const
{
	Map.Radar_Pixel(RadarPos);
}


/// <summary>
/// Scores how worthwhile it would be to attack the given target.
/// This routine is the heart of automatic target selection. It weighs the harm the target
/// could do to this object against the harm this object could do to it, how badly hurt the
/// target already is, whether it belongs to the house's declared enemy, and how far out of
/// range the shot would be. Whether the careful per-type weights or the crude house-wide
/// ones are used depends on the house having a threat rating node.
/// </summary>
/// <param name="target">The candidate target to weigh up.</param>
/// <param name="firing_coord">The position the shot would be taken from. Pass COORD_NONE to
/// measure the range from this object's own center.</param>
/// <returns>Returns with the threat score. A target with no type at all scores zero, and
/// every real target scores above that.</returns>
double TechnoClass::Target_Threat(TechnoClass * target, Coord const & firing_coord) const
{
	double target_effectiveness_coefficient;
	double target_special_threat_coefficient;
	double my_effectiveness_coefficient;
	double target_strength_coefficient;
	double target_distance_coefficient;

	TechnoTypeClass const * ttype = Techno_Type_Class(); // RA2: Target_Threat at 0x6D7550 matches only so

	if (target->Class_Of() == NULL) {
		return(0);
	}

	if (House->IsThreatRatingNodeActive) {
		my_effectiveness_coefficient = ttype->MyEffectivenessCoefficient;
		target_effectiveness_coefficient = ttype->TargetEffectivenessCoefficient;
		target_special_threat_coefficient = ttype->TargetSpecialThreatCoefficient;
		target_strength_coefficient = ttype->TargetStrengthCoefficient;
		target_distance_coefficient = ttype->TargetDistanceCoefficient;
	} else {
		my_effectiveness_coefficient = Rule->DumbMyEffectivenessCoefficient;
		target_effectiveness_coefficient = Rule->DumbTargetEffectivenessCoefficient;
		target_special_threat_coefficient = Rule->DumbTargetSpecialThreatCoefficient;
		target_strength_coefficient = Rule->DumbTargetStrengthCoefficient;
		target_distance_coefficient = Rule->DumbTargetDistanceCoefficient;
	}

	int my_weapon_index = What_Weapon_Should_I_Use(target);
	WeaponTypeClass const * my_weapon = Get_Class_Weapon_Data(my_weapon_index)->Weapon;
	double threat = 0.0;
	RTTIType target_rtti = target->RTTI;

	if (target_rtti == RTTI_BUILDING || target_rtti == RTTI_INFANTRY || target_rtti == RTTI_UNIT || target_rtti == RTTI_AIRCRAFT) {
		if (target != NULL) {
			int target_weapon_index = target->What_Weapon_Should_I_Use((AbstractClass *)this);
			WeaponTypeClass const * target_weapon = target->Get_Class_Weapon_Data(target_weapon_index)->Weapon;
			WarheadTypeClass const * target_warhead = target_weapon != NULL ? target_weapon->WarheadPtr : NULL;
			if (target_warhead != NULL) {
				if (target->TarCom == (AbstractClass *)this) {
					threat = -target_effectiveness_coefficient * target_warhead->Modifier[ttype->Armor];
				} else {
					threat = target_effectiveness_coefficient * target_warhead->Modifier[ttype->Armor];
				}
			}

			threat += target_special_threat_coefficient * target->Techno_Type_Class()->SpecialThreatValue; // RA2: as above

			if (House->Enemy != HOUSE_NONE && House->Enemy == target->House->HeapID) {
				threat += Rule->EnemyHouseThreatBonus;
			}
		}
	}

	if (my_weapon && my_weapon->WarheadPtr) {
		threat += my_effectiveness_coefficient * my_weapon->WarheadPtr->Modifier[target->Class_Of()->Armor];
	}

	threat += target->HealthRatio * target_strength_coefficient;

	int range = my_weapon != NULL ? my_weapon->Range : Techno_Type_Class()->ThreatRange; // RA2: as above
	range /= CELL_LEPTON;

	int dist;
	if (firing_coord != COORD_NONE) {
		dist = firing_coord.Distance_To(target->Center_Coord());
	} else {
		dist = Center_Coord().Distance_To(target->Center_Coord()) / CELL_LEPTON;
	}

	threat += MAX(0, dist - range) * target_distance_coefficient;

	return(threat + 100000.0);
}





/// <summary>
/// Checks whether this veteran or elite unit has the given ability.
/// Veterans use VeteranAbilities; elites use either VeteranAbilities or EliteAbilities.
/// </summary>
/// <param name="ability">The ability to test for.</param>
/// <returns>True if the unit's veterancy grants the ability, false otherwise.</returns>
bool TechnoClass::Has_Ability(AbilityType ability) const
{
	if (Veterancy.Is_Veteran() || Veterancy.Is_Elite()) {
		TechnoTypeClass const * ttype = Techno_Type_Class(); // RA2: Has_Ability at 0x6D7910 matches only so
		if (Veterancy.Is_Veteran() && ttype->VeteranAbilities[ability]) {
			return(true);
		}
		if (Veterancy.Is_Elite() && (ttype->VeteranAbilities[ability] || ttype->EliteAbilities[ability])) {
			return(true);
		}
	}
	return(false);
}


/// <summary>
/// Resets the action line display timer to 25.
/// </summary>
void TechnoClass::Reset_Action_Line_Timer(void)
{
	ActionLineTimer = 25;
}


/// <summary>
/// Sets whether action lines are drawn for this object.
/// </summary>
/// <param name="on">True to enable action lines, false to disable.</param>
void TechnoClass::Set_Action_Lines(bool on)
{
	ActionLines = on;
}


/// <summary>
/// Fetches the brightness this object should be drawn at.
/// This routine is what makes a flashing object flash. While the flash is on, the object
/// is drawn at the opposite extreme of its usual brightness, so a dark object glares and
/// a bright one goes dim.
/// </summary>
/// <param name="brightness">The brightness the object would normally be drawn at.</param>
/// <returns>Returns with the brightness to actually draw with.</returns>
int TechnoClass::Apparent_Brightness(int brightness) const
{
	if (((FlashCount / 2) % 2) == 1) {
		return(brightness > 1500 ? 500 : 2000);
	} else {
		return(brightness);
	}
}


/// <summary>
/// Should this object show up on the radar?
/// This routine gathers up every reason an object might stay hidden -- shroud, fog, cloak,
/// being underground, or simply never having been discovered -- and decides whether the
/// radar may draw it. An object the player has merely sensed rather than seen also reports
/// how it gave itself away, so that the radar can announce it.
/// </summary>
/// <param name="detected">Set to how the object was detected when it is only showing
/// because the player sensed it.</param>
/// <returns>bool; Should the object appear on the radar?</returns>
bool TechnoClass::Is_Radar_Visible(DetectedType & detected) const
{
	TechnoTypeClass const *type = TClass; // RA2: RA2's form
	if (type->IsInvisible || IsSinking) { // RA2: as above
		return(false); // RA2: as above
	}
	if (type->IsRadarVisible) { // RA2: as above
		return(true);
	}
	if (House->Is_Player_Control()) { // RA2: as above
		return IsDiscoveredByPlayer ? true : false; // RA2: as above
	}
	int value = HeightAGL; // RA2: as above
	bool ability = Has_Ability(ABILITY_RADAR_INVISIBLE); // RA2: as above
	bool hidden = Map.Is_Shrouded(Get_Coord()) && MainWindow != 0; // RA2: as above
	bool tunnel = (Scen->Special.IsFogOfWar) && Map.Is_Fogged(Get_Coord()); // RA2: as above
	if (!tunnel && Cloak != CLOAKED && value >= -20 && !ability && !hidden) { // RA2: as above
		return(true);
	}
	if (!Is_Sensed_By_Player()) { // RA2: as above
		return(false); // RA2: as above
	}
	if (!PlayerPtr->Is_Ally(House) && !ability && !tunnel && !hidden) { // RA2: as above
		detected = value < -20 ? DETECTED_SUBTERRANEAN : DETECTED_CLOAKED; // RA2: as above
	}
	return(true); // RA2: as above
}



/// <summary>
/// Checks whether the cell at this object's center is sensed by the local player.
/// </summary>
/// <returns>True if the player senses the object's cell, false if there is no player.</returns>
bool TechnoClass::Is_Sensed_By_Player(void) const
{
	if (PlayerPtr != NULL) {
		CellClass * cptr = &Map[Center_Coord()];
		return(cptr->Is_Sensed(PlayerPtr->HeapID));
	}
	return(false);
}


/// <summary>
/// Checks whether the cell at this object's center is sensed by the given house.
/// </summary>
/// <param name="house">The house to test sensing for.</param>
/// <returns>True if the house senses the object's cell, false if house is NULL.</returns>
bool TechnoClass::Is_Sensed_By_House(HouseClass const * house) const
{
	if (house != NULL) {
		CellClass * cptr = &Map[Center_Coord()];
		return(cptr->Is_Sensed(house->HeapID));
	}
	return(false);
}


/// <summary>
/// Scrubs a departing target from everything that was aiming at it.
/// This routine is called when a target is about to cease to exist. Anything attacking it
/// falls back to whatever mission it was on beforehand, and any team that was built around
/// it is left without one, rather than being left holding a pointer to something that is
/// no longer there.
/// </summary>
/// <param name="target">The target that is going away.</param>
void TechnoClass::Remove_Target(AbstractClass * target)
{
	for (int i = Technos.Count() - 1; i >= 0; i--) {
		TechnoClass * techno = Technos[i];
		if (techno->TarCom == target) {
			techno->Restore_Mission();
			if (techno->Fetch_RTTI() == RTTI_AIRCRAFT && techno->CurrentMission == MISSION_PATROL) {
				techno->Status = 0;
				((AircraftClass*)techno)->IsLockedStraight = false;
			}
			if (techno->TarCom == target) {
				techno->Assign_Target(NULL);
			}
		}
	}
	for (int j = Teams.Count() - 1; j >= 0; j--) {
		TeamClass * team = Teams[j];
		if (team->MissionTarget == target) {
			team->MissionTarget = NULL;
		}
		if (team->Target == target) {
			team->Target = NULL;
		}
	}
}





/// <summary>
/// Should this weapon lob its shot rather than fire flat?
/// Some weapons always arc their shots. The rest will arc anyway when the target is high
/// enough overhead that a flat shot would only bury itself in the hillside between them.
/// </summary>
/// <param name="which">Which of this object's weapons is being fired.</param>
/// <returns>bool; Should the shot be lobbed? An object with no weapon in that slot answers
/// yes.</returns>
bool TechnoClass::Should_Use_High_Arc(int which) const
{
	WeaponTypeClass const * weapon = Get_Class_Weapon_Data(which)->Weapon;
	if (weapon == NULL) {
		return(true);
	}

	bool result = weapon->IsLobber;

	if (!result) {
		if (TarCom != NULL) {
			int dz = TarCom->Center_Coord().Z - Get_Coord().Z;
			if (dz > 0) {
				if (Point2D(Get_Coord()).Distance_To(Point2D(TarCom->Center_Coord())) < dz) {
					result = true;
				}
			}
		}
	}

	return(result);
}


/// <summary>
/// Decrements the ammunition counter by one if any ammo remains.
/// </summary>
void TechnoClass::Reduce_Ammunition(void)
{
	if (Ammo > 0) {
		Ammo--;
	}
}





/// <summary>
/// Explodes this object with its death weapon (the type's, else its turret weapon's, scaled by the type's
/// DeathWeaponDamageModifier, else the rules' at half its strength), the given damage added. Returns the damage.
/// </summary>
int TechnoClass::FireDeathWeapon(int additional_damage) // RA2: not in TibSun, at 0x6D7E20
{
	WeaponTypeClass const * weapon;
	TechnoTypeClass const * type;
	int damage;
	if (Techno_Type_Class()->DeathWeapon) {
		weapon = Techno_Type_Class()->DeathWeapon;
		type = Techno_Type_Class();
		damage = (int)(weapon->Attack * type->DeathWeaponDamageModifier);
	} else {
		WeaponDataStruct const * data = GetTurretWeapon();
		if (data && data->Weapon) {
			weapon = GetTurretWeapon()->Weapon;
			type = Techno_Type_Class();
			damage = (int)(weapon->Attack * type->DeathWeaponDamageModifier);
		} else {
			weapon = Rule->DeathWeapon;
			damage = (int)(Techno_Type_Class()->MaxStrength * 0.5);
		}
	}
	if (weapon == NULL) {
		return(0);
	}
	damage += additional_damage;
	BulletClass * bullet = weapon->Bullet->Create_Bullet(this, this, damage, weapon->WarheadPtr, 0, weapon->IsBright);
	if (bullet != NULL) {
		bullet->Set_Weapon_Type((WeaponTypeClass *)weapon);
		bullet->Limbo();
		bullet->Detonate(Center_Coord());
		bullet->Release();
	}
	return(damage);
}


/// <summary>
/// Removes the damage smoke when the object no longer warrants it.
/// This routine is called as the object's condition is re-examined. A repaired object
/// stops smoking, and so does one that has gone below ground where nobody can see it.
/// </summary>
void TechnoClass::Remove_Damage_Particle(void)
{
	if (HealthRatio > Rule->ConditionYellow || HeightAGL < -10) {
		if (ParticleSystems[3]) { // RA2: Remove_Damage_Particle at 0x6D7F30 matches only so
			ParticleSystems[3]->Delete_Me(); // RA2: as above
		}
	}
}


/// <summary>
/// Sends this object into the building it was waiting to enter.
/// A unit that is sent to a repair bay, or an aircraft sent to a helipad, remembers the
/// building it wants and asks again once it has finished moving. This routine makes that
/// second attempt. An aircraft that only needs rearming settles for docking instead, and
/// a building that has been destroyed in the meantime is simply forgotten.
/// </summary>
/// <returns>bool; Was the enter mission assigned?</returns>
bool TechnoClass::Enter_Object_Nearby(void)
{
	if (QueueUpToEnter == 0) { // RA2: RA2's form
		return(false); // RA2: as above
	}
	if (((ObjectClass *)QueueUpToEnter)->IsActive) { // RA2: as above
		AbstractClass *contact = QueueUpToEnter; // RA2: as above
		if (contact != 0 && (contact->AbstractFlags & ABSTRACT_TECHNO)) { // RA2: as above
			TechnoClass *techno = (TechnoClass *)contact; // RA2: as above
			if (Transmit_Message(RADIO_HELLO, techno) == RADIO_ROGER) { // RA2: as above
				if (Transmit_Message(RADIO_CAN_LOAD, techno) == RADIO_ROGER) { // RA2: as above
					QueueMission(MISSION_ENTER, false); // RA2: as above
					Assign_Destination(techno, true); // RA2: as above
					QueueUpToEnter = 0; // RA2: as above
					return(true);
				}
				QueueMission(MISSION_NONE, false); // RA2: as above
				Assign_Destination(0, true); // RA2: as above
				QueueUpToEnter = 0; // RA2: as above
				Transmit_Message(RADIO_OVER_OUT, techno); // RA2: as above
				return(true); // RA2: as above
			}
			if (techno->RTTI == RTTI_BUILDING && RTTI == RTTI_AIRCRAFT) { // RA2: as above
				BuildingTypeClass *type = ((BuildingClass *)techno)->Class; // RA2: as above
				if (type->IsCanUnitReload && !type->IsCanUnitRepair) { // RA2: as above
					Transmit_Message(RADIO_DOCKING, techno); // RA2: as above
				}
			}
		}
	} else { // RA2: as above
		QueueUpToEnter = 0; // RA2: as above
	}
	return(false);
}



/// <summary>
/// Assigns a MISSION_MOVE toward a cell near the cached nearby object.
/// Clears the nearby object reference if it is no longer active.
/// </summary>
/// <returns>True if a move mission was assigned; false otherwise.</returns>
bool TechnoClass::Move_To_Object_Nearby(void)
{
	TechnoClass * techno = QueueUpToEnter; // RA2: Move_To_Object_Nearby at 0x6D8080 matches only so
	if (techno != NULL) { // RA2: as above
		if ((techno->AbstractFlags & ABSTRACT_TECHNO) && techno->IsActive) { // RA2: as above
			Cell cell = Nearby_Location(techno); // RA2: as above
			if (cell != CELL_NONE) { // RA2: as above
				Assign_Destination(&Map[cell], true); // RA2: as above
				QueueMission(MISSION_MOVE, false); // RA2: as above
				return(true);
			} // RA2: Move_To_Object_Nearby at 0x6D8080 has no NearbyObject
			return(false); // RA2: as above
		}
		QueueUpToEnter = NULL; // RA2: as above
	}
	return(false);
}


/// <summary>
/// Scatters any infantry that were heading into this object.
/// The base object never receives infantry, so the routine does nothing here. Objects
/// that infantry can be sent into override it.
/// </summary>
void TechnoClass::Scatter_Incoming_Infantry(void) const
{
}


/// <summary>
/// Keeps this object's blip in the right place on the radar.
/// This routine is called as the object moves about. It works out where the object now
/// belongs on the radar and moves its tracking entry to suit, announcing the first
/// sighting of a cloaked or subterranean enemy as it does so. An object that has come
/// out from under the shroud is marked as discovered while it is at it.
/// </summary>
/// <param name="force_update">Should the radar position be recomputed even for a building?</param>
void TechnoClass::Update_Radar_Position(bool force_update)
{
	if (!IsDiscoveredByPlayer && Session.Type == GAME_NORMAL) {
		IsDiscoveredByPlayer = !Map.Is_Shrouded(Center_Coord());
	}

	Point2D point;
	if (RTTI != RTTI_BUILDING || force_update) {
		point = Map.Coord_To_Radar_Pixel(PositionCoord, false);
	} else {
		point = RadarPos;
	}

	DetectedType detected = DETECTED_NONE;
	bool visible = Is_Radar_Visible(detected);
	Rect radar_rect = Map.RadarSurface->Get_Rect();

	if (detected != DETECTED_NONE && visible) { // RA2
		bool foot;
		if (this != NULL) {
			foot = (AbstractFlags & ABSTRACT_FOOT) != 0; // RA2
		}
		if (this == NULL || !foot || ((FootClass *)this)->CurrentTube < TUBE_FIRST) { // RA2
			Submit_Radar_Event(RADAREVENT_ENEMY_SENSED, Get_Coord().As_Cell()); // RA2: no speech
		}
	}

	if (!radar_rect.Is_Point_Within(point) && visible) { // RA2
		visible = Map.In_Local_Radar(Get_Target_Cell_Ptr());
		if (visible) {
			point = Map.Coord_To_Radar_Pixel(Get_Coord(), true);
		}
	}

	if (IsRadarTracked && (point != RadarPos || !visible)) { // RA2
		Radar_Untrack();
	}

	RadarPos = point;

	if (!IsRadarTracked && visible) {
		Radar_Track();
	}

	if (House == PlayerPtr && RadarFlashTimer.Value() > 0 && RadarFlashTimer.Value() % Rule->FlashFrameTime == 0) { // RA2
		Plot_On_Radar();
	}
}

/// <summary>
/// Whether the type shows turret tooltips.
/// </summary>
bool TechnoClass::Has_Turret_Tooltips(void) // RA2: not in TibSun, at 0x6D83D0
{
	return(Techno_Type_Class()->HasTurretTooltips);
}


/// <summary>
/// Whether the type has turret weapons to choose from.
/// </summary>
bool TechnoClass::Has_Turret_Weapons(void) // RA2: not in TibSun, at 0x6D83E0
{
	return(Techno_Type_Class()->Has_Turret_Weapons());
}


/// <summary>
/// Picks the turret weapon (and so the turret) at the index, or the first one when the index is out of
/// range; a charge turret keeps its own.
/// </summary>
void TechnoClass::Set_Current_Weapon(int which) // RA2: not in TibSun, at 0x6D83F0
{
	if (!Techno_Type_Class()->IsChargeTurret) {
		if (which >= 0 && which < 15) {
			CurrentTurretNumber = Techno_Type_Class()->Get_Turret_Weapon(which);
			CurrentWeaponNumber = which;
		} else {
			CurrentTurretNumber = Techno_Type_Class()->Get_Turret_Weapon(0);
			CurrentWeaponNumber = 0;
		}
	}
}


/// <summary>
/// Whether a turret has been picked.
/// </summary>
bool TechnoClass::Has_Current_Turret(void) const // RA2: not in TibSun, at 0x6D8460
{
	return(CurrentTurretNumber != -1);
}


/// <summary>
/// The turret picked last.
/// </summary>
int TechnoClass::Get_Current_Turret_Number(void) const // RA2: not in TibSun, at 0x6D8470
{
	return(CurrentTurretNumber);
}


/// <summary>
/// Whether the turret changed since the last check, which this records.
/// </summary>
int TechnoClass::Turret_Changed(void) // RA2: not in TibSun, at 0x6D8480
{
	int changed = CurrentTurretNumber != LastTurretNumber;
	LastTurretNumber = CurrentTurretNumber;
	return(changed);
}


/// <summary>
/// The weapon index of the picked turret, or 0 for a type without turret weapons.
/// </summary>
int TechnoClass::Get_Current_Weapon_Number(void) // RA2: not in TibSun, at 0x6D84A0
{
	if (Techno_Type_Class()->Has_Turret_Weapons()) {
		return(CurrentWeaponNumber);
	}
	return(0);
}


/// <summary>
/// Whether the deploy weapon fires on an area.
/// </summary>
bool TechnoClass::IsAreaFire(void) const // RA2: not in TibSun, at 0x6D84D0
{
	WeaponDataStruct const * wdata = GetDeployWeapon();
	if (wdata != NULL && wdata->Weapon != NULL) {
		return(wdata->Weapon->AreaFire);
	}
	return(false);
}


/// <summary>
/// The weapon index used for spray attacks: the secondary one.
/// </summary>
int TechnoClass::IsNotSprayAttack(void) const // RA2: not in TibSun, at 0x6D84F0
{
	return(1);
}


/// <summary>
/// The index of the secondary weapon.
/// </summary>
int TechnoClass::GetSecondaryWeaponIndex(void) const // RA2: not in TibSun, at 0x6D8500
{
	return(1);
}


/// <summary>
/// The weapon index used for spray attacks, the second query.
/// </summary>
int TechnoClass::IsNotSprayAttack2(void) const // RA2: not in TibSun, at 0x6D8510
{
	return(1);
}


/// <summary>
/// The weapon fired when deployed.
/// </summary>
WeaponDataStruct const * TechnoClass::GetDeployWeapon(void) const // RA2: not in TibSun, at 0x6D8520
{
	return(Get_Class_Weapon_Data(IsNotSprayAttack()));
}


/// <summary>
/// The weapon at the index: the elite one when this object is elite and has one there.
/// </summary>
WeaponDataStruct const * TechnoClass::Get_Class_Weapon_Data(int which) const // RA2: out of line, at 0x6D8540
{
	if (which == -1) {
		return(NULL);
	}
	if (Veterancy.Is_Elite()) {
		WeaponDataStruct const * wdata = Techno_Type_Class()->Get_Elite_Weapon(which);
		if (wdata != NULL && wdata->Weapon != NULL) {
			return(wdata);
		}
	}
	return(Techno_Type_Class()->Get_Weapon(which));
}


/// <summary>
/// The weapon of the picked turret.
/// </summary>
WeaponDataStruct const * TechnoClass::GetTurretWeapon(void) const // RA2: not in TibSun, at 0x6D85A0
{
	return(Get_Class_Weapon_Data(((TechnoClass *)this)->Get_Current_Weapon_Number()));
}


/// <summary>
/// Whether the index names a weapon: -1, the primary or secondary one, or a turret weapon of the type.
/// </summary>
int TechnoClass::Is_Valid_Weapon_Index(int which) // RA2: not in TibSun, at 0x6D85E0
{
	bool basic = which < 2;
	bool turret = Techno_Type_Class()->Has_Turret_Weapons() && which < Techno_Type_Class()->WeaponCount;
	if (which >= -1 && (basic || turret)) {
		return(1);
	}
	return(0);
}


/// <summary>
/// Whether the weapon slot holds a weapon.
/// </summary>
int __stdcall Is_Weapon_Present(WeaponDataStruct const * wdata) // RA2: not in TibSun, at 0x6D8640
{
	if (wdata != NULL && wdata->Weapon != NULL) {
		return(1);
	}
	return(0);
}


/// <summary>
/// Whether the picked turret has a weapon.
/// </summary>
int TechnoClass::Has_Turret_Weapon(void) const // RA2: not in TibSun, at 0x6D8660
{
	WeaponDataStruct const * wdata = GetTurretWeapon();
	if (wdata != NULL && wdata->Weapon != NULL) {
		return(1);
	}
	return(0);
}


#pragma warning(push)
#pragma warning(disable: 4716)
/// <summary>
/// Disguises the object as the target; a disguise records when it was put on.
/// </summary>
unsigned TechnoClass::DisguiseAs(AbstractClass * target) // RA2: not in TibSun, at 0x6D8680
{
	if (target != NULL) {
		Disguised = true;
		DisguiseCreationFrame = Frame;
	}
	Plot_On_Radar(); // RA2: no return value (the slot returns whatever Plot_On_Radar left)
}
#pragma warning(pop)


/// <summary>
/// Puts the object under the Iron Curtain for the duration.
/// </summary>
void TechnoClass::IronCurtain(int duration) // RA2: not in TibSun, at 0x6D86B0
{
	IronCurtainTimer = duration;
	IronTintStage = 0;
}


/// <summary>
/// Scales a brightness by the Iron Curtain tint stage, at most 2000.
/// </summary>
int TechnoClass::GetEffectTintIntensity(int brightness) const // RA2: not in TibSun, at 0x6D86F0
{
	int time = IronTintTimer.Value();
	int factor;
	switch (IronTintStage) {
		case 1:
			factor = ((12 - time) << 8) / 6;
			break;
		case 2:
		case 8:
			factor = 512;
			break;
		case 3:
			factor = (461 * time + 1020) / 20;
			break;
		case 4:
			factor = (1024 - 77 * time) / 8;
			break;
		case 5:
			factor = (77 * time + 816) / 16;
			break;
		case 6:
			factor = 51;
			break;
		case 7:
			factor = (3072 - 461 * time) / 6;
			break;
		case 9:
			factor = ((time + 20) << 8) / 20;
			break;
		default:
			return(brightness);
	}
	brightness = (factor * brightness) >> 8;
	if (brightness > 2000) {
		brightness = 2000;
	}
	return(brightness);
}


/// <summary>
/// RA2
/// </summary>
static inline int Timer_Left(CDTimerClass<FrameTimerClass> const & timer)
{
	return timer();
}


/// <summary>
/// Steps the Iron Curtain tint through its pulse while the curtain holds, and resets it otherwise.
/// </summary>
void TechnoClass::Update_Iron_Tint(void) // RA2: not in TibSun, at 0x6D8820
{
	if (Unknown_Virtual_32C()) {
		switch (IronTintStage) {
			case 0:
				IronTintStage = 1;
				IronTintTimer = 6;
				break;
			case 1:
				if (IronTintTimer.Value() == 0) {
					IronTintStage = 2;
					IronTintTimer = 4;
				}
				break;
			case 2:
				if (IronTintTimer.Value() == 0) {
					IronTintStage = 3;
					IronTintTimer = 20;
				}
				break;
			case 3:
				if (IronTintTimer.Value() == 0) {
					IronTintStage = 4;
					IronTintTimer = 8;
				}
				break;
			case 4:
				if (IronTintTimer.Value() == 0) {
					IronTintStage = 5;
					IronTintTimer = 16;
				}
				break;
			case 5:
				if (IronTintTimer.Value() == 0) {
					if (Timer_Left(IronCurtainTimer) < 54) {
						IronTintStage = 6;
					} else {
						IronTintStage = 4;
						IronTintTimer = 8;
					}
				}
				break;
			case 6:
				if (Timer_Left(IronCurtainTimer) <= 30) {
					IronTintStage = 7;
					IronTintTimer = 6;
				}
				break;
			case 7:
				if (IronTintTimer.Value() == 0) {
					IronTintStage = 8;
					IronTintTimer = 4;
				}
				break;
			case 8:
				if (IronTintTimer.Value() == 0) {
					IronTintStage = 9;
					IronTintTimer = 20;
				}
				break;
			case 9:
				if (IronTintTimer.Value() != 0) {
					break;
				}
			default:
				IronTintStage = 10;
				break;
		}
	} else {
		IronTintStage = 0;
	}
}


/// <summary>
/// Starts a recoil: the barrel travels back over the compress frames.
/// </summary>
void RecoilData::Fire(void) // RA2: not in TibSun, at 0x6D8B90
{
	if (Turret.Travel != 0) {
		State = 1;
		TravelFramesLeft = Turret.CompressFrames;
		if (Turret.CompressFrames < 1) {
			TravelFramesLeft = 1;
		}
		TravelPerFrame = (float)Turret.Travel / TravelFramesLeft;
	}
}


/// <summary>
/// One frame of recoil: travel, then hold, then recover, then rest.
/// </summary>
void RecoilData::Update(void) // RA2: not in TibSun, at 0x6D8BC0
{
	if (State != 0) {
		TravelSoFar = TravelPerFrame + TravelSoFar;
		if (--TravelFramesLeft <= 0) {
			switch (State) {
				case 1:
					State = 2;
					TravelFramesLeft = Turret.HoldFrames;
					TravelPerFrame = 0;
					if (Turret.HoldFrames > 1) {
						break;
					}
				case 2:
					State = 3;
					TravelFramesLeft = Turret.RecoverFrames;
					if (TravelFramesLeft < 1) {
						TravelFramesLeft = 1;
					}
					TravelPerFrame = -((float)Turret.Travel / TravelFramesLeft);
					break;
				case 3:
					State = 0;
					TravelSoFar = 0;
					break;
			}
		}
	}
}


/// <summary>
/// Adds the blink flags of a disguised object's pulse for its own player (or while the blink timer is
/// idle) to the drawing flags.
/// </summary>
unsigned TechnoClass::GetDisguiseFlags(unsigned flags) const // RA2: not in TibSun, at 0x6D8C30
{
	unsigned result = flags;
	if (DisguiseBlinkTimer.Value() != 0 && !House->Is_Player_Control()) {
		return(result);
	}
	int phase = (int)(Frame - DisguiseCreationFrame + 64) % 256;
	if (phase >= 64) {
		if (phase < 68) {
			result |= 2;
		} else if (phase < 76) {
			result |= 4;
		} else if (phase < 80) {
			result |= 2;
		} else if (phase >= 112) {
			if (phase < 116) {
				result |= 2;
			} else if (phase < 124) {
				result |= 4;
			} else if (phase < 128) {
				result |= 2;
			}
		}
	}
	return(result);
}


/// <summary>
/// Whether the house sees through this object's disguise: not while the blink timer runs for another
/// player's object, for its own player only during the visible part of the pulse, and otherwise unless
/// the cell senses disguises for the house.
/// </summary>
bool TechnoClass::IsClearlyVisibleTo(HouseClass * house) const // RA2: not in TibSun, at 0x6D8CE0
{
	if (DisguiseBlinkTimer.Value() != 0 && !House->Is_Player_Control()) {
		return(true);
	}
	if (House->Is_Player_Control() && IsDisguised()) {
		int phase = (int)(Frame - DisguiseCreationFrame + 64) % 256;
		if (phase >= 72 && phase <= 119) {
			return(false);
		}
		return(true);
	}
	CellClass const * cell = Get_Cell_Ptr();
	if (IsDisguised() && cell != NULL && !cell->Is_Disguise_Sensed((HousesType)house->HeapID)) {
		return(false);
	}
	return(true);
}


/// <summary>
/// Whether the weapon chosen against the target fires on terrain: trees, rocks and undisguised objects.
/// </summary>
bool TechnoClass::CanDisguiseAs(AbstractClass * target) const // RA2: not in TibSun, at 0x6D8DB0
{
	WeaponTypeClass const * weapon = Get_Class_Weapon_Data(What_Weapon_Should_I_Use(target))->Weapon;
	if (weapon == NULL) {
		return(false);
	}
	if (target->Fetch_RTTI() == RTTI_TERRAIN) {
		if (Rule->IsTreeTarget || weapon->TerrainFire) {
			return(true);
		}
		return(false);
	}
	if (!weapon->TerrainFire) {
		return(false);
	}
	if (target->Fetch_RTTI() == RTTI_CELL && ((CellClass *)target)->Overlay != OVERLAY_NONE
		&& OverlayTypes[((CellClass *)target)->Overlay]->IsARock) {
		return(true);
	}
	if (target != NULL && (target->AbstractFlags & ABSTRACT_OBJECT) && ((ObjectClass *)target)->IsDisguised() && ((ObjectClass *)target)->GetDisguiseHouse(true) == NULL) {
		return(true);
	}
	return(false);
}




































































































/// <summary>
/// Determines whether this object is immobilized.
/// Returns true while the stun duration is greater than zero.
/// </summary>
/// <returns>bool; is the object currently immobilized?</returns>
bool TechnoClass::IsUnderEMP(void) const
{
	return(StunDuration > 0);
}

















/// <summary>
/// Returns the maximum speed from the techno type class, or 0 if the type is NULL.
/// </summary>
/// <returns>The type's MaxSpeed, or 0 when no type is present.</returns>
int TechnoClass::Get_Max_Speed(void) const
{
	TechnoTypeClass const * type = Techno_Type_Class(); // RA2: Get_Max_Speed at 0x6D8E90 matches only so
	if (type != NULL) { // RA2: as above
		return(type->MaxSpeed); // RA2: as above
	}
	return(0);
}


/// <summary>
/// Hands back the mission code a mega event carries (a signed byte at +12 of the event).
/// </summary>
/// <param name="event">The mega event.</param>
/// <returns>The signed byte at offset 12 of the event.</returns>
int TechnoClass::RespondMegaEventMission(EventClass * event) // RA2: 0x6D8EB0 (vtable slot)
{
	return(((char *)event)[12]); // RA2: the executable reads one signed byte at +12 of its argument
}


/// <summary>
/// Drops the mega mission data of this object. The base class holds none.
/// </summary>
void TechnoClass::ClearMegaMissionData(void) // RA2: 0x6D8EC0 (vtable slot), a single ret
{
}


/// <summary>
/// Tells whether this object follows a mega mission. The base class never does.
/// </summary>
/// <returns>Always false here.</returns>
bool TechnoClass::HaveMegaMission(void) const // RA2: 0x6D8ED0 (vtable slot)
{
	return(false); // RA2: the executable returns zero
}


/// <summary>
/// Tells whether this object has an attack-move target. The base class has none.
/// </summary>
/// <returns>Always false here.</returns>
bool TechnoClass::HaveAttackMoveTarget(void) const // RA2: 0x6D8EE0 (vtable slot)
{
	return(false); // RA2: the executable returns zero
}


/// <summary>
/// Returns the mega mission of this object; -1 stands for none.
/// </summary>
/// <returns>Always -1 here.</returns>
int TechnoClass::GetMegaMission(void) const // RA2: 0x6D8EF0 (vtable slot)
{
	return(-1); // RA2: the executable returns -1
}


/// <summary>
/// Returns the coordinate of the attack-move target; (-1, -1, -1) stands for none.
/// </summary>
/// <returns>The invalid coordinate here.</returns>
Coord TechnoClass::GetAttackMoveCoords(void) // RA2: 0x6D8F00 (vtable slot)
{
	return(Coord(-1, -1, -1)); // RA2: the executable returns the coordinate (-1, -1, -1)
}


/// <summary>
/// Tells whether the type of this object lets it follow waypoints.
/// </summary>
/// <returns>What the type's CanUseWaypoint says.</returns>
bool TechnoClass::CanUseWaypoint(void) const // RA2: 0x6D8F20 (vtable slot)
{
	return(Techno_Type_Class()->CanUseWaypoint()); // RA2: two virtual calls, the type and then its CanUseWaypoint
}


/// <summary>
/// Tells whether the type of this object lets it attack while it moves.
/// </summary>
/// <returns>What the type's CanAttackMove says.</returns>
bool TechnoClass::CanAttackOnTheMove(void) const // RA2: 0x6D8F40 (vtable slot)
{
	return(Techno_Type_Class()->CanAttackMove()); // RA2: two virtual calls, the type and then its CanAttackMove
}


/// <summary>
/// Turns a plain move or attack order into the attack-move order while the force key is held.
/// </summary>
/// <param name="action">The action the cursor offers.</param>
/// <returns>The attack-move action for a move or an attack under the force key, else the action itself.</returns>
ActionType __fastcall Attack_Move_Action(ActionType action, int, int) // RA2: 0x6D8F60, a fastcall with one unused register and one unused stack argument (called at 0x49B5B5 in DisplayClass::Mouse_Left_Up)
{
	bool force = Can_Force_Order(); // RA2: the force key is asked first
	if (action == ACTION_MOVE) { // RA2: action 1 becomes 62
		if (force) { // RA2: as above
			return(ACTION_ATTACK_MOVE_NAV); // RA2: as above
		}
	} else if (action == ACTION_ATTACK && force) { // RA2: action 5 becomes 63
		return(ACTION_ATTACK_MOVE_TAR); // RA2: as above
	}
	return(action); // RA2: as above
}


/// <summary>
/// Tells whether the mega mission of this object is an attack-move. The base class has none.
/// </summary>
/// <returns>Always false here.</returns>
bool TechnoClass::MegaMissionIsAttackMove(void) const // RA2: 0x6D8F90 (vtable slot)
{
	return(false); // RA2: the executable returns zero
}


/// <summary>
/// Continues the mega mission of this object. The base class has none.
/// </summary>
/// <returns>Always false here.</returns>
bool TechnoClass::ContinueMegaMission(void) // RA2: 0x6D8FA0 (vtable slot)
{
	return(false); // RA2: the executable returns zero
}


/// <summary>
/// Runs the attack-move update of this object. The base class has none.
/// </summary>
void TechnoClass::UpdateAttackMove(void) // RA2: 0x6D8FB0 (vtable slot), a single ret
{
}


/// <summary>
/// Refreshes the mega mission of this object. The base class has none.
/// </summary>
/// <returns>Always false here.</returns>
bool TechnoClass::RefreshMegaMission(void) // RA2: 0x6D8FC0 (vtable slot)
{
	return(false); // RA2: the executable returns zero
}


















































/// <summary>
/// Sets the object that is speaking through the talk bubble.
/// This routine is used by trigger actions and team missions to put a bubble over a
/// character for a while and to take it away again afterwards. Giving the bubble to
/// someone also lifts the shroud around them, so the player can see who is talking.
/// </summary>
/// <param name="techno">The object doing the talking. Pass NULL to clear the bubble.</param>
/// <param name="bubble">The bubble to display. TALK_NONE clears it.</param>
void TechnoClass::Set_Talker(TechnoClass * techno, TalkType bubble)
{
	if (techno != NULL && bubble != TALK_NONE) {
		TalkBubbleType = bubble;
		TalkBubbleOwner = techno;
		TalkBubbleTimer = Rule->TalkBubbleTime;
		Map.RevealArea1(techno->Center_Coord(), 2, PlayerPtr, false, false, false, true, true); // RA2: the executable calls RevealArea1 (0x549C50) with both trailing flags set, not Sight_From
	} else {
		TalkBubbleType = TALK_NONE;
		TalkBubbleOwner = NULL;
		TalkBubbleTimer = 0;
	}
}


/// <summary>
/// Marks an object that stands behind something that covers it. The rules may name an
/// animation for this; it is created at the object and attached to it. Without one a
/// pair of square brackets is drawn around the point instead.
/// </summary>
/// <param name="point">Where the object is on the screen.</param>
/// <param name="window">The window being drawn (the executable does not read it).</param>
void TechnoClass::Draw_Behind_Marker(Point2D const & point, Rect const * window) // RA2: 0x6D9080 (meastes: unknown_6d9080; the name follows its caller AI)
{
	if (Visual_Character(false, NULL) == VISUAL_NORMAL && BehindAnim == NULL) { // RA2: only an object that is shown normally and has no marker yet
		if (Rule->Behind != NULL) { // RA2: the rules name the behind animation
			BehindAnim = new AnimClass(Rule->Behind, Center_Coord(), 0, 1, ShapeFlags_Type(SHAPE_CENTER|SHAPE_WIN_REL), 0); // RA2: the flags are 0x600
			if (BehindAnim != NULL) { // RA2: as above
				if (Fetch_RTTI() != RTTI_BUILDING) { // RA2: as above
					AnimPtrTracker.Delete(this); // RA2: as above
					AnimPtrTracker.Add(this); // RA2: as above
				}
				BehindAnim->Attach_To(this); // RA2: as above
				if (!Options.IsShowHidden) { // RA2: Options+0x1B (0xA40B33); without it the marker is made invisible
					BehindAnim->Make_Invisible(); // RA2: the executable sets the byte at +381 of the animation
				}
			}
		} else {
			int color = NormalDrawer->Convert_Pixel(12); // RA2: the first pass is drawn in the palette colour 12
			Point2D origin = point; // RA2: the brackets are drawn around a copy of the point
			int outside; // RA2: as above
			int inside; // RA2: as above
			if (Frame & 8) { // RA2: the brackets pulse with the frame number
				outside = 12; // RA2: as above
				inside = 8; // RA2: as above
			} else {
				outside = 10; // RA2: as above
				inside = 6; // RA2: as above
			}
			for (int pass = 0; pass < 2; pass++) { // RA2: the second pass is drawn one pixel lower right in the player's colour
				LogicalSurface->Draw_Line(origin + Point2D(-outside, -outside), origin + Point2D(-inside, -outside), color); // RA2: the executable draws ten lines per pass
				LogicalSurface->Draw_Line(origin + Point2D(inside, -outside), origin + Point2D(outside, -outside), color); // RA2: as above
				LogicalSurface->Draw_Line(origin + Point2D(outside, -outside), origin + Point2D(outside, -inside), color); // RA2: as above
				LogicalSurface->Draw_Line(origin + Point2D(outside, inside), origin + Point2D(outside, outside), color); // RA2: as above
				LogicalSurface->Draw_Line(origin + Point2D(outside, outside), origin + Point2D(inside, outside), color); // RA2: as above
				LogicalSurface->Draw_Line(origin + Point2D(-inside, outside), origin + Point2D(-outside, outside), color); // RA2: as above
				LogicalSurface->Draw_Line(origin + Point2D(-outside, outside), origin + Point2D(-outside, inside), color); // RA2: as above
				LogicalSurface->Draw_Line(origin + Point2D(-outside, -inside), origin + Point2D(-outside, -outside), color); // RA2: as above
				LogicalSurface->Draw_Line(origin + Point2D(-4, 0), origin + Point2D(4, 0), color); // RA2: the cross in the middle
				LogicalSurface->Draw_Line(origin + Point2D(0, -4), origin + Point2D(0, 4), color); // RA2: as above
				origin.X++; // RA2: as above
				origin.Y++; // RA2: as above
				RGBClass rgb = ColorSchemes[PlayerPtr->Scheme]->HSV; // RA2: the scheme's colour (ColorScheme+776) converted to RGB
				color = DSurface::Build_Hicolor_Pixel(rgb); // RA2: as a hicolor pixel
			}
		}
	}
}


/// <summary>
/// Puts the risk of this object into the threat of a cell for the owner's house.
/// </summary>
/// <param name="cell">The cell, or NULL for none.</param>
void TechnoClass::Add_Cell_Threat(CellClass * cell) // RA2: 0x6D95A0
{
	if (cell != NULL) { // RA2: as above
		ThreatPosed = Risk(); // RA2: the risk is kept in ThreatPosed
		cell->Adjust_Threat(Owner(), ThreatPosed); // RA2: as above
	}
}


/// <summary>
/// Takes back the threat this object has put into a cell.
/// </summary>
/// <param name="cell">The cell, or NULL for none.</param>
void TechnoClass::Remove_Cell_Threat(CellClass * cell) // RA2: 0x6D95D0
{
	if (cell != NULL) { // RA2: as above
		cell->Adjust_Threat(Owner(), -(int)ThreatPosed); // RA2: what ThreatPosed holds is taken off again
		ThreatPosed = 0; // RA2: as above
	}
}


/// <summary>
/// Renews the threat this object has put into a cell: the old amount goes, the current risk comes.
/// </summary>
/// <param name="cell">The cell, or NULL for none.</param>
void TechnoClass::UpdateThreatInCell(CellClass * cell) // RA2: 0x6D9610
{
	if (cell != NULL) { // RA2: as above
		Remove_Cell_Threat(cell); // RA2: both calls are expanded in the executable
		Add_Cell_Threat(cell); // RA2: as above
	}
}


/// <summary>
/// Starts the targeting timer anew with the given delay.
/// </summary>
/// <param name="delay">The number of frames until the next scan.</param>
void TechnoClass::Set_Targeting_Timer(int delay) // RA2: 0x6D9670 (no caller found in the executable; the name is mine)
{
	TargetingTimer = delay; // RA2: the executable builds a timer of the delay and stores it (+0x148), the middle field comes from an unset temporary
}


/// <summary>
/// Brings the next scan for a target closer when it is more than 10 frames away.
/// </summary>
void TechnoClass::Shorten_Targeting_Timer(void) // RA2: 0x6D96A0
{
	if (TargetingTimer.Value() > 10 && ScenarioInit == 0) { // RA2: the next scan comes soon
		TargetingTimer = Random_Pick(4, 8); // RA2: as above
	}
}


/// <summary>
/// Tells whether the targeting timer has run out.
/// </summary>
/// <returns>True when no frames remain.</returns>
bool TechnoClass::TargetingTimerFinished(void) // RA2: 0x6D9710 (YR's name)
{
	return(TargetingTimer.Value() == 0); // RA2: as above
}


// RA2: no Get_Collateral_Damage (all of YR's uses YR-only members)


/***********************************************************************************************
 * TechnoClass::Find_Docking_Bay -- Searches for a close docking bay.                          *
 *                                                                                             *
 *    This routine will be used to find a building that can serve as a docking bay. The        *
 *    closest building that qualifies will be returned. If no building could be found then     *
 *    return with NULL.                                                                        *
 *                                                                                             *
 * INPUT:   b  -- The structure type to look for.                                              *
 *                                                                                             *
 *          friendly -- Allow searching for allied buildings as well.                          *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the building that can serve as the best docking bay.     *
 *                                                                                             *
 * WARNINGS:   This routine might return NULL even if there are buildings of the specified     *
 *             type available. This is the case when the building(s) are currently busy and    *
 *             cannot serve as a docking bay at the moment.                                    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1995 JLB : Created.                                                                 *
 *   08/13/1995 JLB : Recognizes the "IsLeader" method of building preference.                 *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
BuildingClass * TechnoClass::Find_Docking_Bay(BuildingTypeClass const * b, bool friendly, bool evenoccupied) const
{
	BuildingClass * best = 0;

	/*
	**	First check to see if there are ANY buildings of the specified
	**	type in this house's inventory. If not, then don't bother to scan
	**	for one.
	*/
	if (House->BQuantity.Value(b->HeapID) != 0) {
		int bestval = -1;

		/*
		**	Loop through all the buildings and find the one that matches the specification
		**	and is willing to dock with this object.
		*/
		for (int index = 0; index < Buildings.Count(); index++) {
			BuildingClass * building = Buildings[index];

			/*
			**	Check to see if the building qualifies (preliminary scan).
			*/
			if (building != NULL &&
				(friendly ? building->House->Is_Ally(this) : building->House == House) &&
				!building->IsInLimbo &&
				building->Class == b &&
				(!evenoccupied || !building->In_Radio_Contact()) &&
				(RTTI == RTTI_AIRCRAFT || Map.Is_Same_Cell_Zone(Destination_Coord().As_Cell(), building->Center_Coord().As_Cell(), TClass->MZone, Is_Moving_Onto_Bridge(), false, false)) &&
				((TechnoClass *)this)->Transmit_Message(RADIO_CAN_LOAD, building) == RADIO_ROGER) {

				/*
				**	If the building qualifies and this building is better than the
				**	last qualifying building (as rated by distance), then record
				**	this building and keep scanning.
				*/
				int dist = Relative_Distance(building);
				if (bestval == -1 || dist < bestval || building->IsLeader) {
					best = building;
					bestval = dist;
				}
			}
		}
	}
	return(best);
} // RA2: last, no copy in game.exe
#endif


/// <summary>
/// Returns the object's current weed (tiberium weed) load.
/// Base implementation always returns 0; overridden by derived classes that carry weed.
/// </summary>
/// <returns>Current weed load (always 0.0 in this base implementation).</returns>
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
double TechnoClass::Weed_Load(void) const
{
	return(0.0);
} // RA2: last, no copy in game.exe
#endif
