// TriggerClass::TriggerClass(TriggerTypeClass *type) (0x006ED220)
// A live trigger; disabled when its type is, or for the current difficulty.
#include "shared/object.hpp"

// The abstract list 0x00AC0D48 (element type AbstractClass * by RTTI).
extern DynamicVectorClass<AbstractClass *> unknown_AbstractClass_list_00ac0d48;

TriggerClass::TriggerClass(TriggerTypeClass *type) :
    Class(type),
    LinkedTo(0),
    IsToDelete(false),
    IsTripped(0),
    IsActive(true)
{
    unknown_objects_ac0cb0.Add((ObjectClass *)this);
    unknown_AbstractClass_list_00ac0d48.Add(this);
    Triggers.Add(this);
    if (Class != 0) {
        Reset_All_Timed_Events();
        if (!Class->IsEnabled || (Scenario_Difficulty() == 0 && !Class->IsEasy) ||
            (Scenario_Difficulty() == 1 && !Class->IsNormal) || (Scenario_Difficulty() == 2 && !Class->IsHard)) {
            IsActive = false;
        }
    }
}
