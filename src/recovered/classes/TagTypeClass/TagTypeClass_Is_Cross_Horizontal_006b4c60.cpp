// bool TagTypeClass::Is_Cross_Horizontal() const (0x006B4C60)
// Whether any trigger type answers TriggerTypeClass::Is_Cross_Horizontal.
#include "shared/object.hpp"

bool TagTypeClass::Is_Cross_Horizontal() const
{
    for (TriggerTypeClass *trigger = FirstTrigger; trigger != 0; trigger = trigger->Next) {
        if (trigger->Is_Cross_Horizontal()) {
            return true;
        }
    }
    return false;
}
