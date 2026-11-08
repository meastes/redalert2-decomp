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
#include "shared/rad_beam_list.hpp"
#include "shared/fast_math.hpp"
#include "shared/crt_memory.hpp"
extern double unknown_deg_to_rad_7ee398;
// @ref ?unknown_deg_to_rad_7ee398@@3NA=007ee398
// @ref __real@8@4007b400000000000000=007a8898

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
