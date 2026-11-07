// bool FootClass::unknown_slot120() (0x004C7B00)
// TechnoClass's; on success also slot 260 with unknown_498.
#include "shared/foot.hpp"
// @ref ?unknown_slot120@TechnoClass@@UAE_NXZ=006cd880

bool FootClass::unknown_slot120()
{
    if (TechnoClass::unknown_slot120()) {
        Assign_Destination(unknown_498, true);
        return true;
    }
    return false;
}
