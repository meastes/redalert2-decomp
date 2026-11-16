//  (0x006D8E90)
#include "shared/ghidra_types.hpp"

undefined4 __fastcall Get_Max_Speed_006d8e90(int *self)
{
    int type;

    type = (int)((**(code **)(*self + 0x80))());
    if (type != 0) {
        return *(undefined4 *)(type + 0x5a0);
    }
    return 0;
}

