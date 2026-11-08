// NON-EXACT DRAFT 0x41F5E0 AlphaShapeClass::Draw_All
// entry 0x41F5E0; exact-match score 34.72 (misses_index best); plan "draft, verify"
// origin lane: Scout1000 (build/class-work/Scout1000/w/a2.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1089 (native/drafts.json). Not original bytes.
// Native adaptation (Scout1089): defined under its callers' name Unknown_0041F5E0 (Tactical 0x006A7820) instead of the overlay's AlphaShapeClass::Draw_All declaration; marked NATIVE.
// void AlphaShapeClass::Draw_All(Rect const &cliprect) (0x0041F5E0)
// Blends every shape into the alpha (depth) buffer; a shape without its
// image fetches it from the owner's type first.
#include "shared/alpha_shape.hpp"
#include "shared/blit.hpp"
#include "shared/display.hpp"
#include "shared/object.hpp"
#include "shared/object_type.hpp"
#include "shared/shape_file.hpp"
#include "shared/tactical.hpp"
// @ref ?unknown_AlphaShapeClass_list_0083c648@@3V?$DynamicVectorClass@PAVAlphaShapeClass@@@@A=0083c648
// @ref ?unknown_alpha_table_83c670@@3PAEA=0083c670
// @ref ?Intersect@@YI?AVRect@@ABV1@0PAH1@Z=0041fdf0
// @ref ?Get_Buffer_Ptr@ZBuffer@@QAEPAGUPoint2D@@@Z=00411230
// @ref ?ZBufferPtr@@3PAVZBuffer@@A=008315b4
// @ref ?unknown_rect_839630@@3VRect@@A=00839630
// @ref ?unknown_tactical_83996c@@3PAVTactical@@A=0083996c
// @ref _unknown_empty_rect=0083c600

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

void __fastcall Unknown_0041F5E0(Rect const &cliprect)   // NATIVE: the callers' name (overlay ov3 declared AlphaShapeClass::Draw_All)
{
    for (int index = 0; index < unknown_AlphaShapeClass_list_0083c648.Count(); index++) {
        {
            Rect r1 = unknown_AlphaShapeClass_list_0083c648[index]->Bounds;
            r1 -= Point2D(unknown_tactical_83996c->TacPixelX, unknown_tactical_83996c->TacPixelY);
            r1.X += unknown_rect_839630.X;   // NATIVE: the overlay's Rect += Point2D (RA2_RECT_POINT_INLINE): moves the corner
            r1.Y += unknown_rect_839630.Y;   // NATIVE
            Rect clip = cliprect;
            Rect r2 = Intersect(r1, clip);
            if (r2.Is_Valid()) {
                int x1 = r1.X;
                int y1 = r1.Y;
                ShapeFileStruct const *shape = unknown_AlphaShapeClass_list_0083c648[index]->Image;
                if (shape == 0) {
                    unknown_AlphaShapeClass_list_0083c648[index]->Image = (ShapeFileStruct *)unknown_AlphaShapeClass_list_0083c648[index]->Owner->Class_Of()->unknown_ac;
                    shape = unknown_AlphaShapeClass_list_0083c648[index]->Image;
                }
                Rect shape_rect = shape->Get_Frame_Rect(0);
                int top = MAX(r2.Y, shape_rect.Y + y1);
                int src_y = top - shape_rect.Y - y1;
                int bottom = MIN(clip.Y + clip.Height, shape_rect.Y + shape_rect.Height + y1);
                int left = MAX(r2.X, shape_rect.X + x1);
                int src_x = left - shape_rect.X - x1;
                int right = MIN(clip.X + clip.Width, shape_rect.X + shape_rect.Width + x1);
                int shape_skip = shape_rect.Width - right + shape_rect.X + x1 + src_x;
                int alpha_skip = ZBufferPtr->Width - right + left;
                unsigned char *shapedata = (unsigned char *)shape->Get_Frame_Data(0);
                unsigned short *alphaptr = ZBufferPtr->Get_Buffer_Ptr(Point2D(left - unknown_rect_839630.X, top - unknown_rect_839630.Y));
                unsigned char *shapeptr = &shapedata[src_x + src_y * shape_rect.Width];
                if (&alphaptr[right - left + (bottom - top) * ZBufferPtr->Width + 2] >= (unsigned short *)ZBufferPtr->BufferTail) {
                    for (int y = top; y < bottom; y++) {
                        for (int x = left; x < right; x++) {
                            unsigned char pixel = *shapeptr++;
                            *alphaptr = unknown_alpha_table_83c670[pixel * 256 + *alphaptr];
                            alphaptr++;
                            alphaptr = ZBufferPtr->Wrap_Pointer(alphaptr);
                        }
                        shapeptr += shape_skip;
                        alphaptr += alpha_skip;
                        alphaptr = ZBufferPtr->Wrap_Pointer(alphaptr);
                    }
                } else {
                    for (int y = top; y < bottom; y++) {
                        for (int x = left; x < right; x++) {
                            unsigned char pixel = *shapeptr++;
                            *alphaptr = unknown_alpha_table_83c670[pixel * 256 + *alphaptr];
                            ++alphaptr;
                        }
                        shapeptr += shape_skip;
                        alphaptr += alpha_skip;
                    }
                }
            }
        }
    }
}
