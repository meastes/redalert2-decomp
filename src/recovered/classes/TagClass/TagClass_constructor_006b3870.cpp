// TagClass::TagClass(TagTypeClass *type) (0x006B3870)
// A live tag with one trigger per trigger type of its type.
#include "shared/object.hpp"

// The abstract list 0x00AC0D48 (element type AbstractClass * by RTTI).
extern DynamicVectorClass<AbstractClass *> unknown_AbstractClass_list_00ac0d48;

TagClass::TagClass(TagTypeClass *type) :
    Class(type),
    Trigger(0),
    AttachCount(0),
    CellID(unknown_geometry_k),
    IsToDie(false),
    IsCurrentlySprung(false)
{
    Tags.Add(this);
    unknown_objects_ac0cb0.Add((ObjectClass *)this);
    unknown_AbstractClass_list_00ac0d48.Add(this);
    if (type != 0) {
        for (TriggerTypeClass *trigger = type->FirstTrigger; trigger != 0; trigger = trigger->LinkedTo) {
            TriggerClass *made = new TriggerClass(trigger);
            made->LinkedTo = Trigger;
            Trigger = made;
        }
    }
}
