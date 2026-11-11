// TriggerTypeClass::TriggerTypeClass(char const *ini_name) (0x006EDEB0)
// Enabled on every difficulty, no house, next trigger, events or actions;
// listed in the abstract lists and numbered by its place in TriggerTypes.
// @ref ?unknown_AbstractClass_list_00ac0c98@@3V?$DynamicVectorClass@PAVAbstractClass@@@@A=00ac0c98
#include "shared/object.hpp"
#include "shared/trigger_type.hpp"

TriggerTypeClass::TriggerTypeClass(char const *ini_name) :
    AbstractTypeClass(ini_name),
    ArrayIndex(-1),
    IsEnabledOnEasy(true),
    IsEnabledOnMedium(true),
    IsEnabledOnHard(true),
    IsEnabled(true),
    IsToInherit(false),
    House(0),
    LinkedTo(0),
    FirstEvent(0),
    FirstAction(0)
{
    unknown_AbstractClass_list_00ac0c98.Add(this);
    unknown_objects_ac0cb0.Add(this);
    ArrayIndex = TriggerTypes.Count();
    TriggerTypes.Add(this);
}
