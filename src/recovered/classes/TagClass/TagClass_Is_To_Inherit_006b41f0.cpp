// bool TagClass::Is_To_Inherit() const (0x006B41F0)
// Whether any attached trigger's type has +0xA0 set.
#include "shared/object.hpp"

bool TagClass::Is_To_Inherit() const
{
    for (TriggerClass *trigger = Trigger; trigger != 0; trigger = trigger->Next) {
        if (trigger->Class->unknown_a0) {
            return true;
        }
    }
    return false;
}
