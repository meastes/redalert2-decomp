// bool TriggerClass::Is_Cross_Vertical() const (0x006ED510)
// TriggerTypeClass::unknown_func_6ee180 of this trigger's type or of any later one.
#include "shared/object.hpp"

bool TriggerClass::Is_Cross_Vertical() const
{
    if (Next != 0) {
        return Class->unknown_func_6ee180() || Next->Is_Cross_Vertical();
    }
    return Class->unknown_func_6ee180();
}
