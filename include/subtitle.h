// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
#pragma once

/// <summary>
/// RA2: one line of movie subtitle (0x0069B0C0): when it shows, for how long, its colour, the text line
/// and alignment it uses, and the caption.
/// </summary>
class SubTitle
{
	public:
		enum Alignment
		{
			Left, // RA2: 0, the "Left" entry of the alignment table at 0x007F62F8
			Right, // RA2: 1, the "Right" entry
			Center // RA2: 2, the "Center" entry; what a subtitle uses when none is named
		};

		SubTitle(void); // RA2: 0x0069B0C0
		~SubTitle(void); // RA2: 0x0069B0F0
		void Set_RGB_Color(unsigned char red, unsigned char green, unsigned char blue); // RA2: 0x0069B100
		void Set_Caption(wchar_t * string); // RA2: 0x0069B120

		void Set_Display_Time(unsigned long time) { TimeStamp = time; } // RA2: +0x00, in ticks of 1/60 second
		unsigned long Get_Display_Time(void) const { return TimeStamp; }
		void Set_Display_Duration(unsigned long duration) { Duration = duration; } // RA2: +0x04
		unsigned long Get_Display_Duration(void) const { return Duration; }
		unsigned long Get_RGB_Color(void) const { return RGBColor; } // RA2: +0x08
		void Set_Line_Position(int line) { LinePosition = line; } // RA2: +0x0C
		int Get_Line_Position(void) const { return LinePosition; }
		void Set_Alignment(Alignment alignment) { Align = alignment; } // RA2: +0x10
		Alignment Get_Alignment(void) const { return Align; }
		wchar_t const * Get_Caption(void) const { return Caption; } // RA2: +0x14

	private:
		unsigned long TimeStamp; // RA2: +0x00; 0 from the constructor
		unsigned long Duration; // RA2: +0x04; 1200 (20 seconds) from the constructor
		unsigned long RGBColor; // RA2: +0x08; 0x00FFFFFF (white) from the constructor
		int LinePosition; // RA2: +0x0C; 15 from the constructor
		Alignment Align; // RA2: +0x10; Center (2) from the constructor
		wchar_t * Caption; // RA2: +0x14; owned copy of the text
};

// Declarations moved from the units that used them (one declaration per name).
extern wchar_t * __fastcall wcstrim(wchar_t * buffer); // RA2: 0x006EEF80; cuts the blanks off both ends of the text in place
extern void __stdcall Parse_Token(wchar_t const * token, wchar_t * param, SubTitle * subtitle); // RA2: 0x0069BC60; defined in subtitle2.cpp
extern void __fastcall Parse_Time(wchar_t * value, SubTitle * subtitle); // RA2: 0x0069BE30; defined in subtitle2.cpp
extern void __fastcall Parse_Duration(wchar_t * value, SubTitle * subtitle); // RA2: 0x0069BE40; defined in subtitle2.cpp
extern void __fastcall Parse_Position(wchar_t * value, SubTitle * subtitle); // RA2: 0x0069BE60; defined in subtitle2.cpp
extern void __fastcall Parse_Color(wchar_t * value, SubTitle * subtitle); // RA2: 0x0069BEF0; defined in subtitle2.cpp
extern void __fastcall Parse_Text(wchar_t * value, SubTitle * subtitle); // RA2: 0x0069BF70; defined in subtitle2.cpp
class Straw;
extern bool __fastcall Read_Line(Straw & input, wchar_t * buffer, int count); // RA2: 0x0069C100; defined at the end of subtitle2.cpp
