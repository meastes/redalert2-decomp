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
// From TibSun map.cpp Zone_Span; adapted to native RA2 layout and passability6.
#include "shared/map.hpp"
#include "shared/crt_memory.hpp"
extern int unknown_zone_a6fc34;
// @ref ?unknown_zone_a6fc34@@3HA=00a6fc34
// @ref ?Unknown_0054F250@MapClass@@QAEHPAUUnknownMapElem4@@HAAH@Z=0054f250
int MapClass::Unknown_0054F250(UnknownMapElem4 * data, int zone, int & skip)
{
	UnknownMapElem4 * begin = data;
	UnknownMapElem4 * end = data;
	int passability = data->unknown_00;
	bool nopass = passability == 6;
	int cell_height = data->unknown_01;

	/*
	**	Find the full extent of the current span by first scanning leftward
	**	until a boundary is reached.
	*/
	while (begin->unknown_00 == passability) {
		if (abs(begin->unknown_01 - cell_height) >= 2) {
			break;
		}
		begin->unknown_02 = zone;
		cell_height = begin->unknown_01;
		begin--;
	}

	int begin_zone = begin->unknown_02;
	if (begin_zone != 0 && (abs(begin->unknown_01 - cell_height) < 2 || nopass) && begin_zone != unknown_zone_a6fc34 && begin_zone != (unsigned short)zone) {
		unknown_14->Add_Object(HashObject<unsigned int, unsigned int>(begin_zone, (unsigned short)zone));
		unknown_zone_a6fc34 = begin_zone;
	}

	/*
	 * Scan rightward until a boundary is reached. This will then define the
	 * extent of the current span. The end pointer advances, so its ZoneID is
	 * the active zone boundary; data remains fixed at the seed cell.
	 */
	while (end->unknown_00 == passability) {
		if (abs(end->unknown_01 - cell_height) >= 4) {
			break;
		}
		end->unknown_02 = zone;
		cell_height = end->unknown_01;
		end++;
	}

	begin_zone = end->unknown_02;
	if (begin_zone != 0 && (abs(end->unknown_01 - cell_height) < 2 || nopass) && begin_zone != unknown_zone_a6fc34 && begin_zone != (unsigned short)zone) {
		unknown_14->Add_Object(HashObject<unsigned int, unsigned int>(begin_zone, (unsigned short)zone));
		unknown_zone_a6fc34 = begin_zone;
	}

	int filled = end - begin - 1;
	skip = end - data - 1;
	begin++;
	int skip1 = 0;
	int skip2 = 0;
	end--;

	int stride = MapSize.Height + MapSize.Width + 1;
	int diag = stride + 1;

	UnknownMapElem4 * fbegin = begin - diag;
	UnknownMapElem4 * fbegin2 = begin + stride - 1;
	UnknownMapElem4 * fend = end - stride + 1;
	UnknownMapElem4 * fend2 = end + diag;

	/*
	**	At this point we know the bounds of the current span. Fill in the zone values
	**	for the entire span.
	*/
	while (fbegin <= fend) {
		int zzone = fbegin->unknown_02;
		UnknownMapElem4 * adjacent;
		if (fbegin < fend - 1) {
			adjacent = fbegin + diag;
		} else {
			adjacent = fbegin == fend - 1 ? fbegin + stride : fbegin - 1 + stride;
		}
		if (zzone == 0) {
			if (fbegin->unknown_00 == passability && abs(fbegin->unknown_01 - adjacent->unknown_01) < 2) {
				filled += Unknown_0054F250(fbegin, zone, skip1);
				fbegin += skip1;
			} else {
				fbegin++;
			}
		} else {
			if (zzone != (unsigned short)zone && zzone != unknown_zone_a6fc34 && (abs(fbegin->unknown_01 - adjacent->unknown_01) < 2 || nopass)) {
				unknown_14->Add_Object(HashObject<unsigned int, unsigned int>(zzone, (unsigned short)zone));
				unknown_zone_a6fc34 = zzone;
			}
			fbegin++;
		}
	}

	/*
	**	Now scan the upper and lower shadow rows. If any of these rows contain
	**	candidate cells, then recursively call the span process for them. Take
	**	note that the adjacent span scanning starts one cell wider on each
	**	end of the scan. This is necessary because diagonals are considered
	**	adjacent.
	*/
	while (fbegin2 <= fend2) {
		int id = fbegin2->unknown_02;
		UnknownMapElem4 * adjacent;
		if (fbegin2 < fend2 - 1) {
			adjacent = fbegin2 - stride + 1;
		} else {
			adjacent = fbegin2 == fend2 - 1 ? fbegin2 - stride : fbegin2 - diag;
		}
		if (id == 0) {
			if (fbegin2->unknown_00 == passability && abs(fbegin2->unknown_01 - adjacent->unknown_01) < 2) {
				filled += MapClass::Unknown_0054F250(fbegin2, zone, skip2);
				fbegin2 += skip2;
			} else {
				fbegin2++;
			}
		} else {
			if (id != (unsigned short)zone && id != unknown_zone_a6fc34 && (abs(fbegin2->unknown_01 - adjacent->unknown_01) < 2 || nopass)) {
				unknown_14->Add_Object(HashObject<unsigned int, unsigned int>(id, (unsigned short)zone));
				unknown_zone_a6fc34 = id;
			}
			fbegin2++;
		}
	}

	return(filled);
}
