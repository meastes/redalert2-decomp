// bool TriggerClass::Is_Cross_Horizontal() const (0x006ED4D0)
// TriggerTypeClass::Is_Cross_Horizontal of this trigger's type or of any later one.
#include "shared/object.hpp"

bool TriggerClass::Is_Cross_Horizontal() const
{
    if (LinkedTo != 0) {
        return Class->Is_Cross_Horizontal() || LinkedTo->Is_Cross_Horizontal();
    }
    return Class->Is_Cross_Horizontal();
}
