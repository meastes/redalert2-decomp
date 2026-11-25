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
#include "palette.h"
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

/// <summary>
/// Constructor for a rules object that is about to be overwritten.
/// The load process reads the rule data straight off the save game stream, so this
/// constructor deliberately leaves every value alone rather than paying for defaults that
/// would only be thrown away.
/// </summary>
RulesClass::RulesClass(NoInitClass const & x) :
	RadColor(x), // RA2: the load constructor builds only the colour members that have a NoInit form
	LineTrailColorOverride(x),
	ChronoBeamColor(x)
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
RulesClass::RulesClass(void) :
	DetailMinFrameRateNormal(15), // RA2: the executable's initializer list, in RA2's member order and values
	DetailMinFrameRateMovie(20),
	DetailBufferZoneWidth(5),
	AmmoCrateDamage(100),
	LargeVisceroid(0), // RA2: the executable's initializer list, in RA2's member order and values
	SmallVisceroid(0),
	UnloadingHarvester(0),
	AttackingAircraftSightRange(5),
	TunnelSpeed(1.0), // RA2: the executable's initializer list, in RA2's member order and values
	TiberiumHeal(0.016666666666666666),
	IsFreeMCV(false),
	IsBerzerkAllowed(false),
	PoseDir(DIR_N),
	DropPodPuff(0), // RA2: the executable's initializer list, in RA2's member order and values
	WaypointAnimationSpeed(12),
	BarrelExplode(0), // RA2: the executable's initializer list, in RA2's member order and values
	BarrelParticle(0),
	RadarEventColorSpeed(0.05f),
	RadarEventMinRadius(5),
	RadarEventSpeed(1.0f), // RA2: the executable's initializer list, in RA2's member order and values
	RadarEventRotationSpeed(0.1f),
	FlashFrameTime(7),
	RadarCombatFlashTime(21),
	MaxWaypointPathLength(15),
	Wake(0), // RA2: the executable's initializer list, in RA2's member order and values
	NukeTakeOff(0),
	InfantryExplode(0),
	FlamingInfantry(0),
	InfantryHeadPop(0),
	InfantryNuked(0),
	Behind(0),
	AITriggerSuccessWeightDelta(1.0),
	AITriggerFailureWeightDelta(-1.0),
	AITriggerTrackRecordCoefficient(1.0),
	VeinholeMonsterStrength(10000),
	MaxVeinholeGrowth(1000),
	VeinholeGrowthRate(100),
	VeinholeShrinkRate(100),
	VeinAttack(0), // RA2: the executable's initializer list, in RA2's member order and values
	VeinDamage(2),
	MaximumQueuedObjects(5),
	AircraftFogReveal(6),
	WoodCrateImg(0), // RA2: the executable's initializer list, in RA2's member order and values
	CrateImg(0),
	WaterCrateImg(0),
	DigSound(VOC_NONE),
	CreateUnitSound(VOC_NONE), // RA2: the executable's initializer list, in RA2's member order and values
	CreateInfantrySound(VOC_NONE),
	CreateAircraftSound(VOC_NONE),
	BaseUnderAttackSound(VOC_NONE),
	GUIMainButtonSound(VOC_NONE),
	GUIBuildSound(VOC_NONE),
	GUITabSound(VOC_NONE),
	GUIOpenSound(VOC_NONE),
	GUICloseSound(VOC_NONE),
	GUIMoveOutSound(VOC_NONE),
	GUIMoveInSound(VOC_NONE),
	GUIComboOpenSound(VOC_NONE),
	GUIComboCloseSound(VOC_NONE),
	GUICheckboxSound(VOC_NONE),
	ScoreAnimSound(VOC_NONE),
	IFVTransformSound(VOC_NONE),
	PsychicSensorDetectSound(VOC_NONE),
	BuildingGarrisonedSound(VOC_NONE),
	BuildingAbandonedSound(VOC_NONE),
	BuildingRepairedSound(VOC_NONE),
	CheerSound(VOC_NONE),
	PlaceBeaconSound(VOC_NONE),
	DefaultChronoSound(VOC_NONE),
	StartPlanningModeSound(VOC_NONE),
	AddPlanningModeCommandSound(VOC_NONE),
	ExecutePlanSound(VOC_NONE),
	EndPlanningModeSound(VOC_NONE),
	CrateMoneySound(VOC_NONE),
	CrateRevealSound(VOC_NONE),
	CrateFireSound(VOC_NONE),
	CrateArmourSound(VOC_NONE),
	CrateSpeedSound(VOC_NONE),
	CrateUnitSound(VOC_NONE),
	CratePromoteSound(VOC_NONE),
	ImpactWaterSound(VOC_NONE),
	ImpactLandSound(VOC_NONE),
	SinkingSound(VOC_NONE),
	BombTickingSound(VOC_NONE),
	BombAttachSound(VOC_NONE),
	ChronoInSound(VOC_NONE),
	ChronoOutSound(VOC_NONE),
	SpySatActivationSound(VOC_NONE),
	SpySatDeactivationSound(VOC_NONE),
	UpgradeVeteranSound(VOC_NONE),
	UpgradeEliteSound(VOC_NONE),
	Dig(0),
	IonBlast(0),
	IonBeam(0),
	WeatherConBoltExplosion(0),
	ChronoBlast(0),
	ChronoBlastDest(0),
	ChronoPlacement(0),
	ChronoBeam(0),
	WarpIn(0),
	WarpOut(0),
	WarpAway(0),
	ChronoSparkle1(0),
	IronCurtainInvokeAnim(0),
	WeaponNullifyAnim(0),
	AtmosphereEntry(0),
	GateUpSound(VOC_NONE),
	GateDownSound(VOC_NONE),
	JumpjetTurnRate(3),
	JumpjetSpeed(30),
	JumpjetClimb(5.0), // RA2: the executable's initializer list, in RA2's member order and values
	JumpjetCruiseHeight(400),
	JumpjetAcceleration(0.25), // RA2: the executable's initializer list, in RA2's member order and values
	JumpjetWobblesPerSecond(0.25),
	JumpjetWobbleDeviation(40),
	IonCannonDamage(700),
	RailgunDamageRadius(128), // RA2: the executable's initializer list, in RA2's member order and values
	PrismType(0),
	PrismSupportModifier(100),
	PrismSupportMax(8),
	PrismSupportDelay(100),
	PrismSupportDuration(15),
	PrismSupportHeight(300),
	ParadropRadius(1024),
	ZoomInFactor(2.0),
	ConditionRedSparkingProbability(0.02),
	ConditionYellowSparkingProbability(0.01),
	TiberiumExplosionDamage(100),
	TiberiumStrength(10),
	MinLowPowerProductionSpeed(0.5f), // RA2: the executable's initializer list, in RA2's member order and values
	MaxLowPowerProductionSpeed(0.9f),
	LowPowerPenaltyModifier(1.0f),
	MultipleFactory(1.0f),
	MaximumCheerRate(1),
	TreeFlammability(0.1),
	MissileSpeedVar(0.25),
	MissileROTVar(0.25),
	MissileSafetyAltitude(500),
	DropPodWeapon(0),
	DropPodHeight(1500),
	DropPodSpeed(40),
	DropPodAngle(1.5707963267948966), // RA2: the executable's initializer list, in RA2's member order and values
	ScrollMultiplier(1.0),
	CrewEscape(0.5),
	ShakeScreen(400),
	HoverHeight(120),
	HoverBob(30.0), // RA2: the executable's initializer list, in RA2's member order and values
	HoverBoost(1.3),
	HoverAcceleration(0.03), // RA2: the executable's initializer list, in RA2's member order and values
	HoverBrake(0.03),
	HoverDampen(0.8),
	PlacementDelay(0.05),
	TireVoxelDebris(0),
	ScrapVoxelDebris(0),
	BridgeVoxelMax(3),
	CloakingStages(9),
	RevealTriggerRadius(5),
	ShipSinkingWeight(3.0), // RA2: the executable's initializer list, in RA2's member order and values
	IceCrackingWeight(2.0),
	IceBreakingWeight(4.0),
	CliffBackImpassability((char)0),
	VeteranRatio(10.0),
	VeteranCombat(1.0),
	VeteranSpeed(1.0),
	VeteranSight(1.0),
	VeteranArmor(1.0),
	VeteranROF(1.0),
	VeteranCap(1.0),
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
	BuildingDieSound(VOC_NONE), // RA2: the executable's initializer list, in RA2's member order and values
	BuildingSlam(VOC_NONE),
	RadarOn(VOC_NONE),
	RadarOff(VOC_NONE),
	MovieOn(-1), // RA2: the executable's initializer list, in RA2's member order and values
	MovieOff(-1),
	ScoldSound(VOC_NONE),
	TeslaCharge(VOC_NONE),
	TeslaZap(VOC_NONE),
	GenericClick(VOC_NONE),
	GenericBeep(VOC_NONE),
	BuildingDamageSound(VOC_NONE), // RA2: the executable's initializer list, in RA2's member order and values
	HealCrateSound(VOC_NONE),
	ChuteSound(VOC_NONE),
	StopSound(VOC_NONE),
	GuardSound(VOC_NONE),
	ScatterSound(VOC_NONE),
	DeploySound(VOC_NONE),
	StormSound(VOC_NONE), // RA2: the executable's initializer list, in RA2's member order and values
	ShellButtonSlideSound(VOC_NONE),
	WallBuildSpeedCoefficient(0.5),
	ChargeToDrainRatio(3.0),
	DamageToFirestormDamageCoefficient(0.1),
	TrackedUphill(1.0),
	TrackedDownhill(1.0),
	WheeledUphill(1.0),
	WheeledDownhill(1.0),
	SpotlightMovementRadius(2000),
	SpotlightLocationRadius(1000),
	SpotlightSpeed(0.05), // RA2: the executable's initializer list, in RA2's member order and values
	SpotlightAcceleration(0.005),
	SpotlightAngle(20.0),
	SpotlightRadius(175),
	WindDirection(FACING_NONE),
	CameraRange(2304), // RA2: the executable's initializer list, in RA2's member order and values
	FlightLevel(500),
	ParachuteMaxFallRate(-3), // RA2: the executable's initializer list, in RA2's member order and values
	NoParachuteMaxFallRate(-100),
	BuildingDrop(VOC_NONE),
	RepairBay(0), // RA2: the executable's initializer list, in RA2's member order and values
	GDIGateOne(0),
	GDIGateTwo(0),
	NodGateOne(0),
	NodGateTwo(0),
	WallTower(0),
	GDIPowerPlant(0),
	GDIPowerTurbine(0),
	NodRegularPower(0),
	NodAdvancedPower(0),
	GDIFirestormGenerator(0),
	GDIHunterSeeker(0),
	NodHunterSeeker(0),
	GDIWallDefense(6.0),
	GDIWallDefenseCoefficient(3.0),
	NodBaseDefenseCoefficient(1.0),
	GDIBaseDefenseCoefficient(1.0),
	ComputerBaseDefenseResponse(3),
	MaximumBaseDefenseValue(60),
	Smoke1(0), // RA2: the executable's initializer list, in RA2's member order and values
	Smoke2(0),
	FirestormActiveAnim(0),
	FirestormIdleAnim(0),
	FirestormAirAnim(0),
	FirestormGroundAnim(0),
	MoveFlash(0),
	BombParachute(0),
	Parachute(0),
	SmallFire(0),
	LargeFire(0),
	Paratrooper(0),
	EliteFlashTimer(100),
	ChronoDelay(60),
	ChronoReinfDelay(60),
	ChronoDistanceFactor(32),
	ChronoTrigger(true),
	ChronoMinimumDelay(0),
	ChronoRangeMinimum(0),
	AlliedDisguise(0),
	SovietDisguise(0),
	SpyPowerBlackout(0),
	SpyMoneyStealPercent(0.5f),
	AttackCursorOnDisguise(false),
	AIMinorSuperReadyPercent(0.8f),
	AISafeDistance(8),
	HarvesterTooFarDistance(5),
	ChronoHarvTooFarDistance(50),
	AIRestrictReplaceTime(500),
	ThreatPerOccupant(5),
	ApproachTargetResetMultiplier(1),
	CampaignMoneyDeltaEasy(5000),
	CampaignMoneyDeltaHard(-5000),
	GuardAreaTargetingDelay(36),
	NormalTargetingDelay(27),
	AINavalYardAdjacency(20),
	MaximumBuildingPlacementFailures(5),
	PurifierBonus(0.25f),
	Technician(0),
	Engineer(0),
	Pilot(0),
	AlliedCrew(0),
	SovietCrew(0),
	FlameDamage(0),
	FlameDamage2(0),
	NukeWarhead(0),
	NukeProjectile(0),
	NukeDown(0),
	EMPulseWarhead(0),
	EMPulseProjectile(0),
	C4Warhead(0),
	V3Warhead(0),
	DMislWarhead(0),
	V3EliteWarhead(0),
	DMislEliteWarhead(0),
	IvanWarhead(0),
	IvanDamage(100),
	IvanTimedDelay(450),
	CanDetonateTimeBomb(false),
	CanDetonateDeathBomb(false),
	IvanIconFlickerRate(8),
	DeathWeapon(0),
	BOMBCURS_SHP(0),
	IronCurtainDuration(0),
	IonCannonWarhead(0),
	FirestormWarhead(0),
	VeinholeWarhead(0),
	VeinholeTypeClass(0),
	InfantryBlinkDisguiseTime(0),
	DefaultLargeGreySmokeSystem(0),
	DefaultSmallGreySmokeSystem(0),
	DefaultSparkSystem(0),
	DefaultLargeRedSmokeSystem(0),
	DefaultSmallRedSmokeSystem(0),
	DefaultDebrisSmokeSystem(0),
	DefaultFireStreamSystem(0),
	DefaultFirestormExplosionSystem(0),
	DefaultTestParticleSystem(0),
	DefaultRepairParticleSystem(0),
	MyEffectivenessCoefficientDefault(0.0),
	TargetEffectivenessCoefficientDefault(0.0),
	TargetSpecialThreatCoefficientDefault(0.0),
	TargetStrengthCoefficientDefault(0.0),
	TargetDistanceCoefficientDefault(0.0),
	DumbMyEffectivenessCoefficient(0.0),
	DumbTargetEffectivenessCoefficient(0.0),
	DumbTargetSpecialThreatCoefficient(0.0),
	DumbTargetStrengthCoefficient(0.0),
	DumbTargetDistanceCoefficient(0.0),
	EnemyHouseThreatBonus(0.0),
	HunterSeekerDetonateProximity(0),
	HunterSeekerDescendProximity(0),
	HunterSeekerDescentSpeed(0),
	HunterSeekerAscentSpeed(0),
	HunterSeekerEmergeSpeed(0), // RA2: the executable's initializer list, in RA2's member order and values
	TurboBoost(1.5),
	AttackInterval(3.0), // RA2: the executable's initializer list, in RA2's member order and values
	AttackDelay(5.0),
	PowerEmergencyFraction(0.75),
	AirstripRatio(0.12),
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
	PatrolTime(0.016),
	DissolveUnfilledTeamDelay(5000),
	AIAlternateProductionCreditCutoff(1000),
	AIUseTurbineUpgradeChance(1.0), // RA2: the executable's initializer list, in RA2's member order and values
	CloakDelay(0.0),
	GameSpeedBias(1.0),
	NervousBias(1.0),
	ExplosionSpread(0.5),
	SupressRadius(256),
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
	SilverCrate((CrateType)CRATE_HEAL_BASE), // RA2: the executable's initializer list, in RA2's member order and values
	WoodCrate((CrateType)CRATE_MONEY),
	WaterCrate((CrateType)CRATE_MONEY),
	CrateMinimum(1),
	CrateMaximum(255),
	LZScanRadius(8192), // RA2: the executable's initializer list, in RA2's member order and values
	FlareAnim(0),
	MinMoney(2500),
	MPMoney(10000),
	MPMaxMoney(10000),
	MoneyIncrement(100), // RA2: the executable's initializer list, in RA2's member order and values
	MinUnitCount(1),
	MPUnitCount(10),
	MaxUnitCount(20), // RA2: the executable's initializer list, in RA2's member order and values
	MPBuildLevel(10),
	GameSpeed(6), // RA2: the executable's initializer list, in RA2's member order and values
	AIDifficultyStruct(1),
	AIPlayers(1),
	IsMPBridgeDestruction(true),
	IsMPShadowGrow(false), // RA2: the executable's initializer list, in RA2's member order and values
	Shroud(true),
	IsMPBasesOn(true),
	IsMPTiberiumGrow(true),
	IsMPCrates(true),
	IsMPCaptureTheFlag(false),
	HarvesterTruce(false), // RA2: the executable's initializer list, in RA2's member order and values
	MultiEngineer(false),
	AlliesAllowed(false),
	ShortGame(false),
	FogOfWar(false),
	MCVRedeploys(true),
	SuperWeaponsAllowed(true),
	BuildOffAlly(true),
	DropZoneRadius(1024),
	MessageDelay(0.6),
	SavourDelay(0.03),
	MaxPlayers(8),
	BaseDefenseDelay(0.25),
	SuspendPriority(20),
	SuspendDelay(2.0), // RA2: the executable's initializer list, in RA2's member order and values
	SurvivorFraction(0.5),
	AlliedSurvivorDivisor(200),
	SovietSurvivorDivisor(100),
	ReloadRate(0.05),
	AutocreateTime(5.0),
	BuildupTime(0.05),
	HarvesterLoadRate(2),
	HarvesterDumpRate(0.016), // RA2: the executable's initializer list, in RA2's member order and values
	AtomDamage(1000),
	GrowthRate(2.0), // RA2: the executable's initializer list, in RA2's member order and values
	ShroudRate(4.0),
	FogRate(0.05),
	IceGrowthRate(1.0),
	VeinGrowthRate(1.0),
	IceSolidifyDelay(500),
	AmbientLightChangeRate(0.2), // RA2: the executable's initializer list, in RA2's member order and values
	AmbientLightChangeStep(0.1),
	CrateTime(10.0),
	TimerWarning(2.0),
	NukeTime(14.0),
	EMPulseTime(5.0),
	IonCannonTime(10.0),
	FirestormTime(10.0),
	SpeakDelay(2.0),
	DamageDelay(1.0),
	Gravity(3),
	LeptonsPerSightIncrease(50),
	Incoming(MPH_IMMOBILE),
	MinDamage(1),
	MaxDamage(1000),
	RepairStep(5),
	RepairPercent(0.25), // RA2: the executable's initializer list, in RA2's member order and values
	IRepairStep(1),
	RepairRate(0.016), // RA2: the executable's initializer list, in RA2's member order and values
	URepairRate(0.016),
	IRepairRate(0.016),
	ConditionGreen(1.0),
	ConditionYellow(0.5),
	ConditionRed(0.5),
	RandomAnimateTime(0.083),
	CloseEnoughDistance(640),
	StrayDistance(512),
	RelaxedStray(512),
	CrushDistance(384),
	CrateRadius(640),
	HomingScatter(512),
	BallisticScatter(256),
	RefundPercent(0.5),
	BridgeStrength(1000),
	BuildSpeedBias(1.0), // RA2: the executable's initializer list, in RA2's member order and values
	C4Delay(0.03),
	RepairThreshhold(1000),
	PathDelay(0.016), // RA2: the executable's initializer list, in RA2's member order and values
	BlockagePathDelay(60),
	MovieTime(0.25),
	TiberiumShortScan(1536),
	TiberiumLongScan(8192),
	LightningDeferment(250),
	LightningDamage(200),
	LightningStormDuration(900),
	LightningHitDelay(90),
	LightningScatterDelay(10),
	LightningCellSpread(10),
	LightningSeparation(3),
	LightningPrintText(true),
	LightningWarhead(0),
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
	EngineerCaptureLevel(1.0f),
	EngineerDamage(0.0f),
	TalkBubbleTime(300),
	RadDurationMultiple(0),
	RadApplicationDelay(0),
	RadLevelMax(0),
	RadLevelDelay(0),
	RadLightDelay(0),
	RadLevelFactor(0.0),
	RadLightFactor(0.0),
	RadTintFactor(0.0),
	RadColor(0, 0, 0), // RA2: written out, as ra2-decomp's twin has it (the three-argument constructor at each colour)
	RadSiteWarhead(0),
	ElevationIncrement(0),
	ElevationIncrementBonus(1.0),
	ElevationBonusCap(0.0),
	AlliedWallTransparency(false),
	WallPenetratorThreshold(0.5),
	LocalRadarColor(255, 255, 255),
	LineTrailColorOverride(0, 0, 0),
	ChronoBeamColor(0, 0, 0),
	OreTwinkleChance(50),
	OreTwinkle(0)
{
	V3Rocket.PauseFrames = 0; // RA2: the executable's initializer list, in RA2's member order and values
	V3Rocket.TiltFrames = 60;
	V3Rocket.PitchInitial = 0.0f;
	V3Rocket.PitchFinal = 1.0f;
	V3Rocket.TurnRate = 0.05f;
	V3Rocket.Acceleration = 0.4f;
	V3Rocket.Altitude = 768;
	V3Rocket.Damage = 1000;
	V3Rocket.EliteDamage = 1000;
	V3Rocket.BodyLength = 256;
	V3Rocket.LazyCurve = true;
	V3Rocket.Type = 0;
	DMisl.PauseFrames = 10;
	DMisl.TiltFrames = 60;
	DMisl.PitchInitial = 0.0f;
	DMisl.PitchFinal = 1.0f;
	DMisl.TurnRate = 0.05f;
	DMisl.Acceleration = 0.4f;
	DMisl.Altitude = 768;
	DMisl.Damage = 1000;
	DMisl.EliteDamage = 1000;
	DMisl.BodyLength = 128;
	DMisl.LazyCurve = false;
	DMisl.Type = 0;
}



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
void RulesClass::Initialize(CCINIClass & ini) // RA2: a writable INI
{
	while (ColorSchemes.Count()) delete ColorSchemes[0];
	ColorSchemes.Clear();
	SchemeLibrary_Free();

	while (TeamTypes.Count()) delete TeamTypes[0]; // RA2: the executable's Initialize (no Firestorm or art ini loading, scheme library and session ini)
	while (TaskForces.Count()) delete TaskForces[0];
	while (ScriptTypes.Count()) delete ScriptTypes[0];
	while (OverlayTypes.Count()) delete OverlayTypes[0];
	while (SmudgeTypes.Count()) delete SmudgeTypes[0];
	while (TerrainTypes.Count()) delete TerrainTypes[0];
	while (InfantryTypes.Count()) delete InfantryTypes[0];
	while (UnitTypes.Count()) delete UnitTypes[0];
	while (AircraftTypes.Count()) delete AircraftTypes[0];
	while (BulletTypes.Count()) delete BulletTypes[0];
	while (BuildingTypes.Count()) delete BuildingTypes[0];
	while (SuperWeaponTypes.Count()) delete SuperWeaponTypes[0];
	while (AnimTypes.Count()) delete AnimTypes[0];
	while (Weapons.Count()) delete Weapons[0];
	while (::Warheads.Count()) delete ::Warheads[0];
	while (VoxelAnimTypes.Count()) delete VoxelAnimTypes[0];
	while (ParticleTypes.Count()) delete ParticleTypes[0];
	while (ParticleSystemTypes.Count()) delete ParticleSystemTypes[0];
	while (::Houses.Count()) delete ::Houses[0];
	while (HouseTypes.Count()) delete HouseTypes[0];

	Heap_Maximums(ini); // RA2: the [Maximums] reader, expanded here by the compiler (ra2-decomp's twin calls it the same way)
	Addition(ini);

	CCFileClass file("LANGRULE.INI"); // RA2: the executable's Initialize (no Firestorm or art ini loading, scheme library and session ini)
	if (file.Is_Available() == true) {
		CCINIClass langini;
		if (langini.Load(file, true) > 1) { // RA2: the executable's Initialize (no Firestorm or art ini loading, scheme library and session ini)
			return;
		}
		Addition(langini);
	}
	if (Session.Type && Session.MPGameMode) { // RA2: the executable's Initialize (no Firestorm or art ini loading, scheme library and session ini)
		Addition(*Session.MPGameMode->GetINI());
	}
}



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
	Elevation_Model(ini);
	Wall_Model(ini);
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
/// RA2: reads a sound effect named by an entry (a 128-byte buffer looked up with VocClass::From_Name), else the
/// default, also for an unknown name (ra2-decomp's rules.cpp helper, with main's body).
/// </summary>
static inline VocType Read_Sound(CCINIClass const & ini, char const * section, char const * entry, VocType defvalue)
{
	char buffer[128];
	if (ini.Get_String(section, entry, "", buffer, sizeof(buffer))) {
		VocType voc = VocClass::From_Name(buffer);
		if (voc != VOC_NONE) {
			return(voc);
		}
	}
	return(defvalue);
}


/// <summary>
/// Fetches the audio and visual control values.
/// This is the catch-all section for presentation -- the sounds the interface plays, the
/// animations attached to smoke, fire, parachutes and drop pods, the shroud and fog rates,
/// and the extra lighting the various object kinds are drawn with.
/// </summary>
/// <returns>bool; Was an audio visual section found in the control file?</returns>
bool RulesClass::Audio_Visual_Rules(CCINIClass const & ini)
{
	static char const * const RulesSection_AudioVisual = "AudioVisual"; // RA2: the executable's section name, a file-static pointer at 0x007A903C
	if (ini.Is_Present(RulesSection_AudioVisual)) {
		DetailMinFrameRateNormal = ini.Get_Int(RulesSection_AudioVisual, "DetailMinFrameRateNormal", DetailMinFrameRateNormal);
		DetailMinFrameRateMovie = ini.Get_Int(RulesSection_AudioVisual, "DetailMinFrameRateMovie", DetailMinFrameRateMovie);
		DetailBufferZoneWidth = ini.Get_Int(RulesSection_AudioVisual, "DetailBufferZoneWidth", DetailBufferZoneWidth);
		UnloadingHarvester = TGet_Class(ini, RulesSection_AudioVisual, "UnloadingHarvester", UnloadingHarvester);
		PoseDir = (Dir256)ini.Get_Int(RulesSection_AudioVisual, "PoseDir", PoseDir);
		DropPodPuff = TGet_Class(ini, RulesSection_AudioVisual, "DropPodPuff", DropPodPuff);
		WaypointAnimationSpeed = ini.Get_Int(RulesSection_AudioVisual, "WaypointAnimationSpeed", WaypointAnimationSpeed);
		BarrelExplode = TGet_Class(ini, RulesSection_AudioVisual, "BarrelExplode", BarrelExplode);
		BarrelDebris = TGet_TypeList<VoxelAnimTypeClass>(ini, RulesSection_AudioVisual, "BarrelDebris", BarrelDebris);
		BarrelParticle = TGet_Class(ini, RulesSection_AudioVisual, "BarrelParticle", BarrelParticle);
		Wake = TGet_Class(ini, RulesSection_AudioVisual, "Wake", Wake);
		FlamingInfantry = TGet_Class(ini, RulesSection_AudioVisual, "FlamingInfantry", FlamingInfantry);
		NukeTakeOff = TGet_Class(ini, RulesSection_AudioVisual, "NukeTakeOff", NukeTakeOff);
		InfantryHeadPop = TGet_Class(ini, RulesSection_AudioVisual, "InfantryHeadPop", InfantryHeadPop);
		InfantryNuked = TGet_Class(ini, RulesSection_AudioVisual, "InfantryNuked", InfantryNuked);
		Behind = TGet_Class(ini, RulesSection_AudioVisual, "Behind", Behind);
		VeinAttack = TGet_Class(ini, RulesSection_AudioVisual, "VeinAttack", VeinAttack);
		DropPod = TGet_TypeList<AnimTypeClass>(ini, RulesSection_AudioVisual, "DropPod", DropPod);
		VocType dig_sound = Read_Sound(ini, RulesSection_AudioVisual, "DigSound", (VocType)DigSound);
		DigSound = dig_sound;
		VocType guimain_button_sound = Read_Sound(ini, RulesSection_AudioVisual, "GUIMainButtonSound", (VocType)GUIMainButtonSound);
		GUIMainButtonSound = guimain_button_sound;
		VocType guibuild_sound = Read_Sound(ini, RulesSection_AudioVisual, "GUIBuildSound", (VocType)GUIBuildSound);
		GUIBuildSound = guibuild_sound;
		VocType guitab_sound = Read_Sound(ini, RulesSection_AudioVisual, "GUITabSound", (VocType)GUITabSound);
		GUITabSound = guitab_sound;
		VocType guiopen_sound = Read_Sound(ini, RulesSection_AudioVisual, "GUIOpenSound", (VocType)GUIOpenSound);
		GUIOpenSound = guiopen_sound;
		VocType guiclose_sound = Read_Sound(ini, RulesSection_AudioVisual, "GUICloseSound", (VocType)GUICloseSound);
		GUICloseSound = guiclose_sound;
		VocType guimove_out_sound = Read_Sound(ini, RulesSection_AudioVisual, "GUIMoveOutSound", (VocType)GUIMoveOutSound);
		GUIMoveOutSound = guimove_out_sound;
		VocType guimove_in_sound = Read_Sound(ini, RulesSection_AudioVisual, "GUIMoveInSound", (VocType)GUIMoveInSound);
		GUIMoveInSound = guimove_in_sound;
		VocType guicombo_open_sound = Read_Sound(ini, RulesSection_AudioVisual, "GUIComboOpenSound", (VocType)GUIComboOpenSound);
		GUIComboOpenSound = guicombo_open_sound;
		VocType guicombo_close_sound = Read_Sound(ini, RulesSection_AudioVisual, "GUIComboCloseSound", (VocType)GUIComboCloseSound);
		GUIComboCloseSound = guicombo_close_sound;
		VocType guicheckbox_sound = Read_Sound(ini, RulesSection_AudioVisual, "GUICheckboxSound", (VocType)GUICheckboxSound);
		GUICheckboxSound = guicheckbox_sound;
		VocType score_anim_sound = Read_Sound(ini, RulesSection_AudioVisual, "ScoreAnimSound", (VocType)ScoreAnimSound);
		ScoreAnimSound = score_anim_sound;
		VocType cheer_sound = Read_Sound(ini, RulesSection_AudioVisual, "CheerSound", (VocType)CheerSound);
		CheerSound = cheer_sound;
		VocType default_chrono_sound = Read_Sound(ini, RulesSection_AudioVisual, "DefaultChronoSound", (VocType)DefaultChronoSound);
		DefaultChronoSound = default_chrono_sound;
		VocType start_planning_mode_sound = Read_Sound(ini, RulesSection_AudioVisual, "StartPlanningModeSound", (VocType)StartPlanningModeSound);
		StartPlanningModeSound = start_planning_mode_sound;
		VocType end_planning_mode_sound = Read_Sound(ini, RulesSection_AudioVisual, "EndPlanningModeSound", (VocType)EndPlanningModeSound);
		EndPlanningModeSound = end_planning_mode_sound;
		VocType crate_money_sound = Read_Sound(ini, RulesSection_AudioVisual, "CrateMoneySound", (VocType)CrateMoneySound);
		CrateMoneySound = crate_money_sound;
		VocType crate_reveal_sound = Read_Sound(ini, RulesSection_AudioVisual, "CrateRevealSound", (VocType)CrateRevealSound);
		CrateRevealSound = crate_reveal_sound;
		VocType crate_fire_sound = Read_Sound(ini, RulesSection_AudioVisual, "CrateFireSound", (VocType)CrateFireSound);
		CrateFireSound = crate_fire_sound;
		VocType crate_armour_sound = Read_Sound(ini, RulesSection_AudioVisual, "CrateArmourSound", (VocType)CrateArmourSound);
		CrateArmourSound = crate_armour_sound;
		VocType crate_speed_sound = Read_Sound(ini, RulesSection_AudioVisual, "CrateSpeedSound", (VocType)CrateSpeedSound);
		CrateSpeedSound = crate_speed_sound;
		VocType crate_unit_sound = Read_Sound(ini, RulesSection_AudioVisual, "CrateUnitSound", (VocType)CrateUnitSound);
		CrateUnitSound = crate_unit_sound;
		VocType crate_promote_sound = Read_Sound(ini, RulesSection_AudioVisual, "CratePromoteSound", (VocType)CratePromoteSound);
		CratePromoteSound = crate_promote_sound;
		VocType impact_water_sound = Read_Sound(ini, RulesSection_AudioVisual, "ImpactWaterSound", (VocType)ImpactWaterSound);
		ImpactWaterSound = impact_water_sound;
		VocType impact_land_sound = Read_Sound(ini, RulesSection_AudioVisual, "ImpactLandSound", (VocType)ImpactLandSound);
		ImpactLandSound = impact_land_sound;
		VocType sinking_sound = Read_Sound(ini, RulesSection_AudioVisual, "SinkingSound", (VocType)SinkingSound);
		SinkingSound = sinking_sound;
		VocType chrono_in_sound = Read_Sound(ini, RulesSection_AudioVisual, "ChronoInSound", (VocType)ChronoInSound);
		ChronoInSound = chrono_in_sound;
		VocType chrono_out_sound = Read_Sound(ini, RulesSection_AudioVisual, "ChronoOutSound", (VocType)ChronoOutSound);
		ChronoOutSound = chrono_out_sound;
		VocType bomb_ticking_sound = Read_Sound(ini, RulesSection_AudioVisual, "BombTickingSound", (VocType)BombTickingSound);
		BombTickingSound = bomb_ticking_sound;
		VocType bomb_attach_sound = Read_Sound(ini, RulesSection_AudioVisual, "BombAttachSound", (VocType)BombAttachSound);
		BombAttachSound = bomb_attach_sound;
		VocType yuri_mind_control_sound = Read_Sound(ini, RulesSection_AudioVisual, "YuriMindControlSound", (VocType)YuriMindControlSound);
		YuriMindControlSound = yuri_mind_control_sound;
		VocType add_planning_mode_command_sound = Read_Sound(ini, RulesSection_AudioVisual, "AddPlanningModeCommandSound", (VocType)AddPlanningModeCommandSound);
		AddPlanningModeCommandSound = add_planning_mode_command_sound;
		VocType execute_plan_sound = Read_Sound(ini, RulesSection_AudioVisual, "ExecutePlanSound", (VocType)ExecutePlanSound);
		ExecutePlanSound = execute_plan_sound;
		VocType place_beacon_sound = Read_Sound(ini, RulesSection_AudioVisual, "PlaceBeaconSound", (VocType)PlaceBeaconSound);
		PlaceBeaconSound = place_beacon_sound;
		VocType building_garrisoned_sound = Read_Sound(ini, RulesSection_AudioVisual, "BuildingGarrisonedSound", (VocType)BuildingGarrisonedSound);
		BuildingGarrisonedSound = building_garrisoned_sound;
		VocType building_abandoned_sound = Read_Sound(ini, RulesSection_AudioVisual, "BuildingAbandonedSound", (VocType)BuildingAbandonedSound);
		BuildingAbandonedSound = building_abandoned_sound;
		VocType building_repaired_sound = Read_Sound(ini, RulesSection_AudioVisual, "BuildingRepairedSound", (VocType)BuildingRepairedSound);
		BuildingRepairedSound = building_repaired_sound;
		VocType base_under_attack_sound = Read_Sound(ini, RulesSection_AudioVisual, "BaseUnderAttackSound", (VocType)BaseUnderAttackSound);
		BaseUnderAttackSound = base_under_attack_sound;
		VocType upgrade_veteran_sound = Read_Sound(ini, RulesSection_AudioVisual, "UpgradeVeteranSound", (VocType)UpgradeVeteranSound);
		UpgradeVeteranSound = upgrade_veteran_sound;
		VocType upgrade_elite_sound = Read_Sound(ini, RulesSection_AudioVisual, "UpgradeEliteSound", (VocType)UpgradeEliteSound);
		UpgradeEliteSound = upgrade_elite_sound;
		VocType create_unit_sound = Read_Sound(ini, RulesSection_AudioVisual, "CreateUnitSound", (VocType)CreateUnitSound);
		CreateUnitSound = create_unit_sound;
		VocType create_infantry_sound = Read_Sound(ini, RulesSection_AudioVisual, "CreateInfantrySound", (VocType)CreateInfantrySound);
		CreateInfantrySound = create_infantry_sound;
		VocType create_aircraft_sound = Read_Sound(ini, RulesSection_AudioVisual, "CreateAircraftSound", (VocType)CreateAircraftSound);
		CreateAircraftSound = create_aircraft_sound;
		VocType ifvtransform_sound = Read_Sound(ini, RulesSection_AudioVisual, "IFVTransformSound", (VocType)IFVTransformSound);
		IFVTransformSound = ifvtransform_sound;
		VocType psychic_sensor_detect_sound = Read_Sound(ini, RulesSection_AudioVisual, "PsychicSensorDetectSound", (VocType)PsychicSensorDetectSound);
		PsychicSensorDetectSound = psychic_sensor_detect_sound;
		VocType spy_sat_activation_sound = Read_Sound(ini, RulesSection_AudioVisual, "SpySatActivationSound", (VocType)SpySatActivationSound);
		SpySatActivationSound = spy_sat_activation_sound;
		VocType spy_sat_deactivation_sound = Read_Sound(ini, RulesSection_AudioVisual, "SpySatDeactivationSound", (VocType)SpySatDeactivationSound);
		SpySatDeactivationSound = spy_sat_deactivation_sound;
		Dig = TGet_Class(ini, RulesSection_AudioVisual, "Dig", Dig);
		IonBlast = TGet_Class(ini, RulesSection_AudioVisual, "IonBlast", IonBlast);
		IonBeam = TGet_Class(ini, RulesSection_AudioVisual, "IonBeam", IonBeam);
		ChronoBeam = TGet_Class(ini, RulesSection_AudioVisual, "ChronoBeam", ChronoBeam);
		ChronoBlast = TGet_Class(ini, RulesSection_AudioVisual, "ChronoBlast", ChronoBlast);
		ChronoBlastDest = TGet_Class(ini, RulesSection_AudioVisual, "ChronoBlastDest", ChronoBlastDest);
		ChronoPlacement = TGet_Class(ini, RulesSection_AudioVisual, "ChronoPlacement", ChronoPlacement);
		WarpIn = TGet_Class(ini, RulesSection_AudioVisual, "WarpIn", WarpIn);
		WarpOut = TGet_Class(ini, RulesSection_AudioVisual, "WarpOut", WarpOut);
		WarpAway = TGet_Class(ini, RulesSection_AudioVisual, "WarpAway", WarpAway);
		ChronoSparkle1 = TGet_Class(ini, RulesSection_AudioVisual, "ChronoSparkle1", ChronoSparkle1);
		IronCurtainInvokeAnim = TGet_Class(ini, RulesSection_AudioVisual, "IronCurtainInvokeAnim", IronCurtainInvokeAnim);
		WeaponNullifyAnim = TGet_Class(ini, RulesSection_AudioVisual, "WeaponNullifyAnim", WeaponNullifyAnim);
		InfantryExplode = TGet_Class(ini, RulesSection_AudioVisual, "InfantryExplode", InfantryExplode);
		AtmosphereEntry = TGet_Class(ini, RulesSection_AudioVisual, "AtmosphereEntry", AtmosphereEntry);
		VocType gate_up = Read_Sound(ini, RulesSection_AudioVisual, "GateUp", (VocType)GateUpSound);
		GateUpSound = gate_up;
		VocType gate_down = Read_Sound(ini, RulesSection_AudioVisual, "GateDown", (VocType)GateDownSound);
		GateDownSound = gate_down;
		IsShroudGrow = ini.Get_Bool(RulesSection_AudioVisual, "ShroudGrow", IsShroudGrow);
		double scroll_multiplier = ini.Get_Float(RulesSection_AudioVisual, "ScrollMultiplier", ScrollMultiplier);
		ScrollMultiplier = scroll_multiplier;
		ShakeScreen = ini.Get_Int(RulesSection_AudioVisual, "ShakeScreen", ShakeScreen);
		VocType cloak_sound = Read_Sound(ini, RulesSection_AudioVisual, "CloakSound", (VocType)CloakSound);
		CloakSound = cloak_sound;
		VocType sell_sound = Read_Sound(ini, RulesSection_AudioVisual, "SellSound", (VocType)SellSound);
		SellSound = sell_sound;
		VocType game_closed = Read_Sound(ini, RulesSection_AudioVisual, "GameClosed", (VocType)GameClosed);
		GameClosed = game_closed;
		VocType incoming_message = Read_Sound(ini, RulesSection_AudioVisual, "IncomingMessage", (VocType)IncomingMessage);
		IncomingMessage = incoming_message;
		VocType message_char_typed = Read_Sound(ini, RulesSection_AudioVisual, "MessageCharTyped", (VocType)MessageCharTyped);
		MessageCharTyped = message_char_typed;
		VocType system_error = Read_Sound(ini, RulesSection_AudioVisual, "SystemError", (VocType)SystemError);
		SystemError = system_error;
		VocType options_changed = Read_Sound(ini, RulesSection_AudioVisual, "OptionsChanged", (VocType)OptionsChanged);
		OptionsChanged = options_changed;
		VocType game_forming = Read_Sound(ini, RulesSection_AudioVisual, "GameForming", (VocType)GameForming);
		GameForming = game_forming;
		VocType player_left = Read_Sound(ini, RulesSection_AudioVisual, "PlayerLeft", (VocType)PlayerLeft);
		PlayerLeft = player_left;
		VocType player_joined = Read_Sound(ini, RulesSection_AudioVisual, "PlayerJoined", (VocType)PlayerJoined);
		PlayerJoined = player_joined;
		VocType construction = Read_Sound(ini, RulesSection_AudioVisual, "Construction", (VocType)Construction);
		Construction = construction;
		CreditTicks = CCINIClass::Get_VocType_List(ini, RulesSection_AudioVisual, "CreditTicks", CreditTicks);
		VocType building_die_sound = Read_Sound(ini, RulesSection_AudioVisual, "BuildingDieSound", (VocType)BuildingDieSound);
		BuildingDieSound = building_die_sound;
		VocType building_slam = Read_Sound(ini, RulesSection_AudioVisual, "BuildingSlam", (VocType)BuildingSlam);
		BuildingSlam = building_slam;
		VocType radar_on = Read_Sound(ini, RulesSection_AudioVisual, "RadarOn", (VocType)RadarOn);
		RadarOn = radar_on;
		VocType radar_off = Read_Sound(ini, RulesSection_AudioVisual, "RadarOff", (VocType)RadarOff);
		RadarOff = radar_off;
		VocType movie_on = Read_Sound(ini, RulesSection_AudioVisual, "MovieOn", (VocType)MovieOn);
		MovieOn = movie_on;
		VocType movie_off = Read_Sound(ini, RulesSection_AudioVisual, "MovieOff", (VocType)MovieOff);
		MovieOff = movie_off;
		VocType scold_sound = Read_Sound(ini, RulesSection_AudioVisual, "ScoldSound", (VocType)ScoldSound);
		ScoldSound = scold_sound;
		VocType tesla_charge = Read_Sound(ini, RulesSection_AudioVisual, "TeslaCharge", (VocType)TeslaCharge);
		TeslaCharge = tesla_charge;
		VocType tesla_zap = Read_Sound(ini, RulesSection_AudioVisual, "TeslaZap", (VocType)TeslaZap);
		TeslaZap = tesla_zap;
		VocType building_damage_sound = Read_Sound(ini, RulesSection_AudioVisual, "BuildingDamageSound", (VocType)BuildingDamageSound);
		BuildingDamageSound = building_damage_sound;
		VocType chute_sound = Read_Sound(ini, RulesSection_AudioVisual, "ChuteSound", (VocType)ChuteSound);
		ChuteSound = chute_sound;
		VocType generic_click = Read_Sound(ini, RulesSection_AudioVisual, "GenericClick", (VocType)GenericClick);
		GenericClick = generic_click;
		VocType generic_beep = Read_Sound(ini, RulesSection_AudioVisual, "GenericBeep", (VocType)GenericBeep);
		GenericBeep = generic_beep;
		VocType building_drop = Read_Sound(ini, RulesSection_AudioVisual, "BuildingDrop", (VocType)BuildingDrop);
		BuildingDrop = building_drop;
		VocType stop_sound = Read_Sound(ini, RulesSection_AudioVisual, "StopSound", (VocType)StopSound);
		StopSound = stop_sound;
		VocType guard_sound = Read_Sound(ini, RulesSection_AudioVisual, "GuardSound", (VocType)GuardSound);
		GuardSound = guard_sound;
		VocType scatter_sound = Read_Sound(ini, RulesSection_AudioVisual, "ScatterSound", (VocType)ScatterSound);
		ScatterSound = scatter_sound;
		VocType storm_sound = Read_Sound(ini, RulesSection_AudioVisual, "StormSound", (VocType)StormSound);
		StormSound = storm_sound;
		LightningSounds = CCINIClass::Get_VocType_List(ini, RulesSection_AudioVisual, "LightningSounds", LightningSounds);
		VocType shell_button_slide_sound = Read_Sound(ini, RulesSection_AudioVisual, "ShellButtonSlideSound", (VocType)ShellButtonSlideSound);
		ShellButtonSlideSound = shell_button_slide_sound;
		TreeFire = TGet_TypeList<AnimTypeClass>(ini, RulesSection_AudioVisual, "TreeFire", TreeFire);
		DeadBodies = TGet_TypeList<AnimTypeClass>(ini, RulesSection_AudioVisual, "DeadBodies", DeadBodies);
		MetallicDebris = TGet_TypeList<AnimTypeClass>(ini, RulesSection_AudioVisual, "MetallicDebris", MetallicDebris);
		BridgeExplosions = TGet_TypeList<AnimTypeClass>(ini, RulesSection_AudioVisual, "BridgeExplosions", BridgeExplosions);
		DamageFireTypes = TGet_TypeList<AnimTypeClass>(ini, RulesSection_AudioVisual, "DamageFireTypes", DamageFireTypes);
		WeatherConClouds = TGet_TypeList<AnimTypeClass>(ini, RulesSection_AudioVisual, "WeatherConClouds", WeatherConClouds);
		WeatherConBolts = TGet_TypeList<AnimTypeClass>(ini, RulesSection_AudioVisual, "WeatherConBolts", WeatherConBolts);
		WeatherConBoltExplosion = TGet_Class(ini, RulesSection_AudioVisual, "WeatherConBoltExplosion", WeatherConBoltExplosion);
		OnFire = TGet_TypeList<AnimTypeClass>(ini, RulesSection_AudioVisual, "OnFire", OnFire);
		Smoke1 = TGet_Class(ini, RulesSection_AudioVisual, "Smoke", Smoke1);
		Smoke2 = TGet_Class(ini, RulesSection_AudioVisual, "Smoke", Smoke2);
		FirestormActiveAnim = TGet_Class(ini, RulesSection_AudioVisual, "FirestormActiveAnim", FirestormActiveAnim);
		FirestormIdleAnim = TGet_Class(ini, RulesSection_AudioVisual, "FirestormIdleAnim", FirestormIdleAnim);
		FirestormAirAnim = TGet_Class(ini, RulesSection_AudioVisual, "FirestormAirAnim", FirestormAirAnim);
		FirestormGroundAnim = TGet_Class(ini, RulesSection_AudioVisual, "FirestormGroundAnim", FirestormGroundAnim);
		MoveFlash = TGet_Class(ini, RulesSection_AudioVisual, "MoveFlash", MoveFlash);
		EliteFlashTimer = ini.Get_Int(RulesSection_AudioVisual, "EliteFlashTimer", EliteFlashTimer);
		Parachute = TGet_Class(ini, RulesSection_AudioVisual, "Parachute", Parachute);
		BombParachute = TGet_Class(ini, RulesSection_AudioVisual, "BombParachute", BombParachute);
		SmallFire = TGet_Class(ini, RulesSection_AudioVisual, "SmallFire", SmallFire);
		LargeFire = TGet_Class(ini, RulesSection_AudioVisual, "LargeFire", LargeFire);
		IsAllyReveal = ini.Get_Bool(RulesSection_AudioVisual, "AllyReveal", IsAllyReveal);
		ConditionGreen = 1;
		double condition_red = ini.Get_Float(RulesSection_AudioVisual, "ConditionRed", ConditionRed); // RA2: the executable's AudioVisual reader: RA2's keys and member order
		ConditionRed = condition_red;
		double condition_yellow = ini.Get_Float(RulesSection_AudioVisual, "ConditionYellow", ConditionYellow);
		ConditionYellow = condition_yellow;
		DropZoneRadius = ini.Get_Lepton(RulesSection_AudioVisual, "DropZoneRadius", DropZoneRadius);
		FlareAnim = TGet_Class(ini, RulesSection_AudioVisual, "DropZoneAnim", FlareAnim);
		IsHealthBar = ini.Get_Bool(RulesSection_AudioVisual, "EnemyHealth", IsHealthBar);
		Gravity = ini.Get_Int(RulesSection_AudioVisual, "Gravity", Gravity);
		double idle_action_frequency = ini.Get_Float(RulesSection_AudioVisual, "IdleActionFrequency", RandomAnimateTime);
		RandomAnimateTime = idle_action_frequency;
		double message_delay = ini.Get_Float(RulesSection_AudioVisual, "MessageDelay", MessageDelay);
		MessageDelay = message_delay;
		double movie_time = ini.Get_Float(RulesSection_AudioVisual, "MovieTime", MovieTime);
		MovieTime = movie_time;
		IsNamed = ini.Get_Bool(RulesSection_AudioVisual, "NamedCivilians", IsNamed);
		double savour_delay = ini.Get_Float(RulesSection_AudioVisual, "SavourDelay", SavourDelay);
		SavourDelay = savour_delay;
		double shroud_rate = ini.Get_Float(RulesSection_AudioVisual, "ShroudRate", ShroudRate);
		ShroudRate = shroud_rate;
		double fog_rate = ini.Get_Float(RulesSection_AudioVisual, "FogRate", FogRate);
		FogRate = fog_rate;
		double vein_growth_rate = ini.Get_Float(RulesSection_AudioVisual, "VeinGrowthRate", VeinGrowthRate);
		VeinGrowthRate = vein_growth_rate;
		double ice_growth_rate = ini.Get_Float(RulesSection_AudioVisual, "IceGrowthRate", IceGrowthRate);
		IceGrowthRate = ice_growth_rate;
		IceSolidifyDelay = ini.Get_Int(RulesSection_AudioVisual, "IceSolidifyFrameTime", IceSolidifyDelay);
		IceCrackSounds.Clear();
		IceCrackSounds = CCINIClass::Get_VocType_List(ini, RulesSection_AudioVisual, "IceCrackSounds", IceCrackSounds); // RA2: the executable's AudioVisual reader: RA2's keys and member order
		double ambient_change_rate = ini.Get_Float(RulesSection_AudioVisual, "AmbientChangeRate", AmbientLightChangeRate);
		AmbientLightChangeRate = ambient_change_rate;
		double ambient_change_step = ini.Get_Float(RulesSection_AudioVisual, "AmbientChangeStep", AmbientLightChangeStep);
		AmbientLightChangeStep = ambient_change_step;
		double speak_delay = ini.Get_Float(RulesSection_AudioVisual, "SpeakDelay", SpeakDelay);
		SpeakDelay = speak_delay;
		double timer_warning = ini.Get_Float(RulesSection_AudioVisual, "TimerWarning", TimerWarning);
		TimerWarning = timer_warning;
		ExtraUnitLight = ini.Get_Float(RulesSection_AudioVisual, "ExtraUnitLight", ExtraUnitLight / 1000) * 1000.0;
		ExtraInfantryLight = ini.Get_Float(RulesSection_AudioVisual, "ExtraInfantryLight", ExtraInfantryLight / 1000) * 1000.0;
		ExtraAircraftLight = ini.Get_Float(RulesSection_AudioVisual, "ExtraAircraftLight", ExtraAircraftLight / 1000) * 1000.0;
		EMPulseSparkles = TGet_Class(ini, RulesSection_AudioVisual, "EMPulseSparkles", EMPulseSparkles);
		LocalRadarColor = ini.Get_RGBClass(RulesSection_AudioVisual, "LocalRadarColor", LocalRadarColor);
		LineTrailColorOverride = ini.Get_RGBClass(RulesSection_AudioVisual, "LineTrailColorOverride", LineTrailColorOverride);
		ChronoBeamColor = ini.Get_RGBClass(RulesSection_AudioVisual, "ChronoBeamColor", ChronoBeamColor);
		OreTwinkleChance = ini.Get_Int(RulesSection_AudioVisual, "OreTwinkleChance", OreTwinkleChance);
		OreTwinkle = TGet_Class(ini, RulesSection_AudioVisual, "OreTwinkle", OreTwinkle);
		return(true);
	}
	return(false);
}



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
		WaterCrate = ini.Get_CrateType(CRATERULES, "WaterCrate", WaterCrate);
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
bool RulesClass::Combat_Damage(CCINIClass const & ini) // RA2: ra2-decomp's twin text (direct assignments, the two elite warhead blocks)
{
	static char const * const RulesSection_CombatDamage = "CombatDamage"; // RA2: the executable's section name, a file-static pointer at 0x007A9044
	if (ini.Is_Present(RulesSection_CombatDamage)) {
		AmmoCrateDamage = ini.Get_Int(RulesSection_CombatDamage, "AmmoCrateDamage", AmmoCrateDamage);
		IonCannonDamage = ini.Get_Int(RulesSection_CombatDamage, "IonCannonDamage", IonCannonDamage);
		RailgunDamageRadius = ini.Get_Int(RulesSection_CombatDamage, "RailgunDamageRadius", RailgunDamageRadius);
		TiberiumExplosionDamage = ini.Get_Int(RulesSection_CombatDamage, "TiberiumExplosionDamage", TiberiumExplosionDamage);
		TiberiumStrength = ini.Get_Int(RulesSection_CombatDamage, "TiberiumStrength", TiberiumStrength);
		Scorches = TGet_TypeList<SmudgeTypeClass>(ini, RulesSection_CombatDamage, "Scorches", Scorches);
		Scorches1 = TGet_TypeList<SmudgeTypeClass>(ini, RulesSection_CombatDamage, "Scorches1", Scorches1);
		Scorches2 = TGet_TypeList<SmudgeTypeClass>(ini, RulesSection_CombatDamage, "Scorches2", Scorches2);
		Scorches3 = TGet_TypeList<SmudgeTypeClass>(ini, RulesSection_CombatDamage, "Scorches3", Scorches3);
		Scorches4 = TGet_TypeList<SmudgeTypeClass>(ini, RulesSection_CombatDamage, "Scorches4", Scorches4);
		SplashList = TGet_TypeList<AnimTypeClass>(ini, RulesSection_CombatDamage, "SplashList", SplashList);
		FlameDamage = TGet_Class(ini, RulesSection_CombatDamage, "FlameDamage", FlameDamage);
		FlameDamage2 = TGet_Class(ini, RulesSection_CombatDamage, "FlameDamage2", FlameDamage2);
		C4Warhead = TGet_Class(ini, RulesSection_CombatDamage, "C4Warhead", C4Warhead);
		V3Warhead = TGet_Class(ini, RulesSection_CombatDamage, "V3Warhead", V3Warhead);
		DMislWarhead = TGet_Class(ini, RulesSection_CombatDamage, "DMislWarhead", DMislWarhead);
		{ // RA2: the elite warhead falls back to the plain one; written out as ra2-decomp's twin has it (the fallback is read before the name)
			char name[128];
			WarheadTypeClass const * fallback = V3Warhead;
			V3EliteWarhead = ini.Get_String(RulesSection_CombatDamage, "V3EliteWarhead", "", name, sizeof(name)) ? WarheadTypeClass::Find_Or_Make(name) : fallback;
		}
		{
			char name[128];
			WarheadTypeClass const * fallback = DMislWarhead;
			DMislEliteWarhead = ini.Get_String(RulesSection_CombatDamage, "DMislEliteWarhead", "", name, sizeof(name)) ? WarheadTypeClass::Find_Or_Make(name) : fallback;
		}
		IvanWarhead = TGet_Class(ini, RulesSection_CombatDamage, "IvanWarhead", IvanWarhead);
		CanDetonateTimeBomb = ini.Get_Bool(RulesSection_CombatDamage, "CanDetonateTimeBomb", CanDetonateTimeBomb);
		CanDetonateDeathBomb = ini.Get_Bool(RulesSection_CombatDamage, "CanDetonateDeathBomb", CanDetonateDeathBomb);
		DeathWeapon = TGet_Class(ini, RulesSection_CombatDamage, "DeathWeapon", DeathWeapon);
		IvanDamage = ini.Get_Int(RulesSection_CombatDamage, "IvanDamage", IvanDamage);
		IvanTimedDelay = ini.Get_Int(RulesSection_CombatDamage, "IvanTimedDelay", IvanTimedDelay);
		BOMBCURS_SHP = (ShapeSet const *)MixFileClass::Retrieve("BOMBCURS.SHP");
		IvanIconFlickerRate = ini.Get_Int(RulesSection_CombatDamage, "IvanIconFlickerRate", IvanIconFlickerRate);
		IronCurtainDuration = ini.Get_Int(RulesSection_CombatDamage, "IronCurtainDuration", IronCurtainDuration);
		IonCannonWarhead = TGet_Class(ini, RulesSection_CombatDamage, "IonCannonWarhead", IonCannonWarhead);
		FirestormWarhead = TGet_Class(ini, RulesSection_CombatDamage, "FirestormWarhead", FirestormWarhead);
		VeinholeWarhead = TGet_Class(ini, RulesSection_CombatDamage, "VeinholeWarhead", VeinholeWarhead);
		DefaultFirestormExplosionSystem = TGet_Class(ini, RulesSection_CombatDamage, "DefaultFirestormExplosionSystem", DefaultFirestormExplosionSystem);
		DefaultLargeGreySmokeSystem = TGet_Class(ini, RulesSection_CombatDamage, "DefaultLargeGreySmokeSystem", DefaultLargeGreySmokeSystem);
		DefaultSmallGreySmokeSystem = TGet_Class(ini, RulesSection_CombatDamage, "DefaultSmallGreySmokeSystem", DefaultSmallGreySmokeSystem);
		DefaultSparkSystem = TGet_Class(ini, RulesSection_CombatDamage, "DefaultSparkSystem", DefaultSparkSystem);
		DefaultLargeRedSmokeSystem = TGet_Class(ini, RulesSection_CombatDamage, "DefaultLargeRedSmokeSystem", DefaultLargeRedSmokeSystem);
		DefaultSmallRedSmokeSystem = TGet_Class(ini, RulesSection_CombatDamage, "DefaultSmallRedSmokeSystem", DefaultSmallRedSmokeSystem);
		DefaultDebrisSmokeSystem = TGet_Class(ini, RulesSection_CombatDamage, "DefaultDebrisSmokeSystem", DefaultDebrisSmokeSystem);
		DefaultFireStreamSystem = TGet_Class(ini, RulesSection_CombatDamage, "DefaultFireStreamSystem", DefaultFireStreamSystem);
		DefaultTestParticleSystem = TGet_Class(ini, RulesSection_CombatDamage, "DefaultTestParticleSystem", DefaultTestParticleSystem);
		DefaultRepairParticleSystem = TGet_Class(ini, RulesSection_CombatDamage, "DefaultRepairParticleSystem", DefaultRepairParticleSystem);
		IsBerzerkAllowed = ini.Get_Bool(RulesSection_CombatDamage, "BerzerkAllowed", IsBerzerkAllowed);
		TurboBoost = ini.Get_Float(RulesSection_CombatDamage, "TurboBoost", TurboBoost);
		AtomDamage = ini.Get_Int(RulesSection_CombatDamage, "AtomDamage", AtomDamage);
		BallisticScatter = ini.Get_Lepton(RulesSection_CombatDamage, "BallisticScatter", BallisticScatter);
		BridgeStrength = ini.Get_Int(RulesSection_CombatDamage, "BridgeStrength", BridgeStrength);
		C4Delay = ini.Get_Float(RulesSection_CombatDamage, "C4Delay", C4Delay);
		CrushDistance = ini.Get_Lepton(RulesSection_CombatDamage, "Crush", CrushDistance);
		ExplosionSpread = ini.Get_Float(RulesSection_CombatDamage, "ExpSpread", ExplosionSpread);
		SupressRadius = ini.Get_Lepton(RulesSection_CombatDamage, "FireSupress", SupressRadius);
		HomingScatter = ini.Get_Lepton(RulesSection_CombatDamage, "HomingScatter", HomingScatter);
		MaxDamage = ini.Get_Int(RulesSection_CombatDamage, "MaxDamage", MaxDamage);
		MinDamage = ini.Get_Int(RulesSection_CombatDamage, "MinDamage", MinDamage);
		IsExplosiveHarvester = ini.Get_Bool(RulesSection_CombatDamage, "TiberiumExplosive", IsExplosiveHarvester);
		IsAutoCrush = ini.Get_Bool(RulesSection_CombatDamage, "PlayerAutoCrush", IsAutoCrush);
		IsSmartDefense = ini.Get_Bool(RulesSection_CombatDamage, "PlayerReturnFire", IsSmartDefense);
		IsScatter = ini.Get_Bool(RulesSection_CombatDamage, "PlayerScatter", IsScatter);
		IsTreeTarget = ini.Get_Bool(RulesSection_CombatDamage, "TreeTargeting", IsTreeTarget);
		Incoming = ini.Get_MPHType(RulesSection_CombatDamage, "Incoming", Incoming);
		CollapseChance = ini.Get_Int(RulesSection_CombatDamage, "CollapseChance", CollapseChance);
		return(true);
	}
	return(false);
}



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
			HSVClass hsv = ini.Get_HSVClass(COLORS, entry, HSVClass());
			SchemeLibrary_Add(entry, hsv); // RA2: every scheme is also recorded in the scheme library (0x603A60)
			ColorScheme::Find_Or_Make(entry, hsv, SchemePalette, GamePalette, 1);
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
bool RulesClass::General(CCINIClass const & ini)
{
	static char const * const RulesSection_General = "General"; // RA2: the executable's section name, a file-static pointer at 0x007A9058
	if (ini.Is_Present(RulesSection_General)) {
		LargeVisceroid = TGet_Class(ini, RulesSection_General, "LargeVisceroid", LargeVisceroid);
		SmallVisceroid = TGet_Class(ini, RulesSection_General, "SmallVisceroid", SmallVisceroid);
		TiberiumHeal = ini.Get_Float(RulesSection_General, "TiberiumHeal", TiberiumHeal);
		PrerequisitePower = CCINIClass::Get_BuildingType_List(ini, RulesSection_General, "PrerequisitePower", PrerequisitePower);
		PrerequisiteFactory = CCINIClass::Get_BuildingType_List(ini, RulesSection_General, "PrerequisiteFactory", PrerequisiteFactory);
		PrerequisiteBarracks = CCINIClass::Get_BuildingType_List(ini, RulesSection_General, "PrerequisiteBarracks", PrerequisiteBarracks);
		PrerequisiteRadar = CCINIClass::Get_BuildingType_List(ini, RulesSection_General, "PrerequisiteRadar", PrerequisiteRadar);
		PrerequisiteTech = CCINIClass::Get_BuildingType_List(ini, RulesSection_General, "PrerequisiteTech", PrerequisiteTech);
		PrerequisiteProc = CCINIClass::Get_BuildingType_List(ini, RulesSection_General, "PrerequisiteProc", PrerequisiteProc);
		ZoomInFactor = ini.Get_Float(RulesSection_General, "ZoomInFactor", ZoomInFactor);
		IsRevealByHeight = ini.Get_Bool(RulesSection_General, "RevealByHeight", IsRevealByHeight);
		IsShroudedSubteranneanMovesAllowed = ini.Get_Bool(RulesSection_General, "AllowShroudedSubteranneanMoves", IsShroudedSubteranneanMovesAllowed);
		AircraftFogReveal = ini.Get_Int(RulesSection_General, "AircraftFogReveal", AircraftFogReveal);
		MinLowPowerProductionSpeed = ini.Get_Float(RulesSection_General, "MinLowPowerProductionSpeed", MinLowPowerProductionSpeed);
		MaxLowPowerProductionSpeed = ini.Get_Float(RulesSection_General, "MaxLowPowerProductionSpeed", MaxLowPowerProductionSpeed);
		LowPowerPenaltyModifier = ini.Get_Float(RulesSection_General, "LowPowerPenaltyModifier", LowPowerPenaltyModifier);
		MultipleFactory = ini.Get_Float(RulesSection_General, "MultipleFactory", MultipleFactory);
		MaximumCheerRate = ini.Get_Int(RulesSection_General, "MaximumCheerRate", MaximumCheerRate);
		TreeFlammability = ini.Get_Float(RulesSection_General, "TreeFlammability", TreeFlammability);
		MissileROTVar = ini.Get_Float(RulesSection_General, "MissileROTVar", MissileROTVar);
		MissileSafetyAltitude = ini.Get_Int(RulesSection_General, "MissileSafetyAltitude", MissileSafetyAltitude);
		MissileSpeedVar = ini.Get_Float(RulesSection_General, "MissileSpeedVar", MissileSpeedVar);
		DropPodWeapon = TGet_Class(ini, RulesSection_General, "DropPodWeapon", DropPodWeapon);
		DropPodHeight = ini.Get_Int(RulesSection_General, "DropPodHeight", DropPodHeight);
		DropPodSpeed = ini.Get_Int(RulesSection_General, "DropPodSpeed", DropPodSpeed);
		DropPodAngle = ini.Get_Float(RulesSection_General, "DropPodAngle", DropPodAngle);
		DropPodAngle = MIN(DropPodAngle, DEG_TO_RAD(67.5));
		DropPodAngle = MAX(DropPodAngle, DEG_TO_RAD(22.5));
		CrewEscape = ini.Get_Float(RulesSection_General, "CrewEscape", CrewEscape); // RA2: the executable's General reader: RA2's keys and member order
		TunnelSpeed = ini.Get_Float(RulesSection_General, "TunnelSpeed", TunnelSpeed);
		HoverDampen = ini.Get_Float(RulesSection_General, "HoverDampen", HoverDampen);
		HoverBob = ini.Get_Float(RulesSection_General, "HoverBob", HoverBob);
		HoverHeight = ini.Get_Int(RulesSection_General, "HoverHeight", HoverHeight);
		HoverBoost = ini.Get_Float(RulesSection_General, "HoverBoost", HoverBoost);
		HoverAcceleration = ini.Get_Float(RulesSection_General, "HoverAcceleration", HoverAcceleration);
		HoverBrake = ini.Get_Float(RulesSection_General, "HoverBrake", HoverBrake);
		VeteranRatio = ini.Get_Float(RulesSection_General, "VeteranRatio", VeteranRatio);
		VeteranCombat = ini.Get_Float(RulesSection_General, "VeteranCombat", VeteranCombat);
		VeteranSpeed = ini.Get_Float(RulesSection_General, "VeteranSpeed", VeteranSpeed);
		VeteranSight = ini.Get_Float(RulesSection_General, "VeteranSight", VeteranSight);
		VeteranArmor = ini.Get_Float(RulesSection_General, "VeteranArmor", VeteranArmor);
		VeteranROF = ini.Get_Float(RulesSection_General, "VeteranROF", VeteranROF);
		VeteranCap = ini.Get_Float(RulesSection_General, "VeteranCap", VeteranCap);
		ExplosiveVoxelDebris = TGet_TypeList<VoxelAnimTypeClass>(ini, RulesSection_General, "ExplosiveVoxelDebris", ExplosiveVoxelDebris);
		BridgeVoxelMax = ini.Get_Int(RulesSection_General, "BridgeVoxelMax", BridgeVoxelMax);
		TireVoxelDebris = TGet_Class(ini, RulesSection_General, "TireVoxelDebris", TireVoxelDebris);
		ScrapVoxelDebris = TGet_Class(ini, RulesSection_General, "ScrapVoxelDebris", ScrapVoxelDebris);
		CloakingStages = ini.Get_Int(RulesSection_General, "CloakingStages", CloakingStages);
		ShipSinkingWeight = ini.Get_Float(RulesSection_General, "ShipSinkingWeight", ShipSinkingWeight);
		IceCrackingWeight = ini.Get_Float(RulesSection_General, "IceCrackingWeight", IceCrackingWeight);
		IceBreakingWeight = ini.Get_Float(RulesSection_General, "IceBreakingWeight", IceBreakingWeight);
		CliffBackImpassability = ini.Get_Int(RulesSection_General, "CliffBackImpassability", CliffBackImpassability);
		PlacementDelay = ini.Get_Float(RulesSection_General, "PlacementDelay", PlacementDelay);
		TrackedUphill = ini.Get_Float(RulesSection_General, "TrackedUphill", TrackedUphill);
		TrackedDownhill = ini.Get_Float(RulesSection_General, "TrackedDownhill", TrackedDownhill);
		WheeledUphill = ini.Get_Float(RulesSection_General, "WheeledUphill", WheeledUphill);
		WheeledDownhill = ini.Get_Float(RulesSection_General, "WheeledDownhill", WheeledDownhill);
		WindDirection = (FacingType)ini.Get_Int(RulesSection_General, "WindDirection", WindDirection);
		CameraRange = ini.Get_Lepton(RulesSection_General, "CameraRange", CameraRange);
		FlightLevel = ini.Get_Int(RulesSection_General, "FlightLevel", FlightLevel);
		ParachuteMaxFallRate = ini.Get_Int(RulesSection_General, "ParachuteMaxFallRate", ParachuteMaxFallRate);
		NoParachuteMaxFallRate = ini.Get_Int(RulesSection_General, "NoParachuteMaxFallRate", NoParachuteMaxFallRate);
		RepairBay = TGet_Class(ini, RulesSection_General, "RepairBay", RepairBay);
		GDIGateOne = TGet_Class(ini, RulesSection_General, "GDIGateOne", GDIGateOne);
		GDIGateTwo = TGet_Class(ini, RulesSection_General, "GDIGateTwo", GDIGateTwo);
		NodGateOne = TGet_Class(ini, RulesSection_General, "NodGateOne", NodGateOne);
		NodGateTwo = TGet_Class(ini, RulesSection_General, "NodGateTwo", NodGateTwo);
		WallTower = TGet_Class(ini, RulesSection_General, "WallTower", WallTower);
		Shipyard = TGet_TypeList<BuildingTypeClass>(ini, RulesSection_General, "Shipyard", Shipyard);
		GDIPowerPlant = TGet_Class(ini, RulesSection_General, "GDIPowerPlant", GDIPowerPlant);
		GDIPowerTurbine = TGet_Class(ini, RulesSection_General, "GDIPowerTurbine", GDIPowerTurbine);
		NodRegularPower = TGet_Class(ini, RulesSection_General, "NodRegularPower", NodRegularPower);
		NodAdvancedPower = TGet_Class(ini, RulesSection_General, "NodAdvancedPower", NodAdvancedPower);
		GDIFirestormGenerator = TGet_Class(ini, RulesSection_General, "GDIFirestormGenerator", GDIFirestormGenerator);
		GDIHunterSeeker = TGet_Class(ini, RulesSection_General, "GDIHunterSeeker", GDIHunterSeeker);
		NodHunterSeeker = TGet_Class(ini, RulesSection_General, "NodHunterSeeker", NodHunterSeeker);
		BaseUnit = TGet_TypeList<UnitTypeClass>(ini, RulesSection_General, "BaseUnit", BaseUnit);
		HarvesterUnit = TGet_TypeList<UnitTypeClass>(ini, RulesSection_General, "HarvesterUnit", HarvesterUnit);
		PadAircraft = TGet_TypeList<AircraftTypeClass>(ini, RulesSection_General, "PadAircraft", PadAircraft);
		Paratrooper = TGet_Class(ini, RulesSection_General, "Paratrooper", Paratrooper);
		ChronoDelay = ini.Get_Int(RulesSection_General, "ChronoDelay", ChronoDelay);
		ChronoReinfDelay = ini.Get_Int(RulesSection_General, "ChronoReinfDelay", ChronoReinfDelay);
		ChronoDistanceFactor = ini.Get_Int(RulesSection_General, "ChronoDistanceFactor", ChronoDistanceFactor);
		ChronoTrigger = ini.Get_Bool(RulesSection_General, "ChronoTrigger", ChronoTrigger);
		ChronoMinimumDelay = ini.Get_Int(RulesSection_General, "ChronoMinimumDelay", ChronoMinimumDelay);
		ChronoRangeMinimum = ini.Get_Int(RulesSection_General, "ChronoRangeMinimum", ChronoRangeMinimum);
		AlliedDisguise = TGet_Class(ini, RulesSection_General, "AlliedDisguise", AlliedDisguise);
		SovietDisguise = TGet_Class(ini, RulesSection_General, "SovietDisguise", SovietDisguise);
		SpyPowerBlackout = ini.Get_Int(RulesSection_General, "SpyPowerBlackout", SpyPowerBlackout);
		SpyMoneyStealPercent = ini.Get_Float(RulesSection_General, "SpyMoneyStealPercent", SpyMoneyStealPercent);
		AttackCursorOnDisguise = ini.Get_Bool(RulesSection_General, "AttackCursorOnDisguise", AttackCursorOnDisguise);
		AIMinorSuperReadyPercent = ini.Get_Float(RulesSection_General, "AIMinorSuperReadyPercent", AIMinorSuperReadyPercent);
		AISafeDistance = ini.Get_Int(RulesSection_General, "AISafeDistance", AISafeDistance);
		AINavalYardAdjacency = ini.Get_Int(RulesSection_General, "AINavalYardAdjacency", AINavalYardAdjacency);
		MaximumBuildingPlacementFailures = ini.Get_Int(RulesSection_General, "MaximumBuildingPlacementFailures", MaximumBuildingPlacementFailures);
		HarvesterTooFarDistance = ini.Get_Int(RulesSection_General, "HarvesterTooFarDistance", HarvesterTooFarDistance);
		ApproachTargetResetMultiplier = ini.Get_Int(RulesSection_General, "ApproachTargetResetMultiplier", ApproachTargetResetMultiplier);
		CampaignMoneyDeltaEasy = ini.Get_Int(RulesSection_General, "CampaignMoneyDeltaEasy", CampaignMoneyDeltaEasy);
		CampaignMoneyDeltaHard = ini.Get_Int(RulesSection_General, "CampaignMoneyDeltaHard", CampaignMoneyDeltaHard);
		GuardAreaTargetingDelay = ini.Get_Int(RulesSection_General, "GuardAreaTargetingDelay", GuardAreaTargetingDelay);
		NormalTargetingDelay = ini.Get_Int(RulesSection_General, "NormalTargetingDelay", NormalTargetingDelay);
		ChronoHarvTooFarDistance = ini.Get_Int(RulesSection_General, "ChronoHarvTooFarDistance", ChronoHarvTooFarDistance);
		AIRestrictReplaceTime = ini.Get_Int(RulesSection_General, "AIRestrictReplaceTime", AIRestrictReplaceTime);
		ThreatPerOccupant = ini.Get_Int(RulesSection_General, "ThreatPerOccupant", ThreatPerOccupant);
		PurifierBonus = ini.Get_Float(RulesSection_General, "PurifierBonus", PurifierBonus);
		Engineer = TGet_Class(ini, RulesSection_General, "Engineer", Engineer);
		Technician = TGet_Class(ini, RulesSection_General, "Technician", Technician);
		Pilot = TGet_Class(ini, RulesSection_General, "Pilot", Pilot);
		AlliedCrew = TGet_Class(ini, RulesSection_General, "AlliedCrew", AlliedCrew);
		SovietCrew = TGet_Class(ini, RulesSection_General, "SovietCrew", SovietCrew);
		IsCurleyShuffle = ini.Get_Bool(RulesSection_General, "CurleyShuffle", IsCurleyShuffle);
		IsFineDifficulty = ini.Get_Bool(RulesSection_General, "FineDiffControl", IsFineDifficulty);
		TeamDelays = ini.Get_IntList(RulesSection_General, "TeamDelays", TeamDelays);
		AlliedBaseDefenseCounts = ini.Get_IntList(RulesSection_General, "AlliedBaseDefenseCounts", AlliedBaseDefenseCounts);
		SovietBaseDefenseCounts = ini.Get_IntList(RulesSection_General, "SovietBaseDefenseCounts", SovietBaseDefenseCounts);
		DisabledDisguiseDetectionPercent = ini.Get_IntList(RulesSection_General, "DisabledDisguiseDetectionPercent", DisabledDisguiseDetectionPercent);
		AIAutoDeployFrameDelay = ini.Get_IntList(RulesSection_General, "AIAutoDeployFrameDelay", AIAutoDeployFrameDelay);
		AIPickWallDefensePercent = ini.Get_IntList(RulesSection_General, "AIPickWallDefensePercent", AIPickWallDefensePercent);
		AIHateDelays = ini.Get_IntList(RulesSection_General, "AIHateDelays", AIHateDelays);
		AIAlternateProductionCreditCutoff = ini.Get_Int(RulesSection_General, "AIAlternateProductionCreditCutoff", AIAlternateProductionCreditCutoff);
		AIUseTurbineUpgradeChance = ini.Get_Float(RulesSection_General, "AIUseTurbineUpgradeProbability", AIUseTurbineUpgradeChance);
		NodAIBuildsWalls = ini.Get_Bool(RulesSection_General, "NodAIBuildsWalls", NodAIBuildsWalls);
		AIBuildsWalls = ini.Get_Bool(RulesSection_General, "AIBuildsWalls", AIBuildsWalls);
		FillEarliestTeamProbability = ini.Get_IntList(RulesSection_General, "FillEarliestTeamProbability", FillEarliestTeamProbability);
		MinimumAIDefensiveTeams = ini.Get_IntList(RulesSection_General, "MinimumAIDefensiveTeams", MinimumAIDefensiveTeams);
		MaximumAIDefensiveTeams = ini.Get_IntList(RulesSection_General, "MaximumAIDefensiveTeams", MaximumAIDefensiveTeams);
		TotalAITeamCap = ini.Get_IntList(RulesSection_General, "TotalAITeamCap", TotalAITeamCap);
		UseMinDefenseRule = ini.Get_Bool(RulesSection_General, "UseMinDefenseRule", UseMinDefenseRule);
		DissolveUnfilledTeamDelay = ini.Get_Int(RulesSection_General, "DissolveUnfilledTeamDelay", DissolveUnfilledTeamDelay);
		MultiplayerAICreditMultipliers = ini.Get_IntList(RulesSection_General, "MultiplayerAICM", MultiplayerAICreditMultipliers);
		AIVirtualPurifiers = ini.Get_IntList(RulesSection_General, "AIVirtualPurifiers", AIVirtualPurifiers);
		AmerParaDropInf = TGet_TypeList<InfantryTypeClass>(ini, RulesSection_General, "AmerParaDropInf", AmerParaDropInf);
		AmerParaDropNum = ini.Get_IntList(RulesSection_General, "AmerParaDropNum", AmerParaDropNum);
		AllyParaDropInf = TGet_TypeList<InfantryTypeClass>(ini, RulesSection_General, "AllyParaDropInf", AllyParaDropInf);
		AllyParaDropNum = ini.Get_IntList(RulesSection_General, "AllyParaDropNum", AllyParaDropNum);
		SovParaDropInf = TGet_TypeList<InfantryTypeClass>(ini, RulesSection_General, "SovParaDropInf", SovParaDropInf);
		SovParaDropNum = ini.Get_IntList(RulesSection_General, "SovParaDropNum", SovParaDropNum);
		AIIonCannonConYardValue = ini.Get_IntList(RulesSection_General, "AIIonCannonConYardValue", AIIonCannonConYardValue);
		AIIonCannonWarFactoryValue = ini.Get_IntList(RulesSection_General, "AIIonCannonWarFactoryValue", AIIonCannonWarFactoryValue);
		AIIonCannonPowerValue = ini.Get_IntList(RulesSection_General, "AIIonCannonPowerValue", AIIonCannonPowerValue);
		AIIonCannonEngineerValue = ini.Get_IntList(RulesSection_General, "AIIonCannonEngineerValue", AIIonCannonEngineerValue);
		AIIonCannonThiefValue = ini.Get_IntList(RulesSection_General, "AIIonCannonThiefValue", AIIonCannonThiefValue);
		AIIonCannonHarvesterValue = ini.Get_IntList(RulesSection_General, "AIIonCannonHarvesterValue", AIIonCannonHarvesterValue);
		AIIonCannonMCVValue = ini.Get_IntList(RulesSection_General, "AIIonCannonMCVValue", AIIonCannonMCVValue);
		AIIonCannonAPCValue = ini.Get_IntList(RulesSection_General, "AIIonCannonAPCValue", AIIonCannonAPCValue);
		AIIonCannonBaseDefenseValue = ini.Get_IntList(RulesSection_General, "AIIonCannonBaseDefenseValue", AIIonCannonBaseDefenseValue);
		AIIonCannonPlugValue = ini.Get_IntList(RulesSection_General, "AIIonCannonPlugValue", AIIonCannonPlugValue);
		AIIonCannonHelipadValue = ini.Get_IntList(RulesSection_General, "AIIonCannonHelipadValue", AIIonCannonHelipadValue);
		AIIonCannonTempleValue = ini.Get_IntList(RulesSection_General, "AIIonCannonTempleValue", AIIonCannonTempleValue);
		CloakDelay = ini.Get_Float(RulesSection_General, "CloakDelay", CloakDelay);
		GameSpeedBias = ini.Get_Float(RulesSection_General, "GameSpeedBias", GameSpeedBias);
		NervousBias = ini.Get_Float(RulesSection_General, "BaseBias", NervousBias);
		IsSeparate = ini.Get_Bool(RulesSection_General, "SeparateAircraft", IsSeparate);
		BaseDefenseDelay = ini.Get_Float(RulesSection_General, "BaseDefenseDelay", BaseDefenseDelay);
		SuspendPriority = ini.Get_Int(RulesSection_General, "SuspendPriority", SuspendPriority);
		SuspendDelay = ini.Get_Float(RulesSection_General, "SuspendDelay", SuspendDelay);
		SurvivorFraction = ini.Get_Float(RulesSection_General, "SurvivorRate", SurvivorFraction);
		AlliedSurvivorDivisor = ini.Get_Int(RulesSection_General, "AlliedSurvivorDivisor", AlliedSurvivorDivisor);
		SovietSurvivorDivisor = ini.Get_Int(RulesSection_General, "SovietSurvivorDivisor", SovietSurvivorDivisor);
		ReloadRate = ini.Get_Float(RulesSection_General, "ReloadRate", ReloadRate);
		BuildupTime = ini.Get_Float(RulesSection_General, "BuildupTime", BuildupTime);
		HarvesterDumpRate = ini.Get_Float(RulesSection_General, "HarvesterDumpRate", HarvesterDumpRate);
		HarvesterLoadRate = ini.Get_Int(RulesSection_General, "HarvesterLoadRate", HarvesterLoadRate);
		BuildSpeedBias = ini.Get_Float(RulesSection_General, "BuildSpeed", BuildSpeedBias);
		DamageDelay = ini.Get_Float(RulesSection_General, "DamageDelay", DamageDelay);
		GrowthRate = ini.Get_Float(RulesSection_General, "GrowthRate", GrowthRate);
		RefundPercent = ini.Get_Float(RulesSection_General, "RefundPercent", RefundPercent);
		RepairPercent = ini.Get_Float(RulesSection_General, "RepairPercent", RepairPercent);
		RepairStep = ini.Get_Int(RulesSection_General, "RepairStep", RepairStep);
		IRepairStep = ini.Get_Int(RulesSection_General, "IRepairStep", IRepairStep);
		RepairRate = ini.Get_Float(RulesSection_General, "RepairRate", RepairRate);
		URepairRate = ini.Get_Float(RulesSection_General, "URepairRate", URepairRate);
		IRepairRate = ini.Get_Float(RulesSection_General, "IRepairRate", IRepairRate);
		StrayDistance = ini.Get_Lepton(RulesSection_General, "Stray", StrayDistance);
		RelaxedStray = ini.Get_Lepton(RulesSection_General, "RelaxedStray", RelaxedStray);
		GuardModeStray = ini.Get_Lepton(RulesSection_General, "GuardModeStray", GuardModeStray);
		CloseEnoughDistance = ini.Get_Lepton(RulesSection_General, "CloseEnough", CloseEnoughDistance);
		IsBlendedFog = ini.Get_Bool(RulesSection_General, "BlendedFog", IsBlendedFog);
		AttackingAircraftSightRange = ini.Get_Int(RulesSection_General, "AttackingAircraftSightRange", AttackingAircraftSightRange);
		LeptonsPerSightIncrease = ini.Get_Int(RulesSection_General, "LeptonsPerSightIncrease", LeptonsPerSightIncrease);
		TiberiumTransmogrify = ini.Get_Int(RulesSection_General, "TiberiumTransmogrify", TiberiumTransmogrify);
		LightningDeferment = ini.Get_Int(RulesSection_General, "LightningDeferment", LightningDeferment);
		LightningDamage = ini.Get_Int(RulesSection_General, "LightningDamage", LightningDamage);
		LightningStormDuration = ini.Get_Int(RulesSection_General, "LightningStormDuration", LightningStormDuration);
		LightningHitDelay = ini.Get_Int(RulesSection_General, "LightningHitDelay", LightningHitDelay);
		LightningScatterDelay = ini.Get_Int(RulesSection_General, "LightningScatterDelay", LightningScatterDelay);
		LightningCellSpread = ini.Get_Int(RulesSection_General, "LightningCellSpread", LightningCellSpread);
		LightningSeparation = ini.Get_Int(RulesSection_General, "LightningSeparation", LightningSeparation);
		LightningWarhead = TGet_Class(ini, RulesSection_General, "LightningWarhead", LightningWarhead);  /* out of line 653da0 */
		LightningPrintText = ini.Get_Bool(RulesSection_General, "LightningPrintText", LightningPrintText);
		PrismType = TGet_Class(ini, RulesSection_General, "PrismType", PrismType);  /* out of line 654060 */
		PrismSupportModifier = ini.Get_Float(RulesSection_General, "PrismSupportModifier", PrismSupportModifier) * 100.0;
		PrismSupportMax = ini.Get_Int(RulesSection_General, "PrismSupportMax", PrismSupportMax);
		PrismSupportDelay = ini.Get_Int(RulesSection_General, "PrismSupportDelay", PrismSupportDelay);
		PrismSupportDuration = ini.Get_Int(RulesSection_General, "PrismSupportDuration", PrismSupportDuration);
		PrismSupportHeight = ini.Get_Int(RulesSection_General, "PrismSupportHeight", PrismSupportHeight);
		V3Rocket.PauseFrames = ini.Get_Int(RulesSection_General, "V3RocketPauseFrames", V3Rocket.PauseFrames);
		V3Rocket.TiltFrames = ini.Get_Int(RulesSection_General, "V3RocketTiltFrames", V3Rocket.TiltFrames);
		V3Rocket.PitchInitial = ini.Get_Float(RulesSection_General, "V3RocketPitchInitial", V3Rocket.PitchInitial);
		V3Rocket.PitchFinal = ini.Get_Float(RulesSection_General, "V3RocketPitchFinal", V3Rocket.PitchFinal);
		V3Rocket.TurnRate = ini.Get_Float(RulesSection_General, "V3RocketTurnRate", V3Rocket.TurnRate);
		V3Rocket.Acceleration = ini.Get_Float(RulesSection_General, "V3RocketAcceleration", V3Rocket.Acceleration);
		V3Rocket.Altitude = ini.Get_Int(RulesSection_General, "V3RocketAltitude", V3Rocket.Altitude);
		V3Rocket.Damage = ini.Get_Int(RulesSection_General, "V3RocketDamage", V3Rocket.Damage);
		V3Rocket.EliteDamage = ini.Get_Int(RulesSection_General, "V3RocketEliteDamage", V3Rocket.EliteDamage);
		V3Rocket.BodyLength = ini.Get_Int(RulesSection_General, "V3RocketBodyLength", V3Rocket.BodyLength);
		V3Rocket.LazyCurve = ini.Get_Bool(RulesSection_General, "V3RocketLazyCurve", V3Rocket.LazyCurve);
		V3Rocket.Type = TGet_Class(ini, RulesSection_General, "V3RocketType", V3Rocket.Type);  /* out of line 6540b0 */
		DMisl.PauseFrames = ini.Get_Int(RulesSection_General, "DMislPauseFrames", DMisl.PauseFrames);
		DMisl.TiltFrames = ini.Get_Int(RulesSection_General, "DMislTiltFrames", DMisl.TiltFrames);
		DMisl.PitchInitial = ini.Get_Float(RulesSection_General, "DMislPitchInitial", DMisl.PitchInitial);
		DMisl.PitchFinal = ini.Get_Float(RulesSection_General, "DMislPitchFinal", DMisl.PitchFinal);
		DMisl.TurnRate = ini.Get_Float(RulesSection_General, "DMislTurnRate", DMisl.TurnRate);
		DMisl.Acceleration = ini.Get_Float(RulesSection_General, "DMislAcceleration", DMisl.Acceleration);
		DMisl.Altitude = ini.Get_Int(RulesSection_General, "DMislAltitude", DMisl.Altitude);
		DMisl.Damage = ini.Get_Int(RulesSection_General, "DMislDamage", DMisl.Damage);
		DMisl.EliteDamage = ini.Get_Int(RulesSection_General, "DMislEliteDamage", DMisl.EliteDamage);
		DMisl.BodyLength = ini.Get_Int(RulesSection_General, "DMislBodyLength", DMisl.BodyLength);
		DMisl.LazyCurve = ini.Get_Bool(RulesSection_General, "DMislLazyCurve", DMisl.LazyCurve);
		DMisl.Type = TGet_Class(ini, RulesSection_General, "DMislType", DMisl.Type);  /* out of line 6540b0 */
		ParadropRadius = ini.Get_Int(RulesSection_General, "ParadropRadius", ParadropRadius);
		SpotlightMovementRadius = ini.Get_Int(RulesSection_General, "SpotlightMovementRadius", SpotlightMovementRadius);
		SpotlightLocationRadius = ini.Get_Int(RulesSection_General, "SpotlightLocationRadius", SpotlightLocationRadius);
		SpotlightSpeed = ini.Get_Float(RulesSection_General, "SpotlightSpeed", SpotlightSpeed);
		SpotlightAcceleration = ini.Get_Float(RulesSection_General, "SpotlightAcceleration", SpotlightAcceleration);
		SpotlightAngle = ini.Get_Float(RulesSection_General, "SpotlightAngle", SpotlightAngle);
		SpotlightRadius = ini.Get_Int(RulesSection_General, "SpotlightRadius", SpotlightRadius);
		RevealTriggerRadius = ini.Get_Int(RulesSection_General, "RevealTriggerRadius", RevealTriggerRadius);
		ChargeToDrainRatio = ini.Get_Float(RulesSection_General, "ChargeToDrainRatio", ChargeToDrainRatio);
		DamageToFirestormDamageCoefficient = ini.Get_Float(RulesSection_General, "DamageToFirestormDamageCoefficient", DamageToFirestormDamageCoefficient);
		WallBuildSpeedCoefficient = ini.Get_Float(RulesSection_General, "WallBuildSpeedCoefficient", WallBuildSpeedCoefficient);
		ConditionYellowSparkingProbability = ini.Get_Float(RulesSection_General, "ConditionYellowSparkingProbability", ConditionYellowSparkingProbability);
		ConditionRedSparkingProbability = ini.Get_Float(RulesSection_General, "ConditionRedSparkingProbability", ConditionRedSparkingProbability);
		AITriggerSuccessWeightDelta = ini.Get_Float(RulesSection_General, "AITriggerSuccessWeightDelta", AITriggerSuccessWeightDelta);
		AITriggerFailureWeightDelta = ini.Get_Float(RulesSection_General, "AITriggerFailureWeightDelta", AITriggerFailureWeightDelta);
		AITriggerTrackRecordCoefficient = ini.Get_Float(RulesSection_General, "AITriggerTrackRecordCoefficient", AITriggerTrackRecordCoefficient);
		WeedCapacity = ini.Get_Int(RulesSection_General, "WeedCapacity", WeedCapacity);
		HarvestersPerRefinery = ini.Get_Int(RulesSection_General, "HarvestersPerRefinery", HarvestersPerRefinery);
		FlashFrameTime = ini.Get_Int(RulesSection_General, "FlashFrameTime", FlashFrameTime);
		RadarCombatFlashTime = ini.Get_Int(RulesSection_General, "RadarCombatFlashTime", RadarCombatFlashTime);
		RadarEventSpeed = ini.Get_Float(RulesSection_General, "RadarEventSpeed", RadarEventSpeed);
		RadarEventRotationSpeed = ini.Get_Float(RulesSection_General, "RadarEventRotationSpeed", RadarEventRotationSpeed);
		RadarEventSuppressionDistances = ini.Get_IntList(RulesSection_General, "RadarEventSuppressionDistances", RadarEventSuppressionDistances);
		RadarEventVisibilityDurations = ini.Get_IntList(RulesSection_General, "RadarEventVisibilityDurations", RadarEventVisibilityDurations);
		RadarEventDurations = ini.Get_IntList(RulesSection_General, "RadarEventDurations", RadarEventDurations);
		RadarEventMinRadius = ini.Get_Int(RulesSection_General, "RadarEventMinRadius", RadarEventMinRadius);
		RadarEventColorSpeed = ini.Get_Float(RulesSection_General, "RadarEventColorSpeed", RadarEventColorSpeed);
		HunterSeekerDetonateProximity = ini.Get_Int(RulesSection_General, "HunterSeekerDetonateProximity", HunterSeekerDetonateProximity);
		HunterSeekerDescendProximity = ini.Get_Int(RulesSection_General, "HunterSeekerDescendProximity", HunterSeekerDescendProximity);
		HunterSeekerDescentSpeed = ini.Get_Int(RulesSection_General, "HunterSeekerDescentSpeed", HunterSeekerDescentSpeed);
		HunterSeekerAscentSpeed = ini.Get_Int(RulesSection_General, "HunterSeekerAscentSpeed", HunterSeekerAscentSpeed);
		HunterSeekerEmergeSpeed = ini.Get_Int(RulesSection_General, "HunterSeekerEmergeSpeed", HunterSeekerEmergeSpeed);
		MyEffectivenessCoefficientDefault = ini.Get_Float(RulesSection_General, "MyEffectivenessCoefficientDefault", MyEffectivenessCoefficientDefault);
		TargetEffectivenessCoefficientDefault = ini.Get_Float(RulesSection_General, "TargetEffectivenessCoefficientDefault", TargetEffectivenessCoefficientDefault);
		TargetSpecialThreatCoefficientDefault = ini.Get_Float(RulesSection_General, "TargetSpecialThreatCoefficientDefault", TargetSpecialThreatCoefficientDefault);
		TargetStrengthCoefficientDefault = ini.Get_Float(RulesSection_General, "TargetStrengthCoefficientDefault", TargetStrengthCoefficientDefault);
		TargetDistanceCoefficientDefault = ini.Get_Float(RulesSection_General, "TargetDistanceCoefficientDefault", TargetDistanceCoefficientDefault);
		DumbMyEffectivenessCoefficient = ini.Get_Float(RulesSection_General, "DumbMyEffectivenessCoefficient", DumbMyEffectivenessCoefficient);
		DumbTargetEffectivenessCoefficient = ini.Get_Float(RulesSection_General, "DumbTargetEffectivenessCoefficient", DumbTargetEffectivenessCoefficient);
		DumbTargetSpecialThreatCoefficient = ini.Get_Float(RulesSection_General, "DumbTargetSpecialThreatCoefficient", DumbTargetSpecialThreatCoefficient);
		DumbTargetStrengthCoefficient = ini.Get_Float(RulesSection_General, "DumbTargetStrengthCoefficient", DumbTargetStrengthCoefficient);
		DumbTargetDistanceCoefficient = ini.Get_Float(RulesSection_General, "DumbTargetDistanceCoefficient", DumbTargetDistanceCoefficient);
		EnemyHouseThreatBonus = ini.Get_Float(RulesSection_General, "EnemyHouseThreatBonus", EnemyHouseThreatBonus);
		VeinholeMonsterStrength = ini.Get_Int(RulesSection_General, "VeinholeMonsterStrength", VeinholeMonsterStrength);
		MaxVeinholeGrowth = ini.Get_Int(RulesSection_General, "MaxVeinholeGrowth", MaxVeinholeGrowth);
		VeinholeGrowthRate = ini.Get_Int(RulesSection_General, "VeinholeGrowthRate", VeinholeGrowthRate);
		VeinholeShrinkRate = ini.Get_Int(RulesSection_General, "VeinholeShrinkRate", VeinholeShrinkRate);
		VeinDamage = ini.Get_Int(RulesSection_General, "VeinDamage", VeinDamage);
		VeinholeTypeClass = TGet_Class(ini, RulesSection_General, "VeinholeTypeClass", VeinholeTypeClass);  /* out of line 654100 */
		DefaultMirageDisguises = TGet_TypeList<TerrainTypeClass>(ini, RulesSection_General, "DefaultMirageDisguises", DefaultMirageDisguises);
		InfantryBlinkDisguiseTime = ini.Get_Int(RulesSection_General, "InfantryBlinkDisguiseTime", InfantryBlinkDisguiseTime);
		MaximumQueuedObjects = ini.Get_Int(RulesSection_General, "MaximumQueuedObjects", MaximumQueuedObjects);
		MaxWaypointPathLength = ini.Get_Int(RulesSection_General, "MaxWaypointPathLength", MaxWaypointPathLength);
		TreeStrength = ini.Get_Int(RulesSection_General, "TreeStrength", TreeStrength);
		EngineerCaptureLevel = ini.Get_Float(RulesSection_General, "EngineerCaptureLevel", EngineerCaptureLevel);
		EngineerDamage = ini.Get_Float(RulesSection_General, "EngineerCaptureLevel", EngineerDamage); // RA2: TibSun reads the damage under the capture level's key too
		TalkBubbleTime = ini.Get_Float(RulesSection_General, "TalkBubbleTime", TalkBubbleTime * (1.0f / 60)) * 60.0;
		return(true);
	}
	return(false);
}



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
	if (ini.Is_Present(RulesSection_MultiplayerDialogSettings)) {
		MinMoney = ini.Get_Int(RulesSection_MultiplayerDialogSettings, "MinMoney", MinMoney);
		MPMoney = ini.Get_Int(RulesSection_MultiplayerDialogSettings, "Money", MPMoney);
		MPMaxMoney = ini.Get_Int(RulesSection_MultiplayerDialogSettings, "MaxMoney", MPMaxMoney);
		MoneyIncrement = ini.Get_Int(RulesSection_MultiplayerDialogSettings, "MoneyIncrement", MoneyIncrement);
		MinUnitCount = ini.Get_Int(RulesSection_MultiplayerDialogSettings, "MinUnitCount", MinUnitCount);
		MPUnitCount = ini.Get_Int(RulesSection_MultiplayerDialogSettings, "UnitCount", MPUnitCount);
		MaxUnitCount = ini.Get_Int(RulesSection_MultiplayerDialogSettings, "MaxUnitCount", MaxUnitCount);
		MPBuildLevel = ini.Get_Int(RulesSection_MultiplayerDialogSettings, "TechLevel", MPBuildLevel);
		GameSpeed = ini.Get_Int(RulesSection_MultiplayerDialogSettings, "GameSpeed", GameSpeed);
		AIDifficultyStruct = ini.Get_Int(RulesSection_MultiplayerDialogSettings, "AIDifficulty", AIDifficultyStruct);
		AIPlayers = ini.Get_Int(RulesSection_MultiplayerDialogSettings, "AIPlayers", AIPlayers);
		IsMPBridgeDestruction = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "BridgeDestruction", IsMPBridgeDestruction);
		IsMPShadowGrow = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "ShadowGrow", IsMPShadowGrow);
		Shroud = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "Shroud", Shroud);
		IsMPBasesOn = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "Bases", IsMPBasesOn);
		IsMPTiberiumGrow = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "TiberiumGrows", IsMPTiberiumGrow);
		IsMPCrates = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "Crates", IsMPCrates);
		IsMPCaptureTheFlag = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "CaptureTheFlag", IsMPCaptureTheFlag);
		HarvesterTruce = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "HarvesterTruce", HarvesterTruce);
		MultiEngineer = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "MultiEngineer", MultiEngineer);
		AlliesAllowed = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "AlliesAllowed", AlliesAllowed);
		ShortGame = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "ShortGame", ShortGame);
		SuperWeaponsAllowed = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "SuperWeaponsAllowed", SuperWeaponsAllowed);
		BuildOffAlly = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "BuildOffAlly", BuildOffAlly);
		FogOfWar = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "FogOfWar", FogOfWar);
		MCVRedeploys = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "MCVRedeploys", MCVRedeploys);
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
	if (ini.Is_Present(RulesSection_AI)) {
		BuildConst = TGet_TypeList<BuildingTypeClass>(ini, RulesSection_AI, "BuildConst", BuildConst);
		BuildPower = TGet_TypeList<BuildingTypeClass>(ini, RulesSection_AI, "BuildPower", BuildPower);
		BuildRefinery = TGet_TypeList<BuildingTypeClass>(ini, RulesSection_AI, "BuildRefinery", BuildRefinery);
		BuildBarracks = TGet_TypeList<BuildingTypeClass>(ini, RulesSection_AI, "BuildBarracks", BuildBarracks);
		BuildTech = TGet_TypeList<BuildingTypeClass>(ini, RulesSection_AI, "BuildTech", BuildTech);
		BuildWeapons = TGet_TypeList<BuildingTypeClass>(ini, RulesSection_AI, "BuildWeapons", BuildWeapons);
		BuildDefense = TGet_TypeList<BuildingTypeClass>(ini, RulesSection_AI, "BuildDefense", BuildDefense);
		BuildPDefense = TGet_TypeList<BuildingTypeClass>(ini, RulesSection_AI, "BuildPDefense", BuildPDefense);
		BuildAA = TGet_TypeList<BuildingTypeClass>(ini, RulesSection_AI, "BuildAA", BuildAA);
		BuildHelipad = TGet_TypeList<BuildingTypeClass>(ini, RulesSection_AI, "BuildHelipad", BuildHelipad);
		BuildRadar = TGet_TypeList<BuildingTypeClass>(ini, RulesSection_AI, "BuildRadar", BuildRadar);
		ConcreteWalls = TGet_TypeList<BuildingTypeClass>(ini, RulesSection_AI, "ConcreteWalls", ConcreteWalls);
		NSGates = TGet_TypeList<BuildingTypeClass>(ini, RulesSection_AI, "NSGates", NSGates);
		EWGates = TGet_TypeList<BuildingTypeClass>(ini, RulesSection_AI, "EWGates", EWGates);
		BuildNavalYard = TGet_TypeList<BuildingTypeClass>(ini, RulesSection_AI, "BuildNavalYard", BuildNavalYard);
		BuildDummy = TGet_TypeList<BuildingTypeClass>(ini, RulesSection_AI, "BuildDummy", BuildDummy);
		NeutralTechBuildings = TGet_TypeList<BuildingTypeClass>(ini, RulesSection_AI, "NeutralTechBuildings", NeutralTechBuildings);
		AttackInterval = ini.Get_Float(RulesSection_AI, "AttackInterval", AttackInterval);
		AttackDelay = ini.Get_Float(RulesSection_AI, "AttackDelay", AttackDelay);
		PatrolTime = ini.Get_Float(RulesSection_AI, "PatrolScan", PatrolTime);
		int credit_reserve = ini.Get_Int(RulesSection_AI, "CreditReserve", RepairThreshhold);
		RepairThreshhold = credit_reserve;
		PathDelay = ini.Get_Float(RulesSection_AI, "PathDelay", PathDelay);
		int blockage_path_delay = ini.Get_Int(RulesSection_AI, "BlockagePathDelay", BlockagePathDelay);
		BlockagePathDelay = blockage_path_delay;
		int tiberium_near_scan = ini.Get_Lepton(RulesSection_AI, "TiberiumNearScan", TiberiumShortScan);
		TiberiumShortScan = tiberium_near_scan;
		int tiberium_far_scan = ini.Get_Lepton(RulesSection_AI, "TiberiumFarScan", TiberiumLongScan);
		TiberiumLongScan = tiberium_far_scan;
		AutocreateTime = ini.Get_Float(RulesSection_AI, "AutocreateTime", AutocreateTime);
		int infantry_reserve = ini.Get_Int(RulesSection_AI, "InfantryReserve", InfantryReserve);
		InfantryReserve = infantry_reserve;
		int infantry_base_mult = ini.Get_Int(RulesSection_AI, "InfantryBaseMult", InfantryBaseMult);
		InfantryBaseMult = infantry_base_mult;
		int power_surplus = ini.Get_Int(RulesSection_AI, "PowerSurplus", PowerSurplus);
		PowerSurplus = power_surplus;
		int base_size_add = ini.Get_Int(RulesSection_AI, "BaseSizeAdd", BaseSizeAdd);
		BaseSizeAdd = base_size_add;
		RefineryRatio = ini.Get_Float(RulesSection_AI, "RefineryRatio", RefineryRatio);
		int refinery_limit = ini.Get_Int(RulesSection_AI, "RefineryLimit", RefineryLimit);
		RefineryLimit = refinery_limit;
		BarracksRatio = ini.Get_Float(RulesSection_AI, "BarracksRatio", BarracksRatio);
		int barracks_limit = ini.Get_Int(RulesSection_AI, "BarracksLimit", BarracksLimit);
		BarracksLimit = barracks_limit;
		WarRatio = ini.Get_Float(RulesSection_AI, "WarRatio", WarRatio);
		int war_limit = ini.Get_Int(RulesSection_AI, "WarLimit", WarLimit);
		WarLimit = war_limit;
		DefenseRatio = ini.Get_Float(RulesSection_AI, "DefenseRatio", DefenseRatio);
		int defense_limit = ini.Get_Int(RulesSection_AI, "DefenseLimit", DefenseLimit);
		DefenseLimit = defense_limit;
		AARatio = ini.Get_Float(RulesSection_AI, "AARatio", AARatio);
		int aa_limit = ini.Get_Int(RulesSection_AI, "AALimit", AALimit);
		AALimit = aa_limit;
		TeslaRatio = ini.Get_Float(RulesSection_AI, "TeslaRatio", TeslaRatio);
		int tesla_limit = ini.Get_Int(RulesSection_AI, "TeslaLimit", TeslaLimit);
		TeslaLimit = tesla_limit;
		HelipadRatio = ini.Get_Float(RulesSection_AI, "HelipadRatio", HelipadRatio);
		int helipad_limit = ini.Get_Int(RulesSection_AI, "HelipadLimit", HelipadLimit);
		HelipadLimit = helipad_limit;
		AirstripRatio = ini.Get_Float(RulesSection_AI, "AirstripRatio", AirstripRatio);
		int airstrip_limit = ini.Get_Int(RulesSection_AI, "AirstripLimit", AirstripLimit);
		AirstripLimit = airstrip_limit;
		IsCompEasyBonus = ini.Get_Bool(RulesSection_AI, "CompEasyBonus", IsCompEasyBonus);
		IsComputerParanoid = ini.Get_Bool(RulesSection_AI, "Paranoid", IsComputerParanoid);
		PowerEmergencyFraction = ini.Get_Float(RulesSection_AI, "PowerEmergency", PowerEmergencyFraction);
		int ai_base_spacing = ini.Get_Int(RulesSection_AI, "AIBaseSpacing", AIBaseSpacing);
		AIBaseSpacing = ai_base_spacing;
		GDIWallDefense = ini.Get_Float(RulesSection_AI, "GDIWallDefense", GDIWallDefense);
		GDIWallDefenseCoefficient = ini.Get_Float(RulesSection_AI, "GDIWallDefenseCoefficient", GDIWallDefenseCoefficient);
		NodBaseDefenseCoefficient = ini.Get_Float(RulesSection_AI, "NodBaseDefenseCoefficient", NodBaseDefenseCoefficient);
		GDIBaseDefenseCoefficient = ini.Get_Float(RulesSection_AI, "GDIBaseDefenseCoefficient", GDIBaseDefenseCoefficient);
		int maximum_base_defense_value = ini.Get_Int(RulesSection_AI, "MaximumBaseDefenseValue", MaximumBaseDefenseValue);
		MaximumBaseDefenseValue = maximum_base_defense_value;
		int computer_base_defense_response = ini.Get_Int(RulesSection_AI, "ComputerBaseDefenseResponse", ComputerBaseDefenseResponse);
		ComputerBaseDefenseResponse = computer_base_defense_response;
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
				**	Can it appear on water?
				*/
				token = strtok(NULL, ",");
				if (token != NULL) {
					strtrim(token);
					if (stricmp(token, "yes") == 0) {
						CrateWater[crate] = true;
					} else if (stricmp(token, "no") == 0) {
						CrateWater[crate] = false;
					}
				}

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
		"Clear", "Road", "Water", "Rock", "Wall", "Tiberium",
		"Beach", "Rough", "Ice", "Railroad", "Tunnel", "Weeds",
	};
	for (int land = LAND_FIRST; land < LAND_COUNT; land++) {
		GroundType * gptr = &Ground[land]; // RA2: Land_Types at 0x64D610 has no static char const * _lands[LAND_COUNT] =
		if (ini.Is_Present(_lands[land])) {
			gptr->Cost[SPEED_HOVER] = MIN(ini.Get_Float(_lands[land], "Hover", 1.0), 1.0);
			gptr->Cost[SPEED_FOOT] = MIN(ini.Get_Float(_lands[land], "Foot", 1.0), 1.0);
			gptr->Cost[SPEED_TRACK] = MIN(ini.Get_Float(_lands[land], "Track", 1.0), 1.0);
			gptr->Cost[SPEED_WHEEL] = MIN(ini.Get_Float(_lands[land], "Wheel", 1.0), 1.0);
			gptr->Cost[SPEED_WINGED] = 1.0f;
			gptr->Cost[SPEED_FLOAT] = MIN(ini.Get_Float(_lands[land], "Float", 1.0), 1.0);
			gptr->Cost[SPEED_AMPHIBIOUS] = MIN(ini.Get_Float(_lands[land], "Amphibious", 1.0), 1.0);
			gptr->Cost[SPEED_CREEP] = MIN(ini.Get_Float(_lands[land], "FloatBeach", 1.0), 1.0);
			gptr->Build = ini.Get_Bool(_lands[land], "Buildable", false);
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
	stream->Write(this, sizeof(*this), 0);
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
	AlliedBaseDefenseCounts.Save(stream);
	SovietBaseDefenseCounts.Save(stream);
	DisabledDisguiseDetectionPercent.Save(stream);
	AIAutoDeployFrameDelay.Save(stream);
	AIPickWallDefensePercent.Save(stream);
	MinimumAIDefensiveTeams.Save(stream);
	MaximumAIDefensiveTeams.Save(stream);
	MultiplayerAICreditMultipliers.Save(stream);
	AIVirtualPurifiers.Save(stream);
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
	PrerequisiteProc.Save(stream);
	CreditTicks.Save(stream);
	LightningSounds.Save(stream);
	Scorches.Save(stream);
	Scorches1.Save(stream);
	Scorches2.Save(stream);
	Scorches3.Save(stream);
	Scorches4.Save(stream);
	Shipyard.Save(stream);
	BuildConst.Save(stream);
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
	BuildNavalYard.Save(stream);
	BuildDummy.Save(stream);
	NeutralTechBuildings.Save(stream);
	PadAircraft.Save(stream);
	OnFire.Save(stream);
	TreeFire.Save(stream);
	SplashList.Save(stream);
	BaseUnit.Save(stream);
	DefaultMirageDisguises.Save(stream);
	HarvesterUnit.Save(stream);
	DeadBodies.Save(stream);
	DropPod.Save(stream);
	MetallicDebris.Save(stream);
	BridgeExplosions.Save(stream);
	IceCrackSounds.Save(stream);
	HSBuilding.Save(stream);
	AmerParaDropInf.Save(stream);
	AmerParaDropNum.Save(stream);
	AllyParaDropInf.Save(stream);
	AllyParaDropNum.Save(stream);
	SovParaDropInf.Save(stream);
	SovParaDropNum.Save(stream);
	DamageFireTypes.Save(stream);
	WeatherConClouds.Save(stream);
	WeatherConBolts.Save(stream);
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
	SovietBaseDefenseCounts.Clear();
	DisabledDisguiseDetectionPercent.Clear();
	AIAutoDeployFrameDelay.Clear();
	AIPickWallDefensePercent.Clear();
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
	BuildDummy.Clear();
	NeutralTechBuildings.Clear();
	PadAircraft.Clear();
	OnFire.Clear();
	TreeFire.Clear();
	SplashList.Clear();
	BaseUnit.Clear(); // RA2: the executable's Load: RA2's members and order
	DefaultMirageDisguises.Clear();
	HarvesterUnit.Clear();
	IceCrackSounds.Clear();
	HSBuilding.Clear();
	AmerParaDropInf.Clear(); // RA2: the executable's Load: RA2's members and order
	AmerParaDropNum.Clear();
	AllyParaDropInf.Clear();
	AllyParaDropNum.Clear();
	SovParaDropInf.Clear();
	SovParaDropNum.Clear();
	stream->Read(this, sizeof(*this), 0);
	new (this) RulesClass(NoInitClass());
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
	SovietBaseDefenseCounts.Load(stream);
	DisabledDisguiseDetectionPercent.Load(stream);
	AIAutoDeployFrameDelay.Load(stream);
	AIPickWallDefensePercent.Load(stream);
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
	BuildDummy.Load(stream);
	NeutralTechBuildings.Load(stream);
	PadAircraft.Load(stream);
	OnFire.Load(stream);
	TreeFire.Load(stream);
	SplashList.Load(stream);
	BaseUnit.Load(stream); // RA2: the executable's Load: RA2's members and order
	DefaultMirageDisguises.Load(stream);
	HarvesterUnit.Load(stream);
	DeadBodies.Load(stream);
	DropPod.Load(stream);
	MetallicDebris.Load(stream);
	BridgeExplosions.Load(stream);
	IceCrackSounds.Load(stream);
	HSBuilding.Load(stream);
	AmerParaDropInf.Load(stream); // RA2: the executable's Load: RA2's members and order
	AmerParaDropNum.Load(stream);
	AllyParaDropInf.Load(stream);
	AllyParaDropNum.Load(stream);
	SovParaDropInf.Load(stream);
	SovParaDropNum.Load(stream);
	DamageFireTypes.Load(stream);
	WeatherConClouds.Load(stream);
	WeatherConBolts.Load(stream);
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
	Swizzle_Pointer(&DMislWarhead);
	Swizzle_Pointer(&V3EliteWarhead);
	Swizzle_Pointer(&DMislEliteWarhead);
	Swizzle_Pointer(&V3Rocket.Type);
	Swizzle_Pointer(&DMisl.Type);
	Swizzle_Pointer(&PrismType);
	Swizzle_Pointer(&IvanWarhead);
	Swizzle_Pointer(&DeathWeapon);
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
	Swizzle_Pointer(&SovietDisguise);
	Swizzle_Pointer(&Technician);
	Swizzle_Pointer(&Engineer);
	Swizzle_Pointer(&Pilot);
	Swizzle_Pointer(&AlliedCrew); // RA2: the executable's Load: RA2's members and order
	Swizzle_Pointer(&SovietCrew);
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
	Swizzle_Pointer(&ChronoBlast);
	Swizzle_Pointer(&ChronoBlastDest);
	Swizzle_Pointer(&ChronoPlacement);
	Swizzle_Pointer(&IonBeam);
	Swizzle_Pointer(&WarpIn); // RA2: the executable's Load: RA2's members and order
	Swizzle_Pointer(&WarpOut);
	Swizzle_Pointer(&WarpAway);
	Swizzle_Pointer(&ChronoSparkle1);
	Swizzle_Pointer(&IronCurtainInvokeAnim);
	Swizzle_Pointer(&WeaponNullifyAnim);
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
	Swizzle_Pointer(&InfantryHeadPop);
	Swizzle_Pointer(&InfantryNuked);
	Swizzle_Pointer(&Behind);
	Swizzle_Pointer(&VeinAttack);
	Swizzle_Pointer(&BarrelExplode);
	Swizzle_Pointer(&BarrelParticle);
	Swizzle_Pointer(&DropPodPuff);
	Swizzle_Pointer(&LightningWarhead); // RA2: the executable's Load: RA2's members and order
	Swizzle_Pointer(&RadSiteWarhead);
	Swizzle_Pointer(&WoodCrateImg);
	Swizzle_Pointer(&CrateImg);
	Swizzle_Pointer(&WaterCrateImg); // RA2: the executable's Load: RA2's members and order
	BOMBCURS_SHP = (ShapeSet const *)MixFileClass::Retrieve("BOMBCURS.SHP");
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
		Swizzle_Pointer(&BaseUnit[index]);
	}
	for (index = 0; index < DefaultMirageDisguises.Count(); index++) { // RA2: the executable's Load: RA2's members and order
		Swizzle_Pointer(&DefaultMirageDisguises[index]);
	}
	for (index = 0; index < HarvesterUnit.Count(); index++) {
		Swizzle_Pointer(&HarvesterUnit[index]);
	}
	for (index = 0; index < Shipyard.Count(); index++) { // RA2: the executable's Load: RA2's members and order
		Swizzle_Pointer(&Shipyard[index]);
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
		Swizzle_Pointer(&BuildNavalYard[index]);
	}
	for (index = 0; index < BuildDummy.Count(); index++) { // RA2: the executable's Load: RA2's members and order
		Swizzle_Pointer(&BuildDummy[index]);
	}
	for (index = 0; index < NeutralTechBuildings.Count(); index++) { // RA2: the executable's Load: RA2's members and order
		Swizzle_Pointer(&NeutralTechBuildings[index]);
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
	for (index = 0; index < count; index++) {
		Swizzle_Pointer(&WeatherConClouds[index]);
	}
	count = WeatherConBolts.Count(); // RA2: the executable's Load: RA2's members and order
	for (index = 0; index < count; index++) {
		Swizzle_Pointer(&WeatherConBolts[index]);
	}
	for (index = 0; index < AmerParaDropInf.Count(); index++) { // RA2: the executable's Load: RA2's members and order
		Swizzle_Pointer(&AmerParaDropInf[index]);
	}
	for (index = 0; index < AllyParaDropInf.Count(); index++) { // RA2: the executable's Load: RA2's members and order
		Swizzle_Pointer(&AllyParaDropInf[index]);
	}
	for (index = 0; index < SovParaDropInf.Count(); index++) { // RA2: the executable's Load: RA2's members and order
		Swizzle_Pointer(&SovParaDropInf[index]);
	}
	count = DamageFireTypes.Count(); // RA2: the executable's Load: RA2's members and order
	for (index = 0; index < count; index++) {
		Swizzle_Pointer(&DamageFireTypes[index]);
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
		LargeVisceroid = 0;
	}
	if (target == SmallVisceroid) {
		SmallVisceroid = 0;
	}
	if (target == UnloadingHarvester) {
		UnloadingHarvester = 0;
	}
	if (target == VeinAttack) {
		VeinAttack = 0;
	}
	if (target == Wake) {
		Wake = 0;
	}
	if (target == OreTwinkle) {
		OreTwinkle = 0;
	}
	if (target == FlamingInfantry) {
		FlamingInfantry = 0;
	}
	if (target == NukeTakeOff) {
		NukeTakeOff = 0;
	}
	if (target == InfantryHeadPop) {
		InfantryHeadPop = 0;
	}
	if (target == InfantryNuked) {
		InfantryNuked = 0;
	}
	if (target == Behind) {
		Behind = 0;
	}
	if (target == DropPodWeapon) {
		DropPodWeapon = 0;
	}
	if (target == FlameDamage) {
		FlameDamage = 0;
	}
	if (target == FlameDamage2) {
		FlameDamage2 = 0;
	}
	if (target == NukeWarhead) {
		NukeWarhead = 0;
	}
	if (target == EMPulseWarhead) {
		EMPulseWarhead = 0;
	}
	if (target == C4Warhead) {
		C4Warhead = 0;
	}
	if (target == V3Warhead) {
		V3Warhead = 0;
	}
	if (target == DMislWarhead) {
		DMislWarhead = 0;
	}
	if (target == V3EliteWarhead) {
		V3EliteWarhead = 0;
	}
	if (target == DMislEliteWarhead) {
		DMislEliteWarhead = 0;
	}
	if (target == V3Rocket.Type) {
		V3Rocket.Type = 0;
	}
	if (target == DMisl.Type) {
		DMisl.Type = 0;
	}
	if (target == IvanWarhead) {
		IvanWarhead = 0;
	}
	if (target == DeathWeapon) {
		DeathWeapon = 0;
	}
	if (target == IonCannonWarhead) {
		IonCannonWarhead = 0;
	}
	if (target == FirestormWarhead) {
		FirestormWarhead = 0;
	}
	if (target == EMPulseSparkles) {
		EMPulseSparkles = 0;
	}
	if (target == VeinholeWarhead) {
		VeinholeWarhead = 0;
	}
	if (target == DefaultFirestormExplosionSystem) {
		DefaultFirestormExplosionSystem = 0;
	}
	if (target == DefaultLargeGreySmokeSystem) {
		DefaultLargeGreySmokeSystem = 0;
	}
	if (target == DefaultSmallGreySmokeSystem) {
		DefaultSmallGreySmokeSystem = 0;
	}
	if (target == DefaultSparkSystem) {
		DefaultSparkSystem = 0;
	}
	if (target == DefaultLargeRedSmokeSystem) {
		DefaultLargeRedSmokeSystem = 0;
	}
	if (target == DefaultSmallRedSmokeSystem) {
		DefaultSmallRedSmokeSystem = 0;
	}
	if (target == DefaultDebrisSmokeSystem) {
		DefaultDebrisSmokeSystem = 0;
	}
	if (target == DefaultFireStreamSystem) {
		DefaultFireStreamSystem = 0;
	}
	if (target == DefaultTestParticleSystem) {
		DefaultTestParticleSystem = 0;
	}
	if (target == DefaultRepairParticleSystem) {
		DefaultRepairParticleSystem = 0;
	}
	if (target == VeinholeTypeClass) {
		VeinholeTypeClass = 0;
	}
	if (target == WeatherConBoltExplosion) {
		WeatherConBoltExplosion = 0;
	}
	if (target == Smoke1) {
		Smoke1 = 0;
	}
	if (target == Smoke2) {
		Smoke2 = 0;
	}
	if (target == MoveFlash) {
		MoveFlash = 0;
	}
	if (target == BombParachute) {
		BombParachute = 0;
	}
	if (target == Parachute) {
		Parachute = 0;
	}
	if (target == SmallFire) {
		SmallFire = 0;
	}
	if (target == LargeFire) {
		LargeFire = 0;
	}
	if (target == FlareAnim) {
		FlareAnim = 0;
	}
	if (target == UnitCrateType) {
		UnitCrateType = 0;
	}
	if (target == Paratrooper) {
		Paratrooper = 0;
	}
	if (target == AlliedDisguise) {
		AlliedDisguise = 0;
	}
	if (target == SovietDisguise) {
		SovietDisguise = 0;
	}
	if (target == Technician) {
		Technician = 0;
	}
	if (target == Engineer) {
		Engineer = 0;
	}
	if (target == Pilot) {
		Pilot = 0;
	}
	if (target == AlliedCrew) {
		AlliedCrew = 0;
	}
	if (target == SovietCrew) {
		SovietCrew = 0;
	}
	if (target == RepairBay) {
		RepairBay = 0;
	}
	if (target == GDIGateOne) {
		GDIGateOne = 0;
	}
	if (target == GDIGateTwo) {
		GDIGateTwo = 0;
	}
	if (target == NodGateOne) {
		NodGateOne = 0;
	}
	if (target == NodGateTwo) {
		NodGateTwo = 0;
	}
	if (target == WallTower) {
		WallTower = 0;
	}
	if (target == GDIPowerPlant) {
		GDIPowerPlant = 0;
	}
	if (target == GDIPowerTurbine) {
		GDIPowerTurbine = 0;
	}
	if (target == NodRegularPower) {
		NodRegularPower = 0;
	}
	if (target == NodAdvancedPower) {
		NodAdvancedPower = 0;
	}
	if (target == GDIFirestormGenerator) {
		GDIFirestormGenerator = 0;
	}
	if (target == GDIHunterSeeker) {
		GDIHunterSeeker = 0;
	}
	if (target == NodHunterSeeker) {
		NodHunterSeeker = 0;
	}
	if (target == NukeProjectile) {
		NukeProjectile = 0;
	}
	if (target == NukeDown) {
		NukeDown = 0;
	}
	if (target == EMPulseProjectile) {
		EMPulseProjectile = 0;
	}
	if (target == TireVoxelDebris) {
		TireVoxelDebris = 0;
	}
	if (target == ScrapVoxelDebris) {
		ScrapVoxelDebris = 0;
	}
	if (target == AtmosphereEntry) {
		AtmosphereEntry = 0;
	}
	if (target == InfantryExplode) {
		InfantryExplode = 0;
	}
	if (target == IonBlast) {
		IonBlast = 0;
	}
	if (target == IonBeam) {
		IonBeam = 0;
	}
	if (target == ChronoBlast) {
		ChronoBlast = 0;
	}
	if (target == ChronoBlastDest) {
		ChronoBlastDest = 0;
	}
	if (target == ChronoPlacement) {
		ChronoPlacement = 0;
	}
	if (target == ChronoBeam) {
		ChronoBeam = 0;
	}
	if (target == WarpIn) {
		WarpIn = 0;
	}
	if (target == WarpOut) {
		WarpOut = 0;
	}
	if (target == WarpAway) {
		WarpAway = 0;
	}
	if (target == ChronoSparkle1) {
		ChronoSparkle1 = 0;
	}
	if (target == IronCurtainInvokeAnim) {
		IronCurtainInvokeAnim = 0;
	}
	if (target == WeaponNullifyAnim) {
		WeaponNullifyAnim = 0;
	}
	if (target == Dig) {
		Dig = 0;
	}
	if (target == FirestormIdleAnim) {
		FirestormIdleAnim = 0;
	}
	if (target == FirestormAirAnim) {
		FirestormAirAnim = 0;
	}
	if (target == FirestormActiveAnim) {
		FirestormActiveAnim = 0;
	}
	if (target == FirestormGroundAnim) {
		FirestormGroundAnim = 0;
	}
	if (target == BarrelExplode) {
		BarrelExplode = 0;
	}
	if (target == BarrelParticle) {
		BarrelParticle = 0;
	}
	if (target == DropPodPuff) {
		DropPodPuff = 0;
	}
	if (target == LightningWarhead) {
		LightningWarhead = 0;
	}
	if (target == RadSiteWarhead) {
		RadSiteWarhead = 0;
	}
	BarrelDebris.Delete((VoxelAnimTypeClass *)target);
	OnFire.Delete((AnimTypeClass *)target);
	TreeFire.Delete((AnimTypeClass *)target);
	SplashList.Delete((AnimTypeClass *)target);
	Scorches.Delete((SmudgeTypeClass *)target);
	Scorches1.Delete((SmudgeTypeClass *)target);
	Scorches2.Delete((SmudgeTypeClass *)target);
	Scorches3.Delete((SmudgeTypeClass *)target);
	Scorches4.Delete((SmudgeTypeClass *)target);
	BaseUnit.Delete((UnitTypeClass *)target);
	DefaultMirageDisguises.Delete((TerrainTypeClass *)target);
	HarvesterUnit.Delete((UnitTypeClass *)target);
	Shipyard.Delete((BuildingTypeClass *)target);
	BuildConst.Delete((BuildingTypeClass *)target);
	BuildPower.Delete((BuildingTypeClass *)target);
	BuildRefinery.Delete((BuildingTypeClass *)target);
	BuildBarracks.Delete((BuildingTypeClass *)target);
	BuildTech.Delete((BuildingTypeClass *)target);
	BuildWeapons.Delete((BuildingTypeClass *)target);
	BuildDefense.Delete((BuildingTypeClass *)target);
	BuildPDefense.Delete((BuildingTypeClass *)target);
	BuildAA.Delete((BuildingTypeClass *)target);
	BuildHelipad.Delete((BuildingTypeClass *)target);
	BuildRadar.Delete((BuildingTypeClass *)target);
	ConcreteWalls.Delete((BuildingTypeClass *)target);
	NSGates.Delete((BuildingTypeClass *)target);
	EWGates.Delete((BuildingTypeClass *)target);
	BuildNavalYard.Delete((BuildingTypeClass *)target);
	BuildDummy.Delete((BuildingTypeClass *)target);
	NeutralTechBuildings.Delete((BuildingTypeClass *)target);
	PadAircraft.Delete((AircraftTypeClass *)target);
	DeadBodies.Delete((AnimTypeClass *)target);
	DropPod.Delete((AnimTypeClass *)target);
	MetallicDebris.Delete((AnimTypeClass *)target);
	BridgeExplosions.Delete((AnimTypeClass *)target);
	HSBuilding.Delete((BuildingTypeClass *)target);
	WeatherConClouds.Delete((AnimTypeClass *)target);
	WeatherConBolts.Delete((AnimTypeClass *)target);
	AmerParaDropInf.Delete((InfantryTypeClass *)target);
	AllyParaDropInf.Delete((InfantryTypeClass *)target);
	SovParaDropInf.Delete((InfantryTypeClass *)target);
	DamageFireTypes.Delete((AnimTypeClass *)target);
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
		HouseTypes[index]->Read_INI(ini);
	}
	for (index = 0; index < SuperWeaponTypes.Count(); index++) { // RA2: the executable's Objects (no stricmp on the infantry name)
		SuperWeaponTypes[index]->Read_INI(ini);
	}
	for (index = 0; index < AnimTypes.Count(); index++) { // RA2: the executable's Objects (no stricmp on the infantry name)
		AnimTypes[index]->Read_INI(ArtINI);
	}
	for (index = 0; index < BuildingTypes.Count(); index++) { // RA2: the executable's Objects (no stricmp on the infantry name)
		BuildingTypes[index]->Read_INI(ini);
	}
	for (index = 0; index < AircraftTypes.Count(); index++) { // RA2: the executable's Objects (no stricmp on the infantry name)
		AircraftTypes[index]->Read_INI(ini);
	}
	for (index = 0; index < UnitTypes.Count(); index++) { // RA2: the executable's Objects (no stricmp on the infantry name)
		UnitTypes[index]->Read_INI(ini);
	}
	for (index = 0; index < InfantryTypes.Count(); index++) { // RA2: the executable's Objects (no stricmp on the infantry name)
		InfantryTypes[index]->Read_INI(ini);
	}
	for (index = 0; index < Weapons.Count(); index++) { // RA2: the executable's Objects (no stricmp on the infantry name)
		Weapons[index]->Read_INI(ini);
	}
	for (index = 0; index < BulletTypes.Count(); index++) { // RA2: the executable's Objects (no stricmp on the infantry name)
		BulletTypes[index]->Read_INI(ini);
	}
	for (index = 0; index < Warheads.Count(); index++) { // RA2: the executable's Objects (no stricmp on the infantry name)
		Warheads[index]->Read_INI(ini);
	}
	for (index = 0; index < Weapons.Count(); index++) { // RA2: the executable's Objects (no stricmp on the infantry name)
		Weapons[index]->Init_Max_Speed();
	}
	for (index = 0; index < BuildingTypes.Count(); index++) { // RA2: the executable's Objects (no stricmp on the infantry name)
		BuildingTypes[index]->Calculate_Base_Defense_Values();
	}
	for (index = 0; index < TerrainTypes.Count(); index++) { // RA2: the executable's Objects (no stricmp on the infantry name)
		TerrainTypes[index]->Read_INI(ini);
	}
	for (index = 0; index < SmudgeTypes.Count(); index++) { // RA2: the executable's Objects (no stricmp on the infantry name)
		SmudgeTypes[index]->Read_INI(ini);
	}
	for (index = 0; index < OverlayTypes.Count(); index++) { // RA2: the executable's Objects (no stricmp on the infantry name)
		OverlayTypes[index]->Read_INI(ini);
	}
	for (index = 0; index < ParticleTypes.Count(); index++) { // RA2: the executable's Objects (no stricmp on the infantry name)
		ParticleTypes[index]->Read_INI(ini);
	}
	for (index = 0; index < ParticleSystemTypes.Count(); index++) { // RA2: the executable's Objects (no stricmp on the infantry name)
		ParticleSystemTypes[index]->Read_INI(ini);
	}
	for (index = 0; index < VoxelAnimTypes.Count(); index++) { // RA2: the executable's Objects (no stricmp on the infantry name)
		VoxelAnimTypes[index]->Read_INI(ini);
	}
	for (int mission = MISSION_FIRST; mission < MISSION_COUNT; mission++) {
		MissionControl[mission].Mission = (MissionType)mission; // RA2: the executable's Objects (no stricmp on the infantry name)
		MissionControl[mission].Read_INI(ini);
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
	int count = MultiplayerGameMode::GetModeCount();
	for (int index = 1; index <= count; index++) {
		MultiplayerGameMode * session = MultiplayerGameMode::Get(index);
		if (session != 0) {
			id += session->GetINI()->Get_Unique_ID();
		}
	}
	return id + Coop_Camp_Unique_ID();
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

