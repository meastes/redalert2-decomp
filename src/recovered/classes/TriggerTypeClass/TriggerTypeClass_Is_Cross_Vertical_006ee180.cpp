// bool TriggerTypeClass::Is_Cross_Vertical() const (0x006EE180)
// Whether the type has an Event of type 0x1A.
#include "shared/object.hpp"

bool TriggerTypeClass::Is_Cross_Vertical() const
{
    bool result = false;
    for (TEventClass *item = FirstEvent; item != 0; item = item->Next) {
        if (item->Type == 0x1A) {
            result = true;
            break;
        }
    }
    return result;
}
