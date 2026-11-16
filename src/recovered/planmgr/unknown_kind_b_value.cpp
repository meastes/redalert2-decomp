//  (0x0061A290)
#include "shared/ghidra_types.hpp"

extern DWORD  unknown_006f6760(void);

DWORD __fastcall unknown_kind_b_value_0061a290(int *object)
{
    int kind;
    DWORD result;

    kind = (int)((**(code **)(*object + 0x16c))());
    if (kind == 0xb) {
        result = (DWORD)(unknown_006f6760());
        return result;
    }
    return 0xffffffff;
}

