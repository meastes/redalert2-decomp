/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 ra2-decomp contributors
 * Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
 *
 * Derived from Electronic Arts source code; EA's GPL Section 7 terms apply.
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/

#pragma once

/*
**	RA2: the 256 byte name the color scheme library hashes its palettes under (palette.cpp). The members are
**	defined in gscreen.cpp, where ra2-decomp's file has them.
*/
class HashString
{
	public:
		HashString(void);
		HashString(char const * str);
		HashString(HashString const & that);
		HashString & operator = (HashString const & that);
		bool operator == (HashString const & that);
		bool operator != (HashString const & that);
		bool operator ! (void) const;

		char Str[256];
};

bool operator == (HashString const & a, HashString const & b);
bool operator != (HashString const & a, HashString const & b);
