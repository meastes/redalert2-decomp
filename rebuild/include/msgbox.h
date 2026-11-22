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

/* $Header: /CounterStrike/MSGBOX.H 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : OPTIONS.H                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : June 8, 1994                                                 *
 *                                                                                             *
 *                  Last Update : June 8, 1994   [JLB]                                         *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#pragma once

#include "language\language.h"
#include "win.h"

class WWMessageBox
{
		wchar_t const * Caption; // RA2: wide text

	public:
		WWMessageBox(wchar_t const * caption = L" ") {Caption = caption;}; // RA2: wide caption, a space by default

		int Process(wchar_t const * msg, wchar_t const * b1txt, wchar_t const * b2txt, wchar_t const * b3txt, bool preserve = false); // RA2: wide text

};

bool Prompt_Dialog(wchar_t const * text); // RA2: 0x00600C80; shows the text and returns false when the player cancels

class Surface;
void __fastcall Darken_Surface(Surface * surface, bool update); // RA2: 0x5159B0, defined in init.cpp
