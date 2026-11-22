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

/* $Header: /CounterStrike/BDATA.CPP 2     3/03/97 10:37p Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : BDATA.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : September 10, 1993                                           *
 *                                                                                             *
 *                  Last Update : October 2, 1996 [JLB]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   BuildingTypeClass::As_Reference -- Fetches reference to the building type specified.      *
 *   BuildingTypeClass::Bib_And_Offset -- Determines the bib and appropriate cell offset.      *
 *   BuildingTypeClass::BuildingTypeClass -- This is the constructor for the building types.   *
 *   BuildingTypeClass::Coord_Fixup -- Adjusts coordinate to be legal for assignment.          *
 *   BuildingTypeClass::Cost_Of -- Fetches the cost of this building.                          *
 *   BuildingTypeClass::Create_And_Place -- Creates and places a building object onto the map. *
 *   BuildingTypeClass::Create_One_Of -- Creates a building of this type.                      *
 *   BuildingTypeClass::Dimensions -- Fetches the pixel dimensions of the building.            *
 *   BuildingTypeClass::Display -- Renders a generic view of building.                         *
 *   BuildingTypeClass::Flush_For_Placement -- Tries to clear placement area for this building *
 *   BuildingTypeClass::Full_Name -- Fetches the name to give this building.                   *
 *   BuildingTypeClass::Height -- Determines the height of the building in icons.              *
 *   BuildingTypeClass::Init -- Performs theater specific initialization.                      *
 *   BuildingTypeClass::Init_Anim -- Initialize an animation control for a building.           *
 *   BuildingTypeClass::Init_Heap -- Initialize the heap as necessary for the building type obj*
 *   BuildingTypeClass::Max_Pips -- Determines the maximum pips to display.                    *
 *   BuildingTypeClass::Occupy_List -- Fetches the occupy list for the building.               *
 *   BuildingTypeClass::One_Time -- Performs special one time action for buildings.            *
 *   BuildingTypeClass::Overlap_List -- Fetches the overlap list for the building.             *
 *   BuildingTypeClass::Prep_For_Add -- Prepares scenario editor for adding an object.         *
 *   BuildingTypeClass::Raw_Cost -- Fetches the raw (base) cost of this building type.         *
 *   BuildingTypeClass::Read_INI -- Fetch building type data from the INI database.            *
 *   BuildingTypeClass::Width -- Determines width of building in icons.                        *
 *   BuildingTypeClass::operator delete -- Deletes a building type object from the special heap*
 *   BuildingTypeClass::operator new -- Allocates a building type object from the special heap.*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#define INCLUDE_COM
#include "always.h"
#include "geometry.h" // RA2: the header statics, in the order of the unit's initializer table

#include "builtype.h"

#include "_map.h"
#include "_mixfile.h"
#include "_rules.h"
#include "_theater.h"
#include "airctype.h"
#include "bound.h"
#include "bsurface.h"
#include "building.h"
#include "bullettype.h"
#include "ccfile.h"
#include "cell.h"
#include "data.h"
#include "findmake.h"
#include "foot.h"
#include "globals.h"
#include "house.h"
#include "incdec.h"
#include "isotype.h"
#include "mixfile.h"
#include "overtype.h"
#include "rules.h"
#include "scenario.h"
#include "shapeset.h"
#include "sun.h"
#include "swizzle.h"
#include "tracker.h"
#include "unittype.h"
#include "warhead.h"
#include "weapon.h"

void const * BuildingTypeClass::BuildingZShape;
void const * BuildingTypeClass::PowerOffShapes;
void const * BuildingTypeClass::WrenchShapes;

BSurface * CloakingSurface;

Cell const BuildingTypeClass::OccupyLists[BSIZE_COUNT][30] /* RA2: rows of 30 */ = {
	/* BSIZE_11,	*/ { Cell(0,0),REFRESH_EOL },
	/* BSIZE_21,	*/ { Cell(0,0),Cell(1,0),REFRESH_EOL },
	/* BSIZE_12,	*/ { Cell(0,0),Cell(0,1),REFRESH_EOL },
	/* BSIZE_22,	*/ { Cell(0,0),Cell(1,0),Cell(0,1),Cell(1,1),REFRESH_EOL },
	/* BSIZE_23,	*/ { Cell(0,0),Cell(1,0),Cell(0,1),Cell(1,1),Cell(0,2),Cell(1,2),REFRESH_EOL },
	/* BSIZE_32,	*/ { Cell(0,0),Cell(1,0),Cell(2,0),Cell(0,1),Cell(1,1),Cell(2,1),REFRESH_EOL },
	/* BSIZE_33,	*/ { Cell(0,0),Cell(1,0),Cell(2,0),Cell(0,1),Cell(1,1),Cell(2,1),Cell(0,2),Cell(1,2),Cell(2,2),REFRESH_EOL },
	/* BSIZE_35,	*/ { Cell(0,0),Cell(1,0),Cell(2,0),Cell(0,1),Cell(1,1),Cell(2,1),Cell(0,2),Cell(1,2),Cell(2,2),Cell(0,3),Cell(1,3),Cell(2,3),Cell(0,4),Cell(1,4),Cell(2,4),REFRESH_EOL },
	/* BSIZE_42,	*/ { Cell(0,0),Cell(1,0),Cell(2,0),Cell(3,0),Cell(0,1),Cell(1,1),Cell(2,1),Cell(3,1),REFRESH_EOL },
	/* BSIZE_33_REF	*/ { Cell(0,0),Cell(1,0),Cell(2,0),Cell(0,1),Cell(1,1),Cell(0,2),Cell(1,2),Cell(2,2),REFRESH_EOL },
	/* BSIZE_13,	*/ { Cell(0,0),Cell(0,1),Cell(0,2),REFRESH_EOL },
	/* BSIZE_31,	*/ { Cell(0,0),Cell(1,0),Cell(2,0),REFRESH_EOL },
	/* BSIZE_43,	*/ { Cell(0,0),Cell(1,0),Cell(2,0),Cell(3,0),Cell(0,1),Cell(1,1),Cell(2,1),Cell(3,1),Cell(0,2),Cell(1,2),Cell(2,2),Cell(3,2),REFRESH_EOL },
	/* BSIZE_14,	*/ { Cell(0,0),Cell(0,1),Cell(0,2),Cell(0,3),REFRESH_EOL },
	/* BSIZE_15,	*/ { Cell(0,0),Cell(0,1),Cell(0,2),Cell(0,3),Cell(0,4),REFRESH_EOL },
	/* BSIZE_26,	*/ { Cell(0,0),Cell(1,0),Cell(0,1),Cell(1,1),Cell(0,2),Cell(1,2),Cell(0,3),Cell(1,3),Cell(0,4),Cell(1,4),Cell(0,5),Cell(1,5),REFRESH_EOL },
	/* BSIZE_25,	*/ { Cell(0,0),Cell(1,0),Cell(0,1),Cell(1,1),Cell(0,2),Cell(1,2),Cell(0,3),Cell(1,3),Cell(0,4),Cell(1,4),REFRESH_EOL },
	/* BSIZE_53,	*/ { Cell(0,0),Cell(1,0),Cell(2,0),Cell(3,0),Cell(4,0),Cell(0,1),Cell(1,1),Cell(2,1),Cell(3,1),Cell(4,1),Cell(0,2),Cell(1,2),Cell(2,2),Cell(3,2),Cell(4,2),REFRESH_EOL },
	/* BSIZE_44,	*/ { Cell(0,0),Cell(1,0),Cell(2,0),Cell(3,0),Cell(0,1),Cell(1,1),Cell(2,1),Cell(3,1),Cell(0,2),Cell(1,2),Cell(2,2),Cell(3,2),Cell(0,3),Cell(1,3),Cell(2,3),Cell(3,3),REFRESH_EOL },
	/* BSIZE_34,	*/ { Cell(0,0),Cell(1,0),Cell(2,0),Cell(0,1),Cell(1,1),Cell(2,1),Cell(0,2),Cell(1,2),Cell(2,2),Cell(0,3),Cell(1,3),Cell(2,3),REFRESH_EOL },
	/* BSIZE_64,	*/ { Cell(0, 0), Cell(1, 0), Cell(2, 0), Cell(3, 0), Cell(4, 0), Cell(5, 0), Cell(0, 1), Cell(1, 1), Cell(2, 1), Cell(3, 1), Cell(4, 1), Cell(5, 1), Cell(0, 2), Cell(1, 2), Cell(2, 2), Cell(3, 2), Cell(4, 2), Cell(5, 2), Cell(0, 3), Cell(1, 3), Cell(2, 3), Cell(3, 3), Cell(4, 3), Cell(5, 3), REFRESH_EOL },
	/* BSIZE_00,	*/ { REFRESH_EOL }
};


Cell const BuildingTypeClass::ExitLists[BSIZE_COUNT][30] = {
	/* BSIZE_11,	*/	{ Cell(0,1),Cell(-1,1),Cell(1,1),Cell(-1,0),Cell(1,0),Cell(0,-1),Cell(-1,-1),Cell(1,-1),REFRESH_EOL },
	/* BSIZE_21,	*/	{ Cell(0,1),Cell(1,1),Cell(-1,1),Cell(2,1),Cell(-1,0),Cell(2,0),Cell(0,-1),Cell(1,-1),Cell(-1,-1),Cell(2,-1),REFRESH_EOL },
	/* BSIZE_12,	*/	{ Cell(0,2),Cell(1,2),Cell(-1,2),Cell(-1,1),Cell(1,1),Cell(-1,0),Cell(1,0),Cell(0,-1),Cell(-1,-1),Cell(1,-1),REFRESH_EOL },
	/* BSIZE_22,	*/	{ Cell(0,2),Cell(1,2),Cell(-1,2),Cell(2,2),Cell(-1,1),Cell(2,1),Cell(-1,0),Cell(2,0),Cell(0,-1),Cell(1,-1),Cell(-1,-1),Cell(2,-1),REFRESH_EOL },
	/* BSIZE_23,	*/	{ Cell(0,3),Cell(1,3),Cell(-1,3),Cell(2,3),Cell(-1,2),Cell(2,2),Cell(-1,1),Cell(2,1),Cell(-1,0),Cell(2,0),Cell(0,-1),Cell(1,-1),Cell(-1,-1),Cell(2,-1),REFRESH_EOL },
	/* BSIZE_32,	*/	{ Cell(0,2),Cell(1,2),Cell(2,2),Cell(-1,2),Cell(3,2),Cell(-1,1),Cell(3,1),Cell(-1,0),Cell(3,0),Cell(0,-1),Cell(1,-1),Cell(2,-1),Cell(-1,-1),Cell(3,-1),REFRESH_EOL },
	/* BSIZE_33,	*/	{ Cell(0,3),Cell(1,3),Cell(2,3),Cell(-1,3),Cell(3,3),Cell(-1,2),Cell(3,2),Cell(-1,1),Cell(3,1),Cell(-1,0),Cell(3,0),Cell(0,-1),Cell(1,-1),Cell(2,-1),Cell(-1,-1),Cell(3,-1),REFRESH_EOL },
	/* BSIZE_35,	*/	{ Cell(-1,-1),Cell(0,-1),Cell(1,-1),Cell(2,-1),Cell(3,-1),Cell(-1,0),Cell(3,0),Cell(-1,1),Cell(3,1),Cell(-1,2),Cell(3,2),Cell(-1,3),Cell(3,3),Cell(-1,4),Cell(3,4),Cell(-1,5),Cell(0,5),Cell(1,5),Cell(2,5),Cell(3,5),REFRESH_EOL },
	/* BSIZE_42,	*/	{ Cell(0,2),Cell(1,2),Cell(2,2),Cell(3,2),Cell(-1,2),Cell(4,2),Cell(-1,1),Cell(4,1),Cell(-1,0),Cell(4,0),Cell(0,-1),Cell(1,-1),Cell(2,-1),Cell(3,-1),Cell(-1,-1),Cell(4,-1),REFRESH_EOL },
	/* BSIZE_33_REF	*/	{ Cell(0,0),REFRESH_EOL },
	/* BSIZE_13,	*/	{ Cell(-1,-1),Cell(0,-1),Cell(1,-1),Cell(-1,0),Cell(1,0),Cell(-1,1),Cell(1,1),Cell(-1,2),Cell(1,2),Cell(-1,3),Cell(0,3),Cell(1,3),REFRESH_EOL },
	/* BSIZE_31,	*/	{ Cell(-1,-1),Cell(0,-1),Cell(1,-1),Cell(2,-1),Cell(3,-1),Cell(-1,0),Cell(3,0),Cell(-1,1),Cell(0,1),Cell(1,1),Cell(2,1),Cell(3,1),REFRESH_EOL },
	/* BSIZE_43,	*/	{ Cell(0,3),Cell(1,3),Cell(2,3),Cell(-1,3),Cell(3,3),Cell(-1,2),Cell(3,2),Cell(-1,1),Cell(3,1),Cell(-1,0),Cell(3,0),Cell(0,-1),Cell(1,-1),Cell(2,-1),Cell(-1,-1),Cell(3,-1),REFRESH_EOL },
	/* BSIZE_14,	*/	{ Cell(-1,-1),Cell(0,-1),Cell(1,-1),Cell(-1,0),Cell(1,0),Cell(-1,1),Cell(1,1),Cell(-1,2),Cell(1,2),Cell(-1,3),Cell(1,3),Cell(-1,4),Cell(0,4),Cell(1,4),REFRESH_EOL },
	/* BSIZE_15,	*/	{ Cell(-1,-1),Cell(0,-1),Cell(1,-1),Cell(-1,0),Cell(1,0),Cell(-1,1),Cell(1,1),Cell(-1,2),Cell(1,2),Cell(-1,3),Cell(1,3),Cell(-1,4),Cell(1,4),Cell(-1,5),Cell(0,5),Cell(1,5),REFRESH_EOL },
	/* BSIZE_26,	*/	{ Cell(-1,-1),Cell(0,-1),Cell(1,-1),Cell(2,-1),Cell(-1,0),Cell(2,0),Cell(-1,1),Cell(2,1),Cell(-1,2),Cell(2,2),Cell(-1,3),Cell(2,3),Cell(-1,5),Cell(2,5),Cell(-1,6),Cell(0,6),Cell(1,6),Cell(2,6),REFRESH_EOL },
	/* BSIZE_25,	*/	{ Cell(-1,-1),Cell(0,-1),Cell(1,-1),Cell(2,-1),Cell(-1,0),Cell(2,0),Cell(-1,1),Cell(2,1),Cell(-1,2),Cell(2,2),Cell(-1,3),Cell(2,3),Cell(-1,5),Cell(0,5),Cell(1,5),Cell(2,5),REFRESH_EOL },
	/* BSIZE_53,	*/	{ Cell(-1,-1),Cell(0,-1),Cell(1,-1),Cell(2,-1),Cell(3,-1),Cell(4,-1),Cell(5,-1),Cell(-1,0),Cell(5,0),Cell(-1,1),Cell(5,1),Cell(-1,2),Cell(5,2),Cell(-1,3),Cell(0,3),Cell(1,3),Cell(2,3),Cell(3,3),Cell(4,3),Cell(5,3),REFRESH_EOL },
	/* BSIZE_44,	*/	{ Cell(-1,-1),Cell(0,-1),Cell(1,-1),Cell(2,-1),Cell(3,-1),Cell(4,-1),Cell(-1,4),Cell(0,4),Cell(1,4),Cell(2,4),Cell(3,4),Cell(4,4),Cell(-1,0),Cell(-1,1),Cell(-1,2),Cell(-1,3),Cell(-1,4),Cell(4,0),Cell(4,1),Cell(4,2),Cell(4,3),Cell(4,4),REFRESH_EOL },
	/* BSIZE_34,	*/	{ Cell(0,4),Cell(1,4),Cell(2,4),Cell(-1,4),Cell(3,4),Cell(-1,2),Cell(3,2),Cell(-1,1),Cell(3,1),Cell(-1,0),Cell(3,0),Cell(0,-1),Cell(1,-1),Cell(2,-1),Cell(-1,-1),Cell(3,-1),REFRESH_EOL },
	/* BSIZE_64,	*/	{ Cell(2,-1),REFRESH_EOL },
	/* BSIZE_00,	*/	{ REFRESH_EOL }
};


int BuildingTypeClass::SizeWidth[BSIZE_COUNT] = {
	1,
	2,
	1,
	2,
	2,
	3,
	3,
	3,
	4,
	3,
	1,
	3,
	4,
	1,
	1,
	2,
	2,
	5,
	4,
	3,
	6,
	0
};


int BuildingTypeClass::SizeHeight[BSIZE_COUNT] = {
	1,
	1,
	2,
	2,
	3,
	2,
	3,
	5,
	2,
	3,
	3,
	1,
	3,
	4,
	5,
	6,
	5,
	3,
	4,
	4,
	4,
	0
};


// RA2: the build category names and values (table at 0x7CFC40, six rows; the exe's order is DontCare, Tech, Power, Resource, Infrastructure, Combat)
static struct { // RA2: as above
	char const * Name; // RA2: as above
	int Value; // RA2: as above
} _buildcats[6] = { // RA2: as above
	{ "DontCare", 0 }, // RA2: as above
	{ "Tech", 1 }, // RA2: as above
	{ "Power", 3 }, // RA2: as above
	{ "Resource", 2 }, // RA2: as above
	{ "Infrastructure", 4 }, // RA2: as above
	{ "Combat", 5 } // RA2: as above
};


// RA2: Name_From_BuildCat 0x457A50 (new in RA2): linear search of the six rows by value, row 0 if none is found
char const * __fastcall Name_From_BuildCat(int value) // RA2: as above
{
	for (int index = 0; index < 6; index++) { // RA2: as above
		if (_buildcats[index].Value == value) { // RA2: as above
			return(_buildcats[index].Name); // RA2: as above
		}
	}
	return(_buildcats[0].Name); // RA2: as above
}


// RA2: BuildCat_From_Name 0x457A80 (new in RA2): case-insensitive linear search of the six rows by name, 0 if none is found
int __fastcall BuildCat_From_Name(char const * name) // RA2: as above
{
	for (int index = 0; index < 6; index++) { // RA2: as above
		if (strcmpi(_buildcats[index].Name, name) == 0) { // RA2: as above
			return(_buildcats[index].Value); // RA2: as above
		}
	}
	return(0); // RA2: as above
}


/***********************************************************************************************
 * BuildingTypeClass::BuildingTypeClass -- This is the constructor for the building types.     *
 *                                                                                             *
 *    This is the constructor used to create the building types.                               *
 *                                                                                             *
 * INPUT:   see below...                                                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
BuildingTypeClass::BuildingTypeClass(char const * ininame) :
	BASECLASS(ininame, SPEED_FOOT),
	HeapID(STRUCT_NONE),
	OccupyList(NULL),
	BuildupData(NULL),
	BuildupLoaded(false), // RA2: +0xB9C, cleared by the initializer list
	BuildCat(0), // RA2: +0xBA0
	HalfDamageSmokeLocation1(COORD_NONE),
	HalfDamageSmokeLocation2(COORD_NONE),
	GateCloseDelay(0.0),
	LightVisibility(5000),
	LightIntensity(0),
	LightRedTint(1000000),
	LightGreenTint(1000000),
	LightBlueTint(1000000),
	PrimaryFirePixelOffset(Point2D(0xFFFF, 0xFFFF)),
	SecondaryFirePixelOffset(Point2D(0xFFFF, 0xFFFF)),
	ToOverlay(NULL),
	ToTile(NULL),
	BuildupFilename(NoInitClass()),
	BuildupSound(VOC_NONE), // RA2: 0xC04..0xC1C are all -1
	PackupSound(VOC_NONE), // RA2: as above
	CreateUnitSound(VOC_NONE), // RA2: as above
	UnitEnterSound(VOC_NONE), // RA2: as above
	UnitExitSound(VOC_NONE), // RA2: as above
	WorkingSound(VOC_NONE), // RA2: as above
	NotWorkingSound(VOC_NONE), // RA2: as above
	PowersUpBuilding(NoInitClass()),
	FreeUnit(NULL),
	FoundationFace(FACING_NONE),
	Adjacent(3),
	ToBuild(RTTI_NONE),
	TargetCoordOffset(0, 0, 0), // RA2: +0xC48
	ExitCoordinate(0, 0, 0),
	ExitList(NULL),
	StartFace(DIR_N),
	Power(0),
	Drain(0),
	ExtraPower(0), // RA2: +0xC70
	ExtraDrain(0), // RA2: +0xC74
	Size(BSIZE_11),
	ZHeight(2), // RA2: 2, not TibSun's 1
	OccupyHeight(2), // RA2: +0xC80
	MidPoint(0),
	DoorStages(0),
	Upgrades(0),
	DeployingAnim(NULL),
	UnderDoorAnim(NULL),
	RoofDeployingAnim(NULL), // RA2: +0x11B0
	UnderRoofDoorAnim(NULL), // RA2: +0x11B8
	DoorAnim(NULL),
	SpecialZOverlay(NULL),
	SpecialZOverlayZAdjust(0),
	BibShape(NULL),
	BibShapeLoaded(false), // RA2: +0x11D0
	NormalZAdjust(0),
	AntiAirValue(0),
	AntiArmorValue(0),
	AntiInfantryValue(0),
	ZShapePointMove(Point2D(0, 0)),
	DrawRect(RECT_NONE),
	ExtraLight(0),
	IsCanTogglePower(true),
	HasSpotlight(false),
	IsTemple(false),
	IsPlug(false),
	IsHoverPad(false),
	IsBase(true),
	EligibleForAllyBuilding(false), // RA2: +0x1204
	EligibleForDelayKill(false), // RA2: as above
	NeedsEngineer(false), // RA2: as above
	ProduceCashStartup(0), // RA2: +0x1208
	ProduceCashAmount(0), // RA2: as above
	ProduceCashDelay(0), // RA2: as above
	IsBibbed(false),
	IsWall(false),
	IsCaptureable(false),
	IsPowered(false),
	Overpowerable(false), // RA2: +0x1218
	Spyable(false), // RA2: as above
	CanC4(true), // RA2: as above
	WantsExtraSpace(false), // RA2: as above
	IsUnsellable(false),
	ClickRepairable(true), // RA2: +0x121D
	CanBeOccupied(false), // RA2: as above
	MaxNumberOccupants(0), // RA2: as above
	ShowOccupantPips(true), // RA2: as above
	QueueingCell(Point2D(0, 0)), // RA2: +0x12B8
	NumberImpassableRows(-1), // RA2: +0x12C0
	IsRadar(false),
	SpySat(false), // RA2: +0x1345
	IsHasChargeAnim(false),
	IsAnimDelayedFire(false), // RA2: +0x1347
	IsSiloDamage(false),
	IsCanUnitRepair(false),
	IsCanUnitReload(false),
	Cloning(false), // RA2: +0x134B
	IsFlat(false),
	IsDockUnload(false),
	IsRecoilless(false),
	IsHasStupidGuardMode(true),
	IsBridgeRepairHut(false),
	IsGate(false),
	IsSAM(false),
	IsConstructionYard(false),
	IsNukeSilo(false),
	IsRefinery(false),
	IsWeeder(false),
	IsWeaponsFactory(false),
	IsLaserFencePost(false),
	IsLaserFence(false),
	IsFirestormWall(false),
	IsHospital(false),
	IsArmory(false),
	IsEMPulseCannon(false),
	IsTickTank(false),
	IsTurretAnimAVoxel(false),
	IsBarrelAnimAVoxel(false), // RA2: +0x1360, set here instead of the body
	IsCloakGenerator(false),
	IsSensorArray(false),
	IsICBMLauncher(false),
	IsArtillary(false),
	IsHelipad(false),
	OrePurifier(false), // RA2: +0x1366
	IsGDIBarracks(false),
	IsNODBarracks(false),
	ChargedAnimTime(999.0f), // RA2: +0x136C
	DelayedFireDelay(0), // RA2: +0x1370
	SuperWeapon(SUPER_NONE),
	SuperWeapon2(SUPER_NONE),
	GateStages(9),
	PowersUpToLevel(-1),
	IsDamagedDoor(false), // RA2: as above (initializer order follows the executable's member order)
	IsInvisibleInGame(false),
	IsTerrainPalette(false),
	IsCanPlaceAnywhere(false),
	IsExtraDamageStage(true),
	CanAIBuildThis(false),
	IsBaseDefense(false),
	CloakRadiusInCells(20),
	ConcentricRadialIndicator(false), // RA2: +0x138C
	PsychicDetectionRadius(0), // RA2: +0x1390
	StartPitch(Dir256(64)), // RA2: 64
	IsDemandLoad(false),
	IsDemandLoadBuildup(false),
	IsFreeBuildup(false),
	IsThreatRatingNode(false), // RA2: the executable's form of this statement (0x457AC0)
	PrimaryFireDualOffset(false), // RA2: +0x13E4
	ProtectWithWall(false), // RA2: as above
	CanHideThings(true), // RA2: as above
	CanBeHidden(true), // RA2: as above
	CrateBeneath(false), // RA2: as above
	LeaveRubble(false), // RA2: as above
	CrateBeneathIsMoney(false), // RA2: as above
	NumberOfDocks(1), // RA2: +0x1400
	DockingOffsets(1) // RA2: +0x1404, one entry
{
	Create_ID();
	BuildingTypes.Add(this);
	HeapID = (StructType)BuildingTypes.ID(this);

	Init_Anim(BSTATE_CONSTRUCTION, 0, 1, 0);
	Init_Anim(BSTATE_IDLE, 0, 1, 0);
	Init_Anim(BSTATE_ACTIVE, 0, 1, 0);
	Init_Anim(BSTATE_AUX1, 0, 1, 0);
	Init_Anim(BSTATE_AUX2, 0, 1, 0);

	for (int i = 0; i < 18; i++) { // RA2: eighteen, not BANIM_COUNT
		memset(AnimData[i].Anim, 0, sizeof(AnimData[i].Anim));
		memset(AnimData[i].AnimDamaged, 0, sizeof(AnimData[i].AnimDamaged));
		memset(AnimData[i].AnimGarrisoned, 0, sizeof(AnimData[i].AnimGarrisoned)); // RA2: the garrisoned animation name is cleared too
		AnimData[i].Location = Point2D(0, 0);
		AnimData[i].ZAdjust = 0;
		AnimData[i].YSort = 0;
		AnimData[i].Powered = true;
		AnimData[i].PoweredLight = false;
	}

	BuildupFilename[0] = '\0';
	PowersUpBuilding[0] = '\0';
	TheaterImageFile[0] = '\0';
	VoxelBarrelFile[0] = '\0';
	IsTrainable = false;
	DeployingAnimLoaded = false; // RA2: the four loaded flags are cleared in the body
	UnderDoorAnimLoaded = false; // RA2: as above
	RoofDeployingAnimLoaded = false; // RA2: as above
	UnderRoofDoorAnimLoaded = false; // RA2: as above
	for (int first = 0; first < 10; first++) { // RA2: every muzzle flash and damage fire offset starts at the none-coordinate's X/Y
		MuzzleFlash[first] = Point2D(COORD_NONE.X, COORD_NONE.Y); // RA2: as above
	}
	for (int second = 0; second < 8; second++) { // RA2: as above
		DamageFireOffset[second] = Point2D(COORD_NONE.X, COORD_NONE.Y); // RA2: as above
	}
	DockingOffsets[0] = Point3D(0, 0, 0); // RA2: the one docking offset
	ImmuneToPsionics = true; // RA2: TechnoTypeClass members set by this constructor
	Parasiteable = false; // RA2: as above
	Organic = false; // RA2: as above
	ConsideredAircraft = false; // RA2: as above
}




/// <summary>
/// Constructs a building type without initializing it.
/// This constructor is used by the load game system, which fills the object in from the
/// save file rather than from the rules.
/// </summary>
BuildingTypeClass::BuildingTypeClass(NoInitClass const & x) :
	BASECLASS(x),
	BuildupData(NULL), /// Will get loaded on demand
	BuildupFilename(x),
	PowersUpBuilding(x)
{

}


/// <summary>
/// Destroys the building type and releases any art it demand loaded.
/// The type also detaches itself from everything that refers to it and drops out of the
/// building type heap.
/// </summary>
BuildingTypeClass::~BuildingTypeClass(void)
{
	if (BuildupLoaded && BuildupData != NULL) { // RA2: 0x4581C0 frees the six images it loaded, in this order; not ImageData
		delete (ShapeSet *)BuildupData;
		BuildupData = NULL;
		BuildupLoaded = false; // RA2: ~BuildingTypeClass at 0x4581C0 matches only so
	}
	if (BibShapeLoaded && BibShape != NULL) { // RA2: as above
		delete (ShapeSet *)BibShape; // RA2: as above
		BibShape = NULL; // RA2: as above
		BibShapeLoaded = false; // RA2: as above
	}
	if (DeployingAnimLoaded && DeployingAnim != NULL) { // RA2: as above
		delete (ShapeSet *)DeployingAnim; // RA2: as above
		DeployingAnim = NULL; // RA2: as above
		DeployingAnimLoaded = false; // RA2: as above
	}
	if (UnderDoorAnimLoaded && UnderDoorAnim != NULL) { // RA2: as above
		delete (ShapeSet *)UnderDoorAnim; // RA2: as above
		UnderDoorAnim = NULL; // RA2: as above
		UnderDoorAnimLoaded = false; // RA2: as above
	}
	if (RoofDeployingAnimLoaded && RoofDeployingAnim != NULL) { // RA2: as above
		delete (ShapeSet *)RoofDeployingAnim; // RA2: as above
		RoofDeployingAnim = NULL; // RA2: as above
		RoofDeployingAnimLoaded = false; // RA2: as above
	}
	if (UnderRoofDoorAnimLoaded && UnderRoofDoorAnim != NULL) { // RA2: as above
		delete (ShapeSet *)UnderRoofDoorAnim; // RA2: as above
		UnderRoofDoorAnim = NULL; // RA2: as above
		UnderRoofDoorAnimLoaded = false; // RA2: as above
	}
	Detach_This_From_All(this, true);
	BuildingTypes.Delete(this);
}


/***********************************************************************************************
 * BuildingTypeClass::One_Time -- Performs special one time action for buildings.              *
 *                                                                                             *
 *    This routine is used to do the one time action necessary to handle building type class   *
 *    objects. This entails loading of the building shapes and the brain file used by          *
 *    buildings.                                                                               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine should only be called ONCE.                                        *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/28/1994 JLB : Created.                                                                 *
 *   06/11/1994 JLB : Updated construction time and frame count logic.                         *
 *=============================================================================================*/
void BuildingTypeClass::One_Time(void)
{

}


/// <summary>
/// Converts a given name into a building type number.
/// This routine matches against the name the building is known by in the game, rather
/// than the name it is declared under in the rules.
/// </summary>
/// <param name="name">The given name of the building to search for.</param>
/// <returns>Returns with the building type that matches, or STRUCT_NONE if there is no
/// match.</returns>
StructType BuildingTypeClass::From_Given_Name(char const * name)
{
	if (name != NULL) {
		for (int classid = STRUCT_FIRST; classid < BuildingTypes.Count(); classid++) {
			if (stricmp(BuildingTypes[classid]->GivenName, name) == 0) {
				return((StructType)classid);
			}
		}
	}
	return(STRUCT_NONE);
}


/***********************************************************************************************
 * Struct_From_Name -- Find BData structure from its name.                                     *
 *                                                                                             *
 *    This routine will convert an ASCII name for a building class into                        *
 *    the actual building class it represents.                                                 *
 *                                                                                             *
 * INPUT:   name  -- ASCII representation of a building class.                                 *
 *                                                                                             *
 * OUTPUT:  Returns with the actual building class number that the string                      *
 *          represents.                                                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/07/1992 JLB : Created.                                                                 *
 *   05/02/1994 JLB : Converted to member function.                                            *
 *=============================================================================================*/
StructType BuildingTypeClass::From_Name(char const * name)
{
	if (name != NULL) {
		for (StructType classid = STRUCT_FIRST; classid < BuildingTypes.Count(); classid++) {
			if (stricmp(BuildingTypes[classid]->Name(), name) == 0) {
				return(classid);
			}
		}
	}
	return(STRUCT_NONE);
}


/***********************************************************************************************
 * BuildingTypeClass::Create_And_Place -- Creates and places a building object onto the map.   *
 *                                                                                             *
 *    This routine is used by the scenario editor to create and place buildings on the map.    *
 *                                                                                             *
 * INPUT:   cell     -- The cell that the building is to be placed upon.                       *
 *                                                                                             *
 *          house    -- The owner of the building.                                             *
 *                                                                                             *
 * OUTPUT:  bool; Was the building successfully created and placed on the map?                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/28/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool BuildingTypeClass::Create_And_Place(Cell const & cell, HouseClass * house) const
{
	BuildingClass * ptr;

	ptr = new BuildingClass(this, house);
	if (ptr != NULL) {
		return(ptr->Unlimbo(cell, DIR_N));
	}
	return(false);
}


/***********************************************************************************************
 * BuildingTypeClass::Create_One_Of -- Creates a building of this type.                        *
 *                                                                                             *
 *    This routine will create a building object of this type. The building object is in a     *
 *    limbo state. It is presumed that the building object will be unlimboed at the correct    *
 *    place and time. Typical use is when the building is created in a factory situation       *
 *    and will be placed on the map when construction completes.                               *
 *                                                                                             *
 * INPUT:   house -- Pointer to the house that is to be the owner of the building.             *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the building. If the building could not be created       *
 *          then a NULL is returned.                                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/07/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
ObjectClass * BuildingTypeClass::Create_One_Of(HouseClass * house) const
{
	return(new BuildingClass(this, house));
}


/***********************************************************************************************
 * BuildingTypeClass::Init_Anim -- Initialize an animation control for a building.             *
 *                                                                                             *
 *    This routine will initialize one animation control element for a                         *
 *    specified building. This modifies a "const" class and thus must                          *
 *    perform some strategic casting to get away with this.                                    *
 *                                                                                             *
 * INPUT:   state -- The animation state to apply these data values to.                        *
 *                                                                                             *
 *          start -- Starting frame for the building's animation.                              *
 *                                                                                             *
 *          count -- The number of frames in this animation.                                   *
 *                                                                                             *
 *          rate  -- The countdown timer between animation frames.                             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/18/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void BuildingTypeClass::Init_Anim(BStateType state, int start, int count, int rate) const
{
	((int &)Anims[state].Start) = start;
	((int &)Anims[state].Count) = count;
	((int &)Anims[state].Rate) = rate;
}


/// <summary>
/// Fetches the depth shape shared by all buildings.
/// This routine loads the Z shape that buildings are rendered against and biases its
/// values into the range the depth buffer expects.
/// </summary>
void BuildingTypeClass::Fetch_Z_Data(void)
{
	if (BuildingZShape != NULL) {
		delete [] (char*) BuildingZShape;
		BuildingZShape = NULL;
	}

	int size = CCFileClass("BUILDNGZ.SHP").Size();
	BuildingZShape = new char[size];
	memcpy((void *)BuildingZShape, MFCD::Retrieve("BUILDNGZ.SHP"), size);

	ShapeSet * zshape = (ShapeSet *)BuildingZShape;
	char * data = (char *)zshape->Get_Data(0);
	int width = zshape->Get_Width();
	int height = zshape->Get_Height();

	for (int h = 0; h < height; h++) {
		for (int w = 0; w < width; w++) {
			if (data[h * width + w] != 0) {
				data[h * width + w] -= 65; // RA2: sub 65 (0x458510)
			}
		}
	}
}


/***********************************************************************************************
 * BuildingTypeClass::Init -- Performs theater specific initialization.                        *
 *                                                                                             *
 *    This routine is used to perform any initialization that is custom per theater.           *
 *    Typically, this is fetching the building shape data for those building types that have   *
 *    theater specific art.                                                                    *
 *                                                                                             *
 * INPUT:   theater  -- The theater to base this initialization on.                            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void BuildingTypeClass::Init(TheaterType theater)
{
	Fetch_Z_Data();
	PowerOffShapes = MFCD::Retrieve("POWEROFF.SHP");
	WrenchShapes = MFCD::Retrieve("WRENCH.SHP");

	char fullname[_MAX_FNAME+_MAX_EXT];

	int maxcloak = 0;

	for (StructType sindex = STRUCT_FIRST; sindex < BuildingTypes.Count(); sindex++) {
		BuildingTypeClass * classptr = BuildingTypes[sindex];

		maxcloak = MAX(maxcloak, classptr->CloakRadiusInCells);

		if (classptr->IsTheater) {

			if (!classptr->IsDemandLoad) {
				_makepath(fullname, NULL, NULL, classptr->Graphic_Name(), Theaters[theater].Suffix);
				classptr->ImageData = MFCD::Retrieve(fullname);
			} else {
				if (classptr->ImageData != NULL) {
					delete (ShapeSet *)classptr->ImageData;
					classptr->ImageData = NULL;
					classptr->ImageAllocated = false; // RA2: 0x458600 clears the flag too
				}
			}

			/*
			**	Buildup data is probably theater specific as well. Fetch a pointer to the
			**	data at this time as well.
			*/
			if (!classptr->IsDemandLoadBuildup) {
				_makepath(fullname, NULL, NULL, classptr->BuildupFilename, Theaters[theater].Suffix);
				classptr->BuildupData = MFCD::Retrieve(fullname);
			} else {
				if (classptr->BuildupData != NULL) {
					delete (ShapeSet *)classptr->BuildupData;
					classptr->BuildupData = NULL;
					classptr->BuildupLoaded = false; // RA2: 0x458600 clears the flag too
				}
			}

			if (classptr->BuildupData) {
				int timedelay = 1;
				int count = ((ShapeSet const *)classptr->BuildupData)->Get_Count();
				if (count != 0) {
					timedelay = (5 * TICKS_PER_SECOND) / count;
				}
				classptr->Init_Anim(BSTATE_CONSTRUCTION, 0, count, timedelay);
			}
		} else if (classptr->IsNewTheater) {
			if (classptr->IsDemandLoad) {
				if (classptr->ImageData != NULL) {
					delete (ShapeSet *)classptr->ImageData;
					classptr->ImageData = NULL;
					classptr->ImageAllocated = false; // RA2: 0x458600 clears the flag too
				}
			}
			if (classptr->IsDemandLoadBuildup) {
				if (classptr->BuildupData != NULL) {
					delete (ShapeSet *)classptr->BuildupData;
					classptr->BuildupData = NULL;
					classptr->BuildupLoaded = false; // RA2: 0x458600 clears the flag too
				}
			}
			classptr->Fetch_Building_Normal_Image(theater);
		}
	}

	if (maxcloak != 0) {
		delete CloakingSurface;
		CloakingSurface = new BSurface(maxcloak + 16, maxcloak + 16, 1);
	}
}


/***********************************************************************************************
 * BuildingTypeClass::Dimensions -- Fetches the pixel dimensions of the building.              *
 *                                                                                             *
 *    This routine will fetch the dimensions of the building (in pixels). These dimensions are *
 *    used to render the selection rectangle and the health bar.                               *
 *                                                                                             *
 * INPUT:   width    -- Reference to the pixel width (to be filled in).                        *
 *                                                                                             *
 *          height   -- Reference to the pixel height (to be filled in).                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
Point3D BuildingTypeClass::Pixel_Dimensions(void) const
{
	ShapeSet const * data = (ShapeSet const *)Get_Image_Data();
	int w;
	int h;
	if (data != NULL) {
		Rect r = data->Get_Rect(0);
		w = r.Width;
		h = r.Height;
	} else {
		w = 0;
		h = 0;
	}
	return(Point3D(w, w, h));
}


/***********************************************************************************************
 * BuildingTypeClass::Occupy_List -- Fetches the occupy list for the building.                 *
 *                                                                                             *
 *    Use this routine to fetch the occupy list pointer for the building. The occupy list is   *
 *    used to determine what cells the building occupies and thus precludes other buildings    *
 *    or objects from using.                                                                   *
 *                                                                                             *
 * INPUT:   placement   -- Is this for placement legality checking only? The normal condition  *
 *                         is for marking occupation flags.                                    *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to a cell offset list to be used to determine what cells    *
 *          this building occupies.                                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
Cell const * BuildingTypeClass::Occupy_List(bool placement) const
{
	static Cell const _templap[] = {REFRESH_EOL};
	if (OccupyList != NULL) {
		return(OccupyList);
	}
	return(_templap);
}


/***********************************************************************************************
 * BuildingTypeClass::Width -- Determines width of building in icons.                          *
 *                                                                                             *
 *    Use this routine to determine the width of the building type in icons.                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the building width in icons.                                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int BuildingTypeClass::Width(void) const
{
	return(SizeWidth[Size]);
}


/***********************************************************************************************
 * BuildingTypeClass::Height -- Determines the height of the building in icons.                *
 *                                                                                             *
 *    Use this routine to find the height of the building in icons.                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the building height in icons.                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int BuildingTypeClass::Height(bool bib) const
{
	return(SizeHeight[Size] + ((bib && IsBibbed) ? 1 : 0));
}


/***********************************************************************************************
 * BuildingTypeClass::Max_Pips -- Determines the maximum pips to display.                      *
 *                                                                                             *
 *    Use this routine to determine the maximum number of pips to display on this building     *
 *    when it is rendered. Typically, this is the tiberium capacity divided by 100.            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of pips to display on this building when selected.         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int BuildingTypeClass::Max_Pips(void) const
{
	int maxpips = (Width() * 60) / 8; // RA2: Max_Pips at 0x458980 matches only so

	switch (PipScale) {
		case PIPSCALE_TIBERIUM:
			if (IsWeeder) {
				return(Bound((int)(Rule->WeedCapacity), 0, maxpips));
			} else {
				return(Bound((int)(Capacity), 0, maxpips));
			}

		case PIPSCALE_POWER:
			return(maxpips);

		default:
			return(BASECLASS::Max_Pips());
	}
}


/***********************************************************************************************
 * BuildingTypeClass::Raw_Cost -- Fetches the raw (base) cost of this building type.           *
 *                                                                                             *
 *    This routine is used to fetch the real raw base cost of the building. The raw cost       *
 *    is the cost of the building less any free unit that would come with the building         *
 *    if it were built in the normal fashion. Specifically, the helicopter cost is subtracted  *
 *    from the helipad and the harvester cost is subtracted from the refinery. This cost       *
 *    is used for refunding.                                                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the raw (base) cost to build the building of this type.                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int BuildingTypeClass::Raw_Cost(void) const
{
	int cost = BASECLASS::Raw_Cost();

	if (this == Rule->PadAircraft[0]->Dock[0] && !Rule->IsSeparate) {
		cost -= (Rule->PadAircraft[0]->Raw_Cost() + Rule->PadAircraft[1]->Raw_Cost())/2;
	}
	if (FreeUnit != NULL) {
		cost -= FreeUnit->Raw_Cost();
		cost = MAX(cost, 0);
	}
	return(cost);
}


/***********************************************************************************************
 * BuildingTypeClass::Cost_Of -- Fetches the cost of this building.                            *
 *                                                                                             *
 *    This routine will fetch the cost to build the building of this type.                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the cost to produce this building.                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int BuildingTypeClass::Cost_Of(HouseClass * house) const
{
	int cost = BASECLASS::Cost_Of(house);

	if (this == Rule->PadAircraft[0]->Dock[0] && !Rule->IsSeparate) {
		cost += (Rule->PadAircraft[0]->Cost_Of(house) + Rule->PadAircraft[1]->Cost_Of(house))/2;
	}
	if (FreeUnit != NULL) {
		cost += FreeUnit->Cost_Of(house);
		cost = MAX(cost, 0);
	}
	return(cost);
}


/***********************************************************************************************
 * BuildingTypeClass::Flush_For_Placement -- Tries to clear placement area for this building t *
 *                                                                                             *
 *    This routine is called when a clear space for placement is desired at the cell location  *
 *    specified. Typical use of this routine is by the computer when it wants to build up      *
 *    its base.                                                                                *
 *                                                                                             *
 * INPUT:   cell  -- The cell that the building of this type would like to be placed down at.  *
 *                                                                                             *
 *          house -- Pointer to the house that want to clear the foundation zone.              *
 *                                                                                             *
 * OUTPUT:  Placement is temporarily blocked, please try again later?                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/27/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int BuildingTypeClass::Flush_For_Placement(Cell const & cell, HouseClass * house) const
{
	bool again = false;
	if (cell != CELL_NONE) {
		Cell const * list = Occupy_List(true);

		while (*list != REFRESH_EOL) {
			Cell newcell = cell + *list++;

			if (Map.In_Radar(newcell)) {
				CellClass & cptr = Map[newcell];
				if (cptr.Overlay != OVERLAY_NONE && (this != Rule->WallTower || cptr.Overlay != OVERLAY_BRICK_WALL)) {
					return(2);
				}
				ObjectClass * occupier = cptr.Cell_Occupier();
				if (occupier != NULL) {
					if (occupier->RTTI == RTTI_TERRAIN) {
						return(2);
					}
					if (occupier->AbstractFlags & ABSTRACT_TECHNO) { // RA2: Flush_For_Placement at 0x458B10 matches only so
						TechnoClass * techno = (TechnoClass *)occupier; // RA2: as above
						if (techno->RTTI == RTTI_BUILDING) {
							if (!((BuildingClass *)techno)->Can_Upgrade(this, house)) {
								return(2);
							}
						} else {
							if (techno->House->Is_Ally(house) && techno->Is_Foot()) {
								again = true;
								if (((FootClass *)techno)->NavCom == NULL || &Map[techno->Get_Coord()] == ((FootClass *)techno)->NavCom) {
									Map[newcell].Incoming(COORD_NONE, true, true);
								}
							} else {
								return(2);
							}
						}
					}
				}
			}
		}
	}
	if (again) {
		return(1);
	} else {
		return(0);
	}

}


/// <summary>
/// Fetches the shape data for this building.
/// This routine will load the art the first time it is asked for if this building type
/// was flagged to defer loading until it is really needed.
/// </summary>
/// <returns>Returns with a pointer to the shape data, or NULL if there is none.</returns>
void Generic_Theater_Name(char * name); // RA2: 0x5D7C20 (objtype.cpp)
void Theater_Name_Fixup(char * name, TheaterType theater); // RA2: 0x5D7BD0 (objtype.cpp)

void const * BuildingTypeClass::Get_Image_Data(void) const
{
	if (ImageData == NULL && IsDemandLoad) {
		if (TheaterImageFile[0] != '\0') {
			CCFileClass file(TheaterImageFile);
			file.Is_Available();
			(void *&)ImageData = Load_Alloc_Data(file);
			if (ImageData != NULL) { // RA2: 0x458CE0 sets ImageAllocated, else retries the generic theater name
				(bool &)ImageAllocated = true; // RA2: Get_Image_Data at 0x458CE0 matches only so
			} else {
				char fallback[260]; // RA2: as above
				strncpy(fallback, TheaterImageFile, 19); // RA2: as above
				Generic_Theater_Name(fallback); // RA2: as above
				CCFileClass file2(fallback); // RA2: as above
				(void *&)ImageData = Load_Alloc_Data(file2); // RA2: as above
				if (ImageData != NULL) { // RA2: as above
					(bool &)ImageAllocated = true; // RA2: as above
				}
			}
		}
		return(ImageData);
	}
	return(ImageData);
}

/// <summary>
/// Fetches the frame that shows this building as rubble.
/// </summary>
/// <returns>bool; Was there a rubble frame to give?</returns>
bool BuildingTypeClass::Get_Rubble_Shape(ShapeSet const ** shape, int * frame) // RA2: not in TibSun, at 0x458E00
{
	if (LeaveRubble && shape != NULL) {
		*shape = (ShapeSet const *)Get_Image_Data();
		if (*shape != NULL && (*shape)->Get_Count() / 2 > 3) {
			*frame = 3;
			return(true);
		}
	}
	return(false);
}


/// <summary>
/// Fetches the shadow frame of this building's rubble.
/// </summary>
/// <returns>bool; Was there a rubble shadow frame to give?</returns>
bool BuildingTypeClass::Get_Rubble_Shadow(ShapeSet const ** shape, int * frame) // RA2: not in TibSun, at 0x458E50
{
	if (LeaveRubble && shape != NULL) {
		*shape = (ShapeSet const *)Get_Image_Data();
		if (*shape != NULL && (*shape)->Get_Count() / 2 > 3) {
			*frame = (*shape)->Get_Count() / 2 + 3;
			return(true);
		}
	}
	return(false);
}


/// <summary>
/// Fetches the shape art for this building.
/// This routine picks up the building's own shape along with all of its supporting art --
/// the construction animation, the deploy and door animations, the bib and the special Z
/// overlay -- and then fetches any voxel art the building needs as well.
/// </summary>
/// <param name="theater">The theater to fetch the art for.</param>
void BuildingTypeClass::Fetch_Building_Normal_Image(TheaterType theater)
{
	char fullname[MAX_PATH];
	char buffer[64];

	if (!IsDemandLoadBuildup && strlen(BuildupFilename) != 0) { // RA2: one condition (0x458EA0)
		_makepath(fullname, NULL, NULL, BuildupFilename, ".SHP"); // RA2: moved with the buildup block, which now runs as one condition (0x458EA0)
		Theater_Name_Fixup(fullname, theater); // RA2: the free function at 0x5D7BD0
		void const * shape = MFCD::Retrieve(fullname); // RA2: the buildup shape is a local first
		if (shape == NULL) { // RA2: retries the generic theater name
			Generic_Theater_Name(fullname); // RA2: as above
			CCFileClass file(fullname); // RA2: as above
			shape = Load_Alloc_Data(file); // RA2: as above
		}
		BuildupData = shape; // RA2: as above
		if (shape != NULL) { // RA2: the executable's form of this statement (0x458EA0)
			int timedelay = 1; // RA2: as above
			int count = ((ShapeSet const *)shape)->Get_Count()/2; // RA2: the executable's form of this statement (0x458EA0)
			if (IsGate) { // RA2: as above
				count = GateStages + 1; // RA2: as above
			}
			if (count > 0) { // RA2: as above
				timedelay = (int)(Rule->BuildupTime * 900.0 / count); // RA2: double math (0x45E790)
			}
			Init_Anim(BSTATE_CONSTRUCTION, 0, count, timedelay); // RA2: as above
		}
	}

	ArtINI.Get_String(Graphic_Name(), "DeployingAnim", "", buffer, sizeof(buffer));
	if (strlen(buffer) != 0) {
		_makepath(fullname, NULL, NULL, buffer, ".SHP");
		Theater_Name_Fixup(fullname, theater); // RA2: as above
		Free_Deploying_Anim(); // RA2: frees an earlier copy first, loads through Load_Alloc_Data
		DeployingAnim = (ShapeSet const *)Load_Alloc_Data(fullname, DeployingAnimLoaded); // RA2: as above
		if (DeployingAnim == NULL) { // RA2: retries the generic theater name
			Generic_Theater_Name(fullname); // RA2: as above
			Free_Deploying_Anim(); // RA2: as above
			DeployingAnim = (ShapeSet const *)Load_Alloc_Data(fullname, DeployingAnimLoaded); // RA2: as above
		}
	}

	ArtINI.Get_String(Graphic_Name(), "RoofDeployingAnim", "", buffer, sizeof(buffer)); // RA2: not in TibSun
	if (strlen(buffer) != 0) { // RA2: as above
		_makepath(fullname, NULL, NULL, buffer, ".SHP"); // RA2: as above
		Theater_Name_Fixup(fullname, theater); // RA2: as above
		Free_Roof_Deploying_Anim(); // RA2: as above
		RoofDeployingAnim = (ShapeSet const *)Load_Alloc_Data(fullname, RoofDeployingAnimLoaded); // RA2: as above
		if (RoofDeployingAnim == NULL) { // RA2: as above
			Generic_Theater_Name(fullname); // RA2: as above
			Free_Roof_Deploying_Anim(); // RA2: as above
			RoofDeployingAnim = (ShapeSet const *)Load_Alloc_Data(fullname, DeployingAnimLoaded); // RA2: the retry sets DeployingAnimLoaded, not the roof flag
		}
	}

	ArtINI.Get_String(Graphic_Name(), "DoorAnim", "", buffer, sizeof(buffer));
	if (strlen(buffer) != 0) {
		_makepath(fullname, NULL, NULL, buffer, ".SHP");
		Theater_Name_Fixup(fullname, theater); // RA2: as above
		DoorAnim = (ShapeSet const *)MFCD::Retrieve(fullname);
		if (DoorAnim == NULL) { // RA2: retries the generic theater name
			Generic_Theater_Name(fullname); // RA2: as above
			DoorAnim = (ShapeSet const *)MFCD::Retrieve(fullname); // RA2: as above
		}
	}

	ArtINI.Get_String(Graphic_Name(), "UnderDoorAnim", "", buffer, sizeof(buffer));
	if (strlen(buffer) != 0) {
		_makepath(fullname, NULL, NULL, buffer, ".SHP");
		Theater_Name_Fixup(fullname, theater); // RA2: as above
		Free_Under_Door_Anim(); // RA2: frees an earlier copy first, loads through Load_Alloc_Data
		UnderDoorAnim = (ShapeSet const *)Load_Alloc_Data(fullname, UnderDoorAnimLoaded); // RA2: as above
		if (UnderDoorAnim == NULL) { // RA2: retries the generic theater name through the mix
			Generic_Theater_Name(fullname); // RA2: as above
			Free_Under_Door_Anim(); // RA2: as above
			UnderDoorAnim = (ShapeSet const *)MFCD::Retrieve(fullname); // RA2: as above
		}
	}

	ArtINI.Get_String(Graphic_Name(), "UnderRoofDoorAnim", "", buffer, sizeof(buffer)); // RA2: not in TibSun
	if (strlen(buffer) != 0) { // RA2: as above
		_makepath(fullname, NULL, NULL, buffer, ".SHP"); // RA2: as above
		Theater_Name_Fixup(fullname, theater); // RA2: as above
		Free_Under_Roof_Door_Anim(); // RA2: as above
		UnderRoofDoorAnim = (ShapeSet const *)Load_Alloc_Data(fullname, UnderRoofDoorAnimLoaded); // RA2: as above
		if (UnderRoofDoorAnim == NULL) { // RA2: as above
			Generic_Theater_Name(fullname); // RA2: as above
			Free_Under_Roof_Door_Anim(); // RA2: as above
			UnderRoofDoorAnim = (ShapeSet const *)MFCD::Retrieve(fullname); // RA2: as above
		}
	}

	ArtINI.Get_String(Graphic_Name(), "SpecialZOverlay", "", buffer, sizeof(buffer));
	if (strlen(buffer) != 0) {
		_makepath(fullname, NULL, NULL, buffer, ".SHP");
		Theater_Name_Fixup(fullname, theater); // RA2: as above
		SpecialZOverlay = (ShapeSet const *)MFCD::Retrieve(fullname);
		if (SpecialZOverlay == NULL) { // RA2: retries the generic theater name
			Generic_Theater_Name(fullname); // RA2: as above
			SpecialZOverlay = (ShapeSet const *)MFCD::Retrieve(fullname); // RA2: as above
		}
	}

	ArtINI.Get_String(Graphic_Name(), "BibShape", "", buffer, sizeof(buffer));
	if (strlen(buffer) != 0) {
		_makepath(fullname, NULL, NULL, buffer, ".SHP");
		Theater_Name_Fixup(fullname, theater); // RA2: as above
		Free_Bib_Shape(); // RA2: frees an earlier copy first, loads through Load_Alloc_Data
		BibShape = (ShapeSet const *)Load_Alloc_Data(fullname, BibShapeLoaded); // RA2: as above
		if (BibShape == NULL) { // RA2: retries the generic theater name through the mix
			Generic_Theater_Name(fullname); // RA2: as above
			Free_Bib_Shape(); // RA2: as above
			BibShape = (ShapeSet const *)MFCD::Retrieve(fullname); // RA2: as above
		}
	}

	char ext[16];
	ArtINI.Get_String(Graphic_Name(), "Image", "", buffer, sizeof(buffer));
	if (IsTheater && theater != THEATER_NONE) { // RA2: the theater test comes first
		strcpy(ext, Theaters[theater].Suffix);
	} else { // RA2: the theater suffix is the first branch in 0x458EA0
		strcpy(ext, ".SHP"); // RA2: as above
	}

	if (strlen(buffer)) {
		_makepath(fullname, NULL, NULL, buffer, ext);
	} else { // RA2: the theater suffix is the first branch in 0x458EA0
		_makepath(fullname, NULL, NULL, Graphic_Name(), ext);
	}

	Theater_Name_Fixup(fullname, theater); // RA2: as above
	strncpy(TheaterImageFile, fullname, sizeof(TheaterImageFile) - 1);

	/*
	**	Fetch the normal game shape for this building.
	*/
	if (!IsDemandLoad) {
		ImageData = MFCD::Retrieve(fullname);
		if (ImageData == NULL) { // RA2: retries the generic theater name
			Generic_Theater_Name(fullname); // RA2: as above
			strncpy(TheaterImageFile, fullname, sizeof(TheaterImageFile) - 1); // RA2: as above
			ImageData = MFCD::Retrieve(fullname); // RA2: as above
		}
	}

	Fetch_Building_Voxel_Image();
}




/// <summary>
/// Searches for a substring, ignoring case.
/// Both strings are uppercased in place, so neither survives the call unchanged.
/// </summary>
/// <param name="string">String to search; uppercased in place.</param>
/// <param name="substring">Substring to search for; uppercased in place.</param>
/// <returns>Returns with a pointer to the substring within the string. Otherwise, NULL is
/// returned.</returns>
static char * Upr_Strstr(char * string, char const * substring)
{
	return(strstr(_strupr(string), _strupr((char *)substring)));
}


/// <summary>
/// Fetches the voxel art for this building's turret and barrel.
/// This routine is used by those buildings that draw their turret or barrel as a voxel
/// rather than as an animation. The matching motion data is loaded alongside each model.
/// </summary>
void BuildingTypeClass::Fetch_Building_Voxel_Image(void)
{
	char buffer[128];
	char fullname[256];

	if (IsTurretAnimAVoxel || IsBarrelAnimAVoxel) {
		strcpy(buffer, AnimData[BANIM_TURRET].Anim);
		_makepath(fullname, NULL, NULL, buffer, ".VXL");

		if (strstr(strupr(&fullname[4]), strupr("TUR"))) { // RA2: Fetch_Building_Voxel_Image at 0x459640 matches only so
			CCFileClass vxl(fullname);
			if (vxl.Is_Available()) {
				delete AuxVoxel.VoxLib;
				AuxVoxel.VoxLib = new VoxelLibrary(vxl);

				_makepath(fullname, NULL, NULL, buffer, ".HVA");
				CCFileClass hva(fullname);
				delete AuxVoxel.MotLib;
				AuxVoxel.MotLib = new MotionLibrary(hva);

				if (AuxVoxel.MotLib != NULL && !AuxVoxel.MotLib->Load_Failed()) {
					AuxVoxel.MotLib->Scale(AuxVoxel.VoxLib->Get_Layer_Info(0, 0).Scale);
				}
			}

			if (char * nameroot = strstr(strupr(&buffer[4]), strupr("TUR"))) { // RA2: as above
				*nameroot++ = 'B';
				*nameroot++ = 'A';
				*nameroot++ = 'R';
				*nameroot++ = 'L';
				*nameroot++ = '\0';
			}
		} else if (IsBarrelAnimAVoxel) {
			strncpy(buffer, VoxelBarrelFile, sizeof(buffer));
		}

		_makepath(fullname, NULL, NULL, buffer, ".VXL");
		CCFileClass turvxl(fullname);
		if (turvxl.Is_Available()) {
			delete AuxVoxel2.VoxLib;
			AuxVoxel2.VoxLib = new VoxelLibrary(turvxl);

			_makepath(fullname, NULL, NULL, buffer, ".HVA");
			CCFileClass turhva(fullname);
			delete AuxVoxel2.MotLib;
			AuxVoxel2.MotLib = new MotionLibrary(turhva);

			if (AuxVoxel2.MotLib != NULL && !AuxVoxel2.MotLib->Load_Failed()) {
				AuxVoxel2.MotLib->Scale(AuxVoxel2.VoxLib->Get_Layer_Info(0, 0).Scale);
			}
		}
	}
}


/// <summary>
/// Tells whether two points are the same.
/// </summary>
static bool Same_Point(Point2D const & first, Point2D const & second) // RA2: not in TibSun; expanded into Read_INI (0x459A00)
{
	return(first.X == second.X && first.Y == second.Y); // RA2: as above
}


/***********************************************************************************************
 * BuildingTypeClass::Read_INI -- Fetch building type data from the INI database.              *
 *                                                                                             *
 *    This routine will fetch the building type class data from the INI database file.         *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database that will be examined.                      *
 *                                                                                             *
 * OUTPUT:  bool; Was the building entry found and the data extracted?                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool BuildingTypeClass::Read_INI(CCINIClass const & ini)
{
	char key[128]; // RA2: Read_INI's ten char buffers (0x459A00), sized as the executable's 856-byte frame shows
	char triple_out[64]; // RA2: as above
	char triple_def[64]; // RA2: as above
	char dock_key[40]; // RA2: as above
	char y_key[64]; // RA2: as above
	char ysort_key[64]; // RA2: as above
	char anim_key[64]; // RA2: as above
	char damaged_key[64]; // RA2: as above
	char z_key[64]; // RA2: as above
	char x_key[64]; // RA2: as above
	if (BASECLASS::Read_INI(ini)) {
		BuildCat = ini.Get_BuildCat(Name(), "BuildCat", BuildCat); // RA2: the executable reads "BuildCat" at this point of 0x459A00
		HasSpotlight = ini.Get_Bool(Name(), "HasSpotlight", HasSpotlight);
		HalfDamageSmokeLocation1 = ini.Get_Point(Name(), "HalfDamageSmokeLocation1", HalfDamageSmokeLocation1);
		HalfDamageSmokeLocation2 = ini.Get_Point(Name(), "HalfDamageSmokeLocation2", HalfDamageSmokeLocation2);
		IsRadar = ini.Get_Bool(Name(), "Radar", IsRadar);
		SpySat = ini.Get_Bool(Name(), "SpySat", SpySat); // RA2: the executable reads "SpySat" at this point of 0x459A00
		Speed = ini.Get_Bool(Name(), "WaterBound", Speed == SPEED_FLOAT) ? SPEED_FLOAT : SPEED_NONE;
		Adjacent = ini.Get_Int(Name(), "Adjacent", Adjacent);
		IsCaptureable = ini.Get_Bool(Name(), "Capturable", IsCaptureable);
		IsPowered = ini.Get_Bool(Name(), "Powered", IsPowered);
		Overpowerable = ini.Get_Bool(Name(), "Overpowerable", Overpowerable); // RA2: the executable reads "Overpowerable" at this point of 0x459A00
		Spyable = ini.Get_Bool(Name(), "Spyable", Spyable); // RA2: as above
		CanC4 = ini.Get_Bool(Name(), "CanC4", CanC4); // RA2: as above
		WantsExtraSpace = ini.Get_Bool(Name(), "WantsExtraSpace", WantsExtraSpace); // RA2: as above
		IsBibbed = ini.Get_Bool(Name(), "Bib", IsBibbed);
		IsUnsellable = ini.Get_Bool(Name(), "Unsellable", IsUnsellable);
		ClickRepairable = ini.Get_Bool(Name(), "ClickRepairable", ClickRepairable); // RA2: the executable reads "ClickRepairable" at this point of 0x459A00
		CanBeOccupied = ini.Get_Bool(Name(), "CanBeOccupied", CanBeOccupied); // RA2: as above
		ShowOccupantPips = ini.Get_Bool(Name(), "ShowOccupantPips", ShowOccupantPips); // RA2: as above
		MaxNumberOccupants = ini.Get_Int(Name(), "MaxNumberOccupants", MaxNumberOccupants); // RA2: as above
		NumberImpassableRows = ini.Get_Int(Name(), "NumberImpassableRows", NumberImpassableRows); // RA2: as above
		ProduceCashStartup = ini.Get_Int(Name(), "ProduceCashStartup", ProduceCashStartup); // RA2: as above
		ProduceCashAmount = ini.Get_Int(Name(), "ProduceCashAmount", ProduceCashAmount); // RA2: as above
		ProduceCashDelay = ini.Get_Int(Name(), "ProduceCashDelay", ProduceCashDelay); // RA2: as above
		IsBase = ini.Get_Bool(Name(), "BaseNormal", IsBase);
		EligibleForAllyBuilding = ini.Get_Bool(Name(), "EligibileForAllyBuilding", EligibleForAllyBuilding); // RA2: the executable reads "EligibileForAllyBuilding" at this point of 0x459A00
		EligibleForDelayKill = ini.Get_Bool(Name(), "EligibleForDelayKill", EligibleForDelayKill); // RA2: as above
		NeedsEngineer = ini.Get_Bool(Name(), "NeedsEngineer", NeedsEngineer); // RA2: as above
		if (NeedsEngineer) { // RA2: as above
			SightRange = 6; // RA2: as above
		}
		ProtectWithWall = ini.Get_Bool(Name(), "ProtectWithWall", ProtectWithWall); // RA2: the executable reads "ProtectWithWall" at this point of 0x459A00
		int index; // RA2: as above
		{ // RA2: the muzzle flash and damage fire offsets are read from the art INI (not in TibSun)
			char number[8]; // RA2: as above
			for (index = 0; index < MaxNumberOccupants; index++) { // RA2: as above
				strcpy(key, "MuzzleFlash"); // RA2: as above
				_itoa(index, number, 10); // RA2: as above
				Point2D point = ArtINI.Get_Point(Graphic_Name(), strcat(key, number), Point2D(0xFFFF, 0xFFFF)); // RA2: as above
				MuzzleFlash[index] = Point2D(point.X, point.Y); // RA2: as above
				if (Same_Point(MuzzleFlash[index], Point2D(0xFFFF, 0xFFFF))) { // RA2: as above
					MuzzleFlash[index] = Point2D(0, 0); // RA2: as above
				}
			}
			for (index = 0; index < 8; index++) { // RA2: the executable's form of this statement (0x459A00)
				strcpy(key, "DamageFireOffset"); // RA2: as above
				_itoa(index, number, 10); // RA2: as above
				Point2D point = ArtINI.Get_Point(Graphic_Name(), strcat(key, number), Point2D(0xFFFF, 0xFFFF)); // RA2: as above
				DamageFireOffset[index] = Point2D(point.X, point.Y); // RA2: as above
				if (Same_Point(DamageFireOffset[index], Point2D(0xFFFF, 0xFFFF))) { // RA2: as above
					DamageFireOffset[index] = Point2D(COORD_NONE.X, COORD_NONE.Y); // RA2: as above
					break; // RA2: as above
				}
			}
		}
		IsWall = ini.Get_Bool(Name(), "Wall", IsWall);
		IsWeeder = ini.Get_Bool(Name(), "Weeder", IsWeeder);
		IsHelipad = ini.Get_Bool(Name(), "Helipad", IsHelipad);
		OrePurifier = ini.Get_Bool(Name(), "OrePurifier", OrePurifier); // RA2: the executable reads "OrePurifier" at this point of 0x459A00
		ToBuild = ini.Get_RTTIType(Name(), "Factory", (RTTIType)ToBuild); // RA2: as above
		FreeUnit = TGet_Class(ini, Name(), "FreeUnit", FreeUnit);
		IsHoverPad = ini.Get_Bool(Name(), "HoverPad", IsHoverPad);
		IsTemple = ini.Get_Bool(Name(), "IsTemple", IsTemple);
		IsPlug = ini.Get_Bool(Name(), "IsPlug", IsPlug);
		IsCanTogglePower = ini.Get_Bool(Name(), "TogglePower", IsCanTogglePower);
		BuildupSound = ini.Get_VocType(Name(), "BuildupSound", (VocType)BuildupSound); // RA2: the executable reads "BuildupSound" at this point of 0x459A00
		PackupSound = ini.Get_VocType(Name(), "PackupSound", (VocType)PackupSound); // RA2: as above
		CreateUnitSound = ini.Get_VocType(Name(), "CreateUnitSound", (VocType)CreateUnitSound); // RA2: as above
		UnitExitSound = ini.Get_VocType(Name(), "UnitExitSound", (VocType)UnitExitSound); // RA2: as above
		UnitEnterSound = ini.Get_VocType(Name(), "UnitEnterSound", (VocType)UnitEnterSound); // RA2: as above
		WorkingSound = ini.Get_VocType(Name(), "WorkingSound", (VocType)WorkingSound); // RA2: as above
		NotWorkingSound = ini.Get_VocType(Name(), "NotWorkingSound", (VocType)NotWorkingSound); // RA2: as above
		IsCanUnitRepair = ini.Get_Bool(Name(), "UnitRepair", IsCanUnitRepair);
		IsCanUnitReload = ini.Get_Bool(Name(), "UnitReload", IsCanUnitReload);
		Cloning = ini.Get_Bool(Name(), "Cloning", Cloning); // RA2: the executable reads "Cloning" at this point of 0x459A00
		IsDockUnload = ini.Get_Bool(Name(), "DockUnload", IsDockUnload);
		IsGate = ini.Get_Bool(Name(), "Gate", IsGate);
		IsSAM = ini.Get_Bool(Name(), "SAM", IsSAM);
		IsConstructionYard = ini.Get_Bool(Name(), "ConstructionYard", IsConstructionYard);
		IsNukeSilo = ini.Get_Bool(Name(), "NukeSilo", IsNukeSilo);
		IsRefinery = ini.Get_Bool(Name(), "Refinery", IsRefinery);
		IsWeaponsFactory = ini.Get_Bool(Name(), "WeaponsFactory", IsWeaponsFactory);
		IsLaserFencePost = ini.Get_Bool(Name(), "LaserFencePost", IsLaserFencePost);
		IsLaserFence = ini.Get_Bool(Name(), "LaserFence", IsLaserFence);
		IsFirestormWall = ini.Get_Bool(Name(), "FirestormWall", IsFirestormWall);
		IsHospital = ini.Get_Bool(Name(), "Hospital", IsHospital);
		IsArmory = ini.Get_Bool(Name(), "Armory", IsArmory);
		IsGDIBarracks = ini.Get_Bool(Name(), "GDIBarracks", IsGDIBarracks);
		IsNODBarracks = ini.Get_Bool(Name(), "NODBarracks", IsNODBarracks);
		IsEMPulseCannon = ini.Get_Bool(Name(), "EMPulseCannon", IsEMPulseCannon);
		IsTickTank = ini.Get_Bool(Name(), "TickTank", IsTickTank);
		ChargedAnimTime = (float)ini.Get_Float(Name(), "ChargedAnimTime", ChargedAnimTime); // RA2: the executable reads "ChargedAnimTime" at this point of 0x459A00
		SuperWeapon = ini.Get_SuperWeaponType(Name(), "SuperWeapon", SuperWeapon);
		SuperWeapon2 = ini.Get_SuperWeaponType(Name(), "SuperWeapon2", SuperWeapon2);
		IsCloakGenerator = ini.Get_Bool(Name(), "CloakGenerator", IsCloakGenerator);
		IsSensorArray = ini.Get_Bool(Name(), "SensorArray", IsSensorArray);
		CloakRadiusInCells = ini.Get_Int(Name(), "CloakRadiusInCells", CloakRadiusInCells);
		PsychicDetectionRadius = ini.Get_Int(Name(), "PsychicDetectionRadius", PsychicDetectionRadius); // RA2: the executable reads "PsychicDetectionRadius" at this point of 0x459A00
		StartPitch = (Dir256)(ini.Get_Int(Name(), "BarrelStartPitch", StartPitch >> 5) << 5); // RA2: as above
		LightVisibility = ini.Get_Int(Name(), "LightVisibility", LightVisibility);
		// RA2: each light value is read into a double local and stored as thousandths plus 0.1 (0x459A00)
		{
			double value = ini.Get_Float(Name(), "LightIntensity", (double)(LightIntensity / 1000)); // RA2: the executable reads "LightIntensity" at this point of 0x459A00
			LightIntensity = (int)(value * 1000.0 + 0.1); // RA2: as above
		}
		{
			double value = ini.Get_Float(Name(), "LightRedTint", (double)(LightRedTint / 1000)); // RA2: the executable reads "LightRedTint" at this point of 0x459A00
			LightRedTint = (int)(value * 1000.0 + 0.1); // RA2: as above
		}
		{
			double value = ini.Get_Float(Name(), "LightGreenTint", (double)(LightGreenTint / 1000)); // RA2: the executable reads "LightGreenTint" at this point of 0x459A00
			LightGreenTint = (int)(value * 1000.0 + 0.1); // RA2: as above
		}
		{
			double value = ini.Get_Float(Name(), "LightBlueTint", (double)(LightBlueTint / 1000)); // RA2: the executable reads "LightBlueTint" at this point of 0x459A00
			LightBlueTint = (int)(value * 1000.0 + 0.1); // RA2: as above
		}
		GateCloseDelay = ini.Get_Float(Name(), "GateCloseDelay", GateCloseDelay);
		IsInvisibleInGame = ini.Get_Bool(Name(), "InvisibleInGame", IsInvisibleInGame);
		if (IsInvisibleInGame) {
			IsInvisible = true;
			IsRadarVisible = false;
		}
		ini.Get_String(Name(), "PowersUpBuilding", "", key, 24); // RA2: the executable reads "PowersUpBuilding" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(PowersUpBuilding, key); // RA2: as above
		}
		PowersUpToLevel = ini.Get_Int(Name(), "PowersUpToLevel", PowersUpToLevel);
		IsBridgeRepairHut = ini.Get_Bool(Name(), "BridgeRepairHut", IsBridgeRepairHut);
		IsHasStupidGuardMode = ini.Get_Bool(Name(), "HasStupidGuardMode", IsHasStupidGuardMode);
		CrateBeneath = ini.Get_Bool(Name(), "CrateBeneath", CrateBeneath); // RA2: the executable reads "CrateBeneath" at this point of 0x459A00
		CrateBeneathIsMoney = ini.Get_Bool(Name(), "CrateBeneathIsMoney", CrateBeneathIsMoney); // RA2: as above
		LeaveRubble = ini.Get_Bool(Name(), "LeaveRubble", LeaveRubble); // RA2: as above
		IsCanPlaceAnywhere = ini.Get_Bool(Name(), "PlaceAnywhere", IsCanPlaceAnywhere);
		IsICBMLauncher = ini.Get_Bool(Name(), "ICBMLauncher", IsICBMLauncher);
		IsArtillary = ini.Get_Bool(Name(), "Artillary", IsArtillary);
		{
			Point3D offset = Point3D(TargetCoordOffset); // RA2: a Coord like ExitCoordinate, copied as a block
			TargetCoordOffset = ini.Get_Point(Name(), "TargetCoordOffset", offset); // RA2: the executable reads "TargetCoordOffset" at this point of 0x459A00
		}
		{
			Point3D offset = Point3D(ExitCoordinate); // RA2: the executable's form of this statement (0x459A00)
			ExitCoordinate = ini.Get_Point(Name(), "ExitCoord", offset); // RA2: as above
		}
		CanAIBuildThis = ini.Get_Bool(Name(), "AIBuildThis", CanAIBuildThis);
		IsBaseDefense = ini.Get_Bool(Name(), "IsBaseDefense", IsBaseDefense);
		IsThreatRatingNode = ini.Get_Bool(Name(), "IsThreatRatingNode", IsThreatRatingNode);
		ConcentricRadialIndicator = ini.Get_Bool(Name(), "ConcentricRadialIndicator", ConcentricRadialIndicator); // RA2: the executable reads "ConcentricRadialIndicator" at this point of 0x459A00
		Rotation = IsTurretEquipped ? 32 : 1;
		Power = ini.Get_Int(Name(), "Power", Power > 0 ? Power : -Drain); // RA2: the executable reads "Power" at this point of 0x459A00
		if (Power < 0) {
			Drain = -Power;
			Power = 0;
		} else { // RA2: the theater suffix is the first branch in 0x458EA0
			Drain = 0;
		}
		ExtraPower = ini.Get_Int(Name(), "ExtraPower", ExtraPower > 0 ? ExtraPower : -ExtraDrain); // RA2: the executable reads "ExtraPower" at this point of 0x459A00
		if (ExtraPower < 0) { // RA2: as above
			ExtraDrain = -ExtraPower; // RA2: as above
			ExtraPower = 0; // RA2: as above
		}
		ZHeight = ArtINI.Get_Int(Graphic_Name(), "Height", ZHeight);
		OccupyHeight = ArtINI.Get_Int(Graphic_Name(), "OccupyHeight", ZHeight); // RA2: the executable reads "OccupyHeight" at this point of 0x459A00
		IsRecoilless = ArtINI.Get_Bool(Graphic_Name(), "Recoilless", IsRecoilless);
		IsFlat = ArtINI.Get_Bool(Graphic_Name(), "Flat", IsFlat);
		IsSiloDamage = ArtINI.Get_Bool(Graphic_Name(), "SiloDamage", IsSiloDamage);
		IsHasChargeAnim = ArtINI.Get_Bool(Graphic_Name(), "ChargeAnim", IsHasChargeAnim);
		IsAnimDelayedFire = ArtINI.Get_Bool(Graphic_Name(), "IsAnimDelayedFire", IsAnimDelayedFire); // RA2: the executable reads "IsAnimDelayedFire" at this point of 0x459A00
		DelayedFireDelay = ArtINI.Get_Int(Graphic_Name(), "DelayedFireDelay", DelayedFireDelay); // RA2: as above
		ToOverlay = TGet_Class(ArtINI, Graphic_Name(), "ToOverlay", ToOverlay);
		Size = ArtINI.Get_BSizeType(Graphic_Name(), "Foundation", Size);
		BSizeType foundation = ArtINI.Get_BSizeType(Name(), "Foundation", Size); // RA2: the executable reads "Foundation" at this point of 0x459A00
		if (foundation != BSIZE_FIRST) { // RA2: as above
			Size = foundation; // RA2: as above
		}
		MidPoint = ArtINI.Get_Int(Graphic_Name(), "MidPoint", MidPoint);
		DoorStages = ArtINI.Get_Int(Graphic_Name(), "DoorStages", DoorStages);
		IsDamagedDoor = ArtINI.Get_Bool(Graphic_Name(), "DamagedDoor", IsDamagedDoor);
		IsTerrainPalette = ArtINI.Get_Bool(Graphic_Name(), "TerrainPalette", IsTerrainPalette);
		GateStages = ArtINI.Get_Int(Graphic_Name(), "GateStages", GateStages);
		PrimaryFirePixelOffset = ArtINI.Get_Point(Graphic_Name(), "PrimaryFirePixelOffset", PrimaryFirePixelOffset);
		SecondaryFirePixelOffset = ArtINI.Get_Point(Graphic_Name(), "SecondaryFirePixelOffset", SecondaryFirePixelOffset);
		PrimaryFireDualOffset = ArtINI.Get_Bool(Graphic_Name(), "PrimaryFireDualOffset", PrimaryFireDualOffset); // RA2: the executable reads "PrimaryFireDualOffset" at this point of 0x459A00
		IsExtraDamageStage = ArtINI.Get_Bool(Graphic_Name(), "ExtraDamageStage", IsExtraDamageStage);
		SpecialZOverlayZAdjust = ArtINI.Get_Int(Graphic_Name(), "SpecialZOverlayZAdjust", SpecialZOverlayZAdjust);
		NormalZAdjust = ArtINI.Get_Int(Graphic_Name(), "NormalZAdjust", NormalZAdjust);
		ZShapePointMove = ArtINI.Get_Point(Graphic_Name(), "ZShapePointMove", ZShapePointMove);
		ExtraLight = ArtINI.Get_Int(Graphic_Name(), "ExtraLight", ExtraLight);
		IsDemandLoad = ArtINI.Get_Bool(Graphic_Name(), "DemandLoad", IsDemandLoad);
		IsDemandLoadBuildup = ArtINI.Get_Bool(Graphic_Name(), "DemandLoadBuildup", IsDemandLoadBuildup);
		IsFreeBuildup = ArtINI.Get_Bool(Graphic_Name(), "FreeBuildup", IsFreeBuildup);
		CanHideThings = ArtINI.Get_Bool(Name(), "CanHideThings", CanHideThings); // RA2: the executable reads "CanHideThings" at this point of 0x459A00
		CanBeHidden = ArtINI.Get_Bool(Name(), "CanBeHidden", CanBeHidden); // RA2: as above
		for (index = 0; index < 8; index++) { // RA2: as above
			wsprintfA(key, "AddOccupy%d", index + 1); // RA2: the "AddOccupy%d" keys fill the array at +0x12C4, which builtype.h names RemoveOccupy (the names look swapped; offsets are what the executable shows)
			RemoveOccupy[index] = ArtINI.Get_Point(Graphic_Name(), key, Point2D(0xFFFF, 0xFFFF)); // RA2: as above
			wsprintfA(key, "RemoveOccupy%d", index + 1); // RA2: as above
			AddOccupy[index] = ArtINI.Get_Point(Graphic_Name(), key, Point2D(0xFFFF, 0xFFFF)); // RA2: as above
		}
		QueueingCell = ArtINI.Get_Point(Graphic_Name(), "QueueingCell", Point2D(0, 0)); // RA2: the executable reads "QueueingCell" at this point of 0x459A00
		OccupyList = OccupyLists[Size];
		ExitList = ExitLists[Size];
		ArtINI.Get_String(Graphic_Name(), "Buildup", "", key, 16); // RA2: the executable reads "Buildup" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(BuildupFilename, key); // RA2: as above
		}
		Fetch_Building_Normal_Image(Scen->Theater); // RA2: the executable's form of this statement (0x459A00)
		sprintf(triple_def, "%d,%d,%d", Anims[BSTATE_IDLE].Start, Anims[BSTATE_IDLE].Count, Anims[BSTATE_IDLE].Rate); // RA2: as above
		if (ArtINI.Get_String(Graphic_Name(), "AnimIdle", triple_def, triple_out, 64) > 0) { // RA2: as above
			sscanf(triple_out, "%d,%d,%d", &Anims[BSTATE_IDLE].Start, &Anims[BSTATE_IDLE].Count, &Anims[BSTATE_IDLE].Rate); // RA2: as above
		}
		sprintf(triple_def, "%d,%d,%d", Anims[BSTATE_ACTIVE].Start, Anims[BSTATE_ACTIVE].Count, Anims[BSTATE_ACTIVE].Rate); // RA2: the executable's form of this statement (0x459A00)
		if (ArtINI.Get_String(Graphic_Name(), "AnimActive", triple_def, triple_out, 64) > 0) { // RA2: as above
			sscanf(triple_out, "%d,%d,%d", &Anims[BSTATE_ACTIVE].Start, &Anims[BSTATE_ACTIVE].Count, &Anims[BSTATE_ACTIVE].Rate); // RA2: as above
		}
		sprintf(triple_def, "%d,%d,%d", Anims[BSTATE_AUX1].Start, Anims[BSTATE_AUX1].Count, Anims[BSTATE_AUX1].Rate); // RA2: the executable's form of this statement (0x459A00)
		if (ArtINI.Get_String(Graphic_Name(), "AnimAux1", triple_def, triple_out, 64) > 0) { // RA2: as above
			sscanf(triple_out, "%d,%d,%d", &Anims[BSTATE_AUX1].Start, &Anims[BSTATE_AUX1].Count, &Anims[BSTATE_AUX1].Rate); // RA2: as above
		}
		sprintf(triple_def, "%d,%d,%d", Anims[BSTATE_AUX2].Start, Anims[BSTATE_AUX2].Count, Anims[BSTATE_AUX2].Rate); // RA2: the executable's form of this statement (0x459A00)
		if (ArtINI.Get_String(Graphic_Name(), "AnimAux2", triple_def, triple_out, 64) > 0) { // RA2: as above
			sscanf(triple_out, "%d,%d,%d", &Anims[BSTATE_AUX2].Start, &Anims[BSTATE_AUX2].Count, &Anims[BSTATE_AUX2].Rate); // RA2: as above
		}
		ArtINI.Get_String(Graphic_Name(), "ActiveAnim", "", key, 16); // RA2: the executable reads "ActiveAnim" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_ACTIVE_ONE].Anim, key); // RA2: as above
		}
		ArtINI.Get_String(Graphic_Name(), "ActiveAnimDamaged", "", key, 16); // RA2: the executable reads "ActiveAnimDamaged" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_ACTIVE_ONE].AnimDamaged, key); // RA2: as above
		}
		if (strlen(AnimData[BANIM_ACTIVE_ONE].AnimDamaged) == 0) { // RA2: the executable's form of this statement (0x459A00)
			strcpy(AnimData[BANIM_ACTIVE_ONE].AnimDamaged, AnimData[BANIM_ACTIVE_ONE].Anim); // RA2: the damaged name defaults to the plain one
		}
		ArtINI.Get_String(Graphic_Name(), "ActiveAnimGarrisoned", "", key, 16); // RA2: the executable reads "ActiveAnimGarrisoned" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_ACTIVE_ONE].AnimGarrisoned, key); // RA2: as above
		}
		if (strlen(AnimData[BANIM_ACTIVE_ONE].AnimGarrisoned) == 0) { // RA2: the executable's form of this statement (0x459A00)
			strcpy(AnimData[BANIM_ACTIVE_ONE].AnimGarrisoned, AnimData[BANIM_ACTIVE_ONE].Anim); // RA2: as above
		}
		if (strlen(AnimData[BANIM_ACTIVE_ONE].Anim) != 0 || strlen(AnimData[BANIM_ACTIVE_ONE].AnimDamaged) != 0 || strlen(AnimData[BANIM_ACTIVE_ONE].AnimGarrisoned) != 0) { // RA2: the executable's form of this statement (0x459A00)
			AnimData[BANIM_ACTIVE_ONE].Location.X = ArtINI.Get_Int(Graphic_Name(), "ActiveAnimX", AnimData[BANIM_ACTIVE_ONE].Location.X);
			AnimData[BANIM_ACTIVE_ONE].Location.Y = ArtINI.Get_Int(Graphic_Name(), "ActiveAnimY", AnimData[BANIM_ACTIVE_ONE].Location.Y);
			AnimData[BANIM_ACTIVE_ONE].ZAdjust = ArtINI.Get_Int(Graphic_Name(), "ActiveAnimZAdjust", AnimData[BANIM_ACTIVE_ONE].ZAdjust);
			AnimData[BANIM_ACTIVE_ONE].YSort = ArtINI.Get_Int(Graphic_Name(), "ActiveAnimYSort", AnimData[BANIM_ACTIVE_ONE].YSort);
			AnimData[BANIM_ACTIVE_ONE].Powered = ArtINI.Get_Bool(Graphic_Name(), "ActiveAnimPowered", AnimData[BANIM_ACTIVE_ONE].Powered);
			AnimData[BANIM_ACTIVE_ONE].PoweredLight = ArtINI.Get_Bool(Graphic_Name(), "ActiveAnimPoweredLight", AnimData[BANIM_ACTIVE_ONE].PoweredLight);
		}
		ArtINI.Get_String(Graphic_Name(), "ActiveAnimTwo", "", key, 16); // RA2: the executable reads "ActiveAnimTwo" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_ACTIVE_TWO].Anim, key); // RA2: as above
		}
		ArtINI.Get_String(Graphic_Name(), "ActiveAnimTwoDamaged", "", key, 16); // RA2: the executable reads "ActiveAnimTwoDamaged" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_ACTIVE_TWO].AnimDamaged, key); // RA2: as above
		}
		if (strlen(AnimData[BANIM_ACTIVE_TWO].AnimDamaged) == 0) { // RA2: the executable's form of this statement (0x459A00)
			strcpy(AnimData[BANIM_ACTIVE_TWO].AnimDamaged, AnimData[BANIM_ACTIVE_TWO].Anim); // RA2: the damaged name defaults to the plain one
		}
		ArtINI.Get_String(Graphic_Name(), "ActiveAnimTwoGarrisoned", "", key, 16); // RA2: the executable reads "ActiveAnimTwoGarrisoned" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_ACTIVE_TWO].AnimGarrisoned, key); // RA2: as above
		}
		if (strlen(AnimData[BANIM_ACTIVE_TWO].AnimGarrisoned) == 0) { // RA2: the executable's form of this statement (0x459A00)
			strcpy(AnimData[BANIM_ACTIVE_TWO].AnimGarrisoned, AnimData[BANIM_ACTIVE_TWO].Anim); // RA2: as above
		}
		if (strlen(AnimData[BANIM_ACTIVE_TWO].Anim) != 0 || strlen(AnimData[BANIM_ACTIVE_TWO].AnimDamaged) != 0 || strlen(AnimData[BANIM_ACTIVE_TWO].AnimGarrisoned) != 0) { // RA2: the executable's form of this statement (0x459A00)
			AnimData[BANIM_ACTIVE_TWO].Location.X = ArtINI.Get_Int(Graphic_Name(), "ActiveAnimTwoX", AnimData[BANIM_ACTIVE_TWO].Location.X);
			AnimData[BANIM_ACTIVE_TWO].Location.Y = ArtINI.Get_Int(Graphic_Name(), "ActiveAnimTwoY", AnimData[BANIM_ACTIVE_TWO].Location.Y);
			AnimData[BANIM_ACTIVE_TWO].ZAdjust = ArtINI.Get_Int(Graphic_Name(), "ActiveAnimTwoZAdjust", AnimData[BANIM_ACTIVE_TWO].ZAdjust);
			AnimData[BANIM_ACTIVE_TWO].YSort = ArtINI.Get_Int(Graphic_Name(), "ActiveAnimTwoYSort", AnimData[BANIM_ACTIVE_TWO].YSort);
			AnimData[BANIM_ACTIVE_TWO].Powered = ArtINI.Get_Bool(Graphic_Name(), "ActiveAnimTwoPowered", AnimData[BANIM_ACTIVE_TWO].Powered);
			AnimData[BANIM_ACTIVE_TWO].PoweredLight = ArtINI.Get_Bool(Graphic_Name(), "ActiveAnimTwoPoweredLight", AnimData[BANIM_ACTIVE_TWO].PoweredLight);
		}
		ArtINI.Get_String(Graphic_Name(), "ActiveAnimThree", "", key, 16); // RA2: the executable reads "ActiveAnimThree" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_ACTIVE_THREE].Anim, key); // RA2: as above
		}
		ArtINI.Get_String(Graphic_Name(), "ActiveAnimThreeDamaged", "", key, 16); // RA2: the executable reads "ActiveAnimThreeDamaged" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_ACTIVE_THREE].AnimDamaged, key); // RA2: as above
		}
		if (strlen(AnimData[BANIM_ACTIVE_THREE].AnimDamaged) == 0) { // RA2: the executable's form of this statement (0x459A00)
			strcpy(AnimData[BANIM_ACTIVE_THREE].AnimDamaged, AnimData[BANIM_ACTIVE_THREE].Anim); // RA2: the damaged name defaults to the plain one
		}
		ArtINI.Get_String(Graphic_Name(), "ActiveAnimThreeGarrisoned", "", key, 16); // RA2: the executable reads "ActiveAnimThreeGarrisoned" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_ACTIVE_THREE].AnimGarrisoned, key); // RA2: as above
		}
		if (strlen(AnimData[BANIM_ACTIVE_THREE].AnimGarrisoned) == 0) { // RA2: the executable's form of this statement (0x459A00)
			strcpy(AnimData[BANIM_ACTIVE_THREE].AnimGarrisoned, AnimData[BANIM_ACTIVE_THREE].Anim); // RA2: as above
		}
		if (strlen(AnimData[BANIM_ACTIVE_THREE].Anim) != 0 || strlen(AnimData[BANIM_ACTIVE_THREE].AnimDamaged) != 0 || strlen(AnimData[BANIM_ACTIVE_THREE].AnimGarrisoned) != 0) { // RA2: the executable's form of this statement (0x459A00)
			AnimData[BANIM_ACTIVE_THREE].Location.X = ArtINI.Get_Int(Graphic_Name(), "ActiveAnimThreeX", AnimData[BANIM_ACTIVE_THREE].Location.X);
			AnimData[BANIM_ACTIVE_THREE].Location.Y = ArtINI.Get_Int(Graphic_Name(), "ActiveAnimThreeY", AnimData[BANIM_ACTIVE_THREE].Location.Y);
			AnimData[BANIM_ACTIVE_THREE].ZAdjust = ArtINI.Get_Int(Graphic_Name(), "ActiveAnimThreeZAdjust", AnimData[BANIM_ACTIVE_THREE].ZAdjust);
			AnimData[BANIM_ACTIVE_THREE].YSort = ArtINI.Get_Int(Graphic_Name(), "ActiveAnimThreeYSort", AnimData[BANIM_ACTIVE_THREE].YSort);
			AnimData[BANIM_ACTIVE_THREE].Powered = ArtINI.Get_Bool(Graphic_Name(), "ActiveAnimThreePowered", AnimData[BANIM_ACTIVE_THREE].Powered);
			AnimData[BANIM_ACTIVE_THREE].PoweredLight = ArtINI.Get_Bool(Graphic_Name(), "ActiveAnimThreePoweredLight", AnimData[BANIM_ACTIVE_THREE].PoweredLight);
		}
		ArtINI.Get_String(Graphic_Name(), "ActiveAnimFour", "", key, 16); // RA2: the executable reads "ActiveAnimFour" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_ACTIVE_FOUR].Anim, key); // RA2: as above
		}
		ArtINI.Get_String(Graphic_Name(), "ActiveAnimFourDamaged", "", key, 16); // RA2: the executable reads "ActiveAnimFourDamaged" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_ACTIVE_FOUR].AnimDamaged, key); // RA2: as above
		}
		if (strlen(AnimData[BANIM_ACTIVE_FOUR].AnimDamaged) == 0) { // RA2: the executable's form of this statement (0x459A00)
			strcpy(AnimData[BANIM_ACTIVE_FOUR].AnimDamaged, AnimData[BANIM_ACTIVE_FOUR].Anim); // RA2: the damaged name defaults to the plain one
		}
		ArtINI.Get_String(Graphic_Name(), "ActiveAnimFourGarrisoned", "", key, 16); // RA2: the executable reads "ActiveAnimFourGarrisoned" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_ACTIVE_FOUR].AnimGarrisoned, key); // RA2: as above
		}
		if (strlen(AnimData[BANIM_ACTIVE_FOUR].AnimGarrisoned) == 0) { // RA2: the executable's form of this statement (0x459A00)
			strcpy(AnimData[BANIM_ACTIVE_FOUR].AnimGarrisoned, AnimData[BANIM_ACTIVE_FOUR].Anim); // RA2: as above
		}
		if (strlen(AnimData[BANIM_ACTIVE_FOUR].Anim) != 0 || strlen(AnimData[BANIM_ACTIVE_FOUR].AnimDamaged) != 0 || strlen(AnimData[BANIM_ACTIVE_FOUR].AnimGarrisoned) != 0) { // RA2: the executable's form of this statement (0x459A00)
			AnimData[BANIM_ACTIVE_FOUR].Location.X = ArtINI.Get_Int(Graphic_Name(), "ActiveAnimFourX", AnimData[BANIM_ACTIVE_FOUR].Location.X);
			AnimData[BANIM_ACTIVE_FOUR].Location.Y = ArtINI.Get_Int(Graphic_Name(), "ActiveAnimFourY", AnimData[BANIM_ACTIVE_FOUR].Location.Y);
			AnimData[BANIM_ACTIVE_FOUR].ZAdjust = ArtINI.Get_Int(Graphic_Name(), "ActiveAnimFourZAdjust", AnimData[BANIM_ACTIVE_FOUR].ZAdjust);
			AnimData[BANIM_ACTIVE_FOUR].YSort = ArtINI.Get_Int(Graphic_Name(), "ActiveAnimFourYSort", AnimData[BANIM_ACTIVE_FOUR].YSort);
			AnimData[BANIM_ACTIVE_FOUR].Powered = ArtINI.Get_Bool(Graphic_Name(), "ActiveAnimFourPowered", AnimData[BANIM_ACTIVE_FOUR].Powered);
			AnimData[BANIM_ACTIVE_FOUR].PoweredLight = ArtINI.Get_Bool(Graphic_Name(), "ActiveAnimFourPoweredLight", AnimData[BANIM_ACTIVE_FOUR].PoweredLight);
		}
		ArtINI.Get_String(Graphic_Name(), "SuperAnim", "", key, 16); // RA2: the executable reads "SuperAnim" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_SUPER_ONE].Anim, key); // RA2: as above
		}
		ArtINI.Get_String(Graphic_Name(), "SuperAnimDamaged", "", key, 16); // RA2: the executable reads "SuperAnimDamaged" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_SUPER_ONE].AnimDamaged, key); // RA2: as above
		}
		if (strlen(AnimData[BANIM_SUPER_ONE].AnimDamaged) == 0) { // RA2: the executable's form of this statement (0x459A00)
			strcpy(AnimData[BANIM_SUPER_ONE].AnimDamaged, AnimData[BANIM_SUPER_ONE].Anim); // RA2: as above
		}
		ArtINI.Get_String(Graphic_Name(), "SuperAnimGarrisoned", "", key, 16); // RA2: the executable reads "SuperAnimGarrisoned" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_SUPER_ONE].AnimGarrisoned, key); // RA2: as above
		}
		if (strlen(AnimData[BANIM_SUPER_ONE].AnimGarrisoned) == 0) { // RA2: the executable's form of this statement (0x459A00)
			strcpy(AnimData[BANIM_SUPER_ONE].AnimGarrisoned, AnimData[BANIM_SUPER_ONE].Anim); // RA2: as above
		}
		if (strlen(AnimData[BANIM_SUPER_ONE].Anim) != 0 || strlen(AnimData[BANIM_SUPER_ONE].AnimDamaged) != 0 || strlen(AnimData[BANIM_SUPER_ONE].AnimGarrisoned) != 0) { // RA2: the executable's form of this statement (0x459A00)
			AnimData[BANIM_SUPER_ONE].Location.X = ArtINI.Get_Int(Graphic_Name(), "SuperAnimX", AnimData[BANIM_ACTIVE_FOUR].Location.X); // RA2: as above
			AnimData[BANIM_SUPER_ONE].Location.Y = ArtINI.Get_Int(Graphic_Name(), "SuperAnimY", AnimData[BANIM_ACTIVE_FOUR].Location.Y); // RA2: as above
			AnimData[BANIM_SUPER_ONE].ZAdjust = ArtINI.Get_Int(Graphic_Name(), "SuperAnimZAdjust", AnimData[BANIM_SUPER_ONE].ZAdjust); // RA2: as above
			AnimData[BANIM_SUPER_ONE].YSort = ArtINI.Get_Int(Graphic_Name(), "SuperAnimYSort", AnimData[BANIM_SUPER_ONE].YSort); // RA2: as above
			AnimData[BANIM_SUPER_ONE].Powered = ArtINI.Get_Bool(Graphic_Name(), "SuperAnimPowered", AnimData[BANIM_SUPER_ONE].Powered); // RA2: as above
			AnimData[BANIM_SUPER_ONE].PoweredLight = ArtINI.Get_Bool(Graphic_Name(), "SuperAnimPoweredLight", AnimData[BANIM_SUPER_ONE].PoweredLight); // RA2: as above
		}
		ArtINI.Get_String(Graphic_Name(), "SuperAnimTwo", "", key, 16); // RA2: the executable reads "SuperAnimTwo" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_SUPER_TWO].Anim, key); // RA2: as above
		}
		ArtINI.Get_String(Graphic_Name(), "SuperAnimTwoDamaged", "", key, 16); // RA2: the executable reads "SuperAnimTwoDamaged" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_SUPER_TWO].AnimDamaged, key); // RA2: as above
		}
		if (strlen(AnimData[BANIM_SUPER_TWO].AnimDamaged) == 0) { // RA2: the executable's form of this statement (0x459A00)
			strcpy(AnimData[BANIM_SUPER_TWO].AnimDamaged, AnimData[BANIM_SUPER_TWO].Anim); // RA2: as above
		}
		ArtINI.Get_String(Graphic_Name(), "SuperAnimTwoGarrisoned", "", key, 16); // RA2: the executable reads "SuperAnimTwoGarrisoned" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_SUPER_TWO].AnimGarrisoned, key); // RA2: as above
		}
		if (strlen(AnimData[BANIM_SUPER_TWO].AnimGarrisoned) == 0) { // RA2: the executable's form of this statement (0x459A00)
			strcpy(AnimData[BANIM_SUPER_TWO].AnimGarrisoned, AnimData[BANIM_SUPER_TWO].Anim); // RA2: as above
		}
		if (strlen(AnimData[BANIM_SUPER_TWO].Anim) != 0 || strlen(AnimData[BANIM_SUPER_TWO].AnimDamaged) != 0 || strlen(AnimData[BANIM_SUPER_TWO].AnimGarrisoned) != 0) { // RA2: the executable's form of this statement (0x459A00)
			AnimData[BANIM_SUPER_TWO].Location.X = ArtINI.Get_Int(Graphic_Name(), "SuperAnimTwoX", AnimData[BANIM_ACTIVE_TWO].Location.X); // RA2: as above
			AnimData[BANIM_SUPER_TWO].Location.Y = ArtINI.Get_Int(Graphic_Name(), "SuperAnimTwoY", AnimData[BANIM_ACTIVE_TWO].Location.Y); // RA2: as above
			AnimData[BANIM_SUPER_TWO].ZAdjust = ArtINI.Get_Int(Graphic_Name(), "SuperAnimTwoZAdjust", AnimData[BANIM_SUPER_TWO].ZAdjust); // RA2: as above
			AnimData[BANIM_SUPER_TWO].YSort = ArtINI.Get_Int(Graphic_Name(), "SuperAnimTwoYSort", AnimData[BANIM_SUPER_TWO].YSort); // RA2: as above
			AnimData[BANIM_SUPER_TWO].Powered = ArtINI.Get_Bool(Graphic_Name(), "SuperAnimTwoPowered", AnimData[BANIM_SUPER_TWO].Powered); // RA2: as above
			AnimData[BANIM_SUPER_TWO].PoweredLight = ArtINI.Get_Bool(Graphic_Name(), "SuperAnimTwoPoweredLight", AnimData[BANIM_SUPER_TWO].PoweredLight); // RA2: as above
		}
		ArtINI.Get_String(Graphic_Name(), "SuperAnimThree", "", key, 16); // RA2: the executable reads "SuperAnimThree" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_SUPER_THREE].Anim, key); // RA2: as above
		}
		ArtINI.Get_String(Graphic_Name(), "SuperAnimThreeDamaged", "", key, 16); // RA2: the executable reads "SuperAnimThreeDamaged" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_SUPER_THREE].AnimDamaged, key); // RA2: as above
		}
		if (strlen(AnimData[BANIM_SUPER_THREE].AnimDamaged) == 0) { // RA2: the executable's form of this statement (0x459A00)
			strcpy(AnimData[BANIM_SUPER_THREE].AnimDamaged, AnimData[BANIM_SUPER_THREE].Anim); // RA2: as above
		}
		ArtINI.Get_String(Graphic_Name(), "SuperAnimThreeGarrisoned", "", key, 16); // RA2: the executable reads "SuperAnimThreeGarrisoned" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_SUPER_THREE].AnimGarrisoned, key); // RA2: as above
		}
		if (strlen(AnimData[BANIM_SUPER_THREE].AnimGarrisoned) == 0) { // RA2: the executable's form of this statement (0x459A00)
			strcpy(AnimData[BANIM_SUPER_THREE].AnimGarrisoned, AnimData[BANIM_SUPER_THREE].Anim); // RA2: as above
		}
		if (strlen(AnimData[BANIM_SUPER_THREE].Anim) != 0 || strlen(AnimData[BANIM_SUPER_THREE].AnimDamaged) != 0 || strlen(AnimData[BANIM_SUPER_THREE].AnimGarrisoned) != 0) { // RA2: the executable's form of this statement (0x459A00)
			AnimData[BANIM_SUPER_THREE].Location.X = ArtINI.Get_Int(Graphic_Name(), "SuperAnimThreeX", AnimData[BANIM_ACTIVE_THREE].Location.X); // RA2: as above
			AnimData[BANIM_SUPER_THREE].Location.Y = ArtINI.Get_Int(Graphic_Name(), "SuperAnimThreeY", AnimData[BANIM_ACTIVE_THREE].Location.Y); // RA2: as above
			AnimData[BANIM_SUPER_THREE].ZAdjust = ArtINI.Get_Int(Graphic_Name(), "SuperAnimThreeZAdjust", AnimData[BANIM_SUPER_THREE].ZAdjust); // RA2: as above
			AnimData[BANIM_SUPER_THREE].YSort = ArtINI.Get_Int(Graphic_Name(), "SuperAnimThreeYSort", AnimData[BANIM_SUPER_THREE].YSort); // RA2: as above
			AnimData[BANIM_SUPER_THREE].Powered = ArtINI.Get_Bool(Graphic_Name(), "SuperAnimThreePowered", AnimData[BANIM_SUPER_THREE].Powered); // RA2: as above
			AnimData[BANIM_SUPER_THREE].PoweredLight = ArtINI.Get_Bool(Graphic_Name(), "SuperAnimThreePoweredLight", AnimData[BANIM_SUPER_THREE].PoweredLight); // RA2: as above
		}
		ArtINI.Get_String(Graphic_Name(), "SuperAnimFour", "", key, 16); // RA2: the executable reads "SuperAnimFour" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_SUPER_FOUR].Anim, key); // RA2: as above
		}
		ArtINI.Get_String(Graphic_Name(), "SuperAnimFourDamaged", "", key, 16); // RA2: the executable reads "SuperAnimFourDamaged" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_SUPER_FOUR].AnimDamaged, key); // RA2: as above
		}
		if (strlen(AnimData[BANIM_SUPER_FOUR].AnimDamaged) == 0) { // RA2: the executable's form of this statement (0x459A00)
			strcpy(AnimData[BANIM_SUPER_FOUR].AnimDamaged, AnimData[BANIM_SUPER_FOUR].Anim); // RA2: as above
		}
		ArtINI.Get_String(Graphic_Name(), "SuperAnimFourGarrisoned", "", key, 16); // RA2: the executable reads "SuperAnimFourGarrisoned" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_SUPER_FOUR].AnimGarrisoned, key); // RA2: as above
		}
		if (strlen(AnimData[BANIM_SUPER_FOUR].AnimGarrisoned) == 0) { // RA2: the executable's form of this statement (0x459A00)
			strcpy(AnimData[BANIM_SUPER_FOUR].AnimGarrisoned, AnimData[BANIM_SUPER_FOUR].Anim); // RA2: as above
		}
		if (strlen(AnimData[BANIM_SUPER_FOUR].Anim) != 0 || strlen(AnimData[BANIM_SUPER_FOUR].AnimDamaged) != 0 || strlen(AnimData[BANIM_SUPER_FOUR].AnimGarrisoned) != 0) { // RA2: the executable's form of this statement (0x459A00)
			AnimData[BANIM_SUPER_FOUR].Location.X = ArtINI.Get_Int(Graphic_Name(), "SuperAnimFourX", AnimData[BANIM_ACTIVE_THREE].Location.X); // RA2: as above
			AnimData[BANIM_SUPER_FOUR].Location.Y = ArtINI.Get_Int(Graphic_Name(), "SuperAnimFourY", AnimData[BANIM_ACTIVE_THREE].Location.Y); // RA2: as above
			AnimData[BANIM_SUPER_FOUR].ZAdjust = ArtINI.Get_Int(Graphic_Name(), "SuperAnimFourZAdjust", AnimData[BANIM_SUPER_FOUR].ZAdjust); // RA2: as above
			AnimData[BANIM_SUPER_FOUR].YSort = ArtINI.Get_Int(Graphic_Name(), "SuperAnimFourYSort", AnimData[BANIM_SUPER_FOUR].YSort); // RA2: as above
			AnimData[BANIM_SUPER_FOUR].Powered = ArtINI.Get_Bool(Graphic_Name(), "SuperAnimFourPowered", AnimData[BANIM_SUPER_FOUR].Powered); // RA2: as above
			AnimData[BANIM_SUPER_FOUR].PoweredLight = ArtINI.Get_Bool(Graphic_Name(), "SuperAnimFourPoweredLight", AnimData[BANIM_SUPER_FOUR].PoweredLight); // RA2: as above
		}
		ArtINI.Get_String(Graphic_Name(), "SpecialAnim", "", key, 16); // RA2: the executable reads "SpecialAnim" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_SPECIAL_ONE].Anim, key); // RA2: as above
		}
		ArtINI.Get_String(Graphic_Name(), "SpecialAnimDamaged", "", key, 16); // RA2: the executable reads "SpecialAnimDamaged" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_SPECIAL_ONE].AnimDamaged, key); // RA2: as above
		}
		if (strlen(AnimData[BANIM_SPECIAL_ONE].AnimDamaged) == 0) { // RA2: the executable's form of this statement (0x459A00)
			strcpy(AnimData[BANIM_SPECIAL_ONE].AnimDamaged, AnimData[BANIM_SPECIAL_ONE].Anim); // RA2: as above
		}
		ArtINI.Get_String(Graphic_Name(), "SpecialAnimGarrisoned", "", key, 16); // RA2: the executable reads "SpecialAnimGarrisoned" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_SPECIAL_ONE].AnimGarrisoned, key); // RA2: as above
		}
		if (strlen(AnimData[BANIM_SPECIAL_ONE].AnimGarrisoned) == 0) { // RA2: the executable's form of this statement (0x459A00)
			strcpy(AnimData[BANIM_SPECIAL_ONE].AnimGarrisoned, AnimData[BANIM_SPECIAL_ONE].Anim); // RA2: as above
		}
		if (strlen(AnimData[BANIM_SPECIAL_ONE].Anim) != 0 || strlen(AnimData[BANIM_SPECIAL_ONE].AnimDamaged) != 0 || strlen(AnimData[BANIM_SPECIAL_ONE].AnimGarrisoned) != 0) { // RA2: the executable's form of this statement (0x459A00)
			AnimData[BANIM_SPECIAL_ONE].Location.X = ArtINI.Get_Int(Graphic_Name(), "SpecialAnimX", AnimData[BANIM_SPECIAL_ONE].Location.X); // RA2: as above
			AnimData[BANIM_SPECIAL_ONE].Location.Y = ArtINI.Get_Int(Graphic_Name(), "SpecialAnimY", AnimData[BANIM_SPECIAL_ONE].Location.Y); // RA2: as above
			AnimData[BANIM_SPECIAL_ONE].ZAdjust = ArtINI.Get_Int(Graphic_Name(), "SpecialAnimZAdjust", AnimData[BANIM_SPECIAL_ONE].ZAdjust); // RA2: as above
			AnimData[BANIM_SPECIAL_ONE].YSort = ArtINI.Get_Int(Graphic_Name(), "SpecialAnimYSort", AnimData[BANIM_SPECIAL_ONE].YSort); // RA2: as above
			AnimData[BANIM_SPECIAL_ONE].Powered = ArtINI.Get_Bool(Graphic_Name(), "SpecialAnimPowered", AnimData[BANIM_SPECIAL_ONE].Powered); // RA2: as above
			AnimData[BANIM_SPECIAL_ONE].PoweredLight = ArtINI.Get_Bool(Graphic_Name(), "SpecialAnimPoweredLight", AnimData[BANIM_SPECIAL_ONE].PoweredLight); // RA2: as above
		}
		ArtINI.Get_String(Graphic_Name(), "SpecialAnimTwo", "", key, 16); // RA2: the executable reads "SpecialAnimTwo" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_SPECIAL_TWO].Anim, key); // RA2: as above
		}
		ArtINI.Get_String(Graphic_Name(), "SpecialAnimTwoDamaged", "", key, 16); // RA2: the executable reads "SpecialAnimTwoDamaged" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_SPECIAL_TWO].AnimDamaged, key); // RA2: as above
		}
		if (strlen(AnimData[BANIM_SPECIAL_TWO].AnimDamaged) == 0) { // RA2: the executable's form of this statement (0x459A00)
			strcpy(AnimData[BANIM_SPECIAL_TWO].AnimDamaged, AnimData[BANIM_SPECIAL_TWO].Anim); // RA2: the damaged name defaults to the plain one
		}
		ArtINI.Get_String(Graphic_Name(), "SpecialAnimTwoGarrisoned", "", key, 16); // RA2: the executable reads "SpecialAnimTwoGarrisoned" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_SPECIAL_TWO].AnimGarrisoned, key); // RA2: as above
		}
		if (strlen(AnimData[BANIM_SPECIAL_TWO].AnimGarrisoned) == 0) { // RA2: the executable's form of this statement (0x459A00)
			strcpy(AnimData[BANIM_SPECIAL_TWO].AnimGarrisoned, AnimData[BANIM_SPECIAL_TWO].Anim); // RA2: as above
		}
		if (strlen(AnimData[BANIM_SPECIAL_TWO].Anim) != 0 || strlen(AnimData[BANIM_SPECIAL_TWO].AnimDamaged) != 0 || strlen(AnimData[BANIM_SPECIAL_TWO].AnimGarrisoned) != 0) { // RA2: the executable's form of this statement (0x459A00)
			AnimData[BANIM_SPECIAL_TWO].Location.X = ArtINI.Get_Int(Graphic_Name(), "SpecialAnimTwoX", AnimData[BANIM_SPECIAL_TWO].Location.X); // RA2: as above
			AnimData[BANIM_SPECIAL_TWO].Location.Y = ArtINI.Get_Int(Graphic_Name(), "SpecialAnimTwoY", AnimData[BANIM_SPECIAL_TWO].Location.Y); // RA2: as above
			AnimData[BANIM_SPECIAL_TWO].ZAdjust = ArtINI.Get_Int(Graphic_Name(), "SpecialAnimTwoZAdjust", AnimData[BANIM_SPECIAL_TWO].ZAdjust); // RA2: as above
			AnimData[BANIM_SPECIAL_TWO].YSort = ArtINI.Get_Int(Graphic_Name(), "SpecialAnimTwoYSort", AnimData[BANIM_SPECIAL_TWO].YSort); // RA2: as above
			AnimData[BANIM_SPECIAL_TWO].Powered = ArtINI.Get_Bool(Graphic_Name(), "SpecialAnimTwoPowered", AnimData[BANIM_SPECIAL_TWO].Powered); // RA2: as above
			AnimData[BANIM_SPECIAL_TWO].PoweredLight = ArtINI.Get_Bool(Graphic_Name(), "SpecialAnimTwoPoweredLight", AnimData[BANIM_SPECIAL_TWO].PoweredLight); // RA2: as above
		}
		ArtINI.Get_String(Graphic_Name(), "SpecialAnimThree", "", key, 16); // RA2: the executable reads "SpecialAnimThree" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_SPECIAL_THREE].Anim, key); // RA2: as above
		}
		ArtINI.Get_String(Graphic_Name(), "SpecialAnimThreeDamaged", "", key, 16); // RA2: the executable reads "SpecialAnimThreeDamaged" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_SPECIAL_THREE].AnimDamaged, key); // RA2: as above
		}
		if (strlen(AnimData[BANIM_SPECIAL_THREE].AnimDamaged) == 0) { // RA2: the executable's form of this statement (0x459A00)
			strcpy(AnimData[BANIM_SPECIAL_THREE].AnimDamaged, AnimData[BANIM_SPECIAL_THREE].Anim); // RA2: the damaged name defaults to the plain one
		}
		ArtINI.Get_String(Graphic_Name(), "SpecialAnimThreeGarrisoned", "", key, 16); // RA2: the executable reads "SpecialAnimThreeGarrisoned" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_SPECIAL_THREE].AnimGarrisoned, key); // RA2: as above
		}
		if (strlen(AnimData[BANIM_SPECIAL_THREE].AnimGarrisoned) == 0) { // RA2: the executable's form of this statement (0x459A00)
			strcpy(AnimData[BANIM_SPECIAL_THREE].AnimGarrisoned, AnimData[BANIM_SPECIAL_THREE].Anim); // RA2: as above
		}
		if (strlen(AnimData[BANIM_SPECIAL_THREE].Anim) != 0 || strlen(AnimData[BANIM_SPECIAL_THREE].AnimDamaged) != 0 || strlen(AnimData[BANIM_SPECIAL_THREE].AnimGarrisoned) != 0) { // RA2: the executable's form of this statement (0x459A00)
			AnimData[BANIM_SPECIAL_THREE].Location.X = ArtINI.Get_Int(Graphic_Name(), "SpecialAnimThreeX", AnimData[BANIM_SPECIAL_THREE].Location.X); // RA2: as above
			AnimData[BANIM_SPECIAL_THREE].Location.Y = ArtINI.Get_Int(Graphic_Name(), "SpecialAnimThreeY", AnimData[BANIM_SPECIAL_THREE].Location.Y); // RA2: as above
			AnimData[BANIM_SPECIAL_THREE].ZAdjust = ArtINI.Get_Int(Graphic_Name(), "SpecialAnimThreeZAdjust", AnimData[BANIM_SPECIAL_THREE].ZAdjust); // RA2: as above
			AnimData[BANIM_SPECIAL_THREE].YSort = ArtINI.Get_Int(Graphic_Name(), "SpecialAnimThreeYSort", AnimData[BANIM_SPECIAL_THREE].YSort); // RA2: as above
			AnimData[BANIM_SPECIAL_THREE].Powered = ArtINI.Get_Bool(Graphic_Name(), "SpecialAnimThreePowered", AnimData[BANIM_SPECIAL_THREE].Powered); // RA2: as above
			AnimData[BANIM_SPECIAL_THREE].PoweredLight = ArtINI.Get_Bool(Graphic_Name(), "SpecialAnimThreePoweredLight", AnimData[BANIM_SPECIAL_THREE].PoweredLight); // RA2: as above
		}
		ArtINI.Get_String(Graphic_Name(), "ProductionAnim", "", key, 16); // RA2: the executable reads "ProductionAnim" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_PRODUCTION].Anim, key); // RA2: as above
		}
		ArtINI.Get_String(Graphic_Name(), "ProductionAnimDamaged", "", key, 16); // RA2: the executable reads "ProductionAnimDamaged" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_PRODUCTION].AnimDamaged, key); // RA2: as above
		}
		if (strlen(AnimData[BANIM_PRODUCTION].AnimDamaged) == 0) { // RA2: the executable's form of this statement (0x459A00)
			strcpy(AnimData[BANIM_PRODUCTION].AnimDamaged, AnimData[BANIM_PRODUCTION].Anim); // RA2: the damaged name defaults to the plain one
		}
		ArtINI.Get_String(Graphic_Name(), "ProductionAnimGarrisoned", "", key, 16); // RA2: the executable reads "ProductionAnimGarrisoned" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_PRODUCTION].AnimGarrisoned, key); // RA2: as above
		}
		if (strlen(AnimData[BANIM_PRODUCTION].AnimGarrisoned) == 0) { // RA2: the executable's form of this statement (0x459A00)
			strcpy(AnimData[BANIM_PRODUCTION].AnimGarrisoned, AnimData[BANIM_PRODUCTION].Anim); // RA2: as above
		}
		if (strlen(AnimData[BANIM_PRODUCTION].Anim) != 0 || strlen(AnimData[BANIM_PRODUCTION].AnimDamaged) != 0 || strlen(AnimData[BANIM_PRODUCTION].AnimGarrisoned) != 0) { // RA2: the executable's form of this statement (0x459A00)
			AnimData[BANIM_PRODUCTION].Location.X = ArtINI.Get_Int(Graphic_Name(), "ProductionAnimX", AnimData[BANIM_PRODUCTION].Location.X); // RA2: as above
			AnimData[BANIM_PRODUCTION].Location.Y = ArtINI.Get_Int(Graphic_Name(), "ProductionAnimY", AnimData[BANIM_PRODUCTION].Location.Y); // RA2: as above
			AnimData[BANIM_PRODUCTION].ZAdjust = ArtINI.Get_Int(Graphic_Name(), "ProductionAnimZAdjust", AnimData[BANIM_PRODUCTION].ZAdjust); // RA2: as above
			AnimData[BANIM_PRODUCTION].YSort = ArtINI.Get_Int(Graphic_Name(), "ProductionAnimYSort", AnimData[BANIM_PRODUCTION].YSort); // RA2: as above
		}
		ArtINI.Get_String(Graphic_Name(), "IdleAnim", "", key, 16); // RA2: the executable reads "IdleAnim" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_IDLE].Anim, key); // RA2: as above
		}
		ArtINI.Get_String(Graphic_Name(), "IdleAnimDamaged", "", key, 16); // RA2: the executable reads "IdleAnimDamaged" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_IDLE].AnimDamaged, key); // RA2: as above
		}
		if (strlen(AnimData[BANIM_IDLE].AnimDamaged) == 0) { // RA2: the executable's form of this statement (0x459A00)
			strcpy(AnimData[BANIM_IDLE].AnimDamaged, AnimData[BANIM_IDLE].Anim); // RA2: as above
		}
		ArtINI.Get_String(Graphic_Name(), "IdleAnimGarrisoned", "", key, 16); // RA2: the executable reads "IdleAnimGarrisoned" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_IDLE].AnimGarrisoned, key); // RA2: as above
		}
		if (strlen(AnimData[BANIM_IDLE].AnimGarrisoned) == 0) { // RA2: the executable's form of this statement (0x459A00)
			strcpy(AnimData[BANIM_IDLE].AnimGarrisoned, AnimData[BANIM_IDLE].Anim); // RA2: as above
		}
		if (strlen(AnimData[BANIM_IDLE].Anim) != 0 || strlen(AnimData[BANIM_IDLE].AnimDamaged) != 0 || strlen(AnimData[BANIM_IDLE].AnimGarrisoned) != 0) { // RA2: the executable's form of this statement (0x459A00)
			AnimData[BANIM_IDLE].Location.X = ArtINI.Get_Int(Graphic_Name(), "IdleAnimX", AnimData[BANIM_IDLE].Location.X); // RA2: as above
			AnimData[BANIM_IDLE].Location.Y = ArtINI.Get_Int(Graphic_Name(), "IdleAnimY", AnimData[BANIM_IDLE].Location.Y); // RA2: as above
			AnimData[BANIM_IDLE].ZAdjust = ArtINI.Get_Int(Graphic_Name(), "IdleAnimZAdjust", AnimData[BANIM_IDLE].ZAdjust); // RA2: as above
			AnimData[BANIM_IDLE].YSort = ArtINI.Get_Int(Graphic_Name(), "IdleAnimYSort", AnimData[BANIM_IDLE].YSort); // RA2: as above
		}
		ArtINI.Get_String(Graphic_Name(), "PreProductionAnim", "", key, 16); // RA2: the executable reads "PreProductionAnim" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_PRE_PRODUCTION].Anim, key); // RA2: as above
		}
		ArtINI.Get_String(Graphic_Name(), "PreProductionAnimDamaged", "", key, 16); // RA2: the executable reads "PreProductionAnimDamaged" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_PRE_PRODUCTION].AnimDamaged, key); // RA2: as above
		}
		if (strlen(AnimData[BANIM_PRE_PRODUCTION].AnimDamaged) == 0) { // RA2: the executable's form of this statement (0x459A00)
			strcpy(AnimData[BANIM_PRE_PRODUCTION].AnimDamaged, AnimData[BANIM_PRE_PRODUCTION].Anim); // RA2: the damaged name defaults to the plain one
		}
		ArtINI.Get_String(Graphic_Name(), "PreProductionAnimGarrisoned", "", key, 16); // RA2: the executable reads "PreProductionAnimGarrisoned" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_PRE_PRODUCTION].AnimGarrisoned, key); // RA2: as above
		}
		if (strlen(AnimData[BANIM_PRE_PRODUCTION].AnimGarrisoned) == 0) { // RA2: the executable's form of this statement (0x459A00)
			strcpy(AnimData[BANIM_PRE_PRODUCTION].AnimGarrisoned, AnimData[BANIM_PRE_PRODUCTION].Anim); // RA2: as above
		}
		if (strlen(AnimData[BANIM_PRE_PRODUCTION].Anim) != 0 || strlen(AnimData[BANIM_PRE_PRODUCTION].AnimDamaged) != 0 || strlen(AnimData[BANIM_PRE_PRODUCTION].AnimGarrisoned) != 0) { // RA2: the executable's form of this statement (0x459A00)
			AnimData[BANIM_PRE_PRODUCTION].Location.X = ArtINI.Get_Int(Graphic_Name(), "PreProductionAnimX", AnimData[BANIM_PRE_PRODUCTION].Location.X); // RA2: as above
			AnimData[BANIM_PRE_PRODUCTION].Location.Y = ArtINI.Get_Int(Graphic_Name(), "PreProductionAnimY", AnimData[BANIM_PRE_PRODUCTION].Location.Y); // RA2: as above
			AnimData[BANIM_PRE_PRODUCTION].ZAdjust = ArtINI.Get_Int(Graphic_Name(), "PreProductionAnimZAdjust", AnimData[BANIM_PRE_PRODUCTION].ZAdjust); // RA2: as above
			AnimData[BANIM_PRE_PRODUCTION].YSort = ArtINI.Get_Int(Graphic_Name(), "PreProductionAnimYSort", AnimData[BANIM_PRE_PRODUCTION].YSort); // RA2: as above
		}
		ini.Get_String(Name(), "TurretAnim", "", key, 16); // RA2: the executable reads "TurretAnim" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_TURRET].Anim, key); // RA2: as above
		}
		ini.Get_String(Name(), "TurretAnimDamaged", "", key, 16); // RA2: the executable reads "TurretAnimDamaged" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_TURRET].AnimDamaged, key); // RA2: as above
		}
		if (strlen(AnimData[BANIM_TURRET].AnimDamaged) == 0) { // RA2: the executable's form of this statement (0x459A00)
			strcpy(AnimData[BANIM_TURRET].AnimDamaged, AnimData[BANIM_TURRET].Anim); // RA2: the damaged name defaults to the plain one
		}
		ini.Get_String(Graphic_Name(), "TurretAnimGarrisoned", "", key, 16); // RA2: the executable reads "TurretAnimGarrisoned" at this point of 0x459A00
		if (strlen(key) != 0) { // RA2: as above
			strcpy(AnimData[BANIM_TURRET].AnimGarrisoned, key); // RA2: as above
		}
		if (strlen(AnimData[BANIM_TURRET].AnimGarrisoned) == 0) { // RA2: the executable's form of this statement (0x459A00)
			strcpy(AnimData[BANIM_TURRET].AnimGarrisoned, AnimData[BANIM_TURRET].Anim); // RA2: as above
		}
		AnimData[BANIM_TURRET].Location.X = ini.Get_Int(Name(), "TurretAnimX", AnimData[BANIM_TURRET].Location.X);
		AnimData[BANIM_TURRET].Location.Y = ini.Get_Int(Name(), "TurretAnimY", AnimData[BANIM_TURRET].Location.Y);
		AnimData[BANIM_TURRET].ZAdjust = ini.Get_Int(Name(), "TurretAnimZAdjust", AnimData[BANIM_TURRET].ZAdjust);
		AnimData[BANIM_TURRET].YSort = ini.Get_Int(Name(), "TurretAnimYSort", AnimData[BANIM_TURRET].YSort);
		IsTurretAnimAVoxel = ini.Get_Bool(Name(), "TurretAnimIsVoxel", IsTurretAnimAVoxel);
		IsBarrelAnimAVoxel = ini.Get_Bool(Name(), "BarrelAnimIsVoxel", IsBarrelAnimAVoxel); // RA2: read after TurretAnimIsVoxel, not at the head of the function
		ini.Get_String(Name(), "VoxelBarrelFile", VoxelBarrelFile, VoxelBarrelFile, 16); // RA2: the executable reads "VoxelBarrelFile" at this point of 0x459A00
		VoxelBarrelOffsetToPitchPivotPoint = ini.Get_Point(Name(), "VoxelBarrelOffsetToPitchPivotPoint", VoxelBarrelOffsetToPitchPivotPoint); // RA2: the voxel barrel offsets are read after the animation slots
		VoxelBarrelOffsetToRotatePivotPoint = ini.Get_Point(Name(), "VoxelBarrelOffsetToRotatePivotPoint", VoxelBarrelOffsetToRotatePivotPoint); // RA2: as above
		VoxelBarrelOffsetToBuildingPivotPoint = ini.Get_Point(Name(), "VoxelBarrelOffsetToBuildingPivotPoint", VoxelBarrelOffsetToBuildingPivotPoint); // RA2: as above
		VoxelBarrelOffsetToBarrelEnd = ini.Get_Point(Name(), "VoxelBarrelOffsetToBarrelEnd", VoxelBarrelOffsetToBarrelEnd); // RA2: as above
		Upgrades = ini.Get_Int(Name(), "Upgrades", Upgrades);
		if (Upgrades > 0) { // RA2: the executable's form of this statement (0x459A00)
			for (index = 0; index < Upgrades; index++) { // RA2: as above
				sprintf(anim_key, "PowerUp%01dAnim", index + 1); // RA2: as above
				sprintf(damaged_key, "PowerUp%01dDamagedAnim", index + 1); // RA2: as above
				sprintf(x_key, "PowerUp%01dLocXX", index + 1); // RA2: as above
				sprintf(y_key, "PowerUp%01dLocYY", index + 1); // RA2: as above
				sprintf(z_key, "PowerUp%01dLocZZ", index + 1); // RA2: as above
				sprintf(ysort_key, "PowerUp%01dYSort", index + 1); // RA2: as above
				ArtINI.Get_String(Graphic_Name(), anim_key, "", key, 16); // RA2: as above
				if (strlen(key) != 0) { // RA2: as above
					strcpy(AnimData[index].Anim, key); // RA2: as above
				}
				ArtINI.Get_String(Graphic_Name(), damaged_key, "", key, 16); // RA2: the executable's form of this statement (0x459A00)
				if (strlen(key) != 0) { // RA2: as above
					strcpy(AnimData[index].AnimDamaged, key); // RA2: as above
				}
				AnimData[index].Location.X = ArtINI.Get_Int(Graphic_Name(), x_key, AnimData[index].Location.X); // RA2: the executable's form of this statement (0x459A00)
				AnimData[index].Location.Y = ArtINI.Get_Int(Graphic_Name(), y_key, AnimData[index].Location.Y); // RA2: as above
				AnimData[index].ZAdjust = ArtINI.Get_Int(Graphic_Name(), z_key, AnimData[index].ZAdjust); // RA2: as above
				AnimData[index].YSort = ArtINI.Get_Int(Graphic_Name(), ysort_key, AnimData[index].YSort); // RA2: as above
			}
		}
		int docks = NumberOfDocks; // RA2: the executable's form of this statement (0x459A00)
		NumberOfDocks = ini.Get_Int(Name(), "NumberOfDocks", docks); // RA2: as above
		if (NumberOfDocks > DockingOffsets.Length()) { // RA2: as above
			DockingOffsets.Resize(NumberOfDocks); // RA2: as above
			for (index = docks; index < NumberOfDocks; index++) { // RA2: as above
				DockingOffsets[index] = Point3D(0, 0, 0); // RA2: as above
			}
		}
		for (index = 0; index < NumberOfDocks; index++) { // RA2: the executable's form of this statement (0x459A00)
			sprintf(dock_key, "DockingOffset%d", index); // RA2: as above
			Point3D offset = DockingOffsets[index]; // RA2: as above
			offset = ArtINI.Get_Point(Graphic_Name(), dock_key, offset); // RA2: as above
			DockingOffsets[index] = offset; // RA2: as above
		}
		return(true);
	}
	return(false);
}





/***********************************************************************************************
 * BuildingTypeClass::Coord_Fixup -- Adjusts coordinate to be legal for assignment.            *
 *                                                                                             *
 *    This routine will adjust the specified coordinate so that it will be legal for assignment*
 *    to this building. All buildings are given a coordinate that is in the upper left corner  *
 *    of a cell. This routine will drop the fractional component of the coordinate.            *
 *                                                                                             *
 * INPUT:   coord -- The coordinate to fixup into a legal to assign value.                     *
 *                                                                                             *
 * OUTPUT:  Returns with a coordinate that can be assigned to the building.                    *
 *                                                                                             *
 * WARNINGS:   The coordinate is not examined to see if the cell is legal for placing the      *
 *             building. It merely adjusts the coordinate so that is legal at first glance.    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/14/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
Coord const BuildingTypeClass::Coord_Fixup(Coord const & coord) const
{
	Coord crd = coord;
	crd.Z = Map.Get_Height_GL(crd);
	return(crd);
}


/// <summary>
/// Can this building legally be placed at the location specified?
/// A building type flagged as placeable anywhere bypasses the usual terrain and
/// adjacency restrictions.
/// </summary>
/// <param name="pos">The cell to test placement at.</param>
/// <param name="house">The house that wishes to place the building.</param>
/// <returns>bool; Is the placement legal?</returns>
bool BuildingTypeClass::Legal_Placement(Cell const & pos, HouseClass * house) const
{
	if (IsCanPlaceAnywhere) {
		return(true);
	}
	return(TechnoTypeClass::Legal_Placement(pos, house)); // RA2: Legal_Placement at 0x45D880 matches only so
}


/// <summary>
/// Fetches the dimensions of this building in leptons.
/// </summary>
/// <returns>Returns with the width, height and vertical extent of the building.</returns>
Point3D BuildingTypeClass::Lepton_Dimensions(void) const
{
	return(Point3D(SizeWidth[Size] << 8, SizeHeight[Size] << 8, ZHeight * LEVEL_LEPTON_H)); // RA2: Lepton_Dimensions at 0x45D8B0 matches only so
}


/// <summary>
/// Submits this building type to the CRC engine specified.
/// This routine is used by the multiplayer sync check to prove that every machine is
/// playing by an identical set of building rules.
/// </summary>
/// <param name="crc">The CRC engine to submit this object's data to.</param>
void BuildingTypeClass::Compute_CRC(CRCEngine & crc) const
{
	TechnoTypeClass::Compute_CRC(crc); // RA2: Compute_CRC at 0x45D8F0 matches only so
	crc(GateCloseDelay);
	crc(LightVisibility);
	crc(LightIntensity);
	crc(LightRedTint);
	crc(LightGreenTint);
	crc(LightBlueTint);
	crc((char const *)BuildupFilename);
	crc((char const *)PowersUpBuilding);
	crc(Adjacent);
	crc(ToBuild);
	crc(Power);
	crc(Drain);
	crc(ExtraPower); // RA2: as above
	crc(ExtraDrain); // RA2: as above
	crc(Size);
	crc(ZHeight);
	crc(OccupyHeight); // RA2: as above
	crc(MidPoint);
	crc(DoorStages);
	crc(&Anims, 72); // RA2: as above
	crc(&AnimData, sizeof(AnimData));
	crc(Upgrades);
	crc(SpecialZOverlayZAdjust);
	crc(IsHoverPad);
	crc(IsPlug);
	crc(IsTemple);
	crc(HasSpotlight);
	crc(IsBase);
	crc(EligibleForAllyBuilding); // RA2: as above
	crc(NeedsEngineer); // RA2: as above
	crc(IsBibbed);
	crc(IsWall);
	crc(IsCaptureable);
	crc(IsPowered);
	crc(IsUnsellable);
	crc(IsRadar);
	crc(SpySat); // RA2: as above
	crc(IsHasChargeAnim);
	crc(IsAnimDelayedFire); // RA2: as above
	crc(DelayedFireDelay); // RA2: as above
	crc(IsSiloDamage);
	crc(IsCanUnitRepair);
	crc(IsCanUnitReload);
	crc(Cloning); // RA2: as above
	crc(IsFlat);
	crc(IsDockUnload);
	crc(IsRecoilless);
	crc(IsBridgeRepairHut);
	crc(IsGate);
	crc(IsSAM);
	crc(IsConstructionYard);
	crc(IsNukeSilo);
	crc(IsRefinery);
	crc(IsWeaponsFactory);
	crc(IsLaserFencePost);
	crc(IsLaserFence);
	crc(IsFirestormWall);
	crc(IsHelipad);
	crc(OrePurifier); // RA2: as above
	crc(IsExtraDamageStage);
	crc(IsHospital);
	crc(IsArmory);
	crc(GateStages);
	crc(PowersUpToLevel);
	crc(IsDamagedDoor);
	crc(IsInvisibleInGame);
	crc(IsTerrainPalette);
	crc(IsTurretAnimAVoxel);
	crc(IsBarrelAnimAVoxel); // RA2: as above
	crc((char const *)VoxelBarrelFile); // RA2: as above
	crc(VoxelBarrelScale); // RA2: as above
	crc(&VoxelBarrelOffsetToPitchPivotPoint, 12); // RA2: as above
	crc(&VoxelBarrelOffsetToRotatePivotPoint, 12); // RA2: as above
	crc(&VoxelBarrelOffsetToBuildingPivotPoint, 12); // RA2: as above
	crc(&VoxelBarrelOffsetToBarrelEnd, 12); // RA2: as above
	crc(CrateBeneath); // RA2: as above
	crc(LeaveRubble); // RA2: as above
	crc(CrateBeneathIsMoney); // RA2: as above
	crc(NumberOfDocks); // RA2: as above
	crc(StartPitch); // RA2: as above
	crc(ConcentricRadialIndicator); // RA2: as above
}


/// <summary>
/// Loads this building type from the stream specified.
/// This routine will bring an object written by an older version of the game up to the
/// current layout, then reacquire the art and pointers that are never saved.
/// </summary>
/// <param name="stream">The stream to read this object from.</param>
/// <returns>Returns with S_OK if the object was read.</returns>
HRESULT STDMETHODCALLTYPE BuildingTypeClass::Load(IStream * stream) // RA2: Load at 0x45DD90 matches only so
{
	if (IsFreeBuildup && BuildupData != NULL) { // RA2: as above
		delete (ShapeSet *)BuildupData; // RA2: as above
		BuildupData = NULL; // RA2: as above
		BuildupLoaded = false; // RA2: as above
	}
	if (BibShapeLoaded && BibShape != NULL) { // RA2: as above
		delete (ShapeSet *)BibShape; // RA2: as above
		BibShape = NULL; // RA2: as above
		BibShapeLoaded = false; // RA2: as above
	}
	if (DeployingAnimLoaded && DeployingAnim != NULL) { // RA2: as above
		delete (ShapeSet *)DeployingAnim; // RA2: as above
		DeployingAnim = NULL; // RA2: as above
		DeployingAnimLoaded = false; // RA2: as above
	}
	if (UnderDoorAnimLoaded && UnderDoorAnim != NULL) { // RA2: as above
		delete (ShapeSet *)UnderDoorAnim; // RA2: as above
		UnderDoorAnim = NULL; // RA2: as above
		UnderDoorAnimLoaded = false; // RA2: as above
	}
	if (RoofDeployingAnimLoaded && RoofDeployingAnim != NULL) { // RA2: as above
		delete (ShapeSet *)RoofDeployingAnim; // RA2: as above
		RoofDeployingAnim = NULL; // RA2: as above
		RoofDeployingAnimLoaded = false; // RA2: as above
	}
	if (UnderRoofDoorAnimLoaded && UnderRoofDoorAnim != NULL) { // RA2: as above
		delete (ShapeSet *)UnderRoofDoorAnim; // RA2: as above
		UnderRoofDoorAnim = NULL; // RA2: as above
		UnderRoofDoorAnimLoaded = false; // RA2: as above
	}

	HRESULT result = TechnoTypeClass::Load(stream); // RA2: as above
	if (FAILED(result)) { // RA2: as above
		return(result); // RA2: as above
	}

	{
		new (this) BuildingTypeClass(NoInitClass()); // RA2: as above

		int count; // RA2: as above
		result = stream->Read(&count, sizeof(count), NULL); // RA2: as above
		if (FAILED(result)) { // RA2: as above
			return(result); // RA2: as above
		}

		new (&DockingOffsets) VectorClass<Point3D>(); // RA2: as above
		DockingOffsets.Resize(count, NULL); // RA2: as above
		for (int index = 0; index < count; index++) { // RA2: as above
			Point3D offset(0, 0, 0); // RA2: as above
			result = stream->Read(&offset, sizeof(offset), NULL); // RA2: as above
			if (FAILED(result)) { // RA2: as above
				return(result); // RA2: as above
			}
			DockingOffsets[index] = offset; // RA2: as above
		}
	}

		Fetch_Building_Voxel_Image();
		Fetch_Normal_Image();

		ToTile = NULL;
		OccupyList = OccupyLists[Size];
		ExitList = ExitLists[Size];

		Swizzle_Pointer(&ToOverlay);
		Swizzle_Pointer(&FreeUnit);

	Fetch_Building_Voxel_Image(); // RA2: as above

	BuildupData = NULL; // RA2: as above
	BuildupLoaded = false; // RA2: as above
	BibShape = NULL; // RA2: as above
	BibShapeLoaded = false; // RA2: as above
	DeployingAnim = NULL; // RA2: as above
	DeployingAnimLoaded = false; // RA2: as above
	UnderDoorAnim = NULL; // RA2: as above
	UnderDoorAnimLoaded = false; // RA2: as above
	RoofDeployingAnim = NULL; // RA2: as above
	RoofDeployingAnimLoaded = false; // RA2: as above
	UnderRoofDoorAnim = NULL; // RA2: as above
	UnderRoofDoorAnimLoaded = false; // RA2: as above

	Fetch_Building_Normal_Image(Scen->Theater); // RA2: as above
	return(S_OK); // RA2: as above
} // RA2: Load at 0x45DD90 has no return(result);


/// <summary>
/// Saves this building type to the stream specified.
/// </summary>
/// <param name="stream">The stream to write this object to.</param>
/// <param name="cleardirty">Should the dirty flag be cleared by this save?</param>
/// <returns>Returns with S_OK if the object was written.</returns>
HRESULT STDMETHODCALLTYPE BuildingTypeClass::Save(IStream * stream, BOOL cleardirty) // RA2: Save at 0x45E020 matches only so
{
	HRESULT result = TechnoTypeClass::Save(stream, cleardirty); // RA2: as above
	if (FAILED(result)) { // RA2: as above
		return(result); // RA2: as above
	}

	int count = DockingOffsets.Length(); // RA2: as above
	result = stream->Write(&count, sizeof(count), NULL); // RA2: as above
	if (FAILED(result)) { // RA2: as above
		return(result); // RA2: as above
	}

	for (int index = 0; index < count; index++) { // RA2: as above
		result = stream->Write(&DockingOffsets[index], sizeof(Point3D), NULL); // RA2: as above
		if (FAILED(result)) { // RA2: as above
			return(result); // RA2: as above
		}
	}
	return(S_OK); // RA2: as above
}


/// <summary>
/// Fetches the class identifier of this object.
/// This routine is used by the save game system so that it knows what kind of object to
/// construct when the stream is read back in.
/// </summary>
/// <param name="retval">Pointer to the class ID to fill in.</param>
/// <returns>Returns with S_OK, or E_POINTER if no destination was supplied.</returns>
HRESULT STDMETHODCALLTYPE BuildingTypeClass::GetClassID(CLSID * retval)
{
	if (retval == NULL) return(E_POINTER);
	*retval = CLSID_BuildingTypeClass;
	return(S_OK);
}


/// <summary>
/// Fetches the building type of the name specified, creating it if necessary.
/// </summary>
/// <param name="name">The name of the building type to find.</param>
/// <returns>Returns with a pointer to the building type, newly created if it did not
/// already exist.</returns>
BuildingTypeClass * BuildingTypeClass::Find_Or_Make(char const * name)
{
	return(TFind_Or_Make<BuildingTypeClass>(name, BuildingTypes));
}


/// <summary>
/// Performs the post load game fixups for the building types.
/// This routine sizes the shared cloaking surface so that it can serve the greediest
/// cloak radius any building asks for, and resolves the tile each building leaves behind.
/// </summary>
void BuildingTypeClass::Post_Load_Game(void)
{
	int maxcloak = 0;

	for (int sindex = 0; sindex < BuildingTypes.Count(); sindex++) {
		BuildingTypeClass * classptr = BuildingTypes[sindex];
		maxcloak = MAX(maxcloak, classptr->CloakRadiusInCells);
	}

	if (maxcloak != 0) {
		delete CloakingSurface;
		CloakingSurface = new BSurface(maxcloak + 16, maxcloak + 16, 1);
	}

	char buffer[60]; // RA2: Post_Load_Game at 0x45E170 matches only so
	for (int i = 0; i < BuildingTypes.Count(); i++) { // RA2: as above
		BuildingTypeClass * building = BuildingTypes[i]; // RA2: as above
		buffer[0] = '\0'; // RA2: as above
		RuleINI->Get_String(building->Name(), "ToTile", NULL, buffer, sizeof(buffer)); // RA2: as above
		IsometricTileType ittype = IsometricTileTypeClass::From_Name(buffer); // RA2: as above
		if (ittype >= ISOTILE_FIRST && ittype < IsometricTileTypes.Count()) { // RA2: as above
			building->ToTile = IsometricTileTypes[ittype]; // RA2: as above
		}
	}
}


/// <summary>
/// Fetches the list of cells that objects leave this building through.
/// </summary>
/// <returns>Returns with a pointer to the exit cell list that suits this building's
/// size.</returns>
Cell const * BuildingTypeClass::Exit_List(void) const
{
	BSizeType size = Size;
	if (size == BSIZE_33_REF) {
		size = BSIZE_33;
	}
	return(ExitLists[size]);
}


/// <summary>
/// Fetches the rectangle that this building draws within.
/// This routine is used by the render logic to know how much screen area the building can
/// cover. Every piece of its art is taken into account -- the normal shapes, the
/// construction animation and the bib.
/// </summary>
/// <returns>Returns with the draw rectangle, or RECT_NONE if the building has no art.</returns>
Rect BuildingTypeClass::Get_Draw_Rect(void)
{
	int i;

	if (DrawRect != RECT_NONE) {
		return(DrawRect);
	}

	ShapeSet const * shape = (ShapeSet const *)Get_Image_Data();
	if (shape == NULL) {
		return(RECT_NONE);
	}

	int frames = shape->Get_Count();
	Rect rect = RECT_NONE;
	int width = shape->Get_Width();
	int height = shape->Get_Height();

	for (i = 0; i < frames; i++) {
		rect = Union(rect, shape->Get_Rect(i));
	}

	bool free_buildup = (BuildupData == NULL);
	shape = (ShapeSet const *)Get_Buildup_Data(); // RA2: 0x45E290 reuses the one image pointer for the buildup shapes

	if (shape != NULL) { // RA2: as above
		frames = shape->Get_Count(); // RA2: as above
		for (i = 0; i < frames; i++) {
			rect = Union(rect, shape->Get_Rect(i)); // RA2: as above
		}
		if (free_buildup) {
			Free_Buildup_Data();
		}
	}

	if (BibShape != NULL) {
		Rect bibrect = BibShape->Get_Rect(0);
		rect = Union(rect, bibrect);
	}

	int x = rect.X - width / 2;
	int y = rect.Y - height / 2;
	Rect drawrect;
	drawrect.X = x;
	drawrect.Y = y;
	drawrect.Width = rect.Width;
	drawrect.Height = rect.Height;
	DrawRect = drawrect;

	return(DrawRect);
}


/// <summary>
/// A building cannot be given waypoints (RA2; ra2-decomp builtype.cpp).
/// </summary>
bool BuildingTypeClass::CanUseWaypoint(void) const // RA2: 0x45E740
{
	return(false);
}


/// <summary>
/// A building cannot be ordered to attack-move (RA2; ra2-decomp builtype.cpp).
/// </summary>
bool BuildingTypeClass::CanAttackMove(void) const // RA2: 0x45E750
{
	return(false);
}


/// <summary>
/// Fetches the largest power drain in the list of building types.
/// </summary>
/// <param name="list">The list of building types to examine.</param>
/// <returns>Returns with the greatest drain found, or zero if the list holds none.</returns>
int BuildingTypeClass::Get_Max_Drain(DynamicVectorClass<BuildingTypeClass *> const & list)
{
	int maxdrain = 0;
	for (int i = 0; i < list.Count(); i++) {
		if (list[i]->Drain > maxdrain) {
			maxdrain = list[i]->Drain;
		}
	}
	return(maxdrain);
}


/// <summary>
/// Fetches the construction animation shapes for this building.
/// This routine will load the buildup art the first time it is asked for if this building
/// type was flagged to defer loading, and it prepares the construction animation timing
/// to suit whatever art it finds.
/// </summary>
/// <returns>Returns with a pointer to the buildup shape data, or NULL if there is
/// none.</returns>
void const * BuildingTypeClass::Get_Buildup_Data(void) const
{
	char fullname[MAX_PATH];

	if (BuildupData == NULL && IsDemandLoadBuildup) {
		if (strlen(BuildupFilename) != 0) {
			_makepath(fullname, NULL, NULL, BuildupFilename, ".SHP");
			Theater_Name_Fixup(fullname, Scen->Theater); // RA2: the free function at 0x5D7BD0
			CCFileClass file(fullname);
			ShapeSet * data = (ShapeSet *)Load_Alloc_Data(file);
			if (data == NULL) { // RA2: 0x45E790 retries the generic theater name
				Generic_Theater_Name(fullname); // RA2: Get_Buildup_Data at 0x45E790 matches only so
				CCFileClass file2(fullname); // RA2: as above
				data = (ShapeSet *)Load_Alloc_Data(file2); // RA2: as above
			}
			((void const *&)BuildupData) = data;
			if (BuildupData != NULL) {
				int timedelay = 1;
				(bool &)BuildupLoaded = true; // RA2: as above, marks it loaded
				int count = ((ShapeSet const *)BuildupData)->Get_Count()/2;
				if (IsGate) {
					count = GateStages + 1;
				}
				if (count > 0) {
					timedelay = (int)(Rule->BuildupTime * 900.0 / count); // RA2: double math (0x45E790)
				}
				Init_Anim(BSTATE_CONSTRUCTION, 0, count, timedelay);
			}
		}
		return(BuildupData);
	}
	return(BuildupData);
}


/// <summary>
/// Frees the construction animation shapes for this building.
/// Only those building types that are flagged as worth discarding will actually give
/// their buildup art back.
/// </summary>
void BuildingTypeClass::Free_Buildup_Data(void)
{
	if (IsFreeBuildup && BuildupData != NULL) { // RA2: Free_Buildup_Data at 0x45E920 matches only so
		delete (ShapeSet *)BuildupData; // RA2: as above
		BuildupData = NULL; // RA2: as above
		BuildupLoaded = false; // RA2: as above
	}
}


/// <summary>
/// Releases this building's BibShape if it loaded it itself.
/// </summary>
void BuildingTypeClass::Free_Bib_Shape(void) // RA2: not in TibSun, at 0x45E960
{
	if (BibShapeLoaded && BibShape != NULL) {
		delete (ShapeSet *)BibShape;
		BibShape = NULL;
		BibShapeLoaded = false;
	}
}


/// <summary>
/// Releases this building's DeployingAnim if it loaded it itself.
/// </summary>
void BuildingTypeClass::Free_Deploying_Anim(void) // RA2: not in TibSun, at 0x45E9A0
{
	if (DeployingAnimLoaded && DeployingAnim != NULL) {
		delete (ShapeSet *)DeployingAnim;
		DeployingAnim = NULL;
		DeployingAnimLoaded = false;
	}
}


/// <summary>
/// Releases this building's UnderDoorAnim if it loaded it itself.
/// </summary>
void BuildingTypeClass::Free_Under_Door_Anim(void) // RA2: not in TibSun, at 0x45E9E0
{
	if (UnderDoorAnimLoaded && UnderDoorAnim != NULL) {
		delete (ShapeSet *)UnderDoorAnim;
		UnderDoorAnim = NULL;
		UnderDoorAnimLoaded = false;
	}
}


/// <summary>
/// Releases this building's RoofDeployingAnim if it loaded it itself.
/// </summary>
void BuildingTypeClass::Free_Roof_Deploying_Anim(void) // RA2: not in TibSun, at 0x45EA20
{
	if (RoofDeployingAnimLoaded && RoofDeployingAnim != NULL) {
		delete (ShapeSet *)RoofDeployingAnim;
		RoofDeployingAnim = NULL;
		RoofDeployingAnimLoaded = false;
	}
}


/// <summary>
/// Releases this building's UnderRoofDoorAnim if it loaded it itself.
/// </summary>
void BuildingTypeClass::Free_Under_Roof_Door_Anim(void) // RA2: not in TibSun, at 0x45EA60
{
	if (UnderRoofDoorAnimLoaded && UnderRoofDoorAnim != NULL) {
		delete (ShapeSet *)UnderRoofDoorAnim;
		UnderRoofDoorAnim = NULL;
		UnderRoofDoorAnimLoaded = false;
	}
}


/// <summary>
/// Calculates the threat values for this base defense.
/// This routine rates how dangerous the structure is to aircraft, armor and infantry so
/// that the computer can weigh its base defenses against the enemy it expects to face.
/// </summary>
static inline double Defense_Cap(double amount, double cap) // RA2: the cap compare 0x45EAA0 inlines (fcom, keep the smaller)
{
	return(cap < amount ? cap : amount);
}

void BuildingTypeClass::Calculate_Base_Defense_Values(void)
{
	if (IsBaseDefense) { // RA2: 0x45EAA0 rates the first weapon (TibSun's is empty)
		WeaponTypeClass const * weapon = Get_Weapon(0)->Weapon; // RA2: Calculate_Base_Defense_Values at 0x45EAA0 matches only so
		if (weapon != NULL) {
			int rate = weapon->Attack / (weapon->ROF * 0.025); // RA2: as above
			if (weapon->Bullet->IsAntiAircraft) {
				AntiAirValue = Defense_Cap(rate * weapon->WarheadPtr->Modifier[ARMOR_LIGHT], Rule->MaximumBaseDefenseValue); // RA2: as above
			}

			if (weapon->Bullet->IsAntiGround) {
				AntiArmorValue = Defense_Cap(rate * weapon->WarheadPtr->Modifier[ARMOR_MEDIUM], Rule->MaximumBaseDefenseValue); // RA2: as above
				AntiInfantryValue = Defense_Cap(rate * weapon->WarheadPtr->Modifier[ARMOR_NONE], Rule->MaximumBaseDefenseValue); // RA2: as above
			}
		}
	}
}


/// <summary>
/// Resolves the tile that each building type leaves behind.
/// This routine must wait until the isometric tile types have been loaded, since the
/// rule names a tile that does not exist before then.
/// </summary>
void BuildingTypeClass::Post_Read_Tile_Fixup(void)
{
	char buffer[60];

	for (int i = 0; i < BuildingTypes.Count(); i++) {
		BuildingTypeClass * building = BuildingTypes[i];
		buffer[0] = '\0';
		RuleINI->Get_String(building->Name(), "ToTile", NULL, buffer, sizeof(buffer));
		IsometricTileType ittype = IsometricTileTypeClass::From_Name(buffer);
		if (ittype >= ISOTILE_FIRST && ittype < IsometricTileTypes.Count()) {
			building->ToTile = IsometricTileTypes[ittype];
		}
	}
}


/// <summary>
/// Can this building undeploy regardless of circumstances?
/// This routine spots the building types that are allowed to pack themselves up even
/// where the usual undeploy restrictions would forbid it.
/// </summary>
/// <returns>bool; Is undeploying always permitted for this building?</returns>
bool BuildingTypeClass::Can_Always_Undeploy(void) const
{
	return(false); // RA2: 0x45EC40 (xor al,al; ret)
}


/// <summary>
/// Is this building the deployed form of a mobile unit?
/// </summary>
/// <returns>bool; Can this building undeploy back into a unit?</returns>
bool BuildingTypeClass::Is_Mobile_Deployer(void) const
{
	return(IsSensorArray || IsTickTank || IsICBMLauncher || IsArtillary); // RA2: Is_Mobile_Deployer at 0x45EC50 matches only so
}


/// <summary>
/// Fetches the facing this building takes when it deploys.
/// This routine is used when a mobile deployer converts between its unit and building
/// forms so that the two share a sensible orientation.
/// </summary>
/// <returns>Returns with the direction the building faces once deployed.</returns>
Dir256 BuildingTypeClass::Deploy_Facing(void) const
{
	if (!Is_Mobile_Deployer()) {
		return(DIR_S);
	}
	if (IsArtillary) {
		return(DIR_N);
	}
	if (IsSensorArray || IsTickTank || IsICBMLauncher) {
		return(DIR_E);
	}
	return(DIR_S);
}


/// <summary>
/// Fetches the RTTI type of this object.
/// </summary>
/// <returns>Returns with RTTI_BUILDINGTYPE.</returns>
RTTIType BuildingTypeClass::Fetch_RTTI(void) const
{
	return(RTTI_BUILDINGTYPE);
}


/// <summary>
/// Fetches the size of this object for save game purposes.
/// </summary>
/// <param name="oldsave">Is the size wanted for an old format save game?</param>
/// <returns>Returns with the number of bytes this object occupies in the save file.</returns>
int BuildingTypeClass::Fetch_Object_Size(void) const // RA2: no argument (as hover.h), at 0x0045ED10
{
	return(sizeof(*this));
}


/// <summary>
/// Fetches the heap identifier of this building type.
/// </summary>
/// <returns>Returns with the index of this type within the building type heap.</returns>
int BuildingTypeClass::Fetch_Heap_ID(void) const
{
	return(HeapID);
}
