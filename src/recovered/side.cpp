/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/
// Function comment blocks from mischa85/RedAlert2 392e904 src/side.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// side.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/side.json carries the addresses and the rows that
// stay in their own files.

// Header-static dynamic initializers of the unit at 0x0067AC40:  the
// shared/unknown_geometry.hpp constants (0x0067AC40-0x0067AE01); the other
// drafts of this plan are emitted by these includes.
// Unit-wide (Side.cpp 0x0067AC40): side.hpp leaves ~SideClass out of line here.
#define RA2_SIDE_UNIT
#include "shared/unknown_geometry.hpp"


// Dynamic initializer (0x0067AC70, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_0067ac40.cpp; no source of its own.


// Dynamic initializer (0x0067AC90, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_0067ac40.cpp; no source of its own.


// Dynamic initializer (0x0067ACB0, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_0067ac40.cpp; no source of its own.


// Dynamic initializer (0x0067ACD0, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_0067ac40.cpp; no source of its own.


// Dynamic initializer (0x0067ACF0, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_0067ac40.cpp; no source of its own.


// Dynamic initializer (0x0067AD10, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_0067ac40.cpp; no source of its own.


// Dynamic initializer (0x0067AD50, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_0067ac40.cpp; no source of its own.


// Dynamic initializer (0x0067AD80, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_0067ac40.cpp; no source of its own.


// Dynamic initializer (0x0067ADB0, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_0067ac40.cpp; no source of its own.


// Dynamic initializer (0x0067ADE0, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_0067ac40.cpp; no source of its own.


// Dynamic initializer (0x0067ADF0, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_0067ac40.cpp; no source of its own.




#include "shared/side.hpp"


/// <summary>
/// Creates a side and adds it to the global side list.
/// This routine is used as the rules are parsed. The side is given its unique heap
/// identifier so that it can be referred to by the houses that belong to it.
/// </summary>
// (0x0067AE10)
// An empty side named ini_name; registers it in Sides.

SideClass::SideClass(char const *ini_name) :
    AbstractTypeClass(ini_name)
{
    Create_ID();
    Sides.Add(this);
}


/// <summary>
/// Destroys the side and removes it from the game.
/// This routine detaches anything that referred to this side before dropping it from
/// the global side list.
/// </summary>
// (0x0067AED0)
// Leaves the dying side's notice lists and Sides; ~TypeList and ~AbstractTypeClass follow. Defined
// here, not inline: the scalar deleting destructor 0x0067B1F0 expands this body.

SideClass::~SideClass()
{
    Unknown_006ECB90(this, true);
    Sides.Delete(this);
}


// int __fastcall SideClass::Find_Index(char const *name) (0x0067AF90)

int __fastcall SideClass::Find_Index(char const *name)
{
    for (int index = 0; index < Sides.Count(); index++) {
        if (_stricmp(Sides[index]->ID, name) == 0) {
            return index;
        }
    }
    return -1;
}


/// <summary>
/// Adds this side to the running game state checksum.
/// This routine is used by the multiplayer sync checking to prove that every machine
/// agrees on the side definitions it was handed.
/// </summary>
// (0x0067AFD0)
// The AbstractTypeClass data and the number of houses.

void SideClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractTypeClass::Compute_CRC(crc);
    crc((long)Houses.Count());
}


/// <summary>
/// Fetches the class identifier of this object.
/// This routine is part of the IPersist interface. It is used by the save and load
/// system to recognize what kind of object it is about to create.
/// </summary>
/// <param name="retval">Pointer to the identifier to fill in.</param>
/// <returns>Returns with S_OK, or E_POINTER if no destination was supplied.</returns>
// (0x0067B000)

HRESULT __stdcall SideClass::GetClassID(GUID *classid)
{
    if (classid == 0) {
        return E_POINTER;
    }
    *classid = CLSID_SideClass;
    return S_OK;
}


// HRESULT __stdcall SideClass::Load(IStream *stream) (0x0067B040)
// Restores the side in place and rebuilds its house index list from the
// saved count and entries.

// Rebuilds a list in place from its saved count and elements.
template <class T>
static inline void Load_List(IStream *stream, TypeList<T> &list)
{
    new (&list) TypeList<T>;
    int count;
    stream->Read(&count, sizeof(count), 0);
    for (int index = 0; index < count; index++) {
        T value;
        stream->Read(&value, sizeof(value), 0);
        list.Add(value);
    }
}

/// <summary>
/// Loads this side from a save game stream.
/// This routine is part of the IPersistStream interface. The base class data is read first
/// and then the list of houses that belong to this side is restored.
/// </summary>
/// <returns>Returns with S_OK if the side was loaded, otherwise the failure code that the
/// base class reported.</returns>
HRESULT __stdcall SideClass::Load(IStream *stream)
{
    HRESULT result = AbstractTypeClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    new (this) SideClass(NoInitClass());
    Load_List(stream, Houses);
    return S_OK;
}


/// <summary>
/// Saves this side to a save game stream.
/// This routine is part of the IPersistStream interface. The base class data is written
/// first and then the list of houses that belong to this side.
/// </summary>
/// <param name="cleardirty">Should the dirty flag be cleared as a result of saving?</param>
/// <returns>Returns with S_OK if the side was saved, otherwise the failure code that the
/// base class reported.</returns>
// (0x0067B160)
// The AbstractClass data, the house count and the houses.

HRESULT __stdcall SideClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = AbstractClass::Save(stream, clear_dirty);
    if (FAILED(result)) {
        return result;
    }
    int count = Houses.Count();
    stream->Write(&count, sizeof(count), 0);
    for (int index = 0; index < count; index++) {
        stream->Write(&Houses[index], sizeof(int), 0);
    }
    return S_OK;
}


// int SideClass::Size_Of() const (0x0067B1D0)

int SideClass::Size_Of() const
{
    return sizeof(SideClass);
}


// int SideClass::Kind_Of() const (0x0067B1E0): the side RTTI kind.

int SideClass::Kind_Of() const
{
    return 0x1C;
}


// SideClass scalar deleting destructor (0x0067B1F0): emitted with the vtable;
// it expands the inline ~SideClass (shared/side.hpp), ~TypeList and calls
// ~AbstractTypeClass. No source of its own.
