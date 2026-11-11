// bool TriggerClass::Is_Event_Tripped(int index) const (0x006ED970)
#include "shared/object.hpp"

bool TriggerClass::Is_Event_Tripped(int index) const
{
    return (IsTripped & (1 << index)) != 0;
}
