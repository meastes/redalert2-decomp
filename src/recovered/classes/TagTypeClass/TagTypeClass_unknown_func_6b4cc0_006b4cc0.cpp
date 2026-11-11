// bool TagTypeClass::unknown_func_6b4cc0() const (0x006B4CC0)
// Whether any trigger type answers TriggerTypeClass::Is_Enters_Zone.
#include "shared/object.hpp"

bool TagTypeClass::unknown_func_6b4cc0() const
{
    for (TriggerTypeClass *trigger = Triggers; trigger != 0; trigger = trigger->Next) {
        if (trigger->Is_Enters_Zone()) {
            return true;
        }
    }
    return false;
}
