// bool TagTypeClass::Unlink(TriggerTypeClass *trigger) (0x006B4810)
#include "shared/object.hpp"

bool TagTypeClass::Unlink(TriggerTypeClass *trigger)
{
    TriggerTypeClass *current = FirstTrigger;
    if (current == trigger) {
        FirstTrigger = trigger->LinkedTo;
        return true;
    }
    while (current != 0) {
        if (current->LinkedTo == trigger) {
            current->LinkedTo = trigger->LinkedTo;
            return true;
        }
        current = current->LinkedTo;
    }
    return false;
}
