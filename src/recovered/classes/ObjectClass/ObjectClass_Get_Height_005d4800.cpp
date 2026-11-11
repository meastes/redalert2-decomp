// int ObjectClass::Get_Height() const (0x005D4800)
#include "shared/object_type.hpp"

int ObjectClass::Get_Height() const
{
    return Coord.Z;
}
