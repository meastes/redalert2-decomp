// bool FootClass::unknown_slot46(TechnoClass *techno) (0x004CC4B0)
// ObjectClass's slot 46 only while +0x574 is negative.
#include "shared/foot.hpp"
// @ref ?unknown_slot46@ObjectClass@@UAE_NPAVTechnoClass@@@Z=005d52f0

bool FootClass::unknown_slot46(TechnoClass *techno)
{
    if (unknown_574 >= 0) {
        return false;
    }
    return ObjectClass::unknown_slot46(techno);
}
