// Native layout check (Scout1098), as check_light_views.cpp: the Map unit's
// stand-ins in shared/map_techno.hpp, whose VC6 padding port/fixups.json
// respells, against the full classes' arm64 offsets (object_type.hpp,
// techno_type.hpp, object.hpp, techno.hpp, building_type_class.hpp,
// overlay_type.hpp, house_type.hpp, building_class.hpp, anim.hpp; measured
// with offsetof on the native view). Compile-time only; defines nothing.
#define RA2_MAP_COLLAPSE_CLIFF
#define RA2_MAP_CLOSE_OBJECT
#include "shared/rect.hpp"
#include "shared/coord.hpp"
#include "shared/map_techno.hpp"
#include "shared/map_overlay.hpp"
#include <cstddef>

static_assert(offsetof(ObjectTypeClass, unknown_205) == 0x325, "map_techno.hpp ObjectTypeClass IsFootprint");
static_assert(offsetof(UnknownMapTechnoType, unknown_500) == 0x6F0, "map_techno.hpp TechnoTypeClass MovementZone");
static_assert(offsetof(ObjectClass, unknown_74) == 0xB8, "map_techno.hpp ObjectClass::unknown_74");
static_assert(offsetof(ObjectClass, unknown_76) == 0xBA, "map_techno.hpp ObjectClass::unknown_76");
static_assert(offsetof(ObjectClass, unknown_79) == 0xBD, "map_techno.hpp ObjectClass::unknown_79");
static_assert(offsetof(TechnoClass, unknown_30) == 0x40, "map_techno.hpp TechnoClass::unknown_30");
static_assert(offsetof(TechnoClass, unknown_74) == 0xB8, "map_techno.hpp TechnoClass::unknown_74");
static_assert(offsetof(TechnoClass, unknown_7d) == 0xC1, "map_techno.hpp TechnoClass::unknown_7d");
static_assert(offsetof(TechnoClass, House) == 0x218, "map_techno.hpp TechnoClass::House");
static_assert(offsetof(TechnoClass, unknown_1b8) == 0x220, "map_techno.hpp TechnoClass::unknown_1b8");
static_assert(offsetof(TechnoClass, unknown_32d) == 0x3DD, "map_techno.hpp TechnoClass::unknown_32d");
static_assert(offsetof(TechnoClass, unknown_372) == 0x422, "map_techno.hpp TechnoClass::unknown_372");
static_assert(offsetof(BuildingTypeClass, ThreatRange) == 0x6F4, "map_techno.hpp BuildingTypeClass GuardRange");
static_assert(offsetof(BuildingTypeClass, Speed) == 0x7BC, "map_techno.hpp BuildingTypeClass SpeedType");
static_assert(offsetof(BuildingTypeClass, ToOverlay) == 0xEF0, "map_techno.hpp BuildingTypeClass::ToOverlay");
static_assert(offsetof(BuildingTypeClass, OccupyHeight) == 0xF9C, "map_techno.hpp BuildingTypeClass::OccupyHeight");
static_assert(offsetof(BuildingTypeClass, AddOccupy) == 0x1614, "map_techno.hpp BuildingTypeClass::AddOccupy");
static_assert(offsetof(BuildingTypeClass, RemoveOccupy) == 0x1654, "map_techno.hpp BuildingTypeClass::RemoveOccupy");
static_assert(offsetof(BuildingTypeClass, Gate) == 0x16A1, "map_techno.hpp BuildingTypeClass::Gate");
static_assert(offsetof(BuildingTypeClass, FirestormWall) == 0x16AA, "map_techno.hpp BuildingTypeClass::FirestormWall");
static_assert(offsetof(BuildingTypeClass, CanHideThings) == 0x1736, "map_techno.hpp BuildingTypeClass::CanHideThings");
static_assert(offsetof(OverlayTypeClass, unknown_264) == 0x3B8, "map_techno.hpp OverlayTypeClass ArrayIndex");
static_assert(offsetof(BuildingClass, Type) == 0x518, "map_techno.hpp BuildingClass::Type");
static_assert(offsetof(HouseTypeClass, House) == 0xC8, "map_techno.hpp HouseTypeClass::House");
static_assert(offsetof(HouseTypeClass, WallOwner) == 0x1DF, "map_techno.hpp HouseTypeClass::WallOwner");
static_assert(offsetof(AnimClass, unknown_177) == 0x1CF, "map_techno.hpp AnimClass::unknown_177");
static_assert(sizeof(AnimClass) == 0x228, "map_techno.hpp sizeof(AnimClass)");
static_assert(offsetof(OverlayClass, Class) == 0xE0, "map_overlay.hpp OverlayClass::Class");
