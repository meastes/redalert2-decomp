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
#include <string.h> // RA2: as above
#include <stdlib.h> // RA2: as above
#include <stdio.h> // RA2: as above
#include <ctype.h> // RA2: as above
#include <wchar.h> // RA2: as above
#include <windows.h> // RA2: as above
#undef LoadString // RA2: windows.h maps LoadString to LoadStringA
#undef getc // RA2: the executable calls the library function getc (0x00784A3C), not the macro of stdio.h
#include "rawfile.h" // RA2: as above
#include "ccfile.h" // RA2: as above
#include "session.h" // RA2: as above
#include "mapgen.h" // RA2: as above
#include "dbgprint.h" // RA2: as above
#include "stringtable.h" // RA2: as above


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
	{CSF_US, "US", "us", "e"}, // RA2: as above
	{CSF_UK, "UK", "uk", "e"}, // RA2: the strings of the UK language are the English ones
	{CSF_GERMAN, "German", "ge", "g"}, // RA2: as above
	{CSF_FRENCH, "French", "fr", "f"}, // RA2: as above
	{CSF_SPANISH, "Spanish", "sp", "s"}, // RA2: as above
	{CSF_ITALIAN, "Italian", "it", "i"}, // RA2: as above
	{CSF_JAPANESE, "Japanese", "ja", "j"}, // RA2: as above
	{CSF_JABBERWOCKIE, "Jabberwockie", "jb", "e"}, // RA2: the test language reads like English
	{CSF_KOREAN, "Korean", "ko", "k"}, // RA2: as above
	{CSF_CHINESE, "Chinese", "ch", "c"}, // RA2: as above
	{CSF_UNKNOWN, "Unknown", NULL, NULL} // RA2: as above
};

char NarrowBuffers[8][1024]; // RA2: 0x00AC136C; the eight results Wide_To_Narrow hands out in turn
char TextBuffer[4104]; // RA2: 0x00AC3378; the text of one entry of a text file (the executable gives only the distance to the next object)
char LabelBuffer[4104]; // RA2: 0x00AC4380; the speech name of one entry of a text file
char CSFName[116]; // RA2: 0x00AC5388; the file name with its extension replaced by .csf
wchar_t WideBuffers[8][1024]; // RA2: 0x00AC53FC; the eight results Narrow_To_Wide hands out in turn
wchar_t CSFWideText[8192]; // RA2: 0x00AC93FC; the text of one string while it is made ready
char ReversedText[8192]; // RA2: 0x00ACD410; the text of one entry with its runs of letters reversed
char TextLine[4096]; // RA2: 0x00ACF410; one line of a text file, and the label of a .csf file
FILE * TextFile; // RA2: 0x00AD0430; the text file being read
int NarrowIndex; // RA2: 0x00AD045C; which of the eight buffers Wide_To_Narrow uses next
int WideIndex; // RA2: 0x00AD0460; which of the eight buffers Narrow_To_Wide uses next

/// <summary>
/// The table entry of a language, or NULL when the language is not in the table.
/// </summary>
CSFLanguage const * StringTable::GetLanguage(CSFLanguages language) // RA2: 0x006F8DD0
{
	for (CSFLanguage const * entry = Languages; entry->Index != CSF_UNKNOWN; entry++) { // RA2: the table ends with the entry of CSF_UNKNOWN
		if (entry->Index == language) { // RA2: as above
			return entry; // RA2: as above
		}
	}
	return NULL; // RA2: as above
}

/// <summary>
/// The name of a language, or "unknown" when the language is not in the table.
/// </summary>
char const * StringTable::GetLanguageName(CSFLanguages language) // RA2: 0x006F8E00
{
	CSFLanguage const * entry = GetLanguage(language); // RA2: its expansion is the loop at 0x006F8E00
	if (entry != NULL) { // RA2: as above
		return entry->Name; // RA2: the name is the second field of the entry
	}
	return "unknown"; // RA2: the literal at 0x007CCCE0
}

/// <summary>
/// Drops the spaces that start a line, repeat, or end a line from a wide string, in place.
/// </summary>
static inline void Collapse_White_Space(wchar_t * string) // RA2: as above
{
	wchar_t * in = string; // RA2: the code of this helper is expanded in Read_Text_File (0x006F9790) and ReadFile (0x006F9400)
	wchar_t previous = 0; // RA2: as above
	int line_start = 1; // RA2: as above
	wchar_t * out = string; // RA2: as above
	for (wchar_t c = *in++; c != 0; c = *in++) { // RA2: as above
		if (c == ' ') { // RA2: as above
			if (previous == c || line_start) { // RA2: a space after a space or at the start of a line is dropped
				continue; // RA2: as above
			}
		} else if (c == '\n' || c == '\t') { // RA2: a line break or tab ends the line
			if (previous == ' ') { // RA2: the space before it is dropped
				out--; // RA2: as above
			}
			*out = c; // RA2: as above
			line_start = 1; // RA2: as above
			previous = c; // RA2: as above
			out++; // RA2: as above
			continue; // RA2: as above
		}
		*out++ = c; // RA2: as above
		previous = c; // RA2: as above
		line_start = 0; // RA2: as above
	}
	if (previous == ' ') { // RA2: a space at the very end is dropped as well
		out--; // RA2: as above
	}
	*out = 0; // RA2: as above
}

/// <summary>
/// Reads the header of a .csf file: how many labels and strings it holds, and its language. False when
/// the file is missing or is no .csf file.
/// </summary>
static int Read_CSF_Header(char const * name) // RA2: as above
{
	CCFileClass file(name); // RA2: its expansion is at 0x006F8EE1 in LoadFile; the failing paths call the destructor
	int ok = 0; // RA2: as above
	if (file.Is_Available() && file.Open()) { // RA2: as above
		CSFHeader header; // RA2: as above
		if (file.Read(&header, sizeof(header)) == sizeof(header) && header.ID == 'CSF ') { // RA2: 24 bytes that start with 0x43534620
			StringLabelCount = header.LabelCount; // RA2: as above
			StringValueCount = header.StringCount; // RA2: as above
			StringLanguage = (CSFLanguages)(header.Version < 2 ? 0 : header.Language); // RA2: versions below 2 have no language field and are US
			ok = 1; // RA2: as above
		}
		return ok; // RA2: as above
	}
	return 0; // RA2: as above
}

/// <summary>
/// Drops the white space from both ends of a line, in place.
/// </summary>
static void Strip_Line(char * line) // RA2: as above
{
	char * in = line; // RA2: its code is expanded in LoadFile and Read_Text_File
	char c = *in; // RA2: as above
	while (c != 0 && iswspace(c)) { // RA2: leading white space is skipped (iswspace of the signed character)
		c = *++in; // RA2: as above
	}
	char * out = line; // RA2: as above
	while ((*out++ = *in++) != 0) { // RA2: the rest is moved to the start
	}
	out -= 2; // RA2: as above
	while (out > line && *out != 0 && iswspace(*out)) { // RA2: trailing white space is cut off
		out--; // RA2: as above
	}
	out[1] = 0; // RA2: as above
}

void __fastcall Read_Text_Entry(FILE * file, char * source, char * text, char * label, int limit); // RA2: 0x006F92A0, defined below
int __fastcall Read_Text_File(void); // RA2: 0x006F9790, defined below

/// <summary>
/// Reads the string table: from the .csf file named like the given file when there is one, else (when
/// -str allows it) from the text file itself. The tables are sorted by label at the end. 1 when the table
/// holds strings.
/// </summary>
#ifdef RA2_NON_MATCHING // not exact: 783 bytes differ (adopted text)
int StringTable::LoadFile(char const * filename) // RA2: 0x006F8E30
{
	if (StringsLoaded != 0) { // RA2: the table is read only once until Unload
		return 1; // RA2: as above
	}
	StringsLoaded = 1; // RA2: set even when the reading fails below
	MaxLabelLen = 0; // RA2: as above
	StringFileName = filename; // RA2: as above
	if (filename == 0 || *filename == 0) { // RA2: as above
		return 0; // RA2: as above
	}
	strcpy(CSFName, filename); // RA2: as above
	char * dot = strchr(CSFName, '.'); // RA2: the extension is replaced by .csf
	if (dot != 0) { // RA2: as above
		*dot = 0; // RA2: as above
	}
	strcat(CSFName, ".csf"); // RA2: the literal at 0x007F8E68
	int csf; // RA2: 1 when the strings come from the .csf file
	if (Read_CSF_Header(CSFName)) { // RA2: as above
		csf = 1; // RA2: as above
	} else {
		if (StrOption == 0) { // RA2: no .csf file, and -nostr forbids the text file
			return 0; // RA2: as above
		}
		TextFile = fopen(filename, "r"); // RA2: the text file is counted first: every END is a label, every quoted line a string
		if (TextFile == 0) { // RA2: as above
			return 0; // RA2: as above
		}
		StringLabelCount = 0; // RA2: as above
		StringValueCount = 0; // RA2: as above
		while (true) { // RA2: as above
			if (fgets(TextLine, 0xFFF, TextFile) == 0) { // RA2: as above
				break; // RA2: as above
			}
			Strip_Line(TextLine); // RA2: as above
			if (TextLine[0] == '"') { // RA2: a quoted line starts a string, which may continue over several lines
				int length = strlen(TextLine); // RA2: as above
				TextLine[length] = '\n'; // RA2: the line end the strip took off is put back so that the string goes on
				TextLine[length + 1] = 0; // RA2: as above
				Read_Text_Entry(TextFile, &TextLine[1], TextBuffer, LabelBuffer, 0x1000); // RA2: reads the rest of the string, only to skip it here
				StringValueCount++; // RA2: as above
				continue; // RA2: as above
			}
			if (_stricmp(TextLine, "END") == 0) { // RA2: the literal at 0x007F8E64; END closes a label
				StringLabelCount++; // RA2: as above
			}
		}
		StringLabelCount += 500; // RA2: room for 500 more labels
		StringValueCount += 1000; // RA2: and 1000 more strings
		fclose(TextFile); // RA2: as above
		csf = 0; // RA2: as above
	}
	if (StringValueCount == 0 || StringLabelCount == 0) { // RA2: as above
		return 0; // RA2: as above
	}
	StringLabels = (CSFLabel *)::operator new(StringLabelCount * sizeof(CSFLabel)); // RA2: the three tables are made one after the other
	if (StringLabels == 0) { // RA2: as above
		return 0; // RA2: as above
	}
	memset(StringLabels, 0, StringLabelCount * sizeof(CSFLabel)); // RA2: as above
	StringValues = (wchar_t **)::operator new(StringValueCount * sizeof(wchar_t *)); // RA2: as above
	if (StringValues == 0) { // RA2: as above
		return 0; // RA2: as above
	}
	memset(StringValues, 0, StringValueCount * sizeof(wchar_t *)); // RA2: as above
	StringExtraValues = (char **)::operator new(StringValueCount * sizeof(char *)); // RA2: as above
	if (StringExtraValues == 0) { // RA2: as above
		return 0; // RA2: as above
	}
	memset(StringExtraValues, 0, StringValueCount * sizeof(char *)); // RA2: as above
	if (csf == 0) { // RA2: as above
		TextFile = fopen(filename, "r"); // RA2: the text file is read a second time to fill the tables
		if (TextFile == 0) { // RA2: as above
			return 0; // RA2: as above
		}
		if (!Read_Text_File()) { // RA2: 0 when the file ends inside a label
			fclose(TextFile); // RA2: as above
			return 0; // RA2: as above
		}
		fclose(TextFile); // RA2: as above
	} else {
		ReadFile(CSFName); // RA2: as above
	}
	qsort(StringLabels, StringLabelCount, sizeof(CSFLabel), (int (__cdecl *)(void const *, void const *))_stricmp); // RA2: the labels are compared without regard to case, which is what LoadString searches by
	DebugString("MaxLabelLen = %d\n", MaxLabelLen); // RA2: the literal at 0x007F8E50
	DebugString("Language: %s\n", GetLanguageName(StringLanguage)); // RA2: the literal at 0x007F8E40; GetLanguageName is expanded here
	Session.Load_Call_Wait_Strings(); // RA2: the call-wait texts are fetched now that there are strings
	RandomMapGen.SeedData.Set_Default_Description(); // RA2: as above
	return 1; // RA2: as above
}
#endif

/// <summary>
/// Takes the next character of an entry: from the rest of the line already read (source, which becomes NULL
/// when it runs out) and then from the file.
/// </summary>
static inline void Next_Character(FILE * file, char *& source, char & character) // RA2: as above
{
	if (source != NULL) { // RA2: its code is expanded twice in Read_Text_Entry
		character = *source++; // RA2: as above
		if (character == 0) { // RA2: the line is used up
			source = NULL; // RA2: as above
			character = getc(file); // RA2: the library function getc, which calls fgetc
		}
	} else {
		character = getc(file); // RA2: as above
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
	char character; // RA2: as above
	for (; limit != 0; limit--) { // RA2: the text of the entry
		Next_Character(file, source, character); // RA2: as above
		if (character == -1) { // RA2: end of file ends the entry at once, without terminating the text
			return; // RA2: as above
		}
		if (character == '\n') { // RA2: a line break is a space, and no backslash goes over it
			escaped = 0; // RA2: as above
			character = ' '; // RA2: as above
		} else if (character == '\\') { // RA2: a backslash escapes the next character
			escaped = escaped == 0; // RA2: two backslashes cancel out
		} else {
			if (character == '"' && !escaped) { // RA2: an unescaped quote ends the text
				break; // RA2: as above
			}
			escaped = 0; // RA2: as above
		}
		if (iswspace(character)) { // RA2: the library's iswspace
			character = ' '; // RA2: as above
		}
		*text++ = character; // RA2: as above
	}
	*text = 0; // RA2: as above
	for (;;) { // RA2: the speech name after the quote
		Next_Character(file, source, character); // RA2: as above
		if (character == '\n' || character == -1) { // RA2: the name ends with the line
			*label = 0; // RA2: as above
			if (length != 0 && label[-1] >= '0' && label[-1] <= '9') { // RA2: a name that ends in a digit gets an e appended
				*label = 'e'; // RA2: as above
				label[1] = 0; // RA2: as above
			}
			return; // RA2: as above
		}
		switch (state) { // RA2: as above
		case 0: // RA2: before the name
			if (iswspace(character) || character == '=') { // RA2: white space and = are skipped
				continue; // RA2: as above
			}
			state = 1; // RA2: the name starts; falls into the next case
		case 1: // RA2: inside the name
			if ((character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z') || (character >= '0' && character <= '9') || character == '_') { // RA2: as above
				*label++ = character; // RA2: as above
				length++; // RA2: as above
			} else {
				state = 2; // RA2: anything else ends the name; the rest of the line is skipped
			}
			break; // RA2: as above
		}
	}
}

/// <summary>
/// Reads the labels and strings of a .csf file into the tables LoadFile made room for. 0 when the file is
/// short or damaged.
/// </summary>
int StringTable::ReadFile(char const * name) // RA2: 0x006F9400
{
	CCFileClass file(name); // RA2: as above
	int string_index = 0; // RA2: the index of the next string in the tables
	if (!file.Is_Available() || !file.Open()) { // RA2: as above
		return 0; // RA2: as above
	}
	CSFHeader header; // RA2: the header was read once already by LoadFile
	if (file.Read(&header, sizeof(header)) != sizeof(header)) { // RA2: as above
		return 0; // RA2: as above
	}
	int id; // RA2: as above
	int count; // RA2: as above
	int length; // RA2: as above
	int label = 0; // RA2: the index of the next label
	while (file.Read(&id, 4) == 4) { // RA2: a label: ' LBL', the number of strings, the length and the name
		if (id != 'LBL ') { // RA2: 0x4C424C20
			return 0; // RA2: as above
		}
		file.Read(&count, 4); // RA2: as above
		file.Read(&length, 4); // RA2: as above
		if (length != 0) { // RA2: as above
			file.Read(TextLine, length); // RA2: the name goes through the line buffer
		}
		TextLine[length] = 0; // RA2: as above
		strcpy(StringLabels[label].Name, TextLine); // RA2: as above
		if (length > MaxLabelLen) { // RA2: as above
			MaxLabelLen = length; // RA2: as above
		}
		StringLabels[label].FirstString = string_index; // RA2: as above
		StringLabels[label].StringCount = count; // RA2: as above
		for (int index = 0; index < count; index++) { // RA2: the strings of the label
			file.Read(&id, 4); // RA2: ' RTS' for a string, 'WRTS' for a string with a speech name
			if (id != 'STR ' && id != 'STRW') { // RA2: 0x53545220 and 0x53545257
				return 0; // RA2: as above
			}
			file.Read(&length, 4); // RA2: the length in characters
			if (length != 0) { // RA2: as above
				file.Read(CSFWideText, length * 2); // RA2: as above
			}
			CSFWideText[length] = 0; // RA2: as above
			for (wchar_t * string = CSFWideText; *string != 0; string++) { // RA2: the text is stored with every bit inverted
				*string = ~*string; // RA2: as above
			}
			Collapse_White_Space(CSFWideText); // RA2: as above
			StringValues[string_index + index] = (wchar_t *)::operator new((wcslen(CSFWideText) + 1) * sizeof(wchar_t)); // RA2: as above
			wcscpy(StringValues[string_index + index], CSFWideText); // RA2: as above
			if (id == 'STRW') { // RA2: the speech name follows
				file.Read(&length, 4); // RA2: as above
				if (length != 0) { // RA2: as above
					file.Read(TextLine, length); // RA2: as above
				}
				TextLine[length] = 0; // RA2: as above
				if (length != 0) { // RA2: as above
					StringExtraValues[string_index + index] = (char *)::operator new(length + 1); // RA2: as above
					strcpy(StringExtraValues[string_index + index], TextLine); // RA2: as above
				}
			}
		}
		label++; // RA2: as above
		string_index += count; // RA2: as above
	}
	return 1; // RA2: as above
}

/// <summary>
/// Reads the labels and strings of the text file into the tables LoadFile made room for: a label line, then its
/// quoted strings, then END. Lines that are empty or start with // are skipped. 1 at the end of the file,
/// 0 when the file ends inside a label.
/// </summary>
int __fastcall Read_Text_File(void) // RA2: 0x006F9790
{
	int label = 0; // RA2: the index of the next label
	int string_index = 0; // RA2: the index of the next string in the tables
	while (true) { // RA2: as above
		do { // RA2: as above
			if (fgets(TextLine, 0x1000, TextFile) == 0) { // RA2: the end of the file between two labels is the normal end
				return 1; // RA2: as above
			}
			Strip_Line(TextLine); // RA2: as above
		} while (*(unsigned short *)TextLine == '//' || TextLine[0] == 0); // RA2: both bytes are compared at once (0x2F2F)
		for (int index = 0; index < label; index++) { // RA2: the executable compares the new label with every earlier one and ignores the answer
			_stricmp(StringLabels[index].Name, TextLine); // RA2: as above
		}
		strcpy(StringLabels[label].Name, TextLine); // RA2: as above
		int length = strlen(TextLine); // RA2: as above
		if (length > MaxLabelLen) { // RA2: as above
			MaxLabelLen = length; // RA2: as above
		}
		int count = 0; // RA2: how many strings the label has
		StringLabels[label].FirstString = string_index; // RA2: as above
		while (true) { // RA2: the lines of the label
			if (fgets(TextLine, 0xFFF, TextFile) == 0) { // RA2: the file ends before END
				return 0; // RA2: as above
			}
			Strip_Line(TextLine); // RA2: as above
			if (TextLine[0] == '"') { // RA2: a string
				int line_length = strlen(TextLine); // RA2: as above
				TextLine[line_length] = '\n'; // RA2: the line end is put back, as LoadFile did when counting
				TextLine[line_length + 1] = 0; // RA2: as above
				Read_Text_Entry(TextFile, &TextLine[1], TextBuffer, LabelBuffer, 0x1000); // RA2: as above
				unsigned char * text = (unsigned char *)TextBuffer; // RA2: the text that is made wide below
				if (JabberOption != 0) { // RA2: with -jabber every run of letters is written backwards (a test of right to left text)
					char * run = NULL; // RA2: where the current run of letters starts in the result
					char * reversed = ReversedText; // RA2: as above
					char * next = TextBuffer; // RA2: as above
					char c = *next++; // RA2: as above
					while (c != 0) { // RA2: as above
						if ((c < 'a' || c > 'z') && (c < 'A' || c > 'Z')) { // RA2: not a letter
							if (run != NULL) { // RA2: a run of letters ends here
								StringTable::Reverse_String(run, reversed - 1); // RA2: expanded here; the call after the loop stays a call
								run = NULL; // RA2: as above
							}
							*reversed++ = c; // RA2: as above
							if (c == '\\') { // RA2: the character after a backslash is copied as it is
								*reversed++ = *next++; // RA2: as above
							} else if (c == '%') { // RA2: a format specification is copied up to its letter
								c = *next++; // RA2: as above
								while (c != 0 && (c < 'a' || c > 'z') && (c < 'A' || c > 'Z')) { // RA2: as above
									*reversed++ = c; // RA2: as above
									c = *next++; // RA2: as above
								}
								*reversed++ = c; // RA2: the letter that ends the specification
							}
						} else {
							if (run == NULL) { // RA2: a run of letters starts here
								run = reversed; // RA2: as above
							}
							*reversed++ = c; // RA2: as above
						}
						c = *next++; // RA2: as above
					}
					if (run != NULL) { // RA2: the last run
						StringTable::Reverse_String(run, reversed - 1); // RA2: a call in the executable (0x006F9D50)
					}
					*reversed = 0; // RA2: as above
					text = (unsigned char *)ReversedText; // RA2: as above
				}
				wchar_t * wide = CSFWideText; // RA2: the text widened, with its escapes resolved
				bool escaped = false; // RA2: after a backslash
				unsigned char character = *text; // RA2: as above
				while (character != 0) { // RA2: as above
					if (escaped) { // RA2: as above
						escaped = false; // RA2: as above
						switch (character) { // RA2: a jump table in the executable
						default: // RA2: any other character stands for itself
							*wide++ = character; // RA2: as above
							break; // RA2: as above
						case '"': // RA2: as above
							*wide++ = '"'; // RA2: as above
							break; // RA2: as above
						case '\'': // RA2: as above
							*wide++ = '\''; // RA2: as above
							break; // RA2: as above
						case '?': // RA2: as above
							*wide++ = '?'; // RA2: as above
							break; // RA2: as above
						case '\\': // RA2: as above
							*wide++ = '\\'; // RA2: as above
							break; // RA2: as above
						case 'n': // RA2: as above
							*wide++ = '\n'; // RA2: as above
							break; // RA2: as above
						case 't': // RA2: as above
							*wide++ = '\t'; // RA2: as above
							break; // RA2: as above
						}
					} else if (character != '\\') { // RA2: as above
						*wide++ = character; // RA2: the character is not sign extended
					} else {
						escaped = true; // RA2: as above
					}
					character = *++text; // RA2: as above
				}
				*wide = 0; // RA2: as above
				Collapse_White_Space(CSFWideText); // RA2: as above
				StringValues[string_index + count] = (wchar_t *)::operator new((wcslen(CSFWideText) + 1) * sizeof(wchar_t)); // RA2: as above
				wcscpy(StringValues[string_index + count], CSFWideText); // RA2: as above
				int speech_length = strlen(LabelBuffer) + 1; // RA2: the speech name read behind the quote
				if (speech_length > 1) { // RA2: only a name that is not empty is kept
					StringExtraValues[string_index + count] = (char *)::operator new(speech_length); // RA2: as above
					strcpy(StringExtraValues[string_index + count], LabelBuffer); // RA2: as above
				}
				count++; // RA2: as above
				continue; // RA2: as above
			}
			if (_stricmp(TextLine, "END") == 0) { // RA2: END closes the label
				break; // RA2: as above
			}
		}
		StringLabels[label].StringCount = count; // RA2: as above
		label++; // RA2: as above
		string_index += count; // RA2: as above
	}
}

/// <summary>
/// Reverses the characters from start to end in place. The first pair also swaps its case when the first
/// is a capital and the last a small letter, so that a capitalized word stays capitalized.
/// </summary>
void StringTable::Reverse_String(char * start, char * end) // RA2: 0x006F9D50
{
	int first = 1; // RA2: as above
	while (true) { // RA2: as above
		if (start >= end) { // RA2: as above
			return; // RA2: as above
		}
		char left = *start; // RA2: as above
		char right = *end; // RA2: as above
		if (first) { // RA2: only the outermost pair
			if (left >= 'A' && left <= 'Z' && right >= 'a' && right <= 'z') { // RA2: as above
				left += 'a' - 'A'; // RA2: as above
				right -= 'a' - 'A'; // RA2: as above
			}
			first = 0; // RA2: as above
		}
		*end-- = left; // RA2: as above
		*start = right; // RA2: as above
		start++; // RA2: as above
	}
}

/// <summary>
/// Frees the tables and every text LoadString made up, and lets LoadFile read the table again.
/// </summary>
void StringTable::Unload(void) // RA2: 0x006F9D90
{
	while (StringChunkList) { // RA2: the second list, which nothing fills
		CSFString * chunk = StringChunkList; // RA2: as above
		StringChunkList = chunk->PreviousEntry; // RA2: as above
		delete chunk; // RA2: as above
	}
	StringChunkList = 0; // RA2: as above
	if (StringValues) { // RA2: the texts
		for (int index = 0; index < StringValueCount; index++) { // RA2: as above
			if (StringValues[index]) { // RA2: as above
				delete StringValues[index]; // RA2: as above
			}
		}
		delete StringValues; // RA2: as above
		StringValues = 0; // RA2: as above
	}
	if (StringExtraValues) { // RA2: the speech names
		for (int index = 0; index < StringValueCount; index++) { // RA2: as above
			if (StringExtraValues[index]) { // RA2: as above
				delete StringExtraValues[index]; // RA2: as above
			}
		}
		delete StringExtraValues; // RA2: as above
		StringExtraValues = 0; // RA2: as above
	}
	if (StringLabels) { // RA2: the labels
		delete StringLabels; // RA2: as above
		StringLabels = 0; // RA2: as above
	}
	CSFString * chunk = LastLoadedString; // RA2: the texts made up for missing labels
	while (chunk) { // RA2: as above
		CSFString * next = chunk->PreviousEntry; // RA2: as above
		delete chunk; // RA2: as above
		chunk = next; // RA2: as above
	}
	LastLoadedString = 0; // RA2: as above
	StringsLoaded = 0; // RA2: the next LoadFile reads the table again
}

/// <summary>
/// True when every character of a wide string fits one byte.
/// </summary>
int __fastcall Is_Narrow_Text(wchar_t const * text) // RA2: 0x006F9E90
{
	while (*text) { // RA2: as above
		if (*text++ > 0xFF) { // RA2: as above
			return 0; // RA2: as above
		}
	}
	return 1; // RA2: as above
}

/// <summary>
/// The text of a label, and through extra its speech name (NULL when it has none). A label that is not in the
/// table gives a made-up text MISSING:'label' (and a note in the debug log naming the file and line of the caller);
/// without a table the text says so.
/// </summary>
wchar_t const * StringTable::LoadString(char const * label, char ** extra, char const * file, int line) // RA2: 0x006F9EC0
{
	if (extra) { // RA2: as above
		*extra = 0; // RA2: as above
	}
	if (!StringLabels) { // RA2: nothing was read
		return L"***FATAL*** String Manager failed to initilaized properly"; // RA2: the spelling is the executable's
	}
	CSFLabel * found = (CSFLabel *)bsearch(label, StringLabels, StringLabelCount, sizeof(CSFLabel), (int (__cdecl *)(void const *, void const *))_stricmp); // RA2: the table is sorted by the same comparison
	if (found) { // RA2: as above
		if (extra) { // RA2: as above
			*extra = StringExtraValues[found->FirstString]; // RA2: as above
		}
		return StringValues[found->FirstString]; // RA2: only the first string of a label is ever used
	}
	CSFString * chunk = new CSFString; // RA2: 0x208 bytes
	swprintf(chunk->Text, L"MISSING:'%hs'", label, file, line); // RA2: file and line are passed on although the format uses only the label
	DebugString("***NO_STRING*** '%s' -> file:%s line:%d\n", label, file, line); // RA2: the literal at 0x007F8E70
	chunk->PreviousEntry = LastLoadedString; // RA2: the chunk joins the list that Unload frees
	LastLoadedString = chunk; // RA2: as above
	return chunk->Text; // RA2: as above
}

/// <summary>
/// True when the language read is one whose strings are English (or has no code letter).
/// </summary>
int __fastcall Is_Language_English(void) // RA2: 0x006F9F80
{
	char const * letter = Languages[StringLanguage].Letter; // RA2: the table is indexed by the language directly
	if (letter == 0) { // RA2: as above
		return 1; // RA2: as above
	}
	return stricmp(letter, "e") == 0; // RA2: the literal at 0x007F8E34, shared with the table
}

/// <summary>
/// The -jabber switch: changes the option, and when it changed reads the table again.
/// </summary>
void __fastcall Set_Jabber_Option(int value) // RA2: 0x006F9FB0
{
	if (JabberOption != value) { // RA2: as above
		StringTable::Unload(); // RA2: as above
		JabberOption = value; // RA2: as above
		StringTable::LoadFile(StringFileName); // RA2: as above
	}
}

/// <summary>
/// The -str and -nostr switches: changes whether a text file may be read, and when it changed reads the
/// table again.
/// </summary>
void __fastcall Set_Str_Option(int value) // RA2: 0x006F9FE0
{
	if (StrOption != value) { // RA2: as above
		StringTable::Unload(); // RA2: as above
		StrOption = value; // RA2: as above
		StringTable::LoadFile(StringFileName); // RA2: as above
	}
}

/// <summary>
/// The value of the -jabber option. Nothing calls it.
/// </summary>
int __fastcall Get_Jabber_Option(void) // RA2: 0x006FA010
{
	return JabberOption; // RA2: as above
}

/// <summary>
/// The value of the -str option. Nothing calls it.
/// </summary>
int __fastcall Get_Str_Option(void) // RA2: 0x006FA020
{
	return StrOption; // RA2: as above
}

/// <summary>
/// True once LoadFile has been called.
/// </summary>
int StringTable::Is_Initialized(void) // RA2: 0x006FA030
{
	return StringsLoaded; // RA2: as above
}

/// <summary>
/// A message box for wide text: the text and caption are narrowed (at most 5120 and 2048 bytes) for MessageBoxA.
/// </summary>
int __fastcall Message_Box_Wide(HWND window, wchar_t const * text, wchar_t const * caption, unsigned int type) // RA2: 0x006FA040
{
	char narrow_text[0x1400]; // RA2: as above
	char narrow_caption[0x800]; // RA2: as above
	WideCharToMultiByte(0, 0, text, -1, narrow_text, sizeof(narrow_text), 0, 0); // RA2: the ANSI code page
	WideCharToMultiByte(0, 0, caption, -1, narrow_caption, sizeof(narrow_caption), 0, 0); // RA2: as above
	return MessageBoxA(window, narrow_text, narrow_caption, type); // RA2: as above
}

/// <summary>
/// Widens a narrow string into dest, at most limit characters (-1 for no limit). The characters are not sign extended.
/// </summary>
void __fastcall Char_To_Wide(wchar_t * dest, char const * source, int limit) // RA2: 0x006FA0C0
{
	while (*source != 0 && limit != 0) { // RA2: as above
		*dest++ = (unsigned char)*source++; // RA2: as above
		if (limit != -1) { // RA2: -1 counts down no further
			limit--; // RA2: as above
		}
	}
	*dest = 0; // RA2: as above
}

namespace OwnerDraw { // RA2: as above
void __fastcall WideToCharString(char * dest, wchar_t const * source, int limit); // RA2: 0x006FA0F0, defined below
}

/// <summary>
/// Narrows a wide string into dest, at most limit characters (-1 for no limit). The symbol says it returns
/// the text, but the executable's code leaves no result.
/// </summary>
void __fastcall OwnerDraw::WideToCharString(char * dest, wchar_t const * source, int limit) // RA2: 0x006FA0F0
{
	while (*source != 0 && limit != 0) { // RA2: as above
		*dest++ = (char)*source++; // RA2: the high byte is cut off
		if (limit != -1) { // RA2: -1 counts down no further
			limit--; // RA2: as above
		}
	}
	*dest = 0; // RA2: as above
}

/// <summary>
/// A wide string narrowed into one of eight static buffers of 1024 bytes, used in turn; NULL for NULL.
/// </summary>
char * __fastcall Wide_To_Narrow(wchar_t const * text) // RA2: 0x006FA120
{
	if (!text) { // RA2: as above
		return 0; // RA2: as above
	}
	char * buffer = NarrowBuffers[NarrowIndex]; // RA2: as above
	OwnerDraw::WideToCharString(buffer, text, 0x400); // RA2: as above
	buffer[0x3FF] = 0; // RA2: as above
	NarrowIndex = (NarrowIndex + 1) & 7; // RA2: the next call uses the next buffer
	return buffer; // RA2: as above
}

/// <summary>
/// A narrow string widened into one of eight static buffers of 1024 characters, used in turn; NULL for NULL.
/// </summary>
wchar_t * __fastcall Narrow_To_Wide(char const * narrow) // RA2: 0x006FA180
{
	if (!narrow) { // RA2: as above
		return 0; // RA2: as above
	}
	wchar_t * buffer = WideBuffers[WideIndex]; // RA2: as above
	Char_To_Wide(buffer, narrow, 0x400); // RA2: as above
	buffer[0x3FF] = 0; // RA2: as above
	WideIndex = (WideIndex + 1) & 7; // RA2: the next call uses the next buffer
	return buffer; // RA2: as above
}
