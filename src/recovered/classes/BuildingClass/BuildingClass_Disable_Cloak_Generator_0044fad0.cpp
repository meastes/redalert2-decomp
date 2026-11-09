// void BuildingClass::Disable_Cloak_Generator_0044fad0() (0x0044FAD0): TibSun Disable_Cloak_Generator. A cloak
// generator's field starts collapsing: state +0x5BB = -1, the radius +0x5BC starts from the type's full
// CloakRadiusInCells when it stood at 0, and the building is flagged for redraw. No call site references it:
// Captured 0x00444660 expands it (declared under RA2_BUILDING_CLOAK_GENERATOR, defined by this unit's first row).
#include "shared/building.hpp"

void BuildingClass::Disable_Cloak_Generator_0044fad0()
{
    if (Type->CloakGenerator) {
        unknown_5bb = -1;
        if (unknown_5bc == 0) {
            unknown_5bc = Type->CloakRadiusInCells;
        }
        unknown_75 = true;
    }
}
