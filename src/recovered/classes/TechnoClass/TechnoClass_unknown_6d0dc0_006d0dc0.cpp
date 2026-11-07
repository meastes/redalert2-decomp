// TechnoClass 0x006D0DC0: a line from `from` to `to` in 64 steps, shaded from the 0x00AB6841 colour at
// `from` to `color` at `to`, with end markers and an optional one-pixel-lower shadow (never called).
// Moved_Rect comes from the 0x006D0510 source earlier in this unit.
#include "shared/techno.hpp"
#include "shared/tactical.hpp"
#include "shared/hicolor.hpp"
#include "shared/surface.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/convert.hpp"
#include "shared/rgb_globals.hpp"
// @ref ?Adjust@RGBClass@@QAEXHABV1@@Z=0063cf20
// @ref ??0Rect@@QAE@ABURectPoint2D@@HH@Z=00453e40
// @ref ?Intersect@@YI?AVRect@@ABV1@0PAH1@Z=0041fdf0
// @ref ??HRectPoint2D@@QBE?BU0@ABU0@@Z=00435540
// TibSun DSurface::Build_Hicolor_Pixel(RGBClass const &): the three-channel builder on a colour.
static inline int Build_Hicolor_Pixel(RGBClass const &rgb)
{
    return ((rgb.Red >> unknown_shift_852fbc) << unknown_shift_852fb8)
         | ((rgb.Green >> unknown_shift_852fcc) << unknown_shift_852fc8)
         | ((rgb.Blue >> unknown_shift_852fc4) << unknown_shift_852fc0);
}
// The 3x3 end marker of a line, its corner two pixels up and left of the point.
static inline Rect Line_End_Rect(Point2D const &point)
{
    Point2D offset=point-Point2D(2,2);
    return Rect(RectPoint2D(offset.X,offset.Y),3,3);
}
void TechnoClass::unknown_6d0dc0(CoordStruct from, CoordStruct to, RGBClass color, bool shadow)
{
    RGBClass shade;
    Point2D start;
    Point2D end;
    unknown_tactical_83996c->Coord_To_Pixel(from,start);
    unknown_tactical_83996c->Coord_To_Pixel(to,end);
    start.Y+=unknown_rect_839630.Y;
    end.Y+=unknown_rect_839630.Y;
    int pixel=Build_Hicolor_Pixel(color);
    int fade=Build_Hicolor_Pixel(unknown_rgb_ab6841);
    Point2D shadow_start=start;
    shadow_start.Y+=1;
    Point2D shadow_end=end;
    shadow_end.Y+=1;
    if (shadow) {
        unknown_surface_839964->Fill_Rect(unknown_rect_839630.Intersect(Moved_Rect(Rect(-2,-2,3,3),shadow_start)),pixel);
        unknown_surface_839964->Fill_Rect(unknown_rect_839630.Intersect(Moved_Rect(Rect(-2,-2,3,3),shadow_end)),fade);
    }
    Rect clip=unknown_rect_839630;
    if (shadow && Clip_Line(shadow_start,shadow_end,clip)) {
        unknown_surface_839964->Draw_Line(shadow_start,shadow_end,unknown_drawer_8323c4->Convert(0));
    }
    unknown_surface_839964->Fill_Rect(unknown_rect_839630.Intersect(Line_End_Rect(start)),fade);
    unknown_surface_839964->Fill_Rect(unknown_rect_839630.Intersect(Line_End_Rect(end)),pixel);
    CoordStruct last=from;
    CoordStruct next;
    int i=1;
    int j=63;
    while (i<=64) {
        next.X=(from.X*j+to.X*i)/64;
        next.Y=(from.Y*j+to.Y*i)/64;
        next.Z=(from.Z*j+to.Z*i)/64;
        unknown_tactical_83996c->Coord_To_Pixel(last,start);
        unknown_tactical_83996c->Coord_To_Pixel(next,end);
        start.Y+=unknown_rect_839630.Y;
        end.Y+=unknown_rect_839630.Y;
        if (Clip_Line(start,end,clip)) {
            shade=color;
            int ratio=j*4;
            shade.Adjust(ratio<0xfe?ratio:0xfe,unknown_rgb_ab6841);
            unknown_surface_839964->Draw_Line(start,end,Build_Hicolor_Pixel(shade));
        }
        last=next;
        i++;
        j--;
    }
}
