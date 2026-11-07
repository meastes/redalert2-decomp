/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/
// From TibSun map.cpp Destroy_High_Bridge_Span: the high bridge twin of Destroy_Train_Bridge_Span 0x00553000,
// adapted against independently bounded RA2 bytes.
#include "shared/map.hpp"
#include "shared/map_view.hpp"
#include "shared/map_cell.hpp"
#include "shared/map_adjacent.hpp"
#include "shared/tactical.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/crt_memory.hpp"
#include "shared/always.hpp"
bool MapClass::Destroy_High_Bridge_Span_00558660(Cell const & cell, FacingType dir, Rect * dirty)
{
	CellClass * cellptr = &Map[cell];

	int count = 1;

	CellClass * startptr;
	bool was_owner;
	Cell ownercell;
	bool sprung;
	int index;

	/*
	 * Remember the first adjacent cell of the span. It is used as one endpoint
	 * when springing the bridge destruction triggers.
	 */
	Cell spanstart = Adjacent_Cell(cellptr->Fetch_CellID(), dir);
	Cell lastcell = spanstart;

	int ittype;

	/*
	 * Walk along the span looking for the end tile, giving up after 30 cells.
	 */
	while (true) {
		lastcell = Adjacent_Cell(cellptr->Fetch_CellID(), dir);
		cellptr = &Map[lastcell];

		ittype = int(cellptr->IsoTileIndex - unknown_tileset_a52c74 + 1);

		if (dir == FACING_E) {

			if (ittype == unknown_tileset_a6e028 && cellptr->unknown_11a == 4
				|| ittype == unknown_tileset_a52c80 && cellptr->unknown_11a == 4
				|| (ittype == unknown_tileset_a6cb7c
					|| ittype == unknown_tileset_a6cb7c + 3
					|| ittype == unknown_tileset_a6cb7c + 1
					|| ittype == unknown_tileset_a6cb7c + 2)
					&& cellptr->unknown_11a == 4) {
				break;
			}

		} else if (dir == FACING_S
			&& (ittype == unknown_tileset_a6e010 && cellptr->unknown_11a == 2
				|| ittype == unknown_tileset_a53390 && cellptr->unknown_11a == 2
				|| (ittype == unknown_tileset_a52e70
					|| ittype == unknown_tileset_a52e70 + 3
					|| ittype == unknown_tileset_a52e70 + 1
					|| ittype == unknown_tileset_a52e70 + 2)
					&& cellptr->unknown_11a == 2)) {
			break;
		}

		if (++count >= 30) {
			break;
		}
	}

	/*
	 * If no span end was found within range, there is nothing to destroy.
	 */
	if (count != 30) {

		/*
		 * Re-fetch the span start cell. Its height anchors the dirty rectangle.
		 */
		startptr = &Map[cell];

		if (dirty != 0) {

			/*
			 * Compute the screen rectangle spanning from the original cell to the
			 * located span end and union it into the supplied dirty rectangle.
			 */
			Point2D startpix;
			CoordStruct coord;
			coord.X = cell.X * 256 + 128;
            coord.Y = cell.Y * 256 + 128;
            coord.Z = unknown_geometry_g * startptr->unknown_11b;
			unknown_tactical_83996c->Coord_To_Pixel(coord, startpix);

			Point2D endpix;
			coord.X = lastcell.X * 256 + 128;
            coord.Y = lastcell.Y * 256 + 128;
            coord.Z = unknown_geometry_g * startptr->unknown_11b;
			unknown_tactical_83996c->Coord_To_Pixel(coord, endpix);

            Rect bounds;
            bounds.X = min(startpix.X, endpix.X) - 64;
            bounds.Y = min(startpix.Y, endpix.Y) - 64;
            bounds.Width = abs(startpix.X - endpix.X) + 128;
            bounds.Height = abs(startpix.Y - endpix.Y) + 128;
            *dirty = Union(*dirty, bounds);
		}

		/*
		 * Walk the span again, springing the bridge destruction triggers and locating
		 * the transition from a bridge owner cell to a non-owner cell.
		 */
		was_owner = false;
		ownercell = Cell(-1, -1);
		sprung = false;

		for (index = 0; index < count; index++) {

			if (startptr->IsBridgeDeck && was_owner) {
				ownercell = startptr->MapCoords;
			}

			if (!startptr->IsBridgeDeck && !was_owner && ownercell != Cell(-1, -1)) {

				/*
				 * Found the span end owner. Clear the rail bridge state on the cell
				 * behind it, remove its overlay, refresh the radar background, then
				 * recurse to continue.
				 */
				Cell behind = Adjacent_Cell_L(startptr->Fetch_CellID(), Facing_Sub(dir, 4));
				CellClass * behindptr = &Map[behind];

				if (dir == FACING_E) behindptr->Set_Under_Bridge(FACING_N, false);
                else behindptr->Set_Under_Bridge(FACING_W, false);
				behindptr->OverlayData = 0;
				behindptr->OverlayIndex = -1;
				Map.Unknown_00631570(behindptr->MapCoords);

				Destroy_High_Bridge_Span_00558660(cell, dir, dirty);
				return(true);
			}

			was_owner = !startptr->IsBridgeDeck;

			if (was_owner && !sprung) {
				sprung = true;
                Spring_Bridge_Destruction_Triggers(spanstart, lastcell);
			}

			TPoint2D<short> const &origin = startptr->Fetch_CellID();
            Cell next = origin + AdjacentCell[dir & 7];
			startptr = &Map[next];
		}
	}

	return(false);
}
