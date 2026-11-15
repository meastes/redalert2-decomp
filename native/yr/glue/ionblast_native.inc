
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y3), appended to the ionblast unit: IonBlastClass::One_Time, YR
// 0x0053D330 (541 bytes, unmatched in ra2-decomp; init.cpp calls it FUN_0053D330, bound by
// data/name_aliases.tsv). From the RA2 tree's draft (src/recovered/ionblast.cpp 0x00521A90, from
// TibSun's ionblast.cpp) with gamemd.exe's sizes: the 80 shockwave surfaces are 512x256 (centre
// 0x10100, quadrant 256x128, rows of 512) and the band stops at 256.0; the other constants
// (38, 0.11, 3.5, 3, 51, 1, 0.5, 1.2) read from gamemd.exe's .rdata are the RA2 ones.
// Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------
void IonBlastClass::One_Time(void)
{
	static bool one_time = false;
	if (one_time) {
		return;
	}

	double min_dist = WaveStartDistance - WaveBandWidth;
	double max_dist = WaveStartDistance;

	for (int frame = 0; frame < ARRAY_SIZE(BlastSurfaces); frame++) {
		BlastSurfaces[frame] = new BSurface(512, 256, 1);
		BlastSurfaces[frame]->Fill(-1);

		char *center = (char *)BlastSurfaces[frame]->Lock() + 0x10100;
		for (int y = 127; y >= 0; y--) {
			int y_dist_sq = y * y * 4;
			for (int x = 255; x >= 0; x--) {
				double dist = fastmath::sqrt(x * x + y_dist_sq);
				if (dist >= min_dist && dist <= max_dist) {
					double wave_height = (fastmath::sin((dist - frame * WavePhaseSpeed + 38.0) * WaveFrequency) * 3.5 + 3.0)
					                     / (dist / WaveAmplitudeFalloff + 1.0) + 0.5;
					char spiral_index = Point_To_Spiral_Index(Point2D(0, (int)wave_height));
					int row = y << 9;
					center[row + x] = spiral_index;
					center[row - x] = spiral_index;
					char *mirror = center - row;
					mirror[x] = spiral_index;
					mirror[-x] = spiral_index;
				}
			}
		}

		if (256.0 - WaveSpeed > max_dist) {
			max_dist += WaveSpeed;
		}
		min_dist = WaveSpeed * 1.2 + min_dist;
		if (min_dist > max_dist) {
			min_dist = max_dist;
		}
	}

	one_time = true;
}
