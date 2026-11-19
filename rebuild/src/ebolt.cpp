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
**	RA2: the electric bolt (Tesla coil and Tesla tank) drawn between a weapon and its target for a few
**	frames. TibSun has no such file; the code is the executable's from 0x004B17F0.
*/

#include "always.h" // RA2: the includes this file needs
#include "geometry.h" // RA2: the header statics, in the order of the unit's initializer table
#include <math.h> // RA2: as above
#include <string.h> // RA2: as above
#include "ebolt.h" // RA2: as above
#include "_convert.h" // RA2: as above
#include "_rect.h" // RA2: as above
#include "_rules.h" // RA2: as above
#include "_surface.h" // RA2: as above
#include "_tactica.h" // RA2: as above
#include "ccrand.h" // RA2: as above
#include "partsys.h" // RA2: as above
#include "rules.h" // RA2: as above
#include "tactical.h" // RA2: as above
#include "techno.h" // RA2: as above
#include "unit.h" // RA2: as above
#include "xsurface.h" // RA2: as above
#include "convert.h" // RA2: as above

DynamicVectorClass<EBolt *> EBolt::Bolts; // RA2: 0x00853050; the live bolts (its initializer is the one at 0x004B1770)

/// <summary>
/// One saved half of a split bolt segment in Draw_Bolt: the three line end points still to be drawn and the values that go with them.
/// </summary>
struct BoltSegment // RA2: the executable keeps eight of these on the stack
{
	Coord Start[3]; // RA2: as above
	Coord End[3]; // RA2: as above
	int Length; // RA2: as above
	int Deviation; // RA2: as above
	int StartDepth; // RA2: as above
	int EndDepth; // RA2: as above
};

/// <summary>
/// Creates an idle bolt: no end points, no owner and the full lifetime (0x10000, halved each time it is drawn).
/// </summary>
EBolt::EBolt(void) // RA2: 0x004B17F0
{
	Point1 = Coord(0, 0, 0); // RA2: as above
	Unknown_18 = 0; // RA2: as above
	Lifetime = 0x10000; // RA2: as above
	AlternateColor = false; // RA2: as above
	Random = 0; // RA2: as above
	Owner = NULL; // RA2: as above
	WeaponSlot = 0; // RA2: as above
	Point2 = Point1; // RA2: as above
}

/// <summary>
/// Detaches the bolt from the unit that fires it, so that unit no longer points at the bolt.
/// </summary>
void EBolt::Clear_Owner(void) // RA2: 0x004B1830
{
	if (Owner != NULL) { // RA2: as above
		((UnitClass *)Owner)->ElectricBolt = NULL; // RA2: as above
		Owner = NULL; // RA2: as above
	} // RA2: as above
}

/// <summary>
/// Draws one depth shaded line of the bolt; the depth at each end is lowered by the height of that end.
/// </summary>
void EBolt::Draw_Line(Coord start, Coord end, int color, int start_depth, int end_depth) // RA2: 0x004B1850
{
	Point2D p1; // RA2: as above
	Point2D p2; // RA2: as above
	TacticalMap->Coord_To_Pixel(start, p1); // RA2: as above
	TacticalMap->Coord_To_Pixel(end, p2); // RA2: as above
	int d1 = start_depth - Tactical::Z_Lepton_To_Pixel(start.Z) - 2; // RA2: as above
	int d2 = end_depth - Tactical::Z_Lepton_To_Pixel(end.Z) - 2; // RA2: as above
	int pixel = NormalDrawer->Convert_Pixel(color); // RA2: as above
	CompositeSurface->Draw_Depth_Shaded_Line(TacticalRect, p1, p2, pixel, d1, d2); // RA2: as above
}

/*
**	RA2: the executable expands the subtraction and the length here but calls TPoint3D's constructor (0x0041B630) out
**	of line, which VC6 does only once the expansion is nested deeply enough. The inline chain the original source had is
**	not known; these six layers stand in for it (placeholders, to be replaced when the real chain is recovered).
*/
static inline int Bolt_Length0(Coord const & a, Coord const & b) // RA2: placeholder layer, see above
{ // RA2: as above
	return(a.Distance_YZX(b)); // RA2: as above
} // RA2: as above

static inline int Bolt_Length1(Coord const & a, Coord const & b) // RA2: placeholder layer, see above
{ // RA2: as above
	return(Bolt_Length0(a, b)); // RA2: as above
} // RA2: as above

static inline int Bolt_Length2(Coord const & a, Coord const & b) // RA2: placeholder layer, see above
{ // RA2: as above
	return(Bolt_Length1(a, b)); // RA2: as above
} // RA2: as above

static inline int Bolt_Length3(Coord const & a, Coord const & b) // RA2: placeholder layer, see above
{ // RA2: as above
	return(Bolt_Length2(a, b)); // RA2: as above
} // RA2: as above

static inline int Bolt_Length4(Coord const & a, Coord const & b) // RA2: placeholder layer, see above
{ // RA2: as above
	return(Bolt_Length3(a, b)); // RA2: as above
} // RA2: as above

static inline int Bolt_Length5(Coord const & a, Coord const & b) // RA2: placeholder layer, see above
{ // RA2: as above
	return(Bolt_Length4(a, b)); // RA2: as above
} // RA2: as above

/// <summary>
/// Draws the bolt as three jagged lines (two in the bolt color, a white core in color 15) by halving the segment up to eight times and moving the midpoints at random.
/// </summary>
void EBolt::Draw_Bolt(Coord start, Coord end, int color) // RA2: 0x004B1900
{
	BoltSegment stack[8]; // RA2: as above
	Coord mids[3]; // RA2: as above
	Coord starts[3]; // RA2: as above
	Coord ends[3]; // RA2: as above
	int offsets[6]; // RA2: as above
	bool first = true; // RA2: as above
	int depth = 0; // RA2: as above

	int length = Bolt_Length5(start, end); // RA2: through the placeholder layers above; the executable calls TPoint3D's constructor here
	if (length != 0) { // RA2: as above
		int threshold = (length * 102) >> 8; // RA2: as above
		int deviation = (length * 23) >> 8; // RA2: as above
		int start_z = Unknown_18; // RA2: as above
		BoltSegment * seg = stack; // RA2: as above
		int end_z = 0; // RA2: as above
		for (int i = 0; i < 3; i++) { // RA2: as above
			starts[i] = start; // RA2: as above
			ends[i] = end; // RA2: as above
		} // RA2: as above

		for (;;) { // RA2: as above
			if (length > 64 && depth < 8) { // RA2: as above
				for (int j = 0; j < 3; j++) { // RA2: as above
					mids[j].X = (starts[j].X + ends[j].X) >> 1; // RA2: as above
					mids[j].Y = (starts[j].Y + ends[j].Y) >> 1; // RA2: as above
					mids[j].Z = (starts[j].Z + ends[j].Z) >> 1; // RA2: as above
				} // RA2: as above
				if (first) { // RA2: as above
					first = false; // RA2: as above
					double dev = deviation; // RA2: as above
					for (int k = 0; k < 6; k++) { // RA2: as above
						offsets[k] = fastmath::sin(Random * 3.141592653589793 / (k + 7)) * dev; // RA2: fastmath::sin (0x004BA480) instead of the intrinsic
					} // RA2: as above
					int zoff = (offsets[2] + offsets[4] + deviation * 2) >> 1; // RA2: as above
					for (int m = 0; m < 3; m++) { // RA2: as above
						mids[m].X += offsets[0] + offsets[3]; // RA2: as above
						mids[m].Y += offsets[1] + offsets[5]; // RA2: as above
						mids[m].Z += zoff; // RA2: as above
					} // RA2: as above
				} // RA2: as above
				if (length <= 128) { // RA2: as above
					mids[0].X += NonCriticalRandomNumber(-1, 1) * deviation * 2; // RA2: as above
					mids[0].Y += NonCriticalRandomNumber(-1, 1) * deviation * 2; // RA2: as above
					mids[0].Z += NonCriticalRandomNumber(-1, 1) * deviation * 2; // RA2: as above
				} else { // RA2: as above
					mids[0].X += NonCriticalRandomNumber(-deviation, deviation); // RA2: as above
					mids[0].Y += NonCriticalRandomNumber(-deviation, deviation); // RA2: as above
					mids[0].Z += NonCriticalRandomNumber(-deviation, deviation); // RA2: as above
				} // RA2: as above
				if (length > threshold) { // RA2: as above
					for (int n = 1; n < 3; n++) { // RA2: as above
						mids[n].X = mids[0].X + (NonCriticalRandomNumber(-deviation, deviation) >> 1); // RA2: as above
						mids[n].Y = mids[0].Y + (NonCriticalRandomNumber(-deviation, deviation) >> 1); // RA2: as above
						mids[n].Z = mids[0].Z + (NonCriticalRandomNumber(-deviation, deviation) >> 1); // RA2: as above
					} // RA2: as above
				} else { // RA2: as above
					for (int n2 = 1; n2 < 3; n2++) { // RA2: as above
						mids[n2].X += NonCriticalRandomNumber(-deviation, deviation); // RA2: as above
						mids[n2].Y += NonCriticalRandomNumber(-deviation, deviation); // RA2: as above
						mids[n2].Z += NonCriticalRandomNumber(-deviation, deviation); // RA2: as above
					} // RA2: as above
				} // RA2: as above
				length >>= 1; // RA2: as above
				deviation >>= 1; // RA2: as above
				int mid_z = (end_z + start_z) >> 1; // RA2: as above
				for (int p = 0; p < 3; p++) { // RA2: as above
					seg->Start[p] = mids[p]; // RA2: as above
					seg->End[p] = ends[p]; // RA2: as above
				} // RA2: as above
				seg->Length = length; // RA2: as above
				seg->Deviation = deviation; // RA2: as above
				seg->StartDepth = mid_z; // RA2: as above
				seg->EndDepth = end_z; // RA2: as above
				seg++; // RA2: as above
				depth++; // RA2: as above
				memcpy(ends, mids, sizeof(ends)); // RA2: as above
				end_z = mid_z; // RA2: as above
			} else { // RA2: as above
				Draw_Line(starts[1], ends[1], color, start_z, end_z); // RA2: as above
				Draw_Line(starts[2], ends[2], color, start_z, end_z); // RA2: as above
				Draw_Line(starts[0], ends[0], 15, start_z, end_z); // RA2: as above
				if (--depth < 0) break; // RA2: as above
				seg--; // RA2: as above
				memcpy(ends, seg->End, sizeof(ends)); // RA2: as above
				memcpy(starts, seg->Start, sizeof(starts)); // RA2: as above
				length = seg->Length; // RA2: as above
				deviation = seg->Deviation; // RA2: as above
				start_z = seg->StartDepth; // RA2: as above
				end_z = seg->EndDepth; // RA2: as above
			} // RA2: as above
		} // RA2: as above
	} // RA2: as above
}

/// <summary>
/// Draws the bolt when it crosses the visible part of the tactical view, then halves its lifetime.
/// </summary>
void EBolt::Draw(void) // RA2: 0x004B2170
{
	if (Lifetime != 0) { // RA2: as above
		Point2D p1; // RA2: as above
		Point2D p2; // RA2: as above
		TacticalMap->Coord_To_Pixel(Point1, p1); // RA2: as above
		TacticalMap->Coord_To_Pixel(Point2, p2); // RA2: as above
		if (Clip_Line_To_Rect(p1, p2, TacticalRect)) { // RA2: as above
			Draw_Bolt(Point1, Point2, AlternateColor ? 5 : 10); // RA2: as above
		} // RA2: as above
		Random++; // RA2: as above
	} // RA2: as above
	Lifetime >>= 1; // RA2: as above
}

/// <summary>
/// Moves each bolt to its owner's firing point, draws it, and deletes the bolts whose lifetime has run out.
/// </summary>
void __fastcall EBolt::Update_All(void) // RA2: 0x004B2210
{
	for (int i = Bolts.Count() - 1; i >= 0; i--) { // RA2: as above
		EBolt * bolt = Bolts[i]; // RA2: as above
		if (bolt->Owner != NULL) { // RA2: as above
			Coord source = bolt->Owner->Fire_Coord(bolt->WeaponSlot); // RA2: as above
			if (source != COORD_NONE) { // RA2: as above
				bolt->Point1 = source; // RA2: as above
			} // RA2: as above
		} // RA2: as above
		bolt->Draw(); // RA2: as above
		if (bolt->Lifetime == 0) { // RA2: as above
			Bolts.Delete(bolt); // RA2: as above
			delete bolt; // RA2: as above
		} // RA2: as above
	} // RA2: as above
}

/// <summary>
/// Deletes every bolt and empties the list.
/// </summary>
void __cdecl EBolt::Delete_All(void) // RA2: 0x004B23B0
{
	for (int i = 0; i < Bolts.Count(); i++) { // RA2: as above
		delete Bolts[i]; // RA2: as above
	} // RA2: as above
	Bolts.Clear(); // RA2: as above
}

/// <summary>
/// Starts the bolt between two coordinates, lists it and makes the sparks at its end.
/// </summary>
void EBolt::Fire(Coord source, Coord target, int zadjust) // RA2: 0x004B2430
{
	Point1 = source; // RA2: as above
	Point2 = target; // RA2: as above
	Unknown_18 = zadjust; // RA2: as above
	Random = NonCriticalRandomNumber(0, 0x100); // RA2: as above
	Bolts.Add(this); // RA2: as above
	new ParticleSystemClass(Rule->DefaultSparkSystem, target); // RA2: as above
}

/// <summary>
/// Returns the firing point of the owning unit, or COORD_NONE when the bolt has no owner.
/// </summary>
Coord EBolt::Owner_Coord(void) const // RA2: 0x004B2510
{
	if (Owner != NULL) { // RA2: as above
		return(Owner->Fire_Coord(WeaponSlot)); // RA2: as above
	} // RA2: as above
	return(COORD_NONE); // RA2: as above
}

/// <summary>
/// Ties the bolt to an active unit that is not in limbo, so the start of the bolt follows the unit.
/// </summary>
void EBolt::Set_Owner(TechnoClass * owner, int which) // RA2: 0x004B2580
{
	if (owner != NULL && owner->Fetch_RTTI() == RTTI_UNIT && owner->IsActive && !owner->IsInLimbo) { // RA2: as above
		Owner = owner; // RA2: as above
		WeaponSlot = which; // RA2: as above
	} // RA2: as above
}

/// <summary>
/// Destroys the bolt and detaches it from its owner.
/// </summary>
EBolt::~EBolt(void) // RA2: 0x004B25C0
{
	Clear_Owner(); // RA2: as above
}
