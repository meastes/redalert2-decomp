// ovrlight.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/ovrlight.json carries the addresses and the rows that
// stay in their own files.


#include "shared/spot_light.hpp"
#include "shared/always.hpp"
#include "shared/surface.hpp"
#include "shared/rules.hpp"
#define RGB_DECLARE_ACCESSORS
#include "shared/hicolor.hpp"
#include "shared/cpu_info.hpp"
#include "shared/combat.hpp"
#include "shared/frame_rate.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/blit.hpp"
#include "shared/tactical.hpp"
#include "shared/map_view.hpp"
#include "shared/id_source.hpp"
#define SL_MIN(a, b) ((a) < (b) ? (a) : (b))


// Dynamic initializer (0x005DD510, _$E) emitted by HeaderStatics_diagonal_init_005dd320.cpp; no source of its own.


// Dynamic initializer (0x005DD550, _$E) emitted by HeaderStatics_diagonal_init_005dd320.cpp; no source of its own.


// SpotLightClass::SpotLightClass(CoordStruct coord, int size) (0x005DD590)

SpotLightClass::SpotLightClass(CoordStruct coord, int size) :
    Coord(coord),
    unknown_0c(0),
    unknown_10(size),
    unknown_14(0)
{
    unknown_SpotLightClass_list_00a731c0.Add(this);
}


// SpotLightClass::~SpotLightClass() (0x005DD610)

SpotLightClass::~SpotLightClass()
{
    unknown_SpotLightClass_list_00a731c0.Delete(this);
}


// void SpotLightClass::AI() (0x005DD660)

void SpotLightClass::AI()
{
    unknown_0c += 8;
    if (unknown_0c >= 80) {
        delete this;
    }
}


// void SpotLightClass::Update_All() (0x005DD6D0)

void SpotLightClass::Update_All()
{
    for (int index = unknown_SpotLightClass_list_00a731c0.Count() - 1; index >= 0; index--) {
        unknown_SpotLightClass_list_00a731c0[index]->AI();
    }
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 OpenTS contributors
// Derived from TibSun code/ovrlight.cpp; modified for the supplied RA2 executable.
// EA's additional GPL Section 7 terms apply; see the upstream LICENSE.md.
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


// The CRT free (lobby.hpp and main_opt.hpp declare it too; neither belongs in this unit).
void Free_Memory_00781d3b(void *block);

// A polymorphic object the SpotLight unit keeps in a 74-entry table (0x00A731E0; type not recovered).
class UnknownSpotLightObject_a731e0 {
public:
    virtual ~UnknownSpotLightObject_a731e0();
};
extern UnknownSpotLightObject_a731e0 *unknown_objects_a731e0[74];
// A block freed by Unknown_005DDB40 (0x00A73378).
extern void *unknown_block_a73378;

void Unknown_005DDB40()
{
    if (unknown_block_a73378 != 0) {
        Free_Memory_00781d3b(unknown_block_a73378);
        unknown_block_a73378 = 0;
    }
    for (int i = 0; i < 74; i++) {
        delete unknown_objects_a731e0[i];
        unknown_objects_a731e0[i] = 0;
    }
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// 

// void SpotLightClass::unknown_func_5ddb90() (0x005DDB90; TibSun ovrlight.cpp Draw_It):
// lights, darkens or tints the pixels under the spot light through the type's falloff
// surface for the light's current radius; the plain light uses the asm blitters.
// From ra2-decomp SpotLightClass::Draw_It (YR 0x005FF850), adapted to our names.
extern Rect unknown_rect_839630;        // 0x00839630 tactical X, Y, width, height
extern "C" char SpotLight_Index_Table[];        // 0x007E8C50
extern "C" int SpotLight_Color_Mode;            // 0x007E8C4C
extern "C" void *SpotLight_MMX_Buffer;          // 0x00A73378
extern "C" Surface *SpotLight_Surfaces[];       // 0x00A731E0
extern "C" void __cdecl Brighten_Color_555(void *sptr, void *dptr, int spitch, int dpitch, int width, int height);       // 0x007983D1
extern "C" void __cdecl Brighten_Color_556(void *sptr, void *dptr, int spitch, int dpitch, int width, int height);       // 0x007982F4
extern "C" void __cdecl Brighten_Color_565(void *sptr, void *dptr, int spitch, int dpitch, int width, int height);       // 0x0079813A
extern "C" void __cdecl Brighten_Color_655(void *sptr, void *dptr, int spitch, int dpitch, int width, int height);       // 0x00798217
extern "C" void __cdecl MMX_Brighten_Color_555(void *sptr, void *dptr, int spitch, int dpitch, int width, int height, void *buffer);   // 0x00798602
extern "C" void __cdecl MMX_Brighten_Color_556(void *sptr, void *dptr, int spitch, int dpitch, int width, int height, void *buffer);   // 0x00798740
extern "C" void __cdecl MMX_Brighten_Color_565(void *sptr, void *dptr, int spitch, int dpitch, int width, int height, void *buffer);   // 0x007984C0
extern "C" void __cdecl MMX_Brighten_Color_655(void *sptr, void *dptr, int spitch, int dpitch, int width, int height, void *buffer);   // 0x00798880


void SpotLightClass::unknown_func_5ddb90()
{
    unsigned int flags = unknown_14 & 0xF;
    if ((flags & ~1) == 0) {
        if (unknown_a6eb1c < Unknown_0053D840()) {
            return;
        }
    }

    Point2D drawpoint;
    if (unknown_tactical_83996c->Coord_To_Pixel(Coord, drawpoint)) {
        if (!(unknown_id_source_a3d290->unknown_000 & 0x1000) || !Map.Is_Fogged(Coord)) {
            Surface *dsurf = unknown_surface_83995c;
            Rect drect(drawpoint.X - 128, drawpoint.Y - 64, 255, 127);
            Rect dcliprect = unknown_rect_839630;
            int index = SpotLight_Index_Table[unknown_0c];
            if (index < 64) {
                index = index * unknown_10 / 64;
            }
            Surface *ssurf = SpotLight_Surfaces[index];
            int stride = dsurf->Stride();
            bool overlapped = false;
            Rect srect(0, 0, 255, 127);
            Rect scliprect(0, 0, 255, 127);
            void *dbuffer;
            void *sbuffer;
            if (Prep_For_Blit(*dsurf, dcliprect, drect, *ssurf, scliprect, srect, overlapped, dbuffer, sbuffer)) {

                unsigned short *dptr_row = (unsigned short *)dbuffer;
                unsigned char *sptr_row = (unsigned char *)sbuffer;
                unsigned short *dptr = dptr_row;
                unsigned char *sptr = sptr_row;
                unsigned char spixel;
                RGBClass rgb;

                int colormode = SpotLight_Color_Mode;
                if (flags == 0 && (unsigned)colormode <= 3) {
                    switch (colormode) {
                    case 0:
                        if (SpotLight_MMX_Buffer) {
                            MMX_Brighten_Color_555(sptr, dptr, 256, stride, srect.Width, srect.Height, SpotLight_MMX_Buffer);
                        } else {
                            Brighten_Color_555(sptr, dptr, 256, stride, srect.Width, srect.Height);
                        }
                        break;
                    case 1:
                        if (SpotLight_MMX_Buffer) {
                            MMX_Brighten_Color_556(sptr, dptr, 256, stride, srect.Width, srect.Height, SpotLight_MMX_Buffer);
                        } else {
                            Brighten_Color_556(sptr, dptr, 256, stride, srect.Width, srect.Height);
                        }
                        break;
                    case 2:
                        if (SpotLight_MMX_Buffer) {
                            MMX_Brighten_Color_565(sptr, dptr, 256, stride, srect.Width, srect.Height, SpotLight_MMX_Buffer);
                        } else {
                            Brighten_Color_565(sptr, dptr, 256, stride, srect.Width, srect.Height);
                        }
                        break;
                    case 3:
                        if (SpotLight_MMX_Buffer) {
                            MMX_Brighten_Color_655(sptr, dptr, 256, stride, srect.Width, srect.Height, SpotLight_MMX_Buffer);
                        } else {
                            Brighten_Color_655(sptr, dptr, 256, stride, srect.Width, srect.Height);
                        }
                        break;
                    default:
                        goto fallback;
                    }
                } else {
                    fallback:
                    if (unknown_14 & 1) {
                        for (int y = 0; y < srect.Height; y++) {
                            for (int x = 0; x < srect.Width; x++) {
                                spixel = *sptr++;
                                if (spixel != 0) {
                                    int scale = 256 - spixel;
                                    rgb = Deconstruct_Hicolor_Pixel(*dptr);
                                    *dptr = Build_Hicolor_Pixel((rgb.Red * scale) >> 8, (rgb.Green * scale) >> 8, (rgb.Blue * scale) >> 8);
                                }
                                dptr++;
                            }
                            dptr_row = (unsigned short *)((unsigned char *)dptr_row + stride);
                            dptr = dptr_row;
                            sptr_row += 256;
                            sptr = sptr_row;
                        }
                    } else if (unknown_14 & 0xE) {
                        for (int x = 0; x < srect.Height; x++) {
                            for (int y = 0; y < srect.Width; y++) {
                                spixel = *sptr++;
                                if (spixel != 0) {
                                    rgb = Deconstruct_Hicolor_Pixel(*dptr);
                                    int scale = spixel + 256;
                                    int rr = rgb.Red;
                                    int gg = rgb.Green;
                                    int bb = rgb.Blue;
                                    if (!(unknown_14 & 2)) {
                                        rr = (rr * scale) >> 8;
                                        if (rr > 255) rr = 255;
                                    }
                                    if (!(unknown_14 & 4)) {
                                        gg = (gg * scale) >> 8;
                                        if (gg > 255) gg = 255;
                                    }
                                    if (!(unknown_14 & 8)) {
                                        bb = (bb * scale) >> 8;
                                        if (bb > 255) bb = 255;
                                    }
                                    *dptr = Build_Hicolor_Pixel(rr, gg, bb);
                                }
                                dptr++;
                            }
                            dptr_row = (unsigned short *)((unsigned char *)dptr_row + stride);
                            dptr = dptr_row;
                            sptr_row += 256;
                            sptr = sptr_row;
                        }
                    } else {
                        for (int x = 0; x < srect.Height; x++) {
                            for (int y = 0; y < srect.Width; y++) {
                                spixel = *sptr++;
                                if (spixel != 0) {
                                    int scale = spixel + 256;
                                    rgb = Deconstruct_Hicolor_Pixel(*dptr);
                                    int rr = (rgb.Red * scale) >> 8;
                                    int gg = (rgb.Green * scale) >> 8;
                                    int bb = (rgb.Blue * scale) >> 8;
                                    *dptr = Build_Hicolor_Pixel(SL_MIN(255, rr), SL_MIN(255, gg), SL_MIN(255, bb));
                                }
                                dptr++;
                            }
                            dptr_row = (unsigned short *)((unsigned char *)dptr_row + stride);
                            dptr = dptr_row;
                            sptr_row += 256;
                            sptr = sptr_row;
                        }
                    }
                }
                dsurf->Unlock();
                ssurf->Unlock();
            }
        }
    }
}


// void SpotLightClass::Draw_All() (0x005DE2E0)

void SpotLightClass::Draw_All()
{
    for (int index = unknown_SpotLightClass_list_00a731c0.Count() - 1; index >= 0; index--) {
        unknown_SpotLightClass_list_00a731c0[index]->unknown_func_5ddb90();
    }
}


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<SpotLightClass *>::Clear (0x005DE300; DynamicVectorClass<SpotLightClass *> vtable 0x007a7ac4 slot 3).
// DynamicVectorClass<SpotLightClass *>::ID_pointer (0x005DE330; DynamicVectorClass<SpotLightClass *> vtable 0x007a7ac4 slot 5).
// VectorClass<SpotLightClass *>::Fetch (0x005DE350; VectorClass<SpotLightClass *> vtable 0x007a7ae4 slot 6).
// VectorClass<SpotLightClass *>::operator equal (0x005DE360; VectorClass<SpotLightClass *> vtable 0x007a7ae4 slot 1).
// VectorClass<SpotLightClass *>::Resize (0x005DE3A0; VectorClass<SpotLightClass *> vtable 0x007a7ae4 slot 2).
// VectorClass<SpotLightClass *>::Clear (0x005DE450; VectorClass<SpotLightClass *> vtable 0x007a7ae4 slot 3).
// VectorClass<SpotLightClass *>::ID_object (0x005DE480; VectorClass<SpotLightClass *> vtable 0x007a7ae4 slot 4).
// VectorClass<SpotLightClass *>::ID_pointer (0x005DE4C0; VectorClass<SpotLightClass *> vtable 0x007a7ae4 slot 5).
// DynamicVectorClass<SpotLightClass *>::Resize (0x005DE4E0; DynamicVectorClass<SpotLightClass *> vtable 0x007a7ac4 slot 2).
// DynamicVectorClass<SpotLightClass *>::ID_object (0x005DE590; DynamicVectorClass<SpotLightClass *> vtable 0x007a7ac4 slot 4).


extern "C" unsigned char vtable_5de5c0[]; extern "C" void __cdecl release_buffer_5de5c0(void*); extern "C" void __cdecl release_object_5de5c0(void*);
class Leaf_5de5c0 { public: void* get(unsigned int flags); };
void* Leaf_5de5c0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_5de5c0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_5de5c0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_5de5c0(this); return this; }


extern "C" unsigned char vtable_5de610[]; extern "C" void __cdecl release_buffer_5de610(void*); extern "C" void __cdecl release_object_5de610(void*);
class Leaf_5de610 { public: void* get(unsigned int flags); };
void* Leaf_5de610::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_5de610; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_5de610((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_5de610(this); return this; }
