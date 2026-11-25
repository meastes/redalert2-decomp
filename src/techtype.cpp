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
#ifdef RA2_NON_MATCHING // not exact: 15 bytes differ (the two pushes and the lea of Prerequisite's vector constructor are scheduled one member store early among the +0x538..+0x544 stores); exact when exactly one scalar member before LeadershipRating is a class with an inline one-argument constructor (any of them, a probe; two overshoot), which no reference header has; main had `Leptons GuardRange`
TechnoTypeClass::TechnoTypeClass(char const * ininame, SpeedType speed) :
	BASECLASS(ininame),
	WalkRate(1),
	IdleRate(0),
	SpecialThreatValue(0.0),
	MyEffectivenessCoefficient(0.0),
	TargetEffectivenessCoefficient(0.0),
	TargetSpecialThreatCoefficient(0.0),
	TargetStrengthCoefficient(0.0),
	TargetDistanceCoefficient(0.0),
	ThreatAvoidanceCoefficient(0.0),
	SlowdownDistance(500),
	DeaccelerationFactor(0.002),
	AccelerationFactor(0.03),
	CloakingSpeed(7),
	Locomotor(CLSID_TeleportLocomotion),
	VoxelCenterY(0.0),
	VoxelCenterX(0.0),
	Weight(1.0),
	PhysicalSize(2.0),
	Size(1.0),
	SizeLimit(0.0),
	HoverAttack(false),
	VHPScan(0),
	InitialMission(MISSION_HUNT),
	RollAngle(0.5235987755982988),
	PitchSpeed(0.25),
	PitchAngle(0.3490658503988659),
	BuildLimit(0x7FFFFFFF),
	Category(CATEGORY_NONE),
	Unused2(0),
	DeployTime(0.0),
	FireAngle(8),
	PipScale(PIPSCALE_NONE),
	PipWrap(0),
	DeploysInto(0),
	UndeploysInto(0),
	AuxSound1(VOC_NONE),
	AuxSound2(VOC_NONE),
	CreateSound(-1),
	DamageSound(-1),
	ImpactWaterSound(-1),
	ImpactLandSound(-1),
	CrashingSound(-1),
	SinkingSound(-1),
	VoiceFalling(-1),
	VoiceCrashing(-1),
	VoiceSinking(-1),
	VoiceEnter(-1),
	VoiceCapture(-1),
	TurretRotateSound(-1),
	EnterTransportSound(-1),
	LeaveTransportSound(-1),
	DeploySound(-1),
	UndeploySound(-1),
	ChronoInSound(-1),
	ChronoOutSound(-1),
	MZone(MZONE_NORMAL),
	ThreatRange(0),
	MaxDebris(0),
	MinDebris(0),
	MaxPassengers(0),
	SightRange(0),
	RevealToAll(false),
	SensorsSight(0),
	DetectDisguiseRange(0),
	BombSight(0),
	LeadershipRating(5),
	NavalTargeting(0),
	LandTargeting(0),
	Cost(0),
	Soylent(0),
	FlightLevel(-1),
	Level(255),
	Risk(0),
	Reward(0),
	MaxSpeed(MPH_IMMOBILE),
	Speed(speed),
	InitialAmmo(-1),
	MaxAmmo(-1),
	IFVMode(0),
	AirRangeBonus(0),
	Reload(0),
	EmptyReload(-1),
	ReloadIncrement(0),
	RadialFireSegments(0),
	DeployFireWeapon(1),
	DeployFire(false),
	MobileFire(true),
	OpportunityFire(false),
	DistributedFire(false),
	DamageReducesReadiness(false),
	ReadinessReductionMultiplier(0.0f),
	UnloadingClass(0),
	AttackFriendlies(false),
	AttackCursorOnFriendlies(false),
	UndeployDelay(-1),
	PreventAttackMove(false),
	Ownable(0),
	AIBasePlanningSide(-1),
	StupidHunt(false),
	IsAllowedToStartInMultiplayer(true),
	CameoFilename(""),
	CameoData(0),
	CameoAllocated(false),
	AltCameoFilename(""),
	AltCameoData(0),
	AltCameoAllocated(false),
	Rotation(0),
	ROT(0),
	TurretOffset(0),
	Points(0),
	NaturalParticleSystem(0),
	NaturalParticleLocation(0, 0, 0),
	DamageSmokeOffset(0, 0, 0),
	DamSmkOffScrnRel(false),
	DestroySmokeOffset(0, 0, 0),
	RefinerySmokeOffsetOne(0, 0, 0),
	RefinerySmokeOffsetTwo(0, 0, 0),
	ShadowIndex(0),
	Capacity(0),
	TurretNotExportedOnGround(false),
	Gunner(false),
	HasTurretTooltips(false),
	TurretCount(0),
	IsChargeTurret(false),
	ClearAllWeapons(false),
	IsTypeImmune(false),
	IsMoveToShroud(true),
	IsTrainable(true),
	IsDamageSparks(false),
	IsTargetLaser(false),
	IsImmuneToVeins(false),
	IsTiberiumHeal(false),
	IsCloakStop(false),
	IsTrain(false),
	IsDropship(false),
	IsToProtect(false),
	IsDisableable(true),
	IsUnbuildable(false),
	IsDoubleOwned(false),
	IsInvisible(false),
	IsRadarVisible(false),
	IsLeader(false),
	IsScanner(false),
	IsNominal(false),
	IsTurretEquipped(false),
	TurretRecoil(true),
	TurretAnimData(2, 1, 1, 1), // RA2: +0xAB0 is built with travel 2, compress 1, recover 1, hold 1 (the same values as the NoInit constructor)
	BarrelAnimData(2, 1, 1, 1), // RA2: +0xAC4 as above
	IsRepairable(true),
	IsCrew(false),
	Naval(false),
	IsRemappable(false),
	IsCloakable(false),
	GapGenerator(false),
	GapRadiusInCells(0),
	SuperGapRadiusInCells(0),
	Teleporter(false),
	IsSelfHealing(false),
	IsExploding(false),
	DeathWeapon(0),
	DeathWeaponDamageModifier(1.0f),
	IsNoAutoFire(false),
	TurretSpins(false),
	Normalized(false),
	IsManualReload(false),
	IsVisibleLoad(false),
	IsLightningRod(false),
	IsHunterSeeker(false),
	IsCrusher(false),
	IsTiltsWhenCrushes(true),
	IsSubterranean(false),
	IsAutoCrush(false),
	CanDisguise(false),
	PermaDisguise(false),
	DetectDisguise(false),
	DisguiseWhenStill(false),
	CanApproachTarget(true),
	CanRecalcApproachTarget(true),
	ImmuneToRadiation(false),
	DefaultToGuardArea(false),
	Warpable(true),
	ReselectIfLimboed(false),
	RejoinTeamIfLimboed(false),
	Spawned(false),
	Spawns(0),
	SpawnsNumber(0),
	SpawnRegenRate(0),
	SpawnReloadRate(0),
	MissileSpawn(false),
	Underwater(false),
	BalloonHover(false),
	SuppressionThreshold(0),
	JumpjetTurnRate(4),
	JumpjetSpeed(14),
	JumpjetClimb(5.0f),
	JumpjetCrash(5.0f),
	JumpjetHeight(500),
	JumpjetAccel(2.0f),
	JumpjetWobbles(0.15f),
	JumpjetNoWobbles(false),
	JumpjetDeviation(40),
	JumpJet(false),
	Crashable(false),
	NoShadow(false),
	CanPassiveAquire(true),
	CanRetaliate(true),
	RequiresStolenSovietTech(false),
	RequiresStolenAlliedTech(false),
	RequiredHouses(-1),
	ForbiddenHouses(-1),
	IsSelectableCombatant(false),
	IsAccelerates(true),
	DisableVoxelCache(false),
	DisableShadowCache(false),
	ZFudgeCliff(10),
	ZFudgeColumn(5),
	ZFudgeTunnel(10),
	ZFudgeBridge(0),
	Palette(0)
{
	PaletteFile[0] = '\0';
	IsSentient = true;
	DebrisTypes.Clear();
	DebrisAnims.Clear();
	DebrisMaximums.Clear();
	Dock.Clear();
	int index;
	for (index = 0; index < 15; index++) {
		Weapons[index].Weapon = 0;
		Weapons[index].BarrelLength = 0;
		Weapons[index].BarrelThickness = 0;
		Weapons[index].FireFLH = TPoint3D<int>(0, 0, 0);
		Weapons[index].TurretLocked = false;
	}
	for (index = 0; index < 15; index++) {
		EliteWeapons[index].Weapon = 0;
		EliteWeapons[index].BarrelLength = 0;
		EliteWeapons[index].BarrelThickness = 0;
		EliteWeapons[index].FireFLH = TPoint3D<int>(0, 0, 0);
		EliteWeapons[index].TurretLocked = false;
	}
	for (index = 0; index < 15; index++) {
		TurretWeapon[index] = -1;
	}
	AbstractTypePtrTracker.Add(this);
	TechnoTypes.Add(this);
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
static inline void Free_Cameo(TechnoTypeClass * type)
{
	if (type->CameoAllocated && type->CameoData) {
		delete (char *)type->CameoData;
		type->CameoData = NULL;
		type->CameoAllocated = false;
	}
}

// RA2: the same for the alternate cameo (meastes' second inline helper)
static inline void Free_Alt_Cameo(TechnoTypeClass * type)
{
	if (type->AltCameoAllocated && type->AltCameoData) {
		delete (char *)type->AltCameoData;
		type->AltCameoData = NULL;
		type->AltCameoAllocated = false;
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
	return true;
}


/// <summary>
/// RA2: whether an object of this type may attack-move: the primary weapon must exist and PreventAttackMove must be clear.
/// </summary>
bool TechnoTypeClass::CanAttackMove(void) const // RA2: at 0x6DACC0
{
	if (Weapons[0].Weapon && !PreventAttackMove) {
		return true;
	}
	return false;
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
	if (house) {
		float factor = house->Cost_Multiplier(this);
		return (int)(Raw_Cost() * factor);
	}
	return Raw_Cost();
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
	if (PlayerPtr && PlayerPtr->BarracksInfiltrated && Fetch_RTTI() == RTTI_INFANTRYTYPE && AltCameoData && IsTrainable) {
		return AltCameoData;
	}
	if (PlayerPtr && PlayerPtr->WarFactoryInfiltrated && Fetch_RTTI() == RTTI_UNITTYPE && AltCameoData && IsTrainable) {
		return AltCameoData;
	}
	return CameoData;
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
		WeaponTypeClass * sec = Weapons[1].Weapon;

		if (pri == sec || pri->Burst > 1) {
			return(true);
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


// RA2: the recoil accessors Read_INI reads the turret and barrel timing through; a frame count is at least one (ra2-decomp's TurretControl members, src/techtype.cpp; Yuri's Revenge keeps them out of line at 0x717A30-0x717AD0, RA2 expands every one)
inline void TurretControl::Set_Travel(int travel)
{
	Travel = travel;
}

inline int TurretControl::Get_Travel(void) const
{
	return(Travel);
}

inline void TurretControl::Set_Compress_Frames(int frames)
{
	CompressFrames = (frames > 0) ? frames : 1;
}

inline int TurretControl::Get_Compress_Frames(void) const
{
	return(CompressFrames);
}

inline void TurretControl::Set_Recover_Frames(int frames)
{
	RecoverFrames = (frames > 0) ? frames : 1;
}

inline int TurretControl::Get_Recover_Frames(void) const
{
	return(RecoverFrames);
}

inline void TurretControl::Set_Hold_Frames(int frames)
{
	HoldFrames = (frames > 0) ? frames : 1;
}

inline int TurretControl::Get_Hold_Frames(void) const
{
	return(HoldFrames);
}


// RA2: a sound read by name, as the index of its VocClass; the current value stays when the entry is missing or names no sound (ra2-decomp's unit-local helper, src/techtype.cpp)
static inline int Read_Sound(CCINIClass const & ini, char const * section, char const * key, int fallback)
{
	char sound[128];
	if (ini.Get_String(section, key, "", sound, sizeof(sound)) != 0) {
		int index = VocClass::From_Name(sound);
		if (index != VOC_NONE)
			return index;
	}
	return fallback;
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
bool TechnoTypeClass::Read_INI(CCINIClass const & ini) // RA2: RA2's rules and art reading, meastes' body at 0x6DAF60
{
	if (BASECLASS::Read_INI(ini)) {
		LandTargeting = ini.Get_Int(IniName, "LandTargeting", LandTargeting);
		NavalTargeting = ini.Get_Int(IniName, "NavalTargeting", NavalTargeting);
		Speed = ini.Get_SpeedType(IniName, "SpeedType", (::SpeedType)Speed);
		IsTypeImmune = ini.Get_Bool(IniName, "TypeImmune", IsTypeImmune);
		WalkRate = ini.Get_Int(IniName, "WalkRate", WalkRate);
		IdleRate = ini.Get_Int(IniName, "IdleRate", IdleRate);
		IsMoveToShroud = ini.Get_Bool(IniName, "MoveToShroud", IsMoveToShroud);
		IsTrain = ini.Get_Bool(IniName, "IsTrain", IsTrain);
		IsDoubleOwned = ini.Get_Bool(IniName, "DoubleOwned", IsDoubleOwned);
		ThreatRange = ini.Get_Lepton(IniName, "GuardRange", ThreatRange);
		IsExploding = ini.Get_Bool(IniName, "Explodes", IsExploding);
		DeathWeapon = TGet_Class(ini, IniName, "DeathWeapon", DeathWeapon);
		DeathWeaponDamageModifier = ini.Get_Float(IniName, "DeathWeaponDamageModifier", DeathWeaponDamageModifier);
		FlightLevel = ini.Get_Int(IniName, "FlightLevel", FlightLevel);
		IsDropship = ini.Get_Bool(IniName, "IsDropship", IsDropship);
		double pitch = ini.Get_Float(IniName, "PitchAngle", -1.0);
		if (pitch != -1.0) {
			PitchAngle = pitch * (3.141592653589793 / 180.0);
		}
		double roll = ini.Get_Float(IniName, "RollAngle", -1.0);
		if (roll != -1.0) {
			RollAngle = roll * (3.141592653589793 / 180.0);
		}
		PitchSpeed = ini.Get_Float(IniName, "PitchSpeed", PitchSpeed);
		Locomotor = ini.Get_CLSID(IniName, "Locomotor", Locomotor);
		CloakingSpeed = ini.Get_Int(IniName, "CloakingSpeed", CloakingSpeed);
		ThreatAvoidanceCoefficient = ini.Get_Float(IniName, "ThreatAvoidanceCoefficient", ThreatAvoidanceCoefficient);
		SlowdownDistance = ini.Get_Int(IniName, "SlowdownDistance", SlowdownDistance);
		DeaccelerationFactor = ini.Get_Float(IniName, "DeaccelerationFactor", DeaccelerationFactor);
		AccelerationFactor = ini.Get_Float(IniName, "AccelerationFactor", AccelerationFactor);
		Weight = ini.Get_Float(IniName, "Weight", Weight);
		PhysicalSize = ini.Get_Float(IniName, "PhysicalSize", PhysicalSize);
		Size = ini.Get_Float(IniName, "Size", Size);
		SizeLimit = ini.Get_Float(IniName, "SizeLimit", SizeLimit);
		HoverAttack = ini.Get_Bool(IniName, "HoverAttack", HoverAttack);
		VHPScan = ini.Get_VHPScan(IniName, "VHPScan", VHPScan);
		MaxDebris = ini.Get_Int(IniName, "MaxDebris", MaxDebris);
		MinDebris = ini.Get_Int(IniName, "MinDebris", MinDebris);
		if (MinDebris < 0) {
			MinDebris = 0;
		}
		if (MaxDebris < MinDebris) {
			MaxDebris = MinDebris;
		}
		DebrisTypes = TGet_TypeList<VoxelAnimTypeClass>(ini, IniName, "DebrisTypes", DebrisTypes);
		DebrisMaximums = ini.Get_IntList(IniName, "DebrisMaximums", DebrisMaximums);
		DebrisAnims = TGet_TypeList<AnimTypeClass>(ini, IniName, "DebrisAnims", DebrisAnims);
		HasTurretTooltips = ini.Get_Bool(IniName, "HasTurretTooltips", HasTurretTooltips);
		TurretCount = ini.Get_Int(IniName, "TurretCount", TurretCount);
		WeaponCount = ini.Get_Int(IniName, "WeaponCount", WeaponCount);
		IsChargeTurret = ini.Get_Bool(IniName, "IsChargeTurret", IsChargeTurret);
		ClearAllWeapons = ini.Get_Bool(IniName, "ClearAllWeapons", ClearAllWeapons); // RA2: weapon slots are read per turret (WeaponN, EliteWeaponN) or as Primary/Secondary
		if (TurretCount > 0) {
			for (int index = 0; index < WeaponCount; index++) {
				char weapon[12];
				char elite[16];
				sprintf(weapon, "Weapon%d", index + 1);
				sprintf(elite, "EliteWeapon%d", index + 1);
				Weapons[index].Weapon = TGet_Class(ini, IniName, weapon, Weapons[index].Weapon);
				EliteWeapons[index].Weapon = TGet_Class(ini, IniName, elite, EliteWeapons[index].Weapon);
			}
		} else if (!ClearAllWeapons) { // RA2: weapon slots are read per turret (WeaponN, EliteWeaponN) or as Primary/Secondary
			Weapons[0].Weapon = TGet_Class(ini, IniName, "Primary", Weapons[0].Weapon);
			Weapons[1].Weapon = TGet_Class(ini, IniName, "Secondary", Weapons[1].Weapon);
			EliteWeapons[0].Weapon = TGet_Class(ini, IniName, "ElitePrimary", EliteWeapons[0].Weapon);
			EliteWeapons[1].Weapon = TGet_Class(ini, IniName, "EliteSecondary", EliteWeapons[1].Weapon);
		}
		if (ClearAllWeapons) { // RA2: weapon slots are read per turret (WeaponN, EliteWeaponN) or as Primary/Secondary
			Weapons[0].Weapon = 0;
			Weapons[1].Weapon = 0;
			EliteWeapons[0].Weapon = 0;
			EliteWeapons[1].Weapon = 0;
		}
		VoiceMove = CCINIClass::Get_VocType_List(ini, IniName, "VoiceMove", VoiceMove);
		VoiceSelect = CCINIClass::Get_VocType_List(ini, IniName, "VoiceSelect", VoiceSelect);
		VoiceAttack = CCINIClass::Get_VocType_List(ini, IniName, "VoiceAttack", VoiceAttack);
		VoiceSpecialAttack = CCINIClass::Get_VocType_List(ini, IniName, "VoiceSpecialAttack", VoiceSpecialAttack);
		VoiceDie = CCINIClass::Get_VocType_List(ini, IniName, "VoiceDie", VoiceDie);
		VoiceFeedback = CCINIClass::Get_VocType_List(ini, IniName, "VoiceFeedback", VoiceFeedback);
		AuxSound1 = (VocType)Read_Sound(ini, IniName, "AuxSound1", AuxSound1);
		AuxSound2 = (VocType)Read_Sound(ini, IniName, "AuxSound2", AuxSound2);
		CreateSound = Read_Sound(ini, IniName, "CreateSound", CreateSound);
		DamageSound = Read_Sound(ini, IniName, "DamageSound", DamageSound);
		ImpactWaterSound = Read_Sound(ini, IniName, "ImpactWaterSound", ImpactWaterSound);
		ImpactLandSound = Read_Sound(ini, IniName, "ImpactLandSound", ImpactLandSound);
		CrashingSound = Read_Sound(ini, IniName, "CrashingSound", CrashingSound);
		SinkingSound = Read_Sound(ini, IniName, "SinkingSound", SinkingSound);
		VoiceFalling = Read_Sound(ini, IniName, "VoiceFalling", VoiceFalling);
		VoiceCrashing = Read_Sound(ini, IniName, "VoiceCrashing", VoiceCrashing);
		VoiceSinking = Read_Sound(ini, IniName, "VoiceSinking", VoiceSinking);
		VoiceEnter = Read_Sound(ini, IniName, "VoiceEnter", VoiceEnter);
		VoiceCapture = Read_Sound(ini, IniName, "VoiceCapture", VoiceCapture);
		IsCloakStop = ini.Get_Bool(IniName, "CloakStop", IsCloakStop);
		Capacity = ini.Get_Int(IniName, "Storage", Capacity);
		BuildLimit = ini.Get_Int(IniName, "BuildLimit", BuildLimit);
		Category = ini.Get_CategoryType(IniName, "Category", (CategoryType)Category);
		Dock = TGet_TypeList<BuildingTypeClass>(ini, IniName, "Dock", Dock);
		DeploysInto = TGet_Class(ini, IniName, "DeploysInto", DeploysInto);
		UndeploysInto = TGet_Class(ini, IniName, "UndeploysInto", UndeploysInto);
		IsLightningRod = ini.Get_Bool(IniName, "LightningRod", IsLightningRod);
		IsManualReload = ini.Get_Bool(IniName, "ManualReload", IsManualReload);
		TurretSpins = ini.Get_Bool(IniName, "TurretSpins", TurretSpins);
		IsTurretEquipped = ini.Get_Bool(IniName, "Turret", IsTurretEquipped);
		TurretRotateSound = Read_Sound(ini, IniName, "TurretRotateSound", TurretRotateSound);
		EnterTransportSound = Read_Sound(ini, IniName, "EnterTransportSound", EnterTransportSound);
		LeaveTransportSound = Read_Sound(ini, IniName, "LeaveTransportSound", LeaveTransportSound);
		MoveSound = CCINIClass::Get_VocType_List(ini, IniName, "MoveSound", MoveSound);
		DieSound = CCINIClass::Get_VocType_List(ini, IniName, "DieSound", DieSound);
		DeploySound = Read_Sound(ini, IniName, "DeploySound", DeploySound);
		UndeploySound = Read_Sound(ini, IniName, "UndeploySound", UndeploySound);
		ChronoInSound = Read_Sound(ini, IniName, "ChronoInSound", ChronoInSound);
		ChronoOutSound = Read_Sound(ini, IniName, "ChronoOutSound", ChronoOutSound);
		Explosion = TGet_TypeList<AnimTypeClass>(ini, IniName, "Explosion", Explosion);
		DestroyAnim = TGet_TypeList<AnimTypeClass>(ini, IniName, "DestroyAnim", DestroyAnim);
		NaturalParticleSystem = TGet_Class(ini, IniName, "NaturalParticleSystem", NaturalParticleSystem);
		NaturalParticleLocation = ini.Get_Offset(IniName, "NaturalParticleLocation", NaturalParticleLocation);
		DamageParticleSystems = TGet_TypeList<ParticleSystemTypeClass>(ini, IniName, "DamageParticleSystems", DamageParticleSystems);
		DestroyParticleSystems = TGet_TypeList<ParticleSystemTypeClass>(ini, IniName, "DestroyParticleSystems", DestroyParticleSystems);
		DamageSmokeOffset = ini.Get_Offset(IniName, "DamageSmokeOffset", DamageSmokeOffset);
		DamSmkOffScrnRel = ini.Get_Bool(IniName, "DamSmkOffScrnRel", DamSmkOffScrnRel);
		DestroySmokeOffset = ini.Get_Offset(IniName, "DestroySmokeOffset", DestroySmokeOffset);
		RefinerySmokeOffsetOne = ini.Get_Offset(IniName, "RefnSmokeOffsetOne", RefinerySmokeOffsetOne);
		RefinerySmokeOffsetTwo = ini.Get_Offset(IniName, "RefnSmokeOffsetTwo", RefinerySmokeOffsetTwo);
		IsNominal = ini.Get_Bool(IniName, "Nominal", IsNominal);
		IsCloakable = ini.Get_Bool(IniName, "Cloakable", IsCloakable);
		GapGenerator = ini.Get_Bool(IniName, "GapGenerator", GapGenerator);
		GapRadiusInCells = ini.Get_Int(IniName, "GapRadiusInCells", GapRadiusInCells);
		SuperGapRadiusInCells = ini.Get_Int(IniName, "SuperGapRadiusInCells", SuperGapRadiusInCells);
		Teleporter = ini.Get_Bool(IniName, "Teleporter", Teleporter);
		IsScanner = ini.Get_Bool(IniName, "Sensors", IsScanner);
		PipScale = ini.Get_PipScaleType(IniName, "PipScale", PipScale);
		PipWrap = ini.Get_Int(IniName, "PipWrap", PipWrap);
		Prerequisite = CCINIClass::Get_BuildingType_List(ini, IniName, "Prerequisite", Prerequisite);
		PrerequisiteOverride = CCINIClass::Get_BuildingType_List(ini, IniName, "PrerequisiteOverride", PrerequisiteOverride);
		SightRange = ini.Get_Int(IniName, "Sight", SightRange);
		ReselectIfLimboed = ini.Get_Bool(IniName, "ReselectIfLimboed", ReselectIfLimboed);
		RejoinTeamIfLimboed = ini.Get_Bool(IniName, "RejoinTeamIfLimboed", RejoinTeamIfLimboed);
		SensorsSight = ini.Get_Int(IniName, "SensorsSight", SensorsSight);
		DetectDisguiseRange = ini.Get_Int(IniName, "DetectDisguiseRange", DetectDisguiseRange);
		BombSight = ini.Get_Int(IniName, "BombSight", BombSight);
		LeadershipRating = ini.Get_Int(IniName, "LeadershipRating", LeadershipRating);
		RevealToAll = ini.Get_Bool(IniName, "RevealToAll", RevealToAll);
		CanDisguise = ini.Get_Bool(IniName, "CanDisguise", CanDisguise);
		PermaDisguise = ini.Get_Bool(IniName, "PermaDisguise", PermaDisguise);
		DetectDisguise = ini.Get_Bool(IniName, "DetectDisguise", DetectDisguise);
		DisguiseWhenStill = ini.Get_Bool(IniName, "DisguiseWhenStill", DisguiseWhenStill);
		CanPassiveAquire = ini.Get_Bool(IniName, "CanPassiveAquire", CanPassiveAquire);
		CanRetaliate = ini.Get_Bool(IniName, "CanRetaliate", CanRetaliate);
		CanApproachTarget = ini.Get_Bool(IniName, "CanApproachTarget", CanApproachTarget);
		CanRecalcApproachTarget = ini.Get_Bool(IniName, "CanRecalcApproachTarget", CanRecalcApproachTarget);
		RequiresStolenSovietTech = ini.Get_Bool(IniName, "RequiresStolenSovietTech", RequiresStolenSovietTech);
		RequiresStolenAlliedTech = ini.Get_Bool(IniName, "RequiresStolenAlliedTech", RequiresStolenAlliedTech);
		RequiredHouses = ini.Get_Owners(IniName, "RequiredHouses", RequiredHouses);
		ForbiddenHouses = ini.Get_Owners(IniName, "ForbiddenHouses", ForbiddenHouses);
		Level = ini.Get_Int(IniName, "TechLevel", Level);
		int maxspeed = ini.Get_Int(IniName, "Speed", -1);
		if (maxspeed != -1) {
			MaxSpeed = MPHType(_Scale_To_256(maxspeed)); // RA2: Speed= scales 0..100 to 0..255 into MaxSpeed (+0x5A0)
		}
		Cost = ini.Get_Int(IniName, "Cost", Cost);
		Soylent = ini.Get_Int(IniName, "Soylent", Soylent);
		UnloadingClass = TGet_Class(ini, IniName, "UnloadingClass", UnloadingClass);
		InitialAmmo = ini.Get_Int(IniName, "InitialAmmo", InitialAmmo);
		MaxAmmo = ini.Get_Int(IniName, "Ammo", MaxAmmo);
		IFVMode = ini.Get_Int(IniName, "IFVMode", IFVMode);
		AirRangeBonus = ini.Get_Lepton(IniName, "AirRangeBonus", AirRangeBonus);
		RadialFireSegments = ini.Get_Int(IniName, "RadialFireSegments", RadialFireSegments);
		DeployFireWeapon = ini.Get_Int(IniName, "DeployFireWeapon", DeployFireWeapon);
		DeployFire = ini.Get_Bool(IniName, "DeployFire", DeployFire);
		MobileFire = ini.Get_Bool(IniName, "MobileFire", MobileFire);
		OpportunityFire = ini.Get_Bool(IniName, "OpportunityFire", OpportunityFire);
		DistributedFire = ini.Get_Bool(IniName, "DistributedFire", DistributedFire);
		Reload = ini.Get_Int(IniName, "Reload", Reload);
		EmptyReload = ini.Get_Int(IniName, "EmptyReload", EmptyReload);
		ReloadIncrement = ini.Get_Int(IniName, "ReloadIncrement", ReloadIncrement);
		DamageReducesReadiness = ini.Get_Bool(IniName, "DamageReducesReadiness", DamageReducesReadiness);
		ReadinessReductionMultiplier = ini.Get_Float(IniName, "ReadinessReductionMultiplier", ReadinessReductionMultiplier);
		PreventAttackMove = ini.Get_Bool(IniName, "PreventAttackMove", PreventAttackMove);
		Points = ini.Get_Int(IniName, "Points", Points);
		Reward = Points;
		Risk = ini.Get_Int(IniName, "ThreatPosed", Risk);
		Ownable = ini.Get_Owners(IniName, "Owner", Ownable);
		AIBasePlanningSide = ini.Get_Int(IniName, "AIBasePlanningSide", AIBasePlanningSide);
		IsTrainable = ini.Get_Bool(IniName, "Trainable", IsTrainable);
		IsCrew = ini.Get_Bool(IniName, "Crewed", IsCrew);
		Gunner = ini.Get_Bool(IniName, "Gunner", Gunner);
		Naval = ini.Get_Bool(IniName, "Naval", Naval);
		IsRepairable = ini.Get_Bool(IniName, "Repairable", IsRepairable);
		IsInvisible = ini.Get_Bool(IniName, "Invisible", IsInvisible);
		IsRadarVisible = ini.Get_Bool(IniName, "RadarVisible", IsRadarVisible);
		IsSelfHealing = ini.Get_Bool(IniName, "SelfHealing", IsSelfHealing);
		IsNoAutoFire = ini.Get_Bool(IniName, "NoAutoFire", IsNoAutoFire);
		ROT = ini.Get_Int(IniName, "ROT", ROT);
		MaxPassengers = ini.Get_Int(IniName, "Passengers", MaxPassengers);
		FireAngle = ini.Get_Int(IniName, "FireAngle", FireAngle);
		DeployTime = ini.Get_Float(IniName, "DeployTime", DeployTime);
		UndeployDelay = ini.Get_Int(IniName, "UndeployDelay", UndeployDelay);
		IsDisableable = ini.Get_Bool(IniName, "Disableable", IsDisableable);
		IsToProtect = ini.Get_Bool(IniName, "ToProtect", IsToProtect);
		IsTiberiumHeal = ini.Get_Bool(IniName, "TiberiumHeal", IsTiberiumHeal);
		IsImmuneToVeins = ini.Get_Bool(IniName, "ImmuneToVeins", IsImmuneToVeins);
		IsAllowedToStartInMultiplayer = ini.Get_Bool(IniName, "AllowedToStartInMultiplayer", IsAllowedToStartInMultiplayer);
		StupidHunt = ini.Get_Bool(IniName, "StupidHunt", StupidHunt);
		IsTargetLaser = ini.Get_Bool(IniName, "TargetLaser", IsTargetLaser);
		IsHunterSeeker = ini.Get_Bool(IniName, "HunterSeeker", IsHunterSeeker);
		IsCrusher = ini.Get_Bool(IniName, "Crusher", IsCrusher);
		IsAutoCrush = ini.Get_Bool(IniName, "AutoCrush", IsAutoCrush);
		ImmuneToRadiation = ini.Get_Bool(IniName, "ImmuneToRadiation", ImmuneToRadiation);
		Spawned = ini.Get_Bool(IniName, "Spawned", Spawned);
		Underwater = ini.Get_Bool(IniName, "Underwater", Underwater);
		BalloonHover = ini.Get_Bool(IniName, "BalloonHover", BalloonHover);
		Spawns = TGet_Class(ini, IniName, "Spawns", Spawns);
		SpawnRegenRate = ini.Get_Int(IniName, "SpawnRegenRate", SpawnRegenRate);
		SpawnsNumber = ini.Get_Int(IniName, "SpawnsNumber", SpawnsNumber);
		SpawnReloadRate = ini.Get_Int(IniName, "SpawnReloadRate", SpawnReloadRate);
		MissileSpawn = ini.Get_Bool(IniName, "MissileSpawn", MissileSpawn);
		DefaultToGuardArea = ini.Get_Bool(IniName, "DefaultToGuardArea", DefaultToGuardArea);
		Warpable = ini.Get_Bool(IniName, "Warpable", Warpable);
		Parasiteable = ini.Get_Bool(IniName, "Parasiteable", Parasiteable);
		ImmuneToPsionics = ini.Get_Bool(IniName, "ImmuneToPsionics", ImmuneToPsionics);
		ConsideredAircraft = ini.Get_Bool(IniName, "ConsideredAircraft", ConsideredAircraft);
		Organic = ini.Get_Bool(IniName, "Organic", Organic);
		SuppressionThreshold = ini.Get_Int(IniName, "SuppressionThreshold", SuppressionThreshold);
		NoShadow = ini.Get_Bool(IniName, "NoShadow", NoShadow);
		JumpjetTurnRate = ini.Get_Int(IniName, "JumpjetTurnRate", JumpjetTurnRate);
		JumpjetSpeed = ini.Get_Int(IniName, "JumpjetSpeed", JumpjetSpeed);
		JumpjetClimb = ini.Get_Float(IniName, "JumpjetClimb", JumpjetClimb);
		JumpjetCrash = ini.Get_Float(IniName, "JumpjetCrash", JumpjetCrash);
		JumpjetHeight = ini.Get_Int(IniName, "JumpjetHeight", JumpjetHeight);
		JumpjetAccel = ini.Get_Float(IniName, "JumpjetAccel", JumpjetAccel);
		JumpjetWobbles = ini.Get_Float(IniName, "JumpjetWobbles", JumpjetWobbles);
		JumpjetNoWobbles = ini.Get_Bool(IniName, "JumpjetNoWobbles", JumpjetNoWobbles);
		JumpjetDeviation = ini.Get_Int(IniName, "JumpjetDeviation", JumpjetDeviation);
		JumpJet = ini.Get_Bool(IniName, "JumpJet", JumpJet);
		Crashable = ini.Get_Bool(IniName, "Crashable", Crashable);
		AttackFriendlies = ini.Get_Bool(IniName, "AttackFriendlies", AttackFriendlies);
		AttackCursorOnFriendlies = ini.Get_Bool(IniName, "AttackCursorOnFriendlies", AttackCursorOnFriendlies);
		TurretRecoil = ini.Get_Bool(IniName, "TurretRecoil", TurretRecoil);
		TurretAnimData.Set_Travel(ini.Get_Int(IniName, "TurretTravel", TurretAnimData.Get_Travel()));
		TurretAnimData.Set_Compress_Frames(ini.Get_Int(IniName, "TurretCompressFrames", TurretAnimData.Get_Compress_Frames()));
		TurretAnimData.Set_Hold_Frames(ini.Get_Int(IniName, "TurretHoldFrames", TurretAnimData.Get_Hold_Frames()));
		TurretAnimData.Set_Recover_Frames(ini.Get_Int(IniName, "TurretRecoverFrames", TurretAnimData.Get_Recover_Frames()));
		BarrelAnimData = TurretAnimData;
		BarrelAnimData.Set_Travel(ini.Get_Int(IniName, "BarrelTravel", TurretAnimData.Get_Travel()));
		BarrelAnimData.Set_Compress_Frames(ini.Get_Int(IniName, "BarrelCompressFrames", TurretAnimData.Get_Compress_Frames()));
		BarrelAnimData.Set_Hold_Frames(ini.Get_Int(IniName, "BarrelHoldFrames", TurretAnimData.Get_Hold_Frames()));
		BarrelAnimData.Set_Recover_Frames(ini.Get_Int(IniName, "BarrelRecoverFrames", TurretAnimData.Get_Recover_Frames()));
		IsTiltsWhenCrushes = ini.Get_Bool(IniName, "TiltsWhenCrushes", IsTiltsWhenCrushes);
		IsAccelerates = ini.Get_Bool(IniName, "Accelerates", IsAccelerates);
		ZFudgeCliff = ini.Get_Int(IniName, "ZFudgeCliff", ZFudgeCliff);
		ZFudgeColumn = ini.Get_Int(IniName, "ZFudgeColumn", ZFudgeColumn);
		ZFudgeTunnel = ini.Get_Int(IniName, "ZFudgeTunnel", ZFudgeTunnel);
		ZFudgeBridge = ini.Get_Int(IniName, "ZFudgeBridge", ZFudgeBridge);
		VeteranAbilities = ini.Get_Abilities(IniName, "VeteranAbilities", VeteranAbilities);
		EliteAbilities = ini.Get_Abilities(IniName, "EliteAbilities", EliteAbilities);
		MyEffectivenessCoefficient = ini.Get_Float(IniName, "MyEffectivenessCoefficient", MyEffectivenessCoefficient == 0.0 ? Rule->MyEffectivenessCoefficientDefault : MyEffectivenessCoefficient);
		TargetEffectivenessCoefficient = ini.Get_Float(IniName, "TargetEffectivenessCoefficient", TargetEffectivenessCoefficient == 0.0 ? Rule->TargetEffectivenessCoefficientDefault : TargetEffectivenessCoefficient);
		TargetSpecialThreatCoefficient = ini.Get_Float(IniName, "TargetSpecialThreatCoefficient", TargetSpecialThreatCoefficient == 0.0 ? Rule->TargetSpecialThreatCoefficientDefault : TargetSpecialThreatCoefficient);
		TargetStrengthCoefficient = ini.Get_Float(IniName, "TargetStrengthCoefficient", TargetStrengthCoefficient == 0.0 ? Rule->TargetStrengthCoefficientDefault : TargetStrengthCoefficient);
		TargetDistanceCoefficient = ini.Get_Float(IniName, "TargetDistanceCoefficient", TargetDistanceCoefficient == 0.0 ? Rule->TargetDistanceCoefficientDefault : TargetDistanceCoefficient);
		SpecialThreatValue = ini.Get_Float(IniName, "SpecialThreatValue", SpecialThreatValue);
		IsSelectableCombatant = ini.Get_Bool(IniName, "IsSelectableCombatant", IsSelectableCombatant);

		IsLeader = false;
		if (Weapons[0].Weapon != 0 && Weapons[0].Weapon->Attack > 0) {
			IsLeader = true;
		}

		char fullname[512];
		_makepath(fullname, 0, 0, GraphicName, ".SHP");
		if (ImageAllocated && ImageData != 0) {
			delete (char *)ImageData;
			ImageData = 0;
			ImageAllocated = false;
		}
		ImageData = (void *)MixFileClass::Retrieve(fullname);

		ArtINI.Get_String(GraphicName, "Palette", "", PaletteFile, sizeof(PaletteFile));
		Load_Palette(); // RA2: Load_Palette 0x006DF620 builds the schemes of Palette= (outside this module)
		if (Palette_Scheme_Count() > 1) { // RA2: 0x00603D50 counts the palettes that have a scheme list
			DebugString("You have violated the limit of having only one extra building palette.\n"); // RA2: the exe aborts when more than one extra building palette is loaded
			Exit_Process(-1); // RA2: exits with -1 through Exit_Process 0x691310 (main: Emergency_Exit(-1))
		}
		TurretOffset = ArtINI.Get_Int(GraphicName, "TurretOffset", TurretOffset);
		Rotation = ArtINI.Get_Int(GraphicName, "RotCount", Rotation);
		IsRemappable = ArtINI.Get_Bool(GraphicName, "Remapable", IsRemappable);
		Normalized = ArtINI.Get_Bool(GraphicName, "Normalized", Normalized);
		IsVisibleLoad = ArtINI.Get_Bool(GraphicName, "VisibleLoad", IsVisibleLoad);
		ShadowIndex = ArtINI.Get_Int(GraphicName, "ShadowIndex", ShadowIndex);
		DisableVoxelCache = ArtINI.Get_Bool(GraphicName, "DisableVoxelCache", DisableVoxelCache);
		DisableShadowCache = ArtINI.Get_Bool(GraphicName, "DisableShadowCache", DisableShadowCache);
		char cameo[32];
		ArtINI.Get_String(GraphicName, "Cameo", "", cameo, sizeof(CameoFilename));
		if (strlen(cameo) != 0) {
			strcpy(CameoFilename, cameo);
			_makepath(fullname, 0, 0, CameoFilename, ".SHP");
			if (CameoData == 0) {
				CameoData = (void *)MixFileClass::Retrieve(fullname);
			}
		}
		if (CameoData == 0) {
			CameoData = (void *)MixFileClass::Retrieve("XXICON.SHP");
		}
		ArtINI.Get_String(GraphicName, "AltCameo", "", cameo, sizeof(AltCameoFilename)); // RA2: the alternate cameo, read the same way as Cameo
		if (strlen(cameo) != 0) {
			strcpy(AltCameoFilename, cameo);
			_makepath(fullname, 0, 0, AltCameoFilename, ".SHP");
			if (AltCameoData == 0) {
				AltCameoData = (void *)MixFileClass::Retrieve(fullname);
			}
		}

		if (Has_Turret_Weapons()) {
			for (int index = 0; index < WeaponCount; index++) {
				char weapon[32];
				char entry[128];
				sprintf(weapon, "Weapon%d", index + 1);
				sprintf(entry, "%sFLH", weapon);
				Weapons[index].FireFLH = ArtINI.Get_Point(GraphicName, entry, Weapons[index].FireFLH);
				sprintf(entry, "%sBarrelLength", weapon);
				Weapons[index].BarrelLength = ArtINI.Get_Int(GraphicName, entry, Weapons[index].BarrelLength);
				sprintf(entry, "%sBarrelThickness", weapon);
				Weapons[index].BarrelThickness = ArtINI.Get_Int(GraphicName, entry, Weapons[index].BarrelThickness);
				sprintf(entry, "%sTurretLocked", weapon);
				Weapons[index].TurretLocked = ArtINI.Get_Bool(GraphicName, entry, Weapons[index].TurretLocked);

				sprintf(weapon, "EliteWeapon%d", index + 1);
				sprintf(entry, "%sFLH", weapon);
				EliteWeapons[index].FireFLH = ArtINI.Get_Point(GraphicName, entry, Weapons[index].FireFLH);
				sprintf(entry, "%sBarrelLength", weapon);
				EliteWeapons[index].BarrelLength = ArtINI.Get_Int(GraphicName, entry, Weapons[index].BarrelLength);
				sprintf(entry, "%sBarrelThickness", weapon);
				EliteWeapons[index].BarrelThickness = ArtINI.Get_Int(GraphicName, entry, Weapons[index].BarrelThickness);
				sprintf(entry, "%sTurretLocked", weapon);
				EliteWeapons[index].TurretLocked = ArtINI.Get_Bool(GraphicName, entry, Weapons[index].TurretLocked);
			}
		} else {
			Weapons[0].FireFLH = ArtINI.Get_Point(GraphicName, "PrimaryFireFLH", Weapons[0].FireFLH);
			Weapons[0].BarrelLength = ArtINI.Get_Int(GraphicName, "PBarrelLength", Weapons[0].BarrelLength);
			Weapons[0].BarrelThickness = ArtINI.Get_Int(GraphicName, "PBarrelThickness", Weapons[0].BarrelThickness);
			Weapons[1].FireFLH = ArtINI.Get_Point(GraphicName, "SecondaryFireFLH", Weapons[1].FireFLH);
			Weapons[1].BarrelLength = ArtINI.Get_Int(GraphicName, "SBarrelLength", Weapons[1].BarrelLength);
			Weapons[1].BarrelThickness = ArtINI.Get_Int(GraphicName, "SBarrelThickness", Weapons[1].BarrelThickness);
			EliteWeapons[0].FireFLH = ArtINI.Get_Point(GraphicName, "ElitePrimaryFireFLH", Weapons[0].FireFLH);
			EliteWeapons[0].BarrelLength = ArtINI.Get_Int(GraphicName, "ElitePBarrelLength", Weapons[0].BarrelLength);
			EliteWeapons[0].BarrelThickness = ArtINI.Get_Int(GraphicName, "ElitePBarrelThickness", Weapons[0].BarrelThickness);
			EliteWeapons[1].FireFLH = ArtINI.Get_Point(GraphicName, "EliteSecondaryFireFLH", Weapons[1].FireFLH);
			EliteWeapons[1].BarrelLength = ArtINI.Get_Int(GraphicName, "EliteSBarrelLength", Weapons[1].BarrelLength);
			EliteWeapons[1].BarrelThickness = ArtINI.Get_Int(GraphicName, "EliteSBarrelThickness", Weapons[1].BarrelThickness);
		}
		TurretNotExportedOnGround = ArtINI.Get_Bool(GraphicName, "TurretNotExportedOnGround", TurretNotExportedOnGround);
		MZone = ini.Get_MZoneType(IniName, "MovementZone", MZone);
		IsSubterranean = MZone == MZONE_SUBTERANNEAN;

		if (IsVoxel) {
			Fetch_Voxel_Image();
		} else if (IsTurretEquipped) {
			Fetch_Aux_Voxel_Image();
		}
		if (Voxel.VoxLib != NULL && !Voxel.VoxLib->Load_Failed()) {
			VoxelCenterY = Voxel.VoxLib->Get_Layer_Info(0, 0).YSize * 0.5f + 0.5;
			VoxelCenterX = Voxel.VoxLib->Get_Layer_Info(0, 0).XSize * 0.5f;
		}

		return(true);
	}
	return(false);
}



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
		case PIPSCALE_POWER:
			return 10;
		case PIPSCALE_AMMO:
			return MaxAmmo;
		case PIPSCALE_TIBERIUM:
			return 5;
		case PIPSCALE_PASSENGERS:
			return MaxPassengers;
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
	VoiceSpecialAttack.Clear();
	VoiceDie.Clear();
	VoiceFeedback.Clear();
	MoveSound.Clear();
	DieSound.Clear();
	Explosion.Clear();
	DestroyAnim.Clear();
	Prerequisite.Clear();
	PrerequisiteOverride.Clear();
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
		VoiceSpecialAttack.Load(stream);
		VoiceDie.Load(stream);
		VoiceFeedback.Load(stream);
		MoveSound.Load(stream);
		DieSound.Load(stream);
		Explosion.Load(stream);
		DestroyAnim.Load(stream);
		DebrisMaximums.Load(stream);
		Prerequisite.Load(stream);
		PrerequisiteOverride.Load(stream);
		DamageParticleSystems.Load(stream);
		DestroyParticleSystems.Load(stream);

		for (i = 0; i < Dock.Count(); i++) {
			Swizzle_Pointer(&Dock[i]);
		}
		for (i = 0; i < Explosion.Count(); i++) {
			Swizzle_Pointer(&Explosion[i]);
		}
		for (i = 0; i < DestroyAnim.Count(); i++) {
			Swizzle_Pointer(&DestroyAnim[i]);
		}
		for (i = 0; i < DebrisTypes.Count(); i++) {
			Swizzle_Pointer(&DebrisTypes[i]);
		}
		for (i = 0; i < DebrisAnims.Count(); i++) {
			Swizzle_Pointer(&DebrisAnims[i]);
		}
		for (i = 0; i < 15; i++) { // RA2: all fifteen slots (WEAPON_SLOT_COUNT is TibSun's 3)
			Swizzle_Pointer(&Weapons[i].Weapon);
		}
		for (i = 0; i < 15; i++) { // RA2: the elite weapons are swizzled too
			Swizzle_Pointer(&EliteWeapons[i].Weapon);
		}
		for (i = 0; i < DamageParticleSystems.Count(); i++) {
			Swizzle_Pointer(&DamageParticleSystems[i]);
		}
		for (i = 0; i < DestroyParticleSystems.Count(); i++) {
			Swizzle_Pointer(&DestroyParticleSystems[i]);
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
			CameoData = (const ShapeSet *)MFCD::Retrieve("XXICON.SHP");
		}

		ArtINI.Get_String((const char *)GraphicName, "AltCameo", "", buffer, sizeof(buffer)); // RA2: the alternate cameo, no fallback
		_makepath(fname, NULL, NULL, buffer, ".SHP");
		AltCameoData = NULL;
		AltCameoData = (const ShapeSet *)MFCD::Retrieve(fname);

		ArtINI.Get_String((const char *)GraphicName, "Palette", "", PaletteFile, sizeof(PaletteFile)); // RA2: the palette named by Palette=
		if (PaletteFile[0] != '\0') {
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
	HRESULT result = ObjectTypeClass::Save(stream, cleardirty);
	if (FAILED(result)) {
		return result;
	}
	Dock.Save(stream);
	DebrisTypes.Save(stream);
	DebrisAnims.Save(stream);
	VoiceSelect.Save(stream);
	VoiceMove.Save(stream);
	VoiceAttack.Save(stream);
	VoiceSpecialAttack.Save(stream);
	VoiceDie.Save(stream);
	VoiceFeedback.Save(stream);
	MoveSound.Save(stream);
	DieSound.Save(stream);
	Explosion.Save(stream);
	DestroyAnim.Save(stream);
	DebrisMaximums.Save(stream);
	Prerequisite.Save(stream);
	PrerequisiteOverride.Save(stream);
	DamageParticleSystems.Save(stream);
	DestroyParticleSystems.Save(stream);
	return S_OK;
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
	HRESULT result = ObjectTypeClass::GetSizeMax(pcbSize);
	if (FAILED(result)) {
		return result;
	}
	pcbSize->LowPart += VoiceSelect.Count() * sizeof(int) + sizeof(int);
	pcbSize->LowPart += VoiceMove.Count() * sizeof(int) + sizeof(int);
	pcbSize->LowPart += VoiceAttack.Count() * sizeof(int) + sizeof(int);
	pcbSize->LowPart += VoiceSpecialAttack.Count() * sizeof(int) + sizeof(int);
	pcbSize->LowPart += VoiceDie.Count() * sizeof(int) + sizeof(int);
	pcbSize->LowPart += VoiceFeedback.Count() * sizeof(int) + sizeof(int);
	pcbSize->LowPart += MoveSound.Count() * sizeof(int) + sizeof(int);
	pcbSize->LowPart += DieSound.Count() * sizeof(int) + sizeof(int);
	pcbSize->LowPart += Explosion.Count() * sizeof(AnimTypeClass *) + sizeof(int);
	pcbSize->LowPart += DestroyAnim.Count() * sizeof(AnimTypeClass *) + sizeof(int);
	pcbSize->LowPart += DebrisTypes.Count() * sizeof(VoxelAnimTypeClass *) + sizeof(int);
	pcbSize->LowPart += DebrisAnims.Count() * sizeof(AnimTypeClass *) + sizeof(int);
	pcbSize->LowPart += Dock.Count() * sizeof(BuildingTypeClass *) + sizeof(int);
	pcbSize->LowPart += DebrisMaximums.Count() * sizeof(int) + sizeof(int);
	pcbSize->LowPart += DamageParticleSystems.Count() * sizeof(ParticleSystemTypeClass *) + sizeof(int);
	pcbSize->LowPart += DestroyParticleSystems.Count() * sizeof(ParticleSystemTypeClass *) + sizeof(int);
	return S_OK;
}



/// <summary>
/// Adds this object type's rule data to a running checksum.
/// This routine is used by the multiplayer sync check to prove that every machine in the
/// game is playing by the same rules.
/// </summary>
void TechnoTypeClass::Compute_CRC(class CRCEngine & crc) const
{
	AbstractTypeClass::Compute_CRC(crc);
	crc(ThreatAvoidanceCoefficient);
	crc((long)SlowdownDistance);
	crc(DeaccelerationFactor);
	crc(AccelerationFactor);
	crc((long)CloakingSpeed);
	crc((long)DebrisTypes.Count());
	crc((long)DebrisAnims.Count());
	crc((long)Dock.Count());
	crc((long)DebrisMaximums.Count());
	crc(&Locomotor, sizeof(Locomotor));
	crc(VoxelCenterY);
	crc(VoxelCenterX);
	crc(Weight);
	crc(PhysicalSize);
	crc(Size);
	crc(SizeLimit);
	crc(HoverAttack);
	crc((long)InitialMission);
	crc(IsImmuneToVeins);
	crc(IsTiberiumHeal);
	crc(IsTargetLaser);
	crc(RollAngle);
	crc(PitchAngle);
	crc(PitchSpeed);
	crc((long)FlightLevel);
	crc((long)BuildLimit);
	crc((long)Category);
	crc(DeployTime);
	crc((long)FireAngle);
	crc((long)PipScale);
	crc((long)VoiceSelect.Count());
	crc((long)VoiceMove.Count());
	crc((long)VoiceAttack.Count());
	crc((long)VoiceSpecialAttack.Count());
	crc((long)VoiceDie.Count());
	crc((long)VoiceFeedback.Count());
	crc((long)MoveSound.Count());
	crc((long)DieSound.Count());
	crc((long)DamageParticleSystems.Count());
	crc((long)DestroyParticleSystems.Count());
	crc((long)MZone);
	crc((long)ThreatRange);
	crc((long)MaxDebris);
	crc((long)MinDebris);
	crc((long)MaxPassengers);
	crc((long)SightRange);
	crc((long)Cost);
	crc((long)Soylent);
	crc((long)Level);
	crc((long)Prerequisite.Count());
	crc((long)PrerequisiteOverride.Count());
	crc((long)Risk);
	crc((long)Reward);
	crc((long)MaxSpeed);
	crc((long)Speed);
	crc((long)MaxAmmo);
	crc((long)AirRangeBonus);
	crc((long)Ownable);
	crc((long)AIBasePlanningSide);
	crc((long)Rotation);
	crc((long)ROT);
	crc((long)TurretOffset);
	crc((long)Points);
	crc((long)Explosion.Count());
	crc((long)DestroyAnim.Count());
	crc((long)ShadowIndex);
	crc((long)Capacity);
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
	crc(GapGenerator);
	crc(GapRadiusInCells);
	crc(SuperGapRadiusInCells);
	crc(Teleporter);
	crc(IsSelfHealing);
	crc(IsExploding);
	crc(IsNoAutoFire);
	crc(TurretSpins);
	crc(Normalized);
	crc(IsManualReload);
	crc(IsVisibleLoad);
	crc(IsLightningRod);
	crc(DisableVoxelCache);
	crc(DisableShadowCache);
	crc(MobileFire);
	crc(OpportunityFire);
	crc(DistributedFire);
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
	return &Weapons[which];
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

