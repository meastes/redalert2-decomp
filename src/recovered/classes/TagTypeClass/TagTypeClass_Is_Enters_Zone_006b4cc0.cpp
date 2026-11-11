// bool TagTypeClass::Is_Enters_Zone() const (0x006B4CC0)
// Whether any trigger type answers TriggerTypeClass::Is_Enters_Zone.
#include "shared/object.hpp"

bool TagTypeClass::Is_Enters_Zone() const
{
    for (TriggerTypeClass *trigger = FirstTrigger; trigger != 0; trigger = trigger->Next) {
        if (trigger->Is_Enters_Zone()) {
            return true;
        }
    }
    return false;
}
