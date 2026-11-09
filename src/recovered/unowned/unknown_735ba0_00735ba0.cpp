// void __fastcall unknown_735ba0(HWND window) (0x00735BA0)
// Prepares every child control of a dialog, then the dialog itself, with
// 0x00735BC0 (unknown_show_dialog 0x005FF7E0 calls it).
#include "shared/win_util.hpp"
#include "shared/win32_window.hpp"

void __fastcall unknown_735ba0(HWND window)
{
    EnumChildWindows(window, unknown_prepare_control_735bc0, 1);
    unknown_prepare_control_735bc0(window, 0);
}
