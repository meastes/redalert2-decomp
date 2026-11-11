// bool TriggerClass::Is_Allow_Win() const (0x006ED590)
// TriggerTypeClass::Is_Allow_Win of this trigger's type or the next one's.
#include "shared/object.hpp"

bool TriggerClass::Is_Allow_Win() const
{
    if (Next != 0) {
        return Class->Is_Allow_Win() || Next->Class->Is_Allow_Win();
    }
    return Class->Is_Allow_Win();
}
