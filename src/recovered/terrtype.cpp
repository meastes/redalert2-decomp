// terrtype.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/terrtype.json carries the addresses and the rows that
// stay in their own files.


#include "shared/terrain_type.hpp"
#include "shared/anim_type.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/mix_file_class.hpp"
#include "shared/terrain.hpp"
#include "shared/shape_file.hpp"
#include "shared/rules.hpp"
#include "shared/map_view.hpp"


// The foundation (occupy) lists of the terrain types, one per foundation size, each ended by
// Cell(0x7FFF, 0x7FFF) (TibSun terrtype.cpp's _OccupyLists, BSIZE_COUNT 22 rows of 10 cells; the
// rows after BSIZE_35 are empty). Its dynamic initializer is 0x006E4D60.

Cell unknown_foundation_lists_ac0430[22][10] = {
    { Cell(0, 0), Cell(0x7FFF, 0x7FFF) },
    { Cell(0, 0), Cell(1, 0), Cell(0x7FFF, 0x7FFF) },
    { Cell(0, 0), Cell(0, 1), Cell(0x7FFF, 0x7FFF) },
    { Cell(0, 0), Cell(1, 0), Cell(0, 1), Cell(1, 1), Cell(0x7FFF, 0x7FFF) },
    { Cell(0, 0), Cell(1, 0), Cell(0, 1), Cell(1, 1), Cell(0, 2), Cell(1, 2), Cell(0x7FFF, 0x7FFF) },
    { Cell(0, 0), Cell(1, 0), Cell(2, 0), Cell(0, 1), Cell(1, 1), Cell(2, 1), Cell(0x7FFF, 0x7FFF) },
    { Cell(0, 0), Cell(1, 0), Cell(2, 0), Cell(0, 1), Cell(1, 1), Cell(2, 1), Cell(0, 2), Cell(1, 2), Cell(2, 2), Cell(0x7FFF, 0x7FFF) },
    { Cell(0, 0), Cell(1, 0), Cell(2, 0), Cell(3, 0), Cell(0, 1), Cell(1, 1), Cell(2, 1), Cell(3, 1), Cell(0x7FFF, 0x7FFF) },
};


// TerrainTypeClass::TerrainTypeClass(char const *ini_name) (0x006E5260)
// Default terrain values, registration in TerrainTypes, then the default
// strength (-1: the rules value, see Read_INI) and armor (6; hypothesis).

TerrainTypeClass::TerrainTypeClass(char const *ini_name) :
    ObjectTypeClass(ini_name),
    ArrayIndex(-1),
    Foundation(0),
    TemperateOccupationBits(7),
    SnowOccupationBits(7),
    AnimationRate(0),
    AnimationProbability(0),
    WaterBound(false),
    SpawnsTiberium(false),
    IsFlammable(false),
    IsAnimated(false),
    IsVeinhole(false),
    OccupyList(0)
{
    AbstractClass::Create_ID();
    unknown_200 = false;
    unknown_204 = true;
    unknown_1ff = true;
    unknown_201 = false;
    unknown_202 = true;
    TerrainTypes.Add(this);
    ArrayIndex = TerrainTypes.ID(this);
    MaxStrength = -1;
    unknown_9c = 6;
}


// TerrainTypeClass::TerrainTypeClass(NoInitClass const &noinit) (0x006E53B0)
// Load path: installs the vtables only.

TerrainTypeClass::TerrainTypeClass(NoInitClass const &noinit) :
    ObjectTypeClass(noinit),
    RadarColor(noinit)
{
}


// TerrainTypeClass::~TerrainTypeClass() (0x006E53E0)
// Leaves the notice lists and TerrainTypes.

TerrainTypeClass::~TerrainTypeClass()
{
    Unknown_006ECB90((ObjectClass *)(AbstractClass *)this, true);
    TerrainTypes.Delete(this);
}


// static void __fastcall TerrainTypeClass::Init(int theater) (0x006E5460)
// Reloads the theater images of theater-specific terrain types when the
// theater changes (or a reload is forced), freeing owned images first.
// Assumptions: unknown_1fc is "Theater="; fullname size 512.

void __fastcall TerrainTypeClass::Init(int theater)
{
    if (Debug_Map || theater != unknown_loaded_theater_7d88dc) {
        for (int index = 0; index < TerrainTypes.Count(); index++) {
            TerrainTypeClass *terrain = TerrainTypes[index];
            if (terrain->unknown_1fc) {
                char fullname[512];
                _makepath(fullname, 0, 0, terrain->ID, Theaters[theater].Suffix);
                if (terrain->unknown_a8 && terrain->unknown_a4 != 0) {
                    operator delete(terrain->unknown_a4);
                }
                terrain->unknown_a4 = 0;
                terrain->unknown_a8 = false;
                terrain->unknown_a4 = (void *)MixFileClass::Retrieve(fullname);
            }
        }
    }
}


// static int __fastcall TerrainTypeClass::From_Name(char const *name) (0x006E5510)
// Index of the type with this ID (case-insensitive), or -1.

int __fastcall TerrainTypeClass::From_Name(char const *name)
{
    if (name != 0) {
        for (int index = 0; index < TerrainTypes.Count(); index++) {
            if (_stricmp(name, TerrainTypes[index]->ID) == 0) {
                return index;
            }
        }
    }
    return -1;
}


// bool TerrainTypeClass::Create_And_Place(Cell const &cell, HouseClass *house) const (0x006E5560)
// Creates a terrain object of this type at the cell.

bool TerrainTypeClass::Create_And_Place(Cell const &cell, HouseClass *house) const
{
    if (new TerrainClass((TerrainTypeClass *)this, cell) != 0) {
        return true;
    }
    return false;
}


// ObjectClass *TerrainTypeClass::Create_One_Of(HouseClass *house) const (0x006E55A0)
// Creates an unplaced terrain object of this type (at the zero cell).

ObjectClass *TerrainTypeClass::Create_One_Of(HouseClass *house) const
{
    return new TerrainClass((TerrainTypeClass *)this, unknown_geometry_k);
}


// Cell const *TerrainTypeClass::Occupy_List(int arg1) const (0x006E55D0)
// The foundation's occupation list, else an empty list (only the 0x7FFF
// end marker; a function-local static registered with atexit on first use).

Cell const *TerrainTypeClass::Occupy_List(int arg1) const
{
    if (OccupyList != 0) {
        return OccupyList;
    }
    static UnknownStaticCell const unknown_list[] = {Cell(0x7FFF, 0x7FFF)};
    return &unknown_list[0].Cell;
}


// TerrainTypeClass::Occupy_List static exit thunk (0x006E5620)
// Compiler-emitted atexit destructor of the function-local list in
// TerrainTypeClass_unknown_slot36_006e55d0.cpp; no source of its own.
// @symbol _$E57


// bool TerrainTypeClass::Read_INI(CCINIClass &ini) (0x006E5630)
// Reads the terrain keys; the foundation comes from the art INI (image
// section) and selects the occupation list; the radar colour is the first
// image frame's colour. Veinholes clear unknown_204 and set unknown_201.
// A strength of -1 takes the rules default (+0xE1C).

// The colour stored in a shape frame header (+0x0C; hypothesis: the radar
// colour of the frame).
static inline RGBClass Frame_Color(ShapeFileStruct const *shape, int frame)
{
    ShapeFileFrameStruct const *header = shape->Get_Frame(frame);
    if (header != 0) {
        unsigned char const *color = (unsigned char const *)&header->unknown_0C;
        return RGBClass(color[0], color[1], color[2]);
    }
    return RGBClass();
}

bool TerrainTypeClass::Read_INI(CCINIClass &ini)
{
    if (ObjectTypeClass::Read_INI(ini)) {
        if (MaxStrength == -1) {
            MaxStrength = Rules->unknown_e1c;
        }
        char const *section = ID;
        IsVeinhole = ini.Get_Bool(section, "IsVeinhole", IsVeinhole);
        if (IsVeinhole) {
            unknown_204 = false;
            unknown_201 = true;
        }
        WaterBound = ini.Get_Bool(section, "WaterBound", WaterBound);
        SpawnsTiberium = ini.Get_Bool(section, "SpawnsTiberium", SpawnsTiberium);
        IsFlammable = ini.Get_Bool(section, "IsFlammable", IsFlammable);
        Foundation = ArtINI.Get_Foundation(unknown_1c8, "Foundation", Foundation);
        OccupyList = unknown_foundation_lists_ac0430[Foundation];
        if (!unknown_1fc) {
            char fullname[512];
            _makepath(fullname, 0, 0, unknown_1c8, ".SHP");
            unknown_a4 = (void *)MixFileClass::Retrieve(fullname);
        }
        ShapeFileStruct const *shape = (ShapeFileStruct const *)Get_Image_Data();
        if (shape != 0) {
            RadarColor = Frame_Color(shape, 0);
        }
        IsAnimated = ini.Get_Bool(section, "IsAnimated", IsAnimated);
        AnimationRate = ini.Get_Int(section, "AnimationRate", AnimationRate);
        AnimationProbability = ini.Get_Double(section, "AnimationProbability", AnimationProbability);
        TemperateOccupationBits = ini.Get_Int(section, "TemperateOccupationBits", TemperateOccupationBits);
        SnowOccupationBits = ini.Get_Int(section, "SnowOccupationBits", SnowOccupationBits);
        return true;
    }
    return false;
}


// CoordStruct TerrainTypeClass::Coord_Fixup(CoordStruct const &coord) const (0x006E5850)
// The coordinate, raised to the ground height when below it.

CoordStruct TerrainTypeClass::Coord_Fixup(CoordStruct const &coord) const
{
    CoordStruct result = coord;
    if (result.Z < Map.Get_Height_GL(result)) {
        result.Z = Map.Get_Height_GL(result);
    }
    return result;
}


// void TerrainTypeClass::Compute_CRC(WWCRCEngine &crc) const (0x006E58C0)
// Note: the base called is AbstractTypeClass's (0x00410960).

void TerrainTypeClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractTypeClass::Compute_CRC(crc);
    crc((long)ArrayIndex);
    crc(WaterBound);
    crc(SpawnsTiberium);
    crc(IsFlammable);
    crc((long)Foundation);
    crc(IsAnimated);
    crc((long)AnimationRate);
    crc(AnimationProbability);
}


// HRESULT __stdcall TerrainTypeClass::Load(IStream *stream) (0x006E5950)
// Restores the type in place, reloads its voxel and image and points the
// occupation list at the foundation's list again.

HRESULT __stdcall TerrainTypeClass::Load(IStream *stream)
{
    HRESULT result = ObjectTypeClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) TerrainTypeClass(NoInitClass());
        Fetch_Voxel_Image();
        Fetch_Normal_Image();
        OccupyList = unknown_foundation_lists_ac0430[Foundation];
        result = S_OK;
    }
    return result;
}


// HRESULT __stdcall TerrainTypeClass::Save(IStream *stream, BOOL clear_dirty) (0x006E59C0)

HRESULT __stdcall TerrainTypeClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = ObjectTypeClass::Save(stream, clear_dirty);
    if (SUCCEEDED(result)) {
        result = S_OK;
    }
    return result;
}


// HRESULT __stdcall TerrainTypeClass::GetClassID(GUID *clsid) (0x006E59E0)

HRESULT __stdcall TerrainTypeClass::GetClassID(GUID *clsid)
{
    if (clsid == 0) {
        return E_POINTER;
    }
    *clsid = CLSID_TerrainTypeClass;
    return S_OK;
}


// static TerrainTypeClass *__fastcall TerrainTypeClass::Find_Or_Make(char const *name) (0x006E5A20)
// The type with this ID, else a new one; null for "<none>" and "none".

TerrainTypeClass *__fastcall TerrainTypeClass::Find_Or_Make(char const *name)
{
    if (_stricmp("<none>", name) == 0 || _stricmp("none", name) == 0) {
        return 0;
    }
    for (int index = 0; index < TerrainTypes.Count(); index++) {
        if (_stricmp(TerrainTypes[index]->ID, name) == 0) {
            return TerrainTypes[index];
        }
    }
    return new TerrainTypeClass(name);
}


// int TerrainTypeClass::Kind_Of() const (0x006E5AB0)
// Compiler-emitted from shared/terrain_type.hpp in this unit; no source of its own.


// int TerrainTypeClass::Size_Of() const (0x006E5AC0)
// Compiler-emitted from shared/terrain_type.hpp in this unit; no source of its own.
