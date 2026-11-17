// 0x005e8710: function not yet identified
//
// Body: 15 bytes (frozen inventory bound).
// Referenced from 4 sites (unconditional call).
// Ghidra prototype: void __fastcall unknown_enum_children_005e8710(HWND param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//
#include "shared/win32_types.hpp"

namespace seed_005e8710 {

extern void __cdecl unknown_enum_proc_005e86d0(void);
extern "C" __declspec(dllimport) unsigned int __stdcall EnumChildWindows(int, int, int);  // @ref __imp__EnumChildWindows@12=0079a4dc

void __fastcall unknown_enum_children_005e8710(HWND window)
{
    EnumChildWindows((int)(window), (int)(unknown_enum_proc_005e86d0), (int)(0));
    return;
}

}  // namespace seed_005e8710
