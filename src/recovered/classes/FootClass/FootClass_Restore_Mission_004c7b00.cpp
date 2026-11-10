// bool FootClass::Restore_Mission() (0x004C7B00)
// TechnoClass's; on success also slot 260 with unknown_498.
#include "shared/foot.hpp"
// @ref ?Restore_Mission@TechnoClass@@UAE_NXZ=006cd880

bool FootClass::Restore_Mission()
{
    if (TechnoClass::Restore_Mission()) {
        Assign_Destination(unknown_498, true);
        return true;
    }
    return false;
}
