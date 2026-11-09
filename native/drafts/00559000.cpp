// NON-EXACT DRAFT 0x559000 MapClass::Unknown_00559000 (TibSun Damage_High_Bridge)
// entry 0x559000; exact-match score 91.17 (Scout1157 keep/high_rt_pa_9117.cpp)
// origin lane: Scout1157 (Scout953 pa form ported to High, with TibSun High's return(true) after each
// middle-span collapse: the original returns 1 there, 0x5595AE and 0x559ABE; the 56.15 draft fell through).
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1133 (native/drafts.json, M16); replaced by Scout1153 (M19, owner note). Not original bytes.
// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2025 Electronic Arts Inc.
// Copyright 2026 OpenTS contributors
// Contains material derived from Electronic Arts source code.
// Modified by OpenTS contributors, 2026.
// EA's GPLv3 Section 7 additional terms and supplemental warranty disclaimers apply; see LICENSE.md.
// Adapted from TibSun code/map.cpp, MapClass::Damage_High_Bridge, base RA2 00559000.
// Scout1157: Scout953 pa form (per-case full switch tails) ported to High plus TibSun High's return(true) after
// each middle-span collapse (the original returns 1 there: 0x5595AE, 0x559ABE). 91.17; only the m2 block's
// DynamicVectorClass dtor Clear 0x42CFB0 still inlines (needs 3..6 more free root sites after it, or ~150-190 IL less R).
// @ref ??0?$VectorClass@VCell@@@@QAE@IPBVCell@@@Z=0042d4a0
// @ref ?Clear@?$VectorClass@VCell@@@@UAEXXZ=0042cfb0
// @ref ??H?$TPoint2D@F@@QBE?BV0@ABV0@@Z=0042add0
// @ref ??_7?$DynamicVectorClass@VCell@@@@6B@=0079c5a0
// @ref ??_7?$VectorClass@VCell@@@@6B@=0079c5e0
#define RA2_MAP_BRIDGE_PIECES   // NATIVE: the declarations its plan context had (map.hpp)
#include "shared/dir_type.hpp"
#include "shared/map.hpp"
#include "shared/map_view.hpp"
#include "shared/map_cell.hpp"
#include "shared/map_adjacent.hpp"
#include "shared/tileset_index.hpp"

bool MapClass::Unknown_00559000(Cell const & cell)
{
	CellClass * bridge_deck_cell = &Map[cell];
	int ittype = int(bridge_deck_cell->IsoTileIndex - unknown_tileset_a52c74 + 1);

	if (!bridge_deck_cell->IsUnderBridge) {

		bool valid_span = ittype == unknown_tileset_a6cb7c || ittype == unknown_tileset_a6cb7c + 1 || ittype == unknown_tileset_a6cb7c + 2 || ittype == unknown_tileset_a6cb7c + 3 || ittype == unknown_tileset_a52e70 || ittype == unknown_tileset_a52e70 + 1 || ittype == unknown_tileset_a52e70 + 2 || ittype == unknown_tileset_a52e70 + 3;
if (!valid_span) {
			return(false);
		}
	}

	if (!bridge_deck_cell->IsUnderBridge) {

		int subtile = (unsigned char)bridge_deck_cell->unknown_11a;
		if (ittype == unknown_tileset_a6cb7c ||
				ittype == unknown_tileset_a6cb7c + 3 ||
				ittype == unknown_tileset_a6cb7c + 1 ||
				ittype == unknown_tileset_a6cb7c + 2) {

		if ((subtile & 1) != 0) {
			return(false);
		}

		Cell middle_cell = bridge_deck_cell->MapCoords;
		while (subtile != 4) {
			if (subtile < 4) {
				middle_cell = Adjacent_Cell(middle_cell, FACING_S);
			} else {
				middle_cell = Adjacent_Cell(middle_cell, FACING_N);
			}
			subtile = (unsigned char)Map[middle_cell].unknown_11a;
		}

		CellClass * middle_cellptr = &Map[middle_cell];

		if (ittype == unknown_tileset_a6cb7c + 3) {
			Cell zone_cell = middle_cellptr->MapCoords;

			if ((middle_cellptr->unknown_11a & 1) != 0) {
				zone_cell.X--;
				Map[middle_cell + Cell(-1, -1)].unknown_473000();
				Map[middle_cell + Cell(-1, 0)].unknown_473000();
				Map[middle_cell + Cell(-1, 1)].unknown_473000();
				Cell cellid = middle_cellptr->Fetch_CellID();
				Set_Bridge_Middle_State(
					middle_cellptr->Fetch_CellID(),
					int(unknown_tileset_a52c74 + unknown_tileset_a6cb7c + 3),
					-1,
					Map[cellid + Cell(-1, 0)].unknown_11b - 4,
					false
				);
			} else {
				Map[middle_cell + Cell(0, -1)].unknown_473000();
				Map[Cell(middle_cell.X, middle_cell.Y)].unknown_473000();
				Map[middle_cell + Cell(0, 1)].unknown_473000();
				Set_Bridge_Middle_State(
					middle_cellptr->Fetch_CellID(),
					int(unknown_tileset_a52c74 + unknown_tileset_a6cb7c + 3),
					-1,
					middle_cellptr->unknown_11b - 4,
					false
				);
			}

			Internal_Destroy_High_Bridge_EW_BottomRight(middle_cellptr->Fetch_CellID(), FACING_E);
			Internal_Destroy_High_Bridge_EW_TopLeft(middle_cellptr->Fetch_CellID(), FACING_W);

			Destroy_High_Bridge_Connections_00558BD0(Adjacent_Cell(middle_cellptr->Fetch_CellID(), FACING_W));
			Destroy_High_Bridge_Connections_00558BD0(Adjacent_Cell(middle_cellptr->Fetch_CellID(), FACING_E));

			if (Unknown_005501A0(zone_cell)) {
				Unknown_0054EBD0();
			}

			DynamicVectorClass<Cell> cells;
			for (int x = 0; x < 2; x++) {
				for (int y = -2; y < 3; y++) {
					cells.Add(Cell(zone_cell.X + x, zone_cell.Y + y));
				}
			}
			Map.Unknown_005688A0(cells);
			return(true);
		}

		if (ittype == unknown_tileset_a6cb7c ||
			ittype == unknown_tileset_a6cb7c + 1 ||
			ittype == unknown_tileset_a6cb7c + 2) {

			Set_Bridge_Middle_State(
				middle_cellptr->Fetch_CellID(),
				int(unknown_tileset_a52c74 + unknown_tileset_a6cb7c + 2),
				-1,
				-1,
				false
			);
			Internal_Damage_High_Bridge_EW_BottomRight(middle_cellptr->Fetch_CellID(), FACING_E);
			Internal_Damage_High_Bridge_EW_TopLeft(middle_cellptr->Fetch_CellID(), FACING_W);
		}

		return(false);
		} else {


			if (subtile > 4) {
				return(false);
			}

			Cell middle_cell = bridge_deck_cell->MapCoords;
			while (subtile != 2) {
				if (subtile < 2) {
					middle_cell = Adjacent_Cell(middle_cell, FACING_E);
				} else {
					middle_cell = Adjacent_Cell(middle_cell, FACING_W);
				}
				subtile = (unsigned char)Map[middle_cell].unknown_11a;
			}

			CellClass * middle_cellptr = &Map[middle_cell];

			if (ittype == unknown_tileset_a52e70 + 3) {
				Cell zone_cell = middle_cellptr->MapCoords;

				if ((unsigned char)middle_cellptr->unknown_11a > 4) {
					zone_cell.Y--;
					Map[middle_cell + Cell(-1, -1)].unknown_473000();
					Map[middle_cell + Cell(0, -1)].unknown_473000();
					Map[middle_cell + Cell(1, -1)].unknown_473000();
					Cell cellid = middle_cellptr->Fetch_CellID();
					Set_Bridge_Middle_State(
						middle_cellptr->Fetch_CellID(),
						int(unknown_tileset_a52c74 + unknown_tileset_a52e70 + 3),
						-1,
						Map[cellid + Cell(0, -1)].unknown_11b - 4,
						false
					);
				} else {
					Map[middle_cell + Cell(-1, 0)].unknown_473000();
					Map[Cell(middle_cell.X, middle_cell.Y)].unknown_473000();
					Map[middle_cell + Cell(1, 0)].unknown_473000();
					Set_Bridge_Middle_State(
						middle_cellptr->Fetch_CellID(),
						int(unknown_tileset_a52c74 + unknown_tileset_a52e70 + 3),
						-1,
						middle_cellptr->unknown_11b - 4,
						false
					);
				}

				Internal_Destroy_High_Bridge_NS_BottomLeft(middle_cellptr->Fetch_CellID(), FACING_S);
				Internal_Destroy_High_Bridge_NS_TopRight(middle_cellptr->Fetch_CellID(), FACING_N);

				Destroy_High_Bridge_Connections_00558BD0(Adjacent_Cell(middle_cellptr->Fetch_CellID(), FACING_N));
				Destroy_High_Bridge_Connections_00558BD0(Adjacent_Cell(middle_cellptr->Fetch_CellID(), FACING_S));

				if (Unknown_005501A0(zone_cell)) {
					Unknown_0054EBD0();
				}

				DynamicVectorClass<Cell> cells;
				for (int y = 0; y < 2; y++) {
					for (int x = -2; x < 3; x++) {
						cells.Add(Cell(zone_cell.X + x, zone_cell.Y + y));
					}
				}
				Map.Unknown_005688A0(cells);
			return(true);
			}

			if (ittype == unknown_tileset_a52e70 ||
				ittype == unknown_tileset_a52e70 + 1 ||
				ittype == unknown_tileset_a52e70 + 2) {

				Set_Bridge_Middle_State(
					middle_cellptr->Fetch_CellID(),
					int(unknown_tileset_a52c74 + unknown_tileset_a52e70 + 2),
					-1,
					-1,
					false
				);
				Internal_Damage_High_Bridge_NS_BottomLeft(middle_cellptr->Fetch_CellID(), FACING_S);
				Internal_Damage_High_Bridge_NS_TopRight(middle_cellptr->Fetch_CellID(), FACING_N);
			}

			return(false);
		
		}
	} else {
		if (!bridge_deck_cell->IsBridgeDeck) {
			bridge_deck_cell = bridge_deck_cell->unknown_2c;
		}

		int overlay_data = bridge_deck_cell->OverlayData;
		FacingType dir = overlay_data < 9 ? FACING_E : FACING_S;

		switch (overlay_data) {
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
				bridge_deck_cell->OverlayData = 6;
				Internal_Damage_High_Bridge_EW_BottomRight(bridge_deck_cell->Fetch_CellID(), dir);
				Internal_Damage_High_Bridge_EW_TopLeft(bridge_deck_cell->Fetch_CellID(), Facing_Sub(dir, FACING_S));
				return(false);

			case 6:
				Internal_Destroy_High_Bridge_EW_BottomRight(bridge_deck_cell->Fetch_CellID(), dir);
				Internal_Destroy_High_Bridge_EW_TopLeft(bridge_deck_cell->Fetch_CellID(), Facing_Sub(dir, FACING_S));
				bridge_deck_cell->Set_Under_Bridge(FACING_N, false);
				bridge_deck_cell->OverlayData = 0;
				bridge_deck_cell->OverlayIndex = -1;
				Destroy_High_Bridge_Connections_00558BD0(cell);
				if (Unknown_005501A0(bridge_deck_cell->MapCoords)) {
					Unknown_0054EBD0();
				}
				return(true);

			case 7:
				Internal_Destroy_High_Bridge_EW_BottomRight(bridge_deck_cell->Fetch_CellID(), dir);
				bridge_deck_cell->Set_Under_Bridge(FACING_N, false);
				bridge_deck_cell->OverlayData = 0;
				bridge_deck_cell->OverlayIndex = -1;
				Destroy_High_Bridge_Connections_00558BD0(cell);
				if (Unknown_005501A0(bridge_deck_cell->MapCoords)) {
					Unknown_0054EBD0();
				}
				return(true);

			case 8:
				Internal_Destroy_High_Bridge_EW_TopLeft(bridge_deck_cell->Fetch_CellID(), Facing_Sub(dir, FACING_S));
				bridge_deck_cell->Set_Under_Bridge(FACING_N, false);
				bridge_deck_cell->OverlayData = 0;
				bridge_deck_cell->OverlayIndex = -1;
				Destroy_High_Bridge_Connections_00558BD0(cell);
				if (Unknown_005501A0(bridge_deck_cell->MapCoords)) {
					Unknown_0054EBD0();
				}
				return(true);

			case 9:
			case 10:
			case 11:
			case 12:
			case 13:
			case 14:
				bridge_deck_cell->OverlayData = 15;
				Internal_Damage_High_Bridge_NS_BottomLeft(bridge_deck_cell->Fetch_CellID(), dir);
				Internal_Damage_High_Bridge_NS_TopRight(bridge_deck_cell->Fetch_CellID(), Facing_Sub(dir, FACING_S));
				return(false);

			case 15:
				Internal_Destroy_High_Bridge_NS_BottomLeft(bridge_deck_cell->Fetch_CellID(), dir);
				Internal_Destroy_High_Bridge_NS_TopRight(bridge_deck_cell->Fetch_CellID(), Facing_Sub(dir, FACING_S));
				bridge_deck_cell->Set_Under_Bridge(FACING_W, false);
				bridge_deck_cell->OverlayData = 0;
				bridge_deck_cell->OverlayIndex = -1;
				Destroy_High_Bridge_Connections_00558BD0(cell);
				if (Unknown_005501A0(bridge_deck_cell->MapCoords)) {
					Unknown_0054EBD0();
				}
				return(true);

			case 16:
				Internal_Destroy_High_Bridge_NS_TopRight(bridge_deck_cell->Fetch_CellID(), Facing_Sub(dir, FACING_S));
				bridge_deck_cell->Set_Under_Bridge(FACING_W, false);
				bridge_deck_cell->OverlayData = 0;
				bridge_deck_cell->OverlayIndex = -1;
				Destroy_High_Bridge_Connections_00558BD0(cell);
				if (Unknown_005501A0(bridge_deck_cell->MapCoords)) {
					Unknown_0054EBD0();
				}
				return(true);

			case 17:
				Internal_Destroy_High_Bridge_NS_BottomLeft(bridge_deck_cell->Fetch_CellID(), dir);
				bridge_deck_cell->Set_Under_Bridge(FACING_W, false);
				bridge_deck_cell->OverlayData = 0;
				bridge_deck_cell->OverlayIndex = -1;
				Destroy_High_Bridge_Connections_00558BD0(cell);
				if (Unknown_005501A0(bridge_deck_cell->MapCoords)) {
					Unknown_0054EBD0();
				}
				return(true);

			default:
				return(false);
		}

	}
	return(false);
}
