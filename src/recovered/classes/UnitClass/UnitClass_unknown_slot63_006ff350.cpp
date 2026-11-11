// bool UnitClass::unknown_slot63(Rect *bounds, bool arg2, bool arg3) (0x006FF350, slot 63)
// Not drawn while +0x370 is set and the first contact is a building in or
// queued for mission 16 and the state at +0x2A8 is busy; otherwise the
// object behaviour (with arg3 cleared).
#include "shared/unit.hpp"
// @ref ?Contact_With_Whom@RadioClass@@QBEPAVTechnoClass@@H@Z=00636e20
// @ref ?unknown_slot63@ObjectClass@@UAE_NPAVRect@@_N1@Z=005d34e0
// @ref ?Is_Count_Up_Active@DelayTimerClass@@QBE_NXZ=00496a90
// @ref ?Is_Count_Down_Active@DelayTimerClass@@QBE_NXZ=00496ab0
// @ref ?Is_Count_Up_Complete@DelayTimerClass@@QBE_NXZ=00496b30
// @ref ?Is_Count_Down_Complete@DelayTimerClass@@QBE_NXZ=00496b50

bool UnitClass::unknown_slot63(Rect *bounds, bool arg2, bool arg3)
{
    if (unknown_370) {
        TechnoClass *contact = Contact_With_Whom(0);
        if (contact->Kind_Of() == 6 && (contact->Get_Mission() == 16 || contact->MissionQueue == 16)) {
            DelayTimerClass *state = &contact->unknown_2a8;
            if (state->Is_Count_Up_Active() || state->Is_Count_Down_Active() || state->Is_Count_Up_Complete() || state->Is_Count_Down_Complete()) {
                return false;
            }
        }
    }
    return ObjectClass::unknown_slot63(bounds, arg2, false);
}
