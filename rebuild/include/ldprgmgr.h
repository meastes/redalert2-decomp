/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 * Copyright 2026 ra2-decomp contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/

#pragma once

#include "always.h"
#include "rect.h"
#include "abstract.h"

class MixFileClass;
class Surface;
class BSurface;

/*
**	RA2: the loading screen shown while a scenario loads. It is made on first use, keeps the
**	art of the screen (the title bar, the picture and the bar below it in a campaign, the
**	picture of the player's country in a multiplayer game) drawn once into a surface of
**	its own, and gives that surface back whenever the progress bar asks for a redraw
**	(message 0x11AE of the INoticeSink the progress screen is told of).
*/
class LoadProgressManager : public INoticeSink
{
	public:
		LoadProgressManager(void); // RA2: 0x5359A0

		virtual bool Unknown_Virtual_0(int message, int data) override; // RA2: 0x536EA0, the INoticeSink virtual; 0x11AE copies the picture to the surface

		static LoadProgressManager * Create(void); // RA2: 0x535910, makes the one object on first use
		static void Destroy(void); // RA2: 0x535970

		void Compute_Rects(void); // RA2: 0x5359E0, YR's name
		Point2D Text_Origin(void); // RA2: 0x535AB0, YR's name
		int Get_Row_Width(void); // RA2: 0x535B60, the width of the names beside the progress bars (-1 in a campaign)
		void Load_Mixes(void); // RA2: 0x535B90, YR's name; opens LOAD.MIX and loads the palettes
		void Draw(void); // RA2: 0x535BF0, YR's name; draws the whole screen into ProgressSurface
		void Copy_Surface(Surface * target); // RA2: 0x536E00
		void Release(void); // RA2: 0x536E20, frees the shapes, the palettes, LOAD.MIX and the surface

		static LoadProgressManager * Instance; // RA2: 0xA6E7C4

		MixFileClass * LoadMix; // RA2: +0x04, LOAD.MIX
		Rect TitleBarRect; // RA2: +0x08, a campaign's bar above the picture
		Rect PictureRect; // RA2: +0x18, the picture
		Rect TextBarRect; // RA2: +0x28, a campaign's bar below the picture
		wchar_t const * LoadMessage; // RA2: +0x38, the text in the title bar
		wchar_t const * LoadBriefing; // RA2: +0x3C, the text in the picture
		void * TitleBarShape; // RA2: +0x40
		void * PictureShape; // RA2: +0x44
		void * TextBarShape; // RA2: +0x48
		bool TitleBarShapeLoaded; // RA2: +0x4C, the shape was read from a file and is ours to free
		bool PictureShapeLoaded; // RA2: +0x4D
		bool TextBarShapeLoaded; // RA2: +0x4E
		int Field50; // RA2: +0x50, only the constructor writes it
		int Field54; // RA2: +0x54, as above
		int Field58; // RA2: +0x58, as above
		BSurface * ProgressSurface; // RA2: +0x5C
};
