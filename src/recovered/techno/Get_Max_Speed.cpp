//  (0x006D8E90)
#include "shared/ghidra_types.hpp"

unsigned int __fastcall Get_Max_Speed_006d8e90(int *self)
{
    int type;

    type = (int)((**(code **)(*self + 0x80))());
    if (type != 0) {
        return *(unsigned int *)(type + 0x5a0);
    }
    return 0;
}

