// techtype.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/techtype.json carries the addresses and the rows that
// stay in their own files.


#include "shared/techno_type.hpp"
#include "shared/house.hpp"
#include "shared/ccini_class.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/mix_file_class.hpp"
#include "shared/crt_string.hpp"
#include "shared/crt_tokens.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/rules.hpp"
#include "shared/anim_type.hpp"
#include "shared/voxel_anim_type.hpp"
#include "shared/building_type.hpp"
#include "shared/unit_type.hpp"
#include "shared/aircraft_type.hpp"
#include "shared/particle_system.hpp"
#include "shared/debug_printf.hpp"
#include "shared/exit.hpp"
#include "shared/object.hpp"
#include "shared/map_view.hpp"
#include "shared/building_type_class.hpp"
#include "shared/fast_math.hpp"
#include "shared/tactical.hpp"


// bool TechnoTypeClass::CanUseWaypoint() const (0x006DACB0)
// Always true (meaning unknown).

bool TechnoTypeClass::CanUseWaypoint() const
{
    return true;
}


// bool TechnoTypeClass::CanAttackMove() const (0x006DACC0)
// True when the primary weapon exists and PreventAttackMove is clear.

bool TechnoTypeClass::CanAttackMove() const
{
    if (Weapon[0].WeaponType && !PreventAttackMove) {
        return true;
    }
    return false;
}


// int TechnoTypeClass::Raw_Cost() const (0x006DACE0)
// Cost= (slot 43).

int TechnoTypeClass::Raw_Cost() const
{
    return Cost;
}


// int TechnoTypeClass::unknown_slot28() const (0x006DACF0)
// Who may own the type (hypothesis: RA1 Get_Ownable): every house when
// DoubleOwned in a non-zero game mode, else the Owner= house set.

int TechnoTypeClass::unknown_slot28() const
{
    if (DoubleOwned && unknown_game_mode_a3d298) {
        return 0x7FFFFFFF;
    }
    return Owner;
}


// int TechnoTypeClass::unknown_slot34() const (0x006DAD10)
// Cost scaled by the rules factor at +0x13B0 and 0.9 (meaning unknown).

int TechnoTypeClass::unknown_slot34() const
{
    return (int)(Cost * Rules->unknown_13b0 * 0.9);
}


// int TechnoTypeClass::Cost_Of(HouseClass *house) const (0x006DAD30)
// Cost for a house (hypothesis: RA1 Cost_Of): Raw_Cost scaled by the
// house's multiplier.

int TechnoTypeClass::Cost_Of(HouseClass *house) const
{
    if (house) {
        float factor = house->Cost_Multiplier(this);
        return (int)(Raw_Cost() * factor);
    }
    return Raw_Cost();
}


// int TechnoTypeClass::unknown_slot46(HouseClass *house, bool full) const (0x006DAD80)
// Sell value (hypothesis): Soylent= when set, else the house cost scaled by
// the rules refund factor (1.0 when full).

int TechnoTypeClass::unknown_slot46(HouseClass *house, bool full) const
{
    float percent = (float)Rules->unknown_13a0;
    if (full) {
        percent = 1.0f;
    }
    if (house) {
        float factor = house->Cost_Multiplier(this);
        if (Soylent) {
            return (int)(Soylent * factor);
        }
        int cost = (int)(Raw_Cost() * factor);
        if (house->Unknown_004F52F0()) {
            cost = (int)(cost * percent);
        }
        return cost;
    }
    return (int)(Raw_Cost() * percent);
}


// void const *TechnoTypeClass::unknown_slot38() const (0x006DAE40)
// Cameo data (hypothesis: RA1 Get_Cameo_Data): the alternate cameo for
// kinds 0x10 and 0x28 when the player's house flags allow it.

void const *TechnoTypeClass::unknown_slot38() const
{
    if (unknown_player_a35db4 && unknown_player_a35db4->unknown_206 && Kind_Of() == 0x10 && AltCameoData && Trainable) {
        return AltCameoData;
    }
    if (unknown_player_a35db4 && unknown_player_a35db4->unknown_207 && Kind_Of() == 0x28 && AltCameoData && Trainable) {
        return AltCameoData;
    }
    return CameoData;
}


// int TechnoTypeClass::Repair_Cost() const (0x006DAEC0)
// Cost of one repair step (slot 44; RA1 formula): at least 1.

int TechnoTypeClass::Repair_Cost() const
{
    int result = (int)((Raw_Cost() / (MaxStrength / Rules->unknown_1334)) * Rules->unknown_1338);
    if (result <= 1) {
        result = 1;
    }
    return result;
}


// int TechnoTypeClass::Repair_Step() const (0x006DAF10)
// The rules repair step (slot 45).

int TechnoTypeClass::Repair_Step() const
{
    return Rules->unknown_1334;
}


// bool TechnoTypeClass::Is_Two_Shooter() const (0x006DAF20)
// True when the primary fires twice: same secondary or a burst above one.

bool TechnoTypeClass::Is_Two_Shooter() const
{
    WeaponTypeClass *primary = Weapon[0].WeaponType;
    if (primary) {
        WeaponTypeClass *secondary = Weapon[1].WeaponType;
        if (primary == secondary || primary->Burst > 1 || (secondary && secondary->Burst > 1)) {
            return true;
        }
    }
    return false;
}


// TechnoTypeClass::Read_INI (0x006DAF60)
// Reads the type's rules section (ini), then its art section (ArtINI,
// section Image=): image and cameo shapes, palette, weapon fire offsets.

// A recoil timing value (TurretTravel=, BarrelTravel=); the default is the
// current value.
static inline int Get_Recoil(CCINIClass &ini, char const *section, char const *entry, int value)
{
    value = ini.Get_Int(section, entry, value);
    return value;
}

// A recoil frame count (TurretCompressFrames= ...): at least one.
static inline int Get_Recoil_Frames(CCINIClass &ini, char const *section, char const *entry, int frames)
{
    frames = ini.Get_Int(section, entry, frames);
    if (frames <= 0) {
        return 1;
    }
    return frames;
}

bool TechnoTypeClass::Read_INI(CCINIClass &ini)
{
    if (ObjectTypeClass::Read_INI(ini)) {
        LandTargeting = ini.Get_Int(ID, "LandTargeting", LandTargeting);
        NavalTargeting = ini.Get_Int(ID, "NavalTargeting", NavalTargeting);
        SpeedType = ini.Get_SpeedType(ID, "SpeedType", (::SpeedType)SpeedType);
        TypeImmune = ini.Get_Bool(ID, "TypeImmune", TypeImmune);
        WalkRate = ini.Get_Int(ID, "WalkRate", WalkRate);
        IdleRate = ini.Get_Int(ID, "IdleRate", IdleRate);
        MoveToShroud = ini.Get_Bool(ID, "MoveToShroud", MoveToShroud);
        IsTrain = ini.Get_Bool(ID, "IsTrain", IsTrain);
        DoubleOwned = ini.Get_Bool(ID, "DoubleOwned", DoubleOwned);
        GuardRange.Value = ini.Get_Lepton(ID, "GuardRange", GuardRange.Value);
        Explodes = ini.Get_Bool(ID, "Explodes", Explodes);
        DeathWeapon = Get_Type(ini, ID, "DeathWeapon", DeathWeapon);
        DeathWeaponDamageModifier = ini.Get_Double(ID, "DeathWeaponDamageModifier", DeathWeaponDamageModifier);
        FlightLevel = ini.Get_Int(ID, "FlightLevel", FlightLevel);
        IsDropship = ini.Get_Bool(ID, "IsDropship", IsDropship);
        double pitch = ini.Get_Double(ID, "PitchAngle", -1.0);
        if (pitch != -1.0) {
            PitchAngle = pitch * (3.141592653589793 / 180.0);
        }
        double roll = ini.Get_Double(ID, "RollAngle", -1.0);
        if (roll != -1.0) {
            RollAngle = roll * (3.141592653589793 / 180.0);
        }
        PitchSpeed = ini.Get_Double(ID, "PitchSpeed", PitchSpeed);
        Locomotor = ini.Get_UUID(ID, "Locomotor", Locomotor);
        CloakingSpeed = ini.Get_Int(ID, "CloakingSpeed", CloakingSpeed);
        ThreatAvoidanceCoefficient = ini.Get_Double(ID, "ThreatAvoidanceCoefficient", ThreatAvoidanceCoefficient);
        SlowdownDistance = ini.Get_Int(ID, "SlowdownDistance", SlowdownDistance);
        DeaccelerationFactor = ini.Get_Double(ID, "DeaccelerationFactor", DeaccelerationFactor);
        AccelerationFactor = ini.Get_Double(ID, "AccelerationFactor", AccelerationFactor);
        Weight = ini.Get_Double(ID, "Weight", Weight);
        PhysicalSize = ini.Get_Double(ID, "PhysicalSize", PhysicalSize);
        Size = ini.Get_Double(ID, "Size", Size);
        SizeLimit = ini.Get_Double(ID, "SizeLimit", SizeLimit);
        HoverAttack = ini.Get_Bool(ID, "HoverAttack", HoverAttack);
        VHPScan = ini.Get_Level(ID, "VHPScan", (LevelType)VHPScan);
        MaxDebris = ini.Get_Int(ID, "MaxDebris", MaxDebris);
        MinDebris = ini.Get_Int(ID, "MinDebris", MinDebris);
        if (MinDebris < 0) {
            MinDebris = 0;
        }
        if (MaxDebris < MinDebris) {
            MaxDebris = MinDebris;
        }
        DebrisTypes = TypeListReader<VoxelAnimTypeClass>::Get(ini, ID, "DebrisTypes", DebrisTypes);
        DebrisMaximums = ini.Get_Integer_List(ID, "DebrisMaximums", DebrisMaximums);
        DebrisAnims = TypeListReader<AnimTypeClass>::Get(ini, ID, "DebrisAnims", DebrisAnims);
        HasTurretTooltips = ini.Get_Bool(ID, "HasTurretTooltips", HasTurretTooltips);
        TurretCount = ini.Get_Int(ID, "TurretCount", TurretCount);
        WeaponCount = ini.Get_Int(ID, "WeaponCount", WeaponCount);
        IsChargeTurret = ini.Get_Bool(ID, "IsChargeTurret", IsChargeTurret);
        ClearAllWeapons = ini.Get_Bool(ID, "ClearAllWeapons", ClearAllWeapons);
        if (TurretCount > 0) {
            for (int index = 0; index < WeaponCount; index++) {
                char weapon[12];
                char elite[16];
                sprintf(weapon, "Weapon%d", index + 1);
                sprintf(elite, "EliteWeapon%d", index + 1);
                Get_Weapon(index)->WeaponType = Get_Type(ini, ID, weapon, Get_Weapon(index)->WeaponType);
                Get_Elite_Weapon(index)->WeaponType = Get_Type(ini, ID, elite, Get_Elite_Weapon(index)->WeaponType);
            }
        } else if (!ClearAllWeapons) {
            Weapon[0].WeaponType = Get_Type(ini, ID, "Primary", Weapon[0].WeaponType);
            Weapon[1].WeaponType = Get_Type(ini, ID, "Secondary", Weapon[1].WeaponType);
            EliteWeapon[0].WeaponType = Get_Type(ini, ID, "ElitePrimary", EliteWeapon[0].WeaponType);
            EliteWeapon[1].WeaponType = Get_Type(ini, ID, "EliteSecondary", EliteWeapon[1].WeaponType);
        }
        if (ClearAllWeapons) {
            Weapon[0].WeaponType = 0;
            Weapon[1].WeaponType = 0;
            EliteWeapon[0].WeaponType = 0;
            EliteWeapon[1].WeaponType = 0;
        }
        VoiceMove = Get_Voc_List(ini, ID, "VoiceMove", VoiceMove);
        VoiceSelect = Get_Voc_List(ini, ID, "VoiceSelect", VoiceSelect);
        VoiceAttack = Get_Voc_List(ini, ID, "VoiceAttack", VoiceAttack);
        VoiceSpecialAttack = Get_Voc_List(ini, ID, "VoiceSpecialAttack", VoiceSpecialAttack);
        VoiceDie = Get_Voc_List(ini, ID, "VoiceDie", VoiceDie);
        VoiceFeedback = Get_Voc_List(ini, ID, "VoiceFeedback", VoiceFeedback);
        AuxSound1 = Get_Sound(ini, ID, "AuxSound1", AuxSound1);
        AuxSound2 = Get_Sound(ini, ID, "AuxSound2", AuxSound2);
        CreateSound = Get_Sound(ini, ID, "CreateSound", CreateSound);
        DamageSound = Get_Sound(ini, ID, "DamageSound", DamageSound);
        ImpactWaterSound = Get_Sound(ini, ID, "ImpactWaterSound", ImpactWaterSound);
        ImpactLandSound = Get_Sound(ini, ID, "ImpactLandSound", ImpactLandSound);
        CrashingSound = Get_Sound(ini, ID, "CrashingSound", CrashingSound);
        SinkingSound = Get_Sound(ini, ID, "SinkingSound", SinkingSound);
        VoiceFalling = Get_Sound(ini, ID, "VoiceFalling", VoiceFalling);
        VoiceCrashing = Get_Sound(ini, ID, "VoiceCrashing", VoiceCrashing);
        VoiceSinking = Get_Sound(ini, ID, "VoiceSinking", VoiceSinking);
        VoiceEnter = Get_Sound(ini, ID, "VoiceEnter", VoiceEnter);
        VoiceCapture = Get_Sound(ini, ID, "VoiceCapture", VoiceCapture);
        CloakStop = ini.Get_Bool(ID, "CloakStop", CloakStop);
        Storage = ini.Get_Int(ID, "Storage", Storage);
        BuildLimit = ini.Get_Int(ID, "BuildLimit", BuildLimit);
        Category = ini.Get_CategoryType(ID, "Category", (CategoryType)Category);
        Dock = TypeListReader<BuildingTypeClass>::Get(ini, ID, "Dock", Dock);
        DeploysInto = Get_Type(ini, ID, "DeploysInto", DeploysInto);
        UndeploysInto = Get_Type(ini, ID, "UndeploysInto", UndeploysInto);
        LightningRod = ini.Get_Bool(ID, "LightningRod", LightningRod);
        ManualReload = ini.Get_Bool(ID, "ManualReload", ManualReload);
        TurretSpins = ini.Get_Bool(ID, "TurretSpins", TurretSpins);
        Turret = ini.Get_Bool(ID, "Turret", Turret);
        TurretRotateSound = Get_Sound(ini, ID, "TurretRotateSound", TurretRotateSound);
        EnterTransportSound = Get_Sound(ini, ID, "EnterTransportSound", EnterTransportSound);
        LeaveTransportSound = Get_Sound(ini, ID, "LeaveTransportSound", LeaveTransportSound);
        MoveSound = Get_Voc_List(ini, ID, "MoveSound", MoveSound);
        DieSound = Get_Voc_List(ini, ID, "DieSound", DieSound);
        DeploySound = Get_Sound(ini, ID, "DeploySound", DeploySound);
        UndeploySound = Get_Sound(ini, ID, "UndeploySound", UndeploySound);
        ChronoInSound = Get_Sound(ini, ID, "ChronoInSound", ChronoInSound);
        ChronoOutSound = Get_Sound(ini, ID, "ChronoOutSound", ChronoOutSound);
        Explosion = TypeListReader<AnimTypeClass>::Get(ini, ID, "Explosion", Explosion);
        DestroyAnim = TypeListReader<AnimTypeClass>::Get(ini, ID, "DestroyAnim", DestroyAnim);
        NaturalParticleSystem = Get_Type(ini, ID, "NaturalParticleSystem", NaturalParticleSystem);
        (INIPoint3D &)NaturalParticleLocation = ini.Get_Integer_Triple(ID, "NaturalParticleLocation", (INIPoint3D &)NaturalParticleLocation);
        DamageParticleSystems = TypeListReader<ParticleSystemTypeClass>::Get(ini, ID, "DamageParticleSystems", DamageParticleSystems);
        DestroyParticleSystems = TypeListReader<ParticleSystemTypeClass>::Get(ini, ID, "DestroyParticleSystems", DestroyParticleSystems);
        (INIPoint3D &)DamageSmokeOffset = ini.Get_Integer_Triple(ID, "DamageSmokeOffset", (INIPoint3D &)DamageSmokeOffset);
        DamSmkOffScrnRel = ini.Get_Bool(ID, "DamSmkOffScrnRel", DamSmkOffScrnRel);
        (INIPoint3D &)DestroySmokeOffset = ini.Get_Integer_Triple(ID, "DestroySmokeOffset", (INIPoint3D &)DestroySmokeOffset);
        (INIPoint3D &)RefnSmokeOffsetOne = ini.Get_Integer_Triple(ID, "RefnSmokeOffsetOne", (INIPoint3D &)RefnSmokeOffsetOne);
        (INIPoint3D &)RefnSmokeOffsetTwo = ini.Get_Integer_Triple(ID, "RefnSmokeOffsetTwo", (INIPoint3D &)RefnSmokeOffsetTwo);
        Nominal = ini.Get_Bool(ID, "Nominal", Nominal);
        Cloakable = ini.Get_Bool(ID, "Cloakable", Cloakable);
        GapGenerator = ini.Get_Bool(ID, "GapGenerator", GapGenerator);
        GapRadiusInCells = ini.Get_Int(ID, "GapRadiusInCells", GapRadiusInCells);
        SuperGapRadiusInCells = ini.Get_Int(ID, "SuperGapRadiusInCells", SuperGapRadiusInCells);
        Teleporter = ini.Get_Bool(ID, "Teleporter", Teleporter);
        Sensors = ini.Get_Bool(ID, "Sensors", Sensors);
        PipScale = ini.Get_PipScale(ID, "PipScale", PipScale);
        PipWrap = ini.Get_Int(ID, "PipWrap", PipWrap);
        Prerequisite = Get_Prerequisite_List(ini, ID, "Prerequisite", Prerequisite);
        PrerequisiteOverride = Get_Prerequisite_List(ini, ID, "PrerequisiteOverride", PrerequisiteOverride);
        Sight = ini.Get_Int(ID, "Sight", Sight);
        ReselectIfLimboed = ini.Get_Bool(ID, "ReselectIfLimboed", ReselectIfLimboed);
        RejoinTeamIfLimboed = ini.Get_Bool(ID, "RejoinTeamIfLimboed", RejoinTeamIfLimboed);
        SensorsSight = ini.Get_Int(ID, "SensorsSight", SensorsSight);
        DetectDisguiseRange = ini.Get_Int(ID, "DetectDisguiseRange", DetectDisguiseRange);
        BombSight = ini.Get_Int(ID, "BombSight", BombSight);
        LeadershipRating = ini.Get_Int(ID, "LeadershipRating", LeadershipRating);
        RevealToAll = ini.Get_Bool(ID, "RevealToAll", RevealToAll);
        CanDisguise = ini.Get_Bool(ID, "CanDisguise", CanDisguise);
        PermaDisguise = ini.Get_Bool(ID, "PermaDisguise", PermaDisguise);
        DetectDisguise = ini.Get_Bool(ID, "DetectDisguise", DetectDisguise);
        DisguiseWhenStill = ini.Get_Bool(ID, "DisguiseWhenStill", DisguiseWhenStill);
        CanPassiveAquire = ini.Get_Bool(ID, "CanPassiveAquire", CanPassiveAquire);
        CanRetaliate = ini.Get_Bool(ID, "CanRetaliate", CanRetaliate);
        CanApproachTarget = ini.Get_Bool(ID, "CanApproachTarget", CanApproachTarget);
        CanRecalcApproachTarget = ini.Get_Bool(ID, "CanRecalcApproachTarget", CanRecalcApproachTarget);
        RequiresStolenSovietTech = ini.Get_Bool(ID, "RequiresStolenSovietTech", RequiresStolenSovietTech);
        RequiresStolenAlliedTech = ini.Get_Bool(ID, "RequiresStolenAlliedTech", RequiresStolenAlliedTech);
        RequiredHouses = ini.Get_Owners(ID, "RequiredHouses", RequiredHouses);
        ForbiddenHouses = ini.Get_Owners(ID, "ForbiddenHouses", ForbiddenHouses);
        TechLevel = ini.Get_Int(ID, "TechLevel", TechLevel);
        int speed = ini.Get_Int(ID, "Speed", -1);
        if (speed != -1) {
            if (speed >= 100) {
                speed = 100;
            }
            speed = speed <= 0 ? 0 : speed;
            int value = speed * 256 / 100;
            Speed = value < 255 ? value : 255;
        }
        Cost = ini.Get_Int(ID, "Cost", Cost);
        Soylent = ini.Get_Int(ID, "Soylent", Soylent);
        UnloadingClass = Get_Type(ini, ID, "UnloadingClass", UnloadingClass);
        InitialAmmo = ini.Get_Int(ID, "InitialAmmo", InitialAmmo);
        Ammo = ini.Get_Int(ID, "Ammo", Ammo);
        IFVMode = ini.Get_Int(ID, "IFVMode", IFVMode);
        AirRangeBonus = ini.Get_Lepton(ID, "AirRangeBonus", AirRangeBonus);
        RadialFireSegments = ini.Get_Int(ID, "RadialFireSegments", RadialFireSegments);
        DeployFireWeapon = ini.Get_Int(ID, "DeployFireWeapon", DeployFireWeapon);
        DeployFire = ini.Get_Bool(ID, "DeployFire", DeployFire);
        MobileFire = ini.Get_Bool(ID, "MobileFire", MobileFire);
        OpportunityFire = ini.Get_Bool(ID, "OpportunityFire", OpportunityFire);
        DistributedFire = ini.Get_Bool(ID, "DistributedFire", DistributedFire);
        Reload = ini.Get_Int(ID, "Reload", Reload);
        EmptyReload = ini.Get_Int(ID, "EmptyReload", EmptyReload);
        ReloadIncrement = ini.Get_Int(ID, "ReloadIncrement", ReloadIncrement);
        DamageReducesReadiness = ini.Get_Bool(ID, "DamageReducesReadiness", DamageReducesReadiness);
        ReadinessReductionMultiplier = ini.Get_Double(ID, "ReadinessReductionMultiplier", ReadinessReductionMultiplier);
        PreventAttackMove = ini.Get_Bool(ID, "PreventAttackMove", PreventAttackMove);
        Points = ini.Get_Int(ID, "Points", Points);
        unknown_59c = Points;
        ThreatPosed = ini.Get_Int(ID, "ThreatPosed", ThreatPosed);
        Owner = ini.Get_Owners(ID, "Owner", Owner);
        AIBasePlanningSide = ini.Get_Int(ID, "AIBasePlanningSide", AIBasePlanningSide);
        Trainable = ini.Get_Bool(ID, "Trainable", Trainable);
        Crewed = ini.Get_Bool(ID, "Crewed", Crewed);
        Gunner = ini.Get_Bool(ID, "Gunner", Gunner);
        Naval = ini.Get_Bool(ID, "Naval", Naval);
        Repairable = ini.Get_Bool(ID, "Repairable", Repairable);
        Invisible = ini.Get_Bool(ID, "Invisible", Invisible);
        RadarVisible = ini.Get_Bool(ID, "RadarVisible", RadarVisible);
        SelfHealing = ini.Get_Bool(ID, "SelfHealing", SelfHealing);
        NoAutoFire = ini.Get_Bool(ID, "NoAutoFire", NoAutoFire);
        ROT = ini.Get_Int(ID, "ROT", ROT);
        Passengers = ini.Get_Int(ID, "Passengers", Passengers);
        FireAngle = ini.Get_Int(ID, "FireAngle", FireAngle);
        DeployTime = ini.Get_Double(ID, "DeployTime", DeployTime);
        UndeployDelay = ini.Get_Int(ID, "UndeployDelay", UndeployDelay);
        Disableable = ini.Get_Bool(ID, "Disableable", Disableable);
        ToProtect = ini.Get_Bool(ID, "ToProtect", ToProtect);
        TiberiumHeal = ini.Get_Bool(ID, "TiberiumHeal", TiberiumHeal);
        ImmuneToVeins = ini.Get_Bool(ID, "ImmuneToVeins", ImmuneToVeins);
        AllowedToStartInMultiplayer = ini.Get_Bool(ID, "AllowedToStartInMultiplayer", AllowedToStartInMultiplayer);
        StupidHunt = ini.Get_Bool(ID, "StupidHunt", StupidHunt);
        TargetLaser = ini.Get_Bool(ID, "TargetLaser", TargetLaser);
        HunterSeeker = ini.Get_Bool(ID, "HunterSeeker", HunterSeeker);
        Crusher = ini.Get_Bool(ID, "Crusher", Crusher);
        AutoCrush = ini.Get_Bool(ID, "AutoCrush", AutoCrush);
        ImmuneToRadiation = ini.Get_Bool(ID, "ImmuneToRadiation", ImmuneToRadiation);
        Spawned = ini.Get_Bool(ID, "Spawned", Spawned);
        Underwater = ini.Get_Bool(ID, "Underwater", Underwater);
        BalloonHover = ini.Get_Bool(ID, "BalloonHover", BalloonHover);
        Spawns = Get_Type(ini, ID, "Spawns", Spawns);
        SpawnRegenRate = ini.Get_Int(ID, "SpawnRegenRate", SpawnRegenRate);
        SpawnsNumber = ini.Get_Int(ID, "SpawnsNumber", SpawnsNumber);
        SpawnReloadRate = ini.Get_Int(ID, "SpawnReloadRate", SpawnReloadRate);
        MissileSpawn = ini.Get_Bool(ID, "MissileSpawn", MissileSpawn);
        DefaultToGuardArea = ini.Get_Bool(ID, "DefaultToGuardArea", DefaultToGuardArea);
        Warpable = ini.Get_Bool(ID, "Warpable", Warpable);
        Parasiteable = ini.Get_Bool(ID, "Parasiteable", Parasiteable);
        ImmuneToPsionics = ini.Get_Bool(ID, "ImmuneToPsionics", ImmuneToPsionics);
        ConsideredAircraft = ini.Get_Bool(ID, "ConsideredAircraft", ConsideredAircraft);
        Organic = ini.Get_Bool(ID, "Organic", Organic);
        SuppressionThreshold = ini.Get_Int(ID, "SuppressionThreshold", SuppressionThreshold);
        NoShadow = ini.Get_Bool(ID, "NoShadow", NoShadow);
        JumpjetTurnRate = ini.Get_Int(ID, "JumpjetTurnRate", JumpjetTurnRate);
        JumpjetSpeed = ini.Get_Int(ID, "JumpjetSpeed", JumpjetSpeed);
        JumpjetClimb = ini.Get_Double(ID, "JumpjetClimb", JumpjetClimb);
        JumpjetCrash = ini.Get_Double(ID, "JumpjetCrash", JumpjetCrash);
        JumpjetHeight = ini.Get_Int(ID, "JumpjetHeight", JumpjetHeight);
        JumpjetAccel = ini.Get_Double(ID, "JumpjetAccel", JumpjetAccel);
        JumpjetWobbles = ini.Get_Double(ID, "JumpjetWobbles", JumpjetWobbles);
        JumpjetNoWobbles = ini.Get_Bool(ID, "JumpjetNoWobbles", JumpjetNoWobbles);
        JumpjetDeviation = ini.Get_Int(ID, "JumpjetDeviation", JumpjetDeviation);
        JumpJet = ini.Get_Bool(ID, "JumpJet", JumpJet);
        Crashable = ini.Get_Bool(ID, "Crashable", Crashable);
        AttackFriendlies = ini.Get_Bool(ID, "AttackFriendlies", AttackFriendlies);
        AttackCursorOnFriendlies = ini.Get_Bool(ID, "AttackCursorOnFriendlies", AttackCursorOnFriendlies);
        TurretRecoil = ini.Get_Bool(ID, "TurretRecoil", TurretRecoil);
        TurretAnimData.Travel = Get_Recoil(ini, ID, "TurretTravel", TurretAnimData.Travel);
        TurretAnimData.CompressFrames = Get_Recoil_Frames(ini, ID, "TurretCompressFrames", TurretAnimData.CompressFrames);
        TurretAnimData.HoldFrames = Get_Recoil_Frames(ini, ID, "TurretHoldFrames", TurretAnimData.HoldFrames);
        TurretAnimData.RecoverFrames = Get_Recoil_Frames(ini, ID, "TurretRecoverFrames", TurretAnimData.RecoverFrames);
        BarrelAnimData = TurretAnimData;
        BarrelAnimData.Travel = Get_Recoil(ini, ID, "BarrelTravel", TurretAnimData.Travel);
        BarrelAnimData.CompressFrames = Get_Recoil_Frames(ini, ID, "BarrelCompressFrames", TurretAnimData.CompressFrames);
        BarrelAnimData.HoldFrames = Get_Recoil_Frames(ini, ID, "BarrelHoldFrames", TurretAnimData.HoldFrames);
        BarrelAnimData.RecoverFrames = Get_Recoil_Frames(ini, ID, "BarrelRecoverFrames", TurretAnimData.RecoverFrames);
        TiltsWhenCrushes = ini.Get_Bool(ID, "TiltsWhenCrushes", TiltsWhenCrushes);
        Accelerates = ini.Get_Bool(ID, "Accelerates", Accelerates);
        ZFudgeCliff = ini.Get_Int(ID, "ZFudgeCliff", ZFudgeCliff);
        ZFudgeColumn = ini.Get_Int(ID, "ZFudgeColumn", ZFudgeColumn);
        ZFudgeTunnel = ini.Get_Int(ID, "ZFudgeTunnel", ZFudgeTunnel);
        ZFudgeBridge = ini.Get_Int(ID, "ZFudgeBridge", ZFudgeBridge);
        VeteranAbilities = ini.Get_Abilities(ID, "VeteranAbilities", VeteranAbilities);
        EliteAbilities = ini.Get_Abilities(ID, "EliteAbilities", EliteAbilities);
        MyEffectivenessCoefficient = ini.Get_Double(ID, "MyEffectivenessCoefficient", MyEffectivenessCoefficient == 0.0 ? Rules->unknown_d00 : MyEffectivenessCoefficient);
        TargetEffectivenessCoefficient = ini.Get_Double(ID, "TargetEffectivenessCoefficient", TargetEffectivenessCoefficient == 0.0 ? Rules->unknown_d08 : TargetEffectivenessCoefficient);
        TargetSpecialThreatCoefficient = ini.Get_Double(ID, "TargetSpecialThreatCoefficient", TargetSpecialThreatCoefficient == 0.0 ? Rules->unknown_d10 : TargetSpecialThreatCoefficient);
        TargetStrengthCoefficient = ini.Get_Double(ID, "TargetStrengthCoefficient", TargetStrengthCoefficient == 0.0 ? Rules->unknown_d18 : TargetStrengthCoefficient);
        TargetDistanceCoefficient = ini.Get_Double(ID, "TargetDistanceCoefficient", TargetDistanceCoefficient == 0.0 ? Rules->unknown_d20 : TargetDistanceCoefficient);
        SpecialThreatValue = ini.Get_Double(ID, "SpecialThreatValue", SpecialThreatValue);
        IsSelectableCombatant = ini.Get_Bool(ID, "IsSelectableCombatant", IsSelectableCombatant);

        unknown_aa8 = false;
        if (Weapon[0].WeaponType != 0 && Weapon[0].WeaponType->unknown_a4 > 0) {
            unknown_aa8 = true;
        }

        char fullname[512];
        _makepath(fullname, 0, 0, unknown_1c8, ".SHP");
        if (unknown_a8 && unknown_a4 != 0) {
            delete unknown_a4;
            unknown_a4 = 0;
            unknown_a8 = false;
        }
        unknown_a4 = (void *)MixFileClass::Retrieve(fullname);

        ArtINI.Get_String(unknown_1c8, "Palette", "", PaletteFile.Buffer, sizeof(PaletteFile.Buffer));
        Load_Palette();
        if (Unknown_00603D50() > 1) {
            Debug_Printf("You have violated the limit of having only one extra building palette.\n");
            Emergency_Exit(-1);
        }
        TurretOffset = ArtINI.Get_Int(unknown_1c8, "TurretOffset", TurretOffset);
        RotCount = ArtINI.Get_Int(unknown_1c8, "RotCount", RotCount);
        Remapable = ArtINI.Get_Bool(unknown_1c8, "Remapable", Remapable);
        Normalized = ArtINI.Get_Bool(unknown_1c8, "Normalized", Normalized);
        VisibleLoad = ArtINI.Get_Bool(unknown_1c8, "VisibleLoad", VisibleLoad);
        ShadowIndex = ArtINI.Get_Int(unknown_1c8, "ShadowIndex", ShadowIndex);
        DisableVoxelCache = ArtINI.Get_Bool(unknown_1c8, "DisableVoxelCache", DisableVoxelCache);
        DisableShadowCache = ArtINI.Get_Bool(unknown_1c8, "DisableShadowCache", DisableShadowCache);
        char cameo[32];
        ArtINI.Get_String(unknown_1c8, "Cameo", "", cameo, sizeof(CameoFile));
        if (strlen(cameo) != 0) {
            strcpy(CameoFile.Buffer, cameo);
            _makepath(fullname, 0, 0, CameoFile, ".SHP");
            if (CameoData == 0) {
                CameoData = (void *)MixFileClass::Retrieve(fullname);
            }
        }
        if (CameoData == 0) {
            CameoData = (void *)MixFileClass::Retrieve("XXICON.SHP");
        }
        ArtINI.Get_String(unknown_1c8, "AltCameo", "", cameo, sizeof(AltCameoFile));
        if (strlen(cameo) != 0) {
            strcpy(AltCameoFile.Buffer, cameo);
            _makepath(fullname, 0, 0, AltCameoFile, ".SHP");
            if (AltCameoData == 0) {
                AltCameoData = (void *)MixFileClass::Retrieve(fullname);
            }
        }

        if (TurretCount > 0) {
            for (int index = 0; index < WeaponCount; index++) {
                char weapon[32];
                char entry[128];
                sprintf(weapon, "Weapon%d", index + 1);
                sprintf(entry, "%sFLH", weapon);
                (INIPoint3D &)Get_Weapon(index)->FLH = ArtINI.Get_Point(unknown_1c8, entry, (INIPoint3D &)Get_Weapon(index)->FLH);
                sprintf(entry, "%sBarrelLength", weapon);
                Get_Weapon(index)->BarrelLength = ArtINI.Get_Int(unknown_1c8, entry, Get_Weapon(index)->BarrelLength);
                sprintf(entry, "%sBarrelThickness", weapon);
                Get_Weapon(index)->BarrelThickness = ArtINI.Get_Int(unknown_1c8, entry, Get_Weapon(index)->BarrelThickness);
                sprintf(entry, "%sTurretLocked", weapon);
                Get_Weapon(index)->TurretLocked = ArtINI.Get_Bool(unknown_1c8, entry, Get_Weapon(index)->TurretLocked);

                sprintf(weapon, "EliteWeapon%d", index + 1);
                sprintf(entry, "%sFLH", weapon);
                (INIPoint3D &)Get_Elite_Weapon(index)->FLH = ArtINI.Get_Point(unknown_1c8, entry, (INIPoint3D &)Get_Weapon(index)->FLH);
                sprintf(entry, "%sBarrelLength", weapon);
                Get_Elite_Weapon(index)->BarrelLength = ArtINI.Get_Int(unknown_1c8, entry, Get_Weapon(index)->BarrelLength);
                sprintf(entry, "%sBarrelThickness", weapon);
                Get_Elite_Weapon(index)->BarrelThickness = ArtINI.Get_Int(unknown_1c8, entry, Get_Weapon(index)->BarrelThickness);
                sprintf(entry, "%sTurretLocked", weapon);
                Get_Elite_Weapon(index)->TurretLocked = ArtINI.Get_Bool(unknown_1c8, entry, Get_Weapon(index)->TurretLocked);
            }
        } else {
            (INIPoint3D &)Weapon[0].FLH = ArtINI.Get_Point(unknown_1c8, "PrimaryFireFLH", (INIPoint3D &)Weapon[0].FLH);
            Weapon[0].BarrelLength = ArtINI.Get_Int(unknown_1c8, "PBarrelLength", Weapon[0].BarrelLength);
            Weapon[0].BarrelThickness = ArtINI.Get_Int(unknown_1c8, "PBarrelThickness", Weapon[0].BarrelThickness);
            (INIPoint3D &)Weapon[1].FLH = ArtINI.Get_Point(unknown_1c8, "SecondaryFireFLH", (INIPoint3D &)Weapon[1].FLH);
            Weapon[1].BarrelLength = ArtINI.Get_Int(unknown_1c8, "SBarrelLength", Weapon[1].BarrelLength);
            Weapon[1].BarrelThickness = ArtINI.Get_Int(unknown_1c8, "SBarrelThickness", Weapon[1].BarrelThickness);
            (INIPoint3D &)EliteWeapon[0].FLH = ArtINI.Get_Point(unknown_1c8, "ElitePrimaryFireFLH", (INIPoint3D &)Weapon[0].FLH);
            EliteWeapon[0].BarrelLength = ArtINI.Get_Int(unknown_1c8, "ElitePBarrelLength", Weapon[0].BarrelLength);
            EliteWeapon[0].BarrelThickness = ArtINI.Get_Int(unknown_1c8, "ElitePBarrelThickness", Weapon[0].BarrelThickness);
            (INIPoint3D &)EliteWeapon[1].FLH = ArtINI.Get_Point(unknown_1c8, "EliteSecondaryFireFLH", (INIPoint3D &)Weapon[1].FLH);
            EliteWeapon[1].BarrelLength = ArtINI.Get_Int(unknown_1c8, "EliteSBarrelLength", Weapon[1].BarrelLength);
            EliteWeapon[1].BarrelThickness = ArtINI.Get_Int(unknown_1c8, "EliteSBarrelThickness", Weapon[1].BarrelThickness);
        }
        TurretNotExportedOnGround = ArtINI.Get_Bool(unknown_1c8, "TurretNotExportedOnGround", TurretNotExportedOnGround);
        MovementZone = ini.Get_MovementZone(ID, "MovementZone", MovementZone);
        unknown_af5 = MovementZone == 6;

        if (unknown_206) {
            Fetch_Voxel_Image();
        } else if (Turret) {
            Fetch_Aux_Voxel_Image();
        }
        if (unknown_b0.unknown_00 != 0 && !unknown_b0.unknown_00->LoadFailed) {
            unknown_330 = unknown_b0.unknown_00->Get_Layer_Info(0, 0)->unknown_a1 * 0.5f + 0.5;
            unknown_338 = unknown_b0.unknown_00->Get_Layer_Info(0, 0)->unknown_a0 * 0.5f;
        }

        return true;
    }
    return false;
}


// bool TechnoTypeClass::Legal_Placement(Cell const &cell, HouseClass *house) const (0x006DE0E0, slot 42)
// Whether the type can be placed at a cell (hypothesis: Legal_Placement):
// every cell of its occupy list (slot 36) must be open for it, by
// 0x004718B0 for building types (Kind_Of 7) and 0x00478760 otherwise; a
// building type with a +0xBF0 tile type (BuildingTypeClass::unknown_bf0) needs only
// one open cell.

// Member-wise sum of two cells: shared/coord.hpp.

bool TechnoTypeClass::Legal_Placement(Cell const &cell, HouseClass *house) const
{
    if (cell == unknown_geometry_k) {
        return false;
    }
    Cell const *offset = Occupy_List(1);
    bool building = Kind_Of() == 7;
    bool blocked = false;
    bool open = false;
    while (offset && *offset != Cell(0x7FFF, 0x7FFF)) {
        Cell place = cell + *offset++;
        if (building) {
            if (!((UnknownPlacementCellView &)Map[place]).Unknown_004718B0(SpeedType, this, house)) {
                blocked = true;
            } else {
                open = true;
            }
        } else {
            if (!((UnknownPlacementCellView &)Map[place]).Unknown_00478760(SpeedType, false, false, -1, 0, -1, true)) {
                blocked = true;
            } else {
                open = true;
            }
        }
    }
    if (building && static_cast<BuildingTypeClass const *>(this)->unknown_bf0) {
        return open;
    }
    return !blocked;
}


// int TechnoTypeClass::Max_Pips() const (0x006DE220)
// Pip count for PipScale= (hypothesis: RA1 Max_Pips); the jump table follows
// the code (0x006DE250, 4 entries).

int TechnoTypeClass::Max_Pips() const
{
    switch (PipScale) {
    case 4:
        return 10;
    case 1:
        return Ammo;
    case 2:
        return 5;
    case 3:
        return Passengers;
    }
    return 0;
}


// HRESULT __stdcall TechnoTypeClass::Load(IStream *stream) (0x006DE260)
// Empties the lists, loads the object type, fixes up the type pointers,
// rebuilds every list from its saved count and elements and fixes up the
// pointer lists and weapons, then reloads the image, the cameos (XXICON when
// none) and the palette from the art INI.

extern CCINIClass unknown_art_ini_8397f0;
extern "C" void __cdecl _makepath(char *path, char const *drive, char const *dir, char const *fname, char const *ext);
extern "C" int __cdecl _stricmp(char const *string1, char const *string2);

// Rebuilds a list in place from its saved count and elements.
template <class T>
static inline void Load_List(IStream *stream, TypeList<T> &list)
{
    new (&list) TypeList<T>;
    int count;
    stream->Read(&count, sizeof(count), 0);
    for (int index = 0; index < count; index++) {
        T value;
        stream->Read(&value, sizeof(value), 0);
        list.Add(value);
    }
}

HRESULT __stdcall TechnoTypeClass::Load(IStream *stream)
{
    Dock.Clear();
    DebrisTypes.Clear();
    DebrisAnims.Clear();
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

    HRESULT result = ObjectTypeClass::Load(stream);
    if (SUCCEEDED(result)) {

    Swizzle_Pointer(DeploysInto);
    Swizzle_Pointer(UndeploysInto);
    Swizzle_Pointer(NaturalParticleSystem);
    Swizzle_Pointer(Spawns);
    Swizzle_Pointer(DeathWeapon);
    Swizzle_Pointer(UnloadingClass);

    Load_List(stream, Dock);
    Load_List(stream, DebrisTypes);
    Load_List(stream, DebrisAnims);
    Load_List(stream, VoiceSelect);
    Load_List(stream, VoiceMove);
    Load_List(stream, VoiceAttack);
    Load_List(stream, VoiceSpecialAttack);
    Load_List(stream, VoiceDie);
    Load_List(stream, VoiceFeedback);
    Load_List(stream, MoveSound);
    Load_List(stream, DieSound);
    Load_List(stream, Explosion);
    Load_List(stream, DestroyAnim);
    Load_List(stream, DebrisMaximums);
    Load_List(stream, Prerequisite);
    Load_List(stream, PrerequisiteOverride);
    Load_List(stream, DamageParticleSystems);
    Load_List(stream, DestroyParticleSystems);

    int index;
    for (index = 0; index < Dock.Count(); index++) {
        Swizzle_Pointer(Dock[index]);
    }
    for (index = 0; index < Explosion.Count(); index++) {
        Swizzle_Pointer(Explosion[index]);
    }
    for (index = 0; index < DestroyAnim.Count(); index++) {
        Swizzle_Pointer(DestroyAnim[index]);
    }
    for (index = 0; index < DebrisTypes.Count(); index++) {
        Swizzle_Pointer(DebrisTypes[index]);
    }
    for (index = 0; index < DebrisAnims.Count(); index++) {
        Swizzle_Pointer(DebrisAnims[index]);
    }
    for (index = 0; index < 15; index++) {
        Swizzle_Pointer(Weapon[index].WeaponType);
    }
    for (index = 0; index < 15; index++) {
        Swizzle_Pointer(EliteWeapon[index].WeaponType);
    }
    for (index = 0; index < DamageParticleSystems.Count(); index++) {
        Swizzle_Pointer(DamageParticleSystems[index]);
    }
    for (index = 0; index < DestroyParticleSystems.Count(); index++) {
        Swizzle_Pointer(DestroyParticleSystems[index]);
    }

    char name[256];
    char filename[260];
    _makepath(filename, 0, 0, unknown_1c8, ".SHP");
    unknown_a4 = (void *)MixFileClass::Retrieve(filename);

    unknown_art_ini_8397f0.Get_String(unknown_1c8, "Cameo", "XXICON", name, sizeof(name));
    if (_stricmp(name, "XXICON") == 0) {
        unknown_art_ini_8397f0.Get_String(ID, "Cameo", "XXICON", name, sizeof(name));
    }
    _makepath(filename, 0, 0, name, ".SHP");
    CameoData = 0;
    CameoData = (void *)MixFileClass::Retrieve(filename);
    if (CameoData == 0) {
        CameoData = (void *)MixFileClass::Retrieve("XXICON.SHP");
    }

    unknown_art_ini_8397f0.Get_String(unknown_1c8, "AltCameo", unknown_string_83c55c, name, sizeof(name));
    _makepath(filename, 0, 0, name, ".SHP");
    AltCameoData = 0;
    AltCameoData = (void *)MixFileClass::Retrieve(filename);
    PaletteName *palette = &PaletteFile;

    unknown_art_ini_8397f0.Get_String(unknown_1c8, "Palette", unknown_string_83c55c, palette->Buffer, sizeof(palette->Buffer));
    if (palette->Buffer[0] != 0) {
        Palette = palette->Load();
    }
    result = S_OK;
    }
    return result;
}


// HRESULT __stdcall TechnoTypeClass::Save(IStream *stream, BOOL clear_dirty) (0x006DEC10)
// The base, then every list (count and elements).

HRESULT __stdcall TechnoTypeClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = ObjectTypeClass::Save(stream, clear_dirty);
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


// HRESULT __stdcall TechnoTypeClass::GetSizeMax(ULARGE_INTEGER *size) (0x006DEEE0)
// The base size plus a count and the elements of sixteen lists.

HRESULT __stdcall TechnoTypeClass::GetSizeMax(ULARGE_INTEGER *size)
{
    HRESULT result = ObjectTypeClass::GetSizeMax(size);
    if (FAILED(result)) {
        return result;
    }
    size->LowPart += VoiceSelect.Count() * sizeof(int) + sizeof(int);
    size->LowPart += VoiceMove.Count() * sizeof(int) + sizeof(int);
    size->LowPart += VoiceAttack.Count() * sizeof(int) + sizeof(int);
    size->LowPart += VoiceSpecialAttack.Count() * sizeof(int) + sizeof(int);
    size->LowPart += VoiceDie.Count() * sizeof(int) + sizeof(int);
    size->LowPart += VoiceFeedback.Count() * sizeof(int) + sizeof(int);
    size->LowPart += MoveSound.Count() * sizeof(int) + sizeof(int);
    size->LowPart += DieSound.Count() * sizeof(int) + sizeof(int);
    size->LowPart += Explosion.Count() * sizeof(AnimTypeClass *) + sizeof(int);
    size->LowPart += DestroyAnim.Count() * sizeof(AnimTypeClass *) + sizeof(int);
    size->LowPart += DebrisTypes.Count() * sizeof(VoxelAnimTypeClass *) + sizeof(int);
    size->LowPart += DebrisAnims.Count() * sizeof(AnimTypeClass *) + sizeof(int);
    size->LowPart += Dock.Count() * sizeof(BuildingTypeClass *) + sizeof(int);
    size->LowPart += DebrisMaximums.Count() * sizeof(int) + sizeof(int);
    size->LowPart += DamageParticleSystems.Count() * sizeof(ParticleSystemTypeClass *) + sizeof(int);
    size->LowPart += DestroyParticleSystems.Count() * sizeof(ParticleSystemTypeClass *) + sizeof(int);
    return S_OK;
}


// void TechnoTypeClass::Compute_CRC(WWCRCEngine &crc) const (0x006DEFD0)
// The base CRC, then the rules values in Westwood's order (list counts,
// not their elements).

void TechnoTypeClass::Compute_CRC(WWCRCEngine &crc) const
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
    crc(unknown_330);
    crc(unknown_338);
    crc(Weight);
    crc(PhysicalSize);
    crc(Size);
    crc(SizeLimit);
    crc(HoverAttack);
    crc((long)unknown_368);
    crc(ImmuneToVeins);
    crc(TiberiumHeal);
    crc(TargetLaser);
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
    crc((long)MovementZone);
    crc((long)GuardRange.Value);
    crc((long)MaxDebris);
    crc((long)MinDebris);
    crc((long)Passengers);
    crc((long)Sight);
    crc((long)Cost);
    crc((long)Soylent);
    crc((long)TechLevel);
    crc((long)Prerequisite.Count());
    crc((long)PrerequisiteOverride.Count());
    crc((long)ThreatPosed);
    crc((long)unknown_59c);
    crc((long)Speed);
    crc((long)SpeedType);
    crc((long)Ammo);
    crc((long)AirRangeBonus);
    crc((long)Owner);
    crc((long)AIBasePlanningSide);
    crc((long)RotCount);
    crc((long)ROT);
    crc((long)TurretOffset);
    crc((long)Points);
    crc((long)Explosion.Count());
    crc((long)DestroyAnim.Count());
    crc((long)ShadowIndex);
    crc((long)Storage);
    crc(IsTrain);
    crc(IsDropship);
    crc(ToProtect);
    crc(Disableable);
    crc(unknown_aa4);
    crc(DoubleOwned);
    crc(Invisible);
    crc(RadarVisible);
    crc(unknown_aa8);
    crc(Sensors);
    crc(Nominal);
    crc(Turret);
    crc(Repairable);
    crc(Crewed);
    crc(Remapable);
    crc(Cloakable);
    crc(GapGenerator);
    crc(GapRadiusInCells);
    crc(SuperGapRadiusInCells);
    crc(Teleporter);
    crc(SelfHealing);
    crc(Explodes);
    crc(NoAutoFire);
    crc(TurretSpins);
    crc(Normalized);
    crc(ManualReload);
    crc(VisibleLoad);
    crc(LightningRod);
    crc(DisableVoxelCache);
    crc(DisableShadowCache);
    crc(MobileFire);
    crc(OpportunityFire);
    crc(DistributedFire);
}


// WeaponStruct *TechnoTypeClass::Get_Weapon(int index) (0x006DF5C0)
// Weapon slot `index`.

WeaponStruct *TechnoTypeClass::Get_Weapon(int index)
{
    return &Weapon[index];
}


// WeaponStruct *TechnoTypeClass::Get_Elite_Weapon(int index) (0x006DF5E0)
// Elite weapon slot `index`.

WeaponStruct *TechnoTypeClass::Get_Elite_Weapon(int index)
{
    return &EliteWeapon[index];
}


// int TechnoTypeClass::Flight_Level() const (0x006DF600)
// FlightLevel=, or the rules default when unset (-1) (slot 47).

int TechnoTypeClass::Flight_Level() const
{
    if (FlightLevel == -1) {
        return Rules->unknown_6dc;
    }
    return FlightLevel;
}


// void TechnoTypeClass::Load_Palette() (0x006DF620)
// Loads Palette= when named.

void TechnoTypeClass::Load_Palette()
{
    if (PaletteFile.Buffer[0]) {
        Palette = PaletteFile.Load();
    }
}


// static void __fastcall TechnoTypeClass::Load_Palettes() (0x006DF640; __fastcall: Init_Theater 0x00519E70 sets the next call's ecx before its push)
// Load_Palette for every type. Not in the function inventory: the gap
// 0x006DF640-0x006DF67F holds this body (61 bytes) and padding.

void __fastcall TechnoTypeClass::Load_Palettes()
{
    for (int index = 0; index < TechnoTypes.Count(); index++) {
        TechnoTypes[index]->Load_Palette();
    }
}


// bool TechnoTypeClass::Has_Turret_Weapons() const (0x006DF680)
// TurretCount= is positive.

bool TechnoTypeClass::Has_Turret_Weapons() const
{
    return TurretCount > 0;
}


// void TechnoTypeClass::Set_Turret_Weapon(int weapon, int index) (0x006DF690)

void TechnoTypeClass::Set_Turret_Weapon(int weapon, int index)
{
    unknown_710[index] = weapon;
}


// int TechnoTypeClass::Get_Turret_Weapon(int index) const (0x006DF6B0)

int TechnoTypeClass::Get_Turret_Weapon(int index) const
{
    return unknown_710[index];
}


// CoordStruct TechnoTypeClass::Damage_Smoke_Offset() const (0x006DF6C0)
// The damage smoke offset; when it is given relative to the screen
// (DamSmkOffScrnRel) its x/y are converted through the tactical view and
// its height (in tenths) is applied as a diagonal shift and a lift.

// The projected screen point as a coordinate on the ground plane (the same
// inline helper the BuildingClass fire-coordinate units use, 0x0043BBD0).
static inline CoordStruct Projected_Coord_006df6c0(Point2D const &point) { return CoordStruct(point.X, point.Y, 0); }

CoordStruct TechnoTypeClass::Damage_Smoke_Offset() const
{
    if (DamSmkOffScrnRel) {
        Point2D screen = Point2D(DamageSmokeOffset.X, DamageSmokeOffset.Y);
        CoordStruct offset = Projected_Coord_006df6c0(unknown_tactical_83996c->Unknown_006A2900(screen));
        int height = DamageSmokeOffset.Z * 10;
        offset.X -= height;
        offset.Y -= height;
        double side = height;
        int lift = (int)(Fast_Sqrt(side * side + side * side) * 0.58);
        if (DamageSmokeOffset.Z < 0) {
            offset.Z = lift;
        } else {
            offset.Z -= lift;
        }
        return offset;
    }
    return DamageSmokeOffset;
}


// TechnoTypeClass scalar deleting destructor (0x006DF7A0)
// Compiler generated after the unit's last member: ~TechnoTypeClass,
// then operator delete when bit 0 of the flags is set.


// TypeList members this unit emits by its own use of its type lists (bodies: shared/type_list.hpp):
// TypeList<ParticleSystemTypeClass *>::TypeList, default constructor (0x006DF7C0; vtable 0x007AD144).


// void TypeList<ParticleSystemTypeClass *>::Save(IStream *stream) (0x006DF7F0)
// TechnoType.cpp's copy of TypeList<T>::Save (shared/techno_type.hpp),
// called by TechnoTypeClass::Save for the particle-system lists.
// @symbol ?Save@?$TypeList@PAVParticleSystemTypeClass@@@@QAEXPAUIStream@@@Z


// TypeList<ParticleSystemTypeClass *> scalar deleting destructor (0x006DF840)
// Compiler generated (vtable 0x007AD144 slot 0); the base destructor and
// Clear inlined.
// @symbol ??_G?$TypeList@PAVParticleSystemTypeClass@@@@UAEPAXI@Z


// DynamicVectorClass<VoxelAnimTypeClass *>::DynamicVectorClass(NoInitClass const &) (0x006DF890)
// Instantiated from shared/vector_class.hpp by TechnoTypeClass's load
// constructor.
// @symbol ??0?$DynamicVectorClass@PAVVoxelAnimTypeClass@@@@QAE@ABVNoInitClass@@@Z


// DynamicVectorClass<int>::DynamicVectorClass(NoInitClass const &) (0x006DF8A0)
// Instantiated from shared/vector_class.hpp by TechnoTypeClass's load
// constructor.
// @symbol ??0?$DynamicVectorClass@H@@QAE@ABVNoInitClass@@@Z


// DynamicVectorClass<BuildingTypeClass *>::DynamicVectorClass(NoInitClass const &) (0x006DF8B0)
// Instantiated from shared/vector_class.hpp by TechnoTypeClass's load
// constructor.
// @symbol ??0?$DynamicVectorClass@PAVBuildingTypeClass@@@@QAE@ABVNoInitClass@@@Z


// VectorClass<ParticleSystemTypeClass *>::operator=(VectorClass const &) (0x006DF8C0)
// Instantiated from shared/vector_class.hpp for TechnoTypeClass::Read_INI and
// Load, which copy the particle-system lists (DamageParticleSystems,
// DestroyParticleSystems) through it.

// The generic body of shared/vector_class.hpp, written as an explicit
// specialization (VC6 cannot explicitly instantiate an operator).
template <>
VectorClass<ParticleSystemTypeClass *> &VectorClass<ParticleSystemTypeClass *>::operator=(VectorClass<ParticleSystemTypeClass *> const &vector)
{
    if (this != &vector) {
        Clear();
        VectorMax = vector.VectorMax;
        if (VectorMax) {
            Vector = new ParticleSystemTypeClass *[VectorMax];
            if (Vector) {
                IsAllocated = true;
                for (int index = 0; index < VectorMax; index++) {
                    Vector[index] = vector.Vector[index];
                }
            }
        } else {
            Vector = 0;
            IsAllocated = false;
        }
    }
    return *this;
}


// DynamicVectorClass<ParticleSystemTypeClass *>::DynamicVectorClass(unsigned size, ParticleSystemTypeClass * const *array) (0x006DF930)
// Instantiated from shared/vector_class.hpp (the base constructor inlined)
// by the default TypeList constructions in TechnoTypeClass's load constructor.
// @symbol ??0?$DynamicVectorClass@PAVParticleSystemTypeClass@@@@QAE@IPBQAVParticleSystemTypeClass@@@Z


// DynamicVectorClass<ParticleSystemTypeClass *>::Add(ParticleSystemTypeClass * const &object) (0x006DF990)
// Instantiated from shared/vector_class.hpp for TechnoTypeClass::Read_INI
// and Load, which append to the particle-system lists through it.

// The generic body of shared/vector_class.hpp, written as an explicit
// specialization (VC6 emits no code for an explicit member instantiation).
template <>
bool DynamicVectorClass<ParticleSystemTypeClass *>::Add(ParticleSystemTypeClass * const &object)
{
    if (ActiveCount >= Length()) {
        if ((IsAllocated || !VectorMax) && GrowthStep > 0) {
            if (!Resize(Length() + GrowthStep)) {
                return false;
            }
        } else {
            return false;
        }
    }
    int index = ActiveCount++;
    Vector[index] = object;
    return true;
}


// DynamicVectorClass<TechnoTypeClass *>::Delete(int index) (0x006DFA70)
// Instantiated from shared/vector_class.hpp by TechnoTypeClass's destructor,
// whose TechnoTypes.Delete(this) expands Delete(T const &) and calls this.

// The generic body of shared/vector_class.hpp, written as an explicit
// specialization (VC6 emits no code for an explicit member instantiation).
template <>
bool DynamicVectorClass<TechnoTypeClass *>::Delete(int index)
{
    if (index < ActiveCount) {
        ActiveCount--;
        for (int i = index; i < ActiveCount; i++) {
            Vector[i] = Vector[i + 1];
        }
        return true;
    }
    return false;
}
