// bool BuildingLightClass::Limbo() (0x004347C0, slot 52)
// ObjectClass's slot 52, then removes the light from the list at 0x00832470.
#include "shared/building_light.hpp"
// @ref ?Limbo@ObjectClass@@UAE_NXZ=005d3700
// @ref ?Remove@UnknownList832470@@QAEXPAVObjectClass@@@Z=0053e370
// @ref ?unknown_list_832470@@3VUnknownList832470@@A=00832470

bool BuildingLightClass::Limbo()
{
    if (ObjectClass::Limbo()) {
        unknown_list_832470.Remove(this);
        return true;
    }
    return false;
}
