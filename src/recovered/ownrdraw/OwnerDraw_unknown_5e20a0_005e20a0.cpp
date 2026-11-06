#include "shared/win32_api.hpp"
#include "shared/owner_draw_dialog_control.hpp"

namespace OwnerDraw {

int __fastcall unknown_5e20a0(HWND control)
{
    UnknownControlRecord *record = 0;
    unknown_control_table_a73640.Get_Pointer((unsigned int)control, &record);
    if (record) {
        return record->unknown_c0;
    }
    return -1;
}

}
