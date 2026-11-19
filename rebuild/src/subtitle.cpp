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
**	RA2: the movie subtitles: the subtitle control file (subtitle.txt) is read once per movie into a
**	list of timed lines (subtitle2.cpp), which MovieSubtitles then draws into a 640x255 surface as the
**	movie plays. TibSun has no such file; the code is the executable's from 0x0069B190.
*/

#include "always.h" // RA2: the includes this file needs
#include "movies.h" // RA2: the includes this file needs
#include "subtitle.h" // RA2: the includes this file needs
#include "bsurface.h" // RA2: the includes this file needs
#include "straw.h" // RA2: the includes this file needs
#include "vector.h" // RA2: the includes this file needs
#include "bitfont.h" // RA2: the includes this file needs
#include "bittext.h" // RA2: the includes this file needs
#include "dsurface.h" // RA2: the includes this file needs
#include "ccfile.h" // RA2: the includes this file needs
#include "xstraw.h" // RA2: the includes this file needs
#include "dbgprint.h" // RA2: the includes this file needs
#include <stddef.h> // RA2: the includes this file needs
#include <stdlib.h> // RA2: the includes this file needs
#include <string.h> // RA2: the includes this file needs
#include <wchar.h> // RA2: the includes this file needs

/// <summary>
/// RA2: loads the subtitles of a movie; the surface they are drawn on exists only when the movie has some.
/// </summary>
MovieSubtitles * __fastcall Subtitles_Open(char const * name) // RA2: 0x0069B190
{
	char fname[256]; // RA2: the file name without path and extension

	if (name != NULL && strlen(name) != 0) { // RA2: as above
		MovieSubtitles * subs = new MovieSubtitles; // RA2: as above
		if (subs != NULL) { // RA2: as above
			_splitpath(name, NULL, NULL, fname, NULL); // RA2: as above
			if (subs->Load_Sub_Titles(fname) == true) { // RA2: as above
				BSurface * surface = new BSurface(640, 255, 2); // RA2: the 16 bit surface the lines are drawn on
				if (surface != NULL) { // RA2: as above
					surface->Fill(0); // RA2: as above
					subs->SubSurface = surface; // RA2: as above
				}
			}
		}
		return subs; // RA2: as above
	}
	return NULL; // RA2: as above
}

/// <summary>
/// RA2: starts without subtitles, without a callback and with an empty caption rectangle.
/// </summary>
MovieSubtitles::MovieSubtitles(void) // RA2: 0x0069B280
	: mSubTitles(NULL), // RA2: as above
	mSubTitleIndex(0), // RA2: as above
	mActiveSubTitle(NULL), // RA2: as above
	Callback(NULL), // RA2: as above
	SubSurface(NULL), // RA2: as above
	CaptionRect(0, 0, 0, 0) // RA2: as above
{
}

/// <summary>
/// RA2: frees every subtitle, the list and the surface.
/// </summary>
MovieSubtitles::~MovieSubtitles(void) // RA2: 0x0069B2A0
{
	if (mSubTitles != NULL) { // RA2: as above
		for (int i = 0; i < mSubTitles->Count(); i++) { // RA2: as above
			SubTitle * subtitle = (*mSubTitles)[i]; // RA2: as above
			if (subtitle != NULL) { // RA2: as above
				delete subtitle; // RA2: as above
			}
		}
		delete mSubTitles; // RA2: as above
	}
	if (SubSurface != NULL) { // RA2: as above
		delete SubSurface; // RA2: as above
	}
}

/// <summary>
/// RA2: reads the subtitles of the named movie from subtitle.txt; false when the file or the movie's entry is missing.
/// </summary>
bool MovieSubtitles::Load_Sub_Titles(char const * moviename) // RA2: 0x0069B300
{
	if (moviename != NULL && strlen(moviename) != 0) { // RA2: as above
		CCFileClass file("subtitle.txt"); // RA2: the subtitle control file (0x007F62DC)

		if (file.Is_Available()) { // RA2: as above
			FileStraw input(file); // RA2: as above
			SubTitleParserClass parser(input); // RA2: as above

			mSubTitles = parser.Get_Sub_Titles(moviename); // RA2: as above

			if (mSubTitles == NULL) { // RA2: as above
				return false; // RA2: as above
			}

			return true; // RA2: as above
		}
	}
	return false; // RA2: as above
}

/// <summary>
/// RA2: shows every subtitle that is due at the movie time and ends the one that has run out; true when the
/// surface changed. A callback takes the subtitles over from the drawing.
/// </summary>
bool MovieSubtitles::Process(unsigned long movietime) // RA2: 0x0069B420
{
	if (mSubTitles == NULL) { // RA2: as above
		return false; // RA2: as above
	}

	bool changed = false; // RA2: as above
	while (mSubTitleIndex < mSubTitles->Count()) { // RA2: as above
		SubTitle * subtitle = (*mSubTitles)[mSubTitleIndex]; // RA2: as above
		if (subtitle->Get_Display_Time() > movietime) { // RA2: as above
			break; // RA2: as above
		}
		mActiveSubTitle = subtitle; // RA2: as above
		mSubTitleIndex++; // RA2: as above
		if (Callback != NULL) { // RA2: as above
			Callback(subtitle); // RA2: as above
		} else { // RA2: as above
			Draw_Sub_Title(subtitle); // RA2: as above
			changed = true; // RA2: as above
		}
	}

	if (mActiveSubTitle != NULL) { // RA2: as above
		if (movietime >= mActiveSubTitle->Get_Display_Time() + mActiveSubTitle->Get_Display_Duration()) { // RA2: as above
			mActiveSubTitle = NULL; // RA2: as above
			if (Callback == NULL && SubSurface != NULL) { // RA2: as above
				Rect & caption = CaptionRect; // RA2: as above
				SubSurface->Fill_Rect(caption, 0); // RA2: wipes the caption that ended
				changed = true; // RA2: as above
				PreviousRect = caption; // RA2: remembers what was wiped
				caption.X = 0; // RA2: the caption rectangle is emptied member by member
				caption.Y = 0; // RA2: as above
				caption.Width = 0; // RA2: as above
				caption.Height = 0; // RA2: as above
			}
		}
	}
	return changed; // RA2: as above
}

/// <summary>
/// RA2: starts over at the first subtitle.
/// </summary>
void MovieSubtitles::Reset(void) // RA2: 0x0069B4E0
{
	mSubTitleIndex = 0; // RA2: as above
	mActiveSubTitle = NULL; // RA2: as above
}

/// <summary>
/// RA2: lets the callback show the subtitles instead of this class.
/// </summary>
void MovieSubtitles::SetCallback(void (__fastcall * callback)(void *)) // RA2: 0x0069B4F0
{
	Callback = callback; // RA2: as above
}

/// <summary>
/// RA2: wipes the last caption and draws the subtitle into its place on the surface: coloured, on its line,
/// aligned to the left, right or center.
/// </summary>
void MovieSubtitles::Draw_Sub_Title(SubTitle * subtitle) // RA2: 0x0069B500
{
	if (SubSurface != NULL) { // RA2: as above
		Rect & caption = CaptionRect; // RA2: as above
		SubSurface->Fill_Rect(caption, 0); // RA2: wipes the last caption
		BitFont * font = BitFont::Instance; // RA2: as above
		unsigned long rgb = subtitle->Get_RGB_Color(); // RA2: as above
		font->Set_Color(DSurface::Build_Hicolor_Pixel((rgb >> 16) & 0xFF, (rgb >> 8) & 0xFF, rgb & 0xFF)); // RA2: the colour as a 16 bit pixel
		int width = SubSurface->Get_Width(); // RA2: as above
		int height = SubSurface->Get_Height(); // RA2: as above
		LTRBStruct bounds; // RA2: the whole surface
		bounds.Left = 0; // RA2: as above
		bounds.Top = 0; // RA2: as above
		bounds.Right = width; // RA2: as above
		bounds.Bottom = height; // RA2: as above
		font->Set_Bounds(&bounds); // RA2: as above
		wchar_t const * text = subtitle->Get_Caption(); // RA2: as above
		int textwidth; // RA2: as above
		int textheight; // RA2: as above
		font->Text_Dimensions(text, &textwidth, &textheight, width); // RA2: as above
		int x = 0; // RA2: as above
		int y = (subtitle->Get_Line_Position() - 1) * font->LineHeight; // RA2: lines count from 1
		if (subtitle->Get_Alignment() == SubTitle::Center) { // RA2: as above
			x = (width - textwidth) / 2; // RA2: as above
		} else if (subtitle->Get_Alignment() == SubTitle::Right) { // RA2: as above
			x = width - textwidth; // RA2: as above
		}
		BitText::Instance->Draw_Text(font, SubSurface, text, x, y, width, height, 0, 0, 0); // RA2: as above
		PreviousRect = caption; // RA2: as above
		caption.X = x; // RA2: the caption rectangle takes the drawn text's place
		caption.Y = y; // RA2: as above
		caption.Width = textwidth; // RA2: as above
		caption.Height = textheight; // RA2: as above
	}
}
