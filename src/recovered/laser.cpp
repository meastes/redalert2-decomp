// laser.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/laser.json carries the addresses and the rows that
// stay in their own files.


// Header-static dynamic initializers of the unit at 0x00532E80 and its
// list globals (LaserDrawList::Array); the other drafts of this plan are emitted by this file.
// A list whose atexit thunk tests and sets a guard bit is a static data
// member (VC6 guards their destruction); its owner class is a hypothesis.
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/vector_class.hpp"
#include "shared/laser_draw_list.hpp"

DynamicVectorClass<LaserDrawClass *> LaserDrawList::Array;


// Dynamic initializer (0x00532EB0, _$E5) emitted by HeaderStatics_diagonal_init_00532e80.cpp; no source of its own.


// Dynamic initializer (0x00532ED0, _$E8) emitted by HeaderStatics_diagonal_init_00532e80.cpp; no source of its own.


// Dynamic initializer (0x00532EF0, _$E11) emitted by HeaderStatics_diagonal_init_00532e80.cpp; no source of its own.


// Dynamic initializer (0x00532F10, _$E14) emitted by HeaderStatics_diagonal_init_00532e80.cpp; no source of its own.


// Dynamic initializer (0x00532F30, _$E17) emitted by HeaderStatics_diagonal_init_00532e80.cpp; no source of its own.


// Dynamic initializer (0x00532F50, _$E20) emitted by HeaderStatics_diagonal_init_00532e80.cpp; no source of its own.


// Dynamic initializer (0x00532F90, _$E23) emitted by HeaderStatics_diagonal_init_00532e80.cpp; no source of its own.


// Dynamic initializer (0x00532FC0, _$E26) emitted by HeaderStatics_diagonal_init_00532e80.cpp; no source of its own.


// Dynamic initializer (0x00532FF0, _$E29) emitted by HeaderStatics_diagonal_init_00532e80.cpp; no source of its own.


// Dynamic initializer (0x00533020, _$E32) emitted by HeaderStatics_diagonal_init_00532e80.cpp; no source of its own.


// Dynamic initializer (0x00533030, _$E35) emitted by HeaderStatics_diagonal_init_00532e80.cpp; no source of its own.


// Dynamic initializer (0x00533050, _$E38) emitted by HeaderStatics_diagonal_init_00532e80.cpp; no source of its own.


// Dynamic initializer (0x00533070, _$E45) emitted by HeaderStatics_diagonal_init_00532e80.cpp; no source of its own.


// atexit destructor (0x005330B0, _$E42) emitted by HeaderStatics_diagonal_init_00532e80.cpp; no source of its own.


// LaserDrawClass::LaserDrawClass(...) (0x00533110)
// A laser from source to target with its colours, drawn for duration
// progress steps (one per frame) and listed in LaserDrawList::Array.

LaserDrawClass::LaserDrawClass(CoordStruct source, CoordStruct target, int z_adjust, bool unknown, RGBClass inner,
                               RGBClass outer, RGBClass outer_spread, int duration, bool blinks, bool fades,
                               float start_intensity, float end_intensity) :
    unknown_18(1),
    IsHouseColor(false),
    unknown_1d(false),
    Source(source),
    Target(target),
    ZAdjust(z_adjust),
    unknown_3c(unknown),
    Duration(duration),
    Blinks(blinks),
    BlinkState(false),
    Fades(fades),
    StartIntensity(start_intensity),
    EndIntensity(end_intensity)
{
    Progress.Stage = 0;
    Progress.Set_Rate(1);
    InnerColor = inner;
    OuterColor = outer;
    OuterSpread = outer_spread;
    LaserDrawList::Array.Add(this);
}


// LaserDrawClass::~LaserDrawClass() (0x00533260)

LaserDrawClass::~LaserDrawClass()
{
    LaserDrawList::Array.Delete(this);
}


// void LaserDrawClass::Clear_All() (0x005332B0)

void LaserDrawClass::Clear_All()
{
    while (LaserDrawList::Array.Count() != 0) {
        delete LaserDrawList::Array[0];
    }
}


// void LaserDrawClass::Update() (0x00533330)

void LaserDrawClass::Update()
{
    Progress.Advance_Stage_When_Ready();
    if (Blinks) {
        BlinkState = !BlinkState;
    }
    if (Progress.Stage >= Duration) {
        delete this;
    }
}


// void LaserDrawClass::Update_All() (0x005333F0)

void LaserDrawClass::Update_All()
{
    for (int index = LaserDrawList::Array.Count() - 1; index >= 0; index--) {
        LaserDrawList::Array[index]->Update();
    }
}


// void LaserDrawClass::Draw_All() (0x005334D0)

void LaserDrawClass::Draw_All()
{
    for (int index = LaserDrawList::Array.Count() - 1; index >= 0; index--) {
        LaserDrawList::Array[index]->Draw_It();
    }
}


#include "shared/laser_draw.hpp"
#include "shared/surface.hpp"
#include "shared/tactical.hpp"
#include "shared/facing.hpp"
#include "shared/fast_math.hpp"
#include "shared/random2.hpp"
#include "shared/hicolor_pixel.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/frame_rate.hpp"
#include "shared/combat.hpp"


// void LaserDrawClass::Draw_It() (0x005334F0; TibSun laser.cpp Draw_It)
// Draws the laser as a core line and, with an outer colour, two one-pixel glow lines beside it
// offset by the beam's eight-way facing. RA2: no fog test; house-colour lasers draw through
// Draw_House_Color; at detail level 0 depth-shaded lines, else antialiased (blinking) or
// intensity-scaled (slot 16) lines.

extern double const unknown_facing_scale_79b628;
extern double const unknown_facing_origin_79b630;
extern Rect unknown_rect_839630;

// The direction from one coordinate to another (TibSun inline.h Direction(Coord const &, Coord const &);
// file-local here until another unit shows it).
inline UnknownFacingDir Direction(CoordStruct const &coord1, CoordStruct const &coord2)
{
    int x1 = coord1.X;
    int x2 = coord2.X;

    return UnknownFacingDir((unsigned short)(int)((Fast_Atan2((double)coord1.Y - (double)coord2.Y, (double)x2 - (double)x1)
        - unknown_facing_origin_79b630) * unknown_facing_scale_79b628));
}

// A value in [a, b] from the generator at 0x00839218 (TibSun Sim_Random_Pick; file-local hypothesis).
inline int Sim_Random_Pick(int a, int b)
{
    return unknown_random_839218(a, b);
}

// The glow offset table's element: a screen point with a destructor (the table's atexit
// destructor 0x00533C70 is an empty function; type hypothesis).
struct LaserGlowOffset : Point2D {
    LaserGlowOffset(Point2D const &point) : Point2D(point) {}
    ~LaserGlowOffset() {}
};

void LaserDrawClass::Draw_It()
{
    if (Duration > 0) {
        if (IsHouseColor) {
            Draw_House_Color();
            return;
        }

        static LaserGlowOffset _glow_offsets[16] = {
            Point2D(0, -1),
            Point2D(1, 0),
            Point2D(0, -1),
            Point2D(0, 1),
            Point2D(1, 0),
            Point2D(0, 1),
            Point2D(-1, 0),
            Point2D(1, 0),
            Point2D(-1, 0),
            Point2D(0, 1),
            Point2D(0, -1),
            Point2D(1, 0),
            Point2D(0, -1),
            Point2D(-1, 0),
            Point2D(-1, 0),
            Point2D(1, 0)
        };

        if (!BlinkState) {
            UnknownFacingDir dir = Direction(Source, Target);
            int facing = Dir_To_8(dir);

            Point2D start_pixel;
            unknown_tactical_83996c->Coord_To_Pixel(Source, start_pixel);
            Point2D end_pixel;
            unknown_tactical_83996c->Coord_To_Pixel(Target, end_pixel);

            int start_z = ZAdjust - Unknown_006A2970(Source.Z) - 2;
            int end_z = -Unknown_006A2970(Target.Z) - 2;

            RGBClass outer_color;
            int outer_hicolor = 0;
            bool has_outer_glow = true;

            if (OuterColor == RGBClass(0, 0, 0)) {
                has_outer_glow = false;
            } else {
                int red = Sim_Random_Pick(-OuterSpread.Red, OuterSpread.Red);
                int green = Sim_Random_Pick(-OuterSpread.Green, OuterSpread.Green);
                int blue = Sim_Random_Pick(-OuterSpread.Blue, OuterSpread.Blue);

                red += OuterColor.Red;
                red = red < 0 ? 0 : red;
                red = red > 255 ? 255 : red;

                green += OuterColor.Green;
                green = green < 0 ? 0 : green;
                green = green > 255 ? 255 : green;

                blue += OuterColor.Blue;
                blue = blue < 0 ? 0 : blue;
                blue = blue > 255 ? 255 : blue;

                outer_hicolor = RGB_To_Pixel(red, green, blue);
                outer_color.Red = red;
                outer_color.Green = green;
                outer_color.Blue = blue;
            }

            float current_intensity = 1.0f;

            bool has_r = InnerColor.Red > 0;
            bool has_g = InnerColor.Green > 0;
            bool has_b = InnerColor.Blue > 0;

            if (Fades) {
                current_intensity = (StartIntensity - EndIntensity) * (Duration - Progress.Stage) / Duration + EndIntensity;
            }

            if (unknown_a40b2c != 0) {
                if (Blinks) {
                    unknown_surface_83995c->Draw_Depth_Antialiased_Line(unknown_rect_839630, start_pixel, end_pixel,
                        InnerColor, start_z, end_z, false, true, true, true, current_intensity);
                    if (has_outer_glow) {
                        unknown_surface_83995c->Draw_Depth_Antialiased_Line(unknown_rect_839630,
                            start_pixel + _glow_offsets[2 * facing], end_pixel + _glow_offsets[2 * facing],
                            outer_color, start_z, end_z, false, true, true, true, 1.0f);
                        unknown_surface_83995c->Draw_Depth_Antialiased_Line(unknown_rect_839630,
                            start_pixel + _glow_offsets[2 * facing + 1], end_pixel + _glow_offsets[2 * facing + 1],
                            outer_color, start_z, end_z, false, true, true, true, 1.0f);
                    }
                } else if (has_outer_glow) {
                    unknown_surface_83995c->Unknown_Slot16(unknown_rect_839630,
                        start_pixel + _glow_offsets[2 * facing], end_pixel + _glow_offsets[2 * facing],
                        outer_color, current_intensity, start_z, end_z);
                    unknown_surface_83995c->Unknown_Slot16(unknown_rect_839630,
                        start_pixel + _glow_offsets[2 * facing + 1], end_pixel + _glow_offsets[2 * facing + 1],
                        outer_color, current_intensity, start_z, end_z);
                    unknown_surface_83995c->Unknown_Slot16(unknown_rect_839630, start_pixel, end_pixel,
                        InnerColor, current_intensity, start_z, end_z);
                } else {
                    unknown_surface_83995c->Draw_Depth_Antialiased_Line(unknown_rect_839630, start_pixel, end_pixel,
                        InnerColor, start_z, end_z, false, has_r, has_g, has_b, current_intensity);
                }
            } else {
                unknown_surface_83995c->Draw_Depth_Shaded_Line(unknown_rect_839630, start_pixel, end_pixel,
                    RGB_To_Pixel(InnerColor.Red, InnerColor.Green, InnerColor.Blue), start_z, end_z);
                if (has_outer_glow) {
                    unknown_surface_83995c->Draw_Depth_Shaded_Line(unknown_rect_839630,
                        start_pixel + _glow_offsets[2 * facing], end_pixel + _glow_offsets[2 * facing],
                        outer_hicolor, start_z, end_z);
                    unknown_surface_83995c->Draw_Depth_Shaded_Line(unknown_rect_839630,
                        start_pixel + _glow_offsets[2 * facing + 1], end_pixel + _glow_offsets[2 * facing + 1],
                        outer_hicolor, start_z, end_z);
                }
            }
        }
    }
}


// 0x00533C70: the atexit destructor of LaserDrawClass::Draw_It's static _glow_offsets table (emitted there).


// LaserDrawClass::Draw_House_Color (00533C80), reconstructed from the exact base RA2 body.
// The LaserGlowOffset element type and Direction helper belong to this unit's
// existing Draw_It source. Its nontrivial empty destructor explains the original
// local table's atexit registration; the original class identity remains a hypothesis.

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


// 0x00534340: function not yet identified
//
// Body: 1 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: void unknown_empty_00534340(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00534340 {

void  unknown_empty_00534340(void)
{
    return;
}

}  // namespace seed_00534340


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<LaserDrawClass *>::Clear (0x00534350; DynamicVectorClass<LaserDrawClass *> vtable 0x007a5414 slot 3).
// DynamicVectorClass<LaserDrawClass *>::ID_pointer (0x00534380; DynamicVectorClass<LaserDrawClass *> vtable 0x007a5414 slot 5).
// VectorClass<LaserDrawClass *>::Fetch (0x005343A0; VectorClass<LaserDrawClass *> vtable 0x007a5434 slot 6).
// VectorClass<LaserDrawClass *>::operator equal (0x005343B0; VectorClass<LaserDrawClass *> vtable 0x007a5434 slot 1).
// VectorClass<LaserDrawClass *>::Resize (0x005343F0; VectorClass<LaserDrawClass *> vtable 0x007a5434 slot 2).
// VectorClass<LaserDrawClass *>::Clear (0x005344A0; VectorClass<LaserDrawClass *> vtable 0x007a5434 slot 3).
// VectorClass<LaserDrawClass *>::ID_object (0x005344D0; VectorClass<LaserDrawClass *> vtable 0x007a5434 slot 4).
// VectorClass<LaserDrawClass *>::ID_pointer (0x00534510; VectorClass<LaserDrawClass *> vtable 0x007a5434 slot 5).
// DynamicVectorClass<LaserDrawClass *>::Resize (0x00534530; DynamicVectorClass<LaserDrawClass *> vtable 0x007a5414 slot 2).
// DynamicVectorClass<LaserDrawClass *>::ID_object (0x005345E0; DynamicVectorClass<LaserDrawClass *> vtable 0x007a5414 slot 4).


extern "C" unsigned char vtable_534610[]; extern "C" void __cdecl release_buffer_534610(void*); extern "C" void __cdecl release_object_534610(void*);
class Leaf_534610 { public: void* get(unsigned int flags); };
void* Leaf_534610::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_534610; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_534610((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_534610(this); return this; }


extern "C" unsigned char vtable_534660[]; extern "C" void __cdecl release_buffer_534660(void*); extern "C" void __cdecl release_object_534660(void*);
class Leaf_534660 { public: void* get(unsigned int flags); };
void* Leaf_534660::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_534660; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_534660((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_534660(this); return this; }
