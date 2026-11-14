// 0x005b69e0: function not yet identified
//
// Body: 14 bytes (frozen inventory bound).
// Referenced from 1 site (unconditional call).
// Ghidra prototype: void __fastcall MSSfx_Destructor_005b69e0(int *param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//
#include "shared/win32_types.hpp"

namespace seed_005b69e0 {

extern void  free_007825e8(LPVOID allocation_address);

void __fastcall MSSfx_Destructor_005b69e0(int *object_words)
{
    if (*object_words != 0) {
        free_007825e8((LPVOID)(*object_words));
    }
    return;
}

}  // namespace seed_005b69e0
