// int AnimClass::In_Which_Layer() const (0x004226A0)
// The layer: 2 when attached to an object, else the type's Layer (3 without
// a type).
#include "shared/anim.hpp"

int AnimClass::In_Which_Layer() const
{
    if (unknown_b4 != 0) {
        return 2;
    }
    if (Class != 0) {
        return Class->Layer;
    }
    return 3;
}
