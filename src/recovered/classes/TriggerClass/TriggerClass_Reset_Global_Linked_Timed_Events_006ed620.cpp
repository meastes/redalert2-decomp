// void TriggerClass::Reset_Global_Linked_Timed_Events(int value) (0x006ED620)
// Resets every trigger of the chain (from this one) whose type answers
// TriggerTypeClass::unknown_func_6ee240(value).
#include "shared/object.hpp"

void TriggerClass::Reset_Global_Linked_Timed_Events(int value)
{
    for (TriggerClass *trigger = this; trigger != 0; trigger = trigger->Next) {
        if (trigger->Class->unknown_func_6ee240(value)) {
            trigger->Reset();
        }
    }
}
