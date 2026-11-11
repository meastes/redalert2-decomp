// TagTypeClass *__fastcall TagTypeClass::From_Name(char const *name) (0x006B4880)
// The tag type whose ID or name matches, or 0.
#include "shared/object.hpp"

TagTypeClass *__fastcall TagTypeClass::From_Name(char const *name)
{
    if (name != 0) {
        for (int index = 0; index < TagTypes.Count(); index++) {
            TagTypeClass *type = TagTypes[index];
            if (!_stricmp(type->ID, name) || !_stricmp(type->Name, name)) {
                return type;
            }
        }
    }
    return 0;
}
