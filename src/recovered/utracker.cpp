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
// Function comment blocks from mischa85/RedAlert2 392e904 src/utracker.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// utracker.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/utracker.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unit_tracker.hpp"


/***********************************************************************************************
 * UTC::UnitTrackerClass -- Class constructor                                                  *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Number of unit types to reserve space for                                         *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    6/7/96 0:10AM ST : Created                                                               *
 *=============================================================================================*/
// (0x0070C440)

UnitTrackerClass::UnitTrackerClass()
{
    UnitCount = 512;
    Clear_Unit_Total();
    InNetworkFormat = 0;
}


/***********************************************************************************************
 * UTC::~UnitTrackerClass -- Class destructor                                                  *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    6/7/96 0:10AM ST : Created                                                               *
 *=============================================================================================*/
// (0x0070C480)

UnitTrackerClass::~UnitTrackerClass()
{
}


/***********************************************************************************************
 * UTC::Increment_Unit_Total -- Increment the total for the specefied unit                     *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Unit number                                                                       *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    6/7/96 0:12AM ST : Created                                                               *
 *=============================================================================================*/
// (0x0070C490)

void UnitTrackerClass::Increment_Unit_Total(int unit_type)
{
    if (unit_type < UnitCount) {
        UnitTotals[unit_type]++;
    }
}


/***********************************************************************************************
 * UTC::Decrement_Unit_Total -- Decrement the total for the specefied unit                     *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Unit number                                                                       *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    6/7/96 0:13AM ST : Created                                                               *
 *=============================================================================================*/
// (0x0070C4B0)

void UnitTrackerClass::Decrement_Unit_Total(int unit_type)
{
    if (unit_type < UnitCount) {
        UnitTotals[unit_type]--;
    }
}


// void UnitTrackerClass::Set_Unit_Count(int count) -- 0x0070C4D0
// Clears every entry, then limits the tracked types to count.

void UnitTrackerClass::Set_Unit_Count(int count)
{
    UnitCount = 512;
    Clear_Unit_Total();
    UnitCount = count;
}


// int UnitTrackerClass::Get_All_Totals_Sum() -- 0x0070C510

int UnitTrackerClass::Get_All_Totals_Sum()
{
    int total = 0;
    for (int index = 0; index < UnitCount; index++) {
        total += UnitTotals[index];
    }
    return total;
}


/***********************************************************************************************
 * UTC::Get_All_Totals -- Returns a pointer to the start of the unit totals list               *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Ptr to unit totals list                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    6/7/96 0:13AM ST : Created                                                               *
 *=============================================================================================*/
// (0x0070C530)

int *UnitTrackerClass::Get_All_Totals()
{
    return UnitTotals;
}


/***********************************************************************************************
 * UTC::Clear_Unit_Total -- Clear out all the unit totals                                      *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    6/7/96 0:14AM ST : Created                                                               *
 *=============================================================================================*/
// (0x0070C540)

void UnitTrackerClass::Clear_Unit_Total()
{
    for (int index = 0; index < UnitCount; index++) {
        UnitTotals[index] = 0;
    }
}


/***********************************************************************************************
 * UTC::To_Network_Format -- Changes all unit totals to network format for the internet        *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    6/7/96 0:15AM ST : Created                                                               *
 *=============================================================================================*/
// (0x0070C570)
// Byte-swaps every count to network order once.

extern "C" unsigned long __stdcall htonl(unsigned long value);

void UnitTrackerClass::To_Network_Format()
{
    if (!InNetworkFormat) {
        for (int index = 0; index < UnitCount; index++) {
            UnitTotals[index] = htonl(UnitTotals[index]);
        }
    }
    InNetworkFormat = 1;
}


/***********************************************************************************************
 * UTC::To_PC_Format -- Changes all unit totals to PC format from network format               *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    6/7/96 0:15AM ST : Created                                                               *
 *=============================================================================================*/
// (0x0070C5C0)
// Byte-swaps every count back to host order once.

extern "C" unsigned long __stdcall ntohl(unsigned long value);

void UnitTrackerClass::To_PC_Format()
{
    if (InNetworkFormat) {
        for (int index = 0; index < UnitCount; index++) {
            UnitTotals[index] = ntohl(UnitTotals[index]);
        }
    }
    InNetworkFormat = 0;
}
