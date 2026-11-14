// _warhead.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/_warhead.json carries the addresses and the rows that
// stay in their own files.


#include "shared/warhead_type.hpp"
#include "shared/crt_string.hpp"


// int unknown_find_or_make_warhead_0040f2f0(char const *name) (0x0040F2F0)
// Index of the warhead type with this ID in the unit's list 0x00839B00 (-1 for
// "<none>" and "none"); an unknown ID makes a new type and returns its index.
// No direct callers.

int __fastcall unknown_find_or_make_warhead_0040f2f0(char const *name)
{
    if (_stricmp(name, "<none>") == 0 || _stricmp(name, "none") == 0) {
        return -1;
    }
    for (int index = 0; index < unknown_WarheadTypeClass_list_00839b00.Count(); index++) {
        if (_stricmp(name, unknown_WarheadTypeClass_list_00839b00[index]->ID) == 0) {
            return index;
        }
    }
    return unknown_WarheadTypeClass_list_00839b00.ID(new WarheadTypeClass(name));
}


// char const *unknown_warhead_name_0040f3a0(int index) (0x0040F3A0)
// ID of the warhead type at this index of the unit's list 0x00839B00, or null.
// No direct callers.

char const *__fastcall unknown_warhead_name_0040f3a0(int index)
{
    if (index != -1 && (unsigned)index < (unsigned)unknown_WarheadTypeClass_list_00839b00.Count()) {
        return unknown_WarheadTypeClass_list_00839b00[index]->ID;
    }
    return 0;
}
