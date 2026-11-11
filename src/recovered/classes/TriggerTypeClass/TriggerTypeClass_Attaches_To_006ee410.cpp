// int TriggerTypeClass::Attaches_To() const (0x006EE410)
// The combined flags of the events and actions of this and the later types.
#include "shared/object.hpp"

int TriggerTypeClass::Attaches_To() const
{
    int flags = 0;
    for (TEventClass *event = FirstEvent; event != 0; event = event->Next) {
        flags |= TEventClass::Event_Flags(event->Type);
    }
    for (TActionClass *action = FirstAction; action != 0; action = action->Next) {
        flags |= TActionClass::Action_Flags(action->Type);
    }
    if (LinkedTo != 0) {
        return LinkedTo->Attaches_To() | flags;
    }
    return flags;
}
