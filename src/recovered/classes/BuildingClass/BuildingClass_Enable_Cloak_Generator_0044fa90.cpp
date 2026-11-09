// void BuildingClass::Enable_Cloak_Generator_0044fa90() (0x0044FA90): TibSun Enable_Cloak_Generator. A cloak
// generator's field starts growing again: state +0x5BB = 1, the radius +0x5BC restarts from 0 when it stood at the
// type's full CloakRadiusInCells, and the building is flagged for redraw. No call site references it: Captured
// 0x00444660 expands it (declared under RA2_BUILDING_CLOAK_GENERATOR, defined by this unit's first row).
#include "shared/building.hpp"

void BuildingClass::Enable_Cloak_Generator_0044fa90()
{
    if (Type->CloakGenerator) {
        unknown_5bb = 1;
        if (unknown_5bc == Type->CloakRadiusInCells) {
            unknown_5bc = 0;
        }
        unknown_75 = true;
    }
}
