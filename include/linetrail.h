/*******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 RedAlert2 contributors
 ******************************************************************************/

#pragma once

#include "coord.h"
#include "rgb.h"

class ObjectClass;

/*
**	RA2: one point of a line trail: a position and the intensity it has left (16 bytes).
*/
struct LineTrailNode
{
	Coord Position; // RA2: +0x00
	int Value; // RA2: +0x0C
};

/*
**	RA2: the fading line behind a moving object (link2.cpp in YR). ObjectClass::Unlimbo makes one for a type
**	with UseLineTrail and keeps it in LineTrailer (+0x94). The object is 528 bytes: a colour, the owner,
**	the fade step, the newest slot and a ring of 32 points.
*/
class LineTrail
{
	public:
		LineTrail(void); // RA2: 0x005393E0, grey, decrement 16, and enters the list of all trails
		~LineTrail(void); // RA2: 0x00539490, lets go of the owner and leaves the list
		void Release_Owner(void); // RA2: 0x005394F0, called by Detach_All; YR's name Release_Owner (meastes: Clear_Owner)
		void Set_Decrement(int value); // RA2: 0x00539510, the fade step, doubled unless the detail level is set
		void Update(void); // RA2: 0x00539530, adds the owner's position when it moved and fades every point
		void Draw(void); // RA2: 0x005395C0, draws a line between each pair of live points
		bool Is_Expired(void) const; // RA2: 0x005396E0, no owner and the newest point has faded
		static void Process_All(void); // RA2: 0x00539700, updates every trail, deleting the expired ones and drawing the others

		RGBClass Color; // RA2: +0x00 (128, 128, 128)
		ObjectClass * Owner; // RA2: +0x04, the object that draws the trail
		int Decrement; // RA2: +0x08 (16)
		int ActiveSlot; // RA2: +0x0C, the newest point
		LineTrailNode Trails[32]; // RA2: +0x10, the ring of points
};
