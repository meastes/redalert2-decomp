// 0x0075ca00: function not yet identified
//
// Body: 34 bytes (frozen inventory bound).
// Referenced from 1 site (unconditional call).
// Ghidra prototype: void unknown_send_4dc_0075ca00(void)
// Callees with accepted source:
//   WS_Top_Window_00735b10 -> src/recovered/00735b10.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//
#include "shared/win32_types.hpp"

namespace seed_0075ca00 {

extern unsigned int  WS_Top_Window_00735b10(void);
extern "C" __declspec(dllimport) unsigned int __stdcall GetDlgItem(int, int);  // @ref __imp__GetDlgItem@8=0079a49c
extern "C" __declspec(dllimport) unsigned int __stdcall SendMessageA(int, int, int, int);  // @ref __imp__SendMessageA@16=0079a4a0

void  unknown_send_4dc_0075ca00(void)
{
    HWND window;

    window = (HWND)WS_Top_Window_00735b10();
    window = (HWND)(GetDlgItem((int)(window), (int)(0x59f)));
                                        /* WARNING: Subroutine does not return */
    SendMessageA((int)(window), (int)(0x4dc), (int)(0), (int)(0));
}

}  // namespace seed_0075ca00
