// 0x00781d3b: function not yet identified
//
// Body: 11 bytes (frozen inventory bound).
// Referenced from 3512 sites (unconditional call).
// Ghidra prototype: void operator_delete_00781d3b(undefined4 block)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//
#include "shared/win32_types.hpp"

namespace seed_00781d3b {

extern void  free_007825e8(LPVOID block);

void  operator_delete_00781d3b(unsigned int block)
{
    free_007825e8((LPVOID)(block));
    return;
}

}  // namespace seed_00781d3b
