// 0x007856a2: function not yet identified
//
// Body: 11 bytes (frozen inventory bound).
// Referenced from 1 site (unconditional call).
// Ghidra prototype: void unknown_007856a2(undefined4 string)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//
#include "shared/win32_types.hpp"

namespace seed_007856a2 {

extern unsigned int  unknown_00785678(LPCSTR string);

void  unknown_007856a2(unsigned int string)
{
    unknown_00785678((LPCSTR)(string));
    return;
}

}  // namespace seed_007856a2
