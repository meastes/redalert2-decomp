// void ObjectClass::unknown_func_5d4920(int z) (0x005D4920)
#include "shared/object_type.hpp"

void ObjectClass::unknown_func_5d4920(int z)
{
    if (unknown_74) {
        Mark(0);
        Coord.Z = z;
        Mark(1);
    } else {
        Coord.Z = z;
    }
}
