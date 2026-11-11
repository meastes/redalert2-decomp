// void TriggerClass::Flag_Event_Tripped(int index) (0x006ED930)
#include "shared/object.hpp"

void TriggerClass::Flag_Event_Tripped(int index)
{
    IsTripped |= 1 << index;
}
