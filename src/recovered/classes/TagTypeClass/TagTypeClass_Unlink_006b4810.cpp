// bool TagTypeClass::Unlink(TriggerTypeClass *trigger) (0x006B4810)
#include "shared/object.hpp"

bool TagTypeClass::Unlink(TriggerTypeClass *trigger)
{
    TriggerTypeClass *current = FirstTrigger;
    if (current == trigger) {
        FirstTrigger = trigger->Next;
        return true;
    }
    while (current != 0) {
        if (current->Next == trigger) {
            current->Next = trigger->Next;
            return true;
        }
        current = current->Next;
    }
    return false;
}
