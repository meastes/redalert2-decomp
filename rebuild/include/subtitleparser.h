// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// Changed to RA2 1.006's layout by RedAlert2 contributors, 2026.
// RA2: the subtitle control file parser, moved out of movies.h (lane R9) so that subtitle2.cpp, its own
// object, does not include rect.h: the original has no RECT_NONE initializer at 0x0069B680.
#pragma once

#include <wchar.h>

template<class T> class DynamicVectorClass;
class Straw;
class SubTitle;

class SubTitleParserClass
{
	public:
		SubTitleParserClass(Straw & input);
		~SubTitleParserClass(void);
		DynamicVectorClass<SubTitle *> * Get_Sub_Titles(char const * moviename);

	private:
		bool Find_Movie_Entry(char const * moviename);
		bool Parse_Sub_Title(wchar_t * string, SubTitle * subtitle);
		wchar_t * Get_Next_Line(void);

		Straw & mInput;
		wchar_t mBuffer[1024];
		unsigned int mLineNumber;
};
