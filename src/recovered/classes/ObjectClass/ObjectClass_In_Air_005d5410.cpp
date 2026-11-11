// bool ObjectClass::In_Air() const (0x005D5410)
#include "shared/object_type.hpp"

bool ObjectClass::In_Air() const
{
    if (unknown_74 && Get_Height_AGL() >= unknown_geometry_g * 2) {
        return true;
    }
    return false;
}
