// unittype.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/unittype.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/unit_type.hpp"
#include "shared/surface.hpp"
#include "shared/convert.hpp"
#include "shared/mix_file_class.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/object.hpp"
#include "shared/id_source.hpp"
#include "shared/facing.hpp"
#define Min(a, b) ((a) < (b) ? (a) : (b))
#include "shared/crt_string.hpp"
#include "shared/map_view.hpp"


// Header-static dynamic initializers of the unit at 0x0070A410: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x0070A410-0x0070A5F1); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x0070A430, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_0070a410.cpp; no source of its own.


// Dynamic initializer (0x0070A460, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_0070a410.cpp; no source of its own.


// Dynamic initializer (0x0070A480, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_0070a410.cpp; no source of its own.


// Dynamic initializer (0x0070A4A0, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_0070a410.cpp; no source of its own.


// Dynamic initializer (0x0070A4C0, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_0070a410.cpp; no source of its own.


// Dynamic initializer (0x0070A4E0, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_0070a410.cpp; no source of its own.


// Dynamic initializer (0x0070A500, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_0070a410.cpp; no source of its own.


// Dynamic initializer (0x0070A540, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_0070a410.cpp; no source of its own.


// Dynamic initializer (0x0070A570, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_0070a410.cpp; no source of its own.


// Dynamic initializer (0x0070A5A0, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_0070a410.cpp; no source of its own.


// Dynamic initializer (0x0070A5D0, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_0070a410.cpp; no source of its own.


// Dynamic initializer (0x0070A5E0, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_0070a410.cpp; no source of its own.


// UnitTypeClass::UnitTypeClass(char const *ini_name) (0x0070A600)

UnitTypeClass::UnitTypeClass(char const *ini_name) :
    TechnoTypeClass(ini_name, -1),
    ArrayIndex(-1),
    MovementRestrictedTo(-1),
    HalfDamageSmokeLocation(0, 0, 0),
    Passive(false),
    CrateGoodie(false),
    Harvester(false),
    Weeder(false),
    unknown_ba8(false),
    unknown_ba9(false),
    DeployToFire(false),
    IsSimpleDeployer(false),
    IsTilter(true),
    UseTurretShadow(false),
    TooBigToFitUnderBridge(false),
    CanBeach(false),
    SmallVisceroid(false),
    LargeVisceroid(false),
    CarriesCrate(false),
    NonVehicle(false),
    StandingFrames(0),
    DeathFrames(0),
    DeathFrameRate(1),
    StartStandFrame(-1),
    StartWalkFrame(-1),
    StartFiringFrame(-1),
    StartDeathFrame(-1),
    MaxDeathCounter(-1),
    Facings(8),
    AltImage(0),
    WalkFrames(12),
    FiringFrames(0)
{
    Create_ID();
    RotCount = 32;
    UnitTypes.Add(this);
    ArrayIndex = UnitTypes.ID(this);
    int index;
    for (index = 0; index < 2; index++) {
        FiringSyncFrame[index] = -1;
    }
    for (index = 0; index < 4; index++) {
        BurstDelay[index] = -1;
    }
    ImmuneToPsionics = false;
    Parasiteable = true;
    Organic = false;
    ConsideredAircraft = false;
}


// UnitTypeClass::UnitTypeClass(NoInitClass const &noinit) (0x0070A7E0)
// Load path.

UnitTypeClass::UnitTypeClass(NoInitClass const &noinit) :
    TechnoTypeClass(noinit),
    AltImageFile(noinit)
{
}


// UnitTypeClass::~UnitTypeClass() (0x0070A810)
// Leaves UnitTypes.

UnitTypeClass::~UnitTypeClass()
{
    Unknown_006ECB90(this, true);
    UnitTypes.Delete(this);
}


// static int __fastcall UnitTypeClass::From_Name(char const *name) (0x0070A890)
// Index of the type with this ID (case-insensitive), or -1.

int __fastcall UnitTypeClass::From_Name(char const *name)
{
    if (name) {
        for (int index = 0; index < UnitTypes.Count(); index++) {
            if (_stricmp(UnitTypes[index]->ID, name) == 0) {
                return index;
            }
        }
    }
    return -1;
}


// bool UnitTypeClass::CanUseWaypoint() const (0x0070A8E0)

bool UnitTypeClass::CanUseWaypoint() const
{
    return TechnoTypeClass::CanUseWaypoint();
}


// bool UnitTypeClass::CanAttackMove() const (0x0070A8F0)

bool UnitTypeClass::CanAttackMove() const
{
    return TechnoTypeClass::CanAttackMove();
}


// void UnitTypeClass::One_Time() (0x0070A900)
// Makes the shared drawing resources once: a 256x256 byte surface cleared to 0, a drawer converting
// unknown_palette_837e10 onto it, and the small and large visceroid shapes.


void UnitTypeClass::One_Time()
{
    if (unknown_surface_ad0604 == 0) {
        unknown_surface_ad0604 = new BSurface(256, 256, 1);
        unknown_surface_ad0604->Fill(0);
    }
    if (unknown_drawer_ad0608 == 0) {
        unknown_drawer_ad0608 = new ConvertClass(unknown_palette_837e10, unknown_palette_837e10, unknown_surface_ad0604, 1, false);
    }
    VisceroidSmallShape = MixFileClass::Retrieve("VISC_SML.SHP");
    VisceroidLargeShape = MixFileClass::Retrieve("VISC_LRG.SHP");
}


// bool UnitTypeClass::Create_And_Place(Cell const &cell, HouseClass *house) const (0x0070A9D0)
// Places a new vehicle of this type for the house at the cell centre, facing a
// random direction (RA1: Create_And_Place).

bool UnitTypeClass::Create_And_Place(Cell const &cell, HouseClass *house) const
{
    ObjectClass *unit = (ObjectClass *)new UnitClass((UnitTypeClass *)this, house);
    if (unit != 0) {
        // A random facing in the direction's high byte (the 32-bit value
        // is rewritten, as the Dir_To_* readers read it).
        UnknownFacingDir dir(0);
        *(unsigned *)&dir = (*(unsigned *)&dir & 0xFFFF00FF) | ((unknown_id_source_a3d290->Random()() & 0xFF) << 8);
        CoordStruct coord(cell.X * 256 + 128, cell.Y * 256 + 128, 0);
        return unit->Unlimbo(&coord, Dir_To_256(dir));
    }
    return false;
}


// ObjectClass *UnitTypeClass::Create_One_Of(HouseClass *house) const (0x0070AA80)
// A new vehicle of this type for the house (RA1: Create_One_Of).

ObjectClass *UnitTypeClass::Create_One_Of(HouseClass *house) const
{
    return (ObjectClass *)new UnitClass((UnitTypeClass *)this, house);
}


// CoordStruct UnitTypeClass::Pixel_Dimensions() const (0x0070AAB0)
// Three quarters of +0x1BC in each dimension, at most 60.


CoordStruct UnitTypeClass::Pixel_Dimensions() const
{
    int width = unknown_1bc - unknown_1bc / 4;
    if (width >= 60) {
        width = 60;
    }
    return CoordStruct(width, width, Min(unknown_1bc - unknown_1bc / 4, 60));
}


// CoordStruct UnitTypeClass::Lepton_Dimensions() const (0x0070AAF0)
// One cell square, twice the 0x00AD0600 height.

CoordStruct UnitTypeClass::Lepton_Dimensions() const
{
    return CoordStruct(256, 256, unknown_ad0600 * 2);
}


// Point2D UnitTypeClass::Turret_Adjust(int dir, Point2D const &xy) const -- 0x0070AB20
// The turret drawing offset for a facing: this class leaves it unchanged.

Point2D UnitTypeClass::Turret_Adjust(int dir, Point2D const &xy) const
{
    return xy;
}


// Unit type rules/art loading, original0x0070AB40.
bool UnitTypeClass::Read_INI(CCINIClass &ini)
{
    if (TechnoTypeClass::Read_INI(ini)) {
        char const *section = ID;
        CrateGoodie = ini.Get_Bool(section, "CrateGoodie", CrateGoodie);
        DeployToFire = ini.Get_Bool(section, "DeployToFire", DeployToFire);
        IsSimpleDeployer = ini.Get_Bool(section, "IsSimpleDeployer", IsSimpleDeployer);
        Harvester = ini.Get_Bool(section, "Harvester", Harvester);
        Weeder = ini.Get_Bool(section, "Weeder", Weeder);
        if (SpeedType == -1) SpeedType = Crusher ? 1 : 2;
        SpeedType = ini.Get_SpeedType(section, "SpeedType", (::SpeedType)SpeedType);
        IsTilter = ini.Get_Bool(section, "IsTilter", IsTilter);
        CarriesCrate = ini.Get_Bool(section, "CarriesCrate", CarriesCrate);
        unknown_ba9 = !Turret;
        TooBigToFitUnderBridge = ini.Get_Bool(section, "TooBigToFitUnderBridge", TooBigToFitUnderBridge);
        (INIPoint3D &)HalfDamageSmokeLocation = ini.Get_Point(section, "HalfDamageSmokeLocation", (INIPoint3D const &)HalfDamageSmokeLocation);
        unknown_368 = 15;
        if (Harvester || Weeder) unknown_368 = 10;
        char const *art_section = unknown_1c8;
        UseTurretShadow = ArtINI.Get_Bool(art_section, "UseTurretShadow", UseTurretShadow);
        WalkFrames = (char)ArtINI.Get_Int(art_section, "WalkFrames", WalkFrames);
        FiringFrames = (char)ArtINI.Get_Int(art_section, "FiringFrames", FiringFrames);
        Passive = ini.Get_Bool(section, "Passive", Passive);
        MovementRestrictedTo = ini.Get_LandType(section, "MovementRestrictedTo", (LandType)MovementRestrictedTo);
        CanBeach = ini.Get_Bool(section, "CanBeach", CanBeach);
        SmallVisceroid = ini.Get_Bool(section, "SmallVisceroid", SmallVisceroid);
        LargeVisceroid = ini.Get_Bool(section, "LargeVisceroid", LargeVisceroid);
        NonVehicle = ini.Get_Bool(section, "NonVehicle", NonVehicle);
        if (FiringFrames > 0) StandingFrames = 1;
        StandingFrames = ArtINI.Get_Int(art_section, "StandingFrames", StandingFrames);
        DeathFrames = ArtINI.Get_Int(art_section, "DeathFrames", DeathFrames);
        DeathFrameRate = ArtINI.Get_Int(art_section, "DeathFrameRate", DeathFrameRate);
        if (DeathFrameRate < 1) DeathFrameRate = 1;
        if (!FiringFrames && !Turret) Facings = 1;
        Facings = ArtINI.Get_Int(art_section, "Facings", Facings);
        if (StartWalkFrame == -1) StartWalkFrame = 0;
        if (StartStandFrame == -1) StartStandFrame = StandingFrames == 0 ? StartWalkFrame : WalkFrames * Facings;
        if (StartFiringFrame == -1) StartFiringFrame = FiringFrames == 0 ? StartStandFrame : (WalkFrames + StandingFrames) * Facings;
        if (StartDeathFrame == -1) {
            StartDeathFrame = DeathFrames == 0 ? -1 : (FiringFrames + 1 + WalkFrames) * Facings;
            MaxDeathCounter = StartDeathFrame + DeathFrames;
        }
        StartStandFrame = ArtINI.Get_Int(art_section, "StartStandFrame", StartStandFrame);
        StartWalkFrame = ArtINI.Get_Int(art_section, "StartWalkFrame", StartWalkFrame);
        StartFiringFrame = ArtINI.Get_Int(art_section, "StartFiringFrame", StartFiringFrame);
        StartDeathFrame = ArtINI.Get_Int(art_section, "StartDeathFrame", StartDeathFrame);
        MaxDeathCounter = ArtINI.Get_Int(art_section, "MaxDeathCounter", MaxDeathCounter);
        for (int sync = 0; sync < 2; ++sync) {
            char key[20];
            sprintf(key, "FiringSyncFrame%d", sync);
            FiringSyncFrame[sync] = ArtINI.Get_Int(art_section, key, FiringSyncFrame[sync]);
        }
        for (int burst = 0; burst < 4; ++burst) {
            char key[20];
            sprintf(key, "BurstDelay%d", burst);
            BurstDelay[burst] = ini.Get_Int(section, key, BurstDelay[burst]);
        }
        char default_image[25];
        strcpy(default_image, AltImageFile);
        ini.Get_String(section, "AltImage", default_image, AltImageFile.Buffer, sizeof(AltImageFile));
        char filename[260];
        _makepath(filename, 0, 0, AltImageFile, ".SHP");
        AltImage = MixFileClass::Retrieve(filename);
        if (*this == "FV") {
            {
                int turret = ini.Get_Int(section, "NormalTurretIndex", 0);
                int weapon = ini.Get_Int(section, "NormalTurretWeapon", -1);
                Set_Turret_Weapon(turret, weapon);
            }
            {
                int turret = ini.Get_Int(section, "RepairTurretIndex", 1);
                int weapon = ini.Get_Int(section, "RepairTurretWeapon", -1);
                Set_Turret_Weapon(turret, weapon);
            }
            {
                int turret = ini.Get_Int(section, "MachineGunTurretIndex", 2);
                int weapon = ini.Get_Int(section, "MachineGunTurretWeapon", -1);
                Set_Turret_Weapon(turret, weapon);
            }
            {
                int turret = ini.Get_Int(section, "FlakTurretIndex", 3);
                int weapon = ini.Get_Int(section, "FlakTurretWeapon", -1);
                Set_Turret_Weapon(turret, weapon);
            }
            {
                int turret = ini.Get_Int(section, "PistolTurretIndex", 0);
                int weapon = ini.Get_Int(section, "PistolTurretWeapon", -1);
                Set_Turret_Weapon(turret, weapon);
            }
            {
                int turret = ini.Get_Int(section, "SniperTurretIndex", 0);
                int weapon = ini.Get_Int(section, "SniperTurretWeapon", -1);
                Set_Turret_Weapon(turret, weapon);
            }
            {
                int turret = ini.Get_Int(section, "ShockTurretIndex", 0);
                int weapon = ini.Get_Int(section, "ShockTurretWeapon", -1);
                Set_Turret_Weapon(turret, weapon);
            }
            {
                int turret = ini.Get_Int(section, "ExplodeTurretIndex", 0);
                int weapon = ini.Get_Int(section, "ExplodeTurretWeapon", -1);
                Set_Turret_Weapon(turret, weapon);
            }
            {
                int turret = ini.Get_Int(section, "BrainBlastTurretIndex", 0);
                int weapon = ini.Get_Int(section, "BrainBlastTurretWeapon", -1);
                Set_Turret_Weapon(turret, weapon);
            }
            {
                int turret = ini.Get_Int(section, "RadCannonTurretIndex", 0);
                int weapon = ini.Get_Int(section, "RadCannonTurretWeapon", -1);
                Set_Turret_Weapon(turret, weapon);
            }
            {
                int turret = ini.Get_Int(section, "ChronoTurretIndex", 0);
                int weapon = ini.Get_Int(section, "ChronoTurretWeapon", -1);
                Set_Turret_Weapon(turret, weapon);
            }
            {
                int turret = ini.Get_Int(section, "TerroristExplodeTurretIndex", 0);
                int weapon = ini.Get_Int(section, "TerroristExplodeTurretWeapon", -1);
                Set_Turret_Weapon(turret, weapon);
            }
            {
                int turret = ini.Get_Int(section, "CowTurretIndex", 0);
                int weapon = ini.Get_Int(section, "CowTurretWeapon", -1);
                Set_Turret_Weapon(turret, weapon);
            }
        }
        return true;
    }
    return false;
}


// CoordStruct UnitTypeClass::Coord_Fixup(CoordStruct const &coord) const (0x0070B320)
// The coordinate, raised to the ground height when below it.

CoordStruct UnitTypeClass::Coord_Fixup(CoordStruct const &coord) const
{
    CoordStruct result = coord;
    if (result.Z < Map.Get_Height_GL(result)) {
        result.Z = Map.Get_Height_GL(result);
    }
    return result;
}


// int UnitTypeClass::Repair_Step() const (0x0070B390)
// The rules repair step.

int UnitTypeClass::Repair_Step() const
{
    return Rules->unknown_1334;
}


// HRESULT __stdcall UnitTypeClass::GetClassID(GUID *clsid) (0x0070B3A0)

HRESULT __stdcall UnitTypeClass::GetClassID(GUID *clsid)
{
    if (clsid == 0) {
        return E_POINTER;
    }
    *clsid = unknown_clsid_unit_type_7a1b00;
    return S_OK;
}


// void UnitTypeClass::Compute_CRC(WWCRCEngine &crc) const (0x0070B3E0)

void UnitTypeClass::Compute_CRC(WWCRCEngine &crc) const
{
    TechnoTypeClass::Compute_CRC(crc);
    crc((long)MovementRestrictedTo);
    crc(Passive);
    crc(CrateGoodie);
    crc(Harvester);
    crc(Weeder);
    crc(unknown_ba8);
    crc(unknown_ba9);
    crc(IsTilter);
    crc(IsSimpleDeployer);
    crc(UseTurretShadow);
}


// HRESULT __stdcall UnitTypeClass::Load(IStream *stream) (0x0070B480)
// Rebuilds the object in place, reloads the image and the alternate image.

HRESULT __stdcall UnitTypeClass::Load(IStream *stream)
{
    HRESULT result = TechnoTypeClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    new (this) UnitTypeClass(NoInitClass());
    Fetch_Voxel_Image();
    Fetch_Normal_Image();
    if (AltImage) {
        char filename[260];
        _makepath(filename, 0, 0, AltImageFile, ".SHP");
        AltImage = MixFileClass::Retrieve(filename);
    }
    return S_OK;
}


// HRESULT __stdcall UnitTypeClass::Save(IStream *stream, BOOL clear_dirty) (0x0070B520)

HRESULT __stdcall UnitTypeClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = TechnoTypeClass::Save(stream, clear_dirty);
    if (SUCCEEDED(result)) {
        result = S_OK;
    }
    return result;
}


// static UnitTypeClass *__fastcall UnitTypeClass::Find_Or_Make(char const *name) (0x0070B540)
// The type with this ID, a new one when unknown, 0 for "<none>"/"none".

UnitTypeClass *__fastcall UnitTypeClass::Find_Or_Make(char const *name)
{
    if (_stricmp("<none>", name) == 0 || _stricmp("none", name) == 0) {
        return 0;
    }
    for (int index = 0; index < UnitTypes.Count(); index++) {
        if (_stricmp(UnitTypes[index]->ID, name) == 0) {
            return UnitTypes[index];
        }
    }
    return new UnitTypeClass(name);
}


// int UnitTypeClass::Size_Of() const (0x0070B5D0)

int UnitTypeClass::Size_Of() const
{
    return sizeof(UnitTypeClass);
}


// int UnitTypeClass::Kind_Of() const (0x0070B5E0)

int UnitTypeClass::Kind_Of() const
{
    return 0x28;
}


// int UnitTypeClass::Fetch_Heap_ID() const (0x0070B5F0)
// The type's index in UnitTypes.

int UnitTypeClass::Fetch_Heap_ID() const
{
    return ArrayIndex;
}


// UnitTypeClass scalar deleting destructor (0x0070B600)
// Compiler generated after the unit's last member (destructor inlined).
