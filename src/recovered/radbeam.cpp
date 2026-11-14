// radbeam.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/radbeam.json carries the addresses and the rows that
// stay in their own files.

// Dynamic initializer (0x006351E0, _$E5) emitted by HeaderStatics_diagonal_init_006351b0.cpp; no source of its own.


// Dynamic initializer (0x00635200, _$E8) emitted by HeaderStatics_diagonal_init_006351b0.cpp; no source of its own.


// Dynamic initializer (0x00635220, _$E11) emitted by HeaderStatics_diagonal_init_006351b0.cpp; no source of its own.


// Dynamic initializer (0x00635240, _$E14) emitted by HeaderStatics_diagonal_init_006351b0.cpp; no source of its own.


// Dynamic initializer (0x00635260, _$E17) emitted by HeaderStatics_diagonal_init_006351b0.cpp; no source of its own.


// Dynamic initializer (0x00635280, _$E20) emitted by HeaderStatics_diagonal_init_006351b0.cpp; no source of its own.


// Dynamic initializer (0x006352C0, _$E23) emitted by HeaderStatics_diagonal_init_006351b0.cpp; no source of its own.


// Dynamic initializer (0x006352F0, _$E26) emitted by HeaderStatics_diagonal_init_006351b0.cpp; no source of its own.


// Dynamic initializer (0x00635320, _$E29) emitted by HeaderStatics_diagonal_init_006351b0.cpp; no source of its own.


// Dynamic initializer (0x00635350, _$E32) emitted by HeaderStatics_diagonal_init_006351b0.cpp; no source of its own.


// Dynamic initializer (0x00635360, _$E35) emitted by HeaderStatics_diagonal_init_006351b0.cpp; no source of its own.


// Dynamic initializer (0x00635380, _$E38) emitted by HeaderStatics_diagonal_init_006351b0.cpp; no source of its own.


// Dynamic initializer (0x006353A0, _$E45) emitted by HeaderStatics_diagonal_init_006351b0.cpp; no source of its own.


// atexit destructor (0x006353E0, _$E42) emitted by HeaderStatics_diagonal_init_006351b0.cpp; no source of its own.




#include "shared/rad_beam_list.hpp"
#include "shared/tactical.hpp"
#include "shared/surface.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/tool_tip.hpp"
#include "shared/hicolor.hpp"
#include "shared/unknown_geometry.hpp"


// void RadBeam::unknown_6357b0(double value) (0x006357B0)

void RadBeam::unknown_6357b0(double value)
{
    unknown_40 = value;
}


// void RadBeam::unknown_6357d0(int value) (0x006357D0)

void RadBeam::unknown_6357d0(int value)
{
    unknown_48 = value;
}


// void RadBeam::Deactivate() (0x006357E0)

void RadBeam::Deactivate()
{
    IsActive = false;
}


// bool RadBeam::Is_Active() const (0x006357F0)

bool RadBeam::Is_Active() const
{
    return IsActive;
}


// void RadBeam::Set_Step_Length(int length) (0x00635800)

void RadBeam::Set_Step_Length(int length)
{
    StepLength = length;
}


// void RadBeam::Draw_Line(CoordStruct const &start, CoordStruct const &end, short color) (0x00635810)
// Draws one segment of the beam on the current surface, clipped to the
// tactical area, its ends' depths from the beam's Depth and their heights.


void RadBeam::Draw_Line(CoordStruct const &start, CoordStruct const &end, short color)
{
    Point2D first_pixel;
    Point2D second_pixel;
    unknown_tactical_83996c->Coord_To_Pixel(start, first_pixel);
    unknown_tactical_83996c->Coord_To_Pixel(end, second_pixel);
    Rect clip = unknown_rect_839630;
    if (Clip_Line(first_pixel, second_pixel, clip)) {
        int first_z = Depth - Unknown_006A2970(start.Z) - 2;
        int second_z = 0 - Unknown_006A2970(end.Z) - 2;
        unknown_surface_83995c->Draw_Depth_Shaded_Line(unknown_rect_839630, first_pixel, second_pixel,
                                                       color, first_z, second_z);
    }
}


bool RadBeam::unknown_6358d0()
{
    if (!unknown_00) {
        unknown_00=true;
        Compute_Delta();
    }
    if (IsActive) {
        Steps=(int)(Length/StepLength);
        int x=Delta.X;
        int y=Delta.Y;
        int z=Delta.Z;
        if(Steps!=0) { x/=Steps; y/=Steps; z/=Steps; }
        StepDelta=CoordStruct(x,y,z);
        Current=Source;
        double phase=unknown_6c;
        phase/=Period;
        unknown_70=phase*Amplitude;
        CurrentColor=Color;
        for(unknown_78=0;double(unknown_78)*StepLength<Length;++unknown_78) {
            unknown_635c30();
            unsigned short green=CurrentColor.Green;
            short pixel=(short)Build_Hicolor_Pixel(CurrentColor.Red,green,CurrentColor.Blue);
            Draw_Line(Current,unknown_98,pixel);
        }
    }
    ++unknown_6c;
    return unknown_6c>=Period;
}


// void RadBeam::Compute_Delta() (0x00635B30)

void RadBeam::Compute_Delta()
{
    Delta = Target - Source;
    Length = unknown_sqrt_4ba410(Delta.X * Delta.X + Delta.Y * Delta.Y + Delta.Z * Delta.Z);
}


// void RadBeam::Start_Pass() (0x00635BA0)
// Steps, the per-step delta, the first step, the wave value and the colour for a new pass (no
// caller: unknown_6358d0 0x006358D0 writes the same code out).

void RadBeam::Start_Pass()
{
    Steps = (int)(Length / StepLength);
    int x = Delta.X;
    int y = Delta.Y;
    int z = Delta.Z;
    if (Steps != 0) {
        x /= Steps;
        y /= Steps;
        z /= Steps;
    }
    StepDelta = CoordStruct(x, y, z);
    Current = Source;
    double phase = unknown_6c;
    phase /= Period;
    unknown_70 = phase * Amplitude;
    CurrentColor = Color;
}


// Not exact (RA2_NON_MATCHING): RadBeam::unknown_635c30 at 0x00635C30, best score 96.92, 2026-11-13; differs: same instruction count (130), shape 96.92: instruction choice and registers differ
#ifdef RA2_NON_MATCHING
// NON-EXACT DRAFT 0x635C30 FUN_00635c30
// entry 0x635C30; exact-match score 100.0 (misses_index best), last lane score 96.92; plan "draft as-is"
// origin lane: Scout993 (build/class-work/Scout993/w/c0.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1061 (native/drafts.json). Not original bytes.
// RadBeam::unknown_635c30 (0x00635C30; type 1 twin of 0x00636130: the wave spans twice the length, at least 360 degrees)
// The ends of step unknown_78 (Current) and the next (unknown_98): Source
// moved by whole steps, raised by the wave (a sine over the beam's length,
// scaled by unknown_70); the step's colour is Color scaled by
// unknown_a8 = 0.5 + 0.5 * |wave| / Amplitude.
#include "shared/fast_math.hpp"
#include "shared/crt_memory.hpp"
extern double unknown_deg_to_rad_7ee398;

inline void Tint5c30(RGBClass &c, double r, double b, double g) { c.Red = (unsigned char)r; c.Green = (unsigned char)g; c.Blue = (unsigned char)b; }
inline void Scale5c30(RGBClass &c, RGBClass const &from, double f) { double r = from.Red * f; double g = from.Green * f; double b = from.Blue * f; c.Red = (unsigned char)r; c.Green = (unsigned char)g; c.Blue = (unsigned char)b; }
void RadBeam::unknown_635c30()
{
    double span = (Length < 360.0 ? 360.0 : Length) * 2.0;
    double index = unknown_78;
    double degrees = index / Steps * span;
    double wave = Fast_Sin(unknown_deg_to_rad_7ee398 * degrees) * unknown_70;
    int step = (int)index;
    Current = Source + CoordStruct(StepDelta.X * step, StepDelta.Y * step, StepDelta.Z * step);
    Current.Z = (int)(Current.Z + wave);

    index = unknown_78 + 1;
    degrees = index / Steps * span;
    wave = Fast_Sin(unknown_deg_to_rad_7ee398 * degrees) * unknown_70;
    step = (int)index;
    unknown_98 = Source + CoordStruct(StepDelta.X * step, StepDelta.Y * step, StepDelta.Z * step);
    unknown_98.Z = (int)(unknown_98.Z + wave);

    unknown_a8 = abs((int)wave) * 0.5 / Amplitude + 0.5;
    Scale5c30(CurrentColor, Color, unknown_a8);
}
#endif

// bool RadBeam::unknown_635e00() (0x00635E00)
// One frame of a type 2 beam: set up on the first frame, then (while active)
// the wave is recomputed and every step drawn as a segment in its colour.
// True once the period is over.

bool RadBeam::unknown_635e00()
{
    if (!unknown_00) {
        unknown_00 = true;
        unknown_635f80();
    }
    if (IsActive) {
        unknown_636020();
        for (unknown_78 = 0; unknown_78 < Steps; unknown_78++) {
            unknown_636130();
            Draw_Line(Current, unknown_98, Build_Hicolor_Pixel(CurrentColor.Red, CurrentColor.Green, CurrentColor.Blue));
        }
    }
    unknown_6c++;
    return unknown_6c >= Period;
}


// void RadBeam::unknown_635f80() (0x00635F80)

void RadBeam::unknown_635f80()
{
    Delta = Target - Source;
    Length = unknown_sqrt_4ba410(Delta.X * Delta.X + Delta.Y * Delta.Y + Delta.Z * Delta.Z);
    Steps = (int)(Length / StepLength);
    int x = Delta.X;
    int y = Delta.Y;
    int z = Delta.Z;
    if (Steps != 0) {
        x /= Steps;
        y /= Steps;
        z /= Steps;
    }
    unknown_60 = CoordStruct(x, y, z);
}


// void RadBeam::unknown_636020() (0x00636020)

void RadBeam::unknown_636020()
{
    int period = Period;
    double phase;
    if (unknown_6c < period / 2) {
        phase = unknown_6c * 2;
    } else {
        phase = period - unknown_6c * 2 % period;
    }
    unknown_70 = phase / period * Amplitude;
    int offset = (int)(unknown_48 * unknown_6c * unknown_40);
    Delta = Delta + CoordStruct(unknown_60.X * offset, unknown_60.Y * offset, unknown_60.Z * offset);
    Length = unknown_sqrt_4ba410(Delta.X * Delta.X + Delta.Y * Delta.Y + Delta.Z * Delta.Z);
    Steps = (int)(Length / StepLength);
    int x = Delta.X;
    int y = Delta.Y;
    int z = Delta.Z;
    if (Steps != 0) {
        x /= Steps;
        y /= Steps;
        z /= Steps;
    }
    StepDelta = CoordStruct(x, y, z);
}


// Not exact (RA2_NON_MATCHING): RadBeam::unknown_636130 at 0x00636130, best score 71.90, 2026-11-13; differs: 120 instructions in the original, 122 in the draft (shape 94.21)
#ifdef RA2_NON_MATCHING
// NON-EXACT DRAFT 0x636130 FUN_00636130
// entry 0x636130; exact-match score 95.83 (misses_index best); plan "draft as-is"
// origin lane: Scout993 (build/class-work/Scout993/w/a3.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1069 (native/drafts.json). Not original bytes.
// Native adaptation (Scout1069): RA2_COORD_TIMES operator* spelled out; each changed line is marked NATIVE.
// RadBeam::unknown_636130 (0x00636130)
// The ends of step unknown_78 (Current) and the next (unknown_98): Source
// moved by whole steps, raised by the wave (a sine over the beam's length,
// scaled by unknown_70); the step's colour is Color scaled by
// unknown_a8 = 0.5 + 0.5 * |wave| / Amplitude.
extern double unknown_deg_to_rad_7ee398;

inline void Scale6130(RGBClass &c, RGBClass const &from, double f) { double r = from.Red * f; double g = from.Green * f; double b = from.Blue * f; c.Red = (unsigned char)r; c.Green = (unsigned char)g; c.Blue = (unsigned char)b; }
void RadBeam::unknown_636130()
{
    double index = unknown_78;
    double degrees = index / Steps * 180.0;
    double wave = Fast_Sin(unknown_deg_to_rad_7ee398 * degrees) * unknown_70;
    int step = (int)index;
    Current = Source + CoordStruct(StepDelta.X * step, StepDelta.Y * step, StepDelta.Z * step);   // NATIVE: RA2_COORD_TIMES operator* spelled out
    Current.Z = (int)(Current.Z + wave);

    index = unknown_78 + 1;
    degrees = index / Steps * 180.0;
    wave = Fast_Sin(unknown_deg_to_rad_7ee398 * degrees) * unknown_70;
    step = (int)index;
    unknown_98 = Source + CoordStruct(StepDelta.X * step, StepDelta.Y * step, StepDelta.Z * step);   // NATIVE: RA2_COORD_TIMES operator* spelled out
    unknown_98.Z = (int)(unknown_98.Z + wave);

    unknown_a8 = abs((int)wave) * 0.5 / Amplitude + 0.5;
    Scale6130(CurrentColor, Color, unknown_a8);
}
#endif

// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<RadBeam *>::Clear (0x006362E0; DynamicVectorClass<RadBeam *> vtable 0x007a885c slot 3).
// DynamicVectorClass<RadBeam *>::ID_pointer (0x00636310; DynamicVectorClass<RadBeam *> vtable 0x007a885c slot 5).
// VectorClass<RadBeam *>::Fetch (0x00636330; VectorClass<RadBeam *> vtable 0x007a887c slot 6).
// VectorClass<RadBeam *>::operator equal (0x00636340; VectorClass<RadBeam *> vtable 0x007a887c slot 1).
// VectorClass<RadBeam *>::Resize (0x00636380; VectorClass<RadBeam *> vtable 0x007a887c slot 2).
// VectorClass<RadBeam *>::Clear (0x00636430; VectorClass<RadBeam *> vtable 0x007a887c slot 3).
// VectorClass<RadBeam *>::ID_object (0x00636460; VectorClass<RadBeam *> vtable 0x007a887c slot 4).
// VectorClass<RadBeam *>::ID_pointer (0x006364A0; VectorClass<RadBeam *> vtable 0x007a887c slot 5).
// DynamicVectorClass<RadBeam *>::Resize (0x006364C0).
// DynamicVectorClass<RadBeam *>::ID_object (0x00636570; DynamicVectorClass<RadBeam *> vtable 0x007a885c slot 4).


extern "C" unsigned char vtable_6365a0[]; extern "C" void __cdecl release_buffer_6365a0(void*); extern "C" void __cdecl release_object_6365a0(void*);
class Leaf_6365a0 { public: void* get(unsigned int flags); };
void* Leaf_6365a0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_6365a0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_6365a0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_6365a0(this); return this; }


extern "C" unsigned char vtable_6365f0[]; extern "C" void __cdecl release_buffer_6365f0(void*); extern "C" void __cdecl release_object_6365f0(void*);
class Leaf_6365f0 { public: void* get(unsigned int flags); };
void* Leaf_6365f0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_6365f0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_6365f0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_6365f0(this); return this; }
