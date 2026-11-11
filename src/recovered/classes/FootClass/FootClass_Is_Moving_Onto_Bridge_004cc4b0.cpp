// bool FootClass::Is_Moving_Onto_Bridge(TechnoClass *techno) (0x004CC4B0)
// ObjectClass's slot 46 only while +0x574 is negative.
#include "shared/foot.hpp"
// @ref ?Is_Moving_Onto_Bridge@ObjectClass@@UAE_NPAVTechnoClass@@@Z=005d52f0

bool FootClass::Is_Moving_Onto_Bridge(TechnoClass *techno)
{
    if (unknown_574 >= 0) {
        return false;
    }
    return ObjectClass::Is_Moving_Onto_Bridge(techno);
}
