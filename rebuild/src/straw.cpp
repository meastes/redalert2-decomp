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

#include "always.h"

#include "straw.h"

#include <cstddef>
#include <cstring>
#include <wchar.h> // RA2: wcslen, wcscpy for SubTitle
#include "subtitle.h" // RA2: SubTitle


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
Straw::~Straw(void)
{
	if (ChainTo != NULL) {
		ChainTo->ChainFrom = ChainFrom;
	}
	if (ChainFrom != NULL) {
		ChainFrom->Get_From(ChainTo);
	}

	ChainFrom = NULL;
	ChainTo = NULL;
}


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
		if (straw != NULL && straw->ChainFrom != NULL) {
			straw->ChainFrom->Get_From(NULL);
			straw->ChainFrom = NULL;
		}

		if (ChainTo != NULL) {
			ChainTo->ChainFrom = NULL;
		}

		ChainTo = straw;
		if (ChainTo != NULL) {
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
	if (ChainTo != NULL) {
		return(ChainTo->Get(source, slen));
	}
	return(0);
}

/// <summary>
/// RA2: a subtitle that shows at once, for 20 seconds, in white, on line 15, centered, without text.
/// </summary>
SubTitle::SubTitle(void) // RA2: 0x0069B0C0
	: TimeStamp(0),
	Duration(20 * 60),
	RGBColor(0x00FFFFFF),
	LinePosition(15),
	Align(Center),
	Caption(NULL)
{
}

/// <summary>
/// RA2: frees the caption.
/// </summary>
SubTitle::~SubTitle(void) // RA2: 0x0069B0F0
{
	if (Caption != NULL) {
		delete[] Caption;
	}
}

/// <summary>
/// RA2: sets the colour of the text as 0x00RRGGBB.
/// </summary>
void SubTitle::Set_RGB_Color(unsigned char red, unsigned char green, unsigned char blue) // RA2: 0x0069B100
{
	RGBColor = (((unsigned long)red << 16) | ((unsigned long)green << 8) | (unsigned long)blue);
}

/// <summary>
/// RA2: replaces the caption with a copy of the text.
/// </summary>
void SubTitle::Set_Caption(wchar_t * string) // RA2: 0x0069B120
{
	if (Caption != NULL) {
		delete[] Caption;
		Caption = NULL;
	}

	if (string != NULL) {
		unsigned int length = wcslen(string);
		Caption = new wchar_t[length + 1];

		if (Caption != NULL) {
			wcscpy(Caption, string);
		}
	}
}

