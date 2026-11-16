//  (0x0061A2B0)
#include "shared/ghidra_types.hpp"

extern DWORD  unknown_006f6750(void);

DWORD __fastcall unknown_kind_1d_value_0061a2b0(int *object)
{
    int kind;
    DWORD result;

    kind = (int)((**(code **)(*object + 0x440))());
    if (kind == 0x1d) {
        result = (DWORD)(unknown_006f6750());
        return result;
    }
    return 0xffffffff;
}

