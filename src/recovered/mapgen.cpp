// mapgen.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/mapgen.json carries the addresses and the rows that
// stay in their own files.


// Not exact (RA2_NON_MATCHING) prelude: header switches the drafts below need (guard-on compile only)
#ifdef RA2_NON_MATCHING
#define RA2_VECTOR_SIGNED_SIZE
#define TYPELIST_IMPLEMENT_DEFAULT_CONSTRUCTOR
#endif

#include "shared/unknown_map_generator.hpp"
#include "shared/map_gaussian.hpp"
#include "shared/map_random.hpp"
#include "shared/map_region.hpp"
#include "shared/house.hpp"
#include "shared/mouse_class.hpp"
#include "shared/isometric_tile_type.hpp"
#include "shared/map_view.hpp"
#include "shared/cell.hpp"
#include "shared/random2.hpp"
#include "shared/coord.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/crt_math.hpp"
#include "shared/tileset_index.hpp"
#include "shared/building.hpp"
#include "shared/id_source.hpp"
#include "shared/fast_math.hpp"
#include "shared/house_type.hpp"
#include "shared/rules.hpp"
#include "shared/map_preview.hpp"
#include "shared/dialog_create.hpp"
#include "shared/raw_file_class.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/map_seed.hpp"
#include "shared/win32_find.hpp"
#include "shared/options_dialog.hpp"
#include "shared/wdt_territory.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/win32_api.hpp"
#include "shared/mouse.hpp"
#include "shared/keyboard.hpp"
#include "shared/progress_screen.hpp"
#include "shared/string_table.hpp"
#include "shared/crt_memory.hpp"
#include "shared/owner_draw_dialog_control.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/anim_type.hpp"
#define SHAPE_FILE_EMPTY_RECT unknown_static_rect
#include "shared/reference_counted.hpp"
#include "shared/world_domination_tour.hpp"
#include "shared/session_state.hpp"
#include "shared/map.hpp"
#include "shared/radar.hpp"
#define RA2_VEIN_HEAP_REMOVE_MIN_IMPLEMENTATION
#include "shared/tiberium.hpp"
#include "shared/debug_printf.hpp"
#include "shared/ccini_class.hpp"
#include "shared/tactical.hpp"
#include "shared/unit.hpp"
#include "shared/infantry.hpp"
#include "shared/terrain.hpp"
#include "shared/terrain_type.hpp"
#include "shared/overlay_type.hpp"
#include "shared/building_type_class.hpp"
#include "shared/smudge_type.hpp"
#include "shared/isometric_tile.hpp"
#include "shared/vector_class.hpp"
#include "shared/random.hpp"
#include "shared/vein_heap_core.hpp"
#include "shared/map_cell_view.hpp"
#include "shared/cell_class.hpp"
#include "shared/rect.hpp"
#include "shared/com_persist.hpp"
#include "shared/vein_heap.hpp"
#include "shared/building_type_list.hpp"
#include "shared/infantry_type.hpp"
#include "shared/adjacent_cell.hpp"
#include "shared/dir_type.hpp"
#include "shared/map_generation_link.hpp"
#include "shared/map_generation_path.hpp"
#include "shared/event.hpp"
#include "shared/facing.hpp"


// The region-generation unit's globals (definition order = dynamic initializer order): the map generator
// 0x00A6FD68 (initializer 0x0056D4C0, atexit destructor 0x0056D4E0), the generation random generator 0x00A705C0
// (Random2Class(0), initializer 0x0056D4F0) and the generator's Gaussian source 0x00A6FD50 over
// Map_Random_Fraction_00579770 (initializer 0x0056D500), then the region list MapRegionClass::Regions 0x00A6FD28
// (initializer 0x0056D530, atexit destructor 0x0056D570 with guard bit 0x00A6FD40).
double __cdecl Map_Random_Fraction_00579770();

MapGeneratorClass RandomMapGen;
Random2Class unknown_map_random_a705c0;
MapGaussianClass unknown_map_gaussian_a6fd50(Map_Random_Fraction_00579770);
DynamicVectorClass<MapRegionClass *> MapRegionClass::Regions;


// Compiler-generated atexit destructor (0x0056D4E0, _$E) of RandomMapGen, defined in the previous
// draft (0x0056D4C0); no source of its own.


// Dynamic initializer (0x0056D4F0, _$E) of unknown_map_random_a705c0 (Random2Class(0)), defined in the draft at
// 0x0056D4C0; no source of its own.


// Dynamic initializer (0x0056D500, _$E) of unknown_map_gaussian_a6fd50 (MapGaussianClass constructor inline),
// defined in the draft at 0x0056D4C0; no source of its own.


// Dynamic initializer of MapRegionClass::Regions (0x0056D530, _$E) emitted by
// RandomMapGen_init_0056d4c0.cpp; no source of its own.


// Guarded atexit destructor of MapRegionClass::Regions (0x0056D570, _$E) emitted by
// RandomMapGen_init_0056d4c0.cpp; no source of its own.


// Category meanings and fastcall/bool source spelling remain hypotheses.
bool __fastcall Generation_Category_Matches_0056d680(int category, int value)
{
    switch (category) {
    case 0: return value == 3;
    case 1:
    case 3:
    case 4: return value != 3;
    case 2: return true;
    default: return false;
    }
}


// Six qword fields initialized to floating zero; constructor/type intent is a hypothesis.
UnknownMapGenerationCell::UnknownMapGenerationCell()
    : Position(0,0), unknown_08(0.0), unknown_10(0.0), unknown_18(0.0),
      unknown_20(0.0), unknown_28(0.0), unknown_30(0.0), RegionIndex(0),
      unknown_3c(0), unknown_40(-1), Marked(0)
{
    unknown_45[0] = 0;
    unknown_45[1] = 0;
    unknown_45[2] = 0;
    unknown_45[3] = 0;
    unknown_45[4] = 0;
    unknown_45[5] = 1;
    unknown_4b = 0;
}


extern DynamicVectorClass<MapRegionClass *> unknown_map_regions_a6fd28;
MapRegionClass::MapRegionClass(Cell origin)
    : unknown_00(0), NeighborRegionIndices(0), Index(unknown_map_region_next_index_a70a28),
      CellCount(0), unknown_10(Map[origin].unknown_11b), unknown_14(Map[origin].unknown_47b7e0()),
      Origin(origin), unknown_1a(false), unknown_1b(true), unknown_1c(0), unknown_20(0),
      unknown_24(false), Cells(), Bounds(0,0,0,0)
{
    Cells.Set_Growth_Step(100000);
    unknown_map_region_next_index_a70a28 = (int)((unsigned)unknown_map_region_next_index_a70a28 + 1u);
    unknown_map_regions_a6fd28.Add(this);
}


// Physical result is the raw AL byte, not a normalized bool.
unsigned char __fastcall Grid_Cell_Mark_0056daf0(Cell const &position)
{
    int index = (int)((unsigned)(int)position.Y * (unsigned)unknown_map_cell_side_84e804 + (unsigned)(int)position.X);
    return unknown_map_cells_a70a24[index].Marked;
}


// Physical ECX points to packed signed-WORD Cell; source signature/name are hypotheses.
void __fastcall Set_Grid_Visit_Byte_0056db10(Cell const &position)
{
    int index = (int)((unsigned)(int)position.Y * (unsigned)unknown_map_cell_side_84e804 + (unsigned)(int)position.X);
    unknown_map_cells_a70a24[index].unknown_45[0] = 1;
}


// Original repeatedly clears the SAME side*side indexed byte; no corrected traversal is inferred.
void Unknown_Grid_Reset_0056db70()
{
    int count = (int)((unsigned)unknown_map_cell_side_84e804 * (unsigned)unknown_map_cell_side_84e804);
    int index = count;
    while (count > 0) {
        unknown_map_cells_a70a24[index].unknown_45[0] = 0;
        --count;
    }
}


void MapRegionClass::Count_Grid_Cells_0056dbd0()
{
    CellCount = 0;
    int count = unknown_map_cell_side_84e804 * unknown_map_cell_side_84e804;
    for (int i = 0; i < count; i++) {
        if (unknown_map_cells_a70a24[i].RegionIndex == Index && unknown_map_cells_a70a24[i].Position != Cell(0, 0)) {
            CellCount++;
        }
    }
}


// unknown_generation_cell_clear_0056dac0 (0x0056DAC0): defined above in this unit; its definition is the row.


// Get_Generation_Cell_0056dba0 (0x0056DBA0): defined above in this unit; its definition is the row.


// Cell MapRegionClass::Pick_Random_Region_Cell_0056dc20() (0x0056DC20): a random generation-grid cell that
// belongs to this region and is not the (0,0) placeholder. The inner loop is Map_Random_Range_005797a0(0, count - 1)
// (0x005797A0, Map_Random_Fraction_00579770 inline) as written in the region-generation unit.

Cell MapRegionClass::Pick_Random_Region_Cell_0056dc20()
{
    int count = unknown_map_cell_side_84e804 * unknown_map_cell_side_84e804;
    unsigned int index;
    do {
        unsigned int maximum = count - 1;
        do {
            index = (unsigned int)((unsigned int)unknown_map_random_a705c0() * (1.0 / 4294967295.0) * (unsigned int)(maximum + 1) + (double)0);
        } while (index > maximum);
    } while (unknown_map_cells_a70a24[index].RegionIndex != Index
        || (unknown_map_cells_a70a24[index].Position.X == 0 && unknown_map_cells_a70a24[index].Position.Y == 0));
    return unknown_map_cells_a70a24[index].Position;
}


// Cell MapRegionClass::Pick_Random_Clear_Region_Cell_0056dcb0() (0x0056DCB0): up to 200 random cells of this
// region (Pick_Random_Region_Cell_0056dc20, inline) until the map cell passes unknown_47b590; else (0,0).

Cell MapRegionClass::Pick_Random_Clear_Region_Cell_0056dcb0()
{
    int tries = 0;
    for (;;) {
        Cell position = Pick_Random_Region_Cell_0056dc20();
        if (++tries > 200) {
            return Cell(0, 0);
        }
        if (Map[position].unknown_47b590()) {
            return position;
        }
    }
}


// Cell MapRegionClass::Pick_Random_Marked_Cell_0056dda0() (0x0056DDA0, RET4 hidden result): up to 99 random
// generation cells of this region that are not (0,0), are Marked and whose map cell passes unknown_47b590; else
// (0,0). Each pick is copied by value over the first generation cell (as 0x0056DEA0).

Cell MapRegionClass::Pick_Random_Marked_Cell_0056dda0()
{
    UnknownMapGenerationCell *cell = unknown_map_cells_a70a24;
    int count = unknown_map_cell_side_84e804 * unknown_map_cell_side_84e804;
    int tries = 0;
    for (;;) {
        unsigned int maximum = count - 1;
        unsigned int index;
        do {
            index = (unsigned int)((unsigned int)unknown_map_random_a705c0() * (1.0 / 4294967295.0) * (unsigned int)(maximum + 1) + (double)0);
        } while (index > maximum);
        *cell = unknown_map_cells_a70a24[index];
        if (++tries >= 100) {
            break;
        }
        if (cell->RegionIndex != Index) {
            continue;
        }
        if (cell->Position.X == 0 && cell->Position.Y == 0) {
            continue;
        }
        if (!cell->Marked) {
            continue;
        }
        if (Map[cell->Position].unknown_47b590()) {
            return cell->Position;
        }
    }
    return Cell(0, 0);
}


// Cell __fastcall MapRegionClass::Pick_Random_Zone_Cell_0056dea0(int zone, int region) (0x0056DEA0, static; ECX hidden
// result, RET4): up to 99 random generation cells whose map cell is in movement zone zone (Map.Zone(.., 0, false)),
// passes unknown_47b590 and belongs to region (any when -1); else (0,0). Each pick is copied by value over the
// first generation cell (the original writes through the table pointer read at entry).

Cell __fastcall MapRegionClass::Pick_Random_Zone_Cell_0056dea0(int zone, int region)
{
    UnknownMapGenerationCell *cell = unknown_map_cells_a70a24;
    int count = unknown_map_cell_side_84e804 * unknown_map_cell_side_84e804;
    int tries = 0;
    for (;;) {
        unsigned int maximum = count - 1;
        unsigned int index;
        do {
            index = (unsigned int)((unsigned int)unknown_map_random_a705c0() * (1.0 / 4294967295.0) * (unsigned int)(maximum + 1) + (double)0);
        } while (index > maximum);
        *cell = unknown_map_cells_a70a24[index];
        if (++tries >= 100) {
            break;
        }
        CellClass &mapcell = Map[cell->Position];
        if (cell->Position.X == 0 && cell->Position.Y == 0) {
            continue;
        }
        if (Map.Zone(mapcell.MapCoords, 0, false) != zone) {
            continue;
        }
        if (!mapcell.unknown_47b590()) {
            continue;
        }
        if (region == -1 || cell->RegionIndex == region) {
            return cell->Position;
        }
    }
    return Cell(0, 0);
}


// void MapRegionClass::Clear_Cell_Marks_0056dfc0() (0x0056DFC0, static): sets every generation cell's +0x3C
// field to -1.

void MapRegionClass::Clear_Cell_Marks_0056dfc0()
{
    int count = unknown_map_cell_side_84e804 * unknown_map_cell_side_84e804;
    for (int i = 0; i < count; i++) {
        unknown_map_cells_a70a24[i].unknown_3c = -1;
    }
}


// double __stdcall Unknown_Cell_Score_0056dff0(Cell const &from, Cell const &to, double direction) (0x0056DFF0):
// a random-weighted score for the step from -> to: 0.15 * distance + 1.5 * the angle between the step's direction
// (atan of -dy/dx, +pi for negative dx, pi/2 when dx is 0) and direction (folded into [0, pi]) + 2.5 * a random
// fraction. The three weights are double constants in .rdata (0x007A5BE0-0x007A5BF0), not __real literals.
extern const double unknown_weight_random_7a5be0;      // 0x007A5BE0 2.5
extern const double unknown_weight_distance_7a5be8;    // 0x007A5BE8 0.15
extern const double unknown_weight_angle_7a5bf0;       // 0x007A5BF0 1.5

double __stdcall Unknown_Cell_Score_0056dff0(Cell const &from, Cell const &to, double direction)
{
    int dx = to.X - from.X;
    int dy = to.Y - from.Y;
    double distance = unknown_sqrt_4ba410((double)(dy * dy + dx * dx));
    double angle = dx ? unknown_math_4ba5b0(-((double)dy / (double)dx)) : 1.5707963267948966;
    if (dx < 0) {
        angle += 3.141592653589793;
    }
    angle = fabs(angle - direction);
    while (angle >= 6.283185307179586) {
        angle -= 6.283185307179586;
    }
    if (angle > 3.141592653589793) {
        angle = 6.283185307179586 - angle;
    }
    return unknown_weight_distance_7a5be8 * distance + unknown_weight_angle_7a5bf0 * angle + ((unsigned int)unknown_map_random_a705c0() * (1.0 / 4294967295.0)) * unknown_weight_random_7a5be0;
}


// MapRegionClass *__fastcall MapRegionClass::Create_Region_From_Cell_0056e100(CellClass *cell_ptr) (0x0056E100, RET0): floods
// out from a cell over neighbours of its height and water character into a new region. A dry patch under 75 cells is
// merged into a neighbouring region instead (returns 0). TibSun twin: mapgen.cpp MapRegionClass::Create_Region_From_Cell.

static inline Cell Region_Adjacent_Cell(Cell const &cell, int facing)
{
    return cell + unknown_adjacent_851a70[facing & 7];
}

static inline bool Flood_In_Bounds(Cell const &cell)
{
    int x = cell.X;
    int y = cell.Y;
    if (y + x > unknown_map_min_a70a1c && x - y < unknown_map_min_a70a1c &&
        y - x < unknown_map_min_a70a1c && y + x <= unknown_map_max_a70a20) {
        return true;
    }
    return false;
}

static inline void Set_Spread(Cell const &cell, int spread)
{
    MapRegionClass::Get_Cell_Data(cell).unknown_3c = spread;
}

static inline void Set_Region(Cell const &cell, int region)
{
    if (unknown_map_cells_a70a24 != 0) {
        UnknownMapGenerationCell &data = MapRegionClass::Get_Cell_Data(cell);
        data.RegionIndex = region;
    }
}

static inline unsigned Pick_Random_UInt(unsigned minimum, unsigned maximum)
{
    unsigned value;
    do {
        value = (unsigned)((unsigned)unknown_map_random_a705c0() * (1.0 / 4294967295.0) * (unsigned)(maximum + 1) + (double)minimum);
    } while (value > maximum);
    return value;
}

static inline Cell Offset_Cell(Cell cell, int dx, int dy)
{
    return Cell(cell.X + dx, cell.Y + dy);
}

static inline int Region_Of(Cell const &cell)
{
    if (unknown_map_cells_a70a24 == 0) {
        return -1;
    }
    UnknownMapGenerationCell &data = MapRegionClass::Get_Cell_Data(cell);
    return data.RegionIndex;
}

MapRegionClass *__fastcall MapRegionClass::Create_Region_From_Cell_0056e100(CellClass *cell_ptr)
{
    int i;
    int region_id = unknown_map_region_next_index_a70a28;

    DynamicVectorClass<Cell> cells;
    cells.Set_Growth_Step(50000);
    cells.Add(cell_ptr->Fetch_CellID());

    bool has_water = cell_ptr->unknown_47b7e0() || cell_ptr->unknown_47b9c0();
    int cell_height = cell_ptr->unknown_11b;
    int processed_count = 0;

    while (cells.Count()) {
        Cell cell = cells[cells.ActiveCount - 1];
        cells.Delete(cells.ActiveCount - 1);

        Set_Spread(cell, region_id);
        Set_Region(cell, region_id);

        Cell ncell;
        for (i = 0; i < 8; i++) {
            ncell = Region_Adjacent_Cell(cell, i);
            UnknownMapGenerationCell &ndata = MapRegionClass::Get_Cell_Data(ncell);
            if (Flood_In_Bounds(ncell)) {
                if (ndata.RegionIndex == -1 && ndata.unknown_3c != region_id) {
                    ndata.unknown_3c = region_id;
                    if (Map[ncell].unknown_11b == cell_height) {
                        int n_has_water = Map[ncell].unknown_47b7e0() || Map[ncell].unknown_47b9c0();
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
            DynamicVectorClass<Cell> *region_cells = RandomMapGen.Build_Region_Border_Cell_List(0);
            Cell region_cell = (*region_cells)[Pick_Random_UInt(0, region_cells->Count() - 1)];

            Cell ncell;
            for (i = 0; i < 8; i++) {
                ncell = Region_Adjacent_Cell(region_cell, i);
                if (Flood_In_Bounds(ncell)) {
                    if (Map[ncell].unknown_11b != Map[region_cell].unknown_11b && !Map[ncell].unknown_47b7e0()) {
                        int height = Map[ncell].unknown_11b;

                        RandomMapGen.Clear_Region_Cells(0, -1, height, 0);

                        reset_done = true;
                        break;
                    }
                }
            }

            delete region_cells;
        } else {
            Cell side_cell;
            side_cell = Offset_Cell(cell_ptr->Fetch_CellID(), -1, 0);
            if (!Flood_In_Bounds(side_cell)) {
                side_cell = Offset_Cell(cell_ptr->Fetch_CellID(), 0, -1);
            }

            if (Flood_In_Bounds(side_cell)) {
                int id = Region_Of(side_cell);
                int height = Map[side_cell].unknown_11b;

                RandomMapGen.Clear_Region_Cells(region_id, id, height, 0);

                reset_done = true;
            }
        }

        if (reset_done) {
            return 0;
        }
    }

    MapRegionClass *region = new MapRegionClass(cell_ptr->Fetch_CellID());
    region->unknown_10 = cell_height;
    region->unknown_14 = has_water;
    region->CellCount = 0;
    region->Count_Grid_Cells_0056dbd0();
    return region;
}


// double __stdcall unknown_gaussian_value(double mean, double deviation) -- 0x005798D0
// Purpose: a normally distributed value: the shared normal-variate
// generator at 0x00A6FD50 (0x00579830, Box-Muller with a cached spare)
// scaled by `deviation` and offset by `mean`. Random map generator area
// (after MapSeedClass); names are hypotheses.

class UnknownGaussianSource_005798d0 {
public:
    double Next();

    bool HasSpare;       // +0x00
    double Spare;        // +0x08
};

extern UnknownGaussianSource_005798d0 unknown_gaussian_a6fd50;

double __stdcall unknown_gaussian_value_005798d0(double mean, double deviation)
{
    return unknown_gaussian_a6fd50.Next() * deviation + mean;
}


// void MapRegionClass::Destroy_Cell_Regions() (0x0056E790): clears every generation cell's region links
// (RegionIndex, +0x3C = -1) and deletes the regions newest first (each ~MapRegionClass removes itself from
// unknown_map_regions_a6fd28), then resets the next region index. Defined here out of line; the generator's
// destructor 0x005771B0 and Init_Regions in Generate_Random_Map 0x0057A0D0 expand it inline.
extern DynamicVectorClass<MapRegionClass *> unknown_map_regions_a6fd28;
void MapRegionClass::Destroy_Cell_Regions()
{
    int index;
    if(unknown_map_cells_a70a24) {
        int size=(int)((unsigned)unknown_map_cell_side_84e804*(unsigned)unknown_map_cell_side_84e804);
        for(index=0;index<size;++index) {
            unknown_map_cells_a70a24[index].RegionIndex=-1;
            unknown_map_cells_a70a24[index].unknown_3c=-1;
        }
    }
    for(index=(int)((unsigned)unknown_map_regions_a6fd28.Count()-1u);index>=0;--index) {
        MapRegionClass *region=unknown_map_regions_a6fd28[index];
        if(region)delete region;
    }
    unknown_map_region_next_index_a70a28=0;
 }


// void MapRegionClass::Make_Remaining_Regions_0056e890() (0x0056E890, static): creates a region for every real
// generation cell still without one (RegionIndex -1) whose map cell passes unknown_47b7e0 or unknown_47b9c0.

void MapRegionClass::Make_Remaining_Regions_0056e890()
{
    int count = unknown_map_cell_side_84e804 * unknown_map_cell_side_84e804;
    for (int i = 0; i < count; i++) {
        Cell position = unknown_map_cells_a70a24[i].Position;
        CellClass &cell = Map[position];
        if (unknown_map_cells_a70a24[i].RegionIndex == -1
            && (unknown_map_cells_a70a24[i].Position.X != 0 || unknown_map_cells_a70a24[i].Position.Y != 0)
            && (cell.unknown_47b7e0() || cell.unknown_47b9c0())) {
            Create_Region_From_Cell_0056e100(&cell);
        }
    }
}


// void MapRegionClass::Set_Unassigned_Regions_0056e910(bool flag) (0x0056E910, static __fastcall): creates a
// region for every real generation cell that has none yet (RegionIndex -1) and sets each new region's +0x1A flag.

void __fastcall MapRegionClass::Set_Unassigned_Regions_0056e910(bool flag)
{
    int count = unknown_map_cell_side_84e804 * unknown_map_cell_side_84e804;
    for (int i = 0; i < count; i++) {
        UnknownMapGenerationCell &cell = unknown_map_cells_a70a24[i];
        if (cell.RegionIndex == -1 && (cell.Position.X != 0 || cell.Position.Y != 0)) {
            MapRegionClass *region = Create_Region_From_Cell_0056e100(&Map[cell.Position]);
            if (region) {
                region->unknown_1a = flag;
            }
        }
    }
}


// 0056E970: assigns a map-generation grid cell to a region; does nothing without the grid
// (TibSun MapRegionClass::CellData::Set_Region). Physical contract: ECX -> signed16 X/Y,
// EDX -> raw32 stored at grid+38, RET0. Defined in the map generator unit so that its
// callers there may inline it (/Ob2); 33 ramp calls stay out of line.
void __fastcall Set_Cell_Region_Index_0056e970(Cell const &position, int index)
{
    if (unknown_map_cells_a70a24) {
        UnknownMapGenerationCell &data = MapRegionClass::Get_Cell_Data(position);
        data.RegionIndex = index;
    }
}


// void MapRegionClass::Reassign_Cells_0056ea20(int region, int value) (0x0056EA20): moves every generation cell of
// this region to region (clearing its +0x4B byte), clears the map cell's IsoTileIndex and +0x11A byte, sets its
// +0x11B byte to value unless value is -1, and empties this region's cell count.

void MapRegionClass::Reassign_Cells_0056ea20(int region, int value)
{
    int count = unknown_map_cell_side_84e804 * unknown_map_cell_side_84e804;
    for (int i = 0; i < count; i++) {
        if (unknown_map_cells_a70a24[i].RegionIndex == Index) {
            unknown_map_cells_a70a24[i].RegionIndex = region;
            unknown_map_cells_a70a24[i].unknown_4b = 0;
            Cell position = unknown_map_cells_a70a24[i].Position;
            CellClass &cell = Map[position];
            cell.IsoTileIndex = 0;
            cell.unknown_11a = 0;
            if (value != -1) {
                cell.unknown_11b = (char)value;
            }
        }
    }
    CellCount = 0;
}


// void MapRegionClass::unknown_0056eac0(int count, int mark) (0x0056EAC0, RET8): sets RegionIndex to mark
// on count rings of generation cells, starting with this region's boundary cells and growing each ring by the
// 8-neighbours of this region not yet in a ring (+0x3C holds the ring number; cleared before and after when
// count > 1).

void MapRegionClass::unknown_0056eac0(int count, int mark)
{
    DynamicVectorClass<Cell> *ring = Collect_Boundary_Cells_0056ed10();
    if (count > 1) {
        int total = unknown_map_cell_side_84e804 * unknown_map_cell_side_84e804;
        for (int i = 0; i < total; i++) {
            unknown_map_cells_a70a24[i].unknown_3c = -1;
        }
    }
    for (int step = 0; step < count; step++) {
        int index;
        for (index = ring->Count() - 1; index >= 0; index--) {
            Cell cell = (*ring)[index];
            if (unknown_map_cells_a70a24) {
                unknown_map_cells_a70a24[cell.Y * unknown_map_cell_side_84e804 + cell.X].RegionIndex = mark;
            }
        }
        if (step < count - 1) {
            DynamicVectorClass<Cell> *next = new DynamicVectorClass<Cell>;
            next->Set_Growth_Step(ring->Count());
            for (index = ring->Count() - 1; index >= 0; index--) {
                Cell cell = (*ring)[index];
                for (int direction = 0; direction < 8; direction++) {
                    Cell const &offset = unknown_adjacent_851a70[direction & 7];
                    Cell adjacent = cell + offset;
                    if (adjacent.Y + adjacent.X > unknown_map_min_a70a1c && adjacent.X - adjacent.Y < unknown_map_min_a70a1c &&
                        adjacent.Y - adjacent.X < unknown_map_min_a70a1c && adjacent.Y + adjacent.X <= unknown_map_max_a70a20) {
                        UnknownMapGenerationCell &data = unknown_map_cells_a70a24[adjacent.Y * unknown_map_cell_side_84e804 + adjacent.X];
                        if (data.RegionIndex == Index && data.unknown_3c != step + 1) {
                            next->Add(adjacent);
                            data.unknown_3c = step + 1;
                        }
                    }
                }
            }
            delete ring;
            ring = next;
        }
    }
    delete ring;
    if (count > 1) {
        int total = unknown_map_cell_side_84e804 * unknown_map_cell_side_84e804;
        for (int i = 0; i < total; i++) {
            unknown_map_cells_a70a24[i].unknown_3c = -1;
        }
    }
}


// DynamicVectorClass<Cell> *MapRegionClass::Collect_Boundary_Cells_0056ed10() (0x0056ED10): a new list of the
// cells of this region (in map walk order) with at least one in-bounds 8-neighbour of another region.

// The 8-neighbour bounds test of the generation diamond, written as an inline helper.
static inline bool In_Generation_Bounds(Cell const &next)
{
    int x = next.X;
    int y = next.Y;
    return y + x > unknown_map_min_a70a1c && x - y < unknown_map_min_a70a1c &&
        y - x < unknown_map_min_a70a1c && y + x <= unknown_map_max_a70a20;
}

DynamicVectorClass<Cell> *MapRegionClass::Collect_Boundary_Cells_0056ed10()
{
    DynamicVectorClass<Cell> *cells = new DynamicVectorClass<Cell>;
    Map.Reset_Iterator();
    for (CellClass *cell = Map.Iterate(); cell != 0; cell = Map.Iterate()) {
        Cell position = cell->MapCoords;
        if (UnknownMapGenerationCell::Get_Region(position) != Index) {
            continue;
        }
        bool edge = false;
        for (int direction = 0; direction < 8; direction++) {
            CellClass &adjacent = cell->Adjacent_Cell(direction);
            Cell next = adjacent.MapCoords;
            if (In_Generation_Bounds(next)) {
                if (UnknownMapGenerationCell::Get_Region(next) != Index) {
                    edge = true;
                }
            }
        }
        if (edge) {
            Cell coords = cell->MapCoords;
            cells->Add(coords);
        }
    }
    return cells;
}


// void MapRegionClass::Increase_Height_0056eea0(char height) (0x0056EEA0, RET4): adds height to the map cell
// height of every cell owned by this region.

void MapRegionClass::Increase_Height_0056eea0(char height)
{
    Map.Reset_Iterator();
    CellClass *cell = Map.Iterate();
    while (cell != 0) {
        int region = UnknownMapGenerationCell::Get_Region(cell->Fetch_CellID());
        if (region == Index) {
            cell->unknown_11b += height;
        }
        cell = Map.Iterate();
    }
}


// DynamicVectorClass<Cell> *MapRegionClass::Collect_Region_Boundary_0056fed0() (0x0056FED0): a new list of the
// cells in this region's own cell list with at least one in-bounds 8-neighbour of another region.

static inline bool In_Region_Bounds(Cell const &cell)
{
    int x = cell.X;
    int y = cell.Y;
    return y + x > unknown_map_min_a70a1c && x - y < unknown_map_min_a70a1c &&
        y - x < unknown_map_min_a70a1c && y + x <= unknown_map_max_a70a20;
}

DynamicVectorClass<Cell> *MapRegionClass::Collect_Region_Boundary_0056fed0()
{
    DynamicVectorClass<Cell> *cells = new DynamicVectorClass<Cell>;
    cells->Set_Growth_Step(10 > CellCount / 2 ? 10 : CellCount / 2);

    for (int i = Cells.Count() - 1; i >= 0; i--) {
        Cell cell = Cells[i];
        bool add = false;
        for (int facing = 0; facing < 8; facing++) {
            Cell ncell = cell + unknown_adjacent_851a70[facing & 7];
            if (In_Region_Bounds(ncell)) {
                if (UnknownMapGenerationCell::Get_Region(ncell) != Index) {
                    add = true;
                    break;
                }
            }
        }
        if (add) {
            cells->Add(cell);
        }
    }
    return cells;
}


bool __fastcall Generation_Cell_In_Bounds_0058da10(Cell const &position);
bool MapRegionClass::Grow_Border_00570040(int steps)
{
    DynamicVectorClass<Cell> *border=Collect_Boundary_Cells_0056ed10();
    for(int step=0;step<steps;++step) {
        DynamicVectorClass<Cell> *next=new DynamicVectorClass<Cell>;
        next->Set_Growth_Step(3*border->Count());
        for(int index=0;index<border->Count();++index) {
            for(int facing=0;facing<8;++facing) {
                Cell position=CellClass::Adjacent_Cell((*border)[index],facing);
                if(Generation_Cell_In_Bounds_0058da10(position)) {
                    if(Map[position].unknown_11b==unknown_10) {
                        UnknownMapGenerationCell &data=MapRegionClass::Get_Cell_Data(position);
                        int region=data.RegionIndex;
                        if(region==-1 && (Map[position].unknown_47b590() || Map[position].unknown_47b9c0())) {
                            next->Add(position);
                            data.RegionIndex=Index;
                            Map[position].unknown_11b=(char)unknown_10;
                        } else if(region!=Index) {
                            delete border;
                            delete next;
                            return false;
                        }
                    }
                }
            }
        }
        delete border;
        border=next;
    }
    delete border;
    return true;
}


bool __fastcall Generation_Cell_In_Bounds_0058da10(Cell const &position);
bool MapRegionClass::Expand_Cells_005702b0()
{
    int index;
    DynamicVectorClass<Cell> *border=Collect_Boundary_Cells_0056ed10();
    DynamicVectorClass<Cell> open;
    open.Set_Growth_Step(50000);
    for(index=0;index<border->Count();++index)open.Add((*border)[index]);
    delete border;
    while(open.Count()>0) {
        index=open.Count()-1;
        Cell position=open[index];
        open.Delete(index);
        for(int facing=0;facing<8;++facing) {
            Cell adjacent=CellClass::Adjacent_Cell(position,facing);
            if(Generation_Cell_In_Bounds_0058da10(adjacent)) {
                UnknownMapGenerationCell &data=unknown_map_cells_a70a24[adjacent.Y*unknown_map_cell_side_84e804+adjacent.X];
                if(data.RegionIndex==-1 && data.unknown_4b) {
                    data.RegionIndex=Index;
                    open.Add(adjacent);
                }
            }
        }
    }
    return true;
}


extern DynamicVectorClass<MapRegionClass *> unknown_map_regions_a6fd28;
bool __fastcall Generation_Cell_In_Bounds_0058da10(Cell const &position);
// Existing real destructor body, needed by the original inline delete site.
MapRegionClass::~MapRegionClass()
{
    if(unknown_00!=0) {
        delete static_cast<DynamicVectorClass<Cell> *>(unknown_00);
        unknown_00=0;
    }
    unknown_map_regions_a6fd28.Delete(this);
}
void MapRegionClass::Make_Ice_Regions_005704c0()
{
    int index;
    for(index=0;index<unknown_map_regions_a6fd28.Count();++index) {
        unknown_map_regions_a6fd28[index]->Cells.Clear();
        unknown_map_regions_a6fd28[index]->CellCount=0;
        unknown_map_regions_a6fd28[index]->Bounds=Rect(9999,9999,0,0);
    }
    int size=unknown_map_cell_side_84e804*unknown_map_cell_side_84e804;
    for(index=size-1;index>=0;--index) {
        UnknownMapGenerationCell &data=MapRegionClass::Get_Cell_Data(index);
        Cell position=data.Position;
        int y=position.Y;
        int x=position.X;
        if(Generation_Cell_In_Bounds_0058da10(position)) {
            int id=data.RegionIndex;
            if(id>=0 && id<unknown_map_regions_a6fd28.Count()) {
                ++unknown_map_regions_a6fd28[id]->CellCount;
                unknown_map_regions_a6fd28[id]->Cells.Add(position);
                Rect &rect=unknown_map_regions_a6fd28[id]->Bounds;
                if(!rect.Width) {
                    rect.X=x;
                    rect.Y=y;
                    rect.Width=1;
                    rect.Height=1;
                }
                int oldX=rect.X;
                if(x>=rect.X+rect.Width)rect.Width=x-oldX+1;
                if(x<oldX) {
                    int oldWidth=rect.Width;
                    rect.X=x;
                    rect.Width=oldX-x+oldWidth;
                }
                int oldY=rect.Y;
                if(y>=rect.Y+rect.Height)rect.Height=y-oldY+1;
                if(y<oldY) {
                    int oldHeight=rect.Height;
                    rect.Y=y;
                    rect.Height=oldY-y+oldHeight;
                }
            }
        }
    }
    double sizeLimit=(RandomMapGen.SeedData.RegionSize*0.005+0.05)*RandomMapGen.LocalWidth*RandomMapGen.LocalHeight;
    int limit=(int)(sizeLimit+sizeLimit);
    while(true) {
        bool restart=false;
        for(index=0;index<unknown_map_regions_a6fd28.Count();++index) {
            MapRegionClass *region=unknown_map_regions_a6fd28[index];
            if(!region->unknown_1a && !region->unknown_14) {
                if(region->CellCount>limit) {
                    if(region->Split_Region_0056ef20()) {
                        delete region;
                        restart=true;
                        break;
                    }
                } else region->unknown_1a=true;
            }
        }
        if(!restart)break;
    }
}


// TibSun Connect_Regions_With_Bridge: spans a low bridge across the gap between two regions.
bool __fastcall Can_Place_Paved_Road_00588a30(Rect const &area,bool overlap,bool end);
bool __fastcall Can_Place_Paved_Road_End_00588c20(Rect const &area,bool allowRoad);
void __fastcall Place_Tile_Footprint_005883f0(int tile,Cell const &position,int region=-1,int height=-1);
unsigned __fastcall Map_Random_Range_005797a0(unsigned,unsigned);
// Same-file form of the region cell pick (TibSun Pick_Random_Cell); this unit expands it.
inline Cell Pick_Region_Cell(MapRegionClass const *region)
{
    int count=unknown_map_cell_side_84e804*unknown_map_cell_side_84e804;
    unsigned index;
    do {
        index=Map_Random_Range_005797a0(0,count-1);
    } while(unknown_map_cells_a70a24[index].RegionIndex!=region->Index || (unsigned char)unknown_map_cells_a70a24[index].Position.Equal_004fef20(Cell(0,0)));
    return unknown_map_cells_a70a24[index].Position;
}

bool MapRegionClass::Connect_Regions_With_Bridge_00570bc0(MapRegionClass *region1,MapRegionClass *region2)
{
    int id1 = region1->Index;
    int id2 = region2->Index;

    int tries = 0;
    bool result = false;

    for (; tries < 200; tries++) {
        if (result) {
            break;
        }

        Cell cell = Pick_Region_Cell(this);

        Rect rect1(cell.X - 1, cell.Y, 3, 1);
        Rect rect2=rect1;
        Rect rect4(cell.X, cell.Y - 1, 1, 3);
        Rect rect3=rect4;

        bool allow_rect3 = true;
        bool allow_rect1 = true;

        while (!Can_Place_Paved_Road_00588a30(rect1, false, false)) {
            rect1.Y--;
            Cell cell1(rect1.X, rect1.Y);
            Cell cell2(rect1.X + 2, rect1.Y);
            CellClass * cptr1 = &Map[cell1];
            CellClass * cptr2 = &Map[cell2];
            if (!Map.Unknown_0055A660(cell1,true) || !Map.Unknown_0055A660(cell2,true) || cptr1->unknown_47b5e0() || cptr2->unknown_47b5e0()) {
                allow_rect1 = false;
                break;
            }
        }

        if (allow_rect1) {
            if (Can_Place_Paved_Road_00588a30(Rect(rect1.X, rect1.Y - 3, 3, 3), false, false)) {
                while (!Can_Place_Paved_Road_00588a30(rect2, false, false)) {
                    rect2.Y++;
                    Cell cell1(rect2.X, rect2.Y);
                    Cell cell2(rect2.X + 2, rect2.Y);
                    CellClass * cptr1 = &Map[cell1];
                    CellClass * cptr2 = &Map[cell2];
                    if (!Map.Unknown_0055A660(cell1,true) || !Map.Unknown_0055A660(cell2,true) || cptr1->unknown_47b5e0() || cptr2->unknown_47b5e0()) {
                        allow_rect1 = false;
                        break;
                    }
                }

                if (allow_rect1) {
                    if (!Can_Place_Paved_Road_00588a30(Rect(rect2.X, rect2.Y + 1, 3, 3), false, false)) {
                        allow_rect1 = false;
                    }
                }
            } else {
                allow_rect1 = false;
            }
        }

        while (!Can_Place_Paved_Road_00588a30(rect3, false, false)) {
            rect3.X--;
            Cell cell1(rect3.X, rect3.Y);
            Cell cell2(rect3.X, rect3.Y + 2);
            CellClass * cptr1 = &Map[cell1];
            CellClass * cptr2 = &Map[cell2];
            if (!Map.Unknown_0055A660(cell1,true) || !Map.Unknown_0055A660(cell2,true) || cptr1->unknown_47b5e0() || cptr2->unknown_47b5e0()) {
                allow_rect3 = false;
                break;
            }
        }

        if (allow_rect3) {
            if (Can_Place_Paved_Road_00588a30(Rect(rect3.X - 3, rect3.Y, 3, 3), false, false)) {
                while (!Can_Place_Paved_Road_00588a30(rect4, false, false)) {
                    rect4.X++;
                    Cell cell1(rect4.X, rect4.Y);
                    Cell cell2(rect4.X, rect4.Y + 2);
                    CellClass * cptr1 = &Map[cell1];
                    CellClass * cptr2 = &Map[cell2];
                    if (!Map.Unknown_0055A660(cell1,true) || !Map.Unknown_0055A660(cell2,true) || cptr1->unknown_47b5e0() || cptr2->unknown_47b5e0()) {
                        allow_rect3 = false;
                        break;
                    }
                }

                if (allow_rect3) {
                    if (!Can_Place_Paved_Road_00588a30(Rect(rect4.X + 1, rect4.Y, 3, 3), false, false)) {
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
            region_a = UnknownMapGenerationCell::Get_Region(Cell(rect1.X, rect1.Y));
            region_b = UnknownMapGenerationCell::Get_Region(Cell(rect2.X, rect2.Y));
        }

        if (allow_rect3) {
            len2 = abs(rect4.X - rect3.X);
            region_c = UnknownMapGenerationCell::Get_Region(Cell(rect4.X, rect4.Y));
            region_d = UnknownMapGenerationCell::Get_Region(Cell(rect3.X, rect3.Y));
        }

        if (allow_rect1) {
            if ((region_a != id1 || region_b != id2) && (region_a != id2 || region_b != id1)) {
                allow_rect1 = false;
            }
        }

        if (allow_rect3) {
            if ((region_c != id1 || region_d != id2) && (region_c != id2 || region_d != id1)) {
                allow_rect3 = false;
            }
            if (allow_rect3 && allow_rect1) {
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

        if (bridge.Is_Valid() && allow_rect3) {
            if (Is_Bridge_Allowed_00571bc0(bridge)) {
                for (int y = bridge.Y; y < bridge.Y + bridge.Height; y++) {
                    for (int x = bridge.X; x < bridge.X + bridge.Width; x++) {
                        if (x == bridge.X) {
                            Map[Cell(x, y)].OverlayIndex = 0x5e;
                        } else if (x == bridge.X + bridge.Width - 1) {
                            Map[Cell(x, y)].OverlayIndex = 0x5c;
                        } else {
                            Map[Cell(x, y)].OverlayIndex = 0x4a + (x % 4);
                        }
                        Map[Cell(x, y)].OverlayData = y - bridge.Y;
                    }
                }

                if (Can_Place_Paved_Road_End_00588c20(Rect(bridge.X + bridge.Width, bridge.Y - 2, 6, 6), false) && Map_Random_Range_005797a0(0,1)) {
                    Place_Tile_Footprint_005883f0(unknown_tileset_a6dd0c+10, Cell(bridge.X + bridge.Width, bridge.Y));
                } else {
                    Place_Tile_Footprint_005883f0(unknown_tileset_a6dd08, Cell(bridge.X + bridge.Width, bridge.Y));
                }

                if (Can_Place_Paved_Road_End_00588c20(Rect(bridge.X - 6, bridge.Y - 2, 6, 6), false) && Map_Random_Range_005797a0(0,1)) {
                    Place_Tile_Footprint_005883f0(unknown_tileset_a6dd0c+9, Cell(bridge.X - 4, bridge.Y));
                } else {
                    Place_Tile_Footprint_005883f0(unknown_tileset_a6dd08+2, Cell(bridge.X - 1, bridge.Y));
                }

                if (!Place_Neutral_Building_00571db0(Rect(bridge.X, bridge.Y - 1, 2, 5))) {
                    Place_Neutral_Building_00571db0(Rect(bridge.X - 1, bridge.Y - 2, 3, 7));
                }

                if (!Place_Neutral_Building_00571db0(Rect(bridge.X + bridge.Width - 2, bridge.Y - 1, 2, 5))) {
                    Place_Neutral_Building_00571db0(Rect(bridge.X + bridge.Width - 2, bridge.Y - 2, 3, 7));
                }

                result = true;
            }
        } else if (bridge.Is_Valid()) {
            if (allow_rect1 && Is_Bridge_Allowed_00571bc0(bridge)) {
                for (int y = bridge.Y; y < bridge.Y + bridge.Height; y++) {
                    for (int x = bridge.X; x < bridge.X + bridge.Width; x++) {
                        if (y == bridge.Y) {
                            Map[Cell(x, y)].OverlayIndex = 0x60;
                        } else if (y == bridge.Y + bridge.Height - 1) {
                            Map[Cell(x, y)].OverlayIndex = 0x62;
                        } else {
                            Map[Cell(x, y)].OverlayIndex = 0x53 + (y % 4);
                        }
                        Map[Cell(x, y)].OverlayData = x - bridge.X;
                    }
                }

                if (Can_Place_Paved_Road_End_00588c20(Rect(bridge.X - 2, bridge.Y - 6, 7, 6), false) && Map_Random_Range_005797a0(0,1)) {
                    Place_Tile_Footprint_005883f0(unknown_tileset_a6dd0c+13, Cell(bridge.X, bridge.Y - 4));
                } else {
                    Place_Tile_Footprint_005883f0(unknown_tileset_a6dd08+1, Cell(bridge.X, bridge.Y - 1));
                }

                if (Can_Place_Paved_Road_End_00588c20(Rect(bridge.X - 2, bridge.Y + bridge.Height, 7, 6), false) && Map_Random_Range_005797a0(0,1)) {
                    Place_Tile_Footprint_005883f0(unknown_tileset_a6dd0c+12, Cell(bridge.X, bridge.Y + bridge.Height));
                } else {
                    Place_Tile_Footprint_005883f0(unknown_tileset_a6dd08+3, Cell(bridge.X, bridge.Y + bridge.Height));
                }

                if (!Place_Neutral_Building_00571db0(Rect(bridge.X - 1, bridge.Y, 5, 2))) {
                    Place_Neutral_Building_00571db0(Rect(bridge.X - 2, bridge.Y - 1, 7, 3));
                }

                if (!Place_Neutral_Building_00571db0(Rect(bridge.X - 1, bridge.Y + bridge.Height - 2, 5, 2))) {
                    Place_Neutral_Building_00571db0(Rect(bridge.X - 2, bridge.Y + bridge.Height - 2, 7, 3));
                }

                result = true;
            }
        }

    }

    return(result);
}


bool __fastcall Generation_Cell_In_Bounds_0058da10(Cell const &cell);
bool MapRegionClass::Is_Bridge_Allowed_00571bc0(Rect const &area) const
{
    int height=Map[Cell(area.X,area.Y)].unknown_11b;
    if(Generation_Cell_In_Bounds_0058da10(XY_Cell(area.X,area.Y)) &&
       Generation_Cell_In_Bounds_0058da10(XY_Cell(area.X+area.Width-1,area.Y)) &&
       Generation_Cell_In_Bounds_0058da10(XY_Cell(area.X,area.Y+area.Height-1)) &&
       Generation_Cell_In_Bounds_0058da10(XY_Cell(area.X+area.Width-1,area.Y+area.Height-1))) {
        for(int y=area.Y;y<area.Y+area.Height+1;++y) {
            for(int x=area.X;x<area.X+area.Width+1;++x) {
                CellClass &cell=Map[Cell(x,y)];
                if(cell.OverlayIndex!=-1)return false;
                if(cell.unknown_11b!=height)return false;
                if(!cell.unknown_47b590() && !cell.unknown_47b7e0())return false;
            }
        }
        return true;
    }
    return false;
}


bool MapRegionClass::Place_Neutral_Building_00571db0(Rect const &area)
{
    for(int y=area.Y;y<area.Y+area.Height+1;y++) {
        for(int x=area.X;x<area.X+area.Width+1;x++) {
            CellClass &cell=Map[Cell(x,y)];
            if(cell.OverlayIndex==-1 && cell.unknown_47b590() && !cell.OccupierPtr) {
                HouseClass *house=HouseClass::As_Pointer(HouseTypeClass::Find_Index("Neutral"));
                int type=BuildingTypeClass::From_Name("CABHUT");
                BuildingClass *building=new BuildingClass(BuildingTypes[type],house);
                int x=cell.MapCoords.X*256+128;
                int y=cell.MapCoords.Y*256+128;
                building->Unlimbo(&CoordStruct(x,y,0),0);
                return true;
            }
        }
    }
    return false;
}


MapRegionClass *__fastcall Find_Map_Region_By_Index_00575ce0(int);
unsigned __fastcall Map_Random_Range_005797a0(unsigned,unsigned);
extern float const unknown_ramp_attempt_scale_7a5df0;
// Original ECX MapRegion receiver; plain RET. Reference name; unknown fields remain explicit.
void MapRegionClass::Make_Region_Connections_00571ed0()
{
    if(unknown_14) {
        for(int index=0;index<NeighborRegionIndices->Count()-1;++index) {
            MapRegionClass *first=Find_Map_Region_By_Index_00575ce0((*NeighborRegionIndices)[index]);
            if(first->NeighborRegionIndices->Count()>1 || first->CellCount>50) {
                for(int next=index+1;next<NeighborRegionIndices->Count();++next) {
                    MapRegionClass *second=Find_Map_Region_By_Index_00575ce0((*NeighborRegionIndices)[next]);
                    if((second->NeighborRegionIndices->Count()>1 || second->CellCount>50) && !second->unknown_14 &&
                       second->unknown_10==first->unknown_10 && first->unknown_10==unknown_10) {
                        Connect_Regions_With_Bridge_00570bc0(first,second);
                    }
                }
            }
        }
    } else {
        for(int neighbor=0;neighbor<NeighborRegionIndices->Count();++neighbor) {
            MapRegionClass *region=Find_Map_Region_By_Index_00575ce0((*NeighborRegionIndices)[neighbor]);
            if(region->Index>Index && region->unknown_10!=unknown_10) {
                int connected=0;
                int target=((int)Map_Random_Range_005797a0(0,100)<RandomMapGen.SeedData.Accessibility?(int)Map_Random_Range_005797a0(1,2):0)+1;
                int tries=0;
                int higher=region->unknown_10>unknown_10?region->Index:Index;
                int lower=region->unknown_10>unknown_10?Index:region->Index;
                MapRegionClass *upper=Find_Map_Region_By_Index_00575ce0(higher);
                DynamicVectorClass<Cell> *border=upper->Collect_Boundary_Cells_0056ed10();
                while(connected<target && tries<100) {
                    unsigned last=(unsigned)(border->Count()-1);
                    struct Range { double Span; } range={(unsigned)(last+1)};
                    unsigned choice;
                    do {
                        choice=(unsigned)((unsigned)unknown_map_random_a705c0()*range.Span*(1.0/4294967295.0));
                    } while(choice>last);
                    Cell position=(*border)[choice];
                    if(Map.Unknown_0055A660(position,true)) {
                        connected+=upper->Connect_Regions_With_Ramp_00572270(position,lower,(float)tries*unknown_ramp_attempt_scale_7a5df0)?1:0;
                    }
                    ++tries;
                }
                if(connected==0)unknown_1b=false;
                delete border;
            }
        }
    }
}


unsigned __fastcall Map_Random_Range_005797a0(unsigned,unsigned);
bool MapRegionClass::Connect_Regions_With_Ramp_00572270(Cell const &cell,int region,float density)
{
    bool valid=true;
    unsigned mask=Build_Neighbor_Region_Mask_005728d0(cell,valid,region,density);
    if(!valid)return false;
    bool result=false;
    if(mask==255)return false;
    {
    Cell first,second;
    if((mask&0x82)==0x82 && (mask&0x38)==0) {
        first.X=cell.X-1;first.Y=cell.Y-5;
        second.X=cell.X+5;second.Y=cell.Y+1;
        if(mask&0x40)++first.Y;
        if(mask&4)--second.X;
        result=unknown_005753f0(first,second,region);
        if(result)goto fallback;
    }
    if((mask&10)==10 && (mask&0x11)==0) {
        first.X=cell.X+5;first.Y=cell.Y-1;
        second.X=cell.X-1;second.Y=cell.Y+5;
        if(mask&0x10)--second.Y;
        if(mask&1)--first.X;
        result=unknown_00574e50(first,second,region);
        if(result)goto fallback;
    }
    if((mask&0x28)==0x28 && (mask&0x83)==0) {
        first.X=cell.X+1;first.Y=cell.Y+5;
        second.X=cell.X-5;second.Y=cell.Y-1;
        if(mask&0x40)--second.X;
        if(mask&4)--first.Y;
        result=unknown_00574930(first,second,region);
        if(result)goto fallback;
    }
    if((mask&0xa0)==0xa0 && (mask&0xe)==0) {
        first.X=cell.X-5;first.Y=cell.Y+1;
        second.X=cell.X+1;second.Y=cell.Y-5;
        if(mask&1)++second.Y;
        if(mask&0x10)++first.X;
        result=unknown_00574930(first,second,region);
        if(result)goto fallback;
    }
    if((mask&0x70)==0 && (mask&0x88)!=0) {
        first.X=cell.X-1;first.Y=cell.Y-4;
        second.X=first.X;second.Y=cell.Y+4;
        if(mask&0x80) {++first.Y;++second.Y;}
        if(mask&8) {--second.Y;--first.Y;}
        first.X+=(short)Map_Random_Range_005797a0(0,1);
        second.X+=(short)Map_Random_Range_005797a0(0,1);
        result=unknown_00573d40(first,second,region);
        if(result)goto fallback;
    }
    if((mask&7)==0 && (mask&0x88)!=0) {
        first.X=cell.X+1;first.Y=cell.Y+4;
        second.X=first.X;second.Y=cell.Y-4;
        if(mask&0x80) {++second.Y;++first.Y;}
        if(mask&8) {--first.Y;--second.Y;}
        first.X+=(short)Map_Random_Range_005797a0(0,1)-1;
        second.X+=(short)Map_Random_Range_005797a0(0,1)-1;
        result=unknown_00573040(first,second,region);
        if(result)goto fallback;
    }
    if((mask&0xc1)==0 && (mask&0x22)!=0) {
        first.X=cell.X+4;first.Y=cell.Y-1;
        second.X=cell.X-4;second.Y=first.Y;
        if(mask&0x20) {++second.X;++first.X;}
        if(mask&2) {--first.X;--second.X;}
        first.Y+=(short)Map_Random_Range_005797a0(0,1);
        second.Y+=(short)Map_Random_Range_005797a0(0,1);
        result=unknown_00573680(first,second,region);
        if(result)goto fallback;
    }
    if((mask&0x1c)==0 && (mask&0x22)!=0) {
        first.X=cell.X-4;first.Y=cell.Y+1;
        second.X=cell.X+4;second.Y=first.Y;
        if(mask&0x20) {++first.X;++second.X;}
        if(mask&2) {--second.X;--first.X;}
        first.Y+=(short)Map_Random_Range_005797a0(0,1)-1;
        second.Y+=(short)Map_Random_Range_005797a0(0,1)-1;
        result=unknown_005729f0(first,second,region);
    }
    }
fallback:
    if(density>0.5f && !result) {
        Cell first,second;
        if((mask&8)==0) {
            first.X=cell.X-4;first.Y=cell.Y+1;
            second.X=cell.X+4;second.Y=cell.Y+1;
            result=unknown_005729f0(first,second,region);
            if(result)return result;
        }
        if((mask&2)==0) {
            first.X=cell.X+1;first.Y=cell.Y+4;
            second.X=cell.X+1;second.Y=cell.Y-4;
            result=unknown_00573040(first,second,region);
            if(result)return result;
        }
        if((mask&8)==0) {
            first.X=cell.X+4;first.Y=cell.Y-1;
            second.X=cell.X-4;second.Y=cell.Y-1;
            result=unknown_00573680(first,second,region);
            if(result)return result;
        }
        if((mask&32)==0) {
            first.X=cell.X-1;first.Y=cell.Y-4;
            second.X=cell.X-1;second.Y=cell.Y+4;
            result=unknown_00573d40(first,second,region);
            if(result)return result;
        }
    }
    return result;
}


bool __fastcall Rectangle_Has_Region_Cells_00583630(Rect const &area,int region,int unknown,int required);
extern double const unknown_patch_minimum_7a5c10,unknown_patch_maximum_7a5c18;
extern float const unknown_patch_one_79b7f8;
extern unsigned unknown_neighbor_bits_7e09c8[3][3];
unsigned MapRegionClass::Build_Neighbor_Region_Mask_005728d0(Cell const &position,bool &valid,int unknown,float density)
{
    double threshold=unknown_patch_maximum_7a5c18;
    threshold-=unknown_patch_minimum_7a5c10;
    threshold*=unknown_patch_one_79b7f8-density;
    threshold+=unknown_patch_minimum_7a5c10;
    int required=(int)threshold;
    Cell corner=position-Cell(2,2);
    int left=corner.X;
    int top=corner.Y;
    if(!Rectangle_Has_Region_Cells_00583630(Rect(left,top,5,5),Index,unknown,required)) {
        valid=false;
        return 0;
    }
    unsigned mask=0;
    for(int row=0;row<3;++row) {
        for(int column=0;column<3;++column) {
            if(row!=1 || column!=1) {
                if(Rectangle_Has_Region_Cells_00583630(Rect(left+5*(column-1),top+5*(row-1),5,5),Index,unknown,required))
                    mask|=unknown_neighbor_bits_7e09c8[row][column];
            }
        }
    }
    return mask;
}


extern int unknown_ramp_heights_7e09ec[5];
extern int unknown_ramp_shapes_7e0a00[10];
void __stdcall Assign_Region_To_Rectangle_00575a90(Rect const &,int);
void __fastcall Place_Tile_Footprint_005883f0(int,Cell const &,int,int);
unsigned __fastcall Map_Random_Range_005797a0(unsigned,unsigned);
bool MapRegionClass::unknown_005729f0(Cell const &first,Cell const &second,int lowRegion)
{
    Rect area(first.X-5,(first.Y<second.Y?first.Y:second.Y)-3,second.X-first.X+9,(first.Y>second.Y?first.Y:second.Y)-(first.Y<second.Y?first.Y:second.Y)+11);
    if(!Is_Rectangle_Clear_00575910(area,lowRegion)) {
        return false;
    }
    int drift=second.Y-first.Y;
    int span=abs(drift);
    int width=second.X-first.X;
    if(span>width-5) {
        return false;
    }
    int half=(width+1)/2+3;
    Assign_Region_To_Rectangle_00575a90(Rect(area.X,first.Y+1,half+2,area.Y+area.Height-first.Y+1),lowRegion);
    Assign_Region_To_Rectangle_00575a90(Rect(second.X-half+3,second.Y+1,half+2,area.Y+area.Height-second.Y+1),lowRegion);
    Mark_Rectangle_00575bf0(Rect(area.X+4,area.Y+2,half-2,first.Y-area.Y-1));
    Mark_Rectangle_00575bf0(Rect(second.X-half+2,area.Y+2,half-1,second.Y-area.Y-1));
    Place_Tile_Footprint_005883f0(unknown_tileset_a6e038,first,lowRegion,unknown_10);
    Place_Tile_Footprint_005883f0(unknown_tileset_a6e038+1,second-Cell(2,0),lowRegion,unknown_10);
    Cell tail(0,0);
    if(drift!=0) {
        int pick=Map_Random_Range_005797a0(0,1);
        Cell bias;
        bias.X=0;
        int down=drift>0;
        bias.Y=drift<0?-1:0;
        if(pick==1) {
            tail+=Cell(span,drift);
        } else {
            bias.X=(second-Cell(span,0)).X-first.X-5;
        }
        for(int forward=0;forward<span;forward++) {
            for(int row=0;row<5;row++) {
                short offset=down?forward:-forward;
                Cell position=Cell(first.X+3,first.Y)+Cell(forward,row)+bias;
                position=Cell(position.X,position.Y+offset);
                CellClass &target=Map[position];
                target.unknown_11b=(char)(unknown_10+unknown_ramp_heights_7e09ec[row]-4);
                target.unknown_11c=(unsigned char)unknown_ramp_shapes_7e0a00[5*down+row];
                target.IsoTileIndex=(unsigned)unknown_tileset_a6e018+target.unknown_11c-1;
                target.unknown_11a=0;
            }
        }
    }
    for(int column=0;column<second.X-first.X-span-5;column++) {
        for(int row=0;row<4;row++) {
            Cell position=Cell(first.X+3,first.Y)+Cell(column,row)+tail;
            Set_Cell_Region_Index_0056e970(position,lowRegion);
            CellClass &target=Map[position];
            target.unknown_11b=(char)(unknown_10-(char)row-1);
            target.unknown_11c=4;
            target.IsoTileIndex=(unsigned)unknown_tileset_a6e018+3u;
            target.unknown_11a=0;
        }
    }
    return true;
}


// East-facing straight ramp (TibSun Build_East_Ramp): `tail+=` and `bias-=` are in place, RA2_POINT_ADD_IN_PLACE (set by the unit's first source).
extern int unknown_ramp_heights_7e0a28[5];
extern int unknown_ramp_shapes_7e0a3c[10];
void __stdcall Assign_Region_To_Rectangle_00575a90(Rect const &,int);
void __fastcall Place_Tile_Footprint_005883f0(int,Cell const &,int,int);
unsigned __fastcall Map_Random_Range_005797a0(unsigned,unsigned);
bool MapRegionClass::unknown_00573040(Cell const &first,Cell const &second,int lowRegion)
{
    Rect area((first.X<second.X?first.X:second.X)-3,second.Y-4,(first.X>second.X?first.X:second.X)-(first.X<second.X?first.X:second.X)+11,first.Y-second.Y+9);
    if(!Is_Rectangle_Clear_00575910(area,lowRegion)) {
        return false;
    }
    int drift=second.X-first.X;
    int span=abs(drift);
    int height=first.Y-second.Y;
    if(span>height-5) {
        return false;
    }
    int half=(height+1)/2+2;
    Assign_Region_To_Rectangle_00575a90(Rect(first.X+1,first.Y-half+2,7,half+3),lowRegion);
    Assign_Region_To_Rectangle_00575a90(Rect(second.X+1,area.Y,7,half+2),lowRegion);
    Mark_Rectangle_00575bf0(Rect(area.X+2,first.Y-half+2,first.X-area.X-1,half-1));
    Mark_Rectangle_00575bf0(Rect(area.X+2,second.Y,second.X-area.X-1,half-2));
    Place_Tile_Footprint_005883f0(unknown_tileset_a6e038+2,first-Cell(0,2),lowRegion,unknown_10);
    Place_Tile_Footprint_005883f0(unknown_tileset_a6e038+3,second,lowRegion,unknown_10);
    Cell tail(0,0);
    if(drift!=0) {
        int pick=Map_Random_Range_005797a0(0,1);
        int down=drift>0;
        Cell bias(drift<0?-1:0,0);
        if(pick==1) {
            tail+=Cell(drift,-span);
        } else {
            bias-=Cell(0,first.Y-(short)span-second.Y-5);
        }
        for(int forward=0;forward<span;forward++) {
            for(int row=0;row<5;row++) {
                short offset=down?forward:-forward;
                Cell position=Cell(first.X,first.Y-3)+Cell(row,-forward)+bias;
                position=Cell(position.X+offset,position.Y);
                CellClass &target=Map[position];
                target.unknown_11b=(char)(unknown_10+unknown_ramp_heights_7e0a28[row]-4);
                target.unknown_11c=(unsigned char)unknown_ramp_shapes_7e0a3c[5*down+row];
                target.IsoTileIndex=(unsigned)unknown_tileset_a6e018+target.unknown_11c-1;
                target.unknown_11a=0;
            }
        }
    }
    for(int column=0;column<first.Y-second.Y-span-5;column++) {
        for(int row=0;row<4;row++) {
            Cell position=Cell(first.X,first.Y-3)+Cell(row,-column)+tail;
            Set_Cell_Region_Index_0056e970(position,lowRegion);
            CellClass &target=Map[position];
            target.unknown_11b=(char)(unknown_10-(char)row-1);
            target.unknown_11c=3;
            target.IsoTileIndex=(unsigned)unknown_tileset_a6e018+2u;
            target.unknown_11a=0;
        }
    }
    return true;
}


extern int unknown_ramp_heights_7e0a64[5];
extern int unknown_ramp_shapes_7e0a78[10];
void __stdcall Assign_Region_To_Rectangle_00575a90(Rect const &,int);
void __fastcall Place_Tile_Footprint_005883f0(int,Cell const &,int,int);
unsigned __fastcall Map_Random_Range_005797a0(unsigned,unsigned);
bool MapRegionClass::unknown_00573680(Cell const &first,Cell const &second,int lowRegion)
{
    Rect area(second.X-4,(first.Y<second.Y?first.Y:second.Y)-7,first.X-second.X+8,(first.Y>second.Y?first.Y:second.Y)-(first.Y<second.Y?first.Y:second.Y)+11);
    if(!Is_Rectangle_Clear_00575910(area,lowRegion)) {
        return false;
    }
    int drift=second.Y-first.Y;
    int span=abs(drift);
    int width=first.X-second.X;
    if(span>width-5) {
        return false;
    }
    int half=(width+1)/2+3;
    Assign_Region_To_Rectangle_00575a90(Rect(area.X,area.Y,half+2,second.Y-area.Y),lowRegion);
    Assign_Region_To_Rectangle_00575a90(Rect(first.X-half+2,area.Y,half+3,first.Y-area.Y),lowRegion);
    Mark_Rectangle_00575bf0(Rect(second.X,second.Y,half-2,area.Y+area.Height-second.Y-2));
    Mark_Rectangle_00575bf0(Rect(first.X-half+2,first.Y,half-1,area.Y+area.Height-first.Y-2));
    Place_Tile_Footprint_005883f0(unknown_tileset_a6e038+4,second-Cell(0,3),lowRegion,unknown_10);
    Place_Tile_Footprint_005883f0(unknown_tileset_a6e038+5,first-Cell(2,3),lowRegion,unknown_10);
    Place_Tile_Footprint_005883f0(unknown_tileset_a6e038+6,first-Cell(1,1),lowRegion,unknown_10);
    Cell tail(0,0);
    if(drift!=0) {
        int pick=Map_Random_Range_005797a0(0,1);
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
                target.unknown_11b=(char)(unknown_10+unknown_ramp_heights_7e0a64[row]-4);
                target.unknown_11c=(unsigned char)unknown_ramp_shapes_7e0a78[5*down+row];
                target.IsoTileIndex=(unsigned)unknown_tileset_a6e018+target.unknown_11c-1;
                target.unknown_11a=0;
            }
        }
    }
    for(int column=0;column<first.X-second.X-span-5;column++) {
        for(int row=0;row<4;row++) {
            Cell position=Cell(second.X+3,second.Y)+Cell(column,-row)+tail;
            Set_Cell_Region_Index_0056e970(position,lowRegion);
            CellClass &target=Map[position];
            target.unknown_11b=(char)(unknown_10-(char)row-1);
            target.unknown_11c=2;
            target.IsoTileIndex=(unsigned)unknown_tileset_a6e018+1u;
            target.unknown_11a=0;
        }
    }
    return true;
}


// West-facing straight ramp (TibSun Build_West_Ramp): no named height, `tail+=` and `bias+=` in place (RA2_POINT_ADD_IN_PLACE in the unit's first source).
extern int unknown_ramp_heights_7e0aa0[5];
extern int unknown_ramp_shapes_7e0ab4[10];
void __stdcall Assign_Region_To_Rectangle_00575a90(Rect const &,int);
void __fastcall Place_Tile_Footprint_005883f0(int,Cell const &,int,int);
unsigned __fastcall Map_Random_Range_005797a0(unsigned,unsigned);
bool MapRegionClass::unknown_00573d40(Cell const &first,Cell const &second,int lowRegion)
{
    Rect area((first.X<second.X?first.X:second.X)-7,first.Y-4,(first.X>second.X?first.X:second.X)-(first.X<second.X?first.X:second.X)+11,second.Y-first.Y+9);
    if(!Is_Rectangle_Clear_00575910(area,lowRegion)) {
        return false;
    }
    int drift=second.X-first.X;
    int span=abs(drift);
    if(span>second.Y-first.Y-5) {
        return false;
    }
    int half=(second.Y-first.Y+1)/2+2;
    Assign_Region_To_Rectangle_00575a90(Rect(area.X,area.Y,first.X-area.X,half+2),lowRegion);
    Assign_Region_To_Rectangle_00575a90(Rect(area.X,second.Y-half+3,second.X-area.X,half+2),lowRegion);
    Mark_Rectangle_00575bf0(Rect(first.X,first.Y,area.X+area.Width-first.X-1,half-1));
    Mark_Rectangle_00575bf0(Rect(second.X,second.Y-half+3,area.X+area.Width-second.X-1,half-2));
    Place_Tile_Footprint_005883f0(unknown_tileset_a6e038+7,first-Cell(3,0),lowRegion,unknown_10);
    Place_Tile_Footprint_005883f0(unknown_tileset_a6e038+8,second-Cell(3,2),lowRegion,unknown_10);
    Place_Tile_Footprint_005883f0(unknown_tileset_a6e038+9,second-Cell(1,1),lowRegion,unknown_10);
    Cell tail(0,0);
    if(drift!=0) {
        int pick=Map_Random_Range_005797a0(0,1);
        int down=drift>0;
        Cell bias(drift>0,0);
        if(pick==1) {
            tail+=Cell(drift,span);
        } else {
            bias+=Cell(0,second.Y-(short)span-first.Y-5);
        }
        for(int forward=0;forward<span;forward++) {
            for(int row=0;row<5;row++) {
                short offset=down?forward:-forward;
                Cell position=Cell(first.X,first.Y+3)+Cell(-row,forward)+bias;
                position=Cell(position.X+offset,position.Y);
                CellClass &target=Map[position];
                target.unknown_11b=(char)(unknown_10+unknown_ramp_heights_7e0aa0[row]-4);
                target.unknown_11c=(unsigned char)unknown_ramp_shapes_7e0ab4[5*down+row];
                target.IsoTileIndex=(unsigned)unknown_tileset_a6e018+target.unknown_11c-1;
                target.unknown_11a=0;
            }
        }
    }
    for(int column=0;column<second.Y-first.Y-span-5;column++) {
        for(int row=0;row<4;row++) {
            Cell position=Cell(first.X,first.Y+3)+Cell(-row,column)+tail;
            Set_Cell_Region_Index_0056e970(position,lowRegion);
            CellClass &target=Map[position];
            target.unknown_11b=(char)(unknown_10-(char)row-1);
            target.unknown_11c=1;
            target.IsoTileIndex=(unsigned)unknown_tileset_a6e018;
            target.unknown_11a=0;
        }
    }
    return true;
}


// Southeast corner ramp (TibSun Build_Corner_Ramp_SE); the only row with no caller in the unit: Connect_Regions_With_Ramp calls NE twice.
void __stdcall Assign_Region_To_Rectangle_00575a90(Rect const &area,int region);
void __fastcall Place_Tile_Footprint_005883f0(int tile,Cell const &position,int region,int height);
bool MapRegionClass::unknown_005743f0(Cell const &first,Cell const &second,int lowRegion)
{
    int width=second.X-first.X;
    if(width>=3) {
        int height=first.Y-second.Y;
        if(height>=3) {
            
            int top=second.Y-2;
            Rect area(first.X-2,top,width+10,first.Y-second.Y+10);
            if(Is_Rectangle_Clear_00575910(area,lowRegion)) {
                Assign_Region_To_Rectangle_00575a90(Rect(area.X,first.Y+1,area.Width,7),lowRegion);
                Assign_Region_To_Rectangle_00575a90(Rect(second.X+1,area.Y,7,area.Height),lowRegion);
                Mark_Rectangle_00575bf0(Rect(first.X,first.Y-1,1,2));
                // The second X is intentional: original574672 loads both origins from second.X.
                Mark_Rectangle_00575bf0(Rect(second.X-1,second.X,2,1));
                Mark_Rectangle_00575bf0(Rect(first.X+1,second.Y+1,second.X-first.X-1,first.Y-second.Y-1));
                Place_Tile_Footprint_005883f0(unknown_tileset_a6e038,first,lowRegion,unknown_10);
                Place_Tile_Footprint_005883f0((int)((unsigned)unknown_tileset_a6e038+3u),second,lowRegion,unknown_10);
                for(int step=0;step<4;++step) {
                    CellClass &target=Map[Cell(second.X+step,first.Y+step)];
                    target.unknown_11b=(char)(unknown_10-(char)step-1);
                    target.unknown_11c=7;
                    target.IsoTileIndex=(unsigned)unknown_tileset_a6e018+6u;
                    target.unknown_11a=0;
                }
                for(int row=0;row<4;++row) {
                    for(int column=0;column<second.X-first.X+row-3;++column) {
                        CellClass &target=Map[Cell(first.X+column+3,first.Y+row)];
                        target.unknown_11b=(char)(unknown_10-(char)row-1);
                        target.unknown_11c=4;
                        target.IsoTileIndex=(unsigned)unknown_tileset_a6e018+3u;
                        target.unknown_11a=0;
                    }
                }
                for(int column=0;column<4;++column) {
                    for(int row=0;row<first.Y-second.Y+column-3;++row) {
                        CellClass &target=Map[Cell(second.X+column,second.Y+row+3)];
                        target.unknown_11b=(char)(unknown_10-(char)column-1);
                        target.unknown_11c=3;
                        target.IsoTileIndex=(unsigned)unknown_tileset_a6e018+2u;
                        target.unknown_11a=0;
                    }
                }
                return true;
            }
        }
    }
    return false;
}


void __stdcall Assign_Region_To_Rectangle_00575a90(Rect const &,int);
void __fastcall Place_Tile_Footprint_005883f0(int,Cell const &,int,int);
// Original thiscall/AL/RET12. Reference operation name is a hypothesis.
bool MapRegionClass::unknown_00574930(Cell const &first,Cell const &second,int region)
{
    int dx=first.X-second.X;
    if(dx>=3) {
        int dy=first.Y-second.Y;
        if(dy>=3) {
            int top=second.Y-7;
            Rect area(second.X-2,top,dx+10,first.Y-second.Y+10);
            if(Is_Rectangle_Clear_00575910(area,region)) {
                Assign_Region_To_Rectangle_00575a90(Rect(area.X,area.Y,area.Width,7),region);
                Assign_Region_To_Rectangle_00575a90(Rect(first.X+1,area.Y,7,area.Height),region);
                Mark_Rectangle_00575bf0(Rect(second.X,second.Y,1,2));
                Mark_Rectangle_00575bf0(Rect(first.X-1,first.Y,2,1));
                Mark_Rectangle_00575bf0(Rect(second.X+1,second.Y+1,first.X-second.X-1,first.Y-second.Y-1));
                Place_Tile_Footprint_005883f0(unknown_tileset_a6e038+2,first-Cell(0,2),region,unknown_10);
                Place_Tile_Footprint_005883f0(unknown_tileset_a6e038+4,second-Cell(0,3),region,unknown_10);
                for(int corner=0;corner<4;++corner) {
                    CellClass &cell=Map[Cell(first.X+corner,second.Y-corner)];
                    cell.unknown_11b=(char)(unknown_10-(char)corner-1);
                    cell.unknown_11c=6;
                    cell.IsoTileIndex=unknown_tileset_a6e018+5;
                    cell.unknown_11a=0;
                }
                for(int east=0;east<4;++east) {
                    for(int y=0;y<first.Y-second.Y+east-3;++y) {
                        CellClass &cell=Map[Cell(first.X+east,first.Y-y-3)];
                        cell.unknown_11b=(char)(unknown_10-(char)east-1);
                        cell.unknown_11c=3;
                        cell.IsoTileIndex=unknown_tileset_a6e018+2;
                        cell.unknown_11a=0;
                    }
                }
                for(int north=0;north<4;++north) {
                    for(int x=0;x<first.X-second.X+north-3;++x) {
                        CellClass &cell=Map[Cell(second.X+x+3,second.Y-north)];
                        cell.unknown_11b=(char)(unknown_10-(char)north-1);
                        cell.unknown_11c=2;
                        cell.IsoTileIndex=unknown_tileset_a6e018+1;
                        cell.unknown_11a=0;
                    }
                }
                return true;
            }
        }
    }
    return false;
}


void __stdcall Assign_Region_To_Rectangle_00575a90(Rect const &,int);
void __fastcall Place_Tile_Footprint_005883f0(int,Cell const &,int,int);
// First is the eastern endpoint, second the southern endpoint. ECXowner/RET12/AL.
bool MapRegionClass::unknown_00574e50(Cell const &first,Cell const &second,int region)
{
    int dx=first.X-second.X;
    if(dx>=3) {
        int dy=second.Y-first.Y;
        if(dy>=3) {
            Rect area(second.X-7,first.Y-7,dx+10,dy+10);
            if(Is_Rectangle_Clear_00575910(area,region)) {
                Assign_Region_To_Rectangle_00575a90(Rect(area.X,area.Y,area.Width,7),region);
                Assign_Region_To_Rectangle_00575a90(Rect(area.X,area.Y,7,area.Height),region);
                Mark_Rectangle_00575bf0(Rect(first.X,first.Y,1,2));
                Mark_Rectangle_00575bf0(Rect(second.X,second.Y,2,1));
                Mark_Rectangle_00575bf0(Rect(second.X+1,first.Y+1,first.X-second.X-1,second.Y-first.Y-1));
                Place_Tile_Footprint_005883f0(unknown_tileset_a6e038+5,first-Cell(2,3),region,unknown_10);
                Place_Tile_Footprint_005883f0(unknown_tileset_a6e038+6,first-Cell(1,1),region,unknown_10);
                Place_Tile_Footprint_005883f0(unknown_tileset_a6e038+8,second-Cell(3,2),region,unknown_10);
                Place_Tile_Footprint_005883f0(unknown_tileset_a6e038+9,second-Cell(1,1),region,unknown_10);
                for(int corner=0;corner<4;++corner) {
                    CellClass &cell=Map[Cell(second.X-corner,first.Y-corner)];
                    cell.unknown_11b=(char)(unknown_10-(char)corner-1);
                    cell.unknown_11c=5;
                    cell.IsoTileIndex=unknown_tileset_a6e018+4;
                    cell.unknown_11a=0;
                }
                for(int north=0;north<4;++north) {
                    for(int x=0;x<first.X-second.X+north-3;++x) {
                        CellClass &cell=Map[Cell(first.X-x-3,first.Y-north)];
                        cell.unknown_11b=(char)(unknown_10-(char)north-1);
                        cell.unknown_11c=2;
                        cell.IsoTileIndex=unknown_tileset_a6e018+1;
                        cell.unknown_11a=0;
                    }
                }
                for(int west=0;west<4;++west) {
                    for(int y=0;y<second.Y-first.Y+west-3;++y) {
                        CellClass &cell=Map[Cell(second.X-west,second.Y-y-3)];
                        cell.unknown_11b=(char)(unknown_10-(char)west-1);
                        cell.unknown_11c=1;
                        cell.IsoTileIndex=unknown_tileset_a6e018;
                        cell.unknown_11a=0;
                    }
                }
                return true;
            }
        }
    }
    return false;
}


// Name follows reference hypothesis; original receiver, AL result and RET12 verified.
void __stdcall Assign_Region_To_Rectangle_00575a90(Rect const &,int);
void __fastcall Place_Tile_Footprint_005883f0(int,Cell const &,int,int);
bool __fastcall Generation_Cell_In_Bounds_0058da10(Cell const &);

bool MapRegionClass::unknown_005753f0(Cell const &first,Cell const &second,int region)
{
    int dx=second.X-first.X;
    if(dx>=3) {
        int dy=second.Y-first.Y;
        if(dy>=3) {
            int top=first.Y-4;
            Rect area(first.X-7,top,dx+10,second.Y-first.Y+10);
            if(Is_Rectangle_Clear_00575910(area,region)) {
                Assign_Region_To_Rectangle_00575a90(Rect(area.X,area.Y,7,area.Height),region);
                Assign_Region_To_Rectangle_00575a90(Rect(area.X,second.Y+1,area.Width,7),region);
                Mark_Rectangle_00575bf0(Rect(first.X,first.Y,2,1));
                Mark_Rectangle_00575bf0(Rect(second.X,second.Y-1,1,2));
                Mark_Rectangle_00575bf0(Rect(first.X+1,first.Y+1,second.X-first.X-1,second.Y-first.Y-1));
                Place_Tile_Footprint_005883f0(unknown_tileset_a6e038+7,first-Cell(3,0),region,unknown_10);
                Place_Tile_Footprint_005883f0(unknown_tileset_a6e038+1,second-Cell(2,0),region,unknown_10);
                for(int corner=0;corner<4;++corner) {
                    CellClass &cell=Map[Cell(first.X-corner,second.Y+corner)];
                    cell.unknown_11b=(char)(unknown_10-(char)corner-1);
                    cell.unknown_11c=8;
                    cell.IsoTileIndex=unknown_tileset_a6e018+7;
                    cell.unknown_11a=0;
                }
                for(int west=0;west<4;++west) {
                    for(int y=0;y<second.Y-first.Y+west-3;++y) {
                        CellClass &cell=Map[Cell(first.X-west,first.Y+y+3)];
                        cell.unknown_11b=(char)(unknown_10-(char)west-1);
                        cell.unknown_11c=1;
                        cell.IsoTileIndex=unknown_tileset_a6e018;
                        cell.unknown_11a=0;
                    }
                }
                for(int south=0;south<4;++south) {
                    for(int x=0;x<second.X-first.X+south-3;++x) {
                        CellClass &cell=Map[Cell(second.X-x-3,second.Y+south)];
                        cell.unknown_11b=(char)(unknown_10-(char)south-1);
                        cell.unknown_11c=4;
                        cell.IsoTileIndex=unknown_tileset_a6e018+3;
                        cell.unknown_11a=0;
                    }
                }
                return true;
            }
        }
    }
    return false;
}


bool MapRegionClass::Is_Rectangle_Clear_00575910(Rect const &area,int allowedRegion)
{
    if(Map.Unknown_0055A660(Cell(area.X,area.Y),true)) {
       int right=area.Width+area.X-1;
       if(Map.Unknown_0055A660(Cell(right,area.Y),true)) {
       int bottom=area.Y;
       bottom+=area.Height-1;
       if(Map.Unknown_0055A660(Cell(area.X,bottom),true)) {
       right=area.Width+area.X-1;
       bottom=area.Y;
       bottom+=area.Height-1;
       if(Map.Unknown_0055A660(Cell(right,bottom),true)) {
    for(int y=area.Y;y<area.Height+area.Y;++y) {
        for(int x=area.X;x<area.Width+area.X;++x) {
            Cell position(x,y);
            int region=UnknownMapGenerationCell::Get_Region(position);
            if(region!=Index && region!=allowedRegion)return false;
            if(!Map[position].unknown_47b590())return false;
        }
    }
    return true;
    }
    }
    }
    }
    return false;
}


extern DynamicVectorClass<MapRegionClass *> unknown_map_regions_a6fd28;
bool __fastcall Generation_Cell_In_Bounds_0058da10(Cell const &cell);
MapRegionClass *__fastcall Find_Map_Region_By_Index_00575ce0(int index);
void __stdcall Assign_Region_To_Rectangle_00575a90(Rect const &area,int index)
{
    for(int y=area.Y;y<area.Y+area.Height;y++) {
        for(int x=area.X;x<area.X+area.Width;x++) {
            Cell const position((short)x,(short)y);
            if(Generation_Cell_In_Bounds_0058da10(position)) {
                Set_Cell_Region_Index_0056e970(position,index);
                CellClass &target=Map[position];
                target.unknown_11b=(char)Find_Map_Region_By_Index_00575ce0(index)->unknown_10;
            }
        }
    }
}

extern int unknown_map_min_a70a1c,unknown_map_max_a70a20;
bool __fastcall Generation_Cell_In_Bounds_0058da10(Cell const &cell)
{
    int x=cell.X;
    int y=cell.Y;
    bool inside=(x+y>unknown_map_min_a70a1c && x-y<unknown_map_min_a70a1c &&
           y-x<unknown_map_min_a70a1c && x+y<=unknown_map_max_a70a20) ? true : false;
    return inside;
}

extern DynamicVectorClass<MapRegionClass *> unknown_map_regions_a6fd28;
MapRegionClass *__fastcall Find_Map_Region_By_Index_00575ce0(int index)
{
    for(int i=0;i<unknown_map_regions_a6fd28.Count();i++)
        if(unknown_map_regions_a6fd28[i]->Index==index)
            return unknown_map_regions_a6fd28[i];
    return 0;
}


bool __fastcall Generation_Cell_In_Bounds_0058da10(Cell const &);
void MapRegionClass::Mark_Rectangle_00575bf0(Rect const &area)
{
    for(int y=area.Y;y<area.Y+area.Height;++y) {
        for(int x=area.X;x<area.X+area.Width;++x) {
            Cell const position(x,y);
            if(Generation_Cell_In_Bounds_0058da10(position)) {
                Set_Cell_Region_Index_0056e970(position,Index);
                CellClass &target=Map[position];
                target.unknown_11b=(char)unknown_10;
            }
        }
    }
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// From ra2-decomp 0x00594420 Create_Zone_Region (src/mapgen.cpp, YR twin).
// void __fastcall MapRegionClass::Create_Zone_Region_00575d20(CellClass *cell_ptr, int zone_kind, int zone) (0x00575D20,
// RET4): a new region from a flood fill out of cell over the in-bounds cells that have no +0x4C occupant and lie in
// the given zone of the map.

static inline bool In_Zone_Map_Bounds(Cell const &cell)
{
    int x = cell.X;
    int y = cell.Y;
    int diagonal = y + cell.X;
    return diagonal > unknown_map_min_a70a1c &&
        x - y < unknown_map_min_a70a1c &&
        y - x < unknown_map_min_a70a1c &&
        diagonal <= unknown_map_max_a70a20;
}

void __fastcall MapRegionClass::Create_Zone_Region_00575d20(CellClass *cell_ptr, int zone_kind, int zone)
{
    MapRegionClass *region = new MapRegionClass(cell_ptr->MapCoords);
    int id = region->Index;
    DynamicVectorClass<Cell> pending;
    pending.Set_Growth_Step(20000);
    Set_Spread(cell_ptr->MapCoords, id);
    pending.Add(cell_ptr->MapCoords);
    while (pending.Count() > 0) {
        Cell cell = pending[pending.Count() - 1];
        pending.Delete(pending.Count() - 1);
        MapRegionClass::Get_Cell_Data(cell).RegionIndex = id;
        region->CellCount++;
        region->Cells.Add(cell);
        for (int dir = FACING_N; dir < FACING_COUNT; dir++) {
            CellClass &other = Map[unknown_adjacent_851a70[dir & 7] + cell];
            Cell const &next = other.MapCoords;
            if (In_Zone_Map_Bounds(other.MapCoords)) {
                UnknownMapGenerationCell &data = MapRegionClass::Get_Cell_Data(next);
                if (other.unknown_4c == 0 && Map.Zone(next, zone_kind, false) == zone && data.unknown_3c == -1) {
                    data.unknown_3c = id;
                    pending.Add(next);
                }
            }
        }
    }
}


// void __fastcall MapRegionClass::Create_Zone_Regions_00576000(int zone_kind, int zone) (0x00576000): discards every
// region, then makes one (Create_Zone_Region_00575d20) for each unclaimed in-bounds map cell with no +0x4C occupant
// that lies in the given zone. Not in TibSun.

static inline bool Zone_Cell_In_Bounds(Cell const &cell)
{
    int x = cell.X;
    int y = cell.Y;
    return y + x > unknown_map_min_a70a1c && x - y < unknown_map_min_a70a1c &&
        y - x < unknown_map_min_a70a1c && y + x <= unknown_map_max_a70a20;
}

void __fastcall MapRegionClass::Create_Zone_Regions_00576000(int zone_kind, int zone)
{
    Destroy_Cell_Regions();
    Map.Reset_Iterator();
    for (CellClass *cell = Map.Iterate(); cell; cell = Map.Iterate()) {
        Cell const &position = cell->MapCoords;
        if (Zone_Cell_In_Bounds(cell->MapCoords)) {
            if (Get_Cell_Data(position).RegionIndex == -1 && cell->unknown_4c == 0 &&
                Map.Zone(position, zone_kind, false) == zone) {
                Create_Zone_Region_00575d20(cell, zone_kind, zone);
            }
        }
    }
}


bool __fastcall Can_Place_Paved_Road_00588a30(Rect const &area,bool allowFirst,bool allowSecond);
void MapRegionClass::Generate_Starting_Points_00576170(int firstWaypoint)
{
    Rect bounds=Map.MapLocalSize;
    bounds.X+=4;
    bounds.Y+=4;
    bounds.Width-=8;
    bounds.Height-=8;
    DynamicVectorClass<Cell> candidates;
    unsigned int extra;
    do {extra=(unsigned int)((unsigned int)unknown_map_random_a705c0()*(3.0/4294967295.0));} while(extra>2);
    int wanted=unknown_20*15+extra;
    for(int attempt=0;candidates.Count()<wanted && attempt<300;++attempt) {
        unsigned int maximum=Cells.Count()-1;
        struct Range {double Span;};
        Range range={(unsigned int)(maximum+1)};
        unsigned int selected;
        do {selected=(unsigned int)((unsigned int)unknown_map_random_a705c0()*range.Span*(1.0/4294967295.0));} while(selected>maximum);
        Cell position=Cells[selected];
        Rect area(position.X-3,position.Y-3,6,6);
        if(Can_Place_Paved_Road_00588a30(area,false,false) && Map.Unknown_0055A840(bounds,position,true))candidates.Add(position);
    }
    DynamicVectorClass<Cell> *layout=Select_Separated_Cells_00576810(candidates);
    if(!layout)return;
    {
        for(int index=0;index<unknown_20;++index) {
            unknown_id_source_a3d290->Set_Waypoint_Location(firstWaypoint+index,(*layout)[index]);
            Map[unknown_id_source_a3d290->Get_Waypoint_Location(firstWaypoint+index)].unknown_140|=4;
        }
        for(index=0;index<unknown_20;++index)layout->Delete(0);
        delete (DynamicVectorClass<Cell> *)unknown_00;
        unknown_00=layout;
    }
}


// R109-supported original MapRegion receiver and borrowed/owned Cell-vector contracts.
extern double const unknown_separation_scale_7a5e18;
DynamicVectorClass<Cell> *MapRegionClass::Select_Separated_Cells_00576810(DynamicVectorClass<Cell> const &candidates)
{
    double scaling=(double)RandomMapGen.SeedData.TiberiumLayout;
    int assigned=unknown_20;
    scaling*=0.01;
    double best=-1.0;
    int first=-1;
    scaling*=unknown_separation_scale_7a5e18;
    int wanted=(int)((scaling/(double)RandomMapGen.SeedData.NumPlayers+2.0)*assigned);
    if(assigned==0 && candidates.Count()>0)wanted=candidates.Count();
    if(wanted>candidates.Count() || wanted==0) {
        if(assigned==0)return 0;
        wanted=candidates.Count();
    }
    DynamicVectorClass<Cell> *selected=new DynamicVectorClass<Cell>;
    int i;
    for(i=0;i<(int)((unsigned int)candidates.Count()-1u);i++) {
        Cell a=candidates[i];
        int regionA;
            if(!unknown_map_cells_a70a24)regionA=-1;
            else regionA=unknown_map_cells_a70a24[(int)((unsigned int)a.Y*(unsigned int)unknown_map_cell_side_84e804+(unsigned int)a.X)].RegionIndex;
        for(int j=i+1;j<candidates.Count();j++) {
            Cell b=candidates[j];
            int dx=(int)a.X-b.X,dy=(int)a.Y-b.Y;
            int regionB;
            if(!unknown_map_cells_a70a24)regionB=-1;
            else regionB=unknown_map_cells_a70a24[(int)((unsigned int)b.Y*(unsigned int)unknown_map_cell_side_84e804+(unsigned int)b.X)].RegionIndex;
            int square=(int)((unsigned int)dx*(unsigned int)dx+(unsigned int)dy*(unsigned int)dy);
            double distance=(double)Fast_Sqrt((double)square)+(double)(regionB!=regionA ? 20 : 0);
            if(j && distance>best) { best=distance;first=i; }
        }
    }
    if(first!=-1)selected->Add(candidates[first]);
    else if(candidates.Count()!=0) { selected->Add(candidates[0]);return selected; }
    while(selected->Count()<wanted) {
        best=-1.0;
        int next=-1;
        for(i=0;i<candidates.Count();i++) {
            Cell a=candidates[i];
            int regionA;
            if(!unknown_map_cells_a70a24)regionA=-1;
            else regionA=unknown_map_cells_a70a24[(int)((unsigned int)a.Y*(unsigned int)unknown_map_cell_side_84e804+(unsigned int)a.X)].RegionIndex;
            double nearest=9999999.0;
            for(int j=0;j<selected->Count();j++) {
                Cell b=(*selected)[j];
                int dx=(int)a.X-b.X,dy=(int)a.Y-b.Y;
                int regionB;
            if(!unknown_map_cells_a70a24)regionB=-1;
            else regionB=unknown_map_cells_a70a24[(int)((unsigned int)b.Y*(unsigned int)unknown_map_cell_side_84e804+(unsigned int)b.X)].RegionIndex;
                int square=(int)((unsigned int)dx*(unsigned int)dx+(unsigned int)dy*(unsigned int)dy);
                double distance=(double)Fast_Sqrt((double)square)+(double)(regionB!=regionA ? 20 : 0);
                if(distance<nearest)nearest=distance;
            }
            if(nearest>best) { best=nearest;next=i; }
        }
        selected->Add(candidates[next]);
    }
    return selected;
}


unsigned __fastcall Map_Random_Range_005797a0(unsigned,unsigned);
namespace seed_0056dac0 {
bool __fastcall unknown_generation_cell_clear_0056dac0(short *position)
{
    unsigned index=(unsigned)(int)position[1]*(unsigned)unknown_map_cell_side_84e804+(unsigned)(int)position[0];
    return unknown_map_cells_a70a24[index].unknown_45[0]==0;
}
}
void MapRegionClass::Place_Neutral_Tech_Building_00576cd0()
{
    HouseClass *neutral=HouseClass::As_Pointer(HouseTypeClass::Find_Index("Neutral"));
    bool placed=false;
    int tries=0;
    BuildingClass *building=new BuildingClass(Rules->NeutralTechBuildings[Map_Random_Range_005797a0(0,Rules->NeutralTechBuildings.Count()-1)],neutral);
    while(!placed) {
        if(tries>=100)break;
        Cell position=Cells[Map_Random_Range_005797a0(0,Cells.Count()-1)];
        int height=Map[position].unknown_11b;
        Cell const *list=building->Type->Occupy_List(1);
        bool clear=true;
        while(list->X!=0x7fff || list->Y!=0x7fff) {
            Cell next=position + *list;
            CellClass *cell=&Map[next];
            if(cell->OccupierPtr!=0 || !cell->unknown_47b590() || cell->unknown_11b!=height || cell->unknown_ec==3 || !Map.Unknown_0055A740(cell,true) || !seed_0056dac0::unknown_generation_cell_clear_0056dac0((short *)&cell->MapCoords))clear=false;
            ++list;
            if(!clear)break;
        }
        if(clear) {
            if(building->Unlimbo(&CoordStruct(position.X*256+128,position.Y*256+128,0),0))placed=true;
        }
        ++tries;
    }
    if(!placed)delete building;
}


extern DynamicVectorClass<MapRegionClass *> unknown_map_regions_a6fd28;
MapGeneratorClass::~MapGeneratorClass()
{
    MapRegionClass::Destroy_Cell_Regions();
    if(unknown_map_cells_a70a24!=0) {
        delete[] unknown_map_cells_a70a24;
        unknown_map_cells_a70a24=0;
    }
    if(MapSeeder!=0) {
        delete reinterpret_cast<MapSeedClass *>(MapSeeder);
        MapSeeder=0;
    }
    if(MapPreview!=0) {
        delete reinterpret_cast<MapPreviewClass *>(MapPreview);
        MapPreview=0;
    }
}


class Surface;
int __fastcall Write_PCX_File(FileClass &file,Surface &picture,PaletteClass *palette);
void __fastcall unknown_dialog_5ff7c0(HWND window);
void __fastcall unknown_dialog_5ff6e0(HWND window);
void __fastcall unknown_517370(bool flag);
int __stdcall Map_Seed_Dialog_Proc_00577bd0(HWND,UINT,WPARAM,LPARAM);
bool Default_Map_Generation_Callback_00482100();
extern bool (*rmg_callback_7e09c4)();
extern char const rmg_preview_filename_7e1540[];
// Free fastcall ECXcallback/EAXresult/RET0, independently observed. Callback bool intent follows AL0 at482100.
int __fastcall Do_Random_Map_Dialog_00577490(bool (*callback)())
{
    int result=0;
    RandomMapGen.Read_Settings_00579960();
    HWND dialog=unknown_create_dialog_5ff610(0x105,Map_Seed_Dialog_Proc_00577bd0,0);
    if(dialog!=0) {
        rmg_callback_7e09c4=callback;
        RandomMapGen.SeedData.unknown_14=(int)callback;
        SetWindowLongA(dialog,8,(LONG)&result);
        SendMessageA(dialog,0x4a9,0,1);
        unknown_dialog_5ff7c0(dialog);
        while(result==0) {
            if(unknown_6000d0()==true)break;
            if(callback!=0)callback();
            unknown_517370(false);
        }
        unknown_dialog_5ff6e0(dialog);
    }
    rmg_callback_7e09c4=Default_Map_Generation_Callback_00482100;
    RandomMapGen.SeedData.unknown_14=0;
    if(unknown_preview_a6fee4!=0) {
        if(unknown_preview_a6fee4->unknown_00!=0) {
            RawFileClass file(rmg_preview_filename_7e1540);
            Write_PCX_File(file,*static_cast<Surface *>(unknown_preview_a6fee4->unknown_00),&unknown_palette_837e10);
        }
        delete unknown_preview_a6fee4;
        unknown_preview_a6fee4=0;
    }
    if(RandomMapGen.MapSeeder!=0) {
        delete reinterpret_cast<MapSeedClass *>(RandomMapGen.MapSeeder);
        RandomMapGen.MapSeeder=0;
    }
    return result;
}


WIN32_IMPORT LONG WINAPI CompareFileTime(FILETIME const *first,FILETIME const *second);
extern char const rmg_cache_pattern_7e15c4[];
// Prune cached map previews by last access, creation, then write time.
void __fastcall Clean_Up_RMCache_005775b0()
{
    WIN32_FIND_DATA *record;
    DynamicVectorClass<WIN32_FIND_DATA *> files;
    record=new WIN32_FIND_DATA;
    HANDLE search=FindFirstFileA(rmg_cache_pattern_7e15c4,record);
    if(search!=INVALID_HANDLE_VALUE) {
        files.Add(record);
        record=new WIN32_FIND_DATA;
        while(FindNextFileA(search,record)!=0) {
            files.Add(record);
            record=new WIN32_FIND_DATA;
        }
        FindClose(search);
    }
    delete record;
    while(files.Count()>70) {
        FILETIME oldestTime;
        oldestTime.dwLowDateTime=0xffffffff;
        oldestTime.dwHighDateTime=0x7fffffff;
        int oldest=-1;
        for(int index=0;index<files.Count();++index) {
            WIN32_FIND_DATA *file=files[index];
            if(file->ftLastAccessTime.dwHighDateTime!=0) {
                if(CompareFileTime(&file->ftLastAccessTime,&oldestTime)==-1) {
                    oldest=index;
                    oldestTime.dwLowDateTime=files[index]->ftLastAccessTime.dwLowDateTime;
                    oldestTime.dwHighDateTime=files[index]->ftLastAccessTime.dwHighDateTime;
                }
            } else if(file->ftCreationTime.dwHighDateTime!=0) {
                if(CompareFileTime(&file->ftCreationTime,&oldestTime)==-1) {
                    oldest=index;
                    oldestTime.dwLowDateTime=files[index]->ftCreationTime.dwLowDateTime;
                    oldestTime.dwHighDateTime=files[index]->ftCreationTime.dwHighDateTime;
                }
            } else if(file->ftLastWriteTime.dwHighDateTime!=0) {
                if(CompareFileTime(&file->ftLastWriteTime,&oldestTime)==-1) {
                    oldest=index;
                    oldestTime.dwLowDateTime=files[index]->ftLastWriteTime.dwLowDateTime;
                    oldestTime.dwHighDateTime=files[index]->ftLastWriteTime.dwHighDateTime;
                }
            }
        }
        if(oldest==-1)break;
        if(!DeleteFileA(files[oldest]->cFileName))break;
        delete files[oldest];
        files.Delete(oldest);
    }
    while(files.Count()!=0) {
        delete files[0];
        files.Delete(0);
    }
}


class Surface;
int __fastcall Write_PCX_File(FileClass &,Surface &,PaletteClass *);
void __fastcall unknown_517370(bool);
char * __fastcall Random_Map_CRC_005c8420();
void __fastcall Clean_Up_RMCache_005775b0();
extern bool (*rmg_callback_7e09c4)();
extern char const rmg_preview_filename_7e1540[];
extern char const rmg_cache_prefix_7e15f0[];
extern char const rmg_cache_suffix_7e15e8[];
extern char const rmg_cache_directory_7e15d4[];
extern char const rmg_description_7e15dc[];
extern "C" {
__declspec(dllimport) BOOL __stdcall CopyFileA(char const *,char const *,BOOL);
void *__cdecl memset(void *,int,unsigned);
void *__cdecl memcpy(void *,void const *,unsigned);
char *__cdecl strncpy(char *,char const *,unsigned);
char *__cdecl strncat(char *,char const *,unsigned);
}
// ECX dialog/EDX callback, no stack arguments, plain RET. Name from reference operation; callback intent qualified.
void __fastcall Do_Random_Map_00577880(HWND dialog,bool (*callback)())
{
    rc_ptr<WDTTerritory> territory;
    if(unknown_game_mode_a3d298==4 && unknown_a3d2a4==2)territory=Unknown_Territory_00769690(unknown_session_a3d2a0);
    if(territory.Get()!=0)RandomMapGen.SeedData.NumPlayers=4;
    char *digest=Random_Map_CRC_005c8420();
    char name[128];
    memset(name,0,sizeof(name));
    strncpy(name,rmg_cache_prefix_7e15f0,sizeof(name));
    strncat(name,digest,sizeof(name));
    delete digest;
    strncat(name,rmg_cache_suffix_7e15e8,sizeof(name));
    CCFileClass cached(name);
    if(cached.Is_Available()) {
        if(unknown_preview_a6fee4==0)unknown_preview_a6fee4=new MapPreviewClass;
        if(unknown_preview_a6fee4->Read_PCX_Preview(name)) {
            RawFileClass file(rmg_preview_filename_7e1540);
            Write_PCX_File(file,*static_cast<Surface *>(unknown_preview_a6fee4->unknown_00),&unknown_palette_837e10);
            delete unknown_preview_a6fee4;
            unknown_preview_a6fee4=0;
            return;
        }
        delete unknown_preview_a6fee4;
        unknown_preview_a6fee4=0;
    }
    rmg_callback_7e09c4=callback;
    RandomMapGen.SeedData.unknown_14=(int)callback;
    if(RandomMapGen.SeedData.Seed==-1)
        RandomMapGen.SeedData.Seed=unknown_random_839218(0,65535);
    RandomMapGen.Unknown_0057A0D0(1,(int)dialog);
    unknown_preview_a6fee4->Create_Preview();
    if(unknown_game_mode_a3d298==4 && unknown_a3d2a4==2)unknown_preview_a6fee4->unknown_005c7ef0();
    if(RandomMapGen.MapSeeder!=0)delete reinterpret_cast<MapSeedClass *>(RandomMapGen.MapSeeder);
    RandomMapGen.MapSeeder=(int)new MapSeedClass;
    memcpy((void*)RandomMapGen.MapSeeder,&RandomMapGen.SeedData,sizeof(MapSeedClass));
    unknown_id_source_a3d290->Set_Scenario_Name_0065e730(rmg_description_7e15dc);
    unknown_517370(false);
    if(unknown_preview_a6fee4!=0) {
        if(unknown_preview_a6fee4->unknown_00!=0) {
            RawFileClass file(rmg_preview_filename_7e1540);
            Write_PCX_File(file,*static_cast<Surface *>(unknown_preview_a6fee4->unknown_00),&unknown_palette_837e10);
            WIN32_FIND_DATA find;
            if(FindFirstFileA(rmg_cache_directory_7e15d4,&find)==(HANDLE)-1)CreateDirectoryA(rmg_cache_directory_7e15d4,0);
            CopyFileA(rmg_preview_filename_7e1540,name,0);
            Clean_Up_RMCache_005775b0();
        }
        delete unknown_preview_a6fee4;
        unknown_preview_a6fee4=0;
    }
    if(RandomMapGen.MapSeeder!=0) {
        delete reinterpret_cast<MapSeedClass *>(RandomMapGen.MapSeeder);
        RandomMapGen.MapSeeder=0;
    }
}


// Map-seed dialog callback; message and control names follow the original dispatch.

// Existing OwnerDraw default dialog procedure, declared here to avoid unrelated mouse statics.
int __fastcall unknown_dialog_proc_5ffb00(HWND,UINT,WPARAM,LPARAM);
extern int unknown_selection_7e0adc;
extern bool unknown_preview_flag_7e0ae0;
extern bool unknown_generate_flag_a70068;
extern char const unknown_description_label_7e14dc[];
extern char const unknown_mapgen_file_7e14f8[];
extern char const unknown_random_map_name_7e1608[];
extern char const unknown_random_map_file_7e15fc[];

// The real map-seed load-or-prompt body is visible in this original unit.
inline bool MapSeedClass::Load_Or_Prompt_005791a0(char const *filename)
{
    if(filename)return Load(filename);
    return Load_Dialog_0053b190()!=0 ? true : false;
}

int __stdcall Map_Seed_Dialog_Proc_00577bd0(HWND window,UINT message,WPARAM wparam,LPARAM lparam)
{
    int result=unknown_dialog_proc_5ffb00(window,message,wparam,lparam);
    if(result)return result;
    LONG *state=(LONG *)GetWindowLongA(window,8);
    switch(message) {
    case 0xf:
        if(unknown_preview_a6fee4) {
            HWND handle=GetDlgItem(window,0x468);
            if(!OwnerDraw::unknown_5e4720(handle))unknown_preview_a6fee4->Blit_Preview(window);
        }
        if(ProgressScreen.Has_Window())ProgressScreen.Draw_It(Point2D(-1,-1));
        ValidateRect(window,0);
        return 0;
    case 0x114:
        if(lparam && GetDlgCtrlID((HWND)lparam)==0x3eb) {
            HWND handle=GetDlgItem(window,0x6c5);
            if(handle)EnableWindow(handle,0);
            handle=GetDlgItem(window,0x6c3);
            if(handle)EnableWindow(handle,0);
            RandomMapGen.SeedData.Read_Dialog_Parameters_00578420(window);
            InvalidateRect(window,0,0);
            return 0;
        }
        break;
    case 0x111:
        switch((int)(wparam & 0xffff)) {
        case 0x5c0:
            *state=2;
            return 1;
        case 0x3ea: case 0x405: case 0x406: case 0x407: case 0x408:
            if((unsigned short)(wparam>>16)==1) {
                HWND handle=GetDlgItem(window,0x6c5);
                if(handle)EnableWindow(handle,0);
                handle=GetDlgItem(window,0x6c3);
                if(handle)EnableWindow(handle,0);
                RandomMapGen.SeedData.Read_Dialog_Parameters_00578420(window);
                InvalidateRect(window,0,0);
                return 0;
            }
            break;
        case 0x620:
            WWMouse->Hide_Mouse();
            if(unknown_preview_flag_7e0ae0)unknown_generate_flag_a70068=true;
            else {
                unknown_generate_flag_a70068=false;
                unknown_preview_flag_7e0ae0=true;
            }
            RandomMapGen.SeedData.Read_Dialog_Parameters_00578420(window);
            EnableWindow(GetDlgItem(window,0x405),0);
            EnableWindow(GetDlgItem(window,0x3ea),0);
            EnableWindow(GetDlgItem(window,0x407),0);
            EnableWindow(GetDlgItem(window,0x406),0);
            EnableWindow(GetDlgItem(window,0x408),0);
            EnableWindow(GetDlgItem(window,0x3eb),0);
            EnableWindow(GetDlgItem(window,0x621),0);
            EnableWindow(GetDlgItem(window,0x620),0);
            EnableWindow(GetDlgItem(window,0x6c2),0);
            EnableWindow(GetDlgItem(window,0x6c3),0);
            EnableWindow(GetDlgItem(window,0x6c4),0);
            EnableWindow(GetDlgItem(window,0x6c5),0);
            EnableWindow(GetDlgItem(window,0x5c0),0);
            wcscpy(RandomMapGen.SeedData.DescriptionBuffer,
                StringTable::Load_String(unknown_description_label_7e14dc,0,unknown_mapgen_file_7e14f8,0xed1));
            RandomMapGen.Unknown_0057A0D0(1,(int)window);
            unknown_preview_a6fee4->Create_Preview();
            EnableWindow(GetDlgItem(window,0x405),1);
            EnableWindow(GetDlgItem(window,0x3ea),1);
            EnableWindow(GetDlgItem(window,0x407),1);
            EnableWindow(GetDlgItem(window,0x406),1);
            EnableWindow(GetDlgItem(window,0x408),1);
            EnableWindow(GetDlgItem(window,0x3eb),1);
            EnableWindow(GetDlgItem(window,0x621),1);
            EnableWindow(GetDlgItem(window,0x620),1);
            EnableWindow(GetDlgItem(window,0x6c2),1);
            EnableWindow(GetDlgItem(window,0x6c3),1);
            EnableWindow(GetDlgItem(window,0x6c4),1);
            EnableWindow(GetDlgItem(window,0x6c5),1);
            EnableWindow(GetDlgItem(window,0x5c0),1);
            if(RandomMapGen.MapSeeder)delete (MapSeedClass *)RandomMapGen.MapSeeder;
            RandomMapGen.MapSeeder=(int)new MapSeedClass;
            memcpy((void *)RandomMapGen.MapSeeder,&RandomMapGen.SeedData,sizeof(MapSeedClass));
            WWMouse->Show_Mouse();
            WWKeyboard->Clear();
            PostMessageA(window,0xf,0,0);
            return 0;
        case 0x621: {
            RandomMapGen.SeedData.Read_Dialog_Parameters_00578420(window);
            RandomMapGen.SeedData.Populate_Dialog_Parameters_00578600(window);
            RandomMapGen.SeedData.Theater=((unsigned int)unknown_random_839218(0,100)>=50);
            RandomMapGen.SeedData.MapType=unknown_random_839218(1,4);
            RandomMapGen.SeedData.Time=unknown_random_839218(0,3);
            RandomMapGen.SeedData.Resources=unknown_random_839218(0,3);
            RandomMapGen.SeedData.Width=unknown_random_839218(0,3);
            RandomMapGen.SeedData.Height=RandomMapGen.SeedData.Width;
            RandomMapGen.SeedData.Randomize_Parameters_005789f0(RandomMapGen.SeedData.MapType);
            wcscpy(RandomMapGen.SeedData.DescriptionBuffer,
                StringTable::Load_String(unknown_description_label_7e14dc,0,unknown_mapgen_file_7e14f8,0xed1));
            RandomMapGen.SeedData.Seed=unknown_random_839218(0,65535);
            RandomMapGen.SeedData.Clamp_Parameters_00578d80();
            RandomMapGen.SeedData.Populate_Dialog_Parameters_00578600(window);
            delete unknown_preview_a6fee4;
            unknown_preview_a6fee4=0;
            HWND handle=GetDlgItem(window,0x6c5);
            if(handle)EnableWindow(handle,0);
            handle=GetDlgItem(window,0x6c3);
            if(handle)EnableWindow(handle,0);
            InvalidateRect(window,0,0);
            return 0;
        }
        case 0x6c3: {
            RandomMapGen.SeedData.Read_Dialog_Parameters_00578420(window);
            RandomMapGen.SeedData.Save_Dialog_0053b1b0(RandomMapGen.SeedData.DescriptionBuffer);
            bool enable=RandomMapGen.SeedData.Has_Saved_Games_0053c5c0();
            HWND handle=GetDlgItem(window,0x6c2);
            if(handle)EnableWindow(handle,enable);
            handle=GetDlgItem(window,0x6c4);
            if(handle==0)return 0;
            EnableWindow(handle,enable);
            return 0;
        }
        case 0x6c2: {
            RandomMapGen.SeedData.Read_Dialog_Parameters_00578420(window);
            bool loaded=RandomMapGen.SeedData.Load_Or_Prompt_005791a0(0);
            if(loaded) {
                unknown_preview_flag_7e0ae0=false;
                PostMessageA(window,0x111,0x620,(LPARAM)GetDlgItem(window,0x620));
            }
            while(WWMouse->Get_Mouse_State()!=0)WWMouse->Show_Mouse();
            RandomMapGen.SeedData.Populate_Dialog_Parameters_00578600(window);
            return 0;
        }
        case 0x6c4: {
            RandomMapGen.SeedData.Read_Dialog_Parameters_00578420(window);
            RandomMapGen.SeedData.Delete_Dialog_0053b1e0();
            bool enable=RandomMapGen.SeedData.Has_Saved_Games_0053c5c0();
            HWND handle=GetDlgItem(window,0x6c2);
            if(handle)EnableWindow(handle,enable);
            handle=GetDlgItem(window,0x6c4);
            if(handle==0)return 0;
            EnableWindow(handle,enable);
            return 0;
        }
        case 0x6c5:
            RandomMapGen.SeedData.Read_Dialog_Parameters_00578420(window);
            if(Debug_Map) {
                RandomMapGen.Unknown_0057A0D0(0,(int)window);
                unknown_id_source_a3d290->Set_Scenario_Name_0065e730(unknown_random_map_name_7e1608);
                Write_Scenario_INI_0065f980(unknown_random_map_file_7e15fc,true);
            } else if(!unknown_preview_a6fee4 || !unknown_preview_a6fee4->Is_Present()) {
                RandomMapGen.Unknown_0057A0D0(1,(int)window);
                if(Debug_Map) {
                    unknown_id_source_a3d290->Set_Scenario_Name_0065e730(unknown_random_map_name_7e1608);
                    Write_Scenario_INI_0065f980(unknown_random_map_file_7e15fc,true);
                }
            }
            *state=1;
            return 1;
        }
        break;
    case 0x497: {
        unknown_selection_7e0adc=-1;
        HWND preview_handle=GetDlgItem(window,0x620);
        if(Debug_Map)EnableWindow(preview_handle,0);
        else EnableWindow(preview_handle,1);
        if(RandomMapGen.SeedData.Seed==-1) {
            RandomMapGen.SeedData.Seed=unknown_id_source_a3d290->Random()(0,65535);
            unknown_generate_flag_a70068=true;
        }
        unknown_preview_flag_7e0ae0=true;
        RandomMapGen.SeedData.Populate_Dialog_Parameters_00578600(window);
        bool enable=RandomMapGen.SeedData.Has_Saved_Games_0053c5c0();
        HWND handle=GetDlgItem(window,0x6c2);
        if(handle)EnableWindow(handle,enable);
        handle=GetDlgItem(window,0x6c4);
        if(handle)EnableWindow(handle,enable);
        handle=GetDlgItem(window,0x6c5);
        if(handle)EnableWindow(handle,0);
        handle=GetDlgItem(window,0x6c3);
        if(handle)EnableWindow(handle,0);
        return 0;
    }
    }
    return 0;
}


extern char const *unknown_time_labels_7e0b10[4];
extern char const *unknown_size_labels_7e0b20[4];
extern char const *unknown_map_type_labels_7e0ae4[5];
extern char const *unknown_resource_labels_7e0af8[4];
extern char unknown_seed_text_a709c8[];
void MapSeedClass::Populate_Dialog_Parameters_00578600(HWND dialog)
{
    Clamp_Parameters_00578d80();
    {
        HWND control=GetDlgItem(dialog,0x3ea);
        while(SendMessageA(control,0x146,0,0)>0) SendMessageA(control,0x144,0,0);
        for(int i=0;i<4;++i) {
            LRESULT item=SendMessageA(control,0x4c2,0,(LPARAM)StringTable::Load_String(unknown_time_labels_7e0b10[i],0,"D:\\Ra2\\MapGen.cpp",0x1277));
            SendMessageA(control,0x151,item,i);
        }
        LRESULT selected=SendMessageA(control,0x4be,0,(LPARAM)StringTable::Load_String(unknown_time_labels_7e0b10[Time],0,"D:\\Ra2\\MapGen.cpp",0x127b));
        SendMessageA(control,0x14e,selected,0);
    }
    {
        HWND control=GetDlgItem(dialog,0x406);
        while(SendMessageA(control,0x146,0,0)>0) SendMessageA(control,0x144,0,0);
        for(int i=0;i<4;++i) {
            LRESULT item=SendMessageA(control,0x4c2,0,(LPARAM)StringTable::Load_String(unknown_size_labels_7e0b20[i],0,"D:\\Ra2\\MapGen.cpp",0x1286));
            SendMessageA(control,0x151,item,i);
        }
        LRESULT selected=SendMessageA(control,0x4be,0,(LPARAM)StringTable::Load_String(unknown_size_labels_7e0b20[Width],0,"D:\\Ra2\\MapGen.cpp",0x128a));
        SendMessageA(control,0x14e,selected,0);
    }
    {
        HWND control=GetDlgItem(dialog,0x405);
        while(SendMessageA(control,0x146,0,0)>0) SendMessageA(control,0x144,0,0);
        for(int i=1;i<5;++i) {
            LRESULT item=SendMessageA(control,0x4c2,0,(LPARAM)StringTable::Load_String(unknown_map_type_labels_7e0ae4[i],0,"D:\\Ra2\\MapGen.cpp",0x1296));
            SendMessageA(control,0x151,item,i);
        }
        LRESULT selected=SendMessageA(control,0x4be,0,(LPARAM)StringTable::Load_String(unknown_map_type_labels_7e0ae4[MapType],0,"D:\\Ra2\\MapGen.cpp",0x129a));
        SendMessageA(control,0x14e,selected,0);
    }
    {
        HWND control=GetDlgItem(dialog,0x407);
        while(SendMessageA(control,0x146,0,0)>0) SendMessageA(control,0x144,0,0);
        for(int i=0;i<2;++i) {
            LRESULT item=SendMessageA(control,0x4c2,0,(LPARAM)StringTable::Load_String(Theaters[i].UIName,0,"D:\\Ra2\\MapGen.cpp",0x12a6));
            SendMessageA(control,0x151,item,i);
        }
        LRESULT selected=SendMessageA(control,0x4be,0,(LPARAM)StringTable::Load_String(Theaters[Theater].UIName,0,"D:\\Ra2\\MapGen.cpp",0x12aa));
        SendMessageA(control,0x14e,selected,0);
    }
    {
        HWND control=GetDlgItem(dialog,0x408);
        while(SendMessageA(control,0x146,0,0)>0) SendMessageA(control,0x144,0,0);
        for(int i=0;i<4;++i) {
            LRESULT item=SendMessageA(control,0x4c2,0,(LPARAM)StringTable::Load_String(unknown_resource_labels_7e0af8[i],0,"D:\\Ra2\\MapGen.cpp",0x12b5));
            SendMessageA(control,0x151,item,i);
        }
        LRESULT selected=SendMessageA(control,0x4be,0,(LPARAM)StringTable::Load_String(unknown_resource_labels_7e0af8[Resources],0,"D:\\Ra2\\MapGen.cpp",0x12b9));
        SendMessageA(control,0x14e,selected,0);
    }
    HWND seed=GetDlgItem(dialog,0x3fb);
    sprintf(unknown_seed_text_a709c8,"%d",Seed);
    SendMessageA(seed,0x4b4,0,(LPARAM)unknown_seed_text_a709c8);
    HWND players=GetDlgItem(dialog,0x3eb);
    SendMessageA(players,0x406,1,0x80002);
    SendMessageA(players,0x405,1,NumPlayers);
}


class WDTTerritory;
template <> bool VectorClass<rc_ptr<WDTTerritory> >::Resize(int newsize, rc_ptr<WDTTerritory> const *array);
rc_ptr<WDTTerritory> __fastcall Get_WDT_Territory_00769690(int value);
void MapSeedClass::Apply_WDT_Territory_00578bc0()
{
    rc_ptr<WDTTerritory> territory;
    if(unknown_game_mode_a3d298==4 && *(int const*)&unknown_session_a3d298.unknown_00[0x0c]==2)
        territory=Get_WDT_Territory_00769690(*(int const*)&unknown_session_a3d298.unknown_00[8]);
    if(territory.Get()) {
        unsigned short fixed_seed=*(unsigned short const*)territory->unknown_0c;
        Seed=fixed_seed==0 ? unknown_random_839218(0,0xffff) : fixed_seed;
        switch(territory->unknown_12) {
        case 0: Width=0; break;
        case 2: Width=2; break;
        case 3: Width=3; break;
        case 1: Width=1; break;
        default: Width=1; break;
        }
        switch(territory->unknown_13) {
        case 0: Height=0; break;
        case 2: Height=2; break;
        case 3: Height=3; break;
        case 1: Height=1; break;
        default: Height=1; break;
        }
        NumPlayers=4;
        switch((unsigned char)territory->unknown_15[0x1e-0x15]) {
        case 2: Time=0; break;
        case 1: Time=2; break;
        case 3: Time=3; break;
        case 0: Time=1; break;
        default: Time=1; break;
        }
        MapType=(unsigned char)territory->unknown_15[0x1c-0x15];
        Theater=(unsigned char)territory->unknown_15[0x1d-0x15];
        RegionSize=(unsigned char)territory->unknown_15[0x21-0x15];
        Accessibility=(unsigned char)territory->unknown_15[0x24-0x15];
        Ruggedness=(unsigned char)territory->unknown_15[0x27-0x15];
        WaterAmount=(unsigned char)territory->unknown_15[0x2a-0x15];
        Tiberium=(unsigned char)territory->unknown_15[0x2d-0x15];
        TiberiumLayout=(unsigned char)territory->unknown_15[0x30-0x15];
        Vegetation=(unsigned char)territory->unknown_15[0x33-0x15];
        UrbanPresence=(unsigned char)territory->unknown_15[0x36-0x15];
        Resources=*(int const*)&territory->unknown_15[0x18-0x15];
    }
}

// Real original auxiliary: VectorClass<rc_ptr<WDTTerritory>>::Resize
// RTTI table7AF2E8 slot2=72CBD0, zero source credit.
// Ordinary definition of the real RTTI-rooted Resize specialization. Its
// body is the unchanged shared implementation; no whole-class instantiation.
template <>
bool VectorClass<rc_ptr<WDTTerritory> >::Resize(int newsize, rc_ptr<WDTTerritory> const *array)
{
    typedef rc_ptr<WDTTerritory> T;
    if (newsize) {
        T *newptr;
        IsValid = false;
        if (!array) {
            newptr = new T[newsize];
        } else {
            newptr = new ((void *)array) T[newsize];
        }
        IsValid = true;
        if (!newptr) {
            return false;
        }
        if (Vector) {
            int copycount = (newsize < VectorMax) ? newsize : VectorMax;
            for (int index = 0; index < copycount; index++) {
                newptr[index] = Vector[index];
            }
            if (IsAllocated) {
                delete[] Vector;
                Vector = 0;
            }
        }
        Vector = newptr;
        VectorMax = newsize;
        IsAllocated = (Vector && !array);
    } else {
        Clear();
    }
    return true;
}


// Original5794C0 tail-forwards the unchanged complete receiver to53B1E0.
// Return spelling follows the existing base declaration; char/bool source intent remains a hypothesis.
char MapSeedClass::Delete_Dialog_005794c0()
{
    return LoadOptionsClass::Delete_Dialog_0053b1e0();
}


double __stdcall Map_Clamped_Gaussian_005798f0(double mean,double deviation,double minimum,double maximum)
{
    if(mean-deviation>maximum || mean+deviation<minimum) {
        deviation=(maximum-minimum)*0.5;
        mean=deviation+minimum;
    }
    double value;
    do {
        value=unknown_map_gaussian_a6fd50.Next()*deviation+mean;
    } while(value<minimum || value>maximum);
    return value;
}


extern void (__cdecl *unknown_map_generation_callback_7e09c4)();
extern DynamicVectorClass<MapRegionClass *> unknown_map_regions_a6fd28;
extern int unknown_a3d2a4;
bool __cdecl Generate_Starting_Points_00576420();
double __cdecl Map_Random_Fraction_00579770();
void __cdecl Make_Water_Regions_0056e890();
void __fastcall Make_Land_Regions_0056e910(bool flag);
void __fastcall Initialize_Vegetation_Weights_005850a0(MapGeneratorClass *generator);
void Initialize_Grid_Thresholds_00585240();
extern "C" char unknown_rmg_message_007e19b4[];
extern "C" char unknown_rmg_message_007e19a0[];
extern "C" char unknown_rmg_message_007e198c[];
extern "C" char unknown_rmg_message_007e1974[];
extern "C" char unknown_rmg_message_007e1950[];
extern "C" char unknown_rmg_message_007e1930[];
extern "C" char unknown_rmg_message_007e1914[];
extern "C" char unknown_rmg_message_007e18fc[];
extern "C" char unknown_rmg_message_007e18e4[];
extern "C" char unknown_rmg_message_007e18c4[];
extern "C" char unknown_rmg_message_007e18b4[];
extern "C" char unknown_rmg_message_007e1898[];
extern "C" char unknown_rmg_message_007e188c[];
extern "C" char unknown_rmg_progress_shape_7e19cc[];
MapGaussianClass::MapGaussianClass(UniformCallback callback)
    : HasSpare(false),Spare(0.0),Uniform(callback)
{
}

extern "C" char unknown_rmg_radar_message[];
// Use the existing canonical progress operation at 0x0057BE50.
// Its accepted out-of-line copy lives in another population; this is the same real body.
void __stdcall Report_Generation_Progress_0057be50(int progress)
{
    if (unknown_id_source_a3d290->unknown_22fc) {
        unknown_session_a3d298.Loading_Progress_00672160((int)((unsigned int)progress + 99u));
    } else {
        ProgressScreen.Set_Progress_006201c0(0, (double)progress, Point2D(-1, -1));
    }
}

// Init_Regions is evidenced by its original inline site (MapRegionClass::Destroy_Cell_Regions is defined out of
// line at 0x0056E790 and expanded inline here and in the generator destructor 0x005771B0).
inline void MapGeneratorClass::Init_Regions()
{
    int index;
    MapRegionClass::Destroy_Cell_Regions();
    Make_Water_Regions_0056e890();
    for(index=0;index<unknown_map_regions_a6fd28.Count();++index) {
        MapRegionClass *region=unknown_map_regions_a6fd28[index];
        if(region->unknown_14) {
            int wetMap=SeedData.MapType==3 || SeedData.MapType==4;
            region->Grow_Border_00570040((region->CellCount>8000)+4+!(unsigned char)wetMap);
            region->Expand_Cells_005702b0();
        }
    }
    Make_Land_Regions_0056e910(false);
 }
// Existing public int,int/ECX/RET8 contract retained; low-byte flag and HWND intent are hypotheses.
void MapGeneratorClass::Unknown_0057A0D0(int fullInit,int dialogValue)
{
    HWND window=(HWND)dialogValue;
    int index;
    CellClass *cell;
    if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
    unknown_map_random_a705c0=Random2Class(SeedData.Seed);
    unknown_map_gaussian_a6fd50=MapGaussianClass(Map_Random_Fraction_00579770);
    if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
    bool showProgress=!unknown_id_source_a3d290->unknown_22fc;
    if(showProgress) {
        ProgressScreen.Init(100.0,1,window);
        ProgressScreen.Load_Shape(unknown_rmg_progress_shape_7e19cc,0,0);
        ProgressScreen.Unknown_0061EFA0(Point2D(-1,-1),0,false,true);
        ProgressScreen.Draw_It(Point2D(-1,-1));
        HWND control=GetDlgItem(window,0x638);
        if(control)ShowWindow(control,5);
    }
    if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
    Debug_Printf(unknown_rmg_message_007e19b4);
    Init_Map(fullInit);
    if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
    unknown_a40774=(int)((unsigned)unknown_a40774+1u);
    if((unsigned char)fullInit) {
        unknown_preview_a6fee4->Create_Preview();
        SendMessageA(window,15,0,0);
        if(unknown_session_a3d298.Type==4 && unknown_a3d2a4==2)unknown_preview_a6fee4->unknown_005c7ef0();
    }
    if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
    Read_Settings_00579960();
    Debug_Printf(unknown_rmg_message_007e19a0);
    if(SeedData.MapType==3 || SeedData.MapType==4) {
        if(SeedData.WaterAmount)Seed_Water();
    } else Seed_Land();
    Vary_Water_Tiles();
    Report_Generation_Progress_0057be50(55);
    if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
    if((unsigned char)fullInit) {
        unknown_preview_a6fee4->Create_Preview();
        SendMessageA(window,15,0,0);
        if(unknown_session_a3d298.Type==4 && unknown_a3d2a4==2)unknown_preview_a6fee4->unknown_005c7ef0();
    }
    if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
    Report_Generation_Progress_0057be50(60);
    if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
    if((unsigned char)fullInit) {
        unknown_preview_a6fee4->Create_Preview();
        SendMessageA(window,15,0,0);
        if(unknown_session_a3d298.Type==4 && unknown_a3d2a4==2)unknown_preview_a6fee4->unknown_005c7ef0();
    }
    if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
    Debug_Printf(unknown_rmg_message_007e198c);
    Init_Regions();
    Report_Generation_Progress_0057be50(65);
    if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
    Debug_Printf(unknown_rmg_message_007e1974);
    if(SeedData.MapType==4 || SeedData.MapType==3) {
        MapRegionClass::Make_Ice_Regions_005704c0();
        MapRegionClass::Make_Regions_00570810();
        Fixup_Cliff_Corners();
        Map.Build_All_Cliffs(0,-1);
        unknown_00582fd0();
    }
    Report_Generation_Progress_0057be50(70);
    if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
    if((unsigned char)fullInit) {
        unknown_preview_a6fee4->Create_Preview();
        SendMessageA(window,15,0,0);
        if(unknown_session_a3d298.Type==4 && unknown_a3d2a4==2)unknown_preview_a6fee4->unknown_005c7ef0();
    }
    if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
    Scatter_Green_Tiles();
    Debug_Printf(unknown_rmg_message_007e1950);
    Map.Reset_Iterator();
    for(cell=Map.Iterate();cell;cell=Map.Iterate())cell->Recalc_Attributes(-1);
    if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
    Report_Generation_Progress_0057be50(75);
    Debug_Printf(unknown_rmg_message_007e1930);
    bool starts=false;
    while(!starts) {
        starts=Generate_Starting_Points_00576420();
        if(starts)starts=Init_Start_Points();
    }
    Debug_Printf(unknown_rmg_message_007e1914);
    if(SeedData.MapType!=0)Generate_Neutral_Buildings();
    if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
    Debug_Printf(unknown_rmg_message_007e18fc);
    Create_Tiberium();
    if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();

    if(unknown_map_cells_a70a24) {
        int size=(int)((unsigned)unknown_map_cell_side_84e804*(unsigned)unknown_map_cell_side_84e804);
        for(index=0;index<size;++index) {
            unknown_map_cells_a70a24[index].RegionIndex=-1;
            unknown_map_cells_a70a24[index].unknown_3c=-1;
        }
    }
    for(index=(int)((unsigned)unknown_map_regions_a6fd28.Count()-1u);index>=0;--index) {
        MapRegionClass *region=unknown_map_regions_a6fd28[index];
        if(region)delete region;
    }
    unknown_map_region_next_index_a70a28=0;
    Debug_Printf(unknown_rmg_message_007e1950);
    Map.Reset_Iterator();
    for(cell=Map.Iterate();cell;cell=Map.Iterate())cell->Recalc_Attributes(-1);
    Report_Generation_Progress_0057be50(80);
    if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
    if((unsigned char)fullInit) {
        unknown_preview_a6fee4->Create_Preview();
        SendMessageA(window,15,0,0);
        if(unknown_session_a3d298.Type==4 && unknown_a3d2a4==2)unknown_preview_a6fee4->unknown_005c7ef0();
    }
    if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
    Report_Generation_Progress_0057be50(85);
    if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
    if((unsigned char)fullInit) {
        unknown_preview_a6fee4->Create_Preview();
        SendMessageA(window,15,0,0);
        if(unknown_session_a3d298.Type==4 && unknown_a3d2a4==2)unknown_preview_a6fee4->unknown_005c7ef0();
    }
    if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
    Debug_Printf(unknown_rmg_message_007e1950);
    Map.Reset_Iterator();
    for(cell=Map.Iterate();cell;cell=Map.Iterate())cell->Recalc_Attributes(-1);
    if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
    Debug_Printf(unknown_rmg_message_007e18e4);
    Generate_Hills();
    Report_Generation_Progress_0057be50(90);
    if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
    if((unsigned char)fullInit) {
        unknown_preview_a6fee4->Create_Preview();
        SendMessageA(window,15,0,0);
        if(unknown_session_a3d298.Type==4 && unknown_a3d2a4==2)unknown_preview_a6fee4->unknown_005c7ef0();
    }
    if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
    Debug_Printf(unknown_rmg_message_007e18c4);
    if(SeedData.Theater==0) {
        Initialize_Vegetation_Weights_005850a0(this);
        Generate_Vegetation();
    } else {
        Initialize_Grid_Thresholds_00585240();
        Generate_Arctic_Vegetation();
    }
    if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
    Report_Generation_Progress_0057be50(95);
    if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
    if((unsigned char)fullInit) {
        unknown_preview_a6fee4->Create_Preview();
        SendMessageA(window,15,0,0);
        if(unknown_session_a3d298.Type==4 && unknown_a3d2a4==2)unknown_preview_a6fee4->unknown_005c7ef0();
    }
    unknown_a40774=(int)((unsigned)unknown_a40774-1u);
    if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
    Debug_Printf(unknown_rmg_message_007e1950);
    Map.Reset_Iterator();
    for(cell=Map.Iterate();cell;cell=Map.Iterate())cell->Recalc_Attributes(-1);
    if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
    TiberiumClass::Init_Growth();
    TiberiumClass::Init_Spread();
    Debug_Printf(unknown_rmg_message_007e18b4);
    SeededWaterAmount=0;
    WorkingRegionID=0;
    if(unknown_map_cells_a70a24!=0) {
        delete [] unknown_map_cells_a70a24;
        unknown_map_cells_a70a24=0;
    }
    for(index=(int)((unsigned int)unknown_map_regions_a6fd28.Count()-1u);index>=0;--index) {
        MapRegionClass *region=unknown_map_regions_a6fd28[index];
        if(region!=0)delete region;
    }
    unknown_map_region_next_index_a70a28=0;
    Map.Overpass();
    Debug_Printf(unknown_rmg_radar_message);
    Map.RadarClass::Set_Local_Dimensions(Map.MapLocalSize);
    Map.Compute_Radar_Image();
    Report_Generation_Progress_0057be50(100);
    Debug_Printf(unknown_rmg_message_007e188c);
    if(showProgress) {
        HWND control=GetDlgItem(window,0x638);
        if(control)ShowWindow(control,0);
        Report_Generation_Progress_0057be50(0);
        ProgressScreen.Clear_Window();
    }
}


// Complete original 0x57AE00..0x57BE4E, followed by one frozen NOP.
// Source intent follows target operations; int/low-byte full-init ABI is retained.

extern void (__cdecl *unknown_map_generation_callback_7e09c4)();
extern int const unknown_width_min_7a5b6c[7], unknown_width_max_7a5b88[7];
extern int const unknown_height_min_7a5ba4[7], unknown_height_max_7a5bc0[7];
extern float const unknown_waterfall_probability_7a5b68;
extern DynamicVectorClass<MapRegionClass *> unknown_map_regions_a6fd28;
// Existing scenario waypoint-clear operation; canonical provider identity/ABI retained.
namespace seed_00663220 { void __fastcall Clear_All_Waypoints_00663220(int address); }
void __stdcall Report_Generation_Progress_0057be50(int progress);
void __fastcall Delete_All_Objects_00519960();
void Clear_Tiberium_Spread_006e9670();
void Clear_Tiberium_Growth_006ea130();
void __fastcall Read_Tile_Control_00529870(int theater,bool reload);
void Clear_Tile_Use_Counts_0052b650();
void __fastcall Load_Isometric_Tiles_0052b2e0(bool first,bool second);
extern "C" unsigned int __cdecl wcslen(wchar_t const *text);
extern "C" wchar_t *__cdecl wcscpy(wchar_t *target,wchar_t const *text);

double __cdecl Map_Random_Fraction_00579770();
// Existing region teardown operation, whose accepted separate copy remains authoritative.
void __fastcall Clear_Map_Regions_0056da00()
{
    unknown_map_region_next_index_a70a28=0;
    for (int index=(int)((unsigned)unknown_map_regions_a6fd28.Count()-1u);index>=0;--index) {
        delete unknown_map_regions_a6fd28[index];
    }
}

void MapGeneratorClass::Init_Map(int fullInit)
{
    int index;
    int sizeIndex=SeedData.NumPlayers-2;
    float widthFraction=SeedData.Width*(1.0f/3.0f);
    float heightFraction=SeedData.Height*(1.0f/3.0f);
    Read_Settings_00579960();
    if(SeedData.MapType!=3 && SeedData.MapType!=4) {
        widthFraction=widthFraction<1.2 ? widthFraction : 1.2;
        heightFraction=heightFraction<1.2 ? heightFraction : 1.2;
    }
    LocalWidth=(int)(unknown_width_min_7a5b6c[sizeIndex]*(1.0f-widthFraction)+unknown_width_max_7a5b88[sizeIndex]*widthFraction);
    LocalHeight=(int)(unknown_height_min_7a5ba4[sizeIndex]*(1.0f-heightFraction)+unknown_height_max_7a5bc0[sizeIndex]*heightFraction);
    Rect size(0,0,LocalWidth+4,LocalHeight+12);
    Rect localSize(2,5,LocalWidth,LocalHeight);
    CellHeight=4;
    CCINIClass ini;
    ini.Clear();
    CellHeight=4;
    ini.Put_String("Map","Theater",Theaters[SeedData.Theater].Name);
    ini.Put_Rect("Map","Size",*(INIRect const*)&size);
    ini.Put_Rect("Map","LocalSize",*(INIRect const*)&localSize);
    ini.Put_Int("Map","Level",CellHeight,0);
    ini.Put_String(HouseTypes[0]->ID,"TechLevel","0");
    ini.Put_String("Basic","Player",HouseTypes[0]->ID);
    double ambient=(SeedData.Theater==0 ? TemperateAmbientLight[SeedData.Time] : SnowAmbientLight[SeedData.Time])*0.01;
    ini.Put_Double("Lighting","Ambient",ambient);
    ini.Put_Double("Lighting","RedTint",1.0);
    ini.Put_Double("Lighting","GreenTint",1.0);
    ini.Put_Double("Lighting","BlueTint",1.0);
    ini.Put_Double("Lighting","Ground",0.0);
    ini.Put_Double("Lighting","Level",(double)(RMGLevelLightSettings[SeedData.Time]/100));
    ini.Put_Double("Lighting","IonAmbient",ambient);
    ini.Put_Double("Lighting","IonRed",0.3);
    ini.Put_Double("Lighting","IonGreen",0.4);
    ini.Put_Double("Lighting","IonBlue",0.75);
    ini.Put_Double("Lighting","IonGround",0.0);
    ini.Put_Double("Lighting","IonLevel",0.0);
    if(!unknown_id_source_a3d290->unknown_22fc)ProgressScreen.Set_Progress_006201c0(0,1.0,Point2D(-1,-1));
    if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
    Clear_Tiberium_Spread_006e9670();
    Clear_Tiberium_Growth_006ea130();
    if(!(unsigned char)fullInit) {
        unknown_a40774=(int)((unsigned)unknown_a40774+1u);
        unknown_id_source_a3d290->Set_Scenario_Name_0065e730("");
        if(Debug_Map)Clear_Scenario_0065d370();
        Read_Scenario_INI(ini,true);
        Fill_In_Data_0065cdd0();
        Cell center(Map.MapSize.Width/2+Map.MapSize.Height/2+1,Map.MapSize.Width/2+Map.MapSize.Height/2);
        unknown_id_source_a3d290->Set_Waypoint_Location(99,center);
        unknown_id_source_a3d290->Set_Waypoint_Location(unknown_id_source_a3d290->HomeCell,unknown_id_source_a3d290->Get_Waypoint_Location(99));
        Map[unknown_id_source_a3d290->Get_Waypoint_Location(unknown_id_source_a3d290->HomeCell)].unknown_140|=4;
        Map.RadarClass::Set_Tactical_Position(unknown_id_source_a3d290->Waypoint_Coord(unknown_id_source_a3d290->HomeCell));
        unknown_a40774--;
    } else {
        unknown_id_source_a3d290->Set_Defaults();
        if(!MapPreview)MapPreview=(int)new MapPreviewClass;
        Report_Generation_Progress_0057be50(2);
        int theater=SeedData.Theater;
        bool changed=true;
        int lastTheater=unknown_id_source_a3d290->Theater;
        if(MapSeeder) {
            MapSeedClass const *previous=(MapSeedClass*)MapSeeder;
            lastTheater=previous->Theater;
            if(previous->Width==SeedData.Width && previous->Height==SeedData.Height && lastTheater==theater && previous->NumPlayers==SeedData.NumPlayers)changed=false;
        }
        if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
        if(!MapSeeder || theater!=lastTheater || changed) {
            for(index=AbstractTypes.Count()-1;index>=0;--index)if(AbstractTypes[index]->What_Am_I()==0x12)AbstractTypes.Delete(index);
            if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
            Delete_All_Objects_00519960();
            if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
            for(index=0;index<IsoTileTypes.Count();++index)AbstractTypes.Add(IsoTileTypes[index]);
        }
        Report_Generation_Progress_0057be50(5);
        if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
        if(changed) {
            Map.MouseClass::Init_Clear();
            Map.RadarClass::Set_Map_Dimensions(&size,true,false,true);
            Map.RadarClass::Set_Local_Dimensions(localSize);
            if(unknown_tactical_83996c)delete unknown_tactical_83996c;
            unknown_tactical_83996c=new Tactical;
            unknown_tactical_83996c->Reset_Dirty_Rectangles();
        }
        Report_Generation_Progress_0057be50(10);
        while(Units.Count()>0)delete Units[Units.Count()-1];
        while(Infantry.Count()>0)delete Infantry[Infantry.Count()-1];
        while(unknown_BuildingClass_list_00a40af8.Count()>0)delete unknown_BuildingClass_list_00a40af8[unknown_BuildingClass_list_00a40af8.Count()-1];
        while(Terrains.Count()>0)delete Terrains[Terrains.Count()-1];
        if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
        Map.Reset_Iterator();
        CellClass *cell=Map.Iterate();
        while(cell) {
            cell->unknown_11c=0;cell->unknown_11b=4;cell->IsoTileIndex=0;
            cell->unknown_11a=0;cell->OverlayIndex=-1;cell->OverlayData=0;
            cell=Map.Iterate();
        }
        if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
        Report_Generation_Progress_0057be50(15);
        Map.Compute_Zone_Connections();Map.Zone_Reset();Map.Reset_All_Subzones();
        Report_Generation_Progress_0057be50(20);
        if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
        // The canonical legacy provider takes the scenario address as a 32-bit ECX argument.
        seed_00663220::Clear_All_Waypoints_00663220(reinterpret_cast<int>(unknown_id_source_a3d290));
        Cell center(Map.MapSize.Width/2+Map.MapSize.Height/2+1,Map.MapSize.Width/2+Map.MapSize.Height/2);
        unknown_id_source_a3d290->Set_Waypoint_Location(99,center);
        unknown_id_source_a3d290->Set_Waypoint_Location(98,unknown_id_source_a3d290->Get_Waypoint_Location(99));
        Map[unknown_id_source_a3d290->Get_Waypoint_Location(98)].unknown_140|=4;
        Map.RadarClass::Set_Tactical_Position(unknown_id_source_a3d290->Waypoint_Coord(98));
        Report_Generation_Progress_0057be50(25);
        if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
        unknown_a40774=(int)((unsigned)unknown_a40774+1u);
        if(theater!=unknown_id_source_a3d290->Theater) {Init_Theater_00519e70(theater);unknown_id_source_a3d290->Theater=theater;}
        Report_Generation_Progress_0057be50(30);
        if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
        if(!MapSeeder || theater!=lastTheater || changed) {Rules->Process(*unknown_ini_8396d0);unknown_id_source_a3d290->Read_Global_Variables(*unknown_ini_8396d0);}
        Report_Generation_Progress_0057be50(35);
        if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
        if(!MapSeeder || theater!=lastTheater || changed) {
            if(Houses.Count()==0)for(index=0;index<HouseTypes.Count();++index) {HouseClass *house=new HouseClass(HouseTypes[index]);house->Read_INI(ini);}
            if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
            TerrainTypeClass::Init(unknown_id_source_a3d290->Theater);
            if(unknown_id_source_a3d290->Theater!=unknown_loaded_theater_7d88dc)Read_Tile_Control_00529870(unknown_id_source_a3d290->Theater,true);else Clear_Tile_Use_Counts_0052b650();
            OverlayTypeClass::Init(unknown_id_source_a3d290->Theater);
            if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
            if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
            if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
            BuildingTypeClass::Init(unknown_id_source_a3d290->Theater);
            if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
            AnimTypeClass::Init(unknown_id_source_a3d290->Theater);
            if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
            SmudgeTypeClass::Init(unknown_id_source_a3d290->Theater);
            if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
            Load_Isometric_Tiles_0052b2e0(false,true);
        }
        unknown_loaded_theater_7d88dc=unknown_id_source_a3d290->Theater;
        unknown_a40774--;
    }
    Report_Generation_Progress_0057be50(40);
    if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
    if(unknown_map_cells_a70a24)delete [] unknown_map_cells_a70a24;
    unknown_map_cells_a70a24=new UnknownMapGenerationCell[(unsigned)unknown_map_cell_side_84e804*(unsigned)unknown_map_cell_side_84e804];
    Map.Reset_Iterator();
    CellClass *cell=Map.Iterate();
    while(cell) {
        Cell position=cell->MapCoords;
        unknown_map_cells_a70a24[position.Y*unknown_map_cell_side_84e804+position.X].Position=position;
        cell->unknown_11b=(char)CellHeight;cell=Map.Iterate();
    }
    Report_Generation_Progress_0057be50(45);
    if(unknown_map_generation_callback_7e09c4)unknown_map_generation_callback_7e09c4();
    Clear_Map_Regions_0056da00();
    SeededWaterAmount=0;WorkingRegionID=0;
    PlaceWaterfall=Map_Random_Fraction_00579770()<unknown_waterfall_probability_7a5b68;
    Map.Clear_Radar();Map.RadarClass::Set_Local_Dimensions(localSize);Map.Init_Radar();Map.Compute_Radar_Image();
    unknown_id_source_a3d290->unknown_22a8=SeedData.Theater==0 ? TemperateAmbientLight[SeedData.Time] : SnowAmbientLight[SeedData.Time];
    unknown_id_source_a3d290->AmbientLevel=RMGLevelLightSettings[SeedData.Time];
    unknown_id_source_a3d290->AmbientRed=SeedData.Theater==0 ? TemperateAmbientRed[SeedData.Time] : SnowAmbientRed[SeedData.Time];
    unknown_id_source_a3d290->AmbientGreen=SeedData.Theater==0 ? TemperateAmbientGreen[SeedData.Time] : SnowAmbientGreen[SeedData.Time];
    unknown_id_source_a3d290->AmbientBlue=SeedData.Theater==0 ? TemperateAmbientBlue[SeedData.Time] : SnowAmbientBlue[SeedData.Time];
    unknown_id_source_a3d290->IonAmbient=87;unknown_id_source_a3d290->IonRed=30;unknown_id_source_a3d290->IonGreen=40;unknown_id_source_a3d290->IonBlue=75;unknown_id_source_a3d290->IonGround=0;unknown_id_source_a3d290->IonLevel=0;
    Report_Generation_Progress_0057be50(50);
    if(wcslen(SeedData.DescriptionBuffer)==0)wcscpy(SeedData.DescriptionBuffer,StringTable::Load_String("TXT_RANDOM_MAP_DESCRIPTION",0,"D:\\Ra2\\MapGen.cpp",0x19b5));
}


double __cdecl unknown_sqrt_4ba410(double value);
unsigned int __fastcall Map_Random_Range_005797a0(unsigned int minimum,unsigned int maximum);
void MapGeneratorClass::Partition_Rect(DynamicVectorClass<Rect> *regions,int count,Rect const &area)
{
    int floor=(int)unknown_sqrt_4ba410((double)count);
    int columns=floor*floor==count?floor:floor+1;
    int rows=columns*floor>=count?floor:columns;
    int width=area.Width/columns;
    int height=area.Height/columns;
    int rowX=0,rowY=0;
    int stepX=0,stepY=0;
    if((unsigned int)unknown_map_random_a705c0()*(1.0/4294967295.0)<0.5) {
        width=area.Width/rows;
        stepY=height;
        rowX=width;
    } else {
        height=area.Height/rows;
        stepX=width;
        rowY=height;
    }
    count=rows*columns-count;
    Point2D cursor=Point2D(area.X,area.Y);
    DynamicVectorClass<int> counts;
    DynamicVectorClass<int> choices;
    for(int row=0;row<rows;++row) {
        counts.Add(columns);
        choices.Add(row);
    }
    for(int removed=0;removed<count;++removed) {
        unsigned int maximum=choices.Count()-1;
        struct Range {double Span;};
        Range range={(unsigned int)(maximum+1)};
        unsigned int selected;
        do {selected=(unsigned int)((unsigned int)unknown_map_random_a705c0()*range.Span*(1.0/4294967295.0));} while(selected>maximum);
        counts[choices[selected]]=floor;
        choices.Delete((int)selected);
    }
    for(row=0;row<rows;++row) {
        int columnsInRow=counts[row];
        if(columnsInRow<columns) {
            if(stepX>0)cursor.X+=width/2;
            else cursor.Y+=height/2;
        }
        for(int column=0;column<columnsInRow;++column) {
            regions->Add(Rect(cursor.X+2,cursor.Y+2,width-4,height-4));
            cursor.X+=stepX;
            cursor.Y+=stepY;
        }
        cursor.X+=rowX;
        cursor.Y+=rowY;
        if(stepX>0)cursor.X=area.X;
        else cursor.Y=area.Y;
    }
}


extern double const unknown_random_scale_7a5dd0,unknown_chunk_floor_7a5ed0;
void MapGeneratorClass::unknown_0057c4f0()
{
    WorkingRegionID=1;
    DynamicVectorClass<Rect> regions;
    unsigned int range=2>SeedData.NumPlayers/2?2:SeedData.NumPlayers/2;
    double span=(double)range;
    unsigned int selected;
    do {selected=(unsigned int)((unsigned int)unknown_map_random_a705c0()*(1.0/4294967295.0)*span+1.0);} while(selected>range);
    Rect area=Map.MapLocalSize;
    Partition_Rect(&regions,SeedData.NumPlayers+selected,area);
    Map.Reset_Iterator();
    for(CellClass *cell=Map.Iterate();cell;cell=Map.Iterate()) {
        Cell position=cell->MapCoords;
        unknown_map_cells_a70a24[position.Y*unknown_map_cell_side_84e804+position.X].unknown_3c=0;
    }
    while(regions.Count()>0) {
        Rect region=regions[0];
        int count=(int)(((unsigned int)unknown_map_random_a705c0()*unknown_random_scale_7a5dd0*0.05+unknown_chunk_floor_7a5ed0)*(region.Width*region.Height*2));
        Point2D center;
        center.X=region.Width/2+region.Height/2+region.Y+region.X+1;
        center.Y=region.Height/2-region.Width/2-region.X+Map.MapSize.Width+region.Y;
        int attempt=0;
        int result=0;
        while(!result && attempt<10) {
            result=unknown_0057d3a0(count,region,center,true,Cell(center.X,center.Y),0.25,false);
            if(!result)++WorkingRegionID;
            ++attempt;
        }
        regions.Delete(0);
    }
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// From ra2-decomp 0x0059AFA0 MapGeneratorClass::Seed_Land_Type1 (src/mapgen.cpp, YR twin).
// Scatters one land mass from the middle of the map until the land target is met.
int __fastcall unknown_cells_428ac0();
extern double const unknown_land_minimum_7a5c58,unknown_land_maximum_7a5c60,unknown_land_chunk_7a5c68;
void MapGeneratorClass::Seed_Land_Type1()
{
    WorkingRegionID = 1;
    int total = unknown_cells_428ac0();
    int placed = 0;
    int tries = 0;
    double current = 0.0;
    double target = (1.0 - SeedData.WaterAmount * 0.01) * (unknown_land_maximum_7a5c60 - unknown_land_minimum_7a5c58) + unknown_land_minimum_7a5c58;
    int maximum = (int)((double)total * unknown_land_chunk_7a5c68 * target);
    Point2D origin(Map.MapSize.Width / 2 + Map.MapSize.Height / 2 + 1,
                   Map.MapSize.Width / 2 + Map.MapSize.Height / 2);
    Point2D seed = origin;

    Map.Reset_Iterator();
    CellClass *cptr = Map.Iterate();
    while (cptr != 0) {
        MapRegionClass::Get_Cell_Data(cptr->Fetch_CellID()).unknown_3c = 0;
        cptr = Map.Iterate();
    }

    Rect bounds = Map.MapLocalSize;
    bounds.X++;
    bounds.Y++;
    bounds.Width -= 2;
    bounds.Height -= 2;
    while (current < target && tries < 100) {
        int size = (int)((target - current) * (double)total);
        size = size < maximum ? size : maximum;
        current = (double)(placed += unknown_0057d3a0(size, bounds, seed, true, Cell(origin.X, origin.Y), 0.75, true)) / (double)total;
        tries++;

        int nearest = 50000;
        Cell closest(0, 0);
        Map.Reset_Iterator();
        cptr = Map.Iterate();
        while (cptr != 0) {
            if (MapRegionClass::Get_Cell_Data(cptr->MapCoords).unknown_3c == 0) {
                int distance = abs(cptr->MapCoords.X - origin.X) + abs(cptr->MapCoords.Y - origin.Y);
                if (distance < nearest) {
                    nearest = distance;
                    closest = cptr->MapCoords;
                }
            }
            cptr = Map.Iterate();
        }
        seed = Point2D(closest.X, closest.Y);
    }
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// From ra2-decomp 0x0059B200 MapGeneratorClass::Seed_Land_Type2 (src/mapgen.cpp, YR twin).
// Scatters land over two halves of the map until each half meets the land target.
int __fastcall unknown_cells_428ac0();
extern double const unknown_random_scale_7a5dd0;
extern double const unknown_land2_minimum_7a5c78,unknown_land2_maximum_7a5c80,unknown_land2_chunk_7a5c88;
void MapGeneratorClass::Seed_Land_Type2()
{
    WorkingRegionID = 1;
    int total = unknown_cells_428ac0();
    int tries = 0;
    double target = (1.0 - SeedData.WaterAmount * 0.01) * (unknown_land2_maximum_7a5c80 - unknown_land2_minimum_7a5c78) + unknown_land2_minimum_7a5c78;
    int maximum = (int)((double)total * unknown_land2_chunk_7a5c88 * target);

    Map.Reset_Iterator();
    CellClass *cptr = Map.Iterate();
    while (cptr != 0) {
        MapRegionClass::Get_Cell_Data(cptr->Fetch_CellID()).unknown_3c = 0;
        cptr = Map.Iterate();
    }

    Rect halves[2];
    if ((unsigned int)unknown_map_random_a705c0() * unknown_random_scale_7a5dd0 < 0.5) {
        halves[0] = Rect(Map.MapLocalSize.X, Map.MapLocalSize.Y, Map.MapLocalSize.Width / 2 - 1, Map.MapLocalSize.Height);
        halves[1] = Rect(Map.MapLocalSize.X + Map.MapLocalSize.Width / 2 + 1, Map.MapLocalSize.Y, Map.MapLocalSize.Width / 2 - 1, Map.MapLocalSize.Height);
    } else {
        halves[0] = Rect(Map.MapLocalSize.X, Map.MapLocalSize.Y, Map.MapLocalSize.Width, Map.MapLocalSize.Height / 2 - 1);
        halves[1] = Rect(Map.MapLocalSize.X, Map.MapLocalSize.Y + Map.MapLocalSize.Height / 2 + 1, Map.MapLocalSize.Width, Map.MapLocalSize.Height / 2 - 1);
    }

    for (int half = 0; half < 2; half++) {
        int placed = 0;
        double current = 0.0;
        Rect bounds = halves[half];
        Point2D origin(bounds.X + bounds.Width / 2 + bounds.Height / 2 + bounds.Y + 1,
                       bounds.Y + Map.MapSize.Width + bounds.Height / 2 - bounds.Width / 2 - bounds.X);
        Point2D seed = origin;
        double x_scale;
        double y_scale;
        if (bounds.Height > bounds.Width) {
            x_scale = (double)bounds.Height / (double)bounds.Width * 1.2;
            y_scale = 1.0;
        } else {
            x_scale = 1.0;
            y_scale = (double)bounds.Width / (double)bounds.Height * 1.2;
        }
        double half_width = (double)bounds.Width * 0.5;
        double inverse_x = 1.0 / (half_width * half_width);
        double half_height = (double)bounds.Height * 0.5;
        double inverse_y = 1.0 / (half_height * half_height);

        while (current < target && tries < 100) {
            int size = (int)((target - current) * (double)total);
            size = size < maximum ? size : maximum;
            current = (double)(placed += unknown_0057d3a0(size, bounds, seed, true, Cell(origin.X, origin.Y), 0.75, true)) / (double)total;
            tries++;

            int nearest = 50000;
            Cell closest(0, 0);
            int count = unknown_map_cell_side_84e804 * unknown_map_cell_side_84e804;
            int minimum_diagonal = bounds.X * 2 - Map.MapSize.Width + 1;
            int maximum_diagonal = minimum_diagonal + bounds.Width * 2 + 2;
            int minimum_sum = Map.MapSize.Width + bounds.Y * 2 + 1;
            int maximum_sum = minimum_sum + bounds.Height * 2 + 2;
            for (int index = 0; index < count; index++) {
                UnknownMapGenerationCell &data = MapRegionClass::Get_Cell_Data(index);
                int sum = data.Position.X + data.Position.Y;
                int diagonal = data.Position.X - data.Position.Y;
                if (sum >= minimum_sum && sum <= maximum_sum && diagonal >= minimum_diagonal && diagonal <= maximum_diagonal && data.unknown_3c == 0 && Cell_In_Generation_Shape_0057d290(data.Position, bounds, true, inverse_x, inverse_y)) {
                    double dy = (double)abs(data.Position.Y - origin.Y) * y_scale;
                    double dx = (double)abs(data.Position.X - origin.X) * x_scale;
                    int distance = (int)(dy + dx);
                    if (distance < nearest) {
                        nearest = distance;
                        closest = data.Position;
                    }
                }
            }
            seed = Point2D(closest.X, closest.Y);
        }
    }
}


// Operation name/free-cdecl spelling are hypotheses; canonical Cell/Map interfaces unchanged.
void Fill_Adjacent_Empty_Tiles_0057ceb0()
{
    Map.Reset_Iterator();
    for(CellClass *cell=Map.Iterate();cell;cell=Map.Iterate()) {
        if(cell->unknown_47b7c0()) {
            for(int facing=0;facing<8;facing+=2) {
                CellClass &adjacent=cell->Adjacent_Cell(facing);
                if(adjacent.unknown_47b590())
                    adjacent.IsoTileIndex=unknown_tileset_a52c60;
            }
        }
    }
}


// Map tile-growth operation/name hypotheses; original controller passes its owner in ECX, unused here.
void MapGeneratorClass::Scatter_Green_Tiles()
{
    DynamicVectorClass<CellClass *> candidates;
    candidates.Set_Growth_Step(10000);
    Map.Reset_Iterator();
    for(CellClass *cell=Map.Iterate();cell;cell=Map.Iterate()) {
        if(cell->unknown_47b9c0()) {
            for(int facing=0;facing<8;facing+=2) {
                CellClass *adjacent=&cell->Adjacent_Cell(facing);
                if(adjacent->unknown_47b590())candidates.Add(adjacent);
            }
        }
    }
    int remaining=candidates.Count()/3;
    remaining=remaining>1000?1000:remaining;
    for(int step=0;step<remaining;++step) {
        unsigned maximum=(unsigned)candidates.Count()-1;
        struct Range { double Span; };
        Range range={(unsigned)(maximum+1)};
        unsigned index;
        do {
            index=(unsigned)((unsigned)unknown_map_random_a705c0()*range.Span*(1.0/4294967295.0));
        } while(index>maximum);
        CellClass *cell=candidates[index];
        candidates.Delete((int)index);
        cell->IsoTileIndex=unknown_tileset_a52c60;
        for(int facing=0;facing<8;facing+=2) {
            CellClass *adjacent=&cell->Adjacent_Cell(facing);
            if(adjacent->unknown_47b590())candidates.Add(adjacent);
        }
    }
}


// Free stdcall and operation name are hypotheses; original consumes24 stack bytes.
extern float const unknown_zero_79a738;
double __stdcall Direction_Weighted_Cell_Distance_0057d120(Cell const *from,Cell const *to,Rect const *area,Cell const *origin,float xWeight,float yWeight)
{
    if(*area!=unknown_static_rect) {
        int x=abs((((int)to->X-(int)to->Y)>>1)-(((int)origin->X-(int)origin->Y)>>1));
        int y=abs((((int)to->X+(int)to->Y)>>1)-(((int)origin->X+(int)origin->Y)>>1));
        if(x==0 && y==0)return unknown_zero_79a738;
        int squared=(int)((unsigned)y*(unsigned)y+(unsigned)x*(unsigned)x);
        double length=Fast_Sqrt((double)squared);
        double xDirection=(double)x/length;
        double yDirection=(double)y/length;
        int dx=(int)from->X-(int)to->X;
        int dy=(int)from->Y-(int)to->Y;
        double distance=abs(dx)>abs(dy)?abs(dx):abs(dy);
        return (xWeight*xDirection+yWeight*yDirection)*distance;
    }
    int dx=(int)from->X-(int)to->X;
    int dy=(int)from->Y-(int)to->Y;
    return Fast_Sqrt((double)(int)((unsigned)dy*(unsigned)dy+(unsigned)dx*(unsigned)dx));
}


// Not exact (RA2_NON_MATCHING): MapRegionClass::Split_Region_0056ef20 at 0x0056EF20, best score 53.91, 2026-11-13; differs: 1250 instructions in the original, 1284 in the draft (shape 93.76)
#ifdef RA2_NON_MATCHING
extern DynamicVectorClass<MapRegionClass *> unknown_map_regions_a6fd28;
extern int const unknown_facing_offsets_84c838[8];
double __stdcall Unknown_Cell_Score_0056dff0(Cell const &from, Cell const &to, double direction);

static inline UnknownVeinNode *Split_Remove_Min(UnknownVeinHeap *queue)
{
    if (queue->Count == 0) {
        return 0;
    }
    UnknownVeinNode *top = queue->Array[1];
    queue->Array[1] = queue->Array[queue->Count];
    queue->Array[queue->Count] = 0;
    queue->Count--;
    queue->Sift_Down(1);
    return top;
}

static inline MapRegionClass *Split_From_ID(int id)
{
    for (int i = 0; i < unknown_map_regions_a6fd28.Count(); i++) {
        if (unknown_map_regions_a6fd28[i]->Index == id) {
            return unknown_map_regions_a6fd28[i];
        }
    }
    return 0;
}

static inline int Get_Spread(Cell const &cell)
{
    return MapRegionClass::Get_Cell_Data(cell).unknown_3c;
}

static inline unsigned Split_Random(unsigned minimum, unsigned maximum)
{
    struct Range { double Span; double Origin; };
    Range range = {(unsigned)(maximum - minimum + 1), minimum};
    unsigned result;
    do {
        result = (unsigned)((unsigned)unknown_map_random_a705c0() * range.Span * (1.0 / 4294967295.0) + range.Origin);
    } while (result > maximum);
    return result;
}

static inline bool Is_Dry_Map()
{
    if (RandomMapGen.SeedData.MapType == 3 || RandomMapGen.SeedData.MapType == 4) {
        return true;
    }
    return false;
}
bool MapRegionClass::Split_Region_0056ef20()
{
    int i, j;

    if (unknown_1a) {
        return false;
    }

    int heap_size = (2 * CellCount) + 10;
    UnknownVeinNode *nodes = new UnknownVeinNode[heap_size];
    UnknownVeinHeap *queue = new UnknownVeinHeap((2 * CellCount) + 10);
    queue->Clear();

    Clear_Cell_Marks_0056dfc0();

    int spread_count = 0;
    for (i = Cells.Count() - 1; i >= 0; i--) {
        MapRegionClass::Get_Cell_Data(Cells[i]).RegionIndex = -2;
    }

    unsigned target_count = Split_Random(CellCount / 8, CellCount / 3);
    unsigned seed_index = Split_Random(0, Cells.Count() - 1);

    int node_count = 1;
    Cell seed = Cells[seed_index];
    nodes[0].Position = seed;
    nodes[0].Key = 0.0f;
    MapRegionClass::Get_Cell_Data(seed).unknown_3c = -3;
    queue->Insert(&nodes[0]);

    UnknownVeinNode *node = queue->Remove_Min();
    double dist = (unsigned)unknown_map_random_a705c0() * (6.283185307179586 / 4294967295.0);

    while (node != 0) {
        if (spread_count >= (int)target_count) {
            break;
        }

        Set_Region(node->Position, -3);

        int cell_index = node->Position.X + unknown_map_cell_side_84e804 * node->Position.Y;
        UnknownVeinNode *newnode = &nodes[node_count];

        for (int dir = 0; dir < 8; dir += 2) {
            Cell ncell = Region_Adjacent_Cell(node->Position, dir);
            if (Flood_In_Bounds(ncell)) {
                if (UnknownMapGenerationCell::Get_Region(ncell) == -2 && Get_Spread(ncell) != -3 && Map[ncell].unknown_47b590()) {
                    newnode->Position = ncell;
                    newnode->Key = (float)RandomMapGen.Get_Angle_Score(seed, ncell, dist);
                    MapRegionClass::Get_Cell_Data(cell_index + unknown_facing_offsets_84c838[dir]).unknown_3c = -3;
                    node_count++;
                    queue->Insert(newnode++);
                }
            }
        }

        spread_count++;
        dist += unknown_map_gaussian_a6fd50.Next() * 0.39269908169872414;
        node = queue->Remove_Min();
    }

    Clear_Cell_Marks_0056dfc0();

    UnknownVeinNode *remaining = queue->Remove_Min();
    DynamicVectorClass<Cell> ncells;

    while (remaining != 0) {
        Cell c = remaining->Position;
        MapRegionClass::Get_Cell_Data(c).RegionIndex = -3;
        remaining = queue->Remove_Min();
    }

    delete[] nodes;
    delete queue;

    DynamicVectorClass<MapRegionClass *> new_regions;
    for (i = Cells.Count() - 1; i >= 0; i--) {
        Cell cell = Cells[i];
        int cell_index = cell.X + unknown_map_cell_side_84e804 * cell.Y;
        if (MapRegionClass::Get_Cell_Data(cell_index).RegionIndex < -1) {
            int old_id = MapRegionClass::Get_Cell_Data(cell_index).RegionIndex;

            MapRegionClass *region = new MapRegionClass(cell);
            region->Cells.Add(cell);
            region->CellCount++;
            new_regions.Add(region);
            MapRegionClass::Get_Cell_Data(cell_index).RegionIndex = region->Index;

            DynamicVectorClass<Cell> open;
            open.Set_Growth_Step(100000);
            open.Add(cell);

            while (open.Count() != 0) {
                int index = open.Count() - 1;
                Cell c = open[index];
                open.Delete(index);

                for (int m = 0; m < 8; m++) {
                    Cell ncell = Region_Adjacent_Cell(c, m);
                    if (Flood_In_Bounds(ncell)) {
                        UnknownMapGenerationCell &ndata = MapRegionClass::Get_Cell_Data(ncell);
                        if (ndata.RegionIndex == old_id) {
                            region->Cells.Add(ncell);
                            region->CellCount++;
                            ndata.RegionIndex = region->Index;
                            open.Add(ncell);
                        }
                    }
                }
            }
        }
    }

    for (i = new_regions.Count() - 1; i >= 0; i--) {
        MapRegionClass *region = new_regions[i];
        if (region->unknown_24) {
            continue;
        }

        DynamicVectorClass<Cell> *cells = region->Collect_Region_Boundary_0056fed0();
        bool *adjacent = new bool[unknown_map_region_next_index_a70a28];
        for (j = 0; j < unknown_map_region_next_index_a70a28; j++) {
            adjacent[j] = false;
        }

        for (j = cells->Count() - 1; j >= 0; j--) {
            Cell c = (*cells)[j];
            for (int m = 0; m < 8; m++) {
                Cell ncell = Region_Adjacent_Cell(c, m);
                if (Flood_In_Bounds(ncell)) {
                    int id = MapRegionClass::Get_Cell_Data(ncell).RegionIndex;
                    if (id >= 0 && id != region->Index) {
                        adjacent[id] = true;
                    }
                }
            }
        }

        delete cells;

        MapRegionClass *merge_target = 0;
        int min_height = -1;
        int max_height = -1;
        int merge_height = -1;
        int max_count = -1;

        for (j = 0; j < unknown_map_region_next_index_a70a28; j++) {
            if (adjacent[j]) {
                MapRegionClass *other = Split_From_ID(j);

                if (!other->unknown_24) {
                    if (other->CellCount > max_count && !other->unknown_14) {
                        max_count = other->CellCount;
                        merge_height = other->unknown_10;
                        merge_target = other;
                    }

                    if (min_height == -1) {
                        min_height = other->unknown_10;
                        max_height = other->unknown_10;
                    } else {
                        if (other->unknown_10 > max_height) {
                            max_height = other->unknown_10;
                        }
                        if (other->unknown_10 < min_height) {
                            min_height = other->unknown_10;
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

        if (Is_Dry_Map()) {
            for (int h = heights.Count() - 1; h >= 0; h--) {
                if (heights[h] == 0) {
                    heights.Delete(h);
                }
            }
        }

        delete[] adjacent;

        if (region->CellCount > 100) {
            int old_height = region->unknown_10;
            unsigned max_index = (unsigned)heights.Count() - 1;
            unsigned index = Split_Random(0, max_index);

            region->unknown_10 = heights[index];
            region->Increase_Height_0056eea0((char)(region->unknown_10 - old_height));
        } else {
            if (merge_target != 0) {
                int old_height = region->unknown_10;
                region->unknown_10 = merge_height;
                region->Increase_Height_0056eea0((char)(region->unknown_10 - old_height));

                for (int n = region->Cells.Count() - 1; n >= 0; n--) {
                    Cell c = region->Cells[n];
                    merge_target->Cells.Add(c);
                    merge_target->CellCount++;
                    MapRegionClass::Get_Cell_Data(c).RegionIndex = merge_target->Index;
                }

                region->unknown_24 = true;
            }
        }
    }

    for (i = unknown_map_regions_a6fd28.Count() - 1; i >= 0; i--) {
        MapRegionClass *region = unknown_map_regions_a6fd28[i];
        if (region != this && region->unknown_24) {
            delete region;
        }
    }

    unknown_1c++;
    return true;
}
#endif

// Not exact (RA2_NON_MATCHING): FUN_00576420 at 0x00576420, best score 43.93, 2026-11-13; differs: 293 instructions in the original, 358 in the draft (shape 83.87)
#ifdef RA2_NON_MATCHING
// Divides the starting positions between the regions large enough to hold them, largest first.
#include "shared/ini_class.hpp"
extern double const unknown_region_scale_7a5e10,unknown_region_minimum_7a5e08;
extern double const unknown_random_scale_7a5dd0;
bool __fastcall Generation_Cell_In_Bounds_0058da10(Cell const &position);
void __fastcall Grow_Zone_Region_00575d20(CellClass *cell,int zone_type,int zone);
bool __cdecl Generate_Starting_Points_00576420()
{
    int zone_types[5]={5,5,5,0,0};
    double size=(double)RandomMapGen.LocalHeight*RandomMapGen.LocalWidth*unknown_region_scale_7a5e10;
    if(unknown_region_minimum_7a5e08>size) {
        size=unknown_region_minimum_7a5e08;
    }
    int minimum=size;
    int largest=Map.Zone_Reset();
    int zone_type=zone_types[RandomMapGen.SeedData.MapType];
    int main_zone=((unsigned short *)Map.unknown_18[zone_type])[largest];
    MapRegionClass::Destroy_Cell_Regions();
    Map.Reset_Iterator();
    for(CellClass *cell=Map.Iterate();cell!=0;cell=Map.Iterate()) {
        if(Generation_Cell_In_Bounds_0058da10(cell->MapCoords)) {
            if(MapRegionClass::Get_Cell_Data(cell->MapCoords).RegionIndex==-1 && cell->unknown_4c==0) {
                if(Map.Zone(cell->MapCoords,zone_type,false)==main_zone) {
                    Grow_Zone_Region_00575d20(cell,zone_type,main_zone);
                }
            }
        }
    }
    int count=MapRegionClass::Regions.Count();
    for(int i=count-1;i>=0;i--) {
        MapRegionClass *region=MapRegionClass::Regions[i];
        if(region->CellCount<minimum) {
            delete region;
        }
    }
    DynamicVectorClass<MapRegionClass *> sorted;
    IndexClass<int,MapRegionClass *> index;
    sorted.Clear();
    for(i=0;i<MapRegionClass::Regions.Count();i++) {
        MapRegionClass *const &region=MapRegionClass::Regions[i];
        index.Add_Index((500000-region->CellCount)*count+region->Index,region);
    }
    for(i=0;i<index.Count();i++) {
        sorted.Add(index.Fetch_By_Position(i));
    }
    int total=0;
    for(i=0;i<sorted.Count();i++) {
        total+=sorted[i]->CellCount;
    }
    int starts=RandomMapGen.SeedData.NumPlayers;
    int running=0;
    int assigned=0;
    for(i=0;i<sorted.Count();i++) {
        MapRegionClass *region=sorted[i];
        running+=region->CellCount;
        int share;
        if(i==sorted.Count()-1) {
            share=starts-assigned;
        } else {
            share=(double)running/total*starts+0.5-assigned;
        }
        region->unknown_20=share;
        assigned+=share;
    }
    int first=0;
    for(i=0;i<sorted.Count();i++) {
        sorted[i]->Generate_Starting_Points_00576170(first);
        first+=sorted[i]->unknown_20;
    }
    if(RandomMapGen.SeedData.MapType==0) {
        for(i=0;i<sorted.Count();i++) {
            if(sorted[i]->unknown_20==0 && unknown_map_random_a705c0()*unknown_random_scale_7a5dd0<0.5) {
                sorted[i]->Place_Neutral_Tech_Building_00576cd0();
            }
        }
    }
    return true;
}
#endif

// Not exact (RA2_NON_MATCHING): FUN_00577010 at 0x00577010, best score 86.79, 2026-11-13; differs: same instruction count (106), shape 92.45: instruction choice and registers differ
#ifdef RA2_NON_MATCHING
// MapGeneratorClass::MapGeneratorClass() (0x00577010)
// The seed settings, no seeder or preview, empty light/vegetation/lamp lists, tiberium 2500-5500,
// 500 trees, a stale preview and cell height 4. A plain member-initialiser constructor.
// From ra2-decomp 0x00595740 (YR twin, src/mapgen.cpp MapGeneratorClass::MapGeneratorClass).

MapGeneratorClass::MapGeneratorClass() :
    SeedData(),
    MapSeeder(0),
    MapPreview(0),
    LocalWidth(0),
    LocalHeight(0),
    RMGMinimumTiberium(2500),
    RMGMaximumTiberium(5500),
    MaxTrees(500),
    PreviewStale(true),
    SeededWaterAmount(0),
    CellHeight(4),
    PlaceWaterfall(false)
{
}
#endif

// Not exact (RA2_NON_MATCHING): MapGeneratorClass::Read_Settings_00579960 at 0x00579960, best score 61.57, 2026-11-13; differs: 539 instructions in the original, 546 in the draft (shape 58.62)
#ifdef RA2_NON_MATCHING
// void MapGeneratorClass::Read_Settings_00579960() (0x00579960)
// Reads RMG.INI's [General] settings into the generator: the tiberium totals, the level light
// and vegetation lists, the per-theater ambient lights, the tree limit and the ore-patch lamp
// buildings (RA2 only; the lists are emptied first; drafted from the disassembly, Westwood
// Rules-reader idiom: `x = ini.Get_...(section, "x", x)` and TibSun findmake.h TGet_TypeList).

// The [General] section name (a pointer at 0x007E0BE4 to "General").
extern char const *unknown_general_section_7e0be4;

// TibSun findmake.h TGet_TypeList: a comma separated list of type names, each found or made;
// the default list when the entry is missing.
template<class T>
inline TypeList<T const *> TGet_TypeList(CCINIClass const &ini, char const *section, char const *entry, TypeList<T const *> const &defvalue)
{
    char buffer[128];

    if (ini.Get_String(section, entry, unknown_string_83c55c, buffer, sizeof(buffer)) != 0) {
        TypeList<T const *> list;

        char const *token = strtok(buffer, ",");

        while (token != 0 && token[0] != '\0') {
            T const *ptr = T::Find_Or_Make(token);

            if (ptr) {
                list.Add(ptr);
            }

            token = strtok(0, ",");
        }
        return(list);
    }

    return(defvalue);
}

void MapGeneratorClass::Read_Settings_00579960()
{
    CCFileClass file("RMG.INI");
    CCINIClass *ini = new CCINIClass;
    ini->Load(file, false, false);

    RMGLevelLightSettings.Clear();
    RMGVegetationMinimums.Clear();
    RMGVegetationMaximums.Clear();
    TemperateOrePatchLamps.Clear();
    SnowOrePatchLamps.Clear();
    TemperateAmbientLight.Clear();
    SnowAmbientLight.Clear();

    RMGMinimumTiberium = ini->Get_Int(unknown_general_section_7e0be4, "RMGMinimumTiberium", RMGMinimumTiberium);
    RMGMaximumTiberium = ini->Get_Int(unknown_general_section_7e0be4, "RMGMaximumTiberium", RMGMaximumTiberium);
    RMGLevelLightSettings = ini->Get_Integer_List(unknown_general_section_7e0be4, "RMGLevelLightSettings", RMGLevelLightSettings);
    RMGVegetationMinimums = ini->Get_Integer_List(unknown_general_section_7e0be4, "RMGVegetationMinimums", RMGVegetationMinimums);
    RMGVegetationMaximums = ini->Get_Integer_List(unknown_general_section_7e0be4, "RMGVegetationMaximums", RMGVegetationMaximums);
    TemperateAmbientLight = ini->Get_Integer_List(unknown_general_section_7e0be4, "TemperateAmbientLight", TemperateAmbientLight);
    SnowAmbientLight = ini->Get_Integer_List(unknown_general_section_7e0be4, "SnowAmbientLight", SnowAmbientLight);
    TemperateAmbientRed = ini->Get_Integer_List(unknown_general_section_7e0be4, "TemperateAmbientRed", TemperateAmbientRed);
    TemperateAmbientGreen = ini->Get_Integer_List(unknown_general_section_7e0be4, "TemperateAmbientGreen", TemperateAmbientGreen);
    TemperateAmbientBlue = ini->Get_Integer_List(unknown_general_section_7e0be4, "TemperateAmbientBlue", TemperateAmbientBlue);
    SnowAmbientRed = ini->Get_Integer_List(unknown_general_section_7e0be4, "SnowAmbientRed", SnowAmbientRed);
    SnowAmbientGreen = ini->Get_Integer_List(unknown_general_section_7e0be4, "SnowAmbientGreen", SnowAmbientGreen);
    SnowAmbientBlue = ini->Get_Integer_List(unknown_general_section_7e0be4, "SnowAmbientBlue", SnowAmbientBlue);
    MaxTrees = ini->Get_Int(unknown_general_section_7e0be4, "MaxTrees", MaxTrees);
    TemperateOrePatchLamps = TGet_TypeList<BuildingTypeClass>(*ini, unknown_general_section_7e0be4, "TemperateOrePatchLamps", TemperateOrePatchLamps);
    SnowOrePatchLamps = TGet_TypeList<BuildingTypeClass>(*ini, unknown_general_section_7e0be4, "SnowOrePatchLamps", SnowOrePatchLamps);

    delete ini;
}
#endif

// Operation/name/return intent are hypotheses; physical RET28 and AL predicate. A member of the generator: both
// callers (0x57C9E0, 0x57D3A0) pass this in ECX, which is not read (ra2-decomp YR twin MapGeneratorClass::FUN_0059BAB0).
extern double const unknown_half_79a728,unknown_one_79a708;
bool MapGeneratorClass::Cell_In_Generation_Shape_0057d290(Cell const &position,Rect const &area,bool ellipse,double x_weight,double y_weight)
{
    if(area==unknown_static_rect)return true;
    if(ellipse) {
        int x=(int)((unsigned)(int)position.X-2u*(unsigned)area.X-(unsigned)(int)position.Y-1u+(unsigned)Map.MapSize.Width);
        int y=(int)((unsigned)(int)position.X-2u*(unsigned)area.Y-(unsigned)Map.MapSize.Width-1u+(unsigned)(int)position.Y);
        double dx=(double)x*unknown_half_79a728-(double)area.Width*unknown_half_79a728;
        double dy=(double)y*unknown_half_79a728-(double)area.Height*unknown_half_79a728;
        double weight=dy*dy*y_weight;
        weight+=dx*dx*x_weight;
        return weight<unknown_one_79a708;
    }
    int px=position.X;
    int py=position.Y;
    return px>=area.X && px<(int)((unsigned)area.X+(unsigned)area.Width) &&
           py>=area.Y && py<(int)((unsigned)area.Y+(unsigned)area.Height);
}


void MapGeneratorClass::Seed_Water()
{
    ++WorkingRegionID;
    if((SeedData.MapType==3 || SeedData.MapType==4) && SeedData.WaterAmount>20) {
        int attempts=0;
        bool success=false;
        while(!success) {
            if(attempts>=10)break;
            success=Seed_River(Cell(0,0),0.0,false);
            ++attempts;
        }
        if(success)++WorkingRegionID;
    }
    int attempts=0;
    bool success=false;
    while(!success) {
            if(attempts>=10)break;
        success=Seed_Lake(Cell(0,0));
        ++attempts;
    }
    if(success)++WorkingRegionID;
}


unsigned int __fastcall Map_Random_Range_005797a0(unsigned int minimum,unsigned int maximum)
{
    double origin;
    unsigned int result;
    do {
        origin=minimum;
        result=(unsigned int)((unsigned int)unknown_map_random_a705c0()*(double)(unsigned int)(maximum-minimum+1)*(1.0/4294967295.0)+origin);
    } while(result>maximum);
    return result;
}

void __fastcall Stamp_Tile_005883f0(int tile,Cell const &position,int region,int height);
void MapGeneratorClass::Vary_Water_Tiles()
{
    Map.Reset_Iterator();
    for(CellClass *cell=Map.Iterate();cell;cell=Map.Iterate()) {
        if(cell->IsoTileIndex==unknown_tileset_a52580 && cell->unknown_11a==0) {
            CellClass &east=cell->Adjacent_Cell(2);
            CellClass &south=cell->Adjacent_Cell(4);
            CellClass &corner=cell->Adjacent_Cell(3);
            if(east.IsoTileIndex==unknown_tileset_a52580 && east.unknown_11a==0 &&
               south.IsoTileIndex==unknown_tileset_a52580 && south.unknown_11a==0 &&
               corner.IsoTileIndex==unknown_tileset_a52580 && corner.unknown_11a==0 &&
               Map_Random_Range_005797a0(1,10)!=1) {
                    int sample=(int)Map_Random_Range_005797a0(0,241);
                    int variant=sample>=240 ?247-sample:sample/40;
                    Cell const &position=cell->MapCoords;
                    int region=!unknown_map_cells_a70a24 ?-1:unknown_map_cells_a70a24[position.Y*unknown_map_cell_side_84e804+position.X].RegionIndex;
                    Stamp_Tile_005883f0(unknown_tileset_a52580+variant,position,region,-1);
            } else {
                unsigned sample=Map_Random_Range_005797a0(0,200);
                cell->IsoTileIndex=unknown_tileset_a52580+sample/40+8;
            }
        }
    }
}


// Region fill operation/bool-or-char return/free-stdcall intent remain hypotheses.
bool __stdcall Fill_Empty_Region_Tiles_0057dfe0(int index)
{
    Map.Reset_Iterator();
    for(CellClass *cell=Map.Iterate();cell;cell=Map.Iterate()) {
        int region=!unknown_map_cells_a70a24 ? -1 : unknown_map_cells_a70a24[cell->MapCoords.Y*unknown_map_cell_side_84e804+cell->MapCoords.X].RegionIndex;
        if(region==index && (cell->IsoTileIndex==0 || cell->IsoTileIndex==0xffff))
            cell->IsoTileIndex=unknown_tileset_a52c60;
    }
    return 1;
}


void MapGeneratorClass::Clear_Region_Cells(int source_index,int target_index,int level,int tile)
{
    Map.Reset_Iterator();
    for(CellClass *cell=Map.Iterate();cell;cell=Map.Iterate()) {
        Cell position=cell->MapCoords;
        UnknownMapGenerationCell &target=unknown_map_cells_a70a24[position.Y*unknown_map_cell_side_84e804+position.X];
        if(target.RegionIndex==source_index) {
            target.RegionIndex=target_index;
            target.unknown_4b=0;
            cell->IsoTileIndex=tile;
            cell->unknown_11a=0;
            cell->unknown_11b=(char)(level==-1 ? CellHeight:level);
        }
    }
}


// TibSun MapGeneratorClass::Seed_Lake (shape reference); the member keeps its unknown_ name until a rename lands.
double __stdcall Weighted_Cell_Distance_0057ec60(Cell const *,Cell const *,int);
bool __fastcall Generation_Cell_In_Bounds_0058da10(Cell const &position);
double __stdcall Map_Clamped_Gaussian_005798f0(double,double,double,double);

bool MapGeneratorClass::Seed_Lake(Cell const &origin)
{
    unsigned char success=1;
    int spreadLimit=(int)((unsigned)Get_Target_Water_Amount()-(unsigned)SeededWaterAmount);
    if(spreadLimit<=75)return false;
    int capacity=(int)((unsigned)spreadLimit*2u+2u);
    UnknownVeinNode *nodes=new UnknownVeinNode[capacity>100?capacity:100];
    UnknownVeinHeap *heap=new UnknownVeinHeap(capacity>100?capacity:100);
    Map.Reset_Iterator();
    for(CellClass *cell=Map.Iterate();cell;cell=Map.Iterate()) {
        Cell position=cell->MapCoords;
        unsigned index=(unsigned)(int)position.Y*(unsigned)unknown_map_cell_side_84e804+(unsigned)(int)position.X;
        unknown_map_cells_a70a24[index].unknown_3c=0;
    }
    int gridSize=(int)((unsigned)unknown_map_cell_side_84e804*(unsigned)unknown_map_cell_side_84e804);
    if(origin==Cell(0,0)) {
        for(int i=0;i<gridSize;++i)MapRegionClass::Get_Cell_Data(i).Marked=0;
        Clear_Region_Border(0,2,-2);
        for(i=0;i<gridSize;++i) {
            int storedRegion=MapRegionClass::Get_Cell_Data(i).RegionIndex;
            if(storedRegion==0 || storedRegion==(int)WorkingRegionID)MapRegionClass::Get_Cell_Data(i).Marked=1;
            else if(storedRegion==-2)MapRegionClass::Get_Cell_Data(i).RegionIndex=0;
        }
    } else {
        for(int i=0;i<gridSize;++i)MapRegionClass::Get_Cell_Data(i).Marked=1;
    }
    int tries=0;
    Cell seed;
    if(origin==Cell(0,0)) {
        for(;;) {
            int x=(int)Map_Random_Range_005797a0(0,Map.MapSize.Width-1);
            int y=(int)Map_Random_Range_005797a0(0,Map.MapSize.Height-1);
            Cell firstOffset=Cell(1,(short)Map.MapSize.Width)+Cell((short)x,(short)(0u-(unsigned)x));
            seed=firstOffset+Cell((short)y,(short)y);
            if(++tries>=200)return false;
            int region=UnknownMapGenerationCell::Get_Region(seed);
            if(region==0 && Map[seed].unknown_47b590() && unknown_map_cells_a70a24[(unsigned)(int)seed.Y*(unsigned)unknown_map_cell_side_84e804+(unsigned)(int)seed.X].Marked) {
                if(tries<200)break;
                return false;
            }
        }
    } else seed=origin;
    int processed=0;
    int maximum=76;
    if(spreadLimit>=76)maximum=spreadLimit;
    double mean=(double)(spreadLimit/3);
    int spreadCount=(int)Map_Clamped_Gaussian_005798f0(mean,(double)(spreadLimit/6),75.0,(double)maximum);
    heap->Clear();
    int used=1;
    nodes[0].Position=seed;
    nodes[0].Key=0.0f;
    unsigned seedIndex=(unsigned)(int)seed.Y*(unsigned)unknown_map_cell_side_84e804+(unsigned)(int)seed.X;
    unknown_map_cells_a70a24[seedIndex].unknown_3c=(int)WorkingRegionID;
    heap->Insert(nodes);
    UnknownVeinNode *node=heap->Remove_Min();
    while(processed<spreadCount) {
        {
            if(node==0 || !success)break;
            Set_Cell_Region_Index_0056e970(node->Position,(int)WorkingRegionID);
            CellClass *current=&Map[node->Position];
            current->IsoTileIndex=unknown_tileset_a52580;
            current->unknown_11a=0;
            int facing=0;
            UnknownVeinNode *next=nodes+used;
            for(;facing<8;facing+=2) {
                Cell adjacent=CellClass::Adjacent_Cell(node->Position,facing);
                if(Generation_Cell_In_Bounds_0058da10(adjacent)) {
                    unsigned index=(unsigned)(int)adjacent.Y*(unsigned)unknown_map_cell_side_84e804+(unsigned)(int)adjacent.X;
                    UnknownMapGenerationCell &data=unknown_map_cells_a70a24[index];
                    if(data.RegionIndex==0 && data.unknown_3c!=(int)WorkingRegionID && Map[adjacent].unknown_47b590() && data.Marked) {
                        next->Position=adjacent;
                        next->Key=(float)Weighted_Cell_Distance_0057ec60(&seed,&adjacent,processed);
                        data.unknown_3c=(int)WorkingRegionID;
                        ++used;
                        heap->Insert(next++);
                    } else if(data.RegionIndex!=0 && data.RegionIndex!=(int)WorkingRegionID)success=0;
                }
            }
            ++processed;
            node=heap->Remove_Min();
        }
    }
    UnknownVeinNode *remaining=heap->Remove_Min();
    while(remaining) {
        if(!success)break;
        CellClass *current=&Map[remaining->Position];
        unsigned index=(unsigned)(int)remaining->Position.Y*(unsigned)unknown_map_cell_side_84e804+(unsigned)(int)remaining->Position.X;
        UnknownMapGenerationCell &data=unknown_map_cells_a70a24[index];
        if(data.RegionIndex==0 && current->unknown_47b590() && data.Marked) {
            current->IsoTileIndex=unknown_tileset_a52580;
            current->unknown_11a=0;
            data.RegionIndex=(int)WorkingRegionID;
        } else success=0;
        remaining=heap->Remove_Min();
        ++processed;
    }
    if(success) {
        if(processed<=75 || processed<=spreadCount/4)success=0;
        else {
            success=1;
            if(origin==Cell(0,0))success=Map.Unknown_0055C2C0((int)WorkingRegionID,0);
        }
    }
    if(success && origin==Cell(0,0)) {
        success=Grow_Region((int)WorkingRegionID,1,Rect(0,0,512,512),0,0);
        if(success) {
            int region=(int)WorkingRegionID;
            Map.Reset_Iterator();
            for(CellClass *cell=Map.Iterate();cell;cell=Map.Iterate()) {
                int index=UnknownMapGenerationCell::Get_Region(cell->MapCoords);
                if(index==region && (cell->IsoTileIndex==0 || cell->IsoTileIndex==0xffff))cell->IsoTileIndex=unknown_tileset_a52c60;
            }
            success=1;
        }
    }
    delete heap;
    delete[] nodes;
    if(success) {
        SeededWaterAmount=(int)((unsigned)SeededWaterAmount+(unsigned)processed);
        return true;
    }
    Clear_Region_Cells((int)WorkingRegionID,0,-1,0);
    return false;
}


extern double const unknown_water_scale_7a5f38,unknown_water_offset_79b7f0;
int MapGeneratorClass::Get_Target_Water_Amount() const
{
    int water=SeedData.WaterAmount;
    if(water==0)return 0;
    return (int)((double)LocalHeight*LocalWidth*water*unknown_water_scale_7a5f38+unknown_water_offset_79b7f0);
}


// Weighted cell-distance search key (TibSun MapGeneratorClass::Get_Spread_Score); name/stdcall/free/return
// precision are hypotheses. The random term is the inlined Map_Random_Fraction_00579770 callback.
extern float const unknown_distance_scale_7a5c9c,unknown_jitter_scale_7a5ca0,unknown_weight_scale_7a5ca4;
double __cdecl Map_Random_Fraction_00579770();
double __stdcall Weighted_Cell_Distance_0057ec60(Cell const *from,Cell const *to,int weight)
{
    int dx=(int)from->X-(int)to->X;
    int dy=(int)from->Y-(int)to->Y;
    int squared=(int)((unsigned)dy*(unsigned)dy+(unsigned)dx*(unsigned)dx);
    float distance=Fast_Sqrt((double)squared);
    return unknown_jitter_scale_7a5ca0*Map_Random_Fraction_00579770()+distance*unknown_distance_scale_7a5c9c-weight*unknown_weight_scale_7a5ca4;
}


// TibSun MapGeneratorClass::Seed_River (shape reference); the member keeps its unknown_ name until a rename lands.
double __stdcall Map_Clamped_Gaussian_005798f0(double,double,double,double);
bool __fastcall Generation_Cell_In_Bounds_0058da10(Cell const &position);
extern double const unknown_random_scale_7a5dd0;
extern double const unknown_lake_chance_7a5cb0;
static inline double Random_Fraction()
{
    return (unsigned int)unknown_map_random_a705c0()*unknown_random_scale_7a5dd0;
}
static inline void Clear_River_Region(int region,int const &height_source)
{
    Map.Reset_Iterator();
    CellClass *cellptr=Map.Iterate();
    while(cellptr!=0) {
        Cell position=cellptr->MapCoords;
        UnknownMapGenerationCell &data=MapRegionClass::Get_Cell_Data(position);
        if(data.RegionIndex==region) {
            data.RegionIndex=0;
            data.unknown_4b=0;
            cellptr->IsoTileIndex=0;
            cellptr->unknown_11a=0;
            int height=height_source;
            cellptr->unknown_11b=height;
        }
        cellptr=Map.Iterate();
    }
}
bool MapGeneratorClass::Seed_River(Cell const &point,double value,bool flag)
{
    bool orig_subcall=flag;
    unsigned char success=1;
    int waterfall_count=0;
    Cell seed_cell;
    double angle;

    if(point==Cell(0,0)) {
        Cell corner(1,Map.MapSize.Width);
        Cell far_corner=corner+Cell(Map.MapSize.Width,-Map.MapSize.Width)+Cell(Map.MapSize.Height,Map.MapSize.Height)+Cell(-2,0);
        int dir4=Map_Random_Range_005797a0(0,3);
        int x=Map_Random_Range_005797a0(0,Map.MapSize.Width-1);
        int y=Map_Random_Range_005797a0(0,Map.MapSize.Height-1);
        Cell candidates[4];
        candidates[0]=corner+Cell(x,-x);
        candidates[1]=far_corner-Cell(y,y);
        candidates[2]=far_corner-Cell(x,-x);
        candidates[3]=corner+Cell(y,y);
        double base_angle=5.497787143782138-(double)dir4*1.5707963267948966;
        seed_cell=candidates[dir4];
        angle=Map_Clamped_Gaussian_005798f0(base_angle,0.5235987755982988,base_angle-0.7853981633974483,base_angle+0.7853981633974483);
    } else {
        seed_cell=point;
        angle=value;
    }

    if(!Generation_Cell_In_Bounds_0058da10(seed_cell)) {
        return false;
    }

    int seeded_count=0;

    double width_d=(double)SeedData.WaterAmount*0.07;
    if(1.0>width_d) {
        width_d=1.0;
    }
    int width_top=(int)width_d;
    double start_width=(double)Map_Random_Range_005797a0(1,width_top);
    double width=start_width;
    int half=(int)start_width/2;
    double width_hi=(double)half+start_width;
    double width_lo=start_width-(double)half;
    int length=Map_Random_Range_005797a0(35,125);
    double cur_x=(double)seed_cell.X+0.5;
    double cur_y=(double)seed_cell.Y+0.5;
    double cur_angle=angle;
    double angle_hi=angle+1.5707963267948966;
    double angle_lo=angle-1.5707963267948966;
    double stop_roll=1.0;

    while(Generation_Cell_In_Bounds_0058da10(Cell((int)cur_x,(int)cur_y)) && success) {
        double step_y=Fast_Cos(cur_angle);
        double step_x=Fast_Sin(cur_angle);
        int span=(int)(width+0.5);
        bool x_run=true;
        bool y_run=true;
        double offset_x=(double)(span-1)*step_x*0.5;
        double px=cur_x-offset_x;
        double offset_y=(double)(span-1)*step_y*0.5;
        double py=cur_y-offset_y;

        if(span>0) {
            int i=span;
            Cell pcell;

            while(true) {
                if(Generation_Cell_In_Bounds_0058da10(Cell((int)px,(int)py))) {
                    pcell=Cell((int)px,(int)py);

                    if(UnknownMapGenerationCell::Get_Region(pcell)==0 && Map[pcell].unknown_47b590() || UnknownMapGenerationCell::Get_Region(pcell)==(int)WorkingRegionID) {
                        if(unknown_map_cells_a70a24) {
                            MapRegionClass::Get_Cell_Data(pcell).RegionIndex=WorkingRegionID;
                        }
                        CellClass *cellptr=&Map[Cell((int)px,(int)py)];
                        cellptr->IsoTileIndex=unknown_tileset_a52580;
                        cellptr->unknown_11a=0;
                    } else {
                        success=false;
                    }
                }

                double next_x=px+step_x;
                if((int)px!=(int)next_x) {
                    x_run=false;
                }

                double next_y=py+step_y;
                if((int)py!=(int)next_y) {
                    y_run=false;
                }

                px=next_x;
                py=next_y;
                i--;

                if(i==0) {
                    break;
                }
            }
        }

        if(fabs(step_y)>fabs(step_x)) {
            y_run=false;
        } else {
            x_run=false;
        }

        if(!flag && (x_run || y_run) && waterfall_count<1 && (double)abs((int)(cur_angle-angle))<0.7853981633974483) {
            if(PlaceWaterfall) {
                if(seeded_count>length) {
                    int type;

                    if(x_run) {
                        type=(step_y>0.0)?2:6;
                    } else {
                        type=(step_x>0.0)?0:4;
                    }

                    bool placed=false;
                    Cell from((int)(px-step_x),(int)(py-step_y));
                    Cell to((int)(cur_x-offset_x),(int)(cur_y-offset_y));
                    Place_Waterfall(WorkingRegionID,to,from,type,placed,cur_x,cur_y);

                    if(placed) {
                        WorkingRegionID++;
                        waterfall_count++;
                    }
                }
            }
        }

        cur_x+=step_y;
        cur_y-=step_x;

        if(Random_Fraction()<0.01) {
            if(success) {
                if(!flag && waterfall_count==0) {
                    flag=true;
                    double branch_mean=cur_angle+1.5707963267948966;
                    double branch_lo=cur_angle+0.5235987755982988;
                    double branch_hi=cur_angle+2.6179938779914944;
                    double branch_deviation=0.5235987755982988;
                    if(branch_mean-branch_deviation>branch_hi || branch_mean+branch_deviation<branch_lo) {
                        branch_deviation=(branch_hi-branch_lo)*0.5;
                        branch_mean=branch_deviation+branch_lo;
                    }
                    double branch_angle;
                    do {
                        branch_angle=unknown_map_gaussian_a6fd50.Next()*branch_deviation+branch_mean;
                    } while(branch_angle<branch_lo || branch_angle>branch_hi);
                    success=Seed_River(Cell((int)px,(int)py),branch_angle,true);
                }
            }
        }

        if(seeded_count>5) {
            double delta_angle=Map_Clamped_Gaussian_005798f0(0.0,0.3141592653589793,angle_lo-cur_angle,angle_hi-cur_angle);
            cur_angle+=delta_angle;
        }

        if(half>0) {
            double step_hi=width_hi-width;
            double step_lo=width_lo-width;
            double step_dev=0.5;
            double step_mean=0.0;
            if(step_mean-step_dev>step_hi || step_mean+step_dev<step_lo) {
                step_dev=(step_hi-step_lo)*0.5;
                step_mean=step_dev+step_lo;
            }
            double delta_width;
            do {
                delta_width=unknown_map_gaussian_a6fd50.Next()*step_dev+step_mean;
            } while(delta_width<step_lo || delta_width>step_hi);
            width+=delta_width;
        }

        stop_roll=Random_Fraction();
        seeded_count++;

        if(stop_roll<0.005) {
            break;
        }
    }

    if(seeded_count<40) {
        success=false;
    }

    if(stop_roll<0.005) {
        if(Generation_Cell_In_Bounds_0058da10(Cell((int)cur_x,(int)cur_y)) && success) {
            if(!Seed_Lake(Cell((int)cur_x,(int)cur_y))) {
                success=false;
            }
        }
    }

    if(!orig_subcall && success) {
        success=Map.Unknown_0055C2C0(WorkingRegionID,0);

        if(success) {
            success=Grow_Region(WorkingRegionID,1,Rect(0,0,0x200,0x200),0,0);
            int region_id=WorkingRegionID;
            Map.Reset_Iterator();
            CellClass *cptr=Map.Iterate();

            while(cptr!=0) {
                if(UnknownMapGenerationCell::Get_Region(cptr->MapCoords)==region_id && (cptr->IsoTileIndex==0 || cptr->IsoTileIndex==0xffff)) {
                    cptr->IsoTileIndex=unknown_tileset_a52c60;
                }
                cptr=Map.Iterate();
            }
        }
    }

    bool raised=false;

    if(waterfall_count==0 && success && CellHeight==4 && !orig_subcall) {
        if(Random_Fraction()<unknown_lake_chance_7a5cb0) {
            Rect bounds(0,0,0x200,0x200);

            if(Grow_Water_Region(WorkingRegionID,0.01f,bounds,seed_cell,true)) {
                success=Grow_Region(WorkingRegionID,6,Rect(0,0,0x200,0x200),0,0);

                if(success) {
                    int region_id=WorkingRegionID;
                    Map.Reset_Iterator();
                    CellClass *cptr=Map.Iterate();

                    while(cptr!=0) {
                        Cell c=cptr->MapCoords;
                        int rid=UnknownMapGenerationCell::Get_Region(c);

                        if(rid!=region_id) {
                            cptr->unknown_11b+=4;
                        }

                        cptr=Map.Iterate();
                    }

                    raised=true;
                }
            } else {
                success=false;
            }
        }
    }

    if(!orig_subcall && success && waterfall_count==0) {
        if(!raised) {
            success=Grow_Region(WorkingRegionID,2,Rect(0,0,0x200,0x200),0,0);
        }
    } else if(waterfall_count>0) {
        if(success) {
            success=Grow_Region(WorkingRegionID,2,Rect(0,0,0x200,0x200),1,CellHeight);
        }
    }

    if(success) {
        if(raised) {
            CellHeight+=4;
        }

        SeededWaterAmount+=seeded_count;
        return true;
    }

    Clear_River_Region(WorkingRegionID,CellHeight);

    if(waterfall_count>0) {
        Clear_River_Region(WorkingRegionID-1,CellHeight);
    }

    return false;
}


// Region cell-byte iteration; operation/name and free-stdcall source spelling are hypotheses.
void __stdcall Set_Region_Cell_Byte_0057fe20(int index,char value)
{
    Map.Reset_Iterator();
    for(CellClass *cell=Map.Iterate();cell;cell=Map.Iterate()) {
        Cell position=cell->MapCoords;
        int region=!unknown_map_cells_a70a24 ? -1 : unknown_map_cells_a70a24[position.Y*unknown_map_cell_side_84e804+position.X].RegionIndex;
        if(region==index)
            cell->unknown_11b = value;
    }
}


// Region cell-byte iteration; operation/name and free-stdcall source spelling are hypotheses.
void __stdcall Increment_Region_Cell_Byte_0057fea0(int index,char value)
{
    Map.Reset_Iterator();
    for(CellClass *cell=Map.Iterate();cell;cell=Map.Iterate()) {
        Cell position=cell->MapCoords;
        int region=!unknown_map_cells_a70a24 ? -1 : unknown_map_cells_a70a24[position.Y*unknown_map_cell_side_84e804+position.X].RegionIndex;
        if(region==index)
            cell->unknown_11b += value;
    }
}


// Place_Waterfall 0x57FF20: bank the river into a waterfall. Same flow as TibSun MapGeneratorClass::Place_Waterfall.
bool __fastcall Generation_Cell_In_Bounds_0058da10(Cell const &position);
bool MapGeneratorClass::Place_Waterfall(int region_id,Cell const &cell1,Cell const &cell2,int direction,bool &placed,double &head_x,double &head_y)
{
	placed = false;

	Rect pre(0,0,0,0);

	Rect seed(0,0,0,0);
	Rect patch(0,0,0,0);
	Rect bounds_rect(0,0,0,0);
	Cell center;

	switch (direction) {
		case 0: {
			pre=Rect(cell1.X - 2,cell1.Y - 12,cell2.X - cell1.X + 5,12);

			seed=Rect(cell1.X,cell1.Y - 4,cell2.X - cell1.X + 1,4);

			patch=Rect(cell1.X,cell1.Y - 12,cell2.X - cell1.X + 1,8);

			bounds_rect = Rect(0, seed.Y, 0x200, 0x200 - seed.Y);
			center = Cell(seed.X, seed.Y);
			break;
		}
		case 4: {
			pre=Rect(cell2.X - 2,cell2.Y + 1,cell1.X - cell2.X + 5,12);

			seed=Rect(cell2.X,cell2.Y + 1,cell1.X - cell2.X + 1,4);

			patch=Rect(cell2.X,cell2.Y + 5,cell1.X - cell2.X + 1,8);

			bounds_rect = Rect(0, 0, 0x200, patch.Y - 1);
			center = Cell(seed.X, seed.Y);
			break;
		}
		case 2: {
			pre=Rect(cell1.X + 1,cell1.Y - 2,12,cell2.Y - cell1.Y + 5);

			seed=Rect(cell1.X + 1,cell1.Y,4,cell2.Y - cell1.Y + 1);

			patch=Rect(cell1.X + 5,cell1.Y,8,cell2.Y - cell1.Y + 1);

			bounds_rect = Rect(0, 0, patch.X - 1, 0x200);
			center = Cell(patch.X, patch.Y);
			break;
		}
		case 6: {
			pre=Rect(cell2.X - 12,cell2.Y - 2,12,cell1.Y - cell2.Y + 5);

			seed=Rect(cell2.X - 4,cell2.Y,4,cell1.Y - cell2.Y + 1);

			patch=Rect(cell2.X - 12,cell2.Y,8,cell1.Y - cell2.Y + 1);

			bounds_rect = Rect(seed.X, 0, 0x200 - seed.X, 0x200);
			center = Cell(seed.X, seed.Y);
			break;
		}
		default:
			break;
	}

	int yy;
	int xx;

	for (yy = pre.Y; yy < pre.Y + pre.Height; yy++) {
		for (xx = pre.X; xx < pre.X + pre.Width; xx++) {
			if (Generation_Cell_In_Bounds_0058da10(Cell(xx, yy))) {
				if (UnknownMapGenerationCell::Get_Region(Cell(xx, yy)) != 0) {
					return(true);
				}

				if (!Map[Cell(xx, yy)].unknown_47b590()) {
					return(true);
				}
			}
		}
	}

	for (yy = seed.Y; yy < seed.Y + seed.Height; yy++) {
		for (xx = seed.X; xx < seed.X + seed.Width; xx++) {
			if (Generation_Cell_In_Bounds_0058da10(Cell(xx, yy))) {
				int tile_index = Map_Random_Range_005797a0(0, 5);
				Map[Cell(xx, yy)].IsoTileIndex = tile_index+unknown_tileset_a52580;

				int subtile = Map_Random_Range_005797a0(0, 3);
				Map[Cell(xx, yy)].unknown_11a = subtile;

				if(unknown_map_cells_a70a24) {
								MapRegionClass::Get_Cell_Data(Cell(xx,yy)).RegionIndex=region_id;
							}
			}
		}
	}

	bool success = Grow_Water_Region(WorkingRegionID, 0.003f, bounds_rect, center, false);
	if (success) {
		success = Map.Unknown_0055C2C0(WorkingRegionID,0);
		if (success) {
			success = Grow_Region(WorkingRegionID, 2, bounds_rect, 0, 0);
			if (success) {
				int region = WorkingRegionID;
				Map.Reset_Iterator();
				CellClass * cptr = Map.Iterate();
				while (cptr !=0) {
					Cell c = cptr->MapCoords;
					int other_region = UnknownMapGenerationCell::Get_Region(c);
					if (other_region == region) {
						cptr->unknown_11b += 4;
					}
					cptr = Map.Iterate();
				}

				for (yy = patch.Y; yy < patch.Y + patch.Height; yy++) {
					for (xx = patch.X; xx < patch.X + patch.Width; xx++) {
						if (Generation_Cell_In_Bounds_0058da10(Cell(xx, yy))) {
							int tile_index = Map_Random_Range_005797a0(0, 5);
							Map[Cell(xx, yy)].IsoTileIndex = tile_index+unknown_tileset_a52580;

							int subtile = Map_Random_Range_005797a0(0, 3);
							Map[Cell(xx, yy)].unknown_11a = subtile;

							if(unknown_map_cells_a70a24) {
								MapRegionClass::Get_Cell_Data(Cell(xx,yy)).RegionIndex=region_id;
							}
						}
					}
				}

				bool place_ok = true;
				
				switch (direction) {
					case 2: {
						Cell base(patch.X, patch.Y + patch.Height);
						int height = Map[base + Cell(2, 0)].unknown_11b;

						Map.unknown_11a4 = IsoTileTypes[unknown_tileset_a52584];
						Map.Unknown_004994D0(base);
						Map.Pick_Random_Tile_Variant(0,0x10000,height,WorkingRegionID,place_ok,false);

						Map.unknown_11a4 = IsoTileTypes[unknown_tileset_a52584+3];
						Map.Unknown_004994D0(base - Cell(0, patch.Height + 2));
						Map.Pick_Random_Tile_Variant(0,0x10000,height,WorkingRegionID,place_ok,false);

						Cell top = base + Cell(0, 2);
						Cell bottom = base - Cell(0, patch.Height + 3);

						if (Generation_Cell_In_Bounds_0058da10(top)) {
							Map[top].IsoTileIndex = 0xffff;
							Map[top].unknown_11b += 4;
							Map[top].unknown_11a = 0;
							if(unknown_map_cells_a70a24) {
								MapRegionClass::Get_Cell_Data(top).RegionIndex=WorkingRegionID;
							}
						}

						if (Generation_Cell_In_Bounds_0058da10(bottom)) {
							Map[bottom].IsoTileIndex = 0xffff;
							Map[bottom].unknown_11b += 4;
							Map[bottom].unknown_11a = 0;
							if(unknown_map_cells_a70a24) {
								MapRegionClass::Get_Cell_Data(bottom).RegionIndex=WorkingRegionID;
							}
						}

						int seg = 0;
						if (seg < patch.Height) {
						int remaining = patch.Height;
						do {
							if (remaining % 2) {
								Map.Unknown_004994D0(base - Cell(0, seg + 1));
								Map.unknown_11a4 = IsoTileTypes[unknown_tileset_a52584 + 1];
								seg++;
								remaining--;
							} else {
								Map.Unknown_004994D0(base - Cell(0, seg + 2));
								Map.unknown_11a4 = IsoTileTypes[unknown_tileset_a52584 + 2];
								seg += 2;
								remaining -= 2;
							}
							Map.Pick_Random_Tile_Variant(0,0x10000,height,WorkingRegionID,place_ok,false);
						} while (seg < patch.Height);
						}
						break;
					}

					case 6: {
						Cell base(seed.X - 2, patch.Y + patch.Height);
						int height = Map[base - Cell(2, 0)].unknown_11b;

						Map.unknown_11a4 = IsoTileTypes[unknown_tileset_a6cf58];
						Map.Unknown_004994D0(base);
						Map.Pick_Random_Tile_Variant(0,0x10000,height,WorkingRegionID,place_ok,false);

						Map.unknown_11a4 = IsoTileTypes[unknown_tileset_a6cf58+3];
						Map.Unknown_004994D0(base - Cell(0, patch.Height + 2));
						Map.Pick_Random_Tile_Variant(0,0x10000,height,WorkingRegionID,place_ok,false);

						Map[base - Cell(0, 1)].IsoTileIndex = 0xffff;
						Map[base + Cell(0, patch.Width + 2)].IsoTileIndex = 0xffff;

						for (int i = 0; i < patch.Height + 2; i++) {
							Map[base - Cell(0, i)].unknown_11b -= 4;
						}

						Cell top = base + Cell(1, 2);
						Cell bottom = base - Cell(-1, patch.Height + 3);

						if (Generation_Cell_In_Bounds_0058da10(top)) {
							Map[top].IsoTileIndex = 0xffff;
							Map[top].unknown_11b += 4;
							Map[top].unknown_11a = 0;
							if(unknown_map_cells_a70a24) {
								MapRegionClass::Get_Cell_Data(top).RegionIndex=WorkingRegionID;
							}
						}

						if (Generation_Cell_In_Bounds_0058da10(bottom)) {
							Map[bottom].IsoTileIndex = 0xffff;
							Map[bottom].unknown_11b += 4;
							Map[bottom].unknown_11a = 0;
							if(unknown_map_cells_a70a24) {
								MapRegionClass::Get_Cell_Data(bottom).RegionIndex=WorkingRegionID;
							}
						}

						int seg = 0;
						if (seg < patch.Height) {
						int remaining = patch.Height;
						do {
							if (remaining % 2) {
								Map.Unknown_004994D0(base - Cell(-1, seg + 1));
								Map.unknown_11a4 = IsoTileTypes[unknown_tileset_a6cf58 + 1];
								seg++;
								remaining--;
							} else {
								Map.Unknown_004994D0(base - Cell(-1, seg + 2));
								Map.unknown_11a4 = IsoTileTypes[unknown_tileset_a6cf58 + 2];
								seg += 2;
								remaining -= 2;
							}
							Map.Pick_Random_Tile_Variant(0,0x10000,height,WorkingRegionID,place_ok,false);
						} while (seg < patch.Height);
						}
						break;
					}

					case 4: {
						Cell base(patch.X - 2, patch.Y);
						int height = Map[base + Cell(0, 2)].unknown_11b;

						Map.unknown_11a4 = IsoTileTypes[unknown_tileset_a52ea0];
						Map.Unknown_004994D0(base);
						Map.Pick_Random_Tile_Variant(0,0x10000,height,WorkingRegionID,place_ok,false);

						Map.unknown_11a4 = IsoTileTypes[unknown_tileset_a52ea0+3];
						Map.Unknown_004994D0(base + Cell(patch.Width + 2, 0));
						Map.Pick_Random_Tile_Variant(0,0x10000,height,WorkingRegionID,place_ok,false);

						Cell left = base + Cell(-1, 0);
						Cell right = base + Cell(patch.Width + 4, 0);

						if (Generation_Cell_In_Bounds_0058da10(left)) {
							Map[left].IsoTileIndex = 0xffff;
							Map[left].unknown_11b += 4;
							Map[left].unknown_11a = 0;
							if(unknown_map_cells_a70a24) {
								MapRegionClass::Get_Cell_Data(left).RegionIndex=WorkingRegionID;
							}
						}

						if (Generation_Cell_In_Bounds_0058da10(right)) {
							Map[right].IsoTileIndex = 0xffff;
							Map[right].unknown_11b += 4;
							Map[right].unknown_11a = 0;
							if(unknown_map_cells_a70a24) {
								MapRegionClass::Get_Cell_Data(right).RegionIndex=WorkingRegionID;
							}
						}

						int seg = 0;
						if (seg < patch.Width) {
						int remaining = patch.Width;
						do {
							if (remaining % 2) {
								Map.Unknown_004994D0(base + Cell(seg + 2, 0));
								Map.unknown_11a4 = IsoTileTypes[unknown_tileset_a52ea0 + 1];
								seg++;
								remaining--;
							} else {
								Map.Unknown_004994D0(base + Cell(seg + 2, 0));
								Map.unknown_11a4 = IsoTileTypes[unknown_tileset_a52ea0 + 2];
								seg += 2;
								remaining -= 2;
							}
							Map.Pick_Random_Tile_Variant(0,0x10000,height,WorkingRegionID,place_ok,false);
						} while (seg < patch.Width);
						}
						break;
					}

					case 0: {
						Cell base(seed.X - 2, seed.Y - 2);
						int height = Map[base - Cell(0, 2)].unknown_11b;

						Map.unknown_11a4 = IsoTileTypes[unknown_tileset_a52ef0];
						Map.Unknown_004994D0(base);
						Map.Pick_Random_Tile_Variant(0,0x10000,height,WorkingRegionID,place_ok,false);

						Map.unknown_11a4 = IsoTileTypes[unknown_tileset_a52ef0+3];
						Map.Unknown_004994D0(base + Cell(patch.Width + 2, 0));
						Map.Pick_Random_Tile_Variant(0,0x10000,height,WorkingRegionID,place_ok,false);

						Map[base + Cell(1, 0)].IsoTileIndex = 0xffff;
						Map[base + Cell(patch.Width + 2, 0)].IsoTileIndex = 0xffff;

						for (int i = 0; i < patch.Width + 2; i++) {
							Map[base + Cell(i + 1, 0)].unknown_11b -= 4;
						}

						Cell left = base + Cell(-1, 1);
						Cell right = base + Cell(patch.Width + 4, 1);

						if (Generation_Cell_In_Bounds_0058da10(left)) {
							Map[left].IsoTileIndex = 0xffff;
							Map[left].unknown_11b += 4;
							Map[left].unknown_11a = 0;
							if(unknown_map_cells_a70a24) {
								MapRegionClass::Get_Cell_Data(left).RegionIndex=WorkingRegionID;
							}
						}

						if (Generation_Cell_In_Bounds_0058da10(right)) {
							Map[right].IsoTileIndex = 0xffff;
							Map[right].unknown_11b += 4;
							Map[right].unknown_11a = 0;
							if(unknown_map_cells_a70a24) {
								MapRegionClass::Get_Cell_Data(right).RegionIndex=WorkingRegionID;
							}
						}

						int seg = 0;
						if (seg < patch.Width) {
						int remaining = patch.Width;
						do {
							if (remaining % 2) {
								Map.Unknown_004994D0(base + Cell(seg + 2, 1));
								Map.unknown_11a4 = IsoTileTypes[unknown_tileset_a52ef0 + 1];
								seg++;
								remaining--;
							} else {
								Map.Unknown_004994D0(base + Cell(seg + 2, 1));
								Map.unknown_11a4 = IsoTileTypes[unknown_tileset_a52ef0 + 2];
								seg += 2;
								remaining -= 2;
							}
							Map.Pick_Random_Tile_Variant(0,0x10000,height,WorkingRegionID,place_ok,false);
						} while (seg < patch.Width);
						}
						break;
					}

					default:
						break;
				}
			}
		}
	}

	int x_offsets[4] = { 0, 12, 0, -12 };
	int y_offsets[4] = { -12, 0, 12, 0 };

	if (success) {
		head_x += (double)x_offsets[direction / 2];
		head_y += (double)y_offsets[direction / 2];
	}

	placed = success;
	return(success);
}


// Sets the region of a cell's generator record (TibSun MapGeneratorClass::Set_Cell_Data_Region
// over CellData::Set_Region 0x56E970; ECX owner not read, RET8).
void MapGeneratorClass::Set_Cell_Data_Region(Cell const &cell,int region_id)
{
    Set_Cell_Region_Index_0056e970(cell,region_id);
}


// The region of a cell's generator record, or -1 without generator data (TibSun
// MapGeneratorClass::Get_Cell_Data_Region over CellData::Get_Region; ECX owner not read, RET4).
int MapGeneratorClass::Get_Cell_Data_Region(Cell const &cell)
{
    return UnknownMapGenerationCell::Get_Region(cell);
}


// Marks a cell's generator record as reached by the spread patch_id (TibSun
// MapGeneratorClass::Set_Cell_Data_Spread; ECX owner not read, RET8).
void MapGeneratorClass::Set_Cell_Data_Spread(Cell const &cell,int patch_id)
{
    MapRegionClass::Get_Cell_Data(cell).unknown_3c=patch_id;
}


// The spread that last reached a cell's generator record (TibSun
// MapGeneratorClass::Get_Cell_Data_Spread; ECX owner not read, RET4).
int MapGeneratorClass::Get_Cell_Data_Spread(Cell const &cell)
{
    return MapRegionClass::Get_Cell_Data(cell).unknown_3c;
}


bool __fastcall Generation_Cell_In_Bounds_0058da10(Cell const &position);
bool MapGeneratorClass::Grow_Region(int region,int rings,Rect area,char setHeight,int height)
{
    DynamicVectorClass<Cell> *border=Build_Region_Border_Cell_List(region);
    DynamicVectorClass<Cell> *next;
    for(int ring=0;ring<rings;++ring) {
        next=new DynamicVectorClass<Cell>;
        next->Set_Growth_Step(3*border->Count());
        for(int index=0;index<border->Count();++index) {
            for(int facing=0;facing<8;++facing) {
                Cell const &borderCell=(*border)[index];
                Cell position=CellClass::Adjacent_Cell(borderCell,facing);
                if(Generation_Cell_In_Bounds_0058da10(position) && area.Is_Point_Within(Point2D(position.X,position.Y))) {
                    UnknownMapGenerationCell &data=MapRegionClass::Get_Cell_Data(position);
                    int oldRegion=data.RegionIndex;
                    bool reclaim=setHeight && oldRegion==region-1;
                    CellClass &cell=Map[position];
                    if((oldRegion==0 || reclaim) && (cell.unknown_47b590() || (reclaim && cell.unknown_47b7e0()))) {
                        next->Add(position);
                        data.RegionIndex=region;
                        if(setHeight && cell.unknown_47b590())Map[position].unknown_11b=height;
                    } else if(oldRegion!=region) {
                        delete border;
                        delete next;
                        return false;
                    }
                }
            }
        }
        delete border;
        border=next;
    }
    delete border;
    return true;
}


bool __fastcall Generation_Cell_In_Bounds_0058da10(Cell const &position);
Cell __fastcall CellClass::Adjacent_Cell(Cell const &cell,int facing)
{
    return cell + unknown_adjacent_851a70[facing&7];
}
bool MapGeneratorClass::Clear_Region_Border(int region,int rings,int newRegion)
{
    int ring,index;
    DynamicVectorClass<Cell> *border=Build_Region_Border_Cell_List(region);
    if(rings>1) {
        Map.Reset_Iterator();
        for(CellClass *cell=Map.Iterate();cell;cell=Map.Iterate()) {
            Cell position=cell->MapCoords;
            unknown_map_cells_a70a24[position.Y*unknown_map_cell_side_84e804+position.X].unknown_3c=0;
        }
    }
    for(ring=0;ring<rings;++ring) {
        for(index=border->Count()-1;index>=0;--index) {
            Cell position=(*border)[index];
            if(unknown_map_cells_a70a24)unknown_map_cells_a70a24[position.Y*unknown_map_cell_side_84e804+position.X].RegionIndex=newRegion;
            CellClass &cell=Map[position];
            cell.IsoTileIndex=0;
            cell.unknown_11a=0;
            cell.unknown_11b=(char)CellHeight;
        }
        if(ring<rings-1) {
            DynamicVectorClass<Cell> *next=new DynamicVectorClass<Cell>;
            next->Set_Growth_Step(border->Count());
            for(index=border->Count()-1;index>=0;--index) {
                Cell position=(*border)[index];
                for(int facing=0;facing<8;++facing) {
                    Cell adjacent=CellClass::Adjacent_Cell(position,facing);
                    if(Generation_Cell_In_Bounds_0058da10(adjacent)) {
                        UnknownMapGenerationCell &data=unknown_map_cells_a70a24[adjacent.Y*unknown_map_cell_side_84e804+adjacent.X];
                        if(data.RegionIndex==region && data.unknown_3c!=ring+1) {
                            next->Add(adjacent);
                            data.unknown_3c=ring+1;
                        }
                    }
                }
            }
            delete border;
            border=next;
        }
    }
    delete border;
    if(rings>1) {
        Map.Reset_Iterator();
        for(CellClass *cell=Map.Iterate();cell;cell=Map.Iterate()) {
            Cell position=cell->MapCoords;
            unknown_map_cells_a70a24[position.Y*unknown_map_cell_side_84e804+position.X].unknown_3c=0;
        }
    }
    return true;
}


// Collect the nonzero cells in this region that have an in-bounds neighbor in another region.

static inline bool In_Zone_Map_Bounds(Cell const &position);
static inline int Read_Generation_Region(Cell const &position);

DynamicVectorClass<Cell> *MapGeneratorClass::Build_Region_Border_Cell_List(int region)
{
    DynamicVectorClass<Cell> *border = new DynamicVectorClass<Cell>;
    border->Set_Growth_Step((int)((unsigned)LocalHeight * (unsigned)LocalWidth));
    int size = (int)((unsigned)unknown_map_cell_side_84e804 * (unsigned)unknown_map_cell_side_84e804);
    for (int index = 0; index < size; ++index) {
        Cell position = unknown_map_cells_a70a24[index].Position;
        if ((position.X != 0 || position.Y != 0) && Read_Generation_Region(position) == region) {
            bool boundary = false;
            for (int facing = 0; facing < 8; ++facing) {
                Cell adjacent = CellClass::Adjacent_Cell(position, facing);
                if (In_Zone_Map_Bounds(adjacent) && Read_Generation_Region(adjacent) != region) {
                    boundary = true;
                    break;
                }
            }
            if (boundary) {
                border->Add(position);
            }
        }
    }
    return border;
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// From ra2-decomp 0x005A1190 MapGeneratorClass::Get_Angle_Score (src/mapgen.cpp, YR twin): the angular
// score of a cell seen from another against a reference heading, plus a random jitter. The pi constants and
// 1/UINT_MAX are literals (pooled __real constants at 0x79B630/0x79D158/0x79C9B8/0x7A5DD0); the two
// weights at 0x7A5CC8 (2.0, the twin's _random_weight) and 0x7A5CD0 (1.5, _angle_weight) are real data.
extern double const angle_weight_7a5cd0,jitter_weight_7a5cc8;
// ECX is unused, RET16/ST0 are physical.
double MapGeneratorClass::Get_Angle_Score(Cell const &cell1,Cell const &cell2,double ref_angle)
{
    int dx=cell2.X-cell1.X;
    int dy=cell2.Y-cell1.Y;

    double angle=(dx!=0) ? Fast_Atan(-((double)dy/(double)dx)) : 1.5707963267948966;
    if(dx<0) {
        angle+=3.141592653589793;
    }

    double angle_diff=fabs(angle-ref_angle);
    while(angle_diff>=3.141592653589793*2) {
        angle_diff-=3.141592653589793*2;
    }
    if(angle_diff>3.141592653589793) {
        angle_diff=3.141592653589793*2-angle_diff;
    }

    return(((unsigned)unknown_map_random_a705c0()*(1.0 / 4294967295.0))*jitter_weight_7a5cc8+angle_weight_7a5cd0*angle_diff);
}


// Clears every map cell's spread mark back to zero (TibSun MapGeneratorClass::Clear_Cell_Data_Spreads,
// a member in RA2's TibSun-ordered run 0x581870-0x582A50; never called, ECX not read).
void MapGeneratorClass::Clear_Cell_Data_Spreads()
{
    Map.Reset_Iterator();
    CellClass *cell=Map.Iterate();
    while(cell) {
        Set_Cell_Data_Spread(cell->Fetch_CellID(),0);
        cell=Map.Iterate();
    }
}


// Byte wrapping/ordered reads preserved; operation name and free stdcall spelling are hypotheses.
void __stdcall Increment_Other_Region_Cell_Byte_00582ab0(int value,int index)
{
    Map.Reset_Iterator();
    for(CellClass *cell=Map.Iterate();cell;cell=Map.Iterate()) {
        Cell position=cell->MapCoords;
        int region=!unknown_map_cells_a70a24 ? -1 : unknown_map_cells_a70a24[position.Y*unknown_map_cell_side_84e804+position.X].RegionIndex;
        if(region!=index)
            cell->unknown_11b+=(unsigned char)value;
    }
}


unsigned int __fastcall Map_Random_Range_005797a0(unsigned int,unsigned int);
int MapGeneratorClass::Pick_Alternate_Cliff(int tile)
{
    tile-=unknown_tileset_a52e68;
    switch(tile) {
    case 4: tile=Map_Random_Range_005797a0(1,2)+4; break;
    case 5: tile=Map_Random_Range_005797a0(0,1)*2+4; break;
    case 6: tile=Map_Random_Range_005797a0(0,1)+4; break;
    case 8: tile=Map_Random_Range_005797a0(1,2)+8; break;
    case 9: tile=Map_Random_Range_005797a0(0,1)*2+8; break;
    case 10: tile=Map_Random_Range_005797a0(0,1)+8; break;
    case 11: tile=Map_Random_Range_005797a0(1,2)+11; break;
    case 12: tile=Map_Random_Range_005797a0(0,1)*2+11; break;
    case 13: tile=Map_Random_Range_005797a0(0,1)+11; break;
    case 14: tile=Map_Random_Range_005797a0(1,2)+14; break;
    case 15: tile=Map_Random_Range_005797a0(0,1)*2+14; break;
    case 16: tile=Map_Random_Range_005797a0(0,1)+14; break;
    case 22: tile=Map_Random_Range_005797a0(1,2)+22; break;
    case 23: tile=Map_Random_Range_005797a0(0,1)*2+22; break;
    case 24: tile=Map_Random_Range_005797a0(0,1)+22; break;
    case 28: tile=29; break;
    case 29: tile=28; break;
    case 34: tile=Map_Random_Range_005797a0(1,2)+34; break;
    case 35: tile=Map_Random_Range_005797a0(0,1)*2+34; break;
    case 36: tile=Map_Random_Range_005797a0(0,1)+34; break;
    }
    return tile+unknown_tileset_a52e68;
}


// Replaces ramp tiles on sloped runs with a random variant, looking one cell south and east of each cell. Operation name is a hypothesis.
void __fastcall Place_Tile_Footprint_005883f0(int tile,Cell const &position,int region,int height);
void MapGeneratorClass::unknown_00582fd0()
{
    Map.Reset_Iterator();
    CellClass *cellptr=Map.Iterate();

    while(cellptr!=0) {
        if(cellptr->IsoTileIndex>=unknown_tileset_a52e68 && cellptr->IsoTileIndex<unknown_tileset_a52e68+40) {
            CellClass &south=cellptr->Adjacent_Cell(4);
            CellClass &east=cellptr->Adjacent_Cell(2);

            if(cellptr->IsoTileIndex==south.IsoTileIndex && (unsigned char)cellptr->unknown_11a>(unsigned char)south.unknown_11a) {
                int tile=Pick_Alternate_Cliff(cellptr->IsoTileIndex);

                if(tile!=cellptr->IsoTileIndex) {
                    int subtile=(unsigned char)south.unknown_11a;
                    IsometricTileTypeClass *type=IsoTileTypes[cellptr->IsoTileIndex];
                    Cell origin=south.MapCoords-Cell(subtile%type->unknown_2b4,subtile/type->unknown_2b4);
                    Place_Tile_Footprint_005883f0(tile,origin,UnknownMapGenerationCell::Get_Region(origin),-1);
                }
            }

            if(cellptr->IsoTileIndex==east.IsoTileIndex && (unsigned char)cellptr->unknown_11a>(unsigned char)east.unknown_11a) {
                int tile=Pick_Alternate_Cliff(cellptr->IsoTileIndex);

                if(tile!=cellptr->IsoTileIndex) {
                    IsometricTileTypeClass *type=IsoTileTypes[cellptr->IsoTileIndex];
                    int subtile=(unsigned char)east.unknown_11a;
                    Cell origin=east.MapCoords-Cell(subtile%type->unknown_2b4,subtile/type->unknown_2b4);
                    Place_Tile_Footprint_005883f0(tile,origin,UnknownMapGenerationCell::Get_Region(origin),-1);
                }
            }
        }

        cellptr=Map.Iterate();
    }
}


// Fresh complete5831C0 diagnostic. No required incoming receiver is established.
// Free/no-argvoid/name/operation spelling remain hypotheses; no invented per-function owner.
// Genuine repeated grid read/write operations; ordinary inline only, no external binding or forcing.
static inline int Read_Generation_Region(Cell const &position)
{
    UnknownMapGenerationCell *cells=unknown_map_cells_a70a24;
    unsigned side=(unsigned)unknown_map_cell_side_84e804;
    if(cells==0) {
        return -1;
    }
    unsigned at=(unsigned)(int)position.Y*side+(unsigned)(int)position.X;
    return cells[(int)at].RegionIndex;
}
static inline void Write_Generation_Region(Cell const &position,int region)
{
    UnknownMapGenerationCell *cells=unknown_map_cells_a70a24;
    unsigned side=(unsigned)unknown_map_cell_side_84e804;
    if(cells!=0) {
        unsigned at=(unsigned)(int)position.Y*side+(unsigned)(int)position.X;
        cells[(int)at].RegionIndex=region;
    }
}
void Fixup_Cliff_Corners()
{
    Map.Reset_Iterator();
    CellClass *current=Map.Iterate();
    while(current!=0) {
        CellClass &neighbor5=current->Adjacent_Cell(5);
        CellClass &neighbor1=current->Adjacent_Cell(1);
        CellClass &neighbor3=current->Adjacent_Cell(3);
        CellClass &neighbor2=current->Adjacent_Cell(2);
        CellClass &neighbor6=current->Adjacent_Cell(6);
        unsigned int mask=Map.Unknown_0055BD70(current);
        if(mask==0x83) {
            if(Map.Unknown_0055BD70(&neighbor3)==0x83 && neighbor2.unknown_47b590()) {
                unsigned int choice;
                do {
                    choice=(unsigned int)((unsigned int)unknown_map_random_a705c0()*(2.0/4294967295.0));
                } while(choice>1);
                if(choice==0) {
                    neighbor2.unknown_11b=(char)(neighbor2.unknown_11b-4);
                    Write_Generation_Region(neighbor2.MapCoords,Read_Generation_Region(current->MapCoords));
                }
            }
        } else if(mask==0x38) {
            if(Map.Unknown_0055BD70(&neighbor3)==0x38 && neighbor6.unknown_47b590()) {
                unsigned int choice;
                do {
                    choice=(unsigned int)((unsigned int)unknown_map_random_a705c0()*(2.0/4294967295.0));
                } while(choice>1);
                if(choice==0) {
                    neighbor6.unknown_11b=(char)(neighbor6.unknown_11b-4);
                    Write_Generation_Region(neighbor6.MapCoords,Read_Generation_Region(current->MapCoords));
                }
            }
        } else if(mask==0xE0) {
            if(Map.Unknown_0055BD70(&neighbor5)==0xE0 && Map.Unknown_0055BD70(&neighbor1)==0xE0 && current->unknown_47b590()) {
                current->unknown_11b=(char)(current->unknown_11b+4);
                Write_Generation_Region(current->MapCoords,Read_Generation_Region(neighbor6.MapCoords));
            }
        }
        current=Map.Iterate();
    }
}


// Actual ECX is read at+3C: MapSeed prefix/reference/constness and free ownership are hypotheses.
extern DynamicVectorClass<MapRegionClass *> unknown_map_regions_a6fd28;
// Existing exact global/type/name; no definition or initializer.
// Physical ECX CellClass pointer, no stack argument, full EAX constructed region or explicit-zero path.
// Free/member spelling and name are hypotheses; no outside-range body is promoted.
MapRegionClass *__fastcall Build_Map_Region_0056e100(CellClass *cell);
void __fastcall Rebuild_Region_Grid_00583450(MapSeedClass const &options)
{
    if(unknown_map_cells_a70a24!=0) {
        int remaining=(int)((unsigned int)unknown_map_cell_side_84e804*(unsigned int)unknown_map_cell_side_84e804);
        for(int index=0;remaining>0;--remaining,++index) {
            MapRegionClass::Get_Cell_Data(index).RegionIndex=-1;
            MapRegionClass::Get_Cell_Data(index).unknown_3c=-1;
        }
    }
    for(int reverse=(int)((unsigned int)unknown_map_regions_a6fd28.Count()-1u);reverse>=0;--reverse) {
        MapRegionClass *region=unknown_map_regions_a6fd28[reverse];
        delete region;
    }
    {
        int remaining=(int)((unsigned int)unknown_map_cell_side_84e804*(unsigned int)unknown_map_cell_side_84e804);
        unknown_map_region_next_index_a70a28=0;
        for(int index=0;remaining>0;--remaining,++index) {
            Cell position=MapRegionClass::Get_Cell_Data(index).Position;
            CellClass &cell=Map[position];
            if(MapRegionClass::Get_Cell_Data(index).RegionIndex==-1 &&
               (MapRegionClass::Get_Cell_Data(index).Position.X!=0 || MapRegionClass::Get_Cell_Data(index).Position.Y!=0) &&
               (cell.unknown_47b7e0() || cell.unknown_47b9c0())) {
                Build_Map_Region_0056e100(&cell);
            }
        }
    }
    for(int index=0;index<unknown_map_regions_a6fd28.Count();++index) {
        MapRegionClass *region=unknown_map_regions_a6fd28[index];
        if(region->unknown_14!=0) {
            int special;
            if(options.MapType==3 || options.MapType==4)special=1;
            else special=0;
            region->Grow_Border_00570040(4+(region->CellCount>8000)+!(unsigned char)special);
            region->Expand_Cells_005702b0();
        }
    }
    {
        int remaining=(int)((unsigned int)unknown_map_cell_side_84e804*(unsigned int)unknown_map_cell_side_84e804);
        for(int index=0;remaining>0;--remaining,++index) {
            if(MapRegionClass::Get_Cell_Data(index).RegionIndex==-1 &&
               (MapRegionClass::Get_Cell_Data(index).Position.X!=0 || MapRegionClass::Get_Cell_Data(index).Position.Y!=0)) {
                MapRegionClass *region=Build_Map_Region_0056e100(&Map[MapRegionClass::Get_Cell_Data(index).Position]);
                if(region!=0)region->unknown_1a=false;
            }
        }
    }
}


// ECX rectangle, EDX region, RET8; first stack DWORD unused, second is required count.
// Bool, const-reference and unused-parameter spelling remain source hypotheses.
bool __fastcall Generation_Cell_In_Bounds_0058da10(Cell const &cell);
bool __fastcall Rectangle_Has_Region_Cells_00583630(Rect const &area,int region,int unknown,int required)
{
    Cell origin(area.X,area.Y);
    int count=0;
    for(int y=0;y<area.Height;++y) {
        for(int x=0;x<area.Width;++x) {
            if(Generation_Cell_In_Bounds_0058da10(origin+Cell(x,y))) {
                int index=UnknownMapGenerationCell::Get_Region(origin+Cell(x,y));
                if(index==region && ++count>=required)return true;
            }
        }
    }
    return false;
}


bool MapGeneratorClass::Expand_All_High_Ground()
{
    bool success=true;
    Map.Unknown_00498F10(0);
    Map.Reset_Iterator();
    CellClass *cell=Map.Iterate();
    while (cell!=0) {
        if (!success) break;
        success=Map.Expand_High_Ground(cell,-1);
        cell=Map.Iterate();
    }
    return success;
}


void __fastcall Set_Grid_Visit_Byte_0056db10(Cell const &position);
bool __fastcall Generation_Cell_In_Bounds_0058da10(Cell const &position);
namespace seed_0056dba0 {
int __fastcall Get_Generation_Cell_0056dba0(short *position)
{
    unsigned index=(unsigned)(int)position[1]*(unsigned)unknown_map_cell_side_84e804+(unsigned)(int)position[0];
    return (int)((unsigned)unknown_map_cells_a70a24+index*sizeof(UnknownMapGenerationCell));
}
}
bool MapGeneratorClass::Init_Start_Points()
{
    int player=0;
    if(SeedData.NumPlayers>0) {
        do {
            Cell seed=unknown_id_source_a3d290->Get_Waypoint_Location(player);
            UnknownVeinNode *nodes=new UnknownVeinNode[800];
            UnknownVeinHeap *heap=new UnknownVeinHeap(800);
            Map.Reset_Iterator();
            for(CellClass *cell=Map.Iterate();cell;cell=Map.Iterate()) {
                Cell position=cell->MapCoords;
                reinterpret_cast<UnknownMapGenerationCell *>(seed_0056dba0::Get_Generation_Cell_0056dba0((short *)&position))->unknown_3c=0;
            }
            int visited=0;
            int patch=++player;
            heap->Clear();
            int nodeCount=1;
            nodes[0].Position=seed;
            nodes[0].Key=0.0f;
            reinterpret_cast<UnknownMapGenerationCell *>(seed_0056dba0::Get_Generation_Cell_0056dba0((short *)&seed))->unknown_3c=patch;
            heap->Insert(&nodes[0]);
            UnknownVeinNode *node=heap->Remove_Min();
            while(node) {
                if(visited>=400)break;
                Set_Grid_Visit_Byte_0056db10(node->Position);
                int facing=0;
                UnknownVeinNode *next=&nodes[nodeCount];
                for(;facing<8;++facing) {
                    Cell adjacent=CellClass::Adjacent_Cell(node->Position,facing);
                    if(Generation_Cell_In_Bounds_0058da10(adjacent) && reinterpret_cast<UnknownMapGenerationCell *>(seed_0056dba0::Get_Generation_Cell_0056dba0((short *)&adjacent))->unknown_3c==0) {
                        if(Map[adjacent].unknown_47b590()) {
                            next->Position=adjacent;
                            int dx=(int)adjacent.X-(int)seed.X;
                            int dy=(int)adjacent.Y-(int)seed.Y;
                            int squared=(int)((unsigned)dy*(unsigned)dy+(unsigned)dx*(unsigned)dx);
                            next->Key=Fast_Sqrt((double)squared);
                            reinterpret_cast<UnknownMapGenerationCell *>(seed_0056dba0::Get_Generation_Cell_0056dba0((short *)&adjacent))->unknown_3c=patch;
                            ++nodeCount;
                            heap->Insert(next++);
                        }
                    }
                }
                ++visited;
                node=heap->Remove_Min();
            }
            delete[] nodes;
            delete heap;
        } while(player<SeedData.NumPlayers);
    }
    return true;
}


// Scatters resource fields over each region's resource cells, then gives every starting position in the
// region a field of its own (TibSun Create_Tiberium; RA2 groups both by map region). The one-line helpers
// add free /Ob2 sites: VectorClass<double>'s constructor stays out of line as in the original.
extern DynamicVectorClass<MapRegionClass *> unknown_map_regions_a6fd28;
inline float Cell_Distance(Cell const &cell,int x,int y)
{
    int dx=cell.X-x;
    int dy=cell.Y-y;
    return Fast_Sqrt((double)(dy*dy+dx*dx));
}

double __stdcall Map_Clamped_Gaussian_005798f0(double mean,double deviation,double minimum,double maximum);
inline int Start_Count(MapRegionClass const *region)
{
    return region->unknown_20;
}

inline bool Rich_Starts(MapSeedClass const &seed)
{
    return seed.Resources==3 && (seed.MapType==1 || seed.MapType==3 || seed.MapType==4);
}

void MapGeneratorClass::Create_Tiberium()
{
    int waypoint=0;
    bool seek_center;
    int resources=SeedData.Resources;
    switch(SeedData.MapType) {
        case 0:
            seek_center=resources==3;
            break;
        case 1:
        case 3:
        case 4:
            seek_center=resources!=3;
            break;
        case 2:
            seek_center=true;
            break;
        default:
            seek_center=false;
            break;
    }

    for(int index=0;index<unknown_map_regions_a6fd28.Count();index++) {
        MapRegionClass *region=unknown_map_regions_a6fd28[index];
        DynamicVectorClass<Cell> *&layout=*(DynamicVectorClass<Cell> **)&region->unknown_00;
        if(layout==0) {
            continue;
        }
        int nearest=-1;
        if(seek_center) {
            Cell center(0,0);
            if(region->unknown_20>0) {
                for(int player=0;player<region->unknown_20;player++) {
                    Cell start=unknown_id_source_a3d290->Get_Waypoint_Location(player+waypoint);
                    center.Y+=start.Y;
                    center.X+=start.X;
                }
                center.X/=region->unknown_20;
                center.Y/=region->unknown_20;
            } else {
                center=region->Cells[Map_Random_Range_005797a0(0,region->Cells.Count()-1)];
            }
            int best=500000;
            for(int cell=0;cell<layout->Count();cell++) {
                Cell position=(*layout)[cell];
                float distance=Cell_Distance(position,center.X,center.Y);
                if(distance<(float)best) {
                    best=(int)distance;
                    nearest=cell;
                }
            }
        }
        double percent=SeedData.Tiberium*0.01;
        int total=(int)(percent*(RMGMaximumTiberium-RMGMinimumTiberium)+RMGMinimumTiberium);
        double players=region->unknown_20;
        double share=0.5>players?0.5:players;
        int amount=(int)((double)total*share);
        int count=layout->Count();
        if(count==0 || amount==0) {
            continue;
        }
        int per_cell=amount/count;
        for(int field=0;field<layout->Count();field++) {
            int size=(int)((double)per_cell+Map_Clamped_Gaussian_005798f0(0.0,50.0,-100.0,100.0));
            if(size>=0) {
                Create_Tiberium_Patch((*layout)[field],size,field+waypoint+1,field==nearest);
            }
        }

        DynamicVectorClass<double> distances;
        for(int player=0;player<Start_Count(region);player++) {
            double total_distance=0.0;
            Cell start=unknown_id_source_a3d290->Get_Waypoint_Location(player+waypoint);
            for(int cell=0;cell<layout->Count();cell++) {
                Cell position=(*layout)[cell];
                total_distance+=Cell_Distance(position,start.X,start.Y);
            }
            distances.Add(total_distance/(double)layout->Count());
        }

        double closest=9999999.0;
        for(int i=0;i<Start_Count(region);i++) {
            if(distances[i]<closest) {
                closest=distances[i];
            }
        }

        bool rich=Rich_Starts(SeedData);
        for(int spot=0;spot<Start_Count(region);spot++) {
            int size=(int)((distances[spot]-closest)*15.0);
            Create_Tiberium_Patch(unknown_id_source_a3d290->Get_Waypoint_Location(spot+waypoint),size+500,spot+waypoint+1,rich);
        }
        waypoint+=Start_Count(region);
    }
}


extern "C" int __cdecl sprintf(char *,char const *,...);
extern "C" char const unknown_resource_tree_format_7e1b48[];
unsigned __fastcall Map_Random_Range_005797a0(unsigned,unsigned);
double __stdcall Randomized_Cell_Distance_005846b0(Cell const &,Cell const &);
// Original resets marker to zero while current can still alias nodes[0].
// Read current->Position anew for every neighbor; do not cache that coordinate.
void MapGeneratorClass::Create_Tiberium_Patch(Cell const &position,int amount,int patchId,bool special)
{
    int marker=0;
    int capacity=(int)((unsigned)amount*10u);
    UnknownVeinNode *nodes=new UnknownVeinNode[capacity];
    UnknownVeinHeap *queue=new UnknownVeinHeap(capacity);
    bool resetForSpread=false;
    Cell spreadOrigin=position;
    int placed=0;
    int attempts=0;
    int overlayBase=special?27:102;
    queue->Clear();
    UnknownVeinNode *current=0;
    while(placed<amount) {
        if(attempts>=10)break;
        if(current==0) {
            queue->Clear();
            Map.Reset_Iterator();
            for(CellClass *cell=Map.Iterate();cell;cell=Map.Iterate()) {
                Cell cellPosition=cell->MapCoords;
                unknown_map_cells_a70a24[cellPosition.Y*unknown_map_cell_side_84e804+cellPosition.X].unknown_3c=0;
            }
            marker=1;
            float &seedScore=nodes[0].Key;
            nodes[0].Position=position;
            seedScore=0.0f;
            unknown_map_cells_a70a24[position.Y*unknown_map_cell_side_84e804+position.X].unknown_3c=patchId;
            queue->Insert(nodes);
            current=queue->Remove_Min();
            resetForSpread=false;
            ++attempts;
            spreadOrigin=position;
        }
        CellClass &cell=Map[current->Position];
        if(unknown_map_cells_a70a24[current->Position.Y*unknown_map_cell_side_84e804+current->Position.X].unknown_45[0]==0) {
            if(!resetForSpread) {
                resetForSpread=true;
                spreadOrigin=current->Position;
                queue->Clear();
                marker=0;
                if(!special) {
                    char treeName[20];
                    int variant=(int)Map_Random_Range_005797a0(1,3);
                    sprintf(treeName,unknown_resource_tree_format_7e1b48,variant%10);
                    new TerrainClass(TerrainTypes[TerrainTypeClass::From_Name(treeName)],spreadOrigin);
                }
            }
            if(cell.OverlayIndex==-1) {
                cell.OverlayIndex=Map_Random_Range_005797a0(0,11)+overlayBase;
                ++placed;
            } else if(cell.OverlayData<11) {
                ++cell.OverlayData;
                ++placed;
            }
        }
        UnknownVeinNode *next=&nodes[marker];
        for(int facing=0;facing<8;++facing) {
            Cell adjacent=current->Position + unknown_adjacent_851a70[facing&7];
            if(Map.Unknown_0055A660(adjacent,true)) {
                CellClass &neighbor=Map[adjacent];
                if(neighbor.unknown_47b590() &&
                   ((neighbor.OverlayIndex==-1 && unknown_map_cells_a70a24[adjacent.Y*unknown_map_cell_side_84e804+adjacent.X].unknown_3c!=patchId) ||
                    (neighbor.OverlayData<11 && neighbor.Tiberium_Type_Here()!=-1))) {
                    next->Position=adjacent;
                    next->Key=(float)Randomized_Cell_Distance_005846b0(spreadOrigin,adjacent);
                    unknown_map_cells_a70a24[adjacent.Y*unknown_map_cell_side_84e804+adjacent.X].unknown_3c=patchId;
                    ++marker;
                    queue->Insert(next++);
                }
            }
        }
        current=queue->Remove_Min();
    }
    delete[] nodes;
    delete queue;
}


extern double const unknown_random_scale_7a5dd0;
extern double const unknown_distance_scale_007a5cf8;
double __stdcall Randomized_Cell_Distance_005846b0(Cell const &a,Cell const &b)
{
    int dx=(int)a.X-(int)b.X;
    int dy=(int)a.Y-(int)b.Y;
    int square=(int)((unsigned)dy*(unsigned)dy+(unsigned)dx*(unsigned)dx);
    double distance=unknown_sqrt_4ba410((double)square);
    return distance+(unsigned int)unknown_map_random_a705c0()*unknown_random_scale_7a5dd0*unknown_distance_scale_007a5cf8;
}


// TibSun Seed_Hill_Heights: a random walk of height and volatility over the cell grid, trending with the upper neighbours.
double __stdcall Map_Clamped_Gaussian_005798f0(double,double,double,double);
inline bool Hill_In_Radar(Cell const &cell)
{
    int x=cell.X;
    int y=cell.Y;
    return y+x>unknown_map_min_a70a1c && x-y<unknown_map_min_a70a1c && y-x<unknown_map_min_a70a1c && y+x<=unknown_map_max_a70a20;
}

void MapGeneratorClass::Seed_Hill_Heights()
{
    static double const scale=0.025;

    int i;
    int grid_size=unknown_map_cell_side_84e804*unknown_map_cell_side_84e804;
    double hills=SeedData.Ruggedness;
    double height_step=hills*0.0001+0.1;
    double min_volatility=hills*0.0025;
    double max_volatility=hills*0.005;

    if(min_volatility>=scale) {
        for(i=0;i<grid_size;i++) {
            UnknownMapGenerationCell &data=MapRegionClass::Get_Cell_Data(i);
            Cell cell=data.Position;
            if(cell.X!=0||cell.Y!=0) {
                data.unknown_10=0;
                double up_height=0.0;
                double height_trend=0.0;

                if(Hill_In_Radar(cell-Cell(1,1))) {
                    up_height=MapRegionClass::Get_Cell_Data(cell-Cell(1,1)).unknown_08;
                }

                Cell newcell=cell-Cell(1,0);
                if(Hill_In_Radar(newcell)) {
                    UnknownMapGenerationCell &data2=MapRegionClass::Get_Cell_Data(newcell);
                    data.unknown_08+=data2.unknown_08;
                    data.unknown_10=data2.unknown_10;
                    height_trend+=data2.unknown_08-up_height;
                } else {
                    double const &height=data.unknown_08;
                    data.unknown_08=height;
                    data.unknown_10=min_volatility;
                }

                newcell=cell-Cell(0,1);
                if(Hill_In_Radar(newcell)) {
                    UnknownMapGenerationCell &data2=MapRegionClass::Get_Cell_Data(newcell);
                    data.unknown_08=data2.unknown_08+data.unknown_08;
                    data.unknown_10=data2.unknown_10+data.unknown_10;
                    height_trend+=data2.unknown_08-up_height;
                } else {
                    double const &height=data.unknown_08;
                    data.unknown_08=height;
                    data.unknown_10+=min_volatility;
                }

                data.unknown_08*=0.5;
                data.unknown_10*=0.5;
                if(height_trend>0.0) {
                    height_trend=height_step;
                } else if(height_trend<0.0) {
                    height_trend=-height_step;
                }
                if(data.unknown_45[0]) {
                    data.unknown_08=data.unknown_08>0.0?data.unknown_08:0.0;
                    data.unknown_10=0.0025;
                }
                double const mean=0.0;
                data.unknown_10+=Map_Clamped_Gaussian_005798f0(mean,scale,-data.unknown_10,max_volatility-data.unknown_10);

                double min_height=-2.0-data.unknown_08;
                double max_height=2.0-data.unknown_08;
                double trend;
                if(height_trend<min_height) {
                    trend=min_height;
                } else if(height_trend>max_height) {
                    trend=max_height;
                } else {
                    trend=height_trend;
                }
                data.unknown_08+=Map_Clamped_Gaussian_005798f0(trend,data.unknown_10,min_height,max_height);
            }
        }

        for(i=0;i<grid_size;i++) {
            UnknownMapGenerationCell &data=MapRegionClass::Get_Cell_Data(i);
            if(data.unknown_08<0) {
                data.unknown_08=(int)(data.unknown_08-0.5);
            } else if(data.unknown_08>0) {
                data.unknown_08=(int)(data.unknown_08+0.5);
            }
        }
    }
}


// Fresh unowned operation; entryECX overwritten. Source ownership/name remain hypotheses.
// Physical ECX CellClasspointer/AL0or1/plainRET; nominal free/member/bool intent are hypotheses.
bool __fastcall Unknown_Cell_Predicate_0047b5e0(CellClass *cell);
bool __fastcall Generation_Cell_In_Bounds_0058da10(Cell const &position);
void Mark_Adjacent_Grid_Weights_00584bd0()
{
    Map.Reset_Iterator();
    CellClass *current=Map.Iterate();
    while(current!=0) {
        if(current->unknown_47b7c0()) {
            for(int direction=0;direction<8;++direction) {
                Cell position=current->MapCoords;
                Cell next=unknown_adjacent_851a70[direction&7]+position;
                if(Generation_Cell_In_Bounds_0058da10(next) && Map[next].unknown_47b590()) {
                    int at=(int)((unsigned int)(int)next.Y*(unsigned int)unknown_map_cell_side_84e804+(unsigned int)(int)next.X);
                    unknown_map_cells_a70a24[at].unknown_08=0.5;
                    at=(int)((unsigned int)(int)next.Y*(unsigned int)unknown_map_cell_side_84e804+(unsigned int)(int)next.X);
                    unknown_map_cells_a70a24[at].unknown_45[0]=1;
                    break;
                }
            }
        } else if(Unknown_Cell_Predicate_0047b5e0(current)) {
            for(int direction=0;direction<8;++direction) {
                Cell position=current->MapCoords;
                Cell next=unknown_adjacent_851a70[direction&7]+position;
                if(Generation_Cell_In_Bounds_0058da10(next) && Map[next].unknown_47b590()) {
                    int at=(int)((unsigned int)(int)next.Y*(unsigned int)unknown_map_cell_side_84e804+(unsigned int)(int)next.X);
                    unknown_map_cells_a70a24[at].unknown_45[0]=1;
                    break;
                }
            }
        }
        current=Map.Iterate();
    }
}


void Mark_Adjacent_Grid_Weights_00584bd0();
// Original free deformation operations. Return/source names are qualified hypotheses.
void Init_Deform_Grid_RMG_00685600();
bool Commit_Deform_Grid_RMG_006863e0();
int __fastcall Get_Deformed_Cell_Height_00686c90(CellClass *cell);
int __fastcall Get_Deformable_Cell_Corners_00686dd0(CellClass *cell,int direction);
bool __fastcall Deform_Cell_RMG_006869f0(Cell position,int direction,bool forced,unsigned corners);
void MapGeneratorClass::Generate_Hills()
{
    Mark_Adjacent_Grid_Weights_00584bd0();
    Seed_Hill_Heights();
    Init_Deform_Grid_RMG_00685600();
    int size=unknown_map_cell_side_84e804*unknown_map_cell_side_84e804;
    for(int index=0;index<size;++index) {
        UnknownMapGenerationCell &data=unknown_map_cells_a70a24[index];
        if(data.Position.X!=0 || data.Position.Y!=0) {
            CellClass *cell=&Map[data.Position];
            int change=(int)(cell->unknown_11b+data.unknown_08-Get_Deformed_Cell_Height_00686c90(cell));
            int steps=abs(change);
            int direction=change<0?-1:1;
            while(steps>0) {
                Deform_Cell_RMG_006869f0(data.Position,direction,0,Get_Deformable_Cell_Corners_00686dd0(cell,direction));
                --steps;
            }
        }
    }
    Commit_Deform_Grid_RMG_006863e0();
    Map.Reset_Iterator();
    CellClass *current=Map.Iterate();
    Cell const offsets[]={Cell(0,0),Cell(1,0),Cell(1,1),Cell(0,1)};
    int ramps[]={11,12,9,10};
    while(current!=0) {
        int heightStep=0;
        bool raise=true;
        if(current->unknown_11c==5) {
            for(int direction=0;direction<4;++direction) {
                if(Map[current->Fetch_CellID() + offsets[direction]].unknown_11c!=5+direction)raise=false;
            }
        } else if(current->unknown_11c==11) {
            ++heightStep;
            for(int direction=0;direction<4;++direction) {
                if(Map[current->Fetch_CellID() + offsets[direction]].unknown_11c!=ramps[direction])raise=false;
            }
        } else raise=false;
        if(raise) {
            for(int direction=0;direction<4;++direction) {
                CellClass *cell=&Map[current->Fetch_CellID() + offsets[direction]];
                cell->IsoTileIndex=0xffff;
                cell->unknown_11a=0;
                cell->unknown_11c=0;
                cell->unknown_11b+=heightStep;
            }
        }
        current=Map.Iterate();
    }
}


extern double const unknown_grid_weight_7a5d10,unknown_grid_scale_7a5d18,unknown_grid_weight_7a5d20;
bool __fastcall Generation_Cell_In_Bounds_0058da10(Cell const &position);
static inline UnknownMapGenerationCell &Vegetation_Grid_Record(Cell const &position)
{
    unsigned at=(unsigned)(int)position.Y*(unsigned)unknown_map_cell_side_84e804+(unsigned)(int)position.X;
    return unknown_map_cells_a70a24[at];
}
// ECX owner prefix, plain RET; operation/nominal receiver spelling remain hypotheses.
void __fastcall Initialize_Vegetation_Weights_005850a0(MapGeneratorClass *generator)
{
    Map.Reset_Iterator();
    CellClass *current=Map.Iterate();
    double density=generator->SeedData.Vegetation*0.01;
    while(current) {
        Cell position=current->MapCoords;
        UnknownMapGenerationCell &grid=Vegetation_Grid_Record(position);
        if(current->unknown_47b7c0()) {
            for(int x=Cell(current->MapCoords).X-2;x<Cell(current->MapCoords).X+3;++x) {
                for(int y=Cell(current->MapCoords).Y-2;y<Cell(current->MapCoords).Y+3;++y) {
                    Cell nearby; nearby.X=(short)x; nearby.Y=(short)y;
                    if(Generation_Cell_In_Bounds_0058da10(nearby)) {
                        grid.unknown_28=unknown_grid_weight_7a5d10;
                        double weighted=unknown_grid_scale_7a5d18*density;
                        grid.unknown_20=0.0;
                        grid.unknown_18=weighted*10.0;
                    }
                }
            }
        } else if(grid.unknown_28==0.0) {
            grid.unknown_28=unknown_grid_weight_7a5d10;
            grid.unknown_18=unknown_grid_scale_7a5d18*density;
            grid.unknown_20=unknown_grid_weight_7a5d20;
        }
        current=Map.Iterate();
    }
}


// Operation/free-cdecl spelling remain hypotheses; original entry is explicitly unreferenced.
extern double const unknown_map_default_007a5d28;
extern double const unknown_map_default_007a5d30;
void Initialize_Grid_Thresholds_00585240()
{
    Map.Reset_Iterator();
    for(CellClass *cell=Map.Iterate();cell;cell=Map.Iterate()) {
        Cell position=cell->MapCoords;
        UnknownMapGenerationCell &target=unknown_map_cells_a70a24[position.Y*unknown_map_cell_side_84e804+position.X];
        target.unknown_28=unknown_map_default_007a5d28;
        target.unknown_20=unknown_map_default_007a5d30;
    }
}


// TibSun Generate_Vegetation: per-map random patch means, ground cover by each cell's chances, forests, then rocks.
// Overlay_Is_Clear and Set_Overlay are one-line accessors: free /Ob2 sites that keep the Cell ctor out of line at the rock pick, as in the original.
double __cdecl Map_Random_Fraction_00579770();
double __stdcall Map_Clamped_Gaussian_005798f0(double,double,double,double);
unsigned __fastcall Map_Random_Range_005797a0(unsigned,unsigned);
int __fastcall unknown_cells_428ac0();
extern int unknown_tileset_a6e0e8;
extern int unknown_tileset_a6cf4c;
// Same-file bodies of MapRegionClass::Pick_Random_Map_Cell 0x56D720 and Pick_Random_Clear_Map_Cell 0x56D7A0,
// accepted in their own population; this unit expands them.
inline bool Overlay_Is_Clear(CellClass const *cell)
{
    return cell->OverlayIndex==-1;
}

inline void Set_Overlay(CellClass *cell,int overlay)
{
    cell->OverlayIndex=overlay;
}

inline Cell Pick_Random_Map_Cell()
{
    int gridSize=unknown_map_cell_side_84e804*unknown_map_cell_side_84e804;
    unsigned index;
    do { index=Map_Random_Range_005797a0(0,gridSize-1); }
    while((unsigned char)unknown_map_cells_a70a24[index].Position.Equal_004fef20(Cell(0,0)));
    return unknown_map_cells_a70a24[index].Position;
}

inline Cell Pick_Random_Clear_Map_Cell()
{
    int tries=0;
    for(;;) {
        Cell position=Pick_Random_Map_Cell();
        if(++tries>200)return Cell(0,0);
        if(Map[position].unknown_47b590())return position;
    }
}

void MapGeneratorClass::Generate_Vegetation()
{
    Map.Reset_Iterator();
    CellClass *cell=Map.Iterate();
    while(cell) {
        MapRegionClass::Get_Cell_Data(cell->Fetch_CellID()).unknown_3c=0;
        cell=Map.Iterate();
    }

    Map.Reset_Iterator();
    cell=Map.Iterate();

    int green_mean=Map_Random_Range_005797a0(20,40);
    int sand_mean=Map_Random_Range_005797a0(20,40);
    int rough_mean=Map_Random_Range_005797a0(20,40);

    while(cell) {
        UnknownMapGenerationCell &data=MapRegionClass::Get_Cell_Data(cell->Fetch_CellID());
        if(cell->unknown_47b590() && cell->unknown_11c==0 && cell->OverlayIndex==-1 && cell->OccupierPtr==0 && data.unknown_45[0]==0) {
            Cell origin=cell->Fetch_CellID();
            int originID=(int)((unsigned)(int)origin.Y*512u+(unsigned)(int)origin.X);
            if(Map_Random_Fraction_00579770()<data.unknown_18) {
                Place_Tile_Patch(cell,unknown_tileset_a6e0e8,(int)Map_Clamped_Gaussian_005798f0(green_mean,20.0,4.0,80.0),originID,false);
            } else if(Map_Random_Fraction_00579770()<data.unknown_28) {
                Place_Tile_Patch(cell,unknown_tileset_a6cf4c,(int)Map_Clamped_Gaussian_005798f0(sand_mean,20.0,4.0,80.0),originID,false);
            } else if(Map_Random_Fraction_00579770()<data.unknown_20) {
                Place_Tile_Patch(cell,unknown_tileset_a52c60,(int)Map_Clamped_Gaussian_005798f0(rough_mean,20.0,4.0,80.0),originID,false);
            }
        }
        cell=Map.Iterate();
    }

    Map.Reset_Iterator();
    cell=Map.Iterate();
    while(cell) {
        cell->Fixup_LAT();
        cell=Map.Iterate();
    }

    int remaining=(int)((SeedData.Width*0.1+0.7)*(SeedData.Vegetation*0.01)*MaxTrees);
    int tries;
    for(tries=0;remaining>0 && tries<100;++tries) {
        Cell position=Pick_Random_Clear_Map_Cell();
        remaining-=Place_Forest(&Map[position],(int)Map_Clamped_Gaussian_005798f0(25.0,10.0,10.0,35.0),Map_Clamped_Gaussian_005798f0(0.2,0.1,0.05,0.4));
    }

    int rock_count=Map_Random_Range_005797a0(0,unknown_cells_428ac0()/200);
    int placed=0;
    for(tries=0;tries<rock_count*5 && placed<rock_count;tries++) {
        Cell position=Pick_Random_Map_Cell();
        CellClass *target=&Map[position];
        if(Overlay_Is_Clear(target)) {
            if(target->unknown_47b9a0()) {
                Set_Overlay(target,Map_Random_Range_005797a0(0,4)+0xa8);
            } else if(target->unknown_47b590() || target->unknown_47b9c0()) {
                Set_Overlay(target,Map_Random_Range_005797a0(0,4)+0xad);
            }
            target->OverlayData=0;
            target->Recalc_Attributes(-1);
            placed++;
        }
    }
}


namespace seed_0056dba0 { int __fastcall Get_Generation_Cell_0056dba0(short *position); }
bool __fastcall Generation_Cell_In_Bounds_0058da10(Cell const &position);
double __stdcall Randomized_Cell_Distance_005862c0(Cell const &,Cell const &);
extern char const unknown_forest_tree_format_7e1b54[];
// Random map callback: map the generator's complete unsigned word to [0,1].
double __cdecl Map_Random_Fraction_00579770();

int MapGeneratorClass::Place_Forest(CellClass *origin,int count,double density)
{
    int capacity=(int)((unsigned)count*25u);
    UnknownVeinNode *nodes=new UnknownVeinNode[capacity];
    UnknownVeinHeap *heap=new UnknownVeinHeap(capacity);
    heap->Clear();
    int used=1;
    float &seedScore=nodes[0].Key;
    nodes[0].Position=Cell(origin->MapCoords);
    seedScore=0.0f;
    Cell position=Cell(origin->MapCoords);
    (*reinterpret_cast<UnknownMapGenerationCell *>(seed_0056dba0::Get_Generation_Cell_0056dba0((short *)&position))).unknown_45[2]=1;
    heap->Insert(nodes);
    UnknownVeinNode *current=heap->Remove_Min();
    int planted=0;
    while(planted<count && current && used<capacity) {
        CellClass *cell=&Map[current->Position];
        if(cell->unknown_47b590() && cell->OccupierPtr==0 && cell->OverlayIndex==-1 && cell->unknown_ec!=3) {
            if(Map_Random_Fraction_00579770()<density) {
                char treeName[20];
                int tree=(int)Map_Random_Range_005797a0(1,25);
                sprintf(treeName,unknown_forest_tree_format_7e1b54,tree/10,tree%10);
                new TerrainClass(TerrainTypes[TerrainTypeClass::From_Name(treeName)],Cell(cell->MapCoords));
                ++planted;
            }
        }
        int facing=0;
        UnknownVeinNode *nextNode=nodes+used;
        for(;facing<8;++facing) {
            Cell next=current->Position + AdjacentCell[facing & 7];
            if(Generation_Cell_In_Bounds_0058da10(next)) {
                UnknownMapGenerationCell &data=(*reinterpret_cast<UnknownMapGenerationCell *>(seed_0056dba0::Get_Generation_Cell_0056dba0((short *)&next)));
                if(!data.unknown_45[2] && !data.unknown_45[0] && used<capacity) {
                    nextNode->Position=next;
                    Cell originPosition=Cell(origin->MapCoords);
                    nextNode->Key=(float)Randomized_Cell_Distance_005862c0(originPosition,next);
                    data.unknown_45[2]=1;
                    ++used;
                    heap->Insert(nextNode++);
                }
            }
        }
        current=heap->Remove_Min();
    }
    delete[] nodes;
    delete heap;
    return planted;
}


extern double const unknown_random_scale_7a5dd0;
extern double const unknown_distance_scale_007a5d38;
double __stdcall Randomized_Cell_Distance_005862c0(Cell const &a,Cell const &b)
{
    int dx=(int)a.X-(int)b.X;
    int dy=(int)a.Y-(int)b.Y;
    int square=(int)((unsigned)dy*(unsigned)dy+(unsigned)dx*(unsigned)dx);
    double distance=unknown_sqrt_4ba410((double)square);
    return distance+Map_Random_Fraction_00579770()*unknown_distance_scale_007a5d38;
}


bool __fastcall Generation_Cell_In_Bounds_0058da10(Cell const &);
double __stdcall Randomized_Cell_Distance_00586780(Cell const &a,Cell const &b);

// Grows a patch of one tile type out from a cell over clear ground (TibSun
// MapGeneratorClass::Place_Tile_Patch): a priority-queue flood fill scored by
// Get_Tile_Patch_Score 0x00586780, claiming at most count cells.
void MapGeneratorClass::Place_Tile_Patch(CellClass *cell,int tile,int count,int origin,bool flag)
{
    UnknownVeinNode *nodes=new UnknownVeinNode[10*count];
    UnknownVeinHeap *queue=new UnknownVeinHeap(10*count);
    int i=0;
    queue->Clear();

    int node_count=1;
    Cell position=cell->Fetch_CellID();
    float &first_key=nodes[0].Key;
    nodes[0].Position=position;
    first_key=0;

    MapRegionClass::Get_Cell_Data(cell->Fetch_CellID()).unknown_3c=origin;

    queue->Insert(nodes);

    UnknownVeinNode *node=queue->Remove_Min();
    while(i<count && node!=0) {
        CellClass *cptr=&Map[node->Position];
        cptr->IsoTileIndex=tile;

        int dir=0;
        UnknownVeinNode *newnode=&nodes[node_count];
        for(;dir<8;dir++) {
            Cell newcell=CellClass::Adjacent_Cell(node->Position,dir);
            if(Generation_Cell_In_Bounds_0058da10(newcell)) {
                CellClass *newcellptr=&Map[newcell];
                if(newcellptr->unknown_47b590() || flag && (newcellptr->unknown_47b880() || newcellptr->unknown_47b860())) {
                    if(MapRegionClass::Get_Cell_Data(newcell).unknown_3c!=origin && newcellptr->unknown_11c==0 && newcellptr->OverlayIndex==-1 && newcellptr->OccupierPtr==0) {
                        newnode->Position=newcell;
                        Cell start=cell->Fetch_CellID();
                        float score=Randomized_Cell_Distance_00586780(start,newcell);
                        newnode->Key=score;
                        MapRegionClass::Get_Cell_Data(newcell).unknown_3c=origin;
                        node_count++;
                        queue->Insert(newnode++);
                    }
                }
            }
        }

        i++;
        node=queue->Remove_Min();
    }

    delete[] nodes;
    delete queue;
}


extern double const unknown_random_scale_7a5dd0;
extern double const unknown_distance_scale_007a5d40;
double __stdcall Randomized_Cell_Distance_00586780(Cell const &a,Cell const &b)
{
    int dx=(int)a.X-(int)b.X;
    int dy=(int)a.Y-(int)b.Y;
    int square=(int)((unsigned)dy*(unsigned)dy+(unsigned)dx*(unsigned)dx);
    double distance=unknown_sqrt_4ba410((double)square);
    return distance+(unsigned int)unknown_map_random_a705c0()*unknown_random_scale_7a5dd0*unknown_distance_scale_007a5d40;
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// From ra2-decomp 0x005A5360 MapGeneratorClass::Create_Urban_Area (src/mapgen.cpp, YR twin).
// Grows a paved urban area of up to size cells out from a cell (a priority-queue flood fill scored by the urban
// score), rejects it when it is too small or too sparse, trims its edge cells, marks the rest inviolate and
// scatters misc pavement tiles over it.
Rect __fastcall Bounding_Rectangle_00588e20(DynamicVectorClass<Cell> const &cells);
double __stdcall Randomized_Chebyshev_Distance_005872c0(Cell const &a,Cell const &b);

DynamicVectorClass<Cell> *MapGeneratorClass::Create_Urban_Area(Cell const &cell, int size)
{
    DynamicVectorClass<Cell> *cells;
    UnknownVeinNode *nodes = new UnknownVeinNode[10 * size];
    UnknownVeinHeap *queue = new UnknownVeinHeap(10 * size);
    int processed = 0;
    queue->Clear();

    Map.Reset_Iterator();
    CellClass *cptr = Map.Iterate();
    while (cptr != 0) {
        Cell c = cptr->MapCoords;
        Set_Cell_Data_Spread(c, 0);
        cptr = Map.Iterate();
    }

    int node_count = 0;
    nodes[node_count].Position = cell;
    nodes[node_count].Key = 0;
    Set_Cell_Data_Spread(cell, 1);
    node_count++;
    queue->Insert(&nodes[0]);

    UnknownVeinNode *node = queue->Remove_Min();

    cells = new DynamicVectorClass<Cell>;
    cells->Set_Growth_Step(size + 2);

    while (processed < size && node != 0) {
        Map[node->Position].IsoTileIndex = unknown_tileset_a6e0e0;
        cells->Add(node->Position);

        UnknownVeinNode *newnode = &nodes[node_count];
        for (int dir = FACING_N; dir < FACING_COUNT; dir++) {
            Cell newcell = Region_Adjacent_Cell(node->Position, dir);
            if (Hill_In_Radar(newcell)) {
                CellClass *newcellptr = &Map[newcell];
                if (newcellptr->unknown_47b590() && newcellptr->OverlayIndex == -1) {
                    if (Get_Cell_Data_Spread(newcell) != 1 && !MapRegionClass::Get_Cell_Data(newcell).unknown_45[0]) {
                        newnode->Position = newcell;
                        newnode->Key = Randomized_Chebyshev_Distance_005872c0(cell, newcell);
                        Set_Cell_Data_Spread(newcell, 1);
                        node_count++;
                        queue->Insert(newnode++);
                    }
                }
            }
        }

        processed++;
        node = queue->Remove_Min();
    }

    delete[] nodes;
    delete queue;

    Rect bounds = Bounding_Rectangle_00588e20(*cells);

    if (processed <= 100 || (double)(bounds.Width * bounds.Height) >= (double)cells->Count() * 1.6) {
        for (int j = cells->Count() - 1; j >= 0; j--) {
            Map[(*cells)[j]].IsoTileIndex = 0xFFFF;
        }
        delete cells;
        return 0;
    }

    for (int cell_index = cells->Count() - 1; cell_index >= 0; cell_index--) {
        Cell c = (*cells)[cell_index];
        for (int dir = FACING_N; dir < FACING_COUNT; dir++) {
            Cell newcell = Region_Adjacent_Cell(c, dir);
            if (Hill_In_Radar(newcell) && Map[newcell].IsoTileIndex != unknown_tileset_a6e0e0) {
                cells->Delete(cell_index);
            }
        }
    }

    for (int k = 0; k < cells->Count(); k++) {
        MapRegionClass::Get_Cell_Data((*cells)[k]).unknown_45[0] = 1;
    }

    int patch_count = cells->Count() / 3;
    for (int i = 0; i < patch_count; i++) {
        Cell base = (*cells)[Pick_Random_UInt(0, cells->Count() - 1)];
        int tile_variant = Pick_Random_UInt(0, 6);

        bool place = true;
        if (tile_variant > 3) {
            for (int m = 0; m < 2; m++) {
                for (int n = 0; n < 2; n++) {
                    Cell c = base + Cell(m, n);
                    if (!MapRegionClass::Get_Cell_Data(c).unknown_45[0] || Map[c].IsoTileIndex != unknown_tileset_a6e0e0) {
                        place = false;
                    }
                }
            }
        }

        if (place) {
            Place_Tile_Footprint_005883f0(unknown_tileset_a52ef4 + tile_variant, base, -1, -1);
        }
    }

    return cells;
}


extern double unknown_distance_scale_7a5d50;
// The random term scales by 1/UINT_MAX as a literal (pooled __real at 0x007A5DD0), as ra2-decomp's
// RMG_RANDOM_DOUBLE writes it in MapGeneratorClass::Get_Urban_Score (0x005A5AE0, YR twin); a folded literal,
// not a memory operand, is what lets Create_Urban_Area 0x00586B40 inline this body exactly.
double __stdcall Randomized_Chebyshev_Distance_005872c0(Cell const &a,Cell const &b)
{
    int x = (int)a.X - (int)b.X;
    int y = (int)a.Y - (int)b.Y;
    double distance=abs(x)>abs(y)?abs(x):abs(y);
    return distance + (unsigned int)unknown_map_random_a705c0()*(1.0 / 4294967295.0)*unknown_distance_scale_7a5d50;
}


// TibSun Generate_Urban_Roads: east-west bands, then north-south bands with medians.
Rect __fastcall Bounding_Rectangle_00588e20(DynamicVectorClass<Cell> const &cells);
bool __fastcall Can_Place_Paved_Road_00588a30(Rect const &area,bool overlap,bool end);
bool __fastcall Can_Place_Paved_Road_Junction_00588ec0(Rect &area);
void MapGeneratorClass::Generate_Urban_Roads(DynamicVectorClass<Cell> &cells)
{
    Cell start;
    Rect bounds=Bounding_Rectangle_00588e20(cells);

    if(bounds.Height>5) {
        start.Y=bounds.Y+2;
        start.X=bounds.X;
        int remaining=bounds.Height-2;

        while(remaining>3) {
            Cell junction=Plan_Paved_Road_Junction(start,bounds,2);
            Cell end=Plan_Paved_Road(junction,bounds,2);

            bool valid=false;
            int first;

            if(end!=Cell(0,0) && start!=Cell(0,0)) {
                first=start.X;
                valid=true;
            }

            if(valid && end.X-first>=6) {
                if(Can_Place_Paved_Road_00588a30(Rect(junction.X-4,junction.Y,4,3),false,false) && Map_Random_Range_005797a0(0,1)) {
                    Place_Tile_Footprint_005883f0(unknown_tileset_a6dd0c+9,junction-Cell(3,0));
                } else {
                    Place_Tile_Footprint_005883f0(unknown_tileset_a6dd08+2,junction);
                }

                for(int x=0;x<end.X-junction.X-1;x++) {
                    Place_Tile_Footprint_005883f0(unknown_tileset_a6dd0c,junction+Cell(x+1,0));
                }

                if(Can_Place_Paved_Road_00588a30(Rect(end.X,end.Y,4,3),false,false) && Map_Random_Range_005797a0(0,1)) {
                    Place_Tile_Footprint_005883f0(unknown_tileset_a6dd0c+10,end);
                } else {
                    Place_Tile_Footprint_005883f0(unknown_tileset_a6dd08,end);
                }

                remaining-=10;
                start.Y+=10;
            } else {
                start.Y+=2;
                remaining-=2;
            }
        }
    }

    if(bounds.Width>8) {
        int remaining=bounds.Height-5;
        start.X=bounds.X+5;
        start.Y=bounds.Y;

        while(remaining>3) {
            Cell cursor=Plan_Paved_Road_Junction(start,bounds,4);
            Cell end=Plan_Paved_Road(cursor,bounds,4);

            if(end!=Cell(0,0) && start!=Cell(0,0) && end.Y-start.Y>=6) {
                bool in_segment=false;
                bool initial=true;
                bool median_pending=false;
                bool median_flip=false;
                bool skip_tail=false;

                while(cursor.Y<end.Y) {
                    if(!in_segment) {
                        Rect road_rect;
                        Rect junction_rect;
                        road_rect=Rect(cursor.X,cursor.Y,3,2);
                        junction_rect=Rect(cursor.X,cursor.Y-3,3,4);

                        if(end.Y-cursor.Y<2) {
                            skip_tail=true;
                            break;
                        }

                        if(initial && Can_Place_Paved_Road_00588a30(Rect(cursor.X,cursor.Y-3,3,4),false,false) && Map_Random_Range_005797a0(0,1)) {
                            Place_Tile_Footprint_005883f0(unknown_tileset_a6dd0c+13,cursor-Cell(0,3));
                            in_segment=true;
                        } else if(Can_Place_Paved_Road_Junction_00588ec0(junction_rect)) {
                            Place_Tile_Footprint_005883f0(unknown_tileset_a6dd0c+5,cursor-Cell(0,3));
                            Place_Tile_Footprint_005883f0(unknown_tileset_a6dd0c+1,cursor);
                            in_segment=true;
                        } else if(Can_Place_Paved_Road_00588a30(road_rect,false,false)) {
                            Place_Tile_Footprint_005883f0(unknown_tileset_a6dd08+1,cursor);
                            in_segment=true;
                        }
                    } else {
                        if(Can_Place_Paved_Road_00588a30(Rect(cursor.X,cursor.Y,3,1),false,false)) {
                            Place_Tile_Footprint_005883f0(unknown_tileset_a6dd0c+1,cursor);

                            if(median_pending) {
                                if(median_flip) {
                                    Place_Tile_Footprint_005883f0(unknown_tileset_a52c68+5,cursor+Cell(1,0));
                                    median_flip=false;
                                } else {
                                    Place_Tile_Footprint_005883f0(unknown_tileset_a52c68+4,cursor+Cell(1,0));
                                }
                            }
                        } else {
                            Rect junction_rect(cursor.X,cursor.Y,3,4);

                            if(median_pending) {
                                Place_Tile_Footprint_005883f0(unknown_tileset_a52c68+3,cursor+Cell(1,-1));
                                median_pending=false;
                            }

                            if(Can_Place_Paved_Road_Junction_00588ec0(junction_rect)) {
                                int bottom=end.Y;

                                if(bottom-cursor.Y<4) {
                                    Place_Tile_Footprint_005883f0(unknown_tileset_a6dd0c+7,cursor);
                                    skip_tail=true;
                                    break;
                                }

                                if(Can_Place_Paved_Road_00588a30(Rect(cursor.X,cursor.Y+3,3,2),false,false)) {
                                    if(bottom-cursor.Y>5) {
                                        median_pending=true;
                                        median_flip=true;
                                    }
                                }

                                Place_Tile_Footprint_005883f0(unknown_tileset_a6dd0c+3,cursor);

                                if(Can_Place_Paved_Road_00588a30(Rect(cursor.X-1,cursor.Y,1,3),true,false)) {
                                    Place_Tile_Footprint_005883f0(unknown_tileset_a52c68+12,cursor-Cell(1,0));
                                }

                                if(Can_Place_Paved_Road_00588a30(Rect(cursor.X+3,cursor.Y,1,3),true,false)) {
                                    Place_Tile_Footprint_005883f0(unknown_tileset_a52c68+12,cursor+Cell(3,0));
                                }

                                cursor.Y+=2;
                            } else {
                                Place_Tile_Footprint_005883f0(unknown_tileset_a6dd08+3,cursor-Cell(0,1));

                                CellClass *cellptr=&Map[cursor+Cell(1,-2)];

                                if(cellptr->unknown_47b940()) {
                                    while(cellptr->unknown_47b940()) {
                                        cellptr->IsoTileIndex=unknown_tileset_a6dd0c+1;
                                        cellptr->unknown_11a=1;
                                        cellptr=&cellptr->Adjacent_Cell(0);
                                    }
                                }

                                in_segment=false;
                            }
                        }
                    }

                    initial=false;
                    cursor.Y++;
                }

                if(!skip_tail) {
                    if(median_pending) {
                        Place_Tile_Footprint_005883f0(unknown_tileset_a52c68+3,cursor+Cell(1,-1));
                    }

                    if(Can_Place_Paved_Road_00588a30(Rect(cursor.X,cursor.Y,3,4),false,false) && Map_Random_Range_005797a0(0,1)) {
                        Place_Tile_Footprint_005883f0(unknown_tileset_a6dd0c+12,cursor);
                    } else {
                        Place_Tile_Footprint_005883f0(unknown_tileset_a6dd08+3,cursor);
                    }
                }

                remaining-=10;
                start.X+=10;
            } else {
                start.X+=2;
                remaining-=2;
            }
        }
    }
}


extern char *unknown_urban_building_names_7e0be8[18];
unsigned __fastcall Map_Random_Range_005797a0(unsigned,unsigned);
void MapGeneratorClass::Generate_Urban_Buildings(DynamicVectorClass<Cell> &cells)
{
    int count=Map_Random_Range_005797a0(cells.Count()/20,cells.Count()/10)/2;
    int tries=0;
    int placed=0;
    DynamicVectorClass<char *> names;
    for(int index=0;index<18;++index)names.Add(unknown_urban_building_names_7e0be8[index]);
    HouseClass *neutral=HouseClass::As_Pointer(HouseTypeClass::Find_Index("Neutral"));
    if(count>0) {
        while(true) {
            if(tries>=2*count || names.Count()<=1)break;
            int nameIndex=Map_Random_Range_005797a0(0,names.Count()-1);
            int typeIndex=BuildingTypeClass::From_Name(names[nameIndex]);
            {
                BuildingClass *building=new BuildingClass(BuildingTypes[typeIndex],neutral);
                int attempts=0;
                bool wasPlaced=false;
                while(!wasPlaced) {
                    if(attempts>=60)break;
                    Cell position=cells[Map_Random_Range_005797a0(0,cells.Count()-1)];
                    Cell const *list=building->Type->Occupy_List(1);
                    bool clear=true;
                    while(list->X!=0x7fff || list->Y!=0x7fff) {
                        Cell next=(position + *list);
                        CellClass *cell=&Map[next];
                        if((!cell->unknown_47b880() && !cell->unknown_47b860()) || cell->OccupierPtr!=0 || cell->unknown_ec==3 || !Map.Unknown_0055A740(cell,true))clear=false;
                        ++list;
                        if(!clear)break;
                    }
                    if(clear) {
                        CoordStruct point(position.X*256+128,position.Y*256+128,0);
                        if(building->Unlimbo(&point,0)) {
                            int strength=building->Type->MaxStrength;
                            building->Strength=strength;
                            building->unknown_70=strength;
                            building->Mark(2);
                            wasPlaced=true;
                            ++placed;
                            if(nameIndex>17)names.Delete((int)nameIndex);
                        }
                    }
                    ++attempts;
                }
                if(!wasPlaced)delete building;
                ++tries;
            }
            if(placed>=count)break;
        }
    }
}


extern char const *unknown_urban_unit_names_7e0c30[];
unsigned __fastcall Map_Random_Range_005797a0(unsigned,unsigned);
void MapGeneratorClass::Generate_Urban_Units(DynamicVectorClass<Cell> &cells)
{
    HouseClass *neutral=HouseClass::As_Pointer(HouseTypeClass::Find_Index("Neutral"));
    int count=Map_Random_Range_005797a0(1,5);
    int tries=0;
    int placed=0;
    if(count>0) {
    while(true) {
        if(tries>=2*count)break;
        int nameIndex=Map_Random_Range_005797a0(0,6);
        FootClass *unit;
        if(nameIndex<=2)unit=new InfantryClass(InfantryTypes[InfantryTypeClass::From_Name(unknown_urban_unit_names_7e0c30[nameIndex])],neutral);
        else unit=new UnitClass(UnitTypes[UnitTypeClass::From_Name(unknown_urban_unit_names_7e0c30[nameIndex])],neutral);
        int attempts=0;
        bool wasPlaced=false;
        while(!wasPlaced) {
            if(attempts>=60)break;
            Cell position=cells[Map_Random_Range_005797a0(0,cells.Count()-1)];
            if(Map.Unknown_0055A660(position,true) && Map[position].OccupierPtr==0) {
                if(unit->Unlimbo(&CoordStruct(position.X*256+128,position.Y*256+128,0),Map_Random_Range_005797a0(0,7)*32)) {
                    int strength=unit->Techno_Type_Class()->MaxStrength;
                    unit->Strength=strength;
                    unit->unknown_70=strength;
                    unit->Mark(2);
                    wasPlaced=true;
                    ++placed;
                }
            }
            ++attempts;
        }
        if(!wasPlaced)delete unit;
        ++tries;
        if(placed>=count)break;
    }
    }
}


// Packed coordinate ECX and AL-only result; freefastcall/constreference/bool/name are source hypotheses.
bool __fastcall Unknown_Cell_Predicate_0047b5e0(CellClass *cell);
bool __fastcall Neighborhood_Is_Clear_00588380(Cell const &center)
{
    int centerY=center.Y;
    int top=centerY-7;
    int centerX=center.X;
    int right=centerX+7;
    int bottom=centerY+7;
    for(int x=centerX-7;x<right;++x) {
        for(int y=top;y<bottom;++y) {
            Cell position((short)x,(short)y);
            if(Unknown_Cell_Predicate_0047b5e0(&Map[position]))return false;
        }
    }
    return true;
}


// ECX tile index, EDX Cell address, RET8 region/height. Names and nominal types are hypotheses.
void __fastcall Place_Tile_Footprint_005883f0(int tile,Cell const &position,int region,int height)
{
    IsometricTileTypeClass *type=IsoTileTypes[tile];
    IsoTileImageStruct const *image=(IsoTileImageStruct const *)type->Get_Image_Data();
    if(image) {
        for(int y=0;y<type->unknown_2b8;y++) {
            for(int x=0;x<type->unknown_2b4;x++) {
                Cell where=position+Cell(x,y);
                if((int)where.Y+(int)where.X>unknown_map_min_a70a1c &&
                   (int)where.X-(int)where.Y<unknown_map_min_a70a1c &&
                   (int)where.Y-(int)where.X<unknown_map_min_a70a1c &&
                   (int)where.Y+(int)where.X<=unknown_map_max_a70a20) {
                    CellClass &cell=Map[where];
                    int index=(int)((unsigned)type->unknown_2b4*(unsigned)y+(unsigned)x);
                    IsoTileCellStruct const *part=(IsoTileCellStruct const *)image->Tiles[index];
                    if(part) {
                        int tileIndex=type->ArrayIndex;
                        cell.unknown_11a=(char)index;
                        cell.IsoTileIndex=tileIndex;
                        if(height!=-1)cell.unknown_11b=(char)(part->unknown_28+height-4);
                        cell.unknown_11c=part->unknown_2a;
                        if(unknown_map_cells_a70a24) {
                            unsigned at=(unsigned)(int)where.Y*(unsigned)unknown_map_cell_side_84e804+(unsigned)(int)where.X;
                            unknown_map_cells_a70a24[at].RegionIndex=region;
                        }
                    }
                }
            }
        }
    }
}


// TibSun Plan_Paved_Road_Junction: first paved tile along dir, then the first spot a junction fits, walked back a random distance. Member: ECX is the generator.
extern int unknown_tileset_a6e0e0;
bool __fastcall Can_Place_Paved_Road_00588a30(Rect const &area,bool overlap,bool end);
unsigned __fastcall Map_Random_Range_005797a0(unsigned,unsigned);
inline Cell Adjacent_Road_Cell(Cell const &cell,int facing)
{
    return cell+AdjacentCell[facing&7];
}
Cell MapGeneratorClass::Plan_Paved_Road_Junction(Cell const &cell,Rect const &rect,int dir)
{
    Cell wcell=cell;
    Rect const &area=rect;
    int margin_x=0;
    int margin_y=0;
    int margin_width=0;
    int margin_height=0;
    Rect tile_size(0,0,0,0);
    bool south=dir==4;
    if(dir==2) {
        tile_size=Rect(0,0,1,3);
        margin_y=-3;
        margin_height=6;
    } else {
        tile_size=Rect(0,0,3,1);
        margin_x=-3;
        margin_width=6;
    }
    while(true) {
        if(!(wcell.X<area.X+area.Width && wcell.Y<area.Y+area.Height)) {
            return Cell(0,0);
        }
        if(Map[wcell].IsoTileIndex==(unsigned)unknown_tileset_a6e0e0) {
            break;
        }
        wcell=Adjacent_Road_Cell(wcell,dir);
    }
    while(true) {
        if(!(wcell.X<area.X+area.Width && wcell.Y<area.Y+area.Height)) {
            return Cell(0,0);
        }
        if(Can_Place_Paved_Road_00588a30(Rect(margin_x+wcell.X,margin_y+wcell.Y,margin_width+tile_size.Width,margin_height+tile_size.Height),south,south)) {
            break;
        }
        wcell=Adjacent_Road_Cell(wcell,dir);
    }
    int length=wcell.Y-cell.Y>wcell.X-cell.X?wcell.Y-cell.Y:wcell.X-cell.X;
    if(length>0) {
        int shorten=Map_Random_Range_005797a0(0,length);
        while(shorten>0) {
            Cell back=Adjacent_Road_Cell(wcell,dir-4);
            if(!Can_Place_Paved_Road_00588a30(Rect(margin_x+back.X,margin_y+back.Y,margin_width+tile_size.Width,margin_height+tile_size.Height),false,false)) {
                break;
            }
            wcell=back;
            shorten--;
        }
    }
    return wcell;
}


// TibSun Plan_Paved_Road: how far a road can run along dir inside rect. Member: ECX is the generator.
bool __fastcall Can_Place_Paved_Road_00588a30(Rect const &area,bool overlap,bool end);
unsigned __fastcall Map_Random_Range_005797a0(unsigned,unsigned);
Cell MapGeneratorClass::Plan_Paved_Road(Cell const &cell,Rect const &rect,int dir)
{
    bool south=dir==4;
    Cell wcell=cell;
    int margin_x=0;
    int margin_y=0;
    int margin_width=0;
    int margin_height=0;
    Rect tile_size(0,0,0,0);
    if(dir==2) {
        tile_size=Rect(0,0,1,3);
        margin_y=-3;
        margin_height=6;
    } else {
        tile_size=Rect(0,0,3,1);
        margin_x=-3;
        margin_width=6;
    }
    while(true) {
        Rect const &area=rect;
        if(!(wcell.X<area.X+area.Width+1 && wcell.Y<area.Y+area.Height+1)) {
            break;
        }
        if(!Can_Place_Paved_Road_00588a30(Rect(margin_x+wcell.X,margin_y+wcell.Y,margin_width+tile_size.Width,margin_height+tile_size.Height),south,south)) {
            break;
        }
        wcell=Adjacent_Road_Cell(wcell,dir);
    }
    wcell=Adjacent_Road_Cell(wcell,dir-4);
    int length=wcell.X-cell.X>wcell.Y-cell.Y?wcell.X-cell.X:wcell.Y-cell.Y;
    if(length>6) {
        int shorten=Map_Random_Range_005797a0(0,3);
        while(shorten>0) {
            Cell back=Adjacent_Road_Cell(wcell,dir-4);
            shorten--;
            wcell=back;
        }
    }
    return wcell;
}


bool __fastcall Generation_Cell_In_Bounds_0058da10(Cell const &cell);
// ECX rectangle, DL first flag, stack second flag; AL predicate/RET4.
bool __fastcall Can_Place_Paved_Road_00588a30(Rect const &area,bool allowFirst,bool allowSecond)
{
    if(Generation_Cell_In_Bounds_0058da10(XY_Cell(area.X,area.Y)) &&
       Generation_Cell_In_Bounds_0058da10(XY_Cell(area.X+area.Width-1,area.Y)) &&
       Generation_Cell_In_Bounds_0058da10(XY_Cell(area.X,area.Y+area.Height-1)) &&
       Generation_Cell_In_Bounds_0058da10(XY_Cell(area.X+area.Width-1,area.Y+area.Height-1))) {
        for(int y=area.Y;y<area.Y+area.Height;++y) {
            for(int x=area.X;x<area.X+area.Width;++x) {
                CellClass &cell=Map[Cell(x,y)];
                if(cell.unknown_47b8e0()) {
                    if(!allowFirst)return false;
                } else if(cell.unknown_47b900()) {
                    if(!allowSecond)return false;
                } else if(!cell.unknown_47b590() && !cell.unknown_47b860() && !cell.unknown_47b880()) {
                    return false;
                }
            }
        }
        return true;
    }
    return false;
}


bool __fastcall Generation_Cell_In_Bounds_0058da10(Cell const &cell);
// ECX rectangle, DL overlap flag; AL predicate, plain RET.
// The signed byte at cell+0x11b is the elevation hypothesis.
bool __fastcall Can_Place_Paved_Road_End_00588c20(Rect const &area,bool allowRoad)
{
    if(Generation_Cell_In_Bounds_0058da10(XY_Cell(area.X,area.Y)) &&
       Generation_Cell_In_Bounds_0058da10(XY_Cell(area.X+area.Width-1,area.Y)) &&
       Generation_Cell_In_Bounds_0058da10(XY_Cell(area.X,area.Y+area.Height-1)) &&
       Generation_Cell_In_Bounds_0058da10(XY_Cell(area.X+area.Width-1,area.Y+area.Height-1))) {
        int originX=area.X;
        int originY=area.Y;
        int height=Map[Cell(originX,originY)].unknown_11b;
        for(int y=area.Y;y<area.Y+area.Height;++y) {
            for(int x=area.X;x<area.X+area.Width;++x) {
                CellClass &cell=Map[Cell(x,y)];
                if(cell.unknown_11b!=height)return false;
                if(cell.unknown_47b8e0() || cell.unknown_47b900()) {
                    if(!allowRoad)return false;
                } else if(!cell.unknown_47b590() && !cell.unknown_47b860() && !cell.unknown_47b880()) {
                    return false;
                }
            }
        }
        return true;
    }
    return false;
}


// Inclusive cell-vector bounds; free fastcall/hidden-result spelling is a hypothesis.
Rect __fastcall Bounding_Rectangle_00588e20(DynamicVectorClass<Cell> const &cells)
{
    Cell minimum(999,999),maximum(0,0);
    for(int i=cells.Count()-1;i>=0;i--) {
        Cell point=cells[i];
        if(point.X<minimum.X)minimum.X=point.X;
        if(point.Y<minimum.Y)minimum.Y=point.Y;
        if(point.X>maximum.X)maximum.X=point.X;
        if(point.Y>maximum.Y)maximum.Y=point.Y;
    }
    return Rect(minimum.X,minimum.Y,(int)maximum.X-minimum.X+1,(int)maximum.Y-minimum.Y+1);
}


bool __fastcall Generation_Cell_In_Bounds_0058da10(Cell const &cell);
bool __fastcall Can_Place_Paved_Road_Junction_00588ec0(Rect &area)
{
    if(Generation_Cell_In_Bounds_0058da10(XY_Cell(area.X,area.Y)) &&
       Generation_Cell_In_Bounds_0058da10(XY_Cell(area.X+area.Width-1,area.Y)) &&
       Generation_Cell_In_Bounds_0058da10(XY_Cell(area.X,area.Y+area.Height-1)) &&
       Generation_Cell_In_Bounds_0058da10(XY_Cell(area.X+area.Width-1,area.Y+area.Height-1))) {
        for(int y=area.Y;y<area.Y+3;++y) {
            for(int x=area.X;x<area.X+3;++x) {
                CellClass &cell=Map[Cell(x,y)];
                if((cell.IsoTileIndex<unknown_tileset_a6dd0c || cell.IsoTileIndex>unknown_tileset_a6dd0c+1) && !cell.unknown_47b940()) return false;
            }
        }
        for(int x=area.X;x<area.X+3;++x) {
            int y=area.Y+3;
            CellClass &cell=Map[Cell(x,y)];
            if(!cell.unknown_47b880() && !cell.unknown_47b590() && !cell.unknown_47b860()) return false;
        }
        return true;
    }
    return false;
}


unsigned __fastcall Map_Random_Range_005797a0(unsigned minimum,unsigned maximum);
bool __fastcall Can_Place_Misc_Pavement_00589320(Rect const &area);
void __fastcall Place_Tile_Footprint_005883f0(int tile,Cell const &position,int region,int height);
void MapGeneratorClass::Generate_Urban_Pavement(DynamicVectorClass<Cell> &cells)
{
    Rect sizes[5]={Rect(0,0,6,4),Rect(0,0,4,6),Rect(0,0,4,4),Rect(0,0,4,2),Rect(0,0,2,4)};
    int count=cells.Count()/50;
    int placed=0;
    for(int tries=0;tries<10*count && placed<count;++tries) {
        unsigned index;
        {
        unsigned maximum=(unsigned)cells.Count()-1;
        double span=(unsigned)(maximum+1);
        do { index=(unsigned)((unsigned)unknown_map_random_a705c0()*span*(1.0/4294967295.0)); } while(index>maximum);
        }
        Cell cell=cells[index];
        int size=0;
        bool fits=false;
        Rect area(cell.X,cell.Y,0,0);
        while(!fits) {
            if(size>=5)break;
            area.Width=sizes[size].Width;
            area.Height=sizes[size].Height;
            if(Can_Place_Misc_Pavement_00589320(area))fits=true;
            ++size;
        }
        if(fits) {
            unsigned variant;
            do { variant=(unsigned)((unsigned)unknown_map_random_a705c0()*(6.0/4294967295.0)); } while(variant>5);
            int tile=variant+unknown_tileset_a52ef4+8;
            for(int y=0;y<area.Height/2;++y) {
                for(int x=0;x<area.Width/2;++x) {
                    Place_Tile_Footprint_005883f0(tile,Cell(area.X+2*x,area.Y+2*y),-1,-1);
                }
            }
            ++placed;
        }
    }
}


// Decorative pavement operation/name hypothesis; physical ECX rectangle, AL result, plain RET.
bool __fastcall Generation_Cell_In_Bounds_0058da10(Cell const &cell);
bool __fastcall Can_Place_Misc_Pavement_00589320(Rect const &area)
{
    if(Generation_Cell_In_Bounds_0058da10(XY_Cell(area.X,area.Y)) &&
       Generation_Cell_In_Bounds_0058da10(XY_Cell(area.X+area.Width-1,area.Y)) &&
       Generation_Cell_In_Bounds_0058da10(XY_Cell(area.X,area.Y+area.Height-1)) &&
       Generation_Cell_In_Bounds_0058da10(XY_Cell(area.X+area.Width-1,area.Y+area.Height-1))) {
        for(int y=area.Y;y<area.Y+area.Height;++y) {
            for(int x=area.X;x<area.X+area.Width;++x) {
                CellClass &cell=Map[Cell(x,y)];
                int tile=cell.IsoTileIndex;
                if(!cell.unknown_47b880() && (tile<unknown_tileset_a52ef4 || tile>unknown_tileset_a52ef4+7))return false;
                if(cell.OccupierPtr)return false;
            }
        }
        return true;
    }
    return false;
}


// Operation/freefastcall/constreference/flag signedness/bool source intent are hypotheses.
bool __fastcall Can_Use_Tile_Footprint_005894e0(int tile,Cell const &position,unsigned char check_mark,unsigned char reject_outside)
{
    IsometricTileTypeClass *type=IsoTileTypes[tile];
    IsoTileImageStruct const *image=(IsoTileImageStruct const *)type->Get_Image_Data();
    if(!image)return true;
    for(int y=0;y<type->unknown_2b8;y++) {
        for(int x=0;x<type->unknown_2b4;x++) {
            Cell where=position+Cell(x,y);
            if((int)where.Y+(int)where.X>unknown_map_min_a70a1c &&
               (int)where.X-(int)where.Y<unknown_map_min_a70a1c &&
               (int)where.Y-(int)where.X<unknown_map_min_a70a1c &&
               (int)where.Y+(int)where.X<=unknown_map_max_a70a20) {
                CellClass &cell=Map[where];
                unsigned char const *subtile=(unsigned char const *)image->Tiles[(int)((unsigned)type->unknown_2b4*(unsigned)y+(unsigned)x)];
                if(subtile) {
                    if(!cell.unknown_47b590() && !cell.unknown_47b5b0())return false;
                    if((int)cell.unknown_11c!=(int)(signed char)subtile[0x2a])return false;
                    if(check_mark && unknown_map_cells_a70a24[(int)((unsigned)(int)where.Y*(unsigned)unknown_map_cell_side_84e804+(unsigned)(int)where.X)].unknown_45[1])return false;
                }
            } else if(reject_outside)return false;
        }
    }
    return true;
}


// Unchanged real existing original helper; natural standalone copy receives zero credit.
extern int unknown_map_min_a70a1c,unknown_map_max_a70a20;
bool __fastcall Generation_Cell_In_Bounds_0058da10(Cell const &cell);

// Name/free fastcall/void intent are hypotheses, original ECX index/EDX Cell address.
void __fastcall Erase_Tile_Footprint_00589670(int tile,Cell const &position)
{
    IsometricTileTypeClass *type=IsoTileTypes[tile];
    IsoTileImageStruct const *image=(IsoTileImageStruct const *)type->Get_Image_Data();
    if(image) {
        for(int y=0;y<type->unknown_2b8;y++) {
            for(int x=0;x<type->unknown_2b4;x++) {
                Cell where=position+Cell(x,y);
                if((int)where.Y+(int)where.X>unknown_map_min_a70a1c &&
                   (int)where.X-(int)where.Y<unknown_map_min_a70a1c &&
                   (int)where.Y-(int)where.X<unknown_map_min_a70a1c &&
                   (int)where.Y+(int)where.X<=unknown_map_max_a70a20) {
                    CellClass &cell=Map[where];
                    if(image->Tiles[(int)((unsigned)type->unknown_2b4*(unsigned)y+(unsigned)x)]!=0) {
                        cell.IsoTileIndex=0xffff;
                        cell.unknown_11a=0;
                    }
                }
            }
        }
    }
}


bool __fastcall Generation_Cell_In_Bounds_0058da10(Cell const &position);
// Original field semantics unknown: byte46 is existing grid unknown_45[1], not a new layout.
void __fastcall Set_Tile_Footprint_Grid_Byte_00589780(int tile,Cell const &position,unsigned char value)
{
    IsometricTileTypeClass *type=IsoTileTypes[tile];
    IsoTileImageStruct const *image=(IsoTileImageStruct const *)type->Get_Image_Data();
    if(image) {
        for(int y=0;y<type->unknown_2b8;y++) {
            for(int x=0;x<type->unknown_2b4;x++) {
                Cell where;
                where.X=(short)((unsigned)(int)position.X+(unsigned)x);
                where.Y=(short)((unsigned)(int)position.Y+(unsigned)y);
                if(Generation_Cell_In_Bounds_0058da10(where) && image->Tiles[(int)((unsigned)type->unknown_2b4*(unsigned)y+(unsigned)x)]!=0)
                    unknown_map_cells_a70a24[(int)((unsigned)(int)where.Y*(unsigned)unknown_map_cell_side_84e804+(unsigned)(int)where.X)].unknown_45[1]=value;
            }
        }
    }
}


extern char *unknown_rural_building_names_7e0c60[36];
unsigned __fastcall Map_Random_Range_005797a0(unsigned,unsigned);
void MapGeneratorClass::Generate_Rural_Buildings(Cell const &origin)
{
    int count=Map_Random_Range_005797a0(8,12);
    int tries=0;
    int placed=0;
    int height=Map[origin].unknown_11b;
    unsigned first=0;
    unsigned last=35;
    if(SeedData.Theater==0)first=15;
    DynamicVectorClass<char *> names;
    for(int index=0;index<36;++index)names.Add(unknown_rural_building_names_7e0c60[index]);
    HouseClass *neutral=HouseClass::As_Pointer(HouseTypeClass::Find_Index("Neutral"));
    if(count>0) {
        while(true) {
            if(tries>=2*count)break;
            unsigned nameIndex=Map_Random_Range_005797a0(first,last);
            int typeIndex=BuildingTypeClass::From_Name(names[nameIndex]);
            if(typeIndex!=-1) {
                BuildingClass *building=new BuildingClass(BuildingTypes[typeIndex],neutral);
                int attempts=0;
                bool wasPlaced=false;
                while(!wasPlaced) {
                    if(attempts>=60)break;
                    Cell position=(origin + Cell(Map_Random_Range_005797a0(0,14)-7,Map_Random_Range_005797a0(0,14)-7));
                    Cell const *list=building->Type->Occupy_List(1);
                    bool clear=true;
                    while(list->X!=0x7fff || list->Y!=0x7fff) {
                        Cell next=(position + *list);
                        CellClass *cell=&Map[next];
                        if(cell->OccupierPtr!=0 || !cell->unknown_47b590() || cell->unknown_11b!=height || cell->unknown_ec==3 || !Map.Unknown_0055A740(cell,true))clear=false;
                        ++list;
                        if(!clear)break;
                    }
                    if(clear) {
                        CoordStruct point(position.X*256+128,position.Y*256+128,0);
                        if(building->Unlimbo(&point,0)) {
                            wasPlaced=true;
                            ++placed;
                            names.Delete((int)nameIndex);
                            --last;
                        }
                    }
                    ++attempts;
                }
                if(!wasPlaced)delete building;
                ++tries;
            }
            if(placed>=count)break;
        }
    }
}


extern char const *unknown_rural_unit_names_7e0cf0[6];
unsigned __fastcall Map_Random_Range_005797a0(unsigned,unsigned);
void MapGeneratorClass::Generate_Rural_Units(Cell const &origin)
{
    HouseClass *neutral=HouseClass::As_Pointer(HouseTypeClass::Find_Index("Neutral"));
    int count=Map_Random_Range_005797a0(1,4);
    int tries=0;
    int placed=0;
    while(placed<count) {
        if(tries>=2*count)break;
        int nameIndex=Map_Random_Range_005797a0(0,5);
        ObjectClass *unit;
        if(nameIndex<=2)unit=new InfantryClass(InfantryTypes[InfantryTypeClass::From_Name(unknown_rural_unit_names_7e0cf0[nameIndex])],neutral);
        else unit=new UnitClass(UnitTypes[UnitTypeClass::From_Name(unknown_rural_unit_names_7e0cf0[nameIndex])],neutral);
        int attempts=0;
        bool wasPlaced=false;
        while(!wasPlaced) {
            if(attempts>=60)break;
            Cell position=origin + Cell(Map_Random_Range_005797a0(0,16)-8,Map_Random_Range_005797a0(0,16)-8);
            if(Map.Unknown_0055A660(position,true) && Map[position].OccupierPtr==0) {
                if(unit->Unlimbo(&CoordStruct(position.X*256+128,position.Y*256+128,0),Map_Random_Range_005797a0(0,7)*32)) {
                    int strength=unit->Techno_Type_Class()->MaxStrength;
                    unit->Strength=strength;
                    unit->unknown_70=strength;
                    unit->Mark(2);
                    wasPlaced=true;
                    ++placed;
                }
            }
            ++attempts;
        }
        if(!wasPlaced)delete unit;
        ++tries;
    }
}


extern int unknown_tileset_a6dd04;
extern UnknownMapGenerationPath unknown_generation_paths_a700a0[109];
unsigned int __fastcall Map_Random_Range_005797a0(unsigned int minimum,unsigned int maximum);
bool __fastcall Can_Use_Tile_Footprint_005894e0(int tile,Cell const &position,unsigned char check_mark,unsigned char reject_outside);
void __fastcall Place_Tile_Footprint_005883f0(int tile,Cell const &position,int region,int height);
void __fastcall Set_Tile_Footprint_Grid_Byte_00589780(int tile,Cell const &position,unsigned char value);
void __fastcall Erase_Tile_Footprint_00589670(int tile,Cell const &position);
// TibSun MapGeneratorClass::Find_Dirt_Road_Tile (fastcall in RA2: ECX facing, EDX cell).
bool __fastcall Find_Dirt_Road_Tile_0058a6e0(int facing,Cell const &cell,int max_connections,unsigned int &tile,Cell &origin,int &link_index);

// Same-file bodies of the road records' copy and member constructors; the out-of-line copies 0x58B140
// and 0x58B160 are accepted in their own population, this unit expands them.
inline UnknownMapGenerationPoint::UnknownMapGenerationPoint(UnknownMapGenerationPoint const &other)
    : Position(other.Position),unknown_04(other.unknown_04)
{
}
inline UnknownMapGenerationLink::UnknownMapGenerationLink(unsigned int value,Cell const &a,unsigned int next,Cell const &b,int index)
    : unknown_00(value),unknown_04(a),unknown_08(next),unknown_0c(b),unknown_10(index)
{
}

// TibSun facing.h Facing_Sub.
inline FacingType Facing_Sub(int first,int second)
{
    return (FacingType)((first-second)&7);
}

// TibSun Dirt_Road_Tile_Index: the dirt road library entry of a tile type.
inline int Dirt_Road_Tile_Index(int tile)
{
    if(tile>=unknown_tileset_a6e014 && tile<unknown_tileset_a6e014+101) {
        return tile-unknown_tileset_a6e014;
    }
    if(tile<unknown_tileset_a6dd04 || tile>=unknown_tileset_a6dd04+8) {
        return 0;
    }
    return tile-unknown_tileset_a6dd04+101;
}

// Lays a dirt road network out from a junction tile at cell (TibSun
// MapGeneratorClass::Generate_Rural_Roads): breadth-first over a 1024-entry
// queue of road pieces, each tile's links continued or capped.
// A road node field holding a tile (0xffff is the none value, TibSun ISOTILE_NONE).
inline bool Road_Tile_Set(unsigned int tile)
{
    return tile!=0xffff;
}

bool MapGeneratorClass::Generate_Rural_Roads(Cell const &cell)
{
    QueueClass<UnknownMapGenerationLink,1024> queue;

    int processed=0;

    unsigned int random_offset=Map_Random_Range_005797a0(0,10);
    int attempt=0;
    int junction;
    while(true) {
        junction=unknown_tileset_a6cb6c+(int)(random_offset+attempt)%11;
        if(Can_Use_Tile_Footprint_005894e0(junction,cell,false,false)) {
            break;
        }
        if(++attempt>=11) {
            return false;
        }
    }

    UnknownMapGenerationLink root(junction,cell,0xffff,cell,-1);
    queue.Add(root);
    while(queue.Count!=0) {
        if(queue.Count>=1020) {
            break;
        }
        bool success=true;
        UnknownMapGenerationLink node=queue.First();
        bool placed=Road_Tile_Set(node.unknown_00);

        int child_count=0;
        UnknownMapGenerationLink *children=0;

        if(placed) {
            if(Road_Tile_Set(node.unknown_08)) {
                Set_Tile_Footprint_Grid_Byte_00589780(node.unknown_08,node.unknown_0c,false);
            }
            Place_Tile_Footprint_005883f0(node.unknown_00,node.unknown_04,-1,-1);

            UnknownMapGenerationPath &tile=unknown_generation_paths_a700a0[Dirt_Road_Tile_Index(node.unknown_00)];

            if(tile.Count>1) {
                int link_count=tile.Count-(node.unknown_10!=-1);
                children=new UnknownMapGenerationLink[link_count];
                if(children!=0) {
                    UnknownMapGenerationLink *child=children;
                    for(int link_index=0;link_index<tile.Count;link_index++) {
                        if(link_index!=node.unknown_10) {
                            UnknownMapGenerationPoint link=tile.Points[link_index];
                            FacingType reverse=Facing_Sub(link.unknown_04,4);

                            Cell connection=CellClass::Adjacent_Cell(node.unknown_04+link.Position,link.unknown_04);
                            int cap_link=0;
                            if(Find_Dirt_Road_Tile_0058a6e0(reverse,connection,1,child->unknown_08,child->unknown_0c,cap_link)) {
                                if(!Find_Dirt_Road_Tile_0058a6e0(reverse,connection,4,child->unknown_00,child->unknown_04,child->unknown_10)) {
                                    child->unknown_10=cap_link;
                                } else {
                                    Set_Tile_Footprint_Grid_Byte_00589780(child->unknown_00,child->unknown_04,true);
                                }
                                Set_Tile_Footprint_Grid_Byte_00589780(child->unknown_08,child->unknown_0c,true);
                                child_count++;
                                child++;
                            } else {
                                success=false;
                                break;
                            }
                        }
                    }
                }
            }
        }

        if(placed && success) {
            if(children!=0) {
                UnknownMapGenerationLink *src=children;
                for(int j=0;j<child_count;j++) {
                    queue.Add(*src);
                    src++;
                }
            }
        } else {
            if(Road_Tile_Set(node.unknown_00)) {
                Erase_Tile_Footprint_00589670(node.unknown_00,node.unknown_04);
                UnknownMapGenerationLink *undo=children;
                while(child_count>0) {
                    if(Road_Tile_Set(undo->unknown_00)) {
                        Set_Tile_Footprint_Grid_Byte_00589780(undo->unknown_00,undo->unknown_04,false);
                    }
                    if(Road_Tile_Set(undo->unknown_08)) {
                        Set_Tile_Footprint_Grid_Byte_00589780(undo->unknown_08,undo->unknown_0c,false);
                    }
                    undo++;
                    --child_count;
                }
            }
            if(Road_Tile_Set(node.unknown_08)) {
                Place_Tile_Footprint_005883f0(node.unknown_08,node.unknown_0c,-1,-1);
            }
        }

        if(children!=0) {
            delete [] children;
        }

        queue.Next();
        processed++;
    }

    while(queue.Count!=0) {
        UnknownMapGenerationLink node=queue.First();
        Place_Tile_Footprint_005883f0(node.unknown_08,node.unknown_0c,-1,-1);
        queue.Next();
    }
    if(processed>1) {
        return true;
    }
    return false;
}


// Select a dirt-road tile whose link faces the target and whose footprint fits.
extern double const unknown_road_bias_7a5d70;

extern double const random_unit_7a5dd0;
extern int unknown_road_counts_7e1064[8][4];
extern int *unknown_road_library_7e0fe4[8][4];
extern UnknownMapGenerationPath unknown_map_paths_a700a0[109];
double __stdcall Map_Clamped_Gaussian_005798f0(double mean,double deviation,double minimum,double maximum);
unsigned int __fastcall Map_Random_Range_005797a0(unsigned int minimum,unsigned int maximum);
bool __fastcall Can_Use_Tile_Footprint_005894e0(int tile,Cell const &position,unsigned char check_mark,unsigned char reject_outside);
bool __fastcall Find_Dirt_Road_Tile_0058a6e0(int facing,Cell const &target_cell,int max_connections,unsigned int &tile,Cell &origin,int &link_index)
{
    int current=1;
    int initial=1;
    if(max_connections>1) {
        double roll=Map_Clamped_Gaussian_005798f0(2.0,0.5,2.0,max_connections);
        current=roll;
        initial=current;
    }
    while(true) {
        int tile_count=unknown_road_counts_7e1064[facing][current-1];
        int *tile_list=unknown_road_library_7e0fe4[facing][current-1];
        int start_index;
        if(current==2 && (unsigned)unknown_map_random_a705c0()*random_unit_7a5dd0<unknown_road_bias_7a5d70) {
            start_index=Map_Random_Range_005797a0(7,tile_count-1);
        } else {
            start_index=Map_Random_Range_005797a0(0,tile_count-1);
        }
        for(int i=0;i<tile_count;i++) {
            UnknownMapGenerationPath &path=unknown_map_paths_a700a0[tile_list[(i+start_index)%tile_count]];
            int index;
            for(index=0;index<path.Count;index++) {
                UnknownMapGenerationPoint connection=path.Points[index];
                if(connection.unknown_04==facing) {
                    break;
                }
            }
            UnknownMapGenerationPoint link;
            link.Position=path.Points[index].Position;
            Cell position=target_cell-link.Position;
            if(Can_Use_Tile_Footprint_005894e0(path.unknown_08,position,1,1)) {
                UnknownMapGenerationPoint link2;
                link2.Position=path.Points[index].Position;
                origin=target_cell-link2.Position;
                tile=path.unknown_08;
                link_index=index;
                return true;
            }
        }
        current=(current+1)%(max_connections+1);
        if(current==0) {
            current=1;
        }
        if(current==initial) {
            tile=0xffff;
            origin=Cell(0,0);
            link_index=-1;
            return false;
        }
    }
}


double __cdecl unknown_sin_4ba480(double);
double __cdecl unknown_cos_4ba4d0(double);
extern char const unknown_light_type_name_7e1b60[];
extern char const unknown_special_name_7cdd70[];
extern int const unknown_light_counts_7a5d7c[4];
extern double const unknown_angle_turn_79c9b8;
extern double const unknown_angle_quarter_79b630;
extern double const unknown_angle_inverse_79b628;
extern double const unknown_facing_radians_79b620;
inline void UnknownFacingDir::From_Radian(double angle)
{
    Raw=(unsigned short)(short)((angle-unknown_angle_quarter_79b630)*unknown_angle_inverse_79b628);
}
inline double UnknownFacingDir::As_Radian() const
{
    return ((int)(short)Raw-16383)*unknown_facing_radians_79b620;
}
// Genuine Move_Coord operation; reference inline.h. Actual signed16 direction and original FP observation order.
inline CoordStruct Move_Coord(CoordStruct const &start,UnknownFacingDir const &direction,int distance)
{
    double radians=direction.As_Radian();
    return CoordStruct(start.X+unknown_cos_4ba4d0(radians)*distance,start.Y-unknown_sin_4ba480(radians)*distance,start.Z);
}
double __cdecl Map_Random_Fraction_00579770()
{
    return (unsigned)unknown_map_random_a705c0()*(1.0/4294967295.0);
}
void MapGeneratorClass::Generate_Lights()
{
    int lightType=BuildingTypeClass::From_Name(unknown_light_type_name_7e1b60);
    if(lightType!=-1) {
        HouseClass *owner=HouseClass::As_Pointer(HouseTypeClass::Find_Index(unknown_special_name_7cdd70));
        if(owner!=0) {
            bool old=Debug_Map;
            Debug_Map=true;
            for(int player=0;player<SeedData.NumPlayers;++player) {
                Cell waypoint=unknown_id_source_a3d290->Get_Waypoint_Location(player);
                int tries=20;
                int lightCount=unknown_light_counts_7a5d7c[SeedData.Time];
                bool clear=false;
                int index;
                while(tries>=0 && !clear) {
                    double angle=Map_Random_Fraction_00579770()*6.283185307179586;
                    clear=true;
                    for(index=0;index<lightCount;++index) {
                        UnknownFacingDir direction;
                        direction.From_Radian(index*unknown_angle_turn_79c9b8/lightCount+angle);
                        CoordStruct position=Move_Coord(Cell_Center(waypoint),direction,8*256);
                        Cell cell(position);
                        if(!BuildingTypes[lightType]->Legal_Placement(cell,owner) || !Map.In_Radar(cell)) { clear=false;break; }
                    }
                    if(clear) {
                        for(index=0;index<lightCount;++index) {
                            UnknownFacingDir direction;
                            direction.From_Radian(index*unknown_angle_turn_79c9b8/lightCount+angle);
                            CoordStruct position=Move_Coord(Cell_Center(waypoint),direction,8*256);
                            Cell cell(position);
                            BuildingClass *light=new BuildingClass(BuildingTypes[lightType],owner);
                            if(!light->Unlimbo(&CoordStruct(cell.X*256+128,cell.Y*256+128,0),0))delete light;
                        }
                    }
                    --tries;
                }
            }
            Debug_Map=old;
        }
    }
}


unsigned __fastcall Map_Random_Range_005797a0(unsigned,unsigned);
extern DynamicVectorClass<MapRegionClass *> unknown_map_regions_a6fd28;
inline int Region_Size(MapRegionClass const *region)
{
    return region->unknown_20;
}

inline bool Cell_Is_Free(Cell const &position)
{
    return !MapRegionClass::Get_Cell_Data(position).unknown_45[0];
}

inline bool Is_List_End(Cell const *list)
{
    return list->X==0x7fff && list->Y==0x7fff;
}

inline int Cell_Height(Cell const &position)
{
    return Map[position].unknown_11b;
}

void MapGeneratorClass::Generate_Neutral_Buildings()
{
    if(SeedData.MapType==2) {
        int count=Map_Random_Range_005797a0(0,2);
        for(int pass=count;pass>0;pass--) {
            for(int index=0;index<unknown_map_regions_a6fd28.Count();index++) {
                if(Region_Size(unknown_map_regions_a6fd28[index])>0) {
                    unknown_map_regions_a6fd28[index]->Place_Neutral_Tech_Building_00576cd0();
                }
            }
        }
    } else {
        HouseClass *neutral=HouseClass::As_Pointer(HouseTypeClass::Find_Index("Neutral"));
        int count=Map_Random_Range_005797a0(0,4);
        for(int pass=count;pass>0;pass--) {
            bool placed=false;
            int tries=0;
            BuildingClass *building=new BuildingClass(Rules->NeutralTechBuildings[Map_Random_Range_005797a0(0,Rules->NeutralTechBuildings.Count()-1)],neutral);
            while(!placed) {
                if(tries>=100)break;
                Cell position=Pick_Random_Clear_Map_Cell();
                int height=Cell_Height(position);
                Cell const *list=building->Type->Occupy_List(1);
                bool clear=true;
                while(!Is_List_End(list)) {
                    Cell next=position + *list;
                    CellClass *cell=&Map[next];
                    if(cell->OccupierPtr!=0 || !cell->unknown_47b590() || cell->unknown_11b!=height || cell->unknown_ec==3 || !Map.Unknown_0055A740(cell,true) || !Cell_Is_Free(cell->MapCoords))clear=false;
                    ++list;
                    if(!clear)break;
                }
                if(clear) {
                    if(building->Unlimbo(&CoordStruct(position.X*256+128,position.Y*256+128,0),0))placed=true;
                }
                ++tries;
            }
            if(!placed)delete building;
        }
    }
}


UnknownMapGenerationPoint::UnknownMapGenerationPoint(Cell const &position,int value) : Position(position),unknown_04(value) {}


UnknownMapGenerationPoint::UnknownMapGenerationPoint() : Position(0,0),unknown_04(-1) {}


UnknownMapGenerationLink::UnknownMapGenerationLink()
    : unknown_00(0xffff),unknown_04(Cell(0,0)),unknown_08(0xffff),unknown_0c(Cell(0,0)),unknown_10(-1)
{
}


UnknownMapGenerationLink::UnknownMapGenerationLink(UnknownMapGenerationLink const &other)
    : unknown_00(other.unknown_00),unknown_04(other.unknown_04),unknown_08(other.unknown_08),unknown_0c(other.unknown_0c),unknown_10(other.unknown_10)
{
}


UnknownMapGenerationPath::UnknownMapGenerationPath(UnknownMapGenerationPoint const &a)
    : unknown_08(0)
{
    Count=1;
    Points=new UnknownMapGenerationPoint[1];
    Points[0]=a;
}


UnknownMapGenerationPath::UnknownMapGenerationPath(UnknownMapGenerationPoint const &a,UnknownMapGenerationPoint const &b)
    : unknown_08(0)
{
    Count=2;
    Points=new UnknownMapGenerationPoint[2];
    Points[0]=a;
    Points[1]=b;
}


UnknownMapGenerationPath::UnknownMapGenerationPath(UnknownMapGenerationPoint const &a,UnknownMapGenerationPoint const &b,UnknownMapGenerationPoint const &c)
    : unknown_08(0)
{
    Count=3;
    Points=new UnknownMapGenerationPoint[3];
    Points[0]=a;
    Points[1]=b;
    Points[2]=c;
}


UnknownMapGenerationPath::UnknownMapGenerationPath(UnknownMapGenerationPoint const &a,UnknownMapGenerationPoint const &b,UnknownMapGenerationPoint const &c,UnknownMapGenerationPoint const &d)
    : unknown_08(0)
{
    Count=4;
    Points=new UnknownMapGenerationPoint[4];
    Points[0]=a;
    Points[1]=b;
    Points[2]=c;
    Points[3]=d;
}


UnknownMapGenerationPath::UnknownMapGenerationPath() : Count(0),Points(0)
{
}


UnknownMapGenerationPath::~UnknownMapGenerationPath()
{
    delete[] Points;
    Points=0;
}


UnknownMapGenerationPath unknown_map_paths_a700a0[109] = {
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,0),0), UnknownMapGenerationPoint(Cell(2,0),1)), // 0
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,0),0), UnknownMapGenerationPoint(Cell(1,0),2)), // 1
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,0),0), UnknownMapGenerationPoint(Cell(1,1),3)), // 2
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,0),0), UnknownMapGenerationPoint(Cell(1,0),5)), // 3
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,0),0), UnknownMapGenerationPoint(Cell(0,0),6)), // 4
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(1,0),0), UnknownMapGenerationPoint(Cell(1,1),7)), // 5
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(1,0),1), UnknownMapGenerationPoint(Cell(1,1),2)), // 6
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(1,0),1), UnknownMapGenerationPoint(Cell(1,1),3)), // 7
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(1,0),1), UnknownMapGenerationPoint(Cell(0,1),4)), // 8
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(1,0),1), UnknownMapGenerationPoint(Cell(0,0),6)), // 9
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(2,0),1), UnknownMapGenerationPoint(Cell(1,1),7)), // 10
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(2,0),1), UnknownMapGenerationPoint(Cell(0,0),0)), // 11
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(1,0),2), UnknownMapGenerationPoint(Cell(1,2),3)), // 12
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(1,0),2), UnknownMapGenerationPoint(Cell(0,1),4)), // 13
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(1,0),2), UnknownMapGenerationPoint(Cell(1,0),5)), // 14
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(1,0),2), UnknownMapGenerationPoint(Cell(1,1),7)), // 15
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(2,1),3), UnknownMapGenerationPoint(Cell(0,1),4)), // 16
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(1,1),3), UnknownMapGenerationPoint(Cell(1,0),5)), // 17
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(1,1),3), UnknownMapGenerationPoint(Cell(0,0),6)), // 18
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(1,1),4), UnknownMapGenerationPoint(Cell(1,0),5)), // 19
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,1),4), UnknownMapGenerationPoint(Cell(0,0),6)), // 20
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,1),4), UnknownMapGenerationPoint(Cell(1,1),7)), // 21
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(1,1),5), UnknownMapGenerationPoint(Cell(1,1),7)), // 22
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,1),6), UnknownMapGenerationPoint(Cell(1,1),7)), // 23
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,0),0), UnknownMapGenerationPoint(Cell(1,0),2), UnknownMapGenerationPoint(Cell(0,1),4)), // 24
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(1,0),2), UnknownMapGenerationPoint(Cell(0,1),4), UnknownMapGenerationPoint(Cell(0,0),6)), // 25
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,0),0), UnknownMapGenerationPoint(Cell(0,1),4), UnknownMapGenerationPoint(Cell(0,0),6)), // 26
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,0),0), UnknownMapGenerationPoint(Cell(1,0),2), UnknownMapGenerationPoint(Cell(0,0),6)), // 27
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,0),0), UnknownMapGenerationPoint(Cell(1,0),2), UnknownMapGenerationPoint(Cell(0,1),4), UnknownMapGenerationPoint(Cell(0,0),6)), // 28
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(2,0),1), UnknownMapGenerationPoint(Cell(2,2),3), UnknownMapGenerationPoint(Cell(1,1),5)), // 29
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(1,1),7), UnknownMapGenerationPoint(Cell(1,1),5), UnknownMapGenerationPoint(Cell(2,2),3)), // 30
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(2,0),1), UnknownMapGenerationPoint(Cell(1,1),7), UnknownMapGenerationPoint(Cell(1,1),5)), // 31
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(1,1),7), UnknownMapGenerationPoint(Cell(2,0),1), UnknownMapGenerationPoint(Cell(2,2),3)), // 32
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(1,1),7), UnknownMapGenerationPoint(Cell(1,1),5), UnknownMapGenerationPoint(Cell(2,0),1), UnknownMapGenerationPoint(Cell(2,2),3)), // 33
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(3,0),1), UnknownMapGenerationPoint(Cell(3,2),2), UnknownMapGenerationPoint(Cell(0,2),6)), // 34
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(3,0),1), UnknownMapGenerationPoint(Cell(1,2),5)), // 35
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(3,0),1), UnknownMapGenerationPoint(Cell(1,2),5)), // 36
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(3,0),1), UnknownMapGenerationPoint(Cell(1,2),5)), // 37
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(3,0),1), UnknownMapGenerationPoint(Cell(1,2),5)), // 38
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(2,0),1), UnknownMapGenerationPoint(Cell(1,1),5)), // 39
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(1,0),1), UnknownMapGenerationPoint(Cell(1,0),5)), // 40
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(1,0),1), UnknownMapGenerationPoint(Cell(1,0),5)), // 41
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(1,2),5)), // 42
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(1,1),5)), // 43
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(1,1),5)), // 44
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(3,0),1)), // 45
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(2,0),1)), // 46
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(2,0),1)), // 47
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(4,0),1), UnknownMapGenerationPoint(Cell(1,1),5)), // 48
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(2,0),1), UnknownMapGenerationPoint(Cell(1,3),5)), // 49
    UnknownMapGenerationPath(), // 50
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,0),6), UnknownMapGenerationPoint(Cell(3,0),2)), // 51
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,0),6), UnknownMapGenerationPoint(Cell(3,0),2)), // 52
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,0),6), UnknownMapGenerationPoint(Cell(3,0),2)), // 53
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,0),6), UnknownMapGenerationPoint(Cell(3,0),2)), // 54
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,0),6), UnknownMapGenerationPoint(Cell(2,0),2)), // 55
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,0),6), UnknownMapGenerationPoint(Cell(1,0),2)), // 56
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,0),6), UnknownMapGenerationPoint(Cell(0,0),2)), // 57
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,0),6), UnknownMapGenerationPoint(Cell(0,0),2)), // 58
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,0),6)), // 59
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,0),6)), // 60
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,1),6)), // 61
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(2,0),2)), // 62
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(1,0),2)), // 63
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(1,0),2)), // 64
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,0),6), UnknownMapGenerationPoint(Cell(3,1),2)), // 65
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,1),6), UnknownMapGenerationPoint(Cell(3,0),2)), // 66
    UnknownMapGenerationPath(), // 67
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(1,1),7), UnknownMapGenerationPoint(Cell(3,3),3)), // 68
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(1,1),7), UnknownMapGenerationPoint(Cell(3,3),3)), // 69
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(1,1),7), UnknownMapGenerationPoint(Cell(3,3),3)), // 70
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(1,1),7), UnknownMapGenerationPoint(Cell(3,3),3)), // 71
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(1,1),7), UnknownMapGenerationPoint(Cell(2,2),3)), // 72
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(1,1),7), UnknownMapGenerationPoint(Cell(1,1),3)), // 73
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(1,1),7), UnknownMapGenerationPoint(Cell(1,1),3)), // 74
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(1,1),7)), // 75
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(1,1),7)), // 76
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(1,1),7)), // 77
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(3,3),3)), // 78
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(1,2),3)), // 79
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(2,2),3)), // 80
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(1,1),7), UnknownMapGenerationPoint(Cell(4,2),3)), // 81
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(1,1),7), UnknownMapGenerationPoint(Cell(2,4),3)), // 82
    UnknownMapGenerationPath(), // 83
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,0),0), UnknownMapGenerationPoint(Cell(0,3),4)), // 84
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,0),0), UnknownMapGenerationPoint(Cell(0,3),4)), // 85
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,0),0), UnknownMapGenerationPoint(Cell(0,3),4)), // 86
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,0),0), UnknownMapGenerationPoint(Cell(0,3),4)), // 87
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,0),0), UnknownMapGenerationPoint(Cell(0,2),4)), // 88
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,0),0), UnknownMapGenerationPoint(Cell(0,1),4)), // 89
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,0),0), UnknownMapGenerationPoint(Cell(0,0),4)), // 90
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,0),0), UnknownMapGenerationPoint(Cell(0,0),4)), // 91
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,3),4)), // 92
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,2),4)), // 93
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,2),4)), // 94
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,0),0)), // 95
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,0),0)), // 96
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,0),0)), // 97
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(1,0),0), UnknownMapGenerationPoint(Cell(0,3),4)), // 98
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,0),0), UnknownMapGenerationPoint(Cell(1,3),4)), // 99
    UnknownMapGenerationPath(), // 100
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,0),6), UnknownMapGenerationPoint(Cell(0,0),2)), // 101
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,0),6), UnknownMapGenerationPoint(Cell(0,0),2)), // 102
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,0),0), UnknownMapGenerationPoint(Cell(0,0),4)), // 103
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,0),0), UnknownMapGenerationPoint(Cell(0,0),4)), // 104
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,0),6), UnknownMapGenerationPoint(Cell(0,0),2)), // 105
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,0),6), UnknownMapGenerationPoint(Cell(0,0),2)), // 106
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,0),0), UnknownMapGenerationPoint(Cell(0,0),4)), // 107
    UnknownMapGenerationPath(UnknownMapGenerationPoint(Cell(0,0),0), UnknownMapGenerationPoint(Cell(0,0),4)), // 108
};


// Complete compiler-generated initializer0058B420 for the109 road-path descriptors.
// Its actual C++ global definition is in src/recovered/0058b400.cpp of this same unit.
// Original-only closed CFG2616 instructions/9537 bytes, then15 frozen NOP to0058D970.


// Original reverse destruction of109 twelve-byte records, each Points member freed then cleared.
// Complete array extent follows the loop count and field offsets; nominal global name is unknown.
void Destroy_Map_Paths_0058d970()
{
    UnknownMapGenerationPath *current=unknown_map_paths_a700a0+109;
    for(int count=109;count>0;--count) {
        (--current)->~UnknownMapGenerationPath();
    }
}


// Native VC6 scalar deleting destructor, emitted by ordinary MapRegionClass deletion.
// The complete real class destructor is supplied by Make_Ice_Regions in this same unit;
// Generate_Random_Map supplies the naturally retained deleting-destructor call.


// Native scalar deleting destructor at0058DB20.
// Emitted by ordinary UnknownVeinHeap deletion in the existing Init_Start_Points definition.
// No handwritten release operation or forced emission.


UnknownVeinNode *UnknownVeinHeap::Remove_Min()
{
        if (Count == 0) {
            return 0;
        }
        UnknownVeinNode *top = Array[1];
        Array[1] = Array[Count];
        Array[Count] = 0;
        Count--;
        Sift_Down(1);
        return top;
    }


// The complete inline definition is shared with the heap consumers.
// Init_Start_Points and Remove_Min naturally emit the original out-of-line copy.


// Not exact (RA2_NON_MATCHING): MapGeneratorClass::Grow_Water_Region_005820b0 at 0x005820B0, best score 58.71, 2026-11-13; differs: 669 instructions in the original, 646 in the draft (shape 89.58)
#ifdef RA2_NON_MATCHING
void Clear_Map_Cell_Traversal_Marks_00582a50();
extern "C" double __cdecl log(double);
#pragma intrinsic(log)
extern Random2Class unknown_map_random_a705c0;
extern double const angle_half_pi_79b630,angle_pi_79d158,angle_turn_79c9b8;
extern double const angle_weight_7a5cd0,jitter_weight_7a5cc8,random_unit_7a5dd0;
bool __fastcall Generation_Cell_In_Bounds_0058da10(Cell const &);
double __stdcall unknown_gaussian_value_005798d0(double mean,double deviation);
unsigned __fastcall Map_Random_Range_005797a0(unsigned,unsigned);

bool MapGeneratorClass::Grow_Water_Region(int region,float scale,Rect const &bounds,Cell const &origin,bool frontier)
{
    int capacity=(int)((unsigned)LocalWidth*(unsigned)LocalHeight*2u);
    UnknownVeinNode *nodes=new UnknownVeinNode[capacity>100?capacity:100];
    UnknownVeinHeap *queue=new UnknownVeinHeap((int)((unsigned)LocalWidth*(unsigned)LocalHeight*2u)>100?(int)((unsigned)LocalWidth*(unsigned)LocalHeight*2u):100);
    int used=0;
    bool success=true;
    double angle=0.0;
    if(bounds.X!=0)angle=0.0;
    else if(bounds.Width!=512)angle=3.141592653589793;
    if(bounds.Y!=0)angle=4.71238898038469;
    else if(bounds.Height!=512)angle=1.5707963267948966;
    Clear_Map_Cell_Traversal_Marks_00582a50();
    DynamicVectorClass<Cell> *seeds=Build_Region_Border_Cell_List(region);
    int seed_count=seeds->Count();
    int index=0;
    if(seed_count>0) {
        UnknownVeinNode *seedNode=nodes;
        do {
        Cell position=(*seeds)[index];
        if(bounds.Is_Point_Within(Point2D(position.X,position.Y))) {
            float &seedScore=seedNode->Key;
            seedNode->Position=position;
            seedScore=Get_Angle_Score(origin,position,angle);
            ++used;
            queue->Insert(seedNode++);
        }
        } while(++index<seed_count);
    }
    UnknownVeinNode *next;
    delete seeds;
    double logarithm=log((double)seed_count);
    if(1.0>logarithm)logarithm=1.0;
    int limit=(int)(1.0f/((float)(1.0/logarithm)*scale)*0.5f);
    limit+=Map_Random_Range_005797a0(0,limit/2);
    UnknownVeinNode *node=queue->Remove_Min();
    int step=0;
    if(limit>0) {
      while(true) {
        if(!success || !node)break;
        Cell const &position=node->Position;
        UnknownMapGenerationCell &data=MapRegionClass::Get_Cell_Data(position);
        if(data.RegionIndex==0 && Map[position].unknown_47b590()) {
            data.unknown_4b=1;
            data.RegionIndex=region;
        }
        next=&nodes[used];
        for(int direction=0;direction<8;direction+=2) {
            Cell adjacent=CellClass::Adjacent_Cell(position,direction);
            if(Generation_Cell_In_Bounds_0058da10(adjacent)) {
                if(Get_Cell_Data_Region(adjacent)==0 && Get_Cell_Data_Spread(adjacent)!=region && bounds.Is_Point_Within(Point2D(adjacent.X,adjacent.Y)) && Map[adjacent].unknown_47b590()) {
                    next->Position=adjacent;
                    next->Key=Get_Angle_Score(origin,adjacent,angle);
                    Set_Cell_Data_Spread(adjacent,region);
                    ++used;
                    queue->Insert(next++);
                } else {
                    int other=UnknownMapGenerationCell::Get_Region(adjacent);
                    if(other!=0 && other!=region)success=false;
                }
            }
        }
        ++step;
        angle+=unknown_gaussian_value_005798d0(0.0,0.7853981633974483);
        node=queue->Remove_Min();
        if(step>=limit)break;
      }
    }
    if(frontier) {
        UnknownVeinNode *remaining=queue->Remove_Min();
        while(remaining) {
            if(!success)break;
            int other=UnknownMapGenerationCell::Get_Region(remaining->Position);
            if(other==0)Set_Cell_Data_Region(remaining->Position,region);
            else if(other!=region)success=false;
            remaining=queue->Remove_Min();
        }
    }
    delete[] nodes;
    delete queue;
    return success;
}
#endif

// Not exact (RA2_NON_MATCHING): MapGeneratorClass::unknown_0057d3a0 at 0x0057D3A0, best score 51.42, 2026-11-13; differs: 696 instructions in the original, 821 in the draft (shape 87.94)
#ifdef RA2_NON_MATCHING
// int MapGeneratorClass::unknown_0057d3a0(int count, Rect const &area, Point2D const &seed, bool ellipse,
//     Cell const &origin, double weight, bool directional) (0x0057D3A0)
// Grows one land patch of at least 400 cells for the land seeders (Seed_Land_Type1/Type2): from seed, or from
// a random clear cell with no region (up to 200 tries; 0 when none is found), a priority queue spreads over the
// four straight neighbours inside the generation diamond and area's ellipse, keyed by the distance to a centre
// that drifts by the generator's gaussian times weight (or, when directional, by the direction-weighted
// distance 0x0057D120). The reached cells take the working region, the region's shores are built, and a region
// whose shores fail is turned back to plain tiles at the generator's cell height (0); otherwise the working
// region id advances and the number of cells taken is returned. Drafted 2026-11-13 from the disassembly
// (TibSun mapgen.cpp Grow_Water_Region queue shape; the vein heap of shared/vein_heap_core.hpp).

int MapGeneratorClass::unknown_0057d3a0(int count, Rect const &area, Point2D const &seed, bool ellipse, Cell const &origin, double weight, bool directional)
{
    if (count <= 400) {
        count = 400;
    }
    int capacity = count * 8 + 2;
    if (capacity <= 100) {
        capacity = 100;
    }
    UnknownVeinNode *nodes = new UnknownVeinNode[capacity];
    UnknownVeinHeap *queue = new UnknownVeinHeap(capacity);

    Cell start;
    if (seed.X == 0 && seed.Y == 0) {
        int tries = 0;
        do {
            unsigned x_max = Map.MapSize.Width - 1;
            unsigned x;
            do {
                x = (unsigned)((unsigned)unknown_map_random_a705c0() * (double)(x_max + 1) * (1.0 / 4294967296.0));
            } while (x > x_max);
            unsigned y_max = Map.MapSize.Height - 1;
            unsigned y;
            do {
                y = (unsigned)((unsigned)unknown_map_random_a705c0() * (double)(y_max + 1) * (1.0 / 4294967296.0));
            } while (y > y_max);
            start = Cell(x + 1, Map.MapSize.Width - x) + Cell(y, y);
            if (++tries >= 200) {
                break;
            }
        } while (Get_Cell_Data_Region(start) != 0 || !Map[start].unknown_47b7e0());
        if (tries >= 200) {
            return 0;
        }
    } else {
        start = Cell(seed.X, seed.Y);
    }

    float x_weight = 0.0f;
    float y_weight = 0.0f;
    if (area != unknown_static_rect) {
        int width = area.Width;
        int height = area.Height;
        if (width > height) {
            x_weight = 1.0f;
            y_weight = (float)((double)width / height * 1.2);
        } else {
            y_weight = 1.0f;
            x_weight = (float)((double)height / width * 1.2);
        }
    }

    double center_x = start.X;
    double center_y = start.Y;
    int placed = 0;
    queue->Clear();

    UnknownVeinNode *node = nodes;
    int used = 1;
    node->Position = start;
    node->Key = 0;
    Set_Cell_Data_Spread(start, WorkingRegionID);
    queue->Insert(node);
    UnknownVeinNode *current = queue->Remove_Min();

    double half_width = area.Width * 0.5;
    double x_scale = 1.0 / (half_width * half_width);
    double half_height = area.Height * 0.5;
    double y_scale = 1.0 / (half_height * half_height);

    while (placed < count && current != 0) {
        if (unknown_map_cells_a70a24 != 0) {
            MapRegionClass::Get_Cell_Data(current->Position).RegionIndex = WorkingRegionID;
        }
        Map[current->Position].IsoTileIndex = 0;
        UnknownVeinNode *next = &nodes[used];
        for (int direction = 0; direction < 8; direction += 2) {
            Cell adjacent = current->Position + unknown_adjacent_851a70[direction & 7];
            int x = adjacent.X;
            int y = adjacent.Y;
            if (y + x > unknown_map_min_a70a1c && x - y < unknown_map_min_a70a1c &&
                y - x < unknown_map_min_a70a1c && y + x <= unknown_map_max_a70a20) {
                UnknownMapGenerationCell &data = MapRegionClass::Get_Cell_Data(adjacent);
                if (data.RegionIndex == 0 && data.unknown_3c != (int)WorkingRegionID) {
                    if (Map[adjacent].unknown_47b7e0() && used < capacity &&
                        Cell_In_Generation_Shape_0057d290(adjacent, area, ellipse, x_scale, y_scale)) {
                        next->Position = adjacent;
                        Cell center((short)(center_x + 0.5), (short)(center_y + 0.5));
                        double key;
                        if (directional) {
                            key = Direction_Weighted_Cell_Distance_0057d120(&center, &adjacent, &area, &origin, x_weight, y_weight);
                        } else {
                            int dx = adjacent.X - center.X;
                            int dy = adjacent.Y - center.Y;
                            key = unknown_sqrt_4ba410((double)(dx * dx + dy * dy))
                                + (unsigned)unknown_map_random_a705c0() * 5.0 * (1.0 / 4294967296.0);
                        }
                        next->Key = (float)key;
                        data.unknown_3c = WorkingRegionID;
                        used++;
                        queue->Insert(next++);
                    }
                }
            }
        }
        center_x += unknown_map_gaussian_a6fd50.Next() * weight;
        center_y += unknown_map_gaussian_a6fd50.Next() * weight;
        placed++;
        current = queue->Remove_Min();
    }

    for (current = queue->Remove_Min(); current != 0; current = queue->Remove_Min(), placed++) {
        CellClass &cell = Map[current->Position];
        UnknownMapGenerationCell &data = MapRegionClass::Get_Cell_Data(current->Position);
        if (data.RegionIndex == 0 && cell.unknown_47b7e0()) {
            cell.IsoTileIndex = 0;
            data.RegionIndex = WorkingRegionID;
        }
    }

    delete[] nodes;
    delete queue;

    bool shores = Map.Build_All_Shores(WorkingRegionID, true);

    Map.Reset_Iterator();
    for (CellClass *cell = Map.Iterate(); cell != 0; cell = Map.Iterate()) {
        if (cell->IsoTileIndex >= unknown_tileset_a6cb74 && cell->IsoTileIndex < unknown_tileset_a6cb74 + 42) {
            cell->IsoTileIndex = 0;
            cell->unknown_11a = 0;
        }
    }

    if (!shores) {
        int tile = unknown_tileset_a52580;
        int region = WorkingRegionID;
        Map.Reset_Iterator();
        for (CellClass *cell = Map.Iterate(); cell != 0; cell = Map.Iterate()) {
            UnknownMapGenerationCell &data = MapRegionClass::Get_Cell_Data(cell->MapCoords);
            if (data.RegionIndex == region) {
                data.RegionIndex = 0;
                data.unknown_4b = 0;
                cell->IsoTileIndex = tile;
                cell->unknown_11a = 0;
                cell->unknown_11b = (char)CellHeight;
            }
        }
        return 0;
    }

    WorkingRegionID++;
    return placed;
}
#endif

extern "C" void __cdecl release_58da50(void*);
class Leaf_58da50 { public: void cleanup();void* get(unsigned int flags); };
void* Leaf_58da50::get(unsigned int flags) { cleanup(); if (flags & 1) release_58da50(this); return this; }


extern "C" void __cdecl release_58dd30(void*);
class Cleanup_58dd30 { unsigned char pad0[4]; unsigned int* items; int count; unsigned char pad12[1]; unsigned char owns; unsigned char pad14[2]; int capacity; public: void get(); };
void Cleanup_58dd30::get() { unsigned int* pointer=items; capacity=0; if(pointer && owns) { release_58dd30(pointer); items=0; } owns=0; count=0; }


extern "C" void __cdecl release_58dd90(void*);
class Cleanup_58dd90 { unsigned char pad0[4]; unsigned int* items; int count; unsigned char pad12[1]; unsigned char owns; unsigned char pad14[2]; int capacity; public: void get(); };
void Cleanup_58dd90::get() { unsigned int* pointer=items; capacity=0; if(pointer && owns) { release_58dd90(pointer); items=0; } owns=0; count=0; }


// 0x0058de40: function not yet identified
//
// Owner: DynamicVectorClass<CellClass *>, virtual slot 5 (vtable +0x14) (from RTTI/vtables).
// Body: 27 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: uint __thiscall unknown_index_of_0058de40(int param_1,int param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0058de40 {

struct Owner_0058de40 { unsigned int unknown_index_of_0058de40(int item); };

unsigned int Owner_0058de40::unknown_index_of_0058de40(int item)
{
    int self = (int)this;
    if (*(char *)(self + 0xc) == '\0') {
        return 0;
    }
    return (unsigned int)(item - *(int *)(self + 4)) >> 2;
}

}  // namespace seed_0058de40


extern "C" void __cdecl release_58de60(void*);
class Cleanup_58de60 { unsigned char pad0[4]; unsigned int* items; int count; unsigned char pad12[1]; unsigned char owns; unsigned char pad14[2]; int capacity; public: void get(); };
void Cleanup_58de60::get() { unsigned int* pointer=items; capacity=0; if(pointer && owns) { release_58de60(pointer); items=0; } owns=0; count=0; }


// 0x0058e830: function not yet identified
//
// Owner: DynamicVectorClass<TRect<int>>, virtual slot 4 (vtable +0x10) (from RTTI/vtables).
// Body: 74 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: int __thiscall ID_0058e830(int param_1,int *param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0058e830 {

struct Owner_0058e830 { int ID_0058e830(int *rect); };

int Owner_0058e830::ID_0058e830(int *rect)
{
    int self = (int)this;
    int index;
    int *item;

    index = (int)(0);
    if (0 < *(int *)(self + 0x10)) {
        item = (int *)(*(int **)(self + 4));
        do {
            if ((((*item == *rect) && (item[1] == rect[1])) && (item[2] == rect[2])) &&
                  (item[3] == rect[3])) {
                return index;
            }
            index = (int)(index + 1);
            item = (int *)(item + 4);
        } while (index < *(int *)(self + 0x10));
    }
    return -1;
}

}  // namespace seed_0058e830


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<MapRegionClass *>::Clear (0x0058E000; VectorClass<MapRegionClass *> vtable 0x007a5db0 slot 3).
// VectorClass<double>::Clear (0x0058EA60; VectorClass<double> vtable 0x007a5f8c slot 3).
// 0058efb0: emitted by this unit's use of its list


extern "C" unsigned char vtable_58ec00[]; extern "C" void __cdecl release_buffer_58ec00(void*); extern "C" void __cdecl release_object_58ec00(void*);
class Leaf_58ec00 { public: void* get(unsigned int flags); };
void* Leaf_58ec00::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_58ec00; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_58ec00((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_58ec00(this); return this; }


extern "C" unsigned char vtable_58ec50[]; extern "C" void __cdecl release_buffer_58ec50(void*); extern "C" void __cdecl release_object_58ec50(void*);
class Leaf_58ec50 { public: void* get(unsigned int flags); };
void* Leaf_58ec50::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_58ec50; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_58ec50((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_58ec50(this); return this; }


extern "C" unsigned char vtable_58eca0[]; extern "C" void __cdecl release_buffer_58eca0(void*); extern "C" void __cdecl release_object_58eca0(void*);
class Leaf_58eca0 { public: void* get(unsigned int flags); };
void* Leaf_58eca0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_58eca0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_58eca0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_58eca0(this); return this; }


extern "C" unsigned char vtable_58ecf0[]; extern "C" void __cdecl release_buffer_58ecf0(void*); extern "C" void __cdecl release_object_58ecf0(void*);
class Leaf_58ecf0 { public: void* get(unsigned int flags); };
void* Leaf_58ecf0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_58ecf0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_58ecf0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_58ecf0(this); return this; }


extern "C" unsigned char vtable_58ed40[]; extern "C" void __cdecl release_buffer_58ed40(void*); extern "C" void __cdecl release_object_58ed40(void*);
class Leaf_58ed40 { public: void* get(unsigned int flags); };
void* Leaf_58ed40::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_58ed40; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_58ed40((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_58ed40(this); return this; }


extern "C" unsigned char vtable_58ed90[]; extern "C" void __cdecl release_buffer_58ed90(void*); extern "C" void __cdecl release_object_58ed90(void*);
class Leaf_58ed90 { public: void* get(unsigned int flags); };
void* Leaf_58ed90::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_58ed90; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_58ed90((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_58ed90(this); return this; }


extern "C" unsigned char vtable_58ede0[]; extern "C" void __cdecl release_buffer_58ede0(void*); extern "C" void __cdecl release_object_58ede0(void*);
class Leaf_58ede0 { public: void* get(unsigned int flags); };
void* Leaf_58ede0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_58ede0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_58ede0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_58ede0(this); return this; }


extern "C" unsigned char vtable_58ee30[]; extern "C" void __cdecl release_buffer_58ee30(void*); extern "C" void __cdecl release_object_58ee30(void*);
class Leaf_58ee30 { public: void* get(unsigned int flags); };
void* Leaf_58ee30::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_58ee30; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_58ee30((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_58ee30(this); return this; }


extern "C" unsigned char vtable_58ee80[]; extern "C" void __cdecl release_buffer_58ee80(void*); extern "C" void __cdecl release_object_58ee80(void*);
class Leaf_58ee80 { public: void* get(unsigned int flags); };
void* Leaf_58ee80::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_58ee80; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_58ee80((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_58ee80(this); return this; }


extern "C" unsigned char table_58ef60[]; extern "C" void* __cdecl storage_58ef60(unsigned int);
class Storage_58ef60 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_58ef60::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_58ef60; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_58ef60(requested << 2); owns=1; } return this; }


extern "C" unsigned char table_58f040[]; extern "C" void* __cdecl storage_58f040(unsigned int);
class Storage_58f040 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_58f040::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_58f040; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_58f040(requested << 2); owns=1; } return this; }


// 0x0058f1e0: function not yet identified
//
// Body: 34 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: int unknown_compare_0058f1e0(int *param_1,int *param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//
// Adjusted from the Ghidra seed by permute_seeds.py (signedness, comparison order) to
// match the original; edit freely, this file is no longer regenerated.

namespace seed_0058f1e0 {

int  unknown_compare_0058f1e0(int *a, int *b)
{
    if (*a == *b) {
        return 0;
    }
    return ((*a >= *b) - 1 & 0xfffffffe) + 1;
}

}  // namespace seed_0058f1e0


extern "C" unsigned char table_58f210[]; extern "C" void* __cdecl storage_58f210(unsigned int);
class Storage_58f210 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_58f210::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_58f210; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_58f210(requested << 2); owns=1; } return this; }


extern "C" unsigned char table_58f2d0[]; extern "C" void* __cdecl storage_58f2d0(unsigned int);
class Storage_58f2d0 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_58f2d0::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_58f2d0; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_58f2d0(requested << 3); owns=1; } return this; }
