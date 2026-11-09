// NON-EXACT DRAFT 0x535BF0 FUN_00535bf0
// entry 0x535BF0; exact-match score 90.55 (misses_index best); plan "draft, verify"
// origin lane: Scout1024 (build/class-work/Scout1024/keep/ld_u3_9055.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1061 (native/drafts.json). Not original bytes.
// @ref ?Unknown_00536c70@@YIXPBGPAVSurface@@HHHH_N@Z=00536c70
// @ref ??_C@_09HFNC@Name?3Para?$AA@=007df574
// @ref ??_C@_09IFOG@Name?3DESO?$AA@=007df52c
// @ref ??_C@_09JKBK@Name?3TTNK?$AA@=007df514
// @ref ??_C@_09MCLF@Name?3TNKD?$AA@=007df550
// @ref ??_C@_0BA@FLJL@LoadBrief?3Lybia?$AA@=007df4a8
// @ref ??_C@_0BA@HPDI@LoadBrief?3Korea?$AA@=007df4f4
// @ref ??_C@_0BB@NIEF@LoadBrief?3Russia?$AA@=007df474
// @ref ??_C@_0BB@PDEN@LoadBrief?3French?$AA@=007df4e0
// @ref ??_C@_0BC@GFGG@LoadBrief?3Germans?$AA@=007df4cc
// @ref ??_C@_0BC@PLAN@LoadBrief?3British?$AA@=007df4b8
// @ref ??_C@_0BD@KOEF@Name?3Confederation?$AA@=007df590
// @ref ??_C@_0BE@JOJM@D?3?2Ra2?2LdPrgMgr?4cpp?$AA@=007df61c
// @ref ??_C@_0L@DFJF@Name?3Arabs?$AA@=007df5a4
// @ref ??_C@_0L@HDBN@Name?3SNIPE?$AA@=007df544
// @ref ??_C@_0M@BNEJ@Name?3GTGCAN?$AA@=007df55c
// @ref ??_C@_0M@DBMD@Name?3French?$AA@=007df5e0
// @ref ??_C@_0M@FIKD@Name?3TERROR?$AA@=007df520
// @ref ??_C@_0M@KGBB@Name?3DTRUCK?$AA@=007df538
// @ref ??_C@_0M@LJPK@Name?3BEAGLE?$AA@=007df568
// @ref ??_C@_0N@DAKE@Name?3Germans?$AA@=007df5d0
// @ref ??_C@_0N@KOMP@Name?3British?$AA@=007df5c0
// @ref ??_C@_0O@DDAL@LoadBrief?3USA?$AA@=007df504
// @ref ??_C@_0O@GGAG@Name?3Observer?$AA@=007df60c
// @ref ??_C@_0O@IEEH@Name?3Russians?$AA@=007df580
// @ref ??_C@_0O@JKPL@Name?3Alliance?$AA@=007df5ec
// @ref ??_C@_0O@PCJI@Name?3Africans?$AA@=007df5b0
// @ref ??_C@_0P@DMJM@LoadBrief?3Iraq?$AA@=007df498
// @ref ??_C@_0P@HAEG@LoadBrief?3Cuba?$AA@=007df488
// @ref ??_C@_0P@NIKA@Name?3Americans?$AA@=007df5fc
// @ref ?unknown_name_7df370@@3PBDB=007df370
// @ref ?unknown_name_7df374@@3PBDB=007df374
// @ref ?unknown_name_7df378@@3PBDB=007df378
// @ref ?unknown_name_7df37c@@3PBDB=007df37c
// @ref ?unknown_name_7df380@@3PBDB=007df380
// @ref ?unknown_name_7df384@@3PBDB=007df384
// @ref ?unknown_name_7df388@@3PBDB=007df388
// @ref ?unknown_name_7df38c@@3PBDB=007df38c
// @ref ?unknown_name_7df390@@3PBDB=007df390
// @ref ?unknown_name_7df394@@3PBDB=007df394
// @ref ?unknown_suffix_7df398@@3PBDB=007df398
// @ref ?unknown_suffix_7df39c@@3PBDB=007df39c
// void Unknown535910::Unknown_00535BF0() -- 0x00535BF0 (draws the loading screen)
#include "shared/unknown_globals.hpp"
#include "shared/rect.hpp"
#include "shared/surface.hpp"
#include "shared/hicolor.hpp"
#include "shared/text_print.hpp"
#include "shared/string_table.hpp"
#include "shared/progress_screen.hpp"
#include "shared/palette.hpp"
#include "shared/color_scheme.hpp"
#include "shared/hsv_class.hpp"
#include "shared/id_source.hpp"
#include "shared/unknown_player_entry.hpp"
#include "shared/rgb_globals.hpp"
#include "shared/crt_string.hpp"
#include "shared/win32_api.hpp"

void *__fastcall Unknown_Select_Art_6f2400(int width, int height, int side);
void *__fastcall Unknown_Select_Art_6f2420(int width, int height, int side);
void *Unknown_Get_Art_6f2450();
void *Unknown_Get_Art_6f2470();
void *Unknown_Get_Art_6f25b0();
Point2D __fastcall Unknown_Point_Copy_006F1AE0(Point2D const *source);
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

inline Rect Name_Area()
{
    Rect area;
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
    return area;
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
        if (*name == 0) {
            return;
        }
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
            Fancy_Text_Printf(unknown_38, unknown_5c, &screen, &point, scheme, 0, 0x19);
        }
        if (unknown_3c == 0) {
            return;
        }
        if (small) {
            point.X = unknown_id_source_a3d290->LS640BriefLocX + unknown_18.X;
            point.Y = unknown_id_source_a3d290->LS640BriefLocY + unknown_18.Y;
        } else {
            point.X = unknown_id_source_a3d290->LS800BriefLocX + unknown_18.X;
            point.Y = unknown_id_source_a3d290->LS800BriefLocY + unknown_18.Y;
        }
        wcscpy(text, unknown_3c);
        int pixel = Scheme_Pixel(scheme);
        int width = 0;
        int height = 0;
        BitFont->Get_Text_Area(text, &width, &height, 400);
        Rect area(point.X, point.Y, width, height);
        Unknown_00536c70(text, unknown_5c, point.X, point.Y, 400, 0, false);
        Unknown_00536d30(text, unknown_5c, area, pixel, 0);
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

    RGBClass color = ProgressScreen.Text_Color_Scheme()->BaseColor.To_RGB();
    int pixel = Build_Hicolor_Pixel(color.Red, color.Green, color.Blue);

    wchar_t const *name = Country_Name(side);

    Rect area = Name_Area();
    point = Unknown_Point_Copy_006F1AE0((Point2D const *)&area);
    Unknown_00536c70(name, unknown_5c, point.X, point.Y, area.Width, 2, false);
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
        Unknown_00536c70(brief, unknown_5c, point.X, point.Y, area.Width, 0, true);
        Unknown_00536d30(brief, unknown_5c, area, pixel, 0);
    }

    wchar_t const *loading = StringTable::Load_String("GUI:LoadingEx", 0, "D:\\Ra2\\LdPrgMgr.cpp", 0x2ED);
    area = Unknown_00536c00();
    point = Unknown_Point_Copy_006F1AE0((Point2D const *)&area);
    Unknown_00536c70(loading, unknown_5c, point.X, point.Y, area.Width, 0, false);
    Unknown_00536d30(loading, unknown_5c, area, pixel, 0);
    }
}
