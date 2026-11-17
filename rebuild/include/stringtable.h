/*******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 RedAlert2 contributors
 ******************************************************************************/

#pragma once

/*
**	windows.h maps LoadString to LoadStringA.
*/
#ifdef LoadString
#undef LoadString
#endif

/*
**	RA2: the languages of the string table; the number is the language field of a .csf header.
*/
enum CSFLanguages // RA2: as above
{
	CSF_US = 0, // RA2: as above
	CSF_UK = 1, // RA2: as above
	CSF_GERMAN = 2, // RA2: as above
	CSF_FRENCH = 3, // RA2: as above
	CSF_SPANISH = 4, // RA2: as above
	CSF_ITALIAN = 5, // RA2: as above
	CSF_JAPANESE = 6, // RA2: as above
	CSF_JABBERWOCKIE = 7, // RA2: as above
	CSF_KOREAN = 8, // RA2: as above
	CSF_CHINESE = 9, // RA2: as above
	CSF_UNKNOWN = 10 // RA2: ends the language table (0x7F8CF0) and is the language before a table is read
};

/*
**	RA2: the first 24 bytes of a .csf file.
*/
struct CSFHeader // RA2: as above
{
	int ID; // RA2: 'CSF ' (0x43534620)
	int Version; // RA2: versions below 2 have no language field and are US
	int LabelCount; // RA2: how many labels the file holds
	int StringCount; // RA2: how many strings the file holds in all
	int Unknown; // RA2: not read by the executable
	int Language; // RA2: a CSFLanguages value
};

/*
**	RA2: one label of the table (40 bytes); the labels are sorted by name for the search of LoadString.
*/
struct CSFLabel // RA2: as above
{
	char Name[32]; // RA2: at most 31 characters
	int StringCount; // RA2: how many strings follow under this label (LoadString uses only the first)
	int FirstString; // RA2: index of the first of them in the tables of texts and speech names
};

/*
**	RA2: the text LoadString makes up for a label it does not know; the chunks form a list so that
**	Unload can free them.
*/
struct CSFString // RA2: as above
{
	CSFString * PreviousEntry; // RA2: the chunk made before this one
	wchar_t Text[258]; // RA2: the chunk is 0x208 bytes
};

/*
**	RA2: one language of the table at 0x7F8CF0, which ends with the entry for CSF_UNKNOWN.
*/
struct CSFLanguage // RA2: as above
{
	CSFLanguages Index; // RA2: the language
	char const * Name; // RA2: its name in English
	char const * Short; // RA2: its two letter code
	char const * Letter; // RA2: its one letter code, the one stored in the strings of that language
};

/*
**	RA2 fetches its text from a string table file by label. Each lookup also passes the
**	path and line of the source that asks for it, which the game reports when a label is
**	missing; the original path of each source file is therefore part of its code.
*/
class StringTable
{
	public:
		static wchar_t const * LoadString(char const * label, char ** extra, char const * file, int line);
		static int Is_Initialized(void); // RA2: 0x6FA030, has the string table been read in yet?
		static CSFLanguage const * GetLanguage(CSFLanguages language); // RA2: 0x6F8DD0, the table entry of a language, NULL when it is not in the table
		static char const * GetLanguageName(CSFLanguages language); // RA2: 0x6F8E00, the name of a language, "unknown" when it is not in the table
		static int LoadFile(char const * filename); // RA2: 0x6F8E30, reads the table of a .csf file (or of a text file); 1 when it holds strings
		static int ReadFile(char const * filename); // RA2: 0x6F9400, reads the labels and strings of a .csf file into the tables LoadFile made
		static void Reverse_String(char * start, char * end); // RA2: 0x6F9D50, reverses the characters from start to end in place
		static void Unload(void); // RA2: 0x6F9D90, frees every table and every made-up text
};
