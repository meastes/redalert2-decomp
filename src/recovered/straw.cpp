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
// From mischa85/RedAlert2 dbca015 src/straw.cpp (matching RA2 1.006 decomp; TibSun straw.cpp); modified.

/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                     $Archive:: /G/wwlib/STRAW.CPP                                          $*
 *                                                                                             *
 *                      $Author:: Eric_c                                                      $*
 *                                                                                             *
 *                     $Modtime:: 4/15/99 10:16a                                              $*
 *                                                                                             *
 *                    $Revision:: 2                                                           $*
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Straw::Get_From -- Connect one straw segment to another.                                  *
 *   Straw::Get -- Fetch some data from the straw chain.                                       *
 *   Straw::~Straw -- Destructor for a straw segment.                                          *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "shared/straw.hpp"

/***********************************************************************************************
 * Straw::~Straw -- Destructor for a straw segment.                                            *
 *                                                                                             *
 *    This destructor will remove this segment from the straw chain. If there were any         *
 *    connected segments to either side, they will be joined so that the straw chain will      *
 *    still remain linked.                                                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// The scalar deleting destructor the linker kept (compiled with this unit's rows, kept from another
// object) calls this destructor rather than inlining it; auto_inline(off) reproduces that copy.
#pragma auto_inline(off)
Straw::~Straw(void)
{
	if (ChainTo != 0) {
		ChainTo->ChainFrom = ChainFrom;
	}
	if (ChainFrom != 0) {
		ChainFrom->Get_From(ChainTo);
	}

	ChainFrom = 0;
	ChainTo = 0;
}
#pragma auto_inline(on)


/***********************************************************************************************
 * Straw::Get_From -- Connect one straw segment to another.                                    *
 *                                                                                             *
 *    Use this routine to connect straw segments together. The straw segment specified as the  *
 *    parameter will be linked to the chain such that when data is requested, it will be       *
 *    fetched from the specified link before being processed by this link.                     *
 *                                                                                             *
 * INPUT:   straw -- Pointer to the straw segment that data will be fetched from.              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void Straw::Get_From(Straw * straw)
{
	if (ChainTo != straw) {
		if (straw != 0 && straw->ChainFrom != 0) {
			straw->ChainFrom->Get_From(0);
			straw->ChainFrom = 0;
		}

		if (ChainTo != 0) {
			ChainTo->ChainFrom = 0;
		}

		ChainTo = straw;
		if (ChainTo != 0) {
			ChainTo->ChainFrom = this;
		}
	}
}


/***********************************************************************************************
 * Straw::Get -- Fetch some data from the straw chain.                                         *
 *                                                                                             *
 *    Use this routine to fetch some data from the straw. It is presumed that this routine     *
 *    will be overridden to provide some useful functionality. At this level, the straw chain  *
 *    merely passes the request on to the next straw in the chain.                             *
 *                                                                                             *
 * INPUT:   source   -- Pointer to the buffer to hold the requested data.                      *
 *                                                                                             *
 *          length   -- The number of bytes requested.                                         *
 *                                                                                             *
 * OUTPUT:  Returns with the number of bytes stored into the buffer. If the number returned    *
 *          is less than the number requested, then this indicates that the straw data has     *
 *          been exhausted.                                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int Straw::Get(void * source, int slen)
{
	if (ChainTo != 0) {
		return(ChainTo->Get(source, slen));
	}
	return(0);
}
