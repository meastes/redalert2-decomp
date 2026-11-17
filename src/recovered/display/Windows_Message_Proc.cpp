// Windows_Message_Proc_0049e7f0 (0x0049E7F0)
#include "shared/win32_types.hpp"

extern "C" __declspec(dllimport) unsigned int __stdcall DefWindowProcA(int, int, int, int);

void __stdcall Windows_Message_Proc_0049e7f0(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    DefWindowProcA((int)(window), (int)(message), (int)(wparam), (int)(lparam));
    return;
}

