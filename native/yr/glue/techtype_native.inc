
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y3), appended to the techtype unit: TechnoTypeClass::TechnoTypeClass
// (char const *, SpeedType), YR 0x00710AF0 (3,406 bytes, unmatched in ra2-decomp). Written from
// gamemd.exe's code: every store the constructor makes through `this`, mapped to this tree's
// TechnoTypeClass fields by their 32-bit offsets (the header laid out by clang for i686 with the
// MS layout rules), with the values it stores (doubles from their two dwords). The type lists and
// the ability flags are default-constructed as the original's inline constructors do; four lists
// are cleared, the weapon slots, FLHs, stages and turret weapons are set in the original's loops;
// the type joins AbstractTypePtrTracker and TechnoTypeClass::Array as the NoInit constructor
// adds itself (0x00711840). Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------
#include "tracker.h"
#include "ilocos.h"

TechnoTypeClass::TechnoTypeClass(char const * ininame, SpeedType speed) :
	BASECLASS(ininame),
	CameoFilename(DAT_00889F64),
	AltCameoFilename(DAT_00889F64),
	TurretAnimData(2, 1, 1, 1),
	BarrelAnimData(2, 1, 1, 1)
{
	Locomotor = CLSID_TeleportLocomotion;
	IdleRate = 1;
	SpecialThreatValue = 0.0;
	MyEffectivenessCoefficient = 0.0;
	TargetEffectivenessCoefficient = 0.0;
	TargetSpecialThreatCoefficient = 0.0;
	TargetStrengthCoefficient = 0.0;
	TargetDistanceCoefficient = 0.0;
	ThreatAvoidanceCoefficient = 0.0;
	SlowdownDistance = 500;
	DeaccelerationFactor = 0.002;
	AccelerationFactor = 0.03;
	CloakingSpeed = 7;
	VoxelCenterY = 0.0;
	VoxelCenterX = 0.0;
	Weight = 1.0;
	PhysicalSize = 2.0;
	Size = 1.0;
	SizeLimit = 0.0;
	HoverAttack = false;
	VHPScan = 0;
	InitialMission = (MissionType)15;
	RollAngle = 0.5235987755982988;
	PitchSpeed = 0.25;
	PitchAngle = 0.3490658503988659;
	BuildLimit = 0x7fffffff;
	Category = (CategoryType)-1;
	Unused2 = 0;
	DeployTime = 0.0;
	FireAngle = 8;
	PipScale = (PipScaleType)0;
	PipsDrawForAll = false;
	LeptonMindControlOffset = 70;
	PixelSelectionBracketDelta = 0;
	PipWrap = 0;
	DeploysInto = NULL;
	UndeploysInto = NULL;
	PowersUnit = NULL;
	PoweredUnit = false;
	AuxSound1 = (VocType)-1;
	AuxSound2 = (VocType)-1;
	CreateSound = -1;
	DamageSound = -1;
	ImpactWaterSound = -1;
	ImpactLandSound = -1;
	CrashingSound = -1;
	SinkingSound = -1;
	VoiceFalling = -1;
	VoiceCrashing = -1;
	VoiceSinking = -1;
	VoiceEnter = -1;
	VoiceCapture = -1;
	TurretRotateSound = -1;
	EnterTransportSound = -1;
	LeaveTransportSound = -1;
	DeploySound = -1;
	UndeploySound = -1;
	ChronoInSound = -1;
	ChronoOutSound = -1;
	VoiceHarvest = -1;
	VoicePrimaryWeaponAttack = -1;
	VoicePrimaryEliteWeaponAttack = -1;
	VoiceSecondaryWeaponAttack = -1;
	VoiceSecondaryEliteWeaponAttack = -1;
	VoiceDeploy = -1;
	VoiceUndeploy = -1;
	EnterGrinderSound = -1;
	LeaveGrinderSound = -1;
	EnterBioReactorSound = -1;
	LeaveBioReactorSound = -1;
	ActivateSound = -1;
	DeactivateSound = -1;
	MindClearedSound = -1;
	MZone = (MZoneType)0;
	ThreatRange = (LEPTON)0;
	MaxDebris = 0;
	MinDebris = 0;
	MaxPassengers = 0;
	OpenTopped = false;
	SightRange = 0;
	ResourceGatherer = false;
	ResourceDestination = false;
	RevealToAll = false;
	Drainable = false;
	SensorsSight = 0;
	DetectDisguiseRange = 0;
	BombSight = 0;
	LeadershipRating = 5;
	NavalTargeting = 0;
	LandTargeting = 0;
	BuildTimeMultiplier = 1.0f;
	MindControlRingOffset = 140;
	Cost = 0;
	Soylent = 0;
	FlightLevel = -1;
	AirstrikeTeam = 0;
	EliteAirstrikeTeam = 0;
	AirstrikeTeamType = NULL;
	EliteAirstrikeTeamType = NULL;
	AirstrikeRechargeTime = 0;
	EliteAirstrikeRechargeTime = 0;
	Level = 255;
	Risk = 0;
	Reward = 0;
	MaxSpeed = (MPHType)0;
	Speed = (SpeedType)speed;
	InitialAmmo = -1;
	MaxAmmo = -1;
	IFVMode = 0;
	AirRangeBonus = 0;
	BerserkFriendly = false;
	SprayAttack = false;
	Pushy = false;
	Natural = false;
	Unnatural = false;
	CloseRange = false;
	Reload = 0;
	EmptyReload = -1;
	ReloadIncrement = 0;
	RadialFireSegments = 0;
	DeployFireWeapon = 1;
	DeployFire = false;
	DeployToLand = false;
	MobileFire = true;
	OpportunityFire = false;
	DistributedFire = false;
	DamageReducesReadiness = false;
	ReadinessReductionMultiplier = 0.0f;
	UnloadingClass = NULL;
	DeployingAnim = NULL;
	AttackFriendlies = false;
	AttackCursorOnFriendlies = false;
	UndeployDelay = -1;
	PreventAttackMove = false;
	Ownable = 0;
	AIBasePlanningSide = -1;
	StupidHunt = false;
	IsAllowedToStartInMultiplayer = true;
	CameoData = NULL;
	CameoAllocated = false;
	AltCameoData = NULL;
	AltCameoAllocated = false;
	Rotation = 0;
	ROT = 0;
	TurretOffset = (LEPTON)0;
	CanBeHidden = true;
	Points = 0;
	NaturalParticleSystem = NULL;
	NaturalParticleLocation.X = 0;
	NaturalParticleLocation.Y = 0;
	NaturalParticleLocation.Z = 0;
	RefinerySmokeParticleSystem = NULL;
	DamageSmokeOffset.X = 0;
	DamageSmokeOffset.Y = 0;
	DamageSmokeOffset.Z = 0;
	DamSmkOffScrnRel = false;
	DestroySmokeOffset.X = 0;
	DestroySmokeOffset.Y = 0;
	DestroySmokeOffset.Z = 0;
	RefinerySmokeOffsetOne.X = 0;
	RefinerySmokeOffsetOne.Y = 0;
	RefinerySmokeOffsetOne.Z = 0;
	RefinerySmokeOffsetTwo.X = 0;
	RefinerySmokeOffsetTwo.Y = 0;
	RefinerySmokeOffsetTwo.Z = 0;
	RefinerySmokeOffsetThree.X = 0;
	RefinerySmokeOffsetThree.Y = 0;
	RefinerySmokeOffsetThree.Z = 0;
	RefinerySmokeOffsetFour.X = 0;
	RefinerySmokeOffsetFour.Y = 0;
	RefinerySmokeOffsetFour.Z = 0;
	ShadowIndex = 0;
	Capacity = 0;
	TurretNotExportedOnGround = false;
	Gunner = false;
	HasTurretTooltips = false;
	TurretCount = 0;
	IsChargeTurret = false;
	ClearAllWeapons = false;
	IsTypeImmune = false;
	IsMoveToShroud = true;
	IsTrainable = true;
	IsDamageSparks = false;
	IsTargetLaser = false;
	IsImmuneToVeins = false;
	IsTiberiumHeal = false;
	IsCloakStop = false;
	IsTrain = false;
	IsDropship = false;
	IsToProtect = false;
	IsDisableable = true;
	IsUnbuildable = false;
	IsDoubleOwned = false;
	IsInvisible = false;
	IsRadarVisible = false;
	IsLeader = false;
	IsScanner = false;
	IsNominal = false;
	DontScore = false;
	DamageSelf = false;
	IsTurretEquipped = false;
	TurretRecoil = false;
	IsRepairable = true;
	IsCrew = false;
	Naval = false;
	IsRemappable = false;
	IsCloakable = false;
	GapGenerator = false;
	GapRadiusInCells = 0;
	SuperGapRadiusInCells = 0;
	Teleporter = false;
	IsGattling = false;
	WeaponStages = 0;
	RateUp = 0;
	RateDown = 0;
	IsSelfHealing = false;
	IsExploding = false;
	DeathWeapon = NULL;
	DeathWeaponDamageModifier = 1.0f;
	IsNoAutoFire = false;
	TurretSpins = false;
	TiltCrashJumpjet = false;
	Normalized = false;
	IsManualReload = false;
	IsVisibleLoad = false;
	IsLightningRod = false;
	IsHunterSeeker = false;
	IsCrusher = false;
	OmniCrusher = false;
	OmniCrushResistant = false;
	IsTiltsWhenCrushes = true;
	IsSubterranean = false;
	IsAutoCrush = false;
	CanDisguise = false;
	PermaDisguise = false;
	DetectDisguise = false;
	DisguiseWhenStill = false;
	CanApproachTarget = true;
	CanRecalcApproachTarget = true;
	ImmuneToRadiation = false;
	DefaultToGuardArea = false;
	Warpable = true;
	ReselectIfLimboed = false;
	RejoinTeamIfLimboed = false;
	Slaved = false;
	Enslaves = NULL;
	SlavesNumber = 0;
	SlaveRegenRate = 0;
	SlaveReloadRate = 0;
	OpenTransportWeapon = -1;
	Spawned = false;
	Spawns = NULL;
	SpawnsNumber = 0;
	SpawnRegenRate = 0;
	SpawnReloadRate = 0;
	MissileSpawn = false;
	Underwater = false;
	BalloonHover = false;
	SuppressionThreshold = 0;
	JumpjetTurnRate = 4;
	JumpjetSpeed = 14;
	JumpjetClimb = 5.0f;
	JumpjetCrash = 5.0f;
	JumpjetHeight = 500;
	JumpjetAccel = 2.0f;
	JumpjetWobbles = 0.15f;
	JumpjetNoWobbles = false;
	JumpjetDeviation = 40;
	JumpJet = false;
	Crashable = false;
	NoShadow = false;
	CanPassiveAquire = true;
	CanRetaliate = true;
	RequiresStolenThirdTech = false;
	RequiresStolenSovietTech = false;
	RequiresStolenAlliedTech = false;
	RequiredHouses = (DWORD)-1;
	ForbiddenHouses = (DWORD)-1;
	SecretHouses = (DWORD)-1;
	UseBuffer = false;
	IsSelectableCombatant = false;
	IsAccelerates = true;
	DisableVoxelCache = false;
	DisableShadowCache = false;
	ZFudgeCliff = 10;
	ZFudgeColumn = 5;
	ZFudgeTunnel = 10;
	ZFudgeBridge = 0;
	Palette = NULL;
	IsHasRadialIndicator = true;

	DebrisTypes.Clear();
	DebrisAnims.Clear();
	DebrisMaximums.Clear();
	Dock.Clear();

	for (int weapon = 0; weapon < MAX_WEAPONS; weapon++) {
		Weapons[weapon].Weapon = NULL;
		Weapons[weapon].FireFLH = TPoint3D<int>(0, 0, 0);
		Weapons[weapon].BarrelLength = 0;
		Weapons[weapon].BarrelThickness = 0;
		Weapons[weapon].TurretLocked = false;
	}
	for (int elite = 0; elite < MAX_WEAPONS; elite++) {
		EliteWeapons[elite].Weapon = NULL;
		EliteWeapons[elite].FireFLH = TPoint3D<int>(0, 0, 0);
		EliteWeapons[elite].BarrelLength = 0;
		EliteWeapons[elite].BarrelThickness = 0;
		EliteWeapons[elite].TurretLocked = false;
	}
	for (int flh = 0; flh < 5; flh++) {
		AlternativeFLH[flh] = TPoint3D<int>(0, 0, 0);
	}
	SecondSpawnOffset = TPoint3D<int>(0, 0, 0);
	for (int stage = 0; stage < 6; stage++) {
		WeaponStage[stage] = 0;
		EliteStage[stage] = 0;
	}
	for (int turret = 0; turret < MAX_WEAPONS; turret++) {
		TurretWeapon[turret] = -1;
	}

	AbstractTypePtrTracker.Add(this);
	Array.Add(this);
}
