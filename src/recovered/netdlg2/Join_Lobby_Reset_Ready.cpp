// 0x005c2940: function not yet identified
//
// Body: 34 bytes (frozen inventory bound).
// Referenced from 1 site (unconditional call).
// Ghidra prototype: void Join_Lobby_Reset_Ready(void)
// Callees with accepted source:
//   WS_Top_Window -> src/recovered/00735b10.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//
#include "shared/win32_types.hpp"

namespace seed_005c2940 {

extern unsigned int  WS_Top_Window(void);
extern "C" __declspec(dllimport) unsigned int __stdcall GetDlgItem(int, int);
extern "C" __declspec(dllimport) unsigned int __stdcall SendMessageA(int, int, int, int);

void  Join_Lobby_Reset_Ready(void)
{
    HWND window_handle;

    window_handle = (HWND)WS_Top_Window();
    window_handle = (HWND)(GetDlgItem((int)(window_handle), (int)(0x59f)));
                                        /* WARNING: Subroutine does not return */
    SendMessageA((int)(window_handle), (int)(0x4dc), (int)(0), (int)(0));
}

}  // namespace seed_005c2940
