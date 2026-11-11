// bool ObjectClass::Not_Underground() const (0x005D5490)
#include "shared/object_type.hpp"

bool ObjectClass::Not_Underground() const
{
    return Get_Height_AGL() > -20;
}
