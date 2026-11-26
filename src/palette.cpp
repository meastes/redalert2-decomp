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

/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                     $Archive:: /Commando/Library/PALETTE.CPP                               $*
 *                                                                                             *
 *                      $Author:: Greg_h                                                      $*
 *                                                                                             *
 *                     $Modtime:: 7/22/97 11:37a                                              $*
 *                                                                                             *
 *                    $Revision:: 1                                                           $*
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   PaletteClass::Adjust -- Adjusts the palette toward another palette.                       *
 *   PaletteClass::Adjust -- Adjusts this palette toward black.                                *
 *   PaletteClass::Closest_Color -- Finds closest match to color specified.                    *
 *   PaletteClass::PaletteClass -- Constructor that fills palette with color specified.        *
 *   PaletteClass::operator = -- Assignment operator for palette objects.                      *
 *   PaletteClass::operator == -- Equality operator for palette objects.                       *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "always.h"

#include "palette.h"
#include "hashtable.h"
#include "scheme.h"

#include <cstring>
#include "globals.h"
#include "_theater.h"
#include "_palette.h"
#include "mixfile.h"


/***********************************************************************************************
 * PaletteClass::PaletteClass -- Constructor that fills palette with color specified.          *
 *                                                                                             *
 *    This constructor will fill the palette with the color specified.                         *
 *                                                                                             *
 * INPUT:   rgb   -- Reference to the color to fill the entire palette with.                   *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/02/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
PaletteClass::PaletteClass(RGBClass const & rgb)
{
	for (int index = 0; index < COLOR_COUNT; index++) {
		Palette[index] = rgb;
	}
}


/// <summary>
/// Constructs a palette from a raw block of palette bytes.
/// This routine is used when the colors arrive as a flat run of bytes -- straight out of
/// a file or a resource -- rather than as a palette object.
/// </summary>
/// <param name="binary_palette">Pointer to the raw palette bytes to copy from.</param>
/// <remarks>Be sure the source block holds a full palette's worth of bytes.</remarks>
PaletteClass::PaletteClass(unsigned char * binary_palette)
{
	memcpy(&Palette[0], binary_palette, sizeof(Palette));
}


/***********************************************************************************************
 * PaletteClass::operator == -- Equality operator for palette objects.                         *
 *                                                                                             *
 *    This is the comparison for equality operator. It will compare palette objects to         *
 *    determine if they are identical.                                                         *
 *                                                                                             *
 * INPUT:   palette  -- Reference to the palette to compare to this palette.                   *
 *                                                                                             *
 * OUTPUT:  Are the two palettes identical?                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/02/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool PaletteClass::operator == (PaletteClass const & palette) const
{
	if (this == &palette) return(true);
	return(memcmp(&Palette[0], &palette.Palette[0], sizeof(Palette)) == 0);
}


/***********************************************************************************************
 * PaletteClass::operator = -- Assignment operator for palette objects.                        *
 *                                                                                             *
 *    This is the assignment operator for palette objects. Although the default C++ generated  *
 *    assignment operator would function correctly, it would not check for self-assignment     *
 *    and thus this routine can be faster.                                                     *
 *                                                                                             *
 * INPUT:   palette  -- Reference to that palette that will be copied into this palette.       *
 *                                                                                             *
 * OUTPUT:  Returns with a reference to the newly copied to palette.                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/02/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
PaletteClass & PaletteClass::operator = (PaletteClass const & palette)
{
	if (this == &palette) return(*this);

	memcpy(&Palette[0], &palette.Palette[0], sizeof(Palette));
	return(*this);
}


/***********************************************************************************************
 * PaletteClass::Adjust -- Adjusts this palette toward black.                                  *
 *                                                                                             *
 *    This routine is used to adjust this palette toward black. Typical use of this routine    *
 *    is when fading the palette to black.                                                     *
 *                                                                                             *
 * INPUT:   ratio -- The ratio to fade this palette to black. 0 means no fading at all. 255    *
 *                   means 100% faded to black.                                                *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine doesn't actually set the palette to the video card. Use the Set()  *
 *             function to achieve that purpose.                                               *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/02/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void PaletteClass::Adjust(int ratio)
{
	for (int index = 0; index < COLOR_COUNT; index++) {
		Palette[index].Adjust(ratio, BlackColor);
	}
}


/***********************************************************************************************
 * PaletteClass::Adjust -- Adjusts the palette toward another palette.                         *
 *                                                                                             *
 *    This routine is used to adjust a palette toward a destination palette by the ratio       *
 *    specified. This is primarily used by the palette fading routines.                        *
 *                                                                                             *
 * INPUT:   palette  -- Reference to the destination palette.                                  *
 *                                                                                             *
 *          ratio    -- The ratio to adjust this palette toward the destination palette. A     *
 *                      value of 0 means no adjustment at all. A value of 255 means 100%       *
 *                      adjustment.                                                            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/02/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void PaletteClass::Adjust(int ratio, PaletteClass const & palette)
{
	for (int index = 0; index < COLOR_COUNT; index++) {
		Palette[index].Adjust(ratio, palette[index]);
	}
}


/***********************************************************************************************
 * PaletteClass::Partial_Adjust -- Adjusts the specified parts of this palette toward black.   *
 *                                                                                             *
 *    This routine is used to adjust this palette toward black. Typical use of this routine    *
 *    is when fading the palette to black. The input lookup table is used to determine         *
 *    which entries should fade and which should stay the same                                 *
 *                                                                                             *
 * INPUT:   ratio -- The ratio to fade this palette to black. 0 means no fading at all. 255    *
 *                   means 100% faded to black.                                                *
 *                                                                                             *
 *          lookup   -- ptr to lookup table                                                    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine doesn't actually set the palette to the video card. Use the Set()  *
 *             function to achieve that purpose.                                               *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/02/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void PaletteClass::Partial_Adjust(int ratio, char *lut)
{
	for (int index = 0; index < COLOR_COUNT; index++) {
		if (lut[index]) {
			Palette[index].Adjust(ratio, BlackColor);
		}
	}
}


/***********************************************************************************************
 * PaletteClass::Partial_Adjust -- Adjusts the palette toward another palette.                 *
 *                                                                                             *
 *    This routine is used to adjust a palette toward a destination palette by the ratio       *
 *    specified. This is primarily used by the palette fading routines.  The input lookup      *
 *    table is used to determine which entries should fade and which should stay the same      *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   palette  -- Reference to the destination palette.                                  *
 *                                                                                             *
 *          ratio    -- The ratio to adjust this palette toward the destination palette. A     *
 *                      value of 0 means no adjustment at all. A value of 255 means 100%       *
 *                      adjustment.                                                            *
 *                                                                                             *
 *          lookup   -- ptr to lookup table                                                    *
 *                                                                                             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/02/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void PaletteClass::Partial_Adjust(int ratio, PaletteClass const & palette, char *lut)
{
	for (int index = 0; index < COLOR_COUNT; index++) {
		if (lut[index]) {
			Palette[index].Adjust(ratio, palette[index]);
		}
	}
}


/***********************************************************************************************
 * PaletteClass::Closest_Color -- Finds closest match to color specified.                      *
 *                                                                                             *
 *    This routine will examine the palette and return with the color index number for the     *
 *    color that most closely matches the color specified. Remap operations rely heavily on    *
 *    this routine to allow working with a constant palette.                                   *
 *                                                                                             *
 * INPUT:   rgb   -- Reference to a color to search for in the current palette.                *
 *                                                                                             *
 * OUTPUT:  Returns with a color index value to most closely matches the specified color.      *
 *                                                                                             *
 * WARNINGS:   This routine will quite likely not find an exact match.                         *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/02/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int PaletteClass::Closest_Color(RGBClass const & rgb) const
{
	int closest = 0;
	int value = -1;

	RGBClass const * ptr = &Palette[0];
	for (int index = 0; index < COLOR_COUNT; index++) {
		int difference = rgb.Difference(*ptr++);
		if (value == -1 || difference < value) {
			value = difference;
			closest = index;
		}
	}
	return(closest);
}


/*
**	RA2: the color scheme library. Every palette file that has color schemes is generated once per theater and kept
**	in a hash table under the file's name. Nothing of this is in the Tiberian Sun reconstruction, so each line
**	below is what the executable shows (0x00603200 to 0x00603FA0, without Set_Scheme_Table_Theater at 0x00603750).
*/
#include "hashstr.h" // RA2: the table key (ra2-decomp's header; its members are gscreen.cpp's)


class SchemeNode { // RA2: the schemes generated from one palette file
	public:
		SchemeNode(char const * name, DynamicVectorClass<ColorScheme *> * schemes); // RA2: 0x00603200
		~SchemeNode(void); // RA2: 0x00603240
		void Replace(DynamicVectorClass<ColorScheme *> * schemes); // RA2: 0x006032A0
		void Release(void); // RA2: 0x00603320

		char Name[256]; // RA2: +0x0
		DynamicVectorClass<ColorScheme *> * Schemes; // RA2: +0x100
};

typedef HashTableClass<HashString, SchemeNode *> SchemeHashTable;

struct SchemeTable : public SchemeHashTable { // RA2: the members of the table that the executable has out of line
	SchemeNode ** Find(HashString const & key); // RA2: 0x006050C0
	HashString * Next(HashIterator * iterator); // RA2: 0x00605120
};

int __fastcall SchemeHash(HashString const & key); // RA2: 0x00603610, defined below

static inline bool Add_Node(SchemeTable * table, HashString const & key, SchemeNode * const & node) // RA2: files a node under its name, the executable has it inline
{
	SchemeHashTable::BucketType & bucket = table->Buckets[table->HashFunction(key) % table->NumBuckets];
	bool found = false;
	for (int index = bucket.Count() - 1; index >= 0; index--) {
		if (bucket[index].Key == key) {
			found = true;
			break;
		}
	}
	if (found) {
		return(false);
	}
	return(bucket.Add(SchemeHashTable::ObjectType(key, node)));
}

class SchemeLibrary { // RA2: the table of palette files and the list of player colors
	public:
		SchemeLibrary(void) : // RA2: 0x00604120
			Mode(-1),
			Names(new DynamicVectorClass<char *>),
			Colors(new DynamicVectorClass<HSVClass>),
			Table((SchemeTable *)new SchemeHashTable(10, 31, (SchemeHashTable::HASH_FUNC)SchemeHash))
		{
		}
		~SchemeLibrary(void); // RA2: 0x00604250

		DynamicVectorClass<ColorScheme *> * Lookup(char const * name); // RA2: 0x006044A0
		DynamicVectorClass<ColorScheme *> * Generate(char const * name); // RA2: 0x00604540
		void Add(char const * name); // RA2: 0x00604870
		void Replace(char const * name); // RA2: 0x006049C0
		void Set_Mode(int mode); // RA2: 0x00604AD0

		int Mode; // RA2: +0x0, the theater, -1 before the first Set_Scheme_Table_Theater
		DynamicVectorClass<char *> * Names; // RA2: +0x4, the names of the player colors
		DynamicVectorClass<HSVClass> * Colors; // RA2: +0x8, the player colors
		SchemeTable * Table; // RA2: +0xC
};


inline SchemeLibrary * Scheme_Library(void) // RA2: the library is made on first use
{
	if (TheSchemeLibrary == NULL) {
		TheSchemeLibrary = new SchemeLibrary;
	}
	return(TheSchemeLibrary);
}

SchemeNode::SchemeNode(char const * name, DynamicVectorClass<ColorScheme *> * schemes) // RA2: 0x00603200
{
	strcpy(Name, name);
	Schemes = schemes;
}


SchemeNode::~SchemeNode(void) // RA2: 0x00603240, only deletes the schemes
{
	Release();
}


void SchemeNode::Replace(DynamicVectorClass<ColorScheme *> * schemes) // RA2: 0x006032A0, deletes the schemes and takes new ones
{
	Release();
	Schemes = schemes;
}


void SchemeNode::Release(void) // RA2: 0x00603320, deletes the schemes (the executable calls it for the delete of a node too)
{
	if (Schemes) {
		int count = Schemes->Count();
		for (int index = 0; index < count; index++) {
			delete (*Schemes)[index];
		}
		delete Schemes;
		Schemes = NULL;
	}
}


DynamicVectorClass<ColorScheme *> * __fastcall Generate_Palette(char const * name) // RA2: 0x00603380, the schemes of a palette file, generated on first use
{
	SchemeLibrary * library = Scheme_Library();
	DynamicVectorClass<ColorScheme *> * schemes = library->Lookup(name);
	if (schemes != NULL) {
		return(schemes);
	}
	TheSchemeLibrary->Add(name);
	return(TheSchemeLibrary->Lookup(name));
}


int __fastcall SchemeHash(HashString const & key) // RA2: 0x00603610, the bucket of a palette name
{
	char const * str = key.Str;
	int hash = 0;
	while (*str) {
		hash = (*str++ + hash * 26) % 31;
	}
	return(hash);
}



DynamicVectorClass<ColorScheme *> * SchemeListIterator::Next(void) // RA2: 0x00603640, the next palette's schemes
{
	HashString * key = TheSchemeLibrary->Table->Next(this);
	if (key == NULL) {
		return(NULL);
	}
	SchemeNode ** node = TheSchemeLibrary->Table->Find(*key);
	if (node == NULL) {
		return(NULL);
	}
	SchemeNode * entry = *node;
	return(entry->Schemes);
}


void __fastcall Set_Scheme_Table_Theater(TheaterType theater) // RA2: 0x00603750, switches the library to the theater (made on first use)
{
	Scheme_Library();
	if (TheSchemeLibrary->Mode != theater) {
		TheSchemeLibrary->Set_Mode(theater);
	}
}


void SchemeLibrary_Free(void) // RA2: 0x00603950, empties the lists of player colors
{
	SchemeLibrary * library = Scheme_Library();
	int count = library->Names->Count();
	for (int index = 0; index < count; index++) {
		delete (*TheSchemeLibrary->Names)[index];
	}
	TheSchemeLibrary->Names->Clear();
	TheSchemeLibrary->Colors->Clear();
}


void __fastcall SchemeLibrary_Add(char const * name, HSVClass const & color) // RA2: 0x00603A60, records a player color
{
	Scheme_Library();
	char * copy = new char[256];
	strcpy(copy, name);
	TheSchemeLibrary->Names->Add(copy);
	TheSchemeLibrary->Colors->Add(color);
}


int Scheme_List_Count(void) // RA2: 0x00603C10, how many player colors there are
{
	return(Scheme_Library()->Names->Count());
}


int Palette_Scheme_Count(void) // RA2: 0x00603D50, how many palettes have a scheme list
{
	Scheme_Library(); // RA2: makes the library on first use (ra2-decomp's twin FUN_00626DA0 calls it bare)
	SchemeListIterator it;
	int count = 0;
	DynamicVectorClass<ColorScheme *> * schemes = it.Next();
	while (schemes != NULL) {
		count++;
		schemes = it.Next();
	}
	return(count);
}


void SchemeLibrary_Destroy(void) // RA2: 0x00603FA0, deletes the library
{
	if (TheSchemeLibrary != NULL) {
		delete TheSchemeLibrary;
		TheSchemeLibrary = NULL;
	}
}


void SchemeLibrary_Create(void) // RA2: 0x00603FD0, makes the library unless it exists
{
	Scheme_Library();
}


SchemeLibrary::~SchemeLibrary(void) // RA2: 0x00604250, frees the player colors, every node and its schemes, then the table
{
	if (Table != NULL) {
		SchemeLibrary_Free();
		delete Names;
		delete Colors;

		SchemeListIterator it;
		HashString * key = Table->Next(&it);
		while (key != NULL) {
			SchemeNode ** node = Table->Find(*key);
			delete *node;
			key = Table->Next(&it);
		}
		Table->Clear();
		delete Table;
	}
}


DynamicVectorClass<ColorScheme *> * SchemeLibrary::Lookup(char const * name) // RA2: 0x006044A0, the schemes of a palette file, NULL when it is not in the table
{
	HashString key(name);
	SchemeNode ** node = Table->Find(key);
	if (node == NULL) {
		return(NULL);
	}
	return((*node)->Schemes);
}


DynamicVectorClass<ColorScheme *> * SchemeLibrary::Generate(char const * name) // RA2: 0x00604540, the schemes of a palette file for the theater
{
	if (Mode == -1) {
		return(NULL);
	}

	char path[256];
	strcpy(path, name);
	strcat(path, Theaters[Mode].Suffix);
	strcat(path, ".");
	strcat(path, PaletteExtension);
	strupr(path);

	PaletteClass pal;
	void const * file = MixFileClass::Retrieve(path);
	if (file == NULL) {
		return(NULL);
	}
	memcpy(&pal, file, sizeof(pal));

	for (int slot = 0; slot < 256; slot++) {
		RGBClass & entry = pal[slot];
		RGBClass color = entry;
		entry.Set_Red(color.Get_Red() << 2);
		entry.Set_Green(color.Get_Green() << 2);
		entry.Set_Blue(color.Get_Blue() << 2);
	}

	int count = Colors->Count();
	DynamicVectorClass<ColorScheme *> * schemes = new DynamicVectorClass<ColorScheme *>(20);
	for (int index = 0; index < count; index++) {
		char * id = (*Names)[index];
		HSVClass base = (*Colors)[index];
		ColorScheme * scheme = new ColorScheme(id, base, pal, GamePalette, 1, false);
		scheme->ID = index * 2;
		schemes->Add(scheme);
		scheme = new ColorScheme(id, base, pal, GamePalette, 53, false);
		scheme->ID = index * 2 + 1;
		schemes->Add(scheme);
	}
	return(schemes);
}


void SchemeLibrary::Add(char const * name) // RA2: 0x00604870, generates the schemes of a palette file and files them under its name
{
	DynamicVectorClass<ColorScheme *> * schemes = Generate(name);
	SchemeNode * node = new SchemeNode(name, schemes);
	Add_Node(Table, name, node);
}


void SchemeLibrary::Replace(char const * name) // RA2: 0x006049C0, generates the schemes of a filed palette again
{
	HashString key(name);
	SchemeNode ** node = Table->Find(key);
	SchemeNode * entry = *node;
	entry->Replace(Generate(name));
}


void SchemeLibrary::Set_Mode(int mode) // RA2: 0x00604AD0, the theater; every filed palette's schemes are generated again
{
	Mode = mode;
	SchemeListIterator it;
	HashString * key = Table->Next(&it);
	while (key != NULL) {
		Replace(key->Str);
		key = Table->Next(&it);
	}
}


SchemeNode ** SchemeTable::Find(HashString const & key) // RA2: 0x006050C0, the node filed under a name
{
	SchemeHashTable::BucketType & bucket = Buckets[HashFunction(key) % NumBuckets];
	for (int index = bucket.Count() - 1; index >= 0; index--) {
		SchemeHashTable::ObjectType * object = &bucket[index];
		if (object->Key == key) {
			return(&object->Value);
		}
	}
	return(NULL);
}


HashString * SchemeTable::Next(HashIterator * it) // RA2: 0x00605120, the next name in the walk over the table
{
	if (!it->OutOfBuckets) {
		while (it->BucketIndex < NumBuckets) {
			SchemeHashTable::BucketType & bucket = Buckets[it->BucketIndex];
			while (it->InBucketIndex < bucket.Count()) {
				SchemeHashTable::ObjectType * object = &bucket[it->InBucketIndex];
				it->InBucketIndex++;
				if (!!object->Key) {
					return(&object->Key);
				}
			}
			it->BucketIndex++;
			it->InBucketIndex = 0;
		}
		it->OutOfBuckets = true;
	}
	return(NULL);
}
