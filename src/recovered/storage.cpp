/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 * Copyright 2026 ra2-decomp contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/
// Function comment blocks from mischa85/RedAlert2 392e904 src/storage.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// storage.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/storage.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unknown_geometry.hpp"
#define STORAGE_CLASS_UNIT
#include "shared/storage_class.hpp"
#include "shared/tiberium.hpp"


// Header-static dynamic initializers of the unit at 0x0069ABD0:  the
// shared/unknown_geometry.hpp constants (0x0069ABD0-0x0069AD91); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x0069AC00, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_0069abd0.cpp; no source of its own.


// Dynamic initializer (0x0069AC20, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_0069abd0.cpp; no source of its own.


// Dynamic initializer (0x0069AC40, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_0069abd0.cpp; no source of its own.


// Dynamic initializer (0x0069AC60, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_0069abd0.cpp; no source of its own.


// Dynamic initializer (0x0069AC80, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_0069abd0.cpp; no source of its own.


// Dynamic initializer (0x0069ACA0, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_0069abd0.cpp; no source of its own.


// Dynamic initializer (0x0069ACE0, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_0069abd0.cpp; no source of its own.


// Dynamic initializer (0x0069AD10, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_0069abd0.cpp; no source of its own.


// Dynamic initializer (0x0069AD40, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_0069abd0.cpp; no source of its own.


// Dynamic initializer (0x0069AD70, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_0069abd0.cpp; no source of its own.


// Dynamic initializer (0x0069AD80, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_0069abd0.cpp; no source of its own.


/// <summary>
/// Creates an empty storage object.
/// Every tiberium slot starts out holding nothing.
/// </summary>
// (0x0069ADA0)
// with float amounts).

StorageClass::StorageClass()
{
    for (int i = 0; i < 4; i++) {
        Amounts[i] = 0;
    }
}


/// <summary>
/// Fetches the credit value of everything in storage.
/// Each tiberium type carries its own credit value, so this routine weighs every slot
/// against its type. This is what a refinery uses to decide how much to pay out for a
/// harvester load.
/// </summary>
/// <returns>Returns with the total credit value of the tiberium held.</returns>
// (0x0069ADC0)
int StorageClass::Get_Total_Value() const
{
    int value = 0;
    for (int i = 0; i < 4; i++) {
        if (Amounts[i] > 0) {
            value += Amounts[i] * unknown_TiberiumClass_list_00ac0b30[i]->Value;
        }
    }
    return value;
}


/// <summary>
/// Fetches the total quantity held in storage.
/// This is a plain count of everything stored, with no regard for what the various
/// tiberium types happen to be worth. Use it when the question is "how full is this?"
/// rather than "how much is this worth?".
/// </summary>
/// <returns>Returns with the sum of every storage slot.</returns>
// (0x0069AE10)
float StorageClass::Get_Total_Amount() const
{
    int amount = 0;
    for (int i = 0; i < 4; i++) {
        amount += Amounts[i];
    }
    return amount;
}


/// <summary>
/// Fetches the amount held in one storage slot.
/// </summary>
/// <returns>Returns with the quantity of that tiberium type currently stored.</returns>
// (0x0069AE40)
// tiberium type (TibSun StorageClass::Get_Amount).

float StorageClass::Get_Amount(int type) const
{
    return Amounts[type];
}


// float StorageClass::Add_Amount(float amount, int type) -- 0x0069AE50
// Purpose: adds amount to the amount of one type and returns the new amount.

float StorageClass::Add_Amount(float amount, int type)
{
    return Amounts[type] += amount;
}


// float StorageClass::Remove_Amount(float amount, int type) -- 0x0069AE70
// Purpose: removes up to amount from the amount of one type (never below what
// is there) and returns the amount removed.

float StorageClass::Remove_Amount(float amount, int type)
{
    float removed = Amounts[type] < amount ? Amounts[type] : amount;
    Amounts[type] -= removed;
    return removed;
}


// UnknownFloat4_0069aea0 UnknownFloat4_0069aea0::operator+(UnknownFloat4_0069aea0 const &other) const -- 0x0069AEA0
// Purpose: the element-wise sum of two sets of four floats.
// Assumptions: the class (four floats) is unidentified; its zeroing
// constructor clears the words as integers (one xor register), so the
// clear loop is written that way.

struct UnknownFloat4_0069aea0 {
    float Values[4];
    UnknownFloat4_0069aea0 operator+(UnknownFloat4_0069aea0 const &other) const;
};

UnknownFloat4_0069aea0 UnknownFloat4_0069aea0::operator+(UnknownFloat4_0069aea0 const &other) const
{
    UnknownFloat4_0069aea0 result;
    int index;
    for (index = 0; index < 4; index++) {
        *(int *)&result.Values[index] = 0;
    }
    for (index = 0; index < 4; index++) {
        result.Values[index] = other.Values[index] + Values[index];
    }
    return result;
}


// StorageClass StorageClass::operator+=(StorageClass const &that) (0x0069AF00).
StorageClass StorageClass::operator+=(StorageClass const &that)
{
    for (int i = 0; i < 4; i++) {
        Amounts[i] += that.Amounts[i];
    }
    return *this;
}


// StorageClass StorageClass::operator-(StorageClass const &that) const (0x0069AF40).
StorageClass StorageClass::operator-(StorageClass const &that) const
{
    StorageClass tmp;
    for (int i = 0; i < 4; i++) {
        tmp.Amounts[i] = Amounts[i] - that.Amounts[i];
    }
    return tmp;
}


// StorageClass StorageClass::operator-=(StorageClass const &that) (0x0069AFA0).
StorageClass StorageClass::operator-=(StorageClass const &that)
{
    for (int i = 0; i < 4; i++) {
        Amounts[i] -= that.Amounts[i];
    }
    return *this;
}


/// <summary>
/// Fetches the first storage slot that holds anything.
/// This routine is used when the holder of the storage only cares about one tiberium
/// type -- a harvester picking the graphic for its load, for example.
/// </summary>
/// <returns>Returns with the slot of the first tiberium type present, or -1 if the storage
/// is empty.</returns>
// (0x0069AFE0)
int StorageClass::First_Used_Slot() const
{
    for (int i = 0; i < 4; i++) {
        if (Amounts[i] > 0) {
            return i;
        }
    }
    return -1;
}
