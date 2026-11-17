/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/
// Function comment blocks from mischa85/RedAlert2 392e904 src/alphalighting.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.
// Parameter names from mischa85/RedAlert2 392e904 src/alphalighting.cpp where both define the function (names only).

// alphalighting.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/alphalighting.json carries the addresses and the rows that
// stay in their own files.


// Dynamic initializer (0x0041E320) and atexit destructor (0x0041E360) of the
// shared remap list AlphaLightingRemapClass::Array (0x0083C5D8); the exit
// thunk tests and sets a guard bit (VC6 guards the destruction of a static
// data member). The functions of the unit follow in their own drafts.
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/alpha_lighting_remap.hpp"

DynamicVectorClass<AlphaLightingRemapClass *> AlphaLightingRemapClass::Array;


// atexit destructor (0x0041E360, _$E3) emitted by the draft of 0x0041E320; no source of its own.


/// <summary>
/// Constructs the alpha lighting remap manager.
/// </summary>
// (0x0041E3C0)
#include "shared/alpha_table_cache.hpp"

AlphaLightingRemapInitClass::AlphaLightingRemapInitClass()
{
}


/// <summary>
/// Destroys the alpha lighting remap manager.
/// The remap tables are shared and counted, so none of them are released here.
/// </summary>
// (0x0041E3D0)

AlphaLightingRemapInitClass::~AlphaLightingRemapInitClass()
{
}


// AlphaTableRows AlphaLightingRemapInitClass::Get(int count) (0x0041E3E0)
// Returns the shared remap table for a light level, building and registering
// it on first use; each call takes a reference.

AlphaTableRows AlphaLightingRemapInitClass::Get(int count)
{
    for (int index = 0; index < AlphaLightingRemapClass::Array.Count(); index++) {
        if (AlphaLightingRemapClass::Array[index]->Level == count) {
            AlphaLightingRemapClass::Array[index]->RefCount++;
            return (AlphaTableRows)AlphaLightingRemapClass::Array[index]->Table;
        }
    }

    AlphaLightingRemapClass *remap = new AlphaLightingRemapClass(count);
    AlphaLightingRemapClass::Array.Add(remap);
    remap->RefCount++;
    return (AlphaTableRows)remap->Table;
}


// void AlphaLightingRemapInitClass::Release(AlphaTableRows table) (0x0041E510)
// Drops a reference to a shared remap table; the last one unregisters and
// frees it.

void AlphaLightingRemapInitClass::Release(AlphaTableRows table)
{
    if (table != 0) {
        AlphaLightingRemapClass *remap = (AlphaLightingRemapClass *)table;
        remap->RefCount--;
        if (remap->RefCount == 0) {
            AlphaLightingRemapClass::Array.Delete(remap);
            delete remap;
        }
    }
}


/// <summary>
/// Builds an alpha lighting remap table.
/// This routine fills in the lookup that the alpha blitters draw through. A row is
/// fetched for the lighting level in force and indexed with the alpha buffer value, and
/// the result picks which shading band of the blitter's translation table the pixel is
/// drawn from.
/// </summary>
/// <param name="count">The number of shading bands the translation table provides.</param>
// (0x0041E590)

AlphaLightingRemapClass::AlphaLightingRemapClass(int count)
    : Level(count), RefCount(0)
{
    for (int index = 0; index <= 0xFFFF; index++) {
        int value = (index & 0xFF) * (index >> 8) * (count - 1) / 0x7E02;
        if (value > count - 1) {
            value = count - 1;
        }
        Table[index] = (unsigned short)((unsigned char)value << 8);
    }
}


// unknown_empty_0041e600 (0x0041E600)

void  unknown_empty_0041e600(void)
{
    return;
}


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<AlphaLightingRemapClass *>::Clear (0x0041E610; DynamicVectorClass<AlphaLightingRemapClass *> vtable 0x0079b800 slot 3).
// DynamicVectorClass<AlphaLightingRemapClass *>::ID_pointer (0x0041E640; DynamicVectorClass<AlphaLightingRemapClass *> vtable 0x0079b800 slot 5).
// VectorClass<AlphaLightingRemapClass *>::Fetch (0x0041E660; VectorClass<AlphaLightingRemapClass *> vtable 0x0079b820 slot 6).
// VectorClass<AlphaLightingRemapClass *>::operator equal (0x0041E670; VectorClass<AlphaLightingRemapClass *> vtable 0x0079b820 slot 1).
// VectorClass<AlphaLightingRemapClass *>::Resize (0x0041E6B0; VectorClass<AlphaLightingRemapClass *> vtable 0x0079b820 slot 2).
// VectorClass<AlphaLightingRemapClass *>::Clear (0x0041E760; VectorClass<AlphaLightingRemapClass *> vtable 0x0079b820 slot 3).
// VectorClass<AlphaLightingRemapClass *>::ID_object (0x0041E790; VectorClass<AlphaLightingRemapClass *> vtable 0x0079b820 slot 4).
// VectorClass<AlphaLightingRemapClass *>::ID_pointer (0x0041E7D0; VectorClass<AlphaLightingRemapClass *> vtable 0x0079b820 slot 5).
// DynamicVectorClass<AlphaLightingRemapClass *>::Resize (0x0041E7F0; DynamicVectorClass<AlphaLightingRemapClass *> vtable 0x0079b800 slot 2).
// DynamicVectorClass<AlphaLightingRemapClass *>::ID_object (0x0041E8A0; DynamicVectorClass<AlphaLightingRemapClass *> vtable 0x0079b800 slot 4).


// VectorClass<AlphaLightingRemapClass *>::`scalar deleting destructor' (0x0041E8D0)
// Compiler-emitted from shared/vector_class.hpp in this unit; no source of its own.


// DynamicVectorClass<AlphaLightingRemapClass *>::`scalar deleting destructor' (0x0041E920)
// Compiler-emitted from shared/vector_class.hpp in this unit; no source of its own.
