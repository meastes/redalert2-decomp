// 0x0069a780: function not yet identified
//
// Body: 67 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall FUN_0069a780(HWND param_1,int param_2,undefined4 param_3)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//
#include "shared/win32_types.hpp"

namespace seed_0069a780 {

extern unsigned int  FUN_00783764(int, int, int);
extern "C" __declspec(dllimport) unsigned int __stdcall GetDlgItem(int, int);  // @ref __imp__GetDlgItem@8=0079a49c
extern "C" __declspec(dllimport) unsigned int __stdcall SendMessageA(int, int, int, int);  // @ref __imp__SendMessageA@16=0079a4a0
extern "C" void* PTR_LAB_007d1598;

void __fastcall FUN_0069a780(HWND parent_window, int control_id, unsigned int format_argument)
{
    HWND hWnd;
    unsigned char text_buffer [64];

    FUN_00783764((int)(text_buffer), (int)(&PTR_LAB_007d1598), (int)(format_argument));
    hWnd = (HWND)(GetDlgItem((int)(parent_window), (int)(control_id)));
                                        /* WARNING: Subroutine does not return */
    SendMessageA((int)(hWnd), (int)(0x4b2), (int)(0), (int)((LPARAM)text_buffer));
}

}  // namespace seed_0069a780
