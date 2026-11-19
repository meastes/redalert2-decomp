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
 *                     $Archive:: /G/wwlib/PALETTE.H                                          $*
 *                                                                                             *
 *                      $Author:: Eric_c                                                      $*
 *                                                                                             *
 *                     $Modtime:: 4/02/99 12:00p                                              $*
 *                                                                                             *
 *                    $Revision:: 2                                                           $*
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#pragma once

#include "rgb.h"

/*
**	The palette class is used to manipulate a palette as a whole. All 256 colors are
**	represented by the palette class object.
*/
class PaletteClass
{
	public:
		enum {
			COLOR_COUNT=256			// Number of color indices on the palette.
		};

		PaletteClass(void) {};
		PaletteClass(RGBClass const & rgb);
		PaletteClass(unsigned char * binary_palette);

		RGBClass & operator[] (int index) {return(Palette[index % COLOR_COUNT]);};
		RGBClass const & operator[] (int index) const {return(Palette[index % COLOR_COUNT]);};
		RGBClass & Get_Color(int index) {return(Palette[index % COLOR_COUNT]);};
		RGBClass const & Get_Color(int index) const {return(Palette[index % COLOR_COUNT]);};
		bool operator == (PaletteClass const & palette) const;
		bool operator != (PaletteClass const & palette) const {return(!(operator ==(palette)));};
		PaletteClass & operator = (PaletteClass const & palette);
		operator const unsigned char * (void) const {return((const unsigned char *)&Palette[0]);};
		operator unsigned char * (void) {return((unsigned char *)&Palette[0]);};

		void Adjust(int ratio);
		void Adjust(int ratio, PaletteClass const & palette);
		void Partial_Adjust(int ratio, char *lut);
		void Partial_Adjust(int ratio, PaletteClass const & palette, char * lut);
		int Closest_Color(RGBClass const & rgb) const;

	protected:
		RGBClass Palette[COLOR_COUNT];
};

class ColorScheme;
template<class T> class DynamicVectorClass;

struct HashIterator { // RA2: where a walk over the table stands
	int BucketIndex; // RA2: +0x0
	int InBucketIndex; // RA2: +0x4
	bool OutOfBuckets; // RA2: +0x8
};

struct SchemeListIterator : public HashIterator { // RA2: the walk over the palette files' scheme lists (ion.cpp uses it for the storm tints)
	DynamicVectorClass<ColorScheme *> * Next(void); // RA2: 0x00603640
	SchemeListIterator(void) { BucketIndex = 0; InBucketIndex = 0; OutOfBuckets = false; } // RA2: as above
};

DynamicVectorClass<ColorScheme *> * __fastcall Generate_Palette(char const * name);

class HSVClass;
void __fastcall SchemeLibrary_Add(char const * name, HSVClass const & color); // RA2: records a colour scheme in the scheme library, 0x603A60 (YR: SchemeLibrary_Add)
void SchemeLibrary_Free(void); // RA2: empties the scheme library, 0x603950 (YR: SchemeLibrary_Free)

int Palette_Scheme_Count(void); // RA2: 0x00603D50, how many palettes have a scheme list
