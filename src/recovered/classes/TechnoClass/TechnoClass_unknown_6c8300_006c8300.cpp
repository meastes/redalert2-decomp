// void TechnoClass::unknown_6c8300() (0x006C8300)
// Restarts the reload timer (+0x194) while below the type's Ammo:
// EmptyReload when empty (if set), else Reload plus ReloadIncrement times
// the square of the full PipWrap groups loaded.
#include "shared/techno.hpp"
// @ref ?Frame@@3HA=00a40d2c

void TechnoClass::unknown_6c8300()
{
    int ammo = Techno_Type_Class()->Ammo;
    if (unknown_254 < ammo) {
        if (unknown_254 == 0 && Techno_Type_Class()->EmptyReload != -1) {
            unknown_194 = CDTimerClass<FrameTimerClass>(Techno_Type_Class()->EmptyReload);
        } else {
            int groups = Techno_Type_Class()->PipWrap == 0 ? 1 : unknown_254 / Techno_Type_Class()->PipWrap;
            int delay = Techno_Type_Class()->ReloadIncrement * groups * groups;
            unknown_194 = CDTimerClass<FrameTimerClass>(Techno_Type_Class()->Reload + delay);
        }
    }
}
