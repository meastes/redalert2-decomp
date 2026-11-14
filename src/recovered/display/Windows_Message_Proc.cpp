// 0x0049e7f0: function not yet identified
//
// Body: 29 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void Windows_Message_Proc_0049e7f0(HWND param_1,UINT param_2,WPARAM param_3,LPARAM param_4)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//
#include "shared/win32_types.hpp"

namespace seed_0049e7f0 {

extern "C" __declspec(dllimport) unsigned int __stdcall DefWindowProcA(int, int, int, int);  // @ref __imp__DefWindowProcA@16=0079a388

void __stdcall Windows_Message_Proc_0049e7f0(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    DefWindowProcA((int)(window), (int)(message), (int)(wparam), (int)(lparam));
    return;
}

}  // namespace seed_0049e7f0
