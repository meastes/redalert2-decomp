// bool TechnoClass::unknown_slot158() (0x006C7480, slot 158)
// Whether the type's +0xB0 entry is set and starts with a zero byte.
#include "shared/techno.hpp"

bool TechnoClass::unknown_slot158()
{
    if (Techno_Type_Class()->unknown_b0.unknown_00 == 0) {
        return false;
    }
    return *(char const *)Techno_Type_Class()->unknown_b0.unknown_00 == 0 ? true : false;
}
