// void TagClass::Mark_To_Delete() (0x006B3CC0)
// Marks the tag and queues it in the abstract list 0x00AC0CD8.
#include "shared/object.hpp"

void TagClass::Mark_To_Delete()
{
    IsToDie = true;
    unknown_objects_ac0cd8.Add(this);
}
