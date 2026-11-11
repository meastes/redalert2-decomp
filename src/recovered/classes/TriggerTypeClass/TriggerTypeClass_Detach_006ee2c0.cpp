// void TriggerTypeClass::Detach(AbstractClass *target, bool all) (0x006EE2C0)
// Passes the detach on to the actions and events, then unlinks target.
#include "shared/object.hpp"

void TriggerTypeClass::Detach(AbstractClass *target, bool all)
{
    if (LinkedTo != 0 && (AbstractClass *)LinkedTo == target) {
        LinkedTo = LinkedTo->LinkedTo;
    }
    for (TActionClass *action = FirstAction; action != 0; action = action->Next) {
        action->Detach(target, true);
    }
    if ((AbstractClass *)FirstAction == target) {
        FirstAction = FirstAction->Next;
    }
    for (TEventClass *event = FirstEvent; event != 0; event = event->Next) {
        event->Detach(target, true);
    }
    if ((AbstractClass *)FirstEvent == target) {
        FirstEvent = FirstEvent->Next;
    }
}
