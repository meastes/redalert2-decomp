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

	if (name != NULL && strlen(name) != 0) {
		MovieSubtitles * subs = new MovieSubtitles;
		if (subs != NULL) {
			_splitpath(name, NULL, NULL, fname, NULL);
			if (subs->Load_Sub_Titles(fname) == true) {
				BSurface * surface = new BSurface(640, 255, 2); // RA2: the 16 bit surface the lines are drawn on
				if (surface != NULL) {
					surface->Fill(0);
					subs->SubSurface = surface;
				}
			}
		}
		return subs;
	}
	return NULL;
}

/// <summary>
/// RA2: starts without subtitles, without a callback and with an empty caption rectangle.
/// </summary>
MovieSubtitles::MovieSubtitles(void) // RA2: 0x0069B280
	: mSubTitles(NULL),
	mSubTitleIndex(0),
	mActiveSubTitle(NULL),
	Callback(NULL),
	SubSurface(NULL),
	CaptionRect(0, 0, 0, 0)
{
}

/// <summary>
/// RA2: frees every subtitle, the list and the surface.
/// </summary>
MovieSubtitles::~MovieSubtitles(void) // RA2: 0x0069B2A0
{
	if (mSubTitles != NULL) {
		for (int i = 0; i < mSubTitles->Count(); i++) {
			SubTitle * subtitle = (*mSubTitles)[i];
			if (subtitle != NULL) {
				delete subtitle;
			}
		}
		delete mSubTitles;
	}
	if (SubSurface != NULL) {
		delete SubSurface;
	}
}

/// <summary>
/// RA2: reads the subtitles of the named movie from subtitle.txt; false when the file or the movie's entry is missing.
/// </summary>
bool MovieSubtitles::Load_Sub_Titles(char const * moviename) // RA2: 0x0069B300
{
	if (moviename != NULL && strlen(moviename) != 0) {
		CCFileClass file("subtitle.txt"); // RA2: the subtitle control file (0x007F62DC)

		if (file.Is_Available()) {
			FileStraw input(file);
			SubTitleParserClass parser(input);

			mSubTitles = parser.Get_Sub_Titles(moviename);

			if (mSubTitles == NULL) {
				return false;
			}

			return true;
		}
	}
	return false;
}

/// <summary>
/// RA2: shows every subtitle that is due at the movie time and ends the one that has run out; true when the
/// surface changed. A callback takes the subtitles over from the drawing.
/// </summary>
bool MovieSubtitles::Process(unsigned long movietime) // RA2: 0x0069B420
{
	if (mSubTitles == NULL) {
		return false;
	}

	bool changed = false;
	while (mSubTitleIndex < mSubTitles->Count()) {
		SubTitle * subtitle = (*mSubTitles)[mSubTitleIndex];
		if (subtitle->Get_Display_Time() > movietime) {
			break;
		}
		mActiveSubTitle = subtitle;
		mSubTitleIndex++;
		if (Callback != NULL) {
			Callback(subtitle);
		} else {
			Draw_Sub_Title(subtitle);
			changed = true;
		}
	}

	if (mActiveSubTitle != NULL) {
		if (movietime >= mActiveSubTitle->Get_Display_Time() + mActiveSubTitle->Get_Display_Duration()) {
			mActiveSubTitle = NULL;
			if (Callback == NULL && SubSurface != NULL) {
				Rect & caption = CaptionRect;
				SubSurface->Fill_Rect(caption, 0); // RA2: wipes the caption that ended
				changed = true;
				PreviousRect = caption; // RA2: remembers what was wiped
				caption.X = 0; // RA2: the caption rectangle is emptied member by member
				caption.Y = 0;
				caption.Width = 0;
				caption.Height = 0;
			}
		}
	}
	return changed;
}

/// <summary>
/// RA2: starts over at the first subtitle.
/// </summary>
void MovieSubtitles::Reset(void) // RA2: 0x0069B4E0
{
	mSubTitleIndex = 0;
	mActiveSubTitle = NULL;
}

/// <summary>
/// RA2: lets the callback show the subtitles instead of this class.
/// </summary>
void MovieSubtitles::SetCallback(void (__fastcall * callback)(void *)) // RA2: 0x0069B4F0
{
	Callback = callback;
}

/// <summary>
/// RA2: wipes the last caption and draws the subtitle into its place on the surface: coloured, on its line,
/// aligned to the left, right or center.
/// </summary>
void MovieSubtitles::Draw_Sub_Title(SubTitle * subtitle) // RA2: 0x0069B500
{
	if (SubSurface != NULL) {
		Rect & caption = CaptionRect;
		SubSurface->Fill_Rect(caption, 0); // RA2: wipes the last caption
		BitFont * font = BitFont::Instance;
		unsigned long rgb = subtitle->Get_RGB_Color();
		font->Set_Color(DSurface::Build_Hicolor_Pixel((rgb >> 16) & 0xFF, (rgb >> 8) & 0xFF, rgb & 0xFF)); // RA2: the colour as a 16 bit pixel
		int width = SubSurface->Get_Width();
		int height = SubSurface->Get_Height();
		LTRBStruct bounds; // RA2: the whole surface
		bounds.Left = 0;
		bounds.Top = 0;
		bounds.Right = width;
		bounds.Bottom = height;
		font->Set_Bounds(&bounds);
		wchar_t const * text = subtitle->Get_Caption();
		int textwidth;
		int textheight;
		font->Text_Dimensions(text, &textwidth, &textheight, width);
		int x = 0;
		int y = (subtitle->Get_Line_Position() - 1) * font->LineHeight; // RA2: lines count from 1
		if (subtitle->Get_Alignment() == SubTitle::Center) {
			x = (width - textwidth) / 2;
		} else if (subtitle->Get_Alignment() == SubTitle::Right) {
			x = width - textwidth;
		}
		BitText::Instance->Draw_Text(font, SubSurface, text, x, y, width, height, 0, 0, 0);
		PreviousRect = caption;
		caption.X = x; // RA2: the caption rectangle takes the drawn text's place
		caption.Y = y;
		caption.Width = textwidth;
		caption.Height = textheight;
	}
}
