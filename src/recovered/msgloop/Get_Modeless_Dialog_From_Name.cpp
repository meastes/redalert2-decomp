// HWND __fastcall Get_Modeless_Dialog_From_Name(char const *name) -- 0x005B60B0 (Westwood msgloop.cpp)
#include "shared/msgloop.hpp"
#include "shared/crt_string.hpp"

WIN32_IMPORT int WINAPI GetWindowTextA(HWND window, char *text, int length);

HWND __fastcall Get_Modeless_Dialog_From_Name(char const *name)
{
    static char _wname[100];

    for (int i = 0; i < _ModelessDialogs.Count(); i++) {
        GetWindowTextA(_ModelessDialogs[i], _wname, sizeof(_wname) - 1);
        if (!strcmp(_wname, name)) {
            return _ModelessDialogs[i];
        }
    }
    return 0;
}
