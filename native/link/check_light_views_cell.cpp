// Native layout check (Scout1098), as check_light_views.cpp: shared/techno.hpp's
// UnknownCellClass (the cell as the techno units see it), whose VC6 padding
// port/fixups.json respells, against CellClass's arm64 offsets (shared/
// cell_class.hpp, measured with offsetof on the native view). Compile-time only.
#include "shared/techno.hpp"
#include <cstddef>

static_assert(offsetof(UnknownCellClass, unknown_24) == 0x32, "techno.hpp UnknownCellClass MapCoords");
static_assert(offsetof(UnknownCellClass, unknown_28) == 0x38, "techno.hpp UnknownCellClass FoggedObjects");
static_assert(offsetof(UnknownCellClass, unknown_38) == 0x58, "techno.hpp UnknownCellClass IsoTileIndex");
static_assert(offsetof(UnknownCellClass, unknown_3c) == 0x60, "techno.hpp UnknownCellClass unknown_3c");
static_assert(offsetof(UnknownCellClass, unknown_44) == 0x70, "techno.hpp UnknownCellClass OverlayIndex");
static_assert(offsetof(UnknownCellClass, unknown_50) == 0x7C, "techno.hpp UnknownCellClass unknown_50");
static_assert(offsetof(UnknownCellClass, unknown_58) == 0x84, "techno.hpp UnknownCellClass unknown_58");
static_assert(offsetof(UnknownCellClass, unknown_e4) == 0x118, "techno.hpp UnknownCellClass OccupierPtr");
static_assert(offsetof(UnknownCellClass, unknown_e8) == 0x120, "techno.hpp UnknownCellClass AltOccupierPtr");
static_assert(offsetof(UnknownCellClass, unknown_ec) == 0x128, "techno.hpp UnknownCellClass unknown_ec");
static_assert(offsetof(UnknownCellClass, unknown_10a) == 0x152, "techno.hpp UnknownCellClass unknown_10a");
static_assert(offsetof(UnknownCellClass, unknown_11b) == 0x163, "techno.hpp UnknownCellClass unknown_11b");
static_assert(offsetof(UnknownCellClass, unknown_11e) == 0x166, "techno.hpp UnknownCellClass OverlayData");
static_assert(offsetof(UnknownCellClass, unknown_122) == 0x16A, "techno.hpp UnknownCellClass unknown_122");
static_assert(offsetof(UnknownCellClass, unknown_124) == 0x16C, "techno.hpp UnknownCellClass Occupy_124");
static_assert(offsetof(UnknownCellClass, unknown_128) == 0x170, "techno.hpp UnknownCellClass Occupy_128");
static_assert(offsetof(UnknownCellClass, unknown_12c) == 0x174, "techno.hpp UnknownCellClass flags +0x12C");
static_assert(offsetof(UnknownCellClass, Flags) == 0x188, "techno.hpp UnknownCellClass flags +0x140");
