// void __fastcall Remove_Modeless_Dialog(HWND dialog) -- 0x005B6060 (Westwood msgloop.cpp)
#include "shared/msgloop.hpp"
#include "shared/crt_string.hpp"

void __fastcall Remove_Modeless_Dialog(HWND dialog)
{
    _ModelessDialogs.Delete(dialog);
}
