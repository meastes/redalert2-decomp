// End_0046cb80 (0x0046CB80)
#include "shared/ghidra_types.hpp"

void __fastcall End_0046cb80(int *pipe)
{
                                        /* WARNING: Could not recover jumptable at 0x0046cb82. Too many branches */
                                        /* WARNING: Treating indirect jump as call */
    (**(code **)(*pipe + 4))();
    return;
}

