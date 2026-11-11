// bool TriggerTypeClass::Delete_Event(TEventClass *event) (0x006EEC60)
// Unlinks and deletes an event of the type.
#include "shared/object.hpp"

bool TriggerTypeClass::Delete_Event(TEventClass *event)
{
    TEventClass *current = FirstEvent;
    if (current == event) {
        FirstEvent = event->Next;
        delete event;
        return true;
    }
    while (current != 0) {
        if (current->Next == event) {
            current->Next = current->Next->Next;
            delete event;
            return true;
        }
        current = current->Next;
    }
    return false;
}
