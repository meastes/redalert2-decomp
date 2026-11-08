// int BuildingLightClass::unknown_func_434610() const (0x00434610)
// The light's distance from its owner as a step: 0 inside the rules' near
// range (+0x6B4), then tenths of the span up to the far range (+0x6B0).
// TibSun blight.cpp Sweep_Stage shape (OpenTS; SPDX-License-Identifier:
// GPL-3.0-or-later; see THIRD_PARTY.md) with the distance written as the
// Yuri's Revenge twin's `Distance_To(Owner)` (ra2-decomp src/blight.cpp
// banked form). ObjectClass::Distance_To(AbstractClass const *) is TibSun's
// object.h inline (RA2_BUILDING_LIGHT_DISTANCE_TO, defined in the unit's
// first row 0x00432DD0); its TPoint3D::Length (RA2_TPOINT3D_LENGTH_TIBSUN)
// is the 0x0041B780 copy that slot 67's inlined copy of this function calls.
#include "shared/building_light.hpp"
#include "shared/rules.hpp"

int BuildingLightClass::unknown_func_434610() const
{
    int distance = Distance_To(Owner);
    int near_range = Rules->unknown_6b4;
    if (distance < near_range) {
        return 0;
    }
    return (distance - near_range) / ((Rules->unknown_6b0 - near_range) / 10);
}
