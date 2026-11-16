//  (0x006D8660)
#include "shared/ghidra_types.hpp"

undefined4 __fastcall unknown_has_slot_398_006d8660(int *self)
{
    int *result;

    result = (int *)(**(code **)(*self + 0x398))();
    if ((result != (int *)0x0) && (*result != 0)) {
        return 1;
    }
    return 0;
}

