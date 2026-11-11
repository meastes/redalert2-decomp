// bool TagTypeClass::Is_Allow_Win() const (0x006B4C30)
// Whether any trigger type answers TriggerTypeClass::Is_Allow_Win.
#include "shared/object.hpp"

bool TagTypeClass::Is_Allow_Win() const
{
    for (TriggerTypeClass *trigger = FirstTrigger; trigger != 0; trigger = trigger->LinkedTo) {
        if (trigger->Is_Allow_Win()) {
            return true;
        }
    }
    return false;
}
