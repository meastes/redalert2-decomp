// bool TagClass::Is_One_Of_A_Kind() const (0x006B4280)
// True when no other tag has the same type.
#include "shared/object.hpp"

bool TagClass::Is_One_Of_A_Kind() const
{
    for (int index = 0; index < Tags.Count(); index++) {
        TagClass *tag = Tags[index];
        if (tag != this && tag->Class == Class) {
            return false;
        }
    }
    return true;
}
