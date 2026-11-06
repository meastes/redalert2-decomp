// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// UnknownPathEstimator::Unknown_00427060() (0x00427060): the cell at the end
// of count steps along a list of facings.
#ifndef VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#endif
#define VECTOR_IMPLEMENT_DYNAMIC_ADD
#define VECTOR_IMPLEMENT_DYNAMIC_CLEAR
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#include "shared/path_estimator.hpp"
#include "shared/cell.hpp"
#include "shared/subzone_tracking.hpp"
#include "shared/map_view.hpp"
#include "shared/tube.hpp"
// @ref ?unknown_path_facing_table_7cf100@@3PAY02HA=007cf100
// @ref ?unknown_path_facing_offsets_84c838@@3PAHA=0084c838

// The unit expands its DynamicVectorClass constructors and calls the VectorClass base constructors
// (0x0042B510, 0x0042B620) out of line.
template <>
inline DynamicVectorClass<unsigned int>::DynamicVectorClass(unsigned size, unsigned int const *array)
    : VectorClass<unsigned int>(size, array)
{
    GrowthStep = 10;
    ActiveCount = 0;
}
template <>
inline DynamicVectorClass<unsigned short>::DynamicVectorClass(unsigned size, unsigned short const *array)
    : VectorClass<unsigned short>(size, array)
{
    GrowthStep = 10;
    ActiveCount = 0;
}

// The unit's out-of-line copy is the COMDAT 0x0042AD50 (0x00428CF0 calls it).


Cell __fastcall UnknownPathEstimator::Unknown_00427060(Cell const &start, int count, int const *facings)
{
    // ra2-decomp Follow_Path (YR 0x00429780) form: the exit test inside the loop.
    Cell cell = start;
    for (int index = 0; ; index++) {
        if (index >= count) {
            return cell;
        }
        cell = Unknown_0042AD50(cell, facings[index]);
    }
}
