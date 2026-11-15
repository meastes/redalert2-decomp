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
// Function comment blocks from mischa85/RedAlert2 392e904 src/bench.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// bench.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/bench.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/bench.hpp"


// Header-static dynamic initializer of the unit at 0x0042F0A0 (the shape_file.hpp empty rect,
// 0x0084E998); the other drafts of this plan are the unit's Benchmark members.


/***********************************************************************************************
 * Benchmark::Benchmark -- Constructor for the benchmark object.                               *
 *                                                                                             *
 *    This will construct the benchmark object.                                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0042F0C0)

Benchmark::Benchmark() :
    Average(0),
    Counter(0),
    TotalCount(0)
{
}


/***********************************************************************************************
 * Benchmark::Reset -- Clear out the benchmark statistics.                                     *
 *                                                                                             *
 *    Use this routine to clear out all the accumulated statistics within this benchmark       *
 *    object. The object is set just as if it was freshly constructed.                         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0042F0F0)

void Benchmark::Reset()
{
    Average = 0;
    Counter = 0;
    TotalCount = 0;
}


/***********************************************************************************************
 * Benchmark::Begin -- Start the benchmark operation.                                          *
 *                                                                                             *
 *    Call this routine before the operation to be benchmarked is begun. The corresponding     *
 *    End() function must be called after the operation has completed.                         *
 *                                                                                             *
 * INPUT:   reset -- Should the entire benchmark object be reset at this time as well?         *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   The Begin() and End() functions are NOT nestable.                               *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0042F100)
// Bench.cpp).

void Benchmark::Begin(bool reset)
{
    if (reset) Reset();
    Clock = 0;
}


/***********************************************************************************************
 * Benchmark::End -- Mark the end of a benchmarked operation                                   *
 *                                                                                             *
 *    This routine is called at the end of the operation that is being benchmarked. It is      *
 *    important to call this routine as soon as possible after the event being benchmarked     *
 *    has completed.                                                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   The Being() and End() functions are NOT nestable.                               *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0042F140)
// averaged, the oldest share decays instead of the count growing (Westwood's Bench.cpp).

void Benchmark::End()
{
    unsigned long value = Clock;
    if (Counter == 256) {
        Average -= Average / 256;
        Average += value;
    } else {
        Average += value;
        Counter++;
    }
    TotalCount++;
}


/// <summary>
/// Records a benchmark sample and fetches the running event count.
/// Use this routine in place of End() when the caller needs to know how many events have
/// been tracked so far. The elapsed time is folded into the running average and the event
/// tallied.
/// </summary>
/// <returns>Returns with the total number of events recorded by this object.</returns>
// (0x0042F1A0)

unsigned long Benchmark::Step()
{
    unsigned long value = Clock;
    Average += value;
    Counter++;
    TotalCount++;
    return TotalCount;
}


/***********************************************************************************************
 * Benchmark::Value -- Fetch the current average benchmark time.                               *
 *                                                                                             *
 *    This routine will take the statistics already accumulated and determine the average      *
 *    time recorded. This value will be returned.                                              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the average time that all events tracked by this object.              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/18/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0042F1E0)

unsigned long Benchmark::Value() const
{
    if (Counter) {
        return Average / Counter;
    }
    return 0;
}
