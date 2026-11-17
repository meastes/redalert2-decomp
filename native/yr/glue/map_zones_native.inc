
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFTS (lane Y3): functions unmatched in the YR tree, the text TibSun's
// (TibSun archive code/map.cpp; SPDX GPL-3.0-or-later, Copyright 2025 Electronic Arts Inc. and
// OpenTS contributors, EA's GPL section 7 terms; THIRD_PARTY.md); native changes are marked
// NATIVE. Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------

// TibSun map.cpp's file-level helpers the functions below use.
int LastAdjacentZone;

FacingType BridgeSideFacings[BRIDGE_COUNT] = {
	FACING_N,
	FACING_N,
	FACING_NONE,
	FACING_E,
	FACING_E,
	FACING_NONE,
	FACING_N,
	FACING_N,
	FACING_N,
	FACING_N,
	FACING_N,
	FACING_E,
	FACING_E,
	FACING_E,
	FACING_E,
	FACING_E
};

static unsigned Zone_Pack32(int zone1, int zone2)
{
	return(zone2 | (zone1 << 16));
}

static unsigned Zone_Pack8(int zone1, int zone2)
{
	return(zone2 & 0xF | ((zone1 & 0xF) << 4));
}

int FromRegionQuadrants[FACING_COUNT][2] = {
	{ 0, 1 }, { 1, 1 }, { 1, 3 }, { 3, 3 }, { 2, 3 }, { 2, 2 }, { 0, 2 }, { 0, 0 },
};

int ToRegionQuadrants[FACING_COUNT][2] = {
	{ 2, 3 }, { 2, 2 }, { 0, 2 }, { 0, 0 }, { 0, 1 }, { 1, 1 }, { 1, 3 }, { 3, 3 },
};

int MapClass::Zone_Reset(void)
{
	int i;
	int j;
	int k;
	int bestzone = -1;
	int bestspan = -1;

	CellZoneStruct * end = &CellZones[CellZoneCount];

	DynamicVectorClass<PassabilityType> vec;
	vec.Set_Growth_Step(300);

	ZoneAdjacency->Clear();

	for (i = 0; i < MZONE_COUNT; i++) {
		if (Zones[i] != NULL) {
			delete Zones[i];
			Zones[i] = NULL;
		}
	}

	CellZoneStruct * czone = CellZones;
	for (; czone < end; czone++) {
		czone->ZoneID = 0;
	}

	int zone = 1;			// Starting zone number.

	vec.Add(PASSABLE_OUTSIDE);

	int skip = 0;
	czone = CellZones;
	while (czone < end) {
		PassabilityType pass = (PassabilityType)czone->Passability;
		if (pass != PASSABLE_OUTSIDE && czone->ZoneID == 0) {
			LastAdjacentZone = 0;
			int span = Zone_Span(czone, zone, skip);
			if (span > bestspan) {
				bestzone = (unsigned short)zone;
				bestspan = span;
			}
			vec.Add(pass);
			zone++;
			czone += skip;
		} else {
			czone++;
		}
	}

	ZoneCount = (unsigned short)zone;

	for (i = ZoneConnections.Count() - 1; i >= 0; i--) {
		ZoneConnectionClass * connection = &ZoneConnections[i];
		if (connection->IsPassable) {
			int from_zone = Get_Cell_Zone_ID(connection->From);
			int to_zone = Get_Cell_Zone_ID(connection->To);
			if (to_zone != from_zone) {
				if (to_zone < from_zone) {
					swap(to_zone, from_zone);
				}
				ZoneAdjacency->Add_Object(ZONE_PAIR_HASH_SET::ObjectType(from_zone, to_zone));
			}
		}
	}

	unsigned short * zone_degree = new unsigned short[ZoneCount];
	for (i = 0; i < ZoneCount; i++) {
		zone_degree[i] = 0;
	}

	for (i = 0; i < 256; i++) {
		ZONE_PAIR_HASH_SET::BucketType & bucket = ZoneAdjacency->Buckets[i];
		j = bucket.Count();
		if (j > 0) {
			ZONE_PAIR_HASH_SET::ObjectType * obj = &bucket[0];
			do {
				unsigned int value = obj->Value;
				unsigned short zone1 = LOWORD(value);
				unsigned short zone2 = HIWORD(value);
				zone_degree[zone1]++;
				zone_degree[zone2]++;
				obj++;
			} while (--j);
		}
	}

	unsigned short ** zone_neighbors = new unsigned short *[ZoneCount];
	for (i = 0; i < ZoneCount; i++) {
		zone_neighbors[i] = new unsigned short[zone_degree[i]];
	}

	for (i = 0; i < ZoneCount; i++) {
		zone_degree[i] = 0;
	}

	for (i = 0; i < 256; i++) {
		ZONE_PAIR_HASH_SET::BucketType & bucket = ZoneAdjacency->Buckets[i];
		j = bucket.Count();
		if (j > 0) {
			ZONE_PAIR_HASH_SET::ObjectType * obj = &bucket[0];
			do {
				unsigned int value = obj->Value;
				unsigned short zone1 = LOWORD(value);
				unsigned short zone2 = HIWORD(value);
				zone_neighbors[zone1][zone_degree[zone1]] = zone2;
				zone_neighbors[zone2][zone_degree[zone2]] = zone1;
				zone_degree[zone1]++;
				zone_degree[zone2]++;
				obj++;
			} while (--j);
		}
	}

	unsigned char * zone_passability = new unsigned char[ZoneCount];
	for (i = 0; i < ZoneCount; i++) {
		zone_passability[i] = vec[i];
	}

	unsigned short * stack = new unsigned short[ZoneCount];

	for (MZoneType mzone = MZONE_FIRST; mzone < MZONE_COUNT; mzone++) {
		int next_movement_zone = 2;
		int * table = MZonePassability[mzone];

		unsigned short * nzone = new unsigned short[ZoneCount];
		Zones[mzone] = nzone;

		for (j = 0; j < ZoneCount; j++) {
			nzone[j] = table[zone_passability[j]] != TRAVERSAL_PASSABLE;
		}

		for (int zone_index = 0; zone_index < ZoneCount; zone_index++) {
			if (nzone[zone_index] == 0) {
				int stackcount = 1;
				stack[0] = zone_index;
				nzone[zone_index] = next_movement_zone;
				int passable = table[zone_passability[zone_index]];
				while (stackcount) {
					int current = stack[--stackcount];
					unsigned short * neighbors = zone_neighbors[current];
					int degree = zone_degree[current];
					for (k = degree - 1; k >= 0; k--) {
						int neighbor = neighbors[k];
						if (table[zone_passability[neighbor]] == passable && nzone[neighbor] == 0) {
							stack[stackcount++] = neighbor;
							nzone[neighbor] = next_movement_zone;
						}
					}
				}
				next_movement_zone++;
			}
		}

		nzone[0] = -1;
	}

	for (i = 0 ; i < ZoneCount; i++) {
		delete zone_neighbors[i];
	}

	delete [] zone_passability;
	delete [] zone_neighbors;
	delete [] zone_degree;
	delete [] stack;

	return(Zones[0][bestzone]);
}

bool MapClass::Is_Same_Cell_Zone(Cell const & from, Cell const & to, MZoneType mzone, bool from_bridge, bool to_bridge, bool leavemap)
{
	if (mzone == MZONE_NONE) {
		return(true);
	}

	bool flocal = Map.In_Local_Radar(from);
	bool finradar = Map.In_Radar(from);

	if (!flocal && finradar) {
		return(true);
	}

	bool tlocal = Map.In_Local_Radar(to);
	bool tinradar = Map.In_Radar(to);

	if (leavemap && flocal && !tlocal && tinradar) {
		return(true);
	}

	return(Get_Cell_Zone(&from, mzone, from_bridge) == Get_Cell_Zone(&to, mzone, to_bridge));   // NATIVE: the tree's Get_Cell_Zone takes the cell's address
}

void MapClass::Update_Cell_Zone(Cell const & cell)
{
	CellZoneStruct * cell_data = &CellZones[Get_Cell_Zone_Index(cell)];

	/*
	 * Calculate the offsets for accessing neighbors per facing in the flattened map array.
	 */
	int stride = Map.PlayRect.Width + Map.PlayRect.Height + 1;
	int offsets[FACING_COUNT] = {
		-(stride + 0),
		-(stride - 1),
		1,
		stride + 1,
		stride + 0,
		stride - 1,
		-1,
		-(stride + 1),
	};

	/*
	 * Don't assign a zone to cells outside the map.
	 */
	if (cell_data->Passability != PASSABLE_OUTSIDE) {

		/*
		 * Try to find a neighboring cell that's land passable (as a baseline).
		 */
		CellZoneStruct * land_neighbor = NULL;
		bool has_land_neighbor = false;
		for (FacingType i = FACING_FIRST; i < FACING_COUNT; i++) {
			land_neighbor = &cell_data[offsets[i]];
			if (land_neighbor->Passability == PASSABLE_LAND) {
				has_land_neighbor = true;
				break;
			}
		}

		/*
		 * If the cell neighbors another passable cell, we need to check the
		 * topology is not too complicated.
		 */
		if (has_land_neighbor && land_neighbor != NULL) {
			int transitions = 0;
			int prev_zone = 0;

			for (FacingType j = FACING_FIRST; j < FACING_COUNT; j++) {
				CellZoneStruct * neighbor = &cell_data[offsets[j]];
				if (Zones[MZONE_NORMAL][neighbor->ZoneID] != Zones[MZONE_NORMAL][prev_zone] && neighbor->Passability != PASSABLE_OUTSIDE) {
					prev_zone = neighbor->ZoneID;
					transitions++;
				}
			}

			/*
			 * If there are only a few transitions, join this cell to its passable neighbor's zone.
			 */
			if (transitions <= 3 && cell_data->Passability == PASSABLE_LAND) {
				cell_data->ZoneID = land_neighbor->ZoneID;
				return;
			}
		}

		/*
		 * If we've failed, recalculate the map's zones completely.
		 */
		Map.Zone_Reset();
	}
}

void MapClass::Compute_Zone_Connections(void)
{
	static int _bridge_subtiles1[] = {
		7, 7, -1, 7, 7, -1, 4, 4,
		4, 4,  4, 2, 2,  2, 2, 2
	};
	static FacingType _facings[] = {
		FACING_E, FACING_E, FACING_NONE, FACING_S, FACING_S, FACING_NONE, FACING_E, FACING_E,
		FACING_E, FACING_E,    FACING_E, FACING_S, FACING_S,    FACING_S, FACING_S, FACING_S
	};
	static int _bridge_subtiles2[] = {
		-1, -1, 4, -1, -1, 2, 4, 4,
		 4,  4, 4,  2,  2, 2, 2, 2
	};

	ZoneConnections.Clear();
	Reset_Iterator();
	CellClass * cptr = Iterate();

	while (cptr != NULL) {

		if (!cptr->Is_Tile_Bridge() && !cptr->Is_Tile_Train_Bridge()) {

			/*
			 * A tunnel entrance pairs with its exit cell; record the connection once,
			 * from the lower numbered cell of the pair.
			 */
			if (cptr->Has_Tunnel()) {
				if ((cptr->Adjacent_Cell(FACING_E).Has_Tunnel() && cptr->Adjacent_Cell(FACING_W).Has_Tunnel()) ||
					(cptr->Adjacent_Cell(FACING_S).Has_Tunnel() && cptr->Adjacent_Cell(FACING_N).Has_Tunnel())) {

					Cell cell = cptr->Get_Tunnel()->Exit;
					if (Map_Cell_Index(cptr->CellID) < Map_Cell_Index(cell)) {
						Cell exit = cptr->Get_Tunnel()->Exit;
						ZoneConnectionClass con;
						con.From = cptr->CellID;
						con.To = exit;
						con.IsPassable = true;
						con.Type = 1;
						ZoneConnections.Add(con);
					}
				}
			}

		} else {

			int faceindex;
			if (cptr->Is_Tile_Bridge()) {
				faceindex = cptr->ITType - IsometricTileTypeClass::BridgeSet;
			} else {
				faceindex = cptr->ITType - IsometricTileTypeClass::TrainBridgeSet;
			}

			/*
			 * Only walk the span from its starting subtile.
			 */
			if (_bridge_subtiles1[faceindex] == cptr->SubTile) {

				bool crossed = false;
				bool passable = true;
				FacingType facing = _facings[faceindex];

				/*
				 * Walk across the span until the far end subtile has been passed or
				 * the walk leaves the radar map.
				 */
				CellClass * bptr = cptr;
				for (;;) {
					bptr = &bptr->Adjacent_Cell(facing);

					if (In_Radar(bptr->CellID)) {

						if (!crossed) {

							if (bptr->Is_Tile_Bridge()) {
								int index = bptr->ITType - IsometricTileTypeClass::BridgeSet;
								if (index != -1 && _bridge_subtiles2[index] == bptr->SubTile) {
									crossed = true;
								}

							} else if (bptr->Is_Tile_Train_Bridge()) {
								int index = bptr->ITType - IsometricTileTypeClass::TrainBridgeSet;
								if (index != -1 && _bridge_subtiles2[index] == bptr->SubTile) {
									crossed = true;
								}
							} else if (!bptr->IsUnderBridge) {
								passable = false;
							}
							continue;
						}

					} else if (!crossed) {
						break;
					}

					/*
					 * Record the bridge span as a zone connection.
					 */
					ZoneConnectionClass con;
					con.From = cptr->CellID;
					con.To = bptr->Adjacent_Cell(Facing_Sub((unsigned char)facing, FACING_180)).CellID;
					con.IsPassable = passable;
					con.Type = 0;
					ZoneConnections.Add(con);
					break;
				}
			}
		}

		cptr = Iterate();
	}
}

void MapClass::Reset_Subzone(int subzone)
{
	/*
	 * Clear out every bucket of the connection hash set for this subzone level.
	 */
	SUBZONE_CONNECTION_HASH_SET * set = SubzoneConnectionHashTable[subzone];
	DynamicVectorClass<SubzoneTrackingStruct> * track = &SubzoneTracking[subzone];
	CellSubzoneStruct * subend = &CellSubzones[CellZoneCount];
	for (int bucket = 0; bucket < set->NumBuckets; bucket++) {
		set->Buckets[bucket].Clear();
	}

	/*
	 * Reset the per-cell subzone identifiers for this level and refresh the
	 * cached zone id and height from the master zone array.
	 */
	CellSubzoneStruct * subptr = CellSubzones;
	if (subptr < subend) {
		CellZoneStruct * zoneptr = CellZones;
		do {
			subptr->SubzoneID[subzone] = 0;
			subptr->ZoneID = zoneptr->ZoneID;
			subptr->Height = zoneptr->Height;
			subptr++;
			zoneptr++;
		} while (subptr < subend);
	}

	/*
	 * Seed the tracking list with the sentinel entry (index 0). This entry
	 * represents the "impassable" placeholder and is never a real subzone.
	 */
	int entry_count = 1;
	track->Add(SubzoneTrackingStruct());
	(*track)[0].ParentSubzoneID = 0;
	(*track)[0].Passability = PASSABLE_OUTSIDE;

	/*
	 * Walk every cell in the diamond shaped play area. Whenever an unassigned
	 * passable cell is found, flood fill a new subzone from it and record the
	 * resulting subzone in the tracking list.
	 */
	Rect bounds;
	bounds.Width = 1 << (subzone + 1);
	subptr = CellSubzones;
	CellZoneStruct * zoneptr = CellZones;
	bounds.Height = bounds.Width;
	int regionmask = bounds.Width - 1;
	int rowwidth = Map.PlayRect.Width + Map.PlayRect.Height + 1;
	int X = 0;
	bounds.X = 0;
	bounds.Y = 0;
	int rowx = 0;
	int Y = 0;
	while (subptr < subend) {
		int passability = zoneptr->Passability;
		int parent;
		if (subzone + 1 < SUBZONE_COUNT) {
			parent = subptr->SubzoneID[subzone + 1];
		} else {
			parent = 0;
		}

		if (passability == PASSABLE_OUTSIDE || subptr->SubzoneID[subzone] != 0) {
			subptr++;
			zoneptr++;
			X++;
			rowx = X;
		} else {
			Cell cell;
			cell.Y = Y;
			LastAdjacentZone = 0;
			cell.X = X;
			int count = Subzone_Span(subptr, subzone, entry_count, bounds, cell);

			track->Add(SubzoneTrackingStruct());

			SubzoneTrackingStruct * added = &(*track)[(unsigned short)entry_count];
			added->ParentSubzoneID = (unsigned short)parent;
			added->Passability = (PassabilityType)passability;
			added->Connections.Set_Growth_Step(16);
			added->ThreatRegion = (short)X / REGION_WIDTH + MAP_REGION_WIDTH * ((short)Y / REGION_HEIGHT) + (MAP_REGION_WIDTH + 1);

			zoneptr = &zoneptr[count];
			entry_count++;
			X = count + rowx;
			subptr = &subptr[count];
			rowx = X;
		}

		if (X == rowwidth) {
			X = 0;
			Y++;
			rowx = 0;
			bounds.X = 0;
			bounds.Y = Y - (regionmask & Y);
		} else if ((X & regionmask) == 0) {
			bounds.X = X;
		}
	}

	SubzoneTrackingEntryCount[subzone] = (unsigned short)entry_count;

	/*
	 * Re-register the zone-to-zone connections for this subzone level.
	 */
	Register_Subzone_Zone_Connections(subzone);

	/*
	 * Walk every bucket of the connection hash set and register both endpoints
	 * of each recorded connection into the subzone tracking list.
	 */
	for (int bucket_index = 0; bucket_index < 256; bucket_index++) {
		if (set->Buckets[bucket_index].Count() > 0) {
			SUBZONE_CONNECTION_HASH_SET::ObjectType * object = &set->Buckets[bucket_index][0];
			for (int index = set->Buckets[bucket_index].Count(); index > 0; index--) {
				unsigned packed = object->Value.SubzoneID;
				WORD low = LOWORD(packed);
				WORD high = HIWORD(packed);
				bool costly = object->Value.IsCrossBlock;

				SubzoneConnectionStruct conn;
				conn.SubzoneID = high;
				conn.IsCrossBlock = costly;
				(*track)[low].Connections.Add(conn);

				SubzoneConnectionStruct conn2;
				conn2.SubzoneID = low;
				conn2.IsCrossBlock = costly;
				(*track)[high].Connections.Add(conn2);

				object++;
			}
		}
	}
}

void MapClass::Register_Zone_Connection_Entries(ZoneConnectionClass & connection, int index)
{
	SUBZONE_CONNECTION_HASH_SET::ObjectType connstr;
	SUBZONE_CONNECTION_HASH_SET * set = SubzoneConnectionHashTable[index];

	Cell from = connection.From;
	Cell to = connection.To;

	Cell enter1;
	Cell enter2;
	Cell newcell1;
	Cell newcell2;

	CellClass * cell1ptr = &Map[from];

	if (!cell1ptr->Is_Tile_Bridge() && !cell1ptr->Is_Tile_Train_Bridge()) {

		FacingType enter_dir = cell1ptr->Get_Tunnel()->EnterDir;
		enter1 = Adjacent_Cell(from, (FacingType)(enter_dir + FACING_90));
		enter2 = Adjacent_Cell(from, (FacingType)(enter_dir - FACING_90));

		CellClass * enterptr1 = &Map[enter1];
		TubeClass * tube1 = enterptr1->Get_Tunnel();
		CellClass * enterptr2 = &Map[enter2];
		TubeClass * tube2 = enterptr2->Get_Tunnel();

		if (tube1 == NULL || tube2 == NULL) {
			return;
		}

		newcell1 = Follow_Path(enter1, tube1->Count, tube1->Dirs);
		newcell2 = Follow_Path(enter2, tube2->Count, tube2->Dirs);
	} else {
		IsometricTileType bridge_set = cell1ptr->Is_Tile_Bridge() ? IsometricTileTypeClass::BridgeSet : IsometricTileTypeClass::TrainBridgeSet;
		FacingType dir = BridgeSideFacings[cell1ptr->ITType - bridge_set];

		enter1 = Adjacent_Cell(from, dir);
		enter2 = Adjacent_Cell(from, (FacingType)(dir + FACING_180));
		newcell1 = Adjacent_Cell(to, dir);
		newcell2 = Adjacent_Cell(to, (FacingType)(dir - FACING_180));
	}

	{
		int from_zone = CellSubzones[Get_Cell_Subzone_Index(from)].SubzoneID[index];
		int to_zone = CellSubzones[Get_Cell_Subzone_Index(to)].SubzoneID[index];

		int value = Zone_Pack32(from_zone, to_zone);
		int bucket_index = Zone_Pack8(from_zone, to_zone);
		connstr.Key = value;
		connstr.Value = SubzoneConnectionStruct(value);
		connstr.Value.IsCrossBlock = 0;
		set->Add_Object(bucket_index, connstr);
	}

	{
		int from_zone = CellSubzones[Get_Cell_Subzone_Index(enter1)].SubzoneID[index];
		int to_zone = CellSubzones[Get_Cell_Subzone_Index(newcell1)].SubzoneID[index];

		int value = Zone_Pack32(from_zone, to_zone);
		int bucket_index = Zone_Pack8(from_zone, to_zone);
		connstr.Key = value;
		connstr.Value = SubzoneConnectionStruct(value);
		connstr.Value.IsCrossBlock = 0;
		set->Add_Object(bucket_index, connstr);
	}

	{
		int from_zone = CellSubzones[Get_Cell_Subzone_Index(enter2)].SubzoneID[index];
		int to_zone = CellSubzones[Get_Cell_Subzone_Index(newcell2)].SubzoneID[index];

		int value = Zone_Pack32(from_zone, to_zone);
		int bucket_index = Zone_Pack8(from_zone, to_zone);
		connstr.Key = value;
		connstr.Value = SubzoneConnectionStruct(value);
		connstr.Value.IsCrossBlock = 0;
		set->Add_Object(bucket_index, connstr);
	}
}

void MapClass::Update_Cell_Subzones(Cell const & cell)
{
	if (In_Local_Radar(cell)) {

		int y;
		int x;
		Cell trycell;
		for (int subzone = 2; subzone >= 0; subzone--) {

			/*
			 * Compute the square window of cells (aligned to the level's grid size)
			 * that surrounds the changed cell for this coarseness level.
			 */
			int var = subzone + 1;
			Rect bounds;
			bounds.Width = 1 << var;
			bounds.Height = bounds.Width;
			bounds.X = cell.X - cell.X % bounds.Width;
			bounds.Y = cell.Y - cell.Y % bounds.Height;

			/*
			 * Clear out every bucket of the connection hash set for this level.
			 */
			DynamicVectorClass<unsigned short> collected;
			SUBZONE_CONNECTION_HASH_SET * set = SubzoneConnectionHashTable[subzone];
			for (int bucket = 0; bucket < set->NumBuckets; bucket++) {
				set->Buckets[bucket].Clear();
			}

			DynamicVectorClass<SubzoneTrackingStruct> * track = &SubzoneTracking[subzone];

			/*
			 * Collect the distinct subzone ids that currently occupy the window and
			 * clear each cell's subzone id for this level (refreshing its zone id).
			 */
			for (y = bounds.Y; y < bounds.Y + bounds.Height; y++) {
				for (x = bounds.X; x < bounds.Width + bounds.X; x++) {
					CellSubzoneStruct * subptr = &CellSubzones[(short)x + (short)y * (PlayRect.Width + PlayRect.Height + 1)];
					unsigned short subid = subptr->SubzoneID[subzone];
					if (subid != 0) {
						int found;
						for (found = collected.Count() - 1; found >= 0; found--) {
							if (collected[found] == subid) {
								break;
							}
						}
						if (found == -1) {
							collected.Add(subid);
						}
					}
					subptr->SubzoneID[subzone] = 0;
					subptr->ZoneID = CellZones[(short)x + (short)y * (PlayRect.Width + PlayRect.Height + 1)].ZoneID;
				}
			}

			/*
			 * Remove the collected subzones from the tracking graph. For each one,
			 * delete the mirror connection entry from every neighbor it connected to,
			 * then clear its own connection list.
			 */
			for (int idx = collected.Count() - 1; idx >= 0; idx--) {
				unsigned short subid = collected[idx];
				SubzoneTrackingStruct * entry = &SubzoneTracking[subzone][subid];
				for (int i = entry->Connections.Count() - 1; i >= 0; i--) {
					SubzoneTrackingStruct * neighbor = &SubzoneTracking[subzone][entry->Connections[i].SubzoneID];
					int j;
					for (j = neighbor->Connections.Count() - 1; j >= 0; j--) {
						if (neighbor->Connections[j].SubzoneID == subid) {
							neighbor->Connections.Delete_Index(j);
							break;
						}
					}
				}
				entry->Connections.Clear();
			}

			/*
			 * Flood fill fresh subzones for the now unassigned passable cells in the
			 * window, recording a tracking entry for each new subzone.
			 */
			int entry_count = SubzoneTrackingEntryCount[subzone];
			for (y = bounds.Y; y < bounds.Y + bounds.Height; y++) {
				for (x = bounds.X; x < bounds.Width + bounds.X; x++) {

					Cell cella;
					cella.X = x;
					cella.Y = y;
					if (In_Local_Radar(cella)) {

						CellSubzoneStruct * subptr = &CellSubzones[(short)y * (PlayRect.Width + PlayRect.Height + 1) + (short)x];
						int passability = CellZones[(short)y * (PlayRect.Width + PlayRect.Height + 1) + (short)x].Passability;
						if (passability != PASSABLE_OUTSIDE && subptr->SubzoneID[subzone] == 0) {

							trycell.Y = y;
							trycell.X = x;
							Subzone_Span(subptr, subzone, entry_count, bounds, trycell);

							track->Add(SubzoneTrackingStruct());

							SubzoneTrackingStruct * added = &(*track)[entry_count];
							int parent = 0;
							if (subzone != 2) {
								parent = subptr->SubzoneID[subzone + 1];
							}
							added->ParentSubzoneID = (unsigned short)parent;
							added->Passability = (PassabilityType)passability;
							added->Connections.Set_Growth_Step(16);
							added->ThreatRegion = (short)x / REGION_WIDTH + MAP_REGION_WIDTH * ((short)y / REGION_HEIGHT) + (MAP_REGION_WIDTH + 1);

							entry_count++;
							if (entry_count == 0) {
								Reset_All_Subzones();
								return;
							}
						}
					}
				}
			}
			SubzoneTrackingEntryCount[subzone] = entry_count;

			/*
			 * Re-register any zone connections whose endpoints fall inside the window.
			 */
			for (int connection_index = ZoneConnections.Count() - 1; connection_index >= 0; connection_index--) {
				ZoneConnectionClass & connection = ZoneConnections[connection_index];
				if ((connection.From.X >= bounds.X && connection.From.X < bounds.Width + bounds.X
					&& connection.From.Y >= bounds.Y && connection.From.Y < bounds.Y + bounds.Height)
					|| (connection.To.X >= bounds.X && connection.To.X < bounds.Width + bounds.X
					&& connection.To.Y >= bounds.Y && connection.To.Y < bounds.Y + bounds.Height)) {
					if (connection.IsPassable) {
						Register_Zone_Connection_Entries(connection, subzone);
					}
				}
			}

			/*
			 * Walk every bucket of the connection hash set and register both endpoints
			 * of each recorded connection into the subzone tracking list.
			 */
			for (int bucket_index = 0; bucket_index < 256; bucket_index++) {
				if (set->Buckets[bucket_index].Count() > 0) {
					SUBZONE_CONNECTION_HASH_SET::ObjectType * object = &set->Buckets[bucket_index][0];
					for (int index = set->Buckets[bucket_index].Count(); index > 0; index--) {
						unsigned packed = object->Value.SubzoneID;
						WORD low = LOWORD(packed);
						WORD high = HIWORD(packed);
						bool costly = object->Value.IsCrossBlock;

						SubzoneConnectionStruct conn;
						conn.SubzoneID = high;
						conn.IsCrossBlock = costly;
						(*track)[low].Connections.Add(conn);

						SubzoneConnectionStruct conn2;
						conn2.SubzoneID = low;
						conn2.IsCrossBlock = costly;
						(*track)[high].Connections.Add(conn2);

						object++;
					}
				}
			}
		}

		/*
		 * Propagate the parent subzone identifiers down through the coarser levels
		 * for the finest (8 cell) window around the changed cell.
		 */
		int basex = cell.X - cell.X % 8;
		int basey = cell.Y - cell.Y % 8;
		for (y = basey; y < basey + 8; y++) {
			for (x = basex; x < basex + 8; x++) {
				trycell.X = x;
				trycell.Y = y;
				if (In_Local_Radar(trycell)) {
					CellSubzoneStruct * subptr = &CellSubzones[(short)x + (short)y * (PlayRect.Width + PlayRect.Height + 1)];
					for (int level = 0; level < 2; level++) {
						SubzoneTracking[level][subptr->SubzoneID[level]].ParentSubzoneID = subptr->SubzoneID[level + 1];
					}
				}
			}
		}
		Search.Reset();
	}
}

void MapClass::Place_Down(Cell const & cell, ObjectClass * object)
{
	if (!object) return;
	if (cell.X < 0 || cell.Y < 0 || cell.X >= MAP_CELL_W || cell.Y >= MAP_CELL_H) {	// NATIVE (lane Y12): a cell off the cell array (the original's Is_Valid reads past it) is logged and left alone; open: an LTNK in Allied 2 and Soviet 7 gets a garbage position from its drive locomotor
		Coord where = object->Center_Coord();
		static int logged = 0;
		if (logged++ < 5) fprintf(stderr, "place-down: %s at cell %d,%d (coord %d,%d,%d) is off the map\n", object->Class_Of()->Name(), cell.X, cell.Y, where.X, where.Y, where.Z);
		return;
	}

	if (object->Class_Of()->IsFootprint && object->In_Which_Layer() == LAYER_GROUND)  {
		Cell xlist[32];
		List_Copy(object->Occupy_List(), ARRAY_SIZE(xlist), xlist);
		Cell const * list = xlist;
		while (*list != REFRESH_EOL) {
			Cell newcell = cell + *list++;
			if (Is_Valid(newcell)) {
				(*this)[newcell].Occupy_Down(object, object->IsOnBridge);
				(*this)[newcell].Recalc_Attributes();
			}
		}
	}
}

void MapClass::Pick_Up(Cell const & cell, ObjectClass * object)
{
	if (!object) return;
	if (cell.X < 0 || cell.Y < 0 || cell.X >= MAP_CELL_W || cell.Y >= MAP_CELL_H) {	// NATIVE (lane Y12): as Place_Down
		Coord where = object->Center_Coord();
		static int logged = 0;
		if (logged++ < 5) fprintf(stderr, "pick-up: %s at cell %d,%d (coord %d,%d,%d) is off the map\n", object->Class_Of()->Name(), cell.X, cell.Y, where.X, where.Y, where.Z);
		return;
	}

	if (object->Class_Of()->IsFootprint && object->In_Which_Layer() == LAYER_GROUND)  {
		Cell xlist[32];
		List_Copy(object->Occupy_List(), ARRAY_SIZE(xlist), xlist);
		Cell const * list = xlist;
		while (*list != REFRESH_EOL) {
			Cell newcell = cell + *list++;
			if (Is_Valid(newcell)) {
				(*this)[newcell].Occupy_Up(object, object->IsOnBridge);
				(*this)[newcell].Recalc_Attributes();
			}
		}
	}
}

int MapClass::Cell_Threat(Cell const & cell, HouseClass const & house)
{
	int threat = house.Regions[Map.Cell_Region(Map[cell].Fetch_CellID())].Threat_Value();

#if NEVER
	if (!threat && Map[cell].IsVisible) {
		threat = 1;
	}
#endif
	return(threat);
}

int MapClass::Region_Threat(HouseClass * house, int level, int from_subzone, int to_subzone)
{
	switch (level) {
		default:
			break;

		case 0:
			return(0);

		case 1:
			return(house->Regions[Map.SubzoneTracking[level][to_subzone].ThreatRegion].Threat_Value());

		case 2: {
			int from_region = Map.SubzoneTracking[level][from_subzone].ThreatRegion;
			int index = from_region - 1;
			int col = index % MAP_REGION_WIDTH;
			int row = (from_region - col) / MAP_REGION_HEIGHT;
			int from_x = REGION_WIDTH * col;
			int from_y = REGION_HEIGHT * (row - 1);

			short from_block_x = from_x - (from_x / REGION_WIDTH % 2 != 0);
			short from_block_y = from_y - (from_y / REGION_HEIGHT % 2 != 0);

			int to_region = Map.SubzoneTracking[level][to_subzone].ThreatRegion;
			index = to_region - 1;
			col = index % MAP_REGION_WIDTH;
			row = (to_region - col) / MAP_REGION_HEIGHT;
			int to_x = REGION_WIDTH * col;
			int to_y = REGION_HEIGHT * (row - 1);

			short to_block_x = to_x - (to_x / REGION_WIDTH % 2 != 0);
			short to_block_y = to_y - (to_y / REGION_HEIGHT % 2 != 0);

			if (from_block_x == to_block_x && from_block_y == to_block_y) {
				return((house->Regions[Cell_Region(Cell(from_x, from_y))].Threat_Value() + house->Regions[Cell_Region(Cell(to_x, to_y))].Threat_Value()) >> 1);
			}

			int facing;
			if (to_block_x > from_block_x) {
				if (to_block_y > from_block_y) {
					facing = FACING_SE;
				} else if (to_block_y >= from_block_y) {
					facing = FACING_E;
				} else {
					facing = FACING_NE;
				}
			} else if (to_block_x < from_block_x) {
				if (to_block_y > from_block_y) {
					facing = FACING_SW;
				} else if (to_block_y >= from_block_y) {
					facing = FACING_W;
				} else {
					facing = FACING_NW;
				}
			} else if (to_block_y > from_block_y) {
				facing = FACING_S;
			} else {
				facing = FACING_N;
			}

			int from_threat1 = house->Regions[Cell_Region(RegionQuadrantOffsets[FromRegionQuadrants[facing][0]] + Cell(from_block_x, from_block_y))].Threat_Value();
			int from_threat2 = house->Regions[Cell_Region(RegionQuadrantOffsets[FromRegionQuadrants[facing][1]] + Cell(from_block_x, from_block_y))].Threat_Value();
			int from_threat = from_threat1;
			if (from_threat1 >= from_threat2) {
				from_threat = from_threat2;
			}

			int to_threat1 = house->Regions[Cell_Region(RegionQuadrantOffsets[ToRegionQuadrants[facing][0]] + Cell(to_block_x, to_block_y))].Threat_Value();
			int to_threat2 = house->Regions[Cell_Region(RegionQuadrantOffsets[ToRegionQuadrants[facing][1]] + Cell(to_block_x, to_block_y))].Threat_Value();
			if (to_threat1 >= to_threat2) {
				to_threat1 = to_threat2;
			}

			return((to_threat1 + from_threat) >> 1);
		}
	}
	return(0);
}

bool MapClass::Can_Repair_Bridge(Cell const & cell)
{
	static short _x[] = {
		1, 1, 0, 2, 2, 2, 0, 0,
		0, 0, 0, 2, 2, 2, 2, 2
	};
	static short _y[] = {
		2, 2, 2, 1, 1, 0, 2, 2,
		2, 2, 2, 0, 0, 0, 0, 0
	};
	static FacingType _facings[] = {
		FACING_E,       FACING_E,    FACING_W,    FACING_S,    FACING_S,    FACING_N, FACING_NONE, FACING_NONE,
		FACING_NONE, FACING_NONE, FACING_NONE, FACING_NONE, FACING_NONE, FACING_NONE, FACING_NONE, FACING_NONE
	};

	bool found = false;
	CellClass * cellptr = NULL;
	int offset = -1;

	for (int y = -2; y < 3; y++) {
		for (int x = -2; x < 3; x++) {

			IsometricTileType ittype = Map[Cell(cell + Cell(x, y))].ITType;

			if (ittype >= IsometricTileTypeClass::TrainBridgeSet && ittype < IsometricTileTypeClass::TrainBridgeSet + TRAIN_BRIDGE_COUNT) {
				found = true;
				cellptr = &Map[Cell(cell + Cell(x, y))];
				offset = ittype - IsometricTileTypeClass::TrainBridgeSet;
			} else if (ittype >= IsometricTileTypeClass::BridgeSet && ittype < IsometricTileTypeClass::BridgeSet + BRIDGE_COUNT) {
				found = true;
				cellptr = &Map[Cell(cell + Cell(x, y))];
				offset = ittype - IsometricTileTypeClass::BridgeSet;
			} else {
				Cell cellnum = cell + Cell(x, y);
				if (Is_Low_Bridge(cellnum)) {
					found = false;
					cellptr = &Map[Cell(cell + Cell(x, y))];
				}
			}
		}
	}

	if (found) {

		int width = IsometricTileTypes[cellptr->ITType]->Width;
		Cell topleft(cellptr->CellID.X - cellptr->SubTile % width, cellptr->CellID.Y - cellptr->SubTile / width);

		Cell entry = topleft + Cell(_x[offset], _y[offset]);
		FacingType dir = _facings[offset];
		Cell search = entry;

		if (dir != FACING_NONE) {
			while (true) {
				int index = Zone_Connection_Index(search, 3, 0);
				if (index < 0) {
					break;
				}
				ZoneConnectionClass * con = &ZoneConnections[index];
				if (!con->IsPassable) {
					return(true);
				}
				if (con->From == entry) {
					entry = con->To;
					search = Adjacent_Cell(entry, dir);
				} else {
					if (con->To != entry) {
						return(true);
					}
					entry = con->From;
					search = Adjacent_Cell(entry, dir);
				}
			}
		}
	} else if (cellptr != NULL) {

		if (cellptr->Overlay >= OVERLAY_LOWBRIDGE_01 && cellptr->Overlay <= OVERLAY_LOWBRIDGE_09 ||
			cellptr->Overlay >= OVERLAY_LOWBRIDGE_19 && cellptr->Overlay <= OVERLAY_LOWBRIDGE_22 ||
			cellptr->Overlay == OVERLAY_LOWBRIDGE_27) {

			for (Cell walk = cellptr->CellID; ; walk.X--) {
				if (!Is_Low_Bridge(walk)) {
					break;
				}
				OverlayType overlay = Map[walk].Overlay;
				if (overlay == OVERLAY_LOWBRIDGE_27 || overlay == OVERLAY_LOWBRIDGE_28) {
					return(true);
				}
			}

			for (Cell scan = cellptr->CellID; ; scan.X++) {
				if (!Is_Low_Bridge(scan)) {
					break;
				}
				OverlayType overlay = Map[scan].Overlay;
				if (overlay == OVERLAY_LOWBRIDGE_27 || overlay == OVERLAY_LOWBRIDGE_28) {
					return(true);
				}
			}

		} else {

			for (Cell scan = cellptr->CellID; ; scan.Y++) {
				if (!Is_Low_Bridge(scan)) {
					break;
				}
				OverlayType overlay = Map[scan].Overlay;
				if (overlay == OVERLAY_LOWBRIDGE_27 || overlay == OVERLAY_LOWBRIDGE_28) {
					return(true);
				}
			}

			for (Cell walk = cellptr->CellID; ; walk.Y--) {
				if (!Is_Low_Bridge(walk)) {
					break;
				}
				OverlayType overlay = Map[walk].Overlay;
				if (overlay == OVERLAY_LOWBRIDGE_27 || overlay == OVERLAY_LOWBRIDGE_28) {
					return(true);
				}
			}
		}
	}

	return(false);
}

// NATIVE: YR's MapClass::Is_Fogged (0x005865E0, 5 bytes: `xor al, al; ret 4`) always answers false;
// TibSun's fog test is not used.
bool MapClass::Is_Fogged(Coord const &)
{
	return(false);
}

int MapClass::Zone_Span(CellZoneStruct * data, int zone, int & skip)
{
	int passability = data->Passability;
	bool nopass = passability == PASSABLE_NO;
	int cell_height = data->Height;

	CellZoneStruct * begin = data;
	CellZoneStruct * end = data;

	/*
	**	Find the full extent of the current span by first scanning leftward
	**	until a boundary is reached.
	*/
	while (begin->Passability == passability) {
		if (abs(begin->Height - cell_height) >= 2) {
			break;
		}
		begin->ZoneID = zone;
		cell_height = begin->Height;
		begin--;
	}

	int begin_zone = begin->ZoneID;
	if (begin_zone != 0 && (abs(begin->Height - cell_height) < 2 || nopass) && begin_zone != LastAdjacentZone && begin_zone != (unsigned short)zone) {
		ZoneAdjacency->Add_Object(ZONE_PAIR_HASH_SET::ObjectType(begin_zone, (unsigned short)zone));
		LastAdjacentZone = begin_zone;
	}

	/*
	 * Scan rightward until a boundary is reached. This will then define the
	 * extent of the current span. The end pointer advances, so its ZoneID is
	 * the active zone boundary; data remains fixed at the seed cell.
	 */
	while (end->Passability == passability) {
		if (abs(end->Height - cell_height) >= 4) {
			break;
		}
		end->ZoneID = zone;
		cell_height = end->Height;
		end++;
	}

	int end_zone = end->ZoneID;
	if (end_zone != 0 && (abs(end->Height - cell_height) < 2 || nopass) && end_zone != LastAdjacentZone && end_zone != (unsigned short)zone) {
		ZoneAdjacency->Add_Object(ZONE_PAIR_HASH_SET::ObjectType(end_zone, (unsigned short)zone));
		LastAdjacentZone = end_zone;
	}

	int filled = end - begin - 1;
	begin++;
	int skip1 = 0;
	int skip2 = 0;
	skip = end - data - 1;
	end--;

	int stride = PlayRect.Height + PlayRect.Width + 1;
	int diag = stride + 1;

	CellZoneStruct * fbegin = begin - diag;
	CellZoneStruct * fbegin2 = begin + stride - 1;
	CellZoneStruct * fend2 = end + diag;
	CellZoneStruct * fend = end - stride + 1;

	/*
	**	At this point we know the bounds of the current span. Fill in the zone values
	**	for the entire span.
	*/
	while (fbegin <= fend) {
		int zzone = fbegin->ZoneID;
		CellZoneStruct * adjacent;
		if (fbegin < fend - 1) {
			adjacent = fbegin + diag;
		} else {
			adjacent = fbegin == fend - 1 ? fbegin + stride : fbegin - 1 + stride;
		}
		if (zzone == 0) {
			if (fbegin->Passability == passability && abs(fbegin->Height - adjacent->Height) < 2) {
				filled += Zone_Span(fbegin, zone, skip1);
				fbegin += skip1;
			} else {
				fbegin++;
			}
		} else {
			if (zzone != (unsigned short)zone && zzone != LastAdjacentZone && (abs(fbegin->Height - adjacent->Height) < 2 || nopass)) {
				ZoneAdjacency->Add_Object(ZONE_PAIR_HASH_SET::ObjectType(zzone, (unsigned short)zone));
				LastAdjacentZone = zzone;
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
		int id = fbegin2->ZoneID;
		CellZoneStruct * adjacent;
		if (fbegin2 < fend2 - 1) {
			adjacent = fbegin2 - stride + 1;
		} else {
			adjacent = fbegin2 == fend2 - 1 ? fbegin2 - stride : fbegin2 - diag;
		}
		if (id == 0) {
			if (fbegin2->Passability == passability && abs(fbegin2->Height - adjacent->Height) < 2) {
				filled += MapClass::Zone_Span(fbegin2, zone, skip2);
				fbegin2 += skip2;
			} else {
				fbegin2++;
			}
		} else {
			if (id != (unsigned short)zone && id != LastAdjacentZone && (abs(fbegin2->Height - adjacent->Height) < 2 || nopass)) {
				ZoneAdjacency->Add_Object(ZONE_PAIR_HASH_SET::ObjectType(id, (unsigned short)zone));
				LastAdjacentZone = id;
			}
			fbegin2++;
		}
	}

	return(filled);
}

int MapClass::Subzone_Span(CellSubzoneStruct * seed, int subzone_level, int subzone_id, Rect const & bounds, Cell const & cell)
{
	int zone = seed->ZoneID;

	CellSubzoneStruct * begin = seed;
	CellSubzoneStruct * end = seed;
	SUBZONE_CONNECTION_HASH_SET::ObjectType entry;

	int x = cell.X;
	int y = cell.Y;

	int xmin = bounds.X;
	int ymin = bounds.Y;
	int last_adjacent = -1;
	int begin_x = x;
	int xmax = bounds.Width + bounds.X - 1;
	int end_x = x;
	int ymax = bounds.Height + bounds.Y - 1;

	SUBZONE_CONNECTION_HASH_SET * table = SubzoneConnectionHashTable[subzone_level];
	int prev_height = seed->Height;

	/*
	**	Find the full extent of the current span by first scanning leftward
	**	until a boundary is reached.
	*/
	while (true) {
		if (x < xmin) {
			break;
		}
		if (abs(begin->Height - prev_height) >= 2) {
			break;
		}
		begin->SubzoneID[subzone_level] = (short)subzone_id;
		prev_height = begin->Height;
		int prev_zone = begin[-1].ZoneID;
		begin--;
		x--;
		if (prev_zone != zone) {
			break;
		}
	}

	begin_x = x;
	int begin_subzone = begin->SubzoneID[subzone_level];
	if (begin_subzone > 0 && abs(begin->Height - prev_height) < 2 && begin_subzone != last_adjacent) {
		Cell probe;
		probe = Cell(x, y);
		if (In_Local_Radar(probe, true)) {
			probe = Cell(x + 1, y);
			if (In_Local_Radar(probe, true)) {
				int packed_pair = Zone_Pack32(begin_subzone, (unsigned short)subzone_id);
				entry.Value.IsCrossBlock = false;
				entry.Key = entry.Value.SubzoneID = packed_pair;
				table->Add_Object(Zone_Pack8(begin_subzone, (unsigned short)subzone_id), entry);
				last_adjacent = begin_subzone;
			}
		}
	}

	/*
	**	Scan rightward until a boundary is reached. This will then define the
	**	extent of the current span.
	*/
	prev_height = seed->Height;
	while (end->ZoneID == zone) {
		if (end_x > xmax) {
			break;
		}
		if (abs(end->Height - prev_height) >= 2) {
			break;
		}
		end->SubzoneID[subzone_level] = (short)subzone_id;
		prev_height = end->Height;
		end++;
		end_x++;
	}

	int end_subzone = end->SubzoneID[subzone_level];
	if (end_subzone > 0 && abs(end->Height - prev_height) < 2 && end_subzone != last_adjacent) {
		Cell probe;
		probe.X = end_x;
		probe.Y = y;
		if (In_Local_Radar(probe, true)) {
			probe = Cell(end_x - 1, y);
			if (In_Local_Radar(probe, true)) {
				int packed_pair = Zone_Pack32(end_subzone, (unsigned short)subzone_id);
				entry.Value.IsCrossBlock = false;
				entry.Key = entry.Value.SubzoneID = packed_pair;
				table->Add_Object(Zone_Pack8(end_subzone, (unsigned short)subzone_id), entry);
				last_adjacent = end_subzone;
			}
		}
	}

	int skip = end - seed - 1;
	int stride = PlayRect.Width + PlayRect.Height + 1;

	CellSubzoneStruct * above = &begin[-stride];
	CellSubzoneStruct * below = &begin[stride];
	x = begin_x;

	Cell span_cell;
	span_cell.X = 0;
	span_cell.Y = 0;

	/*
	 * Fill the row above the current span, recursing into any same-zone neighbor
	 * spans and recording connections to adjacent foreign subzones.
	 */
	while (x <= end_x) {
		int shadow_subzone = above->SubzoneID[subzone_level];
		CellSubzoneStruct * span_ptr;
		if (x == begin_x) {
			Cell left;
			left.Y = y;
			left.X = x + 1;
			span_cell = left;
			span_ptr = &above[stride + 1];
		} else if (x < end_x) {
			Cell middle;
			middle.X = x;
			middle.Y = y;
			span_cell = middle;
			span_ptr = &above[stride];
		} else {
			Cell right;
			right.X = x - 1;
			right.Y = y;
			span_cell = right;
			span_ptr = &above[stride - 1];
		}

		if (shadow_subzone != 0 || y <= ymin || x < xmin || x > xmax) {
			if (shadow_subzone != (unsigned short)subzone_id && shadow_subzone != last_adjacent) {
				if (shadow_subzone != 0) {
					if (abs(span_ptr->Height - above->Height) < 2 && In_Local_Radar(span_cell, true)) {
						Cell shadow_cell;
						shadow_cell.Y = y - 1;
						shadow_cell.X = x;
						if (In_Local_Radar(shadow_cell, true)) {
							int packed_pair = Zone_Pack32(shadow_subzone, (unsigned short)subzone_id);
							if (x < xmin) {
								entry.Value.IsCrossBlock = true;
							} else {
								entry.Value.IsCrossBlock = false;
								if (x > xmax) {
									entry.Value.IsCrossBlock = true;
								}
							}
							entry.Key = entry.Value.SubzoneID = packed_pair;
							table->Add_Object(Zone_Pack8(shadow_subzone, (unsigned short)subzone_id), entry);
							last_adjacent = shadow_subzone;
						}
					}
				}
			}
			above++;
			x++;
		} else if (above->ZoneID == zone) {
			if (abs(span_ptr->Height - above->Height) < 2) {
				Cell shadow_cell;
				shadow_cell.X = x;
				shadow_cell.Y = y - 1;
				Subzone_Span(above, subzone_level, subzone_id, bounds, shadow_cell);
				continue;
			}
			above++;
			x++;
		} else {
			above++;
			x++;
		}
	}

	/*
	 * Fill the row below the current span using the same rules.
	 */
	x = begin_x;
	while (x <= end_x) {
		int shadow_subzone = below->SubzoneID[subzone_level];
		CellSubzoneStruct * span_ptr;
		if (x == begin_x) {
			Cell left;
			left.X = x + 1;
			left.Y = y;
			span_cell = left;
			span_ptr = &below[-stride + 1];
		} else if (x < end_x) {
			Cell middle;
			middle.X = x;
			middle.Y = y;
			span_cell = middle;
			span_ptr = &below[-stride];
		} else {
			Cell right;
			span_ptr = &below[-stride - 1];
			right.X = x - 1;
			right.Y = y;
			span_cell = right;
		}

		if (shadow_subzone != 0 || y >= ymax || x < xmin || x > xmax) {
			if (shadow_subzone != (unsigned short)subzone_id && shadow_subzone != last_adjacent) {
				if (shadow_subzone != 0) {
					if (abs(below->Height - span_ptr->Height) < 2 && In_Local_Radar(span_cell, true)) {
						Cell shadow_cell;
						shadow_cell.X = x;
						shadow_cell.Y = y + 1;
						if (In_Local_Radar(shadow_cell, true)) {
							int packed_pair = Zone_Pack32(shadow_subzone, (unsigned short)subzone_id);
							if (x < xmin) {
								entry.Value.IsCrossBlock = true;
							} else {
								entry.Value.IsCrossBlock = false;
								if (x > xmax) {
									entry.Value.IsCrossBlock = true;
								}
							}
							entry.Key = entry.Value.SubzoneID = packed_pair;
							table->Add_Object(Zone_Pack8(shadow_subzone, (unsigned short)subzone_id), entry);
							last_adjacent = shadow_subzone;
						}
					}
				}
			}
			below++;
			x++;
		} else if (below->ZoneID == zone && abs(below->Height - span_ptr->Height) < 2) {
			Subzone_Span(below, subzone_level, subzone_id, bounds, Cell(x, y + 1));
		} else {
			below++;
			x++;
		}
	}

	return(skip);
}
