// bool TechnoTypeClass::unknown_slot42(Cell const &cell, HouseClass *house) const (0x006DE0E0, slot 42)
// Whether the type can be placed at a cell (hypothesis: Legal_Placement):
// every cell of its occupy list (slot 36) must be open for it, by
// 0x004718B0 for building types (Kind_Of 7) and 0x00478760 otherwise; a
// building type with a +0xBF0 tile type (BuildingTypeClass::unknown_bf0) needs only
// one open cell.
#include "shared/techno_type.hpp"
#include "shared/object.hpp"
#include "shared/map_view.hpp"
#include "shared/building_type_class.hpp"
// @ref ?Map@@3VMouseClass@@A=008324e0
// @ref ?Unknown_004718B0@UnknownPlacementCellView@@QAE_NHPBVTechnoTypeClass@@PAVHouseClass@@@Z=004718b0
// @ref ?Unknown_00478760@UnknownPlacementCellView@@QAE_NH_N0HHH0@Z=00478760
// @ref _unknown_geometry_k=00ac01e8

// Member-wise sum of two cells: shared/coord.hpp.

bool TechnoTypeClass::unknown_slot42(Cell const &cell, HouseClass *house) const
{
    if (cell == unknown_geometry_k) {
        return false;
    }
    Cell const *offset = unknown_slot36(1);
    bool building = Kind_Of() == 7;
    bool blocked = false;
    bool open = false;
    while (offset && *offset != Cell(0x7FFF, 0x7FFF)) {
        Cell place = cell + *offset++;
        if (building) {
            if (!((UnknownPlacementCellView &)Map[place]).Unknown_004718B0(SpeedType, this, house)) {
                blocked = true;
            } else {
                open = true;
            }
        } else {
            if (!((UnknownPlacementCellView &)Map[place]).Unknown_00478760(SpeedType, false, false, -1, 0, -1, true)) {
                blocked = true;
            } else {
                open = true;
            }
        }
    }
    if (building && static_cast<BuildingTypeClass const *>(this)->unknown_bf0) {
        return open;
    }
    return !blocked;
}
