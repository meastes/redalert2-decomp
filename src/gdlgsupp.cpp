/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/

/*
**	RA2: support for the multiplayer setup dialogs: the controls of each player slot (flag, ping,
**	side and color combo boxes) and the lists those combo boxes offer. TibSun has no such file;
**	the code is the executable's from 0x004D0EE0 (it carries the file name D:\Ra2\GDlgSupp.cpp).
*/

#include "always.h" // RA2: the includes this file needs
#include "rect.h" // RA2: the header statics, in the order of the unit's initializer table
#include "geometry.h"
#include <windows.h>
#include "vector.h"
#include <stddef.h>
#undef LoadString
#include "stringtable.h"
#include "srfcache.h"
#include "globals.h"
#include "houstype.h"
#include "session.h"
#include "netshare.h"
#include "gdlgsupp.h"
#include "ownrdraw.h"

int __fastcall Combo_Selected_Data(HWND dialog, int id, int index); // RA2: 0x005C99D0 (netshare.cpp)
namespace OwnerDraw { void __fastcall Set_Control_Image(HWND control, Surface * image); } // RA2: 0x005E1F30, as ownrdraw.cpp


/// <summary>The pictures of the flag controls: random, observer, then the nine countries.</summary>
char const * FlagImages[11] = { // RA2: 0x007F86B0
	"rani.pcx", "obsi.pcx", "usai.pcx", "japi.pcx", "frai.pcx", "geri.pcx", "gbri.pcx", "djbi.pcx", "arbi.pcx", "lati.pcx", "rusi.pcx"
};

/// <summary>The pictures of the ping controls: good, middling and bad.</summary>
char const * PingImages[3] = { // RA2: 0x007F86DC
	"pingg.pcx", "pingy.pcx", "pingr.pcx"
};

/// <summary>One of the nine colors a player may take: its abbreviation, its value and the player that has it.</summary>
struct ColorEntry { // RA2: 12 bytes, as the table at 0x008660D8 steps
	wchar_t const * Name; // RA2: +0x00 the abbreviation, set by Init_Color_Entries
	COLORREF Color; // RA2: +0x04
	int Player; // RA2: +0x08 the player slot using it, -1 when free
};
ColorEntry ColorEntries[9]; // RA2: 0x008660D8

/// <summary>The id of the control that shows the flag of a player's side, or -1.</summary>
int __fastcall Player_Control_ID(int player) // RA2: 0x004D0EE0, not in TibSun
{
	if (player == 0) { // RA2: the flag control ids of players 0 to 7 are 0x6DA to 0x6E1
		return 0x6DA;
	}
	if (player == 1) {
		return 0x6DB;
	}
	if (player == 2) {
		return 0x6DC;
	}
	if (player == 3) {
		return 0x6DD;
	}
	if (player == 4) {
		return 0x6DE;
	}
	if (player == 5) {
		return 0x6DF;
	}
	if (player == 6) {
		return 0x6E0;
	}
	if (player == 7) {
		return 0x6E1;
	}
	return -1;
}

/// <summary>The player whose flag control has this id, or -1.</summary>
int __fastcall Control_Player(int id) // RA2: 0x004D0F40, the inverse of Player_Control_ID; not in TibSun
{
	if (id == 0x6DA) { // RA2: the inverse table: control 0x6DA is player 0 and so on
		return(0);
	}
	if (id == 0x6DB) {
		return(1);
	}
	if (id == 0x6DC) {
		return 2;
	}
	if (id == 0x6DD) {
		return 3;
	}
	if (id == 0x6DE) {
		return 4;
	}
	if (id == 0x6DF) {
		return 5;
	}
	if (id == 0x6E0) {
		return 6;
	}
	if (id == 0x6E1) {
		return 7;
	}
	return -1;
}

/// <summary>The picture of a ping level (0 to 2), or NULL.</summary>
Surface * __fastcall Ping_Image(int ping) // RA2: 0x004D0FB0, not in TibSun
{
	Surface * image = NULL; // RA2: the ping pictures: 0 good (green), 1 middling (yellow), 2 bad (red)
	if (ping == 0) {
		image = SurfaceCache.GetSurface(PingImages[0], NULL);
	} else if (ping == 1) {
		image = SurfaceCache.GetSurface(PingImages[1], NULL);
	} else if (ping == 2) {
		image = SurfaceCache.GetSurface(PingImages[2], NULL);
	}
	return image;
}

/// <summary>The id of the control that shows the ping of a player, or -1.</summary>
int __fastcall Player_Ping_Control_ID(int player) // RA2: 0x004D1000, not in TibSun
{
	if (player == 0) { // RA2: the ping control ids of players 0 to 7 are 0x754 to 0x75B
		return 0x754;
	}
	if (player == 1) {
		return 0x755;
	}
	if (player == 2) {
		return 0x756;
	}
	if (player == 3) {
		return 0x757;
	}
	if (player == 4) {
		return 0x758;
	}
	if (player == 5) {
		return 0x759;
	}
	if (player == 6) {
		return 0x75A;
	}
	if (player == 7) {
		return 0x75B;
	}
	return -1;
}

/// <summary>Shows the ping level of a player in its control.</summary>
void __fastcall Set_Player_Ping(HWND dialog, int player, int ping) // RA2: 0x004D1060, not in TibSun
{
	HWND control = GetDlgItem(dialog, Player_Ping_Control_ID(player)); // RA2: the executable skips the picture only when GetDlgItem returns -1, not NULL
	Surface * image = Ping_Image(ping);
	if (control != (HWND)-1) {
		OwnerDraw::Set_Control_Image(control, image);
	}
}

/// <summary>The flag picture of a country (-2 random, -3 observer), or NULL.</summary>
Surface * __fastcall Country_Flag(int country) // RA2: 0x004D1120, not in TibSun
{
	Surface * image = NULL; // RA2: country -2 is the first picture, -3 the second, 0 to 8 the third to the eleventh
	if (country == -2) {
		image = SurfaceCache.GetSurface(FlagImages[0], NULL);
	} else if (country == 0) {
		image = SurfaceCache.GetSurface(FlagImages[2], NULL);
	} else if (country == 1) {
		image = SurfaceCache.GetSurface(FlagImages[3], NULL);
	} else if (country == 2) {
		image = SurfaceCache.GetSurface(FlagImages[4], NULL);
	} else if (country == 3) {
		image = SurfaceCache.GetSurface(FlagImages[5], NULL);
	} else if (country == 4) {
		image = SurfaceCache.GetSurface(FlagImages[6], NULL);
	} else if (country == 5) {
		image = SurfaceCache.GetSurface(FlagImages[7], NULL);
	} else if (country == 6) {
		image = SurfaceCache.GetSurface(FlagImages[8], NULL);
	} else if (country == 7) {
		image = SurfaceCache.GetSurface(FlagImages[9], NULL);
	} else if (country == 8) {
		image = SurfaceCache.GetSurface(FlagImages[10], NULL);
	} else if (country == -3) {
		image = SurfaceCache.GetSurface(FlagImages[1], NULL);
	}
	return image;
}

/// <summary>Shows the flag of the side a player's side combo box has selected.</summary>
void __fastcall Show_Player_Flag(HWND dialog, int player) // RA2: 0x004D1230, not in TibSun
{
	HWND control = GetDlgItem(dialog, Player_Control_ID(player)); // RA2: the flag of the side the player's side combo box shows
	OwnerDraw::Set_Control_Image(control, Country_Flag(Side_Combo_Data(dialog, Player_Side_Combo_ID(player), -1)));
}

/// <summary>The id of the side combo box of a player, or -1.</summary>
int __fastcall Player_Side_Combo_ID(int player) // RA2: 0x004D1350, not in TibSun
{
	if (player == 0) { // RA2: the side combo ids: 0x6A1 for player 0, 0x510, 0x513, 0x51E, 0x514, 0x51F, 0x520, 0x521 for 1 to 7
		return 0x6A1;
	}
	if (player == 1) {
		return 0x510;
	}
	if (player == 2) {
		return 0x513;
	}
	if (player == 3) {
		return 0x51E;
	}
	if (player == 4) {
		return 0x514;
	}
	if (player == 5) {
		return 0x51F;
	}
	if (player == 6) {
		return 0x520;
	}
	if (player == 7) {
		return 0x521;
	}
	return -1;
}

/// <summary>The player a side combo box belongs to, or -1.</summary>
int __fastcall Side_Combo_Player(int id) // RA2: 0x004D13B0, not in TibSun
{
	if (id == 0x6A1) { // RA2: the inverse table of Player_Side_Combo_ID
		return(0);
	}
	if (id == 0x510) {
		return(1);
	}
	if (id == 0x513) {
		return 2;
	}
	if (id == 0x51E) {
		return 3;
	}
	if (id == 0x514) {
		return 4;
	}
	if (id == 0x51F) {
		return 5;
	}
	if (id == 0x520) {
		return 6;
	}
	if (id == 0x521) {
		return 7;
	}
	return -1;
}

/// <summary>The name of a side (-2 random, -3 observer, 0 to 8 the countries), or NULL.</summary>
wchar_t const * __fastcall Side_Name(int side) // RA2: 0x004D1420, not in TibSun
{
	if (side == -2) { // RA2: the string labels are the executable's, with the line numbers of GDlgSupp.cpp
		return StringTable::LoadString("STT:PlayerSideRandom", NULL, "D:\\Ra2\\GDlgSupp.cpp", 231);
	}
	if (side == -3) {
		return StringTable::LoadString("STT:PlayerSideObserver", NULL, "D:\\Ra2\\GDlgSupp.cpp", 233);
	}
	if (side == 0) {
		return StringTable::LoadString("STT:PlayerSideAmerica", NULL, "D:\\Ra2\\GDlgSupp.cpp", 235);
	}
	if (side == 1) {
		return StringTable::LoadString("STT:PlayerSideKorea", NULL, "D:\\Ra2\\GDlgSupp.cpp", 237);
	}
	if (side == 2) {
		return StringTable::LoadString("STT:PlayerSideFrance", NULL, "D:\\Ra2\\GDlgSupp.cpp", 239);
	}
	if (side == 3) {
		return StringTable::LoadString("STT:PlayerSideGermany", NULL, "D:\\Ra2\\GDlgSupp.cpp", 241);
	}
	if (side == 4) {
		return StringTable::LoadString("STT:PlayerSideBritain", NULL, "D:\\Ra2\\GDlgSupp.cpp", 243);
	}
	if (side == 5) {
		return StringTable::LoadString("STT:PlayerSideLibya", NULL, "D:\\Ra2\\GDlgSupp.cpp", 245);
	}
	if (side == 6) {
		return StringTable::LoadString("STT:PlayerSideIraq", NULL, "D:\\Ra2\\GDlgSupp.cpp", 247);
	}
	if (side == 7) {
		return StringTable::LoadString("STT:PlayerSideCuba", NULL, "D:\\Ra2\\GDlgSupp.cpp", 249);
	}
	if (side == 8) {
		return StringTable::LoadString("STT:PlayerSideRussia", NULL, "D:\\Ra2\\GDlgSupp.cpp", 251);
	}
	return(0);
}

/// <summary>Fills a side combo box with "random" and every country a player may pick, and selects "random".</summary>
void __fastcall Fill_Side_Combo(HWND dialog, int id) // RA2: 0x004D1560, not in TibSun
{
	HWND combo = GetDlgItem(dialog, id); // RA2: the combo box is hidden while it is filled and shown again if it was visible; messages 0x4DE and 0x4F1 are the owner draw combo's own
	bool hidden = ShowWindow(combo, SW_HIDE) == 0;
	SendMessageA(combo, CB_RESETCONTENT, 0, 0);
	SendMessageA(combo, 0x4DE, 0, 7);
	int item = SendMessageA(combo, 0x4C2, 0, (LPARAM)StringTable::LoadString("GUI:RandomEx", NULL, "D:\\Ra2\\GDlgSupp.cpp", 268));
	SendMessageA(combo, CB_SETITEMDATA, item, -2);
	int count = HouseTypes.Count();
	for (int index = 0; index < count; index++) {
		HouseTypeClass * type = HouseTypes[index];
		if (type->IsMultiplay) {
			wchar_t const * name = type->UIName;
			item = SendMessageA(combo, 0x4C2, 0, (LPARAM)name);
			SendMessageA(combo, CB_SETITEMDATA, item, index);
		}
	}
	SendMessageA(combo, 0x4F1, 0, 0);
	SendMessageA(combo, CB_SETCURSEL, 0, 0);
	if (!hidden) {
		ShowWindow(combo, SW_SHOW);
	}
}

/// <summary>Fills a side combo box with the observer entry only.</summary>
void __fastcall Fill_Observer_Combo(HWND dialog, int id) // RA2: 0x004D1640, not in TibSun
{
	HWND combo = GetDlgItem(dialog, id); // RA2: as Fill_Side_Combo, with the observer entry only
	bool hidden = ShowWindow(combo, SW_HIDE) == 0;
	SendMessageA(combo, CB_RESETCONTENT, 0, 0);
	SendMessageA(combo, 0x4DE, 0, 7);
	int item = SendMessageA(combo, 0x4C2, 0, (LPARAM)StringTable::LoadString("GUI:Observer", NULL, "D:\\Ra2\\GDlgSupp.cpp", 298));
	SendMessageA(combo, CB_SETITEMDATA, item, -3);
	SendMessageA(combo, CB_SETCURSEL, 0, 0);
	SendMessageA(combo, 0x4F1, 0, 1);
	if (!hidden) {
		ShowWindow(combo, SW_SHOW);
	}
}

/// <summary>Fills the side combo box of every player: the observer's has the observer entry only.</summary>
void __fastcall Fill_Side_Combos(HWND dialog) // RA2: 0x004D16D0, not in TibSun
{
	for (int player = 0; player < 8; player++) { // RA2: a slot whose entry is the player entry (lobby entry 6) gets the observer combo box
		int data = Combo_Selected_Data(dialog, player, -1);
		bool observer = false;
		if (LobbyObserverEntry != NULL && data == (int)LobbyObserverEntry) {
			observer = true;
		}
		int id = Player_Side_Combo_ID(player);
		if (observer) {
			Fill_Observer_Combo(dialog, id);
		} else {
			Fill_Side_Combo(dialog, id);
		}
	}
}

/// <summary>Selects the local player's side (or random, or observer) in the combo box of a player.</summary>
void __fastcall Select_Player_Side(HWND dialog, int player) // RA2: 0x004D1820, not in TibSun
{
	int side = Session.Side; // RA2: the random or observer choice of the second field wins over the side
	if (Session.Side2 == -2) {
		side = -2;
	} else if (Session.Side2 == -3) {
		side = -3;
	}
	Select_Side_Combo(dialog, Player_Side_Combo_ID(player), side);
}

/// <summary>Selects the item with this data in a side combo box and shows the flag of that side.</summary>
void __fastcall Select_Side_Combo(HWND dialog, int id, int side) // RA2: 0x004D1A90, not in TibSun
{
	HWND combo = GetDlgItem(dialog, id); // RA2: the executable looks for the item whose data is the side and selects it
	bool hidden = ShowWindow(combo, SW_HIDE) == 0;
	int count = SendDlgItemMessageA(dialog, id, CB_GETCOUNT, 0, 0);
	for (int item = 0; item < count; item++) {
		if (SendDlgItemMessageA(dialog, id, CB_GETITEMDATA, item, 0) == side) {
			SendDlgItemMessageA(dialog, id, CB_SETCURSEL, item, 0);
			break;
		}
	}
	if (!hidden) {
		ShowWindow(combo, SW_SHOW);
	}
	Show_Player_Flag(dialog, Side_Combo_Player(id));
}

/// <summary>The data of an item of a side combo box (the selected item for -1).</summary>
int __fastcall Side_Combo_Data(HWND dialog, int id, int index) // RA2: 0x004D1C70, not in TibSun
{
	if (index == -1) { // RA2: the selected item when the index is -1
		index = SendDlgItemMessageA(dialog, id, CB_GETCURSEL, 0, 0);
	}
	return SendDlgItemMessageA(dialog, id, CB_GETITEMDATA, index, 0);
}

/// <summary>The id of the color combo box of a player, or -1.</summary>
int __fastcall Player_Color_Combo_ID(int player) // RA2: 0x004D1CB0, not in TibSun
{
	if (player == 0) { // RA2: the color combo ids: 0x6A2 for player 0, 0x522 to 0x528 for 1 to 7
		return 0x6A2;
	}
	if (player == 1) {
		return 0x522;
	}
	if (player == 2) {
		return 0x523;
	}
	if (player == 3) {
		return 0x524;
	}
	if (player == 4) {
		return 0x525;
	}
	if (player == 5) {
		return 0x526;
	}
	if (player == 6) {
		return 0x527;
	}
	if (player == 7) {
		return 0x528;
	}
	return -1;
}

/// <summary>The player a color combo box belongs to, or -1.</summary>
int __fastcall Color_Combo_Player(int id) // RA2: 0x004D1D10, not in TibSun
{
	if (id == 0x6A2) { // RA2: the inverse table of Player_Color_Combo_ID
		return(0);
	}
	if (id == 0x522) {
		return(1);
	}
	if (id == 0x523) {
		return 2;
	}
	if (id == 0x524) {
		return 3;
	}
	if (id == 0x525) {
		return 4;
	}
	if (id == 0x526) {
		return 5;
	}
	if (id == 0x527) {
		return 6;
	}
	if (id == 0x528) {
		return 7;
	}
	return -1;
}

/// <summary>The name of a color (-2 random, 0 to 8), or NULL.</summary>
wchar_t const * __fastcall Color_Name(int color) // RA2: 0x004D1D80, not in TibSun
{
	if (color == -2) { // RA2: the color labels, -2 random and 0 to 8
		return StringTable::LoadString("STT:PlayerColorRandom", NULL, "D:\\Ra2\\GDlgSupp.cpp", 429);
	}
	if (color == 0) {
		return StringTable::LoadString("STT:PlayerColorGold", NULL, "D:\\Ra2\\GDlgSupp.cpp", 431);
	}
	if (color == 1) {
		return StringTable::LoadString("STT:PlayerColorRed", NULL, "D:\\Ra2\\GDlgSupp.cpp", 433);
	}
	if (color == 2) {
		return StringTable::LoadString("STT:PlayerColorBlue", NULL, "D:\\Ra2\\GDlgSupp.cpp", 435);
	}
	if (color == 3) {
		return StringTable::LoadString("STT:PlayerColorGreen", NULL, "D:\\Ra2\\GDlgSupp.cpp", 437);
	}
	if (color == 4) {
		return StringTable::LoadString("STT:PlayerColorOrange", NULL, "D:\\Ra2\\GDlgSupp.cpp", 439);
	}
	if (color == 5) {
		return StringTable::LoadString("STT:PlayerColorSkyBlue", NULL, "D:\\Ra2\\GDlgSupp.cpp", 441);
	}
	if (color == 6) {
		return StringTable::LoadString("STT:PlayerColorPurple", NULL, "D:\\Ra2\\GDlgSupp.cpp", 443);
	}
	if (color == 7) {
		return StringTable::LoadString("STT:PlayerColorPink", NULL, "D:\\Ra2\\GDlgSupp.cpp", 445);
	}
	if (color == 8) {
		return StringTable::LoadString("STT:PlayerColorObserver", NULL, "D:\\Ra2\\GDlgSupp.cpp", 447);
	}
	return(0);
}

/// <summary>Sets up the color table: the abbreviations, the color values, and no player has a color yet.</summary>
void __fastcall Init_Color_Entries(void) // RA2: 0x004D1EA0, not in TibSun
{
	ColorEntries[0].Name = StringTable::LoadString("GUI:GoldAbbrev", NULL, "D:\\Ra2\\GDlgSupp.cpp", 455); // RA2: the abbreviations of the nine colors; the color values come from PlayerColorTable
	ColorEntries[1].Name = StringTable::LoadString("GUI:RedAbbrev", NULL, "D:\\Ra2\\GDlgSupp.cpp", 456);
	ColorEntries[2].Name = StringTable::LoadString("GUI:BlueAbbrev", NULL, "D:\\Ra2\\GDlgSupp.cpp", 457);
	ColorEntries[3].Name = StringTable::LoadString("GUI:GreenAbbrev", NULL, "D:\\Ra2\\GDlgSupp.cpp", 458);
	ColorEntries[4].Name = StringTable::LoadString("GUI:OrangeAbbrev", NULL, "D:\\Ra2\\GDlgSupp.cpp", 459);
	ColorEntries[5].Name = StringTable::LoadString("GUI:SkyBlueAbbrev", NULL, "D:\\Ra2\\GDlgSupp.cpp", 460);
	ColorEntries[6].Name = StringTable::LoadString("GUI:PurpleAbbrev", NULL, "D:\\Ra2\\GDlgSupp.cpp", 461);
	ColorEntries[7].Name = StringTable::LoadString("GUI:PinkAbbrev", NULL, "D:\\Ra2\\GDlgSupp.cpp", 462);
	ColorEntries[8].Name = StringTable::LoadString("GUI:GrayAbbrev", NULL, "D:\\Ra2\\GDlgSupp.cpp", 463);
	for (int index = 0; index < 9; index++) {
		ColorEntries[index].Color = PlayerColorTable[index];
		ColorEntries[index].Player = -1;
	}
}

/// <summary>The color a color combo box's player has taken, or -1.</summary>
int __fastcall Color_Entry_Of_Combo(int id) // RA2: 0x004D1FC0, not in TibSun
{
	int player = Color_Combo_Player(id); // RA2: the executable searches the table for the entry whose player is the combo's player
	for (int index = 0; index < 9; index++) {
		if (ColorEntries[index].Player == player) {
			return index;
		}
	}
	return -1;
}

/// <summary>Whether a color is taken by a player.</summary>
bool __fastcall Color_Entry_Taken(int entry) // RA2: 0x004D2060, not in TibSun
{
	return ColorEntries[entry].Player != -1; // RA2: a color is taken when a player is set
}

/// <summary>Fills a color combo box with "random" and the colors that are free or the player's own, and selects the player's.</summary>
void __fastcall Fill_Color_Combo(HWND dialog, int id) // RA2: 0x004D2080, not in TibSun (ra2-decomp's twin text, Yuri's Revenge 0x004E45A0)
{
	HWND item = GetDlgItem(dialog, id); // RA2: the first item is "random" with color -1; the free colors and the player's own follow, the player's own is selected
	bool was_hidden = !ShowWindow(item, SW_HIDE);
	SendMessage(item, CB_RESETCONTENT, 0, 0);
	SendMessage(item, 0x4DD, 0, 1);
	SendMessage(item, 0x4DE, 0, 9);
	int index = SendMessage(item, 0x4C2, 0, (LPARAM)StringTable::LoadString("GUI:RandomAsSymbols", NULL, "D:\\Ra2\\GDlgSupp.cpp", 502));
	SendMessage(item, CB_SETITEMDATA, index, -2);
	SendMessage(item, 0x498, index, -1);
	int owner = Color_Combo_Player(id);
	int selected = 0;
	for (int i = 0; i < 8; i++) {
		bool mine = ColorEntries[i].Player == owner;
		if (mine || ColorEntries[i].Player == -1) {
			wchar_t const * name = ColorEntries[i].Name; // RA2: the name is read into a local first, as Fill_Side_Combo (0x004D1560) above reads its house name; the twin has a literal here
			index = SendMessage(item, 0x4C2, 0, (LPARAM)name);
			SendMessage(item, 0x498, index, ColorEntries[i].Color);
			SendMessage(item, CB_SETITEMDATA, index, i);
			if (mine) {
				selected = index;
			}
		}
	}
	SendMessage(item, CB_SETCURSEL, selected, 0);
	SendMessage(item, 0x4F1, 0, 0);
	if (!was_hidden) {
		ShowWindow(item, SW_SHOW);
	}
}

/// <summary>Fills a color combo box with "random" only, and selects it.</summary>
void __fastcall Fill_Random_Color_Combo(HWND dialog, int id) // RA2: 0x004D2250, not in TibSun
{
	HWND combo = GetDlgItem(dialog, id); // RA2: the executable offers "random" only
	bool hidden = ShowWindow(combo, SW_HIDE) == 0;
	SendMessageA(combo, CB_RESETCONTENT, 0, 0);
	SendMessageA(combo, 0x4DD, 0, 1);
	SendMessageA(combo, 0x4DE, 0, 9);
	int item = SendMessageA(combo, 0x4C2, 0, (LPARAM)StringTable::LoadString("GUI:RandomAsSymbols", NULL, "D:\\Ra2\\GDlgSupp.cpp", 544));
	SendMessageA(combo, CB_SETITEMDATA, item, -2);
	SendMessageA(combo, OD_SETCOLOR, item, -1);
	SendMessageA(combo, CB_SETCURSEL, 0, 0);
	SendMessageA(combo, 0x4F1, 0, 1);
	if (!hidden) {
		ShowWindow(combo, SW_SHOW);
	}
}

/// <summary>Fills the color combo box of every player: in a game with a host the observer's and a team-less player's offers "random" only.</summary>
void __fastcall Fill_Color_Combos(HWND dialog) // RA2: 0x004D2300, not in TibSun
{
	for (int player = 0; player < 8; player++) { // RA2: game types 3 and 4 are IPX and Internet games
		int id = Player_Color_Combo_ID(player);
		if ((Session.Type == GAME_IPX || Session.Type == GAME_INTERNET)
			&& (Session.Slots[player] == LobbyObserverEntry || Session.Slots[player]->Slot.SpectatorFlag == -1)) {
			Fill_Random_Color_Combo(dialog, id);
		} else {
			Fill_Color_Combo(dialog, id);
		}
	}
}

/// <summary>Selects the local player's color (or random) in the combo box of a player.</summary>
void __fastcall Select_Player_Color(HWND dialog, int player) // RA2: 0x004D23C0, not in TibSun
{
	int color = Session.Color; // RA2: color 8 is the observer's gray
	if (Session.Color2 == -2) {
		color = -2;
	}
	int id = Player_Color_Combo_ID(player);
	if ((Session.Type == GAME_IPX || Session.Type == GAME_INTERNET)
		&& Session.Slots[player] == LobbyObserverEntry && Session.Slots[player]->Slot.SpectatorFlag == -1) {
		color = 8;
	}
	Select_Color_Combo(dialog, id, color);
}

/// <summary>Selects the item with this data in a color combo box, gives the color to its player and fills the combo boxes again.</summary>
void __fastcall Select_Color_Combo(HWND dialog, int id, int color) // RA2: 0x004D2480, not in TibSun
{
	HWND combo = GetDlgItem(dialog, id); // RA2: the player gives up its old color first, then takes the one whose item is selected
	bool hidden = ShowWindow(combo, SW_HIDE) == 0;
	int entry = Color_Entry_Of_Combo(id);
	if (entry != -1) {
		ColorEntries[entry].Player = -1;
	}
	int count = SendDlgItemMessageA(dialog, id, CB_GETCOUNT, 0, 0);
	for (int item = 0; item < count; item++) {
		int data = SendDlgItemMessageA(dialog, id, CB_GETITEMDATA, item, 0);
		if (data == color) {
			SendDlgItemMessageA(dialog, id, CB_SETCURSEL, item, 0);
			if (data != -2) {
				ColorEntries[data].Player = Color_Combo_Player(id);
			}
			break;
		}
	}
	if (!hidden) {
		ShowWindow(combo, SW_SHOW);
	}
	Fill_Color_Combos(dialog);
}

/// <summary>A color combo box was changed: its player gives up the old color and takes the chosen one.</summary>
void __fastcall Color_Combo_Changed(HWND dialog, int id) // RA2: 0x004D2700, not in TibSun
{
	int entry = Color_Entry_Of_Combo(id); // RA2: as Select_Color_Combo, for the item the user picked
	if (entry != -1) {
		ColorEntries[entry].Player = -1;
	}
	int color = Color_Combo_Data(dialog, id, -1);
	int player = Color_Combo_Player(id);
	if (color != -2) {
		ColorEntries[color].Player = player;
	}
	Fill_Color_Combos(dialog);
}

/// <summary>The data of an item of a color combo box (the selected item for -1).</summary>
int __fastcall Color_Combo_Data(HWND dialog, int id, int index) // RA2: 0x004D2900, not in TibSun
{
	if (index == -1) { // RA2: the selected item when the index is -1
		index = SendDlgItemMessageA(dialog, id, CB_GETCURSEL, 0, 0);
	}
	return SendDlgItemMessageA(dialog, id, CB_GETITEMDATA, index, 0);
}
