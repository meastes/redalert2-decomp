//  (0x0061A2B0)
#include "shared/ghidra_types.hpp"

extern undefined4  unknown_006f6750(void);

undefined4 __fastcall unknown_kind_1d_value_0061a2b0(int *object)
{
    int kind;
    undefined4 result;

    kind = (int)((**(code **)(*object + 0x440))());
    if (kind == 0x1d) {
        result = (undefined4)(unknown_006f6750());
        return result;
    }
    return 0xffffffff;
}

