// void DisplayClass::unknown_slot34() (0x0049E7D0, slot 34)
// Closes the unused window when there is one (TibSun DisplayClass::Reposition_Sidebar
// and its UnusedWindow, win.h).
#include "shared/display.hpp"
#include "shared/win32_api.hpp"
// @ref ?UnusedWindow@@3PAUHWND__@@A=00b263dc

// @ref __imp__CloseWindow@4=0079a45c

WIN32_IMPORT BOOL __stdcall CloseWindow(HWND window);
extern HWND UnusedWindow;   // 0x00B263DC

void DisplayClass::unknown_slot34()
{
    if (UnusedWindow) {
        CloseWindow(UnusedWindow);
        UnusedWindow = 0;
    }
}
