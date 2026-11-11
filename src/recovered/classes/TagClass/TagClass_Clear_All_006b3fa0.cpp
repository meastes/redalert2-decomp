// void TagClass::Clear_All() (0x006B3FA0)
// Deletes every tag (each destructor removes it from Tags).
#include "shared/object.hpp"

void TagClass::Clear_All()
{
    while (Tags.Count() > 0) {
        delete Tags[0];
    }
}
