// bool TagTypeClass::unknown_func_6b4c30() const (0x006B4C30)
// Whether any trigger type answers TriggerTypeClass::Is_Allow_Win.
#include "shared/object.hpp"

bool TagTypeClass::unknown_func_6b4c30() const
{
    for (TriggerTypeClass *trigger = Triggers; trigger != 0; trigger = trigger->Next) {
        if (trigger->Is_Allow_Win()) {
            return true;
        }
    }
    return false;
}
