// int Run_Game_Options_Dialog() (0x004DDF20)
// Shows dialog 0xB6 and waits until a button stores a result in DWL_USER (0 while waiting,
// 1 when the quit check ends it).
#include "shared/game_options_dialog.hpp"
#include "shared/ms_message_box.hpp"

int Run_Game_Options_Dialog()
{
    int result = 0;
    HWND window = OwnerDraw_Begin_Dialog(0xB6, Game_Options_Proc, 0);
    if (window) {
        SetWindowLongA(window, 8, (LONG)&result);
        Display_Dialog(window);
        while (!result) {
            if (OwnerDraw_Dialog_Message_Handler() == true) {
                result = 1;
            }
        }
        OwnerDraw_End_Dialog(window);
    }
    return result;
}
