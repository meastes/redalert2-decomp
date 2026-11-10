// AlphaShapeClass::Draw_In_Area (0x0041F1C0, 1,048 B; best exact draft 41.92
// was never kept) written from the seed decompile with TibSun alphashp.cpp's
// shape (Scout1103, native M10). NOT EXACT; never original code or bytes.
// For each live alpha shape whose screen rectangle meets cliprect and the
// 60x30 cell at pixel, its frame 0 pixels scale the depth buffer values
// (AlphaBuffer 0x008315B4, as the decompile reads it) through the brightness
// table 0x0083C670 under the iso-cell mask (0x0079B850, 60 x 30 bytes, ' '
// outside the diamond; read from game.exe at run time like other tables).
#include "shared/alpha_shape.hpp"
#include "shared/shape_file.hpp"
#include "shared/blit.hpp"
#include "shared/tactical.hpp"
#include "shared/display.hpp"
#include "shared/vector_class.hpp"

extern DynamicVectorClass<AlphaShapeClass *> unknown_AlphaShapeClass_list_0083c648;
namespace ra2port { void *original_data_pointer(uint32_t va); }

void __fastcall AlphaShapeClass::Draw_In_Area(Point2D const &pixel, Rect const &cliprect)
{
    static unsigned char const *tilemask = 0;
    if (tilemask == 0) {
        tilemask = (unsigned char const *)ra2port::original_data_pointer(0x0079B850);
        if (tilemask == 0) return;
    }
    ABuffer *buffer = AlphaBuffer;
    if (buffer == 0) return;
    for (int index = 0; index < unknown_AlphaShapeClass_list_0083c648.Count(); index++) {
        AlphaShapeClass *alpha = unknown_AlphaShapeClass_list_0083c648[index];
        if (alpha->unknown_3c) continue;
        int x1 = alpha->Bounds.X - unknown_tactical_83996c->TacPixelX + unknown_rect_839630.X;
        int y1 = alpha->Bounds.Y - unknown_tactical_83996c->TacPixelY + unknown_rect_839630.Y;
        Rect r1(x1, y1, alpha->Bounds.Width, alpha->Bounds.Height);
        Rect r2 = Intersect(r1, cliprect);
        Rect r3(pixel.X, pixel.Y, 60, 30);
        Rect r4 = Intersect(r2, r3);
        if (r4.Width <= 0 || r4.Height <= 0) continue;
        int dx = r4.X - r3.X;
        int dy = r4.Y - r3.Y;
        ShapeFileStruct const *shape = alpha->Image;
        if (shape == 0) continue;
        Rect frame = shape->Get_Frame_Rect(0);
        unsigned char const *data = (unsigned char const *)shape->Get_Frame_Data(0);
        if (data == 0) continue;
        int top = r4.Y > frame.Y + y1 ? r4.Y : frame.Y + y1;
        int bottom = r4.Y + r4.Height < frame.Y + frame.Height + y1 ? r4.Y + r4.Height : frame.Y + frame.Height + y1;
        int left = r4.X > frame.X + x1 ? r4.X : frame.X + x1;
        int right = r4.X + r4.Width < frame.X + frame.Width + x1 ? r4.X + r4.Width : frame.X + frame.Width + x1;
        int src_x = left - frame.X - x1;
        int src_y = top - frame.Y - y1;
        if (left >= right || top >= bottom) continue;
        int shape_skip = frame.Width - right + left;
        int alpha_skip = buffer->Width - right + left;
        int mask_skip = 60 - r4.Width;
        unsigned char const *maskptr = &tilemask[60 * dy + dx];
        unsigned char const *shapeptr = &data[src_x + src_y * frame.Width];
        unsigned short *alphaptr = buffer->Get_Buffer_Offset(Point2D(left, top - unknown_rect_839630.Y));
        for (int y = top; y < bottom; y++) {
            for (int x = left; x < right; x++) {
                unsigned char value = *shapeptr++;
                if (*maskptr++ != ' ') {
                    *alphaptr = unknown_alpha_table_83c670[*alphaptr + value * 256];
                }
                alphaptr = buffer->Wrap_Pointer(alphaptr + 1);
            }
            shapeptr += shape_skip;
            maskptr += mask_skip;
            alphaptr = buffer->Wrap_Pointer(alphaptr + alpha_skip);
        }
    }
}
