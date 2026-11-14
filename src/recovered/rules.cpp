// rules.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/rules.json carries the addresses and the rows that
// stay in their own files.


// Header-static dynamic initializers of the unit at 0x00640270:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x00640270-0x00640456); the other
// drafts of this plan are emitted by these includes.
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"


// Dynamic initializer (0x006402A0, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_00640270.cpp; no source of its own.


// Dynamic initializer (0x006402C0, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_00640270.cpp; no source of its own.


// Dynamic initializer (0x006402E0, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_00640270.cpp; no source of its own.


// Dynamic initializer (0x00640300, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_00640270.cpp; no source of its own.


// Dynamic initializer (0x00640320, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_00640270.cpp; no source of its own.


// Dynamic initializer (0x00640340, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_00640270.cpp; no source of its own.


// Dynamic initializer (0x00640380, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_00640270.cpp; no source of its own.


// Dynamic initializer (0x006403B0, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_00640270.cpp; no source of its own.


// Dynamic initializer (0x006403E0, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_00640270.cpp; no source of its own.


// Dynamic initializer (0x00640410, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_00640270.cpp; no source of its own.


// Dynamic initializer (0x00640420, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_00640270.cpp; no source of its own.


// Dynamic initializer (0x00640440, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_00640270.cpp; no source of its own.


// RulesClass::RulesClass(NoInitClass const &) (0x00640460)
// The load constructor: only the members with their own constructors (the lists
// and colours) are built; RulesClass::Load then reads the rest.
#include "shared/ccini_class.hpp"
#include "shared/rules.hpp"

RulesClass::RulesClass(NoInitClass const &noinit) :
    RadColor(noinit),
    LineTrailColorOverride(noinit),
    ChronoBeamColor(noinit)
{
}


// RulesClass::RulesClass (0x00640B80)
// Every rules value starts at its built-in default (the readers then take the
// rules.ini value, defaulting to the current one). The two rocket models are set
// in the body, after the members.

RulesClass::RulesClass() :
    DetailMinFrameRateNormal(15),
    DetailMinFrameRateMovie(20),
    DetailBufferZoneWidth(5),
    AmmoCrateDamage(100),
    LargeVisceroid(0),
    SmallVisceroid(0),
    UnloadingHarvester(0),
    AttackingAircraftSightRange(5),
    unknown_20(1.0),
    TiberiumHeal(0.016666666666666666),
    FreeMCV(false),
    BerzerkAllowed(false),
    unknown_50(0),
    DropPodPuff(0),
    WaypointAnimationSpeed(12),
    BarrelExplode(0),
    BarrelParticle(0),
    RadarEventColorSpeed(0.05f),
    RadarEventMinRadius(5),
    RadarEventSpeed(1.0f),
    RadarEventRotationSpeed(0.1f),
    FlashFrameTime(7),
    RadarCombatFlashTime(21),
    unknown_98(15),
    unknown_09c(0),
    NukeTakeOff(0),
    InfantryExplode(0),
    FlamingInfantry(0),
    InfantryHeadPop(0),
    InfantryNuked(0),
    Behind(0),
    unknown_b8(1.0),
    unknown_c0(-1.0),
    unknown_c8(1.0),
    VeinholeMonsterStrength(10000),
    unknown_d4(1000),
    VeinholeGrowthRate(100),
    VeinholeShrinkRate(100),
    VeinAttack(0),
    unknown_e4(2),
    MaximumQueuedObjects(5),
    unknown_ec(6),
    unknown_f0(0),
    unknown_f4(0),
    unknown_f8(0),
    unknown_16c(-1),
    CreateUnitSound(-1),
    CreateInfantrySound(-1),
    CreateAircraftSound(-1),
    BaseUnderAttackSound(-1),
    unknown_180(-1),
    unknown_184(-1),
    unknown_188(-1),
    unknown_18c(-1),
    unknown_190(-1),
    GUIMoveOutSound(-1),
    GUIMoveInSound(-1),
    GUIComboOpenSound(-1),
    GUIComboCloseSound(-1),
    GUICheckboxSound(-1),
    ScoreAnimSound(-1),
    IFVTransformSound(-1),
    PsychicSensorDetectSound(-1),
    BuildingGarrisonedSound(-1),
    BuildingAbandonedSound(-1),
    unknown_1bc(-1),
    CheerSound(-1),
    PlaceBeaconSound(-1),
    DefaultChronoSound(-1),
    unknown_1cc(-1),
    unknown_1d0(-1),
    ExecutePlanSound(-1),
    unknown_1d8(-1),
    CrateMoneySound(-1),
    CrateRevealSound(-1),
    CrateFireSound(-1),
    CrateArmourSound(-1),
    CrateSpeedSound(-1),
    CrateUnitSound(-1),
    CratePromoteSound(-1),
    ImpactWaterSound(-1),
    ImpactLandSound(-1),
    unknown_200(-1),
    BombTickingSound(-1),
    BombAttachSound(-1),
    ChronoInSound(-1),
    ChronoOutSound(-1),
    SpySatActivationSound(-1),
    SpySatDeactivationSound(-1),
    UpgradeVeteranSound(-1),
    UpgradeEliteSound(-1),
    unknown_228(0),
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
    unknown_2a8(0),
    IronCurtainInvokeAnim(0),
    WeaponNullifyAnim(0),
    AtmosphereEntry(0),
    GateUp(-1),
    GateDown(-1),
    unknown_368(3),
    Speed(30),
    Climb(5.0),
    unknown_378(400),
    Acceleration(0.25),
    WobblesPerSecond(0.25),
    WobbleDeviation(40),
    IonCannonDamage(700),
    RailgunDamageRadius(128),
    PrismType(0),
    PrismSupportModifier(100),
    PrismSupportMax(8),
    PrismSupportDelay(100),
    PrismSupportDuration(15),
    PrismSupportHeight(300),
    unknown_468(1024),
    ZoomInFactor(2.0),
    ConditionRedSparkingProbability(0.02),
    ConditionYellowSparkingProbability(0.01),
    unknown_488(100),
    TiberiumStrength(10),
    MinLowPowerProductionSpeed(0.5f),
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
    DropPodAngle(1.5707963267948966),
    ScrollMultiplier(1.0),
    CrewEscape(0.5),
    unknown_4e8(400),
    HoverHeight(120),
    HoverBob(30.0),
    HoverBoost(1.3),
    HoverAcceleration(0.03),
    HoverBrake(0.03),
    HoverDampen(0.8),
    PlacementDelay(0.05),
    TireVoxelDebris(0),
    ScrapVoxelDebris(0),
    BridgeVoxelMax(3),
    unknown_548(9),
    RevealTriggerRadius(5),
    ShipSinkingWeight(3.0),
    IceCrackingWeight(2.0),
    IceBreakingWeight(4.0),
    CliffBackImpassability((char)0),
    VeteranRatio(10.0),
    VeteranCombat(1.0),
    unknown_598(1.0),
    unknown_5a0(1.0),
    VeteranArmor(1.0),
    VeteranROF(1.0),
    VeteranCap(1.0),
    unknown_5c0(-1),
    unknown_5c4(-1),
    GameClosed(-1),
    IncomingMessage(-1),
    SystemError(-1),
    OptionsChanged(-1),
    GameForming(-1),
    PlayerLeft(-1),
    PlayerJoined(-1),
    MessageCharTyped(-1),
    Construction(-1),
    BuildingDieSound(-1),
    BuildingSlam(-1),
    unknown_610(-1),
    unknown_614(-1),
    unknown_618(-1),
    MovieOff(-1),
    unknown_620(-1),
    TeslaCharge(-1),
    TeslaZap(-1),
    unknown_62c(-1),
    GenericBeep(-1),
    BuildingDamageSound(-1),
    HealCrateSound(-1),
    ChuteSound(-1),
    unknown_640(-1),
    unknown_644(-1),
    unknown_648(-1),
    unknown_64c(-1),
    StormSound(-1),
    ShellButtonSlideSound(-1),
    WallBuildSpeedCoefficient(0.5),
    ChargeToDrainRatio(3.0),
    DamageToFirestormDamageCoefficient(0.1),
    TrackedUphill(1.0),
    TrackedDownhill(1.0),
    WheeledUphill(1.0),
    WheeledDownhill(1.0),
    unknown_6b0(2000),
    unknown_6b4(1000),
    SpotlightSpeed(0.05),
    SpotlightAcceleration(0.005),
    SpotlightAngle(20.0),
    unknown_6d0(175),
    unknown_6d4(-1),
    CameraRange(2304),
    unknown_6dc(500),
    ParachuteMaxFallRate(-3),
    NoParachuteMaxFallRate(-100),
    BuildingDrop(-1),
    unknown_778(0),
    unknown_77c(0),
    unknown_780(0),
    unknown_784(0),
    unknown_788(0),
    unknown_78c(0),
    GDIPowerPlant(0),
    GDIPowerTurbine(0),
    NodRegularPower(0),
    NodAdvancedPower(0),
    unknown_7bc(0),
    GDIHunterSeeker(0),
    NodHunterSeeker(0),
    GDIWallDefense(6.0),
    GDIWallDefenseCoefficient(3.0),
    NodBaseDefenseCoefficient(1.0),
    GDIBaseDefenseCoefficient(1.0),
    ComputerBaseDefenseResponse(3),
    MaximumBaseDefenseValue(60),
    Smoke(0),
    unknown_a60(0),
    FirestormActiveAnim(0),
    FirestormIdleAnim(0),
    FirestormAirAnim(0),
    FirestormGroundAnim(0),
    unknown_a74(0),
    unknown_a78(0),
    unknown_a7c(0),
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
    unknown_b6c(0),
    unknown_b70(0),
    SpyPowerBlackout(0),
    SpyMoneyStealPercent(0.5f),
    AttackCursorOnDisguise(false),
    unknown_b80(0.8f),
    AISafeDistance(8),
    HarvesterTooFarDistance(5),
    ChronoHarvTooFarDistance(50),
    AIRestrictReplaceTime(500),
    unknown_be8(5),
    unknown_bec(1),
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
    unknown_c60(0),
    NukeProjectile(0),
    NukeDown(0),
    EMPulseWarhead(0),
    EMPulseProjectile(0),
    unknown_c74(0),
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
    BombCursorShape(0),
    IronCurtainDuration(0),
    IonCannonWarhead(0),
    unknown_cac(0),
    unknown_cb0(0),
    unknown_cb4(0),
    InfantryBlinkDisguiseTime(0),
    DefaultLargeGreySmokeSystem(0),
    DefaultSmallGreySmokeSystem(0),
    unknown_ce0(0),
    DefaultLargeRedSmokeSystem(0),
    DefaultSmallRedSmokeSystem(0),
    DefaultDebrisSmokeSystem(0),
    DefaultFireStreamSystem(0),
    unknown_cf4(0),
    DefaultTestParticleSystem(0),
    DefaultRepairParticleSystem(0),
    unknown_d00(0.0),
    unknown_d08(0.0),
    unknown_d10(0.0),
    unknown_d18(0.0),
    unknown_d20(0.0),
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
    HunterSeekerEmergeSpeed(0),
    TurboBoost(1.5),
    AttackInterval(3.0),
    AttackDelay(5.0),
    PowerEmergency(0.75),
    AirstripRatio(0.12),
    AirstripLimit(5),
    HelipadRatio(0.12),
    HelipadLimit(5),
    TeslaRatio(0.16),
    TeslaLimit(10),
    AARatio(0.14),
    AALimit(10),
    DefenseRatio(0.5),
    DefenseLimit(40),
    WarRatio(0.1),
    WarLimit(2),
    BarracksRatio(0.16),
    BarracksLimit(2),
    RefineryLimit(4),
    RefineryRatio(0.16),
    BaseSizeAdd(3),
    PowerSurplus(50),
    InfantryReserve(2000),
    InfantryBaseMult(2),
    SoloCrateMoney(2000),
    unknown_e1c(25),
    UnitCrateType(0),
    unknown_e28(0.016),
    DissolveUnfilledTeamDelay(5000),
    AIAlternateProductionCreditCutoff(1000),
    AIUseTurbineUpgradeProbability(1.0),
    CloakDelay(0.0),
    GameSpeedBias(1.0),
    BaseBias(1.0),
    ExpSpread(0.5),
    FireSupress(256),
    MaxIQ(5),
    IQSuperWeapons(4),
    IQProduction(5),
    unknown_10a8(4),
    IQRepairSell(3),
    unknown_10b0(2),
    IQScatter(3),
    unknown_10b8(4),
    IQAircraft(4),
    IQHarvester(3),
    IQSellBack(2),
    AIBaseSpacing(1),
    SilverCrate((CrateType)2),
    WoodCrate((CrateType)0),
    WaterCrate((CrateType)0),
    CrateMinimum(1),
    CrateMaximum(255),
    unknown_10e0(8192),
    DropZoneAnim(0),
    MinMoney(2500),
    Money(10000),
    MaxMoney(10000),
    MoneyIncrement(100),
    MinUnitCount(1),
    UnitCount(10),
    MaxUnitCount(20),
    TechLevel(10),
    GameSpeed(6),
    AIDifficulty(1),
    AIPlayers(1),
    BridgeDestruction(true),
    ShadowGrow(false),
    Shroud(true),
    Bases(true),
    TiberiumGrows(true),
    Crates(true),
    CaptureTheFlag(false),
    HarvesterTruce(false),
    MultiEngineer(false),
    unknown_111d(false),
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
    unknown_1150(2.0),
    SurvivorRate(0.5),
    unknown_1160(200),
    unknown_1164(100),
    ReloadRate(0.05),
    AutocreateTime(5.0),
    unknown_1178(0.05),
    unknown_1180(2),
    HarvesterDumpRate(0.016),
    unknown_1190(1000),
    GrowthRate(2.0),
    ShroudRate(4.0),
    FogRate(0.05),
    IceGrowthRate(1.0),
    VeinGrowthRate(1.0),
    IceSolidifyFrameTime(500),
    AmbientChangeRate(0.2),
    AmbientChangeStep(0.1),
    CrateRegen(10.0),
    TimerWarning(2.0),
    unknown_12f0(14.0),
    unknown_12f8(5.0),
    unknown_1300(10.0),
    unknown_1308(10.0),
    SpeakDelay(2.0),
    DamageDelay(1.0),
    unknown_1320(3),
    unknown_1324(50),
    unknown_1328(0),
    MinDamage(1),
    MaxDamage(1000),
    unknown_1334(5),
    unknown_1338(0.25),
    unknown_1340(1),
    unknown_1348(0.016),
    URepairRate(0.016),
    IRepairRate(0.016),
    unknown_1360(1.0),
    unknown_1368(0.5),
    unknown_1370(0.5),
    IdleActionFrequency(0.083),
    unknown_1380(640),
    unknown_1384(512),
    unknown_1388(512),
    unknown_1390(384),
    CrateRadius(640),
    HomingScatter(512),
    unknown_139c(256),
    unknown_13a0(0.5),
    unknown_13a8(1000),
    unknown_13b0(1.0),
    unknown_13b8(0.03),
    CreditReserve(1000),
    unknown_13c8(0.016),
    unknown_13d0(60),
    MovieTime(0.25),
    TiberiumNearScan(1536),
    TiberiumFarScan(8192),
    unknown_13e8(250),
    LightningDamage(200),
    LightningStormDuration(900),
    LightningHitDelay(90),
    LightningScatterDelay(10),
    LightningCellSpread(10),
    LightningSeparation(3),
    LightningPrintText(true),
    LightningWarhead(0),
    unknown_140c(100),
    unknown_1410(0),
    HarvestersPerRefinery(2),
    ExtraUnitLight(0),
    ExtraInfantryLight(0),
    ExtraAircraftLight(0),
    Paranoid(true),
    CurleyShuffle(false),
    BlendedFog(true),
    CompEasyBonus(true),
    FineDiffControl(false),
    unknown_1429(false),
    EnemyHealth(true),
    unknown_142b(true),
    unknown_142c(false),
    unknown_142d(false),
    NamedCivilians(false),
    unknown_142f(false),
    PlayerReturnFire(false),
    unknown_1431(false),
    RevealByHeight(true),
    AllowShroudedSubteranneanMoves(false),
    ShroudGrow(false),
    NodAIBuildsWalls(true),
    AIBuildsWalls(true),
    unknown_1437(true),
    unknown_1438(0),
    unknown_143c(1.0f),
    EngineerCaptureLevel(0.0f),
    TalkBubbleTime(300),
    RadDurationMultiple(0),
    unknown_144c(0),
    RadLevelMax(0),
    RadLevelDelay(0),
    RadLightDelay(0),
    unknown_1460(0.0),
    RadLightFactor(0.0),
    RadTintFactor(0.0),
    unknown_147c(0),
    unknown_1480(0),
    unknown_1488(1.0),
    unknown_1490(0.0),
    unknown_1498(false),
    WallPenetratorThreshold(0.5),
    LocalRadarColor(255, 255, 255),
    OreTwinkleChance(50),
    OreTwinkle(0)
{
    V3Rocket.PauseFrames = 0;
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


// RulesClass::~RulesClass (0x006429E0)
// Only the members with destructors (the lists) are released.

RulesClass::~RulesClass()
{
}


// void RulesClass::Process(CCINIClass &ini) (0x006433A0)
// Rebuilds the rules: deletes every colour scheme (then 0x00603950), team,
// task force, script, overlay, smudge, terrain, infantry, unit, aircraft,
// bullet, building, super weapon, animation, weapon, warhead, voxel
// animation, particle, particle system, house and house type; reads
// [Maximums] Players, then Addition(ini), then LANGRULE.INI when present
// (a load result above 1 stops here), then the multiplayer session's INI.
#include "shared/rules_sections.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/color_scheme.hpp"
#include "shared/script.hpp"
#include "shared/team_type.hpp"
#include "shared/task_force.hpp"
#include "shared/overlay_type.hpp"
#include "shared/smudge_type.hpp"
#include "shared/terrain_type.hpp"
#include "shared/infantry_type.hpp"
#include "shared/unit_type.hpp"
#include "shared/aircraft_type.hpp"
#include "shared/bullet_type.hpp"
#include "shared/building_type_list.hpp"
#include "shared/building_type.hpp"
#include "shared/super_weapon_type.hpp"
#include "shared/anim_type.hpp"
#include "shared/weapon_type.hpp"
#include "shared/warhead_type.hpp"
#include "shared/voxel_anim_type.hpp"
#include "shared/particle_system.hpp"
#include "shared/house.hpp"
#include "shared/house_type.hpp"
#include "shared/radar.hpp"

void Unknown_00603950();

void RulesClass::Process(CCINIClass &ini)
{
    while (ColorSchemes.Count()) delete ColorSchemes[0];
    ColorSchemes.Clear();
    Unknown_00603950();

    while (TeamTypes.Count()) delete TeamTypes[0];
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
    while (unknown_WeaponTypeClass_list_00839b90.Count()) delete unknown_WeaponTypeClass_list_00839b90[0];
    while (unknown_WarheadTypeClass_list_00839b00.Count()) delete unknown_WarheadTypeClass_list_00839b00[0];
    while (VoxelAnimTypes.Count()) delete VoxelAnimTypes[0];
    while (ParticleTypes.Count()) delete ParticleTypes[0];
    while (ParticleSystemTypes.Count()) delete ParticleSystemTypes[0];
    while (Houses.Count()) delete Houses[0];
    while (HouseTypes.Count()) delete HouseTypes[0];

    if (ini.Is_Present(RulesSection_Maximums)) {
        MaxPlayers = ini.Get_Int(RulesSection_Maximums, "Players", MaxPlayers);
    }
    Addition(ini);

    CCFileClass file("LANGRULE.INI");
    if (file.Is_Available() == true) {
        CCINIClass langini;
        if (langini.Load(file, true) > 1) {
            return;
        }
        Addition(langini);
    }
    if (unknown_game_mode_a3d298 && unknown_session_a3d29c) {
        Addition(*unknown_session_a3d29c->unknown_ini_005b7840());
    }
}


// RulesClass::Addition (0x006437F0)
// Reads every rules section of an INI in order: colours, countries, sides,
// the type lists, the settings sections, the objects' own sections, the
// tiberiums and the advanced command bar of the session kind.
#include "shared/anim.hpp"
#include "shared/tiberium.hpp"

bool RulesClass::Addition(CCINIClass &ini)
{
    Colors(ini);
    Countries(ini);
    Sides(ini);
    Overlays(ini);
    Super_Weapons(ini);
    Warheads(ini);
    Smudges(ini);
    Terrains(ini);
    Buildings(ini);
    Vehicles(ini);
    Aircraft(ini);
    Infantry(ini);
    Animations(ini);
    Voxel_Anims(ini);
    Particles(ini);
    Particle_Systems(ini);
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
    Radiation(ini);
    Elevation_Model(ini);
    Wall_Model(ini);
    Audio_Visual(ini);
    Special_Weapons(ini);
    TiberiumClass::Read_Types(ini);
    return Advanced_Command_Bar(ini, unknown_game_mode_a3d298 != 0 && unknown_game_mode_a3d298 != 5);
}


// RulesClass::Special_Weapons (0x00643B80)

bool RulesClass::Special_Weapons(CCINIClass &ini)
{
    if (ini.Is_Present(RulesSection_SpecialWeapons)) {
        HSBuilding = TypeListReader<BuildingTypeClass>::Get(ini, RulesSection_SpecialWeapons, "HSBuilding", HSBuilding);
        unknown_c60 = Get_Type(ini, RulesSection_SpecialWeapons, "NukeWarhead", unknown_c60);
        NukeProjectile = Get_Type(ini, RulesSection_SpecialWeapons, "NukeProjectile", NukeProjectile);
        NukeDown = Get_Type(ini, RulesSection_SpecialWeapons, "NukeDown", NukeDown);
        EMPulseWarhead = Get_Type(ini, RulesSection_SpecialWeapons, "EMPulseWarhead", EMPulseWarhead);
        EMPulseProjectile = Get_Type(ini, RulesSection_SpecialWeapons, "EMPulseProjectile", EMPulseProjectile);
        
        for (int index = 0; index < unknown_WarheadTypeClass_list_00839b00.Count(); index++) {
            unknown_WarheadTypeClass_list_00839b00[index]->Read_INI(ini);
        }
        return true;
    }
    return false;
}


// RulesClass::Audio_Visual (0x00643E10): the [AudioVisual] rules (TibSun Audio_Visual_Rules order).
// Every sound is read into a VocType local (shared/voc_type.hpp Get_VocType) and every double
// field into a double local before the store; int, bool, lepton, RGB, type and list fields are
// assigned directly. Get_Type<AnimTypeClass> 0x00653DF0 and Get_Type<AnimTypeClass const> 0x00653E40
// are the out-of-line copies once the inline budget is spent (from FirestormActiveAnim on);
// unknown_1360 is TibSun's ConditionGreen (set to 1).
#include "shared/voc_class.hpp"
#include "shared/voc_type.hpp"

bool RulesClass::Audio_Visual(CCINIClass &ini)
{
    if (ini.Is_Present(RulesSection_AudioVisual)) {
        DetailMinFrameRateNormal = ini.Get_Int(RulesSection_AudioVisual, "DetailMinFrameRateNormal", DetailMinFrameRateNormal);
        DetailMinFrameRateMovie = ini.Get_Int(RulesSection_AudioVisual, "DetailMinFrameRateMovie", DetailMinFrameRateMovie);
        DetailBufferZoneWidth = ini.Get_Int(RulesSection_AudioVisual, "DetailBufferZoneWidth", DetailBufferZoneWidth);
        UnloadingHarvester = Get_Type(ini, RulesSection_AudioVisual, "UnloadingHarvester", UnloadingHarvester);
        unknown_50 = ini.Get_Int(RulesSection_AudioVisual, "PoseDir", unknown_50);
        DropPodPuff = Get_Type(ini, RulesSection_AudioVisual, "DropPodPuff", DropPodPuff);
        WaypointAnimationSpeed = ini.Get_Int(RulesSection_AudioVisual, "WaypointAnimationSpeed", WaypointAnimationSpeed);
        BarrelExplode = Get_Type(ini, RulesSection_AudioVisual, "BarrelExplode", BarrelExplode);
        BarrelDebris = TypeListReader<VoxelAnimTypeClass>::Get(ini, RulesSection_AudioVisual, "BarrelDebris", BarrelDebris);
        BarrelParticle = Get_Type(ini, RulesSection_AudioVisual, "BarrelParticle", BarrelParticle);
        unknown_09c = Get_Type(ini, RulesSection_AudioVisual, "Wake", unknown_09c);
        FlamingInfantry = Get_Type(ini, RulesSection_AudioVisual, "FlamingInfantry", FlamingInfantry);
        NukeTakeOff = Get_Type(ini, RulesSection_AudioVisual, "NukeTakeOff", NukeTakeOff);
        InfantryHeadPop = Get_Type(ini, RulesSection_AudioVisual, "InfantryHeadPop", InfantryHeadPop);
        InfantryNuked = Get_Type(ini, RulesSection_AudioVisual, "InfantryNuked", InfantryNuked);
        Behind = Get_Type(ini, RulesSection_AudioVisual, "Behind", Behind);
        VeinAttack = Get_Type(ini, RulesSection_AudioVisual, "VeinAttack", VeinAttack);
        DropPod = TypeListReader<AnimTypeClass>::Get(ini, RulesSection_AudioVisual, "DropPod", DropPod);
        VocType dig_sound = Get_VocType(ini, RulesSection_AudioVisual, "DigSound", (VocType)unknown_16c);
        unknown_16c = dig_sound;
        VocType guimain_button_sound = Get_VocType(ini, RulesSection_AudioVisual, "GUIMainButtonSound", (VocType)unknown_180);
        unknown_180 = guimain_button_sound;
        VocType guibuild_sound = Get_VocType(ini, RulesSection_AudioVisual, "GUIBuildSound", (VocType)unknown_184);
        unknown_184 = guibuild_sound;
        VocType guitab_sound = Get_VocType(ini, RulesSection_AudioVisual, "GUITabSound", (VocType)unknown_188);
        unknown_188 = guitab_sound;
        VocType guiopen_sound = Get_VocType(ini, RulesSection_AudioVisual, "GUIOpenSound", (VocType)unknown_18c);
        unknown_18c = guiopen_sound;
        VocType guiclose_sound = Get_VocType(ini, RulesSection_AudioVisual, "GUICloseSound", (VocType)unknown_190);
        unknown_190 = guiclose_sound;
        VocType guimove_out_sound = Get_VocType(ini, RulesSection_AudioVisual, "GUIMoveOutSound", (VocType)GUIMoveOutSound);
        GUIMoveOutSound = guimove_out_sound;
        VocType guimove_in_sound = Get_VocType(ini, RulesSection_AudioVisual, "GUIMoveInSound", (VocType)GUIMoveInSound);
        GUIMoveInSound = guimove_in_sound;
        VocType guicombo_open_sound = Get_VocType(ini, RulesSection_AudioVisual, "GUIComboOpenSound", (VocType)GUIComboOpenSound);
        GUIComboOpenSound = guicombo_open_sound;
        VocType guicombo_close_sound = Get_VocType(ini, RulesSection_AudioVisual, "GUIComboCloseSound", (VocType)GUIComboCloseSound);
        GUIComboCloseSound = guicombo_close_sound;
        VocType guicheckbox_sound = Get_VocType(ini, RulesSection_AudioVisual, "GUICheckboxSound", (VocType)GUICheckboxSound);
        GUICheckboxSound = guicheckbox_sound;
        VocType score_anim_sound = Get_VocType(ini, RulesSection_AudioVisual, "ScoreAnimSound", (VocType)ScoreAnimSound);
        ScoreAnimSound = score_anim_sound;
        VocType cheer_sound = Get_VocType(ini, RulesSection_AudioVisual, "CheerSound", (VocType)CheerSound);
        CheerSound = cheer_sound;
        VocType default_chrono_sound = Get_VocType(ini, RulesSection_AudioVisual, "DefaultChronoSound", (VocType)DefaultChronoSound);
        DefaultChronoSound = default_chrono_sound;
        VocType start_planning_mode_sound = Get_VocType(ini, RulesSection_AudioVisual, "StartPlanningModeSound", (VocType)unknown_1cc);
        unknown_1cc = start_planning_mode_sound;
        VocType end_planning_mode_sound = Get_VocType(ini, RulesSection_AudioVisual, "EndPlanningModeSound", (VocType)unknown_1d8);
        unknown_1d8 = end_planning_mode_sound;
        VocType crate_money_sound = Get_VocType(ini, RulesSection_AudioVisual, "CrateMoneySound", (VocType)CrateMoneySound);
        CrateMoneySound = crate_money_sound;
        VocType crate_reveal_sound = Get_VocType(ini, RulesSection_AudioVisual, "CrateRevealSound", (VocType)CrateRevealSound);
        CrateRevealSound = crate_reveal_sound;
        VocType crate_fire_sound = Get_VocType(ini, RulesSection_AudioVisual, "CrateFireSound", (VocType)CrateFireSound);
        CrateFireSound = crate_fire_sound;
        VocType crate_armour_sound = Get_VocType(ini, RulesSection_AudioVisual, "CrateArmourSound", (VocType)CrateArmourSound);
        CrateArmourSound = crate_armour_sound;
        VocType crate_speed_sound = Get_VocType(ini, RulesSection_AudioVisual, "CrateSpeedSound", (VocType)CrateSpeedSound);
        CrateSpeedSound = crate_speed_sound;
        VocType crate_unit_sound = Get_VocType(ini, RulesSection_AudioVisual, "CrateUnitSound", (VocType)CrateUnitSound);
        CrateUnitSound = crate_unit_sound;
        VocType crate_promote_sound = Get_VocType(ini, RulesSection_AudioVisual, "CratePromoteSound", (VocType)CratePromoteSound);
        CratePromoteSound = crate_promote_sound;
        VocType impact_water_sound = Get_VocType(ini, RulesSection_AudioVisual, "ImpactWaterSound", (VocType)ImpactWaterSound);
        ImpactWaterSound = impact_water_sound;
        VocType impact_land_sound = Get_VocType(ini, RulesSection_AudioVisual, "ImpactLandSound", (VocType)ImpactLandSound);
        ImpactLandSound = impact_land_sound;
        VocType sinking_sound = Get_VocType(ini, RulesSection_AudioVisual, "SinkingSound", (VocType)unknown_200);
        unknown_200 = sinking_sound;
        VocType chrono_in_sound = Get_VocType(ini, RulesSection_AudioVisual, "ChronoInSound", (VocType)ChronoInSound);
        ChronoInSound = chrono_in_sound;
        VocType chrono_out_sound = Get_VocType(ini, RulesSection_AudioVisual, "ChronoOutSound", (VocType)ChronoOutSound);
        ChronoOutSound = chrono_out_sound;
        VocType bomb_ticking_sound = Get_VocType(ini, RulesSection_AudioVisual, "BombTickingSound", (VocType)BombTickingSound);
        BombTickingSound = bomb_ticking_sound;
        VocType bomb_attach_sound = Get_VocType(ini, RulesSection_AudioVisual, "BombAttachSound", (VocType)BombAttachSound);
        BombAttachSound = bomb_attach_sound;
        VocType yuri_mind_control_sound = Get_VocType(ini, RulesSection_AudioVisual, "YuriMindControlSound", (VocType)YuriMindControlSound);
        YuriMindControlSound = yuri_mind_control_sound;
        VocType add_planning_mode_command_sound = Get_VocType(ini, RulesSection_AudioVisual, "AddPlanningModeCommandSound", (VocType)unknown_1d0);
        unknown_1d0 = add_planning_mode_command_sound;
        VocType execute_plan_sound = Get_VocType(ini, RulesSection_AudioVisual, "ExecutePlanSound", (VocType)ExecutePlanSound);
        ExecutePlanSound = execute_plan_sound;
        VocType place_beacon_sound = Get_VocType(ini, RulesSection_AudioVisual, "PlaceBeaconSound", (VocType)PlaceBeaconSound);
        PlaceBeaconSound = place_beacon_sound;
        VocType building_garrisoned_sound = Get_VocType(ini, RulesSection_AudioVisual, "BuildingGarrisonedSound", (VocType)BuildingGarrisonedSound);
        BuildingGarrisonedSound = building_garrisoned_sound;
        VocType building_abandoned_sound = Get_VocType(ini, RulesSection_AudioVisual, "BuildingAbandonedSound", (VocType)BuildingAbandonedSound);
        BuildingAbandonedSound = building_abandoned_sound;
        VocType building_repaired_sound = Get_VocType(ini, RulesSection_AudioVisual, "BuildingRepairedSound", (VocType)unknown_1bc);
        unknown_1bc = building_repaired_sound;
        VocType base_under_attack_sound = Get_VocType(ini, RulesSection_AudioVisual, "BaseUnderAttackSound", (VocType)BaseUnderAttackSound);
        BaseUnderAttackSound = base_under_attack_sound;
        VocType upgrade_veteran_sound = Get_VocType(ini, RulesSection_AudioVisual, "UpgradeVeteranSound", (VocType)UpgradeVeteranSound);
        UpgradeVeteranSound = upgrade_veteran_sound;
        VocType upgrade_elite_sound = Get_VocType(ini, RulesSection_AudioVisual, "UpgradeEliteSound", (VocType)UpgradeEliteSound);
        UpgradeEliteSound = upgrade_elite_sound;
        VocType create_unit_sound = Get_VocType(ini, RulesSection_AudioVisual, "CreateUnitSound", (VocType)CreateUnitSound);
        CreateUnitSound = create_unit_sound;
        VocType create_infantry_sound = Get_VocType(ini, RulesSection_AudioVisual, "CreateInfantrySound", (VocType)CreateInfantrySound);
        CreateInfantrySound = create_infantry_sound;
        VocType create_aircraft_sound = Get_VocType(ini, RulesSection_AudioVisual, "CreateAircraftSound", (VocType)CreateAircraftSound);
        CreateAircraftSound = create_aircraft_sound;
        VocType ifvtransform_sound = Get_VocType(ini, RulesSection_AudioVisual, "IFVTransformSound", (VocType)IFVTransformSound);
        IFVTransformSound = ifvtransform_sound;
        VocType psychic_sensor_detect_sound = Get_VocType(ini, RulesSection_AudioVisual, "PsychicSensorDetectSound", (VocType)PsychicSensorDetectSound);
        PsychicSensorDetectSound = psychic_sensor_detect_sound;
        VocType spy_sat_activation_sound = Get_VocType(ini, RulesSection_AudioVisual, "SpySatActivationSound", (VocType)SpySatActivationSound);
        SpySatActivationSound = spy_sat_activation_sound;
        VocType spy_sat_deactivation_sound = Get_VocType(ini, RulesSection_AudioVisual, "SpySatDeactivationSound", (VocType)SpySatDeactivationSound);
        SpySatDeactivationSound = spy_sat_deactivation_sound;
        unknown_228 = Get_Type(ini, RulesSection_AudioVisual, "Dig", unknown_228);
        IonBlast = Get_Type(ini, RulesSection_AudioVisual, "IonBlast", IonBlast);
        IonBeam = Get_Type(ini, RulesSection_AudioVisual, "IonBeam", IonBeam);
        ChronoBeam = Get_Type(ini, RulesSection_AudioVisual, "ChronoBeam", ChronoBeam);
        ChronoBlast = Get_Type(ini, RulesSection_AudioVisual, "ChronoBlast", ChronoBlast);
        ChronoBlastDest = Get_Type(ini, RulesSection_AudioVisual, "ChronoBlastDest", ChronoBlastDest);
        ChronoPlacement = Get_Type(ini, RulesSection_AudioVisual, "ChronoPlacement", ChronoPlacement);
        WarpIn = Get_Type(ini, RulesSection_AudioVisual, "WarpIn", WarpIn);
        WarpOut = Get_Type(ini, RulesSection_AudioVisual, "WarpOut", WarpOut);
        WarpAway = Get_Type(ini, RulesSection_AudioVisual, "WarpAway", WarpAway);
        unknown_2a8 = Get_Type(ini, RulesSection_AudioVisual, "ChronoSparkle1", unknown_2a8);
        IronCurtainInvokeAnim = Get_Type(ini, RulesSection_AudioVisual, "IronCurtainInvokeAnim", IronCurtainInvokeAnim);
        WeaponNullifyAnim = Get_Type(ini, RulesSection_AudioVisual, "WeaponNullifyAnim", WeaponNullifyAnim);
        InfantryExplode = Get_Type(ini, RulesSection_AudioVisual, "InfantryExplode", InfantryExplode);
        AtmosphereEntry = Get_Type(ini, RulesSection_AudioVisual, "AtmosphereEntry", AtmosphereEntry);
        VocType gate_up = Get_VocType(ini, RulesSection_AudioVisual, "GateUp", (VocType)GateUp);
        GateUp = gate_up;
        VocType gate_down = Get_VocType(ini, RulesSection_AudioVisual, "GateDown", (VocType)GateDown);
        GateDown = gate_down;
        ShroudGrow = ini.Get_Bool(RulesSection_AudioVisual, "ShroudGrow", ShroudGrow);
        double scroll_multiplier = ini.Get_Double(RulesSection_AudioVisual, "ScrollMultiplier", ScrollMultiplier);
        ScrollMultiplier = scroll_multiplier;
        unknown_4e8 = ini.Get_Int(RulesSection_AudioVisual, "ShakeScreen", unknown_4e8);
        VocType cloak_sound = Get_VocType(ini, RulesSection_AudioVisual, "CloakSound", (VocType)unknown_5c0);
        unknown_5c0 = cloak_sound;
        VocType sell_sound = Get_VocType(ini, RulesSection_AudioVisual, "SellSound", (VocType)unknown_5c4);
        unknown_5c4 = sell_sound;
        VocType game_closed = Get_VocType(ini, RulesSection_AudioVisual, "GameClosed", (VocType)GameClosed);
        GameClosed = game_closed;
        VocType incoming_message = Get_VocType(ini, RulesSection_AudioVisual, "IncomingMessage", (VocType)IncomingMessage);
        IncomingMessage = incoming_message;
        VocType message_char_typed = Get_VocType(ini, RulesSection_AudioVisual, "MessageCharTyped", (VocType)MessageCharTyped);
        MessageCharTyped = message_char_typed;
        VocType system_error = Get_VocType(ini, RulesSection_AudioVisual, "SystemError", (VocType)SystemError);
        SystemError = system_error;
        VocType options_changed = Get_VocType(ini, RulesSection_AudioVisual, "OptionsChanged", (VocType)OptionsChanged);
        OptionsChanged = options_changed;
        VocType game_forming = Get_VocType(ini, RulesSection_AudioVisual, "GameForming", (VocType)GameForming);
        GameForming = game_forming;
        VocType player_left = Get_VocType(ini, RulesSection_AudioVisual, "PlayerLeft", (VocType)PlayerLeft);
        PlayerLeft = player_left;
        VocType player_joined = Get_VocType(ini, RulesSection_AudioVisual, "PlayerJoined", (VocType)PlayerJoined);
        PlayerJoined = player_joined;
        VocType construction = Get_VocType(ini, RulesSection_AudioVisual, "Construction", (VocType)Construction);
        Construction = construction;
        CreditTicks = Get_Voc_List(ini, RulesSection_AudioVisual, "CreditTicks", CreditTicks);
        VocType building_die_sound = Get_VocType(ini, RulesSection_AudioVisual, "BuildingDieSound", (VocType)BuildingDieSound);
        BuildingDieSound = building_die_sound;
        VocType building_slam = Get_VocType(ini, RulesSection_AudioVisual, "BuildingSlam", (VocType)BuildingSlam);
        BuildingSlam = building_slam;
        VocType radar_on = Get_VocType(ini, RulesSection_AudioVisual, "RadarOn", (VocType)unknown_610);
        unknown_610 = radar_on;
        VocType radar_off = Get_VocType(ini, RulesSection_AudioVisual, "RadarOff", (VocType)unknown_614);
        unknown_614 = radar_off;
        VocType movie_on = Get_VocType(ini, RulesSection_AudioVisual, "MovieOn", (VocType)unknown_618);
        unknown_618 = movie_on;
        VocType movie_off = Get_VocType(ini, RulesSection_AudioVisual, "MovieOff", (VocType)MovieOff);
        MovieOff = movie_off;
        VocType scold_sound = Get_VocType(ini, RulesSection_AudioVisual, "ScoldSound", (VocType)unknown_620);
        unknown_620 = scold_sound;
        VocType tesla_charge = Get_VocType(ini, RulesSection_AudioVisual, "TeslaCharge", (VocType)TeslaCharge);
        TeslaCharge = tesla_charge;
        VocType tesla_zap = Get_VocType(ini, RulesSection_AudioVisual, "TeslaZap", (VocType)TeslaZap);
        TeslaZap = tesla_zap;
        VocType building_damage_sound = Get_VocType(ini, RulesSection_AudioVisual, "BuildingDamageSound", (VocType)BuildingDamageSound);
        BuildingDamageSound = building_damage_sound;
        VocType chute_sound = Get_VocType(ini, RulesSection_AudioVisual, "ChuteSound", (VocType)ChuteSound);
        ChuteSound = chute_sound;
        VocType generic_click = Get_VocType(ini, RulesSection_AudioVisual, "GenericClick", (VocType)unknown_62c);
        unknown_62c = generic_click;
        VocType generic_beep = Get_VocType(ini, RulesSection_AudioVisual, "GenericBeep", (VocType)GenericBeep);
        GenericBeep = generic_beep;
        VocType building_drop = Get_VocType(ini, RulesSection_AudioVisual, "BuildingDrop", (VocType)BuildingDrop);
        BuildingDrop = building_drop;
        VocType stop_sound = Get_VocType(ini, RulesSection_AudioVisual, "StopSound", (VocType)unknown_640);
        unknown_640 = stop_sound;
        VocType guard_sound = Get_VocType(ini, RulesSection_AudioVisual, "GuardSound", (VocType)unknown_644);
        unknown_644 = guard_sound;
        VocType scatter_sound = Get_VocType(ini, RulesSection_AudioVisual, "ScatterSound", (VocType)unknown_648);
        unknown_648 = scatter_sound;
        VocType storm_sound = Get_VocType(ini, RulesSection_AudioVisual, "StormSound", (VocType)StormSound);
        StormSound = storm_sound;
        LightningSounds = Get_Voc_List(ini, RulesSection_AudioVisual, "LightningSounds", LightningSounds);
        VocType shell_button_slide_sound = Get_VocType(ini, RulesSection_AudioVisual, "ShellButtonSlideSound", (VocType)ShellButtonSlideSound);
        ShellButtonSlideSound = shell_button_slide_sound;
        TreeFire = TypeListReader<AnimTypeClass>::Get(ini, RulesSection_AudioVisual, "TreeFire", TreeFire);
        DeadBodies = TypeListReader<AnimTypeClass>::Get(ini, RulesSection_AudioVisual, "DeadBodies", DeadBodies);
        MetallicDebris = TypeListReader<AnimTypeClass>::Get(ini, RulesSection_AudioVisual, "MetallicDebris", MetallicDebris);
        BridgeExplosions = TypeListReader<AnimTypeClass>::Get(ini, RulesSection_AudioVisual, "BridgeExplosions", BridgeExplosions);
        DamageFireTypes = TypeListReader<AnimTypeClass>::Get(ini, RulesSection_AudioVisual, "DamageFireTypes", DamageFireTypes);
        WeatherConClouds = TypeListReader<AnimTypeClass>::Get(ini, RulesSection_AudioVisual, "WeatherConClouds", WeatherConClouds);
        WeatherConBolts = TypeListReader<AnimTypeClass>::Get(ini, RulesSection_AudioVisual, "WeatherConBolts", WeatherConBolts);
        WeatherConBoltExplosion = Get_Type(ini, RulesSection_AudioVisual, "WeatherConBoltExplosion", WeatherConBoltExplosion);
        OnFire = TypeListReader<AnimTypeClass>::Get(ini, RulesSection_AudioVisual, "OnFire", OnFire);
        Smoke = Get_Type(ini, RulesSection_AudioVisual, "Smoke", Smoke);
        unknown_a60 = Get_Type(ini, RulesSection_AudioVisual, "Smoke", unknown_a60);
        FirestormActiveAnim = Get_Type(ini, RulesSection_AudioVisual, "FirestormActiveAnim", FirestormActiveAnim);
        FirestormIdleAnim = Get_Type(ini, RulesSection_AudioVisual, "FirestormIdleAnim", FirestormIdleAnim);
        FirestormAirAnim = Get_Type(ini, RulesSection_AudioVisual, "FirestormAirAnim", FirestormAirAnim);
        FirestormGroundAnim = Get_Type(ini, RulesSection_AudioVisual, "FirestormGroundAnim", FirestormGroundAnim);
        unknown_a74 = Get_Type(ini, RulesSection_AudioVisual, "MoveFlash", unknown_a74);
        EliteFlashTimer = ini.Get_Int(RulesSection_AudioVisual, "EliteFlashTimer", EliteFlashTimer);
        unknown_a7c = Get_Type(ini, RulesSection_AudioVisual, "Parachute", unknown_a7c);
        unknown_a78 = Get_Type(ini, RulesSection_AudioVisual, "BombParachute", unknown_a78);
        SmallFire = Get_Type(ini, RulesSection_AudioVisual, "SmallFire", SmallFire);
        LargeFire = Get_Type(ini, RulesSection_AudioVisual, "LargeFire", LargeFire);
        unknown_142b = ini.Get_Bool(RulesSection_AudioVisual, "AllyReveal", unknown_142b);
        unknown_1360 = 1;
        double condition_red = ini.Get_Double(RulesSection_AudioVisual, "ConditionRed", unknown_1370);
        unknown_1370 = condition_red;
        double condition_yellow = ini.Get_Double(RulesSection_AudioVisual, "ConditionYellow", unknown_1368);
        unknown_1368 = condition_yellow;
        DropZoneRadius = ini.Get_Lepton(RulesSection_AudioVisual, "DropZoneRadius", DropZoneRadius);
        DropZoneAnim = Get_Type(ini, RulesSection_AudioVisual, "DropZoneAnim", DropZoneAnim);
        EnemyHealth = ini.Get_Bool(RulesSection_AudioVisual, "EnemyHealth", EnemyHealth);
        unknown_1320 = ini.Get_Int(RulesSection_AudioVisual, "Gravity", unknown_1320);
        double idle_action_frequency = ini.Get_Double(RulesSection_AudioVisual, "IdleActionFrequency", IdleActionFrequency);
        IdleActionFrequency = idle_action_frequency;
        double message_delay = ini.Get_Double(RulesSection_AudioVisual, "MessageDelay", MessageDelay);
        MessageDelay = message_delay;
        double movie_time = ini.Get_Double(RulesSection_AudioVisual, "MovieTime", MovieTime);
        MovieTime = movie_time;
        NamedCivilians = ini.Get_Bool(RulesSection_AudioVisual, "NamedCivilians", NamedCivilians);
        double savour_delay = ini.Get_Double(RulesSection_AudioVisual, "SavourDelay", SavourDelay);
        SavourDelay = savour_delay;
        double shroud_rate = ini.Get_Double(RulesSection_AudioVisual, "ShroudRate", ShroudRate);
        ShroudRate = shroud_rate;
        double fog_rate = ini.Get_Double(RulesSection_AudioVisual, "FogRate", FogRate);
        FogRate = fog_rate;
        double vein_growth_rate = ini.Get_Double(RulesSection_AudioVisual, "VeinGrowthRate", VeinGrowthRate);
        VeinGrowthRate = vein_growth_rate;
        double ice_growth_rate = ini.Get_Double(RulesSection_AudioVisual, "IceGrowthRate", IceGrowthRate);
        IceGrowthRate = ice_growth_rate;
        IceSolidifyFrameTime = ini.Get_Int(RulesSection_AudioVisual, "IceSolidifyFrameTime", IceSolidifyFrameTime);
        IceCrackSounds.Clear();
        IceCrackSounds = Get_Voc_List(ini, RulesSection_AudioVisual, "IceCrackSounds", IceCrackSounds);
        double ambient_change_rate = ini.Get_Double(RulesSection_AudioVisual, "AmbientChangeRate", AmbientChangeRate);
        AmbientChangeRate = ambient_change_rate;
        double ambient_change_step = ini.Get_Double(RulesSection_AudioVisual, "AmbientChangeStep", AmbientChangeStep);
        AmbientChangeStep = ambient_change_step;
        double speak_delay = ini.Get_Double(RulesSection_AudioVisual, "SpeakDelay", SpeakDelay);
        SpeakDelay = speak_delay;
        double timer_warning = ini.Get_Double(RulesSection_AudioVisual, "TimerWarning", TimerWarning);
        TimerWarning = timer_warning;
        ExtraUnitLight = ini.Get_Double(RulesSection_AudioVisual, "ExtraUnitLight", ExtraUnitLight / 1000) * 1000.0;
        ExtraInfantryLight = ini.Get_Double(RulesSection_AudioVisual, "ExtraInfantryLight", ExtraInfantryLight / 1000) * 1000.0;
        ExtraAircraftLight = ini.Get_Double(RulesSection_AudioVisual, "ExtraAircraftLight", ExtraAircraftLight / 1000) * 1000.0;
        unknown_1438 = Get_Type(ini, RulesSection_AudioVisual, "EMPulseSparkles", unknown_1438);
        LocalRadarColor = ini.Get_RGB(RulesSection_AudioVisual, "LocalRadarColor", LocalRadarColor);
        LineTrailColorOverride = ini.Get_RGB(RulesSection_AudioVisual, "LineTrailColorOverride", LineTrailColorOverride);
        ChronoBeamColor = ini.Get_RGB(RulesSection_AudioVisual, "ChronoBeamColor", ChronoBeamColor);
        OreTwinkleChance = ini.Get_Int(RulesSection_AudioVisual, "OreTwinkleChance", OreTwinkleChance);
        OreTwinkle = Get_Type(ini, RulesSection_AudioVisual, "OreTwinkle", OreTwinkle);
        return true;
    }
    return false;
}


// RulesClass::Crate_Rules (0x006469B0)

bool RulesClass::Crate_Rules(CCINIClass &ini)
{
    if (ini.Is_Present(RulesSection_CrateRules)) {
        FreeMCV = ini.Get_Bool(RulesSection_CrateRules, "FreeMCV", FreeMCV);
        unknown_f0 = Get_Type(ini, RulesSection_CrateRules, "WoodCrateImg", unknown_f0);
        unknown_f4 = Get_Type(ini, RulesSection_CrateRules, "CrateImg", unknown_f4);
        unknown_f8 = Get_Type(ini, RulesSection_CrateRules, "WaterCrateImg", unknown_f8);
        HealCrateSound = Get_Sound(ini, RulesSection_CrateRules, "HealCrateSound", HealCrateSound);
        CrateMinimum = ini.Get_Int(RulesSection_CrateRules, "CrateMinimum", CrateMinimum);
        CrateMaximum = ini.Get_Int(RulesSection_CrateRules, "CrateMaximum", CrateMaximum);
        CrateRadius = ini.Get_Lepton(RulesSection_CrateRules, "CrateRadius", CrateRadius);
        CrateRegen = ini.Get_Double(RulesSection_CrateRules, "CrateRegen", CrateRegen);
        UnitCrateType = Get_Type(ini, RulesSection_CrateRules, "UnitCrateType", UnitCrateType);
        SoloCrateMoney = ini.Get_Int(RulesSection_CrateRules, "SoloCrateMoney", SoloCrateMoney);
        SilverCrate = ini.Get_CrateType(RulesSection_CrateRules, "SilverCrate", SilverCrate);
        WoodCrate = ini.Get_CrateType(RulesSection_CrateRules, "WoodCrate", WoodCrate);
        WaterCrate = ini.Get_CrateType(RulesSection_CrateRules, "WaterCrate", WaterCrate);
        return true;
    }
    return false;
}


// bool RulesClass::Combat_Damage(CCINIClass &ini) (0x00646C60)
// Reads [CombatDamage]: damages, the scorch and splash lists, the special
// warheads (the elite V3/DMisl warheads default to the normal ones), the
// default particle systems and the combat rules. Integer and lepton values go
// through named locals, as in RulesClass::AI.
#include "shared/mix_file_class.hpp"

bool RulesClass::Combat_Damage(CCINIClass &ini)
{
    if (ini.Is_Present(RulesSection_CombatDamage)) {
        int ammo_crate_damage = ini.Get_Int(RulesSection_CombatDamage, "AmmoCrateDamage", AmmoCrateDamage);
        AmmoCrateDamage = ammo_crate_damage;
        int ion_cannon_damage = ini.Get_Int(RulesSection_CombatDamage, "IonCannonDamage", IonCannonDamage);
        IonCannonDamage = ion_cannon_damage;
        int railgun_damage_radius = ini.Get_Int(RulesSection_CombatDamage, "RailgunDamageRadius", RailgunDamageRadius);
        RailgunDamageRadius = railgun_damage_radius;
        int tiberium_explosion_damage = ini.Get_Int(RulesSection_CombatDamage, "TiberiumExplosionDamage", unknown_488);
        unknown_488 = tiberium_explosion_damage;
        int tiberium_strength = ini.Get_Int(RulesSection_CombatDamage, "TiberiumStrength", TiberiumStrength);
        TiberiumStrength = tiberium_strength;
        Scorches = TypeListReader<SmudgeTypeClass>::Get(ini, RulesSection_CombatDamage, "Scorches", Scorches);
        Scorches1 = TypeListReader<SmudgeTypeClass>::Get(ini, RulesSection_CombatDamage, "Scorches1", Scorches1);
        Scorches2 = TypeListReader<SmudgeTypeClass>::Get(ini, RulesSection_CombatDamage, "Scorches2", Scorches2);
        Scorches3 = TypeListReader<SmudgeTypeClass>::Get(ini, RulesSection_CombatDamage, "Scorches3", Scorches3);
        Scorches4 = TypeListReader<SmudgeTypeClass>::Get(ini, RulesSection_CombatDamage, "Scorches4", Scorches4);
        SplashList = TypeListReader<AnimTypeClass>::Get(ini, RulesSection_CombatDamage, "SplashList", SplashList);
        FlameDamage = Get_Type(ini, RulesSection_CombatDamage, "FlameDamage", FlameDamage);
        FlameDamage2 = Get_Type(ini, RulesSection_CombatDamage, "FlameDamage2", FlameDamage2);
        unknown_c74 = Get_Type(ini, RulesSection_CombatDamage, "C4Warhead", unknown_c74);
        V3Warhead = Get_Type(ini, RulesSection_CombatDamage, "V3Warhead", V3Warhead);
        DMislWarhead = Get_Type(ini, RulesSection_CombatDamage, "DMislWarhead", DMislWarhead);
        V3EliteWarhead = Get_Type(ini, RulesSection_CombatDamage, "V3EliteWarhead", V3Warhead);
        DMislEliteWarhead = Get_Type(ini, RulesSection_CombatDamage, "DMislEliteWarhead", DMislWarhead);
        IvanWarhead = Get_Type(ini, RulesSection_CombatDamage, "IvanWarhead", IvanWarhead);
        CanDetonateTimeBomb = ini.Get_Bool(RulesSection_CombatDamage, "CanDetonateTimeBomb", CanDetonateTimeBomb);
        CanDetonateDeathBomb = ini.Get_Bool(RulesSection_CombatDamage, "CanDetonateDeathBomb", CanDetonateDeathBomb);
        DeathWeapon = Get_Type(ini, RulesSection_CombatDamage, "DeathWeapon", DeathWeapon);
        int ivan_damage = ini.Get_Int(RulesSection_CombatDamage, "IvanDamage", IvanDamage);
        IvanDamage = ivan_damage;
        int ivan_timed_delay = ini.Get_Int(RulesSection_CombatDamage, "IvanTimedDelay", IvanTimedDelay);
        IvanTimedDelay = ivan_timed_delay;
        BombCursorShape = MixFileClass::Retrieve("BOMBCURS.SHP");
        int ivan_icon_flicker_rate = ini.Get_Int(RulesSection_CombatDamage, "IvanIconFlickerRate", IvanIconFlickerRate);
        IvanIconFlickerRate = ivan_icon_flicker_rate;
        int iron_curtain_duration = ini.Get_Int(RulesSection_CombatDamage, "IronCurtainDuration", IronCurtainDuration);
        IronCurtainDuration = iron_curtain_duration;
        IonCannonWarhead = Get_Type(ini, RulesSection_CombatDamage, "IonCannonWarhead", IonCannonWarhead);
        unknown_cac = Get_Type(ini, RulesSection_CombatDamage, "FirestormWarhead", unknown_cac);
        unknown_cb0 = Get_Type(ini, RulesSection_CombatDamage, "VeinholeWarhead", unknown_cb0);
        unknown_cf4 = Get_Type(ini, RulesSection_CombatDamage, "DefaultFirestormExplosionSystem", unknown_cf4);
        DefaultLargeGreySmokeSystem = Get_Type(ini, RulesSection_CombatDamage, "DefaultLargeGreySmokeSystem", DefaultLargeGreySmokeSystem);
        DefaultSmallGreySmokeSystem = Get_Type(ini, RulesSection_CombatDamage, "DefaultSmallGreySmokeSystem", DefaultSmallGreySmokeSystem);
        unknown_ce0 = Get_Type(ini, RulesSection_CombatDamage, "DefaultSparkSystem", unknown_ce0);
        DefaultLargeRedSmokeSystem = Get_Type(ini, RulesSection_CombatDamage, "DefaultLargeRedSmokeSystem", DefaultLargeRedSmokeSystem);
        DefaultSmallRedSmokeSystem = Get_Type(ini, RulesSection_CombatDamage, "DefaultSmallRedSmokeSystem", DefaultSmallRedSmokeSystem);
        DefaultDebrisSmokeSystem = Get_Type(ini, RulesSection_CombatDamage, "DefaultDebrisSmokeSystem", DefaultDebrisSmokeSystem);
        DefaultFireStreamSystem = Get_Type(ini, RulesSection_CombatDamage, "DefaultFireStreamSystem", DefaultFireStreamSystem);
        DefaultTestParticleSystem = Get_Type(ini, RulesSection_CombatDamage, "DefaultTestParticleSystem", DefaultTestParticleSystem);
        DefaultRepairParticleSystem = Get_Type(ini, RulesSection_CombatDamage, "DefaultRepairParticleSystem", DefaultRepairParticleSystem);
        BerzerkAllowed = ini.Get_Bool(RulesSection_CombatDamage, "BerzerkAllowed", BerzerkAllowed);
        TurboBoost = ini.Get_Double(RulesSection_CombatDamage, "TurboBoost", TurboBoost);
        int atom_damage = ini.Get_Int(RulesSection_CombatDamage, "AtomDamage", unknown_1190);
        unknown_1190 = atom_damage;
        int ballistic_scatter = ini.Get_Lepton(RulesSection_CombatDamage, "BallisticScatter", unknown_139c);
        unknown_139c = ballistic_scatter;
        int bridge_strength = ini.Get_Int(RulesSection_CombatDamage, "BridgeStrength", unknown_13a8);
        unknown_13a8 = bridge_strength;
        unknown_13b8 = ini.Get_Double(RulesSection_CombatDamage, "C4Delay", unknown_13b8);
        int crush = ini.Get_Lepton(RulesSection_CombatDamage, "Crush", unknown_1390);
        unknown_1390 = crush;
        ExpSpread = ini.Get_Double(RulesSection_CombatDamage, "ExpSpread", ExpSpread);
        int fire_supress = ini.Get_Lepton(RulesSection_CombatDamage, "FireSupress", FireSupress);
        FireSupress = fire_supress;
        int homing_scatter = ini.Get_Lepton(RulesSection_CombatDamage, "HomingScatter", HomingScatter);
        HomingScatter = homing_scatter;
        int max_damage = ini.Get_Int(RulesSection_CombatDamage, "MaxDamage", MaxDamage);
        MaxDamage = max_damage;
        int min_damage = ini.Get_Int(RulesSection_CombatDamage, "MinDamage", MinDamage);
        MinDamage = min_damage;
        unknown_1429 = ini.Get_Bool(RulesSection_CombatDamage, "TiberiumExplosive", unknown_1429);
        unknown_142f = ini.Get_Bool(RulesSection_CombatDamage, "PlayerAutoCrush", unknown_142f);
        PlayerReturnFire = ini.Get_Bool(RulesSection_CombatDamage, "PlayerReturnFire", PlayerReturnFire);
        unknown_1431 = ini.Get_Bool(RulesSection_CombatDamage, "PlayerScatter", unknown_1431);
        unknown_142d = ini.Get_Bool(RulesSection_CombatDamage, "TreeTargeting", unknown_142d);
        unknown_1328 = ini.Get_Percent(RulesSection_CombatDamage, "Incoming", unknown_1328);
        int collapse_chance = ini.Get_Int(RulesSection_CombatDamage, "CollapseChance", unknown_140c);
        unknown_140c = collapse_chance;
        return true;
    }
    return false;
}


// RulesClass::Radiation (0x00647B60)

bool RulesClass::Radiation(CCINIClass &ini)
{
    if (ini.Is_Present(RulesSection_Radiation)) {
        RadDurationMultiple = ini.Get_Int(RulesSection_Radiation, "RadDurationMultiple", RadDurationMultiple);
        unknown_144c = ini.Get_Int(RulesSection_Radiation, "RadApplicationDelay", unknown_144c);
        RadLevelMax = ini.Get_Int(RulesSection_Radiation, "RadLevelMax", RadLevelMax);
        RadLevelDelay = ini.Get_Int(RulesSection_Radiation, "RadLevelDelay", RadLevelDelay);
        RadLightDelay = ini.Get_Int(RulesSection_Radiation, "RadLightDelay", RadLightDelay);
        unknown_1460 = ini.Get_Double(RulesSection_Radiation, "RadLevelFactor", unknown_1460);
        RadLightFactor = ini.Get_Double(RulesSection_Radiation, "RadLightFactor", RadLightFactor);
        RadTintFactor = ini.Get_Double(RulesSection_Radiation, "RadTintFactor", RadTintFactor);
        RadColor = ini.Get_RGB(RulesSection_Radiation, "RadColor", RadColor);
        unknown_147c = Get_Type(ini, RulesSection_Radiation, "RadSiteWarhead", unknown_147c);
        return true;
    }
    return false;
}


// RulesClass::Elevation_Model (0x00647D40)

bool RulesClass::Elevation_Model(CCINIClass &ini)
{
    if (ini.Is_Present(RulesSection_ElevationModel)) {
        unknown_1480 = ini.Get_Int(RulesSection_ElevationModel, "ElevationIncrement", unknown_1480);
        unknown_1488 = ini.Get_Double(RulesSection_ElevationModel, "ElevationIncrementBonus", unknown_1488);
        unknown_1490 = ini.Get_Double(RulesSection_ElevationModel, "ElevationBonusCap", unknown_1490);
        return true;
    }
    return false;
}


// RulesClass::Wall_Model (0x00647DE0)

bool RulesClass::Wall_Model(CCINIClass &ini)
{
    if (ini.Is_Present(RulesSection_WallModel)) {
        unknown_1498 = ini.Get_Bool(RulesSection_WallModel, "AlliedWallTransparency", unknown_1498);
        WallPenetratorThreshold = ini.Get_Double(RulesSection_WallModel, "WallPenetratorThreshold", WallPenetratorThreshold);
        return true;
    }
    return false;
}


// RulesClass::Colors (0x00647E60)
// Each [Colors] entry names an HSV colour; a colour scheme is made for it with
// two keys (1 and 0x35). The helpers' names are placeholders.
#include "shared/hsv_class.hpp"

// 0x00603A60: registers the named colour (an unidentified list at 0x00A76414).
void __fastcall unknown_add_color_603a60(char const *name, HSVClass const &color);
// 0x00663CA0: the index of the colour scheme named `name` with `key`, made when missing.
int __fastcall unknown_find_or_make_palette(char const *name, HSVClass const &color, void *arg3, void *arg4, int key);
extern char unknown_object_838a10[];        // 0x00838A10 (type unknown)

bool RulesClass::Colors(CCINIClass &ini)
{
    if (ini.Is_Present(RulesSection_Colors)) {
        int count = ini.Entry_Count(RulesSection_Colors);
        for (int index = 0; index < count; index++) {
            char const *entry = ini.Get_Entry(RulesSection_Colors, index);
            HSVClass color = ini.Get_HSV(RulesSection_Colors, entry, HSVClass(0, 0, 0));
            unknown_add_color_603a60(entry, color);
            unknown_find_or_make_palette(entry, color, unknown_object_838a10, &unknown_palette_837e10, 1);
            unknown_find_or_make_palette(entry, color, unknown_object_838a10, &unknown_palette_837e10, 0x35);
        }
        return true;
    }
    return false;
}


// RulesClass::General (0x00647F40)

// Westwood-style macros: each operand is evaluated where it appears.
#define Min(a, b) ((a) < (b) ? (a) : (b))
#define Max(a, b) ((a) > (b) ? (a) : (b))

bool RulesClass::General(CCINIClass &ini)
{
    if (ini.Is_Present(RulesSection_General)) {
        LargeVisceroid = Get_Type(ini, RulesSection_General, "LargeVisceroid", LargeVisceroid);
        SmallVisceroid = Get_Type(ini, RulesSection_General, "SmallVisceroid", SmallVisceroid);
        TiberiumHeal = ini.Get_Double(RulesSection_General, "TiberiumHeal", TiberiumHeal);
        PrerequisitePower = Get_Prerequisite_List(ini, RulesSection_General, "PrerequisitePower", PrerequisitePower);
        PrerequisiteFactory = Get_Prerequisite_List(ini, RulesSection_General, "PrerequisiteFactory", PrerequisiteFactory);
        PrerequisiteBarracks = Get_Prerequisite_List(ini, RulesSection_General, "PrerequisiteBarracks", PrerequisiteBarracks);
        PrerequisiteRadar = Get_Prerequisite_List(ini, RulesSection_General, "PrerequisiteRadar", PrerequisiteRadar);
        PrerequisiteTech = Get_Prerequisite_List(ini, RulesSection_General, "PrerequisiteTech", PrerequisiteTech);
        PrerequisiteProc = Get_Prerequisite_List(ini, RulesSection_General, "PrerequisiteProc", PrerequisiteProc);
        ZoomInFactor = ini.Get_Double(RulesSection_General, "ZoomInFactor", ZoomInFactor);
        RevealByHeight = ini.Get_Bool(RulesSection_General, "RevealByHeight", RevealByHeight);
        AllowShroudedSubteranneanMoves = ini.Get_Bool(RulesSection_General, "AllowShroudedSubteranneanMoves", AllowShroudedSubteranneanMoves);
        unknown_ec = ini.Get_Int(RulesSection_General, "AircraftFogReveal", unknown_ec);
        MinLowPowerProductionSpeed = ini.Get_Double(RulesSection_General, "MinLowPowerProductionSpeed", MinLowPowerProductionSpeed);
        MaxLowPowerProductionSpeed = ini.Get_Double(RulesSection_General, "MaxLowPowerProductionSpeed", MaxLowPowerProductionSpeed);
        LowPowerPenaltyModifier = ini.Get_Double(RulesSection_General, "LowPowerPenaltyModifier", LowPowerPenaltyModifier);
        MultipleFactory = ini.Get_Double(RulesSection_General, "MultipleFactory", MultipleFactory);
        MaximumCheerRate = ini.Get_Int(RulesSection_General, "MaximumCheerRate", MaximumCheerRate);
        TreeFlammability = ini.Get_Double(RulesSection_General, "TreeFlammability", TreeFlammability);
        MissileROTVar = ini.Get_Double(RulesSection_General, "MissileROTVar", MissileROTVar);
        MissileSafetyAltitude = ini.Get_Int(RulesSection_General, "MissileSafetyAltitude", MissileSafetyAltitude);
        MissileSpeedVar = ini.Get_Double(RulesSection_General, "MissileSpeedVar", MissileSpeedVar);
        DropPodWeapon = Get_Type(ini, RulesSection_General, "DropPodWeapon", DropPodWeapon);
        DropPodHeight = ini.Get_Int(RulesSection_General, "DropPodHeight", DropPodHeight);
        DropPodSpeed = ini.Get_Int(RulesSection_General, "DropPodSpeed", DropPodSpeed);
        DropPodAngle = ini.Get_Double(RulesSection_General, "DropPodAngle", DropPodAngle);
        DropPodAngle = Min(DropPodAngle, 1.1780972450961724);
        DropPodAngle = Max(DropPodAngle, 0.39269908169872414);
        CrewEscape = ini.Get_Double(RulesSection_General, "CrewEscape", CrewEscape);
        unknown_20 = ini.Get_Double(RulesSection_General, "TunnelSpeed", unknown_20);
        HoverDampen = ini.Get_Double(RulesSection_General, "HoverDampen", HoverDampen);
        HoverBob = ini.Get_Double(RulesSection_General, "HoverBob", HoverBob);
        HoverHeight = ini.Get_Int(RulesSection_General, "HoverHeight", HoverHeight);
        HoverBoost = ini.Get_Double(RulesSection_General, "HoverBoost", HoverBoost);
        HoverAcceleration = ini.Get_Double(RulesSection_General, "HoverAcceleration", HoverAcceleration);
        HoverBrake = ini.Get_Double(RulesSection_General, "HoverBrake", HoverBrake);
        VeteranRatio = ini.Get_Double(RulesSection_General, "VeteranRatio", VeteranRatio);
        VeteranCombat = ini.Get_Double(RulesSection_General, "VeteranCombat", VeteranCombat);
        unknown_598 = ini.Get_Double(RulesSection_General, "VeteranSpeed", unknown_598);
        unknown_5a0 = ini.Get_Double(RulesSection_General, "VeteranSight", unknown_5a0);
        VeteranArmor = ini.Get_Double(RulesSection_General, "VeteranArmor", VeteranArmor);
        VeteranROF = ini.Get_Double(RulesSection_General, "VeteranROF", VeteranROF);
        VeteranCap = ini.Get_Double(RulesSection_General, "VeteranCap", VeteranCap);
        ExplosiveVoxelDebris = TypeListReader<VoxelAnimTypeClass>::Get(ini, RulesSection_General, "ExplosiveVoxelDebris", ExplosiveVoxelDebris);
        BridgeVoxelMax = ini.Get_Int(RulesSection_General, "BridgeVoxelMax", BridgeVoxelMax);
        TireVoxelDebris = Get_Type(ini, RulesSection_General, "TireVoxelDebris", TireVoxelDebris);
        ScrapVoxelDebris = Get_Type(ini, RulesSection_General, "ScrapVoxelDebris", ScrapVoxelDebris);
        unknown_548 = ini.Get_Int(RulesSection_General, "CloakingStages", unknown_548);
        ShipSinkingWeight = ini.Get_Double(RulesSection_General, "ShipSinkingWeight", ShipSinkingWeight);
        IceCrackingWeight = ini.Get_Double(RulesSection_General, "IceCrackingWeight", IceCrackingWeight);
        IceBreakingWeight = ini.Get_Double(RulesSection_General, "IceBreakingWeight", IceBreakingWeight);
        CliffBackImpassability = ini.Get_Int(RulesSection_General, "CliffBackImpassability", CliffBackImpassability);
        PlacementDelay = ini.Get_Double(RulesSection_General, "PlacementDelay", PlacementDelay);
        TrackedUphill = ini.Get_Double(RulesSection_General, "TrackedUphill", TrackedUphill);
        TrackedDownhill = ini.Get_Double(RulesSection_General, "TrackedDownhill", TrackedDownhill);
        WheeledUphill = ini.Get_Double(RulesSection_General, "WheeledUphill", WheeledUphill);
        WheeledDownhill = ini.Get_Double(RulesSection_General, "WheeledDownhill", WheeledDownhill);
        unknown_6d4 = ini.Get_Int(RulesSection_General, "WindDirection", unknown_6d4);
        CameraRange = ini.Get_Lepton(RulesSection_General, "CameraRange", CameraRange);
        unknown_6dc = ini.Get_Int(RulesSection_General, "FlightLevel", unknown_6dc);
        ParachuteMaxFallRate = ini.Get_Int(RulesSection_General, "ParachuteMaxFallRate", ParachuteMaxFallRate);
        NoParachuteMaxFallRate = ini.Get_Int(RulesSection_General, "NoParachuteMaxFallRate", NoParachuteMaxFallRate);
        unknown_778 = Get_Type(ini, RulesSection_General, "RepairBay", unknown_778);
        unknown_77c = Get_Type(ini, RulesSection_General, "GDIGateOne", unknown_77c);
        unknown_780 = Get_Type(ini, RulesSection_General, "GDIGateTwo", unknown_780);
        unknown_784 = Get_Type(ini, RulesSection_General, "NodGateOne", unknown_784);
        unknown_788 = Get_Type(ini, RulesSection_General, "NodGateTwo", unknown_788);
        unknown_78c = Get_Type(ini, RulesSection_General, "WallTower", unknown_78c);
        Shipyard = TypeListReader<BuildingTypeClass>::Get(ini, RulesSection_General, "Shipyard", Shipyard);
        GDIPowerPlant = Get_Type(ini, RulesSection_General, "GDIPowerPlant", GDIPowerPlant);
        GDIPowerTurbine = Get_Type(ini, RulesSection_General, "GDIPowerTurbine", GDIPowerTurbine);
        NodRegularPower = Get_Type(ini, RulesSection_General, "NodRegularPower", NodRegularPower);
        NodAdvancedPower = Get_Type(ini, RulesSection_General, "NodAdvancedPower", NodAdvancedPower);
        unknown_7bc = Get_Type(ini, RulesSection_General, "GDIFirestormGenerator", unknown_7bc);
        GDIHunterSeeker = Get_Type(ini, RulesSection_General, "GDIHunterSeeker", GDIHunterSeeker);
        NodHunterSeeker = Get_Type(ini, RulesSection_General, "NodHunterSeeker", NodHunterSeeker);
        BaseUnit = TypeListReader<UnitTypeClass>::Get(ini, RulesSection_General, "BaseUnit", BaseUnit);
        unknown_9ec = TypeListReader<UnitTypeClass>::Get(ini, RulesSection_General, "HarvesterUnit", unknown_9ec);
        PadAircraft = TypeListReader<AircraftTypeClass>::Get(ini, RulesSection_General, "PadAircraft", PadAircraft);
        Paratrooper = Get_Type(ini, RulesSection_General, "Paratrooper", Paratrooper);
        ChronoDelay = ini.Get_Int(RulesSection_General, "ChronoDelay", ChronoDelay);
        ChronoReinfDelay = ini.Get_Int(RulesSection_General, "ChronoReinfDelay", ChronoReinfDelay);
        ChronoDistanceFactor = ini.Get_Int(RulesSection_General, "ChronoDistanceFactor", ChronoDistanceFactor);
        ChronoTrigger = ini.Get_Bool(RulesSection_General, "ChronoTrigger", ChronoTrigger);
        ChronoMinimumDelay = ini.Get_Int(RulesSection_General, "ChronoMinimumDelay", ChronoMinimumDelay);
        ChronoRangeMinimum = ini.Get_Int(RulesSection_General, "ChronoRangeMinimum", ChronoRangeMinimum);
        unknown_b6c = Get_Type(ini, RulesSection_General, "AlliedDisguise", unknown_b6c);
        unknown_b70 = Get_Type(ini, RulesSection_General, "SovietDisguise", unknown_b70);
        SpyPowerBlackout = ini.Get_Int(RulesSection_General, "SpyPowerBlackout", SpyPowerBlackout);
        SpyMoneyStealPercent = ini.Get_Double(RulesSection_General, "SpyMoneyStealPercent", SpyMoneyStealPercent);
        AttackCursorOnDisguise = ini.Get_Bool(RulesSection_General, "AttackCursorOnDisguise", AttackCursorOnDisguise);
        unknown_b80 = ini.Get_Double(RulesSection_General, "AIMinorSuperReadyPercent", unknown_b80);
        AISafeDistance = ini.Get_Int(RulesSection_General, "AISafeDistance", AISafeDistance);
        AINavalYardAdjacency = ini.Get_Int(RulesSection_General, "AINavalYardAdjacency", AINavalYardAdjacency);
        MaximumBuildingPlacementFailures = ini.Get_Int(RulesSection_General, "MaximumBuildingPlacementFailures", MaximumBuildingPlacementFailures);
        HarvesterTooFarDistance = ini.Get_Int(RulesSection_General, "HarvesterTooFarDistance", HarvesterTooFarDistance);
        unknown_bec = ini.Get_Int(RulesSection_General, "ApproachTargetResetMultiplier", unknown_bec);
        CampaignMoneyDeltaEasy = ini.Get_Int(RulesSection_General, "CampaignMoneyDeltaEasy", CampaignMoneyDeltaEasy);
        CampaignMoneyDeltaHard = ini.Get_Int(RulesSection_General, "CampaignMoneyDeltaHard", CampaignMoneyDeltaHard);
        GuardAreaTargetingDelay = ini.Get_Int(RulesSection_General, "GuardAreaTargetingDelay", GuardAreaTargetingDelay);
        NormalTargetingDelay = ini.Get_Int(RulesSection_General, "NormalTargetingDelay", NormalTargetingDelay);
        ChronoHarvTooFarDistance = ini.Get_Int(RulesSection_General, "ChronoHarvTooFarDistance", ChronoHarvTooFarDistance);
        AIRestrictReplaceTime = ini.Get_Int(RulesSection_General, "AIRestrictReplaceTime", AIRestrictReplaceTime);
        unknown_be8 = ini.Get_Int(RulesSection_General, "ThreatPerOccupant", unknown_be8);
        PurifierBonus = ini.Get_Double(RulesSection_General, "PurifierBonus", PurifierBonus);
        Engineer = Get_Type(ini, RulesSection_General, "Engineer", Engineer);
        Technician = Get_Type(ini, RulesSection_General, "Technician", Technician);
        Pilot = Get_Type(ini, RulesSection_General, "Pilot", Pilot);
        AlliedCrew = Get_Type(ini, RulesSection_General, "AlliedCrew", AlliedCrew);
        SovietCrew = Get_Type(ini, RulesSection_General, "SovietCrew", SovietCrew);
        CurleyShuffle = ini.Get_Bool(RulesSection_General, "CurleyShuffle", CurleyShuffle);
        FineDiffControl = ini.Get_Bool(RulesSection_General, "FineDiffControl", FineDiffControl);
        TeamDelays = ini.Get_Integer_List(RulesSection_General, "TeamDelays", TeamDelays);
        AlliedBaseDefenseCounts = ini.Get_Integer_List(RulesSection_General, "AlliedBaseDefenseCounts", AlliedBaseDefenseCounts);
        SovietBaseDefenseCounts = ini.Get_Integer_List(RulesSection_General, "SovietBaseDefenseCounts", SovietBaseDefenseCounts);
        DisabledDisguiseDetectionPercent = ini.Get_Integer_List(RulesSection_General, "DisabledDisguiseDetectionPercent", DisabledDisguiseDetectionPercent);
        AIAutoDeployFrameDelay = ini.Get_Integer_List(RulesSection_General, "AIAutoDeployFrameDelay", AIAutoDeployFrameDelay);
        AIPickWallDefensePercent = ini.Get_Integer_List(RulesSection_General, "AIPickWallDefensePercent", AIPickWallDefensePercent);
        AIHateDelays = ini.Get_Integer_List(RulesSection_General, "AIHateDelays", AIHateDelays);
        AIAlternateProductionCreditCutoff = ini.Get_Int(RulesSection_General, "AIAlternateProductionCreditCutoff", AIAlternateProductionCreditCutoff);
        AIUseTurbineUpgradeProbability = ini.Get_Double(RulesSection_General, "AIUseTurbineUpgradeProbability", AIUseTurbineUpgradeProbability);
        NodAIBuildsWalls = ini.Get_Bool(RulesSection_General, "NodAIBuildsWalls", NodAIBuildsWalls);
        AIBuildsWalls = ini.Get_Bool(RulesSection_General, "AIBuildsWalls", AIBuildsWalls);
        FillEarliestTeamProbability = ini.Get_Integer_List(RulesSection_General, "FillEarliestTeamProbability", FillEarliestTeamProbability);
        MinimumAIDefensiveTeams = ini.Get_Integer_List(RulesSection_General, "MinimumAIDefensiveTeams", MinimumAIDefensiveTeams);
        MaximumAIDefensiveTeams = ini.Get_Integer_List(RulesSection_General, "MaximumAIDefensiveTeams", MaximumAIDefensiveTeams);
        TotalAITeamCap = ini.Get_Integer_List(RulesSection_General, "TotalAITeamCap", TotalAITeamCap);
        unknown_1437 = ini.Get_Bool(RulesSection_General, "UseMinDefenseRule", unknown_1437);
        DissolveUnfilledTeamDelay = ini.Get_Int(RulesSection_General, "DissolveUnfilledTeamDelay", DissolveUnfilledTeamDelay);
        MultiplayerAICM = ini.Get_Integer_List(RulesSection_General, "MultiplayerAICM", MultiplayerAICM);
        AIVirtualPurifiers = ini.Get_Integer_List(RulesSection_General, "AIVirtualPurifiers", AIVirtualPurifiers);
        AmerParaDropInf = TypeListReader<InfantryTypeClass>::Get(ini, RulesSection_General, "AmerParaDropInf", AmerParaDropInf);
        AmerParaDropNum = ini.Get_Integer_List(RulesSection_General, "AmerParaDropNum", AmerParaDropNum);
        AllyParaDropInf = TypeListReader<InfantryTypeClass>::Get(ini, RulesSection_General, "AllyParaDropInf", AllyParaDropInf);
        AllyParaDropNum = ini.Get_Integer_List(RulesSection_General, "AllyParaDropNum", AllyParaDropNum);
        SovParaDropInf = TypeListReader<InfantryTypeClass>::Get(ini, RulesSection_General, "SovParaDropInf", SovParaDropInf);
        SovParaDropNum = ini.Get_Integer_List(RulesSection_General, "SovParaDropNum", SovParaDropNum);
        AIIonCannonConYardValue = ini.Get_Integer_List(RulesSection_General, "AIIonCannonConYardValue", AIIonCannonConYardValue);
        AIIonCannonWarFactoryValue = ini.Get_Integer_List(RulesSection_General, "AIIonCannonWarFactoryValue", AIIonCannonWarFactoryValue);
        AIIonCannonPowerValue = ini.Get_Integer_List(RulesSection_General, "AIIonCannonPowerValue", AIIonCannonPowerValue);
        AIIonCannonEngineerValue = ini.Get_Integer_List(RulesSection_General, "AIIonCannonEngineerValue", AIIonCannonEngineerValue);
        AIIonCannonThiefValue = ini.Get_Integer_List(RulesSection_General, "AIIonCannonThiefValue", AIIonCannonThiefValue);
        AIIonCannonHarvesterValue = ini.Get_Integer_List(RulesSection_General, "AIIonCannonHarvesterValue", AIIonCannonHarvesterValue);
        AIIonCannonMCVValue = ini.Get_Integer_List(RulesSection_General, "AIIonCannonMCVValue", AIIonCannonMCVValue);
        AIIonCannonAPCValue = ini.Get_Integer_List(RulesSection_General, "AIIonCannonAPCValue", AIIonCannonAPCValue);
        AIIonCannonBaseDefenseValue = ini.Get_Integer_List(RulesSection_General, "AIIonCannonBaseDefenseValue", AIIonCannonBaseDefenseValue);
        AIIonCannonPlugValue = ini.Get_Integer_List(RulesSection_General, "AIIonCannonPlugValue", AIIonCannonPlugValue);
        AIIonCannonHelipadValue = ini.Get_Integer_List(RulesSection_General, "AIIonCannonHelipadValue", AIIonCannonHelipadValue);
        AIIonCannonTempleValue = ini.Get_Integer_List(RulesSection_General, "AIIonCannonTempleValue", AIIonCannonTempleValue);
        CloakDelay = ini.Get_Double(RulesSection_General, "CloakDelay", CloakDelay);
        GameSpeedBias = ini.Get_Double(RulesSection_General, "GameSpeedBias", GameSpeedBias);
        BaseBias = ini.Get_Double(RulesSection_General, "BaseBias", BaseBias);
        unknown_142c = ini.Get_Bool(RulesSection_General, "SeparateAircraft", unknown_142c);
        BaseDefenseDelay = ini.Get_Double(RulesSection_General, "BaseDefenseDelay", BaseDefenseDelay);
        SuspendPriority = ini.Get_Int(RulesSection_General, "SuspendPriority", SuspendPriority);
        unknown_1150 = ini.Get_Double(RulesSection_General, "SuspendDelay", unknown_1150);
        SurvivorRate = ini.Get_Double(RulesSection_General, "SurvivorRate", SurvivorRate);
        unknown_1160 = ini.Get_Int(RulesSection_General, "AlliedSurvivorDivisor", unknown_1160);
        unknown_1164 = ini.Get_Int(RulesSection_General, "SovietSurvivorDivisor", unknown_1164);
        ReloadRate = ini.Get_Double(RulesSection_General, "ReloadRate", ReloadRate);
        unknown_1178 = ini.Get_Double(RulesSection_General, "BuildupTime", unknown_1178);
        HarvesterDumpRate = ini.Get_Double(RulesSection_General, "HarvesterDumpRate", HarvesterDumpRate);
        unknown_1180 = ini.Get_Int(RulesSection_General, "HarvesterLoadRate", unknown_1180);
        unknown_13b0 = ini.Get_Double(RulesSection_General, "BuildSpeed", unknown_13b0);
        DamageDelay = ini.Get_Double(RulesSection_General, "DamageDelay", DamageDelay);
        GrowthRate = ini.Get_Double(RulesSection_General, "GrowthRate", GrowthRate);
        unknown_13a0 = ini.Get_Double(RulesSection_General, "RefundPercent", unknown_13a0);
        unknown_1338 = ini.Get_Double(RulesSection_General, "RepairPercent", unknown_1338);
        unknown_1334 = ini.Get_Int(RulesSection_General, "RepairStep", unknown_1334);
        unknown_1340 = ini.Get_Int(RulesSection_General, "IRepairStep", unknown_1340);
        unknown_1348 = ini.Get_Double(RulesSection_General, "RepairRate", unknown_1348);
        URepairRate = ini.Get_Double(RulesSection_General, "URepairRate", URepairRate);
        IRepairRate = ini.Get_Double(RulesSection_General, "IRepairRate", IRepairRate);
        unknown_1384 = ini.Get_Lepton(RulesSection_General, "Stray", unknown_1384);
        unknown_1388 = ini.Get_Lepton(RulesSection_General, "RelaxedStray", unknown_1388);
        GuardModeStray = ini.Get_Lepton(RulesSection_General, "GuardModeStray", GuardModeStray);
        unknown_1380 = ini.Get_Lepton(RulesSection_General, "CloseEnough", unknown_1380);
        BlendedFog = ini.Get_Bool(RulesSection_General, "BlendedFog", BlendedFog);
        AttackingAircraftSightRange = ini.Get_Int(RulesSection_General, "AttackingAircraftSightRange", AttackingAircraftSightRange);
        unknown_1324 = ini.Get_Int(RulesSection_General, "LeptonsPerSightIncrease", unknown_1324);
        TiberiumTransmogrify = ini.Get_Int(RulesSection_General, "TiberiumTransmogrify", TiberiumTransmogrify);
        unknown_13e8 = ini.Get_Int(RulesSection_General, "LightningDeferment", unknown_13e8);
        LightningDamage = ini.Get_Int(RulesSection_General, "LightningDamage", LightningDamage);
        LightningStormDuration = ini.Get_Int(RulesSection_General, "LightningStormDuration", LightningStormDuration);
        LightningHitDelay = ini.Get_Int(RulesSection_General, "LightningHitDelay", LightningHitDelay);
        LightningScatterDelay = ini.Get_Int(RulesSection_General, "LightningScatterDelay", LightningScatterDelay);
        LightningCellSpread = ini.Get_Int(RulesSection_General, "LightningCellSpread", LightningCellSpread);
        LightningSeparation = ini.Get_Int(RulesSection_General, "LightningSeparation", LightningSeparation);
        LightningWarhead = Get_Type(ini, RulesSection_General, "LightningWarhead", LightningWarhead);  /* out of line 653da0 */
        LightningPrintText = ini.Get_Bool(RulesSection_General, "LightningPrintText", LightningPrintText);
        PrismType = Get_Type(ini, RulesSection_General, "PrismType", PrismType);  /* out of line 654060 */
        PrismSupportModifier = ini.Get_Double(RulesSection_General, "PrismSupportModifier", PrismSupportModifier) * 100.0;
        PrismSupportMax = ini.Get_Int(RulesSection_General, "PrismSupportMax", PrismSupportMax);
        PrismSupportDelay = ini.Get_Int(RulesSection_General, "PrismSupportDelay", PrismSupportDelay);
        PrismSupportDuration = ini.Get_Int(RulesSection_General, "PrismSupportDuration", PrismSupportDuration);
        PrismSupportHeight = ini.Get_Int(RulesSection_General, "PrismSupportHeight", PrismSupportHeight);
        V3Rocket.PauseFrames = ini.Get_Int(RulesSection_General, "V3RocketPauseFrames", V3Rocket.PauseFrames);
        V3Rocket.TiltFrames = ini.Get_Int(RulesSection_General, "V3RocketTiltFrames", V3Rocket.TiltFrames);
        V3Rocket.PitchInitial = ini.Get_Double(RulesSection_General, "V3RocketPitchInitial", V3Rocket.PitchInitial);
        V3Rocket.PitchFinal = ini.Get_Double(RulesSection_General, "V3RocketPitchFinal", V3Rocket.PitchFinal);
        V3Rocket.TurnRate = ini.Get_Double(RulesSection_General, "V3RocketTurnRate", V3Rocket.TurnRate);
        V3Rocket.Acceleration = ini.Get_Double(RulesSection_General, "V3RocketAcceleration", V3Rocket.Acceleration);
        V3Rocket.Altitude = ini.Get_Int(RulesSection_General, "V3RocketAltitude", V3Rocket.Altitude);
        V3Rocket.Damage = ini.Get_Int(RulesSection_General, "V3RocketDamage", V3Rocket.Damage);
        V3Rocket.EliteDamage = ini.Get_Int(RulesSection_General, "V3RocketEliteDamage", V3Rocket.EliteDamage);
        V3Rocket.BodyLength = ini.Get_Int(RulesSection_General, "V3RocketBodyLength", V3Rocket.BodyLength);
        V3Rocket.LazyCurve = ini.Get_Bool(RulesSection_General, "V3RocketLazyCurve", V3Rocket.LazyCurve);
        V3Rocket.Type = Get_Type(ini, RulesSection_General, "V3RocketType", V3Rocket.Type);  /* out of line 6540b0 */
        DMisl.PauseFrames = ini.Get_Int(RulesSection_General, "DMislPauseFrames", DMisl.PauseFrames);
        DMisl.TiltFrames = ini.Get_Int(RulesSection_General, "DMislTiltFrames", DMisl.TiltFrames);
        DMisl.PitchInitial = ini.Get_Double(RulesSection_General, "DMislPitchInitial", DMisl.PitchInitial);
        DMisl.PitchFinal = ini.Get_Double(RulesSection_General, "DMislPitchFinal", DMisl.PitchFinal);
        DMisl.TurnRate = ini.Get_Double(RulesSection_General, "DMislTurnRate", DMisl.TurnRate);
        DMisl.Acceleration = ini.Get_Double(RulesSection_General, "DMislAcceleration", DMisl.Acceleration);
        DMisl.Altitude = ini.Get_Int(RulesSection_General, "DMislAltitude", DMisl.Altitude);
        DMisl.Damage = ini.Get_Int(RulesSection_General, "DMislDamage", DMisl.Damage);
        DMisl.EliteDamage = ini.Get_Int(RulesSection_General, "DMislEliteDamage", DMisl.EliteDamage);
        DMisl.BodyLength = ini.Get_Int(RulesSection_General, "DMislBodyLength", DMisl.BodyLength);
        DMisl.LazyCurve = ini.Get_Bool(RulesSection_General, "DMislLazyCurve", DMisl.LazyCurve);
        DMisl.Type = Get_Type(ini, RulesSection_General, "DMislType", DMisl.Type);  /* out of line 6540b0 */
        unknown_468 = ini.Get_Int(RulesSection_General, "ParadropRadius", unknown_468);
        unknown_6b0 = ini.Get_Int(RulesSection_General, "SpotlightMovementRadius", unknown_6b0);
        unknown_6b4 = ini.Get_Int(RulesSection_General, "SpotlightLocationRadius", unknown_6b4);
        SpotlightSpeed = ini.Get_Double(RulesSection_General, "SpotlightSpeed", SpotlightSpeed);
        SpotlightAcceleration = ini.Get_Double(RulesSection_General, "SpotlightAcceleration", SpotlightAcceleration);
        SpotlightAngle = ini.Get_Double(RulesSection_General, "SpotlightAngle", SpotlightAngle);
        unknown_6d0 = ini.Get_Int(RulesSection_General, "SpotlightRadius", unknown_6d0);
        RevealTriggerRadius = ini.Get_Int(RulesSection_General, "RevealTriggerRadius", RevealTriggerRadius);
        ChargeToDrainRatio = ini.Get_Double(RulesSection_General, "ChargeToDrainRatio", ChargeToDrainRatio);
        DamageToFirestormDamageCoefficient = ini.Get_Double(RulesSection_General, "DamageToFirestormDamageCoefficient", DamageToFirestormDamageCoefficient);
        WallBuildSpeedCoefficient = ini.Get_Double(RulesSection_General, "WallBuildSpeedCoefficient", WallBuildSpeedCoefficient);
        ConditionYellowSparkingProbability = ini.Get_Double(RulesSection_General, "ConditionYellowSparkingProbability", ConditionYellowSparkingProbability);
        ConditionRedSparkingProbability = ini.Get_Double(RulesSection_General, "ConditionRedSparkingProbability", ConditionRedSparkingProbability);
        unknown_b8 = ini.Get_Double(RulesSection_General, "AITriggerSuccessWeightDelta", unknown_b8);
        unknown_c0 = ini.Get_Double(RulesSection_General, "AITriggerFailureWeightDelta", unknown_c0);
        unknown_c8 = ini.Get_Double(RulesSection_General, "AITriggerTrackRecordCoefficient", unknown_c8);
        unknown_1410 = ini.Get_Int(RulesSection_General, "WeedCapacity", unknown_1410);
        HarvestersPerRefinery = ini.Get_Int(RulesSection_General, "HarvestersPerRefinery", HarvestersPerRefinery);
        FlashFrameTime = ini.Get_Int(RulesSection_General, "FlashFrameTime", FlashFrameTime);
        RadarCombatFlashTime = ini.Get_Int(RulesSection_General, "RadarCombatFlashTime", RadarCombatFlashTime);
        RadarEventSpeed = ini.Get_Double(RulesSection_General, "RadarEventSpeed", RadarEventSpeed);
        RadarEventRotationSpeed = ini.Get_Double(RulesSection_General, "RadarEventRotationSpeed", RadarEventRotationSpeed);
        RadarEventSuppressionDistances = ini.Get_Integer_List(RulesSection_General, "RadarEventSuppressionDistances", RadarEventSuppressionDistances);
        RadarEventVisibilityDurations = ini.Get_Integer_List(RulesSection_General, "RadarEventVisibilityDurations", RadarEventVisibilityDurations);
        RadarEventDurations = ini.Get_Integer_List(RulesSection_General, "RadarEventDurations", RadarEventDurations);
        RadarEventMinRadius = ini.Get_Int(RulesSection_General, "RadarEventMinRadius", RadarEventMinRadius);
        RadarEventColorSpeed = ini.Get_Double(RulesSection_General, "RadarEventColorSpeed", RadarEventColorSpeed);
        HunterSeekerDetonateProximity = ini.Get_Int(RulesSection_General, "HunterSeekerDetonateProximity", HunterSeekerDetonateProximity);
        HunterSeekerDescendProximity = ini.Get_Int(RulesSection_General, "HunterSeekerDescendProximity", HunterSeekerDescendProximity);
        HunterSeekerDescentSpeed = ini.Get_Int(RulesSection_General, "HunterSeekerDescentSpeed", HunterSeekerDescentSpeed);
        HunterSeekerAscentSpeed = ini.Get_Int(RulesSection_General, "HunterSeekerAscentSpeed", HunterSeekerAscentSpeed);
        HunterSeekerEmergeSpeed = ini.Get_Int(RulesSection_General, "HunterSeekerEmergeSpeed", HunterSeekerEmergeSpeed);
        unknown_d00 = ini.Get_Double(RulesSection_General, "MyEffectivenessCoefficientDefault", unknown_d00);
        unknown_d08 = ini.Get_Double(RulesSection_General, "TargetEffectivenessCoefficientDefault", unknown_d08);
        unknown_d10 = ini.Get_Double(RulesSection_General, "TargetSpecialThreatCoefficientDefault", unknown_d10);
        unknown_d18 = ini.Get_Double(RulesSection_General, "TargetStrengthCoefficientDefault", unknown_d18);
        unknown_d20 = ini.Get_Double(RulesSection_General, "TargetDistanceCoefficientDefault", unknown_d20);
        DumbMyEffectivenessCoefficient = ini.Get_Double(RulesSection_General, "DumbMyEffectivenessCoefficient", DumbMyEffectivenessCoefficient);
        DumbTargetEffectivenessCoefficient = ini.Get_Double(RulesSection_General, "DumbTargetEffectivenessCoefficient", DumbTargetEffectivenessCoefficient);
        DumbTargetSpecialThreatCoefficient = ini.Get_Double(RulesSection_General, "DumbTargetSpecialThreatCoefficient", DumbTargetSpecialThreatCoefficient);
        DumbTargetStrengthCoefficient = ini.Get_Double(RulesSection_General, "DumbTargetStrengthCoefficient", DumbTargetStrengthCoefficient);
        DumbTargetDistanceCoefficient = ini.Get_Double(RulesSection_General, "DumbTargetDistanceCoefficient", DumbTargetDistanceCoefficient);
        EnemyHouseThreatBonus = ini.Get_Double(RulesSection_General, "EnemyHouseThreatBonus", EnemyHouseThreatBonus);
        VeinholeMonsterStrength = ini.Get_Int(RulesSection_General, "VeinholeMonsterStrength", VeinholeMonsterStrength);
        unknown_d4 = ini.Get_Int(RulesSection_General, "MaxVeinholeGrowth", unknown_d4);
        VeinholeGrowthRate = ini.Get_Int(RulesSection_General, "VeinholeGrowthRate", VeinholeGrowthRate);
        VeinholeShrinkRate = ini.Get_Int(RulesSection_General, "VeinholeShrinkRate", VeinholeShrinkRate);
        unknown_e4 = ini.Get_Int(RulesSection_General, "VeinDamage", unknown_e4);
        unknown_cb4 = Get_Type(ini, RulesSection_General, "VeinholeTypeClass", unknown_cb4);  /* out of line 654100 */
        DefaultMirageDisguises = TypeListReader<TerrainTypeClass>::Get(ini, RulesSection_General, "DefaultMirageDisguises", DefaultMirageDisguises);
        InfantryBlinkDisguiseTime = ini.Get_Int(RulesSection_General, "InfantryBlinkDisguiseTime", InfantryBlinkDisguiseTime);
        MaximumQueuedObjects = ini.Get_Int(RulesSection_General, "MaximumQueuedObjects", MaximumQueuedObjects);
        unknown_98 = ini.Get_Int(RulesSection_General, "MaxWaypointPathLength", unknown_98);
        unknown_e1c = ini.Get_Int(RulesSection_General, "TreeStrength", unknown_e1c);
        unknown_143c = ini.Get_Double(RulesSection_General, "EngineerCaptureLevel", unknown_143c);
        EngineerCaptureLevel = ini.Get_Double(RulesSection_General, "EngineerCaptureLevel", EngineerCaptureLevel);
        TalkBubbleTime = ini.Get_Double(RulesSection_General, "TalkBubbleTime", TalkBubbleTime * (1.0f / 60)) * 60.0;
        return true;
    }
    return false;
}

#undef Min
#undef Max


// RulesClass::MPlayer (0x0064B690)

bool RulesClass::MPlayer(CCINIClass &ini)
{
    if (ini.Is_Present(RulesSection_MultiplayerDialogSettings)) {
        MinMoney = ini.Get_Int(RulesSection_MultiplayerDialogSettings, "MinMoney", MinMoney);
        Money = ini.Get_Int(RulesSection_MultiplayerDialogSettings, "Money", Money);
        MaxMoney = ini.Get_Int(RulesSection_MultiplayerDialogSettings, "MaxMoney", MaxMoney);
        MoneyIncrement = ini.Get_Int(RulesSection_MultiplayerDialogSettings, "MoneyIncrement", MoneyIncrement);
        MinUnitCount = ini.Get_Int(RulesSection_MultiplayerDialogSettings, "MinUnitCount", MinUnitCount);
        UnitCount = ini.Get_Int(RulesSection_MultiplayerDialogSettings, "UnitCount", UnitCount);
        MaxUnitCount = ini.Get_Int(RulesSection_MultiplayerDialogSettings, "MaxUnitCount", MaxUnitCount);
        TechLevel = ini.Get_Int(RulesSection_MultiplayerDialogSettings, "TechLevel", TechLevel);
        GameSpeed = ini.Get_Int(RulesSection_MultiplayerDialogSettings, "GameSpeed", GameSpeed);
        AIDifficulty = ini.Get_Int(RulesSection_MultiplayerDialogSettings, "AIDifficulty", AIDifficulty);
        AIPlayers = ini.Get_Int(RulesSection_MultiplayerDialogSettings, "AIPlayers", AIPlayers);
        BridgeDestruction = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "BridgeDestruction", BridgeDestruction);
        ShadowGrow = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "ShadowGrow", ShadowGrow);
        Shroud = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "Shroud", Shroud);
        Bases = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "Bases", Bases);
        TiberiumGrows = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "TiberiumGrows", TiberiumGrows);
        Crates = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "Crates", Crates);
        CaptureTheFlag = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "CaptureTheFlag", CaptureTheFlag);
        HarvesterTruce = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "HarvesterTruce", HarvesterTruce);
        MultiEngineer = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "MultiEngineer", MultiEngineer);
        unknown_111d = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "AlliesAllowed", unknown_111d);
        ShortGame = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "ShortGame", ShortGame);
        SuperWeaponsAllowed = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "SuperWeaponsAllowed", SuperWeaponsAllowed);
        BuildOffAlly = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "BuildOffAlly", BuildOffAlly);
        FogOfWar = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "FogOfWar", FogOfWar);
        MCVRedeploys = ini.Get_Bool(RulesSection_MultiplayerDialogSettings, "MCVRedeploys", MCVRedeploys);
        return true;
    }
    return false;
}


// RulesClass::Heap_Maximums (0x0064BA00)

bool RulesClass::Heap_Maximums(CCINIClass &ini)
{
    if (ini.Is_Present(RulesSection_Maximums)) {
        MaxPlayers = ini.Get_Int(RulesSection_Maximums, "Players", MaxPlayers);
    }
    return true;
}


// RulesClass::Infantry (0x0064BA50)
// Makes a InfantryTypeClass for every name listed in [InfantryTypes].

bool RulesClass::Infantry(CCINIClass &ini)
{
    int count = ini.Entry_Count(RulesSection_InfantryTypes);
    for (int index = 0; index < count; index++) {
        char buffer[32];
        if (ini.Get_String(RulesSection_InfantryTypes, ini.Get_Entry(RulesSection_InfantryTypes, index), "", buffer, sizeof(buffer))) {
            InfantryTypeClass::Find_Or_Make(buffer);
        }
    }
    return count > 0;
}


// RulesClass::Countries (0x0064BAC0)
// Makes a HouseTypeClass for every name listed in [Countries].

bool RulesClass::Countries(CCINIClass &ini)
{
    int count = ini.Entry_Count(RulesSection_Countries);
    for (int index = 0; index < count; index++) {
        char buffer[32];
        if (ini.Get_String(RulesSection_Countries, ini.Get_Entry(RulesSection_Countries, index), "", buffer, sizeof(buffer))) {
            HouseTypeClass::Find_Or_Make(buffer);
        }
    }
    return count > 0;
}


// RulesClass::Vehicles (0x0064BB30)
// Makes a UnitTypeClass for every name listed in [VehicleTypes].

bool RulesClass::Vehicles(CCINIClass &ini)
{
    int count = ini.Entry_Count(RulesSection_VehicleTypes);
    for (int index = 0; index < count; index++) {
        char buffer[32];
        if (ini.Get_String(RulesSection_VehicleTypes, ini.Get_Entry(RulesSection_VehicleTypes, index), "", buffer, sizeof(buffer))) {
            UnitTypeClass::Find_Or_Make(buffer);
        }
    }
    return count > 0;
}


// RulesClass::Aircraft (0x0064BBA0)
// Makes a AircraftTypeClass for every name listed in [AircraftTypes].

bool RulesClass::Aircraft(CCINIClass &ini)
{
    int count = ini.Entry_Count(RulesSection_AircraftTypes);
    for (int index = 0; index < count; index++) {
        char buffer[32];
        if (ini.Get_String(RulesSection_AircraftTypes, ini.Get_Entry(RulesSection_AircraftTypes, index), "", buffer, sizeof(buffer))) {
            AircraftTypeClass::Find_Or_Make(buffer);
        }
    }
    return count > 0;
}


// RulesClass::Sides (0x0064BC10)
// Makes (or finds) every side listed in [Sides], reads its house list and
// tells each listed house type its side.
#include "shared/side.hpp"
#include "shared/debug_printf.hpp"

bool RulesClass::Sides(CCINIClass &ini)
{
    int count = ini.Entry_Count(RulesSection_Sides);
    Debug_Printf("Processing sides.\n");
    for (int index = 0; index < count; index++) {
        char const *entry = ini.Get_Entry(RulesSection_Sides, index);
        int found = SideClass::Find_Index(entry);
        SideClass *side;
        if (found == -1) {
            side = new SideClass(entry);
        } else {
            side = ::Sides[found];
        }
        Debug_Printf("Side %d: %s \n", index, (char const *)side->ID);
        side->Houses = ini.Get_House_List(RulesSection_Sides, entry, side->Houses);
        for (int house = 0; house < side->Houses.Count(); house++) {
            int type = side->Houses[house];
            HouseTypes[type]->Side = ::Sides.ID(side);
            Debug_Printf("  %s\n", (char const *)HouseTypes[type]->ID);
        }
    }
    return count > 0;
}


// RulesClass::Super_Weapons (0x0064BDC0)
// Makes a SuperWeaponTypeClass for every name listed in [SuperWeaponTypes].

bool RulesClass::Super_Weapons(CCINIClass &ini)
{
    int count = ini.Entry_Count(RulesSection_SuperWeaponTypes);
    for (int index = 0; index < count; index++) {
        char buffer[32];
        if (ini.Get_String(RulesSection_SuperWeaponTypes, ini.Get_Entry(RulesSection_SuperWeaponTypes, index), "", buffer, sizeof(buffer))) {
            SuperWeaponTypeClass::Find_Or_Make(buffer);
        }
    }
    return count > 0;
}


// RulesClass::Buildings (0x0064BE30)
// Makes a BuildingTypeClass for every name listed in [BuildingTypes].

bool RulesClass::Buildings(CCINIClass &ini)
{
    int count = ini.Entry_Count(RulesSection_BuildingTypes);
    for (int index = 0; index < count; index++) {
        char buffer[32];
        if (ini.Get_String(RulesSection_BuildingTypes, ini.Get_Entry(RulesSection_BuildingTypes, index), "", buffer, sizeof(buffer))) {
            BuildingTypeClass::Find_Or_Make(buffer);
        }
    }
    return count > 0;
}


// RulesClass::Terrains (0x0064BEA0)
// Makes a TerrainTypeClass for every name listed in [TerrainTypes].

bool RulesClass::Terrains(CCINIClass &ini)
{
    int count = ini.Entry_Count(RulesSection_TerrainTypes);
    for (int index = 0; index < count; index++) {
        char buffer[32];
        if (ini.Get_String(RulesSection_TerrainTypes, ini.Get_Entry(RulesSection_TerrainTypes, index), "", buffer, sizeof(buffer))) {
            TerrainTypeClass::Find_Or_Make(buffer);
        }
    }
    return count > 0;
}


// RulesClass::Teams (0x0064BF10)
// Makes (or finds) every team type listed in [TeamTypes] and reads its own section.

bool RulesClass::Teams(CCINIClass &ini)
{
    int count = ini.Entry_Count(RulesSection_Teams);
    for (int index = 0; index < count; index++) {
        char const *entry = ini.Get_Entry(RulesSection_Teams, index);
        char buffer[128];
        TeamTypeClass *team;
        if (ini.Get_String(RulesSection_Teams, entry, "", buffer, sizeof(buffer))) {
            team = TeamTypeClass::Find_Or_Make(buffer);
        } else {
            team = 0;
        }
        team->Read_INI(ini);
    }
    return count > 0;
}


// RulesClass::Smudges (0x0064BFA0)
// Makes a SmudgeTypeClass for every name listed in [SmudgeTypes].

bool RulesClass::Smudges(CCINIClass &ini)
{
    int count = ini.Entry_Count(RulesSection_SmudgeTypes);
    for (int index = 0; index < count; index++) {
        char buffer[32];
        if (ini.Get_String(RulesSection_SmudgeTypes, ini.Get_Entry(RulesSection_SmudgeTypes, index), "", buffer, sizeof(buffer))) {
            SmudgeTypeClass::Find_Or_Make(buffer);
        }
    }
    return count > 0;
}


// RulesClass::Overlays (0x0064C010)
// Makes a OverlayTypeClass for every name listed in [OverlayTypes].

bool RulesClass::Overlays(CCINIClass &ini)
{
    int count = ini.Entry_Count(RulesSection_OverlayTypes);
    for (int index = 0; index < count; index++) {
        char buffer[32];
        if (ini.Get_String(RulesSection_OverlayTypes, ini.Get_Entry(RulesSection_OverlayTypes, index), "", buffer, sizeof(buffer))) {
            OverlayTypeClass::Find_Or_Make(buffer);
        }
    }
    return count > 0;
}


// RulesClass::Animations (0x0064C080)
// Makes a AnimTypeClass for every name listed in [Animations].

bool RulesClass::Animations(CCINIClass &ini)
{
    int count = ini.Entry_Count("Animations");
    for (int index = 0; index < count; index++) {
        char buffer[32];
        if (ini.Get_String("Animations", ini.Get_Entry("Animations", index), "", buffer, sizeof(buffer))) {
            AnimTypeClass::Find_Or_Make(buffer);
        }
    }
    return count > 0;
}


// RulesClass::Voxel_Anims (0x0064C0F0)
// Makes a VoxelAnimTypeClass for every name listed in [VoxelAnims].

bool RulesClass::Voxel_Anims(CCINIClass &ini)
{
    int count = ini.Entry_Count("VoxelAnims");
    for (int index = 0; index < count; index++) {
        char buffer[32];
        if (ini.Get_String("VoxelAnims", ini.Get_Entry("VoxelAnims", index), "", buffer, sizeof(buffer))) {
            VoxelAnimTypeClass::Find_Or_Make(buffer);
        }
    }
    return count > 0;
}


// RulesClass::Warheads (0x0064C160)
// Makes a WarheadTypeClass for every name listed in [Warheads].

bool RulesClass::Warheads(CCINIClass &ini)
{
    int count = ini.Entry_Count("Warheads");
    for (int index = 0; index < count; index++) {
        char buffer[32];
        if (ini.Get_String("Warheads", ini.Get_Entry("Warheads", index), "", buffer, sizeof(buffer))) {
            WarheadTypeClass::Find_Or_Make(buffer);
        }
    }
    return count > 0;
}


// RulesClass::Particles (0x0064C1D0)
// Makes a ParticleTypeClass for every name listed in [Particles].
#include "shared/particle.hpp"

bool RulesClass::Particles(CCINIClass &ini)
{
    int count = ini.Entry_Count("Particles");
    for (int index = 0; index < count; index++) {
        char buffer[32];
        if (ini.Get_String("Particles", ini.Get_Entry("Particles", index), "", buffer, sizeof(buffer))) {
            ParticleTypeClass::Find_Or_Make(buffer);
        }
    }
    return count > 0;
}


// RulesClass::Particle_Systems (0x0064C240)
// Makes a ParticleSystemTypeClass for every name listed in [ParticleSystems].

bool RulesClass::Particle_Systems(CCINIClass &ini)
{
    int count = ini.Entry_Count("ParticleSystems");
    for (int index = 0; index < count; index++) {
        char buffer[32];
        if (ini.Get_String("ParticleSystems", ini.Get_Entry("ParticleSystems", index), "", buffer, sizeof(buffer))) {
            ParticleSystemTypeClass::Find_Or_Make(buffer);
        }
    }
    return count > 0;
}


// bool RulesClass::AI(CCINIClass &ini) (0x0064C2B0)
// Reads [AI]: the build lists, the attack and scan timings and the per-kind
// ratios and limits. The integer and lepton values go through named locals
// (that form, not the inline argument, gives the original inline budget for
// the TypeListReader::Get expansions).

bool RulesClass::AI(CCINIClass &ini)
{
    if (ini.Is_Present(RulesSection_AI)) {
        unknown_7c8 = TypeListReader<BuildingTypeClass>::Get(ini, RulesSection_AI, "BuildConst", unknown_7c8);
        BuildPower = TypeListReader<BuildingTypeClass>::Get(ini, RulesSection_AI, "BuildPower", BuildPower);
        BuildRefinery = TypeListReader<BuildingTypeClass>::Get(ini, RulesSection_AI, "BuildRefinery", BuildRefinery);
        BuildBarracks = TypeListReader<BuildingTypeClass>::Get(ini, RulesSection_AI, "BuildBarracks", BuildBarracks);
        BuildTech = TypeListReader<BuildingTypeClass>::Get(ini, RulesSection_AI, "BuildTech", BuildTech);
        BuildWeapons = TypeListReader<BuildingTypeClass>::Get(ini, RulesSection_AI, "BuildWeapons", BuildWeapons);
        BuildDefense = TypeListReader<BuildingTypeClass>::Get(ini, RulesSection_AI, "BuildDefense", BuildDefense);
        BuildPDefense = TypeListReader<BuildingTypeClass>::Get(ini, RulesSection_AI, "BuildPDefense", BuildPDefense);
        BuildAA = TypeListReader<BuildingTypeClass>::Get(ini, RulesSection_AI, "BuildAA", BuildAA);
        BuildHelipad = TypeListReader<BuildingTypeClass>::Get(ini, RulesSection_AI, "BuildHelipad", BuildHelipad);
        BuildRadar = TypeListReader<BuildingTypeClass>::Get(ini, RulesSection_AI, "BuildRadar", BuildRadar);
        ConcreteWalls = TypeListReader<BuildingTypeClass>::Get(ini, RulesSection_AI, "ConcreteWalls", ConcreteWalls);
        NSGates = TypeListReader<BuildingTypeClass>::Get(ini, RulesSection_AI, "NSGates", NSGates);
        EWGates = TypeListReader<BuildingTypeClass>::Get(ini, RulesSection_AI, "EWGates", EWGates);
        BuildNavalYard = TypeListReader<BuildingTypeClass>::Get(ini, RulesSection_AI, "BuildNavalYard", BuildNavalYard);
        BuildDummy = TypeListReader<BuildingTypeClass>::Get(ini, RulesSection_AI, "BuildDummy", BuildDummy);
        NeutralTechBuildings = TypeListReader<BuildingTypeClass>::Get(ini, RulesSection_AI, "NeutralTechBuildings", NeutralTechBuildings);
        AttackInterval = ini.Get_Double(RulesSection_AI, "AttackInterval", AttackInterval);
        AttackDelay = ini.Get_Double(RulesSection_AI, "AttackDelay", AttackDelay);
        unknown_e28 = ini.Get_Double(RulesSection_AI, "PatrolScan", unknown_e28);
        int credit_reserve = ini.Get_Int(RulesSection_AI, "CreditReserve", CreditReserve);
        CreditReserve = credit_reserve;
        unknown_13c8 = ini.Get_Double(RulesSection_AI, "PathDelay", unknown_13c8);
        int blockage_path_delay = ini.Get_Int(RulesSection_AI, "BlockagePathDelay", unknown_13d0);
        unknown_13d0 = blockage_path_delay;
        int tiberium_near_scan = ini.Get_Lepton(RulesSection_AI, "TiberiumNearScan", TiberiumNearScan);
        TiberiumNearScan = tiberium_near_scan;
        int tiberium_far_scan = ini.Get_Lepton(RulesSection_AI, "TiberiumFarScan", TiberiumFarScan);
        TiberiumFarScan = tiberium_far_scan;
        AutocreateTime = ini.Get_Double(RulesSection_AI, "AutocreateTime", AutocreateTime);
        int infantry_reserve = ini.Get_Int(RulesSection_AI, "InfantryReserve", InfantryReserve);
        InfantryReserve = infantry_reserve;
        int infantry_base_mult = ini.Get_Int(RulesSection_AI, "InfantryBaseMult", InfantryBaseMult);
        InfantryBaseMult = infantry_base_mult;
        int power_surplus = ini.Get_Int(RulesSection_AI, "PowerSurplus", PowerSurplus);
        PowerSurplus = power_surplus;
        int base_size_add = ini.Get_Int(RulesSection_AI, "BaseSizeAdd", BaseSizeAdd);
        BaseSizeAdd = base_size_add;
        RefineryRatio = ini.Get_Double(RulesSection_AI, "RefineryRatio", RefineryRatio);
        int refinery_limit = ini.Get_Int(RulesSection_AI, "RefineryLimit", RefineryLimit);
        RefineryLimit = refinery_limit;
        BarracksRatio = ini.Get_Double(RulesSection_AI, "BarracksRatio", BarracksRatio);
        int barracks_limit = ini.Get_Int(RulesSection_AI, "BarracksLimit", BarracksLimit);
        BarracksLimit = barracks_limit;
        WarRatio = ini.Get_Double(RulesSection_AI, "WarRatio", WarRatio);
        int war_limit = ini.Get_Int(RulesSection_AI, "WarLimit", WarLimit);
        WarLimit = war_limit;
        DefenseRatio = ini.Get_Double(RulesSection_AI, "DefenseRatio", DefenseRatio);
        int defense_limit = ini.Get_Int(RulesSection_AI, "DefenseLimit", DefenseLimit);
        DefenseLimit = defense_limit;
        AARatio = ini.Get_Double(RulesSection_AI, "AARatio", AARatio);
        int aa_limit = ini.Get_Int(RulesSection_AI, "AALimit", AALimit);
        AALimit = aa_limit;
        TeslaRatio = ini.Get_Double(RulesSection_AI, "TeslaRatio", TeslaRatio);
        int tesla_limit = ini.Get_Int(RulesSection_AI, "TeslaLimit", TeslaLimit);
        TeslaLimit = tesla_limit;
        HelipadRatio = ini.Get_Double(RulesSection_AI, "HelipadRatio", HelipadRatio);
        int helipad_limit = ini.Get_Int(RulesSection_AI, "HelipadLimit", HelipadLimit);
        HelipadLimit = helipad_limit;
        AirstripRatio = ini.Get_Double(RulesSection_AI, "AirstripRatio", AirstripRatio);
        int airstrip_limit = ini.Get_Int(RulesSection_AI, "AirstripLimit", AirstripLimit);
        AirstripLimit = airstrip_limit;
        CompEasyBonus = ini.Get_Bool(RulesSection_AI, "CompEasyBonus", CompEasyBonus);
        Paranoid = ini.Get_Bool(RulesSection_AI, "Paranoid", Paranoid);
        PowerEmergency = ini.Get_Double(RulesSection_AI, "PowerEmergency", PowerEmergency);
        int ai_base_spacing = ini.Get_Int(RulesSection_AI, "AIBaseSpacing", AIBaseSpacing);
        AIBaseSpacing = ai_base_spacing;
        GDIWallDefense = ini.Get_Double(RulesSection_AI, "GDIWallDefense", GDIWallDefense);
        GDIWallDefenseCoefficient = ini.Get_Double(RulesSection_AI, "GDIWallDefenseCoefficient", GDIWallDefenseCoefficient);
        NodBaseDefenseCoefficient = ini.Get_Double(RulesSection_AI, "NodBaseDefenseCoefficient", NodBaseDefenseCoefficient);
        GDIBaseDefenseCoefficient = ini.Get_Double(RulesSection_AI, "GDIBaseDefenseCoefficient", GDIBaseDefenseCoefficient);
        int maximum_base_defense_value = ini.Get_Int(RulesSection_AI, "MaximumBaseDefenseValue", MaximumBaseDefenseValue);
        MaximumBaseDefenseValue = maximum_base_defense_value;
        int computer_base_defense_response = ini.Get_Int(RulesSection_AI, "ComputerBaseDefenseResponse", ComputerBaseDefenseResponse);
        ComputerBaseDefenseResponse = computer_base_defense_response;
        return true;
    }
    return false;
}


// RulesClass::Powerups (0x0064D490)
// Each crate kind's [Powerups] entry: "share, animation, water (yes/no), data";
// a data value with a percent sign is a fraction (times 0.01).
#include "shared/crt_tokens.hpp"

extern "C" int __cdecl _stricmp(char const *left, char const *right);
extern "C" double __cdecl atof(char const *string);

bool RulesClass::Powerups(CCINIClass &ini)
{
    if (ini.Is_Present(RulesSection_Powerups)) {
        for (int crate = 0; crate < 19; crate++) {
            char buffer[128];
            if (ini.Get_String(RulesSection_Powerups, CrateNames[crate], "0,NONE", buffer, sizeof(buffer))) {
                char *token = strtok(buffer, ",");
                if (token != 0) {
                    strtrim(token);
                    CrateShares[crate] = atoi(token);
                }
                token = strtok(0, ",");
                if (token != 0) {
                    strtrim(token);
                    CrateAnims[crate] = Anim_From_Name(token);
                }
                token = strtok(0, ",");
                if (token != 0) {
                    strtrim(token);
                    if (_stricmp(token, "yes") == 0) {
                        CrateWater[crate] = true;
                    } else if (_stricmp(token, "no") == 0) {
                        CrateWater[crate] = false;
                    }
                }
                token = strtok(0, ",");
                if (token != 0) {
                    if (strchr(token, '%') != 0) {
                        CrateData[crate] = atof(token) * 0.01;
                    } else {
                        strtrim(token);
                        CrateData[crate] = atof(token);
                    }
                }
            }
        }
        return true;
    }
    return false;
}


// RulesClass::Land_Types (0x0064D610)
// Reads each land type's section: the speed factor of each movement type
// (at most 1.0; Winged is not read) and whether it is buildable.

// Westwood-style macro: the value expression is evaluated twice.
#define Min(a, b) ((a) < (b) ? (a) : (b))

static char const * const LandNames[12] = {"Clear", "Road", "Water", "Rock", "Wall", "Tiberium", "Beach", "Rough", "Ice", "Railroad", "Tunnel", "Weeds"};

bool RulesClass::Land_Types(CCINIClass &ini)
{
    for (int land = 0; land < 12; land++) {
        GroundType &ground = Ground[land];
        if (ini.Is_Present(LandNames[land])) {
            ground.Cost[3] = Min(ini.Get_Double(LandNames[land], "Hover", 1.0), 1.0);
            ground.Cost[0] = Min(ini.Get_Double(LandNames[land], "Foot", 1.0), 1.0);
            ground.Cost[1] = Min(ini.Get_Double(LandNames[land], "Track", 1.0), 1.0);
            ground.Cost[2] = Min(ini.Get_Double(LandNames[land], "Wheel", 1.0), 1.0);
            ground.Cost[4] = 1.0;
            ground.Cost[5] = Min(ini.Get_Double(LandNames[land], "Float", 1.0), 1.0);
            ground.Cost[6] = Min(ini.Get_Double(LandNames[land], "Amphibious", 1.0), 1.0);
            ground.Cost[7] = Min(ini.Get_Double(LandNames[land], "FloatBeach", 1.0), 1.0);
            ground.Build = ini.Get_Bool(LandNames[land], "Buildable", false);
        }
    }
    return true;
}

#undef Min


// RulesClass::IQ (0x0064D850)

bool RulesClass::IQ(CCINIClass &ini)
{
    if (ini.Is_Present(RulesSection_IQ)) {
        MaxIQ = ini.Get_Int(RulesSection_IQ, "MaxIQLevels", MaxIQ);
        IQSuperWeapons = ini.Get_Int(RulesSection_IQ, "SuperWeapons", IQSuperWeapons);
        IQProduction = ini.Get_Int(RulesSection_IQ, "Production", IQProduction);
        unknown_10a8 = ini.Get_Int(RulesSection_IQ, "GuardArea", unknown_10a8);
        IQRepairSell = ini.Get_Int(RulesSection_IQ, "RepairSell", IQRepairSell);
        unknown_10b0 = ini.Get_Int(RulesSection_IQ, "AutoCrush", unknown_10b0);
        IQScatter = ini.Get_Int(RulesSection_IQ, "Scatter", IQScatter);
        unknown_10b8 = ini.Get_Int(RulesSection_IQ, "ContentScan", unknown_10b8);
        IQAircraft = ini.Get_Int(RulesSection_IQ, "Aircraft", IQAircraft);
        IQHarvester = ini.Get_Int(RulesSection_IQ, "Harvester", IQHarvester);
        IQSellBack = ini.Get_Int(RulesSection_IQ, "SellBack", IQSellBack);
        return true;
    }
    return false;
}


// RulesClass::Jumpjet_Controls (0x0064D9E0)

bool RulesClass::Jumpjet_Controls(CCINIClass &ini)
{
    if (ini.Is_Present(RulesSection_JumpjetControls)) {
        unknown_368 = ini.Get_Int(RulesSection_JumpjetControls, "TurnRate", unknown_368);
        Speed = ini.Get_Int(RulesSection_JumpjetControls, "Speed", Speed);
        Climb = ini.Get_Double(RulesSection_JumpjetControls, "Climb", Climb);
        unknown_378 = ini.Get_Int(RulesSection_JumpjetControls, "CruiseHeight", unknown_378);
        Acceleration = ini.Get_Double(RulesSection_JumpjetControls, "Acceleration", Acceleration);
        WobblesPerSecond = ini.Get_Double(RulesSection_JumpjetControls, "WobblesPerSecond", WobblesPerSecond);
        WobbleDeviation = ini.Get_Int(RulesSection_JumpjetControls, "WobbleDeviation", WobbleDeviation);
        return true;
    }
    return false;
}


// RulesClass::Difficulty (0x0064DB10)
// Reads the [Easy], [Normal] and [Difficult] modifiers (Difficulty_Get 0x0064DB60).

void __fastcall Difficulty_Get(CCINIClass &ini, DifficultyClass &diff, char const *section);

bool RulesClass::Difficulty(CCINIClass &ini)
{
    Difficulty_Get(ini, Diff[0], "Easy");
    Difficulty_Get(ini, Diff[1], "Normal");
    Difficulty_Get(ini, Diff[2], "Difficult");
    return true;
}


// Difficulty_Get (0x0064DB60): one difficulty level's section into a DifficultyClass
// (__fastcall: the INI in ECX, the record in EDX). Called by RulesClass::Difficulty.

void __fastcall Difficulty_Get(CCINIClass &ini, DifficultyClass &diff, char const *section)
{
    if (ini.Is_Present(section)) {
        diff.FirePower = ini.Get_Double(section, "FirePower", 1.0);
        diff.GroundSpeed = ini.Get_Double(section, "Groundspeed", 1.0);
        diff.AirSpeed = ini.Get_Double(section, "Airspeed", 1.0);
        diff.Armor = ini.Get_Double(section, "Armor", 1.0);
        diff.ROF = ini.Get_Double(section, "ROF", 1.0);
        diff.Cost = ini.Get_Double(section, "Cost", 1.0);
        diff.RepairDelay = ini.Get_Double(section, "RepairDelay", 0.02);
        diff.BuildDelay = ini.Get_Double(section, "BuildDelay", 0.03);
        diff.BuildSlowdown = ini.Get_Bool(section, "BuildSlowdown", false);
        diff.BuildTime = ini.Get_Double(section, "BuildTime", 1.0);
        diff.DestroyWalls = ini.Get_Bool(section, "DestroyWalls", true);
        diff.ContentScan = ini.Get_Bool(section, "ContentScan", false);
    }
}


// RulesClass::Movies (0x0064DC90)
// Adds every movie named in [Movies] (a copy of the name) to the movie name
// list unless it is already there.
#include "shared/ccini_extra.hpp"

extern "C" char *__cdecl _strdup(char const *string);

bool RulesClass::Movies(CCINIClass &ini)
{
    if (ini.Is_Present(RulesSection_Movies)) {
        int count = ini.Entry_Count(RulesSection_Movies);
        for (int index = 0; index < count; index++) {
            char buffer[32];
            if (ini.Get_String(RulesSection_Movies, ini.Get_Entry(RulesSection_Movies, index), "<none>", buffer, sizeof(buffer))) {
                if (unknown_name_index_482e60(buffer) == -1) {
                    unknown_names_a71050.Add(_strdup(buffer));
                }
            }
        }
        return true;
    }
    return false;
}


// RulesClass::Advanced_Command_Bar (0x0064DD90)
// Lays out the advanced command bar from ButtonList= of [AdvancedCommandBar]
// ([MultiplayerAdvancedCommandBar] in multiplayer): every slot is cleared,
// then each listed command name takes the next slot.
#include "shared/tab.hpp"

bool RulesClass::Advanced_Command_Bar(CCINIClass &ini, bool multiplayer)
{
    char const *section = multiplayer ? RulesSection_MultiplayerAdvancedCommandBar : RulesSection_AdvancedCommandBar;
    if (ini.Is_Present(section)) {
        for (int slot = 0; slot < 25; slot++) {
            TabClass::Unknown_006A05E0(slot, unknown_no_command_7f67f4);
        }
        char buffer[512];
        if (ini.Get_String(section, RulesSection_ButtonList, RulesSection_None, buffer, sizeof(buffer))) {
            int count = 0;
            int position = 0;
            for (char *token = strtok(buffer, ","); token != 0; token = strtok(0, ",")) {
                int command = TabClass::Unknown_006A0580(token);
                if (command != unknown_no_command_7f67f4) {
                    TabClass::Unknown_006A05E0(command, position);
                }
                count++;
                position++;
            }
            TabClass::Unknown_006A0670(count);
        }
        return true;
    }
    return false;
}


// RulesClass::Save (0x0064DE70)
// Writes the object, then the elements of each list.

void RulesClass::Save(IStream *stream)
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
    MultiplayerAICM.Save(stream);
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
    unknown_7c8.Save(stream);
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
    unknown_9ec.Save(stream);
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


// RulesClass::Load (0x0064E6B0)
// Empties the lists, reads the object, rebuilds it in place (the lists and colours),
// reads each list, then asks for every pointer to be fixed up.
#include "shared/abstract.hpp"

void RulesClass::Load(IStream *stream)
{
    int index;
    int count;
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
    AlliedBaseDefenseCounts.Clear();
    SovietBaseDefenseCounts.Clear();
    DisabledDisguiseDetectionPercent.Clear();
    AIAutoDeployFrameDelay.Clear();
    AIPickWallDefensePercent.Clear();
    MinimumAIDefensiveTeams.Clear();
    MaximumAIDefensiveTeams.Clear();
    MultiplayerAICM.Clear();
    AIVirtualPurifiers.Clear();
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
    PrerequisiteProc.Clear();
    CreditTicks.Clear();
    LightningSounds.Clear();
    Scorches.Clear();
    Scorches1.Clear();
    Scorches2.Clear();
    Scorches3.Clear();
    Scorches4.Clear();
    Shipyard.Clear();
    unknown_7c8.Clear();
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
    BuildNavalYard.Clear();
    BuildDummy.Clear();
    NeutralTechBuildings.Clear();
    PadAircraft.Clear();
    OnFire.Clear();
    TreeFire.Clear();
    SplashList.Clear();
    BaseUnit.Clear();
    DefaultMirageDisguises.Clear();
    unknown_9ec.Clear();
    IceCrackSounds.Clear();
    HSBuilding.Clear();
    AmerParaDropInf.Clear();
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
    AlliedBaseDefenseCounts.Load(stream);
    SovietBaseDefenseCounts.Load(stream);
    DisabledDisguiseDetectionPercent.Load(stream);
    AIAutoDeployFrameDelay.Load(stream);
    AIPickWallDefensePercent.Load(stream);
    MinimumAIDefensiveTeams.Load(stream);
    MaximumAIDefensiveTeams.Load(stream);
    MultiplayerAICM.Load(stream);
    AIVirtualPurifiers.Load(stream);
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
    PrerequisiteProc.Load(stream);
    CreditTicks.Load(stream);
    LightningSounds.Load(stream);
    Scorches.Load(stream);
    Scorches1.Load(stream);
    Scorches2.Load(stream);
    Scorches3.Load(stream);
    Scorches4.Load(stream);
    Shipyard.Load(stream);
    unknown_7c8.Load(stream);
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
    BuildNavalYard.Load(stream);
    BuildDummy.Load(stream);
    NeutralTechBuildings.Load(stream);
    PadAircraft.Load(stream);
    OnFire.Load(stream);
    TreeFire.Load(stream);
    SplashList.Load(stream);
    BaseUnit.Load(stream);
    DefaultMirageDisguises.Load(stream);
    unknown_9ec.Load(stream);
    DeadBodies.Load(stream);
    DropPod.Load(stream);
    MetallicDebris.Load(stream);
    BridgeExplosions.Load(stream);
    IceCrackSounds.Load(stream);
    HSBuilding.Load(stream);
    AmerParaDropInf.Load(stream);
    AmerParaDropNum.Load(stream);
    AllyParaDropInf.Load(stream);
    AllyParaDropNum.Load(stream);
    SovParaDropInf.Load(stream);
    SovParaDropNum.Load(stream);
    DamageFireTypes.Load(stream);
    WeatherConClouds.Load(stream);
    WeatherConBolts.Load(stream);
    Swizzle_Pointer(unknown_c60);
    Swizzle_Pointer(FlameDamage);
    Swizzle_Pointer(FlameDamage2);
    Swizzle_Pointer(LargeVisceroid);
    Swizzle_Pointer(SmallVisceroid);
    Swizzle_Pointer(UnloadingHarvester);
    Swizzle_Pointer(DropPodWeapon);
    Swizzle_Pointer(EMPulseWarhead);
    Swizzle_Pointer(unknown_c74);
    Swizzle_Pointer(V3Warhead);
    Swizzle_Pointer(DMislWarhead);
    Swizzle_Pointer(V3EliteWarhead);
    Swizzle_Pointer(DMislEliteWarhead);
    Swizzle_Pointer(V3Rocket.Type);
    Swizzle_Pointer(DMisl.Type);
    Swizzle_Pointer(PrismType);
    Swizzle_Pointer(IvanWarhead);
    Swizzle_Pointer(DeathWeapon);
    Swizzle_Pointer(IonCannonWarhead);
    Swizzle_Pointer(unknown_cac);
    Swizzle_Pointer(unknown_cb0);
    Swizzle_Pointer(unknown_cf4);
    Swizzle_Pointer(DefaultLargeGreySmokeSystem);
    Swizzle_Pointer(DefaultSmallGreySmokeSystem);
    Swizzle_Pointer(unknown_ce0);
    Swizzle_Pointer(DefaultLargeRedSmokeSystem);
    Swizzle_Pointer(DefaultSmallRedSmokeSystem);
    Swizzle_Pointer(DefaultDebrisSmokeSystem);
    Swizzle_Pointer(DefaultFireStreamSystem);
    Swizzle_Pointer(DefaultTestParticleSystem);
    Swizzle_Pointer(DefaultRepairParticleSystem);
    Swizzle_Pointer(unknown_cb4);
    Swizzle_Pointer(WeatherConBoltExplosion);
    Swizzle_Pointer(Smoke);
    Swizzle_Pointer(unknown_a60);
    Swizzle_Pointer(unknown_a74);
    Swizzle_Pointer(unknown_a78);
    Swizzle_Pointer(unknown_a7c);
    Swizzle_Pointer(SmallFire);
    Swizzle_Pointer(LargeFire);
    Swizzle_Pointer(DropZoneAnim);
    Swizzle_Pointer(UnitCrateType);
    Swizzle_Pointer(Paratrooper);
    Swizzle_Pointer(unknown_b6c);
    Swizzle_Pointer(unknown_b70);
    Swizzle_Pointer(Technician);
    Swizzle_Pointer(Engineer);
    Swizzle_Pointer(Pilot);
    Swizzle_Pointer(AlliedCrew);
    Swizzle_Pointer(SovietCrew);
    Swizzle_Pointer(unknown_778);
    Swizzle_Pointer(unknown_77c);
    Swizzle_Pointer(unknown_780);
    Swizzle_Pointer(unknown_784);
    Swizzle_Pointer(unknown_788);
    Swizzle_Pointer(unknown_78c);
    Swizzle_Pointer(GDIPowerPlant);
    Swizzle_Pointer(GDIPowerTurbine);
    Swizzle_Pointer(NodRegularPower);
    Swizzle_Pointer(NodAdvancedPower);
    Swizzle_Pointer(unknown_7bc);
    Swizzle_Pointer(GDIHunterSeeker);
    Swizzle_Pointer(NodHunterSeeker);
    Swizzle_Pointer(NukeProjectile);
    Swizzle_Pointer(NukeDown);
    Swizzle_Pointer(EMPulseProjectile);
    Swizzle_Pointer(TireVoxelDebris);
    Swizzle_Pointer(ScrapVoxelDebris);
    Swizzle_Pointer(AtmosphereEntry);
    Swizzle_Pointer(InfantryExplode);
    Swizzle_Pointer(IonBlast);
    Swizzle_Pointer(ChronoBeam);
    Swizzle_Pointer(ChronoBlast);
    Swizzle_Pointer(ChronoBlastDest);
    Swizzle_Pointer(ChronoPlacement);
    Swizzle_Pointer(IonBeam);
    Swizzle_Pointer(WarpIn);
    Swizzle_Pointer(WarpOut);
    Swizzle_Pointer(WarpAway);
    Swizzle_Pointer(unknown_2a8);
    Swizzle_Pointer(IronCurtainInvokeAnim);
    Swizzle_Pointer(WeaponNullifyAnim);
    Swizzle_Pointer(unknown_228);
    Swizzle_Pointer(FirestormIdleAnim);
    Swizzle_Pointer(FirestormAirAnim);
    Swizzle_Pointer(FirestormGroundAnim);
    Swizzle_Pointer(FirestormActiveAnim);
    Swizzle_Pointer(unknown_1438);
    Swizzle_Pointer(unknown_09c);
    Swizzle_Pointer(OreTwinkle);
    Swizzle_Pointer(FlamingInfantry);
    Swizzle_Pointer(NukeTakeOff);
    Swizzle_Pointer(InfantryHeadPop);
    Swizzle_Pointer(InfantryNuked);
    Swizzle_Pointer(Behind);
    Swizzle_Pointer(VeinAttack);
    Swizzle_Pointer(BarrelExplode);
    Swizzle_Pointer(BarrelParticle);
    Swizzle_Pointer(DropPodPuff);
    Swizzle_Pointer(LightningWarhead);
    Swizzle_Pointer(unknown_147c);
    Swizzle_Pointer(unknown_f0);
    Swizzle_Pointer(unknown_f4);
    Swizzle_Pointer(unknown_f8);
    BombCursorShape = MixFileClass::Retrieve("BOMBCURS.SHP");
    for (index = 0; index < BarrelDebris.Count(); index++) {
        Swizzle_Pointer(BarrelDebris[index]);
    }
    for (index = 0; index < OnFire.Count(); index++) {
        Swizzle_Pointer(OnFire[index]);
    }
    for (index = 0; index < TreeFire.Count(); index++) {
        Swizzle_Pointer(TreeFire[index]);
    }
    for (index = 0; index < SplashList.Count(); index++) {
        Swizzle_Pointer(SplashList[index]);
    }
    for (index = 0; index < Scorches.Count(); index++) {
        Swizzle_Pointer(Scorches[index]);
    }
    for (index = 0; index < Scorches1.Count(); index++) {
        Swizzle_Pointer(Scorches1[index]);
    }
    for (index = 0; index < Scorches2.Count(); index++) {
        Swizzle_Pointer(Scorches2[index]);
    }
    for (index = 0; index < Scorches3.Count(); index++) {
        Swizzle_Pointer(Scorches3[index]);
    }
    for (index = 0; index < Scorches4.Count(); index++) {
        Swizzle_Pointer(Scorches4[index]);
    }
    for (index = 0; index < BaseUnit.Count(); index++) {
        Swizzle_Pointer(BaseUnit[index]);
    }
    for (index = 0; index < DefaultMirageDisguises.Count(); index++) {
        Swizzle_Pointer(DefaultMirageDisguises[index]);
    }
    for (index = 0; index < unknown_9ec.Count(); index++) {
        Swizzle_Pointer(unknown_9ec[index]);
    }
    for (index = 0; index < Shipyard.Count(); index++) {
        Swizzle_Pointer(Shipyard[index]);
    }
    for (index = 0; index < unknown_7c8.Count(); index++) {
        Swizzle_Pointer(unknown_7c8[index]);
    }
    for (index = 0; index < BuildPower.Count(); index++) {
        Swizzle_Pointer(BuildPower[index]);
    }
    for (index = 0; index < BuildRefinery.Count(); index++) {
        Swizzle_Pointer(BuildRefinery[index]);
    }
    for (index = 0; index < BuildBarracks.Count(); index++) {
        Swizzle_Pointer(BuildBarracks[index]);
    }
    for (index = 0; index < BuildTech.Count(); index++) {
        Swizzle_Pointer(BuildTech[index]);
    }
    for (index = 0; index < BuildWeapons.Count(); index++) {
        Swizzle_Pointer(BuildWeapons[index]);
    }
    for (index = 0; index < BuildDefense.Count(); index++) {
        Swizzle_Pointer(BuildDefense[index]);
    }
    for (index = 0; index < BuildPDefense.Count(); index++) {
        Swizzle_Pointer(BuildPDefense[index]);
    }
    for (index = 0; index < BuildAA.Count(); index++) {
        Swizzle_Pointer(BuildAA[index]);
    }
    for (index = 0; index < BuildHelipad.Count(); index++) {
        Swizzle_Pointer(BuildHelipad[index]);
    }
    for (index = 0; index < BuildRadar.Count(); index++) {
        Swizzle_Pointer(BuildRadar[index]);
    }
    for (index = 0; index < ConcreteWalls.Count(); index++) {
        Swizzle_Pointer(ConcreteWalls[index]);
    }
    for (index = 0; index < NSGates.Count(); index++) {
        Swizzle_Pointer(NSGates[index]);
    }
    for (index = 0; index < EWGates.Count(); index++) {
        Swizzle_Pointer(EWGates[index]);
    }
    for (index = 0; index < BuildNavalYard.Count(); index++) {
        Swizzle_Pointer(BuildNavalYard[index]);
    }
    for (index = 0; index < BuildDummy.Count(); index++) {
        Swizzle_Pointer(BuildDummy[index]);
    }
    for (index = 0; index < NeutralTechBuildings.Count(); index++) {
        Swizzle_Pointer(NeutralTechBuildings[index]);
    }
    for (index = 0; index < PadAircraft.Count(); index++) {
        Swizzle_Pointer(PadAircraft[index]);
    }
    for (index = 0; index < ExplosiveVoxelDebris.Count(); index++) {
        Swizzle_Pointer(ExplosiveVoxelDebris[index]);
    }
    for (index = 0; index < DeadBodies.Count(); index++) {
        Swizzle_Pointer(DeadBodies[index]);
    }
    for (index = 0; index < DropPod.Count(); index++) {
        Swizzle_Pointer(DropPod[index]);
    }
    for (index = 0; index < MetallicDebris.Count(); index++) {
        Swizzle_Pointer(MetallicDebris[index]);
    }
    for (index = 0; index < BridgeExplosions.Count(); index++) {
        Swizzle_Pointer(BridgeExplosions[index]);
    }
    for (index = 0; index < HSBuilding.Count(); index++) {
        Swizzle_Pointer(HSBuilding[index]);
    }
    count = WeatherConClouds.Count();
    for (index = 0; index < count; index++) {
        Swizzle_Pointer(WeatherConClouds[index]);
    }
    count = WeatherConBolts.Count();
    for (index = 0; index < count; index++) {
        Swizzle_Pointer(WeatherConBolts[index]);
    }
    for (index = 0; index < AmerParaDropInf.Count(); index++) {
        Swizzle_Pointer(AmerParaDropInf[index]);
    }
    for (index = 0; index < AllyParaDropInf.Count(); index++) {
        Swizzle_Pointer(AllyParaDropInf[index]);
    }
    for (index = 0; index < SovParaDropInf.Count(); index++) {
        Swizzle_Pointer(SovParaDropInf[index]);
    }
    count = DamageFireTypes.Count();
    for (index = 0; index < count; index++) {
        Swizzle_Pointer(DamageFireTypes[index]);
    }
}


// RulesClass::Detach (0x00651290)
// Forgets an object being removed: each rules reference to it is cleared and it
// leaves each list.
void RulesClass::Detach(AbstractClass const *target, bool all)
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
    if (target == unknown_09c) {
        unknown_09c = 0;
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
    if (target == unknown_c60) {
        unknown_c60 = 0;
    }
    if (target == EMPulseWarhead) {
        EMPulseWarhead = 0;
    }
    if (target == unknown_c74) {
        unknown_c74 = 0;
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
    if (target == unknown_cac) {
        unknown_cac = 0;
    }
    if (target == unknown_1438) {
        unknown_1438 = 0;
    }
    if (target == unknown_cb0) {
        unknown_cb0 = 0;
    }
    if (target == unknown_cf4) {
        unknown_cf4 = 0;
    }
    if (target == DefaultLargeGreySmokeSystem) {
        DefaultLargeGreySmokeSystem = 0;
    }
    if (target == DefaultSmallGreySmokeSystem) {
        DefaultSmallGreySmokeSystem = 0;
    }
    if (target == unknown_ce0) {
        unknown_ce0 = 0;
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
    if (target == unknown_cb4) {
        unknown_cb4 = 0;
    }
    if (target == WeatherConBoltExplosion) {
        WeatherConBoltExplosion = 0;
    }
    if (target == Smoke) {
        Smoke = 0;
    }
    if (target == unknown_a60) {
        unknown_a60 = 0;
    }
    if (target == unknown_a74) {
        unknown_a74 = 0;
    }
    if (target == unknown_a78) {
        unknown_a78 = 0;
    }
    if (target == unknown_a7c) {
        unknown_a7c = 0;
    }
    if (target == SmallFire) {
        SmallFire = 0;
    }
    if (target == LargeFire) {
        LargeFire = 0;
    }
    if (target == DropZoneAnim) {
        DropZoneAnim = 0;
    }
    if (target == UnitCrateType) {
        UnitCrateType = 0;
    }
    if (target == Paratrooper) {
        Paratrooper = 0;
    }
    if (target == unknown_b6c) {
        unknown_b6c = 0;
    }
    if (target == unknown_b70) {
        unknown_b70 = 0;
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
    if (target == unknown_778) {
        unknown_778 = 0;
    }
    if (target == unknown_77c) {
        unknown_77c = 0;
    }
    if (target == unknown_780) {
        unknown_780 = 0;
    }
    if (target == unknown_784) {
        unknown_784 = 0;
    }
    if (target == unknown_788) {
        unknown_788 = 0;
    }
    if (target == unknown_78c) {
        unknown_78c = 0;
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
    if (target == unknown_7bc) {
        unknown_7bc = 0;
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
    if (target == unknown_2a8) {
        unknown_2a8 = 0;
    }
    if (target == IronCurtainInvokeAnim) {
        IronCurtainInvokeAnim = 0;
    }
    if (target == WeaponNullifyAnim) {
        WeaponNullifyAnim = 0;
    }
    if (target == unknown_228) {
        unknown_228 = 0;
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
    if (target == unknown_147c) {
        unknown_147c = 0;
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
    unknown_9ec.Delete((UnitTypeClass *)target);
    Shipyard.Delete((BuildingTypeClass *)target);
    unknown_7c8.Delete((BuildingTypeClass *)target);
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


// RulesClass::Objects (0x00652270)
// Each type reads its own rules section (the animations their art section),
// then the weapon and building types finish their setup, then each mission
// reads its section.
#include "shared/mission.hpp"

bool RulesClass::Objects(CCINIClass &ini)
{
    int index;
    for (index = 0; index < HouseTypes.Count(); index++) {
        HouseTypes[index]->Read_INI(ini);
    }
    for (index = 0; index < SuperWeaponTypes.Count(); index++) {
        SuperWeaponTypes[index]->Read_INI(ini);
    }
    for (index = 0; index < AnimTypes.Count(); index++) {
        AnimTypes[index]->Read_INI(ArtINI);
    }
    for (index = 0; index < BuildingTypes.Count(); index++) {
        BuildingTypes[index]->Read_INI(ini);
    }
    for (index = 0; index < AircraftTypes.Count(); index++) {
        AircraftTypes[index]->Read_INI(ini);
    }
    for (index = 0; index < UnitTypes.Count(); index++) {
        UnitTypes[index]->Read_INI(ini);
    }
    for (index = 0; index < InfantryTypes.Count(); index++) {
        InfantryTypes[index]->Read_INI(ini);
    }
    for (index = 0; index < unknown_WeaponTypeClass_list_00839b90.Count(); index++) {
        unknown_WeaponTypeClass_list_00839b90[index]->Read_INI(ini);
    }
    for (index = 0; index < BulletTypes.Count(); index++) {
        BulletTypes[index]->Read_INI(ini);
    }
    for (index = 0; index < unknown_WarheadTypeClass_list_00839b00.Count(); index++) {
        unknown_WarheadTypeClass_list_00839b00[index]->Read_INI(ini);
    }
    for (index = 0; index < unknown_WeaponTypeClass_list_00839b90.Count(); index++) {
        unknown_WeaponTypeClass_list_00839b90[index]->Init_Max_Speed();
    }
    for (index = 0; index < BuildingTypes.Count(); index++) {
        BuildingTypes[index]->unknown_func_45eaa0();
    }
    for (index = 0; index < TerrainTypes.Count(); index++) {
        TerrainTypes[index]->Read_INI(ini);
    }
    for (index = 0; index < SmudgeTypes.Count(); index++) {
        SmudgeTypes[index]->Read_INI(ini);
    }
    for (index = 0; index < OverlayTypes.Count(); index++) {
        OverlayTypes[index]->Read_INI(ini);
    }
    for (index = 0; index < ParticleTypes.Count(); index++) {
        ParticleTypes[index]->Read_INI(ini);
    }
    for (index = 0; index < ParticleSystemTypes.Count(); index++) {
        ParticleSystemTypes[index]->Read_INI(ini);
    }
    for (index = 0; index < VoxelAnimTypes.Count(); index++) {
        VoxelAnimTypes[index]->Read_INI(ini);
    }
    for (int mission = 0; mission < 30; mission++) {
        MissionControl[mission].Mission = mission;
        MissionControl[mission].Read_INI(ini);
    }
    return true;
}


// int __fastcall Coop_Camp_Unique_ID() (0x00652520, 196 bytes: 0x652520-0x6525E3,
// ret at 0x6525E3; the inventory splits it mid-instruction into 0x652520/0x652570).
// The CRC identifier (CCINIClass::Get_Unique_ID) of CoopCamp.ini, read once and
// kept; 0x006525F0 adds it to the rules/session INI identifiers.
// __fastcall: the Rules unit's free functions with parameters are fastcall (Get_Type,
// Difficulty_Get); with no parameters the call sites cannot tell, but only a fastcall
// caller schedules `lea ecx, file` before `push "CoopCamp.ini"` after the GenericList
// constructor call (VC6_MATCHING_NOTES: call-argument scheduling).
// The two globals follow the unit's geometry statics in .bss (0x00AB6D78-0x00AB6DE8);
// names are hypotheses.

extern bool CoopCampLoaded;         // 0x00AB6DF0
extern int CoopCampUniqueID;        // 0x00AB6DEC

int __fastcall Coop_Camp_Unique_ID()
{
    if (!CoopCampLoaded) {
        CCINIClass ini;
        CCFileClass file("CoopCamp.ini");
        if (ini.Load(file, false, false)) {
            CoopCampUniqueID = ini.Get_Unique_ID();
        }
        CoopCampLoaded = true;
    }
    return CoopCampUniqueID;
}


// int __fastcall Unknown_006525F0() (0x006525F0): the sum of the unique IDs of the
// INI at 0x008396D0, of every session INI (0x005B6F80 count, 0x005B6FE0 lookup,
// UnknownSessionA3D29C 0x005B7840) and of CoopCamp.ini (Coop_Camp_Unique_ID,
// expanded inline). Used for multiplayer synchronisation (hypothesis).
// __fastcall as Coop_Camp_Unique_ID: the inlined CCFileClass/Load setup is
// scheduled register-first only in a fastcall caller.

extern CCINIClass *unknown_ini_8396d0;     // 0x008396D0
int Unknown_005B6F80();
UnknownSessionA3D29C *__fastcall Unknown_005B6FE0(int index);
int __fastcall Coop_Camp_Unique_ID();

int __fastcall Unknown_006525F0()
{
    int id = unknown_ini_8396d0->Get_Unique_ID();
    int count = Unknown_005B6F80();
    for (int index = 1; index <= count; index++) {
        UnknownSessionA3D29C *session = Unknown_005B6FE0(index);
        if (session != 0) {
            id += session->unknown_ini_005b7840()->Get_Unique_ID();
        }
    }
    return id + Coop_Camp_Unique_ID();
}


// void __fastcall Read_Art_INI() (0x00652730): clears ArtINI and loads ART.INI
// into it (name hypothesis).
// __fastcall: only a fastcall caller schedules `lea ecx, file` before
// `push "ART.INI"` and `lea eax, file; mov ecx, ArtINI` before Load's pushes
// after the preceding thiscall (VC6_MATCHING_NOTES: call-argument scheduling).

void __fastcall Read_Art_INI()
{
    ArtINI.Clear();
    CCFileClass file("ART.INI");
    ArtINI.Load(file, false, false);
}


// 0x00652810: function not yet identified
//
// Owner: VectorClass<int>, stores the VectorClass<int> vtable (constructor or destructor) (from RTTI/vtables).
// Body: 45 bytes (frozen inventory bound).
// Referenced from 17 sites (unconditional call).
// Ghidra prototype: void __fastcall FUN_00652810(undefined4 *param_1)
// Callees with accepted source:
//   FUN_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00652810 {

extern void  FUN_00781d3b(unsigned int allocation_address);
extern "C" void* PTR_FUN_0079d9d4;

void __fastcall FUN_00652810(unsigned int *vector_words)
{
    *vector_words = (unsigned int)(&PTR_FUN_0079d9d4);
    if ((vector_words[1] != 0) && (*(char *)((int)vector_words + 0xd) != '\0')) {
        FUN_00781d3b((unsigned int)(vector_words[1]));
        vector_words[1] = 0;
    }
    *(unsigned char *)((int)vector_words + 0xd) = 0;
    vector_words[2] = 0;
    return;
}

}  // namespace seed_00652810


// TypeList members this unit emits by its own use of its type lists (bodies: shared/type_list.hpp):
// TypeList<int>::operator=(TypeList<int> const &) -- 0x00652790
// The implicit assignment (DynamicVectorClass's assignment expanded, then
// the list's own +0x18 field), kept out of line in the Rules unit for
// the integer lists; element type from the callers: the source lists come from CCINIClass::Get_Integer_List (0x0046B1B0) and the temporary is destroyed through VectorClass<int> (vtable 0x0079D9D4, 0x00652810); 18 call sites 0x00646728-0x0064B1D4.
// TypeList<InfantryTypeClass *>::operator=(TypeList<InfantryTypeClass *> const &) -- 0x006528A0
// The implicit assignment (DynamicVectorClass's assignment expanded, then
// the list's own +0x18 field), kept out of line in the Rules unit for
// the infantry type lists; element type from the callers: the temporary read by 0x00653E90 (e.g. "AllyParaDropInf", 0x0064A150) is destroyed through VectorClass<InfantryTypeClass *> (vtable 0x007A3118, 0x00652840).
// TypeList<TerrainTypeClass *>::operator=(TypeList<TerrainTypeClass *> const &) -- 0x00652920
// The implicit assignment (DynamicVectorClass's assignment expanded, then
// the list's own +0x18 field), kept out of line in the Rules unit for
// the terrain type lists; element type from the callers: the temporary read by 0x00654150 ("DefaultMirageDisguises", 0x0064B550) is destroyed through VectorClass<TerrainTypeClass *> (vtable 0x007A9158, 0x00652870).
// TypeList<BuildingTypeClass *>::operator=(TypeList<BuildingTypeClass *> const &) -- 0x006529A0
// The implicit assignment (DynamicVectorClass's assignment expanded, then
// the list's own +0x18 field), kept out of line in the Rules unit for
// the building type lists; element type from the callers: the temporary read by 0x00653BD0 (e.g. "BuildNavalYard", 0x0064CE99) is destroyed through VectorClass<BuildingTypeClass *> (vtable 0x007A2F08).


// void TypeList<BuildingTypeClass *>::Save(IStream *stream) (0x00652A20)
// The Rules unit's copy of TypeList<T>::Save (shared/type_list.hpp), emitted where
// RulesClass::Save (0x0064DE70) calls it out of line.
#include "shared/type_list.hpp"
// @symbol ?Save@?$TypeList@PAVBuildingTypeClass@@@@QAEXPAUIStream@@@Z


// TypeList members this unit emits by its own use of its type lists (bodies: shared/type_list.hpp):
// TypeList<VoxelAnimTypeClass *>::TypeList, default constructor (0x00652A70; vtable 0x007A90F8).


// void TypeList<VoxelAnimTypeClass *>::Save(IStream *stream) (0x00652AA0)
// The Rules unit's copy of TypeList<T>::Save (shared/type_list.hpp), emitted where
// RulesClass::Save (0x0064DE70) calls it out of line.
// @symbol ?Save@?$TypeList@PAVVoxelAnimTypeClass@@@@QAEXPAUIStream@@@Z


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<VoxelAnimTypeClass *>::Clear (0x00652AF0; TypeList<VoxelAnimTypeClass *> vtable 0x007a90f8 slot 3).
// DynamicVectorClass<VoxelAnimTypeClass *>::ID_pointer (0x00652B20; TypeList<VoxelAnimTypeClass *> vtable 0x007a90f8 slot 5).
// VectorClass<VoxelAnimTypeClass *>::Fetch (0x00652B40; TypeList<VoxelAnimTypeClass *> vtable 0x007a90f8 slot 6).


// TypeList members this unit emits by its own use of its type lists (bodies: shared/type_list.hpp):
// TypeList<AnimTypeClass *>::TypeList, default constructor (0x00652B50; vtable 0x007A3B3C).


// void TypeList<AnimTypeClass *>::Save(IStream *stream) (0x00652B80)
// The Rules unit's copy of TypeList<T>::Save (shared/type_list.hpp), emitted where
// RulesClass::Save (0x0064DE70) calls it out of line.
// @symbol ?Save@?$TypeList@PAVAnimTypeClass@@@@QAEXPAUIStream@@@Z


// TypeList members this unit emits by its own use of its type lists (bodies: shared/type_list.hpp):
// TypeList<int>::TypeList, default constructor (0x00652BD0; vtable 0x0079D9F4).


// void TypeList<int>::Save(IStream *stream) (0x00652C00)
// The Rules unit's copy of TypeList<T>::Save (shared/type_list.hpp), emitted where
// RulesClass::Save (0x0064DE70) calls it out of line.
// @symbol ?Save@?$TypeList@H@@QAEXPAUIStream@@@Z


// TypeList members this unit emits by its own use of its type lists (bodies: shared/type_list.hpp):
// TypeList<SmudgeTypeClass *>::TypeList, default constructor (0x00652C50; vtable 0x007A90D8).


// void TypeList<SmudgeTypeClass *>::Save(IStream *stream) (0x00652C80)
// The Rules unit's copy of TypeList<T>::Save (shared/type_list.hpp), emitted where
// RulesClass::Save (0x0064DE70) calls it out of line.
// @symbol ?Save@?$TypeList@PAVSmudgeTypeClass@@@@QAEXPAUIStream@@@Z


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<SmudgeTypeClass *>::Clear (0x00652CD0; TypeList<SmudgeTypeClass *> vtable 0x007a90d8 slot 3).
// DynamicVectorClass<SmudgeTypeClass *>::ID_pointer (0x00652D00; TypeList<SmudgeTypeClass *> vtable 0x007a90d8 slot 5).


// DynamicVectorClass<SmudgeTypeClass *> &DynamicVectorClass<SmudgeTypeClass *>::operator=(DynamicVectorClass<SmudgeTypeClass *> const &) (0x00652D20)
// The Rules unit's copy of DynamicVectorClass<T>::operator= (shared/vector_class.hpp),
// emitted where RulesClass::Combat_Damage (0x00646C60) assigns the Scorches4 list.
#include "shared/vector_class.hpp"
// @symbol ??4?$DynamicVectorClass@PAVSmudgeTypeClass@@@@QAEAAV0@ABV0@@Z


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<SmudgeTypeClass *>::Fetch (0x00652DB0; VectorClass<SmudgeTypeClass *> vtable 0x007a9138 slot 6).


// TypeList members this unit emits by its own use of its type lists (bodies: shared/type_list.hpp):
// TypeList<UnitTypeClass *>::TypeList, default constructor (0x00652DC0; vtable 0x007A30D8).


// void TypeList<UnitTypeClass *>::Save(IStream *stream) (0x00652DF0)
// The Rules unit's copy of TypeList<T>::Save (shared/type_list.hpp), emitted where
// RulesClass::Save (0x0064DE70) calls it out of line.
// @symbol ?Save@?$TypeList@PAVUnitTypeClass@@@@QAEXPAUIStream@@@Z


// void TypeList<AircraftTypeClass *>::Save(IStream *stream) (0x00652E40)
// The Rules unit's copy of TypeList<T>::Save (shared/type_list.hpp), emitted where
// RulesClass::Save (0x0064DE70) calls it out of line.
// @symbol ?Save@?$TypeList@PAVAircraftTypeClass@@@@QAEXPAUIStream@@@Z


// TypeList members this unit emits by its own use of its type lists (bodies: shared/type_list.hpp):
// TypeList<InfantryTypeClass *>::TypeList, default constructor (0x00652E90; vtable 0x007A30F8).


// void TypeList<InfantryTypeClass *>::Save(IStream *stream) (0x00652EC0)
// The Rules unit's copy of TypeList<T>::Save (shared/type_list.hpp), emitted where
// RulesClass::Save (0x0064DE70) calls it out of line.
// @symbol ?Save@?$TypeList@PAVInfantryTypeClass@@@@QAEXPAUIStream@@@Z


// TypeList members this unit emits by its own use of its type lists (bodies: shared/type_list.hpp):
// TypeList<TerrainTypeClass *>::TypeList, default constructor (0x00652F10; vtable 0x007A90B8).


// void TypeList<TerrainTypeClass *>::Save(IStream *stream) (0x00652F40)
// The Rules unit's copy of TypeList<T>::Save (shared/type_list.hpp), emitted where
// RulesClass::Save (0x0064DE70) calls it out of line.
// @symbol ?Save@?$TypeList@PAVTerrainTypeClass@@@@QAEXPAUIStream@@@Z


// TypeList members this unit emits by its own use of its type lists (bodies: shared/type_list.hpp):
// TypeList<VoxelAnimTypeClass *> scalar deleting destructor (0x00652FF0; vtable 0x007A90F8 slot 0).
// Layout and behaviour: shared/type_list.hpp. ~TypeList is implicit; the
// expanded ~VectorClass stores the VectorClass vtable 0x007A9118. The
// constructor below only makes the unit emit the vtable and with it this
// destructor; it is the copy at 0x00652A70.
// TypeList<SmudgeTypeClass *> scalar deleting destructor (0x00653040; vtable 0x007A90D8 slot 0).
// Layout and behaviour: shared/type_list.hpp. ~TypeList is implicit; the
// expanded ~VectorClass stores the VectorClass vtable 0x007A9138. The
// constructor below only makes the unit emit the vtable and with it this
// destructor; it is the copy at 0x00652C50.
// TypeList<TerrainTypeClass *> scalar deleting destructor (0x00653090; vtable 0x007A90B8 slot 0).
// Layout and behaviour: shared/type_list.hpp. ~TypeList is implicit; the
// expanded ~VectorClass stores the VectorClass vtable 0x007A9158. The
// constructor below only makes the unit emit the vtable and with it this
// destructor; it is the copy at 0x00652F10.


extern "C" unsigned char vtable_654390[]; extern "C" void __cdecl release_buffer_654390(void*); extern "C" void __cdecl release_object_654390(void*);
class Leaf_654390 { public: void* get(unsigned int flags); };
void* Leaf_654390::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_654390; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_654390((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_654390(this); return this; }


extern "C" unsigned char vtable_6543e0[]; extern "C" void __cdecl release_buffer_6543e0(void*); extern "C" void __cdecl release_object_6543e0(void*);
class Leaf_6543e0 { public: void* get(unsigned int flags); };
void* Leaf_6543e0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_6543e0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_6543e0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_6543e0(this); return this; }


extern "C" unsigned char vtable_654430[]; extern "C" void __cdecl release_buffer_654430(void*); extern "C" void __cdecl release_object_654430(void*);
class Leaf_654430 { public: void* get(unsigned int flags); };
void* Leaf_654430::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_654430; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_654430((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_654430(this); return this; }


extern "C" unsigned char vtable_654480[]; extern "C" void __cdecl release_buffer_654480(void*); extern "C" void __cdecl release_object_654480(void*);
class Leaf_654480 { public: void* get(unsigned int flags); };
void* Leaf_654480::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_654480; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_654480((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_654480(this); return this; }


extern "C" unsigned char vtable_6544d0[]; extern "C" void __cdecl release_buffer_6544d0(void*); extern "C" void __cdecl release_object_6544d0(void*);
class Leaf_6544d0 { public: void* get(unsigned int flags); };
void* Leaf_6544d0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_6544d0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_6544d0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_6544d0(this); return this; }


extern "C" unsigned char vtable_654520[]; extern "C" void __cdecl release_buffer_654520(void*); extern "C" void __cdecl release_object_654520(void*);
class Leaf_654520 { public: void* get(unsigned int flags); };
void* Leaf_654520::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_654520; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_654520((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_654520(this); return this; }


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<TerrainTypeClass *>::Clear (0x00652F90; TypeList<TerrainTypeClass *> vtable 0x007a90b8 slot 3).
// DynamicVectorClass<TerrainTypeClass *>::ID_pointer (0x00652FC0; TypeList<TerrainTypeClass *> vtable 0x007a90b8 slot 5).
// VectorClass<TerrainTypeClass *>::Fetch (0x00652FE0; VectorClass<TerrainTypeClass *> vtable 0x007a9158 slot 6).
// VectorClass<BuildingTypeClass *>::operator=(VectorClass<BuildingTypeClass *> const &) -- 0x006530E0
// Replaces the contents with an owned copy (shared/vector_class.hpp); the
// copy kept out of line in the Rules unit. Element type from the callers:
// the source lists are TypeList<BuildingTypeClass *> copies (0x0058EED0, e.g. at 0x00643C6E).
// VectorClass<VoxelAnimTypeClass *>::operator=(VectorClass<VoxelAnimTypeClass *> const &) -- 0x00653150
// Replaces the contents with an owned copy (shared/vector_class.hpp); the
// copy kept out of line in the Rules unit. Element type from the callers:
// the source lists are TypeList<VoxelAnimTypeClass *> copies (0x00654570, e.g. at 0x00644024).
// VectorClass<VoxelAnimTypeClass *>::operator equal (0x006531C0; VectorClass<VoxelAnimTypeClass *> vtable 0x007a9118 slot 1).
// VectorClass<VoxelAnimTypeClass *>::Resize (0x00653200; VectorClass<VoxelAnimTypeClass *> vtable 0x007a9118 slot 2).


// 0x006532b0: function not yet identified
//
// Owner: VectorClass<VoxelAnimTypeClass *>, virtual slot 3 (vtable +0xc) (from RTTI/vtables).
// Body: 39 bytes (frozen inventory bound).
// Referenced from 8 sites (computed call, data, unconditional call).
// Ghidra prototype: void __fastcall Clear_006532b0(int param_1)
// Callees with accepted source:
//   FUN_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006532b0 {

extern void  FUN_00781d3b(unsigned int allocation_address);

void __fastcall Clear_006532b0(int vector_address)
{
    if ((*(int *)(vector_address + 4) != 0) && (*(char *)(vector_address + 0xd) != '\0')) {
        FUN_00781d3b((unsigned int)(*(int *)(vector_address + 4)));
        *(unsigned int *)(vector_address + 4) = 0;
    }
    *(unsigned char *)(vector_address + 0xd) = 0;
    *(unsigned int *)(vector_address + 8) = 0;
    return;
}

}  // namespace seed_006532b0


// VectorClass<VoxelAnimTypeClass *>::ID(VoxelAnimTypeClass * const &) (0x006532E0)
// Template member instantiated from shared/vector_class.hpp.
// No source of its own: emitted by this unit (population-2554).


// Original region: 0x00653730 (93 body bytes, 3 independently frozen NOP bytes).
// Exact target SHA256: e6418111c2c36081a1795fb000e0927beea1630170b334c5013d12102602e70f.
// Purpose: initialize an inferred four-byte-element storage wrapper.
// Class/API identity and the three unknown field purposes remain unresolved.

extern "C" void* __cdecl allocate_storage_00653730(unsigned int byte_count);
extern "C" unsigned char base_dispatch_00653730[];
extern "C" unsigned char derived_dispatch_00653730[];

class WordStorage_00653730 {
    unsigned char* dispatch_table;
    unsigned int* storage;
    unsigned int capacity;
    unsigned char unknown_0x0c;
    unsigned char owns_storage;
    // Bytes 0x0e..0x0f are alignment padding and are not written.
    unsigned int unknown_0x10;
    unsigned int unknown_0x14;

public:
    WordStorage_00653730* initialize(unsigned int requested_capacity,
                                unsigned int* supplied_storage);
};

typedef char WordStorage_00653730_size_check[(sizeof(WordStorage_00653730) == 24) ? 1 : -1];

WordStorage_00653730* WordStorage_00653730::initialize(
    unsigned int requested_capacity,
    unsigned int* supplied_storage)
{
    storage = 0;
    capacity = requested_capacity;
    unknown_0x0c = 1;
    owns_storage = 0;
    dispatch_table = base_dispatch_00653730;

    // A zero capacity ignores even a nonnull supplied pointer.
    if (requested_capacity != 0) {
        if (supplied_storage != 0) {
            storage = supplied_storage;
        } else {
            // Preserve 32-bit wrapping and ownership even on a null result.
            storage = (unsigned int*)allocate_storage_00653730(requested_capacity << 2);
            owns_storage = 1;
        }
    }

    dispatch_table = derived_dispatch_00653730;
    unknown_0x14 = 10;
    unknown_0x10 = 0;
    return this;
}


// Original region: 0x00653a40 (93 body bytes, 3 independently frozen NOP bytes).
// Exact target SHA256: e6418111c2c36081a1795fb000e0927beea1630170b334c5013d12102602e70f.
// Purpose: initialize an inferred four-byte-element storage wrapper.
// Class/API identity and the three unknown field purposes remain unresolved.

extern "C" void* __cdecl allocate_storage_00653a40(unsigned int byte_count);
extern "C" unsigned char base_dispatch_00653a40[];
extern "C" unsigned char derived_dispatch_00653a40[];

class WordStorage_00653a40 {
    unsigned char* dispatch_table;
    unsigned int* storage;
    unsigned int capacity;
    unsigned char unknown_0x0c;
    unsigned char owns_storage;
    // Bytes 0x0e..0x0f are alignment padding and are not written.
    unsigned int unknown_0x10;
    unsigned int unknown_0x14;

public:
    WordStorage_00653a40* initialize(unsigned int requested_capacity,
                                unsigned int* supplied_storage);
};

typedef char WordStorage_00653a40_size_check[(sizeof(WordStorage_00653a40) == 24) ? 1 : -1];

WordStorage_00653a40* WordStorage_00653a40::initialize(
    unsigned int requested_capacity,
    unsigned int* supplied_storage)
{
    storage = 0;
    capacity = requested_capacity;
    unknown_0x0c = 1;
    owns_storage = 0;
    dispatch_table = base_dispatch_00653a40;

    // A zero capacity ignores even a nonnull supplied pointer.
    if (requested_capacity != 0) {
        if (supplied_storage != 0) {
            storage = supplied_storage;
        } else {
            // Preserve 32-bit wrapping and ownership even on a null result.
            storage = (unsigned int*)allocate_storage_00653a40(requested_capacity << 2);
            owns_storage = 1;
        }
    }

    dispatch_table = derived_dispatch_00653a40;
    unknown_0x14 = 10;
    unknown_0x10 = 0;
    return this;
}


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<VoxelAnimTypeClass *>::Add (0x00653450): emitted by this unit's use of its list.
// DynamicVectorClass<SmudgeTypeClass *>::Add (0x00653840): emitted by this unit's use of its list.
// DynamicVectorClass<TerrainTypeClass *>::Add (0x00653B50): emitted by this unit's use of its list.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<VoxelAnimTypeClass *>::Resize (0x006533A0; TypeList<VoxelAnimTypeClass *> vtable 0x007a90f8 slot 2).
// DynamicVectorClass<VoxelAnimTypeClass *>::ID_object (0x006534A0; TypeList<VoxelAnimTypeClass *> vtable 0x007a90f8 slot 4).
// VectorClass<AnimTypeClass *>::operator=(VectorClass<AnimTypeClass *> const &) -- 0x006534D0
// Replaces the contents with an owned copy (shared/vector_class.hpp); the
// copy kept out of line in the Rules unit. Element type from the callers:
// the source lists are TypeList<AnimTypeClass *> copies (0x0050B7D0, e.g. at 0x006442FF).
// VectorClass<SmudgeTypeClass *>::operator=(VectorClass<SmudgeTypeClass *> const &) -- 0x00653540
// Replaces the contents with an owned copy (shared/vector_class.hpp); the
// copy kept out of line in the Rules unit. Element type from the callers:
// the source lists are TypeList<SmudgeTypeClass *> copies (0x00654600, e.g. at 0x00646DEA).
// VectorClass<SmudgeTypeClass *>::operator equal (0x006535B0; VectorClass<SmudgeTypeClass *> vtable 0x007a9138 slot 1).
// VectorClass<SmudgeTypeClass *>::Resize (0x006535F0; VectorClass<SmudgeTypeClass *> vtable 0x007a9138 slot 2).
// VectorClass<SmudgeTypeClass *>::Clear (0x006536A0; VectorClass<SmudgeTypeClass *> vtable 0x007a9138 slot 3).
// VectorClass<SmudgeTypeClass *>::ID_object (0x006536D0; VectorClass<SmudgeTypeClass *> vtable 0x007a9138 slot 4).
// VectorClass<SmudgeTypeClass *>::ID_pointer (0x00653710; VectorClass<SmudgeTypeClass *> vtable 0x007a9138 slot 5).
// DynamicVectorClass<SmudgeTypeClass *>::Resize (0x00653790; TypeList<SmudgeTypeClass *> vtable 0x007a90d8 slot 2).
// DynamicVectorClass<SmudgeTypeClass *>::ID_object (0x00653890; TypeList<SmudgeTypeClass *> vtable 0x007a90d8 slot 4).
// VectorClass<TerrainTypeClass *>::operator equal (0x006538C0; VectorClass<TerrainTypeClass *> vtable 0x007a9158 slot 1).
// VectorClass<TerrainTypeClass *>::Resize (0x00653900; VectorClass<TerrainTypeClass *> vtable 0x007a9158 slot 2).
// VectorClass<TerrainTypeClass *>::Clear (0x006539B0; VectorClass<TerrainTypeClass *> vtable 0x007a9158 slot 3).
// VectorClass<TerrainTypeClass *>::ID_object (0x006539E0; VectorClass<TerrainTypeClass *> vtable 0x007a9158 slot 4).
// VectorClass<TerrainTypeClass *>::ID_pointer (0x00653A20; VectorClass<TerrainTypeClass *> vtable 0x007a9158 slot 5).
// DynamicVectorClass<TerrainTypeClass *>::Resize (0x00653AA0; TypeList<TerrainTypeClass *> vtable 0x007a90b8 slot 2).
// DynamicVectorClass<TerrainTypeClass *>::ID_object (0x00653BA0).
// DynamicVectorClass<TerrainTypeClass *>::ID_object (0x00653BA0; TypeList<TerrainTypeClass *> vtable 0x007a90b8 slot 4).


// TypeListReader<BuildingTypeClass>::Get (0x00653BD0).
// Layout and behaviour: shared/ccini_class.hpp.

template class TypeListReader<BuildingTypeClass>;


// Get_Type<WarheadTypeClass> (0x00653DA0): the out-of-line copy of shared/ccini_class.hpp's
// Get_Type template that the Rules unit's readers call once their inline budget is spent.

template WarheadTypeClass *__fastcall Get_Type<WarheadTypeClass>(CCINIClass &ini, char const *section, char const *entry, WarheadTypeClass *defvalue);


// Get_Type<AnimTypeClass> (0x00653DF0): the out-of-line copy of shared/ccini_class.hpp's
// Get_Type template that RulesClass::Audio_Visual (0x00643E10) calls once its inline budget is spent.

template AnimTypeClass *__fastcall Get_Type<AnimTypeClass>(CCINIClass &ini, char const *section, char const *entry, AnimTypeClass *defvalue);


// Get_Type<AnimTypeClass const> (0x00653E40): a second instantiation of shared/ccini_class.hpp's
// Get_Type template, byte-identical to 0x00653DF0, for the const anim fields RulesClass::Audio_Visual
// (0x00643E10) reads (EMPulseSparkles +0x1438, OreTwinkle +0x14B8; TibSun types Rules anims const).

template AnimTypeClass const *__fastcall Get_Type<AnimTypeClass const>(CCINIClass &ini, char const *section, char const *entry, AnimTypeClass const *defvalue);


// TypeListReader<InfantryTypeClass>::Get (0x00653E90).
// Layout and behaviour: shared/ccini_class.hpp.

template class TypeListReader<InfantryTypeClass>;


// Get_Type<BuildingTypeClass> (0x00654060): the out-of-line copy of shared/ccini_class.hpp's
// Get_Type template that the Rules unit's readers call once their inline budget is spent.

template BuildingTypeClass *__fastcall Get_Type<BuildingTypeClass>(CCINIClass &ini, char const *section, char const *entry, BuildingTypeClass *defvalue);


// Get_Type<AircraftTypeClass> (0x006540B0): the out-of-line copy of shared/ccini_class.hpp's
// Get_Type template that the Rules unit's readers call once their inline budget is spent.

template AircraftTypeClass *__fastcall Get_Type<AircraftTypeClass>(CCINIClass &ini, char const *section, char const *entry, AircraftTypeClass *defvalue);


// Get_Type<TerrainTypeClass> (0x00654100): the out-of-line copy of shared/ccini_class.hpp's
// Get_Type template that the Rules unit's readers call once their inline budget is spent.

template TerrainTypeClass *__fastcall Get_Type<TerrainTypeClass>(CCINIClass &ini, char const *section, char const *entry, TerrainTypeClass *defvalue);


// TypeListReader<TerrainTypeClass>::Get (0x00654150).
// Layout and behaviour: shared/ccini_class.hpp.

template class TypeListReader<TerrainTypeClass>;


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<TerrainTypeClass *>::operator=(VectorClass<TerrainTypeClass *> const &) -- 0x00654320
// Replaces the contents with an owned copy (shared/vector_class.hpp); the
// copy kept out of line in the Rules unit. Element type from the callers:
// its one caller, the TypeList<TerrainTypeClass *> copy constructor expanded in 0x00654150, calls it on an object whose vptr was just set to VectorClass<TerrainTypeClass *> (0x007A9158).


// TypeList members this unit emits by its own use of its type lists (bodies: shared/type_list.hpp):
// TypeList<VoxelAnimTypeClass *>::TypeList, copy constructor (0x00654570; vtable 0x007A90F8).
// TypeList<SmudgeTypeClass *>::TypeList, copy constructor (0x00654600; vtable 0x007A90D8).
// TypeList<UnitTypeClass *>::TypeList, copy constructor (0x00654690; vtable 0x007A30D8).
// TypeList<InfantryTypeClass *>::TypeList, copy constructor (0x00654720; vtable 0x007A30F8).


extern "C" unsigned char table_654960[]; extern "C" void* __cdecl storage_654960(unsigned int);
class Storage_654960 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_654960::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_654960; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_654960(requested << 2); owns=1; } return this; }


extern "C" unsigned char table_6549b0[]; extern "C" void* __cdecl storage_6549b0(unsigned int);
class Storage_6549b0 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_6549b0::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_6549b0; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_6549b0(requested << 2); owns=1; } return this; }


extern "C" unsigned char table_654a00[]; extern "C" void* __cdecl storage_654a00(unsigned int);
class Storage_654a00 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_654a00::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_654a00; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_654a00(requested << 2); owns=1; } return this; }
