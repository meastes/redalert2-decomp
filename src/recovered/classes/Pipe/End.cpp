// 0x0046cb80: function not yet identified
//
// Owner: Pipe, virtual slot 2 (vtable +0x8) (from RTTI/vtables).
// Body: 5 bytes (frozen inventory bound).
// Referenced from 8 sites (data).
// Ghidra prototype: void __fastcall End_0046cb80(int *param_1)
//
// Types and names are Ghidra placeholders (see shared/ghidra_types.hpp);
// rename and retype as the purpose becomes known.
//
#include "shared/ghidra_types.hpp"

namespace seed_0046cb80 {

void __fastcall End_0046cb80(int *pipe)
{
                                        /* WARNING: Could not recover jumptable at 0x0046cb82. Too many branches */
                                        /* WARNING: Treating indirect jump as call */
    (**(code **)(*pipe + 4))();
    return;
}

}  // namespace seed_0046cb80
