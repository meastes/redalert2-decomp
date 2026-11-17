#define VECTOR_IMPLEMENT_DYNAMIC_ADD
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#include "shared/unknown_description_list.hpp"
// Trial: leading 0x50 bytes match the existing CCINIClass size;
// its empty implicit derived destructor ends at the observed INIClass destructor.
// Composition is a layout hypothesis, not a recovered owning class name.

bool CoopCampListClass::Get_Record(int index, CoopCampEntry **record)
{
    if (index >= 0 && index < Count) {
        *record = &Entries[index];
        return true;
    }
    *record = 0;
    return false;
}
