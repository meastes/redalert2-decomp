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

/* $Header: /CounterStrike/RULES.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : RULES.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 05/12/96                                                     *
 *                                                                                             *
 *                  Last Update : September 10, 1996 [JLB]                                     *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Difficulty_Get -- Fetch the difficulty bias values.                                       *
 *   RulesClass::AI -- Processes the AI control constants from the database.                   *
 *   RulesClass::General -- Process the general main game rules.                               *
 *   RulesClass::Heap_Maximums -- Fetch and process the heap override values.                  *
 *   RulesClass::IQ -- Fetches the IQ control values from the INI database.                    *
 *   RulesClass::Land_Types -- Inits the land type values.                                     *
 *   RulesClass::MPlayer -- Fetch and process the multiplayer default settings.                *
 *   RulesClass::Powerups -- Process the powerup values from the database.                     *
 *   RulesClass::Process -- Fetch the bulk of the rule data from the control file.             *
 *   RulesClass::Recharge -- Process the super weapon recharge statistics.                     *
 *   RulesClass::RulesClass -- Default constructor for rules class object.                     *
 *   RulesClass::Themes -- Fetches the theme control values from the INI database.             *
 *   Techno_Get -- Get rule data common for all techno type objects.                           *
 *   _Scale_To_256 -- Scales a 1..100 number into a 1..255 number.                             *
 *   RulesClass::Difficulty -- Fetch the various difficulty group settings.                    *
 *   RulesClass::Objects -- Fetch all the object characteristic values.                        *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "always.h"
#include "geometry.h" // RA2: the header statics, in the order of the unit's initializer table
#include "rect.h"

#include "rules.h"
#include "tab.h" // RA2: the advanced command bar

#include "_bench.h"
#include "_palette.h"
#include "_rules.h"
#include "_warhead.h"
#include "_weapon.h"
#include "addon.h"
#include "airctype.h"
#include "anim.h"
#include "animtype.h"
#include "bench.h"
#include "builtype.h"
#include "bullettype.h"
#include "ccfile.h"
#include "conquer.h"
#include "convert.h"
#include "mixfile.h" // RA2: MixFileClass for the bomb cursor shape
#include "dbgprint.h"
#include "findmake.h"
#include "globals.h"
#include "house.h"
#include "houstype.h"
#include "infatype.h"
#include "levitate.h"
#include "mission.h"
#include "movie.h"
#include "multiplayer.h"
#include "overtype.h"
#include "psystype.h"
#include "ptype.h"
#include "scheme.h"
#include "script.h"
#include "session.h" // RA2: Addition reads Session.Type
#include "side.h"
#include "smudtype.h"
#include "stimer.h"
#include "sun.h"
#include "suprtype.h"
#include "swizzle.h"
#include "taskforc.h"
#include "teamtype.h"
#include "terrtype.h"
#include "tiberium.h"
#include "trim.h"
#include "unittype.h"
#include "vanimtype.h"
#include "vector.h"
#include "warhead.h"
#include "weapon.h"

#include "bench.hh"



#include <cstring>


void Difficulty_Get(CCINIClass const & ini, DifficultyClass & diff, char const * section);
void __fastcall SchemeLibrary_Add(char const * name, HSVClass const & color); // RA2: records a colour scheme in the scheme library, 0x603A60 (YR: SchemeLibrary_Add)
void SchemeLibrary_Free(void); // RA2: empties the scheme library, 0x603950 (YR: SchemeLibrary_Free)

/// <summary>
/// Constructor for a rules object that is about to be overwritten.
/// The load process reads the rule data straight off the save game stream, so this
/// constructor deliberately leaves every value alone rather than paying for defaults that
/// would only be thrown away.
/// </summary>
RulesClass::RulesClass(NoInitClass const & x) :
	RadColor(x), // RA2: the load constructor builds only the colour members that have a NoInit form
	LineTrailColorOverride(x), // RA2: as above
	ChronoBeamColor(x) // RA2: as above
{
}


/***********************************************************************************************
 * RulesClass::RulesClass -- Default constructor for rules class object.                       *
 *                                                                                             *
 *    This is the default constructor for the rules class object. Although it initializes the  *
 *    rule data with default values, it is expected that they will all be overridden by the    *
 *    rules control file.                                                                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/17/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: 1477 bytes differ (adopted text)
RulesClass::RulesClass(void) :
	DetailMinFrameRateNormal(15), // RA2: the executable's initializer list, in RA2's member order and values
	DetailMinFrameRateMovie(20), // RA2: as above
	DetailBufferZoneWidth(5), // RA2: as above
	AmmoCrateDamage(100),
	LargeVisceroid(0), // RA2: the executable's initializer list, in RA2's member order and values
	SmallVisceroid(0), // RA2: as above
	UnloadingHarvester(0), // RA2: as above
	AttackingAircraftSightRange(5),
	TunnelSpeed(1.0), // RA2: the executable's initializer list, in RA2's member order and values
	TiberiumHeal(0.016666666666666666), // RA2: as above
	IsFreeMCV(false),
	IsBerzerkAllowed(false),
	PoseDir(DIR_N),
	DropPodPuff(0), // RA2: the executable's initializer list, in RA2's member order and values
	WaypointAnimationSpeed(12),
	BarrelExplode(0), // RA2: the executable's initializer list, in RA2's member order and values
	BarrelParticle(0), // RA2: as above
	RadarEventColorSpeed(0.05f), // RA2: as above
	RadarEventMinRadius(5),
	RadarEventSpeed(1.0f), // RA2: the executable's initializer list, in RA2's member order and values
	RadarEventRotationSpeed(0.1f), // RA2: as above
	FlashFrameTime(7),
	RadarCombatFlashTime(21),
	MaxWaypointPathLength(15),
	Wake(0), // RA2: the executable's initializer list, in RA2's member order and values
	NukeTakeOff(0), // RA2: as above
	InfantryExplode(0), // RA2: as above
	FlamingInfantry(0), // RA2: as above
	InfantryHeadPop(0), // RA2: as above
	InfantryNuked(0), // RA2: as above
	Behind(0), // RA2: as above
	AITriggerSuccessWeightDelta(1.0), // RA2: as above
	AITriggerFailureWeightDelta(-1.0), // RA2: as above
	AITriggerTrackRecordCoefficient(1.0), // RA2: as above
	VeinholeMonsterStrength(10000),
	MaxVeinholeGrowth(1000),
	VeinholeGrowthRate(100),
	VeinholeShrinkRate(100),
	VeinAttack(0), // RA2: the executable's initializer list, in RA2's member order and values
	VeinDamage(2),
	MaximumQueuedObjects(5),
	AircraftFogReveal(6),
	WoodCrateImg(0), // RA2: the executable's initializer list, in RA2's member order and values
	CrateImg(0), // RA2: as above
	WaterCrateImg(0), // RA2: as above
	DigSound(VOC_NONE),
	CreateUnitSound(-1), // RA2: the executable's initializer list, in RA2's member order and values
	CreateInfantrySound(-1), // RA2: as above
	CreateAircraftSound(-1), // RA2: as above
	BaseUnderAttackSound(-1), // RA2: as above
	GUIMainButtonSound(-1), // RA2: as above
	GUIBuildSound(-1), // RA2: as above
	GUITabSound(-1), // RA2: as above
	GUIOpenSound(-1), // RA2: as above
	GUICloseSound(-1), // RA2: as above
	GUIMoveOutSound(-1), // RA2: as above
	GUIMoveInSound(-1), // RA2: as above
	GUIComboOpenSound(-1), // RA2: as above
	GUIComboCloseSound(-1), // RA2: as above
	GUICheckboxSound(-1), // RA2: as above
	ScoreAnimSound(-1), // RA2: as above
	IFVTransformSound(-1), // RA2: as above
	PsychicSensorDetectSound(-1), // RA2: as above
	BuildingGarrisonedSound(-1), // RA2: as above
	BuildingAbandonedSound(-1), // RA2: as above
	BuildingRepairedSound(-1), // RA2: as above
	CheerSound(-1), // RA2: as above
	PlaceBeaconSound(-1), // RA2: as above
	DefaultChronoSound(-1), // RA2: as above
	StartPlanningModeSound(-1), // RA2: as above
	AddPlanningModeCommandSound(-1), // RA2: as above
	ExecutePlanSound(-1), // RA2: as above
	EndPlanningModeSound(-1), // RA2: as above
	CrateMoneySound(-1), // RA2: as above
	CrateRevealSound(-1), // RA2: as above
	CrateFireSound(-1), // RA2: as above
	CrateArmourSound(-1), // RA2: as above
	CrateSpeedSound(-1), // RA2: as above
	CrateUnitSound(-1), // RA2: as above
	CratePromoteSound(-1), // RA2: as above
	ImpactWaterSound(-1), // RA2: as above
	ImpactLandSound(-1), // RA2: as above
	SinkingSound(-1), // RA2: as above
	BombTickingSound(-1), // RA2: as above
	BombAttachSound(-1), // RA2: as above
	ChronoInSound(-1), // RA2: as above
	ChronoOutSound(-1), // RA2: as above
	SpySatActivationSound(-1), // RA2: as above
	SpySatDeactivationSound(-1), // RA2: as above
	UpgradeVeteranSound(-1), // RA2: as above
	UpgradeEliteSound(-1), // RA2: as above
	Dig(0), // RA2: as above
	IonBlast(0), // RA2: as above
	IonBeam(0), // RA2: as above
	WeatherConBoltExplosion(0), // RA2: as above
	ChronoBlast(0), // RA2: as above
	ChronoBlastDest(0), // RA2: as above
	ChronoPlacement(0), // RA2: as above
	ChronoBeam(0), // RA2: as above
	WarpIn(0), // RA2: as above
	WarpOut(0), // RA2: as above
	WarpAway(0), // RA2: as above
	ChronoSparkle1(0), // RA2: as above
	IronCurtainInvokeAnim(0), // RA2: as above
	WeaponNullifyAnim(0), // RA2: as above
	AtmosphereEntry(0), // RA2: as above
	GateUpSound(VOC_NONE),
	GateDownSound(VOC_NONE),
	JumpjetTurnRate(3),
	JumpjetSpeed(30),
	JumpjetClimb(5.0), // RA2: the executable's initializer list, in RA2's member order and values
	JumpjetCruiseHeight(400),
	JumpjetAcceleration(0.25), // RA2: the executable's initializer list, in RA2's member order and values
	JumpjetWobblesPerSecond(0.25), // RA2: as above
	JumpjetWobbleDeviation(40),
	IonCannonDamage(700),
	RailgunDamageRadius(128), // RA2: the executable's initializer list, in RA2's member order and values
	PrismType(0), // RA2: as above
	PrismSupportModifier(100), // RA2: as above
	PrismSupportMax(8), // RA2: as above
	PrismSupportDelay(100), // RA2: as above
	PrismSupportDuration(15), // RA2: as above
	PrismSupportHeight(300), // RA2: as above
	ParadropRadius(1024), // RA2: as above
	ZoomInFactor(2.0), // RA2: as above
	ConditionRedSparkingProbability(0.02), // RA2: as above
	ConditionYellowSparkingProbability(0.01), // RA2: as above
	TiberiumExplosionDamage(100),
	TiberiumStrength(10),
	MinLowPowerProductionSpeed(0.5f), // RA2: the executable's initializer list, in RA2's member order and values
	MaxLowPowerProductionSpeed(0.9f), // RA2: as above
	LowPowerPenaltyModifier(1.0f), // RA2: as above
	MultipleFactory(1.0f), // RA2: as above
	MaximumCheerRate(1), // RA2: as above
	TreeFlammability(0.1), // RA2: as above
	MissileSpeedVar(0.25), // RA2: as above
	MissileROTVar(0.25), // RA2: as above
	MissileSafetyAltitude(500), // RA2: as above
	DropPodWeapon(0), // RA2: as above
	DropPodHeight(1500),
	DropPodSpeed(40),
	DropPodAngle(1.5707963267948966), // RA2: the executable's initializer list, in RA2's member order and values
	ScrollMultiplier(1.0), // RA2: as above
	CrewEscape(0.5), // RA2: as above
	ShakeScreen(400),
	HoverHeight(120),
	HoverBob(30.0), // RA2: the executable's initializer list, in RA2's member order and values
	HoverBoost(1.3),
	HoverAcceleration(0.03), // RA2: the executable's initializer list, in RA2's member order and values
	HoverBrake(0.03), // RA2: as above
	HoverDampen(0.8), // RA2: as above
	PlacementDelay(0.05), // RA2: as above
	TireVoxelDebris(0), // RA2: as above
	ScrapVoxelDebris(0), // RA2: as above
	BridgeVoxelMax(3),
	CloakingStages(9),
	RevealTriggerRadius(5),
	ShipSinkingWeight(3.0), // RA2: the executable's initializer list, in RA2's member order and values
	IceCrackingWeight(2.0), // RA2: as above
	IceBreakingWeight(4.0), // RA2: as above
	CliffBackImpassability((char)0), // RA2: as above
	VeteranRatio(10.0), // RA2: as above
	VeteranCombat(1.0), // RA2: as above
	VeteranSpeed(1.0), // RA2: as above
	VeteranSight(1.0), // RA2: as above
	VeteranArmor(1.0), // RA2: as above
	VeteranROF(1.0), // RA2: as above
	VeteranCap(1.0), // RA2: as above
	CloakSound(VOC_NONE),
	SellSound(VOC_NONE),
	GameClosed(VOC_NONE),
	IncomingMessage(VOC_NONE),
	SystemError(VOC_NONE),
	OptionsChanged(VOC_NONE),
	GameForming(VOC_NONE),
	PlayerLeft(VOC_NONE),
	PlayerJoined(VOC_NONE),
	MessageCharTyped(-1), // RA2: the executable's initializer list, in RA2's member order and values
	Construction(VOC_NONE),
	BuildingDieSound(-1), // RA2: the executable's initializer list, in RA2's member order and values
	BuildingSlam(VOC_NONE),
	RadarOn(VOC_NONE),
	RadarOff(VOC_NONE),
	MovieOn(-1), // RA2: the executable's initializer list, in RA2's member order and values
	MovieOff(-1), // RA2: as above
	ScoldSound(VOC_NONE),
	TeslaCharge(VOC_NONE),
	TeslaZap(VOC_NONE),
	GenericClick(VOC_NONE),
	GenericBeep(VOC_NONE),
	BuildingDamageSound(-1), // RA2: the executable's initializer list, in RA2's member order and values
	HealCrateSound(VOC_NONE),
	ChuteSound(VOC_NONE),
	StopSound(VOC_NONE),
	GuardSound(VOC_NONE),
	ScatterSound(VOC_NONE),
	DeploySound(VOC_NONE),
	StormSound(-1), // RA2: the executable's initializer list, in RA2's member order and values
	ShellButtonSlideSound(-1), // RA2: as above
	WallBuildSpeedCoefficient(0.5), // RA2: as above
	ChargeToDrainRatio(3.0), // RA2: as above
	DamageToFirestormDamageCoefficient(0.1), // RA2: as above
	TrackedUphill(1.0), // RA2: as above
	TrackedDownhill(1.0), // RA2: as above
	WheeledUphill(1.0), // RA2: as above
	WheeledDownhill(1.0), // RA2: as above
	SpotlightMovementRadius(2000),
	SpotlightLocationRadius(1000),
	SpotlightSpeed(0.05), // RA2: the executable's initializer list, in RA2's member order and values
	SpotlightAcceleration(0.005), // RA2: as above
	SpotlightAngle(20.0), // RA2: as above
	SpotlightRadius(175),
	WindDirection(FACING_NONE),
	CameraRange(2304), // RA2: the executable's initializer list, in RA2's member order and values
	FlightLevel(500),
	ParachuteMaxFallRate(-3), // RA2: the executable's initializer list, in RA2's member order and values
	NoParachuteMaxFallRate(-100), // RA2: as above
	BuildingDrop(VOC_NONE),
	RepairBay(0), // RA2: the executable's initializer list, in RA2's member order and values
	GDIGateOne(0), // RA2: as above
	GDIGateTwo(0), // RA2: as above
	NodGateOne(0), // RA2: as above
	NodGateTwo(0), // RA2: as above
	WallTower(0), // RA2: as above
	GDIPowerPlant(0), // RA2: as above
	GDIPowerTurbine(0), // RA2: as above
	NodRegularPower(0), // RA2: as above
	NodAdvancedPower(0), // RA2: as above
	GDIFirestormGenerator(0), // RA2: as above
	GDIHunterSeeker(0), // RA2: as above
	NodHunterSeeker(0), // RA2: as above
	GDIWallDefense(6.0), // RA2: as above
	GDIWallDefenseCoefficient(3.0), // RA2: as above
	NodBaseDefenseCoefficient(1.0), // RA2: as above
	GDIBaseDefenseCoefficient(1.0), // RA2: as above
	ComputerBaseDefenseResponse(3),
	MaximumBaseDefenseValue(60),
	Smoke1(0), // RA2: the executable's initializer list, in RA2's member order and values
	Smoke2(0), // RA2: as above
	FirestormActiveAnim(0), // RA2: as above
	FirestormIdleAnim(0), // RA2: as above
	FirestormAirAnim(0), // RA2: as above
	FirestormGroundAnim(0), // RA2: as above
	MoveFlash(0), // RA2: as above
	BombParachute(0), // RA2: as above
	Parachute(0), // RA2: as above
	SmallFire(0), // RA2: as above
	LargeFire(0), // RA2: as above
	Paratrooper(0), // RA2: as above
	EliteFlashTimer(100), // RA2: as above
	ChronoDelay(60), // RA2: as above
	ChronoReinfDelay(60), // RA2: as above
	ChronoDistanceFactor(32), // RA2: as above
	ChronoTrigger(true), // RA2: as above
	ChronoMinimumDelay(0), // RA2: as above
	ChronoRangeMinimum(0), // RA2: as above
	AlliedDisguise(0), // RA2: as above
	SovietDisguise(0), // RA2: as above
	SpyPowerBlackout(0), // RA2: as above
	SpyMoneyStealPercent(0.5f), // RA2: as above
	AttackCursorOnDisguise(false), // RA2: as above
	AIMinorSuperReadyPercent(0.8f), // RA2: as above
	AISafeDistance(8), // RA2: as above
	HarvesterTooFarDistance(5), // RA2: as above
	ChronoHarvTooFarDistance(50), // RA2: as above
	AIRestrictReplaceTime(500), // RA2: as above
	ThreatPerOccupant(5), // RA2: as above
	ApproachTargetResetMultiplier(1), // RA2: as above
	CampaignMoneyDeltaEasy(5000), // RA2: as above
	CampaignMoneyDeltaHard(-5000), // RA2: as above
	GuardAreaTargetingDelay(36), // RA2: as above
	NormalTargetingDelay(27), // RA2: as above
	AINavalYardAdjacency(20), // RA2: as above
	MaximumBuildingPlacementFailures(5), // RA2: as above
	PurifierBonus(0.25f), // RA2: as above
	Technician(0), // RA2: as above
	Engineer(0), // RA2: as above
	Pilot(0), // RA2: as above
	AlliedCrew(0), // RA2: as above
	SovietCrew(0), // RA2: as above
	FlameDamage(0), // RA2: as above
	FlameDamage2(0), // RA2: as above
	NukeWarhead(0), // RA2: as above
	NukeProjectile(0), // RA2: as above
	NukeDown(0), // RA2: as above
	EMPulseWarhead(0), // RA2: as above
	EMPulseProjectile(0), // RA2: as above
	C4Warhead(0), // RA2: as above
	V3Warhead(0), // RA2: as above
	DMislWarhead(0), // RA2: as above
	V3EliteWarhead(0), // RA2: as above
	DMislEliteWarhead(0), // RA2: as above
	IvanWarhead(0), // RA2: as above
	IvanDamage(100), // RA2: as above
	IvanTimedDelay(450), // RA2: as above
	CanDetonateTimeBomb(false), // RA2: as above
	CanDetonateDeathBomb(false), // RA2: as above
	IvanIconFlickerRate(8), // RA2: as above
	DeathWeapon(0), // RA2: as above
	BOMBCURS_SHP(0), // RA2: as above
	IronCurtainDuration(0), // RA2: as above
	IonCannonWarhead(0), // RA2: as above
	FirestormWarhead(0), // RA2: as above
	VeinholeWarhead(0), // RA2: as above
	VeinholeTypeClass(0), // RA2: as above
	InfantryBlinkDisguiseTime(0), // RA2: as above
	DefaultLargeGreySmokeSystem(0), // RA2: as above
	DefaultSmallGreySmokeSystem(0), // RA2: as above
	DefaultSparkSystem(0), // RA2: as above
	DefaultLargeRedSmokeSystem(0), // RA2: as above
	DefaultSmallRedSmokeSystem(0), // RA2: as above
	DefaultDebrisSmokeSystem(0), // RA2: as above
	DefaultFireStreamSystem(0), // RA2: as above
	DefaultFirestormExplosionSystem(0), // RA2: as above
	DefaultTestParticleSystem(0), // RA2: as above
	DefaultRepairParticleSystem(0), // RA2: as above
	MyEffectivenessCoefficientDefault(0.0), // RA2: as above
	TargetEffectivenessCoefficientDefault(0.0), // RA2: as above
	TargetSpecialThreatCoefficientDefault(0.0), // RA2: as above
	TargetStrengthCoefficientDefault(0.0), // RA2: as above
	TargetDistanceCoefficientDefault(0.0), // RA2: as above
	DumbMyEffectivenessCoefficient(0.0), // RA2: as above
	DumbTargetEffectivenessCoefficient(0.0), // RA2: as above
	DumbTargetSpecialThreatCoefficient(0.0), // RA2: as above
	DumbTargetStrengthCoefficient(0.0), // RA2: as above
	DumbTargetDistanceCoefficient(0.0), // RA2: as above
	EnemyHouseThreatBonus(0.0), // RA2: as above
	HunterSeekerDetonateProximity(0),
	HunterSeekerDescendProximity(0),
	HunterSeekerDescentSpeed(0),
	HunterSeekerAscentSpeed(0),
	HunterSeekerEmergeSpeed(0), // RA2: the executable's initializer list, in RA2's member order and values
	TurboBoost(1.5),
	AttackInterval(3.0), // RA2: the executable's initializer list, in RA2's member order and values
	AttackDelay(5.0), // RA2: as above
	PowerEmergencyFraction(0.75), // RA2: as above
	AirstripRatio(0.12), // RA2: as above
	AirstripLimit(5),
	HelipadRatio(0.12), // RA2: the executable's initializer list, in RA2's member order and values
	HelipadLimit(5),
	TeslaRatio(0.16), // RA2: the executable's initializer list, in RA2's member order and values
	TeslaLimit(10),
	AARatio(0.14), // RA2: the executable's initializer list, in RA2's member order and values
	AALimit(10),
	DefenseRatio(0.5), // RA2: the executable's initializer list, in RA2's member order and values
	DefenseLimit(40),
	WarRatio(0.1), // RA2: the executable's initializer list, in RA2's member order and values
	WarLimit(2),
	BarracksRatio(0.16), // RA2: the executable's initializer list, in RA2's member order and values
	BarracksLimit(2),
	RefineryLimit(4),
	RefineryRatio(0.16), // RA2: the executable's initializer list, in RA2's member order and values
	BaseSizeAdd(3),
	PowerSurplus(50),
	InfantryReserve(2000),
	InfantryBaseMult(2),
	SoloCrateMoney(2000),
	TreeStrength(25),
	UnitCrateType(0), // RA2: the executable's initializer list, in RA2's member order and values
	PatrolTime(0.016), // RA2: as above
	DissolveUnfilledTeamDelay(5000),
	AIAlternateProductionCreditCutoff(1000),
	AIUseTurbineUpgradeChance(1.0), // RA2: the executable's initializer list, in RA2's member order and values
	CloakDelay(0.0), // RA2: as above
	GameSpeedBias(1.0), // RA2: as above
	NervousBias(1.0), // RA2: as above
	ExplosionSpread(0.5), // RA2: as above
	SupressRadius(256), // RA2: as above
	MaxIQ(5),
	IQSuperWeapons(4),
	IQProduction(5),
	IQGuardArea(4),
	IQRepairSell(3),
	IQCrush(2),
	IQScatter(3),
	IQContentScan(4),
	IQAircraft(4),
	IQHarvester(3),
	IQSellBack(2),
	AIBaseSpacing(1),
	SilverCrate((CrateType)2), // RA2: the executable's initializer list, in RA2's member order and values
	WoodCrate((CrateType)0), // RA2: as above
	WaterCrate((CrateType)0), // RA2: as above
	CrateMinimum(1),
	CrateMaximum(255),
	LZScanRadius(8192), // RA2: the executable's initializer list, in RA2's member order and values
	FlareAnim(0), // RA2: as above
	MinMoney(2500), // RA2: as above
	MPMoney(10000), // RA2: as above
	MPMaxMoney(10000),
	MoneyIncrement(100), // RA2: the executable's initializer list, in RA2's member order and values
	MinUnitCount(1), // RA2: as above
	MPUnitCount(10),
	MaxUnitCount(20), // RA2: the executable's initializer list, in RA2's member order and values
	MPBuildLevel(10),
	GameSpeed(6), // RA2: the executable's initializer list, in RA2's member order and values
	AIDifficultyStruct(1), // RA2: as above
	AIPlayers(1), // RA2: as above
	IsMPBridgeDestruction(true),
	IsMPShadowGrow(false), // RA2: the executable's initializer list, in RA2's member order and values
	Shroud(true), // RA2: as above
	IsMPBasesOn(true),
	IsMPTiberiumGrow(true),
	IsMPCrates(true),
	IsMPCaptureTheFlag(false),
	HarvesterTruce(false), // RA2: the executable's initializer list, in RA2's member order and values
	MultiEngineer(false), // RA2: as above
	AlliesAllowed(false), // RA2: as above
	ShortGame(false), // RA2: as above
	FogOfWar(false), // RA2: as above
	MCVRedeploys(true), // RA2: as above
	SuperWeaponsAllowed(true), // RA2: as above
	BuildOffAlly(true), // RA2: as above
	DropZoneRadius(1024), // RA2: as above
	MessageDelay(0.6), // RA2: as above
	SavourDelay(0.03), // RA2: as above
	MaxPlayers(8), // RA2: as above
	BaseDefenseDelay(0.25), // RA2: as above
	SuspendPriority(20),
	SuspendDelay(2.0), // RA2: the executable's initializer list, in RA2's member order and values
	SurvivorFraction(0.5), // RA2: as above
	AlliedSurvivorDivisor(200), // RA2: as above
	SovietSurvivorDivisor(100), // RA2: as above
	ReloadRate(0.05), // RA2: as above
	AutocreateTime(5.0), // RA2: as above
	BuildupTime(0.05), // RA2: as above
	HarvesterLoadRate(2),
	HarvesterDumpRate(0.016), // RA2: the executable's initializer list, in RA2's member order and values
	AtomDamage(1000),
	GrowthRate(2.0), // RA2: the executable's initializer list, in RA2's member order and values
	ShroudRate(4.0), // RA2: as above
	FogRate(0.05), // RA2: as above
	IceGrowthRate(1.0), // RA2: as above
	VeinGrowthRate(1.0), // RA2: as above
	IceSolidifyDelay(500),
	AmbientLightChangeRate(0.2), // RA2: the executable's initializer list, in RA2's member order and values
	AmbientLightChangeStep(0.1), // RA2: as above
	CrateTime(10.0), // RA2: as above
	TimerWarning(2.0), // RA2: as above
	NukeTime(14.0), // RA2: as above
	EMPulseTime(5.0), // RA2: as above
	IonCannonTime(10.0), // RA2: as above
	FirestormTime(10.0), // RA2: as above
	SpeakDelay(2.0), // RA2: as above
	DamageDelay(1.0), // RA2: as above
	Gravity(3),
	LeptonsPerSightIncrease(50),
	Incoming(MPH_IMMOBILE),
	MinDamage(1),
	MaxDamage(1000),
	RepairStep(5),
	RepairPercent(0.25), // RA2: the executable's initializer list, in RA2's member order and values
	IRepairStep(1),
	RepairRate(0.016), // RA2: the executable's initializer list, in RA2's member order and values
	URepairRate(0.016), // RA2: as above
	IRepairRate(0.016), // RA2: as above
	ConditionGreen(1.0), // RA2: as above
	ConditionYellow(0.5), // RA2: as above
	ConditionRed(0.5), // RA2: as above
	RandomAnimateTime(0.083), // RA2: as above
	CloseEnoughDistance(640), // RA2: as above
	StrayDistance(512), // RA2: as above
	RelaxedStray(512), // RA2: as above
	CrushDistance(384), // RA2: as above
	CrateRadius(640), // RA2: as above
	HomingScatter(512), // RA2: as above
	BallisticScatter(256), // RA2: as above
	RefundPercent(0.5), // RA2: as above
	BridgeStrength(1000),
	BuildSpeedBias(1.0), // RA2: the executable's initializer list, in RA2's member order and values
	C4Delay(0.03), // RA2: as above
	RepairThreshhold(1000),
	PathDelay(0.016), // RA2: the executable's initializer list, in RA2's member order and values
	BlockagePathDelay(60), // RA2: as above
	MovieTime(0.25), // RA2: as above
	TiberiumShortScan(1536), // RA2: as above
	TiberiumLongScan(8192), // RA2: as above
	LightningDeferment(250), // RA2: as above
	LightningDamage(200), // RA2: as above
	LightningStormDuration(900), // RA2: as above
	LightningHitDelay(90), // RA2: as above
	LightningScatterDelay(10), // RA2: as above
	LightningCellSpread(10), // RA2: as above
	LightningSeparation(3), // RA2: as above
	LightningPrintText(true), // RA2: as above
	LightningWarhead(0), // RA2: as above
	CollapseChance(100),
	WeedCapacity(0),
	HarvestersPerRefinery(2), // RA2: the executable's initializer list, in RA2's member order and values
	ExtraUnitLight(0),
	ExtraInfantryLight(0),
	ExtraAircraftLight(0),
	IsComputerParanoid(true),
	IsCurleyShuffle(false),
	IsBlendedFog(true),
	IsCompEasyBonus(true),
	IsFineDifficulty(false),
	IsExplosiveHarvester(false),
	IsHealthBar(true),
	IsAllyReveal(true),
	IsSeparate(false),
	IsTreeTarget(false),
	IsNamed(false),
	IsAutoCrush(false),
	IsSmartDefense(false),
	IsScatter(false),
	IsRevealByHeight(true),
	IsShroudedSubteranneanMovesAllowed(false),
	IsShroudGrow(false),
	NodAIBuildsWalls(true),
	AIBuildsWalls(true),
	UseMinDefenseRule(true),
	EMPulseSparkles(0), // RA2: the executable's initializer list, in RA2's member order and values
	EngineerCaptureLevel(1.0f), // RA2: as above
	EngineerDamage(0.0f), // RA2: as above
	TalkBubbleTime(300), // RA2: as above
	RadDurationMultiple(0), // RA2: as above
	RadApplicationDelay(0), // RA2: as above
	RadLevelMax(0), // RA2: as above
	RadLevelDelay(0), // RA2: as above
	RadLightDelay(0), // RA2: as above
	RadLevelFactor(0.0), // RA2: as above
	RadLightFactor(0.0), // RA2: as above
	RadTintFactor(0.0), // RA2: as above
	RadSiteWarhead(0), // RA2: as above
	ElevationIncrement(0), // RA2: as above
	ElevationIncrementBonus(1.0), // RA2: as above
	ElevationBonusCap(0.0), // RA2: as above
	AlliedWallTransparency(false), // RA2: as above
	WallPenetratorThreshold(0.5), // RA2: as above
	LocalRadarColor(255, 255, 255), // RA2: as above
	OreTwinkleChance(50), // RA2: as above
	OreTwinkle(0) // RA2: as above
{
	V3Rocket.PauseFrames = 0; // RA2: the executable's initializer list, in RA2's member order and values
	V3Rocket.TiltFrames = 60; // RA2: as above
	V3Rocket.PitchInitial = 0.0f; // RA2: as above
	V3Rocket.PitchFinal = 1.0f; // RA2: as above
	V3Rocket.TurnRate = 0.05f; // RA2: as above
	V3Rocket.Acceleration = 0.4f; // RA2: as above
	V3Rocket.Altitude = 768; // RA2: as above
	V3Rocket.Damage = 1000; // RA2: as above
	V3Rocket.EliteDamage = 1000; // RA2: as above
	V3Rocket.BodyLength = 256; // RA2: as above
	V3Rocket.LazyCurve = true; // RA2: as above
	V3Rocket.Type = 0; // RA2: as above
	DMisl.PauseFrames = 10; // RA2: as above
	DMisl.TiltFrames = 60; // RA2: as above
	DMisl.PitchInitial = 0.0f; // RA2: as above
	DMisl.PitchFinal = 1.0f; // RA2: as above
	DMisl.TurnRate = 0.05f; // RA2: as above
	DMisl.Acceleration = 0.4f; // RA2: as above
	DMisl.Altitude = 768; // RA2: as above
	DMisl.Damage = 1000; // RA2: as above
	DMisl.EliteDamage = 1000; // RA2: as above
	DMisl.BodyLength = 128; // RA2: as above
	DMisl.LazyCurve = false; // RA2: as above
	DMisl.Type = 0; // RA2: as above
}
#endif



/// <summary>
/// Destructor for the rules class object.
/// The rules own nothing that needs releasing -- the type objects they point at belong to
/// their own heaps and outlive the rules.
/// </summary>
RulesClass::~RulesClass(void)
{
}


/// <summary>
/// Builds the game's rule data from scratch.
/// This routine wipes every object type heap clean, reloads the art database, and then
/// processes the rule file supplied. The Firestorm and language specific rule files are
/// layered over the top afterwards, so that each may override what came before it.
/// </summary>
/// <remarks>Every type object in the game is destroyed here, so nothing may be holding a
/// pointer to one when this routine is called.</remarks>
#ifdef RA2_NON_MATCHING // not exact: 233 bytes differ (adopted text)
void RulesClass::Initialize(CCINIClass & ini) // RA2: a writable INI
{
	static char const * const RulesSection_Maximums = "Maximums"; // RA2: the executable's section name, a file-static pointer at 0x007A9060
	while (ColorSchemes.Count()) delete ColorSchemes[0]; // RA2: as above
	ColorSchemes.Clear(); // RA2: as above
	SchemeLibrary_Free(); // RA2: as above

	while (TeamTypes.Count()) delete TeamTypes[0]; // RA2: the executable's Initialize (no Firestorm or art ini loading, scheme library and session ini)
	while (TaskForces.Count()) delete TaskForces[0]; // RA2: as above
	while (ScriptTypes.Count()) delete ScriptTypes[0]; // RA2: as above
	while (OverlayTypes.Count()) delete OverlayTypes[0]; // RA2: as above
	while (SmudgeTypes.Count()) delete SmudgeTypes[0]; // RA2: as above
	while (TerrainTypes.Count()) delete TerrainTypes[0]; // RA2: as above
	while (InfantryTypes.Count()) delete InfantryTypes[0]; // RA2: as above
	while (UnitTypes.Count()) delete UnitTypes[0]; // RA2: as above
	while (AircraftTypes.Count()) delete AircraftTypes[0]; // RA2: as above
	while (BulletTypes.Count()) delete BulletTypes[0]; // RA2: as above
	while (BuildingTypes.Count()) delete BuildingTypes[0]; // RA2: as above
	while (SuperWeaponTypes.Count()) delete SuperWeaponTypes[0]; // RA2: as above
	while (AnimTypes.Count()) delete AnimTypes[0]; // RA2: as above
	while (Weapons.Count()) delete Weapons[0]; // RA2: as above
	while (::Warheads.Count()) delete ::Warheads[0]; // RA2: as above
	while (VoxelAnimTypes.Count()) delete VoxelAnimTypes[0]; // RA2: as above
	while (ParticleTypes.Count()) delete ParticleTypes[0]; // RA2: as above
	while (ParticleSystemTypes.Count()) delete ParticleSystemTypes[0]; // RA2: as above
	while (::Houses.Count()) delete ::Houses[0]; // RA2: as above
	while (HouseTypes.Count()) delete HouseTypes[0]; // RA2: as above

	if (ini.Is_Present(RulesSection_Maximums)) { // RA2: the executable's Initialize (no Firestorm or art ini loading, scheme library and session ini)
		MaxPlayers = ini.Get_Int(RulesSection_Maximums, "Players", MaxPlayers); // RA2: as above
	}
	Addition(ini);

	CCFileClass file("LANGRULE.INI"); // RA2: the executable's Initialize (no Firestorm or art ini loading, scheme library and session ini)
	if (file.Is_Available() == true) { // RA2: as above
		CCINIClass langini;
		if (langini.Load(file, true) > 1) { // RA2: the executable's Initialize (no Firestorm or art ini loading, scheme library and session ini)
			return;
		}
		Addition(langini);
	}
	if (Session.Type && Session.MPGameMode) { // RA2: the executable's Initialize (no Firestorm or art ini loading, scheme library and session ini)
		Addition(*Session.MPGameMode->GetINI()); // RA2: as above
	}
}
#endif



/***********************************************************************************************
 * RulesClass::Process -- Fetch the bulk of the rule data from the control file.               *
 *                                                                                             *
 *    This routine will fetch the rule data from the control file.                             *
 *                                                                                             *
 * INPUT:   file  -- Reference to the rule file to process.                                    *
 *                                                                                             *
 * OUTPUT:  bool; Was the rule file processed?                                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/17/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool RulesClass::Addition(CCINIClass & ini) // RA2: a writable INI
{
	Color_Schemes(ini); // RA2: no benchmark brackets in the executable
	Do_HouseTypes(ini);
	Do_Sides(ini);
	Do_OverlayTypes(ini);
	Do_SuperWeaponTypes(ini);
	Do_WarheadTypes(ini);
	Do_SmudgeTypes(ini);
	Do_TerrainTypes(ini);
	Do_BuildingTypes(ini);
	Do_VehicleTypes(ini);
	Do_AircraftTypes(ini);
	Do_InfantryTypes(ini);
	Do_AnimTypes(ini);
	Do_VoxelAnimTypes(ini);
	Do_ParticleTypes(ini);
	Do_ParticleSystemTypes(ini);

	Jumpjet_Controls(ini);
	MPlayer(ini);
	AI(ini);
	Powerups(ini);
	Land_Types(ini);
	IQ(ini);

	General(ini);
	Objects(ini);
	Difficulty(ini);
	Crate_Rules(ini);
	Combat_Damage(ini);
	Radiation(ini); // RA2: Radiation, Elevation_Model and Wall_Model are read here (the executable has them as separate readers)
	Elevation_Model(ini); // RA2: as above
	Wall_Model(ini); // RA2: as above
	Audio_Visual_Rules(ini);
	Special_Weapons(ini);
	TiberiumClass::Process(ini); // RA2: the result is not used
	return(Read_AdvancedCommandBar(ini, Session.Type != GAME_NORMAL && Session.Type != GAME_SKIRMISH)); // RA2: the command bar is read last and its result returned
}



/// <summary>
/// Fetches the special weapon control values.
/// These are the warheads, projectiles and animations the nuclear missile and the EM pulse
/// are built from. Every warhead in the game is given a chance to reread itself here as
/// well, since the special weapons can redefine what the ordinary ones do.
/// </summary>
/// <returns>bool; Was a special weapons section found in the control file?</returns>
bool RulesClass::Special_Weapons(CCINIClass & ini) // RA2: a writable INI
{
	static char const * const SPECIALWEAPONS = "SpecialWeapons";
	if (ini.Is_Present(SPECIALWEAPONS)) {
		HSBuilding = TGet_TypeList<BuildingTypeClass>(ini, SPECIALWEAPONS, "HSBuilding", HSBuilding);
		NukeWarhead = TGet_Class(ini, SPECIALWEAPONS, "NukeWarhead", NukeWarhead);
		NukeProjectile = TGet_Class(ini, SPECIALWEAPONS, "NukeProjectile", NukeProjectile);
		NukeDown = TGet_Class(ini, SPECIALWEAPONS, "NukeDown", NukeDown);
		EMPulseWarhead = TGet_Class(ini, SPECIALWEAPONS, "EMPulseWarhead", EMPulseWarhead);
		EMPulseProjectile = TGet_Class(ini, SPECIALWEAPONS, "EMPulseProjectile", EMPulseProjectile);
		for (int i = 0; i < ::Warheads.Count(); i++) {
			::Warheads[i]->Read_INI(ini);
		}
		return(true);
	}
	return(false);
}


/// <summary>
/// RA2
/// </summary>
static inline int Sound_From_Name(char const * name, int fallback)
{
	int index = VocClass::From_Name(name); // RA2: the voice lookup
	return index == -1 ? fallback : index;
}


/// <summary>
/// RA2
/// </summary>
static inline int Read_Sound(CCINIClass const & ini, char const * section, char const * key, int fallback)
{
	char name[128];
	if (ini.Get_String(section, key, "", name, sizeof(name)))
		return Sound_From_Name(name, fallback);
	return fallback;
}


/// <summary>
/// Fetches the audio and visual control values.
/// This is the catch-all section for presentation -- the sounds the interface plays, the
/// animations attached to smoke, fire, parachutes and drop pods, the shroud and fog rates,
/// and the extra lighting the various object kinds are drawn with.
/// </summary>
/// <returns>bool; Was an audio visual section found in the control file?</returns>
#ifdef RA2_NON_MATCHING // not exact: 10402 bytes differ (adopted text)
bool RulesClass::Audio_Visual_Rules(CCINIClass const & ini)
{
	static char const * const RulesSection_AudioVisual = "AudioVisual"; // RA2: the executable's section name, a file-static pointer at 0x007A903C
	if (ini.Is_Present(RulesSection_AudioVisual)) { // RA2: as above
		DetailMinFrameRateNormal = ini.Get_Int(RulesSection_AudioVisual, "DetailMinFrameRateNormal", DetailMinFrameRateNormal); // RA2: as above
		DetailMinFrameRateMovie = ini.Get_Int(RulesSection_AudioVisual, "DetailMinFrameRateMovie", DetailMinFrameRateMovie); // RA2: as above
		DetailBufferZoneWidth = ini.Get_Int(RulesSection_AudioVisual, "DetailBufferZoneWidth", DetailBufferZoneWidth); // RA2: as above
		UnloadingHarvester = TGet_Class(ini, RulesSection_AudioVisual, "UnloadingHarvester", UnloadingHarvester); // RA2: as above
		PoseDir = (Dir256)ini.Get_Int(RulesSection_AudioVisual, "PoseDir", PoseDir); // RA2: as above
		DropPodPuff = TGet_Class(ini, RulesSection_AudioVisual, "DropPodPuff", DropPodPuff); // RA2: as above
		WaypointAnimationSpeed = ini.Get_Int(RulesSection_AudioVisual, "WaypointAnimationSpeed", WaypointAnimationSpeed); // RA2: as above
		BarrelExplode = TGet_Class(ini, RulesSection_AudioVisual, "BarrelExplode", BarrelExplode); // RA2: as above
		BarrelDebris = TGet_TypeList<VoxelAnimTypeClass>(ini, RulesSection_AudioVisual, "BarrelDebris", BarrelDebris); // RA2: as above
		BarrelParticle = TGet_Class(ini, RulesSection_AudioVisual, "BarrelParticle", BarrelParticle); // RA2: as above
		Wake = TGet_Class(ini, RulesSection_AudioVisual, "Wake", Wake); // RA2: as above
		FlamingInfantry = TGet_Class(ini, RulesSection_AudioVisual, "FlamingInfantry", FlamingInfantry); // RA2: as above
		NukeTakeOff = TGet_Class(ini, RulesSection_AudioVisual, "NukeTakeOff", NukeTakeOff); // RA2: as above
		InfantryHeadPop = TGet_Class(ini, RulesSection_AudioVisual, "InfantryHeadPop", InfantryHeadPop); // RA2: as above
		InfantryNuked = TGet_Class(ini, RulesSection_AudioVisual, "InfantryNuked", InfantryNuked); // RA2: as above
		Behind = TGet_Class(ini, RulesSection_AudioVisual, "Behind", Behind); // RA2: as above
		VeinAttack = TGet_Class(ini, RulesSection_AudioVisual, "VeinAttack", VeinAttack); // RA2: as above
		DropPod = TGet_TypeList<AnimTypeClass>(ini, RulesSection_AudioVisual, "DropPod", DropPod); // RA2: as above
		VocType dig_sound = ini.Get_VocType(RulesSection_AudioVisual, "DigSound", (VocType)DigSound); // RA2: as above
		DigSound = dig_sound; // RA2: as above
		VocType guimain_button_sound = ini.Get_VocType(RulesSection_AudioVisual, "GUIMainButtonSound", (VocType)GUIMainButtonSound); // RA2: as above
		GUIMainButtonSound = guimain_button_sound; // RA2: as above
		VocType guibuild_sound = ini.Get_VocType(RulesSection_AudioVisual, "GUIBuildSound", (VocType)GUIBuildSound); // RA2: as above
		GUIBuildSound = guibuild_sound; // RA2: as above
		VocType guitab_sound = ini.Get_VocType(RulesSection_AudioVisual, "GUITabSound", (VocType)GUITabSound); // RA2: as above
		GUITabSound = guitab_sound; // RA2: as above
		VocType guiopen_sound = ini.Get_VocType(RulesSection_AudioVisual, "GUIOpenSound", (VocType)GUIOpenSound); // RA2: as above
		GUIOpenSound = guiopen_sound; // RA2: as above
		VocType guiclose_sound = ini.Get_VocType(RulesSection_AudioVisual, "GUICloseSound", (VocType)GUICloseSound); // RA2: as above
		GUICloseSound = guiclose_sound; // RA2: as above
		VocType guimove_out_sound = ini.Get_VocType(RulesSection_AudioVisual, "GUIMoveOutSound", (VocType)GUIMoveOutSound); // RA2: as above
		GUIMoveOutSound = guimove_out_sound; // RA2: as above
		VocType guimove_in_sound = ini.Get_VocType(RulesSection_AudioVisual, "GUIMoveInSound", (VocType)GUIMoveInSound); // RA2: as above
		GUIMoveInSound = guimove_in_sound; // RA2: as above
		VocType guicombo_open_sound = ini.Get_VocType(RulesSection_AudioVisual, "GUIComboOpenSound", (VocType)GUIComboOpenSound); // RA2: as above
		GUIComboOpenSound = guicombo_open_sound; // RA2: as above
		VocType guicombo_close_sound = ini.Get_VocType(RulesSection_AudioVisual, "GUIComboCloseSound", (VocType)GUIComboCloseSound); // RA2: as above
		GUIComboCloseSound = guicombo_close_sound; // RA2: as above
		VocType guicheckbox_sound = ini.Get_VocType(RulesSection_AudioVisual, "GUICheckboxSound", (VocType)GUICheckboxSound); // RA2: as above
		GUICheckboxSound = guicheckbox_sound; // RA2: as above
		VocType score_anim_sound = ini.Get_VocType(RulesSection_AudioVisual, "ScoreAnimSound", (VocType)ScoreAnimSound); // RA2: as above
		ScoreAnimSound = score_anim_sound; // RA2: as above
		VocType cheer_sound = ini.Get_VocType(RulesSection_AudioVisual, "CheerSound", (VocType)CheerSound); // RA2: as above
		CheerSound = cheer_sound; // RA2: as above
		VocType default_chrono_sound = ini.Get_VocType(RulesSection_AudioVisual, "DefaultChronoSound", (VocType)DefaultChronoSound); // RA2: as above
		DefaultChronoSound = default_chrono_sound; // RA2: as above
		VocType start_planning_mode_sound = ini.Get_VocType(RulesSection_AudioVisual, "StartPlanningModeSound", (VocType)StartPlanningModeSound); // RA2: as above
		StartPlanningModeSound = start_planning_mode_sound; // RA2: as above
		VocType end_planning_mode_sound = ini.Get_VocType(RulesSection_AudioVisual, "EndPlanningModeSound", (VocType)EndPlanningModeSound); // RA2: as above
		EndPlanningModeSound = end_planning_mode_sound; // RA2: as above
		VocType crate_money_sound = ini.Get_VocType(RulesSection_AudioVisual, "CrateMoneySound", (VocType)CrateMoneySound); // RA2: as above
		CrateMoneySound = crate_money_sound; // RA2: as above
		VocType crate_reveal_sound = ini.Get_VocType(RulesSection_AudioVisual, "CrateRevealSound", (VocType)CrateRevealSound); // RA2: as above
		CrateRevealSound = crate_reveal_sound; // RA2: as above
		VocType crate_fire_sound = ini.Get_VocType(RulesSection_AudioVisual, "CrateFireSound", (VocType)CrateFireSound); // RA2: as above
		CrateFireSound = crate_fire_sound; // RA2: as above
		VocType crate_armour_sound = ini.Get_VocType(RulesSection_AudioVisual, "CrateArmourSound", (VocType)CrateArmourSound); // RA2: as above
		CrateArmourSound = crate_armour_sound; // RA2: as above
		VocType crate_speed_sound = ini.Get_VocType(RulesSection_AudioVisual, "CrateSpeedSound", (VocType)CrateSpeedSound); // RA2: as above
		CrateSpeedSound = crate_speed_sound; // RA2: as above
		VocType crate_unit_sound = ini.Get_VocType(RulesSection_AudioVisual, "CrateUnitSound", (VocType)CrateUnitSound); // RA2: as above
		CrateUnitSound = crate_unit_sound; // RA2: as above
		VocType crate_promote_sound = ini.Get_VocType(RulesSection_AudioVisual, "CratePromoteSound", (VocType)CratePromoteSound); // RA2: as above
		CratePromoteSound = crate_promote_sound; // RA2: as above
		VocType impact_water_sound = ini.Get_VocType(RulesSection_AudioVisual, "ImpactWaterSound", (VocType)ImpactWaterSound); // RA2: as above
		ImpactWaterSound = impact_water_sound; // RA2: as above
		VocType impact_land_sound = ini.Get_VocType(RulesSection_AudioVisual, "ImpactLandSound", (VocType)ImpactLandSound); // RA2: as above
		ImpactLandSound = impact_land_sound; // RA2: as above
		VocType sinking_sound = ini.Get_VocType(RulesSection_AudioVisual, "SinkingSound", (VocType)SinkingSound); // RA2: as above
		SinkingSound = sinking_sound; // RA2: as above
		VocType chrono_in_sound = ini.Get_VocType(RulesSection_AudioVisual, "ChronoInSound", (VocType)ChronoInSound); // RA2: as above
		ChronoInSound = chrono_in_sound; // RA2: as above
		VocType chrono_out_sound = ini.Get_VocType(RulesSection_AudioVisual, "ChronoOutSound", (VocType)ChronoOutSound); // RA2: as above
		ChronoOutSound = chrono_out_sound; // RA2: as above
		VocType bomb_ticking_sound = ini.Get_VocType(RulesSection_AudioVisual, "BombTickingSound", (VocType)BombTickingSound); // RA2: as above
		BombTickingSound = bomb_ticking_sound; // RA2: as above
		VocType bomb_attach_sound = ini.Get_VocType(RulesSection_AudioVisual, "BombAttachSound", (VocType)BombAttachSound); // RA2: as above
		BombAttachSound = bomb_attach_sound; // RA2: as above
		VocType yuri_mind_control_sound = ini.Get_VocType(RulesSection_AudioVisual, "YuriMindControlSound", (VocType)YuriMindControlSound); // RA2: as above
		YuriMindControlSound = yuri_mind_control_sound; // RA2: as above
		VocType add_planning_mode_command_sound = ini.Get_VocType(RulesSection_AudioVisual, "AddPlanningModeCommandSound", (VocType)AddPlanningModeCommandSound); // RA2: as above
		AddPlanningModeCommandSound = add_planning_mode_command_sound; // RA2: as above
		VocType execute_plan_sound = ini.Get_VocType(RulesSection_AudioVisual, "ExecutePlanSound", (VocType)ExecutePlanSound); // RA2: as above
		ExecutePlanSound = execute_plan_sound; // RA2: as above
		VocType place_beacon_sound = ini.Get_VocType(RulesSection_AudioVisual, "PlaceBeaconSound", (VocType)PlaceBeaconSound); // RA2: as above
		PlaceBeaconSound = place_beacon_sound; // RA2: as above
		VocType building_garrisoned_sound = ini.Get_VocType(RulesSection_AudioVisual, "BuildingGarrisonedSound", (VocType)BuildingGarrisonedSound); // RA2: as above
		BuildingGarrisonedSound = building_garrisoned_sound; // RA2: as above
		VocType building_abandoned_sound = ini.Get_VocType(RulesSection_AudioVisual, "BuildingAbandonedSound", (VocType)BuildingAbandonedSound); // RA2: as above
		BuildingAbandonedSound = building_abandoned_sound; // RA2: as above
		VocType building_repaired_sound = ini.Get_VocType(RulesSection_AudioVisual, "BuildingRepairedSound", (VocType)BuildingRepairedSound); // RA2: as above
		BuildingRepairedSound = building_repaired_sound; // RA2: as above
		VocType base_under_attack_sound = ini.Get_VocType(RulesSection_AudioVisual, "BaseUnderAttackSound", (VocType)BaseUnderAttackSound); // RA2: as above
		BaseUnderAttackSound = base_under_attack_sound; // RA2: as above
		VocType upgrade_veteran_sound = ini.Get_VocType(RulesSection_AudioVisual, "UpgradeVeteranSound", (VocType)UpgradeVeteranSound); // RA2: as above
		UpgradeVeteranSound = upgrade_veteran_sound; // RA2: as above
		VocType upgrade_elite_sound = ini.Get_VocType(RulesSection_AudioVisual, "UpgradeEliteSound", (VocType)UpgradeEliteSound); // RA2: as above
		UpgradeEliteSound = upgrade_elite_sound; // RA2: as above
		VocType create_unit_sound = ini.Get_VocType(RulesSection_AudioVisual, "CreateUnitSound", (VocType)CreateUnitSound); // RA2: as above
		CreateUnitSound = create_unit_sound; // RA2: as above
		VocType create_infantry_sound = ini.Get_VocType(RulesSection_AudioVisual, "CreateInfantrySound", (VocType)CreateInfantrySound); // RA2: as above
		CreateInfantrySound = create_infantry_sound; // RA2: as above
		VocType create_aircraft_sound = ini.Get_VocType(RulesSection_AudioVisual, "CreateAircraftSound", (VocType)CreateAircraftSound); // RA2: as above
		CreateAircraftSound = create_aircraft_sound; // RA2: as above
		VocType ifvtransform_sound = ini.Get_VocType(RulesSection_AudioVisual, "IFVTransformSound", (VocType)IFVTransformSound); // RA2: as above
		IFVTransformSound = ifvtransform_sound; // RA2: as above
		VocType psychic_sensor_detect_sound = ini.Get_VocType(RulesSection_AudioVisual, "PsychicSensorDetectSound", (VocType)PsychicSensorDetectSound); // RA2: as above
		PsychicSensorDetectSound = psychic_sensor_detect_sound; // RA2: as above
		VocType spy_sat_activation_sound = ini.Get_VocType(RulesSection_AudioVisual, "SpySatActivationSound", (VocType)SpySatActivationSound); // RA2: as above
		SpySatActivationSound = spy_sat_activation_sound; // RA2: as above
		VocType spy_sat_deactivation_sound = ini.Get_VocType(RulesSection_AudioVisual, "SpySatDeactivationSound", (VocType)SpySatDeactivationSound); // RA2: as above
		SpySatDeactivationSound = spy_sat_deactivation_sound; // RA2: as above
		Dig = TGet_Class(ini, RulesSection_AudioVisual, "Dig", Dig); // RA2: as above
		IonBlast = TGet_Class(ini, RulesSection_AudioVisual, "IonBlast", IonBlast); // RA2: as above
		IonBeam = TGet_Class(ini, RulesSection_AudioVisual, "IonBeam", IonBeam); // RA2: as above
		ChronoBeam = TGet_Class(ini, RulesSection_AudioVisual, "ChronoBeam", ChronoBeam); // RA2: as above
		ChronoBlast = TGet_Class(ini, RulesSection_AudioVisual, "ChronoBlast", ChronoBlast); // RA2: as above
		ChronoBlastDest = TGet_Class(ini, RulesSection_AudioVisual, "ChronoBlastDest", ChronoBlastDest); // RA2: as above
		ChronoPlacement = TGet_Class(ini, RulesSection_AudioVisual, "ChronoPlacement", ChronoPlacement); // RA2: as above
		WarpIn = TGet_Class(ini, RulesSection_AudioVisual, "WarpIn", WarpIn); // RA2: as above
		WarpOut = TGet_Class(ini, RulesSection_AudioVisual, "WarpOut", WarpOut); // RA2: as above
		WarpAway = TGet_Class(ini, RulesSection_AudioVisual, "WarpAway", WarpAway); // RA2: as above
		ChronoSparkle1 = TGet_Class(ini, RulesSection_AudioVisual, "ChronoSparkle1", ChronoSparkle1); // RA2: as above
		IronCurtainInvokeAnim = TGet_Class(ini, RulesSection_AudioVisual, "IronCurtainInvokeAnim", IronCurtainInvokeAnim); // RA2: as above
		WeaponNullifyAnim = TGet_Class(ini, RulesSection_AudioVisual, "WeaponNullifyAnim", WeaponNullifyAnim); // RA2: as above
		InfantryExplode = TGet_Class(ini, RulesSection_AudioVisual, "InfantryExplode", InfantryExplode); // RA2: as above
		AtmosphereEntry = TGet_Class(ini, RulesSection_AudioVisual, "AtmosphereEntry", AtmosphereEntry); // RA2: as above
		VocType gate_up = ini.Get_VocType(RulesSection_AudioVisual, "GateUp", (VocType)GateUpSound); // RA2: as above
		GateUpSound = gate_up; // RA2: as above
		VocType gate_down = ini.Get_VocType(RulesSection_AudioVisual, "GateDown", (VocType)GateDownSound); // RA2: as above
		GateDownSound = gate_down; // RA2: as above
		IsShroudGrow = ini.Get_Bool(RulesSection_AudioVisual, "ShroudGrow", IsShroudGrow); // RA2: as above
		double scroll_multiplier = ini.Get_Float(RulesSection_AudioVisual, "ScrollMultiplier", ScrollMultiplier); // RA2: as above
		ScrollMultiplier = scroll_multiplier; // RA2: as above
		ShakeScreen = ini.Get_Int(RulesSection_AudioVisual, "ShakeScreen", ShakeScreen); // RA2: as above
		VocType cloak_sound = ini.Get_VocType(RulesSection_AudioVisual, "CloakSound", (VocType)CloakSound); // RA2: as above
		CloakSound = cloak_sound; // RA2: as above
		VocType sell_sound = ini.Get_VocType(RulesSection_AudioVisual, "SellSound", (VocType)SellSound); // RA2: as above
		SellSound = sell_sound; // RA2: as above
		VocType game_closed = ini.Get_VocType(RulesSection_AudioVisual, "GameClosed", (VocType)GameClosed); // RA2: as above
		GameClosed = game_closed; // RA2: as above
		VocType incoming_message = ini.Get_VocType(RulesSection_AudioVisual, "IncomingMessage", (VocType)IncomingMessage); // RA2: as above
		IncomingMessage = incoming_message; // RA2: as above
		VocType message_char_typed = ini.Get_VocType(RulesSection_AudioVisual, "MessageCharTyped", (VocType)MessageCharTyped); // RA2: as above
		MessageCharTyped = message_char_typed; // RA2: as above
		VocType system_error = ini.Get_VocType(RulesSection_AudioVisual, "SystemError", (VocType)SystemError); // RA2: as above
		SystemError = system_error; // RA2: as above
		VocType options_changed = ini.Get_VocType(RulesSection_AudioVisual, "OptionsChanged", (VocType)OptionsChanged); // RA2: as above
		OptionsChanged = options_changed; // RA2: as above
		VocType game_forming = ini.Get_VocType(RulesSection_AudioVisual, "GameForming", (VocType)GameForming); // RA2: as above
		GameForming = game_forming; // RA2: as above
		VocType player_left = ini.Get_VocType(RulesSection_AudioVisual, "PlayerLeft", (VocType)PlayerLeft); // RA2: as above
		PlayerLeft = player_left; // RA2: as above
		VocType player_joined = ini.Get_VocType(RulesSection_AudioVisual, "PlayerJoined", (VocType)PlayerJoined); // RA2: as above
		PlayerJoined = player_joined; // RA2: as above
		VocType construction = ini.Get_VocType(RulesSection_AudioVisual, "Construction", (VocType)Construction); // RA2: as above
		Construction = construction; // RA2: as above
		CreditTicks = CCINIClass::Get_VocType_List(ini, RulesSection_AudioVisual, "CreditTicks", CreditTicks); // RA2: as above
		VocType building_die_sound = ini.Get_VocType(RulesSection_AudioVisual, "BuildingDieSound", (VocType)BuildingDieSound); // RA2: as above
		BuildingDieSound = building_die_sound; // RA2: as above
		VocType building_slam = ini.Get_VocType(RulesSection_AudioVisual, "BuildingSlam", (VocType)BuildingSlam); // RA2: as above
		BuildingSlam = building_slam; // RA2: as above
		VocType radar_on = ini.Get_VocType(RulesSection_AudioVisual, "RadarOn", (VocType)RadarOn); // RA2: as above
		RadarOn = radar_on; // RA2: as above
		VocType radar_off = ini.Get_VocType(RulesSection_AudioVisual, "RadarOff", (VocType)RadarOff); // RA2: as above
		RadarOff = radar_off; // RA2: as above
		VocType movie_on = ini.Get_VocType(RulesSection_AudioVisual, "MovieOn", (VocType)MovieOn); // RA2: as above
		MovieOn = movie_on; // RA2: as above
		VocType movie_off = ini.Get_VocType(RulesSection_AudioVisual, "MovieOff", (VocType)MovieOff); // RA2: as above
		MovieOff = movie_off; // RA2: as above
		VocType scold_sound = ini.Get_VocType(RulesSection_AudioVisual, "ScoldSound", (VocType)ScoldSound); // RA2: as above
		ScoldSound = scold_sound; // RA2: as above
		VocType tesla_charge = ini.Get_VocType(RulesSection_AudioVisual, "TeslaCharge", (VocType)TeslaCharge); // RA2: as above
		TeslaCharge = tesla_charge; // RA2: as above
		VocType tesla_zap = ini.Get_VocType(RulesSection_AudioVisual, "TeslaZap", (VocType)TeslaZap); // RA2: as above
		TeslaZap = tesla_zap; // RA2: as above
		VocType building_damage_sound = ini.Get_VocType(RulesSection_AudioVisual, "BuildingDamageSound", (VocType)BuildingDamageSound); // RA2: as above
		BuildingDamageSound = building_damage_sound; // RA2: as above
		VocType chute_sound = ini.Get_VocType(RulesSection_AudioVisual, "ChuteSound", (VocType)ChuteSound); // RA2: as above
		ChuteSound = chute_sound; // RA2: as above
		VocType generic_click = ini.Get_VocType(RulesSection_AudioVisual, "GenericClick", (VocType)GenericClick); // RA2: as above
		GenericClick = generic_click; // RA2: as above
		VocType generic_beep = ini.Get_VocType(RulesSection_AudioVisual, "GenericBeep", (VocType)GenericBeep); // RA2: as above
		GenericBeep = generic_beep; // RA2: as above
		VocType building_drop = ini.Get_VocType(RulesSection_AudioVisual, "BuildingDrop", (VocType)BuildingDrop); // RA2: as above
		BuildingDrop = building_drop; // RA2: as above
		VocType stop_sound = ini.Get_VocType(RulesSection_AudioVisual, "StopSound", (VocType)StopSound); // RA2: as above
		StopSound = stop_sound; // RA2: as above
		VocType guard_sound = ini.Get_VocType(RulesSection_AudioVisual, "GuardSound", (VocType)GuardSound); // RA2: as above
		GuardSound = guard_sound; // RA2: as above
		VocType scatter_sound = ini.Get_VocType(RulesSection_AudioVisual, "ScatterSound", (VocType)ScatterSound); // RA2: as above
		ScatterSound = scatter_sound; // RA2: as above
		VocType storm_sound = ini.Get_VocType(RulesSection_AudioVisual, "StormSound", (VocType)StormSound); // RA2: as above
		StormSound = storm_sound; // RA2: as above
		LightningSounds = CCINIClass::Get_VocType_List(ini, RulesSection_AudioVisual, "LightningSounds", LightningSounds); // RA2: as above
		VocType shell_button_slide_sound = ini.Get_VocType(RulesSection_AudioVisual, "ShellButtonSlideSound", (VocType)ShellButtonSlideSound); // RA2: as above
		ShellButtonSlideSound = shell_button_slide_sound; // RA2: as above
		TreeFire = TGet_TypeList<AnimTypeClass>(ini, RulesSection_AudioVisual, "TreeFire", TreeFire); // RA2: as above
		DeadBodies = TGet_TypeList<AnimTypeClass>(ini, RulesSection_AudioVisual, "DeadBodies", DeadBodies); // RA2: as above
		MetallicDebris = TGet_TypeList<AnimTypeClass>(ini, RulesSection_AudioVisual, "MetallicDebris", MetallicDebris); // RA2: as above
		BridgeExplosions = TGet_TypeList<AnimTypeClass>(ini, RulesSection_AudioVisual, "BridgeExplosions", BridgeExplosions); // RA2: as above
		DamageFireTypes = TGet_TypeList<AnimTypeClass>(ini, RulesSection_AudioVisual, "DamageFireTypes", DamageFireTypes); // RA2: as above
		WeatherConClouds = TGet_TypeList<AnimTypeClass>(ini, RulesSection_AudioVisual, "WeatherConClouds", WeatherConClouds); // RA2: as above
		WeatherConBolts = TGet_TypeList<AnimTypeClass>(ini, RulesSection_AudioVisual, "WeatherConBolts", WeatherConBolts); // RA2: as above
		WeatherConBoltExplosion = TGet_Class(ini, RulesSection_AudioVisual, "WeatherConBoltExplosion", WeatherConBoltExplosion); // RA2: as above
		OnFire = TGet_TypeList<AnimTypeClass>(ini, RulesSection_AudioVisual, "OnFire", OnFire); // RA2: as above
		Smoke1 = TGet_Class(ini, RulesSection_AudioVisual, "Smoke", Smoke1); // RA2: as above
		Smoke2 = TGet_Class(ini, RulesSection_AudioVisual, "Smoke", Smoke2); // RA2: as above
		FirestormActiveAnim = TGet_Class(ini, RulesSection_AudioVisual, "FirestormActiveAnim", FirestormActiveAnim); // RA2: as above
		FirestormIdleAnim = TGet_Class(ini, RulesSection_AudioVisual, "FirestormIdleAnim", FirestormIdleAnim); // RA2: as above
		FirestormAirAnim = TGet_Class(ini, RulesSection_AudioVisual, "FirestormAirAnim", FirestormAirAnim); // RA2: as above
		FirestormGroundAnim = TGet_Class(ini, RulesSection_AudioVisual, "FirestormGroundAnim", FirestormGroundAnim); // RA2: as above
		MoveFlash = TGet_Class(ini, RulesSection_AudioVisual, "MoveFlash", MoveFlash); // RA2: as above
		EliteFlashTimer = ini.Get_Int(RulesSection_AudioVisual, "EliteFlashTimer", EliteFlashTimer); // RA2: as above
		Parachute = TGet_Class(ini, RulesSection_AudioVisual, "Parachute", Parachute); // RA2: as above
		BombParachute = TGet_Class(ini, RulesSection_AudioVisual, "BombParachute", BombParachute); // RA2: as above
		SmallFire = TGet_Class(ini, RulesSection_AudioVisual, "SmallFire", SmallFire); // RA2: as above
		LargeFire = TGet_Class(ini, RulesSection_AudioVisual, "LargeFire", LargeFire); // RA2: as above
		IsAllyReveal = ini.Get_Bool(RulesSection_AudioVisual, "AllyReveal", IsAllyReveal); // RA2: as above
		ConditionGreen = 1;
		double condition_red = ini.Get_Float(RulesSection_AudioVisual, "ConditionRed", ConditionRed); // RA2: the executable's AudioVisual reader: RA2's keys and member order
		ConditionRed = condition_red; // RA2: as above
		double condition_yellow = ini.Get_Float(RulesSection_AudioVisual, "ConditionYellow", ConditionYellow); // RA2: as above
		ConditionYellow = condition_yellow; // RA2: as above
		DropZoneRadius = ini.Get_Lepton(RulesSection_AudioVisual, "DropZoneRadius", DropZoneRadius); // RA2: as above
		FlareAnim = TGet_Class(ini, RulesSection_AudioVisual, "DropZoneAnim", FlareAnim); // RA2: as above
		IsHealthBar = ini.Get_Bool(RulesSection_AudioVisual, "EnemyHealth", IsHealthBar); // RA2: as above
		Gravity = ini.Get_Int(RulesSection_AudioVisual, "Gravity", Gravity); // RA2: as above
		double idle_action_frequency = ini.Get_Float(RulesSection_AudioVisual, "IdleActionFrequency", RandomAnimateTime); // RA2: as above
		RandomAnimateTime = idle_action_frequency; // RA2: as above
		double message_delay = ini.Get_Float(RulesSection_AudioVisual, "MessageDelay", MessageDelay); // RA2: as above
		MessageDelay = message_delay; // RA2: as above
		double movie_time = ini.Get_Float(RulesSection_AudioVisual, "MovieTime", MovieTime); // RA2: as above
		MovieTime = movie_time; // RA2: as above
		IsNamed = ini.Get_Bool(RulesSection_AudioVisual, "NamedCivilians", IsNamed); // RA2: as above
		double savour_delay = ini.Get_Float(RulesSection_AudioVisual, "SavourDelay", SavourDelay); // RA2: as above
		SavourDelay = savour_delay; // RA2: as above
		double shroud_rate = ini.Get_Float(RulesSection_AudioVisual, "ShroudRate", ShroudRate); // RA2: as above
		ShroudRate = shroud_rate; // RA2: as above
		double fog_rate = ini.Get_Float(RulesSection_AudioVisual, "FogRate", FogRate); // RA2: as above
		FogRate = fog_rate; // RA2: as above
		double vein_growth_rate = ini.Get_Float(RulesSection_AudioVisual, "VeinGrowthRate", VeinGrowthRate); // RA2: as above
		VeinGrowthRate = vein_growth_rate; // RA2: as above
		double ice_growth_rate = ini.Get_Float(RulesSection_AudioVisual, "IceGrowthRate", IceGrowthRate); // RA2: as above
		IceGrowthRate = ice_growth_rate; // RA2: as above
		IceSolidifyDelay = ini.Get_Int(RulesSection_AudioVisual, "IceSolidifyFrameTime", IceSolidifyDelay); // RA2: as above
		IceCrackSounds.Clear();
		IceCrackSounds = CCINIClass::Get_VocType_List(ini, RulesSection_AudioVisual, "IceCrackSounds", IceCrackSounds); // RA2: the executable's AudioVisual reader: RA2's keys and member order
		double ambient_change_rate = ini.Get_Float(RulesSection_AudioVisual, "AmbientChangeRate", AmbientLightChangeRate); // RA2: as above
		AmbientLightChangeRate = ambient_change_rate; // RA2: as above
		double ambient_change_step = ini.Get_Float(RulesSection_AudioVisual, "AmbientChangeStep", AmbientLightChangeStep); // RA2: as above
		AmbientLightChangeStep = ambient_change_step; // RA2: as above
		double speak_delay = ini.Get_Float(RulesSection_AudioVisual, "SpeakDelay", SpeakDelay); // RA2: as above
		SpeakDelay = speak_delay; // RA2: as above
		double timer_warning = ini.Get_Float(RulesSection_AudioVisual, "TimerWarning", TimerWarning); // RA2: as above
		TimerWarning = timer_warning; // RA2: as above
		ExtraUnitLight = ini.Get_Float(RulesSection_AudioVisual, "ExtraUnitLight", ExtraUnitLight / 1000) * 1000.0; // RA2: as above
		ExtraInfantryLight = ini.Get_Float(RulesSection_AudioVisual, "ExtraInfantryLight", ExtraInfantryLight / 1000) * 1000.0; // RA2: as above
		ExtraAircraftLight = ini.Get_Float(RulesSection_AudioVisual, "ExtraAircraftLight", ExtraAircraftLight / 1000) * 1000.0; // RA2: as above
		EMPulseSparkles = TGet_Class(ini, RulesSection_AudioVisual, "EMPulseSparkles", EMPulseSparkles); // RA2: as above
		LocalRadarColor = ini.Get_RGBClass(RulesSection_AudioVisual, "LocalRadarColor", LocalRadarColor); // RA2: as above
		LineTrailColorOverride = ini.Get_RGBClass(RulesSection_AudioVisual, "LineTrailColorOverride", LineTrailColorOverride); // RA2: as above
		ChronoBeamColor = ini.Get_RGBClass(RulesSection_AudioVisual, "ChronoBeamColor", ChronoBeamColor); // RA2: as above
		OreTwinkleChance = ini.Get_Int(RulesSection_AudioVisual, "OreTwinkleChance", OreTwinkleChance); // RA2: as above
		OreTwinkle = TGet_Class(ini, RulesSection_AudioVisual, "OreTwinkle", OreTwinkle); // RA2: as above
		return(true);
	}
	return(false);
}
#endif



/// <summary>
/// Fetches the crate control values.
/// These are the values the crate system works from -- how many crates the map is to carry,
/// how far from the crate a unit may be and still claim it, how long before a claimed crate
/// is replaced, and which bonus the wood and silver crates hand out.
/// </summary>
/// <returns>bool; Was a crate rules section found in the control file?</returns>
bool RulesClass::Crate_Rules(CCINIClass const & ini)
{
	static char const * CRATERULES = "CrateRules"; // RA2: a plain pointer (the Do_ readers match only so)
	if (ini.Is_Present(CRATERULES)) {
		IsFreeMCV = ini.Get_Bool(CRATERULES, "FreeMCV", IsFreeMCV);
		WoodCrateImg = TGet_Class(ini, CRATERULES, "WoodCrateImg", WoodCrateImg);
		CrateImg = TGet_Class(ini, CRATERULES, "CrateImg", CrateImg);
		WaterCrateImg = TGet_Class(ini, CRATERULES, "WaterCrateImg", WaterCrateImg); // RA2: Crate_Rules at 0x6469B0 matches only so
		HealCrateSound = ini.Get_VocType(CRATERULES, "HealCrateSound", HealCrateSound);
		CrateMinimum = ini.Get_Int(CRATERULES, "CrateMinimum", CrateMinimum);
		CrateMaximum = ini.Get_Int(CRATERULES, "CrateMaximum", CrateMaximum);
		CrateRadius = ini.Get_Lepton(CRATERULES, "CrateRadius", CrateRadius);
		CrateTime = ini.Get_Float(CRATERULES, "CrateRegen", CrateTime);
		UnitCrateType = TGet_Class(ini, CRATERULES, "UnitCrateType", UnitCrateType);
		SoloCrateMoney = ini.Get_Int(CRATERULES, "SoloCrateMoney", SoloCrateMoney);
		SilverCrate = ini.Get_CrateType(CRATERULES, "SilverCrate", SilverCrate);
		WoodCrate = ini.Get_CrateType(CRATERULES, "WoodCrate", WoodCrate);
		WaterCrate = ini.Get_CrateType(CRATERULES, "WaterCrate", WaterCrate); // RA2: as above
		return(true);
	}
	return(false);
}


/// <summary>
/// Fetches the combat and damage control values.
/// This section holds the constants the combat system works from -- the damage figures for
/// the fixed weapons, the scatter of ballistic and homing projectiles, the crushing and
/// suppression distances, and the scorches, craters and smoke systems left behind.
/// </summary>
/// <returns>bool; Was a combat damage section found in the control file?</returns>
#ifdef RA2_NON_MATCHING // not exact: 2611 bytes differ (adopted text)
bool RulesClass::Combat_Damage(CCINIClass const & ini)
{
	static char const * const RulesSection_CombatDamage = "CombatDamage"; // RA2: the executable's section name, a file-static pointer at 0x007A9044
	if (ini.Is_Present(RulesSection_CombatDamage)) { // RA2: as above
		int ammo_crate_damage = ini.Get_Int(RulesSection_CombatDamage, "AmmoCrateDamage", AmmoCrateDamage); // RA2: as above
		AmmoCrateDamage = ammo_crate_damage; // RA2: as above
		int ion_cannon_damage = ini.Get_Int(RulesSection_CombatDamage, "IonCannonDamage", IonCannonDamage); // RA2: as above
		IonCannonDamage = ion_cannon_damage; // RA2: as above
		int railgun_damage_radius = ini.Get_Int(RulesSection_CombatDamage, "RailgunDamageRadius", RailgunDamageRadius); // RA2: as above
		RailgunDamageRadius = railgun_damage_radius; // RA2: as above
		int tiberium_explosion_damage = ini.Get_Int(RulesSection_CombatDamage, "TiberiumExplosionDamage", TiberiumExplosionDamage); // RA2: as above
		TiberiumExplosionDamage = tiberium_explosion_damage; // RA2: as above
		int tiberium_strength = ini.Get_Int(RulesSection_CombatDamage, "TiberiumStrength", TiberiumStrength); // RA2: as above
		TiberiumStrength = tiberium_strength; // RA2: as above
		Scorches = TGet_TypeList<SmudgeTypeClass>(ini, RulesSection_CombatDamage, "Scorches", Scorches); // RA2: as above
		Scorches1 = TGet_TypeList<SmudgeTypeClass>(ini, RulesSection_CombatDamage, "Scorches1", Scorches1); // RA2: as above
		Scorches2 = TGet_TypeList<SmudgeTypeClass>(ini, RulesSection_CombatDamage, "Scorches2", Scorches2); // RA2: as above
		Scorches3 = TGet_TypeList<SmudgeTypeClass>(ini, RulesSection_CombatDamage, "Scorches3", Scorches3); // RA2: as above
		Scorches4 = TGet_TypeList<SmudgeTypeClass>(ini, RulesSection_CombatDamage, "Scorches4", Scorches4); // RA2: as above
		SplashList = TGet_TypeList<AnimTypeClass>(ini, RulesSection_CombatDamage, "SplashList", SplashList); // RA2: as above
		FlameDamage = TGet_Class(ini, RulesSection_CombatDamage, "FlameDamage", FlameDamage); // RA2: as above
		FlameDamage2 = TGet_Class(ini, RulesSection_CombatDamage, "FlameDamage2", FlameDamage2); // RA2: as above
		C4Warhead = TGet_Class(ini, RulesSection_CombatDamage, "C4Warhead", C4Warhead); // RA2: as above
		V3Warhead = TGet_Class(ini, RulesSection_CombatDamage, "V3Warhead", V3Warhead); // RA2: as above
		DMislWarhead = TGet_Class(ini, RulesSection_CombatDamage, "DMislWarhead", DMislWarhead); // RA2: as above
		V3EliteWarhead = TGet_Class(ini, RulesSection_CombatDamage, "V3EliteWarhead", V3Warhead); // RA2: as above
		DMislEliteWarhead = TGet_Class(ini, RulesSection_CombatDamage, "DMislEliteWarhead", DMislWarhead); // RA2: as above
		IvanWarhead = TGet_Class(ini, RulesSection_CombatDamage, "IvanWarhead", IvanWarhead); // RA2: as above
		CanDetonateTimeBomb = ini.Get_Bool(RulesSection_CombatDamage, "CanDetonateTimeBomb", CanDetonateTimeBomb); // RA2: as above
		CanDetonateDeathBomb = ini.Get_Bool(RulesSection_CombatDamage, "CanDetonateDeathBomb", CanDetonateDeathBomb); // RA2: as above
		DeathWeapon = TGet_Class(ini, RulesSection_CombatDamage, "DeathWeapon", DeathWeapon); // RA2: as above
		int ivan_damage = ini.Get_Int(RulesSection_CombatDamage, "IvanDamage", IvanDamage); // RA2: as above
		IvanDamage = ivan_damage; // RA2: as above
		int ivan_timed_delay = ini.Get_Int(RulesSection_CombatDamage, "IvanTimedDelay", IvanTimedDelay); // RA2: as above
		IvanTimedDelay = ivan_timed_delay; // RA2: as above
		BOMBCURS_SHP = (ShapeSet const *)MixFileClass::Retrieve("BOMBCURS.SHP"); // RA2: as above
		int ivan_icon_flicker_rate = ini.Get_Int(RulesSection_CombatDamage, "IvanIconFlickerRate", IvanIconFlickerRate); // RA2: as above
		IvanIconFlickerRate = ivan_icon_flicker_rate; // RA2: as above
		int iron_curtain_duration = ini.Get_Int(RulesSection_CombatDamage, "IronCurtainDuration", IronCurtainDuration); // RA2: as above
		IronCurtainDuration = iron_curtain_duration; // RA2: as above
		IonCannonWarhead = TGet_Class(ini, RulesSection_CombatDamage, "IonCannonWarhead", IonCannonWarhead); // RA2: as above
		FirestormWarhead = TGet_Class(ini, RulesSection_CombatDamage, "FirestormWarhead", FirestormWarhead); // RA2: as above
		VeinholeWarhead = TGet_Class(ini, RulesSection_CombatDamage, "VeinholeWarhead", VeinholeWarhead); // RA2: as above
		DefaultFirestormExplosionSystem = TGet_Class(ini, RulesSection_CombatDamage, "DefaultFirestormExplosionSystem", DefaultFirestormExplosionSystem); // RA2: as above
		DefaultLargeGreySmokeSystem = TGet_Class(ini, RulesSection_CombatDamage, "DefaultLargeGreySmokeSystem", DefaultLargeGreySmokeSystem); // RA2: as above
		DefaultSmallGreySmokeSystem = TGet_Class(ini, RulesSection_CombatDamage, "DefaultSmallGreySmokeSystem", DefaultSmallGreySmokeSystem); // RA2: as above
		DefaultSparkSystem = TGet_Class(ini, RulesSection_CombatDamage, "DefaultSparkSystem", DefaultSparkSystem); // RA2: as above
		DefaultLargeRedSmokeSystem = TGet_Class(ini, RulesSection_CombatDamage, "DefaultLargeRedSmokeSystem", DefaultLargeRedSmokeSystem); // RA2: as above
		DefaultSmallRedSmokeSystem = TGet_Class(ini, RulesSection_CombatDamage, "DefaultSmallRedSmokeSystem", DefaultSmallRedSmokeSystem); // RA2: as above
		DefaultDebrisSmokeSystem = TGet_Class(ini, RulesSection_CombatDamage, "DefaultDebrisSmokeSystem", DefaultDebrisSmokeSystem); // RA2: as above
		DefaultFireStreamSystem = TGet_Class(ini, RulesSection_CombatDamage, "DefaultFireStreamSystem", DefaultFireStreamSystem); // RA2: as above
		DefaultTestParticleSystem = TGet_Class(ini, RulesSection_CombatDamage, "DefaultTestParticleSystem", DefaultTestParticleSystem); // RA2: as above
		DefaultRepairParticleSystem = TGet_Class(ini, RulesSection_CombatDamage, "DefaultRepairParticleSystem", DefaultRepairParticleSystem); // RA2: as above
		IsBerzerkAllowed = ini.Get_Bool(RulesSection_CombatDamage, "BerzerkAllowed", IsBerzerkAllowed); // RA2: as above
		TurboBoost = ini.Get_Float(RulesSection_CombatDamage, "TurboBoost", TurboBoost); // RA2: as above
		int atom_damage = ini.Get_Int(RulesSection_CombatDamage, "AtomDamage", AtomDamage); // RA2: as above
		AtomDamage = atom_damage; // RA2: as above
		int ballistic_scatter = ini.Get_Lepton(RulesSection_CombatDamage, "BallisticScatter", BallisticScatter); // RA2: as above
		BallisticScatter = ballistic_scatter; // RA2: as above
		int bridge_strength = ini.Get_Int(RulesSection_CombatDamage, "BridgeStrength", BridgeStrength); // RA2: as above
		BridgeStrength = bridge_strength; // RA2: as above
		C4Delay = ini.Get_Float(RulesSection_CombatDamage, "C4Delay", C4Delay); // RA2: as above
		int crush = ini.Get_Lepton(RulesSection_CombatDamage, "Crush", CrushDistance); // RA2: as above
		CrushDistance = crush; // RA2: as above
		ExplosionSpread = ini.Get_Float(RulesSection_CombatDamage, "ExpSpread", ExplosionSpread); // RA2: as above
		int fire_supress = ini.Get_Lepton(RulesSection_CombatDamage, "FireSupress", SupressRadius); // RA2: as above
		SupressRadius = fire_supress; // RA2: as above
		int homing_scatter = ini.Get_Lepton(RulesSection_CombatDamage, "HomingScatter", HomingScatter); // RA2: as above
		HomingScatter = homing_scatter; // RA2: as above
		int max_damage = ini.Get_Int(RulesSection_CombatDamage, "MaxDamage", MaxDamage); // RA2: as above
		MaxDamage = max_damage; // RA2: as above
		int min_damage = ini.Get_Int(RulesSection_CombatDamage, "MinDamage", MinDamage); // RA2: as above
		MinDamage = min_damage; // RA2: as above
		IsExplosiveHarvester = ini.Get_Bool(RulesSection_CombatDamage, "TiberiumExplosive", IsExplosiveHarvester); // RA2: as above
		IsAutoCrush = ini.Get_Bool(RulesSection_CombatDamage, "PlayerAutoCrush", IsAutoCrush); // RA2: as above
		IsSmartDefense = ini.Get_Bool(RulesSection_CombatDamage, "PlayerReturnFire", IsSmartDefense); // RA2: as above
		IsScatter = ini.Get_Bool(RulesSection_CombatDamage, "PlayerScatter", IsScatter); // RA2: as above
		IsTreeTarget = ini.Get_Bool(RulesSection_CombatDamage, "TreeTargeting", IsTreeTarget); // RA2: as above
		Incoming = ini.Get_MPHType(RulesSection_CombatDamage, "Incoming", Incoming); // RA2: as above
		int collapse_chance = ini.Get_Int(RulesSection_CombatDamage, "CollapseChance", CollapseChance); // RA2: as above
		CollapseChance = collapse_chance; // RA2: as above
		return(true);
	}
	return(false);
}
#endif



/// <summary>
/// Fetches the radiation control values.
/// These govern how long a radiation site lasts, how strong it can grow, how fast it
/// decays, the light and tint it casts on the terrain and the warhead it damages with.
/// </summary>
/// <returns>bool; Was a radiation section found in the control file?</returns>
bool RulesClass::Radiation(CCINIClass const & ini) // RA2: not in TibSun, at 0x647B60
{
	static char const * const RADIATION = "Radiation";
	if (ini.Is_Present(RADIATION)) {
		RadDurationMultiple = ini.Get_Int(RADIATION, "RadDurationMultiple", RadDurationMultiple);
		RadApplicationDelay = ini.Get_Int(RADIATION, "RadApplicationDelay", RadApplicationDelay);
		RadLevelMax = ini.Get_Int(RADIATION, "RadLevelMax", RadLevelMax);
		RadLevelDelay = ini.Get_Int(RADIATION, "RadLevelDelay", RadLevelDelay);
		RadLightDelay = ini.Get_Int(RADIATION, "RadLightDelay", RadLightDelay);
		RadLevelFactor = ini.Get_Float(RADIATION, "RadLevelFactor", RadLevelFactor);
		RadLightFactor = ini.Get_Float(RADIATION, "RadLightFactor", RadLightFactor);
		RadTintFactor = ini.Get_Float(RADIATION, "RadTintFactor", RadTintFactor);
		RadColor = ini.Get_RGBClass(RADIATION, "RadColor", RadColor);
		RadSiteWarhead = TGet_Class(ini, RADIATION, "RadSiteWarhead", RadSiteWarhead);
		return(true);
	}
	return(false);
}


/// <summary>
/// Fetches the elevation model values.
/// Units on higher ground gain range in steps of the elevation increment, up to the cap.
/// </summary>
/// <returns>bool; Was an elevation model section found in the control file?</returns>
bool RulesClass::Elevation_Model(CCINIClass const & ini) // RA2: not in TibSun, at 0x647D40
{
	static char const * const ELEVATIONMODEL = "ElevationModel";
	if (ini.Is_Present(ELEVATIONMODEL)) {
		ElevationIncrement = ini.Get_Int(ELEVATIONMODEL, "ElevationIncrement", ElevationIncrement);
		ElevationIncrementBonus = ini.Get_Float(ELEVATIONMODEL, "ElevationIncrementBonus", ElevationIncrementBonus);
		ElevationBonusCap = ini.Get_Float(ELEVATIONMODEL, "ElevationBonusCap", ElevationBonusCap);
		return(true);
	}
	return(false);
}


/// <summary>
/// Fetches the wall model values.
/// These say whether allied walls let fire through and how strong a weapon must be to
/// punch through a wall.
/// </summary>
/// <returns>bool; Was a wall model section found in the control file?</returns>
bool RulesClass::Wall_Model(CCINIClass const & ini) // RA2: not in TibSun, at 0x647DE0
{
	static char const * const WALLMODEL = "WallModel";
	if (ini.Is_Present(WALLMODEL)) {
		AlliedWallTransparency = ini.Get_Bool(WALLMODEL, "AlliedWallTransparency", AlliedWallTransparency);
		WallPenetratorThreshold = ini.Get_Float(WALLMODEL, "WallPenetratorThreshold", WallPenetratorThreshold);
		return(true);
	}
	return(false);
}











/***********************************************************************************************
 * Difficulty_Get -- Fetch the difficulty bias values.                                         *
 *                                                                                             *
 *    This will fetch the difficulty bias values for the section specified.                    *
 *                                                                                             *
 * INPUT:   ini   -- Reference the INI database to fetch the values from.                      *
 *                                                                                             *
 *          diff  -- Reference to the difficulty class object to fill in with the values.      *
 *                                                                                             *
 *          section  -- The section identifier to lift the values from.                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/11/1996 JLB : Created.                                                                 *
 *=============================================================================================*/



/// <summary>
/// Creates the color schemes declared in the control file.
/// Each entry names a scheme and gives the hue, saturation and value that the game palette
/// is to be remapped through. Both a flat scheme and a full intensity ramp are built for
/// every entry, since the drawing code asks for whichever suits the object being drawn.
/// </summary>
/// <returns>bool; Were any color schemes declared?</returns>
bool RulesClass::Color_Schemes(CCINIClass const & ini)
{
	static char const * const COLORS = "Colors";
	if (ini.Is_Present(COLORS)) {
		int count = ini.Entry_Count(COLORS);
		for (int i = 0; i < count; i++) {
			char const * entry = ini.Get_Entry(COLORS, i); // RA2: the entry name is kept in a register
			HSVClass hsv = ini.Get_HSVClass(COLORS, entry, HSVClass()); // RA2: as above
			SchemeLibrary_Add(entry, hsv); // RA2: every scheme is also recorded in the scheme library (0x603A60)
			ColorScheme::Find_Or_Make(entry, hsv, SchemePalette, GamePalette, 1); // RA2: as above
			ColorScheme::Find_Or_Make(entry, hsv, SchemePalette, GamePalette, 53); // RA2: 53 intensity levels, not TibSun's NUM_INTENSITY_LEVELS (63)
		}
		return(true);
	}
	return(false);
}






/***********************************************************************************************
 * RulesClass::General -- Process the general main game rules.                                 *
 *                                                                                             *
 *    This fetches the control constants uses for regular game processing. Any game behavior   *
 *    controlling values that don't properly fit in any of the other catagories will be        *
 *    stored here.                                                                             *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the database to fetch the values from.                       *
 *                                                                                             *
 * OUTPUT:  bool; Was the general section found and processed?                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: 5734 bytes differ (adopted text)
bool RulesClass::General(CCINIClass const & ini)
{
	static char const * const RulesSection_General = "General"; // RA2: the executable's section name, a file-static pointer at 0x007A9058
	if (ini.Is_Present(RulesSection_General)) { // RA2: as above
		LargeVisceroid = TGet_Class(ini, RulesSection_General, "LargeVisceroid", LargeVisceroid); // RA2: as above
		SmallVisceroid = TGet_Class(ini, RulesSection_General, "SmallVisceroid", SmallVisceroid); // RA2: as above
		TiberiumHeal = ini.Get_Float(RulesSection_General, "TiberiumHeal", TiberiumHeal); // RA2: as above
		PrerequisitePower = CCINIClass::Get_BuildingType_List(ini, RulesSection_General, "PrerequisitePower", PrerequisitePower); // RA2: as above
		PrerequisiteFactory = CCINIClass::Get_BuildingType_List(ini, RulesSection_General, "PrerequisiteFactory", PrerequisiteFactory); // RA2: as above
		PrerequisiteBarracks = CCINIClass::Get_BuildingType_List(ini, RulesSection_General, "PrerequisiteBarracks", PrerequisiteBarracks); // RA2: as above
		PrerequisiteRadar = CCINIClass::Get_BuildingType_List(ini, RulesSection_General, "PrerequisiteRadar", PrerequisiteRadar); // RA2: as above
		PrerequisiteTech = CCINIClass::Get_BuildingType_List(ini, RulesSection_General, "PrerequisiteTech", PrerequisiteTech); // RA2: as above
		PrerequisiteProc = CCINIClass::Get_BuildingType_List(ini, RulesSection_General, "PrerequisiteProc", PrerequisiteProc); // RA2: as above
		ZoomInFactor = ini.Get_Float(RulesSection_General, "ZoomInFactor", ZoomInFactor); // RA2: as above
		IsRevealByHeight = ini.Get_Bool(RulesSection_General, "RevealByHeight", IsRevealByHeight); // RA2: as above
		IsShroudedSubteranneanMovesAllowed = ini.Get_Bool(RulesSection_General, "AllowShroudedSubteranneanMoves", IsShroudedSubteranneanMovesAllowed); // RA2: as above
		AircraftFogReveal = ini.Get_Int(RulesSection_General, "AircraftFogReveal", AircraftFogReveal); // RA2: as above
		MinLowPowerProductionSpeed = ini.Get_Float(RulesSection_General, "MinLowPowerProductionSpeed", MinLowPowerProductionSpeed); // RA2: as above
		MaxLowPowerProductionSpeed = ini.Get_Float(RulesSection_General, "MaxLowPowerProductionSpeed", MaxLowPowerProductionSpeed); // RA2: as above
		LowPowerPenaltyModifier = ini.Get_Float(RulesSection_General, "LowPowerPenaltyModifier", LowPowerPenaltyModifier); // RA2: as above
		MultipleFactory = ini.Get_Float(RulesSection_General, "MultipleFactory", MultipleFactory); // RA2: as above
		MaximumCheerRate = ini.Get_Int(RulesSection_General, "MaximumCheerRate", MaximumCheerRate); // RA2: as above
		TreeFlammability = ini.Get_Float(RulesSection_General, "TreeFlammability", TreeFlammability); // RA2: as above
		MissileROTVar = ini.Get_Float(RulesSection_General, "MissileROTVar", MissileROTVar); // RA2: as above
		MissileSafetyAltitude = ini.Get_Int(RulesSection_General, "MissileSafetyAltitude", MissileSafetyAltitude); // RA2: as above
		MissileSpeedVar = ini.Get_Float(RulesSection_General, "MissileSpeedVar", MissileSpeedVar); // RA2: as above
		DropPodWeapon = TGet_Class(ini, RulesSection_General, "DropPodWeapon", DropPodWeapon); // RA2: as above
		DropPodHeight = ini.Get_Int(RulesSection_General, "DropPodHeight", DropPodHeight); // RA2: as above
		DropPodSpeed = ini.Get_Int(RulesSection_General, "DropPodSpeed", DropPodSpeed); // RA2: as above
		DropPodAngle = ini.Get_Float(RulesSection_General, "DropPodAngle", DropPodAngle); // RA2: as above
		DropPodAngle = MIN(DropPodAngle, DEG_TO_RAD(67.5));
		DropPodAngle = MAX(DropPodAngle, DEG_TO_RAD(22.5));
		CrewEscape = ini.Get_Float(RulesSection_General, "CrewEscape", CrewEscape); // RA2: the executable's General reader: RA2's keys and member order
		TunnelSpeed = ini.Get_Float(RulesSection_General, "TunnelSpeed", TunnelSpeed); // RA2: as above
		HoverDampen = ini.Get_Float(RulesSection_General, "HoverDampen", HoverDampen); // RA2: as above
		HoverBob = ini.Get_Float(RulesSection_General, "HoverBob", HoverBob); // RA2: as above
		HoverHeight = ini.Get_Int(RulesSection_General, "HoverHeight", HoverHeight); // RA2: as above
		HoverBoost = ini.Get_Float(RulesSection_General, "HoverBoost", HoverBoost); // RA2: as above
		HoverAcceleration = ini.Get_Float(RulesSection_General, "HoverAcceleration", HoverAcceleration); // RA2: as above
		HoverBrake = ini.Get_Float(RulesSection_General, "HoverBrake", HoverBrake); // RA2: as above
		VeteranRatio = ini.Get_Float(RulesSection_General, "VeteranRatio", VeteranRatio); // RA2: as above
		VeteranCombat = ini.Get_Float(RulesSection_General, "VeteranCombat", VeteranCombat); // RA2: as above
		VeteranSpeed = ini.Get_Float(RulesSection_General, "VeteranSpeed", VeteranSpeed); // RA2: as above
		VeteranSight = ini.Get_Float(RulesSection_General, "VeteranSight", VeteranSight); // RA2: as above
		VeteranArmor = ini.Get_Float(RulesSection_General, "VeteranArmor", VeteranArmor); // RA2: as above
		VeteranROF = ini.Get_Float(RulesSection_General, "VeteranROF", VeteranROF); // RA2: as above
		VeteranCap = ini.Get_Float(RulesSection_General, "VeteranCap", VeteranCap); // RA2: as above
		ExplosiveVoxelDebris = TGet_TypeList<VoxelAnimTypeClass>(ini, RulesSection_General, "ExplosiveVoxelDebris", ExplosiveVoxelDebris); // RA2: as above
		BridgeVoxelMax = ini.Get_Int(RulesSection_General, "BridgeVoxelMax", BridgeVoxelMax); // RA2: as above
		TireVoxelDebris = TGet_Class(ini, RulesSection_General, "TireVoxelDebris", TireVoxelDebris); // RA2: as above
		ScrapVoxelDebris = TGet_Class(ini, RulesSection_General, "ScrapVoxelDebris", ScrapVoxelDebris); // RA2: as above
		CloakingStages = ini.Get_Int(RulesSection_General, "CloakingStages", CloakingStages); // RA2: as above
		ShipSinkingWeight = ini.Get_Float(RulesSection_General, "ShipSinkingWeight", ShipSinkingWeight); // RA2: as above
		IceCrackingWeight = ini.Get_Float(RulesSection_General, "IceCrackingWeight", IceCrackingWeight); // RA2: as above
		IceBreakingWeight = ini.Get_Float(RulesSection_General, "IceBreakingWeight", IceBreakingWeight); // RA2: as above
		CliffBackImpassability = ini.Get_Int(RulesSection_General, "CliffBackImpassability", CliffBackImpassability); // RA2: as above
		PlacementDelay = ini.Get_Float(RulesSection_General, "PlacementDelay", PlacementDelay); // RA2: as above
		TrackedUphill = ini.Get_Float(RulesSection_General, "TrackedUphill", TrackedUphill); // RA2: as above
		TrackedDownhill = ini.Get_Float(RulesSection_General, "TrackedDownhill", TrackedDownhill); // RA2: as above
		WheeledUphill = ini.Get_Float(RulesSection_General, "WheeledUphill", WheeledUphill); // RA2: as above
		WheeledDownhill = ini.Get_Float(RulesSection_General, "WheeledDownhill", WheeledDownhill); // RA2: as above
		WindDirection = (FacingType)ini.Get_Int(RulesSection_General, "WindDirection", WindDirection); // RA2: as above
		CameraRange = ini.Get_Lepton(RulesSection_General, "CameraRange", CameraRange); // RA2: as above
		FlightLevel = ini.Get_Int(RulesSection_General, "FlightLevel", FlightLevel); // RA2: as above
		ParachuteMaxFallRate = ini.Get_Int(RulesSection_General, "ParachuteMaxFallRate", ParachuteMaxFallRate); // RA2: as above
		NoParachuteMaxFallRate = ini.Get_Int(RulesSection_General, "NoParachuteMaxFallRate", NoParachuteMaxFallRate); // RA2: as above
		RepairBay = TGet_Class(ini, RulesSection_General, "RepairBay", RepairBay); // RA2: as above
		GDIGateOne = TGet_Class(ini, RulesSection_General, "GDIGateOne", GDIGateOne); // RA2: as above
		GDIGateTwo = TGet_Class(ini, RulesSection_General, "GDIGateTwo", GDIGateTwo); // RA2: as above
		NodGateOne = TGet_Class(ini, RulesSection_General, "NodGateOne", NodGateOne); // RA2: as above
		NodGateTwo = TGet_Class(ini, RulesSection_General, "NodGateTwo", NodGateTwo); // RA2: as above
		WallTower = TGet_Class(ini, RulesSection_General, "WallTower", WallTower); // RA2: as above
		Shipyard = TGet_TypeList<BuildingTypeClass>(ini, RulesSection_General, "Shipyard", Shipyard); // RA2: as above
		GDIPowerPlant = TGet_Class(ini, RulesSection_General, "GDIPowerPlant", GDIPowerPlant); // RA2: as above
		GDIPowerTurbine = TGet_Class(ini, RulesSection_General, "GDIPowerTurbine", GDIPowerTurbine); // RA2: as above
		NodRegularPower = TGet_Class(ini, RulesSection_General, "NodRegularPower", NodRegularPower); // RA2: as above
		NodAdvancedPower = TGet_Class(ini, RulesSection_General, "NodAdvancedPower", NodAdvancedPower); // RA2: as above
		GDIFirestormGenerator = TGet_Class(ini, RulesSection_General, "GDIFirestormGenerator", GDIFirestormGenerator); // RA2: as above
		GDIHunterSeeker = TGet_Class(ini, RulesSection_General, "GDIHunterSeeker", GDIHunterSeeker); // RA2: as above
		NodHunterSeeker = TGet_Class(ini, RulesSection_General, "NodHunterSeeker", NodHunterSeeker); // RA2: as above
		BaseUnit = TGet_TypeList<UnitTypeClass>(ini, RulesSection_General, "BaseUnit", BaseUnit); // RA2: as above
		HarvesterUnit = TGet_TypeList<UnitTypeClass>(ini, RulesSection_General, "HarvesterUnit", HarvesterUnit); // RA2: as above
		PadAircraft = TGet_TypeList<AircraftTypeClass>(ini, RulesSection_General, "PadAircraft", PadAircraft); // RA2: as above
		Paratrooper = TGet_Class(ini, RulesSection_General, "Paratrooper", Paratrooper); // RA2: as above
		ChronoDelay = ini.Get_Int(RulesSection_General, "ChronoDelay", ChronoDelay); // RA2: as above
		ChronoReinfDelay = ini.Get_Int(RulesSection_General, "ChronoReinfDelay", ChronoReinfDelay); // RA2: as above
		ChronoDistanceFactor = ini.Get_Int(RulesSection_General, "ChronoDistanceFactor", ChronoDistanceFactor); // RA2: as above
		ChronoTrigger = ini.Get_Bool(RulesSection_General, "ChronoTrigger", ChronoTrigger); // RA2: as above
		ChronoMinimumDelay = ini.Get_Int(RulesSection_General, "ChronoMinimumDelay", ChronoMinimumDelay); // RA2: as above
		ChronoRangeMinimum = ini.Get_Int(RulesSection_General, "ChronoRangeMinimum", ChronoRangeMinimum); // RA2: as above
		AlliedDisguise = TGet_Class(ini, RulesSection_General, "AlliedDisguise", AlliedDisguise); // RA2: as above
		SovietDisguise = TGet_Class(ini, RulesSection_General, "SovietDisguise", SovietDisguise); // RA2: as above
		SpyPowerBlackout = ini.Get_Int(RulesSection_General, "SpyPowerBlackout", SpyPowerBlackout); // RA2: as above
		SpyMoneyStealPercent = ini.Get_Float(RulesSection_General, "SpyMoneyStealPercent", SpyMoneyStealPercent); // RA2: as above
		AttackCursorOnDisguise = ini.Get_Bool(RulesSection_General, "AttackCursorOnDisguise", AttackCursorOnDisguise); // RA2: as above
		AIMinorSuperReadyPercent = ini.Get_Float(RulesSection_General, "AIMinorSuperReadyPercent", AIMinorSuperReadyPercent); // RA2: as above
		AISafeDistance = ini.Get_Int(RulesSection_General, "AISafeDistance", AISafeDistance); // RA2: as above
		AINavalYardAdjacency = ini.Get_Int(RulesSection_General, "AINavalYardAdjacency", AINavalYardAdjacency); // RA2: as above
		MaximumBuildingPlacementFailures = ini.Get_Int(RulesSection_General, "MaximumBuildingPlacementFailures", MaximumBuildingPlacementFailures); // RA2: as above
		HarvesterTooFarDistance = ini.Get_Int(RulesSection_General, "HarvesterTooFarDistance", HarvesterTooFarDistance); // RA2: as above
		ApproachTargetResetMultiplier = ini.Get_Int(RulesSection_General, "ApproachTargetResetMultiplier", ApproachTargetResetMultiplier); // RA2: as above
		CampaignMoneyDeltaEasy = ini.Get_Int(RulesSection_General, "CampaignMoneyDeltaEasy", CampaignMoneyDeltaEasy); // RA2: as above
		CampaignMoneyDeltaHard = ini.Get_Int(RulesSection_General, "CampaignMoneyDeltaHard", CampaignMoneyDeltaHard); // RA2: as above
		GuardAreaTargetingDelay = ini.Get_Int(RulesSection_General, "GuardAreaTargetingDelay", GuardAreaTargetingDelay); // RA2: as above
		NormalTargetingDelay = ini.Get_Int(RulesSection_General, "NormalTargetingDelay", NormalTargetingDelay); // RA2: as above
		ChronoHarvTooFarDistance = ini.Get_Int(RulesSection_General, "ChronoHarvTooFarDistance", ChronoHarvTooFarDistance); // RA2: as above
		AIRestrictReplaceTime = ini.Get_Int(RulesSection_General, "AIRestrictReplaceTime", AIRestrictReplaceTime); // RA2: as above
		ThreatPerOccupant = ini.Get_Int(RulesSection_General, "ThreatPerOccupant", ThreatPerOccupant); // RA2: as above
		PurifierBonus = ini.Get_Float(RulesSection_General, "PurifierBonus", PurifierBonus); // RA2: as above
		Engineer = TGet_Class(ini, RulesSection_General, "Engineer", Engineer); // RA2: as above
		Technician = TGet_Class(ini, RulesSection_General, "Technician", Technician); // RA2: as above
		Pilot = TGet_Class(ini, RulesSection_General, "Pilot", Pilot); // RA2: as above
		AlliedCrew = TGet_Class(ini, RulesSection_General, "AlliedCrew", AlliedCrew); // RA2: as above
		SovietCrew = TGet_Class(ini, RulesSection_General, "SovietCrew", SovietCrew); // RA2: as above
		IsCurleyShuffle = ini.Get_Bool(RulesSection_General, "CurleyShuffle", IsCurleyShuffle); // RA2: as above
		IsFineDifficulty = ini.Get_Bool(RulesSection_General, "FineDiffControl", IsFineDifficulty); // RA2: as above
		TeamDelays = ini.Get_IntList(RulesSection_General, "TeamDelays", TeamDelays); // RA2: as above
		AlliedBaseDefenseCounts = ini.Get_IntList(RulesSection_General, "AlliedBaseDefenseCounts", AlliedBaseDefenseCounts); // RA2: as above
		SovietBaseDefenseCounts = ini.Get_IntList(RulesSection_General, "SovietBaseDefenseCounts", SovietBaseDefenseCounts); // RA2: as above
		DisabledDisguiseDetectionPercent = ini.Get_IntList(RulesSection_General, "DisabledDisguiseDetectionPercent", DisabledDisguiseDetectionPercent); // RA2: as above
		AIAutoDeployFrameDelay = ini.Get_IntList(RulesSection_General, "AIAutoDeployFrameDelay", AIAutoDeployFrameDelay); // RA2: as above
		AIPickWallDefensePercent = ini.Get_IntList(RulesSection_General, "AIPickWallDefensePercent", AIPickWallDefensePercent); // RA2: as above
		AIHateDelays = ini.Get_IntList(RulesSection_General, "AIHateDelays", AIHateDelays); // RA2: as above
		AIAlternateProductionCreditCutoff = ini.Get_Int(RulesSection_General, "AIAlternateProductionCreditCutoff", AIAlternateProductionCreditCutoff); // RA2: as above
		AIUseTurbineUpgradeChance = ini.Get_Float(RulesSection_General, "AIUseTurbineUpgradeProbability", AIUseTurbineUpgradeChance); // RA2: as above
		NodAIBuildsWalls = ini.Get_Bool(RulesSection_General, "NodAIBuildsWalls", NodAIBuildsWalls); // RA2: as above
		AIBuildsWalls = ini.Get_Bool(RulesSection_General, "AIBuildsWalls", AIBuildsWalls); // RA2: as above
		FillEarliestTeamProbability = ini.Get_IntList(RulesSection_General, "FillEarliestTeamProbability", FillEarliestTeamProbability); // RA2: as above
		MinimumAIDefensiveTeams = ini.Get_IntList(RulesSection_General, "MinimumAIDefensiveTeams", MinimumAIDefensiveTeams); // RA2: as above
		MaximumAIDefensiveTeams = ini.Get_IntList(RulesSection_General, "MaximumAIDefensiveTeams", MaximumAIDefensiveTeams); // RA2: as above
		TotalAITeamCap = ini.Get_IntList(RulesSection_General, "TotalAITeamCap", TotalAITeamCap); // RA2: as above
		UseMinDefenseRule = ini.Get_Bool(RulesSection_General, "UseMinDefenseRule", UseMinDefenseRule); // RA2: as above
		DissolveUnfilledTeamDelay = ini.Get_Int(RulesSection_General, "DissolveUnfilledTeamDelay", DissolveUnfilledTeamDelay); // RA2: as above
		MultiplayerAICreditMultipliers = ini.Get_IntList(RulesSection_General, "MultiplayerAICM", MultiplayerAICreditMultipliers); // RA2: as above
		AIVirtualPurifiers = ini.Get_IntList(RulesSection_General, "AIVirtualPurifiers", AIVirtualPurifiers); // RA2: as above
		AmerParaDropInf = TGet_TypeList<InfantryTypeClass>(ini, RulesSection_General, "AmerParaDropInf", AmerParaDropInf); // RA2: as above
		AmerParaDropNum = ini.Get_IntList(RulesSection_General, "AmerParaDropNum", AmerParaDropNum); // RA2: as above
		AllyParaDropInf = TGet_TypeList<InfantryTypeClass>(ini, RulesSection_General, "AllyParaDropInf", AllyParaDropInf); // RA2: as above
		AllyParaDropNum = ini.Get_IntList(RulesSection_General, "AllyParaDropNum", AllyParaDropNum); // RA2: as above
		SovParaDropInf = TGet_TypeList<InfantryTypeClass>(ini, RulesSection_General, "SovParaDropInf", SovParaDropInf); // RA2: as above
		SovParaDropNum = ini.Get_IntList(RulesSection_General, "SovParaDropNum", SovParaDropNum); // RA2: as above
		AIIonCannonConYardValue = ini.Get_IntList(RulesSection_General, "AIIonCannonConYardValue", AIIonCannonConYardValue); // RA2: as above
		AIIonCannonWarFactoryValue = ini.Get_IntList(RulesSection_General, "AIIonCannonWarFactoryValue", AIIonCannonWarFactoryValue); // RA2: as above
		AIIonCannonPowerValue = ini.Get_IntList(RulesSection_General, "AIIonCannonPowerValue", AIIonCannonPowerValue); // RA2: as above
		AIIonCannonEngineerValue = ini.Get_IntList(RulesSection_General, "AIIonCannonEngineerValue", AIIonCannonEngineerValue); // RA2: as above
		AIIonCannonThiefValue = ini.Get_IntList(RulesSection_General, "AIIonCannonThiefValue", AIIonCannonThiefValue); // RA2: as above
		AIIonCannonHarvesterValue = ini.Get_IntList(RulesSection_General, "AIIonCannonHarvesterValue", AIIonCannonHarvesterValue); // RA2: as above
		AIIonCannonMCVValue = ini.Get_IntList(RulesSection_General, "AIIonCannonMCVValue", AIIonCannonMCVValue); // RA2: as above
		AIIonCannonAPCValue = ini.Get_IntList(RulesSection_General, "AIIonCannonAPCValue", AIIonCannonAPCValue); // RA2: as above
		AIIonCannonBaseDefenseValue = ini.Get_IntList(RulesSection_General, "AIIonCannonBaseDefenseValue", AIIonCannonBaseDefenseValue); // RA2: as above
		AIIonCannonPlugValue = ini.Get_IntList(RulesSection_General, "AIIonCannonPlugValue", AIIonCannonPlugValue); // RA2: as above
		AIIonCannonHelipadValue = ini.Get_IntList(RulesSection_General, "AIIonCannonHelipadValue", AIIonCannonHelipadValue); // RA2: as above
		AIIonCannonTempleValue = ini.Get_IntList(RulesSection_General, "AIIonCannonTempleValue", AIIonCannonTempleValue); // RA2: as above
		CloakDelay = ini.Get_Float(RulesSection_General, "CloakDelay", CloakDelay); // RA2: as above
		GameSpeedBias = ini.Get_Float(RulesSection_General, "GameSpeedBias", GameSpeedBias); // RA2: as above
		NervousBias = ini.Get_Float(RulesSection_General, "BaseBias", NervousBias); // RA2: as above
		IsSeparate = ini.Get_Bool(RulesSection_General, "SeparateAircraft", IsSeparate); // RA2: as above
		BaseDefenseDelay = ini.Get_Float(RulesSection_General, "BaseDefenseDelay", BaseDefenseDelay); // RA2: as above
		SuspendPriority = ini.Get_Int(RulesSection_General, "SuspendPriority", SuspendPriority); // RA2: as above
		SuspendDelay = ini.Get_Float(RulesSection_General, "SuspendDelay", SuspendDelay); // RA2: as above
		SurvivorFraction = ini.Get_Float(RulesSection_General, "SurvivorRate", SurvivorFraction); // RA2: as above
		AlliedSurvivorDivisor = ini.Get_Int(RulesSection_General, "AlliedSurvivorDivisor", AlliedSurvivorDivisor); // RA2: as above
		SovietSurvivorDivisor = ini.Get_Int(RulesSection_General, "SovietSurvivorDivisor", SovietSurvivorDivisor); // RA2: as above
		ReloadRate = ini.Get_Float(RulesSection_General, "ReloadRate", ReloadRate); // RA2: as above
		BuildupTime = ini.Get_Float(RulesSection_General, "BuildupTime", BuildupTime); // RA2: as above
		HarvesterDumpRate = ini.Get_Float(RulesSection_General, "HarvesterDumpRate", HarvesterDumpRate); // RA2: as above
		HarvesterLoadRate = ini.Get_Int(RulesSection_General, "HarvesterLoadRate", HarvesterLoadRate); // RA2: as above
		BuildSpeedBias = ini.Get_Float(RulesSection_General, "BuildSpeed", BuildSpeedBias); // RA2: as above
		DamageDelay = ini.Get_Float(RulesSection_General, "DamageDelay", DamageDelay); // RA2: as above
		GrowthRate = ini.Get_Float(RulesSection_General, "GrowthRate", GrowthRate); // RA2: as above
		RefundPercent = ini.Get_Float(RulesSection_General, "RefundPercent", RefundPercent); // RA2: as above
		RepairPercent = ini.Get_Float(RulesSection_General, "RepairPercent", RepairPercent); // RA2: as above
		RepairStep = ini.Get_Int(RulesSection_General, "RepairStep", RepairStep); // RA2: as above
		IRepairStep = ini.Get_Int(RulesSection_General, "IRepairStep", IRepairStep); // RA2: as above
		RepairRate = ini.Get_Float(RulesSection_General, "RepairRate", RepairRate); // RA2: as above
		URepairRate = ini.Get_Float(RulesSection_General, "URepairRate", URepairRate); // RA2: as above
		IRepairRate = ini.Get_Float(RulesSection_General, "IRepairRate", IRepairRate); // RA2: as above
		StrayDistance = ini.Get_Lepton(RulesSection_General, "Stray", StrayDistance); // RA2: as above
		RelaxedStray = ini.Get_Lepton(RulesSection_General, "RelaxedStray", RelaxedStray); // RA2: as above
		GuardModeStray = ini.Get_Lepton(RulesSection_General, "GuardModeStray", GuardModeStray); // RA2: as above
		CloseEnoughDistance = ini.Get_Lepton(RulesSection_General, "CloseEnough", CloseEnoughDistance); // RA2: as above
		IsBlendedFog = ini.Get_Bool(RulesSection_General, "BlendedFog", IsBlendedFog); // RA2: as above
		AttackingAircraftSightRange = ini.Get_Int(RulesSection_General, "AttackingAircraftSightRange", AttackingAircraftSightRange); // RA2: as above
		LeptonsPerSightIncrease = ini.Get_Int(RulesSection_General, "LeptonsPerSightIncrease", LeptonsPerSightIncrease); // RA2: as above
		TiberiumTransmogrify = ini.Get_Int(RulesSection_General, "TiberiumTransmogrify", TiberiumTransmogrify); // RA2: as above
		LightningDeferment = ini.Get_Int(RulesSection_General, "LightningDeferment", LightningDeferment); // RA2: as above
		LightningDamage = ini.Get_Int(RulesSection_General, "LightningDamage", LightningDamage); // RA2: as above
		LightningStormDuration = ini.Get_Int(RulesSection_General, "LightningStormDuration", LightningStormDuration); // RA2: as above
		LightningHitDelay = ini.Get_Int(RulesSection_General, "LightningHitDelay", LightningHitDelay); // RA2: as above
		LightningScatterDelay = ini.Get_Int(RulesSection_General, "LightningScatterDelay", LightningScatterDelay); // RA2: as above
		LightningCellSpread = ini.Get_Int(RulesSection_General, "LightningCellSpread", LightningCellSpread); // RA2: as above
		LightningSeparation = ini.Get_Int(RulesSection_General, "LightningSeparation", LightningSeparation); // RA2: as above
		LightningWarhead = TGet_Class(ini, RulesSection_General, "LightningWarhead", LightningWarhead);  /* out of line 653da0 */ // RA2: as above
		LightningPrintText = ini.Get_Bool(RulesSection_General, "LightningPrintText", LightningPrintText); // RA2: as above
		PrismType = TGet_Class(ini, RulesSection_General, "PrismType", PrismType);  /* out of line 654060 */ // RA2: as above
		PrismSupportModifier = ini.Get_Float(RulesSection_General, "PrismSupportModifier", PrismSupportModifier) * 100.0; // RA2: as above
		PrismSupportMax = ini.Get_Int(RulesSection_General, "PrismSupportMax", PrismSupportMax); // RA2: as above
		PrismSupportDelay = ini.Get_Int(RulesSection_General, "PrismSupportDelay", PrismSupportDelay); // RA2: as above
		PrismSupportDuration = ini.Get_Int(RulesSection_General, "PrismSupportDuration", PrismSupportDuration); // RA2: as above
		PrismSupportHeight = ini.Get_Int(RulesSection_General, "PrismSupportHeight", PrismSupportHeight); // RA2: as above
		V3Rocket.PauseFrames = ini.Get_Int(RulesSection_General, "V3RocketPauseFrames", V3Rocket.PauseFrames); // RA2: as above
		V3Rocket.TiltFrames = ini.Get_Int(RulesSection_General, "V3RocketTiltFrames", V3Rocket.TiltFrames); // RA2: as above
		V3Rocket.PitchInitial = ini.Get_Float(RulesSection_General, "V3RocketPitchInitial", V3Rocket.PitchInitial); // RA2: as above
		V3Rocket.PitchFinal = ini.Get_Float(RulesSection_General, "V3RocketPitchFinal", V3Rocket.PitchFinal); // RA2: as above
		V3Rocket.TurnRate = ini.Get_Float(RulesSection_General, "V3RocketTurnRate", V3Rocket.TurnRate); // RA2: as above
		V3Rocket.Acceleration = ini.Get_Float(RulesSection_General, "V3RocketAcceleration", V3Rocket.Acceleration); // RA2: as above
		V3Rocket.Altitude = ini.Get_Int(RulesSection_General, "V3RocketAltitude", V3Rocket.Altitude); // RA2: as above
		V3Rocket.Damage = ini.Get_Int(RulesSection_General, "V3RocketDamage", V3Rocket.Damage); // RA2: as above
		V3Rocket.EliteDamage = ini.Get_Int(RulesSection_General, "V3RocketEliteDamage", V3Rocket.EliteDamage); // RA2: as above
		V3Rocket.BodyLength = ini.Get_Int(RulesSection_General, "V3RocketBodyLength", V3Rocket.BodyLength); // RA2: as above
		V3Rocket.LazyCurve = ini.Get_Bool(RulesSection_General, "V3RocketLazyCurve", V3Rocket.LazyCurve); // RA2: as above
		V3Rocket.Type = TGet_Class(ini, RulesSection_General, "V3RocketType", V3Rocket.Type);  /* out of line 6540b0 */ // RA2: as above
		DMisl.PauseFrames = ini.Get_Int(RulesSection_General, "DMislPauseFrames", DMisl.PauseFrames); // RA2: as above
		DMisl.TiltFrames = ini.Get_Int(RulesSection_General, "DMislTiltFrames", DMisl.TiltFrames); // RA2: as above
		DMisl.PitchInitial = ini.Get_Float(RulesSection_General, "DMislPitchInitial", DMisl.PitchInitial); // RA2: as above
		DMisl.PitchFinal = ini.Get_Float(RulesSection_General, "DMislPitchFinal", DMisl.PitchFinal); // RA2: as above
		DMisl.TurnRate = ini.Get_Float(RulesSection_General, "DMislTurnRate", DMisl.TurnRate); // RA2: as above
		DMisl.Acceleration = ini.Get_Float(RulesSection_General, "DMislAcceleration", DMisl.Acceleration); // RA2: as above
		DMisl.Altitude = ini.Get_Int(RulesSection_General, "DMislAltitude", DMisl.Altitude); // RA2: as above
		DMisl.Damage = ini.Get_Int(RulesSection_General, "DMislDamage", DMisl.Damage); // RA2: as above
		DMisl.EliteDamage = ini.Get_Int(RulesSection_General, "DMislEliteDamage", DMisl.EliteDamage); // RA2: as above
		DMisl.BodyLength = ini.Get_Int(RulesSection_General, "DMislBodyLength", DMisl.BodyLength); // RA2: as above
		DMisl.LazyCurve = ini.Get_Bool(RulesSection_General, "DMislLazyCurve", DMisl.LazyCurve); // RA2: as above
		DMisl.Type = TGet_Class(ini, RulesSection_General, "DMislType", DMisl.Type);  /* out of line 6540b0 */ // RA2: as above
		ParadropRadius = ini.Get_Int(RulesSection_General, "ParadropRadius", ParadropRadius); // RA2: as above
		SpotlightMovementRadius = ini.Get_Int(RulesSection_General, "SpotlightMovementRadius", SpotlightMovementRadius); // RA2: as above
		SpotlightLocationRadius = ini.Get_Int(RulesSection_General, "SpotlightLocationRadius", SpotlightLocationRadius); // RA2: as above
		SpotlightSpeed = ini.Get_Float(RulesSection_General, "SpotlightSpeed", SpotlightSpeed); // RA2: as above
		SpotlightAcceleration = ini.Get_Float(RulesSection_General, "SpotlightAcceleration", SpotlightAcceleration); // RA2: as above
		SpotlightAngle = ini.Get_Float(RulesSection_General, "SpotlightAngle", SpotlightAngle); // RA2: as above
		SpotlightRadius = ini.Get_Int(RulesSection_General, "SpotlightRadius", SpotlightRadius); // RA2: as above
		RevealTriggerRadius = ini.Get_Int(RulesSection_General, "RevealTriggerRadius", RevealTriggerRadius); // RA2: as above
		ChargeToDrainRatio = ini.Get_Float(RulesSection_General, "ChargeToDrainRatio", ChargeToDrainRatio); // RA2: as above
		DamageToFirestormDamageCoefficient = ini.Get_Float(RulesSection_General, "DamageToFirestormDamageCoefficient", DamageToFirestormDamageCoefficient); // RA2: as above
		WallBuildSpeedCoefficient = ini.Get_Float(RulesSection_General, "WallBuildSpeedCoefficient", WallBuildSpeedCoefficient); // RA2: as above
		ConditionYellowSparkingProbability = ini.Get_Float(RulesSection_General, "ConditionYellowSparkingProbability", ConditionYellowSparkingProbability); // RA2: as above
		ConditionRedSparkingProbability = ini.Get_Float(RulesSection_General, "ConditionRedSparkingProbability", ConditionRedSparkingProbability); // RA2: as above
		AITriggerSuccessWeightDelta = ini.Get_Float(RulesSection_General, "AITriggerSuccessWeightDelta", AITriggerSuccessWeightDelta); // RA2: as above
		AITriggerFailureWeightDelta = ini.Get_Float(RulesSection_General, "AITriggerFailureWeightDelta", AITriggerFailureWeightDelta); // RA2: as above
		AITriggerTrackRecordCoefficient = ini.Get_Float(RulesSection_General, "AITriggerTrackRecordCoefficient", AITriggerTrackRecordCoefficient); // RA2: as above
		WeedCapacity = ini.Get_Int(RulesSection_General, "WeedCapacity", WeedCapacity); // RA2: as above
		HarvestersPerRefinery = ini.Get_Int(RulesSection_General, "HarvestersPerRefinery", HarvestersPerRefinery); // RA2: as above
		FlashFrameTime = ini.Get_Int(RulesSection_General, "FlashFrameTime", FlashFrameTime); // RA2: as above
		RadarCombatFlashTime = ini.Get_Int(RulesSection_General, "RadarCombatFlashTime", RadarCombatFlashTime); // RA2: as above
		RadarEventSpeed = ini.Get_Float(RulesSection_General, "RadarEventSpeed", RadarEventSpeed); // RA2: as above
		RadarEventRotationSpeed = ini.Get_Float(RulesSection_General, "RadarEventRotationSpeed", RadarEventRotationSpeed); // RA2: as above
		RadarEventSuppressionDistances = ini.Get_IntList(RulesSection_General, "RadarEventSuppressionDistances", RadarEventSuppressionDistances); // RA2: as above
		RadarEventVisibilityDurations = ini.Get_IntList(RulesSection_General, "RadarEventVisibilityDurations", RadarEventVisibilityDurations); // RA2: as above
		RadarEventDurations = ini.Get_IntList(RulesSection_General, "RadarEventDurations", RadarEventDurations); // RA2: as above
		RadarEventMinRadius = ini.Get_Int(RulesSection_General, "RadarEventMinRadius", RadarEventMinRadius); // RA2: as above
		RadarEventColorSpeed = ini.Get_Float(RulesSection_General, "RadarEventColorSpeed", RadarEventColorSpeed); // RA2: as above
		HunterSeekerDetonateProximity = ini.Get_Int(RulesSection_General, "HunterSeekerDetonateProximity", HunterSeekerDetonateProximity); // RA2: as above
		HunterSeekerDescendProximity = ini.Get_Int(RulesSection_General, "HunterSeekerDescendProximity", HunterSeekerDescendProximity); // RA2: as above
		HunterSeekerDescentSpeed = ini.Get_Int(RulesSection_General, "HunterSeekerDescentSpeed", HunterSeekerDescentSpeed); // RA2: as above
		HunterSeekerAscentSpeed = ini.Get_Int(RulesSection_General, "HunterSeekerAscentSpeed", HunterSeekerAscentSpeed); // RA2: as above
		HunterSeekerEmergeSpeed = ini.Get_Int(RulesSection_General, "HunterSeekerEmergeSpeed", HunterSeekerEmergeSpeed); // RA2: as above
		MyEffectivenessCoefficientDefault = ini.Get_Float(RulesSection_General, "MyEffectivenessCoefficientDefault", MyEffectivenessCoefficientDefault); // RA2: as above
		TargetEffectivenessCoefficientDefault = ini.Get_Float(RulesSection_General, "TargetEffectivenessCoefficientDefault", TargetEffectivenessCoefficientDefault); // RA2: as above
		TargetSpecialThreatCoefficientDefault = ini.Get_Float(RulesSection_General, "TargetSpecialThreatCoefficientDefault", TargetSpecialThreatCoefficientDefault); // RA2: as above
		TargetStrengthCoefficientDefault = ini.Get_Float(RulesSection_General, "TargetStrengthCoefficientDefault", TargetStrengthCoefficientDefault); // RA2: as above
		TargetDistanceCoefficientDefault = ini.Get_Float(RulesSection_General, "TargetDistanceCoefficientDefault", TargetDistanceCoefficientDefault); // RA2: as above
		DumbMyEffectivenessCoefficient = ini.Get_Float(RulesSection_General, "DumbMyEffectivenessCoefficient", DumbMyEffectivenessCoefficient); // RA2: as above
		DumbTargetEffectivenessCoefficient = ini.Get_Float(RulesSection_General, "DumbTargetEffectivenessCoefficient", DumbTargetEffectivenessCoefficient); // RA2: as above
		DumbTargetSpecialThreatCoefficient = ini.Get_Float(RulesSection_General, "DumbTargetSpecialThreatCoefficient", DumbTargetSpecialThreatCoefficient); // RA2: as above
		DumbTargetStrengthCoefficient = ini.Get_Float(RulesSection_General, "DumbTargetStrengthCoefficient", DumbTargetStrengthCoefficient); // RA2: as above
		DumbTargetDistanceCoefficient = ini.Get_Float(RulesSection_General, "DumbTargetDistanceCoefficient", DumbTargetDistanceCoefficient); // RA2: as above
		EnemyHouseThreatBonus = ini.Get_Float(RulesSection_General, "EnemyHouseThreatBonus", EnemyHouseThreatBonus); // RA2: as above
		VeinholeMonsterStrength = ini.Get_Int(RulesSection_General, "VeinholeMonsterStrength", VeinholeMonsterStrength); // RA2: as above
		MaxVeinholeGrowth = ini.Get_Int(RulesSection_General, "MaxVeinholeGrowth", MaxVeinholeGrowth); // RA2: as above
		VeinholeGrowthRate = ini.Get_Int(RulesSection_General, "VeinholeGrowthRate", VeinholeGrowthRate); // RA2: as above
		VeinholeShrinkRate = ini.Get_Int(RulesSection_General, "VeinholeShrinkRate", VeinholeShrinkRate); // RA2: as above
		VeinDamage = ini.Get_Int(RulesSection_General, "VeinDamage", VeinDamage); // RA2: as above
		VeinholeTypeClass = TGet_Class(ini, RulesSection_General, "VeinholeTypeClass", VeinholeTypeClass);  /* out of line 654100 */ // RA2: as above
		DefaultMirageDisguises = TGet_TypeList<TerrainTypeClass>(ini, RulesSection_General, "DefaultMirageDisguises", DefaultMirageDisguises); // RA2: as above
		InfantryBlinkDisguiseTime = ini.Get_Int(RulesSection_General, "InfantryBlinkDisguiseTime", InfantryBlinkDisguiseTime); // RA2: as above
		MaximumQueuedObjects = ini.Get_Int(RulesSection_General, "MaximumQueuedObjects", MaximumQueuedObjects); // RA2: as above
		MaxWaypointPathLength = ini.Get_Int(RulesSection_General, "MaxWaypointPathLength", MaxWaypointPathLength); // RA2: as above
		TreeStrength = ini.Get_Int(RulesSection_General, "TreeStrength", TreeStrength); // RA2: as above
		EngineerCaptureLevel = ini.Get_Float(RulesSection_General, "EngineerCaptureLevel", EngineerCaptureLevel); // RA2: as above
		EngineerCaptureLevel = ini.Get_Float(RulesSection_General, "EngineerCaptureLevel", EngineerCaptureLevel); // RA2: as above
		TalkBubbleTime = ini.Get_Float(RulesSection_General, "TalkBubbleTime", TalkBubbleTime * (1.0f / 60)) * 60.0; // RA2: as above
		return(true);
	}
	return(false);
}
#endif



/***********************************************************************************************
 * RulesClass::MPlayer -- Fetch and process the multiplayer default settings.                  *
 *                                                                                             *
 *    This is used to set the default settings for the multiplayer system.                     *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database.                                            *
 *                                                                                             *
 * OUTPUT:  bool; Was the multiplayer default override section found and processed?            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool RulesClass::MPlayer(CCINIClass const & ini)
{
	static char const * const RulesSection_MultiplayerDialogSettings = "MultiplayerDialogSettings"; // RA2: the executable's section name (a file-static pointer, 0x7A905C)
	if (ini.Is_Present(RulesSection_MultiplayerDialogSettings)) { // RA2: as above
		MinMoney = ini.Get_Int(RulesSection_MultiplayerDialogSettings, "MinMoney", MinMoney); // RA2: as above
		MPMoney = ini.Get_Int(RulesSection_MultiplayerDialogSettings, "Money", MPMoney); // RA2: as above
		MPMaxMoney = ini.Get_Int(RulesSection_MultiplayerDialogSettings, "MaxMoney", MPMaxMoney); // RA2: as above
		MoneyIncrement = ini.Get_Int(RulesSection_MultiplayerDialogSettings, "MoneyIncrement", MoneyIncrement); // RA2: as above
		MinUnitCount = ini.Get_Int(RulesSection_MultiplayerDialogSettings, "MinUnitCount", MinUnitCount); // RA2: as above
		MPUnitCount = ini.Get_Int(RulesSection_MultiplayerDialogSettings, "UnitCount", MPUnitCount); // RA2: as above
		MaxUnitCount = ini.Get_Int(RulesSection_MultiplayerDialogSettings, "MaxUnitCount", MaxUnitCount); // RA2: as above
		MPBuildLevel = ini.Get_Int(RulesSection_MultiplayerDialogSettings, "TechLevel", MPBuildLevel); // RA2: as above
		GameSpeed = ini.Get_Int(RulesSection_MultiplayerDialogSettings, "GameSpeed", GameSpeed); // RA2: as above
		AIDifficultyStruct = ini.Get_Int(RulesSection_MultiplayerDialogSettings, "AIDifficulty", AIDifficultyStruct); // RA2: as above
		AIPlayers = ini.Get_Int(RulesSection_MultiplayerDialogSettings, "AIPlayers", AIPlayers); // RA2: as above
		IsMPBridgeDestruction = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "BridgeDestruction", IsMPBridgeDestruction); // RA2: as above
		IsMPShadowGrow = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "ShadowGrow", IsMPShadowGrow); // RA2: as above
		Shroud = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "Shroud", Shroud); // RA2: as above
		IsMPBasesOn = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "Bases", IsMPBasesOn); // RA2: as above
		IsMPTiberiumGrow = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "TiberiumGrows", IsMPTiberiumGrow); // RA2: as above
		IsMPCrates = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "Crates", IsMPCrates); // RA2: as above
		IsMPCaptureTheFlag = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "CaptureTheFlag", IsMPCaptureTheFlag); // RA2: as above
		HarvesterTruce = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "HarvesterTruce", HarvesterTruce); // RA2: as above
		MultiEngineer = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "MultiEngineer", MultiEngineer); // RA2: as above
		AlliesAllowed = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "AlliesAllowed", AlliesAllowed); // RA2: as above
		ShortGame = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "ShortGame", ShortGame); // RA2: as above
		SuperWeaponsAllowed = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "SuperWeaponsAllowed", SuperWeaponsAllowed); // RA2: as above
		BuildOffAlly = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "BuildOffAlly", BuildOffAlly); // RA2: as above
		FogOfWar = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "FogOfWar", FogOfWar); // RA2: as above
		MCVRedeploys = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "MCVRedeploys", MCVRedeploys); // RA2: as above
		return(true);
	}
	return(false);
}



/***********************************************************************************************
 * RulesClass::Heap_Maximums -- Fetch and process the heap override values.                    *
 *                                                                                             *
 *    This fetches the maximum heap sizes from the database specified. The heaps will be       *
 *    initialized by this routine as indicated.                                                *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database.                                            *
 *                                                                                             *
 * OUTPUT:  bool; Was the maximum section found and processed?                                 *
 *                                                                                             *
 * WARNINGS:   This process is catastrophic to any data currently existing in the heaps        *
 *             modified. This should only be processed during the game initialization stage.   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool RulesClass::Heap_Maximums(CCINIClass const & ini)
{
	static char const * MAXIMUMS = "Maximums"; // RA2: a plain pointer (the Do_ readers match only so)
	if (ini.Is_Present(MAXIMUMS)) {
		MaxPlayers = ini.Get_Int(MAXIMUMS, "Players", MaxPlayers);
	}
	return(true);
}


/// <summary>
/// Creates the infantry types declared in the control file.
/// Each entry of the infantry list names a soldier, which is created if the game has not
/// heard of it before. The soldier then reads its own section for its statistics.
/// </summary>
/// <returns>bool; Were any infantry declared?</returns>
bool RulesClass::Do_InfantryTypes(CCINIClass const & ini)
{
	static char const * INFANTRYTYPES = "InfantryTypes"; // RA2: a plain pointer (the Do_ readers match only so)
	char buffer[32];
	int count = ini.Entry_Count(INFANTRYTYPES);
	for (int i = 0; i < count; i++) {
		if (ini.Get_String(INFANTRYTYPES, ini.Get_Entry(INFANTRYTYPES, i), "", buffer, sizeof(buffer))) {
			InfantryTypeClass::Find_Or_Make(buffer);
		}
	}
	return(count > 0);
}


/// <summary>
/// Creates the house types declared in the control file.
/// Each entry of the house list names a house, which is created if the game has not heard
/// of it before. The house then reads its own section for its personality and colors.
/// </summary>
/// <returns>bool; Were any houses declared?</returns>
bool RulesClass::Do_HouseTypes(CCINIClass const & ini)
{
	static char const * HOUSES = "Countries"; // RA2: a plain pointer (the Do_ readers match only so)
	char buffer[32];
	int count = ini.Entry_Count(HOUSES);
	for (int i = 0; i < count; i++) {
		if (ini.Get_String(HOUSES, ini.Get_Entry(HOUSES, i), "", buffer, sizeof(buffer))) {
			HouseTypeClass::Find_Or_Make(buffer);
		}
	}
	return(count > 0);
}


/// <summary>
/// Creates the vehicle types declared in the control file.
/// Each entry of the vehicle list names a vehicle, which is created if the game has not
/// heard of it before. The vehicle then reads its own section for its statistics.
/// </summary>
/// <returns>bool; Were any vehicles declared?</returns>
bool RulesClass::Do_VehicleTypes(CCINIClass const & ini)
{
	static char const * VEHICLETYPES = "VehicleTypes"; // RA2: a plain pointer (the Do_ readers match only so)
	char buffer[32];
	int count = ini.Entry_Count(VEHICLETYPES);
	for (int i = 0; i < count; i++) {
		if (ini.Get_String(VEHICLETYPES, ini.Get_Entry(VEHICLETYPES, i), "", buffer, sizeof(buffer))) {
			UnitTypeClass::Find_Or_Make(buffer);
		}
	}
	return(count > 0);
}


/// <summary>
/// Creates the aircraft types declared in the control file.
/// Each entry of the aircraft list names an aircraft, which is created if the game has not
/// heard of it before. The aircraft then reads its own section for its statistics.
/// </summary>
/// <returns>bool; Were any aircraft declared?</returns>
bool RulesClass::Do_AircraftTypes(CCINIClass const & ini)
{
	static char const * AIRCRAFTTYPES = "AircraftTypes"; // RA2: a plain pointer (the Do_ readers match only so)
	char buffer[32];
	int count = ini.Entry_Count(AIRCRAFTTYPES);
	for (int i = 0; i < count; i++) {
		if (ini.Get_String(AIRCRAFTTYPES, ini.Get_Entry(AIRCRAFTTYPES, i), "", buffer, sizeof(buffer))) {
			AircraftTypeClass::Find_Or_Make(buffer);
		}
	}
	return(count > 0);
}


/// <summary>
/// Creates the sides declared in the control file and populates them.
/// A side is the umbrella a group of houses fights under -- GDI and Nod being the obvious
/// pair. Each entry names a side and lists the houses that belong to it, and every house
/// named is pointed back at its side so that either can be reached from the other.
/// </summary>
/// <returns>bool; Were any sides declared?</returns>
bool RulesClass::Do_Sides(CCINIClass const & ini)
{
	static char const * SIDES = "Sides"; // RA2: a plain pointer (the Do_ readers match only so)
	int count = ini.Entry_Count(SIDES);
	DebugString("Processing sides.\n");
	for (int i = 0; i < count; i++) {
		char const * name = ini.Get_Entry(SIDES, i);
		int side = SideClass::From_Name(name);
		SideClass * sidep;
		if (side == SIDE_NONE) {
			sidep = new SideClass(name);
		} else {
			sidep = ::Sides[side];
		}
		DebugString("Side %d: %s \n", i, (char const *)sidep->IniName);
		sidep->Houses = ini.Get_House_List(SIDES, name, sidep->Houses);
		for (int house = 0; house < sidep->Houses.Count(); house++) {
			side = sidep->Houses[house];
			HouseTypes[side]->Side = (SideType)::Sides.ID(sidep);
			DebugString("  %s\n", (char const *)HouseTypes[side]->IniName);
		}
	}
	return(count > 0);
}


/// <summary>
/// Creates the super weapon types declared in the control file.
/// Each entry of the super weapon list names a super weapon, which is created if the game
/// has not heard of it before. The weapon then reads its own section for its behavior.
/// </summary>
/// <returns>bool; Were any super weapons declared?</returns>
bool RulesClass::Do_SuperWeaponTypes(CCINIClass const & ini)
{
	static char const * SUPERWEAPONTYPES = "SuperWeaponTypes"; // RA2: a plain pointer (the Do_ readers match only so)
	char buffer[32];
	int count = ini.Entry_Count(SUPERWEAPONTYPES);
	for (int i = 0; i < count; i++) {
		if (ini.Get_String(SUPERWEAPONTYPES, ini.Get_Entry(SUPERWEAPONTYPES, i), "", buffer, sizeof(buffer))) {
			SuperWeaponTypeClass::Find_Or_Make(buffer);
		}
	}
	return(count > 0);
}


/// <summary>
/// Creates the building types declared in the control file.
/// Each entry of the building list names a building, which is created if the game has not
/// heard of it before. The building then reads its own section for its statistics.
/// </summary>
/// <returns>bool; Were any buildings declared?</returns>
bool RulesClass::Do_BuildingTypes(CCINIClass const & ini)
{
	static char const * BUILDINGTYPES = "BuildingTypes"; // RA2: a plain pointer (the Do_ readers match only so)
	char buffer[32];
	int count = ini.Entry_Count(BUILDINGTYPES);
	for (int i = 0; i < count; i++) {
		if (ini.Get_String(BUILDINGTYPES, ini.Get_Entry(BUILDINGTYPES, i), "", buffer, sizeof(buffer))) {
			BuildingTypeClass::Find_Or_Make(buffer);
		}
	}
	return(count > 0);
}


/// <summary>
/// Creates the terrain types declared in the control file.
/// Each entry of the terrain list names a terrain object, which is created if the game has
/// not heard of it before. The object then reads its own section for its behavior.
/// </summary>
/// <returns>bool; Were any terrain objects declared?</returns>
bool RulesClass::Do_TerrainTypes(CCINIClass const & ini)
{
	static char const * TERRAINTYPES = "TerrainTypes"; // RA2: a plain pointer (the Do_ readers match only so)
	char buffer[32];
	int count = ini.Entry_Count(TERRAINTYPES);
	for (int i = 0; i < count; i++) {
		if (ini.Get_String(TERRAINTYPES, ini.Get_Entry(TERRAINTYPES, i), "", buffer, sizeof(buffer))) {
			TerrainTypeClass::Find_Or_Make(buffer);
		}
	}
	return(count > 0);
}


/// <summary>
/// Creates the team types declared in the control file.
/// Unlike the other type readers, each team is filled in as soon as it is created, since a
/// team declaration carries its whole roster and script assignment with it.
/// </summary>
/// <returns>bool; Were any teams declared?</returns>
bool RulesClass::Do_TeamTypes(CCINIClass & ini) // RA2: a writable INI
{
	static char const * const TEAMS = "Teams";
	int count = ini.Entry_Count(TEAMS);
	for (int i = 0; i < count; i++) {
		TeamTypeClass * team = TGet_Class<TeamTypeClass>(ini, TEAMS, ini.Get_Entry(TEAMS, i), NULL);
		team->Read_INI(ini);
	}
	return(count > 0);
}


/// <summary>
/// Creates the smudge types declared in the control file.
/// Each entry of the smudge list names a smudge, which is created if the game has not
/// heard of it before. The smudge then reads its own section for its behavior.
/// </summary>
/// <returns>bool; Were any smudges declared?</returns>
bool RulesClass::Do_SmudgeTypes(CCINIClass const & ini)
{
	static char const * SMUDGETYPES = "SmudgeTypes"; // RA2: a plain pointer (the Do_ readers match only so)
	char buffer[32];
	int count = ini.Entry_Count(SMUDGETYPES);
	for (int i = 0; i < count; i++) {
		if (ini.Get_String(SMUDGETYPES, ini.Get_Entry(SMUDGETYPES, i), "", buffer, sizeof(buffer))) {
			SmudgeTypeClass::Find_Or_Make(buffer);
		}
	}
	return(count > 0);
}


/// <summary>
/// Creates the overlay types declared in the control file.
/// Each entry of the overlay list names an overlay, which is created if the game has not
/// heard of it before. The overlay then reads its own section for its behavior.
/// </summary>
/// <returns>bool; Were any overlays declared?</returns>
bool RulesClass::Do_OverlayTypes(CCINIClass const & ini)
{
	static char const * OVERLAYTYPES = "OverlayTypes"; // RA2: a plain pointer (the Do_ readers match only so)
	char buffer[32];
	int count = ini.Entry_Count(OVERLAYTYPES);
	for (int i = 0; i < count; i++) {
		if (ini.Get_String(OVERLAYTYPES, ini.Get_Entry(OVERLAYTYPES, i), "", buffer, sizeof(buffer))) {
			OverlayTypeClass::Find_Or_Make(buffer);
		}
	}
	return(count > 0);
}


/// <summary>
/// Creates the animation types declared in the control file.
/// Each entry of the animation list names an animation, which is created if the game has
/// not heard of it before. The animation then reads its own section for its behavior.
/// </summary>
/// <returns>bool; Were any animations declared?</returns>
bool RulesClass::Do_AnimTypes(CCINIClass const & ini)
{
	char const * const ANIMATIONS = "Animations";
	char buffer[32];
	int count = ini.Entry_Count(ANIMATIONS);
	for (int i = 0; i < count; i++) {
		if (ini.Get_String(ANIMATIONS, ini.Get_Entry(ANIMATIONS, i), "", buffer, sizeof(buffer))) {
			AnimTypeClass::Find_Or_Make(buffer);
		}
	}
	return(count > 0);
}


/// <summary>
/// Creates the voxel animation types declared in the control file.
/// Each entry of the voxel animation list names a type, which is created if the game has
/// not heard of it before. The type then reads its own section for its behavior.
/// </summary>
/// <returns>bool; Were any voxel animations declared?</returns>
bool RulesClass::Do_VoxelAnimTypes(CCINIClass const & ini)
{
	char const * const VOXELANIMS = "VoxelAnims";
	char buffer[32];
	int count = ini.Entry_Count(VOXELANIMS);
	for (int i = 0; i < count; i++) {
		if (ini.Get_String(VOXELANIMS, ini.Get_Entry(VOXELANIMS, i), "", buffer, sizeof(buffer))) {
			VoxelAnimTypeClass::Find_Or_Make(buffer);
		}
	}
	return(count > 0);
}


/// <summary>
/// Creates the warhead types declared in the control file.
/// Each entry of the warhead list names a warhead, which is created if the game has not
/// heard of it before. The warhead then reads its own section for its damage behavior.
/// </summary>
/// <returns>bool; Were any warheads declared?</returns>
bool RulesClass::Do_WarheadTypes(CCINIClass const & ini)
{
	char buffer[32];
	int count = ini.Entry_Count("Warheads");
	for (int i = 0; i < count; i++) {
		if (ini.Get_String("Warheads", ini.Get_Entry("Warheads", i), "", buffer, sizeof(buffer))) {
			WarheadTypeClass::Find_Or_Make(buffer);
		}
	}
	return(count > 0);
}


/// <summary>
/// Creates the particle types declared in the control file.
/// Each entry of the particle list names a particle, which is created if the game has not
/// heard of it before. The particle then reads its own section for its behavior.
/// </summary>
/// <returns>bool; Were any particles declared?</returns>
bool RulesClass::Do_ParticleTypes(CCINIClass const & ini)
{
	char const * const PARTICLES = "Particles";
	char buffer[32];
	int count = ini.Entry_Count(PARTICLES);
	for (int i = 0; i < count; i++) {
		if (ini.Get_String(PARTICLES, ini.Get_Entry(PARTICLES, i), "", buffer, sizeof(buffer))) {
			ParticleTypeClass::Find_Or_Make(buffer);
		}
	}
	return(count > 0);
}


/// <summary>
/// Creates the particle system types declared in the control file.
/// Each entry of the particle system list names a system, which is created if the game has
/// not heard of it before. The system then reads its own section for its behavior.
/// </summary>
/// <returns>bool; Were any particle systems declared?</returns>
bool RulesClass::Do_ParticleSystemTypes(CCINIClass const & ini)
{
	char const * const PARTICLESYSTEMS = "ParticleSystems";
	char buffer[32];
	int count = ini.Entry_Count(PARTICLESYSTEMS);
	for (int i = 0; i < count; i++) {
		if (ini.Get_String(PARTICLESYSTEMS, ini.Get_Entry(PARTICLESYSTEMS, i), "", buffer, sizeof(buffer))) {
			ParticleSystemTypeClass::Find_Or_Make(buffer);
		}
	}
	return(count > 0);
}


/***********************************************************************************************
 * RulesClass::AI -- Processes the AI control constants from the database.                     *
 *                                                                                             *
 *    This will examine the database specified and set the AI override values accordingly.     *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database that holds the AI overrides.                *
 *                                                                                             *
 * OUTPUT:  bool; Was the AI section found and processed?                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool RulesClass::AI(CCINIClass const & ini)
{
	static char const * const RulesSection_AI = "AI"; // RA2: the executable's section name, a file-static pointer at 0x007A9090
	if (ini.Is_Present(RulesSection_AI)) { // RA2: as above
		BuildConst = TGet_TypeList<BuildingTypeClass>(ini, RulesSection_AI, "BuildConst", BuildConst); // RA2: as above
		BuildPower = TGet_TypeList<BuildingTypeClass>(ini, RulesSection_AI, "BuildPower", BuildPower); // RA2: as above
		BuildRefinery = TGet_TypeList<BuildingTypeClass>(ini, RulesSection_AI, "BuildRefinery", BuildRefinery); // RA2: as above
		BuildBarracks = TGet_TypeList<BuildingTypeClass>(ini, RulesSection_AI, "BuildBarracks", BuildBarracks); // RA2: as above
		BuildTech = TGet_TypeList<BuildingTypeClass>(ini, RulesSection_AI, "BuildTech", BuildTech); // RA2: as above
		BuildWeapons = TGet_TypeList<BuildingTypeClass>(ini, RulesSection_AI, "BuildWeapons", BuildWeapons); // RA2: as above
		BuildDefense = TGet_TypeList<BuildingTypeClass>(ini, RulesSection_AI, "BuildDefense", BuildDefense); // RA2: as above
		BuildPDefense = TGet_TypeList<BuildingTypeClass>(ini, RulesSection_AI, "BuildPDefense", BuildPDefense); // RA2: as above
		BuildAA = TGet_TypeList<BuildingTypeClass>(ini, RulesSection_AI, "BuildAA", BuildAA); // RA2: as above
		BuildHelipad = TGet_TypeList<BuildingTypeClass>(ini, RulesSection_AI, "BuildHelipad", BuildHelipad); // RA2: as above
		BuildRadar = TGet_TypeList<BuildingTypeClass>(ini, RulesSection_AI, "BuildRadar", BuildRadar); // RA2: as above
		ConcreteWalls = TGet_TypeList<BuildingTypeClass>(ini, RulesSection_AI, "ConcreteWalls", ConcreteWalls); // RA2: as above
		NSGates = TGet_TypeList<BuildingTypeClass>(ini, RulesSection_AI, "NSGates", NSGates); // RA2: as above
		EWGates = TGet_TypeList<BuildingTypeClass>(ini, RulesSection_AI, "EWGates", EWGates); // RA2: as above
		BuildNavalYard = TGet_TypeList<BuildingTypeClass>(ini, RulesSection_AI, "BuildNavalYard", BuildNavalYard); // RA2: as above
		BuildDummy = TGet_TypeList<BuildingTypeClass>(ini, RulesSection_AI, "BuildDummy", BuildDummy); // RA2: as above
		NeutralTechBuildings = TGet_TypeList<BuildingTypeClass>(ini, RulesSection_AI, "NeutralTechBuildings", NeutralTechBuildings); // RA2: as above
		AttackInterval = ini.Get_Float(RulesSection_AI, "AttackInterval", AttackInterval); // RA2: as above
		AttackDelay = ini.Get_Float(RulesSection_AI, "AttackDelay", AttackDelay); // RA2: as above
		PatrolTime = ini.Get_Float(RulesSection_AI, "PatrolScan", PatrolTime); // RA2: as above
		int credit_reserve = ini.Get_Int(RulesSection_AI, "CreditReserve", RepairThreshhold); // RA2: as above
		RepairThreshhold = credit_reserve; // RA2: as above
		PathDelay = ini.Get_Float(RulesSection_AI, "PathDelay", PathDelay); // RA2: as above
		int blockage_path_delay = ini.Get_Int(RulesSection_AI, "BlockagePathDelay", BlockagePathDelay); // RA2: as above
		BlockagePathDelay = blockage_path_delay; // RA2: as above
		int tiberium_near_scan = ini.Get_Lepton(RulesSection_AI, "TiberiumNearScan", TiberiumShortScan); // RA2: as above
		TiberiumShortScan = tiberium_near_scan; // RA2: as above
		int tiberium_far_scan = ini.Get_Lepton(RulesSection_AI, "TiberiumFarScan", TiberiumLongScan); // RA2: as above
		TiberiumLongScan = tiberium_far_scan; // RA2: as above
		AutocreateTime = ini.Get_Float(RulesSection_AI, "AutocreateTime", AutocreateTime); // RA2: as above
		int infantry_reserve = ini.Get_Int(RulesSection_AI, "InfantryReserve", InfantryReserve); // RA2: as above
		InfantryReserve = infantry_reserve; // RA2: as above
		int infantry_base_mult = ini.Get_Int(RulesSection_AI, "InfantryBaseMult", InfantryBaseMult); // RA2: as above
		InfantryBaseMult = infantry_base_mult; // RA2: as above
		int power_surplus = ini.Get_Int(RulesSection_AI, "PowerSurplus", PowerSurplus); // RA2: as above
		PowerSurplus = power_surplus; // RA2: as above
		int base_size_add = ini.Get_Int(RulesSection_AI, "BaseSizeAdd", BaseSizeAdd); // RA2: as above
		BaseSizeAdd = base_size_add; // RA2: as above
		RefineryRatio = ini.Get_Float(RulesSection_AI, "RefineryRatio", RefineryRatio); // RA2: as above
		int refinery_limit = ini.Get_Int(RulesSection_AI, "RefineryLimit", RefineryLimit); // RA2: as above
		RefineryLimit = refinery_limit; // RA2: as above
		BarracksRatio = ini.Get_Float(RulesSection_AI, "BarracksRatio", BarracksRatio); // RA2: as above
		int barracks_limit = ini.Get_Int(RulesSection_AI, "BarracksLimit", BarracksLimit); // RA2: as above
		BarracksLimit = barracks_limit; // RA2: as above
		WarRatio = ini.Get_Float(RulesSection_AI, "WarRatio", WarRatio); // RA2: as above
		int war_limit = ini.Get_Int(RulesSection_AI, "WarLimit", WarLimit); // RA2: as above
		WarLimit = war_limit; // RA2: as above
		DefenseRatio = ini.Get_Float(RulesSection_AI, "DefenseRatio", DefenseRatio); // RA2: as above
		int defense_limit = ini.Get_Int(RulesSection_AI, "DefenseLimit", DefenseLimit); // RA2: as above
		DefenseLimit = defense_limit; // RA2: as above
		AARatio = ini.Get_Float(RulesSection_AI, "AARatio", AARatio); // RA2: as above
		int aa_limit = ini.Get_Int(RulesSection_AI, "AALimit", AALimit); // RA2: as above
		AALimit = aa_limit; // RA2: as above
		TeslaRatio = ini.Get_Float(RulesSection_AI, "TeslaRatio", TeslaRatio); // RA2: as above
		int tesla_limit = ini.Get_Int(RulesSection_AI, "TeslaLimit", TeslaLimit); // RA2: as above
		TeslaLimit = tesla_limit; // RA2: as above
		HelipadRatio = ini.Get_Float(RulesSection_AI, "HelipadRatio", HelipadRatio); // RA2: as above
		int helipad_limit = ini.Get_Int(RulesSection_AI, "HelipadLimit", HelipadLimit); // RA2: as above
		HelipadLimit = helipad_limit; // RA2: as above
		AirstripRatio = ini.Get_Float(RulesSection_AI, "AirstripRatio", AirstripRatio); // RA2: as above
		int airstrip_limit = ini.Get_Int(RulesSection_AI, "AirstripLimit", AirstripLimit); // RA2: as above
		AirstripLimit = airstrip_limit; // RA2: as above
		IsCompEasyBonus = ini.Get_Bool(RulesSection_AI, "CompEasyBonus", IsCompEasyBonus); // RA2: as above
		IsComputerParanoid = ini.Get_Bool(RulesSection_AI, "Paranoid", IsComputerParanoid); // RA2: as above
		PowerEmergencyFraction = ini.Get_Float(RulesSection_AI, "PowerEmergency", PowerEmergencyFraction); // RA2: as above
		int ai_base_spacing = ini.Get_Int(RulesSection_AI, "AIBaseSpacing", AIBaseSpacing); // RA2: as above
		AIBaseSpacing = ai_base_spacing; // RA2: as above
		GDIWallDefense = ini.Get_Float(RulesSection_AI, "GDIWallDefense", GDIWallDefense); // RA2: as above
		GDIWallDefenseCoefficient = ini.Get_Float(RulesSection_AI, "GDIWallDefenseCoefficient", GDIWallDefenseCoefficient); // RA2: as above
		NodBaseDefenseCoefficient = ini.Get_Float(RulesSection_AI, "NodBaseDefenseCoefficient", NodBaseDefenseCoefficient); // RA2: as above
		GDIBaseDefenseCoefficient = ini.Get_Float(RulesSection_AI, "GDIBaseDefenseCoefficient", GDIBaseDefenseCoefficient); // RA2: as above
		int maximum_base_defense_value = ini.Get_Int(RulesSection_AI, "MaximumBaseDefenseValue", MaximumBaseDefenseValue); // RA2: as above
		MaximumBaseDefenseValue = maximum_base_defense_value; // RA2: as above
		int computer_base_defense_response = ini.Get_Int(RulesSection_AI, "ComputerBaseDefenseResponse", ComputerBaseDefenseResponse); // RA2: as above
		ComputerBaseDefenseResponse = computer_base_defense_response; // RA2: as above
		return(true);
	}
	return(false);
}



/***********************************************************************************************
 * RulesClass::Powerups -- Process the powerup values from the database.                       *
 *                                                                                             *
 *    This will examine the database and initialize the powerup override values accordingly.   *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database the the powerup values are to be            *
 *                   initialized from.                                                         *
 *                                                                                             *
 * OUTPUT:  bool; Was the powerup section found and processed?                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool RulesClass::Powerups(CCINIClass const & ini) // RA2: at 0x64D490
{
	static char const * const POWERUPS = "Powerups";
	if (ini.Is_Present(POWERUPS)) {
		for (int crate = CRATE_FIRST; crate < CRATE_COUNT; crate++) {
			char buffer[128];
			if (ini.Get_String(POWERUPS, CrateNames[crate], "0,NONE", buffer, sizeof(buffer))) {

				/*
				**	Share odds.
				*/
				char * token = strtok(buffer, ",");
				if (token) {
					strtrim(token);
					CrateShares[crate] = atoi(token);
				}

				/*
				**	Animation to use.
				*/
				token = strtok(NULL, ",");
				if (token) {
					strtrim(token);
					CrateAnims[crate] = Anim_From_Name(token);
				}

				/*
				**	Can it appear on water? // RA2
				*/
				token = strtok(NULL, ","); // RA2
				if (token != NULL) { // RA2
					strtrim(token); // RA2
					if (stricmp(token, "yes") == 0) { // RA2
						CrateWater[crate] = true; // RA2
					} else if (stricmp(token, "no") == 0) { // RA2
						CrateWater[crate] = false; // RA2
					} // RA2
				} // RA2

				/*
				**	Optional data number.
				*/
				token = strtok(NULL, ",");
				if (token != NULL) {
					if (strchr(token, '%') != NULL) {
						CrateData[crate] = atof(token) / 100;
					} else {
						strtrim(token);
						CrateData[crate] = atof(token);
					}
				}
			}
		}
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * RulesClass::Land_Types -- Inits the land type values.                                       *
 *                                                                                             *
 *    This will set the land movement attributes from the database specified.                  *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the database that has the land value overrides.              *
 *                                                                                             *
 * OUTPUT:  bool; Was the land type sections found and processed?                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool RulesClass::Land_Types(CCINIClass const & ini)
{
	static char const * _lands[LAND_COUNT] = { // RA2: Land_Types at 0x64D610 matches only so
		"Clear", "Road", "Water", "Rock", "Wall", "Tiberium", // RA2: as above
		"Beach", "Rough", "Ice", "Railroad", "Tunnel", "Weeds", // RA2: as above
	}; // RA2: as above
	for (int land = LAND_FIRST; land < LAND_COUNT; land++) {
		GroundType * gptr = &Ground[land]; // RA2: Land_Types at 0x64D610 has no static char const * _lands[LAND_COUNT] =
		if (ini.Is_Present(_lands[land])) {
			gptr->Cost[SPEED_HOVER] = MIN(ini.Get_Float(_lands[land], "Hover", 1.0), 1.0); // RA2: as above
			gptr->Cost[SPEED_FOOT] = MIN(ini.Get_Float(_lands[land], "Foot", 1.0), 1.0); // RA2: as above
			gptr->Cost[SPEED_TRACK] = MIN(ini.Get_Float(_lands[land], "Track", 1.0), 1.0); // RA2: as above
			gptr->Cost[SPEED_WHEEL] = MIN(ini.Get_Float(_lands[land], "Wheel", 1.0), 1.0); // RA2: as above
			gptr->Cost[SPEED_WINGED] = 1.0f; // RA2: as above
			gptr->Cost[SPEED_FLOAT] = MIN(ini.Get_Float(_lands[land], "Float", 1.0), 1.0); // RA2: as above
			gptr->Cost[SPEED_AMPHIBIOUS] = MIN(ini.Get_Float(_lands[land], "Amphibious", 1.0), 1.0); // RA2: as above
			gptr->Cost[SPEED_CREEP] = MIN(ini.Get_Float(_lands[land], "FloatBeach", 1.0), 1.0); // RA2: as above
			gptr->Build = ini.Get_Bool(_lands[land], "Buildable", false); // RA2: as above
		}
	}
	return(true);
}


/***********************************************************************************************
 * RulesClass::IQ -- Fetches the IQ control values from the INI database.                      *
 *                                                                                             *
 *    This will scan the database specified and retrieve the IQ control values from it. These  *
 *    IQ control values are what gives the IQ rating meaning. It fundimentally controls how    *
 *    the computer behaves.                                                                    *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database to read the IQ controls from.               *
 *                                                                                             *
 * OUTPUT:  bool; Was the IQ section found and processed?                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/11/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool RulesClass::IQ(CCINIClass const & ini)
{
	static char const * IQCONTROL = "IQ"; // RA2: a plain pointer (the Do_ readers match only so)
	if (ini.Is_Present(IQCONTROL)) {
		MaxIQ = ini.Get_Int(IQCONTROL, "MaxIQLevels", MaxIQ);
		IQSuperWeapons = ini.Get_Int(IQCONTROL, "SuperWeapons", IQSuperWeapons);
		IQProduction = ini.Get_Int(IQCONTROL, "Production", IQProduction);
		IQGuardArea = ini.Get_Int(IQCONTROL, "GuardArea", IQGuardArea);
		IQRepairSell = ini.Get_Int(IQCONTROL, "RepairSell", IQRepairSell);
		IQCrush = ini.Get_Int(IQCONTROL, "AutoCrush", IQCrush);
		IQScatter = ini.Get_Int(IQCONTROL, "Scatter", IQScatter);
		IQContentScan = ini.Get_Int(IQCONTROL, "ContentScan", IQContentScan);
		IQAircraft = ini.Get_Int(IQCONTROL, "Aircraft", IQAircraft);
		IQHarvester = ini.Get_Int(IQCONTROL, "Harvester", IQHarvester);
		IQSellBack = ini.Get_Int(IQCONTROL, "SellBack", IQSellBack);
		return(true);
	}
	return(false);
}


/// <summary>
/// Fetches the flight controls used by jumpjet units.
/// These are the values the jumpjet locomotor consults for its turn rate, speed, cruising
/// height and the lazy wobble it performs while hovering.
/// </summary>
/// <returns>bool; Was a jumpjet control section found in the control file?</returns>
bool RulesClass::Jumpjet_Controls(CCINIClass const & ini)
{
	static char const * SECTION = "JumpjetControls"; // RA2: a plain pointer (the Do_ readers match only so)
	if (ini.Is_Present(SECTION)) {
		JumpjetTurnRate = ini.Get_Int(SECTION, "TurnRate", JumpjetTurnRate);
		JumpjetSpeed = ini.Get_Int(SECTION, "Speed", JumpjetSpeed);
		JumpjetClimb = ini.Get_Float(SECTION, "Climb", JumpjetClimb);
		JumpjetCruiseHeight = ini.Get_Int(SECTION, "CruiseHeight", JumpjetCruiseHeight);
		JumpjetAcceleration = ini.Get_Float(SECTION, "Acceleration", JumpjetAcceleration);
		JumpjetWobblesPerSecond = ini.Get_Float(SECTION, "WobblesPerSecond", JumpjetWobblesPerSecond);
		JumpjetWobbleDeviation = ini.Get_Int(SECTION, "WobbleDeviation", JumpjetWobbleDeviation);
		return(true); // RA2: Jumpjet_Controls at 0x64D9E0 has no JumpjetCloakDetectionRadius, SECTION
	}
	return(false);
}


/***********************************************************************************************
 * RulesClass::Difficulty -- Fetch the various difficulty group settings.                      *
 *                                                                                             *
 *    This routine is used to fetch the various group settings for the difficulty levels.      *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database that has the difficulty setting values.     *
 *                                                                                             *
 * OUTPUT:  bool; Was the difficulty section found and processed.                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/10/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool RulesClass::Difficulty(CCINIClass const & ini)
{
	Difficulty_Get(ini, Diff[DIFF_EASY], "Easy");
	Difficulty_Get(ini, Diff[DIFF_NORMAL], "Normal");
	Difficulty_Get(ini, Diff[DIFF_HARD], "Difficult");
	return(true);
}


static void Difficulty_Get(CCINIClass const & ini, DifficultyClass & diff, char const * section) // RA2: defined after Difficulty, at 0x64DB60
{
	if (ini.Is_Present(section)) {
		diff.FirepowerBias = ini.Get_Float(section, "FirePower", 1);
		diff.GroundspeedBias = ini.Get_Float(section, "Groundspeed", 1);
		diff.AirspeedBias = ini.Get_Float(section, "Airspeed", 1);
		diff.ArmorBias = ini.Get_Float(section, "Armor", 1);
		diff.ROFBias = ini.Get_Float(section, "ROF", 1);
		diff.CostBias = ini.Get_Float(section, "Cost", 1);
		diff.RepairDelay = ini.Get_Float(section, "RepairDelay", .02);
		diff.BuildDelay = ini.Get_Float(section, "BuildDelay", .03);
		diff.IsBuildSlowdown = ini.Get_Bool(section, "BuildSlowdown", false);
		diff.BuildSpeedBias = ini.Get_Float(section, "BuildTime", 1);
		diff.IsWallDestroyer = ini.Get_Bool(section, "DestroyWalls", true);
		diff.IsContentScan = ini.Get_Bool(section, "ContentScan", false);
	}
}


/// <summary>
/// Registers any movies declared in the control file.
/// Movies named in the control file that the game does not already recognize are added to
/// the global movie list, so that a mission can call for a movie the executable was never
/// built to know about.
/// </summary>
/// <returns>bool; Was a movie section found in the control file?</returns>
bool RulesClass::Do_Movies(CCINIClass const & ini)
{
	static char const * const MOVIES = "Movies";
	char buffer[32];
	if (ini.Is_Present(MOVIES)) {
		int count = ini.Entry_Count(MOVIES);
		for (int i = 0; i < count; i++) {
			if (ini.Get_String(MOVIES, ini.Get_Entry(MOVIES, i), "<none>", buffer, sizeof(buffer))) {
				if (VQ_From_Name(buffer) == -1) {
					::Movies.Add(strdup(buffer));
				}
			}
		}
		return(true);
	}
	return(false);
}
/// <summary>
/// RA2: lays out the advanced command bar from the ButtonList of [AdvancedCommandBar] ([MultiplayerAdvancedCommandBar]
/// in a multiplayer game): every command loses its place, then each listed command takes the next one.
/// </summary>
bool RulesClass::Read_AdvancedCommandBar(CCINIClass const & ini, bool multiplayer) // RA2: at 0x64DD90 (YR's body)
{
	static char const * BAR = "AdvancedCommandBar"; // RA2: a plain pointer (the Do_ readers match only so)
	static char const * MPBAR = "MultiplayerAdvancedCommandBar"; // RA2: a plain pointer (the Do_ readers match only so)
	static char const * BUTTONLIST = "ButtonList"; // RA2: a plain pointer (the Do_ readers match only so)
	static char const * NONE = "";
	char const * section = multiplayer ? MPBAR : BAR;
	char buffer[512];
	if (ini.Is_Present(section)) {
		for (int i = 0; i < 25; i++) {
			TabClass::Set_Command_Position(i, NoAdvancedCommand);
		}
		if (ini.Get_String(section, BUTTONLIST, NONE, buffer, sizeof(buffer))) {
			int count = 0;
			int slot = 0;
			for (char * token = strtok(buffer, ","); token; token = strtok(NULL, ",")) {
				int button = TabClass::Command_From_Name(token);
				if (button != NoAdvancedCommand) {
					TabClass::Set_Command_Position(button, slot);
				}
				count++;
				slot++;
			}
			TabClass::Set_Button_Count(count);
		}
		return(true);
	}
	return(false);
}







/// <summary>
/// Writes the rule data out to a save game stream.
/// The rules are stored as a raw copy of the object followed by each of the variable
/// length lists in turn. Load will expect them back in exactly this order.
/// </summary>
void RulesClass::Save(IStream * stream)
{
	stream->Write(this, sizeof(*this), 0); // RA2: RA2's form
	AIIonCannonConYardValue.Save(stream);
	AIIonCannonWarFactoryValue.Save(stream);
	AIIonCannonPowerValue.Save(stream);
	AIIonCannonEngineerValue.Save(stream);
	AIIonCannonThiefValue.Save(stream);
	AIIonCannonHarvesterValue.Save(stream);
	AIIonCannonMCVValue.Save(stream);
	AIIonCannonAPCValue.Save(stream);
	AIIonCannonBaseDefenseValue.Save(stream);
	AIIonCannonPlugValue.Save(stream);
	AIIonCannonHelipadValue.Save(stream);
	AIIonCannonTempleValue.Save(stream);
	FillEarliestTeamProbability.Save(stream);
	TeamDelays.Save(stream);
	AlliedBaseDefenseCounts.Save(stream); // RA2: as above
	SovietBaseDefenseCounts.Save(stream); // RA2: as above
	DisabledDisguiseDetectionPercent.Save(stream); // RA2: as above
	AIAutoDeployFrameDelay.Save(stream); // RA2: as above
	AIPickWallDefensePercent.Save(stream); // RA2: as above
	MinimumAIDefensiveTeams.Save(stream);
	MaximumAIDefensiveTeams.Save(stream);
	MultiplayerAICreditMultipliers.Save(stream);
	AIVirtualPurifiers.Save(stream); // RA2: as above
	TotalAITeamCap.Save(stream);
	AIHateDelays.Save(stream);
	RadarEventSuppressionDistances.Save(stream);
	RadarEventVisibilityDurations.Save(stream);
	RadarEventDurations.Save(stream);
	BarrelDebris.Save(stream);
	ExplosiveVoxelDebris.Save(stream);
	PrerequisitePower.Save(stream);
	PrerequisiteFactory.Save(stream);
	PrerequisiteBarracks.Save(stream);
	PrerequisiteRadar.Save(stream);
	PrerequisiteTech.Save(stream);
	PrerequisiteProc.Save(stream); // RA2: as above
	CreditTicks.Save(stream);
	LightningSounds.Save(stream); // RA2: as above
	Scorches.Save(stream);
	Scorches1.Save(stream);
	Scorches2.Save(stream);
	Scorches3.Save(stream);
	Scorches4.Save(stream);
	Shipyard.Save(stream); // RA2: as above
	BuildConst.Save(stream); // RA2: as above
	BuildPower.Save(stream);
	BuildRefinery.Save(stream);
	BuildBarracks.Save(stream);
	BuildTech.Save(stream);
	BuildWeapons.Save(stream);
	BuildDefense.Save(stream);
	BuildPDefense.Save(stream);
	BuildAA.Save(stream);
	BuildHelipad.Save(stream);
	BuildRadar.Save(stream);
	ConcreteWalls.Save(stream);
	NSGates.Save(stream);
	EWGates.Save(stream);
	BuildNavalYard.Save(stream); // RA2: as above
	BuildDummy.Save(stream); // RA2: as above
	NeutralTechBuildings.Save(stream); // RA2: as above
	PadAircraft.Save(stream);
	OnFire.Save(stream);
	TreeFire.Save(stream);
	SplashList.Save(stream);
	BaseUnit.Save(stream); // RA2: as above
	DefaultMirageDisguises.Save(stream); // RA2: as above
	HarvesterUnit.Save(stream);
	DeadBodies.Save(stream);
	DropPod.Save(stream);
	MetallicDebris.Save(stream);
	BridgeExplosions.Save(stream);
	IceCrackSounds.Save(stream);
	HSBuilding.Save(stream);
	AmerParaDropInf.Save(stream); // RA2: as above
	AmerParaDropNum.Save(stream); // RA2: as above
	AllyParaDropInf.Save(stream); // RA2: as above
	AllyParaDropNum.Save(stream); // RA2: as above
	SovParaDropInf.Save(stream); // RA2: as above
	SovParaDropNum.Save(stream); // RA2: as above
	DamageFireTypes.Save(stream); // RA2: as above
	WeatherConClouds.Save(stream); // RA2: as above
	WeatherConBolts.Save(stream); // RA2: as above
}
 // RA2: no PrerequisiteNodFactory, PrerequisiteGDIFactory (not in RA2's layout)


/// <summary>
/// Reads the rule data back from a save game stream.
/// This routine restores the rules that were in force when the game was saved, then
/// swizzles every object pointer the rules hold so that it refers to the freshly loaded
/// objects. Save games written before the Firestorm additions existed are recognized and
/// the missing values are given their default settings.
/// </summary>
/// <remarks>Be sure the object heaps have been loaded before calling this routine, since
/// the pointer swizzle needs them.</remarks>
void RulesClass::Load(IStream * stream)
{
	int index;
	int count; // RA2: the executable's Load: RA2's members and order
	AIIonCannonConYardValue.Clear();
	AIIonCannonWarFactoryValue.Clear();
	AIIonCannonPowerValue.Clear();
	AIIonCannonEngineerValue.Clear();
	AIIonCannonThiefValue.Clear();
	AIIonCannonHarvesterValue.Clear();
	AIIonCannonMCVValue.Clear();
	AIIonCannonAPCValue.Clear();
	AIIonCannonBaseDefenseValue.Clear();
	AIIonCannonPlugValue.Clear();
	AIIonCannonHelipadValue.Clear();
	AIIonCannonTempleValue.Clear();
	FillEarliestTeamProbability.Clear();
	TeamDelays.Clear();
	AlliedBaseDefenseCounts.Clear(); // RA2: the executable's Load: RA2's members and order
	SovietBaseDefenseCounts.Clear(); // RA2: as above
	DisabledDisguiseDetectionPercent.Clear(); // RA2: as above
	AIAutoDeployFrameDelay.Clear(); // RA2: as above
	AIPickWallDefensePercent.Clear(); // RA2: as above
	MinimumAIDefensiveTeams.Clear();
	MaximumAIDefensiveTeams.Clear();
	MultiplayerAICreditMultipliers.Clear();
	AIVirtualPurifiers.Clear(); // RA2: the executable's Load: RA2's members and order
	TotalAITeamCap.Clear();
	AIHateDelays.Clear();
	RadarEventSuppressionDistances.Clear();
	RadarEventVisibilityDurations.Clear();
	RadarEventDurations.Clear();
	ExplosiveVoxelDebris.Clear();
	PrerequisitePower.Clear();
	PrerequisiteFactory.Clear();
	PrerequisiteBarracks.Clear();
	PrerequisiteRadar.Clear();
	PrerequisiteTech.Clear();
	PrerequisiteProc.Clear(); // RA2: the executable's Load: RA2's members and order
	CreditTicks.Clear();
	LightningSounds.Clear(); // RA2: the executable's Load: RA2's members and order
	Scorches.Clear();
	Scorches1.Clear();
	Scorches2.Clear();
	Scorches3.Clear();
	Scorches4.Clear();
	Shipyard.Clear(); // RA2: the executable's Load: RA2's members and order
	BuildConst.Clear();
	BuildPower.Clear();
	BuildRefinery.Clear();
	BuildBarracks.Clear();
	BuildTech.Clear();
	BuildWeapons.Clear();
	BuildDefense.Clear();
	BuildPDefense.Clear();
	BuildAA.Clear();
	BuildHelipad.Clear();
	BuildRadar.Clear();
	ConcreteWalls.Clear();
	NSGates.Clear();
	EWGates.Clear();
	BuildNavalYard.Clear(); // RA2: the executable's Load: RA2's members and order
	BuildDummy.Clear(); // RA2: as above
	NeutralTechBuildings.Clear(); // RA2: as above
	PadAircraft.Clear();
	OnFire.Clear();
	TreeFire.Clear();
	SplashList.Clear();
	BaseUnit.Clear(); // RA2: the executable's Load: RA2's members and order
	DefaultMirageDisguises.Clear(); // RA2: as above
	HarvesterUnit.Clear();
	IceCrackSounds.Clear();
	HSBuilding.Clear();
	AmerParaDropInf.Clear(); // RA2: the executable's Load: RA2's members and order
	AmerParaDropNum.Clear(); // RA2: as above
	AllyParaDropInf.Clear(); // RA2: as above
	AllyParaDropNum.Clear(); // RA2: as above
	SovParaDropInf.Clear(); // RA2: as above
	SovParaDropNum.Clear(); // RA2: as above
	stream->Read(this, sizeof(*this), 0); // RA2: as above
	new (this) RulesClass(NoInitClass()); // RA2: as above
	AIIonCannonConYardValue.Load(stream);
	AIIonCannonWarFactoryValue.Load(stream);
	AIIonCannonPowerValue.Load(stream);
	AIIonCannonEngineerValue.Load(stream);
	AIIonCannonThiefValue.Load(stream);
	AIIonCannonHarvesterValue.Load(stream);
	AIIonCannonMCVValue.Load(stream);
	AIIonCannonAPCValue.Load(stream);
	AIIonCannonBaseDefenseValue.Load(stream);
	AIIonCannonPlugValue.Load(stream);
	AIIonCannonHelipadValue.Load(stream);
	AIIonCannonTempleValue.Load(stream);
	FillEarliestTeamProbability.Load(stream);
	TeamDelays.Load(stream);
	AlliedBaseDefenseCounts.Load(stream); // RA2: the executable's Load: RA2's members and order
	SovietBaseDefenseCounts.Load(stream); // RA2: as above
	DisabledDisguiseDetectionPercent.Load(stream); // RA2: as above
	AIAutoDeployFrameDelay.Load(stream); // RA2: as above
	AIPickWallDefensePercent.Load(stream); // RA2: as above
	MinimumAIDefensiveTeams.Load(stream);
	MaximumAIDefensiveTeams.Load(stream);
	MultiplayerAICreditMultipliers.Load(stream);
	AIVirtualPurifiers.Load(stream); // RA2: the executable's Load: RA2's members and order
	TotalAITeamCap.Load(stream);
	AIHateDelays.Load(stream);
	RadarEventSuppressionDistances.Load(stream);
	RadarEventVisibilityDurations.Load(stream);
	RadarEventDurations.Load(stream);
	BarrelDebris.Load(stream);
	ExplosiveVoxelDebris.Load(stream);
	PrerequisitePower.Load(stream);
	PrerequisiteFactory.Load(stream);
	PrerequisiteBarracks.Load(stream);
	PrerequisiteRadar.Load(stream);
	PrerequisiteTech.Load(stream);
	PrerequisiteProc.Load(stream); // RA2: the executable's Load: RA2's members and order
	CreditTicks.Load(stream);
	LightningSounds.Load(stream); // RA2: the executable's Load: RA2's members and order
	Scorches.Load(stream);
	Scorches1.Load(stream);
	Scorches2.Load(stream);
	Scorches3.Load(stream);
	Scorches4.Load(stream);
	Shipyard.Load(stream); // RA2: the executable's Load: RA2's members and order
	BuildConst.Load(stream);
	BuildPower.Load(stream);
	BuildRefinery.Load(stream);
	BuildBarracks.Load(stream);
	BuildTech.Load(stream);
	BuildWeapons.Load(stream);
	BuildDefense.Load(stream);
	BuildPDefense.Load(stream);
	BuildAA.Load(stream);
	BuildHelipad.Load(stream);
	BuildRadar.Load(stream);
	ConcreteWalls.Load(stream);
	NSGates.Load(stream);
	EWGates.Load(stream);
	BuildNavalYard.Load(stream); // RA2: the executable's Load: RA2's members and order
	BuildDummy.Load(stream); // RA2: as above
	NeutralTechBuildings.Load(stream); // RA2: as above
	PadAircraft.Load(stream);
	OnFire.Load(stream);
	TreeFire.Load(stream);
	SplashList.Load(stream);
	BaseUnit.Load(stream); // RA2: the executable's Load: RA2's members and order
	DefaultMirageDisguises.Load(stream); // RA2: as above
	HarvesterUnit.Load(stream);
	DeadBodies.Load(stream);
	DropPod.Load(stream);
	MetallicDebris.Load(stream);
	BridgeExplosions.Load(stream);
	IceCrackSounds.Load(stream);
	HSBuilding.Load(stream);
	AmerParaDropInf.Load(stream); // RA2: the executable's Load: RA2's members and order
	AmerParaDropNum.Load(stream); // RA2: as above
	AllyParaDropInf.Load(stream); // RA2: as above
	AllyParaDropNum.Load(stream); // RA2: as above
	SovParaDropInf.Load(stream); // RA2: as above
	SovParaDropNum.Load(stream); // RA2: as above
	DamageFireTypes.Load(stream); // RA2: as above
	WeatherConClouds.Load(stream); // RA2: as above
	WeatherConBolts.Load(stream); // RA2: as above
	Swizzle_Pointer(&NukeWarhead);
	Swizzle_Pointer(&FlameDamage);
	Swizzle_Pointer(&FlameDamage2);
	Swizzle_Pointer(&LargeVisceroid);
	Swizzle_Pointer(&SmallVisceroid);
	Swizzle_Pointer(&UnloadingHarvester);
	Swizzle_Pointer(&DropPodWeapon);
	Swizzle_Pointer(&EMPulseWarhead);
	Swizzle_Pointer(&C4Warhead);
	Swizzle_Pointer(&V3Warhead); // RA2: the executable's Load: RA2's members and order
	Swizzle_Pointer(&DMislWarhead); // RA2: as above
	Swizzle_Pointer(&V3EliteWarhead); // RA2: as above
	Swizzle_Pointer(&DMislEliteWarhead); // RA2: as above
	Swizzle_Pointer(&V3Rocket.Type); // RA2: as above
	Swizzle_Pointer(&DMisl.Type); // RA2: as above
	Swizzle_Pointer(&PrismType); // RA2: as above
	Swizzle_Pointer(&IvanWarhead); // RA2: as above
	Swizzle_Pointer(&DeathWeapon); // RA2: as above
	Swizzle_Pointer(&IonCannonWarhead);
	Swizzle_Pointer(&FirestormWarhead);
	Swizzle_Pointer(&VeinholeWarhead);
	Swizzle_Pointer(&DefaultFirestormExplosionSystem);
	Swizzle_Pointer(&DefaultLargeGreySmokeSystem);
	Swizzle_Pointer(&DefaultSmallGreySmokeSystem);
	Swizzle_Pointer(&DefaultSparkSystem);
	Swizzle_Pointer(&DefaultLargeRedSmokeSystem);
	Swizzle_Pointer(&DefaultSmallRedSmokeSystem);
	Swizzle_Pointer(&DefaultDebrisSmokeSystem);
	Swizzle_Pointer(&DefaultFireStreamSystem);
	Swizzle_Pointer(&DefaultTestParticleSystem);
	Swizzle_Pointer(&DefaultRepairParticleSystem);
	Swizzle_Pointer(&VeinholeTypeClass);
	Swizzle_Pointer(&WeatherConBoltExplosion); // RA2: the executable's Load: RA2's members and order
	Swizzle_Pointer(&Smoke1);
	Swizzle_Pointer(&Smoke2);
	Swizzle_Pointer(&MoveFlash);
	Swizzle_Pointer(&BombParachute);
	Swizzle_Pointer(&Parachute);
	Swizzle_Pointer(&SmallFire);
	Swizzle_Pointer(&LargeFire);
	Swizzle_Pointer(&FlareAnim);
	Swizzle_Pointer(&UnitCrateType);
	Swizzle_Pointer(&Paratrooper);
	Swizzle_Pointer(&AlliedDisguise); // RA2: the executable's Load: RA2's members and order
	Swizzle_Pointer(&SovietDisguise); // RA2: as above
	Swizzle_Pointer(&Technician);
	Swizzle_Pointer(&Engineer);
	Swizzle_Pointer(&Pilot);
	Swizzle_Pointer(&AlliedCrew); // RA2: the executable's Load: RA2's members and order
	Swizzle_Pointer(&SovietCrew); // RA2: as above
	Swizzle_Pointer(&RepairBay);
	Swizzle_Pointer(&GDIGateOne);
	Swizzle_Pointer(&GDIGateTwo);
	Swizzle_Pointer(&NodGateOne);
	Swizzle_Pointer(&NodGateTwo);
	Swizzle_Pointer(&WallTower);
	Swizzle_Pointer(&GDIPowerPlant);
	Swizzle_Pointer(&GDIPowerTurbine);
	Swizzle_Pointer(&NodRegularPower);
	Swizzle_Pointer(&NodAdvancedPower);
	Swizzle_Pointer(&GDIFirestormGenerator);
	Swizzle_Pointer(&GDIHunterSeeker);
	Swizzle_Pointer(&NodHunterSeeker);
	Swizzle_Pointer(&NukeProjectile);
	Swizzle_Pointer(&NukeDown);
	Swizzle_Pointer(&EMPulseProjectile);
	Swizzle_Pointer(&TireVoxelDebris);
	Swizzle_Pointer(&ScrapVoxelDebris);
	Swizzle_Pointer(&AtmosphereEntry);
	Swizzle_Pointer(&InfantryExplode);
	Swizzle_Pointer(&IonBlast);
	Swizzle_Pointer(&ChronoBeam); // RA2: the executable's Load: RA2's members and order
	Swizzle_Pointer(&ChronoBlast); // RA2: as above
	Swizzle_Pointer(&ChronoBlastDest); // RA2: as above
	Swizzle_Pointer(&ChronoPlacement); // RA2: as above
	Swizzle_Pointer(&IonBeam);
	Swizzle_Pointer(&WarpIn); // RA2: the executable's Load: RA2's members and order
	Swizzle_Pointer(&WarpOut); // RA2: as above
	Swizzle_Pointer(&WarpAway); // RA2: as above
	Swizzle_Pointer(&ChronoSparkle1); // RA2: as above
	Swizzle_Pointer(&IronCurtainInvokeAnim); // RA2: as above
	Swizzle_Pointer(&WeaponNullifyAnim); // RA2: as above
	Swizzle_Pointer(&Dig);
	Swizzle_Pointer(&FirestormIdleAnim);
	Swizzle_Pointer(&FirestormAirAnim);
	Swizzle_Pointer(&FirestormGroundAnim);
	Swizzle_Pointer(&FirestormActiveAnim);
	Swizzle_Pointer(&EMPulseSparkles);
	Swizzle_Pointer(&Wake);
	Swizzle_Pointer(&OreTwinkle); // RA2: the executable's Load: RA2's members and order
	Swizzle_Pointer(&FlamingInfantry);
	Swizzle_Pointer(&NukeTakeOff); // RA2: the executable's Load: RA2's members and order
	Swizzle_Pointer(&InfantryHeadPop); // RA2: as above
	Swizzle_Pointer(&InfantryNuked); // RA2: as above
	Swizzle_Pointer(&Behind); // RA2: as above
	Swizzle_Pointer(&VeinAttack);
	Swizzle_Pointer(&BarrelExplode);
	Swizzle_Pointer(&BarrelParticle);
	Swizzle_Pointer(&DropPodPuff);
	Swizzle_Pointer(&LightningWarhead); // RA2: the executable's Load: RA2's members and order
	Swizzle_Pointer(&RadSiteWarhead); // RA2: as above
	Swizzle_Pointer(&WoodCrateImg);
	Swizzle_Pointer(&CrateImg);
	Swizzle_Pointer(&WaterCrateImg); // RA2: the executable's Load: RA2's members and order
	BOMBCURS_SHP = (ShapeSet const *)MixFileClass::Retrieve("BOMBCURS.SHP"); // RA2: as above
	for (index = 0; index < BarrelDebris.Count(); index++) {
		Swizzle_Pointer(&BarrelDebris[index]);
	}
	for (index = 0; index < OnFire.Count(); index++) {
		Swizzle_Pointer(&OnFire[index]);
	}
	for (index = 0; index < TreeFire.Count(); index++) {
		Swizzle_Pointer(&TreeFire[index]);
	}
	for (index = 0; index < SplashList.Count(); index++) {
		Swizzle_Pointer(&SplashList[index]);
	}
	for (index = 0; index < Scorches.Count(); index++) {
		Swizzle_Pointer(&Scorches[index]);
	}
	for (index = 0; index < Scorches1.Count(); index++) {
		Swizzle_Pointer(&Scorches1[index]);
	}
	for (index = 0; index < Scorches2.Count(); index++) {
		Swizzle_Pointer(&Scorches2[index]);
	}
	for (index = 0; index < Scorches3.Count(); index++) {
		Swizzle_Pointer(&Scorches3[index]);
	}
	for (index = 0; index < Scorches4.Count(); index++) {
		Swizzle_Pointer(&Scorches4[index]);
	}
	for (index = 0; index < BaseUnit.Count(); index++) { // RA2: the executable's Load: RA2's members and order
		Swizzle_Pointer(&BaseUnit[index]); // RA2: as above
	}
	for (index = 0; index < DefaultMirageDisguises.Count(); index++) { // RA2: the executable's Load: RA2's members and order
		Swizzle_Pointer(&DefaultMirageDisguises[index]); // RA2: as above
	}
	for (index = 0; index < HarvesterUnit.Count(); index++) {
		Swizzle_Pointer(&HarvesterUnit[index]);
	}
	for (index = 0; index < Shipyard.Count(); index++) { // RA2: the executable's Load: RA2's members and order
		Swizzle_Pointer(&Shipyard[index]); // RA2: as above
	}
	for (index = 0; index < BuildConst.Count(); index++) {
		Swizzle_Pointer(&BuildConst[index]);
	}
	for (index = 0; index < BuildPower.Count(); index++) {
		Swizzle_Pointer(&BuildPower[index]);
	}
	for (index = 0; index < BuildRefinery.Count(); index++) {
		Swizzle_Pointer(&BuildRefinery[index]);
	}
	for (index = 0; index < BuildBarracks.Count(); index++) {
		Swizzle_Pointer(&BuildBarracks[index]);
	}
	for (index = 0; index < BuildTech.Count(); index++) {
		Swizzle_Pointer(&BuildTech[index]);
	}
	for (index = 0; index < BuildWeapons.Count(); index++) {
		Swizzle_Pointer(&BuildWeapons[index]);
	}
	for (index = 0; index < BuildDefense.Count(); index++) {
		Swizzle_Pointer(&BuildDefense[index]);
	}
	for (index = 0; index < BuildPDefense.Count(); index++) {
		Swizzle_Pointer(&BuildPDefense[index]);
	}
	for (index = 0; index < BuildAA.Count(); index++) {
		Swizzle_Pointer(&BuildAA[index]);
	}
	for (index = 0; index < BuildHelipad.Count(); index++) {
		Swizzle_Pointer(&BuildHelipad[index]);
	}
	for (index = 0; index < BuildRadar.Count(); index++) {
		Swizzle_Pointer(&BuildRadar[index]);
	}
	for (index = 0; index < ConcreteWalls.Count(); index++) {
		Swizzle_Pointer(&ConcreteWalls[index]);
	}
	for (index = 0; index < NSGates.Count(); index++) {
		Swizzle_Pointer(&NSGates[index]);
	}
	for (index = 0; index < EWGates.Count(); index++) {
		Swizzle_Pointer(&EWGates[index]);
	}
	for (index = 0; index < BuildNavalYard.Count(); index++) { // RA2: the executable's Load: RA2's members and order
		Swizzle_Pointer(&BuildNavalYard[index]); // RA2: as above
	}
	for (index = 0; index < BuildDummy.Count(); index++) { // RA2: the executable's Load: RA2's members and order
		Swizzle_Pointer(&BuildDummy[index]); // RA2: as above
	}
	for (index = 0; index < NeutralTechBuildings.Count(); index++) { // RA2: the executable's Load: RA2's members and order
		Swizzle_Pointer(&NeutralTechBuildings[index]); // RA2: as above
	}
	for (index = 0; index < PadAircraft.Count(); index++) {
		Swizzle_Pointer(&PadAircraft[index]);
	}
	for (index = 0; index < ExplosiveVoxelDebris.Count(); index++) {
		Swizzle_Pointer(&ExplosiveVoxelDebris[index]);
	}
	for (index = 0; index < DeadBodies.Count(); index++) {
		Swizzle_Pointer(&DeadBodies[index]);
	}
	for (index = 0; index < DropPod.Count(); index++) {
		Swizzle_Pointer(&DropPod[index]);
	}
	for (index = 0; index < MetallicDebris.Count(); index++) {
		Swizzle_Pointer(&MetallicDebris[index]);
	}
	for (index = 0; index < BridgeExplosions.Count(); index++) {
		Swizzle_Pointer(&BridgeExplosions[index]);
	}
	for (index = 0; index < HSBuilding.Count(); index++) {
		Swizzle_Pointer(&HSBuilding[index]);
	}
	count = WeatherConClouds.Count(); // RA2: the executable's Load: RA2's members and order
	for (index = 0; index < count; index++) { // RA2: as above
		Swizzle_Pointer(&WeatherConClouds[index]); // RA2: as above
	}
	count = WeatherConBolts.Count(); // RA2: the executable's Load: RA2's members and order
	for (index = 0; index < count; index++) { // RA2: as above
		Swizzle_Pointer(&WeatherConBolts[index]); // RA2: as above
	}
	for (index = 0; index < AmerParaDropInf.Count(); index++) { // RA2: the executable's Load: RA2's members and order
		Swizzle_Pointer(&AmerParaDropInf[index]); // RA2: as above
	}
	for (index = 0; index < AllyParaDropInf.Count(); index++) { // RA2: the executable's Load: RA2's members and order
		Swizzle_Pointer(&AllyParaDropInf[index]); // RA2: as above
	}
	for (index = 0; index < SovParaDropInf.Count(); index++) { // RA2: the executable's Load: RA2's members and order
		Swizzle_Pointer(&SovParaDropInf[index]); // RA2: as above
	}
	count = DamageFireTypes.Count(); // RA2: the executable's Load: RA2's members and order
	for (index = 0; index < count; index++) { // RA2: as above
		Swizzle_Pointer(&DamageFireTypes[index]); // RA2: as above
	}
}



/// <summary>
/// Removes all references to the specified object from the rules.
/// The rules hold direct pointers to a great many object types and warheads, and keep
/// lists of others. This routine is called as part of the general detach process when an
/// object is about to be destroyed, so that no rule is left pointing at freed memory.
/// </summary>
void RulesClass::Detach(AbstractClass const * target, bool all)
{
	if (target == LargeVisceroid) {
		LargeVisceroid = 0; // RA2: RA2's form
	}
	if (target == SmallVisceroid) {
		SmallVisceroid = 0; // RA2: as above
	}
	if (target == UnloadingHarvester) {
		UnloadingHarvester = 0; // RA2: as above
	}
	if (target == VeinAttack) {
		VeinAttack = 0; // RA2: as above
	}
	if (target == Wake) {
		Wake = 0; // RA2: as above
	}
	if (target == OreTwinkle) { // RA2: as above
		OreTwinkle = 0; // RA2: as above
	}
	if (target == FlamingInfantry) {
		FlamingInfantry = 0; // RA2: as above
	}
	if (target == NukeTakeOff) { // RA2: as above
		NukeTakeOff = 0; // RA2: as above
	}
	if (target == InfantryHeadPop) { // RA2: as above
		InfantryHeadPop = 0; // RA2: as above
	}
	if (target == InfantryNuked) { // RA2: as above
		InfantryNuked = 0; // RA2: as above
	}
	if (target == Behind) { // RA2: as above
		Behind = 0; // RA2: as above
	}
	if (target == DropPodWeapon) {
		DropPodWeapon = 0; // RA2: as above
	}
	if (target == FlameDamage) {
		FlameDamage = 0; // RA2: as above
	}
	if (target == FlameDamage2) {
		FlameDamage2 = 0; // RA2: as above
	}
	if (target == NukeWarhead) {
		NukeWarhead = 0; // RA2: as above
	}
	if (target == EMPulseWarhead) {
		EMPulseWarhead = 0; // RA2: as above
	}
	if (target == C4Warhead) {
		C4Warhead = 0; // RA2: as above
	}
	if (target == V3Warhead) { // RA2: as above
		V3Warhead = 0; // RA2: as above
	}
	if (target == DMislWarhead) { // RA2: as above
		DMislWarhead = 0; // RA2: as above
	}
	if (target == V3EliteWarhead) { // RA2: as above
		V3EliteWarhead = 0; // RA2: as above
	}
	if (target == DMislEliteWarhead) { // RA2: as above
		DMislEliteWarhead = 0; // RA2: as above
	}
	if (target == V3Rocket.Type) { // RA2: as above
		V3Rocket.Type = 0; // RA2: as above
	}
	if (target == DMisl.Type) { // RA2: as above
		DMisl.Type = 0; // RA2: as above
	}
	if (target == IvanWarhead) { // RA2: as above
		IvanWarhead = 0; // RA2: as above
	}
	if (target == DeathWeapon) { // RA2: as above
		DeathWeapon = 0; // RA2: as above
	}
	if (target == IonCannonWarhead) {
		IonCannonWarhead = 0; // RA2: as above
	}
	if (target == FirestormWarhead) {
		FirestormWarhead = 0; // RA2: as above
	}
	if (target == EMPulseSparkles) {
		EMPulseSparkles = 0; // RA2: as above
	}
	if (target == VeinholeWarhead) {
		VeinholeWarhead = 0; // RA2: as above
	}
	if (target == DefaultFirestormExplosionSystem) {
		DefaultFirestormExplosionSystem = 0; // RA2: as above
	}
	if (target == DefaultLargeGreySmokeSystem) {
		DefaultLargeGreySmokeSystem = 0; // RA2: as above
	}
	if (target == DefaultSmallGreySmokeSystem) {
		DefaultSmallGreySmokeSystem = 0; // RA2: as above
	}
	if (target == DefaultSparkSystem) {
		DefaultSparkSystem = 0; // RA2: as above
	}
	if (target == DefaultLargeRedSmokeSystem) {
		DefaultLargeRedSmokeSystem = 0; // RA2: as above
	}
	if (target == DefaultSmallRedSmokeSystem) {
		DefaultSmallRedSmokeSystem = 0; // RA2: as above
	}
	if (target == DefaultDebrisSmokeSystem) {
		DefaultDebrisSmokeSystem = 0; // RA2: as above
	}
	if (target == DefaultFireStreamSystem) {
		DefaultFireStreamSystem = 0; // RA2: as above
	}
	if (target == DefaultTestParticleSystem) {
		DefaultTestParticleSystem = 0; // RA2: as above
	}
	if (target == DefaultRepairParticleSystem) {
		DefaultRepairParticleSystem = 0; // RA2: as above
	}
	if (target == VeinholeTypeClass) {
		VeinholeTypeClass = 0; // RA2: as above
	}
	if (target == WeatherConBoltExplosion) { // RA2: as above
		WeatherConBoltExplosion = 0; // RA2: as above
	}
	if (target == Smoke1) {
		Smoke1 = 0; // RA2: as above
	}
	if (target == Smoke2) {
		Smoke2 = 0; // RA2: as above
	}
	if (target == MoveFlash) {
		MoveFlash = 0; // RA2: as above
	}
	if (target == BombParachute) {
		BombParachute = 0; // RA2: as above
	}
	if (target == Parachute) {
		Parachute = 0; // RA2: as above
	}
	if (target == SmallFire) {
		SmallFire = 0; // RA2: as above
	}
	if (target == LargeFire) {
		LargeFire = 0; // RA2: as above
	}
	if (target == FlareAnim) {
		FlareAnim = 0; // RA2: as above
	}
	if (target == UnitCrateType) {
		UnitCrateType = 0; // RA2: as above
	}
	if (target == Paratrooper) {
		Paratrooper = 0; // RA2: as above
	}
	if (target == AlliedDisguise) { // RA2: as above
		AlliedDisguise = 0; // RA2: as above
	}
	if (target == SovietDisguise) { // RA2: as above
		SovietDisguise = 0; // RA2: as above
	}
	if (target == Technician) {
		Technician = 0; // RA2: as above
	}
	if (target == Engineer) {
		Engineer = 0; // RA2: as above
	}
	if (target == Pilot) {
		Pilot = 0; // RA2: as above
	}
	if (target == AlliedCrew) { // RA2: as above
		AlliedCrew = 0; // RA2: as above
	}
	if (target == SovietCrew) { // RA2: as above
		SovietCrew = 0; // RA2: as above
	}
	if (target == RepairBay) {
		RepairBay = 0; // RA2: as above
	}
	if (target == GDIGateOne) {
		GDIGateOne = 0; // RA2: as above
	}
	if (target == GDIGateTwo) {
		GDIGateTwo = 0; // RA2: as above
	}
	if (target == NodGateOne) {
		NodGateOne = 0; // RA2: as above
	}
	if (target == NodGateTwo) {
		NodGateTwo = 0; // RA2: as above
	}
	if (target == WallTower) {
		WallTower = 0; // RA2: as above
	}
	if (target == GDIPowerPlant) {
		GDIPowerPlant = 0; // RA2: as above
	}
	if (target == GDIPowerTurbine) {
		GDIPowerTurbine = 0; // RA2: as above
	}
	if (target == NodRegularPower) {
		NodRegularPower = 0; // RA2: as above
	}
	if (target == NodAdvancedPower) {
		NodAdvancedPower = 0; // RA2: as above
	}
	if (target == GDIFirestormGenerator) {
		GDIFirestormGenerator = 0; // RA2: as above
	}
	if (target == GDIHunterSeeker) {
		GDIHunterSeeker = 0; // RA2: as above
	}
	if (target == NodHunterSeeker) {
		NodHunterSeeker = 0; // RA2: as above
	}
	if (target == NukeProjectile) {
		NukeProjectile = 0; // RA2: as above
	}
	if (target == NukeDown) {
		NukeDown = 0; // RA2: as above
	}
	if (target == EMPulseProjectile) {
		EMPulseProjectile = 0; // RA2: as above
	}
	if (target == TireVoxelDebris) {
		TireVoxelDebris = 0; // RA2: as above
	}
	if (target == ScrapVoxelDebris) {
		ScrapVoxelDebris = 0; // RA2: as above
	}
	if (target == AtmosphereEntry) {
		AtmosphereEntry = 0; // RA2: as above
	}
	if (target == InfantryExplode) {
		InfantryExplode = 0; // RA2: as above
	}
	if (target == IonBlast) {
		IonBlast = 0; // RA2: as above
	}
	if (target == IonBeam) {
		IonBeam = 0; // RA2: as above
	}
	if (target == ChronoBlast) { // RA2: as above
		ChronoBlast = 0; // RA2: as above
	}
	if (target == ChronoBlastDest) { // RA2: as above
		ChronoBlastDest = 0; // RA2: as above
	}
	if (target == ChronoPlacement) { // RA2: as above
		ChronoPlacement = 0; // RA2: as above
	}
	if (target == ChronoBeam) { // RA2: as above
		ChronoBeam = 0; // RA2: as above
	}
	if (target == WarpIn) { // RA2: as above
		WarpIn = 0; // RA2: as above
	}
	if (target == WarpOut) { // RA2: as above
		WarpOut = 0; // RA2: as above
	}
	if (target == WarpAway) { // RA2: as above
		WarpAway = 0; // RA2: as above
	}
	if (target == ChronoSparkle1) { // RA2: as above
		ChronoSparkle1 = 0; // RA2: as above
	}
	if (target == IronCurtainInvokeAnim) { // RA2: as above
		IronCurtainInvokeAnim = 0; // RA2: as above
	}
	if (target == WeaponNullifyAnim) { // RA2: as above
		WeaponNullifyAnim = 0; // RA2: as above
	}
	if (target == Dig) {
		Dig = 0; // RA2: as above
	}
	if (target == FirestormIdleAnim) {
		FirestormIdleAnim = 0; // RA2: as above
	}
	if (target == FirestormAirAnim) {
		FirestormAirAnim = 0; // RA2: as above
	}
	if (target == FirestormActiveAnim) {
		FirestormActiveAnim = 0; // RA2: as above
	}
	if (target == FirestormGroundAnim) {
		FirestormGroundAnim = 0; // RA2: as above
	}
	if (target == BarrelExplode) {
		BarrelExplode = 0; // RA2: as above
	}
	if (target == BarrelParticle) {
		BarrelParticle = 0; // RA2: as above
	}
	if (target == DropPodPuff) {
		DropPodPuff = 0; // RA2: as above
	}
	if (target == LightningWarhead) { // RA2: as above
		LightningWarhead = 0; // RA2: as above
	}
	if (target == RadSiteWarhead) { // RA2: as above
		RadSiteWarhead = 0; // RA2: as above
	}
	BarrelDebris.Delete((VoxelAnimTypeClass *)target); // RA2: as above
	OnFire.Delete((AnimTypeClass *)target); // RA2: as above
	TreeFire.Delete((AnimTypeClass *)target); // RA2: as above
	SplashList.Delete((AnimTypeClass *)target); // RA2: as above
	Scorches.Delete((SmudgeTypeClass *)target); // RA2: as above
	Scorches1.Delete((SmudgeTypeClass *)target); // RA2: as above
	Scorches2.Delete((SmudgeTypeClass *)target); // RA2: as above
	Scorches3.Delete((SmudgeTypeClass *)target); // RA2: as above
	Scorches4.Delete((SmudgeTypeClass *)target); // RA2: as above
	BaseUnit.Delete((UnitTypeClass *)target); // RA2: as above
	DefaultMirageDisguises.Delete((TerrainTypeClass *)target); // RA2: as above
	HarvesterUnit.Delete((UnitTypeClass *)target); // RA2: as above
	Shipyard.Delete((BuildingTypeClass *)target); // RA2: as above
	BuildConst.Delete((BuildingTypeClass *)target); // RA2: as above
	BuildPower.Delete((BuildingTypeClass *)target); // RA2: as above
	BuildRefinery.Delete((BuildingTypeClass *)target); // RA2: as above
	BuildBarracks.Delete((BuildingTypeClass *)target); // RA2: as above
	BuildTech.Delete((BuildingTypeClass *)target); // RA2: as above
	BuildWeapons.Delete((BuildingTypeClass *)target); // RA2: as above
	BuildDefense.Delete((BuildingTypeClass *)target); // RA2: as above
	BuildPDefense.Delete((BuildingTypeClass *)target); // RA2: as above
	BuildAA.Delete((BuildingTypeClass *)target); // RA2: as above
	BuildHelipad.Delete((BuildingTypeClass *)target); // RA2: as above
	BuildRadar.Delete((BuildingTypeClass *)target); // RA2: as above
	ConcreteWalls.Delete((BuildingTypeClass *)target); // RA2: as above
	NSGates.Delete((BuildingTypeClass *)target); // RA2: as above
	EWGates.Delete((BuildingTypeClass *)target); // RA2: as above
	BuildNavalYard.Delete((BuildingTypeClass *)target); // RA2: as above
	BuildDummy.Delete((BuildingTypeClass *)target); // RA2: as above
	NeutralTechBuildings.Delete((BuildingTypeClass *)target); // RA2: as above
	PadAircraft.Delete((AircraftTypeClass *)target); // RA2: as above
	DeadBodies.Delete((AnimTypeClass *)target); // RA2: as above
	DropPod.Delete((AnimTypeClass *)target); // RA2: as above
	MetallicDebris.Delete((AnimTypeClass *)target); // RA2: as above
	BridgeExplosions.Delete((AnimTypeClass *)target); // RA2: as above
	HSBuilding.Delete((BuildingTypeClass *)target); // RA2: as above
	WeatherConClouds.Delete((AnimTypeClass *)target); // RA2: as above
	WeatherConBolts.Delete((AnimTypeClass *)target); // RA2: as above
	AmerParaDropInf.Delete((InfantryTypeClass *)target); // RA2: as above
	AllyParaDropInf.Delete((InfantryTypeClass *)target); // RA2: as above
	SovParaDropInf.Delete((InfantryTypeClass *)target); // RA2: as above
	DamageFireTypes.Delete((AnimTypeClass *)target); // RA2: as above
}



/***********************************************************************************************
 * RulesClass::Objects -- Fetch all the object characteristic values.                          *
 *                                                                                             *
 *    This will parse the specified INI database and fetch all the object characteristic       *
 *    values specified therein.                                                                *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the ini database to scan.                                    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/10/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool RulesClass::Objects(CCINIClass & ini) // RA2: a writable INI
{
	int index;
	for (index = 0; index < HouseTypes.Count(); index++) { // RA2: the executable's Objects (no stricmp on the infantry name)
		HouseTypes[index]->Read_INI(ini); // RA2: as above
	}
	for (index = 0; index < SuperWeaponTypes.Count(); index++) { // RA2: the executable's Objects (no stricmp on the infantry name)
		SuperWeaponTypes[index]->Read_INI(ini);
	}
	for (index = 0; index < AnimTypes.Count(); index++) { // RA2: the executable's Objects (no stricmp on the infantry name)
		AnimTypes[index]->Read_INI(ArtINI); // RA2: as above
	}
	for (index = 0; index < BuildingTypes.Count(); index++) { // RA2: the executable's Objects (no stricmp on the infantry name)
		BuildingTypes[index]->Read_INI(ini); // RA2: as above
	}
	for (index = 0; index < AircraftTypes.Count(); index++) { // RA2: the executable's Objects (no stricmp on the infantry name)
		AircraftTypes[index]->Read_INI(ini); // RA2: as above
	}
	for (index = 0; index < UnitTypes.Count(); index++) { // RA2: the executable's Objects (no stricmp on the infantry name)
		UnitTypes[index]->Read_INI(ini); // RA2: as above
	}
	for (index = 0; index < InfantryTypes.Count(); index++) { // RA2: the executable's Objects (no stricmp on the infantry name)
		InfantryTypes[index]->Read_INI(ini); // RA2: as above
	}
	for (index = 0; index < Weapons.Count(); index++) { // RA2: the executable's Objects (no stricmp on the infantry name)
		Weapons[index]->Read_INI(ini); // RA2: as above
	}
	for (index = 0; index < BulletTypes.Count(); index++) { // RA2: the executable's Objects (no stricmp on the infantry name)
		BulletTypes[index]->Read_INI(ini); // RA2: as above
	}
	for (index = 0; index < Warheads.Count(); index++) { // RA2: the executable's Objects (no stricmp on the infantry name)
		Warheads[index]->Read_INI(ini); // RA2: as above
	}
	for (index = 0; index < Weapons.Count(); index++) { // RA2: the executable's Objects (no stricmp on the infantry name)
		Weapons[index]->Init_Max_Speed(); // RA2: as above
	}
	for (index = 0; index < BuildingTypes.Count(); index++) { // RA2: the executable's Objects (no stricmp on the infantry name)
		BuildingTypes[index]->Calculate_Base_Defense_Values(); // RA2: as above
	}
	for (index = 0; index < TerrainTypes.Count(); index++) { // RA2: the executable's Objects (no stricmp on the infantry name)
		TerrainTypes[index]->Read_INI(ini); // RA2: as above
	}
	for (index = 0; index < SmudgeTypes.Count(); index++) { // RA2: the executable's Objects (no stricmp on the infantry name)
		SmudgeTypes[index]->Read_INI(ini); // RA2: as above
	}
	for (index = 0; index < OverlayTypes.Count(); index++) { // RA2: the executable's Objects (no stricmp on the infantry name)
		OverlayTypes[index]->Read_INI(ini); // RA2: as above
	}
	for (index = 0; index < ParticleTypes.Count(); index++) { // RA2: the executable's Objects (no stricmp on the infantry name)
		ParticleTypes[index]->Read_INI(ini); // RA2: as above
	}
	for (index = 0; index < ParticleSystemTypes.Count(); index++) { // RA2: the executable's Objects (no stricmp on the infantry name)
		ParticleSystemTypes[index]->Read_INI(ini); // RA2: as above
	}
	for (index = 0; index < VoxelAnimTypes.Count(); index++) { // RA2: the executable's Objects (no stricmp on the infantry name)
		VoxelAnimTypes[index]->Read_INI(ini); // RA2: as above
	}
	for (int mission = MISSION_FIRST; mission < MISSION_COUNT; mission++) {
		MissionControl[mission].Mission = (MissionType)mission; // RA2: the executable's Objects (no stricmp on the infantry name)
		MissionControl[mission].Read_INI(ini); // RA2: as above
	}
	return(true);
}



static bool CoopCampLoaded; // RA2: has CoopCamp.ini's unique ID been read yet?
static int CoopCampUniqueID; // RA2: CoopCamp.ini's unique ID


/// <summary>
/// Fetches the identifying checksum of the cooperative campaign control file.
/// The file is read the first time only; the rule identifier folds the result in, so
/// that machines running different cooperative campaigns can tell.
/// </summary>
/// <returns>Returns with the unique ID of CoopCamp.ini (0 when it cannot be loaded).</returns>
int Coop_Camp_Unique_ID(void) // RA2: not in TibSun, at 0x652520
{
	if (!CoopCampLoaded) {
		CCINIClass ini;
		CCFileClass file("CoopCamp.ini");
		if (ini.Load(file, false, false)) {
			CoopCampUniqueID = ini.Get_Unique_ID();
		}
		CoopCampLoaded = true;
	}
	return(CoopCampUniqueID);
}





/// <summary>
/// Fetches the identifying checksum of the main rule file.
/// This routine is used when comparing rule versions between machines, so that a
/// multiplayer game can be refused when the players are not running the same rules. The
/// Firestorm rule file is folded into the result whenever that addon is enabled.
/// </summary>
/// <returns>Returns with the unique ID of the rules currently in force.</returns>
int RulesClass::Get_Rule_Unique_ID(void)
{
	int id = RuleINI->Get_Unique_ID();
	int count = MultiplayerGameMode::GetModeCount(); // RA2: RA2's form
	for (int index = 1; index <= count; index++) { // RA2: as above
		MultiplayerGameMode *session = MultiplayerGameMode::Get(index); // RA2: as above
		if (session != 0) { // RA2: as above
			id += session->GetINI()->Get_Unique_ID(); // RA2: as above
		}
	}
	return id + Coop_Camp_Unique_ID(); // RA2: as above
}



/// <summary>
/// Fetches the identifying checksum of the art control file.
/// This routine serves the same purpose as its rule and AI counterparts -- it lets the
/// game tell whether two machines are working from the same art data. The Firestorm art
/// file is not folded in (RA2).
/// </summary>
/// <returns>Returns with the unique ID of the art rules currently in force.</returns>
int RulesClass::Get_Art_Unique_ID(void)
{
	return ArtINI.Get_Unique_ID(); // RA2: no Firestorm add-on art (0x652710 matches only so)
} // RA2: no return(id); (not in RA2's layout)


/// <summary>
/// Fetches the identifying checksum of the AI control file.
/// This routine is used when comparing rule versions between machines, so that a
/// multiplayer game can be refused when the players are not running the same AI rules.
/// The Firestorm AI file is not folded in (RA2).
/// </summary>
/// <returns>Returns with the unique ID of the AI rules currently in force.</returns>
int RulesClass::Get_AI_Unique_ID(void)
{
	return AIINI.Get_Unique_ID(); // RA2: no Firestorm add-on AI (0x652720 matches only so)
} // RA2: no return(id); (not in RA2's layout)


/// <summary>
/// Loads the art control file into the art database.
/// This routine throws away whatever art rules were previously loaded and reads ART.INI
/// afresh. The initialization pass calls it before any object type is created, since the
/// type objects take their imagery from this database.
/// </summary>
void RulesClass::Load_Art_INI(void)
{
	ArtINI.Clear();
	CCFileClass art("ART.INI");
	ArtINI.Load(art, false);
}








/***********************************************************************************************
 * _Scale_To_256 -- Scales a 1..100 number into a 1..255 number.                               *
 *                                                                                             *
 *    This is a helper routine that will take a decimal percentage number and convert it       *
 *    into a game based fixed point number.                                                    *
 *                                                                                             *
 * INPUT:   val   -- Decimal percent number to convert.                                        *
 *                                                                                             *
 * OUTPUT:  Returns with the decimal percent number converted to a game fixed point number.    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/17/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
static inline int _Scale_To_256(int val)
{
	val = 100.0 / (MPH_LIGHT_SPEED + 1) * val;
	val = MIN(val, MPH_LIGHT_SPEED);
	return(val);
}

