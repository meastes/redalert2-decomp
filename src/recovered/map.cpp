// map.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/map.json carries the addresses and the rows that
// stay in their own files.


#define RA2_COORD_FROM_CELL_INLINE
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/coord.hpp"
#include "shared/map.hpp"
#include "shared/map_cell.hpp"
#include "shared/path_estimator.hpp"
#include "shared/house_screen.hpp"
#include "shared/map_view.hpp"
#include "shared/map_techno.hpp"
#include "shared/map_adjacent.hpp"
#include "shared/rules.hpp"
#include "shared/overlay_view.hpp"
#include "shared/dir_type.hpp"
#include "shared/map_overlay.hpp"
#include "shared/tactical.hpp"
#include "shared/crt_memory.hpp"
#include "shared/always.hpp"
#include "shared/lcw_pipe.hpp"
#include "shared/pipe.hpp"
#include "shared/lzo_pipe.hpp"
#include "shared/map_isotile.hpp"
#include "shared/lcw_straw.hpp"
#include "shared/straw.hpp"
#include "shared/lzo_straw.hpp"
#include "shared/id_source.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/fast_math.hpp"
#include "shared/tileset_index.hpp"
#include "shared/abstract_class.hpp"
#include "shared/map_region.hpp"
#include "shared/unknown_map_generator.hpp"
#include "shared/tube.hpp"
#include "shared/cell_subzone.hpp"
#include "shared/coord_distance.hpp"
#include "shared/zone_table.hpp"
#include "shared/crt_stdio.hpp"


// The Map unit takes shared/coord.hpp's inline Cell-to-coordinate constructor
// (Place_Firestorm_Wall 0x0056A480 unlimbos at a cell through it).
// Header-static dynamic initializers of the unit at 0x00543FE0: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x00543FE0-0x005441C1); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x00544000, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_00543fe0.cpp; no source of its own.


// Dynamic initializer (0x00544030, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_00543fe0.cpp; no source of its own.


// Dynamic initializer (0x00544050, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_00543fe0.cpp; no source of its own.


// Dynamic initializer (0x00544070, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_00543fe0.cpp; no source of its own.


// Dynamic initializer (0x00544090, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_00543fe0.cpp; no source of its own.


// Dynamic initializer (0x005440B0, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_00543fe0.cpp; no source of its own.


// Dynamic initializer (0x005440D0, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_00543fe0.cpp; no source of its own.


// Dynamic initializer (0x00544110, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_00543fe0.cpp; no source of its own.


// Dynamic initializer (0x00544140, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_00543fe0.cpp; no source of its own.


// Dynamic initializer (0x00544170, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_00543fe0.cpp; no source of its own.


// Dynamic initializer (0x005441A0, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_00543fe0.cpp; no source of its own.


// Dynamic initializer (0x005441B0, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_00543fe0.cpp; no source of its own.


// Cell unknown_cells_a6f248[369] (0x00A6F248) and its dynamic initializer
// (0x005441D0, a jump to the construction at 0x005441E0): cell offsets in
// rings of growing radius around (0, 0) (read by MapClass 0x00549C50 and
// 0x0054A110, CellClass, HouseClass and BulletClass; a spread table).
// /Ob2 expands the first 288 Cell constructors and calls the kept copy
// (0x0042AD30) for the rest: the budget of the initializer is proportional
// to its size, and Cell(int, int) is a charged callee (see shared/coord.hpp).

Cell unknown_cells_a6f248[369] = {
    Cell(0, 0), Cell(1, -1), Cell(0, -1), Cell(-1, -1), Cell(-1, 0), Cell(1, 0),
    Cell(-1, 1), Cell(0, 1), Cell(1, 1), Cell(-1, -2), Cell(0, -2), Cell(1, -2),
    Cell(-2, -1), Cell(2, -1), Cell(-2, 0), Cell(2, 0), Cell(-2, 1), Cell(2, 1),
    Cell(-1, 2), Cell(0, 2), Cell(1, 2), Cell(-1, -3), Cell(0, -3), Cell(1, -3),
    Cell(-2, -2), Cell(2, -2), Cell(-3, -1), Cell(3, -1), Cell(-3, 0), Cell(3, 0),
    Cell(-3, 1), Cell(3, 1), Cell(-2, 2), Cell(2, 2), Cell(-1, 3), Cell(0, 3),
    Cell(1, 3), Cell(-1, -4), Cell(0, -4), Cell(1, -4), Cell(-3, -3), Cell(-2, -3),
    Cell(2, -3), Cell(3, -3), Cell(-3, -2), Cell(3, -2), Cell(-4, -1), Cell(4, -1),
    Cell(-4, 0), Cell(4, 0), Cell(-4, 1), Cell(4, 1), Cell(-3, 2), Cell(3, 2),
    Cell(-3, 3), Cell(-2, 3), Cell(2, 3), Cell(3, 3), Cell(-1, 4), Cell(0, 4),
    Cell(1, 4), Cell(-1, -5), Cell(0, -5), Cell(1, -5), Cell(-3, -4), Cell(-2, -4),
    Cell(2, -4), Cell(3, -4), Cell(-4, -3), Cell(4, -3), Cell(-4, -2), Cell(4, -2),
    Cell(-5, -1), Cell(5, -1), Cell(-5, 0), Cell(5, 0), Cell(-5, 1), Cell(5, 1),
    Cell(-4, 2), Cell(4, 2), Cell(-4, 3), Cell(4, 3), Cell(-3, 4), Cell(-2, 4),
    Cell(2, 4), Cell(3, 4), Cell(-1, 5), Cell(0, 5), Cell(1, 5), Cell(-1, -6),
    Cell(0, -6), Cell(1, -6), Cell(-3, -5), Cell(-2, -5), Cell(2, -5), Cell(3, -5),
    Cell(-4, -4), Cell(4, -4), Cell(-5, -3), Cell(5, -3), Cell(-5, -2), Cell(5, -2),
    Cell(-6, -1), Cell(6, -1), Cell(-6, 0), Cell(6, 0), Cell(-6, 1), Cell(6, 1),
    Cell(-5, 2), Cell(5, 2), Cell(-5, 3), Cell(5, 3), Cell(-4, 4), Cell(4, 4),
    Cell(-3, 5), Cell(-2, 5), Cell(2, 5), Cell(3, 5), Cell(-1, 6), Cell(0, 6),
    Cell(1, 6), Cell(-1, -7), Cell(0, -7), Cell(1, -7), Cell(-3, -6), Cell(-2, -6),
    Cell(2, -6), Cell(3, -6), Cell(-5, -5), Cell(-4, -5), Cell(4, -5), Cell(5, -5),
    Cell(-5, -4), Cell(5, -4), Cell(-6, -3), Cell(6, -3), Cell(-6, -2), Cell(6, -2),
    Cell(-7, -1), Cell(7, -1), Cell(-7, 0), Cell(7, 0), Cell(-7, 1), Cell(7, 1),
    Cell(-6, 2), Cell(6, 2), Cell(-6, 3), Cell(6, 3), Cell(-5, 4), Cell(5, 4),
    Cell(-5, 5), Cell(-4, 5), Cell(4, 5), Cell(5, 5), Cell(-3, 6), Cell(-2, 6),
    Cell(2, 6), Cell(3, 6), Cell(-1, 7), Cell(0, 7), Cell(1, 7), Cell(-1, -8),
    Cell(0, -8), Cell(1, -8), Cell(-3, -7), Cell(-2, -7), Cell(2, -7), Cell(3, -7),
    Cell(-5, -6), Cell(-4, -6), Cell(4, -6), Cell(5, -6), Cell(-6, -5), Cell(6, -5),
    Cell(-6, -4), Cell(6, -4), Cell(-7, -3), Cell(7, -3), Cell(-7, -2), Cell(7, -2),
    Cell(-8, -1), Cell(8, -1), Cell(-8, 0), Cell(8, 0), Cell(-8, 1), Cell(8, 1),
    Cell(-7, 2), Cell(7, 2), Cell(-7, 3), Cell(7, 3), Cell(-6, 4), Cell(6, 4),
    Cell(-6, 5), Cell(6, 5), Cell(-5, 6), Cell(-4, 6), Cell(4, 6), Cell(5, 6),
    Cell(-3, 7), Cell(-2, 7), Cell(2, 7), Cell(3, 7), Cell(-1, 8), Cell(0, 8),
    Cell(1, 8), Cell(-1, -9), Cell(0, -9), Cell(1, -9), Cell(-3, -8), Cell(-2, -8),
    Cell(2, -8), Cell(3, -8), Cell(-5, -7), Cell(-4, -7), Cell(4, -7), Cell(5, -7),
    Cell(-6, -6), Cell(6, -6), Cell(-7, -5), Cell(7, -5), Cell(-7, -4), Cell(7, -4),
    Cell(-8, -3), Cell(8, -3), Cell(-8, -2), Cell(8, -2), Cell(-9, -1), Cell(9, -1),
    Cell(-9, 0), Cell(9, 0), Cell(-9, 1), Cell(9, 1), Cell(-8, 2), Cell(8, 2),
    Cell(-8, 3), Cell(8, 3), Cell(-7, 4), Cell(7, 4), Cell(-7, 5), Cell(7, 5),
    Cell(-6, 6), Cell(6, 6), Cell(-5, 7), Cell(-4, 7), Cell(4, 7), Cell(5, 7),
    Cell(-3, 8), Cell(-2, 8), Cell(2, 8), Cell(3, 8), Cell(-1, 9), Cell(0, 9),
    Cell(1, 9), Cell(-1, -10), Cell(0, -10), Cell(1, -10), Cell(-3, -9), Cell(-2, -9),
    Cell(2, -9), Cell(3, -9), Cell(-5, -8), Cell(-4, -8), Cell(4, -8), Cell(5, -8),
    Cell(-7, -7), Cell(-6, -7), Cell(6, -7), Cell(7, -7), Cell(-7, -6), Cell(7, -6),
    Cell(-8, -5), Cell(8, -5), Cell(-8, -4), Cell(8, -4), Cell(-9, -3), Cell(9, -3),
    Cell(-9, -2), Cell(9, -2), Cell(-10, -1), Cell(10, -1), Cell(-10, 0), Cell(10, 0),
    Cell(-10, 1), Cell(10, 1), Cell(-9, 2), Cell(9, 2), Cell(-9, 3), Cell(9, 3),
    Cell(-8, 4), Cell(8, 4), Cell(-8, 5), Cell(8, 5), Cell(-7, 6), Cell(7, 6),
    Cell(-7, 7), Cell(-6, 7), Cell(6, 7), Cell(7, 7), Cell(-5, 8), Cell(-4, 8),
    Cell(4, 8), Cell(5, 8), Cell(-3, 9), Cell(-2, 9), Cell(2, 9), Cell(3, 9),
    Cell(-1, 10), Cell(0, 10), Cell(1, 10), Cell(0, 11), Cell(0, -11), Cell(-1, 11),
    Cell(1, 11), Cell(-1, -11), Cell(1, -11), Cell(-2, 11), Cell(2, 11), Cell(-2, -11),
    Cell(2, -11), Cell(-3, 11), Cell(3, 11), Cell(-3, -11), Cell(-3, 11), Cell(-4, 9),
    Cell(4, 9), Cell(-4, -9), Cell(4, -9), Cell(-5, 9), Cell(5, 9), Cell(-5, -9),
    Cell(5, -9), Cell(-6, 8), Cell(6, 8), Cell(-6, -8), Cell(6, -8), Cell(-7, 8),
    Cell(7, 8), Cell(-7, -8), Cell(7, -8), Cell(-8, 7), Cell(8, 7), Cell(-8, -7),
    Cell(8, -7), Cell(-8, 6), Cell(8, 6), Cell(-8, -6), Cell(8, -6), Cell(-9, 5),
    Cell(9, 5), Cell(-9, -5), Cell(9, -5), Cell(-9, 4), Cell(9, 4), Cell(-9, -4),
    Cell(9, -4), Cell(-10, 3), Cell(10, 3), Cell(-10, -3), Cell(10, -3), Cell(-10, 2),
    Cell(10, 2), Cell(-10, -2), Cell(10, -2), Cell(-11, 1), Cell(11, 1), Cell(-11, -1),
    Cell(11, -1), Cell(11, 0), Cell(-11, 0)
};


// Construction of unknown_cells_a6f248 (0x005441E0), the dynamic
// initializer that 0x005441D0 jumps to; emitted by the definition in
// MapClass_cells_a6f248_init_005441d0.cpp, no source of its own.


// Cell unknown_cells_a6ed18[309] (0x00A6ED18) and its dynamic initializer
// (0x00546190, a jump to the construction at 0x005461A0): the companion
// spread table (read by MapClass 0x00549C50 and 0x0054A110). The first 241
// constructors are expanded, the rest call 0x0042AD30 (same 78% budget).

Cell unknown_cells_a6ed18[309] = {
    Cell(0, 0), Cell(-1, 1), Cell(0, 1), Cell(1, 1), Cell(1, 0), Cell(-1, 0),
    Cell(1, -1), Cell(0, -1), Cell(-1, -1), Cell(1, 1), Cell(0, 1), Cell(-1, 1),
    Cell(1, 1), Cell(-1, 1), Cell(1, 0), Cell(-1, 0), Cell(1, -1), Cell(-1, -1),
    Cell(1, -1), Cell(0, -1), Cell(-1, -1), Cell(0, 1), Cell(0, 1), Cell(0, 1),
    Cell(1, 1), Cell(-1, 1), Cell(1, 0), Cell(-1, 0), Cell(1, 0), Cell(-1, 0),
    Cell(1, 0), Cell(-1, 0), Cell(1, -1), Cell(-1, -1), Cell(0, -1), Cell(0, -1),
    Cell(0, -1), Cell(0, 1), Cell(0, 1), Cell(0, 1), Cell(1, 1), Cell(1, 1),
    Cell(-1, 1), Cell(-1, 1), Cell(1, 1), Cell(-1, 1), Cell(1, 0), Cell(-1, 0),
    Cell(1, 0), Cell(-1, 0), Cell(1, 0), Cell(-1, 0), Cell(1, -1), Cell(-1, -1),
    Cell(1, -1), Cell(1, -1), Cell(-1, -1), Cell(-1, -1), Cell(0, -1), Cell(0, -1),
    Cell(0, -1), Cell(0, 1), Cell(0, 1), Cell(0, 1), Cell(1, 1), Cell(1, 1),
    Cell(-1, 1), Cell(-1, 1), Cell(1, 1), Cell(-1, 1), Cell(1, 1), Cell(-1, 1),
    Cell(1, 0), Cell(-1, 0), Cell(1, 0), Cell(-1, 0), Cell(1, 0), Cell(-1, 0),
    Cell(1, -1), Cell(-1, -1), Cell(1, -1), Cell(-1, -1), Cell(1, -1), Cell(1, -1),
    Cell(-1, -1), Cell(-1, -1), Cell(0, -1), Cell(0, -1), Cell(0, -1), Cell(0, 1),
    Cell(0, 1), Cell(0, 1), Cell(1, 1), Cell(0, 1), Cell(0, 1), Cell(-1, 1),
    Cell(1, 1), Cell(-1, 1), Cell(1, 1), Cell(-1, 1), Cell(1, 0), Cell(1, 0),
    Cell(1, 0), Cell(-1, 0), Cell(1, 0), Cell(-1, 0), Cell(1, 0), Cell(-1, 0),
    Cell(1, 0), Cell(-1, 0), Cell(1, -1), Cell(-1, -1), Cell(1, -1), Cell(-1, -1),
    Cell(1, -1), Cell(0, -1), Cell(0, -1), Cell(-1, -1), Cell(0, -1), Cell(0, -1),
    Cell(0, -1), Cell(0, 1), Cell(0, 1), Cell(0, 1), Cell(1, 1), Cell(0, 1),
    Cell(0, 1), Cell(-1, 1), Cell(1, 1), Cell(1, 1), Cell(-1, 1), Cell(-1, 1),
    Cell(1, 1), Cell(-1, 1), Cell(1, 1), Cell(-1, 1), Cell(1, 0), Cell(-1, 0),
    Cell(1, 0), Cell(-1, 0), Cell(1, 0), Cell(-1, 0), Cell(1, 0), Cell(-1, 0),
    Cell(1, 0), Cell(-1, 0), Cell(1, -1), Cell(-1, -1), Cell(1, -1), Cell(-1, -1),
    Cell(1, -1), Cell(1, -1), Cell(-1, -1), Cell(-1, -1), Cell(1, -1), Cell(0, -1),
    Cell(0, -1), Cell(-1, -1), Cell(0, -1), Cell(0, -1), Cell(0, -1), Cell(0, 1),
    Cell(0, 1), Cell(0, 1), Cell(0, 1), Cell(0, 1), Cell(0, 1), Cell(0, 1),
    Cell(1, 1), Cell(1, 1), Cell(-1, 1), Cell(-1, 1), Cell(1, 1), Cell(-1, 1),
    Cell(1, 1), Cell(-1, 1), Cell(1, 0), Cell(-1, 0), Cell(1, 0), Cell(-1, 0),
    Cell(1, 0), Cell(-1, 0), Cell(1, 0), Cell(-1, 0), Cell(1, 0), Cell(-1, 0),
    Cell(1, 0), Cell(-1, 0), Cell(1, 0), Cell(-1, 0), Cell(1, -1), Cell(-1, -1),
    Cell(1, -1), Cell(-1, -1), Cell(1, -1), Cell(1, -1), Cell(-1, -1), Cell(-1, -1),
    Cell(0, -1), Cell(0, -1), Cell(0, -1), Cell(0, -1), Cell(0, -1), Cell(0, -1),
    Cell(0, -1), Cell(0, 1), Cell(0, 1), Cell(0, 1), Cell(0, 1), Cell(0, 1),
    Cell(0, 1), Cell(0, 1), Cell(1, 1), Cell(1, 1), Cell(-1, 1), Cell(-1, 1),
    Cell(1, 1), Cell(-1, 1), Cell(1, 1), Cell(-1, 1), Cell(1, 1), Cell(-1, 1),
    Cell(1, 0), Cell(-1, 0), Cell(1, 0), Cell(-1, 0), Cell(1, 0), Cell(-1, 0),
    Cell(1, 0), Cell(-1, 0), Cell(1, 0), Cell(-1, 0), Cell(1, 0), Cell(-1, 0),
    Cell(1, 0), Cell(-1, 0), Cell(1, -1), Cell(-1, -1), Cell(1, -1), Cell(-1, -1),
    Cell(1, -1), Cell(-1, -1), Cell(1, -1), Cell(1, -1), Cell(-1, -1), Cell(-1, -1),
    Cell(0, -1), Cell(0, -1), Cell(0, -1), Cell(0, -1), Cell(0, -1), Cell(0, -1),
    Cell(0, -1), Cell(0, 1), Cell(0, 1), Cell(0, 1), Cell(0, 1), Cell(0, 1),
    Cell(0, 1), Cell(0, 1), Cell(1, 1), Cell(1, 1), Cell(-1, 1), Cell(-1, 1),
    Cell(1, 1), Cell(1, 1), Cell(-1, 1), Cell(-1, 1), Cell(1, 1), Cell(-1, 1),
    Cell(1, 1), Cell(-1, 1), Cell(1, 1), Cell(-1, 1), Cell(1, 0), Cell(-1, 0),
    Cell(1, 0), Cell(-1, 0), Cell(1, 0), Cell(-1, 0), Cell(1, 0), Cell(-1, 0),
    Cell(1, 0), Cell(-1, 0), Cell(1, 0), Cell(-1, 0), Cell(1, 0), Cell(-1, 0),
    Cell(1, -1), Cell(-1, -1), Cell(1, -1), Cell(-1, -1), Cell(1, -1), Cell(-1, -1),
    Cell(1, -1), Cell(1, -1), Cell(-1, -1), Cell(-1, -1), Cell(1, -1), Cell(1, -1),
    Cell(-1, -1), Cell(-1, -1), Cell(0, -1), Cell(0, -1), Cell(0, -1), Cell(0, -1),
    Cell(0, -1), Cell(0, -1), Cell(0, -1)
};


// Construction of unknown_cells_a6ed18 (0x005461A0), the dynamic
// initializer that 0x00546190 jumps to; emitted by the definition in
// MapClass_cells_a6ed18_init_00546190.cpp, no source of its own.


// Dynamic initializer of the dummy cell (0x00547930, _$E): constructs
// unknown_dummy_cell_a6f9f8 and registers its atexit destructor
// (0x00547950, the following _$E draft, emitted by this definition).

CellClass unknown_dummy_cell_a6f9f8;


// atexit destructor of the dummy cell (0x00547950, _$E): emitted by the
// definition in MapClass_dummy_cell_init_00547930.cpp; no source of its own.


// MapClass::MapClass() (0x00547960)
// Constructs the base screen and the member vectors (the zone, subzone and
// first cell-list vectors through their out-of-line constructors), clears
// the entry cells and sets +0x144 to 13.

MapClass::MapClass()
{
    unknown_144 = 13;
}


// MapClass::MapClass(NoInitClass const &noinit) (0x00547A60)
// Hypothesis: the restore-in-place constructor. The base screen keeps its
// state (vtable only) but the members are rebuilt empty and the hash-table
// pointers and per-frame state cleared.

MapClass::MapClass(NoInitClass const &noinit) : GScreenClass(noinit), unknown_10(false), unknown_14(0), Cells(noinit)
{
    for (int clear = 0; clear < 12; clear++) {
        unknown_18[clear] = 0;
    }
    memset(unknown_7c, 0, sizeof(unknown_7c));
    for (int index = 0; index < 3; index++) {
        SubzoneTracking[index].Clear();
        unknown_7c[index] = 0;
    }
    unknown_14 = 0;
}


// MapClass::~MapClass() (0x00547B90)
// Runs four unidentified global cleanups, frees the owned buffers and hash
// tables, then the member vectors are destroyed.

MapClass::~MapClass()
{
    Unknown_00710B50();
    Unknown_00711C70();
    Unknown_006E9670();
    Unknown_006EA130();
    if (CellZones != 0) {
        delete CellZones;
        CellZones = 0;
    }
    if (unknown_14 != 0) {
        delete unknown_14;
        unknown_14 = 0;
    }
    if (CellSubzones != 0) {
        delete CellSubzones;
        CellSubzones = 0;
    }
    for (int index = 0; index < 3; index++) {
        if (unknown_7c[index] != 0) {
            delete unknown_7c[index];
            unknown_7c[index] = 0;
        }
    }
}


// Point2D MapClass::Unknown_00547D40(Point2D const &point) (0x00547D40)
// Isometric transform: X gains half of Y (rounded up), Y becomes half of Y
// plus the map width less X. No direct callers (inlined everywhere).

Point2D MapClass::Unknown_00547D40(Point2D const &point)
{
    return Point2D(((point.Y + 1) >> 1) + point.X, (point.Y >> 1) + MapSize.Width - point.X);
}


// Point2D MapClass::Unknown_00547D70(Point2D const &point) (0x00547D70)
// Unknown_00547D40 of point moved by MapLocalSize's origin.

Point2D MapClass::Unknown_00547D70(Point2D const &point)
{
    Point2D offset(MapLocalSize.X, MapLocalSize.Y);
    return Unknown_00547D40(Point2D(point.X + offset.X, point.Y + offset.Y));
}


// Point2D MapClass::Unknown_00547DB0(Point2D const &point) (0x00547DB0)
// Inverse of Unknown_00547D40. No direct callers (inlined everywhere).

Point2D MapClass::Unknown_00547DB0(Point2D const &point)
{
    int width = MapSize.Width;
    int x = (width & 1) ? point.X - point.Y + 1 : point.X - point.Y;
    return Point2D((x >> 1) + width / 2, point.Y - width + point.X);
}


// Point2D MapClass::Unknown_00547DF0(Point2D const &point) (0x00547DF0)
// Unknown_00547DB0 relative to MapLocalSize's origin.

Point2D MapClass::Unknown_00547DF0(Point2D const &point)
{
    Point2D offset(MapLocalSize.X, MapLocalSize.Y);
    Point2D result = Unknown_00547DB0(point);
    return Point2D(result.X - offset.X, result.Y - offset.Y);
}


// Cell MapClass::Unknown_00547E50(Cell const &cell) (0x00547E50)
// Unknown_00547D40 for cells. No direct callers (inlined everywhere).

Cell MapClass::Unknown_00547E50(Cell const &cell)
{
    Point2D point = Unknown_00547D40(Point2D(cell.X, cell.Y));
    return Cell(point.X, point.Y);
}


// Cell MapClass::Unknown_00547E90(Cell const &cell) (0x00547E90)
// Unknown_00547D70 for cells. No direct callers (inlined everywhere).

Cell MapClass::Unknown_00547E90(Cell const &cell)
{
    Point2D point = Unknown_00547D70(Point2D(cell.X, cell.Y));
    return Cell(point.X, point.Y);
}


// Cell MapClass::Unknown_00547EE0(Cell const &cell) (0x00547EE0)
// Unknown_00547DB0 for cells. No direct callers (inlined everywhere).

Cell MapClass::Unknown_00547EE0(Cell const &cell)
{
    Point2D point = Unknown_00547DB0(Point2D(cell.X, cell.Y));
    return Cell(point.X, point.Y);
}


// Cell MapClass::Unknown_00547F30(Cell const &cell) (0x00547F30)
// Unknown_00547DF0 for cells.

Cell MapClass::Unknown_00547F30(Cell const &cell)
{
    Point2D point = Unknown_00547DF0(Point2D(cell.X, cell.Y));
    return Cell(point.X, point.Y);
}


// long __stdcall MapClass::Is_Visible(Cell cell) (0x00547FA0, IGameMap slot 3)
// Bit 3 of the cell's flags (the dummy cell's when off the map).

long __stdcall MapClass::Is_Visible(Cell cell)
{
    return (*this)[Cell(cell.X, cell.Y)].IsMapped;
}


// CellClass &MapClass::operator[](CoordStruct const &coord) (0x00548000)
// The cell under a lepton coordinate; the dummy cell (with the requested
// coordinates) when the index is outside the cell vector or the slot is empty.

CellClass &MapClass::operator[](CoordStruct const &coord)
{
    int x = coord.X / 256;
    int y = coord.Y / 256;
    int index = (y << 9) + x;
    if (index >= 0 && index < Cells.Length() && Cells[index] != 0) {
        return *Cells[index];
    }
    unknown_dummy_cell_a6f9f8.MapCoords = Cell(x, y);
    return unknown_dummy_cell_a6f9f8;
}


// CellClass &MapClass::operator[](Cell const &cell) (0x00548070)
// The cell at cell; the dummy cell (with the requested coordinates) outside
// the 512 x 512 index range or for an empty slot.

CellClass &MapClass::operator[](Cell const &cell)
{
    int index = (cell.Y << 9) + cell.X;
    if (index >= 0 && index < 0x40000 && Cells[index] != 0) {
        return *Cells[index];
    }
    unknown_dummy_cell_a6f9f8.MapCoords = cell;
    return unknown_dummy_cell_a6f9f8;
}


// bool MapClass::Has_Cell(Cell const &cell) (0x005480B0)
// Whether the cell vector holds a cell at cell (no range check).

bool MapClass::Has_Cell(Cell const &cell)
{
    return Cells[(cell.Y << 9) + cell.X] ? true : false;
}


// void MapClass::One_Time() (0x005480D0, slot 5)
// Sets the 512 x 512 cell dimensions, builds the cell vector (slot 22) and
// the hash tables (256 buckets, growth step 20) the zone code uses.

void MapClass::One_Time()
{
    GScreenClass::One_Time();
    MapCellWidth = 512;
    MapCellHeight = 512;
    MapCellTotal = 0x40000;
    unknown_slot22();
    if (unknown_14 == 0) {
        unknown_14 = new HashTableClass<unsigned int, unsigned int>(20, 256, Unknown_Hash_0054F240);
    }
    for (int index = 0; index < 3; index++) {
        SubzoneTracking[index].Clear();
        unknown_7c[index] = new HashTableClass<unsigned int, SubzoneConnectionStruct>(20, 256, Unknown_Hash_0054F240);
    }
    for (int clear = 0; clear < 12; clear++) {
        unknown_18[clear] = 0;
    }
}


// void MapClass::Init_Clear() (0x005482C0, slot 7)
// Resets the map between scenarios: the two unidentified resets around the
// base screen's Init_Clear, every +0x154 entry's cell and stopped countdown,
// +0x144 back to 13 and the +0x1154 counter. Its trace strings name it.

void MapClass::Init_Clear()
{
    Debug_Printf("MapClass::Init_Clear entry\n");
    unknown_481fb0();
    unknown_839c6c->Unknown_00776160(0xFFFF);
    unknown_8315b4->Unknown_00411050(0x7F);
    unknown_481fb0();
    GScreenClass::Init_Clear();
    unknown_481fb0();
    for (int index = 0; index < 256; index++) {
        UnknownMapEntry154 &entry = unknown_154[index];
        entry.Cell = unknown_cell_a6f238;
        entry.Timer.Stop();
    }
    unknown_144 = 13;
    unknown_1154 = false;
    Debug_Printf("MapClass::Init_Clear done\n");
}


// void MapClass::unknown_slot22() (0x00548370, slot 22)
// Rebuilds the cell vector in place with MapCellTotal empty slots.

void MapClass::unknown_slot22()
{
    new (&Cells) VectorClass<CellClass *>;
    Cells.Resize(MapCellTotal);
    for (int index = 0; index < MapCellTotal; index++) {
        Cells[index] = 0;
    }
}


// void MapClass::unknown_slot23() (0x005483D0, slot 23)
// Clears two members of every cell, deletes all cells, runs 0x00529710 and
// makes sure the cell vector holds 512 x 512 empty slots.

void MapClass::unknown_slot23()
{
    int index;
    for (index = 0; index < Cells.Length(); index++) {
        CellClass *cell = Cells[index];
        if (cell != 0) {
            cell->unknown_2c = 0;
            cell->unknown_30 = 0;
        }
    }
    for (index = 0; index < Cells.Length(); index++) {
        if (Cells[index] != 0) {
            delete Cells[index];
            Cells[index] = 0;
        }
    }
    Unknown_00529710(0, true);
    if (Cells.Length() < 0x40000) {
        Cells.Resize(0x40000);
        for (index = 0; index < Cells.Length(); index++) {
            Cells[index] = 0;
        }
    }
}


// void MapClass::unknown_slot24() (0x00548490, slot 24)
// Re-constructs every existing cell in place, row by row.

void MapClass::unknown_slot24()
{
    unknown_130 = 0;
    for (int y = 0; y < 512; y++) {
        for (int x = 0; x < 512; x++) {
            int index = y * 512 + x;
            if (index < Cells.Length() && Cells[index] != 0) {
                new (Cells[index]) CellClass;
            }
        }
    }
}


// Not exact (RA2_NON_MATCHING): MapClass::Set_Map_Dimensions at 0x005484E0, best score 87.33, 2026-11-13; differs: 1404 instructions in the original, 1429 in the draft (shape 96.93)
#ifdef RA2_NON_MATCHING
// NON-EXACT DRAFT 0x5484E0 MapClass::Set_Map_Dimensions (Set_Map_Dimensions)
// entry 0x5484E0; exact-match score 94.60 (Scout1013 best; loose 98.86); plan "draft, verify"
// origin lane: Scout1013 (build/class-work/Scout1013/keep/smd_9460.cpp; from Scout1002 w3/smd0.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1089 (native/drafts.json). Not original bytes.
// Native adaptation (Scout1089): the draft was scored with Scout1013's private overlay
// (RA2_MAP_SET_DIMENSIONS stand-in members in map_techno.hpp, RA2_CELL_ADD_IN_PLACE
// Cell::operator+= in coord.hpp). Natively the Map unit's stand-in ObjectClass/AnimClass
// do not lay out like the real classes (pointer widths), so the members the overlay
// added are read through native helpers compiled against the real headers
// (native/link/behaviour_map_dimensions.cpp), and the tube cells use Cell + Cell
// (same value as the base operator+= 0x0056AA20). Each changed line is marked NATIVE.
// void MapClass::Set_Map_Dimensions (0x005484E0): TibSun Set_Map_Dimensions.
#include "shared/object_lists.hpp"
// NATIVE: shared/main_loop.hpp left out (its min() repeats always.hpp's without the overlay's guard; nothing used)
#include "shared/unknown_vector3.hpp"

// NATIVE: real-layout accessors (native/link/behaviour_map_dimensions.cpp).
CoordStruct ra2native_object_coord_5484e0(ObjectClass const *object);
Cell ra2native_object_cell_5484e0(ObjectClass const *object);
bool ra2native_anim_attached_5484e0(ObjectClass const *anim);
bool ra2native_is_gas_system_5484e0(ObjectClass const *object);

extern int unknown_833968;
extern int unknown_int_8325d0;   // NATIVE: declared here (shared/main_loop.hpp, left out above)
extern int unknown_int_8325d4;   // NATIVE

void MapClass::Set_Map_Dimensions(Rect const *rect, bool reset_cells, int cell_height, bool refresh_map)
{
    int i = 0;
    int w = MapSize.Width;
    CellClass *cells_base = 0;
    CellClass *cells = 0;
    int h = 2 * unknown_int_8325d4 + 8;
    CellClass *cptr;
    Cell cell(unknown_cell_a6f238);

    for (int j = 0; j < unknown_objects_a40368.Count(); j++) {
        unknown_objects_a40368[j]->Mark(0);
    }

    for (; i < Cells.Length(); i++) {
        if (!In_Radar(Cell(i % 512, i / 512))) {
            CellClass *c = Cells[i];
            if (c != 0) {
                c->unknown_11b = cell_height;
            }
        }
    }

    int cellcount;

    if (!reset_cells) {
        cellcount = 0;
        cells_base = (CellClass *) new char[h * w * sizeof(CellClass)];
        cells = cells_base;
        Reset_Iterator();
        cptr = Iterate();
        cell = cptr->Fetch_CellID();
        while (cptr != 0) {
            cells_base[cellcount] = *cptr;
            cells_base[cellcount].unknown_2c = 0;
            cells_base[cellcount].unknown_30 = 0;
            cptr = Iterate();
            cellcount++;
        }
    }

    MapSize = *rect;
    MapSize.X = 0;
    MapSize.Y = 0;

    if (MapSize.Width > 63 || MapSize.Height > 63) {
        unknown_833968 = 2;
    }

    unknown_map_min_a70a1c = unknown_int_8325d0;
    unknown_map_max_a70a20 = unknown_int_8325d0 + 2 * unknown_int_8325d4;

    int y = 0;
    unknown_120 = 1;
    unknown_124 = 1;
    int msize = rect->Height + rect->Width - 1;
    unknown_128 = msize;
    unknown_12c = msize;

    for (; y < 2 * unknown_12c + 2; y++) {
        for (int x = 0; x < unknown_128 + 2; x++) {
            Cell cell(x, y);
            if (In_Radar(cell)) {
                int idx = (512 * y) + x;
                CellClass *c = Cells[idx];
                if (c == 0) {
                    CellClass *cc = new CellClass;
                    cc->Set_CellID(cell);
                    Cells[idx] = cc;
                } else {
                    c = new(c) CellClass;
                    c->Set_CellID(cell);
                }
                Cells[idx]->unknown_11b = cell_height;
            }
        }
    }

    bool freshcalled = false;

    if (!reset_cells) {
        Reset_Iterator();
        Cell icell = Iterate()->Fetch_CellID();
        Cell cellpos = Cell(rect->Y + rect->X, rect->Y - rect->X);
        Cell offset = (icell - cell) - cellpos;

        for (i = 0; i < cellcount; i++) {
            CellClass *src = &cells[i];
            Cell scell = src->MapCoords;
            Cell shifted = scell + offset;

            CellClass *c = &(*this)[shifted];

            Cell dcell = c->Fetch_CellID();
            if (c != 0) {
                *c = *src;
                c->Set_CellID(dcell);
            }
        }

        Reset_Iterator();
        cptr = Iterate();
        while (cptr != 0) {

            Cell c = cptr->Fetch_CellID();

            if (c.X + c.Y < MapSize.Width - 2 * rect->Y + 1 ||
                c.Y - c.X > MapSize.Width + 2 * rect->X - 1 ||
                c.X + c.Y > MapSize.Width + 2 * (h + MapSize.Height - rect->Y - unknown_int_8325d4) ||
                c.X - c.Y > MapSize.Width + 2 * (w - rect->X - unknown_int_8325d0) - 1) {

                int cellindex = c.X + c.Y * 512;

                delete Cells[cellindex];
                Cells[cellindex] = new CellClass;
                Cells[cellindex]->Set_CellID(c);
                Cells[cellindex]->IsoTileIndex = 0xFFFF;
                Cells[cellindex]->unknown_11a = 0;
                Cells[cellindex]->unknown_11c = 0;
                Cells[cellindex]->OverlayIndex = -1;
                Cells[cellindex]->unknown_11b = cell_height;
            }
            cptr = Iterate();
        }

        for (i = 0; i < 101; i++) {
            if (unknown_id_source_a3d290->Unknown_00663240(i)) {
                unknown_id_source_a3d290->Set_Waypoint_Location(i, unknown_id_source_a3d290->Get_Waypoint_Location(i) + offset);
            }
        }

        for (i = 0; i < unknown_TubeClass_list_008661c8.Count(); i++) {
            TubeClass *tube = unknown_TubeClass_list_008661c8[i];
            tube->EnterCell = tube->EnterCell + offset;   // NATIVE: overlay Cell::operator+= (base 0x0056AA20)
            tube->ExitCell = tube->ExitCell + offset;     // NATIVE
        }

        CoordStruct coord = CoordStruct(offset, 0) - CoordStruct(128, 128, 0);

        for (i = 0; i < unknown_objects_a40368.Count(); i++) {
            ObjectClass *obj = unknown_objects_a40368[i];
            if (obj->Kind_Of() != 4) {
                obj->Set_Coord(ra2native_object_coord_5484e0(obj) + coord);   // NATIVE: Coord +0x88
            }
        }

        for (i = 0; i < Anims.Count(); i++) {
            AnimClass *anim = Anims[i];
            if (!ra2native_anim_attached_5484e0(anim)) {   // NATIVE: AnimClass +0xB4
                anim->Set_Coord(ra2native_object_coord_5484e0(anim) + coord);   // NATIVE
            }
        }

        unknown_a40774++;

        for (i = 0; i < unknown_objects_a40368.Count(); i++) {
            if (unknown_objects_a40368[i]->Kind_Of() != 0x18 || !ra2native_is_gas_system_5484e0(unknown_objects_a40368[i])) {   // NATIVE: Class +0x98, BehavesLike +0x284 == 1
                Cell cell = ra2native_object_cell_5484e0(unknown_objects_a40368[i]);   // NATIVE: slot 104
                Cell const *list = unknown_objects_a40368[i]->Occupy_List(0);
                while (*list != Cell(0x7FFF, 0x7FFF)) {
                    Cell ncell = cell + *list;
                    if (!In_Radar(ncell)) {
                        unknown_objects_a40368[i]->Limbo();
                        delete unknown_objects_a40368[i];
                        i--;
                        break;
                    }
                    list++;
                }
            }
        }

        unknown_a40774--;

        for (i = 0; i < Cells.Length(); i++) {
            CellClass *c1 = Cells[i];
            CellClass **c2 = &Cells[i];
            if (c1 != 0) {
                if (!In_Radar(Cell(i % 512, i / 512))) {
                    *c2 = 0;
                    delete c1;
                }
            }
        }

        Reset_Iterator();
        cptr = Iterate();
        while (cptr != 0) {
            if (cptr->IsBridgeDeck && cptr->unknown_2c == 0) {
                unsigned facing = !cptr->IsBridgeEastWest * 2;
                if (cptr->OverlayIndex == 0x18 || cptr->OverlayIndex == 0x19) {
                    cptr->Set_Under_Bridge(facing, true);
                } else {
                    cptr->Set_Under_Rail_Bridge(facing, true);
                }
            }
            cptr = Iterate();
        }

        if (refresh_map) {
            Fresh_Map();
            freshcalled = true;
        }

        for (i = 0; i < unknown_objects_a40368.Count(); i++) {
            unknown_objects_a40368[i]->Mark(1);
        }

        Map.Flag_To_Redraw(2);
    }

    if (!freshcalled && refresh_map) {
        Fresh_Map();
    }

    new(&unknown_dummy_cell_a6f9f8) CellClass;
    delete (void *)cells_base;
}
#endif

// void MapClass::Fresh_Map() (0x005499C0)
// Rebuilds the zone data for the map size: frees and reallocates the +0x64
// elements ((width + height + 1) squared, +0x68) and the +0x6C buffer (ten
// bytes per element), sizes the path estimator for the map rectangle, sets
// the three subzone tracking vectors' growth steps (width * height * 4 over
// 4, 16 and 64), runs 0x0054B2E0, 0x0054FDA0 and 0x0054EBD0 and rebuilds the
// subzone tracking (0x00563D40).

void MapClass::Fresh_Map()
{
    if (CellZones != 0) {
        delete CellZones;
        CellZones = 0;
    }
    if (CellSubzones != 0) {
        delete CellSubzones;
        CellSubzones = 0;
    }
    int size = MapSize.Width + MapSize.Height + 1;
    CellZoneCount = size * size;
    CellZones = new UnknownMapElem4[CellZoneCount];
    CellSubzones = new char[CellZoneCount * 10];
    Search.Update_Map_Dimensions(MapSize);
    for (int index = 0; index < 3; index++) {
        int scale = 1 << (index + 1);
        SubzoneTracking[index].Set_Growth_Step(MapSize.Width * MapSize.Height * 4 / (scale * scale));
    }
    Overpass();
    Compute_Zone_Connections();
    Zone_Reset();
    Reset_All_Subzones();
}


// void MapClass::Set_Local_Dimensions(Rect const &rect) (0x00549AE0, slot 29)
// Sets the local map rectangle (+0xF8) to the part of rect inside MapSize,
// at least 2 in from the left and top and 2 / 6 cells clear of the right
// and bottom edges, flags a full redraw, and refreshes every techno's +0x32D
// cell test (0x0055A660); a techno that just passed it, whose house answers
// 0x004F52B0, of a kind other than 6, with +0x7D set and +0x76 clear, gets
// its slot 70 (false, false).

void MapClass::Set_Local_Dimensions(Rect const &rect)
{
    MapLocalSize = Intersect(rect, MapSize);
    MapLocalSize.X = MapLocalSize.X > 2 ? MapLocalSize.X : 2;
    MapLocalSize.Y = MapLocalSize.Y > 2 ? MapLocalSize.Y : 2;
    MapLocalSize.Width = MapLocalSize.Width < MapSize.Width - MapLocalSize.X - 2 ? MapLocalSize.Width : MapSize.Width - MapLocalSize.X - 2;
    MapLocalSize.Height = MapLocalSize.Height < MapSize.Height - MapLocalSize.Y - 6 ? MapLocalSize.Height : MapSize.Height - MapLocalSize.Y - 6;
    Map.Flag_To_Redraw(2);
    for (int index = 0; index < unknown_list_a40c20.Count(); index++) {
        TechnoClass *techno = unknown_list_a40c20[index];
        bool was = techno->unknown_32d;
        techno->unknown_32d = Unknown_0055A660(techno->unknown_slot104(), true);
        if (!was && techno->unknown_32d && techno->House->Is_Player_Control()
            && techno->Kind_Of() != 6 && techno->unknown_7d && !techno->unknown_76) {
            techno->Look(false, false);
        }
    }
}


// void MapClass::Unknown_00549C20(int arg1, int arg2, CoordStruct const &coord, int sight, HouseClass *house, bool arg6, bool arg7, bool arg8, bool arg9) (0x00549C20)
// Forwards to 0x00549C50 with true for its last argument; the first two
// arguments are not read (no callers or pointers in the executable).

void MapClass::Unknown_00549C20(int arg1, int arg2, CoordStruct const &coord, int sight, HouseClass *house, bool arg6, bool arg7, bool arg8, bool arg9)
{
    Unknown_00549C50(coord, sight, house, arg6, arg7, arg8, arg9, true);
}


// void MapClass::Unknown_00549C50(CoordStruct const &xcoord, int sightrange, HouseClass *house,
//     bool incremental, bool dont_map, bool unfog, bool byheight, bool arg8) (0x00549C50)
// Sight from a coordinate (TibSun Sight_From): the coordinate moved up the
// screen by its height, its cell on the map (0x0054AAB0) and a sight of 1-10;
// the radius offsets (0x00A6F248, ring counts 0x007A5908; with incremental
// and no reveal by height only the outer three rings) and their occlusion
// offsets (0x00A6ED18). The house is the player's when the player spies its
// radar or, with AllyReveal, is its ally; other houses do not map. Each cell
// within the sight (and, by height, not more than three levels below the
// occluding cell) loses +0x140 bit 6; unfog fogs mapped cells (Display slot
// 38), else unmapped cells are mapped unless dont_map (Radar slot 37 with
// arg8, else 0x0047C900). Finally 0x0054A550 with the sight + 3.
// The clamp tests 11 and then 10 (two compares in the original; MIN(10, MIN(sight, 11))
// compiles the same).

// The sight scan tables (TibSun MapClass::RadiusCount, RadiusOffset, OcclusionOffset).
extern int const unknown_radius_count_7a5908[];
extern Cell const unknown_radius_offset_a6f248[];
extern Cell const unknown_occlusion_offset_a6ed18[];

void MapClass::Unknown_00549C50(CoordStruct const &xcoord, int sightrange, HouseClass *house, bool incremental, bool dont_map, bool unfog, bool byheight, bool arg8)
{
    int xx;
    Cell const *ptr;
    Cell const *ptr2;
    int count;

    int cell_height = xcoord.Z / unknown_geometry_g;

    CoordStruct coord = xcoord;
    coord.X = coord.X + (Unknown_006A2970(coord.Z) / -30) * 256;
    coord.Y = coord.Y + (Unknown_006A2970(coord.Z) / -30) * 256;

    Cell cell = Coord_Cell(coord);
    Cell hoffset = cell - Coord_Cell(xcoord) - Cell(2, 2);

    if (!In_Radar(cell)) return;
    if (!sightrange) return;
    if (sightrange >= 11 || sightrange > 10) sightrange = 10;

    xx = cell.X;

    count = unknown_radius_count_7a5908[sightrange];
    ptr = &unknown_radius_offset_a6f248[0];
    ptr2 = &unknown_occlusion_offset_a6ed18[0];
    if (!Rules->RevealByHeight && incremental) {
        if (sightrange > 2) {
            ptr += unknown_radius_count_7a5908[sightrange - 3];
            ptr2 += unknown_radius_count_7a5908[sightrange - 3];
            count -= unknown_radius_count_7a5908[sightrange - 3];
        }
    }
    ptr2--;

    if (house != 0 && unknown_player_a35db4 != 0) {
        if (house != unknown_player_a35db4) {
            if ((house->unknown_5414 & (1 << unknown_player_a35db4->Type->House)) != 0) {
                house = unknown_player_a35db4;
            }
        }
        if (house != unknown_player_a35db4) {
            if (house->Is_Ally(unknown_player_a35db4) && Rules->unknown_142b) {
                house = unknown_player_a35db4;
            }
        }
    }

    if (house != unknown_player_a35db4) {
        return;
    }

    while (count--) {
        Cell newcell;
        int xdiff;

        newcell = cell + *ptr++;
        ptr2++;

        if (!In_Radar(newcell)) continue;
        xdiff = newcell.X - xx;
        xdiff = abs(xdiff);
        if (xdiff > sightrange) continue;
        Cell delta = newcell - cell;
        if ((short)unknown_sqrt_4ba410((double)delta.X * delta.X + (double)delta.Y * delta.Y) > sightrange) continue;

        bool ok = false;
        CellClass *cellptr = &(*this)[newcell];
        if (byheight && Rules->RevealByHeight) {
            if (Map[newcell - hoffset + *ptr2].unknown_11b <= cell_height + 3) {
                ok = true;
            }
        } else {
            ok = true;
        }

        if (ok) {
            cellptr->IsToFog = false;
            if (unfog) {
                if ((!cellptr->IsFogVisible || !cellptr->IsFogMapped) && cellptr->IsMapped) {
                    Map.unknown_slot38(newcell, house);
                }
            } else {
                if ((!cellptr->IsMapped || !cellptr->IsVisible || !cellptr->IsFogVisible || !cellptr->IsFogMapped) && !dont_map) {
                    if (arg8) {
                        Map.RevealFogShroud(newcell, house, false);
                    } else {
                        cellptr->unknown_47c900();
                    }
                }
            }
        }
    }

    Unknown_0054A550(coord, 0, sightrange + 3, 0);
}


// Sight scan0054A110, complete1088+0. Shares the owned accepted549C50 scan layout.
// unknown_arg5 is not read; original flags unfog and arg8 have byte-valued bool ABI.

// The sight scan tables (TibSun MapClass::RadiusCount, RadiusOffset, OcclusionOffset).
extern int const unknown_radius_count_7a5908[];
extern Cell const unknown_radius_offset_a6f248[];
extern Cell const unknown_occlusion_offset_a6ed18[];

void MapClass::Sight_From(CoordStruct const &xcoord, int sightrange, HouseClass *house, bool incremental, bool unknown_arg5, bool unfog, bool byheight, bool arg8)
{
    int xx;
    Cell const *ptr;
    Cell const *ptr2;
    int count;

    int cell_height = xcoord.Z / unknown_geometry_g;

    CoordStruct coord = xcoord;
    coord.X = coord.X + (Unknown_006A2970(coord.Z) / -30) * 256;
    coord.Y = coord.Y + (Unknown_006A2970(coord.Z) / -30) * 256;

    Cell cell = Coord_Cell(coord);
    Cell hoffset = cell - Coord_Cell(xcoord) - Cell(2, 2);

    if (!In_Radar(cell)) return;
    if (!sightrange) return;
    if (sightrange >= 11 || sightrange > 10) sightrange = 10;

    xx = cell.X;

    count = unknown_radius_count_7a5908[sightrange];
    ptr = &unknown_radius_offset_a6f248[0];
    ptr2 = &unknown_occlusion_offset_a6ed18[0];
    if (!Rules->RevealByHeight && incremental) {
        if (sightrange > 2) {
            ptr += unknown_radius_count_7a5908[sightrange - 3];
            ptr2 += unknown_radius_count_7a5908[sightrange - 3];
            count -= unknown_radius_count_7a5908[sightrange - 3];
        }
    }
    ptr2--;

    if (house != 0 && unknown_player_a35db4 != 0) {
        if (house != unknown_player_a35db4) {
            if ((house->unknown_5414 & (1 << unknown_player_a35db4->Type->House)) != 0) {
                house = unknown_player_a35db4;
            }
        }
        if (house != unknown_player_a35db4) {
            if (house->Is_Ally(unknown_player_a35db4) && Rules->unknown_142b) {
                house = unknown_player_a35db4;
            }
        }
    }

    if (house != unknown_player_a35db4) {
        return;
    }

    while (count--) {
        Cell newcell;
        int xdiff;

        newcell = cell + *ptr++;
        ptr2++;

        if (!In_Radar(newcell)) continue;
        xdiff = newcell.X - xx;
        xdiff = abs(xdiff);
        if (xdiff > sightrange) continue;
        Cell delta = newcell - cell;
        if ((short)unknown_sqrt_4ba410((double)delta.X * delta.X + (double)delta.Y * delta.Y) > sightrange) continue;

        bool ok = false;
        CellClass *cellptr = &(*this)[newcell];
        if (byheight && Rules->RevealByHeight) {
            if (Map[newcell - hoffset + *ptr2].unknown_11b <= cell_height + 3) {
                ok = true;
            }
        } else {
            ok = true;
        }

        if (ok) {
            cellptr->IsToFog = false;
            if (unfog) {
                if ((!cellptr->IsFogVisible || !cellptr->IsFogMapped) && cellptr->IsMapped) {
                    Map.unknown_slot38(newcell, house);
                }
            } else {
                Map.RevealFogShroud(newcell, house, arg8);
            }
        }
    }

}


// void MapClass::Unknown_0054A550(CoordStruct const &coord, int start, int sightrange, int skip) (0x0054A550)
// Refreshes the shroud frame of the cells of the sight rings start..sightrange
// (sight 3-11, start 0-8) around coord moved up the screen by its height
// (the radius tables of 0x00549C50): each cell whose tactical shroud frame
// (0x006A8D40) changed takes it and is redrawn (0x006AADD0). Nothing when skip.

extern int const unknown_radius_count_7a5908[];
extern Cell const unknown_radius_offset_a6f248[];

void MapClass::Unknown_0054A550(CoordStruct const &coord, int start, int sightrange, int skip)
{
    if (sightrange > 11) {
        sightrange = 11;
    } else if (sightrange < 3) {
        sightrange = 3;
    }
    if (start < 0) {
        start = 0;
    } else if (start > 8) {
        start = 8;
    }
    int count = (start == 0) ? unknown_radius_count_7a5908[sightrange] : unknown_radius_count_7a5908[sightrange] - unknown_radius_count_7a5908[start - 1];

    if (!skip) {
        CoordStruct center = coord;
        center.X += (Unknown_006A2970(center.Z) / -30) * 256;
        center.Y += (Unknown_006A2970(center.Z) / -30) * 256;
        Cell cell = Coord_Cell(center);

        Cell const *ptr = &unknown_radius_offset_a6f248[0];
        if (start) {
            ptr += unknown_radius_count_7a5908[start - 1];
        }
        while (count--) {
            Cell newcell = *ptr++ + cell;
            CellClass &cellptr = (*this)[newcell];
            signed char frame = unknown_tactical_83996c->Cell_Shadow(newcell, 0);
            if (frame != cellptr.unknown_120) {
                cellptr.unknown_120 = frame;
                cellptr.unknown_138 = true;
                unknown_tactical_83996c->Flag_Cell(&cellptr);
            }
        }
    }
}


// void MapClass::Unknown_0054A720(CoordStruct const &coord, int radius) (0x0054A720)
// Moves coord up the screen by its height (0x006A2970, in cells of 30
// pixels), then for every cell strictly inside radius + 1 of its cell whose
// +0x130 is clear sets +0x130 and clears +0x12C bits 3 and 4 (Map's cells;
// the reverse of 0x0054A8F0).

void MapClass::Unknown_0054A720(CoordStruct const &coord, int radius)
{
    CoordStruct center = coord;
    center.X += (Unknown_006A2970(center.Z) / -30) * 256;
    center.Y += (Unknown_006A2970(center.Z) / -30) * 256;
    Cell cell = Coord_Cell(center);

    for (int x = -2 - radius; x < radius + 2; x++) {
        for (int y = -2 - radius; y < radius + 2; y++) {
            if (x * x + y * y < (radius + 1) * (radius + 1)) {
                Cell newcell = XY_Cell(cell.X + x, cell.Y + y);
                if (Map[newcell].unknown_130 == 0) {
                    Map[newcell].unknown_130 = 1;
                    Map[newcell].IsMapped = false;
                    Map[newcell].IsVisible = false;
                }
            }
        }
    }
}


// void MapClass::Unknown_0054A8F0(CoordStruct const &coord, int radius) (0x0054A8F0)
// Moves coord up the screen by its height (0x006A2970, in cells of 30
// pixels), then for every cell strictly inside radius + 1 of its cell clears
// +0x130 and +0x134 and sets +0x12C bits 4 and 3 (Map's cells).

void MapClass::Unknown_0054A8F0(CoordStruct const &coord, int radius)
{
    CoordStruct center = coord;
    center.X += (Unknown_006A2970(center.Z) / -30) * 256;
    center.Y += (Unknown_006A2970(center.Z) / -30) * 256;
    Cell cell = Coord_Cell(center);

    for (int x = -2 - radius; x < radius + 2; x++) {
        for (int y = -2 - radius; y < radius + 2; y++) {
            if (x * x + y * y < (radius + 1) * (radius + 1)) {
                Cell newcell = XY_Cell(cell.X + x, cell.Y + y);
                Map[newcell].unknown_130 = 0;
                Map[newcell].unknown_134 = 0;
                Map[newcell].IsVisible = true;
                Map[newcell].IsMapped = true;
            }
        }
    }
}


// bool MapClass::In_Radar(Cell const &cell) (0x0054AAB0)
// Whether a cell lies inside the map's diamond: X + Y beyond the map
// width (+0xF0) and up to width + 2 * height (+0xF4), X - Y and Y - X
// below the width.

bool MapClass::In_Radar(Cell const &cell)
{
    int x = cell.X;
    int y = cell.Y;
    if (x + y > MapSize.Width && x - y < MapSize.Width && y - x < MapSize.Width &&
        x + y <= MapSize.Width + MapSize.Height * 2) {
        return true;
    }
    return false;
}


// bool MapClass::Unknown_0054AB00(CoordStruct const &coord) (0x0054AB00)
// The diamond test of 0x0054AAB0 for the cell of a lepton position
// (256 leptons per cell, through Cell's coordinate constructor); the test
// is expanded inline.

bool MapClass::Unknown_0054AB00(CoordStruct const &coord)
{
    return In_Radar(Cell(coord));
}


// void MapClass::Place_Down(Cell const &cell, ObjectClass *object) (0x0054AB70)
// TibSun MapClass::Pick_Up: the object leaves the cells of its occupy list (each cell
// unlinks it, 0x00473D20, and recalculates, 0x00472540). RA2 adds: a building whose
// type hides the cells behind it (CanHideThings) gives back the OccupyHeight - 1 cells
// up and left of each foundation cell (each counted once) and its AddOccupy cells
// (the cell's +0x100 hide count).

void __fastcall Copy_Cell_List_00482e10(Cell const *input, int capacity, Cell *output);

void MapClass::Place_Down(Cell const &cell, ObjectClass *object)
{
    if (!object) return;

    if (object->Class_Of()->unknown_205 && object->In_Which_Layer() == 2) {
        Cell xlist[32];
        Copy_Cell_List_00482e10(object->Occupy_List(0), 32, xlist);
        Cell const *list = xlist;
        while (list->X != 0x7FFF || list->Y != 0x7FFF) {
            Cell newcell = cell + *list;
            if (Has_Cell(newcell)) {
                (*this)[newcell].Occupy_Down(object, object->unknown_79);
                (*this)[newcell].Recalc_Attributes(-1);
            }
            list++;
        }

        if (object->Kind_Of() == 6) {
            BuildingClass *building = object->Kind_Of() == 6 ? (BuildingClass *)object : 0;
            if (building->Type->CanHideThings) {
                int height = building->Type->OccupyHeight - 1;
                if (height <= 0) height = 1;
                bool seen[0x800];
                memset(seen, 0, sizeof(seen));
                for (list = xlist; list->X != 0x7FFF || list->Y != 0x7FFF; list++) {
                    Cell newcell = cell + *list;
                    Cell pos = newcell;
                    int x = list->X;
                    int y = list->Y;
                    for (int i = 0; i < height; i++) {
                        if (pos.X >= 0 && pos.Y >= 0) {
                            if (x < 0) x += 16;
                            if (y < 0) y += 16;
                            if (!seen[y * 16 + x]) {
                                seen[y * 16 + x] = true;
                                if (Has_Cell(pos)) {
                                    CellClass &cellptr = (*this)[pos];
                                    cellptr.unknown_100++;
                                }
                            }
                            pos.X--;
                            pos.Y--;
                            x--;
                            y--;
                        }
                    }
                }
                for (int index = 0; index < 8; index++) {
                    if (Is_Occupy_Offset(building->Type->AddOccupy[index])) {
                        Cell newcell = cell;
                        newcell.X += building->Type->AddOccupy[index].X;
                        newcell.Y += building->Type->AddOccupy[index].Y;
                        if (Has_Cell(newcell)) {
                            CellClass &cellptr = (*this)[newcell];
                            cellptr.unknown_100++;
                        }
                    }
                    if (Is_Occupy_Offset(building->Type->RemoveOccupy[index])) {
                        Cell newcell = cell;
                        newcell.X += building->Type->RemoveOccupy[index].X;
                        newcell.Y += building->Type->RemoveOccupy[index].Y;
                        if (Has_Cell(newcell)) {
                            CellClass &cellptr = (*this)[newcell];
                            if (cellptr.unknown_100) cellptr.unknown_100--;
                        }
                    }
                }
            }
        }
    }
}


// void MapClass::Pick_Up(Cell const &cell, ObjectClass *object) (0x0054AF70)
// TibSun MapClass::Pick_Up: the object leaves the cells of its occupy list (each cell
// unlinks it, 0x00473D20, and recalculates, 0x00472540). RA2 adds: a building whose
// type hides the cells behind it (CanHideThings) gives back the OccupyHeight - 1 cells
// up and left of each foundation cell (each counted once) and its AddOccupy cells
// (the cell's +0x100 hide count).

void __fastcall Copy_Cell_List_00482e10(Cell const *input, int capacity, Cell *output);

void MapClass::Pick_Up(Cell const &cell, ObjectClass *object)
{
    if (!object) return;

    if (object->Class_Of()->unknown_205 && object->In_Which_Layer() == 2) {
        Cell xlist[32];
        Copy_Cell_List_00482e10(object->Occupy_List(0), 32, xlist);
        Cell const *list = xlist;
        while (list->X != 0x7FFF || list->Y != 0x7FFF) {
            Cell newcell = cell + *list;
            if (Has_Cell(newcell)) {
                (*this)[newcell].Occupy_Up(object, object->unknown_79);
                (*this)[newcell].Recalc_Attributes(-1);
            }
            list++;
        }

        if (object->Kind_Of() == 6) {
            BuildingClass *building = object->Kind_Of() == 6 ? (BuildingClass *)object : 0;
            if (building->Type->CanHideThings) {
                int height = building->Type->OccupyHeight - 1;
                if (height <= 0) height = 1;
                bool seen[0x800];
                memset(seen, 0, sizeof(seen));
                for (list = xlist; list->X != 0x7FFF || list->Y != 0x7FFF; list++) {
                    Cell newcell = cell + *list;
                    Cell pos = newcell;
                    int x = list->X;
                    int y = list->Y;
                    for (int i = 0; i < height; i++) {
                        if (pos.X >= 0 && pos.Y >= 0) {
                            if (x < 0) x += 16;
                            if (y < 0) y += 16;
                            if (!seen[y * 16 + x]) {
                                seen[y * 16 + x] = true;
                                if (Has_Cell(pos)) {
                                    CellClass &cellptr = (*this)[pos];
                                    if (cellptr.unknown_100) cellptr.unknown_100--;
                                }
                            }
                            pos.X--;
                            pos.Y--;
                            x--;
                            y--;
                        }
                    }
                }
                for (int index = 0; index < 8; index++) {
                    if (Is_Occupy_Offset(building->Type->AddOccupy[index])) {
                        Cell newcell = cell;
                        newcell.X += building->Type->AddOccupy[index].X;
                        newcell.Y += building->Type->AddOccupy[index].Y;
                        if (Has_Cell(newcell)) {
                            CellClass &cellptr = (*this)[newcell];
                            if (cellptr.unknown_100) cellptr.unknown_100--;
                        }
                    }
                }
            }
        }
    }
}


// int MapClass::Overpass() (0x0054B2E0)
// TibSun Overpass: the final pass over a freshly loaded map. Deletes the animations
// marked for it (+0x177), clears every cell's cross-line flags (+0x140 bits 20-21),
// then per cell clears +0x30, rebuilds its drawer, clears the anim-attached bit (17),
// marks the row or column of a cell whose tag crosses horizontally or vertically,
// adjusts its tiberium (summing the values), recalculates it and gives a wall its owner.

int MapClass::Overpass()
{
    int i;
    int value = 0;

    for (i = 0; i < Anims.Count(); i++) {
        if (Anims[i]->unknown_177) {
            delete Anims[i];
            i--;
        }
    }

    Reset_Iterator();

    CellClass *iter = Iterate();
    while (iter) {
        iter->IsHorizontalLine = false; iter->IsVerticalLine = false;
        iter = Iterate();
    }

    Reset_Iterator();

    iter = Iterate();
    while (iter) {

        iter->unknown_30 = 0;
        iter->Init_Drawer(0, 0x10000, 0, 1000, 1000, 1000);
        iter->IsAnimAttached = false;

        if (iter->unknown_3c) {

            if (iter->unknown_3c->Is_Cross_Horizontal()) {
                for (i = 0; i < Map.unknown_128; i++) {
                    Map[XY_Cell(i + unknown_120, iter->MapCoords.Y)].IsHorizontalLine = true;
                }
            }
            else if (iter->unknown_3c->Is_Cross_Vertical()) {
                for (i = 0; i < Map.unknown_12c; i++) {
                    Map[XY_Cell(iter->MapCoords.X, i + unknown_124)].IsVerticalLine = true;
                }
            }
        }

        value += iter->Tiberium_Adjust(false);
        iter->Recalc_Attributes(-1);
        if (iter->OverlayIndex != -1) {
            if (unknown_overlay_types_a35de8[iter->OverlayIndex]->unknown_278) {
                iter->Set_Wall_Owner();
            }
        }

        iter = Iterate();
    }
    return value;
}


// void MapClass::Repair_High_Bridge_Span_0054b560(CellClass *start, FacingType direction, Rect *dirty) (0x0054B560)
// TibSun MapClass::Repair_High_Bridge_Span: walks up to 30 cells from start toward direction (2 east-west,
// 4 north-south), repairs the middle pieces (Set_Bridge_Middle_State, subzone registration 0x00550230, a raised
// centre piece gets its three cells' heights back and its 2 x 5 cells queued) and stops at the far end piece
// (Set_Bridge_End_State). When an end was found the span is walked again laying the bridge overlay (OverlayTypes
// 24 / 25) on each repaired cell, the dirty rectangle spans both ends, and the zones / queued cells are
// recalculated. Draft by codex2 (86.96); both walks step with TibSun's
// `Adjacent_Cell(cursor->Fetch_CellID(), direction)` (Scout1010, TPoint3D coordinate profile), and the dirty corner
// uses always.h min (two /Ob2 sites the original needs to call operator+ 0x0042ADD0 out of line).
void *__cdecl operator new(unsigned int size);
extern int unknown_tileset_a6e028;
extern int unknown_tileset_a52c80;
extern int unknown_tileset_a6e010;
extern int unknown_tileset_a53390;

void MapClass::Repair_High_Bridge_Span_0054b560(CellClass *start, FacingType direction, Rect *dirty)
{
    Cell last = unknown_cell_a6f238;
    CellClass *cursor = &Map[start->Fetch_CellID()];
    bool zones_changed = false;
    DynamicVectorClass<Cell> changed;
    int length = 1;
    while (length < 30) {
        Cell next = Adjacent_Cell(cursor->Fetch_CellID(), direction);
        last = next;
        cursor = &Map[next];
        int tile = cursor->IsoTileIndex - unknown_tileset_a52c74 + 1;
        if (direction == 2) {
            if (tile == unknown_tileset_a6e028 && cursor->unknown_11a == 4) {
                Set_Bridge_End_State(last, false, false);
                zones_changed |= Unknown_00550230(Adjacent_Cell(last, 6));
                break;
            }
            if (tile == unknown_tileset_a52c80 && cursor->unknown_11a == 4) {
                Set_Bridge_End_State(last, false, false);
                zones_changed |= Unknown_00550230(Adjacent_Cell(last, 6));
                break;
            }
            if ((tile == unknown_tileset_a6cb7c || tile == unknown_tileset_a6cb7c + 3 ||
                 tile == unknown_tileset_a6cb7c + 4 || tile == unknown_tileset_a6cb7c + 1 ||
                 tile == unknown_tileset_a6cb7c + 2) && cursor->unknown_11a == 4) {
                Set_Bridge_Middle_State(last, unknown_tileset_a6cb7c - 1 + unknown_tileset_a52c74, -1, -1, false);
                zones_changed |= Unknown_00550230(last);
                if (tile == unknown_tileset_a6cb7c + 4) {
                    Map[last].unknown_11b += 4;
                    Map[Adjacent_Cell(last, 0)].unknown_11b += 4;
                    Map[Adjacent_Cell(last, 4)].unknown_11b += 4;
                    for (int x = 0; x < 2; ++x) {
                        for (int y = -2; y < 3; ++y) {
                            changed.Add(Cell(last + Cell(x, y)));
                        }
                    }
                }
            }
        } else if (direction == 4) {
            if (tile == unknown_tileset_a6e010 && cursor->unknown_11a == 2) {
                Set_Bridge_End_State(last, false, false);
                zones_changed |= Unknown_00550230(Adjacent_Cell(last, 0));
                break;
            }
            if (tile == unknown_tileset_a53390 && cursor->unknown_11a == 2) {
                Set_Bridge_End_State(last, false, false);
                zones_changed |= Unknown_00550230(Adjacent_Cell(last, 0));
                break;
            }
            if ((tile == unknown_tileset_a52e70 || tile == unknown_tileset_a52e70 + 3 ||
                 tile == unknown_tileset_a52e70 + 4 || tile == unknown_tileset_a52e70 + 2 ||
                 tile == unknown_tileset_a52e70 + 1) && cursor->unknown_11a == 2) {
                Set_Bridge_Middle_State(last, unknown_tileset_a52c74 - 1 + unknown_tileset_a52e70, -1, -1, false);
                zones_changed |= Unknown_00550230(last);
                if (tile == unknown_tileset_a52e70 + 4) {
                    Map[last].unknown_11b += 4;
                    Map[Adjacent_Cell(last, 2)].unknown_11b += 4;
                    Map[Adjacent_Cell(last, 6)].unknown_11b += 4;
                    for (int y = 0; y < 2; ++y) {
                        for (int x = -2; x < 3; ++x) {
                            changed.Add(Cell(last + Cell(x, y)));
                        }
                    }
                }
            }
        }
        ++length;
    }
    if (length == 30) return;
    cursor = &Map[start->Fetch_CellID()];
    if (dirty) {
        CoordStruct coord(start->Fetch_CellID(), unknown_geometry_g * start->unknown_11b);
        Point2D pixel;
        unknown_tactical_83996c->Coord_To_Pixel(coord, pixel);
        dirty->X = pixel.X;
        dirty->Y = pixel.Y;
    }
    if (length > 0) {
        int remaining = length;
        do {
            int tile = cursor->IsoTileIndex - unknown_tileset_a52c74 + 1;
            if ((tile != unknown_tileset_a6cb7c || (cursor->unknown_11a & 1)) &&
                (tile != unknown_tileset_a52e70 || (unsigned char)cursor->unknown_11a >= 5)) {
                if (direction == 2) new OverlayClass(OverlayTypes[24], cursor->Fetch_CellID(), -1);
                else new OverlayClass(OverlayTypes[25], cursor->Fetch_CellID(), -1);
            }
            Cell next = Adjacent_Cell(cursor->Fetch_CellID(), direction);
            last = next;
            cursor = &Map[next];
            --remaining;
        } while (remaining);
    }
    if (dirty) {
        CoordStruct coord(last, unknown_geometry_g * Map[last].unknown_11b);
        Point2D pixel;
        unknown_tactical_83996c->Coord_To_Pixel(coord, pixel);
        int original_x = dirty->X;
        int original_y = dirty->Y;
        dirty->X = min(dirty->X, pixel.X) - 64;
        dirty->Y = min(dirty->Y, pixel.Y) - 64;
        dirty->Width = abs(original_x - pixel.X) + 128;
        dirty->Height = abs(original_y - pixel.Y) + 128;
    }
    if (zones_changed) Zone_Reset();
    if (changed.Count() > 0) Unknown_005688A0(changed);
}


// void MapClass::Repair_Train_Bridge_Span_0054be80(CellClass *start, FacingType direction, Rect *dirty) (0x0054BE80)
// Train-bridge twin of Repair_High_Bridge_Span 0x0054B560 (same TibSun shape): walks up to 30 cells from start
// toward direction (2 east-west, 4 north-south) over the train-bridge tiles (tileset base unknown_tileset_a6cb68),
// repairs the middle pieces and stops at the far end piece. When an end was found the span is walked again laying
// the train-bridge overlay (OverlayTypes 237 / 238) on each repaired cell, the dirty rectangle spans both ends,
// and the zones / queued cells are recalculated. Draft by codex2 (86.94) finished with 0x0054B560's walk and
// min() forms.
void *__cdecl operator new(unsigned int size);
extern int unknown_tileset_a6e028;
extern int unknown_tileset_a52c80;
extern int unknown_tileset_a6e010;
extern int unknown_tileset_a53390;

void MapClass::Repair_Train_Bridge_Span_0054be80(CellClass *start, FacingType direction, Rect *dirty)
{
    Cell last = unknown_cell_a6f238;
    CellClass *cursor = &Map[start->Fetch_CellID()];
    bool zones_changed = false;
    DynamicVectorClass<Cell> changed;
    int length = 1;
    while (length < 30) {
        Cell next = Adjacent_Cell(cursor->Fetch_CellID(), direction);
        last = next;
        cursor = &Map[next];
        int tile = cursor->IsoTileIndex - unknown_tileset_a6cb68 + 1;
        if (direction == 2) {
            if (tile == unknown_tileset_a6e028 && cursor->unknown_11a == 4) {
                Set_Bridge_End_State(last, false, false);
                zones_changed |= Unknown_00550230(Adjacent_Cell(last, 6));
                break;
            }
            if (tile == unknown_tileset_a52c80 && cursor->unknown_11a == 4) {
                Set_Bridge_End_State(last, false, false);
                zones_changed |= Unknown_00550230(Adjacent_Cell(last, 6));
                break;
            }
            if ((tile == unknown_tileset_a6cb7c || tile == unknown_tileset_a6cb7c + 3 ||
                 tile == unknown_tileset_a6cb7c + 4 || tile == unknown_tileset_a6cb7c + 1 ||
                 tile == unknown_tileset_a6cb7c + 2) && cursor->unknown_11a == 4) {
                Set_Bridge_Middle_State(last, unknown_tileset_a6cb7c - 1 + unknown_tileset_a6cb68, -1, -1, false);
                zones_changed |= Unknown_00550230(last);
                if (tile == unknown_tileset_a6cb7c + 4) {
                    Map[last].unknown_11b += 4;
                    Map[Adjacent_Cell(last, 0)].unknown_11b += 4;
                    Map[Adjacent_Cell(last, 4)].unknown_11b += 4;
                    for (int x = 0; x < 2; ++x) {
                        for (int y = -2; y < 3; ++y) {
                            changed.Add(Cell(last + Cell(x, y)));
                        }
                    }
                }
            }
        } else if (direction == 4) {
            if (tile == unknown_tileset_a6e010 && cursor->unknown_11a == 2) {
                Set_Bridge_End_State(last, false, false);
                zones_changed |= Unknown_00550230(Adjacent_Cell(last, 0));
                break;
            }
            if (tile == unknown_tileset_a53390 && cursor->unknown_11a == 2) {
                Set_Bridge_End_State(last, false, false);
                zones_changed |= Unknown_00550230(Adjacent_Cell(last, 0));
                break;
            }
            if ((tile == unknown_tileset_a52e70 || tile == unknown_tileset_a52e70 + 3 ||
                 tile == unknown_tileset_a52e70 + 4 || tile == unknown_tileset_a52e70 + 2 ||
                 tile == unknown_tileset_a52e70 + 1) && cursor->unknown_11a == 2) {
                Set_Bridge_Middle_State(last, unknown_tileset_a6cb68 - 1 + unknown_tileset_a52e70, -1, -1, false);
                zones_changed |= Unknown_00550230(last);
                if (tile == unknown_tileset_a52e70 + 4) {
                    Map[last].unknown_11b += 4;
                    Map[Adjacent_Cell(last, 2)].unknown_11b += 4;
                    Map[Adjacent_Cell(last, 6)].unknown_11b += 4;
                    for (int y = 0; y < 2; ++y) {
                        for (int x = -2; x < 3; ++x) {
                            changed.Add(Cell(last + Cell(x, y)));
                        }
                    }
                }
            }
        }
        ++length;
    }
    if (length == 30) return;
    cursor = &Map[start->Fetch_CellID()];
    if (dirty) {
        CoordStruct coord(start->Fetch_CellID(), unknown_geometry_g * start->unknown_11b);
        Point2D pixel;
        unknown_tactical_83996c->Coord_To_Pixel(coord, pixel);
        dirty->X = pixel.X;
        dirty->Y = pixel.Y;
    }
    if (length > 0) {
        int remaining = length;
        do {
            int tile = cursor->IsoTileIndex - unknown_tileset_a6cb68 + 1;
            if ((tile != unknown_tileset_a6cb7c || (cursor->unknown_11a & 1)) &&
                (tile != unknown_tileset_a52e70 || (unsigned char)cursor->unknown_11a >= 5)) {
                if (direction == 2) new OverlayClass(OverlayTypes[237], cursor->Fetch_CellID(), -1);
                else new OverlayClass(OverlayTypes[238], cursor->Fetch_CellID(), -1);
            }
            Cell next = Adjacent_Cell(cursor->Fetch_CellID(), direction);
            last = next;
            cursor = &Map[next];
            --remaining;
        } while (remaining);
    }
    if (dirty) {
        CoordStruct coord(last, unknown_geometry_g * Map[last].unknown_11b);
        Point2D pixel;
        unknown_tactical_83996c->Coord_To_Pixel(coord, pixel);
        int original_x = dirty->X;
        int original_y = dirty->Y;
        dirty->X = min(dirty->X, pixel.X) - 64;
        dirty->Y = min(dirty->Y, pixel.Y) - 64;
        dirty->Width = abs(original_x - pixel.X) + 128;
        dirty->Height = abs(original_y - pixel.Y) + 128;
    }
    if (zones_changed) Zone_Reset();
    if (changed.Count() != 0) Unknown_005688A0(changed);
}


extern int unknown_tileset_a52c74;
extern int unknown_tileset_a6cb7c;
extern int unknown_tileset_a52e70;
extern int unknown_tileset_a6e0e4;
extern int unknown_tileset_a52f80;
extern int unknown_tileset_a53394;
extern int unknown_tileset_a52588;

void MapClass::Repair_All_High_Bridges_0054c7a0()
{
    for (int y = unknown_124; y < unknown_124 + unknown_12c; ++y) {
        for (int x = unknown_120; x < unknown_120 + unknown_128; ++x) {
            Cell cell(x, y);
            if (!Has_Cell(cell)) continue;
            CellClass *piece = &Map[cell];
            int tile = piece->IsoTileIndex - unknown_tileset_a52c74 + 1;
            if ((tile == unknown_tileset_a6e0e4 || tile == unknown_tileset_a52f80) && piece->unknown_11a == 8) {
                Repair_High_Bridge_Span_0054b560(piece, FACING_E, 0);
            }
            if ((tile == unknown_tileset_a6cb7c || tile == unknown_tileset_a6cb7c + 3 ||
                 tile == unknown_tileset_a6cb7c + 4 || tile == unknown_tileset_a6cb7c + 1 ||
                 tile == unknown_tileset_a6cb7c + 2) && piece->unknown_11a == 5) {
                if (tile == unknown_tileset_a6cb7c + 4) {
                    Set_Bridge_Middle_State(cell, unknown_tileset_a6cb7c - 1 + unknown_tileset_a52c74, -1, -1, false);
                    Cell const &base = Adjacent_Cell(cell, FACING_W);
                    Map[base].unknown_11b += 4;
                    Map[Adjacent_Cell(base, FACING_N)].unknown_11b += 4;
                    Map[Adjacent_Cell(base, FACING_S)].unknown_11b += 4;
                }
                Repair_High_Bridge_Span_0054b560(piece, FACING_E, 0);
            }
            if ((tile == unknown_tileset_a53394 || tile == unknown_tileset_a52588) && piece->unknown_11a == 12) {
                Repair_High_Bridge_Span_0054b560(piece, FACING_S, 0);
            }
            if ((tile == unknown_tileset_a52e70 || tile == unknown_tileset_a52e70 + 3 ||
                 tile == unknown_tileset_a52e70 + 4 || tile == unknown_tileset_a52e70 + 2 ||
                 tile == unknown_tileset_a52e70 + 1) && piece->unknown_11a == 7) {
                if (tile == unknown_tileset_a52e70 + 4) {
                    Set_Bridge_Middle_State(cell, unknown_tileset_a52e70 - 1 + unknown_tileset_a52c74, -1, -1, false);
                    Cell const &base = Adjacent_Cell(cell, FACING_N);
                    Map[base].unknown_11b += 4;
                    Map[Adjacent_Cell(base, FACING_E)].unknown_11b += 4;
                    Map[Adjacent_Cell(base, FACING_W)].unknown_11b += 4;
                }
                Repair_High_Bridge_Span_0054b560(piece, FACING_S, 0);
            }
        }
    }
}


extern int unknown_tileset_a52c74;
extern int unknown_tileset_a6cb7c;
extern int unknown_tileset_a52e70;
extern int unknown_tileset_a6e0e4;
extern int unknown_tileset_a52f80;
extern int unknown_tileset_a53394;
extern int unknown_tileset_a52588;

void MapClass::Repair_High_Bridge_From_Cell_0054cbe0(Cell const &origin)
{
    int radius = 1;
    for (;;) {
        int extent = unknown_128 > unknown_12c ? unknown_128 : unknown_12c;
        if (radius >= extent) return;
        for (int dy = -radius; dy < radius; ++dy) {
            for (int dx = -radius; dx < radius; ++dx) {
                Cell cell = origin + Cell(dx, dy);
                if (cell.X < unknown_120 || cell.X > unknown_120 + unknown_128 ||
                cell.Y < unknown_124 || cell.Y > unknown_124 + unknown_12c || !Has_Cell(cell)) continue;
                CellClass *piece = &Map[cell];
                int tile = piece->IsoTileIndex - unknown_tileset_a52c74 + 1;
                if ((tile == unknown_tileset_a6e0e4 || tile == unknown_tileset_a52f80) && piece->unknown_11a == 8) {
                    Set_Bridge_End_State(cell, false, false);
                    Repair_High_Bridge_Span_0054b560(piece, FACING_E, 0);
                    return;
                }
                if ((tile == unknown_tileset_a6cb7c || tile == unknown_tileset_a6cb7c + 3 ||
                tile == unknown_tileset_a6cb7c + 4 || tile == unknown_tileset_a6cb7c + 1 ||
                tile == unknown_tileset_a6cb7c + 2) && piece->unknown_11a == 5) {
                    if (tile == unknown_tileset_a6cb7c + 4) {
                        Set_Bridge_Middle_State(cell, unknown_tileset_a6cb7c - 1 + unknown_tileset_a52c74, -1, -1, false);
                        Cell const &base = Adjacent_Cell(cell, FACING_W);
                        Map[base].unknown_11b += 4;
                        Map[Adjacent_Cell(base, FACING_N)].unknown_11b += 4;
                        Map[Adjacent_Cell(base, FACING_S)].unknown_11b += 4;
                    }
                    Repair_High_Bridge_Span_0054b560(piece, FACING_E, 0);
                    return;
                }
                if ((tile == unknown_tileset_a53394 || tile == unknown_tileset_a52588) && piece->unknown_11a == 12) {
                    Set_Bridge_End_State(cell, false, false);
                    Repair_High_Bridge_Span_0054b560(piece, FACING_S, 0);
                    return;
                }
                if ((tile == unknown_tileset_a52e70 || tile == unknown_tileset_a52e70 + 3 ||
                tile == unknown_tileset_a52e70 + 4 || tile == unknown_tileset_a52e70 + 2 ||
                tile == unknown_tileset_a52e70 + 1) && piece->unknown_11a == 7) {
                    if (tile == unknown_tileset_a52e70 + 4) {
                        Set_Bridge_Middle_State(cell, unknown_tileset_a52e70 - 1 + unknown_tileset_a52c74, -1, -1, false);
                        Cell const &base = Adjacent_Cell(cell, FACING_N);
                        Map[base].unknown_11b += 4;
                        Map[Adjacent_Cell(base, FACING_E)].unknown_11b += 4;
                        Map[Adjacent_Cell(base, FACING_W)].unknown_11b += 4;
                    }
                    Repair_High_Bridge_Span_0054b560(piece, FACING_S, 0);
                    return;
                }
            }
        }
        ++radius;
    }
}


extern int unknown_tileset_a6cb68;
extern int unknown_tileset_a6cb7c;
extern int unknown_tileset_a52e70;
extern int unknown_tileset_a6e0e4;
extern int unknown_tileset_a52f80;
extern int unknown_tileset_a53394;
extern int unknown_tileset_a52588;

void MapClass::Repair_Train_Bridge_From_Cell_0054d0a0(Cell const &origin)
{
    int radius = 1;
    for (;;) {
        int extent = unknown_128 > unknown_12c ? unknown_128 : unknown_12c;
        if (radius >= extent) return;
        for (int dy = -radius; dy < radius; ++dy) {
            for (int dx = -radius; dx < radius; ++dx) {
                Cell cell = origin + Cell(dx, dy);
                if (cell.X < unknown_120 || cell.X > unknown_120 + unknown_128 ||
                cell.Y < unknown_124 || cell.Y > unknown_124 + unknown_12c || !Has_Cell(cell)) continue;
                CellClass *piece = &Map[cell];
                int tile = piece->IsoTileIndex - unknown_tileset_a6cb68 + 1;
                if ((tile == unknown_tileset_a6e0e4 || tile == unknown_tileset_a52f80) && piece->unknown_11a == 8) {
                    Set_Bridge_End_State(cell, false, false);
                    Repair_Train_Bridge_Span_0054be80(piece, FACING_E, 0);
                    return;
                }
                if ((tile == unknown_tileset_a6cb7c || tile == unknown_tileset_a6cb7c + 3 ||
                tile == unknown_tileset_a6cb7c + 4 || tile == unknown_tileset_a6cb7c + 1 ||
                tile == unknown_tileset_a6cb7c + 2) && piece->unknown_11a == 5) {
                    if (tile == unknown_tileset_a6cb7c + 4) {
                        Set_Bridge_Middle_State(cell, unknown_tileset_a6cb7c - 1 + unknown_tileset_a6cb68, -1, -1, false);
                        Cell const &base = Adjacent_Cell(cell, FACING_W);
                        Map[base].unknown_11b += 4;
                        Map[Adjacent_Cell(base, FACING_N)].unknown_11b += 4;
                        Map[Adjacent_Cell(base, FACING_S)].unknown_11b += 4;
                    }
                    Repair_Train_Bridge_Span_0054be80(piece, FACING_E, 0);
                    return;
                }
                if ((tile == unknown_tileset_a53394 || tile == unknown_tileset_a52588) && piece->unknown_11a == 12) {
                    Set_Bridge_End_State(cell, false, false);
                    Repair_Train_Bridge_Span_0054be80(piece, FACING_S, 0);
                    return;
                }
                if ((tile == unknown_tileset_a52e70 || tile == unknown_tileset_a52e70 + 3 ||
                tile == unknown_tileset_a52e70 + 4 || tile == unknown_tileset_a52e70 + 2 ||
                tile == unknown_tileset_a52e70 + 1) && piece->unknown_11a == 7) {
                    if (tile == unknown_tileset_a52e70 + 4) {
                        Set_Bridge_Middle_State(cell, unknown_tileset_a52e70 - 1 + unknown_tileset_a6cb68, -1, -1, false);
                        Cell const &base = Adjacent_Cell(cell, FACING_N);
                        Map[base].unknown_11b += 4;
                        Map[Adjacent_Cell(base, FACING_E)].unknown_11b += 4;
                        Map[Adjacent_Cell(base, FACING_W)].unknown_11b += 4;
                    }
                    Repair_Train_Bridge_Span_0054be80(piece, FACING_S, 0);
                    return;
                }
            }
        }
        ++radius;
    }
}


// int MapClass::Unknown_0054D560(Pipe &pipe) (0x0054D560)
// Writes the tile data of every cell slot through an LCW pipe (TibSun
// Write_Binary_1): every tile type (two bytes), then every sub tile, then
// every height; an empty slot writes the dummy cell's. Returns the bytes put.

int MapClass::Unknown_0054D560(Pipe &pipe)
{
    int total = 0;

    LCWPipe comp(LCWPipe::COMPRESS, 0x2000);
    comp.Put_To(&pipe);

    for (int i = 0; i < Cells.Length(); i++) {
        CellClass *cellptr = Cells[i];
        if (cellptr) {
            total += comp.Put(&cellptr->IsoTileIndex, 2);
        } else {
            total += comp.Put(&unknown_dummy_cell_a6f9f8.IsoTileIndex, 2);
        }
    }

    for (int j = 0; j < Cells.Length(); j++) {
        CellClass *cellptr = Cells[j];
        if (cellptr) {
            total += comp.Put(&cellptr->unknown_11a, 1);
        } else {
            total += comp.Put(&unknown_dummy_cell_a6f9f8.unknown_11a, 1);
        }
    }

    for (int k = 0; k < Cells.Length(); k++) {
        CellClass *cellptr = Cells[k];
        if (cellptr) {
            total += comp.Put(&cellptr->unknown_11b, 1);
        } else {
            total += comp.Put(&unknown_dummy_cell_a6f9f8.unknown_11b, 1);
        }
    }

    return total;
}


// int MapClass::Unknown_0054D660(Pipe &pipe) (0x0054D660)
// Writes, through an LCW pipe, the cell, tile type, sub tile and height of
// every cell on the map (0x0054AAB0) that has a tile or height (TibSun
// Write_Binary_2), then the "no cell" value 101 times. Returns the bytes put.

int MapClass::Unknown_0054D660(Pipe &pipe)
{
    int total = 0;

    LCWPipe comp(LCWPipe::COMPRESS, 0x2000);
    comp.Put_To(&pipe);

    int i;
    for (i = 0; i < Cells.Length(); i++) {
        CellClass *cellptr = Cells[i];
        if (i != 0 && cellptr) {
            if (In_Radar(cellptr->Fetch_CellID())) {
                if (cellptr->Fetch_CellID() != unknown_cell_a6f238 && cellptr != &unknown_dummy_cell_a6f9f8 &&
                    (cellptr->IsoTileIndex != 0xFFFF || cellptr->unknown_11b > 0)) {
                    Cell cell = cellptr->Fetch_CellID();
                    total += comp.Put(&cell, sizeof(cell));
                    total += comp.Put(&cellptr->IsoTileIndex, sizeof(cellptr->IsoTileIndex));
                    total += comp.Put(&cellptr->unknown_11a, sizeof(cellptr->unknown_11a));
                    total += comp.Put(&cellptr->unknown_11b, sizeof(cellptr->unknown_11b));
                }
            }
        }
    }

    Cell empty = unknown_cell_a6f238;
    total += comp.Put(&empty, sizeof(empty));
    for (i = 0; i < 100; i++) {
        total += comp.Put(&empty, sizeof(empty));
    }
    comp.End();

    return total;
}


// int MapClass::Unknown_0054D810(Pipe &pipe) (0x0054D810)
// As 0x0054D660 straight into the pipe, with one "no cell" value at the end
// (TibSun Write_Binary_3).

int MapClass::Unknown_0054D810(Pipe &pipe)
{
    int total = 0;

    for (int i = 0; i < Cells.Length(); i++) {
        CellClass *cellptr = Cells[i];
        if (i != 0 && cellptr) {
            if (In_Radar(cellptr->Fetch_CellID())) {
                if (cellptr->Fetch_CellID() != unknown_cell_a6f238 && cellptr != &unknown_dummy_cell_a6f9f8 &&
                    (cellptr->IsoTileIndex != 0xFFFF || cellptr->unknown_11b > 0)) {
                    Cell cell = cellptr->Fetch_CellID();
                    total += pipe.Put(&cell, sizeof(cell));
                    total += pipe.Put(&cellptr->IsoTileIndex, sizeof(cellptr->IsoTileIndex));
                    total += pipe.Put(&cellptr->unknown_11a, sizeof(cellptr->unknown_11a));
                    total += pipe.Put(&cellptr->unknown_11b, sizeof(cellptr->unknown_11b));
                }
            }
        }
    }

    Cell empty = unknown_cell_a6f238;
    total += pipe.Put(&empty, sizeof(empty));

    return total;
}


// int MapClass::Unknown_0054D970(Pipe &pipe) (0x0054D970)
// As 0x0054D810 through an LZO pipe, adding what End flushes (TibSun Write_Binary_4).

int MapClass::Unknown_0054D970(Pipe &pipe)
{
    LZOPipe comp(LZOPipe::COMPRESS, 0x2000);
    comp.Put_To(&pipe);
    int total = 0;

    for (int i = 0; i < Cells.Length(); i++) {
        CellClass *cellptr = Cells[i];
        if (i != 0 && cellptr) {
            if (In_Radar(cellptr->Fetch_CellID())) {
                if (cellptr->Fetch_CellID() != unknown_cell_a6f238 && cellptr != &unknown_dummy_cell_a6f9f8 &&
                    (cellptr->IsoTileIndex != 0xFFFF || cellptr->unknown_11b > 0)) {
                    Cell cell = cellptr->Fetch_CellID();
                    total += comp.Put(&cell, sizeof(cell));
                    total += comp.Put(&cellptr->IsoTileIndex, sizeof(cellptr->IsoTileIndex));
                    total += comp.Put(&cellptr->unknown_11a, sizeof(cellptr->unknown_11a));
                    total += comp.Put(&cellptr->unknown_11b, sizeof(cellptr->unknown_11b));
                }
            }
        }
    }

    Cell empty = unknown_cell_a6f238;
    total += comp.Put(&empty, sizeof(empty));
    total += comp.End();

    return total;
}


// int MapClass::Unknown_0054DB10(Pipe &pipe) (0x0054DB10)
// As 0x0054D970 with each cell's +0x119 after its height (TibSun Write_Binary_5).

int MapClass::Unknown_0054DB10(Pipe &pipe)
{
    LZOPipe comp(LZOPipe::COMPRESS, 0x2000);
    comp.Put_To(&pipe);
    int total = 0;

    for (int i = 0; i < Cells.Length(); i++) {
        CellClass *cellptr = Cells[i];
        if (i != 0 && cellptr) {
            if (In_Radar(cellptr->Fetch_CellID())) {
                if (cellptr->Fetch_CellID() != unknown_cell_a6f238 && cellptr != &unknown_dummy_cell_a6f9f8 &&
                    (cellptr->IsoTileIndex != 0xFFFF || cellptr->unknown_11b > 0)) {
                    Cell cell = cellptr->Fetch_CellID();
                    total += comp.Put(&cell, sizeof(cell));
                    total += comp.Put(&cellptr->IsoTileIndex, sizeof(cellptr->IsoTileIndex));
                    total += comp.Put(&cellptr->unknown_11a, sizeof(cellptr->unknown_11a));
                    total += comp.Put(&cellptr->unknown_11b, sizeof(cellptr->unknown_11b));
                    total += comp.Put(&cellptr->unknown_119, sizeof(cellptr->unknown_119));
                }
            }
        }
    }

    Cell empty = unknown_cell_a6f238;
    total += comp.Put(&empty, sizeof(empty));
    total += comp.End();

    return total;
}


// bool MapClass::Unknown_0054DCC0(Straw &straw) (0x0054DCC0)
// Reads the tile data of the first 128 x 128 cells through an LCW straw
// (TibSun Read_Binary_1): every tile type (two bytes, fixed up by
// 0x00529550), then every sub tile, then every height; a cell off the map
// reads into the dummy cell, which is rebuilt afterwards.

bool MapClass::Unknown_0054DCC0(Straw &straw)
{
    LCWStraw decomp(LCWStraw::DECOMPRESS, 0x2000);
    decomp.Get_From(&straw);

    int i;

    for (i = 0; i < 0x40000 / 16; i++) {
        CellClass *cellptr = &(*this)[Cell(i % 128, i / 128)];
        if (cellptr != 0) {
            cellptr->IsoTileIndex = 0xFFFF;
            decomp.Get(&cellptr->IsoTileIndex, sizeof(cellptr->IsoTileIndex) / 2);
            cellptr->IsoTileIndex = IsometricTileTypeClass::Unknown_00529550(cellptr->IsoTileIndex);
        } else {
            decomp.Get(&unknown_dummy_cell_a6f9f8.IsoTileIndex, sizeof(unknown_dummy_cell_a6f9f8.IsoTileIndex) / 2);
        }
    }

    for (i = 0; i < 0x40000 / 16; i++) {
        CellClass *cellptr = &(*this)[Cell(i % 128, i / 128)];
        if (cellptr != 0) {
            decomp.Get(&cellptr->unknown_11a, sizeof(cellptr->unknown_11a));
        } else {
            decomp.Get(&unknown_dummy_cell_a6f9f8.unknown_11a, sizeof(unknown_dummy_cell_a6f9f8.unknown_11a));
        }
    }

    for (i = 0; i < 0x40000 / 16; i++) {
        CellClass *cellptr = &(*this)[Cell(i % 128, i / 128)];
        if (cellptr != 0) {
            decomp.Get(&cellptr->unknown_11b, sizeof(cellptr->unknown_11b));
        } else {
            decomp.Get(&unknown_dummy_cell_a6f9f8.unknown_11b, sizeof(unknown_dummy_cell_a6f9f8.unknown_11b));
        }
    }

    new (&unknown_dummy_cell_a6f9f8) CellClass;
    return true;
}


// bool MapClass::Unknown_0054DEA0(Straw &straw) (0x0054DEA0)
// As 0x0054DFC0 through an LCW straw (TibSun Read_Binary_2).

bool MapClass::Unknown_0054DEA0(Straw &straw)
{
    LCWStraw decomp(LCWStraw::DECOMPRESS, 0x2000);
    decomp.Get_From(&straw);

    Cell cell;
    decomp.Get(&cell, sizeof(cell));

    while (cell != unknown_cell_a6f238) {
        CellClass *cellptr = &(*this)[cell];
        if (cellptr != 0 && cellptr != &unknown_dummy_cell_a6f9f8) {
            cellptr->IsoTileIndex = 0xFFFF;
            decomp.Get(&cellptr->IsoTileIndex, sizeof(cellptr->IsoTileIndex));
            cellptr->IsoTileIndex = IsometricTileTypeClass::Unknown_00529550(cellptr->IsoTileIndex);
            decomp.Get(&cellptr->unknown_11a, sizeof(cellptr->unknown_11a));
            decomp.Get(&cellptr->unknown_11b, sizeof(cellptr->unknown_11b));
        } else {
            char tmp[sizeof(cellptr->IsoTileIndex) + sizeof(cellptr->unknown_11a) + sizeof(cellptr->unknown_11b)];
            decomp.Get(&tmp, sizeof(tmp));
        }
        cell = unknown_cell_a6f238;
        decomp.Get(&cell, sizeof(cell));
    }
    new (&unknown_dummy_cell_a6f9f8) CellClass;
    return true;
}


// bool MapClass::Unknown_0054DFC0(Straw &straw) (0x0054DFC0)
// Reads tile data from an uncompressed straw (TibSun Read_Binary_3): for each
// cell up to the "no cell" value its tile type (fixed up by 0x00529550), sub
// tile and height; the record of a cell off the map is skipped. The dummy cell
// is rebuilt afterwards.

bool MapClass::Unknown_0054DFC0(Straw &straw)
{
    Cell cell;
    straw.Get(&cell, sizeof(cell));

    while (cell != unknown_cell_a6f238) {
        CellClass *cellptr = &(*this)[cell];
        if (cellptr != 0 && cellptr != &unknown_dummy_cell_a6f9f8) {
            cellptr->IsoTileIndex = 0xFFFF;
            straw.Get(&cellptr->IsoTileIndex, sizeof(cellptr->IsoTileIndex));
            cellptr->IsoTileIndex = IsometricTileTypeClass::Unknown_00529550(cellptr->IsoTileIndex);
            straw.Get(&cellptr->unknown_11a, sizeof(cellptr->unknown_11a));
            straw.Get(&cellptr->unknown_11b, sizeof(cellptr->unknown_11b));
        } else {
            char tmp[sizeof(cellptr->IsoTileIndex) + sizeof(cellptr->unknown_11a) + sizeof(cellptr->unknown_11b)];
            straw.Get(&tmp, sizeof(tmp));
        }
        cell = unknown_cell_a6f238;
        straw.Get(&cell, sizeof(cell));
    }
    new (&unknown_dummy_cell_a6f9f8) CellClass;
    return true;
}


// bool MapClass::Unknown_0054E0C0(Straw &straw) (0x0054E0C0)
// As 0x0054DFC0 through an LZO straw (TibSun Read_Binary_4).

bool MapClass::Unknown_0054E0C0(Straw &straw)
{
    LZOStraw decomp(LZOStraw::DECOMPRESS, 0x2000);
    decomp.Get_From(&straw);

    Cell cell;
    decomp.Get(&cell, sizeof(cell));

    while (cell != unknown_cell_a6f238) {
        CellClass *cellptr = &(*this)[cell];
        if (cellptr != 0 && cellptr != &unknown_dummy_cell_a6f9f8) {
            cellptr->IsoTileIndex = 0xFFFF;
            decomp.Get(&cellptr->IsoTileIndex, sizeof(cellptr->IsoTileIndex));
            cellptr->IsoTileIndex = IsometricTileTypeClass::Unknown_00529550(cellptr->IsoTileIndex);
            decomp.Get(&cellptr->unknown_11a, sizeof(cellptr->unknown_11a));
            decomp.Get(&cellptr->unknown_11b, sizeof(cellptr->unknown_11b));
        } else {
            char tmp[sizeof(cellptr->IsoTileIndex) + sizeof(cellptr->unknown_11a) + sizeof(cellptr->unknown_11b)];
            decomp.Get(&tmp, sizeof(tmp));
        }
        cell = unknown_cell_a6f238;
        decomp.Get(&cell, sizeof(cell));
    }
    new (&unknown_dummy_cell_a6f9f8) CellClass;
    return true;
}


// bool MapClass::Unknown_0054E1E0(Straw &straw) (0x0054E1E0)
// As 0x0054E0C0 with each cell's +0x119 after its height (TibSun Read_Binary_5).

bool MapClass::Unknown_0054E1E0(Straw &straw)
{
    LZOStraw decomp(LZOStraw::DECOMPRESS, 0x2000);
    decomp.Get_From(&straw);

    Cell cell;
    decomp.Get(&cell, sizeof(cell));

    while (cell != unknown_cell_a6f238) {
        CellClass *cellptr = &(*this)[cell];
        if (cellptr != 0 && cellptr != &unknown_dummy_cell_a6f9f8) {
            cellptr->IsoTileIndex = 0xFFFF;
            decomp.Get(&cellptr->IsoTileIndex, sizeof(cellptr->IsoTileIndex));
            cellptr->IsoTileIndex = IsometricTileTypeClass::Unknown_00529550(cellptr->IsoTileIndex);
            decomp.Get(&cellptr->unknown_11a, sizeof(cellptr->unknown_11a));
            decomp.Get(&cellptr->unknown_11b, sizeof(cellptr->unknown_11b));
            decomp.Get(&cellptr->unknown_119, sizeof(cellptr->unknown_119));
        } else {
            char tmp[sizeof(cellptr->IsoTileIndex) + sizeof(cellptr->unknown_11a) + sizeof(cellptr->unknown_11b) + sizeof(cellptr->unknown_119)];
            decomp.Get(&tmp, sizeof(tmp));
        }
        cell = unknown_cell_a6f238;
        decomp.Get(&cell, sizeof(cell));
    }
    new (&unknown_dummy_cell_a6f9f8) CellClass;
    return true;
}


// void MapClass::unknown_slot27() (0x0054E300, slot 27)
// Outside the campaign and with the 0x00A3D2C1 option on, every +0x154
// entry that has a cell and whose countdown ran out is removed (0x00493930)
// and a new one placed (0x0054E410).

void MapClass::unknown_slot27()
{
    if (unknown_game_mode_a3d298 != 0 && unknown_flag_a3d2c1) {
        for (int index = 0; index < 256; index++) {
            if (unknown_154[index].Cell != unknown_cell_a6f238 && unknown_154[index].Timer.Value() == 0) {
                unknown_154[index].Unknown_00493930();
                Unknown_0054E410();
            }
        }
    }
}


// int MapClass::Unknown_0054E370(Cell const &cell) (0x0054E370)
// The index of the cell's 4 x 4 block in a 130 x 130 grid that keeps a
// one-block border (the layout of HouseClass +0x56D0).

int MapClass::Unknown_0054E370(Cell const &cell)
{
    return (cell.Y / 4 + 1) * 130 + (cell.X / 4 + 1);
}


// int MapClass::Cell_Threat(Cell const &cell, HouseClass *house) (0x0054E3A0)
// The house's +0x56D0 value for the 4 x 4 block of a cell (taken from the
// map's cell object, so off-map cells use the dummy cell's coordinates).

int MapClass::Cell_Threat(Cell const &cell, HouseClass *house)
{
    Cell coords = Map[cell].MapCoords;
    return house->unknown_56d0[Unknown_0054E370(coords)];
}


// bool MapClass::Unknown_0054E410() (0x0054E410)
// Places a crate on a random cell (TibSun Place_Random_Crate): the first +0x154 entry whose
// cell is unknown_cell_a6f238; up to 1000 random cells inside the map's cell rectangle, each
// moved to a reachable cell nearby (0x005502E0; speed type 5 on water, land type 2, else 1)
// and offered to the entry (0x004939A0). False when no entry is free or no cell took the crate.

bool MapClass::Unknown_0054E410()
{
    int crateindex = 0;
    for (crateindex = 0; crateindex < 256; crateindex++) {
        if (unknown_154[crateindex].Cell == unknown_cell_a6f238) break;
    }
    if (crateindex == 256) {
        return false;
    }

    for (int index = 0; index < 1000; index++) {
        int x = Map.unknown_120 + Random_Pick(0, Map.unknown_128 - 1);
        int y = Map.unknown_124 + Random_Pick(0, Map.unknown_12c - 1);
        Cell cell = XY_Cell(x, y);

        if (Map[cell].unknown_ec == 2) {
            cell = Map.Nearby_Location(cell, 5, -1, 0, false, Point2D(1, 1), false, false, false, true, Cell(0, 0), 0);
        } else {
            cell = Map.Nearby_Location(cell, 1, -1, 0, false, Point2D(1, 1), false, false, false, true, Cell(0, 0), 0);
        }

        if (unknown_154[crateindex].Unknown_004939A0(cell)) {
            return true;
        }
    }
    return false;
}


// bool MapClass::Unknown_0054E580(Cell cell, int arg2) (0x0054E580)
// Places a crate near a cell: the nearest reachable cell for speed type 5 on
// water (cell land type 2) or 1 elsewhere (0x005502E0), the first free entry
// of the +0x154 table (cell unknown_cell_a6f238) creates the crate there
// (0x004939A0), and unless arg2 is 0x14 the cell's +0x11E takes arg2.

bool MapClass::Unknown_0054E580(Cell cell, int arg2)
{
    if (Map[cell].unknown_ec == 2) {
        cell = Map.Nearby_Location(cell, 5, -1, 0, false, Point2D(1, 1), 0, 0, 0, 1, Cell(0, 0), 0);
    } else {
        cell = Map.Nearby_Location(cell, 1, -1, 0, false, Point2D(1, 1), 0, 0, 0, 1, Cell(0, 0), 0);
    }

    int index;
    for (index = 0; index < 256; index++) {
        if (unknown_154[index].Cell == unknown_cell_a6f238) {
            break;
        }
    }
    if (index == 256) {
        return false;
    }

    if (unknown_154[index].Unknown_004939A0(cell)) {
        if (arg2 != 0x14) {
            Map[cell].OverlayData = arg2;
        }
        return true;
    }
    return false;
}


// bool MapClass::Remove_Crate(Cell const &cell) (0x0054E6E0)
// Removes the crate at a cell (TibSun Remove_Crate): in the campaign a crate overlay
// (+0x27A of its type) is taken off the cell (+0x44 = -1, +0x11E = 0) and its overlay
// rectangles (0x00474EB0, 0x00475100) are redrawn; otherwise the +0x154 entry holding the
// cell removes its crate (0x00493930). False when there was no crate.

bool MapClass::Remove_Crate(Cell const &cell)
{
    if (unknown_game_mode_a3d298 != 0) {
        for (int index = 0; index < 256; index++) {
            if (unknown_154[index].Cell != unknown_cell_a6f238 && cell == unknown_154[index].Cell) {
                return unknown_154[index].Unknown_00493930();
            }
        }
    } else {
        CellClass *cellptr = &(*this)[cell];
        if (cellptr->OverlayIndex != -1 && unknown_overlay_types_a35de8[cellptr->OverlayIndex]->unknown_27a) {
            Rect dirty = Union(cellptr->Overlay_Render_Rect(), cellptr->Overlay_Shadow_Render_Rect());
            dirty.Y -= unknown_int_839634;
            unknown_tactical_83996c->Register_Dirty_Area(dirty, 0);
            cellptr->OverlayIndex = -1;
            cellptr->OverlayData = 0;
            return true;
        }
    }
    return false;
}


// 0x0054e8c0: function not yet identified
//
// Body: 6 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 FUN_0054e8c0(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0054e8c0 {

unsigned int  FUN_0054e8c0(void)
{
    return 1;
}

}  // namespace seed_0054e8c0


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// ObjectClass *MapClass::Close_Object(CoordStruct const &coord) (0x0054E8D0), from ra2-decomp
// MapClass::Close_Object (YR 0x0056C210, src/map.cpp) on Map388's draft: the clickable object
// closest to coord among the occupiers of its cell and the eight around it, skipping a techno
// cloaked (+0x1B8 2) unless the player owns it (+0x372); a building counts by its cell centre and
// only within 0xB5 leptons. Null unless the closest lies within 0xB5. The twin's trailing
// `Cell unused = cell + _offsets[0];` is kept (twin-shaped; its inline Cell sum is the pending
// inline site the original's budget shows).

// The distance between two coordinates (TibSun Distance): the length (0x0041B780) of their
// difference.
inline int Distance(CoordStruct const &coord1, CoordStruct const &coord2)
{
    return (coord1 - coord2).Length();
}

ObjectClass *MapClass::Close_Object(CoordStruct const &coord)
{
    TechnoClass *object = 0;
    int distance = 0;
    Cell cell = Cell(coord);

    static Cell _offsets[] = {Cell(0, 0), Cell(-1, 0), Cell(1, 0), Cell(0, -1), Cell(0, 1), Cell(-1, 1), Cell(1, 1), Cell(1, -1), Cell(-1, -1)};
    for (int index = 0; index < 9; index++) {
        Cell newcell = Cell(cell + _offsets[index]);
        if (In_Radar(newcell)) {
            TechnoClass *o = (TechnoClass *)(*this)[newcell].Cell_Occupier();
            while (o != 0) {
                if (!o->AbstractFlags.IsTechno || o->unknown_372 || o->unknown_1b8 != 2) {
                    int d = -1;
                    if (o->Kind_Of() == 6) {
                        d = Distance(coord, CoordStruct(newcell.X * 256 + 128, newcell.Y * 256 + 128, 0));
                        if (d > 0xB5) d = -1;
                    } else {
                        d = Distance(coord, o->Center_Coord());
                    }
                    if (d >= 0 && (!object || d < distance)) {
                        distance = d;
                        object = o;
                    }
                }
                o = (TechnoClass *)o->unknown_30;
            }
        }
    }

    if (object && distance > 0xB5) {
        object = 0;
    }
    Cell unused = cell + _offsets[0];
    return (ObjectClass *)object;
}


// The exit-time destructor of Close_Object 0x0054E8D0's function-local static `_offsets` (0x0054EBC0, `_$E64`):
// Cell has no destructor work, so the body is a lone ret. The guarded static's initialisation registers it
// with atexit.
// No source of its own: emitted by this unit (Map.CPP) for the static in Close_Object.


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
extern int unknown_zone_a6fc34;
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


// Not exact (RA2_NON_MATCHING): MapClass::Unknown_0054EBD0 at 0x0054EBD0, best score 83.05, 2026-11-13; differs: 547 instructions in the original, 556 in the draft (shape 97.55)
#ifdef RA2_NON_MATCHING
// NON-EXACT DRAFT 0x54EBD0 MapClass::Zone_Reset (Zone_Reset)
// entry 0x54EBD0; exact-match score 84.41 (misses_index best); plan "draft, verify"
// origin lane: Scout641 (build/class-work/Scout641/w/keep/zone_reset_8441.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1089 (native/drafts.json). Not original bytes.
// Native adaptation (Scout1089): overlay names replaced by HEAD's; each changed line is marked NATIVE.
// int MapClass::Zone_Reset() (0x0054EBD0; TibSun MapClass::Zone_Reset)
// Rebuilds the zones: clears the zone adjacency table and the twelve movement zone
// tables, numbers every contiguous span of +0x64 elements (Zone_Span 0x0054F250, the
// passability of each new zone collected in a local list), files the zone pairs of the
// passable zone connections, builds each zone's neighbour list from the table, then
// for each of the 12 rows of 0x007E00C4 floods the zones whose passability the row
// treats alike into movement zones (2 up; zone 0 gets -1). Answers the zone of the
// largest span (TibSun answers its row-0 movement zone).
// NATIVE: the overlay hZR names (PassabilityType, LastAdjacentZone, Zone_Span) spelled with HEAD's names.
enum PassabilityType { PASSABLE_OUTSIDE = 7 };   // NATIVE: hZR map.hpp (RA2 outside value 7)
extern int unknown_zone_a6fc34;                  // NATIVE: hZR LastAdjacentZone (0x00A6FC34)

int MapClass::Zone_Reset()
{
    int i;
    int j;
    int k;
    int bestzone = -1;
    int bestspan = -1;

    UnknownMapElem4 *end = &CellZones[CellZoneCount];

    DynamicVectorClass<PassabilityType> vec;
    vec.Set_Growth_Step(300);

    unknown_14->Clear();

    for (i = 0; i < 12; i++) {
        if (unknown_18[i] != 0) {
            delete unknown_18[i];
            unknown_18[i] = 0;
        }
    }

    UnknownMapElem4 *czone = CellZones;
    for (; czone < end; czone++) {
        czone->unknown_02 = 0;
    }

    int zone = 1;

    vec.Add(PASSABLE_OUTSIDE);

    int skip = 0;
    czone = CellZones;
    while (czone < end) {
        PassabilityType pass = (PassabilityType)czone->unknown_00;
        if (pass != PASSABLE_OUTSIDE && czone->unknown_02 == 0) {
            unknown_zone_a6fc34 = 0;   // NATIVE
            int span = Unknown_0054F250(czone, zone, skip);   // NATIVE: Zone_Span
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

    unknown_48 = (unsigned short)zone;

    for (i = ZoneConnections.Count() - 1; i >= 0; i--) {
        if (ZoneConnections[i].unknown_08) {
            int from_zone = Unknown_0054FA60(ZoneConnections[i].From);
            int to_zone = Unknown_0054FA60(ZoneConnections[i].To);
            if (to_zone != from_zone) {
                if (to_zone < from_zone) {
                    int temp = to_zone;
                    to_zone = from_zone;
                    from_zone = temp;
                }
                unknown_14->Add_Object(HashObject<unsigned int, unsigned int>(from_zone, to_zone));
            }
        }
    }

    unsigned short *zone_degree = new unsigned short[unknown_48];
    for (i = 0; i < unknown_48; i++) {
        zone_degree[i] = 0;
    }

    for (i = 0; i < 256; i++) {
        DynamicVectorClass<HashObject<unsigned int, unsigned int> > &bucket = unknown_14->Buckets[i];
        if (bucket.Count() > 0) {
            j = bucket.Count();
            HashObject<unsigned int, unsigned int> *obj = &bucket[0];
            do {
                unsigned int value = obj->Value;
                unsigned short zone1 = (unsigned short)value;
                unsigned short zone2 = (unsigned short)(value >> 16);
                zone_degree[zone1]++;
                zone_degree[zone2]++;
                obj++;
            } while (--j);
        }
    }

    unsigned short **zone_neighbors = new unsigned short *[unknown_48];
    for (i = 0; i < unknown_48; i++) {
        zone_neighbors[i] = new unsigned short[zone_degree[i]];
    }

    for (i = 0; i < unknown_48; i++) {
        zone_degree[i] = 0;
    }

    for (i = 0; i < 256; i++) {
        DynamicVectorClass<HashObject<unsigned int, unsigned int> > &bucket = unknown_14->Buckets[i];
        if (bucket.Count() > 0) {
            j = bucket.Count();
            HashObject<unsigned int, unsigned int> *obj = &bucket[0];
            do {
                unsigned int value = obj->Value;
                unsigned short zone1 = (unsigned short)value;
                unsigned short zone2 = (unsigned short)(value >> 16);
                zone_neighbors[zone1][zone_degree[zone1]] = zone2;
                zone_neighbors[zone2][zone_degree[zone2]] = zone1;
                zone_degree[zone1]++;
                zone_degree[zone2]++;
                obj++;
            } while (--j);
        }
    }

    unsigned char *zone_passability = new unsigned char[unknown_48];
    for (i = 0; i < unknown_48; i++) {
        zone_passability[i] = vec[i];
    }

    unsigned short *stack = new unsigned short[unknown_48];

    for (int mzone = 0; mzone < 12; mzone++) {
        int next_movement_zone = 2;
        int *table = unknown_table_7e00c4[mzone];

        unsigned short *nzone = new unsigned short[unknown_48];
        unknown_18[mzone] = nzone;

        for (j = 0; j < unknown_48; j++) {
            nzone[j] = table[zone_passability[j]] != 1;
        }

        for (int zone_index = 0; zone_index < unknown_48; zone_index++) {
            if (nzone[zone_index] == 0) {
                int stackcount = 1;
                stack[0] = zone_index;
                nzone[zone_index] = next_movement_zone;
                int passable = table[zone_passability[zone_index]];
                while (stackcount) {
                    int current = stack[--stackcount];
                    unsigned short *neighbors = zone_neighbors[current];
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

    for (i = 0; i < unknown_48; i++) {
        delete zone_neighbors[i];
    }

    delete [] zone_passability;
    delete [] zone_neighbors;
    delete [] zone_degree;
    delete [] stack;

    return bestzone;
}
#endif

// 0x0054f240: function not yet identified
//
// Body: 16 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: uint __fastcall FUN_0054f240(uint *param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0054f240 {

unsigned int __fastcall FUN_0054f240(unsigned int *packed_value)
{
    return *packed_value >> 0xc & 0xf | *packed_value & 0xf;
}

}  // namespace seed_0054f240


// bool MapClass::Unknown_0054F7C0(Cell const &from, Cell const &to, int zone, bool arg4,
//     bool bridge, bool arg6) (0x0054F7C0)
// Whether two cells share a movement zone (TibSun Is_Same_Cell_Zone): always
// for zone type -1, for a source on the map but outside the local view, and
// (arg6) for leaving the view onto the map; else the two cells' zones
// (0x0054F8F0 with arg4 and bridge as the bridge flags) are compared.

bool MapClass::Unknown_0054F7C0(Cell const &from, Cell const &to, int zone, bool arg4, bool bridge, bool arg6)
{
    if (zone == -1) {
        return true;
    }

    bool from_local = Map.Unknown_0055A660(from, true);
    bool from_map = Map.In_Radar(from);
    if (!from_local && from_map) {
        return true;
    }

    bool to_local = Map.Unknown_0055A660(to, true);
    bool to_map = Map.In_Radar(to);
    if (arg6 && from_local && !to_local && to_map) {
        return true;
    }

    return Zone(from, zone, arg4) == Zone(to, zone, bridge);
}


// int MapClass::Zone(Cell const &cell, int zone_type, bool alt) (0x0054F8F0)
// The cell's zone for a zone type (TibSun Get_Cell_Zone): with alt, a cell under a
// bridge takes the zone of its connection's start, or of its end when the walk
// along the bridge (south or east) ends on a bridge tile that is not rock; -1
// without a connection (0x005500D0).

int MapClass::Zone(Cell const &cell, int zone_type, bool alt)
{
    Cell const *p;
    if (alt && (Map[cell].IsUnderBridge)) {
        int index = Zone_Connection_Index(cell, 1, 0);
        if (index != -1) {
            ZoneConnectionClass &connection = ZoneConnections[index];
            if (connection.unknown_08) {
                p = &connection.From;
            } else {
                CellClass *cptr = &Map[cell];
                int facing = connection.From.X == connection.To.X ? 4 : 2;
                while (cptr->IsUnderBridge) {
                    cptr = &cptr->Adjacent_Cell(facing);
                }
                if ((cptr->unknown_47b960() || cptr->unknown_47b980()) && cptr->unknown_ec != 3) {
                    p = &connection.To;
                } else {
                    p = &connection.From;
                }
            }
        } else {
            return -1;
        }
    } else {
        p = &cell;
    }
    return ((unsigned short *)unknown_18[zone_type])[Unknown_0054FA60(*p)];
}


// int MapClass::Unknown_0054FA60(Cell const &cell) (0x0054FA60)
// The +0x02 value of the +0x64 element of a cell (index clamped by
// 0x0054FAB0, expanded inline).

int MapClass::Unknown_0054FA60(Cell const &cell)
{
    return CellZones[Get_Cell_Zone_Index(cell)].unknown_02;
}


int MapClass::Get_Cell_Zone_Index(Cell const &cell)
{
    int index = Unknown_0054FAF0(cell);
    int result;
    if (index < 0) {
        result = 0;
    } else if (index >= CellZoneCount) {
        result = CellZoneCount - 1;
    } else {
        result = index;
    }
    return result;
}


// int MapClass::Unknown_0054FAF0(Cell const &cell) (0x0054FAF0)
// The cell's index in the +0x64 array without clamping:
// Y * (width + height + 1) + X.

int MapClass::Unknown_0054FAF0(Cell const &cell)
{
    return (MapSize.Height + 1 + MapSize.Width) * cell.Y + cell.X;
}


// void MapClass::Update_Cell_Zone(Cell const &cell) (0x0054FB20)
// Updates one cell's zone after its passability changed (TibSun Update_Cell_Zone):
// a passable cell next to a passable neighbour with at most three zone changes
// around it joins that neighbour's zone; otherwise the zones are rebuilt
// (0x0054EBD0). Cells outside the map (kind 7) are left alone.

void MapClass::Update_Cell_Zone(Cell const &cell)
{
    UnknownMapElem4 *cell_data = &CellZones[Get_Cell_Zone_Index(cell)];

    int stride = Map.MapSize.Width + Map.MapSize.Height + 1;
    int offsets[8] = {
        -(stride + 0),
        -(stride - 1),
        1,
        stride + 1,
        stride + 0,
        stride - 1,
        -1,
        -(stride + 1),
    };

    if (cell_data->unknown_00 != 7) {
        UnknownMapElem4 *land_neighbor = 0;
        bool has_land_neighbor = false;
        for (int i = 0; i < 8; i++) {
            land_neighbor = &cell_data[offsets[i]];
            if (land_neighbor->unknown_00 == 0) {
                has_land_neighbor = true;
                break;
            }
        }

        if (has_land_neighbor && land_neighbor != 0) {
            int transitions = 0;
            int prev_zone = 0;

            for (int j = 0; j < 8; j++) {
                UnknownMapElem4 *neighbor = &cell_data[offsets[j]];
                if (((unsigned short *)unknown_18[0])[neighbor->unknown_02] != ((unsigned short *)unknown_18[0])[prev_zone] && neighbor->unknown_00 != 7) {
                    prev_zone = neighbor->unknown_02;
                    transitions++;
                }
            }

            if (transitions <= 3 && cell_data->unknown_00 == 0) {
                cell_data->unknown_02 = land_neighbor->unknown_02;
                return;
            }
        }

        Map.Zone_Reset();
    }
}


// void MapClass::Update_Cell_Zone_Constructively(Cell const &cell) (0x0054FC60)
// Updates one cell's zone after it became passable (TibSun
// Update_Cell_Zone_Constructively): a cell next to a neighbour of the same
// passability with at most three zone changes around it joins that
// neighbour's zone; otherwise the zones are rebuilt (0x0054EBD0).

void MapClass::Update_Cell_Zone_Constructively(Cell const &cell)
{
    UnknownMapElem4 *cell_data = &CellZones[Get_Cell_Zone_Index(cell)];
    int passability = cell_data->unknown_00;

    int stride = Map.MapSize.Width + Map.MapSize.Height + 1;
    int offsets[8] = {
        -(stride + 0),
        -(stride - 1),
        1,
        stride + 1,
        stride + 0,
        stride - 1,
        -1,
        -(stride + 1),
    };

    if (passability != 7) {
        UnknownMapElem4 *land_neighbor = 0;
        bool has_land_neighbor = false;
        for (int i = 0; i < 8; i++) {
            land_neighbor = &cell_data[offsets[i]];
            if (land_neighbor->unknown_00 == passability) {
                has_land_neighbor = true;
                break;
            }
        }

        if (has_land_neighbor && land_neighbor != 0) {
            int transitions = 0;
            int prev_zone = 0;

            for (int j = 0; j < 8; j++) {
                if (((unsigned short *)unknown_18[0])[cell_data[offsets[j]].unknown_02] != ((unsigned short *)unknown_18[0])[prev_zone] && cell_data[offsets[j]].unknown_00 != 7) {
                    prev_zone = cell_data[offsets[j]].unknown_02;
                    transitions++;
                }
            }

            if (transitions <= 3) {
                cell_data->unknown_02 = land_neighbor->unknown_02;
                return;
            }
        }

        Map.Zone_Reset();
    }
}


// Not exact (RA2_NON_MATCHING): MapClass::Unknown_0054FDA0 at 0x0054FDA0, best score 62.12, 2026-11-13; differs: 249 instructions in the original, 250 in the draft (shape 92.99)
#ifdef RA2_NON_MATCHING
// void MapClass::Compute_Zone_Connections() (0x0054FDA0)
// TibSun Compute_Zone_Connections: rebuilds ZoneConnections (+0x4C) from a walk over every
// cell. A tunnel cell with tunnel cells on both sides (east and west, or south and north)
// records one connection to its tube's exit, from the lower numbered cell of the pair
// (type 1). A bridge or train bridge cell at the starting subtile of its span walks along
// the span's facing until it passes the far end subtile, then records a connection to the
// cell before the end (type 0), passable unless a cell on the way was neither a bridge nor
// under one; a walk that leaves the map first records nothing.
int __fastcall unknown_cell_index_428a90(Cell const &cell);

void MapClass::Compute_Zone_Connections()
{
    static int _bridge_subtiles1[] = {
        7, 7, -1, 7, 7, -1, 4, 4,
        4, 4,  4, 2, 2,  2, 2, 2
    };
    static int _facings[] = {
        2, 2, -1, 4, 4, -1, 2, 2,
        2, 2,  2, 4, 4,  4, 4, 4
    };
    static int _bridge_subtiles2[] = {
        -1, -1, 4, -1, -1, 2, 4, 4,
         4,  4, 4,  2,  2, 2, 2, 2
    };

    ZoneConnections.Clear();
    Reset_Iterator();
    CellClass *cptr = Iterate();

    while (cptr != 0) {

        if (!cptr->unknown_47b960() && !cptr->unknown_47b980()) {

            if (cptr->Has_Tunnel()) {
                if ((cptr->Adjacent_Cell(2).Has_Tunnel() && cptr->Adjacent_Cell(6).Has_Tunnel()) ||
                    (cptr->Adjacent_Cell(4).Has_Tunnel() && cptr->Adjacent_Cell(0).Has_Tunnel())) {

                    Cell cell = cptr->Get_Tunnel()->ExitCell;
                    if (unknown_cell_index_428a90(cptr->MapCoords) < unknown_cell_index_428a90(cell)) {
                        Cell exit = cptr->Get_Tunnel()->ExitCell;
                        ZoneConnectionClass con;
                        con.From = cptr->MapCoords;
                        con.To = exit;
                        con.unknown_08 = true;
                        con.unknown_0c = 1;
                        ZoneConnections.Add(con);
                    }
                }
            }

        } else {

            int faceindex;
            if (cptr->unknown_47b960()) {
                faceindex = cptr->IsoTileIndex - unknown_tileset_a52c74;
            } else {
                faceindex = cptr->IsoTileIndex - unknown_tileset_a6cb68;
            }

            if (_bridge_subtiles1[faceindex] == (unsigned char)cptr->unknown_11a) {

                bool crossed = false;
                bool passable = true;
                int facing = _facings[faceindex];

                for (CellClass *bptr = &cptr->Adjacent_Cell(facing); ; bptr = &bptr->Adjacent_Cell(facing)) {

                    if (In_Radar(bptr->MapCoords)) {

                        if (!crossed) {
                            int index;
                            if (bptr->unknown_47b960()) {
                                index = bptr->IsoTileIndex - unknown_tileset_a52c74;
                            } else if (bptr->unknown_47b980()) {
                                index = bptr->IsoTileIndex - unknown_tileset_a6cb68;
                            } else {
                                if (!bptr->IsUnderBridge) {
                                    passable = false;
                                }
                                continue;
                            }
                            if (index != -1 && _bridge_subtiles2[index] == (unsigned char)bptr->unknown_11a) {
                                crossed = true;
                            }
                            continue;
                        }

                    } else if (!crossed) {
                        break;
                    }

                    ZoneConnectionClass con;
                    con.From = cptr->MapCoords;
                    con.To = bptr->Adjacent_Cell((facing - 4) & 7).MapCoords;
                    con.unknown_08 = passable;
                    con.unknown_0c = 0;
                    ZoneConnections.Add(con);
                    break;
                }
            }
        }

        cptr = Iterate();
    }
}
#endif

// int MapClass::Zone_Connection_Index(Cell const &cell, int distance, int start) (0x005500D0)
// The first zone connection from start on whose +0x0C is 0 and whose
// straight segment (From..To, along Y when the X are equal, else along X)
// passes within distance of a cell; -1 when there is none.

int MapClass::Zone_Connection_Index(Cell const &cell, int distance, int start)
{
    int index = start;
    int count = ZoneConnections.Count();
    for (; index < count; index++) {
        ZoneConnectionClass &connection = ZoneConnections[index];
        if (connection.unknown_0c == 0) {
            Cell from = connection.From;
            Cell to = connection.To;
            if (from.X - to.X == 0) {
                if (cell.Y >= from.Y && cell.Y <= to.Y && abs(cell.X - from.X) <= distance) {
                    return index;
                }
            } else {
                if (cell.X <= to.X && cell.X >= from.X && abs(cell.Y - from.Y) <= distance) {
                    return index;
                }
            }
        }
    }
    return -1;
}


// bool MapClass::Unknown_005501A0(Cell const &cell) (0x005501A0)
// Runs 0x00566C30 on every zone connection within 3 of a cell that has
// +0x08 set, clearing it; when no connection is near, the connections are
// rebuilt (0x0054FDA0) and searched once more. Whether any was flagged.

bool MapClass::Unknown_005501A0(Cell const &cell)
{
    int index = Zone_Connection_Index(cell, 3, 0);
    if (index == -1) {
        Compute_Zone_Connections();
        index = Zone_Connection_Index(cell, 3, 0);
        if (index == -1) {
            return false;
        }
    }
    bool found = false;
    while (index != -1) {
        if (ZoneConnections[index].unknown_08) {
            Unknown_00566C30(&ZoneConnections[index]);
            found = true;
            ZoneConnections[index].unknown_08 = false;
        }
        index++;
        index = Zone_Connection_Index(cell, 3, index);
    }
    return found;
}


// bool MapClass::Unknown_00550230(Cell const &cell) (0x00550230)
// Every zone connection within 3 of a cell that is not flagged gets +0x08
// set, 0x00566F90 run on it and its From-To path tested (0x0054F7C0);
// when no connection is near, the connections are rebuilt (0x0054FDA0)
// and searched once more. Whether any tested path failed.

bool MapClass::Unknown_00550230(Cell const &cell)
{
    int index = Zone_Connection_Index(cell, 3, 0);
    if (index == -1) {
        Compute_Zone_Connections();
        index = Zone_Connection_Index(cell, 3, 0);
        if (index == -1) {
            return false;
        }
    }
    bool failed = false;
    while (index != -1) {
        if (!ZoneConnections[index].unknown_08) {
            ZoneConnections[index].unknown_08 = true;
            Unknown_00566F90(&ZoneConnections[index]);
            if (!Unknown_0054F7C0(ZoneConnections[index].From, ZoneConnections[index].To, 0, false, 0, false)) {
                failed = true;
            }
        }
        index++;
        index = Zone_Connection_Index(cell, 3, index);
    }
    return failed;
}


// Cell MapClass::Nearby_Location(Cell const &cell, int speed, int zone, int check, bool checkbridge,
//     Point2D dimensions, bool checkoverlay, bool checkheight, bool checkburrow, bool allowunderbridge,
//     Cell const &nearto, int unknown_scan) (0x005502E0; TibSun MapClass::Nearby_Location)
// Gathers up to 24 cells on successive square rings around `cell` that pass the placement test
// 0x00550CF0 (and optionally the height, burrow and under-bridge checks), stopping at the first ring
// with a cell the tactical view projects back onto itself; returns one at random by frame (visible
// ones first) or, given `nearto`, the closest by Fast_Sqrt distance. Draft by codex2 (99.72); exact
// under the Map unit's TPoint3D coordinate profile (RA2_COORD_TPOINT3D + RA2_COORD_CELL_BASE_INIT,
// first row 0x0042AD30), which makes CoordStruct(Cell, z) TibSun's `: Point3D(x, y, z)` (Scout1005).
extern int unknown_frame_a40d2c;

Cell MapClass::Nearby_Location(Cell const &cell, int speed, int zone, int check,
    bool checkbridge, Point2D dimensions, bool checkoverlay, bool checkheight,
    bool checkburrow, bool allowunderbridge, Cell const &nearto, int unknown_scan)
{
    Cell candidates[24];
    int count = 0;
    int xx = cell.X;
    int yy = cell.Y;
    bool found = false;
    if (zone == 0xffff) zone = -1;
    int height = Map[cell].unknown_11b;
    if (checkbridge) {
        if (Map[cell].IsUnderBridge) height += 4;
    }
    int const maxradius = MapSize.Height + MapSize.Width > 32 ? 32 : MapSize.Height + MapSize.Width;
    for (int radius = 0; radius < maxradius; ++radius) {
        int negative_radius = -radius;
        for (int x = negative_radius; x <= radius; ++x) {
            Cell newcell;
            CellClass *cellptr;
            // The existing int formal carries a scan flag observed only through its low byte.
            if ((unsigned char)unknown_scan == 0) {
                newcell = Cell(x + xx, yy - radius);
                cellptr = &Map[newcell];
                if (Unknown_0055A740(cellptr, true) && Unknown_00550CF0(newcell, dimensions.X, dimensions.Y, speed, zone, check, -1, checkbridge, checkoverlay)) {
                    if (!checkheight || abs(height - 4 * cellptr->IsUnderBridge - cellptr->unknown_11b) < 2) {
                        if ((!checkburrow || cellptr->Can_Burrow_Here()) && (allowunderbridge || !cellptr->IsUnderBridge)) {
                            candidates[count++] = newcell;
                            if (checkbridge || newcell == unknown_tactical_83996c->Coord_To_Cell(CoordStruct(newcell, 0))) {
                                found = true;
                            }
                        }
                    }
                }
            }
            if (count == 24) break;
            if ((unsigned char)unknown_scan == 0 || x > negative_radius) {
                int bottom_y = yy + radius;
                newcell = Cell(x + xx, bottom_y);
                cellptr = &Map[newcell];
                if (Unknown_0055A740(cellptr, true) && Unknown_00550CF0(newcell, dimensions.X, dimensions.Y, speed, zone, check, -1, checkbridge, checkoverlay)) {
                    if (!checkheight || abs(height - 4 * cellptr->IsUnderBridge - cellptr->unknown_11b) < 2) {
                        if ((!checkburrow || cellptr->Can_Burrow_Here()) && (allowunderbridge || !cellptr->IsUnderBridge)) {
                            candidates[count++] = newcell;
                            if (checkbridge || newcell == unknown_tactical_83996c->Coord_To_Cell(CoordStruct(newcell, 0))) {
                                found = true;
                            }
                        }
                    }
                }
            }
            if (count == 24) break;
        }
        if (count == 24) break;
        for (int y = negative_radius + 1; y <= radius - 1; ++y) {
            Cell newcell;
            CellClass *cellptr;
            if ((unsigned char)unknown_scan == 0) {
                newcell = Cell(xx - radius, yy + y);
                cellptr = &Map[newcell];
                if (Unknown_0055A740(cellptr, true) && Unknown_00550CF0(newcell, dimensions.X, dimensions.Y, speed, zone, check, -1, checkbridge, checkoverlay)) {
                    if (!checkheight || abs(height - 4 * cellptr->IsUnderBridge - cellptr->unknown_11b) < 2) {
                        if ((!checkburrow || cellptr->Can_Burrow_Here()) && (allowunderbridge || !cellptr->IsUnderBridge)) {
                            candidates[count++] = newcell;
                            if (checkbridge || newcell == unknown_tactical_83996c->Coord_To_Cell(CoordStruct(newcell, 0))) {
                                found = true;
                            }
                        }
                    }
                }
            }
            if (count == 24) break;
            newcell = Cell(xx + radius, yy + y);
            cellptr = &Map[newcell];
            if (Unknown_0055A740(cellptr, true) && Unknown_00550CF0(newcell, dimensions.X, dimensions.Y, speed, zone, check, -1, checkbridge, checkoverlay)) {
                if (!checkheight || abs(height - 4 * cellptr->IsUnderBridge - cellptr->unknown_11b) < 2) {
                    if ((!checkburrow || cellptr->Can_Burrow_Here()) && (allowunderbridge || !cellptr->IsUnderBridge)) {
                        candidates[count++] = newcell;
                        if (checkbridge || newcell == unknown_tactical_83996c->Coord_To_Cell(CoordStruct(newcell, 0))) {
                            found = true;
                        }
                    }
                }
            }
            if (count == 24) break;
        }
        if (count == 24) break;
        if (found) break;
    }
    if (count > 0) {
        Cell visible[24];
        Cell hidden[24];
        int visible_count = 0;
        int hidden_count = 0;
        int index;
        for (index = 0; index < count; ++index) {
            Cell projected = unknown_tactical_83996c->Coord_To_Cell(CoordStruct(candidates[index], 0));
            if (projected == candidates[index]) {
                visible[visible_count++] = candidates[index];
            } else {
                hidden[hidden_count++] = candidates[index];
            }
        }
        if (nearto == unknown_geometry_k) {
            if (visible_count != 0) return visible[unknown_frame_a40d2c % visible_count];
            return hidden[unknown_frame_a40d2c % hidden_count];
        }
        int total = visible_count;
        if (visible_count == 0) total = hidden_count;
        double best_distance = 100000.0;
        Cell best = unknown_geometry_k;
        for (index = 0; index < total; ++index) {
            Cell candidate = visible_count != 0 ? visible[index] : hidden[index];
            int dx = candidate.X - nearto.X;
            int dy = candidate.Y - nearto.Y;
            double distance = Fast_Sqrt((double)(dy * dy + dx * dx));
            if (distance < best_distance) {
                best_distance = distance;
                best = candidate;
            }
        }
        return best;
    }
    return unknown_geometry_k;
}


// bool MapClass::Unknown_00550CF0(Cell const &cell, int width, int height, int speed, int arg5, int arg6, int arg7, bool arg8, bool no_overlay) (0x00550CF0)
// Whether every cell of the width x height block at a cell (of the global
// Map) can be entered (CellClass 0x00478760 with speed and the other
// movement arguments) and, with no_overlay, has no overlay.

bool MapClass::Unknown_00550CF0(Cell const &cell, int width, int height, int speed, int arg5, int arg6, int arg7, bool arg8, bool no_overlay)
{
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            CellClass &object = Map[Cell(cell.X + x, cell.Y + y)];
            if (no_overlay && object.OverlayIndex != -1) {
                return false;
            }
            if (!object.Unknown_00478760(speed, false, false, arg5, arg6, arg7, arg8)) {
                return false;
            }
        }
    }
    return true;
}


// bool MapClass::Base_Region(Cell const &cell, int &house, int &zone) (0x00550DC0)
// The house whose base holds a cell (TibSun Base_Region): for a cell other than
// unknown_cell_a6f238 inside the map (0x0054AAB0), the first house in Houses not defeated
// (+0x13D) with a base centre (+0x53BC) whose zone for the cell (0x004EAEF0) is not -1;
// house takes its type's index (+0xB8), zone the zone. False when there is none.

bool MapClass::Base_Region(Cell const &cell, int &house, int &zone)
{
    if (cell != unknown_cell_a6f238 && In_Radar(cell)) {
        for (int index = 0; index < Houses.Count(); index++) {
            HouseClass *h = Houses[index];

            if (h && !h->unknown_13d && h->BaseCenter != unknown_geometry_l) {
                zone = h->Which_Zone(cell);
                if (zone != -1) {
                    house = h->Type->House;
                    return true;
                }
            }
        }
    }
    return false;
}


// void MapClass::Set_Bridge_End_State(Cell const &cell, bool damaged, bool recursive) (0x00550EB0)
// Sets a bridge end cell's damaged flag (+0x140 bit 0x2000; TibSun name): the
// first call needs a tile whose sub tile is a random variant (0x0052B730) and
// redraws the cell's screen area; a change updates the radar (0x00631570) and
// passes on to each neighbour with the same tile.

void MapClass::Set_Bridge_End_State(Cell const &cell, bool damaged, bool recursive)
{
    CellClass *cellptr = &Map[cell];
    if (!recursive) {
        if (cellptr->IsoTileIndex == 0xFFFF || cellptr->IsoTileIndex == 0xFF) {
            return;
        }
        IsometricTileTypeClass *isotype = IsoTileTypes[cellptr->IsoTileIndex];
        if (!isotype->unknown_func_52b730((unsigned char)cellptr->unknown_11a)) {
            return;
        }
        Point2D point;
        CoordStruct coord;
        coord.X = cell.X * 256 + 128;
        coord.Y = cell.Y * 256 + 128;
        coord.Z = 0;
        unknown_tactical_83996c->Coord_To_Pixel(coord, point);
        point.Y -= 15 * cellptr->unknown_11b;
        unknown_tactical_83996c->Register_Dirty_Area(Rect(point.X - 128, point.Y - 128, 256, 256), 0);
    }

    if (damaged != (bool)cellptr->IsBridgeDamaged) {
        cellptr->IsBridgeDamaged = damaged;
        int ittype = cellptr->IsoTileIndex;
        Map.Radar_Background(cellptr->MapCoords);
        int facing = 0;
        for (int i = 0; i < 8; i++) {
            Cell adjacent = Adjacent_Cell(cell, facing);
            CellClass *c = &Map[adjacent];
            if (c->IsoTileIndex == ittype) {
                Set_Bridge_End_State(adjacent, damaged, true);
            }
            facing = (facing + 1) & 7;
        }
    }
}


// void MapClass::Set_Bridge_Middle_State(Cell const &cell, int new_tile, int match_tile,
//     int cell_height, bool recursive) (0x005510A0)
// Gives a bridge middle cell a new tile (TibSun name): the first call redraws
// the cell's screen area (0x006A29D0, 0x006A3020) and stops when the tile is
// already new_tile; a changed cell recalculates (0x00472540), updates the radar
// (0x00631570) and passes the change on to each neighbour that shows the old tile.

void MapClass::Set_Bridge_Middle_State(Cell const &cell, int new_tile, int match_tile, int cell_height, bool recursive)
{
    CellClass *cellptr = &Map[cell];
    int old_tile;
    if (!recursive) {
        Point2D point;
        CoordStruct coord;
        coord.X = cell.X * 256 + 128;
        coord.Y = cell.Y * 256 + 128;
        coord.Z = 0;
        unknown_tactical_83996c->Coord_To_Pixel(coord, point);
        point.Y -= 15 * cellptr->unknown_11b;
        unknown_tactical_83996c->Register_Dirty_Area(Rect(point.X - 128, point.Y - 128, 256, 256), 0);
        old_tile = cellptr->IsoTileIndex;
        if (new_tile == old_tile) {
            return;
        }
    } else {
        old_tile = match_tile;
    }

    if (cellptr->IsoTileIndex != new_tile) {
        cellptr->IsoTileIndex = new_tile;
        cellptr->Recalc_Attributes(cell_height);
        Map.Radar_Background(cellptr->MapCoords);
        int facing = 0;
        for (int i = 0; i < 8; i++) {
            Cell adjacent = AdjacentCell[facing & 7] + cell;
            CellClass *c = &Map[adjacent];
            if (c->IsoTileIndex == old_tile) {
                Set_Bridge_Middle_State(adjacent, new_tile, old_tile, cell_height, true);
            }
            facing = (facing + 1) & 7;
        }
    }
}


// void MapClass::Internal_Damage_Train_Bridge_EW_BottomRight(Cell const &cell, int dir) (0x00551260)
// The bridge cell next to a damaged one (TibSun name): a bridge deck cell's overlay
// frame moves to its damaged stage; an end tile (set offsets
// unknown_tileset_a6e028 / a52c80) is marked damaged (0x00550EB0) and a middle tile
// (unknown_tileset_a6cb7c + 0 / 2) takes its next stage (0x005510A0).

void MapClass::Internal_Damage_Train_Bridge_EW_BottomRight(Cell const &cell, int dir)
{
    Cell adjacent = Adjacent_Cell(cell, dir);
    CellClass *adjacentptr = &Map[adjacent];
    if (adjacentptr->IsBridgeDeck) {
        if (adjacentptr->OverlayData > 3) {
            if (adjacentptr->OverlayData == 5) {
                adjacentptr->OverlayData = 6;
            }
        } else {
            adjacentptr->OverlayData = 4;
        }
    }
    int ittype = adjacentptr->IsoTileIndex - unknown_tileset_a6cb68 + 1;
    if (ittype == unknown_tileset_a6e028 || ittype == unknown_tileset_a52c80) {
        Set_Bridge_End_State(adjacent, true, false);
    } else if (ittype == unknown_tileset_a6cb7c + 0) {
        Set_Bridge_Middle_State(adjacent, unknown_tileset_a6cb68 + unknown_tileset_a6cb7c + 0, -1, -1, false);
    } else if (ittype == unknown_tileset_a6cb7c + 2) {
        Set_Bridge_Middle_State(adjacent, unknown_tileset_a6cb68 + unknown_tileset_a6cb7c + 2, -1, -1, false);
    }
}


// void MapClass::Internal_Damage_Train_Bridge_EW_TopLeft(Cell const &cell, int dir) (0x00551360)
// The bridge cell next to a damaged one (TibSun name): a bridge deck cell's overlay
// frame moves to its damaged stage; an end tile (set offsets
// unknown_tileset_a6e0e4 / a52f80) is marked damaged (0x00550EB0) and a middle tile
// (unknown_tileset_a6cb7c + 0 / 1) takes its next stage (0x005510A0).

void MapClass::Internal_Damage_Train_Bridge_EW_TopLeft(Cell const &cell, int dir)
{
    Cell adjacent = Adjacent_Cell(cell, dir);
    CellClass *adjacentptr = &Map[adjacent];
    if (adjacentptr->IsBridgeDeck) {
        if (adjacentptr->OverlayData > 3) {
            if (adjacentptr->OverlayData == 4) {
                adjacentptr->OverlayData = 6;
            }
        } else {
            adjacentptr->OverlayData = 5;
        }
    }
    int ittype = adjacentptr->IsoTileIndex - unknown_tileset_a6cb68 + 1;
    if (ittype == unknown_tileset_a6e0e4 || ittype == unknown_tileset_a52f80) {
        Set_Bridge_End_State(adjacent, true, false);
    } else if (ittype == unknown_tileset_a6cb7c + 0) {
        Set_Bridge_Middle_State(adjacent, unknown_tileset_a6cb68 + unknown_tileset_a6cb7c + 1, -1, -1, false);
    } else if (ittype == unknown_tileset_a6cb7c + 1) {
        Set_Bridge_Middle_State(adjacent, unknown_tileset_a6cb68 + unknown_tileset_a6cb7c + 2, -1, -1, false);
    }
}


// void MapClass::Internal_Destroy_Train_Bridge_EW_BottomRight(Cell const &cell, int dir) (0x00551470)
// TibSun Internal_Destroy_Train_Bridge_EW_BottomRight: the piece of an east-west train bridge
// next to `cell` toward `dir` (Adjacent_Cell, whose TPoint2D<short> sum is called
// out of line here, 0x0042ADD0): a bridge deck cell's overlay drops a stage,
// its end piece (overlay data 8) is destroyed further along, cleared
// (0x00473700, overlay -1) and redrawn on the radar, another stage becomes (data 7); an end
// tile is marked damaged (0x00550EB0), a middle tile (set
// offset unknown_tileset_a6cb7c + 0 / 2) takes stage 2, and a stage-3 middle
// tile is destroyed further along, its three bridge cells across the span dropped
// (0x00473000) and the tile set at stage 3 four levels lower.

void MapClass::Internal_Destroy_Train_Bridge_EW_BottomRight(Cell const &cell, int dir)
{
    Cell adjacent = Adjacent_Cell(cell, dir);
    CellClass *adjacentptr = &Map[adjacent];
    if (adjacentptr->IsBridgeDeck) {
        if (adjacentptr->OverlayData > 6) {
            if (adjacentptr->OverlayData == 8) {
                Internal_Destroy_Train_Bridge_EW_BottomRight(adjacent, dir);
                adjacentptr->Set_Under_Rail_Bridge(0, false);
                adjacentptr->OverlayData = 0;
                adjacentptr->OverlayIndex = -1;
                Map.Radar_Background(adjacentptr->MapCoords);
            }
        } else {
            adjacentptr->OverlayData = 7;
        }
    }
    int ittype = adjacentptr->IsoTileIndex - unknown_tileset_a6cb68 + 1;
    if (ittype != unknown_tileset_a6e028 && ittype != unknown_tileset_a52c80) {
        if (ittype == unknown_tileset_a6cb7c || ittype == unknown_tileset_a6cb7c + 2) {
            Set_Bridge_Middle_State(adjacent, unknown_tileset_a6cb68 + unknown_tileset_a6cb7c + 2, -1, -1, false);
        } else if (ittype == unknown_tileset_a6cb7c + 3) {
            Internal_Destroy_Train_Bridge_EW_BottomRight(adjacent, dir);
            if ((adjacentptr->unknown_11a & 1) != 0) {
                Map[adjacent + Cell(-1, 0)].unknown_473000();
                Map[adjacent + Cell(-1, -1)].unknown_473000();
                Map[adjacent + Cell(-1, 1)].unknown_473000();
                Set_Bridge_Middle_State(adjacent, unknown_tileset_a6cb68 + unknown_tileset_a6cb7c + 3, -1, Map[adjacent + Cell(-1, 0)].unknown_11b - 4, false);
            } else {
                Map[adjacent + Cell(0, 0)].unknown_473000();
                Map[adjacent + Cell(0, -1)].unknown_473000();
                Map[adjacent + Cell(0, 1)].unknown_473000();
                Set_Bridge_Middle_State(adjacent, unknown_tileset_a6cb68 + unknown_tileset_a6cb7c + 3, -1, Map[adjacent].unknown_11b - 4, false);
            }
        }
    } else {
        Set_Bridge_End_State(adjacent, true, false);
    }
}


// void MapClass::Internal_Destroy_Train_Bridge_EW_TopLeft(Cell const &cell, int dir) (0x00551810)
// TibSun Internal_Destroy_Train_Bridge_EW_TopLeft: the piece of an east-west train bridge
// next to `cell` toward `dir` (Adjacent_Cell, whose TPoint2D<short> sum is called
// out of line here, 0x0042ADD0): a bridge deck cell's overlay drops a stage,
// its end piece (overlay data 7) is destroyed further along, cleared
// (0x00473700, overlay -1) and redrawn on the radar, another stage becomes (data 8); an end
// tile is marked damaged (0x00550EB0), a middle tile (set
// offset unknown_tileset_a6cb7c + 0 / 1) takes stage 2, and a stage-3 middle
// tile is destroyed further along, its three bridge cells across the span dropped
// (0x00473000) and the tile set at stage 3 four levels lower.

void MapClass::Internal_Destroy_Train_Bridge_EW_TopLeft(Cell const &cell, int dir)
{
    Cell adjacent = Adjacent_Cell(cell, dir);
    CellClass *adjacentptr = &Map[adjacent];
    if (adjacentptr->IsBridgeDeck) {
        if (adjacentptr->OverlayData > 6) {
            if (adjacentptr->OverlayData == 7) {
                Internal_Destroy_Train_Bridge_EW_TopLeft(adjacent, dir);
                adjacentptr->Set_Under_Rail_Bridge(0, false);
                adjacentptr->OverlayData = 0;
                adjacentptr->OverlayIndex = -1;
                Map.Radar_Background(adjacentptr->MapCoords);
            }
        } else {
            adjacentptr->OverlayData = 8;
        }
    }
    int ittype = adjacentptr->IsoTileIndex - unknown_tileset_a6cb68 + 1;
    if (ittype != unknown_tileset_a6e0e4 && ittype != unknown_tileset_a52f80) {
        if (ittype == unknown_tileset_a6cb7c || ittype == unknown_tileset_a6cb7c + 1) {
            Set_Bridge_Middle_State(adjacent, unknown_tileset_a6cb68 + unknown_tileset_a6cb7c + 2, -1, -1, false);
        } else if (ittype == unknown_tileset_a6cb7c + 3) {
            Internal_Destroy_Train_Bridge_EW_TopLeft(adjacent, dir);
            if ((adjacentptr->unknown_11a & 1) != 0) {
                Map[adjacent + Cell(-1, 0)].unknown_473000();
                Map[adjacent + Cell(-1, -1)].unknown_473000();
                Map[adjacent + Cell(-1, 1)].unknown_473000();
                Set_Bridge_Middle_State(adjacent, unknown_tileset_a6cb68 + unknown_tileset_a6cb7c + 3, -1, Map[adjacent + Cell(-1, 0)].unknown_11b - 4, false);
            } else {
                Map[adjacent + Cell(0, 0)].unknown_473000();
                Map[adjacent + Cell(0, -1)].unknown_473000();
                Map[adjacent + Cell(0, 1)].unknown_473000();
                Set_Bridge_Middle_State(adjacent, unknown_tileset_a6cb68 + unknown_tileset_a6cb7c + 3, -1, Map[adjacent].unknown_11b - 4, false);
            }
        }
    } else {
        Set_Bridge_End_State(adjacent, true, false);
    }
}


// void MapClass::Internal_Damage_Train_Bridge_NS_BottomLeft(Cell const &cell, int dir) (0x00551BB0)
// The bridge cell next to a damaged one (TibSun name): a bridge deck cell's overlay
// frame moves to its damaged stage; an end tile (set offsets
// unknown_tileset_a6e010 / a53390) is marked damaged (0x00550EB0) and a middle tile
// (unknown_tileset_a52e70 + 0 / 2) takes its next stage (0x005510A0).

void MapClass::Internal_Damage_Train_Bridge_NS_BottomLeft(Cell const &cell, int dir)
{
    Cell adjacent = Adjacent_Cell(cell, dir);
    CellClass *adjacentptr = &Map[adjacent];
    if (adjacentptr->IsBridgeDeck) {
        if (adjacentptr->OverlayData >= 9) {
            if (adjacentptr->OverlayData > 0xC) {
                if (adjacentptr->OverlayData == 0xD) {
                    adjacentptr->OverlayData = 0xF;
                }
            } else {
                adjacentptr->OverlayData = 0xE;
            }
        }
    }
    int ittype = adjacentptr->IsoTileIndex - unknown_tileset_a6cb68 + 1;
    if (ittype == unknown_tileset_a6e010 || ittype == unknown_tileset_a53390) {
        Set_Bridge_End_State(adjacent, true, false);
    } else if (ittype == unknown_tileset_a52e70 + 0) {
        Set_Bridge_Middle_State(adjacent, unknown_tileset_a6cb68 + unknown_tileset_a52e70 + 0, -1, -1, false);
    } else if (ittype == unknown_tileset_a52e70 + 2) {
        Set_Bridge_Middle_State(adjacent, unknown_tileset_a6cb68 + unknown_tileset_a52e70 + 2, -1, -1, false);
    }
}


// void MapClass::Internal_Damage_Train_Bridge_NS_TopRight(Cell const &cell, int dir) (0x00551CC0)
// The bridge cell next to a damaged one (TibSun name): a bridge deck cell's overlay
// frame moves to its damaged stage; an end tile (set offsets
// unknown_tileset_a53394 / a52588) is marked damaged (0x00550EB0) and a middle tile
// (unknown_tileset_a52e70 + 0 / 1) takes its next stage (0x005510A0).

void MapClass::Internal_Damage_Train_Bridge_NS_TopRight(Cell const &cell, int dir)
{
    Cell adjacent = Adjacent_Cell(cell, dir);
    CellClass *adjacentptr = &Map[adjacent];
    if (adjacentptr->IsBridgeDeck) {
        if (adjacentptr->OverlayData >= 9) {
            if (adjacentptr->OverlayData > 0xC) {
                if (adjacentptr->OverlayData == 0xE) {
                    adjacentptr->OverlayData = 0xF;
                }
            } else {
                adjacentptr->OverlayData = 0xD;
            }
        }
    }
    int ittype = adjacentptr->IsoTileIndex - unknown_tileset_a6cb68 + 1;
    if (ittype == unknown_tileset_a53394 || ittype == unknown_tileset_a52588) {
        Set_Bridge_End_State(adjacent, true, false);
    } else if (ittype == unknown_tileset_a52e70 + 0) {
        Set_Bridge_Middle_State(adjacent, unknown_tileset_a6cb68 + unknown_tileset_a52e70 + 1, -1, -1, false);
    } else if (ittype == unknown_tileset_a52e70 + 1) {
        Set_Bridge_Middle_State(adjacent, unknown_tileset_a6cb68 + unknown_tileset_a52e70 + 2, -1, -1, false);
    }
}


// void MapClass::Internal_Destroy_Train_Bridge_NS_BottomLeft(Cell const &cell, int dir) (0x00551DD0)
// TibSun Internal_Destroy_Train_Bridge_NS_BottomLeft: the piece of a north-south train bridge
// next to `cell` toward `dir` (Adjacent_Cell, whose TPoint2D<short> sum is called
// out of line here, 0x0042ADD0): a bridge deck cell's overlay drops a stage,
// its end piece (overlay data 16) is destroyed further along, cleared
// (0x00473700, overlay -1) and redrawn on the radar, another stage becomes (data 17); an end
// tile is marked damaged (0x00550EB0), a middle tile (set
// offset unknown_tileset_a52e70 + 0 / 2) takes stage 2, and a stage-3 middle
// tile is destroyed further along, its three bridge cells across the span dropped
// (0x00473000) and the tile set at stage 3 four levels lower.

void MapClass::Internal_Destroy_Train_Bridge_NS_BottomLeft(Cell const &cell, int dir)
{
    Cell adjacent = Adjacent_Cell(cell, dir);
    CellClass *adjacentptr = &Map[adjacent];
    if (adjacentptr->IsBridgeDeck) {
        if (adjacentptr->OverlayData >= 9) {
            if (adjacentptr->OverlayData > 15) {
                if (adjacentptr->OverlayData == 16) {
                    Internal_Destroy_Train_Bridge_NS_BottomLeft(adjacent, dir);
                    adjacentptr->Set_Under_Rail_Bridge(6, false);
                    adjacentptr->OverlayData = 0;
                    adjacentptr->OverlayIndex = -1;
                    Map.Radar_Background(adjacentptr->MapCoords);
                }
            } else {
                adjacentptr->OverlayData = 17;
            }
        }
    }
    int ittype = adjacentptr->IsoTileIndex - unknown_tileset_a6cb68 + 1;
    if (ittype != unknown_tileset_a6e010 && ittype != unknown_tileset_a53390) {
        if (ittype == unknown_tileset_a52e70 || ittype == unknown_tileset_a52e70 + 2) {
            Set_Bridge_Middle_State(adjacent, unknown_tileset_a6cb68 + unknown_tileset_a52e70 + 2, -1, -1, false);
        } else if (ittype == unknown_tileset_a52e70 + 3) {
            Internal_Destroy_Train_Bridge_NS_BottomLeft(adjacent, dir);
            if ((unsigned char)adjacentptr->unknown_11a > 4) {
                Map[adjacent + Cell(-1, -1)].unknown_473000();
                Map[adjacent + Cell(0, -1)].unknown_473000();
                Map[adjacent + Cell(1, -1)].unknown_473000();
                Set_Bridge_Middle_State(adjacent, unknown_tileset_a6cb68 + unknown_tileset_a52e70 + 3, -1, Map[adjacent + Cell(0, -1)].unknown_11b - 4, false);
            } else {
                Map[adjacent + Cell(-1, 0)].unknown_473000();
                Map[adjacent + Cell(0, 0)].unknown_473000();
                Map[adjacent + Cell(1, 0)].unknown_473000();
                Set_Bridge_Middle_State(adjacent, unknown_tileset_a6cb68 + unknown_tileset_a52e70 + 3, -1, Map[adjacent].unknown_11b - 4, false);
            }
        }
    } else {
        Set_Bridge_End_State(adjacent, true, false);
    }
}


// void MapClass::Internal_Destroy_Train_Bridge_NS_TopRight(Cell const &cell, int dir) (0x005521A0)
// TibSun Internal_Destroy_Train_Bridge_NS_TopRight: the piece of a north-south train bridge
// next to `cell` toward `dir` (Adjacent_Cell, whose TPoint2D<short> sum is called
// out of line here, 0x0042ADD0): a bridge deck cell's overlay drops a stage,
// its end piece (overlay data 17) is destroyed further along, cleared
// (0x00473700, overlay -1) and redrawn on the radar, another stage becomes (data 16); an end
// tile is marked damaged (0x00550EB0), a middle tile (set
// offset unknown_tileset_a52e70 + 0 / 1) takes stage 2, and a stage-3 middle
// tile is destroyed further along, its three bridge cells across the span dropped
// (0x00473000) and the tile set at stage 3 four levels lower.

void MapClass::Internal_Destroy_Train_Bridge_NS_TopRight(Cell const &cell, int dir)
{
    Cell adjacent = Adjacent_Cell(cell, dir);
    CellClass *adjacentptr = &Map[adjacent];
    if (adjacentptr->IsBridgeDeck) {
        if (adjacentptr->OverlayData >= 9) {
            if (adjacentptr->OverlayData > 15) {
                if (adjacentptr->OverlayData == 17) {
                    Internal_Destroy_Train_Bridge_NS_TopRight(adjacent, dir);
                    adjacentptr->Set_Under_Rail_Bridge(6, false);
                    adjacentptr->OverlayData = 0;
                    adjacentptr->OverlayIndex = -1;
                    Map.Radar_Background(adjacentptr->MapCoords);
                }
            } else {
                adjacentptr->OverlayData = 16;
            }
        }
    }
    int ittype = adjacentptr->IsoTileIndex - unknown_tileset_a6cb68 + 1;
    if (ittype != unknown_tileset_a53394 && ittype != unknown_tileset_a52588) {
        if (ittype == unknown_tileset_a52e70 || ittype == unknown_tileset_a52e70 + 1) {
            Set_Bridge_Middle_State(adjacent, unknown_tileset_a6cb68 + unknown_tileset_a52e70 + 2, -1, -1, false);
        } else if (ittype == unknown_tileset_a52e70 + 3) {
            Internal_Destroy_Train_Bridge_NS_TopRight(adjacent, dir);
            if ((unsigned char)adjacentptr->unknown_11a > 4) {
                Map[adjacent + Cell(-1, -1)].unknown_473000();
                Map[adjacent + Cell(0, -1)].unknown_473000();
                Map[adjacent + Cell(1, -1)].unknown_473000();
                Set_Bridge_Middle_State(adjacent, unknown_tileset_a6cb68 + unknown_tileset_a52e70 + 3, -1, Map[adjacent + Cell(0, -1)].unknown_11b - 4, false);
            } else {
                Map[adjacent + Cell(-1, 0)].unknown_473000();
                Map[adjacent + Cell(0, 0)].unknown_473000();
                Map[adjacent + Cell(1, 0)].unknown_473000();
                Set_Bridge_Middle_State(adjacent, unknown_tileset_a6cb68 + unknown_tileset_a52e70 + 3, -1, Map[adjacent].unknown_11b - 4, false);
            }
        }
    } else {
        Set_Bridge_End_State(adjacent, true, false);
    }
}


// void MapClass::Unknown_00552570(Cell const &cell) (0x00552570)
// TibSun MapClass::Repair_Train_Bridge: a low bridge in the 5 x 5 block around the cell is repaired as a low
// span (0x00561100); otherwise the nearest bridge cell (the cell itself, else up to three cells out along each
// of the eight facings) gives the span start (its deck cell, or for a destroyed span the cell two steps back
// from the end of the run of was-under-bridge cells), and the span is walked repairing its end or middle
// pieces (Set_Bridge_End_State / Set_Bridge_Middle_State, Repair_Train_Bridge_Span 0x0054BE80 and the
// tactical dirty area; a raised centre piece gets its heights back, its subzones registered and its 2 x 5
// cells queued, then the neighbouring span is repaired recursively). Draft started from codex2's (45.68).
// The step back to the span start is one inline sum and one Adjacent_Cell (the original expands the first
// step's operator+ and calls the second; hypothesis for the source form).
extern int unknown_tileset_a6e0e4;
extern int unknown_tileset_a52f80;
extern int unknown_tileset_a53394;
extern int unknown_tileset_a52588;

void MapClass::Unknown_00552570(Cell const &cell)
{
    for (int x = -2; x < 3; x++) {
        for (int y = -2; y < 3; y++) {
            if (Unknown_00562930(cell + Cell(x, y))) {
                Repair_Low_Bridge_Span(cell + Cell(x, y));
                return;
            }
        }
    }

    CellClass *cellptr = &Map[cell];
    int facing = FACING_N;
    if (!cellptr->IsUnderBridge && !cellptr->WasUnderBridge) {
        for (int index = 0; index < 8; index++) {
            Cell adjacent = Adjacent_Cell(cell, Facing_Add(facing, FACING_N));
            cellptr = &Map[adjacent];
            if (cellptr->IsUnderBridge || cellptr->WasUnderBridge) {
                break;
            }
            adjacent = Adjacent_Cell(adjacent, Facing_Add(facing, FACING_N));
            cellptr = &Map[adjacent];
            if (cellptr->IsUnderBridge || cellptr->WasUnderBridge) {
                break;
            }
            adjacent = Adjacent_Cell(adjacent, Facing_Add(facing, FACING_N));
            cellptr = &Map[adjacent];
            if (cellptr->IsUnderBridge || cellptr->WasUnderBridge) {
                break;
            }
            facing = Facing_Add(facing, FACING_NE);
        }
    }

    if (!cellptr->IsUnderBridge && !cellptr->WasUnderBridge) {
        return;
    }

    Cell origin;
    if (cellptr->IsUnderBridge) {
        origin = cellptr->Get_Bridge_Deck_Cell();
    } else {
        int count = 0;
        int step = cellptr->IsBridgeEastWest ? FACING_S : FACING_E;
        origin = cellptr->Fetch_CellID();
        CellClass *walkptr;
        while (true) {
            origin = Adjacent_Cell(origin, Facing_Add(step, FACING_N));
            walkptr = &Map[origin];
            if (walkptr->WasUnderBridge) {
                if (++count >= 4) {
                    return;
                }
            } else {
                break;
            }
        }
        FacingType back = Facing_Sub(step, FACING_S);
        origin = Adjacent_Cell(origin + AdjacentCell[back & 7], back);
    }

    int advance = cellptr->IsBridgeEastWest ? FACING_W : FACING_N;
    Cell spancell = origin;
    char zonechanged = 0;
    int ittype = 0;
    DynamicVectorClass<Cell> cells;
    Rect dirty;
    while (true) {
        if (!Map.In_Radar(spancell)) {
            break;
        }
        if (Cells[spancell.X + spancell.Y * 512] != 0) {
            CellClass *spanptr = &Map[spancell];
            ittype = spanptr->IsoTileIndex - unknown_tileset_a6cb68 + 1;
            if ((ittype == unknown_tileset_a6e0e4 || ittype == unknown_tileset_a52f80) && spanptr->unknown_11a == 8) {
                Set_Bridge_End_State(spancell, false, false);
                Repair_Train_Bridge_Span_0054be80(&Map[spancell], FACING_E, &dirty);
                unknown_tactical_83996c->Register_Dirty_Area(dirty, false);
                break;
            }
            if ((ittype == unknown_tileset_a6cb7c || ittype == unknown_tileset_a6cb7c + 3 ||
                 ittype == unknown_tileset_a6cb7c + 4 || ittype == unknown_tileset_a6cb7c + 1 ||
                 ittype == unknown_tileset_a6cb7c + 2) && spanptr->unknown_11a == 5) {
                if (ittype == unknown_tileset_a6cb7c + 4) {
                    Set_Bridge_Middle_State(spancell, unknown_tileset_a6cb7c + unknown_tileset_a6cb68 - 1, -1, -1, false);
                    Cell middle = Adjacent_Cell(spancell, FACING_W);
                    Map[middle].unknown_11b += 4;
                    Map[Adjacent_Cell(middle, FACING_N)].unknown_11b += 4;
                    Map[Adjacent_Cell(middle, FACING_S)].unknown_11b += 4;
                    zonechanged = Unknown_00550230(middle);
                    for (int dx = 0; dx < 2; dx++) {
                        for (int dy = -2; dy < 3; dy++) {
                            cells.Add(Cell(middle + Cell(dx, dy)));
                        }
                    }
                }
                Unknown_00552570(spancell + Cell(-2, 0));
                Repair_Train_Bridge_Span_0054be80(&Map[spancell], FACING_E, &dirty);
                unknown_tactical_83996c->Register_Dirty_Area(dirty, false);
                break;
            }
            if ((ittype == unknown_tileset_a53394 || ittype == unknown_tileset_a52588) && spanptr->unknown_11a == 12) {
                Set_Bridge_End_State(spancell, false, false);
                Repair_Train_Bridge_Span_0054be80(&Map[spancell], FACING_S, &dirty);
                unknown_tactical_83996c->Register_Dirty_Area(dirty, false);
                break;
            }
            if ((ittype == unknown_tileset_a52e70 || ittype == unknown_tileset_a52e70 + 3 ||
                 ittype == unknown_tileset_a52e70 + 4 || ittype == unknown_tileset_a52e70 + 1 ||
                 ittype == unknown_tileset_a52e70 + 2) && spanptr->unknown_11a == 7) {
                if (ittype == unknown_tileset_a52e70 + 4) {
                    Set_Bridge_Middle_State(spancell, unknown_tileset_a52e70 + unknown_tileset_a6cb68 - 1, -1, -1, false);
                    Cell middle = Adjacent_Cell(spancell, FACING_N);
                    Map[middle].unknown_11b += 4;
                    Map[Adjacent_Cell(middle, FACING_E)].unknown_11b += 4;
                    Map[Adjacent_Cell(middle, FACING_W)].unknown_11b += 4;
                    zonechanged = Unknown_00550230(middle);
                    for (int dy = 0; dy < 2; dy++) {
                        for (int dx = -2; dx < 3; dx++) {
                            cells.Add(Cell(middle + Cell(dx, dy)));
                        }
                    }
                }
                Unknown_00552570(spancell + Cell(0, -2));
                Repair_Train_Bridge_Span_0054be80(&Map[spancell], FACING_S, &dirty);
                unknown_tactical_83996c->Register_Dirty_Area(dirty, false);
                break;
            }
        }
        spancell = Adjacent_Cell(spancell, Facing_Add(advance, FACING_N));
    }

    if (zonechanged) {
        Zone_Reset();
    }
    if (cells.Count() > 0) {
        Unknown_005688A0(cells);
    }
}


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
// From TibSun map.cpp Destroy_Train_Bridge_Span, adapted against independently bounded RA2 bytes.
bool MapClass::Destroy_Train_Bridge_Span_00553000(Cell const & cell, FacingType dir, Rect * dirty)
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

		ittype = int(cellptr->IsoTileIndex - unknown_tileset_a6cb68 + 1);

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

				if (dir == FACING_E) behindptr->Set_Under_Rail_Bridge(FACING_N, false);
                else behindptr->Set_Under_Rail_Bridge(FACING_W, false);
				behindptr->OverlayData = 0;
				behindptr->OverlayIndex = -1;
				Map.Radar_Background(behindptr->MapCoords);

				Destroy_Train_Bridge_Span_00553000(cell, dir, dirty);
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


// void MapClass::Destroy_Train_Bridge_Connections_00553570(Cell const &cell) (0x00553570)
// TibSun MapClass::Destroy_Train_Bridge_Connections: finds the first of the eight cells around `cell` that is
// (or was) under a bridge, takes the span's anchor from it (the deck cell of a live bridge, or two cells back
// from the end of a damaged one), then walks from the anchor toward the span start inside the map rectangle
// until an end or middle train-bridge piece is found and drops that span (0x00553000), registering the
// dirty area it reports.
extern int unknown_tileset_a6e0e4;
extern int unknown_tileset_a52f80;
extern int unknown_tileset_a53394;
extern int unknown_tileset_a52588;

void MapClass::Destroy_Train_Bridge_Connections_00553570(Cell const &cell)
{
    CellClass *cellptr = &unknown_dummy_cell_a6f9f8;
    int facing = FACING_N;
    for (int index = 0; index < 8; index++) {
        Cell adjacent = Adjacent_Cell(cell, facing);
        cellptr = &Map[adjacent];
        if (cellptr->IsUnderBridge || cellptr->WasUnderBridge) {
            break;
        }
        facing = Facing_Add(facing, FACING_NE);
    }

    if (!cellptr->IsUnderBridge && !cellptr->WasUnderBridge) {
        return;
    }

    Cell origin;
    if (cellptr->IsUnderBridge) {
        origin = cellptr->Get_Bridge_Deck_Cell();
    } else {
        Cell walk = cellptr->Fetch_CellID();
        int count = 0;
        int step;
        if (cellptr->IsBridgeEastWest) {
            step = FACING_S;
        } else {
            step = FACING_E;
        }
        CellClass *walkptr;
        while (true) {
            Cell next = Adjacent_Cell(walk, step);
            walk = next;
            walkptr = &Map[next];
            if (walkptr->WasUnderBridge) {
                if (++count >= 4) {
                    return;
                }
            } else {
                break;
            }
        }
        FacingType back = Facing_Sub(step, FACING_S);
        origin = Adjacent_Cell(walk + AdjacentCell[back & 7], back);
    }

    int advance = cellptr->IsBridgeEastWest ? FACING_W : FACING_N;
    Rect dirty = unknown_empty_rect;
    Cell current = origin;

    while (current.X >= unknown_120) {
        if (current.X > unknown_120 + unknown_128) {
            return;
        }
        if (current.Y < unknown_124 || current.Y > unknown_124 + unknown_12c) {
            return;
        }

        int cellnum = current.X + current.Y * 512;
        if (Cells[cellnum] != 0) {
            CellClass *spanptr = &Map[current];
            int ittype = spanptr->IsoTileIndex - unknown_tileset_a6cb68 + 1;

            if ((ittype == unknown_tileset_a6e0e4 || ittype == unknown_tileset_a52f80) && spanptr->unknown_11a == 8) {
                if (Destroy_Train_Bridge_Span_00553000(current, FACING_E, &dirty)) {
                    if (dirty != unknown_empty_rect) {
                        unknown_tactical_83996c->Register_Dirty_Area(dirty, false);
                    }
                }
                return;
            }

            if ((ittype == unknown_tileset_a6cb7c || ittype == unknown_tileset_a6cb7c + 3 ||
                 ittype == unknown_tileset_a6cb7c + 1 || ittype == unknown_tileset_a6cb7c + 2) &&
                spanptr->unknown_11a == 5) {
                if (Destroy_Train_Bridge_Span_00553000(current, FACING_E, &dirty)) {
                    if (dirty != unknown_empty_rect) {
                        unknown_tactical_83996c->Register_Dirty_Area(dirty, false);
                    }
                }
                return;
            }

            if ((ittype == unknown_tileset_a53394 || ittype == unknown_tileset_a52588) && spanptr->unknown_11a == 12) {
                if (Destroy_Train_Bridge_Span_00553000(current, FACING_S, &dirty)) {
                    if (dirty != unknown_empty_rect) {
                        unknown_tactical_83996c->Register_Dirty_Area(dirty, false);
                    }
                }
                return;
            }

            if ((ittype == unknown_tileset_a52e70 || ittype == unknown_tileset_a52e70 + 3 ||
                 ittype == unknown_tileset_a52e70 + 2 || ittype == unknown_tileset_a52e70 + 1) &&
                spanptr->unknown_11a == 7) {
                if (Destroy_Train_Bridge_Span_00553000(current, FACING_S, &dirty)) {
                    if (dirty != unknown_empty_rect) {
                        unknown_tactical_83996c->Register_Dirty_Area(dirty, false);
                    }
                }
                return;
            }
        }

        current = Adjacent_Cell(current, advance);
    }
}


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


// void MapClass::Internal_Damage_High_Bridge_EW_BottomRight(Cell const &cell, int dir) (0x00554750)
// The bridge cell next to a damaged one (TibSun name): a bridge deck cell's overlay
// frame moves to its damaged stage; an end tile (set offsets
// unknown_tileset_a6e028 / a52c80) is marked damaged (0x00550EB0) and a middle tile
// (unknown_tileset_a6cb7c + 0 / 2) takes its next stage (0x005510A0).

void MapClass::Internal_Damage_High_Bridge_EW_BottomRight(Cell const &cell, int dir)
{
    Cell adjacent = Adjacent_Cell(cell, dir);
    CellClass *adjacentptr = &Map[adjacent];
    if (adjacentptr->IsBridgeDeck) {
        if (adjacentptr->OverlayData > 3) {
            if (adjacentptr->OverlayData == 5) {
                adjacentptr->OverlayData = 6;
            }
        } else {
            adjacentptr->OverlayData = 4;
        }
    }
    int ittype = adjacentptr->IsoTileIndex - unknown_tileset_a52c74 + 1;
    if (ittype == unknown_tileset_a6e028 || ittype == unknown_tileset_a52c80) {
        Set_Bridge_End_State(adjacent, true, false);
    } else if (ittype == unknown_tileset_a6cb7c + 0) {
        Set_Bridge_Middle_State(adjacent, unknown_tileset_a52c74 + unknown_tileset_a6cb7c + 0, -1, -1, false);
    } else if (ittype == unknown_tileset_a6cb7c + 2) {
        Set_Bridge_Middle_State(adjacent, unknown_tileset_a52c74 + unknown_tileset_a6cb7c + 2, -1, -1, false);
    }
}


// void MapClass::Internal_Damage_High_Bridge_EW_TopLeft(Cell const &cell, int dir) (0x00554850)
// The bridge cell next to a damaged one (TibSun name): a bridge deck cell's overlay
// frame moves to its damaged stage; an end tile (set offsets
// unknown_tileset_a6e0e4 / a52f80) is marked damaged (0x00550EB0) and a middle tile
// (unknown_tileset_a6cb7c + 0 / 1) takes its next stage (0x005510A0).

void MapClass::Internal_Damage_High_Bridge_EW_TopLeft(Cell const &cell, int dir)
{
    Cell adjacent = Adjacent_Cell(cell, dir);
    CellClass *adjacentptr = &Map[adjacent];
    if (adjacentptr->IsBridgeDeck) {
        if (adjacentptr->OverlayData > 3) {
            if (adjacentptr->OverlayData == 4) {
                adjacentptr->OverlayData = 6;
            }
        } else {
            adjacentptr->OverlayData = 5;
        }
    }
    int ittype = adjacentptr->IsoTileIndex - unknown_tileset_a52c74 + 1;
    if (ittype == unknown_tileset_a6e0e4 || ittype == unknown_tileset_a52f80) {
        Set_Bridge_End_State(adjacent, true, false);
    } else if (ittype == unknown_tileset_a6cb7c + 0) {
        Set_Bridge_Middle_State(adjacent, unknown_tileset_a52c74 + unknown_tileset_a6cb7c + 1, -1, -1, false);
    } else if (ittype == unknown_tileset_a6cb7c + 1) {
        Set_Bridge_Middle_State(adjacent, unknown_tileset_a52c74 + unknown_tileset_a6cb7c + 2, -1, -1, false);
    }
}


// void MapClass::Internal_Destroy_High_Bridge_EW_BottomRight(Cell const &cell, int dir) (0x00554960)
// TibSun Internal_Destroy_High_Bridge_EW_BottomRight: the piece of an east-west high bridge
// next to `cell` toward `dir` (Adjacent_Cell, whose TPoint2D<short> sum is called
// out of line here, 0x0042ADD0): a bridge deck cell's overlay drops a stage,
// its end piece (overlay data 8) is destroyed further along, cleared
// (0x004732D0, overlay -1) and redrawn on the radar, another stage becomes (data 7); an end
// tile is marked damaged (0x00550EB0), a middle tile (set
// offset unknown_tileset_a6cb7c + 0 / 2) takes stage 2, and a stage-3 middle
// tile is destroyed further along, its three bridge cells across the span dropped
// (0x00473000) and the tile set at stage 3 four levels lower.

void MapClass::Internal_Destroy_High_Bridge_EW_BottomRight(Cell const &cell, int dir)
{
    Cell adjacent = Adjacent_Cell(cell, dir);
    CellClass *adjacentptr = &Map[adjacent];
    if (adjacentptr->IsBridgeDeck) {
        if (adjacentptr->OverlayData > 6) {
            if (adjacentptr->OverlayData == 8) {
                Internal_Destroy_High_Bridge_EW_BottomRight(adjacent, dir);
                adjacentptr->Set_Under_Bridge(0, false);
                adjacentptr->OverlayData = 0;
                adjacentptr->OverlayIndex = -1;
                Map.Radar_Background(adjacentptr->MapCoords);
            }
        } else {
            adjacentptr->OverlayData = 7;
        }
    }
    int ittype = adjacentptr->IsoTileIndex - unknown_tileset_a52c74 + 1;
    if (ittype != unknown_tileset_a6e028 && ittype != unknown_tileset_a52c80) {
        if (ittype == unknown_tileset_a6cb7c || ittype == unknown_tileset_a6cb7c + 2) {
            Set_Bridge_Middle_State(adjacent, unknown_tileset_a52c74 + unknown_tileset_a6cb7c + 2, -1, -1, false);
        } else if (ittype == unknown_tileset_a6cb7c + 3) {
            Internal_Destroy_High_Bridge_EW_BottomRight(adjacent, dir);
            if ((adjacentptr->unknown_11a & 1) != 0) {
                Map[adjacent + Cell(-1, 0)].unknown_473000();
                Map[adjacent + Cell(-1, -1)].unknown_473000();
                Map[adjacent + Cell(-1, 1)].unknown_473000();
                Set_Bridge_Middle_State(adjacent, unknown_tileset_a52c74 + unknown_tileset_a6cb7c + 3, -1, Map[adjacent + Cell(-1, 0)].unknown_11b - 4, false);
            } else {
                Map[adjacent + Cell(0, 0)].unknown_473000();
                Map[adjacent + Cell(0, -1)].unknown_473000();
                Map[adjacent + Cell(0, 1)].unknown_473000();
                Set_Bridge_Middle_State(adjacent, unknown_tileset_a52c74 + unknown_tileset_a6cb7c + 3, -1, Map[adjacent].unknown_11b - 4, false);
            }
        }
    } else {
        Set_Bridge_End_State(adjacent, true, false);
    }
}


// void MapClass::Internal_Destroy_High_Bridge_EW_TopLeft(Cell const &cell, int dir) (0x00554D00)
// TibSun Internal_Destroy_High_Bridge_EW_TopLeft: the piece of an east-west high bridge
// next to `cell` toward `dir` (Adjacent_Cell, whose TPoint2D<short> sum is called
// out of line here, 0x0042ADD0): a bridge deck cell's overlay drops a stage,
// its end piece (overlay data 7) is destroyed further along, cleared
// (0x004732D0, overlay -1) and redrawn on the radar, another stage becomes (data 8); an end
// tile is marked damaged (0x00550EB0), a middle tile (set
// offset unknown_tileset_a6cb7c + 0 / 1) takes stage 2, and a stage-3 middle
// tile is destroyed further along, its three bridge cells across the span dropped
// (0x00473000) and the tile set at stage 3 four levels lower.

void MapClass::Internal_Destroy_High_Bridge_EW_TopLeft(Cell const &cell, int dir)
{
    Cell adjacent = Adjacent_Cell(cell, dir);
    CellClass *adjacentptr = &Map[adjacent];
    if (adjacentptr->IsBridgeDeck) {
        if (adjacentptr->OverlayData > 6) {
            if (adjacentptr->OverlayData == 7) {
                Internal_Destroy_High_Bridge_EW_TopLeft(adjacent, dir);
                adjacentptr->Set_Under_Bridge(0, false);
                adjacentptr->OverlayData = 0;
                adjacentptr->OverlayIndex = -1;
                Map.Radar_Background(adjacentptr->MapCoords);
            }
        } else {
            adjacentptr->OverlayData = 8;
        }
    }
    int ittype = adjacentptr->IsoTileIndex - unknown_tileset_a52c74 + 1;
    if (ittype != unknown_tileset_a6e0e4 && ittype != unknown_tileset_a52f80) {
        if (ittype == unknown_tileset_a6cb7c || ittype == unknown_tileset_a6cb7c + 1) {
            Set_Bridge_Middle_State(adjacent, unknown_tileset_a52c74 + unknown_tileset_a6cb7c + 2, -1, -1, false);
        } else if (ittype == unknown_tileset_a6cb7c + 3) {
            Internal_Destroy_High_Bridge_EW_TopLeft(adjacent, dir);
            if ((adjacentptr->unknown_11a & 1) != 0) {
                Map[adjacent + Cell(-1, 0)].unknown_473000();
                Map[adjacent + Cell(-1, -1)].unknown_473000();
                Map[adjacent + Cell(-1, 1)].unknown_473000();
                Set_Bridge_Middle_State(adjacent, unknown_tileset_a52c74 + unknown_tileset_a6cb7c + 3, -1, Map[adjacent + Cell(-1, 0)].unknown_11b - 4, false);
            } else {
                Map[adjacent + Cell(0, 0)].unknown_473000();
                Map[adjacent + Cell(0, -1)].unknown_473000();
                Map[adjacent + Cell(0, 1)].unknown_473000();
                Set_Bridge_Middle_State(adjacent, unknown_tileset_a52c74 + unknown_tileset_a6cb7c + 3, -1, Map[adjacent].unknown_11b - 4, false);
            }
        }
    } else {
        Set_Bridge_End_State(adjacent, true, false);
    }
}


// void MapClass::Internal_Damage_High_Bridge_NS_BottomLeft(Cell const &cell, int dir) (0x005550A0)
// The bridge cell next to a damaged one (TibSun name): a bridge deck cell's overlay
// frame moves to its damaged stage; an end tile (set offsets
// unknown_tileset_a6e010 / a53390) is marked damaged (0x00550EB0) and a middle tile
// (unknown_tileset_a52e70 + 0 / 2) takes its next stage (0x005510A0).

void MapClass::Internal_Damage_High_Bridge_NS_BottomLeft(Cell const &cell, int dir)
{
    Cell adjacent = Adjacent_Cell(cell, dir);
    CellClass *adjacentptr = &Map[adjacent];
    if (adjacentptr->IsBridgeDeck) {
        if (adjacentptr->OverlayData >= 9) {
            if (adjacentptr->OverlayData > 0xC) {
                if (adjacentptr->OverlayData == 0xD) {
                    adjacentptr->OverlayData = 0xF;
                }
            } else {
                adjacentptr->OverlayData = 0xE;
            }
        }
    }
    int ittype = adjacentptr->IsoTileIndex - unknown_tileset_a52c74 + 1;
    if (ittype == unknown_tileset_a6e010 || ittype == unknown_tileset_a53390) {
        Set_Bridge_End_State(adjacent, true, false);
    } else if (ittype == unknown_tileset_a52e70 + 0) {
        Set_Bridge_Middle_State(adjacent, unknown_tileset_a52c74 + unknown_tileset_a52e70 + 0, -1, -1, false);
    } else if (ittype == unknown_tileset_a52e70 + 2) {
        Set_Bridge_Middle_State(adjacent, unknown_tileset_a52c74 + unknown_tileset_a52e70 + 2, -1, -1, false);
    }
}


// void MapClass::Internal_Damage_High_Bridge_NS_TopRight(Cell const &cell, int dir) (0x005551B0)
// The bridge cell next to a damaged one (TibSun name): a bridge deck cell's overlay
// frame moves to its damaged stage; an end tile (set offsets
// unknown_tileset_a53394 / a52588) is marked damaged (0x00550EB0) and a middle tile
// (unknown_tileset_a52e70 + 0 / 1) takes its next stage (0x005510A0).

void MapClass::Internal_Damage_High_Bridge_NS_TopRight(Cell const &cell, int dir)
{
    Cell adjacent = Adjacent_Cell(cell, dir);
    CellClass *adjacentptr = &Map[adjacent];
    if (adjacentptr->IsBridgeDeck) {
        if (adjacentptr->OverlayData >= 9) {
            if (adjacentptr->OverlayData > 0xC) {
                if (adjacentptr->OverlayData == 0xE) {
                    adjacentptr->OverlayData = 0xF;
                }
            } else {
                adjacentptr->OverlayData = 0xD;
            }
        }
    }
    int ittype = adjacentptr->IsoTileIndex - unknown_tileset_a52c74 + 1;
    if (ittype == unknown_tileset_a53394 || ittype == unknown_tileset_a52588) {
        Set_Bridge_End_State(adjacent, true, false);
    } else if (ittype == unknown_tileset_a52e70 + 0) {
        Set_Bridge_Middle_State(adjacent, unknown_tileset_a52c74 + unknown_tileset_a52e70 + 1, -1, -1, false);
    } else if (ittype == unknown_tileset_a52e70 + 1) {
        Set_Bridge_Middle_State(adjacent, unknown_tileset_a52c74 + unknown_tileset_a52e70 + 2, -1, -1, false);
    }
}


// void MapClass::Internal_Destroy_High_Bridge_NS_BottomLeft(Cell const &cell, int dir) (0x005552C0)
// TibSun Internal_Destroy_High_Bridge_NS_BottomLeft: the piece of a north-south high bridge
// next to `cell` toward `dir` (Adjacent_Cell, whose TPoint2D<short> sum is called
// out of line here, 0x0042ADD0): a bridge deck cell's overlay drops a stage,
// its end piece (overlay data 16) is destroyed further along, cleared
// (0x004732D0, overlay -1) and redrawn on the radar, another stage becomes (data 17); an end
// tile is marked damaged (0x00550EB0), a middle tile (set
// offset unknown_tileset_a52e70 + 0 / 2) takes stage 2, and a stage-3 middle
// tile is destroyed further along, its three bridge cells across the span dropped
// (0x00473000) and the tile set at stage 3 four levels lower.

void MapClass::Internal_Destroy_High_Bridge_NS_BottomLeft(Cell const &cell, int dir)
{
    Cell adjacent = Adjacent_Cell(cell, dir);
    CellClass *adjacentptr = &Map[adjacent];
    if (adjacentptr->IsBridgeDeck) {
        if (adjacentptr->OverlayData >= 9) {
            if (adjacentptr->OverlayData > 15) {
                if (adjacentptr->OverlayData == 16) {
                    Internal_Destroy_High_Bridge_NS_BottomLeft(adjacent, dir);
                    adjacentptr->Set_Under_Bridge(6, false);
                    adjacentptr->OverlayData = 0;
                    adjacentptr->OverlayIndex = -1;
                    Map.Radar_Background(adjacentptr->MapCoords);
                }
            } else {
                adjacentptr->OverlayData = 17;
            }
        }
    }
    int ittype = adjacentptr->IsoTileIndex - unknown_tileset_a52c74 + 1;
    if (ittype != unknown_tileset_a6e010 && ittype != unknown_tileset_a53390) {
        if (ittype == unknown_tileset_a52e70 || ittype == unknown_tileset_a52e70 + 2) {
            Set_Bridge_Middle_State(adjacent, unknown_tileset_a52c74 + unknown_tileset_a52e70 + 2, -1, -1, false);
        } else if (ittype == unknown_tileset_a52e70 + 3) {
            Internal_Destroy_High_Bridge_NS_BottomLeft(adjacent, dir);
            if ((unsigned char)adjacentptr->unknown_11a > 4) {
                Map[adjacent + Cell(-1, -1)].unknown_473000();
                Map[adjacent + Cell(0, -1)].unknown_473000();
                Map[adjacent + Cell(1, -1)].unknown_473000();
                Set_Bridge_Middle_State(adjacent, unknown_tileset_a52c74 + unknown_tileset_a52e70 + 3, -1, Map[adjacent + Cell(0, -1)].unknown_11b - 4, false);
            } else {
                Map[adjacent + Cell(-1, 0)].unknown_473000();
                Map[adjacent + Cell(0, 0)].unknown_473000();
                Map[adjacent + Cell(1, 0)].unknown_473000();
                Set_Bridge_Middle_State(adjacent, unknown_tileset_a52c74 + unknown_tileset_a52e70 + 3, -1, Map[adjacent].unknown_11b - 4, false);
            }
        }
    } else {
        Set_Bridge_End_State(adjacent, true, false);
    }
}


// void MapClass::Internal_Destroy_High_Bridge_NS_TopRight(Cell const &cell, int dir) (0x00555690)
// TibSun Internal_Destroy_High_Bridge_NS_TopRight: the piece of a north-south high bridge
// next to `cell` toward `dir` (Adjacent_Cell, whose TPoint2D<short> sum is called
// out of line here, 0x0042ADD0): a bridge deck cell's overlay drops a stage,
// its end piece (overlay data 17) is destroyed further along, cleared
// (0x004732D0, overlay -1) and redrawn on the radar, another stage becomes (data 16); an end
// tile is marked damaged (0x00550EB0), a middle tile (set
// offset unknown_tileset_a52e70 + 0 / 1) takes stage 2, and a stage-3 middle
// tile is destroyed further along, its three bridge cells across the span dropped
// (0x00473000) and the tile set at stage 3 four levels lower.

void MapClass::Internal_Destroy_High_Bridge_NS_TopRight(Cell const &cell, int dir)
{
    Cell adjacent = Adjacent_Cell(cell, dir);
    CellClass *adjacentptr = &Map[adjacent];
    if (adjacentptr->IsBridgeDeck) {
        if (adjacentptr->OverlayData >= 9) {
            if (adjacentptr->OverlayData > 15) {
                if (adjacentptr->OverlayData == 17) {
                    Internal_Destroy_High_Bridge_NS_TopRight(adjacent, dir);
                    adjacentptr->Set_Under_Bridge(6, false);
                    adjacentptr->OverlayData = 0;
                    adjacentptr->OverlayIndex = -1;
                    Map.Radar_Background(adjacentptr->MapCoords);
                }
            } else {
                adjacentptr->OverlayData = 16;
            }
        }
    }
    int ittype = adjacentptr->IsoTileIndex - unknown_tileset_a52c74 + 1;
    if (ittype != unknown_tileset_a53394 && ittype != unknown_tileset_a52588) {
        if (ittype == unknown_tileset_a52e70 || ittype == unknown_tileset_a52e70 + 1) {
            Set_Bridge_Middle_State(adjacent, unknown_tileset_a52c74 + unknown_tileset_a52e70 + 2, -1, -1, false);
        } else if (ittype == unknown_tileset_a52e70 + 3) {
            Internal_Destroy_High_Bridge_NS_TopRight(adjacent, dir);
            if ((unsigned char)adjacentptr->unknown_11a > 4) {
                Map[adjacent + Cell(-1, -1)].unknown_473000();
                Map[adjacent + Cell(0, -1)].unknown_473000();
                Map[adjacent + Cell(1, -1)].unknown_473000();
                Set_Bridge_Middle_State(adjacent, unknown_tileset_a52c74 + unknown_tileset_a52e70 + 3, -1, Map[adjacent + Cell(0, -1)].unknown_11b - 4, false);
            } else {
                Map[adjacent + Cell(-1, 0)].unknown_473000();
                Map[adjacent + Cell(0, 0)].unknown_473000();
                Map[adjacent + Cell(1, 0)].unknown_473000();
                Set_Bridge_Middle_State(adjacent, unknown_tileset_a52c74 + unknown_tileset_a52e70 + 3, -1, Map[adjacent].unknown_11b - 4, false);
            }
        }
    } else {
        Set_Bridge_End_State(adjacent, true, false);
    }
}


// void MapClass::Unknown_00555A60(Cell const &cell) (0x00555A60)
// TibSun MapClass::Repair_Bridge: the high bridge twin of Repair_Train_Bridge 0x00552570 (the second low bridge
// set 0x00562980 / 0x00561340, the bridge tile set 0x00A52C74, Repair_High_Bridge_Span 0x0054B560, and the span
// walk kept inside the map rectangle +0x120..+0x12C).
// The walk names its source cell through `Cell const &from` as the accepted twin 0x00557090 does: that one reference
// puts origin.Y first in the walk sum (`mov di,[esp+0x1a]`), and the walk-back step is Adjacent_Cell_L, offset left.
extern int unknown_tileset_a6e0e4;
extern int unknown_tileset_a52f80;
extern int unknown_tileset_a53394;
extern int unknown_tileset_a52588;

void MapClass::Unknown_00555A60(Cell const &cell)
{
    for (int x = -2; x < 3; x++) {
        for (int y = -2; y < 3; y++) {
            if (Unknown_00562980(cell + Cell(x, y))) {
                Repair_Low_Bridge2_Span(cell + Cell(x, y));
                return;
            }
        }
    }

    CellClass *cellptr = &Map[cell];
    int facing = FACING_N;
    if (!cellptr->IsUnderBridge && !cellptr->WasUnderBridge) {
        for (int index = 0; index < 8; index++) {
            Cell adjacent = Adjacent_Cell(cell, Facing_Add(facing, FACING_N));
            cellptr = &Map[adjacent];
            if (cellptr->IsUnderBridge || cellptr->WasUnderBridge) {
                break;
            }
            adjacent = Adjacent_Cell(adjacent, Facing_Add(facing, FACING_N));
            cellptr = &Map[adjacent];
            if (cellptr->IsUnderBridge || cellptr->WasUnderBridge) {
                break;
            }
            adjacent = Adjacent_Cell(adjacent, Facing_Add(facing, FACING_N));
            cellptr = &Map[adjacent];
            if (cellptr->IsUnderBridge || cellptr->WasUnderBridge) {
                break;
            }
            facing = Facing_Add(facing, FACING_NE);
        }
    }

    if (!cellptr->IsUnderBridge && !cellptr->WasUnderBridge) {
        return;
    }

    Cell origin;
    if (cellptr->IsUnderBridge) {
        origin = cellptr->Get_Bridge_Deck_Cell();
    } else {
        origin = cellptr->Fetch_CellID();
        int count = 0;
        int step;
        if (cellptr->IsBridgeEastWest) {
            step = FACING_S;
        } else {
            step = FACING_E;
        }
        CellClass *walkptr;
        while (true) {
            Cell const &from = origin;
            origin = Adjacent_Cell_L(from, step);
            walkptr = &Map[origin];
            if (walkptr->WasUnderBridge) {
                if (++count >= 4) {
                    return;
                }
            } else {
                break;
            }
        }
        FacingType back = Facing_Sub(step, FACING_S);
        origin = Adjacent_Cell_L(origin + AdjacentCell[back & 7], back);
    }

    int advance;
    if (cellptr->IsBridgeEastWest) {
        advance = FACING_W;
    } else {
        advance = FACING_N;
    }
    Cell spancell = origin;
    int ittype = 0;
    DynamicVectorClass<Cell> cells;
    char zonechanged = 0;
    Rect dirty;
    if (spancell.X >= unknown_120) {
    while (spancell.X >= unknown_120) {
        if (spancell.X > unknown_120 + unknown_128) {
            break;
        }
        if (spancell.Y < unknown_124 || spancell.Y > unknown_124 + unknown_12c) {
            break;
        }
        if (Has_Cell(spancell)) {
            CellClass *spanptr = &Map[spancell];
            Cell middle;
            ittype = spanptr->IsoTileIndex - unknown_tileset_a52c74 + 1;
            if ((ittype == unknown_tileset_a6e0e4 || ittype == unknown_tileset_a52f80) && spanptr->unknown_11a == 8) {
                Set_Bridge_End_State(spancell, false, false);
                Repair_High_Bridge_Span_0054b560(&Map[spancell], FACING_E, &dirty);
                unknown_tactical_83996c->Register_Dirty_Area(dirty, false);
                break;
            }
            if ((ittype == unknown_tileset_a6cb7c || ittype == unknown_tileset_a6cb7c + 3 ||
                 ittype == unknown_tileset_a6cb7c + 4 || ittype == unknown_tileset_a6cb7c + 1 ||
                 ittype == unknown_tileset_a6cb7c + 2) && spanptr->unknown_11a == 5) {
                if (ittype == unknown_tileset_a6cb7c + 4) {
                    Set_Bridge_Middle_State(spancell, unknown_tileset_a6cb7c + unknown_tileset_a52c74 - 1, -1, -1, false);
                    middle = Adjacent_Cell(spancell, FACING_W);
                    Map[middle].unknown_11b += 4;
                    Map[Adjacent_Cell(middle, FACING_N)].unknown_11b += 4;
                    Map[Adjacent_Cell(middle, FACING_S)].unknown_11b += 4;
                    zonechanged = Unknown_00550230(middle);
                    for (int dx = 0; dx < 2; dx++) {
                        for (int dy = -2; dy < 3; dy++) {
                            cells.Add(Cell(middle + Cell(dx, dy)));
                        }
                    }
                }
                Unknown_00555A60(spancell + Cell(-2, 0));
                Repair_High_Bridge_Span_0054b560(&Map[spancell], FACING_E, &dirty);
                unknown_tactical_83996c->Register_Dirty_Area(dirty, false);
                break;
            }
            if ((ittype == unknown_tileset_a53394 || ittype == unknown_tileset_a52588) && spanptr->unknown_11a == 12) {
                Set_Bridge_End_State(spancell, false, false);
                Repair_High_Bridge_Span_0054b560(&Map[spancell], FACING_S, &dirty);
                unknown_tactical_83996c->Register_Dirty_Area(dirty, false);
                break;
            }
            if ((ittype == unknown_tileset_a52e70 || ittype == unknown_tileset_a52e70 + 3 ||
                 ittype == unknown_tileset_a52e70 + 4 || ittype == unknown_tileset_a52e70 + 1 ||
                 ittype == unknown_tileset_a52e70 + 2) && spanptr->unknown_11a == 7) {
                if (ittype == unknown_tileset_a52e70 + 4) {
                    Set_Bridge_Middle_State(spancell, unknown_tileset_a52e70 + unknown_tileset_a52c74 - 1, -1, -1, false);
                    middle = Adjacent_Cell(spancell, FACING_N);
                    Map[middle].unknown_11b += 4;
                    Map[Adjacent_Cell(middle, FACING_E)].unknown_11b += 4;
                    Map[Adjacent_Cell(middle, FACING_W)].unknown_11b += 4;
                    zonechanged = Unknown_00550230(middle);
                    for (int dy = 0; dy < 2; dy++) {
                        for (int dx = -2; dx < 3; dx++) {
                            cells.Add(Cell(middle + Cell(dx, dy)));
                        }
                    }
                }
                Unknown_00555A60(spancell + Cell(0, -2));
                Repair_High_Bridge_Span_0054b560(&Map[spancell], FACING_S, &dirty);
                unknown_tactical_83996c->Register_Dirty_Area(dirty, false);
                break;
            }
        }
        spancell = Adjacent_Cell(spancell, advance);
    }
    }

    if (zonechanged) {
        Zone_Reset();
    }
    if (cells.Count() > 0) {
        Unknown_005688A0(cells);
    }
}


// void MapClass::Unknown_00556520(Cell const &cell) (0x00556520)
// The twin of 0x00557090 for the second low bridge set (0x00562980, Destroy_Low_Bridge2_Span, tile set 0x00A52C74).
void MapClass::Unknown_00556520(Cell const &cell)
{
    for (int x = -2; x < 3; ++x) {
        for (int y = -2; y < 3; ++y) {
            if (Unknown_00562980(cell + Cell(x, y))) {
                Destroy_Low_Bridge2_Span(cell + Cell(x, y));
                return;
            }
        }
    }
    CellClass *cellptr = &Map[cell];
    int step;
    int facing = 0;
    if (!cellptr->IsUnderBridge && !cellptr->WasUnderBridge) {
        for (int index = 0; index < 8; ++index) {
            Cell adjacent = Adjacent_Cell(cell, facing);
            cellptr = &Map[adjacent];
            if (cellptr->IsUnderBridge || cellptr->WasUnderBridge) break;
            Cell adjacent2 = Adjacent_Cell(adjacent, facing);
            cellptr = &Map[adjacent2];
            if (cellptr->IsUnderBridge || cellptr->WasUnderBridge) break;
            adjacent = Adjacent_Cell(adjacent2, facing);
            cellptr = &Map[adjacent];
            if (cellptr->IsUnderBridge || cellptr->WasUnderBridge) break;
            facing = (facing + 1) & 7;
        }
    }
    if (!cellptr->IsUnderBridge && !cellptr->WasUnderBridge) return;
    Cell origin;
    if (cellptr->IsUnderBridge) {
        if (cellptr->IsBridgeDeck) origin = cellptr->MapCoords;
        else origin = cellptr->unknown_2c->MapCoords;
    } else {
        Cell walk = cellptr->Fetch_CellID();
        int count = 0;
        
        if (cellptr->IsBridgeEastWest) step = 4;
        else step = 2;
        CellClass *walkptr;
        while (true) {
            Cell const &from = walk; walk = Adjacent_Cell_L(from, step);
            walkptr = &Map[walk];
            if (walkptr->WasUnderBridge) {
                if (++count >= 4) return;
            } else {
                break;
            }
        }
        int back = (step - 4) & 7;
        origin = (AdjacentCell[back & 7] + walk) + AdjacentCell[back & 7];
    }
    
    int shared_direction = cellptr->IsBridgeEastWest ? 6 : 0;
    Cell spancell = origin;
    DynamicVectorClass<Cell> cells;
    int tile;
    CellClass *spanptr;
    if (spancell.X >= unknown_120) {
        while (spancell.X >= unknown_120) {
            if (spancell.X > unknown_120 + unknown_128) break;
            if (spancell.Y < unknown_124 || spancell.Y > unknown_124 + unknown_12c) break;
            if (Cells[spancell.X + spancell.Y * 512] != 0) {
                spanptr = &Map[spancell];
                tile = spanptr->IsoTileIndex - unknown_tileset_a52c74 + 1;
                if (((tile == unknown_tileset_a6e0e4 || tile == unknown_tileset_a52f80) && spanptr->unknown_11a == 8) ||
                    ((tile == unknown_tileset_a53394 || tile == unknown_tileset_a52588) && spanptr->unknown_11a == 12)) {
                    goto destroy_span;
                }
            }
            spancell = Adjacent_Cell(spancell, shared_direction);
        }
    }
    goto finished;

destroy_span:
    {
        step = (shared_direction - 4) & 7;
        Cell end = spancell;
        int hits;
        for (hits = 0; hits < 3; ++hits) {
            if (Unknown_00569090(&spancell)) break;
        }
        while (true) {
            end = Adjacent_Cell(end, step);
            spanptr = &Map[end];
            if (end.X < unknown_120 || end.X > unknown_120 + unknown_128 ||
                end.Y < unknown_124 || end.Y > unknown_124 + unknown_12c)
                goto destroy_connections;
            tile = spanptr->IsoTileIndex - unknown_tileset_a52c74 + 1;
            if (Unknown_00556B30(tile, step, spanptr)) break;
        }
        if (tile != -1) {
            end = end + AdjacentCell[(step - 4) & 7];
            for (hits = 0; hits < 3; ++hits) {
                if (Unknown_00569090(&end)) break;
            }
        }
destroy_connections:
        Destroy_High_Bridge_Connections_00558BD0(spancell);
        unknown_tactical_83996c->unknown_d7c = true;
    }

finished:
    Zone_Reset();
}


// bool __fastcall Unknown_00556B30(int tile, int kind, CellClass const *cell) (0x00556B30)
// Whether a tile is a bridge piece of direction kind 2 or 4 (one tile of two
// sets, or one of four consecutive tiles of a third) on a cell whose +0x11A
// is the other kind (4 for kind 2, 2 for kind 4) (called by 0x00556520 and 0x00557090; name hypothesis).

bool __fastcall Unknown_00556B30(int tile, int kind, CellClass const *cell)
{
    if (kind == 2) {
        if (tile == unknown_tileset_a6e028 && cell->unknown_11a == 4) {
            return true;
        }
        if (tile == unknown_tileset_a52c80 && cell->unknown_11a == 4) {
            return true;
        }
        if ((tile == unknown_tileset_a6cb7c || tile == unknown_tileset_a6cb7c + 3
                || tile == unknown_tileset_a6cb7c + 1 || tile == unknown_tileset_a6cb7c + 2)
                && cell->unknown_11a == 4) {
            return true;
        }
    } else if (kind == 4) {
        if (tile == unknown_tileset_a6e010 && cell->unknown_11a == 2) {
            return true;
        }
        if (tile == unknown_tileset_a53390 && cell->unknown_11a == 2) {
            return true;
        }
        if ((tile == unknown_tileset_a52e70 || tile == unknown_tileset_a52e70 + 3
                || tile == unknown_tileset_a52e70 + 1 || tile == unknown_tileset_a52e70 + 2)
                && cell->unknown_11a == 2) {
            return true;
        }
    }
    return false;
}


// void MapClass::Destroy_Low_Bridge_Span(Cell const &cell) (0x00556BF0)
// Tiberian Sun's Repair_Low_Bridge_Span shape: for a low bridge overlay, finds
// the span's end cell next to `cell` (through the low bridge test 0x00562930)
// and destroys the EW or NS span from there. The differences are
// TPoint2D<short>::operator- (the last one is the Map unit's out-of-line copy
// 0x0056AA40) and operator+.

void MapClass::Destroy_Low_Bridge_Span(Cell const &cell)
{
    int overlay = Map[cell].OverlayIndex;

    if (overlay >= 0x4a && overlay <= 0x52 || overlay >= 0x5c && overlay <= 0x5f || overlay == 0x64) {
        if (!Unknown_00562930(cell - Cell(0, 1))) {
            Destroy_Low_Bridge_EW(cell + Cell(0, 1));
        } else {
            if (!Unknown_00562930(cell - Cell(0, 2))) {
                Destroy_Low_Bridge_EW(cell);
            } else {
                Destroy_Low_Bridge_EW(cell - Cell(0, 1));
            }
        }
    } else if (overlay >= 0x53 && overlay <= 0x5b || overlay >= 0x60 && overlay <= 0x63 || overlay == 0x65) {
        if (!Unknown_00562930(cell - Cell(1, 0))) {
            Destroy_Low_Bridge_NS(cell + Cell(1, 0));
        } else {
            if (!Unknown_00562930(cell - Cell(2, 0))) {
                Destroy_Low_Bridge_NS(cell);
            } else {
                Destroy_Low_Bridge_NS(cell - Cell(1, 0));
            }
        }
    }
}


// void MapClass::Destroy_Low_Bridge2_Span(Cell const &cell) (0x00556E30)
// Tiberian Sun's Repair_Low_Bridge_Span shape for the second low bridge set: for its overlay, finds
// the span's end cell next to `cell` (through the low bridge test 0x00562980)
// and destroys the EW or NS span from there. The differences are
// TPoint2D<short>::operator- (the last one is the Map unit's out-of-line copy
// 0x0056AA40) and operator+.

void MapClass::Destroy_Low_Bridge2_Span(Cell const &cell)
{
    int overlay = Map[cell].OverlayIndex;

    if (overlay >= 0xcd && overlay <= 0xd5 || overlay >= 0xdf && overlay <= 0xe2 || overlay == 0xe7) {
        if (!Unknown_00562980(cell - Cell(0, 1))) {
            Destroy_Low_Bridge2_EW(cell + Cell(0, 1));
        } else {
            if (!Unknown_00562980(cell - Cell(0, 2))) {
                Destroy_Low_Bridge2_EW(cell);
            } else {
                Destroy_Low_Bridge2_EW(cell - Cell(0, 1));
            }
        }
    } else if (overlay >= 0xd6 && overlay <= 0xde || overlay >= 0xe3 && overlay <= 0xe6 || overlay == 0xe8) {
        if (!Unknown_00562980(cell - Cell(1, 0))) {
            Destroy_Low_Bridge2_NS(cell + Cell(1, 0));
        } else {
            if (!Unknown_00562980(cell - Cell(2, 0))) {
                Destroy_Low_Bridge2_NS(cell);
            } else {
                Destroy_Low_Bridge2_NS(cell - Cell(1, 0));
            }
        }
    }
}


// void MapClass::Unknown_00557090(Cell const &cell) (0x00557090)
// Destroys the high bridge span near a cell (RA2 only; 0x00556520 is its twin for the second low bridge set).
// The span advance stands before the destroy block in the source: inline sites number in source order, so the
// goto layout is what puts the end-walk sums on the original operand order (docs/VC6_MATCHING_NOTES, Cell sum tie).
void MapClass::Unknown_00557090(Cell const &cell)
{
    for (int x = -2; x < 3; ++x) {
        for (int y = -2; y < 3; ++y) {
            if (Unknown_00562930(cell + Cell(x, y))) {
                Destroy_Low_Bridge_Span(cell + Cell(x, y));
                return;
            }
        }
    }
    CellClass *cellptr = &Map[cell];
    int step;
    int facing = 0;
    if (!cellptr->IsUnderBridge && !cellptr->WasUnderBridge) {
        for (int index = 0; index < 8; ++index) {
            Cell adjacent = Adjacent_Cell(cell, facing);
            cellptr = &Map[adjacent];
            if (cellptr->IsUnderBridge || cellptr->WasUnderBridge) break;
            Cell adjacent2 = Adjacent_Cell(adjacent, facing);
            cellptr = &Map[adjacent2];
            if (cellptr->IsUnderBridge || cellptr->WasUnderBridge) break;
            adjacent = Adjacent_Cell(adjacent2, facing);
            cellptr = &Map[adjacent];
            if (cellptr->IsUnderBridge || cellptr->WasUnderBridge) break;
            facing = (facing + 1) & 7;
        }
    }
    if (!cellptr->IsUnderBridge && !cellptr->WasUnderBridge) return;
    Cell origin;
    if (cellptr->IsUnderBridge) {
        if (cellptr->IsBridgeDeck) origin = cellptr->MapCoords;
        else origin = cellptr->unknown_2c->MapCoords;
    } else {
        Cell walk = cellptr->Fetch_CellID();
        int count = 0;
        
        if (cellptr->IsBridgeEastWest) step = 4;
        else step = 2;
        CellClass *walkptr;
        while (true) {
            Cell const &from = walk; walk = Adjacent_Cell_L(from, step);
            walkptr = &Map[walk];
            if (walkptr->WasUnderBridge) {
                if (++count >= 4) return;
            } else {
                break;
            }
        }
        int back = (step - 4) & 7;
        origin = (AdjacentCell[back & 7] + walk) + AdjacentCell[back & 7];
    }
    
    int shared_direction = cellptr->IsBridgeEastWest ? 6 : 0;
    Cell spancell = origin;
    DynamicVectorClass<Cell> cells;
    int tile;
    CellClass *spanptr;
    if (spancell.X >= unknown_120) {
        while (spancell.X >= unknown_120) {
            if (spancell.X > unknown_120 + unknown_128) break;
            if (spancell.Y < unknown_124 || spancell.Y > unknown_124 + unknown_12c) break;
            if (Cells[spancell.X + spancell.Y * 512] != 0) {
                spanptr = &Map[spancell];
                tile = spanptr->IsoTileIndex - unknown_tileset_a6cb68 + 1;
                if (((tile == unknown_tileset_a6e0e4 || tile == unknown_tileset_a52f80) && spanptr->unknown_11a == 8) ||
                    ((tile == unknown_tileset_a53394 || tile == unknown_tileset_a52588) && spanptr->unknown_11a == 12)) {
                    goto destroy_span;
                }
            }
            spancell = Adjacent_Cell(spancell, shared_direction);
        }
    }
    goto finished;

destroy_span:
    {
        step = (shared_direction - 4) & 7;
        Cell end = spancell;
        int hits;
        for (hits = 0; hits < 3; ++hits) {
            if (Unknown_00569090(&spancell)) break;
        }
        while (true) {
            end = Adjacent_Cell(end, step);
            spanptr = &Map[end];
            if (end.X < unknown_120 || end.X > unknown_120 + unknown_128 ||
                end.Y < unknown_124 || end.Y > unknown_124 + unknown_12c)
                goto destroy_connections;
            tile = spanptr->IsoTileIndex - unknown_tileset_a6cb68 + 1;
            if (Unknown_00556B30(tile, step, spanptr)) break;
        }
        if (tile != -1) {
            end = end + AdjacentCell[(step - 4) & 7];
            for (hits = 0; hits < 3; ++hits) {
                if (Unknown_00569090(&end)) break;
            }
        }
destroy_connections:
        Destroy_High_Bridge_Connections_00558BD0(spancell);
        unknown_tactical_83996c->unknown_d7c = true;
    }

finished:
    Zone_Reset();
}


// void MapClass::Destroy_Low_Bridge_EW(Cell const &cell) (0x005576A0)
// Blows up the east-west low bridge run through a cell (first low bridge overlays
// 0x4A..0x65, Is_Low_Bridge 0x00562930): the run is measured both ways along X, the walk
// starts at its middle and steps toward the longer end (dir) for up to 4 cells while the
// bridge continues. At each cell not already wrecked (overlay 0x64) three random
// Rules->BridgeExplosions go off at random spots of the cells Y-1..Y+1 (at the bridge
// height), then the cell's low bridge is damaged (0x0055DCA0) up to three times until it
// gives way. Then 0x0054EBD0 and the tactical view's +0xD7C redraw flag.
// The coordinate is built member-wise with a Z = 0 store before the height (the
// original keeps that dead store as an immediate; Cell_Coord's inline would hoist the 0
// into a register).

void MapClass::Destroy_Low_Bridge_EW(Cell const &cell)
{
    Cell cellnum = cell;
    Cell center = cellnum;
    int dir = 1;
    int left = 0;
    int right = 0;

    do {
        cellnum.X--;
        left++;
    } while (Unknown_00562930(cellnum));

    cellnum = center;
    do {
        cellnum.X++;
        right++;
    } while (Unknown_00562930(cellnum));

    if (right < left) {
        dir = -1;
    }

    cellnum = center;
    cellnum.X -= (left - right) / 2;

    int count = 4;
    while (true) {
        if (count <= 0) {
            break;
        }
        if (Map[cellnum].OverlayIndex != 0x64) {
            Cell piece = Cell(cellnum.X, cellnum.Y - 1);
            for (int index = 3; index > 0; index--) {
                CellClass *cellptr = &Map[piece];
                Cell id = cellptr->Fetch_CellID();
                CoordStruct coord;
                coord.X = id.X * 256 + 128;
                coord.Y = id.Y * 256 + 128;
                coord.Z = 0;
                coord.Z = cellptr->unknown_11b * unknown_geometry_g;
                coord.X += (Random_Pick(0, 0x7ffffffe) * 4.656612877414201e-10 - 0.5) * 50.0;
                coord.Y += (Random_Pick(0, 0x7ffffffe) * 4.656612877414201e-10 - 0.5) * 50.0;
                new AnimClass(Rules->BridgeExplosions[Random_Pick(0, Rules->BridgeExplosions.Count() - 1)], coord, Random_Pick(1, 5), 1, 0x600, 0);
                piece = Cell(piece.X, piece.Y + 1);
            }
        }

        for (int tries = 0; tries < 3; tries++) {
            if (Unknown_0055DCA0(cellnum)) {
                break;
            }
        }

        cellnum.X += dir;
        count--;
        if (!Unknown_00562930(cellnum)) {
            break;
        }
    }

    Zone_Reset();
    unknown_tactical_83996c->unknown_d7c = true;
}


// void MapClass::Destroy_Low_Bridge_NS(Cell const &cell) (0x005579C0)
// The north-south counterpart of Destroy_Low_Bridge_EW (0x005576A0): the run is measured
// along Y, the explosions go off around the cells X-1..X+1 and the wrecked overlay is 0x65.

void MapClass::Destroy_Low_Bridge_NS(Cell const &cell)
{
    Cell cellnum = cell;
    Cell center = cellnum;
    int dir = 1;
    int left = 0;
    int right = 0;

    do {
        cellnum.Y--;
        left++;
    } while (Unknown_00562930(cellnum));

    cellnum = center;
    do {
        cellnum.Y++;
        right++;
    } while (Unknown_00562930(cellnum));

    if (right < left) {
        dir = -1;
    }

    cellnum = center;
    cellnum.Y -= (left - right) / 2;

    int count = 4;
    while (true) {
        if (count <= 0) {
            break;
        }
        if (Map[cellnum].OverlayIndex != 0x65) {
            Cell piece = Cell(cellnum.X - 1, cellnum.Y);
            for (int index = 3; index > 0; index--) {
                CellClass *cellptr = &Map[piece];
                Cell id = cellptr->Fetch_CellID();
                CoordStruct coord;
                coord.X = id.X * 256 + 128;
                coord.Y = id.Y * 256 + 128;
                coord.Z = 0;
                coord.Z = cellptr->unknown_11b * unknown_geometry_g;
                coord.X += (Random_Pick(0, 0x7ffffffe) * 4.656612877414201e-10 - 0.5) * 50.0;
                coord.Y += (Random_Pick(0, 0x7ffffffe) * 4.656612877414201e-10 - 0.5) * 50.0;
                new AnimClass(Rules->BridgeExplosions[Random_Pick(0, Rules->BridgeExplosions.Count() - 1)], coord, Random_Pick(1, 5), 1, 0x600, 0);
                piece = Cell(piece.X + 1, piece.Y);
            }
        }

        for (int tries = 0; tries < 3; tries++) {
            if (Unknown_0055DCA0(cellnum)) {
                break;
            }
        }

        cellnum.Y += dir;
        count--;
        if (!Unknown_00562930(cellnum)) {
            break;
        }
    }

    Zone_Reset();
    unknown_tactical_83996c->unknown_d7c = true;
}


// void MapClass::Destroy_Low_Bridge2_EW(Cell const &cell) (0x00557CE0)
// Destroy_Low_Bridge_EW (0x005576A0) for the second low bridge overlays 0xCD..0xE8
// (0x00562980; wrecked 0xE7; damaged by 0x0055EDB0).

void MapClass::Destroy_Low_Bridge2_EW(Cell const &cell)
{
    Cell cellnum = cell;
    Cell center = cellnum;
    int dir = 1;
    int left = 0;
    int right = 0;

    do {
        cellnum.X--;
        left++;
    } while (Unknown_00562980(cellnum));

    cellnum = center;
    do {
        cellnum.X++;
        right++;
    } while (Unknown_00562980(cellnum));

    if (right < left) {
        dir = -1;
    }

    cellnum = center;
    cellnum.X -= (left - right) / 2;

    int count = 4;
    while (true) {
        if (count <= 0) {
            break;
        }
        if (Map[cellnum].OverlayIndex != 0xE7) {
            Cell piece = Cell(cellnum.X, cellnum.Y - 1);
            for (int index = 3; index > 0; index--) {
                CellClass *cellptr = &Map[piece];
                Cell id = cellptr->Fetch_CellID();
                CoordStruct coord;
                coord.X = id.X * 256 + 128;
                coord.Y = id.Y * 256 + 128;
                coord.Z = 0;
                coord.Z = cellptr->unknown_11b * unknown_geometry_g;
                coord.X += (Random_Pick(0, 0x7ffffffe) * 4.656612877414201e-10 - 0.5) * 50.0;
                coord.Y += (Random_Pick(0, 0x7ffffffe) * 4.656612877414201e-10 - 0.5) * 50.0;
                new AnimClass(Rules->BridgeExplosions[Random_Pick(0, Rules->BridgeExplosions.Count() - 1)], coord, Random_Pick(1, 5), 1, 0x600, 0);
                piece = Cell(piece.X, piece.Y + 1);
            }
        }

        for (int tries = 0; tries < 3; tries++) {
            if (Unknown_0055EDB0(cellnum)) {
                break;
            }
        }

        cellnum.X += dir;
        count--;
        if (!Unknown_00562980(cellnum)) {
            break;
        }
    }

    Zone_Reset();
    unknown_tactical_83996c->unknown_d7c = true;
}


// void MapClass::Destroy_Low_Bridge2_NS(Cell const &cell) (0x00558010)
// Destroy_Low_Bridge_NS (0x005579C0) for the second low bridge overlays 0xCD..0xE8
// (0x00562980; wrecked 0xE8; damaged by 0x0055EDB0).

void MapClass::Destroy_Low_Bridge2_NS(Cell const &cell)
{
    Cell cellnum = cell;
    Cell center = cellnum;
    int dir = 1;
    int left = 0;
    int right = 0;

    do {
        cellnum.Y--;
        left++;
    } while (Unknown_00562980(cellnum));

    cellnum = center;
    do {
        cellnum.Y++;
        right++;
    } while (Unknown_00562980(cellnum));

    if (right < left) {
        dir = -1;
    }

    cellnum = center;
    cellnum.Y -= (left - right) / 2;

    int count = 4;
    while (true) {
        if (count <= 0) {
            break;
        }
        if (Map[cellnum].OverlayIndex != 0xE8) {
            Cell piece = Cell(cellnum.X - 1, cellnum.Y);
            for (int index = 3; index > 0; index--) {
                CellClass *cellptr = &Map[piece];
                Cell id = cellptr->Fetch_CellID();
                CoordStruct coord;
                coord.X = id.X * 256 + 128;
                coord.Y = id.Y * 256 + 128;
                coord.Z = 0;
                coord.Z = cellptr->unknown_11b * unknown_geometry_g;
                coord.X += (Random_Pick(0, 0x7ffffffe) * 4.656612877414201e-10 - 0.5) * 50.0;
                coord.Y += (Random_Pick(0, 0x7ffffffe) * 4.656612877414201e-10 - 0.5) * 50.0;
                new AnimClass(Rules->BridgeExplosions[Random_Pick(0, Rules->BridgeExplosions.Count() - 1)], coord, Random_Pick(1, 5), 1, 0x600, 0);
                piece = Cell(piece.X + 1, piece.Y);
            }
        }

        for (int tries = 0; tries < 3; tries++) {
            if (Unknown_0055EDB0(cellnum)) {
                break;
            }
        }

        cellnum.Y += dir;
        count--;
        if (!Unknown_00562980(cellnum)) {
            break;
        }
    }

    Zone_Reset();
    unknown_tactical_83996c->unknown_d7c = true;
}


// void MapClass::Spring_Bridge_Destruction_Triggers(Cell cell1, Cell cell2) (0x00558340)
// TibSun Spring_Bridge_Destruction_Triggers: walks the span from the lower end to the
// higher one and springs the bridge-destroyed event (0x1F) on the tags of each cell and
// of the cells beside it (one on one side, two on the other).

inline void Swap_Cells(Cell &first, Cell &second)
{
    Cell temp(first);
    first = second;
    second = temp;
}

void MapClass::Spring_Bridge_Destruction_Triggers(Cell cell1, Cell cell2)
{
    bool is_east_west = false;

    if (cell1.Y == cell2.Y) {
        is_east_west = true;
        if (cell1.X > cell2.X) {
            Swap_Cells(cell1, cell2);
        }
    } else if (cell1.Y > cell2.Y) {
        Swap_Cells(cell1, cell2);
    }

    Cell current = cell1;
    Cell end = cell2;
    while (current != end) {
        CellClass *cptr = &Map[current];
        if (cptr->unknown_3c != 0) {
            cptr->unknown_3c->Spring(0x1F, 0, unknown_cell_a6f238);
        }

        if (is_east_west) {
            cptr = &Map[Adjacent_Cell(current, 4)];
            if (cptr->unknown_3c != 0) {
                cptr->unknown_3c->Spring(0x1F, 0, unknown_cell_a6f238);
            }
            cptr = &Map[Adjacent_Cell(current, 0)];
            if (cptr->unknown_3c != 0) {
                cptr->unknown_3c->Spring(0x1F, 0, unknown_cell_a6f238);
            }
            cptr = &Map[Adjacent_Cell(cptr->MapCoords, 0)];
            if (cptr->unknown_3c != 0) {
                cptr->unknown_3c->Spring(0x1F, 0, unknown_cell_a6f238);
            }
            current.X++;
        } else {
            cptr = &Map[Adjacent_Cell(current, 2)];
            if (cptr->unknown_3c != 0) {
                cptr->unknown_3c->Spring(0x1F, 0, unknown_cell_a6f238);
            }
            cptr = &Map[Adjacent_Cell(current, 6)];
            if (cptr->unknown_3c != 0) {
                cptr->unknown_3c->Spring(0x1F, 0, unknown_cell_a6f238);
            }
            cptr = &Map[Adjacent_Cell(cptr->MapCoords, 6)];
            if (cptr->unknown_3c != 0) {
                cptr->unknown_3c->Spring(0x1F, 0, unknown_cell_a6f238);
            }
            current.Y++;
        }
    }
}


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
				Map.Radar_Background(behindptr->MapCoords);

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


// void MapClass::Destroy_High_Bridge_Connections_00558BD0(Cell const &cell) (0x00558BD0)
// void MapClass::Destroy_High_Bridge_Connections_00558BD0(Cell const &cell) (0x00558BD0)
// TibSun MapClass::Destroy_High_Bridge_Connections: finds the first of the eight cells around `cell` that is
// (or was) under a bridge, takes the span's anchor from it (the deck cell of a live bridge, or two cells back
// from the end of a damaged one), then walks from the anchor toward the span start inside the map rectangle
// until an end or middle high-bridge piece is found and drops that span (0x00553000), registering the
// dirty area it reports. The walk stays inside the map diamond (0x0054AAB0).
extern int unknown_tileset_a6e0e4;
extern int unknown_tileset_a52f80;
extern int unknown_tileset_a53394;
extern int unknown_tileset_a52588;

void MapClass::Destroy_High_Bridge_Connections_00558BD0(Cell const &cell)
{
    CellClass *cellptr = &unknown_dummy_cell_a6f9f8;
    int facing = FACING_N;
    for (int index = 0; index < 8; index++) {
        Cell adjacent = Adjacent_Cell(cell, facing);
        cellptr = &Map[adjacent];
        if (cellptr->IsUnderBridge || cellptr->WasUnderBridge) {
            break;
        }
        facing = Facing_Add(facing, FACING_NE);
    }

    if (!cellptr->IsUnderBridge && !cellptr->WasUnderBridge) {
        return;
    }

    Cell origin;
    if (cellptr->IsUnderBridge) {
        origin = cellptr->Get_Bridge_Deck_Cell();
    } else {
        Cell walk = cellptr->Fetch_CellID();
        int count = 0;
        int step;
        if (cellptr->IsBridgeEastWest) {
            step = FACING_S;
        } else {
            step = FACING_E;
        }
        CellClass *walkptr;
        while (true) {
            Cell next = Adjacent_Cell(walk, step);
            walk = next;
            walkptr = &Map[next];
            if (walkptr->WasUnderBridge) {
                if (++count >= 4) {
                    return;
                }
            } else {
                break;
            }
        }
        FacingType back = Facing_Sub(step, FACING_S);
        origin = Adjacent_Cell(Adjacent_Cell(walk, back), back);
    }

    int advance = cellptr->IsBridgeEastWest ? FACING_W : FACING_N;
    Cell current = origin;
    Rect dirty = unknown_empty_rect;

    while (true) {
        if (!Map.In_Radar(current)) {
            return;
        }

        int cellnum = current.X + current.Y * 512;
        if (Cells[cellnum] != 0) {
            CellClass *spanptr = &Map[current];
            int ittype = spanptr->IsoTileIndex - unknown_tileset_a52c74 + 1;

            if ((ittype == unknown_tileset_a6e0e4 || ittype == unknown_tileset_a52f80) && spanptr->unknown_11a == 8) {
                if (Destroy_High_Bridge_Span_00558660(current, FACING_E, &dirty)) {
                    if (dirty != unknown_empty_rect) {
                        unknown_tactical_83996c->Register_Dirty_Area(dirty, false);
                    }
                }
                return;
            }

            if ((ittype == unknown_tileset_a6cb7c || ittype == unknown_tileset_a6cb7c + 3 ||
                 ittype == unknown_tileset_a6cb7c + 1 || ittype == unknown_tileset_a6cb7c + 2) &&
                spanptr->unknown_11a == 5) {
                if (Destroy_High_Bridge_Span_00558660(current, FACING_E, &dirty)) {
                    if (dirty != unknown_empty_rect) {
                        unknown_tactical_83996c->Register_Dirty_Area(dirty, false);
                    }
                }
                return;
            }

            if ((ittype == unknown_tileset_a53394 || ittype == unknown_tileset_a52588) && spanptr->unknown_11a == 12) {
                if (Destroy_High_Bridge_Span_00558660(current, FACING_S, &dirty)) {
                    if (dirty != unknown_empty_rect) {
                        unknown_tactical_83996c->Register_Dirty_Area(dirty, false);
                    }
                }
                return;
            }

            if ((ittype == unknown_tileset_a52e70 || ittype == unknown_tileset_a52e70 + 3 ||
                 ittype == unknown_tileset_a52e70 + 2 || ittype == unknown_tileset_a52e70 + 1) &&
                spanptr->unknown_11a == 7) {
                if (Destroy_High_Bridge_Span_00558660(current, FACING_S, &dirty)) {
                    if (dirty != unknown_empty_rect) {
                        unknown_tactical_83996c->Register_Dirty_Area(dirty, false);
                    }
                }
                return;
            }
        }

        current = Adjacent_Cell(current, advance);
    }
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2025 Electronic Arts Inc.
// Copyright 2026 OpenTS contributors
// Contains material derived from Electronic Arts source code.
// Modified by OpenTS contributors, 2026.
// EA's GPLv3 Section 7 additional terms and supplemental warranty disclaimers apply; see LICENSE.md.
// Adapted from TibSun code/map.cpp, MapClass::Damage_High_Bridge, base RA2 00559000.
// Per-case full switch tails (Scout953 pa form) and TibSun High's return(true) after each middle-span collapse
// (the original returns 1 there: 0x5595AE, 0x559ABE). The collapse list is built as the accepted twins
// Repair_Train_Bridge 0x552570 / 0x555A60 build theirs, `cells.Add(Cell(zone_cell + Cell(x, y)))`, and the zone
// cell steps with `zone_cell += Cell(...)` (TPoint2D<short>::operator+=, as Find_Exit_Cell 0x44A400 and the
// Tactical tile walks); with those inline sums both DynamicVectorClass dtors keep Clear 0x42CFB0 out of line.
// One `CellClass * middle_cellptr` declared with `subtile` serves both middle blocks (as Damage_Train_Bridge 0x5539B0,
// where that single web, not ittype, takes the dead `cell` argument slot).

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
				Zone_Reset();
			}

			DynamicVectorClass<Cell> cells;
			for (int x = 0; x < 2; x++) {
				for (int y = -2; y < 3; y++) {
					cells.Add(Cell(zone_cell + Cell(x, y)));
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
					Zone_Reset();
				}

				DynamicVectorClass<Cell> cells;
				for (int y = 0; y < 2; y++) {
					for (int x = -2; x < 3; x++) {
						cells.Add(Cell(zone_cell + Cell(x, y)));
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
					Zone_Reset();
				}
				return(true);

			case 7:
				Internal_Destroy_High_Bridge_EW_BottomRight(bridge_deck_cell->Fetch_CellID(), dir);
				bridge_deck_cell->Set_Under_Bridge(FACING_N, false);
				bridge_deck_cell->OverlayData = 0;
				bridge_deck_cell->OverlayIndex = -1;
				Destroy_High_Bridge_Connections_00558BD0(cell);
				if (Unknown_005501A0(bridge_deck_cell->MapCoords)) {
					Zone_Reset();
				}
				return(true);

			case 8:
				Internal_Destroy_High_Bridge_EW_TopLeft(bridge_deck_cell->Fetch_CellID(), Facing_Sub(dir, FACING_S));
				bridge_deck_cell->Set_Under_Bridge(FACING_N, false);
				bridge_deck_cell->OverlayData = 0;
				bridge_deck_cell->OverlayIndex = -1;
				Destroy_High_Bridge_Connections_00558BD0(cell);
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
					Zone_Reset();
				}
				return(true);

			case 16:
				Internal_Destroy_High_Bridge_NS_TopRight(bridge_deck_cell->Fetch_CellID(), Facing_Sub(dir, FACING_S));
				bridge_deck_cell->Set_Under_Bridge(FACING_W, false);
				bridge_deck_cell->OverlayData = 0;
				bridge_deck_cell->OverlayIndex = -1;
				Destroy_High_Bridge_Connections_00558BD0(cell);
				if (Unknown_005501A0(bridge_deck_cell->MapCoords)) {
					Zone_Reset();
				}
				return(true);

			case 17:
				Internal_Destroy_High_Bridge_NS_BottomLeft(bridge_deck_cell->Fetch_CellID(), dir);
				bridge_deck_cell->Set_Under_Bridge(FACING_W, false);
				bridge_deck_cell->OverlayData = 0;
				bridge_deck_cell->OverlayIndex = -1;
				Destroy_High_Bridge_Connections_00558BD0(cell);
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


// void MapClass::Detach(AbstractClass *target, bool all) (0x00559D80, slot 25)
// Forgets a target: for a tag (kind 0x2C) every cell of the tagged-cell list
// (+0x1158) carrying it loses its tag and leaves the list, the dummy cell
// forgets it (0x0047A340), and the tag leaves the tag list 0x00866238.

// The tag list (0x00866238; as TagClass's destructor 0x006B39F0 declares it).
extern DynamicVectorClass<TagClass *> unknown_tags_866238;

void MapClass::Detach(AbstractClass *target, bool all)
{
    if (target->Kind_Of() == 0x2C) {
        for (int index = 0; index < unknown_1158.Count(); index++) {
            CellClass *cell = &(*this)[unknown_1158[index]];
            if (cell != 0 && cell->unknown_3c == (TagClass *)target) {
                cell->Attach_Tag(0);
                unknown_1158.Delete(cell->MapCoords);
                index--;
            }
        }
        unknown_dummy_cell_a6f9f8.unknown_47a340(target);
    }
    if (target->Kind_Of() == 0x2C) {
        unknown_tags_866238.Delete((TagClass *)target);
    }
}


// Cell MapClass::Pick_Random_Location() const (0x00559EB0)
// TibSun Pick_Random_Location: a random cell of the map rectangle (Map +0x120/+0x124 the
// first cell, +0x128/+0x12C the size in cells), read through the global Map. No callers or
// pointers to it were found; the thiscall member ABI follows from its body (ECX unused, the
// returned Cell through the hidden pointer, ret 4).

Cell MapClass::Pick_Random_Location() const
{
    int x = Map.unknown_120 + Random_Pick(0, Map.unknown_128 - 1);
    int y = Map.unknown_124 + Random_Pick(0, Map.unknown_12c - 1);
    return XY_Cell(x, y);
}


// void MapClass::Unknown_00559F10() (0x00559F10)
// Covers the whole map again: runs DisplayClass 0x0049E000 (false, false),
// clears every cell's visibility (+0x12C bits 3-4, +0x140 bits 0-1; +0x130
// set, +0x134 cleared), runs DisplayClass 0x0049DE00 (false, false), clears
// the player's +0x188, redraws the radar and flags a full redraw.

void MapClass::Unknown_00559F10()
{
    Map.Unknown_0049E000(false, false);
    Reset_Iterator();
    for (CellClass *cell = Iterate(); cell != 0; cell = Iterate()) {
        cell->IsMapped = false; cell->IsVisible = false;
        cell->unknown_130 = 1;
        cell->unknown_134 = 0;
        cell->IsFogVisible = false; cell->IsFogMapped = false;
    }
    Map.Unknown_0049DE00(false, false);
    unknown_player_a35db4->unknown_188 = false;
    Map.Complete_Radar_Refresh();
    Flag_To_Redraw(2);
}


// void MapClass::Unknown_00559FD0() (0x00559FD0)
// Clears every cell's visibility bits (+0x12C bits 3-4, +0x140 bits 0-1),
// runs DisplayClass 0x0049E110 (false, false), clears the player's +0x188,
// redraws the radar and flags a full redraw.

void MapClass::Unknown_00559FD0()
{
    Reset_Iterator();
    for (CellClass *cell = Iterate(); cell != 0; cell = Iterate()) {
        cell->IsMapped = false; cell->IsVisible = false;
        cell->IsFogVisible = false; cell->IsFogMapped = false;
    }
    Map.Unknown_0049E110(false, false);
    unknown_player_a35db4->unknown_188 = false;
    Map.Complete_Radar_Refresh();
    Flag_To_Redraw(2);
}


// void MapClass::Unknown_0055A070() (0x0055A070)
// Lets every cell refresh itself (0x0047C8A0) and clears the visibility
// bits (+0x12C bits 3-4, +0x140 bits 0-1) of those whose +0x130 is not
// negative; then DisplayClass 0x0049E110 (false, false), the player's
// +0x188 cleared, the radar redrawn and a full redraw.

void MapClass::Unknown_0055A070()
{
    Reset_Iterator();
    for (CellClass *cell = Iterate(); cell != 0; cell = Iterate()) {
        cell->unknown_47c8a0();
        if (cell->unknown_130 >= 0) {
            cell->IsMapped = false; cell->IsVisible = false;
            cell->IsFogVisible = false; cell->IsFogMapped = false;
        }
    }
    Map.Unknown_0049E110(false, false);
    unknown_player_a35db4->unknown_188 = false;
    Map.Complete_Radar_Refresh();
    Flag_To_Redraw(2);
}


// void MapClass::Unknown_0055A120() (0x0055A120)
// Unless the player's +0x188 is set: DisplayClass 0x0049E000 (false,
// false), +0x188 set, every cell of the global Map covered (+0x12C bits
// 3-4 and +0x140 bits 0-1 set, +0x130 and +0x134 cleared), DisplayClass
// 0x0049DE00 (false, false), the radar redrawn and a full redraw.

void MapClass::Unknown_0055A120()
{
    if (!unknown_player_a35db4->unknown_188) {
        Map.Unknown_0049E000(false, false);
        unknown_player_a35db4->unknown_188 = true;
        Map.Reset_Iterator();
        for (CellClass *cell = Map.Iterate(); cell != 0; cell = Map.Iterate()) {
            cell->IsMapped = true; cell->IsVisible = true;
            cell->unknown_130 = 0;
            cell->unknown_134 = 0;
            cell->IsFogVisible = true; cell->IsFogMapped = true;
        }
        Map.Unknown_0049DE00(false, false);
        Map.Complete_Radar_Refresh();
        Map.Flag_To_Redraw(2);
    }
}


// void MapClass::Unknown_0055A1F0() (0x0055A1F0)
// Once per game (player +0x188 clear): passes every cell of the global Map
// with the player to DisplayClass slot 39, then redraws.

void MapClass::Unknown_0055A1F0()
{
    if (!unknown_player_a35db4->unknown_188) {
        unknown_player_a35db4->unknown_188 = true;
        Map.Reset_Iterator();
        for (CellClass *cell = Map.Iterate(); cell != 0; cell = Map.Iterate()) {
            Map.unknown_slot39(cell->MapCoords, unknown_player_a35db4);
        }
        Map.Complete_Radar_Refresh();
        Map.Flag_To_Redraw(1);
    }
}


// int MapClass::Get_Height_GL(CoordStruct const &coord) (0x0055A280)
// The ground height at a lepton coordinate (TibSun Get_Height_GL): the global
// Map's cell under it, asked for the height at the coordinate's X and Y. The
// original copies the coordinate's first two dwords into the Point2D it
// passes (an 8-byte struct copy, Y read again for the cell index).

int MapClass::Get_Height_GL(CoordStruct const &coord)
{
    Point2D point = *(Point2D const *)&coord;
    return Map[coord].unknown_4706c0(point);
}


// void MapClass::Unknown_0055A300() (0x0055A300)
// Refreshes the shroud drawing of every cell in two walks (0x0055A490): cells
// flagged +0x140 bit 0x20 lose their +0x12C bits 0x18 and +0x140 bits 0x23 and
// are redrawn; then each cell whose layer 0 shroud frame (0x006A8D40) changed
// takes it and is redrawn. A redrawn cell that is fully shrouded (frame -2)
// reveals its nearby technos to the player (0x00568520).

void MapClass::Unknown_0055A300()
{
    Reset_Iterator();
    CellClass *cellptr = Iterate();
    while (cellptr != 0) {
        if (cellptr->IsToShroud) {
            cellptr->IsMapped = false; cellptr->IsVisible = false;
            cellptr->IsFogVisible = false; cellptr->IsFogMapped = false; cellptr->IsToShroud = false;
            cellptr->unknown_138 = true;
            unknown_tactical_83996c->Flag_Cell(cellptr);
            if (cellptr->unknown_120 == -2) {
                Map.Unknown_00568520(cellptr, unknown_player_a35db4, true);
            }
        }
        cellptr = Iterate();
    }

    Reset_Iterator();
    cellptr = Iterate();
    while (cellptr != 0) {
        Cell cell = Coord_Cell(cellptr->Center_Coord());
        signed char frame = unknown_tactical_83996c->Cell_Shadow(cell, 0);
        if (frame != cellptr->unknown_120) {
            cellptr->unknown_120 = frame;
            cellptr->unknown_138 = true;
            unknown_tactical_83996c->Flag_Cell(cellptr);
            if (cellptr->unknown_120 == -2) {
                Map.Unknown_00568520(cellptr, unknown_player_a35db4, true);
            }
        }
        cellptr = Iterate();
    }
}


// CellClass *MapClass::Iterate() (0x0055A490)
// The next cell of the walk 0x0055A550 started: the walk follows diagonal
// rows (X up, Y down, 511 slots back in Cells per step); at a row's end the
// current X and Y are swapped and one of them stepped on by the parity of
// X - width + Y - 1, starting a row one or two cells shorter than the map
// width. Returns the cell of the slot it was at.

CellClass *MapClass::Iterate()
{
    CellClass **slot = unknown_114;
    if (unknown_110 != 0) {
        unknown_108++;
        unknown_10c--;
        unknown_110--;
        unknown_114 = slot - 511;
    } else {
        int temp = unknown_108;
        unknown_108 = unknown_10c;
        unknown_10c = temp;
        if ((unknown_108 - MapSize.Width + unknown_10c - 1) & 1) {
            unknown_10c++;
            unknown_110 = MapSize.Width - 1;
        } else {
            unknown_108++;
            unknown_110 = MapSize.Width - 2;
        }
        unknown_114 = &Cells[(unknown_10c << 9) + unknown_108];
    }
    return *slot;
}


// void MapClass::Reset_Iterator() (0x0055A550)
// Starts the cell walk of 0x0055A490 at cell (1, width): a diagonal row of
// width - 1 more cells.

void MapClass::Reset_Iterator()
{
    unknown_108 = 1;
    unknown_10c = MapSize.Width;
    unknown_110 = MapSize.Width - 1;
    unknown_114 = &Cells[(MapSize.Width << 9) + 1];
}


// bool MapClass::Unknown_0055A590(Rect const &rect, bool flag) (0x0055A590)
// TibSun In_Local_Radar(Rect) shape: whether all four corner cells of a cell rectangle pass
// 0x0055A660; the corners are summed in 32 bits and narrowed by the Cell(int, int) constructor.

bool MapClass::Unknown_0055A590(Rect const &rect, bool flag)
{
    return(Unknown_0055A660(Cell(rect.X, rect.Y), flag) &&
        Unknown_0055A660(Cell(rect.X + rect.Width - 1, rect.Y), flag) &&
        Unknown_0055A660(Cell(rect.X, rect.Y + rect.Height - 1), flag) &&
        Unknown_0055A660(Cell(rect.X + rect.Width - 1, rect.Y + rect.Height - 1), flag) ? 1 : 0);
}


// bool MapClass::Unknown_0055A660(Cell const &cell, bool flag) (0x0055A660)
// Whether a cell lies inside the local map rectangle (+0xF8) in the map's
// diamond coordinates; with flag, the cell's height (+0x11B, one more on a
// bridge cell, +0x11C, below the top band) moves the X + Y bounds.

bool MapClass::Unknown_0055A660(Cell const &cell, bool flag)
{
    int height = 0;
    int x = cell.X;
    int y = cell.Y;
    if (flag) {
        CellClass &object = Map[cell];
        height = object.unknown_11b;
        if (object.unknown_11c && x + y < MapSize.Width + MapLocalSize.Y * 2 + 4 + height) {
            height++;
        }
    }
    if (x + y > MapSize.Width + MapLocalSize.Y * 2 + height &&
        x + y <= MapSize.Width + (MapLocalSize.Height + MapLocalSize.Y) * 2 + 2 + height &&
        x - y < (MapLocalSize.Width + MapLocalSize.X) * 2 - MapSize.Width &&
        y - x < MapSize.Width - MapLocalSize.X * 2) {
        return true;
    }
    return false;
}


// bool MapClass::Unknown_0055A740(CellClass *cell, bool flag) (0x0055A740)
// The test of 0x0055A660 for a cell object (its own height bytes when flag
// is set).

bool MapClass::Unknown_0055A740(CellClass *cell, bool flag)
{
    int height = 0;
    int x = cell->MapCoords.X;
    int y = cell->MapCoords.Y;
    if (flag) {
        height = cell->unknown_11b;
        if (cell->unknown_11c && x + y < MapSize.Width + MapLocalSize.Y * 2 + 4 + height) {
            height++;
        }
    }
    if (x + y > MapSize.Width + MapLocalSize.Y * 2 + height &&
        x + y <= MapSize.Width + (MapLocalSize.Height + MapLocalSize.Y) * 2 + 2 + height &&
        x - y < (MapLocalSize.Width + MapLocalSize.X) * 2 - MapSize.Width &&
        y - x < MapSize.Width - MapLocalSize.X * 2) {
        return true;
    }
    return false;
}


// bool MapClass::Unknown_0055A7F0(CoordStruct const &coord) (0x0055A7F0)
// The cell test 0x0055A660 (with true) for the cell of a lepton position.

bool MapClass::Unknown_0055A7F0(CoordStruct const &coord)
{
    Cell cell = Cell(coord.X / 256, coord.Y / 256);
    return Unknown_0055A660(cell, true);
}


// bool MapClass::Unknown_0055A840(Rect const &rect, Cell const &cell, bool flag) (0x0055A840)
// Whether a cell lies inside a rectangle in the map's diamond coordinates
// (0x0055A660 with the rectangle in place of the local map rectangle);
// with flag, the cell's height moves the X + Y bounds.

bool MapClass::Unknown_0055A840(Rect const &rect, Cell const &cell, bool flag)
{
    int height = 0;
    int x = cell.X;
    int y = cell.Y;
    if (flag) {
        CellClass &object = Map[cell];
        height = object.unknown_11b;
        if (object.unknown_11c && x + y < MapSize.Width + rect.Y * 2 + 4 + height) {
            height++;
        }
    }
    if (x + y > MapSize.Width + rect.Y * 2 + height &&
        x + y <= MapSize.Width + (rect.Height + rect.Y) * 2 + 2 + height &&
        x - y < (rect.Width + rect.X) * 2 - MapSize.Width &&
        y - x < MapSize.Width - rect.X * 2) {
        return true;
    }
    return false;
}


// CellClass *MapClass::Unknown_0055A910() (0x0055A910)
// Steps the walk over the local map rectangle kept in +0x118 (X) and +0x11C
// (Y): along a diagonal row (X up, Y down) and, past the rectangle's right
// edge, back to the start of the next row; returns the first cell it
// passes that is inside the local map (0x0055A660), or null below the
// rectangle's bottom.

CellClass *MapClass::Unknown_0055A910()
{
    while (unknown_118 + unknown_11c <= MapSize.Width + (MapLocalSize.Y + MapLocalSize.Height) * 2 + 12) {
        int x = unknown_118;
        int y = unknown_11c;
        Cell cell(x, y);
        if (x - y >= (MapLocalSize.X + MapLocalSize.Width) * 2 - MapSize.Width - 2) {
            bool odd = ((unknown_118 - MapSize.Width + unknown_11c - 1) & 1) == 1;
            int length = MapLocalSize.Width - (odd ? 2 : 1);
            unknown_118 -= length;
            unknown_11c += length;
            if (odd) {
                unknown_11c++;
            } else {
                unknown_118++;
            }
        } else {
            unknown_118++;
            unknown_11c--;
        }
        if (Unknown_0055A660(cell, true)) {
            return &(*this)[cell];
        }
    }
    return 0;
}


// CellClass *MapClass::Get_Local_Grid_Cell(Cell const &cell, int spacing, bool unbiased) (0x0055AA80)
// The cell of a sampling grid of the given spacing over the local map
// (TibSun Get_Local_Grid_Cell): the cell moved up to the next grid line of
// each axis, measured from the local map's corner; unless unbiased, cells in
// the lower part of the local map step back half a spacing, or one less than
// a spacing near the bottom.

CellClass *MapClass::Get_Local_Grid_Cell(Cell const &cell, int spacing, bool unbiased)
{
    if (spacing == 1) {
        return &Map[cell];
    }

    int x = cell.X;
    int y = cell.Y;
    Cell local = XY_Cell(MapLocalSize.X + MapLocalSize.Y + 1, MapSize.Width - MapLocalSize.X + MapLocalSize.Y);
    int xx = (6000 * spacing + local.X - x) % spacing;
    int yy = (6000 * spacing + local.Y - y) % spacing;

    if (xx > 0) {
        x += spacing - xx;
    }
    if (yy > 0) {
        y += spacing - yy;
    }

    int dsum = local.X + local.Y;
    if (unbiased || (y + x < dsum + 2 * MapLocalSize.Height / 3)) {
        return &Map[Cell(x, y)];
    } else if (y + x < dsum + 4 * MapLocalSize.Height / 3 && spacing > 2) {
        return &Map[Cell(x, y) - Cell(spacing / 2, spacing / 2)];
    } else {
        return &Map[Cell(x, y) - Cell(spacing - 1, spacing - 1)];
    }
}


class Leaf_55acc0 { public: void get(); };
void Leaf_55acc0::get() { unsigned int map_address = (unsigned int)this; unsigned int unused_result; unsigned int unused_value; *(unsigned char*)(map_address + 0x1154) = (*(unsigned char*)(map_address + 0x1154)) + 1; }


// bool MapClass::Unknown_0055ACD0(ObjectClass *object, Cell const &cell) (0x0055ACD0)
// Whether the object may enter the cell (TibSun Try_Open_Gate): a gate building
// among the cell's other occupiers opens for an allied object (BuildingClass
// 0x0044D7D0) and otherwise lets it pass only when already open (0x0044D880).

bool MapClass::Unknown_0055ACD0(ObjectClass *object, Cell const &cell)
{
    TechnoClass *optr = (TechnoClass *)Map[cell].OccupierPtr;
    while (optr != 0) {
        if ((TechnoClass *)object != optr) {
            BuildingClass *bptr = (BuildingClass *)optr;
            if (bptr->Kind_Of() == 6 && bptr->Type->Gate) {
                if (bptr->House->Is_Ally((TechnoClass *)object)) {
                    return bptr->unknown_44d7d0();
                }
                if (bptr->unknown_44d880()) {
                    return true;
                }
            }
        }
        optr = (TechnoClass *)optr->unknown_30;
    }
    return true;
}


// int MapClass::Cell_To_Local_Sector_Index(Cell const &cell) (0x0055AD70)
// The local view is a 3 x 3 grid of sectors along its diagonal rows: the
// sector (1-9) that holds a cell (TibSun Cell_To_Local_Sector_Index).

int MapClass::Cell_To_Local_Sector_Index(Cell const &cell)
{
    int wrem = (2 * MapLocalSize.Width - 1) % 3;
    int wstep = (2 * MapLocalSize.Width - 1) / 3;
    int hrem = (2 * MapLocalSize.Height - 1) % 3;
    int hstep = (2 * MapLocalSize.Height - 1) / 3;

    int x = MapLocalSize.X + MapLocalSize.Y + 1;
    int y = MapSize.Width - MapLocalSize.X + MapLocalSize.Y;
    Cell origin = Cell(x, y);

    int column = (cell.X + (origin.Y - (wrem ? 1 : 0) - origin.X - cell.Y)) / wstep;
    int row = (cell.X + (cell.Y - (hrem ? 1 : 0) - origin.X - origin.Y)) / hstep;
    if (column == 3) {
        column = 2;
    }
    if (row == 3) {
        row = 2;
    }
    return column + 3 * row + 1;
}


// Cell MapClass::Local_Sector_Index_To_Cell(int index) (0x0055AE50)
// The cell that stands for one sector (1-9) of the local view's 3 x 3 grid
// (the inverse of 0x0055AD70; TibSun Local_Sector_Index_To_Cell).

// The sector's cell offset as a cell (a one-line inline builder; name and form
// are hypotheses: the extra inline level over a bare Cell(x, y) gives the
// original's frame slots for the two TPoint2D<short>::operator+ operands).
inline Cell Sector_Offset_Cell(int x, int y)
{
    return Cell(x, y);
}

Cell MapClass::Local_Sector_Index_To_Cell(int index)
{
    int wrem = (2 * MapLocalSize.Width - 1) % 3;
    int wstep = (2 * MapLocalSize.Width - 1) / 3;
    int hrem = (2 * MapLocalSize.Height - 1) % 3;
    int hstep = (2 * MapLocalSize.Height - 1) / 3;

    int column = (index - 1) % 3;
    int row = (index - 1) / 3;

    int x = MapLocalSize.X + MapLocalSize.Y + 1;
    int y = MapSize.Width - MapLocalSize.X + MapLocalSize.Y;

    int left = (wrem ? 1 : 0) + column * wstep;
    int top = (hrem ? 1 : 0) + row * hstep;
    int right = left + ((left & 1) ? 1 : 0);
    int bottom = top + ((top & 1) ? 1 : 0);

    int cx = (bottom + right) / 2;
    int cy = (top - left) / 2;
    return Sector_Offset_Cell(cx, cy) + Cell(x, y);
}


// bool __fastcall Unknown_0055AF60(int tile) (0x0055AF60)
// Whether a tile index is one of the 42 tiles of the set starting at
// 0x00A6CB74.

bool __fastcall Unknown_0055AF60(int tile)
{
    if (tile >= unknown_tileset_a6cb74 && tile < unknown_tileset_a6cb74 + 42) {
        return true;
    }
    return false;
}


// bool __fastcall Unknown_0055AF80(int tile, int kind) (0x0055AF80)
// Whether a tile allows kind: every tile of the sets at 0x00A52E68 (40) and
// 0x00A6DD00 (20); of the four-tile sets at 0x00A52584, 0x00A6CF58,
// 0x00A52EA0 and 0x00A52EF0, the middle two always and the first and last
// unless kind is 0 or 4, 1 or 3, 0 or 1, 2 or 3 respectively.

bool __fastcall Unknown_0055AF80(int tile, int kind)
{
    if (tile >= unknown_tileset_a52e68 && tile < unknown_tileset_a52e68 + 40) {
        return true;
    }
    if (tile >= unknown_tileset_a52584 && tile < unknown_tileset_a52584 + 4) {
        if (tile == unknown_tileset_a52584 || tile == unknown_tileset_a52584 + 3) {
            if (kind == 0 || kind == 4) {
                return false;
            }
            return true;
        }
    } else if (tile >= unknown_tileset_a6cf58 && tile < unknown_tileset_a6cf58 + 4) {
        if (tile == unknown_tileset_a6cf58 || tile == unknown_tileset_a6cf58 + 3) {
            if (kind == 1 || kind == 3) {
                return false;
            }
            return true;
        }
    } else if (tile >= unknown_tileset_a52ea0 && tile < unknown_tileset_a52ea0 + 4) {
        if (tile == unknown_tileset_a52ea0 || tile == unknown_tileset_a52ea0 + 3) {
            if (kind == 0 || kind == 1) {
                return false;
            }
            return true;
        }
    } else if (tile >= unknown_tileset_a52ef0 && tile < unknown_tileset_a52ef0 + 4) {
        if (tile == unknown_tileset_a52ef0 || tile == unknown_tileset_a52ef0 + 3) {
            if (kind == 2 || kind == 3) {
                return false;
            }
            return true;
        }
    } else if (tile < unknown_tileset_a6dd00 || tile >= unknown_tileset_a6dd00 + 20) {
        return false;
    }
    return true;
}


// bool MapClass::Build_All_Cliffs(int unused, int region_id) (0x0055B060)
// The map generator's cliff pass (TibSun Build_All_Cliffs): with no generator
// cell data, a temporary array; every record's +0x4A set; the placement
// cursor cleared; every cell offered to Expand_High_Ground (0x0055B210),
// then Place_Cliff (0x0055B820) on the cells with no ramp (+0x11C) and no
// tile (0x0047B590), each pass stopping at the first failure; the pending
// placement object deleted; the temporary array freed. Whether every pass
// succeeded.

// The generator record of a cell index (TibSun MapRegionClass::Get_Cell_Data; __fastcall).
UnknownMapGenerationCell &__fastcall Get_Cell_Data_0056dbc0(int index);

bool MapClass::Build_All_Cliffs(int unused, int region_id)
{
    bool expanded = true;

    bool is_temp_data = false;
    if (unknown_map_cells_a70a24 == 0) {
        unknown_map_cells_a70a24 = new UnknownMapGenerationCell[unknown_map_cell_side_84e804 * unknown_map_cell_side_84e804];
        is_temp_data = true;
    }

    int r = unknown_map_cell_side_84e804;
    int s = r * r;
    for (int i = 0; i < s; i++) {
        Get_Cell_Data_0056dbc0(i).unknown_45[5] = 1;
    }

    Map.Unknown_00498F10(0);

    Map.Reset_Iterator();
    CellClass *cellptr = Map.Iterate();
    while (cellptr != 0 && expanded) {
        expanded = Expand_High_Ground(cellptr, region_id);
        cellptr = Map.Iterate();
    }

    Map.Reset_Iterator();
    cellptr = Map.Iterate();
    while (cellptr != 0 && expanded) {
        if (cellptr->unknown_11c == 0 && cellptr->unknown_47b590()) {
            expanded = Place_Cliff(cellptr, region_id);
        }
        cellptr = Map.Iterate();
    }

    ObjectClass *obj = Map.unknown_11a0;
    Map.unknown_11a4 = 0;
    if (obj != 0) {
        delete obj;
        Map.unknown_11a0 = 0;
    }

    if (is_temp_data) {
        delete [] unknown_map_cells_a70a24;
        unknown_map_cells_a70a24 = 0;
    }

    return expanded;
}


// bool MapClass::Expand_High_Ground(CellClass *cellptr, int region_id) (0x0055B210)
// The map generator's high-ground flood (TibSun Expand_High_Ground): a cell whose high-ground
// mask (0x0055BD70) shows a pinch, a lone raised neighbour pair, a notch or high ground across
// it is raised one step (+0x11B += 4) and joins region_id (0x00581870), unless another region
// holds it (0x005818A0; region_id -1 claims anything); the flood then spreads to all eight
// neighbours. False only when another region holds the cell.

bool MapClass::Expand_High_Ground(CellClass *cellptr, int region_id)
{
    bool expand = false;
    unsigned int mask = Unknown_0055BD70(cellptr);

    if ((mask & 0xA0) == 0xA0 && (mask & 0x11) == 0x11) {
        expand = true;
    }

    if ((mask & 0x20) != 0) {
        if ((Unknown_0055BD70(&Map[cellptr->Fetch_CellID() + Cell(1, 0)]) & 0x02) != 0) {
            expand = true;
        }
    }

    if ((mask & 0x02) != 0) {
        if ((Unknown_0055BD70(&Map[cellptr->Fetch_CellID() + Cell(-1, 0)]) & 0x20) != 0) {
            expand = true;
        }
    }

    if ((mask & 0x08) != 0) {
        if ((Unknown_0055BD70(&Map[cellptr->Fetch_CellID() + Cell(0, -1)]) & 0x80) != 0) {
            expand = true;
        }
    }

    if ((mask & 0x80) != 0) {
        if ((Unknown_0055BD70(&Map[cellptr->Fetch_CellID() + Cell(0, 1)]) & 0x08) != 0) {
            expand = true;
        }
    }

    if ((mask & 0x11) == 0x11 &&
        ((mask & 0x0A) != 0x0A || (mask & 0xE0) != 0) &&
        ((mask & 0xA0) != 0xA0 || (mask & 0x0E) != 0)) {
        expand = true;
    }

    if ((mask & 0x44) == 0x44 &&
        ((mask & 0x28) != 0x28 || (mask & 0x83) != 0) &&
        ((mask & 0x82) != 0x82 || (mask & 0x38) != 0)) {
        expand = true;
    }

    if ((mask & 0x2C) == 0x24 ||
        (mask & 0xA1) == 0x21 ||
        (mask & 0x1A) == 0x12 ||
        (mask & 0xC2) == 0x42 ||
        (mask & 0x0B) == 0x09 ||
        (mask & 0x68) == 0x48 ||
        (mask & 0x86) == 0x84 ||
        (mask & 0xB0) == 0x90) {
        expand = true;
    }

    if ((mask & 0x88) == 0x88 || (mask & 0x22) == 0x22 || expand) {
        int cell_region_id = RandomMapGen.Get_Cell_Data_Region(cellptr->Fetch_CellID());
        if (cell_region_id > 0 && cell_region_id != region_id && region_id != -1) {
            return false;
        }
        cellptr->unknown_11b += 4;
        RandomMapGen.Set_Cell_Data_Region(cellptr->Fetch_CellID(), region_id);
        for (int dir = 0; dir < 8; dir++) {
            Expand_High_Ground(&cellptr->Adjacent_Cell(dir), region_id);
        }
    }
    return true;
}


// Cell unknown_cells_a6fb50[40] (0x00A6FB50) and its dynamic initializer
// (0x0055B520; small enough that the construction is expanded into the
// .CRT$XCU entry itself): forty cells, all (0, 0) but entry 31 (1, 0).
// Only 8 constructors are expanded; the rest call 0x0042AD30.

Cell unknown_cells_a6fb50[40] = {
    Cell(0, 0), Cell(0, 0), Cell(0, 0), Cell(0, 0), Cell(0, 0), Cell(0, 0),
    Cell(0, 0), Cell(0, 0), Cell(0, 0), Cell(0, 0), Cell(0, 0), Cell(0, 0),
    Cell(0, 0), Cell(0, 0), Cell(0, 0), Cell(0, 0), Cell(0, 0), Cell(0, 0),
    Cell(0, 0), Cell(0, 0), Cell(0, 0), Cell(0, 0), Cell(0, 0), Cell(0, 0),
    Cell(0, 0), Cell(0, 0), Cell(0, 0), Cell(0, 0), Cell(0, 0), Cell(0, 0),
    Cell(0, 0), Cell(1, 0), Cell(0, 0), Cell(0, 0), Cell(0, 0), Cell(0, 0),
    Cell(0, 0), Cell(0, 0), Cell(0, 0), Cell(0, 0)
};


// bool MapClass::Place_Cliff(CellClass *cellptr, int region_id) (0x0055B820)
// The cliff pass's per-cell step (TibSun Place_Cliff without the Firestorm crystal cliffs):
// the high-ground mask around the cell picks a cliff piece (inside corners, straight runs with
// random variants, outside corners); the piece becomes the pending tile type (+0x11A4, from
// IsoTileTypes at the cliff set 0x00A52E68), the cursor moves to the cell plus the piece's
// offset (unknown_cells_a6fb50, 0x004994D0) and Pick_Random_Tile_Variant (0x0055D640) places
// it over clear ground. False only when that placement fails and clears its result flag.

unsigned int __fastcall Map_Random_Range_005797a0(unsigned int minimum, unsigned int maximum);
// unknown_cells_a6fb50 (the cliff piece offsets) is defined by 0x0055B520's source earlier in
// this unit: an extern redeclaration after it would emit an empty dynamic initializer.

bool MapClass::Place_Cliff(CellClass *cellptr, int region_id)
{
    int random6 = Map_Random_Range_005797a0(0, 5);
    unsigned int mask = Unknown_0055BD70(cellptr);

    int variant = -1;
    unsigned int south;
    unsigned int east;
    unsigned int southeast;

    if ((mask & 0xA0) == 0xA0) {
        south = Unknown_0055BD70(&Map[cellptr->Fetch_CellID() + Cell(0, 1)]);
        east = Unknown_0055BD70(&Map[cellptr->Fetch_CellID() + Cell(1, 0)]);
        if ((south & 0x20) != 0 || (east & 0x80) != 0) {
            variant = 34;
        } else {
            variant = random6 % 3 + 9;
        }
    } else if ((mask & 0x82) == 0x82) {
        variant = 39;
    } else if ((mask & 0x0A) == 0x0A) {
        variant = 33;
    } else if ((mask & 0x28) == 0x28) {
        variant = 40;
    }

    if (variant == -1) {
        if ((mask & 0x02) != 0) {
            south = Unknown_0055BD70(&Map[cellptr->Fetch_CellID() + Cell(0, 1)]);
            southeast = Unknown_0055BD70(&Map[cellptr->Fetch_CellID() + Cell(1, 1)]);
            if ((mask & 0x04) != 0 && (mask & 0x18) == 0 && (south & 0x0A) != 0x0A) {
                variant = random6 % 3 + 35;
            } else if ((mask & 0x0C) != 0 || (southeast & 0x02) != 0 || (random6 & 1) == 0) {
                variant = 38;
            } else {
                variant = 1;
            }
        } else if ((mask & 0x20) != 0) {
            south = Unknown_0055BD70(&Map[cellptr->Fetch_CellID() + Cell(0, 1)]);
            if ((mask & 0x10) == 0 || (mask & 0x0C) != 0 || (south & 0x28) == 0x28) {
                variant = 18;
            } else {
                variant = random6 % 3 + 15;
            }
        } else if ((mask & 0x08) != 0) {
            east = Unknown_0055BD70(&Map[cellptr->Fetch_CellID() + Cell(1, 0)]);
            southeast = Unknown_0055BD70(&Map[cellptr->Fetch_CellID() + Cell(1, 1)]);
            if ((mask & 0x04) == 0 || (mask & 0x03) != 0 || (east & 0x0A) == 0x0A) {
                if ((mask & 0x06) != 0 || (southeast & 0x08) != 0 || (random6 & 1) == 0) {
                    variant = 26;
                } else {
                    variant = 21;
                }
            } else {
                variant = random6 % 3 + 23;
            }
        } else if ((mask & 0x80) != 0) {
            east = Unknown_0055BD70(&Map[cellptr->Fetch_CellID() + Cell(1, 0)]);
            if ((mask & 0x01) == 0 || (mask & 0x06) != 0 || (east & 0x82) == 0x82) {
                variant = 8;
            } else {
                variant = random6 % 3 + 5;
            }
        }
    }

    if (variant == -1) {
        if ((mask & 0x01) != 0) {
            variant = 2;
        } else if ((mask & 0x04) != 0) {
            variant = (random6 & 1) + 29;
        } else if ((mask & 0x10) != 0) {
            variant = 22;
        } else if ((mask & 0x40) != 0) {
            variant = random6 % 3 + 12;
        } else {
            return true;
        }
    }

    if (variant > 0) {
        Map.unknown_11a4 = IsoTileTypes[unknown_tileset_a52e68 + variant - 1];
        Cell cell = cellptr->Fetch_CellID();
        Cell const &varcell = unknown_cells_a6fb50[variant - 1];

        Map.Unknown_004994D0(XY_Cell(varcell.X + cell.X, varcell.Y + cell.Y));

        bool result = true;
        if (!Pick_Random_Tile_Variant(0, 0, cellptr->unknown_11b, region_id, result, false) && !result) {
            return false;
        }
    }
    return true;
}


// unsigned int MapClass::Unknown_0055BD70(CellClass *cellptr) (0x0055BD70)
// The pattern of high ground around a cell (TibSun Get_High_Ground_Mask): for a cell inside
// the map whose generator record is in the fill area (+0x4A), the mask bit of every
// neighbour inside the map one level (4) above the cell (+0x11B) and not a cliff tile
// (0x0047B5E0); zero otherwise.

// The generator record of a cell (TibSun MapRegionClass::Get_Cell_Data; __fastcall).
UnknownMapGenerationCell &__fastcall Get_Cell_Data_0056dba0(Cell const &cell);

unsigned int MapClass::Unknown_0055BD70(CellClass *cellptr)
{
    int mask = 0;
    int height = cellptr->unknown_11b;
    Cell origin = cellptr->Fetch_CellID();

    if (!Map.In_Radar(origin) || !Get_Cell_Data_0056dba0(origin).unknown_45[5]) {
        return 0;
    }

    CellClass *cptr_ne = &Map[(Cell)(origin + Cell(1, -1))];
    CellClass *cptr_e = &Map[(Cell)(origin + Cell(1, 0))];
    CellClass *cptr_se = &Map[(Cell)(origin + Cell(1, 1))];
    CellClass *cptr_s = &Map[(Cell)(origin + Cell(0, 1))];
    CellClass *cptr_sw = &Map[(Cell)(origin + Cell(-1, 1))];
    CellClass *cptr_w = &Map[(Cell)(origin + Cell(-1, 0))];
    CellClass *cptr_nw = &Map[(Cell)(origin + Cell(-1, -1))];
    CellClass *cptr_n = &Map[(Cell)(origin + Cell(0, -1))];

    if (Map.In_Radar(origin + Cell(-1, -1)) && cptr_nw->unknown_11b == height + 4 && !cptr_nw->unknown_47b5e0()) {
        mask |= 0x40;
    }
    if (Map.In_Radar(origin + Cell(0, -1)) && cptr_n->unknown_11b == height + 4 && !cptr_n->unknown_47b5e0()) {
        mask |= 0x80;
    }
    if (Map.In_Radar(origin + Cell(1, -1)) && cptr_ne->unknown_11b == height + 4 && !cptr_ne->unknown_47b5e0()) {
        mask |= 0x01;
    }
    if (Map.In_Radar(origin + Cell(-1, 0)) && cptr_w->unknown_11b == height + 4 && !cptr_w->unknown_47b5e0()) {
        mask |= 0x20;
    }
    if (Map.In_Radar(origin + Cell(1, 0)) && cptr_e->unknown_11b == height + 4 && !cptr_e->unknown_47b5e0()) {
        mask |= 0x02;
    }
    if (Map.In_Radar(origin + Cell(-1, 1)) && cptr_sw->unknown_11b == height + 4 && !cptr_sw->unknown_47b5e0()) {
        mask |= 0x10;
    }
    if (Map.In_Radar(origin + Cell(0, 1)) && cptr_s->unknown_11b == height + 4 && !cptr_s->unknown_47b5e0()) {
        mask |= 0x08;
    }
    if (Map.In_Radar(origin + Cell(1, 1)) && cptr_se->unknown_11b == height + 4 && !cptr_se->unknown_47b5e0()) {
        mask |= 0x04;
    }
    return mask;
}


// bool MapClass::Build_All_Shores(int region_id, bool set_region) (0x0055C2C0)
// The map generator's water pass (TibSun Build_All_Shores): the placement cursor cleared;
// with no generator cell data, a temporary array; every record's water mask (+0x40) set to
// -1; every cell offered to Expand_Water (0x0055C630), then Prune_Water (0x0055C520), then
// Place_Shore (0x0055CEF0) passes 1 and 2, each pass stopping at the first failure; the
// pending placement object deleted; the temporary array freed. Whether every pass succeeded.

bool MapClass::Build_All_Shores(int region_id, bool set_region)
{
    Map.Unknown_00498F10(0);

    bool is_temp_data = false;
    if (unknown_map_cells_a70a24 == 0) {
        unknown_map_cells_a70a24 = new UnknownMapGenerationCell[unknown_map_cell_side_84e804 * unknown_map_cell_side_84e804];
        is_temp_data = true;
    }

    int r = unknown_map_cell_side_84e804;
    int s = r * r;
    for (int i = 0; i < s; i++) {
        Get_Cell_Data_0056dbc0(i).unknown_40 = -1;
    }

    Map.Reset_Iterator();
    CellClass *cellptr = Map.Iterate();
    bool expanded = true;
    while (cellptr != 0 && expanded) {
        expanded = Expand_Water(cellptr, region_id, set_region);
        cellptr = Map.Iterate();
    }

    Map.Reset_Iterator();
    cellptr = Map.Iterate();
    while (cellptr != 0 && expanded) {
        Unknown_0055C520(cellptr, region_id, set_region);
        cellptr = Map.Iterate();
    }

    Map.Reset_Iterator();
    cellptr = Map.Iterate();
    while (cellptr != 0 && expanded) {
        expanded = Place_Shore(cellptr, 1, region_id, set_region);
        cellptr = Map.Iterate();
    }

    Map.Reset_Iterator();
    cellptr = Map.Iterate();
    while (cellptr != 0 && expanded) {
        expanded = Place_Shore(cellptr, 2, region_id, set_region);
        cellptr = Map.Iterate();
    }

    ObjectClass *obj = Map.unknown_11a0;
    Map.unknown_11a4 = 0;
    if (obj != 0) {
        delete obj;
        Map.unknown_11a0 = 0;
    }

    if (is_temp_data) {
        delete [] unknown_map_cells_a70a24;
        unknown_map_cells_a70a24 = 0;
    }

    return expanded;
}


// void MapClass::Unknown_0055C520(CellClass *cellptr, int region_id, bool set_region) (0x0055C520)
// The map generator's water pruning (TibSun Prune_Water): a water tile (the 12 tiles from
// unknown_tileset_a52580) whose water mask (0x0055D410, control 2) is one of the eight
// lone-corner shapes loses its tile (+0x38 = 0xFFFF, +0x11A = 0); with set_region its
// generator record takes the region; each neighbour inside the generator's diagonal
// bounds has its record's water mask (+0x40) recomputed.

// The generator record of a cell (TibSun MapRegionClass::Get_Cell_Data; __fastcall).
UnknownMapGenerationCell &__fastcall Get_Cell_Data_0056dba0(Cell const &cell);

// Whether a cell is inside the generator's diagonal map bounds (TibSun My_In_Radar).
inline bool My_In_Radar(Cell const &cell)
{
    int x = cell.X;
    int y = cell.Y;
    return (y + x > unknown_map_min_a70a1c &&
            x - y < unknown_map_min_a70a1c &&
            y - x < unknown_map_min_a70a1c &&
            y + x <= unknown_map_max_a70a20) ? true : false;
}

void MapClass::Unknown_0055C520(CellClass *cellptr, int region_id, bool set_region)
{
    int itype = cellptr->IsoTileIndex;
    if (itype >= unknown_tileset_a52580 && itype < unknown_tileset_a52580 + 12) {
        int mask = Unknown_0055D410(cellptr, 2);
        if (mask == 0xC7 || mask == 0x7C || mask == 0xF1 || mask == 0x1F ||
            mask == 0xC6 || mask == 0x6C || mask == 0xB1 || mask == 0x1B) {
            cellptr->IsoTileIndex = 0xFFFF;
            cellptr->unknown_11a = 0;
            if (set_region) {
                Get_Cell_Data_0056dba0(cellptr->MapCoords).RegionIndex = region_id;
            }
            for (int facing = 0; facing < 8; facing++) {
                CellClass *adjptr = &cellptr->Adjacent_Cell(facing);
                if (My_In_Radar(adjptr->Fetch_CellID())) {
                    UnknownMapGenerationCell &data = Get_Cell_Data_0056dba0(adjptr->Fetch_CellID());
                    data.unknown_40 = -1;
                    data.unknown_40 = Unknown_0055D410(adjptr, 2);
                }
            }
        }
    }
}


// bool MapClass::Expand_Water(CellClass *cellptr, int region_id, bool set_region) (0x0055C630)
// The map generator's water flood (TibSun Expand_Water): the cell's water mask (+0x40) is
// recomputed (0x0055D410, control 2); a cell whose land-side mask (control 0) shows a pinch,
// a narrow neck or water across it becomes water (+0x38 the first water tile, +0x11A = 0)
// unless another region holds it (without set_region); its region becomes region_id (or 0
// with set_region) and the flood spreads to every neighbour inside the generator bounds.
// False only when another region holds the cell.

bool MapClass::Expand_Water(CellClass *cellptr, int region_id, bool set_region)
{
    bool expand = false;

    UnknownMapGenerationCell &data = Get_Cell_Data_0056dba0(cellptr->Fetch_CellID());
    data.unknown_40 = Unknown_0055D410(cellptr, 2);

    int mask = Unknown_0055D410(cellptr, 0);
    if (mask > 0) {

        if (mask == 0x0B || mask == 0x1A) {
            expand = true;
        }

        if ((mask & 0xA0) == 0xA0 && (mask & 0x11) == 0 ||
            (mask & 0x82) == 0x82 && (mask & 0x44) == 0 ||
            (mask & 0x0A) == 0x0A && (mask & 0x11) == 0 ||
            (mask & 0x28) == 0x28 && (mask & 0x44) == 0) {
            expand = true;
        }

        if ((mask & 0x20) != 0) {
            Cell cell1 = cellptr->Fetch_CellID() + Cell(1, 0);
            CellClass *cptr1 = &Map[cell1];
            int mask1 = Unknown_0055D410(cptr1, 0);
            Cell cell2 = cellptr->Fetch_CellID() + Cell(2, 0);
            CellClass *cptr2 = &Map[cell2];
            int mask2 = Unknown_0055D410(cptr2, 0);
            if ((mask1 & 0x02) != 0 || (mask2 & 0x02) != 0) {
                expand = true;
            }
        }

        if ((mask & 0x02) != 0) {
            Cell cell1 = cellptr->Fetch_CellID() + Cell(-1, 0);
            CellClass *cptr1 = &Map[cell1];
            int mask1 = Unknown_0055D410(cptr1, 0);
            Cell cell2 = cellptr->Fetch_CellID() + Cell(-2, 0);
            CellClass *cptr2 = &Map[cell2];
            int mask2 = Unknown_0055D410(cptr2, 0);
            if ((mask1 & 0x20) != 0 || (mask2 & 0x20) != 0) {
                expand = true;
            }
        }

        if ((mask & 0x08) != 0) {
            Cell cell1 = cellptr->Fetch_CellID() + Cell(0, -1);
            CellClass *cptr1 = &Map[cell1];
            int mask1 = Unknown_0055D410(cptr1, 0);
            Cell cell2 = cellptr->Fetch_CellID() + Cell(0, -2);
            CellClass *cptr2 = &Map[cell2];
            int mask2 = Unknown_0055D410(cptr2, 0);
            if ((mask1 & 0x80) != 0 || (mask2 & 0x80) != 0) {
                expand = true;
            }
        }

        if ((mask & 0x80) != 0) {
            Cell cell1 = cellptr->Fetch_CellID() + Cell(0, 1);
            CellClass *cptr1 = &Map[cell1];
            int mask1 = Unknown_0055D410(cptr1, 0);
            Cell cell2 = cellptr->Fetch_CellID() + Cell(0, 2);
            CellClass *cptr2 = &Map[cell2];
            int mask2 = Unknown_0055D410(cptr2, 0);
            if ((mask1 & 0x08) != 0 || (mask2 & 0x08) != 0) {
                expand = true;
            }
        }

        if ((mask & 0x88) == 0x88 || (mask & 0x22) == 0x22 || expand) {

            int cell_region_id = RandomMapGen.Get_Cell_Data_Region(cellptr->Fetch_CellID());
            if (cell_region_id > 0 && cell_region_id != region_id && !set_region) {
                return false;
            }
            cellptr->IsoTileIndex = unknown_tileset_a52580;
            cellptr->unknown_11a = 0;
            if (set_region) {
                RandomMapGen.Set_Cell_Data_Region(cellptr->Fetch_CellID(), 0);
            } else {
                RandomMapGen.Set_Cell_Data_Region(cellptr->Fetch_CellID(), region_id);
            }
            for (int dir = 0; dir < 8; dir++) {
                CellClass *adjptr = &cellptr->Adjacent_Cell(dir);
                if (My_In_Radar(adjptr->Fetch_CellID())) {
                    Get_Cell_Data_0056dba0(adjptr->Fetch_CellID()).unknown_40 = -1;
                    Expand_Water(adjptr, region_id, set_region);
                }
            }
        }
    }
    return true;
}


// int __fastcall Straight_Shore_Length(CellClass *cellptr, int direction) (0x0055CB00)
// Counts the cells of a straight shore (TibSun name): from the cell's neighbour
// along the facing snapped to east or south, while that side is land and the
// cell beyond it toward `direction` is water (0x0047A270).

int __fastcall Straight_Shore_Length(CellClass *cellptr, int direction)
{
    int snap[4] = {
        2,
        4,
        2,
        4,
    };

    int count = 1;
    int s_direction = snap[direction / 2];
    CellClass *adj1 = &cellptr->Adjacent_Cell(s_direction);
    CellClass *adj2 = &adj1->Adjacent_Cell(direction);
    bool side_1_is_land = adj1->unknown_47a270() == false;
    bool side_2_is_water = adj2->unknown_47a270();

    while (side_1_is_land && side_2_is_water) {
        count++;
        adj1 = &adj1->Adjacent_Cell(s_direction);
        adj2 = &adj2->Adjacent_Cell(s_direction);
        side_1_is_land = adj1->unknown_47a270() == false;
        side_2_is_water = adj2->unknown_47a270();
    }

    return count;
}


// Cell unknown_cells_a6f920[42] (0x00A6F920) and its dynamic initializer
// (0x0055CBC0, expanded into the .CRT$XCU entry): small cell offsets
// (0 or -1/-2 per axis). Only 7 constructors are expanded; the rest call
// 0x0042AD30.

Cell unknown_cells_a6f920[42] = {
    Cell(0, -1), Cell(0, -1), Cell(0, -1), Cell(0, -1), Cell(0, -2), Cell(-1, -2),
    Cell(-1, -1), Cell(-1, -1), Cell(-1, 0), Cell(-1, 0), Cell(-1, 0), Cell(-1, 0),
    Cell(-2, -1), Cell(-2, 0), Cell(-1, 0), Cell(-1, 0), Cell(0, 0), Cell(0, 0),
    Cell(0, 0), Cell(0, 0), Cell(0, 0), Cell(-1, 0), Cell(0, 0), Cell(0, 0),
    Cell(0, 0), Cell(0, 0), Cell(0, 0), Cell(0, 0), Cell(0, -1), Cell(0, 0),
    Cell(0, -1), Cell(0, -1), Cell(-1, -1), Cell(-1, -1), Cell(-1, 0), Cell(-1, 0),
    Cell(0, 0), Cell(0, 0), Cell(0, -1), Cell(0, -1), Cell(0, 0), Cell(0, 0)
};


// Select and place a shore tile from adjacent water and straight-coast length, original0055CEF0.
extern Cell unknown_shore_offsets_a6f91c[42];
unsigned int __fastcall Map_Random_Range_005797a0(unsigned int minimum, unsigned int maximum);

bool MapClass::Place_Shore(CellClass *cellptr, int pass, int region_id, bool set_region)
{
    int random = Map_Random_Range_005797a0(0, 5);
    int mask;
    if (pass == 2) mask = Unknown_0055D410(cellptr, 1);
    else mask = Unknown_0055D410(cellptr, 0);
    if (mask == 0) return true;
    int index = -1;
    int count;
    CellClass *adj1;
    CellClass *adj2;
    bool side_1_is_land;
    bool side_2_is_water;
    if (pass == 2) {
        if ((mask & 0xA0) == 0xA0) {
            if ((mask & 0x11) == 0x11) index = (random & 1) + 23;
            else { if ((mask & 1) != 0) index = 21; else index = 30; }
        } else if ((mask & 0x82) == 0x82) {
            if ((mask & 0x44) == 0x44) index = (random & 1) + 15;
            else { if ((mask & 4) != 0) index = 14; else index = 22; }
        } else if ((mask & 0x0A) == 0x0A) {
            if ((mask & 0x11) == 0x11) index = (random & 1) + 7;
            else { if ((mask & 1) != 0) index = 13; else index = 6; }
        } else {
            if ((mask & 0x28) != 0x28) return true;
            if ((mask & 0x44) == 0x44) index = (random & 1) + 31;
            else { if ((mask & 4) != 0) index = 5; else index = 29; }
        }
    } else {
    if (mask & 2) {
        count = 1;
        adj1 = &cellptr->Adjacent_Cell(4);
        adj2 = &adj1->Adjacent_Cell(2);
        side_1_is_land = !adj1->unknown_47a270();
        side_2_is_water = adj2->unknown_47a270();
        while (side_1_is_land && side_2_is_water) {
            ++count;
            adj1 = &adj1->Adjacent_Cell(4);
            adj2 = &adj2->Adjacent_Cell(4);
            side_1_is_land = !adj1->unknown_47a270();
            side_2_is_water = adj2->unknown_47a270();
        }
        if ((count & 1) && !(mask & 0x80)) index = 12;
        if (index == -1) {
            if (!(mask & 4) || (mask & 0x18)) index = 12;
            else index = abs(random % 3) + 9;
        }
    } else if (mask & 0x20) {
        count = 1;
        adj1 = &cellptr->Adjacent_Cell(4);
        adj2 = &adj1->Adjacent_Cell(6);
        side_1_is_land = !adj1->unknown_47a270();
        side_2_is_water = adj2->unknown_47a270();
        while (side_1_is_land && side_2_is_water) {
            ++count;
            adj1 = &adj1->Adjacent_Cell(4);
            adj2 = &adj2->Adjacent_Cell(4);
            side_1_is_land = !adj1->unknown_47a270();
            side_2_is_water = adj2->unknown_47a270();
        }
        if ((count & 1) && !(mask & 0x80)) index = 28;
        if (index == -1) {
            if (!(mask & 0x10) || (mask & 0x0C)) index = 28;
            else index = random % 3 + 25;
        }
    } else if (mask & 8) {
        count = 1;
        adj1 = &cellptr->Adjacent_Cell(2);
        adj2 = &adj1->Adjacent_Cell(4);
        side_1_is_land = !adj1->unknown_47a270();
        side_2_is_water = adj2->unknown_47a270();
        while (side_1_is_land && side_2_is_water) {
            ++count;
            adj1 = &adj1->Adjacent_Cell(2);
            adj2 = &adj2->Adjacent_Cell(2);
            side_1_is_land = !adj1->unknown_47a270();
            side_2_is_water = adj2->unknown_47a270();
        }
        if (count & 1) index = 4;
        if (index == -1) {
            if (!(mask & 4) || (mask & 3)) index = 4;
            else index = random % 3 + 1;
        }
    } else if (mask & 0x80) {
        count = 1;
        adj1 = &cellptr->Adjacent_Cell(2);
        adj2 = &adj1->Adjacent_Cell(0);
        side_1_is_land = !adj1->unknown_47a270();
        side_2_is_water = adj2->unknown_47a270();
        while (side_1_is_land && side_2_is_water) {
            ++count;
            adj1 = &adj1->Adjacent_Cell(2);
            adj2 = &adj2->Adjacent_Cell(2);
            side_1_is_land = !adj1->unknown_47a270();
            side_2_is_water = adj2->unknown_47a270();
        }
        if (count & 1) index = 20;
        if (index == -1) {
            if (!(mask & 1) || (mask & 6)) index = 20;
            else index = random % 3 + 17;
        }
    }
    if (index == -1) {
        if (mask & 1) index = (random & 1) + 35;
        else if (mask & 4) index = (random & 1) + 33;
        else if (mask & 0x10) index = (random & 1) + 39;
        else {
            if (!(mask & 0x40)) return true;
            index = (random & 1) + 37;
        }
    }
    }
    if (index > 0) {
        Map.unknown_11a4 = IsoTileTypes[unknown_tileset_a6cb74 + index - 1];
        Cell cellid = cellptr->Fetch_CellID();
        Cell cursor = Map.Unknown_004994D0(cellid + unknown_shore_offsets_a6f91c[index]);
        bool result = true;
        if (pass == 1) Pick_Random_Tile_Variant(0, 0, cellptr->unknown_11b, region_id, result, set_region);
        else {
            if (pass != 2) return true;
            Pick_Random_Tile_Variant(unknown_tileset_a6cb74, unknown_tileset_a6cb74 + 41, cellptr->unknown_11b, region_id, result, set_region);
        }
        if (!result) return false;
    }
    return true;
}


// int MapClass::Unknown_0055D410(CellClass *cellptr, int control) (0x0055D410)
// The map generator's water mask of a cell (TibSun Get_Water_Mask): 0 outside the
// generator's diagonal bounds, for control 0 on a cell with a tile (0x0047B590), for
// control 1 on a water tile (14 tiles from unknown_tileset_a52580), or when its
// generator record is outside the fill reach (+0x4A); the record's mask (+0x40) when
// known; else one bit per neighbour that is water (0x0047A270), a missing neighbour
// counting as water when the cell itself is.
// My_In_Radar and Get_Cell_Data_0056dba0 are declared in 0x0055C520's source (this unit).

int MapClass::Unknown_0055D410(CellClass *cellptr, int control)
{
    int mask = 0;
    Cell cell = cellptr->Fetch_CellID();
    int water_start = unknown_tileset_a52580;
    int water_end = water_start + 14;
    bool is_water = cellptr->unknown_47a270();
    int x = cell.X;
    int y = cell.Y;

    if (!My_In_Radar(cell)) {
        return mask;
    }

    if (control == 0) {
        if (!cellptr->unknown_47b590()) {
            return mask;
        }
    } else if (control == 1) {
        if (cellptr->IsoTileIndex >= water_start && cellptr->IsoTileIndex < water_end) {
            return mask;
        }
    }

    UnknownMapGenerationCell &data = Get_Cell_Data_0056dba0(cellptr->Fetch_CellID());
    if (!data.unknown_45[5]) {
        return mask;
    }

    if (data.unknown_40 >= 0) {
        return data.unknown_40;
    }

    CellClass **cptr = &Cells[x + y * 512 - 512 - 1];
    if ((cptr[0] && cptr[0]->unknown_47a270()) || (!cptr[0] && is_water)) {
        mask |= 0x40;
    }
    cptr += 1;
    if ((cptr[0] && cptr[0]->unknown_47a270()) || (!cptr[0] && is_water)) {
        mask |= 0x80;
    }
    cptr += 1;
    if ((cptr[0] && cptr[0]->unknown_47a270()) || (!cptr[0] && is_water)) {
        mask |= 0x01;
    }
    cptr += 512 - 2;
    if ((cptr[0] && cptr[0]->unknown_47a270()) || (!cptr[0] && is_water)) {
        mask |= 0x20;
    }
    cptr += 2;
    if ((cptr[0] && cptr[0]->unknown_47a270()) || (!cptr[0] && is_water)) {
        mask |= 0x02;
    }
    cptr += 512 - 2;
    if ((cptr[0] && cptr[0]->unknown_47a270()) || (!cptr[0] && is_water)) {
        mask |= 0x10;
    }
    cptr += 1;
    if ((cptr[0] && cptr[0]->unknown_47a270()) || (!cptr[0] && is_water)) {
        mask |= 0x08;
    }
    cptr += 1;
    if ((cptr[0] && cptr[0]->unknown_47a270()) || (!cptr[0] && is_water)) {
        mask |= 0x04;
    }
    return mask;
}


// bool MapClass::Pick_Random_Tile_Variant(int first, int last, int height, int region_id, bool &result,
//     bool set_region) (0x0055D640)
// Places the pending isometric tile (Map +0x11A4) at the zone cell (Map +0x1170): every subtile cell inside the
// playfield diamond with a record is checked against the map generator's region (0x005818A0; set_region claims
// cell region 0). A foreign region allows only a matching shore group (0x7E0304) on a non-clear cell; the own region
// refuses shore facings 3-5 apart (0x7E03AC); a clear cell joins the region (0x00581870); an occupied unclaimed
// cell refuses unless region_id is -1 and is then skipped. Clear cells and tiles in [first, last] take the subtile;
// otherwise placement fails, setting result false only on a shore-vs-cliff conflict.
// Shape after TibSun MapClass::Pick_Random_Tile_Variant (map.cpp), our text; RA2 adds set_region and ends the
// unclaimed occupied arm with a `place` flag where TibSun has `continue` (the flag test is jump-threaded, which
// lays that arm after the shared exit as in the original).
extern int unknown_shore_groups_7e0304[42];
extern int unknown_shore_facings_7e03ac[42];
struct IsoTileCellHeight {
    char unknown_00[0x28];
    char Height;
};

bool MapClass::Pick_Random_Tile_Variant(int first, int last, int height, int region_id, bool &result, bool set_region)
{
    if (((AbstractClass *)Map.unknown_11a4)->Kind_Of() == 18) {
        if (set_region) region_id = 0;
        IsometricTileTypeClass *type = (IsometricTileTypeClass *)Map.unknown_11a4;
        IsoTileImageStruct const *image = (IsoTileImageStruct const *)type->Get_Image_Data();
        if (image != 0) {
            for (int row = 0; row < type->unknown_2b8; ++row) {
                for (int column = 0; column < type->unknown_2b4; ++column) {
                    Cell cell = XY_Cell(column + Map.unknown_1170.X, row + Map.unknown_1170.Y);
                    if (cell.X + cell.Y <= Map.MapSize.Width || cell.X - cell.Y >= Map.MapSize.Width ||
                        cell.Y - cell.X >= Map.MapSize.Width || cell.X + cell.Y > Map.MapSize.Width + 2 * Map.MapSize.Height)
                        continue;
                    CellClass *cellptr = &Map[cell];
                    int subtile = column + type->unknown_2b4 * row;
                    IsoTileCellHeight const *record = (IsoTileCellHeight const *)image->Tiles[subtile];
                    if (record == 0) continue;
                    int id = RandomMapGen.Get_Cell_Data_Region(cell);
                    if (set_region) id = 0;
                    bool place = true;
                    if (id > 0 && id != region_id && region_id != -1) {
                        int existing = cellptr->IsoTileIndex - unknown_tileset_a6cb74;
                        int pending = type->ArrayIndex - unknown_tileset_a6cb74;
                        if (!cellptr->unknown_47b590()) {
                            if (existing < 0 || pending < 0 || existing > 41 || pending > 41 ||
                                unknown_shore_groups_7e0304[existing] != unknown_shore_groups_7e0304[pending]) {
                                result = false;
                                return false;
                            }
                            return true;
                        }
                        RandomMapGen.Set_Cell_Data_Region(cell, region_id);
                    } else if (id == region_id) {
                        int existing = cellptr->IsoTileIndex - unknown_tileset_a6cb74;
                        int pending = type->ArrayIndex - unknown_tileset_a6cb74;
                        if (existing >= 0 && pending >= 0 && existing <= 41 && pending <= 41) {
                            int difference = abs(unknown_shore_facings_7e03ac[existing] - unknown_shore_facings_7e03ac[pending]);
                            if (difference >= 3 && difference <= 5) {
                                result = false;
                                return false;
                            }
                        }
                    } else if (cellptr->unknown_47b590()) {
                        RandomMapGen.Set_Cell_Data_Region(cell, region_id);
                    } else if (region_id != -1) {
                        result = false;
                        return false;
                    } else {
                        place = false;
                    }
                    if (place) {
                        if (cellptr->unknown_47b590() || (cellptr->IsoTileIndex >= first && cellptr->IsoTileIndex <= last)) {
                            cellptr->IsoTileIndex = type->ArrayIndex;
                            cellptr->unknown_11a = subtile;
                            cellptr->unknown_11b = height + record->Height;
                        } else {
                            if ((cellptr->unknown_47b7c0() && Unknown_0055AF80(type->ArrayIndex, subtile)) ||
                                (type->ArrayIndex >= unknown_tileset_a6cb74 && type->ArrayIndex < unknown_tileset_a6cb74 + 42 &&
                                 cellptr->unknown_47b5e0()))
                                result = false;
                            return false;
                        }
                    }
                }
            }
            return true;
        }
    }
    return false;
}


// void MapClass::Unknown_0055D990(Cell const &cell) (0x0055D990)
// Reduces tiberium around a cell (TibSun Area_Reduce_Tiberium): for each
// offset within 5 cells (11 x 11 square, Cell_Length of the offset up to 5)
// the centre cell itself, as in the original, gives up one unit (CellClass
// 0x00475DB0).

// The length of a cell offset in cells, truncated to a short (TibSun Cell::Length;
// X squared first, unlike unknown_geometry.hpp's Cell_Length).
static inline short Offset_Length(Cell const &cell)
{
    return (short)unknown_sqrt_4ba410((double)cell.X * cell.X + (double)cell.Y * cell.Y);
}

void MapClass::Unknown_0055D990(Cell const &cell)
{
    for (int x = -5; x <= 5; x++) {
        for (int y = -5; y <= 5; y++) {
            Cell other = Cell(cell + Cell(x, y));
            Cell offset = cell - other;
            if (Offset_Length(offset) <= 5) {
                CellClass *cellptr = &Map[cell];
                if (cellptr != 0) {
                    cellptr->unknown_475db0(1);
                }
            }
        }
    }
}


// int __fastcall Unknown_0055DA70(Cell const &cell) (0x0055DA70)
// Flags from the overlays of the cells at X - 1 and X + 1 of a cell (of the
// global Map): the right neighbour's overlay 0x4E, 0x50, 0x52 or 0x5D gives
// 1, 0x51 or 0x64 gives 2; the left one's 0x4F-0x51 or 0x5F adds 4, 0x52 or
// 0x64 adds 8.

int __fastcall Unknown_0055DA70(Cell const &cell)
{
    int left = Map[cell + Cell(-1, 0)].OverlayIndex;
    int right = Map[cell + Cell(1, 0)].OverlayIndex;
    int flags = 0;
    switch (right) {
    case 0x4E:
    case 0x50:
    case 0x52:
    case 0x5D:
        flags = 1;
        break;
    case 0x51:
    case 0x64:
        flags = 2;
        break;
    }
    switch (left) {
    case 0x4F:
    case 0x50:
    case 0x51:
    case 0x5F:
        flags |= 4;
        break;
    case 0x52:
    case 0x64:
        flags |= 8;
        break;
    }
    return flags;
}


// int __fastcall Unknown_0055DB90(Cell const &cell) (0x0055DB90)
// Flags from the overlays of the cells at Y - 1 and Y + 1 of a cell (of the
// global Map): the upper one's overlay gives 1 or 2, the lower one's adds
// 4 or 8.

int __fastcall Unknown_0055DB90(Cell const &cell)
{
    int up = Map[cell + Cell(0, -1)].OverlayIndex;
    int down = Map[cell + Cell(0, 1)].OverlayIndex;
    int flags = 0;
    switch (up) {
    case 0x57:
    case 0x59:
    case 0x5B:
    case 0x61:
        flags = 1;
        break;
    case 0x5A:
    case 0x65:
        flags = 2;
        break;
    }
    switch (down) {
    case 0x58:
    case 0x59:
    case 0x5A:
    case 0x63:
        flags |= 4;
        break;
    case 0x5B:
    case 0x65:
        flags |= 8;
        break;
    }
    return flags;
}


// bool MapClass::Unknown_0055DCA0(Cell const &cell) (0x0055DCA0)
// Tiberian Sun's Damage_Low_Bridge: for a low bridge overlay, finds the span's
// centre next to `cell` (through the low bridge test 0x00562930) and damages the
// EW (0x0055DEF0) or NS (0x0055E410) span there; true when it gave way.

bool MapClass::Unknown_0055DCA0(Cell const &cell)
{
    int overlay = Map[cell].OverlayIndex;

    if (overlay >= 0x4a && overlay <= 0x52 || overlay >= 0x5c && overlay <= 0x5f || overlay == 0x64) {
        if (!Unknown_00562930(cell - Cell(0, 1))) {
            return Unknown_0055DEF0(cell + Cell(0, 1));
        } else {
            if (!Unknown_00562930(cell - Cell(0, 2))) {
                return Unknown_0055DEF0(cell);
            } else {
                return Unknown_0055DEF0(cell - Cell(0, 1));
            }
        }
    } else if (overlay >= 0x53 && overlay <= 0x5b || overlay >= 0x60 && overlay <= 0x63 || overlay == 0x65) {
        if (!Unknown_00562930(cell - Cell(1, 0))) {
            return Unknown_0055E410(cell + Cell(1, 0));
        } else {
            if (!Unknown_00562930(cell - Cell(2, 0))) {
                return Unknown_0055E410(cell);
            } else {
                return Unknown_0055E410(cell - Cell(1, 0));
            }
        }
    }
    return false;
}


// Not exact (RA2_NON_MATCHING): MapClass::Unknown_0055DEF0 at 0x0055DEF0, best score 44.98, 2026-11-13; differs: 395 instructions in the original, 441 in the draft (shape 90.43)
#ifdef RA2_NON_MATCHING
// bool MapClass::Unknown_0055DEF0(Cell const &cell) (0x0055DEF0)
// Tiberian Sun's Damage_Low_Bridge_EW: damages the three cells of an east/west
// low bridge piece one step (0x5C -> 0x5D, 0x5E -> 0x5F, below 0x50 -> 0x50,
// below 0x53 -> 0x64 destroyed), damages the pieces beside it (0x0055FCC0),
// redraws the overlays, recalculates the cells and, once destroyed, springs the
// span's triggers (0x0055EA50) and resets the zones. True when it gave way.
extern Rect unknown_rect_839630;

bool MapClass::Unknown_0055DEF0(Cell const &cell)
{
    CellClass *cellptr1 = &Map[cell];
    CellClass *cellptr2 = &Map[cell - Cell(0, 1)];
    CellClass *cellptr3 = &Map[cell + Cell(0, 1)];

    int otype = cellptr1->OverlayIndex;
    bool didwork = false;

    Rect draw_rect = Union(cellptr1->Overlay_Render_Rect(), cellptr1->Overlay_Shadow_Render_Rect());
    Rect recalc_rect(0, 0, 0, 0);

    do {
        if (otype == 0x5c) {
            cellptr2->OverlayIndex = 0x5d;
            cellptr3->OverlayIndex = 0x5d;
            cellptr1->OverlayIndex = 0x5d;
            Damage_Low_Bridge_Piece_EW_0055FCC0(cell - Cell(1, 0));
        } else if (otype == 0x5e) {
            cellptr2->OverlayIndex = 0x5f;
            cellptr3->OverlayIndex = 0x5f;
            cellptr1->OverlayIndex = 0x5f;
            Damage_Low_Bridge_Piece_EW_0055FCC0(cell + Cell(1, 0));
        } else if (otype < 0x50) {
            cellptr2->OverlayIndex = 0x50;
            cellptr3->OverlayIndex = 0x50;
            cellptr1->OverlayIndex = 0x50;
            Damage_Low_Bridge_Piece_EW_0055FCC0(cell - Cell(1, 0));
            Damage_Low_Bridge_Piece_EW_0055FCC0(cell + Cell(1, 0));
        } else if (otype < 0x53) {
            cellptr2->OverlayIndex = 0x64;
            cellptr3->OverlayIndex = 0x64;
            cellptr1->OverlayIndex = 0x64;
            Map.Radar_Background(cellptr2->MapCoords);
            Map.Radar_Background(cellptr3->MapCoords);
            Map.Radar_Background(cellptr1->MapCoords);
            Damage_Low_Bridge_Piece_EW_0055FCC0(cell - Cell(1, 0));
            Damage_Low_Bridge_Piece_EW_0055FCC0(cell + Cell(1, 0));
            Spring_Low_Bridge_Destroyed_EW(cell);
            didwork = true;
            recalc_rect = Rect(cell.X - 1, cell.Y - 1, 3, 3);
        } else {
            break;
        }

        draw_rect = Union(draw_rect, Union(cellptr1->Overlay_Render_Rect(), cellptr1->Overlay_Shadow_Render_Rect())) - unknown_rect_839630.Top_Left();
        unknown_tactical_83996c->Register_Dirty_Area(draw_rect, false);

        cellptr1->Recalc_Attributes(-1);
        cellptr2->Recalc_Attributes(-1);
        cellptr3->Recalc_Attributes(-1);
        cellptr1->Kill_Illegal_Occupiers();
        cellptr2->Kill_Illegal_Occupiers();
        cellptr3->Kill_Illegal_Occupiers();

        if (didwork) {
            Zone_Reset();
        }

        if (recalc_rect.Is_Valid()) {
            Unknown_005687B0(recalc_rect);
        }
    } while (false);

    return didwork;
}
#endif

// Not exact (RA2_NON_MATCHING): MapClass::Unknown_0055E410 at 0x0055E410, best score 52.93, 2026-11-13; differs: 395 instructions in the original, 440 in the draft (shape 90.78)
#ifdef RA2_NON_MATCHING
// bool MapClass::Unknown_0055E410(Cell const &cell) (0x0055E410)
// north/south twin of 0x0055DEF0 (Damage_Low_Bridge_EW): piece 0x005601F0, spring 0x0055E930 (draft, Scout1029).
extern Rect unknown_rect_839630;
// ref ?Damage_Low_Bridge_Piece_NS_005601F0MapClassQAEXABVCellZ=0055fcc0
// ref ?Spring_Low_Bridge_Destroyed_EWMapClassQAEXVCellZ=0055ea50
// ref ?Overlay_Render_RectCellClassQBE?AVRectXZ=00474eb0
// ref ?Overlay_Shadow_Render_RectCellClassQBE?AVRectXZ=00475100
// ref ?Recalc_AttributesCellClassQAEXHZ=00472540
// ref ?Unknown_0047CC10CellClassQAEXXZ=0047cc10
// ref ??$UnionVRectYI?BVRectABV00Z=0045efe0
// ref ??GRectPoint2DQBE?AU0ABU0Z=0047d040
// ref ??0RectQAEABURectPoint2DHHZ=00453e40
// ref ?Register_Dirty_AreaTacticalQAEXVRect_NZ=006a3020
// ref ?unknown_tactical_83996c3PAVTacticalA=0083996c
// ref ?unknown_rect_8396303VRectA=00839630
// ref ?Unknown_00631570RadarClassQAEXABVCellZ=00631570
// ref ?Unknown_0054EBD0MapClassQAEHXZ=0054ebd0
// ref ?Unknown_005687B0MapClassQAEXABVRectZ=005687b0

bool MapClass::Unknown_0055E410(Cell const &cell)
{
    CellClass *cellptr1 = &Map[cell];
    CellClass *cellptr2 = &Map[cell - Cell(1, 0)];
    CellClass *cellptr3 = &Map[cell + Cell(1, 0)];

    int otype = cellptr1->OverlayIndex;
    bool didwork = false;

    Rect draw_rect = Union(cellptr1->Overlay_Render_Rect(), cellptr1->Overlay_Shadow_Render_Rect());
    Rect recalc_rect(0, 0, 0, 0);

    do {
        if (otype == 0x60) {
            cellptr2->OverlayIndex = 0x61;
            cellptr3->OverlayIndex = 0x61;
            cellptr1->OverlayIndex = 0x61;
            Damage_Low_Bridge_Piece_NS_005601F0(cell + Cell(0, 1));
        } else if (otype == 0x62) {
            cellptr2->OverlayIndex = 0x63;
            cellptr3->OverlayIndex = 0x63;
            cellptr1->OverlayIndex = 0x63;
            Damage_Low_Bridge_Piece_NS_005601F0(cell - Cell(0, 1));
        } else if (otype < 0x59) {
            cellptr2->OverlayIndex = 0x59;
            cellptr3->OverlayIndex = 0x59;
            cellptr1->OverlayIndex = 0x59;
            Damage_Low_Bridge_Piece_NS_005601F0(cell - Cell(0, 1));
            Damage_Low_Bridge_Piece_NS_005601F0(cell + Cell(0, 1));
        } else if (otype < 0x5c) {
            cellptr2->OverlayIndex = 0x65;
            cellptr3->OverlayIndex = 0x65;
            cellptr1->OverlayIndex = 0x65;
            Map.Radar_Background(cellptr2->MapCoords);
            Map.Radar_Background(cellptr3->MapCoords);
            Map.Radar_Background(cellptr1->MapCoords);
            Damage_Low_Bridge_Piece_NS_005601F0(cell - Cell(0, 1));
            Damage_Low_Bridge_Piece_NS_005601F0(cell + Cell(0, 1));
            Spring_Low_Bridge_Destroyed_NS(cell);
            didwork = true;
            recalc_rect = Rect(cell.X - 1, cell.Y - 1, 3, 3);
        } else {
            break;
        }

        draw_rect = Union(draw_rect, Union(cellptr1->Overlay_Render_Rect(), cellptr1->Overlay_Shadow_Render_Rect())) - unknown_rect_839630.Top_Left();
        unknown_tactical_83996c->Register_Dirty_Area(draw_rect, false);

        cellptr1->Recalc_Attributes(-1);
        cellptr2->Recalc_Attributes(-1);
        cellptr3->Recalc_Attributes(-1);
        cellptr1->Kill_Illegal_Occupiers();
        cellptr2->Kill_Illegal_Occupiers();
        cellptr3->Kill_Illegal_Occupiers();

        if (didwork) {
            Zone_Reset();
        }

        if (recalc_rect.Is_Valid()) {
            Unknown_005687B0(recalc_rect);
        }
    } while (false);

    return didwork;
}
#endif

// void MapClass::Spring_Low_Bridge_Destroyed_NS(Cell cell) (0x0055E930)
// Springs the destruction triggers of a north/south low bridge (TibSun name):
// the bridge's span runs from the last low bridge cell (0x00562930) north of
// the cell to the last one south of it (0x00558340).

void MapClass::Spring_Low_Bridge_Destroyed_NS(Cell cell)
{
    Cell tmp = cell;
    do {
        tmp = Adjacent_Cell_XY(tmp, 0);
    } while (Unknown_00562930(tmp));

    Cell start = Adjacent_Cell_XY(tmp, 4);

    tmp = cell;
    do {
        tmp = Adjacent_Cell_XY(tmp, 4);
    } while (Unknown_00562930(tmp));

    Cell end = Adjacent_Cell_XY(tmp, 0);

    Spring_Bridge_Destruction_Triggers(start, end);
}


// void MapClass::Spring_Low_Bridge_Destroyed_EW(Cell cell) (0x0055EA50)
// Springs the destruction triggers of an east/west low bridge (TibSun name):
// the bridge's span runs from the last low bridge cell (0x00562930) west of
// the cell to the last one east of it (0x00558340).

void MapClass::Spring_Low_Bridge_Destroyed_EW(Cell cell)
{
    Cell tmp = cell;
    do {
        tmp = Adjacent_Cell_XY(tmp, 6);
    } while (Unknown_00562930(tmp));

    Cell start = Adjacent_Cell_XY(tmp, 2);

    tmp = cell;
    do {
        tmp = Adjacent_Cell_XY(tmp, 2);
    } while (Unknown_00562930(tmp));

    Cell end = Adjacent_Cell_XY(tmp, 6);

    Spring_Bridge_Destruction_Triggers(start, end);
}


// int __fastcall Unknown_0055EB70(Cell const &cell) (0x0055EB70)
// Flags from the overlays of the cells at X - 1 and X + 1 of a cell (of the
// global Map): the right neighbour's overlay 0xD1, 0xD3, 0xD5 or 0xE0 gives
// 1, 0xD4 or 0xE7 gives 2; the left one's 0xD2-0xD4 or 0xE2 adds 4, 0xD5 or
// 0xE7 adds 8.

int __fastcall Unknown_0055EB70(Cell const &cell)
{
    int left = Map[cell + Cell(-1, 0)].OverlayIndex;
    int right = Map[cell + Cell(1, 0)].OverlayIndex;
    int flags = 0;
    switch (right) {
    case 0xD1:
    case 0xD3:
    case 0xD5:
    case 0xE0:
        flags = 1;
        break;
    case 0xD4:
    case 0xE7:
        flags = 2;
        break;
    }
    switch (left) {
    case 0xD2:
    case 0xD3:
    case 0xD4:
    case 0xE2:
        flags |= 4;
        break;
    case 0xD5:
    case 0xE7:
        flags |= 8;
        break;
    }
    return flags;
}


// int __fastcall Unknown_0055ECA0(Cell const &cell) (0x0055ECA0)
// Flags from the overlays of the cells at Y - 1 and Y + 1 of a cell (of the
// global Map): the upper one's overlay gives 1 or 2, the lower one's adds
// 4 or 8.

int __fastcall Unknown_0055ECA0(Cell const &cell)
{
    int up = Map[cell + Cell(0, -1)].OverlayIndex;
    int down = Map[cell + Cell(0, 1)].OverlayIndex;
    int flags = 0;
    switch (up) {
    case 0xDA:
    case 0xDC:
    case 0xDE:
    case 0xE4:
        flags = 1;
        break;
    case 0xDD:
    case 0xE8:
        flags = 2;
        break;
    }
    switch (down) {
    case 0xDB:
    case 0xDC:
    case 0xDD:
    case 0xE6:
        flags |= 4;
        break;
    case 0xDE:
    case 0xE8:
        flags |= 8;
        break;
    }
    return flags;
}


// bool MapClass::Unknown_0055EDB0(Cell const &cell) (0x0055EDB0)
// Tiberian Sun's Damage_Low_Bridge for the second low bridge set: for its overlay, finds the span's
// centre next to `cell` (through the low bridge test 0x00562980) and damages the
// EW (0x0055F020) or NS (0x0055F540) span there; true when it gave way.

bool MapClass::Unknown_0055EDB0(Cell const &cell)
{
    int overlay = Map[cell].OverlayIndex;

    if (overlay >= 0xcd && overlay <= 0xd5 || overlay >= 0xdf && overlay <= 0xe2 || overlay == 0xe7) {
        if (!Unknown_00562980(cell - Cell(0, 1))) {
            return Unknown_0055F020(cell + Cell(0, 1));
        } else {
            if (!Unknown_00562980(cell - Cell(0, 2))) {
                return Unknown_0055F020(cell);
            } else {
                return Unknown_0055F020(cell - Cell(0, 1));
            }
        }
    } else if (overlay >= 0xd6 && overlay <= 0xde || overlay >= 0xe3 && overlay <= 0xe6 || overlay == 0xe8) {
        if (!Unknown_00562980(cell - Cell(1, 0))) {
            return Unknown_0055F540(cell + Cell(1, 0));
        } else {
            if (!Unknown_00562980(cell - Cell(2, 0))) {
                return Unknown_0055F540(cell);
            } else {
                return Unknown_0055F540(cell - Cell(1, 0));
            }
        }
    }
    return false;
}


// Not exact (RA2_NON_MATCHING): MapClass::Unknown_0055F020 at 0x0055F020, best score 44.98, 2026-11-13; differs: 395 instructions in the original, 441 in the draft (shape 90.43)
#ifdef RA2_NON_MATCHING
// bool MapClass::Unknown_0055F020(Cell const &cell) (0x0055F020)
// east/west-set-2 twin of 0x0055DEF0 (Damage_Low_Bridge_EW): piece 0x005606D0, spring 0x0055FB90 (draft, Scout1029).
extern Rect unknown_rect_839630;
// ref ?Damage_Low_Bridge2_Piece_EW_005606D0MapClassQAEXABVCellZ=0055fcc0
// ref ?Spring_Low_Bridge_Destroyed_EWMapClassQAEXVCellZ=0055ea50
// ref ?Overlay_Render_RectCellClassQBE?AVRectXZ=00474eb0
// ref ?Overlay_Shadow_Render_RectCellClassQBE?AVRectXZ=00475100
// ref ?Recalc_AttributesCellClassQAEXHZ=00472540
// ref ?Unknown_0047CC10CellClassQAEXXZ=0047cc10
// ref ??$UnionVRectYI?BVRectABV00Z=0045efe0
// ref ??GRectPoint2DQBE?AU0ABU0Z=0047d040
// ref ??0RectQAEABURectPoint2DHHZ=00453e40
// ref ?Register_Dirty_AreaTacticalQAEXVRect_NZ=006a3020
// ref ?unknown_tactical_83996c3PAVTacticalA=0083996c
// ref ?unknown_rect_8396303VRectA=00839630
// ref ?Unknown_00631570RadarClassQAEXABVCellZ=00631570
// ref ?Unknown_0054EBD0MapClassQAEHXZ=0054ebd0
// ref ?Unknown_005687B0MapClassQAEXABVRectZ=005687b0

bool MapClass::Unknown_0055F020(Cell const &cell)
{
    CellClass *cellptr1 = &Map[cell];
    CellClass *cellptr2 = &Map[cell - Cell(0, 1)];
    CellClass *cellptr3 = &Map[cell + Cell(0, 1)];

    int otype = cellptr1->OverlayIndex;
    bool didwork = false;

    Rect draw_rect = Union(cellptr1->Overlay_Render_Rect(), cellptr1->Overlay_Shadow_Render_Rect());
    Rect recalc_rect(0, 0, 0, 0);

    do {
        if (otype == 0xdf) {
            cellptr2->OverlayIndex = 0xe0;
            cellptr3->OverlayIndex = 0xe0;
            cellptr1->OverlayIndex = 0xe0;
            Damage_Low_Bridge2_Piece_EW_005606D0(cell - Cell(1, 0));
        } else if (otype == 0xe1) {
            cellptr2->OverlayIndex = 0xe2;
            cellptr3->OverlayIndex = 0xe2;
            cellptr1->OverlayIndex = 0xe2;
            Damage_Low_Bridge2_Piece_EW_005606D0(cell + Cell(1, 0));
        } else if (otype < 0xd3) {
            cellptr2->OverlayIndex = 0xd3;
            cellptr3->OverlayIndex = 0xd3;
            cellptr1->OverlayIndex = 0xd3;
            Damage_Low_Bridge2_Piece_EW_005606D0(cell - Cell(1, 0));
            Damage_Low_Bridge2_Piece_EW_005606D0(cell + Cell(1, 0));
        } else if (otype < 0xd6) {
            cellptr2->OverlayIndex = 0xe7;
            cellptr3->OverlayIndex = 0xe7;
            cellptr1->OverlayIndex = 0xe7;
            Map.Radar_Background(cellptr2->MapCoords);
            Map.Radar_Background(cellptr3->MapCoords);
            Map.Radar_Background(cellptr1->MapCoords);
            Damage_Low_Bridge2_Piece_EW_005606D0(cell - Cell(1, 0));
            Damage_Low_Bridge2_Piece_EW_005606D0(cell + Cell(1, 0));
            Unknown_0055FB90(cell);
            didwork = true;
            recalc_rect = Rect(cell.X - 1, cell.Y - 1, 3, 3);
        } else {
            break;
        }

        draw_rect = Union(draw_rect, Union(cellptr1->Overlay_Render_Rect(), cellptr1->Overlay_Shadow_Render_Rect())) - unknown_rect_839630.Top_Left();
        unknown_tactical_83996c->Register_Dirty_Area(draw_rect, false);

        cellptr1->Recalc_Attributes(-1);
        cellptr2->Recalc_Attributes(-1);
        cellptr3->Recalc_Attributes(-1);
        cellptr1->Kill_Illegal_Occupiers();
        cellptr2->Kill_Illegal_Occupiers();
        cellptr3->Kill_Illegal_Occupiers();

        if (didwork) {
            Zone_Reset();
        }

        if (recalc_rect.Is_Valid()) {
            Unknown_005687B0(recalc_rect);
        }
    } while (false);

    return didwork;
}
#endif

// Not exact (RA2_NON_MATCHING): MapClass::Unknown_0055F540 at 0x0055F540, best score 52.93, 2026-11-13; differs: 395 instructions in the original, 440 in the draft (shape 90.78)
#ifdef RA2_NON_MATCHING
// bool MapClass::Unknown_0055F540(Cell const &cell) (0x0055F540)
// north/south-set-2 twin of 0x0055DEF0 (Damage_Low_Bridge_EW): piece 0x00560C10, spring 0x0055FA60 (draft, Scout1029).
extern Rect unknown_rect_839630;
// ref ?Damage_Low_Bridge2_Piece_NS_00560C10MapClassQAEXABVCellZ=0055fcc0
// ref ?Spring_Low_Bridge_Destroyed_EWMapClassQAEXVCellZ=0055ea50
// ref ?Overlay_Render_RectCellClassQBE?AVRectXZ=00474eb0
// ref ?Overlay_Shadow_Render_RectCellClassQBE?AVRectXZ=00475100
// ref ?Recalc_AttributesCellClassQAEXHZ=00472540
// ref ?Unknown_0047CC10CellClassQAEXXZ=0047cc10
// ref ??$UnionVRectYI?BVRectABV00Z=0045efe0
// ref ??GRectPoint2DQBE?AU0ABU0Z=0047d040
// ref ??0RectQAEABURectPoint2DHHZ=00453e40
// ref ?Register_Dirty_AreaTacticalQAEXVRect_NZ=006a3020
// ref ?unknown_tactical_83996c3PAVTacticalA=0083996c
// ref ?unknown_rect_8396303VRectA=00839630
// ref ?Unknown_00631570RadarClassQAEXABVCellZ=00631570
// ref ?Unknown_0054EBD0MapClassQAEHXZ=0054ebd0
// ref ?Unknown_005687B0MapClassQAEXABVRectZ=005687b0

bool MapClass::Unknown_0055F540(Cell const &cell)
{
    CellClass *cellptr1 = &Map[cell];
    CellClass *cellptr2 = &Map[cell - Cell(1, 0)];
    CellClass *cellptr3 = &Map[cell + Cell(1, 0)];

    int otype = cellptr1->OverlayIndex;
    bool didwork = false;

    Rect draw_rect = Union(cellptr1->Overlay_Render_Rect(), cellptr1->Overlay_Shadow_Render_Rect());
    Rect recalc_rect(0, 0, 0, 0);

    do {
        if (otype == 0xe3) {
            cellptr2->OverlayIndex = 0xe4;
            cellptr3->OverlayIndex = 0xe4;
            cellptr1->OverlayIndex = 0xe4;
            Damage_Low_Bridge2_Piece_NS_00560C10(cell + Cell(0, 1));
        } else if (otype == 0xe5) {
            cellptr2->OverlayIndex = 0xe6;
            cellptr3->OverlayIndex = 0xe6;
            cellptr1->OverlayIndex = 0xe6;
            Damage_Low_Bridge2_Piece_NS_00560C10(cell - Cell(0, 1));
        } else if (otype < 0xdc) {
            cellptr2->OverlayIndex = 0xdc;
            cellptr3->OverlayIndex = 0xdc;
            cellptr1->OverlayIndex = 0xdc;
            Damage_Low_Bridge2_Piece_NS_00560C10(cell - Cell(0, 1));
            Damage_Low_Bridge2_Piece_NS_00560C10(cell + Cell(0, 1));
        } else if (otype < 0xdf) {
            cellptr2->OverlayIndex = 0xe8;
            cellptr3->OverlayIndex = 0xe8;
            cellptr1->OverlayIndex = 0xe8;
            Map.Radar_Background(cellptr2->MapCoords);
            Map.Radar_Background(cellptr3->MapCoords);
            Map.Radar_Background(cellptr1->MapCoords);
            Damage_Low_Bridge2_Piece_NS_00560C10(cell - Cell(0, 1));
            Damage_Low_Bridge2_Piece_NS_00560C10(cell + Cell(0, 1));
            Unknown_0055FA60(cell);
            didwork = true;
            recalc_rect = Rect(cell.X - 1, cell.Y - 1, 3, 3);
        } else {
            break;
        }

        draw_rect = Union(draw_rect, Union(cellptr1->Overlay_Render_Rect(), cellptr1->Overlay_Shadow_Render_Rect())) - unknown_rect_839630.Top_Left();
        unknown_tactical_83996c->Register_Dirty_Area(draw_rect, false);

        cellptr1->Recalc_Attributes(-1);
        cellptr2->Recalc_Attributes(-1);
        cellptr3->Recalc_Attributes(-1);
        cellptr1->Kill_Illegal_Occupiers();
        cellptr2->Kill_Illegal_Occupiers();
        cellptr3->Kill_Illegal_Occupiers();

        if (didwork) {
            Zone_Reset();
        }

        if (recalc_rect.Is_Valid()) {
            Unknown_005687B0(recalc_rect);
        }
    } while (false);

    return didwork;
}
#endif

// void MapClass::Unknown_0055FA60(Cell cell) (0x0055FA60)
// Springs the destruction triggers of a north/south low bridge (the overlays 0xCD..0xE8):
// the bridge's span runs from the last low bridge cell (0x00562980) north of
// the cell to the last one south of it (0x00558340).

void MapClass::Unknown_0055FA60(Cell cell)
{
    Cell tmp = cell;
    do {
        tmp = Adjacent_Cell_XY(tmp, 0);
    } while (Unknown_00562980(tmp));

    Cell start = Adjacent_Cell_XY(tmp, 4);

    tmp = cell;
    do {
        tmp = Adjacent_Cell_XY(tmp, 4);
    } while (Unknown_00562980(tmp));

    Cell end = Adjacent_Cell_XY(tmp, 0);

    Spring_Bridge_Destruction_Triggers(start, end);
}


// void MapClass::Unknown_0055FB90(Cell cell) (0x0055FB90)
// Springs the destruction triggers of an east/west low bridge (the overlays 0xCD..0xE8):
// the bridge's span runs from the last low bridge cell (0x00562980) west of
// the cell to the last one east of it (0x00558340).

void MapClass::Unknown_0055FB90(Cell cell)
{
    Cell tmp = cell;
    do {
        tmp = Adjacent_Cell_XY(tmp, 6);
    } while (Unknown_00562980(tmp));

    Cell start = Adjacent_Cell_XY(tmp, 2);

    tmp = cell;
    do {
        tmp = Adjacent_Cell_XY(tmp, 2);
    } while (Unknown_00562980(tmp));

    Cell end = Adjacent_Cell_XY(tmp, 6);

    Spring_Bridge_Destruction_Triggers(start, end);
}


extern Rect unknown_rect_839630;
inline RectPoint2D Rect::Top_Left() const { return RectPoint2D(X, Y); }
inline Rect Rect::operator-(RectPoint2D const &point) const { return Rect(Top_Left() - point, Width, Height); }
// Damage one low-bridge piece using its neighboring span mask, original0055FCC0.
void MapClass::Damage_Low_Bridge_Piece_EW_0055FCC0(Cell const &cell)
{
    if (!Unknown_00562930(cell)) return;
    CellClass *center = &Map[cell];
    CellClass *side1 = &Map[cell - Cell(0, 1)];
    CellClass *side2 = &Map[cell + Cell(0, 1)];
    Rect draw_rect = Union(center->Overlay_Render_Rect(), center->Overlay_Shadow_Render_Rect());
    int lookup[16] = { -1, 0x4f, 0x52, -1, 0x4e, 0x50, 0x52, -1, 0x51, 0x51, 0x64, -1, -1, -1, -1, -1 };
    int mask = Unknown_0055DA70(cell);
    int current = Map[cell].OverlayIndex;
    if (mask > 0) {
        int next;
        if (current >= 0x5c) {
            if (current == 0x5c) next = 0x5d;
            else if (current == 0x5e) next = 0x5f;
            else return;
        } else {
            next = lookup[mask];
            if (current == next) return;
        }
        side2->OverlayIndex = side1->OverlayIndex = center->OverlayIndex = next;
        draw_rect = Union(draw_rect, Union(center->Overlay_Render_Rect(), center->Overlay_Shadow_Render_Rect())) - unknown_rect_839630.Top_Left();
        unknown_tactical_83996c->Register_Dirty_Area(draw_rect, false);
        if (next == 0x64) {
            Map.Radar_Background(cell);
            Map.Radar_Background(cell + Cell(0, 1));
            Map.Radar_Background(cell - Cell(0, 1));
        }
        center->Recalc_Attributes(-1);
        side1->Recalc_Attributes(-1);
        side2->Recalc_Attributes(-1);
        center->Kill_Illegal_Occupiers();
        side1->Kill_Illegal_Occupiers();
        side2->Kill_Illegal_Occupiers();
    }
}


extern Rect unknown_rect_839630;
// Damage one low-bridge piece using its neighboring span mask, original005601F0.
void MapClass::Damage_Low_Bridge_Piece_NS_005601F0(Cell const &cell)
{
    if (!Unknown_00562930(cell)) return;
    CellClass *center = &Map[cell];
    CellClass *side1 = &Map[cell - Cell(1, 0)];
    CellClass *side2 = &Map[cell + Cell(1, 0)];
    Rect draw_rect = Union(center->Overlay_Render_Rect(), center->Overlay_Shadow_Render_Rect());
    int lookup[16] = { -1, 0x58, 0x5b, -1, 0x57, 0x59, 0x5b, -1, 0x5a, 0x5a, 0x65, -1, -1, -1, -1, -1 };
    int mask = Unknown_0055DB90(cell);
    int current = center->OverlayIndex;
    if (mask > 0) {
        int next;
        if (current >= 0x60) {
            if (current == 0x60) next = 0x61;
            else if (current == 0x62) next = 0x63;
            else return;
        } else {
            next = lookup[mask];
            if (next == current) return;
        }
        side2->OverlayIndex = side1->OverlayIndex = center->OverlayIndex = next;
        draw_rect = Union(draw_rect, Union(center->Overlay_Render_Rect(), center->Overlay_Shadow_Render_Rect())) - unknown_rect_839630.Top_Left();
        unknown_tactical_83996c->Register_Dirty_Area(draw_rect, false);
        if (next == 0x65) {
            Map.Radar_Background(cell);
            Map.Radar_Background(cell + TPoint2D<short>(1, 0));
            Map.Radar_Background(cell - Cell(1, 0));
        }
        center->Recalc_Attributes(-1);
        side1->Recalc_Attributes(-1);
        side2->Recalc_Attributes(-1);
        center->Kill_Illegal_Occupiers();
        side1->Kill_Illegal_Occupiers();
        side2->Kill_Illegal_Occupiers();
    }
}


extern Rect unknown_rect_839630;
// Damage one low-bridge piece using its neighboring span mask, original005606D0.
void MapClass::Damage_Low_Bridge2_Piece_EW_005606D0(Cell const &cell)
{
    if (!Unknown_00562980(cell)) return;
    CellClass *center = &Map[cell];
    CellClass *side1 = &Map[cell - Cell(0, 1)];
    CellClass *side2 = &Map[cell + Cell(0, 1)];
    Rect draw_rect = Union(center->Overlay_Render_Rect(), center->Overlay_Shadow_Render_Rect());
    int lookup[16] = { -1, 0xd2, 0xd5, -1, 0xd1, 0xd3, 0xd5, -1, 0xd4, 0xd4, 0xe7, -1, -1, -1, -1, -1 };
    int mask = Unknown_0055EB70(cell);
    int current = Map[cell].OverlayIndex;
    if (mask > 0) {
        int next;
        if (current >= 0xdf) {
            if (current == 0xdf) next = 0xe0;
            else if (current == 0xe1) next = 0xe2;
            else return;
        } else {
            next = lookup[mask];
            if (current == next) return;
        }
        side2->OverlayIndex = side1->OverlayIndex = center->OverlayIndex = next;
        draw_rect = Union(draw_rect, Union(center->Overlay_Render_Rect(), center->Overlay_Shadow_Render_Rect())) - unknown_rect_839630.Top_Left();
        unknown_tactical_83996c->Register_Dirty_Area(draw_rect, false);
        if (next == 0xe7) {
            Map.Radar_Background(cell);
            Map.Radar_Background(cell + Cell(0, 1));
            Map.Radar_Background(cell - Cell(0, 1));
        }
        center->Recalc_Attributes(-1);
        side1->Recalc_Attributes(-1);
        side2->Recalc_Attributes(-1);
        center->Kill_Illegal_Occupiers();
        side1->Kill_Illegal_Occupiers();
        side2->Kill_Illegal_Occupiers();
    }
}


extern Rect unknown_rect_839630;
// Damage one low-bridge piece using its neighboring span mask, original00560C10.
void MapClass::Damage_Low_Bridge2_Piece_NS_00560C10(Cell const &cell)
{
    if (!Unknown_00562980(cell)) return;
    CellClass *center = &Map[cell];
    CellClass *side1 = &Map[cell - Cell(1, 0)];
    CellClass *side2 = &Map[cell + Cell(1, 0)];
    Rect draw_rect = Union(center->Overlay_Render_Rect(), center->Overlay_Shadow_Render_Rect());
    int lookup[16] = { -1, 0xdb, 0xde, -1, 0xda, 0xdc, 0xde, -1, 0xdd, 0xdd, 0xe8, -1, -1, -1, -1, -1 };
    int mask = Unknown_0055ECA0(cell);
    int current = center->OverlayIndex;
    if (mask > 0) {
        int next;
        if (current >= 0xe3) {
            if (current == 0xe3) next = 0xe4;
            else if (current == 0xe5) next = 0xe6;
            else return;
        } else {
            next = lookup[mask];
            if (next == current) return;
        }
        side2->OverlayIndex = side1->OverlayIndex = center->OverlayIndex = next;
        draw_rect = Union(draw_rect, Union(center->Overlay_Render_Rect(), center->Overlay_Shadow_Render_Rect())) - unknown_rect_839630.Top_Left();
        unknown_tactical_83996c->Register_Dirty_Area(draw_rect, false);
        if (next == 0xe8) {
            Map.Radar_Background(cell);
            Map.Radar_Background(cell + TPoint2D<short>(1, 0));
            Map.Radar_Background(cell - Cell(1, 0));
        }
        center->Recalc_Attributes(-1);
        side1->Recalc_Attributes(-1);
        side2->Recalc_Attributes(-1);
        center->Kill_Illegal_Occupiers();
        side1->Kill_Illegal_Occupiers();
        side2->Kill_Illegal_Occupiers();
    }
}


// void MapClass::Repair_Low_Bridge_Span(Cell const &cell) (0x00561100)
// Tiberian Sun's Repair_Low_Bridge_Span (name a hypothesis): for a low bridge overlay, finds
// the span's end cell next to `cell` (through the low bridge test 0x00562930) and
// rebuilds the EW (0x005615A0) or NS (0x00561A80) span from there; as Destroy_Low_Bridge_Span
// the last NS step is the Map unit's out-of-line TPoint2D<short>::operator- 0x0056AA40.

void MapClass::Repair_Low_Bridge_Span(Cell const &cell)
{
    int overlay = Map[cell].OverlayIndex;

    if (overlay >= 0x4a && overlay <= 0x52 || overlay >= 0x5c && overlay <= 0x5f || overlay == 0x64) {
        if (!Unknown_00562930(cell - Cell(0, 1))) {
            Repair_Low_Bridge_EW(cell + Cell(0, 1));
        } else {
            if (!Unknown_00562930(cell - Cell(0, 2))) {
                Repair_Low_Bridge_EW(cell);
            } else {
                Repair_Low_Bridge_EW(cell - Cell(0, 1));
            }
        }
    } else if (overlay >= 0x53 && overlay <= 0x5b || overlay >= 0x60 && overlay <= 0x63 || overlay == 0x65) {
        if (!Unknown_00562930(cell - Cell(1, 0))) {
            Repair_Low_Bridge_NS(cell + Cell(1, 0));
        } else {
            if (!Unknown_00562930(cell - Cell(2, 0))) {
                Repair_Low_Bridge_NS(cell);
            } else {
                Repair_Low_Bridge_NS(cell - Cell(1, 0));
            }
        }
    }
}


// void MapClass::Repair_Low_Bridge2_Span(Cell const &cell) (0x00561340)
// Tiberian Sun's Repair_Low_Bridge_Span for the second low bridge set (name a hypothesis): for a low bridge overlay, finds
// the span's end cell next to `cell` (through the low bridge test 0x00562980) and
// rebuilds the EW (0x00561F60) or NS (0x00562450) span from there; as Destroy_Low_Bridge_Span
// the last NS step is the Map unit's out-of-line TPoint2D<short>::operator- 0x0056AA40.

void MapClass::Repair_Low_Bridge2_Span(Cell const &cell)
{
    int overlay = Map[cell].OverlayIndex;

    if (overlay >= 0xcd && overlay <= 0xd5 || overlay >= 0xdf && overlay <= 0xe2 || overlay == 0xe7) {
        if (!Unknown_00562980(cell - Cell(0, 1))) {
            Repair_Low_Bridge2_EW(cell + Cell(0, 1));
        } else {
            if (!Unknown_00562980(cell - Cell(0, 2))) {
                Repair_Low_Bridge2_EW(cell);
            } else {
                Repair_Low_Bridge2_EW(cell - Cell(0, 1));
            }
        }
    } else if (overlay >= 0xd6 && overlay <= 0xde || overlay >= 0xe3 && overlay <= 0xe6 || overlay == 0xe8) {
        if (!Unknown_00562980(cell - Cell(1, 0))) {
            Repair_Low_Bridge2_NS(cell + Cell(1, 0));
        } else {
            if (!Unknown_00562980(cell - Cell(2, 0))) {
                Repair_Low_Bridge2_NS(cell);
            } else {
                Repair_Low_Bridge2_NS(cell - Cell(1, 0));
            }
        }
    }
}


// Rebuild the east/west low bridge run, original005615A0.
unsigned int __fastcall Map_Random_Range_005797a0(unsigned int minimum, unsigned int maximum);
extern Rect unknown_rect_839630;

void MapClass::Repair_Low_Bridge_EW(Cell const &cell)
{
    // These known 0/1 predicates are consumed through AL in the original.
    unsigned char low_bridge;
    Cell cursor = cell;
    bool changed = false;
    do {
        --cursor.X;
        low_bridge = (unsigned char)Unknown_00562930(cursor);
    } while (low_bridge);
    ++cursor.X;
    Rect recalc_rect(0, 0, 0, 0);
    do {
        CellClass *center = &Map[cursor];
        CellClass *side1 = &Map[cursor - Cell(0, 1)];
        CellClass *side2 = &Map[cursor + Cell(0, 1)];
        Rect draw_rect = Union(center->Overlay_Render_Rect(), center->Overlay_Shadow_Render_Rect());
        int old_overlay = center->OverlayIndex;
        int new_overlay = old_overlay;
        switch (old_overlay) {
        case 0x5c: case 0x5d:
            new_overlay = 0x5c;
            break;
        case 0x5e: case 0x5f:
            new_overlay = 0x5e;
            break;
        case 0x4e:
        case 0x4f:
        case 0x50:
        case 0x51:
        case 0x52:
        case 0x64:
            new_overlay = Map_Random_Range_005797a0(0, 3) + 0x4a;
            recalc_rect = Union(recalc_rect, Rect(cursor.X, cursor.Y - 1, 1, 3));
            changed = true;
            break;
        default:
            break;
        }
        if (new_overlay != old_overlay) {
            center->OverlayIndex = new_overlay;
            side1->OverlayIndex = new_overlay;
            side2->OverlayIndex = new_overlay;
            draw_rect = Union(draw_rect, Union(center->Overlay_Render_Rect(), center->Overlay_Shadow_Render_Rect())) - unknown_rect_839630.Top_Left();
            unknown_tactical_83996c->Register_Dirty_Area(draw_rect, false);
            if (old_overlay == 0x64) {
                Map.Radar_Background(cursor);
                Map.Radar_Background(cursor + Cell(0, 1));
                Map.Radar_Background(cursor - Cell(0, 1));
            }
            center->Recalc_Attributes(-1);
            side1->Recalc_Attributes(-1);
            side2->Recalc_Attributes(-1);
        }
        ++cursor.X;
        low_bridge = (unsigned char)Unknown_00562930(cursor);
    } while (low_bridge);
    if (changed) Zone_Reset();
    if (recalc_rect.Is_Valid()) Unknown_005687B0(recalc_rect);
}


// Rebuild the north/south low bridge run, original00561A80.
unsigned int __fastcall Map_Random_Range_005797a0(unsigned int minimum, unsigned int maximum);
extern Rect unknown_rect_839630;

void MapClass::Repair_Low_Bridge_NS(Cell const &cell)
{
    // These known 0/1 predicates are consumed through AL in the original.
    unsigned char low_bridge;
    Cell cursor = cell;
    Rect recalc_rect(0, 0, 0, 0);
    bool changed = false;
    do {
        --cursor.Y;
        low_bridge = (unsigned char)Unknown_00562930(cursor);
    } while (low_bridge);
    ++cursor.Y;
    do {
        CellClass *center = &Map[cursor];
        CellClass *side1 = &Map[cursor - Cell(1, 0)];
        CellClass *side2 = &Map[cursor + Cell(1, 0)];
        Rect draw_rect = Union(center->Overlay_Render_Rect(), center->Overlay_Shadow_Render_Rect());
        int old_overlay = center->OverlayIndex;
        int new_overlay = old_overlay;
        switch (old_overlay) {
        case 0x60: case 0x61:
            new_overlay = 0x60;
            break;
        case 0x62: case 0x63:
            new_overlay = 0x62;
            break;
        case 0x57:
        case 0x58:
        case 0x59:
        case 0x5a:
        case 0x5b:
        case 0x65:
            new_overlay = Map_Random_Range_005797a0(0, 3) + 0x53;
            recalc_rect = Union(recalc_rect, Rect(cursor.X - 1, cursor.Y, 3, 1));
            changed = true;
            break;
        default:
            break;
        }
        if (new_overlay != old_overlay) {
            center->OverlayIndex = new_overlay;
            side1->OverlayIndex = new_overlay;
            side2->OverlayIndex = new_overlay;
            draw_rect = Union(draw_rect, Union(center->Overlay_Render_Rect(), center->Overlay_Shadow_Render_Rect())) - unknown_rect_839630.Top_Left();
            unknown_tactical_83996c->Register_Dirty_Area(draw_rect, false);
            if (old_overlay == 0x65) {
                Map.Radar_Background(cursor);
                Map.Radar_Background(cursor + Cell(1, 0));
                Map.Radar_Background(cursor - Cell(1, 0));
            }
            center->Recalc_Attributes(-1);
            side1->Recalc_Attributes(-1);
            side2->Recalc_Attributes(-1);
        }
        ++cursor.Y;
        low_bridge = (unsigned char)Unknown_00562930(cursor);
    } while (low_bridge);
    if (changed) Zone_Reset();
    if (recalc_rect.Is_Valid()) Unknown_005687B0(recalc_rect);
}


// Rebuild the east/west low bridge run, original00561F60.
unsigned int __fastcall Map_Random_Range_005797a0(unsigned int minimum, unsigned int maximum);
extern Rect unknown_rect_839630;

void MapClass::Repair_Low_Bridge2_EW(Cell const &cell)
{
    // These known 0/1 predicates are consumed through AL in the original.
    unsigned char low_bridge;
    Cell cursor = cell;
    bool changed = false;
    do {
        --cursor.X;
        low_bridge = (unsigned char)Unknown_00562980(cursor);
    } while (low_bridge);
    ++cursor.X;
    Rect recalc_rect(0, 0, 0, 0);
    do {
        CellClass *center = &Map[cursor];
        CellClass *side1 = &Map[cursor - Cell(0, 1)];
        CellClass *side2 = &Map[cursor + Cell(0, 1)];
        Rect draw_rect = Union(center->Overlay_Render_Rect(), center->Overlay_Shadow_Render_Rect());
        int old_overlay = center->OverlayIndex;
        int new_overlay = old_overlay;
        switch (old_overlay) {
        case 0xdf: case 0xe0:
            new_overlay = 0xdf;
            break;
        case 0xe1: case 0xe2:
            new_overlay = 0xe1;
            break;
        case 0xd1:
        case 0xd2:
        case 0xd3:
        case 0xd4:
        case 0xd5:
        case 0xe7:
            new_overlay = Map_Random_Range_005797a0(0, 3) + 0xcd;
            recalc_rect = Union(recalc_rect, Rect(cursor.X, cursor.Y - 1, 1, 3));
            changed = true;
            break;
        default:
            break;
        }
        if (new_overlay != old_overlay) {
            center->OverlayIndex = new_overlay;
            side1->OverlayIndex = new_overlay;
            side2->OverlayIndex = new_overlay;
            draw_rect = Union(draw_rect, Union(center->Overlay_Render_Rect(), center->Overlay_Shadow_Render_Rect())) - unknown_rect_839630.Top_Left();
            unknown_tactical_83996c->Register_Dirty_Area(draw_rect, false);
            if (old_overlay == 0xe7) {
                Map.Radar_Background(cursor);
                Map.Radar_Background(cursor + Cell(0, 1));
                Map.Radar_Background(cursor - Cell(0, 1));
            }
            center->Recalc_Attributes(-1);
            side1->Recalc_Attributes(-1);
            side2->Recalc_Attributes(-1);
        }
        ++cursor.X;
        low_bridge = (unsigned char)Unknown_00562980(cursor);
    } while (low_bridge);
    if (changed) Zone_Reset();
    if (recalc_rect.Is_Valid()) Unknown_005687B0(recalc_rect);
}


// Rebuild the north/south low bridge run, original00562450.
unsigned int __fastcall Map_Random_Range_005797a0(unsigned int minimum, unsigned int maximum);
extern Rect unknown_rect_839630;

void MapClass::Repair_Low_Bridge2_NS(Cell const &cell)
{
    // These known 0/1 predicates are consumed through AL in the original.
    unsigned char low_bridge;
    Cell cursor = cell;
    Rect recalc_rect(0, 0, 0, 0);
    bool changed = false;
    do {
        --cursor.Y;
        low_bridge = (unsigned char)Unknown_00562980(cursor);
    } while (low_bridge);
    ++cursor.Y;
    do {
        CellClass *center = &Map[cursor];
        CellClass *side1 = &Map[cursor - Cell(1, 0)];
        CellClass *side2 = &Map[cursor + Cell(1, 0)];
        Rect draw_rect = Union(center->Overlay_Render_Rect(), center->Overlay_Shadow_Render_Rect());
        int old_overlay = center->OverlayIndex;
        int new_overlay = old_overlay;
        switch (old_overlay) {
        case 0xe3: case 0xe4:
            new_overlay = 0xe3;
            break;
        case 0xe5: case 0xe6:
            new_overlay = 0xe5;
            break;
        case 0xda:
        case 0xdb:
        case 0xdc:
        case 0xdd:
        case 0xde:
        case 0xe8:
            new_overlay = Map_Random_Range_005797a0(0, 3) + 0xd6;
            recalc_rect = Union(recalc_rect, Rect(cursor.X - 1, cursor.Y, 3, 1));
            changed = true;
            break;
        default:
            break;
        }
        if (new_overlay != old_overlay) {
            center->OverlayIndex = new_overlay;
            side1->OverlayIndex = new_overlay;
            side2->OverlayIndex = new_overlay;
            draw_rect = Union(draw_rect, Union(center->Overlay_Render_Rect(), center->Overlay_Shadow_Render_Rect())) - unknown_rect_839630.Top_Left();
            unknown_tactical_83996c->Register_Dirty_Area(draw_rect, false);
            if (old_overlay == 0xe8) {
                Map.Radar_Background(cursor);
                Map.Radar_Background(cursor + Cell(1, 0));
                Map.Radar_Background(cursor - Cell(1, 0));
            }
            center->Recalc_Attributes(-1);
            side1->Recalc_Attributes(-1);
            side2->Recalc_Attributes(-1);
        }
        ++cursor.Y;
        low_bridge = (unsigned char)Unknown_00562980(cursor);
    } while (low_bridge);
    if (changed) Zone_Reset();
    if (recalc_rect.Is_Valid()) Unknown_005687B0(recalc_rect);
}


// int __fastcall Unknown_00562930(Cell const &cell) (0x00562930)
// Whether the overlay of a cell (of the global Map) is in 0x4A..0x65.

int __fastcall Unknown_00562930(Cell const &cell)
{
    int overlay = Map[cell].OverlayIndex;
    return overlay >= 0x4a && overlay <= 0x65;
}


// int __fastcall Unknown_00562980(Cell const &cell) (0x00562980)
// Whether the overlay of a cell (of the global Map) is in 0xCD..0xE8.

int __fastcall Unknown_00562980(Cell const &cell)
{
    int overlay = Map[cell].OverlayIndex;
    return overlay >= 0xcd && overlay <= 0xe8;
}


// CellClass *MapClass::Unknown_005629D0(AbstractClass *object) (0x005629D0)
// TibSun Find_Nearby_Shroud: up to 24 shrouded cells (0x00568140 at the ground height) that
// pass 0x0055A660 are gathered from square rings around the object's cell, out to the map's
// width plus height but at most 32 rings; one of them at random, else the object's own cell.
// Each test coordinate is TibSun's `Coord coord(cell, 0)` (CoordStruct(Cell, z); Scout1010).

CellClass *MapClass::Unknown_005629D0(AbstractClass *object)
{
    Cell cells[24];

    Cell center = Coord_Cell(object->Center_Coord());
    int found = 0;
    int xx = center.X;
    int yy = center.Y;

    int radius = 32;
    if (MapSize.Width + MapSize.Height <= 32) {
        radius = MapSize.Width + MapSize.Height;
    }

    for (int ring = 0; ring < radius; ring++) {

        for (int x = -ring; x <= ring; x++) {

            Cell cell = Cell(xx + x, yy - ring);
            if (Unknown_0055A660(cell, true)) {
                CoordStruct coord = CoordStruct(cell, 0);
                coord.Z = Get_Height_GL(coord);
                if (Is_Shrouded(coord)) {
                    cells[found++] = cell;
                }
            }
            if (found == 24) {
                break;
            }

            cell = Cell(xx + x, yy + ring);
            if (Unknown_0055A660(cell, true)) {
                CoordStruct coord = CoordStruct(cell, 0);
                coord.Z = Get_Height_GL(coord);
                if (Is_Shrouded(coord)) {
                    cells[found++] = cell;
                }
            }
            if (found == 24) {
                break;
            }
        }
        if (found == 24) {
            break;
        }

        for (int y = -ring + 1; y <= ring - 1; y++) {

            Cell cell = Cell(xx - ring, yy + y);
            if (Unknown_0055A660(cell, true)) {
                CoordStruct coord = CoordStruct(cell, 0);
                coord.Z = Get_Height_GL(coord);
                if (Is_Shrouded(coord)) {
                    cells[found++] = cell;
                }
            }
            if (found == 24) {
                break;
            }

            cell = Cell(xx + ring, yy + y);
            if (Unknown_0055A660(cell, true)) {
                CoordStruct coord = CoordStruct(cell, 0);
                coord.Z = Get_Height_GL(coord);
                if (Is_Shrouded(coord)) {
                    cells[found++] = cell;
                }
            }
            if (found == 24) {
                break;
            }
        }
        if (found == 24) {
            break;
        }
    }

    if (found > 0) {
        return &Map[cells[Random_Pick(0, found - 1)]];
    }

    return &Map[center];
}


// void MapClass::Unknown_00562EA0() (0x00562EA0)
// Frees the zone data for Prog_End (TibSun Shutdown): the twelve unknown_18
// blocks, the unknown_14 hash table, and per subzone level the tracking
// vector (cleared) and the connection hash table; then the four global
// resets the destructor also calls.

void MapClass::Unknown_00562EA0()
{
    int index;
    for (index = 0; index < 12; index++) {
        if (unknown_18[index] != 0) {
            delete unknown_18[index];
            unknown_18[index] = 0;
        }
    }

    if (unknown_14 != 0) {
        delete unknown_14;
        unknown_14 = 0;
    }

    for (index = 0; index < 3; index++) {
        SubzoneTracking[index].Clear();
        if (unknown_7c[index] != 0) {
            delete unknown_7c[index];
            unknown_7c[index] = 0;
        }
    }

    Unknown_00710B50();
    Unknown_00711C70();
    Unknown_006E9670();
    Unknown_006EA130();
}


// void MapClass::Unknown_00562F50(CellClass *cell) (0x00562F50)
// Collapses the high bridge whose tile covers `cell`: the east-west (tile set 0xA6E0F8, 6 x ? cells)
// or north-south (tile set +1) bridge tile is replaced by its wrecked twin, the two end pieces
// (tile sets 0xA6E038 +0/+1, +3/+2) are placed, the zones of the tile's cells are rebuilt, the
// screen area is redrawn and grey debris (XGRYMED1/XGRYMED2/XGRYSML1) falls over the span.
// The debris statements are TibSun's (Scen->RandomNumber for the switch and the offsets, a named
// AnimClass result, no Cell casts in the dirty corners; Scout1010, TPoint3D coordinate profile).

void MapClass::Unknown_00562F50(CellClass *cell)
{
    if (cell->IsoTileIndex == unknown_tileset_a6e0f8) {
        Cell origin = cell->MapCoords + Cell(-((unsigned char)cell->unknown_11a % 6), -((unsigned char)cell->unknown_11a / 6));

        ObjectClass *object = IsoTileTypes[unknown_tileset_a6e0f8]->Create_One_Of(0);
        object->Set_Coord(origin);
        object->unknown_76 = false;
        object->unknown_74 = true;
        object->Mark(0);
        delete object;

        ObjectClass *slope1 = IsoTileTypes[unknown_tileset_a6e038]->Create_One_Of(0);
        slope1->Unlimbo(origin, 0);
        ObjectClass *slope2 = IsoTileTypes[unknown_tileset_a6e038 + 1]->Create_One_Of(0);
        slope2->Unlimbo((Cell)(origin + Cell(3, 0)), 0);

        Zone_Reset();

        IsometricTileTypeClass *isotype = IsoTileTypes[unknown_tileset_a6e0f8];
        IsoTileImageStruct *image = (IsoTileImageStruct *)isotype->Get_Image_Data();

        int x;
        int y;
        for (y = 0; y < isotype->unknown_2b8; y++) {
            for (x = 0; x < isotype->unknown_2b4; x++) {
                Cell here = origin + Cell(x, y);
                if (image->Tiles[y * isotype->unknown_2b4 + x] != 0 && Unknown_0055A660(here, true)) {
                    Fine_Subzone(here) = 0;
                    Map[here].Recalc_Attributes(-1);
                }
            }
        }
        for (y = 0; y < isotype->unknown_2b8; y++) {
            for (x = 0; x < isotype->unknown_2b4; x++) {
                Cell here = origin + Cell(x, y);
                if (image->Tiles[y * isotype->unknown_2b4 + x] != 0 && Unknown_0055A660(here, true)
                    && Fine_Subzone(here) == 0) {
                    Unknown_00566340(here);
                }
            }
        }
        for (y = 0; y < isotype->unknown_2b8; y++) {
            for (x = 0; x < isotype->unknown_2b4; x++) {
                Cell here = origin + Cell(x, y);
                if (image->Tiles[y * isotype->unknown_2b4 + x] != 0 && Unknown_0055A660(here, true)) {
                    Unknown_006D7C80(&Map[here]);
                    Map.Radar_Background(here);
                }
            }
        }

        Point2D start;
        Point2D end;
        unknown_tactical_83996c->Coord_To_Pixel(CoordStruct(origin, Map[origin].unknown_11b * unknown_geometry_g), start);
        unknown_tactical_83996c->Coord_To_Pixel(CoordStruct(origin + Cell(5, 3), Map[origin + Cell(5, 3)].unknown_11b * unknown_geometry_g), end);
        unknown_tactical_83996c->Register_Dirty_Area(Rect(start.X - 120, start.Y - 60, end.X - start.X + 240, end.Y - start.Y + 120), false);

        AnimTypeClass *med1 = AnimTypes[AnimTypeClass::From_Name("XGRYMED1")];
        AnimTypeClass *med2 = AnimTypes[AnimTypeClass::From_Name("XGRYMED2")];
        AnimTypeClass *sml1 = AnimTypes[AnimTypeClass::From_Name("XGRYSML1")];

        for (y = origin.Y; y < origin.Y + 3; y++) {
            for (x = origin.X; x < origin.X + 5; x++) {
                Cell piece = Cell(x, y);
                AnimTypeClass *type = 0;
                for (int i = 0; i < 2; i++) {
                    switch (unknown_id_source_a3d290->RandomNumber(0, 2)) {
                    case 0:
                        type = med1;
                        break;
                    case 1:
                        type = med2;
                        break;
                    case 2:
                        type = sml1;
                        break;
                    }
                    AnimClass *anim = new AnimClass(type, CoordStruct(piece, Map[piece].unknown_11b * unknown_geometry_g) + CoordStruct(unknown_id_source_a3d290->RandomNumber(-12, 12), unknown_id_source_a3d290->RandomNumber(-8, 8), 0), Random_Pick(0, 2), 1, 0x600, 0);
                }
            }
        }
    } else if (cell->IsoTileIndex == unknown_tileset_a6e0f8 + 1) {
        Cell origin = cell->MapCoords + Cell(-((unsigned char)cell->unknown_11a % 4), -((unsigned char)cell->unknown_11a / 4));

        ObjectClass *object = IsoTileTypes[unknown_tileset_a6e0f8 + 1]->Create_One_Of(0);
        object->Set_Coord(origin);
        object->unknown_76 = false;
        object->unknown_74 = true;
        object->Mark(0);
        delete object;

        ObjectClass *slope1 = IsoTileTypes[unknown_tileset_a6e038 + 3]->Create_One_Of(0);
        slope1->Unlimbo(origin, 0);
        ObjectClass *slope2 = IsoTileTypes[unknown_tileset_a6e038 + 2]->Create_One_Of(0);
        slope2->Unlimbo((Cell)(origin + Cell(0, 3)), 0);

        Zone_Reset();

        IsometricTileTypeClass *isotype = IsoTileTypes[unknown_tileset_a6e0f8 + 1];
        IsoTileImageStruct *image = (IsoTileImageStruct *)isotype->Get_Image_Data();

        int x;
        int y;
        int row_width;
        for (y = 0; y < isotype->unknown_2b8; y++) {
            row_width = isotype->unknown_2b4;
            for (x = 0; x < row_width; row_width = isotype->unknown_2b4, x++) {
                Cell here = origin + Cell(x, y);
                if (image->Tiles[y * row_width + x] != 0 && Unknown_0055A660(here, true)) {
                    Fine_Subzone(here) = 0;
                    Map[here].Recalc_Attributes(-1);
                }
            }
        }
        for (y = 0; y < isotype->unknown_2b8; y++) {
            row_width = isotype->unknown_2b4;
            for (x = 0; x < row_width; row_width = isotype->unknown_2b4, x++) {
                Cell here = origin + Cell(x, y);
                if (image->Tiles[y * row_width + x] != 0 && Unknown_0055A660(here, true)) {
                    if (Fine_Subzone(here) == 0) {
                        Unknown_00566340(here);
                    }
                }
            }
        }
        for (y = 0; y < isotype->unknown_2b8; y++) {
            row_width = isotype->unknown_2b4;
            for (x = 0; x < row_width; row_width = isotype->unknown_2b4, x++) {
                Cell here = origin + Cell(x, y);
                if (image->Tiles[y * row_width + x] != 0 && Unknown_0055A660(here, true)) {
                    Unknown_006D7C80(&Map[here]);
                    Map.Radar_Background(here);
                }
            }
        }

        Point2D start;
        Point2D end;
        unknown_tactical_83996c->Coord_To_Pixel(CoordStruct(origin, Map[origin].unknown_11b * unknown_geometry_g), start);
        unknown_tactical_83996c->Coord_To_Pixel(CoordStruct(origin + Cell(3, 5), Map[origin + Cell(3, 5)].unknown_11b * unknown_geometry_g), end);
        unknown_tactical_83996c->Register_Dirty_Area(Rect(end.X - 120, start.Y - 60, start.X - end.X + 240, end.Y - start.Y + 120), false);

        AnimTypeClass *med1 = AnimTypes[AnimTypeClass::From_Name("XGRYMED1")];
        AnimTypeClass *med2 = AnimTypes[AnimTypeClass::From_Name("XGRYMED2")];
        AnimTypeClass *sml1 = AnimTypes[AnimTypeClass::From_Name("XGRYSML1")];

        for (y = origin.Y; y < origin.Y + 5; y++) {
            for (x = origin.X; x < origin.X + 3; x++) {
                Cell piece = Cell(x, y);
                AnimTypeClass *type = 0;
                for (int i = 0; i < 2; i++) {
                    switch (unknown_id_source_a3d290->RandomNumber(0, 2)) {
                    case 0:
                        type = med1;
                        break;
                    case 1:
                        type = med2;
                        break;
                    case 2:
                        type = sml1;
                        break;
                    }
                    AnimClass *anim = new AnimClass(type, CoordStruct(piece, Map[piece].unknown_11b * unknown_geometry_g) + CoordStruct(unknown_id_source_a3d290->RandomNumber(-12, 12), unknown_id_source_a3d290->RandomNumber(-8, 8), 0), Random_Pick(0, 2), 1, 0x600, 0);
                }
            }
        }
    }
}


// void MapClass::Reset_All_Subzones() (0x00563D40)
// Rebuilds the subzone tracking: each of the three vectors, from the last,
// is cleared and refilled by 0x00563D80; then the path estimator is reset.

void MapClass::Reset_All_Subzones()
{
    for (int index = 2; index >= 0; index--) {
        SubzoneTracking[index].Clear();
        Reset_Subzone(index);
    }
    Search.Reset();
}


// Not exact (RA2_NON_MATCHING): MapClass::Unknown_00563D80 at 0x00563D80, best score 76.89, 2026-11-13; differs: 402 instructions in the original, 407 in the draft (shape 93.94)
#ifdef RA2_NON_MATCHING
// NON-EXACT DRAFT 0x563D80 MapClass::Reset_Subzone (Reset_Subzone)
// entry 0x563D80; exact-match score 76.89 (misses_index best); plan "draft, verify"
// origin lane: Scout660 (build/class-work/Scout664/w/keep/a6_7689.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1089 (native/drafts.json). Not original bytes.
// Native adaptation (Scout1089): 0x00A6FC34 under its HEAD name; 0x00564290 declared by a native map.hpp fixup. Changed lines marked NATIVE.
// MapClass::Reset_Subzone: TibSun Reset_Subzone; rebuilds one level of the subzone graph.
extern int unknown_zone_a6fc34;   // NATIVE: the same global as the accepted Zone_Span 0x0054F250 names it
#define unknown_last_adjacent_zone_a6fc34 unknown_zone_a6fc34   // NATIVE

inline unsigned short Packed_Low(unsigned int packed)
{
    return (unsigned short)packed;
}

inline unsigned short Packed_High(unsigned int packed)
{
    return (unsigned short)(packed >> 16);
}

inline int Threat_Region(int x, int y)
{
    return (short)x / 4 + 130 * ((short)y / 4) + 131;
}

inline void Set_Connection(SubzoneConnectionStruct &connection, int id, bool costly)
{
    connection.unknown_00 = id;
    connection.unknown_04 = costly;
}

void MapClass::Reset_Subzone(int level)
{
    CellSubzoneStruct *subend = (CellSubzoneStruct *)CellSubzones + CellZoneCount;
    HashTableClass<unsigned int, SubzoneConnectionStruct> *set = unknown_7c[level];
    DynamicVectorClass<SubzoneTrackingStruct> *track = &SubzoneTracking[level];
    for (int bucket = 0; bucket < set->BucketCount; ++bucket) set->Buckets[bucket].Clear();
    CellSubzoneStruct *subptr = (CellSubzoneStruct *)CellSubzones;
    UnknownMapElem4 *zoneptr = CellZones;
    if (subptr < subend) {
        do {
            subptr->SubzoneID[level] = 0;
            subptr->ZoneID = zoneptr->unknown_02;
            subptr->Height = zoneptr->unknown_01;
            ++subptr;
            ++zoneptr;
        } while (subptr < subend);
    }
    int entry_count = 1;
    track->Add(SubzoneTrackingStruct());
    (*track)[0].unknown_18 = 0;
    (*track)[0].unknown_1c = 7;
    subptr = (CellSubzoneStruct *)CellSubzones;
    zoneptr = CellZones;
    UnknownMapElem4 *zonerow = zoneptr;
    int width = 1 << (level + 1);
    int regionmask = width - 1;
    int rowwidth = Map.MapSize.Width + Map.MapSize.Height + 1;
    int x = 0;
    int rowx = 0;
    int y = 0;
    Rect bounds(0, 0, width, width);
    while (subptr < subend) {
        int passability = zoneptr->unknown_00;
        int parent;
        if (level + 1 < 3) parent = subptr->SubzoneID[level + 1];
        else parent = 0;
        if (passability == 7 || subptr->SubzoneID[level] != 0) {
            ++subptr;
            ++zoneptr;
            ++x;
            rowx = x;
            zonerow = zoneptr;
        } else {
            Cell cell;
            cell.Y = (short)y;
            unknown_last_adjacent_zone_a6fc34 = 0;
            cell.X = (short)x;
            int count = Unknown_00564290(subptr, level, entry_count, bounds, cell);
            track->Add(SubzoneTrackingStruct());
            SubzoneTrackingStruct *added = &(*track)[(unsigned short)entry_count];
            added->unknown_18 = (short)parent;
            added->unknown_1c = passability;
            added->Connections.Set_Growth_Step(16);
            added->unknown_20 = Threat_Region(x, y);
            zonerow += count;
            zoneptr = zonerow;
            ++entry_count;
            subptr += count;
            rowx += count;
            x = rowx;
        }
        if (x == rowwidth) {
            x = 0;
            ++y;
            rowx = 0;
            bounds.X = 0;
            bounds.Y = y - (regionmask & y);
        } else if ((x & regionmask) == 0) bounds.X = x;
    }
    unknown_70[level] = (unsigned short)entry_count;
    Unknown_00564B20(level);
    for (int bucket_index = 0; bucket_index < 256; ++bucket_index) {
        if (set->Buckets[bucket_index].Count() > 0) {
            HashObject<unsigned int, SubzoneConnectionStruct> *object = &set->Buckets[bucket_index][0];
            for (int left = set->Buckets[bucket_index].Count(); left > 0; --left) {
                unsigned packed = (unsigned)object->Value.unknown_00;
                unsigned short low = Packed_Low(packed);
                unsigned short high = Packed_High(packed);
                bool costly = object->Value.unknown_04;
                SubzoneConnectionStruct forward;
                Set_Connection(forward, high, costly);
                (*track)[low].Connections.Add(forward);
                SubzoneConnectionStruct backward;
                Set_Connection(backward, low, costly);
                (*track)[high].Connections.Add(backward);
                ++object;
            }
        }
    }
}
#endif

// Not exact (RA2_NON_MATCHING): FUN_00564290 at 0x00564290, best score 42.47, 2026-11-13; differs: 688 instructions in the original, 687 in the draft (shape 92.36)
#ifdef RA2_NON_MATCHING
// NON-EXACT DRAFT 0x564290 MapClass::Unknown_00564290 (Subzone_Span)
// entry 0x564290; exact-match score 42.47 (misses_index best); plan "draft, verify"
// origin lane: Scout660 (build/class-work/Scout664/w/keep/b_4247.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1089 (native/drafts.json). Not original bytes.
// Low exact score; the shape follows TibSun map.cpp Subzone_Span.
// MapClass::Unknown_00564290: TibSun Subzone_Span, grows one subzone from a seed cell (0x00564290).

inline int Span_Pack32(int zone1, int zone2)
{
    return (zone1 << 16) | zone2;
}

inline int Span_Pack8(int zone1, int zone2)
{
    return ((zone1 & 15) << 4) | (zone2 & 15);
}

int MapClass::Unknown_00564290(CellSubzoneStruct *seed, int level, unsigned int id, Rect const &bounds, Cell const &cell)
{
    int zone = seed->ZoneID;
    CellSubzoneStruct *begin = seed;
    CellSubzoneStruct *end = seed;
    HashObject<unsigned int, SubzoneConnectionStruct> entry;
    int x = cell.X;
    int y = cell.Y;
    int xmin = bounds.X;
    int ymin = bounds.Y;
    int last_adjacent = -1;
    int begin_x = x;
    int xmax = bounds.Width + bounds.X - 1;
    int end_x = x;
    int ymax = bounds.Height + bounds.Y - 1;
    HashTableClass<unsigned int, SubzoneConnectionStruct> *table = unknown_7c[level];
    int prev_height = seed->Height;

    while (true) {
        if (x < xmin) {
            break;
        }
        if (abs(begin->Height - prev_height) >= 2) {
            break;
        }
        begin->SubzoneID[level] = (short)id;
        prev_height = begin->Height;
        int prev_zone = begin[-1].ZoneID;
        begin--;
        x--;
        if (prev_zone != zone) {
            break;
        }
    }

    begin_x = x;
    int adjacent = begin->SubzoneID[level];
    if (adjacent > 0 && abs(begin->Height - prev_height) < 2 && adjacent != last_adjacent) {
        Cell probe;
        probe = Cell(x, y);
        if (Unknown_0055A660(probe, true)) {
            probe = Cell(x + 1, y);
            if (Unknown_0055A660(probe, true)) {
                int packed_pair = Span_Pack32(adjacent, (unsigned short)id);
                entry.Value.unknown_04 = false;
                entry.Key = entry.Value.unknown_00 = packed_pair;
                table->Add_Object(Span_Pack8(adjacent, (unsigned short)id), entry);
                last_adjacent = adjacent;
            }
        }
    }

    prev_height = seed->Height;
    while (end->ZoneID == zone) {
        if (end_x > xmax) {
            break;
        }
        if (abs(end->Height - prev_height) >= 2) {
            break;
        }
        end->SubzoneID[level] = (short)id;
        prev_height = end->Height;
        end++;
        end_x++;
    }

    adjacent = end->SubzoneID[level];
    if (adjacent > 0 && abs(end->Height - prev_height) < 2 && adjacent != last_adjacent) {
        Cell probe;
        probe.X = (short)end_x;
        probe.Y = (short)y;
        if (Unknown_0055A660(probe, true)) {
            probe = Cell(end_x - 1, y);
            if (Unknown_0055A660(probe, true)) {
                int packed_pair = Span_Pack32(adjacent, (unsigned short)id);
                entry.Value.unknown_04 = false;
                entry.Key = entry.Value.unknown_00 = packed_pair;
                table->Add_Object(Span_Pack8(adjacent, (unsigned short)id), entry);
                last_adjacent = adjacent;
            }
        }
    }

    int skip = end - seed - 1;
    int stride = MapSize.Width + MapSize.Height + 1;

    CellSubzoneStruct *above = &begin[-stride];
    CellSubzoneStruct *below = &begin[stride];
    x = begin_x;

    Cell span_cell;
    span_cell.X = 0;
    span_cell.Y = 0;

    while (x <= end_x) {
        int shadow_subzone = above->SubzoneID[level];
        CellSubzoneStruct *span_ptr;
        if (x == begin_x) {
            Cell left;
            left.Y = (short)y;
            left.X = (short)(x + 1);
            span_cell = left;
            span_ptr = &above[stride + 1];
        } else if (x < end_x) {
            Cell middle;
            middle.X = (short)x;
            middle.Y = (short)y;
            span_cell = middle;
            span_ptr = &above[stride];
        } else {
            Cell right;
            right.X = (short)(x - 1);
            right.Y = (short)y;
            span_cell = right;
            span_ptr = &above[stride - 1];
        }

        if (shadow_subzone != 0 || y <= ymin || x < xmin || x > xmax) {
            if (shadow_subzone != (unsigned short)id && shadow_subzone != last_adjacent) {
                if (shadow_subzone != 0) {
                    if (abs(span_ptr->Height - above->Height) < 2 && Unknown_0055A660(span_cell, true)) {
                        Cell shadow_cell;
                        shadow_cell.Y = (short)(y - 1);
                        shadow_cell.X = (short)x;
                        if (Unknown_0055A660(shadow_cell, true)) {
                            int packed_pair = Span_Pack32(shadow_subzone, (unsigned short)id);
                            if (x < xmin) {
                                entry.Value.unknown_04 = true;
                            } else {
                                entry.Value.unknown_04 = false;
                                if (x > xmax) {
                                    entry.Value.unknown_04 = true;
                                }
                            }
                            entry.Key = entry.Value.unknown_00 = packed_pair;
                            table->Add_Object(Span_Pack8(shadow_subzone, (unsigned short)id), entry);
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
                shadow_cell.X = (short)x;
                shadow_cell.Y = (short)(y - 1);
                Unknown_00564290(above, level, id, bounds, shadow_cell);
                continue;
            }
            above++;
            x++;
        } else {
            above++;
            x++;
        }
    }

    x = begin_x;
    while (x <= end_x) {
        int shadow_subzone = below->SubzoneID[level];
        CellSubzoneStruct *span_ptr;
        if (x == begin_x) {
            Cell left;
            left.X = (short)(x + 1);
            left.Y = (short)y;
            span_cell = left;
            span_ptr = &below[-stride + 1];
        } else if (x < end_x) {
            Cell middle;
            middle.X = (short)x;
            middle.Y = (short)y;
            span_cell = middle;
            span_ptr = &below[-stride];
        } else {
            Cell right;
            span_ptr = &below[-stride - 1];
            right.X = (short)(x - 1);
            right.Y = (short)y;
            span_cell = right;
        }

        if (shadow_subzone != 0 || y >= ymax || x < xmin || x > xmax) {
            if (shadow_subzone != (unsigned short)id && shadow_subzone != last_adjacent) {
                if (shadow_subzone != 0) {
                    if (abs(below->Height - span_ptr->Height) < 2 && Unknown_0055A660(span_cell, true)) {
                        Cell shadow_cell;
                        shadow_cell.X = (short)x;
                        shadow_cell.Y = (short)(y + 1);
                        if (Unknown_0055A660(shadow_cell, true)) {
                            int packed_pair = Span_Pack32(shadow_subzone, (unsigned short)id);
                            if (x < xmin) {
                                entry.Value.unknown_04 = true;
                            } else {
                                entry.Value.unknown_04 = false;
                                if (x > xmax) {
                                    entry.Value.unknown_04 = true;
                                }
                            }
                            entry.Key = entry.Value.unknown_00 = packed_pair;
                            table->Add_Object(Span_Pack8(shadow_subzone, (unsigned short)id), entry);
                            last_adjacent = shadow_subzone;
                        }
                    }
                }
            }
            below++;
            x++;
        } else if (below->ZoneID == zone && abs(below->Height - span_ptr->Height) < 2) {
            Unknown_00564290(below, level, id, bounds, Cell(x, y + 1));
        } else {
            below++;
            x++;
        }
    }

    return skip;
}
#endif

// void MapClass::Unknown_00564B20(int arg) (0x00564B20)
// Runs 0x00564B60 with arg on every zone connection whose +0x08 is set.

void MapClass::Unknown_00564B20(int arg)
{
    for (int index = 0; index < ZoneConnections.Count(); index++) {
        ZoneConnectionClass &connection = ZoneConnections[index];
        if (connection.unknown_08) {
            Unknown_00564B60(&connection, arg);
        }
    }
}


// MapClass::Unknown_00564B60: registers the center and both side subzone pairs of a bridge or tube connection.
extern int unknown_bridge_side_facings_7e0454[];

inline int Zone_Pack32(int from, int to)
{
    return (from << 16) | to;
}

inline int Zone_Pack8(int from, int to)
{
    return ((from & 15) << 4) | (to & 15);
}

inline void Set_Zone_Record(HashObject<unsigned int, SubzoneConnectionStruct> &record, unsigned int key, SubzoneConnectionStruct const &value)
{
    record.Key = key;
    record.Value.unknown_00 = value.unknown_00;
    record.Value.unknown_04 = value.unknown_04;
}

void MapClass::Unknown_00564B60(ZoneConnectionClass *connection, int layer)
{
    HashObject<unsigned int, SubzoneConnectionStruct> record;
    HashTableClass<unsigned int, SubzoneConnectionStruct> *table = unknown_7c[layer];
    Cell from = connection->From;
    Cell to = connection->To;
    Cell enter1;
    Cell enter2;
    Cell exit1;
    Cell exit2;
    CellClass *from_cell = &Map[from];
    if (!from_cell->unknown_47b960() && !from_cell->unknown_47b980()) {
        TubeClass *tube = from_cell->Get_Tunnel();
        int facing = tube->Facing;
        enter1 = Adjacent_Cell(from, facing + 2);
        enter2 = from + AdjacentCell[(facing - 2) & 7];
        CellClass *enterptr1 = &Map[enter1];
        TubeClass *tube1 = enterptr1->Get_Tunnel();
        CellClass *enterptr2 = &Map[enter2];
        TubeClass *tube2 = enterptr2->Get_Tunnel();
        if (!tube1 || !tube2) return;
        int count1 = tube1->unknown_1c0;
        exit1 = AStarClass::Follow_Path(enter1, count1, tube1->Directions);
        int count2 = tube2->unknown_1c0;
        exit2 = AStarClass::Follow_Path(enter2, count2, tube2->Directions);
    } else {
        bool is_bridge = from_cell->unknown_47b960();
        int tile_set = unknown_tileset_a52c74;
        int tile;
        if (!is_bridge) { tile = from_cell->IsoTileIndex; tile_set = unknown_tileset_a6cb68; }
        else tile = from_cell->IsoTileIndex;
        int facing = unknown_bridge_side_facings_7e0454[tile - tile_set];
        enter1 = Adjacent_Cell(from, facing);
        enter2 = Adjacent_Cell(from, facing - 4);
        exit1 = Adjacent_Cell(to, facing);
        exit2 = Adjacent_Cell(to, facing - 4);
    }
    {
        int from_id = ((CellSubzoneStruct *)CellSubzones)[Get_Cell_Zone_Index(from)].SubzoneID[layer];
        int to_id = ((CellSubzoneStruct *)CellSubzones)[Get_Cell_Zone_Index(to)].SubzoneID[layer];
        int value = Zone_Pack32(from_id, to_id);
        Set_Zone_Record(record, value, SubzoneConnectionStruct(value, false));
        table->Add_Object(Zone_Pack8(from_id, to_id), record);
    }
    {
        int from_id = ((CellSubzoneStruct *)CellSubzones)[Get_Cell_Zone_Index(enter1)].SubzoneID[layer];
        int to_id = ((CellSubzoneStruct *)CellSubzones)[Get_Cell_Zone_Index(exit1)].SubzoneID[layer];
        int value = Zone_Pack32(from_id, to_id);
        Set_Zone_Record(record, value, SubzoneConnectionStruct(value, false));
        table->Add_Object(Zone_Pack8(from_id, to_id), record);
    }
    {
        int from_id = ((CellSubzoneStruct *)CellSubzones)[Get_Cell_Zone_Index(enter2)].SubzoneID[layer];
        int to_id = ((CellSubzoneStruct *)CellSubzones)[Get_Cell_Zone_Index(exit2)].SubzoneID[layer];
        int value = Zone_Pack32(from_id, to_id);
        Set_Zone_Record(record, value, SubzoneConnectionStruct(value, false));
        table->Add_Object(Zone_Pack8(from_id, to_id), record);
    }
}


// Cell MapClass::Get_Bridge_Zone_Connection_Cell(CellClass *cptr, bool isbridge) (0x00564F70)
// The zone connection cell of a cell (TibSun Get_Bridge_Zone_Connection_Cell): a cell
// under a bridge (+0x140 bit 8) with isbridge maps to the connection at it
// (0x005500D0, distance 2), or without one to the bridge end of 0x005653C0 when there is
// one; the cell is offset along the connection (+0x140 bit 11: east/west) from its From.
// On a passable connection the nearer end, else walking east or south off the bridge,
// To when the cell reached is a bridge tile whose land type is not 3, else From.

Cell MapClass::Get_Bridge_Zone_Connection_Cell(CellClass *cptr, bool isbridge)
{
    Cell ncell;

    if (isbridge && cptr->IsUnderBridge) {
        int cidx = Map.Zone_Connection_Index(cptr->MapCoords, 2, 0);
        if (cidx == -1) {
            ncell = Unknown_005653C0(cptr->MapCoords, cptr->MapCoords);
            if ((Cell)ncell != unknown_cell_a6f238) {
                return (Cell)ncell;
            }
        }

        ZoneConnectionClass *zcon = &Map.ZoneConnections[cidx];
        if (cptr->IsBridgeEastWest) {
            ncell = Cell(0, cptr->MapCoords.Y - zcon->From.Y);
        } else {
            ncell = Cell(cptr->MapCoords.X - zcon->From.X, 0);
        }

        if (zcon->unknown_08) {
            Cell d1 = zcon->From + ncell - cptr->MapCoords;
            short dist1 = Cell_Distance(d1.X, d1.Y);
            Cell d2 = zcon->To + ncell - cptr->MapCoords;
            short dist2 = Cell_Distance(d2.X, d2.Y);
            if (dist1 < dist2) {
                return zcon->From + ncell;
            } else {
                return zcon->To + ncell;
            }
        } else {
            int facing = zcon->From.X != zcon->To.X ? 2 : 4;
            while (cptr->IsUnderBridge) {
                cptr = &cptr->Adjacent_Cell(facing);
            }
            if ((cptr->unknown_47b960() || cptr->unknown_47b980()) && cptr->unknown_ec != 3) {
                return zcon->To + ncell;
            } else {
                return zcon->From + ncell;
            }
        }
    }

    return cptr->MapCoords;
}


// Cell MapClass::Unknown_005651E0(Cell const &cell, Cell const &from) (0x005651E0)
// For a cell on a zone connection (0x005500D0, distance 1): with +0x08 set,
// the connection end nearer to from; otherwise walks from the cell along the
// connection (east when the ends' X differ, else south) while the cell's
// +0x140 bit 8 is set and returns To when the cell reached is a bridge tile
// (0x0047B960 / 0x0047B980) whose land type is not 3, else From. Off any
// connection, the cell from 0x005653C0 unless it is invalid, else the cell.

Cell MapClass::Unknown_005651E0(Cell const &cell, Cell const &from)
{
    int index = Map.Zone_Connection_Index(cell, 1, 0);
    if (index != -1) {
        ZoneConnectionClass &connection = Map.ZoneConnections[index];
        if (connection.unknown_08) {
            Cell d1 = from - connection.From;
            short dist1 = unknown_sqrt_4ba410((double)d1.X * d1.X + (double)d1.Y * d1.Y);
            Cell d2 = from - connection.To;
            short dist2 = unknown_sqrt_4ba410((double)d2.X * d2.X + (double)d2.Y * d2.Y);
            if (dist1 < dist2) {
                return connection.From;
            }
            return connection.To;
        }
        int facing = (short)(connection.From.X - connection.To.X) ? 2 : 4;
        CellClass *cellptr = &Map[cell];
        while (cellptr->IsUnderBridge) {
            cellptr = &cellptr->Adjacent_Cell(facing);
        }
        if ((cellptr->unknown_47b960() || cellptr->unknown_47b980()) && cellptr->unknown_ec != 3) {
            return connection.To;
        }
        return connection.From;
    }
    Cell result = Unknown_005653C0(cell, from);
    if (result != unknown_cell_a6f238) {
        return result;
    }
    return cell;
}


// Cell MapClass::Unknown_005653C0(Cell const &cell, Cell const &reference) (0x005653C0)
// The nearer end of the bridge over a cell (TibSun Find_Bridge_Span_End_Cell): a cell not
// under a bridge (+0x140 bit 8) is its own answer; otherwise walks forward (east on an
// east/west bridge, +0x140 bit 11, else south) and back until both walks leave the bridge,
// keeping the last bridge tile cell (0x0047B960 / 0x0047B980) of each whose land type is
// not 3. Ends in the local radar (0x0055A660 by height) qualify; with both, the one
// nearer to reference. The "no cell" value when neither does.
// Walk loop as the accepted sibling 0x00565610 writes it (while (true) with a joint exit test, the
// cells' MapCoords read directly); the end choice is one `||` test (TibSun splits it in two).

Cell MapClass::Unknown_005653C0(Cell const &cell, Cell const &reference)
{
    CellClass *cptr = &Map[cell];

    if (!cptr->IsUnderBridge) {
        return cptr->MapCoords;
    }

    int fwd = cptr->IsBridgeEastWest ? 2 : 4;
    int bwd = (fwd - 4) & 7;

    CellClass *fcell = cptr;
    CellClass *bcell = cptr;

    Cell fresult = unknown_cell_a6f238;
    Cell bresult = unknown_cell_a6f238;

    while (true) {
        if (fcell->IsUnderBridge) {
            fcell = &fcell->Adjacent_Cell(fwd);
            if (!fcell->IsUnderBridge &&
                (fcell->unknown_47b960() || fcell->unknown_47b980()) && fcell->unknown_ec != 3)
                fresult = fcell->MapCoords;
        }
        if (bcell->IsUnderBridge) {
            bcell = &bcell->Adjacent_Cell(bwd);
            if (!bcell->IsUnderBridge &&
                (bcell->unknown_47b960() || bcell->unknown_47b980()) && bcell->unknown_ec != 3)
                bresult = bcell->MapCoords;
        }
        if (!bcell->IsUnderBridge && !fcell->IsUnderBridge) break;
    }

    Cell result = unknown_cell_a6f238;

    if (Map.Unknown_0055A660(fresult, true)) {
        result = fresult;
    }

    if (Map.Unknown_0055A660(bresult, true)) {
        if (result == unknown_cell_a6f238 || bresult.Distance_To(reference) < fresult.Distance_To(reference)) {
            result = bresult;
        }
    }

    return result;
}


// Select a bridge span end or side cell in the requested subzone, original00565610.

Cell MapClass::Find_Bridge_End_Cell_For_Subzone(Cell const &cell, int layer, unsigned int subzone)
{
    CellClass *cellptr = &Map[cell];
    if (!cellptr->IsUnderBridge) return cellptr->MapCoords;
    int facing = cellptr->IsBridgeEastWest ? 2 : 4;
    int opposite = (facing - 4) & 7;
    CellClass *forward = cellptr;
    CellClass *back = cellptr;
    // Four-byte point-base view; the original local nominal type remains unknown.
    TPoint2D<short> front_end = unknown_cell_a6f238;
    Cell back_end = unknown_cell_a6f238;
    while (true) {
        if (forward->IsUnderBridge) {
            forward = &forward->Adjacent_Cell(facing);
            if (!forward->IsUnderBridge &&
                (forward->unknown_47b960() || forward->unknown_47b980()) && forward->unknown_ec != 3)
                front_end = forward->MapCoords;
        }
        if (back->IsUnderBridge) {
            back = &back->Adjacent_Cell(opposite);
            if (!back->IsUnderBridge &&
                (back->unknown_47b960() || back->unknown_47b980()) && back->unknown_ec != 3)
                back_end = back->MapCoords;
        }
        if (!back->IsUnderBridge && !forward->IsUnderBridge) break;
    }
    Cell candidates[6];
    candidates[0] = front_end;
    candidates[1] = Adjacent_Cell(front_end, (facing + 2) & 7);
    candidates[2] = Adjacent_Cell(front_end, (facing - 2) & 7);
    candidates[3] = back_end;
    candidates[4] = Adjacent_Cell(back_end, (facing + 2) & 7);
    candidates[5] = Adjacent_Cell(back_end, (facing - 2) & 7);
    for (int index = 0; index < 6; ++index) {
        Cell candidate = candidates[index];
        if (Map.Unknown_0055A660(candidate, true)) {
            int fine_index = Get_Cell_Zone_Index(candidate);
            CellSubzoneStruct *record = (CellSubzoneStruct *)CellSubzones + fine_index;
            if (record->SubzoneID[layer] == subzone)
                return candidate;
        }
    }
    return unknown_cell_a6f238;
}


// int MapClass::Unknown_005658C0(int &zone) (0x005658C0)
// Counts the zones (0x0054F8F0, type 10) of the local map's cells whose
// +0x4C is 4 (the walk 0x0055A910 over Map) and returns the count of the
// most common one, its zone in `zone` (-1 and 0 when none).

int MapClass::Unknown_005658C0(int &zone)
{
    DynamicVectorClass<int> counts;
    Map.unknown_118 = Map.MapLocalSize.Y + Map.MapLocalSize.X + 1;
    Map.unknown_11c = Map.MapSize.Width - Map.MapLocalSize.X + Map.MapLocalSize.Y;
    for (CellClass *cellptr = Map.Unknown_0055A910(); cellptr != 0; cellptr = Map.Unknown_0055A910()) {
        if (cellptr->unknown_4c == 4) {
            int z = Zone(cellptr->MapCoords, 10, false);
            if (z >= 0) {
                if (z >= counts.Count()) {
                    int need = z - counts.Count() + 1;
                    for (int index = 0; index < need; index++) {
                        counts.Add(0);
                    }
                }
                counts[z]++;
            }
        }
    }
    int best = -1;
    int most = 0;
    for (int index = 0; index < counts.Count(); index++) {
        if (counts[index] > most) {
            best = index;
            most = counts[index];
        }
    }
    zone = best;
    if (best >= 0) {
        return counts[best];
    }
    return 0;
}


// MapClass::Unknown_00565A50: collects clear cells on successive square rings around origin in one zone.

void MapClass::Unknown_00565A50(Cell const &origin, int radius_limit, int count_limit, int zone_type, int zone, DynamicVectorClass<Cell> &cells, int width, int height)
{
    for (int radius = 1; radius <= radius_limit; radius++) {
        int negative_radius = -radius;
        for (int offset = negative_radius; offset <= radius; ++offset) {
            Cell candidate = (Cell)(origin + Cell(radius, offset));
            if (Zone(candidate, zone_type, false) == zone) {
                if (Unknown_00565E20(Rect(candidate.X, candidate.Y, width, height)))
                    cells.Add(candidate);
            }
            candidate = (Cell)(origin + Cell(-radius, offset));
            if (Zone(candidate, zone_type, false) == zone) {
                if (Unknown_00565E20(Rect(candidate.X, candidate.Y, width, height)))
                    cells.Add(candidate);
            }
            if (offset != radius && offset != negative_radius) {
                candidate = (Cell)(origin + Cell(offset, radius));
                if (Zone(candidate, zone_type, false) == zone) {
                    if (Unknown_00565E20(Rect(candidate.X, candidate.Y, width, height)))
                        cells.Add(candidate);
                }
                candidate = (Cell)(origin + Cell(offset, negative_radius));
                if (Zone(candidate, zone_type, false) == zone) {
                    if (Unknown_00565E20(Rect(candidate.X, candidate.Y, width, height)))
                        cells.Add(candidate);
                }
            }
        }
        if (cells.Count() > count_limit) return;
    }
}


// bool MapClass::Unknown_00565E20(Rect const &rect) (0x00565E20)
// Whether every cell of a cell rectangle (of the global Map) has +0x4C
// 3 or 4.

bool MapClass::Unknown_00565E20(Rect const &rect)
{
    int height = rect.Height;
    int width = rect.Width;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int left = rect.X;
            int top = rect.Y;
            int cx = left + x;
            int cy = top + y;
            int kind = Map[Cell(cx, cy)].unknown_4c;
            if (kind != 4 && kind != 3) {
                return false;
            }
        }
    }
    return true;
}


// bool MapClass::Build_Reachable_Subzones(CellClass &cell, int layer, DynamicVectorClass<unsigned short> *list, int arg4) (0x00565EB0)
// TibSun Build_Reachable_Subzones: floods the layer's (1 << (layer + 1)) square block of
// cells from the cell through the cells of the same subzone that the techno (arg4) can enter
// (slot 101 answers 0, or the cell's land is not impassable for its movement zone),
// collecting the other subzones it touches. True when a cell of the starting subzone in the
// block was not reached; otherwise every connection of the starting subzone that the flood
// did not touch is added to list.

inline unsigned short Cell_Subzone(MapClass *map, Cell const &cell, int layer)
{
    return ((short *)(map->CellSubzones + map->Get_Cell_Zone_Index(cell) * 10))[layer];
}

bool MapClass::Build_Reachable_Subzones(CellClass *cptr, int layer, DynamicVectorClass<unsigned short> const &connections, ObjectClass const *foot)
{
    static CellClass *_subzone_flood_stack[64];
    static char _subzone_flood_visited[8][8];

    int dim = 1 << (layer + 1);

    int facing;
    DynamicVectorClass<unsigned short> list;
    list.Clear();

    if (dim > 0) {
        for (facing = 0; facing < dim; facing++) {
            memset(_subzone_flood_visited[facing], 0, dim);
        }
    }

    unsigned short start_subzone = Cell_Subzone(this, cptr->MapCoords, layer);
    unsigned short other_subzone = 0;
    _subzone_flood_stack[0] = cptr;
    _subzone_flood_visited[(dim - 1) & cptr->MapCoords.X][(dim - 1) & cptr->MapCoords.Y] = 1;
    int count = 1;

    int *mzone_table = unknown_table_7e00c4[foot->Techno_Type_Class()->unknown_500];

    while (true) {

        facing = 0;
        count--;
        CellClass **slot = &_subzone_flood_stack[count];
        CellClass *cur = *slot;

        while (true) {

            CellClass *adj = &cur->Adjacent_Cell(facing);
            int ax = (dim - 1) & adj->MapCoords.X;
            int ay = (dim - 1) & adj->MapCoords.Y;
            unsigned short adj_subzone = Cell_Subzone(this, adj->MapCoords, layer);

            if (foot->Can_Enter_Cell(adj, facing, adj->unknown_11b, 0, true) == 0 || mzone_table[adj->unknown_4c] != 1) {

                if (adj_subzone == start_subzone) {

                    if (!_subzone_flood_visited[ax][ay]) {
                        _subzone_flood_visited[ax][ay]++;
                        _subzone_flood_stack[count] = adj;
                        count++;
                    }

                } else if (adj_subzone != other_subzone) {

                    if (adj_subzone != 0) {
                        other_subzone = adj_subzone;

                        int j;
                        for (j = list.Count() - 1; j >= 0; j--) {
                            if (list[j] == adj_subzone) {
                                break;
                            }
                        }
                        if (j == -1) {
                            list.Add(adj_subzone);
                        }
                    }
                }
            }

            facing++;
            if (facing >= 8) {
                break;
            }
        }

        if (count <= 0) {
            break;
        }
    }

    int x = cptr->MapCoords.X;
    int y = cptr->MapCoords.Y;
    int bx = (dim - 1) & x;
    int by = (dim - 1) & y;

    for (facing = 0; facing < dim; facing++) {
        for (int col = 0; col < dim; col++) {

            Cell c = cptr->MapCoords + Cell(bx, by);

            if (Unknown_0055A660(c, true)) {
                if (start_subzone == ((unsigned short *)(CellSubzones + Get_Cell_Zone_Index(c) * 10))[layer] && !_subzone_flood_visited[facing][col]) {
                    return true;
                }
            }
        }
    }

    DynamicVectorClass<SubzoneConnectionStruct> &track = SubzoneTracking[layer][start_subzone].Connections;
    SubzoneConnectionStruct *vec = &track[0];
    for (int i = track.Count() - 1; i >= 0; i--) {
        SubzoneConnectionStruct &conn = vec[i];
        int sub = conn.unknown_00;
        int j;
        for (j = list.Count() - 1; j >= 0; j--) {
            if (list[j] == sub) {
                break;
            }
        }
        if (j == -1) {
            ((DynamicVectorClass<unsigned short> &)connections).Add(sub);
        }
    }

    return false;
}


// Remove both directions of a zone connection and its two bridge-side pairs, original00566C30.
extern int unknown_bridge_side_facings_7e0454[];

void MapClass::Unknown_00566C30(ZoneConnectionClass *connection)
{
    short to_id;
    Cell from = connection->From;
    Cell to = connection->To;
    CellClass *from_cell = &Map[from];
    Cell none = unknown_cell_a6f238;
    Cell from_side1 = none;
    Cell from_side2 = none;
    Cell to_side1 = none;
    Cell to_side2 = none;
    if (from_cell->unknown_47b960() || from_cell->unknown_47b980()) {
        bool is_bridge = from_cell->unknown_47b960();
        int tile_set = unknown_tileset_a52c74;
        int tile;
        if (!is_bridge) { tile = from_cell->IsoTileIndex; tile_set = unknown_tileset_a6cb68; }
        else tile = from_cell->IsoTileIndex;
        int facing = unknown_bridge_side_facings_7e0454[tile - tile_set];
        from_side1 = Adjacent_Cell(from, facing);
        from_side2 = Adjacent_Cell(from, (facing - 4) & 7);
        to_side1 = Adjacent_Cell(to, facing);
        to_side2 = Adjacent_Cell(to, (facing - 4) & 7);
    }
    for (int layer = 0; layer < 3; ++layer) {
        {
            int from_id = ((CellSubzoneStruct *)&Fine_Subzone(from))->SubzoneID[layer];
            to_id = ((CellSubzoneStruct *)&Fine_Subzone(to))->SubzoneID[layer];
            SubzoneTracking[layer][from_id].Connections.Delete(SubzoneConnectionStruct(to_id, false));
            SubzoneTracking[layer][to_id].Connections.Delete(SubzoneConnectionStruct(from_id, false));
        }
        {
            int from_id = ((CellSubzoneStruct *)&Fine_Subzone(from_side1))->SubzoneID[layer];
            to_id = ((CellSubzoneStruct *)&Fine_Subzone(to_side1))->SubzoneID[layer];
            SubzoneTracking[layer][from_id].Connections.Delete(SubzoneConnectionStruct(to_id, false));
            SubzoneTracking[layer][to_id].Connections.Delete(SubzoneConnectionStruct(from_id, false));
        }
        {
            int from_id = ((CellSubzoneStruct *)&Fine_Subzone(from_side2))->SubzoneID[layer];
            to_id = ((CellSubzoneStruct *)&Fine_Subzone(to_side2))->SubzoneID[layer];
            SubzoneTracking[layer][from_id].Connections.Delete(SubzoneConnectionStruct(to_id, false));
            SubzoneTracking[layer][to_id].Connections.Delete(SubzoneConnectionStruct(from_id, false));
        }
    }
}


// Add both directions of a zone connection and its two bridge-side pairs, original00566F90.
extern int unknown_bridge_side_facings_7e0454[];

void MapClass::Unknown_00566F90(ZoneConnectionClass *connection)
{
    int to_id;
    Cell from = connection->From;
    Cell to = connection->To;
    CellClass *from_cell = &Map[from];
    // Native initialization covers only to_side2; the bridge branch fills all four.
    // The three other locals retain the original nonbridge-path indeterminacy.
    Cell from_side1;
    Cell from_side2;
    Cell to_side1;
    Cell to_side2 = unknown_cell_a6f238;
    if (from_cell->unknown_47b960() || from_cell->unknown_47b980()) {
        bool is_bridge = from_cell->unknown_47b960();
        int tile_set = unknown_tileset_a52c74;
        int tile;
        if (!is_bridge) { tile = from_cell->IsoTileIndex; tile_set = unknown_tileset_a6cb68; }
        else tile = from_cell->IsoTileIndex;
        int facing = unknown_bridge_side_facings_7e0454[tile - tile_set];
        from_side1 = Adjacent_Cell(from, facing);
        from_side2 = Adjacent_Cell(from, (facing - 4) & 7);
        to_side1 = Adjacent_Cell(to, facing);
        to_side2 = Adjacent_Cell(to, (facing - 4) & 7);
    }
    for (int layer = 0; layer < 3; ++layer) {
        {
            int from_id = ((CellSubzoneStruct *)&Fine_Subzone(from))->SubzoneID[layer];
            to_id = ((CellSubzoneStruct *)&Fine_Subzone(to))->SubzoneID[layer];
            SubzoneConnectionStruct to_link;
            to_link.unknown_00 = to_id;
            to_link.unknown_04 = false;
            SubzoneTracking[layer][from_id].Connections.Add(to_link);
            SubzoneConnectionStruct from_link;
            from_link.unknown_00 = from_id;
            from_link.unknown_04 = false;
            SubzoneTracking[layer][to_id].Connections.Add(from_link);
        }
        {
            int from_id = ((CellSubzoneStruct *)&Fine_Subzone(from_side1))->SubzoneID[layer];
            to_id = ((CellSubzoneStruct *)&Fine_Subzone(to_side1))->SubzoneID[layer];
            SubzoneConnectionStruct to_link;
            to_link.unknown_00 = to_id;
            to_link.unknown_04 = false;
            SubzoneTracking[layer][from_id].Connections.Add(to_link);
            SubzoneConnectionStruct from_link;
            from_link.unknown_00 = from_id;
            from_link.unknown_04 = false;
            SubzoneTracking[layer][to_id].Connections.Add(from_link);
        }
        {
            int from_id = ((CellSubzoneStruct *)&Fine_Subzone(from_side2))->SubzoneID[layer];
            to_id = ((CellSubzoneStruct *)&Fine_Subzone(to_side2))->SubzoneID[layer];
            SubzoneConnectionStruct to_link;
            to_link.unknown_00 = to_id;
            to_link.unknown_04 = false;
            SubzoneTracking[layer][from_id].Connections.Add(to_link);
            SubzoneConnectionStruct from_link;
            from_link.unknown_00 = from_id;
            from_link.unknown_04 = false;
            SubzoneTracking[layer][to_id].Connections.Add(from_link);
        }
    }
}


// Not exact (RA2_NON_MATCHING): MapClass::Unknown_00566340 at 0x00566340, best score 64.37, 2026-11-13; differs: 709 instructions in the original, 711 in the draft (shape 98.45)
#ifdef RA2_NON_MATCHING
// NON-EXACT DRAFT 0x566340 MapClass::Unknown_00566340 (rebuild the subzone graph around a changed cell)
// entry 0x566340; exact-match score 64.37 (misses_index best; shape 98.45, calls equal); plan "draft, verify"
// origin lane: Scout664 (build/class-work/Scout664/w/keep/c_6437.cpp, overlay w/h2; Scout662 c_5141 before it)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1111 (native/drafts.json). Not original bytes. Native adaptation (Scout1111): compiled
// against HEAD's headers (one population macro added, marked NATIVE); reached when the computer player's
// building or wall marks an overlay (OverlayClass::Mark -> CellClass 0x00475960).
// Rebuild the subzone graph around one changed map cell, original00566340.
#define RA2_MAP_COLLAPSE_CLIFF   // NATIVE: MapClass::Fine_Subzone (shared/map.hpp; Scout664's overlay had it unconditionally)





inline int Cell_Passability(MapClass *map, Cell const &cell)
{
    return map->CellZones[map->Get_Cell_Zone_Index(cell)].unknown_00;
}

void MapClass::Unknown_00566340(Cell const &cell)
{
    if (Unknown_0055A660(cell, true)) {
        int y;
        int x;
        Cell trycell;
        for (int level = 2; level >= 0; --level) {
            Rect bounds;
            bounds.Width = 1 << (level + 1);
            bounds.Height = bounds.Width;
            int cx = cell.X;
            int cy = cell.Y;
            bounds.X = cx - cx % bounds.Width;
            bounds.Y = cy - cy % bounds.Height;
            DynamicVectorClass<unsigned short> collected;
            HashTableClass<unsigned int, SubzoneConnectionStruct> *set = unknown_7c[level];
            for (int bucket = 0; bucket < set->BucketCount; ++bucket) set->Buckets[bucket].Clear();
            DynamicVectorClass<SubzoneTrackingStruct> *track = &SubzoneTracking[level];
            for (y = bounds.Y; y < bounds.Y + bounds.Height; ++y) {
                for (x = bounds.X; x < bounds.Width + bounds.X; ++x) {
                    CellSubzoneStruct *subptr = (CellSubzoneStruct *)&Fine_Subzone(Cell(x, y));
                    unsigned short subid = (unsigned short)subptr->SubzoneID[level];
                    if (subid != 0) {
                        int found;
                        for (found = collected.Count() - 1; found >= 0; --found) {
                            if (collected[found] == subid) break;
                        }
                        if (found == -1) collected.Add(subid);
                    }
                    subptr->SubzoneID[level] = 0;
                    subptr->ZoneID = Unknown_0054FA60(Cell(x, y));
                }
            }
            for (int index = collected.Count() - 1; index >= 0; --index) {
                unsigned short id = collected[index];
                SubzoneTrackingStruct *entry = &SubzoneTracking[level][id];
                for (int connection = entry->Connections.Count() - 1; connection >= 0; --connection) {
                    SubzoneTrackingStruct *neighbor = &SubzoneTracking[level][entry->Connections[connection].unknown_00];
                    for (int reverse = neighbor->Connections.Count() - 1; reverse >= 0; --reverse) {
                        if (neighbor->Connections[reverse].unknown_00 == id) {
                            neighbor->Connections.Delete(reverse);
                            break;
                        }
                    }
                }
                entry->Connections.Clear();
            }
            int entry_count = unknown_70[level];
            for (y = bounds.Y; y < bounds.Y + bounds.Height; ++y) {
                for (x = bounds.X; x < bounds.Width + bounds.X; ++x) {
                    if (Unknown_0055A660(Cell(x, y), true)) {
                        CellSubzoneStruct *subptr = (CellSubzoneStruct *)&Fine_Subzone(Cell(x, y));
                        int passability = Cell_Passability(this, Cell(x, y));
                        if (passability != 7 && subptr->SubzoneID[level] == 0) {
                            trycell.Y = (short)y;
                            trycell.X = (short)x;
                            Unknown_00564290(subptr, level, entry_count, bounds, trycell);
                            track->Add(SubzoneTrackingStruct());
                            SubzoneTrackingStruct *added = &(*track)[entry_count];
                            int parent = 0;
                            if (level != 2) parent = subptr->SubzoneID[level + 1];
                            added->unknown_18 = (short)parent;
                            added->unknown_1c = passability;
                            added->Connections.Set_Growth_Step(16);
                            added->unknown_20 = Threat_Region(x, y);
                            ++entry_count;
                            if (entry_count == 0) {
                                Reset_All_Subzones();
                                return;
                            }
                        }
                    }
                }
            }
            unknown_70[level] = entry_count;
            for (int connection_index = ZoneConnections.Count() - 1; connection_index >= 0; --connection_index) {
                ZoneConnectionClass &connection = ZoneConnections[connection_index];
                if ((connection.From.X >= bounds.X && connection.From.X < bounds.Width + bounds.X &&
                     connection.From.Y >= bounds.Y && connection.From.Y < bounds.Y + bounds.Height) ||
                    (connection.To.X >= bounds.X && connection.To.X < bounds.Width + bounds.X &&
                     connection.To.Y >= bounds.Y && connection.To.Y < bounds.Y + bounds.Height)) {
                    if (connection.unknown_08) Unknown_00564B60(&connection, level);
                }
            }
            for (int bucket_index = 0; bucket_index < 256; ++bucket_index) {
                if (set->Buckets[bucket_index].Count() > 0) {
                    HashObject<unsigned int, SubzoneConnectionStruct> *object = &set->Buckets[bucket_index][0];
                    for (int remaining = set->Buckets[bucket_index].Count(); remaining > 0; --remaining) {
                        unsigned int packed = (unsigned int)object->Value.unknown_00;
                        unsigned short first = Packed_Low(packed);
                        unsigned short second = Packed_High(packed);
                        bool cross_block = object->Value.unknown_04;
                        SubzoneConnectionStruct forward;
                        forward.unknown_00 = second;
                        forward.unknown_04 = cross_block;
                        (*track)[first].Connections.Add(forward);
                        SubzoneConnectionStruct reverse;
                        reverse.unknown_00 = first;
                        reverse.unknown_04 = cross_block;
                        (*track)[second].Connections.Add(reverse);
                        ++object;
                    }
                }
            }
        }
        int base_x = cell.X - cell.X % 8;
        int base_y = cell.Y - cell.Y % 8;
        for (y = base_y; y < base_y + 8; ++y) {
            for (x = base_x; x < base_x + 8; ++x) {
                trycell.X = (short)x;
                trycell.Y = (short)y;
                if (Unknown_0055A660(trycell, true)) {
                    CellSubzoneStruct *subptr = (CellSubzoneStruct *)&Fine_Subzone(Cell(x, y));
                    for (int parent_level = 0; parent_level < 2; ++parent_level) {
                        SubzoneTracking[parent_level][subptr->SubzoneID[parent_level]].unknown_18 = subptr->SubzoneID[parent_level + 1];
                    }
                }
            }
        }
        Search.Reset();
    }
}
#endif

// void MapClass::Unknown_00567380() (0x00567380)
// Gives every wall overlay cell an owner (TibSun Initialize_Wall_Ownership):
// the house of the nearest building (ObjectClass 0x005D4C00) that is +0x7D and
// +0x74 and whose house type is a WallOwner goes into the cell's +0x50.

void MapClass::Unknown_00567380()
{
    Reset_Iterator();
    CellClass *cellptr = Iterate();
    while (cellptr != 0) {
        if (cellptr->OverlayIndex != -1) {
            if (unknown_overlay_types_a35de8[cellptr->OverlayIndex]->unknown_278) {
                int best = -1;
                int index = 0;
                int bestdist = 0x7FFFFFFF;
                for (index = 0; index < unknown_BuildingClass_list_00a40af8.Count(); index++) {
                    BuildingClass *building = unknown_BuildingClass_list_00a40af8[index];
                    int distance = building->unknown_5d4c00(cellptr);
                    if (building->unknown_7d && building->unknown_74 && building->House->Type->WallOwner) {
                        if (best == -1 || distance < bestdist) {
                            bestdist = distance;
                            best = index;
                        }
                    }
                }
                if (best != -1) {
                    cellptr->unknown_50 = unknown_BuildingClass_list_00a40af8[best]->House->ArrayIndex;
                }
            }
        }
        cellptr = Iterate();
    }
}


// bool MapClass::Rebuild_Shores_At(Cell const &origin) (0x00567490)
// Build_All_Shores (0x0055C2C0) around a cell (TibSun Rebuild_Shores_At): the placement
// cursor cleared; with no generator cell data, a temporary array; every record's +0x4A set
// and water mask (+0x40) set to -1; every cell offered to Expand_Water, then Prune_Water,
// Mark_Fill_Area (0x00567710) on the origin, then Place_Shore passes 1 and 2, each pass
// stopping at the first failure; the pending placement object deleted; +0x4A set again and
// the temporary array freed. Whether every pass succeeded.

bool MapClass::Rebuild_Shores_At(Cell const &origin)
{
    Map.Unknown_00498F10(0);

    bool is_temp_data = false;
    if (unknown_map_cells_a70a24 == 0) {
        unknown_map_cells_a70a24 = new UnknownMapGenerationCell[unknown_map_cell_side_84e804 * unknown_map_cell_side_84e804];
        is_temp_data = true;
    }

    int r = unknown_map_cell_side_84e804;
    int s = r * r;
    for (int i = 0; i < s; i++) {
        Get_Cell_Data_0056dbc0(i).unknown_45[5] = 1;
        Get_Cell_Data_0056dbc0(i).unknown_40 = -1;
    }

    Map.Reset_Iterator();
    CellClass *cellptr = Map.Iterate();
    bool expanded = true;
    while (cellptr != 0 && expanded) {
        expanded = Expand_Water(cellptr, 0, 0);
        cellptr = Map.Iterate();
    }

    Map.Reset_Iterator();
    cellptr = Map.Iterate();
    while (cellptr != 0 && expanded) {
        Unknown_0055C520(cellptr, 0, 0);
        cellptr = Map.Iterate();
    }

    Mark_Fill_Area(origin, true);

    Map.Reset_Iterator();
    cellptr = Map.Iterate();
    while (cellptr != 0 && expanded) {
        expanded = Place_Shore(cellptr, 1, 0, 0);
        cellptr = Map.Iterate();
    }

    Map.Reset_Iterator();
    cellptr = Map.Iterate();
    while (cellptr != 0 && expanded) {
        expanded = Place_Shore(cellptr, 2, 0, 0);
        cellptr = Map.Iterate();
    }

    ObjectClass *obj = Map.unknown_11a0;
    Map.unknown_11a4 = 0;
    if (obj != 0) {
        delete obj;
        Map.unknown_11a0 = 0;
    }

    for (int j = 0; j < s; j++) {
        Get_Cell_Data_0056dbc0(j).unknown_45[5] = 1;
    }

    if (is_temp_data) {
        delete [] unknown_map_cells_a70a24;
        unknown_map_cells_a70a24 = 0;
    }

    return expanded;
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void MapClass::Mark_Fill_Area(Cell const &origin, bool on_water) (0x00567710)
// TibSun Mark_Fill_Area: floods out from the origin over water cells (on_water) or
// clear cells of the origin's height, marking the generator records' +0x49 (in the
// fill area), then marks +0x4A (in reach) on the 3 x 3 block around every filled cell.
// From ra2-decomp MapClass::Mark_Fill_Area (YR 0x00585930), adapted to our names.

void MapClass::Mark_Fill_Area(Cell const &origin, bool on_water)
{
    DynamicVectorClass<Cell> region_cells;
    region_cells.Clear();
    region_cells.Set_Growth_Step(100);

    DynamicVectorClass<Cell> frontier_cells;
    frontier_cells.Clear();
    frontier_cells.Set_Growth_Step(1000);

    for (int i = unknown_map_cell_side_84e804 * unknown_map_cell_side_84e804 - 1; i >= 0; i--) {
        UnknownMapGenerationCell &data = Get_Cell_Data_0056dbc0(i);
        data.unknown_45[4] = 0;
        data.unknown_45[5] = 0;
    }

    frontier_cells.Add(origin);
    region_cells.Add(origin);

    if (In_Radar(origin)) {
        Get_Cell_Data_0056dba0(origin).unknown_45[4] = 1;
    }

    int height = Map[origin].unknown_11b;

    while (frontier_cells.Count()) {
        Cell cell = frontier_cells[frontier_cells.Count() - 1];
        frontier_cells.Delete(frontier_cells.Count() - 1);

        for (int dir = 0; dir < 8; dir++) {
            Cell adjacent = Adjacent_Cell(cell, dir);
            if (In_Radar(adjacent)) {
                UnknownMapGenerationCell &data = Get_Cell_Data_0056dba0(adjacent);
                CellClass *adjptr = &Map[adjacent];
                if (!data.unknown_45[4]) {
                    if (on_water && adjptr->unknown_47b7e0() || !on_water && adjptr->unknown_11b == height && adjptr->unknown_47b590()) {
                        frontier_cells.Add(adjacent);
                        bool legal = In_Radar(adjacent);
                        bool legal2 = In_Radar(cell);
                        region_cells.Add(adjacent);
                        data.unknown_45[4] = 1;
                    }
                }
            }
        }
    }

    for (int j = 0; j < region_cells.Count(); j++) {
        Cell cell = region_cells[j];
        for (int y = cell.Y - 1; y < cell.Y + 2; y++) {
            for (int x = cell.X - 1; x < cell.X + 2; x++) {
                Cell influence;
                influence.X = x;
                influence.Y = y;
                if (In_Radar(influence)) {
                    Get_Cell_Data_0056dba0(Cell(x, y)).unknown_45[5] = 1;
                }
            }
        }
    }
}


// bool MapClass::Rebuild_Cliffs_At(Cell const &cell) (0x00567AE0)
// Build_All_Cliffs (0x0055B060) around a cell (TibSun Rebuild_Cliffs_At):
// the generator records' +0x4A set (unless the array is made here), every
// cell offered to Expand_High_Ground with region 0, Mark_Fill_Area
// (0x00567710) on the cell, then Place_Cliff on the bare cells, the pending
// placement object deleted, +0x4A set again and a temporary array freed.

bool MapClass::Rebuild_Cliffs_At(Cell const &cell)
{
    int s = unknown_map_cell_side_84e804 * unknown_map_cell_side_84e804;
    bool expanded = true;

    bool is_temp_data = false;
    if (unknown_map_cells_a70a24 == 0) {
        unknown_map_cells_a70a24 = new UnknownMapGenerationCell[s];
        is_temp_data = true;
    } else {
        for (int i = 0; i < s; i++) {
            Get_Cell_Data_0056dbc0(i).unknown_45[5] = 1;
        }
    }

    Map.Unknown_00498F10(0);

    Map.Reset_Iterator();
    CellClass *cellptr = Map.Iterate();
    while (cellptr != 0 && expanded) {
        expanded = Expand_High_Ground(cellptr, 0);
        cellptr = Map.Iterate();
    }

    Mark_Fill_Area(cell, false);

    Map.Reset_Iterator();
    cellptr = Map.Iterate();
    while (cellptr != 0 && expanded) {
        if (cellptr->unknown_11c == 0 && cellptr->unknown_47b590()) {
            expanded = Place_Cliff(cellptr, 0);
        }
        cellptr = Map.Iterate();
    }

    ObjectClass *obj = Map.unknown_11a0;
    Map.unknown_11a4 = 0;
    if (obj != 0) {
        delete obj;
        Map.unknown_11a0 = 0;
    }

    for (int i = 0; i < s; i++) {
        Get_Cell_Data_0056dbc0(i).unknown_45[5] = 1;
    }

    if (is_temp_data) {
        delete [] unknown_map_cells_a70a24;
        unknown_map_cells_a70a24 = 0;
    }

    return expanded;
}


// Cell unknown_cells_a6f218[4] (0x00A6F218) and its dynamic initializer
// (0x00567CC0, the unit's last .CRT$XCU entry): the corner offsets 0 and 4
// of a 4 x 4 block (read by 0x00567D20).

Cell unknown_cells_a6f218[4] = { Cell(0, 0), Cell(4, 0), Cell(0, 4), Cell(4, 4) };


// int MapClass::Region_Threat(HouseClass *house, int level, int from_subzone, int to_subzone) (0x00567D20)
// TibSun Region_Threat: the threat between two subzones' threat regions.

extern int unknown_from_quadrants_7e0494[8][2];
extern int unknown_to_quadrants_7e04d4[8][2];

int MapClass::Region_Threat(HouseClass *house, int level, int from_subzone, int to_subzone)
{
    switch (level) {
        default:
            break;

        case 0:
            return 0;

        case 1:
            return house->unknown_56d0[Map.SubzoneTracking[level][to_subzone].unknown_20];

        case 2: {
            int from_region = Map.SubzoneTracking[level][from_subzone].unknown_20;
            int index = from_region - 1;
            int col = index % 130;
            int row = (from_region - col) / 130;
            int from_x = 4 * col;
            int from_y = 4 * (row - 1);

            short from_block_x = from_x - (from_x / 4 % 2 != 0);
            short from_block_y = from_y - (from_y / 4 % 2 != 0);

            int to_region = Map.SubzoneTracking[level][to_subzone].unknown_20;
            index = to_region - 1;
            col = index % 130;
            row = (to_region - col) / 130;
            int to_x = 4 * col;
            int to_y = 4 * (row - 1);

            short to_block_x = to_x - (to_x / 4 % 2 != 0);
            short to_block_y = to_y - (to_y / 4 % 2 != 0);

            if (from_block_x == to_block_x && from_block_y == to_block_y) {
                return (house->unknown_56d0[Unknown_0054E370(XY_Cell(from_x, from_y))] + house->unknown_56d0[Unknown_0054E370(XY_Cell(to_x, to_y))]) >> 1;
            }

            int facing;
            if (to_block_x > from_block_x) {
                if (to_block_y > from_block_y) {
                    facing = 3;
                } else if (to_block_y >= from_block_y) {
                    facing = 2;
                } else {
                    facing = 1;
                }
            } else if (to_block_x < from_block_x) {
                if (to_block_y > from_block_y) {
                    facing = 5;
                } else if (to_block_y >= from_block_y) {
                    facing = 6;
                } else {
                    facing = 7;
                }
            } else if (to_block_y > from_block_y) {
                facing = 4;
            } else {
                facing = 0;
            }

            int from_threat1 = house->unknown_56d0[Unknown_0054E370(XY_Cell(from_block_x, from_block_y) + unknown_cells_a6f218[unknown_from_quadrants_7e0494[facing][0]])];
            int from_threat2 = house->unknown_56d0[Unknown_0054E370(unknown_cells_a6f218[unknown_from_quadrants_7e0494[facing][1]] + XY_Cell(from_block_x, from_block_y))];
            int from_threat = from_threat1;
            if (from_threat1 >= from_threat2) {
                from_threat = from_threat2;
            }

            int to_threat1 = house->unknown_56d0[Unknown_0054E370(unknown_cells_a6f218[unknown_to_quadrants_7e04d4[facing][0]] + XY_Cell(to_block_x, to_block_y))];
            int to_threat2 = house->unknown_56d0[Unknown_0054E370(unknown_cells_a6f218[unknown_to_quadrants_7e04d4[facing][1]] + XY_Cell(to_block_x, to_block_y))];
            if (to_threat1 >= to_threat2) {
                to_threat1 = to_threat2;
            }

            return (to_threat1 + from_threat) >> 1;
        }
    }
    return 0;
}


// bool MapClass::Is_Something_Nearby(Cell const &cell, int radius) (0x005680A0)
// Whether any cell of the square of the given radius around a cell (of the
// global Map) has an occupier (+0xE4).

bool MapClass::Is_Something_Nearby(Cell const &cell, int radius)
{
    for (int y = cell.Y - radius; y <= cell.Y + radius; y++) {
        for (int x = cell.X - radius; x <= cell.X + radius; x++) {
            if (Map[Cell(x, y)].OccupierPtr != 0) {
                return true;
            }
        }
    }
    return false;
}


// bool MapClass::Is_Shrouded(CoordStruct const &coord) (0x00568140)
// Whether a lepton position is shrouded (TibSun Is_Shrouded): the cell its
// height level projects onto (half a cell per level up and left) is not
// mapped (+0x12C bit 3); on an odd level both that cell and its facing-3
// neighbour must be unmapped.

bool MapClass::Is_Shrouded(CoordStruct const &coord)
{
    int level = coord.Z / unknown_geometry_g;
    if ((level & 1) != 0) {
        int offset = level / 2 + 1;
        Cell cell = Cell(coord.X / 256, coord.Y / 256);
        CellClass *cellptr = &Map[Cell(cell.X - offset, cell.Y - offset)];
        if (cellptr->IsMapped) {
            return false;
        }
        cellptr = &cellptr->Adjacent_Cell(3);
        if (!cellptr->IsMapped) {
            return true;
        }
    } else {
        int offset = level / 2;
        Cell cell = Cell(coord.X / 256, coord.Y / 256);
        CellClass *cellptr = &Map[Cell(cell.X - offset, cell.Y - offset)];
        if (!cellptr->IsMapped) {
            return true;
        }
    }
    return false;
}


// bool MapClass::Is_Under_Fog(CoordStruct const &coord) (0x00568140)
// Whether a lepton position is shrouded (TibSun Is_Shrouded): the cell its
// height level projects onto (half a cell per level up and left) is not
// mapped (+0x12C bit 3); on an odd level both that cell and its facing-3
// neighbour must be unmapped.

bool MapClass::Is_Under_Fog(CoordStruct const &coord)
{
    int level = coord.Z / unknown_geometry_g;
    if ((level & 1) != 0) {
        int offset = level / 2 + 1;
        Cell cell = Cell(coord.X / 256, coord.Y / 256);
        CellClass *cellptr = &Map[Cell(cell.X - offset, cell.Y - offset)];
        if (cellptr->unknown_13c <= 0) {
            return false;
        }
        if (cellptr->Adjacent_Cell(3).unknown_13c > 0) {
            return true;
        }
    } else {
        int offset = level / 2;
        Cell cell = Cell(coord.X / 256, coord.Y / 256);
        CellClass *cellptr = &Map[Cell(cell.X - offset, cell.Y - offset)];
        if (cellptr->unknown_13c > 0) {
            return true;
        }
    }
    return false;
}


// bool MapClass::Is_Fogged(CoordStruct const &coord) (0x005683C0)
// Whether a lepton position is fogged (TibSun Is_Fogged): never for a
// defeated player watching a multiplayer game (game mode not 0, player
// +0x13D and 0x00A3D548 set); otherwise the projected cell (and on an odd
// level its facing-3 neighbour) lacks +0x140 bit 1.

bool MapClass::Is_Fogged(CoordStruct const &coord)
{
    CellClass *cellptr;
    if (unknown_game_mode_a3d298 == 0 || !unknown_player_a35db4->unknown_13d || !unknown_int_a3d548) {
        int level = coord.Z / unknown_geometry_g;
        if ((level & 1) != 0) {
            int offset = level / 2 + 1;
            Cell cell = Cell(coord.X / 256, coord.Y / 256);
            cellptr = &Map[Cell(cell.X - offset, cell.Y - offset)];
            if (cellptr->IsFogMapped) {
                return false;
            }
            cellptr = &cellptr->Adjacent_Cell(3);
        } else {
            int offset = level / 2;
            Cell cell = Cell(coord.X / 256, coord.Y / 256);
            cellptr = &Map[Cell(cell.X - offset, cell.Y - offset)];
        }
        if (!cellptr->IsFogMapped) {
            return true;
        }
    }
    return false;
}


// void MapClass::Unknown_00568520(CellClass *cell, HouseClass *house, bool arg3) (0x00568520)
// Reveals the technos standing on the cells a shroud change uncovers (TibSun
// Reveal_Nearby_Technos): along the diagonal from the cell, the cell of each
// height band (heights i - 2 to i for i = 1, 3, ... 13) is marked for a radar
// redraw when arg3 is set, and its techno (CellClass 0x00471660) gets slot 96.

void MapClass::Unknown_00568520(CellClass *cell, HouseClass *house, bool arg3)
{
    Cell where = cell->MapCoords;
    for (int level = 1; level < 15; level += 2) {
        CellClass *cellptr = &Map[where];
        int height = cellptr->unknown_11b;
        if (height >= level - 2 && height <= level) {
            if (arg3) {
                Map.Radar_Cell(cellptr->MapCoords);
            }
            TechnoClass *techno = (TechnoClass *)cellptr->Unknown_00471660(Point2D(0, 0), 0, 0);
            if (techno != 0) {
                techno->Revealed(house);
            }
        }
        where.X++;
        where.Y++;
    }
}


// void MapClass::Unknown_005685F0() (0x005685F0)
// Runs CellClass 0x0047BC80 on every cell whose +0x140 bit 1 is clear.

void MapClass::Unknown_005685F0()
{
    Reset_Iterator();
    for (CellClass *cell = Iterate(); cell != 0; cell = Iterate()) {
        if (!cell->IsFogMapped) {
            cell->Fog_Cell();
        }
    }
}


// void MapClass::Unknown_00568650() (0x00568650)
// Runs CellClass 0x0047BE00 on every cell.

void MapClass::Unknown_00568650()
{
    Reset_Iterator();
    for (CellClass *cell = Iterate(); cell != 0; cell = Iterate()) {
        cell->unknown_47be00();
    }
}


// bool MapClass::Unknown_005686B0(Rect const &rect, int house) (0x005686B0)
// Whether a cell rectangle (of the global Map) is free for a house: no cell
// has terrain, the house's bit in +0xDC, an overlay, +0x4C or +0x11C set or
// a building, and its corners are inside the local map (0x0055A590).

bool MapClass::Unknown_005686B0(Rect const &rect, int house)
{
    int bit = 1 << house;
    for (int x = rect.X; x < rect.X + rect.Width; x++) {
        for (int y = rect.Y; y < rect.Y + rect.Height; y++) {
            CellClass &cell = Map[Cell(x, y)];
            if (cell.Cell_Terrain() != 0) {
                return false;
            }
            if (bit & cell.unknown_dc) {
                return false;
            }
            if (cell.OverlayIndex != -1) {
                return false;
            }
            if (cell.unknown_4c != 0) {
                return false;
            }
            if (cell.unknown_11c) {
                return false;
            }
            if (cell.Cell_Building() != 0) {
                return false;
            }
        }
    }
    return Unknown_0055A590(rect, true);
}


// void MapClass::Unknown_005687B0(Rect const &rect) (0x005687B0)
// Collects every cell of a cell rectangle (X outer, Y inner) into a local
// vector and hands it to 0x005688A0.

void MapClass::Unknown_005687B0(Rect const &rect)
{
    DynamicVectorClass<Cell> cells;
    for (int x = rect.X; x < rect.X + rect.Width; x++) {
        for (int y = rect.Y; y < rect.Y + rect.Height; y++) {
            cells.Add(Cell(x, y));
        }
    }
    Unknown_005688A0(cells);
}


// void MapClass::Unknown_005688A0(DynamicVectorClass<Cell> &cells) (0x005688A0)
// Recalculates a list of cells (TibSun Recalc_Cells_In_List): every listed
// cell in the local view loses its first subzone (the short at +0x6C's
// 10-byte entry) and recalculates (CellClass 0x00472540 with -1); then each
// such cell still without a subzone gets its subzones rebuilt (0x00566340).

void MapClass::Unknown_005688A0(DynamicVectorClass<Cell> &cells)
{
    int index;
    for (index = cells.Count() - 1; index >= 0; index--) {
        if (Unknown_0055A660(cells[index], true)) {
            *(short *)(CellSubzones + Get_Cell_Zone_Index(cells[index]) * 10) = 0;
            CellClass *cellptr = &Map[cells[index]];
            cellptr->Recalc_Attributes(-1);
        }
    }
    for (index = cells.Count() - 1; index >= 0; index--) {
        Cell cell = cells[index];
        if (Unknown_0055A660(cell, true)) {
            if (*(short *)(CellSubzones + Get_Cell_Zone_Index(cell) * 10) == 0) {
                Unknown_00566340(cell);
            }
        }
    }
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void MapClass::Set_WasUnderBridge_Flags() (0x00568B00), from ra2-decomp MapClass::Set_WasUnderBridge_Flags
// (YR 0x00586BF0, src/map.cpp; layout twin): every impassable bridge connection (type 0) walks from
// its From cell to its To cell; each spanned cell that is not itself under the bridge marks a four-cell
// strip across it as having been under a bridge, with the strip's direction in IsBridgeEastWest (set
// for a bridge running east/west). The twin's file-local Bridge_Start_Cell helper takes the second
// branch's first step (one more inline level than the first branch's direct Adjacent_Cell).

static inline Cell Bridge_Start_Cell(Cell const &cell, FacingType facing)
{
    return Adjacent_Cell(cell, facing);
}

void MapClass::Set_WasUnderBridge_Flags()
{
    for (int index = ZoneConnections.Count() - 1; index >= 0; index--) {
        ZoneConnectionClass *con = &ZoneConnections[index];

        if (con->unknown_0c == 0 && !con->unknown_08) {
            if (con->From.X == con->To.X) {
                FacingType facing = con->From.Y < con->To.Y ? FACING_S : FACING_N;
                Cell cell = Adjacent_Cell(con->From, facing);
                for (; cell != con->To; cell = cell + AdjacentCell[(int)facing & 7]) {
                    if (!Map[cell].IsUnderBridge) {
                        for (int offset = -2; offset <= 1; offset++) {
                            CellClass &cptr = Map[cell + Cell(offset, 0)];
                            cptr.IsBridgeEastWest = false;
                            cptr.WasUnderBridge = true;
                        }
                    }
                }
            } else {
                FacingType facing = con->From.X < con->To.X ? FACING_E : FACING_W;
                Cell cell = Bridge_Start_Cell(con->From, facing);
                for (; cell != con->To; cell = AdjacentCell[(int)facing & 7] + cell) {
                    if (!Map[cell].IsUnderBridge) {
                        for (int offset = -2; offset <= 1; offset++) {
                            CellClass &cptr = Map[cell + Cell(0, offset)];
                            cptr.IsBridgeEastWest = true;
                            cptr.WasUnderBridge = true;
                        }
                    }
                }
            }
        }
    }
}


// Not exact (RA2_NON_MATCHING): MapClass::Closest_Edge_Cell at 0x005689D0, best score 58.18, 2026-11-13; differs: 109 instructions in the original, 111 in the draft (shape 97.27)
#ifdef RA2_NON_MATCHING
// Cell MapClass::Closest_Edge_Cell(Cell const &cell, bool inset) (0x005689D0)
// The cell on the local view's nearest edge to a cell (one cell inside it
// when inset; TibSun Closest_Edge_Cell): the cell's local view point is
// moved to the edge on the longer axis from the view's centre.
extern "C" int __cdecl abs(int value);

Cell MapClass::Unknown_005689D0(Cell const &cell, bool inset)
{
    Point2D point = Unknown_00547DB0(Point2D(cell.X, cell.Y));
    point = point - MapLocalSize.Top_Left();

    Point2D center(MapLocalSize.Width / 2, MapLocalSize.Height / 2);
    int x = point.X - center.X;
    int y = point.Y - center.Y;

    if (abs(x) < abs(y)) {
        if (point.X < center.X) {
            point.X = inset ? 1 : 0;
        } else {
            point.X = inset ? MapLocalSize.Width - 1 : MapLocalSize.Width;
        }
    } else {
        if (point.Y < center.Y) {
            point.Y = inset ? 1 : 0;
        } else {
            point.Y = inset ? MapLocalSize.Height - 1 : MapLocalSize.Height;
        }
    }

    point = Unknown_00547D70(point);
    return(Cell(point.X, point.Y));
}
#endif

// Cell MapClass::Clip_To_Map(Cell const &cell) (0x00568D60)
// A cell moved inside the local view (TibSun Clip_To_Map): first onto the
// view's diagonal band, then along the diagonal until 0x0055A660 accepts it
// (the cell's +0x11B height, one more on a slope +0x11C, offsets the test).

Cell MapClass::Clip_To_Map(Cell const &cell)
{
    int x = cell.X;
    int y = cell.Y;

    if (x - y >= 2 * (MapLocalSize.X + MapLocalSize.Width) - MapSize.Width) {
        int delta = x + MapSize.Width + 2 * (1 - MapLocalSize.X - MapLocalSize.Width) - y;
        x -= delta / 2;
        y += delta / 2;
    } else if (y - x >= MapSize.Width - 2 * MapLocalSize.X) {
        int delta = y + 2 * MapLocalSize.X + 2 - MapSize.Width - x;
        y -= delta / 2;
        x += delta / 2;
    }

    CellClass *cellptr = &Map[Cell(x, y)];
    int height = cellptr->unknown_11b;
    int slope = cellptr->unknown_11c;
    if (slope != 0) {
        if (x + y < height + MapSize.Width + 2 * MapLocalSize.Y + 4) {
            height++;
        }
    }

    if (x + y <= height + MapSize.Width + 2 * MapLocalSize.Y) {
        do {
            x++;
            y++;
        } while (!Unknown_0055A660(Cell(x, y), true));
    } else if (x + y > height + MapSize.Width + 2 * (MapLocalSize.Y + MapLocalSize.Height) + 2) {
        do {
            x--;
            y--;
        } while (!Unknown_0055A660(Cell(x, y), true));
    }
    return Cell(x, y);
}


// Cell MapClass::Unknown_00568ED0(Cell const &cell, Cell const &from) (0x00568ED0)
// The cell of the isometric tile under `cell` that is closest to `from`
// (TibSun Closest_Passable_Cell): every cell of the tile's grid that has an
// image and lies in the local view (0x0055A660) is measured with Fast_Sqrt;
// the cell itself when it has no tile type.

Cell MapClass::Unknown_00568ED0(Cell const &cell, Cell const &from)
{
    Cell best_cell = unknown_cell_a6f238;
    double best_dist = 10000.0;
    CellClass *cptr = &Map[cell];

    if (cptr->IsoTileIndex < 0) {
        return cell;
    }

    IsometricTileTypeClass *iptr = IsoTileTypes[cptr->IsoTileIndex];
    IsoTileImageStruct *image = (IsoTileImageStruct *)iptr->Get_Image_Data();

    int w = iptr->unknown_2b4;
    int h = iptr->unknown_2b8;
    int s = (unsigned char)cptr->unknown_11a;

    Cell d = cell - Cell(s % w, s / w);

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            Cell here = d + Cell(x, y);
            if (image->Tiles[x + y * iptr->unknown_2b4] != 0 && Unknown_0055A660(here, true)) {
                int xdiff = here.X - from.X;
                int ydiff = here.Y - from.Y;
                double dist = Fast_Sqrt(double(xdiff * xdiff + ydiff * ydiff));
                if (dist < best_dist) {
                    best_dist = dist;
                    best_cell = here;
                }
            }
        }
    }

    return best_cell;
}


// bool MapClass::Unknown_00569090(Cell *cell) (0x00569090; TibSun Damage_Bridge, a rename candidate)
// Damages the bridge at a cell (TibSun Damage_Bridge): a low bridge overlay (0x4A-0x63,
// 0x0055DCA0; 0xCD-0xE6, 0x0055EDB0); a high bridge (deck overlay 0x18/0x19 or a bridge
// middle tile, 0x00559000) or a train bridge (deck overlay 0xED/0xEE or a train bridge
// middle tile, 0x005539B0). The cells left pending (+0xD0) collapse (0x0047C930).

bool MapClass::Unknown_00569090(Cell *cell)
{
    bool result = false;
    unknown_d0.Clear();
    CellClass *cptr = &Map[*cell];

    if (cptr->OverlayIndex >= 0x4A && cptr->OverlayIndex <= 0x63) {
        result = Unknown_0055DCA0(*cell);
    } else if (cptr->OverlayIndex >= 0xCD && cptr->OverlayIndex <= 0xE6) {
        result = Unknown_0055EDB0(*cell);
    } else {
        int ittype = cptr->IsoTileIndex - unknown_tileset_a52c74 + 1;
        CellClass *bcptr = 0;
        if (cptr->IsUnderBridge) {
            if (cptr->IsBridgeDeck) {
                bcptr = &Map[cptr->Fetch_CellID()];
            } else {
                bcptr = &Map[cptr->unknown_2c->Fetch_CellID()];
            }
        }

        if ((bcptr != 0 && (bcptr->OverlayIndex == 0x18 || bcptr->OverlayIndex == 0x19)) ||
            ittype == unknown_tileset_a6cb7c + 0 ||
            ittype == unknown_tileset_a6cb7c + 1 ||
            ittype == unknown_tileset_a6cb7c + 2 ||
            ittype == unknown_tileset_a6cb7c + 3 ||
            ittype == unknown_tileset_a52e70 + 0 ||
            ittype == unknown_tileset_a52e70 + 1 ||
            ittype == unknown_tileset_a52e70 + 2 ||
            ittype == unknown_tileset_a52e70 + 3) {
            result = Unknown_00559000(*cell);
        } else {
            ittype = cptr->IsoTileIndex - unknown_tileset_a6cb68 + 1;
            if ((bcptr != 0 && (bcptr->OverlayIndex == 0xED || bcptr->OverlayIndex == 0xEE)) ||
                ittype == unknown_tileset_a6cb7c + 0 ||
                ittype == unknown_tileset_a6cb7c + 1 ||
                ittype == unknown_tileset_a6cb7c + 2 ||
                ittype == unknown_tileset_a6cb7c + 3 ||
                ittype == unknown_tileset_a52e70 + 0 ||
                ittype == unknown_tileset_a52e70 + 1 ||
                ittype == unknown_tileset_a52e70 + 2 ||
                ittype == unknown_tileset_a52e70 + 3) {
                result = Unknown_005539B0(*cell);
            }
        }
    }

    for (int i = 0; i < unknown_d0.Count(); i++) {
        Map[unknown_d0[i]].On_Bridge_Collapse();
    }

    unknown_d0.Clear();
    return result;
}


// Not exact (RA2_NON_MATCHING): MapClass::Unknown_00569320 at 0x00569320, best score 99.49, 2026-11-13; differs: same instructions; registers, stack slots or operands differ
#ifdef RA2_NON_MATCHING
// MapClass::Unknown_00569320: whether a bridge repair hut's bridge is intact (hypothesis).
extern short unknown_bridge_entry_x_7e0514[];
extern short unknown_bridge_entry_y_7e0534[];
extern int unknown_bridge_facings_7e0554[];


static inline bool Is_Hut1(int overlay)
{
    return overlay == 0x64 || overlay == 0x65;
}

static inline bool Is_Hut2(int overlay)
{
    return overlay == 0xE7 || overlay == 0xE8;
}

static inline bool Is_Span_EW(int overlay, int base)
{
    return (overlay >= base && overlay <= base + 8) || (overlay >= base + 0x12 && overlay <= base + 0x15) || overlay == base + 0x1A;
}

bool MapClass::Unknown_00569320(Cell const &cell)
{
    bool found = false;
    bool second_low_bridge = false;
    CellClass *cellptr = 0;
    int offset = -1;
    for (int y = -2; y < 3; ++y) {
        for (int x = -2; x < 3; ++x) {
            int tile = Map[Cell(Cell(x, y) + cell)].IsoTileIndex;
            if (tile >= unknown_tileset_a6cb68 && tile < unknown_tileset_a6cb68 + 16) {
                found = true;
                cellptr = &Map[Cell(Cell(x, y) + cell)];
                offset = tile - unknown_tileset_a6cb68;
            } else if (tile >= unknown_tileset_a52c74 && tile < unknown_tileset_a52c74 + 16) {
                found = true;
                cellptr = &Map[Cell(Cell(x, y) + cell)];
                offset = tile - unknown_tileset_a52c74;
            } else {
                if (Unknown_00562930(Cell(x, y) + cell)) {
                    found = false;
                    second_low_bridge = false;
                    cellptr = &Map[Cell(Cell(x, y) + cell)];
                } else if (Unknown_00562980(Cell(x, y) + cell)) {
                    found = false;
                    second_low_bridge = true;
                    cellptr = &Map[Cell(Cell(x, y) + cell)];
                }
            }
        }
    }
    switch (found) {
    case false:
        if (cellptr != 0) {
        if (second_low_bridge) {
            int overlay = cellptr->OverlayIndex;
            if (Is_Span_EW(overlay, 0xCD)) {
                for (Cell walk2 = cellptr->MapCoords; ; walk2 -= Cell(1, 0)) {
                    if (!Unknown_00562980(walk2)) break;
                    int overlay = Map[walk2].OverlayIndex;
                    if (overlay == 0xE7 || overlay == 0xE8) return true;
                }
                for (Cell scan2 = cellptr->MapCoords; ; scan2.X++) {
                    if (!Unknown_00562980(scan2)) break;
                    int overlay = Map[scan2].OverlayIndex;
                    if (overlay == 0xE7 || overlay == 0xE8) return true;
                }
            } else {
                for (Cell scan = cellptr->MapCoords; ; scan.Y++) {
                    if (!Unknown_00562980(scan)) break;
                    int overlay = Map[scan].OverlayIndex;
                    if (overlay == 0xE7 || overlay == 0xE8) return true;
                }
                for (Cell walk = cellptr->MapCoords; ; walk -= Cell(0, 1)) {
                    if (!Unknown_00562980(walk)) break;
                    int overlay = Map[walk].OverlayIndex;
                    if (overlay == 0xE7 || overlay == 0xE8) return true;
                }
            }
        } else {
            int overlay = cellptr->OverlayIndex;
            if (Is_Span_EW(overlay, 0x4A)) {
                for (Cell walk2 = cellptr->MapCoords; ; walk2 -= Cell(1, 0)) {
                    if (!Unknown_00562930(walk2)) break;
                    int overlay = Map[walk2].OverlayIndex;
                    if (overlay == 0x64 || overlay == 0x65) return true;
                }
                for (Cell scan2 = cellptr->MapCoords; ; scan2.X++) {
                    if (!Unknown_00562930(scan2)) break;
                    int overlay = Map[scan2].OverlayIndex;
                    if (overlay == 0x64 || overlay == 0x65) return true;
                }
            } else {
                for (Cell scan = cellptr->MapCoords; ; scan.Y++) {
                    if (!Unknown_00562930(scan)) break;
                    int overlay = Map[scan].OverlayIndex;
                    if (overlay == 0x64 || overlay == 0x65) return true;
                }
                for (Cell walk = cellptr->MapCoords; ; walk -= Cell(0, 1)) {
                    if (!Unknown_00562930(walk)) break;
                    int overlay = Map[walk].OverlayIndex;
                    if (overlay == 0x64 || overlay == 0x65) return true;
                }
            }
        }
        }
        break;
    default:
        IsometricTileTypeClass *type = IsoTileTypes[cellptr->IsoTileIndex];
        int subtile = (unsigned char)cellptr->unknown_11a;
        Cell topleft = cellptr->MapCoords - Cell(subtile % type->unknown_2b4, subtile / type->unknown_2b4);
        Cell search = topleft + Cell(unknown_bridge_entry_x_7e0514[offset], unknown_bridge_entry_y_7e0534[offset]);
        int dir = unknown_bridge_facings_7e0554[offset];
        if (dir != -1) {
            Cell entry = search;
            while (true) {
                int index = Zone_Connection_Index(search, 3, 0);
                if (index < 0) break;
                if (!ZoneConnections[index].unknown_08) return true;
                ZoneConnectionClass *con = &ZoneConnections[index];
Cell next;
if (con->From == entry) {
entry = con->To;
                    next = Adjacent_Cell(entry, dir);
} else {
 if (con->To != entry) return true;
entry = con->From;
                    next = Adjacent_Cell(entry, dir);
}
search = next;

            }
        }
        break;
    }
    return false;
}
#endif

// bool MapClass::Check_Map() (0x00569B60)
// A debug consistency check of the zone tables (TibSun MapClass::Check_Map): for every
// map cell and every movement zone but Subterannean (6) and Fly (9), every later cell of
// the local map that the zone tables say is reachable (0x0054F7C0) ground to ground,
// bridge to bridge, ground to bridge or bridge to ground is walked by the path estimator
// (0x0042AA30); each walk that fails is written to MapCheck.txt. Whether none failed.

// The movement zone names (0x007D1A68; as shared/ccini_extra.hpp, which the Map unit
// does not include).
extern char const *MovementZoneNames[12];

bool MapClass::Check_Map()
{
    bool from_ground;
    bool from_bridge;
    bool to_ground;
    bool to_bridge;
    CellClass *from;
    CellClass *to;
    CellClass *bcptr;

    bool passed = true;

    _iobuf *file = fopen("MapCheck.txt", "w");

    Reset_Iterator();
    from = Iterate();
    while (from != 0) {
        if (Unknown_0055A740(from, true)) {

            for (int mzone = 0; mzone < 12; mzone++) {
                if (mzone != 9 && mzone != 6) {

                    from_ground = unknown_table_7e00c4[mzone][from->unknown_4c] == 1;

                    from_bridge = from->IsUnderBridge
                        && ((from->IsBridgeEastWest && ((bcptr = from->unknown_2c) == 0 || bcptr->MapCoords.Y - from->MapCoords.Y != 2))
                            || (!from->IsBridgeEastWest && ((bcptr = from->unknown_2c) == 0 || bcptr->MapCoords.X - from->MapCoords.X != 2)));

                    if (from_ground || from_bridge) {
                        unknown_118 = MapLocalSize.X + 1 + MapLocalSize.Y;
                        unknown_11c = MapLocalSize.Y - MapLocalSize.X + MapSize.Width;
                        to = Unknown_0055A910();
                        while (to != 0) {

                            if ((to->MapCoords.X + to->MapCoords.Y > from->MapCoords.X + from->MapCoords.Y) || (to->MapCoords.X + to->MapCoords.Y == from->MapCoords.X + from->MapCoords.Y && to->MapCoords.X - to->MapCoords.Y > from->MapCoords.X - from->MapCoords.Y)) {
                                to_ground = unknown_table_7e00c4[mzone][to->unknown_4c] == 1;
                                to_bridge = to->IsUnderBridge
                                    && ((to->IsBridgeEastWest && ((bcptr = to->unknown_2c) == 0 || bcptr->MapCoords.Y - to->MapCoords.Y != 2))
                                        || (!to->IsBridgeEastWest && ((bcptr = to->unknown_2c) == 0 || bcptr->MapCoords.X - to->MapCoords.X != 2)));

                                if (to_ground
                                    && from_ground
                                    && Unknown_0054F7C0(from->MapCoords, to->MapCoords, mzone, false, false, false)
                                    && Search.Test_Cell_Walk(from->MapCoords, to->MapCoords, 0, false, false, mzone) == 0x7FFFFFFF) {
                                    passed = false;
                                    fprintf(file, "Failure: (%d,%d)->(%d,%d) ; (ground->ground) ; MZONE %s\n", from->MapCoords.X, from->MapCoords.Y, to->MapCoords.X, to->MapCoords.Y, MovementZoneNames[mzone]);
                                }

                                if (to_bridge) {
                                    if (from_bridge
                                        && Unknown_0054F7C0(from->MapCoords, to->MapCoords, mzone, true, true, false)
                                        && Search.Test_Cell_Walk(from->MapCoords, to->MapCoords, 0, true, true, mzone) == 0x7FFFFFFF) {
                                        passed = false;
                                        fprintf(file, "Failure: (%d,%d)->(%d,%d) ; (bridge->bridge) ; MZONE %s\n", from->MapCoords.X, from->MapCoords.Y, to->MapCoords.X, to->MapCoords.Y, MovementZoneNames[mzone]);
                                    }

                                    if (from_ground
                                        && Unknown_0054F7C0(from->MapCoords, to->MapCoords, mzone, false, true, false)
                                        && Search.Test_Cell_Walk(from->MapCoords, to->MapCoords, 0, false, true, mzone) == 0x7FFFFFFF) {
                                        passed = false;
                                        fprintf(file, "Failure: (%d,%d)->(%d,%d) ; (ground->bridge) ; MZONE %s\n", from->MapCoords.X, from->MapCoords.Y, to->MapCoords.X, to->MapCoords.Y, MovementZoneNames[mzone]);
                                    }
                                }

                                if (to_ground
                                    && from_bridge
                                    && Unknown_0054F7C0(from->MapCoords, to->MapCoords, mzone, true, false, false)
                                    && Search.Test_Cell_Walk(from->MapCoords, to->MapCoords, 0, true, false, mzone) == 0x7FFFFFFF) {
                                    passed = false;
                                    fprintf(file, "Failure: (%d,%d)->(%d,%d) ; (bridge->ground) ; MZONE %s\n", from->MapCoords.X, from->MapCoords.Y, to->MapCoords.X, to->MapCoords.Y, MovementZoneNames[mzone]);
                                }
                            }

                            to = Unknown_0055A910();
                        }
                    }
                }
            }
        }

        from = Iterate();
    }

    fclose(file);
    return passed;
}


// CoordStruct MapClass::Unknown_00569FB0(CoordStruct const &from, CoordStruct const &to, HouseClass *house) (0x00569FB0)
// The first hostile firestorm wall on a path (TibSun Firestorm_On_Path): along a column, a
// row or (otherwise) the cell edges a straight line crosses, the first cell whose building is a
// firestorm wall (+0x135A of its type) of a house with its firestorm up (+0x142) other than
// house; the cell's centre (the crossing point on a slanted line), else the zero coordinate.

CoordStruct MapClass::Unknown_00569FB0(CoordStruct const &from, CoordStruct const &to, HouseClass *house)
{
    if (from != to) {

        if ((short)(from.X / 256) == (short)(to.X / 256)) {
            Cell current_cell(from);
            Cell target_cell(to);
            int step = current_cell.Y < target_cell.Y ? 1 : -1;

            while (current_cell != target_cell) {
                BuildingClass *building = (BuildingClass *)Map[current_cell].Cell_Building();
                if (building != 0 && building->Type->FirestormWall) {
                    HouseClass *owner = building->House;
                    if (owner->FirestormActive && owner != house) {
                        return Cell_Coord(current_cell);
                    }
                }

                current_cell.Y += step;
            }

        } else if ((short)(from.Y / 256) == (short)(to.Y / 256)) {
            Cell current_cell(from);
            Cell target_cell(to);
            int step = current_cell.X < target_cell.X ? 1 : -1;

            while (current_cell != target_cell) {
                BuildingClass *building = (BuildingClass *)Map[current_cell].Cell_Building();
                if (building != 0 && building->Type->FirestormWall) {
                    HouseClass *owner = building->House;
                    if (owner->FirestormActive && owner != house) {
                        return Cell_Coord(current_cell);
                    }
                }

                current_cell.X += step;
            }

        } else {
            int x_step = from.X < to.X ? 256 : -256;
            int y_step = from.Y < to.Y ? 256 : -256;
            int current_x = x_step < 0 ? from.X - from.X % 256 : from.X - from.X % 256 + 256;
            int current_y = y_step < 0 ? from.Y - from.Y % 256 : from.Y - from.Y % 256 + 256;
            int delta_x = to.X - from.X;
            double x_progress = (double)(current_x - from.X) / (double)delta_x;
            int delta_y = to.Y - from.Y;
            double y_progress = (double)(current_y - from.Y) / (double)delta_y;
            double inv_delta_x = 1.0 / (double)delta_x;
            double inv_delta_y = 1.0 / (double)delta_y;

            while ((x_progress <= 1.0 || y_progress <= 1.0) && x_progress >= 0.0 && y_progress >= 0.0) {
                BuildingClass *building = (BuildingClass *)Map[CoordStruct(current_x, current_y, 0)].Cell_Building();
                if (building != 0 && building->Type->FirestormWall) {
                    HouseClass *owner = building->House;
                    if (owner->FirestormActive && owner != house) {
                        return CoordStruct(current_x, current_y, 0);
                    }
                }

                if (x_progress < y_progress) {
                    current_x += x_step;
                    x_progress = (double)(current_x - from.X) * inv_delta_x;
                } else {
                    current_y += y_step;
                    y_progress = (double)(current_y - from.Y) * inv_delta_y;
                }
            }
        }
    }

    return unknown_geometry_l;
}


// void MapClass::Unknown_0056A480(Cell const &cell, HouseClass *house, BuildingTypeClass *type) (0x0056A480)
// Joins a new firestorm wall to the house's (TibSun Place_Firestorm_Wall): looks along
// each of the four straight directions up to the type's ThreatRange cells for a
// firestorm wall building (type +0x135A) of the house, and fills the gap with new
// buildings of the type (slot 35, placed with slot 53); a cell where the type may not be
// placed (0x004718B0) ends the direction.

void MapClass::Unknown_0056A480(Cell const &cell, HouseClass *house, BuildingTypeClass *type)
{
    for (int dir = 0; dir < 8; dir += 2) {
        Cell current_cell = cell;
        int radius = type->ThreatRange >> 8;

        for (int distance = 0; distance < radius; distance++) {
            current_cell = (Cell)Adjacent_Cell(current_cell, dir);

            BuildingClass *building = (BuildingClass *)Map[current_cell].Cell_Building();
            if (building != 0 && building->Type->FirestormWall && building->House == house) {
                if (distance > 0) {
                    Cell place_cell = (Cell)Adjacent_Cell(cell, dir);
                    for (int i = 0; i < distance; i++) {
                        ObjectClass *object = type->Create_One_Of(house);
                        object->Unlimbo(place_cell, 0);
                        place_cell = (Cell)Adjacent_Cell(place_cell, dir);
                    }
                }
                break;
            }

            if (!Map[current_cell].Is_Clear_To_Build(type->Speed, type, house)) {
                break;
            }
        }
    }
}


// void MapClass::Unknown_0056A660(Cell const &cell, HouseClass *house, BuildingTypeClass *type) (0x0056A660)
// Joins a new wall to the house's walls (TibSun Place_Wall): for a type with a wall
// overlay (+0xBEC), looks along each of the four straight directions up to the type's
// ThreatRange cells for a cell with that overlay owned by the house (+0x50), and fills
// the gap with new buildings of the type (slot 35, placed with slot 53); a cell where
// the type may not be placed (0x004718B0) ends the direction. TibSun's locals: the threat
// range and the cell are read in place (Scout1010, under the TPoint3D coordinate profile).

void MapClass::Unknown_0056A660(Cell const &cell, HouseClass *house, BuildingTypeClass *type)
{
    OverlayTypeClass const *overtype = type->ToOverlay;
    if (overtype == 0) {
        return;
    }

    for (int dir = 0; dir < 8; dir += 2) {
        Cell current_cell = cell;

        int radius = type->ThreatRange >> 8;

        for (int distance = 0; distance < radius; distance++) {
            current_cell = (Cell)Adjacent_Cell(current_cell, dir);

            int overlay = Map[current_cell].OverlayIndex;
            if (overlay != -1 && overtype->unknown_264 == overlay && house->ArrayIndex == Map[current_cell].unknown_50) {
                if (distance > 0) {
                    Cell place_cell = (Cell)Adjacent_Cell_L(cell, dir);
                    for (int i = 0; i < distance; i++) {
                        ObjectClass *building = type->Create_One_Of(house);
                        building->Unlimbo(place_cell, 0);
                        place_cell = (Cell)Adjacent_Cell((Cell)place_cell, dir);
                    }
                }
                break;
            }

            if (!Map[current_cell].Is_Clear_To_Build(type->Speed, type, house)) {
                break;
            }
        }
    }
}


// void MapClass::Lower_Max_Level(int value) (0x0056A870)
// Lowers +0x144 to value when it is higher, and keeps it from going below 0.

void MapClass::Lower_Max_Level(int value)
{
    unknown_144 = unknown_144 < value ? unknown_144 : value;
    unknown_144 = unknown_144 > 0 ? unknown_144 : 0;
}


// int MapClass::Unknown_0056A8A0() (0x0056A8A0)
// While +0x144 is still 13 or more, lowers it to the +0x11B value of every
// cell (0x0056A870, expanded inline); returns +0x144.

int MapClass::Unknown_0056A8A0()
{
    if (unknown_144 >= 13) {
        int count = Cells.Length();
        for (int index = 0; index < count; index++) {
            CellClass *cell = Cells[index];
            if (cell != 0) {
                Lower_Max_Level(cell->unknown_11b);
            }
        }
    }
    return unknown_144;
}


// int __fastcall Unknown_0056A900(int row1, int row2) (0x0056A900)
// The row of the 12 x 8 table at 0x007E00C4 that best matches two of its
// rows: a row qualifies when it has no 1 where either row has a 2; the
// qualifying row with the most 1s shared by both rows wins (the first on a
// tie); -1 when none has any.

int __fastcall Unknown_0056A900(int row1, int row2)
{
    int best = -1;
    int best_count = 0;
    for (int row = 0; row < 12; row++) {
        bool ok = true;
        int count = 0;
        for (int index = 0; index < 8; index++) {
            int value1 = unknown_table_7e00c4[row1][index];
            int value2 = unknown_table_7e00c4[row2][index];
            int value = unknown_table_7e00c4[row][index];
            if ((value1 == 2 || value2 == 2) && value == 1) {
                ok = false;
            }
            if (value1 == value && value2 == value && value == 1) {
                count++;
            }
        }
        if (ok && count > best_count) {
            best_count = count;
            best = row;
        }
    }
    return best;
}


// MapClass scalar deleting destructor (0x0056A9D0, slot 4)
// Compiler generated: calls the (out-of-line) destructor, then operator
// delete when bit 0 of the flags is set.


// AbstractClass::operator=0056A9F0, member-wise copy without vptr writes.
AbstractClass &AbstractClass::operator=(AbstractClass const &other)
{
    UniqueID = other.UniqueID;
    AbstractFlags = other.AbstractFlags;
    unknown_18 = other.unknown_18;
    RefCount = other.RefCount;
    Dirty = other.Dirty;
    return *this;
}


// TPoint2D<short>::operator+=, original0056AA20; only this original member is emitted.
template <> TPoint2D<short> const &TPoint2D<short>::operator+=(TPoint2D<short> const &other)
{
    X += other.X;
    Y += other.Y;
    return *this;
}


// TPoint2D<short>::operator-(TPoint2D<short> const &) const (0x0056AA40)
// Inline member in shared/point.hpp, emitted out of line by this unit
// (Destroy_Low_Bridge_Span 0x00556BF0 and its family call it once their /Ob2
// budget runs out).
// No source of its own: emitted by this unit (the Map unit).


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<SubzoneConnectionStruct>::Clear (0x0056AA70; DynamicVectorClass<SubzoneConnectionStruct> vtable 0x007a5ad8 slot 3).
// The element type is left incomplete: Clear does not depend on it.
// DynamicVectorClass<SubzoneConnectionStruct>::ID_pointer (0x0056AAA0; DynamicVectorClass<SubzoneConnectionStruct> vtable 0x007a5ad8 slot 5).
// SubzoneConnectionStruct is a size-only stand-in (8 bytes, from the ID(T const *) divisor);
// its real fields are not recovered.
// DynamicVectorClass<ZoneConnectionClass>::Clear (0x0056AAC0; DynamicVectorClass<ZoneConnectionClass> vtable 0x007a59f8 slot 3).
// The element type is left incomplete: Clear does not depend on it.
// DynamicVectorClass<ZoneConnectionClass>::ID_pointer (0x0056AAF0; DynamicVectorClass<ZoneConnectionClass> vtable 0x007a59f8 slot 5).
// ZoneConnectionClass is a size-only stand-in (16 bytes, from the ID(T const *) divisor);
// its real fields are not recovered.
// VectorClass<ZoneConnectionClass>::Fetch (0x0056AB10).


// DynamicVectorClass<SubzoneTrackingStruct>::Clear (0x0056AB40)
// Template member instantiated from shared/vector_class.hpp.
// No source of its own: emitted by this unit (the Map unit, population-2652).


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<SubzoneTrackingStruct>::ID_pointer (0x0056ABC0; DynamicVectorClass<SubzoneTrackingStruct> vtable 0x007a59d8 slot 5).
// SubzoneTrackingStruct is a size-only stand-in (36 bytes, from the ID(T const *) divisor);
// its real fields are not recovered.


// VectorClass<SubzoneTrackingStruct>::Fetch(int) const (0x0056ABF0)
// Template member instantiated from shared/vector_class.hpp.
// No source of its own: emitted by this unit (the Map unit, population-2652).


extern "C" void __cdecl release_56ace0(void*);
class Cleanup_56ace0 { unsigned char pad0[4]; unsigned int* items; int count; unsigned char pad12[1]; unsigned char owns; unsigned char pad14[2]; int capacity; public: void get(); };
void Cleanup_56ace0::get() { unsigned int* pointer=items; capacity=0; if(pointer && owns) { release_56ace0(pointer); items=0; } owns=0; count=0; }


class Leaf_56ad30 { public: unsigned int get(unsigned int index); };
unsigned int Leaf_56ad30::get(unsigned int index) { unsigned int vector_or_index = (unsigned int)this; unsigned int buffer_or_element; unsigned int unused_value; buffer_or_element = *(unsigned int*)(vector_or_index + 4); vector_or_index = index; buffer_or_element = *(unsigned int*)(buffer_or_element + vector_or_index*4); return buffer_or_element; }


extern "C" unsigned char vtable_56ae40[]; extern "C" void __cdecl release_buffer_56ae40(void*); extern "C" void __cdecl release_object_56ae40(void*);
class Leaf_56ae40 { public: void* get(unsigned int flags); };
void* Leaf_56ae40::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_56ae40; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_56ae40((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_56ae40(this); return this; }


extern "C" unsigned char vtable_56ae90[]; extern "C" void __cdecl release_buffer_56ae90(void*); extern "C" void __cdecl release_object_56ae90(void*);
class Leaf_56ae90 { public: void* get(unsigned int flags); };
void* Leaf_56ae90::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_56ae90; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_56ae90((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_56ae90(this); return this; }


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<CellClass *>::Fetch (0x0056ACD0; VectorClass<CellClass *> vtable 0x007a59b8 slot 6).
// DynamicVectorClass<HashObject<unsigned int, unsigned int>>::Clear (0x0056AD40; DynamicVectorClass<HashObject<unsigned int, unsigned int>> vtable 0x007a5a78 slot 3).
// The element type is left incomplete: Clear does not depend on it.
// DynamicVectorClass<HashObject<unsigned int, unsigned int>>::ID_pointer (0x0056AD70; DynamicVectorClass<HashObject<unsigned int, unsigned int>> vtable 0x007a5a78 slot 5).
// HashObject<unsigned int, unsigned int> is a size-only stand-in (8 bytes, from the ID(T const *) divisor);
// its real fields are not recovered.
// VectorClass<HashObject<unsigned int, unsigned int>>::Fetch (0x0056AD90).
// VectorClass<HashObject<unsigned int, SubzoneConnectionStruct>>::Fetch (0x0056AE10).
// DynamicVectorClass<SubzoneConnectionStruct>::Resize (0x0056AF70).
// DynamicVectorClass<SubzoneConnectionStruct>::Delete(int) (0x0056B070).
// Layout and behaviour: shared/vector_class.hpp, element shared/map_elements.hpp. The Map unit's
// out-of-line copy (a subzone's connection list; 0x00566DA7 and two more call it after ID).
// DynamicVectorClass<SubzoneConnectionStruct>::ID_object (0x0056B0B0).
// VectorClass<ZoneConnectionClass>::operator== (0x0056B0E0).
// VectorClass<ZoneConnectionClass>::Resize (0x0056B150).
// VectorClass<ZoneConnectionClass>::Clear (0x0056B280; VectorClass<ZoneConnectionClass> vtable 0x007a5a18 slot 3).
// The element type is left incomplete: Clear does not depend on it.
// VectorClass<ZoneConnectionClass>::ID_object (0x0056B2B0; virtual slot 4).
// Layout and behaviour: shared/vector_class.hpp; element comparison: shared/map_elements.hpp.
// VectorClass<ZoneConnectionClass>::ID_pointer (0x0056B310; VectorClass<ZoneConnectionClass> vtable 0x007a5a18 slot 5).
// ZoneConnectionClass is a size-only stand-in (16 bytes, from the ID(T const *) divisor);
// its real fields are not recovered.
// DynamicVectorClass<ZoneConnectionClass>::ID_object (0x0056B480; virtual slot 4).
// Layout and behaviour: shared/vector_class.hpp; element comparison: shared/map_elements.hpp.
// VectorClass<SubzoneTrackingStruct>::operator== (0x0056B4D0).


// VectorClass<SubzoneTrackingStruct>::Resize (0x0056B520)
// Template member instantiated from shared/vector_class.hpp.
// No source of its own: emitted by this unit (the Map unit, population-2652).


// VectorClass<SubzoneTrackingStruct>::Clear (0x0056B740)
// Template member instantiated from shared/vector_class.hpp.
// No source of its own: emitted by this unit (the Map unit, population-2652).


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<SubzoneTrackingStruct>::ID_object (0x0056B7C0; virtual slot 4).
// Layout and behaviour: shared/vector_class.hpp; element comparison: shared/subzone_tracking.hpp.
// VectorClass<SubzoneTrackingStruct>::ID_pointer (0x0056B810; VectorClass<SubzoneTrackingStruct> vtable 0x007a5a38 slot 5).
// SubzoneTrackingStruct is a size-only stand-in (36 bytes, from the ID(T const *) divisor);
// its real fields are not recovered.


// DynamicVectorClass<SubzoneTrackingStruct>::Resize (0x0056B840)
// Template member instantiated from shared/vector_class.hpp.
// No source of its own: emitted by this unit (the Map unit, population-2652).


// HashTableClass<unsigned int, SubzoneConnectionStruct>::Add_Object0056BC00.
template <> bool HashTableClass<unsigned int, SubzoneConnectionStruct>::Add_Object(int bucket_index, HashObject<unsigned int, SubzoneConnectionStruct> const &object)
{
    DynamicVectorClass<HashObject<unsigned int, SubzoneConnectionStruct> > &bucket = Buckets[bucket_index];
    HashObject<unsigned int, SubzoneConnectionStruct> *item = &bucket[0];
    for (int remaining = bucket.Count() - 1; remaining >= 0; --remaining) {
        if (object.Key == item->Key) return false;
        ++item;
    }
    bucket.Add(object);
    return true;
}


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<Cell>::Add (0x0056BA30): emitted by this unit's use of its list.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<SubzoneTrackingStruct>::ID_object (0x0056B9F0; virtual slot 4).
// Layout and behaviour: shared/vector_class.hpp; element comparison: shared/subzone_tracking.hpp.
// VectorClass<CellClass *>::operator equal (0x0056BA80; VectorClass<CellClass *> vtable 0x007a59b8 slot 1).
// VectorClass<CellClass *>::Resize (0x0056BAC0; VectorClass<CellClass *> vtable 0x007a59b8 slot 2).
// VectorClass<CellClass *>::Clear (0x0056BB70; VectorClass<CellClass *> vtable 0x007a59b8 slot 3).
// VectorClass<CellClass *>::ID_object (0x0056BBA0; VectorClass<CellClass *> vtable 0x007a59b8 slot 4).
// VectorClass<CellClass *>::ID_pointer (0x0056BBE0; VectorClass<CellClass *> vtable 0x007a59b8 slot 5).
// VectorClass<HashObject<unsigned int, unsigned int>>::operator== (0x0056BEF0).
// DynamicVectorClass<HashObject<unsigned int, unsigned int>>::Resize (0x0056BF30).
// DynamicVectorClass<HashObject<unsigned int, unsigned int>>::ID_object (0x0056C010).
// VectorClass<HashObject<unsigned int, SubzoneConnectionStruct>>::operator== (0x0056C040).
// DynamicVectorClass<HashObject<unsigned int, SubzoneConnectionStruct>>::Resize (0x0056C080).
// DynamicVectorClass<HashObject<unsigned int, SubzoneConnectionStruct>>::ID_object (0x0056C180).


extern "C" unsigned char vtable_56c1b0[]; extern "C" void __cdecl release_buffer_56c1b0(void*); extern "C" void __cdecl release_object_56c1b0(void*);
class Leaf_56c1b0 { public: void* get(unsigned int flags); };
void* Leaf_56c1b0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_56c1b0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_56c1b0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_56c1b0(this); return this; }


extern "C" unsigned char vtable_56c200[]; extern "C" void __cdecl release_buffer_56c200(void*); extern "C" void __cdecl release_object_56c200(void*);
class Leaf_56c200 { public: void* get(unsigned int flags); };
void* Leaf_56c200::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_56c200; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_56c200((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_56c200(this); return this; }


// VectorClass<SubzoneTrackingStruct> scalar deleting destructor (??_G) (0x0056C250)
// Compiler generated with the template vtable.
// No source of its own: emitted by this unit (the Map unit, population-2652).


// SubzoneTrackingStruct::operator=0056C2E0, member-wise list and tracking-state assignment.
SubzoneTrackingStruct &SubzoneTrackingStruct::operator=(SubzoneTrackingStruct const &other)
{
    Connections = other.Connections;
    unknown_18 = other.unknown_18;
    unknown_1c = other.unknown_1c;
    unknown_20 = other.unknown_20;
    return *this;
}


// DynamicVectorClass<SubzoneTrackingStruct> scalar deleting destructor (??_G) (0x0056C390)
// Compiler generated with the template vtable.
// No source of its own: emitted by this unit (the Map unit, population-2652).


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<HashObject<unsigned int, unsigned int>>::Resize (0x0056C4C0).
// VectorClass<HashObject<unsigned int, unsigned int>>::Clear (0x0056C590; VectorClass<HashObject<unsigned int, unsigned int>> vtable 0x007a5af8 slot 3).
// The element type is left incomplete: Clear does not depend on it.
// VectorClass<HashObject<unsigned int, unsigned int>>::ID_object (0x0056C5C0).
// VectorClass<HashObject<unsigned int, unsigned int>>::ID_pointer (0x0056C600; VectorClass<HashObject<unsigned int, unsigned int>> vtable 0x007a5af8 slot 5).
// HashObject<unsigned int, unsigned int> is a size-only stand-in (8 bytes, from the ID(T const *) divisor);
// its real fields are not recovered.
// VectorClass<HashObject<unsigned int, SubzoneConnectionStruct>>::Resize (0x0056C6D0).
// VectorClass<HashObject<unsigned int, SubzoneConnectionStruct>>::ID_object (0x0056C800).


extern "C" unsigned char vtable_56c870[]; extern "C" void __cdecl release_buffer_56c870(void*); extern "C" void __cdecl release_object_56c870(void*);
class Leaf_56c870 { public: void* get(unsigned int flags); };
void* Leaf_56c870::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_56c870; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_56c870((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_56c870(this); return this; }


extern "C" unsigned char vtable_56c8c0[]; extern "C" void __cdecl release_buffer_56c8c0(void*); extern "C" void __cdecl release_object_56c8c0(void*);
class Leaf_56c8c0 { public: void* get(unsigned int flags); };
void* Leaf_56c8c0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_56c8c0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_56c8c0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_56c8c0(this); return this; }


extern "C" unsigned char vtable_56c910[]; extern "C" void __cdecl release_buffer_56c910(void*); extern "C" void __cdecl release_object_56c910(void*);
class Leaf_56c910 { public: void* get(unsigned int flags); };
void* Leaf_56c910::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_56c910; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_56c910((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_56c910(this); return this; }


extern "C" unsigned char vtable_56c960[]; extern "C" void __cdecl release_buffer_56c960(void*); extern "C" void __cdecl release_object_56c960(void*);
class Leaf_56c960 { public: void* get(unsigned int flags); };
void* Leaf_56c960::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_56c960; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_56c960((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_56c960(this); return this; }


// DynamicVectorClass<SubzoneConnectionStruct>::operator= (0x0056C9B0)
// Template member instantiated from shared/vector_class.hpp.
// No source of its own: emitted by this unit (the Map unit, population-2652).


// VectorClass<SubzoneConnectionStruct>::VectorClass(unsigned int, SubzoneConnectionStruct const *) (0x0056CA50)
// Template constructor instantiated from shared/vector_class.hpp.
// No source of its own: emitted by this unit (the Map unit, population-2652).


// VectorClass<SubzoneConnectionStruct>::operator= (0x0056CAF0)
// Template member instantiated from shared/vector_class.hpp.
// No source of its own: emitted by this unit (the Map unit, population-2652).


// VectorClass<ZoneConnectionClass>::VectorClass(unsigned int, ZoneConnectionClass const *) (0x0056CB90)
// Template constructor instantiated from shared/vector_class.hpp.
// No source of its own: emitted by this unit (the Map unit, population-2652).


// VectorClass<SubzoneTrackingStruct>::VectorClass(unsigned int, SubzoneTrackingStruct const *) (0x0056CC40)
// Template constructor instantiated from shared/vector_class.hpp.
// No source of its own: emitted by this unit (the Map unit, population-2652).


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<HashObject<unsigned int, SubzoneConnectionStruct>>::Add (0x0056CD60).
// Layout and behaviour: shared/vector_class.hpp, element shared/map_elements.hpp. The Map
// unit's copy (0x00564E38 and 0x00564ED2 add a subzone connection to a hash bucket).
