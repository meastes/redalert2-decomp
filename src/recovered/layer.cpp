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
// Function comment blocks from mischa85/RedAlert2 392e904 src/layer.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// layer.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/layer.json carries the addresses and the rows that
// stay in their own files.


// Header-static dynamic initializers of the unit at 0x005346B0:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x005346B0-0x00534896); the other
// drafts of this plan are emitted by these includes.
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"


// Dynamic initializer (0x005346E0, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_005346b0.cpp; no source of its own.


// Dynamic initializer (0x00534700, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_005346b0.cpp; no source of its own.


// Dynamic initializer (0x00534720, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_005346b0.cpp; no source of its own.


// Dynamic initializer (0x00534740, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_005346b0.cpp; no source of its own.


// Dynamic initializer (0x00534760, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_005346b0.cpp; no source of its own.


// Dynamic initializer (0x00534780, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_005346b0.cpp; no source of its own.


// Dynamic initializer (0x005347C0, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_005346b0.cpp; no source of its own.


// Dynamic initializer (0x005347F0, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_005346b0.cpp; no source of its own.


// Dynamic initializer (0x00534820, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_005346b0.cpp; no source of its own.


// Dynamic initializer (0x00534850, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_005346b0.cpp; no source of its own.


// Dynamic initializer (0x00534860, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_005346b0.cpp; no source of its own.


// Dynamic initializer (0x00534880, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_005346b0.cpp; no source of its own.


/***********************************************************************************************
 * LayerClass::Submit -- Adds an object to a layer list.                                       *
 *                                                                                             *
 *    This routine is used to add an object to the layer list. If the list is full, then the   *
 *    object is not added.                                                                     *
 *                                                                                             *
 * INPUT:   object   -- Pointer to the object to add.                                          *
 *                                                                                             *
 * OUTPUT:  bool; Was the object added successfully?                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/31/1994 JLB : Created.                                                                 *
 *   05/31/1994 JLB : Allows sorted insert.                                                    *
 *   01/02/1995 JLB : Fixed to work with EMSListOf template.                                   *
 *=============================================================================================*/
// (0x005348A0, slot 7)
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/vector_class.hpp"
#include "shared/layer.hpp"
#include "shared/object.hpp"

bool LayerClass::Submit(ObjectClass *object, bool sort)
{
    if (sort) {
        return Sorted_Add(object);
    }
    return Add(object) != false;
}


/***********************************************************************************************
 * LayerClass::Sort -- Handles sorting the objects in the layer.                               *
 *                                                                                             *
 *    This routine is used if the layer objects must be sorted and sorting is to occur now.    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Don't call this routine too often since it does take a bit of time to           *
 *             execute. It is a single pass binary sort and thus isn't horribly slow,          *
 *             but it does take some time.                                                     *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *   03/10/1995 JLB : Uses comparison operator.                                                *
 *=============================================================================================*/
// (0x00534920)
// One pass of a bubble sort by ObjectClass slot 45 (Westwood's LAYER.CPP).

void LayerClass::Sort()
{
    for (int index = 0; index < Count() - 1; index++) {
        ObjectClass *next = (*this)[index + 1];
        ObjectClass *object = (*this)[index];
        int value = next->Sort_Y();
        if (value < object->Sort_Y()) {
            (*this)[index + 1] = object;
            (*this)[index] = next;
        }
    }
}


/***********************************************************************************************
 * DynamicVectorClass<T>::Sorted_Add -- Adds object in sorted order to vector.                 *
 *                                                                                             *
 *    Use this routine to add an object to the vector but it will be inserted in sorted        *
 *    order. This depends on the ">" operator being defined for the vector object.             *
 *                                                                                             *
 * INPUT:   object   -- Reference to the object that will be added to the vector.              *
 *                                                                                             *
 * OUTPUT:  bool; Was the object added to the vector successfully?                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/10/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00534980)
// Inserts the object before the first larger one (Westwood's LAYER.CPP).

int LayerClass::Sorted_Add(ObjectClass *object)
{
    if (ActiveCount >= Length()) {
        if ((IsAllocated || !VectorMax) && GrowthStep > 0) {
            if (!Resize(Length() + GrowthStep)) {
                return false;
            }
        } else {
            return false;
        }
    }
    int index;
    for (index = 0; index < ActiveCount; index++) {
        if (*(*this)[index] > *object) {
            break;
        }
    }
    for (int i = ActiveCount - 1; i >= index; i--) {
        (*this)[i + 1] = (*this)[i];
    }
    (*this)[index] = object;
    ActiveCount++;
    return true;
}


/// <summary>
/// Saves the layer to the data stream.
/// This routine is called as part of the persistent save process. The objects themselves
/// are written by their own owners -- only the layer's object pointers are recorded here,
/// to be swizzled back into real addresses when the game is loaded.
/// </summary>
/// <returns>Returns with S_OK if the layer was written. Otherwise, the failure code from
/// the stream is returned.</returns>
// (0x00534A10)
// The object count, then each object pointer.

HRESULT LayerClass::Save(IStream *stream)
{
    if (!stream) {
        return E_POINTER;
    }
    int count = Count();
    HRESULT result = stream->Write(&count, sizeof(count), 0);
    if (FAILED(result)) {
        return result;
    }
    for (int index = 0; index < count; index++) {
        result = stream->Write(&(*this)[index], sizeof(ObjectClass *), 0);
        if (FAILED(result)) {
            return result;
        }
    }
    return S_OK;
}


/// <summary>
/// Loads the layer from the data stream.
/// This routine is the counterpart to Save and is called while the save game is being
/// reconstructed. The object pointers are read back exactly as they were written and then
/// handed to the pointer swizzler, so they do not become usable until the remap pass has
/// run.
/// </summary>
/// <returns>Returns with S_OK if the layer was read. Otherwise, the failure code from the
/// stream is returned.</returns>
// (0x00534A80)
// Reads the object pointers back and registers each for swizzling.

HRESULT LayerClass::Load(IStream *stream)
{
    if (!stream) {
        return E_POINTER;
    }
    int count = 0;
    HRESULT result = stream->Read(&count, sizeof(count), 0);
    if (FAILED(result)) {
        return result;
    }
    for (int index = 0; index < count; index++) {
        ObjectClass *object;
        result = stream->Read(&object, sizeof(object), 0);
        if (FAILED(result)) {
            return result;
        }
        Add(object);
    }
    for (int i = 0; i < count; i++) {
        Swizzle_Pointer((*this)[i]);
    }
    return S_OK;
}
