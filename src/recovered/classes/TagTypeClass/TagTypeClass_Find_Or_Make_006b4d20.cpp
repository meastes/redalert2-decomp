// TagTypeClass *__fastcall TagTypeClass::Find_Or_Make(char const *name) (0x006B4D20)
// The named tag type, created when missing; 0 for "<none>" and "none".
#include "shared/object.hpp"

TagTypeClass *__fastcall TagTypeClass::Find_Or_Make(char const *name)
{
    if (!_stricmp("<none>", name) || !_stricmp("none", name)) {
        return 0;
    }
    for (int index = 0; index < TagTypes.Count(); index++) {
        if (!_stricmp(TagTypes[index]->ID, name)) {
            return TagTypes[index];
        }
    }
    return new TagTypeClass(name);
}
