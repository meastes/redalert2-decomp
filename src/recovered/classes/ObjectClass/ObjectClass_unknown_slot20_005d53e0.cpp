// bool ObjectClass::unknown_slot20() const (0x005D53E0)
#include "shared/object_type.hpp"

bool ObjectClass::unknown_slot20() const
{
    if (unknown_74 && Get_Height_AGL() < unknown_geometry_g * 2) {
        return true;
    }
    return false;
}
