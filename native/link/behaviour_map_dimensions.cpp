// Set_Map_Dimensions support (Scout1089; skirmish harness, Read_Scenario_INI ->
// RadarClass slot 28): the RadarClass thunk to MapClass's slot 28 and the
// real-layout accessors the adapted draft native/drafts/005484e0.cpp calls in
// place of its overlay's stand-in members. NOT EXACT; never original code or bytes.
#define RA2_CELL_FULL_ASSIGN   // as the Map unit: declares Set_CellID (same layout)
#include "shared/map_cell.hpp"   // the Map units' view: declares Set_CellID (first: later headers bring the plain class)
#include "shared/map.hpp"
#include "shared/radar.hpp"
#include "shared/object.hpp"
#include "shared/anim.hpp"
#include "shared/particle_system.hpp"
#include "shared/sidebar.hpp"
#include "shared/building_type_class.hpp"
#include <cstddef>

// The arm64 offsets the light views of shared/map_view.hpp are respelled to
// (port/fixups.json; check_map_view_layout.cpp asserts the light side).
static_assert(offsetof(MapClass, CellSubzones) == 0xB8, "full MapClass::CellSubzones");
static_assert(offsetof(MapClass, unknown_70) == 0xC0, "full MapClass::unknown_70");
static_assert(offsetof(MapClass, SubzoneTracking) == 0xE8, "full MapClass::SubzoneTracking");
static_assert(offsetof(MapClass, MapSize) == 0x168, "full MapClass::MapSize");
static_assert(offsetof(MapClass, MapLocalSize) == 0x178, "full MapClass::MapLocalSize");
static_assert(offsetof(MapClass, unknown_120) == 0x1A8, "full MapClass::unknown_120");
static_assert(sizeof(MapClass) == 0x1210, "full MapClass");
static_assert(offsetof(DisplayClass, unknown_11a0) == 0x1258, "full DisplayClass::unknown_11a0");
static_assert(offsetof(DisplayClass, unknown_11b4) == 0x1274, "full DisplayClass::unknown_11b4");
static_assert(offsetof(RadarClass, unknown_11b8) == 0x1278, "full RadarClass::unknown_11b8");
static_assert(offsetof(RadarClass, unknown_1220) == 0x12F0, "full RadarClass::unknown_1220");
static_assert(offsetof(SidebarClass, Column) == 0x16E8, "full SidebarClass::Column");
static_assert(sizeof(SidebarClass::StripClass) == 0x10C0, "full StripClass");
static_assert(offsetof(CellClass, MapCoords) == 0x32, "Building/Terrain slot 101 cell views (port/fixups.json)");

// The draft's stand-in reads, on the real classes (TibSun map.cpp Set_Map_Dimensions:
// Get_Coord, PositionCell, anim->xObject, ParticleSystemClass Class->Behaves_Like()).

// ObjectClass +0x88 Coord (object.hpp Position_Coord_004f3f40).
CoordStruct ra2native_object_coord_5484e0(ObjectClass const *object)
{
    return object->Position_Coord_004f3f40();
}

// ObjectClass slot 104 (+0x1A0, 0x0041B320): the object's cell.
Cell ra2native_object_cell_5484e0(ObjectClass const *object)
{
    return object->unknown_slot104();
}

// AnimClass +0xB4: the object the animation is attached to.
bool ra2native_anim_attached_5484e0(ObjectClass const *anim)
{
    return static_cast<AnimClass const *>(anim)->unknown_b4 != 0;
}

// A particle system (Kind_Of 0x18) whose type's BehavesLike (+0x284) is 1 (Gas).
bool ra2native_is_gas_system_5484e0(ObjectClass const *object)
{
    return static_cast<ParticleSystemClass const *>(object)->Class->BehavesLike == 1;
}

// 0x0047C2E0: techno.hpp's cell view UnknownCellClass names CellClass::Is_Sensed (recovered in
// src/recovered/cell.cpp); the view's callers reach it through this forwarder.
class UnknownCellClass {
public:
    bool Is_Sensed(int house);     // 0x0047C2E0 (as shared/techno.hpp's view)
};

bool UnknownCellClass::Is_Sensed(int house)
{
    return ((CellClass const *)this)->Is_Sensed(house);
}

// 0x004718B0 under object.hpp's placement view (TechnoTypeClass slot 42 and
// BuildingClass/TerrainClass slot 101 call it on Map[cell]): the cell's
// Is_Clear_To_Build (accepted in src/recovered/cell.cpp; the type is a
// BuildingTypeClass there).
bool UnknownPlacementCellView::Unknown_004718B0(int speed, TechnoTypeClass const *type, HouseClass *house)
{
    return ((CellClass *)this)->Is_Clear_To_Build(speed, (BuildingTypeClass const *)type, house);
}

// 0x006328D0 (leaf src/recovered/006328d0.cpp): the radar dot offsets of a
// building type's foundation, element Foundation (+0xC78) of the 22 vectors at
// +0x1274 (built by 0x00632700).
DynamicVectorClass<Point2D> *RadarClass::Unknown_006328D0(BuildingTypeClass *type)
{
    return &unknown_1274[type->Foundation];
}
