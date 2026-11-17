#define RA2_SDK_HEADERS
// The unit's coordinate is TibSun's Coord on TPoint3D with point.h's member operators (shared/coord.hpp), and
// Coord::As_Cell (Find_First_Obstacle_004bb890, from mischa85 findpath2.cpp).
#define RA2_COORD_TPOINT3D
#define RA2_POINT3D_TIBSUN
#define RA2_COORD_AS_CELL
#define RA2_COORD_AS_CELL_NO_CASTS
// Get_Obstacle_004bbaf0 calls CellClass 0x0047CF10 as a member (shared/cell_class.hpp).
#define RA2_CELL_DECLARE_BRIDGE_ADJUSTED_HEIGHT
// Unit converted to the pinned SDK headers (RA2_SDK_HEADERS, docs/DECISIONS.md). Include order of the original
// findpath2.cpp:6-8 (always.h, rect.h, cell.h; mischa85 src/findpath2.cpp, ra2-decomp src/findpath2.cpp):
#include "shared/always.hpp"
#include "shared/rect.hpp"
#include "shared/cell_class.hpp"
// The screen chain cell_class.hpp brings in (RA2_SDK_HEADERS) makes the unit emit these template members and
// helpers; each is the original's copy compiled in another unit:
// Header-static dynamic initializers of the unit at 0x004BB6A0: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x004BB6A0-0x004BB881); the other
// drafts of this plan are emitted by these includes.
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
