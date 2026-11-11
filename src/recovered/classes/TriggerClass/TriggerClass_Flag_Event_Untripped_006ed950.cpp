// void TriggerClass::Flag_Event_Untripped(int index) (0x006ED950)
#include "shared/object.hpp"

void TriggerClass::Flag_Event_Untripped(int index)
{
    IsTripped &= ~(1 << index);
}
