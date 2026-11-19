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
#include "linetrail.h" // RA2: as above
#include "dsurface.h" // RA2: as above
#include "fastmath.h" // RA2: as above
#include "globals.h" // RA2: as above
#include "goptions.h" // RA2: as above
#include "object.h" // RA2: as above
#include "tactical.h" // RA2: as above
#include "vector.h" // RA2: as above
#include "_rect.h" // RA2: as above
#include "_surface.h" // RA2: as above
#include "_tactica.h" // RA2: as above



template<> inline bool DynamicVectorClass<LineTrail *>::Add(LineTrail * const & object) // RA2: this unit's Add holds the new length in a variable before the Resize call (the sum is formed in the register that held Length)
{
	if (ActiveCount >= Length()) { // RA2: as above
		if ((IsAllocated || !VectorMax) && GrowthStep > 0) { // RA2: as above
			int new_length = Length(); // RA2: as above
			new_length += GrowthStep; // RA2: as above
			if (!Resize(new_length)) { // RA2: as above
				return(false); // RA2: as above
			}
		} else {
			return(false); // RA2: as above
		}
	}
	(*this)[ActiveCount++] = object; // RA2: as above
	return(true); // RA2: as above
}

DynamicVectorClass<LineTrail *> LineTrailArray; // RA2: 0x00A6E970, every trail that exists


LineTrail::LineTrail(void) : // RA2: at 0x005393E0
	Color(128, 128, 128), // RA2: as above
	Owner(NULL), // RA2: as above
	Decrement(16), // RA2: as above
	ActiveSlot(0) // RA2: as above
{
	for (int index = 0; index < 32; index++) { // RA2: as above
		Trails[index].Position = COORD_NONE; // RA2: as above
		Trails[index].Value = 0; // RA2: as above
	}
	LineTrailArray.Add(this); // RA2: as above
}


LineTrail::~LineTrail(void) // RA2: at 0x00539490
{
	if (Owner != NULL) { // RA2: as above
		Owner->LineTrailer = NULL; // RA2: as above
		Owner = NULL; // RA2: as above
	}
	LineTrailArray.Delete(this); // RA2: as above
}


void LineTrail::Release_Owner(void) // RA2: at 0x005394F0
{
	if (Owner != NULL) { // RA2: as above
		Owner->LineTrailer = NULL; // RA2: as above
		Owner = NULL; // RA2: as above
	}
}


void LineTrail::Set_Decrement(int value) // RA2: at 0x00539510
{
	if (!Options.DetailLevel) value <<= 1; // RA2: as above
	Decrement = value; // RA2: as above
}


void LineTrail::Update(void) // RA2: at 0x00539530
{
	if (Owner != NULL) { // RA2: as above
		Coord position = Owner->Position; // RA2: as above
		if (position != Trails[ActiveSlot].Position) { // RA2: as above
			int & active = ActiveSlot; // RA2: as above
			if (--active < 0) active = 31; // RA2: as above
			LineTrailNode * node = &Trails[active]; // RA2: as above
			node->Position = position; // RA2: as above
			node->Value = 255; // RA2: as above
		}
	}

	int slot = ActiveSlot; // RA2: as above
	do { // RA2: as above
		LineTrailNode * fade = &Trails[slot]; // RA2: as above
		fade->Value -= Decrement; // RA2: as above
		if (fade->Value < 0) fade->Value = 0; // RA2: as above
		slot++; // RA2: as above
		if (slot >= 32) slot = 0; // RA2: as above
	} while (slot != ActiveSlot); // RA2: as above
}


void LineTrail::Draw(void) // RA2: at 0x005395C0
{
	int slot = ActiveSlot + 1; // RA2: as above
	if (slot >= 32) slot = 0; // RA2: as above

	LineTrailNode * to = &Trails[ActiveSlot]; // RA2: as above
	if (to->Position == COORD_NONE) return; // RA2: as above

	for (;;) { // RA2: as above
		LineTrailNode * from = to; // RA2: as above
		to = &Trails[slot]; // RA2: as above
		if (from->Value == 0 || to->Position == COORD_NONE) break; // RA2: as above

		Point2D start; // RA2: as above
		Point2D end; // RA2: as above
		TacticalMap->Coord_To_Pixel(from->Position, start); // RA2: as above
		TacticalMap->Coord_To_Pixel(to->Position, end); // RA2: as above
		int start_height = -2 - Tactical::Z_Lepton_To_Pixel(from->Position.Z); // RA2: as above
		int end_height = -2 - Tactical::Z_Lepton_To_Pixel(to->Position.Z); // RA2: as above
		((DSurface *)CompositeSurface)->Draw_Trail_Line(TacticalRect, start, end, Color, from->Value, start_height, end_height); // RA2: as above

		slot++; // RA2: as above
		if (slot >= 32) slot = 0; // RA2: as above
		if (slot == ActiveSlot) break; // RA2: as above
	}
}


bool LineTrail::Is_Expired(void) const // RA2: at 0x005396E0
{
	return(Owner == NULL && Trails[ActiveSlot].Value == 0); // RA2: as above
}


void LineTrail::Process_All(void) // RA2: at 0x00539700
{
	for (int index = LineTrailArray.Count() - 1; index >= 0; index--) { // RA2: as above
		LineTrail * trail = LineTrailArray[index]; // RA2: as above
		trail->Update(); // RA2: as above
		if (trail->Is_Expired()) { // RA2: as above
			delete trail; // RA2: as above
		} else {
			trail->Draw(); // RA2: as above
		}
	}
}


void __cdecl Delete_All_Line_Trails(void) // RA2: at 0x005397B0, called by init.cpp
{
	for (int index = 0; index < LineTrailArray.Count(); index++) { // RA2: as above
		delete LineTrailArray[index]; // RA2: as above
	}
	LineTrailArray.Clear(); // RA2: as above
}
