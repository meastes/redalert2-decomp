// 0x004015c0: function not yet identified
//
// Body: 55 bytes (frozen inventory bound).
// Referenced from 1 site (unconditional call).
// Ghidra prototype: uint __fastcall FindSampleIndex_004015c0(int *param_1,undefined4 param_2)
//
// Types and names are Ghidra placeholders (see shared/ghidra_types.hpp);
// rename and retype as the purpose becomes known.
//
#include "shared/ghidra_types.hpp"

namespace seed_004015c0 {

extern void __cdecl stricmp_00781f20(void);
extern undefined4  FUN_00782025(undefined4 key, uint entries, uint entry_count, int entry_width, code *comparator);

uint __fastcall FindSampleIndex_004015c0(int *bag_words, undefined4 name_address)
{
    int entry_address;

    entry_address = (int)(FUN_00782025((undefined4)(name_address), (uint)(*bag_words), (uint)(bag_words[1]), (int)(0x24), (code *)(stricmp_00781f20)));
    if (entry_address != 0) {
        return (uint)(entry_address - *bag_words) / 0x24;
    }
    return 0xffffffff;
}

}  // namespace seed_004015c0
