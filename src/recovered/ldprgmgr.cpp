// ldprgmgr.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/ldprgmgr.json carries the addresses and the rows that
// stay in their own files.


#define RA2_LOAD_PROGRESS_RECTS
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/rect.hpp"
#include "shared/mix_file_class.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/id_source.hpp"
#include "shared/cd.hpp"
#include "shared/text_print.hpp"
#include "shared/hicolor.hpp"
#include "shared/win32_api.hpp"


// Unknown535910 0x005359E0/0x00535AB0 read the three areas as Rects.
// Header-static dynamic initializers of the unit at 0x00535720: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x00535720-0x00535901); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x00535740, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_00535720.cpp; no source of its own.


// Dynamic initializer (0x00535770, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_00535720.cpp; no source of its own.


// Dynamic initializer (0x00535790, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_00535720.cpp; no source of its own.


// Dynamic initializer (0x005357B0, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_00535720.cpp; no source of its own.


// Dynamic initializer (0x005357D0, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_00535720.cpp; no source of its own.


// Dynamic initializer (0x005357F0, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_00535720.cpp; no source of its own.


// Dynamic initializer (0x00535810, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_00535720.cpp; no source of its own.


// Dynamic initializer (0x00535850, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_00535720.cpp; no source of its own.


// Dynamic initializer (0x00535880, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_00535720.cpp; no source of its own.


// Dynamic initializer (0x005358B0, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_00535720.cpp; no source of its own.


// Dynamic initializer (0x005358E0, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_00535720.cpp; no source of its own.


// Dynamic initializer (0x005358F0, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_00535720.cpp; no source of its own.


// Unknown535910 *Unknown_00535910() -- 0x00535910
// Makes the loading progress object once and returns it.

Unknown535910 *Unknown_00535910()
{
    if (unknown_load_progress_a6e7c4 == 0) {
        unknown_load_progress_a6e7c4 = new Unknown535910;
    }
    return unknown_load_progress_a6e7c4;
}


// void Unknown_00535970() -- 0x00535970
// Releases and deletes the loading progress object.

void Unknown_00535970()
{
    if (unknown_load_progress_a6e7c4 != 0) {
        unknown_load_progress_a6e7c4->Unknown_00536E20();
        delete unknown_load_progress_a6e7c4;
    }
    unknown_load_progress_a6e7c4 = 0;
}


// Unknown535910::Unknown535910() -- 0x005359A0

Unknown535910::Unknown535910()
{
    LoadMix = 0;
    unknown_38 = 0;
    unknown_3c = 0;
    unknown_40 = 0;
    unknown_44 = 0;
    unknown_48 = 0;
    unknown_4c = false;
    unknown_4d = false;
    unknown_4e = false;
    unknown_50 = 0;
    unknown_54 = 0;
    unknown_58 = 0;
    unknown_5c = 0;
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void Unknown535910::Unknown_005359E0() -- 0x005359E0
// Places the loading screen's three areas by the screen size: in a campaign
// the title bar (+0x08), picture (+0x18) and text bar (+0x28) stacked and
// centred; otherwise only the picture, centred.
// from ra2-decomp 0x00552B10 (LoadProgressManager::Compute_Rects, src/ldprgmgr.cpp)

void Unknown535910::Unknown_005359E0()
{
    int render_width = unknown_8523e0;
    bool low_res = render_width == 640;
    int width = low_res ? unknown_screen_widths_7add68[0] : unknown_screen_widths_7add68[1];
    if (unknown_game_mode_a3d298 == 0) {
        int height = (low_res ? unknown_screen_heights_7add74[0] : unknown_screen_heights_7add74[1]) - 80;
        int x = (render_width - width) / 2;
        int y = (unknown_8523e4 - height - 80) / 2;
        unknown_28.Width = unknown_18.Width = unknown_08.Width = width;
        unknown_18.X = unknown_08.X = x;
        unknown_08.Y = y;
        unknown_08.Height = 40;
        y += 40;
        unknown_18.Y = y;
        unknown_28.X = x;
        unknown_18.Height = height;
        y += height;
        unknown_28.Y = y;
        unknown_28.Height = 40;
    } else {
        int height = low_res ? unknown_screen_heights_7add74[0] : unknown_screen_heights_7add74[1];
        unknown_18.Set((render_width - width) / 2, (unknown_8523e4 - height) / 2, width, height);
    }
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// Point2D Unknown535910::Unknown_00535AB0() -- 0x00535AB0
// The loading screen's text position: in a campaign inside the text bar,
// otherwise offset from the picture's corner, both by the screen size.
// from ra2-decomp 0x00552BE0 (LoadProgressManager::Text_Origin, src/ldprgmgr.cpp)

Point2D __fastcall Unknown_Point_Copy_006F1AE0(Point2D const *source);

Point2D Unknown535910::Unknown_00535AB0()
{
    Point2D p;
    if (unknown_game_mode_a3d298 == 0) {
        p.X = unknown_28.X;
        if (unknown_8523e0 == unknown_screen_widths_7add68[0]) {
            p.X += 0x54;
        } else {
            p.X += 0xa4;
        }
        p.Y = unknown_28.Y + 7;
    } else {
        p = Unknown_Point_Copy_006F1AE0((Point2D const *)&unknown_18);
        if (unknown_8523e0 == unknown_screen_widths_7add68[0]) {
            p.X += 0xc;
            p.Y += 0x100;
        } else {
            p.X += 0x10;
            p.Y += 0x141;
        }
    }
    return p;
}


// int Unknown535910::Unknown_00535B60() -- 0x00535B60
// The name width (-1 in a campaign).

int Unknown535910::Unknown_00535B60()
{
    int width = -1;
    if (unknown_game_mode_a3d298 != 0) {
        width = unknown_8523e0 == unknown_screen_widths_7add68[0] ? 326 : 406;
    }
    return width;
}


// void Unknown535910::Unknown_00535B90() -- 0x00535B90
// Opens LOAD.MIX once and loads the loading-screen art.

void Unknown535910::Unknown_00535B90()
{
    if (LoadMix == 0) {
        LoadMix = new MixFileClass("LOAD.MIX", &FastKey);
        if (LoadMix == 0) {
            return;
        }
    }
    if (unknown_game_mode_a3d298 == 0) {
        Unknown_006f24a0(unknown_id_source_a3d290->unknown_2239 == 0);
    } else {
        Unknown_006f25c0();
    }
}


// Rect __fastcall Unknown_00536b80() -- 0x00536B80

Rect __fastcall Unknown_00536b80()
{
    Rect area;
    if (unknown_8523e0 == unknown_screen_widths_7add68[0]) {
        area = Rect(16, 126, 318, 104);
    } else {
        area = Rect(20, 158, 398, 130);
    }
    if (unknown_8523e0 >= unknown_screen_widths_7add68[1]) {
        area.X += (unknown_8523e0 - unknown_screen_widths_7add68[1]) / 2;
        area.Y += (unknown_8523e4 - unknown_screen_heights_7add74[1]) / 2;
    }
    return area;
}


// Rect __fastcall Unknown_00536c00() -- 0x00536C00

Rect __fastcall Unknown_00536c00()
{
    Rect area;
    if (unknown_8523e0 == unknown_screen_widths_7add68[0]) {
        area = Rect(16, 235, 200, 20);
    } else {
        area = Rect(20, 300, 200, 20);
    }
    if (unknown_8523e0 >= unknown_screen_widths_7add68[1]) {
        area.X += (unknown_8523e0 - unknown_screen_widths_7add68[1]) / 2;
        area.Y += (unknown_8523e4 - unknown_screen_heights_7add74[1]) / 2;
    }
    return area;
}


// void __fastcall Unknown_00536c70(text, surface, x, y, width, flags, fill) -- 0x00536C70

void __fastcall Unknown_00536c70(wchar_t const *text, Surface *surface, int x, int y, int width, int flags, bool fill)
{
    int text_width = 0;
    int text_height = 0;
    BitFont->Get_Text_Area(text, &text_width, &text_height, width);
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
    Unknown_Rect_Shrink_006F1B00(&area, -4);
    Unknown_005feb60(&area, surface, 0, 0x9F);
}


// void __fastcall Unknown_00536d30(text, surface, area, color, flags) -- 0x00536D30

void __fastcall Unknown_00536d30(wchar_t const *text, Surface *surface, Rect const &area, unsigned short color, int flags)
{
    RECT rect;
    rect.left = area.X;
    rect.top = area.Y;
    rect.right = area.X + area.Width;
    rect.bottom = area.Y + area.Height;
    RGBClass rgb = Unpack_Hicolor_Pixel(color);
    OwnerDraw::unknown_draw_text_5fe020(surface, text, &rect, BitFont, rgb.Red | (rgb.Green << 8) | (rgb.Blue << 16), flags, 12, 0, 0, 0);
}

// Not exact (RA2_NON_MATCHING): Unknown535910::Unknown_00535BF0 at 0x00535BF0, best score 56.00, 2026-11-13; differs: 1180 instructions in the original, 1195 in the draft (shape 89.09)
#ifdef RA2_NON_MATCHING
// void Unknown535910::Unknown_00535BF0() -- 0x00535BF0 (draws the loading screen)
#include "shared/surface.hpp"
#include "shared/string_table.hpp"
#include "shared/progress_screen.hpp"
#include "shared/palette.hpp"
#include "shared/color_scheme.hpp"
#include "shared/hsv_class.hpp"
#include "shared/unknown_player_entry.hpp"
#include "shared/rgb_globals.hpp"
#include "shared/crt_string.hpp"

void *__fastcall Unknown_Select_Art_6f2400(int width, int height, int side);
void *__fastcall Unknown_Select_Art_6f2420(int width, int height, int side);
void *Unknown_Get_Art_6f2450();
void *Unknown_Get_Art_6f2470();
void *Unknown_Get_Art_6f25b0();
Point2D __fastcall Unknown_Point_Copy_006F1AE0(Point2D const *source);
void __fastcall Unknown_00536c70(wchar_t const *text, Surface *surface, Point2D point, int width, int flags, bool fill);
extern char const *unknown_name_7df370;
extern char const *unknown_name_7df374;
extern char const *unknown_name_7df378;
extern char const *unknown_name_7df37c;
extern char const *unknown_name_7df380;
extern char const *unknown_name_7df384;
extern char const *unknown_name_7df388;
extern char const *unknown_name_7df38c;
extern char const *unknown_name_7df390;
extern char const *unknown_name_7df394;
extern char const *unknown_suffix_7df398;
extern char const *unknown_suffix_7df39c;


inline bool Is_Observer()
{
    return (unknown_game_mode_a3d298 == 3 || unknown_game_mode_a3d298 == 4) && unknown_players_a3fa7c[0]->Team == -1;
}

inline void Loading_Filename(char *filename, int side, char const *suffix)
{
    if (Is_Observer()) {
        sprintf(filename, unknown_name_7df394, suffix);
    } else if (side == 0) {
        sprintf(filename, unknown_name_7df390, suffix);
    } else if (side == 1) {
        sprintf(filename, unknown_name_7df384, suffix);
    } else if (side == 2) {
        sprintf(filename, unknown_name_7df378, suffix);
    } else if (side == 3) {
        sprintf(filename, unknown_name_7df37c, suffix);
    } else if (side == 4) {
        sprintf(filename, unknown_name_7df370, suffix);
    } else if (side == 5) {
        sprintf(filename, unknown_name_7df388, suffix);
    } else if (side == 6) {
        sprintf(filename, unknown_name_7df380, suffix);
    } else if (side == 7) {
        sprintf(filename, unknown_name_7df374, suffix);
    } else if (side == 8) {
        sprintf(filename, unknown_name_7df38c, suffix);
    }
}

inline void Name_Area(Rect &area)
{
    if (unknown_8523e0 == unknown_screen_widths_7add68[0]) {
        area.X = 0x181;
        area.Y = 0x1B4;
    } else {
        area.X = 0x215;
        area.Y = 0x221;
    }
    area.Width = 200;
    area.Height = 20;
    if (unknown_8523e0 >= unknown_screen_widths_7add68[1]) {
        area.X += (unknown_8523e0 - unknown_screen_widths_7add68[1]) / 2;
        area.Y += (unknown_8523e4 - unknown_screen_heights_7add74[1]) / 2;
    }
}

inline Rect Unit_Area()
{
    Rect area;
    if (unknown_8523e0 == unknown_screen_widths_7add68[0]) {
        area.X = 0x10;
        area.Y = 0x48;
    } else {
        area.X = 0x14;
        area.Y = 0x5A;
    }
    area.Width = 200;
    area.Height = 20;
    if (unknown_8523e0 >= unknown_screen_widths_7add68[1]) {
        area.X += (unknown_8523e0 - unknown_screen_widths_7add68[1]) / 2;
        area.Y += (unknown_8523e4 - unknown_screen_heights_7add74[1]) / 2;
    }
    return area;
}

inline void Upper_Case(wchar_t *string)
{
    for (wchar_t *p = string; *p != 0; p++) {
        if ((*p >= 'a' && *p <= 'z') || (*p >= 0xE0 && *p <= 0xFE)) {
            *p -= 0x20;
        }
    }
}

inline wchar_t const *Country_Name(int side)
{
    if (Is_Observer()) {
        return StringTable::Load_String("Name:Observer", 0, "D:\\Ra2\\LdPrgMgr.cpp", 0xA2);
    }
    if (side == 0) {
        return StringTable::Load_String("Name:Americans", 0, "D:\\Ra2\\LdPrgMgr.cpp", 0xA7);
    }
    if (side == 1) {
        return StringTable::Load_String("Name:Alliance", 0, "D:\\Ra2\\LdPrgMgr.cpp", 0xA9);
    }
    if (side == 2) {
        return StringTable::Load_String("Name:French", 0, "D:\\Ra2\\LdPrgMgr.cpp", 0xAB);
    }
    if (side == 3) {
        return StringTable::Load_String("Name:Germans", 0, "D:\\Ra2\\LdPrgMgr.cpp", 0xAD);
    }
    if (side == 4) {
        return StringTable::Load_String("Name:British", 0, "D:\\Ra2\\LdPrgMgr.cpp", 0xAF);
    }
    if (side == 5) {
        return StringTable::Load_String("Name:Africans", 0, "D:\\Ra2\\LdPrgMgr.cpp", 0xB1);
    }
    if (side == 6) {
        return StringTable::Load_String("Name:Arabs", 0, "D:\\Ra2\\LdPrgMgr.cpp", 0xB3);
    }
    if (side == 7) {
        return StringTable::Load_String("Name:Confederation", 0, "D:\\Ra2\\LdPrgMgr.cpp", 0xB5);
    }
    if (side == 8) {
        return StringTable::Load_String("Name:Russians", 0, "D:\\Ra2\\LdPrgMgr.cpp", 0xB7);
    }
    return 0;
}

inline wchar_t const *Unit_Name(int side)
{
    if (side == 0) {
        return StringTable::Load_String("Name:Para", 0, "D:\\Ra2\\LdPrgMgr.cpp", 0xE5);
    }
    if (side == 1) {
        return StringTable::Load_String("Name:BEAGLE", 0, "D:\\Ra2\\LdPrgMgr.cpp", 0xE7);
    }
    if (side == 2) {
        return StringTable::Load_String("Name:GTGCAN", 0, "D:\\Ra2\\LdPrgMgr.cpp", 0xE9);
    }
    if (side == 3) {
        return StringTable::Load_String("Name:TNKD", 0, "D:\\Ra2\\LdPrgMgr.cpp", 0xEB);
    }
    if (side == 4) {
        return StringTable::Load_String("Name:SNIPE", 0, "D:\\Ra2\\LdPrgMgr.cpp", 0xED);
    }
    if (side == 5) {
        return StringTable::Load_String("Name:DTRUCK", 0, "D:\\Ra2\\LdPrgMgr.cpp", 0xEF);
    }
    if (side == 6) {
        return StringTable::Load_String("Name:DESO", 0, "D:\\Ra2\\LdPrgMgr.cpp", 0xF1);
    }
    if (side == 7) {
        return StringTable::Load_String("Name:TERROR", 0, "D:\\Ra2\\LdPrgMgr.cpp", 0xF3);
    }
    if (side == 8) {
        return StringTable::Load_String("Name:TTNK", 0, "D:\\Ra2\\LdPrgMgr.cpp", 0xF5);
    }
    return 0;
}

inline wchar_t const *Briefing(int side)
{
    if (side == 0) {
        return StringTable::Load_String("LoadBrief:USA", 0, "D:\\Ra2\\LdPrgMgr.cpp", 0x116);
    }
    if (side == 1) {
        return StringTable::Load_String("LoadBrief:Korea", 0, "D:\\Ra2\\LdPrgMgr.cpp", 0x118);
    }
    if (side == 2) {
        return StringTable::Load_String("LoadBrief:French", 0, "D:\\Ra2\\LdPrgMgr.cpp", 0x11A);
    }
    if (side == 3) {
        return StringTable::Load_String("LoadBrief:Germans", 0, "D:\\Ra2\\LdPrgMgr.cpp", 0x11C);
    }
    if (side == 4) {
        return StringTable::Load_String("LoadBrief:British", 0, "D:\\Ra2\\LdPrgMgr.cpp", 0x11E);
    }
    if (side == 5) {
        return StringTable::Load_String("LoadBrief:Lybia", 0, "D:\\Ra2\\LdPrgMgr.cpp", 0x120);
    }
    if (side == 6) {
        return StringTable::Load_String("LoadBrief:Iraq", 0, "D:\\Ra2\\LdPrgMgr.cpp", 0x122);
    }
    if (side == 7) {
        return StringTable::Load_String("LoadBrief:Cuba", 0, "D:\\Ra2\\LdPrgMgr.cpp", 0x124);
    }
    if (side == 8) {
        return StringTable::Load_String("LoadBrief:Russia", 0, "D:\\Ra2\\LdPrgMgr.cpp", 0x126);
    }
    return 0;
}

inline int Scheme_Pixel(ColorScheme *scheme)
{
    RGBClass color = scheme->BaseColor.To_RGB();
    return Build_Hicolor_Pixel(color.Red, color.Green, color.Blue);
}

void Unknown535910::Unknown_00535BF0()
{
    wchar_t text[2048];
    wchar_t unit_name[128];
    char filename[32];

    bool small = unknown_8523e0 == unknown_screen_widths_7add68[0];
    unknown_5c = new BSurface(unknown_8523e0, unknown_8523e4, 2);
    if (unknown_5c == 0) {
        return;
    }
    Rect screen(0, 0, 0, 0);
    int background = Build_Hicolor_Pixel(unknown_rgb_ab6841.Red, unknown_rgb_ab6841.Green, unknown_rgb_ab6841.Blue);
    screen.Width = unknown_8523e0;
    screen.Height = unknown_8523e4;
    unknown_5c->Fill_Rect(screen, screen, background);

    if (unknown_game_mode_a3d298 == 0) {
        char const *name = small ? unknown_id_source_a3d290->LS640BkgdName : unknown_id_source_a3d290->LS800BkgdName;
        if (*name != 0) {
        unknown_38 = 0;
        if (unknown_id_source_a3d290->LSLoadMessage[0] != 0) {
            unknown_38 = StringTable::Load_String(unknown_id_source_a3d290->LSLoadMessage, 0, "D:\\Ra2\\LdPrgMgr.cpp", 0x24F);
        }
        unknown_3c = 0;
        if (unknown_id_source_a3d290->LSLoadBriefing[0] != 0) {
            unknown_3c = StringTable::Load_String(unknown_id_source_a3d290->LSLoadBriefing, 0, "D:\\Ra2\\LdPrgMgr.cpp", 0x253);
        }
        int side = ProgressScreen.Get_Side();
        char const *top_name = (char const *)Unknown_Select_Art_6f2400(unknown_8523e0, unknown_8523e4, side);
        char const *bottom_name = (char const *)Unknown_Select_Art_6f2420(unknown_8523e0, unknown_8523e4, side);
        unknown_40 = (void *)Unknown_Load_Shape_00495270(top_name, unknown_4c);
        unknown_48 = (void *)Unknown_Load_Shape_00495270(bottom_name, unknown_4e);
        unknown_44 = (void *)Unknown_Load_Shape_00495270(name, unknown_4d);
        ConvertClass *top_drawer = (ConvertClass *)Unknown_Get_Art_6f2450();
        ConvertClass *drawer = (ConvertClass *)Unknown_Get_Art_6f2470();
        ConvertClass *bottom_drawer = (ConvertClass *)Unknown_Get_Art_6f2490();

        Point2D point(unknown_08.X, unknown_08.Y);
        if (unknown_40 != 0) {
            CC_Draw_Shape(unknown_5c, top_drawer, (ShapeFileStruct const *)unknown_40, 0, point, screen, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
        } else {
            unknown_5c->Fill_Rect(screen, unknown_08, background);
        }
        point.Y = unknown_18.Y;
        CC_Draw_Shape(unknown_5c, drawer, (ShapeFileStruct const *)unknown_44, 0, point, screen, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
        point.Y = unknown_28.Y;
        if (unknown_48 != 0) {
            CC_Draw_Shape(unknown_5c, bottom_drawer, (ShapeFileStruct const *)unknown_48, 0, point, screen, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
        } else {
            unknown_5c->Fill_Rect(screen, unknown_28, background);
        }

        point.X = unknown_08.X + 10;
        point.Y = unknown_08.Y + 10;
        ColorScheme *scheme = ProgressScreen.Side_Color_Scheme();
        if (unknown_38 != 0) {
            Fancy_Text_Printf((wchar_t const *)unknown_38, unknown_5c, &screen, &point, scheme, 0, 0x19);
        }
        if (unknown_3c != 0) {
        if (small) {
            point.X = unknown_id_source_a3d290->LS640BriefLocX + unknown_18.X;
            point.Y = unknown_id_source_a3d290->LS640BriefLocY + unknown_18.Y;
        } else {
            point.X = unknown_id_source_a3d290->LS800BriefLocX + unknown_18.X;
            point.Y = unknown_id_source_a3d290->LS800BriefLocY + unknown_18.Y;
        }
        wcscpy(text, (wchar_t const *)unknown_3c);
        int pixel = Scheme_Pixel(scheme);
        int width = 0;
        int height = 0;
        BitFont->Get_Text_Area(text, &width, &height, 400);
        Rect area(point.X, point.Y, width, height);
        Unknown_00536c70(text, unknown_5c, point, 400, 0, false);
        Unknown_00536d30(text, unknown_5c, area, pixel, 0);
        }
        }
        return;
    }

    {
    int side = unknown_players_a3fa7c[0]->unknown_66e290();
    char const *suffix = unknown_8523e0 == unknown_screen_widths_7add68[0] ? unknown_suffix_7df398 : unknown_suffix_7df39c;
    Loading_Filename(filename, side, suffix);
    unknown_44 = (void *)Unknown_Load_Shape_00495270(filename, unknown_4d);
    ConvertClass *drawer = (ConvertClass *)Unknown_Get_Art_6f25b0();
    Point2D point = Unknown_Point_Copy_006F1AE0((Point2D const *)&unknown_18);
    CC_Draw_Shape(unknown_5c, drawer, (ShapeFileStruct const *)unknown_44, 0, point, screen, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));

    ColorScheme *scheme = ProgressScreen.Text_Color_Scheme();
    RGBClass color = scheme->BaseColor.To_RGB();
    int pixel = Build_Hicolor_Pixel(color.Red, color.Green, color.Blue);

    wchar_t const *name = Country_Name(side);

    Rect area;
    Name_Area(area);
    point = Unknown_Point_Copy_006F1AE0((Point2D const *)&area);
    Unknown_00536c70(name, unknown_5c, point, area.Width, 2, false);
    Unknown_00536d30(name, unknown_5c, area, pixel, 2);

    if (!Is_Observer()) {
        wcscpy(unit_name, Unit_Name(side));
        Upper_Case(unit_name);
        area = Unit_Area();
        int unit_pixel = Build_Hicolor_Pixel(unknown_rgb_ab6841.Red, unknown_rgb_ab6841.Green, unknown_rgb_ab6841.Blue);
        Unknown_00536d30(unit_name, unknown_5c, area, unit_pixel, 0);

        wchar_t const *brief = Briefing(side);
        area = Unknown_00536b80();
        point = Unknown_Point_Copy_006F1AE0((Point2D const *)&area);
        Unknown_00536c70(brief, unknown_5c, point, area.Width, 0, true);
        Unknown_00536d30(brief, unknown_5c, area, pixel, 0);
    }

    wchar_t const *loading = StringTable::Load_String("GUI:LoadingEx", 0, "D:\\Ra2\\LdPrgMgr.cpp", 0x2ED);
    area = Unknown_00536c00();
    point = Unknown_Point_Copy_006F1AE0((Point2D const *)&area);
    Unknown_00536c70(loading, unknown_5c, point, area.Width, 0, false);
    Unknown_00536d30(loading, unknown_5c, area, pixel, 0);
    }
}
#endif

