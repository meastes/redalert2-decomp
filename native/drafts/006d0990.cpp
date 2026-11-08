// NON-EXACT DRAFT 0x6D0990 FUN_006d0990
// entry 0x6D0990; exact-match score 70.83 (misses_index best), this text 69.91; plan "review: rewrite likely"
// origin lane: Scout1009 (build/class-work/Scout1009/keep/990_while_6991.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1069 (native/drafts.json). Not original bytes.
// Native adaptation (Scout1069): Line_End_Rect (already in the host group) dropped; each changed line is marked NATIVE.
// TechnoClass 0x006D0990.
#include "shared/techno.hpp"
#include "shared/tactical.hpp"
#include "shared/tool_tip.hpp"
#include "shared/hicolor.hpp"
#include "shared/surface.hpp"
#include "shared/unknown_globals.hpp"
extern "C" __declspec(dllimport) unsigned long __stdcall timeGetTime();
// @ref ?Adjust@RGBClass@@QAEXHABV1@@Z=0063cf20
// @ref __real@8@40038000000000000000=007acff8
// @ref __real@8@3ffb8000000000000000=007acff0
// @ref __real@8@40088000000000000000=007acfe8
// @ref __real@8@3fff8000000000000000=0079a708
void TechnoClass::unknown_6d0990(CoordStruct from, CoordStruct to, RGBClass color)
{
    RGBClass shade;
    RGBClass white(255,255,255);
    {
        Point2D start;
        Point2D end;
        unknown_tactical_83996c->Coord_To_Pixel(from,start);
        unknown_tactical_83996c->Coord_To_Pixel(to,end);
        start.Y+=unknown_rect_839630.Y;
        end.Y+=unknown_rect_839630.Y;
        int pixel=Build_Hicolor_Pixel(color.Red,color.Green,color.Blue);
        unknown_surface_839964->Fill_Rect(unknown_rect_839630.Intersect(Line_End_Rect(start)),pixel);
        unknown_surface_839964->Fill_Rect(unknown_rect_839630.Intersect(Line_End_Rect(end)),pixel);
    }
    Rect clip=unknown_rect_839630;
    int phase=timeGetTime()>>4;
    phase<<=3;
    CoordStruct last=from;
    CoordStruct next;
    int i=1;
    int j=31;
    while (i<=32) {
        next.X=(to.X*i+from.X*j)/32;
        next.Y=(to.Y*i+from.Y*j)/32;
        double t=(i-16.0)*0.0625;
        int z=(to.Z*i+from.Z*j)/32;
        next.Z=(int)(z+(-(t*t)+1.0)*512.0);
        Point2D start;
        Point2D end;
        unknown_tactical_83996c->Coord_To_Pixel(last,start);
        unknown_tactical_83996c->Coord_To_Pixel(next,end);
        start.Y+=unknown_rect_839630.Y;
        end.Y+=unknown_rect_839630.Y;
        if (Clip_Line(start,end,clip)) {
            shade=color;
            unsigned ratio=(i*8-phase)&0x1f8;
            if (ratio<=0xf8 && ratio!=0) {
                shade.Adjust(ratio,white);
            }
            unknown_surface_839964->Draw_Line(start,end,Build_Hicolor_Pixel(shade.Red,shade.Green,shade.Blue));
        }
        last=next;
        i++;
        j--;
    }
}
