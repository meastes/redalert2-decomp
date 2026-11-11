// bool TActionClass::unknown_func_6b1ec0(house, object, trigger, cell) (0x006B1EC0)
// Springs (forced, no object) every trigger whose type is the one at +0x50; always true.
#include "shared/taction.hpp"
#include "shared/trigger.hpp"
// @ref ?Fire_Actions@TriggerClass@@QAE_NPAVObjectClass@@VCell@@@Z=006ed830
// @ref ?Triggers@@3V?$DynamicVectorClass@PAVTriggerClass@@@@A=00a40aa0
// @ref _unknown_geometry_k=00abfdb0

bool TActionClass::unknown_func_6b1ec0(HouseClass *house, ObjectClass *object, TriggerClass *trigger, Cell *cell)
{
    if (unknown_50 != 0) {
        for (int index = 0; index < Triggers.Count(); index++) {
            if ((AbstractClass *)Triggers[index]->Class == unknown_50) {
                Triggers[index]->Fire_Actions(0, unknown_geometry_k);
            }
        }
    }
    return true;
}


