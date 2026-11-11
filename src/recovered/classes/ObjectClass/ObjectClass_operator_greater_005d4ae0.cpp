// bool ObjectClass::operator>(ObjectClass const &other) const (0x005D4AE0)
#include "shared/object_type.hpp"

bool ObjectClass::operator>(ObjectClass const &other) const
{
    return Sort_Y() > other.Sort_Y() ? true : false;
}
