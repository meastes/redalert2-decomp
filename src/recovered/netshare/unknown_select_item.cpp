// 0x005c3480: function not yet identified
//
// Body: 38 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall unknown_select_item_005c3480(HWND param_1,int param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//
#include "shared/win32_types.hpp"

namespace seed_005c3480 {

extern "C" __declspec(dllimport) unsigned int __stdcall GetDlgItem(int, int);  // @ref __imp__GetDlgItem@8=0079a49c
extern "C" __declspec(dllimport) unsigned int __stdcall SendMessageA(int, int, int, int);  // @ref __imp__SendMessageA@16=0079a4a0
extern "C" unsigned int unknown_item_data_00a3d8dc;

void __fastcall unknown_select_item_005c3480(HWND window, int index)
{
    LPARAM lParam;
    HWND hWnd;

    lParam = (LPARAM)(*(LPARAM *)(unknown_item_data_00a3d8dc + index * 4));
    hWnd = (HWND)(GetDlgItem((int)(window), (int)(0x5a8)));
                                        /* WARNING: Subroutine does not return */
    SendMessageA((int)(hWnd), (int)(0x4b2), (int)(0), (int)(lParam));
}

}  // namespace seed_005c3480
