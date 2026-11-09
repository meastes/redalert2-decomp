// Native layout check (Scout1108), as check_light_views_cell.cpp: the cell views
// of shared/anim.hpp, terrain.hpp and locomotion.hpp, whose VC6
// padding port/fixups.json respells, against CellClass's arm64 offsets
// (shared/cell_class.hpp; native/tools/view_audit.py measures every cell view,
// in-file ones included, and runs as the native-view-audit test).
// Compile-time only.
#include "shared/anim.hpp"
#include "shared/terrain.hpp"
#include "shared/locomotion.hpp"
#include "shared/cell_class.hpp"
#include <cstddef>

#if defined(__LP64__)
static_assert(offsetof(UnknownAnimCell, unknown_24) == offsetof(CellClass, MapCoords), "anim.hpp UnknownAnimCell MapCoords");
static_assert(offsetof(UnknownAnimCell, unknown_44) == offsetof(CellClass, OverlayIndex), "anim.hpp UnknownAnimCell OverlayIndex");
static_assert(offsetof(UnknownAnimCell, unknown_e4) == offsetof(CellClass, OccupierPtr), "anim.hpp UnknownAnimCell OccupierPtr");
static_assert(offsetof(UnknownAnimCell, unknown_ec) == offsetof(CellClass, unknown_ec), "anim.hpp UnknownAnimCell unknown_ec");
static_assert(offsetof(UnknownAnimCell, unknown_10a) == offsetof(CellClass, unknown_10a), "anim.hpp UnknownAnimCell unknown_10a");
static_assert(offsetof(UnknownAnimCell, unknown_11b) == offsetof(CellClass, unknown_11b), "anim.hpp UnknownAnimCell unknown_11b");
static_assert(offsetof(UnknownAnimCell, unknown_11c) == offsetof(CellClass, unknown_11c), "anim.hpp UnknownAnimCell unknown_11c");
static_assert(offsetof(UnknownAnimCell, unknown_11e) == offsetof(CellClass, OverlayData), "anim.hpp UnknownAnimCell OverlayData");
static_assert(offsetof(UnknownAnimCell, unknown_124) == 0x16C, "anim.hpp UnknownAnimCell Occupy_124");
static_assert(offsetof(UnknownAnimCell, unknown_128) == 0x170, "anim.hpp UnknownAnimCell Occupy_128");
static_assert(offsetof(UnknownAnimCell, Flags) == 0x188, "anim.hpp UnknownAnimCell flags +0x140");
static_assert(offsetof(TerrainCellView, unknown_124) == 0x16C, "terrain.hpp TerrainCellView Occupy_124");
static_assert(offsetof(UnknownLocoCell, Flags) == 0x188, "locomotion.hpp UnknownLocoCell flags +0x140");
static_assert(offsetof(CellClass, unknown_122) == 0x16A && offsetof(CellClass, OccupierPtr) == 0x118, "cell_class.hpp arm64 offsets");
#endif
