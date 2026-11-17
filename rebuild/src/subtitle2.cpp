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
#include "movies.h" // RA2: the includes this file needs
#include "subtitle.h" // RA2: the includes this file needs
#include "straw.h" // RA2: the includes this file needs
#include "vector.h" // RA2: the includes this file needs
#include "dbgprint.h" // RA2: the includes this file needs
#include <stddef.h> // RA2: the includes this file needs
#include <stdlib.h> // RA2: the includes this file needs
#include <string.h> // RA2: the includes this file needs
#include <wchar.h> // RA2: the includes this file needs
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
	{L"Time", Parse_Time}, // RA2: as above
	{L"Duration", Parse_Duration}, // RA2: as above
	{L"Position", Parse_Position}, // RA2: as above
	{L"Color", Parse_Color}, // RA2: as above
	{L"Text", Parse_Text}, // RA2: as above
	{NULL, NULL} // RA2: as above
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
	{L"Left", SubTitle::Left}, // RA2: as above
	{L"Right", SubTitle::Right}, // RA2: as above
	{L"Center", SubTitle::Center}, // RA2: as above
	{NULL, SubTitle::Center} // RA2: as above
};

/// <summary>
/// RA2: starts reading the control file from its byte order mark; a file that is not unicode is only reported.
/// </summary>
SubTitleParserClass::SubTitleParserClass(Straw & input) // RA2: 0x0069B680
	: mInput(input), // RA2: as above
	mLineNumber(0) // RA2: as above
{
	wchar_t bom = 0; // RA2: as above
	mInput.Get(&bom, sizeof(wchar_t)); // RA2: as above
	if (bom != 0xFEFF) { // RA2: as above
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
	DynamicVectorClass<SubTitle *> * subtitles = NULL; // RA2: as above

	if (Find_Movie_Entry(moviename) == true) { // RA2: as above
		subtitles = new DynamicVectorClass<SubTitle *>; // RA2: as above

		if (subtitles != NULL) { // RA2: as above
			for (;;) { // RA2: as above
				wchar_t * string = Get_Next_Line(); // RA2: as above

				if (string != NULL && wcslen(string) > 0) { // RA2: as above
					if (string[0] == L'<' && string[wcslen(string) - 1] == L'>') { // RA2: a line in angle brackets is one subtitle
						string++; // RA2: as above
						string[wcslen(string) - 1] = 0; // RA2: as above
						wcstrim(string); // RA2: as above

						if (wcslen(string) == 0) { // RA2: as above
							continue; // RA2: as above
						}

						SubTitle * subtitle = new SubTitle(); // RA2: as above

						if (subtitle == NULL) { // RA2: as above
							DebugString("***** Failed to create SubTitle!\n"); // RA2: 0x007F644C
							break; // RA2: as above
						}

						if (Parse_Sub_Title(string, subtitle) == true) { // RA2: as above
							subtitles->Add(subtitle); // RA2: as above
						} else { // RA2: as above
							delete subtitle; // RA2: as above
						}

						continue; // RA2: as above
					}

					if (wcsnicmp(string, L"EndMovie", wcslen(L"EndMovie")) == 0) { // RA2: as above
						break; // RA2: as above
					}
				}
			}

			if (subtitles->Count() == 0) { // RA2: as above
				delete subtitles; // RA2: as above
				subtitles = NULL; // RA2: as above
			}
		}
	}

	return subtitles; // RA2: as above
}

/// <summary>
/// RA2: reads lines until the BeginMovie line of the named movie; false at the end of the file.
/// </summary>
bool SubTitleParserClass::Find_Movie_Entry(char const * moviename) // RA2: 0x0069B9A0
{
	wchar_t movie[32]; // RA2: as above
	mbstowcs(movie, moviename, 32); // RA2: as above
	wchar_t * line; // RA2: as above
	while ((line = Get_Next_Line()) != NULL) { // RA2: as above
		if (_wcsnicmp(line, L"BeginMovie", wcslen(L"BeginMovie")) == 0) { // RA2: as above
			wchar_t * name = wcschr(line, L' '); // RA2: as above
			if (name) { // RA2: as above
				wcstrim(name); // RA2: as above
				if (_wcsicmp(name, movie) == 0) { // RA2: as above
					DebugString("Found movie entry %s\n", moviename); // RA2: as above
					return true; // RA2: as above
				}
			}
		}
	}
	return false; // RA2: as above
}

/// <summary>
/// RA2: reads the name=value pairs of one subtitle line into the subtitle; false (after a message) when a pair
/// has no = sign, a quote without its end or no name.
/// </summary>
bool SubTitleParserClass::Parse_Sub_Title(wchar_t * string, SubTitle * subtitle) // RA2: 0x0069BAD0
{
	for (;;) { // RA2: as above
		wchar_t * separator = wcschr(string, L'='); // RA2: as above

		if (separator == NULL) { // RA2: as above
			DebugString("Error on line %d: syntax error\n", mLineNumber); // RA2: as above
			return false; // RA2: as above
		}

		*separator++ = 0; // RA2: as above

		wchar_t * token = string; // RA2: as above
		wcstrim(token); // RA2: as above

		wchar_t * param = separator; // RA2: as above
		wcstrim(param); // RA2: as above

		if (param[0] == L'"') { // RA2: a quoted value runs to the closing quote
			param++; // RA2: as above
			separator = wcschr(param, L'"'); // RA2: as above

			if (separator == NULL) { // RA2: as above
				DebugString("Error on line %d: mismatched quotes\n", mLineNumber); // RA2: as above
				return false; // RA2: as above
			}

			*separator++ = 0; // RA2: as above
			wcstrim(separator); // RA2: as above

			if (*separator == L',') { // RA2: as above
				separator++; // RA2: as above
			}
			string = separator; // RA2: as above
		} else { // RA2: as above
			separator = wcspbrk(param, L", "); // RA2: a plain value runs to the next comma or blank

			if (separator != NULL) { // RA2: as above
				*separator++ = 0; // RA2: as above
				string = separator; // RA2: as above
			} else { // RA2: as above
				string = L""; // RA2: as above
			}
		}

		if (wcslen(token) == 0) { // RA2: as above
			DebugString("Error on line %d: missing token\n", mLineNumber); // RA2: as above
			return false; // RA2: as above
		}

		Parse_Token(token, param, subtitle); // RA2: as above

		wcstrim(string); // RA2: as above

		if (wcslen(string) == 0) { // RA2: as above
			break; // RA2: as above
		}
	}

	return true; // RA2: as above
}

/// <summary>
/// RA2: hands the value to the routine of the token (Time, Duration, Position, Color or Text); unknown tokens are ignored.
/// </summary>
void __stdcall Parse_Token(wchar_t const * token, wchar_t * param, SubTitle * subtitle) // RA2: 0x0069BC60
{
	if (token != NULL && SubtitleTokens[0].Name != NULL) { // RA2: as above
		SubtitleToken * var = SubtitleTokens; // RA2: as above
		SubtitleToken * entry = var; // RA2: as above
		while (entry->Name != NULL) { // RA2: as above
			if (_wcsicmp(var->Name, token) == 0) { // RA2: as above
				var->Handler(param, subtitle); // RA2: as above
				break; // RA2: as above
			}
			entry++; // RA2: as above
			var = entry; // RA2: as above
		}
	}
}

/// <summary>
/// RA2: the next line of the control file that is not empty and not a comment (starting with ;), trimmed; NULL at the end.
/// </summary>
wchar_t * SubTitleParserClass::Get_Next_Line(void) // RA2: 0x0069BCB0
{
	bool eof = false; // RA2: as above
	while (!eof) { // RA2: as above
		eof = Read_Line(mInput, mBuffer, 1024); // RA2: as above
		++mLineNumber; // RA2: as above
		wchar_t * line = wcstrim(mBuffer); // RA2: as above
		if (wcslen(line) > 0 && *line != L';') { // RA2: as above
			return line; // RA2: as above
		}
	}
	return NULL; // RA2: as above
}

/// <summary>
/// RA2: a time as hours:minutes:seconds:hundredths in ticks of 1/60 second.
/// </summary>
unsigned long __fastcall Parse_Timecode(wchar_t const * text) // RA2: 0x0069BD60
{
	wchar_t buf[12]; // RA2: as above
	wchar_t * colon; // RA2: as above

	wcsncpy(buf, text, 12); // RA2: as above
	buf[11] = 0; // RA2: as above

	colon = wcschr(buf, L':'); // RA2: as above
	*colon = 0; // RA2: as above
	unsigned long hours = wcstoul(buf, NULL, 10); // RA2: as above

	wchar_t * minutes = colon + 1; // RA2: as above
	colon = wcschr(minutes, L':'); // RA2: as above
	*colon = 0; // RA2: as above
	unsigned long mins = wcstoul(minutes, NULL, 10); // RA2: as above

	wchar_t * seconds = colon + 1; // RA2: as above
	colon = wcschr(seconds, L':'); // RA2: as above
	*colon = 0; // RA2: as above
	unsigned long secs = wcstoul(seconds, NULL, 10); // RA2: as above

	wchar_t * frames = colon + 1; // RA2: as above
	unsigned long hundredths = wcstoul(frames, NULL, 10); // RA2: as above

	return (((hours * 60 + mins) * 60 + secs) * 60) + (hundredths * 60) / 100; // RA2: as above
}

/// <summary>
/// RA2: Time=: when the subtitle shows.
/// </summary>
void __fastcall Parse_Time(wchar_t * value, SubTitle * subtitle) // RA2: 0x0069BE30
{
	subtitle->Set_Display_Time(Parse_Timecode(value)); // RA2: as above
}

/// <summary>
/// RA2: Duration=: how long the subtitle stays; a zero keeps the 20 seconds.
/// </summary>
void __fastcall Parse_Duration(wchar_t * value, SubTitle * subtitle) // RA2: 0x0069BE40
{
	unsigned long duration = Parse_Timecode(value); // RA2: as above
	if (duration > 0) { // RA2: as above
		subtitle->Set_Display_Duration(duration); // RA2: as above
	}
}

/// <summary>
/// RA2: Position=: the line (optional, before a colon) and the alignment (Left, Right or Center).
/// </summary>
void __fastcall Parse_Position(wchar_t * value, SubTitle * subtitle) // RA2: 0x0069BE60
{
	wchar_t * colon = wcschr(value, L':'); // RA2: as above
	if (colon != NULL) { // RA2: as above
		*colon = 0; // RA2: as above
		subtitle->Set_Line_Position(wcstol(value, NULL, 0)); // RA2: as above
		value = colon + 1; // RA2: as above
	}

	int alignment = SubTitle::Center; // RA2: as above
	for (int i = 0; SubtitleAlignments[i].Name != NULL; i++) { // RA2: as above
		if (_wcsicmp(value, SubtitleAlignments[i].Name) == 0) { // RA2: as above
			alignment = SubtitleAlignments[i].Value; // RA2: as above
			break; // RA2: as above
		}
	}
	subtitle->Set_Alignment((SubTitle::Alignment)alignment); // RA2: as above
}

/// <summary>
/// RA2: Color=: red:green:blue.
/// </summary>
void __fastcall Parse_Color(wchar_t * value, SubTitle * subtitle) // RA2: 0x0069BEF0
{
	wchar_t * colon = wcschr(value, L':'); // RA2: as above
	*colon = 0; // RA2: as above
	unsigned char red = (unsigned char)wcstoul(value, NULL, 10); // RA2: as above

	wchar_t * next = colon + 1; // RA2: as above
	colon = wcschr(next, L':'); // RA2: as above
	*colon = 0; // RA2: as above
	unsigned char green = (unsigned char)wcstoul(next, NULL, 10); // RA2: as above

	unsigned char blue = (unsigned char)wcstoul(colon + 1, NULL, 10); // RA2: as above

	subtitle->Set_RGB_Color(red, green, blue); // RA2: as above
}

/// <summary>
/// RA2: Text=: the caption.
/// </summary>
void __fastcall Parse_Text(wchar_t * value, SubTitle * subtitle) // RA2: 0x0069BF70
{
	subtitle->Set_Caption(value); // RA2: as above
}

template class DynamicVectorClass<SubTitle *>; // RA2: the virtual functions of the subtitle list (0x0069BF80, 0x0069BFB0, 0x0069BFD0, 0x0069BFE0, 0x0069C020, 0x0069C0D0)
template class VectorClass<SubTitle *>; // RA2: the virtual functions of its base (0x0069C1D0 onward)

/// <summary>
/// RA2: reads one line of unicode text from the straw into the buffer, without the carriage returns;
/// true when the straw ran dry.
/// </summary>
bool __fastcall Read_Line(Straw & input, wchar_t * buffer, int count) // RA2: 0x0069C100
{
	buffer[0] = 0; // RA2: as above
	bool eof = false; // RA2: as above
	int length = 0; // RA2: as above

	while (length < count) { // RA2: as above
		wchar_t character; // RA2: as above
		if (input.Get(&character, sizeof(wchar_t)) != sizeof(wchar_t)) { // RA2: as above
			eof = true; // RA2: as above
			break; // RA2: as above
		}
		if (character == 0xA) { // RA2: as above
			break; // RA2: as above
		}
		if (character != 0xD) { // RA2: as above
			buffer[length++] = character; // RA2: as above
		}
	}
	buffer[length] = 0; // RA2: as above
	return eof; // RA2: as above
}
