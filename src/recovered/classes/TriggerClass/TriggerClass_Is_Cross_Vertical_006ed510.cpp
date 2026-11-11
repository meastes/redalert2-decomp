// bool TriggerClass::Is_Cross_Vertical() const (0x006ED510)
// TriggerTypeClass::Is_Cross_Vertical of this trigger's type or of any later one.
#include "shared/object.hpp"

bool TriggerClass::Is_Cross_Vertical() const
{
    if (Next != 0) {
        return Class->Is_Cross_Vertical() || Next->Is_Cross_Vertical();
    }
    return Class->Is_Cross_Vertical();
}
