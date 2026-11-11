// void TriggerClass::Detach(AbstractClass *target, bool all) (0x006ED900)
#include "shared/object.hpp"

void TriggerClass::Detach(AbstractClass *target, bool all)
{
    if ((AbstractClass *)Class == target) {
        Class = 0;
    }
    if ((AbstractClass *)LinkedTo == target) {
        LinkedTo = LinkedTo->LinkedTo;
    }
}
