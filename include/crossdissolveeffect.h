/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/

// RA2: the dissolve record of the dropship loadout screen (TibSun's dropship.cpp text); the struct tag is the
// RTTI's: VectorClass<CrossDissolveEffect *> (vtable 0x7A0B30) names .?AV?$VectorClass@PAUCrossDissolveEffect@@@@.
#pragma once

#include "rect.h"
#include "win.h"

class BSurface;
class Blitter;


/*
 * One cameo slot dissolving from one piece of art to another. The "from" art is
 * blitted over the slot as it is, then the "to" art is blitted on top of it through
 * the alpha buffer, whose level ramps up to 255 over the life of the effect.
 */
struct CrossDissolveEffect
{
		Rect FromRect;
		Rect ToRect;
		Rect DestRect;
		BSurface * FromSurface;
		BSurface * ToSurface;
		int Alpha;
		unsigned int StartTime;
		Blitter const * FromBlitter;
		Blitter const * ToBlitter;
		bool WasDrawn;

		CrossDissolveEffect(Rect const & from_rect, Rect const & to_rect, Rect const & dest_rect, BSurface * from_surface, BSurface * to_surface, Blitter const * from_blitter, Blitter const * to_blitter) :
			FromRect(from_rect),
			ToRect(to_rect),
			DestRect(dest_rect),
			FromSurface(from_surface),
			ToSurface(to_surface),
			Alpha(0),
			StartTime(::timeGetTime()),
			FromBlitter(from_blitter),
			ToBlitter(to_blitter),
			WasDrawn(false)
		{
		}
};
