// bool BuildingClass::unknown_4444b0(bool take) 0x004444B0: makes this the primary factory of its
// factory type and naval group unless another building already is (that one gives way when take).
// Returns whether this building's primary state changed. Shape: YR twin FUN_00448070 (ra2-decomp).
#include "shared/building.hpp"
#include "shared/house.hpp"
bool BuildingClass::unknown_4444b0(bool take)
{
    int factory = Type->Factory;
    if (factory == 0) {
        return false;
    }
    bool was = unknown_32b;
    int count = House->Buildings.Count();
    BuildingClass *building;
    for (int index = 0; index < count; index++) {
        building = House->Buildings[index];
        if (!building->unknown_76 && building->unknown_32b) {
            if (building->Type->Factory == factory) {
                if ((Type->Naval && building->Type->Naval) || (!Type->Naval && !building->Type->Naval)) {
                    if (take) {
                        building->unknown_32b = false;
                        unknown_32b = true;
                    }
                    goto done;
                }
            }
        }
    }
    unknown_32b = true;
done:
    return unknown_32b != was;
}
