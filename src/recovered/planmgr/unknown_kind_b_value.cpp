// 0x0061a290: function not yet identified
//
// Body: 22 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 __fastcall unknown_kind_b_value_0061a290(int *param_1)
// Callees with accepted source:
//   unknown_006f6760 -> src/recovered/006f6760.cpp
//
// Types and names are Ghidra placeholders (see shared/ghidra_types.hpp);
// rename and retype as the purpose becomes known.
//
// seed-generated: tools/original_target/regenerate_seeds.py may rewrite this file;
// delete this line when editing it by hand.
#include "shared/ghidra_types.hpp"

namespace seed_0061a290 {

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

}  // namespace seed_0061a290
