// TriggerClass *__fastcall TriggerClass::Find_Or_Make(TriggerTypeClass *type) (0x006ED8A0)
// The live trigger of a type, created when missing; 0 without a type.
#include "shared/object.hpp"

TriggerClass *__fastcall TriggerClass::Find_Or_Make(TriggerTypeClass *type)
{
    if (type != 0) {
        for (int index = 0; index < Triggers.Count(); index++) {
            if (type == Triggers[index]->Class) {
                return Triggers[index];
            }
        }
        return new TriggerClass(type);
    }
    return 0;
}
