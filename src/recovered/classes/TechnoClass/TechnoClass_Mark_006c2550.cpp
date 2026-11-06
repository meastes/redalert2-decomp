// bool TechnoClass::Mark(int arg1) (0x006C2550, slot 71)
// The ObjectClass version, then radio message 13 when unknown_370 is set.
#include "shared/techno.hpp"
// @ref ?Mark@ObjectClass@@UAE_NH@Z=005d4160

bool TechnoClass::Mark(int arg1)
{
    if (ObjectClass::Mark(arg1)) {
        if (unknown_370) {
            unknown_slot149(13);
        }
        return true;
    }
    return false;
}
