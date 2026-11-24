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
**	RA2: the parser of the movie subtitle control file (subtitle.txt): it reads the lines between a
**	movie's BeginMovie and EndMovie into SubTitle objects. A separate object from subtitle.cpp (its
**	callers there do not expand the parser). TibSun has no such file; the code is the executable's
**	from 0x0069B680.
*/

#include "always.h" // RA2: the includes this file needs
#include <stddef.h> // RA2: the includes this file needs
#include <stdlib.h> // RA2: the includes this file needs
#include <string.h> // RA2: the includes this file needs
#include <wchar.h> // RA2: the includes this file needs
#include "subtitle.h" // RA2: the includes this file needs
#include "subtitleparser.h"
#include "straw.h" // RA2: the includes this file needs
#include "vector.h" // RA2: the includes this file needs
#include "dbgprint.h" // RA2: the includes this file needs
#include "globals.h"
#include "readline.h"



/// <summary>
/// RA2: the names a subtitle control file line may set and the routine that reads each value (0x007F62F0).
/// </summary>
struct SubtitleToken
{
	wchar_t const * Name; // RA2: +0x00; the name before the = sign
	void (__fastcall * Handler)(wchar_t *, SubTitle *); // RA2: +0x04; reads the value into the subtitle
};

SubtitleToken SubtitleTokens[] = { // RA2: 0x007F62F0; ends with an empty entry
	{ L"Time", Parse_Time },
	{ L"Duration", Parse_Duration },
	{ L"Position", Parse_Position },
	{ L"Color", Parse_Color },
	{ L"Text", Parse_Text },
	{ NULL, NULL }
};

/// <summary>
/// RA2: the alignments a Position value may name (0x007F6338); Center when none matches.
/// </summary>
struct SubtitleAlignment
{
	wchar_t const * Name; // RA2: +0x00
	int Value; // RA2: +0x04, a SubTitle::Alignment
};

SubtitleAlignment SubtitleAlignments[] = { // RA2: 0x007F6338
	{ L"Left", SubTitle::Left },
	{ L"Right", SubTitle::Right },
	{ L"Center", SubTitle::Center },
	{ NULL, SubTitle::Center }
};

/// <summary>
/// RA2: starts reading the control file from its byte order mark; a file that is not unicode is only reported.
/// </summary>
SubTitleParserClass::SubTitleParserClass(Straw & input) // RA2: 0x0069B680
	: mInput(input),
	mLineNumber(0)
{
	wchar_t bom = 0;
	mInput.Get(&bom, sizeof(wchar_t));
	if (bom != 0xFEFF) {
		DebugString("Error: Subtitle control file is not unicode!\n"); // RA2: 0x007F63B4
	}
}

SubTitleParserClass::~SubTitleParserClass(void) // RA2: 0x0069B6C0
{
}

/// <summary>
/// RA2: reads the subtitles between the movie's BeginMovie line and its EndMovie line into a new list; NULL
/// when the movie has no entry or no subtitles.
/// </summary>
DynamicVectorClass<SubTitle *> * SubTitleParserClass::Get_Sub_Titles(char const * moviename) // RA2: 0x0069B6D0
{
	DynamicVectorClass<SubTitle *> * subtitles = NULL;

	if (Find_Movie_Entry(moviename) == true) {
		subtitles = new DynamicVectorClass<SubTitle *>;

		if (subtitles != NULL) {
			for (;;) {
				wchar_t * string = Get_Next_Line();

				if (string != NULL && wcslen(string) > 0) {
					if (string[0] == L'<' && string[wcslen(string) - 1] == L'>') { // RA2: a line in angle brackets is one subtitle
						string++;
						string[wcslen(string) - 1] = 0;
						wcstrim(string);

						if (wcslen(string) == 0) {
							continue;
						}

						SubTitle * subtitle = new SubTitle();

						if (subtitle == NULL) {
							DebugString("***** Failed to create SubTitle!\n"); // RA2: 0x007F644C
							break;
						}

						if (Parse_Sub_Title(string, subtitle) == true) {
							subtitles->Add(subtitle);
						} else {
							delete subtitle;
						}

						continue;
					}

					if (wcsnicmp(string, L"EndMovie", wcslen(L"EndMovie")) == 0) {
						break;
					}
				}
			}

			if (subtitles->Count() == 0) {
				delete subtitles;
				subtitles = NULL;
			}
		}
	}

	return subtitles;
}

/// <summary>
/// RA2: reads lines until the BeginMovie line of the named movie; false at the end of the file.
/// </summary>
bool SubTitleParserClass::Find_Movie_Entry(char const * moviename) // RA2: 0x0069B9A0
{
	wchar_t movie[32];
	mbstowcs(movie, moviename, 32);
	wchar_t * line;
	while ((line = Get_Next_Line()) != NULL) {
		if (_wcsnicmp(line, L"BeginMovie", wcslen(L"BeginMovie")) == 0) {
			wchar_t * name = wcschr(line, L' ');
			if (name) {
				wcstrim(name);
				if (_wcsicmp(name, movie) == 0) {
					DebugString("Found movie entry %s\n", moviename);
					return true;
				}
			}
		}
	}
	return false;
}

/// <summary>
/// RA2: reads the name=value pairs of one subtitle line into the subtitle; false (after a message) when a pair
/// has no = sign, a quote without its end or no name.
/// </summary>
bool SubTitleParserClass::Parse_Sub_Title(wchar_t * string, SubTitle * subtitle) // RA2: 0x0069BAD0
{
	for (;;) {
		wchar_t * separator = wcschr(string, L'=');

		if (separator == NULL) {
			DebugString("Error on line %d: syntax error\n", mLineNumber);
			return false;
		}

		*separator++ = 0;

		wchar_t * token = string;
		wcstrim(token);

		wchar_t * param = separator;
		wcstrim(param);

		if (param[0] == L'"') { // RA2: a quoted value runs to the closing quote
			param++;
			separator = wcschr(param, L'"');

			if (separator == NULL) {
				DebugString("Error on line %d: mismatched quotes\n", mLineNumber);
				return false;
			}

			*separator++ = 0;
			wcstrim(separator);

			if (*separator == L',') {
				separator++;
			}
			string = separator;
		} else {
			separator = wcspbrk(param, L", "); // RA2: a plain value runs to the next comma or blank

			if (separator != NULL) {
				*separator++ = 0;
				string = separator;
			} else {
				string = L"";
			}
		}

		if (wcslen(token) == 0) {
			DebugString("Error on line %d: missing token\n", mLineNumber);
			return false;
		}

		Parse_Token(token, param, subtitle);

		wcstrim(string);

		if (wcslen(string) == 0) {
			break;
		}
	}

	return true;
}

/// <summary>
/// RA2: hands the value to the routine of the token (Time, Duration, Position, Color or Text); unknown tokens are ignored.
/// </summary>
void __stdcall Parse_Token(wchar_t const * token, wchar_t * param, SubTitle * subtitle) // RA2: 0x0069BC60
{
	if (token != NULL && SubtitleTokens[0].Name != NULL) {
		SubtitleToken * var = SubtitleTokens;
		SubtitleToken * entry = var;
		while (entry->Name != NULL) {
			if (_wcsicmp(var->Name, token) == 0) {
				var->Handler(param, subtitle);
				break;
			}
			entry++;
			var = entry;
		}
	}
}

/// <summary>
/// RA2: the next line of the control file that is not empty and not a comment (starting with ;), trimmed; NULL at the end.
/// </summary>
wchar_t * SubTitleParserClass::Get_Next_Line(void) // RA2: 0x0069BCB0
{
	bool eof = false;
	while (!eof) {
		eof = Read_Line(mInput, mBuffer, 1024);
		++mLineNumber;
		wchar_t * line = wcstrim(mBuffer);
		if (wcslen(line) > 0 && *line != L';') {
			return line;
		}
	}
	return NULL;
}

/// <summary>
/// RA2: a time as hours:minutes:seconds:hundredths in ticks of 1/60 second.
/// </summary>
unsigned long __fastcall Parse_Timecode(wchar_t const * text) // RA2: 0x0069BD60
{
	wchar_t buf[12];
	wchar_t * colon;

	wcsncpy(buf, text, 12);
	buf[11] = 0;

	colon = wcschr(buf, L':');
	*colon = 0;
	unsigned long hours = wcstoul(buf, NULL, 10);

	wchar_t * minutes = colon + 1;
	colon = wcschr(minutes, L':');
	*colon = 0;
	unsigned long mins = wcstoul(minutes, NULL, 10);

	wchar_t * seconds = colon + 1;
	colon = wcschr(seconds, L':');
	*colon = 0;
	unsigned long secs = wcstoul(seconds, NULL, 10);

	wchar_t * frames = colon + 1;
	unsigned long hundredths = wcstoul(frames, NULL, 10);

	return (((hours * 60 + mins) * 60 + secs) * 60) + (hundredths * 60) / 100;
}

/// <summary>
/// RA2: Time=: when the subtitle shows.
/// </summary>
void __fastcall Parse_Time(wchar_t * value, SubTitle * subtitle) // RA2: 0x0069BE30
{
	subtitle->Set_Display_Time(Parse_Timecode(value));
}

/// <summary>
/// RA2: Duration=: how long the subtitle stays; a zero keeps the 20 seconds.
/// </summary>
void __fastcall Parse_Duration(wchar_t * value, SubTitle * subtitle) // RA2: 0x0069BE40
{
	unsigned long duration = Parse_Timecode(value);
	if (duration > 0) {
		subtitle->Set_Display_Duration(duration);
	}
}

/// <summary>
/// RA2: Position=: the line (optional, before a colon) and the alignment (Left, Right or Center).
/// </summary>
void __fastcall Parse_Position(wchar_t * value, SubTitle * subtitle) // RA2: 0x0069BE60
{
	wchar_t * colon = wcschr(value, L':');
	if (colon != NULL) {
		*colon = 0;
		subtitle->Set_Line_Position(wcstol(value, NULL, 0));
		value = colon + 1;
	}

	int alignment = SubTitle::Center;
	for (int i = 0; SubtitleAlignments[i].Name != NULL; i++) {
		if (_wcsicmp(value, SubtitleAlignments[i].Name) == 0) {
			alignment = SubtitleAlignments[i].Value;
			break;
		}
	}
	subtitle->Set_Alignment((SubTitle::Alignment)alignment);
}

/// <summary>
/// RA2: Color=: red:green:blue.
/// </summary>
void __fastcall Parse_Color(wchar_t * value, SubTitle * subtitle) // RA2: 0x0069BEF0
{
	wchar_t * colon = wcschr(value, L':');
	*colon = 0;
	unsigned char red = (unsigned char)wcstoul(value, NULL, 10);

	wchar_t * next = colon + 1;
	colon = wcschr(next, L':');
	*colon = 0;
	unsigned char green = (unsigned char)wcstoul(next, NULL, 10);

	unsigned char blue = (unsigned char)wcstoul(colon + 1, NULL, 10);

	subtitle->Set_RGB_Color(red, green, blue);
}

/// <summary>
/// RA2: Text=: the caption.
/// </summary>
void __fastcall Parse_Text(wchar_t * value, SubTitle * subtitle) // RA2: 0x0069BF70
{
	subtitle->Set_Caption(value);
}


/// <summary>
/// RA2: reads one line of unicode text from the straw into the buffer, without the carriage returns;
/// true when the straw ran dry.
/// </summary>
bool __fastcall Read_Line(Straw & input, wchar_t * buffer, int count) // RA2: 0x0069C100
{
	buffer[0] = 0;
	bool eof = false;
	int length = 0;

	while (length < count) {
		wchar_t character;
		if (input.Get(&character, sizeof(wchar_t)) != sizeof(wchar_t)) {
			eof = true;
			break;
		}
		if (character == 0xA) {
			break;
		}
		if (character != 0xD) {
			buffer[length++] = character;
		}
	}
	buffer[length] = 0;
	return eof;
}
