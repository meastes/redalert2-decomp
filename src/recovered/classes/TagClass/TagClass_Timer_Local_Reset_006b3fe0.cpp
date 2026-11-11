// void TagClass::Timer_Local_Reset(int value) (0x006B3FE0)
// Passes value to the attached chain's 0x006ED650 reset (the companion of 0x006B3FD0).
#include "shared/object.hpp"

void TagClass::Timer_Local_Reset(int value)
{
    Trigger->Reset_Local_Linked_Timed_Events(value);
}
