/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/
// Function comment blocks from mischa85/RedAlert2 392e904 src/isotile.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.
// Parameter names from mischa85/RedAlert2 392e904 src/isotile.cpp where both define the function (names only).

// isotile.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/isotile.json carries the addresses and the rows that
// stay in their own files.

// Header-static dynamic initializers of the unit at 0x00527890:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x00527890-0x00527A76); the other
// drafts of this plan are emitted by these includes.
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"


// Dynamic initializer (0x005278C0, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_00527890.cpp; no source of its own.


// Dynamic initializer (0x005278E0, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_00527890.cpp; no source of its own.


// Dynamic initializer (0x00527900, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_00527890.cpp; no source of its own.


// Dynamic initializer (0x00527920, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_00527890.cpp; no source of its own.


// Dynamic initializer (0x00527940, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_00527890.cpp; no source of its own.


// Dynamic initializer (0x00527960, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_00527890.cpp; no source of its own.


// Dynamic initializer (0x005279A0, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_00527890.cpp; no source of its own.


// Dynamic initializer (0x005279D0, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_00527890.cpp; no source of its own.


// Dynamic initializer (0x00527A00, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_00527890.cpp; no source of its own.


// Dynamic initializer (0x00527A30, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_00527890.cpp; no source of its own.


// Dynamic initializer (0x00527A40, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_00527890.cpp; no source of its own.


// Dynamic initializer (0x00527A60, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_00527890.cpp; no source of its own.




#include "shared/isometric_tile.hpp"
#include "shared/isometric_tile_type.hpp"
#include "shared/cell.hpp"
#include "shared/map_view.hpp"
#include "shared/anim.hpp"


/// <summary>
/// Marks this tile up or down on the map.
/// This routine stamps the tile's terrain -- tile index, subtile, ramp and height -- into
/// every cell it covers, or lifts that data back out again. Marking down also clears any
/// smudge or overlay those cells were carrying and repairs the neighboring LAT edges.
/// </summary>
/// <param name="mark">The marking operation to perform.</param>
/// <returns>bool; Was the tile marked?</returns>
/// <remarks>Marking the tile down destroys it. The terrain now lives in the cells, so the
/// tile must not be referred to again after such a call.</remarks>
// (0x00527A80)
// Takes the tile off the map (mark 0) or puts it on (marks 1 and 3): for
// every cell of the type's grid that has an image tile, the cell's tile
// index, sub-tile and height are reset or set, its smudge and overlay are
// cleared (removing the overlay's cell animation sitting on the cell) and the
// cell and its four straight neighbours are recomputed. Placing a tile then
// deletes the object (the tile lives on in the cells).

bool IsometricTileClass::Mark(int mark)
{
    IsoTileImageStruct *image = (IsoTileImageStruct *)Get_Image_Data();
    if (image == 0 || !ObjectClass::Mark(mark)) {
        return false;
    }
    for (int row = 0; row < Type->unknown_2b8; row++) {
        for (int column = 0; column < Type->unknown_2b4; column++) {
            Cell cell = XY_Cell(column, row) + unknown_slot104();
            if (!Map.In_Radar(cell)) {
                continue;
            }
            CellClass &here = Map[cell];
            int index = Type->unknown_2b4 * row + column;
            if (image->Tiles[index] == 0) {
                continue;
            }
            if (mark == 0) {
                if (here.IsoTileIndex == Type->ArrayIndex && (unsigned char)here.unknown_11a == index) {
                    here.IsoTileIndex = 0xFFFF;
                    here.unknown_11a = 0;
                    here.unknown_11b -= ((IsoTileCellStruct *)image->Tiles[index])->unknown_28;
                }
            } else if (mark == 1 || mark == 3) {
                if (Type->ArrayIndex == IsometricTileTypeClass_ClearTile) {
                    here.IsoTileIndex = 0xFFFF;
                    here.unknown_11a = 0;
                } else {
                    here.IsoTileIndex = Type->ArrayIndex;
                    here.unknown_11a = index;
                    here.unknown_11c = Type->Ramp_Type(index);
                    if (here.IsoTileIndex == IsometricTileTypeClass_SlopeSetPieces + 5) {
                        switch ((unsigned char)here.unknown_11a) {
                        case 0:
                        case 3:
                        case 6:
                        case 9:
                            here.IsoTileIndex = IsometricTileTypeClass_RampStart + 1;
                            here.unknown_11a = 0;
                        }
                    }
                    if (here.IsoTileIndex == unknown_tileset_a52ee8 + 5) {
                        switch ((unsigned char)here.unknown_11a) {
                        case 0:
                        case 3:
                        case 6:
                        case 9:
                            here.IsoTileIndex = unknown_tileset_a52eec + 1;
                            here.unknown_11a = 0;
                        }
                    }
                    if (here.IsoTileIndex == IsometricTileTypeClass_SlopeSetPieces + 8 && (unsigned char)here.unknown_11a <= 3) {
                        here.IsoTileIndex = IsometricTileTypeClass_RampStart;
                        here.unknown_11a = 0;
                    }
                    if (here.IsoTileIndex == unknown_tileset_a52ee8 + 8 && (unsigned char)here.unknown_11a <= 3) {
                        here.IsoTileIndex = unknown_tileset_a52eec;
                        here.unknown_11a = 0;
                    }
                }
                here.SmudgeIndex = -1;
                here.SmudgeData = 0;
                if (here.OverlayIndex >= 0x1B && here.OverlayIndex < 0x26) {
                    here.unknown_140 &= ~0x20000;
                    CoordStruct spot = here.Center_Coord() + CoordStruct(0x180, 0x180, 0);
                    for (int i = 0; i < Anims.Count(); i++) {
                        AnimClass *anim = Anims[i];
                        if (anim->Class == unknown_overlay_types_a35de8[here.OverlayIndex]->unknown_26c) {
                            CoordStruct coord = anim->Coord;
                            if (coord == spot) {
                                delete anim;
                                break;
                            }
                        }
                    }
                }
                here.OverlayIndex = -1;
                here.OverlayData = 0;
                here.unknown_11b += ((IsoTileCellStruct *)image->Tiles[index])->unknown_28;
                here.Fixup_LAT();
                here.Adjacent_Cell(0).Fixup_LAT();
                here.Adjacent_Cell(2).Fixup_LAT();
                here.Adjacent_Cell(4).Fixup_LAT();
                here.Adjacent_Cell(6).Fixup_LAT();
            }
            here.Recalc_Attributes(-1);
        }
    }
    if (mark == 1 || mark == 3) {
        unknown_74 = false;
        delete this;
    }
    return true;
}


/// <summary>
/// Creates an isometric tile of the type specified.
/// The tile is stamped onto the map straight away when a real cell is supplied, and is
/// added to the global isometric tile list either way.
/// </summary>
/// <param name="cell">The cell to place the tile at, or CELL_NONE to leave it in limbo.</param>
// (0x00527ED0)
// A tile of the given type; unless the cell is the "no cell" value it is
// placed at the middle of the cell by slot 53 (qualified, expanded inline).

IsometricTileClass::IsometricTileClass(int type, Cell const &cell) :
    Type(IsoTileTypes[type])
{
    if (cell != unknown_geometry_k) {
        CoordStruct coord(cell.X * 256 + 128, cell.Y * 256 + 128, 0);
        IsometricTileClass::Unlimbo(&coord, 0);
    }
    IsoTiles.Add(this);
}


/// <summary>
/// Destroys this isometric tile.
/// The tile is detached from everything that refers to it, lifted back off the map, and
/// dropped from the isometric tile list.
/// </summary>
// (0x00527FD0)
// Leaves the map (slot 52), then the tile list and the object list.

IsometricTileClass::~IsometricTileClass()
{
    Detach_This_From_All(this, true);
    IsometricTileClass::Limbo();
    Type = 0;
    IsoTiles.Delete(this);
    AbstractTypePtrTracker.Delete(this);
}


/// <summary>
/// Loads this tile from the stream specified.
/// The tile is rebuilt in place and its type pointer is put back through the swizzle
/// manager so that it refers to the tile type as loaded by this session.
/// </summary>
/// <returns>Returns with S_OK if the tile was read, otherwise the failure code.</returns>
// (0x005280E0)

HRESULT __stdcall IsometricTileClass::Load(IStream *stream)
{
    HRESULT result = ObjectClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) IsometricTileClass(NoInitClass());
        Swizzler.Swizzle((void **)&Type);
    }
    return result;
}


/// <summary>
/// Saves this tile to the stream specified.
/// This routine is part of the persistence contract used by the game's save system.
/// </summary>
/// <param name="cleardirty">Should the dirty flag be cleared once the tile is written?</param>
/// <returns>Returns with S_OK if the tile was written, otherwise the failure code.</returns>
// (0x00528140)

HRESULT __stdcall IsometricTileClass::Save(IStream *stream, BOOL cleardirty)
{
    HRESULT result = AbstractClass::Save(stream, cleardirty);
    if (SUCCEEDED(result)) {
        result = S_OK;
    }
    return result;
}


/// <summary>
/// Places this tile onto the map at the coordinate specified.
/// This routine will stamp the tile's terrain data down into every cell it covers.
/// </summary>
/// <returns>bool; Was the tile placed onto the map?</returns>
/// <remarks>Placing the tile down destroys it. Do not refer to the tile after this call.</remarks>
// (0x00528160)
// Places the tile at the coordinate and marks it mapped (slot 71).

bool IsometricTileClass::Unlimbo(CoordStruct const *coord, int dir)
{
    unknown_76 = false;
    Set_Coord(*coord);
    return Mark(1);
}


/// <summary>
/// Removes this tile from the map.
/// This routine will unselect the tile, break every attachment to it, and lift its
/// terrain data back out of the cells it was covering.
/// </summary>
/// <returns>bool; Was the tile removed from the map?</returns>
// (0x00528190)
// Takes the tile off the map (only while the 0x00A40958 flag is set).

bool IsometricTileClass::Limbo()
{
    if (GameActive && !unknown_76) {
        Unselect();
        unknown_slot54(true);
        Mark(0);
        unknown_76 = true;
        unknown_75 = false;
        return true;
    }
    return false;
}


// int IsometricTileClass::Size_Of() const (0x005281E0)
// Compiler-emitted from shared/isometric_tile.hpp in this unit; no source of its own.


// int IsometricTileClass::Kind_Of() const (0x005281F0)
// Compiler-emitted from shared/isometric_tile.hpp in this unit; no source of its own.


// HRESULT __stdcall IsometricTileClass::GetClassID(GUID *clsid) (0x00528200)
// Compiler-emitted from shared/isometric_tile.hpp in this unit; no source of its own.


// ObjectTypeClass *IsometricTileClass::Class_Of() const (0x00528240)
// Compiler-emitted from shared/isometric_tile.hpp in this unit; no source of its own.


// void IsometricTileClass::Draw_It(Point2D const &point, Rect const &window) (0x00528250)
// Compiler-emitted from shared/isometric_tile.hpp in this unit; no source of its own.
