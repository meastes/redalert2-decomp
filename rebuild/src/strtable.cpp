/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/

/*
**	RA2: the string table: the localized text of the game, read from a .csf file (or, when the
**	-str switch allows it, from a plain text file) into a table sorted by label, and found again
**	by StringTable::LoadString. The file ends with the helpers that turn narrow text into wide text
**	and back. TibSun has no such file; the code is the executable's from 0x006F8DD0.
*/

#define _WCTYPE_INLINE_DEFINED // RA2: the executable calls the library's iswspace (0x00784AA8); the inline version in wchar.h would call iswctype instead
#include "geometry.h" // RA2: the geometry constants come before the rectangle of rect.h (the executable initializes them first)
#include "always.h" // RA2: the includes this file needs
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <wchar.h>
#include <windows.h>
#undef LoadString // RA2: windows.h maps LoadString to LoadStringA
#undef getc // RA2: the executable calls the library function getc (0x00784A3C), not the macro of stdio.h
#include "rawfile.h"
#include "ccfile.h"
#include "session.h"
#include "mapgen.h"
#include "dbgprint.h"
#include "stringtable.h"


int MaxLabelLen; // RA2: 0x00AD0428; the longest label name read so far
char const * StringFileName; // RA2: 0x00AD0434; the file LoadFile was asked for, which Set_Jabber_Option and Set_Str_Option load again
int StringLabelCount; // RA2: 0x00AD0438; how many labels the tables have room for
int StringValueCount; // RA2: 0x00AD043C; how many strings the tables have room for
CSFLabel * StringLabels; // RA2: 0x00AD0440; the labels, sorted by name
wchar_t ** StringValues; // RA2: 0x00AD0444; the text of each string
char ** StringExtraValues; // RA2: 0x00AD0448; the speech name of each string
int StringsLoaded; // RA2: 0x00AD044C; set by the first LoadFile
int JabberOption; // RA2: 0x00AD0450; set by Set_Jabber_Option
CSFString * LastLoadedString; // RA2: 0x00AD0454; the latest text LoadString made up for a missing label
CSFString * StringChunkList; // RA2: 0x00AD0458; a second list of such texts; Unload frees it but nothing fills it
int StrOption = 1; // RA2: 0x007F8CE8; initialized to 1: a text file is allowed until -nostr says otherwise
CSFLanguages StringLanguage = CSF_UNKNOWN; // RA2: 0x007F8DA0; initialized to 10, the language before a file is read

CSFLanguage Languages[11] = { // RA2: 0x007F8CF0; the table ends with the entry for CSF_UNKNOWN, which has no codes
	{ CSF_US, "US", "us", "e" },
	{CSF_UK, "UK", "uk", "e"}, // RA2: the strings of the UK language are the English ones
	{ CSF_GERMAN, "German", "ge", "g" },
	{ CSF_FRENCH, "French", "fr", "f" },
	{ CSF_SPANISH, "Spanish", "sp", "s" },
	{ CSF_ITALIAN, "Italian", "it", "i" },
	{ CSF_JAPANESE, "Japanese", "ja", "j" },
	{CSF_JABBERWOCKIE, "Jabberwockie", "jb", "e"}, // RA2: the test language reads like English
	{ CSF_KOREAN, "Korean", "ko", "k" },
	{ CSF_CHINESE, "Chinese", "ch", "c" },
	{ CSF_UNKNOWN, "Unknown", NULL, NULL }
};

char NarrowBuffers[8][1024]; // RA2: 0x00AC136C; the eight results Wide_To_Narrow hands out in turn
wchar_t WideBuffers[8][1024]; // RA2: 0x00AC53FC; the eight results Narrow_To_Wide hands out in turn
wchar_t CSFWideText[8192]; // RA2: 0x00AC93FC; the text of one string while it is made ready
char TextLine[4096]; // RA2: 0x00ACF410; one line of a text file, and the label of a .csf file
int NarrowIndex; // RA2: 0x00AD045C; which of the eight buffers Wide_To_Narrow uses next
int WideIndex; // RA2: 0x00AD0460; which of the eight buffers Narrow_To_Wide uses next
#ifdef RA2_NON_MATCHING // used only by the parked LoadFile and Read_Text_File drafts (DECISIONS: a global whose only user is a parked function)
char TextBuffer[4104]; // RA2: 0x00AC3378; the text of one entry of a text file (the executable gives only the distance to the next object)
char LabelBuffer[4104]; // RA2: 0x00AC4380; the speech name of one entry of a text file
char CSFName[116]; // RA2: 0x00AC5388; the file name with its extension replaced by .csf
char ReversedText[8192]; // RA2: 0x00ACD410; the text of one entry with its runs of letters reversed
FILE * TextFile; // RA2: 0x00AD0430; the text file being read
#endif

/// <summary>
/// The table entry of a language, or NULL when the language is not in the table.
/// </summary>
CSFLanguage const * StringTable::GetLanguage(CSFLanguages language) // RA2: 0x006F8DD0
{
	for (CSFLanguage const * entry = Languages; entry->Index != CSF_UNKNOWN; entry++) { // RA2: the table ends with the entry of CSF_UNKNOWN
		if (entry->Index == language) {
			return entry;
		}
	}
	return NULL;
}

/// <summary>
/// The name of a language, or "unknown" when the language is not in the table.
/// </summary>
char const * StringTable::GetLanguageName(CSFLanguages language) // RA2: 0x006F8E00
{
	CSFLanguage const * entry = GetLanguage(language); // RA2: its expansion is the loop at 0x006F8E00
	if (entry != NULL) {
		return entry->Name; // RA2: the name is the second field of the entry
	}
	return "unknown"; // RA2: the literal at 0x007CCCE0
}

/// <summary>
/// Drops the spaces that start a line, repeat, or end a line from a wide string, in place.
/// </summary>
/// <remarks>From Generals GameText.cpp (GameTextManager::stripSpaces), Copyright 2025 Electronic Arts Inc.,
/// GPL-3.0-or-later; modified: this tree's names.</remarks>
static inline void Collapse_White_Space(wchar_t * string)
{
	wchar_t * str;
	wchar_t * ptr;
	wchar_t ch;
	wchar_t last = 0;
	int skipall = TRUE;

	str = ptr = string;

	while ((ch = *ptr++) != 0) {
		if (ch == ' ') {
			if (last == ' ' || skipall) {
				continue;
			}
		}

		if (ch == '\n' || ch == '\t') {
			// remove last space
			if (last == ' ') {
				str--;
			}

			skipall = TRUE;		// skip all spaces
			last = *str++ = ch;
			continue;
		}

		last = *str++ = ch;
		skipall = FALSE;
	}

	if (last == ' ') {
		str--;
	}

	*str = 0;
}

/// <summary>
/// Reads the header of a .csf file: how many labels and strings it holds, and its language. False when
/// the file is missing or is no .csf file.
/// </summary>
static int Read_CSF_Header(char const * name)
{
	CCFileClass file(name); // RA2: its expansion is at 0x006F8EE1 in LoadFile; the failing paths call the destructor
	int ok = 0;
	if (file.Is_Available() && file.Open()) {
		CSFHeader header;
		if (file.Read(&header, sizeof(header)) == sizeof(header) && header.ID == 'CSF ') { // RA2: 24 bytes that start with 0x43534620
			StringLabelCount = header.LabelCount;
			StringValueCount = header.StringCount;
			StringLanguage = (CSFLanguages)(header.Version < 2 ? 0 : header.Language); // RA2: versions below 2 have no language field and are US
			ok = 1;
		}
		return ok;
	}
	return 0;
}

/// <summary>
/// Drops the white space from both ends of a line, in place.
/// </summary>
/// <remarks>From Generals GameText.cpp (GameTextManager::removeLeadingAndTrailing), Copyright 2025 Electronic Arts Inc.,
/// GPL-3.0-or-later; modified: this tree's names.</remarks>
static void Strip_Line(char * line)
{
	char * first;
	char * ptr;
	char ch;

	ptr = first = line;

	while ((ch = *first) != 0 && iswspace(ch)) {
		first++;
	}

	while ((*ptr++ = *first++) != 0);

	ptr -= 2;

	while ((ptr > line) && (ch = *ptr) != 0 && iswspace(ch)) {
		ptr--;
	}

	ptr++;
	*ptr = 0;
}

void __fastcall Read_Text_Entry(FILE * file, char * source, char * text, char * label, int limit); // RA2: 0x006F92A0, defined below
int __fastcall Read_Text_File(void); // RA2: 0x006F9790, defined below

#ifdef RA2_NON_MATCHING // not exact: one call differs (the failing exit of the expanded Read_CSF_Header calls ~CCFileClass 0x51AD20 in the image and expands it here); 777 bytes differ by position, 1,168 B against 1,134. The image's LoadFile holds two more inline sites behind the header helper than this text: it is exact (measured, not landed) with any two of: Read_Text_File file-static, ReadFile file-static, the option read through Get_Str_Option(). Text of the counting pass and of the choice of format from Generals GameText.cpp (getStringCount, init), modified for RA2: the .csf file first, two counts, the tables allocated one by one
#define STRING_FILE 0
#define CSF_FILE 1

/// <summary>
/// Counts the labels and strings of the text file: every END closes a label, every quoted line starts a
/// string. Leaves room for 500 more labels and 1000 more strings. False when the file cannot be opened.
/// </summary>
/// <remarks>From Generals GameText.cpp (GameTextManager::getStringCount), Copyright 2025 Electronic Arts Inc.,
/// GPL-3.0-or-later; modified: RA2's file and globals, and a count of the strings beside the count of the labels.</remarks>
static int Get_String_Count(char const * filename)
{
	int ok = TRUE;

	TextFile = fopen(filename, "r");

	if (TextFile == NULL) {
		return FALSE;
	}

	StringLabelCount = 0;
	StringValueCount = 0;

	while (ok) {
		if (!fgets(TextLine, 0xFFF, TextFile)) {
			break;
		}
		Strip_Line(TextLine);

		if (TextLine[0] == '"') {
			int len = strlen(TextLine);
			TextLine[len] = '\n';
			TextLine[len + 1] = 0;
			Read_Text_Entry(TextFile, &TextLine[1], TextBuffer, LabelBuffer, 0x1000);
			StringValueCount++;
		} else if (!_stricmp(TextLine, "END")) {
			StringLabelCount++;
		}
	}

	StringLabelCount += 500;
	StringValueCount += 1000;
	fclose(TextFile);
	return TRUE;
}

/// <summary>
/// Reads the string table: from the .csf file named like the given file when there is one, else (when
/// -str allows it) from the text file itself. The tables are sorted by label at the end. 1 when the table
/// holds strings.
/// </summary>
/// <remarks>The choice between the two file formats follows Generals GameText.cpp (GameTextManager::init),
/// Copyright 2025 Electronic Arts Inc., GPL-3.0-or-later; modified: RA2 tries the .csf file first.</remarks>
int StringTable::LoadFile(char const * filename) // RA2: 0x006F8E30
{
	if (StringsLoaded != 0) { // RA2: the table is read only once until Unload
		return 1;
	}
	StringsLoaded = 1; // RA2: set even when the reading fails below
	MaxLabelLen = 0;
	StringFileName = filename;
	if (filename == 0 || *filename == 0) {
		return 0;
	}
	strcpy(CSFName, filename);
	char * dot = strchr(CSFName, '.'); // RA2: the extension is replaced by .csf
	if (dot != 0) {
		*dot = 0;
	}
	strcat(CSFName, ".csf"); // RA2: the literal at 0x007F8E68
	int format;

	if (Read_CSF_Header(CSFName)) {
		format = CSF_FILE;
	} else if (StrOption && Get_String_Count(filename)) { // RA2: no .csf file, and -nostr forbids the text file
		format = STRING_FILE;
	} else {
		return 0;
	}

	if (StringValueCount == 0 || StringLabelCount == 0) {
		return 0;
	}
	StringLabels = (CSFLabel *)::operator new(StringLabelCount * sizeof(CSFLabel)); // RA2: the three tables are made one after the other
	if (StringLabels == 0) {
		return 0;
	}
	memset(StringLabels, 0, StringLabelCount * sizeof(CSFLabel));
	StringValues = (wchar_t **)::operator new(StringValueCount * sizeof(wchar_t *));
	if (StringValues == 0) {
		return 0;
	}
	memset(StringValues, 0, StringValueCount * sizeof(wchar_t *));
	StringExtraValues = (char **)::operator new(StringValueCount * sizeof(char *));
	if (StringExtraValues == 0) {
		return 0;
	}
	memset(StringExtraValues, 0, StringValueCount * sizeof(char *));
	if (format == STRING_FILE) {
		TextFile = fopen(filename, "r"); // RA2: the text file is read a second time to fill the tables
		if (TextFile == 0) {
			return 0;
		}
		if (!Read_Text_File()) { // RA2: 0 when the file ends inside a label
			fclose(TextFile);
			return 0;
		}
		fclose(TextFile);
	} else {
		ReadFile(CSFName);
	}
	qsort(StringLabels, StringLabelCount, sizeof(CSFLabel), (int (__cdecl *)(void const *, void const *))_stricmp); // RA2: the labels are compared without regard to case, which is what LoadString searches by
	DebugString("MaxLabelLen = %d\n", MaxLabelLen); // RA2: the literal at 0x007F8E50
	DebugString("Language: %s\n", GetLanguageName(StringLanguage)); // RA2: the literal at 0x007F8E40; GetLanguageName is expanded here
	Session.Load_Call_Wait_Strings(); // RA2: the call-wait texts are fetched now that there are strings
	RandomMapGen.SeedData.Set_Default_Description();
	return 1;
}
#endif

/// <summary>
/// Takes the next character of an entry: from the rest of the line already read (source, which becomes NULL
/// when it runs out) and then from the file.
/// </summary>
static inline void Next_Character(FILE * file, char *& source, char & character)
{
	if (source != NULL) { // RA2: its code is expanded twice in Read_Text_Entry
		character = *source++;
		if (character == 0) { // RA2: the line is used up
			source = NULL;
			character = getc(file); // RA2: the library function getc, which calls fgetc
		}
	} else {
		character = getc(file);
	}
}

/// <summary>
/// Reads one entry of a text file: the quoted text up to the closing quote (white space becomes spaces) into text,
/// then the name after it (letters, digits and underscores) into label, which is the speech name. The first
/// characters come from source, the rest of the line already read; limit is the room for the text.
/// </summary>
void __fastcall Read_Text_Entry(FILE * file, char * source, char * text, char * label, int limit) // RA2: 0x006F92A0
{
	int escaped = 0; // RA2: 1 after a backslash
	int state = 0; // RA2: 0 before the name, 1 inside it, 2 after it
	int length = 0; // RA2: the length of the name
	char character;
	for (; limit != 0; limit--) { // RA2: the text of the entry
		Next_Character(file, source, character);
		if (character == -1) { // RA2: end of file ends the entry at once, without terminating the text
			return;
		}
		if (character == '\n') { // RA2: a line break is a space, and no backslash goes over it
			escaped = 0;
			character = ' ';
		} else if (character == '\\') { // RA2: a backslash escapes the next character
			escaped = escaped == 0; // RA2: two backslashes cancel out
		} else {
			if (character == '"' && !escaped) { // RA2: an unescaped quote ends the text
				break;
			}
			escaped = 0;
		}
		if (iswspace(character)) { // RA2: the library's iswspace
			character = ' ';
		}
		*text++ = character;
	}
	*text = 0;
	for (;;) { // RA2: the speech name after the quote
		Next_Character(file, source, character);
		if (character == '\n' || character == -1) { // RA2: the name ends with the line
			*label = 0;
			if (length != 0 && label[-1] >= '0' && label[-1] <= '9') { // RA2: a name that ends in a digit gets an e appended
				*label = 'e';
				label[1] = 0;
			}
			return;
		}
		switch (state) {
		case 0: // RA2: before the name
			if (iswspace(character) || character == '=') { // RA2: white space and = are skipped
					continue;
			}
			state = 1; // RA2: the name starts; falls into the next case
		case 1: // RA2: inside the name
				if ((character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z') || (character >= '0' && character <= '9') || character == '_') {
					*label++ = character;
					length++;
			} else {
				state = 2; // RA2: anything else ends the name; the rest of the line is skipped
			}
				break;
		}
	}
}

/// <summary>
/// Reads the labels and strings of a .csf file into the tables LoadFile made room for. 0 when the file is
/// short or damaged.
/// </summary>
int StringTable::ReadFile(char const * name) // RA2: 0x006F9400
{
	CCFileClass file(name);
	int string_index = 0; // RA2: the index of the next string in the tables
	if (!file.Is_Available() || !file.Open()) {
		return 0;
	}
	CSFHeader header; // RA2: the header was read once already by LoadFile
	if (file.Read(&header, sizeof(header)) != sizeof(header)) {
		return 0;
	}
	int id;
	int count;
	int length;
	int label = 0; // RA2: the index of the next label
	while (file.Read(&id, 4) == 4) { // RA2: a label: ' LBL', the number of strings, the length and the name
		if (id != 'LBL ') { // RA2: 0x4C424C20
			return 0;
		}
		file.Read(&count, 4);
		file.Read(&length, 4);
		if (length != 0) {
			file.Read(TextLine, length); // RA2: the name goes through the line buffer
		}
		TextLine[length] = 0;
		strcpy(StringLabels[label].Name, TextLine);
		if (length > MaxLabelLen) {
			MaxLabelLen = length;
		}
		StringLabels[label].FirstString = string_index;
		StringLabels[label].StringCount = count;
		for (int index = 0; index < count; index++) { // RA2: the strings of the label
			file.Read(&id, 4); // RA2: ' RTS' for a string, 'WRTS' for a string with a speech name
			if (id != 'STR ' && id != 'STRW') { // RA2: 0x53545220 and 0x53545257
				return 0;
			}
			file.Read(&length, 4); // RA2: the length in characters
			if (length != 0) {
				file.Read(CSFWideText, length * 2);
			}
			CSFWideText[length] = 0;
			for (wchar_t * string = CSFWideText; *string != 0; string++) { // RA2: the text is stored with every bit inverted
				*string = ~*string;
			}
			Collapse_White_Space(CSFWideText);
			StringValues[string_index + index] = (wchar_t *)::operator new((wcslen(CSFWideText) + 1) * sizeof(wchar_t));
			wcscpy(StringValues[string_index + index], CSFWideText);
			if (id == 'STRW') { // RA2: the speech name follows
				file.Read(&length, 4);
				if (length != 0) {
					file.Read(TextLine, length);
				}
				TextLine[length] = 0;
				if (length != 0) {
					StringExtraValues[string_index + index] = (char *)::operator new(length + 1);
					strcpy(StringExtraValues[string_index + index], TextLine);
				}
			}
		}
		label++;
		string_index += count;
	}
	return 1;
}

#ifdef RA2_NON_MATCHING // not exact: the jabber block of the expanded Translate_Copy keeps the run start in esi and the input in ecx where the image has ecx and edx (0x6F99E1 to 0x6F9AB3, same instructions but two copies, and the first character through al); 740 bytes differ by position against the image-derived bound 006f9790 (1,469 B; ours 1,469 B, the image's calls, every reference bound): 21 groups of instructions, all in that block but the switch table offset one byte later. Text from Generals GameText.cpp (parseStringFile), modified for RA2: several strings to a label, the tables allocated here
/// <summary>
/// Copies a string into a wide buffer, resolving its backslash escapes. With the jabber option every run of
/// letters is written backwards first.
/// </summary>
/// <remarks>From Generals GameText.cpp (GameTextManager::translateCopy), Copyright 2025 Electronic Arts Inc.,
/// GPL-3.0-or-later; modified: RA2's globals, no munkee branch, no format-word flag, and the escape cases
/// store through the pointer and step it once after the switch (RA2: the image's register ranks).</remarks>
static void Translate_Copy(wchar_t * outbuf, char * inbuf)
{
	int slash = FALSE;

	if (JabberOption) {
		char *firstLetter = NULL, *lastLetter;
		char * b = ReversedText;
		char ch;

		while ((ch = *inbuf++) != 0) {
			if (!((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))) {
				if (firstLetter) {
					lastLetter = b - 1;
					StringTable::Reverse_String(firstLetter, lastLetter);
					firstLetter = NULL;
				}
				*b++ = ch;
				if (ch == '\\') {
					*b++ = *inbuf++;
				}
				if (ch == '%') {
					while ((ch = *inbuf++) != 0 && !((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))) {
						*b++ = ch;
					}
					*b++ = ch;
				}
			} else {
				if (!firstLetter) {
					firstLetter = b;
				}

				*b++ = ch;
			}
		}

		if (firstLetter) {
			lastLetter = b - 1;
			StringTable::Reverse_String(firstLetter, lastLetter);
		}

		*b++ = 0;
		inbuf = ReversedText;
	}

	while (*inbuf != '\0') {
		if (slash == TRUE) {
			slash = FALSE;

			switch (*inbuf) {
				// in case end of string is reached
				// should never happen!!!
				case '\0':
					return;

				case '\\':
					*outbuf = '\\';
					break;

				case '\'':
					*outbuf = '\'';
					break;

				case '\"':
					*outbuf = '\"';
					break;

				case '\?':
					*outbuf = '\?';
					break;

				case 't':
					*outbuf = '\t';
					break;

				case 'n':
					*outbuf = '\n';
					break;

				default:
					*outbuf = *inbuf & 0x00FF;
					break;
			}
			outbuf++;
		} else if (*inbuf != '\\') {
			*outbuf++ = *inbuf & 0x00FF;
		} else {
			slash = TRUE;
		}

		inbuf++;
	}
	*outbuf = 0;
}

/// <summary>
/// Reads the labels and strings of the text file into the tables LoadFile made room for: a label line, then its
/// quoted strings, then END. Lines that are empty or start with // are skipped. 1 at the end of the file,
/// 0 when the file ends inside a label.
/// </summary>
/// <remarks>From Generals GameText.cpp (GameTextManager::parseStringFile), Copyright 2025 Electronic Arts Inc.,
/// GPL-3.0-or-later; modified: RA2 keeps several strings to a label and allocates each text here.</remarks>
int __fastcall Read_Text_File(void) // RA2: 0x006F9790
{
	int listCount = 0;
	int string_index = 0;
	int ok = TRUE;

	while (ok) {
		int len;
		if (!fgets(TextLine, 0x1000, TextFile)) {
			break;
		}

		Strip_Line(TextLine);

		if ((*(unsigned short *)TextLine == 0x2F2F) || !TextLine[0]) {
			continue;
		}

		for (int i = 0; i < listCount; i++) {
			_stricmp(StringLabels[i].Name, TextLine);
		}

		strcpy(StringLabels[listCount].Name, TextLine);
		len = strlen(TextLine);

		if (len > MaxLabelLen) {
			MaxLabelLen = len;
		}

		int count = 0;
		StringLabels[listCount].FirstString = string_index;
		while (ok) {
			if (!fgets(TextLine, 0xFFF, TextFile)) {
				ok = FALSE;
				goto quit;
			}

			Strip_Line(TextLine);

			if (TextLine[0] == '"') {
				len = strlen(TextLine);
				TextLine[len] = '\n';
				TextLine[len + 1] = 0;
				Read_Text_Entry(TextFile, &TextLine[1], TextBuffer, LabelBuffer, 0x1000);

				Translate_Copy(CSFWideText, TextBuffer);
				Collapse_White_Space(CSFWideText);
				len = wcslen(CSFWideText) + 1;
				StringValues[string_index + count] = new wchar_t[len];
				wcscpy(StringValues[string_index + count], CSFWideText);
				int speech_length = strlen(LabelBuffer) + 1; // RA2: the speech name read behind the quote
				if (speech_length > 1) { // RA2: only a name that is not empty is kept
					StringExtraValues[string_index + count] = (char *)::operator new(speech_length);
					strcpy(StringExtraValues[string_index + count], LabelBuffer);
				}
				count++;
			} else if (!_stricmp(TextLine, "END")) {
				break;
			}
		}
		StringLabels[listCount].StringCount = count;
		listCount++;
		string_index += count;
	}

quit:

	return(ok);
}
#endif

/// <summary>
/// Reverses the characters from start to end in place. The first pair also swaps its case when the first
/// is a capital and the last a small letter, so that a capitalized word stays capitalized.
/// </summary>
/// <remarks>From Generals GameText.cpp (GameTextManager::reverseWord), Copyright 2025 Electronic Arts Inc.,
/// GPL-3.0-or-later; modified: this tree's names.</remarks>
void StringTable::Reverse_String(char * file, char * lp) // RA2: 0x006F9D50
{
	int first = TRUE;
	char f;
	char l;
	int ok = TRUE;

	while (ok) {
		if (file >= lp) {
			return;
		}

		f = *file;
		l = *lp;

		if (first) {
			if (f >= 'A' && f <= 'Z') {
				if (l >= 'a' && l <= 'z') {
					f = (f - 'A') + 'a';
					l = (l - 'a') + 'A';
				}
			}

			first = FALSE;
		}

		*lp-- = f;
		*file++ = l;
	}
}

/// <summary>
/// Frees the tables and every text LoadString made up, and lets LoadFile read the table again.
/// </summary>
void StringTable::Unload(void) // RA2: 0x006F9D90
{
	while (StringChunkList) { // RA2: the second list, which nothing fills
		CSFString * chunk = StringChunkList;
		StringChunkList = chunk->PreviousEntry;
		delete chunk;
	}
	StringChunkList = 0;
	if (StringValues) { // RA2: the texts
		for (int index = 0; index < StringValueCount; index++) {
			if (StringValues[index]) {
				delete StringValues[index];
			}
		}
		delete StringValues;
		StringValues = 0;
	}
	if (StringExtraValues) { // RA2: the speech names
		for (int index = 0; index < StringValueCount; index++) {
			if (StringExtraValues[index]) {
				delete StringExtraValues[index];
			}
		}
		delete StringExtraValues;
		StringExtraValues = 0;
	}
	if (StringLabels) { // RA2: the labels
		delete StringLabels;
		StringLabels = 0;
	}
	CSFString * chunk = LastLoadedString; // RA2: the texts made up for missing labels
	while (chunk) {
		CSFString * next = chunk->PreviousEntry;
		delete chunk;
		chunk = next;
	}
	LastLoadedString = 0;
	StringsLoaded = 0; // RA2: the next LoadFile reads the table again
}

/// <summary>
/// True when every character of a wide string fits one byte.
/// </summary>
int __fastcall Is_Narrow_Text(wchar_t const * text) // RA2: 0x006F9E90
{
	while (*text) {
		if (*text++ > 0xFF) {
			return 0;
		}
	}
	return 1;
}

/// <summary>
/// The text of a label, and through extra its speech name (NULL when it has none). A label that is not in the
/// table gives a made-up text MISSING:'label' (and a note in the debug log naming the file and line of the caller);
/// without a table the text says so.
/// </summary>
wchar_t const * StringTable::LoadString(char const * label, char ** extra, char const * file, int line) // RA2: 0x006F9EC0
{
	if (extra) {
		*extra = 0;
	}
	if (!StringLabels) { // RA2: nothing was read
		return L"***FATAL*** String Manager failed to initilaized properly"; // RA2: the spelling is the executable's
	}
	CSFLabel * found = (CSFLabel *)bsearch(label, StringLabels, StringLabelCount, sizeof(CSFLabel), (int (__cdecl *)(void const *, void const *))_stricmp); // RA2: the table is sorted by the same comparison
	if (found) {
		if (extra) {
			*extra = StringExtraValues[found->FirstString];
		}
		return StringValues[found->FirstString]; // RA2: only the first string of a label is ever used
	}
	CSFString * chunk = new CSFString; // RA2: 0x208 bytes
	swprintf(chunk->Text, L"MISSING:'%hs'", label, file, line); // RA2: file and line are passed on although the format uses only the label
	DebugString("***NO_STRING*** '%s' -> file:%s line:%d\n", label, file, line); // RA2: the literal at 0x007F8E70
	chunk->PreviousEntry = LastLoadedString; // RA2: the chunk joins the list that Unload frees
	LastLoadedString = chunk;
	return chunk->Text;
}

/// <summary>
/// True when the language read is one whose strings are English (or has no code letter).
/// </summary>
int __fastcall Is_Language_English(void) // RA2: 0x006F9F80
{
	char const * letter = Languages[StringLanguage].Letter; // RA2: the table is indexed by the language directly
	if (letter == 0) {
		return 1;
	}
	return stricmp(letter, "e") == 0; // RA2: the literal at 0x007F8E34, shared with the table
}

/// <summary>
/// The -jabber switch: changes the option, and when it changed reads the table again.
/// </summary>
void __fastcall Set_Jabber_Option(int value) // RA2: 0x006F9FB0
{
	if (JabberOption != value) {
		StringTable::Unload();
		JabberOption = value;
		StringTable::LoadFile(StringFileName);
	}
}

/// <summary>
/// The -str and -nostr switches: changes whether a text file may be read, and when it changed reads the
/// table again.
/// </summary>
void __fastcall Set_Str_Option(int value) // RA2: 0x006F9FE0
{
	if (StrOption != value) {
		StringTable::Unload();
		StrOption = value;
		StringTable::LoadFile(StringFileName);
	}
}

/// <summary>
/// The value of the -jabber option. Nothing calls it.
/// </summary>
int __fastcall Get_Jabber_Option(void) // RA2: 0x006FA010
{
	return JabberOption;
}

/// <summary>
/// The value of the -str option. Nothing calls it.
/// </summary>
int __fastcall Get_Str_Option(void) // RA2: 0x006FA020
{
	return StrOption;
}

/// <summary>
/// True once LoadFile has been called.
/// </summary>
int StringTable::Is_Initialized(void) // RA2: 0x006FA030
{
	return StringsLoaded;
}

/// <summary>
/// A message box for wide text: the text and caption are narrowed (at most 5120 and 2048 bytes) for MessageBoxA.
/// </summary>
int __fastcall Message_Box_Wide(HWND window, wchar_t const * text, wchar_t const * caption, unsigned int type) // RA2: 0x006FA040
{
	char narrow_text[0x1400];
	char narrow_caption[0x800];
	WideCharToMultiByte(CP_ACP, 0, text, -1, narrow_text, sizeof(narrow_text), 0, 0); // RA2: the ANSI code page
	WideCharToMultiByte(CP_ACP, 0, caption, -1, narrow_caption, sizeof(narrow_caption), 0, 0);
	return MessageBoxA(window, narrow_text, narrow_caption, type);
}

/// <summary>
/// Widens a narrow string into dest, at most limit characters (-1 for no limit). The characters are not sign extended.
/// </summary>
void __fastcall Char_To_Wide(wchar_t * dest, char const * source, int limit) // RA2: 0x006FA0C0
{
	while (*source != 0 && limit != 0) {
		*dest++ = (unsigned char)*source++;
		if (limit != -1) { // RA2: -1 counts down no further
			limit--;
		}
	}
	*dest = 0;
}

namespace OwnerDraw {
void __fastcall WideToCharString(char * dest, wchar_t const * source, int limit); // RA2: 0x006FA0F0, defined below
}

/// <summary>
/// Narrows a wide string into dest, at most limit characters (-1 for no limit). The symbol says it returns
/// the text, but the executable's code leaves no result.
/// </summary>
void __fastcall OwnerDraw::WideToCharString(char * dest, wchar_t const * source, int limit) // RA2: 0x006FA0F0
{
	while (*source != 0 && limit != 0) {
		*dest++ = (char)*source++; // RA2: the high byte is cut off
		if (limit != -1) { // RA2: -1 counts down no further
			limit--;
		}
	}
	*dest = 0;
}

/// <summary>
/// A wide string narrowed into one of eight static buffers of 1024 bytes, used in turn; NULL for NULL.
/// </summary>
char * __fastcall Wide_To_Narrow(wchar_t const * text) // RA2: 0x006FA120
{
	if (!text) {
		return 0;
	}
	char * buffer = NarrowBuffers[NarrowIndex];
	OwnerDraw::WideToCharString(buffer, text, 0x400);
	buffer[0x3FF] = 0;
	NarrowIndex = (NarrowIndex + 1) & 7; // RA2: the next call uses the next buffer
	return buffer;
}

/// <summary>
/// A narrow string widened into one of eight static buffers of 1024 characters, used in turn; NULL for NULL.
/// </summary>
wchar_t * __fastcall Narrow_To_Wide(char const * narrow) // RA2: 0x006FA180
{
	if (!narrow) {
		return 0;
	}
	wchar_t * buffer = WideBuffers[WideIndex];
	Char_To_Wide(buffer, narrow, 0x400);
	buffer[0x3FF] = 0;
	WideIndex = (WideIndex + 1) & 7; // RA2: the next call uses the next buffer
	return buffer;
}
