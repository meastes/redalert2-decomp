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

#include "always.h"
#include "geometry.h" // RA2: the header statics, in the order of the unit's initializer table
#include "rect.h"

#include "techtype.h"

#include "_map.h"
#include "_mixfile.h"
#include "_rules.h"
#include "airctype.h" // RA2: TGet_Class of Spawns in Read_INI
#include "animtype.h"
#include "building.h"
#include "builtype.h"
#include "bullet.h"
#include "bullettype.h"
#include "cell.h"
#include "combat.h"
#include "conquer.h"
#include "debug.h"
#include "palette.h"
#include "findmake.h"
#include "globals.h"
#include "ilocos.h"
#include "infatype.h"
#include "mixfile.h"
#include "psystype.h"
#include "rules.h"
#include "session.h"
#include "swizzle.h"
#include "tracker.h"
#include "tactical.h"
#include "_tactica.h"
#include "unittype.h"
#include "vanimtype.h"
#include "weapon.h"

#include "pip.hh"
#include "voc.hh"

#include <new.h>







//**********************************************************************************************
// MODULE SEPARATION -- TechnoTypeClass member functions follow.
//**********************************************************************************************


/***********************************************************************************************
 * TechnoTypeClass::TechnoTypeClass -- Constructor for techno type objects.                    *
 *                                                                                             *
 *    This is the normal constructor for techno type objects. It is called in the process of   *
 *    constructing all the object type (constant) data for the various techno type objects.    *
 *                                                                                             *
 * INPUT:   see below...                                                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/19/1995 JLB : Created.                                                                 *
 *   05/11/1996 JLB : Moderated risk calc so range doesn't dominate.                           *
 *=============================================================================================*/
// RA2: the constructor of the types read from the rules, meastes' body at 0x6D9B40 (every member in its initialization list order)
#ifdef RA2_NON_MATCHING // not exact: 27 bytes differ (the original calls the vector constructors kept at 0x654960 and 0x454AD0)
TechnoTypeClass::TechnoTypeClass(char const * ininame, SpeedType speed) : // RA2: as above
	BASECLASS(ininame), // RA2: as above
	WalkRate(1), // RA2: as above
	IdleRate(0), // RA2: as above
	SpecialThreatValue(0.0), // RA2: as above
	MyEffectivenessCoefficient(0.0), // RA2: as above
	TargetEffectivenessCoefficient(0.0), // RA2: as above
	TargetSpecialThreatCoefficient(0.0), // RA2: as above
	TargetStrengthCoefficient(0.0), // RA2: as above
	TargetDistanceCoefficient(0.0), // RA2: as above
	ThreatAvoidanceCoefficient(0.0), // RA2: as above
	SlowdownDistance(500), // RA2: as above
	DeaccelerationFactor(0.002), // RA2: as above
	AccelerationFactor(0.03), // RA2: as above
	CloakingSpeed(7), // RA2: as above
	Locomotor(CLSID_TeleportLocomotion), // RA2: as above
	VoxelCenterY(0.0), // RA2: as above
	VoxelCenterX(0.0), // RA2: as above
	Weight(1.0), // RA2: as above
	PhysicalSize(2.0), // RA2: as above
	Size(1.0), // RA2: as above
	SizeLimit(0.0), // RA2: as above
	HoverAttack(false), // RA2: as above
	VHPScan(0), // RA2: as above
	InitialMission(MISSION_HUNT), // RA2: as above
	RollAngle(0.5235987755982988), // RA2: as above
	PitchSpeed(0.25), // RA2: as above
	PitchAngle(0.3490658503988659), // RA2: as above
	BuildLimit(0x7FFFFFFF), // RA2: as above
	Category(CATEGORY_NONE), // RA2: as above
	Unused2(0), // RA2: as above
	DeployTime(0.0), // RA2: as above
	FireAngle(8), // RA2: as above
	PipScale(PIPSCALE_NONE), // RA2: as above
	PipWrap(0), // RA2: as above
	DeploysInto(0), // RA2: as above
	UndeploysInto(0), // RA2: as above
	AuxSound1(VOC_NONE), // RA2: as above
	AuxSound2(VOC_NONE), // RA2: as above
	CreateSound(-1), // RA2: as above
	DamageSound(-1), // RA2: as above
	ImpactWaterSound(-1), // RA2: as above
	ImpactLandSound(-1), // RA2: as above
	CrashingSound(-1), // RA2: as above
	SinkingSound(-1), // RA2: as above
	VoiceFalling(-1), // RA2: as above
	VoiceCrashing(-1), // RA2: as above
	VoiceSinking(-1), // RA2: as above
	VoiceEnter(-1), // RA2: as above
	VoiceCapture(-1), // RA2: as above
	TurretRotateSound(-1), // RA2: as above
	EnterTransportSound(-1), // RA2: as above
	LeaveTransportSound(-1), // RA2: as above
	DeploySound(-1), // RA2: as above
	UndeploySound(-1), // RA2: as above
	ChronoInSound(-1), // RA2: as above
	ChronoOutSound(-1), // RA2: as above
	MZone(MZONE_NORMAL), // RA2: as above
	ThreatRange(0), // RA2: as above
	MaxDebris(0), // RA2: as above
	MinDebris(0), // RA2: as above
	MaxPassengers(0), // RA2: as above
	SightRange(0), // RA2: as above
	RevealToAll(false), // RA2: as above
	SensorsSight(0), // RA2: as above
	DetectDisguiseRange(0), // RA2: as above
	BombSight(0), // RA2: as above
	LeadershipRating(5), // RA2: as above
	NavalTargeting(0), // RA2: as above
	LandTargeting(0), // RA2: as above
	Cost(0), // RA2: as above
	Soylent(0), // RA2: as above
	FlightLevel(-1), // RA2: as above
	Level(255), // RA2: as above
	Risk(0), // RA2: as above
	Reward(0), // RA2: as above
	MaxSpeed(MPH_IMMOBILE), // RA2: as above
	Speed(speed), // RA2: as above
	InitialAmmo(-1), // RA2: as above
	MaxAmmo(-1), // RA2: as above
	IFVMode(0), // RA2: as above
	AirRangeBonus(0), // RA2: as above
	Reload(0), // RA2: as above
	EmptyReload(-1), // RA2: as above
	ReloadIncrement(0), // RA2: as above
	RadialFireSegments(0), // RA2: as above
	DeployFireWeapon(1), // RA2: as above
	DeployFire(false), // RA2: as above
	MobileFire(true), // RA2: as above
	OpportunityFire(false), // RA2: as above
	DistributedFire(false), // RA2: as above
	DamageReducesReadiness(false), // RA2: as above
	ReadinessReductionMultiplier(0.0f), // RA2: as above
	UnloadingClass(0), // RA2: as above
	AttackFriendlies(false), // RA2: as above
	AttackCursorOnFriendlies(false), // RA2: as above
	UndeployDelay(-1), // RA2: as above
	PreventAttackMove(false), // RA2: as above
	Ownable(0), // RA2: as above
	AIBasePlanningSide(-1), // RA2: as above
	StupidHunt(false), // RA2: as above
	IsAllowedToStartInMultiplayer(true), // RA2: as above
	CameoFilename(""), // RA2: as above
	CameoData(0), // RA2: as above
	CameoAllocated(false), // RA2: as above
	AltCameoFilename(""), // RA2: as above
	AltCameoData(0), // RA2: as above
	AltCameoAllocated(false), // RA2: as above
	Rotation(0), // RA2: as above
	ROT(0), // RA2: as above
	TurretOffset(0), // RA2: as above
	Points(0), // RA2: as above
	NaturalParticleSystem(0), // RA2: as above
	NaturalParticleLocation(0, 0, 0), // RA2: as above
	DamageSmokeOffset(0, 0, 0), // RA2: as above
	DamSmkOffScrnRel(false), // RA2: as above
	DestroySmokeOffset(0, 0, 0), // RA2: as above
	RefinerySmokeOffsetOne(0, 0, 0), // RA2: as above
	RefinerySmokeOffsetTwo(0, 0, 0), // RA2: as above
	ShadowIndex(0), // RA2: as above
	Capacity(0), // RA2: as above
	TurretNotExportedOnGround(false), // RA2: as above
	Gunner(false), // RA2: as above
	HasTurretTooltips(false), // RA2: as above
	TurretCount(0), // RA2: as above
	IsChargeTurret(false), // RA2: as above
	ClearAllWeapons(false), // RA2: as above
	IsTypeImmune(false), // RA2: as above
	IsMoveToShroud(true), // RA2: as above
	IsTrainable(true), // RA2: as above
	IsDamageSparks(false), // RA2: as above
	IsTargetLaser(false), // RA2: as above
	IsImmuneToVeins(false), // RA2: as above
	IsTiberiumHeal(false), // RA2: as above
	IsCloakStop(false), // RA2: as above
	IsTrain(false), // RA2: as above
	IsDropship(false), // RA2: as above
	IsToProtect(false), // RA2: as above
	IsDisableable(true), // RA2: as above
	IsUnbuildable(false), // RA2: as above
	IsDoubleOwned(false), // RA2: as above
	IsInvisible(false), // RA2: as above
	IsRadarVisible(false), // RA2: as above
	IsLeader(false), // RA2: as above
	IsScanner(false), // RA2: as above
	IsNominal(false), // RA2: as above
	IsTurretEquipped(false), // RA2: as above
	TurretRecoil(true), // RA2: as above
	TurretAnimData(2, 1, 1, 1), // RA2: +0xAB0 is built with travel 2, compress 1, recover 1, hold 1 (the same values as the NoInit constructor)
	BarrelAnimData(2, 1, 1, 1), // RA2: +0xAC4 as above
	IsRepairable(true), // RA2: as above
	IsCrew(false), // RA2: as above
	Naval(false), // RA2: as above
	IsRemappable(false), // RA2: as above
	IsCloakable(false), // RA2: as above
	GapGenerator(false), // RA2: as above
	GapRadiusInCells(0), // RA2: as above
	SuperGapRadiusInCells(0), // RA2: as above
	Teleporter(false), // RA2: as above
	IsSelfHealing(false), // RA2: as above
	IsExploding(false), // RA2: as above
	DeathWeapon(0), // RA2: as above
	DeathWeaponDamageModifier(1.0f), // RA2: as above
	IsNoAutoFire(false), // RA2: as above
	TurretSpins(false), // RA2: as above
	Normalized(false), // RA2: as above
	IsManualReload(false), // RA2: as above
	IsVisibleLoad(false), // RA2: as above
	IsLightningRod(false), // RA2: as above
	IsHunterSeeker(false), // RA2: as above
	IsCrusher(false), // RA2: as above
	IsTiltsWhenCrushes(true), // RA2: as above
	IsSubterranean(false), // RA2: as above
	IsAutoCrush(false), // RA2: as above
	CanDisguise(false), // RA2: as above
	PermaDisguise(false), // RA2: as above
	DetectDisguise(false), // RA2: as above
	DisguiseWhenStill(false), // RA2: as above
	CanApproachTarget(true), // RA2: as above
	CanRecalcApproachTarget(true), // RA2: as above
	ImmuneToRadiation(false), // RA2: as above
	DefaultToGuardArea(false), // RA2: as above
	Warpable(true), // RA2: as above
	ReselectIfLimboed(false), // RA2: as above
	RejoinTeamIfLimboed(false), // RA2: as above
	Spawned(false), // RA2: as above
	Spawns(0), // RA2: as above
	SpawnsNumber(0), // RA2: as above
	SpawnRegenRate(0), // RA2: as above
	SpawnReloadRate(0), // RA2: as above
	MissileSpawn(false), // RA2: as above
	Underwater(false), // RA2: as above
	BalloonHover(false), // RA2: as above
	SuppressionThreshold(0), // RA2: as above
	JumpjetTurnRate(4), // RA2: as above
	JumpjetSpeed(14), // RA2: as above
	JumpjetClimb(5.0f), // RA2: as above
	JumpjetCrash(5.0f), // RA2: as above
	JumpjetHeight(500), // RA2: as above
	JumpjetAccel(2.0f), // RA2: as above
	JumpjetWobbles(0.15f), // RA2: as above
	JumpjetNoWobbles(false), // RA2: as above
	JumpjetDeviation(40), // RA2: as above
	JumpJet(false), // RA2: as above
	Crashable(false), // RA2: as above
	NoShadow(false), // RA2: as above
	CanPassiveAquire(true), // RA2: as above
	CanRetaliate(true), // RA2: as above
	RequiresStolenSovietTech(false), // RA2: as above
	RequiresStolenAlliedTech(false), // RA2: as above
	RequiredHouses(-1), // RA2: as above
	ForbiddenHouses(-1), // RA2: as above
	IsSelectableCombatant(false), // RA2: as above
	IsAccelerates(true), // RA2: as above
	DisableVoxelCache(false), // RA2: as above
	DisableShadowCache(false), // RA2: as above
	ZFudgeCliff(10), // RA2: as above
	ZFudgeColumn(5), // RA2: as above
	ZFudgeTunnel(10), // RA2: as above
	ZFudgeBridge(0), // RA2: as above
	Palette(0) // RA2: as above
{
	PaletteFile[0] = '\0'; // RA2: as above
	IsSentient = true; // RA2: as above
	DebrisTypes.Clear(); // RA2: as above
	DebrisAnims.Clear(); // RA2: as above
	DebrisMaximums.Clear(); // RA2: as above
	Dock.Clear(); // RA2: as above
	int index; // RA2: as above
	for (index = 0; index < 15; index++) { // RA2: as above
		Weapons[index].Weapon = 0; // RA2: as above
		Weapons[index].BarrelLength = 0; // RA2: as above
		Weapons[index].BarrelThickness = 0; // RA2: as above
		Weapons[index].FireFLH = TPoint3D<int>(0, 0, 0); // RA2: as above
		Weapons[index].TurretLocked = false; // RA2: as above
	}
	for (index = 0; index < 15; index++) { // RA2: as above
		EliteWeapons[index].Weapon = 0; // RA2: as above
		EliteWeapons[index].BarrelLength = 0; // RA2: as above
		EliteWeapons[index].BarrelThickness = 0; // RA2: as above
		EliteWeapons[index].FireFLH = TPoint3D<int>(0, 0, 0); // RA2: as above
		EliteWeapons[index].TurretLocked = false; // RA2: as above
	}
	for (index = 0; index < 15; index++) { // RA2: as above
		TurretWeapon[index] = -1; // RA2: as above
	}
	AbstractTypePtrTracker.Add(this); // RA2: as above
	TechnoTypes.Add(this); // RA2: as above
}
#endif


/***********************************************************************************************
 * TechnoTypeClass::TechnoTypeClass -- the NoInit constructor.                                 *
 *=============================================================================================*/
TechnoTypeClass::TechnoTypeClass(NoInitClass const & x) :
	BASECLASS(x),
	VeteranAbilities(x),
	EliteAbilities(x),
	DebrisMaximums(x),
	DebrisTypes(x),
	Unused2(0),
	Dock(x),
	VoiceSelect(x),
	VoiceMove(x),
	VoiceAttack(x),
	VoiceSpecialAttack(x), // RA2: +0x424 is initialized with NoInit here (one pushed argument)
	VoiceDie(x),
	VoiceFeedback(x),
	MoveSound(x), // RA2: +0x478 as above
	DieSound(x), // RA2: +0x494 as above
	DebrisAnims(x), // RA2: +0x510 as above
	Prerequisite(x),
	PrerequisiteOverride(x), // RA2: +0x57C as above
	CameoFilename(NULL),
	Explosion(x),
	DestroyAnim(x), // RA2: +0x660 as above
	DamageParticleSystems(), // RA2: followed by the two turret animation controls
	TurretAnimData(2, 1, 1, 1), // RA2: +0xAB0 is built with travel 2, compress 1, recover 1, hold 1 before the vtables are stored
	BarrelAnimData(2, 1, 1, 1) // RA2: +0xAC4 as above
{
	bool found = false;
	for (int i = 0; i < TechnoTypes.Count(); i++) {
		if (TechnoTypes[i] == this) {
			found = true;
			break;
		}
	}
	if (!found) {
		TechnoTypes.Add(this);
	}
}



// RA2: frees the cameo shapes the type owns (meastes' inline helper of the destructor at 0x6DA950)
static inline void Free_Cameo(TechnoTypeClass * type) // RA2: as above
{
	if (type->CameoAllocated && type->CameoData) { // RA2: as above
		delete (char *)type->CameoData; // RA2: as above
		type->CameoData = NULL; // RA2: as above
		type->CameoAllocated = false; // RA2: as above
	}
}

// RA2: the same for the alternate cameo (meastes' second inline helper)
static inline void Free_Alt_Cameo(TechnoTypeClass * type) // RA2: as above
{
	if (type->AltCameoAllocated && type->AltCameoData) { // RA2: as above
		delete (char *)type->AltCameoData; // RA2: as above
		type->AltCameoData = NULL; // RA2: as above
		type->AltCameoAllocated = false; // RA2: as above
	}
}


/// <summary>
/// Destroys the object type.
/// The type removes itself from the master type lists so that nothing can look it up
/// after it is gone.
/// </summary>
TechnoTypeClass::~TechnoTypeClass(void)
{
	Free_Cameo(this); // RA2: frees the cameo shapes first (meastes' inline helper: byte at +0x610, pointer at +0x60C)
	Free_Alt_Cameo(this); // RA2: the same for the alternate cameo (+0x630, +0x62C)
	AbstractTypePtrTracker.Delete(this);
	TechnoTypes.Delete(this);
}



/// <summary>
/// RA2: whether an object of this type may follow waypoints; always so for the base type (the derived types ask it).
/// </summary>
bool TechnoTypeClass::CanUseWaypoint(void) const // RA2: at 0x6DACB0
{
	return true; // RA2: as above
}


/// <summary>
/// RA2: whether an object of this type may attack-move: the primary weapon must exist and PreventAttackMove must be clear.
/// </summary>
bool TechnoTypeClass::CanAttackMove(void) const // RA2: at 0x6DACC0
{
	if (Weapons[0].Weapon && !PreventAttackMove) { // RA2: as above
		return true; // RA2: as above
	}
	return false; // RA2: as above
}








/***********************************************************************************************
 * TechnoTypeClass::Raw_Cost -- Fetches the raw (base) cost of the object.                     *
 *                                                                                             *
 *    This routine is used to find the underlying cost for this object. The underlying cost    *
 *    does not include any free items that normally come with the object when purchased        *
 *    directly. Example: The raw cost of a refinery is the normal cost minus the cost of a     *
 *    harvester.                                                                               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the credit cost of the base object type.                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/13/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int TechnoTypeClass::Raw_Cost(void) const
{
	return(Cost);
}


/***********************************************************************************************
 * TechnoTypeClass::Get_Ownable -- Fetches the ownable bits for this object type.              *
 *                                                                                             *
 *    This routine will return the ownable bits for this object type. The ownable bits are     *
 *    a bitflag composite of the houses that can own (build) this object type.                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the ownable bits for this object type.                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int TechnoTypeClass::Get_Ownable(void) const
{
	if (IsDoubleOwned && Session.Type != GAME_NORMAL) {
		return(0x7FFFFFFF);
	}
	return(Ownable);
}


/***********************************************************************************************
 * TechnoTypeClass::Time_To_Build -- Fetches the time to build this object.                    *
 *                                                                                             *
 *    This routine will return the time it takes to construct this object. Usually the time    *
 *    to produce is directly related to cost.                                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the time to produce this object type. The time is expressed in the    *
 *          form of game ticks.                                                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int TechnoTypeClass::Time_To_Build(void) const
{
	return(Cost * Rule->BuildSpeedBias * (TICKS_PER_MINUTE / 1000.));
}


/***********************************************************************************************
 * TechnoTypeClass::Cost_Of -- Fetches the cost of this object type.                           *
 *                                                                                             *
 *    This routine will return the cost to produce an object of this type.                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the cost to produce one object of this type.                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int TechnoTypeClass::Cost_Of(HouseClass * house) const
{
	if (house) { // RA2: RA2's form
		float factor = house->Cost_Multiplier(this); // RA2: as above
		return (int)(Raw_Cost() * factor); // RA2: as above
	}
	return Raw_Cost(); // RA2: as above
}



/// <summary>
/// RA2: the money a sold object of this type returns: Soylent when set, else the cost scaled by the rules' refund
/// percentage (all of it when full); a computer house gets the full cost.
/// </summary>
int TechnoTypeClass::GetRefund(HouseClass * house, bool full) const // RA2: at 0x6DAD80 (YR's name)
{
	float refund = Rule->RefundPercent;
	if (full) {
		refund = 1.0f;
	}
	if (house != NULL) {
		float factor = house->Cost_Multiplier(this);
		if (Soylent) {
			return(Soylent * factor);
		}
		int cost = Raw_Cost() * factor;
		if (house->Is_Human_Player()) {
			cost = cost * refund;
		}
		return(cost);
	}
	return(Raw_Cost() * refund);
}





/***********************************************************************************************
 * TechnoTypeClass::Get_Cameo_Data -- Fetches the cameo image for this object type.            *
 *                                                                                             *
 *    This routine will fetch the cameo (sidebar small image) shape of this object type.       *
 *    If there is no cameo data available (typical for non-produceable units), then NULL will  *
 *    be returned.                                                                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the cameo data for this object type if present.          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void const * TechnoTypeClass::Get_Cameo_Data(void) const
{
	if (PlayerPtr && PlayerPtr->BarracksInfiltrated && Fetch_RTTI() == 0x10 && AltCameoData && IsTrainable) { // RA2: RA2's form
		return AltCameoData; // RA2: as above
	}
	if (PlayerPtr && PlayerPtr->WarFactoryInfiltrated && Fetch_RTTI() == 0x28 && AltCameoData && IsTrainable) { // RA2: as above
		return AltCameoData; // RA2: as above
	}
	return CameoData; // RA2: as above
}



/***********************************************************************************************
 * TechnoTypeClass::Repair_Cost -- Fetches the cost to repair one step.                        *
 *                                                                                             *
 *    This routine will return the cost to repair one step. At the TechnoTypeClass level,      *
 *    this merely serves as a placeholder function. The derived classes will provide a         *
 *    functional version of this routine.                                                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the cost to repair one step.                                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int TechnoTypeClass::Repair_Cost(void) const
{
	int cost = (Raw_Cost()/(MaxStrength/Rule->RepairStep)) * Rule->RepairPercent;
	return(MAX(cost, 1));
}


/***********************************************************************************************
 * TechnoTypeClass::Repair_Step -- Fetches the health to repair one step.                      *
 *                                                                                             *
 *    This routine merely serves as placeholder virtual function. The various type classes     *
 *    will override this routine to return the number of health points to repair in one        *
 *    "step".                                                                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of health points to repair in one step.                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int TechnoTypeClass::Repair_Step(void) const
{
	return(Rule->RepairStep);
}


/***********************************************************************************************
 * TechnoTypeClass::Is_Two_Shooter -- Determines if this object is a double shooter.           *
 *                                                                                             *
 *    Some objects fire two shots in quick succession. If this is true for this object, then   *
 *    a 'true' value will be returned from this routine.                                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Is this object a two shooter?                                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TechnoTypeClass::Is_Two_Shooter(void) const
{
	WeaponTypeClass *pri = Weapons[0].Weapon; // RA2: Is_Two_Shooter at 0x6DAF20 matches only so

	if (pri != NULL) {
		WeaponTypeClass *sec = Weapons[1].Weapon; // RA2: as above

		if (pri == sec || pri->Burst > 1) {
			return(true); // RA2: as above
		}

		if (sec != NULL && sec->Burst > 1) {
			return(true);
		}
	}
	return(false);
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
	val = MIN(val, 100);
	val = MAX(val, 0);
	val = ((val * (MPH_LIGHT_SPEED + 1)) / 100);
	val = MIN(val, MPH_LIGHT_SPEED);
	return (val);
}


// RA2: a recoil timing value (TurretTravel=, BarrelTravel=); the default is the current value (meastes' inline helper)
static inline int Get_Recoil(CCINIClass const & ini, char const * section, char const * entry, int value) // RA2: as above
{
	value = ini.Get_Int(section, entry, value); // RA2: as above
	return value; // RA2: as above
}

// RA2: a recoil frame count (TurretCompressFrames= ...): at least one (meastes' inline helper)
static inline int Get_Recoil_Frames(CCINIClass const & ini, char const * section, char const * entry, int frames) // RA2: as above
{
	frames = ini.Get_Int(section, entry, frames); // RA2: as above
	if (frames <= 0) { // RA2: as above
		return 1; // RA2: as above
	}
	return frames; // RA2: as above
}


/***********************************************************************************************
 * TechnoTypeClass::Read_INI -- Reads the techno type data from the INI database.              *
 *                                                                                             *
 *    Use this routine to fill in the data for this techno type class object from the          *
 *    database specified. Typical use of this is for the rules parsing.                        *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database that the information will be lifted from.   *
 *                                                                                             *
 * OUTPUT:  bool; Was the database used to extract information? A failure (false) response     *
 *                would mean that the database didn't contain a section that applies to this   *
 *                techno class object.                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/19/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: 9841 bytes differ (adopted text)
bool TechnoTypeClass::Read_INI(CCINIClass const & ini) // RA2: RA2's rules and art reading, meastes' body at 0x6DAF60
{
	if (BASECLASS::Read_INI(ini)) { // RA2: as above
		LandTargeting = ini.Get_Int(IniName, "LandTargeting", LandTargeting); // RA2: as above
		NavalTargeting = ini.Get_Int(IniName, "NavalTargeting", NavalTargeting); // RA2: as above
		Speed = ini.Get_SpeedType(IniName, "SpeedType", (::SpeedType)Speed); // RA2: as above
		IsTypeImmune = ini.Get_Bool(IniName, "TypeImmune", IsTypeImmune); // RA2: as above
		WalkRate = ini.Get_Int(IniName, "WalkRate", WalkRate); // RA2: as above
		IdleRate = ini.Get_Int(IniName, "IdleRate", IdleRate); // RA2: as above
		IsMoveToShroud = ini.Get_Bool(IniName, "MoveToShroud", IsMoveToShroud); // RA2: as above
		IsTrain = ini.Get_Bool(IniName, "IsTrain", IsTrain); // RA2: as above
		IsDoubleOwned = ini.Get_Bool(IniName, "DoubleOwned", IsDoubleOwned); // RA2: as above
		ThreatRange = ini.Get_Lepton(IniName, "GuardRange", ThreatRange); // RA2: as above
		IsExploding = ini.Get_Bool(IniName, "Explodes", IsExploding); // RA2: as above
		DeathWeapon = TGet_Class(ini, IniName, "DeathWeapon", DeathWeapon); // RA2: as above
		DeathWeaponDamageModifier = ini.Get_Float(IniName, "DeathWeaponDamageModifier", DeathWeaponDamageModifier); // RA2: as above
		FlightLevel = ini.Get_Int(IniName, "FlightLevel", FlightLevel); // RA2: as above
		IsDropship = ini.Get_Bool(IniName, "IsDropship", IsDropship); // RA2: as above
		double pitch = ini.Get_Float(IniName, "PitchAngle", -1.0); // RA2: as above
		if (pitch != -1.0) { // RA2: as above
			PitchAngle = pitch * (3.141592653589793 / 180.0); // RA2: as above
		}
		double roll = ini.Get_Float(IniName, "RollAngle", -1.0); // RA2: as above
		if (roll != -1.0) { // RA2: as above
			RollAngle = roll * (3.141592653589793 / 180.0); // RA2: as above
		}
		PitchSpeed = ini.Get_Float(IniName, "PitchSpeed", PitchSpeed); // RA2: as above
		Locomotor = ini.Get_CLSID(IniName, "Locomotor", Locomotor); // RA2: as above
		CloakingSpeed = ini.Get_Int(IniName, "CloakingSpeed", CloakingSpeed); // RA2: as above
		ThreatAvoidanceCoefficient = ini.Get_Float(IniName, "ThreatAvoidanceCoefficient", ThreatAvoidanceCoefficient); // RA2: as above
		SlowdownDistance = ini.Get_Int(IniName, "SlowdownDistance", SlowdownDistance); // RA2: as above
		DeaccelerationFactor = ini.Get_Float(IniName, "DeaccelerationFactor", DeaccelerationFactor); // RA2: as above
		AccelerationFactor = ini.Get_Float(IniName, "AccelerationFactor", AccelerationFactor); // RA2: as above
		Weight = ini.Get_Float(IniName, "Weight", Weight); // RA2: as above
		PhysicalSize = ini.Get_Float(IniName, "PhysicalSize", PhysicalSize); // RA2: as above
		Size = ini.Get_Float(IniName, "Size", Size); // RA2: as above
		SizeLimit = ini.Get_Float(IniName, "SizeLimit", SizeLimit); // RA2: as above
		HoverAttack = ini.Get_Bool(IniName, "HoverAttack", HoverAttack); // RA2: as above
		VHPScan = ini.Get_VHPScan(IniName, "VHPScan", VHPScan); // RA2: as above
		MaxDebris = ini.Get_Int(IniName, "MaxDebris", MaxDebris); // RA2: as above
		MinDebris = ini.Get_Int(IniName, "MinDebris", MinDebris); // RA2: as above
		if (MinDebris < 0) { // RA2: as above
			MinDebris = 0; // RA2: as above
		}
		if (MaxDebris < MinDebris) { // RA2: as above
			MaxDebris = MinDebris; // RA2: as above
		}
		DebrisTypes = TGet_TypeList<VoxelAnimTypeClass>(ini, IniName, "DebrisTypes", DebrisTypes); // RA2: as above
		DebrisMaximums = ini.Get_IntList(IniName, "DebrisMaximums", DebrisMaximums); // RA2: as above
		DebrisAnims = TGet_TypeList<AnimTypeClass>(ini, IniName, "DebrisAnims", DebrisAnims); // RA2: as above
		HasTurretTooltips = ini.Get_Bool(IniName, "HasTurretTooltips", HasTurretTooltips); // RA2: as above
		TurretCount = ini.Get_Int(IniName, "TurretCount", TurretCount); // RA2: as above
		WeaponCount = ini.Get_Int(IniName, "WeaponCount", WeaponCount); // RA2: as above
		IsChargeTurret = ini.Get_Bool(IniName, "IsChargeTurret", IsChargeTurret); // RA2: as above
		ClearAllWeapons = ini.Get_Bool(IniName, "ClearAllWeapons", ClearAllWeapons); // RA2: weapon slots are read per turret (WeaponN, EliteWeaponN) or as Primary/Secondary
		if (TurretCount > 0) { // RA2: as above
			for (int index = 0; index < WeaponCount; index++) { // RA2: as above
				char weapon[12]; // RA2: as above
				char elite[16]; // RA2: as above
				sprintf(weapon, "Weapon%d", index + 1); // RA2: as above
				sprintf(elite, "EliteWeapon%d", index + 1); // RA2: as above
				Weapons[index].Weapon = TGet_Class(ini, IniName, weapon, Weapons[index].Weapon); // RA2: as above
				EliteWeapons[index].Weapon = TGet_Class(ini, IniName, elite, EliteWeapons[index].Weapon); // RA2: as above
			}
		} else if (!ClearAllWeapons) { // RA2: weapon slots are read per turret (WeaponN, EliteWeaponN) or as Primary/Secondary
			Weapons[0].Weapon = TGet_Class(ini, IniName, "Primary", Weapons[0].Weapon); // RA2: as above
			Weapons[1].Weapon = TGet_Class(ini, IniName, "Secondary", Weapons[1].Weapon); // RA2: as above
			EliteWeapons[0].Weapon = TGet_Class(ini, IniName, "ElitePrimary", EliteWeapons[0].Weapon); // RA2: as above
			EliteWeapons[1].Weapon = TGet_Class(ini, IniName, "EliteSecondary", EliteWeapons[1].Weapon); // RA2: as above
		}
		if (ClearAllWeapons) { // RA2: weapon slots are read per turret (WeaponN, EliteWeaponN) or as Primary/Secondary
			Weapons[0].Weapon = 0; // RA2: as above
			Weapons[1].Weapon = 0; // RA2: as above
			EliteWeapons[0].Weapon = 0; // RA2: as above
			EliteWeapons[1].Weapon = 0; // RA2: as above
		}
		VoiceMove = CCINIClass::Get_VocType_List(ini, IniName, "VoiceMove", VoiceMove); // RA2: as above
		VoiceSelect = CCINIClass::Get_VocType_List(ini, IniName, "VoiceSelect", VoiceSelect); // RA2: as above
		VoiceAttack = CCINIClass::Get_VocType_List(ini, IniName, "VoiceAttack", VoiceAttack); // RA2: as above
		VoiceSpecialAttack = CCINIClass::Get_VocType_List(ini, IniName, "VoiceSpecialAttack", VoiceSpecialAttack); // RA2: as above
		VoiceDie = CCINIClass::Get_VocType_List(ini, IniName, "VoiceDie", VoiceDie); // RA2: as above
		VoiceFeedback = CCINIClass::Get_VocType_List(ini, IniName, "VoiceFeedback", VoiceFeedback); // RA2: as above
		AuxSound1 = ini.Get_VocType(IniName, "AuxSound1", (VocType)AuxSound1); // RA2: as above
		AuxSound2 = ini.Get_VocType(IniName, "AuxSound2", (VocType)AuxSound2); // RA2: as above
		CreateSound = ini.Get_VocType(IniName, "CreateSound", (VocType)CreateSound); // RA2: as above
		DamageSound = ini.Get_VocType(IniName, "DamageSound", (VocType)DamageSound); // RA2: as above
		ImpactWaterSound = ini.Get_VocType(IniName, "ImpactWaterSound", (VocType)ImpactWaterSound); // RA2: as above
		ImpactLandSound = ini.Get_VocType(IniName, "ImpactLandSound", (VocType)ImpactLandSound); // RA2: as above
		CrashingSound = ini.Get_VocType(IniName, "CrashingSound", (VocType)CrashingSound); // RA2: as above
		SinkingSound = ini.Get_VocType(IniName, "SinkingSound", (VocType)SinkingSound); // RA2: as above
		VoiceFalling = ini.Get_VocType(IniName, "VoiceFalling", (VocType)VoiceFalling); // RA2: as above
		VoiceCrashing = ini.Get_VocType(IniName, "VoiceCrashing", (VocType)VoiceCrashing); // RA2: as above
		VoiceSinking = ini.Get_VocType(IniName, "VoiceSinking", (VocType)VoiceSinking); // RA2: as above
		VoiceEnter = ini.Get_VocType(IniName, "VoiceEnter", (VocType)VoiceEnter); // RA2: as above
		VoiceCapture = ini.Get_VocType(IniName, "VoiceCapture", (VocType)VoiceCapture); // RA2: as above
		IsCloakStop = ini.Get_Bool(IniName, "CloakStop", IsCloakStop); // RA2: as above
		Capacity = ini.Get_Int(IniName, "Storage", Capacity); // RA2: as above
		BuildLimit = ini.Get_Int(IniName, "BuildLimit", BuildLimit); // RA2: as above
		Category = ini.Get_CategoryType(IniName, "Category", (CategoryType)Category); // RA2: as above
		Dock = TGet_TypeList<BuildingTypeClass>(ini, IniName, "Dock", Dock); // RA2: as above
		DeploysInto = TGet_Class(ini, IniName, "DeploysInto", DeploysInto); // RA2: as above
		UndeploysInto = TGet_Class(ini, IniName, "UndeploysInto", UndeploysInto); // RA2: as above
		IsLightningRod = ini.Get_Bool(IniName, "LightningRod", IsLightningRod); // RA2: as above
		IsManualReload = ini.Get_Bool(IniName, "ManualReload", IsManualReload); // RA2: as above
		TurretSpins = ini.Get_Bool(IniName, "TurretSpins", TurretSpins); // RA2: as above
		IsTurretEquipped = ini.Get_Bool(IniName, "Turret", IsTurretEquipped); // RA2: as above
		TurretRotateSound = ini.Get_VocType(IniName, "TurretRotateSound", (VocType)TurretRotateSound); // RA2: as above
		EnterTransportSound = ini.Get_VocType(IniName, "EnterTransportSound", (VocType)EnterTransportSound); // RA2: as above
		LeaveTransportSound = ini.Get_VocType(IniName, "LeaveTransportSound", (VocType)LeaveTransportSound); // RA2: as above
		MoveSound = CCINIClass::Get_VocType_List(ini, IniName, "MoveSound", MoveSound); // RA2: as above
		DieSound = CCINIClass::Get_VocType_List(ini, IniName, "DieSound", DieSound); // RA2: as above
		DeploySound = ini.Get_VocType(IniName, "DeploySound", (VocType)DeploySound); // RA2: as above
		UndeploySound = ini.Get_VocType(IniName, "UndeploySound", (VocType)UndeploySound); // RA2: as above
		ChronoInSound = ini.Get_VocType(IniName, "ChronoInSound", (VocType)ChronoInSound); // RA2: as above
		ChronoOutSound = ini.Get_VocType(IniName, "ChronoOutSound", (VocType)ChronoOutSound); // RA2: as above
		Explosion = TGet_TypeList<AnimTypeClass>(ini, IniName, "Explosion", Explosion); // RA2: as above
		DestroyAnim = TGet_TypeList<AnimTypeClass>(ini, IniName, "DestroyAnim", DestroyAnim); // RA2: as above
		NaturalParticleSystem = TGet_Class(ini, IniName, "NaturalParticleSystem", NaturalParticleSystem); // RA2: as above
		NaturalParticleLocation = ini.Get_Offset(IniName, "NaturalParticleLocation", NaturalParticleLocation); // RA2: as above
		DamageParticleSystems = TGet_TypeList<ParticleSystemTypeClass>(ini, IniName, "DamageParticleSystems", DamageParticleSystems); // RA2: as above
		DestroyParticleSystems = TGet_TypeList<ParticleSystemTypeClass>(ini, IniName, "DestroyParticleSystems", DestroyParticleSystems); // RA2: as above
		DamageSmokeOffset = ini.Get_Offset(IniName, "DamageSmokeOffset", DamageSmokeOffset); // RA2: as above
		DamSmkOffScrnRel = ini.Get_Bool(IniName, "DamSmkOffScrnRel", DamSmkOffScrnRel); // RA2: as above
		DestroySmokeOffset = ini.Get_Offset(IniName, "DestroySmokeOffset", DestroySmokeOffset); // RA2: as above
		RefinerySmokeOffsetOne = ini.Get_Offset(IniName, "RefnSmokeOffsetOne", RefinerySmokeOffsetOne); // RA2: as above
		RefinerySmokeOffsetTwo = ini.Get_Offset(IniName, "RefnSmokeOffsetTwo", RefinerySmokeOffsetTwo); // RA2: as above
		IsNominal = ini.Get_Bool(IniName, "Nominal", IsNominal); // RA2: as above
		IsCloakable = ini.Get_Bool(IniName, "Cloakable", IsCloakable); // RA2: as above
		GapGenerator = ini.Get_Bool(IniName, "GapGenerator", GapGenerator); // RA2: as above
		GapRadiusInCells = ini.Get_Int(IniName, "GapRadiusInCells", GapRadiusInCells); // RA2: as above
		SuperGapRadiusInCells = ini.Get_Int(IniName, "SuperGapRadiusInCells", SuperGapRadiusInCells); // RA2: as above
		Teleporter = ini.Get_Bool(IniName, "Teleporter", Teleporter); // RA2: as above
		IsScanner = ini.Get_Bool(IniName, "Sensors", IsScanner); // RA2: as above
		PipScale = ini.Get_PipScaleType(IniName, "PipScale", PipScale); // RA2: as above
		PipWrap = ini.Get_Int(IniName, "PipWrap", PipWrap); // RA2: as above
		Prerequisite = CCINIClass::Get_BuildingType_List(ini, IniName, "Prerequisite", Prerequisite); // RA2: as above
		PrerequisiteOverride = CCINIClass::Get_BuildingType_List(ini, IniName, "PrerequisiteOverride", PrerequisiteOverride); // RA2: as above
		SightRange = ini.Get_Int(IniName, "Sight", SightRange); // RA2: as above
		ReselectIfLimboed = ini.Get_Bool(IniName, "ReselectIfLimboed", ReselectIfLimboed); // RA2: as above
		RejoinTeamIfLimboed = ini.Get_Bool(IniName, "RejoinTeamIfLimboed", RejoinTeamIfLimboed); // RA2: as above
		SensorsSight = ini.Get_Int(IniName, "SensorsSight", SensorsSight); // RA2: as above
		DetectDisguiseRange = ini.Get_Int(IniName, "DetectDisguiseRange", DetectDisguiseRange); // RA2: as above
		BombSight = ini.Get_Int(IniName, "BombSight", BombSight); // RA2: as above
		LeadershipRating = ini.Get_Int(IniName, "LeadershipRating", LeadershipRating); // RA2: as above
		RevealToAll = ini.Get_Bool(IniName, "RevealToAll", RevealToAll); // RA2: as above
		CanDisguise = ini.Get_Bool(IniName, "CanDisguise", CanDisguise); // RA2: as above
		PermaDisguise = ini.Get_Bool(IniName, "PermaDisguise", PermaDisguise); // RA2: as above
		DetectDisguise = ini.Get_Bool(IniName, "DetectDisguise", DetectDisguise); // RA2: as above
		DisguiseWhenStill = ini.Get_Bool(IniName, "DisguiseWhenStill", DisguiseWhenStill); // RA2: as above
		CanPassiveAquire = ini.Get_Bool(IniName, "CanPassiveAquire", CanPassiveAquire); // RA2: as above
		CanRetaliate = ini.Get_Bool(IniName, "CanRetaliate", CanRetaliate); // RA2: as above
		CanApproachTarget = ini.Get_Bool(IniName, "CanApproachTarget", CanApproachTarget); // RA2: as above
		CanRecalcApproachTarget = ini.Get_Bool(IniName, "CanRecalcApproachTarget", CanRecalcApproachTarget); // RA2: as above
		RequiresStolenSovietTech = ini.Get_Bool(IniName, "RequiresStolenSovietTech", RequiresStolenSovietTech); // RA2: as above
		RequiresStolenAlliedTech = ini.Get_Bool(IniName, "RequiresStolenAlliedTech", RequiresStolenAlliedTech); // RA2: as above
		RequiredHouses = ini.Get_Owners(IniName, "RequiredHouses", RequiredHouses); // RA2: as above
		ForbiddenHouses = ini.Get_Owners(IniName, "ForbiddenHouses", ForbiddenHouses); // RA2: as above
		Level = ini.Get_Int(IniName, "TechLevel", Level); // RA2: as above
		int speed = ini.Get_Int(IniName, "Speed", -1); // RA2: as above
		if (speed != -1) { // RA2: as above
			if (speed >= 100) { // RA2: as above
				speed = 100; // RA2: as above
			}
			speed = speed <= 0 ? 0 : speed; // RA2: as above
			int value = speed * 256 / 100; // RA2: as above
			MaxSpeed = (MPHType)(value < 255 ? value : 255); // RA2: Speed= scales 0..100 to 0..255 into MaxSpeed (+0x5A0)
		}
		Cost = ini.Get_Int(IniName, "Cost", Cost); // RA2: as above
		Soylent = ini.Get_Int(IniName, "Soylent", Soylent); // RA2: as above
		UnloadingClass = TGet_Class(ini, IniName, "UnloadingClass", UnloadingClass); // RA2: as above
		InitialAmmo = ini.Get_Int(IniName, "InitialAmmo", InitialAmmo); // RA2: as above
		MaxAmmo = ini.Get_Int(IniName, "Ammo", MaxAmmo); // RA2: as above
		IFVMode = ini.Get_Int(IniName, "IFVMode", IFVMode); // RA2: as above
		AirRangeBonus = ini.Get_Lepton(IniName, "AirRangeBonus", AirRangeBonus); // RA2: as above
		RadialFireSegments = ini.Get_Int(IniName, "RadialFireSegments", RadialFireSegments); // RA2: as above
		DeployFireWeapon = ini.Get_Int(IniName, "DeployFireWeapon", DeployFireWeapon); // RA2: as above
		DeployFire = ini.Get_Bool(IniName, "DeployFire", DeployFire); // RA2: as above
		MobileFire = ini.Get_Bool(IniName, "MobileFire", MobileFire); // RA2: as above
		OpportunityFire = ini.Get_Bool(IniName, "OpportunityFire", OpportunityFire); // RA2: as above
		DistributedFire = ini.Get_Bool(IniName, "DistributedFire", DistributedFire); // RA2: as above
		Reload = ini.Get_Int(IniName, "Reload", Reload); // RA2: as above
		EmptyReload = ini.Get_Int(IniName, "EmptyReload", EmptyReload); // RA2: as above
		ReloadIncrement = ini.Get_Int(IniName, "ReloadIncrement", ReloadIncrement); // RA2: as above
		DamageReducesReadiness = ini.Get_Bool(IniName, "DamageReducesReadiness", DamageReducesReadiness); // RA2: as above
		ReadinessReductionMultiplier = ini.Get_Float(IniName, "ReadinessReductionMultiplier", ReadinessReductionMultiplier); // RA2: as above
		PreventAttackMove = ini.Get_Bool(IniName, "PreventAttackMove", PreventAttackMove); // RA2: as above
		Points = ini.Get_Int(IniName, "Points", Points); // RA2: as above
		Reward = Points; // RA2: as above
		Risk = ini.Get_Int(IniName, "ThreatPosed", Risk); // RA2: as above
		Ownable = ini.Get_Owners(IniName, "Owner", Ownable); // RA2: as above
		AIBasePlanningSide = ini.Get_Int(IniName, "AIBasePlanningSide", AIBasePlanningSide); // RA2: as above
		IsTrainable = ini.Get_Bool(IniName, "Trainable", IsTrainable); // RA2: as above
		IsCrew = ini.Get_Bool(IniName, "Crewed", IsCrew); // RA2: as above
		Gunner = ini.Get_Bool(IniName, "Gunner", Gunner); // RA2: as above
		Naval = ini.Get_Bool(IniName, "Naval", Naval); // RA2: as above
		IsRepairable = ini.Get_Bool(IniName, "Repairable", IsRepairable); // RA2: as above
		IsInvisible = ini.Get_Bool(IniName, "Invisible", IsInvisible); // RA2: as above
		IsRadarVisible = ini.Get_Bool(IniName, "RadarVisible", IsRadarVisible); // RA2: as above
		IsSelfHealing = ini.Get_Bool(IniName, "SelfHealing", IsSelfHealing); // RA2: as above
		IsNoAutoFire = ini.Get_Bool(IniName, "NoAutoFire", IsNoAutoFire); // RA2: as above
		ROT = ini.Get_Int(IniName, "ROT", ROT); // RA2: as above
		MaxPassengers = ini.Get_Int(IniName, "Passengers", MaxPassengers); // RA2: as above
		FireAngle = ini.Get_Int(IniName, "FireAngle", FireAngle); // RA2: as above
		DeployTime = ini.Get_Float(IniName, "DeployTime", DeployTime); // RA2: as above
		UndeployDelay = ini.Get_Int(IniName, "UndeployDelay", UndeployDelay); // RA2: as above
		IsDisableable = ini.Get_Bool(IniName, "Disableable", IsDisableable); // RA2: as above
		IsToProtect = ini.Get_Bool(IniName, "ToProtect", IsToProtect); // RA2: as above
		IsTiberiumHeal = ini.Get_Bool(IniName, "TiberiumHeal", IsTiberiumHeal); // RA2: as above
		IsImmuneToVeins = ini.Get_Bool(IniName, "ImmuneToVeins", IsImmuneToVeins); // RA2: as above
		IsAllowedToStartInMultiplayer = ini.Get_Bool(IniName, "AllowedToStartInMultiplayer", IsAllowedToStartInMultiplayer); // RA2: as above
		StupidHunt = ini.Get_Bool(IniName, "StupidHunt", StupidHunt); // RA2: as above
		IsTargetLaser = ini.Get_Bool(IniName, "TargetLaser", IsTargetLaser); // RA2: as above
		IsHunterSeeker = ini.Get_Bool(IniName, "HunterSeeker", IsHunterSeeker); // RA2: as above
		IsCrusher = ini.Get_Bool(IniName, "Crusher", IsCrusher); // RA2: as above
		IsAutoCrush = ini.Get_Bool(IniName, "AutoCrush", IsAutoCrush); // RA2: as above
		ImmuneToRadiation = ini.Get_Bool(IniName, "ImmuneToRadiation", ImmuneToRadiation); // RA2: as above
		Spawned = ini.Get_Bool(IniName, "Spawned", Spawned); // RA2: as above
		Underwater = ini.Get_Bool(IniName, "Underwater", Underwater); // RA2: as above
		BalloonHover = ini.Get_Bool(IniName, "BalloonHover", BalloonHover); // RA2: as above
		Spawns = TGet_Class(ini, IniName, "Spawns", Spawns); // RA2: as above
		SpawnRegenRate = ini.Get_Int(IniName, "SpawnRegenRate", SpawnRegenRate); // RA2: as above
		SpawnsNumber = ini.Get_Int(IniName, "SpawnsNumber", SpawnsNumber); // RA2: as above
		SpawnReloadRate = ini.Get_Int(IniName, "SpawnReloadRate", SpawnReloadRate); // RA2: as above
		MissileSpawn = ini.Get_Bool(IniName, "MissileSpawn", MissileSpawn); // RA2: as above
		DefaultToGuardArea = ini.Get_Bool(IniName, "DefaultToGuardArea", DefaultToGuardArea); // RA2: as above
		Warpable = ini.Get_Bool(IniName, "Warpable", Warpable); // RA2: as above
		Parasiteable = ini.Get_Bool(IniName, "Parasiteable", Parasiteable); // RA2: as above
		ImmuneToPsionics = ini.Get_Bool(IniName, "ImmuneToPsionics", ImmuneToPsionics); // RA2: as above
		ConsideredAircraft = ini.Get_Bool(IniName, "ConsideredAircraft", ConsideredAircraft); // RA2: as above
		Organic = ini.Get_Bool(IniName, "Organic", Organic); // RA2: as above
		SuppressionThreshold = ini.Get_Int(IniName, "SuppressionThreshold", SuppressionThreshold); // RA2: as above
		NoShadow = ini.Get_Bool(IniName, "NoShadow", NoShadow); // RA2: as above
		JumpjetTurnRate = ini.Get_Int(IniName, "JumpjetTurnRate", JumpjetTurnRate); // RA2: as above
		JumpjetSpeed = ini.Get_Int(IniName, "JumpjetSpeed", JumpjetSpeed); // RA2: as above
		JumpjetClimb = ini.Get_Float(IniName, "JumpjetClimb", JumpjetClimb); // RA2: as above
		JumpjetCrash = ini.Get_Float(IniName, "JumpjetCrash", JumpjetCrash); // RA2: as above
		JumpjetHeight = ini.Get_Int(IniName, "JumpjetHeight", JumpjetHeight); // RA2: as above
		JumpjetAccel = ini.Get_Float(IniName, "JumpjetAccel", JumpjetAccel); // RA2: as above
		JumpjetWobbles = ini.Get_Float(IniName, "JumpjetWobbles", JumpjetWobbles); // RA2: as above
		JumpjetNoWobbles = ini.Get_Bool(IniName, "JumpjetNoWobbles", JumpjetNoWobbles); // RA2: as above
		JumpjetDeviation = ini.Get_Int(IniName, "JumpjetDeviation", JumpjetDeviation); // RA2: as above
		JumpJet = ini.Get_Bool(IniName, "JumpJet", JumpJet); // RA2: as above
		Crashable = ini.Get_Bool(IniName, "Crashable", Crashable); // RA2: as above
		AttackFriendlies = ini.Get_Bool(IniName, "AttackFriendlies", AttackFriendlies); // RA2: as above
		AttackCursorOnFriendlies = ini.Get_Bool(IniName, "AttackCursorOnFriendlies", AttackCursorOnFriendlies); // RA2: as above
		TurretRecoil = ini.Get_Bool(IniName, "TurretRecoil", TurretRecoil); // RA2: as above
		TurretAnimData.Travel = Get_Recoil(ini, IniName, "TurretTravel", TurretAnimData.Travel); // RA2: as above
		TurretAnimData.CompressFrames = Get_Recoil_Frames(ini, IniName, "TurretCompressFrames", TurretAnimData.CompressFrames); // RA2: as above
		TurretAnimData.HoldFrames = Get_Recoil_Frames(ini, IniName, "TurretHoldFrames", TurretAnimData.HoldFrames); // RA2: as above
		TurretAnimData.RecoverFrames = Get_Recoil_Frames(ini, IniName, "TurretRecoverFrames", TurretAnimData.RecoverFrames); // RA2: as above
		BarrelAnimData = TurretAnimData; // RA2: as above
		BarrelAnimData.Travel = Get_Recoil(ini, IniName, "BarrelTravel", TurretAnimData.Travel); // RA2: as above
		BarrelAnimData.CompressFrames = Get_Recoil_Frames(ini, IniName, "BarrelCompressFrames", TurretAnimData.CompressFrames); // RA2: as above
		BarrelAnimData.HoldFrames = Get_Recoil_Frames(ini, IniName, "BarrelHoldFrames", TurretAnimData.HoldFrames); // RA2: as above
		BarrelAnimData.RecoverFrames = Get_Recoil_Frames(ini, IniName, "BarrelRecoverFrames", TurretAnimData.RecoverFrames); // RA2: as above
		IsTiltsWhenCrushes = ini.Get_Bool(IniName, "TiltsWhenCrushes", IsTiltsWhenCrushes); // RA2: as above
		IsAccelerates = ini.Get_Bool(IniName, "Accelerates", IsAccelerates); // RA2: as above
		ZFudgeCliff = ini.Get_Int(IniName, "ZFudgeCliff", ZFudgeCliff); // RA2: as above
		ZFudgeColumn = ini.Get_Int(IniName, "ZFudgeColumn", ZFudgeColumn); // RA2: as above
		ZFudgeTunnel = ini.Get_Int(IniName, "ZFudgeTunnel", ZFudgeTunnel); // RA2: as above
		ZFudgeBridge = ini.Get_Int(IniName, "ZFudgeBridge", ZFudgeBridge); // RA2: as above
		VeteranAbilities = ini.Get_Abilities(IniName, "VeteranAbilities", VeteranAbilities); // RA2: as above
		EliteAbilities = ini.Get_Abilities(IniName, "EliteAbilities", EliteAbilities); // RA2: as above
		MyEffectivenessCoefficient = ini.Get_Float(IniName, "MyEffectivenessCoefficient", MyEffectivenessCoefficient == 0.0 ? Rule->MyEffectivenessCoefficientDefault : MyEffectivenessCoefficient); // RA2: as above
		TargetEffectivenessCoefficient = ini.Get_Float(IniName, "TargetEffectivenessCoefficient", TargetEffectivenessCoefficient == 0.0 ? Rule->TargetEffectivenessCoefficientDefault : TargetEffectivenessCoefficient); // RA2: as above
		TargetSpecialThreatCoefficient = ini.Get_Float(IniName, "TargetSpecialThreatCoefficient", TargetSpecialThreatCoefficient == 0.0 ? Rule->TargetSpecialThreatCoefficientDefault : TargetSpecialThreatCoefficient); // RA2: as above
		TargetStrengthCoefficient = ini.Get_Float(IniName, "TargetStrengthCoefficient", TargetStrengthCoefficient == 0.0 ? Rule->TargetStrengthCoefficientDefault : TargetStrengthCoefficient); // RA2: as above
		TargetDistanceCoefficient = ini.Get_Float(IniName, "TargetDistanceCoefficient", TargetDistanceCoefficient == 0.0 ? Rule->TargetDistanceCoefficientDefault : TargetDistanceCoefficient); // RA2: as above
		SpecialThreatValue = ini.Get_Float(IniName, "SpecialThreatValue", SpecialThreatValue); // RA2: as above
		IsSelectableCombatant = ini.Get_Bool(IniName, "IsSelectableCombatant", IsSelectableCombatant); // RA2: as above

		IsLeader = false; // RA2: as above
		if (Weapons[0].Weapon != 0 && Weapons[0].Weapon->Attack > 0) { // RA2: as above
			IsLeader = true; // RA2: as above
		}

		char fullname[512]; // RA2: as above
		_makepath(fullname, 0, 0, GraphicName, ".SHP"); // RA2: as above
		if (ImageAllocated && ImageData != 0) { // RA2: as above
			delete (char *)ImageData; // RA2: as above
			ImageData = 0; // RA2: as above
			ImageAllocated = false; // RA2: as above
		}
		ImageData = (void *)MixFileClass::Retrieve(fullname); // RA2: as above

		ArtINI.Get_String(GraphicName, "Palette", "", PaletteFile, sizeof(PaletteFile)); // RA2: as above
		Load_Palette(); // RA2: Load_Palette 0x006DF620 builds the schemes of Palette= (outside this module)
		if (Palette_Scheme_Count() > 1) { // RA2: 0x00603D50 counts the palettes that have a scheme list
			DebugString("You have violated the limit of having only one extra building palette.\n"); // RA2: the exe aborts when more than one extra building palette is loaded
			Emergency_Exit(); // RA2: as above
		}
		TurretOffset = ArtINI.Get_Int(GraphicName, "TurretOffset", TurretOffset); // RA2: as above
		Rotation = ArtINI.Get_Int(GraphicName, "RotCount", Rotation); // RA2: as above
		IsRemappable = ArtINI.Get_Bool(GraphicName, "Remapable", IsRemappable); // RA2: as above
		Normalized = ArtINI.Get_Bool(GraphicName, "Normalized", Normalized); // RA2: as above
		IsVisibleLoad = ArtINI.Get_Bool(GraphicName, "VisibleLoad", IsVisibleLoad); // RA2: as above
		ShadowIndex = ArtINI.Get_Int(GraphicName, "ShadowIndex", ShadowIndex); // RA2: as above
		DisableVoxelCache = ArtINI.Get_Bool(GraphicName, "DisableVoxelCache", DisableVoxelCache); // RA2: as above
		DisableShadowCache = ArtINI.Get_Bool(GraphicName, "DisableShadowCache", DisableShadowCache); // RA2: as above
		char cameo[32]; // RA2: as above
		ArtINI.Get_String(GraphicName, "Cameo", "", cameo, sizeof(CameoFilename)); // RA2: as above
		if (strlen(cameo) != 0) { // RA2: as above
			strcpy(CameoFilename, cameo); // RA2: as above
			_makepath(fullname, 0, 0, CameoFilename, ".SHP"); // RA2: as above
			if (CameoData == 0) { // RA2: as above
				CameoData = (void *)MixFileClass::Retrieve(fullname); // RA2: as above
			}
		}
		if (CameoData == 0) { // RA2: as above
			CameoData = (void *)MixFileClass::Retrieve("XXICON.SHP"); // RA2: as above
		}
		ArtINI.Get_String(GraphicName, "AltCameo", "", cameo, sizeof(AltCameoFilename)); // RA2: the alternate cameo, read the same way as Cameo
		if (strlen(cameo) != 0) { // RA2: as above
			strcpy(AltCameoFilename, cameo); // RA2: as above
			_makepath(fullname, 0, 0, AltCameoFilename, ".SHP"); // RA2: as above
			if (AltCameoData == 0) { // RA2: as above
				AltCameoData = (void *)MixFileClass::Retrieve(fullname); // RA2: as above
			}
		}

		if (TurretCount > 0) { // RA2: as above
			for (int index = 0; index < WeaponCount; index++) { // RA2: as above
				char weapon[32]; // RA2: as above
				char entry[128]; // RA2: as above
				sprintf(weapon, "Weapon%d", index + 1); // RA2: as above
				sprintf(entry, "%sFLH", weapon); // RA2: as above
				Weapons[index].FireFLH = ArtINI.Get_Point(GraphicName, entry, Weapons[index].FireFLH); // RA2: as above
				sprintf(entry, "%sBarrelLength", weapon); // RA2: as above
				Weapons[index].BarrelLength = ArtINI.Get_Int(GraphicName, entry, Weapons[index].BarrelLength); // RA2: as above
				sprintf(entry, "%sBarrelThickness", weapon); // RA2: as above
				Weapons[index].BarrelThickness = ArtINI.Get_Int(GraphicName, entry, Weapons[index].BarrelThickness); // RA2: as above
				sprintf(entry, "%sTurretLocked", weapon); // RA2: as above
				Weapons[index].TurretLocked = ArtINI.Get_Bool(GraphicName, entry, Weapons[index].TurretLocked); // RA2: as above

				sprintf(weapon, "EliteWeapon%d", index + 1); // RA2: as above
				sprintf(entry, "%sFLH", weapon); // RA2: as above
				EliteWeapons[index].FireFLH = ArtINI.Get_Point(GraphicName, entry, Weapons[index].FireFLH); // RA2: as above
				sprintf(entry, "%sBarrelLength", weapon); // RA2: as above
				EliteWeapons[index].BarrelLength = ArtINI.Get_Int(GraphicName, entry, Weapons[index].BarrelLength); // RA2: as above
				sprintf(entry, "%sBarrelThickness", weapon); // RA2: as above
				EliteWeapons[index].BarrelThickness = ArtINI.Get_Int(GraphicName, entry, Weapons[index].BarrelThickness); // RA2: as above
				sprintf(entry, "%sTurretLocked", weapon); // RA2: as above
				EliteWeapons[index].TurretLocked = ArtINI.Get_Bool(GraphicName, entry, Weapons[index].TurretLocked); // RA2: as above
			}
		} else { // RA2: as above
			Weapons[0].FireFLH = ArtINI.Get_Point(GraphicName, "PrimaryFireFLH", Weapons[0].FireFLH); // RA2: as above
			Weapons[0].BarrelLength = ArtINI.Get_Int(GraphicName, "PBarrelLength", Weapons[0].BarrelLength); // RA2: as above
			Weapons[0].BarrelThickness = ArtINI.Get_Int(GraphicName, "PBarrelThickness", Weapons[0].BarrelThickness); // RA2: as above
			Weapons[1].FireFLH = ArtINI.Get_Point(GraphicName, "SecondaryFireFLH", Weapons[1].FireFLH); // RA2: as above
			Weapons[1].BarrelLength = ArtINI.Get_Int(GraphicName, "SBarrelLength", Weapons[1].BarrelLength); // RA2: as above
			Weapons[1].BarrelThickness = ArtINI.Get_Int(GraphicName, "SBarrelThickness", Weapons[1].BarrelThickness); // RA2: as above
			EliteWeapons[0].FireFLH = ArtINI.Get_Point(GraphicName, "ElitePrimaryFireFLH", Weapons[0].FireFLH); // RA2: as above
			EliteWeapons[0].BarrelLength = ArtINI.Get_Int(GraphicName, "ElitePBarrelLength", Weapons[0].BarrelLength); // RA2: as above
			EliteWeapons[0].BarrelThickness = ArtINI.Get_Int(GraphicName, "ElitePBarrelThickness", Weapons[0].BarrelThickness); // RA2: as above
			EliteWeapons[1].FireFLH = ArtINI.Get_Point(GraphicName, "EliteSecondaryFireFLH", Weapons[1].FireFLH); // RA2: as above
			EliteWeapons[1].BarrelLength = ArtINI.Get_Int(GraphicName, "EliteSBarrelLength", Weapons[1].BarrelLength); // RA2: as above
			EliteWeapons[1].BarrelThickness = ArtINI.Get_Int(GraphicName, "EliteSBarrelThickness", Weapons[1].BarrelThickness); // RA2: as above
		}
		TurretNotExportedOnGround = ArtINI.Get_Bool(GraphicName, "TurretNotExportedOnGround", TurretNotExportedOnGround); // RA2: as above
		MZone = ini.Get_MZoneType(IniName, "MovementZone", MZone); // RA2: as above
		IsSubterranean = MZone == 6; // RA2: as above

		if (IsVoxel) { // RA2: as above
			Fetch_Voxel_Image(); // RA2: as above
		} else if (IsTurretEquipped) { // RA2: as above
			Fetch_Aux_Voxel_Image(); // RA2: as above
		}
		if (Voxel.VoxLib != NULL && !Voxel.VoxLib->Load_Failed()) { // RA2: as above
			VoxelCenterY = Voxel.VoxLib->Get_Layer_Info(0, 0).YSize * 0.5f + 0.5; // RA2: as above
			VoxelCenterX = Voxel.VoxLib->Get_Layer_Info(0, 0).XSize * 0.5f; // RA2: as above
		}

		return(true); // RA2: as above
	}
	return(false); // RA2: as above
}
#endif



/// <summary>
/// Determines if this object type may legally be placed at the specified location.
/// Every square of the object's foundation is examined for obstacles -- buildings are
/// weighed against the house's own build restrictions, everything else merely has to fit.
/// A building that paves what it covers is more forgiving, since it only needs one square
/// of the foundation to be free.
/// </summary>
/// <param name="pos">The cell to anchor the object's foundation at.</param>
/// <param name="house">The house that wishes to do the placing.</param>
/// <returns>bool; May the object be placed at this location?</returns>
bool TechnoTypeClass::Legal_Placement(Cell const & pos, HouseClass * house) const
{
	if (pos == CELL_NONE) return(0);

	Cell const * offset = Occupy_List(true);
	bool build = (Fetch_RTTI() == RTTI_BUILDINGTYPE); // RA2: Legal_Placement at 0x6DE0E0 matches only so
	bool any_blocked = false;
	bool any_clear = false;

	BuildingTypeClass * bptr = (BuildingTypeClass *)this;

	while (offset != NULL && *offset != REFRESH_EOL) {
		Cell cell = pos + *offset++;

		if (build) {
			if (!Map[cell].Is_Clear_To_Build(bptr->Speed, bptr, house)) {
				any_blocked = true;
			} else {
				any_clear = true;
			}
		} else {
			if (!Map[cell].Is_Clear_To_Move(Speed, false, false)) {
				any_blocked = true;
			} else {
				any_clear = true;
			}
		}
	}

	if (build && bptr->ToTile != NULL) {
		return(any_clear);
	}
	return(any_blocked == 0);
}


/// <summary>
/// Fetches the number of pips this object type displays.
/// The pip scale assigned in the rules decides both what the pips stand for -- ammo,
/// passengers, tiberium, power or charge -- and how many of them there can be.
/// </summary>
/// <returns>Returns with the maximum pip count, or zero if this object type shows none.</returns>
int TechnoTypeClass::Max_Pips(void) const
{
	switch (PipScale) {
	case 4: // RA2: RA2's form
		return 10; // RA2: as above
	case 1: // RA2: as above
		return MaxAmmo; // RA2: as above
	case 2: // RA2: as above
		return 5; // RA2: as above
	case 3: // RA2: as above
		return MaxPassengers; // RA2: as above
	}
	return(0);
}



/// <summary>
/// Loads this object type from a save game stream.
/// The variable length lists are read back behind the flat data and every pointer is
/// swizzled to its new home. Artwork is never written to a save game, so the shape and
/// cameo images are re-fetched from the mix files here. Save games written by an older
/// version are shifted into the current member layout as they arrive.
/// </summary>
/// <returns>Returns with S_OK if the object type was restored, otherwise the failure
/// code.</returns>
HRESULT STDMETHODCALLTYPE TechnoTypeClass::Load(IStream *stream)
{
	int i;


	Dock.Clear(); // RA2: no Fetch_Object_Size (TibSun's use does not compile against RA2's declaration)
	DebrisTypes.Clear();
	DebrisAnims.Clear(); // RA2: all sixteen lists are cleared, in this order (meastes' body)
	VoiceSelect.Clear();
	VoiceMove.Clear();
	VoiceAttack.Clear();
	VoiceSpecialAttack.Clear(); // RA2: as above
	VoiceDie.Clear();
	VoiceFeedback.Clear();
	MoveSound.Clear(); // RA2: as above
	DieSound.Clear(); // RA2: as above
	Explosion.Clear();
	DestroyAnim.Clear(); // RA2: as above
	Prerequisite.Clear();
	PrerequisiteOverride.Clear(); // RA2: as above
	DebrisMaximums.Clear();

	HRESULT result = BASECLASS::Load(stream);
	if (SUCCEEDED(result)) {

		Swizzle_Pointer(&DeploysInto); // RA2: no CollateralDamageCoefficient, IsOldSaveGame (TibSun's use does not compile against RA2's declaration)
		Swizzle_Pointer(&UndeploysInto);
		Swizzle_Pointer(&NaturalParticleSystem);
		Swizzle_Pointer(&Spawns); // RA2: +0xB08, swizzled after the first three
		Swizzle_Pointer(&DeathWeapon); // RA2: +0xAE4, as above
		Swizzle_Pointer(&UnloadingClass); // RA2: +0x5D8, as above

		Dock.Load(stream);
		DebrisTypes.Load(stream);
		DebrisAnims.Load(stream); // RA2: the lists are loaded in this order (meastes' body)
		VoiceSelect.Load(stream);
		VoiceMove.Load(stream);
		VoiceAttack.Load(stream);
		VoiceSpecialAttack.Load(stream); // RA2: as above
		VoiceDie.Load(stream);
		VoiceFeedback.Load(stream);
		MoveSound.Load(stream); // RA2: as above
		DieSound.Load(stream); // RA2: as above
		Explosion.Load(stream);
		DestroyAnim.Load(stream); // RA2: as above
		DebrisMaximums.Load(stream);
		Prerequisite.Load(stream);
		PrerequisiteOverride.Load(stream); // RA2: as above
		DamageParticleSystems.Load(stream);
		DestroyParticleSystems.Load(stream); // RA2: as above

		for (i = 0; i < Dock.Count(); i++) {
			Swizzle_Pointer(&Dock[i]);
		}
		for (i = 0; i < Explosion.Count(); i++) {
			Swizzle_Pointer(&Explosion[i]);
		}
		for (i = 0; i < DestroyAnim.Count(); i++) { // RA2: as above
			Swizzle_Pointer(&DestroyAnim[i]); // RA2: as above
		}
		for (i = 0; i < DebrisTypes.Count(); i++) {
			Swizzle_Pointer(&DebrisTypes[i]);
		}
		for (i = 0; i < DebrisAnims.Count(); i++) { // RA2: as above
			Swizzle_Pointer(&DebrisAnims[i]); // RA2: as above
		}
		for (i = 0; i < 15; i++) { // RA2: all fifteen slots (WEAPON_SLOT_COUNT is TibSun's 3)
			Swizzle_Pointer(&Weapons[i].Weapon);
		}
		for (i = 0; i < 15; i++) { // RA2: the elite weapons are swizzled too
			Swizzle_Pointer(&EliteWeapons[i].Weapon); // RA2: as above
		}
		for (i = 0; i < DamageParticleSystems.Count(); i++) {
			Swizzle_Pointer(&DamageParticleSystems[i]);
		}
		for (i = 0; i < DestroyParticleSystems.Count(); i++) { // RA2: as above
			Swizzle_Pointer(&DestroyParticleSystems[i]); // RA2: as above
		}

		char buffer[256];
		char fname[_MAX_PATH];

		_makepath(fname, NULL, NULL, (const char *)GraphicName, ".SHP");
		ImageData = MFCD::Retrieve(fname);

		ArtINI.Get_String((const char *)GraphicName, "Cameo", "XXICON", buffer, sizeof(buffer));
		if (stricmp(buffer, "XXICON") == 0) {
			ArtINI.Get_String((const char *)IniName, "Cameo", "XXICON", buffer, sizeof(buffer));
		}
		_makepath(fname, NULL, NULL, buffer, ".SHP");
		CameoData = NULL; // RA2: cleared first, then looked up
		CameoData = (const ShapeSet *)MFCD::Retrieve(fname);
		if (CameoData == NULL) { // RA2: falls back to the generic cameo
			CameoData = (const ShapeSet *)MFCD::Retrieve("XXICON.SHP"); // RA2: as above
		}

		ArtINI.Get_String((const char *)GraphicName, "AltCameo", "", buffer, sizeof(buffer)); // RA2: the alternate cameo, no fallback
		_makepath(fname, NULL, NULL, buffer, ".SHP"); // RA2: as above
		AltCameoData = NULL; // RA2: as above
		AltCameoData = (const ShapeSet *)MFCD::Retrieve(fname); // RA2: as above

		ArtINI.Get_String((const char *)GraphicName, "Palette", "", PaletteFile, sizeof(PaletteFile)); // RA2: the palette named by Palette=
		if (PaletteFile[0] != '\0') { // RA2: as above
			Palette = Generate_Palette(PaletteFile); // RA2: Generate_Palette 0x00603380, declared in anim.cpp as well
		}

		result = S_OK;
	}
	return(result);
}



/// <summary>
/// Saves this object type to a save game stream.
/// The base class writes the flat member data, then the variable length lists this object
/// type carries are written out behind it.
/// </summary>
/// <returns>Returns with S_OK if the object type was written, otherwise the failure code.</returns>
HRESULT STDMETHODCALLTYPE TechnoTypeClass::Save(IStream *stream, int cleardirty)
{
	HRESULT result = ObjectTypeClass::Save(stream, cleardirty); // RA2: RA2's form
	if (FAILED(result)) { // RA2: as above
		return result; // RA2: as above
	}
	Dock.Save(stream); // RA2: as above
	DebrisTypes.Save(stream); // RA2: as above
	DebrisAnims.Save(stream); // RA2: as above
	VoiceSelect.Save(stream); // RA2: as above
	VoiceMove.Save(stream); // RA2: as above
	VoiceAttack.Save(stream); // RA2: as above
	VoiceSpecialAttack.Save(stream); // RA2: as above
	VoiceDie.Save(stream); // RA2: as above
	VoiceFeedback.Save(stream); // RA2: as above
	MoveSound.Save(stream); // RA2: as above
	DieSound.Save(stream); // RA2: as above
	Explosion.Save(stream); // RA2: as above
	DestroyAnim.Save(stream); // RA2: as above
	DebrisMaximums.Save(stream); // RA2: as above
	Prerequisite.Save(stream); // RA2: as above
	PrerequisiteOverride.Save(stream); // RA2: as above
	DamageParticleSystems.Save(stream); // RA2: as above
	DestroyParticleSystems.Save(stream); // RA2: as above
	return S_OK; // RA2: as above
}



/// <summary>
/// Fetches the largest number of bytes this object type could occupy in a stream.
/// The variable length lists this object type carries are added on top of whatever the
/// base class asks for, so the persistence layer can size its buffer up front.
/// </summary>
/// <param name="pcbSize">The running size total to add this object type's needs to.</param>
/// <returns>Returns with S_OK, or the failure code reported by the base class.</returns>
HRESULT STDMETHODCALLTYPE TechnoTypeClass::GetSizeMax(ULARGE_INTEGER *pcbSize)
{
	HRESULT result = ObjectTypeClass::GetSizeMax(pcbSize); // RA2: RA2's form
	if (FAILED(result)) { // RA2: as above
		return result; // RA2: as above
	}
	pcbSize->LowPart += VoiceSelect.Count() * sizeof(int) + sizeof(int); // RA2: as above
	pcbSize->LowPart += VoiceMove.Count() * sizeof(int) + sizeof(int); // RA2: as above
	pcbSize->LowPart += VoiceAttack.Count() * sizeof(int) + sizeof(int); // RA2: as above
	pcbSize->LowPart += VoiceSpecialAttack.Count() * sizeof(int) + sizeof(int); // RA2: as above
	pcbSize->LowPart += VoiceDie.Count() * sizeof(int) + sizeof(int); // RA2: as above
	pcbSize->LowPart += VoiceFeedback.Count() * sizeof(int) + sizeof(int); // RA2: as above
	pcbSize->LowPart += MoveSound.Count() * sizeof(int) + sizeof(int); // RA2: as above
	pcbSize->LowPart += DieSound.Count() * sizeof(int) + sizeof(int); // RA2: as above
	pcbSize->LowPart += Explosion.Count() * sizeof(AnimTypeClass *) + sizeof(int); // RA2: as above
	pcbSize->LowPart += DestroyAnim.Count() * sizeof(AnimTypeClass *) + sizeof(int); // RA2: as above
	pcbSize->LowPart += DebrisTypes.Count() * sizeof(VoxelAnimTypeClass *) + sizeof(int); // RA2: as above
	pcbSize->LowPart += DebrisAnims.Count() * sizeof(AnimTypeClass *) + sizeof(int); // RA2: as above
	pcbSize->LowPart += Dock.Count() * sizeof(BuildingTypeClass *) + sizeof(int); // RA2: as above
	pcbSize->LowPart += DebrisMaximums.Count() * sizeof(int) + sizeof(int); // RA2: as above
	pcbSize->LowPart += DamageParticleSystems.Count() * sizeof(ParticleSystemTypeClass *) + sizeof(int); // RA2: as above
	pcbSize->LowPart += DestroyParticleSystems.Count() * sizeof(ParticleSystemTypeClass *) + sizeof(int); // RA2: as above
	return S_OK; // RA2: as above
}



/// <summary>
/// Adds this object type's rule data to a running checksum.
/// This routine is used by the multiplayer sync check to prove that every machine in the
/// game is playing by the same rules.
/// </summary>
void TechnoTypeClass::Compute_CRC(class CRCEngine & crc) const
{
	AbstractTypeClass::Compute_CRC(crc); // RA2: RA2's form
	crc(ThreatAvoidanceCoefficient);
	crc((long)SlowdownDistance); // RA2: as above
	crc(DeaccelerationFactor);
	crc(AccelerationFactor);
	crc((long)CloakingSpeed); // RA2: as above
	crc((long)DebrisTypes.Count()); // RA2: as above
	crc((long)DebrisAnims.Count()); // RA2: as above
	crc((long)Dock.Count()); // RA2: as above
	crc((long)DebrisMaximums.Count()); // RA2: as above
	crc(&Locomotor, sizeof(Locomotor)); // RA2: as above
	crc(VoxelCenterY);
	crc(VoxelCenterX);
	crc(Weight);
	crc(PhysicalSize);
	crc(Size); // RA2: as above
	crc(SizeLimit); // RA2: as above
	crc(HoverAttack); // RA2: as above
	crc((long)InitialMission); // RA2: as above
	crc(IsImmuneToVeins);
	crc(IsTiberiumHeal);
	crc(IsTargetLaser);
	crc(RollAngle);
	crc(PitchAngle);
	crc(PitchSpeed);
	crc((long)FlightLevel); // RA2: as above
	crc((long)BuildLimit); // RA2: as above
	crc((long)Category); // RA2: as above
	crc(DeployTime);
	crc((long)FireAngle); // RA2: as above
	crc((long)PipScale); // RA2: as above
	crc((long)VoiceSelect.Count()); // RA2: as above
	crc((long)VoiceMove.Count()); // RA2: as above
	crc((long)VoiceAttack.Count()); // RA2: as above
	crc((long)VoiceSpecialAttack.Count()); // RA2: as above
	crc((long)VoiceDie.Count()); // RA2: as above
	crc((long)VoiceFeedback.Count()); // RA2: as above
	crc((long)MoveSound.Count()); // RA2: as above
	crc((long)DieSound.Count()); // RA2: as above
	crc((long)DamageParticleSystems.Count()); // RA2: as above
	crc((long)DestroyParticleSystems.Count()); // RA2: as above
	crc((long)MZone); // RA2: as above
	crc((long)ThreatRange); // RA2: as above
	crc((long)MaxDebris); // RA2: as above
	crc((long)MinDebris); // RA2: as above
	crc((long)MaxPassengers); // RA2: as above
	crc((long)SightRange); // RA2: as above
	crc((long)Cost); // RA2: as above
	crc((long)Soylent); // RA2: as above
	crc((long)Level); // RA2: as above
	crc((long)Prerequisite.Count()); // RA2: as above
	crc((long)PrerequisiteOverride.Count()); // RA2: as above
	crc((long)Risk); // RA2: as above
	crc((long)Reward); // RA2: as above
	crc((long)MaxSpeed); // RA2: as above
	crc((long)Speed); // RA2: as above
	crc((long)MaxAmmo); // RA2: as above
	crc((long)AirRangeBonus); // RA2: as above
	crc((long)Ownable); // RA2: as above
	crc((long)AIBasePlanningSide); // RA2: as above
	crc((long)Rotation); // RA2: as above
	crc((long)ROT); // RA2: as above
	crc((long)TurretOffset); // RA2: as above
	crc((long)Points); // RA2: as above
	crc((long)Explosion.Count()); // RA2: as above
	crc((long)DestroyAnim.Count()); // RA2: as above
	crc((long)ShadowIndex); // RA2: as above
	crc((long)Capacity); // RA2: as above
	crc(IsTrain);
	crc(IsDropship);
	crc(IsToProtect);
	crc(IsDisableable);
	crc(IsUnbuildable);
	crc(IsDoubleOwned);
	crc(IsInvisible);
	crc(IsRadarVisible);
	crc(IsLeader);
	crc(IsScanner);
	crc(IsNominal);
	crc(IsTurretEquipped);
	crc(IsRepairable);
	crc(IsCrew);
	crc(IsRemappable);
	crc(IsCloakable);
	crc(GapGenerator); // RA2: as above
	crc(GapRadiusInCells); // RA2: as above
	crc(SuperGapRadiusInCells); // RA2: as above
	crc(Teleporter); // RA2: as above
	crc(IsSelfHealing);
	crc(IsExploding);
	crc(IsNoAutoFire);
	crc(TurretSpins); // RA2: as above
	crc(Normalized); // RA2: as above
	crc(IsManualReload); // RA2: as above
	crc(IsVisibleLoad);
	crc(IsLightningRod);
	crc(DisableVoxelCache); // RA2: as above
	crc(DisableShadowCache); // RA2: as above
	crc(MobileFire); // RA2: as above
	crc(OpportunityFire); // RA2: as above
	crc(DistributedFire); // RA2: as above
}



/// <summary>
/// Fetches the weapon data for one of this object type's weapon slots.
/// An object type that was never given an elite weapon quietly serves up its primary
/// instead, so a veteran object may ask for its elite armament without checking first.
/// </summary>
/// <param name="which">The weapon slot desired.</param>
/// <returns>Returns with a pointer to the weapon data for that slot.</returns>
WeaponDataStruct const * TechnoTypeClass::Get_Weapon(int which) const
{
	return &Weapons[which]; // RA2: RA2's form
}



/// <summary>
/// RA2: the elite weapon of the slot specified.
/// </summary>
WeaponDataStruct const * TechnoTypeClass::Get_Elite_Weapon(int which) const // RA2: at 0x6DF5E0
{
	return(&EliteWeapons[which]);
}


/// <summary>
/// Fetches the altitude that this object type travels at.
/// An object type that does not name a flight level of its own falls back on the global
/// rule, so most flying objects share one cruising height.
/// </summary>
/// <returns>Returns with the height above ground that this object type should fly at.</returns>
int TechnoTypeClass::Flight_Level(void) const
{
	if (FlightLevel == -1) {
		return(Rule->FlightLevel);
	}
	return(FlightLevel);
}


/// <summary>
/// Builds the colour schemes of the palette file that Palette= names, when it names one.
/// </summary>
void TechnoTypeClass::Load_Palette(void)
{
	if (PaletteFile[0] != '\0') {
		Palette = Generate_Palette(PaletteFile);
	}
}


/// <summary>
/// Builds the palette schemes of every techno type.
/// </summary>
void __fastcall Load_Techno_Palettes(void)
{
	for (int index = 0; index < TechnoTypes.Count(); index++) {
		TechnoTypes[index]->Load_Palette();
	}
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
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
bool TechnoTypeClass::In_Range(Coord const & coord, AbstractClass * target, WeaponTypeClass * weapon) const
{
//	//assert(IsActive);

	bool is_within_ground_firing_angle;

	if (target != NULL && weapon != NULL) {
		int range = weapon->Range - CELL_LEPTON / 3;
		Coord tcoord = target->Center_Coord();
		int minrange = weapon->MinimumRange;

		if (minrange && (coord - tcoord).Length() < minrange) {
			return(false);
		}

		if (weapon->Bullet->IsArcing) {
			int dist = (Point2D(coord) - Point2D(tcoord)).Length();
			int height = tcoord.Z - coord.Z;
			double gravity = weapon->Bullet->IsFloater ? Get_Floater_Gravity() : Rule->Gravity;
			if (!Is_Projectile_Trajectory_Valid(weapon->MaxSpeed, dist, height, gravity) || (Map[tcoord].IsUnderBridge && tcoord.Z - coord.Z >= 3 * LEVEL_LEPTON_H)) {
				return(false);
			}
			Coord dist2d = Coord(coord.X - tcoord.X, coord.Y - tcoord.Y, 0);
			is_within_ground_firing_angle = coord.Z - tcoord.Z < dist2d.Length();
		} else {

			BuildingClass const * building = target->As_BuildingClass();
			if (building != NULL) {
				range += ((building->Class->Width() + building->Class->Height()) * (CELL_LEPTON_W / 4));
			}

			int cdist;
			if (RTTI != RTTI_AIRCRAFTTYPE) {
				cdist = (coord - tcoord).Length();
			} else {
				cdist = (Coord(Point2D(coord) - Point2D(tcoord), 0)).Length();
			}

			if (cdist > range) {
				return(false);
			}

			if (Map[coord].IsUnderBridge) {
				int height = BRIDGE_LEPTON_HEIGHT + Map.Get_Height_GL(coord);
				if (coord.Z < height && tcoord.Z >= height) {
					return(false);
				}
			}

			/*
			 * A ground weapon cannot hit a target that is too high above it -- the
			 * target must be no higher than the horizontal distance to it.
			 */
			Coord coord1 = coord;
			Coord coord2 = tcoord;
			if (!weapon->Bullet->IsAntiAircraft) {
				Point2D dist2d = Point2D(coord1 - coord2);
				if (tcoord.Z - coord.Z >= dist2d.Length()) {
					return(false);
				}
			}

			Point2D dist2d = Point2D(coord1 - coord2);
			is_within_ground_firing_angle = coord.Z - tcoord.Z < dist2d.Length();
		}

		if (!is_within_ground_firing_angle) {
			int hoffset = Map[coord].IsUnderBridge ? BRIDGE_LEPTON_HEIGHT : 0;
			if (coord.Z <= hoffset + Map.Get_Height_GL(coord)) {
				return(false);
			}
		}
		return(true);
	}

	return(false);
}
#endif


/// <summary>
/// Sets one of this object type's weapon slots.
/// </summary>
/// <param name="which">The weapon slot to fill in.</param>
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
void TechnoTypeClass::Set_Weapon(WeaponDataStruct const & weapon, int which)
{
	Weapons[which] = weapon;
}
#endif


bool TechnoTypeClass::Has_Turret_Weapons(void) const
{
	return TurretCount > 0;
}

int TechnoTypeClass::Get_Turret_Weapon(int which) const
{
	return TurretWeapon[which];
}

// RA2: converts screen-relative damage smoke to a world offset and height.
Coord TechnoTypeClass::Damage_Smoke_Offset(void) const
{
	if (DamSmkOffScrnRel) {
		Point2D screen = Point2D(DamageSmokeOffset.X, DamageSmokeOffset.Y);
		Coord offset = Coord(TacticalMap->Pixel_To_Lepton(screen), 0);
		int height = DamageSmokeOffset.Z * 10;
		offset.X -= height;
		offset.Y -= height;
		double side = height;
		int lift = (int)(fastmath::sqrt(side * side + side * side) * 0.58);
		if (DamageSmokeOffset.Z < 0) {
			offset.Z = lift;
		} else {
			offset.Z -= lift;
		}
		return offset;
	}
	return DamageSmokeOffset;
}

// RA2: choose the weapon used by one turret.
void TechnoTypeClass::Set_Turret_Weapon(int weapon, int which)
{
	TurretWeapon[which] = weapon;
}
