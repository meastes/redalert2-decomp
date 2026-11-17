// void __fastcall Unknown_Draw_Shell_Shape_Second_Frame_6F3AC0(Surface *surface, Rect const &window) -- 0x6F3AC0
// Draws frame 1 of the shape at 0x00AC10E8 (hypothesis).
#include "shared/rect.hpp"

class Surface;
class ConvertClass;
struct ShapeFileStruct;
void __fastcall CC_Draw_Shape(Surface *surface, ConvertClass *drawer, ShapeFileStruct const *shape, int frame,
                              Point2D const &point, Rect const &window, int flags, unsigned char const *unknown8,
                              int unknown9, int unknown10, int brightness, ShapeFileStruct const *unknown12,
                              int unknown13, Point2D offset);   // 0x0049EEA0
extern Point2D *unknown_point_ac11c4;
extern ConvertClass *unknown_drawer_ac1188;
extern ShapeFileStruct *unknown_shape_ac10e8;

void __fastcall Unknown_Draw_Shell_Shape_Second_Frame_6F3AC0(Surface *surface, Rect const &window)
{
    Point2D point(0, 0);
    point.X = unknown_point_ac11c4->X;
    point.Y = unknown_point_ac11c4->Y;
    CC_Draw_Shape(surface, unknown_drawer_ac1188, unknown_shape_ac10e8, 1, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
}
