// bool TechnoClass::Restore_Mission() (0x006CD880, slot 120)
// Restores the suspended mission and the +0x22C value.
#include "shared/techno.hpp"
// @ref ?Restore_Mission@MissionClass@@UAE_NXZ=00594d80

bool TechnoClass::Restore_Mission()
{
    if (MissionClass::Restore_Mission()) {
        unknown_slot220(unknown_22c);
        return true;
    }
    return false;
}
