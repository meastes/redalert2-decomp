// bool ObjectClass::Is_Inactive() const (0x005D4F10)
#include "shared/object_type.hpp"

bool ObjectClass::Is_Inactive() const
{
    return !unknown_7d;
}
