// bool TagTypeClass::unknown_func_6b4c90() const (0x006B4C90)
// Whether any trigger type answers TriggerTypeClass::Is_Cross_Vertical.
#include "shared/object.hpp"

bool TagTypeClass::unknown_func_6b4c90() const
{
    for (TriggerTypeClass *trigger = Triggers; trigger != 0; trigger = trigger->Next) {
        if (trigger->Is_Cross_Vertical()) {
            return true;
        }
    }
    return false;
}
