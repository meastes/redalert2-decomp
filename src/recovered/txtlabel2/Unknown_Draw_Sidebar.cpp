// void Unknown_Draw_Sidebar_006F4620(Surface *surface, Rect const &window, bool tabs) (0x006F4620)
// Loads the sidebar art once (0x006F4B00, the SIDEBAR/UIBKGD palettes of
// 0x006F4C60 expanded inline, the layout 0x006F4C90), clears the surface and
// draws the sidebar pieces at their layout rectangles; the bottom bar is
// clipped to the tactical width, and `tabs` selects the repeated tab strip.
#define SHAPE_FILE_EMPTY_RECT Rect(0, 0, 0, 0)
#include "shared/shape_file.hpp"
#include "shared/surface.hpp"
#include "shared/shell_art.hpp"

class ConvertClass;
void __fastcall CC_Draw_Shape(Surface *surface, ConvertClass *drawer, ShapeFileStruct const *shape, int frame,
                              Point2D const &point, Rect const &window, int flags, unsigned char const *unknown8,
                              int unknown9, int unknown10, int brightness, ShapeFileStruct const *unknown12,
                              int unknown13, Point2D offset);   // 0x0049EEA0
void __fastcall Unknown_Load_Art_Buffers_006F4B00();
void __fastcall Unknown_Load_Art_Pairs_006F4C60();
void __fastcall Unknown_Layout_Sidebar_006F4C90(int width, int height);
extern bool unknown_flag_ac11b0;
extern int VideoModeWidth;                 // screen width
extern int VideoModeHeight;                 // screen height
extern ConvertClass *unknown_drawer_ac11a4;      // SIDEBAR.PAL
extern ConvertClass *unknown_drawer_ac11ac;      // UIBKGD.PAL
extern int unknown_value_ac10cc;
extern int unknown_value_ac0ff4;
extern ShapeFileStruct *unknown_shape_ac10c4;
extern ShapeFileStruct *unknown_shape_ac10bc;
extern ShapeFileStruct *unknown_shape_ac105c;
extern ShapeFileStruct *unknown_shape_ac10f8;
extern ShapeFileStruct *unknown_shape_ac0ff0;
extern ShapeFileStruct *unknown_shape_ac1070;
extern ShapeFileStruct *unknown_shape_ac1078;
extern ShapeFileStruct *unknown_shape_ac100c;
extern ShapeFileStruct *unknown_shape_ac1094;
extern ShapeFileStruct *unknown_shape_ac1058;
extern ShapeFileStruct *unknown_shape_ac1048;
extern ShapeFileStruct *unknown_shape_ac1098;
extern ShapeFileStruct *unknown_shape_ac10a4;
extern ShapeFileStruct *unknown_shape_ac10b0;
extern Rect *unknown_rect_ac11d4;
extern Rect *unknown_rect_ac11d8;
extern Rect *unknown_rect_ac11dc;
extern Rect *unknown_rect_ac11e0;
extern Rect *unknown_rect_ac11e8;
extern Rect *unknown_rect_ac11ec;
extern Rect *unknown_rect_ac11f0;
extern Rect *unknown_rect_ac11f4;
extern Rect *unknown_rect_ac1200;
extern Rect *ThumbOpenRect;
extern Rect *ThumbClosedRect;
extern Rect *ButtonSlotRect;
extern Rect *TabEndRect;

void __fastcall Unknown_Draw_Sidebar_006F4620(Surface *surface, Rect const &window, bool tabs)
{
    if (!unknown_flag_ac11b0) {
        Unknown_Load_Art_Buffers_006F4B00();
        Unknown_Load_Art_Pairs_006F4C60();
        Unknown_Layout_Sidebar_006F4C90(VideoModeWidth, VideoModeHeight);
        unknown_flag_ac11b0 = true;
    }
    surface->Fill(0);
    Point2D point(0, 0);
    point.X = unknown_rect_ac11d4->X;
    point.Y = unknown_rect_ac11d4->Y;
    if (VideoModeWidth == 640) {
        CC_Draw_Shape(surface, unknown_drawer_ac11ac, unknown_shape_ac10c4, 0, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
    } else if (VideoModeWidth == 800) {
        CC_Draw_Shape(surface, unknown_drawer_ac11ac, unknown_shape_ac10bc, 0, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
    } else {
        CC_Draw_Shape(surface, unknown_drawer_ac11ac, unknown_shape_ac105c, 0, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
    }
    point.X = unknown_rect_ac11d8->X;
    point.Y = unknown_rect_ac11d8->Y;
    CC_Draw_Shape(surface, unknown_drawer_ac11a4, unknown_shape_ac10f8, 0, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
    point.X = unknown_rect_ac11dc->X;
    point.Y = unknown_rect_ac11dc->Y;
    CC_Draw_Shape(surface, unknown_drawer_ac11a4, unknown_shape_ac0ff0, 0, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
    point.X = unknown_rect_ac11e0->X;
    point.Y = unknown_rect_ac11e0->Y;
    CC_Draw_Shape(surface, unknown_drawer_ac11a4, unknown_shape_ac1070, 0, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
    point.X = unknown_rect_ac11e8->X;
    point.Y = unknown_rect_ac11e8->Y;
    CC_Draw_Shape(surface, unknown_drawer_ac11a4, unknown_shape_ac1078, 0, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
    int index = 0;
    point.X = unknown_rect_ac11ec->X;
    point.Y = unknown_rect_ac11ec->Y;
    if (unknown_value_ac10cc > 0) {
        do {
            CC_Draw_Shape(surface, unknown_drawer_ac11a4, unknown_shape_ac100c, 0, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            point.Y += unknown_rect_ac11ec->Height;
            index++;
        } while (index < unknown_value_ac10cc);
    }
    point.X = unknown_rect_ac11f0->X;
    point.Y = unknown_rect_ac11f0->Y;
    CC_Draw_Shape(surface, unknown_drawer_ac11a4, unknown_shape_ac1094, 0, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
    point.X = unknown_rect_ac11f4->X;
    point.Y = unknown_rect_ac11f4->Y;
    CC_Draw_Shape(surface, unknown_drawer_ac11a4, unknown_shape_ac1058, 0, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
    Rect clip = window;
    clip.Width = Unknown_Bar_Rect().Width;
    point.X = unknown_rect_ac1200->X;
    point.Y = unknown_rect_ac1200->Y;
    CC_Draw_Shape(surface, unknown_drawer_ac11a4, unknown_shape_ac1048, 0, point, clip, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
    if (tabs) {
        point.X = ThumbOpenRect->X;
        point.Y = ThumbOpenRect->Y;
        CC_Draw_Shape(surface, unknown_drawer_ac11a4, unknown_shape_ac1098, 2, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
        index = 0;
        point.X = ButtonSlotRect->X;
        point.Y = ButtonSlotRect->Y;
        if (unknown_value_ac0ff4 > 0) {
            do {
                CC_Draw_Shape(surface, unknown_drawer_ac11a4, unknown_shape_ac10a4, 0, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
                point.X += ButtonSlotRect->Width;
                index++;
            } while (index < unknown_value_ac0ff4);
        }
    } else {
        point.X = ThumbClosedRect->X;
        point.Y = ThumbClosedRect->Y;
        CC_Draw_Shape(surface, unknown_drawer_ac11a4, unknown_shape_ac1098, 2, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
    }
    point.X = TabEndRect->X;
    point.Y = TabEndRect->Y;
    CC_Draw_Shape(surface, unknown_drawer_ac11a4, unknown_shape_ac10b0, 0, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
}
