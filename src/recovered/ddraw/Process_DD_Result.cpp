// Process_DD_Result_00495770 (0x00495770)
#include "shared/ghidra_types.hpp"

extern DWORD  sprintf_007820f4(int, int, int);
extern "C" __declspec(dllimport) DWORD __stdcall MessageBoxA(int, int, int, int);
extern "C" DWORD unknown_string_note_007d59f4;
extern "C" code * direct_draw_error_handler_008523ec;
extern "C" DWORD MainWindow_00b263d8;
extern "C" char s_DDRAW_DLL_Error_code____08X_007d59a8[];
extern "C" char s_Direct_Draw_operation_processed_w_007d59c4[];
extern "C" char s_Direct_X_007d599c[];

void __fastcall Process_DD_Result_00495770(int error_code, int show_success)
{
    CHAR error_message [80];

    if (error_code == 0) {
        if (show_success != 0) {
            MessageBoxA((int)(MainWindow_00b263d8), (int)(s_Direct_Draw_operation_processed_w_007d59c4), (int)(&unknown_string_note_007d59f4), (int)(0));
            return;
        }
    }
    else {
        if ((uint)(direct_draw_error_handler_008523ec) != (uint)((code *)0x0)) {
            (*direct_draw_error_handler_008523ec)();
            return;
        }
        sprintf_007820f4((int)(error_message), (int)(s_DDRAW_DLL_Error_code____08X_007d59a8), (int)(error_code));
        MessageBoxA((int)(MainWindow_00b263d8), (int)(error_message), (int)(s_Direct_X_007d599c), (int)(0x30));
    }
    return;
}

