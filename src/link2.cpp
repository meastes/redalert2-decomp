/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/

/*
**	RA2: the line trails: the fading line an object draws behind itself. TibSun has no such file (YR calls it
**	link2.cpp); the code is the executable's from 0x005393E0.
*/

#include "geometry.h" // RA2: the geometry constants come before the rectangle of rect.h (the executable initializes them first)
#include "always.h" // RA2: the includes this file needs
#include "linetrail.h"
#include "dsurface.h"
#include "fastmath.h"
#include "globals.h"
#include "goptions.h"
#include "object.h"
#include "tactical.h"
#include "vector.h"
#include "_rect.h"
#include "_surface.h"
#include "_tactica.h"



template<> inline bool DynamicVectorClass<LineTrail *>::Add(LineTrail * const & object) // RA2: this unit's Add holds the new length in a variable before the Resize call (the sum is formed in the register that held Length)
{
	if (ActiveCount >= Length()) {
		if ((IsAllocated || !VectorMax) && GrowthStep > 0) {
			int new_length = Length();
			new_length += GrowthStep;
			if (!Resize(new_length)) {
				return(false);
			}
		} else {
			return(false);
		}
	}
	(*this)[ActiveCount++] = object;
	return(true);
}

DynamicVectorClass<LineTrail *> LineTrailArray; // RA2: 0x00A6E970, every trail that exists


LineTrail::LineTrail(void) : // RA2: at 0x005393E0
	Color(128, 128, 128),
	Owner(NULL),
	Decrement(16),
	ActiveSlot(0)
{
	for (int index = 0; index < 32; index++) {
		Trails[index].Position = COORD_NONE;
		Trails[index].Value = 0;
	}
	LineTrailArray.Add(this);
}


LineTrail::~LineTrail(void) // RA2: at 0x00539490
{
	if (Owner != NULL) {
		Owner->LineTrailer = NULL;
		Owner = NULL;
	}
	LineTrailArray.Delete(this);
}


void LineTrail::Release_Owner(void) // RA2: at 0x005394F0
{
	if (Owner != NULL) {
		Owner->LineTrailer = NULL;
		Owner = NULL;
	}
}


void LineTrail::Set_Decrement(int value) // RA2: at 0x00539510
{
	if (!Options.DetailLevel) value <<= 1;
	Decrement = value;
}


void LineTrail::Update(void) // RA2: at 0x00539530
{
	if (Owner != NULL) {
		Coord position = Owner->Position;
		if (position != Trails[ActiveSlot].Position) {
			int & active = ActiveSlot;
			if (--active < 0) active = 31;
			LineTrailNode * node = &Trails[active];
			node->Position = position;
			node->Value = 255;
		}
	}

	int slot = ActiveSlot;
	do {
		LineTrailNode * fade = &Trails[slot];
		fade->Value -= Decrement;
		if (fade->Value < 0) fade->Value = 0;
		slot++;
		if (slot >= 32) slot = 0;
	} while (slot != ActiveSlot);
}


void LineTrail::Draw(void) // RA2: at 0x005395C0
{
	int slot = ActiveSlot + 1;
	if (slot >= 32) slot = 0;

	LineTrailNode * to = &Trails[ActiveSlot];
	if (to->Position == COORD_NONE) return;

	for (;;) {
		LineTrailNode * from = to;
		to = &Trails[slot];
		if (from->Value == 0 || to->Position == COORD_NONE) break;

		Point2D start;
		Point2D end;
		TacticalMap->Coord_To_Pixel(from->Position, start);
		TacticalMap->Coord_To_Pixel(to->Position, end);
		int start_height = -2 - Tactical::Z_Lepton_To_Pixel(from->Position.Z);
		int end_height = -2 - Tactical::Z_Lepton_To_Pixel(to->Position.Z);
		((DSurface *)CompositeSurface)->Draw_Trail_Line(TacticalRect, start, end, Color, from->Value, start_height, end_height);

		slot++;
		if (slot >= 32) slot = 0;
		if (slot == ActiveSlot) break;
	}
}


bool LineTrail::Is_Expired(void) const // RA2: at 0x005396E0
{
	return(Owner == NULL && Trails[ActiveSlot].Value == 0);
}


void LineTrail::Process_All(void) // RA2: at 0x00539700
{
	for (int index = LineTrailArray.Count() - 1; index >= 0; index--) {
		LineTrail * trail = LineTrailArray[index];
		trail->Update();
		if (trail->Is_Expired()) {
			delete trail;
		} else {
			trail->Draw();
		}
	}
}


void __cdecl Delete_All_Line_Trails(void) // RA2: at 0x005397B0, called by init.cpp
{
	for (int index = 0; index < LineTrailArray.Count(); index++) {
		delete LineTrailArray[index];
	}
	LineTrailArray.Clear();
}
