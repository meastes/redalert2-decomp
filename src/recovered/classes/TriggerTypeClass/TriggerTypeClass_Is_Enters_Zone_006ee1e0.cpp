// bool TriggerTypeClass::Is_Enters_Zone() const (0x006EE1E0)
// Whether the type has an Event of type 0x18.
#include "shared/object.hpp"

bool TriggerTypeClass::Is_Enters_Zone() const
{
    bool result = false;
    for (TEventClass *item = Events; item != 0; item = item->Next) {
        if (item->Type == 0x18) {
            result = true;
            break;
        }
    }
    return result;
}
