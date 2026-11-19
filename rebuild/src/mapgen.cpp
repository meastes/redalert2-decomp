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

#include "always.h"
#include "geometry.h" // RA2: the header statics, in the order of the unit's initializer table

#include "mapgen.h"

#include "_map.h"
#include "_palette.h"
#include "_rules.h"
#include "_tactica.h"
#include "animtype.h"
#include "astar.h"
#include "building.h"
#include "builtype.h"
#include "ccrand.h"
#include "cell.h"
#include "conquer.h"
#include "coord.h"
#include "data.h"
#include "dbgprint.h"
#include "fastmath.h"
#include "house.h"
#include "houstype.h"
#include "incdec.h"
#include "infantry.h"
#include "infatype.h"
#include "init.h"
#include "inline.h"
#include "isotype.h"
#include "language\language.h"
#include "netshare.h"
#include "nodes.h"
#include "overtype.h"
#include "ownrdraw.h"
#include "pcx.h"
#include "progress.h"
#include "queue.h"
#include "rules.h"
#include "smartdeform.h"
#include "smudtype.h"
#include "tactical.h"
#include "tagtype.h"
#include "terrain.h"
#include "terrtype.h"
#include "tiberium.h"
#include "trigtype.h"
#include "unit.h"
#include "unittype.h"
#include "vector.h"
#include "vein.h"
#include "wdtnet.h"
#include "winfix.h"
#include "worlddom.h"

#include "ramp.hh"
#include "stringtable.h" // RA2

#include "rcptr.h" // RA2
#include "_theater.h" // RA2
#include "_keyboar.h" // RA2: Keyboard->Clear in Map_Seed_Dialog_Proc
#include "keyboard.h" // RA2: as above
#include "findmake.h" // RA2: TGet_TypeList, used by Read_RMG_INI

#define RA2_SOURCE "D:\\Ra2\\MapGen.cpp" // RA2: the file name RA2 passes to StringTable::LoadString

rc_ptr<WDTTerritory> Find_WDT_Territory(int index); // RA2: 0x769690, the tour's territory with this index (not in this module)
void __fastcall Send_Map_Preview(MapPreviewClass * preview); // RA2: 0x5C7EF0, sends the preview to the guests (defined in netshare.cpp)




bool (*RMGCallback)() = MapGen_Call_Back;

BOOL CALLBACK Map_Seed_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);


double Random_Fraction(void);
unsigned int Pick_Random_UInt(unsigned int minval, unsigned int maxval);

MapGeneratorClass RandomMapGen;

Random2Class RMGRandom;
#define RMG_RANDOM_DOUBLE(max_value) ((unsigned int)RMGRandom() * (double)(max_value)) * (1.0 / 4294967295.0)
#define RMG_RANDOM_DOUBLE2(max_value) ((unsigned int)RMGRandom() * (1.0 / 4294967295.0)) * (double)(max_value)

double Sample_Normal(double mean, double standard_dev);



struct DirtRoadLink {
	DirtRoadLink(void);
	DirtRoadLink(Cell const & cell, FacingType dir);
	DirtRoadLink(DirtRoadLink const & that);

	Cell Offset;
	FacingType Facing;
};


struct DirtRoadTile {
	DirtRoadTile(DirtRoadLink const & link1);
	DirtRoadTile(DirtRoadLink const & link1, DirtRoadLink const & link2);
	DirtRoadTile(DirtRoadLink const & link1, DirtRoadLink const & link2, DirtRoadLink const & link3);
	DirtRoadTile(DirtRoadLink const & link1, DirtRoadLink const & link2, DirtRoadLink const & link3, DirtRoadLink const & link4);
	DirtRoadTile(void);
	~DirtRoadTile(void);

	int Count;
	DirtRoadLink * Links;
	IsometricTileType ITType;
};


struct DirtRoadNode {
	DirtRoadNode(void);
	DirtRoadNode(IsometricTileType road_tile, Cell const & origin, IsometricTileType cap_tile, Cell const & cap_origin, int link_index);
	DirtRoadNode(DirtRoadNode const & that);

	IsometricTileType Tile;
	Cell Origin;
	IsometricTileType CapTile;
	Cell CapOrigin;
	int BackLinkIndex;
};


extern DirtRoadTile DirtRoadTiles[];








































/// <summary>
/// RA2
/// </summary>
static inline bool Is_Three_Or_Four(int value)
{
	return value == 3 || value == 4;
}

















/// Gaussian (normal) random number generator using the Marsaglia polar form of the Box-Muller transform.
class NormalDistribution
{
	public:
		NormalDistribution(double (*function)(void));
		double operator() (void);

		bool HasSpare;
		double Spare;
		double (*Rand)(void);
};


NormalDistribution RMGGaussian(Random_Fraction);

int MapRegionClass::MapStartDiagonal;
int MapRegionClass::MapEndDiagonal;

MapRegionClass::CellData *RMGCellData;

int MapRegionClass::TotalCount;

DynamicVectorClass<MapRegionClass *> MapRegionClass::MapRegions;
// RA2: no TiberiumLayoutCells (each region keeps its own StartCells)


/// <summary>
/// Creates a cleared working record for a map cell.
/// This routine puts the cell into the state the random map generator expects to find it in
/// before any region has laid claim to it -- unowned, flat, unforested and with no tile
/// placed on it yet.
/// </summary>
MapRegionClass::CellData::CellData(void) :
	CellID(0,0),
	HillHeight(0),
	HillVolatility(0),
	GreenTileChance(0),
	SandTileChance(0),
	RoughTileChance(0),
	ForestChance(0),
	RegionID(0),
	SpreadID(0),
	WaterMask(-1),
	Interior(false),
	Inviolate(false),
	TilePlaced(false),
	Forested(false),
	Iced(false),
	InFillArea(false),
	InFillReach(true),
	CanExpand(false)
{
	//nothing
}

void Init_Dirt_Roads(void);
int Dirt_Road_Tile_Index(IsometricTileType ittype);


/// <summary>
/// Fetches a random cell from anywhere on the map.
/// Slots the generator never filled in are passed over, so the cell handed back is
/// always a real one.
/// </summary>
/// <returns>Returns with the cell picked, chosen uniformly from the mapped cells.</returns>
Cell MapRegionClass::Pick_Random_Map_Cell(void)
{
	int grid_size = MapCellStride * MapCellStride;
	unsigned int index;
	do {
		index = Pick_Random_UInt(0, grid_size - 1);
	} while (RMGCellData[index].CellID == Cell(0,0));
	return(RMGCellData[index].CellID);
}


/// <summary>
/// Fetches a random clear cell from anywhere on the map.
/// This is the map wide counterpart of Pick_Random_Clear_Cell, for callers that do not
/// care which region the cell falls in. The search gives up rather than blocking if the
/// map is congested.
/// </summary>
/// <returns>Returns with the clear cell found, or Cell(0,0) if the map has none to
/// spare.</returns>
Cell MapRegionClass::Pick_Random_Clear_Map_Cell(void)
{
	int tries = 0;
	while (true) {
		Cell cell = Pick_Random_Map_Cell();
		if (++tries > 200) {
			return(Cell(0,0));
		}
		if (Map[cell].Is_Tile_Clear()) {
			return(cell);
		}
	}
}


/// <summary>
/// Constructs a map region grown from a seed cell.
/// The region takes its terrain from the seed and joins the generator's list of regions,
/// which is what the map generator later grows, merges and scores.
/// </summary>
/// <param name="cell">The seed cell the region starts out from.</param>
MapRegionClass::MapRegionClass(Cell cell) :
	StartCells(0), // RA2
	NeighborRegions(0),
	ID(TotalCount),
	CellCount(0),
	CellHeight(Map[cell].Height),
	ContainsWater(Map[cell].Is_Tile_With_Water()),
	Position(cell),
	Unsplittable(false),
	HasConnections(true),
	SplitCount(0),
	StartCount(0), // RA2: as YR's
	Discardable(false),
	Cells(),
	RegionBounds(0,0,0,0)
{
	Cells.Set_Growth_Step(100000);

	TotalCount++;
	MapRegions.Add(this);
}


/// <summary>
/// Destructor. Removes this region from the global MapRegions list.
/// </summary>
MapRegionClass::~MapRegionClass(void)
{
	if (StartCells != NULL) { // RA2
		delete StartCells; // RA2
		StartCells = NULL; // RA2
	} // RA2
	MapRegions.Delete(this);
}


/// <summary>
/// Removes every map region the generator has built.
/// This routine wipes the slate between generation attempts, so that a fresh run starts
/// with no regions at all and its region numbering back at zero.
/// </summary>
void MapRegionClass::Delete_All_Regions(void)
{
	MapRegionClass::TotalCount = 0;

	for (int i = MapRegionClass::MapRegions.Count() - 1; i >= 0; i--) { // RA2: no TiberiumLayoutCells (not in RA2's layout)
		MapRegionClass * region = MapRegionClass::MapRegions[i];
		if (region != NULL) {
			delete region;
		}
	}
}


/// <summary>
/// Determines if the generator may still alter this cell.
/// </summary>
/// <returns>bool; Is the cell still unprotected?</returns>
bool MapRegionClass::CellData::Is_Not_Inviolate(Cell const &cell)
{
	return(!MapRegionClass::Get_Cell_Data(cell).Inviolate);
}


/// <summary>
/// Determines if the cell lies well in from the edge of its region.
/// The interior flag is what keeps lake seeding off the fringe of the ground it grows on.
/// </summary>
/// <returns>bool; Does the cell lie in the interior?</returns>
bool MapRegionClass::CellData::Is_Interior(Cell const &cell)
{
	return(MapRegionClass::Get_Cell_Data(cell).Interior);
}


/// <summary>
/// Protects a cell from further alteration.
/// This routine is used to pin down a cell the generator has finished with, so that a
/// later pass leaves it alone.
/// </summary>
void MapRegionClass::CellData::Set_Inviolate(Cell const &cell)
{
	MapRegionClass::Get_Cell_Data(cell).Inviolate = true;
}


/// <summary>
/// Lifts the protection from a cell.
/// This routine is the counterpart of Set_Inviolate, and hands a pinned down cell back to
/// the passes that follow.
/// </summary>
void MapRegionClass::CellData::Clear_Inviolate(Cell const &cell)
{
	MapRegionClass::Get_Cell_Data(cell).Inviolate = false;
}


/// The loop body indexes with the bound `grid_size` rather than the loop variable, so
/// nothing within the grid is cleared and the write lands one slot past the end of it.

/// <summary>
/// Lifts the protection from every cell on the map.
/// This routine is used between generator passes, when cells that an earlier pass pinned
/// down are meant to become fair game again.
/// </summary>
void MapRegionClass::CellData::Clear_Inviolates(void)
{
	int grid_size = MapCellStride * MapCellStride;
	for (int i = 0; i < grid_size; i++) {
		MapRegionClass::Get_Cell_Data(grid_size).Inviolate = false;
	}
}


/// <summary>
/// Fetches the generator's working record for a cell.
/// Every cell carries one of these records for as long as the random map generator is
/// running. It is where region ownership, the terrain chances and the protection flag
/// live, and every other routine here reaches its cell through this one.
/// </summary>
/// <returns>Returns with a reference to the cell's working record.</returns>
MapRegionClass::CellData & MapRegionClass::Get_Cell_Data(Cell const & cell)
{
	return(RMGCellData[cell.X + cell.Y * (int)MapCellStride]); // RA2: Get_Cell_Data at 0x56DBA0 matches only so
}


/// <summary>
/// Fetches the generator's working record for a grid slot.
/// This is the raw index overload, for callers that are already walking the cell data
/// grid rather than holding a cell.
/// </summary>
/// <param name="index">The slot within the cell data grid.</param>
/// <returns>Returns with a reference to the working record at that slot.</returns>
MapRegionClass::CellData & MapRegionClass::Get_Cell_Data(int index)
{
	return(RMGCellData[index]);
}


/// <summary>
/// Brings this region's cell tally back in step with the grid.
/// Regions hand cells back and forth as they grow, merge and split, which leaves the
/// running count stale. Use this routine to re-establish it.
/// </summary>
void MapRegionClass::Recount_Cells(void)
{
	CellCount = 0;
	int grid_size = MapCellStride * MapCellStride;

	for (int i = 0; i < grid_size; i++) {
		if (RMGCellData[i].RegionID == ID && RMGCellData[i].CellID != Cell(0,0)) {
			CellCount++;
		}
	}
}


/// <summary>
/// Fetches a random cell belonging to this region.
/// </summary>
/// <returns>Returns with the cell picked, chosen uniformly from the region's cells.</returns>
Cell MapRegionClass::Pick_Random_Cell(void)
{
	int grid_size = MapCellStride * MapCellStride;
	unsigned int index = 0;
	do {
		index = Pick_Random_UInt(0, grid_size - 1);
	} while (RMGCellData[index].RegionID != ID || RMGCellData[index].CellID == Cell(0,0));

	return(RMGCellData[index].CellID);
}


/// <summary>
/// Fetches a random clear cell from this region.
/// This routine is used when the generator needs somewhere unobstructed to place
/// something. The search gives up rather than blocking if the region is congested.
/// </summary>
/// <returns>Returns with the clear cell found, or Cell(0,0) if the region has none to
/// spare.</returns>
Cell MapRegionClass::Pick_Random_Clear_Cell(void)
{
	int tries = 0;
	while (true) {
		Cell cell = Pick_Random_Cell();
		if (++tries > 200) {
			return(Cell(0,0));
		}
		if (Map[cell].Is_Tile_Clear()) {
			return(cell);
		}
	}
}


/// The candidate is assigned through a reference bound to grid entry 0, so the search
/// scribbles over that entry as it runs and the cell handed back is read from it.

/// <summary>
/// Fetches a random clear cell from the interior of this region.
/// This routine is used where the generator must keep well away from the region's edges.
/// The search gives up rather than blocking if the interior is congested.
/// </summary>
/// <returns>Returns with the clear interior cell found, or Cell(0,0) if the region has
/// none to spare.</returns>
Cell MapRegionClass::Pick_Random_Clear_Interior_Cell(void)
{
	MapRegionClass::CellData *cell = RMGCellData; // RA2: RA2's form
	int count = MapCellStride * MapCellStride; // RA2: as above
	int tries = 0;
	for (;;) { // RA2: as above
		unsigned int maximum = count - 1; // RA2: as above
		unsigned int index; // RA2: as above
		do { // RA2: as above
			index = (unsigned int)((unsigned int)RMGRandom() * (1.0 / 4294967295.0) * (unsigned int)(maximum + 1) + (double)0); // RA2: as above
		} while (index > maximum); // RA2: as above
		*cell = RMGCellData[index]; // RA2: as above
		if (++tries >= 100) {
			break; // RA2: as above
		}
		if (cell->RegionID != ID) { // RA2: as above
			continue; // RA2: as above
		}
		if (cell->CellID.X == 0 && cell->CellID.Y == 0) { // RA2: as above
			continue; // RA2: as above
		}
		if (!cell->Interior) { // RA2: as above
			continue; // RA2: as above
		}
		if (Map[cell->CellID].Is_Tile_Clear()) { // RA2: as above
			return cell->CellID; // RA2: as above
		}
	}
	return Cell(0, 0); // RA2: as above
}



/// <summary>
/// Fetches a random clear cell lying within a movement zone.
/// This routine is used when the generator needs somewhere unobstructed that is also
/// reachable -- a starting point, or anything that would be useless marooned. The search
/// gives up rather than blocking if nothing suitable turns up.
/// </summary>
/// <param name="zone">The movement zone the cell must lie within.</param>
/// <param name="region_id">The region the cell must belong to, or -1 to accept any region.</param>
/// <returns>Returns with the cell found, or Cell(0,0) if the zone has none to spare.</returns>
Cell MapRegionClass::Pick_Random_Clear_Cell_In_Zone(int zone, int region_id)
{
	MapRegionClass::CellData & data = RMGCellData[0];
	int grid_size = MapCellStride * MapCellStride;
	int tries = 0;
	CellClass * cellptr; // RA2: Pick_Random_Clear_Cell_In_Zone at 0x56DEA0 matches only so
	do { // RA2: as above
		data = RMGCellData[Pick_Random_UInt(0, grid_size - 1)];
		if (++tries >= 100) {
			return(Cell(0,0)); // RA2: as above
		}
		cellptr = &Map[data.CellID]; // RA2: as above
	} while (data.CellID == Cell(0,0) // RA2: as above
			|| Map.Get_Cell_Zone(cellptr->CellID, MZONE_NORMAL, false) != zone // RA2: the cell by reference
			|| !cellptr->Is_Tile_Clear() // RA2: as above
			|| (region_id != -1 && data.RegionID != region_id)); // RA2: as above
	return(data.CellID); // RA2: as above
} // RA2: Pick_Random_Clear_Cell_In_Zone at 0x56DEA0 has no return(c);


/// <summary>
/// Clears the spread markers on every cell, back to -1.
/// Every routine that spreads out from a seed cell stamps its own id here to remember which
/// cells it has already reached. This routine wipes the grid clean for the next one.
/// Note that Clear_Cell_Data_Spreads clears to zero rather than to -1.
/// </summary>
void MapRegionClass::CellData::Reset_Spreads(bool)
{
	int grid_size = MapCellStride * MapCellStride;
	for (int i = 0; i < grid_size; i++) {
		RMGCellData[i].SpreadID = -1;
	}
}


/// <summary>
/// Scores a step against a preferred direction.
/// This routine is used to steer the wandering boundary that Split_Region grows. A dash
/// of randomness is mixed into the score so that the boundary does not come out looking
/// ruler-drawn.
/// </summary>
/// <param name="cell1">The cell the step starts from.</param>
/// <param name="cell2">The cell the step ends at.</param>
/// <param name="ref_angle">The preferred direction, in radians.</param>
/// <returns>Returns with the score. It rises with both the length of the step and how
/// far the step strays from the preferred direction, so the lowest-scoring step is the
/// one that suits best.</returns>
double MapRegionClass::Get_Angle_Score(const Cell & cell1, const Cell & cell2, double ref_angle)
{
	static const double _angle_weight = 1.5;
	static const double _length_weight = 0.15;

	int dx = cell2.X - cell1.X;
	int dy = cell2.Y - cell1.Y;

	double length = fastmath::sqrt((double)(dx * dx + dy * dy));

	double angle = (dx != 0) ? fastmath::atan(-((double)dy / (double)dx)) : M_PI_2;
	if (dx < 0) {
		angle += M_PI;
	}

	double angle_diff = fabs(angle - ref_angle);
	while (angle_diff >= M_PI * 2) {
		angle_diff -= M_PI * 2;
	}
	if (angle_diff > M_PI) {
		angle_diff = M_PI * 2 - angle_diff;
	}

	return(RMG_RANDOM_DOUBLE2(2.5) + _length_weight * length + _angle_weight * angle_diff);
}











/// <summary>
/// Creates a region grown out from a seed cell.
/// This routine gathers up every cell reachable from the seed that shares its height and
/// its wet or dry character, and makes a region of them. A dry patch too small to be
/// worth keeping is dissolved into its surroundings instead of becoming a region, and
/// the ground under it is flattened to match.
/// </summary>
/// <param name="cptr">The cell to grow the region out from.</param>
/// <returns>Returns with the region created. NULL is returned when the patch was
/// dissolved rather than kept.</returns>
#ifdef RA2_NON_MATCHING // not exact: 1401 bytes differ (adopted text)
MapRegionClass *MapRegionClass::Create_Region_From_Cell(CellClass *cptr) // RA2: RA2's form (0x56E100): green-LAT ground counts as water-like and the wipes call Clear_Region_Cells; near miss, the exe calls TPoint2D::operator+ out of line
{
	int i;
	int region_id = MapRegionClass::TotalCount;

	DynamicVectorClass<Cell> cells;
	cells.Set_Growth_Step(50000);
	cells.Add(cptr->Fetch_CellID());

	bool has_water = cptr->Is_Tile_With_Water() || cptr->Is_Tile_Clear_To_Green_LAT(); // RA2: as above
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
						int n_has_water = Map[ncell].Is_Tile_With_Water() || Map[ncell].Is_Tile_Clear_To_Green_LAT(); // RA2: as above
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

						RandomMapGen.Clear_Region_Cells(0, -1, height, 0); // RA2: as above

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

				RandomMapGen.Clear_Region_Cells(region_id, id, height, 0); // RA2: as above

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
#endif



/// <summary>
/// Discards every region on the map.
/// This routine strips the region assignments out of the generator's cell grid and
/// destroys the regions themselves. Use it before rebuilding the regions from scratch.
/// </summary>
void MapRegionClass::Destroy_Cell_Regions(void)
{
	if (RMGCellData != NULL) {
		int grid_size = MapCellStride * MapCellStride;
		for (int i = 0; i < grid_size; i++) {
			RMGCellData[i].RegionID = -1;
			RMGCellData[i].SpreadID = -1;
		}
	}

	for (int i = MapRegionClass::MapRegions.Count() - 1; i >= 0; i--) {
		MapRegionClass * region = MapRegionClass::MapRegions[i];
		if (region != NULL) {
			delete region;
		}
	}

	MapRegionClass::TotalCount = 0;
}


/// <summary>
/// Creates a region for every stretch of water.
/// This routine is run ahead of the land pass so that water and swamp are gathered up
/// first, leaving Create_Land_Regions to mop up whatever is left over.
/// </summary>
void MapRegionClass::Create_Water_Regions(void)
{
	int grid_size = MapCellStride * MapCellStride;

	for (int i = 0; i < grid_size; i++) {
		Cell cell = RMGCellData[i].CellID;
		CellClass *cptr = &Map[cell];
		if (RMGCellData[i].RegionID == -1 && RMGCellData[i].CellID != Cell(0,0)) {
			if (cptr->Is_Tile_With_Water() || cptr->Is_Tile_Swamp()) {
				Create_Region_From_Cell(cptr);
			}
		}
	}
}


/// <summary>
/// Creates regions out of the remaining land cells.
/// This routine is the companion to Create_Water_Regions and is run after it, so that
/// every cell the water pass left over ends up belonging to some region of land.
/// </summary>
/// <param name="unsplittable">Should the regions created be exempt from later splitting?</param>
void MapRegionClass::Create_Land_Regions(bool unsplittable)
{
	int grid_size = MapCellStride * MapCellStride;

	for (int i = 0; i < grid_size; i++) {
		if (RMGCellData[i].RegionID == -1 && RMGCellData[i].CellID != Cell(0,0)) {
			CellClass *cptr = &Map[RMGCellData[i].CellID];
			MapRegionClass *rgn = Create_Region_From_Cell(cptr);
			if (rgn != NULL) {
				rgn->Unsplittable = unsplittable;
			}
		}
	}
}


/// <summary>
/// Assigns a cell to a region.
/// This routine quietly does nothing when the generator's cell grid has not been
/// allocated.
/// </summary>
/// <param name="region_id">The region id to store.</param>
void MapRegionClass::CellData::Set_Region(Cell const &cell, int region_id)
{
	if (RMGCellData != NULL) {
		CellData & data = MapRegionClass::Get_Cell_Data(cell);
		data.RegionID = region_id;
	}
}


/// <summary>
/// Fetches the region that a cell belongs to.
/// </summary>
/// <returns>Returns with the cell's region id. -1 is returned if the generator's cell
/// grid has not been allocated.</returns>
int MapRegionClass::CellData::Get_Region(Cell const &cell)
{
	if (RMGCellData == NULL) {
		return(-1);
	}
	return(MapRegionClass::Get_Cell_Data(cell).RegionID);
}


/// <summary>
/// Marks a cell as reached by the spread currently running.
/// </summary>
/// <param name="cell">The cell to mark.</param>
/// <param name="patch_id">The id the spreading routine knows itself by.</param>
void MapRegionClass::CellData::Set_Spread(Cell const &cell, int patch_id)
{
	MapRegionClass::Get_Cell_Data(cell).SpreadID = patch_id;
}


/// <summary>
/// Fetches the spread marker for a cell.
/// A spreading routine tests this against its own id to tell whether it has already been over
/// the cell, so that it cannot double back on itself.
/// </summary>
/// <returns>Returns with the id of the spread that last reached the cell.</returns>
int MapRegionClass::CellData::Get_Spread(Cell const &cell)
{
	return(MapRegionClass::Get_Cell_Data(cell).SpreadID);
}


/// <summary>
/// Wipes this region's cells back to bare ground.
/// This routine hands every cell of the region over to another region and flattens the
/// tile underneath it, optionally setting a new height as it goes. Use it when a region
/// is being dissolved or merged away.
/// </summary>
/// <param name="new_id">The region the freed cells are handed to.</param>
/// <param name="cell_height">Height to set on each cell, or -1 to leave heights alone.</param>
void MapRegionClass::Clear_Cells(int new_id, int cell_height)
{
	int grid_size = MapCellStride * MapCellStride;

	for (int i = 0; i < grid_size; i++) {
		if (RMGCellData[i].RegionID == ID) {
			RMGCellData[i].RegionID = new_id;
			RMGCellData[i].CanExpand = false;
			Cell cell = RMGCellData[i].CellID;
			CellClass *cptr = &Map[cell];
			cptr->ITType = ISOTILE_CLEAR;
			cptr->SubTile = 0;
			if (cell_height != -1) {
				cptr->Height = cell_height;
			}
		}
	}
	CellCount = 0;
}


/// <summary>
/// Hands this region's outer cells over to another region.
/// This routine peels the region inward, a ring at a time, stamping every cell it takes
/// with the region id given. It is used both to shave a region back and, with a scratch
/// id, to work out how far in from the border a cell lies.
/// </summary>
/// <param name="rings">How many rings deep to peel.</param>
/// <param name="region_id">The region id stamped onto each cell taken.</param>
void MapRegionClass::Reassign_Border_Cells(int rings, unsigned int region_id)
{
	DynamicVectorClass<Cell> *ring = Build_Border_Cell_List(); // RA2: RA2's form
	if (rings > 1) {
		int total = MapCellStride * MapCellStride; // RA2: as above
		for (int i = 0; i < total; i++) { // RA2: as above
			RMGCellData[i].SpreadID = -1; // RA2: as above
		}
	}
	for (int step = 0; step < rings; step++) { // RA2: as above
		int index; // RA2: as above
		for (index = ring->Count() - 1; index >= 0; index--) { // RA2: as above
			Cell cell = (*ring)[index]; // RA2: as above
			if (RMGCellData) { // RA2: as above
				RMGCellData[cell.Y * MapCellStride + cell.X].RegionID = region_id; // RA2: as above
			}
		}
		if (step < rings - 1) { // RA2: as above
			DynamicVectorClass<Cell> *next = new DynamicVectorClass<Cell>; // RA2: as above
			next->Set_Growth_Step(ring->Count()); // RA2: as above
			for (index = ring->Count() - 1; index >= 0; index--) { // RA2: as above
				Cell cell = (*ring)[index]; // RA2: as above
				for (int direction = 0; direction < 8; direction++) { // RA2: as above
					Cell const &offset = AdjacentCell[direction & 7]; // RA2: as above
					Cell adjacent = cell + offset; // RA2: as above
					if (adjacent.Y + adjacent.X > MapStartDiagonal && adjacent.X - adjacent.Y < MapStartDiagonal && // RA2: as above
						adjacent.Y - adjacent.X < MapStartDiagonal && adjacent.Y + adjacent.X <= MapEndDiagonal) { // RA2: as above
						MapRegionClass::CellData &data = RMGCellData[adjacent.Y * MapCellStride + adjacent.X]; // RA2: as above
						if (data.RegionID == ID && data.SpreadID != step + 1) { // RA2: as above
							next->Add(adjacent); // RA2: as above
							data.SpreadID = step + 1; // RA2: as above
						}
					}
				}
			}
			delete ring; // RA2: as above
			ring = next; // RA2: as above
		}
	}
	delete ring; // RA2: as above
	if (rings > 1) {
		int total = MapCellStride * MapCellStride; // RA2: as above
		for (int i = 0; i < total; i++) { // RA2: as above
			RMGCellData[i].SpreadID = -1; // RA2: as above
		}
	}
}



/// <summary>
/// Fetches the cells along this region's edge.
/// This routine sweeps the whole map looking for cells of this region that touch some
/// other region. Build_Border_Cell_List2 does the same job from the region's own cell
/// list and is the cheaper of the two when that list can be trusted.
/// </summary>
/// <returns>Returns with a newly allocated vector of the region's edge cells. The caller
/// owns it and must delete it.</returns>
DynamicVectorClass<Cell> *MapRegionClass::Build_Border_Cell_List(void)
{
	DynamicVectorClass<Cell> *cells = new DynamicVectorClass<Cell>;

	Map.Reset_Iterator();
	CellClass *cptr = Map.Iterate();

	while (cptr != NULL) {

		if (CellData::Get_Region(cptr->Fetch_CellID()) == ID) {
			bool add = false;

			for (int facing = FACING_FIRST; facing < FACING_COUNT; facing++) {
				Cell ncell = cptr->Adjacent_Cell((FacingType)facing).Fetch_CellID();
				if (My_In_Radar(ncell)) {
					if (CellData::Get_Region(ncell) != ID) {
						add = true;
					}
				}
			}

			if (add) {
				cells->Add(cptr->Fetch_CellID());
			}
		}

		cptr = Map.Iterate();
	}

	return(cells);
}


/// <summary>
/// Raises or lowers every cell in this region.
/// </summary>
/// <param name="cell_height">The signed amount to add to each cell's height.</param>
void MapRegionClass::Increase_Height(char cell_height)
{
	Map.Reset_Iterator();
	CellClass *cptr = Map.Iterate();
	while (cptr != NULL) {
		int rid = CellData::Get_Region(cptr->Fetch_CellID());
		if (rid == ID) {
			cptr->Height += cell_height;
		}
		cptr = Map.Iterate();
	}
}


/// <summary>
/// Splits this region into two smaller regions.
/// This routine is how the generator turns a large flat expanse into varied ground. A
/// wandering boundary is grown out from a random cell until it has taken a fair share of
/// the region, and each half then becomes a region in its own right, one of them at a
/// new height. Fragments too small to stand on their own are folded into the largest
/// neighbor instead. A region marked unsplittable is left well alone.
/// </summary>
/// <returns>bool; Was the region split?</returns>
#ifdef RA2_NON_MATCHING // not exact: no independent bound yet (0056ef20 has no row)
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
	double dist = (unsigned int)RMGRandom() * (DEG_TO_RAD(360) / 4294967295.0);

	while (node != NULL) {
		if (spread_count >= (int)target_count) {
			break;
		}

		CellData::Set_Region(node->Element, -3);

		int cell_index = node->Element.X + MapCellStride * node->Element.Y;
		CellNode * newnode = &nodes[node_count];

		for (FacingType dir = FACING_FIRST; dir < FACING_COUNT; dir = (FacingType)(dir + FACING_90)) {
			Cell ncell = Adjacent_Cell(node->Element, dir);
			if (My_In_Radar(ncell)) {
				if (CellData::Get_Region(ncell) == -2 && CellData::Get_Spread(ncell) != -3 && Map[ncell].Is_Tile_Clear()) {
					newnode->Element = ncell;
					newnode->Score = Get_Angle_Score(seed, ncell, dist);
					MapRegionClass::Get_Cell_Data(cell_index + AStarFacingToOffset[dir]).SpreadID = -3;
					node_count++;
					queue->Insert(*newnode++);
				}
			}
		}

		spread_count++;
		dist += Sample_Normal(0, DEG_TO_RAD(22.5));
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
				MapRegionClass * other = NULL;
				for (int k = 0; k < MapRegionClass::MapRegions.Count(); k++) {
					if (MapRegionClass::MapRegions[k]->ID == j) {
						other = MapRegionClass::MapRegions[k];
						break;
					}
				}

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

		delete[] adjacent;

		if (region->CellCount > 100) {
			int old_height = region->CellHeight;
			unsigned int max_index = (unsigned int)heights.Count() - 1;
			unsigned int index = Pick_Random_UInt(0, max_index);

			region->CellHeight = heights[index];
			int height_diff = region->CellHeight - old_height;

			Map.Reset_Iterator();
			for (CellClass * cellptr = Map.Iterate(); cellptr; cellptr = Map.Iterate()) {
				Cell c = cellptr->CellID;
				if (MapRegionClass::CellData::Get_Region(c) == region->ID) {
					cellptr->Height += height_diff;
				}
			}
		} else {
			if (merge_target != NULL) {
				int old_height = region->CellHeight;
				region->CellHeight = merge_height;
				int height_diff = region->CellHeight - old_height;

				Map.Reset_Iterator();
				for (CellClass * cellptr = Map.Iterate(); cellptr; cellptr = Map.Iterate()) {
					Cell c = cellptr->CellID;
					if (MapRegionClass::CellData::Get_Region(c) == region->ID) {
						cellptr->Height += height_diff;
					}
				}

				for (int j = region->Cells.Count() - 1; j >= 0; j--) {
					Cell c = region->Cells[j];
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
#endif


/// <summary>
/// Fetches the cells along this region's edge.
/// This routine works from the region's own cell list rather than sweeping the whole
/// map, so it is the cheaper of the two border builders. Use it whenever that list can
/// be trusted to be current.
/// </summary>
/// <returns>Returns with a newly allocated vector of the region's edge cells. The caller
/// owns it and must delete it.</returns>
DynamicVectorClass<Cell> *MapRegionClass::Build_Border_Cell_List2(void)
{
	DynamicVectorClass<Cell> *cells = new DynamicVectorClass<Cell>;
	cells->Set_Growth_Step(MAX(10, CellCount / 2));

	for (int i = Cells.Count() - 1; i >= 0; i--) {
		Cell cell = Cells[i];
		bool add = false;
		for (int facing = FACING_FIRST; facing < FACING_COUNT; facing++) {
			Cell ncell = Adjacent_Cell(cell, (FacingType)facing);
			if (My_In_Radar(ncell)) {
				if (CellData::Get_Region(ncell) != ID) {
					add = true;
					break;
				}
			}
		}

		if (add) {
			cells->Add(cell);
		}
	}

	return(cells);
}


/// <summary>
/// Grows this region outward into unclaimed ground.
/// This routine takes clear, unowned cells that lie at the region's own height, working
/// outward a ring at a time. Running into ground owned by another region abandons the
/// whole attempt, so a caller may use this to find out whether there is room to grow.
/// </summary>
/// <param name="rings">How far out to grow, in rings.</param>
/// <returns>bool; Did the region grow without running into a neighbor?</returns>
#ifdef RA2_NON_MATCHING // not exact: 470 bytes differ (adopted text)
bool MapRegionClass::Grow(int rings) // RA2: RA2's form of Grow (0x570040); near miss (the inlined Adjacent_Cell sum is scheduled differently)
{
	DynamicVectorClass<Cell> * cells = Build_Border_Cell_List();

	for (int i = 0; i < rings; i++) {
		DynamicVectorClass<Cell> * ncells = new DynamicVectorClass<Cell>;
		ncells->Set_Growth_Step(3 * cells->Count());

		for (int j = 0; j < cells->Count(); j++) {
			for (FacingType dir = FACING_FIRST; dir < FACING_COUNT; dir++) {
				Cell ncell = Adjacent_Cell(Cell((*cells)[j]), dir); // RA2: as above
				if (My_In_Radar(ncell)) {
					if (Map[ncell].Height == CellHeight) {
						MapRegionClass::CellData & data = MapRegionClass::Get_Cell_Data(ncell);
						int id = data.RegionID;
						if (id == -1 && (Map[ncell].Is_Tile_Clear() || Map[ncell].Is_Tile_Clear_To_Green_LAT())) { // RA2: green-LAT-clear ground is claimable too
							ncells->Add(ncell);
							data.RegionID = ID;
							Map[ncell].Height = CellHeight;
						} else if (id != ID) {
							delete cells;
							delete ncells;
							return(false);
						}
					}
				}
			}
		}

		delete cells;
		cells = ncells;
	}

	delete cells;
	return(true);
}
#endif



/// <summary>
/// Claims every expandable cell this region can reach.
/// This routine spreads out from the region's border through ground that is still
/// unowned and marked as expandable, taking all of it for this region.
/// </summary>
/// <returns>bool; Always true.</returns>
bool MapRegionClass::Claim_Expandable_Cells(void)
{
	int i;
	int j;

	DynamicVectorClass<Cell> * cells = Build_Border_Cell_List();

	DynamicVectorClass<Cell> ncells;
	ncells.Set_Growth_Step(50000);

	for (i = 0; i < cells->Count(); i++) {
		ncells.Add((*cells)[i]);
	}

	delete cells;

	while (ncells.Count() > 0) {
		j = ncells.Count() - 1;
		Cell c = ncells[j];
		ncells.Delete_Index(j);
		for (FacingType dir = FACING_FIRST; dir < FACING_COUNT; dir++) {
			Cell ncell = Adjacent_Cell(c, dir);
			if (My_In_Radar(ncell)) {
				MapRegionClass::CellData & data = MapRegionClass::Get_Cell_Data(ncell);
				int id = data.RegionID;
				if (id == -1 && data.CanExpand) {
					data.RegionID = ID;
					ncells.Add(ncell);
				}
			}
		}
	}

	return(true);
}


/// <summary>
/// Breaks the map's oversized regions down into smaller ones.
/// This routine first brings every region's cell list and bounding rectangle back in
/// step with the cell grid, then keeps splitting any dry region larger than the map's
/// cliff setting allows until nothing oversized is left. Regions small enough to keep
/// are marked unsplittable so that they are not picked over again.
/// </summary>
void MapRegionClass::Make_Ice_Regions(void)
{
	int i;
	for (i = 0; i < MapRegionClass::MapRegions.Count(); i++) {
		MapRegionClass::MapRegions[i]->Cells.Clear();
		MapRegionClass::MapRegions[i]->CellCount = 0;
		MapRegionClass::MapRegions[i]->RegionBounds = Rect(9999, 9999, 0, 0);
	}

	int grid_size = MapCellStride * MapCellStride;
	for (i = grid_size - 1; i >= 0; i--) {
		CellData & data = MapRegionClass::Get_Cell_Data(i);
		Cell cell = data.CellID;
		int y = cell.Y;
		int x = cell.X;
		if (My_In_Radar(cell)) {
			int id = data.RegionID;
			if (id >= 0 && id < MapRegionClass::MapRegions.Count()) {
				MapRegionClass::MapRegions[id]->CellCount++;
				MapRegionClass::MapRegions[id]->Cells.Add(cell);

				Rect & rect = MapRegionClass::MapRegions[id]->RegionBounds;
				if (!rect.Width) {
					rect.X = x;
					rect.Y = y;
					rect.Width = 1;
					rect.Height = 1;
				}

				int old_x = rect.X;
				if (x >= rect.X + rect.Width) {
					rect.Width = x - old_x + 1;
				}
				if (x < old_x) {
					int old_width = rect.Width;
					rect.X = x;
					rect.Width = old_x - x + old_width;
				}

				int old_y = rect.Y;
				if (y >= rect.Y + rect.Height) {
					rect.Height = y - old_y + 1;
				}
				if (y < old_y) {
					int old_height = rect.Height;
					rect.Y = y;
					rect.Height = old_y - y + old_height;
				}
			}
		}
	}

	double size_limit = ((double)RandomMapGen.SeedData.Cliffs * 0.005 + 0.05) * (double)(int)RandomMapGen.LocalWidth * (double)(int)RandomMapGen.LocalHeight;
	int limit = (int)(size_limit + size_limit);

	while (true) {
		bool restart = false;

		for (int i = 0; i < MapRegionClass::MapRegions.Count(); i++) {
			MapRegionClass * region = MapRegionClass::MapRegions[i];
			if (!region->Unsplittable && !region->ContainsWater) {
				if (region->CellCount > limit) {
					if (region->Split_Region()) {
						delete region;
						restart = true;
						break;
					}
				} else {
					region->Unsplittable = true;
				}
			}
		}

		if (!restart) {
			break;
		}
	}
}


/// <summary>
/// Marks the cells lying deep inside this region.
/// This routine flags every cell of the region that sits well in from its border,
/// leaving the fringe unmarked. The region itself is left owning all of its cells.
/// </summary>
void MapRegionClass::Mark_Interior_Cells(void)
{
	int grid_size = MapCellStride * MapCellStride;
	for (int i = 0; i < grid_size; i++) {
		MapRegionClass::Get_Cell_Data(i).Interior = false;
	}

	Reassign_Border_Cells(8, -2);

	for (int j = 0; j < grid_size; j++) {
		CellData & data = RMGCellData[j];
		if (data.RegionID == ID) {
			RMGCellData[j].Interior = true;
		} else if (data.RegionID == -2) {
			RMGCellData[j].RegionID = ID;
		}
	}
}


/// <summary>
/// Divides the finished terrain up into regions.
/// This routine is the top level of the region pass. The height regions are expanded,
/// the map is carved up afresh, and every region then works out which regions it borders
/// and connects itself to them with bridges and ramps.
/// </summary>
/// <returns>bool; Always true.</returns>
bool MapRegionClass::Make_Regions(void)
{
	bool success = RandomMapGen.Expand_All_High_Ground();
	Destroy_Cell_Regions();
	Create_Land_Regions(false);
	if (success) {
		int i;
		for (i = 0; i < MapRegions.Count(); i++) {
			MapRegions[i]->Build_Neighbor_Regions();
		}
		for (i = 0; i < MapRegions.Count(); i++) {
			MapRegions[i]->Make_Region_Connections();
		}
		for (i = 0; i < MapRegions.Count(); i++) {
			delete MapRegions[i]->NeighborRegions;
			MapRegions[i]->NeighborRegions = NULL;
		}
	}
	return(true);
}


/// <summary>
/// Builds the list of regions bordering this one.
/// This routine is called by Make_Regions before any connections are laid out, since
/// deciding where a bridge or a ramp should go means knowing which regions touch. The
/// region's cell tally is brought back up to date on the way through.
/// </summary>
void MapRegionClass::Build_Neighbor_Regions(void)
{
	NeighborRegions = new DynamicVectorClass<int>;
	unsigned char * seen = new unsigned char[TotalCount]; // RA2: RA2's form
	int index; // RA2: as above
	for (index = 0; index < TotalCount; index++) { // RA2: as above
		seen[index] = 0; // RA2: as above
	}
	DynamicVectorClass<Cell> * boundary = Build_Border_Cell_List(); // RA2: as above
	for (index = 0; index < boundary->Count(); index++) { // RA2: as above
		Cell cell = (*boundary)[index]; // RA2: as above
		for (int direction = 0; direction < 8; direction++) { // RA2: as above
			Cell next = cell + AdjacentCell[direction & 7]; // RA2: as above
			if (next.Y + next.X > MapStartDiagonal && next.X - next.Y < MapStartDiagonal && // RA2: as above
			    next.Y - next.X < MapStartDiagonal && next.Y + next.X <= MapEndDiagonal) { // RA2: as above
				if (RMGCellData) { // RA2: as above
					int regionIndex = RMGCellData[next.Y * MapCellStride + next.X].RegionID; // RA2: as above
					if (regionIndex >= 0) { // RA2: as above
						seen[regionIndex] = 1; // RA2: as above
					}
				}
			}
		}
	}
	for (index = 0; index < TotalCount; index++) { // RA2: as above
		if (seen[index] && index != ID) { // RA2: as above
			NeighborRegions->Add(index); // RA2: as above
		}
	}
	CellCount = 0; // RA2: as above
	int totalCells = MapCellStride * MapCellStride; // RA2: as above
	for (index = 0; index < totalCells; index++) { // RA2: as above
		if (RMGCellData[index].RegionID == ID && // RA2: as above
		    RMGCellData[index].CellID != Cell(0, 0)) { // RA2: as above
			CellCount++; // RA2: as above
		}
	}
	delete boundary; // RA2: as above
	delete[] seen; // RA2: as above
}



/// <summary>
/// Connects two regions with a bridge over the water.
/// This routine is used when a stretch of water separates two pieces of land. It hunts
/// for a straight span whose ends land in the two regions given, lays a low bridge and
/// its approach roads along it, and drops a repair hut at either end. The search gives
/// up rather than blocking if the water proves too awkward to cross.
/// </summary>
/// <param name="region1">First region to connect.</param>
/// <param name="region2">Second region to connect.</param>
/// <returns>bool; Was a bridge placed?</returns>
bool MapRegionClass::Connect_Regions_With_Bridge(MapRegionClass *region1,MapRegionClass *region2) // RA2: RA2's form of the bridge connector (0x570BC0)
{
	int id1 = region1->ID;
	int id2 = region2->ID;

	int tries = 0;
	bool result = false;

	for (; tries < 200; tries++) { // RA2: as above
		if (result) {
			break;
		}

		Cell cell = Pick_Random_Cell();

		Rect rect1(cell.X - 1, cell.Y, 3, 1);
		Rect rect2=rect1; // RA2: as above
		Rect rect4(cell.X, cell.Y - 1, 1, 3);
		Rect rect3=rect4; // RA2: as above

		bool allow_rect3 = true;
		bool allow_rect1 = true;

		while (!MapGeneratorClass::Can_Place_Paved_Road(rect1, false, false)) {
			rect1.Y--;
			Cell cell1(rect1.X, rect1.Y); // RA2: as above
			Cell cell2(rect1.X + 2, rect1.Y); // RA2: as above
			CellClass * cptr1 = &Map[cell1];
			CellClass * cptr2 = &Map[cell2];
			if (!Map.In_Local_Radar(cell1,true) || !Map.In_Local_Radar(cell2,true) || cptr1->Is_Tile_Cliff() || cptr2->Is_Tile_Cliff()) { // RA2: as above
				allow_rect1 = false;
				break;
			}
		}

		if (allow_rect1) {
			if (MapGeneratorClass::Can_Place_Paved_Road(Rect(rect1.X, rect1.Y - 3, 3, 3), false, false)) {
				while (!MapGeneratorClass::Can_Place_Paved_Road(rect2, false, false)) {
					rect2.Y++;
					Cell cell1(rect2.X, rect2.Y); // RA2: as above
					Cell cell2(rect2.X + 2, rect2.Y); // RA2: as above
					CellClass * cptr1 = &Map[cell1];
					CellClass * cptr2 = &Map[cell2];
					if (!Map.In_Local_Radar(cell1,true) || !Map.In_Local_Radar(cell2,true) || cptr1->Is_Tile_Cliff() || cptr2->Is_Tile_Cliff()) { // RA2: as above
						allow_rect1 = false;
						break;
					}
				}

				if (allow_rect1) {
					if (!MapGeneratorClass::Can_Place_Paved_Road(Rect(rect2.X, rect2.Y + 1, 3, 3), false, false)) {
						allow_rect1 = false;
					}
				}
			} else {
				allow_rect1 = false;
			}
		}

		while (!MapGeneratorClass::Can_Place_Paved_Road(rect3, false, false)) {
			rect3.X--;
			Cell cell1(rect3.X, rect3.Y); // RA2: as above
			Cell cell2(rect3.X, rect3.Y + 2); // RA2: as above
			CellClass * cptr1 = &Map[cell1];
			CellClass * cptr2 = &Map[cell2];
			if (!Map.In_Local_Radar(cell1,true) || !Map.In_Local_Radar(cell2,true) || cptr1->Is_Tile_Cliff() || cptr2->Is_Tile_Cliff()) { // RA2: as above
				allow_rect3 = false;
				break;
			}
		}

		if (allow_rect3) {
			if (MapGeneratorClass::Can_Place_Paved_Road(Rect(rect3.X - 3, rect3.Y, 3, 3), false, false)) {
				while (!MapGeneratorClass::Can_Place_Paved_Road(rect4, false, false)) {
					rect4.X++;
					Cell cell1(rect4.X, rect4.Y); // RA2: as above
					Cell cell2(rect4.X, rect4.Y + 2); // RA2: as above
					CellClass * cptr1 = &Map[cell1];
					CellClass * cptr2 = &Map[cell2];
					if (!Map.In_Local_Radar(cell1,true) || !Map.In_Local_Radar(cell2,true) || cptr1->Is_Tile_Cliff() || cptr2->Is_Tile_Cliff()) { // RA2: as above
						allow_rect3 = false;
						break;
					}
				}

				if (allow_rect3) {
					if (!MapGeneratorClass::Can_Place_Paved_Road(Rect(rect4.X + 1, rect4.Y, 3, 3), false, false)) {
						allow_rect3 = false;
					}
				}
			} else {
				allow_rect3 = false;
			}
		}

		int len1 = 999;
		int len2 = 999;
		int region_a = -1;
		int region_b = -1;
		int region_c = -1;
		int region_d = -1;

		if (allow_rect1) {
			len1 = abs(rect2.Y - rect1.Y);
			region_a = CellData::Get_Region(Cell(rect1.X, rect1.Y)); // RA2: as above
			region_b = CellData::Get_Region(Cell(rect2.X, rect2.Y)); // RA2: as above
		}

		if (allow_rect3) {
			len2 = abs(rect4.X - rect3.X);
			region_c = CellData::Get_Region(Cell(rect4.X, rect4.Y)); // RA2: as above
			region_d = CellData::Get_Region(Cell(rect3.X, rect3.Y)); // RA2: as above
		}

		if (allow_rect1) {
			if ((region_a != id1 || region_b != id2) && (region_a != id2 || region_b != id1)) {
				allow_rect1 = false;
			}
		}

		if (allow_rect3) {
			if ((region_c != id1 || region_d != id2) && (region_c != id2 || region_d != id1)) { // RA2: as above
				allow_rect3 = false;
			}
			if (allow_rect3 && allow_rect1) { // RA2: as above
				if (len1 < len2) {
					allow_rect3 = false;
				} else {
					allow_rect1 = false;
				}
			}
		}

		Rect bridge(0, 0, 0, 0);
		int max_len = tries / 25 + 8;

		if (allow_rect3 && len2 < max_len) {
			bridge = Rect(rect3.X, rect3.Y, rect4.X - rect3.X + 1, 3);
		} else if (allow_rect1 && len1 < max_len) {
			bridge = Rect(rect1.X, rect1.Y, 3, rect2.Y - rect1.Y + 1);
		}

		if (bridge.Is_Valid() && allow_rect3) { // RA2: as above
			if (Is_Bridge_Allowed(bridge)) {
				for (int y = bridge.Y; y < bridge.Y + bridge.Height; y++) {
					for (int x = bridge.X; x < bridge.X + bridge.Width; x++) {
						if (x == bridge.X) {
							Map[Cell(x, y)].Overlay = OVERLAY_LOWBRIDGE_21;
						} else if (x == bridge.X + bridge.Width - 1) {
							Map[Cell(x, y)].Overlay = OVERLAY_LOWBRIDGE_19;
						} else {
							Map[Cell(x, y)].Overlay = (OverlayType)(OVERLAY_LOWBRIDGE_01 + (x % 4));
						}
						Map[Cell(x, y)].OverlayData = y - bridge.Y;
					}
				}

				if (MapGeneratorClass::Can_Place_Paved_Road_End(Rect(bridge.X + bridge.Width, bridge.Y - 2, 6, 6), false) && Pick_Random_UInt(0,1)) { // RA2: as above
					MapGeneratorClass::Place_Tile((IsometricTileType)(IsometricTileTypeClass::PavedRoads + 10), Cell(bridge.X + bridge.Width, bridge.Y));
				} else {
					MapGeneratorClass::Place_Tile(IsometricTileTypeClass::PavedRoadEnds, Cell(bridge.X + bridge.Width, bridge.Y));
				}

				if (MapGeneratorClass::Can_Place_Paved_Road_End(Rect(bridge.X - 6, bridge.Y - 2, 6, 6), false) && Pick_Random_UInt(0,1)) { // RA2: as above
					MapGeneratorClass::Place_Tile((IsometricTileType)(IsometricTileTypeClass::PavedRoads + 9), Cell(bridge.X - 4, bridge.Y));
				} else {
					MapGeneratorClass::Place_Tile((IsometricTileType)(IsometricTileTypeClass::PavedRoadEnds + 2), Cell(bridge.X - 1, bridge.Y));
				}

				if (!Place_Bridge_Hut(Rect(bridge.X, bridge.Y - 1, 2, 5))) {
					Place_Bridge_Hut(Rect(bridge.X - 1, bridge.Y - 2, 3, 7));
				}

				if (!Place_Bridge_Hut(Rect(bridge.X + bridge.Width - 2, bridge.Y - 1, 2, 5))) {
					Place_Bridge_Hut(Rect(bridge.X + bridge.Width - 2, bridge.Y - 2, 3, 7));
				}

				result = true;
			}
		} else if (bridge.Is_Valid()) { // RA2: as above
			if (allow_rect1 && Is_Bridge_Allowed(bridge)) { // RA2: as above
				for (int y = bridge.Y; y < bridge.Y + bridge.Height; y++) {
					for (int x = bridge.X; x < bridge.X + bridge.Width; x++) {
						if (y == bridge.Y) {
							Map[Cell(x, y)].Overlay = OVERLAY_LOWBRIDGE_23;
						} else if (y == bridge.Y + bridge.Height - 1) {
							Map[Cell(x, y)].Overlay = OVERLAY_LOWBRIDGE_25;
						} else {
							Map[Cell(x, y)].Overlay = (OverlayType)(OVERLAY_LOWBRIDGE_10 + (y % 4));
						}
						Map[Cell(x, y)].OverlayData = x - bridge.X;
					}
				}

				if (MapGeneratorClass::Can_Place_Paved_Road_End(Rect(bridge.X - 2, bridge.Y - 6, 7, 6), false) && Pick_Random_UInt(0,1)) { // RA2: as above
					MapGeneratorClass::Place_Tile((IsometricTileType)(IsometricTileTypeClass::PavedRoads + 13), Cell(bridge.X, bridge.Y - 4));
				} else {
					MapGeneratorClass::Place_Tile((IsometricTileType)(IsometricTileTypeClass::PavedRoadEnds + 1), Cell(bridge.X, bridge.Y - 1));
				}

				if (MapGeneratorClass::Can_Place_Paved_Road_End(Rect(bridge.X - 2, bridge.Y + bridge.Height, 7, 6), false) && Pick_Random_UInt(0,1)) { // RA2: as above
					MapGeneratorClass::Place_Tile((IsometricTileType)(IsometricTileTypeClass::PavedRoads + 12), Cell(bridge.X, bridge.Y + bridge.Height));
				} else {
					MapGeneratorClass::Place_Tile((IsometricTileType)(IsometricTileTypeClass::PavedRoadEnds + 3), Cell(bridge.X, bridge.Y + bridge.Height));
				}

				if (!Place_Bridge_Hut(Rect(bridge.X - 1, bridge.Y, 5, 2))) {
					Place_Bridge_Hut(Rect(bridge.X - 2, bridge.Y - 1, 7, 3));
				}

				if (!Place_Bridge_Hut(Rect(bridge.X - 1, bridge.Y + bridge.Height - 2, 5, 2))) {
					Place_Bridge_Hut(Rect(bridge.X - 2, bridge.Y + bridge.Height - 2, 7, 3));
				}

				result = true;
			}
		}

	}

	return(result);
}



/// <summary>
/// Determines if a bridge may be built here.
/// This routine is used by the bridge layer to vet a candidate span before committing to
/// it. The ground must lie on the map, and be unobstructed, level, and either clear or
/// water throughout.
/// </summary>
/// <param name="rect">The candidate bridge footprint.</param>
/// <returns>bool; May the bridge be built?</returns>
bool MapRegionClass::Is_Bridge_Allowed(Rect const & rect) const
{
	int height = Map[Cell(rect.X, rect.Y)].Height;

	if (My_In_Radar(Cell(rect.X, rect.Y)) &&
		My_In_Radar(Cell(rect.X + rect.Width - 1, rect.Y)) &&
		My_In_Radar(Cell(rect.X, rect.Y + rect.Height - 1)) &&
		My_In_Radar(Cell(rect.X + rect.Width - 1,rect.Y + rect.Height - 1))) {
		for (int y = rect.Y; y < rect.Y + rect.Height + 1; y++) {
			for (int x = rect.X; x < rect.X + rect.Width + 1; x++) {
				CellClass *cptr = &Map[Cell(x,y)];
				if (cptr->Overlay != OVERLAY_NONE) {
					return(false);
				}
				if (cptr->Height != height) {
					return(false);
				}
				if (!cptr->Is_Tile_Clear() && !cptr->Is_Tile_With_Water()) {
					return(false);
				}
			}
		}
		return(true);
	}
	return(false);
}


/// <summary>
/// Places a repair hut at the end of a bridge.
/// This routine hunts within the given area for somewhere the hut will actually fit and
/// builds it there for the neutral house. Only the one hut is placed.
/// </summary>
/// <param name="rect">The area to search for a buildable cell.</param>
/// <returns>bool; Was a hut placed?</returns>
bool MapRegionClass::Place_Bridge_Hut(Rect const & rect) const
{
	for (int y = rect.Y; y < rect.Y + rect.Height + 1; y++) {
		for (int x = rect.X; x < rect.X + rect.Width + 1; x++) {
			CellClass *cptr = &Map[Cell(x,y)];
			if (cptr->Overlay == OVERLAY_NONE && cptr->Is_Tile_Clear() && cptr->Cell_Occupier() == NULL) {
				HouseClass *hptr = House_From_HousesType(HouseTypeClass::From_Name("Neutral"));
				StructType caba = BuildingTypeClass::From_Name("CABHUT");
				BuildingClass *thehut = new BuildingClass(BuildingTypes[caba], hptr);
				thehut->Unlimbo(cptr->CellID);
				return(true);
			}
		}
	}
	return(false);
}


/// <summary>
/// Creates the connections from this region to its neighbors.
/// This routine is called once per region by Make_Regions. A water region spans itself
/// with bridges to link the dry land on either side; dry land instead carves ramps down
/// to each lower neighbor, as many of them as the map's accessibility setting will bear.
/// A region that ends up with nothing joining it to its neighbors is flagged as
/// unconnected.
/// </summary>
void MapRegionClass::Make_Region_Connections(void)
{
	if (ContainsWater) {
		for (int i = 0; i < NeighborRegions->Count() - 1; i++) {
			MapRegionClass * region = From_ID((*NeighborRegions)[i]);
			if (region->NeighborRegions->Count() > 1 || region->CellCount > 50) {
				for (int k = i + 1; k < NeighborRegions->Count(); k++) {
					MapRegionClass * region2 = From_ID((*NeighborRegions)[k]);
					if ((region2->NeighborRegions->Count() > 1 || region2->CellCount > 50) && !region2->ContainsWater) {
						if (region2->CellHeight == region->CellHeight && region->CellHeight == CellHeight) {
							Connect_Regions_With_Bridge(region, region2);
						}
					}
				}
			}
		}
	} else {
		for (int i = 0; i < NeighborRegions->Count(); i++) {
			MapRegionClass * region = From_ID((*NeighborRegions)[i]);
			if (region->ID > ID && region->CellHeight != CellHeight) {
				int total = 0;
				int target = ((int)Pick_Random_UInt(0, 100) < RandomMapGen.SeedData.Accessibility ? Pick_Random_UInt(1, 2) : 0) + 1;

				int tries = 0;

				int id1 = region->CellHeight > CellHeight ? region->ID : ID;
				int id2 = region->CellHeight > CellHeight ? ID : region->ID;

				MapRegionClass * region2 = From_ID(id1);
				DynamicVectorClass<Cell> * cells = region2->Build_Border_Cell_List();

				while (total < target && tries < 100) {
					Cell cell = (*cells)[Pick_Random_UInt(0, cells->Count() - 1)];
					if (Map.In_Local_Radar(cell)) {
						total += region2->Connect_Regions_With_Ramp(cell, id2, (float)tries * 0.01f) ? 1 : 0; // RA2: the third argument is a float density rising by 0.01 per try (0x7A5DF0), not a flag after 50 tries
					}
					tries++;
				}
				if (total == 0) {
					HasConnections = false;
				}

				delete cells;
			}
		}
	}
}



/// <summary>
/// Carves a ramp joining this region to a lower neighbor.
/// This routine is called by the region connector for each candidate cell along the
/// boundary. It looks at which way the region continues around the cell, picks the ramp
/// orientation that suits -- corner ramps first, then the straight ones -- and hands the
/// digging to the matching builder. A caller that has run out of patience may ask for
/// the fallback, which tries the four straight ramps at default endpoints instead.
/// </summary>
/// <param name="cell">The cell around which to attempt the ramp.</param>
/// <param name="region_id">The region on the far side of the height change.</param>
/// <param name="allow_fallback">Should default ramp placements be tried if nothing suits?</param>
/// <returns>bool; Was a ramp carved?</returns>
bool MapRegionClass::Connect_Regions_With_Ramp(Cell const &cell,int region_id, float density) // RA2: RA2's form of the ramp connector (0x572270): a float density, the neighbor mask as a bit set, do/while(0) with break once a ramp is built
{
	bool valid=true; // RA2: as above
	unsigned mask=Build_Neighbor_Region_Mask(cell,valid,region_id,density); // RA2: as above
	if(!valid)return(false); // RA2: as above
	bool result=false; // RA2: as above
	if(mask==(MG_FACINGF_N | MG_FACINGF_NE | MG_FACINGF_E | MG_FACINGF_SE | MG_FACINGF_S | MG_FACINGF_SW | MG_FACINGF_W | MG_FACINGF_NW))return(false); // RA2: as above
	do {
	Cell first,second; // RA2: as above
	if((mask & (MG_FACINGF_N | MG_FACINGF_E))==(MG_FACINGF_N | MG_FACINGF_E) && (mask & (MG_FACINGF_S | MG_FACINGF_SW | MG_FACINGF_W))==0) { // RA2: as above
		first.X=cell.X-1;first.Y=cell.Y-5; // RA2: as above
		second.X=cell.X+5;second.Y=cell.Y+1; // RA2: as above
		if(mask & MG_FACINGF_NW)++first.Y; // RA2: as above
		if(mask & MG_FACINGF_SE)--second.X; // RA2: as above
		result=Build_Corner_Ramp_SW(first,second,region_id); // RA2: as above
		if(result)break; // RA2: as above
	}
	if((mask & (MG_FACINGF_E | MG_FACINGF_S))==(MG_FACINGF_E | MG_FACINGF_S) && (mask & (MG_FACINGF_NE | MG_FACINGF_SW))==0) { // RA2: as above
		first.X=cell.X+5;first.Y=cell.Y-1; // RA2: as above
		second.X=cell.X-1;second.Y=cell.Y+5; // RA2: as above
		if(mask & MG_FACINGF_SW)--second.Y; // RA2: as above
		if(mask & MG_FACINGF_NE)--first.X; // RA2: as above
		result=Build_Corner_Ramp_NW(first,second,region_id); // RA2: as above
		if(result)break; // RA2: as above
	}
	if((mask & (MG_FACINGF_S | MG_FACINGF_W))==(MG_FACINGF_S | MG_FACINGF_W) && (mask & (MG_FACINGF_N | MG_FACINGF_NE | MG_FACINGF_E))==0) { // RA2: as above
		first.X=cell.X+1;first.Y=cell.Y+5; // RA2: as above
		second.X=cell.X-5;second.Y=cell.Y-1; // RA2: as above
		if(mask & MG_FACINGF_NW)--second.X; // RA2: as above
		if(mask & MG_FACINGF_SE)--first.Y; // RA2: as above
		result=Build_Corner_Ramp_NE(first,second,region_id); // RA2: as above
		if(result)break; // RA2: as above
	}
	if((mask & (MG_FACINGF_N | MG_FACINGF_W))==(MG_FACINGF_N | MG_FACINGF_W) && (mask & (MG_FACINGF_E | MG_FACINGF_SE | MG_FACINGF_S))==0) { // RA2: as above
		first.X=cell.X-5;first.Y=cell.Y+1; // RA2: as above
		second.X=cell.X+1;second.Y=cell.Y-5; // RA2: as above
		if(mask & MG_FACINGF_NE)++second.Y; // RA2: as above
		if(mask & MG_FACINGF_SW)++first.X; // RA2: as above
		result=Build_Corner_Ramp_NE(first,second,region_id); // RA2: as above
		if(result)break; // RA2: as above
	}
	if((mask & (MG_FACINGF_SW | MG_FACINGF_W | MG_FACINGF_NW))==0 && (mask & (MG_FACINGF_N | MG_FACINGF_S))!=0) { // RA2: as above
		first.X=cell.X-1;first.Y=cell.Y-4; // RA2: as above
		second.X=first.X;second.Y=cell.Y+4; // RA2: as above
		if(mask & MG_FACINGF_N) {++first.Y;++second.Y;} // RA2: as above
		if(mask & MG_FACINGF_S) {--second.Y;--first.Y;} // RA2: as above
		first.X+=(short)Pick_Random_UInt(0,1); // RA2: as above
		second.X+=(short)Pick_Random_UInt(0,1); // RA2: as above
		result=Build_West_Ramp(first,second,region_id); // RA2: as above
		if(result)break; // RA2: as above
	}
	if((mask & (MG_FACINGF_NE | MG_FACINGF_E | MG_FACINGF_SE))==0 && (mask & (MG_FACINGF_N | MG_FACINGF_S))!=0) { // RA2: as above
		first.X=cell.X+1;first.Y=cell.Y+4; // RA2: as above
		second.X=first.X;second.Y=cell.Y-4; // RA2: as above
		if(mask & MG_FACINGF_N) {++second.Y;++first.Y;} // RA2: as above
		if(mask & MG_FACINGF_S) {--first.Y;--second.Y;} // RA2: as above
		first.X+=(short)Pick_Random_UInt(0,1)-1; // RA2: as above
		second.X+=(short)Pick_Random_UInt(0,1)-1; // RA2: as above
		result=Build_East_Ramp(first,second,region_id); // RA2: as above
		if(result)break; // RA2: as above
	}
	if((mask & (MG_FACINGF_N | MG_FACINGF_NE | MG_FACINGF_NW))==0 && (mask & (MG_FACINGF_E | MG_FACINGF_W))!=0) { // RA2: as above
		first.X=cell.X+4;first.Y=cell.Y-1; // RA2: as above
		second.X=cell.X-4;second.Y=first.Y; // RA2: as above
		if(mask & MG_FACINGF_W) {++second.X;++first.X;} // RA2: as above
		if(mask & MG_FACINGF_E) {--first.X;--second.X;} // RA2: as above
		first.Y+=(short)Pick_Random_UInt(0,1); // RA2: as above
		second.Y+=(short)Pick_Random_UInt(0,1); // RA2: as above
		result=Build_North_Ramp(first,second,region_id); // RA2: as above
		if(result)break; // RA2: as above
	}
	if((mask & (MG_FACINGF_SE | MG_FACINGF_S | MG_FACINGF_SW))==0 && (mask & (MG_FACINGF_E | MG_FACINGF_W))!=0) { // RA2: as above
		first.X=cell.X-4;first.Y=cell.Y+1; // RA2: as above
		second.X=cell.X+4;second.Y=first.Y; // RA2: as above
		if(mask & MG_FACINGF_W) {++first.X;++second.X;} // RA2: as above
		if(mask & MG_FACINGF_E) {--second.X;--first.X;} // RA2: as above
		first.Y+=(short)Pick_Random_UInt(0,1)-1; // RA2: as above
		second.Y+=(short)Pick_Random_UInt(0,1)-1; // RA2: as above
		result=Build_South_Ramp(first,second,region_id); // RA2: as above
	}
	} while(0); // RA2: as above
	if(density>0.5f && !result) { // RA2: as above
		Cell first,second; // RA2: as above
		if((mask & MG_FACINGF_S)==0) { // RA2: as above
			first.X=cell.X-4;first.Y=cell.Y+1; // RA2: as above
			second.X=cell.X+4;second.Y=cell.Y+1; // RA2: as above
			result=Build_South_Ramp(first,second,region_id); // RA2: as above
			if(result)return result; // RA2: as above
		}
		if((mask & MG_FACINGF_E)==0) { // RA2: as above
			first.X=cell.X+1;first.Y=cell.Y+4; // RA2: as above
			second.X=cell.X+1;second.Y=cell.Y-4; // RA2: as above
			result=Build_East_Ramp(first,second,region_id); // RA2: as above
			if(result)return result; // RA2: as above
		}
		if((mask & MG_FACINGF_S)==0) { // RA2: as above
			first.X=cell.X+4;first.Y=cell.Y-1; // RA2: as above
			second.X=cell.X-4;second.Y=cell.Y-1; // RA2: as above
			result=Build_North_Ramp(first,second,region_id); // RA2: as above
			if(result)return result; // RA2: as above
		}
		if((mask & MG_FACINGF_W)==0) { // RA2: as above
			first.X=cell.X-1;first.Y=cell.Y-4; // RA2: as above
			second.X=cell.X-1;second.Y=cell.Y+4; // RA2: as above
			result=Build_West_Ramp(first,second,region_id); // RA2: as above
			if(result)return result; // RA2: as above
		}
	}
	return result; // RA2: as above
}



/// <summary>
/// Determines which way this region continues around a cell.
/// This routine samples the eight patches of ground surrounding the cell and reports
/// which of them still belong to this region. Connect_Regions_With_Ramp uses the answer
/// to pick a ramp orientation, and the site is refused outright when the ground under
/// the cell itself is barely part of the region at all.
/// </summary>
/// <param name="cell">The cell at the center of the area to sample.</param>
/// <param name="valid">Cleared when the site is unsuitable for a ramp.</param>
/// <returns>Returns with a mask of the facings in which the region continues, or zero
/// if the site was refused.</returns>
unsigned MapRegionClass::Build_Neighbor_Region_Mask(Cell const & cell, bool & valid, int unknown, float density) // RA2: a fourth parameter, the density scaling the required cell count
{
	static const double _count_minimum = 5.0; // RA2: static constants (0x7A5C10, 0x7A5C18), not folded literals
	static const double _count_maximum = 15.0; // RA2: as above
	int required = (int)((_count_maximum - _count_minimum) * (1.0f - density) + _count_minimum); // RA2: the bounds are static constants, not folded literals; the required count scales with the density
	Cell c = (cell - Cell(2, 2));
	int ox = c.X;
	int oy = c.Y;

	if (!MapGeneratorClass::Region_Cell_Count_At_Least(Rect(ox, oy, 5, 5), ID, unknown, required)) { // RA2: the computed count instead of 6
		valid = false;
		return(0);
	}

	unsigned mask = 0;
	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			if (y != 1 || x != 1) {
				if (MapGeneratorClass::Region_Cell_Count_At_Least(Rect(ox + 5 * (x - 1), oy + 5 * (y - 1), 5, 5), ID, unknown, required)) { // RA2: as above
					/// One bit per neighboring direction, clockwise from the northeast, laid out
					/// as a 3x3 grid so that it can be indexed by the offsets of the scan below.
					static int _facing_bits[3][3] = {
						MG_FACINGF_NW, MG_FACINGF_N, MG_FACINGF_NE,
						MG_FACINGF_W, 0, MG_FACINGF_E,
						MG_FACINGF_SW, MG_FACINGF_S, MG_FACINGF_SE
					};
					mask |= _facing_bits[y][x];
				}
			}
		}
	}

	return(mask);
}



/// <summary>
/// Carves a south-facing transition ramp between two cells.
/// This routine cuts one of the four straight ramps the region connector uses to join
/// ground at differing heights. The ramp runs west to east with the low ground lying to
/// the south of it. The east, north and west builders are this same routine turned a
/// quarter at a time.
/// </summary>
/// <param name="cell1">Left endpoint of the ramp.</param>
/// <param name="cell2">Right endpoint of the ramp.</param>
/// <param name="region_id">The region whose height the leveled ground adopts.</param>
/// <returns>bool; Was the ramp carved?</returns>
bool MapRegionClass::Build_South_Ramp(Cell const & cell1, Cell const & cell2, int region_id)
{
	/*
	 * Compute a bounding rect around the ramp area and validate it.
	 */
	Rect area(cell1.X - 5, (cell1.Y < cell2.Y ? cell1.Y : cell2.Y) - 3, cell2.X - cell1.X + 9, (cell1.Y > cell2.Y ? cell1.Y : cell2.Y) - (cell1.Y < cell2.Y ? cell1.Y : cell2.Y) + 11); // RA2: RA2's form
	if (!Is_Ramp_Area_Clear(area, region_id)) { // RA2: as above
		return(false);
	}

	/*
	 * The ramp body drifts sideways as it runs; reject if the drift is too
	 * large to fit within the horizontal span.
	 */
	int drift = cell2.Y - cell1.Y; // RA2: as above
	int span = abs(drift); // RA2: as above
	int width = cell2.X - cell1.X; // RA2: as above
	if (span > width - 5) { // RA2: as above
		return(false);
	}

	int half = (width + 1) / 2 + 3; // RA2: as above
	Flatten_Area(Rect(area.X, cell1.Y + 1, half + 2, area.Y + area.Height - cell1.Y + 1), region_id); // RA2: as above
	Flatten_Area(Rect(cell2.X - half + 3, cell2.Y + 1, half + 2, area.Y + area.Height - cell2.Y + 1), region_id); // RA2: as above
	Flatten_Area(Rect(area.X + 4, area.Y + 2, half - 2, cell1.Y - area.Y - 1)); // RA2: as above
	Flatten_Area(Rect(cell2.X - half + 2, area.Y + 2, half - 1, cell2.Y - area.Y - 1)); // RA2: as above

	/*
	 * Place the two slope set pieces at the ramp ends.
	 */
	MapGeneratorClass::Place_Tile(IsometricTileTypeClass::SlopeSetPieces, cell1, region_id, CellHeight);
	MapGeneratorClass::Place_Tile((IsometricTileType)(IsometricTileTypeClass::SlopeSetPieces + 1), cell2 - Cell(2, 0), region_id, CellHeight);

	Cell tail(0, 0);
	if (drift != 0) { // RA2: as above
		int pick = Pick_Random_UInt(0, 1); // RA2: as above
		Cell bias;
		bias.X = 0;
		int down = drift > 0; // RA2: as above
		bias.Y = drift < 0 ? -1 : 0; // RA2: as above
		if (pick == 1) { // RA2: as above
			tail += Cell(span, drift); // RA2: as above
		} else {
			bias.X = (cell2 - Cell(span, 0)).X - cell1.X - 5; // RA2: as above
		}
		static int _heights[] = {3, 2, 1, 0, 0}; // RA2: as above
		static int _ramps[] = {11, 15, 15, 15, 7, 12, 16, 16, 16, 8}; // RA2: as above
		for (int forward = 0; forward < span; forward++) { // RA2: as above
			for (int row = 0; row < 5; row++) { // RA2: as above
				short offset = down ? forward : -forward; // RA2: as above
				Cell position = Cell(cell1.X + 3, cell1.Y) + Cell(forward, row) + bias; // RA2: as above
				position = Cell(position.X, position.Y + offset); // RA2: as above
				CellClass &target = Map[position]; // RA2: as above
				target.Height = (char)(CellHeight + _heights[row] - 4); // RA2: as above
				target.Ramp = (unsigned char)_ramps[5 * down + row]; // RA2: as above
				target.ITType = (IsometricTileType)((unsigned)IsometricTileTypeClass::RampStart + target.Ramp - 1); // RA2: as above
				target.SubTile = 0; // RA2: as above
			}
		}
	}

	for (int column = 0; column < cell2.X - cell1.X - span - 5; column++) { // RA2: as above
		for (int row = 0; row < 4; row++) { // RA2: as above
			Cell position = Cell(cell1.X + 3, cell1.Y) + Cell(column, row) + tail; // RA2: as above
			CellData::Set_Region(position, region_id); // RA2: as above
			CellClass &target = Map[position]; // RA2: as above
			target.Height = (char)(CellHeight - (char)row - 1); // RA2: as above
			target.Ramp = 4; // RA2: as above
			target.ITType = (IsometricTileType)((unsigned)IsometricTileTypeClass::RampStart + 3u); // RA2: as above
			target.SubTile = 0; // RA2: as above
		}
	}
	return(true);
}



/// <summary>
/// Carves an east-facing transition ramp between two cells.
/// This routine cuts one of the four straight ramps the region connector uses to join
/// ground at differing heights. The ramp runs south to north with the low ground lying
/// to the east of it. See Build_South_Ramp for the rest of the family.
/// </summary>
/// <param name="cell1">Bottom endpoint of the ramp.</param>
/// <param name="cell2">Top endpoint of the ramp.</param>
/// <param name="region_id">The region whose height the leveled ground adopts.</param>
/// <returns>bool; Was the ramp carved?</returns>
bool MapRegionClass::Build_East_Ramp(Cell const &first,Cell const &second,int lowRegion) // RA2: RA2's form of the east ramp (0x573040), with the ramp height and shape tables of 0x7E0A28 / 0x7E0A3C
{
	static int RampHeights[] = {3, 2, 1, 0, 0}; // RA2: the table at 0x7E0A28
	static int RampShapes[] = {10, 14, 14, 14, 6, 11, 15, 15, 15, 7}; // RA2: the table at 0x7E0A3C
	Rect area((first.X<second.X?first.X:second.X)-3,second.Y-4,(first.X>second.X?first.X:second.X)-(first.X<second.X?first.X:second.X)+11,first.Y-second.Y+9); // RA2: as above
	if(!Is_Ramp_Area_Clear(area,lowRegion)) { // RA2: as above
		return(false);
	}
	int drift=second.X-first.X; // RA2: as above
	int span=abs(drift); // RA2: as above
	int height=first.Y-second.Y; // RA2: as above
	if(span>height-5) { // RA2: as above
		return(false);
	}
	int half=(height+1)/2+2; // RA2: as above
	Flatten_Area(Rect(first.X+1,first.Y-half+2,7,half+3),lowRegion); // RA2: as above
	Flatten_Area(Rect(second.X+1,area.Y,7,half+2),lowRegion); // RA2: as above
	Flatten_Area(Rect(area.X+2,first.Y-half+2,first.X-area.X-1,half-1)); // RA2: as above
	Flatten_Area(Rect(area.X+2,second.Y,second.X-area.X-1,half-2)); // RA2: as above
	MapGeneratorClass::Place_Tile((IsometricTileType)(IsometricTileTypeClass::SlopeSetPieces + 2),first-Cell(0,2),lowRegion,CellHeight); // RA2: as above
	MapGeneratorClass::Place_Tile((IsometricTileType)(IsometricTileTypeClass::SlopeSetPieces + 3),second,lowRegion,CellHeight); // RA2: as above
	Cell tail(0,0); // RA2: as above
	if(drift!=0) { // RA2: as above
		int pick=Pick_Random_UInt(0,1); // RA2: as above
		int down=drift>0; // RA2: as above
		Cell bias(drift<0?-1:0,0); // RA2: as above
		if(pick==1) { // RA2: as above
			tail+=Cell(drift,-span); // RA2: as above
		} else {
			bias-=Cell(0,first.Y-(short)span-second.Y-5); // RA2: as above
		}
		for(int forward=0;forward<span;forward++) { // RA2: as above
			for(int row=0;row<5;row++) { // RA2: as above
				short offset=down?forward:-forward; // RA2: as above
				Cell position=Cell(first.X,first.Y-3)+Cell(row,-forward)+bias; // RA2: as above
				position=Cell(position.X+offset,position.Y); // RA2: as above
				CellClass &target=Map[position]; // RA2: as above
				target.Height=(char)(RampHeights[row] + CellHeight - 4); // RA2: as above
				target.Ramp=(unsigned char)RampShapes[5*down+row]; // RA2: as above
				target.ITType=(IsometricTileType)((unsigned)IsometricTileTypeClass::RampStart + target.Ramp - 1); // RA2: as above
				target.SubTile=0; // RA2: as above
			}
		}
	}
	for(int column=0;column<first.Y-second.Y-span-5;column++) { // RA2: as above
		for(int row=0;row<4;row++) { // RA2: as above
			Cell position=Cell(first.X,first.Y-3)+Cell(row,-column)+tail; // RA2: as above
			CellData::Set_Region(position,lowRegion); // RA2: as above
			CellClass &target=Map[position]; // RA2: as above
			target.Height=(char)(CellHeight-(char)row-1); // RA2: as above
			target.Ramp=3; // RA2: as above
			target.ITType=(IsometricTileType)((unsigned)IsometricTileTypeClass::RampStart + 2u); // RA2: as above
			target.SubTile=0; // RA2: as above
		}
	}
	return(true);
}



/// <summary>
/// Carves a north-facing transition ramp between two cells.
/// This routine cuts one of the four straight ramps the region connector uses to join
/// ground at differing heights. The ramp runs east to west with the low ground lying to
/// the north of it. See Build_South_Ramp for the rest of the family.
/// </summary>
/// <param name="cell1">East endpoint of the ramp.</param>
/// <param name="cell2">West endpoint of the ramp.</param>
/// <param name="region_id">The region whose height the leveled ground adopts.</param>
/// <returns>bool; Was the ramp carved?</returns>
bool MapRegionClass::Build_North_Ramp(Cell const &first,Cell const &second,int lowRegion) // RA2: RA2's form of the north ramp (0x573680), with the ramp height and shape tables of 0x7E0A64 / 0x7E0A78
{
	static int RampHeights[] = {3, 2, 1, 0, 0}; // RA2: the table at 0x7E0A64
	static int RampShapes[] = {10, 14, 14, 14, 6, 9, 13, 13, 13, 5}; // RA2: the table at 0x7E0A78
	Rect area(second.X-4,(first.Y<second.Y?first.Y:second.Y)-7,first.X-second.X+8,(first.Y>second.Y?first.Y:second.Y)-(first.Y<second.Y?first.Y:second.Y)+11); // RA2: as above
	if(!Is_Ramp_Area_Clear(area,lowRegion)) { // RA2: as above
		return(false);
	}
	int drift=second.Y-first.Y; // RA2: as above
	int span=abs(drift); // RA2: as above
	int width=first.X-second.X; // RA2: as above
	if(span>width-5) { // RA2: as above
		return(false);
	}
	int half=(width+1)/2+3; // RA2: as above
	Flatten_Area(Rect(area.X,area.Y,half+2,second.Y-area.Y),lowRegion); // RA2: as above
	Flatten_Area(Rect(first.X-half+2,area.Y,half+3,first.Y-area.Y),lowRegion); // RA2: as above
	Flatten_Area(Rect(second.X,second.Y,half-2,area.Y+area.Height-second.Y-2)); // RA2: as above
	Flatten_Area(Rect(first.X-half+2,first.Y,half-1,area.Y+area.Height-first.Y-2)); // RA2: as above
	MapGeneratorClass::Place_Tile((IsometricTileType)(IsometricTileTypeClass::SlopeSetPieces + 4),second-Cell(0,3),lowRegion,CellHeight); // RA2: as above
	MapGeneratorClass::Place_Tile((IsometricTileType)(IsometricTileTypeClass::SlopeSetPieces + 5),first-Cell(2,3),lowRegion,CellHeight); // RA2: as above
	MapGeneratorClass::Place_Tile((IsometricTileType)(IsometricTileTypeClass::SlopeSetPieces + 6),first-Cell(1,1),lowRegion,CellHeight); // RA2: as above
	Cell tail(0,0); // RA2: as above
	if(drift!=0) { // RA2: as above
		int pick=Pick_Random_UInt(0,1); // RA2: as above
		Cell bias;
		bias.X=0; // RA2: as above
		int down=drift>0; // RA2: as above
		bias.Y=drift<0; // RA2: as above
		if(pick==1) { // RA2: as above
			tail+=Cell(span,-drift); // RA2: as above
		} else {
			bias.X=(first-Cell(span,0)).X-second.X-5; // RA2: as above
		}
		for(int forward=0;forward<span;forward++) { // RA2: as above
			for(int row=0;row<5;row++) { // RA2: as above
				short offset=down?-forward:forward; // RA2: as above
				Cell position=Cell(second.X+3,second.Y)+Cell(forward,-row)+bias; // RA2: as above
				position=Cell(position.X,position.Y+offset); // RA2: as above
				CellClass &target=Map[position]; // RA2: as above
				short height=RampHeights[row]; // RA2: ra2-decomp src/mapgen.cpp keeps the height in a short
				target.Height=height + CellHeight - 4; // RA2: as above
				target.Ramp=(unsigned char)RampShapes[5*down+row]; // RA2: as above
				target.ITType=(IsometricTileType)((unsigned)IsometricTileTypeClass::RampStart + target.Ramp - 1); // RA2: as above
				target.SubTile=0; // RA2: as above
			}
		}
	}
	for(int column=0;column<first.X-second.X-span-5;column++) { // RA2: as above
		for(int row=0;row<4;row++) { // RA2: as above
			Cell position=Cell(second.X+3,second.Y)+Cell(column,-row)+tail; // RA2: as above
			CellData::Set_Region(position,lowRegion); // RA2: as above
			CellClass &target=Map[position]; // RA2: as above
			target.Height=(char)(CellHeight-(char)row-1); // RA2: as above
			target.Ramp=2; // RA2: as above
			target.ITType=(IsometricTileType)((unsigned)IsometricTileTypeClass::RampStart + 1u); // RA2: as above
			target.SubTile=0; // RA2: as above
		}
	}
	return(true);
}



/// <summary>
/// Carves a west facing ramp between two regions.
/// One of the four straight ramp builders, each a duplicate of the same algorithm turned a
/// quarter circle. This routine runs the slope north to south with the low ground off to its
/// west, levels the ground either side of it, and jogs sideways as far as it must to meet
/// both endpoints.
/// </summary>
/// <param name="cell1">The northern endpoint of the ramp.</param>
/// <param name="cell2">The southern endpoint of the ramp.</param>
/// <param name="region_id">The region on the low side, whose height the leveled ground
/// takes.</param>
/// <returns>bool; Was the ramp carved?</returns>
bool MapRegionClass::Build_West_Ramp(Cell const &first,Cell const &second,int lowRegion) // RA2: RA2's form of the west ramp (0x573D40)
{
	static int RampHeights[] = {3, 2, 1, 0, 0}; // RA2: the table at 0x7E0AA0
	static int RampShapes[] = {9, 13, 13, 13, 5, 12, 16, 16, 16, 8}; // RA2: the table at 0x7E0AB4
	Rect area((first.X<second.X?first.X:second.X)-7,first.Y-4,(first.X>second.X?first.X:second.X)-(first.X<second.X?first.X:second.X)+11,second.Y-first.Y+9); // RA2: as above
	if(!Is_Ramp_Area_Clear(area,lowRegion)) { // RA2: as above
		return(false);
	}
	int drift=second.X-first.X; // RA2: as above
	int span=abs(drift); // RA2: as above
	if(span>second.Y-first.Y-5) { // RA2: as above
		return(false);
	}
	int half=(second.Y-first.Y+1)/2+2; // RA2: as above
	Flatten_Area(Rect(area.X,area.Y,first.X-area.X,half+2),lowRegion); // RA2: as above
	Flatten_Area(Rect(area.X,second.Y-half+3,second.X-area.X,half+2),lowRegion); // RA2: as above
	Flatten_Area(Rect(first.X,first.Y,area.X+area.Width-first.X-1,half-1)); // RA2: as above
	Flatten_Area(Rect(second.X,second.Y-half+3,area.X+area.Width-second.X-1,half-2)); // RA2: as above
	MapGeneratorClass::Place_Tile((IsometricTileType)(IsometricTileTypeClass::SlopeSetPieces + 7),first-Cell(3,0),lowRegion,CellHeight); // RA2: as above
	MapGeneratorClass::Place_Tile((IsometricTileType)(IsometricTileTypeClass::SlopeSetPieces + 8),second-Cell(3,2),lowRegion,CellHeight); // RA2: as above
	MapGeneratorClass::Place_Tile((IsometricTileType)(IsometricTileTypeClass::SlopeSetPieces + 9),second-Cell(1,1),lowRegion,CellHeight); // RA2: as above
	Cell tail(0,0); // RA2: as above
	if(drift!=0) { // RA2: as above
		int pick=Pick_Random_UInt(0,1); // RA2: as above
		int down=drift>0; // RA2: as above
		Cell bias(drift>0,0); // RA2: as above
		if(pick==1) { // RA2: as above
			tail+=Cell(drift,span); // RA2: as above
		} else {
			bias+=Cell(0,second.Y-(short)span-first.Y-5); // RA2: as above
		}
		for(int forward=0;forward<span;forward++) { // RA2: as above
			for(int row=0;row<5;row++) { // RA2: as above
				short offset=down?forward:-forward; // RA2: as above
				Cell position=Cell(first.X,first.Y+3)+Cell(-row,forward)+bias; // RA2: as above
				position=Cell(position.X+offset,position.Y); // RA2: as above
				CellClass &target=Map[position]; // RA2: as above
				target.Height=(char)(RampHeights[row] + CellHeight - 4); // RA2: as above
				target.Ramp=(unsigned char)RampShapes[5*down+row]; // RA2: as above
				target.ITType=(IsometricTileType)((unsigned)IsometricTileTypeClass::RampStart + target.Ramp - 1); // RA2: as above
				target.SubTile=0; // RA2: as above
			}
		}
	}
	for(int column=0;column<second.Y-first.Y-span-5;column++) { // RA2: as above
		for(int row=0;row<4;row++) { // RA2: as above
			Cell position=Cell(first.X,first.Y+3)+Cell(-row,column)+tail; // RA2: as above
			CellData::Set_Region(position,lowRegion); // RA2: as above
			CellClass &target=Map[position]; // RA2: as above
			target.Height=(char)(CellHeight-(char)row-1); // RA2: as above
			target.Ramp=1; // RA2: as above
			target.ITType=(IsometricTileType)((unsigned)IsometricTileTypeClass::RampStart); // RA2: as above
			target.SubTile=0; // RA2: as above
		}
	}
	return(true);
}



/// <summary>
/// Carves a ramp around the southeast corner of the high ground.
/// One of the four corner ramp builders. This routine joins the high ground to the low
/// ground lying off to its south and east, with a diagonal slope turning the corner between
/// the two straight faces. The corner must measure at least three cells on either arm.
/// </summary>
/// <param name="cell1">The west endpoint of the corner, upon the south facing arm.</param>
/// <param name="cell2">The north endpoint of the corner, upon the east facing arm.</param>
/// <param name="region_id">The region on the low side, whose height the leveled ground
/// takes.</param>
/// <returns>bool; Was the corner ramp carved?</returns>
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
bool MapRegionClass::Build_Corner_Ramp_SE(Cell const & cell1, Cell const & cell2, int region_id)
{
	int dx = cell2.X - cell1.X;
	if (dx >= 3) {
		int dy = cell1.Y - cell2.Y;
		if (dy >= 3) {
			Rect rect(cell1.X - 2, cell2.Y - 2, dx + 10, dy + 10);
			if (Is_Ramp_Area_Clear(rect, region_id)) {

				Flatten_Area(Rect(rect.X, cell1.Y + 1, rect.Width, 7), region_id);
				Flatten_Area(Rect(cell2.X + 1, rect.Y, 7, rect.Height), region_id);
				Flatten_Area(Rect(cell1.X, cell1.Y - 1, 1, 2));
				Flatten_Area(Rect(cell2.X - 1, cell2.X, 2, 1));
				Flatten_Area(Rect(cell1.X + 1, cell2.Y + 1, cell2.X - cell1.X - 1, cell1.Y - cell2.Y - 1));

				MapGeneratorClass::Place_Tile(IsometricTileTypeClass::SlopeSetPieces, cell1, region_id, CellHeight);
				MapGeneratorClass::Place_Tile((IsometricTileType)(IsometricTileTypeClass::SlopeSetPieces + 3), cell2, region_id, CellHeight);

				for (int i = 0; i < 4; ++i) {
					CellClass * cptr = &Map[Cell(cell2.X + i, cell1.Y + i)];
					cptr->Height = CellHeight - (char)i - 1;
					cptr->Ramp = 7;
					cptr->ITType = (IsometricTileType)(IsometricTileTypeClass::RampStart + 6);
					cptr->SubTile = 0;
				}

				for (int j = 0; j < 4; ++j) {
					for (int k = 0; k < cell2.X - cell1.X + j - 3; ++k) {
						CellClass * cptr = &Map[Cell(cell1.X + k + 3, cell1.Y + j)];
						cptr->Height = CellHeight - (char)j - 1;
						cptr->Ramp = 4;
						cptr->ITType = (IsometricTileType)(IsometricTileTypeClass::RampStart + 3);
						cptr->SubTile = 0;
					}
				}

				for (int m = 0; m < 4; ++m) {
					for (int n = 0; n < cell1.Y - cell2.Y + m - 3; ++n) {
						CellClass * cptr = &Map[Cell(cell2.X + m, cell2.Y + n + 3)];
						cptr->Height = CellHeight - (char)m - 1;
						cptr->Ramp = 3;
						cptr->ITType = (IsometricTileType)(IsometricTileTypeClass::RampStart + 2);
						cptr->SubTile = 0;
					}
				}

				return(true);
			}
		}
	}
	return(false);
}
#endif


/// <summary>
/// Carves a ramp around the northeast corner of the high ground.
/// One of the four corner ramp builders. This routine joins the high ground to the low
/// ground lying off to its north and east, with a diagonal slope turning the corner between
/// the two straight faces. The corner must measure at least three cells on either arm.
/// </summary>
/// <param name="cell1">The lower right endpoint of the corner.</param>
/// <param name="cell2">The upper left endpoint of the corner.</param>
/// <param name="region_id">The region on the low side, whose height the leveled ground
/// takes.</param>
/// <returns>bool; Was the corner ramp carved?</returns>
bool MapRegionClass::Build_Corner_Ramp_NE(Cell const &first,Cell const &second,int region) // RA2: RA2's form of the NE corner ramp (0x574930)
{
	int dx=first.X-second.X; // RA2: as above
	if(dx>=3) { // RA2: as above
		int dy=first.Y-second.Y; // RA2: as above
		if(dy>=3) { // RA2: as above
			int top=second.Y-7; // RA2: as above
			Rect area(second.X-2,top,dx+10,first.Y-second.Y+10); // RA2: as above
			if(Is_Ramp_Area_Clear(area,region)) { // RA2: as above
				Flatten_Area(Rect(area.X,area.Y,area.Width,7),region); // RA2: as above
				Flatten_Area(Rect(first.X+1,area.Y,7,area.Height),region); // RA2: as above
				Flatten_Area(Rect(second.X,second.Y,1,2)); // RA2: as above
				Flatten_Area(Rect(first.X-1,first.Y,2,1)); // RA2: as above
				Flatten_Area(Rect(second.X+1,second.Y+1,first.X-second.X-1,first.Y-second.Y-1)); // RA2: as above
				MapGeneratorClass::Place_Tile((IsometricTileType)(IsometricTileTypeClass::SlopeSetPieces + 2),first-Cell(0,2),region,CellHeight); // RA2: as above
				MapGeneratorClass::Place_Tile((IsometricTileType)(IsometricTileTypeClass::SlopeSetPieces + 4),second-Cell(0,3),region,CellHeight); // RA2: as above
				for(int corner=0;corner<4;++corner) { // RA2: as above
					CellClass &cell=Map[Cell(first.X+corner,second.Y-corner)]; // RA2: as above
					cell.Height=(char)(CellHeight-(char)corner-1); // RA2: as above
					cell.Ramp=6; // RA2: as above
					cell.ITType=(IsometricTileType)((unsigned)IsometricTileTypeClass::RampStart + 5); // RA2: as above
					cell.SubTile=0; // RA2: as above
				}
				for(int east=0;east<4;++east) { // RA2: as above
					for(int y=0;y<first.Y-second.Y+east-3;++y) { // RA2: as above
						CellClass &cell=Map[Cell(first.X+east,first.Y-y-3)]; // RA2: as above
						cell.Height=(char)(CellHeight-(char)east-1); // RA2: as above
						cell.Ramp=3; // RA2: as above
						cell.ITType=(IsometricTileType)((unsigned)IsometricTileTypeClass::RampStart + 2); // RA2: as above
						cell.SubTile=0; // RA2: as above
					}
				}
				for(int north=0;north<4;++north) { // RA2: as above
					for(int x=0;x<first.X-second.X+north-3;++x) { // RA2: as above
						CellClass &cell=Map[Cell(second.X+x+3,second.Y-north)]; // RA2: as above
						cell.Height=(char)(CellHeight-(char)north-1); // RA2: as above
						cell.Ramp=2; // RA2: as above
						cell.ITType=(IsometricTileType)((unsigned)IsometricTileTypeClass::RampStart + 1); // RA2: as above
						cell.SubTile=0; // RA2: as above
					}
				}
				return(true);
			}
		}
	}
	return(false);
}



/// <summary>
/// Carves a ramp around the northwest corner of the high ground.
/// One of the four corner ramp builders. This routine joins the high ground to the low
/// ground lying off to its north and west, with a diagonal slope turning the corner between
/// the two straight faces. The corner must measure at least three cells on either arm.
/// </summary>
/// <param name="cell2">The upper right endpoint of the corner.</param>
/// <param name="cell1">The lower left endpoint of the corner.</param>
/// <param name="region_id">The region on the low side, whose height the leveled ground
/// takes.</param>
/// <returns>bool; Was the corner ramp carved?</returns>
bool MapRegionClass::Build_Corner_Ramp_NW(Cell const &first,Cell const &second,int region) // RA2: RA2's form of the NW corner ramp (0x574E50): one Rect per flattened strip, the slope pieces and the three tile loops
{
	int dx=first.X-second.X; // RA2: as above
	if(dx>=3) { // RA2: as above
		int dy=second.Y-first.Y; // RA2: as above
		if(dy>=3) { // RA2: as above
			Rect area(second.X-7,first.Y-7,dx+10,dy+10); // RA2: as above
			if(Is_Ramp_Area_Clear(area,region)) { // RA2: as above
				Flatten_Area(Rect(area.X,area.Y,area.Width,7),region); // RA2: as above
				Flatten_Area(Rect(area.X,area.Y,7,area.Height),region); // RA2: as above
				Flatten_Area(Rect(first.X,first.Y,1,2)); // RA2: as above
				Flatten_Area(Rect(second.X,second.Y,2,1)); // RA2: as above
				Flatten_Area(Rect(second.X+1,first.Y+1,first.X-second.X-1,second.Y-first.Y-1)); // RA2: as above
				MapGeneratorClass::Place_Tile((IsometricTileType)(IsometricTileTypeClass::SlopeSetPieces + 5),first-Cell(2,3),region,CellHeight); // RA2: as above
				MapGeneratorClass::Place_Tile((IsometricTileType)(IsometricTileTypeClass::SlopeSetPieces + 6),first-Cell(1,1),region,CellHeight); // RA2: as above
				MapGeneratorClass::Place_Tile((IsometricTileType)(IsometricTileTypeClass::SlopeSetPieces + 8),second-Cell(3,2),region,CellHeight); // RA2: as above
				MapGeneratorClass::Place_Tile((IsometricTileType)(IsometricTileTypeClass::SlopeSetPieces + 9),second-Cell(1,1),region,CellHeight); // RA2: as above
				for(int corner=0;corner<4;++corner) { // RA2: as above
					CellClass &cell=Map[Cell(second.X-corner,first.Y-corner)]; // RA2: as above
					cell.Height=(char)(CellHeight-(char)corner-1); // RA2: as above
					cell.Ramp=5; // RA2: as above
					cell.ITType=(IsometricTileType)((unsigned)IsometricTileTypeClass::RampStart + 4); // RA2: as above
					cell.SubTile=0; // RA2: as above
				}
				for(int north=0;north<4;++north) { // RA2: as above
					for(int x=0;x<first.X-second.X+north-3;++x) { // RA2: as above
						CellClass &cell=Map[Cell(first.X-x-3,first.Y-north)]; // RA2: as above
						cell.Height=(char)(CellHeight-(char)north-1); // RA2: as above
						cell.Ramp=2; // RA2: as above
						cell.ITType=(IsometricTileType)((unsigned)IsometricTileTypeClass::RampStart + 1); // RA2: as above
						cell.SubTile=0; // RA2: as above
					}
				}
				for(int west=0;west<4;++west) { // RA2: as above
					for(int y=0;y<second.Y-first.Y+west-3;++y) { // RA2: as above
						CellClass &cell=Map[Cell(second.X-west,second.Y-y-3)]; // RA2: as above
						cell.Height=(char)(CellHeight-(char)west-1); // RA2: as above
						cell.Ramp=1; // RA2: as above
						cell.ITType=(IsometricTileType)((unsigned)IsometricTileTypeClass::RampStart); // RA2: as above
						cell.SubTile=0; // RA2: as above
					}
				}
				return(true);
			}
		}
	}
	return(false);
}



/// <summary>
/// Carves a ramp around the southwest corner of the high ground.
/// One of the four corner ramp builders. This routine joins the high ground to the low
/// ground lying off to its south and west, with a diagonal slope turning the corner between
/// the two straight faces. The corner must measure at least three cells on either arm.
/// </summary>
/// <param name="cell1">The upper left endpoint of the corner.</param>
/// <param name="cell2">The lower right endpoint of the corner.</param>
/// <param name="region_id">The region on the low side, whose height the leveled ground
/// takes.</param>
/// <returns>bool; Was the corner ramp carved?</returns>
bool MapRegionClass::Build_Corner_Ramp_SW(Cell const &first,Cell const &second,int region) // RA2: RA2's form of the SW corner ramp (0x5753F0)
{
	int dx=second.X-first.X; // RA2: as above
	if(dx>=3) { // RA2: as above
		int dy=second.Y-first.Y; // RA2: as above
		if(dy>=3) { // RA2: as above
			int top=first.Y-4; // RA2: as above
			Rect area(first.X-7,top,dx+10,second.Y-first.Y+10); // RA2: as above
			if(Is_Ramp_Area_Clear(area,region)) { // RA2: as above
				Flatten_Area(Rect(area.X,area.Y,7,area.Height),region); // RA2: as above
				Flatten_Area(Rect(area.X,second.Y+1,area.Width,7),region); // RA2: as above
				Flatten_Area(Rect(first.X,first.Y,2,1)); // RA2: as above
				Flatten_Area(Rect(second.X,second.Y-1,1,2)); // RA2: as above
				Flatten_Area(Rect(first.X+1,first.Y+1,second.X-first.X-1,second.Y-first.Y-1)); // RA2: as above
				MapGeneratorClass::Place_Tile((IsometricTileType)(IsometricTileTypeClass::SlopeSetPieces + 7),first-Cell(3,0),region,CellHeight); // RA2: as above
				MapGeneratorClass::Place_Tile((IsometricTileType)(IsometricTileTypeClass::SlopeSetPieces + 1),second-Cell(2,0),region,CellHeight); // RA2: as above
				for(int corner=0;corner<4;++corner) { // RA2: as above
					CellClass &cell=Map[Cell(first.X-corner,second.Y+corner)]; // RA2: as above
					cell.Height=(char)(CellHeight-(char)corner-1); // RA2: as above
					cell.Ramp=8; // RA2: as above
					cell.ITType=(IsometricTileType)((unsigned)IsometricTileTypeClass::RampStart + 7); // RA2: as above
					cell.SubTile=0; // RA2: as above
				}
				for(int west=0;west<4;++west) { // RA2: as above
					for(int y=0;y<second.Y-first.Y+west-3;++y) { // RA2: as above
						CellClass &cell=Map[Cell(first.X-west,first.Y+y+3)]; // RA2: as above
						cell.Height=(char)(CellHeight-(char)west-1); // RA2: as above
						cell.Ramp=1; // RA2: as above
						cell.ITType=(IsometricTileType)((unsigned)IsometricTileTypeClass::RampStart); // RA2: as above
						cell.SubTile=0; // RA2: as above
					}
				}
				for(int south=0;south<4;++south) { // RA2: as above
					for(int x=0;x<second.X-first.X+south-3;++x) { // RA2: as above
						CellClass &cell=Map[Cell(second.X-x-3,second.Y+south)]; // RA2: as above
						cell.Height=(char)(CellHeight-(char)south-1); // RA2: as above
						cell.Ramp=4; // RA2: as above
						cell.ITType=(IsometricTileType)((unsigned)IsometricTileTypeClass::RampStart + 3); // RA2: as above
						cell.SubTile=0; // RA2: as above
					}
				}
				return(true);
			}
		}
	}
	return(false);
}



/// <summary>
/// Is there room upon the map to carve a ramp here?
/// Every ramp builder asks this routine before it disturbs a single cell, so that a ramp is
/// never left half carved. The area must lie on the playable map, be clear of anything
/// already laid down, and belong to no region but the two being joined.
/// </summary>
/// <param name="rect">The area the ramp would occupy.</param>
/// <param name="id">The other region allowed to hold cells here, besides this one.</param>
/// <returns>bool; Is the area free for the ramp?</returns>
bool MapRegionClass::Is_Ramp_Area_Clear(Rect & rect, int id)
{
	if (Map.In_Local_Radar(Cell(rect.X, rect.Y)) &&
		Map.In_Local_Radar(Cell(rect.X + rect.Width - 1, rect.Y)) &&
		Map.In_Local_Radar(Cell(rect.X, rect.Y + rect.Height - 1)) &&
		Map.In_Local_Radar(Cell(rect.X + rect.Width - 1,rect.Y + rect.Height - 1))) {

		for (int y = rect.Y; y < rect.Y + rect.Height; y++) {
			for (int x = rect.X; x < rect.X + rect.Width; x++) {
				Cell cell(x, y);
				int rid = CellData::Get_Region(cell);
				if (rid != ID && rid != id) {
					return(false);
				}
				CellClass * cptr = &Map[cell];
				if (!cptr->Is_Tile_Clear()) {
					return(false);
				}
			}
		}
		return(true);
	}
	return(false);
}


/// <summary>
/// Levels an area of ground into another region.
/// This is the counterpart of the one argument version, used by the ramp builders to prepare
/// the ground on the far side of a slope -- the side belonging to the region being joined to
/// rather than to this one.
/// </summary>
/// <param name="rect">The area of cells to level.</param>
/// <param name="region_id">The region to claim the cells for, and to take the height from.</param>
void MapRegionClass::Flatten_Area(Rect const & rect, int region_id)
{
	for (int y = rect.Y; y < rect.Y + rect.Height; y++) {
		for (int x = rect.X; x < rect.X + rect.Width; x++) {
			Cell cell(x, y);
			if (My_In_Radar(cell)) {
				CellData::Set_Region(cell, region_id);
				CellClass * cellptr = &Map[cell];
				cellptr->Height = From_ID(region_id)->CellHeight;
			}
		}
	}
}


/// <summary>
/// Levels an area of ground into this region.
/// The cells are claimed by this region and dragged to its height. This routine is used by
/// the ramp builders to prepare the flat ground on the near side of a slope.
/// </summary>
/// <param name="rect">The area of cells to level.</param>
void MapRegionClass::Flatten_Area(Rect const & rect)
{
	for (int y = rect.Y; y < rect.Y + rect.Height; y++) {
		for (int x = rect.X; x < rect.X + rect.Width; x++) {
			Cell const position(x, y); // RA2: a const Cell named position, and the cell reference in a local, as RA2's form; needed for the inlined copies in the ramp builders
			if (My_In_Radar(position)) { // RA2: as above
				CellData::Set_Region(position, ID); // RA2: as above
				CellClass & target = Map[position]; // RA2: as above
				target.Height = CellHeight; // RA2: as above
			}
		}
	}
}



/// <summary>
/// Fetches a region by the number it is known by.
/// The cell data grid records nothing but region numbers, so every routine that works from
/// the grid comes back through here to reach the region itself.
/// </summary>
/// <param name="id">The region number to look up.</param>
/// <returns>Returns with a pointer to the region. Otherwise, NULL is returned if no region
/// carries that number.</returns>
MapRegionClass * MapRegionClass::From_ID(int id)
{
	for (int i = 0; i < MapRegions.Count(); i++) {
		if (MapRegions[i]->ID == id) {
			return(MapRegions[i]);
		}
	}
	return(NULL);
}


/// <summary>
/// RA2: is the cell within the playable diamond? (My_In_Radar with RA2's register use)
/// </summary>
static inline bool In_Zone_Map_Bounds(Cell const & cell)
{
	int x = cell.X;
	int y = cell.Y;
	int diagonal = y + cell.X;
	return diagonal > MapRegionClass::MapStartDiagonal &&
		x - y < MapRegionClass::MapStartDiagonal &&
		y - x < MapRegionClass::MapStartDiagonal &&
		diagonal <= MapRegionClass::MapEndDiagonal;
}


/// <summary>
/// Makes a region of the passable ground of one zone, grown out from a seed cell.
/// This routine gathers up every passable cell reachable from the seed that lies in the
/// same movement zone, so that the start points can be shared out over the pieces of the
/// map the players can actually reach.
/// </summary>
/// <param name="cptr">The cell to grow the region out from.</param>
/// <param name="mzone">The kind of movement the zones are reckoned for.</param>
/// <param name="zone">The zone the region is to cover.</param>
void MapRegionClass::Create_Zone_Region(CellClass * cptr, MZoneType mzone, int zone) // RA2: not in TibSun, at 0x575D20
{
	MapRegionClass * region = new MapRegionClass(cptr->CellID); // RA2
	int region_id = region->ID; // RA2

	DynamicVectorClass<Cell> cells; // RA2
	cells.Set_Growth_Step(20000); // RA2
	CellData::Set_Spread(cptr->CellID, region_id); // RA2
	cells.Add(cptr->CellID); // RA2

	while (cells.Count() > 0) { // RA2
		Cell cell = cells[cells.Count() - 1]; // RA2
		cells.Delete_Index(cells.Count() - 1); // RA2
		MapRegionClass::Get_Cell_Data(cell).RegionID = region_id; // RA2
		region->CellCount++; // RA2
		region->Cells.Add(cell); // RA2

		for (int i = FACING_FIRST; i < FACING_COUNT; i++) { // RA2
			CellClass & other = Map[AdjacentCell[i & 7] + cell]; // RA2
			Cell const & ncell = other.CellID; // RA2
			if (In_Zone_Map_Bounds(other.CellID)) { // RA2
				CellData & ndata = MapRegionClass::Get_Cell_Data(ncell); // RA2
				if (other.Passability == PASSABLE_LAND && Map.Get_Cell_Zone(ncell, mzone, false) == zone && ndata.SpreadID == -1) { // RA2
					ndata.SpreadID = region_id; // RA2
					cells.Add(ncell); // RA2
				} // RA2
			} // RA2
		} // RA2
	} // RA2
}


/// <summary>
/// Remakes the regions so that each covers a piece of passable ground in one zone.
/// The regions built while shaping the terrain are thrown away first.
/// </summary>
/// <param name="mzone">The kind of movement the zones are reckoned for.</param>
/// <param name="zone">The zone the regions are to cover.</param>
void MapRegionClass::Create_Zone_Regions(MZoneType mzone, int zone) // RA2: not in TibSun, at 0x576000
{
	Destroy_Cell_Regions(); // RA2
	Map.Reset_Iterator(); // RA2
	for (CellClass * cptr = Map.Iterate(); cptr != NULL; cptr = Map.Iterate()) { // RA2
		Cell const & cell = cptr->CellID; // RA2
		if (My_In_Radar(cptr->CellID)) { // RA2
			if (Get_Cell_Data(cell).RegionID == -1 && cptr->Passability == PASSABLE_LAND && Map.Get_Cell_Zone(cell, mzone, false) == zone) { // RA2
				Create_Zone_Region(cptr, mzone, zone); // RA2
			} // RA2
		} // RA2
	} // RA2
}




/// <summary>
/// Places the start points shared out to this region.
/// This routine draws candidate spots for the bases at random from the region's cells,
/// keeping the ones with room for a base well inside the map, and then picks the best spread
/// of them. The first picks become the waypoints of the start points; the rest are kept in
/// StartCells, where the tiberium fields are later grown.
/// </summary>
/// <param name="first_waypoint">The waypoint of this region's first start point.</param>
void MapRegionClass::Place_Start_Points(int first_waypoint) // RA2: not in TibSun, at 0x576170
{
	Rect bounds = Map.LocalRect; // RA2
	bounds.X += 4; // RA2
	bounds.Y += 4; // RA2
	bounds.Width -= 8; // RA2
	bounds.Height -= 8; // RA2
	DynamicVectorClass<Cell> cells; // RA2

	unsigned int extra; // RA2
	do { // RA2
		extra = (unsigned int)RMGRandom() * (3.0 / 4294967295.0); // RA2
	} while (extra > 2); // RA2
	int wanted = StartCount * 15 + extra; // RA2

	for (int tries = 0; cells.Count() < wanted && tries < 300; tries++) { // RA2
		unsigned int last = Cells.Count() - 1; // RA2
		unsigned int pick; // RA2
		do { // RA2
			pick = RMG_RANDOM_DOUBLE(last + 1); // RA2
		} while (pick > last); // RA2
		Cell cell = Cells[pick]; // RA2
		Rect rect(cell.X - 3, cell.Y - 3, 6, 6); // RA2
		if (MapGeneratorClass::Can_Place_Paved_Road(rect, false, false) && Map.In_Area_Radar(bounds, cell, true)) { // RA2
			cells.Add(cell); // RA2
		} // RA2
	} // RA2

	DynamicVectorClass<Cell> * layout = Pick_Spread_Cells(cells); // RA2
	if (layout == NULL) { // RA2
		return; // RA2
	} // RA2

	int i; // RA2
	for (i = 0; i < StartCount; i++) { // RA2
		Scen->Set_Waypoint(first_waypoint + i, (*layout)[i]); // RA2
		Map[Scen->Get_Waypoint_Cell(first_waypoint + i)].IsWaypoint = true; // RA2
	} // RA2
	for (i = 0; i < StartCount; i++) { // RA2
		layout->Delete_Index(0); // RA2
	} // RA2

	delete StartCells; // RA2
	StartCells = layout; // RA2
}


/// <summary>
/// Picks the player start locations for a random map.
/// This routine remakes the regions so that each covers a piece of ground the players can
/// reach one another across (by land, or on the island maps by water too), throws away the
/// pieces too small to hold a base, and shares the players out over what is left in
/// proportion to its size, the largest pieces first. Each region then places its own share
/// of start points. On an archipelago a region left without players may be given a neutral
/// tech building.
/// </summary>
/// <returns>bool; Was a start location found for every player? RA2 always says so.</returns>
bool Generate_Starting_Points(void); // RA2: the definition is not exact yet
#ifdef RA2_NON_MATCHING // not exact: no independent bound yet (00576420 has no row)
bool Generate_Starting_Points(void)
{
	MZoneType mzones[5] = {MZONE_AMPHIBIOUS, MZONE_AMPHIBIOUS, MZONE_AMPHIBIOUS, MZONE_NORMAL, MZONE_NORMAL}; // RA2: RA2's form (zone regions, shared out by size)
	int region_size_threshold = MAX(400.0, (double)RandomMapGen.LocalHeight * RandomMapGen.LocalWidth * 0.03); // RA2: as above
	int index = Map.Zone_Reset(); // RA2: as above
	MZoneType mzone = mzones[RandomMapGen.SeedData.MapType]; // RA2: as above
	int zone = Map.Zones[mzone][index]; // RA2: as above
	MapRegionClass::Create_Zone_Regions(mzone, zone); // RA2: as above

	int region_count = MapRegionClass::MapRegions.Count(); // RA2: as above
	int i; // RA2: as above
	for (i = MapRegionClass::MapRegions.Count() - 1; i >= 0; i--) { // RA2: as above
		if (MapRegionClass::MapRegions[i]->CellCount < region_size_threshold) { // RA2: as above
			delete MapRegionClass::MapRegions[i]; // RA2: as above
		} // RA2: as above
	} // RA2: as above

	DynamicVectorClass<MapRegionClass *> regions; // RA2: as above
	IndexClass<int, MapRegionClass *> sorter; // RA2: as above
	regions.Clear(); // RA2: as above

	/*
	** RA2: the biggest regions sort first, and regions of a size by their id.
	*/
	for (i = 0; i < MapRegionClass::MapRegions.Count(); i++) { // RA2: as above
		MapRegionClass * region = MapRegionClass::MapRegions[i]; // RA2: as above
		sorter.Add_Index((500000 - region->CellCount) * region_count + region->ID, MapRegionClass::MapRegions[i]); // RA2: as above
	} // RA2: as above
	for (i = 0; i < sorter.Count(); i++) { // RA2: as above
		regions.Add(sorter.Fetch_By_Position(i)); // RA2: as above
	} // RA2: as above

	int total_cells = 0; // RA2: as above
	for (i = 0; i < regions.Count(); i++) { // RA2: as above
		total_cells += regions[i]->CellCount; // RA2: as above
	} // RA2: as above

	int players = RandomMapGen.SeedData.NumPlayers; // RA2: as above
	int cells_so_far = 0; // RA2: as above
	int players_so_far = 0; // RA2: as above
	for (i = 0; i < regions.Count(); i++) { // RA2: as above
		MapRegionClass * region = regions[i]; // RA2: as above
		cells_so_far += region->CellCount; // RA2: as above
		int count; // RA2: as above
		if (i == regions.Count() - 1) { // RA2: as above
			count = players - players_so_far; // RA2: as above
		} else { // RA2: as above
			count = (double)cells_so_far / total_cells * players + 0.5 - players_so_far; // RA2: as above
		} // RA2: as above
		region->StartCount = count; // RA2: as above
		players_so_far += count; // RA2: as above
	} // RA2: as above

	int waypoint = 0; // RA2: as above
	for (i = 0; i < regions.Count(); i++) { // RA2: as above
		regions[i]->Place_Start_Points(waypoint); // RA2: as above
		waypoint += regions[i]->StartCount; // RA2: as above
	} // RA2: as above

	if (RandomMapGen.SeedData.MapType == 0) { // RA2: as above
		for (i = 0; i < regions.Count(); i++) { // RA2: as above
			if (regions[i]->StartCount == 0 && (unsigned int)RMGRandom() * (1.0 / 4294967295.0) < 0.5) { // RA2: as above
				regions[i]->Place_Neutral_Tech_Building(); // RA2: as above
			} // RA2: as above
		} // RA2: as above
	} // RA2: as above

	return(true);
}
#endif


/// <summary>
/// Chooses a well spread out subset of the candidate cells.
/// This routine is used to place the start points shared out to this region, and the
/// tiberium fields that go with them, so that they do not end up bunched together. Cells in
/// different regions are reckoned further apart than they really are, so the picks favor
/// separate pieces of ground. The richer the tiberium layout setting, the more cells are
/// picked beyond one per start point.
/// </summary>
/// <param name="cells">The candidate cells to choose from.</param>
/// <returns>Returns with a newly allocated list of the cells chosen (the caller owns it), or
/// NULL when this region has no start points and no candidates.</returns>
DynamicVectorClass<Cell> * MapRegionClass::Pick_Spread_Cells(DynamicVectorClass<Cell> const & cells) // RA2: a MapRegionClass member, picking for its own start points
{
	double layout_fraction = (double)RandomMapGen.SeedData.TiberiumLayout; // RA2: RA2's form (sized by StartCount)
	int start_count = StartCount; // RA2: as above
	layout_fraction *= 0.01; // RA2: as above
	double max_distance = -1.0;
	int max_cell_index = -1;
	layout_fraction *= 12.0; // RA2: as above
	int target_count = (int)((layout_fraction / (double)RandomMapGen.SeedData.NumPlayers + 2.0) * start_count); // RA2: as above
	if (start_count == 0 && cells.Count() > 0) { // RA2: as above
		target_count = cells.Count(); // RA2: as above
	} // RA2: as above
	if (target_count > cells.Count() || target_count == 0) { // RA2: as above
		if (start_count == 0) { // RA2: as above
			return(NULL); // RA2: as above
		} // RA2: as above
		target_count = cells.Count(); // RA2: as above
	} // RA2: as above

	DynamicVectorClass<Cell> *layout = new DynamicVectorClass<Cell>;

	int i; // RA2: as above
	for (i = 0; i < (int)((unsigned)cells.Count() - 1u); i++) { // RA2: as above
		Cell c1 = cells[i]; // RA2: as above
		int region1 = MapRegionClass::CellData::Get_Region(c1); // RA2: as above
		for (int j = i + 1; j < cells.Count(); j++) { // RA2: as above
			Cell c2 = cells[j]; // RA2: as above
			int dx = (int)c1.X - c2.X, dy = (int)c1.Y - c2.Y; // RA2: as above
			int region2 = MapRegionClass::CellData::Get_Region(c2); // RA2: as above
			int square = (int)((unsigned)dx * (unsigned)dx + (unsigned)dy * (unsigned)dy); // RA2: as above
			double score = (double)fastmath::sqrt((double)square) + (double)(region2 != region1 ? 20 : 0); // RA2: as above

			/*
			 * The check against zero here is always true; the inner cursor
			 * starts past the first cell.
			 */
			if (j && score > max_distance) { // RA2: as above
				max_distance = score; // RA2: as above
				max_cell_index = i; // RA2: as above
			} // RA2: as above
		} // RA2: as above
	} // RA2: as above

	if (max_cell_index != -1) { // RA2: as above
		layout->Add(cells[max_cell_index]);
	} else if (cells.Count() != 0) { // RA2: as above
		layout->Add(cells[0]); // RA2: as above
		return(layout); // RA2: as above
	} // RA2: as above

	while (layout->Count() < target_count) {
		double best_score = -1.0;
		int best_index = -1;

		for (i = 0; i < cells.Count(); i++) { // RA2: as above
			Cell c1 = cells[i]; // RA2: as above
			int region1 = MapRegionClass::CellData::Get_Region(c1);

			double min_score = 9999999.0;
			for (int j = 0; j < layout->Count(); j++) {
				Cell c2 = (*layout)[j];
				int dx = (int)c1.X - c2.X, dy = (int)c1.Y - c2.Y; // RA2: as above
				int region2 = MapRegionClass::CellData::Get_Region(c2);
				int square = (int)((unsigned)dx * (unsigned)dx + (unsigned)dy * (unsigned)dy); // RA2: as above
				double score = (double)fastmath::sqrt((double)square) + (double)(region2 != region1 ? 20 : 0); // RA2: as above
				if (score < min_score) {
					min_score = score;
				}
			}

			if (min_score > best_score) {
				best_score = min_score;
				best_index = i; // RA2: as above
			}
		}

		layout->Add(cells[best_index]);
	}

	return(layout);
}


/// <summary>
/// RA2: puts one of the rules' neutral tech buildings on a random spot of this region where its whole footprint is
/// clear, level, inside the map and not reserved; gives up after 100 tries.
/// </summary>
void MapRegionClass::Place_Neutral_Tech_Building(void)
{
	HouseClass * neutral = House_From_HousesType(HouseTypeClass::From_Name("Neutral"));
	bool placed = false;
	int tries = 0;
	BuildingClass * building = new BuildingClass(Rule->NeutralTechBuildings[Pick_Random_UInt(0, Rule->NeutralTechBuildings.Count() - 1)], neutral);

	for (; !placed; ++tries) {
		if (tries >= 100) {
			break;
		}

		Cell building_cell = Cells[Pick_Random_UInt(0, Cells.Count() - 1)];
		int height = Map[building_cell].Height;

		Cell const * list = building->Class->Occupy_List(true);
		bool ok = true;
		while (*list != REFRESH_EOL) {
			CellClass * cellptr = &Map[building_cell + *list];
			if (cellptr->Cell_Occupier() != NULL || !cellptr->Is_Tile_Clear() || cellptr->Height != height || cellptr->Land_Type() == LAND_ROCK || !Map.In_Local_Radar(cellptr) || !MapRegionClass::CellData::Is_Not_Inviolate(cellptr->CellID)) {
				ok = false;
			}
			list++;
			if (!ok) break;
		}

		if (ok) {
			if (building->Unlimbo(Coord(building_cell, 0))) {
				placed = true;
			}
		}
	}

	if (!placed) {
		delete building;
	}
}














/// <summary>
/// Creates a random map seed with the default settings.
/// This routine establishes the map parameters the random map dialog offers the player
/// before anything has been chosen, and gives the seed the file extension and description
/// that the load and save dialogs list it under.
/// </summary>
MapSeedClass::MapSeedClass(void) :
	Biome(0), // RA2: RA2's seed settings (YR's form: map type and resources; TibSun's Firestorm settings are not in RA2's MapSeedClass)
	MapType(1), // RA2: as above
	Resources(1), // RA2: as above
	Hills(0),
	Time(1), // RA2: as above
	WaterAmount(0),
	NumPlayers(2),
	Tiberium(0),
	TiberiumLayout(0),
	Vegetation(0),
	Cities(0),
	Width(0),
	Height(0),
	Accessibility(0),
	Cliffs(0),
	Seed(-1) // RA2: as above
{
	Extension = "SED";
	MapDescription[0] = L'\0'; // RA2: as above
	Description = MapDescription;
	if (StringTable::Is_Initialized()) { // RA2: the stock description, once there is a string table to take it from
		Set_Default_Description(); // RA2: as above
	} // RA2: as above
}


/// <summary>
/// Gives the settings the stock random map description.
/// A freshly randomized seed carries no description of its own, so this routine puts the
/// standard "random map" text in its place.
/// </summary>
void MapSeedClass::Set_Default_Description(void) // RA2: not in TibSun, at 0x576FE0
{
	wcscpy(MapDescription, StringTable::LoadString("TXT_RANDOM_MAP_DESCRIPTION", NULL, RA2_SOURCE, 3793)); // RA2
}







/// <summary>
/// Constructs the random map generator.
/// The game keeps a single generator for its whole run; this routine merely puts it into the
/// idle state it must be in before the first map is asked of it.
/// </summary>
#ifdef RA2_NON_MATCHING // not exact: no independent bound yet (00577010 has no row)
MapGeneratorClass::MapGeneratorClass(void) :
	SeedData(),
	MapSeeder(NULL),
	MapPreview(NULL),
	LocalWidth(0),
	LocalHeight(0),
	RMGMinimumTiberium(2500), // RA2: MapGeneratorClass at 0x577010 matches only so
	RMGMaximumTiberium(5500), // RA2: as above
	MaxTrees(500), // RA2: as above
	PreviewStale(true), // RA2: as above
	SeededWaterAmount(0),
	CellHeight(4), // RA2: as above
	PlaceWaterfall(false) // RA2: as above
{
}
#endif


/// <summary>
/// RA2
/// </summary>
static inline void Reset_Cell_Data(void)
{
	if (RMGCellData != NULL) {
		int grid_size = MapCellStride * MapCellStride;
		for (int i = 0; i < grid_size; i++) {
			RMGCellData[i].RegionID = -1;
			RMGCellData[i].SpreadID = -1;
		}
	}
}


/// <summary>
/// RA2
/// </summary>
static inline void Delete_Regions(void)
{
	for (int i = MapRegionClass::MapRegions.Count() - 1; i >= 0; i--) {
		delete MapRegionClass::MapRegions[i];
	}
}


/// <summary>
/// Destroys the random map generator.
/// This routine gives back the regions, the cell data grid, and the preview and seed objects
/// the generator accumulated while it was building maps.
/// </summary>
MapGeneratorClass::~MapGeneratorClass(void)
{
	Reset_Cell_Data(); // RA2: ~MapGeneratorClass at 0x5771B0 matches only so
	Delete_Regions(); // RA2: as above
	MapRegionClass::TotalCount = 0; // RA2: as above

	if (RMGCellData != NULL) {
		delete [] RMGCellData;
		RMGCellData = NULL;
	}

	if (MapSeeder != NULL) {
		delete MapSeeder;
		MapSeeder = NULL;
	}

	if (MapPreview != NULL) {
		delete MapPreview;
		MapPreview = NULL;
	}
}


/// <summary>
/// Runs the random map generator dialog.
/// Use this routine to let the player lay out and preview a random battlefield before a
/// skirmish or multiplayer game begins. It does not return until the player accepts the map
/// or gives up on it, and the title screen behind is kept alive in the meantime.
/// </summary>
/// <param name="callback">Progress callback to run while the dialog is up.</param>
/// <returns>Returns with the dialog result -- 1 if the player accepted the map, 2 if the
/// dialog was canceled, and 0 if it could not be opened at all.</returns>
int Do_Random_Map_Dialog(bool (*callback)())
{
	LONG res = 0; // RA2: no WDTTerritory, wdt (TibSun's use does not compile against RA2's declaration)


	RandomMapGen.Read_RMG_INI(); // RA2: the generator's rules are read before the dialog opens
	HWND dialog = OwnerDraw::Begin_Dialog(261, Map_Seed_Dialog_Proc); // RA2: RA2's map generator dialog; no IsWDT, WDTTerritory, wdt (TibSun's use does not compile against RA2's declaration)

	if (dialog) {
		RMGCallback = callback;
		RandomMapGen.SeedData.Callback = callback;
		SetWindowLongA(dialog, DWL_USER, (LONG)&res);
		SendMessageA(dialog, 0x4A9, 0, 1); // RA2: a message of RA2's dialogs (0x4A9, wparam 0, lparam 1)
		OwnerDraw::Display_Dialog(dialog);
		while (res == 0) {
			if (OwnerDraw::Dialog_Message_Handler() == 1) {
				break;
			}
			if (callback != NULL) {
				callback();
			}
			Title_Screen_Restore(false);
		}
		OwnerDraw::End_Dialog(dialog);
	}

	RMGCallback = MapGen_Call_Back;
	RandomMapGen.SeedData.Callback = NULL;

	if (RandomMapGen.MapPreview != NULL) {
		if (RandomMapGen.MapPreview->Get_Preview_Surface() != NULL) {
			RawFileClass file("RandMap.img");
			Write_PCX_File(file, *RandomMapGen.MapPreview->Get_Preview_Surface(), &GamePalette);
		}
		delete RandomMapGen.MapPreview;
		RandomMapGen.MapPreview = NULL;
	}

	if (RandomMapGen.MapSeeder != NULL) {
		delete RandomMapGen.MapSeeder;
		RandomMapGen.MapSeeder = NULL;
	}

	return(res);
}


/// <summary>
/// Prunes the random map preview cache.
/// This routine is called after a fresh preview has been cached. It throws away the previews
/// that have gone longest untouched, so that the cache directory cannot grow forever.
/// </summary>
void Clean_Up_RMCache(void)
{
	WIN32_FIND_DATA *ff;

	DynamicVectorClass<WIN32_FIND_DATA *> files;

	ff = new WIN32_FIND_DATA;
	HANDLE handle = FindFirstFile("rmcache\\*.mmp", ff);

	if (handle != INVALID_HANDLE_VALUE) {
		files.Add(ff);

		ff = new WIN32_FIND_DATA;
		while (FindNextFile(handle, ff) != 0) {
			files.Add(ff);
			ff = new WIN32_FIND_DATA;
		}
		FindClose(handle);
	}

	delete ff;

	while (files.Count() > 70) {
		FILETIME oldesttime;
		oldesttime.dwLowDateTime = 0xFFFFFFFF;
		oldesttime.dwHighDateTime = 0x7FFFFFFF;

		int oldest = -1;
		for (int index = 0; index < files.Count(); index++) {
			WIN32_FIND_DATA * file = files[index];

			if (file->ftLastAccessTime.dwHighDateTime != 0) {
				if (CompareFileTime(&file->ftLastAccessTime, &oldesttime) == -1) {
					oldest = index;
					oldesttime.dwLowDateTime = files[index]->ftLastAccessTime.dwLowDateTime;
					oldesttime.dwHighDateTime = files[index]->ftLastAccessTime.dwHighDateTime;
				}
			} else if (file->ftCreationTime.dwHighDateTime != 0) {
				if (CompareFileTime(&file->ftCreationTime, &oldesttime) == -1) {
					oldest = index;
					oldesttime.dwLowDateTime = files[index]->ftCreationTime.dwLowDateTime;
					oldesttime.dwHighDateTime = files[index]->ftCreationTime.dwHighDateTime;
				}
			} else if (file->ftLastWriteTime.dwHighDateTime != 0) {
				if (CompareFileTime(&file->ftLastWriteTime, &oldesttime) == -1) {
					oldest = index;
					oldesttime.dwLowDateTime = files[index]->ftLastWriteTime.dwLowDateTime;
					oldesttime.dwHighDateTime = files[index]->ftLastWriteTime.dwHighDateTime;
				}
			}
		}

		if (oldest == -1) {
			break;
		}

		if (!DeleteFile(files[oldest]->cFileName)) {
			break;
		}

		delete files[oldest];
		files.Delete_Index(oldest);
	}

	while (files.Count()) {
		delete files[0];
		files.Delete_Index(0);
	}
}


/// <summary>
/// Generates the random map the player has asked for.
/// This routine is what the map generator dialog's preview and generate buttons come down
/// to. A map that has been built for these exact settings before is kept in a cache and
/// merely fetched back, so flipping between two seeds costs nothing the second time. The
/// finished preview is left in RandMap.img for the lobby to show.
/// </summary>
/// <param name="dialog">The dialog to show generation progress within.</param>
/// <param name="callback">Progress callback to run while the map is being built.</param>
void Do_Random_Map(HWND dialog, bool (*callback)())
{
	rc_ptr<WDTTerritory> territory; // RA2: a tour game limits the player count, found as in Fixup_WDT_Settings
	if (Session.Type == GAME_INTERNET && Session.TournamentState == 2) { // RA2: as above
		territory = Find_WDT_Territory(Session.TournamentTerritory); // RA2: as above
	}
	if (territory.Get() != NULL) { // RA2: as above
		RandomMapGen.SeedData.NumPlayers = 4; // RA2: as above
	}

	char *digest = CalcRandomMapDigest(); // RA2: no IsWDT, WDTTerritory (TibSun's use does not compile against RA2's declaration)
	char name[128];
	memset(name, 0, sizeof(name));
	strncpy(name, "rmcache\\", sizeof(name));
	strncat(name, digest, sizeof(name));
	delete digest;
	strncat(name, ".mmp", sizeof(name));
	CCFileClass cfile(name); // RA2: no DebugString of the cache file name

	if (cfile.Is_Available()) {
		if (RandomMapGen.MapPreview == NULL) {
			RandomMapGen.MapPreview = new MapPreviewClass;
		}
		if (RandomMapGen.MapPreview->Read_PCX_Preview(name)) {
			RawFileClass file("RandMap.img");
			Write_PCX_File(file, *RandomMapGen.MapPreview->Get_Preview_Surface(), &GamePalette);
			delete RandomMapGen.MapPreview;
			RandomMapGen.MapPreview = NULL;
			return;
		}

		delete RandomMapGen.MapPreview;
		RandomMapGen.MapPreview = NULL;
	}

	RMGCallback = callback;
	RandomMapGen.SeedData.Callback = callback;
	if (RandomMapGen.SeedData.Seed == -1) {
		RandomMapGen.SeedData.Seed = NonCriticalRandomNumber(0, 65535); // RA2: the non-critical generator, 0x6388A0
	}
	RandomMapGen.Generate_Random_Map(true, dialog);
	RandomMapGen.MapPreview->Create_Preview();
	if (Session.Type == GAME_INTERNET && Session.TournamentState == 2) { // RA2: a tour game sends the preview to the other players
		Send_Map_Preview(RandomMapGen.MapPreview); // RA2: 0x5C7EF0
	}

	if (RandomMapGen.MapSeeder != NULL) {
		delete RandomMapGen.MapSeeder;
	}

	RandomMapGen.MapSeeder = new MapSeedClass;
	memcpy(RandomMapGen.MapSeeder, &RandomMapGen.SeedData, sizeof(RandomMapGen.SeedData));

	Scen->Set_Scenario_Name("RandomMap"); // RA2: a literal, not Fetch_String(TXT_RANDOM_MAP_DESCRIPTION)
	Title_Screen_Restore(false); // RA2: the argument is spelled out

	if (RandomMapGen.MapPreview != NULL) {
		if (RandomMapGen.MapPreview->Get_Preview_Surface() != NULL) {
			RawFileClass file("RandMap.img");
			Write_PCX_File(file, *RandomMapGen.MapPreview->Get_Preview_Surface(), &GamePalette);
			WIN32_FIND_DATA ff;
			if (FindFirstFile("rmcache", &ff) == INVALID_HANDLE_VALUE) {
				CreateDirectory("rmcache", 0);
			}
			CopyFile("RandMap.img", name, FALSE);
			Clean_Up_RMCache();
		}
		delete RandomMapGen.MapPreview;
		RandomMapGen.MapPreview = NULL;
	}

	if (RandomMapGen.MapSeeder != NULL) {
		delete RandomMapGen.MapSeeder;
		RandomMapGen.MapSeeder = NULL;
	}
}





static int MapSeedSelection = -1; // RA2: 0x7E0ADC
static bool MapSeedPreviewFlag = true; // RA2: 0x7E0AE0
namespace OwnerDraw { bool __fastcall Is_Control_Disabled(HWND control); } // RA2: 0x5E4720, defined in ownrdraw.cpp


/// <summary>
/// Dialog procedure for the random map generator ("Map Seed") dialog.
/// Handles previewing, generating, saving, loading and deleting random maps, and randomizing
/// the generator settings. The dialog's result code is written through the DWL_USER pointer set
/// up by Do_Random_Map_Dialog so that writing it ends that dialog's modal message loop.
/// </summary>
/// <param name="window">Handle to the dialog window.</param>
/// <param name="message">Window message identifier.</param>
/// <param name="wparam">Message-specific first parameter.</param>
/// <param name="lparam">Message-specific second parameter.</param>
/// <returns>TRUE if the message was processed, FALSE otherwise.</returns>
#ifdef RA2_NON_MATCHING // not exact: no exact match yet (row 00577bd0)
BOOL CALLBACK Map_Seed_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: RA2's form (0x577BD0); near miss, 5 bytes short: after LoadOptionsClass::Load the exe normalizes the result (test al, al / setne al / test al, al), which no source form tried reproduces
{
	BOOL result = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam); // RA2: as above
	if (result) { // RA2: as above
		return(result); // RA2: as above
	}

	LONG * state = (LONG *)GetWindowLongA(window, DWL_USER); // RA2: as above

	switch (message) { // RA2: as above

		/*
		 * Repaint the map preview, if one exists, and the progress screen over it.
		 */
		case WM_PAINT: // RA2: the preview is skipped when its frame control is disabled, and the progress screen is drawn over it
			if (RandomMapGen.MapPreview != NULL) { // RA2: as above
				HWND frame = GetDlgItem(window, IDC_PREVIEW_FRAME); // RA2: as above
				if (!OwnerDraw::Is_Control_Disabled(frame)) { // RA2: as above
					RandomMapGen.MapPreview->Blit_Preview(window); // RA2: as above
				}
			}
			if (Progress.Has_Dialog()) { // RA2: as above
				Progress.Display_Progress(Point2D(-1, -1)); // RA2: as above
			}
			ValidateRect(window, NULL); // RA2: as above
			return(0); // RA2: as above

		/*
		 * A change of the player count scroll bar invalidates the saved preview.
		 */
		case WM_HSCROLL: // RA2: the player count slider
			if (lparam != 0 && GetDlgCtrlID((HWND)lparam) == IDC_MAPGEN_PLAYERS) { // RA2: as above
				HWND handle = GetDlgItem(window, 0x6C5); // RA2: the generate button
				if (handle != NULL) { // RA2: as above
					EnableWindow(handle, false); // RA2: as above
				}
				handle = GetDlgItem(window, 0x6C3); // RA2: the save button
				if (handle != NULL) { // RA2: as above
					EnableWindow(handle, false); // RA2: as above
				}
				RandomMapGen.SeedData.Get_Settings(window); // RA2: as above
				InvalidateRect(window, NULL, false); // RA2: as above
				return(0); // RA2: as above
			}
			break; // RA2: as above

		case WM_COMMAND:
			switch (LOWORD(wparam)) { // RA2: as above

				/*
				 * Cancel the dialog.
				 */
				case 0x5C0: // RA2: the cancel button
					*state = 2;
					return(1); // RA2: as above

				/*
				 * A change of any of the choices invalidates the saved preview.
				 */
				case IDC_MAPGEN_TIME_OF_DAY: // RA2: the choices
				case 0x405: // RA2: as above
				case 0x406: // RA2: as above
				case 0x407: // RA2: as above
				case 0x408: // RA2: as above
					if (HIWORD(wparam) == CBN_SELCHANGE) { // RA2: as above
						HWND handle = GetDlgItem(window, 0x6C5); // RA2: as above
						if (handle != NULL) { // RA2: as above
							EnableWindow(handle, false); // RA2: as above
						}
						handle = GetDlgItem(window, 0x6C3); // RA2: as above
						if (handle != NULL) { // RA2: as above
							EnableWindow(handle, false); // RA2: as above
						}
						RandomMapGen.SeedData.Get_Settings(window); // RA2: as above
						InvalidateRect(window, NULL, false); // RA2: as above
						return(0); // RA2: as above
					}
					break; // RA2: as above

				/*
				 * Build and display a preview of the current map seed.
				 */
				case IDC_MAPGEN_PREVIEW: // RA2: the controls are disabled while the map is made
					MouseCursor->Hide_Mouse(); // RA2: as above
					if (MapSeedPreviewFlag) { // RA2: as above
						RandomMapGen.PreviewStale = true; // RA2: as above
					} else {
						RandomMapGen.PreviewStale = false; // RA2: as above
						MapSeedPreviewFlag = true; // RA2: as above
					}
					RandomMapGen.SeedData.Get_Settings(window); // RA2: as above
					EnableWindow(GetDlgItem(window, 0x405), false); // RA2: as above
					EnableWindow(GetDlgItem(window, IDC_MAPGEN_TIME_OF_DAY), false); // RA2: as above
					EnableWindow(GetDlgItem(window, 0x407), false); // RA2: as above
					EnableWindow(GetDlgItem(window, 0x406), false); // RA2: as above
					EnableWindow(GetDlgItem(window, 0x408), false); // RA2: as above
					EnableWindow(GetDlgItem(window, IDC_MAPGEN_PLAYERS), false); // RA2: as above
					EnableWindow(GetDlgItem(window, IDC_MAPGEN_SURPRISE), false); // RA2: as above
					EnableWindow(GetDlgItem(window, IDC_MAPGEN_PREVIEW), false); // RA2: as above
					EnableWindow(GetDlgItem(window, 0x6C2), false); // RA2: as above
					EnableWindow(GetDlgItem(window, 0x6C3), false); // RA2: as above
					EnableWindow(GetDlgItem(window, 0x6C4), false); // RA2: as above
					EnableWindow(GetDlgItem(window, 0x6C5), false); // RA2: as above
					EnableWindow(GetDlgItem(window, 0x5C0), false); // RA2: as above
					wcscpy(RandomMapGen.SeedData.MapDescription, StringTable::LoadString("TXT_RANDOM_MAP_DESCRIPTION", NULL, RA2_SOURCE, 3793)); // RA2: the description is reset
					RandomMapGen.Generate_Random_Map(true, window); // RA2: as above
					RandomMapGen.MapPreview->Create_Preview(); // RA2: as above
					EnableWindow(GetDlgItem(window, 0x405), true); // RA2: as above
					EnableWindow(GetDlgItem(window, IDC_MAPGEN_TIME_OF_DAY), true); // RA2: as above
					EnableWindow(GetDlgItem(window, 0x407), true); // RA2: as above
					EnableWindow(GetDlgItem(window, 0x406), true); // RA2: as above
					EnableWindow(GetDlgItem(window, 0x408), true); // RA2: as above
					EnableWindow(GetDlgItem(window, IDC_MAPGEN_PLAYERS), true); // RA2: as above
					EnableWindow(GetDlgItem(window, IDC_MAPGEN_SURPRISE), true); // RA2: as above
					EnableWindow(GetDlgItem(window, IDC_MAPGEN_PREVIEW), true); // RA2: as above
					EnableWindow(GetDlgItem(window, 0x6C2), true); // RA2: as above
					EnableWindow(GetDlgItem(window, 0x6C3), true); // RA2: as above
					EnableWindow(GetDlgItem(window, 0x6C4), true); // RA2: as above
					EnableWindow(GetDlgItem(window, 0x6C5), true); // RA2: as above
					EnableWindow(GetDlgItem(window, 0x5C0), true); // RA2: as above
					if (RandomMapGen.MapSeeder != NULL) { // RA2: as above
						delete RandomMapGen.MapSeeder; // RA2: as above
					}
					RandomMapGen.MapSeeder = new MapSeedClass; // RA2: as above
					memcpy(RandomMapGen.MapSeeder, &RandomMapGen.SeedData, sizeof(MapSeedClass)); // RA2: as above
					MouseCursor->Show_Mouse(); // RA2: as above
					Keyboard->Clear(); // RA2: as above
					PostMessageA(window, WM_PAINT, 0, 0); // RA2: as above
					return(0); // RA2: as above

				/*
				 * Randomize the generator settings.
				 */
				case IDC_MAPGEN_SURPRISE: { // RA2: the settings are drawn here, not by Randomize
					RandomMapGen.SeedData.Get_Settings(window); // RA2: as above
					RandomMapGen.SeedData.Set_Settings(window); // RA2: as above
					RandomMapGen.SeedData.Biome = (unsigned int)NonCriticalRandomNumber(0, 100) >= 50; // RA2: as above
					RandomMapGen.SeedData.MapType = NonCriticalRandomNumber(1, 4); // RA2: as above
					RandomMapGen.SeedData.Time = NonCriticalRandomNumber(0, 3); // RA2: as above
					RandomMapGen.SeedData.Resources = NonCriticalRandomNumber(0, 3); // RA2: as above
					RandomMapGen.SeedData.Width = NonCriticalRandomNumber(0, 3); // RA2: as above
					RandomMapGen.SeedData.Height = RandomMapGen.SeedData.Width; // RA2: as above
					RandomMapGen.SeedData.Randomize_For_Map_Type(RandomMapGen.SeedData.MapType); // RA2: as above
					wcscpy(RandomMapGen.SeedData.MapDescription, StringTable::LoadString("TXT_RANDOM_MAP_DESCRIPTION", NULL, RA2_SOURCE, 3793)); // RA2: as above
					RandomMapGen.SeedData.Seed = NonCriticalRandomNumber(0, 65535); // RA2: as above
					RandomMapGen.SeedData.Fixup_Settings(); // RA2: as above
					RandomMapGen.SeedData.Set_Settings(window); // RA2: as above
					delete RandomMapGen.MapPreview; // RA2: the old preview is dropped
					RandomMapGen.MapPreview = NULL; // RA2: as above
					HWND handle = GetDlgItem(window, 0x6C5); // RA2: as above
					if (handle != NULL) { // RA2: as above
						EnableWindow(handle, false); // RA2: as above
					}
					handle = GetDlgItem(window, 0x6C3); // RA2: as above
					if (handle != NULL) { // RA2: as above
						EnableWindow(handle, false); // RA2: as above
					}
					InvalidateRect(window, NULL, false); // RA2: as above
					return(0); // RA2: as above
				}

				/*
				 * Save the current map seed.
				 */
				case 0x6C3: { // RA2: the save button
					RandomMapGen.SeedData.Get_Settings(window); // RA2: as above
					RandomMapGen.SeedData.LoadOptionsClass::Save(RandomMapGen.SeedData.MapDescription); // RA2: as above

					bool enable = RandomMapGen.SeedData.Files_Present(); // RA2: as above
					HWND handle = GetDlgItem(window, 0x6C2); // RA2: the load button
					if (handle != NULL) { // RA2: as above
						EnableWindow(handle, enable); // RA2: as above
					}
					handle = GetDlgItem(window, 0x6C4); // RA2: the delete button
					if (handle == NULL) { // RA2: as above
						return(0); // RA2: as above
					}
					EnableWindow(handle, enable); // RA2: as above
					return(0); // RA2: as above
				}

				/*
				 * Load a saved map seed.
				 */
				case 0x6C2: { // RA2: the load button
					RandomMapGen.SeedData.Get_Settings(window); // RA2: as above
					bool loaded = RandomMapGen.SeedData.LoadOptionsClass::Load(); // RA2: held in a local
					if (loaded) { // RA2: as above
						MapSeedPreviewFlag = false; // RA2: the next preview starts fresh
						PostMessageA(window, WM_COMMAND, MAKEWPARAM(IDC_MAPGEN_PREVIEW, BN_CLICKED), (LPARAM)GetDlgItem(window, IDC_MAPGEN_PREVIEW)); // RA2: as above
					}
					while (MouseCursor->Get_Mouse_State() != 0) { // RA2: the mouse is shown again
						MouseCursor->Show_Mouse(); // RA2: as above
					}
					RandomMapGen.SeedData.Set_Settings(window); // RA2: as above
					return(0); // RA2: as above
				}

				/*
				 * Delete the saved map seed.
				 */
				case 0x6C4: { // RA2: the delete button
					RandomMapGen.SeedData.Get_Settings(window); // RA2: as above
					RandomMapGen.SeedData.LoadOptionsClass::Delete(); // RA2: as above

					bool enable = RandomMapGen.SeedData.Files_Present(); // RA2: as above
					HWND handle = GetDlgItem(window, 0x6C2); // RA2: as above
					if (handle != NULL) { // RA2: as above
						EnableWindow(handle, enable); // RA2: as above
					}
					handle = GetDlgItem(window, 0x6C4); // RA2: as above
					if (handle == NULL) { // RA2: as above
						return(0); // RA2: as above
					}
					EnableWindow(handle, enable); // RA2: as above
					return(0); // RA2: as above
				}

				/*
				 * Generate the map and accept the dialog.
				 */
				case 0x6C5: // RA2: the generate button
					RandomMapGen.SeedData.Get_Settings(window); // RA2: as above
					if (Debug_Map) { // RA2: as above
						RandomMapGen.Generate_Random_Map(false, window); // RA2: as above
						Scen->Set_Scenario_Name("Random Map"); // RA2: a literal
						Write_Scenario_INI("RandMap.Map", true); // RA2: as above
					} else {
						if (RandomMapGen.MapPreview == NULL || RandomMapGen.MapPreview->Get_Preview_Surface() == NULL) { // RA2: as above
							RandomMapGen.Generate_Random_Map(true, window); // RA2: as above
							if (Debug_Map) { // RA2: as above
								Scen->Set_Scenario_Name("Random Map"); // RA2: as above
								Write_Scenario_INI("RandMap.Map", true); // RA2: as above
							}
						}
					}
					*state = 1;
					return(1); // RA2: as above
			}
			break; // RA2: the other commands fall out to the bottom

		/*
		 * Initialize the dialog controls from the current seed settings.
		 */
		case 0x497: { // RA2: RA2's own message to set the dialog up (WM_USER + 0x97)
			MapSeedSelection = -1; // RA2: a file scope variable at 0x7E0ADC
			HWND handle = GetDlgItem(window, IDC_MAPGEN_PREVIEW); // RA2: as above
			if (Debug_Map) { // RA2: as above
				EnableWindow(handle, false); // RA2: as above
			} else {
				EnableWindow(handle, true); // RA2: as above
			}
			if (RandomMapGen.SeedData.Seed == -1) { // RA2: as above
				RandomMapGen.SeedData.Seed = Scen->RandomNumber(0, 65535); // RA2: the scenario's own generator
				RandomMapGen.PreviewStale = true; // RA2: as above
			}
			MapSeedPreviewFlag = true; // RA2: a file scope variable at 0x7E0AE0
			RandomMapGen.SeedData.Set_Settings(window); // RA2: as above

			bool enable = RandomMapGen.SeedData.Files_Present(); // RA2: as above
			handle = GetDlgItem(window, 0x6C2); // RA2: as above
			if (handle != NULL) { // RA2: as above
				EnableWindow(handle, enable); // RA2: as above
			}
			handle = GetDlgItem(window, 0x6C4); // RA2: as above
			if (handle != NULL) { // RA2: as above
				EnableWindow(handle, enable); // RA2: as above
			}
			handle = GetDlgItem(window, 0x6C5); // RA2: as above
			if (handle != NULL) { // RA2: as above
				EnableWindow(handle, false); // RA2: as above
			}
			handle = GetDlgItem(window, 0x6C3); // RA2: as above
			if (handle != NULL) { // RA2: as above
				EnableWindow(handle, false); // RA2: as above
			}
			return(0); // RA2: as above
		}
	}
	return(0); // RA2: as above
}
#endif



/// <summary>
/// Reads the map generator dialog into these settings.
/// This routine is called before a preview or a generate, so that whatever the player has
/// dialed in on the controls becomes the seed the generator works from. The settings taken
/// off the dialog are run through Fixup_Settings, so an impossible combination can never
/// reach the generator. The Firestorm settings are cleared away when that addon is absent.
/// </summary>
/// <param name="dialog">The map generator dialog to read.</param>
void MapSeedClass::Get_Settings(HWND dialog)
{
	HWND handle; // RA2: RA2's dialog controls - map type, resources, a stale preview (YR's form; TibSun's does not compile against RA2's declaration)
	int old; // RA2: as above

	old = MapType; // RA2: as above
	handle = GetDlgItem(dialog, 0x405); // RA2: as above
	MapType = ComboBox_GetItemData(handle, ComboBox_GetCurSel(handle)); // RA2: as above
	if (old != MapType) { // RA2: as above
		RandomMapGen.PreviewStale = true; // RA2: as above
	}

	old = Biome; // RA2: as above
	handle = GetDlgItem(dialog, 0x407); // RA2: as above
	Biome = ComboBox_GetItemData(handle, ComboBox_GetCurSel(handle));
	if (old != Biome) { // RA2: as above
		RandomMapGen.PreviewStale = true; // RA2: as above
	}

	old = Resources; // RA2: as above
	handle = GetDlgItem(dialog, 0x408); // RA2: as above
	Resources = ComboBox_GetItemData(handle, ComboBox_GetCurSel(handle)); // RA2: as above
	if (old != Resources) { // RA2: as above
		RandomMapGen.PreviewStale = true; // RA2: as above
	}

	old = Time; // RA2: as above
	handle = GetDlgItem(dialog, 0x3EA); // RA2: as above
	Time = ComboBox_GetItemData(handle, ComboBox_GetCurSel(handle)); // RA2: as above
	if (old != Time) { // RA2: as above
		RandomMapGen.PreviewStale = true; // RA2: as above
	}

	old = Width; // RA2: as above
	handle = GetDlgItem(dialog, 0x406); // RA2: as above
	Height = Width = ComboBox_GetItemData(handle, ComboBox_GetCurSel(handle)); // RA2: as above
	if (old != Width) { // RA2: as above
		RandomMapGen.PreviewStale = true; // RA2: as above
	}

	int old_players = NumPlayers; // RA2: as above
	handle = GetDlgItem(dialog, 0x3EB); // RA2: as above
	NumPlayers = SendMessageA(handle, WM_USER, 0, 0); // RA2: as above
	if (old_players != NumPlayers) { // RA2: as above
		RandomMapGen.PreviewStale = true; // RA2: as above
	} else if (!RandomMapGen.PreviewStale) { // RA2: as above
		goto done; // RA2: as above
	}

	Randomize_For_Map_Type(MapType); // RA2: redraws the terrain settings for the map type
	if (RandomMapGen.MapPreview != NULL) { // RA2: as above
		delete RandomMapGen.MapPreview; // RA2: as above
	}
	RandomMapGen.MapPreview = NULL; // RA2: as above
	RandomMapGen.PreviewStale = false; // RA2: as above

done: // RA2: as above
	Fixup_Settings();
}


/*
** RA2: the string table names of the map generator dialog's choices.
*/
static char const * _map_type_names[5] = {"TXT_MAP_ARCHIPELAGO", "TXT_MAP_CONTINENT", "TXT_MAP_TEAM_CONTINENTS", "TXT_MAP_INLAND", "TXT_MAP_MOUNTAINOUS"}; // RA2
static char const * _resource_names[4] = {"TXT_RESOURCE_LOW", "TXT_RESOURCE_MODERATE", "TXT_RESOURCE_HIGH", "TXT_RESOURCE_EXTREME"}; // RA2
static char const * _theater_names[2] = {"TXT_THEATER_TEMPERATE", "TXT_THEATER_SNOW"}; // RA2: no code reads it
static char const * _time_names[4] = {"TXT_TIME_MORNING", "TXT_TIME_AFTERNOON", "TXT_TIME_DUSK", "TXT_TIME_NIGHT"}; // RA2
static char const * _map_size_names[4] = {"TXT_MAPSIZE_SMALL", "TXT_MAPSIZE_MEDIUM", "TXT_MAPSIZE_LARGE", "TXT_MAPSIZE_VERY_LARGE"}; // RA2

/*
** RA2: the messages RA2's own combo box, edit and spinner controls take (wide text).
*/
#define CB_FINDSTRING_WIDE	(WM_USER + 0xBE) // RA2
#define CB_ADDSTRING_WIDE	(WM_USER + 0xC2) // RA2
#define WM_SETTEXT_WIDE		(WM_USER + 0xB4) // RA2
#define UDM_SETPOS_RA2		(WM_USER + 5) // RA2
#define UDM_SETRANGE_RA2	(WM_USER + 6) // RA2


/// <summary>
/// Fills the map generator dialog in from these settings.
/// This routine is the counterpart of Get_Settings, and is called whenever the dialog must
/// show a different set of options -- when it first appears, after a randomize, and after a
/// load. In a tournament game the controls are further restricted, or locked outright, to
/// whatever the territory permits the player to meddle with.
/// </summary>
/// <param name="dialog">The map generator dialog to fill in.</param>
void MapSeedClass::Set_Settings(HWND dialog)
{
	static char _seed_text[24]; // RA2: RA2's dialog (string table names, wide strings; YR's form)

	Fixup_Settings();

	HWND handle;
	LRESULT item;
	int i;

	handle = GetDlgItem(dialog, IDC_MAPGEN_TIME_OF_DAY); // RA2: as above
	while (SendMessageA(handle, CB_GETCOUNT, 0, 0) > 0) {
		SendMessageA(handle, CB_DELETESTRING, 0, 0);
	}
	for (i = 0; i < 4; i++) { // RA2: as above
		item = SendMessageA(handle, CB_ADDSTRING_WIDE, 0, (LPARAM)StringTable::LoadString(_time_names[i], NULL, RA2_SOURCE, 4727)); // RA2: as above
		SendMessageA(handle, CB_SETITEMDATA, item, i);
	}
	item = SendMessageA(handle, CB_FINDSTRING_WIDE, 0, (LPARAM)StringTable::LoadString(_time_names[Time], NULL, RA2_SOURCE, 4731)); // RA2: as above
	SendMessageA(handle, CB_SETCURSEL, item, 0);

	handle = GetDlgItem(dialog, 0x406); // RA2: as above
	while (SendMessageA(handle, CB_GETCOUNT, 0, 0) > 0) {
		SendMessageA(handle, CB_DELETESTRING, 0, 0);
	}
	for (i = 0; i < 4; i++) { // RA2: as above
		item = SendMessageA(handle, CB_ADDSTRING_WIDE, 0, (LPARAM)StringTable::LoadString(_map_size_names[i], NULL, RA2_SOURCE, 4742)); // RA2: as above
		SendMessageA(handle, CB_SETITEMDATA, item, i);
	}
	item = SendMessageA(handle, CB_FINDSTRING_WIDE, 0, (LPARAM)StringTable::LoadString(_map_size_names[Width], NULL, RA2_SOURCE, 4746)); // RA2: as above
	SendMessageA(handle, CB_SETCURSEL, item, 0);

	handle = GetDlgItem(dialog, 0x405); // RA2: as above
	while (SendMessageA(handle, CB_GETCOUNT, 0, 0) > 0) {
		SendMessageA(handle, CB_DELETESTRING, 0, 0);
	}
	for (i = 1; i < 5; i++) { // RA2: as above
		item = SendMessageA(handle, CB_ADDSTRING_WIDE, 0, (LPARAM)StringTable::LoadString(_map_type_names[i], NULL, RA2_SOURCE, 4758)); // RA2: as above
		SendMessageA(handle, CB_SETITEMDATA, item, i);
	}
	item = SendMessageA(handle, CB_FINDSTRING_WIDE, 0, (LPARAM)StringTable::LoadString(_map_type_names[MapType], NULL, RA2_SOURCE, 4762)); // RA2: as above
	SendMessageA(handle, CB_SETCURSEL, item, 0);

	handle = GetDlgItem(dialog, 0x407); // RA2: as above
	while (SendMessageA(handle, CB_GETCOUNT, 0, 0) > 0) {
		SendMessageA(handle, CB_DELETESTRING, 0, 0);
	}
	for (i = 0; i < 2; i++) { // RA2: as above
		item = SendMessageA(handle, CB_ADDSTRING_WIDE, 0, (LPARAM)StringTable::LoadString(Theaters[i].UIName, NULL, RA2_SOURCE, 4774)); // RA2: as above
		SendMessageA(handle, CB_SETITEMDATA, item, i);
	}
	item = SendMessageA(handle, CB_FINDSTRING_WIDE, 0, (LPARAM)StringTable::LoadString(Theaters[Biome].UIName, NULL, RA2_SOURCE, 4778)); // RA2: as above
	SendMessageA(handle, CB_SETCURSEL, item, 0);

	handle = GetDlgItem(dialog, 0x408); // RA2: as above
	while (SendMessageA(handle, CB_GETCOUNT, 0, 0) > 0) {
		SendMessageA(handle, CB_DELETESTRING, 0, 0);
	}
	for (i = 0; i < 4; i++) { // RA2: as above
		item = SendMessageA(handle, CB_ADDSTRING_WIDE, 0, (LPARAM)StringTable::LoadString(_resource_names[i], NULL, RA2_SOURCE, 4789)); // RA2: as above
		SendMessageA(handle, CB_SETITEMDATA, item, i);
	}
	item = SendMessageA(handle, CB_FINDSTRING_WIDE, 0, (LPARAM)StringTable::LoadString(_resource_names[Resources], NULL, RA2_SOURCE, 4793)); // RA2: as above
	SendMessageA(handle, CB_SETCURSEL, item, 0);

	handle = GetDlgItem(dialog, IDC_MAPGEN_DIMENSION_EDIT);
	sprintf(_seed_text, "%d", Seed); // RA2: as above
	SendMessageA(handle, WM_SETTEXT_WIDE, 0, (LPARAM)_seed_text); // RA2: as above

	handle = GetDlgItem(dialog, 0x3EB); // RA2: as above; the player count spinner
	SendMessageA(handle, UDM_SETRANGE_RA2, 1, MAKELONG(2, MAX_PLAYERS)); // RA2: as above
	SendMessageA(handle, UDM_SETPOS_RA2, 1, NumPlayers); // RA2: as above
}





/*
** RA2: the ranges Randomize_For_Map_Type draws each setting from, indexed by MapType (the first
** entry is the unused map type 0). The urban and accessibility minimums are all zero.
*/
static int _region_size_min[5] = {50, 0, 35, 0, 0}; // RA2
static int _region_size_max[5] = {100, 100, 100, 100, 50}; // RA2
static int _water_min[5] = {75, 0, 50, 0, 0}; // RA2
static int _water_max[5] = {100, 25, 100, 100, 100}; // RA2
static int _access_min[5] = {0, 0, 0, 0, 0}; // RA2
static int _access_max[5] = {100, 100, 100, 100, 20}; // RA2
static int _urban_min[5] = {0, 0, 0, 0, 0}; // RA2
static int _urban_max[5] = {50, 100, 100, 100, 0}; // RA2
static int _rugged_min[5] = {20, 20, 20, 20, 20}; // RA2
static int _rugged_max[5] = {100, 100, 100, 100, 100}; // RA2

/*
** RA2: the vegetation ranges are read through pointers that nothing ever sets, so RA2 reads
** through a null pointer here (RA2 bug, kept).
*/
static int * _vegetation_min; // RA2
static int * _vegetation_max; // RA2


/// <summary>
/// RA2: draws a setting from its range; going through this wrapper gives RA2's register use.
/// </summary>
static inline int Random_Setting(int min, int max) // RA2
{
	return(NonCriticalRandomNumber(min, max)); // RA2
}


/// <summary>
/// Draws the terrain settings at random for a kind of map.
/// Each kind of map (archipelago, continent and so on) allows its own range of water, hills,
/// cities, accessibility and cliffs; this routine picks a value inside each range and a fresh
/// tiberium layout, vegetation and seed to go with them.
/// </summary>
/// <param name="map_type">The kind of map the settings are drawn for.</param>
void MapSeedClass::Randomize_For_Map_Type(int map_type) // RA2: not in TibSun, at 0x5789F0
{
	WaterAmount = Random_Setting(_water_min[map_type], _water_max[map_type]); // RA2
	Hills = Random_Setting(_rugged_min[map_type], _rugged_max[map_type]); // RA2
	Cities = Random_Setting(_urban_min[map_type], _urban_max[map_type]); // RA2
	Accessibility = Random_Setting(_access_min[map_type], _access_max[map_type]); // RA2
	Cliffs = Random_Setting(_region_size_min[map_type], _region_size_max[map_type]); // RA2
	Tiberium = Resources * 20; // RA2
	TiberiumLayout = Random_Setting(0, 100); // RA2
	int minimum = _vegetation_min[map_type]; // RA2
	int maximum = _vegetation_max[map_type]; // RA2
	if (minimum < 0) { // RA2
		minimum = 0; // RA2
	} else if (minimum > 100) { // RA2
		minimum = 100; // RA2
	} // RA2
	if (maximum < 0) { // RA2
		maximum = 0; // RA2
	} else if (maximum > 100) { // RA2
		maximum = 100; // RA2
	} // RA2
	if (minimum > maximum) { // RA2
		minimum = maximum; // RA2
	} // RA2
	Vegetation = Random_Setting(minimum, maximum); // RA2
	Seed = NonCriticalRandomNumber(0, 65535); // RA2
}


/// <summary>
/// Picks a whole new set of map generator settings at random.
/// The theater, map type, time of day, resources and (square) map size are drawn first, and
/// the terrain settings are then drawn from the ranges the map type allows.
/// </summary>
void MapSeedClass::Randomize(void)
{
	Biome = (unsigned)NonCriticalRandomNumber(0, 100) >= 50U; // RA2: RA2's form; TibSun's does not compile against RA2's declaration
	MapType = NonCriticalRandomNumber(1, 4); // RA2: as above
	Time = NonCriticalRandomNumber(0, 3); // RA2: as above
	Resources = NonCriticalRandomNumber(0, 3); // RA2: as above
	Height = Width = NonCriticalRandomNumber(0, 3); // RA2: as above
	Randomize_For_Map_Type(MapType); // RA2: as above
	Set_Default_Description(); // RA2: as above
	Seed = NonCriticalRandomNumber(0, 65535); // RA2: as above
	Fixup_Settings();
}


/// <summary>
/// Imposes the tournament territory's map settings.
/// A Worldwide Domination Tournament game does not let the players design the battlefield --
/// the territory being fought over dictates what it looks like. This routine replaces the
/// generator settings with that territory's own, and does nothing at all in any other game.
/// </summary>
void MapSeedClass::Fixup_WDT_Settings(void)
{
	rc_ptr<WDTTerritory> territory; // RA2: RA2's form (the territory comes from the tour by index)
	if (Session.Type == GAME_INTERNET && Session.TournamentState == 2) { // RA2: as above
		territory = Find_WDT_Territory(Session.TournamentTerritory); // RA2: as above
	} // RA2: as above

	if (territory.Get() != NULL) { // RA2: as above
		unsigned short seed = territory->Seed; // RA2: as above
		Seed = seed == 0 ? NonCriticalRandomNumber(0, 0xFFFF) : seed; // RA2: as above

		switch (territory->Width) { // RA2: as above
			case 0: Width = 0; break; // RA2: as above
			case 2: Width = 2; break; // RA2: as above
			case 3: Width = 3; break; // RA2: as above
			case 1: Width = 1; break; // RA2: as above
			default: Width = 1; break; // RA2: as above
		} // RA2: as above

		switch (territory->Height) { // RA2: as above
			case 0: Height = 0; break; // RA2: as above
			case 2: Height = 2; break; // RA2: as above
			case 3: Height = 3; break; // RA2: as above
			case 1: Height = 1; break; // RA2: as above
			default: Height = 1; break; // RA2: as above
		} // RA2: as above

		NumPlayers = 4; // RA2: as above

		switch (territory->Time) { // RA2: as above
			case 2: Time = 0; break; // RA2: as above
			case 1: Time = 2; break; // RA2: as above
			case 3: Time = 3; break; // RA2: as above
			case 0: Time = 1; break; // RA2: as above
			default: Time = 1; break; // RA2: as above
		} // RA2: as above

		MapType = territory->MapType; // RA2: as above
		Biome = territory->Theater; // RA2: as above
		Cliffs = territory->CliffsDefault; // RA2: as above
		Accessibility = territory->AccessibilityDefault; // RA2: as above
		Hills = territory->HillsDefault; // RA2: as above
		WaterAmount = territory->WaterDefault; // RA2: as above
		Tiberium = territory->TiberiumAmountDefault; // RA2: as above
		TiberiumLayout = territory->TiberiumFieldsDefault; // RA2: as above
		Vegetation = territory->VegetationDefault; // RA2: as above
		Cities = territory->CitiesDefault; // RA2: as above
		Resources = territory->Resources; // RA2: as above
	} // RA2: as above
}


/// <summary>
/// Forces every map generation setting within its legal range.
/// This routine is called whenever the settings may have been tampered with -- after the
/// dialog is read, after a randomize, and after a load -- so that the generator itself never
/// has to defend against a nonsensical value. A tournament territory gets to narrow the
/// limits further, and the mutated biome is only allowed when the Firestorm addon is here.
/// </summary>
void MapSeedClass::Fixup_Settings(void)
{
	if (Resources >= 4) { // RA2: Fixup_Settings at 0x578D80 matches only so
		Resources = 3; // RA2: as above
	} else if (Resources < 0) { // RA2: as above
		Resources = 0; // RA2: as above
	}
	if (MapType >= 5) { // RA2: as above
		MapType = 4; // RA2: as above
	} else if (MapType < 0) { // RA2: as above
		MapType = 0; // RA2: as above
	}
	if (Time >= 4) { // RA2: as above
		Time = 3; // RA2: as above
	} else if (Time < 0) { // RA2: as above
		Time = 0; // RA2: as above
	}
	if (Hills > 100) { // RA2: Fixup_Settings at 0x578D80 has no Biome, Time
		Hills = 100;
	} else if (Hills < 0) {
		Hills = 0;
	}
	if (WaterAmount > 100) {
		WaterAmount = 100;
	} else if (WaterAmount < 0) {
		WaterAmount = 0;
	}
	if (NumPlayers > 8) { // RA2: as above
		NumPlayers = 8; // RA2: as above
	} else if (NumPlayers < 2) {
		NumPlayers = 2;
	}
	if (Tiberium > 100) {
		Tiberium = 100;
	} else if (Tiberium < 1) {
		Tiberium = 1;
	}
	if (TiberiumLayout > 100) {
		TiberiumLayout = 100;
	} else if (TiberiumLayout < 0) {
		TiberiumLayout = 0;
	}
	if (Vegetation > 100) {
		Vegetation = 100;
	} else if (Vegetation < 0) {
		Vegetation = 0;
	}
	if (Cities > 100) {
		Cities = 100;
	} else if (Cities < 0) {
		Cities = 0;
	}
	if (Width >= 4) {
		Width = 3; // RA2: as above
	} else if (Width < 0) {
		Width = 0;
	}
	if (Height >= 4) {
		Height = 3; // RA2: as above
	} else if (Height < 0) {
		Height = 0;
	}
	if (Accessibility > 100) {
		Accessibility = 100;
	} else if (Accessibility < 0) {
		Accessibility = 0;
	}
	if (Cliffs > 100) {
		Cliffs = 100;
	} else if (Cliffs < 0) {
		Cliffs = 0;
	}
	if (Seed > 65535) {
		Seed = 65535;
	} else if (Seed < 0) {
		Seed = 0;
	}
} // RA2: Fixup_Settings at 0x578D80 has no VeinholeMonsters


/// <summary>
/// Saves the current map generator settings.
/// Name a file to write it directly. Pass NULL instead and the standard save dialog asks the
/// player where to put it and what to call it.
/// </summary>
/// <param name="name">Name of the settings file, or NULL to ask the player.</param>
/// <returns>bool; Were the settings saved?</returns>
bool MapSeedClass::Save(const char * name)
{
	if (name) {
		return(Save_File(name, MapDescription));
	}

	if (BASECLASS::Save(MapDescription)) {
		return(true);
	}
	return(false);
}


/// <summary>
/// Writes the map generator settings to a file.
/// This routine records everything the random map dialog offers, so that loading the file
/// back and generating again lays down the very same terrain.
/// </summary>
/// <param name="file_name">Name of the settings file to write.</param>
/// <param name="descr">Description to keep with the settings. This is the text the load
/// dialog lists the map under.</param>
/// <returns>bool; Were the settings written?</returns>
bool MapSeedClass::Save_File(const char * file_name, const wchar_t * descr)
{
	if (file_name != NULL) {
		DebugString("Saving random map: %s - %ls\n", file_name, descr);
		CCFileClass file(file_name);
		INIClass ini;
		if (descr) {
			wcscpy(MapDescription, descr);
		}
		ini.Put_UnicodeString("RandomMap", "Description", descr); // RA2: the description is kept in the file
		ini.Put_Int("RandomMap", "Width", Width, 0);
		ini.Put_Int("RandomMap", "Height", Height, 0);
		ini.Put_Int("RandomMap", "NumPlayers", NumPlayers, 0);
		ini.Put_Int("RandomMap", "Seed", Seed, 0);
		ini.Put_Int("RandomMap", "MapType", MapType, 0);
		ini.Put_Int("RandomMap", "Theater", Biome, 0);
		ini.Put_Int("RandomMap", "Time", Time, 0);
		ini.Put_Int("RandomMap", "RegionSize", Cliffs, 0);
		ini.Put_Int("RandomMap", "Ruggedness", Hills, 0);
		ini.Put_Int("RandomMap", "Accessibility", Accessibility, 0);
		ini.Put_Int("RandomMap", "WaterAmount", WaterAmount, 0);
		ini.Put_Int("RandomMap", "Tiberium", Tiberium, 0);
		ini.Put_Int("RandomMap", "TiberiumLayout", TiberiumLayout, 0);
		ini.Put_Int("RandomMap", "Vegetation", Vegetation, 0);
		ini.Put_Int("RandomMap", "UrbanPresence", Cities, 0);
		ini.Put_Int("RandomMap", "Resources", Resources, 0);
		ini.Save(file);
		return(true);
	}
	return(false);
}


/// <summary>
/// Loads a saved set of map generator settings.
/// Name a file to read it directly. Pass NULL instead and the standard load dialog asks the
/// player which of the saved maps to bring back.
/// </summary>
/// <param name="name">Name of the settings file, or NULL to ask the player.</param>
/// <returns>bool; Were the settings loaded?</returns>
bool MapSeedClass::Load(const char * name)
{
	if (name) {
		return(Load_File(name));
	}

	if (BASECLASS::Load()) {
		return(true);
	}
	return(false);
}


/// <summary>
/// Reads the map generator settings from a file.
/// This routine restores a set of options saved earlier, so that a map the player liked can
/// be generated all over again. Any setting the file leaves out keeps the value it has.
/// </summary>
/// <param name="file_name">Name of the settings file to read.</param>
/// <returns>bool; Were the settings read?</returns>
bool MapSeedClass::Load_File(const char * file_name)
{
	if (file_name != NULL) {
		DebugString("Loading random map: %s\n", file_name);
		CCFileClass file(file_name);
		INIClass ini;

		if (ini.Load(file, false)) { // RA2: RA2's seed file keys (YR's form; TibSun's does not compile against RA2's declaration)
			ini.Get_UnicodeString("RandomMap", "Description", StringTable::LoadString("TXT_RANDOM_MAP_DESCRIPTION", NULL, RA2_SOURCE, 5275), MapDescription, 128); // RA2: as above
			Width = ini.Get_Int("RandomMap", "Width", Width);
			Height = ini.Get_Int("RandomMap", "Height", Height);
			NumPlayers = ini.Get_Int("RandomMap", "NumPlayers", NumPlayers);
			Seed = ini.Get_Int("RandomMap", "Seed", Seed);
			MapType = ini.Get_Int("RandomMap", "MapType", MapType); // RA2: as above
			Biome = ini.Get_Int("RandomMap", "Theater", Biome); // RA2: as above
			Time = ini.Get_Int("RandomMap", "Time", Time);
			Cliffs = ini.Get_Int("RandomMap", "RegionSize", Cliffs);
			Hills = ini.Get_Int("RandomMap", "Ruggedness", Hills);
			Accessibility = ini.Get_Int("RandomMap", "Accessibility", Accessibility);
			WaterAmount = ini.Get_Int("RandomMap", "WaterAmount", WaterAmount);
			Tiberium = ini.Get_Int("RandomMap", "Tiberium", Tiberium);
			TiberiumLayout = ini.Get_Int("RandomMap", "TiberiumLayout", TiberiumLayout);
			Vegetation = ini.Get_Int("RandomMap", "Vegetation", Vegetation);
			Cities = ini.Get_Int("RandomMap", "UrbanPresence", Cities);
			Resources = ini.Get_Int("RandomMap", "Resources", Resources); // RA2: as above
			return(true);
		}
		return(false); // RA2: as above
	}
	return(false);
}


/// <summary>
/// Discards a saved set of map generator settings.
/// The standard delete dialog asks the player which of the saved maps to throw away.
/// </summary>
/// <returns>bool; Were the settings deleted?</returns>
bool MapSeedClass::Delete(void)
{
	return(BASECLASS::Delete());
}


/// <summary>
/// Removes a saved settings file from the disk.
/// This routine is what the delete dialog calls once the player has settled on which of the
/// saved maps is to go.
/// </summary>
/// <param name="file_name">Name of the settings file to remove.</param>
/// <returns>bool; Was the file removed?</returns>
bool MapSeedClass::Delete_File(const char * file_name)
{
	return(BASECLASS::Delete_File(file_name));
}


/// <summary>
/// Fills in a list entry for a saved random map.
/// This routine is called once per candidate file as the load and save dialogs build their
/// lists. The description stored inside the map becomes the text the player picks from; a
/// map that carries no description is listed as unusable. The generator's own scratch file
/// is passed over.
/// </summary>
/// <param name="entry">The list entry to fill in.</param>
/// <param name="ff">The file the directory search turned up.</param>
/// <returns>bool; Was the entry filled in from a readable random map file?</returns>
bool MapSeedClass::Read_File(FileEntryClass * entry, WIN32_FIND_DATAA * ff)
{
	wchar_t buffer[128]; // RA2: a wide description, lastmap.sed skipped (YR's form; TibSun's does not compile against RA2's declaration)

	if (entry != NULL && ff != NULL) {
		if (stricmp(ff->cFileName, RANDOM_MAP_FILE_NAME) && stricmp(ff->cFileName, "lastmap.sed")) { // RA2: as above
			CCFileClass file(ff->cFileName);
			INIClass ini;
			if (ini.Load(file, false)) { // RA2: as above
				if (ini.Get_UnicodeString("RandomMap", "Description", L"", buffer, 128) > 0) { // RA2: as above
					wcsncpy(entry->Descr, buffer, 128); // RA2: as above
					entry->Old = false; // RA2: as above
					entry->Valid = true; // RA2: as above
				} else { // RA2: as above
					entry->Descr[0] = L'\0'; // RA2: as above
					entry->Old = true; // RA2: as above
					entry->Valid = false; // RA2: as above
				} // RA2: as above
				entry->Scenario = 0;
				entry->House = HOUSE_FIRST;
				strncpy(entry->Filename, ff->cFileName, 32); // RA2: as above
				if (!strlen(entry->Filename)) {
					strncpy(entry->Filename, ff->cAlternateFileName, 32); // RA2: as above
				}
				entry->DateTime.dwHighDateTime = ff->ftLastWriteTime.dwHighDateTime;
				entry->DateTime.dwLowDateTime = ff->ftLastWriteTime.dwLowDateTime;
				return(true);
			}
		}
	}
	return(false);
}


/// <summary>
/// RA2: the caption of the dialog that loads a saved random map.
/// </summary>
wchar_t const * MapSeedClass::Load_Caption(void) const // RA2: not in TibSun, at 0x5796F0
{
	return(StringTable::LoadString("GUI:LoadMapMenu", NULL, RA2_SOURCE, 5414)); // RA2
}


/// <summary>
/// RA2: the caption of the dialog that saves a random map.
/// </summary>
wchar_t const * MapSeedClass::Save_Caption(void) const // RA2: not in TibSun, at 0x579710
{
	return(StringTable::LoadString("GUI:SaveMapMenu", NULL, RA2_SOURCE, 5420)); // RA2
}


/// <summary>
/// RA2: the caption of the dialog that deletes a saved random map.
/// </summary>
wchar_t const * MapSeedClass::Delete_Caption(void) const // RA2: not in TibSun, at 0x579730
{
	return(StringTable::LoadString("GUI:DeleteMapMenu", NULL, RA2_SOURCE, 5426)); // RA2
}


/// <summary>
/// RA2: the message shown once a random map has been saved.
/// </summary>
wchar_t const * MapSeedClass::Saved_Message(void) const // RA2: not in TibSun, at 0x579750
{
	return(StringTable::LoadString("GUI:MapSaved", NULL, RA2_SOURCE, 5432)); // RA2
}














/// <summary>
/// Fetches a random fraction from the map generator.
/// This routine is the uniform source that every other random pick in the generator is
/// built upon. It draws from the generator's own stream rather than the game's, so the
/// terrain a given map seed produces never varies.
/// </summary>
/// <returns>Returns with a fraction from zero up to, but never reaching, one.</returns>
double Random_Fraction(void)
{
	return RMG_RANDOM_DOUBLE(1);
}


/// <summary>
/// Fetches a random number within the specified range.
/// This is the workhorse random pick of the map generator. Use this routine wherever a
/// count, an index, or an offset must be chosen at random. Both bounds are legal picks.
/// </summary>
/// <param name="minval">The lowest number that may be picked.</param>
/// <param name="maxval">The highest number that may be picked.</param>
/// <returns>Returns with the number picked, somewhere from minval to maxval inclusive.</returns>
unsigned int Pick_Random_UInt(unsigned int minval, unsigned int maxval)
{
	unsigned int value = 0;
	while (true) {
		value = ((Random_Fraction() * (maxval - minval + 1)) + (double)minval);
		if (value <= maxval) {
			break;
		}
	}
	return(value);
}


/// <summary>
/// Constructs a normal distribution sampler.
/// This routine builds a bell curve source on top of whatever uniform random generator the
/// caller supplies. The map generator uses it wherever a value should cluster about a mean
/// rather than land anywhere in a range with equal chance.
/// </summary>
/// <param name="function">The uniform random source to draw upon. It must return a fraction
/// from zero up to one.</param>
NormalDistribution::NormalDistribution(double (*function)(void))
{
	HasSpare = false;
	Spare = 0;
	Rand = function;
}


/// <summary>
/// Draws a value from the standard normal distribution.
/// Values come out of the polar method two at a time, so every other call is served from the one
/// held over from the call before it and consumes nothing from the random source.
/// </summary>
/// <returns>Returns with a sample of mean zero and unit standard deviation.</returns>
double NormalDistribution::operator ()(void)
{
	if (HasSpare) {
		HasSpare = false;
		return Spare;
	}

	double u = 0.0;
	double v = 0.0;
	double s = 0.0;

	do {
		u = 2.0 * Rand() - 1.0;
		v = 2.0 * Rand() - 1.0;
		s = v * v + u * u;
	} while (s >= 1.0 || s == 0.0);

	double l = log(s);
	double f = fastmath::sqrt((-l - l) / s); // RA2: YR's form: TibSun's matches at 0x579830 by itself, but VC6 then inlines it into Sample_Normal 0x5798D0, which in RA2 calls it
	double r1 = f * u; // RA2: as above
	double r2 = f * v; // RA2: as above
	Spare = r2; // RA2: as above
	HasSpare = true;
	return(r1); // RA2: as above
}


/// <summary>
/// Draws a normally distributed value.
/// The map generator reaches for this routine wherever it wants a quantity that clusters about a
/// preferred value rather than being spread evenly across a range.
/// </summary>
/// <param name="mean">The center of the distribution.</param>
/// <param name="standard_dev">The spread of the distribution.</param>
/// <returns>Returns with the sample drawn.</returns>
double Sample_Normal(double mean, double standard_dev)
{
	return(standard_dev * RMGGaussian() + mean);
}


/// <summary>
/// Draws a normally distributed value that keeps within bounds.
/// Samples that stray outside the bounds are thrown away and drawn again. A distribution that
/// sits outside the bounds altogether is recentered on them first, so that the draw does have
/// something to land on.
/// </summary>
/// <param name="mean">The center of the distribution to draw from.</param>
/// <param name="scale">The spread of the distribution to draw from.</param>
/// <param name="lower_bound">The lowest value that may be returned.</param>
/// <param name="upper_bound">The highest value that may be returned.</param>
/// <returns>Returns with the sample drawn, which always lies within the bounds.</returns>
double Sample_Truncated_Normal(double mean, double scale, double lower_bound, double upper_bound)
{
	double value;

	if (mean - scale > upper_bound || mean + scale < lower_bound) {
		scale = (upper_bound - lower_bound) * 0.5;
		mean = scale + lower_bound;
	}
	do {
		do {
			value = Sample_Normal(mean, scale);
		}
		while (value < lower_bound);
	}
	while (value > upper_bound);
	return(value);
}


/// <summary>
/// RA2
/// </summary>
inline double Sample_Truncated_Normal_Inline(double mean, double scale, double lower_bound, double upper_bound)
{
	double value;

	if (mean - scale > upper_bound || mean + scale < lower_bound) {
		scale = (upper_bound - lower_bound) * 0.5;
		mean = scale + lower_bound;
	}
	do {
		value = Sample_Normal(mean, scale);
	} while (value < lower_bound || value > upper_bound);
	return(value);
}


/// <summary>
/// Reads the random map generator's settings from RMG.INI's General section: the tiberium limits, the level light
/// and theater ambient light lists, the vegetation limits, the tree count and the buildings that light ore patches.
/// </summary>
#ifdef RA2_NON_MATCHING // not exact: no independent bound yet (00579960 has no row)
void MapGeneratorClass::Read_RMG_INI(void) // RA2: not in TibSun, at 0x579960; near miss, exact size: the exe expands the second Get_IntList assignment inline (copying three fields) where this build still calls DynamicVectorClass<int>::operator= for it, so the exe keeps the second and third list address in registers
{
	static char const * section = "General"; // RA2: the section name is a static pointer (0x7E0BE4), read for every key
	CCFileClass file("RMG.INI"); // RA2: as above
	CCINIClass * ini = new CCINIClass; // RA2: as above
	ini->Load(file, false); // RA2: as above

	RMGLevelLightSettings.Clear(); // RA2: as above
	RMGVegetationMinimums.Clear(); // RA2: as above
	RMGVegetationMaximums.Clear(); // RA2: as above
	TemperateOrePatchLamps.Clear(); // RA2: as above
	SnowOrePatchLamps.Clear(); // RA2: as above
	TemperateAmbientLight.Clear(); // RA2: as above
	SnowAmbientLight.Clear(); // RA2: as above

	RMGMinimumTiberium = ini->Get_Int(section, "RMGMinimumTiberium", RMGMinimumTiberium); // RA2: as above
	RMGMaximumTiberium = ini->Get_Int(section, "RMGMaximumTiberium", RMGMaximumTiberium); // RA2: as above
	RMGLevelLightSettings = ini->Get_IntList(section, "RMGLevelLightSettings", RMGLevelLightSettings); // RA2: as above
	RMGVegetationMinimums = ini->Get_IntList(section, "RMGVegetationMinimums", RMGVegetationMinimums); // RA2: as above
	RMGVegetationMaximums = ini->Get_IntList(section, "RMGVegetationMaximums", RMGVegetationMaximums); // RA2: as above
	TemperateAmbientLight = ini->Get_IntList(section, "TemperateAmbientLight", TemperateAmbientLight); // RA2: as above
	SnowAmbientLight = ini->Get_IntList(section, "SnowAmbientLight", SnowAmbientLight); // RA2: as above
	TemperateAmbientRed = ini->Get_IntList(section, "TemperateAmbientRed", TemperateAmbientRed); // RA2: as above
	TemperateAmbientGreen = ini->Get_IntList(section, "TemperateAmbientGreen", TemperateAmbientGreen); // RA2: as above
	TemperateAmbientBlue = ini->Get_IntList(section, "TemperateAmbientBlue", TemperateAmbientBlue); // RA2: as above
	SnowAmbientRed = ini->Get_IntList(section, "SnowAmbientRed", SnowAmbientRed); // RA2: as above
	SnowAmbientGreen = ini->Get_IntList(section, "SnowAmbientGreen", SnowAmbientGreen); // RA2: as above
	SnowAmbientBlue = ini->Get_IntList(section, "SnowAmbientBlue", SnowAmbientBlue); // RA2: as above
	MaxTrees = ini->Get_Int(section, "MaxTrees", MaxTrees); // RA2: as above

	TemperateOrePatchLamps = TGet_TypeList<BuildingTypeClass>(*ini, section, "TemperateOrePatchLamps", TemperateOrePatchLamps); // RA2: the lamp lists are read with the generic type-list reader (a 128 byte buffer inside), not by hand
	SnowOrePatchLamps = TGet_TypeList<BuildingTypeClass>(*ini, section, "SnowOrePatchLamps", SnowOrePatchLamps); // RA2: as above

	delete ini; // RA2: as above
	file.Close(); // RA2: as above
}
#endif



/// <summary>
/// Generates a complete random map.
/// This is the top level of the random map generator. Working from the seed data the player
/// settled on, it lays down the water, carves the terrain into regions, finds somewhere for each
/// player to start, and then dresses the result with veinholes, tiberium, towns, hills and
/// vegetation, reporting its progress as it goes.
/// </summary>
/// <param name="full_init">Should the scenario be rebuilt from scratch and the preview redrawn
/// between phases?</param>
/// <param name="dialog">The map generator dialog to repaint as the preview is refreshed.</param>
void MapGeneratorClass::Generate_Random_Map(bool full_init, HWND dialog) // RA2: RA2's form (0x57A0D0): no ice smoothing, veinholes, lights or urban areas; RMG.INI read here; the preview goes to the guests of a tour game
{
	int index; // RA2: counters shared by the loops below
	CellClass * cptr; // RA2: as above

	if (RMGCallback != NULL) RMGCallback();

	RMGRandom = Random2Class(SeedData.Seed);
	RMGGaussian = NormalDistribution(Random_Fraction);

	if (RMGCallback != NULL) RMGCallback();

	bool show_progress = !Scen->IsReadingScenario; // RA2: kept in a local, the progress screen is closed again at the end
	if (show_progress) { // RA2: the local flag
		Progress.Initialize(100, 1, dialog); // RA2: the dialog window
		Progress.Set_Graphic_Data("PROGBAR2.SHP");
		Progress.Set_Layout(Point2D(-1, -1), NULL, false, true); // RA2: the layout is set apart
		Progress.Display_Progress();
		HWND control = GetDlgItem(dialog, 0x638); // RA2: a control of the dialog, shown while the map is made
		if (control != NULL) { // RA2: as above
			ShowWindow(control, SW_SHOW); // RA2: as above
		}
	}

	if (RMGCallback != NULL) RMGCallback();

	DebugString("RMG: Init random map\n");

	Init_Map(full_init);

	if (RMGCallback != NULL) RMGCallback();

	ScenarioInit++; // RA2: as above

	if (full_init) { // RA2: as above
		RandomMapGen.MapPreview->Create_Preview(); // RA2: as above
		SendMessage(dialog, WM_PAINT, 0, 0); // RA2: as above
		if (Session.Type == GAME_INTERNET && Session.TournamentState == 2) { // RA2: a tour game sends each new preview to the guests
			Send_Map_Preview(RandomMapGen.MapPreview); // RA2: as above
		}
	}

	if (RMGCallback != NULL) RMGCallback();

	Read_RMG_INI(); // RA2: RMG.INI is read here, not in the constructor

	DebugString("RMG: Seeding water\n");

	if (SeedData.MapType == 3 || SeedData.MapType == 4) { // RA2: the inland and mountainous maps keep their water apart from the land
		if (SeedData.WaterAmount != 0) { // RA2: as above
			Seed_Water(); // RA2: as above
		}
	} else {
		Seed_Land(); // RA2: the other maps lay land down first
	}

	Vary_Water_Tiles(); // RA2: 0x57DE10

	Update_Progress(55);

	if (RMGCallback != NULL) RMGCallback();

	if (full_init) { // RA2: as above
		RandomMapGen.MapPreview->Create_Preview(); // RA2: as above
		SendMessage(dialog, WM_PAINT, 0, 0); // RA2: as above
		if (Session.Type == GAME_INTERNET && Session.TournamentState == 2) { // RA2: a tour game sends each new preview to the guests
			Send_Map_Preview(RandomMapGen.MapPreview); // RA2: as above
		}
	}

	if (RMGCallback != NULL) RMGCallback();

	Update_Progress(60); // RA2: no ice smoothing

	if (RMGCallback != NULL) RMGCallback();

	if (full_init) { // RA2: as above
		RandomMapGen.MapPreview->Create_Preview(); // RA2: as above
		SendMessage(dialog, WM_PAINT, 0, 0); // RA2: as above
		if (Session.Type == GAME_INTERNET && Session.TournamentState == 2) { // RA2: a tour game sends each new preview to the guests
			Send_Map_Preview(RandomMapGen.MapPreview); // RA2: as above
		}
	}

	if (RMGCallback != NULL) RMGCallback();

	DebugString("RMG: Init regions\n");

	Init_Regions();

	Update_Progress(65);

	if (RMGCallback != NULL) RMGCallback();

	DebugString("RMG: Making regions\n");

	if (SeedData.MapType == 4 || SeedData.MapType == 3) { // RA2: as above
		MapRegionClass::Make_Ice_Regions(); // RA2: as above
		MapRegionClass::Make_Regions(); // RA2: as above
		Fixup_Cliff_Corners(); // RA2: 0x5831C0
		Map.Build_All_Cliffs(0, -1); // RA2: as above
		Vary_Cliff_Runs(); // RA2: 0x582FD0
	}

	Update_Progress(70);

	if (RMGCallback != NULL) RMGCallback();

	if (full_init) { // RA2: as above
		RandomMapGen.MapPreview->Create_Preview(); // RA2: as above
		SendMessage(dialog, WM_PAINT, 0, 0); // RA2: as above
		if (Session.Type == GAME_INTERNET && Session.TournamentState == 2) { // RA2: a tour game sends each new preview to the guests
			Send_Map_Preview(RandomMapGen.MapPreview); // RA2: as above
		}
	}

	if (RMGCallback != NULL) RMGCallback();

	Scatter_Green_Tiles(); // RA2: 0x57CF20

	DebugString("RMG: Recalculating cell attributes\n");
	Map.Reset_Iterator(); // RA2: as above
	for (cptr = Map.Iterate(); cptr != NULL; cptr = Map.Iterate()) { // RA2: as above
		cptr->Recalc_Attributes(-1); // RA2: as above
	}

	if (RMGCallback != NULL) RMGCallback();

	Update_Progress(75);

	DebugString("RMG: Creating starting points\n");

	bool ok = false;
	while (!ok) {
		ok = Generate_Starting_Points();
		if (ok) {
			ok = Init_Start_Points();
		}
	}

	DebugString("RMG: Adding tech buildings\n"); // RA2: the neutral buildings stand in for the lights and urban areas
	if (SeedData.MapType != 0) { // RA2: as above
		Generate_Neutral_Buildings(); // RA2: 0x58AD90
	}

	if (RMGCallback != NULL) RMGCallback();

	DebugString("RMG: Creating tiberium\n");
	Create_Tiberium();

	if (RMGCallback != NULL) RMGCallback(); // RA2: as above

	if (RMGCellData != NULL) { // RA2: the work cells are reset for the next stage
		int size = MapCellStride * MapCellStride; // RA2: as above
		for (index = 0; index < size; index++) { // RA2: as above
			RMGCellData[index].RegionID = -1; // RA2: as above
			RMGCellData[index].SpreadID = -1; // RA2: as above
		}
	}

	for (index = MapRegionClass::MapRegions.Count() - 1; index >= 0; index--) { // RA2: the regions are dropped here as well as at the end
		MapRegionClass * region = MapRegionClass::MapRegions[index]; // RA2: as above
		if (region != NULL) { // RA2: as above
			delete region; // RA2: as above
		}
	}

	MapRegionClass::TotalCount = 0; // RA2: as above

	DebugString("RMG: Recalculating cell attributes\n");
	Map.Reset_Iterator(); // RA2: as above
	for (cptr = Map.Iterate(); cptr != NULL; cptr = Map.Iterate()) { // RA2: as above
		cptr->Recalc_Attributes(-1); // RA2: as above
	}

	Update_Progress(80);

	if (RMGCallback != NULL) RMGCallback();

	if (full_init) { // RA2: as above
		RandomMapGen.MapPreview->Create_Preview(); // RA2: as above
		SendMessage(dialog, WM_PAINT, 0, 0); // RA2: as above
		if (Session.Type == GAME_INTERNET && Session.TournamentState == 2) { // RA2: a tour game sends each new preview to the guests
			Send_Map_Preview(RandomMapGen.MapPreview); // RA2: as above
		}
	}

	if (RMGCallback != NULL) RMGCallback();

	Update_Progress(85); // RA2: no urban or rural areas

	if (RMGCallback != NULL) RMGCallback();

	if (full_init) { // RA2: as above
		RandomMapGen.MapPreview->Create_Preview(); // RA2: as above
		SendMessage(dialog, WM_PAINT, 0, 0); // RA2: as above
		if (Session.Type == GAME_INTERNET && Session.TournamentState == 2) { // RA2: a tour game sends each new preview to the guests
			Send_Map_Preview(RandomMapGen.MapPreview); // RA2: as above
		}
	}

	if (RMGCallback != NULL) RMGCallback();

	DebugString("RMG: Recalculating cell attributes\n");
	Map.Reset_Iterator(); // RA2: as above
	for (cptr = Map.Iterate(); cptr != NULL; cptr = Map.Iterate()) { // RA2: as above
		cptr->Recalc_Attributes(-1); // RA2: as above
	}

	if (RMGCallback != NULL) RMGCallback();

	DebugString("RMG: Creating hills\n");
	Generate_Hills();

	Update_Progress(90);

	if (RMGCallback != NULL) RMGCallback();

	if (full_init) { // RA2: as above
		RandomMapGen.MapPreview->Create_Preview(); // RA2: as above
		SendMessage(dialog, WM_PAINT, 0, 0); // RA2: as above
		if (Session.Type == GAME_INTERNET && Session.TournamentState == 2) { // RA2: a tour game sends each new preview to the guests
			Send_Map_Preview(RandomMapGen.MapPreview); // RA2: as above
		}
	}

	if (RMGCallback != NULL) RMGCallback();

	DebugString("RMG: Creating LATs, rocks etc\n");

	if (SeedData.Biome == 0) { // RA2: only the temperate biome
		Seed_Vegetation();
		Generate_Vegetation();
	} else {
		Seed_Arctic_Vegetation();
		Generate_Arctic_Vegetation();
	}

	if (RMGCallback != NULL) RMGCallback();

	Update_Progress(95);

	if (RMGCallback != NULL) RMGCallback();

	if (full_init) { // RA2: as above
		RandomMapGen.MapPreview->Create_Preview(); // RA2: as above
		SendMessage(dialog, WM_PAINT, 0, 0); // RA2: as above
		if (Session.Type == GAME_INTERNET && Session.TournamentState == 2) { // RA2: a tour game sends each new preview to the guests
			Send_Map_Preview(RandomMapGen.MapPreview); // RA2: as above
		}
	}

	ScenarioInit--;

	if (RMGCallback != NULL) RMGCallback();

	DebugString("RMG: Recalculating cell attributes\n");
	Map.Reset_Iterator(); // RA2: as above
	for (cptr = Map.Iterate(); cptr != NULL; cptr = Map.Iterate()) { // RA2: as above
		cptr->Recalc_Attributes(-1); // RA2: as above
	}

	if (RMGCallback != NULL) RMGCallback();

	TiberiumClass::Init_Tiberium_Growth_System(); // RA2: no veinhole growth system
	TiberiumClass::Init_Tiberium_Spread_System();

	DebugString("RMG: Cleanup\n");

	SeededWaterAmount = 0;
	WorkingRegionID = 0;

	if (RMGCellData != NULL) {
		delete [] RMGCellData;
		RMGCellData = NULL;
	}

	for (index = MapRegionClass::MapRegions.Count() - 1; index >= 0; index--) { // RA2: as above
		MapRegionClass * region = MapRegionClass::MapRegions[index]; // RA2: as above
		if (region != NULL) {
			delete region;
		}
	}

	MapRegionClass::TotalCount = 0;
	Map.Overpass();
	DebugString("RMG: Compute Radar Image\n");
	Map.Set_Local_Dimensions(Map.LocalRect);
	Map.Compute_Radar_Image();

	Update_Progress(100);

	DebugString("RMG: Done\n");

	if (show_progress) { // RA2: the progress screen is closed again
		HWND control = GetDlgItem(dialog, 0x638); // RA2: as above
		if (control != NULL) { // RA2: as above
			ShowWindow(control, SW_HIDE); // RA2: as above
		}
		Update_Progress(0); // RA2: as above
		Progress.Clear_Dialog(); // RA2: 0x6203D0, the window is forgotten
	}
}



/// <summary>
/// Releases the working state generation leaves behind.
/// The per-cell bookkeeping and the region objects are thrown away, and the finished map is
/// handed back to the game proper with its overpass, its dimensions and its radar image all
/// brought up to date.
/// </summary>
void MapGeneratorClass::Cleanup(void)
{
	SeededWaterAmount = 0;
	WorkingRegionID = 0;

	if (RMGCellData != NULL) {
		delete [] RMGCellData;
		RMGCellData = NULL;
	}

	for (int i = MapRegionClass::MapRegions.Count() - 1; i >= 0; i--) {
		MapRegionClass * region = MapRegionClass::MapRegions[i];
		if (region != NULL) {
			delete region;
		}
	}

	MapRegionClass::TotalCount = 0;
	Map.Overpass(); // RA2: no argument
	DebugString("RMG: Compute Radar Image\n");
	Map.Set_Local_Dimensions(Map.LocalRect);
	Map.Compute_Radar_Image();
}


/// <summary>
/// Prepares a blank map for the generator to work on.
/// The biome, size and time of day the player chose are written out as a scenario the engine can
/// swallow, and the map comes back sized, cleared to its fill terrain and lit to suit the hour.
/// A full initialization goes further and tears down the previous scenario altogether -- its
/// objects, houses and theater data -- which is what the generator dialog does between previews.
/// </summary>
/// <param name="full_init">Should the whole scenario and theater be rebuilt from scratch?</param>
void MapGeneratorClass::Init_Map(bool full_init) // RA2: RA2's form (0x57AE00): the sizes, lighting and theater come from RA2's own tables, the map is rebuilt in RA2's order
{
	int index; // RA2: shared by the loops below
	int size_index = SeedData.NumPlayers - 2; // RA2: as above
	float width_frac = SeedData.Width * (1.0f / 3.0f); // RA2: as above
	float height_frac = SeedData.Height * (1.0f / 3.0f); // RA2: as above

	Read_RMG_INI(); // RA2: RMG.INI is read first

	if (SeedData.MapType != 3 && SeedData.MapType != 4) { // RA2: only the maps with land are cut down to a size
		width_frac = width_frac < 1.2 ? width_frac : 1.2; // RA2: as above
		height_frac = height_frac < 1.2 ? height_frac : 1.2; // RA2: as above
	}

	/*
	 * Interpolate the local map dimensions between the minimum and maximum size
	 * tables (indexed by player count) by the requested Width/Height fraction.
	 */
	static const int _width_min[] = {70, 70, 70, 80, 90, 100, 100}; // RA2: the tables at 0x7A5B6C
	static const int _width_max[] = {80, 80, 80, 90, 100, 110, 120}; // RA2: 0x7A5B88
	static const int _height_min[] = {70, 70, 70, 80, 90, 100, 100}; // RA2: 0x7A5BA4
	static const int _height_max[] = {80, 80, 80, 90, 100, 110, 120}; // RA2: 0x7A5BC0

	LocalWidth = (int)(_width_min[size_index] * (1.0f - width_frac) + _width_max[size_index] * width_frac);
	LocalHeight = (int)(_height_min[size_index] * (1.0f - height_frac) + _height_max[size_index] * height_frac);

	Rect size(0, 0, LocalWidth + 4, LocalHeight + 12);
	Rect lsize(2, 5, LocalWidth, LocalHeight);

	CellHeight = 4; // RA2: as above

	CCINIClass ini; // RA2: as above

	ini.Clear();

	CellHeight = 4;

	ini.Put_String("Map", "Theater", Theaters[SeedData.Biome].Name); // RA2: the biome is the theater
	ini.Put_Rect("Map", "Size", size);
	ini.Put_Rect("Map", "LocalSize", lsize);
	ini.Put_Int("Map", "Level", CellHeight, 0);
	ini.Put_String(HouseTypes[HOUSE_GOOD]->IniName, "TechLevel", "0");
	ini.Put_String("Basic", "Player", HouseTypes[HOUSE_GOOD]->IniName);
	double ambient = (SeedData.Biome == 0 ? TemperateAmbientLight[SeedData.Time] : SnowAmbientLight[SeedData.Time]) * 0.01; // RA2: the ambient light comes from RMG.INI
	ini.Put_Float("Lighting", "Ambient", ambient); // RA2: as above
	ini.Put_Float("Lighting", "RedTint", 1.0);
	ini.Put_Float("Lighting", "GreenTint", 1.0);
	ini.Put_Float("Lighting", "BlueTint", 1.0);
	ini.Put_Float("Lighting", "Ground", 0.0);
	ini.Put_Float("Lighting", "Level", (double)(RMGLevelLightSettings[SeedData.Time] / 100)); // RA2: as above
	ini.Put_Float("Lighting", "IonAmbient", ambient); // RA2: the ion storm lighting is written too
	ini.Put_Float("Lighting", "IonRed", 0.3); // RA2: as above
	ini.Put_Float("Lighting", "IonGreen", 0.4); // RA2: as above
	ini.Put_Float("Lighting", "IonBlue", 0.75); // RA2: as above
	ini.Put_Float("Lighting", "IonGround", 0.0); // RA2: as above
	ini.Put_Float("Lighting", "IonLevel", 0.0); // RA2: as above

	if (!Scen->IsReadingScenario) {
		Progress.Set_Progress_Percent(0, 1);
	}

	if (RMGCallback != NULL) RMGCallback();

	TiberiumClass::Deinit_Tiberium_Spread_System();
	TiberiumClass::Deinit_Tiberium_Growth_System();
	if (!full_init) {
		ScenarioInit++;
		Scen->Set_Scenario_Name("");
		if (Debug_Map) {
			Clear_Scenario();
		}
		Read_Scenario_INI(ini, true);
		Fill_In_Data();
		Cell c(Map.PlayRect.Width / 2 + Map.PlayRect.Height / 2 + 1, Map.PlayRect.Width / 2 + Map.PlayRect.Height / 2); // RA2: built from both coordinates at once
		Scen->Set_Waypoint(WAYPT_REINF, c);
		Scen->Set_Waypoint(Scen->Home, Scen->Get_Waypoint_Cell(WAYPT_REINF));
		Map[Scen->Get_Waypoint_Cell(Scen->Home)].IsWaypoint = true;
		Map.Set_Tactical_Position(Scen->Get_Waypoint_Coord(Scen->Home));
		ScenarioInit--; // RA2: closed in this branch
	} else {

		Scen->Reset();
		if (MapPreview == NULL) {
			MapPreview = new MapPreviewClass;
		}

		Update_Progress(2);

		int theater = SeedData.Biome; // RA2: the biome is the theater
		bool changed = true;
		int last = Scen->Theater; // RA2: as above

		if (MapSeeder != NULL) {
			last = MapSeeder->Biome; // RA2: as above
			if (MapSeeder->Width == SeedData.Width && MapSeeder->Height == SeedData.Height && last == theater
					&& MapSeeder->NumPlayers == SeedData.NumPlayers) {
				changed = false;
			}
		}

		if (RMGCallback != NULL) RMGCallback();

		/*
		 * On a theater/seed change strip the isometric tile types from the
		 * AbstractTypes list, tear down all existing objects, then re-add the
		 * (possibly reloaded) isometric tile types.
		 */
		if (MapSeeder == NULL || theater != last || changed) {
			for (index = AbstractTypes.Count() - 1; index >= 0; index--) { // RA2: the counter is shared
				if (AbstractTypes[index]->What_Am_I() == RTTI_ISOTILETYPE) {
					AbstractTypes.Delete_Index(index);
				}
			}

			if (RMGCallback != NULL) RMGCallback();

			Delete_All_Objects();

			if (RMGCallback != NULL) RMGCallback();

			for (index = 0; index < IsometricTileTypes.Count(); index++) { // RA2: as above
				AbstractTypes.Add(IsometricTileTypes[index]); // RA2: as above
			}
		}

		Update_Progress(5);

		if (RMGCallback != NULL) RMGCallback();

		if (changed) {
			Map.Init_Clear();
			Map.Set_Map_Dimensions(size, true, 0, true);
			Map.Set_Local_Dimensions(lsize);
			if (TacticalMap != NULL) {
				delete TacticalMap;
			}
			TacticalMap = new Tactical;
			TacticalMap->Reset_Dirty_Rectangles();
		}

		Update_Progress(10);

		while (Units.Count() > 0) {
			delete Units[Units.Count() - 1];
		}

		while (Infantry.Count() > 0) {
			delete Infantry[Infantry.Count() - 1];
		}

		while (Buildings.Count() > 0) {
			delete Buildings[Buildings.Count() - 1];
		}

		while (Terrains.Count() > 0) {
			delete Terrains[Terrains.Count() - 1];
		}

		if (RMGCallback != NULL) RMGCallback(); // RA2: no veinhole reset

		Map.Reset_Iterator();
		CellClass *cptr = Map.Iterate();
		while (cptr != NULL) {
			cptr->Ramp = 0;
			cptr->Height = 4;
			cptr->ITType = ISOTILE_CLEAR;
			cptr->SubTile = 0;
			cptr->Overlay = OVERLAY_NONE;
			cptr->OverlayData = 0;
			cptr = Map.Iterate();
		}

		if (RMGCallback != NULL) RMGCallback();

		Update_Progress(15);

		Map.Compute_Zone_Connections();
		Map.Zone_Reset();
		Map.Reset_All_Subzones();

		Update_Progress(20);

		if (RMGCallback != NULL) RMGCallback();

		Scen->Clear_All_Waypoints();
		Cell c(Map.PlayRect.Width / 2 + Map.PlayRect.Height / 2 + 1, Map.PlayRect.Width / 2 + Map.PlayRect.Height / 2); // RA2: built from both coordinates at once
		Scen->Set_Waypoint(WAYPT_REINF, c);
		Scen->Set_Waypoint(WAYPT_HOME, Scen->Get_Waypoint_Cell(WAYPT_REINF)); // RA2: the home waypoint itself, not Scen->Home
		Map[Scen->Get_Waypoint_Cell(WAYPT_HOME)].IsWaypoint = true; // RA2: as above
		Map.Set_Tactical_Position(Scen->Get_Waypoint_Coord(WAYPT_HOME)); // RA2: as above

		Update_Progress(25);

		if (RMGCallback != NULL) RMGCallback();

		ScenarioInit++;

		if (theater != Scen->Theater) {
			Init_Theater((TheaterType)theater); // RA2: as above
			Scen->Theater = (TheaterType)theater; // RA2: as above
		}

		Update_Progress(30);

		if (RMGCallback != NULL) RMGCallback();

		if (MapSeeder == NULL || theater != last || changed) {
			Rule->Initialize(*RuleINI);
			Scen->Read_Global_INI(*RuleINI);
		}

		Update_Progress(35);

		if (RMGCallback != NULL) RMGCallback();

		if (MapSeeder == NULL || theater != last || changed) {

			if (Houses.Count() == 0) {
				for (index = 0; index < HouseTypes.Count(); index++) { // RA2: a house for every house type
					HouseClass * hptr = new HouseClass(HouseTypes[index]); // RA2: as above
					hptr->Read_INI(ini); // RA2: as above
				}
			}

			if (RMGCallback != NULL) RMGCallback();

			TerrainTypeClass::Init(Scen->Theater);
			if (Scen->Theater != LastTheater) {
				IsometricTileTypeClass::Read_Control_File(Scen->Theater, true);
			} else {
				IsometricTileTypeClass::Clear_Use_Counts();
			}

			OverlayTypeClass::Init(Scen->Theater);

			if (RMGCallback != NULL) RMGCallback();
			if (RMGCallback != NULL) RMGCallback();
			if (RMGCallback != NULL) RMGCallback();

			BuildingTypeClass::Init(Scen->Theater);

			if (RMGCallback != NULL) RMGCallback();

			AnimTypeClass::Init(Scen->Theater);

			if (RMGCallback != NULL) RMGCallback();

			SmudgeTypeClass::Init(Scen->Theater);

			if (RMGCallback != NULL) RMGCallback();

			IsometricTileTypeClass::Load_Tiles(0, 1);
		}
		LastTheater = Scen->Theater;
		ScenarioInit--; // RA2: closed in this branch
	}

	Update_Progress(40);

	if (RMGCallback != NULL) RMGCallback();

	if (RMGCellData != NULL) { // RA2: the old cells are only freed when there are some
		delete [] RMGCellData; // RA2: as above
	}

	RMGCellData = new MapRegionClass::CellData[MapCellStride * MapCellStride];

	Map.Reset_Iterator();
	CellClass *cptr = Map.Iterate();
	while (cptr != NULL) {
		Cell c = cptr->CellID;
		MapRegionClass::Get_Cell_Data(c).CellID = c;
		cptr->Height = CellHeight;
		cptr = Map.Iterate();
	}

	Update_Progress(45);

	if (RMGCallback != NULL) RMGCallback();

	MapRegionClass::Delete_All_Regions();

	SeededWaterAmount = 0;
	WorkingRegionID = 0;
	static const float _waterfall_chance = 0.25f; // RA2: a static constant (0x7A5B68), loaded from memory
	PlaceWaterfall = Random_Fraction() < _waterfall_chance; // RA2: the fraction is not narrowed to float
	Map.Clear_Radar();
	Map.Set_Local_Dimensions(lsize);
	Map.Init_Radar();
	Map.Compute_Radar_Image();

	Scen->AmbientLight = SeedData.Biome == 0 ? TemperateAmbientLight[SeedData.Time] : SnowAmbientLight[SeedData.Time]; // RA2: the lighting comes from RMG.INI
	Scen->LevelLight = RMGLevelLightSettings[SeedData.Time]; // RA2: as above
	Scen->RedTint = SeedData.Biome == 0 ? TemperateAmbientRed[SeedData.Time] : SnowAmbientRed[SeedData.Time]; // RA2: as above
	Scen->GreenTint = SeedData.Biome == 0 ? TemperateAmbientGreen[SeedData.Time] : SnowAmbientGreen[SeedData.Time]; // RA2: as above
	Scen->BlueTint = SeedData.Biome == 0 ? TemperateAmbientBlue[SeedData.Time] : SnowAmbientBlue[SeedData.Time]; // RA2: as above
	Scen->IonAmbientLight = 87; // RA2: the ion storm lighting
	Scen->IonRedTint = 30; // RA2: as above
	Scen->IonGreenTint = 40; // RA2: as above
	Scen->IonBlueTint = 75; // RA2: as above
	Scen->IonGroundLight = 0; // RA2: as above
	Scen->IonLevelLight = 0; // RA2: as above

	Update_Progress(50); // RA2: no UseTransitions (not in RA2's layout)
	if (wcslen(SeedData.MapDescription) == 0) { // RA2: an empty description gets the default one
		wcscpy(SeedData.MapDescription, StringTable::LoadString("TXT_RANDOM_MAP_DESCRIPTION", NULL, RA2_SOURCE, 6581)); // RA2: as above
	}
}



/// <summary>
/// Reports how far generation has come.
/// The word goes to the loading screen when a scenario is being read, and to the map generator
/// dialog's own progress bar otherwise.
/// </summary>
/// <param name="percent_progress">How far along generation is, from 0 to 100.</param>
void MapGeneratorClass::Update_Progress(int percent_progress)
{
	if (Scen->IsReadingScenario) {
		Session.Update_Progress(percent_progress + 99);
	} else {
		Progress.Set_Progress_Percent(0, percent_progress);
	}
}


void MapGeneratorClass::Seed_Land(void) // RA2: not in TibSun, at 0x57BEA0
{
	Map.Reset_Iterator();
	CellClass * cptr = Map.Iterate();
	while (cptr != NULL) {
		cptr->ITType = IsometricTileTypeClass::WaterSet;
		cptr = Map.Iterate();
	}

	switch (SeedData.MapType) {
		case 0:
			Seed_Land_Type0();
			break;
		case 1:
			Seed_Land_Type1();
			break;
		case 2:
			Seed_Land_Type2();
			break;
	}

	Map.Reset_Iterator();
	cptr = Map.Iterate();
	while (cptr != NULL) {
		if (cptr->ITType == IsometricTileTypeClass::WaterSet) {
			bool inland = true;
			for (int facing = FACING_N; facing < FACING_COUNT; facing += 2) {
				if (!cptr->Adjacent_Cell((FacingType)facing).Is_Tile_Clear()) {
					inland = false;
				}
			}
			if (inland) {
				cptr->ITType = ISOTILE_CLEAR;
			}
		}
		cptr = Map.Iterate();
	}

	Map.Build_All_Shores(0, 0); // RA2: 0x55C2C0 (meastes MapClass_Build_All_Shores)

	if (RMGCellData != NULL) {
		int grid_size = MapCellStride * MapCellStride;
		for (int i = 0; i < grid_size; i++) {
			MapRegionClass::Get_Cell_Data(i).RegionID = -1;
			MapRegionClass::Get_Cell_Data(i).SpreadID = -1;
		}
	}

	for (int i = MapRegionClass::MapRegions.Count() - 1; i >= 0; i--) {
		MapRegionClass * region = MapRegionClass::MapRegions[i];
		if (region != NULL) {
			delete region;
		}
	}

	MapRegionClass::TotalCount = 0;

	Green_Shores();
}


void MapGeneratorClass::Partition_Rect(DynamicVectorClass<Rect> & rects, int count, Rect const & area) // RA2: not in TibSun, at 0x57C0D0
{
	int root = (int)fastmath::sqrt((double)count);
	int cols = (root * root == count) ? root : root + 1;
	int rows = (cols * root >= count) ? root : cols;
	int cell_w = area.Width / cols;
	int cell_h = area.Height / cols;
	int step_x = 0;
	int step_y = 0;
	int skip_x = 0;
	int skip_y = 0;

	if (Random_Fraction() < 0.5) {
		cell_w = area.Width / rows;
		skip_y = cell_h;
		step_x = cell_w;
	} else {
		cell_h = area.Height / rows;
		skip_x = cell_w;
		step_y = cell_h;
	}

	int empty = rows * cols - count;
	Point2D pos = area.Top_Left();

	DynamicVectorClass<int> lengths;
	DynamicVectorClass<int> open;
	for (int i = 0; i < rows; i++) {
		lengths.Add(cols);
		open.Add(i);
	}

	for (int n = 0; n < empty; n++) {
		unsigned int last = open.Count() - 1;
		unsigned int pick;
		do {
			pick = (unsigned int)(RMG_RANDOM_DOUBLE(last + 1));
		} while (pick > last);
		lengths[open[pick]] = root;
		open.Delete_Index(pick);
	}

	for (int row = 0; row < rows; row++) {
		int length = lengths[row];
		if (length < cols) {
			if (skip_x > 0) {
				pos.X += cell_w / 2;
			} else {
				pos.Y += cell_h / 2;
			}
		}
		for (int j = 0; j < length; j++) {
			rects.Add(Rect(pos.X + 2, pos.Y + 2, cell_w - 4, cell_h - 4));
			pos.X += skip_x;
			pos.Y += skip_y;
		}
		pos.X += step_x;
		pos.Y += step_y;
		if (skip_x > 0) {
			pos.X = area.X;
		} else {
			pos.Y = area.Y;
		}
	}
}


/// <summary>
/// RA2: lays the land down as one chunk per player plus a few spare ones, each grown from the middle of its share
/// of the map.
/// </summary>
#ifdef RA2_NON_MATCHING // not exact: 18 bytes differ (adopted text)
void MapGeneratorClass::Seed_Land_Type0(void) // RA2: RA2's form of Seed_Land_Type0 (0x57C4F0); near miss, 8 bytes differ (an integer shift is scheduled before the 0.45 addition)
{
	WorkingRegionID = 1; // RA2: as above
	DynamicVectorClass<Rect> regions; // RA2: as above

	unsigned int range = 2 > SeedData.NumPlayers / 2 ? 2 : SeedData.NumPlayers / 2; // RA2: a signed compare and quotient, not MAX of unsigned values
	double span = (double)range; // RA2: converted once before the loop
	unsigned int selected; // RA2: as above
	do {
		selected = (unsigned int)(((unsigned int)RMGRandom() * span) * (1.0 / 4294967295.0) + 1.0); // RA2: as above
	} while (selected > range); // RA2: as above

	Rect area = Map.LocalRect; // RA2: the rectangle is copied to a local first
	Partition_Rect(regions, SeedData.NumPlayers + selected, area); // RA2: as above

	Map.Reset_Iterator();
	for (CellClass * cptr = Map.Iterate(); cptr != NULL; cptr = Map.Iterate()) { // RA2: as above
		Cell position = cptr->CellID; // RA2: indexes the cell data directly instead of calling Get_Cell_Data
		RMGCellData[position.Y * MapCellStride + position.X].SpreadID = 0; // RA2: as above
	}

	while (regions.Count() > 0) { // RA2: as above
		Rect region = regions[0]; // RA2: as above
		int count = (int)((RMG_RANDOM_DOUBLE2(0.05) + 0.45) * (region.Width * region.Height * 2)); // RA2: as above
		Point2D center; // RA2: assigned member by member
		center.X = region.Width / 2 + region.Height / 2 + region.Y + region.X + 1; // RA2: as above
		center.Y = region.Height / 2 - region.Width / 2 - region.X + Map.PlayRect.Width + region.Y; // RA2: as above
		int attempt = 0;
		int result = 0; // RA2: as above
		while (!result && attempt < 10) { // RA2: a while loop
			result = Spread_Land(count, region, center, true, Cell(center.X, center.Y), 0.25, false); // RA2: as above
			if (!result) {
				++WorkingRegionID; // RA2: as above
			}
			++attempt; // RA2: as above
		}
		regions.Delete_Index(0); // RA2: as above
	}
}
#endif



/// <summary>
/// RA2: lays the land down as one mass grown from the middle of the map until the water setting's share of land is
/// met.
/// </summary>
void MapGeneratorClass::Seed_Land_Type1(void) // RA2: RA2's form of Seed_Land_Type1 (0x57C780): the land fraction bounds and chunk size are static constants
{
	static const double _land_minimum = 0.45; // RA2: static constants (0x7A5C58, 0x7A5C60, 0x7A5C68), not folded literals
	static const double _land_maximum = 0.5; // RA2: as above
	static const double _land_chunk = 0.03; // RA2: as above
	WorkingRegionID = 1; // RA2: as above
	int total = Map_Cell_Count(); // RA2: as above
	int placed = 0;
	int tries = 0;
	double current = 0.0; // RA2: as above
	double target = (1.0 - SeedData.WaterAmount * 0.01) * (_land_maximum - _land_minimum) + _land_minimum; // RA2: the bounds are static constants, not folded literals (0x7A5C60 / 0x7A5C58)
	int maximum = (int)((double)total * _land_chunk * target); // RA2: a static constant instead of the literal
	Point2D origin(Map.PlayRect.Width / 2 + Map.PlayRect.Height / 2 + 1, // RA2: as above
				   Map.PlayRect.Width / 2 + Map.PlayRect.Height / 2); // RA2: as above
	Point2D seed = origin; // RA2: as above

	Map.Reset_Iterator();
	CellClass * cptr = Map.Iterate();
	while (cptr != NULL) {
		MapRegionClass::Get_Cell_Data(cptr->Fetch_CellID()).SpreadID = 0;
		cptr = Map.Iterate();
	}

	Rect bounds = Map.LocalRect; // RA2: as above
	bounds.X++; // RA2: as above
	bounds.Y++; // RA2: as above
	bounds.Width -= 2; // RA2: as above
	bounds.Height -= 2; // RA2: as above
	while (current < target && tries < 100) { // RA2: as above
		int size = (int)((target - current) * (double)total); // RA2: as above
		size = MIN(size, maximum); // RA2: as above
		current = (double)(placed += Spread_Land(size, bounds, seed, true, Cell(origin.X, origin.Y), 0.75, true)) / (double)total; // RA2: as above
		tries++;

		int nearest = 50000; // RA2: as above
		Cell closest(0, 0); // RA2: as above
		Map.Reset_Iterator();
		cptr = Map.Iterate();
		while (cptr != NULL) {
			if (MapRegionClass::Get_Cell_Data(cptr->CellID).SpreadID == 0) { // RA2: as above
				int distance = abs(cptr->CellID.X - origin.X) + abs(cptr->CellID.Y - origin.Y); // RA2: as above
				if (distance < nearest) { // RA2: as above
					nearest = distance; // RA2: as above
					closest = cptr->CellID; // RA2: as above
				}
			}
			cptr = Map.Iterate();
		}
		seed = Point2D(closest.X, closest.Y); // RA2: as above
	}
}



/// <summary>
/// RA2: lays the land down as two masses, one in each half of the map, each grown until its half has the water
/// setting's share of land.
/// </summary>
void MapGeneratorClass::Seed_Land_Type2(void) // RA2: RA2's form of Seed_Land_Type2 (0x57C9E0): the land fraction bounds and chunk size are static constants
{
	static const double _land_minimum = 0.15; // RA2: static constants (0x7A5C78, 0x7A5C80, 0x7A5C88), not folded literals
	static const double _land_maximum = 0.2; // RA2: as above
	static const double _land_chunk = 0.06; // RA2: as above
	WorkingRegionID = 1; // RA2: as above
	int total = Map_Cell_Count(); // RA2: as above
	int tries = 0;
	double target = (1.0 - SeedData.WaterAmount * 0.01) * (_land_maximum - _land_minimum) + _land_minimum; // RA2: the bounds are static constants, not folded literals (0x7A5C80 / 0x7A5C78)
	int maximum = (int)((double)total * _land_chunk * target); // RA2: a static constant instead of the literal

	Map.Reset_Iterator();
	CellClass * cptr = Map.Iterate();
	while (cptr != NULL) {
		MapRegionClass::Get_Cell_Data(cptr->Fetch_CellID()).SpreadID = 0;
		cptr = Map.Iterate();
	}

	Rect halves[2]; // RA2: as above
	if (Random_Fraction() < 0.5) { // RA2: as above
		halves[0] = Rect(Map.LocalRect.X, Map.LocalRect.Y, Map.LocalRect.Width / 2 - 1, Map.LocalRect.Height); // RA2: as above
		halves[1] = Rect(Map.LocalRect.X + Map.LocalRect.Width / 2 + 1, Map.LocalRect.Y, Map.LocalRect.Width / 2 - 1, Map.LocalRect.Height); // RA2: as above
	} else {
		halves[0] = Rect(Map.LocalRect.X, Map.LocalRect.Y, Map.LocalRect.Width, Map.LocalRect.Height / 2 - 1); // RA2: as above
		halves[1] = Rect(Map.LocalRect.X, Map.LocalRect.Y + Map.LocalRect.Height / 2 + 1, Map.LocalRect.Width, Map.LocalRect.Height / 2 - 1); // RA2: as above
	}

	for (int half = 0; half < 2; half++) { // RA2: as above
		int placed = 0;
		double current = 0.0; // RA2: as above
		Rect bounds = halves[half]; // RA2: as above
		Point2D origin(bounds.X + bounds.Width / 2 + bounds.Height / 2 + bounds.Y + 1, // RA2: as above
					   bounds.Y + Map.PlayRect.Width + bounds.Height / 2 - bounds.Width / 2 - bounds.X); // RA2: as above
		Point2D seed = origin; // RA2: as above
		double x_scale; // RA2: as above
		double y_scale; // RA2: as above
		if (bounds.Height > bounds.Width) { // RA2: as above
			x_scale = (double)bounds.Height / (double)bounds.Width * 1.2; // RA2: as above
			y_scale = 1.0; // RA2: as above
		} else {
			x_scale = 1.0; // RA2: as above
			y_scale = (double)bounds.Width / (double)bounds.Height * 1.2; // RA2: as above
		}
		double half_width = (double)bounds.Width * 0.5; // RA2: as above
		double inverse_x = 1.0 / (half_width * half_width); // RA2: as above
		double half_height = (double)bounds.Height * 0.5; // RA2: as above
		double inverse_y = 1.0 / (half_height * half_height); // RA2: as above

		while (current < target && tries < 100) { // RA2: as above
			int size = (int)((target - current) * (double)total); // RA2: as above
			size = MIN(size, maximum); // RA2: as above
			current = (double)(placed += Spread_Land(size, bounds, seed, true, Cell(origin.X, origin.Y), 0.75, true)) / (double)total; // RA2: as above
			tries++;

			int nearest = 50000; // RA2: as above
			Cell closest(0, 0); // RA2: as above
			int count = MapCellStride * MapCellStride; // RA2: as above
			int minimum_diagonal = bounds.X * 2 - Map.PlayRect.Width + 1; // RA2: as above
			int maximum_diagonal = minimum_diagonal + bounds.Width * 2 + 2; // RA2: as above
			int minimum_sum = Map.PlayRect.Width + bounds.Y * 2 + 1; // RA2: as above
			int maximum_sum = minimum_sum + bounds.Height * 2 + 2; // RA2: as above
			for (int index = 0; index < count; index++) { // RA2: as above
				MapRegionClass::CellData & data = MapRegionClass::Get_Cell_Data(index); // RA2: as above
				Cell const & candidate = data.CellID; // RA2: as above
				int sum = candidate.X + candidate.Y; // RA2: as above
				int diagonal = candidate.X - candidate.Y; // RA2: as above
				if (sum >= minimum_sum && sum <= maximum_sum && diagonal >= minimum_diagonal && diagonal <= maximum_diagonal && data.SpreadID == 0 && Cell_In_Generation_Shape(candidate, bounds, true, inverse_x, inverse_y)) { // RA2: as above
					double dy = (double)abs(candidate.Y - origin.Y) * y_scale; // RA2: as above
					double dx = (double)abs(candidate.X - origin.X) * x_scale; // RA2: as above
					int distance = (int)(dy + dx); // RA2: as above
					if (distance < nearest) { // RA2: as above
						nearest = distance; // RA2: as above
						closest = candidate; // RA2: as above
					}
				}
			}
			seed = Point2D(closest.X, closest.Y); // RA2: as above
		}
	}
}



void MapGeneratorClass::Green_Shores(void) // RA2: not in TibSun, at 0x57CEB0
{
	Map.Reset_Iterator();
	CellClass * cptr = Map.Iterate();
	while (cptr != NULL) {
		if (cptr->Is_Tile_Shore()) {
			for (int facing = FACING_N; facing < FACING_COUNT; facing += 2) {
				CellClass * adjacent = &cptr->Adjacent_Cell((FacingType)facing);
				if (adjacent->Is_Tile_Clear()) {
					adjacent->ITType = IsometricTileTypeClass::GreenTile;
				}
			}
		}
		cptr = Map.Iterate();
	}
}


void MapGeneratorClass::Scatter_Green_Tiles(void) // RA2: not in TibSun, at 0x57CF20
{
	DynamicVectorClass<CellClass *> candidates;
	candidates.Set_Growth_Step(10000);

	Map.Reset_Iterator();
	CellClass * cptr = Map.Iterate();
	while (cptr != NULL) {
		if (cptr->Is_Tile_Clear_To_Green_LAT()) {
			for (int facing = FACING_N; facing < FACING_COUNT; facing += 2) {
				CellClass * adjacent = &cptr->Adjacent_Cell((FacingType)facing);
				if (adjacent->Is_Tile_Clear()) {
					candidates.Add(adjacent);
				}
			}
		}
		cptr = Map.Iterate();
	}

	int passes = candidates.Count() / 3;
	if (passes > 1000) {
		passes = 1000;
	}
	for (int pass = passes; pass > 0; pass--) {
		int index = Pick_Random_UInt(0, candidates.Count() - 1);
		CellClass * chosen = candidates[index];
		candidates.Delete_Index(index);
		chosen->ITType = IsometricTileTypeClass::GreenTile;
		for (int facing = FACING_N; facing < FACING_COUNT; facing += 2) {
			CellClass * adjacent = &chosen->Adjacent_Cell((FacingType)facing);
			if (adjacent->Is_Tile_Clear()) {
				candidates.Add(adjacent);
			}
		}
	}
}


/// <summary>
/// RA2: the spreader's score for stepping to a cell. Inside a shaped area it is the step's length weighted by how
/// far the cell lies from the area's centre along each diagonal; otherwise the plain distance.
/// </summary>
double MapGeneratorClass::Direction_Weighted_Cell_Distance(Cell const & from, Cell const & to, Rect const & rect, Cell const & center, float weight_x, float weight_y)
{
	if (!(rect == RECT_NONE)) {
		int u = abs(((to.X - to.Y) >> 1) - ((center.X - center.Y) >> 1));
		int v = abs(((to.X + to.Y) >> 1) - ((center.X + center.Y) >> 1));
		if (u == 0 && v == 0) {
			static const float zero = 0.0f;
			return zero;
		}
		double length = fastmath::sqrt((double)(v * v + u * u));
		double ratio_u = u / length;
		double ratio_v = v / length;
		int x = from.X - to.X;
		int y = from.Y - to.Y;
		double steps = MAX(abs(x), abs(y));
		return (weight_x * ratio_u + ratio_v * weight_y) * steps;
	}
	int dx = from.X - to.X;
	int dy = from.Y - to.Y;
	return fastmath::sqrt((double)(dy * dy + dx * dx));
}


/// <summary>
/// RA2: whether a cell lies in the area the spreader may grow over: the ellipse inscribed in the area when shaped,
/// else the area itself; anywhere when there is no area.
/// </summary>
bool MapGeneratorClass::Cell_In_Generation_Shape(Cell const & cell, Rect const & rect, bool shaped, double scale_x, double scale_y)
{
	if (rect == RECT_NONE) {
		return true;
	}
	if (shaped) {
		int a = cell.X - rect.X * 2 - cell.Y + Map.PlayRect.Width - 1;
		int b = cell.X - rect.Y * 2 - Map.PlayRect.Width + cell.Y - 1;
		double u = a * 0.5 - rect.Width * 0.5;
		double v = b * 0.5 - rect.Height * 0.5;
		return (u * u) * scale_x + (v * v) * scale_y < 1.0;
	}
	return rect.Is_Point_Within(Point2D(cell.X, cell.Y));
}


/// <summary>
/// RA2: grows a patch of land (clear tiles in the water) of about count cells as a priority-queue flood fill from
/// the seed point, or from a random clear unclaimed cell when the seed is (0,0). Candidate cells are scored by their
/// distance from a point that wanders off by a normal sample of the wander scale every step -- or, when weighted,
/// by Direction_Weighted_Cell_Distance -- so the patch comes out ragged. The cells taken become the working region;
/// the region's shores are then built. If the shores cannot be built, the region is flooded again.
/// </summary>
/// <returns>The number of cells taken, or 0 when no seed was found or the region was flooded again.</returns>
#ifdef RA2_NON_MATCHING // not exact: no independent bound yet (0057d3a0 has no row)
int MapGeneratorClass::Spread_Land(int count, Rect const & bounds, Point2D const & seed, bool shaped, Cell const & center, double wander, bool weighted)
{
	if (count <= 400) {
		count = 400;
	}
	int heap_size = MAX((8 * count) + 2, 100);
	CellNode * nodes = new CellNode[heap_size];
	PriorityQueueClass<CellNode> * queue = new PriorityQueueClass<CellNode>(heap_size);

	/*
	 * Without a seed, pick a clear cell that no region has claimed yet.
	 */
	Cell start;
	if (seed.X == 0 && seed.Y == 0) {
		int tries = 0;
		do {
			do {
				unsigned int x;
				do {
					x = (unsigned int)RMG_RANDOM_DOUBLE2(Map.PlayRect.Width);
				} while (x > (unsigned int)Map.PlayRect.Width - 1);
				unsigned int y;
				do {
					y = (unsigned int)RMG_RANDOM_DOUBLE2(Map.PlayRect.Height);
				} while (y > (unsigned int)Map.PlayRect.Height - 1);
				start = Cell(x + 1 + y, Map.PlayRect.Width - x + y);
				if (++tries >= 200) {
					return(0);
				}
			} while (RMGCellData == NULL || MapRegionClass::Get_Cell_Data(start).RegionID != 0);
		} while (!Map[start].Is_Tile_Clear());
		if (tries >= 200) {
			return(0);
		}
	} else {
		start = Cell(seed.X, seed.Y);
	}

	/*
	 * A shaped area weights the steps along its longer side less.
	 */
	float weight_x = 0.0f;
	float weight_y = 0.0f;
	if (!(bounds == RECT_NONE)) {
		if (bounds.Height < bounds.Width) {
			weight_x = 1.0f;
			weight_y = (float)bounds.Width / (float)bounds.Height * 1.2;
		} else {
			weight_y = 1.0f;
			weight_x = (float)bounds.Height / (float)bounds.Width * 1.2;
		}
	}

	double wander_x = start.X;
	double wander_y = start.Y;
	int spread_count = 0;

	queue->Clear();
	int marker_index = 0;
	nodes[marker_index].Element = start;
	nodes[marker_index].Score = 0;
	marker_index++;
	MapRegionClass::Get_Cell_Data(start).SpreadID = WorkingRegionID;
	queue->Insert(nodes[0]);
	CellNode * node = queue->Extract_Min();

	double inverse_x = 1.0 / ((double)bounds.Width * 0.5 * (double)bounds.Width * 0.5);
	double inverse_y = 1.0 / ((double)bounds.Height * 0.5 * (double)bounds.Height * 0.5);

	while (spread_count < count && node != NULL) {
		if (RMGCellData != NULL) {
			MapRegionClass::Get_Cell_Data(node->Element).RegionID = WorkingRegionID;
		}
		Map[node->Element].ITType = ISOTILE_CLEAR;

		CellNode * newnode = &nodes[marker_index];
		for (int facing = FACING_N; facing < FACING_COUNT; facing += 2) {
			Cell adjacent = Adjacent_Cell(node->Element, (FacingType)facing);
			if (My_In_Radar(adjacent)) {
				MapRegionClass::CellData & data = MapRegionClass::Get_Cell_Data(adjacent);
				if (data.RegionID == 0 && data.SpreadID != WorkingRegionID && Map[adjacent].Is_Tile_Clear() && marker_index < heap_size &&
					Cell_In_Generation_Shape(adjacent, bounds, shaped, inverse_x, inverse_y)) {

					newnode->Element = adjacent;
					Cell wander_cell((int)wander_x, (int)wander_y);
					if (!weighted) {
						int dx = adjacent.X - wander_cell.X;
						int dy = adjacent.Y - wander_cell.Y;
						newnode->Score = fastmath::sqrt((double)(dx * dx + dy * dy)) + RMG_RANDOM_DOUBLE(5.0);
					} else {
						newnode->Score = Direction_Weighted_Cell_Distance(wander_cell, adjacent, bounds, center, weight_x, weight_y);
					}
					marker_index++;
					data.SpreadID = WorkingRegionID;
					queue->Insert(*newnode++);
				}
			}
		}

		wander_x += RMGGaussian() * wander;
		wander_y += RMGGaussian() * wander;
		spread_count++;
		node = queue->Extract_Min();
	}

	/*
	 * The cells still waiting in the queue join the region too.
	 */
	CellNode * remaining = queue->Extract_Min();
	while (remaining != NULL) {
		CellClass & cell = Map[remaining->Element];
		MapRegionClass::CellData & data = MapRegionClass::Get_Cell_Data(remaining->Element);
		if (data.RegionID == 0 && cell.Is_Tile_Clear()) {
			cell.ITType = ISOTILE_CLEAR;
			data.RegionID = WorkingRegionID;
		}
		remaining = queue->Extract_Min();
		spread_count++;
	}

	delete [] nodes;
	delete queue;

	bool built = Map.Build_All_Shores(WorkingRegionID, 1) != 0;

	Map.Reset_Iterator();
	CellClass * cptr = Map.Iterate();
	while (cptr != NULL) {
		if (cptr->ITType >= IsometricTileTypeClass::ShorePieces && cptr->ITType < IsometricTileTypeClass::ShorePieces + 42) {
			cptr->ITType = ISOTILE_CLEAR;
			cptr->SubTile = 0;
		}
		cptr = Map.Iterate();
	}

	if (built) {
		WorkingRegionID++;
		return(spread_count);
	}

	/*
	 * The shores would not fit: flood the region again.
	 */
	int id = WorkingRegionID;
	Map.Reset_Iterator();
	cptr = Map.Iterate();
	while (cptr != NULL) {
		MapRegionClass::CellData & data = MapRegionClass::Get_Cell_Data(cptr->CellID);
		if (data.RegionID == id) {
			data.RegionID = 0;
			data.CanExpand = false;
			cptr->ITType = IsometricTileTypeClass::WaterSet;
			cptr->SubTile = 0;
			cptr->Height = CellHeight;
		}
		cptr = Map.Iterate();
	}
	return(0);
}
#endif


void MapGeneratorClass::Clear_Shore_Tiles(void) // RA2: not in TibSun, at 0x57DD10
{
	Map.Reset_Iterator();
	CellClass * cptr = Map.Iterate();
	while (cptr != NULL) {
		if (cptr->ITType >= IsometricTileTypeClass::ShorePieces && cptr->ITType < IsometricTileTypeClass::ShorePieces + 42) {
			cptr->ITType = ISOTILE_CLEAR;
			cptr->SubTile = 0;
		}
		cptr = Map.Iterate();
	}
}

















/// <summary>
/// Puts the map's water onto the terrain.
/// This routine is the water phase of generation. A river is attempted first on any map wet
/// enough to carry one, then a lake, each of them retried a few times before the generator
/// gives up and settles for a drier map than was asked for.
/// </summary>
void MapGeneratorClass::Seed_Water(void)
{
	WorkingRegionID++;

	bool river_seeded;
	bool lake_seeded;
	int i;
	int j;

	if ((SeedData.MapType == 3 || SeedData.MapType == 4) && SeedData.WaterAmount > 20) { // RA2: Seed_Water at 0x57DD60 matches only so
		i = 0;
		river_seeded = false;
		while (!river_seeded && i < 10) {
			river_seeded = Seed_River(Cell(0, 0), 0.0, 0); // RA2: as above
			i++;
		}
		if (river_seeded) {
			WorkingRegionID++;
		}
	}

	j = 0;
	lake_seeded = false;
	while (!lake_seeded && j < 10) {
		lake_seeded = Seed_Lake(Cell(0, 0)); // RA2: as above
		j++;
	}
	if (lake_seeded) {
		WorkingRegionID++;
	}
}


void MapGeneratorClass::Vary_Water_Tiles(void) // RA2: not in TibSun, at 0x57DE10
{
	Map.Reset_Iterator();
	CellClass * cptr = Map.Iterate();
	while (cptr != NULL) {
		if (cptr->ITType == IsometricTileTypeClass::WaterSet && cptr->SubTile == 0) {
			CellClass * east = &cptr->Adjacent_Cell(FACING_E);
			CellClass * south = &cptr->Adjacent_Cell(FACING_S);
			CellClass * southeast = &cptr->Adjacent_Cell(FACING_SE);
			if (east->ITType == IsometricTileTypeClass::WaterSet && east->SubTile == 0
				&& south->ITType == IsometricTileTypeClass::WaterSet && south->SubTile == 0
				&& southeast->ITType == IsometricTileTypeClass::WaterSet && southeast->SubTile == 0
				&& Pick_Random_UInt(1, 10) != 1) {
				int pick = Pick_Random_UInt(0, 241);
				int tile_index;
				if (pick >= 240) {
					tile_index = 247 - pick;
				} else {
					tile_index = pick / 40;
				}
				MapGeneratorClass::Place_Tile((IsometricTileType)(IsometricTileTypeClass::WaterSet + tile_index), cptr->CellID, MapRegionClass::CellData::Get_Region(cptr->CellID), -1);
			} else {
				cptr->ITType = (IsometricTileType)(IsometricTileTypeClass::WaterSet + 8 + Pick_Random_UInt(0, 200) / 40);
			}
		}
		cptr = Map.Iterate();
	}
}


bool MapGeneratorClass::Green_Region_Tiles(int region_id) // RA2: not in TibSun, at 0x57DFE0
{
	Map.Reset_Iterator();
	CellClass * cptr = Map.Iterate();
	while (cptr != NULL) {
		if (MapRegionClass::CellData::Get_Region(cptr->CellID) == region_id) {
			if (cptr->ITType == ISOTILE_CLEAR || cptr->ITType == 0xFFFF) {
				cptr->ITType = IsometricTileTypeClass::GreenTile;
			}
		}
		cptr = Map.Iterate();
	}
	return true;
}








/// <summary>
/// RA2
/// </summary>
static inline double Random_Scale(void)
{
	return RMG_RANDOM_DOUBLE2(0.05) + 0.45;
}


/// <summary>
/// RA2
/// </summary>
static inline int Double_Area(Rect const & rect)
{
	return rect.Width * rect.Height * 2;
}


/// <summary>
/// RA2
/// </summary>
static inline int Double_Area(int width, int height)
{
	return width * height * 2;
}








/// <summary>
/// Wipes out a region and hands its cells to another.
/// This routine is used when the generator abandons something it has already laid down. The
/// cells are flattened back to clear terrain at a single height, are barred from expanding
/// again, and become the property of the region named by the caller.
/// </summary>
/// <param name="id">The region whose cells are to be wiped out.</param>
/// <param name="new_id">The region that is to inherit the wiped cells.</param>
/// <param name="cell_height">The height to flatten the cells to, or -1 to use the
/// generator's own working height.</param>
void MapGeneratorClass::Clear_Region_Cells(int id, int new_id, int cell_height, int tile) // RA2: a fourth parameter, the tile type the wiped cells receive (the executable's callers pass it)
{
	Map.Reset_Iterator();
	CellClass *cptr = Map.Iterate();
	while (cptr != NULL) {
		MapRegionClass::CellData & data = MapRegionClass::Get_Cell_Data(cptr->Fetch_CellID());
		if (data.RegionID == id) {
			data.RegionID = new_id;
			data.CanExpand = false;
			cptr->ITType = (IsometricTileType)tile; // RA2: the tile parameter instead of ISOTILE_CLEAR
			cptr->SubTile = 0;
			cptr->Height = cell_height == -1 ? CellHeight : cell_height;
		}
		cptr = Map.Iterate();
	}
}



/// <summary>
/// Seeds a lake on a temperate map.
/// The water spreads out from a seed cell until the map's water budget is spent, and is then
/// handed to the water region builder -- on a mutated map it may be soured into swamp along the
/// way. A lake that comes out too small to be worth keeping is rolled back and the ground
/// restored.
/// </summary>
/// <param name="cell">The cell to grow the lake from, or Cell(0,0) to let the routine choose
/// one.</param>
/// <returns>bool; Was a lake of usable size seeded?</returns>
bool MapGeneratorClass::Seed_Lake(const Cell & cell) // RA2: RA2's form of Seed_Lake (0x57E100)
{
	bool success = true;

	int spread_limit = Get_Target_Water_Amount() - SeededWaterAmount; // RA2: the target comes from Get_Target_Water_Amount
	if (spread_limit <= 75) {
		return(false);
	}

	CellNode *nodes = new CellNode[MAX((2 * spread_limit) + 2, 100)];
	PriorityQueueClass<CellNode> *queue = new PriorityQueueClass<CellNode>(MAX((2 * spread_limit) + 2, 100));

	Map.Reset_Iterator();
	for (CellClass *iter = Map.Iterate(); iter; iter = Map.Iterate()) { // RA2: indexes the cell data directly
		Cell position = iter->CellID; // RA2: as above
		RMGCellData[position.Y * MapCellStride + position.X].SpreadID = 0; // RA2: as above
	}

	int grid_size = MapCellStride * MapCellStride;

	if (cell == Cell(0, 0)) {
		int i;
		for (i = 0; i < grid_size; i++) {
			MapRegionClass::Get_Cell_Data(i).Interior = false;
		}

		Clear_Region_Border(0, 2, -2);

		for (i = 0; i < grid_size; i++) {
			int stored_region = MapRegionClass::Get_Cell_Data(i).RegionID; // RA2: the region is read into a local first
			if (stored_region == 0 || stored_region == WorkingRegionID) { // RA2: as above
				MapRegionClass::Get_Cell_Data(i).Interior = true;
			} else if (stored_region == -2) { // RA2: as above
				MapRegionClass::Get_Cell_Data(i).RegionID = 0; // RA2: as above
			}
		}
	} else {
		for (int i = 0; i < grid_size; i++) {
			MapRegionClass::Get_Cell_Data(i).Interior = true;
		}
	}

	int tries = 0;
	Cell seed_cell;
	if (cell == Cell(0, 0)) {
		for (;;) { // RA2: as above
			int x = Pick_Random_UInt(0, Map.PlayRect.Width - 1); // RA2: as above
			int y = Pick_Random_UInt(0, Map.PlayRect.Height - 1); // RA2: as above

			Cell first_offset = Cell(1, (short)Map.PlayRect.Width) + Cell((short)x, (short)(0u - (unsigned)x)); // RA2: explicit short conversions, in two steps
			seed_cell = first_offset + Cell((short)y, (short)y); // RA2: as above

			if (++tries >= 200) {
				return(false);
			}

			int region = MapRegionClass::CellData::Get_Region(seed_cell); // RA2: kept in a local
			if (region == 0 && Map[seed_cell].Is_Tile_Clear() && RMGCellData[seed_cell.Y * MapCellStride + seed_cell.X].Interior) { // RA2: indexes the cell data directly
				if (tries < 200) {
					break;
				}
				return(false);
			}
		}
	} else {
		seed_cell = cell;
	}

	int seeded_count = 0;

	int max_spread = 76;
	if (spread_limit >= 76) {
		max_spread = spread_limit;
	}
	double mean = (double)(spread_limit / 3);
	int spread_count = (int)Sample_Truncated_Normal_Inline(mean, (double)(spread_limit / 6), 75.0, (double)max_spread); // RA2: the scale is evaluated in the call

	queue->Clear();

	int marker_index = 1;
	nodes[0].Element = seed_cell;
	nodes[0].Score = 0.0f;
	RMGCellData[seed_cell.Y * MapCellStride + seed_cell.X].SpreadID = WorkingRegionID; // RA2: indexes the cell data directly
	queue->Insert(nodes[0]);

	CellNode *node = queue->Extract_Min();
	while (seeded_count < spread_count) { // RA2: no tile randomization and no cell list
		if (node == NULL || !success) { // RA2: as above
			break;
		}

		MapRegionClass::CellData::Set_Region(node->Element, WorkingRegionID); // RA2: the casual set region call

		CellClass *cellptr = &Map[node->Element];
		cellptr->ITType = IsometricTileTypeClass::WaterSet; // RA2: always the first water tile
		cellptr->SubTile = 0; // RA2: as above

		int facing = 0; // RA2: as above
		CellNode *newnode = nodes + marker_index; // RA2: as above
		for (; facing < 8; facing += 2) { // RA2: as above
			Cell newcell = Adjacent_Cell(node->Element, (FacingType)facing); // RA2: as above
			if (My_In_Radar(newcell)) {
				MapRegionClass::CellData &data = RMGCellData[newcell.Y * MapCellStride + newcell.X]; // RA2: as above
				if (data.RegionID == 0 && data.SpreadID != WorkingRegionID && Map[newcell].Is_Tile_Clear() && data.Interior) { // RA2: as above
					newnode->Element = newcell;
					newnode->Score = (float)Get_Spread_Score(seed_cell, newcell, seeded_count); // RA2: as above
					data.SpreadID = WorkingRegionID;
					marker_index++;
					queue->Insert(*newnode++);
				} else if (data.RegionID != 0 && data.RegionID != WorkingRegionID) { // RA2: as above
					success = false;
				}
			}
		}

		seeded_count++;
		node = queue->Extract_Min();
	}

	CellNode *remaining = queue->Extract_Min();
	while (remaining) { // RA2: as above
		if (!success) {
			break;
		}

		CellClass *cellptr = &Map[remaining->Element];
		MapRegionClass::CellData &data = RMGCellData[remaining->Element.Y * MapCellStride + remaining->Element.X]; // RA2: as above
		if (data.RegionID == 0 && cellptr->Is_Tile_Clear() && data.Interior) { // RA2: as above
			cellptr->ITType = IsometricTileTypeClass::WaterSet; // RA2: as above
			cellptr->SubTile = 0;
			data.RegionID = WorkingRegionID;
		} else {
			success = false;
		}

		remaining = queue->Extract_Min();
		seeded_count++;
	}

	if (success) {
		if (seeded_count <= 75 || seeded_count <= spread_count / 4) {
			success = false;
		} else {
			success = true;
			if (cell == Cell(0, 0)) {
				success = Map.Build_All_Shores(WorkingRegionID, false); // RA2: the two-argument form
			}
		}
	}

	if (success && cell == Cell(0, 0)) {
		success = Grow_Region(WorkingRegionID, 1, Rect(0, 0, 512, 512), 0, 0); // RA2: as above
		if (success) {
			int region = WorkingRegionID;
			Map.Reset_Iterator();
			for (CellClass *iter = Map.Iterate(); iter; iter = Map.Iterate()) {
				int index = MapRegionClass::CellData::Get_Region(iter->CellID); // RA2: as above
				if (index == region && (iter->ITType == 0 || iter->ITType == (IsometricTileType)0xffff)) { // RA2: as above
					iter->ITType = IsometricTileTypeClass::GreenTile; // RA2: as above
				}
			}
			success = true;
		}
	}

	delete queue;
	delete[] nodes;

	if (success) {
		SeededWaterAmount += seeded_count;
		return(true);
	}

	Clear_Region_Cells(WorkingRegionID, 0, -1, 0); // RA2: the fourth argument
	return(false);
}




/// <summary>
/// Fetches how much water this map is meant to hold.
/// The figure follows from the water setting the player chose, the biome, and the size of the
/// playable area. The seeding routines treat it as a budget and stop once they have spent it.
/// </summary>
/// <returns>Returns with the wanted number of water cells, or zero for a map that is to have no
/// water at all.</returns>
int MapGeneratorClass::Get_Target_Water_Amount(void)
{
	if (SeedData.WaterAmount == 0) { // RA2: Get_Target_Water_Amount at 0x57EC20 has no float ratios[BIOME_COUNT] = {
		return(0);
	}
	return(0.008 * SeedData.WaterAmount * LocalWidth * LocalHeight + 100.0); // RA2: Get_Target_Water_Amount at 0x57EC20 matches only so
}


/// <summary>
/// Scores a candidate cell for a spreading water body.
/// The flood fill takes the lowest scoring candidate first, so ground near the seed is claimed
/// first while a dose of randomness keeps the resulting shore from coming out round. The score
/// also eases off as the spread wears on, which lets the shape reach further out than it
/// otherwise would.
/// </summary>
/// <param name="cell1">The cell the body is spreading from.</param>
/// <param name="cell2">The candidate cell being scored.</param>
/// <param name="spread_step">How far along the spread already is.</param>
/// <returns>Returns with the candidate's priority. Lower is claimed first.</returns>
double MapGeneratorClass::Get_Spread_Score(const Cell & cell1, const Cell & cell2, int spread_step) const
{
	static const float scale = 0.5;
	static const float _random_range = 10.0f;
	static const float _step_weight = 0.02f;

	int x = cell1.X - cell2.X;
	int y = cell1.Y - cell2.Y;
	float dist = fastmath::sqrt(x * x + y * y);

	return(_random_range * Random_Fraction() + dist * scale - spread_step * _step_weight); // RA2: one expression, the random term first
}



/// <summary>
/// Draws a wandering river of water tiles across the map, perturbing the heading
/// and brush width at each step, occasionally spawning a branch or a waterfall.
/// On success the painted region is converted into proper water regions and, for
/// elevation-4 maps, an optional raised lake edge is built. This is the temperate
/// sibling of Seed_Arctic_River.
/// </summary>
/// <param name="cell">Seed cell, or Cell(0, 0) to pick a random edge seed.</param>
/// <param name="start_angle">Initial heading angle in radians (used when cell is given).</param>
/// <param name="is_branch">True when invoked recursively as a branch sub-call.</param>
/// <returns>True if enough water was painted, false otherwise.</returns>
#ifdef RA2_NON_MATCHING // not exact: 3059 bytes differ (adopted text)
bool MapGeneratorClass::Seed_River(const Cell & cell, double start_angle, bool is_branch) // RA2: RA2's form (0x57ECF0): one water tile, the draws inline, the shores built before the region grows; near miss, 32 bytes short: same instructions but a frame 8 bytes smaller and the zero constant kept in another register
{
	bool orig_subcall = is_branch;
	bool success = true;
	int waterfall_count = 0;

	Cell seed_cell;
	double angle;

	if (cell == Cell(0, 0)) {
		Cell corner(1, Map.PlayRect.Width);
		Cell far_corner = corner + Cell(Map.PlayRect.Width, -Map.PlayRect.Width) + Cell(Map.PlayRect.Height, Map.PlayRect.Height) + Cell(-2, 0);

		int dir4 = (int)Pick_Random_UInt(0, 3);
		int x = (int)Pick_Random_UInt(0, Map.PlayRect.Width - 1);
		int y = (int)Pick_Random_UInt(0, Map.PlayRect.Height - 1);

		Cell candidates[4];
		candidates[0] = corner + Cell(x, -x);
		candidates[1] = far_corner - Cell(y, y);
		candidates[2] = far_corner - Cell(x, -x);
		candidates[3] = corner + Cell(y, y);

		double base_angle = DEG_TO_RAD(315) - (double)dir4 * M_PI_2;

		seed_cell = candidates[dir4];
		angle = Sample_Truncated_Normal(base_angle, DEG_TO_RAD(30),
				base_angle - M_PI_4, base_angle + M_PI_4);
	} else {
		seed_cell = cell;
		angle = start_angle;
	}

	if (!My_In_Radar(seed_cell)) {
		return(false);
	}

	int seeded_count = 0;

	double width_d = (double)SeedData.WaterAmount * 0.07;
	if (1.0 > width_d) {
		width_d = 1.0;
	}
	int width_top = (int)width_d;
	double start_width = (double)Pick_Random_UInt(1, width_top); // RA2: the first width is kept apart from the running one
	double width = start_width; // RA2: as above
	int half = (int)start_width / 2; // RA2: as above
	double width_hi = (double)half + start_width; // RA2: as above
	double width_lo = start_width - (double)half; // RA2: as above

	int length = (int)Pick_Random_UInt(35, 125);

	double cur_angle = angle;
	double cur_x = (double)seed_cell.X + 0.5;
	double cur_y = (double)seed_cell.Y + 0.5;
	double angle_hi = angle + M_PI_2;
	double angle_lo = angle - M_PI_2;

	double stop_roll = 1.0;
	while (My_In_Radar(Cell((int)cur_x, (int)cur_y)) && success) {

		double step_y = fastmath::cos(cur_angle);
		double step_x = fastmath::sin(cur_angle);

		int span = (int)(width + 0.5);
		bool x_run = true;
		bool y_run = true;

		double offset_x = (double)(span - 1) * step_x * 0.5;
		double px = cur_x - offset_x;
		double offset_y = (double)(span - 1) * step_y * 0.5;
		double py = cur_y - offset_y;

		if (span > 0) {
			int i = span;
			Cell pcell;
			while (true) {
				if (My_In_Radar(Cell((int)px, (int)py))) {
					pcell = Cell((int)px, (int)py);
					if (MapRegionClass::CellData::Get_Region(pcell) == 0 // RA2: as above
							&& Map[pcell].Is_Tile_Clear()
							|| MapRegionClass::CellData::Get_Region(pcell) == WorkingRegionID) { // RA2: as above

						if (RMGCellData != NULL) { // RA2: as above
							MapRegionClass::Get_Cell_Data(pcell).RegionID = WorkingRegionID; // RA2: as above
						}
						CellClass * cellptr = &Map[Cell((int)px, (int)py)];
						cellptr->ITType = IsometricTileTypeClass::WaterSet; // RA2: always the first water tile
						cellptr->SubTile = 0; // RA2: as above
					} else {
						success = false;
					}
				}

				double next_x = px + step_x; // RA2: the next position is formed first
				if ((int)px != (int)next_x) { // RA2: as above
					x_run = false;
				}

				double next_y = py + step_y; // RA2: as above
				if ((int)py != (int)next_y) { // RA2: as above
					y_run = false; // RA2: as above
				}

				px = next_x; // RA2: as above
				py = next_y; // RA2: as above
				i--;
				if (i == 0) {
					break;
				}
			}
		}

		if (fabs(step_y) > fabs(step_x)) {
			y_run = false;
		} else {
			x_run = false;
		}

		if (!is_branch && (x_run || y_run) && waterfall_count < 1
				&& (double)abs((int)(cur_angle - angle)) < M_PI_4) {

			if (PlaceWaterfall) {
				if (seeded_count > length) {
					int type;
					if (x_run) {
						type = (step_y > 0.0) ? 2 : 6;
					} else {
						type = (step_x > 0.0) ? 0 : 4;
					}

					bool placed = false;
					Cell from((int)(px - step_x), (int)(py - step_y));
					Cell to((int)(cur_x - offset_x), (int)(cur_y - offset_y));
					Place_Waterfall(WorkingRegionID, to, from, type, placed, cur_x, cur_y);
					if (placed) {
						WorkingRegionID++;
						waterfall_count++;
					}
				}
			}
		}

		cur_x += step_y;
		cur_y -= step_x;

		if (Random_Fraction() < 0.01) {
			if (success) {
				if (!is_branch && waterfall_count == 0) {
					is_branch = true;
					double branch_mean = cur_angle + M_PI_2; // RA2: the truncated draw is open coded
					double branch_lo = cur_angle + DEG_TO_RAD(30); // RA2: as above
					double branch_hi = cur_angle + DEG_TO_RAD(150); // RA2: as above
					double branch_deviation = DEG_TO_RAD(30); // RA2: as above
					if (branch_mean - branch_deviation > branch_hi || branch_mean + branch_deviation < branch_lo) { // RA2: as above
						branch_deviation = (branch_hi - branch_lo) * 0.5; // RA2: as above
						branch_mean = branch_deviation + branch_lo; // RA2: as above
					}
					double branch_angle; // RA2: as above
					do { // RA2: as above
						branch_angle = RMGGaussian() * branch_deviation + branch_mean; // RA2: as above
					} while (branch_angle < branch_lo || branch_angle > branch_hi); // RA2: as above

					Cell branch_cell((int)px, (int)py);
					success = Seed_River(branch_cell, branch_angle, true);
				}
			}
		}

		if (seeded_count > 5) {
			double delta_angle = Sample_Truncated_Normal(0.0, DEG_TO_RAD(18),
					angle_lo - cur_angle, angle_hi - cur_angle);
			cur_angle += delta_angle;
		}

		if (half > 0) {
			double step_hi = width_hi - width; // RA2: the truncated draw is open coded
			double step_lo = width_lo - width; // RA2: as above
			double step_dev = 0.5; // RA2: as above
			double step_mean = 0.0; // RA2: as above
			if (step_mean - step_dev > step_hi || step_mean + step_dev < step_lo) { // RA2: as above
				step_dev = (step_hi - step_lo) * 0.5; // RA2: as above
				step_mean = step_dev + step_lo; // RA2: as above
			}
			double delta_width; // RA2: as above
			do { // RA2: as above
				delta_width = RMGGaussian() * step_dev + step_mean; // RA2: as above
			} while (delta_width < step_lo || delta_width > step_hi); // RA2: as above
			width += delta_width;
		}

		stop_roll = Random_Fraction();
		seeded_count++;
		if (stop_roll < 0.005) {
			break;
		}
	}

	if (seeded_count < 40) {
		success = false;
	}

	if (stop_roll < 0.005) {
		if (My_In_Radar(Cell((int)cur_x, (int)cur_y)) && success) {
			if (!Seed_Lake(Cell((int)cur_x, (int)cur_y))) {
				success = false;
			}
		}
	}

	if (!orig_subcall && success) {
		success = Map.Build_All_Shores(WorkingRegionID, false); // RA2: the two-argument form
		if (success) { // RA2: the shore is grown out and the cells still clear are made green
			success = Grow_Region(WorkingRegionID, 1, Rect(0, 0, MAP_CELL_W, MAP_CELL_H), 0, 0); // RA2: as above
			int region_id = WorkingRegionID; // RA2: as above
			Map.Reset_Iterator(); // RA2: as above
			CellClass * cptr = Map.Iterate(); // RA2: as above
			while (cptr != NULL) { // RA2: as above
				if (MapRegionClass::CellData::Get_Region(cptr->CellID) == region_id && (cptr->ITType == ISOTILE_CLEAR || cptr->ITType == ISOTILE_INVALID)) { // RA2: as above
					cptr->ITType = IsometricTileTypeClass::GreenTile; // RA2: as above
				} // RA2: as above
				cptr = Map.Iterate(); // RA2: as above
			} // RA2: as above
		}
	}

	bool raised = false;

	if (waterfall_count == 0 && success && CellHeight == 4 && !orig_subcall) {

		static const double _raise_chance = 0.7; // RA2: a named constant of the function: the executable reads this 0.7 at 0x7A5CB0, among the function statics
		if (Random_Fraction() < _raise_chance) { // RA2: no biome test

			Rect bounds(0, 0, MAP_CELL_W, MAP_CELL_H);
			if (Grow_Water_Region(WorkingRegionID, 0.01f, bounds, seed_cell, true)) {
				success = Grow_Region(WorkingRegionID, 6, Rect(0, 0, MAP_CELL_W, MAP_CELL_H), 0, 0);
				if (success) {
					int region_id = WorkingRegionID;
					Map.Reset_Iterator();
					CellClass * cptr = Map.Iterate();
					while (cptr != NULL) {
						Cell c = cptr->CellID;
						int rid = MapRegionClass::CellData::Get_Region(c); // RA2: the grid accessor
						if (rid != region_id) {
							cptr->Height += 4;
						}
						cptr = Map.Iterate();
					}
					raised = true;
				}
			} else {
				success = false;
			}
		}
	}

	if (!orig_subcall && success && waterfall_count == 0) {
		if (!raised) {
			success = Grow_Region(WorkingRegionID, 2, Rect(0, 0, MAP_CELL_W, MAP_CELL_H), 0, 0);
		}
	} else if (waterfall_count > 0) {
		if (success) {
			success = Grow_Region(WorkingRegionID, 2, Rect(0, 0, MAP_CELL_W, MAP_CELL_H), 1, CellHeight);
		}
	}

	if (success) {
		if (raised) {
			CellHeight += 4;
		}
		SeededWaterAmount += seeded_count;
		return(true);
	}

	Clear_Region_Cells(WorkingRegionID, 0, -1, ISOTILE_CLEAR); // RA2: the fourth argument (RA2's wipe takes a tile)

	if (waterfall_count > 0) {
		Clear_Region_Cells(WorkingRegionID - 1, 0, -1, ISOTILE_CLEAR); // RA2: as above
	}

	return(false);
}
#endif



/// <summary>
/// Levels a region to a single height.
/// Every cell the region owns is forced to the height given, which is how the generator makes a
/// plateau or a basin out of ground that was grown at whatever height it happened to find.
/// </summary>
/// <param name="id">The region to level.</param>
/// <param name="cell_height">The height every one of its cells takes.</param>
void MapGeneratorClass::Set_Height_Of_ID(int id, char cell_height)
{
	Map.Reset_Iterator();
	CellClass * cptr = Map.Iterate(); // RA2: Set_Height_Of_ID at 0x57FE20 matches only so
	while (cptr != NULL) {
		if (MapRegionClass::CellData::Get_Region(cptr->Fetch_CellID()) == id) { // RA2: as above
			cptr->Height = cell_height;
		}
		cptr = Map.Iterate();
	}
}


/// <summary>
/// Raises the terrain belonging to one region.
/// This routine lifts a region bodily, so that the ground it owns keeps whatever shape it was
/// grown with while rising above its neighbors.
/// </summary>
/// <param name="id">The region to raise.</param>
/// <param name="cell_height">The amount each of its cells is to rise by.</param>
void MapGeneratorClass::Increase_Height_Of_ID(int id, char cell_height)
{
	Map.Reset_Iterator();
	CellClass * cptr = Map.Iterate(); // RA2: Increase_Height_Of_ID at 0x57FEA0 matches only so
	while (cptr != NULL) {
		if (MapRegionClass::CellData::Get_Region(cptr->Fetch_CellID()) == id) { // RA2: as above
			cptr->Height += cell_height;
		}
		cptr = Map.Iterate();
	}
}


/// <summary>
/// Places a waterfall where a river spills off a shelf.
/// This routine is used by the river seeding to carry water down a change in height. The ground
/// beyond the drop is examined for room; if there is any, it is painted with water, raised into
/// a shelf of its own, and dressed with the waterfall tiles for the heading given. When there is
/// no room the terrain is left exactly as it was found and the river must go elsewhere.
/// </summary>
/// <param name="region_id">The region the painted spillway cells are given to.</param>
/// <param name="cell1">One end of the river edge the waterfall hangs from.</param>
/// <param name="cell2">The other end of that river edge.</param>
/// <param name="direction">The cardinal heading the water falls toward -- 0 north, 2 east,
/// 4 south, 6 west.</param>
/// <param name="placed">Set to whether the waterfall went in. Mirrors the return value.</param>
/// <param name="head_x">The river head X, carried past the waterfall on success.</param>
/// <param name="head_y">The river head Y, carried past the waterfall on success.</param>
/// <returns>bool; Was the waterfall placed?</returns>
#ifdef RA2_NON_MATCHING // not exact: 4 bytes differ (adopted text)
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
		success = Map.Build_All_Shores(WorkingRegionID, false); // RA2: RA2's form
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
						} while (seg < patch.Height); // RA2: as above
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
						} while (seg < patch.Height); // RA2: as above
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
						} while (seg < patch.Width); // RA2: as above
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
						} while (seg < patch.Width); // RA2: as above
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
#endif



/// <summary>
/// Assigns a cell to a region.
/// This routine is the generator's shorthand for the region data hanging off each cell.
/// </summary>
void MapGeneratorClass::Set_Cell_Data_Region(Cell const &cell, int region_id)
{
	MapRegionClass::CellData::Set_Region(cell, region_id);
}


/// <summary>
/// Fetches the region a cell belongs to.
/// This routine is the generator's shorthand for the region data hanging off each cell.
/// </summary>
int MapGeneratorClass::Get_Cell_Data_Region(Cell const &cell)
{
	return(MapRegionClass::CellData::Get_Region(cell));
}


/// <summary>
/// Marks a cell as reached by the spread currently running.
/// This routine is the generator's shorthand for the region data hanging off each cell.
/// </summary>
void MapGeneratorClass::Set_Cell_Data_Spread(Cell const &cell, int patch_id)
{
	MapRegionClass::CellData::Set_Spread(cell, patch_id);
}


/// <summary>
/// Fetches the id of the spread that last reached a cell.
/// This routine is the generator's shorthand for the region data hanging off each cell.
/// </summary>
int MapGeneratorClass::Get_Cell_Data_Spread(Cell const &cell)
{
	return(MapRegionClass::CellData::Get_Spread(cell));
}


/// <summary>
/// Grows a region outward into the ground around it.
/// The region spreads from its own border, claiming unowned clear ground that lies within the
/// bounding rectangle, and gives up the moment it runs into ground another region already owns.
/// This routine is how the generator turns a seed into a region of usable size.
/// </summary>
/// <param name="id">The region to grow.</param>
/// <param name="rings">How far out from its border the region may spread.</param>
/// <param name="rect">The area the region is confined to.</param>
/// <param name="set_height">Should the claimed ground be flattened to the given height, taking
/// over the preceding region and any water it meets along the way?</param>
/// <param name="height">The height given to newly claimed ground.</param>
/// <returns>bool; Did the region grow without running into a neighbor?</returns>
bool MapGeneratorClass::Grow_Region(int id, int rings, Rect rect, char set_height, char height)
{
	DynamicVectorClass<Cell> * cells = Build_Region_Border_Cell_List(id);

	DynamicVectorClass<Cell> * ncells;
	for (int i = 0; i < rings; i++) {
		ncells = new DynamicVectorClass<Cell>;
		ncells->Set_Growth_Step(3 * cells->Count());

		for (int j = 0; j < cells->Count(); j++) {
			for (int dir = 0; dir < FACING_COUNT; dir++) {
				Cell ncell = Adjacent_Cell((*cells)[j], (FacingType)dir);
				if (My_In_Radar(ncell) && rect.Is_Point_Within(Point2D(ncell.X, ncell.Y))) {
					MapRegionClass::CellData & data = MapRegionClass::Get_Cell_Data(ncell);
					int rid = data.RegionID;
					bool reclaim = set_height && rid == id - 1;
					CellClass * cptr = &Map[ncell];
					if ((rid == 0 || reclaim) && (cptr->Is_Tile_Clear() || (reclaim && cptr->Is_Tile_With_Water()))) {
						ncells->Add(ncell);
						data.RegionID = id;
						if (set_height && cptr->Is_Tile_Clear()) {
							Map[ncell].Height = height;
						}
					} else if (rid != id) {
						delete cells;
						delete ncells;
						return(false);
					}
				}
			}
		}

		delete cells;
		cells = ncells;
	}

	delete cells;
	return(true);
}


/// <summary>
/// Flattens a band of terrain around the edge of a region.
/// The cells along the border are stripped back to plain clear ground at the working height and
/// handed to another region. This routine is used to give a region a clean skirt before
/// anything is grown or placed against it.
/// </summary>
/// <param name="id">The region whose border is to be cleared.</param>
/// <param name="rings">How many cells deep the cleared band should reach.</param>
/// <param name="new_id">The region the cleared cells are handed to.</param>
/// <returns>bool; Always true.</returns>
bool MapGeneratorClass::Clear_Region_Border(int id, int rings, int new_id)
{
	int i;
	int j;

	DynamicVectorClass<Cell> * cells = Build_Region_Border_Cell_List(id);

	if (rings > 1) {
		Map.Reset_Iterator();
		CellClass *cptr = Map.Iterate();
		while (cptr != NULL) {
			MapRegionClass::Get_Cell_Data(cptr->Fetch_CellID()).SpreadID = 0;
			cptr = Map.Iterate();
		}
	}

	for (i = 0; i < rings; i++) {

		for (j = cells->Count() - 1; j >= 0; j--) {
			Cell c = (*cells)[j];
			MapRegionClass::CellData::Set_Region(c, new_id);
			CellClass *cptr = &Map[c];
			cptr->ITType = ISOTILE_CLEAR;
			cptr->SubTile = 0;
			cptr->Height = CellHeight;
		}

		if (i < rings - 1) {
			DynamicVectorClass<Cell> * ncells = new DynamicVectorClass<Cell>;
			ncells->Set_Growth_Step(cells->Count());

			for (j = cells->Count() - 1; j >= 0; j--) {
				Cell c = (*cells)[j];
				for (int dir = 0; dir < FACING_COUNT; dir++) {
					Cell ncell = Adjacent_Cell(c, (FacingType)dir);
					if (My_In_Radar(ncell)) {
						MapRegionClass::CellData & data = MapRegionClass::Get_Cell_Data(ncell);
						if (data.RegionID == id && data.SpreadID != i + 1) {
							ncells->Add(ncell);
							data.SpreadID = i + 1;
						}
					}
				}
			}

			delete cells;
			cells = ncells;
		}
	}

	delete cells;

	if (rings > 1) {
		Map.Reset_Iterator();
		CellClass *cptr = Map.Iterate();
		while (cptr != NULL) {
			MapRegionClass::Get_Cell_Data(cptr->Fetch_CellID()).SpreadID = 0; // RA2: Clear_Region_Border at 0x581BF0 has no MapCellStride
			cptr = Map.Iterate();
		}
	}

	return(true);
}


/// <summary>
/// Gathers the border cells of a region.
/// A cell counts as border when at least one of its neighbors belongs to somebody else. The
/// growing and clearing routines take this list as the frontier they work outward from.
/// </summary>
/// <param name="id">The region whose boundary is wanted.</param>
/// <returns>Returns with a pointer to a freshly allocated list of the region's border cells.
/// The caller owns the list and must delete it.</returns>
DynamicVectorClass<Cell> *MapGeneratorClass::Build_Region_Border_Cell_List(int id)
{
	DynamicVectorClass<Cell> *cells = new DynamicVectorClass<Cell>;
	cells->Set_Growth_Step(LocalWidth * LocalHeight);

	int grid_size = MapCellStride * MapCellStride;
	for (int i = 0; i < grid_size; i++) {
		Cell cell = RMGCellData[i].CellID;
		if (cell != Cell(0, 0) && MapRegionClass::CellData::Get_Region(cell) == id) {
			bool add = false;
			for (FacingType facing = FACING_FIRST; facing < FACING_COUNT; facing++) { // RA2: Build_Region_Border_Cell_List at 0x581EE0 matches only so
				Cell ncell = Adjacent_Cell(cell, facing); // RA2: as above
				if (My_In_Radar(ncell)) {
					if (MapRegionClass::CellData::Get_Region(ncell) != id) {
						add = true;
						break;
					}
				}
			}

			if (add) {
				cells->Add(cell);
			}
		}
	}

	return(cells);
}


/// <summary>
/// Grows the region identified by region_id outward across the map as a
/// priority-queue flood fill, scoring candidate cells by their angle relative
/// to a reference cell. Border cells that abut a different region clear the
/// success flag.
/// </summary>
/// <param name="region_id">Region id written into the cell data of claimed cells.</param>
/// <param name="spread_scale">Scale factor controlling how far the region spreads.</param>
/// <param name="bounds">Bounding rectangle the spread is confined to.</param>
/// <param name="origin">Reference origin used when computing the spread angle.</param>
/// <param name="claim_frontier">When true, drain the remaining frontier into the region.</param>
/// <returns>True if the region grew without colliding with another region.</returns>
#ifdef RA2_NON_MATCHING // not exact: no independent bound yet (its callers Seed_River and Place_Waterfall are not exact, so no row bounds it)
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
#endif


/// <summary>
/// Scores how far a step strays from the wanted heading.
/// The generator uses this routine to bias a spread toward a preferred direction -- a low score
/// means the step points the right way. A little randomness is mixed in so that two equally
/// good candidates do not always resolve the same way.
/// </summary>
/// <param name="cell1">The cell being stepped from.</param>
/// <param name="cell2">The cell being stepped to.</param>
/// <param name="ref_angle">The preferred heading, in radians.</param>
/// <returns>Returns with the penalty for this step. Lower is better.</returns>
double MapGeneratorClass::Get_Angle_Score(const Cell & cell1, const Cell & cell2, double ref_angle)
{
	static const double _random_weight = 2.0;
	static const double _angle_weight = 1.5;

	int dx = cell2.X - cell1.X;
	int dy = cell2.Y - cell1.Y;

	double angle = (dx != 0) ? fastmath::atan(-((double)dy / (double)dx)) : M_PI_2;
	if (dx < 0) {
		angle += M_PI;
	}

	double angle_diff = fabs(angle - ref_angle);
	while (angle_diff >= M_PI * 2) {
		angle_diff -= M_PI * 2;
	}
	if (angle_diff > M_PI) {
		angle_diff = M_PI * 2 - angle_diff;
	}

	return(RMG_RANDOM_DOUBLE2(_random_weight) + _angle_weight * angle_diff);
}


/// <summary>
/// Clears the spread marker from every cell, back to zero.
/// This routine wipes the markers so that the next routine to spread out from a seed starts
/// from a clean map. Note that Reset_Spreads clears to -1 rather than to zero.
/// </summary>
void MapGeneratorClass::Clear_Cell_Data_Spreads(void)
{
	Map.Reset_Iterator();
	CellClass *cptr = Map.Iterate();
	while (cptr != NULL) {
		Set_Cell_Data_Spread(cptr->Fetch_CellID(), 0);
		cptr = Map.Iterate();
	}
}


/// <summary>
/// Raises the terrain everywhere outside one region.
/// Use this routine to sink a region relative to the rest of the map; the named region keeps
/// the height it has while the world around it climbs.
/// </summary>
/// <param name="cell_height">The amount every other cell is to rise by.</param>
/// <param name="id">The region to leave alone.</param>
void MapGeneratorClass::Increase_Height_Except_ID(int cell_height, int id)
{
	Map.Reset_Iterator();
	CellClass *cptr = Map.Iterate();
	while (cptr != NULL) {
		int rid = Get_Cell_Data_Region(cptr->Fetch_CellID());
		if (rid != id) {
			cptr->Height += cell_height;
		}
		cptr = Map.Iterate();
	}
}


IsometricTileType MapGeneratorClass::Pick_Alternate_Cliff(IsometricTileType type) // RA2: not in TibSun, at 0x582B30
{
	int cliff = type - IsometricTileTypeClass::CliffSet;
	switch (cliff) {
		case 4:
			cliff = 4 + Pick_Random_UInt(1, 2);
			break;
		case 5:
			cliff = 4 + Pick_Random_UInt(0, 1) * 2;
			break;
		case 6:
			cliff = 4 + Pick_Random_UInt(0, 1);
			break;
		case 8:
			cliff = 8 + Pick_Random_UInt(1, 2);
			break;
		case 9:
			cliff = 8 + Pick_Random_UInt(0, 1) * 2;
			break;
		case 10:
			cliff = 8 + Pick_Random_UInt(0, 1);
			break;
		case 11:
			cliff = 11 + Pick_Random_UInt(1, 2);
			break;
		case 12:
			cliff = 11 + Pick_Random_UInt(0, 1) * 2;
			break;
		case 13:
			cliff = 11 + Pick_Random_UInt(0, 1);
			break;
		case 14:
			cliff = 14 + Pick_Random_UInt(1, 2);
			break;
		case 15:
			cliff = 14 + Pick_Random_UInt(0, 1) * 2;
			break;
		case 16:
			cliff = 14 + Pick_Random_UInt(0, 1);
			break;
		case 22:
			cliff = 22 + Pick_Random_UInt(1, 2);
			break;
		case 23:
			cliff = 22 + Pick_Random_UInt(0, 1) * 2;
			break;
		case 24:
			cliff = 22 + Pick_Random_UInt(0, 1);
			break;
		case 28:
			cliff = 29;
			break;
		case 29:
			cliff = 28;
			break;
		case 34:
			cliff = 34 + Pick_Random_UInt(1, 2);
			break;
		case 35:
			cliff = 34 + Pick_Random_UInt(0, 1) * 2;
			break;
		case 36:
			cliff = 34 + Pick_Random_UInt(0, 1);
			break;
	}
	return (IsometricTileType)(IsometricTileTypeClass::CliffSet + cliff);
}


/// <summary>
/// RA2: where a cliff piece continues into the same piece to the south or east, gives the continuation a random
/// variant of the piece so long cliff runs do not repeat.
/// </summary>
void MapGeneratorClass::Vary_Cliff_Runs(void)
{
	Map.Reset_Iterator();
	CellClass * cptr = Map.Iterate();
	while (cptr != NULL) {
		if (cptr->ITType >= IsometricTileTypeClass::CliffSet && cptr->ITType < IsometricTileTypeClass::CliffSet + 40) {
			CellClass & south = (CellClass &)cptr->Adjacent_Cell(FACING_S);
			CellClass & east = (CellClass &)cptr->Adjacent_Cell(FACING_E);
			if (cptr->ITType == south.ITType && cptr->SubTile > south.SubTile) {
				IsometricTileType tile = Pick_Alternate_Cliff(cptr->ITType);
				if (tile != cptr->ITType) {
					int subtile = south.SubTile;
					IsometricTileTypeClass * type = IsometricTileTypes[cptr->ITType];
					Cell origin = south.CellID - Cell(subtile % type->Width, subtile / type->Width); // RA2: RA2's form
					Place_Tile(tile, origin, Get_Cell_Data_Region(origin), -1);
				}
			}
			if (cptr->ITType == east.ITType && cptr->SubTile > east.SubTile) {
				IsometricTileType tile = Pick_Alternate_Cliff(cptr->ITType);
				if (tile != cptr->ITType) {
					IsometricTileTypeClass * type = IsometricTileTypes[cptr->ITType];
					int subtile = east.SubTile;
					Cell origin = east.CellID - Cell(subtile % type->Width, subtile / type->Width); // RA2: as above
					Place_Tile(tile, origin, Get_Cell_Data_Region(origin), -1);
				}
			}
		}
		cptr = Map.Iterate();
	}
}








/// <summary>
/// RA2
/// </summary>
static inline bool Is_Special_Map_Type(MapSeedClass const & seed)
{
	return(seed.MapType == 3 || seed.MapType == 4);
}


/// <summary>
/// Fills in the awkward corners left where two cliffs meet.
/// A cell walled in by high ground on three sides at a corner, with another such cell beside
/// it, gets the clear cell next to it lowered (or, at the inner corner, itself raised) so
/// that the cliff pieces can join up. The lowered cells are handed to the region of the cell
/// that caused the change.
/// </summary>
void MapGeneratorClass::Fixup_Cliff_Corners(void) // RA2: not in TibSun, at 0x5831C0
{
	Map.Reset_Iterator(); // RA2
	CellClass * cptr = Map.Iterate(); // RA2
	while (cptr != NULL) { // RA2
		CellClass & south_west = (CellClass &)cptr->Adjacent_Cell(FACING_SW); // RA2
		CellClass & north_east = (CellClass &)cptr->Adjacent_Cell(FACING_NE); // RA2
		CellClass & south_east = (CellClass &)cptr->Adjacent_Cell(FACING_SE); // RA2
		CellClass & east = (CellClass &)cptr->Adjacent_Cell(FACING_E); // RA2
		CellClass & west = (CellClass &)cptr->Adjacent_Cell(FACING_W); // RA2
		unsigned int mask = Map.Get_High_Ground_Mask(cptr); // RA2
		if (mask == 0x83) { // RA2
			if (Map.Get_High_Ground_Mask(&south_east) == 0x83 && east.Is_Tile_Clear()) { // RA2
				unsigned int choice; // RA2
				do { // RA2
					choice = (unsigned int)RMGRandom() * (2.0 / 4294967295.0); // RA2
				} while (choice > 1); // RA2
				if (choice == 0) { // RA2
					east.Height = east.Height - 4; // RA2
					MapRegionClass::CellData::Set_Region(east.CellID, MapRegionClass::CellData::Get_Region(cptr->CellID)); // RA2
				} // RA2
			} // RA2
		} else if (mask == 0x38) { // RA2
			if (Map.Get_High_Ground_Mask(&south_east) == 0x38 && west.Is_Tile_Clear()) { // RA2
				unsigned int choice; // RA2
				do { // RA2
					choice = (unsigned int)RMGRandom() * (2.0 / 4294967295.0); // RA2
				} while (choice > 1); // RA2
				if (choice == 0) { // RA2
					west.Height = west.Height - 4; // RA2
					MapRegionClass::CellData::Set_Region(west.CellID, MapRegionClass::CellData::Get_Region(cptr->CellID)); // RA2
				} // RA2
			} // RA2
		} else if (mask == 0xE0) { // RA2
			if (Map.Get_High_Ground_Mask(&south_west) == 0xE0 && Map.Get_High_Ground_Mask(&north_east) == 0xE0 && cptr->Is_Tile_Clear()) { // RA2
				cptr->Height = cptr->Height + 4; // RA2
				MapRegionClass::CellData::Set_Region(cptr->CellID, MapRegionClass::CellData::Get_Region(west.CellID)); // RA2
			} // RA2
		} // RA2
		cptr = Map.Iterate(); // RA2
	} // RA2
}


/// <summary>
/// Rebuilds the generator's regions from scratch.
/// Whatever regions an earlier pass left behind are thrown away and derived afresh from the
/// terrain as it now stands. The water and clear ground regions are settled first and given
/// room to breathe (more of it on the inland and mountain maps, and for the larger ones), so
/// that the regions of whatever is left over fill in around them.
/// </summary>
void MapGeneratorClass::Init_Regions(void)
{
	int i;

	MapRegionClass::Destroy_Cell_Regions();
	MapRegionClass::Create_Water_Regions();

	for (i = 0; i < MapRegionClass::MapRegions.Count(); i++) {
		MapRegionClass *ptr = MapRegionClass::MapRegions[i];
		if (ptr->ContainsWater) {
			ptr->Grow(4 + (ptr->CellCount > 8000) + !Is_Special_Map_Type(SeedData)); // RA2: wider for big regions and off the inland and mountain maps
			ptr->Claim_Expandable_Cells();
		}
	}

	MapRegionClass::Create_Land_Regions(false);
}


/// <summary>
/// Builds the region map for the generated terrain.
/// Only the inland and mountain maps are given cliffs this way: the ice regions are gathered,
/// the regions raised, the cliff corners tidied up, the cliffs built and their runs varied.
/// </summary>
void MapGeneratorClass::Make_Regions(void) // RA2: returns nothing
{
	if (SeedData.MapType == 4 || SeedData.MapType == 3) { // RA2: only the inland and mountain maps
		MapRegionClass::Make_Ice_Regions(); // RA2: as above
		MapRegionClass::Make_Regions();
		Fixup_Cliff_Corners(); // RA2
		Map.Build_All_Cliffs(0, -1); // RA2
		Vary_Cliff_Runs(); // RA2
	} // RA2
}


/// <summary>
/// Determines if a region holds enough of an area to count.
/// Build_Neighbor_Region_Mask uses this routine to decide which of the blocks around a cell
/// belong to a region, and so which directions a ramp may be run in. Counting stops as soon as
/// the answer is settled.
/// </summary>
/// <param name="rect">The area of cells to examine.</param>
/// <param name="region_id">The region being looked for.</param>
/// <param name="count">The number of cells the region must own for the test to pass.</param>
/// <returns>bool; Does the region own at least that many cells within the area?</returns>
bool MapGeneratorClass::Region_Cell_Count_At_Least(Rect const & rect, int region_id, int, int count)
{
	Cell cell(rect.X, rect.Y);
	int actual = 0;

	for (int y = 0; y < rect.Height; y++) {
		for (int x = 0; x < rect.Width; x++) {
			if (My_In_Radar(cell + Cell(x, y))) {
				if (MapRegionClass::CellData::Get_Region(cell + Cell(x, y)) == region_id) {
					actual++;
					if (actual >= count) {
						return(true);
					}
				}
			}
		}
	}

	return(false);
}


/// <summary>
/// Expands the high ground over the whole map.
/// This routine is the map wide driver for MapClass::Expand_High_Ground. It stops at the
/// first cell that will not expand, so that the caller can throw the map away and start again.
/// </summary>
/// <returns>bool; Did every cell expand successfully?</returns>
bool MapGeneratorClass::Expand_All_High_Ground(void)
{
	bool success = true;
	Map.Set_Cursor_Shape(NULL);
	Map.Reset_Iterator();
	CellClass *cptr = cptr = Map.Iterate();
	while (cptr != NULL) {
		if (!success) {
			break;
		}
		success = Map.Expand_High_Ground(cptr, -1);
		cptr = Map.Iterate();
	}
	return(success);
}


/// <summary>
/// Floods clear, in-radar cells outward from every player's start waypoint, marking each
/// visited cell Inviolate. A player passes only if the flood reaches exactly 400 cells; if any
/// player's flood runs dry first, the Inviolate flags are cleared and the routine fails so the
/// caller regenerates the map.
/// </summary>
/// <returns>True if every player's start point spreads 400 cells; false (after clearing the
/// Inviolate flags) if any player falls short.</returns>
bool MapGeneratorClass::Init_Start_Points(void)
{
	int player_index = 0;

	if (SeedData.NumPlayers > 0) {
		do { // RA2: Init_Start_Points at 0x583790 matches only so
			Cell seed = Scen->Get_Waypoint_Cell((WAYPOINT)player_index);

			CellNode * nodes = new CellNode[800];
			PriorityQueueClass<CellNode> * queue = new PriorityQueueClass<CellNode>(800);

			Clear_Cell_Data_Spreads();
			int spread_count = 0; // RA2: as above
			player_index++; // RA2: as above
			queue->Clear();

			int node_count = 1; // RA2: Init_Start_Points at 0x583790 has no int patch_id = player_index + 1;
			nodes[0].Element = seed; // RA2: Init_Start_Points at 0x583790 has no int spread_count = 0;
			nodes[0].Score = 0.0f;
			MapRegionClass::Get_Cell_Data(seed).SpreadID = player_index; // RA2: as above
			queue->Insert(nodes[0]);

			CellNode * node = queue->Extract_Min();
			while (node != NULL && spread_count < 400) { // RA2: as above
				MapRegionClass::Get_Cell_Data(node->Element).Inviolate = true;

				FacingType dir = FACING_FIRST; // RA2: as above
				CellNode * newnode = &nodes[node_count];
				for (; dir < FACING_COUNT; dir++) { // RA2: as above
					Cell ncell = Adjacent_Cell(node->Element, dir);
					if (My_In_Radar(ncell) && MapRegionClass::Get_Cell_Data(ncell).SpreadID == 0) {
						if (Map[ncell].Is_Tile_Clear()) {
							newnode->Element = ncell;
							int dx = ncell.X - seed.X;
							int dy = ncell.Y - seed.Y;
							newnode->Score = fastmath::sqrt((double)(dx * dx + dy * dy));
							MapRegionClass::Get_Cell_Data(ncell).SpreadID = player_index; // RA2: as above
							node_count++;
							queue->Insert(*newnode++);
						}
					}
				}

				spread_count++;
				node = queue->Extract_Min();
			} // RA2: Init_Start_Points at 0x583790 has no if (node == NULL) {

			delete[] nodes;
			delete queue;

		} while (player_index < SeedData.NumPlayers); // RA2: as above
	}
	return(true);
}


/// <summary>
/// RA2: the number of player start points in a region.
/// </summary>
static inline int Start_Count(MapRegionClass const * region)
{
	return(region->StartCount);
}


/// <summary>
/// RA2: do the players start next to gems rather than ore?
/// </summary>
static inline bool Rich_Starts(MapSeedClass const & seed)
{
	return(seed.Resources == 3 && (seed.MapType == 1 || seed.MapType == 3 || seed.MapType == 4));
}


/// <summary>
/// RA2: the straight line distance between two cells.
/// </summary>
static inline double Cell_Distance(Cell const & c, int x, int y)
{
	int dx = c.X - x;
	int dy = c.Y - y;
	return(fastmath::sqrt((double)(dy * dy + dx * dx)));
}


/// <summary>
/// Distributes tiberium across the map.
/// This routine works region by region: it seeds a field at each of the region's spread
/// cells, then hands out a compensating field to each player starting there -- the further a
/// player sits from the ore already placed, the more of it is put on their doorstep, so that
/// nobody begins the game starved.
/// </summary>
void MapGeneratorClass::Create_Tiberium(void)
{
	int waypoint = 0; // RA2: Create_Tiberium at 0x583B80, RA2's form (fields per region, grown from its StartCells)
	bool seek_center; // RA2: as above
	int resources = SeedData.Resources; // RA2: as above
	switch (SeedData.MapType) { // RA2: as above
		case 0: seek_center = resources == 3; break; // RA2: as above
		case 1: // RA2: as above
		case 3: // RA2: as above
		case 4: seek_center = resources != 3; break; // RA2: as above
		case 2: seek_center = true; break; // RA2: as above
		default: seek_center = false; break; // RA2: as above
	} // RA2: as above

	for (int index = 0; index < MapRegionClass::MapRegions.Count(); index++) { // RA2: as above
		MapRegionClass * region = MapRegionClass::MapRegions[index]; // RA2: as above
		if (region->StartCells == NULL) continue; // RA2: as above

		int nearest = -1; // RA2: as above
		if (seek_center) { // RA2: as above
			Cell center(0, 0); // RA2: as above
			if (region->StartCount > 0) { // RA2: as above
				for (int player = 0; player < region->StartCount; player++) { // RA2: as above
					Cell start = Scen->Get_Waypoint_Cell((WAYPOINT)(player + waypoint)); // RA2: as above
					center.Y += start.Y; // RA2: as above
					center.X += start.X; // RA2: as above
				} // RA2: as above
				center.X = center.X / region->StartCount; // RA2: as above
				center.Y = center.Y / region->StartCount; // RA2: as above
			} else { // RA2: as above
				center = region->Cells[Pick_Random_UInt(0, region->Cells.Count() - 1)]; // RA2: as above
			} // RA2: as above
			int best = 500000; // RA2: as above
			for (int i = 0; i < region->StartCells->Count(); i++) { // RA2: as above
				Cell c = (*region->StartCells)[i]; // RA2: as above
				double distance = fastmath::sqrt((double)((c.Y - center.Y) * (c.Y - center.Y) + (c.X - center.X) * (c.X - center.X))); // RA2: as above
				if (distance < (double)best) { // RA2: as above
					best = (int)distance; // RA2: as above
					nearest = i; // RA2: as above
				} // RA2: as above
			} // RA2: as above
		} // RA2: as above

		double percent = SeedData.Tiberium * 0.01; // RA2: as above
		int total = (int)(percent * (RMGMaximumTiberium - RMGMinimumTiberium) + RMGMinimumTiberium); // RA2: as above
		double players = region->StartCount; // RA2: as above
		int amount = (int)((double)total * (0.5 > players ? 0.5 : players)); // RA2: as above
		if (region->StartCells->Count() == 0 || amount == 0) continue; // RA2: as above

		int per_field = amount / region->StartCells->Count(); // RA2: as above
		for (int field = 0; field < region->StartCells->Count(); field++) { // RA2: as above
			int size = (int)((double)per_field + Sample_Truncated_Normal_Inline(0.0, 50.0, -100.0, 100.0)); // RA2: as above
			if (size >= 0) { // RA2: as above
				Create_Tiberium_Patch((*region->StartCells)[field], size, field + waypoint + 1, field == nearest); // RA2: as above
			} // RA2: as above
		} // RA2: as above

		/*
		**	Every player starting in this region gets a field of their own, the bigger the
		**	further they sit from the fields already grown.
		*/
		DynamicVectorClass<double> distances; // RA2: as above
		for (int player = 0; player < Start_Count(region); player++) { // RA2: as above
			double total_distance = 0.0; // RA2: as above
			Cell start = Scen->Get_Waypoint_Cell((WAYPOINT)(player + waypoint)); // RA2: as above
			for (int i = 0; i < region->StartCells->Count(); i++) { // RA2: as above
				Cell c = (*region->StartCells)[i]; // RA2: as above
				total_distance += Cell_Distance(c, start.X, start.Y); // RA2: as above
			} // RA2: as above
			distances.Add(total_distance / (double)region->StartCells->Count()); // RA2: as above
		} // RA2: as above

		double closest = 9999999.0; // RA2: as above
		for (int i = 0; i < Start_Count(region); i++) { // RA2: as above
			if (distances[i] < closest) closest = distances[i]; // RA2: as above
		} // RA2: as above

		static const double _distance_scale = 15.0; // RA2: the extra field cells per cell of distance beyond the closest start
		bool rich = Rich_Starts(SeedData); // RA2: as above
		for (int spot = 0; spot < Start_Count(region); spot++) { // RA2: as above
			int size = (int)((distances[spot] - closest) * _distance_scale); // RA2: a named constant of the function: the executable reads this 15.0 at 0x7A5CE8, among the function statics, while the literal 15.0 elsewhere in this file is the copy kept from an earlier object (0x7A5398)
			Create_Tiberium_Patch(Scen->Get_Waypoint_Cell((WAYPOINT)(spot + waypoint)), size + 500, spot + waypoint + 1, rich); // RA2: as above
		} // RA2: as above
		waypoint += Start_Count(region); // RA2: as above
	} // RA2: as above
}


/// <summary>
/// Grows one ore field outward from a cell.
/// This routine spreads ore over open ground, thickening what is already there rather than
/// overwriting it, and will pick a fresh start if the growth stalls early. A field of ore
/// gets an ore tree at its heart; a rich field is grown from gems instead and has none.
/// </summary>
/// <param name="cell">The cell the field grows out from.</param>
/// <param name="count">The number of cells of ore to lay down.</param>
/// <param name="patch_id">Tag marking a cell as claimed by this field, so that the field cannot
/// grow back over itself.</param>
/// <param name="rich">Should the field be grown from gems rather than ore?</param>
void MapGeneratorClass::Create_Tiberium_Patch(Cell const &cell, int count, int patch_id, bool rich) // RA2: four arguments
{
	int marker_index = 0; // RA2: Create_Tiberium_Patch at 0x5840A0, RA2's form
	CellNode *nodes = new CellNode[10 * count];
	PriorityQueueClass<CellNode> *queue = new PriorityQueueClass<CellNode>(10 * count);

	bool queue_was_reset_for_spread = false;
	Cell spread_origin = cell;
	int placed_count = 0; // RA2
	int restart_count = 0;
	int overlay_base = rich ? 27 : 102; // RA2: gems, or ore

	queue->Clear();
	CellNode *node = NULL;

	while (placed_count < count) {
		if (restart_count >= 10) {
			break;
		}

		if (node == NULL) {
			queue->Clear();

			Map.Reset_Iterator();
			CellClass *iter = Map.Iterate();
			while (iter != NULL) {
				Cell c = iter->CellID;
				Set_Cell_Data_Spread(c, 0);
				iter = Map.Iterate();
			}

			marker_index = 1;
			float & score = nodes[0].Score; // RA2: score written after the cell
			nodes[0].Element = cell;
			score = 0; // RA2: as above
			Set_Cell_Data_Spread(cell, patch_id);
			queue->Insert(nodes[0]);
			node = queue->Extract_Min();
			queue_was_reset_for_spread = false;
			restart_count++;
			spread_origin = cell;
		}

		/*
		 * Every use below reads node->Element THROUGH the node pointer, and the loop
		 * re-reads it on every pass. This is load-bearing: the neighbor writes go
		 * to nodes[marker_index], and marker_index is reset to 0 in the block below
		 * while node still points at nodes[0], so the first neighbor stored OVERWRITES
		 * node->Element and the remaining directions are taken from the new cell.
		 * Caching the cell in a local would silently scan the true 8-neighborhood and
		 * generate different fields.
		 */
		CellClass *cellptr = &Map[node->Element];
		if (MapRegionClass::CellData::Is_Not_Inviolate(node->Element)) {
			if (!queue_was_reset_for_spread) {
				queue_was_reset_for_spread = true;
				spread_origin = node->Element;
				queue->Clear();
				marker_index = 0;
				if (!rich) { // RA2: an ore tree where the field starts
					char name[20]; // RA2: as above
					sprintf(name, "TIBTRE0%d", (int)Pick_Random_UInt(1, 3) % 10); // RA2: as above
					new TerrainClass(TerrainTypes[TerrainTypeClass::From_Name(name)], spread_origin); // RA2: as above
				} // RA2: as above
			}

			if (cellptr->Overlay == OVERLAY_NONE) {
				cellptr->Overlay = OverlayType(Pick_Random_UInt(0, 11) + overlay_base); // RA2
				placed_count++;
			} else {
				if (cellptr->OverlayData < OVERLAYDATA_TIBERIUM_MAX) {
					cellptr->OverlayData++;
					placed_count++;
				}
			}
		}

		CellNode *newnode = &nodes[marker_index];
		for (int dir = FACING_FIRST; dir < FACING_COUNT; dir++) {
			Cell newcell = Adjacent_Cell(node->Element, (FacingType)dir);
			if (Map.In_Local_Radar(newcell, true)) {
				CellClass *newcellptr = &Map[newcell];
				if (newcellptr->Is_Tile_Clear()) {
					if ((newcellptr->Overlay == OVERLAY_NONE && Get_Cell_Data_Spread(newcell) != patch_id)
							|| (newcellptr->OverlayData < OVERLAYDATA_TIBERIUM_MAX && newcellptr->Tiberium_Type_Here() != TIBERIUM_NONE)) {
						newnode->Element = newcell;
						newnode->Score = Get_Tiberium_Score(spread_origin, newcell);
						Set_Cell_Data_Spread(newcell, patch_id);
						marker_index++;
						queue->Insert(*newnode++);
					}
				}
			}
		}

		node = queue->Extract_Min();
	}

	delete[] nodes;
	delete queue;
}


/// <summary>
/// Fetches the spread priority of a candidate tiberium cell.
/// Create_Tiberium_Patch grows in cheapest first order, so this score is what keeps a field
/// gathered about its origin while leaving the edge irregular.
/// </summary>
/// <param name="cell1">The cell the field is growing out from.</param>
/// <param name="cell2">The candidate cell being considered.</param>
/// <returns>Returns with the score, where a lower score means the cell is reached sooner.</returns>
double MapGeneratorClass::Get_Tiberium_Score(const Cell & cell1, const Cell & cell2) const
{
	static const double _random_range = 5.0;

	int x = cell1.X - cell2.X;
	int y = cell1.Y - cell2.Y;
	double dist = fastmath::sqrt(x * x + y * y);
	return(RMG_RANDOM_DOUBLE(_random_range) + dist);
}


/// <summary>
/// Works out how high the hills of the map should stand.
/// This routine sweeps the map giving each cell a height drawn from its already settled
/// neighbors, which is what makes the terrain roll rather than jump. The seed's hills setting
/// governs how far the heights are allowed to wander, and a flat enough setting leaves the map
/// alone entirely. Cells anchored by Seed_Hill_Anchors keep the height they were given.
/// </summary>
void MapGeneratorClass::Seed_Hill_Heights(void)
{
	static const double scale = 0.025; // RA2: Seed_Hill_Heights at 0x584730 matches only so

	int i;
	int grid_size = MapCellStride * MapCellStride;
	double height_step = SeedData.Hills * 0.0001 + 0.1; // RA2: as above
	double min_volatility = SeedData.Hills * 0.0025;
	double max_volatility = SeedData.Hills * 0.005;

	if (min_volatility >= scale) {
		for (i = 0; i < grid_size; i++) {
			MapRegionClass::CellData * pdata = &MapRegionClass::Get_Cell_Data(i); // RA2: as above
			MapRegionClass::CellData & data = *pdata; // RA2: as above
			Cell cell = data.CellID;
			if (cell.X != 0 || cell.Y != 0) { // RA2: as above
				data.HillVolatility = 0;
				double up_height = 0.0;
				double height_trend = 0.0;

				if (My_In_Radar(cell - Cell(1, 1))) {
					up_height = MapRegionClass::Get_Cell_Data(cell - Cell(1, 1)).HillHeight;
				}

				Cell newcell = cell - Cell(1, 0);
				if (My_In_Radar(newcell)) {
					MapRegionClass::CellData & data2 = MapRegionClass::Get_Cell_Data(newcell);
					pdata->HillHeight += data2.HillHeight; // RA2: as above
					data.HillVolatility = data2.HillVolatility;
					height_trend += data2.HillHeight - up_height;
				} else {
					pdata->HillHeight = data.HillHeight; // RA2: as above
					data.HillVolatility = min_volatility;
				}

				newcell = cell - Cell(0, 1);
				if (My_In_Radar(newcell)) {
					MapRegionClass::CellData & data2 = MapRegionClass::Get_Cell_Data(newcell);
					double height2 = data2.HillHeight; // RA2: as above
					data.HillHeight = height2 + data.HillHeight; // RA2: as above
					data.HillVolatility = data2.HillVolatility + data.HillVolatility; // RA2: as above
					height_trend += data2.HillHeight - up_height;
				} else {
					pdata->HillHeight = data.HillHeight; // RA2: as above
					data.HillVolatility = min_volatility + data.HillVolatility; // RA2: as above
				}

				data.HillHeight *= 0.5;
				data.HillVolatility *= 0.5;
				if (height_trend > 0.0) {
					height_trend = height_step;
				} else if (height_trend < 0.0) {
					height_trend = -height_step;
				}
				if (data.Inviolate) { // RA2: as above
					data.HillHeight = (data.HillHeight <= 0.0) ? 0.0 : data.HillHeight; // RA2: as above
					data.HillVolatility = 0.0025; // RA2: as above
				}
				const double mean = 0.0;
				data.HillVolatility += Sample_Truncated_Normal_Inline(mean, scale, -data.HillVolatility, max_volatility - data.HillVolatility); // RA2: as above

				double min_height = -2.0 - data.HillHeight; // RA2: as above
				double max_height = 2.0 - data.HillHeight; // RA2: as above
				double trend; // RA2: as above
				if (height_trend < min_height) { // RA2: as above
					trend = min_height; // RA2: as above
				} else if (height_trend > max_height) { // RA2: as above
					trend = max_height; // RA2: as above
				} else {
					trend = height_trend; // RA2: as above
				}
				data.HillHeight += Sample_Truncated_Normal_Inline(trend, data.HillVolatility, min_height, max_height); // RA2: as above
			}
		}

		for (i = 0; i < grid_size; i++) {
			MapRegionClass::CellData & data = MapRegionClass::Get_Cell_Data(i);
			if (data.HillHeight < 0) {
				data.HillHeight = (int)(data.HillHeight - 0.5);
			} else if (data.HillHeight > 0) { // RA2: as above
				data.HillHeight = (int)(data.HillHeight + 0.5);
			}
		}
	}
}


/// <summary>
/// Pins down the hill heights that must not move.
/// This routine runs ahead of Seed_Hill_Heights and protects the ground beside shorelines and
/// cliffs, so that the random walk which follows cannot raise a hill into the sea or bury an
/// existing cliff face.
/// </summary>
void MapGeneratorClass::Seed_Hill_Anchors(void)
{
	Map.Reset_Iterator();

	CellClass * cptr = Map.Iterate();
	while (cptr != NULL) {
		if (cptr->Is_Tile_Shore()) {
			for (int dir = FACING_FIRST; dir < FACING_COUNT; dir++) {
				Cell cell = Adjacent_Cell(cptr->Fetch_CellID(), (FacingType)dir);
				if (My_In_Radar(cell) && Map[cell].Is_Tile_Clear()) {
					MapRegionClass::Get_Cell_Data(cell).HillHeight = 0.5;
					MapRegionClass::Get_Cell_Data(cell).Inviolate = true;
					break;
				}
			}
		} else if (cptr->Is_Tile_Cliff()) {
			for (int dir = FACING_FIRST; dir < FACING_COUNT; dir++) {
				Cell cell = Adjacent_Cell(cptr->Fetch_CellID(), (FacingType)dir);
				if (My_In_Radar(cell) && Map[cell].Is_Tile_Clear()) {
					MapRegionClass::Get_Cell_Data(cell).Inviolate = true;
					break;
				}
			}
		}
		cptr = Map.Iterate();
	}
}


/// <summary>
/// Raises the hills of the map.
/// This routine takes the per cell hill heights worked out by the seeding passes and lifts the
/// terrain to meet them one step at a time, letting the smoothing system cut the slopes and
/// ramps in as it goes. Ramp groups that turn out to be redundant are flattened away again.
/// </summary>
void MapGeneratorClass::Generate_Hills(void)
{
	Seed_Hill_Anchors();
	Seed_Hill_Heights();

	Init_Deform_Grid_RMG();

	int grid_size = MapCellStride * MapCellStride;
	for (int i = 0; i < grid_size; i++) {
		MapRegionClass::CellData & data = MapRegionClass::Get_Cell_Data(i);
		if (data.CellID != Cell(0, 0)) {
			CellClass * cellptr = &Map[data.CellID];
			int change = cellptr->Height + data.HillHeight - Get_Deformed_Cell_Height(cellptr);
			int steps = abs(change);
			int direction = change < 0 ? -1 : 1;
			while (steps > 0) {
				Deform_Cell_RMG(data.CellID, direction, 0, Get_Deformable_Cell_Corners(cellptr, direction));
				steps--;
			}
		}
	}

	Commit_Deform_Grid_RMG();

	Map.Reset_Iterator();
	CellClass * cptr = Map.Iterate();

	Cell offsets[] = { Cell(0, 0), Cell(1, 0), Cell(1, 1), Cell(0, 1)};
	RampType ramps[] = { RAMP_MID_SE, RAMP_MID_SW, RAMP_MID_NW, RAMP_MID_NE };

	while (cptr != NULL) {
		int height_step = 0;
		bool raise = true;
		if (cptr->Ramp == RAMP_CORNER_NW) {
			for (int i = 0; i < ARRAY_SIZE(offsets); i++) {
				if (Map[cptr->Fetch_CellID() + offsets[i]].Ramp != RAMP_CORNER_NW + i) {
					raise = false;
				}
			}
		} else if (cptr->Ramp == RAMP_MID_SE) {
			height_step += 1;
			for (int i = 0; i < ARRAY_SIZE(offsets); i++) {
				if (Map[cptr->Fetch_CellID() + offsets[i]].Ramp != ramps[i]) {
					raise = false;
				}
			}
		} else {
			raise = false;
		}

		if (raise) {
			for (int i = 0; i < ARRAY_SIZE(offsets); i++) {
				CellClass * cellptr = &Map[cptr->Fetch_CellID() + offsets[i]];
				cellptr->ITType = ISOTILE_NONE;
				cellptr->SubTile = 0;
				cellptr->Ramp = RAMP_NONE;
				cellptr->Height += height_step;
			}
		}

		cptr = Map.Iterate();
	}
}


/// <summary>
/// Seeds the ground cover chances of the map.
/// This routine decides, cell by cell, how likely each kind of ground cover is before
/// Generate_Vegetation does the actual planting. The seed's vegetation setting scales the lot,
/// and the ground along a shoreline is made far greener than the interior.
/// </summary>
void MapGeneratorClass::Seed_Vegetation(void)
{
	Map.Reset_Iterator();
	CellClass * cptr = Map.Iterate();

	static const double rough = 0.005; // RA2: Seed_Vegetation at 0x5850A0 matches only so
	static const double green = 0.02; // RA2: as above
	static const double sand = 0.005; // RA2: as above
	double density = SeedData.Vegetation * 0.01;

	while (cptr != NULL) { // RA2: Seed_Vegetation at 0x5850A0 has no double chances[3][4] = {
		MapRegionClass::CellData & data = MapRegionClass::Get_Cell_Data(cptr->Fetch_CellID());
		if (cptr->Is_Tile_Shore()) {
			for (int x = cptr->Fetch_CellID().X - 2; x < cptr->Fetch_CellID().X + 3; x++) {
				for (int y = cptr->Fetch_CellID().Y - 2; y < cptr->Fetch_CellID().Y + 3; y++) {
					if (My_In_Radar(Cell(x, y))) {
						data.RoughTileChance = rough; // RA2: as above
						data.GreenTileChance = density * green * 10.0; // RA2: as above
						data.SandTileChance = 0;
					} // RA2: Seed_Vegetation at 0x5850A0 has no ForestChance
				}
			}
		} else if (data.RoughTileChance == 0.0) {
			data.RoughTileChance = rough; // RA2: as above
			data.GreenTileChance = density * green; // RA2: as above
			data.SandTileChance = sand; // RA2: as above
		}
		cptr = Map.Iterate();
	}
}


/// <summary>
/// Seeds the ground cover chances of an arctic map.
/// This is the tundra and taiga counterpart of Seed_Vegetation, and like it, it runs ahead of
/// Generate_Arctic_Vegetation, which does the actual planting.
/// </summary>
void MapGeneratorClass::Seed_Arctic_Vegetation(void)
{
	static const double rough = 0.005; // RA2: Seed_Arctic_Vegetation at 0x585240 matches only so
	static const double sand = 0.001; // RA2: as above

	Map.Reset_Iterator();
	CellClass * cptr = Map.Iterate();

	while (cptr != NULL) { // RA2: Seed_Arctic_Vegetation at 0x585240 has no SeedData, Vegetation
		MapRegionClass::CellData & data = MapRegionClass::Get_Cell_Data(cptr->Fetch_CellID());
		data.RoughTileChance = rough; // RA2: as above
		data.SandTileChance = sand; // RA2: as above
		cptr = Map.Iterate();
	}
}


/// <summary>
/// Dresses a temperate, desert or mutated map with ground cover.
/// This routine walks the map planting green, rough and sand patches and the occasional wood
/// against the chances that Seed_Vegetation left on each cell, then litters the result with
/// rocks. A mutated map is given its mold and crystal growths first.
/// </summary>
void MapGeneratorClass::Generate_Vegetation(void)
{
	CellClass * cptr;

	Clear_Cell_Data_Spreads(); // RA2: Generate_Vegetation at 0x5852C0 has no SeedData, Biome, Width and 15 more

	Map.Reset_Iterator();
	cptr = Map.Iterate();

	int rough_mean = Pick_Random_UInt(20, 40); // RA2: Generate_Vegetation at 0x5852C0 matches only so
	int sand_mean = Pick_Random_UInt(20, 40); // RA2: as above
	int green_mean = Pick_Random_UInt(20, 40); // RA2: as above

	while (cptr != NULL) {
		MapRegionClass::CellData & data = MapRegionClass::Get_Cell_Data(cptr->Fetch_CellID());
		if (cptr->Is_Tile_Clear() && cptr->Ramp == RAMP_NONE && cptr->Overlay == OVERLAY_NONE && cptr->Cell_Occupier() == NULL && !data.Inviolate) {
			Cell cell = cptr->Fetch_CellID();
			int origin_id = cell.X + cell.Y * MAP_CELL_W;
			if (Random_Fraction() < data.GreenTileChance) {
				Place_Tile_Patch(cptr, IsometricTileTypeClass::RoughTile, Sample_Truncated_Normal_Inline(rough_mean, 20, 4, 80), origin_id, false); // RA2: as above
			} else {
				if (Random_Fraction() < data.RoughTileChance) {
					Place_Tile_Patch(cptr, IsometricTileTypeClass::SandTile, Sample_Truncated_Normal_Inline(sand_mean, 20, 4, 80), origin_id, false); // RA2: as above
				} else {
					if (Random_Fraction() < data.SandTileChance) {
						Place_Tile_Patch(cptr, IsometricTileTypeClass::GreenTile, Sample_Truncated_Normal_Inline(green_mean, 20, 4, 80), origin_id, false); // RA2: as above
					}
				}
			}
		}
		cptr = Map.Iterate();
	}

	Map.Reset_Iterator();
	cptr = Map.Iterate();
	while (cptr != NULL) {
		cptr->Fixup_LAT();
		cptr = Map.Iterate();
	}

	int tree_count = (int)((SeedData.Width * 0.1 + 0.7) * (SeedData.Vegetation * 0.01) * MaxTrees); // RA2: as above
	for (int tries = 0; tree_count > 0 && tries < 100; tries++) { // RA2: as above
		Cell cell = MapRegionClass::Pick_Random_Clear_Map_Cell(); // RA2: as above
		tree_count -= Place_Forest(&Map[cell], Sample_Truncated_Normal_Inline(25, 10, 10, 35), Sample_Truncated_Normal_Inline(0.2, 0.1, 0.05, 0.4)); // RA2: as above
	}

	int overlay_count = Pick_Random_UInt(0, Map_Cell_Count() / 200); // RA2: as above

	int placed = 0;
	for (tries = 0; tries < overlay_count * 5 && placed < overlay_count; tries++) { // RA2: as above
		Cell cell = MapRegionClass::Pick_Random_Map_Cell(); // RA2: as above
		CellClass * cellptr = &Map[cell]; // RA2: as above
		MapRegionClass::CellData & data = MapRegionClass::Get_Cell_Data(cell); // RA2: as above
		if (cellptr->Overlay == OVERLAY_NONE) {
			if (cellptr->Is_Tile_Clear_To_Sand_LAT()) {
				cellptr->Overlay = OverlayType(Pick_Random_UInt(0, 4) + OVERLAY_SAND_ROCK_01);
			} else {
				if (cellptr->Is_Tile_Clear() || cellptr->Is_Tile_Clear_To_Green_LAT()) {
					cellptr->Overlay = OverlayType(Pick_Random_UInt(0, 4) + OVERLAY_CLEAR_ROCK_01);
				}
			}
			cellptr->OverlayData = 0;
			cellptr->Recalc_Attributes();
			placed++;
		}
	}
}


/// <summary>
/// Dresses an arctic map with ground cover.
/// This is the tundra and taiga counterpart of Generate_Vegetation. It scatters rough ground,
/// then walks the map planting rough patches, woods and rock fields against the chances that
/// Seed_Arctic_Vegetation left on each cell.
/// </summary>
void MapGeneratorClass::Generate_Arctic_Vegetation(void)
{
	CellClass * cptr;

	Clear_Cell_Data_Spreads(); // RA2: Generate_Arctic_Vegetation at 0x585A60 has no IsometricTileType, IsometricTileTypeClass, RoughGround and 2 more

	Map.Reset_Iterator();
	cptr = Map.Iterate();

	while (cptr != NULL) { // RA2: Generate_Arctic_Vegetation at 0x585A60 has no SeedData, Biome
		MapRegionClass::CellData & data = MapRegionClass::Get_Cell_Data(cptr->Fetch_CellID());
		if (cptr->Is_Tile_Clear() && cptr->Ramp == RAMP_NONE && !data.Inviolate) {
			Cell cell = cptr->Fetch_CellID(); // RA2: Generate_Arctic_Vegetation at 0x585A60 matches only so
			int origin_id = cell.X + cell.Y * MAP_CELL_W;
			if ((unsigned int)RMGRandom() * (1.0 / 4294967295.0) < data.RoughTileChance) { // RA2: as above
				double patch_size = Sample_Truncated_Normal_Inline(20, 15, 4, 60); // RA2: as above
				Place_Tile_Patch(cptr, IsometricTileTypeClass::RoughTile, patch_size, origin_id, false); // RA2: as above
			}
		}
		cptr = Map.Iterate();
	}

	Map.Reset_Iterator();
	cptr = Map.Iterate();
	while (cptr != NULL) {
		cptr->Fixup_LAT();
		cptr = Map.Iterate();
	}

	int tree_count = (int)((SeedData.Width * 0.1 + 0.7) * (SeedData.Vegetation * 0.01) * MaxTrees); // RA2: as above
	for (int tries = 0; tree_count > 0 && tries < 100; tries++) { // RA2: as above
		Cell cell = MapRegionClass::Pick_Random_Clear_Map_Cell(); // RA2: as above
		tree_count -= Place_Forest(&Map[cell], Sample_Truncated_Normal_Inline(25, 10, 10, 35), Sample_Truncated_Normal_Inline(0.2, 0.1, 0.05, 0.4)); // RA2: as above
	}
}


/// <summary>
/// RA2
/// </summary>
static inline void Delete_Forest_Search(CellNode * nodes, PriorityQueueClass<CellNode> * queue)
{
	delete[] nodes;
	delete queue;
}


/// <summary>
/// Plants a wood outward from a starting cell.
/// This routine spreads over the ground around the seed cell and plants a tree on only some of
/// what it reaches, which is what gives a wood a dense heart and a thinning edge. Ground it has
/// walked over is remembered, so that woods do not pile up on one another.
/// </summary>
/// <param name="cellptr">The cell the wood grows out from.</param>
/// <param name="count">The number of cells the wood may spread over.</param>
/// <param name="density">The chance, from 0 to 1, that a cell reached is given a tree.</param>
int MapGeneratorClass::Place_Forest(CellClass const * cellptr, int count, double density) // RA2: Place_Forest at 0x585DC0 matches only so
{
	CellNode * nodes = new CellNode[25 * count]; // RA2: as above
	PriorityQueueClass<CellNode> * queue = new PriorityQueueClass<CellNode>(25 * count); // RA2: as above

	queue->Clear();

	int node_count = 0; // RA2: as above
	Cell cell = cellptr->Fetch_CellID();
	nodes[node_count].Element = cell; // RA2: as above
	nodes[node_count].Score = 0; // RA2: as above
	node_count++; // RA2: as above

	MapRegionClass::Get_Cell_Data(cellptr->Fetch_CellID()).Forested = true;

	queue->Insert(*nodes);

	CellNode * node = queue->Extract_Min();

	int i = 0; // RA2: as above
	while (i < count && node != NULL && node_count < 25 * count) { // RA2: as above
		CellClass * cptr = &Map[node->Element];
		if (cptr->Is_Tile_Clear() && cptr->Cell_Occupier() == NULL && cptr->Overlay == OVERLAY_NONE && cptr->Land_Type() != LAND_ROCK) {
			if (Random_Fraction() < density) {
				char tree_name[20];
				int tree_index = Pick_Random_UInt(1, 25); // RA2: as above
				sprintf(tree_name, "TREE%d%d", tree_index / 10, tree_index % 10);
				new TerrainClass(TerrainTypes[TerrainTypeClass::From_Name(tree_name)], cptr->Fetch_CellID());
				i++; // RA2: as above
			}
		}

		FacingType dir = FACING_FIRST; // RA2: as above
		CellNode * newnode = &nodes[node_count];
		for (; dir < FACING_COUNT; dir++) { // RA2: as above
			Cell const & offset = AdjacentCell[(int)dir & 7]; // RA2: as above
			Cell newcell(offset.X + node->Element.X, offset.Y + node->Element.Y); // RA2: as above
			if (My_In_Radar(newcell)) {
				MapRegionClass::CellData & data = MapRegionClass::Get_Cell_Data(newcell);
				if (!data.Forested && !data.Inviolate && node_count < 25 * count) { // RA2: as above
					newnode->Element = newcell;
					newnode->Score = Get_Forest_Score(cellptr->Fetch_CellID(), newcell);
					data.Forested = true;
					node_count++;
					queue->Insert(*newnode++);
				}
			}
		}

		node = queue->Extract_Min(); // RA2: Place_Forest at 0x585DC0 has no i++;
	}

	Delete_Forest_Search(nodes, queue); // RA2: as above
	return i; // RA2: as above
}


/// <summary>
/// Grows a patch of one isometric tile type across the ground.
/// This is how the vegetation pass lays down its blotches of green, rough and sand ground. The
/// patch spreads outward from the starting cell over open ground alone, and stops once it has
/// claimed its quota of cells.
/// </summary>
/// <param name="cellptr">The cell the patch grows out from.</param>
/// <param name="ittype">The isometric tile type to lay down.</param>
/// <param name="count">The number of cells the patch may claim.</param>
/// <param name="origin_id">Tag marking a cell as claimed by this patch, so that the patch cannot
/// grow back over itself.</param>
/// <param name="on_pavement">May the patch spread over pavement as well as open ground?</param>
void MapGeneratorClass::Place_Tile_Patch(CellClass * cellptr, IsometricTileType ittype, int count, int origin_id, bool on_pavement)
{
	CellNode * node = NULL; // RA2: Place_Tile_Patch at 0x586340 matches only so
	CellNode * nodes = new CellNode[10 * count];
	PriorityQueueClass<CellNode> * queue = new PriorityQueueClass<CellNode>(10 * count);
	int i = 0;
	queue->Clear();

	int node_count = 0; // RA2: as above
	Cell cell = cellptr->Fetch_CellID();
	nodes[node_count].Element = cell; // RA2: as above
	nodes[node_count].Score = 0; // RA2: as above
	node_count++; // RA2: as above

	MapRegionClass::Get_Cell_Data(cellptr->Fetch_CellID()).SpreadID = origin_id;

	queue->Insert(*nodes);

	node = queue->Extract_Min(); // RA2: as above
	while (i < count && node != NULL) {
		CellClass * cptr = &Map[node->Element];
		cptr->ITType = ittype;

		FacingType dir = FACING_FIRST; // RA2: as above
		CellNode * newnode = &nodes[node_count];
		for (; dir < FACING_COUNT; dir++) { // RA2: as above
			Cell newcell = Adjacent_Cell(node->Element, dir);
			if (My_In_Radar(newcell)) {
				CellClass * newcellptr = &Map[newcell];
				if (newcellptr->Is_Tile_Clear() || on_pavement && (newcellptr->Is_Tile_Pavement() || newcellptr->Is_Tile_Misc_Pavement())) {
					if (MapRegionClass::Get_Cell_Data(newcell).SpreadID != origin_id && newcellptr->Ramp == RAMP_NONE && newcellptr->Overlay == OVERLAY_NONE && newcellptr->Cell_Occupier() == NULL) {
						newnode->Element = newcell;
						newnode->Score = Get_Tile_Patch_Score(cellptr->Fetch_CellID(), newcell);
						MapRegionClass::Get_Cell_Data(newcell).SpreadID = origin_id;
						node_count++;
						queue->Insert(*newnode++);
					}
				}
			}
		}

		i++;
		node = queue->Extract_Min();
	}

	delete[] nodes;
	delete queue;
}



/// <summary>
/// Scatters settlements across the map.
/// This routine is the civilization pass of the generator. It works out how much settlement
/// the seed asked for, then either grows a full urban area with roads, buildings and traffic,
/// or drops a smaller rural cluster instead. It gives up rather than hunt forever for
/// somewhere to build.
/// </summary>
void MapGeneratorClass::Generate_Urban_Areas(void)
{
	int count = (SeedData.Cities + 0.32) * 3;
	int tries = 0;
	int placed = 0;

	Init_Dirt_Roads();

	while (true) {
		if (placed >= count) {
			break;
		}
		Cell cell(0, 0);
		int tries_here = 0;
		while (true) {
			cell = MapRegionClass::Pick_Random_Clear_Map_Cell();
			if (++tries_here >= 20) break;
			if (MapRegionClass::CellData::Is_Not_Inviolate(cell) || Is_Area_Cliff_Free(cell)) {
				break;
			}
		}

		if (Random_Fraction() < (1.0 - SeedData.Cities)) {
			if (Generate_Rural_Roads(cell)) {
				Generate_Rural_Buildings(cell);
				Generate_Rural_Units(cell);
				placed++;
			}
		} else {
			if (tries_here != 20) {
				int size = Sample_Truncated_Normal_Inline(2 * Map.PlayRect.Width * Map.PlayRect.Height / 30, 300, 100, 2000); // RA2: Generate_Urban_Areas at 0x586800 matches only so
				DynamicVectorClass<Cell> * cells = Create_Urban_Area(cell, size);
				if (cells != NULL) {
					Generate_Urban_Roads(*cells);
					Generate_Urban_Buildings(*cells);
					Generate_Urban_Units(*cells);
					Generate_Urban_Pavement(*cells);
					placed++;
					delete cells;
				}
			}
			tries++;
		}

		if (tries >= 10) {
			break;
		}
	}
}


/// <summary>
/// Paves an urban area by flood-filling pavement tiles outward from a seed cell
/// using a priority queue, then trims the border cells and scatters miscellaneous
/// pavement tiles across the result.
/// </summary>
/// <param name="cell">The seed cell to grow the urban area from.</param>
/// <param name="size">The maximum number of cells to process during the flood fill.</param>
/// <returns>The collected urban area cells, or NULL if the area is too small or sparse.</returns>
DynamicVectorClass<Cell> * MapGeneratorClass::Create_Urban_Area(Cell const & cell, int size)
{
	DynamicVectorClass<Cell> * cells; // RA2: Create_Urban_Area at 0x586B40 matches only so
	CellNode * nodes = new CellNode[10 * size];
	PriorityQueueClass<CellNode> * queue = new PriorityQueueClass<CellNode>(10 * size);
	int processed = 0; // RA2: as above
	queue->Clear();

	Map.Reset_Iterator(); // RA2: Create_Urban_Area at 0x586B40 has no int processed = 0;
	CellClass * cptr = Map.Iterate();
	while (cptr != NULL) {
		Cell c = cptr->CellID;
		Set_Cell_Data_Spread(c, 0);
		cptr = Map.Iterate();
	}

	int node_count = 0; // RA2: as above
	nodes[node_count].Element = cell; // RA2: as above
	nodes[node_count].Score = 0; // RA2: as above
	Set_Cell_Data_Spread(cell, 1);
	node_count++; // RA2: as above
	queue->Insert(nodes[0]);

	CellNode * node = queue->Extract_Min();

	cells = new DynamicVectorClass<Cell>; // RA2: as above
	cells->Set_Growth_Step(size + 2);

	while (processed < size && node != NULL) { // RA2: as above
		Map[node->Element].ITType = IsometricTileTypeClass::PaveTile;
		cells->Add(node->Element);

		CellNode * newnode = &nodes[node_count];
		for (FacingType dir = FACING_FIRST; dir < FACING_COUNT; dir++) {
			Cell newcell = Adjacent_Cell(node->Element, dir);
			if (My_In_Radar(newcell)) {
				CellClass * newcellptr = &Map[newcell];
				if (newcellptr->Is_Tile_Clear() && newcellptr->Overlay == OVERLAY_NONE) {
					if (Get_Cell_Data_Spread(newcell) != 1 && MapRegionClass::CellData::Is_Not_Inviolate(newcell)) { // RA2: as above
						newnode->Element = newcell;
						newnode->Score = Get_Urban_Score(cell, newcell);
						Set_Cell_Data_Spread(newcell, 1);
						node_count++;
						queue->Insert(*newnode++);
					}
				}
			}
		}

		processed++;
		node = queue->Extract_Min();
	}

	delete[] nodes;
	delete queue;

	Rect bounds = Get_Cell_Bounding_Rect(*cells);

	if (processed <= 100 || (double)(bounds.Width * bounds.Height) >= (double)cells->Count() * 1.6) {
		for (int j = cells->Count() - 1; j >= 0; j--) {
			Map[(*cells)[j]].ITType = ISOTILE_NONE;
		}
		delete cells;
		return(NULL);
	}

	for (int cell_index = cells->Count() - 1; cell_index >= 0; cell_index--) {
		Cell c = (*cells)[cell_index];
		for (FacingType dir = FACING_FIRST; dir < FACING_COUNT; dir++) {
			Cell newcell = Adjacent_Cell(c, dir);
			if (My_In_Radar(newcell) && Map[newcell].ITType != IsometricTileTypeClass::PaveTile) {
				cells->Delete_Index(cell_index);
			}
		}
	}

	for (int k = 0; k < cells->Count(); k++) {
		MapRegionClass::CellData::Set_Inviolate((*cells)[k]);
	}

	int patch_count = cells->Count() / 3;
	for (int i = 0; i < patch_count; i++) {
		Cell base = (*cells)[Pick_Random_UInt(0, cells->Count() - 1)];
		int tile_variant = Pick_Random_UInt(0, 6);

		bool place = true;
		if (tile_variant > 3) {
			for (int m = 0; m < 2; m++) {
				for (int n = 0; n < 2; n++) {
					Cell c = base + Cell(m, n); // RA2: as above
					if (!MapRegionClass::Get_Cell_Data(c).Inviolate || Map[c].ITType != IsometricTileTypeClass::PaveTile) {
						place = false;
					}
				}
			}
		}

		if (place) {
			Place_Tile((IsometricTileType)(IsometricTileTypeClass::MiscPaveTile + tile_variant), base, -1, -1);
		}
	}

	return(cells);
}


/// <summary>
/// Fetches the spread priority of a candidate urban cell.
/// Create_Urban_Area paves in cheapest first order, so this score is what gives a town its
/// roughly square plan and its ragged outskirts.
/// </summary>
/// <param name="cell1">The cell the area is growing out from.</param>
/// <param name="cell2">The candidate cell being considered.</param>
/// <returns>Returns with the score, where a lower score means the cell is paved sooner.</returns>
double MapGeneratorClass::Get_Urban_Score(const Cell & cell1, const Cell & cell2) const
{
	static const double _random_range = 3.0;

	int x = cell1.X - cell2.X;
	int y = cell1.Y - cell2.Y;
	double dist = MAX(abs(x), abs(y));
	return(RMG_RANDOM_DOUBLE(_random_range) + dist);
}


/// <summary>
/// Lays the road grid of an urban area.
/// This routine rules roads east to west and then north to south across the town, complete
/// with junctions, medians and end caps. It runs before the buildings go in, since
/// Generate_Urban_Buildings will only build on the pavement this leaves behind.
/// </summary>
/// <param name="cells">The cells of the urban area the road grid is fitted to.</param>
void MapGeneratorClass::Generate_Urban_Roads(const DynamicVectorClass<Cell> & cells)
{
	Cell start; // RA2: RA2's form
	Rect bounds = Get_Cell_Bounding_Rect(cells);

	/// Horizontal east-west road bands
	if (bounds.Height > 5) {
		start.Y = bounds.Y + 2;
		start.X = bounds.X;
		int remaining = bounds.Height - 2;

		while (remaining > 3) {

			Cell junction = Plan_Paved_Road_Junction(start, bounds, FACING_E);
			Cell end = Plan_Paved_Road(junction, bounds, FACING_E);

			bool valid = false; // RA2: as above
			int first; // RA2: as above

			if (end != Cell(0, 0) && start != Cell(0, 0)) { // RA2: as above
				first = start.X; // RA2: as above
				valid = true; // RA2: as above
			}

			if (valid && end.X - first >= 6) { // RA2: as above

				/// Left road cap / start piece
				if (Can_Place_Paved_Road(Rect(junction.X - 4, junction.Y, 4, 3), false, false) && Pick_Random_UInt(0, 1)) {
					Place_Tile((IsometricTileType)(IsometricTileTypeClass::PavedRoads + 9), junction - Cell(3, 0));
				} else {
					Place_Tile((IsometricTileType)(IsometricTileTypeClass::PavedRoadEnds + 2), junction);
				}

				/// Fill straight horizontal road
				for (int x = 0; x < end.X - junction.X - 1; x++) {
					Place_Tile(IsometricTileTypeClass::PavedRoads, junction + Cell(x + 1, 0));
				}

				/// Right road cap / end piece
				if (Can_Place_Paved_Road(Rect(end.X, end.Y, 4, 3), false, false) && Pick_Random_UInt(0, 1)) {
					Place_Tile((IsometricTileType)(IsometricTileTypeClass::PavedRoads + 10), end);
				} else {
					Place_Tile(IsometricTileTypeClass::PavedRoadEnds, end);
				}

				remaining -= 10;
				start.Y += 10;
			} else {
				start.Y += 2;
				remaining -= 2;
			}
		}
	}

	/// Vertical north-south road bands
	if (bounds.Width > 8) {
		int remaining = bounds.Height - 5;
		start.X = bounds.X + 5; // RA2: as above
		start.Y = bounds.Y; // RA2: as above

		while (remaining > 3) {
			Cell cursor = Plan_Paved_Road_Junction(start, bounds, FACING_S);
			Cell end = Plan_Paved_Road(cursor, bounds, FACING_S);

			if (end != Cell(0, 0) && start != Cell(0, 0) && end.Y - start.Y >= 6) { // RA2: as above
				bool in_segment = false;
				bool initial = true;
				bool median_pending = false;
				bool median_flip = false;
				bool skip_tail = false;

				while (cursor.Y < end.Y) {
					if (!in_segment) {
						Rect road_rect;
						Rect junction_rect;
						road_rect = Rect(cursor.X, cursor.Y, 3, 2); // RA2: as above
						junction_rect = Rect(cursor.X, cursor.Y - 3, 3, 4); // RA2: as above

						if (end.Y - cursor.Y < 2) { // RA2: as above
							skip_tail = true;
							break;
						}

						if (initial && Can_Place_Paved_Road(Rect(cursor.X, cursor.Y - 3, 3, 4), false, false) && Pick_Random_UInt(0, 1)) {
							Place_Tile((IsometricTileType)(IsometricTileTypeClass::PavedRoads + 13), cursor - Cell(0, 3));
							in_segment = true;
						} else if (Can_Place_Paved_Road_Junction(junction_rect)) {
							Place_Tile((IsometricTileType)(IsometricTileTypeClass::PavedRoads + 5), cursor - Cell(0, 3));
							Place_Tile((IsometricTileType)(IsometricTileTypeClass::PavedRoads + 1), cursor);
							in_segment = true;
						} else if (Can_Place_Paved_Road(road_rect, false, false)) {
							Place_Tile((IsometricTileType)(IsometricTileTypeClass::PavedRoadEnds + 1), cursor);
							in_segment = true;
						}
					} else {

						/// Straight vertical road segment
						if (Can_Place_Paved_Road(Rect(cursor.X, cursor.Y, 3, 1), false, false)) {
							Place_Tile((IsometricTileType)(IsometricTileTypeClass::PavedRoads + 1), cursor);

							if (median_pending) {
								if (median_flip) {
									Place_Tile((IsometricTileType)(IsometricTileTypeClass::Medians + 5), cursor + Cell(1, 0));
									median_flip = false;
								} else {
									Place_Tile((IsometricTileType)(IsometricTileTypeClass::Medians + 4), cursor + Cell(1, 0));
								}
							}
						} else {
							Rect junction_rect(cursor.X, cursor.Y, 3, 4);

							if (median_pending) {
								Place_Tile((IsometricTileType)(IsometricTileTypeClass::Medians + 3), cursor + Cell(1, -1));
								median_pending = false;
							}

							if (Can_Place_Paved_Road_Junction(junction_rect)) {
								int bottom = end.Y;

								if (bottom - cursor.Y < 4) { // RA2: as above
									Place_Tile((IsometricTileType)(IsometricTileTypeClass::PavedRoads + 7), cursor);
									skip_tail = true;
									break;
								}

								if (Can_Place_Paved_Road(Rect(cursor.X, cursor.Y + 3, 3, 2), false, false)) {
									if (bottom - cursor.Y > 5) { // RA2: as above
										median_pending = true;
										median_flip = true;
									}
								}

								Place_Tile((IsometricTileType)(IsometricTileTypeClass::PavedRoads + 3), cursor);

								if (Can_Place_Paved_Road(Rect(cursor.X - 1, cursor.Y, 1, 3), true, false)) {
									Place_Tile((IsometricTileType)(IsometricTileTypeClass::Medians + 12), cursor - Cell(1, 0));
								}

								if (Can_Place_Paved_Road(Rect(cursor.X + 3, cursor.Y, 1, 3), true, false)) {
									Place_Tile((IsometricTileType)(IsometricTileTypeClass::Medians + 12), cursor + Cell(3, 0));
								}

								cursor.Y += 2;
							} else {
								Place_Tile((IsometricTileType)(IsometricTileTypeClass::PavedRoadEnds + 3), cursor - Cell(0, 1));

								CellClass * cellptr = &Map[cursor + Cell(1, -2)];

								if (cellptr->Is_Tile_Road_Median()) {
									while (cellptr->Is_Tile_Road_Median()) {
										cellptr->ITType = (IsometricTileType)(IsometricTileTypeClass::PavedRoads + 1);
										cellptr->SubTile = 1;
										cellptr = &cellptr->Adjacent_Cell(FACING_N);
									}
								}

								in_segment = false;
							}
						}
					}

					initial = false;
					cursor.Y++;
				}

				if (!skip_tail) {
					if (median_pending) {
						Place_Tile((IsometricTileType)(IsometricTileTypeClass::Medians + 3), cursor + Cell(1, -1));
					}

					/// Final vertical end cap
					if (Can_Place_Paved_Road(Rect(cursor.X, cursor.Y, 3, 4), false, false) && Pick_Random_UInt(0, 1)) {
						Place_Tile((IsometricTileType)(IsometricTileTypeClass::PavedRoads + 12), cursor);
					} else {
						Place_Tile((IsometricTileType)(IsometricTileTypeClass::PavedRoadEnds + 3), cursor);
					}
				}

				remaining -= 10;
				start.X += 10;
			} else {
				start.X += 2;
				remaining -= 2;
			}
		}
	}
}



/// <summary>
/// Places civilian buildings across an urban area.
/// This routine fills the pavement laid down by Generate_Urban_Roads with city blocks, giving
/// each building the worn look the biome calls for. Landmarks that should appear only once are
/// dropped from the pool as soon as they have been placed.
/// </summary>
/// <param name="cells">The cells of the urban area available to build on.</param>
void MapGeneratorClass::Generate_Urban_Buildings(const DynamicVectorClass<Cell> & cells)
{
	static char * building_names[18] = {"CITY01", "CITY02", "CITY03", "CITY04", "CITY05", "CITY06", "CITY07", "CITY08", "CITY09", "CITY10", "CITY11", "CITY12", "CITY13", "CITY14", "CITY15", "CITY16", "CITY17", "CITY18"}; // RA2: RA2's form

	int count = Pick_Random_UInt(cells.Count() / 20, cells.Count() / 10) / 2;
	int tries = 0;
	int placed = 0;

	DynamicVectorClass<char *> names;
	for (int index = 0; index < 18; ++index) { // RA2: as above
		names.Add(building_names[index]); // RA2: as above
	}

	HouseClass * neutral = House_From_HousesType(HouseTypeClass::From_Name("Neutral"));

	if (count > 0) {
		while (true) {
			if (tries >= 2 * count || names.Count() <= 1) {
				break;
			}
			int nameIndex = Pick_Random_UInt(0, names.Count() - 1); // RA2: as above
			StructType typeIndex = BuildingTypeClass::From_Name(names[nameIndex]); // RA2: as above
			{
				BuildingClass * building = new BuildingClass(BuildingTypes[typeIndex], neutral); // RA2: as above

				int attempts = 0; // RA2: as above
				bool wasPlaced = false; // RA2: as above
				while (!wasPlaced) { // RA2: as above
					if (attempts >= 60) { // RA2: as above
						break; // RA2: as above
					}
					Cell position = cells[Pick_Random_UInt(0, cells.Count() - 1)]; // RA2: as above
					Cell const * list = building->Class->Occupy_List(1); // RA2: as above
					bool clear = true; // RA2: as above
					while (list->X != 0x7fff || list->Y != 0x7fff) { // RA2: as above
						Cell next = (position + *list); // RA2: as above
						CellClass * cell = &Map[next]; // RA2: as above
						if ((!cell->Is_Tile_Pavement() && !cell->Is_Tile_Misc_Pavement()) || cell->Cell_Occupier() != NULL || cell->Land_Type() == LAND_ROCK || !Map.In_Local_Radar(cell, true)) { // RA2: as above
							clear = false; // RA2: as above
						}
						++list; // RA2: as above
						if (!clear) { // RA2: as above
							break; // RA2: as above
						}
					}
					if (clear) { // RA2: as above
						Coord point(position.X * 256 + 128, position.Y * 256 + 128, 0); // RA2: as above
						if (building->Unlimbo(point, DIR_N)) { // RA2: as above
							int strength = building->Class->MaxStrength; // RA2: as above
							building->Strength = strength; // RA2: as above
							building->EstimatedHealth = strength; // RA2: as above
							building->Mark(MARK_CHANGE); // RA2: as above
							wasPlaced = true; // RA2: as above
							++placed; // RA2: as above
							if (nameIndex > 17) { // RA2: as above
								names.Delete_Index((int)nameIndex); // RA2: as above
							}
						}
					}
					++attempts; // RA2: as above
				}
				if (!wasPlaced) { // RA2: as above
					delete building; // RA2: as above
				}
				++tries; // RA2: as above
			}
			if (placed >= count) {
				break;
			}
		}
	}
}



/// <summary>
/// Scatters civilian traffic through an urban area.
/// This routine populates a finished town with a handful of neutral pedestrians and vehicles.
/// How many turn up, and how battered they are, follows from the biome -- a mutated map keeps
/// few civilians and most of those are in poor shape.
/// </summary>
/// <param name="cells">The cells of the urban area the civilians may occupy.</param>
void MapGeneratorClass::Generate_Urban_Units(const DynamicVectorClass<Cell> & cells)
{
	static char const * names[7] = { // RA2: RA2's form
		"CIV1",
		"CIV2",
		"CIV3",
		"PICK",
		"WINI",
		"BUS",
		"CAR"
	};

	HouseClass * neutral = House_From_HousesType(HouseTypeClass::From_Name("Neutral"));
	int count = Pick_Random_UInt(1, 5); // RA2: as above

	int tries = 0;
	int placed = 0;

	if (count > 0) {
		while (true) {
			if (tries >= 2 * count) {
				break;
			}

			int unit_index = Pick_Random_UInt(0, 6); // RA2: as above
			FootClass * foot; // RA2: as above
			if (unit_index <= 2) {
				foot = new InfantryClass(InfantryTypes[InfantryTypeClass::From_Name(names[unit_index])], neutral); // RA2: as above
			} else {
				foot = new UnitClass(UnitTypes[UnitTypeClass::From_Name(names[unit_index])], neutral); // RA2: as above
			}

			int tries_here = 0;
			bool was_placed = false;
			while (!was_placed) {
				if (tries_here >= 60) {
					break;
				}
				Cell unit_cell = cells[Pick_Random_UInt(0, cells.Count() - 1)];
				if (Map.In_Local_Radar(unit_cell, true) && Map[unit_cell].Cell_Occupier() == NULL) { // RA2: as above
					if (foot->Unlimbo(Coord(unit_cell), (Dir256)(Pick_Random_UInt(0, 7) * 32))) { // RA2: as above
						int strength = foot->TClass->MaxStrength; // RA2: as above
						foot->Strength = strength; // RA2: as above
						foot->EstimatedHealth = strength; // RA2: as above
						foot->Mark(MARK_CHANGE);
						was_placed = true;
						placed++;
					}
				}
				tries_here++;
			}

			if (!was_placed) {
				delete foot;
			}

			tries++;
			if (placed >= count) {
				break;
			}
		}
	}
}



/// <summary>
/// Determines if the ground around a cell is free of cliffs.
/// This routine is used when the generator is looking for somewhere to drop a settlement -- a
/// town wants a decent patch of open ground, not a cliff face through the middle of it.
/// </summary>
/// <param name="cell">The center of the area to examine.</param>
/// <returns>bool; Is the surrounding ground free of cliffs?</returns>
bool MapGeneratorClass::Is_Area_Cliff_Free(const Cell & cell)
{
	int yy = cell.Y;
	int xx = cell.X;

	int xmin = xx - 7;
	int ymin = yy - 7;
	int xmax = xx + 7;
	int ymax = yy + 7;

	for (int x = xmin; x < xmax; x++) {
		for (int y = ymin; y < ymax; y++) {
			if (Map[Cell(x, y)].Is_Tile_Cliff()) {
				return(false);
			}
		}
	}
	return(true);
}


/// <summary>
/// Stamps an isometric tile onto the map.
/// This routine covers the whole footprint of a multi-cell tile, so the generator can lay down
/// a cliff, a road piece or a block of pavement with one call. Any part of the footprint that
/// falls off the map is quietly skipped.
/// </summary>
/// <param name="ittype">The isometric tile type to stamp down.</param>
/// <param name="cell">The top left cell of the tile's footprint.</param>
/// <param name="region">The generator region the covered cells are credited to.</param>
/// <param name="height">The height to lay the tile at, or -1 to leave heights alone.</param>
void MapGeneratorClass::Place_Tile(IsometricTileType ittype, const Cell & cell, int region, int height)
{
	IsometricTileTypeClass * itype = IsometricTileTypes[ittype];
	IsoTileSet * set = (IsoTileSet *)itype->Get_Image_Data();
	if (set != NULL) {
		for (int y = 0; y < itype->Height; y++) {
			for (int x = 0; x < itype->Width; x++) {
				Cell ncell = cell + Cell(x, y);
				if (My_In_Radar(ncell)) {
					CellClass * cptr = &Map[ncell];
					int subtile = itype->SubTile_Index(x, y);
					IsoTileRecord const * record = set->Fetch_Record_Pointer_Unsafe(subtile);
					if (record != NULL) {
						cptr->ITType = itype->HeapID;
						cptr->SubTile = subtile;
						if (height != -1) {
							cptr->Height = height + record->Height - 4;
						}
						cptr->Ramp = record->RampType;
						MapRegionClass::CellData::Set_Region(ncell, region);
					}
				}
			}
		}
	}
}


/// <summary>
/// Finds the cell a paved road run should start from.
/// This routine walks outward until it reaches paved ground that will take a road, then backs
/// off by a random amount. Generate_Urban_Roads pairs it with Plan_Paved_Road to fix both ends
/// of every road in the grid.
/// </summary>
/// <param name="cell">The cell to start walking from.</param>
/// <param name="rect">The bounding rectangle of the urban area the road must stay inside.</param>
/// <param name="dir">The direction the road runs -- FACING_E or FACING_S.</param>
/// <returns>Returns with the junction cell found. Otherwise, Cell(0,0) is returned if the walk
/// left the area first.</returns>
Cell MapGeneratorClass::Plan_Paved_Road_Junction(Cell const & cell, Rect const & rect, FacingType dir)
{
	Cell wcell = cell;

	int margin_x = 0;
	int margin_y = 0;
	int margin_width = 0;
	int margin_height = 0;
	Rect tile_size; // RA2: Plan_Paved_Road_Junction at 0x588560 matches only so

	bool south = dir == FACING_S;

	if (dir == FACING_E) {
		tile_size = Rect(0, 0, 1, 3);
		margin_y = -3;
		margin_height = 6;
	} else {
		tile_size = Rect(0, 0, 3, 1);
		margin_x = -3;
		margin_width = 6;
	}

	while (true) {
		if (!(wcell.X < rect.X + rect.Width && wcell.Y < rect.Y + rect.Height)) return(Cell(0, 0));
		if (Map[wcell].ITType == IsometricTileTypeClass::PaveTile) {
			break;
		}
		wcell = Adjacent_Cell(wcell, dir);
	}

	while (true) {
		if (!(wcell.X < rect.X + rect.Width && wcell.Y < rect.Y + rect.Height)) return(Cell(0, 0));
		if (Can_Place_Paved_Road(Rect(margin_x + wcell.X, margin_y + wcell.Y, margin_width + tile_size.Width, margin_height + tile_size.Height), south, south)) {
			break;
		}
		wcell = Adjacent_Cell(wcell, dir);
	}

	int length = MAX(wcell.Y - cell.Y, wcell.X - cell.X);
	if (length > 0) {
		int shorten = Pick_Random_UInt(0, length);
		while (shorten > 0) {
			Cell back = Adjacent_Cell(wcell, Facing_Sub(dir, FACING_180));
			if (!Can_Place_Paved_Road(Rect(margin_x + back.X, margin_y + back.Y, margin_width + tile_size.Width, margin_height + tile_size.Height), false, false)) break;
			wcell = back;
			shorten--;
		}
	}

	return(wcell);
}


/// <summary>
/// Finds the far end of a paved road run.
/// This routine walks outward from the junction cell for as long as road may legally be laid
/// and the run stays within the town, then backs off. A long road is trimmed by a random
/// amount so that the road grid does not look stamped out.
/// </summary>
/// <param name="cell">The cell the road run starts from.</param>
/// <param name="rect">The bounding rectangle of the urban area the road must stay inside.</param>
/// <param name="dir">The direction the road runs -- FACING_E or FACING_S.</param>
/// <returns>Returns with the cell the road run ends at.</returns>
Cell MapGeneratorClass::Plan_Paved_Road(Cell const & cell, Rect const & rect, FacingType dir)
{
	bool south = dir == FACING_S;
	Cell wcell = cell;

	int margin_x = 0; // RA2: Plan_Paved_Road at 0x588850 matches only so
	int margin_y = 0; // RA2: as above
	int margin_width = 0; // RA2: as above
	int margin_height = 0; // RA2: as above
	Rect tile_size; // RA2: as above

	if (dir == FACING_E) {
		tile_size = Rect(0, 0, 1, 3);
		margin_y = -3; // RA2: as above
		margin_height = 6; // RA2: as above
	} else {
		tile_size = Rect(0, 0, 3, 1);
		margin_x = -3; // RA2: as above
		margin_width = 6; // RA2: as above
	}

	while (true) {
		if (!(wcell.X < rect.X + rect.Width + 1 && wcell.Y < rect.Y + rect.Height + 1)) break;
		if (!Can_Place_Paved_Road(Rect(margin_x + wcell.X, margin_y + wcell.Y, margin_width + tile_size.Width, margin_height + tile_size.Height), south, south)) { // RA2: as above
			break;
		}
		{ Cell next = Adjacent_Cell(wcell, dir); wcell = next; } // RA2: as above
	}

	wcell = Adjacent_Cell(wcell, Facing_Sub(dir, FACING_180));

	int length = MAX(wcell.X - cell.X, wcell.Y - cell.Y);
	if (length > 6) {
		int shorten = Pick_Random_UInt(0, 3);
		while (shorten > 0) {
			Cell back = Adjacent_Cell(wcell, Facing_Sub(dir, FACING_180));
			shorten--;
			wcell = back;
		}
	}

	return(wcell);
}


/// <summary>
/// Determines if a paved road may be laid over an area.
/// The urban road planner uses this routine both to walk a road out to its full length and to
/// decide whether a junction or a cap will fit, so the caller states which kinds of road tile
/// it is willing to build over.
/// </summary>
/// <param name="rect">The area of cells the road would cover.</param>
/// <param name="overlap_road">May road already laid be built over?</param>
/// <param name="overlap_road_end">May an end cap already laid be built over?</param>
/// <returns>bool; May the road be placed here?</returns>
bool MapGeneratorClass::Can_Place_Paved_Road(Rect const & rect, bool overlap_road, bool overlap_road_end)
{
	if (My_In_Radar(Cell(rect.X, rect.Y)) &&
		My_In_Radar(Cell(rect.X + rect.Width - 1, rect.Y)) &&
		My_In_Radar(Cell(rect.X, rect.Y + rect.Height - 1)) &&
		My_In_Radar(Cell(rect.X + rect.Width - 1, rect.Y + rect.Height - 1))) {
		for (int y = rect.Y; y < rect.Y + rect.Height; y++) {
			for (int x = rect.X; x < rect.X + rect.Width; x++) {
				CellClass * cptr = &Map[Cell(x, y)];
				if (cptr->Is_Tile_Paved_Road()) {
					if (!overlap_road) {
						return(false);
					}
				} else if (cptr->Is_Tile_Paved_Road_End()) {
					if (!overlap_road_end) {
						return(false);
					}
				} else if (!cptr->Is_Tile_Clear() && !cptr->Is_Tile_Misc_Pavement() && !cptr->Is_Tile_Pavement()) {
					return(false);
				}
			}
		}
		return(true);
	}
	return(false);
}


/// <summary>
/// Determines if a paved road end cap may be laid over an area.
/// This is the level ground variant of Can_Place_Paved_Road -- an end cap cannot straddle a
/// slope, so every cell of the area must sit at the same height.
/// </summary>
/// <param name="rect">The area of cells the end cap would cover.</param>
/// <param name="overlap_road">May road already laid be built over?</param>
/// <returns>bool; May the end cap be placed here?</returns>
bool MapGeneratorClass::Can_Place_Paved_Road_End(Rect const & rect, bool overlap_road)
{
	if (My_In_Radar(Cell(rect.X, rect.Y)) &&
		My_In_Radar(Cell(rect.X + rect.Width - 1, rect.Y)) &&
		My_In_Radar(Cell(rect.X, rect.Y + rect.Height - 1)) &&
		My_In_Radar(Cell(rect.X + rect.Width - 1, rect.Y + rect.Height - 1))) {

		int height = Map[Cell(rect.X, rect.Y)].Height;

		for (int y = rect.Y; y < rect.Y + rect.Height; y++) {
			for (int x = rect.X; x < rect.X + rect.Width; x++) {
				CellClass * cptr = &Map[Cell(x, y)];
				if (cptr->Height != height) {
					return(false);
				}
				if (cptr->Is_Tile_Paved_Road() || cptr->Is_Tile_Paved_Road_End()) {
					if (!overlap_road) {
						return(false);
					}
				} else if (!cptr->Is_Tile_Clear() && !cptr->Is_Tile_Misc_Pavement() && !cptr->Is_Tile_Pavement()) {
					return(false);
				}
			}
		}
		return(true);
	}
	return(false);
}


/// <summary>
/// Fetches the rectangle that encloses a group of cells.
/// </summary>
/// <param name="cells">The cells to enclose.</param>
/// <returns>Returns with the smallest rectangle that holds every one of the cells.</returns>
Rect MapGeneratorClass::Get_Cell_Bounding_Rect(const DynamicVectorClass<Cell> & cells)
{
	Cell cmin;
	Cell cmax;
	cmin.X = 999;
	cmax.X = 0;
	cmin.Y = 999;
	cmax.Y = 0;
	for (int i = cells.Count() - 1; i >= 0; i--) {
		Cell cell = cells[i];
		cmin.X = MIN(cell.X, cmin.X);
		cmin.Y = MIN(cell.Y, cmin.Y);
		cmax.X = MAX(cell.X, cmax.X);
		cmax.Y = MAX(cell.Y, cmax.Y);
	}
	return(Rect(cmin.X, cmin.Y, (cmax.X - cmin.X) + 1, (cmax.Y - cmin.Y) + 1));
}


/// <summary>
/// Determines if a paved road junction will fit.
/// A junction has to grow out of road that is already there, so the ground it would take
/// over must be paved road or median, and the row it would spill onto must be free for it.
/// </summary>
/// <param name="rect">The area the junction would cover.</param>
/// <returns>bool; Will the junction fit?</returns>
bool MapGeneratorClass::Can_Place_Paved_Road_Junction(Rect & rect)
{
	if (My_In_Radar(Cell(rect.X, rect.Y)) &&
		My_In_Radar(Cell(rect.X + rect.Width - 1, rect.Y)) &&
		My_In_Radar(Cell(rect.X, rect.Y + rect.Height - 1)) &&
		My_In_Radar(Cell(rect.X + rect.Width - 1,rect.Y + rect.Height - 1))) {

		for (int y = rect.Y; y < rect.Y + 3; y++) {
			for (int x = rect.X; x < rect.X + 3; x++) {
				CellClass * cptr = &Map[Cell(x, y)];
				if ((cptr->ITType < IsometricTileTypeClass::PavedRoads || cptr->ITType > IsometricTileTypeClass::PavedRoads + 1) && !cptr->Is_Tile_Road_Median()) {
					return(false);
				}
			}
		}
		for (int x = rect.X; x < rect.X + 3; x++) {
			CellClass * cptr = &Map[Cell(x, rect.Y + 3)];
			if (!cptr->Is_Tile_Pavement() && !cptr->Is_Tile_Clear() && !cptr->Is_Tile_Misc_Pavement()) {
				return(false);
			}
		}
		return(true);
	}
	return(false);
}


/// <summary>
/// Weathers a city's pavement with scattered patches.
/// This routine is used to break up the flat sameness of a large paved area. Patches of the
/// alternate pavement tiles are laid over it wherever one of the stock patch shapes will
/// fit, in proportion to how much pavement there is.
/// </summary>
/// <param name="cells">The paved cells that may serve as patch anchors.</param>
void MapGeneratorClass::Generate_Urban_Pavement(DynamicVectorClass<Cell> & cells)
{
	Rect sizes[5] = {
		Rect(0, 0, 6, 4),
		Rect(0, 0, 4, 6),
		Rect(0, 0, 4, 4),
		Rect(0, 0, 4, 2),
		Rect(0, 0, 2, 4)
	};

	int count = cells.Count() / 50;
	int placed = 0;

	for (int tries = 0; tries < 10 * count && placed < count; tries++) {
		Cell cell = cells[Pick_Random_UInt(0, cells.Count() - 1)];
		int size = 0;
		bool place = false;
		Rect area(cell.X, cell.Y, 0, 0);
		while (!place) { // RA2: Generate_Urban_Pavement at 0x5890B0 matches only so
			if (size >= 5) {
				break;
			}
			area.Width = sizes[size].Width;
			area.Height = sizes[size].Height;
			if (Can_Place_Misc_Pavement(area)) {
				place = true;
			}
			size++;
		} // RA2: Generate_Urban_Pavement at 0x5890B0 has no if (place) {
		if (place) {
			IsometricTileType tile = (IsometricTileType)(Pick_Random_UInt(0, 5) + IsometricTileTypeClass::MiscPaveTile + 8);
			for (int y = 0; y < area.Height / 2; y++) {
				for (int x = 0; x < area.Width / 2; x++) {
					Place_Tile(tile, Cell(area.X + 2 * x, area.Y + 2 * y));
				}
			}
			placed++;
		}
	}
}


/// <summary>
/// Determines if a patch of decorative pavement will fit.
/// The ground has to be paved already and clear of anything standing on it, since the patch
/// only changes how the pavement looks and not what may be built there.
/// </summary>
/// <param name="rect">The area the patch would cover.</param>
/// <returns>bool; Will the patch fit?</returns>
bool MapGeneratorClass::Can_Place_Misc_Pavement(Rect const & rect)
{
	if (My_In_Radar(Cell(rect.X, rect.Y)) &&
		My_In_Radar(Cell((rect.X - 1) + rect.Width, rect.Y)) &&
		My_In_Radar(Cell(rect.X, (rect.Y - 1) + rect.Height)) &&
		My_In_Radar(Cell((rect.X - 1) + rect.Width, (rect.Y - 1) + rect.Height))) {

		for (int y = rect.Y; y < rect.Y + rect.Height; y++) {
			for (int x = rect.X; x < rect.X + rect.Width; x++) {
				CellClass *cptr = &Map[Cell(x,y)];
				IsometricTileType itype = cptr->ITType;
				if (!cptr->Is_Tile_Pavement() && (itype < IsometricTileTypeClass::MiscPaveTile || itype > IsometricTileTypeClass::MiscPaveTile + 7)) {
					return(false);
				}
				if (cptr->Cell_Occupier() != NULL) {
					return(false);
				}
			}
		}
		return(true);
	}
	return(false);
}


/// <summary>
/// Determines if a tile will fit where it is wanted.
/// The generator asks this routine before laying anything down. The ground the tile would
/// cover must be clear and sloped the way the tile expects; the caller may also insist that
/// no other tile has spoken for it, and that the whole tile lands inside the map.
/// </summary>
/// <param name="ittype">The isometric tile type to test.</param>
/// <param name="cell">The cell the tile would be anchored at.</param>
/// <param name="collision">Should ground another tile has claimed be refused?</param>
/// <param name="in_map">Must the whole tile land inside the map?</param>
/// <returns>bool; Will the tile fit?</returns>
bool MapGeneratorClass::Can_Place_Tile(IsometricTileType ittype, Cell const & cell, bool collision, bool in_map)
{
	IsometricTileTypeClass * itype = IsometricTileTypes[ittype];
	IsoTileSet * set = (IsoTileSet *)itype->Get_Image_Data();
	if (set != NULL) {
		for (int y = 0; y < itype->Height; y++) {
			for (int x = 0; x < itype->Width; x++) {
				Cell ncell = cell + Cell(x, y);
				if (My_In_Radar(ncell)) {
					CellClass * cptr = &Map[ncell];
					int subtile = itype->SubTile_Index(x, y);
					IsoTileRecord const * record = set->Fetch_Record_Pointer_Unsafe(subtile);
					if (record != NULL) {
						if ((!cptr->Is_Tile_Clear() && !cptr->Is_Tile_Ramp()) || cptr->Ramp != record->RampType || collision && MapRegionClass::Get_Cell_Data(ncell).TilePlaced) {
							return(false);
						}
					}
				} else if (in_map) {
					return(false);
				}
			}
		}
	}
	return(true);
}


/// <summary>
/// Takes a tile back off the map.
/// This routine undoes a placement the generator has thought better of, leaving the ground
/// it covered blank for something else to claim.
/// </summary>
/// <param name="ittype">The isometric tile type to remove.</param>
/// <param name="cell">The cell the tile is anchored at.</param>
void MapGeneratorClass::Remove_Tile(IsometricTileType ittype, Cell const & cell)
{
	IsometricTileTypeClass * itype = IsometricTileTypes[ittype];
	IsoTileSet * set = (IsoTileSet *)itype->Get_Image_Data();
	if (set != NULL) {
		for (int y = 0; y < itype->Height; y++) {
			for (int x = 0; x < itype->Width; x++) {
				Cell ncell = cell + Cell(x, y);
				if (My_In_Radar(ncell)) {
					CellClass * cptr = &Map[ncell];
					int subtile = itype->SubTile_Index(x, y);
					IsoTileRecord const * record = set->Fetch_Record_Pointer_Unsafe(subtile);
					if (record != NULL) {
						cptr->ITType = TILE_NONE;
						cptr->SubTile = 0;
					}
				}
			}
		}
	}
}


/// <summary>
/// Reserves or gives back the ground a tile would cover.
/// The dirt road builder uses this routine to stop two road pieces laying claim to the same
/// ground while it is still deciding, and to release that claim when a stretch of road is
/// abandoned.
/// </summary>
/// <param name="ittype">The isometric tile type whose footprint is affected.</param>
/// <param name="cell">The cell the tile is anchored at.</param>
/// <param name="placed">Should the ground be reserved?</param>
void MapGeneratorClass::Mark_Tile(IsometricTileType ittype, Cell const & cell, bool placed)
{
	IsometricTileTypeClass * itype = IsometricTileTypes[ittype];
	IsoTileSet * set = (IsoTileSet *)itype->Get_Image_Data();
	if (set != NULL) {
		for (int y = 0; y < itype->Height; y++) {
			for (int x = 0; x < itype->Width; x++) {
				Cell ncell = cell + Cell(x, y);
				if (My_In_Radar(ncell)) {
					int subtile = itype->SubTile_Index(x, y);
					IsoTileRecord const * record = set->Fetch_Record_Pointer_Unsafe(subtile);
					if (record != NULL) {
						MapRegionClass::Get_Cell_Data(ncell).TilePlaced = placed;
					}
				}
			}
		}
	}
}


/// <summary>
/// Plants the rural settlements the map calls for.
/// This routine finds open ground with room enough for a road junction and then hands it to
/// the road, building and unit generators in turn, so that every settlement ends up with a
/// road network and something standing along it.
/// </summary>
void MapGeneratorClass::Generate_Rural_Areas(void)
{
	Init_Dirt_Roads();

	int count = (SeedData.Cities + 0.49) * 2;
	int tries = 0;
	int placed = 0;

	while (true) {
		if (placed >= count) {
			break;
		}
		Cell cell(0, 0); // RA2: Generate_Rural_Areas at 0x589890 matches only so
		int tries_here = 0;
		while (true) {
			cell = MapRegionClass::Pick_Random_Clear_Map_Cell();
			if (++tries_here >= 20) break;
			if (MapRegionClass::CellData::Is_Not_Inviolate(cell) && Can_Place_Paved_Road(Rect(cell.X - 4, cell.Y - 4, 8, 8), false, false) && Map.In_Local_Radar(cell)) {
				break;
			}
		}

		if (tries_here != 20) {
			if (Generate_Rural_Roads(cell)) {
				Generate_Rural_Buildings(cell);
				Generate_Rural_Units(cell);
				placed++;
			}
		}

		tries++;
		if (tries >= 10) {
			break;
		}
	}
}


/// <summary>
/// Scatters civilian structures about a rural settlement.
/// This routine drops a handful of neutral buildings on the flat ground near the center,
/// taking each one out of the pool as it is used so the settlement is not built from the
/// same house over and over. The desert, temperate and mutated biomes draw on the civilian
/// structures alone and leave the abandoned ones out.
/// </summary>
/// <param name="cell">The center of the settlement to build around.</param>
void MapGeneratorClass::Generate_Rural_Buildings(const Cell & cell)
{
	static char const * _names[] = {
		"ABAN01",
		"ABAN02",
		"ABAN03",
		"ABAN04",
		"ABAN05",
		"ABAN06",
		"ABAN07",
		"ABAN08",
		"ABAN09",
		"ABAN10",
		"ABAN11",
		"ABAN12",
		"ABAN13",
		"ABAN14",
		"ABAN15",
		"CA0001",
		"CA0002",
		"CA0003",
		"CA0004",
		"CA0005",
		"CA0006",
		"CA0007",
		"CA0008",
		"CA0009",
		"CA0010",
		"CA0011",
		"CA0012",
		"CA0013",
		"CA0014",
		"CA0015",
		"CA0016",
		"CA0017",
		"CA0018",
		"CA0019",
		"CA0020",
		"CA0021"
	};

	int count = Pick_Random_UInt(8, 12);
	int tries = 0;
	int placed = 0;

	CellClass & center = Map[cell]; // RA2: Generate_Rural_Buildings at 0x589AC0 matches only so
	int height = center.Height; // RA2: as above

	int min_building = 0;
	int max_building = ARRAY_SIZE(_names) - 1;
	if (SeedData.Biome == BIOME_TUNDRA) { // RA2: as above
		min_building = 15;
	}

	DynamicVectorClass<char *> names;
	for (int i = 0; i < ARRAY_SIZE(_names); i++) {
		names.Add((char *&)_names[i]);
	}

	HouseClass * neutral = House_From_HousesType(HouseTypeClass::From_Name("Neutral"));

	if (count > 0) {
		while (placed < count) { // RA2: as above
			if (tries >= 2 * count) {
				break;
			}
			int name_index = Pick_Random_UInt(min_building, max_building);
			StructType bindex = BuildingTypeClass::From_Name(names[name_index]);
			if (bindex != STRUCT_NONE) {
				BuildingClass * building = new BuildingClass(BuildingTypes[bindex], neutral);

				int tries_here = 0;
				bool was_placed = false;
				while (!was_placed) {
					if (tries_here >= 60) {
						break;
					}

					Cell building_cell = cell + Cell(Pick_Random_UInt(0, 14) - 7, Pick_Random_UInt(0, 14) - 7);

					Cell const * list = building->Class->Occupy_List(true);
					bool ok = true;
					while (*list != REFRESH_EOL) {
						CellClass * cellptr = &Map[building_cell + *list];
						if (cellptr->Cell_Occupier() != NULL || !cellptr->Is_Tile_Clear() || cellptr->Height != height || cellptr->Land_Type() == LAND_ROCK || !Map.In_Local_Radar(cellptr)) {
							ok = false;
						}
						list++;
						if (!ok) break;
					}

					if (ok) {
						if (building->Unlimbo(Coord(building_cell, 0))) {
							was_placed = true;
							placed++;
							names.Delete_Index(name_index);
							max_building--;
						}
					}
					tries_here++;
				}

				if (!was_placed) {
					delete building;
				}

				tries++;
			} // RA2: Generate_Rural_Buildings at 0x589AC0 has no if (placed >= count) {
		}
	}
}


/// <summary>
/// Scatters civilians and their vehicles about a rural settlement.
/// This routine gives the settlement some neutral life of its own. The harsher the biome,
/// the likelier each of them is to turn up already the worse for wear.
/// </summary>
/// <param name="cell">The center of the settlement to populate.</param>
void MapGeneratorClass::Generate_Rural_Units(const Cell & cell)
{
	static char const * _names[] = {
		"CIV1",
		"CIV2",
		"CIV3",
		"PICK",
		"WINI",
		"CAR"
	};

	HouseClass * neutral = House_From_HousesType(HouseTypeClass::From_Name("Neutral")); // RA2: Generate_Rural_Units at 0x589F00 has no static int _max_counts[BIOME_COUNT] = {
	int count = Pick_Random_UInt(1, 4); // RA2: Generate_Rural_Units at 0x589F00 matches only so

	int tries = 0;
	int placed = 0;

	while (placed < count) {
		if (tries >= 2 * count) {
			break;
		}

		int unit_index = Pick_Random_UInt(0, ARRAY_SIZE(_names) - 1); // RA2: as above
		FootClass * foot = NULL;
		if (unit_index <= 2) { // RA2: as above
			foot = new InfantryClass(InfantryTypes[InfantryTypeClass::From_Name(_names[unit_index])], neutral); // RA2: as above
		} else {
			foot = new UnitClass(UnitTypes[UnitTypeClass::From_Name(_names[unit_index])], neutral); // RA2: as above
		}

		int tries_here = 0;
		bool was_placed = false;
		while (!was_placed) {
			if (tries_here >= 60) {
				break;
			}
			Cell unit_cell = cell + Cell(Pick_Random_UInt(0, 16) - 8, Pick_Random_UInt(0, 16) - 8);
			if (Map.In_Local_Radar(unit_cell) && Map[unit_cell].Cell_Occupier() == NULL) {
				if (foot->Unlimbo(Coord(unit_cell), (Dir256)(Pick_Random_UInt(0, 7) * 32))) { // RA2: as above
					foot->Strength = foot->Techno_Type_Class()->MaxStrength; // RA2: as above
					foot->EstimatedHealth = foot->Strength; // RA2: as above
					foot->Mark(MARK_CHANGE);
					was_placed = true;
					placed++;
				}
			}
			tries_here++;
		}

		if (!was_placed) {
			delete foot;
		}

		tries++;
	}
}


/// <summary>
/// Lays out the dirt roads of a rural settlement.
/// This routine starts from a junction at the cell given and grows the road outward until
/// it runs out of room. Every loose end is sealed with a cap piece, and a stretch that
/// cannot be carried onward is taken back off the map and capped instead, so the network
/// never trails off into open ground.
/// </summary>
/// <param name="cell">The cell the road network should start from.</param>
/// <returns>bool; Was anything more than a lone junction laid down?</returns>
#ifdef RA2_NON_MATCHING // not exact: 1114 bytes differ (adopted text)
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
#endif


// --- FACING 0 (NORTH) ---
static int Road_N_1Way[] = {95, 96, 97};
static int Road_N_2Way[] = {0, 1, 2, 3, 4, 5, 11, 84, 85, 86, 87, 88, 89, 90, 91, 98, 99};
static int Road_N_3Way[] = {24, 26, 27};
static int Road_N_4Way[] = {28};

// --- FACING 1 (NORTHEAST) ---
static int Road_NE_1Way[] = {45, 46, 47};
static int Road_NE_2Way[] = {0, 6, 7, 8, 9, 10, 11, 35, 36, 37, 38, 39, 40, 41, 48, 49};
static int Road_NE_3Way[] = {29, 31, 32, 34};
static int Road_NE_4Way[] = {33};

// --- FACING 2 (EAST) ---
static int Road_E_1Way[] = {62, 63, 64};
static int Road_E_2Way[] = {1, 6, 12, 13, 14, 15, 51, 52, 53, 54, 55, 56, 57, 58, 65, 66};
static int Road_E_3Way[] = {24, 25, 27, 34};
static int Road_E_4Way[] = {28};

// --- FACING 3 (SOUTHEAST) ---
static int Road_SE_1Way[] = {78, 79, 80};
static int Road_SE_2Way[] = {2, 7, 12, 16, 17, 18, 68, 69, 70, 71, 72, 73, 74, 81, 82};
static int Road_SE_3Way[] = {29, 30, 32};
static int Road_SE_4Way[] = {33};

// --- FACING 4 (SOUTH) ---
static int Road_S_1Way[] = {92, 93, 94};
static int Road_S_2Way[] = {8, 13, 16, 19, 20, 21, 84, 85, 86, 87, 88, 89, 90, 91, 98, 99};
static int Road_S_3Way[] = {24, 25, 26};
static int Road_S_4Way[] = {28};

// --- FACING 5 (SOUTHWEST) ---
static int Road_SW_1Way[] = {42, 43, 44};
static int Road_SW_2Way[] = {3, 14, 17, 19, 22, 35, 36, 37, 38, 39, 40, 41, 48, 49};
static int Road_SW_3Way[] = {29, 30, 31};
static int Road_SW_4Way[] = {33};

// --- FACING 6 (WEST) ---
static int Road_W_1Way[] = {59, 60, 61};
static int Road_W_2Way[] = {4, 9, 18, 20, 23, 51, 52, 53, 54, 55, 56, 57, 58, 65, 66};
static int Road_W_3Way[] = {25, 26, 27, 34};
static int Road_W_4Way[] = {28};

// --- FACING 7 (NORTHWEST) ---
static int Road_NW_1Way[] = {75, 76, 77};
static int Road_NW_2Way[] = {5, 10, 15, 21, 22, 23, 68, 69, 70, 71, 72, 73, 74, 81, 82};
static int Road_NW_3Way[] = {30, 31, 32};
static int Road_NW_4Way[] = {33};

/*
 * --- THE MASTER LOOKUP TABLE ---
 * Organizes the sub-arrays into FACING_COUNT arrays
 */
static int * RoadTileLibrary[FACING_COUNT][4] = {
	{ Road_N_1Way,  Road_N_2Way,  Road_N_3Way,  Road_N_4Way  }, // North
	{ Road_NE_1Way, Road_NE_2Way, Road_NE_3Way, Road_NE_4Way }, // North-East
	{ Road_E_1Way,  Road_E_2Way,  Road_E_3Way,  Road_E_4Way  }, // East
	{ Road_SE_1Way, Road_SE_2Way, Road_SE_3Way, Road_SE_4Way }, // South-East
	{ Road_S_1Way,  Road_S_2Way,  Road_S_3Way,  Road_S_4Way  }, // South
	{ Road_SW_1Way, Road_SW_2Way, Road_SW_3Way, Road_SW_4Way }, // South-West
	{ Road_W_1Way,  Road_W_2Way,  Road_W_3Way,  Road_W_4Way  }, // West
	{ Road_NW_1Way, Road_NW_2Way, Road_NW_3Way, Road_NW_4Way }  // North-West
};

/*
 * --- THE COUNT TABLE ---
 * Stores the number of tile variations available for each direction/complexity pair
 */
static int RoadTileCounts[FACING_COUNT][4] = {
	{ ARRAY_SIZE(Road_N_1Way),  ARRAY_SIZE(Road_N_2Way),  ARRAY_SIZE(Road_N_3Way),  ARRAY_SIZE(Road_N_4Way)  }, // N
	{ ARRAY_SIZE(Road_NE_1Way), ARRAY_SIZE(Road_NE_2Way), ARRAY_SIZE(Road_NE_3Way), ARRAY_SIZE(Road_NE_4Way) }, // NE
	{ ARRAY_SIZE(Road_E_1Way),  ARRAY_SIZE(Road_E_2Way),  ARRAY_SIZE(Road_E_3Way),  ARRAY_SIZE(Road_E_4Way)  }, // E
	{ ARRAY_SIZE(Road_SE_1Way), ARRAY_SIZE(Road_SE_2Way), ARRAY_SIZE(Road_SE_3Way), ARRAY_SIZE(Road_SE_4Way) }, // SE
	{ ARRAY_SIZE(Road_S_1Way),  ARRAY_SIZE(Road_S_2Way),  ARRAY_SIZE(Road_S_3Way),  ARRAY_SIZE(Road_S_4Way)  }, // S
	{ ARRAY_SIZE(Road_SW_1Way), ARRAY_SIZE(Road_SW_2Way), ARRAY_SIZE(Road_SW_3Way), ARRAY_SIZE(Road_SW_4Way) }, // SW
	{ ARRAY_SIZE(Road_W_1Way),  ARRAY_SIZE(Road_W_2Way),  ARRAY_SIZE(Road_W_3Way),  ARRAY_SIZE(Road_W_4Way)  }, // W
	{ ARRAY_SIZE(Road_NW_1Way), ARRAY_SIZE(Road_NW_2Way), ARRAY_SIZE(Road_NW_3Way), ARRAY_SIZE(Road_NW_4Way) }  // NW
};


/// <summary>
/// Finds a dirt road piece that connects the way the road needs to go.
/// This routine is used by the rural road builder to carry a road onward. It hunts the road
/// library for a shape that offers a connection facing the right way and that will fit on
/// the ground where that connection would land. How many ways the piece branches is left
/// to chance within the caller's limit, so junctions turn up of their own accord.
/// </summary>
/// <param name="facing">The direction the piece must connect in.</param>
/// <param name="target_cell">The cell the matching connection must land on.</param>
/// <param name="max_connections">The most connections the chosen piece may have.</param>
/// <param name="out_ittype">Filled in with the road piece chosen.</param>
/// <param name="out_origin">Filled in with the cell to anchor the piece at.</param>
/// <param name="out_link_index">Filled in with the connection that matched.</param>
/// <returns>bool; Was a road piece found that fits? The outputs are cleared when none
/// does.</returns>
bool MapGeneratorClass::Find_Dirt_Road_Tile(FacingType facing, Cell const & target_cell, int max_connections, IsometricTileType & out_ittype, Cell & out_origin, int & out_link_index)
{
	int current_conn_count;
	int initial_conn_count;
	initial_conn_count = current_conn_count = 1;
	if (max_connections > 1) {
		double roll = Sample_Truncated_Normal(2.0, 0.5, 2.0, max_connections);
		current_conn_count = roll; // RA2: RA2's form
		initial_conn_count = current_conn_count; // RA2: as above
	}
	static const double _offset_chance = 0.7; // RA2: the chance that a two-way piece is looked for from the eighth piece on
	while (true) {
		int tile_count = RoadTileCounts[facing][current_conn_count - 1];
		int * tile_list = RoadTileLibrary[facing][current_conn_count - 1];
		int start_index;
		if (current_conn_count == 2 && (unsigned)RMGRandom()* (1.0 / 4294967295.0) < _offset_chance) { // RA2: a named constant of the function: the executable reads this 0.7 at 0x7A5D70, among the function statics, while Generate_Vegetation's literal 0.7 lies in the literal pool (0x7A6000)
			start_index = Pick_Random_UInt(7, tile_count - 1);
		} else {
			start_index = Pick_Random_UInt(0, tile_count - 1);
		}
		for (int i = 0; i < tile_count; i++) {
			DirtRoadTile & tile = DirtRoadTiles[tile_list[(i + start_index) % tile_count]];
			int link_index;
			for (link_index = 0; link_index < tile.Count; link_index++) {
				DirtRoadLink connection = tile.Links[link_index]; // RA2: as above
				if (connection.Facing == facing) { // RA2: as above
					break;
				}
			}
			DirtRoadLink link;
			link.Offset = tile.Links[link_index].Offset;
			Cell origin = target_cell - link.Offset;
			if (Can_Place_Tile(tile.ITType, origin, 1, 1)) {
				DirtRoadLink link2;
				link2.Offset = tile.Links[link_index].Offset;
				out_origin = target_cell - link2.Offset;
				out_ittype = tile.ITType;
				out_link_index = link_index;
				return(true);
			}
		}
		current_conn_count = (current_conn_count + 1) % (max_connections + 1);
		if (current_conn_count == 0) {
			current_conn_count = 1;
		}
		if (current_conn_count == initial_conn_count) {
			out_ittype = ISOTILE_NONE;
			out_origin = Cell(0, 0);
			out_link_index = -1;
			return(false);
		}
	}
}



/// <summary>
/// Rings each player's start position with floodlights.
/// This routine is used to light the starting positions on maps set at dusk or at night.
/// The ring is turned to whatever angle clears the terrain, and the lights are tagged so
/// that they can be switched with the time of day when the map uses transitions.
/// </summary>
void MapGeneratorClass::Generate_Lights(void)
{
	StructType glite = BuildingTypeClass::From_Name("GALITE"); // RA2: Generate_Lights at 0x58A9C0 has no TagTypeClass, SeedData, UseTransitions
	if (glite != STRUCT_NONE) {
		HouseClass * hptr = House_From_HousesType(HouseTypeClass::From_Name("Special"));
		if (hptr != NULL) {
			bool old = Debug_Map;
			Debug_Map = true;
			int i;
			for (i = 0; i < SeedData.NumPlayers; i++) {
				Cell waypoint = Scen->Get_Waypoint_Cell(i);
				int tries = 20;

				static const int _light_counts[] = {0, 0, 2, 4};
				int light_count = _light_counts[SeedData.Time];

				bool can_place = false; // RA2: Generate_Lights at 0x58A9C0 has no SeedData, UseTransitions
				int j;
				while (tries >= 0 && !can_place) {
					double angle = Random_Fraction() * DEG_TO_RAD(360);
					can_place = true;

					for (j = 0; j < light_count; j++) {
						Coord pc = Move_Coord(Coord(waypoint), j * DEG_TO_RAD(360) / light_count + angle, 8 * CELL_LEPTON); // RA2: Generate_Lights at 0x58A9C0 matches only so
						Cell place_cell = pc.As_Cell(); // RA2: as above
						if (!BuildingTypes[glite]->Legal_Placement(place_cell, hptr) || !Map.In_Radar(place_cell)) {
							can_place = false;
							break;
						}
					}

					if (can_place) {
						for (j = 0; j < light_count; j++) {
							Coord pc = Move_Coord(Coord(waypoint), j * DEG_TO_RAD(360) / light_count + angle, 8 * CELL_LEPTON); // RA2: as above
						Cell place_cell = pc.As_Cell(); // RA2: as above
							BuildingClass * light = new BuildingClass(BuildingTypes[glite], hptr);
							if (!light->Unlimbo(Coord(place_cell))) {
								delete light;
							} // RA2: Generate_Lights at 0x58A9C0 has no SeedData, UseTransitions
						}
					}

					tries--;
				}
			}
			Debug_Map = old;
		}
	}
}


void MapGeneratorClass::Generate_Neutral_Buildings(void) // RA2: not in TibSun, at 0x58AD90
{
	MapRegionClass * region;
	int j;
	if (SeedData.MapType == 2) {
		int count = Pick_Random_UInt(0, 2);
		int i = 0;
		while (i < count) {
			j = 0;
			if (MapRegionClass::MapRegions.Count() > 0) {
				do {
					region = MapRegionClass::MapRegions[j];
					MapRegionClass * same = region;
					region = same;
					if (MapRegionClass::MapRegions[j]->StartCount > 0) {
						region->Place_Neutral_Tech_Building();
					}
					j++;
				} while (j < MapRegionClass::MapRegions.Count());
			}
			i++;
		}
		return;
	}

	HouseClass * neutral = House_From_HousesType(HouseTypeClass::From_Name("Neutral"));
	int count = Pick_Random_UInt(0, 4);

	for (int i = 0; i < count; i++) {
		bool placed = false;
		int tries = 0;

		BuildingClass * building = new BuildingClass(Rule->NeutralTechBuildings[Pick_Random_UInt(0, Rule->NeutralTechBuildings.Count() - 1)], neutral);

		for (; !placed; ++tries) {
			if (tries >= 100) {
				break;
			}

			Cell building_cell = MapRegionClass::Pick_Random_Clear_Map_Cell();
			int height = Map[building_cell].Height;

			Cell const * list = building->Class->Occupy_List(true);
			bool ok = true;
			while (*list != REFRESH_EOL) {
				CellClass * cellptr = &Map[building_cell + *list];
				if (cellptr->Cell_Occupier() != NULL || !cellptr->Is_Tile_Clear() || cellptr->Height != height || cellptr->Land_Type() == LAND_ROCK || !Map.In_Local_Radar(cellptr) || !MapRegionClass::CellData::Is_Not_Inviolate(cellptr->CellID)) {
					ok = false;
				}
				list++;
				if (!ok) break;
			}

			if (ok) {
				if (building->Unlimbo(Coord(building_cell, 0))) {
					placed = true;
				}
			}
		}

		if (!placed) {
			if (building != NULL) {
				delete building;
			}
		}
	}
}





/// <summary>
/// Constructs a DirtRoadLink from a cell offset and a facing direction.
/// </summary>
/// <param name="cell">Cell offset stored in Offset.</param>
/// <param name="dir">Facing direction stored in Facing.</param>
DirtRoadLink::DirtRoadLink(Cell const & cell, FacingType dir) :
	Offset(cell),
	Facing(dir)
{
}


/// <summary>
/// Default constructor; sets Offset to (0,0) and Facing to FACING_NONE.
/// </summary>
DirtRoadLink::DirtRoadLink(void) :
	Offset(0, 0),
	Facing(FACING_NONE)
{
}


/// <summary>
/// Copy constructor; copies Offset and Facing from another DirtRoadLink.
/// </summary>
/// <param name="that">Source link to copy.</param>
DirtRoadLink::DirtRoadLink(DirtRoadLink const & that) :
	Offset(that.Offset),
	Facing(that.Facing)
{
}


/// <summary>
/// Constructs a pending stretch of dirt road.
/// A node pairs the road piece to be laid with the cap piece that seals it off should the
/// road get no further, and remembers the way the road came in so the builder does not
/// double back on itself. The rural road generator keeps these in its work queue.
/// </summary>
/// <param name="road_tile">The road piece to lay.</param>
/// <param name="origin">The cell the road piece is anchored at.</param>
/// <param name="cap_tile">The cap piece that seals this stretch off.</param>
/// <param name="cap_origin">The cell the cap piece is anchored at.</param>
/// <param name="link_index">The connection the road arrived through, or -1 for the first
/// piece.</param>
DirtRoadNode::DirtRoadNode(IsometricTileType road_tile, Cell const & origin, IsometricTileType cap_tile, Cell const & cap_origin, int link_index) :
	Tile(road_tile),
	Origin(origin),
	CapTile(cap_tile),
	CapOrigin(cap_origin),
	BackLinkIndex(link_index)
{
}


/// <summary>
/// Constructs an empty dirt road node.
/// </summary>
DirtRoadNode::DirtRoadNode(void) :
	Tile(ISOTILE_NONE),
	Origin(Cell(0, 0)),
	CapTile(ISOTILE_NONE),
	CapOrigin(Cell(0, 0)),
	BackLinkIndex(-1)
{
}


/// <summary>
/// Copy constructor; copies all five members from another DirtRoadNode.
/// </summary>
/// <param name="that">Source struct to copy.</param>
DirtRoadNode::DirtRoadNode(DirtRoadNode const & that) :
	Tile(that.Tile),
	Origin(that.Origin),
	CapTile(that.CapTile),
	CapOrigin(that.CapOrigin),
	BackLinkIndex(that.BackLinkIndex)
{
}


/// <summary>
/// Constructs a dirt road tile with a single connection.
/// This is the shape the road builder uses to cap a road off.
/// </summary>
DirtRoadTile::DirtRoadTile(DirtRoadLink const & link1) :
	ITType(ISOTILE_CLEAR)
{
	Count = 1;
	Links = new DirtRoadLink[1];
	Links[0] = link1;
}


/// <summary>
/// Constructs a dirt road tile with two connections.
/// </summary>
DirtRoadTile::DirtRoadTile(DirtRoadLink const & link1, DirtRoadLink const & link2) :
	ITType(ISOTILE_CLEAR)
{
	Count = 2;
	Links = new DirtRoadLink[2];
	Links[0] = link1;
	Links[1] = link2;
}


/// <summary>
/// Constructs a dirt road tile with three connections.
/// </summary>
DirtRoadTile::DirtRoadTile(DirtRoadLink const & link1, DirtRoadLink const & link2, DirtRoadLink const & link3) :
	ITType(ISOTILE_CLEAR)
{
	Count = 3;
	Links = new DirtRoadLink[3];
	Links[0] = link1;
	Links[1] = link2;
	Links[2] = link3;
}


/// <summary>
/// Constructs a dirt road tile with four connections.
/// </summary>
DirtRoadTile::DirtRoadTile(DirtRoadLink const & link1, DirtRoadLink const & link2, DirtRoadLink const & link3, DirtRoadLink const & link4) :
	ITType(ISOTILE_CLEAR)
{
	Count = 4;
	Links = new DirtRoadLink[4];
	Links[0] = link1;
	Links[1] = link2;
	Links[2] = link3;
	Links[3] = link4;
}


/// <summary>
/// Constructs a dirt road tile with no connections.
/// This is the placeholder for the library slots that carry no road at all.
/// </summary>
DirtRoadTile::DirtRoadTile()// :
{
	Count = 0;
	Links = NULL;
}


/// <summary>
/// Destructor; deletes the Links array and sets Links to NULL.
/// </summary>
DirtRoadTile::~DirtRoadTile(void)
{
	delete Links;
	Links = NULL;
}


DirtRoadTile DirtRoadTiles[] = {
	/* 0 */ DirtRoadTile(DirtRoadLink(Cell(0, 0), FACING_N), DirtRoadLink(Cell(2, 0), FACING_NE)),
	/* 1 */ DirtRoadTile(DirtRoadLink(Cell(0, 0), FACING_N), DirtRoadLink(Cell(1, 0), FACING_E)),
	/* 2 */ DirtRoadTile(DirtRoadLink(Cell(0, 0), FACING_N), DirtRoadLink(Cell(1, 1), FACING_SE)),
	/* 3 */ DirtRoadTile(DirtRoadLink(Cell(0, 0), FACING_N), DirtRoadLink(Cell(1, 0), FACING_SW)),
	/* 4 */ DirtRoadTile(DirtRoadLink(Cell(0, 0), FACING_N), DirtRoadLink(Cell(0, 0), FACING_W)),
	/* 5 */ DirtRoadTile(DirtRoadLink(Cell(1, 0), FACING_N), DirtRoadLink(Cell(1, 1), FACING_NW)),
	/* 6 */ DirtRoadTile(DirtRoadLink(Cell(1, 0), FACING_NE), DirtRoadLink(Cell(1, 1), FACING_E)),
	/* 7 */ DirtRoadTile(DirtRoadLink(Cell(1, 0), FACING_NE), DirtRoadLink(Cell(1, 1), FACING_SE)),
	/* 8 */ DirtRoadTile(DirtRoadLink(Cell(1, 0), FACING_NE), DirtRoadLink(Cell(0, 1), FACING_S)),
	/* 9 */ DirtRoadTile(DirtRoadLink(Cell(1, 0), FACING_NE), DirtRoadLink(Cell(0, 0), FACING_W)),
	/* 10 */ DirtRoadTile(DirtRoadLink(Cell(2, 0), FACING_NE), DirtRoadLink(Cell(1, 1), FACING_NW)),
	/* 11 */ DirtRoadTile(DirtRoadLink(Cell(2, 0), FACING_NE), DirtRoadLink(Cell(0, 0), FACING_N)),
	/* 12 */ DirtRoadTile(DirtRoadLink(Cell(1, 0), FACING_E), DirtRoadLink(Cell(1, 2), FACING_SE)),
	/* 13 */ DirtRoadTile(DirtRoadLink(Cell(1, 0), FACING_E), DirtRoadLink(Cell(0, 1), FACING_S)),
	/* 14 */ DirtRoadTile(DirtRoadLink(Cell(1, 0), FACING_E), DirtRoadLink(Cell(1, 0), FACING_SW)),
	/* 15 */ DirtRoadTile(DirtRoadLink(Cell(1, 0), FACING_E), DirtRoadLink(Cell(1, 1), FACING_NW)),
	/* 16 */ DirtRoadTile(DirtRoadLink(Cell(2, 1), FACING_SE), DirtRoadLink(Cell(0, 1), FACING_S)),
	/* 17 */ DirtRoadTile(DirtRoadLink(Cell(1, 1), FACING_SE), DirtRoadLink(Cell(1, 0), FACING_SW)),
	/* 18 */ DirtRoadTile(DirtRoadLink(Cell(1, 1), FACING_SE), DirtRoadLink(Cell(0, 0), FACING_W)),
	/* 19 */ DirtRoadTile(DirtRoadLink(Cell(1, 1), FACING_S), DirtRoadLink(Cell(1, 0), FACING_SW)),
	/* 20 */ DirtRoadTile(DirtRoadLink(Cell(0, 1), FACING_S), DirtRoadLink(Cell(0, 0), FACING_W)),
	/* 21 */ DirtRoadTile(DirtRoadLink(Cell(0, 1), FACING_S), DirtRoadLink(Cell(1, 1), FACING_NW)),
	/* 22 */ DirtRoadTile(DirtRoadLink(Cell(1, 1), FACING_SW), DirtRoadLink(Cell(1, 1), FACING_NW)),
	/* 23 */ DirtRoadTile(DirtRoadLink(Cell(0, 1), FACING_W), DirtRoadLink(Cell(1, 1), FACING_NW)),
	/* 24 */ DirtRoadTile(DirtRoadLink(Cell(0, 0), FACING_N), DirtRoadLink(Cell(1, 0), FACING_E), DirtRoadLink(Cell(0, 1), FACING_S)),
	/* 25 */ DirtRoadTile(DirtRoadLink(Cell(1, 0), FACING_E), DirtRoadLink(Cell(0, 1), FACING_S), DirtRoadLink(Cell(0, 0), FACING_W)),
	/* 26 */ DirtRoadTile(DirtRoadLink(Cell(0, 0), FACING_N), DirtRoadLink(Cell(0, 1), FACING_S), DirtRoadLink(Cell(0, 0), FACING_W)),
	/* 27 */ DirtRoadTile(DirtRoadLink(Cell(0, 0), FACING_N), DirtRoadLink(Cell(1, 0), FACING_E), DirtRoadLink(Cell(0, 0), FACING_W)),
	/* 28 */ DirtRoadTile(DirtRoadLink(Cell(0, 0), FACING_N), DirtRoadLink(Cell(1, 0), FACING_E), DirtRoadLink(Cell(0, 1), FACING_S), DirtRoadLink(Cell(0, 0), FACING_W)),
	/* 29 */ DirtRoadTile(DirtRoadLink(Cell(2, 0), FACING_NE), DirtRoadLink(Cell(2, 2), FACING_SE), DirtRoadLink(Cell(1, 1), FACING_SW)),
	/* 30 */ DirtRoadTile(DirtRoadLink(Cell(1, 1), FACING_NW), DirtRoadLink(Cell(1, 1), FACING_SW), DirtRoadLink(Cell(2, 2), FACING_SE)),
	/* 31 */ DirtRoadTile(DirtRoadLink(Cell(2, 0), FACING_NE), DirtRoadLink(Cell(1, 1), FACING_NW), DirtRoadLink(Cell(1, 1), FACING_SW)),
	/* 32 */ DirtRoadTile(DirtRoadLink(Cell(1, 1), FACING_NW), DirtRoadLink(Cell(2, 0), FACING_NE), DirtRoadLink(Cell(2, 2), FACING_SE)),
	/* 33 */ DirtRoadTile(DirtRoadLink(Cell(1, 1), FACING_NW), DirtRoadLink(Cell(1, 1), FACING_SW), DirtRoadLink(Cell(2, 0), FACING_NE), DirtRoadLink(Cell(2, 2), FACING_SE)),
	/* 34 */ DirtRoadTile(DirtRoadLink(Cell(3, 0), FACING_NE), DirtRoadLink(Cell(3, 2), FACING_E), DirtRoadLink(Cell(0, 2), FACING_W)),
	/* 35 */ DirtRoadTile(DirtRoadLink(Cell(3, 0), FACING_NE), DirtRoadLink(Cell(1, 2), FACING_SW)),
	/* 36 */ DirtRoadTile(DirtRoadLink(Cell(3, 0), FACING_NE), DirtRoadLink(Cell(1, 2), FACING_SW)),
	/* 37 */ DirtRoadTile(DirtRoadLink(Cell(3, 0), FACING_NE), DirtRoadLink(Cell(1, 2), FACING_SW)),
	/* 38 */ DirtRoadTile(DirtRoadLink(Cell(3, 0), FACING_NE), DirtRoadLink(Cell(1, 2), FACING_SW)),
	/* 39 */ DirtRoadTile(DirtRoadLink(Cell(2, 0), FACING_NE), DirtRoadLink(Cell(1, 1), FACING_SW)),
	/* 40 */ DirtRoadTile(DirtRoadLink(Cell(1, 0), FACING_NE), DirtRoadLink(Cell(1, 0), FACING_SW)),
	/* 41 */ DirtRoadTile(DirtRoadLink(Cell(1, 0), FACING_NE), DirtRoadLink(Cell(1, 0), FACING_SW)),
	/* 42 */ DirtRoadTile(DirtRoadLink(Cell(1, 2), FACING_SW)),
	/* 43 */ DirtRoadTile(DirtRoadLink(Cell(1, 1), FACING_SW)),
	/* 44 */ DirtRoadTile(DirtRoadLink(Cell(1, 1), FACING_SW)),
	/* 45 */ DirtRoadTile(DirtRoadLink(Cell(3, 0), FACING_NE)),
	/* 46 */ DirtRoadTile(DirtRoadLink(Cell(2, 0), FACING_NE)),
	/* 47 */ DirtRoadTile(DirtRoadLink(Cell(2, 0), FACING_NE)),
	/* 48 */ DirtRoadTile(DirtRoadLink(Cell(4, 0), FACING_NE), DirtRoadLink(Cell(1, 1), FACING_SW)),
	/* 49 */ DirtRoadTile(DirtRoadLink(Cell(2, 0), FACING_NE), DirtRoadLink(Cell(1, 3), FACING_SW)),
	/* 50 */ DirtRoadTile(),
	/* 51 */ DirtRoadTile(DirtRoadLink(Cell(0, 0), FACING_W), DirtRoadLink(Cell(3, 0), FACING_E)),
	/* 52 */ DirtRoadTile(DirtRoadLink(Cell(0, 0), FACING_W), DirtRoadLink(Cell(3, 0), FACING_E)),
	/* 53 */ DirtRoadTile(DirtRoadLink(Cell(0, 0), FACING_W), DirtRoadLink(Cell(3, 0), FACING_E)),
	/* 54 */ DirtRoadTile(DirtRoadLink(Cell(0, 0), FACING_W), DirtRoadLink(Cell(3, 0), FACING_E)),
	/* 55 */ DirtRoadTile(DirtRoadLink(Cell(0, 0), FACING_W), DirtRoadLink(Cell(2, 0), FACING_E)),
	/* 56 */ DirtRoadTile(DirtRoadLink(Cell(0, 0), FACING_W), DirtRoadLink(Cell(1, 0), FACING_E)),
	/* 57 */ DirtRoadTile(DirtRoadLink(Cell(0, 0), FACING_W), DirtRoadLink(Cell(0, 0), FACING_E)),
	/* 58 */ DirtRoadTile(DirtRoadLink(Cell(0, 0), FACING_W), DirtRoadLink(Cell(0, 0), FACING_E)),
	/* 59 */ DirtRoadTile(DirtRoadLink(Cell(0, 0), FACING_W)),
	/* 60 */ DirtRoadTile(DirtRoadLink(Cell(0, 0), FACING_W)),
	/* 61 */ DirtRoadTile(DirtRoadLink(Cell(0, 1), FACING_W)),
	/* 62 */ DirtRoadTile(DirtRoadLink(Cell(2, 0), FACING_E)),
	/* 63 */ DirtRoadTile(DirtRoadLink(Cell(1, 0), FACING_E)),
	/* 64 */ DirtRoadTile(DirtRoadLink(Cell(1, 0), FACING_E)),
	/* 65 */ DirtRoadTile(DirtRoadLink(Cell(0, 0), FACING_W), DirtRoadLink(Cell(3, 1), FACING_E)),
	/* 66 */ DirtRoadTile(DirtRoadLink(Cell(0, 1), FACING_W), DirtRoadLink(Cell(3, 0), FACING_E)),
	/* 67 */ DirtRoadTile(),
	/* 68 */ DirtRoadTile(DirtRoadLink(Cell(1, 1), FACING_NW), DirtRoadLink(Cell(3, 3), FACING_SE)),
	/* 69 */ DirtRoadTile(DirtRoadLink(Cell(1, 1), FACING_NW), DirtRoadLink(Cell(3, 3), FACING_SE)),
	/* 70 */ DirtRoadTile(DirtRoadLink(Cell(1, 1), FACING_NW), DirtRoadLink(Cell(3, 3), FACING_SE)),
	/* 71 */ DirtRoadTile(DirtRoadLink(Cell(1, 1), FACING_NW), DirtRoadLink(Cell(3, 3), FACING_SE)),
	/* 72 */ DirtRoadTile(DirtRoadLink(Cell(1, 1), FACING_NW), DirtRoadLink(Cell(2, 2), FACING_SE)),
	/* 73 */ DirtRoadTile(DirtRoadLink(Cell(1, 1), FACING_NW), DirtRoadLink(Cell(1, 1), FACING_SE)),
	/* 74 */ DirtRoadTile(DirtRoadLink(Cell(1, 1), FACING_NW), DirtRoadLink(Cell(1, 1), FACING_SE)),
	/* 75 */ DirtRoadTile(DirtRoadLink(Cell(1, 1), FACING_NW)),
	/* 76 */ DirtRoadTile(DirtRoadLink(Cell(1, 1), FACING_NW)),
	/* 77 */ DirtRoadTile(DirtRoadLink(Cell(1, 1), FACING_NW)),
	/* 78 */ DirtRoadTile(DirtRoadLink(Cell(3, 3), FACING_SE)),
	/* 79 */ DirtRoadTile(DirtRoadLink(Cell(1, 2), FACING_SE)),
	/* 80 */ DirtRoadTile(DirtRoadLink(Cell(2, 2), FACING_SE)),
	/* 81 */ DirtRoadTile(DirtRoadLink(Cell(1, 1), FACING_NW), DirtRoadLink(Cell(4, 2), FACING_SE)),
	/* 82 */ DirtRoadTile(DirtRoadLink(Cell(1, 1), FACING_NW), DirtRoadLink(Cell(2, 4), FACING_SE)),
	/* 83 */ DirtRoadTile(),
	/* 84 */ DirtRoadTile(DirtRoadLink(Cell(0, 0), FACING_N), DirtRoadLink(Cell(0, 3), FACING_S)),
	/* 85 */ DirtRoadTile(DirtRoadLink(Cell(0, 0), FACING_N), DirtRoadLink(Cell(0, 3), FACING_S)),
	/* 86 */ DirtRoadTile(DirtRoadLink(Cell(0, 0), FACING_N), DirtRoadLink(Cell(0, 3), FACING_S)),
	/* 87 */ DirtRoadTile(DirtRoadLink(Cell(0, 0), FACING_N), DirtRoadLink(Cell(0, 3), FACING_S)),
	/* 88 */ DirtRoadTile(DirtRoadLink(Cell(0, 0), FACING_N), DirtRoadLink(Cell(0, 2), FACING_S)),
	/* 89 */ DirtRoadTile(DirtRoadLink(Cell(0, 0), FACING_N), DirtRoadLink(Cell(0, 1), FACING_S)),
	/* 90 */ DirtRoadTile(DirtRoadLink(Cell(0, 0), FACING_N), DirtRoadLink(Cell(0, 0), FACING_S)),
	/* 91 */ DirtRoadTile(DirtRoadLink(Cell(0, 0), FACING_N), DirtRoadLink(Cell(0, 0), FACING_S)),
	/* 92 */ DirtRoadTile(DirtRoadLink(Cell(0, 3), FACING_S)),
	/* 93 */ DirtRoadTile(DirtRoadLink(Cell(0, 2), FACING_S)),
	/* 94 */ DirtRoadTile(DirtRoadLink(Cell(0, 2), FACING_S)),
	/* 95 */ DirtRoadTile(DirtRoadLink(Cell(0, 0), FACING_N)),
	/* 96 */ DirtRoadTile(DirtRoadLink(Cell(0, 0), FACING_N)),
	/* 97 */ DirtRoadTile(DirtRoadLink(Cell(0, 0), FACING_N)),
	/* 98 */ DirtRoadTile(DirtRoadLink(Cell(1, 0), FACING_N), DirtRoadLink(Cell(0, 3), FACING_S)),
	/* 99 */ DirtRoadTile(DirtRoadLink(Cell(0, 0), FACING_N), DirtRoadLink(Cell(1, 3), FACING_S)),
	/* 100 */ DirtRoadTile(),
	/* 101 */ DirtRoadTile(DirtRoadLink(Cell(0, 0), FACING_W), DirtRoadLink(Cell(0, 0), FACING_E)),
	/* 102 */ DirtRoadTile(DirtRoadLink(Cell(0, 0), FACING_W), DirtRoadLink(Cell(0, 0), FACING_E)),
	/* 103 */ DirtRoadTile(DirtRoadLink(Cell(0, 0), FACING_N), DirtRoadLink(Cell(0, 0), FACING_S)),
	/* 104 */ DirtRoadTile(DirtRoadLink(Cell(0, 0), FACING_N), DirtRoadLink(Cell(0, 0), FACING_S)),
	/* 105 */ DirtRoadTile(DirtRoadLink(Cell(0, 0), FACING_W), DirtRoadLink(Cell(0, 0), FACING_E)),
	/* 106 */ DirtRoadTile(DirtRoadLink(Cell(0, 0), FACING_W), DirtRoadLink(Cell(0, 0), FACING_E)),
	/* 107 */ DirtRoadTile(DirtRoadLink(Cell(0, 0), FACING_N), DirtRoadLink(Cell(0, 0), FACING_S)),
	/* 108 */ DirtRoadTile(DirtRoadLink(Cell(0, 0), FACING_N), DirtRoadLink(Cell(0, 0), FACING_S))
};


/// <summary>
/// Fetches the road library entry for a dirt road tile.
/// Use this routine to get from a tile already sitting on the map back to the entry that
/// describes where its roads connect. Anything that is not a dirt road lands on the first
/// entry.
/// </summary>
/// <param name="ittype">The isometric tile type to look up.</param>
/// <returns>Returns with the index of the matching dirt road entry.</returns>
int Dirt_Road_Tile_Index(IsometricTileType ittype)
{
	if (ittype >= IsometricTileTypeClass::DirtRoadCurve && ittype < IsometricTileTypeClass::DirtRoadCurve + 101) { // RA2: Dirt_Road_Tile_Index at 0x58D9A0 matches only so
		return(ittype - IsometricTileTypeClass::DirtRoadCurve);
	}
	if (ittype < IsometricTileTypeClass::DirtRoadSlopes || ittype >= IsometricTileTypeClass::DirtRoadSlopes + 8) { // RA2: as above
		return(0);
	}
	return(ittype - IsometricTileTypeClass::DirtRoadSlopes + 101); // RA2: as above
}


/// <summary>
/// Stamps every dirt road entry with its tile type.
/// The road library is written out as connection data alone, so nothing in it knows which
/// isometric tile it stands for until this routine runs. Call it before laying any dirt
/// road.
/// </summary>
void Init_Dirt_Roads(void)
{
	int i;
	for (i = 0; i < 101; i++) { // RA2: Init_Dirt_Roads at 0x58D9D0 matches only so
		DirtRoadTiles[i].ITType = IsometricTileType(IsometricTileTypeClass::DirtRoadCurve + i);
	}
	for (i = 0; i < FACING_COUNT; i++) { // RA2: as above
		DirtRoadTiles[i + 101].ITType = IsometricTileType(IsometricTileTypeClass::DirtRoadSlopes + i); // RA2: as above
	}
}














/// <summary>
/// Fetches the spread priority of a candidate forest cell.
/// Place_Forest plants in cheapest first order, so this score is what gives a wood its rounded
/// shape and its uneven treeline.
/// </summary>
/// <param name="cell1">The cell the forest is growing out from.</param>
/// <param name="cell2">The candidate cell being considered.</param>
/// <returns>Returns with the score, where a lower score means the cell is reached sooner.</returns>
double MapGeneratorClass::Get_Forest_Score(const Cell & cell1, const Cell & cell2) const
{
	static const double _random_range = 5.0;

	int x = cell1.X - cell2.X;
	int y = cell1.Y - cell2.Y;
	double dist = fastmath::sqrt(x * x + y * y);
	return(RMG_RANDOM_DOUBLE(_random_range) + dist);
}


/// <summary>
/// Fetches the spread priority of a candidate patch cell.
/// Place_Tile_Patch fills in cheapest first order, so this score is what keeps a patch roughly
/// round while leaving its edge ragged.
/// </summary>
/// <param name="cell1">The cell the patch is growing out from.</param>
/// <param name="cell2">The candidate cell being considered.</param>
/// <returns>Returns with the score, where a lower score means the cell is filled sooner.</returns>
double MapGeneratorClass::Get_Tile_Patch_Score(const Cell & cell1, const Cell & cell2) const
{
	static const double _random_range = 5.0;

	int x = cell1.X - cell2.X;
	int y = cell1.Y - cell2.Y;
	double dist = fastmath::sqrt(x * x + y * y);
	return(RMG_RANDOM_DOUBLE(_random_range) + dist);
}


/// <summary>
/// Sets up one of the map generation sliders.
/// This routine is used by Set_Settings to point a slider at the span of values its setting
/// is permitted to take. A setting with nothing left to choose between is shown disabled
/// rather than hidden, so the dialog keeps its shape.
/// </summary>
/// <param name="handle">The slider control to set up.</param>
/// <param name="min">The lowest value the slider may be dragged to.</param>
/// <param name="max">The highest value the slider may be dragged to.</param>
/// <param name="position">Where the thumb should sit.</param>
/// <param name="enable">Should the player be allowed to move this slider?</param>
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
void MapSeedClass::Set_Scroll_Bar(HWND handle, unsigned int min, unsigned int max, int position, bool enable)
{
	if (max <= min) {
		EnableWindow(handle, FALSE);
		Slider_SetRange(handle, 0, 100);
		Slider_SetPos(handle, position);
	} else {
		EnableWindow(handle, enable);
		Slider_SetRange(handle, min, max);
		Slider_SetPos(handle, position);
	}
}
#endif


/// <summary>
/// Sets up one of the map generation checkboxes.
/// This routine is the companion of Set_Scroll_Bar, and is used by Set_Settings to show a
/// setting the dialog offers as a simple yes or no. A setting the player is not allowed to
/// touch is shown disabled rather than hidden, so the dialog keeps its shape.
/// </summary>
/// <param name="handle">The checkbox control to set up.</param>
/// <param name="state">Should the box be shown checked?</param>
/// <param name="enable">Should the player be allowed to change this setting?</param>
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
void MapSeedClass::Set_Checkbox(HWND handle, bool state, bool enable)
{
	Button_SetCheck(handle, state != 0);
	Button_Enable(handle, enable);
}
#endif


/// <summary>
/// Forces a setting back within its legal range.
/// This routine is used to hold a single map generation setting to the limits imposed upon
/// it. A setting that is already legal is left exactly as it is.
/// </summary>
/// <param name="setting">The setting to police. It is only written when found out of range.</param>
/// <param name="min">The lowest legal value.</param>
/// <param name="max">The highest legal value.</param>
/// <param name="use_midpoint">Should an illegal value land in the middle of the range rather
/// than on the boundary it broke?</param>
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
void MapSeedClass::Clamp_Setting(int & setting, int min, int max, bool use_midpoint) const
{
	int value = setting;

	if (value > max) {
		if (use_midpoint) {
			value = (max + min) >> 1;
		} else {
			value = max;
		}
		setting = value;
	} else if (value < min) {
		if (use_midpoint) {
			value = (max + min) >> 1;
		} else {
			value = min;
		}
		setting = value;
	}
}
#endif


/// <summary>
/// Turns part of a water body into swamp.
/// This routine is used on a mutated map to spoil an otherwise respectable lake. The water is
/// soured out from a seed cell, the transitions along the new shore are tidied, and a little
/// foliage is dropped on top to finish the job.
/// </summary>
/// <param name="cells">The water cells the swamp may claim.</param>
/// <param name="last">The size of the water body, which decides how far the swamp may
/// spread.</param>
/// <param name="nodes">Scratch storage for the spread.</param>
/// <param name="queue">The queue that orders the spread.</param>
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
void MapGeneratorClass::Generate_Swamp(DynamicVectorClass<Cell> &cells, int last, CellNode *nodes, PriorityQueueClass<CellNode> *queue)
{
	Cell swamp_origin = CELL_NONE;
	int tries = 0;

	DebugString("Generating swamp\n");

	while (swamp_origin == CELL_NONE) {
		Cell c = cells[Pick_Random_UInt(0, cells.Count() - 1)];
		if (!Map[c].Is_Tile_Swamp()) {
			swamp_origin = c;
		}

		tries++;
		if (tries >= 200) {
			break;
		}
	}

	if (swamp_origin == CELL_NONE) {
		return;
	}

	DynamicVectorClass<Cell> swamp_cells;
	swamp_cells.Set_Growth_Step(500);

	int max_spread = MIN(last / 2, 200);
	int min_spread = MIN(last / 8, 50);

	int spread_count = Pick_Random_UInt(min_spread, max_spread);
	queue->Clear();

	int marker_index = 1;
	nodes[0].Element = swamp_origin;
	nodes[0].Score = 0;
	queue->Insert(nodes[0]);

	CellNode * node = queue->Extract_Min();
	int spread_step = 0;
	while (spread_step < spread_count && node != NULL) {
		CellClass * cptr = &Map[node->Element];
		cptr->ITType = IsometricTileTypeClass::SwampTile;
		cptr->SubTile = 0;
		swamp_cells.Add(cptr->CellID);

		CellNode * newnode = &nodes[marker_index];
		for (FacingType dir = FACING_FIRST; dir < FACING_COUNT; dir = FacingType(dir + FACING_90)) {
			Cell newcell = Adjacent_Cell(node->Element, dir);
			if (My_In_Radar(newcell)) {
				MapRegionClass::CellData & data = MapRegionClass::Get_Cell_Data(newcell);
				CellClass * newcellptr = &Map[newcell];
				if (newcellptr->ITType >= IsometricTileTypeClass::WaterSet && newcellptr->ITType < IsometricTileTypeClass::WaterSet + WATER_COUNT) {
					newnode->Element = newcell;
					newnode->Score = Get_Spread_Score(swamp_origin, newcell, spread_step);
					data.SpreadID = WorkingRegionID;
					marker_index++;
					queue->Insert(*newnode++);
				}
			}
		}

		spread_step++;
		node = queue->Extract_Min();
	}

	int i;
	for (i = 0; i < swamp_cells.Count(); i++) {
		Map[swamp_cells[i]].Fixup_LAT();
	}

	DynamicVectorClass<unsigned short> swamp_tiles;

	for (i = 0; i < SWAMP_COUNT - 1; i++) {
		swamp_tiles.Add((unsigned short)(IsometricTileTypeClass::SwampTile + i + 1));
	}

	int patch_count = Pick_Random_UInt(1, 8);
	for (i = 0; i < patch_count; i++) {
		int patch_index = Pick_Random_UInt(0, swamp_tiles.Count() - 1);
		IsometricTileTypeClass * itype = IsometricTileTypes[swamp_tiles[patch_index]];
		int width = itype->Width;
		int height = itype->Height;

		Cell patch_cell = CELL_NONE;
		int patch_tries = 0;
		while (patch_cell == CELL_NONE) {
			Cell base = swamp_cells[Pick_Random_UInt(0, swamp_cells.Count() - 1)];
			patch_cell = base;
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					if (Map[base + Cell(x, y)].ITType != IsometricTileTypeClass::SwampTile) {
						patch_cell = CELL_NONE;
					}
				}
			}

			patch_tries++;
			if (patch_tries >= 200) {
				break;
			}
		}

		if (patch_cell == CELL_NONE) {
			break;
		}

		for (int y = 0; y < height; y++) {
			char subtile = width * y;
			for (int x = 0; x < width; x++) {
				Cell c = patch_cell + Cell(x, y);
				Map[c].ITType = (IsometricTileType)swamp_tiles[patch_index];
				Map[c].SubTile = subtile + x;
			}
		}

		swamp_tiles.Delete_Index(patch_index);
	}

	int foliage_count = Pick_Random_UInt(0, 4);
	for (i = 0; i < foliage_count; i++) {
		Cell c = swamp_cells[Pick_Random_UInt(0, swamp_cells.Count() - 1)];
		if (Map[c].Cell_Terrain(false) == NULL) {
			char terrain_name[8];
			int terrain_index = Pick_Random_UInt(1, 5);
			sprintf(terrain_name, "FONA0%d", terrain_index);
			new TerrainClass(TerrainTypes[TerrainTypeClass::From_Name(terrain_name)], c);
		}
	}

	DebugString("Swamp heap size:?? -- Marker Index:%d\n", marker_index);
}
#endif


/// <summary>
/// Seeds a frozen lake on an arctic map.
/// This is the tundra counterpart of Seed_Lake. The sheet spreads out from a seed cell until the
/// map's water budget is spent and is then given its edge variants. A lake that cannot be grown
/// to a worthwhile size is abandoned and the ground put back the way it was found.
/// </summary>
/// <param name="cell">The cell to grow the lake from, or Cell(0,0) to let the routine choose
/// one.</param>
/// <returns>bool; Was a lake of usable size seeded?</returns>
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
bool MapGeneratorClass::Seed_Arctic_Lake(const Cell & cell)
{
	float ratios[BIOME_COUNT] = {
		0.40f,
		0.35f,
		0.35f,
		0.10f,
		0.30f,
	};

	int water_value;
	if (SeedData.WaterAmount == 0) {
		water_value = 0;
	} else {
		water_value = (int)(ratios[SeedData.Biome] * (double)LocalWidth * (double)LocalHeight * (double)SeedData.WaterAmount * 0.02 + 100.0);
	}

	int spread_limit = water_value - SeededWaterAmount;
	IsometricTileType ice1 = IsometricTileTypeClass::Ice1Set;
	if (spread_limit <= 0) {
		return(false);
	}

	CellNode *nodes = new CellNode[MAX((2 * spread_limit) + 2, 100)];
	PriorityQueueClass<CellNode> *queue = new PriorityQueueClass<CellNode>(MAX((2 * spread_limit) + 2, 100));

	Map.Reset_Iterator();
	CellClass *cptr = Map.Iterate();
	while (cptr != NULL) {
		Set_Cell_Data_Spread(cptr->Fetch_CellID(), 0);
		cptr = Map.Iterate();
	}

	Cell seed_cell;
	if (cell == Cell(0, 0)) {
		int tries = 0;
		while (true) {
			unsigned int x = Pick_Random_UInt(0, Map.PlayRect.Width - 1);
			unsigned int y = Pick_Random_UInt(0, Map.PlayRect.Height - 1);

			seed_cell = Cell(1, Map.PlayRect.Width) + Cell(x, -x) + Cell(y, y);

			tries++;
			if (tries >= 200) {
				return(false);
			}

			if (MapRegionClass::CellData::Get_Region(seed_cell) == 0 && Map[seed_cell].Is_Tile_Clear()) {
				if (tries < 200) {
					break;
				}
				return(false);
			}
		}
	} else {
		seed_cell = cell;
	}

	bool success = true;
	int seeded_count = 0;

	int max_spread = MAX(spread_limit, 76);
	double scale = (double)(spread_limit / 6);
	double mean = (double)(spread_limit / 4);

	int spread_count = (int)Sample_Truncated_Normal(mean, scale, 75.0, (double)max_spread);

	queue->Clear();

	int marker_index = 1;
	nodes[0].Element = seed_cell;
	nodes[0].Score = 0.0f;
	Set_Cell_Data_Spread(seed_cell, WorkingRegionID);
	queue->Insert(nodes[0]);

	CellNode *node = queue->Extract_Min();

	DynamicVectorClass<Cell> *ice_cells = new DynamicVectorClass<Cell>();
	ice_cells->Set_Growth_Step(spread_count + 1);

	while (seeded_count < spread_count && node != NULL) {
		Set_Cell_Data_Region(node->Element, WorkingRegionID);

		CellClass *node_cellptr = &Map[node->Element];
		node_cellptr->ITType = ice1;
		node_cellptr->SubTile = 0;
		node_cellptr->IsIceGrowthAllowed = true;
		ice_cells->Add(node_cellptr->Fetch_CellID());

		CellNode *newnode = &nodes[marker_index];
		for (FacingType dir = FACING_FIRST; dir < FACING_COUNT; dir = FacingType(dir + FACING_90)) {
			Cell newcell = Adjacent_Cell(node->Element, dir);

			if (My_In_Radar(newcell)) {

				if (RMGCellData != NULL) {
					MapRegionClass::CellData &data = MapRegionClass::Get_Cell_Data(newcell);
					if (!data.RegionID && data.SpreadID != WorkingRegionID) {
						CellClass *newcellptr = &Map[newcell];
						if (newcellptr->Is_Tile_Clear()) {
							newnode->Element = newcell;
							newnode->Score = Get_Spread_Score(seed_cell, newcell, seeded_count);
							Set_Cell_Data_Spread(newcell, WorkingRegionID);
							marker_index++;
							queue->Insert(*newnode++);
						}
					}
				}
			}
		}

		seeded_count++;
		node = queue->Extract_Min();
	}

	CellNode *remaining = queue->Extract_Min();
	while (remaining != NULL) {
		if (!success) {
			break;
		}

		MapRegionClass::CellData &data = MapRegionClass::Get_Cell_Data(remaining->Element);
		if (data.RegionID || !Map[remaining->Element].Is_Tile_Clear()) {
			success = false;
		} else {
			CellClass *cellptr = &Map[remaining->Element];
			cellptr->ITType = IsometricTileTypeClass::Ice1Set;
			cellptr->SubTile = 0;
			cellptr->IsIceGrowthAllowed = true;
			data.RegionID = WorkingRegionID;
			//ice_cells->Add(cellptr->Fetch_CellID());
			ice_cells->Add(cellptr->CellID);
		}

		remaining = queue->Extract_Min();
		seeded_count++;
	}

	Seed_Ice(*ice_cells, (IsometricTileType)(IsometricTileTypeClass::Ice1Set + ICE_CRACKED));
	Seed_Ice(*ice_cells, (IsometricTileType)(IsometricTileTypeClass::Ice1Set + ICE_EDGE));

	delete queue;
	delete[] nodes;
	delete ice_cells;

	if (success) {
		SeededWaterAmount += seeded_count;
		return(true);
	}

	Clear_Region_Cells(WorkingRegionID, 0, -1, ISOTILE_CLEAR); // RA2: the fourth argument
	return(false);
}
#endif


/// <summary>
/// Scatters an edge variant over a sheet of ice.
/// This routine is run over an ice sheet once it has been laid down, dabbing blotches of one of
/// the ice variants here and there so that the sheet does not read as one flat expanse. It is
/// called once per variant.
/// </summary>
/// <param name="cells">The ice cells that may be painted.</param>
/// <param name="last">The ice variant to paint with.</param>
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
void MapGeneratorClass::Seed_Ice(DynamicVectorClass<Cell> & cells, IsometricTileType last)
{
	double rand_scale = last == IsometricTileTypeClass::Ice1Set + ICE_CRACKED ? ICE_CRACKED - 1 : 1;

	int seed_count = Pick_Random_UInt(0, 15);

	CellNode * nodes = new CellNode[MAX(cells.Count() * 2, 64)];
	PriorityQueueClass<CellNode> * queue = new PriorityQueueClass<CellNode>(MAX(cells.Count() * 2, 64));

	for (int i = cells.Count() - 1; i >= 0; i--) {
		MapRegionClass::Get_Cell_Data(cells[i]).Iced = false;
	}

	while (seed_count > 0) {
		int spread_step = 0;
		int spread_limit = MAX(4, cells.Count() / 20);
		int spread_count = Pick_Random_UInt(3, spread_limit);
		queue->Clear();

		int seed_index = Pick_Random_UInt(0, cells.Count() - 1);
		int node_count = 1;
		Cell seed_cell = cells[seed_index];
		nodes[0].Element = seed_cell;
		nodes[0].Score = 0.0f;

		MapRegionClass::Get_Cell_Data(seed_cell).Iced = true;
		queue->Insert(nodes[0]);

		CellNode * node = queue->Extract_Min();
		while (spread_step < spread_count && node != NULL) {
			Map[node->Element].ITType = (IsometricTileType)last;

			CellNode * newnode = &nodes[node_count];
			for (FacingType dir = FACING_FIRST; dir < FACING_COUNT; dir = FacingType(dir + FACING_90)) {
				Cell newcell = Adjacent_Cell(node->Element, dir);
				if (My_In_Radar(newcell)) {
					CellClass * newcellptr = &Map[newcell];
					MapRegionClass::CellData & data = MapRegionClass::Get_Cell_Data(newcell);
					if (newcellptr->Is_Tile_Ice() && !data.Iced) {
						newnode->Element = newcell;
						newnode->Score = Get_Ice_Score(seed_cell, newcell, rand_scale);
						data.Iced = true;
						node_count++;
						queue->Insert(*newnode++);
					}
				}
			}

			spread_step++;
			node = queue->Extract_Min();
		}

		seed_count--;
	}

	delete[] nodes;

	delete queue;
}
#endif


/// <summary>
/// Scores a candidate cell for a spreading patch of ice.
/// This is the ice counterpart of Get_Spread_Score. Ground near the seed is claimed first, and
/// the weight given to the random term decides how ragged the edge of the patch turns out.
/// </summary>
/// <param name="cell1">The cell the patch is spreading from.</param>
/// <param name="cell2">The candidate cell being scored.</param>
/// <param name="random_range">How much randomness to allow into the score.</param>
/// <returns>Returns with the candidate's priority. Lower is claimed first.</returns>
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
double MapGeneratorClass::Get_Ice_Score(const Cell & cell1, const Cell & cell2, double random_range) const
{
	int x = cell1.X - cell2.X;
	int y = cell1.Y - cell2.Y;
	double dist = fastmath::sqrt(x * x + y * y);
	return(RMG_RANDOM_DOUBLE(random_range) + dist);
}
#endif


/// <summary>
/// Draws a wandering "river of ice" polyline of Ice1Set tiles across the map.
/// Each step perturbs the heading and length, occasionally recurses to spawn a
/// branch, and finally scatters ice-edge variants via Seed_Ice. This is the
/// arctic sibling of Seed_River.
/// </summary>
/// <param name="cell">Seed cell, or Cell(0, 0) to pick a random edge seed.</param>
/// <param name="start_angle">Initial heading angle in radians (used when cell is given).</param>
/// <returns>True if enough ice was painted, false otherwise.</returns>
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
bool MapGeneratorClass::Seed_Arctic_River(const Cell & cell, double start_angle)
{
	bool success = true;

	Cell seed_cell;
	double angle;

	if (cell == Cell(0, 0)) {
		Cell corner(1, Map.PlayRect.Width);
		Cell far_corner = corner + Cell(Map.PlayRect.Width, -Map.PlayRect.Width) + Cell(Map.PlayRect.Height, Map.PlayRect.Height) + Cell(-2, 0);

		int dir4 = (int)Pick_Random_UInt(0, 3);
		int x = (int)Pick_Random_UInt(0, Map.PlayRect.Width - 1);
		int y = (int)Pick_Random_UInt(0, Map.PlayRect.Height - 1);

		Cell candidates[4];
		candidates[0] = corner + Cell(x, -x);
		candidates[1] = far_corner - Cell(y, y);
		candidates[2] = far_corner - Cell(x, -x);
		candidates[3] = corner + Cell(y, y);

		double base_angle = DEG_TO_RAD(315) - (double)dir4 * M_PI_2;

		seed_cell = candidates[dir4];
		angle = Sample_Truncated_Normal(base_angle, DEG_TO_RAD(60),
				base_angle - M_PI_2, base_angle + M_PI_2);
	} else {
		seed_cell = cell;
		angle = start_angle;
	}

	if (!My_In_Radar(seed_cell)) {
		return(false);
	}

	int seeded_count = 0;

	double length_d = (double)SeedData.WaterAmount * 0.09;
	if (4.0 > length_d) {
		length_d = 4.0;
	}
	int length_top = (int)length_d;
	double length = (double)Pick_Random_UInt(3, length_top);
	int half = (int)length / 2;

	double cur_angle = angle;
	double length_hi = length + (double)half;
	double length_lo = length - (double)half;
	double cur_x = (double)seed_cell.X + 0.5;
	double cur_y = (double)seed_cell.Y + 0.5;
	double angle_hi = angle + M_PI_2;
	double angle_lo = angle - M_PI_2;

	double stop_roll = 1.0;

	DynamicVectorClass<Cell> *ice_cells = new DynamicVectorClass<Cell>();
	ice_cells->Set_Growth_Step(5000);

	while (My_In_Radar(Cell((int)cur_x, (int)cur_y)) && success) {

		double step_y = fastmath::cos(cur_angle);
		double step_x = fastmath::sin(cur_angle);

		int span = (int)(length + 0.5);
		double px = cur_x - step_x * (double)(span - 1) * 0.5;
		double py = cur_y - step_y * (double)(span - 1) * 0.5;

		for (int i = span; i > 0; i--) {
			if (My_In_Radar(Cell((int)px, (int)py))) {
				if (RMGCellData != NULL) {
					MapRegionClass::Get_Cell_Data(Cell((int)px, (int)py)).RegionID = WorkingRegionID;
				}

				CellClass *cellptr = &Map[Cell((int)px, (int)py)];
				Cell id = cellptr->CellID;
				cellptr->ITType = IsometricTileTypeClass::Ice1Set;
				cellptr->SubTile = 0;
				ice_cells->Add(id);
			}

			px += step_x;
			py += step_y;
		}

		cur_x += step_y;
		cur_y -= step_x;

		if (Random_Fraction() < 0.01) {
			double branch_base = cur_angle + M_PI_2;
			double branch_angle = Sample_Truncated_Normal(branch_base, DEG_TO_RAD(30),
					cur_angle + DEG_TO_RAD(30), cur_angle + DEG_TO_RAD(150));

			Cell branch_cell((int)px, (int)py);
			success = Seed_Arctic_River(branch_cell, branch_angle);
		}

		double delta_angle = Sample_Truncated_Normal(0.0, DEG_TO_RAD(18),
				angle_lo - cur_angle, angle_hi - cur_angle);
		cur_angle += delta_angle;

		if (half > 0) {
			double delta_len = Sample_Truncated_Normal(0.0, 0.5,
					length_lo - length, length_hi - length);
			length += delta_len;
		}

		stop_roll = Random_Fraction();
		seeded_count++;
		if (stop_roll < 0.005) {
			break;
		}
	}

	if (seeded_count < 40) {
		success = false;
	}

	if (stop_roll < 0.005) {
		if (My_In_Radar(Cell((int)cur_x, (int)cur_y)) && success) {
			Seed_Arctic_Lake(Cell((int)cur_x, (int)cur_y));
		}
	}

	if (success) {
		Seed_Ice(*ice_cells, (IsometricTileType)(IsometricTileTypeClass::Ice1Set + ICE_CRACKED));
		Seed_Ice(*ice_cells, (IsometricTileType)(IsometricTileTypeClass::Ice1Set + ICE_EDGE));
	}

	delete ice_cells;

	if (success) {
		SeededWaterAmount += seeded_count;
		return(true);
	}

	Clear_Region_Cells(WorkingRegionID, 0, -1, ISOTILE_CLEAR); // RA2: the fourth argument
	return(false);
}
#endif


/// <summary>
/// Grows one patch of blue mold on a mutated map.
/// This routine hunts for an unobstructed spot, spreads mold outward from it, then blends the
/// patch into the surrounding ground and dresses it with a little foliage. If nowhere suitable
/// turns up it does nothing rather than force a patch into a bad place.
/// </summary>
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
void MapGeneratorClass::Generate_Mold(void)
{
	Cell mold_origin = CELL_NONE;
	int tries = 0;

	DebugString("Generating mold\n");

	while (mold_origin == CELL_NONE) {
		Cell c = MapRegionClass::Pick_Random_Clear_Map_Cell();
		if (Is_Cell_Area_Clear(Rect(c.X - 1, c.Y - 1, 3, 3), true)) {
			mold_origin = c;
		}
		if (++tries >= 200) {
			break;
		}
	}

	if (mold_origin == CELL_NONE) {
		return;
	}

	DynamicVectorClass<Cell> cells;
	cells.Set_Growth_Step(500);

	int spread_count = Pick_Random_UInt(30, 150);
	int heap_size = MAX(100, 6 * spread_count);

	CellNode * nodes = new CellNode[heap_size];
	PriorityQueueClass<CellNode> * queue = new PriorityQueueClass<CellNode>(heap_size);
	queue->Clear();

	int marker_index = 1;
	nodes[0].Element = mold_origin;
	nodes[0].Score = 0;
	queue->Insert(nodes[0]);

	CellNode * node = queue->Extract_Min();
	int spread_step = 0;
	while (spread_step < spread_count && node != NULL) {
		CellClass * cptr = &Map[node->Element];
		cptr->ITType = IsometricTileTypeClass::BlueMoldTile;
		cptr->SubTile = 0;
		cells.Add(cptr->CellID);

		CellNode * newnode = &nodes[marker_index];
		for (FacingType dir = FACING_FIRST; dir < FACING_COUNT; dir = FacingType(dir + FACING_90)) {
			Cell newcell = Adjacent_Cell(node->Element, dir);
			if (My_In_Radar(newcell)) {
				MapRegionClass::CellData & data = MapRegionClass::Get_Cell_Data(newcell);

				bool can_spread = true;
				for (FacingType dir2 = FACING_FIRST; dir2 < FACING_COUNT; dir2 = FacingType(dir2 + FACING_90)) {
					Cell adj = Adjacent_Cell(newcell, dir2);
					CellClass * adjptr = &Map[adj];
					if (!adjptr->Is_Tile_Clear() && adjptr->ITType != IsometricTileTypeClass::BlueMoldTile) {
						can_spread = false;
						break;
					}
				}

				if (can_spread && !data.Inviolate && marker_index < heap_size) {
					newnode->Element = newcell;
					newnode->Score = Get_Spread_Score(mold_origin, newcell, spread_step);
					data.SpreadID = WorkingRegionID;
					marker_index++;
					queue->Insert(*newnode++);
				}
			}
		}

		spread_step++;
		node = queue->Extract_Min();
	}

	int i;
	for (i = 0; i < cells.Count(); i++) {
		Map[cells[i]].Fixup_LAT();
	}

	int foliage_count = Pick_Random_UInt(0, 4);
	for (i = 0; i < foliage_count; i++) {
		Cell c = CELL_NONE;
		int local_tries = 0;
		while (c == CELL_NONE) {
			c = cells[Pick_Random_UInt(0, cells.Count() - 1)];
			if (Map[c].ITType != IsometricTileTypeClass::BlueMoldTile) {
				c = CELL_NONE;
			}
			if (++local_tries >= 200) {
				break;
			}
		}

		if (c == CELL_NONE) {
			break;
		}

		if (Map[c].Cell_Terrain(false) == NULL && Map[c].Overlay == OVERLAY_NONE) {
			if (Random_Fraction() < 0.75) {
				char terrain_name[8];
				int terrain_index = Pick_Random_UInt(1, 5);
				sprintf(terrain_name, "FONA0%d", terrain_index);
				new TerrainClass(TerrainTypes[TerrainTypeClass::From_Name(terrain_name)], c);
			} else {
				Map[c].Overlay = OVERLAY_LARGE_TIBERIUM01;
				Map[c].OverlayData = 0;
			}
		}
	}

	DebugString("Mold heap size: %d -- Marker Index: %d\n", heap_size, marker_index);

	delete queue;
	delete[] nodes;
}
#endif


/// <summary>
/// Grows one crystal deposit on a mutated map.
/// The seed may be open ground or a cliff face; on a cliff the routine hangs a crystal
/// formation off the rock and spreads out from the foot of it. The finished patch is blended
/// into its surroundings and dressed with a little foliage.
/// </summary>
/// <param name="cell">The cell to seed the deposit at.</param>
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
void MapGeneratorClass::Generate_Crystals(const Cell & cell)
{
	bool on_cliff = false;

	Cell cliff_seed = CELL_NONE;
	Cell crystal_origin;

	DebugString("Generating crystal\n");

	if (!Map[cell].Is_Tile_Clear()) {
		on_cliff = true;

		int cliff = Map[cell].ITType - IsometricTileTypeClass::CliffSet;
		if (cliff >= 4 && cliff <= 6) {
			if (Is_Cell_Area_Clear(Rect(cell.X, cell.Y - 1, 2, 1), false) && Pick_Random_UInt(0, 1) == 0) {
				Place_Tile((IsometricTileType)(IsometricTileTypeClass::CrystalCliff), cell, -1, Map[Cell(cell.X, cell.Y + 1)].Height + 4);
				crystal_origin = Cell(cell.X, cell.Y - 1);
				cliff_seed = Cell(cell.X + 1, cell.Y - 1);
			} else {
				if (!Is_Cell_Area_Clear(Rect(cell.X, cell.Y + 2, 2, 1), false)) {
					return;
				}
				Place_Tile((IsometricTileType)(IsometricTileTypeClass::CrystalCliff + 1), cell, -1, Map[Cell(cell.X, cell.Y + 1)].Height + 4);
				crystal_origin = Cell(cell.X, cell.Y + 2);
				cliff_seed = Cell(cell.X + 1, cell.Y + 2);
			}
		} else if (cliff >= 14 && cliff <= 16) {
			if (Is_Cell_Area_Clear(Rect(cell.X - 1, cell.Y, 1, 2), false) && Pick_Random_UInt(0, 1) == 0) {
				Place_Tile((IsometricTileType)(IsometricTileTypeClass::CrystalCliff + 4), cell, -1, Map[Cell(cell.X + 1, cell.Y)].Height + 4);
				crystal_origin = Cell(cell.X - 1, cell.Y);
				cliff_seed = Cell(cell.X - 1, cell.Y + 1);
			} else {
				if (!Is_Cell_Area_Clear(Rect(cell.X + 2, cell.Y, 1, 2), false)) {
					return;
				}
				Place_Tile((IsometricTileType)(IsometricTileTypeClass::CrystalCliff + 5), cell, -1, Map[Cell(cell.X + 1, cell.Y)].Height + 4);
				crystal_origin = Cell(cell.X + 2, cell.Y);
				cliff_seed = Cell(cell.X + 2, cell.Y + 1);
			}
		} else {
			return;
		}
	} else {
		crystal_origin = cell;
	}

	if (crystal_origin == CELL_NONE) {
		return;
	}

	DynamicVectorClass<Cell> cells;
	cells.Set_Growth_Step(500);
	int spread_count = Pick_Random_UInt(30, 150);
	int heap_size = 6 * spread_count;
	if (heap_size < 100) {
		heap_size = 100;
	}

	CellNode *nodes = new CellNode[heap_size];
	PriorityQueueClass<CellNode> *queue = new PriorityQueueClass<CellNode>(heap_size);
	queue->Clear();

	int marker_index = 0;
	if (on_cliff) {
		nodes[0].Element = cliff_seed;
		nodes[0].Score = 0;
		marker_index = 1;
		queue->Insert(nodes[0]);
	}

	CellNode *seed = &nodes[marker_index++];
	seed->Element = crystal_origin;
	seed->Score = 0;
	queue->Insert(*seed);

	CellNode *node = queue->Extract_Min();
	int spread_step = 0;
	while (spread_step < spread_count) {
		if (node == NULL) {
			break;
		}
		CellClass *cptr = &Map[node->Element];
		cptr->ITType = IsometricTileTypeClass::CrystalTile;
		cptr->SubTile = 0;
		cells.Add(cptr->CellID);

		CellNode *newnode = &nodes[marker_index];
		for (FacingType dir = FACING_FIRST; dir < FACING_COUNT; dir = FacingType(dir + FACING_90)) {
			Cell newcell = Adjacent_Cell(node->Element, dir);
			if (My_In_Radar(newcell)) {
				MapRegionClass::CellData & data = MapRegionClass::Get_Cell_Data(newcell);
				if (Map[newcell].Is_Tile_Clear() && Map[newcell].Overlay == OVERLAY_NONE && marker_index < heap_size) {
					newnode->Element = newcell;
					newnode->Score = Get_Spread_Score(crystal_origin, newcell, spread_step);
					data.SpreadID = WorkingRegionID;
					marker_index++;
					queue->Insert(*newnode++);
				}
			}
		}

		spread_step++;
		node = queue->Extract_Min();
	}

	int i;
	for (i = 0; i < cells.Count(); i++) {
		Map[cells[i]].Fixup_LAT();
	}

	int foliage_count = Pick_Random_UInt(0, 5);
	for (i = 0; i < foliage_count; i++) {
		Cell c = CELL_NONE;
		int local_tries = 0;
		while (c == CELL_NONE) {
			c = cells[Pick_Random_UInt(0, cells.Count() - 1)];
			if (Map[c].ITType != IsometricTileTypeClass::CrystalTile || MapRegionClass::Get_Cell_Data(c).Inviolate) {
				c = CELL_NONE;
			}
			if (++local_tries >= 200) {
				break;
			}
		}

		if (c == CELL_NONE) {
			break;
		}

		if (Map[c].Cell_Terrain(false) == NULL) {
			char terrain_name[8];
			int terrain_index = Pick_Random_UInt(6, 15);
			sprintf(terrain_name, "FONA%d%d", terrain_index / 10, terrain_index % 10);
			new TerrainClass(TerrainTypes[TerrainTypeClass::From_Name(terrain_name)], c);
		}
	}

	DebugString("Mold heap size: %d -- Marker Index: %d\n", heap_size, marker_index);

	delete queue;
	delete[] nodes;
}
#endif


/// <summary>
/// Determines if an area of the map is clear ground.
/// This routine is used before dropping down something that needs elbow room -- a mold seed,
/// or the ground beside a crystal cliff face. Any part of the area lying off the map counts
/// against it.
/// </summary>
/// <param name="rect">The area of cells to examine.</param>
/// <param name="check_data">Should cells the generator has protected be rejected too?</param>
/// <returns>bool; Is the whole area clear?</returns>
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
bool MapGeneratorClass::Is_Cell_Area_Clear(Rect const & rect, bool check_data)
{
	if (My_In_Radar(Cell(rect.X, rect.Y)) &&
		My_In_Radar(Cell(rect.X + rect.Width - 1, rect.Y)) &&
		My_In_Radar(Cell(rect.X, rect.Y + rect.Height - 1)) &&
		My_In_Radar(Cell(rect.X + rect.Width - 1,rect.Y + rect.Height - 1))) {
		for (int y = rect.Y; y < rect.Y + rect.Height; y++) {
			for (int x = rect.X; x < rect.X + rect.Width; x++) {
				CellClass *cptr = &Map[Cell(x,y)];
				if (!cptr->Is_Tile_Clear()) {
					return(false);
				}
				if (check_data && !MapRegionClass::CellData::Is_Not_Inviolate(cptr->CellID)) {
					return(false);
				}
				if (cptr->Overlay != OVERLAY_NONE) {
					return(false);
				}
			}
		}
		return(true);
	}
	return(false);
}
#endif


/// <summary>
/// Blends the edges of the ice laid down on the map.
/// This routine runs once the ice is in place, so that every sheet of it meets the ground
/// around it with the proper transition tiles rather than a hard edge.
/// </summary>
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
void MapGeneratorClass::Smooth_Ice(void)
{
	int frst = IsometricTileTypeClass::Ice1Set;
	int last = IsometricTileTypeClass::Ice3Set + ICE_COUNT-1;
	Map.Reset_Iterator();
	CellClass * cptr = Map.Iterate();
	while (cptr != NULL) {
		int ittype = cptr->ITType;
		if (ittype >= frst && ittype <= last) {
			Cell cell = cptr->CellID;
			Map.Smoothen_Ice(cell, true);
		}
		cptr = Map.Iterate();
	}
}
#endif


/// <summary>
/// Plants the veinhole monsters the map calls for.
/// This routine looks for flat open ground with room for a monster and the patch of veins
/// it starts out with. A crowded map simply gets fewer monsters rather than holding the
/// generator up.
/// </summary>
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
void MapGeneratorClass::Generate_Veinholes(void)
{
	int tries = 0;
	int placed = 0;

	int save_init = ScenarioInit; // RA2: no VeinholeMonsters, count (TibSun's use does not compile against RA2's declaration)
	ScenarioInit = 0;


	ScenarioInit = save_init; // RA2: no count (TibSun's use does not compile against RA2's declaration)
}
#endif

