// 0x005e7db0: function not yet identified
//
// Body: 30 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 __fastcall unknown_count_children_005e7db0(HWND param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//
#include "shared/win32_types.hpp"

namespace seed_005e7db0 {

extern void __cdecl unknown_count_proc_005e7d00(void);
extern "C" __declspec(dllimport) unsigned int __stdcall EnumChildWindows(int, int, int);  // @ref __imp__EnumChildWindows@12=0079a4dc
extern "C" unsigned int unknown_visible_count_00a737e4;

unsigned int __fastcall unknown_count_children_005e7db0(HWND window)
{
    unknown_visible_count_00a737e4 = 0;
    EnumChildWindows((int)(window), (int)(unknown_count_proc_005e7d00), (int)(0));
    return unknown_visible_count_00a737e4;
}

}  // namespace seed_005e7db0
