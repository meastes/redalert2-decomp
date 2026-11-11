// bool TagTypeClass::Is_Linked(TriggerTypeClass *trigger) const (0x006B4CF0)
#include "shared/object.hpp"

bool TagTypeClass::Is_Linked(TriggerTypeClass *trigger) const
{
    for (TriggerTypeClass *attached = FirstTrigger; attached != 0; attached = attached->LinkedTo) {
        if (attached == trigger) {
            return true;
        }
    }
    return false;
}
