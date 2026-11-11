// bool TriggerClass::Is_Enters_Zone() const (0x006ED550)
// TriggerTypeClass::Is_Enters_Zone of this trigger's type or the next one's.
#include "shared/object.hpp"

bool TriggerClass::Is_Enters_Zone() const
{
    if (LinkedTo != 0) {
        return Class->Is_Enters_Zone() || LinkedTo->Class->Is_Enters_Zone();
    }
    return Class->Is_Enters_Zone();
}
