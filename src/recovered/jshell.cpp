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
// Function comment blocks from mischa85/RedAlert2 392e904 src/jshell.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// jshell.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/jshell.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/jshell.hpp"
#include "shared/crt_stderr.hpp"
#include "shared/exit.hpp"
#include "shared/mono_class.hpp"


// Header-static dynamic initializer of the JShell unit at 0x0052EDE0: the shape_file.hpp empty rect
// (the unit's only .CRT$XCU entry); the other drafts of this plan follow it in the same file.


// Fatal (0x0052EE00): posts the main window's destroy, prints the message to stderr and the
/***********************************************************************************************
 * Fatal -- General purpose fatal error handler.                                               *
 *                                                                                             *
 *    This is a very simple general purpose fatal error handler. It goes directly to text      *
 *    mode, prints the error, and then aborts with a failure code.                             *
 *                                                                                             *
 * INPUT:   message  -- The text message to display.                                           *
 *                                                                                             *
 *          ...      -- Any optional parameters that are used in formatting the message.       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine never returns. The game exits immediately.                         *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
// mono screen, then exits with code 1 (Westwood's JSHELL.CPP Fatal).

MonoClass *__fastcall MonoClass_Get_Page(int page);       // 0x0059DA50

void __cdecl Fatal(char const *message, ...)
{
    Emergency_Exit();
    vfprintf(stderr, message, (char *)(&message + 1));
    MonoClass_Get_Page(4)->Printf(message);
    Emergency_Exit(1);
}


/***********************************************************************************************
 * Set_Bit -- Set bit in a bit array.                                                          *
 *                                                                                             *
 *    This routine is used to set (or clear) a bit in a bit array.                             *
 *                                                                                             *
 * INPUT:   array -- Pointer to the bit array.                                                 *
 *                                                                                             *
 *          bit   -- The bit number to set. I.e., bit 32 is the first bit in the second        *
 *                   long word of memory.                                                      *
 *                                                                                             *
 *          value -- The value to set the bit. The only values supported are 0 and 1.          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Be sure that the array is big enough to set the specified bit.                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/06/1997 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0052EE40)

void __fastcall Set_Bit(void *array, int bit, int value)
{
    unsigned char mask = (unsigned char)(1 << (bit % 8));

    if (value != 0) {
        *((unsigned char *)array + (bit / 8)) |= mask;
    } else {
        *((unsigned char *)array + (bit / 8)) &= (unsigned char)~mask;
    }
}


/***********************************************************************************************
 * Get_Bit -- Fetch the bit value from a bit array.                                            *
 *                                                                                             *
 *    This routine will fetch the specified bit value from the bit array. This is the          *
 *    counterpart function to the Set_Bit() function.                                          *
 *                                                                                             *
 * INPUT:   array -- Pointer to the bit array to fetch the bit value from.                     *
 *                                                                                             *
 *          bit   -- The bit number to fetch.                                                  *
 *                                                                                             *
 * OUTPUT:  Returns with the value of the bit. This return value will be either 1 or 0.        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/06/1997 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0052EE90)

int __fastcall Get_Bit(void const *array, int bit)
{
    unsigned char mask = (unsigned char)(1 << (bit % 8));
    return (*((unsigned char *)array + (bit / 8)) & mask) != 0;
}


/***********************************************************************************************
 * First_True_Bit -- Return with the first true bit index.                                     *
 *                                                                                             *
 *    This routine will scan the bit array and return with the index for the first true bit    *
 *    in the array.                                                                            *
 *                                                                                             *
 * INPUT:   array -- Pointer to the bit array to scan.                                         *
 *                                                                                             *
 * OUTPUT:  Returns with the index of the first true (set to 1) bit in the array.              *
 *                                                                                             *
 * WARNINGS:   This routine does not stop at the end of the array (it doesn't know where the   *
 *             end is) so there must be at least one true bit in the array or else it will     *
 *             end up scanning past the end (undefined results in that case).                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/06/1997 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0052EED0)

int __fastcall First_True_Bit(void const *array)
{
    int index = 0;
    while (*((unsigned char *)array) == 0) {
        index++;
        array = ((char *)array) + 1;
    }

    int subindex;
    for (subindex = 0; subindex < 8; subindex++) {
        if (Get_Bit(array, subindex)) {
            break;
        }
    }
    return index * 8 + subindex;
}


/***********************************************************************************************
 * First_False_Bit -- Find the first false bit in the bit array.                               *
 *                                                                                             *
 *    This routine will scan the bit array and return with the index of the first false (set   *
 *    to 0) bit found.                                                                         *
 *                                                                                             *
 * INPUT:   array -- Pointer to the bit array to scan.                                         *
 *                                                                                             *
 * OUTPUT:  Returns with the index of the first false bit found in the array.                  *
 *                                                                                             *
 * WARNINGS:   This routine will not stop scanning until a false bit was found. This means     *
 *             that there must be at least one false bit in the array or else it will scan     *
 *             past the end of the array.                                                      *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/06/1997 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0052EF20)

int __fastcall First_False_Bit(void const *array)
{
    int index = 0;
    while (*((unsigned char *)array) == 0xFF) {
        index++;
        array = ((char *)array) + 1;
    }

    int subindex;
    for (subindex = 0; subindex < 8; subindex++) {
        if (!Get_Bit(array, subindex)) {
            break;
        }
    }
    return index * 8 + subindex;
}
