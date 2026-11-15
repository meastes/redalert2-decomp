/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 * Copyright 2026 ra2-decomp contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/
// Function comment blocks from mischa85/RedAlert2 392e904 src/tube.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// tube.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/tube.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ADD
#define VECTOR_IMPLEMENT_DYNAMIC_DELETE_OBJECT
#define VECTOR_IMPLEMENT_DYNAMIC_DELETE_INDEX
#include "shared/tube.hpp"
#include "shared/vector_class.hpp"
#include "shared/map_view.hpp"
#include "shared/map_cell.hpp"
#include "shared/tube_data.hpp"
#include "shared/ccini_class.hpp"
#include "shared/wwcrc_engine.hpp"


// Header-static dynamic initializers of the unit at 0x006EEFE0:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x006EEFE0-0x006EF1C6); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x006EF010, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_006eefe0.cpp; no source of its own.


// Dynamic initializer (0x006EF030, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_006eefe0.cpp; no source of its own.


// Dynamic initializer (0x006EF050, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_006eefe0.cpp; no source of its own.


// Dynamic initializer (0x006EF070, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_006eefe0.cpp; no source of its own.


// Dynamic initializer (0x006EF090, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_006eefe0.cpp; no source of its own.


// Dynamic initializer (0x006EF0B0, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_006eefe0.cpp; no source of its own.


// Dynamic initializer (0x006EF0F0, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_006eefe0.cpp; no source of its own.


// Dynamic initializer (0x006EF120, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_006eefe0.cpp; no source of its own.


// Dynamic initializer (0x006EF150, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_006eefe0.cpp; no source of its own.


// Dynamic initializer (0x006EF180, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_006eefe0.cpp; no source of its own.


// Dynamic initializer (0x006EF190, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_006eefe0.cpp; no source of its own.


// Dynamic initializer (0x006EF1B0, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_006eefe0.cpp; no source of its own.


/// <summary>
/// Creates a tunnel and adds it to the tunnel list.
/// The tunnel starts out with no length at all -- it enters and leaves at the same cell
/// until directions are added to it. The cell it is entered from is stamped with this
/// tunnel's index so that anything standing there can find its way underground.
/// </summary>
/// <param name="cell">The cell that the tunnel is entered from.</param>
/// <param name="dir">The facing that the tunnel is entered by.</param>
// (0x006EF1D0)
// (the first Tube source: its vector macros are the unit's, the others' come after the first include)
// A tunnel from the entry cell: no directions yet, listed and indexed by the entry cell.

TubeClass::TubeClass(Cell const &enter, int facing) : EnterCell(enter), ExitCell(enter), Facing(facing), unknown_1c0(0)
{
    Create_ID();
    for (int index = 0; index < 100; index++) {
        Directions[index] = -1;
    }
    unknown_TubeClass_list_008661c8.Add(this);
    if (enter.X != 0 || enter.Y != 0) {
        CellClass &cell = Map[enter];
        int index = Tube_Index(this);
        cell.unknown_116 = index;
    }
}


/// <summary>
/// Removes this tunnel from the game.
/// Everything that was holding on to this tunnel is told to let go of it, and the cell
/// the tunnel was entered from forgets about it as well.
/// </summary>
// (0x006EF2D0)
// Forgets the tube: clears the entry cell's tube index when it is this tube's, leaves the list.

TubeClass::~TubeClass()
{
    Detach_This_From_All(this, true);
    CellClass *cell = &Map[EnterCell];
    if (cell != 0) {
        int index = Tube_Index(this);
        if (cell->unknown_116 == index) {
            cell->unknown_116 = -1;
        }
    }
    unknown_TubeClass_list_008661c8.Delete(this);
}


/// <summary>
/// Loads this tunnel from the save game stream.
/// The object is rebuilt in place once the base class has read the raw image back over
/// it, so that the tunnel has a working virtual table again.
/// </summary>
/// <returns>Returns with S_OK if the tunnel was read successfully.</returns>
// (0x006EF3A0)
// Restores the tube in place.

HRESULT __stdcall TubeClass::Load(IStream *stream)
{
    HRESULT result = AbstractClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) TubeClass(NoInitClass());
        result = S_OK;
    }
    return result;
}


/// <summary>
/// Saves this tunnel to the save game stream.
/// </summary>
/// <param name="cleardirty">Should the dirty flag be cleared once the object is written?</param>
/// <returns>Returns with S_OK if the tunnel was written successfully.</returns>
// (0x006EF3E0)

HRESULT __stdcall TubeClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = AbstractClass::Save(stream, clear_dirty);
    if (SUCCEEDED(result)) {
        result = S_OK;
    }
    return result;
}


// void TubeClass::Update_All_Indexes() (0x006EF400)
// Numbers the tubes whose entry cell can be entered and deletes the rest.

void TubeClass::Update_All_Indexes()
{
    short index = -1;
    int count = unknown_TubeClass_list_008661c8.Count();
    for (int position = 0; position < count; position++) {
        TubeClass *tube = unknown_TubeClass_list_008661c8[position];
        CellClass &cell = Map[tube->EnterCell];
        if (!cell.Has_Tunnel()) {
            delete tube;
            count--;
            cell.unknown_116 = -1;
            position--;
        } else {
            index++;
            cell.unknown_116 = index;
        }
    }
}


/// <summary>
/// Writes all of the tunnels out to the map file.
/// Each tunnel becomes a single entry holding the cell it is entered from, the facing it
/// is entered by, the cell it comes out of, and the chain of directions that traces its
/// path underground. The tunnel list is tidied up first so that the indices written here
/// agree with the ones stamped into the cells.
/// </summary>
// (0x006EF480)
// Rewrites [Tubes]: renumbers the tubes (deleting the unusable), then one "index=x,y,facing,x,y,directions" entry each.
extern "C" int __cdecl sprintf(char *buffer, char const *format, ...);
extern "C" unsigned int __cdecl strlen(char const *string);

void __fastcall TubeClass::Write_INI(CCINIClass &ini)
{
    ini.Clear(TubeSection_Tubes, 0);
    Update_All_Indexes();
    for (int index = 0; index < unknown_TubeClass_list_008661c8.Count(); index++) {
        TubeClass *tube = unknown_TubeClass_list_008661c8[index];
        if (tube != 0) {
            char key[12];
            char value[552];
            sprintf(key, "%d", index);
            sprintf(value, "%d,%d,%d,%d,%d", (int)tube->EnterCell.X, (int)tube->EnterCell.Y, tube->Facing, (int)tube->ExitCell.X, (int)tube->ExitCell.Y);
            for (int step = 0; step < 100; step++) {
                sprintf(value + strlen(value), ",%d", tube->Directions[step]);
            }
            ini.Put_String(TubeSection_Tubes, key, value);
        }
    }
}


/// <summary>
/// Reads the tunnels in from the map file.
/// A tunnel is created for every entry found, and the cell each one is entered from is
/// stamped with the index of the tunnel that leads out of it.
/// </summary>
// (0x006EF5C0)
// Makes a tube of every [Tubes] entry: "enter x,y,facing,exit x,y,directions..." (a -1 direction ends the list).
extern "C" int __cdecl atoi(char const *string);

void __fastcall TubeClass::Read_INI(CCINIClass &ini)
{
    char buffer[512];
    int step = 0;
    int count = ini.Entry_Count(TubeSection_Tubes);
    for (int entry = 0; entry < count; entry++) {
        step = 0;
        char const *key = ini.Get_Entry(TubeSection_Tubes, entry);
        ini.Get_String(TubeSection_Tubes, key, 0, buffer, sizeof(buffer));
        TubeClass *tube = new TubeClass(Cell(0, 0), 0);
        tube->EnterCell.X = atoi(strtok(buffer, ","));
        tube->EnterCell.Y = atoi(strtok(0, ","));
        tube->Facing = atoi(strtok(0, ","));
        tube->ExitCell.X = atoi(strtok(0, ","));
        tube->ExitCell.Y = atoi(strtok(0, ","));
        tube->unknown_1c0 = -1;
        for (; step < 100; step++) {
            tube->Directions[step] = atoi(strtok(0, ","));
            tube->unknown_1c0++;
            if (tube->Directions[step] == -1) {
                break;
            }
        }
        Map[tube->EnterCell].unknown_116 = entry;
    }
}


// bool TubeClass::unknown_6ef740(Cell const &cell) const (0x006EF740)
// TibSun Not_In_Tube: false when the cell lies on the tube's path after its entry cell
// (the entry cell itself gives true).

#ifndef RA2_ADJACENT_CELL
#define RA2_ADJACENT_CELL
#include "shared/adjacent_cell.hpp"
inline Cell Adjacent_Cell(Cell const &cell, int facing)
{
    return cell + AdjacentCell[facing & 7];
}
#endif

bool TubeClass::unknown_6ef740(Cell const &cell) const
{
    Cell tubecell = EnterCell;
    if (tubecell == cell) {
        return true;
    }
    int dir = 0;
    while (Directions[dir] != -1) {
        tubecell = Adjacent_Cell(tubecell, Directions[dir++]);
        if (tubecell == cell) {
            return false;
        }
    }
    return true;
}


// void TubeClass::Add_Direction(int facing) (0x006EF7D0)
// Extends the tube by one step (TibSun Add_Direction): the facing is appended to the
// directions while fewer than 99 are held, and the exit cell moves toward it.

#ifndef RA2_ADJACENT_CELL
#define RA2_ADJACENT_CELL
#include "shared/adjacent_cell.hpp"
inline Cell Adjacent_Cell(Cell const &cell, int facing)
{
    return cell + AdjacentCell[facing & 7];
}
#endif

/// <summary>
/// Extends this tunnel by one step.
/// The direction is appended to the path that traces the tunnel underground, and the
/// exit cell moves along with it. The map editor builds a tunnel up one step at a time
/// this way.
/// </summary>
/// <param name="dir">The direction to extend the tunnel in.</param>
/// <remarks>A tunnel can hold no more than 99 steps. Beyond that the direction is
/// dropped, although the exit cell still moves.</remarks>
void TubeClass::Add_Direction(int facing)
{
    if (unknown_1c0 < 99) {
        Directions[unknown_1c0] = facing;
        Directions[++unknown_1c0] = -1;
    }
    ExitCell = Adjacent_Cell(ExitCell, facing);
}


/// <summary>
/// Folds this tunnel's state into a running CRC.
/// The multiplayer sync check uses this to prove that every machine still agrees on
/// where the tunnels run.
/// </summary>
/// <param name="crc">The running CRC to fold this tunnel's state into.</param>
// (0x006EF830)

void TubeClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractClass::Compute_CRC(crc);
    crc(EnterCell.X);
    crc(EnterCell.Y);
    crc(ExitCell.X);
    crc(ExitCell.Y);
    crc((long)Facing);
    for (int index = 0; index < 100; index++) {
        crc((long)Directions[index]);
    }
    crc((long)unknown_1c0);
}


// int TubeClass::Size_Of() const (0x006EF8B0)

int TubeClass::Size_Of() const
{
    return sizeof(TubeClass);
}


// int TubeClass::Kind_Of() const (0x006EF8C0)

int TubeClass::Kind_Of() const
{
    return 0x35;
}


/// <summary>
/// Fetches the class identifier of this object.
/// This routine is used by the save game system so that it knows what kind of object to
/// construct when the stream is read back in.
/// </summary>
/// <param name="retval">Pointer to the place to store the class identifier.</param>
/// <returns>Returns with S_OK, or E_POINTER if no destination was supplied.</returns>
// (0x006EF8D0)

HRESULT __stdcall TubeClass::GetClassID(GUID *clsid)
{
    if (clsid == 0) {
        return E_POINTER;
    }
    *clsid = CLSID_TubeClass;
    return S_OK;
}


// TubeClass scalar deleting destructor (0x006EF910): compiler generated (the destructor expanded inline); no source of its own.
