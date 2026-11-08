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
#include "shared/rad_beam_list.hpp"
#include "shared/fast_math.hpp"
#include "shared/crt_memory.hpp"
extern double unknown_deg_to_rad_7ee398;
// @ref ?unknown_deg_to_rad_7ee398@@3NA=007ee398
// @ref __real@8@4006b400000000000000=007a88a0

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
