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
// Function comment blocks from mischa85/RedAlert2 392e904 src/vector.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// vector.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/vector.json carries the addresses and the rows that
// stay in their own files.


#include "shared/boolean_vector.hpp"
#include "shared/crt_memory.hpp"


/***********************************************************************************************
 * BooleanVectorClass::BooleanVectorClass -- Explicit data buffer constructor.                 *
 *                                                                                             *
 *    This is the constructor for a boolean array. This constructor takes the memory pointer   *
 *    provided as assigns that as the array data pointer.                                      *
 *                                                                                             *
 * INPUT:   size  -- The size of the array (in bits).                                          *
 *                                                                                             *
 *          array -- Pointer to the memory that the array is to use.                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   You must make sure that the memory specified is large enough to contain the     *
 *             bits specified.                                                                 *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0070EFC0)

BooleanVectorClass::BooleanVectorClass(unsigned size, unsigned char *array) :
    BitCount(size),
    Copy(false),
    LastIndex(-1),
    BitArray(0, 0)
{
    BitArray.Resize((size + (8 - 1)) / 8, array);
}


/***********************************************************************************************
 * BooleanVectorClass::BooleanVectorClass -- Copy constructor of boolean array.                *
 *                                                                                             *
 *    This is the copy constructor for a boolean array. It is used to make a duplicate of the  *
 *    boolean array.                                                                           *
 *                                                                                             *
 * INPUT:   vector   -- Reference to the vector to be duplicated.                              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0070F010)

BooleanVectorClass::BooleanVectorClass(BooleanVectorClass const &vector)
{
    LastIndex = -1;
    *this = vector;
}


// BooleanVectorClass::operator= (0x0070F0C0): stores the fetched copy back, then copies member-wise.

BooleanVectorClass &BooleanVectorClass::operator=(BooleanVectorClass const &vector)
{
    Fixup();
    Copy = vector.Copy;
    LastIndex = vector.LastIndex;
    BitArray = vector.BitArray;
    BitCount = vector.BitCount;
    return *this;
}


// BooleanVectorClass::operator== (0x0070F170): same count and same packed bytes.

int BooleanVectorClass::operator==(BooleanVectorClass const &vector) const
{
    Fixup(LastIndex);
    return BitCount == vector.BitCount && BitArray == vector.BitArray;
}


/***********************************************************************************************
 * BooleanVectorClass::Resize -- Resizes a boolean vector object.                              *
 *                                                                                             *
 *    This routine will resize the boolean vector object. An index value used with a boolean   *
 *    vector must be less than the value specified in as the new size.                         *
 *                                                                                             *
 * INPUT:   size  -- The new maximum size of this boolean vector.                              *
 *                                                                                             *
 * OUTPUT:  Was the boolean vector sized successfully?                                         *
 *                                                                                             *
 * WARNINGS:   The boolean array might be reallocated or even deleted by this routine.         *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0070F1E0)

int BooleanVectorClass::Resize(unsigned size)
{
    Fixup();

    if (size > 0) {
        unsigned oldsize = BitCount;
        int success = BitArray.Resize((size + (8 - 1)) / 8);
        BitCount = size;
        if (success && oldsize < size) {
            for (unsigned index = oldsize; index < size; index++) {
                (*this)[index] = 0;
            }
        }
        return success;
    }

    Clear();
    return true;
}


/***********************************************************************************************
 * BooleanVectorClass::Clear -- Resets boolean vector to empty state.                          *
 *                                                                                             *
 *    This routine will clear out the boolean array. This will free any allocated memory and   *
 *    result in the boolean vector being unusable until the Resize function is subsequently    *
 *    called.                                                                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   The boolean vector cannot be used until it is resized to a non null condition.  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0070F2C0)

void BooleanVectorClass::Clear()
{
    Fixup();
    BitCount = 0;
    BitArray.Clear();
}


/***********************************************************************************************
 * BooleanVectorClass::Reset -- Clear all boolean values in array.                             *
 *                                                                                             *
 *    This is the preferred (and quick) method to clear the boolean array to a false condition.*
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0070F300)

void BooleanVectorClass::Reset()
{
    LastIndex = -1;
    if (BitArray.Length() > 0) {
        memset(&BitArray[0], '\0', BitArray.Length());
    }
}


/***********************************************************************************************
 * BooleanVectorClass::Set -- Forces all boolean elements to true.                             *
 *                                                                                             *
 *    This is the preferred (and fast) way to set all boolean elements to true.                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0070F330)

void BooleanVectorClass::Set()
{
    LastIndex = -1;
    if (BitArray.Length() > 0) {
        memset(&BitArray[0], '\xFF', BitArray.Length());
    }
}


/***********************************************************************************************
 * BooleanVectorClass::Fixup -- Updates the boolean vector to a known state.                   *
 *                                                                                             *
 *    Use this routine to set the boolean value copy to match the appropriate bit in the       *
 *    boolean array. The boolean array will be updated with any changes from the last time     *
 *    a value was fetched from the boolean vector. By using this update method, the boolean    *
 *    array can be treated as a normal array even though the elements are composed of          *
 *    otherwise inaccessible bits.                                                             *
 *                                                                                             *
 * INPUT:   index -- The index to set the new copy value to. If the index is -1, then the      *
 *                   previous value will be updated into the vector array, but no new value    *
 *                   will be fetched from it.                                                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Always call this routine with "-1" if any direct manipulation of the bit        *
 *             array is to occur. This ensures that the bit array is accurate.                 *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0070F360)
// (an out-of-range index only flushes).

void BooleanVectorClass::Fixup(int index) const
{
    if ((unsigned)index >= BitCount) {
        index = -1;
    }

    if (index != LastIndex) {
        if (LastIndex != -1) {
            Set_Bit((void *)&BitArray[0], LastIndex, Copy);
        }
        if (index != -1) {
            ((unsigned char &)Copy) = Get_Bit(&BitArray[0], index);
        }
        ((int &)LastIndex) = index;
    }
}


// VectorClass<unsigned char>::`scalar deleting destructor' (0x0070F3C0)
// Compiler-emitted in this unit; no source of its own.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<unsigned char>::Fetch (0x0070F3B0; VectorClass<unsigned char> vtable 0x007ae6ec slot 6).
// VectorClass<unsigned char>::operator equal (0x0070F410; VectorClass<unsigned char> vtable 0x007ae6ec slot 1).
// VectorClass<unsigned char>::Resize (0x0070F450; VectorClass<unsigned char> vtable 0x007ae6ec slot 2).
// VectorClass<unsigned char>::Clear (0x0070F500; VectorClass<unsigned char> vtable 0x007ae6ec slot 3).
// VectorClass<unsigned char>::ID_object (0x0070F530; VectorClass<unsigned char> vtable 0x007ae6ec slot 4).
// VectorClass<unsigned char>::ID_pointer (0x0070F570; VectorClass<unsigned char> vtable 0x007ae6ec slot 5).
