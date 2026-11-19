// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE STAND-INS (lane N8): Yuri's Revenge's random map generator. The skirmish
// Choose Map dialog's Create Random Map (and its Preview) runs MapGeneratorClass::Generate_Random_Map,
// which the YR tree (pinned ra2-decomp, and its latest) declares and never defines, with the
// generator functions only it reaches. Written from gamemd.exe's instructions (yr_dis.py) in the
// shape of Red Alert 2's twins: each YR body was compared with RA2's (game.exe) and has the same
// size and the same calls except where noted. The text is the rebuild tree's mapgen.cpp
// (rebuild-trial; TibSun archive / OpenTS / ra2-decomp text, GPL-3.0-or-later, EA's GPL section 7
// terms; THIRD_PARTY.md) and main's src/recovered/mapgen.cpp, under the YR tree's names.
//   gamemd.exe   bytes  RA2 twin (game.exe)            function
//   0x0058C800   1678   0x0056E100 (same size)         MapRegionClass::Create_Region_From_Cell
//   0x0058D620   4012   0x0056EF20 (same size)         MapRegionClass::Split_Region
//   0x00591D80   1724   0x00573680 (same size)         MapRegionClass::Build_North_Ramp
//   0x00594B50    998   0x00576420 (925)               Generate_Starting_Points (YR: the zone regions inline, ends in FUN_0058B820)
//   0x005981F0   1889   0x00579960 (1897)              MapGeneratorClass::Load_Settings (YR reads RMGMD.INI)
//   0x00598960   3018   0x0057A0D0 (3086)              MapGeneratorClass::Generate_Random_Map
//   0x0059E740   6472   0x0057FF20 (same size)         MapGeneratorClass::Place_Waterfall
//   0x005A08D0   2230   0x005820B0 (same size)         MapGeneratorClass::Grow_Water_Region
//   0x005A23A0   1300   0x00583B80 (same size)         MapGeneratorClass::Create_Tiberium
//   0x005A89D0   1321   0x0058A1B0 (same size)         MapGeneratorClass::Generate_Rural_Roads
// The "RA2:" comments are the rebuild's notes on Red Alert 2's form, which these YR bodies share.
// Behaviour code for the native build only; it earns no exact-match credit.
// ---------------------------------------------------------------------------------------------
#include "findmake.h"

MapRegionClass *MapRegionClass::Create_Region_From_Cell(CellClass *cptr) // RA2: RA2's form (0x56E100): green-LAT ground counts as water-like and the wipes call Clear_Region_Cells; near miss, the exe calls TPoint2D::operator+ out of line
{
	int i;
	int region_id = MapRegionClass::TotalCount;

	DynamicVectorClass<Cell> cells;
	cells.Set_Growth_Step(50000);
	cells.Add(cptr->Fetch_CellID());

	bool has_water = cptr->Is_Tile_With_Water() || cptr->Is_Tile_Clear_To_Green_LAT();
	int cell_height = cptr->Height;
	int processed_count = 0;

	while (cells.Count()) {
		Cell cell = cells[cells.Count() - 1];
		cells.Delete_Index(cells.Count() - 1);

		CellData::Set_Spread(cell, region_id);
		CellData::Set_Region(cell, region_id);

		Cell ncell;
		for (i = 0; i < FACING_COUNT; i++) {
			ncell = Adjacent_Cell(cell, (FacingType)i);
			CellData & ndata = MapRegionClass::Get_Cell_Data(ncell);
			if (My_In_Radar(ncell)) {
				if (ndata.RegionID == -1 && ndata.SpreadID != region_id) {
					ndata.SpreadID = region_id;
					if (Map[ncell].Height == cell_height) {
						int n_has_water = Map[ncell].Is_Tile_With_Water() || Map[ncell].Is_Tile_Clear_To_Green_LAT();
						if (!(has_water ^ n_has_water)) {
							cells.Add(ncell);
						}
					}
				}
			}
		}

		processed_count++;
	}

	if (processed_count < 75 && !has_water) {
		bool reset_done = false;

		if (region_id == 0) {
			DynamicVectorClass<Cell> * region_cells = RandomMapGen.Build_Region_Border_Cell_List(0);
			Cell region_cell = (*region_cells)[Pick_Random_UInt(0, region_cells->Count() - 1)];

			Cell ncell;
			for (i = 0; i < FACING_COUNT; i++) {
				ncell = Adjacent_Cell(region_cell, (FacingType)i);
				if (My_In_Radar(ncell)) {
					if (Map[ncell].Height != Map[region_cell].Height && !Map[ncell].Is_Tile_With_Water()) {
						int height = Map[ncell].Height;

						RandomMapGen.Clear_Region_Cells(0, -1, height, ISOTILE_CLEAR);

						reset_done = true;
						break;
					}
				}
			}

			delete region_cells;
		} else {
			Cell seed = cptr->Fetch_CellID();
			Cell side_cell;
			side_cell = Cell(seed.X - 1, seed.Y);
			if (!My_In_Radar(side_cell)) {
				side_cell = Cell(seed.X, seed.Y - 1);
			}

			if (My_In_Radar(side_cell)) {
				int id = CellData::Get_Region(side_cell);
				int height = Map[side_cell].Height;

				RandomMapGen.Clear_Region_Cells(region_id, id, height, ISOTILE_CLEAR);

				reset_done = true;
			}
		}

		if (reset_done) {
			return(NULL);
		}
	}

	MapRegionClass * region = new MapRegionClass(cptr->Fetch_CellID());
	region->CellHeight = cell_height;
	region->ContainsWater = has_water;
	region->CellCount = 0;
	region->Recount_Cells();
	return(region);
}


// gamemd.exe 0x0058D620 (4,012 bytes, the size of RA2's 0x0056EF20). Main's draft of the RA2 twin
// (src/recovered/mapgen.cpp Split_Region_0056ef20, behaviour-checked) under the YR tree's names, with
// gamemd.exe's calls: the region's own Get_Angle_Score (0x0058C6F0), RMGGaussian scaled by 22.5
// degrees, Build_Border_Cell_List2 (0x0058E5D0), From_ID inline, the inline height change, and on the
// inland and mountainous maps (MapType 3, 4: 0x0058E2F9) no new height 0.
bool MapRegionClass::Split_Region(void)
{
	int i, j;

	if (Unsplittable) {
		return(false);
	}

	int heap_size = (2 * CellCount) + 10;
	CellNode * nodes = new CellNode[heap_size];
	PriorityQueueClass<CellNode> * queue = new PriorityQueueClass<CellNode>((2 * CellCount) + 10);
	queue->Clear();

	CellData::Reset_Spreads(false);

	int spread_count = 0;
	for (i = Cells.Count() - 1; i >= 0; i--) {
		MapRegionClass::Get_Cell_Data(Cells[i]).RegionID = -2;
	}

	unsigned int target_count = Pick_Random_UInt(CellCount / 8, CellCount / 3);
	unsigned int seed_index = Pick_Random_UInt(0, Cells.Count() - 1);

	int node_count = 1;
	Cell seed = Cells[seed_index];
	nodes[0].Element = seed;
	nodes[0].Score = 0.0f;
	MapRegionClass::Get_Cell_Data(seed).SpreadID = -3;
	queue->Insert(nodes[0]);

	CellNode * node = queue->Extract_Min();
	double dist = (unsigned int)RMGRandom() * (6.283185307179586 / 4294967295.0);

	while (node != NULL) {
		if (spread_count >= (int)target_count) {
			break;
		}

		CellData::Set_Region(node->Element, -3);

		int cell_index = node->Element.X + MapCellStride * node->Element.Y;
		CellNode * newnode = &nodes[node_count];

		for (int dir = FACING_N; dir < FACING_COUNT; dir += 2) {
			Cell ncell = Adjacent_Cell(node->Element, (FacingType)dir);
			if (My_In_Radar(ncell)) {
				if (CellData::Get_Region(ncell) == -2 && CellData::Get_Spread(ncell) != -3 && Map[ncell].Is_Tile_Clear()) {
					newnode->Element = ncell;
					newnode->Score = (float)Get_Angle_Score(seed, ncell, dist);
					MapRegionClass::Get_Cell_Data(cell_index + AStarFacingToOffset[dir]).SpreadID = -3;
					node_count++;
					queue->Insert(*newnode++);
				}
			}
		}

		spread_count++;
		dist += RMGGaussian() * 0.39269908169872414;
		node = queue->Extract_Min();
	}

	CellData::Reset_Spreads(false);

	CellNode * remaining = queue->Extract_Min();
	DynamicVectorClass<Cell> ncells;

	while (remaining != NULL) {
		Cell c = remaining->Element;
		MapRegionClass::Get_Cell_Data(c).RegionID = -3;
		remaining = queue->Extract_Min();
	}

	delete[] nodes;
	delete queue;

	DynamicVectorClass<MapRegionClass *> new_regions;
	for (i = Cells.Count() - 1; i >= 0; i--) {
		Cell cell = Cells[i];
		int cell_index = cell.X + MapCellStride * cell.Y;
		if (MapRegionClass::Get_Cell_Data(cell_index).RegionID < -1) {
			int old_id = MapRegionClass::Get_Cell_Data(cell_index).RegionID;

			MapRegionClass * region = new MapRegionClass(cell);
			region->Cells.Add(cell);
			region->CellCount++;
			new_regions.Add(region);
			MapRegionClass::Get_Cell_Data(cell_index).RegionID = region->ID;

			DynamicVectorClass<Cell> open;
			open.Set_Growth_Step(100000);
			open.Add(cell);

			while (open.Count() != 0) {
				int index = open.Count() - 1;
				Cell c = open[index];
				open.Delete_Index(index);

				for (int m = 0; m < FACING_COUNT; m++) {
					Cell ncell = Adjacent_Cell(c, (FacingType)m);
					if (My_In_Radar(ncell)) {
						CellData & ndata = MapRegionClass::Get_Cell_Data(ncell);
						if (ndata.RegionID == old_id) {
							region->Cells.Add(ncell);
							region->CellCount++;
							ndata.RegionID = region->ID;
							open.Add(ncell);
						}
					}
				}
			}
		}
	}

	for (i = new_regions.Count() - 1; i >= 0; i--) {
		MapRegionClass * region = new_regions[i];
		if (region->Discardable) {
			continue;
		}

		DynamicVectorClass<Cell> * cells = region->Build_Border_Cell_List2();
		bool * adjacent = new bool[MapRegionClass::TotalCount];
		for (j = 0; j < MapRegionClass::TotalCount; j++) {
			adjacent[j] = false;
		}

		for (j = cells->Count() - 1; j >= 0; j--) {
			Cell c = (*cells)[j];
			for (int m = 0; m < FACING_COUNT; m++) {
				Cell ncell = Adjacent_Cell(c, (FacingType)m);
				if (My_In_Radar(ncell)) {
					int id = MapRegionClass::Get_Cell_Data(ncell).RegionID;
					if (id >= 0 && id != region->ID) {
						adjacent[id] = true;
					}
				}
			}
		}

		delete cells;

		MapRegionClass * merge_target = NULL;
		int min_height = -1;
		int max_height = -1;
		int merge_height = -1;
		int max_count = -1;

		for (j = 0; j < MapRegionClass::TotalCount; j++) {
			if (adjacent[j]) {
				MapRegionClass * other = MapRegionClass::From_ID(j);
				if (other == NULL) continue;	// NATIVE: gamemd.exe reads through the missing region

				if (!other->Discardable) {
					if (other->CellCount > max_count && !other->ContainsWater) {
						max_count = other->CellCount;
						merge_height = other->CellHeight;
						merge_target = other;
					}

					if (min_height == -1) {
						min_height = other->CellHeight;
						max_height = other->CellHeight;
					} else {
						if (other->CellHeight > max_height) {
							max_height = other->CellHeight;
						}
						if (other->CellHeight < min_height) {
							min_height = other->CellHeight;
						}
					}
				}
			}
		}

		DynamicVectorClass<int> heights;
		int diff = max_height - min_height;
		if (diff != 0) {
			if (diff != 4) {
				if (diff == 8) {
					heights.Add((max_height + min_height) / 2);
				}
			} else {
				heights.Add(max_height);
				heights.Add(min_height);
			}
		} else {
			if (min_height >= 4) {
				heights.Add(min_height - 4);
			}
			if (max_height <= 7) {
				heights.Add(max_height + 4);
			}
		}

		if (RandomMapGen.SeedData.MapType == 3 || RandomMapGen.SeedData.MapType == 4) {
			for (int h = heights.Count() - 1; h >= 0; h--) {
				if (heights[h] == 0) {
					heights.Delete_Index(h);
				}
			}
		}

		delete[] adjacent;

		if (region->CellCount > 100) {
			/*
			** NATIVE: with no height left to choose gamemd.exe picks past the list's end; the
			** region keeps its height here.
			*/
			if (heights.Count() > 0) {
				int old_height = region->CellHeight;
				unsigned int max_index = (unsigned int)heights.Count() - 1;
				unsigned int index = Pick_Random_UInt(0, max_index);

				region->CellHeight = heights[index];
				region->Increase_Height((char)(region->CellHeight - old_height));
			}
		} else {
			if (merge_target != NULL) {
				int old_height = region->CellHeight;
				region->CellHeight = merge_height;
				region->Increase_Height((char)(region->CellHeight - old_height));

				for (int n = region->Cells.Count() - 1; n >= 0; n--) {
					Cell c = region->Cells[n];
					merge_target->Cells.Add(c);
					merge_target->CellCount++;
					MapRegionClass::Get_Cell_Data(c).RegionID = merge_target->ID;
				}

				region->Discardable = true;
			}
		}
	}

	for (i = MapRegionClass::MapRegions.Count() - 1; i >= 0; i--) {
		MapRegionClass * region = MapRegionClass::MapRegions[i];
		if (region != this && region->Discardable) {
			delete region;
		}
	}

	SplitCount++;
	return(true);
}


bool MapRegionClass::Build_North_Ramp(Cell const &first,Cell const &second,int lowRegion) // RA2: RA2's form of the north ramp (0x573680), with the ramp height and shape tables of 0x7E0A64 / 0x7E0A78
{
	static int RampHeights[] = {3, 2, 1, 0, 0}; // RA2: the table at 0x7E0A64
	static int RampShapes[] = {10, 14, 14, 14, 6, 9, 13, 13, 13, 5}; // RA2: the table at 0x7E0A78
	Rect area(second.X-4,(first.Y<second.Y?first.Y:second.Y)-7,first.X-second.X+8,(first.Y>second.Y?first.Y:second.Y)-(first.Y<second.Y?first.Y:second.Y)+11);
	if(!Is_Ramp_Area_Clear(area,lowRegion)) {
		return(false);
	}
	int drift=second.Y-first.Y;
	int span=abs(drift);
	int width=first.X-second.X;
	if(span>width-5) {
		return(false);
	}
	int half=(width+1)/2+3;
	Flatten_Area(Rect(area.X,area.Y,half+2,second.Y-area.Y),lowRegion);
	Flatten_Area(Rect(first.X-half+2,area.Y,half+3,first.Y-area.Y),lowRegion);
	Flatten_Area(Rect(second.X,second.Y,half-2,area.Y+area.Height-second.Y-2));
	Flatten_Area(Rect(first.X-half+2,first.Y,half-1,area.Y+area.Height-first.Y-2));
	MapGeneratorClass::Place_Tile((IsometricTileType)(IsometricTileTypeClass::SlopeSetPieces + 4),second-Cell(0,3),lowRegion,CellHeight);
	MapGeneratorClass::Place_Tile((IsometricTileType)(IsometricTileTypeClass::SlopeSetPieces + 5),first-Cell(2,3),lowRegion,CellHeight);
	MapGeneratorClass::Place_Tile((IsometricTileType)(IsometricTileTypeClass::SlopeSetPieces + 6),first-Cell(1,1),lowRegion,CellHeight);
	Cell tail(0,0);
	if(drift!=0) {
		int pick=Pick_Random_UInt(0,1);
		Cell bias;
		bias.X=0;
		int down=drift>0;
		bias.Y=drift<0;
		if(pick==1) {
			tail+=Cell(span,-drift);
		} else {
			bias.X=(first-Cell(span,0)).X-second.X-5;
		}
		for(int forward=0;forward<span;forward++) {
			for(int row=0;row<5;row++) {
				short offset=down?-forward:forward;
				Cell position=Cell(second.X+3,second.Y)+Cell(forward,-row)+bias;
				position=Cell(position.X,position.Y+offset);
				CellClass &target=Map[position];
				short height=RampHeights[row]; // RA2: ra2-decomp src/mapgen.cpp keeps the height in a short
				target.Height=height + CellHeight - 4;
				target.Ramp=(unsigned char)RampShapes[5*down+row];
				target.ITType=(IsometricTileType)((unsigned)IsometricTileTypeClass::RampStart + target.Ramp - 1);
				target.SubTile=0;
			}
		}
	}
	for(int column=0;column<first.X-second.X-span-5;column++) {
		for(int row=0;row<4;row++) {
			Cell position=Cell(second.X+3,second.Y)+Cell(column,-row)+tail;
			CellData::Set_Region(position,lowRegion);
			CellClass &target=Map[position];
			target.Height=(char)(CellHeight-(char)row-1);
			target.Ramp=2;
			target.ITType=(IsometricTileType)((unsigned)IsometricTileTypeClass::RampStart + 1u);
			target.SubTile=0;
		}
	}
	return(true);
}


// gamemd.exe 0x00594B50 (998 bytes; RA2's 0x00576420 has 925): the rebuild's text of the RA2 twin with
// gamemd.exe's changes: amphibious zones for map types 0-2 (RA2's table), the zone regions made inline
// (the tree's Create_Zone_Regions, 0x00594700, is the same loop), the start count in Unknown20 (+0x20),
// and the start points copied into the scenario at the end (FUN_0058B820).
bool Generate_Starting_Points(void)
{
	MZoneType mzones[5] = {MZONE_AMPHIBIOUS, MZONE_AMPHIBIOUS, MZONE_AMPHIBIOUS, MZONE_NORMAL, MZONE_NORMAL};
	int region_size_threshold = MAX(400.0, (double)RandomMapGen.LocalHeight * RandomMapGen.LocalWidth * 0.03);
	int index = Map.Zone_Reset();
	MZoneType mzone = mzones[RandomMapGen.SeedData.MapType];
	int zone = Map.Zones[mzone][index];
	Create_Zone_Regions(mzone, zone);

	int region_count = MapRegionClass::MapRegions.Count();
	int i;
	for (i = MapRegionClass::MapRegions.Count() - 1; i >= 0; i--) {
		if (MapRegionClass::MapRegions[i]->CellCount < region_size_threshold) {
			delete MapRegionClass::MapRegions[i];
		}
	}

	DynamicVectorClass<MapRegionClass *> regions;
	IndexClass<int, MapRegionClass *> sorter;
	regions.Clear();

	/*
	** The biggest regions sort first, and regions of a size by their id.
	*/
	for (i = 0; i < MapRegionClass::MapRegions.Count(); i++) {
		MapRegionClass * region = MapRegionClass::MapRegions[i];
		sorter.Add_Index((500000 - region->CellCount) * region_count + region->ID, MapRegionClass::MapRegions[i]);
	}
	for (i = 0; i < sorter.Count(); i++) {
		regions.Add(sorter.Fetch_By_Position(i));
	}

	int total_cells = 0;
	for (i = 0; i < regions.Count(); i++) {
		total_cells += regions[i]->CellCount;
	}

	int players = RandomMapGen.SeedData.NumPlayers;
	int cells_so_far = 0;
	int players_so_far = 0;
	for (i = 0; i < regions.Count(); i++) {
		MapRegionClass * region = regions[i];
		cells_so_far += region->CellCount;
		int count;
		if (i == regions.Count() - 1) {
			count = players - players_so_far;
		} else {
			count = (double)cells_so_far / total_cells * players + 0.5 - players_so_far;
		}
		region->Unknown20 = count;
		players_so_far += count;
	}

	int waypoint = 0;
	for (i = 0; i < regions.Count(); i++) {
		regions[i]->Pick_Start_Points(waypoint);
		waypoint += regions[i]->Unknown20;
	}

	if (RandomMapGen.SeedData.MapType == 0) {
		for (i = 0; i < regions.Count(); i++) {
			if (regions[i]->Unknown20 == 0 && (unsigned int)RMGRandom() * (1.0 / 4294967295.0) < 0.5) {
				regions[i]->Place_Neutral_Building();
			}
		}
	}

	FUN_0058B820();

	return(true);
}


// gamemd.exe 0x005981F0 (1,889 bytes; RA2's 0x00579960 has 1,897): the rebuild's text of the RA2 twin; YR reads
// RMGMD.INI, the same keys in the same order.
void MapGeneratorClass::Load_Settings(void)
{
	static char const * section = "General"; // RA2: the section name is a static pointer (0x7E0BE4), read for every key
	CCFileClass file("RMGMD.INI");
	CCINIClass * ini = new CCINIClass;
	ini->Load(file, false);

	RMGLevelLightSettings.Clear();
	RMGVegetationMinimums.Clear();
	RMGVegetationMaximums.Clear();
	TemperateOrePatchLamps.Clear();
	SnowOrePatchLamps.Clear();
	TemperateAmbientLight.Clear();
	SnowAmbientLight.Clear();

	RMGMinimumTiberium = ini->Get_Int(section, "RMGMinimumTiberium", RMGMinimumTiberium);
	RMGMaximumTiberium = ini->Get_Int(section, "RMGMaximumTiberium", RMGMaximumTiberium);
	RMGLevelLightSettings = ini->Get_IntList(section, "RMGLevelLightSettings", RMGLevelLightSettings);
	RMGVegetationMinimums = ini->Get_IntList(section, "RMGVegetationMinimums", RMGVegetationMinimums);
	RMGVegetationMaximums = ini->Get_IntList(section, "RMGVegetationMaximums", RMGVegetationMaximums);
	TemperateAmbientLight = ini->Get_IntList(section, "TemperateAmbientLight", TemperateAmbientLight);
	SnowAmbientLight = ini->Get_IntList(section, "SnowAmbientLight", SnowAmbientLight);
	TemperateAmbientRed = ini->Get_IntList(section, "TemperateAmbientRed", TemperateAmbientRed);
	TemperateAmbientGreen = ini->Get_IntList(section, "TemperateAmbientGreen", TemperateAmbientGreen);
	TemperateAmbientBlue = ini->Get_IntList(section, "TemperateAmbientBlue", TemperateAmbientBlue);
	SnowAmbientRed = ini->Get_IntList(section, "SnowAmbientRed", SnowAmbientRed);
	SnowAmbientGreen = ini->Get_IntList(section, "SnowAmbientGreen", SnowAmbientGreen);
	SnowAmbientBlue = ini->Get_IntList(section, "SnowAmbientBlue", SnowAmbientBlue);
	MaxTrees = ini->Get_Int(section, "MaxTrees", MaxTrees);

	TemperateOrePatchLamps = TGet_TypeList<BuildingTypeClass>(*ini, section, "TemperateOrePatchLamps", TemperateOrePatchLamps); // RA2: the lamp lists are read with the generic type-list reader (a 128 byte buffer inside), not by hand
	SnowOrePatchLamps = TGet_TypeList<BuildingTypeClass>(*ini, section, "SnowOrePatchLamps", SnowOrePatchLamps);

	delete ini;
	file.Close();
}


/*
** A new preview between the generator's stages (gamemd.exe repeats this block inline): drawn, the
** dialog repainted, and in a Westwood Online tour game sent to the guests.
*/
static void RMG_Refresh_Preview(bool full_init, HWND dialog)
{
	if (full_init) {
		RandomMapGen.MapPreview->Create_Preview();
		SendMessage(dialog, WM_PAINT, 0, 0);
		if (Session.Type == GAME_INTERNET && Session.unknown_0C == 3) {
			FUN_005E7EB0(RandomMapGen.MapPreview);
		}
	}
}

static void RMG_Recalc_Cells(void)
{
	DebugString("RMG: Recalculating cell attributes\n");
	Map.Reset_Iterator();
	for (CellClass * cptr = Map.Iterate(); cptr != NULL; cptr = Map.Iterate()) {
		cptr->Recalc_Attributes(-1);
	}
}

static void RMG_Delete_Regions(void)
{
	for (int index = MapRegionClass::MapRegions.Count() - 1; index >= 0; index--) {
		MapRegionClass * region = MapRegionClass::MapRegions[index];
		if (region != NULL) {
			delete region;
		}
	}
	MapRegionClass::TotalCount = 0;
}

// gamemd.exe 0x00598960 (3,018 bytes; RA2's 0x0057A0D0 has 3,086). The rebuild's text of the RA2 twin
// in gamemd.exe's order: Init_Regions inline (the tree's Init_Regions, 0x005A1C70, is the same code),
// Load_Settings (RMGMD.INI) after the first preview, Fixup_Cliff_Edges after the cliffs, the start
// points (0x00594B50), Create_Tiberium, and on the snow maps the rough and sand chances of
// Seed_Arctic_Vegetation before Generate_Arctic_Vegetation. The tour check is Session +0x0C == 3.
void MapGeneratorClass::Generate_Random_Map(bool full_init, HWND dialog)
{
	if (RMGCallback != NULL) RMGCallback();

	RMGRandom = Random2Class(SeedData.Seed);
	RMGGaussian = NormalDistribution(Random_Fraction);

	if (RMGCallback != NULL) RMGCallback();

	bool show_progress = !Scen->IsReadingScenario;
	if (show_progress) {
		Progress.Initialize(100, 1, dialog);
		Progress.Set_Graphic_Data("PROGBAR2.SHP", NULL, 0);
		Progress.Set_Layout(Point2D(-1, -1), NULL, false, true);
		Progress.Display_Progress();
		HWND control = GetDlgItem(dialog, 0x638);
		if (control != NULL) {
			ShowWindow(control, SW_SHOW);
		}
	}

	if (RMGCallback != NULL) RMGCallback();

	DebugString("RMG: Init random map\n");

	Init_Map(full_init);

	if (RMGCallback != NULL) RMGCallback();

	ScenarioInit++;

	RMG_Refresh_Preview(full_init, dialog);

	if (RMGCallback != NULL) RMGCallback();

	Load_Settings();

	DebugString("RMG: Seeding water\n");

	if (SeedData.MapType == 3 || SeedData.MapType == 4) {
		if (SeedData.WaterAmount != 0) {
			Seed_Water();
		}
	} else {
		Seed_Land();
	}

	Vary_Water_Tiles();

	Update_Progress(55);

	if (RMGCallback != NULL) RMGCallback();

	RMG_Refresh_Preview(full_init, dialog);

	if (RMGCallback != NULL) RMGCallback();

	Update_Progress(60);

	if (RMGCallback != NULL) RMGCallback();

	RMG_Refresh_Preview(full_init, dialog);

	if (RMGCallback != NULL) RMGCallback();

	DebugString("RMG: Init regions\n");

	Init_Regions();

	Update_Progress(65);

	if (RMGCallback != NULL) RMGCallback();

	DebugString("RMG: Making regions\n");

	if (SeedData.MapType == 4 || SeedData.MapType == 3) {
		MapRegionClass::Make_Ice_Regions();
		MapRegionClass::Make_Regions();
		Fixup_Cliff_Corners();
		Map.Build_All_Cliffs(0, -1);
		Fixup_Cliff_Edges();
	}

	Update_Progress(70);

	if (RMGCallback != NULL) RMGCallback();

	RMG_Refresh_Preview(full_init, dialog);

	if (RMGCallback != NULL) RMGCallback();

	Scatter_Green_Tiles();

	RMG_Recalc_Cells();

	if (RMGCallback != NULL) RMGCallback();

	Update_Progress(75);

	DebugString("RMG: Creating starting points\n");

	do {
		while (!Generate_Starting_Points()) {}
	} while (!Init_Start_Points());

	DebugString("RMG: Adding tech buildings\n");
	if (SeedData.MapType != 0) {
		Generate_Neutral_Buildings();
	}

	if (RMGCallback != NULL) RMGCallback();

	DebugString("RMG: Creating tiberium\n");
	Create_Tiberium();

	if (RMGCallback != NULL) RMGCallback();

	if (RMGCellData != NULL) {
		int size = MapCellStride * MapCellStride;
		for (int index = 0; index < size; index++) {
			RMGCellData[index].RegionID = -1;
			RMGCellData[index].SpreadID = -1;
		}
	}

	RMG_Delete_Regions();

	RMG_Recalc_Cells();

	Update_Progress(80);

	if (RMGCallback != NULL) RMGCallback();

	RMG_Refresh_Preview(full_init, dialog);

	if (RMGCallback != NULL) RMGCallback();

	Update_Progress(85);

	if (RMGCallback != NULL) RMGCallback();

	RMG_Refresh_Preview(full_init, dialog);

	if (RMGCallback != NULL) RMGCallback();

	RMG_Recalc_Cells();

	if (RMGCallback != NULL) RMGCallback();

	DebugString("RMG: Creating hills\n");
	Generate_Hills();

	Update_Progress(90);

	if (RMGCallback != NULL) RMGCallback();

	RMG_Refresh_Preview(full_init, dialog);

	if (RMGCallback != NULL) RMGCallback();

	DebugString("RMG: Creating LATs, rocks etc\n");

	if (SeedData.Biome == 0) {
		Seed_Vegetation();
		Generate_Vegetation();
	} else {
		Seed_Arctic_Vegetation();
		Generate_Arctic_Vegetation();
	}

	if (RMGCallback != NULL) RMGCallback();

	Update_Progress(95);

	if (RMGCallback != NULL) RMGCallback();

	RMG_Refresh_Preview(full_init, dialog);

	ScenarioInit--;

	if (RMGCallback != NULL) RMGCallback();

	RMG_Recalc_Cells();

	if (RMGCallback != NULL) RMGCallback();

	TiberiumClass::Init_Tiberium_Growth_System();
	TiberiumClass::Init_Tiberium_Spread_System();

	DebugString("RMG: Cleanup\n");

	SeededWaterAmount = 0;
	WorkingRegionID = 0;

	if (RMGCellData != NULL) {
		delete [] RMGCellData;
		RMGCellData = NULL;
	}

	RMG_Delete_Regions();

	Map.Overpass();
	DebugString("RMG: Compute Radar Image\n");
	Map.Set_Local_Dimensions(Map.LocalRect);
	Map.Compute_Radar_Image();

	Update_Progress(100);

	DebugString("RMG: Done\n");

	if (show_progress) {
		HWND control = GetDlgItem(dialog, 0x638);
		if (control != NULL) {
			ShowWindow(control, SW_HIDE);
		}
		Update_Progress(0);
		Progress.Clear_Dialog();
	}
}


bool MapGeneratorClass::Place_Waterfall(int region_id, Cell const & cell1, Cell const & cell2, int direction, bool & placed, double & head_x, double & head_y) // RA2: RA2's form (0x57FF20): the areas are rectangles; near miss, 2 bytes differ (the south case adds the base to the offset, the exe adds the offset to the base)
{
	placed = false; // RA2: RA2's form

	Rect pre(0,0,0,0); // RA2: RA2's form

	Rect seed(0,0,0,0); // RA2: RA2's form
	Rect patch(0,0,0,0); // RA2: RA2's form
	Rect bounds_rect(0,0,0,0); // RA2: RA2's form
	Cell center; // RA2: left uninitialized

	switch (direction) { // RA2: RA2's form
		case FACING_N: { // RA2: RA2's form
			pre=Rect(cell1.X - 2,cell1.Y - 12,cell2.X - cell1.X + 5,12); // RA2: RA2's form

			seed=Rect(cell1.X,cell1.Y - 4,cell2.X - cell1.X + 1,4); // RA2: RA2's form

			patch=Rect(cell1.X,cell1.Y - 12,cell2.X - cell1.X + 1,8); // RA2: RA2's form

			bounds_rect = Rect(0, seed.Y, MAP_CELL_W, MAP_CELL_H - seed.Y); // RA2: RA2's form
			center = Cell(seed.X, seed.Y); // RA2: RA2's form
			break; // RA2: RA2's form
		}
		case FACING_S: { // RA2: RA2's form
			pre=Rect(cell2.X - 2,cell2.Y + 1,cell1.X - cell2.X + 5,12); // RA2: RA2's form

			seed=Rect(cell2.X,cell2.Y + 1,cell1.X - cell2.X + 1,4); // RA2: RA2's form

			patch=Rect(cell2.X,cell2.Y + 5,cell1.X - cell2.X + 1,8); // RA2: RA2's form

			bounds_rect = Rect(0, 0, MAP_CELL_W, patch.Y - 1); // RA2: RA2's form
			center = Cell(seed.X, seed.Y); // RA2: RA2's form
			break; // RA2: RA2's form
		}
		case FACING_E: { // RA2: RA2's form
			pre=Rect(cell1.X + 1,cell1.Y - 2,12,cell2.Y - cell1.Y + 5); // RA2: RA2's form

			seed=Rect(cell1.X + 1,cell1.Y,4,cell2.Y - cell1.Y + 1); // RA2: RA2's form

			patch=Rect(cell1.X + 5,cell1.Y,8,cell2.Y - cell1.Y + 1); // RA2: RA2's form

			bounds_rect = Rect(0, 0, patch.X - 1, MAP_CELL_H); // RA2: RA2's form
			center = Cell(patch.X, patch.Y); // RA2: RA2's form
			break; // RA2: RA2's form
		}
		case FACING_W: { // RA2: RA2's form
			pre=Rect(cell2.X - 12,cell2.Y - 2,12,cell1.Y - cell2.Y + 5); // RA2: RA2's form

			seed=Rect(cell2.X - 4,cell2.Y,4,cell1.Y - cell2.Y + 1); // RA2: RA2's form

			patch=Rect(cell2.X - 12,cell2.Y,8,cell1.Y - cell2.Y + 1); // RA2: RA2's form

			bounds_rect = Rect(seed.X, 0, MAP_CELL_W - seed.X, MAP_CELL_H); // RA2: RA2's form
			center = Cell(seed.X, seed.Y); // RA2: RA2's form
			break; // RA2: RA2's form
		}
		default: // RA2: RA2's form
			break; // RA2: RA2's form
	}

	int yy; // RA2: RA2's form
	int xx; // RA2: RA2's form

	for (yy = pre.Y; yy < pre.Y + pre.Height; yy++) { // RA2: RA2's form
		for (xx = pre.X; xx < pre.X + pre.Width; xx++) { // RA2: RA2's form
			if (My_In_Radar(Cell(xx, yy))) { // RA2: RA2's form
				if (MapRegionClass::CellData::Get_Region(Cell(xx, yy)) != 0) { // RA2: RA2's form
					return(true); // RA2: RA2's form
				}

				if (!Map[Cell(xx, yy)].Is_Tile_Clear()) { // RA2: RA2's form
					return(true); // RA2: RA2's form
				}
			}
		}
	}

	for (yy = seed.Y; yy < seed.Y + seed.Height; yy++) { // RA2: RA2's form
		for (xx = seed.X; xx < seed.X + seed.Width; xx++) { // RA2: RA2's form
			if (My_In_Radar(Cell(xx, yy))) { // RA2: RA2's form
				int tile_index = Pick_Random_UInt(0, 5); // RA2: RA2's form
				Map[Cell(xx, yy)].ITType = (IsometricTileType)(IsometricTileTypeClass::WaterSet + tile_index); // RA2: RA2's form

				int subtile = Pick_Random_UInt(0, 3); // RA2: RA2's form
				Map[Cell(xx, yy)].SubTile = subtile; // RA2: RA2's form

				if (RMGCellData != NULL) { // RA2: RA2's form
								MapRegionClass::Get_Cell_Data(Cell(xx, yy)).RegionID=region_id; // RA2: RA2's form
							}
			}
		}
	}

	bool success = Grow_Water_Region(WorkingRegionID, 0.003f, bounds_rect, center, false); // RA2: RA2's form
	if (success) { // RA2: RA2's form
		success = Map.FUN_0057A0C0(WorkingRegionID, false); // RA2: RA2's form
		if (success) { // RA2: RA2's form
			success = Grow_Region(WorkingRegionID, 2, bounds_rect, 0, 0); // RA2: RA2's form
			if (success) { // RA2: RA2's form
				int region = WorkingRegionID; // RA2: RA2's form
				Map.Reset_Iterator(); // RA2: RA2's form
				CellClass * cptr = Map.Iterate(); // RA2: RA2's form
				while (cptr != NULL) { // RA2: RA2's form
					Cell c = cptr->CellID; // RA2: RA2's form
					int other_region = MapRegionClass::CellData::Get_Region(c); // RA2: RA2's form
					if (other_region == region) { // RA2: RA2's form
						cptr->Height += 4; // RA2: RA2's form
					}
					cptr = Map.Iterate(); // RA2: RA2's form
				}

				for (yy = patch.Y; yy < patch.Y + patch.Height; yy++) { // RA2: RA2's form
					for (xx = patch.X; xx < patch.X + patch.Width; xx++) { // RA2: RA2's form
						if (My_In_Radar(Cell(xx, yy))) { // RA2: RA2's form
							int tile_index = Pick_Random_UInt(0, 5); // RA2: RA2's form
							Map[Cell(xx, yy)].ITType = (IsometricTileType)(IsometricTileTypeClass::WaterSet + tile_index); // RA2: RA2's form

							int subtile = Pick_Random_UInt(0, 3); // RA2: RA2's form
							Map[Cell(xx, yy)].SubTile = subtile; // RA2: RA2's form

							if (RMGCellData != NULL) { // RA2: RA2's form
								MapRegionClass::Get_Cell_Data(Cell(xx, yy)).RegionID=region_id; // RA2: RA2's form
							}
						}
					}
				}

				bool place_ok = true; // RA2: RA2's form
				
				switch (direction) { // RA2: RA2's form
					case FACING_E: { // RA2: RA2's form
						Cell base(patch.X, patch.Y + patch.Height); // RA2: RA2's form
						int height = Map[base + Cell(2, 0)].Height; // RA2: RA2's form

						Map.PendingObject = IsometricTileTypes[IsometricTileTypeClass::WaterfallEast]; // RA2: RA2's form
						Map.Set_Cursor_Pos(base); // RA2: RA2's form
						Map.Pick_Random_Tile_Variant(ISOTILE_CLEAR, ISOTILE_NONE + 1, height, WorkingRegionID, place_ok, false); // RA2: RA2's form

						Map.PendingObject = IsometricTileTypes[IsometricTileTypeClass::WaterfallEast + WATERFALL_EAST_COUNT - 1]; // RA2: RA2's form
						Map.Set_Cursor_Pos(base - Cell(0, patch.Height + 2)); // RA2: RA2's form
						Map.Pick_Random_Tile_Variant(ISOTILE_CLEAR, ISOTILE_NONE + 1, height, WorkingRegionID, place_ok, false); // RA2: RA2's form

						Cell top = base + Cell(0, 2); // RA2: RA2's form
						Cell bottom = base - Cell(0, patch.Height + 3); // RA2: RA2's form

						if (My_In_Radar(top)) { // RA2: RA2's form
							Map[top].ITType = ISOTILE_NONE; // RA2: RA2's form
							Map[top].Height += 4; // RA2: RA2's form
							Map[top].SubTile = 0; // RA2: RA2's form
							if (RMGCellData != NULL) { // RA2: RA2's form
								MapRegionClass::Get_Cell_Data(top).RegionID=WorkingRegionID; // RA2: RA2's form
							}
						}

						if (My_In_Radar(bottom)) { // RA2: RA2's form
							Map[bottom].ITType = ISOTILE_NONE; // RA2: RA2's form
							Map[bottom].Height += 4; // RA2: RA2's form
							Map[bottom].SubTile = 0; // RA2: RA2's form
							if (RMGCellData != NULL) { // RA2: RA2's form
								MapRegionClass::Get_Cell_Data(bottom).RegionID=WorkingRegionID; // RA2: RA2's form
							}
						}

						int seg = 0; // RA2: RA2's form
						if (seg < patch.Height) { // RA2: RA2's form
						int remaining = patch.Height; // RA2: RA2's form
						do { // RA2: RA2's form
							if (remaining % 2) { // RA2: RA2's form
								Map.Set_Cursor_Pos(base - Cell(0, seg + 1)); // RA2: RA2's form
								Map.PendingObject = IsometricTileTypes[IsometricTileTypeClass::WaterfallEast + 1]; // RA2: RA2's form
								seg++; // RA2: RA2's form
								remaining--; // RA2: RA2's form
							} else {
								Map.Set_Cursor_Pos(base - Cell(0, seg + 2)); // RA2: RA2's form
								Map.PendingObject = IsometricTileTypes[IsometricTileTypeClass::WaterfallEast + 2]; // RA2: RA2's form
								seg += 2; // RA2: RA2's form
								remaining -= 2; // RA2: RA2's form
							}
							Map.Pick_Random_Tile_Variant(ISOTILE_CLEAR, ISOTILE_NONE + 1, height, WorkingRegionID, place_ok, false); // RA2: RA2's form
						} while (seg < patch.Height);
						}
						break; // RA2: RA2's form
					}

					case FACING_W: { // RA2: RA2's form
						Cell base(seed.X - 2, patch.Y + patch.Height); // RA2: RA2's form
						int height = Map[base - Cell(2, 0)].Height; // RA2: RA2's form

						Map.PendingObject = IsometricTileTypes[IsometricTileTypeClass::WaterfallWest]; // RA2: RA2's form
						Map.Set_Cursor_Pos(base); // RA2: RA2's form
						Map.Pick_Random_Tile_Variant(ISOTILE_CLEAR, ISOTILE_NONE + 1, height, WorkingRegionID, place_ok, false); // RA2: RA2's form

						Map.PendingObject = IsometricTileTypes[IsometricTileTypeClass::WaterfallWest + WATERFALL_WEST_COUNT - 1]; // RA2: RA2's form
						Map.Set_Cursor_Pos(base - Cell(0, patch.Height + 2)); // RA2: RA2's form
						Map.Pick_Random_Tile_Variant(ISOTILE_CLEAR, ISOTILE_NONE + 1, height, WorkingRegionID, place_ok, false); // RA2: RA2's form

						Map[base - Cell(0, 1)].ITType = ISOTILE_NONE; // RA2: RA2's form
						Map[base + Cell(0, patch.Width + 2)].ITType = ISOTILE_NONE; // RA2: RA2's form

						for (int i = 0; i < patch.Height + 2; i++) { // RA2: RA2's form
							Map[base - Cell(0, i)].Height -= 4; // RA2: RA2's form
						}

						Cell top = base + Cell(1, 2); // RA2: RA2's form
						Cell bottom = base - Cell(-1, patch.Height + 3); // RA2: RA2's form

						if (My_In_Radar(top)) { // RA2: RA2's form
							Map[top].ITType = ISOTILE_NONE; // RA2: RA2's form
							Map[top].Height += 4; // RA2: RA2's form
							Map[top].SubTile = 0; // RA2: RA2's form
							if (RMGCellData != NULL) { // RA2: RA2's form
								MapRegionClass::Get_Cell_Data(top).RegionID=WorkingRegionID; // RA2: RA2's form
							}
						}

						if (My_In_Radar(bottom)) { // RA2: RA2's form
							Map[bottom].ITType = ISOTILE_NONE; // RA2: RA2's form
							Map[bottom].Height += 4; // RA2: RA2's form
							Map[bottom].SubTile = 0; // RA2: RA2's form
							if (RMGCellData != NULL) { // RA2: RA2's form
								MapRegionClass::Get_Cell_Data(bottom).RegionID=WorkingRegionID; // RA2: RA2's form
							}
						}

						int seg = 0; // RA2: RA2's form
						if (seg < patch.Height) { // RA2: RA2's form
						int remaining = patch.Height; // RA2: RA2's form
						do { // RA2: RA2's form
							if (remaining % 2) { // RA2: RA2's form
								Map.Set_Cursor_Pos(base - Cell(-1, seg + 1)); // RA2: RA2's form
								Map.PendingObject = IsometricTileTypes[IsometricTileTypeClass::WaterfallWest + 1]; // RA2: RA2's form
								seg++; // RA2: RA2's form
								remaining--; // RA2: RA2's form
							} else {
								Map.Set_Cursor_Pos(base - Cell(-1, seg + 2)); // RA2: RA2's form
								Map.PendingObject = IsometricTileTypes[IsometricTileTypeClass::WaterfallWest + 2]; // RA2: RA2's form
								seg += 2; // RA2: RA2's form
								remaining -= 2; // RA2: RA2's form
							}
							Map.Pick_Random_Tile_Variant(ISOTILE_CLEAR, ISOTILE_NONE + 1, height, WorkingRegionID, place_ok, false); // RA2: RA2's form
						} while (seg < patch.Height);
						}
						break; // RA2: RA2's form
					}

					case FACING_S: { // RA2: RA2's form
						Cell base(patch.X - 2, patch.Y); // RA2: RA2's form
						int height = Map[base + Cell(0, 2)].Height; // RA2: RA2's form

						Map.PendingObject = IsometricTileTypes[IsometricTileTypeClass::WaterfallSouth]; // RA2: RA2's form
						Map.Set_Cursor_Pos(base); // RA2: RA2's form
						Map.Pick_Random_Tile_Variant(ISOTILE_CLEAR, ISOTILE_NONE + 1, height, WorkingRegionID, place_ok, false); // RA2: RA2's form

						Map.PendingObject = IsometricTileTypes[IsometricTileTypeClass::WaterfallSouth + WATERFALL_SOUTH_COUNT - 1]; // RA2: RA2's form
						Map.Set_Cursor_Pos(base + Cell(patch.Width + 2, 0)); // RA2: RA2's form
						Map.Pick_Random_Tile_Variant(ISOTILE_CLEAR, ISOTILE_NONE + 1, height, WorkingRegionID, place_ok, false); // RA2: RA2's form

						Cell left = base + Cell(-1, 0); // RA2: RA2's form
						Cell right = base + Cell(patch.Width + 4, 0); // RA2: RA2's form

						if (My_In_Radar(left)) { // RA2: RA2's form
							Map[left].ITType = ISOTILE_NONE; // RA2: RA2's form
							Map[left].Height += 4; // RA2: RA2's form
							Map[left].SubTile = 0; // RA2: RA2's form
							if (RMGCellData != NULL) { // RA2: RA2's form
								MapRegionClass::Get_Cell_Data(left).RegionID=WorkingRegionID; // RA2: RA2's form
							}
						}

						if (My_In_Radar(right)) { // RA2: RA2's form
							Map[right].ITType = ISOTILE_NONE; // RA2: RA2's form
							Map[right].Height += 4; // RA2: RA2's form
							Map[right].SubTile = 0; // RA2: RA2's form
							if (RMGCellData != NULL) { // RA2: RA2's form
								MapRegionClass::Get_Cell_Data(right).RegionID=WorkingRegionID; // RA2: RA2's form
							}
						}

						int seg = 0; // RA2: RA2's form
						if (seg < patch.Width) { // RA2: RA2's form
						int remaining = patch.Width; // RA2: RA2's form
						do { // RA2: RA2's form
							if (remaining % 2) { // RA2: RA2's form
								Map.Set_Cursor_Pos(base + Cell(seg + 2, 0)); // RA2: RA2's form
								Map.PendingObject = IsometricTileTypes[IsometricTileTypeClass::WaterfallSouth + 1]; // RA2: RA2's form
								seg++; // RA2: RA2's form
								remaining--; // RA2: RA2's form
							} else {
								Map.Set_Cursor_Pos(base + Cell(seg + 2, 0)); // RA2: RA2's form
								Map.PendingObject = IsometricTileTypes[IsometricTileTypeClass::WaterfallSouth + 2]; // RA2: RA2's form
								seg += 2; // RA2: RA2's form
								remaining -= 2; // RA2: RA2's form
							}
							Map.Pick_Random_Tile_Variant(ISOTILE_CLEAR, ISOTILE_NONE + 1, height, WorkingRegionID, place_ok, false); // RA2: RA2's form
						} while (seg < patch.Width);
						}
						break; // RA2: RA2's form
					}

					case FACING_N: { // RA2: RA2's form
						Cell base(seed.X - 2, seed.Y - 2); // RA2: RA2's form
						int height = Map[base - Cell(0, 2)].Height; // RA2: RA2's form

						Map.PendingObject = IsometricTileTypes[IsometricTileTypeClass::WaterfallNorth]; // RA2: RA2's form
						Map.Set_Cursor_Pos(base); // RA2: RA2's form
						Map.Pick_Random_Tile_Variant(ISOTILE_CLEAR, ISOTILE_NONE + 1, height, WorkingRegionID, place_ok, false); // RA2: RA2's form

						Map.PendingObject = IsometricTileTypes[IsometricTileTypeClass::WaterfallNorth + WATERFALL_NORTH_COUNT - 1]; // RA2: RA2's form
						Map.Set_Cursor_Pos(base + Cell(patch.Width + 2, 0)); // RA2: RA2's form
						Map.Pick_Random_Tile_Variant(ISOTILE_CLEAR, ISOTILE_NONE + 1, height, WorkingRegionID, place_ok, false); // RA2: RA2's form

						Map[base + Cell(1, 0)].ITType = ISOTILE_NONE; // RA2: RA2's form
						Map[base + Cell(patch.Width + 2, 0)].ITType = ISOTILE_NONE; // RA2: RA2's form

						for (int i = 0; i < patch.Width + 2; i++) { // RA2: RA2's form
							Map[base + Cell(i + 1, 0)].Height -= 4; // RA2: RA2's form
						}

						Cell left = base + Cell(-1, 1); // RA2: RA2's form
						Cell right = base + Cell(patch.Width + 4, 1); // RA2: RA2's form

						if (My_In_Radar(left)) { // RA2: RA2's form
							Map[left].ITType = ISOTILE_NONE; // RA2: RA2's form
							Map[left].Height += 4; // RA2: RA2's form
							Map[left].SubTile = 0; // RA2: RA2's form
							if (RMGCellData != NULL) { // RA2: RA2's form
								MapRegionClass::Get_Cell_Data(left).RegionID=WorkingRegionID; // RA2: RA2's form
							}
						}

						if (My_In_Radar(right)) { // RA2: RA2's form
							Map[right].ITType = ISOTILE_NONE; // RA2: RA2's form
							Map[right].Height += 4; // RA2: RA2's form
							Map[right].SubTile = 0; // RA2: RA2's form
							if (RMGCellData != NULL) { // RA2: RA2's form
								MapRegionClass::Get_Cell_Data(right).RegionID=WorkingRegionID; // RA2: RA2's form
							}
						}

						int seg = 0; // RA2: RA2's form
						if (seg < patch.Width) { // RA2: RA2's form
						int remaining = patch.Width; // RA2: RA2's form
						do { // RA2: RA2's form
							if (remaining % 2) { // RA2: RA2's form
								Map.Set_Cursor_Pos(base + Cell(seg + 2, 1)); // RA2: RA2's form
								Map.PendingObject = IsometricTileTypes[IsometricTileTypeClass::WaterfallNorth + 1]; // RA2: RA2's form
								seg++; // RA2: RA2's form
								remaining--; // RA2: RA2's form
							} else {
								Map.Set_Cursor_Pos(base + Cell(seg + 2, 1)); // RA2: RA2's form
								Map.PendingObject = IsometricTileTypes[IsometricTileTypeClass::WaterfallNorth + 2]; // RA2: RA2's form
								seg += 2; // RA2: RA2's form
								remaining -= 2; // RA2: RA2's form
							}
							Map.Pick_Random_Tile_Variant(ISOTILE_CLEAR, ISOTILE_NONE + 1, height, WorkingRegionID, place_ok, false); // RA2: RA2's form
						} while (seg < patch.Width);
						}
						break; // RA2: RA2's form
					}

					default: // RA2: RA2's form
						break; // RA2: RA2's form
				}
			}
		}
	}

	int x_offsets[4] = { 0, 12, 0, -12 }; // RA2: RA2's form
	int y_offsets[4] = { -12, 0, 12, 0 }; // RA2: RA2's form

	if (success) { // RA2: RA2's form
		head_x += (double)x_offsets[direction / 2]; // RA2: RA2's form
		head_y += (double)y_offsets[direction / 2]; // RA2: RA2's form
	}

	placed = success; // RA2: RA2's form
	return(success); // RA2: RA2's form
}


bool MapGeneratorClass::Grow_Water_Region(int region_id, float spread_scale, Rect const & bounds, Cell const & origin, bool claim_frontier)
{
	CellNode * nodes = new CellNode[MAX(2 * LocalHeight * LocalWidth, 100)];
	PriorityQueueClass<CellNode> * queue = new PriorityQueueClass<CellNode>(MAX(2 * LocalHeight * LocalWidth, 100));

	int marker_index = 0;
	bool result_flag = true;

	/*
	 * Seed the starting angle from which side of the map the region touches.
	 */
	double angle = 0.0;
	if (bounds.X != 0) {
		angle = 0.0;
	} else if (bounds.Width != MAP_CELL_W) {
		angle = M_PI;
	}
	if (bounds.Y != 0) {
		angle = 3 * M_PI_2;
	} else if (bounds.Height != MAP_CELL_H) {
		angle = M_PI_2;
	}

	Clear_Cell_Data_Spreads();

	/*
	 * Insert the region's border cells as the initial frontier.
	 */
	DynamicVectorClass<Cell> * seed_cells = Build_Region_Border_Cell_List(region_id);
	int seed_count = seed_cells->Count();

	int spread_count = 0;
	if (seed_count > 0) {
		CellNode * newnode = nodes;
		do {
			Cell seed = (*seed_cells)[spread_count];
			if (seed.X >= bounds.X && seed.X < bounds.X + bounds.Width &&
				seed.Y >= bounds.Y && seed.Y < bounds.Y + bounds.Height) {

				newnode->Element = seed;
				newnode->Score = Get_Angle_Score(origin, seed, angle);
				marker_index++;
				queue->Insert(*newnode++);
			}
		} while (++spread_count < seed_count);
	}

	delete seed_cells;

	/*
	 * Compute how far to spread.
	 */
	double scale = log((double)seed_count);
	if (1.0 > scale) {
		scale = 1.0;
	}

	spread_count = (int)(1.0f / ((float)(1.0 / scale) * spread_scale) * 0.5f);
	spread_count += Pick_Random_UInt(0, spread_count / 2);

	CellNode * node = queue->Extract_Min();

	int spread_step = 0;
	if (spread_count > 0) {
		while (true) {
			if (!result_flag || node == NULL) {
				break;
			}

			MapRegionClass::CellData & ndata = MapRegionClass::Get_Cell_Data(node->Element);
			if (!ndata.RegionID) {
				if (Map[node->Element].Is_Tile_Clear()) {
					ndata.CanExpand = true;
					ndata.RegionID = region_id;
				}
			}

			CellNode * newnode = &nodes[marker_index];
			for (FacingType dir = FACING_FIRST; dir < FACING_COUNT; dir = FacingType(dir + FACING_90)) {
				Cell newcell = Adjacent_Cell(node->Element, dir);
				if (My_In_Radar(newcell)) {
					int other_region;
					if (RMGCellData != NULL) {
						MapRegionClass::CellData & data = MapRegionClass::Get_Cell_Data(newcell);
						if (!data.RegionID && data.SpreadID != region_id &&
							newcell.X >= bounds.X && newcell.X < bounds.X + bounds.Width &&
							newcell.Y >= bounds.Y && newcell.Y < bounds.Y + bounds.Height &&
							Map[newcell].Is_Tile_Clear()) {

							newnode->Element = newcell;
							newnode->Score = Get_Angle_Score(origin, newcell, angle);
							Set_Cell_Data_Spread(newcell, region_id);
							marker_index++;
							queue->Insert(*newnode++);
							continue;
						}
						other_region = MapRegionClass::Get_Cell_Data(newcell).RegionID;
						if (!other_region) {
							continue;
						}
					} else {
						other_region = -1;
					}

					if (other_region != region_id) {
						result_flag = false;
					}
				}
			}

			spread_step++;
			angle += Sample_Normal(0, M_PI_4);
			node = queue->Extract_Min();
			if (spread_step >= spread_count) {
				break;
			}
		}
	}

	/*
	 * Optionally drain the remaining frontier and assign it to the region.
	 */
	if (claim_frontier) {
		CellNode * remaining = queue->Extract_Min();
		while (remaining != NULL) {
			if (!result_flag) {
				break;
			}

			int other_region;
			if (RMGCellData != NULL) {
				MapRegionClass::CellData & data = MapRegionClass::Get_Cell_Data(remaining->Element);
				other_region = data.RegionID;
				if (!other_region) {
					data.RegionID = region_id;
				} else if (other_region != region_id) {
					result_flag = false;
				}
			} else {
				if (-1 != region_id) {
					result_flag = false;
				}
			}

			remaining = queue->Extract_Min();
		}
	}

	delete[] nodes;
	delete queue;

	return(result_flag);
}


// The rebuild's helpers of Create_Tiberium (YR: the start count is MapRegionClass::Unknown20).
static inline int Start_Count(MapRegionClass const * region)
{
	return(region->Unknown20);
}

static inline bool Rich_Starts(MapSeedClass const & seed)
{
	return(seed.Resources == 3 && (seed.MapType == 1 || seed.MapType == 3 || seed.MapType == 4));
}

static inline double Cell_Distance(Cell const & c, int x, int y)
{
	int dx = c.X - x;
	int dy = c.Y - y;
	return(fastmath::sqrt((double)(dy * dy + dx * dx)));
}

// gamemd.exe 0x005A23A0 (1,300 bytes, the size of RA2's 0x00583B80).
void MapGeneratorClass::Create_Tiberium(void)
{
	int waypoint = 0; // RA2: Create_Tiberium at 0x583B80, RA2's form (fields per region, grown from its StartCells)
	bool seek_center;
	int resources = SeedData.Resources;
	switch (SeedData.MapType) {
		case 0: seek_center = resources == 3; break;
		case 1:
		case 3:
		case 4: seek_center = resources != 3; break;
		case 2: seek_center = true; break;
		default: seek_center = false; break;
	}

	for (int index = 0; index < MapRegionClass::MapRegions.Count(); index++) {
		MapRegionClass * region = MapRegionClass::MapRegions[index];
		if (region->Unknown0 == NULL) continue;

		int nearest = -1;
		if (seek_center) {
			Cell center(0, 0);
			if (region->Unknown20 > 0) {
				for (int player = 0; player < region->Unknown20; player++) {
					Cell start = Scen->Get_Waypoint_Cell((WAYPOINT)(player + waypoint));
					center.Y += start.Y;
					center.X += start.X;
				}
				center.X = center.X / region->Unknown20;
				center.Y = center.Y / region->Unknown20;
			} else {
				center = region->Cells[Pick_Random_UInt(0, region->Cells.Count() - 1)];
			}
			int best = 500000;
			for (int i = 0; i < region->Unknown0->Count(); i++) {
				Cell c = (*region->Unknown0)[i];
				double distance = fastmath::sqrt((double)((c.Y - center.Y) * (c.Y - center.Y) + (c.X - center.X) * (c.X - center.X)));
				if (distance < (double)best) {
					best = (int)distance;
					nearest = i;
				}
			}
		}

		double percent = SeedData.Tiberium * 0.01;
		int total = (int)(percent * (RMGMaximumTiberium - RMGMinimumTiberium) + RMGMinimumTiberium);
		double players = region->Unknown20;
		int amount = (int)((double)total * (0.5 > players ? 0.5 : players));
		if (region->Unknown0->Count() == 0 || amount == 0) continue;

		int per_field = amount / region->Unknown0->Count();
		for (int field = 0; field < region->Unknown0->Count(); field++) {
			int size = (int)((double)per_field + Sample_Truncated_Normal_Inline(0.0, 50.0, -100.0, 100.0));
			if (size >= 0) {
				Create_Tiberium_Patch((*region->Unknown0)[field], size, field + waypoint + 1, field == nearest);
			}
		}

		/*
		**	Every player starting in this region gets a field of their own, the bigger the
		**	further they sit from the fields already grown.
		*/
		DynamicVectorClass<double> distances;
		for (int player = 0; player < Start_Count(region); player++) {
			double total_distance = 0.0;
			Cell start = Scen->Get_Waypoint_Cell((WAYPOINT)(player + waypoint));
			for (int i = 0; i < region->Unknown0->Count(); i++) {
				Cell c = (*region->Unknown0)[i];
				total_distance += Cell_Distance(c, start.X, start.Y);
			}
			distances.Add(total_distance / (double)region->Unknown0->Count());
		}

		double closest = 9999999.0;
		for (int i = 0; i < Start_Count(region); i++) {
			if (distances[i] < closest) closest = distances[i];
		}

		static const double _distance_scale = 15.0; // RA2: the extra field cells per cell of distance beyond the closest start
		bool rich = Rich_Starts(SeedData);
		for (int spot = 0; spot < Start_Count(region); spot++) {
			int size = (int)((distances[spot] - closest) * _distance_scale); // RA2: a named constant of the function: the executable reads this 15.0 at 0x7A5CE8, among the function statics, while the literal 15.0 elsewhere in this file is the copy kept from an earlier object (0x7A5398)
			Create_Tiberium_Patch(Scen->Get_Waypoint_Cell((WAYPOINT)(spot + waypoint)), size + 500, spot + waypoint + 1, rich);
		}
		waypoint += Start_Count(region);
	}
}


bool MapGeneratorClass::Generate_Rural_Roads(const Cell & cell)
{
	QueueClass<DirtRoadNode, 1024> queue;

	int processed = 0;

	/*
	 * Pick a junction tile that fits at the starting cell. All eleven junction
	 * shapes are tried once, beginning at a random shape so that the roads
	 * don't always start out looking the same.
	 */
	int attempt = 0;
	unsigned int random_offset = Pick_Random_UInt(0, 10);
	IsometricTileType junction;
	while (true) {
		junction = (IsometricTileType)(IsometricTileTypeClass::DirtRoadJunction + (int)(random_offset + attempt) % DIRT_ROAD_JUNCTION_COUNT);
		if (Can_Place_Tile(junction, cell, false, false)) {
			break;
		}
		if (++attempt >= 11) {
			return(false);
		}
	}

	/*
	 * Seed the work queue with the junction tile. It has no cap tile and no
	 * back link, since there is no road leading into it yet.
	 */
	DirtRoadNode root(junction, cell, ISOTILE_NONE, cell, -1);
	queue.Add(root);
	if (queue.Count == 0) {
		return(false);
	}

	/*
	 * Grow the road network breadth-first until the queue runs dry or comes
	 * close to overflowing.
	 */
	while (queue.Count < 1020) {
		bool success = true;
		DirtRoadNode node = queue.First();

		int child_count = 0;
		DirtRoadNode * children = NULL;

		if (node.Tile != ISOTILE_NONE) {

			/*
			 * Commit this tile to the map. The cap tile that provisionally
			 * terminated the road here is no longer needed.
			 */
			if (node.CapTile != ISOTILE_NONE) {
				Mark_Tile(node.CapTile, node.CapOrigin, false);
			}
			Place_Tile(node.Tile, node.Origin);

			DirtRoadTile & tile = DirtRoadTiles[Dirt_Road_Tile_Index(node.Tile)];

			/*
			 * Extend the road out of every link of this tile except the one
			 * that leads back the way it came.
			 */
			if (tile.Count > 1) {
				int link_count = tile.Count - (node.BackLinkIndex != -1);
				children = new DirtRoadNode[link_count];
				if (children != NULL) {
					DirtRoadNode * child = children;
					for (int link_index = 0; link_index < tile.Count; link_index++) {
						if (link_index != node.BackLinkIndex) {
							DirtRoadLink link = tile.Links[link_index];
							FacingType reverse = Facing_Sub(link.Facing, FACING_180);

							/*
							 * Find a road piece to continue with at the cell this
							 * link connects to, along with a cap piece in case the
							 * continuation is later rolled back. Failing to connect
							 * dooms the whole tile.
							 */
							Cell connection = Adjacent_Cell(node.Origin + link.Offset, link.Facing);
							int cap_link = 0;
							if (Find_Dirt_Road_Tile(reverse, connection, 1, child->CapTile, child->CapOrigin, cap_link)) {
								if (!Find_Dirt_Road_Tile(reverse, connection, 4, child->Tile, child->Origin, child->BackLinkIndex)) {
									child->BackLinkIndex = cap_link;
								} else {
									Mark_Tile(child->Tile, child->Origin, true);
								}
								Mark_Tile(child->CapTile, child->CapOrigin, true);
								child_count++;
								child++;
							} else {
								success = false;
								break;
							}
						}
					}
				}
			}
		}

		if (success && node.Tile != ISOTILE_NONE) {

			/*
			**	Every link found a continuation, so queue the new road pieces up
			**	for processing of their own links.
			*/
			if (children != NULL) {
				DirtRoadNode * src = children;
				for (int j = 0; j < child_count; j++) {
					queue.Add(*src);
					src++;
				}
			}
		} else {

			/*
			 * One of the links could not be connected. Take the tile back off
			 * the map, unmark whatever continuations were already found, and
			 * terminate the incoming road with the cap tile instead.
			 */
			if (node.Tile != ISOTILE_NONE) {
				Remove_Tile(node.Tile, node.Origin);
				DirtRoadNode * undo = children;
				while (child_count > 0) {
					if (undo->Tile != ISOTILE_NONE) {
						Mark_Tile(undo->Tile, undo->Origin, false);
					}
					if (undo->CapTile != ISOTILE_NONE) {
						Mark_Tile(undo->CapTile, undo->CapOrigin, false);
					}
					undo++;
					--child_count;
				}
			}
			if (node.CapTile != ISOTILE_NONE) {
				Place_Tile(node.CapTile, node.CapOrigin);
			}
		}

		if (children != NULL) {
			delete [] children;
		}

		queue.Next();
		processed++;
		if (queue.Count == 0) {
			return(processed > 1);
		}
	}

	/*
	 * The queue filled up before the road network was finished. Every pending
	 * road gets sealed off with its cap tile.
	 */
	while (queue.Count != 0) {
		DirtRoadNode node = queue.First();
		Place_Tile(node.CapTile, node.CapOrigin);
		queue.Next();
	}
	return(processed > 1);
}


