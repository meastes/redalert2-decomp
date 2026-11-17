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
 *                     $Archive:: /Commando/Code/wwlib/mono.h                                 $*
 *                                                                                             *
 *                      $Author:: Jani_p                                                      $*
 *                                                                                             *
 *                     $Modtime:: 5/04/01 7:36p                                               $*
 *                                                                                             *
 *                    $Revision:: 5                                                           $*
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#pragma once

#include "win.h"

class MonoClass {
	public:
		enum MonoClassPageEnums {
			COLUMNS=80,						// Number of columns.
			LINES=25,						// Number of lines.
			MAX_MONO_PAGES=8				// Maximum RAM pages on mono card.
		};

		enum MonoAttribute {
			INVISIBLE=0x00, // Black on black.
			UNDERLINE=0x01, // Underline.
			BLINKING=0x90,  // Blinking white on black.
			NORMAL=0x02,    // White on black.
			INVERSE=0x70    // Black on white.
		};

		MonoClass(int handle); // RA2: 0x0059E190, TibSun has a default constructor
		~MonoClass(void);

		static void Enable(void); // RA2: 0x0059D9C0, shows the mono window
		static void Disable(void); // RA2: 0x0059DA00, hides it
		static bool Is_Enabled(void) {return(Enabled);};
		static MonoClass * Get_Page(int index); // RA2: returns the mono page of that index

		void Sub_Window(int x=0, int y=0, int w=80, int h=25);
		void Fill_Attrib(int x, int y, int w, int h, MonoAttribute attrib);
		void Clear(void);
		void Set_Cursor(int x, int y);
		void Print(char const *text);
		void Print(int text);
		void __cdecl Printf(char const *text, ...);
		void __cdecl Printf(wchar_t const *text, ...); // RA2: 0x0059E840, takes a wide format string where TibSun takes a text number
		void Text_Print(char const *text, int x, int y, MonoAttribute attrib=NORMAL);
		void Text_Print(wchar_t const *text, int x, int y, MonoAttribute attrib=NORMAL); // RA2: 0x0059E8E0, prints a wide string where TibSun takes a text number
		void View(void);
		void Scroll(int lines=1);
		void Pan(int cols=1);
		void Set_Default_Attribute(MonoAttribute attrib);
		int Get_Handle(void) const; // RA2: 0x0059E230, the page number
		bool Is_Changed(void) const; // RA2: 0x0059E2E0, set by Clear, cleared by Print
		char Map_Character(char character); // RA2: 0x0059E960, a member that does not use this: the IBM line drawing characters become + | - =
		void Refresh(int x, int y, int w, int h); // RA2: 0x0059EA90, redraws a block of cells on the mono window
		void Draw_Character(HDC dc, int x, int y); // RA2: 0x0059E9D0, draws one cell

		/*
		**	This merely makes a duplicate of the mono object into a newly created mono
		**	object.
		*/
		MonoClass (MonoClass const &);

		static MonoClass * Current;

	private:

		/*
		**	The page of RA2 is a grid of characters with an attribute for each, drawn on a
		**	window instead of a monochrome card.
		*/
		char Characters[COLUMNS][LINES]; // RA2: +0x0000, by column then line
		int Attributes[COLUMNS][LINES]; // RA2: +0x07D0
		int Attribute; // RA2: +0x2710, the attribute new text gets
		int DrawnAttribute; // RA2: +0x2714, the attribute the device context is set up for

		/*
		**	Handle of the mono page.
		*/
		int Handle; // RA2: +0x2718, the page number
		int X; // RA2: +0x271C, cursor column
		int Y; // RA2: +0x2720, cursor line
		int WindowX; // RA2: +0x2724, the sub-window
		int WindowY; // RA2: +0x2728
		int WindowWidth; // RA2: +0x272C
		int WindowHeight; // RA2: +0x2730
		bool Changed; // RA2: +0x2734

		/*
		**	If this is true, then monochrome output is allowed. It defaults to false
		**	so that monochrome output must be explicitly enabled.
		*/
		static bool Enabled;

		MonoClass & operator = (MonoClass const & );
};
