// smartdeform.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/smartdeform.json carries the addresses and the rows that
// stay in their own files.


#define RA2_UNDO_INFO_POINT
#define RA2_RECT_POINT_INLINE
#define RA2_RECT_MINUS_INLINE
#define RA2_CELL_LAT_BOOL
#define RA2_MAP_VIEW_56A870
// Header-static dynamic initializers of the unit at 0x00684D50 and its
// list globals (UndoInfoStruct); the other drafts of this plan are emitted by this file.
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/vector_class.hpp"

#include "shared/vector_elements.hpp"

// Corner heights of each ramp shape, indexed by a cell's ramp number (TibSun
// smartdeform.cpp _ramp_corners; values checked against the original image: a full
// height of 30 pixels, half 15). _ramp_corners0 is all zero and sits in .bss.
#define NUM_CORNERS 4
#define RAMP_FULL_HEIGHT 30
#define RAMP_HALF_HEIGHT (RAMP_FULL_HEIGHT / 2)
#define RAMP_COUNT 21

// A flat tile
static int _ramp_corners0[NUM_CORNERS] = {00, 00, 00, 00};

// Standard (two corners high).
static int _ramp_corners1[NUM_CORNERS] = {00, RAMP_HALF_HEIGHT, RAMP_HALF_HEIGHT, 00};
static int _ramp_corners2[NUM_CORNERS] = {00, 00, RAMP_HALF_HEIGHT, RAMP_HALF_HEIGHT};
static int _ramp_corners3[NUM_CORNERS] = {RAMP_HALF_HEIGHT, 00, 00, RAMP_HALF_HEIGHT};
static int _ramp_corners4[NUM_CORNERS] = {RAMP_HALF_HEIGHT, RAMP_HALF_HEIGHT, 00, 00};

// Tile outside corners (one corner high).
static int _ramp_corners5[NUM_CORNERS] = {00, 00, RAMP_HALF_HEIGHT, 00};
static int _ramp_corners6[NUM_CORNERS] = {00, 00, 00, RAMP_HALF_HEIGHT};
static int _ramp_corners7[NUM_CORNERS] = {RAMP_HALF_HEIGHT, 00, 00, 00};
static int _ramp_corners8[NUM_CORNERS] = {00, RAMP_HALF_HEIGHT, 00, 00};

// Tile inside corners (three corners high).
static int _ramp_corners9[NUM_CORNERS] = {00, RAMP_HALF_HEIGHT, RAMP_HALF_HEIGHT, RAMP_HALF_HEIGHT};
static int _ramp_corners10[NUM_CORNERS] = {RAMP_HALF_HEIGHT, 00, RAMP_HALF_HEIGHT, RAMP_HALF_HEIGHT};
static int _ramp_corners11[NUM_CORNERS] = {RAMP_HALF_HEIGHT, RAMP_HALF_HEIGHT, 00, RAMP_HALF_HEIGHT};
static int _ramp_corners12[NUM_CORNERS] = {RAMP_HALF_HEIGHT, RAMP_HALF_HEIGHT, RAMP_HALF_HEIGHT, 00};

// Steep (two corners high, one corner double high).
static int _ramp_corners13[NUM_CORNERS] = {00, RAMP_HALF_HEIGHT, RAMP_FULL_HEIGHT, RAMP_HALF_HEIGHT};
static int _ramp_corners14[NUM_CORNERS] = {RAMP_HALF_HEIGHT, 00, RAMP_HALF_HEIGHT, RAMP_FULL_HEIGHT};
static int _ramp_corners15[NUM_CORNERS] = {RAMP_FULL_HEIGHT, RAMP_HALF_HEIGHT, 00, RAMP_HALF_HEIGHT};
static int _ramp_corners16[NUM_CORNERS] = {RAMP_HALF_HEIGHT, RAMP_FULL_HEIGHT, RAMP_HALF_HEIGHT, 00};

// Double ramps (two corners high, alternating).
static int _ramp_corners17[NUM_CORNERS] = {00, RAMP_HALF_HEIGHT, 00, RAMP_HALF_HEIGHT};
static int _ramp_corners18[NUM_CORNERS] = {RAMP_HALF_HEIGHT, 00, RAMP_HALF_HEIGHT, 00};
static int _ramp_corners19[NUM_CORNERS] = {00, RAMP_HALF_HEIGHT, 00, RAMP_HALF_HEIGHT};
static int _ramp_corners20[NUM_CORNERS] = {RAMP_HALF_HEIGHT, 00, RAMP_HALF_HEIGHT, 00};

static int * _ramp_corners[RAMP_COUNT] = {
    _ramp_corners0,
    _ramp_corners1,
    _ramp_corners2,
    _ramp_corners3,
    _ramp_corners4,
    _ramp_corners5,
    _ramp_corners6,
    _ramp_corners7,
    _ramp_corners8,
    _ramp_corners9,
    _ramp_corners10,
    _ramp_corners11,
    _ramp_corners12,
    _ramp_corners13,
    _ramp_corners14,
    _ramp_corners15,
    _ramp_corners16,
    _ramp_corners17,
    _ramp_corners18,
    _ramp_corners19,
    _ramp_corners20
};


// The unit's list of UndoInfoStruct (0x00ABCE18; name is a hypothesis).
DynamicVectorClass<UndoInfoStruct> unknown_UndoInfoStruct_list_00abce18;


// Dynamic initializer (0x00684D80, _$E5) emitted by HeaderStatics_diagonal_init_00684d50.cpp; no source of its own.


// Dynamic initializer (0x00684DA0, _$E8) emitted by HeaderStatics_diagonal_init_00684d50.cpp; no source of its own.


// Dynamic initializer (0x00684DC0, _$E11) emitted by HeaderStatics_diagonal_init_00684d50.cpp; no source of its own.


// Dynamic initializer (0x00684DE0, _$E14) emitted by HeaderStatics_diagonal_init_00684d50.cpp; no source of its own.


// Dynamic initializer (0x00684E00, _$E17) emitted by HeaderStatics_diagonal_init_00684d50.cpp; no source of its own.


// Dynamic initializer (0x00684E20, _$E20) emitted by HeaderStatics_diagonal_init_00684d50.cpp; no source of its own.


// Dynamic initializer (0x00684E60, _$E23) emitted by HeaderStatics_diagonal_init_00684d50.cpp; no source of its own.


// Dynamic initializer (0x00684E90, _$E26) emitted by HeaderStatics_diagonal_init_00684d50.cpp; no source of its own.


// Dynamic initializer (0x00684EC0, _$E29) emitted by HeaderStatics_diagonal_init_00684d50.cpp; no source of its own.


// Dynamic initializer (0x00684EF0, _$E32) emitted by HeaderStatics_diagonal_init_00684d50.cpp; no source of its own.


// Dynamic initializer (0x00684F00, _$E35) emitted by HeaderStatics_diagonal_init_00684d50.cpp; no source of its own.


// Dynamic initializer (0x00684F20, _$E38) emitted by HeaderStatics_diagonal_init_00684d50.cpp; no source of its own.


// Dynamic initializer (0x00684F40, _$E43) emitted by HeaderStatics_diagonal_init_00684d50.cpp; no source of its own.


// atexit destructor (0x00684F80, _$E41) emitted by HeaderStatics_diagonal_init_00684d50.cpp; no source of its own.


// void __fastcall unknown_undo_record_684fc0(Point2D const &cell, int value) (0x00684FC0)
// Appends (cell X, cell Y, value) to the unit's undo list (0x00ABCE18).
// UndoInfoStruct (shared/vector_elements.hpp) and the list are defined by this unit's first source
// (HeaderStatics_diagonal_init_00684d50.cpp).
#include "shared/rect.hpp"

void __fastcall unknown_undo_record_684fc0(Point2D const &cell, int value)
{
    unknown_UndoInfoStruct_list_00abce18.Add(UndoInfoStruct(cell, value));
}


// void unknown_undo_restore_685030() (0x00685030)
// Writes every recorded value back, newest first, into the unit's cell
// table (0x00ABCEAC, eight bytes a cell, row width 0x008286E4), then
// empties the undo list.
// The cell table is the terrain-deformation grid of 0x00685290/0x00685600:
// Value is a corner height times 15, unknown_04 marks a locked corner,
// unknown_05 a changed one and unknown_06 the centre (0x00685290 only). Its
// constructors are the original's: new[] in 0x00685290 clears Value, 04 and
// 05, and 0x00686DD0 copies the corners member by member without 06.
// The grid origin and size (0x008286E0..0x008286EC) are another unit's data.
struct UnknownUndoCell {
    UnknownUndoCell() : Value(0), unknown_04(0), unknown_05(0) {}
    UnknownUndoCell(UnknownUndoCell const &other) : Value(other.Value), unknown_04(other.unknown_04), unknown_05(other.unknown_05) {}
    int Value;          // +0x00
    char unknown_04;    // +0x04
    char unknown_05;    // +0x05
    char unknown_06;    // +0x06
};
extern UnknownUndoCell *unknown_undo_cells_abceac;
extern int unknown_undo_width_8286e4;
extern int unknown_undo_x_8286e8;
extern int unknown_undo_y_8286ec;

void unknown_undo_restore_685030()
{
    for (int index = unknown_UndoInfoStruct_list_00abce18.Count() - 1; index >= 0; index--) {
        UndoInfoStruct &info = unknown_UndoInfoStruct_list_00abce18[index];
        unknown_undo_cells_abceac[info.CellPoint.Y * unknown_undo_width_8286e4 + info.CellPoint.X].Value = info.Value;
    }
    unknown_UndoInfoStruct_list_00abce18.Clear();
}


// bool __fastcall unknown_undo_cell_open_6850b0(CellClass *cell) (0x006850B0)
// Whether the cell is inside the map diamond, has no overlay or occupier,
// is not marked in the map-generation grid and its tile type allows it
// (unknown_2b0; true without a valid tile type). flag is unused; every caller
// sets dl. The bounds test is the inline form of 0x0058DA10, which goes out of
// line one level down (0x00685600).
#include "shared/cell.hpp"
#include "shared/map_region.hpp"
#include "shared/isometric_tile.hpp"
#include "shared/isometric_tile_type.hpp"
UnknownMapGenerationCell *__fastcall unknown_generation_cell_56dba0(Cell const &cell);

inline bool __fastcall Generation_Cell_In_Bounds_0058da10(Cell const &position)
{
    int x = position.X;
    int y = position.Y;
    if (y + x > unknown_map_min_a70a1c
        && x - y < unknown_map_min_a70a1c
        && y - x < unknown_map_min_a70a1c
        && y + x <= unknown_map_max_a70a20) {
        return true;
    }
    return false;
}

bool __fastcall unknown_undo_cell_open_6850b0(CellClass *cell, bool flag)
{
    if (Generation_Cell_In_Bounds_0058da10(cell->Fetch_CellID())
        && cell->OverlayIndex == -1
        && cell->OccupierPtr == 0) {
        if (!unknown_generation_cell_56dba0(cell->Fetch_CellID())->unknown_45[0]) {
            int tile = cell->IsoTileIndex;
            if (tile < IsoTileTypes.Count() && tile != 0xFFFF) {
                return IsoTileTypes[tile]->unknown_2b0;
            }
            return true;
        }
    }
    return false;
}


// bool __fastcall unknown_undo_cell_free_685150(CellClass *cell) (0x00685150)
// Whether the cell is free: every occupier with unknown_7d set is of kind
// 0x0F or 1, there is no second occupier, flag bits 0x500 are clear, the
// cell north (adjacent 0) is not blocked unless its 0x800 bit is set, the
// cell at adjacent 6 is blocked only with the 0x800 bit, there is no
// overlay, MapClass::In_Radar accepts it and its tile type allows it.
#include "shared/object.hpp"
#include "shared/map_view.hpp"
#include "shared/adjacent_cell.hpp"

bool __fastcall unknown_undo_cell_free_685150(CellClass *cell, bool flag)
{
    for (ObjectClass *object = cell->OccupierPtr; object != 0; object = object->unknown_30) {
        if (object->unknown_7d && object->Kind_Of() != 0x0F && object->Kind_Of() != 1) {
            return false;
        }
    }
    if (cell->AltOccupierPtr == 0 && !(cell->unknown_140 & 0x500)) {
        unsigned int flags = Map[AdjacentCell[0] + cell->MapCoords].unknown_140;
        if (!(flags & 0x500) || !(flags & 0x800)) {
            flags = Map[cell->MapCoords + AdjacentCell[6]].unknown_140;
            if (!(flags & 0x500) || (flags & 0x800)) {
                if (cell->OverlayIndex == -1 && Map.In_Radar(cell->MapCoords)) {
                    int tile = cell->IsoTileIndex;
                    if (tile < IsoTileTypes.Count() && tile != 0xFFFF) {
                        return IsoTileTypes[tile]->unknown_2b0;
                    }
                    return true;
                }
            }
        }
    }
    return false;
}


// void __fastcall Build_Deform_Grid_00685290(bool flag, Cell position) (0x00685290)
// Builds the terrain-deformation grid (0x00ABCEAC, one UnknownUndoCell per map
// cell corner, see 0x00685030) around position: corners within 14 cells take
// the cell's corner height times 15 and are locked unless the four cells
// sharing the corner are free; the corners of position are marked as centre.

extern int unknown_undo_rows_8286e0;

void __fastcall Build_Deform_Grid_00685290(bool flag, Cell position)
{
    if (unknown_undo_cells_abceac != 0) {
        delete unknown_undo_cells_abceac;
        unknown_undo_cells_abceac = 0;
    }
    unknown_undo_cells_abceac = new UnknownUndoCell[(Map.unknown_128 + 1) * (Map.unknown_12c + 1)];
    unknown_undo_width_8286e4 = Map.unknown_128 + 1;
    unknown_undo_rows_8286e0 = Map.unknown_12c + 1;
    unknown_undo_y_8286ec = Map.unknown_124;
    unknown_undo_x_8286e8 = Map.unknown_120;
    Cell lo = position - Cell(14, 14);
    Cell hi = position + Cell(14, 14);
    lo.X = lo.X < 0 ? 0 : lo.X;
    lo.Y = lo.Y < 0 ? 0 : lo.Y;
    int width = Map.unknown_128 - Map.unknown_120;
    hi.X = width < hi.X ? width : hi.X;
    int height = Map.unknown_12c - Map.unknown_124;
    hi.Y = height < hi.Y ? height : hi.Y;
    for (int y = lo.Y; y <= hi.Y; y++) {
        for (int x = lo.X; x <= hi.X; x++) {
            int value = -1;
            UnknownUndoCell *grid = &unknown_undo_cells_abceac[unknown_undo_width_8286e4 * y + x];
            grid->unknown_04 = 0;
            grid->unknown_05 = 0;
            grid->unknown_06 = 0;
            Cell cell(unknown_undo_x_8286e8 + x, unknown_undo_y_8286ec + y);
            if (cell.X >= Map.unknown_120 && cell.X <= Map.unknown_120 + Map.unknown_128
                && cell.Y >= Map.unknown_124 && cell.Y <= Map.unknown_124 + Map.unknown_12c) {
                int ramp = Map[cell].unknown_11c;
                int level = Map[cell].unknown_11b * 15;
                value = *_ramp_corners[ramp] + level;
                if (!unknown_undo_cell_free_685150(&Map[cell], flag)
                    || !unknown_undo_cell_free_685150(&Map[cell - Cell(0, 1)], flag)
                    || !unknown_undo_cell_free_685150(&Map[cell - Cell(1, 1)], flag)
                    || !unknown_undo_cell_free_685150(&Map[cell - Cell(1, 0)], flag)) {
                    grid->unknown_04 = 1;
                }
                if (cell == position || cell - Cell(0, 1) == position
                    || cell - Cell(1, 1) == position || cell - Cell(1, 0) == position) {
                    grid->unknown_06 = 1;
                }
            }
            grid->Value = value;
        }
    }
}


// void Init_Deform_Grid_RMG_00685600() (0x00685600)
// Builds the deformation grid over the whole map for the random map generator
// (declared by Generate_Hills 0x00584DD0): a corner off the map diamond falls
// back to the first adjacent cell inside it, and a corner is locked unless all
// four cells around it are open. The cell_open 0x006850B0 inline runs out of
// budget at the third corner, which leaves TPoint2D operator+ 0x0042ADD0 and
// the bounds test 0x0058DA10 out of line from there on.

extern int unknown_undo_rows_8286e0;

void Init_Deform_Grid_RMG_00685600()
{
    if (unknown_undo_cells_abceac != 0) {
        delete unknown_undo_cells_abceac;
        unknown_undo_cells_abceac = 0;
    }
    unknown_undo_cells_abceac = new UnknownUndoCell[(Map.unknown_128 + 1) * (Map.unknown_12c + 1)];
    unknown_undo_width_8286e4 = Map.unknown_128 + 1;
    unknown_undo_rows_8286e0 = Map.unknown_12c + 1;
    unknown_undo_x_8286e8 = Map.unknown_120;
    unknown_undo_y_8286ec = Map.unknown_124;
    Cell offsets[3] = { Cell(0, -1), Cell(-1, -1), Cell(-1, 0) };
    for (int y = 0; y < unknown_undo_rows_8286e0; y++) {
        for (int x = 0; x < unknown_undo_width_8286e4; x++) {
            int value = 0;
            int tries = 0;
            UnknownUndoCell *grid = &unknown_undo_cells_abceac[unknown_undo_width_8286e4 * y + x];
            grid->unknown_04 = 0;
            grid->unknown_05 = 0;
            Cell position(unknown_undo_x_8286e8 + x, unknown_undo_y_8286ec + y);
            Cell cell = position;
            while (!Generation_Cell_In_Bounds_0058da10(cell) && tries < 3) {
                cell = position + offsets[tries];
                tries++;
            }
            if (Generation_Cell_In_Bounds_0058da10(cell)) {
                int ramp = Map[cell].unknown_11c;
                int level = Map[cell].unknown_11b * 15;
                value = *_ramp_corners[ramp] + level;
                if (Generation_Cell_In_Bounds_0058da10(position + Cell(0, -1))
                    && !unknown_undo_cell_open_6850b0(&Map[position + Cell(0, -1)], true)) {
                    grid->unknown_04 = 1;
                }
                if (Generation_Cell_In_Bounds_0058da10(position + Cell(-1, -1))
                    && !unknown_undo_cell_open_6850b0(&Map[position + Cell(-1, -1)], true)) {
                    grid->unknown_04 = 1;
                }
                if (Generation_Cell_In_Bounds_0058da10(position + Cell(-1, 0))
                    && !unknown_undo_cell_open_6850b0(&Map[position + Cell(-1, 0)], true)) {
                    grid->unknown_04 = 1;
                }
                if (Generation_Cell_In_Bounds_0058da10(position)
                    && !unknown_undo_cell_open_6850b0(&Map[position], true)) {
                    grid->unknown_04 = 1;
                }
            } else {
                grid->unknown_04 = 1;
            }
            grid->Value = value;
        }
    }
}


// bool __fastcall Commit_Deform_Grid_00685ac0(bool forced, Cell center) (0x00685AC0)
// Writes the deformation grid around center back to the map. Every free cell with a changed
// corner and a corner spread under 16 takes the lowest corner / 15 as its height and the ramp
// matching its corners; a smudge on it is cleared whole, its occupiers are lifted and put back,
// and the touched cells and their LAT neighbours are redrawn. Frees the grid.
#include "shared/smudge_type.hpp"
#include "shared/tactical.hpp"

extern Rect unknown_rect_839630;

bool __fastcall Commit_Deform_Grid_00685ac0(bool forced, Cell center)
{
    Cell lo = center - Cell(14, 14);
    Cell hi = center + Cell(14, 14);
    lo.X = lo.X < 0 ? 0 : lo.X;
    lo.Y = lo.Y < 0 ? 0 : lo.Y;
    int width = Map.unknown_128 - Map.unknown_120;
    hi.X = width < hi.X ? width : hi.X;
    int height = Map.unknown_12c - Map.unknown_124;
    hi.Y = height < hi.Y ? height : hi.Y;
    Rect bound(0, 0, 0, 0);
    Cell least(32767, 32767);
    Cell most(0, 0);
    DynamicVectorClass<ObjectClass *> objects;
    for (int y = lo.Y; y <= hi.Y; y++) {
        for (int x = lo.X; x <= hi.X; x++) {
            CellClass *cell = &Map[Cell(x + unknown_undo_x_8286e8, y + unknown_undo_y_8286ec)];
            if (!unknown_undo_cell_free_685150(cell, forced)) continue;
            UnknownUndoCell &point1 = unknown_undo_cells_abceac[x + y * unknown_undo_width_8286e4];
            UnknownUndoCell &point2 = unknown_undo_cells_abceac[x + 1 + y * unknown_undo_width_8286e4];
            UnknownUndoCell &point3 = unknown_undo_cells_abceac[x + 1 + (y + 1) * unknown_undo_width_8286e4];
            UnknownUndoCell &point4 = unknown_undo_cells_abceac[x + (y + 1) * unknown_undo_width_8286e4];
            if (point1.unknown_05 || point2.unknown_05 || point3.unknown_05 || point4.unknown_05) {
                int heights[4];
                heights[0] = point1.Value;
                heights[1] = point2.Value;
                heights[2] = point3.Value;
                heights[3] = point4.Value;
                int lowest = 1000;
                int highest = 0;
                for (int i = 0; i < 4; i++) {
                    if (heights[i] < lowest) lowest = heights[i];
                    if (heights[i] > highest) highest = heights[i];
                }
                if (highest - lowest > 15) continue;
                objects.Clear();
                ObjectClass *object = cell->OccupierPtr;
                while (object) {
                    objects.Add(object);
                    object = object->unknown_30;
                }
                int index;
                for (index = 0; index < objects.Count(); index++) {
                    objects[index]->Mark(0);
                }
                Rect cell_bound(0, 0, 0, 0);
                cell_bound = cell->Cell_Render_Rect();
                int old_height = cell->unknown_11b;
                cell->unknown_11b = (char)(lowest / 15);
                Map.Lower_Max_Level(cell->unknown_11b);
                if (old_height != cell->unknown_11b) {
                    cell->Remove_Fogged_Objects();
                }
                cell->Recalc_Attributes(-1);
                if (cell->SmudgeIndex != -1) {
                    SmudgeTypeClass *smudge = SmudgeTypes[cell->SmudgeIndex];
                    Cell smudge_cell = cell->MapCoords;
                    smudge_cell -= Cell(cell->SmudgeData % smudge->Width, cell->SmudgeData / smudge->Width);
                    for (int row = 0; row < smudge->Height; row++) {
                        for (int column = 0; column < smudge->Width; column++) {
                            Map[smudge_cell + Cell(column, row)].SmudgeIndex = -1;
                        }
                    }
                }
                Cell current(cell->Fetch_CellID());
                Map.Radar_Background(current);
                if (current.X < least.X) least.X = current.X;
                if (current.X > most.X) most.X = current.X;
                if (current.Y < least.Y) least.Y = current.Y;
                if (current.Y > most.Y) most.Y = current.Y;
                for (int j = 0; j < 4; j++) {
                    heights[j] -= lowest;
                }
                for (int ramp = 0; ramp < 19; ramp++) {
                    int *corners = _ramp_corners[ramp];
                    if (corners[0] == heights[0] && corners[1] == heights[1]
                        && corners[2] == heights[2] && corners[3] == heights[3]) {
                        cell->unknown_11c = ramp;
                        if (ramp == 0) {
 cell->IsoTileIndex = unknown_tileset_a52f00;
 } else {
 cell->IsoTileIndex = ramp - 1 + unknown_tileset_a6e018;
 }
                        cell->Recalc_Attributes(-1);
                        Cell id(cell->Fetch_CellID());
                        Map.Radar_Background(id);
                        cell_bound = Union(cell_bound, cell->Cell_Render_Rect());
                        if (id.X < least.X) least.X = id.X;
                        if (id.X > most.X) most.X = id.X;
                        if (id.Y < least.Y) least.Y = id.Y;
                        if (id.Y > most.Y) most.Y = id.Y;
                        break;
                    }
                }
                bound = Union(bound, cell_bound);
                for (index = 0; index < objects.Count(); index++) {
                    objects[index]->Mark(1);
                    objects[index]->Set_Height_AGL(0);
                }
            }
        }
    }
    delete[] unknown_undo_cells_abceac;
    unknown_undo_cells_abceac = 0;
    bound.Y -= unknown_rect_839630.Y;
    unknown_tactical_83996c->Register_Dirty_Area(bound, false);
    for (int i = least.Y - 1; i <= most.Y + 1; i++) {
        for (int j = least.X - 1; j <= most.X + 1; j++) {
            Cell id(j, i);
            CellClass *target = &Map[id];
            Rect lat_rect = target->Cell_Render_Rect();
            if (target->Fixup_LAT()) {
                lat_rect = Union(lat_rect, target->Cell_Render_Rect());
                lat_rect = lat_rect - unknown_rect_839630.Top_Left();
                if (lat_rect.Is_Valid()) {
                    unknown_tactical_83996c->Register_Dirty_Area(lat_rect, false);
                }
            }
        }
    }
    return true;
}


// bool Commit_Deform_Grid_RMG_006863e0() (0x006863E0)
// Writes the deformation grid back to the map: every open cell with a changed corner and a
// corner spread under 16 takes the lowest corner / 15 as its height and the ramp that matches
// its corners; then frees the grid.
#include "shared/tileset_index.hpp"


bool Commit_Deform_Grid_RMG_006863e0()
{
    Map.Reset_Iterator();
    CellClass *cell = 0;
    for (;;) {
        cell = Map.Iterate();
        if (!cell) {
            break;
        }
        int x = cell->Fetch_CellID().X - unknown_undo_x_8286e8;
        int y = cell->Fetch_CellID().Y - unknown_undo_y_8286ec;
        UnknownUndoCell &point1 = unknown_undo_cells_abceac[x + y * unknown_undo_width_8286e4];
        UnknownUndoCell &point2 = unknown_undo_cells_abceac[x + 1 + y * unknown_undo_width_8286e4];
        UnknownUndoCell &point3 = unknown_undo_cells_abceac[x + 1 + (y + 1) * unknown_undo_width_8286e4];
        UnknownUndoCell &point4 = unknown_undo_cells_abceac[x + (y + 1) * unknown_undo_width_8286e4];
        if (point1.unknown_05 || point2.unknown_05 || point3.unknown_05 || point4.unknown_05) {
            int heights[4];
            heights[0] = point1.Value;
            heights[1] = point2.Value;
            heights[2] = point3.Value;
            heights[3] = point4.Value;
            int lowest = 1000;
            int highest = 0;
            for (int index = 0; index < 4; index++) {
                if (heights[index] < lowest) lowest = heights[index];
                if (heights[index] > highest) highest = heights[index];
            }
            if (highest - lowest > 15) continue;
            if (unknown_undo_cell_open_6850b0(cell, true)) {
                cell->unknown_11b = lowest / 15;
            } else {
                continue;
            }
            for (int j = 0; j < 4; j++) {
                heights[j] -= lowest;
            }
            for (int ramp = 0; ramp < 19; ramp++) {
                int *corners = _ramp_corners[ramp];
                if (corners[0] == heights[0] && corners[1] == heights[1]
                    && corners[2] == heights[2] && corners[3] == heights[3]) {
                    cell->unknown_11c = ramp;
                    if (ramp == 0) {
                        cell->IsoTileIndex = unknown_tileset_a52f00;
                    } else {
                        cell->IsoTileIndex = ramp - 1 + unknown_tileset_a6e018;
                    }
                    break;
                }
            }
        }
    }
    delete[] unknown_undo_cells_abceac;
    unknown_undo_cells_abceac = 0;
    return true;
}


// bool __fastcall Ripple_Deform_Points_00686610(Point2D start, int direction, bool forced) (0x00686610)
// Pulls the neighbours of a grid point to within one step (15) of it, recording each moved point
// on the undo list, and recurses from every point it moved. A pinned point out of step, or the
// edge of the grid, stops the operation unless it is forced.
#include "shared/crt_memory.hpp"

extern int unknown_undo_rows_8286e0;
bool __fastcall Ripple_Deform_Points_00686610(Point2D start, int direction, bool forced)
{
    UnknownUndoCell &origin = unknown_undo_cells_abceac[start.X + start.Y * unknown_undo_width_8286e4];
    int height = origin.Value;
    for (int dy = -1; dy < 2; dy++) {
        for (int dx = -1; dx < 2; dx++) {
            if (!(dx || dy)) continue;
            Point2D point = Point2D(dx, dy) + start;
            if (point.X < 0 || point.Y < 0 || point.X >= unknown_undo_width_8286e4 || point.Y >= unknown_undo_rows_8286e0) {
                if (forced) continue;
                return false;
            }
            UnknownUndoCell &cell = unknown_undo_cells_abceac[point.X + point.Y * unknown_undo_width_8286e4];
            if (!cell.unknown_04 || (forced && cell.unknown_06)) {
                if (abs(cell.Value - height) > 15) {
                    if (direction == 1) {
                        if (cell.Value < height) {
                            unknown_undo_record_684fc0(point, cell.Value);
                            cell.Value = height - 15;
                            cell.unknown_05 = 1;
                        }
                    } else {
                        if (cell.Value > height) {
                            unknown_undo_record_684fc0(point, cell.Value);
                            cell.Value = height + 15;
                            cell.unknown_05 = 1;
                        }
                    }
                    if (!cell.unknown_04) {
                        if (!Ripple_Deform_Points_00686610(point, direction, forced)) {
                            if (!forced) {
                                return false;
                            }
                        }
                    }
                }
            } else {
                if (abs(cell.Value - height) > 15) {
                    return false;
                }
            }
        }
    }
    return true;
}


// bool __fastcall Deform_Cell_00686860(Cell position, int direction, bool forced, unsigned corners) (0x00686860)
// Raises or lowers the masked corners of a cell by one step, ripples the change outward and
// writes the grid back to the map; a refused corner frees the grid unless forced.
void __fastcall Build_Deform_Grid_00685290(bool flag, Cell position);
bool __fastcall Commit_Deform_Grid_00685ac0(bool forced, Cell position);

bool __fastcall Deform_Cell_00686860(Cell position, int direction, bool forced, unsigned corners)
{
    unknown_UndoInfoStruct_list_00abce18.Clear();
    Build_Deform_Grid_00685290(forced, position);
    int mask = ((corners >> 1) & 4) | ((corners & 4) << 1) | (corners & 3);
    int rx = position.X - unknown_undo_x_8286e8;
    int ry = position.Y - unknown_undo_y_8286ec;
    int bit = 1;
    for (int y = ry; y < ry + 2; y++) {
        for (int x = rx; x < rx + 2; x++) {
            if (bit & mask) {
                UnknownUndoCell &cell = unknown_undo_cells_abceac[unknown_undo_width_8286e4 * y + x];
                if (!cell.unknown_04 || forced) {
                    cell.Value += direction * 15;
                    if (cell.Value < 0 || cell.Value > 180) {
                        cell.Value -= direction * 15;
                    }
                    cell.unknown_05 = 1;
                    if (!Ripple_Deform_Points_00686610(Point2D(x, y), direction, forced) && !forced) {
                        delete[] unknown_undo_cells_abceac;
                        unknown_undo_cells_abceac = 0;
                        return false;
                    }
                } else if (!forced) {
                    delete[] unknown_undo_cells_abceac;
                    unknown_undo_cells_abceac = 0;
                    return false;
                }
            }
            bit <<= 1;
        }
    }
    return Commit_Deform_Grid_00685ac0(forced, position);
}


// bool __fastcall Deform_Cell_RMG_006869f0(Cell position, int direction, bool forced, unsigned corners) (0x006869F0)
// The map generator's Deform_Cell: moves the masked corners on the prepared grid, recording each
// on the undo list; a refused corner restores the recorded points unless forced.
extern int unknown_undo_rows_8286e0;
bool __fastcall Deform_Cell_RMG_006869f0(Cell position, int direction, bool forced, unsigned corners)
{
    unknown_UndoInfoStruct_list_00abce18.Clear();
    int mask = ((corners >> 1) & 4) | ((corners & 4) << 1) | (corners & 3);
    int rx = position.X - unknown_undo_x_8286e8;
    int ry = position.Y - unknown_undo_y_8286ec;
    int bit = 1;
    for (int y = ry; y < ry + 2; y++) {
        for (int x = rx; x < rx + 2; x++) {
            if (bit & mask) {
                UnknownUndoCell &cell = unknown_undo_cells_abceac[unknown_undo_width_8286e4 * y + x];
                if (!cell.unknown_04) {
                    cell.Value += direction * 15;
                    if (cell.Value < 0 || cell.Value > 180) {
                        cell.Value -= direction * 15;
                    } else {
                        unknown_undo_record_684fc0(Point2D(x, y), cell.Value - direction * 15);
                    }
                    cell.unknown_05 = 1;
                    if (!Ripple_Deform_Points_00686610(Point2D(x, y), direction, forced) && !forced) {
                        unknown_undo_restore_685030();
                        return false;
                    }
                } else if (!forced) {
                    unknown_undo_restore_685030();
                    return false;
                }
            }
            bit <<= 1;
        }
    }
    return true;
}


// int __fastcall Get_Deformed_Cell_Height_00686c90(CellClass *cell) (0x00686C90)
// The cell's height in the deformation grid: the lowest of its four corners
// (divided by 15) when the cell is open (the 0x006850B0 test written out),
// else the map height. The corner index comes from two Fetch_CellID()
// temporaries; the test copies the coordinates into a named local (frame and
// registers match only this way).

int __fastcall Get_Deformed_Cell_Height_00686c90(CellClass *cell)
{
    int x = cell->Fetch_CellID().X - unknown_undo_x_8286e8;
    int y = cell->Fetch_CellID().Y - unknown_undo_y_8286ec;
    int heights[4];
    heights[0] = unknown_undo_cells_abceac[y * unknown_undo_width_8286e4 + x].Value;
    heights[1] = unknown_undo_cells_abceac[y * unknown_undo_width_8286e4 + x + 1].Value;
    heights[2] = unknown_undo_cells_abceac[(y + 1) * unknown_undo_width_8286e4 + x + 1].Value;
    heights[3] = unknown_undo_cells_abceac[(y + 1) * unknown_undo_width_8286e4 + x].Value;
    int lowest = 1000;
    for (int index = 0; index < 4; index++) {
        if (heights[index] < lowest) {
            lowest = heights[index];
        }
    }
    Cell coords = cell->MapCoords;
    if (coords.Y + coords.X > unknown_map_min_a70a1c
        && coords.X - coords.Y < unknown_map_min_a70a1c
        && coords.Y - coords.X < unknown_map_min_a70a1c
        && coords.Y + coords.X <= unknown_map_max_a70a20
        && cell->OverlayIndex == -1
        && cell->OccupierPtr == 0) {
        if (!unknown_generation_cell_56dba0(cell->Fetch_CellID())->unknown_45[0]) {
            int tile = cell->IsoTileIndex;
            if (tile >= IsoTileTypes.Count() || tile == 0xFFFF || IsoTileTypes[tile]->unknown_2b0) {
                return lowest / 15;
            }
        }
    }
    return cell->unknown_11b;
}


// int __fastcall Get_Deformable_Cell_Corners_00686dd0(CellClass *cell, int direction) (0x00686DD0)
// The mask (bit i = corner i: NW, NE, SE, SW) of the cell's unlocked corners
// that a step in direction would move: every unlocked corner on flat ground,
// else those below the highest (raising) or above the lowest (lowering).

int __fastcall Get_Deformable_Cell_Corners_00686dd0(CellClass *cell, int direction)
{
    int x = cell->Fetch_CellID().X - unknown_undo_x_8286e8;
    int y = cell->Fetch_CellID().Y - unknown_undo_y_8286ec;
    UnknownUndoCell corners[4] = {
        unknown_undo_cells_abceac[y * unknown_undo_width_8286e4 + x],
        unknown_undo_cells_abceac[y * unknown_undo_width_8286e4 + x + 1],
        unknown_undo_cells_abceac[(y + 1) * unknown_undo_width_8286e4 + x + 1],
        unknown_undo_cells_abceac[(y + 1) * unknown_undo_width_8286e4 + x]
    };
    int heights[4] = { corners[0].Value, corners[1].Value, corners[2].Value, corners[3].Value };
    int lowest = 1000;
    int highest = -1;
    int index;
    for (index = 0; index < 4; index++) {
        if (heights[index] < lowest) {
            lowest = heights[index];
        }
        if (heights[index] > highest) {
            highest = heights[index];
        }
    }
    int result = 0;
    if (highest == lowest) {
        for (index = 0; index < 4; index++) {
            if (!corners[index].unknown_04) {
                result += 1 << index;
            }
        }
        return result;
    }
    if (direction > 0) {
        for (index = 0; index < 4; index++) {
            if (heights[index] < highest && !corners[index].unknown_04) {
                result += 1 << index;
            }
        }
        return result;
    }
    for (index = 0; index < 4; index++) {
        if (heights[index] > lowest && !corners[index].unknown_04) {
            result += 1 << index;
        }
    }
    return result;
}


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<UndoInfoStruct>::Clear (0x00686F40; DynamicVectorClass<UndoInfoStruct> vtable 0x007ab50c slot 3).
// The element type is left incomplete: Clear does not depend on it.
// DynamicVectorClass<UndoInfoStruct>::ID_pointer (0x00686F70; DynamicVectorClass<UndoInfoStruct> vtable 0x007ab50c slot 5).
// UndoInfoStruct is a size-only stand-in (12 bytes, from the ID(T const *) divisor);
// its real fields are not recovered.
// VectorClass<UndoInfoStruct>::Fetch (0x00686FA0; VectorClass<UndoInfoStruct> vtable 0x007ab52c slot 6).
// UndoInfoStruct is a size-only stand-in (12 bytes, from the ID(T const *) divisor);
// its real fields are not recovered.
// VectorClass<UndoInfoStruct>::operator== (0x00686FD0).


// VectorClass<UndoInfoStruct>::Resize (0x00687030), emitted by HeaderStatics_diagonal_init_00684d50.cpp; no source of its own.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<UndoInfoStruct>::Clear (0x00687130; VectorClass<UndoInfoStruct> vtable 0x007ab52c slot 3).
// The element type is left incomplete: Clear does not depend on it.
// VectorClass<UndoInfoStruct>::ID_object (0x00687160; virtual slot 4).
// Layout and behaviour: shared/vector_class.hpp; element comparison: shared/vector_elements.hpp.
// VectorClass<UndoInfoStruct>::ID_pointer (0x006871B0; VectorClass<UndoInfoStruct> vtable 0x007ab52c slot 5).
// UndoInfoStruct is a size-only stand-in (12 bytes, from the ID(T const *) divisor);
// its real fields are not recovered.


// DynamicVectorClass<UndoInfoStruct>::Resize (0x006871E0), emitted by HeaderStatics_diagonal_init_00684d50.cpp; no source of its own.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<UndoInfoStruct>::ID_object (0x006872F0; virtual slot 4).
// Layout and behaviour: shared/vector_class.hpp; element comparison: shared/vector_elements.hpp.
