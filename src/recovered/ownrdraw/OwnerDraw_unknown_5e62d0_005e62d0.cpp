#include "shared/win32_api.hpp"
#include "shared/owner_draw_dialog_control.hpp"

namespace OwnerDraw {

void __fastcall unknown_5e62d0(HWND control)
{
    UnknownControlRecord *record = 0;
    unknown_control_table_a73640.Get_Pointer((unsigned int)control, &record);
    if (record) {
        record->unknown_bd = true;
    }
}

}
