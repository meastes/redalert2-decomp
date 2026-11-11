// bool TriggerClass::Is_Allow_Win() const (0x006ED590)
// TriggerTypeClass::unknown_func_6ee210 of this trigger's type or the next one's.
#include "shared/object.hpp"

bool TriggerClass::Is_Allow_Win() const
{
    if (Next != 0) {
        return Class->unknown_func_6ee210() || Next->Class->unknown_func_6ee210();
    }
    return Class->unknown_func_6ee210();
}
