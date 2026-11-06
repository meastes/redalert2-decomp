// @ref __imp__GetDlgCtrlID@4=0079a3f4
#include "shared/win32_api.hpp"
#include "shared/owner_draw_dialog_control.hpp"

namespace OwnerDraw {

int __fastcall unknown_5e1a20(unsigned int key, HWND window)
{
    UnknownControlRecord *record = 0;
    unknown_control_table_a73640.Get_Pointer(key, &record);
    int kind = record != 0 ? record->unknown_6c : 0;
    int id = GetDlgCtrlID(window);
    return 0;
}

}
