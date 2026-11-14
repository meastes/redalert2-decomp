// 0x00736880: function not yet identified
//
// Body: 28 bytes (frozen inventory bound).
// Referenced from 1 site (unconditional call).
// Ghidra prototype: void __fastcall On_WM_HELP_00736880(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//
#include "shared/win32_types.hpp"

namespace seed_00736880 {

extern "C" __declspec(dllimport) unsigned int __stdcall GetWindowContextHelpId(int);  // @ref __imp__GetWindowContextHelpId@4=0079a428
extern "C" __declspec(dllimport) unsigned int __stdcall WinHelpA(int, int, int, int);  // @ref __imp__WinHelpA@16=0079a42c
extern "C" char s_SUN_HLP_007fd094[];

void __fastcall On_WM_HELP_00736880(int object_address)
{
    HWND hWndMain;
    DWORD dwData;

    hWndMain = (HWND)(*(HWND *)(object_address + 0xc));
    dwData = (DWORD)(GetWindowContextHelpId((int)(hWndMain)));
    WinHelpA((int)(hWndMain), (int)(s_SUN_HLP_007fd094), (int)(8), (int)(dwData));
    return;
}

}  // namespace seed_00736880
