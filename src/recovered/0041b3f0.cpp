// 0x0041b3f0: function not yet identified
//
// Body: 15 bytes (frozen inventory bound).
// Referenced from 5 sites (data).
// Ghidra prototype: BYTE __fastcall FUN_0041b3f0(int *param_1)
//
// Types and names are Ghidra placeholders (see shared/ghidra_types.hpp);
// rename and retype as the purpose becomes known.
//
#include "shared/ghidra_types.hpp"

namespace seed_0041b3f0 {

BYTE __fastcall FUN_0041b3f0(int *aircraft_words)
{
    int type_address;

    type_address = (int)((**(code **)(*aircraft_words + 0x80))());
    return *(BYTE *)(type_address + 0xaab);
}

}  // namespace seed_0041b3f0
