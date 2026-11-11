// void TriggerClass::Reset_Global_Linked_Timed_Events(int value) (0x006ED620)
// Resets every trigger of the chain (from this one) whose type answers
// TriggerTypeClass::Is_Linked_To_Global(value).
#include "shared/object.hpp"

void TriggerClass::Reset_Global_Linked_Timed_Events(int value)
{
    for (TriggerClass *trigger = this; trigger != 0; trigger = trigger->Next) {
        if (trigger->Class->Is_Linked_To_Global(value)) {
            trigger->Reset();
        }
    }
}
