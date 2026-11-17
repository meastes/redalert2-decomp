// void __fastcall unknown_close_all_dialogs() -- 0x00753F50
// Purpose: while there is a current dialog (0x00735B10), closes it
// (0x007357E0 with 0).
// Assumptions: the dialog helpers are unidentified; names follow
// 0x0074D780's draft.
#include "shared/win32_api.hpp"
// @ref ?WS_Top_Window@@YIPAUHWND__@@XZ=00735b10
// @ref ?WS_Destroy_Dialog@@YIXPAXH@Z=007357e0

void __fastcall WS_Destroy_Dialog(void *window, int arg2);
HWND __fastcall WS_Top_Window();

void __fastcall unknown_close_all_dialogs()
{
    HWND dialog;
    while ((dialog = WS_Top_Window()) != 0) {
        WS_Destroy_Dialog(dialog, 0);
    }
}
