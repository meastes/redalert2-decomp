// BuildingClass *ObjectTypeClass::Who_Can_Build_Me(bool in_theory, bool legal, bool check_limit, HouseClass *house) const (0x005D5FC0, slot 37)
// Scans the house's buildings for a factory of this type's kind that is
// out of limbo, not being sold (mission 0x13), owned by an allowed house
// and, as asked, powered (legal) and below the build limit (check_limit).
// Aircraft (kind 3) not built "in theory" need a helipad with a free radio
// slot; a naval unit type (kind 0x28) needs a naval yard and others a land
// factory. A primary factory (+0x32B) wins at once, else the last match.
#include "shared/techno_type.hpp"
#include "shared/house.hpp"
#include "shared/techno.hpp"
#include "shared/building_type_class.hpp"
// @ref ?In_Radio_Contact@RadioClass@@QBE_NXZ=00636f10
// @ref ?Unknown_00636EA0@RadioClass@@QBE_NXZ=00636ea0
// @ref ?Can_Build@HouseClass@@QAEHPBVTechnoTypeClass@@_N1@Z=004e3660

BuildingClass *ObjectTypeClass::Who_Can_Build_Me(bool in_theory, bool legal, bool check_limit, HouseClass *house) const
{
    BuildingClass *anybuilding = 0;
    int ownable = unknown_slot28();
    int count = house->Buildings.Count();
    for (int index = 0; index < count; index++) {
        BuildingClass *building = house->Buildings[index];
        if (building->unknown_76) {
            continue;
        }
        if (building->Type->Factory != Kind_Of()) {
            continue;
        }
        if (legal && !building->unknown_530) {
            continue;
        }
        if (building->unknown_slot91() == 0x13 || building->MissionQueue == 0x13) {
            continue;
        }
        if (check_limit && building->House->Can_Build((TechnoTypeClass const *)this, 1, 1) <= 0) {
            continue;
        }
        if ((building->Type->unknown_slot28() & ownable) == 0) {
            continue;
        }
        if (!in_theory && building->In_Radio_Contact() && Kind_Of() == 3) {
            if (Kind_Of() == 3 && building->Unknown_00636EA0()) {
                if (building->unknown_32b) {
                    return building;
                }
                anybuilding = building;
            }
            continue;
        }
        if (Kind_Of() == 0x28 && ((TechnoTypeClass const *)this)->Naval) {
            if (!building->Type->Naval) {
                continue;
            }
            if (building->unknown_32b) {
                return building;
            }
        } else if (building->Type->Naval) {
            continue;
        }
        if (building->unknown_32b) {
            return building;
        }
        anybuilding = building;
    }
    if (anybuilding != 0) {
        return anybuilding;
    }
    return 0;
}
