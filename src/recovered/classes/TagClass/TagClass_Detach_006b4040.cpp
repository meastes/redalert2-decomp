// void TagClass::Detach(AbstractClass *target, bool all) (0x006B4040)
// A tag that loses its type queues itself for removal.
#include "shared/object.hpp"

void TagClass::Detach(AbstractClass *target, bool all)
{
    if ((AbstractClass *)Class == target) {
        Class = 0;
    }
    if ((AbstractClass *)Trigger == target) {
        Trigger = Trigger->Next;
    }
    if (Class == 0) {
        Mark_To_Delete();
    }
}
