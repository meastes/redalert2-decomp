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
void __fastcall Shade_Text_Area(wchar_t const * text, Surface * surface, Point2D point, int width, int flags, bool fill);
void __fastcall Draw_Area_Text(wchar_t const * text, Surface * surface, Rect const & area, unsigned short color, int flags);
Point2D __fastcall Copy_Point(Rect const * source); // RA2: 0x006F1AE0 (txtlabel2.cpp)
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
	int width = low_res ? LowResWidth : MedResWidth;
	if (Session.Type == GAME_NORMAL) {
		int height = (low_res ? LowResHeight : MedResHeight) - 80;
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
		int height = low_res ? LowResHeight : MedResHeight;
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
		if (VideoModeWidth == LowResWidth) {
			p.X += 0x54;
		} else {
			p.X += 0xA4;
		}
		p.Y = TextBarRect.Y + 7;
	} else {
		p = Copy_Point(&PictureRect);
		if (VideoModeWidth == LowResWidth) {
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
		width = VideoModeWidth == LowResWidth ? 326 : 406;
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
#ifdef RA2_NON_MATCHING // not exact: 966 bytes differ against the image-derived bound 00535bf0 (3,978 body bytes, 6 padding) (was 1828; ra2-decomp e27159e8's matched Yuri's Revenge text carried over: the file helpers, the name and unit rectangles built in place and copied to one box, and the text origin passed by value; all 85 calls, the frame and the stack probe match; what is left is one register exchange, zero and the pixel in ebx against the country in ebp, 167 instructions; with the campaign picture name chosen by if/else every instruction is the image's except that Get_Briefing_Area is expanded, 57 instructions; lane F74 measured the same forms on its own and found that replacing one of the text colour's two later uses leaves 11 lines, private/dossiers/fable/patches/f74_ldprgmgr_draw_twin_forms_by_value_point.patch)
inline void __fastcall Shade_Text_Area(wchar_t const * text, Surface * surface, Point2D point, int width, int flags, bool fill); // RA2: inline, as ra2-decomp declares it; with the draft closed nothing else would emit it
ConvertClass * __cdecl Get_Load_Screen_Drawer(void); // RA2: 0x006F2450
ConvertClass * __cdecl Get_Load_Screen_Side_Drawer(void); // RA2: 0x006F2470
ConvertClass * __cdecl Get_Splash_Bar_Drawer(void); // RA2: 0x006F2490
ConvertClass * __cdecl Get_MP_Load_Drawer(void); // RA2: 0x006F25B0
char const * __fastcall Get_Title_Bar_File(int width, int, int); // RA2: 0x006F2400
char const * __fastcall Get_Splash_Bar_File(int width, int, int); // RA2: 0x006F2420

/// <summary>
/// Moves an area laid out for 800 by 600 to the middle of a larger screen.
/// </summary>
inline void __fastcall Center_On_Screen(Rect * rect)
{
	int width = VideoModeWidth;
	int box_width = MedResWidth;
	if (width >= box_width) {
		int dy = (VideoModeHeight - MedResHeight) / 2;
		int dx = (width - box_width) / 2;
		rect->X += dx;
		rect->Y += dy;
	}
}

static inline bool Is_Observer(void)
{
	return (Session.Type == GAME_IPX || Session.Type == GAME_INTERNET) && Session.Players[0]->Slot.SpectatorFlag == -1;
}

/// <summary>
/// The name of the picture file of a country, or of the observer, for the screen size.
/// </summary>
static void Background_Name(char * name, int country)
{
	char const * res = (VideoModeWidth == LowResWidth) ? Suffix640 : Suffix800;
	if (Is_Observer()) {
		sprintf(name, ObserverShapeName, res);
	} else if (country == 0) {
		sprintf(name, USShapeName, res);
	} else if (country == 1) {
		sprintf(name, KoreaShapeName, res);
	} else if (country == 2) {
		sprintf(name, FranceShapeName, res);
	} else if (country == 3) {
		sprintf(name, GermanyShapeName, res);
	} else if (country == 4) {
		sprintf(name, UKShapeName, res);
	} else if (country == 5) {
		sprintf(name, LibyaShapeName, res);
	} else if (country == 6) {
		sprintf(name, IraqShapeName, res);
	} else if (country == 7) {
		sprintf(name, CubaShapeName, res);
	} else if (country == 8) {
		sprintf(name, RussiaShapeName, res);
	}
}

/// <summary>
/// The name of a country, or of the observer, from the string table.
/// </summary>
static wchar_t const * Country_Name(int country)
{
	wchar_t const * name;
	if (Is_Observer()) {
		name = StringTable::LoadString("Name:Observer", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 162);
	} else if (country == 0) {
		name = StringTable::LoadString("Name:Americans", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 167);
	} else if (country == 1) {
		name = StringTable::LoadString("Name:Alliance", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 169);
	} else if (country == 2) {
		name = StringTable::LoadString("Name:French", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 171);
	} else if (country == 3) {
		name = StringTable::LoadString("Name:Germans", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 173);
	} else if (country == 4) {
		name = StringTable::LoadString("Name:British", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 175);
	} else if (country == 5) {
		name = StringTable::LoadString("Name:Africans", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 177);
	} else if (country == 6) {
		name = StringTable::LoadString("Name:Arabs", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 179);
	} else if (country == 7) {
		name = StringTable::LoadString("Name:Confederation", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 181);
	} else if (country == 8) {
		name = StringTable::LoadString("Name:Russians", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 183);
	} else {
		name = NULL;
	}
	return(name);
}

/// <summary>
/// The name of a country's unit from the string table.
/// </summary>
static wchar_t const * Country_Unit_Name(int country)
{
	wchar_t const * name;
	if (country == 0) {
		name = StringTable::LoadString("Name:Para", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 229);
	} else if (country == 1) {
		name = StringTable::LoadString("Name:BEAGLE", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 231);
	} else if (country == 2) {
		name = StringTable::LoadString("Name:GTGCAN", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 233);
	} else if (country == 3) {
		name = StringTable::LoadString("Name:TNKD", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 235);
	} else if (country == 4) {
		name = StringTable::LoadString("Name:SNIPE", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 237);
	} else if (country == 5) {
		name = StringTable::LoadString("Name:DTRUCK", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 239);
	} else if (country == 6) {
		name = StringTable::LoadString("Name:DESO", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 241);
	} else if (country == 7) {
		name = StringTable::LoadString("Name:TERROR", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 243);
	} else if (country == 8) {
		name = StringTable::LoadString("Name:TTNK", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 245);
	} else {
		name = NULL;
	}
	return(name);
}

/// <summary>
/// The briefing of a country from the string table.
/// </summary>
static wchar_t const * Country_Brief(int country)
{
	wchar_t const * brief;
	if (country == 0) {
		brief = StringTable::LoadString("LoadBrief:USA", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 278);
	} else if (country == 1) {
		brief = StringTable::LoadString("LoadBrief:Korea", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 280);
	} else if (country == 2) {
		brief = StringTable::LoadString("LoadBrief:French", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 282);
	} else if (country == 3) {
		brief = StringTable::LoadString("LoadBrief:Germans", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 284);
	} else if (country == 4) {
		brief = StringTable::LoadString("LoadBrief:British", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 286);
	} else if (country == 5) {
		brief = StringTable::LoadString("LoadBrief:Lybia", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 288);
	} else if (country == 6) {
		brief = StringTable::LoadString("LoadBrief:Iraq", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 290);
	} else if (country == 7) {
		brief = StringTable::LoadString("LoadBrief:Cuba", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 292);
	} else if (country == 8) {
		brief = StringTable::LoadString("LoadBrief:Russia", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 294);
	} else {
		brief = NULL;
	}
	return(brief);
}

/// <summary>
/// Changes the lower case letters of a text to capitals, those of the Latin-1 letters from 0xE0 to 0xFE too.
/// </summary>
static void Upper_Case(wchar_t * text)
{
	for (; *text; text++) {
		if ((*text >= 'a' && *text <= 'z') || (*text >= 0xe0 && *text <= 0xfe)) {
			*text -= 0x20;
		}
	}
}

/// <summary>
/// Draws the whole screen into a surface of its own: in a campaign the title bar, the picture and the bar
/// below it with the scenario's texts; in a multiplayer game the picture of the player's country with its
/// name, the name of its unit, a briefing and the word for loading.
/// </summary>
void LoadProgressManager::Draw(void)
{
	bool low_res = (VideoModeWidth == LowResWidth);
	ProgressSurface = new BSurface(VideoModeWidth, VideoModeHeight, 2);
	if (ProgressSurface == NULL) {
		return;
	}
	int color = DSurface::Build_Hicolor_Pixel(BlackColor);
	Rect rect(0, 0, 0, 0);
	rect.Width = VideoModeWidth;
	rect.Height = VideoModeHeight;
	ProgressSurface->Fill_Rect(rect, rect, color);

	if (Session.Type == GAME_NORMAL) {
		char const * background;
		if (low_res) {
			background = Scen->LS640BkgdName;
		} else {
			background = Scen->LS800BkgdName;
		}
		if (*background == '\0') {
			return;
		}
		LoadMessage = NULL;
		if (*Scen->LSLoadMessage != '\0') {
			LoadMessage = StringTable::LoadString(Scen->LSLoadMessage, NULL, "D:\\Ra2\\LdPrgMgr.cpp", 591);
		}
		LoadBriefing = NULL;
		if (*Scen->LSBrief != '\0') {
			LoadBriefing = StringTable::LoadString(Scen->LSBrief, NULL, "D:\\Ra2\\LdPrgMgr.cpp", 595);
		}
		int side = Progress.Get_Side();
		char const * titlebar = Get_Title_Bar_File(VideoModeWidth, VideoModeHeight, side);
		char const * loadbar = Get_Splash_Bar_File(VideoModeWidth, VideoModeHeight, side);
		TitleBarShape = (ShapeSet *)Load_Alloc_Data(titlebar, TitleBarShapeLoaded);
		TextBarShape = (ShapeSet *)Load_Alloc_Data(loadbar, TextBarShapeLoaded);
		PictureShape = (ShapeSet *)Load_Alloc_Data(background, PictureShapeLoaded);
		ConvertClass * titlebar_drawer = Get_Load_Screen_Drawer();
		ConvertClass * screen_drawer = Get_Load_Screen_Side_Drawer();
		ConvertClass * loadbar_drawer = Get_Splash_Bar_Drawer();

		Point2D pt(TitleBarRect.X, TitleBarRect.Y);
		if (TitleBarShape != NULL) {
			Draw_Shape(*ProgressSurface, *titlebar_drawer, TitleBarShape, 0, pt, rect, SHAPE_WIN_REL);
		} else {
			ProgressSurface->Fill_Rect(rect, TitleBarRect, color);
		}
		pt.Y = PictureRect.Y;
		Draw_Shape(*ProgressSurface, *screen_drawer, PictureShape, 0, pt, rect, SHAPE_WIN_REL);
		pt.Y = TextBarRect.Y;
		if (TextBarShape != NULL) {
			Draw_Shape(*ProgressSurface, *loadbar_drawer, TextBarShape, 0, pt, rect, SHAPE_WIN_REL);
		} else {
			ProgressSurface->Fill_Rect(rect, TextBarRect, color);
		}

		pt.X = TitleBarRect.X + 10;
		pt.Y = TitleBarRect.Y + 10;
		ColorScheme * scheme = Progress.Side_Scheme();
		if (LoadMessage != NULL) {
			Fancy_Text_Print(LoadMessage, *ProgressSurface, rect, pt, scheme, 0, TextPrintType(TPF_EFNT | TPF_NOSHADOW));
		}
		if (LoadBriefing != NULL) {
			if (low_res) {
				pt.X = Scen->LS640BriefLocX + PictureRect.X;
				pt.Y = Scen->LS640BriefLocY + PictureRect.Y;
			} else {
				pt.X = Scen->LS800BriefLocX + PictureRect.X;
				pt.Y = Scen->LS800BriefLocY + PictureRect.Y;
			}
			wchar_t buffer[2048];
			wcscpy(buffer, LoadBriefing);
			RGBClass rgb = scheme->HSV;
			int text_color = DSurface::Build_Hicolor_Pixel(rgb);
			int width = 0;
			int height = 0;
			BitFont::Instance->Text_Dimensions(buffer, &width, &height, 400);
			Rect text_rect;
			text_rect = Rect(pt.X, pt.Y, width, height);
			Shade_Text_Area(buffer, ProgressSurface, pt, 400, 0, false);
			Draw_Area_Text(buffer, ProgressSurface, text_rect, text_color, 0);
		}
		return;
	} else {
		int country = Session.Players[0]->Get_Country();
		char name[32];
		Background_Name(name, country);
		PictureShape = (ShapeSet *)Load_Alloc_Data(name, PictureShapeLoaded);
		ConvertClass * drawer = Get_MP_Load_Drawer();
		Point2D origin = Copy_Point(&PictureRect);
		Draw_Shape(*ProgressSurface, *drawer, PictureShape, 0, origin, rect, SHAPE_WIN_REL);
		ColorScheme * scheme = Progress.Text_Scheme();
		RGBClass rgb = scheme->HSV;
		int text_color = DSurface::Build_Hicolor_Pixel(rgb);

		wchar_t const * country_name = Country_Name(country);
		Rect name_rect;
		if (VideoModeWidth == LowResWidth) {
			name_rect.X = 385;
			name_rect.Y = 436;
		} else {
			name_rect.X = 533;
			name_rect.Y = 545;
		}
		name_rect.Width = 200;
		name_rect.Height = 20;
		Center_On_Screen(&name_rect);
		Rect box(name_rect);
		origin = Copy_Point(&box);
		Shade_Text_Area(country_name, ProgressSurface, origin, box.Width, 2, false);
		Draw_Area_Text(country_name, ProgressSurface, box, text_color, 2);

		if (!Is_Observer()) {
			wchar_t unit_name[128];
			wcscpy(unit_name, Country_Unit_Name(country));
			Upper_Case(unit_name);
			Rect unit_rect;
			if (VideoModeWidth == LowResWidth) {
				unit_rect.X = 16;
				unit_rect.Y = 72;
			} else {
				unit_rect.X = 20;
				unit_rect.Y = 90;
			}
			unit_rect.Width = 200;
			unit_rect.Height = 20;
			Center_On_Screen(&unit_rect);
			box = unit_rect;
			int black = DSurface::Build_Hicolor_Pixel(BlackColor);
			Draw_Area_Text(unit_name, ProgressSurface, box, black, 0);

			wchar_t const * brief = Country_Brief(country);
			box = Get_Briefing_Area();
			origin = Copy_Point(&box);
			Shade_Text_Area(brief, ProgressSurface, origin, box.Width, 0, true);
			Draw_Area_Text(brief, ProgressSurface, box, text_color, 0);
		}

		wchar_t const * loading = StringTable::LoadString("GUI:LoadingEx", NULL, "D:\\Ra2\\LdPrgMgr.cpp", 749);
		box = Get_Loading_Text_Area();
		origin = Copy_Point(&box);
		Shade_Text_Area(loading, ProgressSurface, origin, box.Width, 0, false);
		Draw_Area_Text(loading, ProgressSurface, box, text_color, 0);
	}
}
#endif


/// <summary>
/// The area of the briefing text, 318 by 104 at 640 wide and 398 by 130 otherwise, centred on larger screens.
/// </summary>
Rect __fastcall Get_Briefing_Area(void)
{
	Rect area;
	if (VideoModeWidth == LowResWidth) {
		area = Rect(16, 126, 318, 104);
	} else {
		area = Rect(20, 158, 398, 130);
	}
	if (VideoModeWidth >= MedResWidth) {
		area.X += (VideoModeWidth - MedResWidth) / 2;
		area.Y += (VideoModeHeight - MedResHeight) / 2;
	}
	return area;
}


/// <summary>
/// The area of the word for loading, 200 by 20, centred on larger screens.
/// </summary>
Rect __fastcall Get_Loading_Text_Area(void)
{
	Rect area;
	if (VideoModeWidth == LowResWidth) {
		area = Rect(16, 235, 200, 20);
	} else {
		area = Rect(20, 300, 200, 20);
	}
	if (VideoModeWidth >= MedResWidth) {
		area.X += (VideoModeWidth - MedResWidth) / 2;
		area.Y += (VideoModeHeight - MedResHeight) / 2;
	}
	return area;
}


/// <summary>
/// Darkens the area behind a text: the area the text takes from a corner and a width (flags 1 centres it, 2
/// aligns it to the right, fill makes it as wide as the width), moved in by four pixels. The corner is passed
/// by value, as ra2-decomp's text has it: the image copies the point at each call.
/// </summary>
void __fastcall Shade_Text_Area(wchar_t const * text, Surface * surface, Point2D point, int width, int flags, bool fill)
{
	int text_width = 0;
	int text_height = 0;
	BitFont::Instance->Text_Dimensions(text, &text_width, &text_height, width);
	if (fill) {
		text_width = width;
	}
	Rect area;
	area = Rect(point.X, point.Y, text_width, text_height);
	if (flags & 1) {
		area.X = (width - text_width) / 2 + point.X;
	} else if (flags & 2) {
		area.X = point.X - text_width + width;
	}
	Inflate_Rect(area, -4);
	Blend_Rect(&area, surface, 0, 159);
}


/// <summary>
/// Draws a text in an area in a pixel color through the game font.
/// </summary>
void __fastcall Draw_Area_Text(wchar_t const * text, Surface * surface, Rect const & area, unsigned short color, int flags)
{
	BitFont * font = BitFont::Instance;
	RECT rect;
	rect.left = area.X;
	rect.top = area.Y;
	rect.right = area.X + area.Width;
	rect.bottom = area.Y + area.Height;
	RGBClass rgb = DSurface::Deconstruct_Hicolor_Pixel(color);
	OwnerDraw::Draw_Text_In_Rect_Ex(surface, text, &rect, font, rgb.Get_Red() | (rgb.Get_Green() << 8) | (rgb.Get_Blue() << 16), flags, 12, 0, 0, 0);
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
