// 0x005e86c0: function not yet identified
//
// Body: 15 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall unknown_enum_children_005e86c0(HWND param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//
#include "shared/win32_types.hpp"

namespace seed_005e86c0 {

extern void __cdecl unknown_enum_proc_005e8130(void);
extern "C" __declspec(dllimport) unsigned int __stdcall EnumChildWindows(int, int, int);  // @ref __imp__EnumChildWindows@12=0079a4dc

void __fastcall unknown_enum_children_005e86c0(HWND window)
{
    EnumChildWindows((int)(window), (int)(unknown_enum_proc_005e8130), (int)(0));
    return;
}

}  // namespace seed_005e86c0
