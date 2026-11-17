// SPDX-License-Identifier: GPL-3.0-or-later
// The touch app's on-screen keyboard for the game's own text entry (lane C14, not original
// code). The host (platform/sdl/host.cpp) raises the keyboard while text can be typed:
// a Win32 edit control has the focus (port/src/user32.cpp ra2port_text_input_wanted), or
// one of the game's own text fields here. Nothing in the game changes.
#include "shared/unknown_globals.hpp"
#include "shared/win32_api.hpp"
#include "shared/win32_window.hpp"
#include "shared/main_loop.hpp"
#include "shared/session_state.hpp"
#include "shared/wol_dictionary.hpp"

namespace OwnerDraw {
LRESULT __stdcall unknown_new_edit_proc_5f22a0(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
}
extern "C" HWND GetFocus(void);   // port/src/user32.cpp (C linkage, as the Win32 import)
// The owner-draw controls' own procedures, by window (ownrdraw.cpp 0x00A73400): the setup
// puts its dispatcher 0x005EE460 in each control's GWL_WNDPROC and the control's procedure here.
extern WolDictionary<HWND, WNDPROC> unknown_window_table_a73400;

// Westwood's "NewEdit" control (a list box standing in for an edit, 0x005F22A0 its procedure
// in the owner-draw table): the skirmish and lobby player-name fields. It has the focus and
// is shown.
extern "C" int ra2_native_new_edit_focused(void)
{
    HWND focus = GetFocus();
    if (!focus || !IsWindowVisible(focus)) return 0;
    WNDPROC proc = 0;
    return unknown_window_table_a73400.Get_Value(focus, proc) && proc == &OwnerDraw::unknown_new_edit_proc_5f22a0;
}

// The edit line the game draws in the message list for chat and beacon text,
// Session.Messages (MessageListClass, IsEdit +0x19; opened by Main_Loop's keyboard
// handling with Add_Edit 0x005B5410 in network games, closed by Remove_Edit 0x005B5580
// on Return or Escape).
extern "C" int ra2_native_message_edit_open(void)
{
    return unknown_session_a3d298.Messages.IsEdit ? 1 : 0;
}
