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

/* $Header: /counterstrike/HOUSE.CPP 4     3/13/97 7:11p Steve_tall $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : HOUSE.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : May 21, 1994                                                 *
 *                                                                                             *
 *                  Last Update : November 4, 1996 [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   HouseClass::AI -- Process house logic.                                                    *
 *   HouseClass::AI_Aircraft -- Determines what aircraft to build next.                        *
 *   HouseClass::AI_Attack -- Handles offensive attack logic.                                  *
 *   HouseClass::AI_Base_Defense -- Handles maintaining a strong base defense.                 *
 *   HouseClass::AI_Building -- Determines what building to build.                             *
 *   HouseClass::AI_Fire_Sale -- Check for and perform a fire sale.                            *
 *   HouseClass::AI_Infantry -- Determines the infantry unit to build.                         *
 *   HouseClass::AI_Money_Check -- Handles money production logic.                             *
 *   HouseClass::AI_Power_Check -- Handle the power situation.                                 *
 *   HouseClass::AI_Unit -- Determines what unit to build next.                                *
 *   HouseClass::Abandon_Production -- Abandons production of item type specified.             *
 *   HouseClass::Active_Add -- Add an object to active duty for this house.                    *
 *   HouseClass::Active_Remove -- Remove this object from active duty for this house.          *
 *   HouseClass::Adjust_Capacity -- Adjusts the house Tiberium storage capacity.               *
 *   HouseClass::Adjust_Drain -- Adjust the power drain value of the house.                    *
 *   HouseClass::Adjust_Power -- Adjust the power value of the house.                          *
 *   HouseClass::Adjust_Threat -- Adjust threat for the region specified.                      *
 *   HouseClass::As_Pointer -- Converts a house number into a house object pointer.            *
 *   HouseClass::Assign_Handicap -- Assigns the specified handicap rating to the house.        *
 *   HouseClass::Attacked -- Lets player know if base is under attack.                         *
 *   HouseClass::Available_Money -- Fetches the total credit worth of the house.               *
 *   HouseClass::Begin_Production -- Starts production of the specified object type.           *
 *   HouseClass::Blowup_All -- blows up everything                                             *
 *   HouseClass::Can_Build -- General purpose build legality checker.                          *
 *   HouseClass::Clobber_All -- removes all objects for this house                             *
 *   HouseClass::Computer_Paranoid -- Cause the computer players to becom paranoid.            *
 *   HouseClass::Debug_Dump -- Dumps the house status data to the mono screen.                 *
 *   HouseClass::Detach -- Removes specified object from house tracking systems.               *
 *   HouseClass::Do_All_To_Hunt -- Send all units to hunt.                                     *
 *   HouseClass::Does_Enemy_Building_Exist -- Checks for enemy building of specified type.     *
 *   HouseClass::Expert_AI -- Handles expert AI processing.                                    *
 *   HouseClass::Factory_Count -- Fetches the number of factories for specified type.          *
 *   HouseClass::Factory_Counter -- Fetches a pointer to the factory counter value.            *
 *   HouseClass::Fetch_Factory -- Finds the factory associated with the object type specified. *
 *   HouseClass::Find_Build_Location -- Finds a suitable building location.                    *
 *   HouseClass::Find_Building -- Finds a building of specified type.                          *
 *   HouseClass::Find_Cell_In_Zone -- Finds a legal placement cell within the zone.            *
 *   HouseClass::Find_Juicy_Target -- Finds a suitable field target.                           *
 *   HouseClass::Fire_Sale -- Cause all buildings to be sold.                                  *
 *   HouseClass::Flag_Attach -- Attach flag to specified cell (or thereabouts).                *
 *   HouseClass::Flag_Attach -- Attaches the house flag the specified unit.                    *
 *   HouseClass::Flag_Remove -- Removes the flag from the specified target.                    *
 *   HouseClass::Flag_To_Die -- Flags the house to blow up soon.                               *
 *   HouseClass::Flag_To_Lose -- Flags the house to die soon.                                  *
 *   HouseClass::Flag_To_Win -- Flags the house to win soon.                                   *
 *   HouseClass::Get_Quantity -- Fetches the total number of aircraft of the specified type.   *
 *   HouseClass::Get_Quantity -- Gets the quantity of the building type specified.             *
 *   HouseClass::Harvested -- Adds Tiberium to the harvest storage.                            *
 *   HouseClass::HouseClass -- Constructor for a house object.                                 *
 *   HouseClass::Init -- init's in preparation for new scenario                                *
 *   HouseClass::Init_Data -- Initializes the multiplayer color data.                          *
 *   HouseClass::Is_Allowed_To_Ally -- Determines if this house is allied to make allies.      *
 *   HouseClass::Is_Ally -- Checks to see if the object is an ally.                            *
 *   HouseClass::Is_Ally -- Determines if the specified house is an ally.                      *
 *   HouseClass::Is_Hack_Prevented -- Is production of the specified type and id prohibted?    *
 *   HouseClass::Is_No_YakMig -- Determines if no more yaks or migs should be allowed.         *
 *   HouseClass::MPlayer_Defeated -- multiplayer; house is defeated                            *
 *   HouseClass::Make_Ally -- Make the specified house an ally.                                *
 *   HouseClass::Make_Enemy -- Make an enemy of the house specified.                           *
 *   HouseClass::Manual_Place -- Inform display system of building placement mode.             *
 *   HouseClass::One_Time -- Handles one time initialization of the house array.               *
 *   HouseClass::Place_Object -- Places the object (building) at location specified.           *
 *   HouseClass::Place_Special_Blast -- Place a special blast effect at location specified.    *
 *   HouseClass::Power_Fraction -- Fetches the current power output rating.                    *
 *   HouseClass::Production_Begun -- Records that production has begun.                        *
 *   HouseClass::Read_INI -- Reads house specific data from INI.                               *
 *   HouseClass::Recalc_Attributes -- Recalcs all houses existence bits.                       *
 *   HouseClass::Recalc_Center -- Recalculates the center point of the base.                   *
 *   HouseClass::Refund_Money -- Refunds money to back to the house.                           *
 *   HouseClass::Remap_Table -- Fetches the remap table for this house object.                 *
 *   HouseClass::Sell_Wall -- Tries to sell the wall at the specified location.                *
 *   HouseClass::Set_Factory -- Assign specified factory to house tracking.                    *
 *   HouseClass::Silo_Redraw_Check -- Flags silos to be redrawn if necessary.                  *
 *   HouseClass::Special_Weapon_AI -- Fires special weapon.                                    *
 *   HouseClass::Spend_Money -- Removes money from the house.                                  *
 *   HouseClass::Suggest_New_Building -- Examines the situation and suggests a building.       *
 *   HouseClass::Suggest_New_Object -- Determine what would the next buildable object be.      *
 *   HouseClass::Suggested_New_Team -- Determine what team should be created.                  *
 *   HouseClass::Super_Weapon_Handler -- Handles the super weapon charge and discharge logic.  *
 *   HouseClass::Suspend_Production -- Temporarily puts production on hold.                    *
 *   HouseClass::Tally_Score -- Fills in the score system for this round                       *
 *   HouseClass::Tiberium_Fraction -- Calculates the tiberium fraction of capacity.            *
 *   HouseClass::Tracking_Add -- Informs house of new inventory item.                          *
 *   HouseClass::Tracking_Remove -- Remove object from house tracking system.                  *
 *   HouseClass::Where_To_Go -- Determines where the object should go and wait.                *
 *   HouseClass::Which_Zone -- Determines what zone a coordinate lies in.                      *
 *   HouseClass::Which_Zone -- Determines which base zone the specified cell lies in.          *
 *   HouseClass::Which_Zone -- Determines which base zone the specified object lies in.        *
 *   HouseClass::Write_INI -- Writes the house data to the INI database.                       *
 *   HouseClass::Zone_Cell -- Finds the cell closest to the center of the zone.                *
 *   HouseClass::delete -- Deallocator function for a house object.                            *
 *   HouseClass::new -- Allocator for a house class.                                           *
 *   HouseClass::operator HousesType -- Conversion to HousesType operator.                     *
 *   HouseClass::~HouseClass -- Default destructor for a house object.                         *
 *   HouseStaticClass::HouseStaticClass -- Default constructor for house static class.         *
 *   HouseClass::AI_Raise_Power -- Try to raise power levels by selling off buildings.         *
 *   HouseClass::AI_Raise_Money -- Raise emergency cash by selling buildings.                  *
 *   HouseClass::Random_Cell_In_Zone -- Find a (technically) legal cell in the zone specified. *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#define INCLUDE_COM
#include "always.h"
#include "rect.h" // RA2: the empty rectangle is initialized first, then the geometry constants
#include "geometry.h"

#include "house.h"

#include "_logic.h"
#include "_map.h"
#include "_rtti.h"
#include "_rules.h"
#include "_rect.h" // RA2: TacticalRect for Place_Object
#include "_surface.h"
#include "_tactica.h"
#include "aircraft.h"
#include "airctype.h"
#include "app.h"
#include "assert.h"
#include "beacon.h" // RA2: BeaconManager for MPlayer_Defeated
#include "astar.h" // RA2: Plan_Naval_Yard_Path (0x4EECB0) searches a path
#include "building.h"
#include "builtype.h"
#include "ccrand.h"
#include "cell.h"
#include "conquer.h"
#include "cpoint.h"
#include "data.h"
#include "dbgprint.h"
#include "ddist.h"
#include "dsurface.h"
#include "factory.h"
#include "globals.h"
#include "goptions.h"
#include "houstype.h"
#include "incdec.h"
#include "infantry.h"
#include "infatype.h"
#include "inline.h"
#include "ion.h"
#include "ipowerevents.h"
#include "language\language.h"
#include "lightcon.h"
#include "logic.h"
#include "planning.h" // RA2: Planning_End for MPlayer_Defeated
#include "mono.h"
#include "mouse.h"
#include "overtype.h"
#include "quarry.h"
#include "revent.h"
#include "rules.h"
#include "scenario.h"
#include "scheme.h"
#include "session.h"
#include "sun.h"
#include "capmgr.h" // RA2: CaptureManagerClass::OriginalOwner for Blowup_All
#include "temporal.h" // RA2: TemporalClass::JustLetGo for Blowup_All
#include "side.h" // RA2: SideClass::From_Name for the civilian side test of Is_Allowed_To_Ally
#include "stringtable.h" // RA2: StringTable::LoadString for the end-game captions
#include "multiplayer.h" // RA2: MultiplayerGameMode, whose virtual tells Computer_Paranoid whether to act
#include "super.h"
#include "suprtype.h"
#include "surface.h"
#include "swizzle.h"
#include "tactical.h"
#include "tag.h"
#include "taskforc.h"
#include "team.h"
#include "techtype.h"
#include "tiberium.h"
#include "tracker.h"
#include "trigger.h"
#include "trigtype.h"
#include "unit.h"
#include "unittype.h"
#include "vector.h"
#include "vein.h"
#include "voc.h"
#include "vox.h"
#include "waypoint.h"
#include "windlg.h" // RA2: WS_Top_Window for MPlayer_Defeated

#include "color.hh"
#include "strategy.hh"

int __fastcall Sidebar_Which_Tab_By_Category(RTTIType, int, bool); // RA2: 0x681940, the sidebar tab of a kind and build category (defined in sidebar.cpp)

ThreatType __fastcall Quarry_To_Threat(int quarry); // RA2: 0x6220E0, a script quarry as a threat type


DynamicVectorClass<HouseClass::BuildChoiceClass *> HouseClass::BuildChoice;


/// <summary>
/// Constructs a house object without initializing it.
/// This routine is used by the load process, which will fill in every member of the house
/// from the save file and so has no use for the work the normal constructor performs.
/// </summary>
HouseClass::HouseClass(NoInitClass const & x) :
	BASECLASS(x),
	Control(x), // RA2: the exe builds these members with the no-init constructor (nothing is stored); the vectors, counters, RepairTimer and loadouts are built empty
	AlertTime(x), // RA2: as above
	BorrowedTime(x), // RA2: as above
	PowerBlackoutTimer(x), // RA2: as above
	RadarBlackoutTimer(x), // RA2: as above
	Tiberium(x), // RA2: as above
	Weed(x), // RA2: as above
	AircraftTotals(x), // RA2: as above
	InfantryTotals(x), // RA2: as above
	UnitTotals(x), // RA2: as above
	BuildingTotals(x), // RA2: as above
	DestroyedAircraft(x), // RA2: as above
	DestroyedInfantry(x), // RA2: as above
	DestroyedUnits(x), // RA2: as above
	DestroyedBuildings(x), // RA2: as above
	CapturedBuildings(x), // RA2: as above
	TotalCrates(x), // RA2: as above
	Attack(x), // RA2: as above
	AITimer(x), // RA2: as above
	PickEnemyTimer(x), // RA2: as above
	RemapColorRGB(x), // RA2: as above
	LaserColor(x), // RA2: as above
	Base(x), // RA2: as above
	DamageTime(x), // RA2: as above
	TeamTime(x), // RA2: as above
	TriggerTime(x), // RA2: as above
	SpeakAttackDelay(x), // RA2: as above
	SpeakPowerDelay(x), // RA2: as above
	SpeakMoneyDelay(x), // RA2: as above
	SpeakMaxedDelay(x), // RA2: as above
	IniName(x), // RA2: as above
	UINameString(x) // RA2: as above
{

}


/***********************************************************************************************
 * HouseClass::HouseClass -- Constructor for a house object.                                   *
 *                                                                                             *
 *    This function is the constructor and it marks the house object                           *
 *    as being allocated.                                                                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/22/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
HouseClass::HouseClass(HouseTypeClass const * type) :
	BASECLASS(),
	HeapID(HOUSE_NONE),
	Class((HouseTypeClass *)type),
	HouseTags(),
	ConYards(),
	Difficulty(Scen->CDifficulty),
	FirepowerBias(1.0),
	GroundspeedBias(1.0),
	AirspeedBias(1.0),
	ArmorBias(1.0),
	ROFBias(1.0),
	CostBias(1.0),
	BuildSpeedBias(1.0),
	RepairDelay(0.0),
	BuildDelay(0.0),
	ProductionMode(EVERYTHING),
	IsHuman(false),
	IsPlayerControl(false),
	IsStarted(false),
	IsAlerted(false),
	IsNavalBaseAllowed(true), // RA2: +0x138 starts true
	IsShipyardConYardWanted(false), // RA2: +0x139 is cleared here
	IsAITriggersOn(false),
	IsBaseBuilding(false),
	IsDiscovered(false),
	IsDefeated(false),
	IsToDie(false),
	IsToLose(false),
	IsToWin(false),
	IsCivEvacuated(false),
	FirestormDefenseActivated(false),
	IsThreatRatingNodeActive(false),
	IsRecalcNeeded(true),
	IPAddress(0),
	SquadID(0),
	LostConnection(false),
	SelectedPath(PATH_NONE),
	IsVisionary(false),
	IsTiberiumShort(false),
	IsSpied(false),
	IsThieved(false),
	IsBuiltSomething(false),
	IsResigner(false),
	IsGiverUpper(false),
	IsAllToHunt(false),
	IsParanoid(false),
	IsToLook(true),
	DidRepair(false),
	IQ(Control.IQ),
	State(STATE_BUILDUP),
	JustBuiltStructure(STRUCT_NONE),
	JustBuiltInfantry(INFANTRY_NONE),
	JustBuiltUnit(UNIT_NONE),
	JustBuiltAircraft(AIRCRAFT_NONE),
	Blockage(0),
	RepairTimer(0),
	AlertTime(0),
	BorrowedTime(0),
	Side1TechInfiltrated(false), // RA2: +0x204..+0x214 and +0x218 are stored explicitly
	Side0TechInfiltrated(false), // RA2: as above
	BarracksInfiltrated(false), // RA2: as above
	WarFactoryInfiltrated(false), // RA2: as above
	InfantryAltOwner(0), // RA2: as above
	UnitAltOwner(0), // RA2: as above
	AircraftAltOwner(0), // RA2: as above
	BuildingAltOwner(0), // RA2: as above
	AirportDocks(0), // RA2: as above
	CreditsSpent(0),
	HarvestedCredits(0),
	StolenBuildingsCredits(0),
	CurUnits(0),
	OwnedNavy(0), // RA2: +0x22C is stored explicitly
	CurBuildings(0),
	CurInfantry(0),
	CurAircraft(0),
	Credits(0),
	Capacity(0),
	AircraftFactories(0),
	InfantryFactories(0),
	UnitFactories(0),
	BuildingFactories(0),
	NumShipyards(0), // RA2: +0x52C8 and +0x52CC are stored explicitly
	NumOrePurifiers(0), // RA2: as above
	Power(0),
	Drain(0),
	AircraftFactory(NULL),
	InfantryFactory(NULL),
	UnitFactory(NULL),
	Primary_ForShips(NULL), // RA2: +0x52E4, between the unit and the building factory
	BuildingFactory(NULL),
	Primary_Unused1(NULL), // RA2: the factory slots up to +0x52F8 are all cleared
	Primary_Unused2(NULL), // RA2: as above
	Primary_Unused3(NULL), // RA2: as above
	Primary_ForDefenses(NULL), // RA2: as above
	AircraftProductionPending(false), // RA2: the nine production-pending flags +0x52FC..+0x5304 are cleared
	InfantryProductionPending(false), // RA2: as above
	UnitProductionPending(false), // RA2: as above
	ShipProductionPending(false), // RA2: as above
	BuildingProductionPending(false), // RA2: as above
	Unused1ProductionPending(false), // RA2: as above
	Unused2ProductionPending(false), // RA2: as above
	Unused3ProductionPending(false), // RA2: as above
	DefenseProductionPending(false), // RA2: as above
	FlagLocation(NULL),
	FlagHome(0,0),
	UnitsLost(0),
	BuildingsLost(0),
	WhoLastHurtMe(HOUSE_NONE),
	Center(0,0,0),
	Radius(0),
	LATime(0),
	LAEnemy(HOUSE_NONE),
	ToCapture(NULL),
	RadarSpied(0),
	PointTotal(0),
	PreferredTarget(QUARRY_ANYTHING),
	Attack(),
	Enemy(HOUSE_NONE),
	AngerNodes(),
	ScoutNodes(),
	AITimer(0),
	BuildStructure(STRUCT_NONE),
	BuildUnit(UNIT_NONE),
	BuildInfantry(INFANTRY_NONE),
	BuildAircraft(AIRCRAFT_NONE),
	RatioAITriggerTeam(100),
	RatioTeamAircraft(75),
	RatioTeamInfantry(75),
	RatioTeamUnits(75),
	BaseDefenseTeamCount(0),
	CurrentDropship(0),
	HasCloakGenerator(0),
	LaserColor(255, 255, 255), // RA2: +0x55EC starts white
	RecalcPower(true),
	RecalcRadar(true),
	SpySatActive(false), // RA2: +0x566A is cleared here
	EMPDest(0,0),
	NukeDest(0,0),
	Allies(0),
	DamageTime(TICKS_PER_MINUTE * Rule->DamageDelay),
	TeamTime(1),
	TriggerTime(0),
	SpeakAttackDelay(1),
	SpeakPowerDelay(1),
	SpeakMoneyDelay(1),
	SpeakMaxedDelay(1),
	AIGeneral(NULL),
	IniName(NULL),
	Scheme(0),
	BaseAreaMap(0),
	PowerEventConnectionPoints(),
	PowerEvents(NULL),
	field_10E20(0),
	field_10E28(1.0),
	DefenseCostMultiplier(1.0),
	EnemyArmorForcePrediction(0.33f),
	EnemyAirForcePrediction(0.33f),
	EnemyInfantryForcePrediction(0.34f),
	PowerSurplus(0)
{
	int index;

	UINameString = TStringID<31>(""); // RA2: the display label starts as the empty string
	UIName[0] = L'\0'; // RA2: the display name starts empty
	Create_ID();

	if (Class) {
		Scheme = Class->Scheme;
	}

	AbstractTypePtrTracker.Add(this);
	FactoryPtrTracker.Add(this);
	WaypointPathPtrTracker.Add(this);
	TagPtrTracker.Add(this);
	ObjectPtrTracker.Add(this);

	HeapID = (HousesType)Houses.Count();

	if (type != NULL) {
		for (index = 0; index < Houses.Count(); index++) {

			Houses[index]->AngerNodes.Add(this);
			AngerNodes.Add(Houses[index]);

			Houses[index]->ScoutNodes.Add(this);
			ScoutNodes.Add(Houses[index]);
		}
	}

	Houses.Add(this);

	memset((void *)&Regions[0], 0x00, sizeof(Regions));

	/*
	**	Explicit in-place construction of the super weapons is
	**	required here because the default constructor for super
	**	weapons must serve as a no-initialization constructor (save/load reasons).
	*/
	for (index = 0; index < SuperWeaponTypes.Count(); index++) {
		SuperWeapon.Add(new SuperClass(SuperWeaponTypes[index], this));
	}

	memset(UnitsKilled, 0, sizeof(UnitsKilled));
	memset(BuildingsKilled, 0, sizeof(BuildingsKilled));
	if (type != NULL) { // RA2: a house made from a type takes the type's name and label
		IniName = type->Name(); // RA2: the type's own name, not a string table entry
		wcscpy(UIName, type->UIName); // RA2: as above
	}
	memset((void *)&Regions[0], 0x00, sizeof(Regions));
	//Allies |= (1L << HeapID);
	Control.Allies |= (1L << HeapID);

	/*
	**	Set the time of the first AI attack.
	*/
	int attack = Random_Pick(TICKS_PER_MINUTE/2, TICKS_PER_MINUTE*2);
	attack = int(attack * Rule->AttackDelay);
	Attack.Timer = attack;
	Attack.InitialAttack = attack;

	AircraftTotals.Populate_Unit_Count(AircraftTypes.Count()); // RA2: the totals are populated for every session type
	InfantryTotals.Populate_Unit_Count(InfantryTypes.Count()); // RA2: as above
	UnitTotals.Populate_Unit_Count(UnitTypes.Count()); // RA2: as above
	BuildingTotals.Populate_Unit_Count(BuildingTypes.Count()); // RA2: as above

	DestroyedAircraft.Populate_Unit_Count(AircraftTypes.Count()); // RA2: as above
	DestroyedInfantry.Populate_Unit_Count(InfantryTypes.Count()); // RA2: as above
	DestroyedUnits.Populate_Unit_Count(UnitTypes.Count()); // RA2: as above
	DestroyedBuildings.Populate_Unit_Count(BuildingTypes.Count()); // RA2: as above

	CapturedBuildings.Populate_Unit_Count(BuildingTypes.Count()); // RA2: as above
	TotalCrates.Populate_Unit_Count(CRATE_COUNT); // RA2: as above

	for (index = 0; index < ARRAY_SIZE(Paths); index++) {
		Paths[index] = 0;
	}

	if (type != NULL) {
		IsThreatRatingNodeActive = true;
	}

	ActLike = HOUSE_NONE;
	if (Class != NULL) {
		if (Class->Side == SIDE_GDI) { // RA2: the side picks the faction, not the name prefix
			ActLike = HOUSE_GOOD;
		} else if (Class->Side == SIDE_NOD) { // RA2: as above
			ActLike = HOUSE_BAD;
		}
	}

	IUnknown * unknown = NULL;
	QueryInterface(IID_IUnknown, (void **)&unknown);
	PowerEvents = new ConnectionPointClass(IID_IPowerEvents, unknown);

	IConnectionPoint * cp;
	PowerEvents->QueryInterface(IID_IConnectionPoint, (void **)&cp);
	PowerEventConnectionPoints.Add(cp);

	unknown->Release();
}


/// <summary>
/// Can this house still bring in money?
/// This routine is used by the base building logic to decide whether the house owns, or can
/// afford to build, the refinery and harvester it needs to keep its income flowing. A house
/// that answers no must put what credits it has toward fixing that before anything else.
/// </summary>
/// <returns>bool; Is the house able to keep earning credits?</returns>
bool HouseClass::Can_Make_Money(void)
{
	int credits = Available_Money();
	int refcost = Get_First_Ownable(Rule->BuildRefinery)->Cost_Of(this); // RA2: the first refinery this house may own, not the list's first
	int harvcost = Get_First_Ownable(Rule->HarvesterUnit)->Cost_Of(this); // RA2: the first harvester this house may own

	bool hasref = ABQuantity.Value(Rule->BuildRefinery[0]->HeapID) > 0 || ABQuantity.Value(Rule->BuildRefinery[1]->HeapID) > 0; // RA2: either of the first two refineries
	bool hasharv = AUQuantity.Value(Rule->HarvesterUnit[0]->HeapID) > 0 || AUQuantity.Value(Rule->HarvesterUnit[1]->HeapID) > 0; // RA2: either of the first two harvesters

	/*
	 * If we don't have any refineries, building one is a priority.
	 */
	if (hasref) {

		/*
		 * If we have a refinery and a harvester, all's well.
		 */
		if (hasharv) {
			return(true);
		}

		bool hasfactory = false;
		for (int index = 0; index < Rule->BuildWeapons.Count(); index++) {
			hasfactory = hasfactory || ABQuantity.Value(Rule->BuildWeapons[index]->HeapID) > 0; // RA2: the flag is a plain or, as meastes
		}

		int factorycost = Rule->BuildWeapons[0]->Cost_Of(this);
		if ((hasfactory && credits >= harvcost) || (credits >= harvcost + factorycost) || (credits >= refcost)) {
			return(true);
		}

		return(false);
	}

	if (credits > refcost) {
		return(true);
	}

	return(false);
}


/// <summary>
/// Fetches one of the interfaces this house presents.
/// This is the standard COM entry point. Through it the house hands out its scripting,
/// connection point and persistence faces to whoever asks for them.
/// </summary>
/// <returns>Returns with S_OK if the interface was supplied. Otherwise, E_NOINTERFACE or
/// E_POINTER is returned.</returns>
HRESULT STDMETHODCALLTYPE HouseClass::QueryInterface(REFIID riid, LPVOID * ppvObject)
{
	if (ppvObject == NULL) {
		return(E_POINTER);
	}

	*ppvObject = NULL;

	if (riid == IID_IUnknown) {
		*ppvObject = reinterpret_cast<IUnknown *>(this);
	}

	if (riid == IID_IHouse) {
		*ppvObject = static_cast<IHouse *>(this);
	}

	if (riid == IID_IPublicHouse) {
		*ppvObject = static_cast<IPublicHouse *>(this);
	}

	if (riid == IID_IConnectionPointContainer) {
		*ppvObject = static_cast<IConnectionPointContainer *>(this);
	}

	if (riid == IID_IPersist) {
		*ppvObject = static_cast<IPersist *>(this);
	}

	if (riid == IID_IPersistStream) {
		*ppvObject = static_cast<IPersistStream *>(this);
	}

	if (*ppvObject == 0) {
		return(E_NOINTERFACE);
	}

	this->AddRef();

	return(S_OK);
}


/// <summary>
/// Is this house's base fully powered?
/// This routine serves the scripting interface, which uses it to tell whether the house is
/// running a power deficit and its buildings are suffering for it.
/// </summary>
/// <returns>bool; Does the power output meet or exceed the drain?</returns>
bool STDMETHODCALLTYPE HouseClass::Is_Powered(void)
{
	return(Power >= Drain);
}


/// <summary>
/// Fetches the application object this house belongs to.
/// This routine serves the scripting interface. Script that has hold of a house can reach
/// the rest of the game object model through the application returned.
/// </summary>
/// <returns>Returns with a pointer to the application interface.</returns>
IApplication * STDMETHODCALLTYPE HouseClass::Get_Application(void)
{
	IApplication *app;
	ApplicationInstance.QueryInterface(IID_IApplication, (void**)&app);
	return(app);
}


/// <summary>
/// Fetches the name of this house.
/// This routine serves the scripting interface. The given name of the house type is handed
/// back as a wide string so that script can identify the house it is talking to.
/// </summary>
/// <returns>Returns with a newly allocated string holding the name of the house.</returns>
/// <remarks>The caller owns the string returned and must free it.</remarks>
BSTR STDMETHODCALLTYPE HouseClass::Name(void)
{
	WCHAR psz[32];
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (char *)Class->GivenName, -1, psz, ARRAY_SIZE(psz));
	return(SysAllocString(psz));
}


/***********************************************************************************************
 * HouseClass::Available_Money -- Fetches the total credit worth of the house.                 *
 *                                                                                             *
 *    Use this routine to determine the total credit value of the house. This is the sum of    *
 *    the harvested Tiberium in storage and the initial unspent cash reserves.                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the total credit value of the house.                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/25/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
LONG STDMETHODCALLTYPE HouseClass::Available_Money(void)
{
	float scale = Class->IncomeMult; // RA2: RA2's form
	return (long)(Tiberium.Get_Total_Value() * scale + Credits); // RA2: as above
}




/// <summary>
/// Fetches the unused tiberium storage of this house.
/// This routine is part of the house interface. The computer's general watches it so that it
/// knows when another silo is called for.
/// </summary>
/// <returns>Returns with the amount of tiberium the house still has room for.</returns>
LONG STDMETHODCALLTYPE HouseClass::Available_Storage(void)
{
	return Capacity - (int)Tiberium.Get_Total_Amount(); // RA2: RA2's form
}



/// <summary>
/// Fetches the power production of this house's base.
/// This routine is part of the house interface that the computer's general uses to keep an
/// eye on the base it is running.
/// </summary>
/// <returns>Returns with the power that the house's buildings produce.</returns>
LONG STDMETHODCALLTYPE HouseClass::Power_Output(void)
{
	return(Power);
}


/// <summary>
/// Fetches the power draw of this house's base.
/// This routine is part of the house interface, and is paired with Power_Output so that the
/// computer's general can tell when another power plant is due.
/// </summary>
/// <returns>Returns with the power that the house's buildings consume.</returns>
LONG STDMETHODCALLTYPE HouseClass::Power_Drain(void)
{
	return(Drain);
}


/// <summary>
/// Fetches the number of objects this house owns in a category.
/// This routine is part of the house interface, and the computer's general consults it when
/// working out what its forces are short of.
/// </summary>
/// <param name="category">The object category to count.</param>
/// <returns>Returns with the number of objects the house owns in that category.</returns>
LONG STDMETHODCALLTYPE HouseClass::Category_Quantity(CategoryType category)
{
	int quantity = 0;

	for (int index = 0; index < Technos.Count(); index++) {
		TechnoClass * techno = Technos[index];
		if (techno->House == this && techno->TClass->Category == category) {
			quantity++;
		}
	}

	return(quantity);
}


/// <summary>
/// Fetches the apparent number of objects this house owns in a category.
/// This routine is part of the public house interface -- it reports what a rival is allowed
/// to know about the size of this house's forces.
/// </summary>
/// <param name="category">The object category to count.</param>
/// <returns>Returns with the number of objects the house owns in that category.</returns>
LONG STDMETHODCALLTYPE HouseClass::Apparent_Category_Quantity(CategoryType category)
{
	int quantity = 0;

	for (int index = 0; index < Technos.Count(); index++) {
		TechnoClass * techno = Technos[index];
		if (techno->House == this && techno->TClass->Category == category) {
			quantity++;
		}
	}

	return(quantity);
}


/// <summary>
/// Fetches the fighting worth of a category of this house's objects.
/// This routine is part of the house interface. The computer's general weighs it when it
/// judges how much it has already invested in a category.
/// </summary>
/// <param name="category">The object category to total up.</param>
/// <returns>Returns with the combined risk and reward of the objects in that
/// category.</returns>
LONG STDMETHODCALLTYPE HouseClass::Category_Power(CategoryType category)
{
	int quantity = 0;

	for (int index = 0; index < Technos.Count(); index++) {
		TechnoClass * techno = Technos[index];
		TechnoTypeClass const * ttype = techno->TClass;
		if (techno->House == this && ttype->Category == category) {
			quantity += ttype->Risk + ttype->Reward;
		}
	}

	return(quantity);
}


/// <summary>
/// Fetches the apparent fighting worth of a category of this house's objects.
/// This routine is part of the public house interface -- it reports what a rival is allowed
/// to know about how much muscle this house keeps in the category.
/// </summary>
/// <param name="category">The object category to total up.</param>
/// <returns>Returns with the combined risk and reward of the objects in that
/// category.</returns>
LONG STDMETHODCALLTYPE HouseClass::Apparent_Category_Power(CategoryType category)
{
	int quantity = 0;

	for (int index = 0; index < Technos.Count(); index++) {
		TechnoClass * techno = Technos[index];
		TechnoTypeClass const * ttype = techno->TClass;
		if (techno->House == this && ttype->Category == category) {
			quantity += ttype->Risk + ttype->Reward;
		}
	}

	return(quantity);
}


/// <summary>
/// Fetches the center of this house's base.
/// This routine is part of the house interface, and the computer's general treats the cell
/// as the anchor for its base expansion and defense decisions.
/// </summary>
/// <returns>Returns with the cell at the center of the house's base.</returns>
CellStruct STDMETHODCALLTYPE HouseClass::Base_Center(void)
{
	Cell cell = Center.As_Cell();
	return((CellStruct &)cell);
}


/// <summary>
/// Fetches the apparent center of this house's base.
/// This routine is part of the public house interface -- it reports what a rival is allowed
/// to know about where this house has settled.
/// </summary>
/// <returns>Returns with the cell at the center of the house's base.</returns>
CellStruct STDMETHODCALLTYPE HouseClass::Apparent_Base_Center(void)
{
	Cell cell = Center.As_Cell();
	return((CellStruct &)cell);
}


/// <summary>
/// Fetches the identifier of this house.
/// This routine is part of the house interface that the computer's general uses to examine
/// the house it has been given to play.
/// </summary>
/// <returns>Returns with the house's index within the house heap.</returns>
LONG STDMETHODCALLTYPE HouseClass::ID_Number(void)
{
	return(HeapID);
}


/***********************************************************************************************
 * HouseClass::Tiberium_Fraction -- Calculates the tiberium fraction of capacity.              *
 *                                                                                             *
 *    This will calculate the current tiberium (gold) load as a ratio of the maximum storage   *
 *    capacity.                                                                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the current tiberium storage situation as a ratio of load over capacity.   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/31/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
double HouseClass::Tiberium_Fraction(void) const
{
	if ((int)Tiberium.Get_Total_Amount() == 0) { // RA2: RA2's form
		return 0.0; // RA2: as above
	}
	return Tiberium.Get_Total_Amount() / Capacity; // RA2: as above
}



/***********************************************************************************************
 * HouseClass::One_Time -- Handles one time initialization of the house array.                 *
 *                                                                                             *
 *    This basically calls the constructor for each of the houses in the game. All other       *
 *    data specific to the house is initialized when the scenario is loaded.                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Only call this ONCE at the beginning of the game.                               *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/09/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::One_Time(void)
{
	// empty
}


/***********************************************************************************************
 * HouseClass::Assign_Handicap -- Assigns the specified handicap rating to the house.          *
 *                                                                                             *
 *    The handicap rating will affect combat, movement, and production for the house. It can   *
 *    either make it more or less difficult for the house (controlled by the handicap value).  *
 *                                                                                             *
 * INPUT:   handicap -- The handicap value to assign to this house. The default value for      *
 *                      a house is DIFF_NORMAL.                                                *
 *                                                                                             *
 * OUTPUT:  Returns with the old handicap value.                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *   10/22/1996 JLB : Uses act like value for multiplay only.                                  *
 *=============================================================================================*/
DiffType HouseClass::Assign_Handicap(DiffType handicap)
{
	DiffType old = Difficulty;
	Difficulty = handicap;

	if (Session.Type != GAME_NORMAL) {
		HouseTypeClass const * hptr = Class;
		FirepowerBias = hptr->FirepowerBias * Rule->Diff[handicap].FirepowerBias;
		GroundspeedBias = hptr->GroundspeedBias * Rule->Diff[handicap].GroundspeedBias * Rule->GameSpeedBias;
		AirspeedBias = hptr->AirspeedBias * Rule->Diff[handicap].AirspeedBias * Rule->GameSpeedBias;
		ArmorBias = hptr->ArmorBias * Rule->Diff[handicap].ArmorBias;
		ROFBias = hptr->ROFBias * Rule->Diff[handicap].ROFBias;
		CostBias = hptr->CostBias * Rule->Diff[handicap].CostBias;
		RepairDelay = Rule->Diff[handicap].RepairDelay;
		BuildDelay = Rule->Diff[handicap].BuildDelay;
		BuildSpeedBias = hptr->BuildSpeedBias * Rule->Diff[handicap].BuildSpeedBias * Rule->GameSpeedBias;
	} else {
		FirepowerBias = Rule->Diff[handicap].FirepowerBias;
		GroundspeedBias = Rule->Diff[handicap].GroundspeedBias * Rule->GameSpeedBias;
		AirspeedBias = Rule->Diff[handicap].AirspeedBias * Rule->GameSpeedBias;
		ArmorBias = Rule->Diff[handicap].ArmorBias;
		ROFBias = Rule->Diff[handicap].ROFBias;
		CostBias = Rule->Diff[handicap].CostBias;
		RepairDelay = Rule->Diff[handicap].RepairDelay;
		BuildDelay = Rule->Diff[handicap].BuildDelay;
		BuildSpeedBias = Rule->Diff[handicap].BuildSpeedBias * Rule->GameSpeedBias;
	}

	TeamTime = 175 * HeapID + Rule->TeamDelays[handicap];

	return(old);
}


/***********************************************************************************************
 * HouseClass::~HouseClass -- House class destructor                                           *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    8/6/96 4:48PM ST : Created                                                               *
 *=============================================================================================*/
HouseClass::~HouseClass (void)
{
	int index;

	Detach_This_From_All(this);
	Class = NULL;
	for (index = 0; index < ARRAY_SIZE(Paths); index++) {
		delete Paths[index];
		Paths[index] = NULL;
	}

	if (Session.Type == GAME_INTERNET) {
	}

	for (index = 0; index < SuperWeapon.Count(); index++) {
		delete SuperWeapon[index];
	}
	SuperWeapon.Clear();

	if (AIGeneral != NULL && GameActive) {
		AIGeneral->Release();
		AIGeneral = NULL;
	}

	while (HouseTags.Count() > 0) {
		delete HouseTags[0];
	}

	for (index = 0; index < PowerEventConnectionPoints.Count(); index++) {
		while (PowerEventConnectionPoints[index]->Release() > 0) {
			;
		}
		PowerEventConnectionPoints[index] = NULL;
	}
	PowerEventConnectionPoints.Clear();

	PowerEvents = NULL;

	AbstractTypePtrTracker.Delete(this);
	FactoryPtrTracker.Delete(this);
	WaypointPathPtrTracker.Delete(this);
	TagPtrTracker.Delete(this);
	ObjectPtrTracker.Delete(this);

	if (UnusedHouse == this) {
		UnusedHouse = NULL;
	}

	Houses.Delete(this);
}


#ifdef _DEBUG

/// <summary>
/// Prints the defense ratings of a zone to the debug screen.
/// This routine lays out one zone's line of statistics for the house debug dump.
/// </summary>
/// <param name="x">The mono screen column to print at.</param>
/// <param name="y">The mono screen row to print at.</param>
/// <param name="zone">The zone whose defense ratings should be printed.</param>
/// <param name="mono">The mono screen to print to.</param>
void HouseClass::Print_Zone_Stats(int x, int y, ZoneType zone, MonoClass * mono) const
{
	mono->Set_Cursor(x, y);
	mono->Printf("A:%-5d I:%-5d V:%-5d", ZoneInfo[zone].AirDefense, ZoneInfo[zone].InfantryDefense, ZoneInfo[zone].ArmorDefense);
}


/***********************************************************************************************
 * HouseClass::Debug_Dump -- Dumps the house status data to the mono screen.                   *
 *                                                                                             *
 *    This utility function will output the current status of the house class to the mono      *
 *    screen. Through this information bugs may be fixed or detected.                          *
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
void HouseClass::Debug_Dump(MonoClass * mono) const
{
	mono->Set_Cursor(0, 0);

	mono->Set_Cursor(1, 1);mono->Printf("[%d]%14.14s", Class->House, Class->Name());
	mono->Set_Cursor(20, 1);mono->Printf("[%d]%13.13s", ActLike, HouseTypes[ActLike]->Name());
	mono->Set_Cursor(39, 1);mono->Printf("%2d", Control.TechLevel);
	mono->Set_Cursor(45, 1);mono->Printf("%2d", Difficulty);
	mono->Set_Cursor(52, 1);mono->Printf("%2d", State);
	mono->Set_Cursor(58, 1);mono->Printf("%2d", Blockage);
	mono->Set_Cursor(65, 1);mono->Printf("%2d", IQ);
	mono->Set_Cursor(72, 1);mono->Printf("%5d", (int)RepairTimer);

	mono->Set_Cursor(10, 3);mono->Printf("%8.8s", (BuildAircraft == AIRCRAFT_NONE) ? " " : AircraftTypes[BuildAircraft]->Graphic_Name());
	mono->Set_Cursor(21, 3);mono->Printf("%3d", CurAircraft);
	mono->Set_Cursor(27, 3);mono->Printf("%8d", Credits);
	mono->Set_Cursor(37, 3);mono->Printf("%5d", Power);
	mono->Set_Cursor(45, 3);mono->Printf("%04X", RadarSpied);
	mono->Set_Cursor(52, 3);mono->Printf("%5d", PointTotal);
	mono->Set_Cursor(62, 3);mono->Printf("%5d", (int)TeamTime);
	mono->Set_Cursor(71, 3);mono->Printf("%5d", (int)AlertTime);

	mono->Set_Cursor(10, 5);mono->Printf("%8.8s", (BuildStructure == STRUCT_NONE) ? " " : BuildingTypes[BuildStructure]->Graphic_Name());
	mono->Set_Cursor(21, 5);mono->Printf("%3d", CurBuildings);
	mono->Set_Cursor(27, 5);mono->Printf("%8d", Tiberium);
	mono->Set_Cursor(37, 5);mono->Printf("%5d", Drain);
	mono->Set_Cursor(44, 5);mono->Printf("%16.16s", Name_From_Quarry(PreferredTarget));
	mono->Set_Cursor(62, 5);mono->Printf("%5d", (int)TriggerTime);
	mono->Set_Cursor(71, 5);mono->Printf("%5d", (int)BorrowedTime);

	mono->Set_Cursor(10, 7);mono->Printf("%8.8s", (BuildUnit == UNIT_NONE) ? " " : UnitTypes[BuildUnit]->Graphic_Name());
	mono->Set_Cursor(21, 7);mono->Printf("%3d", CurUnits);
	mono->Set_Cursor(27, 7);mono->Printf("%8d", Control.InitialCredits);
	mono->Set_Cursor(38, 7);mono->Printf("%5d", UnitsLost);
	mono->Set_Cursor(44, 7);mono->Printf("%08X", Allies);
	mono->Set_Cursor(71, 7);mono->Printf("%5d", (int)Attack.Timer);

	mono->Set_Cursor(10, 9);mono->Printf("%8.8s", (BuildInfantry == INFANTRY_NONE) ? " " : InfantryTypes[BuildInfantry]->Graphic_Name());
	mono->Set_Cursor(21, 9);mono->Printf("%3d", CurInfantry);
	mono->Set_Cursor(27, 9);mono->Printf("%8d", Capacity);
	mono->Set_Cursor(38, 9);mono->Printf("%5d", BuildingsLost);
	mono->Set_Cursor(45, 9);mono->Printf("%4d", Radius / CELL_LEPTON_W);
	mono->Set_Cursor(71, 9);mono->Printf("%5d", (int)AITimer);

	mono->Set_Cursor(54, 11);mono->Printf("%04X", Center.As_Cell());
	mono->Set_Cursor(71, 11);mono->Printf("%5d", (int)DamageTime);

	for (int index = 0; index < ARRAY_SIZE(Scen->GlobalFlags); index++) {
		mono->Set_Cursor(1+index, 15);
		if (Scen->GlobalFlags[index].Value) {
			mono->Print("1");
		} else {
			mono->Print("0");
		}
		if (index >= 24) break;
	}
	if (Enemy != HOUSE_NONE) {
		char const * name = "";
		mono->Set_Cursor(53, 15);mono->Printf("[%d]%21.21s", Enemy, HouseTypes[Enemy]->Name());
	}

	Print_Zone_Stats(27, 11, ZONE_NORTH, mono);
	Print_Zone_Stats(27, 13, ZONE_CORE, mono);
	Print_Zone_Stats(27, 15, ZONE_SOUTH, mono);
	Print_Zone_Stats(1, 13, ZONE_WEST, mono);
	Print_Zone_Stats(53, 13, ZONE_EAST, mono);

	mono->Fill_Attrib(1, 18, 12, 1, IsHuman ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(1, 19, 12, 1, IsPlayerControl ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(1, 20, 12, 1, IsAlerted ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(1, 21, 12, 1, IsDiscovered ? MonoClass::INVERSE : MonoClass::NORMAL);

	mono->Fill_Attrib(14, 17, 12, 1, IsDefeated ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(14, 18, 12, 1, IsToDie ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(14, 19, 12, 1, IsToWin ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(14, 20, 12, 1, IsToLose ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(14, 21, 12, 1, IsCivEvacuated ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(14, 22, 12, 1, IsRecalcNeeded ? MonoClass::INVERSE : MonoClass::NORMAL);

	mono->Fill_Attrib(27, 17, 12, 1, IsVisionary ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(27, 18, 12, 1, IsTiberiumShort ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(27, 19, 12, 1, IsSpied ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(27, 20, 12, 1, IsThieved ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(27, 22, 12, 1, IsStarted ? MonoClass::INVERSE : MonoClass::NORMAL);

	mono->Fill_Attrib(40, 17, 12, 1, IsResigner ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(40, 18, 12, 1, IsGiverUpper ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(40, 19, 12, 1, IsBuiltSomething ? MonoClass::INVERSE : MonoClass::NORMAL);
	mono->Fill_Attrib(40, 20, 12, 1, IsBaseBuilding ? MonoClass::INVERSE : MonoClass::NORMAL);
}
#endif


/***********************************************************************************************
 * HouseStaticClass::HouseStaticClass -- Default constructor for house static class.           *
 *                                                                                             *
 *    This is the default constructor that initializes all the values to their default         *
 *    settings.                                                                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/31/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
HouseStaticClass::HouseStaticClass(void) :
	IQ(0),
	TechLevel(1),
	Allies(0),
	InitialCredits(0),
	Edge(SOURCE_NORTH)
{
}
static inline int House_Owner_Bit(HouseTypeClass const * type) // RA2: the house type's bit in a house set (RequiredHouses, ForbiddenHouses), a file-local helper as in meastes
{
	return 1 << type->House; // RA2: as above
}





/***********************************************************************************************
 * HouseClass::Can_Build -- General purpose build legality checker.                            *
 *                                                                                             *
 *    This routine is called when it needs to be determined if the specified object type can   *
 *    be built by this house. Production and sidebar maintenance use this routine heavily.     *
 *                                                                                             *
 * INPUT:   type  -- Pointer to the type of object that legality is to be checked for.         *
 *                                                                                             *
 *          house -- This is the house to check for legality against. Note that this might     *
 *                   not be 'this' house since the check could be from a captured factory.     *
 *                   Captured factories build what the original owner of them could build.     *
 *                                                                                             *
 * OUTPUT:  Can the specified object be built?                                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/04/1995 JLB : Created.                                                                 *
 *   08/12/1995 JLB : Updated for GDI building sandbag walls in #9.                            *
 *   10/23/1996 JLB : Hack to allow Tanya to both sides in multiplay.                          *
 *   11/04/1996 JLB : Computer uses prerequisite record.                                       *
 *=============================================================================================*/
/*
 * 1 = can build
 * 0 = can't build
 * -1 = build limit reached
 */
int HouseClass::Can_Build(ObjectTypeClass const * type, bool forced, bool include_in_progress) const // RA2: the body follows the executable
{
	assert(type != NULL); // RA2: as above
	TechnoTypeClass const * ttype = (TechnoTypeClass const *)type; // RA2: the type is used as a techno type throughout, kept in one local

	if (!forced) { // RA2: as above

		if (ttype->IsUnbuildable) return(0); // RA2: as TibSun

		/*
		**	RA2: an object named by the prerequisite override list of any building the house owns skips all further checks.
		*/
		TypeList<int> over(ttype->PrerequisiteOverride); // RA2: +0x57C, copied
		for (int i = 0; i < over.Count(); i++) { // RA2: as above
			int id = over[i]; // RA2: as above
			if (ABQuantity.Value(id) > 0) { // RA2: as above
				goto skip; // RA2: tested; the executable jumps straight to the build limit switch
			}
		}

		if (ttype->Level == -1) return(0); // RA2: as above

		TypeList<int> pre = ttype->Prerequisite; // RA2: as above

		int level = Control.TechLevel; // RA2: as above

		if (ttype->RequiresStolenAlliedTech && !Side0TechInfiltrated) { // RA2: stolen tech flags, +0xB4C / +0x204
			return(0); // RA2: as above
		}
		if (ttype->RequiresStolenSovietTech && !Side1TechInfiltrated) { // RA2: +0xB4B / +0x205
			return(0); // RA2: as above
		}

		if (ttype->RequiredHouses != -1 && (ttype->RequiredHouses & House_Owner_Bit(Class)) == 0) { // RA2: only the listed houses may build it
			if ((ttype->RequiredHouses & InfantryAltOwner) == 0 || type->Fetch_RTTI() != RTTI_INFANTRYTYPE) { // RA2: unless an alternate owner mask of its kind lists the house
				if ((ttype->RequiredHouses & UnitAltOwner) == 0 || type->Fetch_RTTI() != RTTI_UNITTYPE) { // RA2: as above
					if ((ttype->RequiredHouses & AircraftAltOwner) == 0 || type->Fetch_RTTI() != RTTI_AIRCRAFTTYPE) { // RA2: as above
						if ((ttype->RequiredHouses & BuildingAltOwner) == 0 || type->Fetch_RTTI() != RTTI_BUILDINGTYPE) { // RA2: as above
							return(0); // RA2: as above
						}
					}
				}
			}
		}
		if (ttype->ForbiddenHouses != -1 && (ttype->ForbiddenHouses & House_Owner_Bit(Class)) != 0) { // RA2: houses that may never build it
			return(0); // RA2: as above
		}
		if (!AllowDisableableSupers && type->Fetch_RTTI() == RTTI_BUILDINGTYPE) { // RA2: a building's super weapon can be disabled from the shell
			BuildingTypeClass const * btype = (BuildingTypeClass const *)type; // RA2: as above
			if (btype->SuperWeapon != -1 && SuperWeapon[btype->SuperWeapon]->Class->DisableableFromShell) { // RA2: as above
				return(0); // RA2: as above
			}
		}

		if (ttype->Level > level) return(0); // RA2: as above

		/*
		**	The computer can always build everything.
		*/
		if (!Is_Human_Player()) return(1); // RA2: as above

		/*
		**	See if the prerequisite requirements have been met.
		*/
		for (int index = 0; index < pre.Count(); index++) { // RA2: its own counter, VC6 keeps the first loop's i in scope
			switch (pre[index]) { // RA2: as above
				case STRUCT_G_POWER: { // RA2: as above
					bool found = false; // RA2: as above
					for (int j = 0; j < Rule->PrerequisitePower.Count(); j++) { // RA2: as above
						if (ABQuantity.Value(Rule->PrerequisitePower[j]) > 0) { // RA2: as above
							found = true; // RA2: as above
							break; // RA2: as above
						}
					}
					if (!found) return(0); // RA2: as above
				}
				break; // RA2: as above

				case STRUCT_G_FACTORY: { // RA2: as above
					bool found = false; // RA2: as above
					for (int j = 0; j < Rule->PrerequisiteFactory.Count(); j++) { // RA2: as above
						if (ABQuantity.Value(Rule->PrerequisiteFactory[j]) > 0) { // RA2: as above
							found = true; // RA2: as above
							break; // RA2: as above
						}
					}
					if (!found) return(0); // RA2: as above
				}
				break; // RA2: as above

				case STRUCT_G_BARRACKS: { // RA2: as above
					bool found = false; // RA2: as above
					for (int j = 0; j < Rule->PrerequisiteBarracks.Count(); j++) { // RA2: as above
						if (ABQuantity.Value(Rule->PrerequisiteBarracks[j]) > 0) { // RA2: as above
							found = true; // RA2: as above
							break; // RA2: as above
						}
					}
					if (!found) return(0); // RA2: as above
				}
				break; // RA2: as above

				case STRUCT_G_RADAR: { // RA2: as above
					bool found = false; // RA2: as above
					for (int j = 0; j < Rule->PrerequisiteRadar.Count(); j++) { // RA2: as above
						if (ABQuantity.Value(Rule->PrerequisiteRadar[j]) > 0) { // RA2: as above
							found = true; // RA2: as above
							break; // RA2: as above
						}
					}
					if (!found) return(0); // RA2: as above
				}
				break; // RA2: as above

				case STRUCT_G_TECH: { // RA2: as above
					bool found = false; // RA2: as above
					for (int j = 0; j < Rule->PrerequisiteTech.Count(); j++) { // RA2: as above
						if (ABQuantity.Value(Rule->PrerequisiteTech[j]) > 0) { // RA2: as above
							found = true; // RA2: as above
							break; // RA2: as above
						}
					}
					if (!found) return(0); // RA2: as above
				}
				break; // RA2: as above

				case STRUCT_G_GDIFACTORY: { // RA2: -6 is the sixth group, the refinery group (Rule->PrerequisiteProc)
					bool found = false; // RA2: as above
					for (int j = 0; j < Rule->PrerequisiteProc.Count(); j++) { // RA2: as above
						if (ABQuantity.Value(Rule->PrerequisiteProc[j]) > 0) { // RA2: as above
							found = true; // RA2: as above
							break; // RA2: as above
						}
					}
					if (!found) return(0); // RA2: as above
				}
				break; // RA2: as above

				default: { // RA2: as above
					BuildingTypeClass * btype = BuildingTypes[pre[index]]; // RA2: as above
					if (btype->PowersUpBuilding[0] != '\0') { // RA2: as above
						BuildingClass * bptr; // RA2: as above
						bool found = false; // RA2: as above
						for (int j = Buildings.Count() - 1; j >= 0; j--) { // RA2: as above
							bptr = Buildings[j]; // RA2: as above
							if (!bptr->IsInLimbo && bptr->IsOn) { // RA2: the house's own building list, no owner test
								if (bptr->Mission != MISSION_DECONSTRUCTION && bptr->MissionQueue != MISSION_DECONSTRUCTION) { // RA2: as above
									found = true; // RA2: as above
									break; // RA2: as above
								}
							}
						}
						if (found) { // RA2: as above
							int k = 0; // RA2: the loop tests the slot first and checks the count at the bottom
							while (true) { // RA2: as above
								if (bptr->Upgrades[k] == btype) { // RA2: as above
									goto breakout; // RA2: as above
								}
								k++; // RA2: as above
								if (k >= ARRAY_SIZE(bptr->Upgrades)) break; // RA2: as above
							}
						}
						return(0); // RA2: as above
					}
					if (ABQuantity.Value(pre[index]) == 0) return(0); // RA2: as above
					breakout:; // RA2: as above
				}
				break; // RA2: as above
			}
		}
	}
	skip: // RA2: tested; reached by the override test above

	switch ((RTTIType)type->Fetch_RTTI()) { // RA2: the executable calls Fetch_RTTI through the vtable
		case RTTI_UNITTYPE: { // RA2: as above
			if (ttype->BuildLimit <= 0) { // RA2: as above
				if (PUQuantity.Value(type->Fetch_Heap_ID()) >= abs(ttype->BuildLimit)) return(0); // RA2: as above
			}
			int limit = ttype->BuildLimit; // RA2: as above
			if (limit <= 0) return(1); // RA2: as above
			if (UQuantity.Value(type->Fetch_Heap_ID()) < ttype->BuildLimit) return(1); // RA2: as above
			if (include_in_progress) { // RA2: as above
				if (Factory_Producing_This(type) != NULL) return(1); // RA2: as above
			}
			return(-1); // RA2: as above
		}

		case RTTI_INFANTRYTYPE: { // RA2: as above
			if (ttype->BuildLimit <= 0) { // RA2: as above
				if (PIQuantity.Value(type->Fetch_Heap_ID()) >= abs(ttype->BuildLimit)) return(0); // RA2: as above
			}
			int count = IQuantity.Value(type->Fetch_Heap_ID()); // RA2: as above
			if (((InfantryTypeClass const *)type)->IsVehicleThief) { // RA2: as above
				for (int i = 0; i < Units.Count(); i++) { // RA2: as above
					UnitClass * unit = Units[i]; // RA2: as above
					if (unit->House == this && unit->EnteredByInfType == type->Fetch_Heap_ID()) { // RA2: as above
						count++; // RA2: as above
					}
				}
			}
			if (ttype->BuildLimit <= 0) return(1); // RA2: as above
			if (count < ttype->BuildLimit) return(1); // RA2: as above
			if (include_in_progress && count == ttype->BuildLimit) { // RA2: as above
				if (Factory_Producing_This(type) != NULL) return(1); // RA2: as above
			}
			return(-1); // RA2: as above
		}

		case RTTI_BUILDINGTYPE: { // RA2: as above
			if (ttype->BuildLimit <= 0) { // RA2: as above
				if (PBQuantity.Value(type->Fetch_Heap_ID()) >= abs(ttype->BuildLimit)) return(0); // RA2: as above
			}
			int limit = ttype->BuildLimit; // RA2: as above
			if (limit <= 0) return(1); // RA2: as above
			if (BQuantity.Value(type->Fetch_Heap_ID()) < ttype->BuildLimit) return(1); // RA2: as above
			if (include_in_progress) { // RA2: as above
				if (Factory_Producing_This(type) != NULL) return(1); // RA2: as above
			}
			return(-1); // RA2: as above
		}

		case RTTI_AIRCRAFTTYPE: { // RA2: as above
			if (ttype->BuildLimit <= 0) { // RA2: as above
				if (PAQuantity.Value(type->Fetch_Heap_ID()) >= abs(ttype->BuildLimit)) return(0); // RA2: as above
			}
			int limit = ttype->BuildLimit; // RA2: as above
			if (limit <= 0) return(1); // RA2: as above
			if (AQuantity.Value(type->Fetch_Heap_ID()) < ttype->BuildLimit) return(1); // RA2: as above
			if (include_in_progress) { // RA2: as above
				if (Factory_Producing_This(type) != NULL) return(1); // RA2: as above
			}
			return(-1); // RA2: as above
		}
	}
	return(1); // RA2: as above
}



/// <summary>
/// Finds the factory that is producing the object type specified.
/// This routine is used by the sidebar so that it can show the progress of an item that is
/// already under construction.
/// </summary>
/// <param name="object">The object type to search this house's factories for.</param>
/// <returns>Returns with a pointer to the factory building it. Otherwise, NULL is
/// returned.</returns>
FactoryClass *HouseClass::Factory_Producing_This(ObjectTypeClass const * object) const
{
	FactoryClass *fptr = NULL;

	for (int i = 0; i < Factories.Count(); i++) {
		if (Factories[i]->Get_House() == this && Factories[i]->Get_Object() != NULL) {
			if (Factories[i]->Get_Object()->TClass == object) {
				fptr = Factories[i];
				break;
			}
		}
	}
	return(fptr);
}


/***********************************************************************************************
 * HouseClass::AI -- Process house logic.                                                      *
 *                                                                                             *
 *    This handles the AI for the house object. It should be called once per house per game    *
 *    tick. It processes all house global tasks such as low power damage accumulation and      *
 *    house specific trigger events.                                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/27/1994 JLB : Created.                                                                 *
 *   07/17/1995 JLB : Limits EVA speaking unless the player can do something.                  *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: 3426 of 3544 bytes differ (mischa85 text; the old-tree body is not carried yet)
void HouseClass::AI(void)
{
	//assert(Houses.ID(this) == ID);

	if (RecalcPower) {
		Recalc_Power_Drain();
		RecalcRadar = true;
	}
	if (RecalcRadar) {
		Recalc_Radar_Availability();
	}

	if ((Frame % 100) == 0) {
		for (int a = 0; a < AngerNodes.Count(); a++) {
			if (AngerNodes[a].Level > 1) {
				AngerNodes[a].Level--;
			}
		}
	}

	if (AIGeneral != NULL) {
		LONG framedelay = 0;
		AIGeneral->AI(&framedelay);
	}

	/*
	**	If base building has been turned on by a trigger, then force the house to begin
	**	production and team creation as well. This is also true if the IQ is high enough to
	**	being base building.
	*/
	if (!Is_Human_Player() && (IsBaseBuilding || IQ >= Rule->IQProduction)) {
		IsBaseBuilding = true;
		IsStarted = true;
		IsAlerted = true;
	}

	/*
	**	Check to see if the house wins.
	*/
	if (IsToWin && BorrowedTime == 0 && (Session.Type != GAME_NORMAL || Blockage <= 0)) {
		CDTimerClass<SystemTimerClass> _timer = TIMER_SECOND * 2;
		while (Is_Speaking() && _timer) {
			Call_Back();
		}
		IsToWin = false;
		if (Is_Player_Control()) {
			PlayerWins = true;
		} else {
			PlayerLoses = true;
		}
	}

	/*
	**	Check to see if the house loses.
	*/
	if (/*Session.Type == GAME_NORMAL &&*/ IsToLose && BorrowedTime == 0) {
		CDTimerClass<SystemTimerClass> _timer = TIMER_SECOND * 2;
		while (Is_Speaking() && _timer) {
			Call_Back();
		}
		IsToLose = false;
		if (Is_Player_Control()) {
			PlayerLoses = true;
		} else {
			PlayerWins = true;
		}
	}

	/*
	**	Check to see if all objects of this house should be blown up.
	*/
	if (IsToDie && BorrowedTime == 0) {
		IsToDie = false;
		Blowup_All();
	}

	/*
	**	Double check power values to correct illegal conditions. It is possible to
	**	get a power output of negative (one usually) as a result of damage sustained
	**	and the fixed point fractional math involved with power adjustments. If the
	**	power rating drops below zero, then make it zero.
	*/
	Power = MAX(Power, 0);
	Drain = MAX(Drain, 0);

#if NEVER
	/*
	**	If the base has been alerted to the enemy and should be attacking, then
	**	see if the attack timer has expired. If it has, then create the attack
	**	teams.
	*/
	if (IsAlerted && AlertTime == 0) {

		/*
		**	Adjusted to reduce maximum number of teams created.
		*/
		int maxteams = Random_Pick(2, (int)(((Control.TechLevel-1)/3)+1));
		for (int index = 0; index < maxteams; index++) {
			TeamTypeClass const * ttype = Suggested_New_Team(true);
			if (ttype != NULL) {
				ScenarioInit++;
				ttype->Create_One_Of();
				ScenarioInit--;
			}
		}
		AlertTime = Rule->AutocreateTime * Random_Pick(TICKS_PER_MINUTE/2, TICKS_PER_MINUTE*2);
//		int mintime = Rule->AutocreateTime * (TICKS_PER_MINUTE/2);
//		int maxtime = Rule->AutocreateTime * (TICKS_PER_MINUTE*2);
//		AlertTime = Random_Pick(mintime, maxtime);
	}
#endif

	/*
	**	If this house's flag waypoint is a valid cell, see if there's
	**	someone sitting on it.  If so, make the scatter.  If they refuse,
	**	blow them up.
	*/
	if (FlagHome != CELL_NONE && (Frame % TICKS_PER_SECOND) == 0) {

		TechnoClass * techno = Map[FlagHome].Cell_Techno();
		if (techno != NULL) {
			bool moving = false;
			FootClass * foot = dynamic_cast<FootClass *>(techno);
			if (foot != NULL) {
				if (foot->NavCom != NULL) {
					moving = true;
				}
			}

			if (!moving) {
				techno->Scatter(COORD_NONE, true, true);
			}

			/*
			**	If the techno doesn't have a valid NavCom, he's not moving,
			**	so blow him up.
			*/
			if (foot != NULL) {
				if (foot->NavCom != NULL) {
					moving = true;
				}
			}

			/*
			**	If the techno wasn't an infantry or unit (ie he's a building),
			**	or he refuses to move, blow him up
			*/
			if (!moving) {
				int count = 0;
				while (!(techno->IsInLimbo) && count++ < 5) {
					int damage = techno->Strength;
					techno->Take_Damage(damage, 0, Rule->C4Warhead, NULL, true);
				}
			}
		}
	}

	/*
	**	Create teams for this house if necessary.
	** (Use the same timer for some extra capture-the-flag logic.)
	*/
	if (/*!IsAlerted &&*/ !TeamTime) {

		SUGGESTED_TEAM_LIST team = Suggested_New_Team(false);
		if (team.Count() > 0) {
			for (int i = 0; i < team.Count(); i++) {
				team[i]->Create_One_Of(this);
			}
		}

		TeamTime = Rule->TeamDelays[Difficulty];
	}

	/*
	**	If there is insufficient power, then all buildings that are above
	**	half strength take a little bit of damage.
	*/
	if (DamageTime == 0) {

		/*
		**	When the power is below required, then the buildings will take damage over
		**	time.
		*/
		if (Power_Fraction() < 1) {
			for (int index = 0; index < Buildings.Count(); index++) {
				BuildingClass & b = *Buildings[index];

				if (b.House == this && b.HealthRatio > Rule->ConditionYellow) {
					// BG: Only damage buildings that require power, to keep the
					//     land mines from blowing up under low-power conditions
					if (b.Class->Drain) {
						int damage = 1;
						b.Take_Damage(damage, 0, Rule->C4Warhead, 0);
					}
				}
			}
		}
		DamageTime = int(TICKS_PER_MINUTE * Rule->DamageDelay);
	}

	/*
	**	If there are no more buildings to sell, then automatically cancel the
	**	sell mode.
	*/
	if (PlayerPtr == this && !CurBuildings && Map.IsSellMode) {
		Map.Sell_Mode_Control(0);
	}

	/*
	**	Various base conditions may be announced to the player. Typically, this would be
	**	low tiberium capacity or low power.
	*/
	if (PlayerPtr == this) {

		if (SpeakMaxedDelay == 0 && Available_Money() < 100 && UnitFactories+BuildingFactories+InfantryFactories > 0) {
			Speak(VOX_NO_CASH);
			Map.Flash_Money();
			SpeakMaxedDelay = Options.Normalize_Delay(int(TICKS_PER_MINUTE * Rule->SpeakDelay));
		}

		if (SpeakMaxedDelay == 0/* && IsMaxedOut*/) {
			//IsMaxedOut = false;
			if ((Capacity - Tiberium.Get_Total_Amount()) < 30/*0*/ && Capacity > 50/*0*/ /*&& (ActiveBScan & (STRUCTF_REFINERY | STRUCTF_CONST))*/) {
				Speak(VOX_NEED_MO_CAPACITY);
				SpeakMaxedDelay = Options.Normalize_Delay(int(TICKS_PER_MINUTE * Rule->SpeakDelay));
			}
		}
		if (SpeakPowerDelay == 0 && Power_Fraction() < 1) {
			if (ABQuantity.Value(Rule->BuildConst[0]->HeapID) > 0) {
				Speak(VOX_LOW_POWER);
				SpeakPowerDelay = Options.Normalize_Delay(int(TICKS_PER_MINUTE * Rule->SpeakDelay));
//				Map.Flash_Power();
// RA2: no Add_Message (TibSun's use does not compile against RA2's declaration)
			}
		}
	}

	/*
	**	If there is a flag associated with this house, then mark it to be
	**	redrawn.
	*/
	if (FlagLocation != NULL) {
		UnitClass * unit = FlagLocation->As_UnitClass();
		if (unit) {
			unit->Mark(MARK_CHANGE);
		} else {
			//CELL cell = As_Cell(FlagLocation);
			//Map[cell].Redraw_Objects();
		}
	}

	bool is_time = false;

	/*
	**	Triggers are only checked every so often. If the trigger timer has expired,
	**	then set the trigger processing flag.
	*/
	if (TriggerTime == 0 || IsBuiltSomething) {
		is_time = true;
		TriggerTime = TICKS_PER_MINUTE/10;
		IsBuiltSomething = false;
	}

	/*
	**	Process any super weapon logic required.
	*/
	Super_Weapon_Handler();

	/*
	**	Special win/lose check for multiplayer games; by-passes the
	**	trigger system.  We must wait for non-zero frame, because init
	**	may not properly set IScan etc for each house; you have to go
	**	through each object's AI before it will be properly set.
	*/
	if (Session.Type != GAME_NORMAL && !IsDefeated && Frame > 0 && !Class->IsMultiplayPassive) {
		bool defeated = false;
		if (Session.Options.ShortGame) {
			if (!CurBuildings && !(UQuantity.Value(Rule->BaseUnit[0]->HeapID) + UQuantity.Value(Rule->BaseUnit[1]->HeapID))) { // RA2: BaseUnit is a list of two
				defeated = true;
			}
		} else {
			if (!CurBuildings && !CurAircraft && !CurInfantry) {
				int units = CurUnits;
				if (units == 0) {
					defeated = true;
				}
				if (units && Scen->Special.IsHarvesterImmune) {
					units -= UQuantity.Value(Rule->HarvesterUnit[0]->HeapID);
				}
				if (units <= 0) {
					defeated = true;
				}
			}
		}

		if (defeated) {
			Blowup_All();
			MPlayer_Defeated();
		}
	}

	/*
	**	Try to spring all events attached to this house. The triggers will check
	**	for themselves if they actually need to be sprung or not.
	*/
	for (int index = 0; index < HouseTags.Count(); index++) {
		if (HouseTags[index]->Spring() && index > 0) {
			index--;
			continue;
		}
	}

#if NEVER
	/*
	**	If a radar facility is not present, but the radar is active, then turn the radar off.
	**	The radar also is turned off when the power gets below 100% capacity.
	*/
	if (PlayerPtr == this) {
		bool jammed = Map.Is_Radar_Active();

		/*
		**	Find if there are any radar facilities, and if they're jammed or not
		*/

		if (IsGPSActive) {
			jammed = false;
		} else {
			for (int index = 0; index < Buildings.Count(); index++) {
				BuildingClass * building = Buildings[index];
				if (building && building->House == PlayerPtr) {
					if (*building == STRUCT_RADAR /* || *building == STRUCT_EYE */) {
						if (!building->IsJammed) {
							jammed = false;
							break;
						}
					}
				}
			}
		}

		if (Map.Get_Jammed() != jammed) {
			Map.RadarClass::Flag_To_Redraw(true);
		}
		Map.Set_Jammed(jammed);
// Need to add in here where we activate it when only GPS is active.
		if (Map.Is_Radar_Active()) {
			if (ActiveBScan & STRUCTF_RADAR) {
				if (Power_Fraction() < 1 && !IsGPSActive) {
					Map.Radar_Activate(0);
				}
			} else {
				if (!IsGPSActive) {
					Map.Radar_Activate(0);
				}
			}

		} else {
			if (IsGPSActive || (ActiveBScan & STRUCTF_RADAR)) {
				if (Power_Fraction() >= 1 || IsGPSActive) {
					Map.Radar_Activate(1);
				}
			} else {
				if (Map.Is_Radar_Existing()) {
					Map.Radar_Activate(4);
				}
			}
		}
	}
#endif

	/*
	**	Perform any expert system AI processing.
	*/
	if (/*IsBaseBuilding &&*/ AITimer == 0 && !Is_Human_Player() && !Class->IsMultiplayPassive) {
		AITimer = Expert_AI();
	}

//	if (!IsBaseBuilding && State == STATE_ENDGAME) {
//		Fire_Sale();
//		Do_All_To_Hunt();
//	}

	if (!Is_Human_Player() && !Class->IsMultiplayPassive) {
		if (ProductionMode == EVERYTHING || Session.Type == GAME_NORMAL) {
			AI_Building();
			AI_Unit();
			AI_Infantry();
			AI_Aircraft();
		} else if (ProductionMode == BUILDINGS) {
			AI_Building();
			if (BuildStructure == STRUCT_NONE || !BuildingTypes[BuildStructure]->Who_Can_Build_Me(true, true, true, this)) {
				AI_Unit();
				AI_Infantry();
				AI_Aircraft();
			}
		} else if (ProductionMode == UNITS) {
			AI_Unit();
			if (BuildUnit != Rule->HarvesterUnit[0]->HeapID) {
				AI_Infantry();
				AI_Aircraft();
			}

			bool build_buildings = BuildUnit == UNIT_NONE && BuildInfantry == INFANTRY_NONE && BuildAircraft == AIRCRAFT_NONE;

			if (BuildUnit != UNIT_NONE && !UnitTypes[BuildUnit]->Who_Can_Build_Me(true, true, true, this)) {
				build_buildings = true;
			}
			if (BuildInfantry != INFANTRY_NONE && !InfantryTypes[BuildInfantry]->Who_Can_Build_Me(true, true, true, this)) {
				build_buildings = true;
			}
			if (BuildAircraft != AIRCRAFT_NONE && !AircraftTypes[BuildAircraft]->Who_Can_Build_Me(true, true, true, this)) {
				build_buildings = true;
			}

			if (build_buildings) {
				AI_Building();
			}
		}
	}

	/*
	**	If the production possibilities need to be recalculated, then do so now. This must
	**	occur after the scan bits have been properly updated.
	*/
	bool recalc_supers = false;
	if (PlayerPtr == this) {
		if (IsRecalcNeeded) {
			recalc_supers = true;
			IsRecalcNeeded = false;
			Map.Recalc();
			Map.SidebarClass::IsToRedraw = true;
			Map.IsToBlitSidebar = true;
			Map.Column[0].Flag_To_Redraw();
			Map.Column[1].Flag_To_Redraw();

			/*
			**	This placement might affect any prerequisite requirements for construction
			**	lists. Update the buildable options accordingly.
			*/
			for (int index = 0; index < Buildings.Count(); index++) {
				BuildingClass * building = Buildings[index];
				if (building && building->IsActive && building->Strength > 0 && building->House == this && building->Mission != MISSION_DECONSTRUCTION && building->MissionQueue != MISSION_DECONSTRUCTION) {

					if (PlayerPtr == building->House) {
						building->Update_Buildables();
					}
				}
			}
		}
	} else {
		if (IsRecalcNeeded) {
			IsRecalcNeeded = false;
			recalc_supers = true;
		}
	}

	if (recalc_supers) {
		Update_Present_Super_Weapons();
		Enable_Available_Super_Weapons();
	}

	/*
	**	See if it's time to re-set the can-repair flag
	*/
	if (DidRepair && RepairTimer == 0) {
		DidRepair = false;
	}

	if (this == PlayerPtr && IsToLook) {
		IsToLook = false;
		Map.All_To_Look();
	}
}
#endif


/***********************************************************************************************
 * HouseClass::Super_Weapon_Handler -- Handles the super weapon charge and discharge logic.    *
 *                                                                                             *
 *    This handles any super weapons assigned to this house. It also performs any necessary    *
 *    maintenance that the super weapons require.                                              *
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
void HouseClass::Super_Weapon_Handler(void)
{
	/*
	**	Perform all super weapon AI processing. This just checks to see if
	**	the graphic needs changing for the special weapon and updates the
	**	sidebar as necessary.
	*/
	for (int index = 0; index < SuperWeapon.Count(); index++) { // RA2: RA2's form
		SuperClass *super = (SuperClass *)SuperWeapon[index]; // RA2: as above
		if (super->Is_Present() && super->AI(this == PlayerPtr) && // RA2: as above
			this == PlayerPtr) { // RA2: as above
			int id = super->Class->Fetch_Heap_ID(); // RA2: as above
			Map.Flag_Tab_To_Redraw(SidebarClass::Which_Tab(RTTI_SUPERWEAPON, id, false)); // RA2: as above
		}
	}
}




/***********************************************************************************************
 * HouseClass::Attacked -- Lets player know if base is under attack.                           *
 *                                                                                             *
 *    Call this function whenever a building is attacked (with malice). This function will     *
 *    then announce to the player that his base is under attack. It checks to make sure that   *
 *    this is referring to the player's house rather than the enemy's.                         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/27/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::Attacked(BuildingClass * source)
{
	if (source != NULL && source->Considered_Vehicle()) return;

	bool is_player = Is_Player_Control(); // RA2: both tests are made first
	bool is_ally = PlayerPtr != NULL && PlayerPtr != this && Is_Ally(PlayerPtr) && (Session.Type == GAME_NORMAL || !source->House->Class->IsMultiplayPassive); // RA2: as above

	if (is_player) { // RA2: no SpeakAttackDelay; the radar event decides whether EVA speaks
		if (Submit_Radar_Event(RADAREVENT_BASE_ATTACKED, source->Center_Coord().As_Cell())) { // RA2: only a radar event that was accepted is announced
			Queue_Speech("EVA_OurBaseIsUnderAttack", -1, -1); // RA2: EVA speaks by name, then the alarm sound plays
			VocClass::PlayGlobal(Rule->BaseUnderAttackSound, 1.0f, 0x2000); // RA2: as above
		}
	} else if (is_ally) { // RA2: as above
		if (Submit_Radar_Event(RADAREVENT_ALLY_BASE_ATTACKED, source->Center_Coord().As_Cell())) { // RA2: the radar event of an attacked ally is 16
			Queue_Speech("EVA_OurAllyIsUnderAttack", -1, -1); // RA2: as above
			VocClass::PlayGlobal(Rule->BaseUnderAttackSound, 1.0f, 0x2000); // RA2: as above
		}
	}

	/*
	**	If there is a trigger event associated with being attacked, process it
	**	now.
	*/
	for (int index = 0; index < HouseTags.Count(); index++) {
		HouseTags[index]->Spring(TEVENT_ATTACKED);
	}
}




/***********************************************************************************************
 * HouseClass::Harvested -- Adds Tiberium to the harvest storage.                              *
 *                                                                                             *
 *    Use this routine whenever Tiberium is harvested. The Tiberium is stored equally between  *
 *    all storage capable buildings for the house. Harvested Tiberium adds to the credit       *
 *    value of the house, but only up to the maximum storage capacity that the house can       *
 *    currently maintain.                                                                      *
 *                                                                                             *
 * INPUT:   tiberium -- The number of Tiberium credits to add to the House's total.            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/25/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
float HouseClass::Give_Tiberium(float amount, int slot) // RA2: 0x4E5100, was TibSun's Harvested(int, TiberiumType); the credits are paid out at once, the storage is not filled
{
	PointTotal = (int)(amount * 5.0f + PointTotal); // RA2: the score counts five points a unit, as a float
	Credits = (int)(Tiberiums[slot]->CreditValue * Class->IncomeMult * amount + Credits); // RA2: the house type's income multiplier scales the credits
	return(amount); // RA2: as above
}



/// <summary>
/// Finds the building of this house that has the most room left for tiberium.
/// </summary>
/// <returns>Returns with the building, or NULL if the house has no building that can store tiberium.</returns>
/// <summary>
/// Finds the building of this house that has the most room left for tiberium.
/// </summary>
/// <returns>Returns with the building, or NULL if the house has no building that can store tiberium.</returns>
BuildingClass * HouseClass::Emptiest_Storage_Building(void) // RA2: 0x4E5160; not in TibSun
{
	BuildingClass * best = NULL; // RA2: as above
	float best_room = 0; // RA2: as above
	int count = Buildings.Count(); // RA2: as above
	for (int index = 0; index < count; index++) { // RA2: as above
		BuildingClass * building = Buildings[index]; // RA2: as above
		if (building != NULL && building->IsDown) { // RA2: as above
			int capacity = building->Class->Capacity; // RA2: as above
			if (capacity > 0) { // RA2: as above
				float room = (float)capacity - building->Storage.Get_Total_Amount(); // RA2: as above
				if (best == NULL || room > best_room) { // RA2: as above
					best_room = room; // RA2: as above
					best = building; // RA2: as above
				}
			}
		}
	}
	return(best); // RA2: as above
}



/// <summary>
/// Adds harvested weed to this house's storage.
/// This routine is called when a weed eater unloads. Anything that will not fit within the
/// house's weed capacity is simply thrown away.
/// </summary>
/// <param name="weed">The amount of weed harvested.</param>
/// <param name="slot">The storage slot to credit the weed to.</param>
void HouseClass::Harvested_Weed(int weed, int slot)
{
	int i = weed;
	while (i > 0) {
		if (Rule->WeedCapacity <= Weed.Get_Total_Amount()) {
			break;
		}
		Weed.Increase_Amount(1, slot);
		i--;
	}
}


/// <summary>
/// Calculates how full this house's weed storage is.
/// This routine is used when the weed storage gauge is drawn.
/// </summary>
/// <returns>Returns with the fraction of the weed storage in use, where 1.0 means
/// full.</returns>
double HouseClass::Weed_Fraction(void) const
{
	if (Weed.Get_Total_Amount() == 0) {
		return(0.f);
	}
	return((double)Weed.Get_Total_Amount() / Rule->WeedCapacity);
}


/***********************************************************************************************
 * HouseClass::Spend_Money -- Removes money from the house.                                    *
 *                                                                                             *
 *    Use this routine to extract money from the house. Typically, this is a result of         *
 *    production spending. The money is extracted from available cash reserves first. When     *
 *    cash reserves are exhausted, then Tiberium is consumed.                                  *
 *                                                                                             *
 * INPUT:   money -- The amount of money to spend.                                             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/25/1995 JLB : Created.                                                                 *
 *   06/20/1995 JLB : Spends Tiberium before spending cash.                                    *
 *=============================================================================================*/
void HouseClass::Spend_Money(int money)
{
	float old_amount = Tiberium.Get_Total_Amount(); // RA2: RA2's form
	int old_capacity = Capacity; // RA2: as above
	int spent; // RA2: as above
	int remaining = money; // RA2: as above
	if (money > Credits) {
		remaining -= Credits; // RA2: as above
		spent = Credits; // RA2: as above
		Credits = 0;
	} else {
		Credits -= money;
		spent = money; // RA2: as above
		remaining = 0; // RA2: as above
	}
	if (remaining > 0 && Tiberium.Get_Total_Amount() > 0.0f) { // RA2: as above
		int count = Buildings.Count(); // RA2: as above
		for (int index = 0; index < count; ++index) { // RA2: as above
			TechnoClass *building = (TechnoClass *)Buildings[index]; // RA2: as above
			if (building) { // RA2: as above
				StorageClass &storage = building->Storage; // RA2: as above
				while (storage.Get_Total_Amount() > 0.0f && remaining > 0) { // RA2: as above
					int type = storage.First_Used_Slot(); // RA2: as above
					while (remaining > 0 && storage.Get_Amount(type) > 0.0f) { // RA2: as above
						float removed = storage.Decrease_Amount(1.0f, type); // RA2: as above
						Tiberium.Decrease_Amount(removed, type); // RA2: as above
						int value = (int)(Tiberiums[type]->CreditValue * // RA2: as above
							Class->IncomeMult * removed); // RA2: as above
						remaining -= value; // RA2: as above
						spent += value; // RA2: as above
						if (remaining < 0) { // RA2: as above
							Credits -= remaining; // RA2: as above
							spent += remaining; // RA2: as above
							remaining = 0; // RA2: as above
							break; // RA2: as above
						}
						if (remaining <= 0) break; // RA2: as above
					}
				}
			}
			if (!remaining) break; // RA2: as above
		}
	}
	Silo_Redraw_Check((int)old_amount, old_capacity); // RA2: as above
	CreditsSpent += spent; // RA2: as above
}



/***********************************************************************************************
 * HouseClass::Refund_Money -- Refunds money to back to the house.                             *
 *                                                                                             *
 *    Use this routine when money needs to be refunded back to the house. This can occur when  *
 *    construction is aborted. At this point, the exact breakdown of Tiberium or initial       *
 *    credits used for the orignal purchase is lost. Presume as much of the money is in the    *
 *    form of Tiberium as storage capacity will allow.                                         *
 *                                                                                             *
 * INPUT:   money -- The number of credits to refund back to the house.                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/25/1995 JLB : Created.                                                                 *
 *   06/01/1995 JLB : Refunded money is never lost                                             *
 *=============================================================================================*/
void HouseClass::Refund_Money(int money)
{
	Credits += money;
}


/***********************************************************************************************
 * HouseClass::Silo_Redraw_Check -- Flags silos to be redrawn if necessary.                    *
 *                                                                                             *
 *    Call this routine when either the capacity or tiberium levels change for a house. This   *
 *    routine will determine if the aggregate tiberium storage level will result in the        *
 *    silos changing their imagery. If this is detected, then all the silos for this house     *
 *    are flagged to be redrawn.                                                               *
 *                                                                                             *
 * INPUT:   oldtib   -- Pre-change tiberium level.                                             *
 *                                                                                             *
 *          oldcap   -- Pre-change tiberium storage capacity.                                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/02/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::Silo_Redraw_Check(int oldtib, int oldcap)
{
	int old_level = 0; // RA2: RA2's form
	if (oldcap != 0) { // RA2: as above
		old_level = (int)((double)oldtib / oldcap * 4.0 + 0.5); // RA2: as above
	}
	int level = 0; // RA2: as above
	if (Capacity != 0) { // RA2: as above
		level = (int)(Tiberium.Get_Total_Amount() / Capacity * 4.0 + 0.5); // RA2: as above
	}
	if (old_level != level) { // RA2: as above
		int count = Buildings.Count(); // RA2: as above
		for (int index = 0; index < count; index++) { // RA2: as above
			BuildingClass *building = Buildings[index]; // RA2: as above
			if (building != 0 && !building->IsInLimbo && // RA2: as above
				building->Class->IsSiloDamage) { // RA2: as above
				building->Mark(MARK_CHANGE); // RA2: as above
			}
		}
	}
}



/***********************************************************************************************
 * HouseClass::Is_Ally -- Determines if the specified house is an ally.                        *
 *                                                                                             *
 *    This routine will determine if the house number specified is a ally to this house.       *
 *                                                                                             *
 * INPUT:   house -- The house number to check to see if it is an ally.                        *
 *                                                                                             *
 * OUTPUT:  Is the house an ally?                                                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool HouseClass::Is_Ally(HousesType house) const
{
	if (house == HeapID) return(true);
	if (house != HOUSE_NONE) {
		return(((1<<house) & Allies) != 0);
	}
	return(false);
}


/***********************************************************************************************
 * HouseClass::Is_Ally -- Determines if the specified house is an ally.                        *
 *                                                                                             *
 *    This routine will examine the specified house and determine if it is an ally.            *
 *                                                                                             *
 * INPUT:   house -- Pointer to the house object to check for ally relationship.               *
 *                                                                                             *
 * OUTPUT:  Is the specified house an ally?                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool HouseClass::Is_Ally(HouseClass const * house) const
{
	if (house) {
		if (house == this) {
			return(true);
		}
		return(Is_Ally(house->HeapID));
	}

	return(false);
}


/***********************************************************************************************
 * HouseClass::Is_Ally -- Checks to see if the object is an ally.                              *
 *                                                                                             *
 *    This routine will examine the specified object and return whether it is an ally or not.  *
 *                                                                                             *
 * INPUT:   object   -- The object to examine to see if it is an ally.                         *
 *                                                                                             *
 * OUTPUT:  Is the specified object an ally?                                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool HouseClass::Is_Ally(ObjectClass const * object) const
{
	if (object) {
		return(Is_Ally(object->Owner_HouseClass()));
	}
	return(false);
}


/// <summary>
/// Is the target specified an ally of this house?
/// This overload is the catch-all for targets whose kind is not known at the call site.
/// Anything that is not an object cannot be allied with, and so is treated as hostile.
/// </summary>
/// <returns>bool; Is the target friendly toward this house?</returns>
bool HouseClass::Is_Ally(AbstractClass const * target) const
{
	if (target != NULL && (target->AbstractFlags & 2)) { // RA2: only objects (flag 2) have an owner; the flag replaces dynamic_cast
		return(Is_Ally(((ObjectClass const *)target)->Owner_HouseClass())); // RA2: as above
	}

	return(false);
}



/***********************************************************************************************
 * HouseClass::Make_Ally -- Make the specified house an ally.                                  *
 *                                                                                             *
 *    This routine will make the specified house an ally to this house. An allied house will   *
 *    not be considered a threat or potential target.                                          *
 *                                                                                             *
 * INPUT:   house -- The house to make an ally of this house.                                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *   08/08/1995 JLB : Breaks off combat when ally commences.                                   *
 *   10/17/1995 JLB : Added reveal base when allied.                                           *
 *=============================================================================================*/
void HouseClass::Make_Ally(HousesType house, bool announce) // RA2: 0x4E5640 forwards the announce flag
{
	Make_Ally(Houses[house], announce); // RA2: as above
}



/// <summary>
/// Makes an ally of the house specified.
/// An allied house is no longer a threat or a legal target, so any fighting already under way
/// between the two is broken off as the alliance forms. Allying with the local player may
/// also lay this house's structures bare to them.
/// </summary>
/// <param name="house">The house to make an ally of this house.</param>
void HouseClass::Make_Ally(HouseClass * house, bool announce) // RA2: the executable's form has the second argument (not used by TibSun's body)
{
	if (Is_Allowed_To_Ally(house)) {
		Map.Update_Technos_Visibility(false, false); // RA2: the visibility of every techno is refreshed before and after (Reveal_Technos at the end)

		Allies |= (1L << house->HeapID);

		Recalc_Threat_Regions();
		Clear_Anger(house);

		/*
		**	Don't consider the newfound ally to be an enemy -- of course.
		*/
		if (Enemy == house->HeapID) {
			Enemy = HOUSE_NONE;
		}

		if (ScenarioInit) {
			Control.Allies |= (1L << house->HeapID);
		}

		if (Session.Type == GAME_INTERNET || !ScenarioInit) {

			/*
			**	An alliance with another human player will cause the computer
			**	players (if present) to become paranoid.
			*/
			if (Is_Human_Player() && Rule->IsComputerParanoid && !house->Class->IsMultiplayPassive && Session.Type != GAME_NORMAL) { // RA2: only outside the campaign
				Computer_Paranoid();
			}

			/*
			**	Sweep through all techno objects and perform a cheeseball tarcom clear to ensure
			**	that fighting will most likely stop when the cease fire begins.
			*/
			for (int index = 0; index < Logic.Count(); index++) {
				ObjectClass * object = Logic[index];

				if (object != NULL && Dynamic_Cast<TechnoClass *>(object) && !object->IsInLimbo && object->Owner() == HeapID) {
					TargetClass target = ((TechnoClass *)object)->TarCom;
					if (target.Is_Valid() && target.As_Techno() != NULL) {
						if (Is_Ally(target.As_Techno())) {
							((TechnoClass *)object)->Assign_Target(NULL);
						}
					}
				}
			}

			if (Is_Human_Player() && Session.Type != GAME_NORMAL && !house->Class->IsMultiplayPassive) {
				wchar_t buffer[80]; // RA2: the message is a wide string
				swprintf(buffer, StringTable::LoadString("TXT_HAS_ALLIED", 0, "D:\\Ra2\\House.CPP", 3149), UIName, house->UIName); // RA2: both house names are the wide UI names
				Session.Messages.Add_Message(NULL, 0, buffer, Class->Scheme, TextPrintType(0x4046), int(Rule->MessageDelay * TICKS_PER_MINUTE), false); // RA2: as above

				if (announce) { // RA2: EVA announces the alliance, or the request for one
					if (house->Is_Ally(this)) { // RA2: as above
						if (!MuteSWLaunches) { // RA2: as above
							if (PlayerPtr->Is_Ally(house) && PlayerPtr->Is_Ally(this)) { // RA2: as above
								Queue_Speech("EVA_AllianceFormed", -1, -1); // RA2: as above
							} else { // RA2: as above
								Queue_Speech("EVA_EnemyAllianceFormed", -1, -1); // RA2: as above
							}
						}
					} else if (Is_Player_Control()) { // RA2: as above
						Queue_Speech("EVA_RequestingAlliance", -1, -1); // RA2: as above
					} else if (house->Is_Player_Control()) { // RA2: as above
						Queue_Speech("EVA_AllianceRequested", -1, -1); // RA2: as above
					}
				}
			}

			Map.Flag_To_Redraw();
		}
		Map.Reveal_Technos(false, false); // RA2: as above
	}
}




/***********************************************************************************************
 * HouseClass::Make_Enemy -- Make an enemy of the house specified.                             *
 *                                                                                             *
 *    This routine will flag the house specified so that it will be an enemy to this house.    *
 *    Enemy houses are legal targets for attack.                                               *
 *                                                                                             *
 * INPUT:   house -- The house to make an enemy of this house.                                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *   07/27/1995 JLB : Making war is a bilateral action.                                        *
 *=============================================================================================*/
void HouseClass::Make_Enemy(HousesType house, bool announce) // RA2: 0x4E5A50 forwards the announce flag
{
	Make_Enemy(Houses[house], announce); // RA2: as above
}



/// <summary>
/// Makes an enemy of the house specified.
/// This routine breaks any alliance between the two houses and stokes this house's anger
/// toward the other. Breaking an alliance is a bilateral event, so the other house is drawn
/// out of it as well.
/// </summary>
/// <param name="house">The house to make an enemy of this house.</param>
void HouseClass::Make_Enemy(HouseClass * house, bool announce) // RA2: 0x4E5A70, takes the announce flag (not used by TibSun's body)
{
	bool broken = false; // RA2: set when the alliance was broken in both directions; the announcement needs it

	if (Session.Type == GAME_NORMAL || (!house->Class->IsMultiplayPassive && !Class->IsMultiplayPassive)) {
		Map.Update_Technos_Visibility(false, false); // RA2: the visibility of every techno is refreshed before and after (Reveal_Technos at the end)
		Add_Anger(1, house);

		if (house != NULL && Is_Ally(house)) {
			Allies &= ~(1L << house->HeapID);

			if (ScenarioInit) {
				Control.Allies &= !(1L << house->HeapID);
			}

			Recalc_Threat_Regions();

			/*
			**	Breaking an alliance is a bilateral event.
			*/
			if (house->Is_Ally(this)) {
				house->Allies &= ~(1L << HeapID);
				broken = true; // RA2: as above

				if (ScenarioInit) {
					Control.Allies &= ~(1L << HeapID);
				}
				house->Recalc_Threat_Regions();
				house->Add_Anger(1, house);
			}

			if (Session.Type != GAME_NORMAL && !ScenarioInit && IsHuman) {
				wchar_t buffer[80]; // RA2: the message is a wide string
				swprintf(buffer, StringTable::LoadString("TXT_AT_WAR", 0, "D:\\Ra2\\House.CPP", 3272), UIName, house->UIName); // RA2: both house names are the wide UI names
				Session.Messages.Add_Message(NULL, 0, buffer, Class->Scheme, TextPrintType(0x4046), int(Rule->MessageDelay * TICKS_PER_MINUTE), false); // RA2: as above
				Map.Flag_To_Redraw();
				if (announce && broken && (Is_Player_Control() || house->Is_Player_Control())) { // RA2: EVA announces the broken alliance when it concerns a player's house; part of the message block
					Queue_Speech("EVA_AllianceBroken", -1, -1); // RA2: as above
				}
			}
		}
		Map.Reveal_Technos(false, false); // RA2: as above
	}
}



/***********************************************************************************************
 * HouseClass::Suggested_New_Team -- Determine what team should be created.                    *
 *                                                                                             *
 *    This routine examines the house condition and returns with the team that it thinks       *
 *    should be created. The units that are not currently a member of a team are examined      *
 *    to determine the team needed.                                                            *
 *                                                                                             *
 * INPUT:   alertcheck  -- Select from the auto-create team list.                              *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the team type that should be created. If no team should  *
 *          be created, then NULL is returned.                                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
SUGGESTED_TEAM_LIST HouseClass::Suggested_New_Team(bool alertcheck)
{
	SUGGESTED_TEAM_LIST teams = TeamTypeClass::Suggested_New_Team(this, alertcheck);
	return(teams);
}


/***********************************************************************************************
 * HouseClass::Adjust_Threat -- Adjust threat for the region specified.                        *
 *                                                                                             *
 *    This routine is called when the threat rating for a region needs to change. The region   *
 *    and threat adjustment are provided.                                                      *
 *                                                                                             *
 * INPUT:   region   -- The region that adjustment is to occur on.                             *
 *                                                                                             *
 *          threat   -- The threat adjustment to perform.                                      *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::Adjust_Threat(int region, int threat)
{
	static int _val[] = {
		-MAP_REGION_WIDTH - 1,	-MAP_REGION_WIDTH, -MAP_REGION_WIDTH + 1,
		-1,							0,						 1,
		MAP_REGION_WIDTH -1,		MAP_REGION_WIDTH,	 MAP_REGION_WIDTH +1
	};
	static int _thr[] = {
		2, 1, 2,
		1, 0, 1,
		2, 1,	2
	};
	int neg;
	int * val = &_val[0];
	int * thr = &_thr[0];

	if (threat < 0) {
		threat = -threat;
		neg = true;
	} else {
		neg = false;
	}

	for (int lp = 0; lp < 9; lp ++) {
		Regions[region + *val].Adjust_Threat(threat >> *thr, neg);
		val++;
		thr++;
	}
}


/***********************************************************************************************
 * HouseClass::Begin_Production -- Starts production of the specified object type.             *
 *                                                                                             *
 *    This routine is called from the event system. It will start production for the object    *
 *    type specified. This will be reflected in the sidebar as well as the house factory       *
 *    tracking variables.                                                                      *
 *                                                                                             *
 * INPUT:   type  -- The type of object to begin production on.                                *
 *                                                                                             *
 *          id    -- The subtype of object.                                                    *
 *                                                                                             *
 * OUTPUT:  Returns with the reason why, or why not, production was started.                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *   10/21/1996 JLB : Handles max object case.                                                 *
 *=============================================================================================*/
ProdFailType HouseClass::Begin_Production(RTTIType type, int id, bool naval, bool resume) // RA2: naval
{
	int result = true;
	FactoryClass * fptr;
	int category = Build_Category(type, id); // RA2: the factory is found by kind, ship flag and build category
	bool was_pending = Is_Production_Pending(type, naval, category); // RA2: the pending state is remembered, then cleared
	Assign_Production_Pending(type, naval, category, false); // RA2: as above
	TechnoTypeClass const * tech = Fetch_Techno_Type(type, id);

	BuildingClass *who = tech->Who_Can_Build_Me(false, true, true, this);
	bool onhold = false;

	if (who == NULL) {
		if (resume) {
			who = tech->Who_Can_Build_Me(true, false, true, this);
		}
		if (who != NULL) {
			onhold = true;
		} else {
			DebugString("Request to Begin_Production of '%s' was rejected. No-one can build.\n", (const char *)tech->GivenName);
			return(PROD_CANT);
		}
	}

	fptr = Fetch_Factory(type, naval, category); // RA2: as above

	if (fptr == NULL) {
		fptr = new FactoryClass;

		if (fptr == NULL) {
			DebugString("Request to Begin_Production of '%s' was rejected. Unable to create factory\n", (const char *)tech->GivenName);
			return(PROD_CANT);
		}
	}

	/*
	**	If the house is already busy producing the requested object, then
	**	return with this failure code, unless we are restarting production.
	*/
	if (fptr->Is_Building() && type == RTTI_BUILDINGTYPE) {
		DebugString("Request to Begin_Production of '%s' was rejected. Cannot queue buildings.\n", (const char *)tech->GivenName);
		return(PROD_CANT);
	}

	Set_Factory(fptr, type, naval, category); // RA2: as above

	bool has_suspended = false;
	if (fptr->IsSuspended) {
		TechnoClass *object = fptr->Object;
		if (object != NULL) {
			if (object->Techno_Type_Class() == tech) {
				has_suspended = true;
			}
		}
	}

	if (!has_suspended) {
		result = fptr->Set(*tech, *this, resume);
	}

	if (result) {
		if (fptr->QueuedObjects.Count() && !resume && !has_suspended) {
			Map.Flag_Tab_To_Redraw(SidebarClass::Which_Tab(type, id, false)); // RA2: only the tab of the kind is redrawn
		} else {
			bool suspended = fptr->IsSuspended; // RA2: a human player's factory that was suspended resumes where the pending production left off
			fptr->Start(onhold);

			if (!suspended && Session.Type != GAME_NORMAL && IsHuman) { // RA2: as above
				int duration = fptr->Object->TClass->Time_To_Build(); // RA2: as above
				int stage = (int)(((unsigned)Session.DesiredFrameRate * (unsigned)Session.MaxAhead / 60) * 54) / duration; // RA2: as above
				if (stage >= 53) { // RA2: as above
					stage = 53; // RA2: as above
				}
				if (was_pending) { // RA2: as above
					fptr->Set_Stage(stage); // RA2: as above
				}
			}

			/*
			**	Link this factory to the sidebar so that proper graphic feedback
			**	can take place.
			*/
			if (PlayerPtr == this) {
				Map.Factory_Link(fptr, type, id);
			}
		}

		return(PROD_OK);
	}

	DebugString("Request to Begin_Production of '%s' was rejected. Factory was unable to create the requested object\n", (char const *)tech->GivenName);

	if (fptr->QueuedObjects.Count() == 0 && fptr->Object == NULL) {
		DebugString("type=%d\n", type);
		DebugString("Frame == %d\n", Frame);
		DebugString("fptr->QueuedObjects.Count() == %d\n", fptr->QueuedObjects.Count());
		DebugString("Object->RTTI == %d\n", fptr->Object != NULL ? fptr->Object->Fetch_RTTI() : -1);
		DebugString("Object->HeapID == %d\n", fptr->Object != NULL ? fptr->Object->Fetch_Heap_ID() : -1);
		DebugString("IsSuspended\t= %d\n", fptr->IsSuspended);

		delete fptr;
		Set_Factory(NULL, type, naval, category); // RA2: as above
	}

	return(PROD_CANT);
}



/***********************************************************************************************
 * HouseClass::Suspend_Production -- Temporarily puts production on hold.                      *
 *                                                                                             *
 *    This routine is called from the event system whenever the production of the specified    *
 *    type needs to be suspended. The suspended production will be reflected in the sidebar    *
 *    as well as in the house control structure.                                               *
 *                                                                                             *
 * INPUT:   type  -- The type of object that production is being suspended for.                *
 *                                                                                             *
 * OUTPUT:  Returns why, or why not, production was suspended.                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ProdFailType HouseClass::Suspend_Production(RTTIType type, int id, bool naval) // RA2: the factory is found by kind, ship flag and build category
{
	FactoryClass * fptr = Fetch_Factory(type, naval, Build_Category(type, id));

	/*
	**	If the house is already busy producing the requested object, then
	**	return with this failure code.
	*/
	if (fptr == NULL) return(PROD_CANT);

	/*
	**	Actually suspend the production.
	*/
	fptr->Suspend();

	/*
	**	Tell the sidebar that it needs to be redrawn because of this.
	*/
	if (PlayerPtr == this) {
		Map.Flag_Tab_To_Redraw(SidebarClass::Which_Tab(type, id, false)); // RA2: only the tab of the kind is redrawn
	}

	return(PROD_OK);
}


/***********************************************************************************************
 * HouseClass::Abandon_Production -- Abandons production of item type specified.               *
 *                                                                                             *
 *    This routine is called from the event system whenever production must be abandoned for   *
 *    the type specified. This will remove the factory and pending object from the sidebar as  *
 *    well as from the house factory record.                                                   *
 *                                                                                             *
 * INPUT:   type  -- The object type that production is being suspended for.                   *
 *                                                                                             *
 * OUTPUT:  Returns the reason why or why not, production was suspended.                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ProdFailType HouseClass::Abandon_Production(RTTIType type, int id, bool naval) // RA2: naval
{
	int category = Build_Category(type, id); // RA2: the factory is found by kind, ship flag and build category
	FactoryClass * fptr = Fetch_Factory(type, naval, category); // RA2: as above

	/*
	**	If there is no factory to abandon, then return with a failure code.
	*/
	if (fptr == NULL) {
		return(PROD_CANT);
	}

	if (fptr->QueuedObjects.Count() > 0 && id >= 0 && category == 0) { // RA2: only objects without a build category are taken out of the queue
		TechnoTypeClass const * tech = Fetch_Techno_Type(type, id);
		if (fptr->Remove_From_Queue(tech)) {
			Map.Flag_Tab_To_Redraw(SidebarClass::Which_Tab(type, id, false)); // RA2: only the tab of the kind is redrawn
			return(PROD_OK);
		}
	}

	if (category == 0 && id != -1) { // RA2: the abandoned object must be the one in production
		if (fptr->Object == NULL || id != fptr->Object->Class_Of()->Fetch_Heap_ID()) { // RA2: as above
			return(PROD_OK); // RA2: as above
		}
	}

	/*
	**	Tell the sidebar that it needs to be redrawn because of this.
	*/
	if (PlayerPtr == this) {
		Map.Abandon_Production(type, id, fptr); // RA2: the sidebar takes the object id too

		if (type == RTTI_BUILDINGTYPE || type == RTTI_BUILDING) {
			Map.PendingObjectPtr = 0;
			Map.PendingObject = 0;
			Map.PendingHouse = HOUSE_NONE;
			Map.Set_Cursor_Shape(0);
		}
	}

	/*
	**	Abandon production of the object.
	*/
	fptr->Abandon();
	if (fptr->QueuedObjects.Count() == 0) {
		Set_Factory(NULL, type, naval, category); // RA2: as above
		delete fptr;
	} else {
		fptr->Resume_Queue();
	}

	return(PROD_OK);
}



/***********************************************************************************************
 * HouseClass::Special_Weapon_AI -- Fires special weapon.                                      *
 *                                                                                             *
 *    This routine will pick a good target to fire the special weapon specified.               *
 *                                                                                             *
 * INPUT:   id -- The special weapon id to fire.                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/24/1995 PWG : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::Special_Weapon_AI(SuperWeaponType id)
{
	/*
	**	Loop through all of the building objects on the map
	**	and see which ones are available.
	*/
	BuildingClass * bestptr = NULL;
	int best = -1;

	for (int index = 0; index < ::Buildings.Count(); index++) { // RA2: as below, the global building heap
		BuildingClass * b = ::Buildings[index]; // RA2: the loop reads the global building heap, not HouseClass::Buildings

		/*
		**	If the building is valid, not in limbo, not in the process of
		**	being destroyed and not our ally, then we can consider it.
		*/
		if (b != NULL && !b->IsInLimbo && b->Strength && !Is_Ally(b)) {
			if (Random_Pick(0, 99) < 90 && (b->Value() > best || best == -1)) { // RA2: a Random_Pick(0, 99) below 90, not Percent_Chance
				best = b->Value();
				bestptr = b;
			}
		}
	}

	if (bestptr) {
		Cell cell = bestptr->Center_Coord().As_Cell();
		Place_Special_Blast(id, cell);
	}
}



/***********************************************************************************************
 * HouseClass::Place_Special_Blast -- Place a special blast effect at location specified.      *
 *                                                                                             *
 *    This routine will create a blast effect at the cell specified. This is the result of     *
 *    the special weapons.                                                                     *
 *                                                                                             *
 * INPUT:   id    -- The special weapon id number.                                             *
 *                                                                                             *
 *          cell  -- The location where the special weapon attack is to occur.                 *
 *                                                                                             *
 * OUTPUT:  Was the special weapon successfully fired at the location specified?               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/18/1995 JLB : commented.                                                               *
 *   07/25/1995 JLB : Added scatter effect for nuclear bomb.                                   *
 *   07/28/1995 JLB : Revamped to use super weapon class controller.                           *
 *=============================================================================================*/
bool HouseClass::Place_Special_Blast(SuperWeaponType id, Cell const & cell)
{
	SuperClass *super = (SuperClass *)SuperWeapon[id]; // RA2: RA2's form
	SuperClass *linked = 0; // RA2: as above
	if (super->Class->PostClick) { // RA2: as above
		linked = (SuperClass *)SuperWeapon[super->Class->PreDependent]; // RA2: as above
		if (linked != 0) { // RA2: as above
			super->ChronoMapCoords = linked->ChronoMapCoords; // RA2: as above
		}
	}
	super->Discharged(this == PlayerPtr, cell); // RA2: as above
	if (super->Class->PostClick) { // RA2: nested ifs; the larger source size keeps this function out of line at every call site
		if (linked != 0) { // RA2: as above
			linked->Set_Ready(0); // RA2: as above
			linked->Recharge(this == PlayerPtr); // RA2: as above
		}
	}
	return(true);
}



/***********************************************************************************************
 * HouseClass::Place_Object -- Places the object (building) at location specified.             *
 *                                                                                             *
 *    This routine is called when a building has been produced and now must be placed on       *
 *    the map. When the player clicks on the map, this routine is ultimately called when the   *
 *    event passes through the event queue system.                                             *
 *                                                                                             *
 * INPUT:   type  -- The object type to place. The actual object is lifted from the sidebar.   *
 *                                                                                             *
 *                                                                                             *
 *          cell  -- The location to place the object on the map.                              *
 *                                                                                             *
 * OUTPUT:  Was the placement successful?                                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool HouseClass::Place_Object(RTTIType type, int id, bool naval, Cell const & cell) // RA2: the factory is found by kind, ship flag and build category
{
	bool placed = false;
	TechnoClass * tech = 0;
	int category = Build_Category(type, id);
	FactoryClass * factory = Fetch_Factory(type, naval, category);

	/*
	**	Only if there is a factory active for this type, can it be "placed".
	**	In the case of a missing factory, then this request is completely bogus --
	**	ignore it. This might occur if, between two events to exit the same
	**	object, the mouse was clicked on the sidebar to start building again.
	**	The second placement event should NOT try to place the object that is
	**	just starting construction.
	*/
	if (factory && factory->Has_Completed()) {
		tech = factory->Get_Object();

		if (cell == CELL_NONE || cell == Cell(-1, -1)) {
			if (tech != NULL) {
				BuildingClass * builder = tech->Who_Can_Build_Me(false, true);
				if (builder == NULL && tech->Fetch_RTTI() == RTTI_AIRCRAFT) {
					builder = tech->Who_Can_Build_Me(true, true);
				}

				if (builder != NULL) {
					int exit = builder->Exit_Object(tech);
					if (exit == 2 || (exit == 1 && builder->Fetch_RTTI() == RTTI_BUILDING && builder->Factory != NULL)) {
						if (Is_Player_Control()) { // RA2: the player hears the unit is ready
							if (Submit_Radar_Event(RADAREVENT_UNIT_PRODUCED, builder->Center_Coord().As_Cell())) {
								Queue_Speech("EVA_UnitReady", -1, -1);
							}
						}

						/*
						**	Since the object has left the factory under its own power, delete
						**	the production manager tied to this slot in the sidebar. Its job
						**	has been completed.
						*/
						factory->Completed();
						Abandon_Production(type, id, naval);
						placed = true;
					} else {
						/*
						**	The object could not leave under it's own power. Just wait
						**	until the player tries to place the object again.
						*/
						if (tech->Fetch_RTTI() != RTTI_BUILDING) {
							DebugString("Failed to exit object from factory - refunding money\n");
							Abandon_Production(type, id, naval);
						}
						return(placed);
					}
				}
			}
		} else {
			Cell location = cell; // RA2: placed at the cell's center coordinate
			if (tech) {
				BuildingClass * builder = tech->Who_Can_Build_Me(false, false);
				if (builder) {

					/*
					**	Try to place the object onto the map.
					*/
					builder->Transmit_Message(RADIO_HELLO, tech);
					if (tech->Unlimbo(Coord(location.X * 256 + 128, location.Y * 256 + 128, 0), DIR_N)) {
						if (tech->Fetch_RTTI() == RTTI_BUILDING) {
							BuildingTypeClass * btype = ((BuildingClass *)tech)->Class;
							if (btype->IsFirestormWall) {
								Map.Place_Firestorm_Wall(location, this, btype);
							} else if (btype->ToOverlay != NULL && btype->ToOverlay->IsWall) {
								Map.Place_Wall(location, this, btype);
							}
						}
						factory->Completed();
						tech->Transmit_Message(RADIO_COMPLETE, builder);
						Abandon_Production(type, id, naval);
						placed = true;

						if (PlayerPtr == this) {
							if (tech->IsActive && !tech->IsDiscoveredByPlayer) {
								tech->Revealed(this);
							}
							VocClass::PlayGlobal(Rule->BuildingSlam, 1.0f, 0x2000);
							if (!Debug_Map) { // RA2: the copy of the placement cursor goes too
								Map.Set_Copy_Cursor_Shape(0);
								Map.CopyPendingObjectPtr = 0;
								Map.CopyPendingObject = 0;
							}
							Map.Set_Cursor_Shape(0);
							Map.PendingObjectPtr = 0;
							Map.PendingObject = 0;
							Map.PendingHouse = HOUSE_NONE;
						}
					} else {
						placed = false;
						if (this == PlayerPtr) {
							Queue_Speech("EVA_CannotDeployHere", -1, -1);
						}
						if (!Debug_Map && this == PlayerPtr) { // RA2: the placement cursor comes back from its copy
							Map.PendingObjectPtr = Map.CopyPendingObjectPtr;
							Map.PendingObject = Map.CopyPendingObject;
							Map.PendingHouse = Map.CopyPendingHouse;
							Map.ZoneCell = Map.CopyZoneCell;
							Map.ZoneOffset = Map.CopyZoneOffset;
							Map.CursorSize = Map.CopyCursorSize;
							if (Map.CopyPendingObject) {
								Map.Set_Cursor_Shape(Map.CopyPendingObject->Occupy_List(true));
								Point2D xy = MouseCursor->Get_Mouse_Point();
								TacticalMap->Pixel_To_Coord(xy);
								xy -= Point2D(TacticalRect.X, TacticalRect.Y);
								Cell mouse_cell;
								Coord mouse_coord;
								ObjectClass * object = 0;
								bool fog, shadow;
								Map.Resolve_Point(xy, mouse_cell, mouse_coord, object, fog, shadow);
								Map.Set_Cursor_Pos(mouse_cell);
							}
							Map.Set_Copy_Cursor_Shape(0);
							Map.CopyPendingObjectPtr = 0;
							Map.CopyPendingObject = 0;
						}
					}
					builder->Transmit_Message(RADIO_OVER_OUT);
				}
			}
		}

		/*
		**	Perform any normal object placement checks
		**	(e.g. Set the Just_Built flag).
		*/
		if (placed) {
			Just_Built(tech);
			BuildingClass * builder = tech->Who_Can_Build_Me(false, true); // RA2: the builder's creation sound
			if (builder && builder->Fetch_RTTI() == RTTI_BUILDING) {
				int sound = builder->Class->CreateUnitSound;
				if (sound != -1) VocClass::PlayIndexAtPos(sound, tech->PositionCoord, 0);
			}
		}
	}
	return(placed);
}


/// <summary>
/// Records that this house has just finished building an object.
/// This routine is called by the production code. The computer consults the record when it
/// decides what to build next, and the produced quantity trackers use it to keep a tally of
/// everything this house has turned out.
/// </summary>
/// <param name="product">The object that just rolled off the production line.</param>
void HouseClass::Just_Built(TechnoClass * product)
{
	int sound = -1; // RA2: the creation sound of the product, played at its location below
	IsBuiltSomething = true;

	TechnoTypeClass const * ttype = product->Techno_Type_Class(); // RA2: the type through the virtual accessor

	switch (product->Fetch_RTTI()) {
		case RTTI_UNIT:
			JustBuiltUnit = (UnitType)ttype->Fetch_Heap_ID();
			PUQuantity.Increment(ttype->Fetch_Heap_ID());
			sound = Rule->CreateUnitSound; // RA2: the default sound of the kind
			break;

		case RTTI_INFANTRY:
			JustBuiltInfantry = (InfantryType)ttype->Fetch_Heap_ID();
			PIQuantity.Increment(ttype->Fetch_Heap_ID());
			sound = Rule->CreateInfantrySound; // RA2: as above
			break;

		case RTTI_BUILDING:
			JustBuiltStructure = (StructType)ttype->Fetch_Heap_ID();
			PBQuantity.Increment(ttype->Fetch_Heap_ID());
			break;

		case RTTI_AIRCRAFT:
			JustBuiltAircraft = (AircraftType)ttype->Fetch_Heap_ID();
			PAQuantity.Increment(ttype->Fetch_Heap_ID());
			sound = Rule->CreateAircraftSound; // RA2: as above
			break;
	}
	if (ttype->CreateSound != -1) { // RA2: the type's own sound wins
		sound = ttype->CreateSound; // RA2: as above
	}
	if (sound != -1) { // RA2: as above
		Coord coord = product->Position; // RA2: as above
		VocClass::PlayAt(sound, coord); // RA2: as above
	}
	IsRecalcNeeded = true;
}



/***********************************************************************************************
 * HouseClass::Manual_Place -- Inform display system of building placement mode.               *
 *                                                                                             *
 *    This routine will inform the display system that building placement mode has begun.      *
 *    The cursor will be created that matches the layout of the building shape.                *
 *                                                                                             *
 * INPUT:   builder  -- The factory that is building this object.                              *
 *                                                                                             *
 *          object   -- The building that is going to be placed down on the map.               *
 *                                                                                             *
 * OUTPUT:  Was the building placement mode successfully initiated?                            *
 *                                                                                             *
 * WARNINGS:   This merely adjusts the cursor shape. Nothing that affects networked games      *
 *             is affected.                                                                    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/04/1995 JLB : Created.                                                                 *
 *   05/30/1995 JLB : Uses the Bib_And_Offset() function to determine bib size.                *
 *=============================================================================================*/
bool HouseClass::Manual_Place(BuildingClass * builder,  BuildingClass * object)
{
	if (this == PlayerPtr && !Map.PendingObject && builder && object) {
		/*
		**	Ensures that object selection doesn't remain when
		**	building placement takes place.
		*/
		Unselect_All();

		Map.Repair_Mode_Control(0);
		Map.Sell_Mode_Control(0);
		Map.IsPowerMode = false; // RA2: placing also leaves power, waypoint, beacon and targetting mode
		Map.IsWaypointMode = false; // RA2: as above
		Map.IsBeaconMode = false; // RA2: as above
		Map.IsTargettingMode = (SuperWeaponType)-1; // RA2: as above

		Map.PendingObject = object->Class;
		Map.PendingObjectPtr = object;
		Map.PendingHouse = HeapID;

		Map.Set_Cursor_Shape(object->Occupy_List(true));
		Map.Set_Cursor_Pos(builder->Get_Cell());
		builder->Mark(MARK_CHANGE);
		return(true);
	}
	return(false);
}



/***************************************************************************
 * HouseClass::Clobber_All -- removes all objects for this house           *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      This routine removes the house itself, so the multiplayer code     *
 *        must not rely on there being "empty" houses lying around.        *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/16/1995 BRR : Created.                                             *
 *   06/09/1995 JLB : Handles aircraft.                                    *
 *=========================================================================*/
void HouseClass::Clobber_All(void)
{
	int i;

	for (i = 0; i < Technos.Count(); i++) {
		if (Technos[i]->House == this) {
			delete Technos[i];
			i--;
		}
	}
	for (i = 0; i < Triggers.Count(); i++) {
		if (Triggers[i]->Class->House == Class) {
			delete Triggers[i];
			i--;
		}
	}

	delete this;
}


/***********************************************************************************************
 * HouseClass::Detach -- Removes specified object from house tracking systems.                 *
 *                                                                                             *
 *    This routine is called when an object is to be removed from the game system. If the      *
 *    specified object is part of the house tracking system, then it will be removed.          *
 *                                                                                             *
 * INPUT:   target   -- The target value of the object that is to be removed from the game.    *
 *                                                                                             *
 *          all      -- Is the target going away for good as opposed to just cloaking/hiding?  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/18/1995 JLB : commented                                                                *
 *=============================================================================================*/
void HouseClass::Detach(AbstractClass const * target, bool all)
{
	if (ToCapture == target) {
		ToCapture = 0; // RA2: RA2's form
	}
	TagClass *tag = dynamic_cast<TagClass *>((AbstractClass *)target); // RA2: as above
	if (tag != 0) { // RA2: as above
		HouseTags.Delete(tag);
	}
	if (target->Fetch_RTTI() == RTTI_BUILDING && all) { // RA2: as above
		ConYards.Delete((BuildingClass *)target); // RA2: as above
		UnitRepairStations.Delete((BuildingClass *)target); // RA2: as above
		CloningVats.Delete((BuildingClass *)target); // RA2: as above
		PsychicDetectionBuildings.Delete((BuildingClass *)target); // RA2: as above
		Buildings.Delete((BuildingClass *)target); // RA2: as above
	}
	for (int index = 0; index < 12; index++) { // RA2: as above
		if (Paths[index] == target) { // RA2: as above
			Paths[index] = 0; // RA2: as above
		}
	}
	if (AircraftFactory == target) {
		AircraftFactory = 0; // RA2: as above
	}
	if (InfantryFactory == target) {
		InfantryFactory = 0; // RA2: as above
	}
	if (UnitFactory == target) {
		UnitFactory = 0; // RA2: as above
	}
	if (Primary_ForShips == target) { // RA2: as above
		Primary_ForShips = 0; // RA2: as above
	}

	if (BuildingFactory == target) {
		BuildingFactory = 0; // RA2: as above
	}
	if (Primary_Unused1 == target) { // RA2: as above
		Primary_Unused1 = 0; // RA2: as above
	}
	if (Primary_Unused2 == target) { // RA2: as above
		Primary_Unused2 = 0; // RA2: as above
	}
	if (Primary_Unused3 == target) { // RA2: as above
		Primary_Unused3 = 0; // RA2: as above
	}
	if (Primary_ForDefenses == target) { // RA2: as above
		Primary_ForDefenses = 0; // RA2: as above
	}
}



/***********************************************************************************************
 * HouseClass::Does_Enemy_Building_Exist -- Checks for enemy building of specified type.       *
 *                                                                                             *
 *    This routine will examine the enemy houses and if there is a building owned by one       *
 *    of those house, true will be returned.                                                   *
 *                                                                                             *
 * INPUT:   btype -- The building type to check for.                                           *
 *                                                                                             *
 * OUTPUT:  Does a building of the specified type exist for one of the enemy houses?           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool HouseClass::Does_Enemy_Building_Exist(StructType btype) const
{
	for (HousesType index = HOUSE_FIRST; index < Houses.Count(); index++) {
		HouseClass * house = Houses[index];

		if (house && !Is_Ally(house) && house->ABQuantity.Value(btype) > 0) {
			return(true);
		}
	}
	return(false);
}


/***********************************************************************************************
 * HouseClass::Suggest_New_Object -- Determine what would the next buildable object be.        *
 *                                                                                             *
 *    This routine will examine the house status and return with a techno type pointer to      *
 *    the object type that it thinks should be created. The type is restricted to match the    *
 *    type specified. Typical use of this routine is by computer controlled factories.         *
 *                                                                                             *
 * INPUT:   objecttype  -- The type of object to restrict the scan for.                        *
 *                                                                                             *
 *          kennel      -- Is this from a kennel? There are special hacks to ensure that only  *
 *                         dogs can be produced from a kennel.                                 *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to a techno type for the object type that should be         *
 *          created. If no object should be created, then NULL is returned.                    *
 *                                                                                             *
 * WARNINGS:   This is a time consuming routine. Only call when necessary.                     *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
TechnoTypeClass const * HouseClass::Suggest_New_Object(RTTIType objecttype, bool kennel) const
{
	TechnoTypeClass const * techno = NULL;

	switch (objecttype) {
		case RTTI_AIRCRAFT:
		case RTTI_AIRCRAFTTYPE:
			if (BuildAircraft != AIRCRAFT_NONE) {
				return(AircraftTypes[BuildAircraft]);
			}
			return(NULL);

		/*
		**	Unit construction is based on the rule that up to twice the number required
		**	to fill all teams will be created.
		*/
		case RTTI_UNIT:
		case RTTI_UNITTYPE:
			if (BuildUnit != UNIT_NONE) {
				return(UnitTypes[BuildUnit]);
			}
			return(NULL);

		/*
		**	Infantry construction is based on the rule that up to twice the number required
		**	to fill all teams will be created.
		*/
		case RTTI_INFANTRY:
		case RTTI_INFANTRYTYPE:
			if (BuildInfantry != INFANTRY_NONE) {
				return(InfantryTypes[BuildInfantry]);
			}
			return(NULL);

		/*
		**	Building construction is based upon the preconstruction list.
		*/
		case RTTI_BUILDING:
		case RTTI_BUILDINGTYPE:
			if (BuildStructure != STRUCT_NONE) {
				return(BuildingTypes[BuildStructure]);
			}
			return(NULL);
	}
	return(techno);
}


/***********************************************************************************************
 * HouseClass::Flag_Remove -- Removes the flag from the specified target.                      *
 *                                                                                             *
 *    This routine will remove the flag attached to the specified target object or cell.       *
 *    Call this routine before placing the object down. This is called inherently by the       *
 *    the Flag_Attach() functions.                                                             *
 *                                                                                             *
 * INPUT:   target   -- The target that the flag was attached to but will be removed from.     *
 *                                                                                             *
 *          set_home -- if true, clears the flag's waypoint designation                        *
 *                                                                                             *
 * OUTPUT:  Was the flag successfully removed from the specified target?                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool HouseClass::Flag_Remove(AbstractClass * target, bool set_home)
{
	bool rc = false;

	if (target) {

		/*
		**	Remove the flag from a unit
		*/
		if (target->Fetch_RTTI() == RTTI_UNIT) { // RA2: tested by Fetch_RTTI, not As_UnitClass
			rc = ((UnitClass *)target)->Flag_Remove(); // RA2: as above
			if (rc && FlagLocation == target) {
				FlagLocation = NULL;
			}

		} else {

			/*
			**	Remove the flag from a cell
			*/
			Cell cell = target->Center_Coord().As_Cell();
			if (Map.In_Radar(cell)) {
				rc = Map[cell].Flag_Remove();
				if (rc && FlagLocation == target) {
					FlagLocation = NULL;
				}
			}
		}

		/*
		**	Handle the flag home cell:
		**	If 'set_home' is set, clear the home value & the cell's overlay
		*/
		if (set_home) {
			if (FlagHome != CELL_NONE) {
				Map[FlagHome].Overlay = OVERLAY_NONE;
				FlagHome = CELL_NONE;
			}
		}
		return(rc);
	}
	return(false);
}



/***********************************************************************************************
 * HouseClass::Flag_Attach -- Attach flag to specified cell (or thereabouts).                  *
 *                                                                                             *
 *    This routine will attach the house flag to the location specified. If the location       *
 *    cannot contain the flag, then a suitable nearby location will be selected.               *
 *                                                                                             *
 * INPUT:   cell  -- The desired cell location to place the flag.                              *
 *                                                                                             *
 *          set_home -- if true, resets the flag's waypoint designation                        *
 *                                                                                             *
 * OUTPUT:  Was the flag successfully placed?                                                  *
 *                                                                                             *
 * WARNINGS:   The cell picked for the flag might very likely not be the cell requested.       *
 *             Check the FlagLocation value to determine the final cell resting spot.          *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1995 JLB : Created.                                                                 *
 *   10/08/1996 JLB : Uses map nearby cell scanning handler.                                   *
 *=============================================================================================*/
bool HouseClass::Flag_Attach(Cell const & cell, bool set_home)
{
	bool rc;

	/*
	**	Only continue if this cell is a legal placement cell.
	*/
	if (Map.In_Radar(cell)) {

		/*
		**	If the flag already exists, then it must be removed from the object
		**	it is attached to.
		*/
		Flag_Remove(FlagLocation, set_home);

		/*
		**	Attach the flag to the cell specified. If it can't be placed, then pick
		**	a nearby cell where it can be placed.
		*/
		Cell newcell = cell;
		rc = Map[newcell].Flag_Place(Class->House);
		if (!rc) {
			newcell = Map.Nearby_Location(cell, SPEED_TRACK);
			if (newcell != CELL_NONE) {
				rc = Map[newcell].Flag_Place(Class->House);
			}
		}

		/*
		**	If we've found a spot for the flag, place the flag at the new cell.
		**	if 'set_home' is set, OR this house has no current flag home cell,
		**	mark that cell as this house's flag home cell.
		*/
		if (rc) {
			FlagLocation = &Map[newcell];
		}

		return(rc);
	}
	return(false);
}


/***********************************************************************************************
 * HouseClass::Flag_Attach -- Attaches the house flag the specified unit.                      *
 *                                                                                             *
 *    This routine will attach the house flag to the specified unit. This routine is called    *
 *    when a unit drives over a cell containing a flag.                                        *
 *                                                                                             *
 * INPUT:   object   -- Pointer to the object that the house flag is to be attached to.        *
 *                                                                                             *
 *          set_home -- if true, clears the flag's waypoint designation                        *
 *                                                                                             *
 * OUTPUT:  Was the flag attached successfully?                                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool HouseClass::Flag_Attach(UnitClass * object, bool set_home)
{
	if (object && !object->IsInLimbo) {
		Flag_Remove(FlagLocation, set_home);

		/*
		**	Attach the flag to the object.
		*/
		object->Flag_Attach(Class->House);
		FlagLocation = object;
		return(true);
	}
	return(false);
}


/***************************************************************************
 * HouseClass::MPlayer_Defeated -- multiplayer; house is defeated          *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/25/1995 BRR : Created.                                             *
 *=========================================================================*/
void HouseClass::MPlayer_Defeated(void)
{
	wchar_t txt[80]; // RA2: wide text
	int i,j;
	HouseClass * hptr;
	HouseClass * hptr2;
	int num_alive;
	int num_humans;
	bool all_allies;

	/*
	**	Set the defeat flag for this house
	*/
	IsDefeated = true;

	/*
	**	If this is a computer controlled house, then all computer controlled
	**	houses become paranoid.
	*/
	if (IQ == Rule->MaxIQ && !Is_Human_Player() && Rule->IsComputerParanoid && Session.Type != GAME_NORMAL) { // RA2: multiplayer only
		Computer_Paranoid();
	}

	/*
	**	Remove this house's flag & flag home cell
	*/
	if (Scen->Special.IsCaptureTheFlag) {
		if (FlagLocation) {
			Flag_Remove(FlagLocation, true);
		} else if (FlagHome != CELL_NONE) {
			Flag_Remove(&Map[FlagHome], true);
		}
	}

	/*
	 * If harvester truce is on, remove all of this player's harvesters.
	 */
	if (Session.Type != GAME_NORMAL && Scen->Special.IsHarvesterImmune) {
		for (i = 0; i < Units.Count(); i++) {
			UnitClass * unit = Units[i];
			if (unit->House == this && unit->IsActive) {
				unit->Delete_Me();
			}
		}
	}

	/*
	**	If this is me:
	**	- Reveal the map
	**	- Add my defeat message
	*/
	if (PlayerPtr == this) {
		BeaconManager.Remove_All_Beacons(PlayerPtr->HeapID); // RA2: the player's beacons go, the open sidebar thumb is remembered and closed
		if (Map.IsThumbActive) {
			Map.Close_Thumb();
			ThumbWasActive = true;
		} else {
			ThumbWasActive = false;
		}
		Planning_End(); // RA2: planning and beacon placement end
		Map.Place_Beacon_Mode_Control(0);
		MuteSWLaunches = 1;
		Map.Reveal_The_Map();
		Map.Toggle_Radar(true); // RA2: the radar comes up
		HiddenSurface->Fill(0);
		Map.Flag_To_Redraw(GS_REDRAW_ALL);
		memset(DiplomacyChatFlags, 1, sizeof(DiplomacyChatFlags)); // RA2: chat goes to every player
		if (WS_Top_Window_ID() == 0x73) {
			Map.Update_Diplomacy_Rows(WS_Top_Window()); // RA2: an open diplomacy dialog is refreshed
		}

		/*
		**	Pop up a message showing that I was defeated
		*/
		if (ObserverHouse != PlayerPtr) { // RA2: an observer is told nothing
			swprintf(txt, StringTable::LoadString("TXT_PLAYER_DEFEATED", NULL, "D:\\Ra2\\House.CPP", 4703), UIName);
			Session.Messages.Add_Message(NULL, 0, txt, Session.Color_Index_To_Scheme(Game::PlayerColor), TextPrintType(0x4046), (int)(Rule->MessageDelay * 900), false);
			Queue_Speech("EVA_YouHaveLost", -1, -1);
		}
		Map.Flag_To_Redraw();
		DebugString("MPlayer_Defeated() - Player %s has been defeated (OBIWAN MODE)\n", (char const *)IniName);
	} else {
		/*
		**	If it wasn't me, find out who was defeated
		*/
		if (!Class->IsMultiplayPassive) {
			if (this != ObserverHouse) {
				swprintf(txt, StringTable::LoadString("TXT_PLAYER_DEFEATED", NULL, "D:\\Ra2\\House.CPP", 4719), UIName);
				Session.Messages.Add_Message(NULL, 0, txt, Scheme, TextPrintType(0x4046), (int)(Rule->MessageDelay * 900), false);
				Queue_Speech("EVA_PlayerDefeated", -1, -1);
			}
			Map.Flag_To_Redraw();
		}
		DebugString("MPlayer_Defeated() - Opponent %s has been defeated\n", (char const *)IniName);
	}

	/*
	**	Find out how many players are left alive.
	*/
	num_alive = 0;
	num_humans = 0;
	for (i = 0; i < Houses.Count(); i++) {
		hptr = Houses[i];
		if (hptr && !hptr->IsDefeated && !hptr->Class->IsMultiplayPassive) {
			if (hptr->Is_Human_Player()) {
				num_humans++;
			}
			num_alive++;
		}
	}
	DebugString("MPlayer_Defeated() - Alive = %d, Humans = %d\n", num_alive, num_humans);

	/*
	**	If all the houses left alive are allied with each other, then in reality
	**	there's only one player left:
	*/
	all_allies = true;
	for (i = 0; i < Houses.Count(); i++) {
		hptr = Houses[i];
		if (!hptr || hptr->IsDefeated || (Session.Type != GAME_SKIRMISH && hptr->Class->IsMultiplayPassive)) continue;

		for (j = 0; j < Houses.Count(); j++) {
			hptr2 = Houses[j];
			if (hptr2 && !hptr2->IsDefeated && (Session.Type == GAME_SKIRMISH || !hptr2->Class->IsMultiplayPassive) && (!hptr->Is_Ally(hptr2) || !hptr2->Is_Ally(hptr))) {
				all_allies = false;
				break;
			}
		}
		if (!all_allies) break;
	}

	/*
	**	If all houses left are allies, set 'num_alive' to 1; game over.
	*/
	if (all_allies) {
		Session.SawGameCompletion = true;
		DebugString("Saw game completion due to player defeat\n");
		num_alive = 1;
		DebugString("MPlayer_Defeated() - All remaining players are allied\n");
	}

	/*
	**	If there's only one human player left or no humans left, the game is over:
	**	the player wins or loses by the player's IsDefeated flag.
	*/
	if (num_alive == 1 || num_humans == 0) {
		if (ThumbWasActive) Map.Open_Thumb(); // RA2: the thumb comes back
		if (PlayerPtr->IsDefeated) {
			MultiplayerGameMode * mode = Session.MPGameMode; // RA2: cooperative modes win together
			bool present = mode != NULL;
			bool cooperative = (Session.Type == GAME_IPX || Session.Type == GAME_INTERNET) && present && mode->vt_entry_04() && mode->vt_entry_08();
			if (num_alive == 1 && cooperative) {
				bool human_alive = false;
				for (i = 0; i < Houses.Count(); i++) {
					hptr = Houses[i];
					if (hptr && !hptr->IsDefeated && hptr->Is_Human_Player()) human_alive = true;
				}
				if (human_alive) {
					DebugString("MPlayer_Defeated() - Allies win, Flag_To_Win\n");
					PlayerPtr->Flag_To_Win(false);
				} else {
					DebugString("MPlayer_Defeated() - Allies lost, Flag_To_Lose\n");
					PlayerPtr->Flag_To_Lose(false);
				}
			} else {
				DebugString("MPlayer_Defeated() - Flag_To_Lose\n");
				PlayerPtr->Flag_To_Lose(false);
			}
		} else {
			DebugString("MPlayer_Defeated() - Flag_To_Win\n");
			PlayerPtr->Flag_To_Win(false);
		}
	}
}


/***************************************************************************
 * HouseClass::Blowup_All -- blows up everything                           *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/16/1995 BRR : Created.                                             *
 *   06/09/1995 JLB : Handles aircraft.                                    *
 *   05/07/1996 JLB : Handles ships.                                       *
 *=========================================================================*/
void HouseClass::Blowup_All(void)
{
	int index = 0; // RA2: one pass over every techno (no units/infantry/buildings lists)
	TechnoClass * last = NULL; // RA2: as above
	int damage; // RA2: as above
	while (index < Technos.Count()) { // RA2: as above
		TechnoClass * techno = Technos[index]; // RA2: as above
		HouseClass * owner = techno->MindControlledBy != NULL ? techno->MindControlledBy->CaptureManager->OriginalOwner : techno->House; // RA2: a mind controlled techno belongs to its original owner
		if (owner == this && techno != last) { // RA2: as above
			damage = techno->Strength; // RA2: as above
			if (techno->TemporalTargetingMe != NULL) { // RA2: a temporal warp on it is released first
				techno->TemporalTargetingMe->JustLetGo(); // RA2: as above
			}
			techno->Take_Damage(damage, 0, Rule->C4Warhead, NULL, true, true); // RA2: a forced blow with the extra flag set
			last = techno; // RA2: as above
		} else {
			index++; // RA2: as above
		}
	}
}



/***********************************************************************************************
 * HouseClass::Flag_To_Die -- Flags the house to blow up soon.                                 *
 *                                                                                             *
 *    When this routine is called, the house will blow up after a period of time. Typically    *
 *    this is called when the flag is captured or the HQ destroyed.                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Was the house flagged to blow up?                                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/20/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool HouseClass::Flag_To_Die(void)
{
	if (!IsToWin && !IsToDie && !IsToLose) {
		IsToDie = true;
		BorrowedTime = CDTimerClass<FrameTimerClass>(0); // RA2: RA2's form
	}
	return IsToDie; // RA2: as above
}



/***********************************************************************************************
 * HouseClass::Flag_To_Win -- Flags the house to win soon.                                     *
 *                                                                                             *
 *    When this routine is called, the house will be declared the winner after a period of     *
 *    time.                                                                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Was the house flagged to win?                                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/20/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool HouseClass::Flag_To_Win(bool silent)
{
	if (!IsToWin && !IsToDie && !IsToLose) {
		IsToWin = true;
		if (!silent) {
			BorrowedTime = int(TICKS_PER_MINUTE * Rule->SavourDelay);
			if (Session.Type != GAME_NORMAL && Session.Type != GAME_SKIRMISH) {
				int time = Frame + MAX(BorrowedTime, (int)Session.MaxAhead);
				BorrowedTime = 10 * ((time + 9) / 10) - Frame;
			}
			DebugString("Frame %d, BorrowedTime == %d\n", Frame, (int)BorrowedTime);
		}
		if (Session.Type == GAME_NORMAL) { // RA2: the player test sits inside the game type test
			if (Is_Player_Control()) { // RA2: as above
				Lock_Scenario_Input(); // RA2: as above
				TacticalMap->Set_Caption_Text(StringTable::LoadString("TXT_SCENARIO_WON", 0, "D:\\Ra2\\House.CPP", 5304), 0); // RA2: the caption is a string table text and a GRFXTXT frame
				Queue_Speech("EVA_MissionAccomplished", -1, -1); // RA2: EVA speaks by name through Queue_Speech
				Map.Flag_To_Redraw(); // RA2: as above
			}
		} else if (Is_Player_Control()) {
			TacticalMap->Set_Caption_Text(StringTable::LoadString("TXT_VICTORIOUS", 0, "D:\\Ra2\\House.CPP", 5307), 2); // RA2: as above
			Queue_Speech("EVA_YouAreVictorious", -1, -1); // RA2: as above
			Map.Flag_To_Redraw(); // RA2: as above
		}
	}
	return(IsToWin);
}




/***********************************************************************************************
 * HouseClass::Flag_To_Lose -- Flags the house to die soon.                                    *
 *                                                                                             *
 *    When this routine is called, it will spell the doom of this house. In a short while      *
 *    all of the object owned by this house will explode. Typical use of this routine is when  *
 *    the flag has been captured or the command vehicle has been destroyed.                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Has the doom been initiated?                                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/12/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool HouseClass::Flag_To_Lose(bool silent)
{
	IsToWin = false;
	if (!IsToDie && !IsToLose) {
		IsToLose = true;
		if (!silent) {
			BorrowedTime = int(TICKS_PER_MINUTE * Rule->SavourDelay);
			if (Session.Type != GAME_NORMAL && Session.Type != GAME_SKIRMISH) {
				int time = Frame + MAX(BorrowedTime, (int)Session.MaxAhead);
				BorrowedTime = 10 * ((time + 9) / 10) - Frame;
			}
			DebugString("Frame %d, BorrowedTime == %d\n", Frame, (int)BorrowedTime);
		}
		if (Session.Type == GAME_NORMAL) { // RA2: the player test sits inside the game type test
			if (Is_Player_Control()) { // RA2: as above
				Lock_Scenario_Input(); // RA2: as above
				TacticalMap->Set_Caption_Text(StringTable::LoadString("TXT_SCENARIO_LOST", 0, "D:\\Ra2\\House.CPP", 5359), 1); // RA2: the caption is a string table text and a GRFXTXT frame
				Queue_Speech("EVA_MissionFailed", -1, -1); // RA2: EVA speaks by name through Queue_Speech
				Map.Flag_To_Redraw(); // RA2: as above
			}
		} else if (Is_Player_Control()) {
			TacticalMap->Set_Caption_Text(StringTable::LoadString("TXT_LOST", 0, "D:\\Ra2\\House.CPP", 5362), 3); // RA2: as above
			if (PlayerPtr != ObserverHouse) { // RA2: an observer is not told that the game is lost
				Queue_Speech("EVA_YouHaveLost", -1, -1); // RA2: as above
			}
			Map.Flag_To_Redraw(); // RA2: as above
		}
	}
	return(IsToLose);
}




/// <summary>
/// Flags this house to bring its game to an end.
/// Use this routine to wind the scenario up regardless of who has earned the victory. A
/// house with no verdict yet is awarded the win; one already flagged merely gets the grace
/// period it needs to play the ending out.
/// </summary>
void HouseClass::Flag_To_End(void)
{
	if (!IsToLose && !IsToWin) {
		Flag_To_Win();
	} else {
		BorrowedTime.Start();
	}
}


/***********************************************************************************************
 * HouseClass::Init_Data -- Initializes the multiplayer color data.                            *
 *                                                                                             *
 *    This routine is called when initializing the color and remap data for this house. The    *
 *    primary user of this routine is the multiplayer version of the game, especially for      *
 *    saving & loading multiplayer games.                                                      *
 *                                                                                             *
 * INPUT:   color    -- The color of this house.                                               *
 *                                                                                             *
 *          house    -- The house that this should act like.                                   *
 *                                                                                             *
 *          credits  -- The initial credits to assign to this house.                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::Init_Data(int color, HousesType house, int credits)
{
	Credits = Control.InitialCredits = credits;
	Credits = credits;
	Class->Scheme = color;
	Scheme = color;
}


/***********************************************************************************************
 * HouseClass::Power_Fraction -- Fetches the current power output rating.                      *
 *                                                                                             *
 *    Use this routine to fetch the current power output as a fixed point fraction. The        *
 *    value 0x0100 is 100% power.                                                              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with power rating as a fixed pointer number.                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/22/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
double HouseClass::Power_Fraction(void) const
{
	if (Power >= Drain || Drain == 0) return(1);

	if (Power) {
		return((float)Power / (float)Drain);
	}
	return(0);
}


/***********************************************************************************************
 * HouseClass::Sell_Wall -- Tries to sell the wall at the specified location.                  *
 *                                                                                             *
 *    This routine will try to sell the wall at the specified location. If there is a wall     *
 *    present and it is owned by this house, then it can be sold.                              *
 *                                                                                             *
 * INPUT:   cell  -- The cell that wall selling is desired.                                    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/05/1995 JLB : Created.                                                                 *
 *   11/02/1996 JLB : Checks unsellable bit for wall type.                                     *
 *=============================================================================================*/
void HouseClass::Sell_Wall(Cell const & cell, bool quiet)
{
	if (cell != CELL_NONE) {
		CellClass const & cptr = Map[cell];
		OverlayType overlay = cptr.Overlay;

		if (overlay != OVERLAY_NONE) {
			HousesType owner = cptr.Owner;

			if (owner != HOUSE_NONE && Houses[owner]->Is_Human_Player()) {
				OverlayTypeClass const * optr = OverlayTypes[overlay];

				if (optr->IsWall) {
					BuildingTypeClass const * btype = NULL;
					for (int i = 0; i < BuildingTypes.Count(); i++) {
						if (BuildingTypes[i]->ToOverlay == optr) {
							btype = BuildingTypes[i];
							break;
						}
					}

					if (btype != NULL && !btype->IsUnsellable) {

						if (PlayerPtr == this && !quiet) {
							VocClass::PlayGlobal(Rule->SellSound, 1.0f, 0x2000); // RA2: the sound plays centred (panning 0x2000), no Sound_Effect
						}

						btype->Cost_Of(this);
						Map[cell].Overlay = OVERLAY_NONE;
						Map[cell].OverlayData = 0;
						Map[cell].Owner = HOUSE_NONE;
						Map[cell].Recalc_Attributes();
						Map[cell].Wall_Update();
						Map.Radar_Background(cell);
						TechnoClass::Remove_Target(&Map[cell]); // RA2: targets on the cell are dropped (0x6D7C80)
						Map.Update_Cell_Zone(cell);
						Map.Update_Cell_Subzones(cell);
					}
				}
			}
		}
	}
}



/***********************************************************************************************
 * HouseClass::Suggest_New_Building -- Examines the situation and suggests a building.         *
 *                                                                                             *
 *    This routine is called when a construction yard needs to know what to build next. It     *
 *    will either examine the prebuilt base list or try to figure out what to build next       *
 *    based on the current game situation.                                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the building type class to build.                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/27/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
BuildingTypeClass const * HouseClass::Suggest_New_Building(void) const
{
	if (BuildStructure != STRUCT_NONE) {
		return(BuildingTypes[BuildStructure]);
	}
	return(NULL);
}


/***********************************************************************************************
 * HouseClass::Find_Building -- Finds a building of specified type.                            *
 *                                                                                             *
 *    This routine is used to find a building of the specified type. This is particularly      *
 *    useful for when some event requires a specific building instance. The nuclear missile    *
 *    launch is a good example.                                                                *
 *                                                                                             *
 * INPUT:   type  -- The building type to scan for.                                            *
 *                                                                                             *
 *          zone  -- The zone that the building must be located in. If no zone specific search *
 *                   is desired, then pass ZONE_NONE.                                          *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the building type requested. If there is no building     *
 *          of the type requested, then NULL is returned.                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/27/1995 JLB : Created.                                                                 *
 *   10/02/1995 JLB : Allows for zone specifics.                                               *
 *=============================================================================================*/
BuildingClass * HouseClass::Find_Building(StructType type, ZoneType zone) const
{
	/*
	**	Only scan if we KNOW there is at least one building of the type
	**	requested.
	*/
	if (BQuantity.Value(type) > 0) {
		int count = Buildings.Count(); // RA2: RA2's form
		for (int index = 0; index < count; index++) { // RA2: as above
			BuildingClass *building = (BuildingClass *)Buildings[index]; // RA2: as above
			if (building != 0 && !building->IsInLimbo && // RA2: as above
				BuildingTypes.ID(building->Class) == type) { // RA2: as above
				if (zone == ZONE_NONE) { // RA2: as above
					return (BuildingClass *)building; // RA2: as above
				}
				if (Which_Zone(building->Center_Coord()) == zone) { // RA2: as above
					return (BuildingClass *)building; // RA2: as above
				}
			}
		}
	}
	return(0); // RA2: as above
}



/***********************************************************************************************
 * HouseClass::Find_Build_Location -- Finds a suitable building location.                      *
 *                                                                                             *
 *    This routine is used to find a suitable building location for the building specified.    *
 *    The auto base building logic uses this when building the base for the computer.          *
 *                                                                                             *
 * INPUT:   building -- Pointer to the building that needs to be placed down.                  *
 *                                                                                             *
 * OUTPUT:  Returns with the coordinate to place the building at. If there are no suitable     *
 *          locations, then NULL is returned.                                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/27/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
Coord HouseClass::Find_Build_Location(BuildingClass * building) const
{
	return(COORD_NONE);
}


/***********************************************************************************************
 * HouseClass::Recalc_Center -- Recalculates the center point of the base.                     *
 *                                                                                             *
 *    This routine will average the location of the base and record the center point. The      *
 *    recorded center point is used to determine such things as how far the base is spread     *
 *    out and where to protect the most. This routine should be called whenever a building     *
 *    is created or destroyed.                                                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/28/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::Recalc_Center(void)
{
	/*
	**	First presume that there is no base. If there is a base, then these values will be
	**	properly filled in below.
	*/
	Center = COORD_NONE;
	Radius = 0;
	for (ZoneType zone = ZONE_FIRST; zone < ZONE_COUNT; zone++) {
		ZoneInfo[zone].AirDefense = 0;
		ZoneInfo[zone].ArmorDefense = 0;
		ZoneInfo[zone].InfantryDefense = 0;
	}

	/*
	**	Only process the center base size/position calculation if there are buildings to
	**	consider. When no buildings for this house are present, then no processing need
	**	occur.
	*/
	if (CurBuildings > 0) {
		int x = 0;
		int y = 0;
		int count = 0;

		int buildings = Buildings.Count(); // RA2: the count is read once for each loop
		for (int index = 0; index < buildings; index++) { // RA2: as above
			BuildingClass const * b = Buildings[index];

			if (b != NULL && !b->IsInLimbo && b->Strength > 0) { // RA2: no owner test, the house array holds only its own buildings

				/*
				**	Give more "weight" to buildings that cost more. The presumption is that cheap
				**	buildings don't affect the base disposition as much as the more expensive
				**	buildings do.
				*/
				int weight = (b->Class->Cost_Of(this) / 1000)+1;
				for (int i = 0; i < weight; i++) {
					x += b->Center_Coord().X;
					y += b->Center_Coord().Y;
					count++;
				}
			}
		}

		/*
		**	This second check for quantity of buildings is necessary because the first
		**	check against CurBuildings doesn't take into account if the building is in
		**	limbo, but for base calculation, the limbo state disqualifies a building
		**	from being processed. Thus, CurBuildings may indicate a base, but count may
		**	not match.
		*/
		if (count > 0) {
			x /= count;
			y /= count;
			Center = Coord(x, y, 0); // RA2: the height is given
			Center = Map.Nearby_Location(Center.As_Cell(), SPEED_FOOT, -1, MZONE_NORMAL, false, Point2D(1, 1), false, false, false, true, Cell(0, 0), false).As_Coord(); // RA2: the center is moved to the nearest free cell
		}

		/*
		**	If there were any buildings discovered as legal to consider as part of the base,
		**	then figure out the general average radius of the building disposition as it
		**	relates to the center of the base.
		*/
		if (count > 1) {
			int radius = 0;

			int buildings = Buildings.Count(); // RA2: as above
			for (int index = 0; index < buildings; index++) { // RA2: as above
				BuildingClass const * b = Buildings[index];

				if (b != NULL && !b->IsInLimbo && b->Strength > 0) { // RA2: as above
					radius += Distance(Center, b->Center_Coord());
				}
			}
			Radius = MAX(radius / count, 2 * CELL_LEPTON_W);

			/*
			**	Determine the relative strength of each base defense zone.
			*/
			buildings = Buildings.Count(); // RA2: as above
			for (index = 0; index < buildings; index++) { // RA2: as above
				BuildingClass const * b = Buildings[index];

				if (b != NULL && !b->IsInLimbo && b->Strength > 0) { // RA2: as above
					ZoneType z = Which_Zone(b);

					if (z != ZONE_NONE) {
						ZoneInfo[z].ArmorDefense += b->Anti_Armor();
						ZoneInfo[z].AirDefense += b->Anti_Air();
						ZoneInfo[z].InfantryDefense += b->Anti_Infantry();
					}
				}
			}

		} else {
			Radius = 2 * CELL_LEPTON;
		}
	}
}





/***********************************************************************************************
 * HouseClass::Expert_AI -- Handles expert AI processing.                                      *
 *                                                                                             *
 *    This routine is called when the computer should perform expert AI processing. This       *
 *    method of AI is categorized as an "Expert System" process.                               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the number of game frames to delay before calling this routine again.      *
 *                                                                                             *
 * WARNINGS:   This is relatively time consuming -- call periodically.                         *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int HouseClass::Expert_AI(void) // RA2: the body follows the executable
{
	BuildingClass * b = NULL; // RA2: as above
	bool stop = false; // RA2: as above
	int time = TICKS_PER_SECOND * 10; // RA2: as above

	/*
	**	If there is no enemy assigned to this house, then assign one now. The
	**	enemy that is closest is picked. However, don't pick an enemy if the
	**	base has not been established yet.
	*/
	if (PickEnemyTimer == 0) { // RA2: as above
		if (Enemy == HOUSE_NONE && Session.Type != GAME_NORMAL && !Class->IsMultiplayPassive) { // RA2: as above
			Coord center = Center; // RA2: as above
			if (center != COORD_NONE) { // RA2: as above
				int close = INT_MAX; // RA2: as above
				HouseClass * enemy = NULL; // RA2: as above

				for (HousesType house = HOUSE_FIRST; house < Houses.Count(); house++) { // RA2: as above
					HouseClass * h = Houses[house]; // RA2: as above
					if (h != this && !h->Class->IsMultiplayPassive && !h->IsDefeated) { // RA2: as above

						/*
						**	Determine a priority value based on distance to the center of the
						**	candidate base. The higher the value, the better the candidate house
						**	is to becoming the preferred enemy for this house.
						*/
						Coord offset = h->Center; // RA2: the offset to the candidate base, subtracted in place
						offset -= center; // RA2: as above
						int value = (int)fastmath::sqrt((double)offset.X * offset.X + (double)offset.Z * offset.Z + (double)offset.Y * offset.Y); // RA2: the distance is summed x, z, y

						/*
						**	Compare the calculated value for this candidate house and if it is
						**	greater than the previously recorded maximum, record this house as
						**	the prime candidate for enemy.
						*/
						if (value < close) { // RA2: as above
							enemy = h; // RA2: as above
							close = value; // RA2: as above
						}
					}
				}

				/*
				**	Record this closest enemy base as the first enemy to attack.
				*/
				if (enemy) { // RA2: as above
					Add_Anger(1, enemy); // RA2: as above
				}
			}
		}
	}

	/*
	**	If the current enemy no longer has a base or is defeated, then don't consider
	**	that house a threat anymore. Clear out the enemy record and then try
	**	to find a new enemy.
	*/
	if (Enemy != HOUSE_NONE) { // RA2: as above
		HouseClass * h = Houses[Enemy]; // RA2: as above

		if (h->IsDefeated) { // RA2: as above
			for (int index = 0; index < AngerNodes.Count(); index++) { // RA2: the anger toward the defeated house is cancelled here
				if (AngerNodes[index].House == h) { // RA2: as above
					Add_Anger(-AngerNodes[index].Level, h); // RA2: as above
					break; // RA2: as above
				}
			}
			Enemy = HOUSE_NONE; // RA2: as above
		}
	}

	if (Session.Type != GAME_NORMAL || IQ >= Rule->IQSuperWeapons) { // RA2: as above
		AI_Super_Weapons(); // RA2: as above
	}

	/*
	**	House state transition check occurs here. Transitions that occur here are ones
	**	that relate to general base condition rather than specific combat events.
	**	Typically, this is limited to transitions between normal buildup mode and
	**	broke mode.
	*/
	if (State == STATE_ENDGAME) { // RA2: as above
		Fire_Sale(); // RA2: as above
		All_To_Hunt(); // RA2: as above
	} else {
		if (State == STATE_BUILDUP) { // RA2: as above
			if (Available_Money() < 25) { // RA2: as above
				State = STATE_BROKE; // RA2: as above
			}
		}
		if (State == STATE_BROKE) { // RA2: as above
			if (Available_Money() >= 25) { // RA2: as above
				State = STATE_BUILDUP; // RA2: as above
			}
		}
		if (State == STATE_ATTACKED && LATime + TICKS_PER_MINUTE < Frame) { // RA2: as above
			State = STATE_BUILDUP; // RA2: as above
		}
		if (State != STATE_ATTACKED && LATime + TICKS_PER_MINUTE > Frame) { // RA2: as above
			State = STATE_ATTACKED; // RA2: as above
		}
	}

	if (Session.Type != GAME_NORMAL) { // RA2: as above
		/*
		**	Records the urgency of all actions possible.
		*/
		UrgencyType urgency[STRATEGY_COUNT]; // RA2: as above
		StrategyType strat; // RA2: as above
		for (strat = STRATEGY_FIRST; strat < STRATEGY_COUNT; strat++) { // RA2: as above
			urgency[strat] = URGENCY_NONE; // RA2: as above

			switch (strat) { // RA2: as above
				case STRATEGY_FIRE_SALE: // RA2: as above
					urgency[strat] = Check_Fire_Sale(); // RA2: as above
					break; // RA2: as above

				case STRATEGY_RAISE_MONEY: // RA2: as above
					urgency[strat] = Check_Raise_Money(); // RA2: as above
					break; // RA2: as above

				default: // RA2: as above
					urgency[strat] = URGENCY_NONE; // RA2: as above
					break; // RA2: as above
			}
		}

		/*
		**	Performs the action required for each of the strategies that share
		**	the most urgent category. Stop processing if any strategy at the
		**	highest urgency performed any action. This is because higher urgency
		**	actions tend to greatly affect the lower urgency actions.
		*/
		for (UrgencyType u = URGENCY_CRITICAL; u >= URGENCY_LOW; u--) { // RA2: as above
			//bool acted = false;

			for (strat = STRATEGY_FIRST; strat < STRATEGY_COUNT; strat++) { // RA2: as above
				if (urgency[strat] == u) { // RA2: as above
					switch (strat) { // RA2: as above
						case STRATEGY_FIRE_SALE: // RA2: as above
							if (CurBuildings > 0 && u == URGENCY_CRITICAL) { // RA2: the fire sale is run here, only at the highest urgency
								Fire_Sale(); // RA2: as above
								All_To_Hunt(); // RA2: as above
							}
							break; // RA2: as above

						case STRATEGY_RAISE_MONEY: // RA2: as above
							AI_Raise_Money(u); // RA2: as above
							break; // RA2: as above

						default: // RA2: as above
							break; // RA2: as above
					}
				}
			}
		}
	}

	return(105 + Random_Pick(1, 7)); // RA2: the next call is 105 plus one to seven frames away
}





/// <summary>
/// Determines how urgently this house should sell off its base.
/// This routine is part of the computer's emergency handling. A house that still holds
/// buildings but has nothing left that can produce is finished, and there is no reason to
/// leave the base standing.
/// </summary>
/// <returns>Returns with the urgency of selling the base off.</returns>
UrgencyType HouseClass::Check_Fire_Sale(void)
{
	/*
	**	If there are no more factories at all, then sell everything off because the game
	**	is basically over at this point.
	*/
	if (State != STATE_ATTACKED && CurBuildings > 0) {
		int count = Buildings.Count(); // RA2: RA2's form
		for (int index = 0; index < count; index++) { // RA2: as above
			BuildingClass *building = (BuildingClass *)Buildings[index]; // RA2: as above
			if (building != 0 && building->IsActive && !building->IsInLimbo && // RA2: as above
				(((BuildingTypeClass *)building->Class))->ToBuild != 0) { // RA2: as above
				return(URGENCY_NONE);
			}
		}
		return(URGENCY_CRITICAL);
	}
	return(URGENCY_NONE);
}



/*
**	Checks to see if money is critically low and something must be done
**	to immediately raise cash.
*/

/// <summary>
/// Determines how urgently this house must raise cash.
/// This routine is part of the computer's emergency handling. It weighs how close the house
/// already is to restoring its income -- a refinery or a harvester already on order and
/// affordable counts as the problem being dealt with. Human controlled houses are left to
/// fend for themselves.
/// </summary>
/// <returns>Returns with the urgency of this house's money troubles.</returns>
UrgencyType HouseClass::Check_Raise_Money(void)
{
	UrgencyType urgency = URGENCY_NONE;

	/*
	 * Humans look after their own finances.
	 */
	if (Is_Human_Player()) {
		return(URGENCY_NONE);
	}

	if (!Can_Make_Money()) {
		bool has_refinery = ABQuantity.Value(Rule->BuildRefinery[0]->HeapID) > 0 || ABQuantity.Value(Rule->BuildRefinery[1]->HeapID) > 0; // RA2: either of the two refineries counts
		if (has_refinery) { // RA2: the refinery test is a flag, and a harvester or refinery in production is found by a search that keeps the factory in a pointer; the urgency is assigned, not counted up

			/*
			 * A refinery already going up means the situation is being taken care of.
			 */
			for (int i = 0; i < ::Buildings.Count(); i++) { // RA2: the search runs over every building in the game
				if (::Buildings[i]->House == this && (::Buildings[i]->Class == Rule->BuildRefinery[0] || ::Buildings[i]->Class == Rule->BuildRefinery[1]) && ::Buildings[i]->CurrentMission == MISSION_CONSTRUCTION) { // RA2: as above
					return(URGENCY_NONE); // RA2: as above
				}
			}

			/*
			 * There is a refinery, so a harvester is what this house is short of. If one is
			 * not on order, or is on order but cannot be paid for, then cash must be raised.
			 */
			if (BuildUnit != Rule->HarvesterUnit[0]->HeapID && BuildUnit != Rule->HarvesterUnit[1]->HeapID) { // RA2: either of the two harvesters counts
				if (Available_Money() < Rule->HarvesterUnit[0]->Cost_Of(this) && Available_Money() < Rule->HarvesterUnit[1]->Cost_Of(this)) { // RA2: as above
					urgency = URGENCY_LOW; // RA2: as above
				}
			} else {
				FactoryClass * fptr = NULL; // RA2: as above
				for (int j = 0; j < Factories.Count(); j++) { // RA2: as above
					FactoryClass * fp = Factories[j]; // RA2: as above
					if (fp->House == this && Factories[j]->Get_Object() != NULL) { // RA2: as above
						UnitClass * unit = (UnitClass *)Factories[j]->Get_Object();
						if (unit->RTTI == RTTI_UNIT && (unit->Class == Rule->HarvesterUnit[0] || unit->Class == Rule->HarvesterUnit[1])) { // RA2: as above
							fptr = Factories[j];
							break;
						}
					}
				}
				if (fptr != NULL) { // RA2: as above
					int owed = fptr->Balance; // RA2: as above
					if (Available_Money() < owed) { // RA2: as above
						urgency = URGENCY_LOW; // RA2: as above
					}
				} else {
					urgency = URGENCY_LOW; // RA2: as above
				}
			}
		} else {

			/*
			 * No refinery at all. The same reasoning applies to getting one built.
			 */
			if (BuildStructure != Rule->BuildRefinery[0]->HeapID && BuildStructure != Rule->BuildRefinery[1]->HeapID) { // RA2: as above
				if (Available_Money() < Rule->BuildRefinery[0]->Cost_Of(this) && Available_Money() < Rule->BuildRefinery[1]->Cost_Of(this)) { // RA2: as above
					urgency = URGENCY_LOW; // RA2: as above
				}
			} else {
				FactoryClass * fptr = NULL; // RA2: as above
				for (int j = 0; j < Factories.Count(); j++) { // RA2: as above
					FactoryClass * fp = Factories[j]; // RA2: as above
					if (fp->House == this && Factories[j]->Get_Object() != NULL) { // RA2: as above
						BuildingClass * building = (BuildingClass *)Factories[j]->Get_Object();
						if (building->RTTI == RTTI_BUILDING && (building->Class == Rule->BuildRefinery[0] || building->Class == Rule->BuildRefinery[1])) { // RA2: as above
							fptr = Factories[j];
							break;
						}
					}
				}
				if (fptr != NULL) { // RA2: as above
					int owed = fptr->Balance; // RA2: as above
					if (Available_Money() < owed) { // RA2: as above
						urgency = URGENCY_LOW; // RA2: as above
					}
				} else {
					urgency = URGENCY_LOW; // RA2: as above
				}
			}
		}
	}
	return(urgency);
}





/// <summary>
/// Sells off the base and sends everything on a suicide run.
/// This is the last resort of the computer's emergency handling, and is acted upon only when
/// the situation has been judged critical.
/// </summary>
/// <param name="urgency">The urgency that the fire sale check reported.</param>
/// <returns>bool; Was the fire sale ordered?</returns>
bool HouseClass::AI_Fire_Sale(UrgencyType urgency)
{
	if (CurBuildings > 0 && urgency == URGENCY_CRITICAL) {
		Fire_Sale();
		All_To_Hunt();
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * HouseClass::AI_Raise_Money -- Raise emergency cash by selling buildings.                    *
 *                                                                                             *
 *    This routine handles the situation where the computer desperately needs cash but cannot  *
 *    wait for normal harvesting to raise it. Buildings must be sold.                          *
 *                                                                                             *
 * INPUT:   urgency  -- The urgency level that cash must be raised. The greater the urgency,   *
 *                      the more important the buildings that can be sold become.              *
 *                                                                                             *
 * OUTPUT:  bool; Was a building sold to raise cash?                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/02/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool HouseClass::AI_Raise_Money(UrgencyType urgency) // RA2: the body follows the executable
{
	int refund = 0; // RA2: the order of the locals follows the executable's
	int needed; // RA2: as above
	bool can_build_harvester = (ABQuantity.Value(Rule->BuildRefinery[0]->HeapID) > 0 || ABQuantity.Value(Rule->BuildRefinery[1]->HeapID) > 0) && // RA2: either of the first two refineries
		(ABQuantity.Value(Rule->BuildWeapons[0]->HeapID) > 0 || ABQuantity.Value(Rule->BuildWeapons[1]->HeapID) > 0); // RA2: as above

	if (can_build_harvester) { // RA2: as above
		needed = Get_First_Ownable(Rule->HarvesterUnit)->Cost_Of(this); // RA2: the first harvester this house may own
	} else {
		needed = Get_First_Ownable(Rule->BuildRefinery)->Cost_Of(this); // RA2: as above
	}

	int i = Base.Nodes.Count() - 1; // RA2: as above
	for (; i >= 0; i--) { // RA2: as above
		BuildingClass * bptr = Base.Get_Building(i); // RA2: as above
		if (bptr != NULL) { // RA2: as above
			char level = bptr->UpgradeLevel; // RA2: as above
			if (level != 0) { // RA2: as above
				int cost = ((BuildingTypeClass *)bptr->Upgrades[level - 1])->GetRefund(this, true); // RA2: the refund of the last upgrade, not its cost
				Credits += cost; // RA2: as above
				bptr->Remove_Upgrade(); // RA2: as above
				RecalcPower = true; // RA2: as above
				RecalcRadar = true; // RA2: as above
				Adjust_House_Power(this); // RA2: as above
			} else {
				bptr->Sell_Back(1); // RA2: as above
				if (bptr->Class->IsGate) { // RA2: as above
					Base.Nodes.Delete_Index(i); // RA2: as above
				}
				refund += bptr->Refund_Amount(); // RA2: as above
			}
		} else if (Base.Is_Built(i)) { // RA2: as above
			Sell_Wall(Base.Nodes[i].CellID, false); // RA2: as above
			Base.Nodes.Delete_Index(i); // RA2: as above
		}

		if (refund + Credits > needed) { // RA2: as above
			for (int j = Factories.Count() - 1; j >= 0; j--) { // RA2: as above
				FactoryClass * fptr = Factories[j]; // RA2: as above
				if (fptr->House == this) { // RA2: as above
					Factories[j]->Suspend(true); // RA2: the hold flag is passed
					Factories[j]->Abandon(); // RA2: as above
					delete Factories[j]; // RA2: as above
				}
			}
			BuildUnit = UNIT_NONE; // RA2: as above
			BuildInfantry = INFANTRY_NONE; // RA2: as above
			BuildAircraft = AIRCRAFT_NONE; // RA2: as above
			BuildStructure = STRUCT_NONE; // RA2: as above
			if (can_build_harvester) { // RA2: as above
				BuildUnit = (UnitType)Get_First_Ownable(Rule->HarvesterUnit)->HeapID; // RA2: the first ownable harvester
				ProductionMode = UNITS; // RA2: as above
			} else {
				int next = Base.Next_Buildable_Index(); // RA2: as above
				if (next != 0) { // RA2: as above
					if (next == -1) { // RA2: nothing buildable left in the base plan
						if (Base.Nodes.Length() < 2) goto GO_HUNT; // RA2: tested; the executable jumps to the hunt call
						next = 1; // RA2: as above
					}
					BaseNodeClass node(Get_First_Ownable(Rule->BuildRefinery)->HeapID, Cell(0, 0)); // RA2: a local node, copied into the plan
					Base.Nodes.Insert_After(next - 1, node); // RA2: as above
					for (int j = Base.Nodes.Count() - 1; j > next; j--) { // RA2: as above
						if (Base.Nodes[j].Type == Get_First_Ownable(Rule->BuildRefinery)->HeapID) { // RA2: as above
							Base.Nodes.Delete_Index(j); // RA2: as above
						}
					}
					ProductionMode = BUILDINGS; // RA2: as above
					AI_Building(); // RA2: as above
				} else {
					goto GO_HUNT; // RA2: as above
				}
			}
			break; // RA2: as above
		}
	}

	if (i <= 0) { // RA2: as above
		GO_HUNT: // RA2: tested; shared with the two jumps above
		All_To_Hunt(); // RA2: as above
	}
	return(false); // RA2: as above
}



/***********************************************************************************************
 * HouseClass::AI_Building -- Determines what building to build.                               *
 *                                                                                             *
 *    This routine handles the general case of determining what building to build next.        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of game frames to delay before calling this routine again. *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/29/1995 JLB : Created.                                                                 *
 *   11/03/1996 JLB : Tries to match aircraft of enemy                                         *
 *=============================================================================================*/
int HouseClass::AI_Building(void)
{
	enum {
		WALL = -3,
		STOP = -2,
		DEFENSE = -1
	};

	if (BuildStructure != STRUCT_NONE) return(TICKS_PER_SECOND);

	if (ConYards.Count() == 0) return(TICKS_PER_SECOND);

	BaseNodeClass * node = Base.Next_Buildable();
	if (node == NULL) return(TICKS_PER_SECOND);

	/*
	**	RA2: a naval node is dropped while naval buildings are not allowed, and the next taken.
	*/
	if (node->Type >= 0 && BuildingTypes[node->Type]->Naval && !IsNavalBaseAllowed) {
		Base.Nodes.Delete_Index(Base.Nodes.ID(node));
		node = Base.Next_Buildable();
	}

	if (node != NULL) {

		/*
		 * Build some walls.
		 */
		if (node->Type == WALL) {
			Base.Nodes.Delete_Index(Base.Nodes.ID(node));
			AI_Build_Wall();
			return(1);
		}

		BaseNodeClass copy = *node;
		if (copy.Type == DEFENSE || (BuildingTypes[copy.Type] == Rule->WallTower && node->CellID == Cell(0, 0))) {
			int nodeid = Base.Nodes.ID(node);

			/*
			**	RA2: by the difficulty's percentage, the defense is a wall around the last building instead.
			*/
			if (Random_Pick(0, 99) < Rule->AIPickWallDefensePercent[Difficulty] && Add_Wall_Nodes(nodeid)) {
				Base.Nodes.Delete_Index(Base.Nodes.ID(copy));
				return(1);
			}

			DynamicVectorClass<Cell> * cells = NULL;
			if (Base.OuterCells.Count() > 0) {
				cells = &Base.OuterCells;
			}
			if (!AI_Build_Defense(nodeid, cells)) {

				/*
				 * A wall tower node is deleted twice, so the node that follows it goes
				 * with it.
				 */
				if (node->Type == Rule->WallTower->HeapID) {
					Base.Nodes.Delete_Index(nodeid);
				}

				/*
				 * Remove the node from the list.
				 */
				Base.Nodes.Delete_Index(nodeid);
				return(1);
			}
		}

		if (node->Type == STOP) return(TICKS_PER_SECOND);

		/*
		**	RA2: a building the power would not cover is preceded by a power plant, unless it is a
		**	construction yard.
		*/
		BuildingTypeClass * b = BuildingTypes[node->Type];
		if (Drain + b->Drain > Power - PowerSurplus) {
			bool is_conyard = false;
			for (int i = 0; i < Rule->BuildConst.Count(); i++) {
				if (Rule->BuildConst[i] == b) {
					is_conyard = true;
					break;
				}
			}
			if (!is_conyard && b->Drain > 0) {
				BuildingTypeClass const * choice;
				if (ActLike == 0) {
					choice = Rule->GDIPowerPlant;
				} else {
					DynamicVectorClass<BuildingTypeClass const *> owned_buildings;
					for (int i = Buildings.Count() - 1; i >= 0; i--) {
						owned_buildings.Add((BuildingTypeClass const *)Buildings[i]);
					}
					if (AI_Has_Prerequisites(Rule->NodAdvancedPower, owned_buildings, owned_buildings.Count())) {
						choice = Rule->NodAdvancedPower;
					} else {
						choice = Rule->NodRegularPower;
					}
				}

				/*
				 * Build our chosen power structure before building whatever else we're trying to build.
				 */
				int id = Base.Nodes.ID(node);
				Base.Nodes.Insert_After(id - 1, BaseNodeClass(choice->HeapID, Cell(0, 0), false, 0));
				return(1);
			}
		}

		/*
		 * If this is a building upgrade, check that it can actually be placed where it is
		 * scheduled to go.
		 */
		if (b->PowersUpToLevel <= 0 && b->PowersUpToLevel == -1 && node->CellID != CELL_NONE && b->PowersUpBuilding[0]) {
			BuildingClass * existing_building = Map[node->CellID].Cell_Building();
			BuildingTypeClass * powerup = BuildingTypes[BuildingTypeClass::From_Name(b->PowersUpBuilding)];
			if (existing_building == NULL) {
				node->CellID = Cell(0, 0);
			} else if (existing_building->Class != powerup) {
				node->CellID = Cell(0, 0);
			} else if ((existing_building->Class->PowersUpToLevel == -1 && existing_building->UpgradeLevel >= existing_building->Class->Upgrades) || (existing_building->Class->PowersUpToLevel > 0 && existing_building->UpgradeLevel > 0)) {
				node->CellID = Cell(0, 0);
			}
		}
		BuildStructure = (StructType)node->Type;
	}
	return(TICKS_PER_SECOND);
}


/***********************************************************************************************
 * HouseClass::AI_Unit -- Determines what unit to build next.                                  *
 *                                                                                             *
 *    This routine handles the general case of determining what units to build next.           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of games frames to delay before calling this routine again.*
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int HouseClass::AI_Unit(void)
{
	if (BuildUnit != UNIT_NONE) return(TICKS_PER_SECOND);

	int harv = AUQuantity.Value(Get_First_Ownable(Rule->HarvesterUnit)->HeapID); // RA2: the first harvester this house may own
	BuildingTypeClass const * const & first = Rule->BuildRefinery[0];
	BuildingTypeClass const * const & second = Rule->BuildRefinery[1];
	int ref = ABQuantity.Value(first->HeapID) + ABQuantity.Value(second->HeapID); // RA2: both refineries count
	int mult = Rule->HarvestersPerRefinery; // RA2: the rules give the number of harvesters for each refinery

	/*
	**	A computer controlled house will try to build a replacement
	**	harvester if possible.
	*/
	if (IQ >= Rule->IQHarvester && !IsTiberiumShort && !Is_Human_Player() && ref * mult > harv) {
		if ((unsigned int)Get_First_Ownable(Rule->HarvesterUnit)->Level <= (unsigned int)Control.TechLevel) { // RA2: as above
			BuildUnit = Get_First_Ownable(Rule->HarvesterUnit)->HeapID; // RA2: as above
			return(TICKS_PER_SECOND);
		}
	}

	int i;
	int counter[100];
	int value[ARRAY_SIZE(counter)];
	memset(counter, 0x00, sizeof(counter));
	for (i = 0; i < ARRAY_SIZE(value); i++) {
		value[i] = 0x7FFFFFFF;
	}

	/*
	**	Build a list of the maximum of each type we wish to produce. This will be
	**	twice the number required to fill all teams.
	*/
	for (i = 0; i < Teams.Count(); i++) {
		TeamClass * tptr = Teams[i];
		if (tptr != NULL) {

			int val = tptr->CreationFrame;

			if (((tptr->Class->IsReinforcable && !tptr->IsFullStrength) || (!tptr->IsForcedActive && !tptr->IsHasBeen)) && tptr->House == this) {
				TEAM_MEMBER_LIST _members;
				tptr->Team_Members(_members);

				for (int subindex = 0; subindex < _members.Count(); subindex++) {

					UnitTypeClass const * memtype = (UnitTypeClass const *)_members[subindex];

					if (memtype->RTTI == RTTI_UNITTYPE) {
						counter[memtype->HeapID]++;
						if (val < value[memtype->HeapID]) {
							value[memtype->HeapID] = val;
						}
					}
				}
			}
		}
	}

	/*
	**	Reduce the theoretical maximum by the actual number of objects currently
	**	in play.
	*/
	for (int oindex = 0; oindex < Units.Count(); oindex++) {
		UnitClass * obj = Units[oindex];
		if (obj != NULL && obj->Is_Recruitable(this) && counter[obj->Class->HeapID] > 0) {
			counter[obj->Class->HeapID]--;
		}
	}

	/*
	**	Pick to build the most needed object but don't consider those object that
	**	can't be built because of scenario restrictions or insufficient cash.
	*/
	int bestval = -1;
	int bestcount = 0;
	UnitType lasttype = UNIT_NONE;
	int lastval = 0x7FFFFFFF;
	UnitType bestlist[ARRAY_SIZE(counter)];
	for (UnitType type = UnitType(0); type < UnitTypes.Count(); type++) {
		if (counter[type] > 0 && Can_Build(UnitTypes[type], false, false) && UnitTypes[type]->Cost_Of(this) <= Available_Money()) {
			if (bestval == -1 || bestval < counter[type]) {
				bestval = counter[type];
				bestcount = 0;
			}
			bestlist[bestcount++] = type;

			if (lasttype == UNIT_NONE || value[type] < lastval) {
				lasttype = type;
				lastval = value[type];
			}
		}
	}

	if (Random_Double(0, 0x7FFFFFFE) < Rule->FillEarliestTeamProbability[Difficulty] / 100.0) {
		BuildUnit = lasttype;
	} else {
		/*
		**	The object type to build is now known. Fetch a pointer to the techno type class.
		*/
		if (bestcount) {
			BuildUnit = bestlist[Random_Pick(0, bestcount-1)];
		}
	}

	return(TICKS_PER_SECOND);
}



/***********************************************************************************************
 * HouseClass::AI_Infantry -- Determines the infantry unit to build.                           *
 *                                                                                             *
 *    This routine handles the general case of determining what infantry unit to build         *
 *    next.                                                                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of game frames to delay before being called again.         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int HouseClass::AI_Infantry(void)
{
	if (BuildInfantry != INFANTRY_NONE) return(TICKS_PER_SECOND);

	int i;
	int counter[100];
	int value[ARRAY_SIZE(counter)];
	memset(counter, 0x00, sizeof(counter));
	for (i = 0; i < ARRAY_SIZE(value); i++) {
		value[i] = 0x7FFFFFFF;
	}

	/*
	**	Build a list of the maximum of each type we wish to produce. This will be
	**	twice the number required to fill all teams.
	*/
	for (i = 0; i < Teams.Count(); i++) {
		TeamClass * tptr = Teams[i];
		if (tptr != NULL) {

			int val = tptr->CreationFrame;

			if (((tptr->Class->IsReinforcable && !tptr->IsFullStrength) || (!tptr->IsForcedActive && !tptr->IsHasBeen)) && tptr->House == this) {
				TEAM_MEMBER_LIST _members;
				tptr->Team_Members(_members);

				for (int subindex = 0; subindex < _members.Count(); subindex++) {

					InfantryTypeClass const * memtype = (InfantryTypeClass const *)_members[subindex];

					if (memtype->RTTI == RTTI_INFANTRYTYPE) {
						counter[memtype->HeapID]++;
						if (val < value[memtype->HeapID]) {
							value[memtype->HeapID] = val;
						}
					}
				}
			}
		}
	}

	/*
	**	Reduce the theoretical maximum by the actual number of objects currently
	**	in play.
	*/
	for (int oindex = 0; oindex < Infantry.Count(); oindex++) {
		InfantryClass * obj = Infantry[oindex];
		if (obj != NULL && obj->Is_Recruitable(this) && counter[obj->Class->HeapID] > 0) {
			counter[obj->Class->HeapID]--;
		}
	}

	/*
	**	Pick to build the most needed object but don't consider those object that
	**	can't be built because of scenario restrictions or insufficient cash.
	*/
	int bestval = -1;
	int bestcount = 0;
	InfantryType lasttype = INFANTRY_NONE;
	int lastval = 0x7FFFFFFF;
	InfantryType bestlist[ARRAY_SIZE(counter)];
	for (InfantryType type = InfantryType(0); type < InfantryTypes.Count(); type++) {
		if (counter[type] > 0 && Can_Build(InfantryTypes[type], false, false) && InfantryTypes[type]->Cost_Of(this) <= Available_Money()) {
			if (bestval == -1 || bestval < counter[type]) {
				bestval = counter[type];
				bestcount = 0;
			}
			bestlist[bestcount++] = type;

			if (lasttype == INFANTRY_NONE || value[type] < lastval) {
				lasttype = type;
				lastval = value[type];
			}
		}
	}

	if (Random_Double(0, 0x7FFFFFFE) < Rule->FillEarliestTeamProbability[Difficulty] / 100.0) {
		BuildInfantry = lasttype;
	} else {
		/*
		**	The object type to build is now known. Fetch a pointer to the techno type class.
		*/
		if (bestcount) {
			BuildInfantry = bestlist[Random_Pick(0, bestcount-1)];
		}
	}

	return(TICKS_PER_SECOND);
}


/***********************************************************************************************
 * HouseClass::AI_Aircraft -- Determines what aircraft to build next.                          *
 *                                                                                             *
 *    This routine is used to determine the general case of what aircraft to build next.       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of frame to delay before calling this routine again.       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int HouseClass::AI_Aircraft(void)
{
	if (BuildAircraft != AIRCRAFT_NONE) return(TICKS_PER_SECOND);

	int i;
	int counter[100];
	int value[ARRAY_SIZE(counter)];
	memset(counter, 0x00, sizeof(counter));
	for (i = 0; i < ARRAY_SIZE(value); i++) {
		value[i] = 0x7FFFFFFF;
	}

	/*
	**	Build a list of the maximum of each type we wish to produce. This will be
	**	twice the number required to fill all teams.
	*/
	for (i = 0; i < Teams.Count(); i++) {
		TeamClass * tptr = Teams[i];
		if (tptr != NULL) {

			int val = tptr->CreationFrame;

			if (((tptr->Class->IsReinforcable && !tptr->IsFullStrength) || (!tptr->IsForcedActive && !tptr->IsHasBeen)) && tptr->House == this) {
				TEAM_MEMBER_LIST _members;
				tptr->Team_Members(_members);

				for (int subindex = 0; subindex < _members.Count(); subindex++) {

					AircraftTypeClass const * memtype = (AircraftTypeClass const *)_members[subindex];

					if (memtype->RTTI == RTTI_AIRCRAFTTYPE) {
						counter[memtype->HeapID]++;
						if (val < value[memtype->HeapID]) {
							value[memtype->HeapID] = val;
						}
					}
				}
			}
		}
	}

	/*
	**	Reduce the theoretical maximum by the actual number of objects currently
	**	in play.
	*/
	for (int oindex = 0; oindex < Aircraft.Count(); oindex++) {
		AircraftClass * obj = Aircraft[oindex];
		if (obj != NULL && obj->Is_Recruitable(this) && counter[obj->Class->HeapID] > 0) {
			counter[obj->Class->HeapID]--;
		}
	}

	/*
	**	Pick to build the most needed object but don't consider those object that
	**	can't be built because of scenario restrictions or insufficient cash.
	*/
	int bestval = -1;
	int bestcount = 0;
	AircraftType lasttype = AIRCRAFT_NONE;
	int lastval = 0x7FFFFFFF;
	AircraftType bestlist[ARRAY_SIZE(counter)];
	for (AircraftType type = AircraftType(0); type < AircraftTypes.Count(); type++) {
		if (counter[type] > 0 && Can_Build(AircraftTypes[type], false, false) && AircraftTypes[type]->Cost_Of(this) <= Available_Money()) {
			if (bestval == -1 || bestval < counter[type]) {
				bestval = counter[type];
				bestcount = 0;
			}
			bestlist[bestcount++] = type;

			if (lasttype == AIRCRAFT_NONE || value[type] < lastval) {
				lasttype = type;
				lastval = value[type];
			}
		}
	}

	if (Random_Double(0, 0x7FFFFFFE) < Rule->FillEarliestTeamProbability[Difficulty] / (100.0)) {
		BuildAircraft = lasttype;
	} else {
		/*
		**	The object type to build is now known. Fetch a pointer to the techno type class.
		*/
		if (bestcount) {
			BuildAircraft = bestlist[Random_Pick(0, bestcount-1)];
		}
	}

	return(TICKS_PER_SECOND);
}


/***********************************************************************************************
 * HouseClass::Production_Begun -- Records that production has begun.                          *
 *                                                                                             *
 *    This routine is used to inform the Expert System that production of the specified object *
 *    has begun. This allows the AI to proceed with picking another object to begin production *
 *    on.                                                                                      *
 *                                                                                             *
 * INPUT:   product  -- Pointer to the object that production has just begun on.               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::Production_Begun(TechnoClass const * product)
{
	// nothing
}


/***********************************************************************************************
 * HouseClass::Tracking_Remove -- Remove object from house tracking system.                    *
 *                                                                                             *
 *    This routine informs the Expert System that the specified object is no longer part of    *
 *    this house's inventory. This occurs when the object is destroyed or captured.            *
 *                                                                                             *
 * INPUT:   techno   -- Pointer to the object to remove from the tracking systems of this      *
 *                      house.                                                                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::Tracking_Remove(TechnoClass const * techno)
{
	((TechnoClass *)techno)->CountedAsOwned = false; // RA2: RA2's form
	if (!techno->TClass->IsInsignificant) { // RA2: as above
		switch ((RTTIType)techno->RTTI) {
		case RTTI_BUILDING:
			if (techno->Considered_Vehicle()) {
				CurUnits--;
			} else {
				CurBuildings--;
			}
			BQuantity.Decrement(techno->TClass->Fetch_Heap_ID());
			break;

		case RTTI_AIRCRAFT:
			CurAircraft--;
			AQuantity.Decrement(techno->TClass->Fetch_Heap_ID());
			break;

		case RTTI_INFANTRY:
			CurInfantry--;
			IQuantity.Decrement(techno->TClass->Fetch_Heap_ID());
			break;

		case RTTI_UNIT:
			CurUnits--;
			UQuantity.Decrement(techno->TClass->Fetch_Heap_ID());
			if (techno->TClass->Naval && techno->TClass->MaxPassengers == 0) { // RA2: as above
				OwnedNavy--; // RA2: as above
			}
			break;
		}
	}
}




/***********************************************************************************************
 * HouseClass::Tracking_Add -- Informs house of new inventory item.                            *
 *                                                                                             *
 *    This function is called when the specified object is now available as part of the house's*
 *    inventory. This occurs when the object is newly produced and also when it is captured    *
 *    by this house.                                                                           *
 *                                                                                             *
 * INPUT:   techno   -- Pointer to the object that is now part of the house inventory.         *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::Tracking_Add(TechnoClass const * techno)
{
	((TechnoClass *)techno)->CountedAsOwned = true; // RA2: RA2's form
	if (!techno->Techno_Type_Class()->IsInsignificant) { // RA2: as above
		switch (techno->Fetch_RTTI()) {
		case 6: { // RA2: as above
			if (techno->Considered_Vehicle()) {
				CurUnits++;
			} else {
				CurBuildings++;
			}
			int index = techno->Techno_Type_Class()->Fetch_Heap_ID(); // RA2: as above
			BQuantity.Increment(index); // RA2: as above
			BuildingTotals.Increment_Unit_Total(index); // RA2: as above
			break;
		}
		case 2: { // RA2: as above
			CurAircraft++;
			int index = techno->Techno_Type_Class()->Fetch_Heap_ID(); // RA2: as above
			AQuantity.Increment(index); // RA2: as above
			AircraftTotals.Increment_Unit_Total(index); // RA2: as above
			break;
		}
		case 15: { // RA2: as above
			int index = techno->Techno_Type_Class()->Fetch_Heap_ID(); // RA2: as above
			CurInfantry++;
			IQuantity.Increment(index); // RA2: as above
			InfantryTotals.Increment_Unit_Total(index); // RA2: as above
			break;
		}
		case 1: { // RA2: as above
			CurUnits++;
			int index = techno->Techno_Type_Class()->Fetch_Heap_ID(); // RA2: as above
			UQuantity.Increment(index); // RA2: as above
			UnitTotals.Increment_Unit_Total(index); // RA2: as above
			if (techno->Techno_Type_Class()->Naval && techno->Techno_Type_Class()->MaxPassengers == 0) { // RA2: as above
				OwnedNavy++; // RA2: as above
			}
			break;
		}
		}
	}
}




/***********************************************************************************************
 * HouseClass::Factory_Counter -- Fetches a pointer to the factory counter value.              *
 *                                                                                             *
 *    Use this routine to fetch a pointer to the variable that holds the number of factories   *
 *    that can produce the specified object type. This is a helper routine used when           *
 *    examining the number of factories as well as adjusting their number.                     *
 *                                                                                             *
 * INPUT:   rtti  -- The RTTI of the object that could be produced.                            *
 *                                                                                             *
 * OUTPUT:  Returns with the number of factories owned by this house that could produce the    *
 *          object of the type specified.                                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/30/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int * HouseClass::Factory_Counter(RTTIType rtti, bool naval) // RA2: 0x4EAB10, takes whether the unit is a ship
{
	switch (rtti) {
		case RTTI_UNITTYPE:
		case RTTI_UNIT:
			if (naval) { // RA2: ships are counted apart (the shipyards)
				return(&NumShipyards); // RA2: as above
			}
			return(&UnitFactories);

		case RTTI_AIRCRAFTTYPE:
		case RTTI_AIRCRAFT:
			return(&AircraftFactories);

		case RTTI_INFANTRYTYPE:
		case RTTI_INFANTRY:
			return(&InfantryFactories);

		case RTTI_BUILDINGTYPE:
		case RTTI_BUILDING:
			return(&BuildingFactories);

		default:
			break;
	}
	return(NULL);
}



/***********************************************************************************************
 * HouseClass::Active_Remove -- Remove this object from active duty for this house.            *
 *                                                                                             *
 *    This routine will recognize the specified object as having been removed from active      *
 *    duty.                                                                                    *
 *                                                                                             *
 * INPUT:   techno   -- Pointer to the object to remove from active duty.                      *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/16/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::Active_Remove(TechnoClass const * techno)
{
	if (techno->Fetch_RTTI() == RTTI_BUILDING) { // RA2: asks the virtual Fetch_RTTI
		TechnoTypeClass const * type = techno->Techno_Type_Class(); // RA2: the type tells whether the factory is a shipyard
		int * fptr = Factory_Counter(((BuildingClass *)techno)->Class->ToBuild, type != NULL && type->Naval); // RA2: as above
		if (fptr != NULL) {
			*fptr = *fptr - 1;
		}
	}
}



/***********************************************************************************************
 * HouseClass::Active_Add -- Add an object to active duty for this house.                      *
 *                                                                                             *
 *    This routine will recognize the specified object as having entered active duty. Any      *
 *    abilities granted to the house by that object are now available.                         *
 *                                                                                             *
 * INPUT:   techno   -- Pointer to the object that is entering active duty.                    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/16/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::Active_Add(TechnoClass const * techno)
{
	if (techno->Fetch_RTTI() == RTTI_BUILDING) { // RA2: asks the virtual Fetch_RTTI
		TechnoTypeClass const * type = techno->Techno_Type_Class(); // RA2: the type tells whether the factory is a shipyard
		int * fptr = Factory_Counter(((BuildingClass *)techno)->Class->ToBuild, type != NULL && type->Naval); // RA2: as above
		if (fptr != NULL) {
			*fptr = *fptr + 1;
		}
	}
}



/***********************************************************************************************
 * HouseClass::Which_Zone -- Determines what zone a coordinate lies in.                        *
 *                                                                                             *
 *    This routine will determine what zone the specified coordinate lies in with respect to   *
 *    this house's base. A location that is too distant from the base, even though it might    *
 *    be a building, is not considered part of the base and returns ZONE_NONE.                 *
 *                                                                                             *
 * INPUT:   coord -- The coordinate to examine.                                                *
 *                                                                                             *
 * OUTPUT:  Returns with the base zone that the specified coordinate lies in.                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/02/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ZoneType HouseClass::Which_Zone(Coord const & coord) const
{
	if (coord == COORD_NONE) return(ZONE_NONE);

	int distance = Distance(Center, coord);
	if (distance <= Radius) return(ZONE_CORE);
	if (distance > Radius*4) return(ZONE_NONE);

	Dir256 facing = Direction(Center, coord).As_Dir256();
	/// Both tests are inverted. ZONE_NORTH claims everything except the north wedge, and
	/// the ZONE_SOUTH arm can never be true at all.
	if (facing > DIR_NE && facing < DIR_NW) return(ZONE_NORTH);
	if (facing > DIR_SW && facing < DIR_SE) return(ZONE_SOUTH);
	if (facing > DIR_NE || facing >= DIR_SE) return(ZONE_WEST);
	return(ZONE_EAST);
}


/***********************************************************************************************
 * HouseClass::Which_Zone -- Determines which base zone the specified object lies in.          *
 *                                                                                             *
 *    Use this routine to determine what zone the specified object lies in.                    *
 *                                                                                             *
 * INPUT:   object   -- Pointer to the object that will be checked for zone occupation.        *
 *                                                                                             *
 * OUTPUT:  Returns with the base zone that the object lies in. For objects that are too       *
 *          distant from the center of the base, ZONE_NONE is returned.                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/02/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ZoneType HouseClass::Which_Zone(ObjectClass const * object) const
{
	if (!object) return(ZONE_NONE);
	return(Which_Zone(object->Center_Coord()));
}


/***********************************************************************************************
 * HouseClass::Which_Zone -- Determines which base zone the specified cell lies in.            *
 *                                                                                             *
 *    This routine is used to determine what base zone the specified cell is in.               *
 *                                                                                             *
 * INPUT:   cell  -- The cell to examine.                                                      *
 *                                                                                             *
 * OUTPUT:  Returns the base zone that the cell lies in or ZONE_NONE if the cell is too far    *
 *          away.                                                                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/02/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
ZoneType HouseClass::Which_Zone(Cell const & cell) const
{
	return(Which_Zone((Coord)cell));
}


/***********************************************************************************************
 * HouseClass::Zone_Cell -- Finds the cell closest to the center of the zone.                  *
 *                                                                                             *
 *    This routine is used to find the cell that is closest to the center point of the         *
 *    zone specified. Typical use of this routine is for building and unit placement so that   *
 *    they can "cover" the specified zone.                                                     *
 *                                                                                             *
 * INPUT:   zone  -- The zone that the center point is to be returned.                         *
 *                                                                                             *
 * OUTPUT:  Returns with the cell that is closest to the center point of the zone specified.   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/02/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
Cell HouseClass::Zone_Cell(ZoneType zone) const
{
	switch (zone) {
		case ZONE_CORE:
			return(Center.As_Cell());

		case ZONE_NORTH:
			return(Move_Coord(Center, DIR_N, Radius*3).As_Cell());

		case ZONE_EAST:
			return(Move_Coord(Center, DIR_E, Radius*3).As_Cell());

		case ZONE_WEST:
			return(Move_Coord(Center, DIR_W, Radius*3).As_Cell());

		case ZONE_SOUTH:
			return(Move_Coord(Center, DIR_S, Radius*3).As_Cell());

		default:
			break;
	}
	return(CELL_NONE);
}


/***********************************************************************************************
 * HouseClass::Where_To_Go -- Determines where the object should go and wait.                  *
 *                                                                                             *
 *    This function is called for every new unit produced or delivered in order to determine   *
 *    where the unit should "hang out" to await further orders. The best area for the          *
 *    unit to loiter is returned as a cell location.                                           *
 *                                                                                             *
 * INPUT:   object   -- Pointer to the object that needs to know where to go.                  *
 *                                                                                             *
 * OUTPUT:  Returns with the cell that the unit should move to.                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/02/1995 JLB : Created.                                                                 *
 *   11/04/1996 JLB : Simplified to use helper functions                                       *
 *=============================================================================================*/
Cell HouseClass::Where_To_Go(FootClass const * object) const
{
	assert(object != NULL);

	ZoneType zone;			// The zone that the object should go to.
	if (object->Anti_Air() + object->Anti_Armor() + object->Anti_Infantry() == 0) {
		zone = ZONE_CORE;
	} else {
		zone = Random_Pick(ZONE_NORTH, ZONE_WEST);
	}

	Cell cell = Random_Cell_In_Zone(zone);
	assert(cell != CELL_NONE);

	return(Map.Nearby_Location(cell, SPEED_TRACK, Map.Get_Cell_Zone(object->PositionCoord.As_Cell())));
}


/***********************************************************************************************
 * HouseClass::Find_Juicy_Target -- Finds a suitable field target.                             *
 *                                                                                             *
 *    This routine is used to find targets out in the field and away from base defense.        *
 *    Typical of this would be the attack helicopters and the roving attack bands of           *
 *    hunter killers.                                                                          *
 *                                                                                             *
 * INPUT:   coord -- The coordinate of the attacker. Closer targets are given preference.      *
 *                                                                                             *
 * OUTPUT:  Returns with a suitable target to attack.                                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/12/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
AbstractClass * HouseClass::Find_Juicy_Target(Coord const & coord) const
{
	UnitClass * best = 0;
	int value = 0;
	Cell center = coord.As_Cell(); // RA2: the units are looked for on the cells around the coordinate, not in the house's unit list

	int count = MapClass::RadiusCount[11]; // RA2: the scan covers the offsets of the whole RadiusOffset table (the entry after radius 10 is its total)
	for (int index = 0; index < count; index++) { // RA2: as above
		UnitClass * unit = Map[center + MapClass::RadiusOffset[index]].Cell_Unit(true); // RA2: as above

		if (unit && !unit->IsInLimbo && !Is_Ally(unit) && unit->House->Which_Zone(unit) == ZONE_NONE && unit->HeightAGL >= -20 && unit->Cloak != CLOAKED) {
			int val = coord.Distance_To(unit->Center_Coord());

			if (unit->Anti_Air()) val *= 2;

			if (value == 0 || val < value) {
				value = val;
				best = unit;
			}
		}
	}
	if (best) {
		return(best);
	}
	return(NULL);
}




/***********************************************************************************************
 * HouseClass::Fetch_Factory -- Finds the factory associated with the object type specified.   *
 *                                                                                             *
 *    This is the counterpart to the Set_Factory function. It will return with a factory       *
 *    pointer that is associated with the object type specified.                               *
 *                                                                                             *
 * INPUT:   rtti  -- The RTTI of the object type to find the factory for.                      *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the factory (if present) that can manufacture the        *
 *          object type specified.                                                             *
 *                                                                                             *
 * WARNINGS:   If this returns a non-NULL pointer, then the factory is probably already busy   *
 *             producing another unit of that category.                                        *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
/// <summary>
/// Fetches the build category of an object type; only buildings have one.
/// </summary>
/// <param name="type">The kind of object type.</param>
/// <param name="id">The heap ID of the object type.</param>
/// <returns>Returns with the category (5 is the defense category), or 0 for any other type.</returns>
/// <summary>
/// Fetches the build category of an object type; only buildings have one.
/// </summary>
/// <param name="type">The kind of object type.</param>
/// <param name="id">The heap ID of the object type.</param>
/// <returns>Returns with the category (5 is the defense category), or 0 for any other type.</returns>
int HouseClass::Build_Category(RTTIType type, int id) // RA2: 0x4EB550; not in TibSun
{
	if (id < 0) { // RA2: as above
		return(0); // RA2: as above
	}
	TechnoTypeClass const * ttype = Fetch_Techno_Type(type, id); // RA2: as above
	if (type == RTTI_BUILDING || type == RTTI_BUILDINGTYPE) { // RA2: as above
		return(((BuildingTypeClass const *)ttype)->BuildCat); // RA2: as above
	}
	return(0); // RA2: as above
}



/// <summary>
/// Fetches the factory that makes an object of the given kind.
/// </summary>
/// <param name="rtti">The kind of object.</param>
/// <param name="naval">Whether the object is a naval one.</param>
/// <param name="category">The build category of the object (5 is the defense category).</param>
/// <returns>Returns with the factory, or NULL if there is none.</returns>
FactoryClass * HouseClass::Fetch_Factory(RTTIType rtti, bool naval, int category) const // RA2: 0x4EB580; the naval and defense factories apart
{
	FactoryClass * factory; // RA2: as above

	switch (rtti) { // RA2: as above
		case RTTI_INFANTRY: // RA2: as above
		case RTTI_INFANTRYTYPE: // RA2: as above
			factory = InfantryFactory; // RA2: as above
			break; // RA2: as above

		case RTTI_UNIT: // RA2: as above
		case RTTI_UNITTYPE: // RA2: as above
			if (naval) { // RA2: as above
				factory = Primary_ForShips; // RA2: as above
			} else { // RA2: as above
				factory = UnitFactory; // RA2: as above
			} // RA2: as above
			break; // RA2: as above

		case RTTI_BUILDING: // RA2: as above
		case RTTI_BUILDINGTYPE: // RA2: as above
			if (category == 5) { // RA2: as above
				factory = Primary_ForDefenses; // RA2: as above
			} else { // RA2: as above
				factory = BuildingFactory; // RA2: as above
			} // RA2: as above
			break; // RA2: as above

		case RTTI_AIRCRAFT: // RA2: as above
		case RTTI_AIRCRAFTTYPE: // RA2: as above
			factory = AircraftFactory; // RA2: as above
			break; // RA2: as above

		default: // RA2: as above
			factory = NULL; // RA2: as above
			break; // RA2: as above
	}
	return(factory); // RA2: as above
}




/// <summary>
/// Marks the production of an object kind as pending.
/// </summary>
/// <param name="type">The kind of object.</param>
/// <param name="naval">Whether the object is a naval one.</param>
/// <param name="category">The build category of the object (5 is the defense category).</param>
/// <summary>
/// Marks the production of an object kind as pending.
/// </summary>
/// <param name="type">The kind of object.</param>
/// <param name="naval">Whether the object is a naval one.</param>
/// <param name="category">The build category of the object (5 is the defense category).</param>
void HouseClass::Set_Production_Pending(RTTIType type, bool naval, int category) // RA2: 0x4EB620; not in TibSun
{
	switch (type) { // RA2: as above
		case RTTI_INFANTRY: // RA2: as above
		case RTTI_INFANTRYTYPE: // RA2: as above
			InfantryProductionPending = true; // RA2: as above
			// fall through // RA2: as above
		case RTTI_UNIT: // RA2: as above
		case RTTI_UNITTYPE: // RA2: as above
			if (naval) { // RA2: as above
				ShipProductionPending = true; // RA2: as above
			} else { // RA2: as above
				UnitProductionPending = true; // RA2: as above
			} // RA2: as above
			break; // RA2: as above

		case RTTI_BUILDING: // RA2: as above
		case RTTI_BUILDINGTYPE: // RA2: as above
			if (category == 5) { // RA2: as above
				DefenseProductionPending = true; // RA2: as above
			} else { // RA2: as above
				BuildingProductionPending = true; // RA2: as above
			} // RA2: as above
			break; // RA2: as above

		case RTTI_AIRCRAFT: // RA2: as above
		case RTTI_AIRCRAFTTYPE: // RA2: as above
			AircraftProductionPending = true; // RA2: as above
			break; // RA2: as above
	}
}



/// <summary>
/// Marks the production of an object kind as no longer pending.
/// </summary>
/// <param name="type">The kind of object.</param>
/// <param name="naval">Whether the object is a naval one.</param>
/// <param name="category">The build category of the object (5 is the defense category).</param>
/// <summary>
/// Marks the production of an object kind as no longer pending.
/// </summary>
/// <param name="type">The kind of object.</param>
/// <param name="naval">Whether the object is a naval one.</param>
/// <param name="category">The build category of the object (5 is the defense category).</param>
void HouseClass::Clear_Production_Pending(RTTIType type, bool naval, int category) // RA2: 0x4EB6D0; not in TibSun
{
	switch (type) { // RA2: as above
		case RTTI_INFANTRY: // RA2: as above
		case RTTI_INFANTRYTYPE: // RA2: as above
			InfantryProductionPending = false; // RA2: as above
			// fall through // RA2: as above
		case RTTI_UNIT: // RA2: as above
		case RTTI_UNITTYPE: // RA2: as above
			if (naval) { // RA2: as above
				ShipProductionPending = false; // RA2: as above
			} else { // RA2: as above
				UnitProductionPending = false; // RA2: as above
			} // RA2: as above
			break; // RA2: as above

		case RTTI_BUILDING: // RA2: as above
		case RTTI_BUILDINGTYPE: // RA2: as above
			if (category == 5) { // RA2: as above
				DefenseProductionPending = false; // RA2: as above
			} else { // RA2: as above
				BuildingProductionPending = false; // RA2: as above
			} // RA2: as above
			break; // RA2: as above

		case RTTI_AIRCRAFT: // RA2: as above
		case RTTI_AIRCRAFTTYPE: // RA2: as above
			AircraftProductionPending = false; // RA2: as above
			break; // RA2: as above
	}
}



/// <summary>
/// Checks whether the production of an object kind is pending.
/// </summary>
/// <param name="type">The kind of object.</param>
/// <param name="naval">Whether the object is a naval one.</param>
/// <param name="category">The build category of the object (5 is the defense category).</param>
/// <summary>
/// Checks whether the production of an object kind is pending.
/// </summary>
/// <param name="type">The kind of object.</param>
/// <param name="naval">Whether the object is a naval one.</param>
/// <param name="category">The build category of the object (5 is the defense category).</param>
bool HouseClass::Is_Production_Pending(RTTIType type, bool naval, int category) // RA2: 0x4EB770; not in TibSun
{
	switch (type) { // RA2: as above
		case RTTI_INFANTRY: // RA2: as above
		case RTTI_INFANTRYTYPE: // RA2: as above
			return(InfantryProductionPending); // RA2: as above
			// fall through // RA2: as above
		case RTTI_UNIT: // RA2: as above
		case RTTI_UNITTYPE: // RA2: as above
			if (naval) { // RA2: as above
				return(ShipProductionPending); // RA2: as above
			} else { // RA2: as above
				return(UnitProductionPending); // RA2: as above
			} // RA2: as above
			break; // RA2: as above

		case RTTI_BUILDING: // RA2: as above
		case RTTI_BUILDINGTYPE: // RA2: as above
			if (category == 5) { // RA2: as above
				return(DefenseProductionPending); // RA2: as above
			} else { // RA2: as above
				return(BuildingProductionPending); // RA2: as above
			} // RA2: as above
			break; // RA2: as above

		case RTTI_AIRCRAFT: // RA2: as above
		case RTTI_AIRCRAFTTYPE: // RA2: as above
			return(AircraftProductionPending); // RA2: as above
			break; // RA2: as above
	}
	return(false); // RA2: as above
}



/// <summary>
/// Sets whether the production of an object kind is pending.
/// </summary>
/// <param name="type">The kind of object.</param>
/// <param name="naval">Whether the object is a naval one.</param>
/// <param name="category">The build category of the object (5 is the defense category).</param>
/// <param name="value">The new state.</param>
/// <summary>
/// Sets whether the production of an object kind is pending.
/// </summary>
/// <param name="type">The kind of object.</param>
/// <param name="naval">Whether the object is a naval one.</param>
/// <param name="category">The build category of the object (5 is the defense category).</param>
/// <param name="value">The new state.</param>
// Never expanded inline: the executable has no inlined copy of these flag stores, and Begin_Production calls it.
#pragma auto_inline(off)
void HouseClass::Assign_Production_Pending(RTTIType type, bool naval, int category, bool value) // RA2: 0x4EB810; not in TibSun
{
	switch (type) { // RA2: as above
		case RTTI_INFANTRY: // RA2: as above
		case RTTI_INFANTRYTYPE: // RA2: as above
			InfantryProductionPending = value; // RA2: as above
			// fall through // RA2: as above
		case RTTI_UNIT: // RA2: as above
		case RTTI_UNITTYPE: // RA2: as above
			if (naval) { // RA2: as above
				ShipProductionPending = value; // RA2: as above
			} else { // RA2: as above
				UnitProductionPending = value; // RA2: as above
			} // RA2: as above
			break; // RA2: as above

		case RTTI_BUILDING: // RA2: as above
		case RTTI_BUILDINGTYPE: // RA2: as above
			if (category == 5) { // RA2: as above
				DefenseProductionPending = value; // RA2: as above
			} else { // RA2: as above
				BuildingProductionPending = value; // RA2: as above
			} // RA2: as above
			break; // RA2: as above

		case RTTI_AIRCRAFT: // RA2: as above
		case RTTI_AIRCRAFTTYPE: // RA2: as above
			AircraftProductionPending = value; // RA2: as above
			break; // RA2: as above
	}
}
#pragma auto_inline(on)




/***********************************************************************************************
 * HouseClass::Set_Factory -- Assign specified factory to house tracking.                      *
 *                                                                                             *
 *    Call this routine when a factory has been created and it now must be passed on to the    *
 *    house for tracking purposes. The house maintains several factory pointers and this       *
 *    routine will ensure that the factory pointer gets stored correctly.                      *
 *                                                                                             *
 * INPUT:   rtti  -- The RTTI of the object the factory it to manufacture.                     *
 *                                                                                             *
 *          factory  -- The factory object pointer.                                            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::Set_Factory(FactoryClass * factory, RTTIType rtti, bool naval, int category) // RA2: 0x4EB8C0, argument order as the executable; ships and defenses have their own primary factory
{
	switch (rtti) {
		case RTTI_UNIT:
		case RTTI_UNITTYPE:
			if (naval) { // RA2: as above
				Primary_ForShips = factory; // RA2: as above
			} else { // RA2: as above
				UnitFactory = factory; // RA2: as above
			}
			break;

		case RTTI_INFANTRY:
		case RTTI_INFANTRYTYPE:
			InfantryFactory = factory;
			break;

		case RTTI_BUILDING:
		case RTTI_BUILDINGTYPE:
			if (category == 5) { // RA2: as above
				Primary_ForDefenses = factory; // RA2: as above
			} else { // RA2: as above
				BuildingFactory = factory; // RA2: as above
			}
			break;

		case RTTI_AIRCRAFT:
		case RTTI_AIRCRAFTTYPE:
			AircraftFactory = factory;
			break;
	}
}



/***********************************************************************************************
 * HouseClass::Factory_Count -- Fetches the number of factories for specified type.            *
 *                                                                                             *
 *    This routine will count the number of factories owned by this house that can build       *
 *    objects of the specified type.                                                           *
 *                                                                                             *
 * INPUT:   rtti  -- The type of object (RTTI) that the factories are to be counted for.       *
 *                                                                                             *
 * OUTPUT:  Returns with the number of factories that can build the object type specified.     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/30/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int HouseClass::Factory_Count(RTTIType rtti, bool naval) const // RA2: 0x4EB980, takes whether the unit is a ship
{
	int const * ptr = ((HouseClass *)this)->Factory_Counter(rtti, naval); // RA2: as above
	if (ptr != NULL) {
		return(*ptr);
	}
	return(0);
}



/// <summary>
/// Creates and initializes every house listed in the INI database.
/// This routine is called as a scenario starts up. Each house is created, given its scenario
/// data, and -- in a solo mission -- handicapped according to the chosen difficulty.
/// </summary>
/// <param name="ini">The INI database to read the houses from.</param>
void HouseClass::Read_All(CCINIClass const & ini)
{
	int count = ini.Entry_Count("Houses");
	for (int index = 0; index < count; index++) { // RA2: RA2's form
		char name[20]; // RA2: as above
		ini.Get_String("Houses", ini.Get_Entry("Houses", index), "", name, sizeof(name)); // RA2: as above
		int type = ini.Get_HousesType(name, "Country", HOUSE_NONE); // RA2: as above
		if (type == -1) { // RA2: as above
			type = 0; // RA2: as above
		}
		HouseClass *house = new HouseClass(HouseTypes[type]); // RA2: as above
		house->IniName = name; // RA2: as above
	}

	for (int i = 0; i < Houses.Count(); i++) { // RA2: as above
		HouseClass *house = Houses[i]; // RA2: as above
		house->Read_INI(ini); // RA2: as above
		if (!Session.Type && (house->IsHuman || house->IsPlayerControl)) { // RA2: as above
			house->Assign_Handicap(Scen->Difficulty); // RA2: as above
		} else if (!Session.Type) { // RA2: as above
			house->Assign_Handicap(Scen->CDifficulty); // RA2: as above
		}
	}

	if (Houses.Count() == 0) { // RA2: as above
		for (int type = 0; type < HouseTypes.Count(); type++) { // RA2: as above
			HouseClass *house = new HouseClass(HouseTypes[type]); // RA2: as above
			house->Read_INI(ini); // RA2: as above
		}
	}
}



/***********************************************************************************************
 * HouseClass::Read_INI -- Reads house specific data from INI.                                 *
 *                                                                                             *
 *    This routine reads the house specific data for a particular                              *
 *    scenario from the scenario INI file. Typical data includes starting                      *
 *    credits, maximum unit count, etc.                                                        *
 *                                                                                             *
 * INPUT:   buffer   -- Pointer to loaded scenario INI file.                                   *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/24/1994 JLB : Created.                                                                 *
 *   05/18/1995 JLB : Creates all houses.                                                      *
 *=============================================================================================*/
void HouseClass::Read_INI(CCINIClass const & ini) // RA2: the body follows the executable
{
	char section[20]; // RA2: the house's own name is copied to a local first
	strcpy(section, IniName); // RA2: as above

	Control.TechLevel = ini.Get_Int(section, "TechLevel", Scen->Scenario); // RA2: as TibSun, but keyed by the house's own name
	Control.InitialCredits = ini.Get_Int(section, "Credits", 0) * 100; // RA2: as above
	IsPlayerControl = ini.Get_Bool(section, "PlayerControl", false); // RA2: read before the credits are set
	if (IsPlayerControl && Session.Type == GAME_NORMAL) { // RA2: a campaign human's starting credits follow the difficulty
		if (Options.Difficulty == 0) { // RA2: as above
			Credits = Rule->CampaignMoneyDeltaEasy + Control.InitialCredits; // RA2: as above
		} else if (Options.Difficulty == 2) { // RA2: as above
			Credits = Rule->CampaignMoneyDeltaHard + Control.InitialCredits; // RA2: as above
		} else {
			Credits = Control.InitialCredits; // RA2: as above
		}
		Credits = Credits > 0 ? Credits : 0; // RA2: never negative
	} else {
		Credits = Control.InitialCredits; // RA2: as above
	}

	char buffer[100]; // RA2: the display name label is read here
	ini.Get_String(section, "UIName", UINameString, buffer, UINameString.Size()); // RA2: as abstype.cpp
	UINameString = TStringID<31>(buffer); // RA2: as above
	if (UINameString[0] != '\0') { // RA2: as above
		wcsncpy(UIName, StringTable::LoadString(UINameString, NULL, "D:\\Ra2\\House.CPP", 7898), 32); // RA2: the label is looked up in the string table
		UIName[20] = L'\0'; // RA2: as above
	} else {
		wcscpy(UIName, Class->UIName); // RA2: the house type's own name
	}

	RatioAITriggerTeam = ini.Get_Int(section, "RatioAITriggerTeam", RatioAITriggerTeam); // RA2: as above
	RatioTeamAircraft = ini.Get_Int(section, "RatioTeamAircraft", 75); // RA2: as above
	RatioTeamInfantry = ini.Get_Int(section, "RatioTeamInfantry", 75); // RA2: as above
	RatioTeamUnits = ini.Get_Int(section, "RatioTeamUnits", 75); // RA2: as above

	ActLike = (HousesType)Class->Side; // RA2: the side of the house type, no ActsLike key
	if (ActLike == HOUSE_NONE) { // RA2: as above
		ActLike = HOUSE_FIRST; // RA2: as above
	}

	int iq = ini.Get_Int(section, "IQ", 0); // RA2: as above
	if (iq > Rule->MaxIQ) iq = 1; // RA2: as above
	Control.IQ = iq; // RA2: stored in the control block first
	IQ = iq; // RA2: as above

	Control.Edge = ini.Get_SourceType(section, "Edge", SOURCE_NORTH); // RA2: as TibSun
	Scheme = ini.Get_Scheme_Index(section, "Color", Scheme); // RA2: as above
	Initialize_Radar_Color(); // RA2: the remap color follows the scheme
	Update_Laser_Color(); // RA2: and the laser color follows the remap color

	AIGeneral = NULL; // RA2: as above
	Make_Ally(Houses[HeapID], false); // RA2: a house allies itself first, without announcing
	int owners = ini.Get_Houses(section, "Allies", 0); // RA2: the allies are named by house number
	for (int index = 0; index < Houses.Count(); index++) { // RA2: as above
		HouseClass * hptr = Houses[index]; // RA2: as above
		if (owners & (1 << hptr->HeapID)) { // RA2: as above
			Make_Ally(hptr, false); // RA2: as above
		}
	}

	TeamTime = Rule->TeamDelays[Difficulty] + HeapID * 175; // RA2: the delay is added to the 175 frame offset in this order
	Base.Read_INI(ini, section); // RA2: as above
	Base.House = this; // RA2: as above
}



/// <summary>
/// Writes every house out to the INI database.
/// This routine records the roster of houses and then lets each one write its own scenario
/// data. It is the counterpart of Read_All.
/// </summary>
/// <param name="ini">The INI database to write the houses to.</param>
void HouseClass::Write_All(CCINIClass & ini)
{
	ini.Clear("Houses", 0); // RA2: RA2's form
	for (int index = 0; index < Houses.Count(); index++) { // RA2: as above
		char name[32]; // RA2: as above
		char entry[32]; // RA2: as above
		strcpy(name, Houses[index]->IniName); // RA2: as above
		sprintf(entry, "%d", index); // RA2: as above
		ini.Put_String("Houses", entry, name); // RA2: as above
	}
	for (int i = 0; i < Houses.Count(); i++) { // RA2: as above
		Houses[i]->Write_INI(ini); // RA2: as above
	}
}



/***********************************************************************************************
 * HouseClass::Write_INI -- Writes the house data to the INI database.                         *
 *                                                                                             *
 *    This routine will write out all data necessary to recreate it in anticipation of a       *
 *    new scenario. All houses (that are active) will have their scenario type data written    *
 *    out.                                                                                     *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database to write the data to.                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::Write_INI(CCINIClass & ini) // RA2: the body follows the executable
{
	char section[20]; // RA2: the section name is copied to a local first
	strcpy(section, IniName); // RA2: the house's own name, not its type's

	ini.Clear(section); // RA2: the keys are written to the local section copy, in the executable's order

	ini.Put_String(section, "Country", Class->IniName); // RA2: the name of the house's country type
	ini.Put_Int(section, "TechLevel", Control.TechLevel); // RA2: as above
	ini.Put_Int(section, "Credits", Control.InitialCredits / 100); // RA2: as above
	ini.Put_Int(section, "IQ", Control.IQ); // RA2: as above
	ini.Put_SourceType(section, "Edge", Control.Edge); // RA2: as above
	ini.Put_Bool(section, "PlayerControl", IsPlayerControl); // RA2: as above
	ini.Put_Scheme_Index(section, "Color", Scheme); // RA2: the color scheme is written, no ActsLike, AI or Side key

	int allies = 0; // RA2: as the executable
	for (int index = 0; index < Houses.Count(); index++) { // RA2: the allied houses are written by house number
		HouseClass * hptr = Houses[index]; // RA2: as above
		if (Control.Allies & (1 << hptr->HeapID)) { // RA2: as above
			allies |= 1 << hptr->HeapID; // RA2: as above
		}
	}
	ini.Put_Houses(section, "Allies", allies); // RA2: as above

	if (UINameString[0] != '\0') { // RA2: the name shown to the player is written when there is one
		ini.Put_String(section, "UIName", UINameString); // RA2: as above
	}
	Base.Write_INI(ini, section); // RA2: as above
}





/***********************************************************************************************
 * HouseClass::Fire_Sale -- Cause all buildings to be sold.                                    *
 *                                                                                             *
 *    This routine will sell back all buildings owned by this house.                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was a fire sale performed?                                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/23/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
HRESULT STDMETHODCALLTYPE HouseClass::Fire_Sale(void)
{
	if (CurBuildings > 0) {
		int count = Buildings.Count(); // RA2: RA2's form
		for (int index = 0; index < count; index++) { // RA2: as above
			BuildingClass *building = (BuildingClass *)Buildings[index]; // RA2: as above
			if (building != 0 && !building->IsInLimbo && building->Strength > 0) { // RA2: as above
				building->Sell_Back(1); // RA2: as above
			}
		}
		return S_OK; // RA2: as above
	}
	return(1);   // S_FALSE: nothing to sell // RA2: as above
}



/***********************************************************************************************
 * HouseClass::Do_All_To_Hunt -- Send all units to hunt.                                       *
 *                                                                                             *
 *    This routine will cause all combatants of this house to go into hunt mode. The effect of *
 *    this is to throw everything this house has to muster at the enemies of this house.       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/23/1996 JLB : Created.                                                                 *
 *   10/02/1996 JLB : Handles aircraft too.                                                    *
 *=============================================================================================*/
HRESULT STDMETHODCALLTYPE HouseClass::All_To_Hunt(void)
{
	for (int index = Technos.Count() - 1; index >= 0; index--) { // RA2: RA2's form
		TechnoClass *techno = Technos[index]; // RA2: as above
		if (this == techno->House && techno->IsDown && !techno->IsInLimbo && techno != 0) { // RA2: as above
			bool is_foot = techno->Is_Foot(); // RA2: as above
			if (is_foot) { // RA2: as above
				FootClass *foot = (FootClass *)techno; // RA2: as above
				if (foot->Team != 0) { // RA2: as above
					foot->Team->Remove(foot, -1, 0); // RA2: as above
				}
				foot->QueueMission(MISSION_HUNT, false); // RA2: as above
			}
			if (!is_foot && techno->Fetch_RTTI() == RTTI_BUILDING) { // RA2: as above
				BuildingClass *building = (BuildingClass *)techno; // RA2: as above
				if (building->GetOccupantCount() > 0) { // RA2: as above
					building->Eject_Occupants(1); // RA2: as above
				}
			}
		}
	}
	IsAllToHunt = true;
	return S_OK; // RA2: as above
}



/***********************************************************************************************
 * HouseClass::Is_Allowed_To_Ally -- Determines if this house is allied to make allies.        *
 *                                                                                             *
 *    Use this routine to determine if this house is legally allowed to ally with the          *
 *    house specified. There are many reason why an alliance is not allowed. Typically, this   *
 *    is when there would be no more opponents left to fight or if this house has been         *
 *    defeated.                                                                                *
 *                                                                                             *
 * INPUT:   house -- The house that alliance with is desired.                                  *
 *                                                                                             *
 * OUTPUT:  bool; Is alliance with the house specified prohibited?                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/23/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool HouseClass::Is_Allowed_To_Ally(HouseClass * house) const
{
	if (house != NULL) {
		/*
		**	One cannot ally twice with the same house.
		*/
		if (Is_Ally(house)) {
			return(false);
		}
	}

	if (house->Class->Side == SideClass::From_Name("Civilian") && Session.Type != GAME_NORMAL) { // RA2: outside the campaign nobody allies with the civilians
		return(false); // RA2: as above
	}

	/*
	**	If the scenario is being set up, then alliances are always
	**	allowed. No further checking is required.
	*/
	if (ScenarioInit) {
		return(true);
	}

	/*
	**	When the house is defeated, it can no longer make alliances.
	*/
	if (IsDefeated) {
		return(false);
	}

	/*
	**	Count the number of active houses in the game as well as the
	**	number of existing allies with this house.
	*/
	int housecount = 0;
	int allycount = 0;
	for (HousesType house2 = HOUSE_FIRST; house2 < Houses.Count(); house2++) {
		HouseClass * hptr = Houses[house2];
		if (!hptr->IsDefeated && !hptr->Class->IsMultiplayPassive) {
			housecount++;
			if (Is_Ally(hptr)) {
				allycount++;
			}
		}
	}

	/*
	**	Alliance is not allowed if there wouldn't be any enemies left to
	**	fight.
	*/
	if (housecount == allycount+1) {
		return(false);
	}

	return(true);
}



/***********************************************************************************************
 * HouseClass::Computer_Paranoid -- Cause the computer players to becom paranoid.              *
 *                                                                                             *
 *    This routine will cause the computer players to become suspicious of the human           *
 *    players and thus the computer players will band together in order to defeat the          *
 *    human players.                                                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/23/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::Computer_Paranoid(void)
{
	bool forced = Rule->AlliesAllowed; // RA2: the rules flag is read first
	if ((Session.MPGameMode != NULL && Session.MPGameMode->vt_entry_04()) || forced) { // RA2: only when the game mode asks for it or the rules force it

	/*
	**	Loop through every computer controlled house and make allies with all other computer
	**	controlled houses and then make enemies with all other human controlled houses.
	*/
	for (HousesType house = HOUSE_FIRST; house < Houses.Count(); house++) {
		HouseClass * hptr = Houses[house];
		if (hptr != NULL && !hptr->IsDefeated && !hptr->Is_Human_Player()) {
			hptr->IsParanoid = true;

			/*
			**	Break alliance with every human it is allied with and make friends with
			**	any other computer players.
			*/
			for (HousesType house2 = HOUSE_FIRST; house2 < Houses.Count(); house2++) {
				HouseClass * hptr2 = Houses[house2];
				if (hptr2 != NULL && !hptr2->IsDefeated) {
					if (hptr2->Is_Human_Player()) {
						hptr->Make_Enemy(hptr2, false); // RA2: the announce flag is false
					} else {
						hptr->Make_Ally(hptr2, false); // RA2: as above
					}
				}
			}
		}
	}

	}
}



/***********************************************************************************************
 * HouseClass::Adjust_Power -- Adjust the power value of the house.                            *
 *                                                                                             *
 *    This routine will update the power output value of the house. It will cause any buildgins*
 *    that need to be redrawn to do so.                                                        *
 *                                                                                             *
 * INPUT:   adjust   -- The amount to adjust the power output value.                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/01/1996 BWG : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::Adjust_Power(int adjust)
{
	if (!GameActive) return;

	bool waslost = Power < Drain;

	Power += adjust;
	Update_Spied_Power_Plants();

	bool islost = Power < Drain;

	if (!ScenarioInit && waslost != islost) {

		IConnectionPointContainer *pContainer;
		QueryInterface(IID_IConnectionPointContainer, (void**)&pContainer);

		IEnumConnectionPoints *ppEnumConnectionPoints;
		pContainer->EnumConnectionPoints(&ppEnumConnectionPoints);

		IConnectionPoint *pConnectionPoint;
		while (SUCCEEDED(ppEnumConnectionPoints->Next(1, &pConnectionPoint, NULL))) {

			IID iid;
			pConnectionPoint->GetConnectionInterface(&iid);
			if (iid != IID_IPowerEvents) {
				pConnectionPoint->Release();
				continue;
			}

			CONNECTDATA rgpcd;
			IEnumConnections *ppEnumConnections;
			pConnectionPoint->EnumConnections(&ppEnumConnections);
			while (SUCCEEDED(ppEnumConnections->Next(1, &rgpcd, NULL))) {
				if (islost) {
					((IPowerEvents *)(rgpcd.pUnk))->Power_Lost();
				} else {
					((IPowerEvents *)(rgpcd.pUnk))->Power_Activated();
				}
				((IPowerEvents *)(rgpcd.pUnk))->Release();
			}

			ppEnumConnections->Release();
			break;
		}

		pContainer->Release();
		ppEnumConnectionPoints->Release();
	}
}


/***********************************************************************************************
 * HouseClass::Adjust_Drain -- Adjust the power drain value of the house.                      *
 *                                                                                             *
 *    This routine will update the drain value of the house. It will cause any buildings that  *
 *    need to be redraw to do so.                                                              *
 *                                                                                             *
 * INPUT:   adjust   -- The amount to adjust the drain (positive means more drain).            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/01/1996 BWG : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::Adjust_Drain(int adjust)
{
	bool waslost = Power < Drain;

	Drain += adjust;
	Update_Spied_Power_Plants();

	bool islost = Power < Drain;

	if (!ScenarioInit && waslost != islost) {

		IConnectionPointContainer *pContainer;
		QueryInterface(IID_IConnectionPointContainer, (void**)&pContainer);

		IEnumConnectionPoints *ppEnumConnectionPoints;
		pContainer->EnumConnectionPoints(&ppEnumConnectionPoints);

		IConnectionPoint *pConnectionPoint;
		while (SUCCEEDED(ppEnumConnectionPoints->Next(1, &pConnectionPoint, NULL))) {

			IID iid;
			pConnectionPoint->GetConnectionInterface(&iid);
			if (iid != IID_IPowerEvents) {
				pConnectionPoint->Release();
				continue;
			}

			CONNECTDATA rgpcd;
			IEnumConnections *ppEnumConnections;
			pConnectionPoint->EnumConnections(&ppEnumConnections);
			while (SUCCEEDED(ppEnumConnections->Next(1, &rgpcd, NULL))) {
				IPowerEvents *iface = (IPowerEvents *)rgpcd.pUnk;
				if (islost) {
					iface->Power_Lost();
				} else {
					iface->Power_Activated();
				}
				iface->Release();

			}

			ppEnumConnections->Release();
			break;
		}

		pContainer->Release();
		ppEnumConnectionPoints->Release();
	}
}


/***********************************************************************************************
 * HouseClass::Update_Spied_Power_Plants -- Redraw power graphs on spied-upon power plants.    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/11/1996 BWG : Created.                                                                 *
 *=============================================================================================*/
void HouseClass::Update_Spied_Power_Plants(void)
{
	int count = CurrentObject.Count();
	if (count) {
		for (int index = 0; index < count; index++) {
			ObjectClass const * tech = CurrentObject[index];
			if (tech && tech->RTTI==RTTI_BUILDING) {
				BuildingClass *bldg = (BuildingClass *)tech;
				if (!bldg->IsOwnedByPlayer && bldg->Class->Power > 0) {
					if ( bldg->SpiedBy & (1<<(PlayerPtr->Class->House)) ) {
						bldg->Mark(MARK_CHANGE);
					}
				}
			}
		}
	}
}


/***********************************************************************************************
 * HouseClass::Find_Cell_In_Zone -- Finds a legal placement cell within the zone.              *
 *                                                                                             *
 *    Use this routine to determine where the specified object should go if it were to go      *
 *    some random (but legal) location within the zone specified.                              *
 *                                                                                             *
 * INPUT:   techno   -- The object that is desirous of going into the zone specified.          *
 *                                                                                             *
 *          zone     -- The zone to find a location within.                                    *
 *                                                                                             *
 * OUTPUT:  Returns with the cell that the specified object could be placed in the zone. If    *
 *          no valid location could be found, then 0 is returned.                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/01/1996 JLB : Created.                                                                 *
 *   11/04/1996 JLB : Not so strict on zone requirement.                                       *
 *=============================================================================================*/
Cell HouseClass::Find_Cell_In_Zone(TechnoClass const * techno, ZoneType zone) const
{
	return(Cell(0, 0));
}


/// <summary>
/// Rotates a direction back by one 45 degree facing.
/// The position within the facing is preserved, so a direction picked anywhere inside a
/// compass quadrant comes back shifted such that the quadrant is centered upon its
/// compass direction.
/// </summary>
/// <param name="dir">The direction to rotate.</param>
/// <returns>Returns with the direction rotated back one facing.</returns>
static DirType Rotate_Back_One_Facing(DirType const & dir)
{
	int fraction = dir.Facing & ((DIR_STEP_8 << 8) - 1);
	DirType sector(dir);
	DirType back(sector.Snap_To_8().Raw - 1);
	return(DirType(back.Snap_To_8().Facing ^ fraction));
}


/***********************************************************************************************
 * HouseClass::Random_Cell_In_Zone -- Find a (technically) legal cell in the zone specified.   *
 *                                                                                             *
 *    This routine will pick a random cell within the zone specified. The pick will be         *
 *    clipped to the map edge when necessary.                                                  *
 *                                                                                             *
 * INPUT:   zone  -- The zone to pick a cell from.                                             *
 *                                                                                             *
 * OUTPUT:  Returns with a picked cell within the zone. If the entire zone lies outside of the *
 *          map, then a cell in the core zone is returned instead.                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/04/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: no independent bound yet (004eca50 has no row); main: best 54.60, tried 5 times (frame, ruling, switch)
Cell HouseClass::Random_Cell_In_Zone(ZoneType zone) const
{
	Coord coord;
	int maxdist = 0;

	int radius = MAX(3 * CELL_LEPTON, MIN(Radius, 8 * CELL_LEPTON));
	maxdist = radius*2;

	switch (zone) {
		case ZONE_CORE:
			coord = Coord_Scatter(Center, Random_Pick(0, radius), true);
			break;

		/*
		 * Each edge zone picks a raw 16 bit facing, forces it into the matching
		 * compass quadrant, then rotates back 45 degrees so that the quadrant is
		 * centered upon the compass direction (e.g., north covers NW..NE). Each
		 * quadrant clears the top bits that must be zero and sets the ones that
		 * must be one, so north needs no set and west needs no clear.
		 */
		case ZONE_NORTH: {
			int distance = Random_Pick(radius, maxdist);
			DirType dir(Scen->RandomNumber());
			dir.Raw &= ~(DIR_W << 8);
			coord = Move_Coord(Center, Rotate_Back_One_Facing(dir), distance);
			break;
		}

		case ZONE_EAST: {
			int distance = Random_Pick(radius, maxdist);
			DirType dir(Scen->RandomNumber());
			dir.Raw &= ~(DIR_S << 8);
			dir.Raw |= DIR_E << 8;
			coord = Move_Coord(Center, Rotate_Back_One_Facing(dir), distance);
			break;
		}

		case ZONE_SOUTH: {
			int distance = Random_Pick(radius, maxdist);
			DirType dir(Scen->RandomNumber());
			dir.Raw &= ~(DIR_E << 8);
			dir.Raw |= DIR_S << 8;
			coord = Move_Coord(Center, Rotate_Back_One_Facing(dir), distance);
			break;
		}

		case ZONE_WEST: {
			int distance = Random_Pick(radius, maxdist);
			DirType dir(Scen->RandomNumber());
			dir.Raw |= DIR_W << 8;
			coord = Move_Coord(Center, Rotate_Back_One_Facing(dir), distance);
			break;
		}
	}

	/*
	**	Double check that the location is valid and if so, convert it into a cell
	**	number.
	*/
	Cell cell;
	if (coord == COORD_NONE /*|| !Map.In_Radar(coord.As_Cell())*/) {
		if (zone == ZONE_CORE) {

			/*
			**	Finding a cell within the core failed, so just pick the center
			**	cell. This cell is guaranteed to be valid.
			*/
			cell = Center.As_Cell();
		} else {

			/*
			**	If the edge fails, then try to find a cell within the core.
			*/
			cell = Random_Cell_In_Zone(ZONE_CORE);
		}
	} else {
		cell = coord.As_Cell();
	}

	/*
	**	If the randomly picked location is not in the legal map area, then clip it to
	**	the legal map area.
	*/
	if (!Map.In_Local_Radar(cell)) {
		cell = Map.Clip_To_Map(cell);
	}
	return(cell);
}
#endif


/// <summary>
/// Makes an ally of the object's owner.
/// This is a convenience for the cases where the house to befriend is known only through one
/// of the objects that belong to it.
/// </summary>
void HouseClass::Make_Ally(ObjectClass * object, bool announce) // RA2: 0x4ECF50 forwards the announce flag
{
	if (object) {
		Make_Ally(object->Owner_HouseClass(), announce); // RA2: as above
	}
}



/// <summary>
/// Makes an enemy of the object's owner.
/// This is a convenience for the cases where the offending house is known only through one
/// of the objects that belong to it.
/// </summary>
void HouseClass::Make_Enemy(ObjectClass * object, bool announce) // RA2: 0x4ECF80 forwards the announce flag
{
	if (object) {
		Make_Enemy(object->Owner_HouseClass(), announce); // RA2: as above
	}
}



/// <summary>
/// Places a waypoint at the coordinate specified.
/// The waypoint is appended to the path this house is currently editing. Nothing happens if
/// the cell has already been claimed by another path.
/// </summary>
void HouseClass::Place_Waypoint(Coord const & coord)
{
	if (Can_Place_Waypoint(coord.As_Cell())) {
		Ensure_Path(SelectedPath);
		Paths[SelectedPath]->Add_Waypoint(coord);
	}
}


/// <summary>
/// Selects the waypoint at the coordinate specified.
/// This routine is used by the waypoint plotting controls, and only considers the path this
/// house is currently editing.
/// </summary>
/// <returns>bool; Was a waypoint selected?</returns>
bool HouseClass::Select_Waypoint(Coord const & coord)
{
	Ensure_Path(SelectedPath);
	return(Paths[SelectedPath]->Select_Waypoint(coord));
}


/// <summary>
/// May a waypoint be placed at the cell specified?
/// A cell may hold only one waypoint, although the currently selected path is free to
/// revisit a cell of its own.
/// </summary>
/// <returns>bool; Is the cell free for a waypoint?</returns>
bool HouseClass::Can_Place_Waypoint(Cell const & cell)
{
	for (PathType path = PATH_FIRST; path < PATH_COUNT; path++) {
		Ensure_Path(path);
		for (int index = 0; index < Paths[path]->Waypoint_Count(); index++) {
			WaypointClass * wp = Paths[path]->Get_Waypoint(index);
			if (wp->Location.As_Cell() == cell && path != SelectedPath) {
				return(false);
			}
		}
	}
	return(true);
}


/// <summary>
/// Fetches the waypoint located at the cell specified.
/// This routine is used by the waypoint plotting controls to determine what it is that the
/// player just clicked on.
/// </summary>
/// <returns>Returns with a pointer to the waypoint found. Otherwise, NULL is
/// returned.</returns>
WaypointClass * HouseClass::Waypoint_At(Cell const & cell)
{
	for (PathType path = PATH_FIRST; path < PATH_COUNT; path++) {
		Ensure_Path(path);
		for (int index = 0; index < Paths[path]->Waypoint_Count(); index++) {
			WaypointClass * waypt = Paths[path]->Get_Waypoint(index);
			if (waypt->Location.As_Cell() == cell) {
				return(waypt);
			}
		}
	}
	return(NULL);
}


/// <summary>
/// Fetches the path that a waypoint belongs to.
/// This routine is used when a waypoint object is in hand but its place among the house's
/// waypoint paths is not known -- the display code needs both in order to label it.
/// </summary>
/// <param name="waypt">The waypoint to look up.</param>
/// <param name="xpath">Reference to fill in with the path that owns the waypoint.</param>
/// <param name="waypt_id">Reference to fill in with the waypoint's position along that
/// path.</param>
/// <returns>bool; Was the waypoint found on one of the paths?</returns>
bool HouseClass::Fetch_Waypoint_Data(WaypointClass * waypt, PathType & xpath, char & waypt_id)
{
	for (PathType path = PATH_FIRST; path < PATH_COUNT; path++) {
		Ensure_Path(path);
		for (int index = 0; index < Paths[path]->Waypoint_Count(); index++) {
			if (Paths[path]->Get_Waypoint(index) == waypt) {
				xpath = path;
				waypt_id = index;
				return(true);
			}
		}
	}
	xpath = PATH_NONE;
	waypt_id = 0;
	return(false);
}


/// <summary>
/// Creates an enumerator over this house's connection points.
/// This routine is part of the connection point container contract, and lets a client
/// discover which outgoing interfaces the house is prepared to serve.
/// </summary>
/// <param name="ppEnum">Pointer to fill in with the newly created enumerator.</param>
/// <returns>Returns with S_OK, or E_OUTOFMEMORY if the enumerator could not be
/// created.</returns>
HRESULT STDMETHODCALLTYPE HouseClass::EnumConnectionPoints(IEnumConnectionPoints **ppEnum)
{
	if (ppEnum == NULL) {
		return(E_POINTER);
	}

	EnumConnectionPointsClass *cpoints = new EnumConnectionPointsClass(PowerEventConnectionPoints);

	if (cpoints == NULL) {
		return(E_OUTOFMEMORY);
	}

	cpoints->AddRef();
	*ppEnum = cpoints;
	return(S_OK);
}


/// <summary>
/// Finds the connection point for the outgoing interface specified.
/// This routine is part of the connection point container contract. A client that wishes to
/// be told about this house's power events locates its connection point through it.
/// </summary>
/// <param name="riid">The outgoing interface being asked for.</param>
/// <param name="ppCP">Pointer to fill in with the connection point found.</param>
/// <returns>Returns with S_OK, or CONNECT_E_CANNOTCONNECT if this house offers no such
/// point.</returns>
HRESULT STDMETHODCALLTYPE HouseClass::FindConnectionPoint(REFIID riid, IConnectionPoint **ppCP)
{
	if (ppCP == NULL) {
		return(E_POINTER);
	}

	*ppCP = NULL;

	for (int i = 0; i < PowerEventConnectionPoints.Count(); i++) {
		IConnectionPoint *cpoint = PowerEventConnectionPoints[i];
		IID iid;
		cpoint->GetConnectionInterface(&iid);
		if (iid == riid) {
			*ppCP = cpoint;
			cpoint->AddRef();
			return(S_OK);
		}
	}

	return(CONNECT_E_CANNOTCONNECT);
}


/// <summary>
/// Stops tracking an object as one of this house's active possessions.
/// This routine is called when an object leaves this house's control. A building also takes
/// its power drain and storage capacity with it, and the tiberium it was holding is banked
/// rather than lost when the building was taken by capture.
/// </summary>
/// <param name="techno">The object to stop tracking.</param>
/// <param name="bycapture">Was this object lost to a capture?</param>
void HouseClass::Tracking_Active_Remove(TechnoClass * techno, bool bycapture)
{
	TechnoTypeClass const * ttype;

	Map.Notice_Object_Change(techno); // RA2: the sidebar is told first (SidebarClass::Notice_Object_Change, 0x67C150)

	switch (techno->Fetch_RTTI()) {
		case RTTI_BUILDING:
			ttype = techno->TClass;
			ABQuantity.Decrement(ttype->Fetch_Heap_ID());

			if (techno != NULL) {
				BuildingClass * bptr = (BuildingClass *)techno;

				RecalcPower = true;
				Capacity -= bptr->Class->Capacity;
				if (bycapture) {
					StorageClass *storage = &bptr->Storage;
					int i = storage->First_Used_Slot();
					while (i != -1) {
						int amount = storage->Decrease_Amount(0x7FFFFFFF, i);
						Give_Tiberium(amount, i); // RA2: as above
						i = storage->First_Used_Slot();
					}
				} else {
					Tiberium -= bptr->Storage;
				}
			}
			break;

		case RTTI_AIRCRAFT:
			ttype = techno->TClass;
			AAQuantity.Decrement(ttype->Fetch_Heap_ID());
			break;

		case RTTI_INFANTRY:
			ttype = techno->TClass;
			AIQuantity.Decrement(ttype->Fetch_Heap_ID());
			break;

		case RTTI_UNIT:
			ttype = techno->TClass;
			AUQuantity.Decrement(ttype->Fetch_Heap_ID());
			break;
	}
}



/// <summary>
/// Begins tracking an object as one of this house's active possessions.
/// This routine is called when an object comes under this house's control. A building also
/// hands over its power drain, its storage capacity, and whatever tiberium it is holding.
/// </summary>
/// <param name="techno">The object to begin tracking.</param>
void HouseClass::Tracking_Active_Add(TechnoClass * techno, bool bycapture)
{
	switch (techno->Fetch_RTTI()) {
	case RTTI_BUILDING: // RA2: RA2's form
		ABQuantity.Increment(techno->Techno_Type_Class()->Fetch_Heap_ID()); // RA2: as above
		if (techno != 0 && techno->Fetch_RTTI() == RTTI_BUILDING) { // RA2: as above
			BuildingClass *building = (BuildingClass *)techno; // RA2: as above
			Adjust_Drain(building->Power_Drain()); // RA2: as above
			Capacity += building->Class->Capacity; // RA2: as above
			Tiberium += building->Storage; // RA2: as above
		}
		break; // RA2: as above
	case RTTI_AIRCRAFT: // RA2: as above
		AAQuantity.Increment(techno->Techno_Type_Class()->Fetch_Heap_ID()); // RA2: as above
		break; // RA2: as above
	case RTTI_INFANTRY: // RA2: as above
		if (!(((InfantryClass *)techno))->IsTechnician) { // RA2: as above
			AIQuantity.Increment(techno->Techno_Type_Class()->Fetch_Heap_ID()); // RA2: as above
		}
		break; // RA2: as above
	case RTTI_UNIT: // RA2: as above
		AUQuantity.Increment(techno->Techno_Type_Class()->Fetch_Heap_ID()); // RA2: as above
		break; // RA2: as above
	}
}



/***********************************************************************************************
 * HouseClass::As_Pointer -- Converts a house number into a house object pointer.              *
 *                                                                                             *
 *    Use this routine to convert a house number into the house pointer that it represents.    *
 *    A simple index into the Houses template array is not sufficient, since the array order   *
 *    is arbitrary. An actual scan through the house object is required in order to find the   *
 *    house object desired.                                                                    *
 *                                                                                             *
 * INPUT:   house -- The house type number to look up.                                         *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the house object that the house number represents.       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
HouseClass * House_From_HousesType(HousesType house)
{
	for (int index = 0; index < Houses.Count(); index++) {
		HouseClass * housep = Houses[index];
		if (housep->Class->House == house) {
			return(housep);
		}
	}
	return(NULL);
}


/// <summary>
/// Adds this house's state to the game checksum.
/// This routine serves the multiplayer sync checker, which compares each machine's checksum
/// in order to detect when the simulations have drifted apart.
/// </summary>
/// <param name="crc">The checksum engine to submit the house data to.</param>
void HouseClass::Compute_CRC(CRCEngine & crc) const
{
	BASECLASS::Compute_CRC(crc);
	crc(Difficulty);
	crc(Class->House);
	crc(FirepowerBias);
	crc(GroundspeedBias);
	crc(AirspeedBias);
	crc(ArmorBias);
	crc(ROFBias);
	crc(CostBias);
	crc(BuildSpeedBias);
	crc(RepairDelay);
	crc(BuildDelay);
	crc(IsHuman);
	crc(IsStarted);
	crc(IsAlerted);
	crc(IsAITriggersOn);
	crc(IsDefeated);
	crc(IQ);
	crc(State);
	crc(Blockage);
	crc((int)RepairTimer);
	crc((int)AlertTime);
	crc((int)BorrowedTime);
	crc(CurUnits);
	crc(CurBuildings);
	crc(CurInfantry);
	crc(CurAircraft);
	crc((int)Credits);
	crc((int)Capacity);
	crc(AircraftFactories);
	crc(InfantryFactories);
	crc(UnitFactories);
	crc(NumShipyards); // RA2: crc'd
	crc(BuildingFactories);
	crc(NumOrePurifiers); // RA2: as above
	crc(Power);
	crc(Drain);
	crc(WhoLastHurtMe);
	crc(Enemy);
	Base.Compute_CRC(crc);
}



/// <summary>
/// Loads this house from the data stream.
/// This routine reads back the parts of the house that Save wrote by hand, and submits every
/// pointer it recovers to the swizzler so that they can be remapped once the whole save file
/// has been read in.
/// </summary>
/// <param name="stream">The stream to read the house from.</param>
/// <returns>Returns with S_OK, or the failure code reported by the stream.</returns>
HRESULT STDMETHODCALLTYPE HouseClass::Load(IStream *stream) // RA2: the body follows the executable
{
	ConYards.Clear(); // RA2: the house's building lists are emptied first
	Buildings.Clear(); // RA2: as above
	UnitRepairStations.Clear(); // RA2: as above
	CloningVats.Clear(); // RA2: as above
	PsychicDetectionBuildings.Clear(); // RA2: as above
	for (int point = 0; point < PowerEventConnectionPoints.Count(); point++) { // RA2: the connection points are released
		while (PowerEventConnectionPoints[point]->Release() > 0U) { // RA2: as above
			; // RA2: as above
		}
		PowerEventConnectionPoints[point] = NULL; // RA2: as above
	}
	PowerEventConnectionPoints.Clear(); // RA2: as above

	while (SuperWeapon.Count()) { // RA2: as above
		delete SuperWeapon[0]; // RA2: as above
		SuperWeapon.Delete_Index(0); // RA2: as above
	}

	HRESULT result = BASECLASS::Load(stream); // RA2: as above
	if (SUCCEEDED(result)) { // RA2: as above
		int count; // RA2: shared by all the list reads
		new(this) HouseClass(NoInitClass()); // RA2: as above

		Base.Load(stream); // RA2: as above

		BQuantity.Load(stream); // RA2: as above
		UQuantity.Load(stream); // RA2: as above
		IQuantity.Load(stream); // RA2: as above
		AQuantity.Load(stream); // RA2: as above

		ABQuantity.Load(stream); // RA2: as above
		AUQuantity.Load(stream); // RA2: as above
		AIQuantity.Load(stream); // RA2: as above
		AAQuantity.Load(stream); // RA2: as above

		PBQuantity.Load(stream); // RA2: as above
		PUQuantity.Load(stream); // RA2: as above
		PIQuantity.Load(stream); // RA2: as above
		PAQuantity.Load(stream); // RA2: as above

		Swizzle_Pointer(&Class); // RA2: as above
		Swizzle_Pointer(&InfantryFactory); // RA2: as above
		Swizzle_Pointer(&UnitFactory); // RA2: as above
		Swizzle_Pointer(&AircraftFactory); // RA2: as above
		Swizzle_Pointer(&Primary_ForShips); // RA2: the factory slots RA2 added
		Swizzle_Pointer(&BuildingFactory); // RA2: as above
		Swizzle_Pointer(&Primary_Unused1); // RA2: as above
		Swizzle_Pointer(&Primary_Unused2); // RA2: as above
		Swizzle_Pointer(&Primary_Unused3); // RA2: as above
		Swizzle_Pointer(&Primary_ForDefenses); // RA2: as above
		Swizzle_Pointer(&ToCapture); // RA2: as above

		int index; // RA2: as above
		for (index = 0; index < PATH_COUNT; index++) { // RA2: as above
			Swizzle_Pointer(&Paths[index]); // RA2: as above
		}

		for (index = 0; index < ARRAY_SIZE(DropshipLoadouts); index++) { // RA2: as above
			DropshipLoadouts[index].Swizzle(); // RA2: as above
		}

		{ // RA2: a counted list of SuperClass * read back
			count = 0; // RA2: as above
			result = stream->Read(&count, sizeof(count), NULL); // RA2: as above
			if (FAILED(result)) { // RA2: as above
				return(result); // RA2: as above
			}
			while (count--) { // RA2: as above
				SuperClass * value = NULL; // RA2: as above
				result = stream->Read(&value, sizeof(value), NULL); // RA2: as above
				if (FAILED(result)) { // RA2: as above
					return(result); // RA2: as above
				}
				SuperWeapon.Add(value); // RA2: as above
			}
		}
		{ // RA2: a counted list of TagClass * read back
			count = 0; // RA2: as above
			result = stream->Read(&count, sizeof(count), NULL); // RA2: as above
			if (FAILED(result)) { // RA2: as above
				return(result); // RA2: as above
			}
			while (count--) { // RA2: as above
				TagClass * value = NULL; // RA2: as above
				result = stream->Read(&value, sizeof(value), NULL); // RA2: as above
				if (FAILED(result)) { // RA2: as above
					return(result); // RA2: as above
				}
				HouseTags.Add(value); // RA2: as above
			}
		}
		{ // RA2: the AngerNodes are read back one structure at a time
			count = 0; // RA2: as above
			result = stream->Read(&count, sizeof(count), NULL); // RA2: as above
			if (FAILED(result)) { // RA2: as above
				return(result); // RA2: as above
			}
			while (count--) { // RA2: as above
				AngerStruct anger; // RA2: as above
				result = stream->Read(&anger, sizeof(anger), NULL); // RA2: as above
				if (FAILED(result)) { // RA2: as above
					return(result); // RA2: as above
				}
				AngerNodes.Add(anger); // RA2: as above
			}
		}
		for (index = 0; index < AngerNodes.Count(); index++) { // RA2: as above
			Swizzle_Pointer(&AngerNodes[index].House); // RA2: as above
		}
		{ // RA2: the ScoutNodes are read back one structure at a time
			count = 0; // RA2: as above
			result = stream->Read(&count, sizeof(count), NULL); // RA2: as above
			if (FAILED(result)) { // RA2: as above
				return(result); // RA2: as above
			}
			while (count--) { // RA2: as above
				ScoutStruct scout; // RA2: as above
				result = stream->Read(&scout, sizeof(scout), NULL); // RA2: as above
				if (FAILED(result)) { // RA2: as above
					return(result); // RA2: as above
				}
				ScoutNodes.Add(scout); // RA2: as above
			}
		}
		for (index = 0; index < ScoutNodes.Count(); index++) { // RA2: as above
			Swizzle_Pointer(&ScoutNodes[ScoutNodes.Count() - 1].House); // RA2: as above
		}
		{ // RA2: a counted list of BuildingClass * read back
			count = 0; // RA2: as above
			result = stream->Read(&count, sizeof(count), NULL); // RA2: as above
			if (FAILED(result)) { // RA2: as above
				return(result); // RA2: as above
			}
			while (count--) { // RA2: as above
				BuildingClass * value = NULL; // RA2: as above
				result = stream->Read(&value, sizeof(value), NULL); // RA2: as above
				if (FAILED(result)) { // RA2: as above
					return(result); // RA2: as above
				}
				ConYards.Add(value); // RA2: as above
			}
		}
		{ // RA2: a counted list of BuildingClass * read back
			count = 0; // RA2: as above
			result = stream->Read(&count, sizeof(count), NULL); // RA2: as above
			if (FAILED(result)) { // RA2: as above
				return(result); // RA2: as above
			}
			while (count--) { // RA2: as above
				BuildingClass * value = NULL; // RA2: as above
				result = stream->Read(&value, sizeof(value), NULL); // RA2: as above
				if (FAILED(result)) { // RA2: as above
					return(result); // RA2: as above
				}
				Buildings.Add(value); // RA2: as above
			}
		}
		{ // RA2: a counted list of BuildingClass * read back
			count = 0; // RA2: as above
			result = stream->Read(&count, sizeof(count), NULL); // RA2: as above
			if (FAILED(result)) { // RA2: as above
				return(result); // RA2: as above
			}
			while (count--) { // RA2: as above
				BuildingClass * value = NULL; // RA2: as above
				result = stream->Read(&value, sizeof(value), NULL); // RA2: as above
				if (FAILED(result)) { // RA2: as above
					return(result); // RA2: as above
				}
				UnitRepairStations.Add(value); // RA2: as above
			}
		}
		{ // RA2: a counted list of BuildingClass * read back
			count = 0; // RA2: as above
			result = stream->Read(&count, sizeof(count), NULL); // RA2: as above
			if (FAILED(result)) { // RA2: as above
				return(result); // RA2: as above
			}
			while (count--) { // RA2: as above
				BuildingClass * value = NULL; // RA2: as above
				result = stream->Read(&value, sizeof(value), NULL); // RA2: as above
				if (FAILED(result)) { // RA2: as above
					return(result); // RA2: as above
				}
				CloningVats.Add(value); // RA2: as above
			}
		}
		{ // RA2: a counted list of BuildingClass * read back
			count = 0; // RA2: as above
			result = stream->Read(&count, sizeof(count), NULL); // RA2: as above
			if (FAILED(result)) { // RA2: as above
				return(result); // RA2: as above
			}
			while (count--) { // RA2: as above
				BuildingClass * value = NULL; // RA2: as above
				result = stream->Read(&value, sizeof(value), NULL); // RA2: as above
				if (FAILED(result)) { // RA2: as above
					return(result); // RA2: as above
				}
				PsychicDetectionBuildings.Add(value); // RA2: as above
			}
		}

		for (index = 0; index < SuperWeapon.Count(); index++) { // RA2: every pointer is submitted to the swizzler
			Swizzle_Pointer(&SuperWeapon[index]); // RA2: as above
		}
		for (index = 0; index < HouseTags.Count(); index++) { // RA2: every pointer is submitted to the swizzler
			Swizzle_Pointer(&HouseTags[index]); // RA2: as above
		}
		for (index = 0; index < ConYards.Count(); index++) { // RA2: every pointer is submitted to the swizzler
			Swizzle_Pointer(&ConYards[index]); // RA2: as above
		}
		for (index = 0; index < Buildings.Count(); index++) { // RA2: every pointer is submitted to the swizzler
			Swizzle_Pointer(&Buildings[index]); // RA2: as above
		}
		for (index = 0; index < UnitRepairStations.Count(); index++) { // RA2: every pointer is submitted to the swizzler
			Swizzle_Pointer(&UnitRepairStations[index]); // RA2: as above
		}
		for (index = 0; index < CloningVats.Count(); index++) { // RA2: every pointer is submitted to the swizzler
			Swizzle_Pointer(&CloningVats[index]); // RA2: as above
		}
		for (index = 0; index < PsychicDetectionBuildings.Count(); index++) { // RA2: every pointer is submitted to the swizzler
			Swizzle_Pointer(&PsychicDetectionBuildings[index]); // RA2: as above
		}

		int hasaigen = false; // RA2: as above
		result = stream->Read(&hasaigen, sizeof(hasaigen), NULL); // RA2: as above
		if (SUCCEEDED(result) && hasaigen) { // RA2: as above
			result = OleLoadFromStream(stream, IID_IAIHouse, (void**)&AIGeneral); // RA2: as above
		}
	}
	return(result); // RA2: as above
}



/// <summary>
/// Saves this house to the data stream.
/// This routine writes out the parts of the house that the base persistence code cannot
/// handle by itself -- the base, the quantity trackers, and the various lists of objects
/// this house has a claim on.
/// </summary>
/// <param name="stream">The stream to write the house to.</param>
/// <param name="cleardirty">Should the house be marked as clean afterward?</param>
/// <returns>Returns with S_OK, or the failure code reported by the stream.</returns>
HRESULT STDMETHODCALLTYPE HouseClass::Save(IStream *stream, int cleardirty)
{
	int count;
	HRESULT result = AbstractClass::Save(stream, cleardirty); // RA2: RA2's form
	if (SUCCEEDED(result)) {
	Base.Save(stream); // RA2: as above
	BQuantity.Save(stream); // RA2: as above
	UQuantity.Save(stream); // RA2: as above
	IQuantity.Save(stream); // RA2: as above
	AQuantity.Save(stream); // RA2: as above
	ABQuantity.Save(stream); // RA2: as above
	AUQuantity.Save(stream); // RA2: as above
	AIQuantity.Save(stream); // RA2: as above
	AAQuantity.Save(stream); // RA2: as above
	PBQuantity.Save(stream); // RA2: as above
	PUQuantity.Save(stream); // RA2: as above
	PIQuantity.Save(stream); // RA2: as above
	PAQuantity.Save(stream); // RA2: as above

	{
		int index; // RA2: as above
		count = SuperWeapon.Count();
		result = stream->Write(&count, sizeof(count), 0); // RA2: as above
		if (FAILED(result)) {
			return(result);
		}

		for (index = 0; index < count; index++) {
			result = stream->Write(&SuperWeapon[index], sizeof(SuperWeapon[index]), 0); // RA2: as above
			if (FAILED(result)) {
				return(result);
			}
		}
	}
	{
		int index; // RA2: as above
		count = HouseTags.Count();
		result = stream->Write(&count, sizeof(count), 0); // RA2: as above
		if (FAILED(result)) {
			return(result);
		}

		for (index = 0; index < count; index++) {
			result = stream->Write(&HouseTags[index], sizeof(HouseTags[index]), 0); // RA2: as above
			if (FAILED(result)) {
				return(result);
			}
		}
	}
	{
		int index; // RA2: as above
		count = AngerNodes.Count();
		result = stream->Write(&count, sizeof(count), 0); // RA2: as above
		if (FAILED(result)) {
			return(result);
		}

		for (index = 0; index < count; index++) {
			result = stream->Write(&AngerNodes[index], sizeof(AngerNodes[index]), 0); // RA2: as above
			if (FAILED(result)) {
				return(result);
			}
		}
	}
	{
		int index; // RA2: as above
		count = ScoutNodes.Count();
		result = stream->Write(&count, sizeof(count), 0); // RA2: as above
		if (FAILED(result)) {
			return(result);
		}

		for (index = 0; index < count; index++) {
			result = stream->Write(&ScoutNodes[index], sizeof(ScoutNodes[index]), 0); // RA2: as above
			if (FAILED(result)) {
				return(result);
			}
		}
	}
	{
		int index; // RA2: as above
		count = ConYards.Count();
		result = stream->Write(&count, sizeof(count), 0); // RA2: as above
		if (FAILED(result)) {
			return(result);
		}

		for (index = 0; index < count; index++) {
			result = stream->Write(&ConYards[index], sizeof(ConYards[index]), 0); // RA2: as above
			if (FAILED(result)) {
				return(result);
			}
		}
	}
	{
		int index; // RA2: as above
		count = Buildings.Count(); // RA2: as above
		result = stream->Write(&count, sizeof(count), 0); // RA2: as above
		if (FAILED(result)) {
			return(result);
		}
		for (index = 0; index < count; index++) {
			result = stream->Write(&Buildings[index], sizeof(Buildings[index]), 0); // RA2: as above
			if (FAILED(result)) {
				return(result);
			}
		}
	}
	{
		int index; // RA2: as above
		count = UnitRepairStations.Count(); // RA2: as above
		result = stream->Write(&count, sizeof(count), 0); // RA2: as above
		if (FAILED(result)) {
			return(result);
		}
		for (index = 0; index < count; index++) {
			result = stream->Write(&UnitRepairStations[index], sizeof(UnitRepairStations[index]), 0); // RA2: as above
			if (FAILED(result)) {
				return(result);
			}
		}
	}
	{
		int index; // RA2: as above
		count = CloningVats.Count(); // RA2: as above
		result = stream->Write(&count, sizeof(count), 0); // RA2: as above
		if (FAILED(result)) {
			return(result);
		}
		for (index = 0; index < count; index++) {
			result = stream->Write(&CloningVats[index], sizeof(CloningVats[index]), 0); // RA2: as above
			if (FAILED(result)) {
				return(result);
			}
		}
	}
	{
		int index; // RA2: as above
		count = PsychicDetectionBuildings.Count(); // RA2: as above
		result = stream->Write(&count, sizeof(count), 0); // RA2: as above
		if (FAILED(result)) {
			return(result);
		}
		for (index = 0; index < count; index++) {
			result = stream->Write(&PsychicDetectionBuildings[index], sizeof(PsychicDetectionBuildings[index]), 0); // RA2: as above
			if (FAILED(result)) {
				return(result);
			}
		}
	}

	BOOL present = AIGeneral != 0; // RA2: as above
	result = stream->Write(&present, sizeof(present), 0); // RA2: as above
	if (SUCCEEDED(result) && present) { // RA2: as above
		IPersistStreamPtr persist(AIGeneral); // RA2: as above
		result = OleSaveToStream(persist, stream); // RA2: as above
	}
	}
	return result; // RA2: as above
}



/// <summary>
/// Fetches the class identifier of this object.
/// This routine is part of the persistence contract. The load system uses the identifier to
/// discover which class to build when the object is read back in.
/// </summary>
/// <param name="retval">Pointer to the identifier to fill in.</param>
/// <returns>Returns with S_OK, or E_POINTER if no destination was supplied.</returns>
HRESULT STDMETHODCALLTYPE HouseClass::GetClassID(CLSID * retval)
{
	if (retval == NULL) return(E_POINTER);
	*retval = CLSID_HouseClass;
	return(S_OK);
}


/// <summary>
/// Fetches the byte size of this house object.
/// This routine is used by the save and load system when it needs to know how much room a
/// house occupies.
/// </summary>
/// <returns>Returns with the size of the house object in bytes.</returns>
int HouseClass::Fetch_Object_Size(void) const // RA2: no argument
{
	return(sizeof(*this));
}


/// <summary>
/// Fetches a waypoint path, creating it if need be.
/// This routine is used by the waypoint plotting code, which should never have to trouble
/// itself over whether the path object exists yet.
/// </summary>
/// <param name="path">The path to fetch.</param>
/// <returns>Returns with a pointer to the waypoint path. It is never NULL.</returns>
WaypointPathClass * HouseClass::Ensure_Path(PathType path)
{
	WaypointPathClass *wp = Paths[path];
	if (wp == NULL) {
		wp = new WaypointPathClass(path);
		Paths[path] = wp;
	}
	return(wp);
}


/// <summary>
/// Raises this house's firestorm wall.
/// Every firestorm wall this house owns is switched on and the map zones are recalculated,
/// since the raised wall now bars movement through the cells it stands in.
/// </summary>
void HouseClass::Activate_Firestorm(void)
{
	if (!FirestormDefenseActivated) {
		DynamicVectorClass<Cell> cells;
		FirestormDefenseActivated = true;
		for (int i = ::Buildings.Count() - 1; i >= 0; i--) { // RA2: as below
			BuildingClass * b = ::Buildings[i]; // RA2: the loop reads the global building heap, not HouseClass::Buildings
			if (b->Class->IsFirestormWall && b->House == this) {
				b->Update_FS_Wall_State();
				cells.Add(b->Center_Coord().As_Cell());
			}
		}
		Map.Zone_Reset();
		Map.Recalc_Cells_In_List(cells);
	}
}



/// <summary>
/// Lowers this house's firestorm wall.
/// Every firestorm wall this house owns is switched off and the map zones are recalculated,
/// since those cells may be walked through once more. The player is told that the defense
/// has gone offline.
/// </summary>
void HouseClass::Deactivate_Firestorm(void)
{
	if (FirestormDefenseActivated) {
		FirestormDefenseActivated = false;
		DynamicVectorClass<Cell> cells;
		for (int i = ::Buildings.Count() - 1; i >= 0; i--) { // RA2: as below
			BuildingClass * b = ::Buildings[i]; // RA2: the loop reads the global building heap, not HouseClass::Buildings
			if (b->Class->IsFirestormWall && b->House == this) {
				b->Update_FS_Wall_State();
				cells.Add(b->Center_Coord().As_Cell());
			}
		}
		Map.Zone_Reset();
		Map.Recalc_Cells_In_List(cells); // RA2: no EVA line (Speak) when the firestorm goes offline
	}
}



/// <summary>
/// Handles the loss of a firestorm generator.
/// This routine is called when one of the house's firestorm generators goes away. Should it
/// have been the last working one, the firestorm super weapon is discharged, so that the
/// wall cannot be raised again until a generator is rebuilt and the weapon recharged.
/// </summary>
void HouseClass::Lost_Firestorm_Generator(void)
{ // RA2: the body is one ret (0x4EEB90); the firestorm super weapon is TibSun's
}


/// <summary>
/// Adds to the anger this house feels toward another house.
/// Anger builds up as a rival makes a nuisance of itself, and the house declares whichever
/// surviving non-ally it is angriest at to be its enemy. Pass a negative level to take some
/// of that anger back off again.
/// </summary>
/// <param name="level">The amount of anger to add, which may be negative.</param>
/// <param name="house">The house that the anger is directed at.</param>
void HouseClass::Add_Anger(int level, HouseClass const * house)
{
	int i;

	for (i = 0; i < AngerNodes.Count(); i++) {
		if (AngerNodes[i].House == house) {
			AngerNodes[i].Level += level;
		}
	}

	int maxanger = 0;
	HouseClass * mostangry = NULL;

	for (i = 0; i < AngerNodes.Count(); i++) {
		HouseClass * enemy = AngerNodes[i].House;
		if (AngerNodes[i].Level > maxanger && !enemy->IsDefeated) {
			if (!Is_Ally(enemy)) {
				maxanger = AngerNodes[i].Level;
				mostangry = AngerNodes[i].House;
			}
		}
	}

	if (mostangry != NULL) {
		Enemy = mostangry->HeapID;
	} else {
		Enemy = HOUSE_NONE;
	}

}


/// <summary>
/// Marks another house as having been scouted.
/// The computer keeps a scouting record so that it knows which of its rivals it has actually
/// found and may therefore start plotting against.
/// </summary>
/// <param name="house">The house that has now been scouted.</param>
void HouseClass::Mark_Scouted(HouseClass const * house)
{
	for (int i = 0; i < ScoutNodes.Count(); i++) {
		if (ScoutNodes[i].House == house) {
			ScoutNodes[i].IsScouted = true;
		}
	}
}


/// <summary>
/// Starts the computer building a base.
/// This routine lays out a base plan for the house, unless the scenario has already supplied
/// one of its own.
/// </summary>
void HouseClass::Plan_Naval_Yard_Path(int node) // RA2: RA2's form
{
	Cell center = Center.As_Cell(); // RA2: as above
	int zone = Map.Get_Cell_Zone(center, MZONE_NORMAL, false); // RA2: as above
	Cell start = Map.Nearby_Location(center, SPEED_FOOT, zone, MZONE_NORMAL, false, Point2D(1, 1), false, false, false, true, Cell(0, 0), false); // RA2: as above
	int water_zone = -1; // RA2: as above
	if (Map.Largest_Water_Zone(water_zone) < 3000) { // RA2: as above
		Base.Nodes.Delete_Index(node); // RA2: as above
		IsNavalBaseAllowed = false; // RA2: as above
		return; // RA2: as above
	}

	int width = 1; // RA2: as above
	int height = 1; // RA2: as above
	for (int index = 0; index < Rule->BuildNavalYard.Count(); index++) { // RA2: as above
		if (Rule->BuildNavalYard[index]->AIBasePlanningSide == Class->Side || Rule->BuildNavalYard[index]->AIBasePlanningSide == -1) { // RA2: as above
			width = Rule->BuildNavalYard[index]->Width() + 1; // RA2: as above
			height = Rule->BuildNavalYard[index]->Height(false) + 1; // RA2: as above
			break; // RA2: as above
		}
	}

	DynamicVectorClass<Cell> cells; // RA2: as above
	Map.Find_Naval_Yard_Cells(center, 25, 30, 10, water_zone, cells, width, height); // RA2: as above
	if (cells.Count() == 0) { // RA2: as above
		Base.Nodes.Delete_Index(node); // RA2: as above
		IsNavalBaseAllowed = false; // RA2: as above
		return; // RA2: as above
	}

	IndexClass<int, Cell> sorted; // RA2: as above
	for (int ci = 0; ci < cells.Count(); ci++) { // RA2: as above
		Cell spot = Map.Nearby_Location(cells[ci], SPEED_FOOT, zone, MZONE_NORMAL, false, Point2D(1, 1), false, false, false, true, Cell(0, 0), false); // RA2: as above
		int distance = Search.Test_Cell_Walk(center, spot, NULL, false, false, MZONE_NORMAL); // RA2: as above
		sorted.Add_Index(cells.Count() * distance + ci, cells[ci]); // RA2: as above
	}
	cells.Clear(); // RA2: as above
	for (int si = 0; si < sorted.Count(); si++) { // RA2: as above
		cells.Add(sorted.Fetch_By_Position(si)); // RA2: as above
	}

	bool found = false; // RA2: as above
	PathStruct * path = NULL; // RA2: as above
	Cell target = CELL_NONE; // RA2: as above
	Cell spot = CELL_NONE; // RA2: as above
	FacingType moves[2048]; // RA2: as above
	UnitClass * unit = new UnitClass(UnitTypes[UnitTypeClass::From_Name("HARV")], this); // RA2: as above
	unit->Unlimbo(Coord(start.X * 256 + 128, start.Y * 256 + 128, 0), DIR_N); // RA2: as above
	while (!found && cells.Count() > 0) { // RA2: as above
		target = cells[0]; // RA2: as above
		spot = Map.Nearby_Location(cells[0], SPEED_FOOT, zone, MZONE_NORMAL, false, Point2D(1, 1), false, false, false, true, Cell(0, 0), false); // RA2: as above
		if (spot != CELL_NONE) { // RA2: as above
			path = Search.Find_Path(start, spot, unit, moves, -1, MZONE_NORMAL, AStarClass::AVOIDANCE_NONE); // RA2: as above
		} else {
			path = NULL; // RA2: as above
		}
		if (path != NULL && path->Length <= 25) { // RA2: as above
			found = true; // RA2: as above
		} else {
			cells.Delete_Index(0); // RA2: as above
		}
	}
	unit->Limbo(); // RA2: as above
	delete unit; // RA2: as above
	if (!found) { // RA2: as above
		Base.Nodes.Delete_Index(node); // RA2: as above
		IsNavalBaseAllowed = false; // RA2: as above
		return; // RA2: as above
	}

	int bit = 1 << HeapID; // RA2: as above
	Cell cell = start; // RA2: as above
	int first = 0; // RA2: as above
	Cell last = start; // RA2: as above
	for (int mi = 0; mi < path->Length; mi++) { // RA2: as above
		cell = AdjacentCell[moves[mi] & 7] + cell; // RA2: as above
		if (Map[cell].OccupiedBy & bit) { // RA2: as above
			first = mi + 1; // RA2: as above
			last = cell; // RA2: as above
		}
	}

	cell = last; // RA2: as above
	int counter = 2; // RA2: as above
	int at = node - 1; // RA2: as above
	int type = -1; // RA2: as above
	for (int di = 0; di < Rule->BuildDummy.Count(); di++) { // RA2: as above
		if (Rule->BuildDummy[di]->AIBasePlanningSide == Class->Side || Rule->BuildDummy[di]->AIBasePlanningSide == -1) { // RA2: as above
			type = Rule->BuildDummy[di]->HeapID; // RA2: as above
			break; // RA2: as above
		}
	}
	for (int step = first; step < path->Length; step++) { // RA2: as above
		if (counter == 0) { // RA2: as above
			counter = 3; // RA2: as above
			Base.Nodes.Insert_After(BaseNodeClass((StructType)type, cell, false, 0), at); // RA2: the object-first order
			at++; // RA2: as above
		} else {
			counter--; // RA2: as above
		}
		cell = cell + AdjacentCell[moves[step] & 7]; // RA2: as above
	}
	Base.Nodes[at + 1].CellID = target; // RA2: as above
}




void HouseClass::Ensure_Base_Nodes(void) // RA2: RA2's form
{
	if (!Is_Human_Player() && Session.Type != GAME_NORMAL) { // RA2: as above
		Computer_Paranoid();
	}
	if (Base.Nodes.Count() == 0) {
		int save_init = ScenarioInit;
		ScenarioInit = 0;
		Make_Base_Nodes();
		ScenarioInit = save_init;
	}
}



/// <summary>
/// Fetches the first structure in a list that this house may own.
/// This routine is used to resolve the generic structure lists in the rules -- the barracks
/// or the power plant, say -- down to the particular one that this house builds.
/// </summary>
/// <param name="vector">The candidate structures, in order of preference.</param>
/// <returns>Returns with the first structure this house may own, or NULL if it may own
/// none.</returns>
BuildingTypeClass const * HouseClass::Get_First_Ownable(DynamicVectorClass<BuildingTypeClass const *> const & owned) const // RA2: RA2's form
{
	int owner_bit = 1 << HouseTypeClass::From_Name(Class->ParentCountry.Buffer); // RA2: as above
	for (int index = 0; index < owned.Count(); index++) { // RA2: as above
		BuildingTypeClass const * building = owned[index]; // RA2: as above
		if ((building->Ownable & owner_bit) != 0 // RA2: as above
			&& (building->RequiredHouses == -1 || (building->RequiredHouses & (1 << Class->House)) != 0) // RA2: as above
			&& (building->ForbiddenHouses == -1 || (building->ForbiddenHouses & (1 << Class->House)) == 0) // RA2: as above
			&& (building->AIBasePlanningSide == -1 || building->AIBasePlanningSide == Class->Side)) { // RA2: as above
			if (AllowDisableableSupers || building->SuperWeapon == -1 || !SuperWeapon[building->SuperWeapon]->Class->DisableableFromShell) { // RA2: as above
				return(building); // RA2: as above
			}
		}
	}
	return(NULL);
}




UnitTypeClass const * HouseClass::Get_First_Ownable(TypeList<UnitTypeClass const *> const & owned) const // RA2: as above
{
	int owner = 1 << HouseTypeClass::From_Name(Class->ParentCountry.Buffer); // RA2: as above
	for (int index = 0; index < owned.Count(); index++) { // RA2: as above
		if (owned[index]->Ownable & owner) { // RA2: as above
			return(owned[index]); // RA2: as above
		}
	}
	return(NULL);
}


/// <summary>
/// Are the prerequisites for this object accounted for?
/// This routine is used while the computer plans its base, where the question must be asked
/// of the structures the house intends to own rather than of those it owns at this moment.
/// A generic prerequisite is resolved to whichever structure of that kind this house builds.
/// </summary>
/// <param name="type">The object whose prerequisites are being tested.</param>
/// <param name="owned">The structures to be counted as owned for the purposes of the test.</param>
/// <param name="ownedcount">How many entries of the owned list are to be considered.</param>
/// <returns>bool; Are all of the prerequisites satisfied?</returns>
bool HouseClass::AI_Has_Prerequisites(TechnoTypeClass const * type, DynamicVectorClass<BuildingTypeClass const *> & owned, int ownedcount) const
{
	int i = 0; // RA2: the loops are the executable's do-while forms; a numbered prerequisite is looked up in the owned list, a negative one names a rules list of which any member will do
	if (type->Prerequisite.Count() > 0) { // RA2: as above
		do { // RA2: as above
			int prereq = type->Prerequisite[i]; // RA2: as above

			if (prereq >= 0) { // RA2: as above
				int item = 0; // RA2: as above
				if (ownedcount <= 0) { // RA2: as above
					return(false); // RA2: as above
				}
				while (true) { // RA2: as above
					if (owned[item] == BuildingTypes[prereq]) { // RA2: as above
						break;
					}
					item++; // RA2: the executable's do-while forms; a negative prerequisite names one of the rules lists (-1 power ... -6 refinery)
					if (item >= ownedcount) { // RA2: as above
						return(false); // RA2: as above
					}
				}

			} else {
				TypeList<BuildingTypeClass const *> * group = NULL; // RA2: -1 power, -2 weapons, -3 barracks, -4 radar, -5 tech, -6 refinery

				switch (prereq) { // RA2: as above
					case -1: // RA2: as above
						group = &Rule->BuildPower; // RA2: as above
						break; // RA2: as above

					case -2: // RA2: as above
						group = &Rule->BuildWeapons; // RA2: as above
						break; // RA2: as above

					case -3: // RA2: as above
						group = &Rule->BuildBarracks; // RA2: as above
						break; // RA2: as above

					case -4: // RA2: as above
						group = &Rule->BuildRadar; // RA2: as above
						break; // RA2: as above

					case -5: // RA2: as above
						group = &Rule->BuildTech; // RA2: as above
						break; // RA2: as above

					case -6: // RA2: as above
						group = &Rule->BuildRefinery; // RA2: as above
						break; // RA2: as above
				}

				int member = 0; // RA2: as above
				if (group->Count() > 0) { // RA2: as above
					do { // RA2: as above
						BuildingTypeClass const * candidate = group->GetItem(member); // RA2: the list's virtual GetItem
						int item = 0; // RA2: as above
						if (ownedcount > 0) { // RA2: as above
							do { // RA2: as above
								if (owned[item] == candidate) { // RA2: as above
									goto found; // RA2: a member of the list is owned; the structured forms (flag and breaks) compile to a longer loop
								}
								item++; // RA2: as above
							} while (item < ownedcount); // RA2: as above
						}
						member++; // RA2: as above
					} while (member < group->Count()); // RA2: as above
				}
				return(false); // RA2: as above
			}
found: // RA2: the shared exit of both searches; the structured form (a flag and breaks) was tested and compiles to a longer loop
			i++; // RA2: as above
		} while (i < type->Prerequisite.Count()); // RA2: as above
	}

	return(true);
}





/// <summary>
/// Builds the computer's base construction plan.
/// This routine fills the base node list with everything this house is allowed and able to
/// build, ordered so that a prerequisite is always queued ahead of whatever needs it. Extra
/// refineries and placeholders for base defenses are woven in along the way, according to
/// the difficulty level and the side's appetite for fortification.
/// </summary>
void HouseClass::Make_Base_Nodes(void) // RA2: the body follows the executable
{
	bool soviet = ActLike == 1; // RA2: the side picks which defense counts apply; no GDI/Nod name tests
	Base.Init(); // RA2: as above

	int owner = 1 << HouseTypeClass::From_Name(Class->ParentCountry.Buffer); // RA2: the bit of the country this house is a variant of
	DynamicVectorClass<BuildingTypeClass const *> eligible; // RA2: RA2's names for the TibSun lists
	DynamicVectorClass<bool> included; // RA2: as above
	int index; // RA2: as above
	for (index = 0; index < BuildingTypes.Count(); index++) { // RA2: as above
		BuildingTypeClass const * type = BuildingTypes[index]; // RA2: as above
		if ((type->Ownable & owner) && (type->AIBasePlanningSide == Class->Side || type->AIBasePlanningSide == -1) // RA2: as above
			&& type->CanAIBuildThis && type->Level <= Control.TechLevel // RA2: as above
			&& (type->RequiredHouses == -1 || (type->RequiredHouses & (1 << Class->House))) // RA2: as above
			&& (type->ForbiddenHouses == -1 || !(type->ForbiddenHouses & (1 << Class->House))) // RA2: as above
			&& (AllowDisableableSupers || type->SuperWeapon == -1 || !SuperWeapon[type->SuperWeapon]->Class->DisableableFromShell)) { // RA2: as above
			eligible.Add(type); // RA2: as above
			included.Add(false); // RA2: as above
		}
	}
	int eligible_count = eligible.Count(); // RA2: as above
	DynamicVectorClass<BuildingTypeClass const *> ordered; // RA2: as above
	BuildingTypeClass const * yard = Get_First_Ownable(Rule->BuildConst); // RA2: as above
	for (index = 0; index < eligible_count; index++) { // RA2: as above
		if (yard == eligible[index]) { // RA2: as above
			included[index] = true; // RA2: as above
			ordered.Add(yard); // RA2: as above
			break; // RA2: as above
		}
	}
	ordered.Add(Get_First_Ownable(Rule->BuildPower)); // RA2: as above
	BuildingTypeClass const * barracks = Get_First_Ownable(Rule->BuildBarracks); // RA2: as above
	for (index = 0; index < eligible.Count(); index++) { // RA2: as above
		if (eligible[index] == barracks) { // RA2: as above
			BuildingTypeClass const * first = eligible[0]; // RA2: as above
			eligible[0] = barracks; // RA2: as above
			included[index] = included[0]; // RA2: as above
			included[0] = false; // RA2: as above
			eligible[index] = first; // RA2: as above
			break; // RA2: as above
		}
	}
	BuildingTypeClass const * weapons = Get_First_Ownable(Rule->BuildWeapons); // RA2: as above
	for (index = 0; index < eligible.Count(); index++) { // RA2: as above
		if (eligible[index] == weapons) { // RA2: as above
			BuildingTypeClass const * second = eligible[1]; // RA2: as above
			eligible[1] = weapons; // RA2: as above
			included[index] = included[1]; // RA2: as above
			included[1] = false; // RA2: as above
			eligible[index] = second; // RA2: as above
			break; // RA2: as above
		}
	}
	int remaining = eligible_count - 1; // RA2: as above
	while (remaining > 0) { // RA2: as above
		int plug = -1; // RA2: as above
		bool added = false; // RA2: as above
		int queued = ordered.Count(); // RA2: as above
		for (index = 0; index < eligible_count; index++) { // RA2: as above
			if (included[index]) continue; // RA2: as above
			BuildingTypeClass const * type = eligible[index]; // RA2: as above
			if (stricmp(type->IniName, "GAPLUG") == 0) { // RA2: as above
				plug = index; // RA2: as above
				continue; // RA2: as above
			}
			if (AI_Has_Prerequisites(type, ordered, queued)) { // RA2: as above
				included[index] = true; // RA2: as above
				ordered.Add(type); // RA2: as above
				added = true; // RA2: as above
				remaining--; // RA2: as above
			}
		}
		if (!added) { // RA2: as above
			if (plug == -1) break; // RA2: as above
			included[plug] = true; // RA2: as above
			ordered.Add(eligible[plug]); // RA2: as above
			remaining--; // RA2: as above
		}
	}
	int extra_refineries = 2 - Difficulty; // RA2: as above
	BuildingTypeClass const * refinery = Get_First_Ownable(Rule->BuildRefinery); // RA2: as above
	int refinery_position = 0; // RA2: as above
	for (index = 0; index < ordered.Count() - 1; index++) { // RA2: as above
		if (ordered[index] == refinery) { // RA2: as above
			if (refinery_position != -1 && extra_refineries > 0) { // RA2: as above
				for (int n = 0; n < extra_refineries; n++) { // RA2: as above
					ordered.Insert_After(Random_Pick(refinery_position, ordered.Count() - 1), refinery); // RA2: as above
				}
			}
			break; // RA2: as above
		}
		refinery_position++; // RA2: as above
	}
	DynamicVectorClass<BuildingTypeClass const *> final_queue; // RA2: as above
	final_queue.Add(ordered[0]); // RA2: as above
	final_queue.Add(ordered[1]); // RA2: as above
	final_queue.Add(ordered[2]); // RA2: as above
	for (index = 3; index < ordered.Count(); index++) { // RA2: as above
		final_queue.Add(ordered[index]); // RA2: as above
	}
	int defenses = soviet ? Rule->SovietBaseDefenseCounts[Difficulty] : Rule->AlliedBaseDefenseCounts[Difficulty]; // RA2: the defense count comes from a rules list per difficulty
	for (; defenses > 0; defenses--) { // RA2: as above
		final_queue.Insert_After(Random_Pick(3, final_queue.Count() - 1), (BuildingTypeClass const *)-1); // RA2: as above
	}
	for (index = 0; index < final_queue.Count(); index++) { // RA2: as above
		BuildingTypeClass const * type = final_queue[index]; // RA2: as above
		if ((int)type < 0 && (int)type >= -4) { // RA2: as above
			Base.Nodes.Add(BaseNodeClass((StructType)(int)type, Cell(0, 0), false, 0)); // RA2: as above
		} else {
			Base.Nodes.Add(BaseNodeClass(type->HeapID, Cell(0, 0), false, 0)); // RA2: as above
		}
	}
}



/// <summary>
/// Scores a base cell by its distance from the base center.
/// This routine is the weight function used when a structure should simply go as near to the
/// middle of the base as it can be got. A cell close to the center scores lowest and is
/// therefore the one preferred.
/// </summary>
/// <param name="house">The house whose base center the distance is measured from.</param>
/// <param name="cell">The cell being scored.</param>
/// <param name="tie_breaker">Tie breaking bias to be added to the score.</param>
/// <returns>Returns with the weight of the cell; the lower the better.</returns>
int HouseClass::Base_Cell_Weight_By_Distance(HouseClass const & house, Cell const & cell, int tie_breaker, int context)
{
	int x = cell.X - house.Base.PlacementCenter.X;
	int y = cell.Y - house.Base.PlacementCenter.Y;

	int size = MAX(abs(x), abs(y));
	return(tie_breaker + size * 1000);
}


/// <summary>
/// Scores a base cell by how well it is already defended.
/// This routine is the weight function that the base defense planner hands to
/// Where_To_Place_Building. A cell that is thinly covered, and that lies toward the quadrant
/// under threat, scores lowest and is therefore the one preferred.
/// </summary>
/// <param name="house">The house whose base defense map is consulted.</param>
/// <param name="cell">The cell being scored.</param>
/// <param name="tie_breaker">Tie breaking bias to be added to the score.</param>
/// <param name="context">The quadrant to favor, or -1 to disregard direction entirely.</param>
/// <returns>Returns with the weight of the cell; the lower the better.</returns>
int HouseClass::Base_Cell_Weight_By_Defense_Coverage(HouseClass const & house, Cell const & cell, int tie_breaker, int context)
{
	BaseClass const &base = house.Base; // RA2: RA2's form
	int value = house.BaseAreaMap[cell.X - base.BaseAreaRect.X + (cell.Y - base.BaseAreaRect.Y) * base.BaseAreaRect.Width]; // RA2: as above
	int diff = 0; // RA2: as above
	if (context != FACING_NONE) {
		Point2D d; // RA2: as above
		d.X = cell.X - base.PlacementCenter.X; // RA2: as above
		d.Y = cell.Y - base.PlacementCenter.Y; // RA2: as above
		DirType dir = DirType(fastmath::atan2((double)-d.Y, (double)d.X)); // RA2: as above
		diff = abs(context * 32 - dir.As_Dir256()); // RA2: as above
		if (diff >= 128) { // RA2: as above
			diff = 255 - diff; // RA2: as above
		}
	}
	return tie_breaker + (value * 128 + diff) * 1000; // RA2: as above
}
struct Facing_Offset : Cell { // RA2: the element type of the static neighbour tables of Where_To_Place_Building; its destructor makes the executable register each table with atexit
	Facing_Offset(Cell const & cell) : Cell(cell) {} // RA2: as above
	~Facing_Offset() {} // RA2: as above
};

inline int Coord_Span(Coord const & a, Coord const & b) // RA2: the distance of two coordinates, as meastes has it
{
	return (a - b).Length(); // RA2: as above
}

inline int Coord_Span2(Coord const & a, Coord const & b) // RA2: the same distance with the square root inline, summed z, y, x
{
	Coord d; // RA2: the differences are stored one at a time, x first
	d.X = a.X - b.X; // RA2: as above
	d.Y = a.Y - b.Y; // RA2: as above
	d.Z = a.Z - b.Z; // RA2: as above
	double x = d.X; // RA2: as above
	double y = d.Y; // RA2: as above
	double z = d.Z; // RA2: as above
	return (int)fastmath::sqrt(z * z + y * y + x * x); // RA2: as above
}





/// <summary>
/// Determines where a structure should be placed within the computer's base.
/// This routine ranks the cells the base occupies with a weight function supplied by the
/// caller, then works outward from the best of them looking for a spot that is clear, level
/// with the base, and legal for the structure. The house's base spacing is honored on the
/// first pass and relaxed on the second, so that a cramped base still gets built up.
/// </summary>
/// <param name="buildingtype">The structure that needs somewhere to go.</param>
/// <param name="func">The weight function used to rank the candidate cells; lower wins.</param>
/// <param name="context">Extra context handed through to the weight function.</param>
/// <returns>Returns with the cell to build upon, or Cell(0,0) if the base has no room.</returns>
#ifdef RA2_NON_MATCHING // not exact: 803 of 2629 bytes differ (mischa85 text; the old-tree body is not carried yet)
Cell HouseClass::Where_To_Place_Building(BuildingTypeClass * buildingtype, int (__fastcall *func)(HouseClass const &, Cell const &, int, int), int context) // RA2: the executable places ship buildings by the yard rule and uses the static offset tables of Facing_Offset
{
	if (buildingtype->Naval) { // RA2: a ship building (the naval yard) is placed on the water next to the construction yard
		Cell no_cell(0, 0); // RA2: as above
		int width = Get_First_Ownable(Rule->Shipyard)->Width() + 2; // RA2: as above
		int height = Get_First_Ownable(Rule->Shipyard)->Height(false) + 2; // RA2: as above
		Cell center = Center.As_Cell(); // RA2: as above
		Cell spot = Map.Nearby_Location(center, SPEED_FLOAT, -1, MZONE_NORMAL, false, Point2D(width, height), false, false, false, true, no_cell, false); // RA2: as above
		if (spot != CELL_NONE) { // RA2: as above
			if (ConYards[0] != NULL) { // RA2: as above
				if (Coord_Span(Map[spot].Center_Coord(), ConYards[0]->Center_Coord()) > Rule->AINavalYardAdjacency * 256) { // RA2: as above
					return(CELL_NONE); // RA2: the "none" cell, not (0, 0)
				}
			}
		}
		return(spot); // RA2: as above
	} else {
		DynamicVectorClass<Cell> sorted_cells; // RA2: as above

		if (Base.PlacementCenter == Cell(0, 0)) { // RA2: as above
			return(Center.As_Cell()); // RA2: as above
		}

		IndexClass<int, Cell> ranking; // RA2: as above
		sorted_cells.Clear(); // RA2: as above

		int i; // RA2: as above
		for (i = 0; i < Base.InnerCells.Count(); i++) { // RA2: as above
			Cell cell = Base.InnerCells[i]; // RA2: as above
			int score; // RA2: as above
			if (!buildingtype->IsCloakGenerator) { // RA2: as above
				score = func(*this, cell, i, context); // RA2: as above
			} else {
				Coord cell_coord(cell, 0); // RA2: the cell's center as a coordinate, built by the inline Coord(Cell, z)
				score = i + 1000 * Coord_Span2(Center, cell_coord); // RA2: as above
			}
			ranking.Add_Index(score, cell); // RA2: as above
		}

		for (i = 0; i < ranking.Count(); i++) { // RA2: as above
			sorted_cells.Add(ranking.Fetch_By_Position(i)); // RA2: as above
		}

		int base_height = Map[Base.PlacementCenter].Height; // RA2: as above
		int house_mask = 1 << HeapID; // RA2: as above
		Cell base_cell; // RA2: as above
		for (int pass = 0; pass < 2; pass++) { // RA2: as above
			for (i = 0; i < sorted_cells.Count(); i++) { // RA2: as above
				base_cell = sorted_cells[i]; // RA2: as above
				Cell occupied(0, 0); // RA2: as above
				for (int face = 0; face < 8; face++) { // RA2: as above
					CellClass * cptr = &Map[Adjacent_Cell(base_cell, (FacingType)face)]; // RA2: as above
					if (cptr->OccupiedBy & house_mask) { // RA2: as above
						occupied = Adjacent_Cell(occupied, (FacingType)face); // RA2: as above
					}
				}
				if (occupied == Cell(0, 0)) { // RA2: as above
					continue; // RA2: as above
				}
				Point2D delta; // RA2: as above
				delta.X = -(int)occupied.X; // RA2: as above
				delta.Y = -(int)occupied.Y; // RA2: as above
				double angle = fastmath::atan2((double)-delta.Y, (double)delta.X); // RA2: as above
				DirType dir = DirType(angle); // RA2: as above
				int facing = dir.As_Facing(); // RA2: the facing of the direction
				Cell offset(0, 0); // RA2: as above
				int width = buildingtype->Width(); // RA2: as above
				int height = buildingtype->Height(false); // RA2: as above
				bool extra = buildingtype->ProtectWithWall || buildingtype->WantsExtraSpace; // RA2: walled and wide structures want one more cell of spacing
				int spacing = Rule->AIBaseSpacing; // RA2: as above
				if (extra) spacing++; // RA2: as above
				if (facing >= 1 && facing <= 3) { // RA2: as above
					offset.X = (short)spacing; // RA2: as above
				} else if (facing >= 5 && facing <= 7) { // RA2: as above
					offset.X = (short)(1 - width - spacing); // RA2: as above
				}
				if (facing >= 3 && facing <= 5) { // RA2: as above
					offset.Y = (short)(spacing + 1); // RA2: as above
				} else if (facing >= 7 || facing <= 1) { // RA2: as above
					offset.Y = (short)(1 - height - spacing); // RA2: as above
				}
				for (int trial = 0; trial < 2; trial++) { // RA2: as above
					static Facing_Offset scan_offsets[8] = { // RA2: as above
						Cell(-1, 0), Cell(-1, -1), Cell(0, -1), Cell(1, -1), // RA2: as above
						Cell(1, 0), Cell(1, 1), Cell(0, 1), Cell(-1, 1) // RA2: as above
					};
					static Facing_Offset step_offsets[8] = { // RA2: as above
						Cell(1, 0), Cell(1, 1), Cell(0, 1), Cell(-1, 1), // RA2: as above
						Cell(-1, 0), Cell(-1, -1), Cell(0, -1), Cell(1, -1) // RA2: as above
					};
					Cell pos = base_cell + offset + scan_offsets[facing]; // RA2: as above
					if (trial == 1) { // RA2: as above
						pos = Adjacent_Cell(pos, Facing_Sub(facing, 4)); // RA2: as above
						spacing -= Rule->AIBaseSpacing; // RA2: the second trial drops the spacing border
					}
					for (int attempt = 0; attempt < 3; attempt++) { // RA2: as above
						Rect rect(pos.X - spacing, pos.Y - spacing, width + spacing * 2, height + spacing * 2); // RA2: as above
						if (Map.Is_Area_Available(rect, HeapID)) { // RA2: no relaxed second pass
							if (buildingtype->Legal_Placement(pos, this)) { // RA2: as above
								if (abs(base_height - Map[pos].Height) < 3 && Can_Build_Here(buildingtype, pos)) { // RA2: as above
									return(pos); // RA2: as above
								}
							}
						}
						pos.X += step_offsets[facing].X; // RA2: as above
						pos.Y += step_offsets[facing].Y; // RA2: as above
					}
				}
			}
		}
		return(Cell(0, 0)); // RA2: as above
	}
}
#endif



/// <summary>
/// Determines where an upgrade should be placed.
/// This routine finds the structure this house owns that the upgrade attaches to and that
/// still has room for it, preferring the least upgraded one.
/// </summary>
/// <param name="upgrade">The upgrade that needs a home.</param>
/// <returns>Returns with the cell of the structure to upgrade, or Cell(0,0) if there is
/// none.</returns>
Cell HouseClass::Where_To_Place_Upgrade(BuildingTypeClass const * upgrade) const
{
	StructType upgradee_id = BuildingTypeClass::From_Name(upgrade->PowersUpBuilding);
	if (upgradee_id == STRUCT_NONE) {
		return(Cell(0, 0));
	}

	BuildingTypeClass const * upgradee_type = BuildingTypes[upgradee_id];
	BuildingClass * upgradee = NULL;
	int upgrade_level = upgradee_type->Upgrades + 10;
	bool anyslot = upgrade->PowersUpToLevel == -1;

	for (int i = 0; i < ::Buildings.Count(); i++) { // RA2: the search runs over every building in the game, the house test is made on each
		BuildingClass * b = ::Buildings[i]; // RA2: as above
		if (b->House == this && b->Class == upgradee_type && b->Can_Upgrade(upgrade, this)) {
			if (anyslot && upgrade_level > b->UpgradeLevel || !anyslot && b->UpgradeLevel == 0) {
				upgrade_level = b->UpgradeLevel;
				upgradee = b;
			}
		}
	}

	if (upgradee != NULL) {
		return(upgradee->PositionCoord.As_Cell());
	}

	return(Cell(0, 0));
}



/// <summary>
/// Clamps a cell so that it lies within a rectangle.
/// </summary>
/// <param name="bounds">The rectangle to confine the cell to.</param>
/// <param name="cell">The cell to be clamped.</param>
/// <returns>Returns with the nearest cell that lies inside the rectangle.</returns>
Cell Clamp_Cell(Rect const & bounds, Cell const & cell)
{
	Cell result = cell;
	if (result.X < bounds.X) {
		result.X = bounds.X;
	}
	if (result.X >= bounds.Width + bounds.X) {
		result.X = bounds.Width + bounds.X - 1;
	}
	if (result.Y < bounds.Y) {
		result.Y = bounds.Y;
	}
	if (result.Y >= bounds.Height + bounds.Y) {
		result.Y = bounds.Height + bounds.Y - 1;
	}
	return(result);
}


/// <summary>
/// Stamps a structure's defense value into the base defense map.
/// The value is spread across the cells about the structure and falls away with distance, so
/// that the base defense planner can see which parts of the base a given structure really
/// protects.
/// </summary>
/// <param name="building">The defending structure to stamp into the map.</param>
/// <param name="value">The defense value of the structure against the category being
/// mapped.</param>
/// <param name="values">The defense map to accumulate into.</param>
/// <param name="area">The area of the world that the defense map covers.</param>
void HouseClass::Calculate_Defense_Values(BuildingClass const * building, int value, int * values, Rect const & area) const
{
	if (value <= 0) value = 1;

	Cell center = building->PositionCoord.As_Cell();

	int x_min = MAX(center.X - 6, area.X);
	int x_max = MIN(area.X + area.Width, center.X + 6);
	int y_min = MAX(center.Y - 6, area.Y);
	int y_max = MIN(area.Y + area.Height, center.Y + 6);

	for (int y = y_min; y < y_max; ++y) {
		for (int x = x_min; x < x_max; ++x) {
			int y_offset = y - center.Y;
			double distance = fastmath::sqrt((double)((x - center.X) * (x - center.X) + y_offset * y_offset));
			if (distance < 6.0) {
				double adjusted_distance = distance;
				int x_offset = x - area.X;
				int area_y_offset = y - area.Y;
				if (adjusted_distance < 1.0) {
					adjusted_distance = 1.0;
				}
				int * cell_value = &values[x_offset + area.Width * area_y_offset];
				*cell_value += ((double)value / ((adjusted_distance - 1.0) * 0.1 + 1.0));
			}
		}
	}
}


/// <summary>
/// Settles a base defense structure onto a base node.
/// This routine is called by the base building logic when the next node of the plan calls
/// for a defense. It finds the weakest defended quadrant of the base, decides whether air,
/// armor or infantry cover is most lacking there, favors a cheap structure that answers that
/// need, and gives it a cell -- either one the enemy has been attacking through, or a fresh
/// spot within the base. Any wall node made redundant by the choice is dropped.
/// </summary>
/// <param name="nodeindex">The base node to fill in.</param>
/// <param name="cells">The threat cells to defend against, or NULL to simply defend the
/// base.</param>
/// <returns>bool; Was a defense settled upon the node?</returns>
#ifdef RA2_NON_MATCHING // not exact: 179 of 3264 bytes differ (mischa85 text; the old-tree body is not carried yet)
bool HouseClass::AI_Build_Defense(int nodeindex, DynamicVectorClass<Cell> * cells) // RA2: RA2's form
{
	int i; // RA2: as above

	int bestval; // RA2: assigned below, as meastes has it
	int bestidx = -1; // RA2: as above
	bestval = 999999; // RA2: as above

	int aair_arr[4]; // RA2: as above
	aair_arr[0] = 0; // RA2: as above
	aair_arr[1] = 0; // RA2: as above
	aair_arr[2] = 0; // RA2: as above
	aair_arr[3] = 0; // RA2: as above

	int aarr_arr[4]; // RA2: as above
	aarr_arr[0] = 0; // RA2: as above
	aarr_arr[1] = 0; // RA2: as above
	aarr_arr[2] = 0; // RA2: as above
	aarr_arr[3] = 0; // RA2: as above

	int agrnd_arr[4]; // RA2: as above
	agrnd_arr[0] = 0; // RA2: as above
	agrnd_arr[1] = 0; // RA2: as above
	agrnd_arr[2] = 0; // RA2: as above
	agrnd_arr[3] = 0; // RA2: as above

	int types[4]; // RA2: as above

	DynamicVectorClass<Cell> cells2[4]; // RA2: as above

	int size = Base.BaseAreaRect.Width * Base.BaseAreaRect.Height; // RA2: as above
	int *aair = new int[size]; // RA2: as above
	int *aarmor = new int[size]; // RA2: as above
	int *aground = new int[size]; // RA2: as above
	memset(aair, 0, size * sizeof(int)); // RA2: as above
	memset(aarmor, 0, size * sizeof(int)); // RA2: as above
	memset(aground, 0, size * sizeof(int)); // RA2: as above

	/*
	 * Bucket the incoming threat cells into the four quadrants around the
	 * base placement center.
	 */
	if (cells != NULL) { // RA2: as above
		for (i = 0; i < cells->Count(); i++) { // RA2: as above
			Cell offset = (*cells)[i] - Base.PlacementCenter; // RA2: as above
			Point2D delta(offset.X, offset.Y); // RA2: the offset as a Point2D
			DirType dir = DirType(fastmath::atan2((double)-delta.Y, (double)delta.X)); // RA2: as above
			cells2[dir.As_Dir4()].Add((*cells)[i]); // RA2: as above
		}
	}

	/*
	 * Sum the defense values of our existing buildings per quadrant and
	 * accumulate the per-cell defense maps.
	 */
	int count = Buildings.Count(); // RA2: the count is read once
	for (i = 0; i < count; i++) { // RA2: no owner test, the list is the house's own
		BuildingClass * building = Buildings[i]; // RA2: as above
		Cell offset = building->Get_Cell() - Base.PlacementCenter; // RA2: the building's cell
		if (offset.X != 0 || offset.Y != 0) { // RA2: as above
			Point2D delta(offset.X, offset.Y); // RA2: as above
			DirType dir = DirType(fastmath::atan2((double)-delta.Y, (double)delta.X)); // RA2: as above
			int quadrant = dir.As_Dir4(); // RA2: as above
			int air = building->Anti_Air_Defense_Value(); // RA2: as above
			int armor = building->Anti_Armor_Defense_Value(); // RA2: as above
			int infantry = building->Anti_Infantry_Defense_Value(); // RA2: as above
			if (air + armor + infantry > 0) { // RA2: as above
				aair_arr[quadrant] += air; // RA2: as above
				aarr_arr[quadrant] += armor; // RA2: as above
				agrnd_arr[quadrant] += infantry; // RA2: as above
				Calculate_Defense_Values(building, air, aair, Base.BaseAreaRect); // RA2: as above
				Calculate_Defense_Values(building, armor, aarmor, Base.BaseAreaRect); // RA2: as above
				Calculate_Defense_Values(building, infantry, aground, Base.BaseAreaRect); // RA2: as above
			}
		}
	}

	/*
	 * Pick the weakest defended quadrant.
	 */
	for (i = 0; i < 4; i++) { // RA2: as above
		types[i] = aair_arr[i] + aarr_arr[i] + agrnd_arr[i]; // RA2: as above
		if (types[i] < bestval && (cells == NULL || cells2[i].Count() > 0)) { // RA2: as above
			bestval = types[i]; // RA2: as above
			bestidx = i; // RA2: as above
		}
	}

	int air_value = aair_arr[bestidx]; // RA2: as above
	int armor_value = aarr_arr[bestidx]; // RA2: as above
	int ground_value = agrnd_arr[bestidx]; // RA2: as above

	Calculate_Enemy_Predictions(); // RA2: as above

	/*
	 * Build the list of non-defense building types we own. This serves as the
	 * prerequisite basis for the defense candidates.
	 */
	DynamicVectorClass<BuildingTypeClass const *> owned; // RA2: as above
	count = Buildings.Count(); // RA2: as above
	for (i = 0; i < count; i++) { // RA2: as above
		BuildingClass * building = Buildings[i]; // RA2: as above
		BuildingTypeClass * type = building->Class; // RA2: no owner test and no wall tower added for GDI
		if (!type->IsBaseDefense && type != Rule->WallTower) { // RA2: as above
			owned.Add(building->Class); // RA2: as above
		}
	}

	StructType type_id = Base.Nodes[nodeindex].Type; // RA2: as above
	BuildingTypeClass * pending = NULL; // RA2: as above

	/*
	 * Compare the share each defense category has in the chosen quadrant
	 * against the predicted enemy force composition.
	 */
	float air_ratio = bestval != 0 ? air_value / (float)bestval : 0.0f; // RA2: as above
	float armor_ratio = bestval != 0 ? armor_value / (float)bestval : 0.0f; // RA2: as above
	float ground_ratio = bestval != 0 ? ground_value / (float)bestval : 0.0f; // RA2: as above
	float air_deficit = EnemyAirForcePrediction - air_ratio; // RA2: as above
	float armor_deficit = EnemyArmorForcePrediction - armor_ratio; // RA2: as above
	float ground_deficit = EnemyInfantryForcePrediction - ground_ratio; // RA2: as above

	DynamicVectorClass<BuildingTypeClass *> air_defenses = Get_Anti_Air_Defense_Buildings(owned); // RA2: as above
	DynamicVectorClass<BuildingTypeClass *> armor_defenses = Get_Anti_Armor_Defense_Buildings(owned); // RA2: as above
	DynamicVectorClass<BuildingTypeClass *> ground_defenses = Get_Anti_Ground_Defense_Buildings(owned); // RA2: as above

	/*
	 * Address the category with the largest deficit first.
	 */
	DynamicVectorClass<BuildingTypeClass *> * defenses; // RA2: as above
	int category; // RA2: as above
	if (air_deficit <= armor_deficit || air_deficit <= ground_deficit) { // RA2: as above
		if (armor_deficit > ground_deficit) { // RA2: as above
			defenses = &armor_defenses; // RA2: as above
			BaseAreaMap = aarmor; // RA2: as above
			category = 1; // RA2: as above
		} else {
			defenses = &ground_defenses; // RA2: as above
			BaseAreaMap = aground; // RA2: as above
			category = 2; // RA2: as above
		}
	} else {
		defenses = &air_defenses; // RA2: as above
		BaseAreaMap = aair; // RA2: as above
		category = 0; // RA2: as above
	}

	/*
	 * If there are no candidates for the chosen category, fall back to any
	 * category that has them.
	 */
	bool success = true; // RA2: as above
	if (defenses->Count() == 0) { // RA2: as above
		if (armor_defenses.Count() > 0) { // RA2: as above
			defenses = &armor_defenses; // RA2: as above
			category = 1; // RA2: as above
			BaseAreaMap = aarmor; // RA2: as above
		} else if (ground_defenses.Count() > 0) { // RA2: as above
			defenses = &ground_defenses; // RA2: as above
			category = 2; // RA2: as above
			BaseAreaMap = aground; // RA2: as above
		} else if (air_defenses.Count() > 0) { // RA2: as above
			defenses = &air_defenses; // RA2: as above
			category = 0; // RA2: as above
			BaseAreaMap = aair; // RA2: as above
		} else {
			success = false; // RA2: as above
		}
	}

	if (success) { // RA2: as above

		/*
		 * Pick the defense to build: cheap buildings and buildings with a high
		 * value against the chosen category are favored.
		 */
		BuildingTypeClass * choice; // RA2: as above
		if (defenses->Count() == 1) { // RA2: as above
			choice = (*defenses)[0]; // RA2: as above
		} else {
			DiscreteDistributionClass<BuildingTypeClass> distribution; // RA2: as above
			for (i = 0; i < defenses->Count(); i++) { // RA2: as above
				BuildingTypeClass * type = (*defenses)[i]; // RA2: as above
				int weight = 10000 / type->Cost_Of(this); // RA2: as above
				if (category == 2) { // RA2: as above
					weight += type->AntiInfantryValue; // RA2: as above
				} else if (category == 1) { // RA2: as above
					weight += type->AntiArmorValue; // RA2: as above
				} else {
					weight += type->AntiAirValue; // RA2: as above
				}
				distribution.Add(type, weight); // RA2: as above
			}
			/*
			 * RA2: Sample()'s walk written out in place; the inline call is
			 * further from the original here.
			 */
			choice = NULL; // RA2: as above
			unsigned int cumulative = 0; // RA2: as above
			if (distribution.Get_Total_Weight() >= 1 && distribution.Count() != 0) { // RA2: as above
				unsigned int pick = Random_Pick<unsigned int>(1, distribution.Get_Total_Weight()); // RA2: as above
				for (int candidate = 0; candidate < distribution.Count(); candidate++) { // RA2: as above
					cumulative += distribution[candidate].Weight; // RA2: as above
					if (cumulative >= pick) { // RA2: as above
						choice = distribution[candidate].Object; // RA2: as above
						break; // RA2: as above
					}
				}
			}
		}

		/*
		 * If the node already has a type assigned, build that type and queue the
		 * picked defense at the following node instead.
		 */
		BuildingTypeClass * build_type; // RA2: as above
		if (type_id >= 0) { // RA2: as above
			build_type = BuildingTypes[type_id]; // RA2: as above
			pending = choice; // RA2: as above
		} else {
			build_type = choice; // RA2: as above
		}

		Cell cell(0, 0); // RA2: as above
		if (cells != NULL) { // RA2: as above
			IndexClass<int, Cell> cellidx; // RA2: as above
			DynamicVectorClass<Cell> cellvec; // RA2: as above
			cellvec.Clear(); // RA2: as above
			for (i = 0; i < cells2[bestidx].Count(); i++) { // RA2: as above
				Cell threat_cell = cells2[bestidx][i]; // RA2: as above
				cellidx.Add_Index(Base_Cell_Weight_By_Defense_Coverage(*this, threat_cell, i, -1), threat_cell); // RA2: as above
			}
			for (i = 0; i < cellidx.Count(); i++) { // RA2: as above
				cellvec.Add(cellidx.Fetch_By_Position(i)); // RA2: as above
			}
			cell = cellvec[0]; // RA2: as above
			cells->Delete_Index(cells->ID(cell)); // RA2: as above
		} else {
			cell = Where_To_Place_Building(build_type, Base_Cell_Weight_By_Defense_Coverage, 2 * bestidx); // RA2: as above
		}

		if (cell == Cell(0, 0)) { // RA2: as above
			success = false; // RA2: as above
		} else if (pending != NULL) { // RA2: as above
			Base.Nodes[nodeindex].CellID = cell; // RA2: as above
			if (Base.Nodes[nodeindex + 1].Type == STRUCT_NONE) { // RA2: as above
				Base.Nodes[nodeindex + 1].Type = (StructType)pending->HeapID; // RA2: as above
				Base.Nodes[nodeindex + 1].CellID = cell; // RA2: as above

				/*
				 * A wall node at this cell is now redundant -- remove it.
				 */
				if (cells != NULL) { // RA2: as above
					BuildingTypeClass const * wall = Get_First_Ownable(Rule->ConcreteWalls); // RA2: as above
					for (i = Base.Nodes.Count() - 1; i >= 0; i--) { // RA2: as above
						if (Base.Nodes[i].CellID == cell && Base.Nodes[i].Type == wall->HeapID) { // RA2: as above
							Base.Nodes.Delete_Index(i); // RA2: as above
							break; // RA2: as above
						}
					}
				}
			}
		} else {
			Base.Nodes[nodeindex].Type = (StructType)build_type->HeapID; // RA2: as above
			Base.Nodes[nodeindex].CellID = cell; // RA2: as above
		}
	}

	delete[] aair; // RA2: as above
	delete[] aarmor; // RA2: as above
	delete[] aground; // RA2: as above
	BaseAreaMap = NULL; // RA2: as above

	return(success); // RA2: as above
}
#endif



/// <summary>
/// Fetches the anti-aircraft defenses this house could build.
/// This routine gathers the candidates that the base defense planner will choose between.
/// Only structures this house may own, that are within its tech level, and whose
/// prerequisites are already accounted for, are listed.
/// </summary>
/// <param name="owned">The structures to count as owned when testing prerequisites.</param>
/// <returns>Returns with the list of candidates, which may well be empty.</returns>
DynamicVectorClass<BuildingTypeClass *> HouseClass::Get_Anti_Air_Defense_Buildings(DynamicVectorClass<BuildingTypeClass const *> & owned) const
{
	unsigned ownable = 1 << HouseTypeClass::From_Name(Class->ParentCountry.Buffer); // RA2: the house type is found by the name of its parent country
	DynamicVectorClass<BuildingTypeClass *> defenses;

	for (int i = 0; i < BuildingTypes.Count(); i++) {
		BuildingTypeClass * b = BuildingTypes[i];
		if (ownable & b->Ownable && b->AntiAirValue > 0 && b->Level <= Control.TechLevel && AI_Has_Prerequisites(b, owned, owned.Count())) {
			defenses.Add(b);
		}
	}

	return(defenses);
}



/// <summary>
/// Fetches the anti-armor defenses this house could build.
/// This routine gathers the candidates that the base defense planner will choose between.
/// Only structures this house may own, that are within its tech level, and whose
/// prerequisites are already accounted for, are listed.
/// </summary>
/// <param name="owned">The structures to count as owned when testing prerequisites.</param>
/// <returns>Returns with the list of candidates, which may well be empty.</returns>
DynamicVectorClass<BuildingTypeClass *> HouseClass::Get_Anti_Armor_Defense_Buildings(DynamicVectorClass<BuildingTypeClass const *> & owned) const
{
	unsigned ownable = 1 << HouseTypeClass::From_Name(Class->ParentCountry.Buffer); // RA2: the house type is found by the name of its parent country
	DynamicVectorClass<BuildingTypeClass *> defenses;

	for (int i = 0; i < BuildingTypes.Count(); i++) {
		BuildingTypeClass * b = BuildingTypes[i];
		if (ownable & b->Ownable && b->AntiArmorValue > 0 && b->Level <= Control.TechLevel && AI_Has_Prerequisites(b, owned, owned.Count())) {
			defenses.Add(b);
		}
	}

	return(defenses);
}



/// <summary>
/// Fetches the anti-infantry defenses this house could build.
/// This routine gathers the candidates that the base defense planner will choose between.
/// Only structures this house may own, that are within its tech level, and whose
/// prerequisites are already accounted for, are listed.
/// </summary>
/// <param name="owned">The structures to count as owned when testing prerequisites.</param>
/// <returns>Returns with the list of candidates, which may well be empty.</returns>
DynamicVectorClass<BuildingTypeClass *> HouseClass::Get_Anti_Ground_Defense_Buildings(DynamicVectorClass<BuildingTypeClass const *> & owned) const
{
	unsigned ownable = 1 << HouseTypeClass::From_Name(Class->ParentCountry.Buffer); // RA2: the house type is found by the name of its parent country
	DynamicVectorClass<BuildingTypeClass *> defenses;

	for (int i = 0; i < BuildingTypes.Count(); i++) {
		BuildingTypeClass * b = BuildingTypes[i];
		if (ownable & b->Ownable && b->AntiInfantryValue > 0 && b->Level <= Control.TechLevel && AI_Has_Prerequisites(b, owned, owned.Count())) {
			defenses.Add(b);
		}
	}

	return(defenses);
}



/// <summary>
/// Predicts the makeup of the enemy's forces.
/// The base defense planner consults these predictions to judge whether it is short of
/// anti-air, anti-armor or anti-infantry cover.
/// </summary>
void HouseClass::Calculate_Enemy_Predictions(void)
{
	EnemyInfantryForcePrediction = 0.33f;
	EnemyAirForcePrediction = 0.33f;
	EnemyArmorForcePrediction = 0.33f;
}


static FacingType const wall_step_facings[4] = { FACING_E, FACING_E, FACING_S, FACING_S }; // RA2: the direction each side is walked (ra2-decomp's twin text)
static FacingType const wall_adjacent_facings[4] = { FACING_N, FACING_S, FACING_W, FACING_E }; // RA2: and the neighbour each side is checked against

/// <summary>
/// Plans a perimeter wall around the computer's base.
/// This routine surveys the edges of the base area and adds wall nodes to the base plan
/// wherever the ground will take them, setting a gate into the middle of any run long enough
/// to need one. GDI additionally gets a share of wall towers to man the finished line.
/// </summary>
void HouseClass::AI_Build_Wall(void) // RA2: the body follows the executable (no GDI wall defenses); ra2-decomp's twin text
{
	Rect perimeter = Base.BaseAreaRect; // RA2: the base area is widened by one cell on every side into a local first
	perimeter.X--; // RA2: as above
	perimeter.Y--; // RA2: as above
	perimeter.Width += 2; // RA2: as above
	perimeter.Height += 2; // RA2: as above
	Base.LastBaseAreaRect = perimeter; // RA2: as above

	DynamicVectorClass<BaseNodeClass> walls; // RA2: RA2's names; the wall nodes
	DynamicVectorClass<BaseNodeClass> gates; // RA2: and the gate nodes, both appended to the plan at the end

	Cell starts[4] = { // RA2: the four sides' first cells, built from the local rectangle
		Cell(perimeter.X, perimeter.Y), // RA2: as above
		Cell(perimeter.X, perimeter.Y + perimeter.Height - 1), // RA2: as above
		Cell(perimeter.X, perimeter.Y + 1), // RA2: as above
		Cell(perimeter.X + perimeter.Width - 1, perimeter.Y + 1) // RA2: as above
	};
	Cell ends[4] = { // RA2: and their end cells
		Cell(perimeter.X + perimeter.Width, perimeter.Y), // RA2: as above
		Cell(perimeter.X + perimeter.Width, perimeter.Y + perimeter.Height - 1), // RA2: as above
		Cell(perimeter.X, perimeter.Y + perimeter.Height - 1), // RA2: as above
		Cell(perimeter.X + perimeter.Width - 1, perimeter.Y + perimeter.Height - 1) // RA2: as above
	};

	BuildingTypeClass const * wall = Get_First_Ownable(Rule->ConcreteWalls); // RA2: as TibSun
	BuildingTypeClass const * east_west_gate = Get_First_Ownable(Rule->EWGates); // RA2: as above
	BuildingTypeClass const * north_south_gate = Get_First_Ownable(Rule->NSGates); // RA2: as above
	BuildingTypeClass const * gate_types[4] = {east_west_gate, east_west_gate, north_south_gate, north_south_gate}; // RA2: as above
	Cell beginning(0, 0); // RA2: as above
	int elevation = Map[Base.PlacementCenter].Height; // RA2: as above
	int side; // RA2: as above; also counts the nodes appended at the end
	for (side = 0; side < 4; side++) { // RA2: as above
		FacingType direction = wall_step_facings[side]; // RA2: as above
		Cell cell = starts[side]; // RA2: as above
		int length = 0; // RA2: as above
		bool running = false; // RA2: as above
		int soft_blocks = 0; // RA2: as above
		while (cell != ends[side]) { // RA2: as above
			CellClass * current = &Map[cell]; // RA2: as above
			CellClass * neighbor = &Map[Adjacent_Cell(cell, wall_adjacent_facings[side])]; // RA2: as above
			LandType land = current->Land_Type(); // RA2: as above
			LandType adjacent_land = neighbor->Land_Type(); // RA2: as above
			int height_diff = abs(current->Height - elevation); // RA2: as above
			bool level = height_diff <= 2; // RA2: as above
			bool ground = land != LAND_ROCK && land != LAND_WATER && land != LAND_ICE && adjacent_land != LAND_ROCK && adjacent_land != LAND_WATER && adjacent_land != LAND_ICE; // RA2: as above
			bool overlay = current->Overlay == OVERLAY_NONE; // RA2: as above
			bool buildings = !current->Cell_Building() && !neighbor->Cell_Building(); // RA2: as above
			bool terrain = !current->Cell_Terrain() && !neighbor->Cell_Terrain(); // RA2: as above
			bool flat = current->Ramp == 0; // RA2: as above
			bool visible = Map.In_Local_Radar(cell); // RA2: as above
			if (level && ground && overlay && buildings && terrain && flat && visible) { // RA2: as above
				if (!running) { // RA2: as above
					running = true; // RA2: as above
					length = 1; // RA2: as above
					beginning = cell; // RA2: as above
				} else {
					length++; // RA2: as above
				}
			} else {
				bool hard_blocked = !(level && ground && buildings && terrain && visible); // RA2: as above
				if (hard_blocked) { // RA2: as above
					if (soft_blocks > 0) { // RA2: as above
						hard_blocked = false; // RA2: as above
						soft_blocks = 0; // RA2: as above
					}
				} else {
					soft_blocks++; // RA2: as above
				}
				if (length >= 5 || !hard_blocked) { // RA2: as above
					Cell position = beginning; // RA2: as above
					int placed = 0; // RA2: as above
					int middle = length / 2 - 1; // RA2: as above
					while (placed < length) { // RA2: as above
						if (placed == middle && hard_blocked) { // RA2: as above
							gates.Add(BaseNodeClass(gate_types[side]->HeapID, position)); // RA2: as above
							placed += 3; // RA2: as above
							position = Adjacent_Cell(Adjacent_Cell(Adjacent_Cell(position, direction), direction), direction); // RA2: as above
						} else {
							walls.Add(BaseNodeClass(wall->HeapID, position)); // RA2: as above
							placed++; // RA2: as above
							position = Adjacent_Cell(position, direction); // RA2: as above
						}
					}
				}
				length = 0; // RA2: as above
				running = false; // RA2: as above
			}
			cell = Adjacent_Cell(cell, direction); // RA2: as above
		}
		if (length >= 5) { // RA2: as above
			Cell position = beginning; // RA2: as above
			int placed = 0; // RA2: as above
			int middle = length / 2 - 1; // RA2: as above
			while (placed < length) { // RA2: as above
				if (placed == middle) { // RA2: as above
					gates.Add(BaseNodeClass(gate_types[side]->HeapID, position)); // RA2: as above
					placed += 3; // RA2: as above
					position = Adjacent_Cell(Adjacent_Cell(Adjacent_Cell(position, direction), direction), direction); // RA2: as above
				} else {
					walls.Add(BaseNodeClass(wall->HeapID, position)); // RA2: as above
					placed++; // RA2: as above
					position = Adjacent_Cell(position, direction); // RA2: as above
				}
			}
		}
	}
	for (side = 0; side < walls.Count(); side++) { // RA2: no GDI wall defenses and no outer cells follow
		Base.Nodes.Add(walls[side]); // RA2: as above
	}
	for (side = 0; side < gates.Count(); side++) { // RA2: as above
		Base.Nodes.Add(gates[side]); // RA2: as above
	}
	Base.BaseAreaRect = Base.LastBaseAreaRect; // RA2: as above
}



/// <summary>
/// Recalculates the power output and drain of this house.
/// This routine tallies every structure the house has running and then refreshes whatever
/// depends on the result -- factory speed and super weapon availability among them. An
/// undiscovered structure contributes nothing while the player is still to find it.
/// </summary>
void HouseClass::Recalc_Power_Drain(void)
{
	bool was_low = Power_Fraction() < 1.0; // RA2: RA2's form
	RecalcPower = false;
	Power = 0;
	Drain = 0;
	int count = Buildings.Count(); // RA2: as above
	for (int index = 0; index < count; index++) {
		BuildingClass * building = Buildings[index];
		if (building != NULL && !building->IsInLimbo && building->IsDown) { // RA2: as above
			if (!Is_Player_Control() || building->IsDiscoveredByPlayer || Session.Type != GAME_NORMAL) { // RA2: as above
				Power += building->Power_Output(); // RA2: as above
				Drain += building->Power_Drain(); // RA2: as above
			}
		}
	}
	if (PowerBlackoutTimer.Value() != 0) { // RA2: as above
		Power = 0;
	}
	Adjust_House_Power(this);
	Recalc_House_Factories(this);
	if (was_low != (Power_Fraction() < 1.0)) { // RA2: as above
		Update_Present_Super_Weapons();
	}

	RecalcRadar = true;
}




/// <summary>
/// Determines whether the radar map should be available.
/// This routine is called after anything that might switch the radar on or off -- a change
/// in power, an ion storm, or the loss of the radar structure itself. The tactical map is
/// told to raise or lower the radar to suit. Only the player's house has a radar to manage.
/// </summary>
void HouseClass::Recalc_Radar_Availability(void)
{
	RecalcRadar = false;

	if (this == PlayerPtr) {
		bool radar = false; // RA2: RA2's form
		if (RadarBlackoutTimer == 0) { // RA2: as above
			if (Scen->IsFreeRadar) {
				radar = true; // RA2: as above
			} else if (Power_Fraction() >= 1.0) { // RA2: as above
				for (int index = 0; index < Buildings.Count(); index++) {
					BuildingClass * building = Buildings[index];
					if (building != NULL && building->IsOn && building->Class->IsRadar && !building->IsInLimbo && building->IsDown // RA2: as above
						&& ((Session.Type == GAME_NORMAL && !IsHuman && !IsPlayerControl) || building->IsDiscoveredByPlayer || Session.Type != GAME_NORMAL) // RA2: as above
						&& building->CurrentMission != MISSION_DECONSTRUCTION && building->MissionQueue != MISSION_DECONSTRUCTION) { // RA2: as above
						if (building->StunDuration == 0 && !building->IsBeingWarpedOut()) { // RA2: as above
							radar = true; // RA2: as above
						}
						break;
					}
				}
			}
		}
		if (Map.Is_Radar_Existing() != radar) { // RA2: as above
			Map.Toggle_Radar(radar); // RA2: as above
		}
	}
}




void HouseClass::Update_Spy_Satellite(void) // RA2: RA2's form
{
	RecalcRadar = false;
	if (this == PlayerPtr) {
		for (int index = 0; index < Buildings.Count(); index++) {
			BuildingClass * building = Buildings[index];
			if (building != NULL && building->Class->SpySat && !building->IsInLimbo && building->IsDown // RA2: as above
				&& ((Session.Type == GAME_NORMAL && !IsHuman && !IsPlayerControl) || building->IsDiscoveredByPlayer || Session.Type != GAME_NORMAL) // RA2: as above
				&& building->CurrentMission != MISSION_DECONSTRUCTION && building->MissionQueue != MISSION_DECONSTRUCTION) { // RA2: as above
				if (!SpySatActive) { // RA2: as above
					VocClass::PlayGlobal(Rule->SpySatActivationSound, 1.0f, 0x2000, 0); // RA2: as above
					Map.Spy_Satellite_Reveal(); // RA2: as above
					SpySatActive = true; // RA2: as above
				}
				return;
			}
		}
		if (SpySatActive) { // RA2: as above
			Map.Spy_Satellite_Hide(); // RA2: as above
			VocClass::PlayGlobal(Rule->SpySatDeactivationSound, 1.0f, 0x2000, 0); // RA2: as above
			SpySatActive = false; // RA2: as above
		}
	}
}



/// <summary>
/// Fetches an unused waypoint path for this house.
/// This routine is used when the player starts plotting a fresh patrol or move path and
/// needs somewhere to record it.
/// </summary>
/// <returns>Returns with the path found, or PATH_NONE if every path is already in use.</returns>
PathType HouseClass::New_Waypoint_Path(void) const
{
	for (int i = 0; i < PATH_COUNT; i++) {
		WaypointPathClass * path = Paths[i];
		if (path == NULL) {
			path = new WaypointPathClass();
		}
		if (path->Waypoint_Count() == 0) {
			return((PathType)i);
		}
	}
	return(PATH_NONE);
}


/// <summary>
/// May another waypoint be added to the selected path?
/// This routine is used by the waypoint plotting mode to decide whether the player is
/// allowed to drop another marker. A path that has reached its length limit, or that is
/// already being traveled, is closed to additions.
/// </summary>
/// <returns>bool; Can the currently selected path take another waypoint?</returns>
bool HouseClass::Can_Add_Waypoint_To_Path(void) const
{
	if (SelectedPath < 0 || SelectedPath >= PATH_COUNT) return(false);
	WaypointPathClass * path = Paths[SelectedPath];
	return(path->Waypoint_Count() < Rule->MaxWaypointPathLength && path->Current_Waypoint() == -1);
}


/// <summary>
/// Enables threat rating for this house.
/// A threat rating structure lets the house weigh targets by the coefficients of the unit
/// doing the choosing rather than by the blunt default rules. This routine is called when
/// such a structure comes into the house's possession.
/// </summary>
void HouseClass::Activate_Threat_Node(void)
{
	IsThreatRatingNodeActive = true;
}


/// <summary>
/// Brings a production factory back in line with what may still be built.
/// This routine is called whenever the house gains or loses something that changes its build
/// options. Objects that can no longer be built are dropped from the queue, production of a
/// now illegal object is abandoned, and an object that is merely unavailable for the moment
/// is suspended until it may proceed again.
/// </summary>
/// <param name="rtti">Which of the house's factories to bring up to date.</param>
void HouseClass::Update_Factories(RTTIType rtti, bool naval, int category) // RA2: 0x4F3170, takes the ship flag and the build category too
{
	FactoryClass * factory = NULL;

	switch (rtti) {
		case RTTI_BUILDINGTYPE:
			if (category == 5) { // RA2: defenses have their own primary factory
				factory = Primary_ForDefenses; // RA2: as above
			} else {
				factory = BuildingFactory; // RA2: as above
			}
			break;

		case RTTI_UNITTYPE:
			if (naval) { // RA2: so have ships
				factory = Primary_ForShips; // RA2: as above
			} else {
				factory = UnitFactory; // RA2: as above
			}
			break;

		case RTTI_INFANTRYTYPE:
			factory = InfantryFactory;
			break;

		case RTTI_AIRCRAFTTYPE:
			factory = AircraftFactory;
			break;
	}

	if (factory != NULL) {
		for (int i = factory->QueuedObjects.Count() - 1; i >= 0; i--) {
			TechnoTypeClass const * ttype = factory->QueuedObjects[i];
			if (ttype->Who_Can_Build_Me(true, false, true, this) == NULL) {
				factory->QueuedObjects.Delete_Index(i);
			}
		}
		if (factory->Object != NULL) {
			if (factory->Object->TClass->Who_Can_Build_Me(true, false, true, this) == NULL) {
				factory->Abandon();
				factory->Resume_Queue();
			} else {
				if (factory->Object->TClass->Who_Can_Build_Me(true, true, true, this) == NULL) {
					factory->Suspend(false);
					if (PlayerPtr == this) {
						Map.Flag_Tab_To_Redraw(Sidebar_Which_Tab_By_Category(rtti, category, naval)); // RA2: only the tab of the kind is redrawn
					}
				} else {
					if (factory->IsSuspended && !factory->IsOnHold) {
						factory->Start(false);
					}
				}
			}
		}
		if (factory->Object == NULL && factory->QueuedObjects.Count() == 0) {
			delete factory;
		}
	}
}



/// <summary>
/// Records that another house has spied upon this house's radar.
/// When it is the player doing the spying, this routine also reveals the map around
/// everything this house owns, since a spied radar hands its owner a look at the entire
/// force.
/// </summary>
/// <param name="house">The house that performed the spying.</param>
void HouseClass::Update_Spied_Radar(HouseClass * house) // RA2: 0x4F3320
{
	RadarSpied |= 1 << house->Class->House; // RA2: as above
	if (house == PlayerPtr) { // RA2: as above
		for (int index = 0; index < Technos.Count(); index++) { // RA2: as above
			TechnoClass * obj = Technos[index]; // RA2: as above
			if (obj && !obj->IsInLimbo && obj->House == this) { // RA2: as above
				Map.RevealArea1(obj->PositionCoord, obj->TClass->SightRange, this, false, false, false, true, true); // RA2: the executable calls RevealArea1 (0x549C50) with both trailing flags set, not TibSun's Sight_From
			}
		}
		Map.Flag_To_Redraw(GS_REDRAW_ALL); // RA2: as above
	}
}




/// <summary>
/// Clears the anger this house holds against another house.
/// This routine is used when a grudge should be dropped -- when an alliance is formed, for
/// example. The house reconsiders which enemy it hates most as a result.
/// </summary>
/// <param name="house">The house to be forgiven.</param>
void HouseClass::Clear_Anger(HouseClass const * house)
{
	for (int i = 0; i < AngerNodes.Count(); i++) {
		if (AngerNodes[i].House == house) {
			Add_Anger(-AngerNodes[i].Level, house);
			break;
		}
	}
}


/// <summary>
/// Rebuilds this house's threat map.
/// Every object that this house has reason to fear adds its risk rating to the map region
/// it stands in. The computer consults the resulting map whenever it needs to know which
/// parts of the world are dangerous -- for target selection, base planning and team
/// routing alike.
/// </summary>
void HouseClass::Recalc_Threat_Regions(void)
{
	for (int region = 0; region < 0x4204; region++) { // RA2: RA2's form
		Regions[region].Reset_Threat(); // RA2: as above
	}
	for (int index = Technos.Count() - 1; index >= 0; index--) { // RA2: as above
		TechnoClass * techno = Technos[index];
		if (techno->IsInLimbo || !techno->IsActive) { // RA2: as above
			continue;
		}
		int threat = techno->Risk(); // RA2: as above
		if (threat <= 0) { // RA2: as above
			continue;
		}
		FootClass * foot = (techno->Fetch_RTTI() == RTTI_INFANTRY || techno->Fetch_RTTI() == RTTI_UNIT || techno->Fetch_RTTI() == RTTI_AIRCRAFT) ? (FootClass *)techno : NULL; // RA2: as above
		if (foot != NULL) {
			if (foot->LastAdjacencyCell == CELL_NONE) { // RA2: as above
				continue;
			}
			int region = Map.Cell_Region(foot->LastAdjacencyCell);
			if (foot->House == this) { // RA2: as above
				continue;
			}
			if (Is_Human_Player() && Is_Ally(foot->House)) { // RA2: as above
				continue;
			}
			Adjust_Threat(region, threat); // RA2: as above
		} else {
			Adjust_Threat(Map.Cell_Region(((CellClass *)techno->Get_Cell_Ptr())->CellID), threat); // RA2: as above
		}
	}
}




/// <summary>
/// Counts the teams of a given type that this house owns.
/// This routine is used by the team creation logic so that a team type's allowed count is
/// not exceeded.
/// </summary>
/// <returns>Returns with the number of teams of that type belonging to this house.</returns>
int HouseClass::Owned_Team_Count(TeamTypeClass const * teamtype) const
{
	int count = 0;
	for (int i = Teams.Count() - 1; i >= 0; i--) {
		TeamClass * team = Teams[i];
		if (team && team->House == this && teamtype == team->Class) {
			count++;
		}
	}
	return(count);
}


/// <summary>
/// Can this house field the specified team type?
/// Every member of the team's task force must be something this house is able to build.
/// Outside of campaign games an existing recruitable unit of that type will serve just as
/// well, so that a house can still assemble teams from what it happens to own.
/// </summary>
/// <returns>bool; Can the team be created by this house?</returns>
bool HouseClass::Can_Create_Team(TeamTypeClass const * teamtype)
{
	if (teamtype == NULL || teamtype->TaskForce == NULL) { // RA2: RA2's form
		return(false);
	}
	for (int index = 0; index < teamtype->TaskForce->ClassCount; index++) { // RA2: as above
		TechnoTypeClass const * type = teamtype->TaskForce->Members[index].Class; // RA2: as above
		if (type == NULL || type->Who_Can_Build_Me(true, true, false, this) == NULL) { // RA2: as above
			if (Session.Type != GAME_NORMAL) {
				return(false);
			}
			bool found = false;
			for (int foot_index = Feet.Count() - 1; foot_index >= 0; foot_index--) { // RA2: as above
				FootClass * foot = Feet[foot_index]; // RA2: as above
				if (foot->House == this && type == foot->Techno_Type_Class() && teamtype->Can_Recruit(foot, this)) { // RA2: as above
					found = true;
					break;
				}
			}
			if (!found) {
				return(false);
			}
		}
	}
	return(true);
}




/// <summary>
/// Updates what the computer is willing to spend its money on.
/// A hard up computer house alternates between structures and units rather than trying to
/// pay for both at once, favoring whichever it did not just finish, and it will insist
/// on structures while it lacks a barracks or a war factory or is running short of power.
/// Once the treasury recovers it goes back to building everything.
/// </summary>
/// <param name="type">The RTTI type of the object production has just dealt with.</param>
void HouseClass::Update_Production_Mode(RTTIType type)
{
	if (Is_Human_Player() || Session.Type == GAME_NORMAL) return;

	switch (ProductionMode) {
		case BUILDINGS:
			if (Available_Money() < Rule->AIAlternateProductionCreditCutoff) {
				if (type == RTTI_BUILDING) {
					ProductionMode = UNITS;
				}
			} else {
				ProductionMode = EVERYTHING;
			}
			break;

		case UNITS:
			if (Available_Money() < Rule->AIAlternateProductionCreditCutoff) {
				bool hasbarracks = false;
				for (int i = 0; i < Rule->BuildBarracks.Count(); i++) {
					if (!hasbarracks && ABQuantity.Value(Rule->BuildBarracks[i]->HeapID) <= 0) {
						hasbarracks = false;
					} else {
						hasbarracks = true;
					}
				}

				bool hasweapons = false;
				for (int j = 0; j < Rule->BuildWeapons.Count(); j++) {
					if (!hasweapons && ABQuantity.Value(Rule->BuildWeapons[j]->HeapID) <= 0) {
						hasweapons = false;
					} else {
						hasweapons = true;
					}
				}

				if (!hasweapons || !hasbarracks || Drain > Power || Random_Pick(0, 1) == 0) {
					if (type != RTTI_BUILDING) {
						ProductionMode = BUILDINGS;
					}
				}
			} else {
				ProductionMode = EVERYTHING;
			}
			break;

		case NOTHING:
			if (Available_Money() > Rule->AIAlternateProductionCreditCutoff) {
				ProductionMode = EVERYTHING;
			}
			break;

		case EVERYTHING:
			if (Available_Money() < Rule->AIAlternateProductionCreditCutoff) {
				ProductionMode = (type == RTTI_BUILDING) ? UNITS : BUILDINGS;
			}
			break;
	}
}


/// <summary>
/// Handles the computer's use of its charged super weapons.
/// This routine is called from the house AI. Every super weapon that has finished
/// charging is handed to the handler for its type, which picks a target and fires it.
/// Human houses are left to fire their own.
/// </summary>
void HouseClass::AI_Super_Weapons(void)
{
	Cell target_0; // RA2: RA2's form
	Cell target_2; // RA2: as above
	if (!Is_Human_Player()) {
		for (int index = 0; index < SuperWeapon.Count(); index++) { // RA2: as above
			SuperClass * super = SuperWeapon[index]; // RA2: as above
			if (super != NULL && super->Is_Ready()) {
				switch (super->Class->Type) {
					case SUPER_MULTI_MISSILE: { // RA2: as above
						SuperClass * pending = super; // RA2: as above
						if (Enemy != -1) { // RA2: as above
							target_0 = CELL_NONE; // RA2: as above
							if (PreferredTarget == 1) { // RA2: as above
								target_0 = Best_Super_Weapon_Target(); // RA2: as above
							} else {
								target_0 = Threat_Target_Cell(PreferredTarget); // RA2: as above
							}
							if (target_0 != CELL_NONE) { // RA2: as above
								Place_Special_Blast((SuperWeaponType)SuperWeapon.ID(pending), target_0); // RA2: as above
							}
						}
						break;
					}
					case SUPER_LIGHTNING_STORM: { // RA2: as above
						SuperClass * pending = super; // RA2: as above
						if (!IonStormClass::Is_Ion_Storm_Active() && Enemy != -1) { // RA2: as above
							target_2 = CELL_NONE; // RA2: as above
							if (PreferredTarget == 1) { // RA2: as above
								target_2 = Best_Super_Weapon_Target(); // RA2: as above
							} else {
								target_2 = Threat_Target_Cell(PreferredTarget); // RA2: as above
							}
							if (target_2 != CELL_NONE) { // RA2: as above
								Place_Special_Blast((SuperWeaponType)SuperWeapon.ID(pending), target_2); // RA2: as above
							}
						}
						break;
					}
					case SUPER_PARA_DROP: // RA2: as above
					case SUPER_AMER_PARA_DROP: // RA2: as above
						AI_Drop_Pods(super);
						break;
				}
			}
		}
	}
}





/// <summary>
/// Handles the computer's use of the multi-missile super weapon.
/// The enemy structure standing in the most threatening spot is chosen as the target. A
/// building hidden by a cloaking field is rated at random instead, so that the computer
/// does not appear to see through it.
/// </summary>
void HouseClass::AI_Multi_Missile(SuperClass * super)
{
	if (Enemy != -1) { // RA2: RA2's form
		Cell cell = CELL_NONE; // RA2: as above
		if (PreferredTarget == 1) { // RA2: as above
			cell = Best_Super_Weapon_Target(); // RA2: as above
		} else {
			cell = Threat_Target_Cell(PreferredTarget); // RA2: as above
		}
		if (cell != CELL_NONE) { // RA2: as above
			Place_Special_Blast((SuperWeaponType)SuperWeapon.ID(super), cell);
		}
	}
}




/// <summary>
/// Handles the computer's use of the hunter seeker super weapon.
/// The drone hunts down a victim of its own choosing, so this routine merely turns it
/// loose once the house has an enemy to send it after.
/// </summary>
void HouseClass::AI_Hunter_Seeker(SuperClass * super)
{
	if (Enemy != -1) { // RA2: RA2's form
		Place_Special_Blast((SuperWeaponType)SuperWeapon.ID(super), CELL_NONE); // RA2: as above
	}
}




/// <summary>
/// Handles the computer's use of the chemical missile super weapon.
/// The enemy structure standing in the most threatening spot is chosen as the target. A
/// building hidden by a cloaking field is rated at random instead, so that the computer
/// does not appear to see through it.
/// </summary>
void HouseClass::AI_Chem_Missile(SuperClass * super)
{
	if (Enemy != -1) { // RA2: RA2's form
		Cell cell = CELL_NONE; // RA2: as above
		if (PreferredTarget == 1) { // RA2: as above
			cell = Best_Super_Weapon_Target(); // RA2: as above
		} else {
			cell = Threat_Target_Cell(PreferredTarget); // RA2: as above
		}
		if (cell != CELL_NONE) { // RA2: as above
			Place_Special_Blast((SuperWeaponType)SuperWeapon.ID(super), cell);
		}
	}
}




void HouseClass::AI_Super_Unused(SuperClass * super) // RA2: RA2's form
{
}



/// <summary>
/// Handles the computer's use of the drop pod super weapon.
/// This routine picks a landing site out along a randomly chosen edge of the map and
/// fires the super weapon at it, provided the house has an enemy worth harassing.
/// </summary>
void HouseClass::AI_Drop_Pods(SuperClass * super)
{
	HouseClass * house = (Enemy != -1) ? Houses[Enemy] : this; // RA2: RA2's form
	Cell cell = CELL_NONE; // RA2: as above
	if (PreferredTarget != 1) { // RA2: as above
		cell = Threat_Target_Cell(PreferredTarget); // RA2: as above
	} else {
		cell = house->Center.As_Cell(); // RA2: as above
		cell = Map.Nearby_Location(cell, SPEED_FOOT, -1, MZONE_NORMAL, false, Point2D(5, 5), false, false, false, true, Cell(0, 0), false); // RA2: as above
		cell.X += 2; // RA2: as above
		cell.Y += 2; // RA2: as above
	}
	if (cell != CELL_NONE) { // RA2: as above
		Place_Special_Blast((SuperWeaponType)SuperWeapon.ID(super), cell);
	}
}




/// <summary>
/// Handles the computer's use of the ion cannon super weapon.
/// This routine rates the enemy's objects by how much their loss would hurt -- engineers,
/// vehicle thieves, construction yards, war factories and power plants are all prized
/// well above ordinary troops -- and strikes one of the best rated. Only objects that
/// would actually die to the blast earn their premium rating, and cloaked objects are
/// rated at random so the computer does not appear to see through them.
/// </summary>
void HouseClass::AI_Ion_Cannon(SuperClass * super)
{
	if (!IonStormClass::Is_Ion_Storm_Active() && Enemy != -1) { // RA2: RA2's form
		Cell cell = CELL_NONE; // RA2: as above
		if (PreferredTarget == 1) { // RA2: as above
			cell = Best_Super_Weapon_Target(); // RA2: as above
		} else {
			cell = Threat_Target_Cell(PreferredTarget); // RA2: as above
		}
		if (cell != CELL_NONE) { // RA2: as above
			Place_Special_Blast((SuperWeaponType)SuperWeapon.ID(super), cell);
		}
	}
}



/// <summary>
/// Handles the computer's use of the chemical missile super weapon.
/// The enemy structure standing in the most threatening spot is chosen as the target. A
/// building hidden by a cloaking field is rated at random instead, so that the computer
/// does not appear to see through it.
/// </summary>

void HouseClass::AI_Strike_Best_Building(SuperClass * super) // RA2: RA2's form
{
	if (Enemy != -1) { // RA2: as above
		BuildingClass * best = NULL; // RA2: as above
		int value = -1; // RA2: as above
		HouseClass * house = Houses[Enemy]; // RA2: as above
		for (int index = ::Buildings.Count() - 1; index >= 0; index--) { // RA2: the global list
			BuildingClass * building = ::Buildings[index]; // RA2: as above
			if (building->House == house) { // RA2: as above
				int rating = Map.Cell_Threat(building->Center_Coord().As_Cell(), *this); // RA2: as above
				if (building->TranslucencyLevel == 15) { // RA2: as above
					rating = Random_Pick(0, 100); // RA2: as above
				}
				if (rating > value) { // RA2: as above
					value = rating; // RA2: as above
					best = building; // RA2: as above
				}
			}
		}
		if (best != NULL) { // RA2: as above
			Place_Special_Blast((SuperWeaponType)SuperWeapon.ID(super), best->Center_Coord().As_Cell()); // RA2: as above
		}
	}
}




/// <summary>
/// Invalidates the base node that a building occupies.
/// This routine is called when a building leaves the map. Any other node laying claim to
/// the same spot is released so the computer may build there again, and base defense
/// nodes are retired outright in multiplayer games, since the AI picks those spots for
/// itself rather than following the pre-built base list.
/// </summary>
/// <param name="building">The building whose base node location is to be invalidated.</param>
void HouseClass::Invalidate_Base_Node_Position(BuildingClass * building)
{
	if (!Debug_Map) {
		Cell cell = building->PositionCoord.As_Cell();
		for (int i = 0; i < Base.Nodes.Count(); i++) {
			if (Base.Nodes[i].Type == building->Class->HeapID && Base.Nodes[i].CellID == cell) {
				Cell node_cell = Base.Nodes[i].CellID;
				for (int j = 0; j < Base.Nodes.Count(); j++) {
					if (i != j && Base.Nodes[j].CellID == node_cell) {
						Base.Nodes[j].CellID = CELL_NONE;
					}
				}
				if (building->Class->IsBaseDefense && Session.Type != GAME_NORMAL) {
					Base.Nodes[i].Type = STRUCT_NONE;
					Base.Nodes[i].CellID = CELL_NONE;
				}
				return;
			}
		}
	}
}


/// <summary>
/// Converts a (formerly human) house into a computer-controlled house. Disowns the
/// player's factories, applies an AI handicap, re-centers the pre-built base list on
/// the existing construction yard, and inserts base-defense, wall-tower and power
/// nodes so the AI can maintain and expand the captured base.
/// </summary>
/*
**	RA2: a computer house in a network game.
*/
inline bool Is_Network_Computer(HouseClass * house)
{
	return(!house->Is_Human_Player() && Session.Type != GAME_NORMAL);
}
void HouseClass::AI_Takeover(void) // RA2: the body follows the executable
{
	if (!Is_Human_Player()) { // RA2: as TibSun
		return; // RA2: as above
	}

	IsHuman = false; // RA2: as above
	IsPlayerControl = false; // RA2: as above
	IQ = Rule->MaxIQ; // RA2: as above
	IniName = TStringID<HOUSE_NAME_MAX>("Computer"); // RA2: the literal name, not a string table entry
	wcscpy(UIName, StringTable::LoadString("TXT_COMPUTER", NULL, "D:\\Ra2\\House.CPP", 12592)); // RA2: the display name comes from the string table

	int index; // RA2: as above
	for (index = Factories.Count() - 1; index >= 0; index--) { // RA2: as above
		FactoryClass * factory = Factories[index]; // RA2: as above
		if (factory->House == this) { // RA2: as above
			Factories[index]->Suspend(true); // RA2: as above
			Factories[index]->Abandon(); // RA2: as above
			delete Factories[index]; // RA2: as above
		}
	}

	Assign_Handicap((DiffType)(2 - Difficulty)); // RA2: as above

	BuildingClass * conyard = NULL; // RA2: as above
	for (index = Buildings.Count() - 1; index >= 0; index--) { // RA2: as above
		BuildingClass * building = Buildings[index]; // RA2: any building of a type in the construction yard list counts
		bool owned_type = false; // RA2: as above
		for (int candidate = 0; candidate < Rule->BuildConst.Count(); candidate++) { // RA2: as above
			if (Rule->BuildConst[candidate] == building->Class) { // RA2: as above
				owned_type = true; // RA2: as above
				break; // RA2: as above
			}
		}
		if (!owned_type) continue; // RA2: as above
		if (!building->IsInLimbo) { // RA2: no owner test, the list is the house's own
			conyard = building; // RA2: as above
			break; // RA2: as above
		}
	}
	if (conyard == NULL) { // RA2: as above
		return; // RA2: as above
	}

	Cell center = conyard->Get_Coord().As_Cell(); // RA2: as above
	Center = Coord(center, 0); // RA2: as above
	if (Is_Network_Computer(this)) { // RA2: a computer house in a network game turns paranoid
		Computer_Paranoid(); // RA2: as above
	}
	if (Base.Nodes.Count() == 0) { // RA2: as above
		int save = ScenarioInit; // RA2: as above
		ScenarioInit = 0; // RA2: as above
		Make_Base_Nodes(); // RA2: as above
		ScenarioInit = save; // RA2: as above
	}
	Base.Nodes[0].CellID = center; // RA2: as above
	Base.PlacementCenter = center; // RA2: as above
	IsStarted = true; // RA2: as above
	IsAITriggersOn = true; // RA2: as above
	IsBaseBuilding = true; // RA2: as above

	for (int node = Base.Nodes.Count() - 1; node >= 0; node--) { // RA2: as above
		int type = Base.Nodes[node].Type; // RA2: as above
		if (type >= STRUCT_FIRST && ABQuantity.Value(type) > 0) { // RA2: as above
			BuildingTypeClass * btype = BuildingTypes[type]; // RA2: as above
			for (int owned = Buildings.Count() - 1; owned >= 0; owned--) { // RA2: as above
				BuildingClass * building = Buildings[owned]; // RA2: as above
				if (!building->IsInLimbo // RA2: no owner test, the list is the house's own
					&& (building->Class == btype || building->Upgrades[0] == btype // RA2: as above
						|| building->Upgrades[1] == btype || building->Upgrades[2] == btype)) { // RA2: as above
					Base.Nodes[node].CellID = building->Get_Coord().As_Cell(); // RA2: as above
				}
			}
		}
	}

	Map.Reset_Iterator(); // RA2: as above
	CellClass * cell = Map.Iterate(); // RA2: as above
	while (cell != NULL) { // RA2: as above
		if (cell->Owner == HeapID && cell->Overlay != OVERLAY_NONE && OverlayTypes[cell->Overlay]->IsWall) { // RA2: walls are sold without the wall tower pass
			Sell_Wall(cell->CellID, true); // RA2: as above
		}
		cell = Map.Iterate(); // RA2: as above
	}

	int building_count = Buildings.Count(); // RA2: the count is read once
	int defense_node = 0; // RA2: as above
	for (index = 0; index < building_count; index++) { // RA2: as above
		BuildingClass * building = Buildings[index]; // RA2: as above
		if (building->IsInLimbo) { // RA2: as above
			continue; // RA2: as above
		}
		BuildingTypeClass * btype = building->Class; // RA2: as above
		if (!btype->IsBaseDefense && !(btype == Rule->WallTower && building->UpgradeLevel > 0)) { // RA2: as above
			continue; // RA2: as above
		}
		if (!btype->Who_Can_Build_Me(true, false, true, this)) { // RA2: as above
			continue; // RA2: as above
		}
		while (defense_node < Base.Nodes.Count()) { // RA2: as above
			if (Base.Nodes[defense_node].Type == STRUCT_NONE) { // RA2: as above
				if (building->Class == Rule->WallTower) { // RA2: as above
					Base.Nodes.Insert_After(defense_node, BaseNodeClass(((BuildingTypeClass *)building->Upgrades[building->UpgradeLevel - 1])->HeapID, building->Get_Coord().As_Cell())); // RA2: as above
					Base.Nodes[defense_node].Type = building->Class->HeapID; // RA2: as above
					Base.Nodes[defense_node].CellID = building->Get_Coord().As_Cell(); // RA2: as above
					defense_node++; // RA2: as above
				} else {
					Base.Nodes[defense_node].Type = building->Class->HeapID; // RA2: as above
					Base.Nodes[defense_node].CellID = building->Get_Coord().As_Cell(); // RA2: as above
				}
				defense_node++; // RA2: as above
				break; // RA2: as above
			}
			defense_node++; // RA2: as above
		}
	}

	int power_node = 0; // RA2: as above
	int power = 0; // RA2: as above
	int drain = 0; // RA2: as above
	building_count = Buildings.Count(); // RA2: as above
	for (index = 0; index < building_count; index++) { // RA2: as above
		BuildingClass * building = Buildings[index]; // RA2: as above
		if (building->IsInLimbo) { // RA2: as above
			continue; // RA2: as above
		}
		BuildingTypeClass * btype = building->Class; // RA2: as above
		if (btype != Rule->GDIPowerPlant && btype != Rule->NodRegularPower && btype != Rule->NodAdvancedPower) { // RA2: as above
			continue; // RA2: as above
		}
		if (building->Get_Coord().As_Cell() == Base.Nodes[1].CellID) { // RA2: as above
			continue; // RA2: as above
		}
		if (!btype->Who_Can_Build_Me(true, false, true, this)) { // RA2: as above
			continue; // RA2: as above
		}
		while (power_node < Base.Nodes.Count()) { // RA2: as above
			if (Base.Nodes[power_node].Type >= STRUCT_FIRST) { // RA2: as above
				BuildingTypeClass * planned_type = BuildingTypes[Base.Nodes[power_node].Type]; // RA2: as above
				power += planned_type->Power; // RA2: as above
				drain += planned_type->Drain; // RA2: as above
				if (power_node > 0 && power < drain + PowerSurplus) { // RA2: as above
					Base.Nodes.Insert_After(power_node - 1, BaseNodeClass(building->Class->HeapID, building->Get_Coord().As_Cell())); // RA2: as above
					power += building->Class->Power; // RA2: as above
					drain -= planned_type->Drain; // RA2: as above
					power_node++; // RA2: as above
					break; // RA2: as above
				}
			}
			power_node++; // RA2: as above
		}
	}

	if (stricmp(Class->IniName, "GDI") == 0) { // RA2: as above
		power_node = 0; // RA2: as above
		power = 0; // RA2: as above
		drain = 0; // RA2: as above
		for (index = 0; index < ::Buildings.Count(); index++) { // RA2: every building in the game, with an owner test
			BuildingClass * building = ::Buildings[index];
			if (building->House != this || building->IsInLimbo || building->Class != Rule->GDIPowerPlant || building->UpgradeLevel <= 0) {
				continue; // RA2: as above
			}
			for (int upgrade = 0; upgrade < building->UpgradeLevel; upgrade++) { // RA2: as above
				while (power_node < Base.Nodes.Count()) { // RA2: as above
					if (Base.Nodes[power_node].Type >= STRUCT_FIRST) { // RA2: as above
						BuildingTypeClass * planned_type = BuildingTypes[Base.Nodes[power_node].Type]; // RA2: as above
						power += planned_type->Power; // RA2: as above
						drain += planned_type->Drain; // RA2: as above
						if (power_node > 0 && power < drain + PowerSurplus) { // RA2: as above
							Base.Nodes.Insert_After(power_node - 1, BaseNodeClass(Rule->GDIPowerTurbine->HeapID, building->Get_Coord().As_Cell())); // RA2: as above
							power += Rule->GDIPowerTurbine->Power; // RA2: as above
							drain -= planned_type->Drain; // RA2: as above
							power_node++; // RA2: as above
							break; // RA2: as above
						}
					}
					power_node++; // RA2: as above
				}
			}
		}
	}
}



/// <summary>
/// Updates the availability of this house's super weapons.
/// A super weapon that depends on a building is suspended while that building is shut
/// down or the base is short of power, and taken away entirely once the building is gone
/// or the house has been defeated. The sidebar is flagged for redraw and any targeting
/// mode cancelled when the local player is the one affected.
/// </summary>
void HouseClass::Update_Present_Super_Weapons(void)
{
	if (!GameActive) return;
	bool is_player = PlayerPtr == this; // RA2: whether this is the player's house is decided once, by house pointer
	for (int s = 0; s < SuperWeapon.Count(); s++) {
		if (SuperWeapon[s]->Is_Present() && ((SuperWeapon[s]->NeedsBuilding && !SuperWeapon[s]->Is_One_Time()) || IsDefeated)) {
			bool present = false;
			bool enabled = false;
			bool alive = !IsDefeated;
			if (alive) {
				for (int j = 0; j < ::Buildings.Count(); j++) { // RA2: every building in the game is looked at, the house test is on the building
					BuildingClass * b = ::Buildings[j];
					if (!b->IsInLimbo && b->IsActive && b->House->Fetch_ID() == Fetch_ID()) {
						for (int k = 0; k < 3; k++) {
							BuildingTypeClass const * upgrade = b->Upgrades[k];
							if (upgrade != NULL && (upgrade->SuperWeapon == s || upgrade->SuperWeapon2 == s)) {
								present = true;
								if (!enabled) enabled = b->IsOn;
							}
						}
						if (b->Fetch_Super_Weapon() == s || b->Fetch_Super_Weapon2() == s) {
							present = true;
							if (enabled) break;
							enabled = b->IsOn;
						}
						if (enabled && present) break;
					}
				}
			}

			int power = Power;
			int drain = Drain;
			if (power < drain && drain != 0) {
				if (power == 0 || (double)power / (double)drain < 1.0) {
					enabled = false;
				}
			}

			int heap_id = SuperWeapon[s]->Class->Fetch_Heap_ID();
			int tab = SidebarClass::Which_Tab(RTTI_SUPERWEAPON, heap_id, false); // RA2: the sidebar tab of the weapon is redrawn

			if (present && !IsDefeated) {
				bool const & on = enabled;
				bool off = !on;
				if ((off && SuperWeapon[s]->Is_Powered() && SuperWeapon[s]->Suspend(true)) || (enabled && SuperWeapon[s]->Suspend(false))) {
					if (is_player) {
						if (Map.IsTargettingMode == s) {
							Map.IsTargettingMode = SUPER_NONE;
						}
						Map.Flag_Tab_To_Redraw(tab);
					}
					IsRecalcNeeded = true;
				}
			} else {
				if (SuperWeapon[s]->Remove() && PlayerPtr != NULL) {
					if (is_player) {
						if (Map.IsTargettingMode == s) {
							Map.IsTargettingMode = SUPER_NONE;
						}
						Map.Flag_Tab_To_Redraw(tab);
					}
					IsRecalcNeeded = true;
				}
			}
		}
	}
}




/// <summary>
/// Enables every super weapon this house has the buildings for.
/// This routine looks over the house's structures, upgrades included, and switches on any
/// super weapon they grant that is not already available. The local player also gains a
/// sidebar button for each one turned on.
/// </summary>
void HouseClass::Enable_Available_Super_Weapons(void)
{
	if (IsDefeated) {
		return;
	}
	for (int super_index = 0; super_index < SuperWeapon.Count(); super_index++) { // RA2: RA2's form
		if (!SuperWeapon[super_index]->Is_Present() || SuperWeapon[super_index]->Is_One_Time()) { // RA2: as above
			bool from_upgrade = false;
			int building_index = Buildings.Count() - 1; // RA2: as above
			if (building_index >= 0) while (true) { // RA2: as above
				BuildingClass * building = Buildings[building_index]; // RA2: as above
				if (building->IsActive && !building->IsInLimbo) { // RA2: as above
					for (int upgrade_index = 0; upgrade_index < 3; upgrade_index++) { // RA2: as above
						BuildingTypeClass const * upgrade = building->Upgrades[upgrade_index]; // RA2: as above
						if (upgrade != NULL && (upgrade->SuperWeapon == super_index || upgrade->SuperWeapon2 == super_index)) { // RA2: as above
							from_upgrade = true;
							break;
						}
					}
					if (building->Fetch_Super_Weapon() == super_index || building->Fetch_Super_Weapon2() == super_index || from_upgrade) { // RA2: as above
						SuperWeapon[super_index]->Enable(false, this == PlayerPtr, Power_Fraction() < 1.0); // RA2: as above
						if (this == PlayerPtr) {
							Map.Add(RTTI_SPECIAL, super_index); // RA2: as above
							int type_index = SuperWeapon[super_index]->Class->Fetch_Heap_ID(); // RA2: as above
							Map.Flag_Tab_To_Redraw(SidebarClass::Which_Tab(RTTI_SUPERWEAPON, type_index, false)); // RA2: as above
						}
						break;
					}
				}
				building_index--; // RA2: as above
				if (building_index < 0) { // RA2: as above
					break;
				}
			}
		}
	}
}




/// <summary>
/// Has this house used up the build limit for an object type?
/// This routine is consulted by the production and sidebar logic so that a limited type
/// stops being offered once the house owns, or has queued, its allotment. A negative
/// limit tallies every one ever produced; a positive limit tallies only the survivors.
/// Vehicle thieves also count the vehicles they are currently sitting in.
/// </summary>
/// <param name="type">The object type to check the build limit of.</param>
/// <returns>bool; Has the limit been used up? An invalid type is always exhausted.</returns>
bool HouseClass::Is_Build_Limited(TechnoTypeClass const * type) const
{
	if (!type) { // RA2: RA2's form
		return true; // RA2: as above
	}
	int queued = 0; // RA2: as above
	bool naval = type->Naval; // RA2: as above
	FactoryClass * factory = Fetch_Factory(type->Fetch_RTTI(), naval, 0); // RA2: as above
	if (factory) { // RA2: as above
		queued = factory->Total(type); // RA2: as above
	}
	switch (type->Fetch_RTTI()) { // RA2: as above
	case 40: // RA2: as above
		if (type->BuildLimit <= 0) { // RA2: as above
			if (queued + PUQuantity.Value(type->Fetch_Heap_ID()) >= abs(type->BuildLimit)) { // RA2: as above
				return true; // RA2: as above
			}
		}
		if (type->BuildLimit > 0) { // RA2: as above
			if (queued + UQuantity.Value(type->Fetch_Heap_ID()) >= type->BuildLimit) { // RA2: as above
				return true; // RA2: as above
			}
		}
		break; // RA2: as above
	case 16: { // RA2: as above
		if (type->BuildLimit <= 0) { // RA2: as above
			if (queued + PIQuantity.Value(type->Fetch_Heap_ID()) >= abs(type->BuildLimit)) { // RA2: as above
				return true; // RA2: as above
			}
		}
		int owned = IQuantity.Value(type->Fetch_Heap_ID()); // RA2: as above
		if (((InfantryTypeClass *)type)->IsVehicleThief) { // RA2: as above
			for (int i = 0; i < Units.Count(); ++i) { // RA2: as above
				UnitClass * unit = Units[i]; // RA2: as above
				if (unit->House == this && unit->EnteredByInfType == type->Fetch_Heap_ID()) { // RA2: as above
					++owned; // RA2: as above
				}
			}
		}
		if (type->BuildLimit > 0) { // RA2: as above
			if (queued + owned >= type->BuildLimit) { // RA2: as above
				return true; // RA2: as above
			}
		}
		break; // RA2: as above
	}
	case 7: // RA2: as above
		if (type->BuildLimit <= 0) { // RA2: as above
			if (queued + PBQuantity.Value(type->Fetch_Heap_ID()) >= abs(type->BuildLimit)) { // RA2: as above
				return true; // RA2: as above
			}
		}
		if (type->BuildLimit > 0) { // RA2: as above
			if (queued + BQuantity.Value(type->Fetch_Heap_ID()) >= type->BuildLimit) { // RA2: as above
				return true; // RA2: as above
			}
		}
		break; // RA2: as above
	case 3: // RA2: as above
		if (((AircraftTypeClass *)type)->IsAirportBound) { // RA2: as above
			int owned = 0; // RA2: as above
			int queued_aircraft = 0; // RA2: as above
			for (int i = 0; i < Rule->PadAircraft.Count(); ++i) { // RA2: as above
				owned += AAQuantity.Value(Rule->PadAircraft[i]->Fetch_Heap_ID()); // RA2: as above
				if (factory) { // RA2: as above
					queued_aircraft += factory->Total(Rule->PadAircraft[i]); // RA2: as above
				}
			}
			if (queued_aircraft + owned >= AirportDocks) { // RA2: as above
				return true; // RA2: as above
			}
			break;
		}
		if (type->BuildLimit <= 0) { // RA2: as above
			if (queued + PAQuantity.Value(type->Fetch_Heap_ID()) >= abs(type->BuildLimit)) { // RA2: as above
				return true; // RA2: as above
			}
		}
		if (type->BuildLimit > 0) { // RA2: as above
			if (queued + AQuantity.Value(type->Fetch_Heap_ID()) >= type->BuildLimit) { // RA2: as above
				return true; // RA2: as above
			}
		}
		break; // RA2: as above
	}
	return false; // RA2: as above
}



/// <summary>
/// Is this house driven from the local machine?
/// A campaign mission may hand the player more than one side, so any house flagged as
/// human or as player controlled counts. Network games recognize only the local player's
/// own house.
/// </summary>
/// <returns>bool; Is this house under local player control?</returns>
bool HouseClass::Is_Player_Control(void) const
{
	if (Session.Type == GAME_NORMAL) {
		return(IsHuman || IsPlayerControl);
	}
	return(this == PlayerPtr) == true;
}


/// <summary>
/// Is this house run by a human?
/// In campaign games the question collapses to whether the house answers to the local
/// player, since the local player is the only human in the game.
/// </summary>
/// <returns>bool; Is this house controlled by a human player?</returns>
bool HouseClass::Is_Human_Player(void) const
{
	if (Session.Type == GAME_NORMAL) {
		return(Is_Player_Control());
	}
	return(IsHuman);
}


/// <summary>
/// Can this house place a building at the specified location?
/// This routine keeps a computer base compact by requiring a candidate site to touch
/// something the house already occupies. Campaign games impose no such restriction.
/// </summary>
/// <param name="cell">The upper left cell of the proposed building location.</param>
/// <returns>bool; Is the location acceptable to build at?</returns>
bool HouseClass::Can_Build_Here(BuildingTypeClass *building, Cell const & cell)
{
	if (Session.Type == GAME_NORMAL) {
		return(true);
	}

	int spacing = Rule->AIBaseSpacing;
	int width = (2 * spacing) + building->Width();
	int height = (2 * spacing) + building->Height();

	int mask = 1 << HeapID;

	for (int x = cell.X - spacing - 1; x < cell.X + width + 1; x++) {
		for (int y = cell.Y - spacing - 1; y < cell.Y + height + 1; y++) {
			if (Map[Cell(x, y)].OccupiedBy & mask) {
				return(true);
			}
		}
	}

	return(false);
}


/// <summary>
/// Sets up the radar remap color for this house.
/// This routine derives the house's remap color from its assigned color scheme, in the
/// pixel format the display is currently running in.
/// </summary>
/// <remarks>Call this routine again if the house color scheme or the display format
/// changes.</remarks>
void HouseClass::Initialize_Radar_Color(void)
{
	ColorScheme * scheme = ColorSchemes[Scheme];
	unsigned short c = scheme->Converter->Convert_Pixel(scheme->Bright);
	int r = (c >> DSurface::RedRight) << DSurface::RedLeft;
	int g = (c >> DSurface::GreenRight) << DSurface::GreenLeft;
	int b = (c >> DSurface::BlueRight) << DSurface::BlueLeft;
	RemapColorRGB = RGBClass(r,g,b);
}


/// <summary>
/// Handles the computer's use of the drop pod super weapon.
/// This routine picks a landing site out along a randomly chosen edge of the map and
/// fires the super weapon at it, provided the house has an enemy worth harassing.
/// </summary>
void __fastcall Normalize_Color(double & red, double & green, double & blue) // RA2: RA2's form
{
	double length = fastmath::sqrt(red * red + green * green + blue * blue); // RA2: as above
	if (length == 0.0) { // RA2: as above
		blue = 255.0; // RA2: as above
		green = 255.0; // RA2: as above
		red = 255.0; // RA2: as above
	} else {
		red = red * 240.0 / length; // RA2: as above
		if (red > 255.0) { // RA2: as above
			red = 255.0; // RA2: as above
		}
		green = green * 240.0 / length; // RA2: as above
		if (green > 255.0) { // RA2: as above
			green = 255.0; // RA2: as above
		}
		blue = blue * 240.0 / length; // RA2: as above
		if (blue > 255.0) { // RA2: as above
			blue = 255.0; // RA2: as above
		}
	}
}



void HouseClass::Update_Laser_Color(void) // RA2: RA2's form
{
	double red = RemapColorRGB.Get_Red(); // RA2: as above
	double green = RemapColorRGB.Get_Green(); // RA2: as above
	double blue = RemapColorRGB.Get_Blue(); // RA2: as above
	Normalize_Color(red, green, blue); // RA2: as above
	if (red < 96.0) { // RA2: as above
		red = 0; // RA2: as above
	}
	if (green < 96.0) { // RA2: as above
		green = 0; // RA2: as above
	}
	if (blue < 96.0) { // RA2: as above
		blue = 0; // RA2: as above
	}
	Normalize_Color(red, green, blue); // RA2: as above
	LaserColor.Set(red, green, blue); // RA2: as above
}



void HouseClass::Begin_Power_Blackout(int frames) // RA2: RA2's form
{
	PowerBlackoutTimer = frames; // RA2: as above
	RecalcPower = true;
}



void HouseClass::Begin_Radar_Blackout(int frames) // RA2: RA2's form
{
	RadarBlackoutTimer = frames; // RA2: as above
	RecalcRadar = true;
}



void HouseClass::Spy_Satellite_Lost(void) // RA2: RA2's form
{
	if (this == PlayerPtr && !SpySatActive) { // RA2: as above
		Map.Spy_Satellite_Hide(); // RA2: as above
	}
}



float HouseClass::Armor_Multiplier(TechnoTypeClass const * type) const // RA2: RA2's form
{
	switch (type->Fetch_RTTI()) { // RA2: as above
		case RTTI_INFANTRYTYPE:
			return(Class->ArmorInfantryMult); // RA2: as above
		case RTTI_UNITTYPE:
			return(Class->ArmorUnitsMult); // RA2: as above
		case RTTI_AIRCRAFTTYPE:
			return(Class->ArmorAircraftMult); // RA2: as above
		case RTTI_BUILDINGTYPE:
			if (((BuildingTypeClass const *)type)->BuildCat == 5) { // RA2: as above
				return(Class->ArmorDefensesMult); // RA2: as above
			}
			return(Class->ArmorBuildingsMult); // RA2: as above
	}
	return(1.0f); // RA2: as above
}



float HouseClass::Cost_Multiplier(TechnoTypeClass const * type) const // RA2: RA2's form
{
	switch (type->Fetch_RTTI()) { // RA2: as above
		case RTTI_INFANTRYTYPE:
			return(Class->CostInfantryMult); // RA2: as above
		case RTTI_UNITTYPE:
			return(Class->CostUnitsMult); // RA2: as above
		case RTTI_AIRCRAFTTYPE:
			return(Class->CostAircraftMult); // RA2: as above
		case RTTI_BUILDINGTYPE:
			if (((BuildingTypeClass const *)type)->BuildCat == 5) { // RA2: as above
				return(Class->CostDefensesMult); // RA2: as above
			}
			return(Class->CostBuildingsMult); // RA2: as above
	}
	return(1.0f); // RA2: as above
}



float HouseClass::Speed_Multiplier(TechnoTypeClass const * type) const // RA2: RA2's form
{
	switch (type->Fetch_RTTI()) { // RA2: as above
		case RTTI_INFANTRYTYPE:
			return(Class->SpeedInfantryMult); // RA2: as above
		case RTTI_UNITTYPE:
			return(Class->SpeedUnitsMult); // RA2: as above
		case RTTI_AIRCRAFTTYPE:
			return(Class->SpeedAircraftMult); // RA2: as above
	}
	return(1.0f); // RA2: as above
}



float HouseClass::Buildtime_Multiplier(TechnoTypeClass const * type) const // RA2: RA2's form
{
	switch (type->Fetch_RTTI()) { // RA2: as above
		case RTTI_INFANTRYTYPE:
			return(Class->BuildtimeInfantryMult); // RA2: as above
		case RTTI_UNITTYPE:
			return(Class->BuildtimeUnitsMult); // RA2: as above
		case RTTI_AIRCRAFTTYPE:
			return(Class->BuildtimeAircraftMult); // RA2: as above
		case RTTI_BUILDINGTYPE:
			if (((BuildingTypeClass const *)type)->BuildCat == 5) { // RA2: as above
				return(Class->BuildtimeDefensesMult); // RA2: as above
			}
			return(Class->BuildtimeBuildingsMult); // RA2: as above
	}
	return(1.0f); // RA2: as above
}



float HouseClass::Income_Multiplier(void) const // RA2: RA2's form
{
	return(Class->IncomeMult); // RA2: as above
}



int __fastcall House_Index_From_Name(char const * name) // RA2: RA2's form
{
	for (int index = 0; index < Houses.Count(); index++) {
		char buffer[20]; // RA2: as above
		strcpy(buffer, Houses[index]->IniName); // RA2: as above
		if (strcmp(buffer, name) == 0) { // RA2: as above
			return(index); // RA2: as above
		}
	}
	return(-1);
}



/// <summary>
/// Starts the computer building a base.
/// This routine lays out a base plan for the house, unless the scenario has already supplied
/// one of its own.
/// </summary>
void HouseClass::Begin_Construction(void)
{
	BuildingClass * yard = NULL; // RA2: RA2's form
	for (int index = Buildings.Count() - 1; index >= 0; index--) {
		BuildingClass * building = Buildings[index];
		bool is_yard = false; // RA2: as above
		for (int type_index = 0; type_index < Rule->BuildConst.Count(); type_index++) { // RA2: as above
			if (Rule->BuildConst[type_index] == building->Class) { // RA2: as above
				is_yard = true; // RA2: as above
				break;
			}
		}
		if (is_yard && !building->IsInLimbo) { // RA2: as above
			yard = building; // RA2: as above
			break;
		}
	}
	if (yard == NULL) { // RA2: as above
		return;
	}
	Coord position = yard->Get_Coord(); // RA2: as above
	Cell center = position.As_Cell(); // RA2: as above
	Center = center.As_Coord(); // RA2: as above
	Ensure_Base_Nodes(); // RA2: as above
	Base.Nodes[0].CellID = center;
	Base.PlacementCenter = center;
}




bool HouseClass::Add_Wall_Nodes(int node) // RA2: RA2's form
{
	int wall = -1; // RA2: as above
	for (int wi = 0; wi < Rule->ConcreteWalls.Count(); wi++) { // RA2: as above
		if (Rule->ConcreteWalls[wi]->AIBasePlanningSide == Class->Side || Rule->ConcreteWalls[wi]->AIBasePlanningSide == -1) { // RA2: as above
			wall = Rule->ConcreteWalls[wi]->HeapID; // RA2: as above
			break;
		}
	}

	BuildingClass * building = NULL; // RA2: as above
	int index;
	for (index = node - 1; index >= 0; index--) { // RA2: as above
		building = Base.Get_Building(index); // RA2: as above
		if (building != NULL && building->Class->ProtectWithWall && Base.Nodes[index + 1].Type != wall) { // RA2: as above
			break;
		}
	}

	if (index == -1 || building == NULL || !building->Class->ProtectWithWall) { // RA2: as above
		return(false);
	}

	Coord coord = building->PositionCoord; // RA2: as above
	Cell cell(coord.X / 256, coord.Y / 256); // RA2: as above
	BuildingTypeClass const * type = building->Class; // RA2: as above
	int width = type->Width(); // RA2: as above
	int height = type->Height(false); // RA2: as above
	Cell corner = cell - Cell(1, 1); // RA2: as above

	for (node = 1; node <= width; node++) { // RA2: as above
		Base.Nodes.Insert_After(BaseNodeClass((StructType)wall, corner + Cell(node, 0)), index); // RA2: the object-first order
		Base.Nodes.Insert_After(BaseNodeClass((StructType)wall, corner + Cell(node, height + 1)), index); // RA2: the object-first order
	}
	for (node = 1; node <= height; node++) { // RA2: as above
		Base.Nodes.Insert_After(BaseNodeClass((StructType)wall, corner + Cell(0, node)), index); // RA2: the object-first order
		Base.Nodes.Insert_After(BaseNodeClass((StructType)wall, corner + Cell(width + 1, node)), index); // RA2: the object-first order
	}

	Base.Nodes.Insert_After(BaseNodeClass((StructType)wall, corner + Cell(0, 0)), index); // RA2: the object-first order
	Base.Nodes.Insert_After(BaseNodeClass((StructType)wall, corner + Cell(width + 1, 0)), index); // RA2: the object-first order
	Base.Nodes.Insert_After(BaseNodeClass((StructType)wall, corner + Cell(0, height + 1)), index); // RA2: the object-first order
	Base.Nodes.Insert_After(BaseNodeClass((StructType)wall, corner + Cell(width + 1, height + 1)), index); // RA2: the object-first order
	return(true);
}



void HouseClass::Cheer(void) // RA2: RA2's form
{
	int count = Technos.Count(); // RA2: as above
	for (int index = 0; index < count; index++) {
		TechnoClass * techno = Technos[index];
		if (techno != NULL && techno->House == this) { // RA2: as above
			techno->Cheer(); // RA2: as above
		}
	}
	VocClass::PlayGlobal(Rule->CheerSound, 1.0f, 0x2000, 0); // RA2: as above
}



void HouseClass::Recall_Units_To_Base(void) // RA2: RA2's form
{
	int count = Technos.Count(); // RA2: as above
	Cell destination = Center.As_Cell(); // RA2: as above
	destination = Map.Nearby_Location(destination, SPEED_FOOT, -1, MZONE_NORMAL, false, Point2D(4, 4), false, false, false, true, Cell(0, 0), false); // RA2: as above
	if (destination != CELL_NONE) { // RA2: as above
		destination.X++; // RA2: as above
		destination.Y++; // RA2: as above
		for (int index = 0; index < count; index++) {
			TechnoClass * techno = Technos[index];
			if (techno != NULL && techno->House == this && techno->Fetch_RTTI() != RTTI_BUILDING && techno->Fetch_RTTI() != RTTI_AIRCRAFT // RA2: as above
				&& (techno->Techno_Type_Class()->DeploysInto == NULL || !techno->Techno_Type_Class()->DeploysInto->IsConstructionYard) // RA2: as above
				&& Center.As_Cell() != CELL_NONE) { // RA2: as above
				techno->Assign_Destination(&Map[destination], true); // RA2: as above
				techno->Assign_Archive_Target(NULL); // RA2: as above
				techno->QueueMission(MISSION_MOVE, false); // RA2: as above
			}
		}
	}
}



bool HouseClass::Can_Rebuild_Node(BaseNodeClass const & node) // RA2: RA2's form
{
	int type_index = node.Type; // RA2: as above
	if (Session.Type == GAME_NORMAL) {
		return(true);
	}
	if (type_index < 0) { // RA2: as above
		return(true);
	}
	if (type_index >= BuildingTypes.Count()) { // RA2: as above
		return(false);
	}
	BuildingTypeClass * type = BuildingTypes[type_index]; // RA2: as above
	if (type == NULL) {
		return(false);
	}
	if (type->Get_Weapon(0)->Weapon) { // RA2: as above
		return(true);
	}
	if (type->IsWall) { // RA2: as above
		for (int facing = 0; facing < 8; facing++) { // RA2: as above
			CellClass & cell = Map[node.CellID]; // RA2: as above
			TechnoClass * building = (TechnoClass *)cell.Adjacent_Cell((FacingType)facing).Cell_Building(); // RA2: as above
			if (building != NULL && building->House == this) { // RA2: as above
				return(true);
			}
		}
		return(false);
	}
	if (type->Power > 0) { // RA2: as above
		return(true);
	}
	if (LATime + Rule->AIRestrictReplaceTime <= Frame) { // RA2: as above
		return(true);
	}
	return(false);
}



Cell HouseClass::Best_Super_Weapon_Target(void) // RA2: RA2's form
{
	HouseClass * enemy = Houses[Enemy];
	DynamicVectorClass<AbstractClass *> targets;
	int best = 0;
	for (int index = 0; index < Technos.Count(); index++) {
		int value = 0;
		bool valid = false;
		TechnoClass * techno = Technos[index];
		if (techno->House == enemy) {
			value = 1;
			if (techno->In_Which_Layer() == LAYER_GROUND && techno->IsActive && !techno->IsInLimbo) {
				valid = true;
			} else if (Difficulty == 0) { // RA2: as above
				for (int fac = 0; fac < Factories.Count(); fac++) { // RA2: as above
					FactoryClass * factory = Factories[fac]; // RA2: as above
					if (factory->Get_Object() == techno && factory->Fetch_Rate() && !factory->IsSuspended) { // RA2: as above
						valid = true;
					}
				}
			}
			switch (techno->Fetch_RTTI()) {
				case RTTI_INFANTRY: { // RA2: as above
					InfantryTypeClass const * type = ((InfantryClass *)techno)->Class; // RA2: as above
					if (type->IsEngineer) { // RA2: as above
						value = Rule->AIIonCannonEngineerValue[Difficulty];
					} else if (type->IsVehicleThief) { // RA2: as above
						value = Rule->AIIonCannonThiefValue[Difficulty];
					} else {
						value = 2;
					}
					break;
				}
				case RTTI_BUILDING: { // RA2: as above
					BuildingTypeClass const * type = ((BuildingClass *)techno)->Class; // RA2: as above
					if (type->ToBuild == RTTI_BUILDINGTYPE) { // RA2: as above
						value = Rule->AIIonCannonConYardValue[Difficulty];
					} else if (type->ToBuild == RTTI_UNITTYPE) { // RA2: as above
						value = Rule->AIIonCannonWarFactoryValue[Difficulty];
					} else if (type->Power > type->Drain) { // RA2: as above
						value = Rule->AIIonCannonPowerValue[Difficulty];
					} else if (type->IsBaseDefense) { // RA2: as above
						value = Rule->AIIonCannonBaseDefenseValue[Difficulty];
					} else if (type->IsPlug) { // RA2: as above
						value = Rule->AIIonCannonPlugValue[Difficulty];
					} else if (type->IsTemple) { // RA2: as above
						value = Rule->AIIonCannonTempleValue[Difficulty];
					} else if (type->IsHoverPad) { // RA2: as above
						value = Rule->AIIonCannonHelipadValue[Difficulty];
					} else {
						value = 4;
					}
					break;
				}
				case RTTI_UNIT: { // RA2: as above
					UnitTypeClass const * type = ((UnitClass *)techno)->Class; // RA2: as above
					if (type->IsToHarvest) { // RA2: as above
						value = Rule->AIIonCannonHarvesterValue[Difficulty];
					} else {
						bool is_mcv = false; // RA2: as above
						for (int yard = 0; yard < Rule->BuildConst.Count(); yard++) { // RA2: as above
							if (Rule->BuildConst[yard] == type->DeploysInto) { // RA2: as above
								value = Rule->AIIonCannonMCVValue[Difficulty];
								is_mcv = true; // RA2: as above
								break;
							}
						}
						if (!is_mcv) { // RA2: as above
							if (type->MaxPassengers > 0) { // RA2: as above
								value = Rule->AIIonCannonAPCValue[Difficulty];
							} else {
								value = 2;
							}
						}
					}
					break;
				}
			}
		}
		if (techno->Cloak == CLOAKED || (techno->Fetch_RTTI() == RTTI_BUILDING && ((BuildingClass *)techno)->TranslucencyLevel == 15)) { // RA2: as above
			value = Random_Pick(0, best + 10);
		}
		if (valid) {
			if (value > best) {
				targets.Clear();
				targets.Add(techno);
				best = value;
			} else if (value == best) {
				targets.Add(techno);
			}
		}
	}
	if (targets.Count() > 0) {
		AbstractClass * target = targets[Random_Pick(0, targets.Count() - 1)];
		if (target != NULL) {
			return(target->Center_Coord().As_Cell()); // RA2: as above
		}
	}
	return(CELL_NONE); // RA2: as above
}



Cell HouseClass::Threat_Target_Cell(int quarry) // RA2: RA2's form
{
	ThreatType flags = Quarry_To_Threat(quarry); // RA2: as above
	TeamClass * team = NULL; // RA2: as above
	for (int index = 0; index < Teams.Count(); index++) { // RA2: as above
		team = Teams[index]; // RA2: as above
		if (team->House == this) { // RA2: as above
			break;
		}
	}
	if (team != NULL && team->House == this) { // RA2: as above
		FootClass * leader = team->Fetch_A_Leader(); // RA2: as above
		AbstractClass * target = leader->Greatest_Threat(flags, leader->Center_Coord(), true); // RA2: as above
		if (target != NULL) {
			return(target->Center_Coord().As_Cell()); // RA2: as above
		}
		return(CELL_NONE); // RA2: as above
	}
	return(CELL_NONE); // RA2: as above
}



/// <summary>
/// Adds a reference to this house.
/// Houses are permanent heap objects rather than reference counted ones, so this routine
/// exists only to satisfy the IUnknown contract.
/// </summary>
/// <returns>Returns with the reference count, which is always one.</returns>
ULONG STDMETHODCALLTYPE HouseClass::AddRef(void)
{
	return(1);
}


/// <summary>
/// Releases a reference to this house.
/// Houses are permanent heap objects rather than reference counted ones, so this routine
/// exists only to satisfy the IUnknown contract. It never destroys the house.
/// </summary>
/// <returns>Returns with the reference count, which is always one.</returns>
ULONG STDMETHODCALLTYPE HouseClass::Release(void)
{
	return(1);
}


/// <summary>
/// Fetches the RTTI type of this object.
/// </summary>
/// <returns>Returns with RTTI_HOUSE.</returns>
RTTIType HouseClass::Fetch_RTTI(void) const
{
	return(RTTI_HOUSE);
}


/// <summary>
/// Fetches the heap index of this house.
/// </summary>
/// <returns>Returns with the index of this house within the house heap.</returns>
int HouseClass::Fetch_Heap_ID(void) const
{
	return(HeapID);
}








































































/// Unlike every other interface identifier, this one is defined in the house module.
#define INITGUID
#undef DEFINE_GUID
#include <basetyps.h>
#include "ihouse_i.c"


// RA2: the weighted building records retain this vector constructor as an ordinary function.
template<>
VectorClass<DiscreteDistributionClass<BuildingTypeClass>::DIST_OBJ>::VectorClass(int size, DiscreteDistributionClass<BuildingTypeClass>::DIST_OBJ const * array) :
	Vector(0),
	VectorMax(size),
	IsValid(true),
	IsAllocated(false)
{
	/*
	**	Allocate the vector. The default constructor will be called for every
	**	object in this vector.
	*/
	if (size) {
		if (array) {
			Vector = new ((void *)array) DiscreteDistributionClass<BuildingTypeClass>::DIST_OBJ[size];
		} else {
			Vector = new DiscreteDistributionClass<BuildingTypeClass>::DIST_OBJ[size];
			IsAllocated = true;
		}
	}
}


/// <summary>
/// RA2: a base node for a building type at a cell, not yet placed and with no attempts made.
/// </summary>
BaseNodeClass::BaseNodeClass(StructType building, Cell const & cell)
{
	Type = building;
	CellID = cell;
	Placed = false;
	Attempts = 0;
}


// RA2: these record-vector members retain ordinary copies in this unit.
template<>
bool DynamicVectorClass<DiscreteDistributionClass<BuildingTypeClass>::DIST_OBJ>::Add(DiscreteDistributionClass<BuildingTypeClass>::DIST_OBJ const & object)
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

template<>
bool DynamicVectorClass<DiscreteDistributionClass<BuildingTypeClass>::DIST_OBJ>::Resize(int newsize, DiscreteDistributionClass<BuildingTypeClass>::DIST_OBJ const * array)
{
	if (BASECLASS::Resize(newsize, array)) {
		if (Length() < ActiveCount) ActiveCount = Length();
		return(true);
	}
	return(false);
}


template<>
int DynamicVectorClass<DiscreteDistributionClass<BuildingTypeClass>::DIST_OBJ>::ID(DiscreteDistributionClass<BuildingTypeClass>::DIST_OBJ const & object)
{
	for (int index = 0; index < Count(); index++) {
		if ((*this)[index] == object) return(index);
	}
	return(-1);
}
