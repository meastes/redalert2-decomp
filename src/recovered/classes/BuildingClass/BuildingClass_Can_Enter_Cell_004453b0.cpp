// int BuildingClass::Can_Enter_Cell(...) (0x004453B0; ret 0x14)
// Whether the building may enter (stand in) a cell: 0 when it may, 7 when it
// may not (hypothesis: Can_Enter_Cell's MOVE_OK / MOVE_NO). A deployed
// building (its type undeploys into a unit) on the map asks the cell
// whether the type's movement may enter; otherwise the type answers for its
// owner (BuildingTypeClass slot 42). arg1 is the cell (its coordinates at
// +0x24); the other arguments are unused.
#include "shared/building.hpp"
#include "shared/gscreen.hpp"
#include "shared/building_map.hpp"
// @ref ?Unknown_004718B0@UnknownPlacementCellView@@QAE_NHPBVTechnoTypeClass@@PAVHouseClass@@@Z=004718b0

// The cell argument's coordinates (a view of CellClass).
struct UnknownBuildingCellArg {
    char unknown_00[0x24];
    Cell MapCoords;                   // +0x24
};

int BuildingClass::Can_Enter_Cell(CellClass *arg1, int arg2, int arg3, CellClass *arg4, bool arg5)
{
    Cell cell = ((UnknownBuildingCellArg *)arg1)->MapCoords;
    if (Type->UndeploysInto != 0 && unknown_74) {
        return ((UnknownPlacementCellView &)Map[cell]).Unknown_004718B0(Type->SpeedType, Type, House) ? 0 : 7;
    }
    return Type->Legal_Placement(cell, House) ? 0 : 7;
}
