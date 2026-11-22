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
**	RA2: the loading screen of a scenario (LdPrgMgr.cpp in the executable's debug text): its
**	areas, the art it draws once into a surface of its own, and the progress screen's hook that
**	hands that surface back. TibSun has no such file; the code is the executable's from 0x00535720.
*/

#include "always.h"
#include "rect.h"
#include "geometry.h"
#include "globals.h"
#include <string.h>
#include <stdio.h>
#include <wchar.h>
#include "abstract.h"
#include "_pk.h"
#include "_surface.h"
#include "bitfont.h"
#include "bsurface.h"
#include "convert.h"
#include "data.h"
#include "dialog.h"
#include "draw.h"
#include "dsurface.h"
#include "mixfile.h"
#include "misc.h"
#include "progress.h"
#include "rgb.h"
#include "scheme.h"
#include "rect.h"
#include "scenario.h"
#include "session.h"
#undef LoadString
#include "stringtable.h"
#include "ldprgmgr.h"
#include "ownrdraw.h"


#include "geometry.h"

Rect __fastcall Get_Briefing_Area(void);
Rect __fastcall Get_Loading_Text_Area(void);
void __fastcall Shade_Text_Area(wchar_t const * text, Surface * surface, int x, int y, int width, int flags, bool fill);
void __fastcall Draw_Area_Text(wchar_t const * text, Surface * surface, Rect const & area, unsigned short color, int flags);
Point2D __fastcall Copy_Point(Point2D const * source); // RA2: 0x006F1AE0 (txtlabel2.cpp)
void __fastcall Load_Loading_Screen_Palettes(int side); // RA2: 0x006F24A0 (txtlabel2.cpp)
void __fastcall Free_Loading_Screen_Palettes(void); // RA2: 0x006F2510 (txtlabel2.cpp)
void __fastcall Load_MP_Load_Palette(void); // RA2: 0x006F25C0 (txtlabel2.cpp)
void __fastcall Free_MP_Load_Palette(void); // RA2: 0x006F26D0 (txtlabel2.cpp)
namespace OwnerDraw { int __fastcall Draw_Text_In_Rect_Ex(Surface * surface, wchar_t const * text, RECT * area, BitFont * font, unsigned long color, int flags, int unknown7, int unknown8, int arg9, int arg10); } // RA2: 0x005FE020 (ownrdraw.cpp)

char const * UKShapeName = "ls%sukingdom.shp";
char const * CubaShapeName = "ls%scuba.shp";
char const * FranceShapeName = "ls%sfrance.shp";
char const * GermanyShapeName = "ls%sgermany.shp";
char const * IraqShapeName = "ls%siraq.shp";
char const * KoreaShapeName = "ls%skorea.shp";
char const * LibyaShapeName = "ls%slibya.shp";
char const * RussiaShapeName = "ls%srussia.shp";
char const * USShapeName = "ls%sustates.shp";
char const * ObserverShapeName = "ls%sobs.shp";
char const * Suffix640 = "640";
char const * Suffix800 = "800";


LoadProgressManager * LoadProgressManager::Instance;

/// <summary>
/// The loading screen, made on first use.
/// </summary>
LoadProgressManager * LoadProgressManager::Create(void)
{
	if (Instance == NULL) {
		Instance = new LoadProgressManager;
	}
	return Instance;
}

/// <summary>
/// Releases and deletes the loading screen.
/// </summary>
void LoadProgressManager::Destroy(void)
{
	if (Instance != NULL) {
		Instance->Release();
		delete Instance;
	}
	Instance = NULL;
}

/// <summary>
/// Starts with nothing loaded.
/// </summary>
LoadProgressManager::LoadProgressManager(void)
{
	LoadMix = NULL;
	LoadMessage = NULL;
	LoadBriefing = NULL;
	TitleBarShape = NULL;
	PictureShape = NULL;
	TextBarShape = NULL;
	TitleBarShapeLoaded = false;
	PictureShapeLoaded = false;
	TextBarShapeLoaded = false;
	Field50 = 0;
	Field54 = 0;
	Field58 = 0;
	ProgressSurface = NULL;
}

/// <summary>
/// Places the areas by the screen size: a campaign stacks the title bar, the picture and the bar
/// below it, centred; a multiplayer game has the picture only, centred.
/// </summary>
void LoadProgressManager::Compute_Rects(void)
{
	int render_width = VideoModeWidth;
	bool low_res = render_width == 640;
	int width = low_res ? ScreenWidths[0] : ScreenWidths[1];
	if (Session.Type == GAME_NORMAL) {
		int height = (low_res ? ScreenHeights[0] : ScreenHeights[1]) - 80;
		int x = (render_width - width) / 2;
		int y = (VideoModeHeight - height - 80) / 2;
		TextBarRect.Width = PictureRect.Width = TitleBarRect.Width = width;
		PictureRect.X = TitleBarRect.X = x;
		TitleBarRect.Y = y;
		TitleBarRect.Height = 40;
		y += 40;
		PictureRect.Y = y;
		TextBarRect.X = x;
		PictureRect.Height = height;
		y += height;
		TextBarRect.Y = y;
		TextBarRect.Height = 40;
	} else {
		int height = low_res ? ScreenHeights[0] : ScreenHeights[1];
		PictureRect.Set((render_width - width) / 2, (VideoModeHeight - height) / 2, width, height);
	}
}


/// <summary>
/// Where the progress bar's text starts: in a campaign inside the bar below the picture, in a
/// multiplayer game at an offset from the picture's corner.
/// </summary>
Point2D LoadProgressManager::Text_Origin(void)
{
	Point2D p;
	if (Session.Type == GAME_NORMAL) {
		p.X = TextBarRect.X;
		if (VideoModeWidth == ScreenWidths[0]) {
			p.X += 0x54;
		} else {
			p.X += 0xA4;
		}
		p.Y = TextBarRect.Y + 7;
	} else {
		p = Copy_Point((Point2D const *)&PictureRect);
		if (VideoModeWidth == ScreenWidths[0]) {
			p.X += 0xC;
			p.Y += 0x100;
		} else {
			p.X += 0x10;
			p.Y += 0x141;
		}
	}
	return p;
}


/// <summary>
/// The width of the names beside the progress bars: -1 in a campaign, 326 or 406 otherwise.
/// </summary>
int LoadProgressManager::Get_Row_Width(void)
{
	int width = -1;
	if (Session.Type != GAME_NORMAL) {
		width = VideoModeWidth == ScreenWidths[0] ? 326 : 406;
	}
	return width;
}

/// <summary>
/// Opens LOAD.MIX once and loads the palettes of the art: the campaign's by the player's side,
/// the multiplayer one otherwise.
/// </summary>
void LoadProgressManager::Load_Mixes(void)
{
	if (LoadMix == NULL) {
		LoadMix = new MixFileClass("LOAD.MIX", &FastKey);
		if (LoadMix == NULL) {
			return;
		}
	}
	if (Session.Type == GAME_NORMAL) {
		Load_Loading_Screen_Palettes(Scen->IsGDI == 0);
	} else {
		Load_MP_Load_Palette();
	}
}


/// <summary>
/// Whether the local player watches the game instead of playing it.
/// </summary>
#ifdef RA2_NON_MATCHING // not exact: no independent bound yet (00535bf0 has no row); 3730 of 3978 bytes differ against the frozen image bound (image_bounds.csv) (emitted 3712 bytes; the draft made to compile: MP point, drawer and file-name declarations), 2026-11-22 (lane O32)
ConvertClass * __cdecl Get_Load_Screen_Drawer(void); // RA2: 0x006F2450
ConvertClass * __cdecl Get_Load_Screen_Side_Drawer(void); // RA2: 0x006F2470
ConvertClass * __cdecl Get_Splash_Bar_Drawer(void); // RA2: 0x006F2490
ConvertClass * __cdecl Get_MP_Load_Drawer(void); // RA2: 0x006F25B0
char const * __fastcall Get_Title_Bar_File(int width, int, int); // RA2: 0x006F2400
char const * __fastcall Get_Splash_Bar_File(int width, int, int); // RA2: 0x006F2420

inline bool Is_Observer(void)
{
	return (Session.Type == GAME_IPX || Session.Type == GAME_INTERNET) && Session.Players[0]->Slot.SpectatorFlag == -1;
}

/// <summary>
/// The name of the picture file of a country, or of the observer, for a screen size.
/// </summary>
inline void Loading_Filename(char * filename, int side, char const * suffix)
{
	if (Is_Observer()) {
		sprintf(filename, ObserverShapeName, suffix);
	} else if (side == 0) {
		sprintf(filename, USShapeName, suffix);
	} else if (side == 1) {
		sprintf(filename, KoreaShapeName, suffix);
	} else if (side == 2) {
		sprintf(filename, FranceShapeName, suffix);
	} else if (side == 3) {
		sprintf(filename, GermanyShapeName, suffix);
	} else if (side == 4) {
		sprintf(filename, UKShapeName, suffix);
	} else if (side == 5) {
		sprintf(filename, LibyaShapeName, suffix);
	} else if (side == 6) {
		sprintf(filename, IraqShapeName, suffix);
	} else if (side == 7) {
		sprintf(filename, CubaShapeName, suffix);
	} else if (side == 8) {
		sprintf(filename, RussiaShapeName, suffix);
	}
}

/// <summary>
/// The area of the country's name, 200 by 20 at the bottom, centred on screens past 800 by 600.
/// </summary>
inline void Name_Area(Rect & area)
{
	if (VideoModeWidth == ScreenWidths[0]) {
		area.X = 385;
		area.Y = 436;
	} else {
		area.X = 533;
		area.Y = 545;
	}
	area.Width = 200;
	area.Height = 20;
	if (VideoModeWidth >= ScreenWidths[1]) {
		area.X += (VideoModeWidth - ScreenWidths[1]) / 2;
		area.Y += (VideoModeHeight - ScreenHeights[1]) / 2;
	}
}

/// <summary>
/// The area of the name of the country's unit, 200 by 20 at the top, centred on screens past 800 by 600.
/// </summary>
inline Rect Unit_Area(void)
{
	Rect area;
	if (VideoModeWidth == ScreenWidths[0]) {
		area.X = 16;
		area.Y = 72;
	} else {
		area.X = 20;
		area.Y = 90;
	}
	area.Width = 200;
	area.Height = 20;
	if (VideoModeWidth >= ScreenWidths[1]) {
		area.X += (VideoModeWidth - ScreenWidths[1]) / 2;
		area.Y += (VideoModeHeight - ScreenHeights[1]) / 2;
	}
	return area;
}

/// <summary>
/// Changes the lower case letters of a text to capitals, those of the Latin-1 letters from 0xE0 to 0xFE too.
/// </summary>
inline void Upper_Case(wchar_t * string)
{
	for (wchar_t * p = string; *p != 0; p++) {
		if ((*p >= 'a' && *p <= 'z') || (*p >= 0xE0 && *p <= 0xFE)) {
			*p -= 0x20;
		}
	}
}

/// <summary>
/// The name of a country, or of the observer, from the string table.
/// </summary>
inline wchar_t const * Country_Name(int side)
{
	if (Is_Observer()) {
		return StringTable::LoadString("Name:Observer", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 162);
	}
	if (side == 0) {
		return StringTable::LoadString("Name:Americans", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 167);
	}
	if (side == 1) {
		return StringTable::LoadString("Name:Alliance", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 169);
	}
	if (side == 2) {
		return StringTable::LoadString("Name:French", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 171);
	}
	if (side == 3) {
		return StringTable::LoadString("Name:Germans", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 173);
	}
	if (side == 4) {
		return StringTable::LoadString("Name:British", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 175);
	}
	if (side == 5) {
		return StringTable::LoadString("Name:Africans", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 177);
	}
	if (side == 6) {
		return StringTable::LoadString("Name:Arabs", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 179);
	}
	if (side == 7) {
		return StringTable::LoadString("Name:Confederation", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 181);
	}
	if (side == 8) {
		return StringTable::LoadString("Name:Russians", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 183);
	}
	return NULL;
}

/// <summary>
/// The name of a country's unit from the string table.
/// </summary>
inline wchar_t const * Unit_Name(int side)
{
	if (side == 0) {
		return StringTable::LoadString("Name:Para", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 229);
	}
	if (side == 1) {
		return StringTable::LoadString("Name:BEAGLE", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 231);
	}
	if (side == 2) {
		return StringTable::LoadString("Name:GTGCAN", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 233);
	}
	if (side == 3) {
		return StringTable::LoadString("Name:TNKD", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 235);
	}
	if (side == 4) {
		return StringTable::LoadString("Name:SNIPE", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 237);
	}
	if (side == 5) {
		return StringTable::LoadString("Name:DTRUCK", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 239);
	}
	if (side == 6) {
		return StringTable::LoadString("Name:DESO", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 241);
	}
	if (side == 7) {
		return StringTable::LoadString("Name:TERROR", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 243);
	}
	if (side == 8) {
		return StringTable::LoadString("Name:TTNK", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 245);
	}
	return NULL;
}

/// <summary>
/// The briefing of a country from the string table.
/// </summary>
inline wchar_t const * Briefing(int side)
{
	if (side == 0) {
		return StringTable::LoadString("LoadBrief:USA", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 278);
	}
	if (side == 1) {
		return StringTable::LoadString("LoadBrief:Korea", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 280);
	}
	if (side == 2) {
		return StringTable::LoadString("LoadBrief:French", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 282);
	}
	if (side == 3) {
		return StringTable::LoadString("LoadBrief:Germans", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 284);
	}
	if (side == 4) {
		return StringTable::LoadString("LoadBrief:British", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 286);
	}
	if (side == 5) {
		return StringTable::LoadString("LoadBrief:Lybia", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 288);
	}
	if (side == 6) {
		return StringTable::LoadString("LoadBrief:Iraq", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 290);
	}
	if (side == 7) {
		return StringTable::LoadString("LoadBrief:Cuba", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 292);
	}
	if (side == 8) {
		return StringTable::LoadString("LoadBrief:Russia", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 294);
	}
	return NULL;
}

/// <summary>
/// Draws the whole screen into a surface of its own: in a campaign the title bar, the picture and the bar
/// below it with the scenario's texts; in a multiplayer game the picture of the player's country with its
/// name, the name of its unit, a briefing and the word for loading.
/// </summary>
void LoadProgressManager::Draw(void)
{
	wchar_t text[2048];
	wchar_t unit_name[128];
	char filename[32];

	bool low_res = VideoModeWidth == ScreenWidths[0];
	ProgressSurface = new BSurface(VideoModeWidth, VideoModeHeight, 2);
	if (ProgressSurface == NULL) {
		return;
	}
	Rect screen(0, 0, 0, 0);
	int background = DSurface::Build_Hicolor_Pixel(BlackColor);
	screen.Width = VideoModeWidth;
	screen.Height = VideoModeHeight;
	ProgressSurface->Fill_Rect(screen, screen, background);

	if (Session.Type == GAME_NORMAL) {
		char const * name = low_res ? Scen->LS640BkgdName : Scen->LS800BkgdName;
		if (*name != '\0') {
			LoadMessage = NULL;
			if (Scen->LSLoadMessage[0] != '\0') {
				LoadMessage = StringTable::LoadString(Scen->LSLoadMessage, NULL, "D:\\Ra2\\LdPrgMgr.cpp", 591);
			}
			LoadBriefing = NULL;
			if (Scen->LSBrief[0] != '\0') {
				LoadBriefing = StringTable::LoadString(Scen->LSBrief, NULL, "D:\\Ra2\\LdPrgMgr.cpp", 595);
			}
			int side = Progress.Get_Side();
			char const * title_name = Get_Title_Bar_File(VideoModeWidth, VideoModeHeight, side);
			char const * bar_name = Get_Splash_Bar_File(VideoModeWidth, VideoModeHeight, side);
			TitleBarShape = Load_Alloc_Data(title_name, TitleBarShapeLoaded);
			TextBarShape = Load_Alloc_Data(bar_name, TextBarShapeLoaded);
			PictureShape = Load_Alloc_Data(name, PictureShapeLoaded);
			ConvertClass * title_drawer = Get_Load_Screen_Drawer();
			ConvertClass * picture_drawer = Get_Load_Screen_Side_Drawer();
			ConvertClass * bar_drawer = Get_Splash_Bar_Drawer();

			Point2D point(TitleBarRect.X, TitleBarRect.Y);
			if (TitleBarShape != NULL) {
				Draw_Shape(*ProgressSurface, *title_drawer, (ShapeSet const *)TitleBarShape, 0, point, screen, SHAPE_WIN_REL, NULL, 0, ZGRAD_GROUND, 1000, NULL, 0, Point2D(0, 0));
			} else {
				ProgressSurface->Fill_Rect(screen, TitleBarRect, background);
			}
			point.Y = PictureRect.Y;
			Draw_Shape(*ProgressSurface, *picture_drawer, (ShapeSet const *)PictureShape, 0, point, screen, SHAPE_WIN_REL, NULL, 0, ZGRAD_GROUND, 1000, NULL, 0, Point2D(0, 0));
			point.Y = TextBarRect.Y;
			if (TextBarShape != NULL) {
				Draw_Shape(*ProgressSurface, *bar_drawer, (ShapeSet const *)TextBarShape, 0, point, screen, SHAPE_WIN_REL, NULL, 0, ZGRAD_GROUND, 1000, NULL, 0, Point2D(0, 0));
			} else {
				ProgressSurface->Fill_Rect(screen, TextBarRect, background);
			}

			point.X = TitleBarRect.X + 10;
			point.Y = TitleBarRect.Y + 10;
			ColorScheme * scheme = Progress.Side_Scheme();
			if (LoadMessage != NULL) {
				Fancy_Text_Print(LoadMessage, *ProgressSurface, screen, point, scheme, 0, TextPrintType(TPF_EFNT | TPF_NOSHADOW));
			}
			if (LoadBriefing != NULL) {
				if (low_res) {
					point.X = Scen->LS640BriefLocX + PictureRect.X;
					point.Y = Scen->LS640BriefLocY + PictureRect.Y;
				} else {
					point.X = Scen->LS800BriefLocX + PictureRect.X;
					point.Y = Scen->LS800BriefLocY + PictureRect.Y;
				}
				wcscpy(text, LoadBriefing);
				RGBClass pixel_color = scheme->HSV;
				int pixel = DSurface::Build_Hicolor_Pixel(pixel_color);
				int width = 0;
				int height = 0;
				BitFont::Instance->Text_Dimensions(text, &width, &height, 400);
				Rect area(point.X, point.Y, width, height);
				Shade_Text_Area(text, ProgressSurface, point.X, point.Y, 400, 0, false);
				Draw_Area_Text(text, ProgressSurface, area, pixel, 0);
			}
		}
		return;
	}

	{
		Point2D point(0, 0);
		int side = Session.Players[0]->Get_Country();
		char const * suffix = VideoModeWidth == ScreenWidths[0] ? Suffix640 : Suffix800;
		Loading_Filename(filename, side, suffix);
		PictureShape = Load_Alloc_Data(filename, PictureShapeLoaded);
		ConvertClass * drawer = Get_MP_Load_Drawer();
		Draw_Shape(*ProgressSurface, *drawer, (ShapeSet const *)PictureShape, 0, point, screen, SHAPE_WIN_REL, NULL, 0, ZGRAD_GROUND, 1000, NULL, 0, Point2D(0, 0));

		ColorScheme * scheme = Progress.Text_Scheme();
		RGBClass pixel_color = scheme->HSV;
		int pixel = DSurface::Build_Hicolor_Pixel(pixel_color);

		wchar_t const * country_name = Country_Name(side);
		Rect area;
		Name_Area(area);
		Shade_Text_Area(country_name, ProgressSurface, point.X, point.Y, area.Width, 2, false);
		Draw_Area_Text(country_name, ProgressSurface, area, pixel, 2);

		if (!Is_Observer()) {
			wcscpy(unit_name, Unit_Name(side));
			Upper_Case(unit_name);
			area = Unit_Area();
			int unit_pixel = DSurface::Build_Hicolor_Pixel(BlackColor);
			Draw_Area_Text(unit_name, ProgressSurface, area, unit_pixel, 0);

			wchar_t const * brief = Briefing(side);
			area = Get_Briefing_Area();
			Shade_Text_Area(brief, ProgressSurface, point.X, point.Y, area.Width, 0, true);
			Draw_Area_Text(brief, ProgressSurface, area, pixel, 0);
		}

		wchar_t const * loading = StringTable::LoadString("GUI:LoadingEx", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 749);
		area = Get_Loading_Text_Area();
		Shade_Text_Area(loading, ProgressSurface, point.X, point.Y, area.Width, 0, false);
		Draw_Area_Text(loading, ProgressSurface, area, pixel, 0);
	}
}
#endif


/// <summary>
/// The area of the briefing text, 318 by 104 at 640 wide and 398 by 130 otherwise, centred on larger screens.
/// </summary>
Rect __fastcall Get_Briefing_Area(void)
{
	Rect area;
	if (VideoModeWidth == ScreenWidths[0]) {
		area = Rect(16, 126, 318, 104);
	} else {
		area = Rect(20, 158, 398, 130);
	}
	if (VideoModeWidth >= ScreenWidths[1]) {
		area.X += (VideoModeWidth - ScreenWidths[1]) / 2;
		area.Y += (VideoModeHeight - ScreenHeights[1]) / 2;
	}
	return area;
}


/// <summary>
/// The area of the word for loading, 200 by 20, centred on larger screens.
/// </summary>
Rect __fastcall Get_Loading_Text_Area(void)
{
	Rect area;
	if (VideoModeWidth == ScreenWidths[0]) {
		area = Rect(16, 235, 200, 20);
	} else {
		area = Rect(20, 300, 200, 20);
	}
	if (VideoModeWidth >= ScreenWidths[1]) {
		area.X += (VideoModeWidth - ScreenWidths[1]) / 2;
		area.Y += (VideoModeHeight - ScreenHeights[1]) / 2;
	}
	return area;
}


/// <summary>
/// Darkens the area behind a text: the area the text takes from a corner and a width (flags 1 centres it, 2
/// aligns it to the right, fill makes it as wide as the width), moved in by four pixels.
/// </summary>
void __fastcall Shade_Text_Area(wchar_t const * text, Surface * surface, int x, int y, int width, int flags, bool fill)
{
	int text_width = 0;
	int text_height = 0;
	BitFont::Instance->Text_Dimensions(text, &text_width, &text_height, width);
	if (fill) {
		text_width = width;
	}
	Rect area;
	area = Rect(x, y, text_width, text_height);
	if (flags & 1) {
		area.X = (width - text_width) / 2 + x;
	} else if (flags & 2) {
		area.X = x - text_width + width;
	}
	Inflate_Rect(area, -4);
	Blend_Rect(&area, surface, 0, 159);
}


/// <summary>
/// Draws a text in an area in a pixel color through the game font.
/// </summary>
void __fastcall Draw_Area_Text(wchar_t const * text, Surface * surface, Rect const & area, unsigned short color, int flags)
{
	RECT rect;
	rect.left = area.X;
	rect.top = area.Y;
	rect.right = area.X + area.Width;
	rect.bottom = area.Y + area.Height;
	RGBClass rgb = DSurface::Deconstruct_Hicolor_Pixel(color);
	OwnerDraw::Draw_Text_In_Rect_Ex(surface, text, &rect, BitFont::Instance, rgb.Get_Red() | (rgb.Get_Green() << 8) | (rgb.Get_Blue() << 16), flags, 12, 0, 0, 0);
}


/// <summary>
/// Copies the drawn screen onto a surface.
/// </summary>
void LoadProgressManager::Copy_Surface(Surface * target)
{
	if (ProgressSurface != NULL) {
		target->Blit_From(*ProgressSurface, false, true);
	}
}

/// <summary>
/// Frees the shapes the screen loaded itself, the palettes, LOAD.MIX and the surface.
/// </summary>
void LoadProgressManager::Release(void)
{
	if (TitleBarShapeLoaded && TitleBarShape != NULL) {
		operator delete(TitleBarShape);
	}
	TitleBarShape = NULL;
	if (PictureShapeLoaded && PictureShape != NULL) {
		operator delete(PictureShape);
	}
	PictureShape = NULL;
	if (TextBarShapeLoaded && TextBarShape != NULL) {
		operator delete(TextBarShape);
	}
	TextBarShape = NULL;
	Free_Loading_Screen_Palettes();
	Free_MP_Load_Palette();
	if (LoadMix != NULL) {
		delete LoadMix;
		LoadMix = NULL;
	}
	delete ProgressSurface;
	ProgressSurface = NULL;
}


/// <summary>
/// Answers the progress screen's redraw request (message 0x11AE) by copying the screen onto the
/// hidden surface.
/// </summary>
bool LoadProgressManager::Unknown_Virtual_0(int message, int data)
{
	if (message == 0x11AE) {
		Copy_Surface(HiddenSurface);
		return true;
	}
	return false;
}
