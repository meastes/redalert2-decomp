#include "shared/win32_api.hpp"
#include "shared/owner_draw_dialog_control.hpp"

namespace OwnerDraw {

void __fastcall unknown_5ea490(HWND control)
{
    UnknownControlRecord *record = 0;
    unknown_control_table_a73640.Get_Pointer((unsigned int)control, &record);
    if (record) {
        record->unknown_d6 = false;
    }
}

}
