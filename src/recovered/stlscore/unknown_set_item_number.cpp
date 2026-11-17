// void __fastcall unknown_set_item_number_0069a780(HWND parent_window, int control_id, unsigned int value)
// (0x0069A780): formats value with L"%d" (swprintf, 0x00783764) and sends the text to the dialog
// control with message 0x4B2.

#include "shared/win32_types.hpp"

extern unsigned short const ladder_number_format_007d1598[];   // 0x007D1598 L"%d" (wonline.cpp)

namespace seed_0069a780 {

extern unsigned int  swprintf_00783764(int, int, int);
extern "C" __declspec(dllimport) unsigned int __stdcall GetDlgItem(int, int);  // @ref __imp__GetDlgItem@8=0079a49c
extern "C" __declspec(dllimport) unsigned int __stdcall SendMessageA(int, int, int, int);  // @ref __imp__SendMessageA@16=0079a4a0

void __fastcall unknown_set_item_number_0069a780(HWND parent_window, int control_id, unsigned int format_argument)
{
    HWND hWnd;
    unsigned char text_buffer [64];

    swprintf_00783764((int)(text_buffer), (int)(ladder_number_format_007d1598), (int)(format_argument));
    hWnd = (HWND)(GetDlgItem((int)(parent_window), (int)(control_id)));
                                        /* WARNING: Subroutine does not return */
    SendMessageA((int)(hWnd), (int)(0x4b2), (int)(0), (int)((LPARAM)text_buffer));
}

}  // namespace seed_0069a780
