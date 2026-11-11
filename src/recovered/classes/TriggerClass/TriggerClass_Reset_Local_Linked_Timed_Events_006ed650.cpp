// void TriggerClass::Reset_Local_Linked_Timed_Events(int value) (0x006ED650)
// Resets every trigger of the chain (from this one) whose type answers
// TriggerTypeClass::Is_Linked_To_Local(value).
#include "shared/object.hpp"

void TriggerClass::Reset_Local_Linked_Timed_Events(int value)
{
    for (TriggerClass *trigger = this; trigger != 0; trigger = trigger->LinkedTo) {
        if (trigger->Class->Is_Linked_To_Local(value)) {
            trigger->Reset_All_Timed_Events();
        }
    }
}
