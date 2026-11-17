// SPDX-License-Identifier: GPL-3.0-or-later
// RA2: not in TibSun. The rad / chrono beam (names from YRpp's RadBeam.h where it has one; the members are named
// after what radbeam.cpp does with them).
#pragma once

#include "coord.h"
#include "rgb.h"
#include "vector.h"

class RadBeam
{
	public:
		RadBeam(int type); // RA2: 0x6356B0
		static RadBeam * __fastcall Allocate(int type); // RA2: 0x635440; 1 the rad beam (10 frames), else 20
		static void __fastcall Update_All(void); // RA2: 0x6354E0; draws every listed beam once and deletes the ones that are done
		void Set_Color(RGBClass const & color); // RA2: 0x635710
		void Set_Depth(int depth); // RA2: 0x635730
		void Set_Source(Coord const & coord); // RA2: 0x635740
		void Set_Target(Coord const & coord); // RA2: 0x635760
		void Set_Period(int period); // RA2: 0x635780
		void Set_Amplitude(double amplitude); // RA2: 0x635790
		void Set_Phase(double phase); // RA2: 0x6357B0; +0x40, 0.0 for the eruption
		void Set_Start_Delay(int delay); // RA2: 0x6357D0; +0x48, a random 0-20 for the eruption
		void Deactivate(void); // RA2: 0x6357E0
		bool Is_Active(void) const; // RA2: 0x6357F0
		void Set_Step_Length(int length); // RA2: 0x635800
		void Draw_Line(Coord const & start, Coord const & end, short color); // RA2: 0x635810; one clipped, depth shaded segment
		bool Draw_Pulse(void); // RA2: 0x6358D0; one frame of a type 1 beam, true once its period is over
		void Compute_Delta(void); // RA2: 0x635B30
		void Start_Pass(void); // RA2: 0x635BA0
		void Compute_Pulse_Step(void); // RA2: 0x635C30; the ends and the color of step StepIndex of a type 1 beam
		bool Draw_Swing(void); // RA2: 0x635E00; one frame of a type 2 beam, true once its period is over
		void Setup_Swing(void); // RA2: 0x635F80
		void Update_Swing(void); // RA2: 0x636020
		void Compute_Swing_Step(void); // RA2: 0x636130; the ends and the color of step StepIndex of a type 2 beam

		static DynamicVectorClass<RadBeam *> Array; // RA2: 0xAB64E0; the live beams (its initializer is the one at 0x6353A0)

	private:
		bool Prepared; // RA2: +0x00; set by the first Draw_Pulse / Draw_Swing
		bool IsActive; // RA2: +0x01
		int Depth; // RA2: +0x04; the screen height difference of the ends
		int Type; // RA2: +0x08; 1 pulse, 2 swing
		double StepLength; // RA2: +0x10
		RGBClass Color; // RA2: +0x18
		Coord Source; // RA2: +0x1C
		Coord Target; // RA2: +0x28
		int Period; // RA2: +0x34
		double Amplitude; // RA2: +0x38
		double Phase; // RA2: +0x40
		int StartDelay; // RA2: +0x48
		Coord Delta; // RA2: +0x4C; Target - Source
		double Length; // RA2: +0x58; the length of Delta
		Coord SwingDelta; // RA2: +0x60; Delta / Steps of the swing beam
		int Time; // RA2: +0x6C; the frame within the period
		double Wave; // RA2: +0x70
		int StepIndex; // RA2: +0x78
		int Steps; // RA2: +0x7C
		Coord StepDelta; // RA2: +0x80
		Coord Current; // RA2: +0x8C; the start of the step being drawn
		Coord Next; // RA2: +0x98; its end
		double Scale; // RA2: +0xA8; the step's color scale
		RGBClass CurrentColor; // RA2: +0xB0
};
