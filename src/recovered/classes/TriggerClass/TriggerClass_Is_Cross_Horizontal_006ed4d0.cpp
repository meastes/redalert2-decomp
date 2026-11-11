// bool TriggerClass::Is_Cross_Horizontal() const (0x006ED4D0)
// TriggerTypeClass::unknown_func_6ee1b0 of this trigger's type or of any later one.
#include "shared/object.hpp"

bool TriggerClass::Is_Cross_Horizontal() const
{
    if (Next != 0) {
        return Class->unknown_func_6ee1b0() || Next->Is_Cross_Horizontal();
    }
    return Class->unknown_func_6ee1b0();
}
