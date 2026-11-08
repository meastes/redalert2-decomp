// NON-EXACT DRAFT 0x521A90 IonBlastClass::One_Time
// entry 0x521A90; exact-match score 88.31 (/O2 /Ob2 /G5, shape 99.35; Scout1076); plan "draft, verify"
// origin lane: Scout1076 (written from the adopted TibSun ionblast.cpp text, THIRD_PARTY.md)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// native/drafts.json. Not original bytes.
// IonBlastClass::One_Time() (0x00521A90): builds the 80 shockwave displacement surfaces
// once (256x128, one byte per pixel, filled with -1). Each frame rasterises the ripple
// band [min_dist, max_dist] of one quadrant around the centre (0x4080) and mirrors it;
// a pixel holds the spiral index of the wave height. Both band edges then advance by
// WaveSpeed. The constants (0, 57, 38, 0.11, 3.5, 3, 25, 1, 0.5, 128, 1.2) were checked
// against the pinned target's .rdata; WavePhaseSpeed/WaveSpeed are IonBlast.cpp's statics.
double __cdecl unknown_sin_4ba480(double angle);    // 0x004BA480 (fast sine)

void IonBlastClass::One_Time()
{
    static bool one_time = false;
    if (one_time) {
        return;
    }

    double min_dist = WaveStartDistance - WaveBandWidth;
    double max_dist = WaveStartDistance;

    for (int frame = 0; frame < 80; frame++) {
        BlastSurfaces[frame] = new BSurface(256, 128, 1);
        BlastSurfaces[frame]->Fill(-1);

        char *center = (char *)BlastSurfaces[frame]->Lock() + 0x4080;
        for (int y = 63; y >= 0; y--) {
            int y_dist_sq = y * y * 4;
            for (int x = 127; x >= 0; x--) {
                double dist = unknown_sqrt_4ba410(x * x + y_dist_sq);
                if (dist >= min_dist && dist <= max_dist) {
                    double wave_height = (unknown_sin_4ba480((dist - frame * WavePhaseSpeed + 38.0) * WaveFrequency) * 3.5 + 3.0)
                                         / (dist / WaveAmplitudeFalloff + 1.0) + 0.5;
                    char spiral_index = Point_To_Spiral_Index(Point2D(0, wave_height));
                    int row = y << 8;
                    center[row + x] = spiral_index;
                    center[row - x] = spiral_index;
                    char *mirror = center - row;
                    mirror[x] = spiral_index;
                    mirror[-x] = spiral_index;
                }
            }
        }

        if (128.0 - WaveSpeed > max_dist) {
            max_dist += WaveSpeed;
        }

        min_dist = WaveSpeed * 1.2 + min_dist;

        if (min_dist > max_dist) {
            min_dist = max_dist;
        }
    }

    one_time = true;
}
