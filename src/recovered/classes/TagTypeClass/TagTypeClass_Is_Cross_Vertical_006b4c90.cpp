// bool TagTypeClass::Is_Cross_Vertical() const (0x006B4C90)
// Whether any trigger type answers TriggerTypeClass::Is_Cross_Vertical.
#include "shared/object.hpp"

bool TagTypeClass::Is_Cross_Vertical() const
{
    for (TriggerTypeClass *trigger = FirstTrigger; trigger != 0; trigger = trigger->Next) {
        if (trigger->Is_Cross_Vertical()) {
            return true;
        }
    }
    return false;
}
