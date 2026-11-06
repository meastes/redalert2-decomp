#include "shared/win32_api.hpp"
#include "shared/owner_draw_dialog_control.hpp"

namespace OwnerDraw {

int __fastcall unknown_5ea900(HWND control)
{
    UnknownControlRecord *record = 0;
    unknown_control_table_a73640.Get_Pointer((unsigned int)control, &record);
    return record ? record->unknown_e4 : 0;
}

}
