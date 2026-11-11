// int TagTypeClass::Attaches_To() const (0x006B4C00)
// The combined flags of the type's trigger types.
#include "shared/object.hpp"

int TagTypeClass::Attaches_To() const
{
    int flags = 0;
    for (TriggerTypeClass *trigger = FirstTrigger; trigger != 0; trigger = trigger->LinkedTo) {
        flags |= trigger->Attaches_To();
    }
    return flags;
}
