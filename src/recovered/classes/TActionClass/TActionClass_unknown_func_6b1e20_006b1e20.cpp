// bool TActionClass::unknown_func_6b1e20(...) (0x006B1E20)
// Deletes every trigger whose type is the one at +0x50 (the count is read
// once; the index steps back after each deletion); always true.
#include "shared/taction.hpp"
#include "shared/trigger.hpp"

bool TActionClass::unknown_func_6b1e20(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    if (unknown_50 != 0) {
        int count = Triggers.Count();
        for (int index = 0; index < count; index++) {
            if (Triggers[index]->Class == (TriggerTypeClass *)unknown_50) {
                delete Triggers[index];
                index--;
            }
        }
    }
    return true;
}


