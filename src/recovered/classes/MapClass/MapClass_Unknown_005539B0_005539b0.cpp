// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2025 Electronic Arts Inc.
// Copyright 2026 OpenTS contributors
// Contains material derived from Electronic Arts source code.
// Modified by OpenTS contributors, 2026.
// EA's GPLv3 Section 7 additional terms and supplemental warranty disclaimers apply; see LICENSE.md.
// Adapted from TibSun code/map.cpp, MapClass::Damage_Train_Bridge, base RA2 005539B0.
// Per-case full switch tails (Scout953 pa form); unlike Damage_High_Bridge the middle-span collapse falls through to
// the damage test (TibSun Train text). The collapse list is built as the accepted twins Repair_Train_Bridge 0x552570 /
// 0x555A60 build theirs, `cells.Add(Cell(zone_cell + Cell(x, y)))`, and the zone cell steps with `zone_cell += Cell(...)`
// (TPoint2D<short>::operator+=, as Find_Exit_Cell 0x44A400); with those inline sums both DynamicVectorClass dtors keep
// Clear 0x42CFB0 out of line. One `CellClass * middle_cellptr` declared with `subtile` serves both middle blocks: that
// single web outranks ittype for the dead `cell` argument slot, as in the original frame.
// The NS damage block calls the High bridge helpers 0x5550A0 / 0x5551B0, as the original does (TibSun's Train text too).
// @ref ??0?$VectorClass@VCell@@@@QAE@IPBVCell@@@Z=0042d4a0
// @ref ?Clear@?$VectorClass@VCell@@@@UAEXXZ=0042cfb0
// @ref ??H?$TPoint2D@F@@QBE?BV0@ABV0@@Z=0042add0
// @ref ??_7?$DynamicVectorClass@VCell@@@@6B@=0079c5a0
// @ref ??_7?$VectorClass@VCell@@@@6B@=0079c5e0
#include "shared/dir_type.hpp"
#include "shared/map.hpp"
#include "shared/map_view.hpp"
#include "shared/map_cell.hpp"
#include "shared/map_adjacent.hpp"
#include "shared/tileset_index.hpp"

bool MapClass::Unknown_005539B0(Cell const & cell)
{
	CellClass * bridge_deck_cell = &Map[cell];
	int ittype = int(bridge_deck_cell->IsoTileIndex - unknown_tileset_a6cb68 + 1);

	if (!bridge_deck_cell->IsUnderBridge) {

		bool valid_span = ittype == unknown_tileset_a6cb7c || ittype == unknown_tileset_a6cb7c + 1 || ittype == unknown_tileset_a6cb7c + 2 || ittype == unknown_tileset_a6cb7c + 3 || ittype == unknown_tileset_a52e70 || ittype == unknown_tileset_a52e70 + 1 || ittype == unknown_tileset_a52e70 + 2 || ittype == unknown_tileset_a52e70 + 3;
		if (!valid_span) {
			return(false);
		}
	}

	if (!bridge_deck_cell->IsUnderBridge) {

		int subtile = (unsigned char)bridge_deck_cell->unknown_11a;
		CellClass * middle_cellptr;
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

		middle_cellptr = &Map[middle_cell];

		if (ittype == unknown_tileset_a6cb7c + 3) {
			Cell zone_cell = middle_cellptr->MapCoords;

			if ((middle_cellptr->unknown_11a & 1) != 0) {
				zone_cell += Cell(-1, 0);
				Map[middle_cell + Cell(-1, -1)].unknown_473000();
				Map[middle_cell + Cell(-1, 0)].unknown_473000();
				Map[middle_cell + Cell(-1, 1)].unknown_473000();
				Cell cellid = middle_cellptr->Fetch_CellID();
				Set_Bridge_Middle_State(
					middle_cellptr->Fetch_CellID(),
					int(unknown_tileset_a6cb68 + unknown_tileset_a6cb7c + 3),
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
					int(unknown_tileset_a6cb68 + unknown_tileset_a6cb7c + 3),
					-1,
					middle_cellptr->unknown_11b - 4,
					false
				);
			}

			Internal_Destroy_Train_Bridge_EW_BottomRight(middle_cellptr->Fetch_CellID(), FACING_E);
			Internal_Destroy_Train_Bridge_EW_TopLeft(middle_cellptr->Fetch_CellID(), FACING_W);

			Destroy_Train_Bridge_Connections_00553570(Adjacent_Cell(middle_cellptr->Fetch_CellID(), FACING_W));
			Destroy_Train_Bridge_Connections_00553570(Adjacent_Cell(middle_cellptr->Fetch_CellID(), FACING_E));

			if (Unknown_005501A0(zone_cell)) {
				Zone_Reset();
			}

			DynamicVectorClass<Cell> cells;
			for (int x = 0; x < 2; x++) {
				for (int y = -2; y < 3; y++) {
					cells.Add(Cell(zone_cell + Cell(x, y)));
				}
			}
			Map.Unknown_005688A0(cells);
		}

		if (ittype == unknown_tileset_a6cb7c ||
			ittype == unknown_tileset_a6cb7c + 1 ||
			ittype == unknown_tileset_a6cb7c + 2) {

			Set_Bridge_Middle_State(
				middle_cellptr->Fetch_CellID(),
				int(unknown_tileset_a6cb68 + unknown_tileset_a6cb7c + 2),
				-1,
				-1,
				false
			);
			Internal_Damage_Train_Bridge_EW_BottomRight(middle_cellptr->Fetch_CellID(), FACING_E);
			Internal_Damage_Train_Bridge_EW_TopLeft(middle_cellptr->Fetch_CellID(), FACING_W);
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

			middle_cellptr = &Map[middle_cell];

			if (ittype == unknown_tileset_a52e70 + 3) {
				Cell zone_cell = middle_cellptr->MapCoords;

				if ((unsigned char)middle_cellptr->unknown_11a > 4) {
					zone_cell += Cell(0, -1);
					Map[middle_cell + Cell(-1, -1)].unknown_473000();
					Map[middle_cell + Cell(0, -1)].unknown_473000();
					Map[middle_cell + Cell(1, -1)].unknown_473000();
					Cell cellid = middle_cellptr->Fetch_CellID();
					Set_Bridge_Middle_State(
						middle_cellptr->Fetch_CellID(),
						int(unknown_tileset_a6cb68 + unknown_tileset_a52e70 + 3),
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
						int(unknown_tileset_a6cb68 + unknown_tileset_a52e70 + 3),
						-1,
						middle_cellptr->unknown_11b - 4,
						false
					);
				}

				Internal_Destroy_Train_Bridge_NS_BottomLeft(middle_cellptr->Fetch_CellID(), FACING_S);
				Internal_Destroy_Train_Bridge_NS_TopRight(middle_cellptr->Fetch_CellID(), FACING_N);

				Destroy_Train_Bridge_Connections_00553570(Adjacent_Cell(middle_cellptr->Fetch_CellID(), FACING_N));
				Destroy_Train_Bridge_Connections_00553570(Adjacent_Cell(middle_cellptr->Fetch_CellID(), FACING_S));

				if (Unknown_005501A0(zone_cell)) {
					Zone_Reset();
				}

				DynamicVectorClass<Cell> cells;
				for (int y = 0; y < 2; y++) {
					for (int x = -2; x < 3; x++) {
						cells.Add(Cell(zone_cell + Cell(x, y)));
					}
				}
				Map.Unknown_005688A0(cells);
			}

			if (ittype == unknown_tileset_a52e70 ||
				ittype == unknown_tileset_a52e70 + 1 ||
				ittype == unknown_tileset_a52e70 + 2) {

				Set_Bridge_Middle_State(
					middle_cellptr->Fetch_CellID(),
					int(unknown_tileset_a6cb68 + unknown_tileset_a52e70 + 2),
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
				Internal_Damage_Train_Bridge_EW_BottomRight(bridge_deck_cell->Fetch_CellID(), dir);
				Internal_Damage_Train_Bridge_EW_TopLeft(bridge_deck_cell->Fetch_CellID(), Facing_Sub(dir, FACING_S));
				return(false);

			case 6:
				Internal_Destroy_Train_Bridge_EW_BottomRight(bridge_deck_cell->Fetch_CellID(), dir);
				Internal_Destroy_Train_Bridge_EW_TopLeft(bridge_deck_cell->Fetch_CellID(), Facing_Sub(dir, FACING_S));
				bridge_deck_cell->Set_Under_Rail_Bridge(FACING_N, false);
				bridge_deck_cell->OverlayData = 0;
				bridge_deck_cell->OverlayIndex = -1;
				Destroy_Train_Bridge_Connections_00553570(cell);
				if (Unknown_005501A0(bridge_deck_cell->MapCoords)) {
					Zone_Reset();
				}
				return(true);

			case 7:
				Internal_Destroy_Train_Bridge_EW_BottomRight(bridge_deck_cell->Fetch_CellID(), dir);
				bridge_deck_cell->Set_Under_Rail_Bridge(FACING_N, false);
				bridge_deck_cell->OverlayData = 0;
				bridge_deck_cell->OverlayIndex = -1;
				Destroy_Train_Bridge_Connections_00553570(cell);
				if (Unknown_005501A0(bridge_deck_cell->MapCoords)) {
					Zone_Reset();
				}
				return(true);

			case 8:
				Internal_Destroy_Train_Bridge_EW_TopLeft(bridge_deck_cell->Fetch_CellID(), Facing_Sub(dir, FACING_S));
				bridge_deck_cell->Set_Under_Rail_Bridge(FACING_N, false);
				bridge_deck_cell->OverlayData = 0;
				bridge_deck_cell->OverlayIndex = -1;
				Destroy_Train_Bridge_Connections_00553570(cell);
				if (Unknown_005501A0(bridge_deck_cell->MapCoords)) {
					Zone_Reset();
				}
				return(true);

			case 9:
			case 10:
			case 11:
			case 12:
			case 13:
			case 14:
				bridge_deck_cell->OverlayData = 15;
				Internal_Damage_Train_Bridge_NS_BottomLeft(bridge_deck_cell->Fetch_CellID(), dir);
				Internal_Damage_Train_Bridge_NS_TopRight(bridge_deck_cell->Fetch_CellID(), Facing_Sub(dir, FACING_S));
				return(false);

			case 15:
				Internal_Destroy_Train_Bridge_NS_BottomLeft(bridge_deck_cell->Fetch_CellID(), dir);
				Internal_Destroy_Train_Bridge_NS_TopRight(bridge_deck_cell->Fetch_CellID(), Facing_Sub(dir, FACING_S));
				bridge_deck_cell->Set_Under_Rail_Bridge(FACING_W, false);
				bridge_deck_cell->OverlayData = 0;
				bridge_deck_cell->OverlayIndex = -1;
				Destroy_Train_Bridge_Connections_00553570(cell);
				if (Unknown_005501A0(bridge_deck_cell->MapCoords)) {
					Zone_Reset();
				}
				return(true);

			case 16:
				Internal_Destroy_Train_Bridge_NS_TopRight(bridge_deck_cell->Fetch_CellID(), Facing_Sub(dir, FACING_S));
				bridge_deck_cell->Set_Under_Rail_Bridge(FACING_W, false);
				bridge_deck_cell->OverlayData = 0;
				bridge_deck_cell->OverlayIndex = -1;
				Destroy_Train_Bridge_Connections_00553570(cell);
				if (Unknown_005501A0(bridge_deck_cell->MapCoords)) {
					Zone_Reset();
				}
				return(true);

			case 17:
				Internal_Destroy_Train_Bridge_NS_BottomLeft(bridge_deck_cell->Fetch_CellID(), dir);
				bridge_deck_cell->Set_Under_Rail_Bridge(FACING_W, false);
				bridge_deck_cell->OverlayData = 0;
				bridge_deck_cell->OverlayIndex = -1;
				Destroy_Train_Bridge_Connections_00553570(cell);
				if (Unknown_005501A0(bridge_deck_cell->MapCoords)) {
					Zone_Reset();
				}
				return(true);

			default:
				return(false);
		}

	}
	return(false);
}
