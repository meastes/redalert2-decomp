// overtype.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/overtype.json carries the addresses and the rows that
// stay in their own files.


#include "shared/overlay_type.hpp"
#include "shared/overlay.hpp"
#include "shared/palette.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/mix_file_class.hpp"
#include "shared/anim_type.hpp"
#include "shared/shape_file.hpp"


// OverlayTypeClass::OverlayTypeClass(char const *ini_name) (0x005DC5A0)
// Default overlay values, then registration in OverlayTypes.

OverlayTypeClass::OverlayTypeClass(char const *ini_name) :
    ObjectTypeClass(ini_name),
    ArrayIndex(-1),
    Land(0),
    CellAnim(0),
    DamageLevels(1),
    Strength(1),
    Wall(false),
    Tiberium(false),
    Crate(false),
    CrateTrigger(false),
    NoUseTileLandType(true),
    IsVeinholeMonster(false),
    IsVeins(false),
    DemandLoad(false),
    Explodes(false),
    ChainReaction(false),
    Overrides(false),
    DrawFlat(true),
    IsRubble(false),
    IsARock(false)
{
    AbstractClass::Create_ID();
    unknown_1ff = true;
    unknown_200 = false;
    unknown_202 = true;
    unknown_205 = false;
    OverlayTypes.Add(this);
    ArrayIndex = OverlayTypes.ID(this);
}


// OverlayTypeClass::OverlayTypeClass(NoInitClass const &noinit) (0x005DC700)
// Load path: installs the vtables only.

OverlayTypeClass::OverlayTypeClass(NoInitClass const &noinit) :
    ObjectTypeClass(noinit)
{
}


// OverlayTypeClass::~OverlayTypeClass() (0x005DC730)
// Leaves the notice lists and OverlayTypes.

OverlayTypeClass::~OverlayTypeClass()
{
    Unknown_006ECB90((ObjectClass *)(AbstractClass *)this, true);
    OverlayTypes.Delete(this);
}


// static int __fastcall OverlayTypeClass::From_Name(char const *name) (0x005DC7B0)
// Index of the type with this ID (case-insensitive), or -1.

int __fastcall OverlayTypeClass::From_Name(char const *name)
{
    if (name != 0) {
        for (int index = 0; index < OverlayTypes.Count(); index++) {
            if (_stricmp(OverlayTypes[index]->ID, name) == 0) {
                return index;
            }
        }
    }
    return -1;
}


// Cell const *OverlayTypeClass::Occupy_List(int arg1) const (0x005DC800)
// The occupation list: the origin cell, then the 0x7FFF end marker (a
// function-local static registered with atexit on first use).

Cell const *OverlayTypeClass::Occupy_List(int arg1) const
{
    static UnknownStaticCell const unknown_list[] = {Cell(0, 0), Cell(0x7FFF, 0x7FFF)};
    return &unknown_list[0].Cell;
}


// OverlayTypeClass::Occupy_List static exit thunk (0x005DC860)
// Compiler-emitted atexit destructor of the function-local list in
// OverlayTypeClass_unknown_slot36_005dc800.cpp; no source of its own.
// @symbol _$E41


// bool OverlayTypeClass::Create_And_Place(Cell const &cell, HouseClass *house) const (0x005DC870)
// Places an overlay of this type at the cell (no house).

bool OverlayTypeClass::Create_And_Place(Cell const &cell, HouseClass *house) const
{
    if (new OverlayClass((OverlayTypeClass *)this, cell, -1) != 0) {
        return true;
    }
    return false;
}


// ObjectClass *OverlayTypeClass::Create_One_Of(HouseClass *house) const (0x005DC8B0)
// Creates an unplaced overlay of this type (at the zero cell).

ObjectClass *OverlayTypeClass::Create_One_Of(HouseClass *house) const
{
    return new OverlayClass((OverlayTypeClass *)this, unknown_geometry_k, -1);
}


// void OverlayTypeClass::Draw_It(Point2D const *point, Rect const *window, int frame) (0x005DC8E0)
// Draws a frame of the type's image at a point of the tactical area (tactical
// by half a cell) with the drawer at 0x008323B8.

void OverlayTypeClass::Draw_It(Point2D const *point, Rect const *window, int frame)
{
    Point2D tactical(unknown_int_839630, unknown_int_839634);
    Point2D position(point->X + tactical.X, point->Y + tactical.Y);
    position.X += 30;
    position.Y += 15;
    CC_Draw_Shape(unknown_surface_83995c, unknown_drawer_8323b8, (ShapeFileStruct const *)Get_Image_Data(), frame, position, *window,
                  0x600, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
}


// static void __fastcall OverlayTypeClass::Init(int theater) (0x005DC960)
// Loads the images of the overlays that are not demand-loaded for a
// theater (theater-named, else generic), and frees the theater images of
// the demand-loaded ones.

void __fastcall OverlayTypeClass::Init(int theater)
{
    for (int index = 0; index < OverlayTypes.Count(); index++) {
        OverlayTypeClass *overlay = OverlayTypes[index];
        if (!overlay->DemandLoad) {
            char fullname[512];
            if (overlay->unknown_1fc) {
                _makepath(fullname, 0, 0, overlay->unknown_1c8, Theaters[theater].Suffix);
                overlay->unknown_a4 = (void *)MixFileClass::Retrieve(fullname);
                if (overlay->unknown_a4 == 0) {
                    Generic_Naming_Convention(fullname);
                    overlay->unknown_a4 = (void *)MixFileClass::Retrieve(fullname);
                }
            } else if (overlay->unknown_207) {
                _makepath(fullname, 0, 0, overlay->unknown_1c8, ".SHP");
                Theater_Naming_Convention(fullname, theater);
                overlay->unknown_a4 = (void *)MixFileClass::Retrieve(fullname);
                if (overlay->unknown_a4 == 0) {
                    Generic_Naming_Convention(fullname);
                    overlay->unknown_a4 = (void *)MixFileClass::Retrieve(fullname);
                }
            }
        } else if ((overlay->unknown_1fc || overlay->unknown_207) && overlay->unknown_a4 != 0 && overlay->unknown_a8) {
            operator delete(overlay->unknown_a4);
            overlay->unknown_a4 = 0;
            overlay->unknown_a8 = false;
        }
    }
}


// bool OverlayTypeClass::Read_INI(CCINIClass &ini) (0x005DCAA0)
// Reads the overlay keys; "DamageLevels" and "DemandLoad" come from the art
// INI (image section). Tiberium is armor 6 (hypothesis) and defaults to
// land type 5. Images that are not theater-specific or demand-loaded are
// loaded at once.

// An animation type named by the entry, else the default (Westwood's
// CCINIClass::Get_AnimType is not recovered; written as a local reader).
static inline AnimTypeClass *Get_AnimType(CCINIClass &ini, char const *section, char const *entry, AnimTypeClass *defvalue)
{
    char buffer[128];
    if (ini.Get_String(section, entry, unknown_string_83c55c, buffer, sizeof(buffer))) {
        return AnimTypeClass::Find_Or_Make(buffer);
    }
    return defvalue;
}

bool OverlayTypeClass::Read_INI(CCINIClass &ini)
{
    if (ObjectTypeClass::Read_INI(ini)) {
        char const *section = ID;
        Land = ini.Get_LandType(section, "Land", (LandType)Land);
        Strength = ini.Get_Int(section, "Strength", Strength);
        Wall = ini.Get_Bool(section, "Wall", Wall);
        Tiberium = ini.Get_Bool(section, "Tiberium", Tiberium);
        Crate = ini.Get_Bool(section, "Crate", Crate);
        CrateTrigger = ini.Get_Bool(section, "CrateTrigger", CrateTrigger);
        Explodes = ini.Get_Bool(section, "Explodes", Explodes);
        Overrides = ini.Get_Bool(section, "Overrides", Overrides);
        CellAnim = Get_AnimType(ini, section, "CellAnim", CellAnim);
        char const *image = unknown_1c8;
        DamageLevels = ArtINI.Get_Int(image, "DamageLevels", DamageLevels);
        DemandLoad = ArtINI.Get_Bool(image, "DemandLoad", DemandLoad);
        if (Tiberium) {
            unknown_9c = 6;
            if (Land == 0) {
                Land = 5;
            }
        }
        if (!unknown_1fc && !DemandLoad) {
            char fullname[512];
            _makepath(fullname, 0, 0, image, ".SHP");
            unknown_a4 = (void *)MixFileClass::Retrieve(fullname);
        }
        NoUseTileLandType = ini.Get_Bool(section, "NoUseTileLandType", NoUseTileLandType);
        IsVeinholeMonster = ini.Get_Bool(section, "IsVeinholeMonster", IsVeinholeMonster);
        IsVeins = ini.Get_Bool(section, "IsVeins", IsVeins);
        ChainReaction = ini.Get_Bool(section, "ChainReaction", ChainReaction);
        DrawFlat = ini.Get_Bool(section, "DrawFlat", DrawFlat);
        IsARock = ini.Get_Bool(section, "IsARock", IsARock);
        IsRubble = ini.Get_Bool(section, "IsRubble", IsRubble);
        return true;
    }
    return false;
}


// CoordStruct OverlayTypeClass::Coord_Fixup(CoordStruct const &coord) const (0x005DCD50)
// The coordinate unchanged.

CoordStruct OverlayTypeClass::Coord_Fixup(CoordStruct const &coord) const
{
    return coord;
}


// void OverlayTypeClass::Compute_CRC(WWCRCEngine &crc) const (0x005DCD70)
// Note: the base called is AbstractTypeClass's (0x00410960).

void OverlayTypeClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractTypeClass::Compute_CRC(crc);
    crc((long)ArrayIndex);
    crc((long)Land);
    crc((long)DamageLevels);
    crc((long)Strength);
    crc(Wall);
    crc(Tiberium);
    crc(Crate);
    crc(CrateTrigger);
    crc(Explodes);
}


// HRESULT __stdcall OverlayTypeClass::Load(IStream *stream) (0x005DCE10)
// Restores the type in place, reloads its voxel and image, swizzles the
// cell animation and reloads a non-demand-loaded image for the scenario's
// theater.

HRESULT __stdcall OverlayTypeClass::Load(IStream *stream)
{
    HRESULT result = ObjectTypeClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) OverlayTypeClass(NoInitClass());
        Fetch_Voxel_Image();
        Fetch_Normal_Image();
        Swizzler.Swizzle((void **)&CellAnim);
        if (!DemandLoad) {
            char fullname[512];
            if (unknown_1fc) {
                _makepath(fullname, 0, 0, unknown_1c8, Theaters[unknown_id_source_a3d290->Theater].Suffix);
            } else {
                _makepath(fullname, 0, 0, unknown_1c8, ".SHP");
                Theater_Naming_Convention(fullname, unknown_id_source_a3d290->Theater);
            }
            unknown_a4 = (void *)MixFileClass::Retrieve(fullname);
        }
        result = S_OK;
    }
    return result;
}


// HRESULT __stdcall OverlayTypeClass::Save(IStream *stream, BOOL clear_dirty) (0x005DCF10)

HRESULT __stdcall OverlayTypeClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = ObjectTypeClass::Save(stream, clear_dirty);
    if (SUCCEEDED(result)) {
        result = S_OK;
    }
    return result;
}


// HRESULT __stdcall OverlayTypeClass::GetClassID(GUID *clsid) (0x005DCF30)

HRESULT __stdcall OverlayTypeClass::GetClassID(GUID *clsid)
{
    if (clsid == 0) {
        return E_POINTER;
    }
    *clsid = CLSID_OverlayTypeClass;
    return S_OK;
}


// static OverlayTypeClass *__fastcall OverlayTypeClass::Find_Or_Make(char const *name) (0x005DCF70)
// The type with this ID, else a new one; null for "<none>" and "none".

OverlayTypeClass *__fastcall OverlayTypeClass::Find_Or_Make(char const *name)
{
    if (_stricmp("<none>", name) == 0 || _stricmp("none", name) == 0) {
        return 0;
    }
    for (int index = 0; index < OverlayTypes.Count(); index++) {
        if (_stricmp(OverlayTypes[index]->ID, name) == 0) {
            return OverlayTypes[index];
        }
    }
    return new OverlayTypeClass(name);
}


// RGBClass OverlayTypeClass::Get_Radar_Color(unsigned frame) const (0x005DD000)
// The radar colour of a frame of the type's image, or of its cell animation's
// (TibSun overtype.cpp OverlayTypeClass::Get_Radar_Color): tiberium overlays
// (0x7F-0x8A, 0x93-0x9E) swap green and blue; black without an image.

RGBClass OverlayTypeClass::Get_Radar_Color(unsigned frame) const
{
    ShapeFileStruct const *image = (ShapeFileStruct const *)Get_Image_Data();

    if (image == 0 && CellAnim != 0) {
        image = (ShapeFileStruct const *)CellAnim->Get_Image_Data();
    }

    if (image != 0) {
        if ((ArrayIndex >= 0x7F && ArrayIndex <= 0x8A) || (ArrayIndex >= 0x93 && ArrayIndex <= 0x9E)) {
            RGBClass color = image->Get_Color(frame);
            return RGBClass(color.Get_Red(), color.Get_Blue(), color.Get_Green());
        }
        return image->Get_Color(frame);
    }

    return RGBClass(0, 0, 0);
}


// void *OverlayTypeClass::Get_Image_Data() const (0x005DD150)
// The image, demand-loading it from its file (theater-named) on first use.

void *OverlayTypeClass::Get_Image_Data() const
{
    OverlayTypeClass *self = (OverlayTypeClass *)this;
    if (unknown_a4 == 0 && DemandLoad) {
        Debug_Printf("Demand loading image for %s\n", (char const *)Name);
        char fullname[512];
        if (unknown_1fc) {
            _makepath(fullname, 0, 0, unknown_1c8, Theaters[unknown_id_source_a3d290->Theater].Suffix);
        } else if (unknown_207) {
            _makepath(fullname, 0, 0, unknown_1c8, ".SHP");
            Theater_Naming_Convention(fullname, unknown_id_source_a3d290->Theater);
        }
        CCFileClass file(fullname);
        self->unknown_a4 = Load_Alloc_Data(file);
        if (unknown_a4 != 0) {
            self->unknown_a8 = true;
        }
        return unknown_a4;
    }
    return unknown_a4;
}


// int OverlayTypeClass::Kind_Of() const (0x005DD260)
// Compiler-emitted from shared/overlay_type.hpp in this unit; no source of its own.


// int OverlayTypeClass::Size_Of() const (0x005DD270)
// Compiler-emitted from shared/overlay_type.hpp in this unit; no source of its own.


// int OverlayTypeClass::Fetch_Heap_ID() const (0x005DD280)
// Compiler-emitted from shared/overlay_type.hpp in this unit; no source of its own.


// void *OverlayTypeClass::`scalar deleting destructor'(unsigned int) (0x005DD290)
// Compiler-emitted from shared/overlay_type.hpp in this unit; no source of its own.
