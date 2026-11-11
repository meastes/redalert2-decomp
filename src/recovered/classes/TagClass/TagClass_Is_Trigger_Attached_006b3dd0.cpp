// bool TagClass::Is_Trigger_Attached(TriggerClass *trigger) const (0x006B3DD0)
#include "shared/object.hpp"

bool TagClass::Is_Trigger_Attached(TriggerClass *trigger) const
{
    for (TriggerClass *attached = Trigger; attached != 0; attached = attached->LinkedTo) {
        if (attached == trigger) {
            return true;
        }
    }
    return false;
}
