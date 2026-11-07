// @ref ?unknown_control_table_a73640@@3V?$WolDictionary@IUUnknownControlRecord@OwnerDraw@@@@A=00a73640
// @ref ?Unknown_Get_Art_006f3480@@YIPAXXZ=006f3480
// @ref ?Unknown_Get_Art_006f34c0@@YIPAXXZ=006f34c0
// @ref ?Unknown_Get_Art_6f2830@@YAPAXXZ=006f2830
// @ref ?unknown_shape_ac10e8@@3PAUShapeFileStruct@@A=00ac10e8
// @ref ?unknown_shape_ac10b8@@3PAUShapeFileStruct@@A=00ac10b8
// @ref ?unknown_shape_ac0fec@@3PAUShapeFileStruct@@A=00ac0fec
// @ref ?unknown_shape_ac10b4@@3PAUShapeFileStruct@@A=00ac10b4
// @ref ?unknown_shape_ac10f0@@3PAUShapeFileStruct@@A=00ac10f0
// @ref ?unknown_shape_ac110c@@3PAUShapeFileStruct@@A=00ac110c
// @ref ?unknown_count_ac102c@@3HA=00ac102c
// @ref ?unknown_rect_ac11b4@@3PAVRect@@A=00ac11b4
// @ref ?unknown_rect_ac11b8@@3PAVRect@@A=00ac11b8
// @ref ?unknown_rect_ac11bc@@3PAVRect@@A=00ac11bc
// @ref ?unknown_rect_ac11c0@@3PAVRect@@A=00ac11c0
// @ref ?unknown_rect_ac11c4@@3PAVRect@@A=00ac11c4
// @ref ?unknown_rect_ac11cc@@3PAVRect@@A=00ac11cc
// @ref ?unknown_surface_839950@@3PAVSurface@@A=00839950
// @ref ?unknown_surface_839958@@3PAVSurface@@A=00839958
// @ref ?Window_Rect_In_Main_Client@@YIHPAUHWND__@@PAURECT@@@Z=00735690
// @ref __imp__GetWindowRect@8=0079a398
// @ref __imp__EnumChildWindows@12=0079a4dc
// @ref __imp__SendMessageA@16=0079a4a0
// @ref __imp__Sleep@4=0079a1e4
// @ref ?unknown_5e7d00@OwnerDraw@@YGHPAUHWND__@@J@Z=005e7d00
// @ref ?unknown_5e7dd0@OwnerDraw@@YGHPAUHWND__@@J@Z=005e7dd0
// @ref ?unknown_visible_count_a737e4@OwnerDraw@@3HA=00a737e4
// @ref ?unknown_visible_count_a763b4@OwnerDraw@@3HA=00a763b4
// @ref ??2@YAPAXI@Z=00782017
// @ref ??3@YAXPAX@Z=00781d3b
// @ref ?Unknown_Point_Copy_006F1AE0@@YI?AUPoint2D@@PBU1@@Z=006f1ae0
// @ref ?CC_Draw_Shape@@YIXPAVSurface@@PAVConvertClass@@PBUShapeFileStruct@@HABUPoint2D@@ABVRect@@HHHHHHHU4@@Z=0049eea0
// @ref ?unknown_8523e0@@3HA=008523e0
// @ref ?unknown_7add6c@OwnerDraw@@3HA=007add6c
// @ref ?Audio_Update@@YAXXZ=00406ea0
// @ref ?Voc_Play@@YIPAUAudioEvent@@HMHPAUAudioEventHandle@@@Z=00713db0
// @ref ?Rules@@3PAVRulesClass@@A=00839848
// @ref ?WWMouse@@3PAVMouse@@A=00839c68
#include "shared/win32_api.hpp"
#include "shared/win32_window.hpp"
#include "shared/owner_draw_dialog_control.hpp"
#include "shared/rect.hpp"
#include "shared/surface.hpp"
#include "shared/palette.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/rules.hpp"
#include "shared/sound_play.hpp"
#include "shared/mouse.hpp"

void *__fastcall Unknown_Get_Art_006f3480();
void *__fastcall Unknown_Get_Art_006f34c0();
void *Unknown_Get_Art_6f2830();
Point2D __fastcall Unknown_Point_Copy_006F1AE0(Point2D const *source);
void __cdecl Audio_Update();
extern ShapeFileStruct *unknown_shape_ac10e8;
extern ShapeFileStruct *unknown_shape_ac10b8;
extern ShapeFileStruct *unknown_shape_ac0fec;
extern ShapeFileStruct *unknown_shape_ac10b4;
extern ShapeFileStruct *unknown_shape_ac10f0;
extern ShapeFileStruct *unknown_shape_ac110c;
extern int unknown_count_ac102c;
extern Rect *unknown_rect_ac11b4;
extern Rect *unknown_rect_ac11b8;
extern Rect *unknown_rect_ac11bc;
extern Rect *unknown_rect_ac11c0;
extern Rect *unknown_rect_ac11c4;
extern Rect *unknown_rect_ac11cc;

namespace OwnerDraw {

BOOL __stdcall unknown_5e7d00(HWND window, LPARAM);
BOOL __stdcall unknown_5e7dd0(HWND window, LPARAM);

// Visible owner-drawn buttons of the dialog (inline; ra2-decomp's FUN_0060A230 in YR).
inline int Visible_Count_A737E4(HWND dialog)
{
    unknown_visible_count_a737e4 = 0;
    EnumChildWindows(dialog, unknown_5e7d00, 0);
    return unknown_visible_count_a737e4;
}

// Whether any 0x005E7DD0 control is visible (inline; ra2-decomp's FUN_0060A300 in YR).
inline bool Any_Visible_A763B4(HWND dialog)
{
    unknown_visible_count_a763b4 = 0;
    EnumChildWindows(dialog, unknown_5e7dd0, 0);
    return unknown_visible_count_a763b4 >= 1;
}

// A piece's slide phase at a step: -1 before it starts, -2 once its six frames are done.
inline int Slide_Phase(int delta)
{
    if (delta < 0) return -1;
    if (delta >= 6) return -2;
    return delta;
}

// The latest start step in a piece order table.
inline int Last_Step(int const *order, int pieces)
{
    int steps = 0;
    for (int j = 0; j < pieces; j++) {
        if (steps <= order[j]) steps = order[j];
    }
    return steps;
}

// Slides the dialog's frame pieces in (reverse) or out, one piece after another, then notifies the
// dialog with 0x4EC (in) or 0x4ED (out) (0x005E5150; YR DrawCampaignMenuTransition).
void __fastcall unknown_5e5150(HWND dialog, bool reverse)
{
    UnknownControlRecord *record = Get_Record(dialog);
    bool has_d7 = record ? record->unknown_d7 : false;
    record = Get_Record(dialog);
    bool has_d6 = record ? record->unknown_d6 : false;
    record = Get_Record(dialog);
    bool has_d5 = record ? record->unknown_d5 : false;

    ConvertClass *drawer = (ConvertClass *)Unknown_Get_Art_006f3480();
    ConvertClass *drawer_b = (ConvertClass *)Unknown_Get_Art_006f34c0();
    ShapeFileStruct *shape_e8 = unknown_shape_ac10e8;
    ShapeFileStruct *shape_b8 = unknown_shape_ac10b8;
    ShapeFileStruct *shape_ec = unknown_shape_ac0fec;
    ShapeFileStruct *shape_b4 = unknown_shape_ac10b4;
    ConvertClass *drawer_c = (ConvertClass *)Unknown_Get_Art_6f2830();
    ShapeFileStruct *shape_f0 = unknown_shape_ac10f0;
    int count = unknown_count_ac102c;
    Rect window = unknown_surface_839958->Get_Rect();
    Rect rect_c4 = *unknown_rect_ac11c4;
    Rect rect_b8 = *unknown_rect_ac11b8;
    Rect rect_bc = *unknown_rect_ac11bc;
    Rect rect_b4 = *unknown_rect_ac11b4;
    Rect rect_c0 = *unknown_rect_ac11c0;

    RECT client;
    Window_Rect_In_Main_Client(dialog, &client);
    RECT screen;
    GetWindowRect(dialog, &screen);
    int dx = screen.left - client.left;
    int dy = screen.top - client.top;

    int x = rect_b4.X;
    int y = rect_b4.Y;
    int width = rect_b4.Width;
    int height = rect_b4.Height * count;
    if (has_d5) {
        x = rect_bc.X;
        width = rect_bc.Width;
        y = rect_bc.Y;
        height += rect_b4.Y;
    }
    Rect from(x, y, width, height);
    Rect to = from;
    to.X += dx;
    to.Y += dy;
    Rect from_c0 = rect_c0;
    Rect to_c0 = from_c0;
    to_c0.X += dx;
    to_c0.Y += dy;

    int dir = reverse ? -1 : 1;
    int base_64 = 5;
    if (reverse) base_64 = 10;
    int base_8c = 11;
    if (reverse) base_8c = 16;
    int base_94 = 1;
    if (reverse) base_94 = 6;
    int base_88 = 0;
    if (reverse) base_88 = 5;
    int base_90 = 0;
    if (reverse) base_90 = 5;

    int buttons = Visible_Count_A737E4(dialog);
    int last = count - 1;
    int top = rect_b4.Y;
    if (has_d6) top += rect_b4.Height;
    bool any = Any_Visible_A763B4(dialog);
    if (!any) last++;

    // One start step per piece: the button rows, then the d6, d5 and d7 frame pieces.
    int pieces = count + 3;
    int limit = pieces - 2;
    int *order = new int[pieces];
    int total = count + 3;  // recounted after the allocation (the original recomputes count + 3 here)
    int value = 1;
    for (int i = 0; i < limit; i++) {
        order[i] = value++;
    }
    order[total - 1] = 0;
    order[limit] = value + 1;
    order[pieces - 3] = 0;
    int steps = Last_Step(order, pieces) + 6;

    for (int step = 0; step < steps; step++) {
        if (has_d6) {
            Point2D point = Unknown_Point_Copy_006F1AE0((Point2D const *)&rect_b8);
            int phase = Slide_Phase(step - order[pieces - 3]);
            if (phase == -1) {
                CC_Draw_Shape(unknown_surface_839958, drawer, shape_ec, reverse ? 6 : 0, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            } else if (phase == -2) {
                if (!reverse) {
                    CC_Draw_Shape(unknown_surface_839958, drawer, shape_ec, 6, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
                }
            }
        }
        if (has_d5) {
            Point2D point = Unknown_Point_Copy_006F1AE0((Point2D const *)&rect_bc);
            int phase = Slide_Phase(step - order[pieces - 2]);
            if (phase == -1) {
                CC_Draw_Shape(unknown_surface_839958, drawer, shape_e8, !reverse, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            } else if (phase == -2) {
                CC_Draw_Shape(unknown_surface_839958, drawer, shape_e8, reverse != 0, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            } else {
                point = Unknown_Point_Copy_006F1AE0((Point2D const *)&rect_c4);
                CC_Draw_Shape(unknown_surface_839958, drawer, shape_e8, 1, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
                point = Unknown_Point_Copy_006F1AE0((Point2D const *)&rect_bc);
                CC_Draw_Shape(unknown_surface_839958, drawer, shape_b4, phase * dir + base_88, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            }
        }
        if (has_d6) {
            Point2D point = Unknown_Point_Copy_006F1AE0((Point2D const *)&rect_b8);
            int phase = Slide_Phase(step - order[pieces - 3]);
            if (phase == -2) {
                if (reverse) {
                    CC_Draw_Shape(unknown_surface_839958, drawer, shape_ec, 0, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
                }
            } else {
                CC_Draw_Shape(unknown_surface_839958, drawer, shape_ec, phase * dir + base_94, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            }
        }
        int k;
        for (k = 0; k < buttons; k++) {
            Point2D point(rect_b4.X, k * rect_b4.Height + top);
            int phase = Slide_Phase(step - order[k]);
            if (phase == -1) {
                CC_Draw_Shape(unknown_surface_839958, drawer_b, shape_b8, reverse ? 10 : 1, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            } else if (phase == -2) {
                CC_Draw_Shape(unknown_surface_839958, drawer_b, shape_b8, reverse ? 1 : 10, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            } else {
                CC_Draw_Shape(unknown_surface_839958, drawer_b, shape_b8, phase * dir + base_64, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            }
        }
        for (k = buttons; k < last; k++) {
            Point2D point(rect_b4.X, k * rect_b4.Height + top);
            int phase = Slide_Phase(step - order[k]);
            if (phase == -1) {
                CC_Draw_Shape(unknown_surface_839958, drawer_b, shape_b8, reverse ? 10 : 0, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            } else if (phase == -2) {
                CC_Draw_Shape(unknown_surface_839958, drawer_b, shape_b8, reverse ? 0 : 10, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            } else {
                CC_Draw_Shape(unknown_surface_839958, drawer_b, shape_b8, phase * dir + base_8c, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            }
        }
        if (any) {
            Point2D point(rect_b4.X, unknown_rect_ac11cc->Y - rect_b4.Height);
            int phase = Slide_Phase(step - order[count - 1]);
            if (phase == -1) {
                CC_Draw_Shape(unknown_surface_839958, drawer_b, shape_b8, reverse ? 10 : 1, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            } else if (phase == -2) {
                CC_Draw_Shape(unknown_surface_839958, drawer_b, shape_b8, reverse ? 1 : 10, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            } else {
                CC_Draw_Shape(unknown_surface_839958, drawer_b, shape_b8, phase * dir + base_64, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            }
        }
        if (has_d7) {
            Point2D point = Unknown_Point_Copy_006F1AE0((Point2D const *)&rect_c0);
            int phase = Slide_Phase(step - order[total - 1]);
            if (phase == -1) {
                if (!reverse) {
                    if (unknown_8523e0 >= unknown_7add6c) point.X += 0x50;
                }
                CC_Draw_Shape(unknown_surface_839958, drawer_c, (reverse ? unknown_shape_ac110c : shape_f0), 0, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            } else if (phase == -2) {
                if (reverse) {
                    if (unknown_8523e0 >= unknown_7add6c) point.X += 0x50;
                }
                CC_Draw_Shape(unknown_surface_839958, drawer_c, (reverse ? shape_f0 : unknown_shape_ac110c), 0, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            } else {
                if (unknown_8523e0 >= unknown_7add6c) point.X += 0x50;
                CC_Draw_Shape(unknown_surface_839958, drawer_c, shape_f0, phase * dir + base_90, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            }
        }
        unknown_surface_839950->Lock();
        unknown_surface_839958->Lock();
        unknown_surface_839950->Copy_From(to, *unknown_surface_839958, from, false, true);
        if (has_d7) {
            unknown_surface_839950->Copy_From(to_c0, *unknown_surface_839958, from_c0, false, true);
        }
        unknown_surface_839958->Unlock();
        unknown_surface_839950->Unlock();
        Audio_Update();
        Sleep(30);
    }
    delete [] order;
    if (reverse) {
        Voc_Play(Rules->ShellButtonSlideSound, 1.0f, 0x2000, 0);
        while (WWMouse->Get_Mouse_State()) {
            WWMouse->Show_Mouse();
        }
        SendMessageA(dialog, 0x4ec, 0, 0);
    } else {
        SendMessageA(dialog, 0x4ed, 0, 0);
    }
}

}
