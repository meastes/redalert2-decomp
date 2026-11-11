// bool FootClass::unknown_slot283() (0x004CA820)
// True without a primary weapon, for tech level -1, for types that deploy
// into a building, or for speeds up to 13.
#include "shared/foot.hpp"

bool FootClass::unknown_slot283()
{
    if (unknown_slot231(0) == 0) {
        return true;
    }
    if (Techno_Type_Class()->TechLevel == -1) {
        return true;
    }
    if (Techno_Type_Class()->DeploysInto != 0) {
        return true;
    }
    return Techno_Type_Class()->Speed <= 13 ? true : false;
}
