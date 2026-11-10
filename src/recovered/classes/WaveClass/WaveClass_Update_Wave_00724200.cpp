// Advances the wave's head/tail, updates its six-point outline, and retires it.
// QWORD progress additions and the original X^2+Z^2+Y^2 sum order are intentional.
// @ref __real@8@3ffefae147ae147ae000=007aeea0
// @ref __real@8@4001c000000000000000=007a2fa0
#include "shared/wave.hpp"
#include "shared/fast_math.hpp"
#include "shared/unknown_geometry.hpp"
extern float const Wave_Advance_Step_007aec88;
// @ref ?Wave_Advance_Step_007aec88@@3MB=007aec88
inline Point2D Interpolate_Wave_Pixel(Point2D const &a, Point2D const &b, float amount)
{
    return Point2D((int)(a.X * amount + b.X * (1.0 - amount)), (int)(a.Y * amount + b.Y * (1.0 - amount)));
}
void WaveClass::Update_Wave_00724200()
{
    if (WaveProgress - FadeProgress > 0.1 && Type == 0) unknown_119 = true;
    if (Target && Owner && WaveEC != (int)(1.0 / Wave_Advance_Step_007aec88) && Owner->unknown_228 == Target) {
        CoordStruct source = Owner->Coord;
        CoordStruct delta = source - Target->unknown_slot22();
        int distance = (int)Fast_Sqrt((double)delta.X * delta.X + (double)delta.Z * delta.Z + (double)delta.Y * delta.Y);
        if (distance <= unknown_geometry_diagonal * 6.0) goto update_geometry;
    }
    IsWaveActive = false;
    unknown_119 = true;
update_geometry:
    if (IsWaveActive) {
        TechnoClass *owner = Owner;
        Draw_NonMagnetic_007238b0(owner->unknown_slot43(0), Target->unknown_slot22());
    }
    if (WaveProgress < 1.0 && IsWaveActive) {
        WaveProgress += Wave_Advance_Step_007aec88;
        if (WaveProgress > 0.98) WaveProgress = 1.0;
        Polygon_130.Points[0] = Interpolate_Wave_Pixel(WaveEndLeft, WaveStartLeft, WaveProgress);
        Polygon_130.Points[1] = Interpolate_Wave_Pixel(WaveEndMiddle, WaveStartMiddle, WaveProgress);
        Polygon_130.Points[2] = Interpolate_Wave_Pixel(WaveEndRight, WaveStartRight, WaveProgress);
    }
    if (unknown_119 && FadeProgress <= Wave_Advance_Step_007aec88 * 0.5 + WaveProgress) {
        FadeProgress += Wave_Advance_Step_007aec88;
        if (FadeProgress >= WaveProgress) {
            unknown_slot60();
            return;
        }
        Polygon_130.Points[3] = Interpolate_Wave_Pixel(WaveEndRight, WaveStartRight, FadeProgress);
        Polygon_130.Points[4] = Interpolate_Wave_Pixel(WaveEndMiddle, WaveStartMiddle, FadeProgress);
        Polygon_130.Points[5] = Interpolate_Wave_Pixel(WaveEndLeft, WaveStartLeft, FadeProgress);
    }
    Wave_Recalc_Affected_Cells();
}
