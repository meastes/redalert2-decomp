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
// Function comment blocks from mischa85/RedAlert2 392e904 src/overlay.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.
// Parameter names from mischa85/RedAlert2 392e904 src/overlay.cpp where both define the function (names only).

// overlay.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/overlay.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/overlay.hpp"
#include "shared/map_view.hpp"
#include "shared/veinhole.hpp"
#include "shared/anim.hpp"
#include "shared/rules.hpp"
#include "shared/tiberium.hpp"
#include "shared/color_scheme.hpp"
#include "shared/id_source.hpp"
#include "shared/crt_memory.hpp"
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#include "shared/surface.hpp"
#include "shared/lcw_straw.hpp"
#include "shared/buffer_straw.hpp"
#include "shared/lcw_pipe.hpp"
#include "shared/buffer_pipe.hpp"
#include "shared/movie_globals.hpp"
#define SHAPE_FILE_EMPTY_RECT unknown_static_rect
#include "shared/tactical.hpp"
#include "shared/palette.hpp"
#include "shared/text_print.hpp"
#include "shared/anim_type.hpp"
#include "shared/house.hpp"
#include "shared/debug_printf.hpp"


// XSurface::XSurface(int, int) (0x0049F430)
// Inline constructor defined in the shared header.
// No source of its own: emitted by this unit (population-2531).


// Header-static dynamic initializers of the unit at 0x005DA5E0:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x005DA5E0-0x005DA7C6); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x005DA610, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_005da5e0.cpp; no source of its own.


// Dynamic initializer (0x005DA630, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_005da5e0.cpp; no source of its own.


// Dynamic initializer (0x005DA650, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_005da5e0.cpp; no source of its own.


// Dynamic initializer (0x005DA670, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_005da5e0.cpp; no source of its own.


// Dynamic initializer (0x005DA690, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_005da5e0.cpp; no source of its own.


// Dynamic initializer (0x005DA6B0, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_005da5e0.cpp; no source of its own.


// Dynamic initializer (0x005DA6F0, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_005da5e0.cpp; no source of its own.


// Dynamic initializer (0x005DA720, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_005da5e0.cpp; no source of its own.


// Dynamic initializer (0x005DA750, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_005da5e0.cpp; no source of its own.


// Dynamic initializer (0x005DA780, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_005da5e0.cpp; no source of its own.


// Dynamic initializer (0x005DA790, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_005da5e0.cpp; no source of its own.


// Dynamic initializer (0x005DA7B0, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_005da5e0.cpp; no source of its own.


/***********************************************************************************************
 * OverlayClass::OverlayClass -- Overlay object constructor.                                   *
 *                                                                                             *
 *    This is the constructor for a overlay object.                                            *
 *                                                                                             *
 * INPUT:   type  -- The overlay object this is to become.                                     *
 *                                                                                             *
 *          pos   -- The position on the map to place the object.                              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x005DA7D0)
// Registers the overlay and places it at the centre of the cell (unless it
// is the zero cell) for the given house.
// Assumptions: slot 53 places an object (Unlimbo); the house global at
// 0x007E8A8C is read while placing.

OverlayClass::OverlayClass(OverlayTypeClass *ttype, Cell const &pos, int house) :
    Class(ttype)
{
    AbstractClass::Create_ID();
    Overlays.Add(this);
    if (pos.X != unknown_geometry_k.X || pos.Y != unknown_geometry_k.Y) {
        OverlayClass_ToOwn = house;
        CoordStruct coord(pos.X * 256 + 128, pos.Y * 256 + 128, 0);
        ObjectClass::Unlimbo(&coord, 0);
        OverlayClass_ToOwn = -1;
    }
}


/// <summary>
/// Removes the overlay from the game.
/// This routine detaches the overlay from anything that might still be referring to it
/// and then lifts it off the map.
/// </summary>
// (0x005DA8D0)
// Leaves the notice lists and the overlay list; leaves the map (ObjectClass
// slot 52) only when the 0x00A40958 flag is set.

OverlayClass::~OverlayClass()
{
    Detach_This_From_All(this, true);
    Overlays.Delete(this);
    if (GameActive) {
        ObjectClass::Limbo();
    }
    Class = 0;
}


// bool OverlayClass::Mark(MarkType mark) (slot 71, 0x005DA970)
// TibSun overlay.cpp; the rail bridge family (0xE9..0xEC) is RA2's copy of the low bridge family.




#ifndef RA2_ADJACENT_CELL
#define RA2_ADJACENT_CELL
inline Cell Adjacent_Cell(Cell const &cell, int facing)
{
    return cell + AdjacentCell[facing & 7];
}
#endif


/***********************************************************************************************
 * OverlayClass::Mark -- Marks the overlay down on the map.                                    *
 *                                                                                             *
 *    This routine will place the overlay onto the map. The overlay object is deleted by this  *
 *    operation. The map is updated to reflect the presence of the overlay.                    *
 *                                                                                             *
 * INPUT:   mark  -- The type of marking to perform. Only MARK_DOWN is supported.              *
 *                                                                                             *
 * OUTPUT:  bool; Was the overlay successfully marked? Failure occurs if it is not being       *
 *                marked down.                                                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/24/1994 JLB : Created.                                                                 *
 *   12/23/1994 JLB : Checks low level legality before proceeding.                             *
 *=============================================================================================*/
bool OverlayClass::Mark(int mark)
{
    int i;

    if (ObjectClass::Mark(mark)) {
        if (mark == 1 || mark == 3) {
            Cell cell = unknown_slot104();
            CellClass *cellptr = &Map[cell];

            int type = Class->ArrayIndex;

            if (cellptr->Ramp > 4 && type != 0xB2) {
                return false;
            }

            if (type == 0x18 || type == 0x19) {
                if (type == 0x18) {
                    cellptr->Set_Under_Bridge(0, 1);
                } else {
                    cellptr->Set_Under_Bridge(6, 1);
                }
            }

            if (type == 0xED || type == 0xEE) {
                if (type == 0xED) {
                    cellptr->Set_Under_Rail_Bridge(0, 1);
                } else {
                    cellptr->Set_Under_Rail_Bridge(6, 1);
                }
            }

            if (type == 0xA7) {
                if (VeinholeMonsterClass::Can_Monster_Go_Here(cellptr->MapCoords)) {
                    for (int face = 0; face < 8; face++) {
                        CellClass *adj = &cellptr->Adjacent_Cell(face);
                        adj->OverlayIndex = 0xB2;
                        adj->OverlayData = 0;
                    }
                    cellptr->OverlayIndex = 0xA7;
                    cellptr->OverlayData = 0;
                    new VeinholeMonsterClass(&cellptr->MapCoords);
                }
            } else if (type == 0x7E && !ScenarioInit) {
                cellptr->Place_Veins();
            } else if (Class->Land == 9) {
                cellptr->OverlayIndex = Class->ArrayIndex;
                cellptr->OverlayData = 0;
            } else if (Class->Wall) {
                if (cellptr->Is_Clear_To_Build(1, 0, 0)) {
                    cellptr->OverlayIndex = Class->ArrayIndex;
                    cellptr->OverlayData = 0;
                    cellptr->Wall_Update(true);
                    if (!ScenarioInit) {
                        Map.Update_Cell_Zone_Constructively(cellptr->MapCoords);
                        Map.Update_Cell_Subzones(cellptr->MapCoords);
                    }

                    if (OverlayClass_ToOwn != -1) {
                        cellptr->unknown_50 = Map.PendingHouse;
                    }

                    for (int face = 0; face < 8; face++) {
                        cellptr->Adjacent_Cell(face).AdjacentObjectCount++;
                    }
                } else {
                    Delete_Me();
                    return false;
                }
            } else if (Class->ArrayIndex >= 0x7A && Class->ArrayIndex <= 0x7D) {
                int index = Class->ArrayIndex - 0x7A;

                static Cell _type_to_offset[4] = {Cell(0, -1), Cell(0, -1), Cell(-1, 0), Cell(-1, 0)};
                static int _type_to_direction[4] = {4, 4, 2, 2};
                static int _type_to_overlay[4] = {92, 94, 96, 98};
                static int _type_to_join_direction[4] = {6, 2, 4, 0};
                static Cell _offset_fixup[2][3] = {
                    Cell(-1, 0), Cell(0, 0), Cell(1, 0),
                    Cell(0, -1), Cell(0, 0), Cell(0, 1)
                };
                static int _overlay_fixup[2] = {83, 74};
                static int _direction_to_end[4] = {96, 92, 98, 94};

                Cell start_cell = _type_to_offset[index] + cell;
                Cell work_cell = start_cell;
                int direction = _type_to_direction[index];
                bool clear = true;

                for (i = 0; i < 3; i++) {
                    if (Map[work_cell].OverlayIndex != -1) {
                        clear = false;
                    }
                    work_cell = Adjacent_Cell(work_cell, direction);
                }

                work_cell = start_cell;
                if (clear) {
                    for (i = 0; i < 3; i++) {
                        CellClass *cptr = &Map[work_cell];
                        cptr->OverlayIndex = _type_to_overlay[index];
                        cptr->OverlayData = i;
                        cptr->Recalc_Attributes(-1);
                        work_cell = Adjacent_Cell(work_cell, direction);
                    }
                    int join_direction = _type_to_join_direction[index];
                    work_cell = Adjacent_Cell(cell, join_direction);
                    bool found_end = false;
                    int end_overlay = _direction_to_end[join_direction / 2];

                    while (Map.In_Radar(work_cell) && !found_end) {
                        CellClass &check_cell_class = Map[work_cell];
                        if (check_cell_class.OverlayIndex == end_overlay && check_cell_class.OverlayData == 1) {
                            found_end = true;
                        } else {
                            work_cell = Adjacent_Cell(work_cell, join_direction);
                        }
                    }
                    if (found_end) {
                        int fixup_direction = (join_direction - 4) & 7;
                        work_cell = Adjacent_Cell(work_cell, fixup_direction);
                        int length = MAX(abs(work_cell.X - start_cell.X), abs(work_cell.Y - start_cell.Y));
                        for (i = 0; i < length; i++) {
                            for (int j = 0; j < 3; j++) {
                                TPoint2D<short> sum = _offset_fixup[(fixup_direction % 4) / 2][j] + work_cell;
                                CellClass *cptr = &Map[sum];
                                cptr->OverlayIndex = _overlay_fixup[(fixup_direction % 4) / 2] + (Scen->RandomNumber() & 0x03);
                                cptr->OverlayData = j;
                                cptr->Recalc_Attributes(-1);
                            }
                            work_cell = Adjacent_Cell(work_cell, fixup_direction);
                        }
                    }
                }
            } else if (Class->ArrayIndex >= 0xE9 && Class->ArrayIndex <= 0xEC) {
                int index = Class->ArrayIndex - 0xE9;

                static Cell _type_to_offset2[4] = {Cell(0, -1), Cell(0, -1), Cell(-1, 0), Cell(-1, 0)};
                static int _type_to_direction2[4] = {4, 4, 2, 2};
                static int _type_to_overlay2[4] = {223, 225, 227, 229};
                static int _type_to_join_direction2[4] = {6, 2, 4, 0};
                static Cell _offset_fixup2[2][3] = {
                    Cell(-1, 0), Cell(0, 0), Cell(1, 0),
                    Cell(0, -1), Cell(0, 0), Cell(0, 1)
                };
                static int _overlay_fixup2[2] = {214, 205};
                static int _direction_to_end2[4] = {227, 223, 229, 225};

                Cell start_cell = _type_to_offset2[index] + cell;
                Cell work_cell = start_cell;
                int direction = _type_to_direction2[index];
                bool clear = true;

                for (i = 0; i < 3; i++) {
                    if (Map[work_cell].OverlayIndex != -1) {
                        clear = false;
                    }
                    TPoint2D<short> next = Adjacent_Cell(work_cell, direction);
                    work_cell = next;
                }

                work_cell = start_cell;
                if (clear) {
                    for (i = 0; i < 3; i++) {
                        CellClass *cptr = &Map[work_cell];
                        cptr->OverlayIndex = _type_to_overlay2[index];
                        cptr->OverlayData = i;
                        cptr->Recalc_Attributes(-1);
                        work_cell = Adjacent_Cell(work_cell, direction);
                    }
                    int join_direction = _type_to_join_direction2[index];
                    work_cell = Adjacent_Cell(cell, join_direction);
                    bool found_end = false;
                    int end_overlay = _direction_to_end2[join_direction / 2];

                    while (Map.In_Radar(work_cell) && !found_end) {
                        CellClass &check_cell_class = Map[work_cell];
                        if (check_cell_class.OverlayIndex == end_overlay && check_cell_class.OverlayData == 1) {
                            found_end = true;
                        } else {
                            work_cell = Adjacent_Cell(work_cell, join_direction);
                        }
                    }
                    if (found_end) {
                        int fixup_direction = (join_direction - 4) & 7;
                        work_cell = Adjacent_Cell(work_cell, fixup_direction);
                        int length = MAX(abs(work_cell.X - start_cell.X), abs(work_cell.Y - start_cell.Y));
                        for (i = 0; i < length; i++) {
                            for (int j = 0; j < 3; j++) {
                                TPoint2D<short> sum = _offset_fixup2[(fixup_direction % 4) / 2][j] + work_cell;
                                CellClass *cptr = &Map[sum];
                                cptr->OverlayIndex = _overlay_fixup2[(fixup_direction % 4) / 2] + (Scen->RandomNumber() & 0x03);
                                cptr->OverlayData = j;
                                cptr->Recalc_Attributes(-1);
                            }
                            work_cell = Adjacent_Cell(work_cell, fixup_direction);
                        }
                    }
                }
            } else {
                bool clear;
                if (!ScenarioInit) {
                    if (Class == Rules->WaterCrateImg) {
                        clear = cellptr->Is_Clear_To_Move(5, false, false, -1, 0, -1, true);
                    } else if (Class == Rules->CrateImg || Class == Rules->WoodCrateImg) {
                        clear = cellptr->Is_Clear_To_Move(1, false, false, -1, 0, -1, true);
                    } else {
                        if (Class->ArrayIndex == 0x18 || Class->ArrayIndex == 0x19 ||
                                Class->ArrayIndex == 0xED || Class->ArrayIndex == 0xEE) {
                            clear = true;
                        } else {
                            if (Class->ArrayIndex == 0x66) {
                                clear = cellptr->Is_Clear_To_Move(1, true, true, -1, 0, -1, true);
                                if (cellptr->Ramp > 4) clear = false;
                            } else {
                                clear = cellptr->Is_Clear_To_Move(1, true, true, -1, 0, -1, true);
                            }
                        }
                    }
                } else {
                    clear = true;
                }

                if ((ScenarioInit || cellptr->OverlayIndex == -1 || !OverlayTypes[cellptr->OverlayIndex]->Overrides) && clear) {

                    cellptr->OverlayIndex = Class->ArrayIndex;
                    if (type != 0x18 && type != 0x19 && type != 0xED && type != 0xEE) {
                        cellptr->OverlayData = 0;
                    }

                    if (Class->Land == 5) {
                        cellptr->OverlayData = 1;
                        cellptr->Tiberium_Adjust(false);
                    }

                    if (Class->Crate) {
                        cellptr->OverlayData = 0xFF;
                    }
                }

                if (Class->CellAnim != 0) {
                    CoordStruct coord = Coord;
                    coord.Z = Map.Get_Height_GL(CoordStruct(Coord));
                    AnimClass *aptr = new AnimClass(Class->CellAnim, coord + CoordStruct(0x180, 0x180, 0), 0, 1, 0x600, 0);

                    int tibtype = cellptr->Tiberium_Type_Here();
                    if (tibtype != -1) {
                        TiberiumClass *tiberium = Tiberiums[tibtype];
                        if (tiberium != 0) {
                            aptr->unknown_bc = ColorSchemes[tiberium->unknown_c0]->Converter;
                            aptr->AlternativeBrightness = cellptr->Brightness;
                        }
                    }
                }
            }

            cellptr->Recalc_Attributes(-1);

            IsDown = false;
            IsInLimbo = true;

            Delete_Me();
            return true;
        }
    }
    return false;
}


// atexit thunk of a function-local Cell static (0x005DB630, _$E45, only returns): emitted by OverlayClass_Mark_005da970.cpp; no source of its own.


// atexit thunk of a function-local Cell static (0x005DB640, _$E44, only returns): emitted by OverlayClass_Mark_005da970.cpp; no source of its own.


// atexit thunk of a function-local Cell static (0x005DB650, _$E43, only returns): emitted by OverlayClass_Mark_005da970.cpp; no source of its own.


// atexit thunk of a function-local Cell static (0x005DB660, _$E42, only returns): emitted by OverlayClass_Mark_005da970.cpp; no source of its own.


/***********************************************************************************************
 * OverlayClass::Read_INI -- Reads the overlay data from an INI file.                          *
 *                                                                                             *
 *    This routine is used to load a scenario's overlay data. The overlay objects are read     *
 *    from the INI file and then created on the map.                                           *
 *                                                                                             *
 * INPUT:   buffer   -- Pointer to the INI file staging buffer.                                *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Requires that all the buildings be placed first, so the scan for assigning wall *
 *             ownership to the nearest building will work.                                    *
 * HISTORY:                                                                                    *
 *   09/01/1994 JLB : Created.                                                                 *
 *   07/24/1995 JLB : Specifically forbid manual crates in multiplayer scenarios.              *
 *=============================================================================================*/
// (0x005DB670)
// Reads [OverlayPack] (an overlay index per cell, LCW compressed) and
// [OverlayDataPack] (the overlay data byte per cell) of a scenario through a
// scratch 640 x 400 surface; every overlay with an image is created, and
// the wall and ore data bytes are restored.

void Process_Deferred_Deletion();            // 0x006ECF10

void __fastcall OverlayClass::Read_INI(CCINIClass &ini)
{
    if (NewINIFormat > 1) {
        BSurface surface(640, 400, 2);
        surface.Fill(0);
        int length = ini.Get_UUBlock("OverlayPack", surface.Lock(), surface.Get_Width() * surface.Get_Height() * surface.Bytes_Per_Pixel());
        if (length > 0) {
            BufferStraw buffer_straw(surface.Lock(), length);
            LCWStraw lcw_straw(LCWStraw::DECOMPRESS, 0x2000);
            lcw_straw.Straw::Get_From(&buffer_straw);
            for (int y = 0; y < 512; y++) {
                for (int x = 0; x < 512; x++) {
                    Cell cell;
                    cell.X = x;
                    cell.Y = y;
                    unsigned int overlay = 0xFFFFFFFF;
                    lcw_straw.Get(&overlay, 1);
                    if (overlay != 0xFFFFFFFF) {
                        overlay &= 0xFF;
                        if (overlay != 0xFFFFFFFF) {
                            if ((OverlayTypes[overlay]->Get_Image_Data() || OverlayTypes[overlay]->CellAnim) && (!unknown_game_mode_a3d298 || !OverlayTypes[overlay]->Crate)) {
                                if (Map.In_Radar(cell)) {
                                    unsigned char data = Map_Cell(cell).OverlayData;
                                    new OverlayClass(OverlayTypes[overlay], cell, -1);
                                    if (overlay == 0x18 || overlay == 0x19 || overlay == 0xED || overlay == 0xEE) {
                                        Map_Cell(cell).OverlayData = data;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            surface.Unlock();
        }
        surface.Unlock();
        int width = surface.Get_Width();
        int height = surface.Get_Height();
        length = ini.Get_UUBlock("OverlayDataPack", surface.Lock(), height * width);
        if (length > 0) {
            BufferStraw data_buffer_straw(surface.Lock(), length);
            LCWStraw data_lcw_straw(LCWStraw::DECOMPRESS, 0x2000);
            data_lcw_straw.Straw::Get_From(&data_buffer_straw);
            for (int y = 0; y < 512; y++) {
                for (int x = 0; x < 512; x++) {
                    Cell cell((short)x, (short)y);
                    unsigned char data = 0;
                    data_lcw_straw.Get(&data, 1);
                    if (Map.In_Radar(cell)) {
                        Map_Cell(cell).OverlayData = data;
                    }
                }
            }
            surface.Unlock();
        }
        surface.Unlock();
    }
    Process_Deferred_Deletion();
}


/// <summary>
/// Stores the map's overlay layer into the INI database.
/// This routine is used when a scenario is saved. The overlay and overlay data of every
/// cell are compressed into the OverlayPack and OverlayDataPack sections, replacing
/// whatever the database held before.
/// </summary>
// (0x005DBA30)
// Rewrites [OverlayPack] and [OverlayDataPack]: the overlay index and the
// overlay data byte of every cell of the 512 x 512 map, LCW compressed into
// the hidden surface's memory and stored as UU blocks.
void __fastcall OverlayClass::Write_INI(CCINIClass &ini)
{
    ini.Clear(OverlayClass_INI_NAME);
    ini.Clear("OverlayPack");

    BufferPipe bpipe(AlternateSurface->Lock(), AlternateSurface->Get_Width() * AlternateSurface->Get_Height());
    LCWPipe comppipe(LCWPipe::COMPRESS, 0x2000);

    comppipe.Put_To(&bpipe);

    int total = 0;
    int y;
    for (y = 0; y < 512; y++) {
        for (int x = 0; x < 512; x++) {
            total += comppipe.Put(&Map_Cell(Cell((short)x, (short)y)).OverlayIndex, 1);
        }
    }
    if (total) {
        ini.Put_UUBlock("OverlayPack", AlternateSurface->Lock(), total);
        AlternateSurface->Unlock();
    }
    AlternateSurface->Unlock();

    ini.Clear("OverlayDataPack");

    BufferPipe databpipe(AlternateSurface->Lock(), AlternateSurface->Get_Width() * AlternateSurface->Get_Height());
    LCWPipe datacomppipe(LCWPipe::COMPRESS, 0x2000);

    datacomppipe.Put_To(&databpipe);

    total = 0;
    for (y = 0; y < 512; y++) {
        for (int x = 0; x < 512; x++) {
            total += datacomppipe.Put(&Map_Cell(Cell((short)x, (short)y)).OverlayData, 1);
        }
    }
    if (total) {
        ini.Put_UUBlock("OverlayDataPack", AlternateSurface->Lock(), total);
        AlternateSurface->Unlock();
    }
    AlternateSurface->Unlock();
}


/// <summary>
/// Loads this overlay from the supplied stream.
/// The overlay is rebuilt in place and its type class pointer is remapped so that it
/// refers to the running game's overlay type heap rather than the saved one.
/// </summary>
/// <returns>Returns with S_OK if the overlay was read, otherwise an error code.</returns>
// (0x005DBC80)
// Restores the overlay in place and swizzles its type.

HRESULT __stdcall OverlayClass::Load(IStream *stream)
{
    HRESULT result = ObjectClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) OverlayClass(NoInitClass());
        Swizzler.Swizzle((void **)&Class);
    }
    return result;
}


class Target_5dbce0 { public: unsigned int target(unsigned int p0, unsigned int p1, unsigned int p2); };
class Leaf_5dbce0 { public: unsigned int get(unsigned int a1, unsigned int a2, unsigned int a3); };
unsigned int Leaf_5dbce0::get(unsigned int a1, unsigned int a2, unsigned int a3) { return ((Target_5dbce0*)(a2))->target((unsigned int)(a1), (unsigned int)(a2), (unsigned int)(a3)); }


// void OverlayClass::Editor_Draw_It(Point2D const &point, Rect const &window) (0x005DBD00)
// Draws the overlay's image (a pixel point and a clip rectangle are passed
// as addresses): ore uses its type's colour scheme at a fixed brightness,
// veins the owner's, and everything else the cell's drawer and light level.

// The index of an overlay type in OverlayTypes (the list's ID lookup, expanded here).
inline int Overlay_Type_Index(OverlayTypeClass *type)
{
    for (int index = 0; index < OverlayTypes.Count(); index++) {
        if (OverlayTypes[index] == type) {
            return index;
        }
    }
    return -1;
}

int __fastcall Tactical_Z_Lepton_To_Pixel(int distance);              // 0x006A2970
extern int unknown_int_839634;                              // 0x00839634
extern short unknown_short_8325d0;                          // 0x008325D0

/// <summary>
/// Draws this overlay onto the tactical map.
/// This routine handles the three flavors of overlay separately. Tiberium is drawn with
/// the color scheme of the tiberium it belongs to, veins with the player's own scheme,
/// and every other overlay with the drawer of the cell it occupies.
/// </summary>
/// <param name="point">The pixel location to draw the overlay at.</param>
/// <param name="cliprect">The clipping rectangle to draw within.</param>
void OverlayClass::Editor_Draw_It(Point2D const &point, Rect const &cliprect)
{
    void *shape = Class->Get_Image_Data();
    int height = Tactical_Z_Lepton_To_Pixel(Map.Get_Height_GL(CoordStruct(Coord)));
    int overlay = Overlay_Type_Index(Class);
    Point2D position = Point2D(point.X, point.Y + Draw_Offset(overlay).Y);
    position.Y += unknown_int_839634;
    if (Class->Tiberium) {
        TiberiumClass *tiberium = Tiberiums[To_Tiberium(Overlay_Type_Index(Class))];
        ColorScheme *scheme = ColorSchemes[tiberium->unknown_c0];
        shape = OverlayTypes[tiberium->Image->ArrayIndex]->Get_Image_Data();
        if (!shape && Class->CellAnim) {
            shape = Class->CellAnim->Get_Image_Data();
        }
        CC_Draw_Shape(LogicalSurface, scheme->Converter, (ShapeFileStruct const *)shape, 0, position, cliprect,
                      0xE00, 0, -2 - height, 0, 1000, 0, 0, Point2D(0, 0));
    } else if (Class->IsVeins) {
        if (Class->ArrayIndex == 0xA7) {
            position.Y -= 0x26;
        }
        Point2D origin(0, 0);
        CC_Draw_Shape(LogicalSurface, ColorSchemes[PlayerPtr->ColorSchemeIndex]->Converter, (ShapeFileStruct const *)shape, 0, position, cliprect,
                      0xE00, 0, -2 - height, 0, Map[CoordStruct(Coord)].TileBrightness, 0, 0, origin);
    } else {
        CellClass *drawn;
        if (Map.In_Radar(Coord_Cell(CoordStruct(Coord)))) {
            drawn = &Map[CoordStruct(Coord)];
        } else if (Map.In_Radar(TacticalMap->Pixel_To_Cell(position))) {
            drawn = &Map[TacticalMap->Pixel_To_Cell(position)];
        } else {
            drawn = &Map[Cell(1, unknown_short_8325d0)];
        }
        CC_Draw_Shape(LogicalSurface, (ConvertClass *)drawn->Drawer, (ShapeFileStruct const *)shape, 0, position, cliprect,
                      0xE00, 0, -2 - height, 0, drawn->TileBrightness, 0, 0, Point2D(0, 0));
    }
}


// static Point2D __fastcall OverlayClass::Draw_Offset(int overlay) (0x005DC050)
// Raises ore, walls, crates and overlay 0x7E by 12 pixels, land type 9
// overlays and overlay 0x7E by one more.
// Assumptions: the purpose (image draw offset) is a hypothesis.

Point2D __fastcall OverlayClass::Draw_Offset(int overlay)
{
    OverlayTypeClass *type = OverlayTypes[overlay];
    Point2D offset(0, 0);
    if (type->Tiberium || type->Wall || type->ArrayIndex == 0x7E || type->Crate) {
        offset.Y = -12;
    }
    if (type->Land == 9) {
        offset.Y--;
    }
    if (overlay == 0x7E) {
        offset.Y--;
    }
    return offset;
}


// static int __fastcall OverlayClass::To_Tiberium(int overlay) (0x005DC0B0)
// The tiberium type whose overlay range (image, then image variants)
// contains the overlay; -1 for non-tiberium overlays, 0 with a debug
// message when no tiberium claims it.


int __fastcall OverlayClass::To_Tiberium(int overlay)
{
    OverlayTypeClass *type;
    if (overlay != -1 && (type = OverlayTypes[overlay])->Tiberium) {
        for (int index = 0; index < Tiberiums.Count(); index++) {
            TiberiumClass *tiberium = Tiberiums[index];
            int first = tiberium->Image->ArrayIndex;
            if ((overlay >= first && overlay < first + tiberium->NumFrames) ||
                (overlay >= first + tiberium->NumFrames && overlay < first + tiberium->NumFrames + tiberium->NumImages)) {
                return tiberium->ArrayIndex;
            }
        }
        Debug_Printf("Overlay %s not really tiberium\n", type->Name);
        return 0;
    }
    return -1;
}


/// <summary>
/// Fetches the type class of this overlay.
/// </summary>
/// <returns>Returns with a pointer to the type class this overlay was made from.</returns>
// (0x005DC170)

ObjectTypeClass *OverlayClass::Class_Of() const
{
    return Class;
}


/// <summary>
/// Repairs the vein overlay after a scenario has been loaded.
/// This routine is called once the map cells are in place. The saved veins are
/// stripped back to bare ground and the solid ones are then placed again, so that the
/// vein network ends up consistent with the terrain it grew over.
/// </summary>
// (0x005DC180)
// Removes overlay 0x7E from every cell, remembering the cells whose
// overlay data was at least 0x30, then runs the 0x0047A670 / 0x0047AD00
// pair on those cells in reverse order.
// Assumptions: the map iterator names (0x0055A550 reset, 0x0055A490 next).

void OverlayClass::Post_Read_Vein_Fixups()
{
    Map.Reset_Iterator();
    CellClass *cell = Map.Iterate();
    DynamicVectorClass<CellClass *> cells;
    cells.Set_Growth_Step(1000);
    while (cell != 0) {
        if (cell->OverlayIndex == 0x7E) {
            if (cell->OverlayData >= 0x30) {
                cells.Add(cell);
            }
            cell->OverlayIndex = -1;
            cell->OverlayData = 0;
        }
        cell = Map.Iterate();
    }
    for (int index = cells.Count() - 1; index >= 0; index--) {
        CellClass *dense = cells[index];
        if (dense->Can_Place_Veins()) {
            dense->Place_Veins();
        }
    }
}


// int OverlayClass::Size_Of() const (0x005DC290)
// Compiler-emitted from shared/overlay.hpp in this unit; no source of its own.


// HRESULT __stdcall OverlayClass::GetClassID(GUID *clsid) (0x005DC2A0)
// Compiler-emitted from shared/overlay.hpp in this unit; no source of its own.


// int OverlayClass::Kind_Of() const (0x005DC2E0)
// Compiler-emitted from shared/overlay.hpp in this unit; no source of its own.


// void OverlayClass::Draw_It(Point2D const &point, Rect const &window) (0x005DC2F0)
// Compiler-emitted from shared/overlay.hpp in this unit; no source of its own.
