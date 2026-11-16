/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 * Copyright 2026 ra2-decomp contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/
// Function comment blocks from mischa85/RedAlert2 392e904 src/smudtype.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.
// Parameter names from mischa85/RedAlert2 392e904 src/smudtype.cpp where both define the function (names only).

// smudtype.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/smudtype.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/smudge_type.hpp"
#include "shared/anim_type.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/mix_file_class.hpp"
#include "shared/smudge.hpp"
#include "shared/map_view.hpp"
#include "shared/palette.hpp"
#include "shared/anim.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/random.hpp"
#include "shared/isometric_tile.hpp"


// Header-static dynamic initializers of the unit at 0x00687BB0: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x00687BB0-0x00687D91); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x00687BD0, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_00687bb0.cpp; no source of its own.


// Dynamic initializer (0x00687C00, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_00687bb0.cpp; no source of its own.


// Dynamic initializer (0x00687C20, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_00687bb0.cpp; no source of its own.


// Dynamic initializer (0x00687C40, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_00687bb0.cpp; no source of its own.


// Dynamic initializer (0x00687C60, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_00687bb0.cpp; no source of its own.


// Dynamic initializer (0x00687C80, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_00687bb0.cpp; no source of its own.


// Dynamic initializer (0x00687CA0, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_00687bb0.cpp; no source of its own.


// Dynamic initializer (0x00687CE0, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_00687bb0.cpp; no source of its own.


// Dynamic initializer (0x00687D10, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_00687bb0.cpp; no source of its own.


// Dynamic initializer (0x00687D40, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_00687bb0.cpp; no source of its own.


// Dynamic initializer (0x00687D70, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_00687bb0.cpp; no source of its own.


// Dynamic initializer (0x00687D80, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_00687bb0.cpp; no source of its own.


/***********************************************************************************************
 * SmudgeTypeClass::SmudgeTypeClass -- Constructor for smudge type objects.                    *
 *                                                                                             *
 *    This constructor is used to create the smudge type objects. These type objects contain   *
 *    static information about the various smudge types supported in the game.                 *
 *                                                                                             *
 * INPUT:   see below...                                                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/12/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00687DA0)
// A 1 x 1 smudge that is neither crater nor scorch mark, registered in
// SmudgeTypes (ArrayIndex is its position there).

SmudgeTypeClass::SmudgeTypeClass(char const *ini_name) :
    ObjectTypeClass(ini_name),
    ArrayIndex(-1),
    Width(1),
    Height(1),
    Crater(false),
    Burn(false)
{
    AbstractClass::Create_ID();
    unknown_1ff = true;
    unknown_200 = false;
    unknown_201 = false;
    unknown_202 = true;
    unknown_203 = true;
    unknown_205 = false;
    SmudgeTypes.Add(this);
    ArrayIndex = SmudgeTypes.ID(this);
}


/// <summary>
/// Creates a smudge type object without initializing it.
/// The load process uses this constructor to give the object its virtual function table
/// back; every value is about to be overwritten with what the save file holds.
/// </summary>
// (0x00687EB0)
// Load path: installs the vtables only.

SmudgeTypeClass::SmudgeTypeClass(NoInitClass const &noinit) :
    ObjectTypeClass(noinit)
{
}


/// <summary>
/// Destroys this smudge type object.
/// Anything still pointing at this type is detached from it before the type is dropped from
/// the smudge type heap.
/// </summary>
// (0x00687EE0)
// Leaves the notice lists and SmudgeTypes.

SmudgeTypeClass::~SmudgeTypeClass()
{
    Unknown_006ECB90((ObjectClass *)(AbstractClass *)this, true);
    SmudgeTypes.Delete(this);
}


/***********************************************************************************************
 * SmudgeTypeClass::From_Name -- Converts an ASCII name into a smudge type.                    *
 *                                                                                             *
 *    This converts an ASCII name into a smudge type number. This is typically necessary       *
 *    when processing scenario INI files and not used otherwise.                               *
 *                                                                                             *
 * INPUT:   name  -- Pointer to the name to convert.                                           *
 *                                                                                             *
 * OUTPUT:  Returns with the SmudgeType number that matches the name supplied. If no match     *
 *          was found, then SMUDGE_NONE is returned.                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/12/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00687F60)
// Index of the type with this ID (case-insensitive), or -1.

int __fastcall SmudgeTypeClass::From_Name(char const *name)
{
    if (name != 0) {
        for (int index = 0; index < SmudgeTypes.Count(); index++) {
            if (_stricmp(SmudgeTypes[index]->ID, name) == 0) {
                return index;
            }
        }
    }
    return -1;
}


/***********************************************************************************************
 * SmudgeTypeClass::Init -- Performs theater specific initializations.                         *
 *                                                                                             *
 *    Smudge object imagery varies between theaters. This routine will load the appropriate    *
 *    imagery for the theater specified.                                                       *
 *                                                                                             *
 * INPUT:   theater  -- The theater to prepare for.                                            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/12/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00687FB0)
// Loads the theater images of the theater-specific smudge types when the
// theater changes (or a reload is forced).
// Assumptions: unknown_1fc is "Theater="; fullname size 512.

void __fastcall SmudgeTypeClass::Init(int theater)
{
    if (Debug_Map || theater != LastTheater) {
        for (int index = 0; index < SmudgeTypes.Count(); index++) {
            SmudgeTypeClass *smudge = SmudgeTypes[index];
            if (smudge->unknown_1fc) {
                char fullname[512];
                _makepath(fullname, 0, 0, smudge->ID, Theaters[theater].Suffix);
                smudge->ImageData = (void *)MixFileClass::Retrieve(fullname);
            }
        }
    }
}


/***********************************************************************************************
 * SmudgeTypeClass::Create_And_Place -- Creates and places on map, a smudge object.            *
 *                                                                                             *
 *    This routine will, in one motion, create a smudge object and place it upon the map.      *
 *    Since placing a smudge on the map will destroy the object, this routine will leave the   *
 *    smudge object count unchanged. Typically, this routine is used by the scenario editor    *
 *    for creating smudges and placing them on the map.                                        *
 *                                                                                             *
 * INPUT:   cell  -- The cell to place the smudge object.                                      *
 *                                                                                             *
 * OUTPUT:  bool; Was the placement successful?                                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/12/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00688030)
// Creates a smudge of this type at the centre of the cell (no house).

bool SmudgeTypeClass::Create_And_Place(Cell const &cell, HouseClass *house) const
{
    if (new SmudgeClass((SmudgeTypeClass *)this, CoordStruct(cell.X * 256 + 128, cell.Y * 256 + 128, 0), -1) != 0) {
        return true;
    }
    return false;
}


/***********************************************************************************************
 * SmudgeTypeClass::Create_One_Of -- Creates a smudge object of this type.                     *
 *                                                                                             *
 *    This routine will create a smudge object of the appropriate type. Smudge objects are     *
 *    transitory in nature. They exist only from the point of creation until they are given    *
 *    a spot on the map to reside. At that time the map data is updated and the smudge         *
 *    object is destroyed.                                                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to a created smudge object. If none could be created, then  *
 *          NULL is returned.                                                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/12/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x006880A0)
// Creates an unplaced smudge of this type (at the zero coordinate).

ObjectClass *SmudgeTypeClass::Create_One_Of(HouseClass *house) const
{
    return new SmudgeClass((SmudgeTypeClass *)this, unknown_geometry_l, -1);
}


/***********************************************************************************************
 * SmudgeTypeClass::Draw_It -- Renders the smudge image at the coordinate specified.           *
 *                                                                                             *
 *    This routine will draw the smudge overlay image at the coordinate (upper left)           *
 *    specified. The underlying terrain icon is presumed to have already been rendered.        *
 *                                                                                             *
 * INPUT:   x,y   -- Coordinate of the upper left corner of icon to render the smudge object.  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/12/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x006880D0)
// Draws the image at a point: a piece of a multi-cell smudge (row-major in
// Width) is moved to its own cell of the isometric block; the cell supplies
// the drawer and the light level, the height the depth.

void SmudgeTypeClass::Draw_It(Point2D const *point, Rect const *cliprect, int size, int z, Cell const *cell)
{
    void *shape = Get_Image_Data();
    if (shape != 0) {
        Point2D position = *point;
        if (size != 0) {
            int x = size % Width;
            int y = size / Width;
            position.Y -= (y + x) * 15;
            position.X += (y - x) * 60 / 2;
        }
        CellClass &here = Map[*cell];
        int level = here.unknown_10c;
        CC_Draw_Shape(LogicalSurface, (ConvertClass *)here.unknown_34, (ShapeFileStruct const *)shape, 0, position, *cliprect,
                      0xE00, 0, -1 - Unknown_006A2970(z), 0, level, 0, 0, Point2D(0, 0));
    }
}


/// <summary>
/// Fetches this smudge type's settings from the rules.
/// This routine picks up the size of the smudge and whether it counts as a crater or a
/// scorch, and it loads the artwork for the type as well.
/// </summary>
/// <returns>bool; Was an entry for this smudge type found and read?</returns>
// (0x006881B0)
// Reads "Crater", "Burn", "Width" and "Height", then loads the image (the
// theater variant for theater-specific types, else the .SHP file).

bool SmudgeTypeClass::Read_INI(CCINIClass &ini)
{
    if (ObjectTypeClass::Read_INI(ini)) {
        char const *section = ID;
        Crater = ini.Get_Bool(section, "Crater", Crater);
        Burn = ini.Get_Bool(section, "Burn", Burn);
        Width = ini.Get_Int(section, "Width", Width);
        Height = ini.Get_Int(section, "Height", Height);
        char fullname[512];
        if (!unknown_1fc) {
            _makepath(fullname, 0, 0, unknown_1c8, ".SHP");
        } else {
            _makepath(fullname, 0, 0, unknown_1c8, Theaters[unknown_id_source_a3d290->Theater].Suffix);
        }
        ImageData = (void *)MixFileClass::Retrieve(fullname);
        return true;
    }
    return false;
}


/// <summary>
/// Adds this smudge type to the game state checksum.
/// The network sync check uses this to prove that every machine in the game is working
/// from the same rules.
/// </summary>
// (0x006882B0)
// Note: the base called is AbstractTypeClass's (0x00410960), not
// ObjectTypeClass's.

void SmudgeTypeClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractTypeClass::Compute_CRC(crc);
    crc((long)ArrayIndex);
    crc(Crater);
    crc(Burn);
    crc((long)Width);
    crc((long)Height);
}


/// <summary>
/// Reads this smudge type back in from a save game.
/// The object is reconstructed in place so that it regains its virtual function table, and
/// its artwork is fetched again, since pointers into the mix files do not survive a save.
/// </summary>
/// <returns>Returns with S_OK once the object has been rebuilt, otherwise the failure code
/// from the base class.</returns>
// (0x00688310)
// Restores the type in place and reloads its voxel and image.

HRESULT __stdcall SmudgeTypeClass::Load(IStream *stream)
{
    HRESULT result = ObjectTypeClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) SmudgeTypeClass(NoInitClass());
        Fetch_Voxel_Image();
        Fetch_Normal_Image();
        result = S_OK;
    }
    return result;
}


/// <summary>
/// Writes this smudge type out to a save game.
/// The smudge type adds nothing of its own beyond what the object type layer records.
/// </summary>
/// <returns>Returns with S_OK once the object has been written, otherwise the failure code
/// from the base class.</returns>
// (0x00688370)

HRESULT __stdcall SmudgeTypeClass::Save(IStream *stream, BOOL cleardirty)
{
    HRESULT result = ObjectTypeClass::Save(stream, cleardirty);
    if (SUCCEEDED(result)) {
        result = S_OK;
    }
    return result;
}


/// <summary>
/// Fetches the class identifier of this object.
/// The save system asks for this so that it knows which class to construct when the object
/// is read back out of a save file.
/// </summary>
/// <returns>Returns with S_OK, or E_POINTER if no destination was supplied.</returns>
// (0x00688390)

HRESULT __stdcall SmudgeTypeClass::GetClassID(GUID *retval)
{
    if (retval == 0) {
        return E_POINTER;
    }
    *retval = CLSID_SmudgeTypeClass;
    return S_OK;
}


/// <summary>
/// Fetches the smudge type of the specified name.
/// This routine is used while the rules are being read, where a type may be referred to
/// before its own section has been reached.
/// </summary>
/// <returns>Returns with a pointer to the smudge type, which is created and added to the
/// heap if no type of that name exists yet.</returns>
// (0x006883D0)
// The type with this ID, else a new one; null for "<none>" and "none".

SmudgeTypeClass *__fastcall SmudgeTypeClass::Find_Or_Make(char const *name)
{
    if (_stricmp("<none>", name) == 0 || _stricmp("none", name) == 0) {
        return 0;
    }
    for (int index = 0; index < SmudgeTypes.Count(); index++) {
        if (_stricmp(SmudgeTypes[index]->ID, name) == 0) {
            return SmudgeTypes[index];
        }
    }
    return new SmudgeTypeClass(name);
}


// static bool __fastcall SmudgeTypeClass::Create_Scorch(CoordStruct const &coord, int width, int height, bool big) (0x00688460)
// Places a random scorch mark ("Burn" type) that fits at the cell: small
// (1 x 1) ones unless the area is large (width > 60 and height > 50), only
// multi-cell ones when big; any fitting scorch when none matches. Always
// returns false.

bool __fastcall SmudgeTypeClass::Create_Scorch(CoordStruct const &coord, int width, int height, bool big)
{
    Cell cell = Coord_Cell(coord);
    if (cell.X == unknown_geometry_k.X && cell.Y == unknown_geometry_k.Y) {
        return false;
    }
    DynamicVectorClass<SmudgeTypeClass *> scorches;
    for (int index = 0; index < SmudgeTypes.Count(); index++) {
        SmudgeTypeClass *smudge = SmudgeTypes[index];
        if (smudge->Burn && smudge->Can_Place(Coord_Cell(coord), big)) {
            scorches.Add(smudge);
        }
    }
    DynamicVectorClass<SmudgeTypeClass *> fits;
    for (index = 0; index < scorches.Count(); index++) {
        SmudgeTypeClass *smudge = scorches[index];
        if (!big) {
            if ((smudge->Width == 1 && smudge->Height == 1) || (width > 60 && height > 50)) {
                fits.Add(smudge);
            }
        } else if (smudge->Width > 1 && smudge->Height > 1) {
            fits.Add(smudge);
        }
    }
    if (fits.Count() > 0) {
        new SmudgeClass(fits[unknown_id_source_a3d290->Random()(0, fits.Count() - 1)], coord, -1);
    } else if (scorches.Count() > 0) {
        new SmudgeClass(scorches[unknown_id_source_a3d290->Random()(0, scorches.Count() - 1)], coord, -1);
    }
    return false;
}


// static bool __fastcall SmudgeTypeClass::Create_Crater(CoordStruct const &coord, int width, int height, bool big) (0x00688750)
// Places a random crater ("Crater" type) that fits at the cell: small
// (1 x 1) ones unless the area is large (width > 60 and height > 50), only
// multi-cell ones when big; any fitting crater when none matches. Always
// returns false.

bool __fastcall SmudgeTypeClass::Create_Crater(CoordStruct const &coord, int width, int height, bool big)
{
    Cell cell = Coord_Cell(coord);
    if (cell.X == unknown_geometry_k.X && cell.Y == unknown_geometry_k.Y) {
        return false;
    }
    DynamicVectorClass<SmudgeTypeClass *> scorches;
    for (int index = 0; index < SmudgeTypes.Count(); index++) {
        SmudgeTypeClass *smudge = SmudgeTypes[index];
        if (smudge->Crater && smudge->Can_Place(Coord_Cell(coord), big)) {
            scorches.Add(smudge);
        }
    }
    DynamicVectorClass<SmudgeTypeClass *> fits;
    for (index = 0; index < scorches.Count(); index++) {
        SmudgeTypeClass *smudge = scorches[index];
        if (!big) {
            if ((smudge->Width == 1 && smudge->Height == 1) || (width > 60 && height > 50)) {
                fits.Add(smudge);
            }
        } else if (smudge->Width > 1 && smudge->Height > 1) {
            fits.Add(smudge);
        }
    }
    if (fits.Count() > 0) {
        new SmudgeClass(fits[unknown_id_source_a3d290->Random()(0, fits.Count() - 1)], coord, -1);
    } else if (scorches.Count() > 0) {
        new SmudgeClass(scorches[unknown_id_source_a3d290->Random()(0, scorches.Count() - 1)], coord, -1);
    }
    return false;
}


// bool SmudgeTypeClass::Can_Place(Cell const &cell, bool arg2) const (0x00688A40)
// True when every cell of the Width x Height block at the cell lies on the
// map, has no smudge, overlay or blocking flag, is unoccupied (unless arg2)
// and has a tile type that accepts smudges.

bool SmudgeTypeClass::Can_Place(Cell const &cell, bool arg2) const
{
    for (int y = 0; y < Height; y++) {
        for (int x = 0; x < Width; x++) {
            Cell pos = cell + Cell(x, y);
            CellClass &here = Map[pos];
            if (!Map.In_Radar(cell) || here.unknown_11c || here.SmudgeIndex != -1 || here.OverlayIndex != -1) {
                return false;
            }
            if (!arg2 && here.Cell_Building() != 0) {
                return false;
            }
            int tile = here.IsoTileIndex;
            if (tile < 0 || tile >= IsoTileTypes.Count()) {
                tile = 0;
            }
            if (!IsoTileTypes[tile]->unknown_2b0) {
                return false;
            }
        }
    }
    return true;
}


/// <summary>
/// Stamps this smudge type onto the map.
/// This routine lays the smudge across the whole area it covers and flags the affected
/// cells so that anything overlapping them is redrawn.
/// </summary>
/// <param name="origin">The upper left cell of the area the smudge will cover.</param>
/// <remarks>Check the location with Can_Place_Here first -- this routine overwrites
/// whatever it finds.</remarks>
// (0x00688B40)
// Marks every cell of the Width x Height block with this type and the
// cell's piece number (row * Width + column).

void SmudgeTypeClass::Place(Cell const &origin) const
{
    for (int y = 0; y < Height; y++) {
        for (int x = 0; x < Width; x++) {
            Cell pos = origin + Cell(x, y);
            CellClass &here = Map[pos];
            here.SmudgeIndex = ArrayIndex;
            here.SmudgeData = y * Width + x;
            here.Register_For_Redraw();
        }
    }
}


// int SmudgeTypeClass::Kind_Of() const (0x00688BF0)
// Compiler-emitted from shared/smudge_type.hpp in this unit; no source of its own.
