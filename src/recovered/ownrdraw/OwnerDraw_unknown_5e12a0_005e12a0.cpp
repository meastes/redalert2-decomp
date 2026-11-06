// @ref __imp__GetDlgCtrlID@4=0079a3f4
#include "shared/win32_api.hpp"
#include "shared/owner_draw_dialog_control.hpp"

namespace OwnerDraw {

int __fastcall unknown_5e12a0(unsigned int key, HWND window)
{
    UnknownControlRecord *record = 0;
    unknown_control_table_a73640.Get_Pointer(key, &record);
    int kind = record != 0 ? record->unknown_6c : 0;
    int id = GetDlgCtrlID(window);
    if (Is_Listed_Kind_5e8590(kind) && id == 0x71c) {
        return 0x25;
    }
    if (((((kind == 0xbc) || (kind == 0xbd)) || (kind == 0xc2)) ||
            ((kind == 0xc9 || (kind == 0x102)))) && ((id == 0x6ec || (id == 0x5a8)))) {
        return 0xe;
    }
    return 0;
}

}
