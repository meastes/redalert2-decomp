// 0x006d8660: function not yet identified
//
// Body: 26 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 __fastcall unknown_has_slot_398_006d8660(int *param_1)
//
// Types and names are Ghidra placeholders (see shared/ghidra_types.hpp);
// rename and retype as the purpose becomes known.
//
// seed-generated: tools/original_target/regenerate_seeds.py may rewrite this file;
// delete this line when editing it by hand.
#include "shared/ghidra_types.hpp"

namespace seed_006d8660 {

undefined4 __fastcall unknown_has_slot_398_006d8660(int *self)
{
    int *result;

    result = (int *)(**(code **)(*self + 0x398))();
    if ((result != (int *)0x0) && (*result != 0)) {
        return 1;
    }
    return 0;
}

}  // namespace seed_006d8660
