// bool TriggerTypeClass::Is_Linked_To_Global(int value) const (0x006EE240)
// Whether the type has an event of type 0x1B or 0x1C with this data.
#include "shared/object.hpp"

bool TriggerTypeClass::Is_Linked_To_Global(int value) const
{
    bool result = false;
    for (TEventClass *event = FirstEvent; event != 0; event = event->Next) {
        if ((event->Type == 0x1B || event->Type == 0x1C) && event->Data == value) {
            result = true;
            break;
        }
    }
    return result;
}
