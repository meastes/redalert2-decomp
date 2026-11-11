// int AnimClass::Sort_Y() const (0x00420D70)
// The base value (ObjectClass slot 45, a sort height) plus YSortAdjust.
#include "shared/anim.hpp"

int AnimClass::Sort_Y() const
{
    return ObjectClass::Sort_Y() + YSortAdjust;
}
