// SPDX-License-Identifier: GPL-3.0-or-later
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
#ifdef RA2_NON_MATCHING // not exact: 755 bytes differ (written fresh)
void RadBeam::Update_All(void)
{
	int swings = 0;
	int active = 0;
	for (int i = Array.Count() - 1; i >= 0; i--) {
		if (Array[i]->Type == 2) {
			swings++;
			if (Array[i]->IsActive) {
				active++;
			}
		}
	}

	double speed = (double)FPSCounter::CurrentFrameRate / Session.DesiredFrameRate;
	speed = speed * speed * speed;
	double excess = (double)active / swings - speed;
	double drop = 0.0;
	if (excess > 0.0) {
		drop = swings * excess;
	}
	drop = drop / active;
	speed = 1.0 / speed;

	for (int index = Array.Count() - 1; index >= 0; index--) {
		RadBeam * beam = Array[index];
		bool done;
		if (beam->Type == 1) {
			int step = speed * 10.0;
			if (step < 10) {
				step = 10;
			} else if (step > 100) {
				step = 100;
			}
			beam->StepLength = step;
			done = beam->Draw_Pulse();
		} else if (beam->Type == 2) {
			if (beam->IsActive) {
				if (NonCriticalRandomNumber(0, 99) < (int)(drop * 100.0)) {
					beam->IsActive = false;
				}
			}
			done = beam->Draw_Swing();
		} else {
			continue;
		}
		if (done) {
			Array.Delete(beam);
			operator delete(beam);
		}
	}
}
#endif


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
#ifdef RA2_NON_MATCHING // not exact: 9 bytes differ (the pixel takes blue first and red last in the original)
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
			Draw_Line(Current, Next, DSurface::Build_Hicolor_Pixel(CurrentColor));
		}
	}
	return(++Time >= Period);
}
#endif


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


#ifdef RA2_NON_MATCHING // not exact: not yet written (no independent bound for 0x635C30)
void RadBeam::Compute_Pulse_Step(void)
{
	Next = Current + StepDelta;
}
#endif


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
			Draw_Line(Current, Next, DSurface::Build_Hicolor_Pixel(CurrentColor));
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


#ifdef RA2_NON_MATCHING // not exact: not yet written (no independent bound for 0x636130)
void RadBeam::Compute_Swing_Step(void)
{
	Next = Current + StepDelta;
}
#endif
