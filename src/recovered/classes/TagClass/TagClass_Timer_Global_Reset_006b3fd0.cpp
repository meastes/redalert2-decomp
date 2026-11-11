// void TagClass::Timer_Global_Reset(int value) (0x006B3FD0)
#include "shared/object.hpp"

void TagClass::Timer_Global_Reset(int value)
{
    Trigger->Reset_Global_Linked_Timed_Events(value);
}
