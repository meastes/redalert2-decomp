// bool TriggerClass::Is_Linked_To_Global(int value) const (0x006ED5D0)
#include "shared/object.hpp"

bool TriggerClass::Is_Linked_To_Global(int value) const
{
    if (Next != 0) {
        return Class->Is_Linked_To_Global(value) || Next->Class->Is_Linked_To_Global(value);
    }
    return Class->Is_Linked_To_Global(value);
}
