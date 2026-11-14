// isotype.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/isotype.json carries the addresses and the rows that
// stay in their own files.

// Dynamic initializer (0x005283D0, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_00528380.cpp; no source of its own.


// Dynamic initializer (0x005283F0, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_00528380.cpp; no source of its own.


// Dynamic initializer (0x00528410, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_00528380.cpp; no source of its own.


// Dynamic initializer (0x00528430, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_00528380.cpp; no source of its own.


// Dynamic initializer (0x00528450, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_00528380.cpp; no source of its own.


// Dynamic initializer (0x00528470, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_00528380.cpp; no source of its own.


// Dynamic initializer (0x005284B0, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_00528380.cpp; no source of its own.


// Dynamic initializer (0x005284E0, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_00528380.cpp; no source of its own.


// Dynamic initializer (0x00528510, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_00528380.cpp; no source of its own.


// Dynamic initializer (0x00528540, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_00528380.cpp; no source of its own.


// Dynamic initializer (0x00528550, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_00528380.cpp; no source of its own.




#define ISOMETRIC_TILE_TYPE_DECLARE_STATICS
// CellClass::Clear_Icon for Load_Tiles (0x0052B2E0; guarded in shared/cell.hpp).
#define CELL_DECLARE_CLEAR_ICON
#define RGB_DECLARE_BRIGHTEN
// The unit implements the vector accessors (its Load 0x0052E1C0 and slot 39 use them): the
// first source of the unit that includes vector_class.hpp defines them.
#include "shared/isometric_tile_type.hpp"
// The isometric tile palette (0x00A6DD10) and its dynamic initializer (0x00528570).
#include "shared/convert.hpp"

PaletteClass IsoTilePalette;


// IsometricTileTypeClass::TileInsertTypes (0x00A52EC8): its initializer (0x00528590) and
// guarded atexit destructor (0x005285D0; a static data member).
DynamicVectorClass<IsometricTileTypeClass::TileInsertType *> IsometricTileTypeClass::TileInsertTypes;


// atexit destructor (0x005285D0) of IsometricTileTypeClass::TileInsertTypes: emitted by its definition.


// A cliff or slope tile's shadow caster: the shadow frame, which image and its pixel offset
// (TibSun ShadowCasterInfo; the constructor's out-of-line copy is 0x0052E660).
struct ShadowCasterInfo {
    int Frame;
    int Index;
    int XOffset;
    int YOffset;

    ShadowCasterInfo(int frame = 0, int index = 0, int x = 0, int y = 0) : Frame(frame), Index(index), XOffset(x), YOffset(y) {}
};

// The cliff tiles' shadow casters (0x00A6E108; initializer 0x00528630).
ShadowCasterInfo ShadowCasterCliffInfo[40] = {
    ShadowCasterInfo(), ShadowCasterInfo(), ShadowCasterInfo(), ShadowCasterInfo(), ShadowCasterInfo(),
    ShadowCasterInfo(), ShadowCasterInfo(), ShadowCasterInfo(), ShadowCasterInfo(), ShadowCasterInfo(),
    ShadowCasterInfo(), ShadowCasterInfo(), ShadowCasterInfo(), ShadowCasterInfo(), ShadowCasterInfo(),
    ShadowCasterInfo(), ShadowCasterInfo(),
    ShadowCasterInfo(0, 0, 30, 30),
    ShadowCasterInfo(0, 0, 30, 30),
    ShadowCasterInfo(0, 0, 30, 30),
    ShadowCasterInfo(1, 0, 30, 30),
    ShadowCasterInfo(1, 0, 30, 30),
    ShadowCasterInfo(2, 1, 60, 15),
    ShadowCasterInfo(3, 1, 60, 15),
    ShadowCasterInfo(4, 1, 60, 15),
    ShadowCasterInfo(5, 0, 90, 30),
    ShadowCasterInfo(6, 0, 60, 15),
    ShadowCasterInfo(7, 1, 30, 0),
    ShadowCasterInfo(8, 0, 60, 15),
    ShadowCasterInfo(9, 0, 60, 15),
    ShadowCasterInfo(10, 1, 0, -15),
    ShadowCasterInfo(11, 1, 0, -15),
    ShadowCasterInfo(12, 0, 60, 15),
    ShadowCasterInfo(), ShadowCasterInfo(), ShadowCasterInfo(), ShadowCasterInfo(), ShadowCasterInfo(),
    ShadowCasterInfo(), ShadowCasterInfo(),
};


// The slope tiles' shadow casters (0x00A6E040; initializer 0x00528DB0).
ShadowCasterInfo ShadowCasterSlopeInfo[10] = {
    ShadowCasterInfo(), ShadowCasterInfo(), ShadowCasterInfo(), ShadowCasterInfo(),
    ShadowCasterInfo(13, 6, 48, 12),
    ShadowCasterInfo(),
    ShadowCasterInfo(14, 1, 48, 12),
    ShadowCasterInfo(), ShadowCasterInfo(), ShadowCasterInfo(),
};


// IsometricTileTypeClass::IsometricTileTypeClass(int, int, unsigned char, char const *, bool) (0x00528EE0)
#include "shared/object_lists.hpp"

IsometricTileTypeClass::IsometricTileTypeClass(int type, int unknown1, unsigned char unknown2, char const *ini_name, bool skip_registration) :
    ObjectTypeClass(ini_name),
    ArrayIndex(type),
    MarbleMadness(0xFFFF),
    NonMarbleMadness(0xFFFF),
    TileSetBaseID(0),
    unknown_274(),
    unknown_28c(0),
    ToSnowTheater(-1),
    ToTemperateTheater(-1),
    unknown_298(-1),
    Offset(Point2D(0, 0)),
    AttachesTo(-1),
    ZAdjust(0),
    Unused1((unsigned char)unknown1),
    unknown_2b0(false),
    IsShadowCaster(false),
    IsAllowToPlace(true),
    IsRequiredForRMG(false),
    unknown_2b4(0),
    unknown_2b8(0),
    Unused2(unknown2),
    NumTileTypesInSet(1),
    unknown_2c4(false),
    unknown_2d5(true),
    unknown_2d6(false),
    UseCount(0)
{
    unknown_objects_ac0cb0.Add(this);
    if (!skip_registration) {
        IsoTileTypes.Add(this);
    }

    Name = FixedString<49>(ini_name);

    Filename[0] = '\0';

    unknown_1ff = true;
    unknown_200 = false;
    unknown_201 = false;
    unknown_202 = true;
    unknown_203 = true;
    unknown_205 = false;
}


// IsometricTileTypeClass::IsometricTileTypeClass(NoInitClass const &) (0x00529120): a type about
// to be filled in from a save game (Load expands it in place).

IsometricTileTypeClass::IsometricTileTypeClass(NoInitClass const &noinit) : ObjectTypeClass(noinit)
{
}


// IsometricTileTypeClass::~IsometricTileTypeClass() (0x00529190)
// Detaches from everything, deletes the owned object, leaves IsoTileTypes and the object list, and frees the colour tables.
#include "shared/vector_class.hpp"
#include "shared/isometric_tile.hpp"
#include "shared/crt_memory.hpp"
void __fastcall Detach_This_From_All(AbstractClass *object, bool all);

IsometricTileTypeClass::~IsometricTileTypeClass()
{
    Detach_This_From_All(this, true);
    if (unknown_28c != 0) {
        AbstractClass *owned = unknown_28c;
        unknown_28c = 0;
        delete owned;
    }
    IsoTileTypes.Delete(this);
    unknown_objects_ac0cb0.Delete(this);
    for (int i = unknown_274.Count() - 1; i >= 0; i--) {
        if (unknown_274[i] != 0) {
            delete unknown_274[i];
        }
        unknown_274.Delete(i);
    }
}


// int IsometricTileTypeClass::Land_Type(int tile) -- 0x00529300
// The land type of sub tile `tile` (TibSun Land_Type): the image's cell record for the tile,
// its index modulo the image's cell count, selects an entry of the land table at 0x007DE4EC
// by the record's signed byte at +0x29; 0 without image or record.

extern int unknown_table_7de4ec[];

int IsometricTileTypeClass::Land_Type(int tile)
{
    IsoTileImageStruct const *image = (IsoTileImageStruct const *)Get_Image_Data();
    if (image != 0) {
        IsoTileCellStruct const *record = (IsoTileCellStruct const *)image->Tiles[tile % (image->unknown_00[1] * image->unknown_00[0])];
        if (record != 0) {
            return unknown_table_7de4ec[record->unknown_29];
        }
    }
    return 0;
}


// bool IsometricTileTypeClass::Is_Tile_Index_Valid(int tile, bool load) (0x00529340)

bool IsometricTileTypeClass::Is_Tile_Index_Valid(int tile, bool load)
{
    if (load) {
        Load_Tile_Image();
    }
    IsoTileImageStruct const *image = (IsoTileImageStruct const *)Get_Image_Data();
    if (image != 0 && tile < image->unknown_00[0] * image->unknown_00[1] && image->Tiles[tile] != 0) {
        return true;
    }
    return false;
}


// bool IsometricTileTypeClass::Load_Tile_Image() (0x005293A0)

bool IsometricTileTypeClass::Load_Tile_Image()
{
    if (unknown_a4 == 0 && unknown_2c4) {
        Load_Tile_Data();
    }
    return unknown_a4 != 0;
}


// void *IsometricTileTypeClass::Get_Image_Data() const (0x005293D0)
// The image, loaded on first use when the type demand-loads it (+0x2C4).
// The unit implements the vector accessors (as its Load, 0x0052E1C0): the
// first source of the unit that includes vector_class.hpp defines them.

void *IsometricTileTypeClass::Get_Image_Data() const
{
    void *image = unknown_a4;
    if (image == 0 && unknown_2c4) {
        ((IsometricTileTypeClass *)this)->Load_Tile_Data();
        image = unknown_a4;
    }
    return image;
}


// int IsometricTileTypeClass::From_Name(char const *name) (0x00529400)

int __fastcall IsometricTileTypeClass::From_Name(char const *name)
{
    if (name != 0) {
        for (int index = 0; index < IsoTileTypes.Count(); index++) {
            if (_stricmp(IsoTileTypes[index]->ID, name) == 0) {
                return index;
            }
        }
    }
    return 0xFFFF;
}


// Cell const *IsometricTileTypeClass::Occupy_List(int arg1) const (0x00529450)
// The occupation list: every cell of the image's grid that has a tile, then
// the 0x7FFF end marker (a function-local static list, not initialized).

Cell const *IsometricTileTypeClass::Occupy_List(int arg1) const
{
    static UnknownStaticCell list[129];
    IsoTileImageStruct const *image = (IsoTileImageStruct const *)Get_Image_Data();
    int const *tile = image->Tiles;
    UnknownStaticCell *cell = list;
    for (int index = 0; index < unknown_2b4 * unknown_2b8; index++) {
        if (*tile++ != 0) {
            cell->Cell = Cell(index % unknown_2b4, index / unknown_2b4);
            cell++;
        }
    }
    cell->Cell = Cell(0x7FFF, 0x7FFF);
    return &list[0].Cell;
}


// IsometricTileTypeClass::Occupy_List static exit thunk (0x00529510)
// Compiler-emitted atexit destructor of the function-local list in
// IsometricTileTypeClass_unknown_slot36_00529450.cpp; no source of its own.
// @symbol _$E58


// IsometricTileTypeClass *IsometricTileTypeClass::Next_Tile_From_Set(int index) (0x00529520)

IsometricTileTypeClass *IsometricTileTypeClass::Next_Tile_From_Set(int index)
{
    int num = NumTileTypesInSet;
    int i = index;
    if (i > num - 1) {
        i = i % num;
    }
    if (i == 0) {
        return this;
    }
    IsometricTileTypeClass *next = this;
    do {
        next = (IsometricTileTypeClass *)next->unknown_28c;
    } while (--i != 0);
    return next;
}


// int IsometricTileTypeClass::Fixup_Tile_Type(int type) (0x00529550)

int __fastcall IsometricTileTypeClass::Fixup_Tile_Type(int type)
{
    int new_type = type;
    if (type == 0xFFFF) {
        return new_type;
    }
    for (int i = 0; i < TileInsertTypes.Count(); i++) {
        TileInsertType *tileinsert = TileInsertTypes[i];
        if (tileinsert->Count > type) {
            break;
        }
        new_type += tileinsert->Offset;
    }
    return new_type;
}


// LightConvertClass *IsometricTileTypeClass::Find_Or_Make_Drawer(int red, int green, int blue) (0x00529590)
// The tile drawer already made for these tints, else a new one added to the list (the first
// drawer, at normal light, gets the blitters).
#include "shared/unknown_globals.hpp"

LightConvertClass *__fastcall IsometricTileTypeClass::Find_Or_Make_Drawer(int red, int green, int blue)
{
    LightConvertClass *drawer;

    if (unknown_surface_839950 == 0) {
        return 0;
    }

    if (red == 1000 && green == 1000 && blue == 1000 && unknown_LightConvertClass_list_00832398.Count() != 0) {
        return unknown_LightConvertClass_list_00832398[0];
    }

    Clamp_Tile_RGB(red, green, blue);

    for (int i = 1; i < unknown_LightConvertClass_list_00832398.Count(); i++) {
        drawer = unknown_LightConvertClass_list_00832398[i];
        if (drawer->Red == red && drawer->Green == green && drawer->Blue == blue) {
            return drawer;
        }
    }

    int shadows = 53;
    if (red + green + blue < 2000) {
        shadows = 27;
    }
    drawer = new LightConvertClass(IsoTilePalette, unknown_palette_837e10, unknown_surface_839950, red, green, blue,
        unknown_LightConvertClass_list_00832398.Count() != 0, 0, shadows);
    unknown_LightConvertClass_list_00832398.Add(drawer);

    return drawer;
}


// bool __fastcall Unknown_00529710(int unknown1, bool unknown2) (0x00529710)
// Frees nothing and reports nothing left to free in this build (TibSun
// IsometricTileTypeClass::Free_Unused_Drawers; LightSourceClass 0x00537830 and
// MapClass slot 23 call it).
#include "shared/map.hpp"

bool __fastcall Unknown_00529710(int unknown1, bool unknown2)
{
    return false;
}


// void IsometricTileTypeClass::Init_Drawers() (0x00529720): frees every tile drawer, makes the
// normal-light one again, and makes the dummy cell and every map cell pick theirs anew.
#include "shared/cell.hpp"
#include "shared/map_view.hpp"

void IsometricTileTypeClass::Init_Drawers()
{
    for (int i = 0; i < unknown_LightConvertClass_list_00832398.Count(); i++) {
        LightConvertClass *drawer = unknown_LightConvertClass_list_00832398[i];
        delete drawer;
        unknown_LightConvertClass_list_00832398.Delete(i);
        i--;
    }

    unknown_LightConvertClass_list_00832398.Add(new LightConvertClass(IsoTilePalette, unknown_palette_837e10,
        unknown_surface_839950, 1000, 1000, 1000, false, 0, 53));

    unknown_dummy_cell_a6f9f8.unknown_34 = 0;
    unknown_dummy_cell_a6f9f8.Init_Drawer(0, 0x10000, 0, 1000, 1000, 1000);

    Map.Reset_Iterator();
    CellClass *cell = Map.Iterate();
    while (cell != 0) {
        cell->unknown_34 = 0;
        cell = Map.Iterate();
    }
}


// TibSun IsometricTileTypeClass::Read_Control_File: a free __fastcall here. RA2 has no ice remap tail.
#include "shared/cd_timer.hpp"
#include "shared/system_timer.hpp"
#include "shared/tileset_index.hpp"
#include "shared/ccini_class.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/mix_file_class.hpp"
#include "shared/anim_type.hpp"
#include "shared/theater_type.hpp"
#include "shared/crt_string.hpp"
extern int unknown_tileset_a6e0e8;
extern int unknown_tileset_a6cf4c;
extern int unknown_tileset_a52f84;
extern int unknown_tileset_a52ef8;
extern int unknown_tileset_a5258c;
extern int unknown_tileset_a6e0fc;
extern int unknown_tileset_a52e6c;
extern int unknown_tileset_a52f04;
extern int unknown_tileset_a6cb78;
extern int unknown_tileset_a52ea4;
extern int unknown_tileset_a6cf50;
extern int unknown_tileset_a52c64;
extern int unknown_tileset_a6dd04;
extern int unknown_tileset_a6cf54;
extern int unknown_tileset_a52ee0;
extern int unknown_tileset_a6e0e4;
extern int unknown_tileset_a52f80;
extern int unknown_tileset_a53394;
extern int unknown_tileset_a52588;
extern void *SlopeZShapes[4];
extern int unknown_tile_set_count_a6e390;
extern int unknown_tile_sets_a52f90[];

void __fastcall Unknown_00529870(int theater, bool from_ccfile)
{
    int i;
    int j;
    char suffix[10];

    CCINIClass ini;
    IsometricTileTypeClass *tile = 0;
    CDTimerClass<SystemTimerClass> callback_timer;
    int shadow_casters = 0;

    memset(IsometricTileTypeClass::ShadowCasterTiles, 0, sizeof(IsometricTileTypeClass::ShadowCasterTiles));

    CCFileClass cshadow("C_SHADOW.SHP");
    if (unknown_block_a6e38c) {
        delete[] (char *)unknown_block_a6e38c;
        unknown_block_a6e38c = 0;
    }
    unknown_block_a6e38c = new char[cshadow.Size()];
    cshadow.Read(unknown_block_a6e38c, cshadow.Size());

    if (IsometricTileTypeClass::TileInsertTypes.Count() != 0) {
        for (i = IsometricTileTypeClass::TileInsertTypes.Count() - 1; i >= 0; --i) {
            delete IsometricTileTypeClass::TileInsertTypes[i];
            IsometricTileTypeClass::TileInsertTypes.Delete(i);
        }
    }

    for (i = 0; i < 4; i++) {
        if (SlopeZShapes[i]) {
            delete[] (char *)SlopeZShapes[i];
            SlopeZShapes[i] = 0;
        }
    }

    char slopzname[32];
    sprintf(slopzname, "SLOP01Z.%s", Theaters[theater].Suffix);
    CCFileClass slopz(slopzname);

    SlopeZShapes[0] = new char[slopz.Size()];
    slopz.Read(SlopeZShapes[0], slopz.Size());

    slopzname[5] = '2';
    slopz.Set_Name(slopzname);
    SlopeZShapes[1] = new char[slopz.Size()];
    slopz.Read(SlopeZShapes[1], slopz.Size());

    slopzname[5] = '3';
    slopz.Set_Name(slopzname);
    SlopeZShapes[2] = new char[slopz.Size()];
    slopz.Read(SlopeZShapes[2], slopz.Size());

    slopzname[5] = '4';
    slopz.Set_Name(slopzname);
    SlopeZShapes[3] = new char[slopz.Size()];
    slopz.Read(SlopeZShapes[3], slopz.Size());

    while (IsoTileTypes.Count() != 0) {
        delete IsoTileTypes[0];
    }

    char palname[32];
    sprintf(palname, "ISO%s.PAL", Theaters[theater].Suffix);
    CCFileClass palette(palname);
    if (palette.Is_Available()) {
        palette.Read(&IsoTilePalette, sizeof(IsoTilePalette));
        unsigned char *color = (unsigned char *)&IsoTilePalette;
        for (i = 0; i < sizeof(PaletteClass); i++) {
            *color <<= 2;
            color++;
        }
    }
    IsometricTileTypeClass::Init_Drawers();

    char ininame[20];
    sprintf(ininame, "%s.INI", Theaters[theater].Root);
    CCFileClass inifile(ininame);
    ini.Load(inifile, false, false);

    int setid = 0;
    int heapid = 0;
    int tile_total = 0;

    int _RampBase = ini.Get_Int("General", "RampBase", -1);
    int _RampSmooth = ini.Get_Int("General", "RampSmooth", -1);
    int _MMRampBase = ini.Get_Int("General", "MMRampBase", -1);
    int _ClearTile = ini.Get_Int("General", "ClearTile", -1);
    int _RoughTile = ini.Get_Int("General", "RoughTile", -1);
    int _SandTile = ini.Get_Int("General", "SandTile", -1);
    int _GreenTile = ini.Get_Int("General", "GreenTile", -1);
    int _PaveTile = ini.Get_Int("General", "PaveTile", -1);
    int _MiscPaveTile = ini.Get_Int("General", "MiscPaveTile", -1);
    int _ClearToRoughLat = ini.Get_Int("General", "ClearToRoughLat", -1);
    int _ClearToSandLat = ini.Get_Int("General", "ClearToSandLat", -1);
    int _ClearToGreenLat = ini.Get_Int("General", "ClearToGreenLat", -1);
    int _ClearToPaveLat = ini.Get_Int("General", "ClearToPaveLat", -1);
    int _HeightBase = ini.Get_Int("General", "HeightBase", -1);
    int _BlackTile = ini.Get_Int("General", "BlackTile", -1);
    int _BridgeSet = ini.Get_Int("General", "BridgeSet", -1);
    int _WoodBridgeSet = ini.Get_Int("General", "WoodBridgeSet", -1);
    int _CliffSet = ini.Get_Int("General", "CliffSet", -1);
    int _ShorePieces = ini.Get_Int("General", "ShorePieces", -1);
    int _WaterSet = ini.Get_Int("General", "WaterSet", -1);
    int _SlopeSetPieces = ini.Get_Int("General", "SlopeSetPieces", -1);
    int _SlopeSetPieces2 = ini.Get_Int("General", "SlopeSetPieces2", -1);
    int _MonorailSlopes = ini.Get_Int("General", "MonorailSlopes", -1);
    int _Tunnels = ini.Get_Int("General", "Tunnels", -1);
    int _TrackTunnels = ini.Get_Int("General", "TrackTunnels", -1);
    int _DirtTunnels = ini.Get_Int("General", "DirtTunnels", -1);
    int _DirtTrackTunnels = ini.Get_Int("General", "DirtTrackTunnels", -1);
    int _WaterfallEast = ini.Get_Int("General", "WaterfallEast", -1);
    int _WaterfallWest = ini.Get_Int("General", "WaterfallWest", -1);
    int _WaterfallNorth = ini.Get_Int("General", "WaterfallNorth", -1);
    int _WaterfallSouth = ini.Get_Int("General", "WaterfallSouth", -1);
    int _CliffRamps = ini.Get_Int("General", "CliffRamps", -1);
    int _PavedRoads = ini.Get_Int("General", "PavedRoads", -1);
    int _PavedRoadEnds = ini.Get_Int("General", "PavedRoadEnds", -1);
    int _Medians = ini.Get_Int("General", "Medians", -1);
    int _RoughGround = ini.Get_Int("General", "RoughGround", -1);
    int _DirtRoadJunction = ini.Get_Int("General", "DirtRoadJunction", -1);
    int _DirtRoadCurve = ini.Get_Int("General", "DirtRoadCurve", -1);
    int _DirtRoadStraight = ini.Get_Int("General", "DirtRoadStraight", -1);
    int _DestroyableCliffs = ini.Get_Int("General", "DestroyableCliffs", -2);
    int _WaterCaves = ini.Get_Int("General", "WaterCaves", -1);
    int _WaterCliffs = ini.Get_Int("General", "WaterCliffs", -1);
    int _PavedRoadSlopes = ini.Get_Int("General", "PavedRoadSlopes", -1);
    int _DirtRoadSlopes = ini.Get_Int("General", "DirtRoadSlopes", -1);
    int _Rocks = ini.Get_Int("General", "Rocks", -1);
    int _WaterBridge = ini.Get_Int("General", "WaterBridge", -1);
    unknown_tileset_a6e018 = -1;
    unknown_tileset_a52ea8 = -1;
    unknown_tileset_a52eec = -1;
    unknown_tileset_a52f00 = -1;
    unknown_tileset_a6e0e8 = -1;
    unknown_tileset_a6cf4c = -1;
    unknown_tileset_a52c60 = -1;
    unknown_tileset_a6e0e0 = -1;
    unknown_tileset_a52ef4 = -1;
    unknown_tileset_a52f84 = -1;
    unknown_tileset_a52c70 = -1;
    unknown_tileset_a52590 = -1;
    unknown_tileset_a52ef8 = -1;
    unknown_tileset_a5258c = -1;
    unknown_tileset_a6e0fc = -1;
    unknown_tileset_a52c74 = -1;
    unknown_tileset_a6cb68 = -1;
    unknown_tileset_a52e68 = -1;
    unknown_tileset_a6cb74 = -1;
    unknown_tileset_a52580 = -1;
    unknown_tileset_a6e038 = -1;
    unknown_tileset_a52ee8 = -1;
    unknown_tileset_a52e6c = -1;
    unknown_tileset_a52ea4 = -1;
    unknown_tileset_a6cf50 = -1;
    unknown_tileset_a52f04 = -1;
    unknown_tileset_a6cb78 = -1;
    unknown_tileset_a52584 = -1;
    unknown_tileset_a6cf58 = -1;
    unknown_tileset_a52ef0 = -1;
    unknown_tileset_a52ea0 = -1;
    unknown_tileset_a6dd00 = -1;
    unknown_tileset_a6dd0c = -1;
    unknown_tileset_a6dd08 = -1;
    unknown_tileset_a52c68 = -1;
    unknown_tileset_a52c64 = -1;
    unknown_tileset_a6cb6c = -1;
    unknown_tileset_a6e014 = -1;
    unknown_tileset_a52efc = -1;
    unknown_tileset_a6e0f8 = -2;
    unknown_tileset_a6cb70 = -1;
    unknown_tileset_a52e64 = -1;
    unknown_tileset_a52ee4 = -1;
    unknown_tileset_a6dd04 = -1;
    unknown_tileset_a6cf54 = -1;
    unknown_tileset_a52ee0 = -1;
    unknown_tileset_a6e0e4 = ini.Get_Int("General", "BridgeTopLeft1", -1);
    unknown_tileset_a52f80 = ini.Get_Int("General", "BridgeTopLeft2", -1);
    unknown_tileset_a6e028 = ini.Get_Int("General", "BridgeBottomRight1", -1);
    unknown_tileset_a52c80 = ini.Get_Int("General", "BridgeBottomRight2", -1);
    unknown_tileset_a53394 = ini.Get_Int("General", "BridgeTopRight1", -1);
    unknown_tileset_a52588 = ini.Get_Int("General", "BridgeTopRight2", -1);
    unknown_tileset_a6e010 = ini.Get_Int("General", "BridgeBottomLeft1", -1);
    unknown_tileset_a53390 = ini.Get_Int("General", "BridgeBottomLeft2", -1);
    unknown_tileset_a6cb7c = ini.Get_Int("General", "BridgeMiddle1", -1);
    unknown_tileset_a52e70 = ini.Get_Int("General", "BridgeMiddle2", -1);

    unknown_tile_set_count_a6e390 = 0;
    {
        CDTimerClass<SystemTimerClass> fresh(4);
        callback_timer = fresh;
    }

    while (true) {
        if (callback_timer == 0) {
            Service_Game_IO_00481fb0();
            CDTimerClass<SystemTimerClass> fresh(4);
            callback_timer = fresh;
        }

        unknown_tile_sets_a52f90[unknown_tile_set_count_a6e390++] = heapid;

        if (setid == _RampBase) unknown_tileset_a6e018 = heapid;
        if (setid == _RampSmooth) unknown_tileset_a52ea8 = heapid;
        if (setid == _MMRampBase) unknown_tileset_a52eec = heapid;
        if (setid == _ClearTile) unknown_tileset_a52f00 = heapid;
        if (setid == _RoughTile) unknown_tileset_a6e0e8 = heapid;
        if (setid == _SandTile) unknown_tileset_a6cf4c = heapid;
        if (setid == _GreenTile) unknown_tileset_a52c60 = heapid;
        if (setid == _PaveTile) unknown_tileset_a6e0e0 = heapid;
        if (setid == _MiscPaveTile) unknown_tileset_a52ef4 = heapid;
        if (setid == _ClearToRoughLat) unknown_tileset_a52f84 = heapid;
        if (setid == _ClearToSandLat) unknown_tileset_a52c70 = heapid;
        if (setid == _ClearToGreenLat) unknown_tileset_a52590 = heapid;
        if (setid == _ClearToPaveLat) unknown_tileset_a52ef8 = heapid;
        if (setid == _HeightBase) unknown_tileset_a5258c = heapid;
        if (setid == _BlackTile) unknown_tileset_a6e0fc = heapid;
        if (setid == _BridgeSet) unknown_tileset_a52c74 = heapid;
        if (setid == _WoodBridgeSet) unknown_tileset_a6cb68 = heapid;
        if (setid == _CliffSet) unknown_tileset_a52e68 = heapid;
        if (setid == _ShorePieces) unknown_tileset_a6cb74 = heapid;
        if (setid == _WaterSet) unknown_tileset_a52580 = heapid;
        if (setid == _SlopeSetPieces) unknown_tileset_a6e038 = heapid;
        if (setid == _SlopeSetPieces2) unknown_tileset_a52ee8 = heapid;
        if (setid == _MonorailSlopes) unknown_tileset_a52e6c = heapid;
        if (setid == _DirtTunnels) unknown_tileset_a52f04 = heapid;
        if (setid == _DirtTrackTunnels) unknown_tileset_a6cb78 = heapid;
        if (setid == _Tunnels) unknown_tileset_a52ea4 = heapid;
        if (setid == _TrackTunnels) unknown_tileset_a6cf50 = heapid;
        if (setid == _WaterfallEast) unknown_tileset_a52584 = heapid;
        if (setid == _WaterfallWest) unknown_tileset_a6cf58 = heapid;
        if (setid == _WaterfallNorth) unknown_tileset_a52ef0 = heapid;
        if (setid == _WaterfallSouth) unknown_tileset_a52ea0 = heapid;
        if (setid == _CliffRamps) unknown_tileset_a6dd00 = heapid;
        if (setid == _PavedRoads) unknown_tileset_a6dd0c = heapid;
        if (setid == _PavedRoadEnds) unknown_tileset_a6dd08 = heapid;
        if (setid == _Medians) unknown_tileset_a52c68 = heapid;
        if (setid == _RoughGround) unknown_tileset_a52c64 = heapid;
        if (setid == _DirtRoadJunction) unknown_tileset_a6cb6c = heapid;
        if (setid == _DirtRoadCurve) unknown_tileset_a6e014 = heapid;
        if (setid == _DirtRoadStraight) unknown_tileset_a52efc = heapid;
        if (setid == _DestroyableCliffs) unknown_tileset_a6e0f8 = heapid;
        if (setid == _WaterCliffs) unknown_tileset_a52e64 = heapid;
        if (setid == _WaterCaves) unknown_tileset_a6cb70 = heapid;
        if (setid == _PavedRoadSlopes) unknown_tileset_a52ee4 = heapid;
        if (setid == _DirtRoadSlopes) unknown_tileset_a6dd04 = heapid;
        if (setid == _Rocks) unknown_tileset_a6cf54 = heapid;
        if (setid == _WaterBridge) unknown_tileset_a52ee0 = heapid;

        char section[64];
        sprintf(section, "TileSet%04d", setid);
        int tiles_in_set = ini.Get_Int(section, "TilesInSet", -1);
        if (tiles_in_set == -1) {
            break;
        }

        int last_tiles_in_set = ini.Get_Int(section, "LastTilesInSet", -1);
        if (last_tiles_in_set == -1 || last_tiles_in_set == tiles_in_set) {
            tile_total += tiles_in_set;
        } else {
            IsometricTileTypeClass::TileInsertType *insert = new IsometricTileTypeClass::TileInsertType;
            insert->Count = tile_total + last_tiles_in_set;
            insert->Offset = tiles_in_set - last_tiles_in_set;
            IsometricTileTypeClass::TileInsertTypes.Add(insert);
            tile_total += last_tiles_in_set;
        }

        char set_name[64];
        char file_name[64];
        ini.Get_String(section, "SetName", "No Name", set_name, sizeof(set_name));
        ini.Get_String(section, "FileName", "TILE", file_name, sizeof(file_name));
        int _MarbleMadness = ini.Get_Int(section, "MarbleMadness", 0xFFFF);
        int _NonMarbleMadness = ini.Get_Int(section, "NonMarbleMadness", 0xFFFF);
        bool _Morphable = ini.Get_Bool(section, "Morphable", false);
        bool _AllowToPlace = ini.Get_Bool(section, "AllowToPlace", true);
        bool _AllowBurrowing = ini.Get_Bool(section, "AllowBurrowing", true);
        bool _AllowTiberium = ini.Get_Bool(section, "AllowTiberium", false);
        bool _RequiredForRMG = ini.Get_Bool(section, "RequiredForRMG", false);
        int _ToSnowTheater = ini.Get_Int(section, "ToSnowTheater", -1);
        int _ToTemperateTheater = ini.Get_Int(section, "ToTemperateTheater", -1);

        bool is_shadow_caster = ini.Get_Bool(section, "ShadowCaster", false);
        if (is_shadow_caster) {
            IsometricTileTypeClass::ShadowCasterTiles[shadow_casters++] = heapid;
        }
        int shadow_tiles = 0;
        if (is_shadow_caster) {
            shadow_tiles = ini.Get_Int(section, "ShadowTiles", shadow_tiles);
        }

        bool has_section = ini.Find_Section(set_name) != 0;

        for (i = 0; i < tiles_in_set; i++) {
            int tile_count = 0;

            IsometricTileTypeClass *head_tile = 0;
            IsometricTileTypeClass *tail_tile = 0;

            IsoTileImageStruct *mixfile_set = 0;
            bool found_image = false;

            do {
                char letter_suffix[2];
                char tile_name[128];
                char given_name[128];

                if (tile_count != 0) {
                    letter_suffix[0] = tile_count + 'a' - 1;
                    letter_suffix[1] = 0;
                } else {
                    letter_suffix[0] = 0;
                }

                sprintf(suffix, "%02d", i + 1);
                strcat(suffix, letter_suffix);
                strcpy(tile_name, file_name);
                strcat(tile_name, suffix);
                sprintf(given_name, "%.28s %02d", set_name, i + 1);

                int uninitialized_value = -9;

                if (tile_count == 0) {
                    tile = new IsometricTileTypeClass(heapid++, uninitialized_value, 0, tile_name, false);

                    tile->Name = FixedString<49>(given_name);
                    tile->MarbleMadness = _MarbleMadness;
                    tile->NonMarbleMadness = _NonMarbleMadness;
                    tile->unknown_2b0 = _Morphable;
                    tile->IsAllowToPlace = _AllowToPlace;
                    tile->unknown_2d5 = _AllowBurrowing;
                    tile->unknown_2d6 = _AllowTiberium;
                    tile->IsRequiredForRMG = _RequiredForRMG;
                    tile->unknown_a4 = 0;
                    tile->unknown_a8 = false;
                    tile->ToSnowTheater = _ToSnowTheater;
                    tile->ToTemperateTheater = _ToTemperateTheater;
                    if (is_shadow_caster && shadow_tiles) {
                        tile->IsShadowCaster = is_shadow_caster;
                    }
                    if (from_ccfile) {
                        tile->unknown_2c4 = true;
                    }
                    tile->TileSetBaseID = unknown_tile_sets_a52f90[unknown_tile_set_count_a6e390 - 1];
                    tile->unknown_274.Clear();
                    if (has_section) {
                        char entry[32];
                        sprintf(entry, "Tile%02dAnim", i + 1);

                        char anim_name[128];
                        if (ini.Get_String(set_name, entry, "", anim_name, sizeof(anim_name))) {
                            AnimTypeClass *animtype = AnimTypeClass::Find_Or_Make(anim_name);
                            if (animtype != 0) {
                                tile->unknown_298 = animtype->Fetch_Heap_ID();
                                sprintf(entry, "Tile%02dXOffset", i + 1);
                                tile->Offset.X = ini.Get_Int(set_name, entry, tile->Offset.X);
                                sprintf(entry, "Tile%02dYOffset", i + 1);
                                tile->Offset.Y = ini.Get_Int(set_name, entry, tile->Offset.Y);
                                sprintf(entry, "Tile%02dAttachesTo", i + 1);
                                tile->AttachesTo = ini.Get_Int(set_name, entry, tile->AttachesTo);
                                sprintf(entry, "Tile%02dZAdjust", i + 1);
                                tile->ZAdjust = ini.Get_Int(set_name, entry, tile->ZAdjust);
                            }
                        }
                    }
                    head_tile = tile;
                    tail_tile = tile;
                }

                char file_path[512];
                _makepath(file_path, 0, 0, tile_name, Theaters[theater].Suffix);

                mixfile_set = 0;
                found_image = false;

                if (from_ccfile) {
                    CCFileClass file(file_path);
                    found_image = file.Is_Available();
                    if (found_image && tile_count == 0) {
                        strncpy(tile->Filename, file_path, sizeof(tile->Filename) - 2);
                    }
                } else {
                    mixfile_set = (IsoTileImageStruct *)MixFileClass::Retrieve(file_path);
                    if (mixfile_set) {
                        found_image = true;
                    }
                }
                if (!found_image && _NonMarbleMadness) {
                    if (theater != -1) {
                        _makepath(file_path, 0, 0, tile_name, Theaters[theater].MMSuffix);
                    } else {
                        _makepath(file_path, 0, 0, tile_name, ".MMT");
                    }
                    if (from_ccfile) {
                        CCFileClass file(file_path);
                        found_image = file.Is_Available();
                        if (found_image && tile_count == 0) {
                            strncpy(tile->Filename, file_path, sizeof(tile->Filename) - 2);
                        }
                    } else {
                        mixfile_set = (IsoTileImageStruct *)MixFileClass::Retrieve(file_path);
                        if (mixfile_set) {
                            found_image = true;
                        }
                    }
                }

                if (tile_count != 0) {
                    if (!mixfile_set && !found_image) {
                        break;
                    }
                    tile = new IsometricTileTypeClass(heapid - 1, uninitialized_value, 0, tile_name, true);

                    tile->Name = FixedString<49>(given_name);
                    tile->MarbleMadness = _MarbleMadness;
                    tile->NonMarbleMadness = _NonMarbleMadness;
                    tile->unknown_2b0 = _Morphable;
                    tile->IsAllowToPlace = _AllowToPlace;
                    tile->unknown_2d5 = _AllowBurrowing;
                    tile->unknown_2d6 = _AllowTiberium;
                    tile->IsRequiredForRMG = _RequiredForRMG;
                    if (from_ccfile) {
                        tile->unknown_2c4 = true;
                        strncpy(tile->Filename, file_path, sizeof(tile->Filename) - 2);
                    }
                    if (is_shadow_caster && shadow_tiles) {
                        tile->IsShadowCaster = is_shadow_caster;
                    }
                    tile->TileSetBaseID = unknown_tile_sets_a52f90[unknown_tile_set_count_a6e390 - 1];
                    tail_tile->unknown_28c = tile;
                    tile->unknown_274.Clear();
                    tail_tile = tile;
                }

                if (tile_count == 0 || mixfile_set) {
                    tile->unknown_a4 = mixfile_set;
                }

                if (mixfile_set != 0) {
                    tile->unknown_2b4 = (unsigned char)mixfile_set->unknown_00[0];
                    tile->unknown_2b8 = (unsigned char)mixfile_set->unknown_00[1];
                    for (j = 0; j < mixfile_set->unknown_00[0] * mixfile_set->unknown_00[1]; j++) {
                        void **record = (void **)&mixfile_set->Tiles[j];
                        if (*record != 0 && (unsigned int)*record < (unsigned int)mixfile_set) {
                            *record = (void *)((unsigned int)mixfile_set + (unsigned int)*record);
                        }
                    }
                    tile->Build_Preview_Tiles();
                } else {
                    if (tile_count == 0) {
                        tile->unknown_2b4 = 0;
                        tile->unknown_2b8 = 0;
                    }
                }
                tile_count++;
            } while (mixfile_set != 0 || found_image);

            if (tile_count > 1) {
                IsometricTileTypeClass *tile = head_tile;
                for (char t = tile_count; t > 1; t--) {
                    tile->NumTileTypesInSet = t;
                    tile = (IsometricTileTypeClass *)tile->unknown_28c;
                }
            }
        }
        setid++;
    }

    for (j = 0; j < IsoTileTypes.Count(); ++j) {
        IsometricTileTypeClass *isotype = IsoTileTypes[j];
        int offset = isotype->ArrayIndex - isotype->TileSetBaseID;
        if (isotype->MarbleMadness != 0xFFFF) {
            isotype->MarbleMadness = unknown_tile_sets_a52f90[isotype->MarbleMadness] + offset;
        }
        if (isotype->NonMarbleMadness != 0xFFFF) {
            isotype->NonMarbleMadness = unknown_tile_sets_a52f90[isotype->NonMarbleMadness] + offset;
        }
    }
}


// void IsometricTileTypeClass::Load_Tiles(bool skip_iteration, bool is_random) (0x0052B2E0)
#include "shared/debug_printf.hpp"

void __fastcall IsometricTileTypeClass::Load_Tiles(bool skip_iteration, bool is_random)
{
    CDTimerClass<SystemTimerClass> timer = 0;
    IsometricTileTypeClass *ptr;

    if (!skip_iteration) {
        Map.Reset_Iterator();
        CellClass *cptr = Map.Iterate();
        while (cptr != 0) {
            int type = cptr->IsoTileIndex;
            int index = 0;
            int num;
            if (type != 0xFFFF && type < IsoTileTypes.Count()) {
                ptr = IsoTileTypes[type];
                if (ptr->NumTileTypesInSet > 1) {
                    num = ptr->NumTileTypesInSet;
                    if (ptr->Is_Randomized((unsigned char)cptr->unknown_11a)) {
                        index = (cptr->unknown_140 >> 13) & 1;
                    } else {
                        num = ptr->NumTileTypesInSet;
                        index = cptr->Clear_Icon(cptr->IsoTileIndex, num);
                    }
                }
            } else {
                ptr = IsoTileTypes[unknown_tileset_a52f00];
                num = ptr->NumTileTypesInSet;
                index = cptr->Clear_Icon(unknown_tileset_a52f00, num);
            }
            ptr = ptr->Next_Tile_From_Set(index);
            ptr->UseCount++;
            cptr = Map.Iterate();
        }
    }

    int tile_count = 0;
    int bytes = 0;
    timer = 1;
    for (int i = 0; i < IsoTileTypes.Count(); i++) {
        if (!timer) {
            Service_Game_IO_00481fb0();
            timer = 4;
        }
        ptr = IsoTileTypes[i];
        while (ptr != 0) {
            if (ptr->unknown_2c4 && strlen(ptr->Filename)) {
                if (!skip_iteration && ptr->UseCount == 0 && (!is_random || !ptr->IsRequiredForRMG)) {
                    if (ptr->unknown_a4 != 0) {
                        delete [] (unsigned char *)ptr->unknown_a4;
                        ptr->unknown_a4 = 0;
                        ptr->unknown_a8 = false;
                    }
                } else {
                    if (ptr->unknown_a4 == 0) {
                        bytes += ptr->Load_Tile_Data();
                        tile_count++;
                    }
                }
            }
            ptr = (IsometricTileTypeClass *)ptr->unknown_28c;
        }
    }
    Debug_Printf("Loaded %d isometric tiles consuming %d Kb\n", tile_count, bytes / 1024);
}


// int IsometricTileTypeClass::Load_Tile_Data() (0x0052B560): reads the image file, turns its
// cell offsets into pointers and rebuilds the preview colours; returns the file's size
// (TibSun Load_Tile_Data).

int IsometricTileTypeClass::Load_Tile_Data()
{
    CCFileClass file(Filename);
    int size = file.Size();
    if (unknown_a4 != 0) {
        delete [] (unsigned char *)unknown_a4;
    }
    unsigned char *data = new unsigned char[size];
    unknown_a4 = data;
    file.Read(data, size);
    unknown_a8 = true;

    IsoTileImageStruct *image = (IsoTileImageStruct *)unknown_a4;
    unknown_2b4 = (unsigned char)image->unknown_00[0];
    unknown_2b8 = (unsigned char)image->unknown_00[1];

    for (int i = 0; i < unknown_2b4 * unknown_2b8; i++) {
        if (image->Tiles[i] != 0) {
            unsigned char *ptr = (unsigned char *)unknown_a4;
            if ((unsigned char *)image->Tiles[i] < ptr) {
                image->Tiles[i] = (int)(ptr + image->Tiles[i]);
            }
        }
    }
    Build_Preview_Tiles();
    return size;
}


// void IsometricTileTypeClass::Clear_Use_Counts() (0x0052B650)

void IsometricTileTypeClass::Clear_Use_Counts()
{
    IsometricTileTypeClass *type;
    for (int i = 0; i < IsoTileTypes.Count(); i++) {
        for (type = IsoTileTypes[i]; type != 0; type = (IsometricTileTypeClass *)type->unknown_28c) {
            type->UseCount = 0;
        }
    }
}


// bool IsometricTileTypeClass::Get_Tile_Pixel_Dimensions(int, int &, int &) (0x0052B690)

bool IsometricTileTypeClass::Get_Tile_Pixel_Dimensions(int tile, int &width, int &height)
{
    IsoTileImageStruct const *image = (IsoTileImageStruct const *)Get_Image_Data();
    if (image != 0) {
        IsoTileCellStruct const *record = (IsoTileCellStruct const *)image->Tiles[tile % (image->unknown_00[0] * image->unknown_00[1])];
        width = image->unknown_00[2];
        height = image->unknown_00[3];
        if (record != 0) {
            if (record->IsHasExtraData) {
                height += record->Y - record->ExtraY;
            }
        }
        return true;
    }
    return false;
}


// int IsometricTileTypeClass::Ramp_Type(int index) (0x0052B6F0)
// The +0x2A byte of sub tile index's cell record in the loaded image (0 without
// an image or a record; hypothesis). Shaped like Is_Randomized 0x0052B730.

int IsometricTileTypeClass::Ramp_Type(int index)
{
    IsoTileImageStruct const *image = (IsoTileImageStruct const *)Get_Image_Data();
    if (image != 0) {
        IsoTileCellStruct const *record = (IsoTileCellStruct const *)image->Tiles[index % (image->unknown_00[0] * image->unknown_00[1])];
        if (record != 0) {
            return record->unknown_2a;
        }
    }
    return 0;
}


// bool IsometricTileTypeClass::Is_Randomized(int tile) const (0x0052B730)

bool IsometricTileTypeClass::Is_Randomized(int tile) const
{
    IsoTileImageStruct const *image = (IsoTileImageStruct const *)Get_Image_Data();
    if (image != 0) {
        IsoTileCellStruct const *record = (IsoTileCellStruct const *)image->Tiles[tile % (image->unknown_00[0] * image->unknown_00[1])];
        if (record != 0) {
            return record->IsRandomized;
        }
    }
    return 0;
}


// void IsometricTileTypeClass::Draw_Shadow_Caster(int index, Surface *surface, Point2D point, Rect cliprect,
// int offset) (0x0052B770)
#include "shared/palette.hpp"

extern Rect unknown_rect_839630;

void IsometricTileTypeClass::Draw_Shadow_Caster(int index, Surface *surface, Point2D point, Rect cliprect, int offset)
{
    int x;
    int y;
    int frame;
    int caster_index;
    int tiletype = ArrayIndex;
    bool flat = false;
    int pieces;

    if (tiletype >= unknown_tileset_a6e038 && tiletype < unknown_tileset_a6e038 + 10) {
        pieces = unknown_tileset_a6e038;
        flat = true;
    } else {
        if (tiletype >= unknown_tileset_a52ee8 && tiletype < unknown_tileset_a52ee8 + 10) {
            pieces = unknown_tileset_a52ee8;
            flat = true;
        }
    }

    bool can_draw = false;
    if (flat && tiletype - pieces != -1) {
        caster_index = tiletype - pieces;
        if (index == ShadowCasterSlopeInfo[caster_index].Index) {
            frame = ShadowCasterSlopeInfo[caster_index].Frame;
            if (frame != 0) {
                x = 30 + ShadowCasterSlopeInfo[caster_index].XOffset;
                y = 15 + ShadowCasterSlopeInfo[caster_index].YOffset;
                can_draw = true;
            }
        }
    } else if (IsShadowCaster) {
        int i;
        for (i = 0; i < 5; i++) {
            caster_index = tiletype - ShadowCasterTiles[i];
            if (caster_index >= 0 && caster_index < 40) {
                break;
            }
            if (i == 4) {
                return;
            }
        }
        if (index == ShadowCasterCliffInfo[caster_index].Index) {
            frame = ShadowCasterCliffInfo[caster_index].Frame;
            if (frame != 0) {
                x = 30 + ShadowCasterCliffInfo[caster_index].XOffset;
                y = 15 + ShadowCasterCliffInfo[caster_index].YOffset;
                can_draw = true;
            }
        }
    }

    if (can_draw) {
        point.X += x;
        point.Y += y;
        point.X += unknown_rect_839630.X;
        point.Y += unknown_rect_839630.Y;
        point.X -= cliprect.X;
        point.Y -= cliprect.Y;
        CC_Draw_Shape(surface, unknown_LightConvertClass_list_00832398[0], (ShapeFileStruct const *)unknown_block_a6e38c,
            frame - 1, point, cliprect, 0x4601, 0, offset, 0, 1000, 0, 0, Point2D(0, 0));
    }
}


// Cell const *IsometricTileTypeClass::Shadow_Caster_List() const (0x0052B8B0)
// The cells a cliff tile's shadow darkens, relative to the tile, for each of
// the 40 tiles of a shadow casting cliff set (ShadowCasterTiles), ending in
// the 0x7FFF end marker; 0 for a tile that casts no shadow (TibSun
// Shadow_Caster_List; the cells redraw with the cliff).
#include "shared/unknown_geometry.hpp"

Cell const *IsometricTileTypeClass::Shadow_Caster_List() const
{
    if (IsShadowCaster) {
        static UnknownStaticCell const _list[40][6] = {
            { unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, Cell(0x7FFF, 0x7FFF) },
            { unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, Cell(0x7FFF, 0x7FFF) },
            { unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, Cell(0x7FFF, 0x7FFF) },
            { unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, Cell(0x7FFF, 0x7FFF) },
            { unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, Cell(0x7FFF, 0x7FFF) },
            { unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, Cell(0x7FFF, 0x7FFF) },
            { unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, Cell(0x7FFF, 0x7FFF) },
            { unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, Cell(0x7FFF, 0x7FFF) },
            { unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, Cell(0x7FFF, 0x7FFF) },
            { unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, Cell(0x7FFF, 0x7FFF) },
            { unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, Cell(0x7FFF, 0x7FFF) },
            { unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, Cell(0x7FFF, 0x7FFF) },
            { unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, Cell(0x7FFF, 0x7FFF) },
            { unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, Cell(0x7FFF, 0x7FFF) },
            { unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, Cell(0x7FFF, 0x7FFF) },
            { unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, Cell(0x7FFF, 0x7FFF) },
            { unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, Cell(0x7FFF, 0x7FFF) },
            { unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, Cell(0x7FFF, 0x7FFF) },
            { unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, Cell(0x7FFF, 0x7FFF) },
            { unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, Cell(0x7FFF, 0x7FFF) },
            { Cell(0, -1), Cell(0, -1), Cell(-1, -2), unknown_geometry_k, unknown_geometry_k, Cell(0x7FFF, 0x7FFF) },
            { Cell(0, -1), Cell(0, -1), Cell(-1, -2), unknown_geometry_k, unknown_geometry_k, Cell(0x7FFF, 0x7FFF) },
            { Cell(-1, -3), Cell(0, -3), unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, Cell(0x7FFF, 0x7FFF) },
            { Cell(-1, -3), Cell(0, -3), unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, Cell(0x7FFF, 0x7FFF) },
            { Cell(-1, -3), Cell(0, -3), unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, Cell(0x7FFF, 0x7FFF) },
            { Cell(0, -3), unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, Cell(0x7FFF, 0x7FFF) },
            { Cell(0, -3), Cell(1, -3), Cell(-1, -3), Cell(-2, -3), unknown_geometry_k, Cell(0x7FFF, 0x7FFF) },
            { Cell(0, -3), Cell(-1, -3), Cell(-2, -3), Cell(-3, -2), unknown_geometry_k, Cell(0x7FFF, 0x7FFF) },
            { Cell(0, -3), Cell(-1, -3), Cell(-2, -3), unknown_geometry_k, unknown_geometry_k, Cell(0x7FFF, 0x7FFF) },
            { Cell(0, -3), Cell(-1, -3), Cell(-2, -3), unknown_geometry_k, unknown_geometry_k, Cell(0x7FFF, 0x7FFF) },
            { Cell(-2, -3), unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, Cell(0x7FFF, 0x7FFF) },
            { Cell(-2, -3), unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, Cell(0x7FFF, 0x7FFF) },
            { Cell(0, -3), unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, Cell(0x7FFF, 0x7FFF) },
            { unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, Cell(0x7FFF, 0x7FFF) },
            { unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, Cell(0x7FFF, 0x7FFF) },
            { unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, Cell(0x7FFF, 0x7FFF) },
            { unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, Cell(0x7FFF, 0x7FFF) },
            { unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, Cell(0x7FFF, 0x7FFF) },
            { unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, Cell(0x7FFF, 0x7FFF) },
            { unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, unknown_geometry_k, Cell(0x7FFF, 0x7FFF) },
        };

        for (int i = 0; i < 5; i++) {
            int index = ArrayIndex - ShadowCasterTiles[i];
            if (index >= 0 && index < 40) {
                return &_list[index][0].Cell;
            }
        }
    }
    return 0;
}


// IsometricTileTypeClass::Shadow_Caster_List static exit thunk (0x0052C220)
// Compiler-emitted atexit destructor of the function-local list in
// IsometricTileTypeClass_Shadow_Caster_List_0052b8b0.cpp; no source of its own.
// @symbol _$E71


// Not exact (RA2_NON_MATCHING): IsometricTileTypeClass::Draw_Tile at 0x0052C230, best score 58.99, 2026-11-13; differs: same instruction count (1807), shape 96.13: instruction choice and registers differ
#ifdef RA2_NON_MATCHING
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

// Draw_Tile body shape from TibSun isotype.cpp (Draw_Tile, line 1665), rewritten to the RA2 target: global
// cursor loops, 60x30 tiles, tail-call variation walk, HalfbrightMask as the IsoBlitState member at +0x68 (TibSun; Scout1152).
// void IsometricTileTypeClass::Draw_Tile(LightConvertClass *drawer, int subtile, Surface &surface, int x, int y,
//     Rect cliprect, int height, int brightness, bool use_z, int cell_variation, bool fill, bool depth_only, bool fog,
//     int fog_color) const (0x0052C230)
// Draws one sub tile of the tile set: picks the requested variation, builds the iso-diamond span tables once,
// clips the 60x29 diamond and blits it lit (alpha buffer) and depth tested (depth buffer), or stamps a fill,
// depth-only or fog pass; then the cliff's extra image (TibSun Draw_Tile; RA2 60x30 tiles, and a tile without
// the sub tile record returns at once). 0x00839C6C is TibSun's DepthBuffer here, 0x008315B4 its AlphaBuffer.
#include "shared/surface.hpp"
#include "shared/blit.hpp"
#include "shared/alpha_table_cache.hpp"
// The tactical area (0x00839630; shared/display.hpp).
extern Rect unknown_rect_839630;
#include "shared/rules.hpp"
#include "shared/always.hpp"

#ifndef NULL
#define NULL 0
#endif
#define ISO_DRAW_WIDTH 60
#define ISO_DRAW_HEIGHT 29
#define ISO_HEIGHT 30
#ifndef MAX
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef LOWORD
#define LOWORD(l) ((unsigned short)(l))
#endif

#pragma pack(4)
struct IsoTileRecord {
    int X;
    int Y;
    int ExtraOffset;
    int ZDataOffset;
    int ExtraZOffset;
    int ExtraX;
    int ExtraY;
    int ExtraWidth;
    int ExtraHeight;
    unsigned int IsHasExtraData:1;
    unsigned int IsHasZData:1;
    unsigned int IsRandomized:1;
    unsigned char Height;
    signed char TileType;
    signed char RampType;
    unsigned char LowColor[3];
    unsigned char HighColor[3];
};
#pragma pack()

struct IsoTileSet {
    IsoTileRecord const * Fetch_Record_Pointer(int index) const
    {
        return(Tiles[index % Tile_Count()]);
    }
    int Tile_Count() const { return MapWidth * MapHeight; }
    int MapWidth;
    int MapHeight;
    int unknown_08;
    int Height;
    IsoTileRecord * Tiles[1];
};

struct IsoBlitState {
    int SrcPixel;
    int SrcDepth;
    unsigned short *PixelTranslate;
    unsigned short *DepthPtr;
    unsigned short *AlphaPtr;
    unsigned short *DestPtr;
    unsigned short *LightRemap;
    unsigned short *RowSrcOffset;
    unsigned char *RowStartCol;
    unsigned char *RowRunLength;
    unsigned char *ImageBase;
    unsigned char *DepthBase;
    unsigned short *RowDest;
    unsigned short *RowDepth;
    unsigned short *RowAlpha;
    signed int SpanWidth;
    signed int SpanHeight;
    unsigned int ClipLeft;
    unsigned int ClipTop;
    unsigned int BaseDepth;
    unsigned int DepthWidth;
    unsigned int AlphaWidth;
    unsigned int SurfacePitch;
    unsigned int ImageRowStep;
    unsigned int FillDepth;
    unsigned int FogColor;
    unsigned short HalfbrightMask;
};


extern IsoBlitState IsoDrawData;                                         // 0x00A52F08
extern unsigned short _iso_row_offsets[ISO_DRAW_WIDTH*ISO_DRAW_HEIGHT];  // 0x00A6CF68
extern unsigned char _iso_start_cols[ISO_DRAW_WIDTH*ISO_DRAW_HEIGHT];    // 0x00A52594
extern unsigned char _iso_run_lengths[ISO_DRAW_WIDTH][ISO_DRAW_WIDTH*ISO_DRAW_HEIGHT];  // 0x00A53398
extern char IsoSpanTablesBuilt;                                          // 0x00A6E395
extern char IsoTileUseAsmDrawFunc;                                       // 0x00A6E394
extern "C" void __cdecl Iso_Blit_Asm2(IsoBlitState *state);             // 0x0077F3DF

// The alpha-lighting row for a brightness (0..2000 -> 0..254).
static inline int Light_Level(int brightness)
{
    int level = ((brightness > 0 ? brightness : 0) * 261) >> 11;
    if (level > 254) {
        level = 254;
    }
    return level;
}

void IsometricTileTypeClass::Draw_Tile(LightConvertClass * drawer, int subtile, Surface & surface, int x, int y, Rect cliprect, int height, int brightness, bool use_z, int cell_variation, bool fill, bool depth_only, bool fog, signed int fog_color) const
{
    static int const _iso_row_bases[ISO_HEIGHT] = {
        0, 4, 12, 24, 40, 60, 84, 112, 144, 180, 220, 264, 312, 364, 420, 480, 536, 588, 636, 680, 720, 756, 788, 816, 840, 860, 876, 888, 896, 900
    };
    #define __ "\x20"
    #define XX "\xDB"
    static const unsigned char _tilemask[] = {
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __
        __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __
        __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __
        __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __
        XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX
        __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __
        __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __
        __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __
        __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
    };
    #undef __
    #undef XX
    IsometricTileTypeClass const * tileptr = this;
    if (drawer != NULL) {
        /*
         * If a variation was requested, walk the tile-set list to it and draw that
         * tile instead of this one (a tail call: VC6 turns it into a jump).
         */
        IsoDrawData.HalfbrightMask = unknown_half_mask_852fd0;
        if (cell_variation != 0) {
            int num = tileptr->NumTileTypesInSet;
            if (cell_variation > num - 1) {
                cell_variation %= num;
            }
            if (cell_variation != 0) {
                IsometricTileTypeClass const * next = tileptr;
                for (; cell_variation != 0; --cell_variation) {
                    next = (IsometricTileTypeClass const *)next->unknown_28c;
                }
                if (next != tileptr) {
                    next->Draw_Tile(drawer, subtile, surface, x, y, cliprect, height, brightness, use_z, 0, fill, depth_only, fog, fog_color);
                    return;
                }
            }
        }
        Point2D work(x, y);
        bool clipped_out = false;
        AlphaTableRows aremap = AlphaTableCache_008315B8.Get(drawer->unknown_shadow_count);
        IsoDrawData.LightRemap = (unsigned short *)aremap[Light_Level(brightness)];
        IsoDrawData.PixelTranslate = (unsigned short *)drawer->AlphaPalette;
        /*
         * One-time build of the iso-diamond span tables. For each of the ISO_DRAW_HEIGHT
         * scanlines of the ISO_DRAW_WIDTH-wide tile diamond record the left column of the
         * solid span (_iso_start_cols), the per-pixel source offset (_iso_row_offsets)
         * and, for every possible right-edge clip, the run length (_iso_run_lengths).
         * Driven by the diamond bitmap in _tilemask.
         */
        if (!IsoSpanTablesBuilt) {
            int cell_offset = 0;
            const int * lut = _iso_row_bases;
            int loop_pos = 0;
            do {
                int span_count;
                /*
                 * Find the first non-space column of this mask row and the
                 * length of the solid span that follows it.
                 */
                int first_col;
                for (first_col = 0; first_col < ISO_DRAW_WIDTH; ++first_col) {
                    if (_tilemask[cell_offset + first_col] != 32) {
                        span_count = 1;
                        while (first_col + span_count < ISO_DRAW_WIDTH && _tilemask[cell_offset + first_col + span_count] != 32) {
                            ++span_count;
                        }
                        break;
                    }
                }
                if (first_col >= ISO_DRAW_WIDTH) {
                    first_col = 0;
                    span_count = 0;
                }
                {
                    int col = 0;
                    unsigned short * shortp = (unsigned short *)&((char *)_iso_row_offsets)[loop_pos];
                    int base = *lut;
                    int startcol = first_col;
                    do {
                        _iso_start_cols[cell_offset + col] = startcol < 0 ? 0 : startcol;
                        *shortp++ = (unsigned short)(base + max(0, col - first_col));
                        ++col;
                        --startcol;
                    } while (col < ISO_DRAW_WIDTH);
                }
                {
                    unsigned char * runp = &_iso_run_lengths[ISO_DRAW_WIDTH-1][cell_offset];
                    int left_clip = span_count + first_col - ISO_DRAW_WIDTH;
                    for (int rowcount = ISO_DRAW_WIDTH; rowcount != 0; --rowcount) {
                        for (int i = 0; i < ISO_DRAW_WIDTH; ++i) {
                            int run = span_count - max(0, i - first_col) - max(0, left_clip);
                            runp[i] = max(0, run);
                        }
                        runp -= (ISO_DRAW_WIDTH*ISO_DRAW_HEIGHT);
                        ++left_clip;
                    }
                }
                cell_offset += ISO_DRAW_WIDTH;
                loop_pos += (ISO_DRAW_WIDTH * 2);
                ++lut;
            } while (loop_pos < ((ISO_DRAW_WIDTH*ISO_DRAW_HEIGHT) *2));
            IsoSpanTablesBuilt = 1;
        }
        if (surface.Bytes_Per_Pixel() == 2) {
            const IsoTileSet * set = (const IsoTileSet *)tileptr->Get_Image_Data();
            if (set != NULL) {
                if (use_z) {
                    IsoDrawData.BaseDepth = (unsigned short)(LOWORD(DepthBuffer->MaxValue) + LOWORD(DepthBuffer->Area.Y) - y - LOWORD(set->Height));
                    IsoDrawData.BaseDepth = (int)&((unsigned char *)IsoDrawData.BaseDepth)[height * set->Height / -2];
                }
                const IsoTileRecord * record = set->Fetch_Record_Pointer(subtile);
                if (record == NULL) {
                    return;
                }
                int spanw = ISO_DRAW_WIDTH;
                int spanh = ISO_DRAW_HEIGHT;
                int clip_y = 0;
                IsoDrawData.SpanWidth = spanw;
                IsoDrawData.SpanHeight = spanh;
                IsoDrawData.ClipLeft = 0;
                IsoDrawData.ClipTop = clip_y;
                IsoDrawData.RowSrcOffset = _iso_row_offsets;
                IsoDrawData.RowStartCol = (unsigned char *)_iso_start_cols;
                IsoDrawData.RowRunLength = (unsigned char *)_iso_run_lengths;
                /*
                 * Clip the ISO_DRAW_WIDTHxISO_DRAW_HEIGHT tile diamond against the clip rectangle, adjusting
                 * the span dimensions and the left/top source offsets. If the tile
                 * clips away entirely, skip straight to the extra-image pass.
                 */
                if (x < cliprect.X) {
                    spanw = x - cliprect.X + IsoDrawData.SpanWidth;
                    IsoDrawData.ClipLeft = cliprect.X - x;
                    IsoDrawData.SpanWidth = spanw;
                    work.X = cliprect.X;
                    if (spanw <= 0) {
                        clipped_out = true;
                    }
                }
                if (y < cliprect.Y) {
                    spanh = y - cliprect.Y + IsoDrawData.SpanHeight;
                    clip_y = cliprect.Y - y;
                    IsoDrawData.ClipTop = clip_y;
                    IsoDrawData.SpanHeight = spanh;
                    work.Y = cliprect.Y;
                    if (spanh <= 0) {
                        clipped_out = true;
                    }
                }
                int right_clip = ISO_DRAW_WIDTH-1;
                int lock_x = work.X;
                int clip_right = cliprect.Width + cliprect.X;
                if (work.X + spanw > clip_right) {
                    int overhang = spanw - cliprect.Width - cliprect.X + work.X;
                    spanw -= overhang;
                    right_clip -= overhang;
                    IsoDrawData.SpanWidth = spanw;
                    if (spanw <= 0) {
                        clipped_out = true;
                    }
                }
                int clip_bottom = cliprect.Y + cliprect.Height;
                if (IsoDrawData.SpanHeight + work.Y > clip_bottom) {
                    IsoDrawData.SpanHeight = cliprect.Height - work.Y + cliprect.Y;
                    if (IsoDrawData.SpanHeight <= 0) {
                        clipped_out = true;
                    }
                }
                bool skip_extra = false;
                if (!clipped_out) {
                    {
                        int clip_x = IsoDrawData.ClipLeft;
                        if (clip_x > ISO_DRAW_WIDTH-1) {
                            clip_x = ISO_DRAW_WIDTH-1;
                            IsoDrawData.ClipLeft = clip_x;
                        }
                        if (clip_y > ISO_DRAW_HEIGHT-1) {
                            clip_y = ISO_DRAW_HEIGHT-1;
                            IsoDrawData.ClipTop = clip_y;
                        }
                        int table_off = clip_x + ISO_DRAW_WIDTH * clip_y;
                        IsoDrawData.RowSrcOffset = &_iso_row_offsets[table_off];
                        IsoDrawData.RowStartCol = (unsigned char *)&_iso_start_cols[table_off];
                        IsoDrawData.RowRunLength = (unsigned char *)&_iso_run_lengths[MAX(0, right_clip)][table_off];
                    }
                    IsoDrawData.SurfacePitch = surface.Stride();
                    IsoDrawData.DestPtr = (unsigned short *)surface.Lock(Point2D(lock_x, work.Y));
                    if (IsoDrawData.DestPtr == NULL) {
                        skip_extra = true;
                    } else {
                        if (!record->IsHasZData) {
                            use_z = false;
                        } else if (use_z) {
                            IsoDrawData.DepthPtr = DepthBuffer->Get_Buffer_Offset(Point2D(lock_x, work.Y - unknown_rect_839630.Y));
                            IsoDrawData.DepthWidth = DepthBuffer->Width;
                        }
                        IsoDrawData.AlphaPtr = AlphaBuffer->Get_Buffer_Offset(Point2D(lock_x, work.Y - unknown_rect_839630.Y));
                        IsoDrawData.AlphaWidth = AlphaBuffer->Width;
                        IsoDrawData.SrcPixel = (int)(record + 1);
                        unsigned char const * image = (unsigned char const *)IsoDrawData.SrcPixel;
                        if (fill || fog) {
                            IsoDrawData.FillDepth = 0;
                        } else {
                            IsoDrawData.FillDepth = 0xFFFF;
                        }
                        IsoDrawData.FogColor = IsoDrawData.HalfbrightMask & (fog_color >> 1);
                        if (use_z && record->IsHasZData) {
                            IsoDrawData.SrcDepth = (int)record + record->ZDataOffset;
                        }
                        IsoDrawData.ImageBase = (unsigned char *)(record + 1);
                        IsoDrawData.DepthBase = (unsigned char *)IsoDrawData.SrcDepth;
                        if (use_z) {
                            if ((unsigned int)&IsoDrawData.DepthPtr[IsoDrawData.SpanWidth + 2 + IsoDrawData.SpanHeight * DepthBuffer->Width] >= (unsigned int)DepthBuffer->BufferTail) {
                                if (fill) {
                                    for (int row = 0; row < IsoDrawData.SpanHeight; ++row) {
                                        int start = *IsoDrawData.RowStartCol;
                                        IsoDrawData.RowDest = IsoDrawData.DestPtr;
                                        IsoDrawData.RowDepth = IsoDrawData.DepthPtr;
                                        IsoDrawData.DestPtr += start;
                                        IsoDrawData.DepthPtr += start;
                                        IsoDrawData.DepthPtr = DepthBuffer->Wrap_Pointer(IsoDrawData.DepthPtr);
                                        for (int run = *IsoDrawData.RowRunLength; run > 0; --run) {
                                            *IsoDrawData.DepthPtr = IsoDrawData.FillDepth;
                                            *IsoDrawData.DestPtr = (unsigned short)IsoDrawData.FogColor;
                                            ++IsoDrawData.DestPtr;
                                            ++IsoDrawData.DepthPtr;
                                            IsoDrawData.DepthPtr = DepthBuffer->Wrap_Pointer(IsoDrawData.DepthPtr);
                                        }
                                        IsoDrawData.DestPtr = (unsigned short *)((char *)IsoDrawData.RowDest + IsoDrawData.SurfacePitch);
                                        IsoDrawData.DepthPtr = &IsoDrawData.RowDepth[IsoDrawData.DepthWidth];
                                        IsoDrawData.RowStartCol += ISO_DRAW_WIDTH;
                                        IsoDrawData.RowRunLength += ISO_DRAW_WIDTH;
                                    }
                                } else if (depth_only) {
                                    for (int row = 0; row < IsoDrawData.SpanHeight; ++row) {
                                        int start = *IsoDrawData.RowStartCol;
                                        IsoDrawData.RowDepth = IsoDrawData.DepthPtr;
                                        IsoDrawData.DepthPtr += start;
                                        IsoDrawData.DepthPtr = DepthBuffer->Wrap_Pointer(IsoDrawData.DepthPtr);
                                        for (int run = *IsoDrawData.RowRunLength; run > 0; --run) {
                                            *IsoDrawData.DepthPtr = IsoDrawData.FillDepth;
                                            ++IsoDrawData.DepthPtr;
                                            IsoDrawData.DepthPtr = DepthBuffer->Wrap_Pointer(IsoDrawData.DepthPtr);
                                        }
                                        IsoDrawData.DepthPtr = &IsoDrawData.RowDepth[IsoDrawData.DepthWidth];
                                        IsoDrawData.RowStartCol += ISO_DRAW_WIDTH;
                                        IsoDrawData.RowRunLength += ISO_DRAW_WIDTH;
                                    }
                                } else if (fog) {
                                    if (!Rules->BlendedFog) {
                                        for (int row = 0; row < IsoDrawData.SpanHeight; ++row) {
                                            IsoDrawData.SrcPixel = (int)IsoDrawData.ImageBase + *IsoDrawData.RowSrcOffset;
                                            int start = *IsoDrawData.RowStartCol;
                                            IsoDrawData.RowDest = IsoDrawData.DestPtr;
                                            IsoDrawData.RowDepth = IsoDrawData.DepthPtr;
                                            IsoDrawData.DestPtr += start;
                                            IsoDrawData.DepthPtr += start;
                                            IsoDrawData.DepthPtr = DepthBuffer->Wrap_Pointer(IsoDrawData.DepthPtr);
                                            int checker = ((unsigned char *)IsoDrawData.SrcPixel - image + row + IsoDrawData.ClipTop) & 1;
                                            for (int run = *IsoDrawData.RowRunLength; run > 0; --run) {
                                                if (checker) {
                                                    *IsoDrawData.DestPtr = (unsigned short)IsoDrawData.FogColor;
                                                }
                                                *IsoDrawData.DepthPtr = 0;
                                                checker = checker == 0;
                                                ++IsoDrawData.SrcPixel;
                                                ++IsoDrawData.DestPtr;
                                                ++IsoDrawData.DepthPtr;
                                                IsoDrawData.DepthPtr = DepthBuffer->Wrap_Pointer(IsoDrawData.DepthPtr);
                                            }
                                            IsoDrawData.DestPtr = (unsigned short *)((char *)IsoDrawData.RowDest + IsoDrawData.SurfacePitch);
                                            IsoDrawData.DepthPtr = &IsoDrawData.RowDepth[IsoDrawData.DepthWidth];
                                            IsoDrawData.RowStartCol += ISO_DRAW_WIDTH;
                                            IsoDrawData.RowRunLength += ISO_DRAW_WIDTH;
                                        }
                                    } else {
                                        for (int row = 0; row < IsoDrawData.SpanHeight; ++row) {
                                            IsoDrawData.SrcPixel = (int)IsoDrawData.ImageBase + *IsoDrawData.RowSrcOffset;
                                            int start = *IsoDrawData.RowStartCol;
                                            IsoDrawData.RowDest = IsoDrawData.DestPtr;
                                            IsoDrawData.RowDepth = IsoDrawData.DepthPtr;
                                            IsoDrawData.DestPtr += start;
                                            IsoDrawData.DepthPtr += start;
                                            IsoDrawData.DepthPtr = DepthBuffer->Wrap_Pointer(IsoDrawData.DepthPtr);
                                            for (int run = *IsoDrawData.RowRunLength; run > 0; --run) {
                                                if (*IsoDrawData.DepthPtr > (int)IsoDrawData.FillDepth) {
                                                    *IsoDrawData.DestPtr = (unsigned short)IsoDrawData.FogColor + (IsoDrawData.HalfbrightMask & (*IsoDrawData.DestPtr >> 1));
                                                }
                                                *IsoDrawData.DepthPtr = 0;
                                                ++IsoDrawData.SrcPixel;
                                                ++IsoDrawData.DestPtr;
                                                ++IsoDrawData.DepthPtr;
                                                IsoDrawData.DepthPtr = DepthBuffer->Wrap_Pointer(IsoDrawData.DepthPtr);
                                            }
                                            IsoDrawData.DestPtr = (unsigned short *)((char *)IsoDrawData.RowDest + IsoDrawData.SurfacePitch);
                                            IsoDrawData.DepthPtr = &IsoDrawData.RowDepth[IsoDrawData.DepthWidth];
                                            IsoDrawData.RowStartCol += ISO_DRAW_WIDTH;
                                            IsoDrawData.RowRunLength += ISO_DRAW_WIDTH;
                                        }
                                    }
                                } else {
                                    for (int row = 0; row < IsoDrawData.SpanHeight; ++row) {
                                        int offset = *IsoDrawData.RowSrcOffset;
                                        IsoDrawData.SrcPixel = (int)IsoDrawData.ImageBase + offset;
                                        IsoDrawData.SrcDepth = (int)IsoDrawData.DepthBase + offset;
                                        int start = *IsoDrawData.RowStartCol;
                                        IsoDrawData.RowDest = IsoDrawData.DestPtr;
                                        IsoDrawData.RowDepth = IsoDrawData.DepthPtr;
                                        IsoDrawData.DestPtr += start;
                                        IsoDrawData.DepthPtr += start;
                                        IsoDrawData.DepthPtr = DepthBuffer->Wrap_Pointer(IsoDrawData.DepthPtr);
                                        IsoDrawData.RowAlpha = IsoDrawData.AlphaPtr;
                                        IsoDrawData.AlphaPtr += start;
                                        IsoDrawData.AlphaPtr = AlphaBuffer->Wrap_Pointer(IsoDrawData.AlphaPtr);
                                        for (int run = *IsoDrawData.RowRunLength; run > 0; --run) {
                                            if (*IsoDrawData.DepthPtr >= (unsigned short)((unsigned short)IsoDrawData.BaseDepth + *(unsigned char *)IsoDrawData.SrcDepth)) {
                                                *IsoDrawData.DepthPtr = (unsigned short)IsoDrawData.BaseDepth + *(unsigned char *)IsoDrawData.SrcDepth;
                                                *IsoDrawData.DestPtr = IsoDrawData.PixelTranslate[*(unsigned char *)IsoDrawData.SrcPixel | IsoDrawData.LightRemap[*IsoDrawData.AlphaPtr]];
                                            }
                                            ++IsoDrawData.SrcDepth;
                                            ++IsoDrawData.SrcPixel;
                                            ++IsoDrawData.DestPtr;
                                            ++IsoDrawData.DepthPtr;
                                            IsoDrawData.DepthPtr = DepthBuffer->Wrap_Pointer(IsoDrawData.DepthPtr);
                                            ++IsoDrawData.AlphaPtr;
                                            IsoDrawData.AlphaPtr = AlphaBuffer->Wrap_Pointer(IsoDrawData.AlphaPtr);
                                        }
                                        IsoDrawData.DestPtr = (unsigned short *)((char *)IsoDrawData.RowDest + IsoDrawData.SurfacePitch);
                                        IsoDrawData.DepthPtr = &IsoDrawData.RowDepth[IsoDrawData.DepthWidth];
                                        IsoDrawData.AlphaPtr = &IsoDrawData.RowAlpha[IsoDrawData.AlphaWidth];
                                        IsoDrawData.RowSrcOffset += ISO_DRAW_WIDTH;
                                        IsoDrawData.RowStartCol += ISO_DRAW_WIDTH;
                                        IsoDrawData.RowRunLength += ISO_DRAW_WIDTH;
                                    }
                                }
                            } else if (!IsoTileUseAsmDrawFunc || depth_only || fill || fog) {
                                if (fill) {
                                    for (int row = 0; row < IsoDrawData.SpanHeight; ++row) {
                                        int start = *IsoDrawData.RowStartCol;
                                        IsoDrawData.RowDest = IsoDrawData.DestPtr;
                                        IsoDrawData.RowDepth = IsoDrawData.DepthPtr;
                                        IsoDrawData.DestPtr += start;
                                        IsoDrawData.DepthPtr += start;
                                        for (int run = *IsoDrawData.RowRunLength; run > 0; --run) {
                                            *IsoDrawData.DepthPtr = IsoDrawData.FillDepth;
                                            *IsoDrawData.DestPtr = (unsigned short)IsoDrawData.FogColor;
                                            ++IsoDrawData.DestPtr;
                                            ++IsoDrawData.DepthPtr;
                                        }
                                        IsoDrawData.DestPtr = (unsigned short *)((char *)IsoDrawData.RowDest + IsoDrawData.SurfacePitch);
                                        IsoDrawData.DepthPtr = &IsoDrawData.RowDepth[IsoDrawData.DepthWidth];
                                        IsoDrawData.RowStartCol += ISO_DRAW_WIDTH;
                                        IsoDrawData.RowRunLength += ISO_DRAW_WIDTH;
                                    }
                                } else if (depth_only) {
                                    for (int row = 0; row < IsoDrawData.SpanHeight; ++row) {
                                        int start = *IsoDrawData.RowStartCol;
                                        IsoDrawData.RowDepth = IsoDrawData.DepthPtr;
                                        IsoDrawData.DepthPtr += start;
                                        for (int run = *IsoDrawData.RowRunLength; run > 0; --run) {
                                            *IsoDrawData.DepthPtr = IsoDrawData.FillDepth;
                                            ++IsoDrawData.DepthPtr;
                                        }
                                        IsoDrawData.DepthPtr = &IsoDrawData.RowDepth[IsoDrawData.DepthWidth];
                                        IsoDrawData.RowStartCol += ISO_DRAW_WIDTH;
                                        IsoDrawData.RowRunLength += ISO_DRAW_WIDTH;
                                    }
                                } else if (fog && !Rules->BlendedFog) {
                                    for (int row = 0; row < IsoDrawData.SpanHeight; ++row) {
                                        IsoDrawData.SrcPixel = (int)IsoDrawData.ImageBase + *IsoDrawData.RowSrcOffset;
                                        int start = *IsoDrawData.RowStartCol;
                                        IsoDrawData.RowDest = IsoDrawData.DestPtr;
                                        IsoDrawData.RowDepth = IsoDrawData.DepthPtr;
                                        IsoDrawData.DestPtr += start;
                                        IsoDrawData.DepthPtr += start;
                                        int checker = ((unsigned char *)IsoDrawData.SrcPixel - image + row + IsoDrawData.ClipTop) & 1;
                                        for (int run = *IsoDrawData.RowRunLength; run > 0; --run) {
                                            if (checker) {
                                                *IsoDrawData.DestPtr = (unsigned short)IsoDrawData.FogColor;
                                            }
                                            *IsoDrawData.DepthPtr = 0;
                                            checker = checker == 0;
                                            ++IsoDrawData.SrcPixel;
                                            ++IsoDrawData.DestPtr;
                                            ++IsoDrawData.DepthPtr;
                                        }
                                        IsoDrawData.DestPtr = (unsigned short *)((char *)IsoDrawData.RowDest + IsoDrawData.SurfacePitch);
                                        IsoDrawData.DepthPtr = &IsoDrawData.RowDepth[IsoDrawData.DepthWidth];
                                        IsoDrawData.RowStartCol += ISO_DRAW_WIDTH;
                                        IsoDrawData.RowRunLength += ISO_DRAW_WIDTH;
                                    }
                                } else if (fog) {
                                    for (int row = 0; row < IsoDrawData.SpanHeight; ++row) {
                                        IsoDrawData.SrcPixel = (int)IsoDrawData.ImageBase + *IsoDrawData.RowSrcOffset;
                                        int start = *IsoDrawData.RowStartCol;
                                        IsoDrawData.RowDest = IsoDrawData.DestPtr;
                                        IsoDrawData.RowDepth = IsoDrawData.DepthPtr;
                                        IsoDrawData.DestPtr += start;
                                        IsoDrawData.DepthPtr += start;
                                        for (int run = *IsoDrawData.RowRunLength; run > 0; --run) {
                                            if (*IsoDrawData.DepthPtr > (int)IsoDrawData.FillDepth) {
                                                *IsoDrawData.DestPtr = (unsigned short)IsoDrawData.FogColor + (IsoDrawData.HalfbrightMask & (*IsoDrawData.DestPtr >> 1));
                                            }
                                            *IsoDrawData.DepthPtr = 0;
                                            ++IsoDrawData.SrcPixel;
                                            ++IsoDrawData.DestPtr;
                                            ++IsoDrawData.DepthPtr;
                                        }
                                        IsoDrawData.DestPtr = (unsigned short *)((char *)IsoDrawData.RowDest + IsoDrawData.SurfacePitch);
                                        IsoDrawData.DepthPtr = &IsoDrawData.RowDepth[IsoDrawData.DepthWidth];
                                        IsoDrawData.RowStartCol += ISO_DRAW_WIDTH;
                                        IsoDrawData.RowRunLength += ISO_DRAW_WIDTH;
                                    }
                                } else {
                                    for (int row = 0; row < IsoDrawData.SpanHeight; ++row) {
                                        int offset = *IsoDrawData.RowSrcOffset;
                                        IsoDrawData.SrcPixel = (int)IsoDrawData.ImageBase + offset;
                                        IsoDrawData.SrcDepth = (int)IsoDrawData.DepthBase + offset;
                                        int start = *IsoDrawData.RowStartCol;
                                        IsoDrawData.RowDest = IsoDrawData.DestPtr;
                                        IsoDrawData.RowDepth = IsoDrawData.DepthPtr;
                                        IsoDrawData.RowAlpha = IsoDrawData.AlphaPtr;
                                        IsoDrawData.DestPtr += start;
                                        IsoDrawData.DepthPtr += start;
                                        IsoDrawData.AlphaPtr += start;
                                        for (int run = *IsoDrawData.RowRunLength; run > 0; --run) {
                                            if (*IsoDrawData.DepthPtr >= (unsigned short)((unsigned short)IsoDrawData.BaseDepth + *(unsigned char *)IsoDrawData.SrcDepth)) {
                                                *IsoDrawData.DepthPtr = (unsigned short)IsoDrawData.BaseDepth + *(unsigned char *)IsoDrawData.SrcDepth;
                                                *IsoDrawData.DestPtr = IsoDrawData.PixelTranslate[*(unsigned char *)IsoDrawData.SrcPixel | IsoDrawData.LightRemap[*IsoDrawData.AlphaPtr]];
                                            }
                                            ++IsoDrawData.SrcDepth;
                                            ++IsoDrawData.SrcPixel;
                                            ++IsoDrawData.DestPtr;
                                            ++IsoDrawData.DepthPtr;
                                            ++IsoDrawData.AlphaPtr;
                                        }
                                        IsoDrawData.DestPtr = (unsigned short *)((char *)IsoDrawData.RowDest + IsoDrawData.SurfacePitch);
                                        IsoDrawData.DepthPtr = &IsoDrawData.RowDepth[IsoDrawData.DepthWidth];
                                        IsoDrawData.AlphaPtr = &IsoDrawData.RowAlpha[IsoDrawData.AlphaWidth];
                                        IsoDrawData.RowSrcOffset += ISO_DRAW_WIDTH;
                                        IsoDrawData.RowStartCol += ISO_DRAW_WIDTH;
                                        IsoDrawData.RowRunLength += ISO_DRAW_WIDTH;
                                    }
                                }
                            }
                        } else if ((unsigned int)&IsoDrawData.AlphaPtr[IsoDrawData.SpanWidth + 2 + IsoDrawData.SpanHeight * AlphaBuffer->Width] >= (unsigned int)AlphaBuffer->BufferTail) {
                            for (int row = 0; row < IsoDrawData.SpanHeight; ++row) {
                                IsoDrawData.SrcPixel = (int)IsoDrawData.ImageBase + *IsoDrawData.RowSrcOffset;
                                int start = *IsoDrawData.RowStartCol;
                                IsoDrawData.RowDest = IsoDrawData.DestPtr;
                                IsoDrawData.RowAlpha = IsoDrawData.AlphaPtr;
                                IsoDrawData.DestPtr += start;
                                IsoDrawData.AlphaPtr += start;
                                IsoDrawData.AlphaPtr = AlphaBuffer->Wrap_Pointer(IsoDrawData.AlphaPtr);
                                for (int run = *IsoDrawData.RowRunLength; run > 0; --run) {
                                    *IsoDrawData.DestPtr = IsoDrawData.PixelTranslate[*(unsigned char *)IsoDrawData.SrcPixel | IsoDrawData.LightRemap[*IsoDrawData.AlphaPtr]];
                                    ++IsoDrawData.DestPtr;
                                    ++IsoDrawData.SrcPixel;
                                    ++IsoDrawData.AlphaPtr;
                                    IsoDrawData.AlphaPtr = AlphaBuffer->Wrap_Pointer(IsoDrawData.AlphaPtr);
                                }
                                IsoDrawData.DestPtr = (unsigned short *)((char *)IsoDrawData.RowDest + IsoDrawData.SurfacePitch);
                                IsoDrawData.AlphaPtr = &IsoDrawData.RowAlpha[IsoDrawData.AlphaWidth];
                                IsoDrawData.RowSrcOffset += ISO_DRAW_WIDTH;
                                IsoDrawData.RowStartCol += ISO_DRAW_WIDTH;
                                IsoDrawData.RowRunLength += ISO_DRAW_WIDTH;
                            }
                        } else {
                            for (int row = 0; row < IsoDrawData.SpanHeight; ++row) {
                                IsoDrawData.SrcPixel = (int)IsoDrawData.ImageBase + *IsoDrawData.RowSrcOffset;
                                int start = *IsoDrawData.RowStartCol;
                                IsoDrawData.RowDest = IsoDrawData.DestPtr;
                                IsoDrawData.RowAlpha = IsoDrawData.AlphaPtr;
                                IsoDrawData.DestPtr += start;
                                IsoDrawData.AlphaPtr += start;
                                for (int run = *IsoDrawData.RowRunLength; run > 0; --run) {
                                    *IsoDrawData.DestPtr = IsoDrawData.PixelTranslate[*(unsigned char *)IsoDrawData.SrcPixel | IsoDrawData.LightRemap[*IsoDrawData.AlphaPtr]];
                                    ++IsoDrawData.DestPtr;
                                    ++IsoDrawData.SrcPixel;
                                    ++IsoDrawData.AlphaPtr;
                                }
                                IsoDrawData.DestPtr = (unsigned short *)((char *)IsoDrawData.RowDest + IsoDrawData.SurfacePitch);
                                IsoDrawData.AlphaPtr = &IsoDrawData.RowAlpha[IsoDrawData.AlphaWidth];
                                IsoDrawData.RowSrcOffset += ISO_DRAW_WIDTH;
                                IsoDrawData.RowStartCol += ISO_DRAW_WIDTH;
                                IsoDrawData.RowRunLength += ISO_DRAW_WIDTH;
                            }
                        }
                        surface.Unlock();
                    }
                }
                if (!skip_extra) {
                    if (record->IsHasExtraData && !fill && !depth_only && !fog) {
                        int ex = record->ExtraX - record->X + x;
                        int ey = record->ExtraY - record->Y + y;
                        IsoDrawData.SpanWidth = record->ExtraWidth;
                        IsoDrawData.SpanHeight = record->ExtraHeight;
                        IsoDrawData.ClipLeft = 0;
                        IsoDrawData.ClipTop = 0;
                        bool extra_ok = true;
                        if (ex < cliprect.X) {
                            IsoDrawData.SpanWidth = ex - cliprect.X + record->ExtraWidth;
                            IsoDrawData.ClipLeft = cliprect.X - ex;
                            ex = cliprect.X;
                            if (IsoDrawData.SpanWidth <= 0) {
                                extra_ok = false;
                            }
                        }
                        if (extra_ok && ey < cliprect.Y) {
                            IsoDrawData.SpanHeight = ey - cliprect.Y + IsoDrawData.SpanHeight;
                            IsoDrawData.ClipTop = cliprect.Y - ey;
                            ey = cliprect.Y;
                            if (IsoDrawData.SpanHeight <= 0) {
                                extra_ok = false;
                            }
                        }
                        if (extra_ok && (int)IsoDrawData.SpanWidth + ex > clip_right) {
                            IsoDrawData.SpanWidth = cliprect.Width - ex + cliprect.X;
                            if (IsoDrawData.SpanWidth <= 0) {
                                extra_ok = false;
                            }
                        }
                        if (extra_ok && IsoDrawData.SpanHeight + ey > clip_bottom) {
                            IsoDrawData.SpanHeight = cliprect.Height - ey + cliprect.Y;
                            if (IsoDrawData.SpanHeight <= 0) {
                                extra_ok = false;
                            }
                        }
                        if (extra_ok) {
                            IsoDrawData.ImageRowStep = record->ExtraWidth - IsoDrawData.SpanWidth;
                            IsoDrawData.SurfacePitch = surface.Stride() - IsoDrawData.SpanWidth * surface.Bytes_Per_Pixel();
                            if (use_z) {
                                IsoDrawData.DepthPtr = (unsigned short *)DepthBuffer->Get_Buffer_Offset(Point2D(ex, ey - unknown_rect_839630.Y));
                                IsoDrawData.DepthWidth = DepthBuffer->Width - IsoDrawData.SpanWidth;
                            }
                            IsoDrawData.AlphaPtr = (unsigned short *)AlphaBuffer->Get_Buffer_Offset(Point2D(ex, ey - unknown_rect_839630.Y));
                            IsoDrawData.AlphaWidth = AlphaBuffer->Width - IsoDrawData.SpanWidth;
                            IsoDrawData.DestPtr = (unsigned short *)surface.Lock(Point2D(ex, ey));
                            if (IsoDrawData.DestPtr != NULL) {
                                IsoDrawData.SrcPixel = (int)record + record->ExtraOffset + IsoDrawData.ClipLeft;
                                IsoDrawData.SrcPixel = IsoDrawData.ClipTop * record->ExtraWidth + IsoDrawData.SrcPixel;
                                if (use_z) {
                                    IsoDrawData.SrcDepth = (int)record + record->ExtraZOffset + IsoDrawData.ClipLeft;
                                    IsoDrawData.SrcDepth = IsoDrawData.ClipTop * record->ExtraWidth + IsoDrawData.SrcDepth;
                                    if ((unsigned int)&IsoDrawData.DepthPtr[IsoDrawData.SpanWidth + 2 + IsoDrawData.SpanHeight * DepthBuffer->Width] >= (unsigned int)DepthBuffer->BufferTail) {
                                        /*
                                         * Depth-tested extra image, Z buffer wrapping.
                                         */
                                        for (int row = 0; row < IsoDrawData.SpanHeight; ++row) {
                                            for (int col = 0; col < (int)IsoDrawData.SpanWidth; ++col) {
                                                if (*(unsigned char *)IsoDrawData.SrcPixel) {
                                                    if (*IsoDrawData.DepthPtr >= (unsigned short)((unsigned short)IsoDrawData.BaseDepth + *(unsigned char *)IsoDrawData.SrcDepth)) {
                                                        *IsoDrawData.DestPtr = IsoDrawData.PixelTranslate[*(unsigned char *)IsoDrawData.SrcPixel | IsoDrawData.LightRemap[*IsoDrawData.AlphaPtr]];
                                                        *IsoDrawData.DepthPtr = (unsigned short)IsoDrawData.BaseDepth + *(unsigned char *)IsoDrawData.SrcDepth;
                                                    }
                                                }
                                                ++IsoDrawData.DestPtr;
                                                ++IsoDrawData.SrcPixel;
                                                ++IsoDrawData.SrcDepth;
                                                ++IsoDrawData.DepthPtr;
                                                IsoDrawData.DepthPtr = DepthBuffer->Wrap_Pointer(IsoDrawData.DepthPtr);
                                                ++IsoDrawData.AlphaPtr;
                                                IsoDrawData.AlphaPtr = AlphaBuffer->Wrap_Pointer(IsoDrawData.AlphaPtr);
                                            }
                                            IsoDrawData.DestPtr = (unsigned short *)((char *)IsoDrawData.DestPtr + IsoDrawData.SurfacePitch);
                                            IsoDrawData.SrcPixel += IsoDrawData.ImageRowStep;
                                            IsoDrawData.SrcDepth += IsoDrawData.ImageRowStep;
                                            IsoDrawData.DepthPtr = &IsoDrawData.DepthPtr[IsoDrawData.DepthWidth];
                                            IsoDrawData.DepthPtr = DepthBuffer->Wrap_Pointer(IsoDrawData.DepthPtr);
                                            IsoDrawData.AlphaPtr = &IsoDrawData.AlphaPtr[IsoDrawData.AlphaWidth];
                                            IsoDrawData.AlphaPtr = AlphaBuffer->Wrap_Pointer(IsoDrawData.AlphaPtr);
                                        }
                                    } else if (IsoTileUseAsmDrawFunc) {
                                        Iso_Blit_Asm2((IsoBlitState *)&IsoDrawData);
                                    } else {
                                        /*
                                         * Depth-tested extra image, no wrapping.
                                         */
                                        for (int row = 0; row < IsoDrawData.SpanHeight; ++row) {
                                            for (int col = 0; col < (int)IsoDrawData.SpanWidth; ++col) {
                                                if (*(unsigned char *)IsoDrawData.SrcPixel) {
                                                    if (*IsoDrawData.DepthPtr >= (unsigned short)((unsigned short)IsoDrawData.BaseDepth + *(unsigned char *)IsoDrawData.SrcDepth)) {
                                                        *IsoDrawData.DestPtr = IsoDrawData.PixelTranslate[*(unsigned char *)IsoDrawData.SrcPixel | IsoDrawData.LightRemap[*IsoDrawData.AlphaPtr]];
                                                        *IsoDrawData.DepthPtr = (unsigned short)IsoDrawData.BaseDepth + *(unsigned char *)IsoDrawData.SrcDepth;
                                                    }
                                                }
                                                ++IsoDrawData.DestPtr;
                                                ++IsoDrawData.SrcPixel;
                                                ++IsoDrawData.SrcDepth;
                                                ++IsoDrawData.DepthPtr;
                                                ++IsoDrawData.AlphaPtr;
                                            }
                                            IsoDrawData.DestPtr = (unsigned short *)((char *)IsoDrawData.DestPtr + IsoDrawData.SurfacePitch);
                                            IsoDrawData.SrcPixel += IsoDrawData.ImageRowStep;
                                            IsoDrawData.SrcDepth += IsoDrawData.ImageRowStep;
                                            IsoDrawData.DepthPtr = &IsoDrawData.DepthPtr[IsoDrawData.DepthWidth];
                                            IsoDrawData.AlphaPtr = &IsoDrawData.AlphaPtr[IsoDrawData.AlphaWidth];
                                        }
                                    }
                                } else if ((unsigned int)&IsoDrawData.AlphaPtr[IsoDrawData.SpanWidth + 2 + IsoDrawData.SpanHeight * AlphaBuffer->Width] >= (unsigned int)AlphaBuffer->BufferTail) {
                                    /*
                                     * Plain extra image (no depth), alpha buffer wrapping.
                                     */
                                    for (int row = 0; row < IsoDrawData.SpanHeight; ++row) {
                                        for (int col = 0; col < (int)IsoDrawData.SpanWidth; ++col) {
                                            if (*(unsigned char *)IsoDrawData.SrcPixel) {
                                                *IsoDrawData.DestPtr = IsoDrawData.PixelTranslate[*(unsigned char *)IsoDrawData.SrcPixel | IsoDrawData.LightRemap[*IsoDrawData.AlphaPtr]];
                                            }
                                            ++IsoDrawData.DestPtr;
                                            ++IsoDrawData.AlphaPtr;
                                            IsoDrawData.AlphaPtr = AlphaBuffer->Wrap_Pointer(IsoDrawData.AlphaPtr);
                                            ++IsoDrawData.SrcPixel;
                                        }
                                        IsoDrawData.DestPtr = (unsigned short *)((char *)IsoDrawData.DestPtr + IsoDrawData.SurfacePitch);
                                        IsoDrawData.AlphaPtr = &IsoDrawData.AlphaPtr[IsoDrawData.AlphaWidth];
                                        IsoDrawData.AlphaPtr = AlphaBuffer->Wrap_Pointer(IsoDrawData.AlphaPtr);
                                        IsoDrawData.SrcPixel += IsoDrawData.ImageRowStep;
                                    }
                                } else {
                                    /*
                                     * Plain extra image (no depth), no wrapping.
                                     */
                                    for (int row = 0; row < IsoDrawData.SpanHeight; ++row) {
                                        for (int col = 0; col < (int)IsoDrawData.SpanWidth; ++col) {
                                            if (*(unsigned char *)IsoDrawData.SrcPixel) {
                                                *IsoDrawData.DestPtr = IsoDrawData.PixelTranslate[*(unsigned char *)IsoDrawData.SrcPixel | IsoDrawData.LightRemap[*IsoDrawData.AlphaPtr]];
                                            }
                                            ++IsoDrawData.DestPtr;
                                            ++IsoDrawData.SrcPixel;
                                            ++IsoDrawData.AlphaPtr;
                                        }
                                        IsoDrawData.AlphaPtr = &IsoDrawData.AlphaPtr[IsoDrawData.AlphaWidth];
                                        IsoDrawData.DestPtr = (unsigned short *)((char *)IsoDrawData.DestPtr + IsoDrawData.SurfacePitch);
                                        IsoDrawData.SrcPixel += IsoDrawData.ImageRowStep;
                                    }
                                }
                                surface.Unlock();
                            }
                        }
                    }
                }
            }
        }
        AlphaTableCache_008315B8.Release(aremap);
    }
}
#endif

// bool IsometricTileTypeClass::Get_Tile_Image(int, unsigned char **, int, int) (0x0052DD70)
// Unpacks one sub tile of the set into a byte image: the destination is cleared, the tile's
// pixels are copied through the diamond mask (0xDB = a tile pixel, 0x20 = outside the diamond),
// then a cliff's extra image is overlaid (zero bytes transparent).

bool IsometricTileTypeClass::Get_Tile_Image(int tile, unsigned char **buffer, int width, int height)
{
#define __ "\x20"
#define XX "\xDB"
    static unsigned char _tilemask[] = {
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __
        __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __
        __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __
        __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __
        XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX
        __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __
        __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __
        __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __
        __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
    };
#undef __
#undef XX

    int tilew = 0;
    int tileh = 0;
    memset(*buffer, 0, width * height);
    IsoTileImageStruct const *image = (IsoTileImageStruct const *)Get_Image_Data();

    if (image != 0) {
        tile = tile % (image->unknown_00[0] * image->unknown_00[1]);
        IsoTileCellStruct const *record = (IsoTileCellStruct const *)image->Tiles[tile];

        if (record != 0) {
            Get_Tile_Pixel_Dimensions(tile, tilew, tileh);

            if (tilew <= width && tileh <= height) {
                unsigned char *buf = *buffer;
                unsigned char *end = &(*buffer)[width * height];

                if (record->IsHasExtraData) {
                    buf += width * (record->Y - record->ExtraY);
                    if (buf >= end) {
                        return false;
                    }
                }

                unsigned char *mask = _tilemask;
                unsigned char *pixels = (unsigned char *)(record + 1);

                for (int py = 0; py < image->unknown_00[3]; py++) {
                    for (int px = 0; px < image->unknown_00[2]; px++) {
                        if (*mask++ != '\x20') {
                            *buf = *pixels;
                            pixels++;
                        }
                        buf++;
                        if (buf >= end) {
                            return false;
                        }
                    }
                    buf += width - image->unknown_00[2];
                    if (buf >= end) {
                        return false;
                    }
                }

                if (record->IsHasExtraData) {
                    buf = &(*buffer)[record->ExtraX - record->X];
                    if (buf >= end) {
                        return false;
                    }

                    unsigned char *extra = (unsigned char *)record + record->ExtraOffset;

                    for (int ey = 0; ey < record->ExtraHeight; ey++) {
                        for (int ex = 0; ex < record->ExtraWidth; ex++) {
                            if (*extra != 0) {
                                *buf = *extra;
                            }
                            buf++;
                            extra++;
                            if (buf >= end) {
                                return false;
                            }
                        }
                        buf += width - record->ExtraWidth;
                        if (buf >= end) {
                            return false;
                        }
                    }
                }
                return true;
            }
        }
        return false;
    }
    return true;
}


// 0x0052df40: function not yet identified
//
// Body: 90 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall FUN_0052df40(int param_1,int param_2,int *param_3,int *param_4)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0052df40 {

void __fastcall FUN_0052df40(int grid_x, int grid_y, int *pixel_x, int *pixel_y)
{
    *pixel_x = (grid_x * 0x3c) / 2;
    *pixel_y = (grid_x * 0x1e) / 2;
    *pixel_x = *pixel_x + (grid_y * -0x3c) / 2;
    *pixel_y = *pixel_y + (grid_y * 0x1e) / 2;
    return;
}

}  // namespace seed_0052df40


// int IsometricTileTypeClass::Get_Y_Offset(int tile) -- 0x0052DFA0
// How far the tile's extra (cliff) image starts below the tile itself: ExtraY - Y of the
// cell record when the record has extra data; 0 otherwise (TibSun Get_Y_Offset).
int IsometricTileTypeClass::Get_Y_Offset(int tile)
{
    IsoTileImageStruct const *image = (IsoTileImageStruct const *)Get_Image_Data();
    if (image != 0 && tile < image->unknown_00[1] * image->unknown_00[0]) {
        IsoTileCellStruct const *record = (IsoTileCellStruct const *)image->Tiles[tile];
        if (record != 0 && record->IsHasExtraData) {
            return record->ExtraY - record->Y;
        }
    }
    return 0;
}


// bool IsometricTileTypeClass::Create_And_Place(Cell const &cell, HouseClass *house) const (0x0052DFE0, slot 32)
// Places a tile of this type at the cell (ObjectTypeClass slot 32: create
// the object there); true when one was made.
// Only the list accessors expand here: the colour table constructor stays a call.

bool IsometricTileTypeClass::Create_And_Place(Cell const &cell, HouseClass *house) const
{
    if (new IsometricTileClass(ArrayIndex, cell) != 0) {
        return true;
    }
    return false;
}


// ObjectClass *IsometricTileTypeClass::Create_One_Of(HouseClass *house) const (0x0052E020, slot 35)
// A new tile of this type at no cell, loading the type's image first when
// it is missing and allowed (unknown_2c4).
// Only the list accessors expand here: the colour table constructor stays a call.

// The "no cell" position (0x00A52EAC; hypothesis).
extern Cell const unknown_cell_a52eac;

ObjectClass *IsometricTileTypeClass::Create_One_Of(HouseClass *house) const
{
    if (unknown_a4 == 0 && unknown_2c4) {
        const_cast<IsometricTileTypeClass *>(this)->Load_Tile_Data();
    }
    return new IsometricTileClass(ArrayIndex, unknown_cell_a52eac);
}


// 0x0052e070: function not yet identified
//
// Body: 28 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void FUN_0052e070(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0052e070 {

extern "C" unsigned int SlopeZShapes_00a52eb0;
extern "C" unsigned int DAT_00a52eb4;
extern "C" unsigned int DAT_00a52eb8;
extern "C" unsigned int DAT_00a52ebc;
extern "C" unsigned int DAT_00a6e38c;

void  FUN_0052e070(void)
{
    DAT_00a6e38c = 0;
    SlopeZShapes_00a52eb0 = 0;
    DAT_00a52eb4 = 0;
    DAT_00a52eb8 = 0;
    DAT_00a52ebc = 0;
    return;
}

}  // namespace seed_0052e070


// CoordStruct IsometricTileTypeClass::Coord_Fixup(CoordStruct const &coord) const (0x0052E090, slot 27)
// The coordinate unchanged.
// Only the list accessors expand here: the colour table constructor stays a call.

CoordStruct IsometricTileTypeClass::Coord_Fixup(CoordStruct const &coord) const
{
    return coord;
}


// void IsometricTileTypeClass::Compute_CRC(WWCRCEngine &crc) const (0x0052E0B0, slot 13)

void IsometricTileTypeClass::Compute_CRC(WWCRCEngine &crc) const
{
    ObjectTypeClass::Compute_CRC(crc);
    crc((long)ArrayIndex);
    crc((long)MarbleMadness);
    crc((long)NonMarbleMadness);
    crc((long)TileSetBaseID);
    crc((long)unknown_274.Count());
    if (unknown_28c != 0) {
        crc(unknown_28c->Fetch_ID());
    }
    crc((long)ToSnowTheater);
    crc((long)ToTemperateTheater);
    crc((long)AttachesTo);
    crc((long)ZAdjust);
    crc((long)Unused1);
    crc(unknown_2b0);
    crc(IsShadowCaster);
    crc((long)unknown_2b4);
    crc((long)unknown_2b8);
    crc((long)Unused2);
    crc((long)NumTileTypesInSet);
}


// HRESULT __stdcall IsometricTileTypeClass::Load(IStream *stream) (0x0052E1C0, slot 5)
// Releases every light converter, loads the object type, restores the
// virtual tables (and an empty unknown_274) in place, fixes up unknown_28c,
// forgets the converters again and rebuilds unknown_274.


HRESULT __stdcall IsometricTileTypeClass::Load(IStream *stream)
{
    int index;
    for (index = 0; index < unknown_LightConvertClass_list_00832398.Count(); index++) {
        delete unknown_LightConvertClass_list_00832398[index];
        unknown_LightConvertClass_list_00832398[index] = 0;
    }

    HRESULT result = ObjectTypeClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    new (this) IsometricTileTypeClass(NoInitClass());
    Swizzler.Swizzle((void **)&unknown_28c);

    for (index = 0; index < unknown_LightConvertClass_list_00832398.Count(); index++) {
        unknown_LightConvertClass_list_00832398[index] = 0;
    }
    Build_Preview_Tiles();
    return S_OK;
}


// HRESULT __stdcall IsometricTileTypeClass::Save(IStream *stream, BOOL clear_dirty) (0x0052E2B0, slot 6)
// The object type's save; S_OK when it succeeded.
// Only the list accessors expand here: the colour table constructor stays a call.

HRESULT __stdcall IsometricTileTypeClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = ObjectTypeClass::Save(stream, clear_dirty);
    if (SUCCEEDED(result)) {
        result = S_OK;
    }
    return result;
}


// HRESULT __stdcall IsometricTileTypeClass::GetClassID(GUID *clsid) (0x0052E2D0, slot 3)
// Only the list accessors expand here: the colour table constructor stays a call.

HRESULT __stdcall IsometricTileTypeClass::GetClassID(GUID *clsid)
{
    if (clsid == 0) {
        return E_POINTER;
    }
    *clsid = CLSID_IsometricTileTypeClass;
    return S_OK;
}


// void IsometricTileTypeClass::Detach(AbstractClass *target, bool all) (0x0052E310, slot 10)
// Forgets unknown_28c and the animation type (unknown_298) when they are the target.
// Only the list accessors expand here: the colour table constructor stays a call.

void IsometricTileTypeClass::Detach(AbstractClass *target, bool all)
{
    if (unknown_28c == target) {
        unknown_28c = 0;
    }
    if (unknown_298 != -1 && AnimTypes[unknown_298] == target) {
        unknown_298 = -1;
    }
}


// unsigned short IsometricTileTypeClass::Preview_Tile_Color(int index, int value, bool second) const (0x0052E360)
// One entry of the colour tables rebuilt by Build_Preview_Tiles: table
// `index`, pair `value`, its first or second word.
// Only the list accessors expand here: the colour table constructor stays a call.

unsigned short IsometricTileTypeClass::Preview_Tile_Color(int index, int value, bool second) const
{
    return unknown_274[index][value * 2 + (second != 0)];
}


// void IsometricTileTypeClass::Build_Preview_Tiles() (0x0052E3D0): rebuilds the preview colour
// tables, 13 light levels of the low and high colour per sub tile (TibSun Build_Preview_Tiles).
#include "shared/hicolor.hpp"

void IsometricTileTypeClass::Build_Preview_Tiles()
{
    int i;
    int j;
    IsoTileImageStruct *tileset = (IsoTileImageStruct *)unknown_a4;
    for (i = 0; i < unknown_274.Count(); i++) {
        if (unknown_274[i] != 0) {
            delete [] unknown_274[i];
        }
    }
    unknown_274.Clear();

    for (i = 0; i < tileset->unknown_00[0] * tileset->unknown_00[1]; i++) {
        if (tileset->Tiles[i] != 0) {
            unsigned short *buffer = new unsigned short[24 + 2];
            IsoTileCellStruct const *record = (IsoTileCellStruct const *)tileset->Tiles[i];
            unknown_274.Add(buffer);
            RGBClass low(record->LowRed, record->LowGreen, record->LowBlue);
            RGBClass high(record->HighRed, record->HighGreen, record->HighBlue);
            static float const _brightness_factor = 1.4f;
            RGBClass alow((NoInitClass()));
            alow.Set(low, _brightness_factor);
            RGBClass ahigh((NoInitClass()));
            ahigh.Set(high, _brightness_factor);
            unsigned short *ptr = buffer;
            RGBClass tmp((NoInitClass()));
            for (j = 0; j <= 12; j++) {
                tmp = RGBClass(low, alow, float((double)j / 12.0f));
                ptr[0] = (unsigned short)Build_Hicolor_Pixel(tmp.Red, tmp.Green, tmp.Blue);
                tmp = RGBClass(high, ahigh, float((double)j / 12.0f));
                ptr[1] = (unsigned short)Build_Hicolor_Pixel(tmp.Red, tmp.Green, tmp.Blue);
                ptr += 2;
            }
        } else {
            unknown_274.Add(0);
        }
    }
}


// ShadowCasterInfo::ShadowCasterInfo (0x0052E660): the out-of-line copy of the inline constructor
// in this unit's cliff initializer (defined with the struct above).


// Not exact (RA2_NON_MATCHING): IsometricTileTypeClass::Preview_Tile at 0x0052E390, best score 81.25, 2026-11-13; differs: same instruction count (16), shape 93.75: instruction choice and registers differ
#ifdef RA2_NON_MATCHING
unsigned short *IsometricTileTypeClass::Preview_Tile(int tile, int level)
{
    Load_Tile_Image();
    return &unknown_274[tile][level * 2];
}
#endif

// int IsometricTileTypeClass::Kind_Of() const (0x0052E680, slot 11)
// Only the list accessors expand here: the colour table constructor stays a call.

int IsometricTileTypeClass::Kind_Of() const
{
    return 0x12;
}


// int IsometricTileTypeClass::Size_Of() const (0x0052E690, slot 12)
// Only the list accessors expand here: the colour table constructor stays a call.

int IsometricTileTypeClass::Size_Of() const
{
    return sizeof(IsometricTileTypeClass);
}


// int IsometricTileTypeClass::Fetch_Heap_ID() const (0x0052E6A0): the type's index.
// Only the list accessors expand here: the colour table constructor stays a call.

int IsometricTileTypeClass::Fetch_Heap_ID() const
{
    return ArrayIndex;
}


extern "C" void __cdecl release_52e6b0(void*);
class Leaf_52e6b0 { public: void cleanup();void* get(unsigned int flags); };
void* Leaf_52e6b0::get(unsigned int flags) { cleanup(); if (flags & 1) release_52e6b0(this); return this; }


// 0x0052e700: function not yet identified
//
// Body: 27 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: uint __thiscall FUN_0052e700(int param_1,int param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0052e700 {

struct Owner_0052e700 { unsigned int FUN_0052e700(int element_address); };

unsigned int Owner_0052e700::FUN_0052e700(int element_address)
{
    int vector_address = (int)this;
    if (*(char *)(vector_address + 0xc) == '\0') {
        return 0;
    }
    return (unsigned int)(element_address - *(int *)(vector_address + 4)) >> 2;
}

}  // namespace seed_0052e700


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<unsigned short *>::Clear (0x0052E6D0; DynamicVectorClass<unsigned short *> vtable 0x007a524c slot 3).
// VectorClass<unsigned short *>::Fetch(int) const (slot 6 of the DynamicVectorClass<unsigned short *> vtable 0x007A524C) (0x0052E720).
// VectorClass<unsigned short *>::operator equal (0x0052E790; VectorClass<unsigned short *> vtable 0x007a526c slot 1).
// VectorClass<unsigned short *>::Resize (0x0052E7D0; VectorClass<unsigned short *> vtable 0x007a526c slot 2).
// VectorClass<unsigned short *>::Clear (0x0052E880; VectorClass<unsigned short *> vtable 0x007a526c slot 3).
// VectorClass<unsigned short *>::ID_object (0x0052E8B0; VectorClass<unsigned short *> vtable 0x007a526c slot 4).
// VectorClass<unsigned short *>::ID_pointer (0x0052E8F0; VectorClass<unsigned short *> vtable 0x007a526c slot 5).
// DynamicVectorClass<unsigned short *>::Resize (0x0052E910; DynamicVectorClass<unsigned short *> vtable 0x007a524c slot 2).
// DynamicVectorClass<unsigned short *>::ID_object (0x0052E9C0; DynamicVectorClass<unsigned short *> vtable 0x007a524c slot 4).


extern "C" unsigned char vtable_52ec50[]; extern "C" void __cdecl release_buffer_52ec50(void*); extern "C" void __cdecl release_object_52ec50(void*);
class Leaf_52ec50 { public: void* get(unsigned int flags); };
void* Leaf_52ec50::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_52ec50; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_52ec50((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_52ec50(this); return this; }


extern "C" unsigned char vtable_52eca0[]; extern "C" void __cdecl release_buffer_52eca0(void*); extern "C" void __cdecl release_object_52eca0(void*);
class Leaf_52eca0 { public: void* get(unsigned int flags); };
void* Leaf_52eca0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_52eca0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_52eca0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_52eca0(this); return this; }


extern "C" unsigned char vtable_52ecf0[]; extern "C" void __cdecl release_buffer_52ecf0(void*); extern "C" void __cdecl release_object_52ecf0(void*);
class Leaf_52ecf0 { public: void* get(unsigned int flags); };
void* Leaf_52ecf0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_52ecf0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_52ecf0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_52ecf0(this); return this; }


extern "C" unsigned char vtable_52ed40[]; extern "C" void __cdecl release_buffer_52ed40(void*); extern "C" void __cdecl release_object_52ed40(void*);
class Leaf_52ed40 { public: void* get(unsigned int flags); };
void* Leaf_52ed40::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_52ed40; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_52ed40((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_52ed40(this); return this; }


// VectorClass<unsigned short *>::VectorClass(unsigned, unsigned short * const *) (0x0052ED90): the
// out-of-line copy Load (0x0052E1C0) calls once its inline budget runs out; emitted by this unit.
