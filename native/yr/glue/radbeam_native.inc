
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFTS (lane Y15), appended to the radbeam unit: RadBeam::FUN_00659AC0 (gamemd.exe
// 0x00659AC0, 474 bytes) and FUN_00659FF0 (0x00659FF0, 447 bytes), unmatched in ra2-decomp. Red
// Alert 2's text of the same two routines (mischa85/RedAlert2 src/radbeam.cpp Compute_Pulse_Step
// 0x00635C30 and Compute_Swing_Step 0x00636130; SPDX GPL-3.0-or-later, Copyright 2026 OpenTS
// contributors; THIRD_PARTY.md) with the YR tree's member names: the step count unknown_8C, the step
// index unknown_88, the step length unknown_18, the wave height unknown_80, the step AndAnotherLocation,
// the segment PointA-PointB, the scale unknown_B8 and the segment's colour Tint (compared field by field
// with gamemd.exe's code: the same offsets, sines and order). Behaviour code for the native build only.
// ---------------------------------------------------------------------------------------------
void RadBeam::FUN_00659AC0(void)
{
	double const degrees_to_radians = 3.14159265358979323846 / 180.0;
	double span = (Length < 360.0 ? 360.0 : Length) * 2.0;
	double index = unknown_88;
	double degrees = index / unknown_8C * span;
	double wave = fastmath::sin(degrees_to_radians * degrees) * unknown_80;
	int step = (int)index;
	PointA = AndAnotherLocation * step + SourceLocation;
	PointA.Z = (int)(PointA.Z + wave);

	double index2 = unknown_88 + 1;
	double degrees2 = index2 / unknown_8C * span;
	double wave2 = fastmath::sin(degrees_to_radians * degrees2) * unknown_80;
	int step2 = (int)index2;
	PointB = SourceLocation + AndAnotherLocation * step2;
	PointB.Z = (int)(PointB.Z + wave2);

	unknown_B8 = abs((int)wave2) * 0.5 / Amplitude + 0.5;
	double red = Color.Get_Red() * unknown_B8;
	double green = Color.Get_Green() * unknown_B8;
	double blue = Color.Get_Blue() * unknown_B8;
	Tint = RGBClass((unsigned char)red, (unsigned char)green, (unsigned char)blue);
}

void RadBeam::FUN_00659FF0(void)
{
	double const degrees_to_radians = 3.14159265358979323846 / 180.0;
	double index = unknown_88;
	double degrees = index / unknown_8C * 180.0;
	double wave = fastmath::sin(degrees_to_radians * degrees) * unknown_80;
	int step = (int)index;
	PointA = AndAnotherLocation * step + SourceLocation;
	PointA.Z = (int)(PointA.Z + wave);

	double index2 = unknown_88 + 1;
	double degrees2 = index2 / unknown_8C * 180.0;
	double wave2 = fastmath::sin(degrees_to_radians * degrees2) * unknown_80;
	int step2 = (int)index2;
	PointB = AndAnotherLocation * step2 + SourceLocation;
	PointB.Z = (int)(PointB.Z + wave2);

	unknown_B8 = abs((int)wave2) * 0.5 / Amplitude + 0.5;
	double red = Color.Get_Red() * unknown_B8;
	double green = Color.Get_Green() * unknown_B8;
	double blue = Color.Get_Blue() * unknown_B8;
	Tint = RGBClass((unsigned char)red, (unsigned char)green, (unsigned char)blue);
}
