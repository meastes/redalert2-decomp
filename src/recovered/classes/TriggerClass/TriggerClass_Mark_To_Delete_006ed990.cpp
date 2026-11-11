// void TriggerClass::Mark_To_Delete() (0x006ED990)
// Marks the trigger and queues it in the abstract list 0x00AC0CD8.
#include "shared/object.hpp"

void TriggerClass::Mark_To_Delete()
{
    IsToDelete = true;
    unknown_objects_ac0cd8.Add(this);
}
