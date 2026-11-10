#include <stdio.h>
// Native M25 link closure rows (Scout1202). NOT EXACT unless stated: behaviour from
// the original's code.

#include "shared/radar.hpp"
#include "shared/house.hpp"
#include "shared/house_type.hpp"
#include "shared/color_scheme.hpp"
#include "shared/hsv_class.hpp"
#include "shared/progress_screen.hpp"
#include "shared/gdlg_supp.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/win32_api.hpp"
#include "shared/owner_draw_dialog_control.hpp"

extern HouseClass *unknown_player_a35db4;                       // 0x00A35DB4

// 0x005E1FD0: sends 0x4E6 with a byte to a control (as 0x005E1FB0 sends 0x4E5; the
// diplomacy dialog marks an allied player's check box with it).
void __fastcall unknown_5e1fd0(HWND control, unsigned char value)
{
    SendMessageA(control, 0x4E6, 0, value);
}

// 0x00634670 (a __stdcall function of three arguments; the tree declares it on
// RadarClass): one player's row of the diplomacy dialog (Diplomacy_Dialog 0x006340F0
// through 0x00634970) from the original's code. The row's controls by index: name
// 1041+i, score 1049+i, alliance check 0x007EE0AC[i] (1743-1749), the team check
// 0x007EE0CC[i] (1750-1753, 1762-1764). For a house of a Multiplay type: its UIName in
// its colour (LightGrey for the observer 0x00A72D28, Grey for a defeated house, else
// its colour scheme's BaseColor), its country flag (the observer's -3) on the control
// 0x004D0EE0 gives, the alliance check (disabled while 0x00A3D548 is set, hidden for a
// defeated house, else checked when the player is allied to it; 0x4E6 tells whether it
// is allied to the player), the team check (hidden for +0x18F, else checked and
// disabled while 0x00A3D548 is set), and its score: the sum of its 20 kill counts of
// each kind (+0x5310, +0x5364), "%d". Otherwise the name and score are cleared.
void RadarClass::Unknown_00634670(int arg1, int index, HouseClass *house)
{
    static int const name_ids[8] = {1041, 1042, 1043, 1044, 1045, 1046, 1047, 1048};
    static int const score_ids[8] = {1049, 1050, 1051, 1052, 1053, 1054, 1055, 1056};
    static int const ally_ids[8] = {-1, 1743, 1744, 1745, 1746, 1747, 1748, 1749};
    static int const team_ids[8] = {-1, 1750, 1751, 1752, 1753, 1762, 1763, 1764};
    HWND dialog = (HWND)(intptr_t)arg1;
    if (index < 0 || index >= 8) return;               // native guard (the tables have 8 rows)
    int score_id = score_ids[index];
    int name_id = name_ids[index];
    unsigned short text[100];
    text[0] = 0;
    if (house == 0 || !house->Type->Multiplay) {
        SendMessageA(GetDlgItem(dialog, name_id), 0x4B2, 0, (LPARAM)text);
        SendMessageA(GetDlgItem(dialog, score_id), 0x4B2, 0, (LPARAM)text);
        return;
    }
    int n = 0;
    for (; house->UIName[n] && n < 20; ++n) text[n] = house->UIName[n];
    text[n] = 0;
    ColorScheme *scheme;
    if (house == unknown_house_a72d28) {
        scheme = unknown_color_scheme_663d30("LightGrey", 1);
    } else if (house->unknown_13d) {
        scheme = unknown_color_scheme_663d30("Grey", 1);
    } else {
        scheme = ColorSchemes[house->ColorSchemeIndex];
    }
    RGBClass rgb = scheme->BaseColor.To_RGB();
    unsigned int color = (unsigned int)rgb.Red | ((unsigned int)rgb.Green << 8) | ((unsigned int)rgb.Blue << 16);
    HWND name = GetDlgItem(dialog, name_id);
    SendMessageA(name, 0x498, 0, color);
    SendMessageA(name, 0x4B2, 0, (LPARAM)text);
    HWND flag = GetDlgItem(dialog, unknown_player_control_4d0ee0(index));
    Surface *image = Country_Flag_004d1120(house->Type->ArrayIndex);
    if (house == unknown_house_a72d28) image = Country_Flag_004d1120(-3);
    OwnerDraw::unknown_5e1f30(flag, image);
    HWND ally = GetDlgItem(dialog, ally_ids[index]);
    if (unknown_int_a3d548) EnableWindow(ally, FALSE);
    if (house->unknown_13d) {
        ShowWindow(ally, 0);
    } else {
        SendMessageA(ally, 0xF1, unknown_player_a35db4->Is_Ally(house) ? 1 : 0, 0);
    }
    unknown_5e1fd0(ally, house->Is_Ally(unknown_player_a35db4));
    if (house->unknown_18f) {
        ShowWindow(GetDlgItem(dialog, team_ids[index]), 0);
    } else if (unknown_int_a3d548) {
        HWND team = GetDlgItem(dialog, team_ids[index]);
        SendMessageA(team, 0xF1, 1, 0);
        EnableWindow(team, FALSE);
    }
    int total = 0;
    for (int i = 0; i < 20; ++i) total += house->unknown_5310[i] + house->unknown_5364[i];
    char digits[16];
    snprintf(digits, sizeof digits, "%d", total);
    for (n = 0; digits[n]; ++n) text[n] = (unsigned char)digits[n];
    text[n] = 0;
    HWND score = GetDlgItem(dialog, score_id);
    SendMessageA(score, 0x498, 0, color);
    SendMessageA(score, 0x4B2, 0, (LPARAM)text);
}

#include "shared/tactical.hpp"

// 0x006AA5A0 / 0x006AA5E0 (Leaf seeds; DisplayClass slot 47 calls them from the scroll
// and mouse AI): the selection band. Start: when no band is open (both coordinates of
// the first corner zero), both corners become the point. Drag: while a band is open,
// the second corner follows the point. Reached by the RA2.app pass (M25): a click on the
// map held for a frame aborted the game here.
void Tactical::Unknown_006AA5A0(Point2D const &point)
{
    if (RubberBandStart.X == 0 && RubberBandStart.Y == 0) {
        RubberBandStart = point;
        RubberBandEnd = point;
    }
}

void Tactical::Unknown_006AA5E0(Point2D const &point)
{
    if (RubberBandStart.X != 0 || RubberBandStart.Y != 0) {
        RubberBandEnd = point;
    }
}
