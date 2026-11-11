// TagTypeClass::~TagTypeClass() (0x006B46B0)
// Deletes the type's trigger types and every tag of this type.
#include "shared/object.hpp"

TagTypeClass::~TagTypeClass()
{
    Unknown_006ECB90(this, true);
    TriggerTypeClass *trigger = Triggers;
    Triggers = 0;
    while (trigger != 0) {
        TriggerTypeClass *next = trigger->Next;
        trigger->Next = 0;
        delete trigger;
        trigger = next;
    }
    for (int index = 0; index < Tags.Count(); index++) {
        if (Tags[index]->Class == this) {
            delete Tags[index];
            index--;
        }
    }
    unknown_objects_ac0cb0.Delete((ObjectClass *)this);
    unknown_TagTypeClass_list_00abfea0.Delete(this);
}
