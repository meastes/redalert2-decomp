
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y3), appended to the map unit: MapClass::Set_Map_Dimensions, YR
// 0x00565C10 (5,374 bytes, unmatched in ra2-decomp; RA2 counterpart 0x005484E0, whose draft in
// src/recovered/map.cpp is this same text). The text is TibSun's (TibSun archive code/map.cpp;
// SPDX GPL-3.0-or-later, Copyright 2025 Electronic Arts Inc. and OpenTS contributors, EA's GPL
// section 7 terms; THIRD_PARTY.md); native name changes are marked NATIVE. Behaviour code for the
// native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------
void MapClass::Set_Map_Dimensions(Rect const & rect, bool reset_cells, int cell_height, bool refresh_map)
{
	int i = 0;

	int w = PlayRect.Width;
	int h = 2 * Map.PlayRect.Height + 8;
	CellClass *cptr;
	CellClass *cells = NULL;
	CellClass *cells_base = NULL;

	Cell cell(CELL_NONE);

	for (i = 0; i < Objects.Count(); i++) {
		Objects[i]->Mark(MARK_UP);
	}

	for (i = 0; i < Array.Length(); i++) {
		if (!In_Radar(Cell(i % MAP_CELL_W, i / MAP_CELL_H))) {
			CellClass *c = Array[i];
			if (c != NULL) {
				c->Height = cell_height;
			}
		}
	}

	int cellcount = 0;

	if (!reset_cells) {
		cells = (CellClass *) new char[sizeof(CellClass) * w * h];//new CellClass[w * h];
		cells_base = cells;
		Reset_Iterator();
		cptr = Iterate();
		cell = cptr->Fetch_CellID();
		while (cptr != NULL) {
			*cells = *cptr;
			cells->BridgeDeckCell = NULL;
			cells->UnusedCell = NULL;
			cptr = Iterate();
			cellcount++;
			cells++;
		}
	}

	PlayRect = rect;
	PlayRect.X = 0;
	PlayRect.Y = 0;

	if (PlayRect.Width > 63 || PlayRect.Height > 63) {
		Map.Set_Radar_Scale(2);
	}

	MapRegionClass::MapStartDiagonal = Map.PlayRect.Width;
	MapRegionClass::MapEndDiagonal = Map.PlayRect.Width + 2 * Map.PlayRect.Height;

	MapRect.X = 1;
	MapRect.Y = 1;
	int msize = rect.Height + rect.Width - 1;
	MapRect.Width = msize;
	MapRect.Height = msize;

	for (int y = 0; y < 2 * MapRect.Height + 2; y++) {
		for (int x = 0; x < MapRect.Width + 2; x++) {
			Cell cell(x, y);
			if (In_Radar(cell)) {
				int idx = (MAP_CELL_H * y) + x;
				CellClass *c = Array[idx];
				if (c == NULL) {
					CellClass *cc = new CellClass;
					cc->Set_CellID(cell);
					Array[idx] = cc;
				} else {
					c = new(c) CellClass;
					c->Set_CellID(cell);
				}
				Array[idx]->Height = cell_height;
			}
		}
	}

	if (!reset_cells) {
		Reset_Iterator();
		Cell icell = Iterate()->CellID;
		Cell cellpos = Cell(rect.Y + rect.X, rect.Y - rect.X);
		cell = (icell - cell) - cellpos;

		cells = cells_base;
		while (cellcount > 0) {
			cptr = cells;
			Cell scell = cptr->CellID;
			Cell shifted = scell + cell;

			CellClass *c = &(*this)[shifted];

			Cell dcell = c->CellID;
			if (c != NULL) {
				*c = *cptr;
				c->Set_CellID(dcell);
			}

			cellcount--;
			cells++;
		}

		Reset_Iterator();
		cptr = Iterate();
		while (cptr != NULL) {

			Cell c = cptr->Fetch_CellID();

			if (c.X + c.Y < PlayRect.Width - 2 * rect.Y + 1 ||
				c.Y - c.X > PlayRect.Width + 2 * rect.X - 1 ||
				c.X + c.Y > PlayRect.Width + 2 * (h + PlayRect.Height - rect.Y - Map.PlayRect.Height) ||
				c.X - c.Y > PlayRect.Width + 2 * (w - rect.X - Map.PlayRect.Width) - 1) {

				int cellindex = c.X + c.Y * MAP_CELL_W;

				delete Array[cellindex];
				Array[cellindex] = new CellClass;
				Array[cellindex]->Set_CellID(c);
				Array[cellindex]->ITType = ISOTILE_NONE;
				Array[cellindex]->SubTile = 0;
				Array[cellindex]->Ramp = 0;
				Array[cellindex]->Overlay = OVERLAY_NONE;
				Array[cellindex]->Height = cell_height;
			}
			cptr = Iterate();
		}

		for (i = 0; i < WAYPT_COUNT; i++) {
			if (Scen->Is_Valid_Waypoint(i)) {
				Scen->Set_Waypoint(i, Scen->Get_Waypoint_Cell(i) + cell);
			}
		}

		for (i = 0; i < Tubes.Count(); i++) {
			Tubes[i]->Enter += cell;
			Tubes[i]->Exit += cell;
		}

		Coord coord = Coord(cell, 0) - Coord(CELL_LEPTON_W / 2, CELL_LEPTON_H / 2, 0);

		for (i = 0; i < Objects.Count(); i++) {
			ObjectClass *obj = Objects[i];
			if (obj->RTTI != RTTI_ANIM) {
				obj->Set_Coord(coord + obj->Get_Coord());
			}
		}

		for (i = 0; i < Anims.Count(); i++) {
			AnimClass *anim = Anims[i];
			if (anim->OwnerObject == NULL) {   // NATIVE: TibSun xObject is the tree's OwnerObject
				anim->Set_Coord(coord + anim->Get_Coord());
			}
		}

		ScenarioInit++;

		for (i = 0; i < Objects.Count(); i++) {
			if (Objects[i]->RTTI != RTTI_PARTICLESYSTEM || ((ParticleSystemClass *)Objects[i])->Class->Behaves_Like() != PSYS_BEHAVIOR_GAS) {
				Cell cell = Objects[i]->PositionCell;
				const Cell *list = Objects[i]->Occupy_List();
				while (*list != REFRESH_EOL) {
					Cell ncell = cell + *list;
					if (!In_Radar(ncell)) {
						Objects[i]->Limbo();
						delete Objects[i];
						i--;
						break;
					}
					list++;
				}
			}
		}

		ScenarioInit--;

		for (i = 0; i < Array.Length(); i++) {
			CellClass *c1 = Array[i];
			CellClass **c2 = &Array[i];
			if (c1 != NULL) {
				if (!In_Radar(Cell(i % MAP_CELL_W, i / MAP_CELL_H))) {
					*c2 = NULL;
					delete c1;
				}
			}
		}

		Reset_Iterator();
		cptr = Iterate();
		while (cptr != NULL) {
			if (cptr->IsBridgeDeck && cptr->BridgeDeckCell == NULL) {
				FacingType facing = cptr->IsBridgeEastWest ? FACING_N : FACING_E;
				if (cptr->Overlay == OVERLAY_BRIDGE1 || cptr->Overlay == OVERLAY_BRIDGE2) {
					cptr->Set_Under_Bridge(facing);
				} else {
					cptr->Set_Under_Rail_Bridge(facing);
				}
			}
			cptr = Iterate();
		}

		bool freshcalled;
		if (refresh_map) {
			Fresh_Map();
			freshcalled = true;
		} else {
			freshcalled = false;
		}

		for (i = 0; i < Objects.Count(); i++) {
			Objects[i]->Mark(MARK_DOWN);
		}

		Map.Flag_To_Redraw(GS_REDRAW_ALL);

		if (freshcalled) {
			refresh_map = false;
		}
	}

	if (refresh_map) {
		Fresh_Map();
	}

	new(&BlubCell) CellClass;
	delete (void *)cells_base; /// operator delete is called directly here.
}

// -------------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y3): MapClass::AddContentAt (gamemd 0x005683C0) is unmatched in the
// YR tree; written from gamemd.exe's code as the mirror of the tree's RemoveContentAt (0x005687F0):
// the object occupies each footprint cell, and a building that hides things raises the cells its
// height covers (and its RemoveOccupy/AddOccupy cells). An out-of-range cell is the map's BlubCell
// through MapClass::operator[], as the original. Behaviour code for the native build only.
// -------------------------------------------------------------------------------------------------
void MapClass::AddContentAt(Cell const * coords, TechnoClass * content)
{
	if (!content) return;
	Cell const * list;
	int height;
	int length = Array.Length();

	if (content->Class_Of()->IsFootprint && content->In_Which_Layer() == LAYER_GROUND) {
		Cell xlist[32];
		List_Copy(content->Occupy_List(), ARRAY_SIZE(xlist), xlist);
		for (list = xlist; *list != REFRESH_EOL; list++) {
			Cell newcell = *coords + *list;
			if (Cell_In_Range(newcell, length)) {
				(*this)[newcell].Occupy_Down(content, content->IsOnBridge);
				(*this)[newcell].Recalc_Attributes();
			}
		}

		if (content->What_Am_I() == RTTI_BUILDING) {
			BuildingClass * building = As_Building(content);
			if (building->Class->CanHideThings) {
				height = building->Class->OccupyHeight - 1;
				if (height <= 0) height = 1;

				char seen[2048];
				memset(seen, 0, sizeof(seen));
				for (list = xlist; *list != REFRESH_EOL; list++) {
					Cell newcell = *coords + *list;
					int rx = list->X;
					int ry = list->Y;
					for (int i = height; i > 0; i--) {
						if (newcell.X < 0 || newcell.Y < 0) continue;
						if (rx < 0) rx += 16;
						if (ry < 0) ry += 16;
						if (!seen[ry * 16 + rx]) {
							seen[ry * 16 + rx] = 1;
							if (Cell_In_Range(newcell, length)) {
								(*this)[newcell].OccupyHeightsCoveringMe++;
							}
						}
						newcell.X--;
						newcell.Y--;
						rx--;
						ry--;
					}
				}

				for (int i = 0; i < 8; i++) {
					if (building->Class->RemoveOccupy[i].X != 0xFFFF || building->Class->RemoveOccupy[i].Y != 0xFFFF) {
						Cell newcell = *coords;
						newcell.X += building->Class->RemoveOccupy[i].X;
						newcell.Y += building->Class->RemoveOccupy[i].Y;
						if (Cell_In_Range(newcell, length)) {
							(*this)[newcell].OccupyHeightsCoveringMe++;
						}
					}
					if (building->Class->AddOccupy[i].X != 0xFFFF || building->Class->AddOccupy[i].Y != 0xFFFF) {
						Cell newcell = *coords;
						newcell.X += building->Class->AddOccupy[i].X;
						newcell.Y += building->Class->AddOccupy[i].Y;
						if (Cell_In_Range(newcell, length)) {
							(*this)[newcell].OccupyHeightsCoveringMe++;
						}
					}
				}
			}
		}
	}
}

// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y12): MapClass::Reveal, gamemd.exe 0x00577D90 (404 bytes, unmatched in the
// YR tree; a Soviet campaign mission reached it as a link-closure trap; the spy satellite's
// HouseClass::Recalc_Spy_Satellite and the reveal crate call it). Written from gamemd.exe's code, as the
// tree's Reveal_The_Map and Reshroud (its neighbours) are written: the house's map is clear; for the
// player, unless already visionary, every cell (but the three edge cells a multiplayer mode keeps hidden)
// is mapped, visible and out of the fog with no shroud counter and no gap covering it, then the shroud's
// edges are rebuilt and the whole screen redrawn.
// ---------------------------------------------------------------------------------------------
void MapClass::Reveal(HouseClass * house)
{
	if (house != NULL) {
		Houses[house->HeapID]->MapIsClear = true;
	}
	if (house != PlayerPtr && house != NULL) {
		return;
	}
	if (house == NULL) {
		FUN_004068E0("We tried to clear the shroud without an owner");
	}
	bool edge = (Session.Type == GAME_IPX || Session.Type == GAME_INTERNET) && Session.MPGameMode != NULL && Session.MPGameMode->vt_entry_04();
	if (PlayerPtr->IsVisionary) {
		return;
	}
	Map.FUN_004ADEE0(false, false);
	PlayerPtr->IsVisionary = true;
	Reset_Iterator();
	for (CellClass * cellptr = Iterate(); cellptr != NULL; cellptr = Iterate()) {
		int x = cellptr->CellID.X;
		int y = cellptr->CellID.Y;
		if (edge && ((x == 7 && y == PlayRect.Width + 5) || (x == 13 && y == PlayRect.Width + 11)
			|| (x == PlayRect.Height + 13 && y == PlayRect.Width + PlayRect.Height - 15))) {
			continue;
		}
		cellptr->ShroudCounter = 0;
		cellptr->GapsCoveringThisCell = 0;
		cellptr->IsMapped = true;
		cellptr->IsVisible = true;
		cellptr->IsFogVisible = true;
		cellptr->IsFogMapped = true;
	}
	Map.FUN_004ADCD0(false, false);
	Map.sub_657CE0();
	Flag_To_Redraw(GS_REDRAW_ALL);
}


// NON-EXACT NATIVE DRAFTS (lane Y14): MapClass::Repair_Train_Bridge and MapClass::Repair_Bridge, unmatched in the
// YR tree (gamemd.exe's engineer bridge repair, called from the TibSun-text InfantryClass::Per_Cell_Process draft;
// gamemd.exe has RepairWoodBridgeAt 0x00570050 and RepairConcreteBridgeAt 0x00573540 there, not yet compared
// instruction by instruction). The text is TibSun's (TibSun archive code/map.cpp, adopted by yr_adopt_tibsun.py;
// the attribution at the top of this file), which the tree's matched span helpers (Repair_Train_Bridge_Span,
// Repair_Train_Bridge_From_Cell, Set_Bridge_End_State, ...) come from. Behaviour code for the native build only.
void MapClass::Repair_Train_Bridge(Cell const & cell)
{
	/*
	 * Scan the 5x5 block of cells around the supplied cell looking for a low
	 * bridge overlay. If one is found, hand off to the low bridge repair routine
	 * and exit.
	 */
	for (int x = -2; x < 3; x++) {
		for (int y = -2; y < 3; y++) {
			if (Is_Low_Bridge(cell + Cell(x, y))) {
				Repair_Low_Bridge_Span(cell + Cell(x, y));
				return;
			}
		}
	}

	/*
	 * Locate a cell that is part of a bridge. Start at the supplied cell and, if it
	 * is not itself bridge related, walk up to three cells out along each of the
	 * eight facings until a bridge cell is found.
	 */
	CellClass * cellptr = &Map[cell];

	int facing = FACING_FIRST;
	if (!cellptr->IsUnderBridge && !cellptr->WasUnderBridge) {
		for (int index = 0; index < FACING_COUNT; index++) {

			Cell adjacent = Adjacent_Cell(cell, Facing_Add(facing, FACING_0));
			cellptr = &Map[adjacent];
			if (cellptr->IsUnderBridge || cellptr->WasUnderBridge) {
				break;
			}

			adjacent = Adjacent_Cell(adjacent, Facing_Add(facing, FACING_0));
			cellptr = &Map[adjacent];
			if (cellptr->IsUnderBridge || cellptr->WasUnderBridge) {
				break;
			}

			adjacent = Adjacent_Cell(adjacent, Facing_Add(facing, FACING_0));
			cellptr = &Map[adjacent];
			if (cellptr->IsUnderBridge || cellptr->WasUnderBridge) {
				break;
			}

			facing = Facing_Add(facing, FACING_45);
		}
	}

	/*
	 * Nothing bridge related found nearby - there is nothing to repair.
	 */
	if (!cellptr->IsUnderBridge && !cellptr->WasUnderBridge) {
		return;
	}

	/*
	 * Resolve the span anchor cell.
	 */
	Cell origin;
	if (cellptr->IsUnderBridge) {

		/*
		 * A live bridge cell points at the bridge owner (or is the owner).
		 */
		origin = cellptr->Get_Bridge_Deck_Cell();

	} else {

		/*
		 * An already damaged ("was under") bridge cell. Walk along the bridge body
		 * until a cell that is no longer marked as a former bridge cell is found,
		 * giving up after four steps, then step back two cells to recover the span
		 * end.
		 */
		int count = 0;
		int step = cellptr->IsBridgeEastWest ? FACING_S : FACING_E;
		Cell walk = cellptr->CellID;
		CellClass * walkptr;
		while (true) {
			walk = Adjacent_Cell(walk, Facing_Add(step, FACING_0));
			walkptr = &Map[walk];
			if (!walkptr->WasUnderBridge) {
				break;
			}
			if (++count >= 4) {
				return;
			}
		}

		FacingType back = Facing_Sub(step, FACING_180);
		origin = Adjacent_Cell(Adjacent_Cell(walk, back), back);
	}

	/*
	 * Walk the span looking for the end or middle tiles, repairing as we go.
	 */
	Cell spancell = origin;
	int advance = cellptr->IsBridgeEastWest ? FACING_W : FACING_N;

	char zonechanged = 0;
	IsometricTileType ittype = ISOTILE_CLEAR;

	DynamicVectorClass<Cell> cells;

	Rect dirty;

	while (true) {

		/*
		 * Bail the moment the span steps outside the playable rectangle.
		 */
		if (!Map.In_Radar(spancell)) {
			break;
		}

		if (Array[spancell.X + spancell.Y * MAP_CELL_W] != NULL) {

			CellClass * spanptr = &Map[spancell];
			ittype = IsometricTileType(spanptr->ITType - IsometricTileTypeClass::TrainBridgeSet + 1);

			/*
			 * East/west bridge head - restore it and register the dirty area.
			 */
			if ((ittype == IsometricTileTypeClass::BridgeTopLeft1
					|| ittype == IsometricTileTypeClass::BridgeTopLeft2) && spanptr->SubTile == 8) {
				Set_Bridge_End_State(spancell, false, false);
				Repair_Train_Bridge_Span(&Map[spancell], FACING_E, &dirty);
				TacticalMap->Register_Dirty_Area(dirty, false);
				break;
			}

			/*
			 * East/west bridge middle piece.
			 */
			if ((ittype == IsometricTileTypeClass::BridgeMiddle1
					|| ittype == IsometricTileTypeClass::BridgeMiddle1 + 3
					|| ittype == IsometricTileTypeClass::BridgeMiddle1 + 4
					|| ittype == IsometricTileTypeClass::BridgeMiddle1 + 1
					|| ittype == IsometricTileTypeClass::BridgeMiddle1 + 2) && spanptr->SubTile == 5) {

				if (ittype == IsometricTileTypeClass::BridgeMiddle1 + 4) {

					/*
					 * A fully destroyed middle - raise the three affected cells back
					 * to bridge height and queue them for recalculation.
					 */
					Set_Bridge_Middle_State(spancell, IsometricTileType(IsometricTileTypeClass::BridgeMiddle1 + IsometricTileTypeClass::TrainBridgeSet - 1), ISOTILE_INVALID, -1, false);

					Cell middle = Adjacent_Cell(spancell, FACING_W);
					Map[middle].Height += BRIDGE_CELL_HEIGHT;
					Map[Adjacent_Cell(middle, FACING_N)].Height += BRIDGE_CELL_HEIGHT;
					Map[Adjacent_Cell(middle, FACING_S)].Height += BRIDGE_CELL_HEIGHT;

					zonechanged = Register_Subzone_Connections(middle);

					for (int dx = 0; dx < 2; dx++) {
						for (int dy = -2; dy < 3; dy++) {
							cells.Add(Cell(middle + Cell(dx, dy)));
						}
					}
				}

				Repair_Train_Bridge(spancell + Cell(-2, 0));
				Repair_Train_Bridge_Span(&Map[spancell], FACING_E, &dirty);
				TacticalMap->Register_Dirty_Area(dirty, false);
				break;
			}

			/*
			 * North/south bridge head - restore it and register the dirty area.
			 */
			if ((ittype == IsometricTileTypeClass::BridgeTopRight1
					|| ittype == IsometricTileTypeClass::BridgeTopRight2) && spanptr->SubTile == 12) {
				Set_Bridge_End_State(spancell, false, false);
				Repair_Train_Bridge_Span(&Map[spancell], FACING_S, &dirty);
				TacticalMap->Register_Dirty_Area(dirty, false);
				break;
			}

			/*
			 * North/south bridge middle piece.
			 */
			if ((ittype == IsometricTileTypeClass::BridgeMiddle2
					|| ittype == IsometricTileTypeClass::BridgeMiddle2 + 3
					|| ittype == IsometricTileTypeClass::BridgeMiddle2 + 4
					|| ittype == IsometricTileTypeClass::BridgeMiddle2 + 1
					|| ittype == IsometricTileTypeClass::BridgeMiddle2 + 2) && spanptr->SubTile == 7) {

				if (ittype == IsometricTileTypeClass::BridgeMiddle2 + 4) {

					Set_Bridge_Middle_State(spancell, IsometricTileType(IsometricTileTypeClass::BridgeMiddle2 + IsometricTileTypeClass::TrainBridgeSet - 1), ISOTILE_INVALID, -1, false);

					Cell middle = Adjacent_Cell(spancell, FACING_N);
					Map[middle].Height += BRIDGE_CELL_HEIGHT;
					Map[Adjacent_Cell(middle, FACING_E)].Height += BRIDGE_CELL_HEIGHT;
					Map[Adjacent_Cell(middle, FACING_W)].Height += BRIDGE_CELL_HEIGHT;

					zonechanged = Register_Subzone_Connections(middle);

					for (int dy = 0; dy < 2; dy++) {
						for (int dx = -2; dx < 3; dx++) {
							cells.Add(Cell(middle + Cell(dx, dy)));
						}
					}
				}

				Repair_Train_Bridge(spancell + Cell(0, -2));
				Repair_Train_Bridge_Span(&Map[spancell], FACING_S, &dirty);
				TacticalMap->Register_Dirty_Area(dirty, false);
				break;
			}
		}

		spancell = Adjacent_Cell(spancell, Facing_Add(advance, FACING_0));
	}

	if (zonechanged) {
		Zone_Reset();
	}

	if (cells.Count() > 0) {
		Recalc_Cells_In_List(cells);
	}
}

void MapClass::Repair_Bridge(Cell const & cell)
{
	/*
	 * Scan the 5x5 block of cells around the supplied cell looking for a low
	 * bridge overlay. If one is found, hand off to the low bridge repair routine
	 * and exit.
	 */
	for (int x = -2; x < 3; x++) {
		for (int y = -2; y < 3; y++) {
			if (Is_Low_Bridge(cell + Cell(x, y))) {
				Repair_Low_Bridge_Span(cell + Cell(x, y));
				return;
			}
		}
	}

	/*
	 * Locate a cell that is part of a bridge. Start at the supplied cell and, if it
	 * is not itself bridge related, walk up to three cells out along each of the
	 * eight facings until a bridge cell is found.
	 */
	CellClass * cellptr = &Map[cell];

	char facing = FACING_FIRST;
	if (!cellptr->IsUnderBridge && !cellptr->WasUnderBridge) {
		for (int index = 0; index < FACING_COUNT; index++) {

			Cell adjacent = Adjacent_Cell(cell, Facing_Add(facing, FACING_0));
			cellptr = &Map[adjacent];
			if (cellptr->IsUnderBridge || cellptr->WasUnderBridge) {
				break;
			}

			adjacent = Adjacent_Cell(adjacent, Facing_Add(facing, FACING_0));
			cellptr = &Map[adjacent];
			if (cellptr->IsUnderBridge || cellptr->WasUnderBridge) {
				break;
			}

			adjacent = Adjacent_Cell(adjacent, Facing_Add(facing, FACING_0));
			cellptr = &Map[adjacent];
			if (cellptr->IsUnderBridge || cellptr->WasUnderBridge) {
				break;
			}

			facing = Facing_Add(facing, FACING_45);
		}
	}

	/*
	 * Nothing bridge related found nearby - there is nothing to repair.
	 */
	if (!cellptr->IsUnderBridge && !cellptr->WasUnderBridge) {
		return;
	}

	/*
	 * Resolve the span anchor cell.
	 */
	Cell origin;
	if (cellptr->IsUnderBridge) {

		/*
		 * A live bridge cell points at the bridge owner (or is the owner).
		 */
		origin = cellptr->Get_Bridge_Deck_Cell();

	} else {

		/*
		 * An already damaged ("was under") bridge cell. Walk along the bridge body
		 * until a cell that is no longer marked as a former bridge cell is found,
		 * giving up after four steps, then step back two cells to recover the span
		 * end.
		 */
		int count = 0;
		int step = cellptr->IsBridgeEastWest ? FACING_S : FACING_E;
		Cell walk = cellptr->CellID;
		CellClass * walkptr;
		while (true) {
			walk = Adjacent_Cell(walk, Facing_Add(step, FACING_0));
			walkptr = &Map[walk];
			if (!walkptr->WasUnderBridge) {
				break;
			}
			if (++count >= 4) {
				return;
			}
		}

		FacingType back = Facing_Sub(step, FACING_180);
		origin = Adjacent_Cell(Adjacent_Cell(walk, back), back);
	}

	/*
	 * Walk the span looking for the end or middle tiles, repairing as we go.
	 */
	Cell spancell = origin;
	int advance = cellptr->IsBridgeEastWest ? FACING_W : FACING_N;

	char zonechanged = 0;
	IsometricTileType ittype = ISOTILE_CLEAR;

	DynamicVectorClass<Cell> cells;

	Rect dirty;

	/*
	 * Bail the moment the span steps outside the map rectangle.
	 */
	if (spancell.X >= MapRect.X) {

		while (true) {

			if (spancell.X > MapRect.X + MapRect.Width) {
				break;
			}
			if (spancell.Y < MapRect.Y || spancell.Y > MapRect.Y + MapRect.Height) {
				break;
			}

			if (Array[spancell.X + spancell.Y * MAP_CELL_W] != NULL) {

				CellClass * spanptr = &Map[spancell];
				ittype = IsometricTileType(spanptr->ITType - IsometricTileTypeClass::BridgeSet + 1);

				/*
				 * East/west bridge head - restore it and register the dirty area.
				 */
				if ((ittype == IsometricTileTypeClass::BridgeTopLeft1
						|| ittype == IsometricTileTypeClass::BridgeTopLeft2) && spanptr->SubTile == 8) {
					Set_Bridge_End_State(spancell, false, false);
					Repair_High_Bridge_Span(&Map[spancell], FACING_E, &dirty);
					TacticalMap->Register_Dirty_Area(dirty, false);
					break;
				}

				/*
				 * East/west bridge middle piece.
				 */
				if ((ittype == IsometricTileTypeClass::BridgeMiddle1
						|| ittype == IsometricTileTypeClass::BridgeMiddle1 + 3
						|| ittype == IsometricTileTypeClass::BridgeMiddle1 + 4
						|| ittype == IsometricTileTypeClass::BridgeMiddle1 + 1
						|| ittype == IsometricTileTypeClass::BridgeMiddle1 + 2) && spanptr->SubTile == 5) {

					if (ittype == IsometricTileTypeClass::BridgeMiddle1 + 4) {

						/*
						 * A fully destroyed middle - raise the three affected cells back
						 * to bridge height and queue them for recalculation.
						 */
						Set_Bridge_Middle_State(spancell, IsometricTileType(IsometricTileTypeClass::BridgeMiddle1 + IsometricTileTypeClass::BridgeSet - 1), ISOTILE_INVALID, -1, false);

						Cell middle = Adjacent_Cell(spancell, FACING_W);
						Map[middle].Height += BRIDGE_CELL_HEIGHT;
						Map[Adjacent_Cell(middle, FACING_N)].Height += BRIDGE_CELL_HEIGHT;
						Map[Adjacent_Cell(middle, FACING_S)].Height += BRIDGE_CELL_HEIGHT;

						zonechanged = Register_Subzone_Connections(middle);

						for (int dx = 0; dx < 2; dx++) {
							for (int dy = -2; dy < 3; dy++) {
								cells.Add(Cell(middle + Cell(dx, dy)));
							}
						}
					}

					Repair_Bridge(spancell + Cell(-2, 0));
					Repair_High_Bridge_Span(&Map[spancell], FACING_E, &dirty);
					TacticalMap->Register_Dirty_Area(dirty, false);
					break;
				}

				/*
				 * North/south bridge head - restore it and register the dirty area.
				 */
				if ((ittype == IsometricTileTypeClass::BridgeTopRight1
						|| ittype == IsometricTileTypeClass::BridgeTopRight2) && spanptr->SubTile == 12) {
					Set_Bridge_End_State(spancell, false, false);
					Repair_High_Bridge_Span(&Map[spancell], FACING_S, &dirty);
					TacticalMap->Register_Dirty_Area(dirty, false);
					break;
				}

				/*
				 * North/south bridge middle piece.
				 */
				if ((ittype == IsometricTileTypeClass::BridgeMiddle2
						|| ittype == IsometricTileTypeClass::BridgeMiddle2 + 3
						|| ittype == IsometricTileTypeClass::BridgeMiddle2 + 4
						|| ittype == IsometricTileTypeClass::BridgeMiddle2 + 1
						|| ittype == IsometricTileTypeClass::BridgeMiddle2 + 2) && spanptr->SubTile == 7) {

					if (ittype == IsometricTileTypeClass::BridgeMiddle2 + 4) {

						Set_Bridge_Middle_State(spancell, IsometricTileType(IsometricTileTypeClass::BridgeMiddle2 + IsometricTileTypeClass::BridgeSet - 1), ISOTILE_INVALID, -1, false);

						Cell middle = Adjacent_Cell(spancell, FACING_N);
						Map[middle].Height += BRIDGE_CELL_HEIGHT;
						Map[Adjacent_Cell(middle, FACING_E)].Height += BRIDGE_CELL_HEIGHT;
						Map[Adjacent_Cell(middle, FACING_W)].Height += BRIDGE_CELL_HEIGHT;

						zonechanged = Register_Subzone_Connections(middle);

						for (int dy = 0; dy < 2; dy++) {
							for (int dx = -2; dx < 3; dx++) {
								cells.Add(Cell(middle + Cell(dx, dy)));
							}
						}
					}

					Repair_Bridge(spancell + Cell(0, -2));
					Repair_High_Bridge_Span(&Map[spancell], FACING_S, &dirty);
					TacticalMap->Register_Dirty_Area(dirty, false);
					break;
				}
			}

			spancell = Adjacent_Cell(spancell, Facing_Add(advance, FACING_0));
			if (spancell.X < MapRect.X) {
				break;
			}
		}
	}

	if (zonechanged) {
		Zone_Reset();
	}

	if (cells.Count() > 0) {
		Recalc_Cells_In_List(cells);
	}
}


// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE STAND-IN (lane N8): MapClass::Pick_Random_Tile_Variant, gamemd.exe 0x0057B440
// (848 bytes, the size of Red Alert 2's 0x0055D640), unmatched in the YR tree; the random map
// generator's shores and clear ground (Place_Shore, the cliff fill) place the pending tile with it.
// Main's exact text of the RA2 twin (src/recovered/map.cpp) under the YR tree's names; gamemd.exe's
// shore group and facing tables are 0x0082A7F4 and 0x0082A89C (42 entries each, read from the
// player's gamemd.exe like the tree's other DAT_ globals).
// ---------------------------------------------------------------------------------------------
extern int DAT_0082A7F4[42];	// the shore pieces' groups
extern int DAT_0082A89C[42];	// the shore pieces' facings

bool MapClass::Pick_Random_Tile_Variant(IsometricTileType first, int last, int height, int region_id, bool & result, bool set_region)
{
	if (Map.PendingObject->What_Am_I() == RTTI_ISOTILETYPE) {
		if (set_region) region_id = 0;
		IsometricTileTypeClass * type = (IsometricTileTypeClass *)Map.PendingObject;
		IsoTileSet const * image = (IsoTileSet const *)type->Get_Image_Data();
		if (image != NULL) {
			for (int row = 0; row < type->Height; ++row) {
				for (int column = 0; column < type->Width; ++column) {
					Cell cell(column + Map.ZoneCell.X, row + Map.ZoneCell.Y);
					if (cell.X + cell.Y <= PlayRect.Width || cell.X - cell.Y >= PlayRect.Width ||
						cell.Y - cell.X >= PlayRect.Width || cell.X + cell.Y > PlayRect.Width + 2 * PlayRect.Height)
						continue;
					CellClass * cellptr = &(*this)[cell];
					int subtile = column + type->Width * row;
					IsoTileRecord const * record = image->Fetch_Record_Pointer_Unsafe(subtile);
					if (record == NULL) continue;
					int id = RandomMapGen.Get_Cell_Data_Region(cell);
					if (set_region) id = 0;
					bool place = true;
					if (id > 0 && id != region_id && region_id != -1) {
						int existing = cellptr->ITType - IsometricTileTypeClass::ShorePieces;
						int pending = type->HeapID - IsometricTileTypeClass::ShorePieces;
						if (!cellptr->Is_Tile_Clear()) {
							if (existing < 0 || pending < 0 || existing > 41 || pending > 41 ||
								DAT_0082A7F4[existing] != DAT_0082A7F4[pending]) {
								result = false;
								return(false);
							}
							return(true);
						}
						RandomMapGen.Set_Cell_Data_Region(cell, region_id);
					} else if (id == region_id) {
						int existing = cellptr->ITType - IsometricTileTypeClass::ShorePieces;
						int pending = type->HeapID - IsometricTileTypeClass::ShorePieces;
						if (existing >= 0 && pending >= 0 && existing <= 41 && pending <= 41) {
							int difference = abs(DAT_0082A89C[existing] - DAT_0082A89C[pending]);
							if (difference >= 3 && difference <= 5) {
								result = false;
								return(false);
							}
						}
					} else if (cellptr->Is_Tile_Clear()) {
						RandomMapGen.Set_Cell_Data_Region(cell, region_id);
					} else if (region_id != -1) {
						result = false;
						return(false);
					} else {
						place = false;
					}
					if (place) {
						if (cellptr->Is_Tile_Clear() || (cellptr->ITType >= first && cellptr->ITType <= last)) {
							cellptr->ITType = type->HeapID;
							cellptr->SubTile = subtile;
							cellptr->Height = height + record->Height;
						} else {
							if ((cellptr->Is_Tile_Shore() && MapClass::Is_Tile_Cliff(type->HeapID, subtile)) ||
								(type->HeapID >= IsometricTileTypeClass::ShorePieces && type->HeapID < IsometricTileTypeClass::ShorePieces + 42 &&
								 cellptr->Is_Tile_Cliff()))
								result = false;
							return(false);
						}
					}
				}
			}
			return(true);
		}
	}
	return(false);
}
