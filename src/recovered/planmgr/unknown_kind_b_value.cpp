//  (0x0061A290)
#include "shared/ghidra_types.hpp"

extern undefined4  unknown_006f6760(void);

undefined4 __fastcall unknown_kind_b_value_0061a290(int *object)
{
    int kind;
    undefined4 result;

    kind = (int)((**(code **)(*object + 0x16c))());
    if (kind == 0xb) {
        result = (undefined4)(unknown_006f6760());
        return result;
    }
    return 0xffffffff;
}

