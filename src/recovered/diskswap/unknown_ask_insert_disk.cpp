// bool __stdcall unknown_ask_insert_disk(int disk) -- 0x00498710
// Purpose: asks the player with an OK/Cancel message box ("Hey!",
// "Please insert disk %d") to insert a disk; true when OK was chosen.
// No direct callers. Owner unit not identified (CD handling).
#include "shared/win32_api.hpp"

extern "C" int __cdecl sprintf(char *buffer, char const *format, ...);
WIN32_IMPORT int __stdcall MessageBoxA(HWND window, char const *text, char const *caption, unsigned int type);

bool __stdcall DiskSwap_Swap(int disk)
{
    char text[256];
    sprintf(text, "Please insert disk %d", disk);
    return MessageBoxA(0, text, "Hey!", 1) == 1 ? true : false;    // MB_OKCANCEL, IDOK
}
