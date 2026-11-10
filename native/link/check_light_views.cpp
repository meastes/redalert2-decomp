// Native layout check (Scout1089): the light views of shared/map_view.hpp
// (screen layers) and shared/house_screen.hpp (HouseClass), whose VC6 padding
// port/fixups.json respells, must place the data other units read at the
// arm64 offsets of the full classes. native/link/behaviour_map_dimensions.cpp
// and behaviour_scenario.cpp assert the same numbers on the full headers.
// Compile-time only; defines nothing.
#include "shared/subzone_tracking.hpp"
#include "shared/map_view.hpp"
#include <cstddef>

static_assert(offsetof(MapClass, CellSubzones) == 0xB8, "map_view.hpp MapClass::CellSubzones");
static_assert(offsetof(MapClass, unknown_70) == 0xC0, "map_view.hpp MapClass::unknown_70");
static_assert(offsetof(MapClass, SubzoneTracking) == 0xE8, "map_view.hpp MapClass::SubzoneTracking");
static_assert(offsetof(MapClass, MapSize) == 0x168, "map_view.hpp MapClass::MapSize");
static_assert(offsetof(MapClass, MapLocalSize) == 0x178, "map_view.hpp MapClass::MapLocalSize");
static_assert(offsetof(MapClass, unknown_120) == 0x1A8, "map_view.hpp MapClass::unknown_120");
static_assert(offsetof(DisplayClass, unknown_11a0) == 0x1258, "map_view.hpp DisplayClass::unknown_11a0");
static_assert(offsetof(DisplayClass, unknown_11b4) == 0x1274, "map_view.hpp DisplayClass::unknown_11b4");
static_assert(offsetof(RadarClass, unknown_11b8) == 0x1278, "map_view.hpp RadarClass::unknown_11b8");
static_assert(offsetof(RadarClass, unknown_1220) == 0x12F0, "map_view.hpp RadarClass::unknown_1220");
static_assert(offsetof(SidebarClass, Column) == 0x16E8, "map_view.hpp SidebarClass::Column");
static_assert(sizeof(SidebarClass::StripClass) == 0x10C0, "map_view.hpp SidebarClass::StripClass");
// shared/house_screen.hpp's light HouseClass: check_light_views_house.cpp.
