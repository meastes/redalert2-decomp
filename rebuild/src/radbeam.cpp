// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// RA2: not in TibSun. The rad and chrono beams: a type 1 beam pulses along the line from Source to
// Target, a type 2 beam swings; Update_All draws every listed beam once a frame and deletes the
// ones whose period is over.

#include "always.h"
#include "geometry.h" // RA2: the header statics, in the order of the unit's initializer table
#include "radbeam.h"

#include "_rect.h"
#include "_surface.h"
#include "_tactica.h"
#include "ccrand.h"
#include "dsurface.h"
#include "fastmath.h"
#include "fpscounter.h"
#include "session.h"
#include "tactical.h"


DynamicVectorClass<RadBeam *> RadBeam::Array;


/// <summary>Makes a beam of the given type and lists it; a rad beam steps every 10 leptons, the others every 20.</summary>
RadBeam * RadBeam::Allocate(int type)
{
	RadBeam * beam = new RadBeam(type);
	Array.Add(beam);
	beam->StepLength = (type == 1) ? 10 : 20;
	return(beam);
}


/// <summary>
/// Draws every listed beam once. Swing beams switch off at random when the game runs slower than
/// the frame rate it wants; pulse beams take longer steps then. A beam whose period is over is
/// taken off the list and deleted.
/// </summary>
void RadBeam::Update_All(void)
{
	int total = Array.Count();
	int count = 0;
	int active = 0;
	for (int index = total - 1; index >= 0; index--) {
		RadBeam * beam = Array[index];
		if (beam->Type == 2) {
			count++;
			if (beam->IsActive) {
				active++;
			}
		}
	}

	double desired = Session.DesiredFrameRate;
	double rate = FPSCounter::CurrentFrameRate / desired;
	rate = rate * rate * rate;
	double excess = (double)active / count - rate;
	double cull = 0.0;
	if (excess > 0.0) {
		cull = count * excess;
	}
	cull /= active;
	double stretch = 1.0 / rate;

	for (index = total - 1; index >= 0; index--) {
		RadBeam * beam = Array[index];
		bool done;
		if (beam->Type == 1) {
			int length = (int)(stretch * 10.0);
			if (length < 10) {
				length = 10;
			} else if (length > 100) {
				length = 100;
			}
			beam->Set_Step_Length(length);
			done = beam->Draw_Pulse();
		} else if (beam->Type == 2) {
			if (beam->IsActive) {
				if (NonCriticalRandomNumber(0, 99) < (int)(cull * 100.0)) {
					beam->Deactivate();
				}
			}
			done = beam->Draw_Swing();
		} else {
			continue;
		}
		if (done) {
			Array.Delete(beam);
			delete beam;
		}
	}
}


RadBeam::RadBeam(int type) :
	Prepared(false),
	IsActive(true),
	Depth(0),
	Type(type),
	StepLength(0),
	Color(0, 0, 0),
	Source(0, 0, 0),
	Target(0, 0, 0),
	Period(0),
	Amplitude(0),
	Phase(0),
	StartDelay(0),
	Time(0),
	CurrentColor(0, 0, 0)
{
}


void RadBeam::Set_Color(RGBClass const & color)
{
	Color = color;
}


void RadBeam::Set_Depth(int depth)
{
	Depth = depth;
}


void RadBeam::Set_Source(Coord const & coord)
{
	Source = coord;
}


void RadBeam::Set_Target(Coord const & coord)
{
	Target = coord;
}


void RadBeam::Set_Period(int period)
{
	Period = period;
}


void RadBeam::Set_Amplitude(double amplitude)
{
	Amplitude = amplitude;
}


void RadBeam::Set_Phase(double phase)
{
	Phase = phase;
}


void RadBeam::Set_Start_Delay(int delay)
{
	StartDelay = delay;
}


void RadBeam::Deactivate(void)
{
	IsActive = false;
}


bool RadBeam::Is_Active(void) const
{
	return(IsActive);
}


void RadBeam::Set_Step_Length(int length)
{
	StepLength = length;
}


/// <summary>Draws one segment of the beam, clipped to the tactical view and shaded by the height of its ends.</summary>
void RadBeam::Draw_Line(Coord const & start, Coord const & end, short color)
{
	Point2D startpixel;
	Point2D endpixel;
	TacticalMap->Coord_To_Pixel(start, startpixel);
	TacticalMap->Coord_To_Pixel(end, endpixel);
	Rect clip = TacticalRect;
	if (Clip_Line_To_Rect(startpixel, endpixel, clip)) {
		int startdepth = Depth - Tactical::Z_Lepton_To_Pixel(start.Z) - 2;
		int enddepth = -2 - Tactical::Z_Lepton_To_Pixel(end.Z);
		LogicalSurface->Draw_Depth_Shaded_Line(TacticalRect, startpixel, endpixel, color, startdepth, enddepth);
	}
}


/// <summary>One frame of a type 1 beam: the line is drawn step by step; true once the period is over.</summary>
bool RadBeam::Draw_Pulse(void)
{
	if (!Prepared) {
		Prepared = true;
		Compute_Delta();
	}
	if (IsActive) {
		Steps = Length / StepLength;
		int x = Delta.X;
		int y = Delta.Y;
		int z = Delta.Z;
		if (Steps != 0) {
			x /= Steps;
			y /= Steps;
			z /= Steps;
		}
		StepDelta = Coord(x, y, z);
		Current = Source;
		double phase = Time;
		phase /= Period;
		Wave = phase * Amplitude;
		CurrentColor = Color;
		for (StepIndex = 0; StepIndex * StepLength < Length; StepIndex++) {
			Compute_Pulse_Step();
			unsigned short green = ((RGBStruct const &)CurrentColor).Green; // RA2: green is read first (ra2-decomp's twin text)
			short pixel = (short)DSurface::Build_Hicolor_Pixel(((RGBStruct const &)CurrentColor).Red, green, ((RGBStruct const &)CurrentColor).Blue);
			Draw_Line(Current, Next, pixel);
		}
	}
	return(++Time >= Period);
}


/// <summary>The vector from the source to the target and its length.</summary>
void RadBeam::Compute_Delta(void)
{
	Delta = Target - Source;
	Length = fastmath::sqrt(Delta.X * Delta.X + Delta.Y * Delta.Y + Delta.Z * Delta.Z);
}


/// <summary>Starts a pass along the beam: the number of steps and the step, the start, the wave and the color.</summary>
void RadBeam::Start_Pass(void)
{
	Steps = Length / StepLength;
	int x = Delta.X;
	int y = Delta.Y;
	int z = Delta.Z;
	if (Steps != 0) {
		x /= Steps;
		y /= Steps;
		z /= Steps;
	}
	StepDelta = Coord(x, y, z);
	Current = Source;
	double phase = Time;
	phase /= Period;
	Wave = phase * Amplitude;
	CurrentColor = Color;
}


static double DegreesToRadians = M_PI / 180.0; // RA2: 0x007EE398, a variable of this file that only the two step routines read (ra2-decomp declares it, DAT_00839530)

void RadBeam::Compute_Pulse_Step(void) // RA2: ra2-decomp's twin text in this tree's names
{
	double span = (Length < 360.0 ? 360.0 : Length) * 2.0;
	double i = StepIndex;
	double fraction = i / Steps;
	double angle = fraction * span;
	double wave = fastmath::sin(angle * DegreesToRadians) * Wave;
	Point3D offset = StepDelta * (int)i;
	Current = Source + offset;
	double first_z = Current.Z;
	first_z += wave;
	Current.Z = first_z;
	double j = StepIndex + 1;
	fraction = j / Steps;
	angle = fraction * span;
	wave = fastmath::sin(angle * DegreesToRadians) * Wave;
	Next = Source + StepDelta * (int)j; // RA2: the swing step's statement (the twin's pulse step keeps the offset in its local here)
	double next_z = Next.Z;
	next_z += wave;
	Next.Z = next_z;
	int height = (int)wave;
	Scale = abs(height) * 0.5 / Amplitude + 0.5;
	double red = Color.Get_Red() * Scale;
	double green = Color.Get_Green() * Scale;
	double blue = Color.Get_Blue() * Scale;
	CurrentColor.Set_Red(red);
	CurrentColor.Set_Green(green);
	CurrentColor.Set_Blue(blue);
}


/// <summary>One frame of a type 2 beam: the line swings across; true once the period is over.</summary>
bool RadBeam::Draw_Swing(void)
{
	if (!Prepared) {
		Prepared = true;
		Setup_Swing();
	}
	if (IsActive) {
		Update_Swing();
		for (StepIndex = 0; StepIndex < Steps; StepIndex++) {
			Compute_Swing_Step();
			Draw_Line(Current, Next, DSurface::Build_Hicolor_Pixel(((RGBStruct const &)CurrentColor).Red, ((RGBStruct const &)CurrentColor).Green, ((RGBStruct const &)CurrentColor).Blue));
		}
	}
	return(++Time >= Period);
}


/// <summary>The swing beam's vector, its length and the per-step share of it.</summary>
void RadBeam::Setup_Swing(void)
{
	Delta = Target - Source;
	Length = fastmath::sqrt(Delta.X * Delta.X + Delta.Y * Delta.Y + Delta.Z * Delta.Z);
	Steps = Length / StepLength;
	int x = Delta.X;
	int y = Delta.Y;
	int z = Delta.Z;
	if (Steps != 0) {
		x /= Steps;
		y /= Steps;
		z /= Steps;
	}
	SwingDelta = Coord(x, y, z);
}


/// <summary>Moves the swing beam on by one frame: the wave, the vector, its length and the step.</summary>
void RadBeam::Update_Swing(void)
{
	int period = Period;
	double time;
	if (Time < period / 2) {
		time = Time * 2;
	} else {
		time = period - (Time * 2) % period;
	}
	Wave = time / (double)period * Amplitude;
	int shift = StartDelay * Time * Phase;
	Delta = Delta + SwingDelta * shift;
	Length = fastmath::sqrt(Delta.X * Delta.X + Delta.Y * Delta.Y + Delta.Z * Delta.Z);
	Steps = Length / StepLength;
	int x = Delta.X;
	int y = Delta.Y;
	int z = Delta.Z;
	if (Steps != 0) {
		x /= Steps;
		y /= Steps;
		z /= Steps;
	}
	StepDelta = Coord(x, y, z);
}


void RadBeam::Compute_Swing_Step(void) // RA2: ra2-decomp's twin text in this tree's names
{
	double i = StepIndex;
	double fraction = i / Steps;
	double angle = fraction * 180.0;
	double wave = fastmath::sin(angle * DegreesToRadians) * Wave;
	Coord offset = StepDelta * (int)i;
	Current = Source + offset;
	double first_z = Current.Z;
	first_z += wave;
	Current.Z = first_z;
	double j = StepIndex + 1;
	fraction = j / Steps;
	angle = fraction * 180.0;
	wave = fastmath::sin(angle * DegreesToRadians) * Wave;
	Next = Source + StepDelta * (int)j;
	double next_z = Next.Z;
	next_z += wave;
	Next.Z = next_z;
	int height = (int)wave;
	Scale = abs(height) * 0.5 / Amplitude + 0.5;
	double red = Color.Get_Red() * Scale;
	double green = Color.Get_Green() * Scale;
	double blue = Color.Get_Blue() * Scale;
	CurrentColor.Set_Red(red);
	CurrentColor.Set_Green(green);
	CurrentColor.Set_Blue(blue);
}
