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
// Function comment blocks from mischa85/RedAlert2 392e904 src/smudge.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.
// Parameter names from mischa85/RedAlert2 392e904 src/smudge.cpp where both define the function (names only).
// Local variable names from mischa85/RedAlert2 392e904 src/smudge.cpp where both functions declare the same local types (names only).

// smudge.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/smudge.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/smudge.hpp"
#include "shared/crt_tokens.hpp"


// Header-static dynamic initializers of the unit at 0x006873D0:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x006873D0-0x006875B6); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x00687400, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_006873d0.cpp; no source of its own.


// Dynamic initializer (0x00687420, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_006873d0.cpp; no source of its own.


// Dynamic initializer (0x00687440, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_006873d0.cpp; no source of its own.


// Dynamic initializer (0x00687460, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_006873d0.cpp; no source of its own.


// Dynamic initializer (0x00687480, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_006873d0.cpp; no source of its own.


// Dynamic initializer (0x006874A0, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_006873d0.cpp; no source of its own.


// Dynamic initializer (0x006874E0, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_006873d0.cpp; no source of its own.


// Dynamic initializer (0x00687510, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_006873d0.cpp; no source of its own.


// Dynamic initializer (0x00687540, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_006873d0.cpp; no source of its own.


// Dynamic initializer (0x00687570, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_006873d0.cpp; no source of its own.


// Dynamic initializer (0x00687580, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_006873d0.cpp; no source of its own.


// Dynamic initializer (0x006875A0, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_006873d0.cpp; no source of its own.


/***********************************************************************************************
 * SmudgeClass::SmudgeClass -- Constructor for smudge objects.                                 *
 *                                                                                             *
 *    This is the typical constructor for smudge objects. If the position to place the         *
 *    smudge is not given, then the smudge will be initialized in a limbo state. If the        *
 *    smudge is placed on the map, then this operation causes the smudge object itself to be   *
 *    deleted and special map values updated to reflect the presence of a smudge.              *
 *                                                                                             *
 * INPUT:   type  -- The type of smudge to construct.                                          *
 *                                                                                             *
 *          pos   -- The position to place the smudge. If -1, then the smudge is initialized   *
 *                   into a limbo state.                                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/01/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x006875C0)
// Registers the smudge and places it at the coordinate (unless it is the
// zero coordinate) for the given house; a smudge that cannot be placed is
// removed again (slot 60).
// Assumptions: slot 53 places an object (Unlimbo); the house global at
// 0x007F4230 is read while placing.

SmudgeClass::SmudgeClass(SmudgeTypeClass *type, CoordStruct const &pos, int house) :
    Class(type)
{
    AbstractClass::Create_ID();
    Smudges.Add(this);
    if (pos.X != unknown_geometry_l.X || pos.Y != unknown_geometry_l.Y || pos.Z != unknown_geometry_l.Z) {
        SmudgeClass_ToOwn = house;
        if (!ObjectClass::Unlimbo(&pos, 0)) {
            ObjectClass::Delete_Me();
            return;
        }
        SmudgeClass_ToOwn = -1;
    }
}


/// <summary>
/// Destroys the smudge and removes it from the game.
/// This routine detaches anything that referred to this smudge, lifts it off the map, and
/// drops it from the global smudge list.
/// </summary>
// (0x006876B0)
// Leaves the notice lists and the smudge list; leaves the map (ObjectClass
// slot 52) only when the 0x00A40958 flag is set.

SmudgeClass::~SmudgeClass()
{
    Detach_This_From_All(this, true);
    Smudges.Delete(this);
    if (GameActive) {
        ObjectClass::Limbo();
    }
    Class = 0;
}


/***********************************************************************************************
 * SmudgeClass::Mark -- Marks a smudge down on the map.                                        *
 *                                                                                             *
 *    This routine will place the smudge on the map. If the map cell allows.                   *
 *                                                                                             *
 * INPUT:   mark  -- The type of marking to perform. Only MARK_DOWN is supported.              *
 *                                                                                             *
 * OUTPUT:  bool; Was the smudge marked successfully? Failure occurs if the smudge isn't       *
 *                marked DOWN.                                                                 *
 *                                                                                             *
 * WARNINGS:   The smudge object is DELETED by this routine.                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/22/1994 JLB : Created.                                                                 *
 *   12/23/1994 JLB : Checks low level legality before proceeding.                             *
 *=============================================================================================*/
// (0x00687750)
// After the base handling succeeds for modes 1 and 3, marks the smudge's
// cells (SmudgeTypeClass::Place, when the cells allow it or placement is
// forced) and removes the object (slot 60).

bool SmudgeClass::Mark(int mark)
{
    if (ObjectClass::Mark(mark) && (mark == 1 || mark == 3)) {
        Cell origin = Cell(unknown_slot104());
        if (ScenarioInit > 0 || Debug_Map || Class->Can_Place(origin, true)) {
            Class->Place(origin);
        }
        IsDown = false;
        Delete_Me();
        return true;
    }
    return false;
}


// 0x006877e0: function not yet identified
//
// Body: 3 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void Disown_006877e0(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006877e0 {

void __stdcall Disown_006877e0(int unused_0)
{
    return;
}

}  // namespace seed_006877e0


/***********************************************************************************************
 * SmudgeClass::Read_INI -- Reads smudge data from an INI file.                                *
 *                                                                                             *
 *    This routine is used by the scenario loader to read the smudge data in an INI file and   *
 *    create the appropriate smudge objects on the map.                                        *
 *                                                                                             *
 * INPUT:   buffer   -- Pointer to the INI file staging buffer.                                *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/01/1994 JLB : Created.                                                                 *
 *   07/24/1995 JLB : Sets the smudge data value as well.                                      *
 *=============================================================================================*/
// (0x006877F0)
// Creates the smudges listed in the [Smudge] section ("type,x,y,piece");
// only first pieces (piece 0) are created, the type marks its other cells.

void __fastcall SmudgeClass::Read_INI(CCINIClass &ini)
{
    int count = ini.Entry_Count(SmudgeClass_INI_NAME);
    for (int index = 0; index < count; index++) {
        char const *entry = ini.Get_Entry(SmudgeClass_INI_NAME, index);
        char buffer[128];
        ini.Get_String(SmudgeClass_INI_NAME, entry, 0, buffer, sizeof(buffer));
        int type = SmudgeTypeClass::From_Name(strtok(buffer, ","));
        if (type != -1) {
            char *token = strtok(0, ",");
            if (token != 0) {
                Cell cell;
                short x = atoi(token);
                cell.Y = atoi(strtok(0, ","));
                cell.X = x;
                token = strtok(0, ",");
                if (token == 0 || atoi(token) == 0) {
                    new SmudgeClass(SmudgeTypes[type], CoordStruct(cell.X * 256 + 128, cell.Y * 256 + 128, 0), -1);
                }
            }
        }
    }
}


/***********************************************************************************************
 * SmudgeClass::Write_INI -- Store all the smudge data to the INI database.                    *
 *                                                                                             *
 *    This routine will output all the smudge data to the INI database.                        *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database object.                                     *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00687930)
// Rewrites the [Smudge] section from the first piece of every smudged cell
// of the 512 x 512 map ("type,x,y,piece").

void __fastcall SmudgeClass::Write_INI(CCINIClass &ini)
{
    ini.Clear(SmudgeClass_INI_NAME);
    int index = 0;
    for (int y = 0; y < 512; y++) {
        for (int x = 0; x < 512; x++) {
            CellClass &cell = Map_Cell(Cell((short)x, (short)y));
            if (cell.SmudgeIndex != -1 && cell.SmudgeData == 0) {
                SmudgeTypeClass *type = SmudgeTypes[cell.SmudgeIndex];
                char entry[12];
                char value[128];
                sprintf(entry, "%d", index);
                sprintf(value, "%s,%d,%d,%d", (char const *)type->ID, x, y, cell.SmudgeData);
                ini.Put_String(SmudgeClass_INI_NAME, entry, value);
                index++;
            }
        }
    }
}


/// <summary>
/// Loads this smudge from a save game stream.
/// This routine is part of the IPersistStream interface. Once the base class data has been
/// read, the smudge type reference is remapped to the matching type object in this session.
/// </summary>
/// <returns>Returns with S_OK if the smudge was loaded, otherwise the failure code that the
/// base class reported.</returns>
// (0x00687A00)
// Restores the smudge in place and swizzles its type.

HRESULT __stdcall SmudgeClass::Load(IStream *stream)
{
    HRESULT result = ObjectClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) SmudgeClass(NoInitClass());
        Swizzler.Swizzle((void **)&Class);
    }
    return result;
}


/// <summary>
/// Saves this smudge to a save game stream.
/// This routine is part of the IPersistStream interface. A smudge carries nothing of its
/// own that must be recorded, so the base class does all of the work.
/// </summary>
/// <param name="cleardirty">Should the dirty flag be cleared as a result of saving?</param>
/// <returns>Returns with the result code that the base class reported.</returns>
// (0x00687A60)

HRESULT __stdcall SmudgeClass::Save(IStream *stream, BOOL cleardirty)
{
    return ObjectClass::Save(stream, cleardirty);
}


// ObjectTypeClass *SmudgeClass::Class_Of() const (0x00687A80)
// Compiler-emitted from shared/smudge.hpp in this unit; no source of its own.


// int SmudgeClass::Size_Of() const (0x00687A90)
// Compiler-emitted from shared/smudge.hpp in this unit; no source of its own.


// int SmudgeClass::Kind_Of() const (0x00687AA0)
// Compiler-emitted from shared/smudge.hpp in this unit; no source of its own.


// HRESULT __stdcall SmudgeClass::GetClassID(GUID *clsid) (0x00687AB0)
// Compiler-emitted from shared/smudge.hpp in this unit; no source of its own.


// void SmudgeClass::Draw_It(Point2D const &point, Rect const &window) (0x00687AF0)
// Compiler-emitted from shared/smudge.hpp in this unit; no source of its own.
