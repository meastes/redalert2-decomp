// LRESULT __stdcall OwnerDraw::unknown_new_edit_proc_5ead00(HWND, UINT, WPARAM, LPARAM) (0x005EAD00)
// The "NewEdit" window class procedure (registered by 0x005EAC30): every message goes to
// DefWindowProcA.
// @ref __imp__DefWindowProcA@16=0079a388
#include "shared/win32_api.hpp"
#include "shared/owner_draw_dialog_control.hpp"

LRESULT __stdcall OwnerDraw::unknown_new_edit_proc_5ead00(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    return DefWindowProcA(window, message, wparam, lparam);
}
