// @ref _?house_offsets@?1??Draw_House_Color@LaserDrawClass@@QAEXXZ@4PAULaserGlowOffset@@A=00a6e618
// @ref _?$S49@?1??Draw_House_Color@LaserDrawClass@@QAEXXZ@4EA=00a6e5f8
// @ref _$E50=00534340
// LaserDrawClass::Draw_House_Color (00533C80), reconstructed from the exact base RA2 body.
// The LaserGlowOffset element type and Direction helper belong to this unit's
// existing Draw_It source. Its nontrivial empty destructor explains the original
// local table's atexit registration; the original class identity remains a hypothesis.
#include "shared/laser_draw.hpp"
#include "shared/surface.hpp"
#include "shared/tactical.hpp"
#include "shared/facing.hpp"
#include "shared/hicolor_pixel.hpp"
#include "shared/frame_rate.hpp"
#include "shared/combat.hpp"
#include "shared/unknown_globals.hpp"

void LaserDrawClass::Draw_House_Color()
{
    static LaserGlowOffset house_offsets[16] = {
        Point2D(-1,-1), Point2D(1,1), Point2D(0,-1), Point2D(0,1),
        Point2D(-1,1), Point2D(1,-1), Point2D(-1,0), Point2D(1,0),
        Point2D(-1,-1), Point2D(1,1), Point2D(0,-1), Point2D(0,1),
        Point2D(-1,1), Point2D(1,-1), Point2D(-1,0), Point2D(1,0)
    };
    UnknownFacingDir dir = Direction(Source, Target);
    int facing = Dir_To_8(dir);
    Point2D start_pixel;
    unknown_tactical_83996c->Coord_To_Pixel(Source, start_pixel);
    Point2D end_pixel;
    unknown_tactical_83996c->Coord_To_Pixel(Target, end_pixel);
    int start_z = ZAdjust - Unknown_006A2970(Source.Z) - 2;
    int end_z = -Unknown_006A2970(Target.Z) - 2;
    bool high_detail = unknown_a6eb1c >= Unknown_0053D840() && unknown_a40b2c != 0;
    float intensity = 1.0f;
    if (Fades) {
        intensity = (StartIntensity - EndIntensity) * (Duration - Progress.Stage) / Duration + EndIntensity;
    }
    int ratio = int(intensity * 255.0f);
    RGBClass adjusted_color;
    RGBClass white(255,255,255);
    RGBClass glow_color;
    RGBClass core_color;
    if (unknown_1d) {
        unsigned int red = InnerColor.Red * 2;
        unsigned int green = InnerColor.Green * 2;
        unsigned int blue = InnerColor.Blue * 2;
        core_color.Set(red > 255 ? 255 : red, green > 255 ? 255 : green, blue > 255 ? 255 : blue);
        glow_color = core_color;
    } else {
        core_color = InnerColor;
        glow_color.Set(InnerColor.Red >> 1, InnerColor.Green >> 1, InnerColor.Blue >> 1);
    }
    Point2D start2 = start_pixel;
    Point2D start1 = start_pixel;
    Point2D end2 = end_pixel;
    Point2D end1 = end_pixel;
    for (int layer = 1; layer <= unknown_18; ++layer) {
        if (facing & 1) {
            start1.X += house_offsets[2 * facing].X;
            start1.Y += house_offsets[2 * facing].Y;
            start2.X += house_offsets[2 * facing + 1].X;
            start2.Y += house_offsets[2 * facing + 1].Y;
            end1.X += house_offsets[2 * facing].X;
            end1.Y += house_offsets[2 * facing].Y;
            end2.X += house_offsets[2 * facing + 1].X;
            end2.Y += house_offsets[2 * facing + 1].Y;
        } else if (layer & 1) {
            start1.X += house_offsets[2 * facing].X;
            start2.Y += house_offsets[2 * facing + 1].Y;
            end1.X += house_offsets[2 * facing].X;
            end2.Y += house_offsets[2 * facing + 1].Y;
        } else {
            start1.Y += house_offsets[2 * facing].Y;
            start2.X += house_offsets[2 * facing + 1].X;
            end1.Y += house_offsets[2 * facing].Y;
            end2.X += house_offsets[2 * facing + 1].X;
        }
        if (high_detail) {
            unknown_surface_83995c->Unknown_Slot16(unknown_rect_839630, start1, end1, glow_color, intensity, start_z, end_z);
            unknown_surface_83995c->Unknown_Slot16(unknown_rect_839630, start2, end2, glow_color, intensity, start_z, end_z);
        } else {
            adjusted_color = glow_color;
            adjusted_color.Adjust(ratio, white);
            int pixel = RGB_To_Pixel(adjusted_color.Red, adjusted_color.Green, adjusted_color.Blue);
            unknown_surface_83995c->Draw_Depth_Shaded_Line(unknown_rect_839630, start1, end1, pixel, start_z, end_z);
            unknown_surface_83995c->Draw_Depth_Shaded_Line(unknown_rect_839630, start2, end2, pixel, start_z, end_z);
        }
        if (unknown_1d && layer == 1) {
            glow_color = InnerColor;
        } else {
            unsigned int red = glow_color.Red >> 1;
            unsigned int green = glow_color.Green >> 1;
            unsigned int blue = glow_color.Blue >> 1;
            unsigned int threshold = high_detail ? 8 : 64;
            if (red < threshold && green < threshold && blue < threshold) break;
            glow_color.Set(red,green,blue);
        }
    }
    if (high_detail) {
        unknown_surface_83995c->Unknown_Slot16(unknown_rect_839630, start_pixel, end_pixel, core_color, intensity, start_z, end_z);
    } else {
        adjusted_color = core_color;
        adjusted_color.Adjust(ratio, white);
        unknown_surface_83995c->Draw_Depth_Shaded_Line(unknown_rect_839630, start_pixel, end_pixel,
            RGB_To_Pixel(adjusted_color.Red, adjusted_color.Green, adjusted_color.Blue), start_z, end_z);
    }
}
