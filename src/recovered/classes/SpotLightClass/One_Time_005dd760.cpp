// @ref ?Get_Unknown_7d6290@@YIHXZ=004abb90
// @ref __real@8@bff6b6db6db6db6db800=007a7b00
// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 OpenTS contributors
// Derived from TibSun code/ovrlight.cpp; modified for the supplied RA2 executable.
// EA's additional GPL Section 7 terms apply; see the upstream LICENSE.md.
// @ref ?unknown_spotlight_ready_a73374@@3_NA=00a73374
// @ref _SpotLight_Surfaces=00a731e0
// @ref _SpotLight_MMX_Buffer=00a73378
// @ref _SpotLight_Color_Mode=007e8c4c
#include "shared/always.hpp"
#include "shared/surface.hpp"
#include "shared/rules.hpp"
#define RGB_DECLARE_ACCESSORS
#include "shared/hicolor.hpp"
#include "shared/cpu_info.hpp"
extern bool unknown_spotlight_ready_a73374;
extern "C" Surface *SpotLight_Surfaces[];
extern "C" void *SpotLight_MMX_Buffer;
extern "C" int SpotLight_Color_Mode;
// SpotLightClass::One_Time; preserve the already bound caller symbol.
void __fastcall Unknown_005DD760()
{
    int i;
    int j;
    if (!unknown_spotlight_ready_a73374) {
        for (i = 0; i < 64; i++) {
            if (SpotLight_Surfaces[i] == 0) SpotLight_Surfaces[i] = new BSurface(256,128,1);
            SpotLight_Surfaces[i]->Fill(0);
            BSurface surface(256,256,1);
            surface.Fill(0);
            Point2D center(128,128);
            Rect bounds(0,0,255,255);
            int radius = i*2+1;
            int ring;
            if ((ring = radius) > 0) {
                for (int color = 12; ring > 0; ring -= 2, color += 4)
                    surface.Fill_Circle(center,ring,bounds,(color <= 255 ? color : 255));
            }
            for (j = 0; j < 128; j++)
                SpotLight_Surfaces[i]->Copy_From(Rect(0,j,255,1),surface,Rect(0,2*j,255,1),false,true);
        }
        for (i = 0; i < 10; i++) {
            if (SpotLight_Surfaces[i+64] == 0) SpotLight_Surfaces[i+64] = new BSurface(256,128,1);
            SpotLight_Surfaces[i+64]->Fill(0);
            BSurface surface(256,256,1);
            surface.Fill(0);
            Point2D center(128,128);
            Rect bounds(0,0,255,255);
            int radius = 60*Rules->unknown_6d0;
            surface.Fill_Circle(center,i-int(radius/-358.4),bounds,128-i*6);
            for (j = 0; j < 128; j++)
                SpotLight_Surfaces[i+64]->Copy_From(Rect(0,j,255,1),surface,Rect(0,2*j,255,1),false,true);
        }
        unknown_spotlight_ready_a73374 = true;
    }
    SpotLight_Color_Mode = Get_Unknown_7d6290();
    int cpu_type = 5;
    unsigned char mmx = 0;
    Get_CPU_Info(&cpu_type,&mmx,0,0);
    if (mmx && SpotLight_MMX_Buffer == 0) {
        SpotLight_MMX_Buffer = new int[65536];
        for (int color = 0; color < 65536; color++) {
            RGBClass rgb = Deconstruct_Hicolor_Pixel((unsigned short)color);
            ((int *)SpotLight_MMX_Buffer)[color] = (rgb.Get_Red()<<16)|(rgb.Get_Green()<<8)|rgb.Get_Blue();
        }
    }
}
