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
#include <math.h>
#include <string.h>
#include "ebolt.h"
#include "_convert.h"
#include "_rect.h"
#include "_rules.h"
#include "_surface.h"
#include "_tactica.h"
#include "ccrand.h"
#include "partsys.h"
#include "rules.h"
#include "tactical.h"
#include "techno.h"
#include "unit.h"
#include "xsurface.h"
#include "convert.h"

DynamicVectorClass<EBolt *> EBoltArray; // RA2: 0x00853050; the live bolts (its initializer is the one at 0x004B1770)

/// <summary>
/// One saved half of a split bolt segment in Draw_Bolt: the three line end points still to be drawn and the values that go with them.
/// </summary>
struct BoltSegment // RA2: the executable keeps eight of these on the stack
{
		Coord Start[3];
		Coord End[3];
		int Length;
		int Deviation;
		int StartDepth;
		int EndDepth;
};

/// <summary>
/// Creates an idle bolt: no end points, no owner and the full lifetime (0x10000, halved each time it is drawn).
/// </summary>
EBolt::EBolt(void) // RA2: 0x004B17F0
{
	Point1 = Coord(0, 0, 0);
	Unknown_18 = 0;
	Lifetime = 0x10000;
	AlternateColor = false;
	Random = 0;
	Owner = NULL;
	WeaponSlot = 0;
	Point2 = Point1;
}

/// <summary>
/// Detaches the bolt from the unit that fires it, so that unit no longer points at the bolt.
/// </summary>
void EBolt::Clear_Owner(void) // RA2: 0x004B1830
{
	if (Owner != NULL) {
		((UnitClass *)Owner)->ElectricBolt = NULL;
		Owner = NULL;
	}
}

/// <summary>
/// Draws one depth shaded line of the bolt; the depth at each end is lowered by the height of that end.
/// </summary>
void EBolt::Draw_Line(Coord start, Coord end, int color, int start_depth, int end_depth) // RA2: 0x004B1850
{
	Point2D p1;
	Point2D p2;
	TacticalMap->Coord_To_Pixel(start, p1);
	TacticalMap->Coord_To_Pixel(end, p2);
	int d1 = start_depth - Tactical::Z_Lepton_To_Pixel(start.Z) - 2;
	int d2 = end_depth - Tactical::Z_Lepton_To_Pixel(end.Z) - 2;
	int pixel = NormalDrawer->Convert_Pixel(color);
	CompositeSurface->Draw_Depth_Shaded_Line(TacticalRect, p1, p2, pixel, d1, d2);
}

/*
**	RA2: the executable expands the subtraction and the length here but calls TPoint3D's constructor (0x0041B630) out
**	of line, which VC6 does only once the expansion is nested deeply enough. The inline chain the original source had is
**	not known; these six layers stand in for it (placeholders, to be replaced when the real chain is recovered).
*/
static inline int Bolt_Length0(Coord const & a, Coord const & b) // RA2: placeholder layer, see above
{
	return(a.Distance_YZX(b));
}

static inline int Bolt_Length1(Coord const & a, Coord const & b) // RA2: placeholder layer, see above
{
	return(Bolt_Length0(a, b));
}

static inline int Bolt_Length2(Coord const & a, Coord const & b) // RA2: placeholder layer, see above
{
	return(Bolt_Length1(a, b));
}

static inline int Bolt_Length3(Coord const & a, Coord const & b) // RA2: placeholder layer, see above
{
	return(Bolt_Length2(a, b));
}

static inline int Bolt_Length4(Coord const & a, Coord const & b) // RA2: placeholder layer, see above
{
	return(Bolt_Length3(a, b));
}

static inline int Bolt_Length5(Coord const & a, Coord const & b) // RA2: placeholder layer, see above
{
	return(Bolt_Length4(a, b));
}

/// <summary>
/// Draws the bolt as three jagged lines (two in the bolt color, a white core in color 15) by halving the segment up to eight times and moving the midpoints at random.
/// </summary>
void EBolt::Draw_Bolt(Coord start, Coord end, int color) // RA2: 0x004B1900
{
	BoltSegment stack[8];
	Coord mids[3];
	Coord starts[3];
	Coord ends[3];
	int offsets[6];
	bool first = true;
	int depth = 0;

	int length = Bolt_Length5(start, end); // RA2: through the placeholder layers above; the executable calls TPoint3D's constructor here
	if (length != 0) {
		int threshold = (length * 102) >> 8;
		int deviation = (length * 23) >> 8;
		int start_z = Unknown_18;
		BoltSegment * seg = stack;
		int end_z = 0;
		for (int i = 0; i < 3; i++) {
			starts[i] = start;
			ends[i] = end;
		}

		for (;;) {
			if (length > 64 && depth < 8) {
				for (int j = 0; j < 3; j++) {
					mids[j].X = (starts[j].X + ends[j].X) >> 1;
					mids[j].Y = (starts[j].Y + ends[j].Y) >> 1;
					mids[j].Z = (starts[j].Z + ends[j].Z) >> 1;
				}
				if (first) {
					first = false;
					double dev = deviation;
					for (int k = 0; k < 6; k++) {
						offsets[k] = fastmath::sin(Random * 3.141592653589793 / (k + 7)) * dev; // RA2: fastmath::sin (0x004BA480) instead of the intrinsic
					}
					int zoff = (offsets[2] + offsets[4] + deviation * 2) >> 1;
					for (int m = 0; m < 3; m++) {
						mids[m].X += offsets[0] + offsets[3];
						mids[m].Y += offsets[1] + offsets[5];
						mids[m].Z += zoff;
					}
				}
				if (length <= 128) {
					mids[0].X += NonCriticalRandomNumber(-1, 1) * deviation * 2;
					mids[0].Y += NonCriticalRandomNumber(-1, 1) * deviation * 2;
					mids[0].Z += NonCriticalRandomNumber(-1, 1) * deviation * 2;
				} else {
					mids[0].X += NonCriticalRandomNumber(-deviation, deviation);
					mids[0].Y += NonCriticalRandomNumber(-deviation, deviation);
					mids[0].Z += NonCriticalRandomNumber(-deviation, deviation);
				}
				if (length > threshold) {
					for (int n = 1; n < 3; n++) {
						mids[n].X = mids[0].X + (NonCriticalRandomNumber(-deviation, deviation) >> 1);
						mids[n].Y = mids[0].Y + (NonCriticalRandomNumber(-deviation, deviation) >> 1);
						mids[n].Z = mids[0].Z + (NonCriticalRandomNumber(-deviation, deviation) >> 1);
					}
				} else {
					for (int n2 = 1; n2 < 3; n2++) {
						mids[n2].X += NonCriticalRandomNumber(-deviation, deviation);
						mids[n2].Y += NonCriticalRandomNumber(-deviation, deviation);
						mids[n2].Z += NonCriticalRandomNumber(-deviation, deviation);
					}
				}
				length >>= 1;
				deviation >>= 1;
				int mid_z = (end_z + start_z) >> 1;
				for (int p = 0; p < 3; p++) {
					seg->Start[p] = mids[p];
					seg->End[p] = ends[p];
				}
				seg->Length = length;
				seg->Deviation = deviation;
				seg->StartDepth = mid_z;
				seg->EndDepth = end_z;
				seg++;
				depth++;
				memcpy(ends, mids, sizeof(ends));
				end_z = mid_z;
			} else {
				Draw_Line(starts[1], ends[1], color, start_z, end_z);
				Draw_Line(starts[2], ends[2], color, start_z, end_z);
				Draw_Line(starts[0], ends[0], 15, start_z, end_z);
				if (--depth < 0) break;
				seg--;
				memcpy(ends, seg->End, sizeof(ends));
				memcpy(starts, seg->Start, sizeof(starts));
				length = seg->Length;
				deviation = seg->Deviation;
				start_z = seg->StartDepth;
				end_z = seg->EndDepth;
			}
		}
	}
}

/// <summary>
/// Draws the bolt when it crosses the visible part of the tactical view, then halves its lifetime.
/// </summary>
void EBolt::Draw(void) // RA2: 0x004B2170
{
	if (Lifetime != 0) {
		Point2D p1;
		Point2D p2;
		TacticalMap->Coord_To_Pixel(Point1, p1);
		TacticalMap->Coord_To_Pixel(Point2, p2);
		if (Clip_Line_To_Rect(p1, p2, TacticalRect)) {
			Draw_Bolt(Point1, Point2, AlternateColor ? 5 : 10);
		}
		Random++;
	}
	Lifetime >>= 1;
}

/// <summary>
/// Moves each bolt to its owner's firing point, draws it, and deletes the bolts whose lifetime has run out.
/// </summary>
void __fastcall EBolt::Update_All(void) // RA2: 0x004B2210
{
	for (int i = EBoltArray.Count() - 1; i >= 0; i--) {
		EBolt * bolt = EBoltArray[i];
		if (bolt->Owner != NULL) {
			Coord source = bolt->Owner->Fire_Coord(bolt->WeaponSlot);
			if (source != COORD_NONE) {
				bolt->Point1 = source;
			}
		}
		bolt->Draw();
		if (bolt->Lifetime == 0) {
			EBoltArray.Delete(bolt);
			delete bolt;
		}
	}
}

/// <summary>
/// Deletes every bolt and empties the list.
/// </summary>
void __cdecl EBolt::Delete_All(void) // RA2: 0x004B23B0
{
	for (int i = 0; i < EBoltArray.Count(); i++) {
		delete EBoltArray[i];
	}
	EBoltArray.Clear();
}

/// <summary>
/// Starts the bolt between two coordinates, lists it and makes the sparks at its end.
/// </summary>
void EBolt::Fire(Coord source, Coord target, int zadjust) // RA2: 0x004B2430
{
	Point1 = source;
	Point2 = target;
	Unknown_18 = zadjust;
	Random = NonCriticalRandomNumber(0, 0x100);
	EBoltArray.Add(this);
	new ParticleSystemClass(Rule->DefaultSparkSystem, target);
}

/// <summary>
/// Returns the firing point of the owning unit, or COORD_NONE when the bolt has no owner.
/// </summary>
Coord EBolt::Owner_Coord(void) const // RA2: 0x004B2510
{
	if (Owner != NULL) {
		return(Owner->Fire_Coord(WeaponSlot));
	}
	return(COORD_NONE);
}

/// <summary>
/// Ties the bolt to an active unit that is not in limbo, so the start of the bolt follows the unit.
/// </summary>
void EBolt::Set_Owner(TechnoClass * owner, int which) // RA2: 0x004B2580
{
	if (owner != NULL && owner->Fetch_RTTI() == RTTI_UNIT && owner->IsActive && !owner->IsInLimbo) {
		Owner = owner;
		WeaponSlot = which;
	}
}

/// <summary>
/// Destroys the bolt and detaches it from its owner.
/// </summary>
EBolt::~EBolt(void) // RA2: 0x004B25C0
{
	Clear_Owner();
}
