// 0x005c2940: function not yet identified
//
// Body: 34 bytes (frozen inventory bound).
// Referenced from 1 site (unconditional call).
// Ghidra prototype: void FUN_005c2940(void)
// Callees with accepted source:
//   FUN_00735b10 -> src/recovered/00735b10.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//
#include "shared/win32_types.hpp"

namespace seed_005c2940 {

extern unsigned int  FUN_00735b10(void);
extern "C" __declspec(dllimport) unsigned int __stdcall GetDlgItem(int, int);  // @ref __imp__GetDlgItem@8=0079a49c
extern "C" __declspec(dllimport) unsigned int __stdcall SendMessageA(int, int, int, int);  // @ref __imp__SendMessageA@16=0079a4a0

void  FUN_005c2940(void)
{
    HWND window_handle;

    window_handle = (HWND)FUN_00735b10();
    window_handle = (HWND)(GetDlgItem((int)(window_handle), (int)(0x59f)));
                                        /* WARNING: Subroutine does not return */
    SendMessageA((int)(window_handle), (int)(0x4dc), (int)(0), (int)(0));
}

}  // namespace seed_005c2940
