// bool TagTypeClass::Link(TriggerTypeClass *trigger) (0x006B47E0)
#include "shared/object.hpp"

bool TagTypeClass::Link(TriggerTypeClass *trigger)
{
    if (trigger != 0) {
        trigger->LinkedTo = FirstTrigger;
        FirstTrigger = trigger;
        return true;
    }
    return false;
}
