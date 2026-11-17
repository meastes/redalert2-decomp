// int __stdcall Skirmish_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) (0x00683D10)
// The common dialog handler first; then the map preview paint, commands, message 0x497 and the tool tips
// of the player-type, side and colour combo boxes.
#include "shared/skirmish.hpp"
#include "shared/gdlg_supp.hpp"
#include "shared/ms_message_box.hpp"
#include "shared/map_preview.hpp"
#include "shared/owner_draw_dialog_control.hpp"
#include "shared/string_table.hpp"

int __stdcall Skirmish_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    int result = Default_Dialog_Proc(window, message, wparam, lparam);
    if (result != 0) {
        return result;
    }
    switch (message) {
    case 0x0F:
        if (MultiplayerMapPreview != 0 && !OwnerDraw::unknown_5e4720(GetDlgItem(window, 0x468))) {
            MultiplayerMapPreview->Blit_Preview(window);
        }
        ValidateRect(window, 0);
        return 0;
    case 0x111:
        Skirmish_Command(window, wparam & 0xFFFF, (HWND)lparam, wparam >> 16);
        return 1;
    case 0x497:
        return Skirmish_Message_497(window, wparam, lparam);
    case 0x4E9: {
        ToolTipTextRequest *request = (ToolTipTextRequest *)lparam;
        request->Text = 0;
        if (request->Window == 0 || request->Item == -1) {
            return 1;
        }
        int id = GetDlgCtrlID(request->Window);
        if (Skirmish_Player_Index(id) == -1) {
            if (Side_Combo_Player_4d13b0(id) == -1) {
                if (Color_Combo_Player_4d1d10(id) == -1) {
                    return 1;
                }
                request->Text = Color_Name_4d1d80(Color_Combo_Data_4d2900(window, id, request->Item));
                return 1;
            }
            int side = Side_Combo_Data_4d1c70(window, id, request->Item);
            if (side == -1) {
                return 1;
            }
            request->Text = Side_Name_4d1420(side);
            return 1;
        }
        int item = request->Item;
        if (item == -1) {
            item = SendDlgItemMessageA(window, id, CB_GETCURSEL, 0, 0);
        }
        request->Text = Skirmish_Player_Type_Name(SendDlgItemMessageA(window, id, CB_GETITEMDATA, item, 0));
        return 1;
    }
    }
    return 0;
}
