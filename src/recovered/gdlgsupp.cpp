// gdlgsupp.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/gdlgsupp.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/gdlg_supp.hpp"
#include "shared/pcx_cache.hpp"
#include "shared/owner_draw_dialog_control.hpp"
#include "shared/progress_screen.hpp"
#include "shared/string_table.hpp"
#include "shared/house_type.hpp"
#include "shared/unknown_player_entry.hpp"
#include "shared/session_state.hpp"
#include "shared/unknown_globals.hpp"


// Header-static dynamic initializers of the unit at 0x004D0CF0: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x004D0CF0-0x004D0ED1); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x004D0D10, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_004d0cf0.cpp; no source of its own.


// Dynamic initializer (0x004D0D40, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_004d0cf0.cpp; no source of its own.


// Dynamic initializer (0x004D0D60, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_004d0cf0.cpp; no source of its own.


// Dynamic initializer (0x004D0D80, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_004d0cf0.cpp; no source of its own.


// Dynamic initializer (0x004D0DA0, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_004d0cf0.cpp; no source of its own.


// Dynamic initializer (0x004D0DC0, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_004d0cf0.cpp; no source of its own.


// Dynamic initializer (0x004D0DE0, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_004d0cf0.cpp; no source of its own.


// Dynamic initializer (0x004D0E20, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_004d0cf0.cpp; no source of its own.


// Dynamic initializer (0x004D0E50, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_004d0cf0.cpp; no source of its own.


// Dynamic initializer (0x004D0E80, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_004d0cf0.cpp; no source of its own.


// Dynamic initializer (0x004D0EB0, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_004d0cf0.cpp; no source of its own.


// Dynamic initializer (0x004D0EC0, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_004d0cf0.cpp; no source of its own.


// int __fastcall unknown_player_control_4d0ee0(int player) (0x004D0EE0)
// A player row's control id (0x6DA-0x6E1; meaning not recovered), -1 for any other value.

int __fastcall unknown_player_control_4d0ee0(int player)
{
    if (player == 0) {
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


// int __fastcall unknown_control_player_4d0f40(int id) (0x004D0F40)
// The player row of a 0x6DA-0x6E1 control id (the inverse of 0x004D0EE0), or -1.

int __fastcall unknown_control_player_4d0f40(int id)
{
    if (id == 0x6DA) {
        return 0;
    }
    if (id == 0x6DB) {
        return 1;
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


// Surface *__fastcall Ping_Image_4d0fb0(int ping) (0x004D0FB0)
// The ping image of a connection quality (0 "pingg.pcx", 1 "pingy.pcx",
// 2 "pingr.pcx"); null for any other value.

Surface *__fastcall Ping_Image_4d0fb0(int ping)
{
    Surface *image = 0;
    if (ping == 0) {
        image = unknown_bitmap_writer_a76378.Get_Image_0068ca40(unknown_ping_images_7f86dc[0], 0);
    } else if (ping == 1) {
        image = unknown_bitmap_writer_a76378.Get_Image_0068ca40(unknown_ping_images_7f86dc[1], 0);
    } else if (ping == 2) {
        image = unknown_bitmap_writer_a76378.Get_Image_0068ca40(unknown_ping_images_7f86dc[2], 0);
    }
    return image;
}


// int __fastcall unknown_player_control_4d1000(int player) (0x004D1000)
// A player row's control id (0x754-0x75B; meaning not recovered), -1 for any other value.

int __fastcall unknown_player_control_4d1000(int player)
{
    if (player == 0) {
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


// void __fastcall Set_Player_Ping_4d1060(HWND dialog, int player, int ping) (0x004D1060)
// Shows a ping image in a player row's 0x754-0x75B control.

void __fastcall Set_Player_Ping_4d1060(HWND dialog, int player, int ping)
{
    HWND control = GetDlgItem(dialog, unknown_player_control_4d1000(player));
    Surface *image = Ping_Image_4d0fb0(ping);
    if (control != (HWND)-1) {
        OwnerDraw::unknown_5e1f30(control, image);
    }
}


// Surface *__fastcall Country_Flag(int country) (0x004D1120)
// A country's flag image: countries 0-8 "usai.pcx" ... "rusi.pcx", -2
// "rani.pcx" (random), -3 "obsi.pcx" (observer); null for any other value.

Surface *__fastcall Country_Flag(int country)
{
    Surface *image = 0;
    if (country == -2) {
        image = unknown_bitmap_writer_a76378.Get_Image_0068ca40(unknown_flag_images_7f86b0[0], 0);
    } else if (country == 0) {
        image = unknown_bitmap_writer_a76378.Get_Image_0068ca40(unknown_flag_images_7f86b0[2], 0);
    } else if (country == 1) {
        image = unknown_bitmap_writer_a76378.Get_Image_0068ca40(unknown_flag_images_7f86b0[3], 0);
    } else if (country == 2) {
        image = unknown_bitmap_writer_a76378.Get_Image_0068ca40(unknown_flag_images_7f86b0[4], 0);
    } else if (country == 3) {
        image = unknown_bitmap_writer_a76378.Get_Image_0068ca40(unknown_flag_images_7f86b0[5], 0);
    } else if (country == 4) {
        image = unknown_bitmap_writer_a76378.Get_Image_0068ca40(unknown_flag_images_7f86b0[6], 0);
    } else if (country == 5) {
        image = unknown_bitmap_writer_a76378.Get_Image_0068ca40(unknown_flag_images_7f86b0[7], 0);
    } else if (country == 6) {
        image = unknown_bitmap_writer_a76378.Get_Image_0068ca40(unknown_flag_images_7f86b0[8], 0);
    } else if (country == 7) {
        image = unknown_bitmap_writer_a76378.Get_Image_0068ca40(unknown_flag_images_7f86b0[9], 0);
    } else if (country == 8) {
        image = unknown_bitmap_writer_a76378.Get_Image_0068ca40(unknown_flag_images_7f86b0[10], 0);
    } else if (country == -3) {
        image = unknown_bitmap_writer_a76378.Get_Image_0068ca40(unknown_flag_images_7f86b0[1], 0);
    }
    return image;
}


// void __fastcall Show_Player_Flag_4d1230(HWND dialog, int player) (0x004D1230)
// Shows the flag of the country selected in a player row's side combo box in
// the row's 0x6DA-0x6E1 control.

void __fastcall Show_Player_Flag_4d1230(HWND dialog, int player)
{
    HWND control = GetDlgItem(dialog, unknown_player_control_4d0ee0(player));
    OwnerDraw::unknown_5e1f30(control, Country_Flag(Side_Combo_Data_4d1c70(dialog, Player_Side_Combo_ID_4d1350(player), -1)));
}


// int __fastcall Player_Side_Combo_ID_4d1350(int player) (0x004D1350)
// The side combo box id of a player row (0-7), -1 for any other value.

int __fastcall Player_Side_Combo_ID_4d1350(int player)
{
    if (player == 0) {
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


// int __fastcall Side_Combo_Player_4d13b0(int id) (0x004D13B0)
// The player row of a side combo box id (the inverse of 0x004D1350), or -1.

int __fastcall Side_Combo_Player_4d13b0(int id)
{
    if (id == 0x6A1) {
        return 0;
    }
    if (id == 0x510) {
        return 1;
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


// wchar_t const *__fastcall Side_Name_4d1420(int side) (0x004D1420)
// The name of a country (STT:PlayerSide<Country>; -2 random, -3 observer),
// null for any other value.

wchar_t const *__fastcall Side_Name_4d1420(int side)
{
    if (side == -2) {
        return StringTable::Load_String("STT:PlayerSideRandom", 0, "D:\\Ra2\\GDlgSupp.cpp", 231);
    }
    if (side == -3) {
        return StringTable::Load_String("STT:PlayerSideObserver", 0, "D:\\Ra2\\GDlgSupp.cpp", 233);
    }
    if (side == 0) {
        return StringTable::Load_String("STT:PlayerSideAmerica", 0, "D:\\Ra2\\GDlgSupp.cpp", 235);
    }
    if (side == 1) {
        return StringTable::Load_String("STT:PlayerSideKorea", 0, "D:\\Ra2\\GDlgSupp.cpp", 237);
    }
    if (side == 2) {
        return StringTable::Load_String("STT:PlayerSideFrance", 0, "D:\\Ra2\\GDlgSupp.cpp", 239);
    }
    if (side == 3) {
        return StringTable::Load_String("STT:PlayerSideGermany", 0, "D:\\Ra2\\GDlgSupp.cpp", 241);
    }
    if (side == 4) {
        return StringTable::Load_String("STT:PlayerSideBritain", 0, "D:\\Ra2\\GDlgSupp.cpp", 243);
    }
    if (side == 5) {
        return StringTable::Load_String("STT:PlayerSideLibya", 0, "D:\\Ra2\\GDlgSupp.cpp", 245);
    }
    if (side == 6) {
        return StringTable::Load_String("STT:PlayerSideIraq", 0, "D:\\Ra2\\GDlgSupp.cpp", 247);
    }
    if (side == 7) {
        return StringTable::Load_String("STT:PlayerSideCuba", 0, "D:\\Ra2\\GDlgSupp.cpp", 249);
    }
    if (side == 8) {
        return StringTable::Load_String("STT:PlayerSideRussia", 0, "D:\\Ra2\\GDlgSupp.cpp", 251);
    }
    return 0;
}


// void __fastcall Fill_Side_Combo_4d1560(HWND dialog, int id) (0x004D1560)
// Fills a side combo box: "GUI:RandomEx" (item data -2), then every
// multiplayer house type (data: its index), and selects the first item. The
// box is hidden while it is filled when it was visible.

void __fastcall Fill_Side_Combo_4d1560(HWND dialog, int id)
{
    HWND combo = GetDlgItem(dialog, id);
    bool hidden = ShowWindow(combo, SW_HIDE) == 0;
    SendMessageA(combo, CB_RESETCONTENT, 0, 0);
    SendMessageA(combo, 0x4DE, 0, 7);
    int item = SendMessageA(combo, 0x4C2, 0, (LPARAM)StringTable::Load_String("GUI:RandomEx", 0, "D:\\Ra2\\GDlgSupp.cpp", 268));
    SendMessageA(combo, CB_SETITEMDATA, item, -2);
    int count = HouseTypes.Count();
    for (int index = 0; index < count; index++) {
        HouseTypeClass *type = HouseTypes[index];
        if (type->Multiplay) {
            const unsigned short *name = type->UIName;
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


// void __fastcall Fill_Observer_Combo_4d1640(HWND dialog, int id) (0x004D1640)
// Fills a side combo box with "GUI:Observer" alone (item data -3) and
// selects it; hidden while filled when it was visible.

void __fastcall Fill_Observer_Combo_4d1640(HWND dialog, int id)
{
    HWND combo = GetDlgItem(dialog, id);
    bool hidden = ShowWindow(combo, SW_HIDE) == 0;
    SendMessageA(combo, CB_RESETCONTENT, 0, 0);
    SendMessageA(combo, 0x4DE, 0, 7);
    int item = SendMessageA(combo, 0x4C2, 0, (LPARAM)StringTable::Load_String("GUI:Observer", 0, "D:\\Ra2\\GDlgSupp.cpp", 298));
    SendMessageA(combo, CB_SETITEMDATA, item, -3);
    SendMessageA(combo, CB_SETCURSEL, 0, 0);
    SendMessageA(combo, 0x4F1, 0, 1);
    if (!hidden) {
        ShowWindow(combo, SW_SHOW);
    }
}


// void __fastcall Fill_Side_Combos_4d16d0(HWND dialog) (0x004D16D0)
// Refills every player row's side combo box: a row whose player combo box
// (control id = the row) holds the 0x00A72D44 entry gets the observer entry
// alone, the others the countries.

void __fastcall Fill_Side_Combos_4d16d0(HWND dialog)
{
    for (int player = 0; player < 8; player++) {
        int data = unknown_combo_selected_data_005c99d0(dialog, player, -1);
        bool observer = false;
        if (unknown_player_a72d44 != 0 && data == (int)unknown_player_a72d44) {
            observer = true;
        }
        int id = Player_Side_Combo_ID_4d1350(player);
        if (observer) {
            Fill_Observer_Combo_4d1640(dialog, id);
        } else {
            Fill_Side_Combo_4d1560(dialog, id);
        }
    }
}


// void __fastcall Select_Player_Side_4d1820(HWND dialog, int player) (0x004D1820)
// Selects the session player's country in a row's side combo box (random,
// -2, or observer, -3, when the session says so).

void __fastcall Select_Player_Side_4d1820(HWND dialog, int player)
{
    int side = unknown_session_a3d298.unknown_134;
    if (unknown_session_a3d298.unknown_138 == -2) {
        side = -2;
    } else if (unknown_session_a3d298.unknown_138 == -3) {
        side = -3;
    }
    Select_Side_Combo_4d1a90(dialog, Player_Side_Combo_ID_4d1350(player), side);
}


// void __fastcall Select_Side_Combo_4d1a90(HWND dialog, int id, int side) (0x004D1A90)
// Selects a country in a player row's side combo box and shows its flag.
// The box is hidden while it changes when it was visible.

void __fastcall Select_Side_Combo_4d1a90(HWND dialog, int id, int side)
{
    HWND combo = GetDlgItem(dialog, id);
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
    Show_Player_Flag_4d1230(dialog, Side_Combo_Player_4d13b0(id));
}


// int __fastcall Side_Combo_Data_4d1c70(HWND dialog, int id, int index) (0x004D1C70)
// The item data of the side combo box's item index (-1: the selection).

int __fastcall Side_Combo_Data_4d1c70(HWND dialog, int id, int index)
{
    if (index == -1) {
        index = SendDlgItemMessageA(dialog, id, CB_GETCURSEL, 0, 0);
    }
    return SendDlgItemMessageA(dialog, id, CB_GETITEMDATA, index, 0);
}


// int __fastcall Player_Color_Combo_ID_4d1cb0(int player) (0x004D1CB0)
// The colour combo box id of a player row (0-7), -1 for any other value.

int __fastcall Player_Color_Combo_ID_4d1cb0(int player)
{
    if (player == 0) {
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


// int __fastcall Color_Combo_Player_4d1d10(int id) (0x004D1D10)
// The player row of a colour combo box id (the inverse of 0x004D1CB0), or -1.

int __fastcall Color_Combo_Player_4d1d10(int id)
{
    if (id == 0x6A2) {
        return 0;
    }
    if (id == 0x522) {
        return 1;
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


// wchar_t const *__fastcall Color_Name_4d1d80(int color) (0x004D1D80)
// The name of a player colour (STT:PlayerColor<Colour>; -2 random, 8
// observer), null for any other value.

wchar_t const *__fastcall Color_Name_4d1d80(int color)
{
    if (color == -2) {
        return StringTable::Load_String("STT:PlayerColorRandom", 0, "D:\\Ra2\\GDlgSupp.cpp", 429);
    }
    if (color == 0) {
        return StringTable::Load_String("STT:PlayerColorGold", 0, "D:\\Ra2\\GDlgSupp.cpp", 431);
    }
    if (color == 1) {
        return StringTable::Load_String("STT:PlayerColorRed", 0, "D:\\Ra2\\GDlgSupp.cpp", 433);
    }
    if (color == 2) {
        return StringTable::Load_String("STT:PlayerColorBlue", 0, "D:\\Ra2\\GDlgSupp.cpp", 435);
    }
    if (color == 3) {
        return StringTable::Load_String("STT:PlayerColorGreen", 0, "D:\\Ra2\\GDlgSupp.cpp", 437);
    }
    if (color == 4) {
        return StringTable::Load_String("STT:PlayerColorOrange", 0, "D:\\Ra2\\GDlgSupp.cpp", 439);
    }
    if (color == 5) {
        return StringTable::Load_String("STT:PlayerColorSkyBlue", 0, "D:\\Ra2\\GDlgSupp.cpp", 441);
    }
    if (color == 6) {
        return StringTable::Load_String("STT:PlayerColorPurple", 0, "D:\\Ra2\\GDlgSupp.cpp", 443);
    }
    if (color == 7) {
        return StringTable::Load_String("STT:PlayerColorPink", 0, "D:\\Ra2\\GDlgSupp.cpp", 445);
    }
    if (color == 8) {
        return StringTable::Load_String("STT:PlayerColorObserver", 0, "D:\\Ra2\\GDlgSupp.cpp", 447);
    }
    return 0;
}


// void __fastcall Init_Color_Entries_4d1ea0() (0x004D1EA0)
// Fills the dialogs' colour table: each entry's abbreviated name
// (GUI:<Colour>Abbrev), its colour value from 0x007E6D94 and no combo box
// item yet (-1).

void __fastcall Init_Color_Entries_4d1ea0()
{
    unknown_color_entries_8660d8[0].Name = StringTable::Load_String("GUI:GoldAbbrev", 0, "D:\\Ra2\\GDlgSupp.cpp", 455);
    unknown_color_entries_8660d8[1].Name = StringTable::Load_String("GUI:RedAbbrev", 0, "D:\\Ra2\\GDlgSupp.cpp", 456);
    unknown_color_entries_8660d8[2].Name = StringTable::Load_String("GUI:BlueAbbrev", 0, "D:\\Ra2\\GDlgSupp.cpp", 457);
    unknown_color_entries_8660d8[3].Name = StringTable::Load_String("GUI:GreenAbbrev", 0, "D:\\Ra2\\GDlgSupp.cpp", 458);
    unknown_color_entries_8660d8[4].Name = StringTable::Load_String("GUI:OrangeAbbrev", 0, "D:\\Ra2\\GDlgSupp.cpp", 459);
    unknown_color_entries_8660d8[5].Name = StringTable::Load_String("GUI:SkyBlueAbbrev", 0, "D:\\Ra2\\GDlgSupp.cpp", 460);
    unknown_color_entries_8660d8[6].Name = StringTable::Load_String("GUI:PurpleAbbrev", 0, "D:\\Ra2\\GDlgSupp.cpp", 461);
    unknown_color_entries_8660d8[7].Name = StringTable::Load_String("GUI:PinkAbbrev", 0, "D:\\Ra2\\GDlgSupp.cpp", 462);
    unknown_color_entries_8660d8[8].Name = StringTable::Load_String("GUI:GrayAbbrev", 0, "D:\\Ra2\\GDlgSupp.cpp", 463);
    for (int index = 0; index < 9; index++) {
        unknown_color_entries_8660d8[index].Color = unknown_color_values_7e6d94[index];
        unknown_color_entries_8660d8[index].Index = -1;
    }
}


// int __fastcall Color_Entry_Of_Combo_4d1fc0(int id) (0x004D1FC0)
// The colour table entry a player row's colour combo box holds, or -1.

int __fastcall Color_Entry_Of_Combo_4d1fc0(int id)
{
    int player = Color_Combo_Player_4d1d10(id);
    for (int index = 0; index < 9; index++) {
        if (unknown_color_entries_8660d8[index].Index == player) {
            return index;
        }
    }
    return -1;
}


// bool __fastcall Color_Entry_Taken_4d2060(int entry) (0x004D2060)
// Whether a colour table entry is held by a player row.

bool __fastcall Color_Entry_Taken_4d2060(int entry)
{
    return unknown_color_entries_8660d8[entry].Index != -1;
}


// Not exact (RA2_NON_MATCHING): Fill_Color_Combo_4d2080 at 0x004D2080, best score 24.42, 2026-11-13; differs: 21 instructions in the original, 151 in the draft (shape 24.42)
#ifdef RA2_NON_MATCHING
// void __fastcall Fill_Color_Combo_4d2080(HWND dialog, int id) (0x004D2080)
// Fills a player row's colour combo box with "GUI:RandomAsSymbols" (item data -2)
// and every colour table entry that is free or held by this row's player (item
// data: the entry), selects the row's own colour (or the random item) and shows
// the box again when it was visible.

void __fastcall Fill_Color_Combo_4d2080(HWND dialog, int id)
{
    HWND combo = GetDlgItem(dialog, id);
    bool hidden = ShowWindow(combo, SW_HIDE) == 0;
    SendMessageA(combo, CB_RESETCONTENT, 0, 0);
    SendMessageA(combo, 0x4DD, 0, 1);
    SendMessageA(combo, 0x4DE, 0, 9);
    int item = SendMessageA(combo, 0x4C2, 0, (LPARAM)StringTable::Load_String("GUI:RandomAsSymbols", 0, "D:\\Ra2\\GDlgSupp.cpp", 502));
    SendMessageA(combo, CB_SETITEMDATA, item, -2);
    SendMessageA(combo, 0x498, item, -1);
    int player = Color_Combo_Player_4d1d10(id);
    int selected = 0;
    for (int index = 0; index < 8; index++) {
        bool mine = unknown_color_entries_8660d8[index].Index == player;
        if (mine || unknown_color_entries_8660d8[index].Index == -1) {
            item = SendMessageA(combo, 0x4C2, 0, (LPARAM)unknown_color_entries_8660d8[index].Name);
            SendMessageA(combo, 0x498, item, unknown_color_entries_8660d8[index].Color);
            SendMessageA(combo, CB_SETITEMDATA, item, index);
            if (mine) {
                selected = item;
            }
        }
    }
    SendMessageA(combo, CB_SETCURSEL, selected, 0);
    SendMessageA(combo, 0x4F1, 0, 0);
    if (!hidden) {
        ShowWindow(combo, SW_SHOW);
    }
}
#endif

// void __fastcall Fill_Random_Color_Combo_4d2250(HWND dialog, int id) (0x004D2250)
// Fills a colour combo box with "GUI:RandomAsSymbols" alone (item data -2)
// and selects it; hidden while it is filled when it was visible.

void __fastcall Fill_Random_Color_Combo_4d2250(HWND dialog, int id)
{
    HWND combo = GetDlgItem(dialog, id);
    bool hidden = ShowWindow(combo, SW_HIDE) == 0;
    SendMessageA(combo, CB_RESETCONTENT, 0, 0);
    SendMessageA(combo, 0x4DD, 0, 1);
    SendMessageA(combo, 0x4DE, 0, 9);
    int item = SendMessageA(combo, 0x4C2, 0, (LPARAM)StringTable::Load_String("GUI:RandomAsSymbols", 0, "D:\\Ra2\\GDlgSupp.cpp", 544));
    SendMessageA(combo, CB_SETITEMDATA, item, -2);
    SendMessageA(combo, 0x498, item, -1);
    SendMessageA(combo, CB_SETCURSEL, 0, 0);
    SendMessageA(combo, 0x4F1, 0, 1);
    if (!hidden) {
        ShowWindow(combo, SW_SHOW);
    }
}


// void __fastcall Fill_Color_Combos_4d2300(HWND dialog) (0x004D2300)
// Refills every player row's colour combo box; in game modes 3 and 4 a row
// with the 0x00A72D44 entry or without a team gets the random entry alone.

void __fastcall Fill_Color_Combos_4d2300(HWND dialog)
{
    for (int player = 0; player < 8; player++) {
        int id = Player_Color_Combo_ID_4d1cb0(player);
        if ((unknown_game_mode_a3d298 == 3 || unknown_game_mode_a3d298 == 4)
            && (unknown_player_slots_a3fa98[player] == unknown_player_a72d44 || unknown_player_slots_a3fa98[player]->Team == -1)) {
            Fill_Random_Color_Combo_4d2250(dialog, id);
        } else {
            Fill_Color_Combo_4d2080(dialog, id);
        }
    }
}


// void __fastcall Select_Player_Color_4d23c0(HWND dialog, int player) (0x004D23C0)
// Selects the session player's colour in a row's colour combo box: random
// (-2) with a random country; in game modes 3 and 4 the observer colour (8)
// for the 0x00A72D44 entry without a team.


void __fastcall Select_Player_Color_4d23c0(HWND dialog, int player)
{
    int color = unknown_session_a3d298.unknown_12c;
    if (unknown_session_a3d298.unknown_130 == -2) {
        color = -2;
    }
    int id = Player_Color_Combo_ID_4d1cb0(player);
    if ((unknown_game_mode_a3d298 == 3 || unknown_game_mode_a3d298 == 4)
        && unknown_player_slots_a3fa98[player] == unknown_player_a72d44 && unknown_player_slots_a3fa98[player]->Team == -1) {
        color = 8;
    }
    Select_Color_Combo_4d2480(dialog, id, color);
}


// void __fastcall Select_Color_Combo_4d2480(HWND dialog, int id, int color) (0x004D2480)
// Selects a colour in a player row's colour combo box: the row gives up the
// colour it held, takes the new one (unless it is random, -2), and every
// row's colour list is refilled. The box is hidden while it changes when it
// was visible.

void __fastcall Select_Color_Combo_4d2480(HWND dialog, int id, int color)
{
    HWND combo = GetDlgItem(dialog, id);
    bool hidden = ShowWindow(combo, SW_HIDE) == 0;
    int entry = Color_Entry_Of_Combo_4d1fc0(id);
    if (entry != -1) {
        unknown_color_entries_8660d8[entry].Index = -1;
    }
    int count = SendDlgItemMessageA(dialog, id, CB_GETCOUNT, 0, 0);
    for (int item = 0; item < count; item++) {
        int data = SendDlgItemMessageA(dialog, id, CB_GETITEMDATA, item, 0);
        if (data == color) {
            SendDlgItemMessageA(dialog, id, CB_SETCURSEL, item, 0);
            if (data != -2) {
                unknown_color_entries_8660d8[data].Index = Color_Combo_Player_4d1d10(id);
            }
            break;
        }
    }
    if (!hidden) {
        ShowWindow(combo, SW_SHOW);
    }
    Fill_Color_Combos_4d2300(dialog);
}


// void __fastcall Color_Combo_Changed_4d2700(HWND dialog, int id) (0x004D2700)
// A row's colour selection changed: the row gives up the colour it held,
// takes the selected one (unless it is random, -2), and every row's colour
// list is refilled.

void __fastcall Color_Combo_Changed_4d2700(HWND dialog, int id)
{
    int entry = Color_Entry_Of_Combo_4d1fc0(id);
    if (entry != -1) {
        unknown_color_entries_8660d8[entry].Index = -1;
    }
    int color = Color_Combo_Data_4d2900(dialog, id, -1);
    int player = Color_Combo_Player_4d1d10(id);
    if (color != -2) {
        unknown_color_entries_8660d8[color].Index = player;
    }
    Fill_Color_Combos_4d2300(dialog);
}


// int __fastcall Color_Combo_Data_4d2900(HWND dialog, int id, int index) (0x004D2900)
// The item data of the colour combo box's item index (-1: the selection).

int __fastcall Color_Combo_Data_4d2900(HWND dialog, int id, int index)
{
    if (index == -1) {
        index = SendDlgItemMessageA(dialog, id, CB_GETCURSEL, 0, 0);
    }
    return SendDlgItemMessageA(dialog, id, CB_GETITEMDATA, index, 0);
}
