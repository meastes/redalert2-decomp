// builtype.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/builtype.json carries the addresses and the rows that
// stay in their own files.


#define RA2_SHAPE_RECORD_RECT
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/building_type.hpp"
#include "shared/building_view.hpp"
#include "shared/mix_file_class.hpp"
#include "shared/cc_file_class.hpp"
#define SHAPE_FILE_EMPTY_RECT unknown_static_rect
#include "shared/shape_file.hpp"
#include "shared/surface.hpp"
#include "shared/anim_type.hpp"
#include "shared/unit.hpp"
#include "shared/unit_type.hpp"
#include "shared/rules.hpp"
#include "shared/aircraft_type.hpp"
#include "shared/map_view.hpp"
#include "shared/cell.hpp"
#include "shared/foot.hpp"
#include "shared/house.hpp"
#include "shared/ccini_class.hpp"
#include "shared/crt_string.hpp"
#include "shared/overlay_type.hpp"
#include "shared/id_source.hpp"
#include "shared/win32_api.hpp"
#define READ_ANIM_NAMES(INDEX, NAME) \
    ArtINI.Get_String(unknown_1c8, NAME, "", key, 16); \
    if (strlen(key) != 0) { \
        strcpy(Anims[INDEX].Anim.Buffer, key); \
    } \
    ArtINI.Get_String(unknown_1c8, NAME "Damaged", "", key, 16); \
    if (strlen(key) != 0) { \
        strcpy(Anims[INDEX].Damaged.Buffer, key); \
    } \
    if (strlen(Anims[INDEX].Damaged.Buffer) == 0) { \
        strcpy(Anims[INDEX].Damaged.Buffer, Anims[INDEX].Anim.Buffer); \
    } \
    ArtINI.Get_String(unknown_1c8, NAME "Garrisoned", "", key, 16); \
    if (strlen(key) != 0) { \
        strcpy(Anims[INDEX].Garrisoned.Buffer, key); \
    } \
    if (strlen(Anims[INDEX].Garrisoned.Buffer) == 0) { \
        strcpy(Anims[INDEX].Garrisoned.Buffer, Anims[INDEX].Anim.Buffer); \
    }
#define ANIM_NAMED(INDEX) \
    (strlen(Anims[INDEX].Anim.Buffer) != 0 || strlen(Anims[INDEX].Damaged.Buffer) != 0 \
     || strlen(Anims[INDEX].Garrisoned.Buffer) != 0)
#define READ_ANIM_PLACEMENT(INDEX, OFFSET, NAME) \
    Anims[INDEX].Offset.X = ArtINI.Get_Int(unknown_1c8, NAME "X", Anims[OFFSET].Offset.X); \
    Anims[INDEX].Offset.Y = ArtINI.Get_Int(unknown_1c8, NAME "Y", Anims[OFFSET].Offset.Y); \
    Anims[INDEX].ZAdjust = ArtINI.Get_Int(unknown_1c8, NAME "ZAdjust", Anims[INDEX].ZAdjust); \
    Anims[INDEX].YSort = ArtINI.Get_Int(unknown_1c8, NAME "YSort", Anims[INDEX].YSort);
#define READ_ANIM(INDEX, OFFSET, NAME) \
    READ_ANIM_NAMES(INDEX, NAME) \
    if (ANIM_NAMED(INDEX)) { \
        READ_ANIM_PLACEMENT(INDEX, OFFSET, NAME) \
        Anims[INDEX].Powered = ArtINI.Get_Bool(unknown_1c8, NAME "Powered", Anims[INDEX].Powered); \
        Anims[INDEX].PoweredLight = ArtINI.Get_Bool(unknown_1c8, NAME "PoweredLight", Anims[INDEX].PoweredLight); \
    }
#define READ_ANIM_PLAIN(INDEX, OFFSET, NAME) \
    READ_ANIM_NAMES(INDEX, NAME) \
    if (ANIM_NAMED(INDEX)) { \
        READ_ANIM_PLACEMENT(INDEX, OFFSET, NAME) \
    }
#include "shared/type_list.hpp"
#include "shared/weapon_type.hpp"
#include "shared/bullet_type.hpp"
#include "shared/warhead_type.hpp"
#include "shared/building_type_class.hpp"
#include "shared/building_foundation_tables.hpp"


// Header-static dynamic initializers of the unit at 0x00454CF0:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x00454CF0-0x00454ED6); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x00454D20, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_00454cf0.cpp; no source of its own.


// Dynamic initializer (0x00454D40, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_00454cf0.cpp; no source of its own.


// Dynamic initializer (0x00454D60, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_00454cf0.cpp; no source of its own.


// Dynamic initializer (0x00454D80, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_00454cf0.cpp; no source of its own.


// Dynamic initializer (0x00454DA0, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_00454cf0.cpp; no source of its own.


// Dynamic initializer (0x00454DC0, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_00454cf0.cpp; no source of its own.


// Dynamic initializer (0x00454E00, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_00454cf0.cpp; no source of its own.


// Dynamic initializer (0x00454E30, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_00454cf0.cpp; no source of its own.


// Dynamic initializer (0x00454E60, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_00454cf0.cpp; no source of its own.


// Dynamic initializer (0x00454E90, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_00454cf0.cpp; no source of its own.


// Dynamic initializer (0x00454EA0, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_00454cf0.cpp; no source of its own.


// Dynamic initializer (0x00454EC0, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_00454cf0.cpp; no source of its own.


// Naturally emitted by the unit foundation tables; no second definition.


// Original array construction iterator called naturally by the omitted Cell tails.
// Natural foundation startup/body emitted by building_foundation_tables.hpp,
// included after the BuildingType methods so existing _$E47/guard46 remain unchanged.


// Naturally emitted by the unit foundation tables; no second definition.


// Naturally emitted by the unit foundation tables; no second definition.


// char const *__fastcall Name_From_BuildCat(BuildCat category) (0x00457A50)
// The INI name of a building AI category from the {name, value} table at
// 0x007CFC40 (DontCare, Tech, Power, Resource, Infrastructure, Combat);
// DontCare for an unknown value.

char const *__fastcall Name_From_BuildCat(BuildCat category)
{
    for (int index = 0; index < 6; index++) {
        if (unknown_buildcat_table_7cfc40[index].Value == category) {
            return unknown_buildcat_table_7cfc40[index].Name;
        }
    }
    return unknown_buildcat_table_7cfc40[0].Name;
}


// BuildCat __fastcall BuildCat_From_Name(char const *name) (0x00457A80)
// The building AI category whose INI name matches (case-insensitive);
// 0 (DontCare) for an unknown name.

BuildCat __fastcall BuildCat_From_Name(char const *name)
{
    for (int index = 0; index < 6; index++) {
        if (_stricmp(unknown_buildcat_table_7cfc40[index].Name, name) == 0) {
            return (BuildCat)unknown_buildcat_table_7cfc40[index].Value;
        }
    }
    return (BuildCat)0;
}


// Construct a building type with the original defaults and registry order.
BuildingTypeClass::BuildingTypeClass(char const *ini_name) :
    TechnoTypeClass(ini_name,0),
    ArrayIndex(-1),
    FoundationData(0),
    BuildupData(0),
    BuildupDataAllocated(false),
    BuildCat(0),
    HalfDamageSmokeLocation1(unknown_geometry_l),
    HalfDamageSmokeLocation2(unknown_geometry_l),
    GateCloseDelay(0.0),
    LightVisibility(5000),
    LightIntensity(0),
    LightRedTint(1000000),
    LightGreenTint(1000000),
    LightBlueTint(1000000),
    PrimaryFirePixelOffset(Point2D(65535,65535)),
    SecondaryFirePixelOffset(Point2D(65535,65535)),
    ToOverlay(0),
    unknown_bf0(0),
    BuildupSound(-1),
    PackupSound(-1),
    CreateUnitSound(-1),
    UnitEnterSound(-1),
    UnitExitSound(-1),
    WorkingSound(-1),
    NotWorkingSound(-1),
    FreeUnit(0),
    unknown_c3c(-1),
    Adjacent(3),
    Factory(0),
    TargetCoordOffset(0,0,0),
    ExitCoord(0,0,0),
    FoundationOutside(0),
    unknown_c64(0),
    Power(0),
    PowerDrain_c6c(0),
    ExtraPower(0),
    ExtraPowerDrain_c74(0),
    Foundation(0),
    Height(2),
    OccupyHeight(2),
    MidPoint(0),
    DoorStages(0),
    Upgrades(0),
    unknown_11a0(0),
    unknown_11a8(0),
    unknown_11b0(0),
    unknown_11b8(0),
    DoorAnimData_11c0(0),
    SpecialZOverlayData_11c4(0),
    SpecialZOverlayZAdjust(0),
    unknown_11cc(0),
    unknown_11d0(false),
    NormalZAdjust(0),
    unknown_11d8(0),
    unknown_11dc(0),
    unknown_11e0(0),
    ZShapePointMove(Point2D(0,0)),
    unknown_11ec(unknown_static_rect),
    ExtraLight(0),
    TogglePower(true),
    HasSpotlight(false),
    IsTemple(false),
    IsPlug(false),
    HoverPad(false),
    BaseNormal(true),
    EligibileForAllyBuilding(false),
    EligibleForDelayKill(false),
    NeedsEngineer(false),
    ProduceCashStartup(0),
    ProduceCashAmount(0),
    ProduceCashDelay(0),
    Bib(false),
    Wall(false),
    Capturable(false),
    Powered(false),
    Overpowerable(false),
    Spyable(false),
    CanC4(true),
    WantsExtraSpace(false),
    Unsellable(false),
    ClickRepairable(true),
    CanBeOccupied(false),
    MaxNumberOccupants(0),
    ShowOccupantPips(true),
    QueueingCell(Point2D(0,0)),
    NumberImpassableRows(-1),
    Radar(false),
    SpySat(false),
    ChargeAnim(false),
    IsAnimDelayedFire(false),
    SiloDamage(false),
    UnitRepair(false),
    UnitReload(false),
    Cloning(false),
    Flat(false),
    DockUnload(false),
    Recoilless(false),
    HasStupidGuardMode(true),
    BridgeRepairHut(false),
    Gate(false),
    SAM(false),
    ConstructionYard(false),
    NukeSilo(false),
    Refinery(false),
    Weeder(false),
    WeaponsFactory(false),
    LaserFencePost(false),
    LaserFence(false),
    FirestormWall(false),
    Hospital(false),
    Armory(false),
    EMPulseCannon(false),
    TickTank(false),
    TurretAnimIsVoxel(false),
    BarrelAnimIsVoxel(false),
    CloakGenerator(false),
    SensorArray(false),
    ICBMLauncher(false),
    Artillary(false),
    Helipad(false),
    OrePurifier(false),
    GDIBarracks(false),
    NODBarracks(false),
    ChargedAnimTime(999.0f),
    DelayedFireDelay(0),
    SuperWeapon(-1),
    SuperWeapon2(-1),
    GateStages(9),
    PowersUpToLevel(-1),
    DamagedDoor(false),
    InvisibleInGame(false),
    TerrainPalette(false),
    PlaceAnywhere(false),
    ExtraDamageStage(true),
    AIBuildThis(false),
    IsBaseDefense(false),
    CloakRadiusInCells(20),
    ConcentricRadialIndicator(false),
    PsychicDetectionRadius(0),
    BarrelStartPitch(64),
    DemandLoad(false),
    DemandLoadBuildup(false),
    FreeBuildup(false),
    IsThreatRatingNode(false),
    PrimaryFireDualOffset(false),
    ProtectWithWall(false),
    CanHideThings(true),
    CanBeHidden(true),
    CrateBeneath(false),
    LeaveRubble(false),
    CrateBeneathIsMoney(false),
    NumberOfDocks(1),
    DockingOffsets(1)
{
    AbstractClass::Create_ID();
    BuildingTypes.Add(this);
    ArrayIndex=BuildingTypes.ID(this);
    unknown_func_4584d0(0,0,1,0);
    unknown_func_4584d0(1,0,1,0);
    unknown_func_4584d0(2,0,1,0);
    unknown_func_4584d0(4,0,1,0);
    unknown_func_4584d0(5,0,1,0);
    for(int index=0;index<18;++index) {
        Anims[index].Anim.Clear();
        Anims[index].Damaged.Clear();
        Anims[index].Garrisoned.Clear();
        Anims[index].Offset=Point2D(0,0);
        Anims[index].ZAdjust=0;
        Anims[index].YSort=0;
        Anims[index].Powered=true;
        Anims[index].PoweredLight=false;
    }
    BuildupFile[0]=0;
    PowersUpBuilding[0]=0;
    unknown_13eb[0]=0;
    VoxelBarrelFile[0]=0;
    Trainable=false;
    unknown_11a4=false;
    unknown_11ac=false;
    unknown_11b4=false;
    unknown_11bc=false;
    for(int first=0;first<10;++first) MuzzleFlash[first]=Point2D(unknown_geometry_l.X,unknown_geometry_l.Y);
    for(int second=0;second<8;++second) DamageFireOffset[second]=Point2D(unknown_geometry_l.X,unknown_geometry_l.Y);
    DockingOffsets[0]=TPoint3D<int>(0,0,0);
    ImmuneToPsionics=true;
    Parasiteable=false;
    Organic=false;
    ConsideredAircraft=false;
}


// BuildingTypeClass::BuildingTypeClass(NoInitClass const &noinit) (0x00458160)
// Load path: no buildup image yet, an empty docking-offset list.

BuildingTypeClass::BuildingTypeClass(NoInitClass const &noinit) :
    TechnoTypeClass(noinit),
    BuildupData(0)
{
}


// BuildingTypeClass::~BuildingTypeClass() (0x004581C0)
// Frees the owned images and leaves BuildingTypes.

BuildingTypeClass::~BuildingTypeClass()
{
    if (BuildupDataAllocated && BuildupData) {
        operator delete(BuildupData);
        BuildupData = 0;
        BuildupDataAllocated = false;
    }
    if (unknown_11d0 && unknown_11cc) {
        operator delete(unknown_11cc);
        unknown_11cc = 0;
        unknown_11d0 = false;
    }
    if (unknown_11a4 && unknown_11a0) {
        operator delete(unknown_11a0);
        unknown_11a0 = 0;
        unknown_11a4 = false;
    }
    if (unknown_11ac && unknown_11a8) {
        operator delete(unknown_11a8);
        unknown_11a8 = 0;
        unknown_11ac = false;
    }
    if (unknown_11b4 && unknown_11b0) {
        operator delete(unknown_11b0);
        unknown_11b0 = 0;
        unknown_11b4 = false;
    }
    if (unknown_11bc && unknown_11b8) {
        operator delete(unknown_11b8);
        unknown_11b8 = 0;
        unknown_11bc = false;
    }
    Unknown_006ECB90(this, true);
    BuildingTypes.Delete(this);
}


// void Unknown_00458370() (0x00458370)
// Empty in this build (Init_Bulk_Data calls it in the One_Time sequence; TibSun's
// BuildingTypeClass One_Time position).

void Unknown_00458370()
{
}


// static int __fastcall BuildingTypeClass::From_Full_Name(char const *name) (0x00458380)
// Index of the type whose Name= matches (case-insensitive), or -1.

int __fastcall BuildingTypeClass::From_Full_Name(char const *name)
{
    if (name) {
        for (int index = 0; index < BuildingTypes.Count(); index++) {
            if (_stricmp(BuildingTypes[index]->Name, name) == 0) {
                return index;
            }
        }
    }
    return -1;
}


// static int __fastcall BuildingTypeClass::From_Name(char const *name) (0x004583D0)
// Index of the type with this ID (case-insensitive), or -1.

int __fastcall BuildingTypeClass::From_Name(char const *name)
{
    if (name) {
        for (int index = 0; index < BuildingTypes.Count(); index++) {
            if (_stricmp(BuildingTypes[index]->ID, name) == 0) {
                return index;
            }
        }
    }
    return -1;
}


// bool BuildingTypeClass::Create_And_Place(Cell const &cell, HouseClass *house) const (0x00458420)
// Places a new building of this type for the house at the cell's centre.

bool BuildingTypeClass::Create_And_Place(Cell const &cell, HouseClass *house) const
{
    BuildingClass *building = new BuildingClass(const_cast<BuildingTypeClass *>(this), house);
    if (building != 0) {
        CoordStruct coord(cell.X * 256 + 128, cell.Y * 256 + 128, 0);
        return building->Unlimbo(&coord, 0);
    }
    return false;
}


// ObjectClass *BuildingTypeClass::Create_One_Of(HouseClass *house) (0x004584A0)
// A new structure of this type for the house (RA1: Create_One_Of).

ObjectClass *BuildingTypeClass::Create_One_Of(HouseClass *house) const
{
    return (ObjectClass *)new BuildingClass((BuildingTypeClass *)this, house);
}


// void BuildingTypeClass::unknown_func_4584d0(int index, int value0, int value1, int value2) (0x004584D0)
// Sets one of the six value triples.

void BuildingTypeClass::unknown_func_4584d0(int index, int value0, int value1, int value2)
{
    AnimFrames[index].unknown_00 = value0;
    AnimFrames[index].unknown_04 = value1;
    AnimFrames[index].unknown_08 = value2;
}


// Load the shared building depth shape and remap its nonzero pixels.
extern ShapeFileStruct *unknown_building_depth_shape_85029c;
void BuildingTypeClass::unknown_func_458510()
{
    if(unknown_building_depth_shape_85029c!=0) {
        operator delete(unknown_building_depth_shape_85029c);
        unknown_building_depth_shape_85029c=0;
    }
    int size=CCFileClass("BUILDNGZ.SHP").Size();
    unknown_building_depth_shape_85029c=(ShapeFileStruct *)operator new(size);
    memcpy(unknown_building_depth_shape_85029c,MixFileClass::Retrieve("BUILDNGZ.SHP"),size);
    unsigned char *pixels=(unsigned char *)unknown_building_depth_shape_85029c->Get_Frame_Data(0);
    int width=unknown_building_depth_shape_85029c->Get_Width();
    int height=unknown_building_depth_shape_85029c->Get_Height();
    // The file size is dead after the copy; its scratch local is reused for columns.
    for(int row=height;row>0;--row) {
        for(size=0;size<width;++size) {
            if(pixels[size]!=0) pixels[size]-=65;
        }
        pixels+=width;
    }
}


// static void __fastcall BuildingTypeClass::Init(int theater) (0x00458600)
// Loads the building images for a theater: the shared shapes
// (0x00458510, POWEROFF.SHP, WRENCH.SHP), then per type with a theater
// image (unknown_1fc) the image and its buildup unless demand loaded
// (otherwise any loaded copy is freed); a buildup sets its animation
// triple (frame 0, the buildup's frame count, 75 / frames per frame or 1).
// "NewTheater" types (unknown_207) free demand-loaded images and load
// through 0x00458EA0. Finally the cloak surface is rebuilt for the largest
// CloakRadiusInCells.

extern "C" void __cdecl _makepath(char *path, char const *drive, char const *dir, char const *fname, char const *ext);

// A shape file header as read here (its frame count at +0x06).
struct UnknownBuildupShape {
    short unknown_0;
    short unknown_2;
    short unknown_4;
    short Frames;                       // +0x06
};

void __fastcall BuildingTypeClass::Init(int theater)
{
    unknown_func_458510();
    unknown_poweroff_shape_8502a4 = MixFileClass::Retrieve("POWEROFF.SHP");
    unknown_wrench_shape_8502a8 = MixFileClass::Retrieve("WRENCH.SHP");
    int radius = 0;
    for (int index = 0; index < BuildingTypes.Count(); index++) {
        BuildingTypeClass *type = BuildingTypes[index];
        if (radius <= type->CloakRadiusInCells) {
            radius = type->CloakRadiusInCells;
        }
        if (type->unknown_1fc) {
            char fullname[512];
            if (!type->DemandLoad) {
                _makepath(fullname, 0, 0, type->unknown_1c8, Theaters[theater].Suffix);
                type->unknown_a4 = (void *)MixFileClass::Retrieve(fullname);
            } else if (type->unknown_a4 != 0) {
                operator delete(type->unknown_a4);
                type->unknown_a4 = 0;
                type->unknown_a8 = false;
            }
            if (!type->DemandLoadBuildup) {
                _makepath(fullname, 0, 0, type->BuildupFile, Theaters[theater].Suffix);
                type->BuildupData = (void *)MixFileClass::Retrieve(fullname);
            } else if (type->BuildupData != 0) {
                operator delete(type->BuildupData);
                type->BuildupData = 0;
                type->BuildupDataAllocated = false;
            }
            if (type->BuildupData != 0) {
                int rate = 1;
                int frames = ((UnknownBuildupShape *)type->BuildupData)->Frames;
                if (frames != 0) {
                    rate = 75 / frames;
                }
                type->AnimFrames[0].unknown_00 = 0;
                type->AnimFrames[0].unknown_04 = frames;
                type->AnimFrames[0].unknown_08 = rate;
            }
        } else if (type->unknown_207) {
            if (type->DemandLoad && type->unknown_a4 != 0) {
                operator delete(type->unknown_a4);
                type->unknown_a4 = 0;
                type->unknown_a8 = false;
            }
            if (type->DemandLoadBuildup && type->BuildupData != 0) {
                operator delete(type->BuildupData);
                type->BuildupData = 0;
                type->BuildupDataAllocated = false;
            }
            type->unknown_func_458ea0(theater);
        }
    }
    if (radius != 0) {
        delete unknown_cloak_surface_8502a0;
        unknown_cloak_surface_8502a0 = new BSurface(radius + 16, radius + 16, 1);
    }
}


// CoordStruct BuildingTypeClass::Pixel_Dimensions() const (0x00458840, slot 30)
// The size of the type's image (slot 39) from its first frame: (width,
// width, height); zero without an image.

CoordStruct BuildingTypeClass::Pixel_Dimensions() const
{
    ShapeFileStruct const *shape = (ShapeFileStruct const *)Get_Image_Data();
    int width;
    int height;
    if (shape) {
        Rect rect = shape->Get_Frame_Rect(0);
        width = rect.Width;
        height = rect.Height;
    } else {
        width = 0;
        height = 0;
    }
    return CoordStruct(width, width, height);
}


// Cell const *BuildingTypeClass::Occupy_List(int arg1) const (0x004588C0)
// The foundation's cell list, or an empty list (the end marker alone).

Cell const *BuildingTypeClass::Occupy_List(int arg1) const
{
    static UnknownStaticCell const unknown_none(Cell(0x7FFF, 0x7FFF));
    if (FoundationData) {
        return FoundationData;
    }
    return &unknown_none.Cell;
}


// Exit thunk of the function-local static in BuildingTypeClass::Occupy_List (0x00458920)
// Compiler generated ($E) after unknown_slot36.
// @symbol _$E47


// int BuildingTypeClass::Width() const (0x00458930)
// Foundation width in cells.

int BuildingTypeClass::Width() const
{
    return unknown_foundation_width_7cfc70[Foundation];
}


// int BuildingTypeClass::Foundation_Height(bool bib) const (0x00458940)
// Foundation height in cells, one more with a bib when asked.

int BuildingTypeClass::Foundation_Height(bool bib) const
{
    return unknown_foundation_height_7cfcc8[Foundation] + ((bib && Bib) ? 1 : 0);
}


// int BuildingTypeClass::Max_Pips() const (0x00458980)
// Pip count (RA1: Max_Pips): storage pips scale with the width.

int BuildingTypeClass::Max_Pips() const
{
    int max = Width() * 60 / 8;
    switch (PipScale) {
    case 2:
        if (Weeder) {
            int storage = Rules->unknown_1410;
            if (storage < 0) {
                return 0;
            }
            return storage > max ? max : storage;
        } else {
            int storage = Storage;
            if (storage < 0) {
                return 0;
            }
            return storage > max ? max : storage;
        }
    case 4:
        return max;
    }
    return TechnoTypeClass::Max_Pips();
}


// BuildingType cost adjustments reconstructed from the exact target.
// Genuine mathematical clamp; no original-address binding.
template<class T> inline T Maximum_Cost(T first,T second) { return first>second ? first : second; }

int BuildingTypeClass::Raw_Cost() const
{
    int value=TechnoTypeClass::Raw_Cost();
    TechnoTypeClass *first_type=Rules->PadAircraft[0];
    if(this==first_type->Dock[0] && !Rules->unknown_142c) {
        int second_cost=Rules->PadAircraft[1]->Raw_Cost();
        int first_cost=first_type->Raw_Cost();
        int average=(int)((unsigned int)first_cost+(unsigned int)second_cost)/2;
        value=(int)((unsigned int)value-(unsigned int)average);
    }
    if(FreeUnit) {
        value=(int)((unsigned int)value-(unsigned int)FreeUnit->Raw_Cost());
        return Maximum_Cost(value,0);
    }
    return value;
}


// BuildingType cost adjustments reconstructed from the exact target.
// Genuine mathematical clamp; no original-address binding.

// Maximum_Cost is defined with Raw_Cost earlier in this whole class unit.
template<class T> inline T Maximum_Cost(T first,T second);

int BuildingTypeClass::Cost_Of(HouseClass *house) const
{
    int value=TechnoTypeClass::Cost_Of(house);
    TechnoTypeClass *first_type=Rules->PadAircraft[0];
    if(this==first_type->Dock[0] && !Rules->unknown_142c) {
        int average=(int)((unsigned int)Rules->PadAircraft[0]->Cost_Of(house)+(unsigned int)Rules->PadAircraft[1]->Cost_Of(house))/2;
        value=(int)((unsigned int)value+(unsigned int)average);
    }
    if(FreeUnit) {
        value=(int)((unsigned int)value+(unsigned int)FreeUnit->Cost_Of(house));
        return Maximum_Cost(value,0);
    }
    return value;
}


// Test a prospective foundation and scatter allied mobile occupants.
// Private complete461-byte nonvirtual BuildingType member; source name/CV hypothesis.
inline FootClass *Mobile_Occupant_00458B10(TechnoClass *techno)
{
 bool mobile;
 if(techno)mobile=techno->AbstractFlags.IsFoot;
 if(!techno || !mobile)return 0;
 return static_cast<FootClass *>(techno);
}
inline bool Scatter_Allied_Occupant_00458B10(TechnoClass *techno,Cell const &cell,bool &scattered)
{
 FootClass *foot=Mobile_Occupant_00458B10(techno);
 if(!foot)return false;
 scattered=true;
 if(foot->unknown_494) {
  CoordStruct position=foot->Coord;
  if(&Map[position]!=foot->unknown_494)return true;
 }
 Map[cell].Incoming(&unknown_geometry_l,true,true,false);
 return true;
}
int BuildingTypeClass::unknown_458b10(Cell const &origin,HouseClass *house) const
{
 bool scattered=false;
 Cell const *offset;
 if(origin==unknown_geometry_k)goto no_placement;
 offset=Occupy_List(1);
 while(*offset!=Cell(0x7fff,0x7fff)) {
  Cell cell=origin+*offset++;
  if(!Map.In_Radar(cell))continue;
  CellClass &map_cell=Map[cell];
  if(map_cell.OverlayIndex!=-1) {
   if(this!=Rules->unknown_78c || map_cell.OverlayIndex!=2)return 2;
  }
  ObjectClass *object=map_cell.OccupierPtr;
  if(!object)continue;
  if(object->Kind_Of()==0x24)return 2;
  if(!object->AbstractFlags.IsTechno)continue;
  if(object->Kind_Of()==6) {
   if(!static_cast<BuildingClass *>(object)->unknown_44d900(const_cast<BuildingTypeClass *>(this),house))return 2;
   continue;
  }
  TechnoClass *techno=static_cast<TechnoClass *>(object);
  if(!techno->House->Is_Ally(house))return 2;
  if(!Scatter_Allied_Occupant_00458B10(techno,cell,scattered))return 2;
 }
 if(scattered)return 1;
no_placement:
 return 0;
}


// void *BuildingTypeClass::Get_Image_Data() const (0x00458CE0, slot 39)
// The building's image, demand-loaded from its own image name (unknown_13eb)
// and, failing that, from the name with the generic theater letter.

void *__fastcall Load_Alloc_Data(FileClass &file);            // 0x00495230

void *BuildingTypeClass::Get_Image_Data() const
{
    BuildingTypeClass *self = (BuildingTypeClass *)this;
    if (unknown_a4 == 0 && DemandLoad) {
        if (unknown_13eb[0] != 0) {
            CCFileClass file(unknown_13eb);
            file.Is_Available();
            self->unknown_a4 = Load_Alloc_Data(file);
            if (unknown_a4 != 0) {
                self->unknown_a8 = true;
            } else {
                char name[260];
                strncpy(name, unknown_13eb, 19);
                Generic_Naming_Convention(name);
                CCFileClass generic(name);
                self->unknown_a4 = Load_Alloc_Data(generic);
                if (unknown_a4 != 0) {
                    self->unknown_a8 = true;
                }
            }
        }
        return unknown_a4;
    }
    return unknown_a4;
}


// bool BuildingTypeClass::unknown_func_458e00(void **image, int *frame) const (0x00458E00)
// For a type that leaves rubble: the image (slot 39) and frame 3 when the
// image has more than three frames per half (hypothesis: the rubble frame).

bool BuildingTypeClass::unknown_func_458e00(void **image, int *frame) const
{
    if (LeaveRubble && image) {
        *image = Get_Image_Data();
        if (*image && ((ShapeFileStruct *)*image)->FrameCount / 2 > 3) {
            *frame = 3;
            return true;
        }
    }
    return false;
}


// bool BuildingTypeClass::unknown_func_458e50(void **image, int *frame) const (0x00458E50)
// Like 0x00458E00, but the frame is half the frame count plus three.

bool BuildingTypeClass::unknown_func_458e50(void **image, int *frame) const
{
    if (LeaveRubble && image) {
        *image = Get_Image_Data();
        if (*image) {
            int half = ((ShapeFileStruct *)*image)->FrameCount / 2;
            if (half > 3) {
                *frame = half + 3;
                return true;
            }
        }
    }
    return false;
}


// Resolve building buildup, deployment, door, overlay and base images.
extern "C" void __cdecl _makepath(char *,char const *,char const *,char const *,char const *);
void const *__fastcall Unknown_Load_Shape_00495270(char const *name,bool &allocated);
void BuildingTypeClass::unknown_func_458ea0(int theater)
{
    char name[64];
    char extension[16];
    char fullname[260];
    if(!DemandLoadBuildup && strlen(BuildupFile)!=0) {
        _makepath(fullname,0,0,BuildupFile,".SHP");
        Theater_Naming_Convention(fullname,theater);
        void *shape=(void *)MixFileClass::Retrieve(fullname);
        if(shape==0) {
            Generic_Naming_Convention(fullname);
            CCFileClass file(fullname);
            shape=Load_Alloc_Data(file);
        }
        BuildupData=shape;
        if(shape) {
            int rate=1;
            int frames=((ShapeFileStruct *)shape)->FrameCount/2;
            if(Gate) frames=(int)((unsigned int)GateStages+1u);
            if(frames>0) rate=(int)(Rules->unknown_1178*900.0/frames);
            AnimFrames[0].unknown_00=0;
            AnimFrames[0].unknown_04=frames;
            AnimFrames[0].unknown_08=rate;
        }
    }
    ArtINI.Get_String(unknown_1c8,"DeployingAnim",unknown_string_83c55c,name,64);
    if(strlen(name)!=0) {
        _makepath(fullname,0,0,name,".SHP");
        Theater_Naming_Convention(fullname,theater);
        Free_unknown_11a0();
        unknown_11a0=(void *)Unknown_Load_Shape_00495270(fullname,unknown_11a4);
        if(unknown_11a0==0) {
            Generic_Naming_Convention(fullname);
            Free_unknown_11a0();
            unknown_11a0=(void *)Unknown_Load_Shape_00495270(fullname,unknown_11a4);
        }
    }
    ArtINI.Get_String(unknown_1c8,"RoofDeployingAnim",unknown_string_83c55c,name,64);
    if(strlen(name)!=0) {
        _makepath(fullname,0,0,name,".SHP");
        Theater_Naming_Convention(fullname,theater);
        Free_unknown_11b0();
        unknown_11b0=(void *)Unknown_Load_Shape_00495270(fullname,unknown_11b4);
        if(unknown_11b0==0) {
            Generic_Naming_Convention(fullname);
            Free_unknown_11b0();
            // The original roof fallback writes the deploying-image allocation flag.
            unknown_11b0=(void *)Unknown_Load_Shape_00495270(fullname,unknown_11a4);
        }
    }
    ArtINI.Get_String(unknown_1c8,"DoorAnim",unknown_string_83c55c,name,64);
    if(strlen(name)!=0) {
        _makepath(fullname,0,0,name,".SHP");
        Theater_Naming_Convention(fullname,theater);
        DoorAnimData_11c0=(void *)MixFileClass::Retrieve(fullname);
        if(DoorAnimData_11c0==0) {
            Generic_Naming_Convention(fullname);
            DoorAnimData_11c0=(void *)MixFileClass::Retrieve(fullname);
        }
    }
    ArtINI.Get_String(unknown_1c8,"UnderDoorAnim",unknown_string_83c55c,name,64);
    if(strlen(name)!=0) {
        _makepath(fullname,0,0,name,".SHP");
        Theater_Naming_Convention(fullname,theater);
        Free_unknown_11a8();
        unknown_11a8=(void *)Unknown_Load_Shape_00495270(fullname,unknown_11ac);
        if(unknown_11a8==0) {
            Generic_Naming_Convention(fullname);
            Free_unknown_11a8();
            unknown_11a8=(void *)MixFileClass::Retrieve(fullname);
        }
    }
    ArtINI.Get_String(unknown_1c8,"UnderRoofDoorAnim",unknown_string_83c55c,name,64);
    if(strlen(name)!=0) {
        _makepath(fullname,0,0,name,".SHP");
        Theater_Naming_Convention(fullname,theater);
        Free_unknown_11b8();
        unknown_11b8=(void *)Unknown_Load_Shape_00495270(fullname,unknown_11bc);
        if(unknown_11b8==0) {
            Generic_Naming_Convention(fullname);
            Free_unknown_11b8();
            unknown_11b8=(void *)MixFileClass::Retrieve(fullname);
        }
    }
    ArtINI.Get_String(unknown_1c8,"SpecialZOverlay",unknown_string_83c55c,name,64);
    if(strlen(name)!=0) {
        _makepath(fullname,0,0,name,".SHP");
        Theater_Naming_Convention(fullname,theater);
        SpecialZOverlayData_11c4=(void *)MixFileClass::Retrieve(fullname);
        if(SpecialZOverlayData_11c4==0) {
            Generic_Naming_Convention(fullname);
            SpecialZOverlayData_11c4=(void *)MixFileClass::Retrieve(fullname);
        }
    }
    ArtINI.Get_String(unknown_1c8,"BibShape",unknown_string_83c55c,name,64);
    if(strlen(name)!=0) {
        _makepath(fullname,0,0,name,".SHP");
        Theater_Naming_Convention(fullname,theater);
        Free_unknown_11cc();
        unknown_11cc=(void *)Unknown_Load_Shape_00495270(fullname,unknown_11d0);
        if(unknown_11cc==0) {
            Generic_Naming_Convention(fullname);
            Free_unknown_11cc();
            unknown_11cc=(void *)MixFileClass::Retrieve(fullname);
        }
    }
    ArtINI.Get_String(unknown_1c8,"Image",unknown_string_83c55c,name,64);
    if(unknown_1fc && theater!=-1) strcpy(extension,Theaters[theater].Suffix);
    else strcpy(extension,".SHP");
    if(strlen(name)!=0) _makepath(fullname,0,0,name,extension);
    else _makepath(fullname,0,0,unknown_1c8,extension);
    Theater_Naming_Convention(fullname,theater);
    strncpy(unknown_13eb,fullname,19);
    if(!DemandLoad) {
        unknown_a4=(void *)MixFileClass::Retrieve(fullname);
        if(unknown_a4==0) {
            Generic_Naming_Convention(fullname);
            strncpy(unknown_13eb,fullname,19);
            unknown_a4=(void *)MixFileClass::Retrieve(fullname);
        }
    }
    unknown_func_459640();
}


// Load the building turret and barrel voxel/motion pairs.
extern "C" char *__cdecl _strupr(char *);
extern "C" char *__cdecl strstr(char const *,char const *);
extern "C" void __cdecl _makepath(char *,char const *,char const *,char const *,char const *);
void BuildingTypeClass::unknown_func_459640()
{
    if(TurretAnimIsVoxel || BarrelAnimIsVoxel) {
        char name[128];
        char fullname[256];
        strcpy(name,Anims[9].Anim.Buffer);
        _makepath(fullname,0,0,name,".VXL");
        if(strstr(_strupr(fullname+4),_strupr("TUR"))) {
            CCFileClass turret(fullname);
            if(turret.Is_Available()) {
                delete unknown_b8.unknown_00;
                unknown_b8.unknown_00=new VoxelLibrary(&turret);
                _makepath(fullname,0,0,name,".HVA");
                CCFileClass motion(fullname);
                delete unknown_b8.unknown_04;
                unknown_b8.unknown_04=new UnknownPairSecond(&motion);
                if(unknown_b8.unknown_04 && !unknown_b8.unknown_04->unknown_00) {
                    unknown_b8.unknown_04->Scale_0059ed30(unknown_b8.unknown_00->Get_Layer_Info(0,0)->unknown_0c);
                }
            }
            char *suffix=strstr(_strupr(name+4),_strupr("TUR"));
            if(suffix) {
                *suffix++='B';
                *suffix++='A';
                *suffix++='R';
                *suffix++='L';
                *suffix=0;
            }
        } else if(BarrelAnimIsVoxel) {
            strncpy(name,VoxelBarrelFile,128);
        }
        _makepath(fullname,0,0,name,".VXL");
        CCFileClass barrel(fullname);
        if(barrel.Is_Available()) {
            delete unknown_c0.unknown_00;
            unknown_c0.unknown_00=new VoxelLibrary(&barrel);
            _makepath(fullname,0,0,name,".HVA");
            CCFileClass motion(fullname);
            delete unknown_c0.unknown_04;
            unknown_c0.unknown_04=new UnknownPairSecond(&motion);
            if(unknown_c0.unknown_04 && !unknown_c0.unknown_04->unknown_00) {
                unknown_c0.unknown_04->Scale_0059ed30(unknown_c0.unknown_00->Get_Layer_Info(0,0)->unknown_0c);
            }
        }
    }
}


// BuildingTypeClass::Read_INI (0x00459A00, slot 25)
// The techno keys, then the building keys: rules keys from the type's own
// section and art keys from the image section of the art INI.
extern "C" char *__cdecl _itoa(int value, char *buffer, int radix);

// The name of one building animation (art INI) and its Damaged and
// Garrisoned variants; a variant left empty takes the plain name.






static bool Same_Point(Point2D const &first, Point2D const &second) { return first.X == second.X && first.Y == second.Y; }
bool BuildingTypeClass::Read_INI(CCINIClass &ini)
{
    char key[128];
    char triple_out[64];
    char triple_def[64];
    char dock_key[40];
    char y_key[64];
    char ysort_key[64];
    char anim_key[64];
    char damaged_key[64];
    char z_key[64];
    char x_key[64];
    if (TechnoTypeClass::Read_INI(ini)) {
        BuildCat = ini.Get_BuildCat(ID, "BuildCat", (::BuildCat)BuildCat);
        HasSpotlight = ini.Get_Bool(ID, "HasSpotlight", HasSpotlight);
        (INIPoint3D &)HalfDamageSmokeLocation1 = ini.Get_Point(ID, "HalfDamageSmokeLocation1", (INIPoint3D &)HalfDamageSmokeLocation1);
        (INIPoint3D &)HalfDamageSmokeLocation2 = ini.Get_Point(ID, "HalfDamageSmokeLocation2", (INIPoint3D &)HalfDamageSmokeLocation2);
        Radar = ini.Get_Bool(ID, "Radar", Radar);
        SpySat = ini.Get_Bool(ID, "SpySat", SpySat);
        SpeedType = ini.Get_Bool(ID, "WaterBound", SpeedType == 5) ? 5 : -1;
        Adjacent = ini.Get_Int(ID, "Adjacent", Adjacent);
        Capturable = ini.Get_Bool(ID, "Capturable", Capturable);
        Powered = ini.Get_Bool(ID, "Powered", Powered);
        Overpowerable = ini.Get_Bool(ID, "Overpowerable", Overpowerable);
        Spyable = ini.Get_Bool(ID, "Spyable", Spyable);
        CanC4 = ini.Get_Bool(ID, "CanC4", CanC4);
        WantsExtraSpace = ini.Get_Bool(ID, "WantsExtraSpace", WantsExtraSpace);
        Bib = ini.Get_Bool(ID, "Bib", Bib);
        Unsellable = ini.Get_Bool(ID, "Unsellable", Unsellable);
        ClickRepairable = ini.Get_Bool(ID, "ClickRepairable", ClickRepairable);
        CanBeOccupied = ini.Get_Bool(ID, "CanBeOccupied", CanBeOccupied);
        ShowOccupantPips = ini.Get_Bool(ID, "ShowOccupantPips", ShowOccupantPips);
        MaxNumberOccupants = ini.Get_Int(ID, "MaxNumberOccupants", MaxNumberOccupants);
        NumberImpassableRows = ini.Get_Int(ID, "NumberImpassableRows", NumberImpassableRows);
        ProduceCashStartup = ini.Get_Int(ID, "ProduceCashStartup", ProduceCashStartup);
        ProduceCashAmount = ini.Get_Int(ID, "ProduceCashAmount", ProduceCashAmount);
        ProduceCashDelay = ini.Get_Int(ID, "ProduceCashDelay", ProduceCashDelay);
        BaseNormal = ini.Get_Bool(ID, "BaseNormal", BaseNormal);
        EligibileForAllyBuilding = ini.Get_Bool(ID, "EligibileForAllyBuilding", EligibileForAllyBuilding);
        EligibleForDelayKill = ini.Get_Bool(ID, "EligibleForDelayKill", EligibleForDelayKill);
        NeedsEngineer = ini.Get_Bool(ID, "NeedsEngineer", NeedsEngineer);
        if (NeedsEngineer) {
            Sight = 6;
        }
        ProtectWithWall = ini.Get_Bool(ID, "ProtectWithWall", ProtectWithWall);
        int index;
        {
        char number[8];
        for (index = 0; index < MaxNumberOccupants; index++) {
            strcpy(key, "MuzzleFlash");
            _itoa(index, number, 10);
            Point2D point = ArtINI.Get_Point(unknown_1c8, strcat(key, number), Point2D(0xFFFF, 0xFFFF));
            MuzzleFlash[index] = Point2D(point.X, point.Y);
            if (Same_Point(MuzzleFlash[index], Point2D(0xFFFF, 0xFFFF))) {
                MuzzleFlash[index] = Point2D(0, 0);
            }
        }
        for (index = 0; index < 8; index++) {
            strcpy(key, "DamageFireOffset");
            _itoa(index, number, 10);
            Point2D point = ArtINI.Get_Point(unknown_1c8, strcat(key, number), Point2D(0xFFFF, 0xFFFF));
            DamageFireOffset[index] = Point2D(point.X, point.Y);
            if (Same_Point(DamageFireOffset[index], Point2D(0xFFFF, 0xFFFF))) {
                DamageFireOffset[index] = Point2D(unknown_geometry_l.X, unknown_geometry_l.Y);
                break;
            }
        }
        }
        Wall = ini.Get_Bool(ID, "Wall", Wall);
        Weeder = ini.Get_Bool(ID, "Weeder", Weeder);
        Helipad = ini.Get_Bool(ID, "Helipad", Helipad);
        OrePurifier = ini.Get_Bool(ID, "OrePurifier", OrePurifier);
        Factory = ini.Get_RTTIType(ID, "Factory", (RTTIType)Factory);
        FreeUnit = Get_Type(ini, ID, "FreeUnit", FreeUnit);
        HoverPad = ini.Get_Bool(ID, "HoverPad", HoverPad);
        IsTemple = ini.Get_Bool(ID, "IsTemple", IsTemple);
        IsPlug = ini.Get_Bool(ID, "IsPlug", IsPlug);
        TogglePower = ini.Get_Bool(ID, "TogglePower", TogglePower);
        BuildupSound = Get_Sound(ini, ID, "BuildupSound", BuildupSound);
        PackupSound = Get_Sound(ini, ID, "PackupSound", PackupSound);
        CreateUnitSound = Get_Sound(ini, ID, "CreateUnitSound", CreateUnitSound);
        UnitExitSound = Get_Sound(ini, ID, "UnitExitSound", UnitExitSound);
        UnitEnterSound = Get_Sound(ini, ID, "UnitEnterSound", UnitEnterSound);
        WorkingSound = Get_Sound(ini, ID, "WorkingSound", WorkingSound);
        NotWorkingSound = Get_Sound(ini, ID, "NotWorkingSound", NotWorkingSound);
        UnitRepair = ini.Get_Bool(ID, "UnitRepair", UnitRepair);
        UnitReload = ini.Get_Bool(ID, "UnitReload", UnitReload);
        Cloning = ini.Get_Bool(ID, "Cloning", Cloning);
        DockUnload = ini.Get_Bool(ID, "DockUnload", DockUnload);
        Gate = ini.Get_Bool(ID, "Gate", Gate);
        SAM = ini.Get_Bool(ID, "SAM", SAM);
        ConstructionYard = ini.Get_Bool(ID, "ConstructionYard", ConstructionYard);
        NukeSilo = ini.Get_Bool(ID, "NukeSilo", NukeSilo);
        Refinery = ini.Get_Bool(ID, "Refinery", Refinery);
        WeaponsFactory = ini.Get_Bool(ID, "WeaponsFactory", WeaponsFactory);
        LaserFencePost = ini.Get_Bool(ID, "LaserFencePost", LaserFencePost);
        LaserFence = ini.Get_Bool(ID, "LaserFence", LaserFence);
        FirestormWall = ini.Get_Bool(ID, "FirestormWall", FirestormWall);
        Hospital = ini.Get_Bool(ID, "Hospital", Hospital);
        Armory = ini.Get_Bool(ID, "Armory", Armory);
        GDIBarracks = ini.Get_Bool(ID, "GDIBarracks", GDIBarracks);
        NODBarracks = ini.Get_Bool(ID, "NODBarracks", NODBarracks);
        EMPulseCannon = ini.Get_Bool(ID, "EMPulseCannon", EMPulseCannon);
        TickTank = ini.Get_Bool(ID, "TickTank", TickTank);
        ChargedAnimTime = (float)ini.Get_Double(ID, "ChargedAnimTime", ChargedAnimTime);
        SuperWeapon = ini.Get_SuperWeaponType(ID, "SuperWeapon", SuperWeapon);
        SuperWeapon2 = ini.Get_SuperWeaponType(ID, "SuperWeapon2", SuperWeapon2);
        CloakGenerator = ini.Get_Bool(ID, "CloakGenerator", CloakGenerator);
        SensorArray = ini.Get_Bool(ID, "SensorArray", SensorArray);
        CloakRadiusInCells = ini.Get_Int(ID, "CloakRadiusInCells", CloakRadiusInCells);
        PsychicDetectionRadius = ini.Get_Int(ID, "PsychicDetectionRadius", PsychicDetectionRadius);
        BarrelStartPitch = ini.Get_Int(ID, "BarrelStartPitch", BarrelStartPitch >> 5) << 5;
        LightVisibility = ini.Get_Int(ID, "LightVisibility", LightVisibility);
        // The light values are stored in thousandths, rounded up by 0.1. Reading each through a
        // double local reproduces the original schedule of the LightGreenTint call (Sched29).
        {
            double value = ini.Get_Double(ID, "LightIntensity", (double)(LightIntensity / 1000));
            LightIntensity = (int)(value * 1000.0 + 0.1);
        }
        {
            double value = ini.Get_Double(ID, "LightRedTint", (double)(LightRedTint / 1000));
            LightRedTint = (int)(value * 1000.0 + 0.1);
        }
        {
            double value = ini.Get_Double(ID, "LightGreenTint", (double)(LightGreenTint / 1000));
            LightGreenTint = (int)(value * 1000.0 + 0.1);
        }
        {
            double value = ini.Get_Double(ID, "LightBlueTint", (double)(LightBlueTint / 1000));
            LightBlueTint = (int)(value * 1000.0 + 0.1);
        }
        GateCloseDelay = ini.Get_Double(ID, "GateCloseDelay", GateCloseDelay);
        InvisibleInGame = ini.Get_Bool(ID, "InvisibleInGame", InvisibleInGame);
        if (InvisibleInGame) {
            Invisible = true;
            RadarVisible = false;
        }
        ini.Get_String(ID, "PowersUpBuilding", "", key, 24);
        if (strlen(key) != 0) {
            strcpy(PowersUpBuilding, key);
        }
        PowersUpToLevel = ini.Get_Int(ID, "PowersUpToLevel", PowersUpToLevel);
        BridgeRepairHut = ini.Get_Bool(ID, "BridgeRepairHut", BridgeRepairHut);
        HasStupidGuardMode = ini.Get_Bool(ID, "HasStupidGuardMode", HasStupidGuardMode);
        CrateBeneath = ini.Get_Bool(ID, "CrateBeneath", CrateBeneath);
        CrateBeneathIsMoney = ini.Get_Bool(ID, "CrateBeneathIsMoney", CrateBeneathIsMoney);
        LeaveRubble = ini.Get_Bool(ID, "LeaveRubble", LeaveRubble);
        PlaceAnywhere = ini.Get_Bool(ID, "PlaceAnywhere", PlaceAnywhere);
        ICBMLauncher = ini.Get_Bool(ID, "ICBMLauncher", ICBMLauncher);
        Artillary = ini.Get_Bool(ID, "Artillary", Artillary);
        {
            INIPoint3D offset = (INIPoint3D &)TargetCoordOffset;
            TargetCoordOffset = ini.Get_Point(ID, "TargetCoordOffset", offset);
        }
        {
            INIPoint3D offset = (INIPoint3D &)ExitCoord;
            ExitCoord = ini.Get_Point(ID, "ExitCoord", offset);
        }
        AIBuildThis = ini.Get_Bool(ID, "AIBuildThis", AIBuildThis);
        IsBaseDefense = ini.Get_Bool(ID, "IsBaseDefense", IsBaseDefense);
        IsThreatRatingNode = ini.Get_Bool(ID, "IsThreatRatingNode", IsThreatRatingNode);
        ConcentricRadialIndicator = ini.Get_Bool(ID, "ConcentricRadialIndicator", ConcentricRadialIndicator);
        RotCount = Turret ? 32 : 1;
        Power = ini.Get_Int(ID, "Power", Power > 0 ? Power : -PowerDrain_c6c);
        if (Power < 0) {
            PowerDrain_c6c = -Power;
            Power = 0;
        } else {
            PowerDrain_c6c = 0;
        }
        ExtraPower = ini.Get_Int(ID, "ExtraPower", ExtraPower > 0 ? ExtraPower : -ExtraPowerDrain_c74);
        if (ExtraPower < 0) {
            ExtraPowerDrain_c74 = -ExtraPower;
            ExtraPower = 0;
        }
        Height = ArtINI.Get_Int(unknown_1c8, "Height", Height);
        OccupyHeight = ArtINI.Get_Int(unknown_1c8, "OccupyHeight", Height);
        Recoilless = ArtINI.Get_Bool(unknown_1c8, "Recoilless", Recoilless);
        Flat = ArtINI.Get_Bool(unknown_1c8, "Flat", Flat);
        SiloDamage = ArtINI.Get_Bool(unknown_1c8, "SiloDamage", SiloDamage);
        ChargeAnim = ArtINI.Get_Bool(unknown_1c8, "ChargeAnim", ChargeAnim);
        IsAnimDelayedFire = ArtINI.Get_Bool(unknown_1c8, "IsAnimDelayedFire", IsAnimDelayedFire);
        DelayedFireDelay = ArtINI.Get_Int(unknown_1c8, "DelayedFireDelay", DelayedFireDelay);
        ToOverlay = Get_Type(ArtINI, unknown_1c8, "ToOverlay", ToOverlay);
        Foundation = ArtINI.Get_Foundation(unknown_1c8, "Foundation", Foundation);
        int foundation = ArtINI.Get_Foundation(ID, "Foundation", Foundation);
        if (foundation != 0) {
            Foundation = foundation;
        }
        MidPoint = ArtINI.Get_Int(unknown_1c8, "MidPoint", MidPoint);
        DoorStages = ArtINI.Get_Int(unknown_1c8, "DoorStages", DoorStages);
        DamagedDoor = ArtINI.Get_Bool(unknown_1c8, "DamagedDoor", DamagedDoor);
        TerrainPalette = ArtINI.Get_Bool(unknown_1c8, "TerrainPalette", TerrainPalette);
        GateStages = ArtINI.Get_Int(unknown_1c8, "GateStages", GateStages);
        PrimaryFirePixelOffset = ArtINI.Get_Point(unknown_1c8, "PrimaryFirePixelOffset", PrimaryFirePixelOffset);
        SecondaryFirePixelOffset = ArtINI.Get_Point(unknown_1c8, "SecondaryFirePixelOffset", SecondaryFirePixelOffset);
        PrimaryFireDualOffset = ArtINI.Get_Bool(unknown_1c8, "PrimaryFireDualOffset", PrimaryFireDualOffset);
        ExtraDamageStage = ArtINI.Get_Bool(unknown_1c8, "ExtraDamageStage", ExtraDamageStage);
        SpecialZOverlayZAdjust = ArtINI.Get_Int(unknown_1c8, "SpecialZOverlayZAdjust", SpecialZOverlayZAdjust);
        NormalZAdjust = ArtINI.Get_Int(unknown_1c8, "NormalZAdjust", NormalZAdjust);
        ZShapePointMove = ArtINI.Get_Point(unknown_1c8, "ZShapePointMove", ZShapePointMove);
        ExtraLight = ArtINI.Get_Int(unknown_1c8, "ExtraLight", ExtraLight);
        DemandLoad = ArtINI.Get_Bool(unknown_1c8, "DemandLoad", DemandLoad);
        DemandLoadBuildup = ArtINI.Get_Bool(unknown_1c8, "DemandLoadBuildup", DemandLoadBuildup);
        FreeBuildup = ArtINI.Get_Bool(unknown_1c8, "FreeBuildup", FreeBuildup);
        CanHideThings = ArtINI.Get_Bool(ID, "CanHideThings", CanHideThings);
        CanBeHidden = ArtINI.Get_Bool(ID, "CanBeHidden", CanBeHidden);
        for (index = 0; index < 8; index++) {
            wsprintfA(key, "AddOccupy%d", index + 1);
            AddOccupy[index] = ArtINI.Get_Point(unknown_1c8, key, Point2D(0xFFFF, 0xFFFF));
            wsprintfA(key, "RemoveOccupy%d", index + 1);
            RemoveOccupy[index] = ArtINI.Get_Point(unknown_1c8, key, Point2D(0xFFFF, 0xFFFF));
        }
        QueueingCell = ArtINI.Get_Point(unknown_1c8, "QueueingCell", Point2D(0, 0));
        FoundationData = unknown_foundation_cells_84ede8[Foundation];
        FoundationOutside = unknown_foundation_outside_84f848[Foundation];
        ArtINI.Get_String(unknown_1c8, "Buildup", "", key, 16);
        if (strlen(key) != 0) {
            strcpy(BuildupFile, key);
        }
        unknown_func_458ea0(unknown_id_source_a3d290->Theater);
        sprintf(triple_def, "%d,%d,%d", AnimFrames[1].unknown_00, AnimFrames[1].unknown_04, AnimFrames[1].unknown_08);
        if (ArtINI.Get_String(unknown_1c8, "AnimIdle", triple_def, triple_out, 64) > 0) {
            sscanf(triple_out, "%d,%d,%d", &AnimFrames[1].unknown_00, &AnimFrames[1].unknown_04, &AnimFrames[1].unknown_08);
        }
        sprintf(triple_def, "%d,%d,%d", AnimFrames[2].unknown_00, AnimFrames[2].unknown_04, AnimFrames[2].unknown_08);
        if (ArtINI.Get_String(unknown_1c8, "AnimActive", triple_def, triple_out, 64) > 0) {
            sscanf(triple_out, "%d,%d,%d", &AnimFrames[2].unknown_00, &AnimFrames[2].unknown_04, &AnimFrames[2].unknown_08);
        }
        sprintf(triple_def, "%d,%d,%d", AnimFrames[4].unknown_00, AnimFrames[4].unknown_04, AnimFrames[4].unknown_08);
        if (ArtINI.Get_String(unknown_1c8, "AnimAux1", triple_def, triple_out, 64) > 0) {
            sscanf(triple_out, "%d,%d,%d", &AnimFrames[4].unknown_00, &AnimFrames[4].unknown_04, &AnimFrames[4].unknown_08);
        }
        sprintf(triple_def, "%d,%d,%d", AnimFrames[5].unknown_00, AnimFrames[5].unknown_04, AnimFrames[5].unknown_08);
        if (ArtINI.Get_String(unknown_1c8, "AnimAux2", triple_def, triple_out, 64) > 0) {
            sscanf(triple_out, "%d,%d,%d", &AnimFrames[5].unknown_00, &AnimFrames[5].unknown_04, &AnimFrames[5].unknown_08);
        }
        READ_ANIM(3, 3, "ActiveAnim")
        READ_ANIM(4, 4, "ActiveAnimTwo")
        READ_ANIM(5, 5, "ActiveAnimThree")
        READ_ANIM(6, 6, "ActiveAnimFour")
        READ_ANIM(13, 6, "SuperAnim")
        READ_ANIM(14, 4, "SuperAnimTwo")
        READ_ANIM(15, 5, "SuperAnimThree")
        READ_ANIM(16, 5, "SuperAnimFour")
        READ_ANIM(10, 10, "SpecialAnim")
        READ_ANIM(11, 11, "SpecialAnimTwo")
        READ_ANIM(12, 12, "SpecialAnimThree")
        READ_ANIM_PLAIN(8, 8, "ProductionAnim")
        READ_ANIM_PLAIN(17, 17, "IdleAnim")
        READ_ANIM_PLAIN(7, 7, "PreProductionAnim")
        ini.Get_String(ID, "TurretAnim", "", key, 16);
        if (strlen(key) != 0) {
            strcpy(Anims[9].Anim.Buffer, key);
        }
        ini.Get_String(ID, "TurretAnimDamaged", "", key, 16);
        if (strlen(key) != 0) {
            strcpy(Anims[9].Damaged.Buffer, key);
        }
        if (strlen(Anims[9].Damaged.Buffer) == 0) {
            strcpy(Anims[9].Damaged.Buffer, Anims[9].Anim.Buffer);
        }
        ini.Get_String(unknown_1c8, "TurretAnimGarrisoned", "", key, 16);
        if (strlen(key) != 0) {
            strcpy(Anims[9].Garrisoned.Buffer, key);
        }
        if (strlen(Anims[9].Garrisoned.Buffer) == 0) {
            strcpy(Anims[9].Garrisoned.Buffer, Anims[9].Anim.Buffer);
        }
        Anims[9].Offset.X = ini.Get_Int(ID, "TurretAnimX", Anims[9].Offset.X);
        Anims[9].Offset.Y = ini.Get_Int(ID, "TurretAnimY", Anims[9].Offset.Y);
        Anims[9].ZAdjust = ini.Get_Int(ID, "TurretAnimZAdjust", Anims[9].ZAdjust);
        Anims[9].YSort = ini.Get_Int(ID, "TurretAnimYSort", Anims[9].YSort);
        TurretAnimIsVoxel = ini.Get_Bool(ID, "TurretAnimIsVoxel", TurretAnimIsVoxel);
        BarrelAnimIsVoxel = ini.Get_Bool(ID, "BarrelAnimIsVoxel", BarrelAnimIsVoxel);
        ini.Get_String(ID, "VoxelBarrelFile", VoxelBarrelFile, VoxelBarrelFile, 16);
        (INIPoint3D &)VoxelBarrelOffsetToPitchPivotPoint = ini.Get_Point(ID, "VoxelBarrelOffsetToPitchPivotPoint", (INIPoint3D &)VoxelBarrelOffsetToPitchPivotPoint);
        (INIPoint3D &)VoxelBarrelOffsetToRotatePivotPoint = ini.Get_Point(ID, "VoxelBarrelOffsetToRotatePivotPoint", (INIPoint3D &)VoxelBarrelOffsetToRotatePivotPoint);
        (INIPoint3D &)VoxelBarrelOffsetToBuildingPivotPoint = ini.Get_Point(ID, "VoxelBarrelOffsetToBuildingPivotPoint", (INIPoint3D &)VoxelBarrelOffsetToBuildingPivotPoint);
        (INIPoint3D &)VoxelBarrelOffsetToBarrelEnd = ini.Get_Point(ID, "VoxelBarrelOffsetToBarrelEnd", (INIPoint3D &)VoxelBarrelOffsetToBarrelEnd);
        Upgrades = ini.Get_Int(ID, "Upgrades", Upgrades);
        if (Upgrades > 0) {
            for (index = 0; index < Upgrades; index++) {
                sprintf(anim_key, "PowerUp%01dAnim", index + 1);
                sprintf(damaged_key, "PowerUp%01dDamagedAnim", index + 1);
                sprintf(x_key, "PowerUp%01dLocXX", index + 1);
                sprintf(y_key, "PowerUp%01dLocYY", index + 1);
                sprintf(z_key, "PowerUp%01dLocZZ", index + 1);
                sprintf(ysort_key, "PowerUp%01dYSort", index + 1);
                ArtINI.Get_String(unknown_1c8, anim_key, "", key, 16);
                if (strlen(key) != 0) {
                    strcpy(Anims[index].Anim.Buffer, key);
                }
                ArtINI.Get_String(unknown_1c8, damaged_key, "", key, 16);
                if (strlen(key) != 0) {
                    strcpy(Anims[index].Damaged.Buffer, key);
                }
                Anims[index].Offset.X = ArtINI.Get_Int(unknown_1c8, x_key, Anims[index].Offset.X);
                Anims[index].Offset.Y = ArtINI.Get_Int(unknown_1c8, y_key, Anims[index].Offset.Y);
                Anims[index].ZAdjust = ArtINI.Get_Int(unknown_1c8, z_key, Anims[index].ZAdjust);
                Anims[index].YSort = ArtINI.Get_Int(unknown_1c8, ysort_key, Anims[index].YSort);
            }
        }
        int docks = NumberOfDocks;
        NumberOfDocks = ini.Get_Int(ID, "NumberOfDocks", docks);
        if (NumberOfDocks > DockingOffsets.Length()) {
            DockingOffsets.Resize(NumberOfDocks);
            for (index = docks; index < NumberOfDocks; index++) {
                DockingOffsets[index] = TPoint3D<int>(0, 0, 0);
            }
        }
        for (index = 0; index < NumberOfDocks; index++) {
            sprintf(dock_key, "DockingOffset%d", index);
            INIPoint3D offset = (INIPoint3D &)DockingOffsets[index];
            offset = ArtINI.Get_Point(unknown_1c8, dock_key, offset);
            (INIPoint3D &)DockingOffsets[index] = offset;
        }
        return true;
    }
    return false;
}


// CoordStruct BuildingTypeClass::Coord_Fixup(CoordStruct const &coord) const (0x0045D830)
// The coordinate placed on the ground.

CoordStruct BuildingTypeClass::Coord_Fixup(CoordStruct const &coord) const
{
    CoordStruct result = coord;
    result.Z = Map.Get_Height_GL(result);
    return result;
}


// bool BuildingTypeClass::Legal_Placement(Cell const &cell, HouseClass *house) const (0x0045D880)
// PlaceAnywhere types may always be placed.

bool BuildingTypeClass::Legal_Placement(Cell const &cell, HouseClass *house) const
{
    if (PlaceAnywhere) {
        return true;
    }
    return TechnoTypeClass::Legal_Placement(cell, house);
}


// CoordStruct BuildingTypeClass::Lepton_Dimensions() const (0x0045D8B0)
// The foundation's extent in leptons, Height= levels high.

CoordStruct BuildingTypeClass::Lepton_Dimensions() const
{
    return CoordStruct(unknown_foundation_width_7cfc70[Foundation] * 256,
                       unknown_foundation_height_7cfcc8[Foundation] * 256,
                       Height * unknown_geometry_g);
}


// void BuildingTypeClass::Compute_CRC(WWCRCEngine &crc) const (0x0045D8F0)
// The techno CRC, then the building values in Westwood's order.

void BuildingTypeClass::Compute_CRC(WWCRCEngine &crc) const
{
    TechnoTypeClass::Compute_CRC(crc);
    crc(GateCloseDelay);
    crc((long)LightVisibility);
    crc((long)LightIntensity);
    crc((long)LightRedTint);
    crc((long)LightGreenTint);
    crc((long)LightBlueTint);
    crc(BuildupFile);
    crc(PowersUpBuilding);
    crc((long)Adjacent);
    crc((long)Factory);
    crc((long)Power);
    crc((long)PowerDrain_c6c);
    crc((long)ExtraPower);
    crc((long)ExtraPowerDrain_c74);
    crc((long)Foundation);
    crc((long)Height);
    crc((long)OccupyHeight);
    crc((long)MidPoint);
    crc((long)DoorStages);
    crc(AnimFrames, sizeof(AnimFrames));
    crc(Anims, sizeof(Anims));
    crc((long)Upgrades);
    crc((long)SpecialZOverlayZAdjust);
    crc(HoverPad);
    crc(IsPlug);
    crc(IsTemple);
    crc(HasSpotlight);
    crc(BaseNormal);
    crc(EligibileForAllyBuilding);
    crc(NeedsEngineer);
    crc(Bib);
    crc(Wall);
    crc(Capturable);
    crc(Powered);
    crc(Unsellable);
    crc(Radar);
    crc(SpySat);
    crc(ChargeAnim);
    crc(IsAnimDelayedFire);
    crc((long)DelayedFireDelay);
    crc(SiloDamage);
    crc(UnitRepair);
    crc(UnitReload);
    crc(Cloning);
    crc(Flat);
    crc(DockUnload);
    crc(Recoilless);
    crc(BridgeRepairHut);
    crc(Gate);
    crc(SAM);
    crc(ConstructionYard);
    crc(NukeSilo);
    crc(Refinery);
    crc(WeaponsFactory);
    crc(LaserFencePost);
    crc(LaserFence);
    crc(FirestormWall);
    crc(Helipad);
    crc(OrePurifier);
    crc(ExtraDamageStage);
    crc(Hospital);
    crc(Armory);
    crc((long)GateStages);
    crc((long)PowersUpToLevel);
    crc(DamagedDoor);
    crc(InvisibleInGame);
    crc(TerrainPalette);
    crc(TurretAnimIsVoxel);
    crc(BarrelAnimIsVoxel);
    crc(VoxelBarrelFile);
    crc(unknown_13a8);
    crc(&VoxelBarrelOffsetToPitchPivotPoint, sizeof(VoxelBarrelOffsetToPitchPivotPoint));
    crc(&VoxelBarrelOffsetToRotatePivotPoint, sizeof(VoxelBarrelOffsetToRotatePivotPoint));
    crc(&VoxelBarrelOffsetToBuildingPivotPoint, sizeof(VoxelBarrelOffsetToBuildingPivotPoint));
    crc(&VoxelBarrelOffsetToBarrelEnd, sizeof(VoxelBarrelOffsetToBarrelEnd));
    crc(CrateBeneath);
    crc(LeaveRubble);
    crc(CrateBeneathIsMoney);
    crc((long)NumberOfDocks);
    crc((long)BarrelStartPitch);
    crc(ConcentricRadialIndicator);
}


// HRESULT __stdcall BuildingTypeClass::Load(IStream *stream) (0x0045DD90)
// Frees the loaded images, loads the techno part, rebuilds the object in
// place, reads the docking offsets, then restores the foundation lists,
// pointers and images for the scenario's theater.

HRESULT __stdcall BuildingTypeClass::Load(IStream *stream)
{
    Free_Buildup();
    Free_unknown_11cc();
    Free_unknown_11a0();
    Free_unknown_11a8();
    Free_unknown_11b0();
    Free_unknown_11b8();
    HRESULT result = TechnoTypeClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) BuildingTypeClass(NoInitClass());
        int count;
        result = stream->Read(&count, sizeof(count), 0);
        if (FAILED(result)) {
            return result;
        }
        new (&DockingOffsets) VectorClass<TPoint3D<int> >();
        DockingOffsets.Resize(count);
        for (int index = 0; index < count; index++) {
            TPoint3D<int> point(0, 0, 0);
            result = stream->Read(&point, sizeof(point), 0);
            if (FAILED(result)) {
                return result;
            }
            DockingOffsets[index] = point;
        }
        unknown_func_459640();
        Fetch_Normal_Image();
        unknown_bf0 = 0;
        FoundationData = unknown_foundation_cells_84ede8[Foundation];
        FoundationOutside = unknown_foundation_outside_84f848[Foundation];
        Swizzler.Swizzle((void **)&ToOverlay);
        Swizzler.Swizzle((void **)&FreeUnit);
        unknown_func_459640();
        BuildupData = 0;
        BuildupDataAllocated = false;
        unknown_11cc = 0;
        unknown_11d0 = false;
        unknown_11a0 = 0;
        unknown_11a4 = false;
        unknown_11a8 = 0;
        unknown_11ac = false;
        unknown_11b0 = 0;
        unknown_11b4 = false;
        unknown_11b8 = 0;
        unknown_11bc = false;
        unknown_func_458ea0(unknown_id_source_a3d290->Theater);
        return S_OK;
    }
    return result;
}


// HRESULT __stdcall BuildingTypeClass::Save(IStream *stream, BOOL clear_dirty) (0x0045E020)
// The techno part, then the docking offsets (count and points).

HRESULT __stdcall BuildingTypeClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = TechnoTypeClass::Save(stream, clear_dirty);
    if (FAILED(result)) {
        return result;
    }
    int count = DockingOffsets.Length();
    result = stream->Write(&count, sizeof(count), 0);
    if (FAILED(result)) {
        return result;
    }
    for (int index = 0; index < count; index++) {
        result = stream->Write(&DockingOffsets[index], sizeof(TPoint3D<int>), 0);
        if (FAILED(result)) {
            return result;
        }
    }
    return S_OK;
}


// HRESULT __stdcall BuildingTypeClass::GetClassID(GUID *clsid) (0x0045E0A0)

HRESULT __stdcall BuildingTypeClass::GetClassID(GUID *clsid)
{
    if (clsid == 0) {
        return E_POINTER;
    }
    *clsid = unknown_clsid_building_type_7a1b30;
    return S_OK;
}


// static BuildingTypeClass *__fastcall BuildingTypeClass::Find_Or_Make(char const *name) (0x0045E0E0)
// The type with this ID, a new one when unknown, 0 for "<none>"/"none".

BuildingTypeClass *__fastcall BuildingTypeClass::Find_Or_Make(char const *name)
{
    if (_stricmp("<none>", name) == 0 || _stricmp("none", name) == 0) {
        return 0;
    }
    for (int index = 0; index < BuildingTypes.Count(); index++) {
        if (_stricmp(BuildingTypes[index]->ID, name) == 0) {
            return BuildingTypes[index];
        }
    }
    return new BuildingTypeClass(name);
}


// static void BuildingTypeClass::unknown_func_45e170() (0x0045E170)
// Rebuilds the cloak surface for the largest CloakRadiusInCells, then gives
// each type the isometric tile type its "ToTile" key (in the INI pointed to by
// 0x008396D0) names (unknown_bf0).

class IsometricTileTypeClass;
extern CCINIClass *unknown_ini_8396d0;                       // 0x008396D0
extern DynamicVectorClass<IsometricTileTypeClass *> IsoTileTypes;
int __fastcall Unknown_00529400(char const *name);           // 0x00529400 (IsometricTileTypeClass: the index by ID)

void BuildingTypeClass::unknown_func_45e170()
{
    int radius = 0;
    for (int index = 0; index < BuildingTypes.Count(); index++) {
        BuildingTypeClass *type = BuildingTypes[index];
        if (radius <= type->CloakRadiusInCells) {
            radius = type->CloakRadiusInCells;
        }
    }
    if (radius != 0) {
        delete unknown_cloak_surface_8502a0;
        unknown_cloak_surface_8502a0 = new BSurface(radius + 16, radius + 16, 1);
    }
    for (int type_index = 0; type_index < BuildingTypes.Count(); type_index++) {
        BuildingTypeClass *type = BuildingTypes[type_index];
        char name[60];
        name[0] = 0;
        unknown_ini_8396d0->Get_String(type->ID, "ToTile", 0, name, 60);
        int tile = Unknown_00529400(name);
        if (tile >= 0 && tile < IsoTileTypes.Count()) {
            type->unknown_bf0 = IsoTileTypes[tile];
        }
    }
}


// Cell const *BuildingTypeClass::unknown_45e270() const (0x0045E270)
// The outer-cell list for the foundation (foundation 9 shares the list of
// foundation 6); each list holds 30 cells.

Cell const *BuildingTypeClass::unknown_45e270() const
{
    int foundation = Foundation;
    if (foundation == 9) {
        foundation = 6;
    }
    return unknown_foundation_outside_84f848[foundation];
}


template<>
inline Rect const __fastcall Union<Rect>(Rect const &rect1,Rect const &rect2)
{
    if (rect1.Is_Valid()) {
        if (rect2.Is_Valid()) {
            Rect result = rect1;
            if (result.X > rect2.X) {
                result.Width += result.X - rect2.X;
                result.X = rect2.X;
            }
            if (result.Y > rect2.Y) {
                result.Height += result.Y - rect2.Y;
                result.Y = rect2.Y;
            }
            int rect2_right=rect2.X+rect2.Width;
            int result_right=result.X+result.Width;
            if(result_right<rect2_right) {
                result.Width = ((rect2.X + rect2.Width) - result.X) + 1;
            }
            if (result.Y + result.Height < rect2.Y + rect2.Height) {
                result.Height = ((rect2.Y + rect2.Height) - result.Y) + 1;
            }
            return result;
        }
        return rect1;
    }
    return rect2;
}

Rect BuildingTypeClass::unknown_func_45e290()
{
    int frame;
    if(unknown_11ec!=unknown_static_rect)return unknown_11ec;
    ShapeFileStruct const *image=(ShapeFileStruct const *)Get_Image_Data();
    if(image==0)return unknown_static_rect;
    int frames=image->Get_Count();
    Rect bounds=unknown_static_rect;
    int width=image->Get_Width();
    int height=image->Get_Height();
    for(frame=0;frame<frames;frame++) {
        bounds=Union(bounds,image->Get_Frame_Rect(frame));
    }
    bool free_buildup=BuildupData==0;
    image=(ShapeFileStruct const *)Get_Buildup_Data();
    if(image) {
        frames=image->Get_Count();
        for(frame=0;frame<frames;frame++) {
            bounds=Union(bounds,image->Get_Frame_Rect(frame));
        }
        if(free_buildup)Free_Buildup();
    }
    if(unknown_11cc) {
        Rect bib=((ShapeFileStruct const *)unknown_11cc)->Get_Frame_Rect(0);
        bounds=Union(bounds,bib);
    }
    int x=bounds.X-width/2;
    int y=bounds.Y-height/2;
    int bounds_width=bounds.Width;
    int bounds_height=bounds.Height;
    unknown_11ec=Rect(x,y,bounds_width,bounds_height);
    return unknown_11ec;
}


// bool BuildingTypeClass::CanUseWaypoint() const (0x0045E740)

bool BuildingTypeClass::CanUseWaypoint() const
{
    return false;
}


// bool BuildingTypeClass::CanAttackMove() const (0x0045E750)

bool BuildingTypeClass::CanAttackMove() const
{
    return false;
}


// 0045E760: greatest base power drain in a building-type list; zero for an empty list.
// The original entry has no recorded incoming reference; the parameter's list identity
// follows its +4 vector pointer and +0x10 count. Exact nominal source signature unknown.
int __fastcall BuildingTypes_Maximum_Power_Drain_0045e760(DynamicVectorClass<BuildingTypeClass *> const &types)
{
    int maximum = 0;
    for (int index = 0; index < types.Count(); ++index) {
        int drain = types[index]->PowerDrain_c6c;
        if (drain > maximum) maximum = drain;
    }
    return maximum;
}


// Demand-loaded building buildup image and its animation frame rate.
void *__fastcall Load_Alloc_Data(FileClass &file);
extern "C" void __cdecl _makepath(char *, char const *, char const *, char const *, char const *);
void *BuildingTypeClass::Get_Buildup_Data()
{
    if(BuildupData==0 && DemandLoadBuildup) {
        if(strlen(BuildupFile)!=0) {
            char fullname[260];
            _makepath(fullname,0,0,BuildupFile,".SHP");
            Theater_Naming_Convention(fullname,unknown_id_source_a3d290->Theater);
            CCFileClass file(fullname);
            void *shape=Load_Alloc_Data(file);
            if(shape==0) {
                Generic_Naming_Convention(fullname);
                CCFileClass generic(fullname);
                shape=Load_Alloc_Data(generic);
            }
            BuildupData=shape;
            if(shape!=0) {
                int rate=1;
                BuildupDataAllocated=true;
                int frames=((ShapeFileStruct *)shape)->FrameCount/2;
                if(Gate) frames=(int)((unsigned int)GateStages+1u);
                if(frames>0) rate=(int)(Rules->unknown_1178*900.0/frames);
                AnimFrames[0].unknown_00=0;
                AnimFrames[0].unknown_04=frames;
                AnimFrames[0].unknown_08=rate;
            }
        }
        return BuildupData;
    }
    return BuildupData;
}


// void BuildingTypeClass::Free_Buildup() (0x0045E920)
// Frees the image when FreeBuildup is set.

void BuildingTypeClass::Free_Buildup()
{
    if (FreeBuildup && BuildupData) {
        operator delete(BuildupData);
        BuildupData = 0;
        BuildupDataAllocated = false;
    }
}


// void BuildingTypeClass::Free_unknown_11cc() (0x0045E960)
// Frees the image when unknown_11d0 is set.

void BuildingTypeClass::Free_unknown_11cc()
{
    if (unknown_11d0 && unknown_11cc) {
        operator delete(unknown_11cc);
        unknown_11cc = 0;
        unknown_11d0 = false;
    }
}


// void BuildingTypeClass::Free_unknown_11a0() (0x0045E9A0)
// Frees the image when unknown_11a4 is set.

void BuildingTypeClass::Free_unknown_11a0()
{
    if (unknown_11a4 && unknown_11a0) {
        operator delete(unknown_11a0);
        unknown_11a0 = 0;
        unknown_11a4 = false;
    }
}


// void BuildingTypeClass::Free_unknown_11a8() (0x0045E9E0)
// Frees the image when unknown_11ac is set.

void BuildingTypeClass::Free_unknown_11a8()
{
    if (unknown_11ac && unknown_11a8) {
        operator delete(unknown_11a8);
        unknown_11a8 = 0;
        unknown_11ac = false;
    }
}


// void BuildingTypeClass::Free_unknown_11b0() (0x0045EA20)
// Frees the image when unknown_11b4 is set.

void BuildingTypeClass::Free_unknown_11b0()
{
    if (unknown_11b4 && unknown_11b0) {
        operator delete(unknown_11b0);
        unknown_11b0 = 0;
        unknown_11b4 = false;
    }
}


// void BuildingTypeClass::Free_unknown_11b8() (0x0045EA60)
// Frees the image when unknown_11bc is set.

void BuildingTypeClass::Free_unknown_11b8()
{
    if (unknown_11bc && unknown_11b8) {
        operator delete(unknown_11b8);
        unknown_11b8 = 0;
        unknown_11bc = false;
    }
}


// Derive the three base-defense ratings from the primary weapon. Full292+12.
extern double const Defense_Rate_Scale_0079d2e8;
inline double Limit_Defense_Rating(double amount,double cap)
{
    return cap<amount?cap:amount;
}
void BuildingTypeClass::unknown_func_45eaa0()
{
    if(IsBaseDefense) {
        WeaponTypeClass *weapon=Get_Weapon(0)->WeaponType;
        if(weapon) {
            int rate=(int)((double)weapon->unknown_a4/((double)weapon->unknown_b0*Defense_Rate_Scale_0079d2e8));
            if(weapon->Projectile->AA) {
                unknown_11d8=(int)Limit_Defense_Rating((double)rate*weapon->Warhead->Verses[3],(double)Rules->MaximumBaseDefenseValue);
            }
            if(weapon->Projectile->AG) {
                unknown_11dc=(int)Limit_Defense_Rating((double)rate*weapon->Warhead->Verses[4],(double)Rules->MaximumBaseDefenseValue);
                unknown_11e0=(int)Limit_Defense_Rating((double)rate*weapon->Warhead->Verses[0],(double)Rules->MaximumBaseDefenseValue);
            }
        }
    }
}


// static void __fastcall BuildingTypeClass::unknown_func_45ebd0() (0x0045EBD0)
// Gives each type the isometric tile type its "ToTile" key (in the INI
// pointed to by 0x008396D0) names (unknown_bf0).

class IsometricTileTypeClass;
extern CCINIClass *unknown_ini_8396d0;                       // 0x008396D0
extern DynamicVectorClass<IsometricTileTypeClass *> IsoTileTypes;
int __fastcall Unknown_00529400(char const *name);           // 0x00529400 (IsometricTileTypeClass: the index by ID)

void __fastcall BuildingTypeClass::unknown_func_45ebd0()
{
    for (int index = 0; index < BuildingTypes.Count(); index++) {
        BuildingTypeClass *type = BuildingTypes[index];
        char name[60];
        name[0] = 0;
        unknown_ini_8396d0->Get_String(type->ID, "ToTile", 0, name, 60);
        int tile = Unknown_00529400(name);
        if (tile >= 0 && tile < IsoTileTypes.Count()) {
            type->unknown_bf0 = IsoTileTypes[tile];
        }
    }
}


// bool BuildingTypeClass::Can_Always_Undeploy() const (0x0045EC40)
// TibSun builtype.cpp: IsLimpetMine || IsMobileWar; RA2 dropped both features and the query is false.

bool BuildingTypeClass::Can_Always_Undeploy() const
{
    return false;
}


// bool BuildingTypeClass::unknown_func_45ec50() const (0x0045EC50)
// Sensor array, tick tank, ICBM launcher or artillery. The callers test al
// (UnitClass 0x006FDFC0); the logical expression is materialized as 0/1 in eax.

bool BuildingTypeClass::unknown_func_45ec50() const
{
    return SensorArray || TickTank || ICBMLauncher || Artillary;
}


// int BuildingTypeClass::unknown_func_45ec90() const (0x0045EC90)
// 0 for artillery; 0x40 for sensor arrays, tick tanks and ICBM launchers;
// otherwise 0x80 (flag values; meaning unknown).

int BuildingTypeClass::unknown_func_45ec90() const
{
    if (!SensorArray && !TickTank && !ICBMLauncher && !Artillary) {
        return 0x80;
    }
    if (Artillary) {
        return 0;
    }
    if (SensorArray || TickTank || ICBMLauncher) {
        return 0x40;
    }
    return 0x80;
}


// 0x0045ecf0: function not yet identified
//
// Body: 3 bytes (frozen inventory bound).
// Referenced from 16 sites (data).
// Ghidra prototype: undefined4 __fastcall FUN_0045ecf0(undefined4 param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0045ecf0 {

unsigned int __fastcall FUN_0045ecf0(unsigned int value)
{
    return value;
}

}  // namespace seed_0045ecf0


// int BuildingTypeClass::Kind_Of() const (0x0045ED00)

int BuildingTypeClass::Kind_Of() const
{
    return 7;
}


// int BuildingTypeClass::Size_Of() const (0x0045ED10)

int BuildingTypeClass::Size_Of() const
{
    return sizeof(BuildingTypeClass);
}


// int BuildingTypeClass::Fetch_Heap_ID() const (0x0045ED20)
// The type's index in BuildingTypes.

int BuildingTypeClass::Fetch_Heap_ID() const
{
    return ArrayIndex;
}


// BuildingTypeClass scalar deleting destructor (0x0045ED30)
// Compiler generated after the unit's last member (calls the destructor).


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<TPoint3D<int>>::Fetch (0x0045EDC0; VectorClass<TPoint3D<int>> vtable 0x0079d2c0 slot 6).
// TPoint3D<int> is a size-only stand-in (12 bytes, from the ID(T const *) divisor);
// its real fields are not recovered.


// VectorClass<TPoint3D<int> >::operator== (slot 1) (0x0045EDF0; vtable 0x0079D2C0)
// Instantiated from shared/vector_class.hpp by DockingOffsets.
// @symbol ??8?$VectorClass@U?$TPoint3D@H@@@@UBE_NABV0@@Z


// VectorClass<TPoint3D<int> >::Resize (slot 2) (0x0045EE50; vtable 0x0079D2C0)
// Instantiated from shared/vector_class.hpp by DockingOffsets.
// @symbol ?Resize@?$VectorClass@U?$TPoint3D@H@@@@UAE_NHPBU?$TPoint3D@H@@@Z


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<TPoint3D<int>>::Clear (0x0045EF30; VectorClass<TPoint3D<int>> vtable 0x0079d2c0 slot 3).
// The element type is left incomplete: Clear does not depend on it.


// VectorClass<TPoint3D<int> >::ID(T const &) (slot 4) (0x0045EF60; vtable 0x0079D2C0)
// Instantiated from shared/vector_class.hpp by DockingOffsets.
// @symbol ?ID@?$VectorClass@U?$TPoint3D@H@@@@UAEHABU?$TPoint3D@H@@@Z


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<TPoint3D<int>>::ID_pointer (0x0045EFB0; VectorClass<TPoint3D<int>> vtable 0x0079d2c0 slot 5).
// TPoint3D<int> is a size-only stand-in (12 bytes, from the ID(T const *) divisor);
// its real fields are not recovered.


// VectorClass<TPoint3D<int>> scalar deleting destructor (0x0045F0D0; vtable 0x0079D2C0 slot 0)
// Compiler generated; instantiated from shared/vector_class.hpp by DockingOffsets.
// @symbol ??_G?$VectorClass@U?$TPoint3D@H@@@@UAEPAXI@Z
