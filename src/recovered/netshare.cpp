// netshare.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/netshare.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/lobby.hpp"
#include "shared/text_print.hpp"
#include "shared/win32_window.hpp"
#include "shared/win32_gdi.hpp"
#include "shared/house.hpp"
#include "shared/campaign_score.hpp"
#include "shared/win32_api.hpp"
#include "shared/multiplayer_game_mode.hpp"
#include "shared/session_state.hpp"
#include "shared/crt_time.hpp"
#include "shared/crt_string.hpp"
#include "shared/ipx_manager.hpp"
#include "shared/game_options_packet.hpp"
#include "shared/debug_printf.hpp"
#include "shared/wol_chat.hpp"
#include "shared/string_table.hpp"
#include "shared/lobby_name_set.hpp"
#include "shared/multi_mission.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/ini_class.hpp"
#include "shared/map_preview.hpp"
#include "shared/ipx_conn.hpp"
#include "shared/skirmish.hpp"
#include "shared/ww_message_box.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/id_source.hpp"
#include "shared/cd_timer.hpp"
#include "shared/special.hpp"
#include "shared/ipx_address.hpp"
#include "shared/serial_progress.hpp"
#include "shared/system_timer.hpp"
#include "shared/raw_file_class.hpp"
#include "shared/queue.hpp"
#include "shared/mix_file_class.hpp"
#include "shared/buffer_straw.hpp"
#include "shared/lzo_straw.hpp"
#include "shared/cd.hpp"
#include "shared/unknown_map_generator.hpp"
#include "shared/gdlg_supp.hpp"
#include "shared/crt_tokens.hpp"
#include "shared/pcx_cache.hpp"
#include "shared/owner_draw_dialog_control.hpp"
#include "shared/progress_screen.hpp"
#include "shared/wide_string.hpp"
#include "shared/ccini_class.hpp"
#include "shared/wol_settings.hpp"


// Header-static dynamic initializers of the unit at 0x005C29B0: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x005C29B0-0x005C2B91); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x005C29D0, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_005c29b0.cpp; no source of its own.


// Dynamic initializer (0x005C2A00, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_005c29b0.cpp; no source of its own.


// Dynamic initializer (0x005C2A20, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_005c29b0.cpp; no source of its own.


// Dynamic initializer (0x005C2A40, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_005c29b0.cpp; no source of its own.


// Dynamic initializer (0x005C2A60, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_005c29b0.cpp; no source of its own.


// Dynamic initializer (0x005C2A80, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_005c29b0.cpp; no source of its own.


// Dynamic initializer (0x005C2AA0, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_005c29b0.cpp; no source of its own.


// Dynamic initializer (0x005C2AE0, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_005c29b0.cpp; no source of its own.


// Dynamic initializer (0x005C2B10, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_005c29b0.cpp; no source of its own.


// Dynamic initializer (0x005C2B40, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_005c29b0.cpp; no source of its own.


// Dynamic initializer (0x005C2B70, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_005c29b0.cpp; no source of its own.


// Dynamic initializer (0x005C2B80, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_005c29b0.cpp; no source of its own.


// unsigned int __fastcall Hash_Name_005c2ba0(Wstring const &name) (0x005C2BA0)
// A rotating hash of a narrow name (its length, then each character plus its index, rotated left by 8);
// the WOL code passes its address to hash tables.

unsigned int __fastcall Hash_Name_005c2ba0(Wstring const &name)
{
    unsigned int hash = name.Length();
    for (unsigned int i = 0; i < name.Length(); i++) {
        hash += name.Get()[i] + i;
        hash = (hash << 8) ^ (hash >> 24);
    }
    return hash;
}


// unsigned int __fastcall Hash_Wide_Name_005c2bf0(WideString const &name) (0x005C2BF0)
// The same hash over a wide name.

unsigned int __fastcall Hash_Wide_Name_005c2bf0(WideString const &name)
{
    unsigned int hash = name.Length();
    for (unsigned int i = 0; i < name.Length(); i++) {
        hash += name.Get()[i] + i;
        hash = (hash << 8) ^ (hash >> 24);
    }
    return hash;
}


// HWND Find_Lobby_Window_005c2c40() (0x005C2C40)
// The first of the dialogs 0xC2, 0xC9, 0xBC and 0xBD that is open, or 0.

HWND Find_Lobby_Window_005c2c40()
{
    HWND window = WS_Find_Dialog_00735940(0xc2);
    if (window == 0) {
        window = WS_Find_Dialog_00735940(0xc9);
        if (window == 0) {
            window = WS_Find_Dialog_00735940(0xbc);
            if (window == 0) {
                window = WS_Find_Dialog_00735940(0xbd);
            }
        }
    }
    return window;
}


// void __cdecl Unknown_005C2C80(int color, unsigned short const *format, ...) (0x005C2c80)
// Formats a line and adds it to the list 0x53f of the current dialog or of the nearest dialog below it that has one.

void __cdecl Unknown_005C2C80(int color, unsigned short const *format, ...)
{
    unsigned short buffer[0x400];
    memset(buffer, 0, sizeof(buffer));
    vswprintf(buffer, format, (char *)(&format + 1));
    if (Unknown_00735B10() != 0) {
        HWND window = Unknown_00735B10();
        HWND list = GetDlgItem(window, 0x53f);
        while (list == 0) {
            window = (HWND)unknown_previous_key((int)window);
            if (window == 0) {
                return;
            }
            list = GetDlgItem(window, 0x53f);
        }
        _DrawMessage_005c2da0(color, buffer, list);
    }
}


// void __cdecl Unknown_005c2d10(int color, unsigned short const *format, ...) (0x005C2d10)
// Formats a line and adds it to the list 0x5b5 of the current dialog or of the nearest dialog below it that has one.

void __cdecl Unknown_005c2d10(int color, unsigned short const *format, ...)
{
    unsigned short buffer[0x400];
    memset(buffer, 0, sizeof(buffer));
    vswprintf(buffer, format, (char *)(&format + 1));
    if (Unknown_00735B10() != 0) {
        HWND window = Unknown_00735B10();
        HWND list = GetDlgItem(window, 0x5b5);
        while (list == 0) {
            window = (HWND)unknown_previous_key((int)window);
            if (window == 0) {
                return;
            }
            list = GetDlgItem(window, 0x5b5);
        }
        _DrawMessage_005c2da0(color, buffer, list);
    }
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void __fastcall _DrawMessage_005c2da0(int color, wchar_t const *message, HWND list) -- 0x005C2DA0
// From ra2-decomp _DrawMessage (YR 0x005E2820).
// Purpose: adds message to the owner-drawn list box as lines that fit its
// width (breaking at a newline or the last non-graphic character), each
// through Add_List_Text_005c2f20 with color.

void __fastcall _DrawMessage_005c2da0(int color, unsigned short const *message, HWND window)
{
    RECT rect;
    struct { HDC hdc; struct { long cx; long cy; } size; } l;    // the twin's { HDC; SIZE } (no SIZE declared here)

    int length = wcslen(message);

    int offset = 18;
    Window_Rect_In_Main_Client(window, &rect);
    if (SendMessageA(window, 0x4a0, 0, 0)) {    // OD_HASATTACHED
        offset = 1;
    }

    l.hdc = GetDC(window);
    SendMessageA(window, 0x49e, 0, (LPARAM)l.hdc);    // OD_RESTOREDC

    while (length) {
        if (message != 0) {
            wchar_t const *newline = wcschr(message, L'\n');
            if (newline != 0) {
                int linelen = newline - message + 1;
                if (length >= linelen) {
                    length = linelen;
                }
            }
        }

        wchar_t *copy = _wcsdup(message);
        copy[length] = 0;
        BitFont->Get_Text_Area(copy, (int *)&l.size.cx, (int *)&l.size.cy, 0);
        free(copy);
        int maxWidth = rect.right - rect.left - offset;

        if (l.size.cx >= maxWidth - 4) {
            int reduceBy;
            if (l.size.cx / 2 > maxWidth) {
                reduceBy = 10;
                length -= reduceBy;
            } else {
                reduceBy = 1;
            }

            int found = -1;
            int idx = length - 1;

            while (idx > 0) {
                if (!iswgraph(message[idx])) {
                    found = idx;
                    break;
                }
                idx--;
            }

            if (found == -1) {
                length -= reduceBy;
                found = length;
            }
            length = found;
        } else {
            Add_List_Text_005c2f20(window, message, length, color);
            message += length;
            while (*message == L' ') {
                message++;
            }
            length = wcslen(message);
        }
    }

    ReleaseDC(window, l.hdc);
}


// void __fastcall Add_List_Text_005c2f20(HWND list, wchar_t const *text, int length, int color) (0x005C2F20)

void __fastcall Add_List_Text_005c2f20(HWND list, wchar_t const *text, int length, int color)
{
    memset(unknown_list_text_a71ea8, 0, sizeof(unknown_list_text_a71ea8));
    wcsncpy(unknown_list_text_a71ea8, text, length);
    wchar_t *end = wcschr(unknown_list_text_a71ea8, '\r');
    if (end == 0) {
        end = wcschr(unknown_list_text_a71ea8, '\n');
    }
    if (end != 0) {
        *end = 0;
    }
    LRESULT redraw = SendMessageA(list, 0x49d, 0, 1);
    int top = SendMessageA(list, 0x18e, 0, 0);                  // LB_GETTOPINDEX
    int count = SendMessageA(list, 0x18b, 0, 0);                // LB_GETCOUNT
    int height = SendMessageA(list, 0x1a1, top, 0);             // LB_GETITEMHEIGHT
    RECT rect;
    GetWindowRect(list, &rect);
    bool at_bottom = count - top <= (rect.bottom - rect.top) / height;
    if (count > 500) {
        SendMessageA(list, 0x182, 0, 0);                        // LB_DELETESTRING
        count--;
    }
    int index = SendMessageA(list, 0x4cd, 0, (LPARAM)unknown_list_text_a71ea8);
    if (color != -1) {
        SendMessageA(list, 0x498, index, color);
    }
    if (at_bottom) {
        SendMessageA(list, 0x197, count, 0);                    // LB_SETTOPINDEX
    }
    SendMessageA(list, 0x49d, 0, redraw);
}


// void __fastcall Add_Chat_Dot_005c3040() (0x005C3040)
// Appends "." to the last line of the chat list 0x53F of the current dialog (or the nearest dialog below it
// that has one), starting a new line once a line has 0x24 characters; redraw is off meanwhile (0x49D).

void __fastcall Add_Chat_Dot_005c3040()
{
    if (Unknown_00735B10() != 0) {
        HWND window = Unknown_00735B10();
        HWND list = GetDlgItem(window, 0x53f);
        while (list == 0) {
            window = (HWND)unknown_previous_key((int)window);
            if (window == 0) {
                return;
            }
            list = GetDlgItem(window, 0x53f);
        }
        LRESULT redraw = SendMessageA(list, 0x49d, 0, 1);
        int count = SendMessageA(list, 0x18b, 0, 0);                    // LB_GETCOUNT
        WideString text;
        if (count > 0) {
            unknown_get_list_item_wide_text(list, count - 1, text);
        }
        if (text.Length() < 0x24) {
            SendMessageA(list, 0x182, count - 1, 0);                    // LB_DELETESTRING
            text += L".";
        } else {
            text = L".";
            count++;
        }
        SendMessageA(list, 0x4cd, 0, (LPARAM)text.unknown_text_770430());
        while (count > 500) {
            SendMessageA(list, 0x182, 0, 0);
        }
        SendMessageA(list, 0x49d, 0, redraw);
    }
}


// int __fastcall Count_Teams_005c3150(HouseClass *exclude) (0x005C3150)
// Counts the teams among the live houses that have unknown_134 set (exclude left out): a house allied both
// ways with another counts once for the group, at its first allied partner after it; a house without such a
// partner counts alone.

int __fastcall Count_Teams_005c3150(HouseClass *exclude)
{
    int count = 0;
    for (int i = 0; i < Houses.Count(); i++) {
        HouseClass *house = Houses[i];
        if (house != 0 && !house->unknown_13d && house->unknown_134 && house != exclude) {
            bool allied = false;
            for (int j = 0; j < Houses.Count(); j++) {
                if (j != i) {
                    HouseClass *other = Houses[j];
                    if (other != 0 && !other->unknown_13d && other->unknown_134 && other != exclude
                        && house->Is_Ally(other) && other->Is_Ally(house)) {
                        if (j > i && !allied) {
                            count++;
                        }
                        allied = true;
                    }
                }
            }
            if (!allied) {
                count++;
            }
        }
    }
    return count;
}


// int __fastcall Message_Box_005c3230(wchar_t const *text, int kind, bool (*callback)(), bool flag) (0x005C3230)

int __fastcall Message_Box_005c3230(wchar_t const *text, int kind, bool (*callback)(), bool flag)
{
    if (text == 0 || wcslen(text) == 0) {
        return 0;
    }
    int id;
    if (flag) {
        id = 0xfb;
    } else if (kind == 0) {
        id = 0xd0;
    } else if (kind == 1) {
        id = 0xfc;
    } else {
        id = kind == 4 ? 0x126 : 0xfb;
    }
    HWND dialog = (HWND)unknown_create_dialog_735700(ProgramInstance, id, (HWND)unknown_b263d8, Unknown_Dialog_Proc_005c33a0, 0);
    unknown_show_dialog_5ff7e0(dialog, id);
    HWND button;
    if (kind == 0) {
        button = GetDlgItem(dialog, 1);
        SetWindowLongA(button, -16, GetWindowLongA(button, -16) | 0x10000000);      // WS_VISIBLE
        if (flag) {
            Unknown_005e8fa0(button);
        }
    } else if (kind == 1) {
        button = GetDlgItem(dialog, 1);
        SetWindowLongA(button, -16, GetWindowLongA(button, -16) | 0x10000000);
        button = GetDlgItem(dialog, 2);
        SetWindowLongA(button, -16, GetWindowLongA(button, -16) | 0x10000000);
    } else if (kind == 4) {
        button = GetDlgItem(dialog, 6);
        SetWindowLongA(button, -16, GetWindowLongA(button, -16) | 0x10000000);
        button = GetDlgItem(dialog, 7);
        SetWindowLongA(button, -16, GetWindowLongA(button, -16) | 0x10000000);
    }
    HWND message = GetDlgItem(dialog, 0x5b0);
    SendMessageA(message, 0x4b2, 0, (LPARAM)text);
    ShowWindow(dialog, 1);
    return Unknown_007359e0(dialog, callback, 0, 1, 1);
}


// long __stdcall Unknown_Dialog_Proc_005c33a0(HWND window, unsigned message, unsigned wparam, long lparam) (0x005C33A0)
// A plain lobby message dialog's procedure: paints and draws its items, and ends with the command id of
// buttons 1, 2, 6 and 7 (IDOK, IDCANCEL, IDYES, IDNO).

long __stdcall Unknown_Dialog_Proc_005c33a0(HWND window, unsigned message, unsigned wparam, long lparam)
{
    switch (message) {
    case 0x14:                  // WM_ERASEBKGND
        return 1;
    case 0x0f:                  // WM_PAINT
        Unknown_005FEE70(window);
        unknown_dialog_5e5f20(window);
        ValidateRect(window, 0);
        return 1;
    case 0x2b:                  // WM_DRAWITEM
        Unknown_005FE380(lparam);
        return 1;
    case 0x111:                 // WM_COMMAND
        switch (wparam & 0xffff) {
        case 1:
        case 2:
        case 6:
        case 7:
            Unknown_007357E0(window, wparam & 0xffff);
            return 1;
        }
        break;
    }
    return 0;
}


// void __fastcall unknown_set_dialog_wide_text(HWND dialog, UnknownTextOwner_005c3450 *owner) -- 0x005C3450
// Purpose: sends the owner's wide text (+0x20; 0x00770430 returns the
// buffer or an empty string) to dialog control 0x6EC with the custom message
// 0x4B2 (WM_USER + 0xB2). No direct callers; bound by linear sweep to the
// ret (the frozen inventory bound stops at the SendMessageA call).

WIN32_IMPORT HWND __stdcall GetDlgItem(HWND dialog, int id);
WIN32_IMPORT long __stdcall SendMessageA(HWND window, unsigned int message, unsigned int wparam, long lparam);

// A wide string holder (0x007703F0 area); 0x00770430 returns its buffer or
// the shared empty string 0x00839D4C.
class UnknownWideText_005c3450 {
public:
    unsigned short const *unknown_text_770430() const;
    unsigned short *Buffer;
};

struct UnknownTextOwner_005c3450 {
    char unknown_00[0x20];
    UnknownWideText_005c3450 unknown_text_20;
};

void __fastcall unknown_set_dialog_wide_text_005c3450(HWND dialog, UnknownTextOwner_005c3450 *owner)
{
    HWND item = GetDlgItem(dialog, 0x6ec);
    SendMessageA(item, 0x4b2, 0, (LPARAM)owner->unknown_text_20.unknown_text_770430());
}


// MultiplayerGameMode *Current_Game_Mode_005c34d0() (0x005C34D0)
// The session's game mode object (session +0x18), or the first registered mode when there is none.

MultiplayerGameMode *Current_Game_Mode_005c34d0()
{
    MultiplayerGameMode *mode = MultiplayerGameMode::unknown_find_mode_5b6fe0(unknown_session_a3d298.GameModeIndex);
    if (mode == 0) {
        return MultiplayerGameMode::unknown_first_mode_5b6ec0();
    }
    return mode;
}


// void __fastcall Unknown_005c34f0() (0x005C34F0)

void __fastcall Unknown_005c34f0()
{
    if (Current_Dialog_Id_00735b20() == 0x6b) {
        HWND dialog = Unknown_00735B10();
        int index = SendDlgItemMessageA(dialog, 0x6eb, 0x188, 0, 0);
        if (index != -1) {
            UnknownSessionA3D29C *mode = (UnknownSessionA3D29C *)SendDlgItemMessageA(dialog, 0x6eb, 0x199, index, 0);
            if (mode != 0) {
                mode->unknown_slot31();
            }
        }
    } else if (unknown_session_a3d29c != 0) {
        unknown_session_a3d29c->unknown_slot31();
    }
}


// void __fastcall Unknown_005c3810(bool unknown_1, bool unknown_2) (0x005C3810)
// Sends the game options when the session's settings differ from the last sent copy (or a resend is
// forced), at most once a second; the copy is refreshed before the send.

extern long unknown_sent_time_a72d48;
extern bool unknown_send_pending_a72d4c;
extern int unknown_sent_speed_7e6dc4;
extern int unknown_sent_24_7e6dc8;
extern int unknown_sent_units_7e6dcc;
extern bool unknown_sent_official_7e6dd0;
extern int unknown_sent_length_7e6dd4;
extern bool unknown_sent_2a_a72d4d;
extern bool unknown_sent_2b_a72d4e;
extern bool unknown_sent_2c_a72d4f;
extern bool unknown_sent_a8_a72d50;
extern bool unknown_sent_crates_a72d51;
extern unsigned short unknown_sent_map_name_a72c90[44];
extern char unknown_sent_file_name_a72890[14];
extern char unknown_sent_digest_a728a0[33];

void __fastcall Unknown_005c3810(bool unknown_1, bool unknown_2)
{
    char buffer[0x208];
    bool changed = false;
    if (time(0) - unknown_sent_time_a72d48 == 0) {
        if (unknown_1) {
            unknown_send_pending_a72d4c = true;
        }
        if (!unknown_2) {
            return;
        }
    }
    if (unknown_send_pending_a72d4c) {
        unknown_1 = true;
    }
    unknown_send_pending_a72d4c = false;
    if (unknown_chat_a72cfc != 0) {
        if (time(0) - unknown_sent_time_a72d48 >= 5) {
            changed = true;
        }
    } else {
        if (time(0) - unknown_sent_time_a72d48 >= 2) {
            changed = true;
        }
    }
    if (unknown_1) {
        changed = true;
    }
    if (unknown_sent_speed_7e6dc4 != unknown_session_a3d298.GameSpeed_30) {
        changed = true;
    }
    if (unknown_sent_24_7e6dc8 != unknown_session_a3d298.unknown_24) {
        changed = true;
    }
    if (unknown_sent_units_7e6dcc != unknown_session_a3d298.UnitCount) {
        changed = true;
    }
    if (unknown_sent_2a_a72d4d != unknown_session_a3d298.unknown_2a) {
        changed = true;
    }
    if (unknown_sent_2b_a72d4e != unknown_session_a3d298.unknown_2b) {
        changed = true;
    }
    if (unknown_sent_2c_a72d4f != unknown_session_a3d298.unknown_2c) {
        changed = true;
    }
    if (unknown_sent_a8_a72d50 != unknown_session_a3d298.unknown_a8) {
        changed = true;
    }
    if (unknown_sent_crates_a72d51 != unknown_session_a3d298.unknown_crates_29) {
        changed = true;
    }
    unknown_sent_map_name_a72c90[43] = 0;
    if (_wcsicmp(unknown_sent_map_name_a72c90, unknown_map_name_a3d342) != 0) {
        changed = true;
    }
    unknown_sent_file_name_a72890[13] = 0;
    if (_stricmp(unknown_sent_file_name_a72890, unknown_session_a3d298.ScenarioFileName_658) != 0) {
        changed = true;
    }
    unknown_sent_digest_a728a0[32] = 0;
    if (_stricmp(unknown_sent_digest_a728a0, unknown_session_a3d298.ScenarioDigest_85a) != 0) {
        changed = true;
    }
    if (unknown_sent_length_7e6dd4 != unknown_session_a3d298.ScenarioFileLength_87c) {
        changed = true;
    }
    if (unknown_sent_official_7e6dd0 != unknown_session_a3d298.ScenarioIsOfficial_880) {
        changed = true;
    }
    if (!changed) {
        return;
    }
    unknown_sent_time_a72d48 = time(0);
    unknown_sent_speed_7e6dc4 = unknown_session_a3d298.GameSpeed_30;
    unknown_sent_24_7e6dc8 = unknown_session_a3d298.unknown_24;
    unknown_sent_units_7e6dcc = unknown_session_a3d298.UnitCount;
    unknown_sent_2a_a72d4d = unknown_session_a3d298.unknown_2a;
    unknown_sent_2b_a72d4e = unknown_session_a3d298.unknown_2b;
    unknown_sent_2c_a72d4f = unknown_session_a3d298.unknown_2c;
    unknown_sent_a8_a72d50 = unknown_session_a3d298.unknown_a8;
    unknown_sent_crates_a72d51 = unknown_session_a3d298.unknown_crates_29;
    wcscpy(unknown_sent_map_name_a72c90, unknown_map_name_a3d342);
    strcpy(unknown_sent_digest_a728a0, unknown_session_a3d298.ScenarioDigest_85a);
    strcpy(unknown_sent_file_name_a72890, unknown_session_a3d298.ScenarioFileName_658);
    unknown_sent_length_7e6dd4 = unknown_session_a3d298.ScenarioFileLength_87c;
    unknown_sent_official_7e6dd0 = unknown_session_a3d298.ScenarioIsOfficial_880;
    memset(buffer, 0, sizeof(buffer));
    if (unknown_chat_a72cfc != 0) {
        Unknown_0073ccb0(buffer);
        Send_Game_Options_005c3c60(buffer);
    } else {
        Build_Game_Options_005bc940(buffer);
        Send_Game_Options_005c3c60(buffer);
    }
}


// void Unknown_005c3ae0() (0x005C3AE0)
// Sends every other player a global packet 0x28 with the local nick name, serving the game I/O after each.

void Unknown_005c3ae0()
{
    UnknownGlobalPacket packet;
    memset(&packet, 0, sizeof(packet));
    packet.Command = 0x28;
    strcpy(packet.Name, unknown_name_a3d3a0);
    int count = unknown_players_a3fa7c.Count();
    for (int i = 1; i < count; i++) {
        unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 1, &unknown_players_a3fa7c[i]->Address, 0, 0);
        Service_Game_IO_00481fb0();
    }
}


// void __fastcall Unknown_005c3b70() (0x005C3B70)
// Game mode 3 sends the players packet 0x28 (0x005C3AE0); game mode 4 runs 0x0075C960.

void __fastcall Unknown_005c3b70()
{
    switch (unknown_game_mode_a3d298) {
    case 3:
        Unknown_005c3ae0();
        break;
    case 4:
        Unknown_0075c960();
        break;
    }
}


// void Unknown_005c3c20() (0x005C3C20)
// Sets the lobby flag 0x00A3FAB8 and runs the chat's (0x0075C980) or the LAN's (0x005C28E0) handler.

void Unknown_005c3c20()
{
    unknown_lobby_flag_a3fab8 = true;
    if (unknown_chat_a72cfc != 0) {
        Unknown_0075c980();
    } else {
        Unknown_005c28e0();
    }
}


// void Unknown_005c3c40() (0x005C3C40)
// Clears the lobby flag 0x00A3FAB8 and runs the chat's (0x0075CA00) or the LAN's (0x005C2940) handler.

void Unknown_005c3c40()
{
    unknown_lobby_flag_a3fab8 = false;
    if (unknown_chat_a72cfc != 0) {
        Unknown_0075ca00();
    } else {
        Unknown_005c2940();
    }
}


// void __fastcall Send_Game_Options_005c3c60(char const *text) (0x005C3C60)
// Under World Online the text goes to the chat server as the public game options; otherwise a global
// packet 0x17 with the local nick name, the text, the colour and the host name's CRC goes to every other
// player, serving the game I/O after each send.

void __fastcall Send_Game_Options_005c3c60(char const *text)
{
    if (unknown_chat_a72cfc != 0) {
        char buffer[1024];
        strncpy(buffer, text, sizeof(buffer) - 1);
        unknown_chat_a72cfc->unknown_slot15(buffer);
        return;
    }
    UnknownOptionsPacket packet;
    memset(&packet, 0, sizeof(packet));
    packet.Command = 0x17;
    strcpy(packet.Name, unknown_name_a3d3a0);
    strcpy(packet.Text, text);
    packet.Color = unknown_a3d3b4;
    packet.NameCRC = Unknown_005bba50(unknown_host_name_a3f618);
    for (int i = 1; i < unknown_players_a3fa7c.Count(); i++) {
        NodeNameType *player = unknown_players_a3fa7c[i];
        Debug_Printf("Sending public game options to %s\n", Wide_To_Narrow_006fa120(player->Name));
        unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 1, &player->Address, 0, 0);
        Service_Game_IO_00481fb0();
    }
}


// void __fastcall Unknown_005c3d90(char const *name, char const *text) (0x005C3D90)
// Sends game options text privately to the player called name: under World Online as a chat page to that
// user; otherwise as a global packet 0x18 (local nick name and the text) to each other player whose narrowed
// name matches, serving the game I/O after each send. The colour and host CRC go into the session's own
// packet (unknown_gpacket_a3f640), not the sent copy.

void __fastcall Unknown_005c3d90(char const *name, char const *text)
{
    WOL::IChat *chat = unknown_chat_a72cfc;
    if (chat != 0) {
        WOL::User user;
        memset(&user, 0, sizeof(user));
        strcpy(user.name, name);
        chat->unknown_slot14(&user, text);
        return;
    }
    UnknownOptionsPacket packet;
    memset(&packet, 0, sizeof(packet));
    packet.Command = 0x18;
    strcpy(packet.Name, unknown_name_a3d3a0);
    strcpy(packet.Text, text);
    ((UnknownOptionsPacket &)unknown_gpacket_a3f640).Color = unknown_session_a3d298.Value_011c_00672a10();
    ((UnknownOptionsPacket &)unknown_gpacket_a3f640).NameCRC = Unknown_005bba50(unknown_host_name_a3f618);
    for (int i = 1; i < unknown_players_a3fa7c.Count(); i++) {
        if (_stricmp(Wide_To_Narrow_006fa120(unknown_players_a3fa7c[i]->Name), name) == 0) {
            Debug_Printf("Sending private game options to %ls\n", unknown_players_a3fa7c[i]->Name);
            unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 1, &unknown_players_a3fa7c[i]->Address, 0, 0);
            Service_Game_IO_00481fb0();
        }
    }
}


// void __fastcall Set_Player_Map_State_005c3f00(char const *name, int state) (0x005C3F00)

void __fastcall Set_Player_Map_State_005c3f00(char const *name, int state)
{
    if (name == 0) {
        name = unknown_name_a3d3a0;
    }
    if (Compare_Wide_007837d3(Narrow_To_Wide_006fa180(name), unknown_host_name_a3f618) == 0) {
        state = 1;
    }
    int found = -1;
    for (int i = 0; i < unknown_players_a3fa7c.Count(); i++) {
        if (Compare_Narrow_00781f20(Wide_To_Narrow_006fa120(unknown_players_a3fa7c[i]->Name), name) == 0) {
            found = i;
            break;
        }
    }
    if (found != -1) {
        unknown_players_a3fa7c[found]->unknown_map_state_6f = state;
        if (state == 0) {
            Unknown_005C2C80(-1, StringTable::Load_String("GUI:HostNoMap", 0, "D:\\Ra2\\netshare.cpp", 0x45B),
                             unknown_players_a3fa7c[found]->Name, unknown_map_name_a3d342);
        } else if (state == 2) {
            Unknown_005C2C80(-1, StringTable::Load_String("GUI:HostMapTransfer", 0, "D:\\Ra2\\netshare.cpp", 0x45F),
                             unknown_players_a3fa7c[found]->Name, unknown_map_name_a3d342);
        }
    }
}


// Save the selected list rows in the lobby name dictionary (0x005C5FF0).

void __fastcall Save_List_Selection_005c5ff0(HWND list, LobbyNameSet *names)
{
    int count = SendMessageA(list, 0x18B, 0, 0);
    WideString name;
    wchar_t text[0x400];
    char narrow[0x400];
    if (count == 0) return;
    {
        LONG style = GetWindowLongA(list, GWL_STYLE);
        if (style & 8) {
            for (int index = 0; index < count; ++index) {
                if (SendMessageA(list, 0x187, index, 0)) {
                    Get_List_Row_Text(list, index, text, narrow);
                    name = text;
                    bool selected = true;
                    names->Add(name, selected);
                }
            }
        } else if (!(style & 0x4000)) {
            int index = SendMessageA(list, 0x188, 0, 0);
            if (index >= 0) {
                text[0] = 0;
                Get_List_Row_Text(list, index, text, narrow);
                name = text;
                bool selected = true;
                names->Add(name, selected);
            }
        }
    }
}


// Not exact (RA2_NON_MATCHING): Decode_Pub_Gameopt_005c3ff0 (Receive_Game_Options) at 0x005C3FF0, best score 64.58, 2026-11-13; differs: 2491 instructions in the original, 2495 in the draft (shape 94.67)
#ifdef RA2_NON_MATCHING
// bool __fastcall Receive_Game_Options_005c3ff0(char const *text, char const *sender) (0x005C3FF0)
// Decodes a received game options string by its first letter: Z (NAT state, port, no preview), O (the
// observer slot), A (a player value), C (cooperative campaign info), L (slot names), G, K (map state),
// P (pings); anything else is the host's full options string (seed, speed, money, units, flags, map,
// digest, the players' colours/countries/addresses and the AI slots).
#include "shared/mplayer_score_type.hpp"
#include "shared/vector_class.hpp"
#include "shared/crt_memory.hpp"
#include "shared/wstring.hpp"
#include "shared/options_dialog.hpp"
#include "shared/options.hpp"

extern "C" char *__cdecl _strdup(char const *string);
extern "C" long __cdecl atol(char const *string);
extern "C" int __cdecl atoi(char const *string);
extern "C" int __cdecl sscanf(char const *string, char const *format, ...);
extern bool unknown_b2abd9;
extern int unknown_a72d08;
extern int unknown_b2ac40;
extern IPXAddressClass unknown_slot_addresses_a3fbd4[8];
extern int unknown_slot_a3fc34[8];
extern int unknown_slot_a40238[8];
extern int unknown_old_speed_7e6dd8;
extern int unknown_old_money_7e6ddc;
extern int unknown_old_units_7e6de0;
extern bool unknown_old_2a_a72d53;
extern bool unknown_old_2b_a72d54;
extern bool unknown_old_2c_a72d55;
extern bool unknown_old_a8_a72d56;
extern bool unknown_old_crates_a72d57;
extern wchar_t unknown_scenario_name_a3d342[];
bool Local_User_Is_Channel_Owner_0074d960();
void __fastcall Register_Connection_State_0074e1f0(char const *sender, char const *receiver, int state);
void __fastcall Store_Player_Port_0074e2d0(char const *name, unsigned short port);
void __fastcall Leave_Channel_With_Message_0074c5f0(HWND dialog);
void __fastcall Store_User_Ping_0075b110(char const *name, int ping);
void __fastcall Unknown_0073d150(Wstring const &name, int value);
int __fastcall Set_Channel_Player_Options_0073d550(Wstring const &name, int country, int color);
int __fastcall Unknown_0073d3f0(Wstring const &name);
WideString __fastcall Wide_From_7_Bit(Wstring const &string);
bool Unknown_0073bdb0();

extern wchar_t const unknown_empty_wide_839d4c[];   // NATIVE: declared for the draft
bool __fastcall Decode_Pub_Gameopt_005c3ff0(char const *text, char const *sender)   // NATIVE: the name callers declare (draft: Receive_Game_Options_005c3ff0)
{
    bool changed = false;
    char *copy = _strdup(text);
    if (copy[0] == 'Z') {
        switch (copy[1]) {
        case '1': {
            for (int i = 0; i < 8; i++) {
                if (_stricmp(sender, Wide_To_Narrow_006fa120(unknown_player_slots_a3fa98[i]->Name)) == 0) {
                    char receiver[64];
                    strcpy(receiver, copy + 3);
                    Register_Connection_State_0074e1f0(sender, receiver, copy[2] - '0');
                }
            }
            break;
        }
        case '2':
            Store_Player_Port_0074e2d0(sender, (unsigned short)atol(copy + 2));
            free(copy);
            return false;
        case '3':
            if (!Local_User_Is_Channel_Owner_0074d960()) {
                HWND dialog = WS_Find_Dialog_00735940(0xC9);
                HWND preview = GetDlgItem(dialog, 0x3F2);
                if (preview != 0) {
                    ShowWindow(preview, SW_SHOWNORMAL);
                    ShowWindow(GetDlgItem(dialog, 0x468), SW_HIDE);
                    SendMessageA(preview, 0x4B2, 0, (LPARAM)StringTable::Load_String("TXT_NAT_NO_PREVIEW", 0, "D:\\Ra2\\netshare.cpp", 0x4BE));
                }
            }
            break;
        }
        free(copy);
        return false;
    }
    if (unknown_game_mode_a3d298 == 4 && (unknown_b2abd9 || unknown_b2ac40 != 0)) {
        free(copy);
        return false;
    }
    if (copy[0] == 'O') {
        HWND dialog = WS_Find_Dialog_00735940(0xC9);
        if (dialog != 0 || (dialog = WS_Find_Dialog_00735940(0xBD)) != 0) {
            int old = unknown_lobby_flag_7e6d90;
            unknown_lobby_flag_7e6d90 = copy[1] == '8' ? -1 : copy[1] - '0';
            if (unknown_lobby_flag_7e6d90 != old) {
                Unknown_005cafd0(dialog);
                wchar_t name[256];
                wcscpy(name, Narrow_To_Wide_006fa180(copy + 2));
                if (name[0] != 0) {
                    NodeNameType *entry;
                    {
                        WideString wide(name);
                        entry = Unknown_Find_Lobby_Entry_005cd1b0(wide);
                    }
                    if (entry == 0) {
                        entry = Unknown_Find_Lobby_Name_005c8b20(name);
                        if (entry == 0) {
                            free(copy);
                            return false;
                        }
                    }
                    if (entry == unknown_player_a72d44
                        || (entry != 0 && entry != unknown_open_entry_a72d2c && entry != unknown_closed_entry_a72d30
                            && entry != unknown_waiting_entry_a72d40 && entry != unknown_easy_ai_entry_a72d34
                            && entry != unknown_medium_ai_entry_a72d38 && entry != unknown_hard_ai_entry_a72d3c
                            && entry->Team == -1)) {
                        unknown_player_slots_a3fa98[unknown_lobby_flag_7e6d90] = entry;
                        Fill_Observer_Combo_4d1640(dialog, Player_Side_Combo_ID_4d1350(unknown_lobby_flag_7e6d90));
                        SendMessageA(GetDlgItem(dialog, Slot_Control_Id_005c97a0(unknown_lobby_flag_7e6d90)), 0x4F1, 0, 1);
                        int id = Player_Color_Combo_ID_4d1cb0(unknown_lobby_flag_7e6d90);
                        Fill_Random_Color_Combo_4d2250(dialog, id);
                        Select_Color_Combo_4d2480(dialog, id, 8);
                    } else {
                        Fill_Side_Combo_4d1560(dialog, Player_Side_Combo_ID_4d1350(unknown_lobby_flag_7e6d90));
                        SendMessageA(GetDlgItem(dialog, Slot_Control_Id_005c97a0(unknown_lobby_flag_7e6d90)), 0x4F1, 0, 0);
                        int id = Player_Color_Combo_ID_4d1cb0(unknown_lobby_flag_7e6d90);
                        Fill_Color_Combo_4d2080(dialog, id);
                        Select_Color_Combo_4d2480(dialog, id, -2);
                    }
                    unknown_player_slots_a3fa98[unknown_lobby_flag_7e6d90] = entry;
                }
            }
            if (unknown_lobby_flag_7e6d90 >= 0 && unknown_lobby_flag_7e6d90 != unknown_lobby_a3fa94) {
                Unknown_005c9b90(dialog, unknown_lobby_flag_7e6d90, false);
            }
        }
        free(copy);
        return false;
    }
    if (copy[0] == 'A') {
        int value = atol(copy + 1);
        if (unknown_chat_a72cfc != 0) {
            Unknown_0073d150(Wstring(sender), value);
            free(copy);
            return false;
        }
        Set_Player_Value_005bcbc0(sender, value);
        free(copy);
        return false;
    }
    if (copy[0] == 'C') {
        if (copy[1] == '0') {
            Debug_Printf("@@@@ Decoding cooperative campaign game options %s\n", text);
            int index = -1;
            DynamicVectorClass<int> list;
            int time = 0;
            int unknown = 0;
            MPlayerScoreType scores[2];
            char *token = strtok(copy + 2, ",");
            if (token != 0) {
                index = atol(token);
            }
            token = strtok(0, ",");
            if (token != 0) {
                time = atol(token);
            }
            token = strtok(0, ",");
            if (token != 0) {
                unknown = atol(token);
            }
            token = strtok(0, ",");
            if (token != 0) {
                scores[1].Kills = atol(token);
            }
            token = strtok(0, ",");
            if (token != 0) {
                scores[1].Built = atol(token);
            }
            token = strtok(0, ",");
            if (token != 0) {
                scores[1].Lost = atol(token);
            }
            token = strtok(0, ",");
            if (token != 0) {
                scores[1].Score = atol(token);
            }
            token = strtok(0, ",");
            if (token != 0) {
                scores[0].Kills = atol(token);
            }
            token = strtok(0, ",");
            if (token != 0) {
                scores[0].Built = atol(token);
            }
            token = strtok(0, ",");
            if (token != 0) {
                scores[0].Lost = atol(token);
            }
            token = strtok(0, ",");
            if (token != 0) {
                scores[0].Score = atol(token);
            }
            for (int i = 0; i < 10; i++) {
                token = strtok(0, ",");
                if (token == 0) {
                    break;
                }
                int value = atol(token);
                if (value < 0) {
                    break;
                }
                list.Add(value);
            }
            char *first = strtok(0, ",");
            char *second = strtok(0, ",");
            for (int mode = 0; mode <= MultiplayerGameMode::unknown_mode_count_5b6f80(); mode++) {
                if (MultiplayerGameMode::unknown_find_mode_5b6fe0(mode) != 0) {
                    Debug_Printf("Setting Campaign info for Mode %d: %s\n", mode,
                        Wide_To_Narrow_006fa120(MultiplayerGameMode::unknown_find_mode_5b6fe0(mode)->unknown_name_20.unknown_text_770430()));
                    if (MultiplayerGameMode::unknown_find_mode_5b6fe0(mode)->unknown_slot18(index, list, first, second)) {
                        MultiplayerGameMode::unknown_find_mode_5b6fe0(mode)->unknown_slot19(time, unknown, scores);
                    }
                }
            }
        } else {
            Debug_Printf("@@@@ Decoding cooperative campaign game options %s\n", text);
            for (int mode = 0; mode <= MultiplayerGameMode::unknown_mode_count_5b6f80(); mode++) {
                if (MultiplayerGameMode::unknown_find_mode_5b6fe0(mode) != 0) {
                    Debug_Printf("Handling Campaign info for Mode %d: %s\n", mode,
                        Wide_To_Narrow_006fa120(MultiplayerGameMode::unknown_find_mode_5b6fe0(mode)->unknown_name_20.unknown_text_770430()));
                    MultiplayerGameMode::unknown_find_mode_5b6fe0(mode)->unknown_slot25(copy[1] - '0', copy + 2);
                }
            }
        }
        free(copy);
        return false;
    }
    if (copy[0] == 'L') {
        WideString names = Wstring_To_Wide(Wstring(text + 1));
        Set_Slot_Names_005c8dd0(names.Get());
        HWND dialog = WS_Find_Dialog_00735940(0xC9);
        if (dialog != 0) {
            int slot = Find_First_Player_Slot_005c8c70();
            if (slot == -1) {
                Leave_Channel_With_Message_0074c5f0(dialog);
            } else if (slot != unknown_lobby_a3fa94) {
                int id = Player_Side_Combo_ID_4d1350(slot);
                if (unknown_player_slots_a3fa98[slot]->Team == -1) {
                    Fill_Observer_Combo_4d1640(dialog, id);
                    SendMessageA(GetDlgItem(dialog, Slot_Control_Id_005c97a0(slot)), 0x4F1, 0, 1);
                    int color = Player_Color_Combo_ID_4d1cb0(slot);
                    Fill_Random_Color_Combo_4d2250(dialog, color);
                    Select_Color_Combo_4d2480(dialog, color, 8);
                } else {
                    id = Player_Side_Combo_ID_4d1350(slot);
                    // NATIVE (Scout1274): the original (0x005C4A13) fills the side combo only when a game mode is set and its slot 1 is false
                    if (unknown_session_a3d298.Mode != 0 && !unknown_session_a3d298.Mode->unknown_slot1()) {
                        Fill_Side_Combo_4d1560(dialog, id);
                    }
                    SendMessageA(GetDlgItem(dialog, Slot_Control_Id_005c97a0(slot)), 0x4F1, 0, 0);
                    int color = Player_Color_Combo_ID_4d1cb0(slot);
                    Fill_Color_Combo_4d2080(dialog, color);
                    Select_Color_Combo_4d2480(dialog, color, -2);
                }
                slot = Find_First_Player_Slot_005c8c70();
                unknown_lobby_a3fa94 = slot;
                for (int i = 0; i < 8; i++) {
                    EnableWindow(GetDlgItem(dialog, Slot_Control_Id_005c97a0(i)), FALSE);
                    Unknown_005c9b90(dialog, i, false);
                }
                EnableWindow(GetDlgItem(dialog, Slot_Control_Id_005c97a0(slot)), TRUE);
                Unknown_005c9b90(dialog, slot, true);
                Select_Player_Side_4d1820(dialog, unknown_lobby_a3fa94);
                Select_Player_Color_4d23c0(dialog, unknown_lobby_a3fa94);
            }
        }
        free(copy);
        return false;
    }
    if (copy[0] == 'G') {
        if (WS_Find_Dialog_00735940(0xC9) != 0) {
            unknown_lobby_flag_a3fab8 = true;
            if (unknown_chat_a72cfc != 0) {
                Unknown_0075c980();
                free(copy);
                return false;
            }
            Unknown_005c28e0();
        }
        free(copy);
        return false;
    }
    if (copy[0] == 'K') {
        if (WS_Find_Dialog_00735940(0xC2) != 0 || WS_Find_Dialog_00735940(0xBC) != 0) {
            int state;
            switch (copy[1]) {
            case '1':
                state = 1;
                break;
            case '2':
                state = 2;
                break;
            default:
                state = 0;
                break;
            }
            Set_Player_Map_State_005c3f00(sender, state);
        }
        free(copy);
        return false;
    }
    if (copy[0] == 'P') {
        Debug_Printf("Got Ping game options string. %s\n", copy);
        char const *p = copy + 3;
        if (unknown_game_mode_a3d298 == 4) {
            while (*p != 0) {
                char name[64];
                char number[68];
                int i = 0;
                while (*p != 0 && *p != ',') {
                    name[i++] = *p++;
                }
                name[i] = 0;
                i = 0;
                if (*p != 0) {
                    p++;
                }
                while (*p != 0 && *p != ',') {
                    number[i++] = *p++;
                }
                number[i] = 0;
                int ping = atol(number);
                if (*p != 0) {
                    p++;
                }
                Store_User_Ping_0075b110(name, ping);
            }
        }
        free(copy);
        return false;
    }

    int dialog_id = Current_Dialog_Id_00735b20();
    if (dialog_id != 0xBC && dialog_id != 0xBD) {
        SendDlgItemMessageA(Find_Lobby_Window_005c2c40(), 0x53E, 0x49D, 0, 1);
    }
    Debug_Printf("Decoding game options %s\n", text);
    char *token = strtok(copy, ",");
    if (token != 0) {
        unknown_a40d3c = atol(token);
    }
    token = strtok(0, ",");
    if (token != 0) {
        *(int *)&Options = atol(token);
    }
    token = strtok(0, ",");
    if (token != 0) {
        unknown_session_a3d298.GameSpeed_30 = atol(token);
    }
    token = strtok(0, ",");
    if (token != 0) {
        unknown_session_a3d298.unknown_24 = atol(token);
    }
    token = strtok(0, ",");
    if (token != 0) {
        unknown_session_a3d298.UnitCount = atol(token);
    }
    token = strtok(0, ",");
    if (token != 0) {
        unknown_session_a3d298.unknown_2a = atol(token) != 0;
    }
    token = strtok(0, ",");
    if (token != 0) {
        unknown_session_a3d298.unknown_2b = atol(token) != 0;
    }
    token = strtok(0, ",");
    if (token != 0) {
        unknown_session_a3d298.unknown_2c = atol(token) != 0;
    }
    token = strtok(0, ",");
    if (token != 0) {
        unknown_session_a3d298.unknown_a8 = atoi(token) != 0;
    }
    token = strtok(0, ",");
    if (token != 0) {
        unknown_session_a3d298.unknown_crates_29 = atoi(token) != 0;
    }
    token = strtok(0, ",");
    if (token != 0) {
        unknown_session_a3d298.GameModeIndex = atol(token);
    }
    bool same = true;
    token = strtok(0, ",");
    WideString scenario_name(unknown_game_mode_a3d298 == 4 && Unknown_0073bdb0()
        ? WideString(unknown_empty_wide_839d4c) : Wide_From_7_Bit(Wstring(token)));
    int old_count = unknown_session_a3d298.unknown_884;
    token = strtok(0, ",");
    if (token != 0) {
        unknown_session_a3d298.unknown_884 = atoi(token);
    }
    bool old_official = unknown_session_a3d298.ScenarioIsOfficial_880;
    token = strtok(0, ",");
    if (token != 0) {
        unknown_session_a3d298.ScenarioIsOfficial_880 = atoi(token) != 0;
    }
    int old_length = unknown_session_a3d298.ScenarioFileLength_87c;
    token = strtok(0, ",");
    if (token != 0) {
        unknown_session_a3d298.ScenarioFileLength_87c = atoi(token);
    }
    token = strtok(0, ",");
    if (token != 0) {
        if (_stricmp(unknown_session_a3d298.ScenarioFileName_658, token) != 0) {
            same = false;
        }
        strncpy(unknown_session_a3d298.ScenarioFileName_658, token, 0x202);
        strcpy(unknown_id_source_a3d290->FileName, unknown_session_a3d298.ScenarioFileName_658);
    }
    char *digest = strtok(0, ":");
    if (digest != 0) {
        if (strcmp(unknown_session_a3d298.ScenarioDigest_85a, digest) != 0) {
            same = false;
        }
        strncpy(unknown_session_a3d298.ScenarioDigest_85a, digest, 0x21);
    }
    MultiplayerGameMode *mode = MultiplayerGameMode::unknown_find_mode_5b6fe0(unknown_session_a3d298.GameModeIndex);
    if (dialog_id == 0xBD || dialog_id == 0xC9) {
        Change_Slot_Count_005cb300(Find_Lobby_Window_005c2c40(), old_count, unknown_session_a3d298.unknown_884,
            (UnknownSessionA3D29C *)mode);
    }
    bool found = true;
    if (!same || wcslen(unknown_scenario_name_a3d342) == 0) {
        found = false;
        same = false;
        for (int i = 0; i < unknown_session_a3d298.Scenarios.Count(); i++) {
            MultiMission *scenario = unknown_session_a3d298.Scenarios[i];
            if (_stricmp(unknown_session_a3d298.ScenarioFileName_658, scenario->unknown_58) == 0) {
                CCFileClass file(scenario->unknown_58);
                if (file.Is_Available(false) && file.Size() == unknown_session_a3d298.ScenarioFileLength_87c) {
                    INIClass ini;
                    ini.Load(file, false);
                    char buffer[32];
                    ini.Get_String("Digest", "1", "No digest here mate. Nope.", buffer, sizeof(buffer));
                    if (unknown_session_a3d298.ScenarioDigest_85a[0] == 0
                        || strcmp(unknown_session_a3d298.ScenarioDigest_85a, buffer) == 0) {
                        wcscpy(unknown_scenario_name_a3d342, scenario->Description);
                        found = true;
                    }
                }
            }
        }
        if (!found && scenario_name.Length() > 0) {
            wcscpy(unknown_scenario_name_a3d342, scenario_name.Get());
        }
        if (unknown_session_a3d298.Mode != 0) {
            unknown_session_a3d298.Mode->unknown_slot9();
        }
    }
    mode = MultiplayerGameMode::unknown_find_mode_5b6fe0(unknown_session_a3d298.GameModeIndex);
    if (mode != unknown_session_a3d298.Mode) {
        if (unknown_session_a3d298.Mode != 0) {
            unknown_session_a3d298.Mode->unknown_slot33();
        }
        if (mode != 0) {
            mode->unknown_slot8();
        }
    }
    unknown_session_a3d298.Mode = mode;
    if (mode != 0 && !same) {
        mode->UnfixAlliances();
    }
    HWND dialog = Find_Lobby_Window_005c2c40();
    SendMessageA(GetDlgItem(dialog, 0x6EC), 0x4B2, 0, (LPARAM)mode->unknown_name_20.unknown_text_770430());
    SendMessageA(GetDlgItem(dialog, 0x5A8), 0x4B2, 0, (LPARAM)unknown_scenario_name_a3d342);
    unknown_id_source_a3d290->TechLevel = -1;
    Frame = 0;
    unknown_session_a3d2ac = 2;
    if (old_official != unknown_session_a3d298.ScenarioIsOfficial_880
        || old_length != unknown_session_a3d298.ScenarioFileLength_87c) {
        same = false;
    }
    if (!same) {
        Debug_Printf("Not same scenario...\n");
        if (unknown_session_a3d298.ScenarioIsOfficial_880 && !found
            && _stricmp(unknown_session_a3d298.ScenarioFileName_658, "RandMap.Sed") != 0) {
            if (Find_Lobby_Window_005c2c40() != 0) {
                Unknown_005C2C80(-1, StringTable::Load_String("GUI:JoinerNoMap", 0, "D:\\Ra2\\netshare.cpp", 0x71A),
                    unknown_scenario_name_a3d342);
            }
            unknown_a72d52 = false;
            EnableWindow(GetDlgItem(Find_Lobby_Window_005c2c40(), 0x59F), FALSE);
            InvalidateRect(GetDlgItem(Find_Lobby_Window_005c2c40(), 0x59F), 0, FALSE);
            Send_Game_Options_005c3c60("K0");
        } else {
            unknown_a72d52 = true;
            EnableWindow(GetDlgItem(Find_Lobby_Window_005c2c40(), 0x59F), TRUE);
            InvalidateRect(GetDlgItem(Find_Lobby_Window_005c2c40(), 0x59F), 0, FALSE);
            if (!unknown_session_a3d298.ScenarioIsOfficial_880 && !found
                && _stricmp(unknown_session_a3d298.ScenarioFileName_658, "RandMap.Sed") != 0) {
                if (Find_Lobby_Window_005c2c40() != 0) {
                    Unknown_005C2C80(-1, StringTable::Load_String("GUI:JoinerMapTransfer", 0, "D:\\Ra2\\netshare.cpp", 0x72A),
                        unknown_scenario_name_a3d342);
                }
                Send_Game_Options_005c3c60("K2");
            } else {
                Send_Game_Options_005c3c60("K1");
            }
        }
        Unknown_005c7570(Find_Lobby_Window_005c2c40());
    }
    if (digest == 0) {
        Unknown_005C2C80(-1, L"Gameopt parse failed");
    }
    {
        unsigned int address = 0;
        for (char *name = strtok(0, ",:"); name != 0; name = strtok(0, ",:")) {
            if (strcmp(name, "@") == 0) {
                break;
            }
            token = strtok(0, ",:");
            if (token == 0) {
                break;
            }
            int country = atol(token);
            token = strtok(0, ",:");
            if (token == 0) {
                break;
            }
            int color = atol(token);
            token = strtok(0, ",:");
            if (token == 0) {
                break;
            }
            sscanf(token, "%X", &address);
            int i = 8;
            if (address != 0 && unknown_game_mode_a3d298 == 4) {
                for (i = 0; i < 8; i++) {
                    if (_stricmp(name, Wide_To_Narrow_006fa120(unknown_player_slots_a3fa98[i]->Name)) == 0) {
                        NetNumType net;
                        NetNodeType node;
                        memset(node, 0, sizeof(node));
                        memcpy(node, &address, sizeof(address));
                        memset(net, 0, sizeof(net));
                        unknown_slot_addresses_a3fbd4[i].Set_Address(net, node, 0);
                        token = strtok(0, ",:");
                        if (token != 0) {
                            int value;
                            sscanf(token, "%X", &value);
                            unknown_slot_a3fc34[i] = value;
                            token = strtok(0, ",:");
                            if (token != 0) {
                                int value2;
                                sscanf(token, "%X", &value2);
                                unknown_slot_a40238[i] = value2;
                            }
                        }
                        break;
                    }
                }
            }
            if (i == 8) {
                strtok(0, ",:");
                strtok(0, ",:");
            }
            unknown_a72d08 = 1;
            if (_stricmp(name, unknown_session_a3d298.Handle) == 0 && color != unknown_session_a3d298.unknown_12c) {
                if (unknown_a72d08 == 0) {
                    Unknown_005C2C80(-1, StringTable::Load_String("TXT_COLOR_IN_USE", 0, "D:\\Ra2\\netshare.cpp", 0x7A2));
                }
                unknown_session_a3d298.unknown_12c = color;
            }
            if (_stricmp(name, unknown_session_a3d298.Handle) == 0) {
                unknown_a72d08 = 0;
            }
            if (unknown_chat_a72cfc != 0 && Set_Channel_Player_Options_0073d550(Wstring(name), country, color) != 0) {
                changed = true;
            } else if (unknown_chat_a72cfc == 0 && Set_Slot_Country_Color_005bccf0(name, country, color) != 0) {
                changed = true;
            }
            address = 0;
        }
    }
    {
        char before[64];
        before[0] = 0;
        for (int i = 0; i < 8; i++) {
            char number[12];
            sprintf(number, "%d,", unknown_session_a3d298.AICountry[i]);
            strcat(before, number);
        }
        int count = 0;
        for (int j = 0; j < 8; j++) {
            token = strtok(0, ",:");
            if (token == 0) {
                break;
            }
            unknown_session_a3d298.AIDifficulty[j] = atol(token);
            token = strtok(0, ",:");
            if (token == 0) {
                break;
            }
            unknown_session_a3d298.AICountry[j] = atol(token);
            token = strtok(0, ",:");
            if (token == 0) {
                break;
            }
            unknown_session_a3d298.AIColor[j] = atol(token);
            if (unknown_session_a3d298.AICountry[j] != -1) {
                count++;
            }
        }
        unknown_session_a3d298.AICount = count;
        char after[64];
        after[0] = 0;
        for (int k = 0; k < 8; k++) {
            char number[12];
            sprintf(number, "%d,", unknown_session_a3d298.AICountry[k]);
            strcat(after, number);
        }
        if (strcmp(before, after) != 0) {
            changed = true;
        }
    }
    free(copy);
    if (unknown_game_mode_a3d298 != 4 || !Unknown_0073bdb0()) {
        Unknown_Fill_Options_Dialog_005c3550(Find_Lobby_Window_005c2c40(), 0);
    }
    if (unknown_old_speed_7e6dd8 != unknown_session_a3d298.GameSpeed_30) {
        changed = true;
    }
    if (unknown_old_money_7e6ddc != unknown_session_a3d298.unknown_24) {
        changed = true;
    }
    if (unknown_old_units_7e6de0 != unknown_session_a3d298.UnitCount) {
        changed = true;
    }
    if (unknown_old_2a_a72d53 != unknown_session_a3d298.unknown_2a) {
        changed = true;
    }
    if (unknown_old_2b_a72d54 != unknown_session_a3d298.unknown_2b) {
        changed = true;
    }
    if (unknown_old_2c_a72d55 != unknown_session_a3d298.unknown_2c) {
        changed = true;
    }
    if (unknown_old_a8_a72d56 != unknown_session_a3d298.unknown_a8) {
        changed = true;
    }
    if (unknown_old_crates_a72d57 != unknown_session_a3d298.unknown_crates_29) {
        changed = true;
    }
    if (!same || changed) {
        int result;
        if (unknown_chat_a72cfc != 0) {
            result = Unknown_0073d3f0(Wstring(0));
        } else {
            result = Unknown_Find_Player_By_Name_005bcc60(0);
        }
        unknown_lobby_flag_a3fab8 = false;
        if (unknown_chat_a72cfc != 0) {
            Unknown_0075ca00();
        } else {
            Unknown_005c2940();
        }
        if (result == 1) {
            if (unknown_chat_a72cfc != 0) {
                Unknown_0073d150(Wstring(0), 0);
            } else {
                Set_Player_Value_005bcbc0(0, 0);
            }
            char buffer[64];
            sprintf(buffer, "A0");
            Send_Game_Options_005c3c60(buffer);
            if (unknown_a72d52) {
                EnableWindow(GetDlgItem(Find_Lobby_Window_005c2c40(), 0x59F), TRUE);
            } else {
                EnableWindow(GetDlgItem(Find_Lobby_Window_005c2c40(), 0x59F), FALSE);
            }
            InvalidateRect(GetDlgItem(Find_Lobby_Window_005c2c40(), 0x59F), 0, FALSE);
        } else if (!IsWindowEnabled(GetDlgItem(Find_Lobby_Window_005c2c40(), 0x59F)) && unknown_a72d52) {
            EnableWindow(GetDlgItem(Find_Lobby_Window_005c2c40(), 0x59F), TRUE);
            InvalidateRect(GetDlgItem(Find_Lobby_Window_005c2c40(), 0x59F), 0, FALSE);
        }
    }
    if (dialog_id != 0xBC && dialog_id != 0xBD) {
        SendDlgItemMessageA(Find_Lobby_Window_005c2c40(), 0x53E, 0x49D, 0, 0);
    }
    if (unknown_chat_a72cfc != 0) {
        unknown_call_73fee0_default();
    } else {
        Unknown_005bbdc0();
    }
    unknown_old_speed_7e6dd8 = unknown_session_a3d298.GameSpeed_30;
    unknown_old_money_7e6ddc = unknown_session_a3d298.unknown_24;
    unknown_old_units_7e6de0 = unknown_session_a3d298.UnitCount;
    unknown_old_2a_a72d53 = unknown_session_a3d298.unknown_2a;
    unknown_old_2b_a72d54 = unknown_session_a3d298.unknown_2b;
    unknown_old_2c_a72d55 = unknown_session_a3d298.unknown_2c;
    unknown_old_a8_a72d56 = unknown_session_a3d298.unknown_a8;
    unknown_old_crates_a72d57 = unknown_session_a3d298.unknown_crates_29;
    return !same;
}
#endif

// void __fastcall Restore_List_Selection_005c6310(HWND list, LobbyNameSet *names) (0x005C6310)
// Selects the list rows whose text is in the name set: every such row of a multiple-selection list,
// or for a single-selection list the row of one name taken out of the set.

void __fastcall Restore_List_Selection_005c6310(HWND list, LobbyNameSet *names)
{
    wchar_t text[0x400];
    char narrow[0x400];
    int count = SendMessageA(list, 0x18B, 0, 0);
    WideString name;
    if (count != 0) {
        LONG style = GetWindowLongA(list, GWL_STYLE);
        if (style & 8) {
            for (int i = 0; i < count; i++) {
                Get_List_Row_Text(list, i, text, narrow);
                name = text;
                if (names->Contains(name)) {
                    SendMessageA(list, 0x185, 1, i);
                }
            }
        } else if (!(style & 0x4000)) {
            bool value;
            if (names->Remove_Any(name, value)) {
                wcscpy(text, name.Get());
                Select_List_Row_Text(list, text);
            }
        }
    }
}


// int Lobby_Slot_Limit_005c6520() (0x005C6520)

int Lobby_Slot_Limit_005c6520()
{
    if (unknown_game_mode_a3d298 == 4) {
        if (unknown_73c370() && !unknown_73c3a0()) {
            return 2;
        }
        return 4;
    }
    return 8;
}


// int Lobby_Slot_Limit_Plus_One_005c6550() (0x005C6550)

int Lobby_Slot_Limit_Plus_One_005c6550()
{
    int limit = Lobby_Slot_Limit_005c6520();
    if (unknown_game_mode_a3d298 == 4) {
        if (limit >= 4) {
            return limit;
        }
    } else if (limit >= 8) {
        return limit;
    }
    return limit + 1;
}


// int __fastcall Scenario_Slot_Limit_005c65a0(int scenario) (0x005C65A0)

int __fastcall Scenario_Slot_Limit_005c65a0(int scenario)
{
    if (scenario == -1) {
        return 8;
    }
    int limit = Unknown_005c6600(scenario);
    if (unknown_session_a3d29c != 0) {
        int mode_limit = unknown_session_a3d29c->unknown_slot32();
        if (mode_limit != -1 && mode_limit < limit) {
            limit = mode_limit;
        }
    }
    if (unknown_game_mode_a3d298 == 4) {
        if (unknown_73c3a0()) {
            limit &= ~1;
        } else if (unknown_73c370()) {
            limit = 2;
        }
    }
    return limit;
}


// int __fastcall Unknown_005c6600(int scenario) (0x005C6600)
// The player count of a multiplayer map (by unknown_multi_missions_a3d8d8 index): its [Waypoints]
// 0..7 that are set, else [RandomMap] NumPlayers; 0 for no map or when the INI does not load.

int __fastcall Unknown_005c6600(int scenario)
{
    if (scenario < 0 || scenario >= unknown_multi_missions_a3d8d8.Count()) {
        return 0;
    }
    char name[132];
    strcpy(name, unknown_multi_missions_a3d8d8[scenario]->unknown_58);
    CCFileClass file(name);
    INIClass ini;
    if (ini.Load(file, false) == 0) {
        return 0;
    }
    int count = 0;
    for (int i = 0; i < 8; i++) {
        char key[32];
        sprintf(key, "%d", i);
        if (ini.Get_Int("Waypoints", key, -1) != -1) {
            count++;
        }
    }
    if (count == 0) {
        count = ini.Get_Int("RandomMap", "NumPlayers", 0);
    }
    return count;
}


// int Selected_Campaign_005c67a0() (0x005C67A0)
// The selected row of the lobby campaign list (0x553 in the lobby dialog; LB_GETCURSEL).

int Selected_Campaign_005c67a0()
{
    return SendDlgItemMessageA(unknown_lobby_dialog_a728e0, 0x553, 0x188, 0, 0);
}


// bool Lobby_Dialog_Update_005c67c0() (0x005C67C0)
// The lobby campaign dialog's update callback: a newly selected map row selects its map (a random
// map loads its preview), the selected row's text is kept, then the game mode's service runs.

bool Lobby_Dialog_Update_005c67c0()
{
    int row = Selected_Campaign_005c67a0();
    if (row != -1) {
        MultiMission *mission = (MultiMission *)SendDlgItemMessageA(unknown_lobby_dialog_a728e0, 0x553, 0x199, row, 0);
        int found = -1;
        for (int i = 0; i < unknown_multi_missions_a3d8d8.Count(); i++) {
            if (mission == unknown_multi_missions_a3d8d8[i]) {
                found = i;
                break;
            }
        }
        if (found != -1 && found != unknown_lobby_map_a728d0) {
            Select_Scenario_005c7c30(found);
            if (unknown_multi_missions_a3d8d8[found]->Unknown_006720C0()) {
                delete unknown_dialog_preview_a72ce8;
                unknown_dialog_preview_a72ce8 = new MapPreviewClass;
                unknown_dialog_preview_a72ce8->Read_PCX_Preview("RandMap.img");
                if (unknown_dialog_preview_a72ce8->unknown_00 == 0) {
                    Unknown_005c7570(unknown_lobby_dialog_a728e0);
                }
                InvalidateRect(unknown_lobby_dialog_a728e0, 0, 0);
            } else {
                Unknown_005c7570(unknown_lobby_dialog_a728e0);
            }
            unknown_lobby_map_a728d0 = found;
            unknown_session_a3d2b4 = unknown_lobby_map_a72c80;
            Select_Scenario_005c7c30(unknown_session_a3d2b4);
        }
    }
    HWND dialog = unknown_lobby_dialog_a728e0;
    row = Selected_Campaign_005c67a0();
    wchar_t const *name;
    if (row != -1 && (name = (wchar_t const *)SendDlgItemMessageA(dialog, 0x553, 0x199, row, 0)) != 0) {
        wcscpy(unknown_lobby_map_name_a72a68, name);
    } else {
        unknown_lobby_map_name_a72a68[0] = 0;
    }
    if (unknown_game_mode_a3d298 == 3) {
        return Unknown_005bbd10();
    }
    if (unknown_game_mode_a3d298 == 4) {
        return Unknown_0073e100();
    }
    if (unknown_game_mode_a3d298 == 1 || unknown_game_mode_a3d298 == 2) {
        return Unknown_0059c880();
    }
    Service_Game_IO_00481fb0();
    return false;
}


// int __fastcall Open_Lobby_Dialog_005c6960(HWND parent) (0x005C6960)

int __fastcall Open_Lobby_Dialog_005c6960(HWND parent)
{
    unknown_5159b0(0, true);
    unknown_lobby_dialog_a728e0 = (HWND)unknown_create_dialog_735700(ProgramInstance, 0x6B, parent, Lobby_Dialog_Proc_005c69e0, 0);
    unknown_show_dialog_5ff7e0(unknown_lobby_dialog_a728e0, 0x6B);
    SendMessageA(unknown_lobby_dialog_a728e0, 0x4A9, 0, 1);
    ShowWindow(unknown_lobby_dialog_a728e0, 1);
    return Unknown_007359e0(unknown_lobby_dialog_a728e0, Lobby_Dialog_Update_005c67c0, 0, 1, 1);
}


// Not exact (RA2_NON_MATCHING): Lobby_Dialog_Proc (Choose Map dialog) at 0x005C69E0, best score 93.62, 2026-11-13; differs: same instruction count (643), shape 97.82: instruction choice and registers differ
#ifdef RA2_NON_MATCHING
// NON-EXACT DRAFT 0x5C69E0 Lobby_Dialog_Proc (the Choose Map dialog's procedure)
// entry 0x5C69E0; exact-match score 93.62 (Scout1005 HEAD rescore of codex3's 94.87 form)
// origin lane: Scout1005 (build/class-work/Scout1005/keep/h5c69e0/best-9362-head.cpp), worker codex3
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1133 (native/drafts.json, M16). Not original bytes.
// The unit-local container tables are identified independently by MSVC RTTI.
// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2025 Electronic Arts Inc.
// Copyright 2026 OpenTS contributors
// Adapted from TibSun code/netshare.cpp Scenario_DlgProc; RA2 mode-selection behavior from original CFG.
// EA GPLv3 Section 7 additional terms apply; see THIRD_PARTY.md.
extern bool unknown_map_list_busy_a72d58;
extern bool unknown_map_mode_warning_a72d59;
bool __fastcall End_Dialog_007357e0(HWND dialog, int result);
int __fastcall Make_Random_Map_005c84e0();
bool __fastcall Accept_Map_Selection_005c71f0(HWND dialog);
int __fastcall unknown_select_list_item_by_data_005c7160(HWND dialog, int data);   // NATIVE: pointer item data (port/fixups.json)
void __fastcall unknown_dialog_5e5f20(HWND dialog);
namespace OwnerDraw {
void __fastcall unknown_draw_dialog_back_5fee70(HWND window);
void __fastcall unknown_5ff380(unsigned int key);
}

// Gather missions separately from replacing the control's visible list.
static inline void Collect_Compatible_Maps(MultiplayerGameMode *mode, DynamicVectorClass<MultiMission *> &maps)
{
    int count = unknown_multi_missions_a3d8d8.Count();
    for (int index = 0; index < count; ++index) {
        MultiMission *map = unknown_multi_missions_a3d8d8[index];
        if (mode->unknown_5b7470(map)) maps.Add(map);
    }
}

// Rebuild the visible map list from the missions accepted by the selected mode.
static inline void Populate_Compatible_Map_List(HWND window, MultiplayerGameMode *mode)
{
    DynamicVectorClass<MultiMission *> maps;
    Collect_Compatible_Maps(mode, maps);
    HWND list = GetDlgItem(window, 0x553);
    mode->DrawTeamDropdown(list, &maps, 0);
}

// Refill the maps and update the random-map control for the new mode.
static inline void Refresh_Mode_Map_Controls(HWND window, MultiplayerGameMode *mode)
{
                    {
                        DynamicVectorClass<MultiMission *> maps;
                        int count = unknown_multi_missions_a3d8d8.Count();
                        for (int index = 0; index < count; ++index) {
                            MultiMission *map = unknown_multi_missions_a3d8d8[index];
                            if (mode->unknown_5b7470(map)) maps.Add(map);
                        }
                        HWND list = GetDlgItem(window, 0x553);
                        mode->DrawTeamDropdown(list, &maps, 0);
                    }
                    bool can_random = mode->IsAIAllowed();
                    HWND random_button = GetDlgItem(window, 0x583);
                    EnableWindow(random_button, can_random);
}

// Validation and refresh when the game-mode list changes.
static inline void Change_Lobby_Game_Mode(HWND window, unsigned wparam, wchar_t const *selected_name)
{
            if (!unknown_map_list_busy_a72d58 && (unsigned short)(wparam >> 16) == 1) {
                int selected = SendDlgItemMessageA(window, 0x6eb, 0x188, 0, 0);
                if (selected == -1) return;
                MultiplayerGameMode *mode = (MultiplayerGameMode *)SendDlgItemMessageA(window, 0x6eb, 0x199, selected, 0);
                if (!mode) return;
                WideString reason;
                if (mode->unknown_slot3(&reason)) {
                    mode->AssignTeams();
                    Refresh_Mode_Map_Controls(window, mode);
                    if (selected_name) {
                    int found = 0;
                    int count = SendDlgItemMessageA(window, 0x553, 0x18b, 0, 0);
                    for (int i = 0; i < count; ++i) {
                        wchar_t const *name = (wchar_t const *)SendDlgItemMessageA(window, 0x553, 0x199, i, 0);
                        if (Compare_Wide_007837d3(name, selected_name) == 0) found = i;
                    }
                    SendDlgItemMessageA(window, 0x553, 0x186, found, 0);
                    SendDlgItemMessageA(window, 0x553, 0x197, found, 0);
                    }
                    HWND dialog = unknown_lobby_dialog_a728e0;
                    selected = Selected_Campaign_005c67a0();
                    wchar_t const *name;
                    if (selected != -1 && (name = (wchar_t const *)SendDlgItemMessageA(dialog, 0x553, 0x199, selected, 0)) != 0) wcscpy(unknown_lobby_map_name_a72a68, name);
                    else unknown_lobby_map_name_a72a68[0] = 0;
                    unknown_map_mode_warning_a72d59 = false;
                } else {
                    if (!unknown_map_mode_warning_a72d59) {
                        Message_Box_005c3230(reason.Get(), 0, 0, false);
                        unknown_map_mode_warning_a72d59 = true;
                    }
                    SendDlgItemMessageA(window, 0x6eb, 0x186, 0, 0);
                }
            }
}

long __stdcall Lobby_Dialog_Proc_005c69e0(HWND window, unsigned message, unsigned wparam, long lparam)
{
    switch (message) {
    case 0x2b:
        Unknown_005FE380(lparam);
        return 1;
    case 0xf:
        OwnerDraw::unknown_draw_dialog_back_5fee70(window);
        if (unknown_dialog_preview_a72ce8) unknown_dialog_preview_a72ce8->Blit_Preview(window);
        unknown_dialog_5e5f20(window);
        ValidateRect(window, 0);
        return 0;
    case 0x14:
        return 1;
    case 0x82:
        OwnerDraw::unknown_5ff380((unsigned int)window);
        return 0;
    case 0x111:
        switch ((unsigned short)wparam) {
        case 0x6c5:
            Accept_Map_Selection_005c71f0(window);
            return 0;
        case 0x583:
            {
                OwnerDraw::unknown_5e5fc0(window);
                ShowWindow(window, 0);
                int index = Make_Random_Map_005c84e0();
                if (index != -1) {
                    int selected = SendDlgItemMessageA(window, 0x6eb, 0x188, 0, 0);
                    if (selected != -1) {
                        MultiplayerGameMode *mode = (MultiplayerGameMode *)SendDlgItemMessageA(window, 0x6eb, 0x199, selected, 0);
                        if (mode) {
                            Populate_Compatible_Map_List(window, mode);
                        }
                    }
                    if (index < unknown_multi_missions_a3d8d8.Count()) unknown_select_list_item_by_data_005c7160(window, (int)unknown_multi_missions_a3d8d8[index]);
                    Select_Scenario_005c7c30(index);
                    if (!unknown_dialog_preview_a72ce8->unknown_00) Unknown_005c7570(window);
                    unknown_session_a3d2b4 = unknown_lobby_map_a72c80;
                    Select_Scenario_005c7c30(unknown_session_a3d2b4);
                    bool accepted = Accept_Map_Selection_005c71f0(window);
                    unknown_5159b0(0, true);
                    if (accepted) return 0;
                }
                OwnerDraw::unknown_show_dialog_5e61b0(window);
                ShowWindow(window, 5);
            }
            return 0;
        case 0x5c0:
            End_Dialog_007357e0(window, 2);
            return 0;
        case 0x6eb:
            Change_Lobby_Game_Mode(window, wparam, unknown_lobby_map_name_a72a68);
            return 0;
        }
        return 0;
    case 0x497:
        {
            unknown_map_list_busy_a72d58 = true;
            MultiplayerGameMode *mode = MultiplayerGameMode::unknown_find_mode_5b6fe0(unknown_session_a3d298.GameModeIndex);
            if (!mode) mode = MultiplayerGameMode::unknown_first_mode_5b6ec0();
            HWND mode_list = GetDlgItem(window, 0x6eb);
            MultiplayerGameMode::unknown_fill_list_5b71e0(mode_list, mode->Get_ID());
            if (mode) {
                Populate_Compatible_Map_List(window, mode);
            }
            bool can_random = mode->IsAIAllowed();
            HWND random_button = GetDlgItem(window, 0x583);
                    EnableWindow(random_button, can_random);
            if (unknown_session_a3d2b4 != -1 && unknown_session_a3d2b4 < unknown_multi_missions_a3d8d8.Count()) {
                MultiMission *map = unknown_multi_missions_a3d8d8[unknown_session_a3d2b4];
                if (map) {
                    int found = 0;
                    int count = SendDlgItemMessageA(window, 0x553, 0x18b, 0, 0);
                    for (int i = 0; i < count; ++i) {
                        if ((MultiMission *)SendDlgItemMessageA(window, 0x553, 0x199, i, 0) == map) found = i;
                    }
                    SendDlgItemMessageA(window, 0x553, 0x186, found, 0);
                    SendDlgItemMessageA(window, 0x553, 0x197, found, 0);
                }
            }
            unknown_lobby_map_a72c80 = unknown_session_a3d2b4;
            unknown_lobby_map_a728d0 = -1;
            unknown_map_list_busy_a72d58 = false;
        }
        return 0;
    case 0x4e9:
        {
            OwnerDraw::UnknownTextRequest *request = (OwnerDraw::UnknownTextRequest *)lparam;
            int control = GetDlgCtrlID((HWND)request->unknown_00);
            request->text = (unsigned short const *)0;
            if (control == 0x6eb && request->unknown_04 != -1) {
                MultiplayerGameMode *mode = (MultiplayerGameMode *)SendDlgItemMessageA(window, 0x6eb, 0x199, request->unknown_04, 0);
                if (mode) request->text = mode->unknown_name_24.unknown_text_770430();
            }
        }
        return 1;
    case 0x4ec:
        Unknown_005e8710(window);
        return 0;
    }
    return 0;
}
#endif

// bool Accept_Map_Selection_005c71f0(HWND dialog) (0x005C71F0)
// Takes the map (list 0x553) and the game mode (list 0x6EB) the lobby dialog selects. When the
// map has fewer players than the lobby holds, the game mode's test (0x005BC430, 0x0075CA30 or
// Skirmish_Has_Computer_After) may ask first (GUI:EjectPlayers / GUI:EjectAIPlayers): Ok goes on,
// Cancel keeps the old settings. The new mode replaces the old one (slot 33 releases it, slot 8
// sets it up), the dialog ends with 1 and the setup dialog's mode and map controls (0x6EC and
// 0x5A8 of dialog 0xC2, 0xC9, 0xBC or 0xBD) take the new choice.

bool __fastcall unknown_dialog_c2_call(int players, bool unknown_1, bool unknown_2);
bool __fastcall Accept_Map_Selection_005c71f0(HWND dialog);

bool __fastcall Accept_Map_Selection_005c71f0(HWND dialog)
{
    int item = Selected_Campaign_005c67a0();
    int index = -1;
    if (item != -1) {
        MultiMission *map = (MultiMission *)SendDlgItemMessageA(unknown_lobby_dialog_a728e0, 0x553, 0x199, item, 0);   // LB_GETITEMDATA
        for (int i = 0; i < unknown_session_a3d298.Scenarios.Count(); i++) {
            if (map == unknown_session_a3d298.Scenarios[i]) {
                index = i;
                break;
            }
        }
    }
    if (index == -1) {
        return false;
    }
    MultiplayerGameMode *old_mode = unknown_session_a3d298.Mode;
    HWND lobby = unknown_lobby_dialog_a728e0;
    MultiplayerGameMode *mode = 0;
    int selected = SendDlgItemMessageA(unknown_lobby_dialog_a728e0, 0x6EB, 0x188, 0, 0);
    if (selected != -1) {
        mode = (MultiplayerGameMode *)SendDlgItemMessageA(lobby, 0x6EB, 0x199, selected, 0);
    }
    unknown_session_a3d298.Mode = mode;
    bool unknown2 = mode->unknown_slot12();
    bool unknown3 = mode->unknown_slot40();
    int players = Unknown_005c6600(index);
    if (unknown_session_a3d298.Mode != 0) {
        int limit = unknown_session_a3d298.Mode->unknown_slot32();
        if (limit != -1 && limit < players) {
            players = limit;
        }
    }
    if (unknown_game_mode_a3d298 == 4) {
        if (unknown_73c3a0()) {
            players &= ~1;
        } else if (unknown_73c370()) {
            players = 2;
        }
    }
    unknown_session_a3d298.Mode = old_mode;
    bool computers = false;
    bool ask;
    switch (unknown_game_mode_a3d298) {
    case 3:
        ask = Unknown_005bc430(players, unknown2, unknown3);
        break;
    case 4:
        ask = unknown_dialog_c2_call(players, unknown2, unknown3);
        break;
    case 5:
        ask = Skirmish_Has_Computer_After(players);
        computers = true;
        break;
    default:
        ask = false;
        break;
    }
    if (ask) {
        unsigned short const *text = StringTable::Load_String(!computers ? "GUI:EjectPlayers" : "GUI:EjectAIPlayers", 0, "D:\\Ra2\\netshare.cpp", 0xA5F);
        unsigned short const *ok = StringTable::Load_String("GUI:Ok", 0, "D:\\Ra2\\netshare.cpp", 0xA60);
        unsigned short const *cancel = StringTable::Load_String("GUI:Cancel", 0, "D:\\Ra2\\netshare.cpp", 0xA61);
        if (WWMessageBox(L" ").Process(text, ok, cancel, 0, false)) {
            return false;
        }
    }
    if (mode != unknown_session_a3d298.Mode) {
        if (unknown_session_a3d298.Mode != 0) {
            unknown_session_a3d298.Mode->unknown_slot33();
        }
        if (mode != 0) {
            unknown_session_a3d298.Mode = mode;
            unknown_session_a3d298.GameModeIndex = mode->Get_ID();
            unknown_session_a3d298.ScenarioIndex = index;
            mode->unknown_slot8();
        }
    }
    unknown_session_a3d298.ScenarioIndex = index;
    if (mode != 0) {
        mode->unknown_slot9();
    }
    mode->AssignTeamsAtStart();
    mode->UnfixAlliances();
    Unknown_007357E0(dialog, 1);
    HWND setup = WS_Find_Dialog_00735940(0xC2);
    if (setup == 0) {
        setup = WS_Find_Dialog_00735940(0xC9);
        if (setup == 0) {
            setup = WS_Find_Dialog_00735940(0xBC);
            if (setup == 0) {
                setup = WS_Find_Dialog_00735940(0xBD);
            }
        }
    }
    HWND control = GetDlgItem(setup, 0x6EC);
    SendMessageA(control, 0x4B2, 0, (LPARAM)mode->Get_Name().unknown_text_770430());
    MultiMission *current = unknown_session_a3d298.Scenarios[unknown_session_a3d298.ScenarioIndex];
    control = GetDlgItem(setup, 0x5A8);
    SendMessageA(control, 0x4B2, 0, (LPARAM)current);
    mode->unknown_slot20();
    return true;
}


// void Unknown_Pregame_Setup_005c74f0() (0x005C74F0)
// Starts a multiplayer game's pregame state: the scenario takes the session's
// tech level, the frame counter and the crate count are reset, the number of
// players is logged and the global game options take the session's bridge and
// harvester settings with tiberium growing and spreading.

void Debug_Log(char const *format, ...);

void Unknown_Pregame_Setup_005c74f0()
{
    unknown_id_source_a3d290->TechLevel = unknown_session_a3d2b4;
    Frame = 0;
    unknown_session_a3d55c = unknown_players_a3fa7c.Count();
    Debug_Log("Pregame setup for %d players.\n", unknown_players_a3fa7c.Count());
    unknown_a40b18 = unknown_session_a3d2c8;
    unknown_session_a3d2ac = 2;
    unknown_special_a40920.DestroyableBridges = unknown_session_a3d2c0;
    unknown_special_a40920.HarvesterImmune = unknown_session_a3d33d;
    unknown_special_a40920.TiberiumGrows = true;
    unknown_special_a40920.TiberiumSpreads = true;
    unknown_special_a40920.Unknown_0068B410();
}


// void Unknown_005c7570(HWND dialog) (0x005C7570)
// Remakes the dialogs' map preview for the session's map. In a modem game (modes 1 and 2) a map
// that is not among the local maps makes the host send its preview (serial command 0x77, for at
// most 1800 ticks); in a LAN game (3) the setup dialog 0xBD asks the host the same (global
// command 0x1B); in a World Online game (4) with the dialog 0xC9 open a local map keeps (or,
// unless 0x00672140 holds, drops) the current preview. Otherwise the preview is made from the
// map file when it is present.

void __fastcall Unknown_005c7570(HWND dialog)
{
    delete unknown_dialog_preview_a72ce8;
    unknown_dialog_preview_a72ce8 = 0;
    switch (unknown_game_mode_a3d298) {
    case 3:
        if (Current_Dialog_Id_00735b20() == 0xBD
            && !Find_Local_Scenario_0066c5b0(unknown_session_a3d298.ScenarioFileName_658, unknown_session_a3d298.ScenarioFileLength_87c,
                unknown_session_a3d298.ScenarioDigest_85a, unknown_session_a3d298.ScenarioIsOfficial_880, false)) {
            InvalidateRect(dialog, 0, FALSE);
            UnknownGlobalPacket packet;
            memset(&packet, 0, sizeof(packet));
            packet.Command = 0x1B;
            for (;;) {
                int sent;
                if (unknown_game_mode_a3d298 == 3) {
                    sent = unknown_ipx_manager_a40978.Send_Global_Message(&packet, 0x2F, 1, &unknown_address_a3db2c, 0, 0);
                } else {
                    WOL::User *host = Unknown_0074DA60();
                    sent = 0;
                    if (host != 0) {
                        sent = unknown_ipx_manager_a40978.unknown_5259a0(&packet, 0x2F, 1, Narrow_To_Wide_006fa180(host->name));
                    }
                }
                if (sent) {
                    break;
                }
                Service_Game_IO_00481fb0();
            }
            while (unknown_ipx_manager_a40978.Global_Num_Send()) {
                Service_Game_IO_00481fb0();
            }
            return;
        }
        break;
    case 4:
        if (WS_Find_Dialog_00735940(0xC9) != 0) {
            bool present = false;
            if (Find_Local_Scenario_0066c5b0(unknown_session_a3d298.ScenarioFileName_658, unknown_session_a3d298.ScenarioFileLength_87c,
                    unknown_session_a3d298.ScenarioDigest_85a, unknown_session_a3d298.ScenarioIsOfficial_880, false)) {
                // 128 is a hypothesis: VC6 shares the case buffers' frame slot with the tail's CCFileClass
                // temporary (frame 0x1F4) only for a name buffer of 125..200 bytes.
                char name[128];
                strcpy(name, unknown_session_a3d298.ScenarioFileName_658);
                _strupr(name);
                Debug_Printf("Checking for file %s in local directory\n", name);
                if (!unknown_session_a3d298.ScenarioIsOfficial_880 && strstr(name, ".MPR") != 0) {
                    break;
                }
                {
                    RawFileClass file(unknown_session_a3d298.ScenarioFileName_658);
                    if (file.Is_Available()) {
                        present = true;
                    }
                }
                if (!present) {
                    break;
                }
            }
            if (!unknown_session_a3d298.Unknown_00672140() && unknown_dialog_preview_a72ce8 != 0) {
                delete unknown_dialog_preview_a72ce8;
                unknown_dialog_preview_a72ce8 = 0;
                InvalidateRect(dialog, 0, FALSE);
                return;
            }
            InvalidateRect(dialog, 0, FALSE);
            return;
        }
        break;
    case 1:
    case 2:
        if (!Find_Local_Scenario_0066c5b0(unknown_session_a3d298.ScenarioFileName_658, unknown_session_a3d298.ScenarioFileLength_87c,
                unknown_session_a3d298.ScenarioDigest_85a, unknown_session_a3d298.ScenarioIsOfficial_880, false)) {
            UnknownSerialPacket packet;
            memset(&packet, 0, sizeof(packet));
            packet.Command = 0x77;
            CDTimerClass<SystemTimerClass> timer(1800);
            while (timer) {
                int sent = serial_manager_00a40a28.Send_Message(&packet, sizeof(packet), 1);
                while (serial_manager_00a40a28.Num_Send() > 0 && timer) {
                    Service_Game_IO_00481fb0();
                }
                if (sent) {
                    break;
                }
            }
            return;
        }
        break;
    }
    if (unknown_dialog_preview_a72ce8 != 0) {
        delete unknown_dialog_preview_a72ce8;
        unknown_dialog_preview_a72ce8 = 0;
    }
    bool missing = !CCFileClass(unknown_session_a3d298.ScenarioFileName_658).Is_Available();
    if (!missing) {
        unknown_dialog_preview_a72ce8 = new MapPreviewClass;
    }
    if (unknown_dialog_preview_a72ce8 != 0) {
        unknown_dialog_preview_a72ce8->Read_INI_Preview(unknown_session_a3d298.ScenarioFileName_658);
        InvalidateRect(dialog, 0, FALSE);
    }
}


// void Download_Map_Preview_005c7970() (0x005C7970)
// Gets the host's map preview: acknowledges the preview mode to the host (game mode 3: to
// unknown_address_a3db2c, else to the World Online host by name) until the packet is sent and
// every global send is done, downloads the image file (0x0066AEF0, Get_File_From_Host by its trace), LZO-decompresses
// it (its first word is the decompressed length) and replaces the dialogs' map preview with it.
// The network timing is slower while the download runs.

void __fastcall Download_Map_Preview_005c7970()
{
    unknown_ipx_manager_a40978.Set_Timing(50, -1, 5000, true);
    Debug_Printf("Starting map preview download\n");
    UnknownGlobalPacket packet;
    memset(&packet, 0, sizeof(packet));
    packet.Command = 0x1A;
    Debug_Printf("Sending preview mode acks\n");
    for (;;) {
        int sent;
        if (unknown_game_mode_a3d298 == 3) {
            sent = unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 1, &unknown_address_a3db2c, 0, 0);
        } else {
            WOL::User *host = Unknown_0074DA60();
            if (host == 0) {
                break;
            }
            sent = unknown_ipx_manager_a40978.unknown_5259a0(&packet, sizeof(packet), 1, Narrow_To_Wide_006fa180(host->name));
        }
        if (sent) {
            break;
        }
        Service_Game_IO_00481fb0();
    }
    while (unknown_ipx_manager_a40978.Global_Num_Send()) {
        Service_Game_IO_00481fb0();
    }
    Debug_Printf("Preview mode acks sent\n");
    unknown_address_a3f80c = unknown_address_a3db2c;
    Debug_Printf("Calling Get_File_From_Host to receive the file download\n");
    char filename[256];
    if (!Get_File_From_Host_0066aef0(filename, 3, false)) {
        Debug_Printf("got_file is false. Download failed\n");
        unknown_ipx_manager_a40978.Set_Timing(30, -1, 600, true);
        return;
    }
    Debug_Printf("Loading the compressed preview image\n");
    RawFileClass file(filename);
    int size = file.Size();
    char *compressed = (char *)operator new(size);
    file.Read(compressed, size);
    int length = *(int *)compressed;
    Debug_Printf("Decompressing the preview image\n");
    BufferStraw source(compressed + 4, size);
    LZOStraw straw(LZOStraw::DECOMPRESS, 0x2000);
    straw.Get_From(&source);
    unsigned short *image = (unsigned short *)operator new(length * 2);
    straw.Get(image, length);
    Debug_Printf("Creating the new preview surface\n");
    if (unknown_dialog_preview_a72ce8 != 0) {
        delete unknown_dialog_preview_a72ce8;
    }
    unknown_dialog_preview_a72ce8 = new MapPreviewClass;
    unknown_dialog_preview_a72ce8->Create_Preview_Surface(image);
    InvalidateRect(Unknown_00735B10(), 0, FALSE);
    Debug_Printf("Cleaning up the temporary decompression buffers\n");
    operator delete(image);
    operator delete(compressed);
    unknown_ipx_manager_a40978.Set_Timing(30, -1, 600, true);
}


// bool Select_Scenario_005c7c30(int index) (0x005C7C30)
// Selects the multiplayer map with this unknown_multi_missions_a3d8d8 index (-1 clears the
// selection). A map file that is not available asks for its disk (or reports
// TXT_UNABLE_READ_SCENARIO when files come from the hard disk); the session takes the map's
// name, file name, digest, official flag and file size, and the player rows are the map's slot limit
// (Scenario_Slot_Limit_005c65a0).

bool __fastcall Select_Scenario_005c7c30(int index)
{
    if (index == -1) {
        unknown_session_a3d298.ScenarioFileName_658[0] = 0;
        unknown_session_a3d298.ScenarioDigest_85a[0] = 0;
        unknown_session_a3d298.ScenarioFileLength_87c = 0;
        unknown_session_a3d298.ScenarioIsOfficial_880 = false;
        unknown_int_a3db1c = 0;
        unknown_map_name_a3d342[0] = 0;
        return false;
    }
    bool missing = !CCFileClass(unknown_session_a3d298.Scenarios[index]->unknown_58).Is_Available();
    if (missing) {
        if (CD::RequiredCD != -2) {
            if (!unknown_session_a3d298.Scenarios[index]->Unknown_00671F00(Unknown_0046E4C0(60))
                && !CD().Force_Available(unknown_session_a3d298.Scenarios[index]->Unknown_00671F90())) {
                return false;
            }
        } else {
            WWMessageBox(L" ").Process(StringTable::Load_String("TXT_UNABLE_READ_SCENARIO", 0, "D:\\Ra2\\netshare.cpp", 0xCF0),
                StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\netshare.cpp", 0xCF0), 0, 0, false);
            return false;
        }
    }
    wcscpy(unknown_map_name_a3d342, unknown_session_a3d298.Scenarios[index]->Description);
    strcpy(unknown_session_a3d298.ScenarioDigest_85a, unknown_session_a3d298.Scenarios[index]->unknown_15c);
    strcpy(unknown_session_a3d298.ScenarioFileName_658, unknown_session_a3d298.Scenarios[index]->unknown_58);
    strcpy(unknown_id_source_a3d290->FileName, unknown_session_a3d298.ScenarioFileName_658);
    unknown_session_a3d298.ScenarioIsOfficial_880 = unknown_session_a3d298.Scenarios[index]->unknown_17c;
    int players = Scenario_Slot_Limit_005c65a0(index);
    unknown_int_a3db1c = players;
    unknown_session_a3d298.ScenarioFileLength_87c = CCFileClass(unknown_session_a3d298.ScenarioFileName_658).Size();
    unknown_session_a3d298.ScenarioFileLength_87c = CCFileClass(unknown_session_a3d298.ScenarioFileName_658).Size();
    return true;
}


// Not exact (RA2_NON_MATCHING): Upload_Map_Preview_005c7ef0 at 0x005C7EF0, best score 87.21, 2026-11-13; differs: 369 instructions in the original, 374 in the draft (shape 96.37)
#ifdef RA2_NON_MATCHING
// void Upload_Map_Preview_005c7ef0(MapPreviewClass *preview) (0x005C7EF0)
// The host sends the map preview to the other players. In a World Online game a player whose NAT
// flags or addresses rule out a direct transfer makes the host send the public options text "Z3"
// instead. Otherwise the players are told the preview mode (global command 0x19, 1200 ticks to
// send), each must answer 0x1A within 1800 ticks, and the preview's low-resolution image is LZO
// compressed into Preview.bin (its length first) and sent with 0x0066BB20 (Send_Remote_File).
#include "shared/buffer_pipe.hpp"
#include "shared/lzo_pipe.hpp"
#include "shared/firewall_settings.hpp"

void __fastcall Upload_Map_Preview_005c7ef0(MapPreviewClass *preview)
{
    CDTimerClass<SystemTimerClass> timer;
    char options[256];
    if (preview == 0 || unknown_players_a3fa7c.Count() <= 1) {
        return;
    }
    if (unknown_game_mode_a3d298 == 4) {
        bool direct = true;
        if ((unknown_firewall_a71b18.unknown_00 != 0 && !(unknown_firewall_a71b18.unknown_00 & 1))) {
            direct = false;
        } else {
            for (int i = 1; i < unknown_players_a3fa7c.Count(); i++) {
                int house = unknown_session_a3d298.Unknown_00672F90(i);
                if (house >= 0 && house < 8) {
                    unsigned int flags = unknown_session_a3d298.unknown_299c.unknown[house];
                    if (flags != 0 && !(flags & 1)) { Debug_Printf("Player %s has a NAT - no preview download possible\n", Wide_To_Narrow_006fa120(unknown_players_a3fa7c[i]->Name)); direct = false; }
                    else if (!unknown_session_a3d298.unknown_293c.Addresses[house].Is_Same_Node(unknown_players_a3fa7c[i]->Address)) { Debug_Printf("Player %s has different local and external IPs - no preview download possible\n", Wide_To_Narrow_006fa120(unknown_players_a3fa7c[i]->Name)); direct = false; }
                    if (!direct) break;
                }
            }
        }
        if (!direct) { options[0] = 'Z'; options[1] = '3'; options[2] = 0; Debug_Printf("Sending public options string %s\n", options); unknown_chat_a72cfc->unknown_slot15(options); return; }
    }
    Debug_Printf("Starting map preview upload\n");
    UnknownGlobalPacket packet;
    memset(&packet, 0, sizeof(packet));
    packet.Command = 0x19;
    strcpy(packet.Name, unknown_name_a3d3a0);
    unknown_ipx_manager_a40978.Set_Timing(50, -1, 5000, true);
    timer = 1200;
    for (int i = 1; i < unknown_players_a3fa7c.Count(); i++) {
        unknown_ipx_manager_a40978.unknown_5259a0(&packet, sizeof(packet), 1, unknown_players_a3fa7c[i]->Name);
        Service_Game_IO_00481fb0();
    }
    while (unknown_ipx_manager_a40978.Global_Num_Send() && timer) {
        Service_Game_IO_00481fb0();
    }
    if (!timer) {
        return;
    }
    IPXAddressClass address;
    bool ready[8];
    memset(ready, 0, sizeof(ready));
    int count = 0;
    timer = 1800;
    Debug_Printf("Waiting for %d players to signal ready to receive\n", unknown_players_a3fa7c.Count() - 1);
    while (count < unknown_players_a3fa7c.Count() - 1 && timer) {
        Service_Game_IO_00481fb0();
        UnknownGlobalPacket reply;
        int length = sizeof(reply);
        unsigned short product;
        if (unknown_ipx_manager_a40978.Get_Global_Message(&reply, &length, &address, &product) && reply.Command == 0x1A) {
            for (int j = 1; j < unknown_players_a3fa7c.Count(); j++) {
                if (address.Is_Same_Node(unknown_players_a3fa7c[j]->Address)) {
                    if (!ready[j]) {
                        ready[j] = true;
                        count++;
                    }
                    break;
                }
            }
        }
    }
    if (count != unknown_players_a3fa7c.Count() - 1) {
        return;
    }
    Debug_Printf("Creating low-res preview image\n");
    int size = 0;
    unsigned short *image = preview->Create_Paletted_Preview(0x40, &size);
    Debug_Printf("Preview size is %d bytes\n", size);
    void *compressed = operator new(size * 4);
    BufferPipe target(compressed, size * 4);
    LZOPipe pipe(LZOPipe::COMPRESS, 0x2000);
    pipe.Put_To(&target);
    int length = pipe.Put(image, size);
    length += pipe.End();
    operator delete(image);
    Debug_Printf("Compressed preview image is %d bytes\n", length);
    RawFileClass file("Preview.bin");
    if (file.Is_Available()) {
        file.Delete();
    }
    file.Open(FILE_RIGHTS_WRITE);
    file.Write(&size, 4);
    file.Write(compressed, length);
    file.Close();
    operator delete(compressed);
    Debug_Printf("Calling Send_Remote_File to send the preview\n");
    Send_Remote_File_0066bb20("Preview.bin", 3, 1, 0);
    unknown_ipx_manager_a40978.Set_Timing(30, -1, 600, true);
}
#endif

// char * __fastcall Random_Map_CRC_005c8420() (0x005C8420; __fastcall: Make_Random_Map 0x005C84E0 copies its result before the ctor pushes)
// The random map's identity text: the CRC of the map generator's seed options (0x140 bytes from Theater,
// with the description buffer cleared while it runs) as "%08X" in a new 12-character buffer.

char * __fastcall Random_Map_CRC_005c8420()
{
    unsigned long crc = 0;
    unsigned long const *data = (unsigned long const *)&RandomMapGen.SeedData.Theater;
    int length = 0x140;
    wchar_t description[128];
    memcpy(description, RandomMapGen.SeedData.DescriptionBuffer, sizeof(description));
    memset(RandomMapGen.SeedData.DescriptionBuffer, 0, sizeof(description));
    do {
        unsigned long hibit = crc >> 31;
        if (length >= 4) {
            crc = (crc << 1) + *data++ + hibit;
            length -= 4;
        } else {
            unsigned long value = 0;
            unsigned char const *bytes = (unsigned char const *)data;
            while (length != 0) {
                value = (value << 8) | *bytes++;
                length--;
            }
            crc = (crc << 1) + value + hibit;
        }
    } while (length > 0);
    char *text = new char[12];
    sprintf(text, "%08X", crc);
    memcpy(RandomMapGen.SeedData.DescriptionBuffer, description, sizeof(description));
    return text;
}


// int __fastcall Make_Random_Map_005c84e0() (0x005C84E0)
// Runs the random map dialog (0x00577490, with the game mode's service callback); after OK saves the seed
// options as RandMap.Sed, reloads the lobby preview from RandMap.img and gives the session's random map entry
// the new description, or adds a new random map entry named by the seed CRC. Returns that entry's index,
// or -1 when the dialog was cancelled.

int __fastcall Do_Random_Map_Dialog_00577490(bool (*callback)());
bool Default_Map_Generation_Callback_00482100();
extern bool unknown_host_shown_7e6dc0;

int __fastcall Make_Random_Map_005c84e0()
{
    bool (*service)();
    if (unknown_game_mode_a3d298 == 3) {
        service = Unknown_005bbd10;
    } else if (unknown_game_mode_a3d298 == 4) {
        service = Unknown_0073e100;
    } else if (unknown_game_mode_a3d298 == 1 || unknown_game_mode_a3d298 == 2) {
        service = Unknown_0059c880;
    } else {
        service = Default_Map_Generation_Callback_00482100;
    }
    if (Do_Random_Map_Dialog_00577490(service) != 1) {
        return -1;
    }
    unknown_host_shown_7e6dc0 = true;
    RandomMapGen.SeedData.Save_Or_Prompt_00578ed0("RandMap.Sed");
    delete unknown_dialog_preview_a72ce8;
    unknown_dialog_preview_a72ce8 = new MapPreviewClass;
    unknown_dialog_preview_a72ce8->Read_PCX_Preview("RandMap.img");
    int index;
    bool found = false;
    for (index = 0; index < unknown_multi_missions_a3d8d8.Count(); index++) {
        if (unknown_multi_missions_a3d8d8[index]->Unknown_006720C0()) {
            found = true;
            break;
        }
    }
    if (!found) {
        char *crc = Random_Map_CRC_005c8420();
        MultiMission *mission = new MultiMission("RandMap.Sed", RandomMapGen.SeedData.DescriptionBuffer, crc, 1, 0, 2, 4);
        unknown_multi_missions_a3d8d8.Add(mission);
        delete crc;
    } else {
        unknown_multi_missions_a3d8d8[index]->Set_Description(RandomMapGen.SeedData.DescriptionBuffer);
    }
    return index;
}


// 0x005c8660: function not yet identified
//
// Body: 20 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: bool __fastcall Is_Host_Name_005c8660(undefined4 param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_005c8660 {

extern unsigned int  Compare_Wide_007837d3(unsigned short *, unsigned short *);
extern "C" unsigned int host_name_00a3f618;

bool __fastcall Is_Host_Name_005c8660(unsigned int name)
{
    int result;

    result = (int)(Compare_Wide_007837d3((unsigned short *)(name), (unsigned short *)(&host_name_00a3f618)));
    return result == 0;
}

}  // namespace seed_005c8660


// 0x005c8680: function not yet identified
//
// Body: 28 bytes (frozen inventory bound).
// Referenced from 1 site (unconditional call).
// Ghidra prototype: bool __fastcall unknown_is_host_player_005c8680(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_005c8680 {

extern unsigned int  Compare_Wide_007837d3(unsigned short *, unsigned short *);
extern "C" unsigned int host_name_00a3f618;
extern "C" unsigned int unknown_lobby_players_00a3fa80;

bool __fastcall unknown_is_host_player_005c8680(int index)
{
    int result;

    result = (int)(Compare_Wide_007837d3((unsigned short *)(*(unsigned int *)(unknown_lobby_players_00a3fa80 + index * 4)), (unsigned short *)(&host_name_00a3f618)));
    return result == 0;
}

}  // namespace seed_005c8680


// bool Is_Host_005c86a0() (0x005C86A0)

bool Is_Host_005c86a0()
{
    wchar_t *name = Narrow_To_Wide_006fa180(unknown_name_a3d3a0);
    return Compare_Wide_007837d3(name, unknown_host_name_a3f618) == 0;
}


// int __fastcall Is_Local_Name_005c86c0(wchar_t const *name) (0x005C86C0)

int __fastcall Is_Local_Name_005c86c0(wchar_t const *name)
{
    return Compare_Wide_007837d3(name, Narrow_To_Wide_006fa180(unknown_name_a3d3a0)) == 0;
}


// void __fastcall Init_Lobby_Entries_005c86e0() (0x005C86E0)

void __fastcall Init_Lobby_Entries_005c86e0()
{
    if (unknown_lobby_texts_a72d0c[0] == 0) {
        unknown_lobby_texts_a72d0c[0] = StringTable::Load_String("GUI:Open", 0, "D:\\Ra2\\netshare.cpp", 0xe98);
        unknown_lobby_texts_a72d0c[1] = StringTable::Load_String("GUI:Closed", 0, "D:\\Ra2\\netshare.cpp", 0xe99);
        unknown_lobby_texts_a72d0c[2] = StringTable::Load_String("GUI:AIEasy", 0, "D:\\Ra2\\netshare.cpp", 0xe9a);
        unknown_lobby_texts_a72d0c[3] = StringTable::Load_String("GUI:AINormal", 0, "D:\\Ra2\\netshare.cpp", 0xe9b);
        unknown_lobby_texts_a72d0c[4] = StringTable::Load_String("GUI:AIHard", 0, "D:\\Ra2\\netshare.cpp", 0xe9c);
        unknown_lobby_texts_a72d0c[5] = StringTable::Load_String("GUI:Waiting", 0, "D:\\Ra2\\netshare.cpp", 0xe9d);
        unknown_lobby_texts_a72d0c[6] = StringTable::Load_String("GUI:OpenObserver", 0, "D:\\Ra2\\netshare.cpp", 0xe9e);
    }
    if (unknown_open_entry_a72d2c == 0) {
        unknown_open_entry_a72d2c = new NodeNameType;
        wcscpy(unknown_open_entry_a72d2c->Name, unknown_lobby_texts_a72d0c[0]);
    }
    if (unknown_closed_entry_a72d30 == 0) {
        unknown_closed_entry_a72d30 = new NodeNameType;
        wcscpy(unknown_closed_entry_a72d30->Name, unknown_lobby_texts_a72d0c[1]);
    }
    if (unknown_easy_ai_entry_a72d34 == 0) {
        unknown_easy_ai_entry_a72d34 = new NodeNameType;
        wcscpy(unknown_easy_ai_entry_a72d34->Name, unknown_lobby_texts_a72d0c[2]);
    }
    if (unknown_medium_ai_entry_a72d38 == 0) {
        unknown_medium_ai_entry_a72d38 = new NodeNameType;
        wcscpy(unknown_medium_ai_entry_a72d38->Name, unknown_lobby_texts_a72d0c[3]);
    }
    if (unknown_hard_ai_entry_a72d3c == 0) {
        unknown_hard_ai_entry_a72d3c = new NodeNameType;
        wcscpy(unknown_hard_ai_entry_a72d3c->Name, unknown_lobby_texts_a72d0c[4]);
    }
    if (unknown_waiting_entry_a72d40 == 0) {
        unknown_waiting_entry_a72d40 = new NodeNameType;
        wcscpy(unknown_waiting_entry_a72d40->Name, unknown_lobby_texts_a72d0c[5]);
    }
    if (unknown_player_a72d44 == 0) {
        unknown_player_a72d44 = new NodeNameType;
        wcscpy(unknown_player_a72d44->Name, unknown_lobby_texts_a72d0c[6]);
    }
}


// void Unknown_Free_Lobby_Entries_005c8950() (0x005C8950)
// Frees the seven fixed lobby entries (0x00A72D2C..0x00A72D44, zeroing each pointer)
// and every session node, then clears the node list (0x005A7480, which frees
// each element again through the list's virtual Clear).

void Unknown_Free_Lobby_Entries_005c8950()
{
    if (unknown_open_entry_a72d2c != 0) {
        Free_Memory_00781d3b(unknown_open_entry_a72d2c);
        unknown_open_entry_a72d2c = 0;
    }
    if (unknown_closed_entry_a72d30 != 0) {
        Free_Memory_00781d3b(unknown_closed_entry_a72d30);
        unknown_closed_entry_a72d30 = 0;
    }
    if (unknown_easy_ai_entry_a72d34 != 0) {
        Free_Memory_00781d3b(unknown_easy_ai_entry_a72d34);
        unknown_easy_ai_entry_a72d34 = 0;
    }
    if (unknown_medium_ai_entry_a72d38 != 0) {
        Free_Memory_00781d3b(unknown_medium_ai_entry_a72d38);
        unknown_medium_ai_entry_a72d38 = 0;
    }
    if (unknown_hard_ai_entry_a72d3c != 0) {
        Free_Memory_00781d3b(unknown_hard_ai_entry_a72d3c);
        unknown_hard_ai_entry_a72d3c = 0;
    }
    if (unknown_waiting_entry_a72d40 != 0) {
        Free_Memory_00781d3b(unknown_waiting_entry_a72d40);
        unknown_waiting_entry_a72d40 = 0;
    }
    if (unknown_player_a72d44 != 0) {
        Free_Memory_00781d3b(unknown_player_a72d44);
        unknown_player_a72d44 = 0;
    }
    Free_Pointers_005a7480(&unknown_players_a3fa7c);
}


// void Fill_Slots_005c8a10() (0x005C8A10)

void Fill_Slots_005c8a10()
{
    int limit = Lobby_Slot_Limit_005c6520();
    for (int i = 0; i < limit; i++) {
        unknown_player_slots_a3fa98[i] = unknown_open_entry_a72d2c;
    }
    for (int j = limit; j < 8; j++) {
        unknown_player_slots_a3fa98[j] = unknown_closed_entry_a72d30;
    }
}


// void Clear_Slot_Lists_005c8a70() (0x005C8A70)

void Clear_Slot_Lists_005c8a70()
{
    unknown_color_list_old_a726b0[0] = 0;
    unknown_color_list_a726d4[0] = 0;
    unknown_country_list_old_a72a44[0] = 0;
    unknown_country_list_a726f8[0] = 0;
}


// void Unknown_Reset_Slots_005c8a90() (0x005C8A90)
// Resets the lobby's player slots: the first 2, 4 or 8 (by game mode and the
// network tests) get the first fixed entry, the rest the second; the slot
// before the table is set to -1, the lobby state is told and two words clear.

void Unknown_Reset_Slots_005c8a90()
{
    int limit;
    if (unknown_game_mode_a3d298 == 4) {
        if (unknown_73c370() && !unknown_73c3a0()) {
            limit = 2;
        } else {
            limit = 4;
        }
    } else {
        limit = 8;
    }
    for (int i = 0; i < limit; i++) {
        unknown_player_slots_a3fa98[i] = unknown_open_entry_a72d2c;
    }
    for (int j = limit; j < 8; j++) {
        unknown_player_slots_a3fa98[j] = unknown_closed_entry_a72d30;
    }
    unknown_lobby_a3fa94 = -1;
    unknown_lobby_state_a3d2b0.Unknown_00672810();
    unknown_slot_names_old_a728e4[0] = 0;
    unknown_slot_names_a72734[0] = 0;
}


// NodeNameType *__fastcall Unknown_Find_Lobby_Name_005c8b20(wchar_t const *name) (0x005C8B20)
// Finds the lobby buffer (the seven fixed ones, then the session nodes' names)
// whose text equals name (0x007837D3 compares wide strings, 0 when equal);
// null when there is none.

NodeNameType *__fastcall Unknown_Find_Lobby_Name_005c8b20(wchar_t const *name)
{
    if (Compare_Wide_007837d3(unknown_open_entry_a72d2c->Name, name) == 0) {
        return unknown_open_entry_a72d2c;
    }
    if (Compare_Wide_007837d3(unknown_closed_entry_a72d30->Name, name) == 0) {
        return unknown_closed_entry_a72d30;
    }
    if (Compare_Wide_007837d3(unknown_easy_ai_entry_a72d34->Name, name) == 0) {
        return unknown_easy_ai_entry_a72d34;
    }
    if (Compare_Wide_007837d3(unknown_medium_ai_entry_a72d38->Name, name) == 0) {
        return unknown_medium_ai_entry_a72d38;
    }
    if (Compare_Wide_007837d3(unknown_hard_ai_entry_a72d3c->Name, name) == 0) {
        return unknown_hard_ai_entry_a72d3c;
    }
    if (Compare_Wide_007837d3(unknown_waiting_entry_a72d40->Name, name) == 0) {
        return unknown_waiting_entry_a72d40;
    }
    if (Compare_Wide_007837d3(unknown_player_a72d44->Name, name) == 0) {
        return unknown_player_a72d44;
    }
    int count = unknown_players_a3fa7c.Count();
    for (int i = 0; i < count; i++) {
        NodeNameType *node = unknown_players_a3fa7c[i];
        if (Compare_Wide_007837d3(node->Name, name) == 0) {
            return node;
        }
    }
    return 0;
}


// int __fastcall Find_Slot_By_Name_005c8c30(wchar_t const *name) (0x005C8C30)

int __fastcall Find_Slot_By_Name_005c8c30(wchar_t const *name)
{
    for (int i = 0; i < 8; i++) {
        if (unknown_player_slots_a3fa98[i] != 0 && _wcsicmp(name, unknown_player_slots_a3fa98[i]->Name) == 0) {
            return i;
        }
    }
    return -1;
}


// int Find_First_Player_Slot_005c8c70() (0x005C8C70)

int Find_First_Player_Slot_005c8c70()
{
    if (unknown_players_a3fa7c.Count() == 0) {
        return -1;
    }
    return Find_Slot_By_Name_005c8c30(unknown_players_a3fa7c[0]->Name);
}


class Leaf_5c8cc0 { public: void __fastcall get(unsigned int value); };
void __fastcall Leaf_5c8cc0::get(unsigned int value) { unsigned int slot = (unsigned int)this; unsigned int spare; *(unsigned int*)(slot*4 + 0xa3fa98) = value; }


// void __fastcall Seat_First_Player_005c8cd0(int slot) (0x005C8CD0)
// The slot index becomes the session's own slot and that slot holds the first session player
// (the host seats itself; Get_Join_Responses seats a joiner).

void __fastcall Seat_First_Player_005c8cd0(int slot)
{
    unknown_lobby_a3fa94 = slot;
    unknown_player_slots_a3fa98[slot] = unknown_players_a3fa7c[0];
}


// void __fastcall Unknown_Slot_Names_005c8cf0(wchar_t *list) (0x005C8CF0)
// Writes the name of each of the eight slots' entries into list, each followed by a comma.

extern "C" unsigned short *__cdecl wcscat(unsigned short *dest, unsigned short const *source);

void __fastcall Unknown_Slot_Names_005c8cf0(wchar_t *list)
{
    *list = 0;
    for (int i = 0; i < 8; i++) {
        NodeNameType *entry = unknown_player_slots_a3fa98[i];
        WideString name;
        if (entry == 0 || entry == unknown_open_entry_a72d2c || entry == unknown_closed_entry_a72d30 ||
            entry == unknown_waiting_entry_a72d40 || entry == unknown_player_a72d44 ||
            entry == unknown_easy_ai_entry_a72d34 || entry == unknown_medium_ai_entry_a72d38 ||
            entry == unknown_hard_ai_entry_a72d3c) {
            Wstring text = Unknown_Lobby_Entry_Name_005cd0e0(entry);
            name = Wstring_To_Wide(text);
        } else {
            name = entry->Name;
        }
        wcscat(list, name.Get());
        wcscat(list, L",");
    }
}


// void Set_Slot_Names_005c8dd0(wchar_t const *names) (0x005C8DD0)
// Seats a comma-separated list of lobby names in the player slots (the host's slot list): the
// slots are refilled (Fill_Slots), then each name takes the next slot (an unknown name waits).
// A slot that changes between observer (OpenObserver or a player of team -1) and player
// rebuilds its side and colour combo boxes in the setup dialog (0xC2, 0xC9, 0xBC or 0xBD).

void __fastcall Set_Slot_Names_005c8dd0(wchar_t const *names)
{
    if (*names == 0) {
        return;
    }
    wchar_t buffer[170];
    wcscpy(buffer, names);
    bool observer[8];
    NodeNameType *observer_entry = unknown_player_a72d44;
    NodeNameType *waiting = unknown_waiting_entry_a72d40;
    NodeNameType *open = unknown_open_entry_a72d2c;
    NodeNameType *closed = unknown_closed_entry_a72d30;
    for (int i = 0; i < 8; i++) {
        NodeNameType *entry = unknown_player_slots_a3fa98[i];
        observer[i] = entry == observer_entry
            || (entry != 0 && entry != open && entry != closed && entry != waiting && entry->Team == -1);
    }
    Fill_Slots_005c8a10();
    int slot = 0;
    for (wchar_t *name = wcstok(buffer, L","); name != 0; name = wcstok(0, L",")) {
        NodeNameType *entry;
        {
            WideString text(name);
            entry = Unknown_Find_Lobby_Entry_005cd1b0(text);
        }
        if (entry == 0) {
            entry = Unknown_Find_Lobby_Name_005c8b20(name);
            if (entry == 0) {
                entry = unknown_waiting_entry_a72d40;
            }
        }
        if (entry == unknown_player_a72d44
            || (entry != 0 && entry != unknown_open_entry_a72d2c && entry != unknown_closed_entry_a72d30
                && entry != unknown_waiting_entry_a72d40 && entry != unknown_easy_ai_entry_a72d34
                && entry != unknown_medium_ai_entry_a72d38 && entry != unknown_hard_ai_entry_a72d3c
                && entry->Team == -1)) {
            HWND dialog = Find_Lobby_Window_005c2c40();
            if (!observer[slot]) {
                unknown_player_slots_a3fa98[slot] = entry;
                Fill_Observer_Combo_4d1640(dialog, Player_Side_Combo_ID_4d1350(slot));
                SendMessageA(GetDlgItem(dialog, Slot_Control_Id_005c97a0(slot)), 0x4F1, 0, 1);
                int id = Player_Color_Combo_ID_4d1cb0(slot);
                Fill_Random_Color_Combo_4d2250(dialog, id);
                Select_Color_Combo_4d2480(dialog, id, 8);
            }
        } else {
            HWND dialog = Find_Lobby_Window_005c2c40();
            if (observer[slot]) {
                int id = Player_Side_Combo_ID_4d1350(slot);
                if (Side_Combo_Data_4d1c70(dialog, id, -1) == -3) {
                    Fill_Side_Combo_4d1560(dialog, id);
                    SendMessageA(GetDlgItem(dialog, Slot_Control_Id_005c97a0(slot)), 0x4F1, 0, 0);
                    id = Player_Color_Combo_ID_4d1cb0(slot);
                    Fill_Color_Combo_4d2080(dialog, id);
                    Select_Color_Combo_4d2480(dialog, id, -2);
                }
            }
        }
        unknown_player_slots_a3fa98[slot] = entry;
        slot++;
    }
}


// int __fastcall Assign_Slot_005c9170(NodeNameType *entry) (0x005C9170)

int __fastcall Assign_Slot_005c9170(NodeNameType *entry)
{
    for (int i = 0; i < 8; i++) {
        if ((unknown_player_slots_a3fa98[i] == unknown_open_entry_a72d2c && entry->Team != -1)
            || (unknown_player_slots_a3fa98[i] == unknown_player_a72d44 && entry->Team == -1)) {
            unknown_player_slots_a3fa98[i] = entry;
            return i;
        }
    }
    return -1;
}


// void __fastcall Unknown_Release_Slot_005c91c0(IPXAddressClass &address) (0x005C91C0)
// Finds the player slot whose node has this address and gives it back to the
// lobby: a node with no ID (-1) becomes the seventh fixed entry, any other
// goes to the first fixed entry within the slot limit and to the second beyond it.

void __fastcall Unknown_Release_Slot_005c91c0(IPXAddressClass &address)
{
    int limit = 4;
    if (unknown_game_mode_a3d298 == 4) {
        if (unknown_73c370() && !unknown_73c3a0()) {
            limit = 2;
        }
    } else {
        limit = 8;
    }
    if (unknown_lobby_flag_7e6d90 == 0 && limit < 7) {
        limit++;
    }
    for (int i = 0; i < 8; i++) {
        NodeNameType *node = unknown_player_slots_a3fa98[i];
        if (node->Address == address) {
            if (unknown_player_slots_a3fa98[i]->Team == -1) {
                unknown_player_slots_a3fa98[i] = unknown_player_a72d44;
                return;
            }
            NodeNameType *entry = unknown_open_entry_a72d2c;
            if (i >= limit) {
                entry = unknown_closed_entry_a72d30;
            }
            unknown_player_slots_a3fa98[i] = entry;
            return;
        }
    }
}


// void __fastcall Unknown_Release_Slot_by_Name_005c9270(char const *name) (0x005C9270)
// Finds the player slot whose node name (narrowed) equals name and gives it back to the
// lobby: a node with no ID (-1) becomes the seventh fixed entry, any other
// goes to the first fixed entry within the slot limit and to the second beyond it.

void __fastcall Unknown_Release_Slot_by_Name_005c9270(char const *name)
{
    int limit = 4;
    if (unknown_game_mode_a3d298 == 4) {
        if (unknown_73c370() && !unknown_73c3a0()) {
            limit = 2;
        }
    } else {
        limit = 8;
    }
    if (unknown_lobby_flag_7e6d90 == 0 && limit < 7) {
        limit++;
    }
    for (int i = 0; i < 8; i++) {
        NodeNameType *node = unknown_player_slots_a3fa98[i];
        if (node != 0 && Compare_Narrow_00781f20(name, Wide_To_Narrow_006fa120(node->Name)) == 0) {
            if (unknown_player_slots_a3fa98[i]->Team == -1) {
                unknown_player_slots_a3fa98[i] = unknown_player_a72d44;
                return;
            }
            NodeNameType *entry = unknown_open_entry_a72d2c;
            if (i >= limit) {
                entry = unknown_closed_entry_a72d30;
            }
            unknown_player_slots_a3fa98[i] = entry;
            return;
        }
    }
}


// 0x005c9320: function not yet identified
//
// Body: 69 bytes (frozen inventory bound).
// Referenced from 7 sites (unconditional call).
// Ghidra prototype: undefined4 __fastcall Is_Real_Player_Entry_005c9320(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_005c9320 {

extern "C" unsigned int open_entry_00a72d2c;
extern "C" unsigned int closed_entry_00a72d30;
extern "C" unsigned int easy_ai_entry_00a72d34;
extern "C" unsigned int medium_ai_entry_00a72d38;
extern "C" unsigned int hard_ai_entry_00a72d3c;
extern "C" unsigned int waiting_entry_00a72d40;
extern "C" unsigned int player_entry_00a72d44;

unsigned int __fastcall Is_Real_Player_Entry_005c9320(int entry)
{
    if (((((entry != 0) && (entry != open_entry_00a72d2c)) && (entry != closed_entry_00a72d30)) &&
            ((entry != waiting_entry_00a72d40 && (entry != player_entry_00a72d44)))) &&
          ((entry != easy_ai_entry_00a72d34 && ((entry != medium_ai_entry_00a72d38 && (entry != hard_ai_entry_00a72d3c)))))) {
        return 1;
    }
    return 0;
}

}  // namespace seed_005c9320


// 0x005c9370: function not yet identified
//
// Body: 37 bytes (frozen inventory bound).
// Referenced from 1 site (unconditional call).
// Ghidra prototype: undefined4 __fastcall Is_AI_Entry_005c9370(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_005c9370 {

extern "C" unsigned int easy_ai_entry_00a72d34;
extern "C" unsigned int medium_ai_entry_00a72d38;
extern "C" unsigned int hard_ai_entry_00a72d3c;

unsigned int __fastcall Is_AI_Entry_005c9370(int entry)
{
    if ((entry != 0) &&
          (((entry == easy_ai_entry_00a72d34 || (entry == medium_ai_entry_00a72d38)) || (entry == hard_ai_entry_00a72d3c)))) {
        return 1;
    }
    return 0;
}

}  // namespace seed_005c9370


// 0x005c93a0: function not yet identified
//
// Body: 45 bytes (frozen inventory bound).
// Referenced from 1 site (unconditional call).
// Ghidra prototype: undefined4 __fastcall Is_Occupant_Entry_005c93a0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_005c93a0 {

extern "C" unsigned int open_entry_00a72d2c;
extern "C" unsigned int closed_entry_00a72d30;
extern "C" unsigned int waiting_entry_00a72d40;
extern "C" unsigned int player_entry_00a72d44;

unsigned int __fastcall Is_Occupant_Entry_005c93a0(int entry)
{
    if ((((entry != 0) && (entry != open_entry_00a72d2c)) && (entry != closed_entry_00a72d30)) &&
          ((entry != player_entry_00a72d44 && (entry != waiting_entry_00a72d40)))) {
        return 1;
    }
    return 0;
}

}  // namespace seed_005c93a0


// 0x005c93d0: function not yet identified
//
// Body: 44 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 __fastcall AI_Difficulty_005c93d0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_005c93d0 {

extern "C" unsigned int easy_ai_entry_00a72d34;
extern "C" unsigned int medium_ai_entry_00a72d38;
extern "C" unsigned int hard_ai_entry_00a72d3c;

unsigned int __fastcall AI_Difficulty_005c93d0(int entry)
{
    unsigned int difficulty;

    difficulty = (unsigned int)(0xffffffff);
    if (entry == easy_ai_entry_00a72d34) {
        return 2;
    }
    if (entry == medium_ai_entry_00a72d38) {
        return 1;
    }
    if (entry == hard_ai_entry_00a72d3c) {
        difficulty = (unsigned int)(0);
    }
    return difficulty;
}

}  // namespace seed_005c93d0


// 0x005c9400: function not yet identified
//
// Body: 62 bytes (frozen inventory bound).
// Referenced from 1 site (unconditional call).
// Ghidra prototype: int Count_AI_Slots_005c9400(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_005c9400 {

extern "C" unsigned int player_slots_00a3fa98;
extern "C" unsigned int easy_ai_entry_00a72d34;
extern "C" unsigned int medium_ai_entry_00a72d38;
extern "C" unsigned int hard_ai_entry_00a72d3c;

int  Count_AI_Slots_005c9400(void)
{
    int entry;
    int count;
    int *slot;

    count = (int)(0);
    slot = (int *)(&player_slots_00a3fa98);
    do {
        entry = (int)(*slot);
        if ((entry != 0) &&
              (((entry == easy_ai_entry_00a72d34 || (entry == medium_ai_entry_00a72d38)) || (entry == hard_ai_entry_00a72d3c)))) {
            count = (int)(count + 1);
        }
        slot = (int *)(slot + 1);
    } while ((int)slot < 0xa3fab8);
    return count;
}

}  // namespace seed_005c9400


// bool Slots_Filled_005c9440() (0x005C9440)

bool Slots_Filled_005c9440()
{
    int count = unknown_max_players_a3d558 < unknown_int_a3db1c ? unknown_max_players_a3d558 : unknown_int_a3db1c;
    for (int i = 0; i < count; i++) {
        if (unknown_player_slots_a3fa98[i] == unknown_open_entry_a72d2c) {
            return false;
        }
    }
    if (unknown_lobby_flag_7e6d90 == 0 && count < 8 && unknown_player_slots_a3fa98[count] == unknown_open_entry_a72d2c) {
        return false;
    }
    return true;
}


// bool No_Observer_Slot_005c9490() (0x005C9490)

bool No_Observer_Slot_005c9490()
{
    for (int i = 0; i < 8; i++) {
        if (unknown_player_slots_a3fa98[i] == unknown_player_a72d44) {
            return false;
        }
    }
    return true;
}


// bool Unknown_005c94b0() (0x005C94B0)

static bool Is_Seated_Entry(NodeNameType *entry)
{
    return entry != 0 && entry != unknown_open_entry_a72d2c && entry != unknown_closed_entry_a72d30
        && entry != unknown_player_a72d44 && entry != unknown_waiting_entry_a72d40;
}

bool Unknown_005c94b0()
{
    for (int i = 0; i < 8; i++) {
        if (Is_Seated_Entry(unknown_player_slots_a3fa98[i]) && i == unknown_lobby_flag_7e6d90
            && unknown_player_slots_a3fa98[i]->Team == -1) {
            return true;
        }
    }
    return false;
}


// bool Channel_Full_005c9510() (0x005C9510)

bool Channel_Full_005c9510()
{
    int limit = unknown_max_players_a3d558 < unknown_int_a3db1c ? unknown_max_players_a3d558 : unknown_int_a3db1c;
    if (unknown_players_a3fa7c.Count() - Unknown_005c94b0() > limit) {
        Debug_Printf("Channel full - Session.ScenarioMaxPlayers = %d\n", unknown_int_a3db1c);
        return true;
    }
    return false;
}


// bool __fastcall Unknown_Is_Color_Taken_005c95d0(int color) (0x005C95D0)
// True when a player slot holds a real player (not empty and not one of the fixed
// lobby entries) whose colour (0x0066E2A0) is color, or when color is already one
// of the eight per-slot values of 0x00A3D31C. -2 (random) is never taken.
// Set_Slot_Color_005c9ab0 inlines it (with the entry test as an inline helper).

static bool Is_Player_Entry(NodeNameType *entry)
{
    return entry != 0 && entry != unknown_open_entry_a72d2c && entry != unknown_closed_entry_a72d30 &&
        entry != unknown_waiting_entry_a72d40 && entry != unknown_player_a72d44;
}

bool __fastcall Unknown_Is_Color_Taken_005c95d0(int color)
{
    if (color != -2) {
        int i;
        for (i = 0; i < 8; i++) {
            NodeNameType *entry = unknown_player_slots_a3fa98[i];
            if (Is_Player_Entry(entry) &&
                entry != unknown_easy_ai_entry_a72d34 && entry != unknown_medium_ai_entry_a72d38 &&
                entry != unknown_hard_ai_entry_a72d3c && entry->unknown_66e2a0() == color) {
                return true;
            }
        }
        for (i = 0; i < 8; i++) {
            if (unknown_lobby_a3d31c[i] == color) {
                return true;
            }
        }
    }
    return false;
}


// int __fastcall Entry_Kind_005c9650(NodeNameType *entry) (0x005C9650)

int __fastcall Entry_Kind_005c9650(NodeNameType *entry)
{
    if (entry == unknown_open_entry_a72d2c) {
        return 2;
    }
    if (entry == unknown_closed_entry_a72d30) {
        return 3;
    }
    if (entry == unknown_easy_ai_entry_a72d34) {
        return 4;
    }
    if (entry == unknown_medium_ai_entry_a72d38) {
        return 5;
    }
    if (entry == unknown_hard_ai_entry_a72d3c) {
        return 6;
    }
    return entry == unknown_player_a72d44 ? 7 : 2;
}


// 0x005c96b0: function not yet identified
//
// Body: 73 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: uint __fastcall Entry_For_Kind_005c96b0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_005c96b0 {

extern "C" unsigned int open_entry_00a72d2c;
extern "C" unsigned int closed_entry_00a72d30;
extern "C" unsigned int easy_ai_entry_00a72d34;
extern "C" unsigned int medium_ai_entry_00a72d38;
extern "C" unsigned int hard_ai_entry_00a72d3c;
extern "C" unsigned int player_entry_00a72d44;

unsigned int __fastcall Entry_For_Kind_005c96b0(int kind)
{
    if (kind == 2) {
        return open_entry_00a72d2c;
    }
    if (kind == 3) {
        return closed_entry_00a72d30;
    }
    if (kind == 4) {
        return easy_ai_entry_00a72d34;
    }
    if (kind == 5) {
        return medium_ai_entry_00a72d38;
    }
    if (kind == 6) {
        return hard_ai_entry_00a72d3c;
    }
    return (kind != 7) - 1 & player_entry_00a72d44;
}

}  // namespace seed_005c96b0


// void Unknown_Enable_Start_005c9700() (0x005C9700)
// Unless the current dialog is 0xBC, enables control 0x59F of the first dialog found among
// the ids 0xC2, 0xC9, 0xBC and 0xBD; then clears unknown_67 of every session entry whose
// name is not the host's.

void Unknown_Enable_Start_005c9700()
{
    if (Current_Dialog_Id_00735b20() != 0xbc) {
        HWND dialog = WS_Find_Dialog_00735940(0xc2);
        if (dialog == 0) {
            dialog = WS_Find_Dialog_00735940(0xc9);
            if (dialog == 0) {
                dialog = WS_Find_Dialog_00735940(0xbc);
                if (dialog == 0) {
                    dialog = WS_Find_Dialog_00735940(0xbd);
                }
            }
        }
        EnableWindow(GetDlgItem(dialog, 0x59f), 1);
    }
    int count = unknown_players_a3fa7c.Count();
    for (int i = 0; i < count; i++) {
        if (Compare_Wide_007837d3(unknown_players_a3fa7c[i]->Name, unknown_host_name_a3f618) != 0 &&
            unknown_players_a3fa7c[i]->unknown_67 != 0) {
            unknown_players_a3fa7c[i]->unknown_67 = 0;
        }
    }
}


// int __fastcall Slot_Control_Id_005c97a0(int slot) (0x005C97A0)

int __fastcall Slot_Control_Id_005c97a0(int slot)
{
    if (slot == 0) {
        return 0x411;
    }
    if (slot == 1) {
        return 0x412;
    }
    if (slot == 2) {
        return 0x413;
    }
    if (slot == 3) {
        return 0x414;
    }
    if (slot == 4) {
        return 0x415;
    }
    if (slot == 5) {
        return 0x416;
    }
    if (slot == 6) {
        return 0x417;
    }
    if (slot == 7) { return 0x418; }
    return -1;
}


// int __fastcall Control_Slot_005c9800(int id) (0x005C9800)

int __fastcall Control_Slot_005c9800(int id)
{
    if (id == 0x411) {
        return 0;
    }
    if (id == 0x412) {
        return 1;
    }
    if (id == 0x413) {
        return 2;
    }
    if (id == 0x414) {
        return 3;
    }
    if (id == 0x415) {
        return 4;
    }
    if (id == 0x416) {
        return 5;
    }
    if (id == 0x417) {
        return 6;
    }
    return id == 0x418 ? 7 : -1;
}


// wchar_t const *__fastcall Entry_Type_Name_005c9870(NodeNameType *entry) (0x005C9870)

wchar_t const *__fastcall Entry_Type_Name_005c9870(NodeNameType *entry)
{
    if (entry == unknown_open_entry_a72d2c) {
        return StringTable::Load_String("STT:PlayerOpen", 0, "D:\\Ra2\\netshare.cpp", 0x11BA);
    }
    if (entry == unknown_closed_entry_a72d30) {
        return StringTable::Load_String("STT:PlayerClosed", 0, "D:\\Ra2\\netshare.cpp", 0x11BC);
    }
    if (entry == unknown_easy_ai_entry_a72d34) {
        return StringTable::Load_String("STT:PlayerDumbAI", 0, "D:\\Ra2\\netshare.cpp", 0x11BE);
    }
    if (entry == unknown_medium_ai_entry_a72d38) {
        return StringTable::Load_String("STT:PlayerSmartAI", 0, "D:\\Ra2\\netshare.cpp", 0x11C0);
    }
    if (entry == unknown_hard_ai_entry_a72d3c) {
        return StringTable::Load_String("STT:PlayerGeniusAI", 0, "D:\\Ra2\\netshare.cpp", 0x11C2);
    }
    if (entry == unknown_player_a72d44) {
        return StringTable::Load_String("STT:PlayerOpenObserver", 0, "D:\\Ra2\\netshare.cpp", 0x11C4);
    }
    return StringTable::Load_String("STT:PlayerHuman", 0, "D:\\Ra2\\netshare.cpp", 0x11C6);
}


// void __fastcall unknown_combo_select_by_data(HWND dialog, int id, int data) -- 0x005C9950
// Purpose: hides a dialog combo box while it selects (CB_SETCURSEL) the first
// item whose item data (CB_GETITEMDATA) equals `data`, then shows it again.
// Its one caller in the unit, 0x005CC700, expands it inline.

WIN32_IMPORT HWND __stdcall GetDlgItem(HWND dialog, int id);
WIN32_IMPORT BOOL __stdcall ShowWindow(HWND window, int command);
WIN32_IMPORT long __stdcall SendDlgItemMessageA(HWND dialog, int id, unsigned int message, unsigned int wparam,
                                                long lparam);

void __fastcall unknown_combo_select_by_data_005c9950(HWND dialog, int id, int data)
{
    HWND combo = GetDlgItem(dialog, id);
    ShowWindow(combo, 0);                                           // SW_HIDE
    int count = SendDlgItemMessageA(dialog, id, 0x146, 0, 0);       // CB_GETCOUNT
    for (int index = 0; index < count; index++) {
        if (SendDlgItemMessageA(dialog, id, 0x150, index, 0) == data) {   // CB_GETITEMDATA
            SendDlgItemMessageA(dialog, id, 0x14e, index, 0);              // CB_SETCURSEL
            break;
        }
    }
    ShowWindow(combo, 5);                                           // SW_SHOW
}


// void __fastcall Set_Slot_Country_005c9a10(HWND dialog, int id) (0x005C9A10)

void __fastcall Set_Slot_Country_005c9a10(HWND dialog, int id)
{
    int country = Side_Combo_Data_4d1c70(dialog, id, -1);
    int slot = Side_Combo_Player_4d13b0(id);
    if (slot == unknown_lobby_a3fa94) {
        unknown_session_a3d298.Set_Value_00672a30(country, false);
        if (unknown_players_a3fa7c.Count() != 0) {
            unknown_players_a3fa7c[0]->unknown_66e2d0(country);
        }
    } else {
        NodeNameType *entry = unknown_player_slots_a3fa98[slot];
        if (entry != 0 && (entry == unknown_easy_ai_entry_a72d34 || entry == unknown_medium_ai_entry_a72d38
                           || entry == unknown_hard_ai_entry_a72d3c)) {
            unknown_lobby_a3d2fc[slot] = country;
        } else {
            unknown_lobby_a3d2fc[slot] = -1;
            unknown_lobby_a3d2dc[slot] = -1;
            unknown_lobby_a3d31c[slot] = -1;
        }
    }
}


// void __fastcall Set_Slot_Color_005c9ab0(HWND dialog, int id) (0x005C9AB0)

void __fastcall Set_Slot_Color_005c9ab0(HWND dialog, int id)
{
    int color = Color_Combo_Data_4d2900(dialog, id, -1);
    int slot = Color_Combo_Player_4d1d10(id);
    if (slot == unknown_lobby_a3fa94) {
        Color_Combo_Changed_4d2700(dialog, id);
        if (Unknown_Is_Color_Taken_005c95d0(color)) {
            color = -2;
        }
        unknown_session_a3d298.Set_Value_00672ae0(color, false);
        if (unknown_players_a3fa7c.Count() != 0) {
            unknown_players_a3fa7c[0]->unknown_66e300(color);
        }
    } else {
        unknown_lobby_a3d31c[slot] = color;
    }
}


// void __fastcall Unknown_005c9b90(HWND dialog, int slot, bool redraw) (0x005C9B90)

void __fastcall Unknown_005c9b90(HWND dialog, int slot, bool redraw)
{
    int side_id = Player_Side_Combo_ID_4d1350(slot);
    int color_id = Player_Color_Combo_ID_4d1cb0(slot);
    if (unknown_lobby_flag_7e6d90 >= 0 && slot == unknown_lobby_flag_7e6d90
        && (unknown_player_slots_a3fa98[slot] == unknown_player_a72d44 || slot == 0
            || unknown_player_slots_a3fa98[slot]->Team == -1)) {
        Select_Side_Combo_4d1a90(dialog, side_id, -3);
        Select_Color_Combo_4d2480(dialog, color_id, -2);
        HWND side = GetDlgItem(dialog, side_id);
        EnableWindow(side, FALSE);
        HWND control = GetDlgItem(dialog, color_id);
        EnableWindow(control, FALSE);
        ShowWindow(control, 0);
        return;
    }
    if (!redraw) {
        Select_Side_Combo_4d1a90(dialog, side_id, -2);
        Select_Color_Combo_4d2480(dialog, color_id, -2);
    }
    HWND side = GetDlgItem(dialog, side_id);
    EnableWindow(side, redraw);
    HWND control = GetDlgItem(dialog, color_id);
    EnableWindow(control, redraw);
    int limit = Scenario_Slot_Limit_005c65a0(unknown_session_a3d2b4);
    if (unknown_lobby_flag_7e6d90 == 0 && limit < 7) {
        limit++;
    }
    if (slot < limit || slot == unknown_lobby_flag_7e6d90) {
        ShowWindow(control, 5);
    }
}


// void __fastcall Refresh_Slots_005c9cf0(HWND dialog, int slot) (0x005C9CF0)

void __fastcall Refresh_Slots_005c9cf0(HWND dialog, int slot)
{
    for (int i = 0; i < 8; i++) {
        HWND control = GetDlgItem(dialog, Slot_Control_Id_005c97a0(i));
        EnableWindow(control, TRUE);
        Unknown_005c9b90(dialog, i, 0);
    }
    Unknown_005c9b90(dialog, slot, 1);
}


// void __fastcall Lock_Slots_005c9da0(HWND dialog, int slot) (0x005C9DA0)

void __fastcall Lock_Slots_005c9da0(HWND dialog, int slot)
{
    for (int i = 0; i < 8; i++) {
        HWND control = GetDlgItem(dialog, Slot_Control_Id_005c97a0(i));
        EnableWindow(control, FALSE);
        Unknown_005c9b90(dialog, i, 0);
    }
    HWND control = GetDlgItem(dialog, Slot_Control_Id_005c97a0(slot));
    EnableWindow(control, TRUE);
    Unknown_005c9b90(dialog, slot, 1);
}


// void __fastcall Refresh_Slot_Images_005c9ec0(HWND dialog) (0x005C9EC0)

// One slot's status image (also expanded in 0x005CA000).
static void Refresh_Slot_Image(HWND dialog, int slot)
{
    HWND control = GetDlgItem(dialog, Slot_Color_Control_Id_005ca4e0(slot));
    Surface *image = 0;
    if (Is_Player_Entry(unknown_player_slots_a3fa98[slot]) && unknown_player_slots_a3fa98[slot] != unknown_easy_ai_entry_a72d34
        && unknown_player_slots_a3fa98[slot] != unknown_medium_ai_entry_a72d38
        && unknown_player_slots_a3fa98[slot] != unknown_hard_ai_entry_a72d3c
        && Compare_Wide_007837d3(unknown_player_slots_a3fa98[slot]->Name, unknown_host_name_a3f618) == 0) {
        image = unknown_bitmap_writer_a76378.Get_Image_0068ca40(unknown_host_image_7e6db8, 0);
    } else if (Is_Accepted_Slot_005ca3b0(slot) == 1) {
        image = unknown_bitmap_writer_a76378.Get_Image_0068ca40(unknown_accept_image_7e6dbc, 0);
    }
    OwnerDraw::unknown_5e1f30(control, image);
}

void __fastcall Refresh_Slot_Images_005c9ec0(HWND dialog)
{
    for (int i = 0; i < 8; i++) {
        Refresh_Slot_Image(dialog, i);
    }
}


// void __fastcall Refresh_Slot_Settings_005ca000(HWND dialog, bool full) (0x005CA000)
// Refreshes the lobby dialog's slot controls. With full set it rebuilds the slot rows (0x005CA6C0) and
// redoes every country flag, side and colour combo box; otherwise it redoes the status images and only
// what changed since the last call: the slot names (rows rebuilt), the countries and the colours, each
// compared as a text list.

// The slots' country flags and side combo boxes (helpers with no out-of-line copy: both expansions in
// 0x005CA000 are identical).
static void Refresh_Country_Flags(HWND dialog)
{
    for (int i = 0; i < 8; i++) {
        HWND control = GetDlgItem(dialog, unknown_player_control_4d0ee0(i));
        OwnerDraw::unknown_5e1f30(control, Country_Flag_004d1120(Slot_Country_005ca3f0(i)));
    }
}

static void Refresh_Side_Combos(HWND dialog)
{
    for (int i = 0; i < 8; i++) {
        int id = Player_Side_Combo_ID_4d1350(i);
        Select_Side_Combo_4d1a90(dialog, id, Slot_Country_005ca3f0(i));
    }
}

static bool Slot_Names_Changed()
{
    wcscpy(unknown_slot_names_old_a728e4, unknown_slot_names_a72734);
    Unknown_Slot_Names_005c8cf0(unknown_slot_names_a72734);
    return Compare_Wide_007837d3(unknown_slot_names_old_a728e4, unknown_slot_names_a72734) != 0;
}

static bool Country_List_Changed()
{
    char text[4];
    strcpy(unknown_country_list_old_a72a44, unknown_country_list_a726f8);
    unknown_country_list_a726f8[0] = 0;
    for (int i = 0; i < 8; i++) {
        sprintf(text, "%d,", Slot_Country_005ca3f0(i));
        strcat(unknown_country_list_a726f8, text);
    }
    return strcmp(unknown_country_list_old_a72a44, unknown_country_list_a726f8) != 0;
}

static bool Color_List_Changed()
{
    char text[4];
    strcpy(unknown_color_list_old_a726b0, unknown_color_list_a726d4);
    unknown_color_list_a726d4[0] = 0;
    for (int i = 0; i < 8; i++) {
        sprintf(text, "%d,", Slot_Color_005ca480(i));
        strcat(unknown_color_list_a726d4, text);
    }
    return strcmp(unknown_color_list_old_a726b0, unknown_color_list_a726d4) != 0;
}

void __fastcall Refresh_Slot_Settings_005ca000(HWND dialog, bool full)
{
    if (full) {
        Unknown_005ca6c0(dialog);
        Refresh_Country_Flags(dialog);
        Refresh_Side_Combos(dialog);
        Refresh_Slot_Colors_005ca540(dialog);
        if (unknown_game_mode_a3d298 == 4) {
            Unknown_0075b0c0();
        }
        return;
    }
    for (int slot = 0; slot < 8; slot++) {
        Refresh_Slot_Image(dialog, slot);
    }
    if (Slot_Names_Changed()) {
        Unknown_005ca6c0(dialog);
        if (unknown_game_mode_a3d298 == 4) {
            Unknown_0075b0c0();
        }
    }
    Refresh_Country_Flags(dialog);
    if (Country_List_Changed()) {
        Refresh_Side_Combos(dialog);
    }
    if (Color_List_Changed()) {
        Refresh_Slot_Colors_005ca540(dialog);
    }
}


// int __fastcall Is_Accepted_Slot_005ca3b0(int slot) (0x005CA3B0)

int __fastcall Is_Accepted_Slot_005ca3b0(int slot)
{
    NodeNameType *entry = unknown_player_slots_a3fa98[slot];
    int accepted = 0;
    if ((entry != 0 && (entry == unknown_easy_ai_entry_a72d34 || entry == unknown_medium_ai_entry_a72d38
                        || entry == unknown_hard_ai_entry_a72d3c))
        || entry->unknown_67 == 1) {
        accepted = 1;
    }
    return accepted;
}


// int __fastcall Slot_Country_005ca3f0(int slot) (0x005CA3F0)

int __fastcall Slot_Country_005ca3f0(int slot)
{
    int country = -2;
    NodeNameType *entry = unknown_player_slots_a3fa98[slot];
    if (unknown_session_a3d29c != 0 && (unknown_game_mode_a3d298 == 3 || unknown_game_mode_a3d298 == 4)) {
        country = unknown_session_a3d29c->unknown_slot10();
    }
    if (entry != 0 && (entry == unknown_easy_ai_entry_a72d34 || entry == unknown_medium_ai_entry_a72d38
                       || entry == unknown_hard_ai_entry_a72d3c)) {
        return unknown_lobby_a3d2fc[slot];
    }
    if (entry == unknown_open_entry_a72d2c || entry == unknown_closed_entry_a72d30
        || entry == unknown_waiting_entry_a72d40) {
        return country;
    }
    if (entry == unknown_player_a72d44) {
        return -3;
    }
    return entry->unknown_66e290();
}


// int __fastcall Slot_Color_005ca480(int slot) (0x005CA480)

int __fastcall Slot_Color_005ca480(int slot)
{
    NodeNameType *entry = unknown_session_a3d298.PlayerSlots[slot];
    if (entry != 0 && (entry == unknown_easy_ai_entry_a72d34 || entry == unknown_medium_ai_entry_a72d38
                       || entry == unknown_hard_ai_entry_a72d3c)) {
        return unknown_session_a3d298.AIColor[slot];
    }
    if (entry == unknown_open_entry_a72d2c) {
        return -2;
    }
    if (entry == unknown_closed_entry_a72d30) {
        return -2;
    }
    if (entry == unknown_waiting_entry_a72d40) {
        return -2;
    }
    if (entry == unknown_player_a72d44) {
        return -2;
    }
    return entry->Get_Color_0066e2a0();
}


// int __fastcall Slot_Color_Control_Id_005ca4e0(int slot) (0x005CA4E0)

int __fastcall Slot_Color_Control_Id_005ca4e0(int slot)
{
    if (slot == 0) {
        return 0x6d2;
    }
    if (slot == 1) {
        return 0x6d3;
    }
    if (slot == 2) {
        return 0x6d4;
    }
    if (slot == 3) {
        return 0x6d5;
    }
    if (slot == 4) {
        return 0x6d6;
    }
    if (slot == 5) {
        return 0x6d7;
    }
    if (slot == 6) {
        return 0x6d8;
    }
    if (slot == 7) { return 0x6d9; }
    return -1;
}


// void __fastcall Refresh_Slot_Colors_005ca540(HWND dialog) (0x005CA540)
// Selects every slot's colour in its colour combo box, the local player's first.

void __fastcall Refresh_Slot_Colors_005ca540(HWND dialog)
{
    int own = Find_First_Player_Slot_005c8c70();
    if (own != -1) {
        int id = Player_Color_Combo_ID_4d1cb0(own);
        Select_Color_Combo_4d2480(dialog, id, Slot_Color_005ca480(own));
    }
    for (int i = 0; i < 8; i++) {
        if (i != own) {
            int id = Player_Color_Combo_ID_4d1cb0(i);
            Select_Color_Combo_4d2480(dialog, id, Slot_Color_005ca480(i));
        }
    }
}


// void __fastcall Unknown_005ca6c0(HWND dialog) (0x005CA6C0)
// Refills the eight slot combo boxes (0x411..0x418): each lists its current entry (open, closed, AI levels,
// waiting, open observer or a player's name); the host also gets the choices for the slot (open within the
// slot limit, open observer for the local slot, closed, and the AI levels when computer players are allowed).
// Each combo is hidden while it is refilled and its current text selected again.


// Adds a row with the given text to a slot combo box (0x4C2) and stores the slot entry as its item data.
// The entry is taken by reference: it is read after the row is added (the globals' loads follow the call).
static inline void Add_Combo_Entry(HWND combo, wchar_t const *text, NodeNameType * const &entry)
{
    int index = SendMessageA(combo, 0x4C2, 0, (LPARAM)text);
    SendMessageA(combo, CB_SETITEMDATA, index, (LPARAM)entry);
}

void __fastcall Unknown_005ca6c0(HWND dialog)
{
    bool ai_allowed = (unknown_session_a3d29c == 0 || unknown_session_a3d29c->unknown_slot12()) && !unknown_73c370();
    int limit = Lobby_Slot_Limit_005c6520();
    if (unknown_lobby_flag_7e6d90 == 0) {
        if (ai_allowed || limit < Lobby_Slot_Limit_Plus_One_005c6550()) {
            limit++;
        }
    }
    for (int slot = 0; slot < 8; slot++) {
        bool beyond_map = false;
        if (slot == unknown_lobby_flag_7e6d90) {
            if (slot > Scenario_Slot_Limit_005c65a0(unknown_session_a3d2b4)) {
                NodeNameType *seated = unknown_player_slots_a3fa98[slot];
                if (seated != unknown_open_entry_a72d2c
                    && (seated == 0 || (seated != unknown_easy_ai_entry_a72d34 && seated != unknown_medium_ai_entry_a72d38
                                        && seated != unknown_hard_ai_entry_a72d3c))) {
                    beyond_map = true;
                }
            }
            if (slot == unknown_lobby_flag_7e6d90 && unknown_session_a3d29c != 0) {
                unknown_session_a3d29c->unknown_slot40();
            }
        }
        int id = Slot_Control_Id_005c97a0(slot);
        HWND combo = GetDlgItem(dialog, id);
        bool hidden = ShowWindow(combo, SW_HIDE) == 0;
        SendDlgItemMessageA(dialog, id, CB_RESETCONTENT, 0, 0);
        NodeNameType *entry = unknown_player_slots_a3fa98[slot];
        wchar_t const *text = entry->Name;
        if (entry == unknown_open_entry_a72d2c) {
            text = StringTable::Load_String("GUI:Open", 0, "D:\\Ra2\\netshare.cpp", 0x1315);
        } else if (entry == unknown_closed_entry_a72d30) {
            text = StringTable::Load_String("GUI:Closed", 0, "D:\\Ra2\\netshare.cpp", 0x1317);
        } else if (entry == unknown_easy_ai_entry_a72d34) {
            text = StringTable::Load_String("GUI:AIEasy", 0, "D:\\Ra2\\netshare.cpp", 0x1319);
        } else if (entry == unknown_medium_ai_entry_a72d38) {
            text = StringTable::Load_String("GUI:AINormal", 0, "D:\\Ra2\\netshare.cpp", 0x131b);
        } else if (entry == unknown_hard_ai_entry_a72d3c) {
            text = StringTable::Load_String("GUI:AIHard", 0, "D:\\Ra2\\netshare.cpp", 0x131d);
        } else if (entry == unknown_waiting_entry_a72d40) {
            text = StringTable::Load_String("GUI:Waiting", 0, "D:\\Ra2\\netshare.cpp", 0x131f);
        } else if (entry == unknown_player_a72d44) {
            text = StringTable::Load_String("GUI:OpenObserver", 0, "D:\\Ra2\\netshare.cpp", 0x1321);
        }
        if (entry != 0
            && (entry == unknown_easy_ai_entry_a72d34 || entry == unknown_medium_ai_entry_a72d38
                || entry == unknown_hard_ai_entry_a72d3c)
            && !ai_allowed) {
            text = StringTable::Load_String("GUI:Open", 0, "D:\\Ra2\\netshare.cpp", 0x1328);
        }
        WideString current(text);
        if (Is_Host_005c86a0()) {
            if (entry != 0 && entry != unknown_open_entry_a72d2c && entry != unknown_closed_entry_a72d30
                && entry != unknown_waiting_entry_a72d40 && entry != unknown_player_a72d44
                && entry != unknown_easy_ai_entry_a72d34 && entry != unknown_medium_ai_entry_a72d38
                && entry != unknown_hard_ai_entry_a72d3c) {
                Add_Combo_Entry(combo, text, entry);
            }
            if (!Is_Local_Name_005c86c0(text)) {
                if (slot < limit
                    && (!beyond_map || slot != unknown_lobby_flag_7e6d90
                        || slot < Scenario_Slot_Limit_005c65a0(unknown_session_a3d2b4))) {
                    Add_Combo_Entry(combo, StringTable::Load_String("GUI:Open", 0, "D:\\Ra2\\netshare.cpp", 0x1340), unknown_open_entry_a72d2c);
                }
                if (slot == unknown_lobby_flag_7e6d90) {
                    Add_Combo_Entry(combo, StringTable::Load_String("GUI:OpenObserver", 0, "D:\\Ra2\\netshare.cpp", 0x134a), unknown_player_a72d44);
                }
                Add_Combo_Entry(combo, StringTable::Load_String("GUI:Closed", 0, "D:\\Ra2\\netshare.cpp", 0x1351), unknown_closed_entry_a72d30);
                if (ai_allowed && !beyond_map) {
                    Add_Combo_Entry(combo, StringTable::Load_String("GUI:AIEasy", 0, "D:\\Ra2\\netshare.cpp", 0x1359), unknown_easy_ai_entry_a72d34);
                    Add_Combo_Entry(combo, StringTable::Load_String("GUI:AINormal", 0, "D:\\Ra2\\netshare.cpp", 0x135f), unknown_medium_ai_entry_a72d38);
                    Add_Combo_Entry(combo, StringTable::Load_String("GUI:AIHard", 0, "D:\\Ra2\\netshare.cpp", 0x1365), unknown_hard_ai_entry_a72d3c);
                }
            }
        } else {
            Add_Combo_Entry(combo, text, entry);
        }
        SendMessageA(combo, 0x4C0, -1, (LPARAM)current.unknown_text_770430());
        if (!hidden) {
            ShowWindow(combo, SW_SHOW);
        }
    }
}


// bool __fastcall Unknown_Clear_Closed_Slots_005cacb0(HWND dialog) (0x005CACB0)
// Walks the eight player slots' combo boxes (controls 0x411..0x418): a slot whose
// selected item data is the third, fourth or fifth fixed entry is reset to the
// first fixed entry within the slot limit (2, 4 or 8 by game mode) or to the second
// beyond it, with its three setting tables cleared. Answers whether any changed.


bool __fastcall Unknown_Clear_Closed_Slots_005cacb0(HWND dialog)
{
    bool changed = false;
    int limit;
    if (unknown_game_mode_a3d298 == 4) {
        if (unknown_73c370() && !unknown_73c3a0()) {
            limit = 2;
        } else {
            limit = 4;
        }
    } else {
        limit = 8;
    }
    for (int i = 0; i < 8; i++) {
        int id;
        if (i == 0) {
            id = 0x411;
        } else if (i == 1) {
            id = 0x412;
        } else if (i == 2) {
            id = 0x413;
        } else if (i == 3) {
            id = 0x414;
        } else if (i == 4) {
            id = 0x415;
        } else if (i == 5) {
            id = 0x416;
        } else if (i == 6) {
            id = 0x417;
        } else {
            id = i == 7 ? 0x418 : -1;
        }
        WPARAM selected = SendDlgItemMessageA(dialog, id, 0x147, 0, 0);
        LRESULT data = SendDlgItemMessageA(dialog, id, 0x150, selected, 0);
        if (data != 0 && (data == (LRESULT)unknown_easy_ai_entry_a72d34 ||
                          data == (LRESULT)unknown_medium_ai_entry_a72d38 ||
                          data == (LRESULT)unknown_hard_ai_entry_a72d3c)) {
            if (i < limit) {
                unknown_player_slots_a3fa98[i] = unknown_open_entry_a72d2c;
            } else {
                unknown_player_slots_a3fa98[i] = unknown_closed_entry_a72d30;
            }
            Unknown_005c9b90(dialog, i, 0);
            unknown_lobby_a3d2dc[i] = -1;
            unknown_lobby_a3d2fc[i] = -1;
            unknown_lobby_a3d31c[i] = -1;
            changed = true;
        }
    }
    return changed;
}


// Show player rows, full original005CAE00 body450+14 frozen padding.
void __fastcall Unknown_005cae00(HWND dialog,int first,int count)
{
    int end=first+count;
    for(int slot=first;slot<end;++slot) {
        int color_id=Slot_Color_Control_Id_005ca4e0(slot);
        int slot_id=Slot_Control_Id_005c97a0(slot);
        int player_id=unknown_player_control_4d0ee0(slot);
        int shade_id=Player_Color_Combo_ID_4d1cb0(slot);
        int side_id=Player_Side_Combo_ID_4d1350(slot);
        HWND color=GetDlgItem(dialog,color_id);
        HWND combo=GetDlgItem(dialog,slot_id);
        HWND player=GetDlgItem(dialog,player_id);
        HWND shade=GetDlgItem(dialog,shade_id);
        HWND side=GetDlgItem(dialog,side_id);
        NodeNameType *entry=unknown_player_slots_a3fa98[slot];
        bool observer;
        if(entry==unknown_player_a72d44 || (entry!=0 && entry!=unknown_open_entry_a72d2c && entry!=unknown_closed_entry_a72d30 && entry!=unknown_waiting_entry_a72d40 && entry->Team==-1)) observer=true;
        else observer=false;
        ShowWindow(color,5);
        ShowWindow(combo,5);
        ShowWindow(player,5);
        if(observer) ShowWindow(shade,0);
        else ShowWindow(shade,5);
        ShowWindow(side,5);
    }
}


// void __fastcall Unknown_005cafd0(HWND dialog) (0x005CAFD0)

void __fastcall Unknown_005cafd0(HWND dialog)
{
    int slot = unknown_lobby_flag_7e6d90;
    if (slot < 0) {
        return;
    }
    if (slot == 0) {
        slot = unknown_int_a3db1c;
    }
    if (slot >= 8) {
        return;
    }
    int color_id;
    int slot_id;
    if (slot == 0) {
        color_id = 0x6D2;
        slot_id = 0x411;
    } else if (slot == 1) {
        color_id = 0x6D3;
        slot_id = 0x412;
    } else if (slot == 2) {
        color_id = 0x6D4;
        slot_id = 0x413;
    } else if (slot == 3) {
        color_id = 0x6D5;
        slot_id = 0x414;
    } else if (slot == 4) {
        color_id = 0x6D6;
        slot_id = 0x415;
    } else if (slot == 5) {
        color_id = 0x6D7;
        slot_id = 0x416;
    } else if (slot == 6) {
        color_id = 0x6D8;
        slot_id = 0x417;
    } else if (slot == 7) {
        color_id = 0x6D9;
        slot_id = 0x418;
    } else {
        color_id = -1;
        slot_id = -1;
    }
    int player_id = unknown_player_control_4d0ee0(slot);
    int color_combo_id = Player_Color_Combo_ID_4d1cb0(slot);
    int side_combo_id = Player_Side_Combo_ID_4d1350(slot);
    NodeNameType *entry = unknown_player_slots_a3fa98[slot];
    bool observer;
    if (entry == unknown_player_a72d44 || (entry != 0 && entry != unknown_open_entry_a72d2c
        && entry != unknown_closed_entry_a72d30 && entry != unknown_waiting_entry_a72d40 && entry->Team == -1)) {
        observer = true;
    } else {
        observer = false;
    }
    HWND color = GetDlgItem(dialog, color_id);
    HWND combo = GetDlgItem(dialog, slot_id);
    HWND player = GetDlgItem(dialog, player_id);
    HWND color_combo = GetDlgItem(dialog, color_combo_id);
    HWND side_combo = GetDlgItem(dialog, side_combo_id);
    ShowWindow(color, 5);
    ShowWindow(combo, 5);
    ShowWindow(player, 5);
    if (!observer) {
        ShowWindow(color_combo, 5);
        ShowWindow(side_combo, 5);
    } else {
        ShowWindow(color_combo, 0);
        ShowWindow(side_combo, 5);
    }
}


// Hide unused player rows, full original 005CB170..005CB300 (400 bytes).
void __fastcall Unknown_005cb170(HWND dialog,int first)
{
    for(int slot=first;slot<8;++slot) {
        if(slot==unknown_lobby_flag_7e6d90) continue;
        int color_id=Slot_Color_Control_Id_005ca4e0(slot);
        int slot_id=Slot_Control_Id_005c97a0(slot);
        int player_id=unknown_player_control_4d0ee0(slot);
        int side_id=Player_Side_Combo_ID_4d1350(slot);
        int shade_id=Player_Color_Combo_ID_4d1cb0(slot);
        HWND color=GetDlgItem(dialog,color_id);
        HWND entry=GetDlgItem(dialog,slot_id);
        HWND player=GetDlgItem(dialog,player_id);
        HWND side=GetDlgItem(dialog,side_id);
        HWND shade=GetDlgItem(dialog,shade_id);
        ShowWindow(color,0);
        ShowWindow(entry,0);
        ShowWindow(player,0);
        ShowWindow(side,0);
        ShowWindow(shade,0);
    }
}


// void __fastcall Change_Slot_Count_005cb300(HWND dialog, int old_count, int new_count, UnknownSessionA3D29C *mode) (0x005CB300)

void __fastcall Change_Slot_Count_005cb300(HWND dialog, int old_count, int new_count, UnknownSessionA3D29C *mode)
{
    if (unknown_session_a3d29c != 0 && !unknown_session_a3d29c->unknown_slot12() && !unknown_73c370()) {
        int limit = Lobby_Slot_Limit_005c6520();
        if (old_count > limit) {
            old_count = limit;
        }
    }
    if ((mode != 0 && !mode->unknown_slot12()) || unknown_73c370()) {
        int limit = Lobby_Slot_Limit_005c6520();
        if (new_count > limit) {
            new_count = limit;
        }
    }
    int added = new_count - old_count;
    if (added > 0) {
        Unknown_005cae00(dialog, old_count, added);
        Unknown_005cafd0(dialog);
    } else {
        if (added < 0) {
            Unknown_005cb170(dialog, new_count);
        }
        Unknown_005cafd0(dialog);
    }
}


// void __fastcall Unknown_005cb3f0(HWND dialog, int first, int count) (0x005CB3F0)

void __fastcall Unknown_005cb3f0(HWND dialog, int first, int count)
{
    int end = first + count;
    int limit = Lobby_Slot_Limit_005c6520();
    for (int i = first; i < end; i++) {
        int id = Slot_Control_Id_005c97a0(i);
        int index = SendDlgItemMessageA(dialog, id, 0x147, 0, 0);
        NodeNameType *entry = (NodeNameType *)SendDlgItemMessageA(dialog, id, 0x150, index, 0);
        if (entry != (NodeNameType *)-1 && Is_Seated_Entry(entry)) {
            Unknown_005cbf20(dialog, id, entry, 0);
        } else if (i != unknown_lobby_flag_7e6d90) {
            if (i < limit) {
                Unknown_005cbf20(dialog, id, unknown_open_entry_a72d2c, 0);
            } else {
                Unknown_005cbf20(dialog, id, unknown_closed_entry_a72d30, 0);
            }
        }
    }
    Unknown_005cae00(dialog, first, count);
    Unknown_005bbdc0();
    Unknown_005c3810(true, false);
}


// void __fastcall Clear_Observer_Slots_005cb560(HWND dialog) (0x005CB560)
// Without an observer slot (unknown_lobby_flag_7e6d90 below 1) every slot but the first that holds the observer
// entry or an observer player (Team -1) becomes open within the slot limit and closed beyond. With one, that
// slot becomes closed when the map has fewer slots and it holds a seated non-observer player, a computer or
// the open entry; its row is shown and refilled either way. Then the colours are redone after a change, the
// LAN lobby is refreshed and the lobby state sent.

void __fastcall Clear_Observer_Slots_005cb560(HWND dialog)
{
    bool changed = false;
    if (unknown_lobby_flag_7e6d90 < 1) {
        int limit = Lobby_Slot_Limit_005c6520();
        for (int slot = 1; slot < 8; slot++) {
            int id = Slot_Control_Id_005c97a0(slot);
            NodeNameType *entry = unknown_player_slots_a3fa98[slot];
            if (slot < limit) {
                if (entry == unknown_player_a72d44 || entry->Team == -1) {
                    Unknown_005cbf20(dialog, id, unknown_open_entry_a72d2c, 1);
                    changed = true;
                }
            } else if (entry == unknown_player_a72d44 || entry->Team == -1) {
                Unknown_005cbf20(dialog, id, unknown_closed_entry_a72d30, 1);
                changed = true;
            }
        }
    } else {
        NodeNameType *entry = unknown_player_slots_a3fa98[unknown_lobby_flag_7e6d90];
        if (unknown_lobby_flag_7e6d90 > Scenario_Slot_Limit_005c65a0(unknown_session_a3d2b4)) {
            if ((Is_Seated_Entry(entry) && entry->Team != -1)
                || (entry != 0 && (entry == unknown_easy_ai_entry_a72d34 || entry == unknown_medium_ai_entry_a72d38
                                   || entry == unknown_hard_ai_entry_a72d3c))
                || entry == unknown_open_entry_a72d2c) {
                entry = unknown_closed_entry_a72d30;
                changed = true;
            }
        }
        Unknown_005cafd0(dialog);
        Unknown_005cbf20(dialog, Slot_Control_Id_005c97a0(unknown_lobby_flag_7e6d90), entry, 1);
    }
    if (changed) {
        Set_Slot_Country_005c9a10(dialog, Player_Side_Combo_ID_4d1350(unknown_lobby_flag_7e6d90));
    }
    Unknown_005bbdc0();
    Unknown_005c3810(true, false);
}


// void __fastcall Reset_Slots_From_005cb7c0(HWND dialog, int first) (0x005CB7C0)

void __fastcall Reset_Slots_From_005cb7c0(HWND dialog, int first)
{
    int limit = Lobby_Slot_Limit_005c6520();
    for (int i = first; i < 8; i++) {
        int id = Slot_Control_Id_005c97a0(i);
        if (i < limit) {
            if (i != unknown_lobby_flag_7e6d90) {
                Unknown_005cbf20(dialog, id, unknown_open_entry_a72d2c, 1);
            }
        } else if (i != unknown_lobby_flag_7e6d90) {
            Unknown_005cbf20(dialog, id, unknown_closed_entry_a72d30, 1);
        }
    }
    Unknown_005cb170(dialog, first);
    if (unknown_chat_a72cfc != 0) {
        unknown_call_73fee0_default();
    } else {
        Unknown_005bbdc0();
    }
    Unknown_005c3810(true, false);
}


// void __fastcall Change_Scenario_Slots_005cb8d0(HWND dialog, int previous, int scenario) (0x005CB8D0)
// Adapts the slot combos to a new scenario: its player count (limited to the lobby's slot limit when
// computer players are not allowed) against the previous count opens the added slots or closes the removed
// ones, an extra open slot is kept when the lobby allows one more; previous -1 resets the counts and the
// host's observer combo. Afterwards the observer slots are redone and, when the mode wants slot 7 as the
// observer slot, the host announces it. The counts are kept for the next call.

void __fastcall Change_Scenario_Slots_005cb8d0(HWND dialog, int previous, int scenario)
{
    int players = Scenario_Slot_Limit_005c65a0(scenario);
    bool ai_allowed = (unknown_session_a3d29c == 0 || unknown_session_a3d29c->unknown_slot12()) && !unknown_73c370();
    if (!ai_allowed) {
        int limit = Lobby_Slot_Limit_005c6520();
        if (players > limit) {
            players = limit;
        }
    }
    if (previous == -1) {
        unknown_slot_count_7e6de4 = 8;
        unknown_extra_slot_a72d5c = 0;
    }
    int added = players - unknown_slot_count_7e6de4;
    int extra = 0;
    if (unknown_lobby_flag_7e6d90 == 0) {
        if (ai_allowed || players < Lobby_Slot_Limit_Plus_One_005c6550()) {
            extra = 1;
            if (unknown_extra_slot_a72d5c == 0 && added + players < 7) {
                added++;
            }
        }
    }
    if (previous == -1 && unknown_lobby_flag_7e6d90 == 0) {
        int side = Player_Side_Combo_ID_4d1350(0);
        Fill_Observer_Combo_4d1640(dialog, side);
        Select_Side_Combo_4d1a90(dialog, side, -3);
        SendMessageA(GetDlgItem(dialog, 0x411), 0x4F1, 0, 1);
        int color = Player_Color_Combo_ID_4d1cb0(0);
        Fill_Random_Color_Combo_4d2250(dialog, color);
        Select_Color_Combo_4d2480(dialog, color, 8);
    }
    if (added > 0) {
        Unknown_005cb3f0(dialog, unknown_extra_slot_a72d5c + unknown_slot_count_7e6de4, added);
    } else if (added < 0) {
        int first = extra + players;
        int limit = Lobby_Slot_Limit_005c6520();
        for (int slot = first; slot < 8; slot++) {
            int id = Slot_Control_Id_005c97a0(slot);
            if (slot < limit) {
                if (slot != unknown_lobby_flag_7e6d90) {
                    Unknown_005cbf20(dialog, id, unknown_open_entry_a72d2c, 1);
                }
            } else if (slot != unknown_lobby_flag_7e6d90) {
                Unknown_005cbf20(dialog, id, unknown_closed_entry_a72d30, 1);
            }
        }
        Unknown_005cb170(dialog, first);
        if (unknown_chat_a72cfc != 0) {
            unknown_call_73fee0_default();
        } else {
            Unknown_005bbdc0();
        }
        Unknown_005c3810(true, false);
    } else if (extra + players >= 7) {
        Unknown_005cb3f0(dialog, 7, 1);
    }
    Unknown_005cb560(dialog);
    if (unknown_lobby_flag_7e6d90 == -1 && unknown_session_a3d29c != 0 && unknown_session_a3d29c->unknown_slot40()) {
        unknown_lobby_flag_7e6d90 = 7;
        Unknown_005cb560(dialog);
        Debug_Printf("Sending observer slot of 7\n");
        char text[0x100];
        NodeNameType *entry = unknown_player_slots_a3fa98[unknown_lobby_flag_7e6d90];
        WideString name;
        if (entry != 0 && entry != unknown_open_entry_a72d2c && entry != unknown_closed_entry_a72d30
            && entry != unknown_waiting_entry_a72d40 && entry != unknown_player_a72d44
            && entry != unknown_easy_ai_entry_a72d34 && entry != unknown_medium_ai_entry_a72d38
            && entry != unknown_hard_ai_entry_a72d3c) {
            name = entry->Name;
        } else {
            Wstring narrow = Unknown_Lobby_Entry_Name_005cd0e0(entry);
            name = Wstring_To_Wide(narrow);
        }
        {
            WideString wide(name.Get());
            sprintf(text, "O%d%s", unknown_lobby_flag_7e6d90, Narrow_Copy_00771280(wide).Buffer);
        }
        Send_Game_Options_005c3c60(text);
    }
    unknown_slot_count_7e6de4 = players;
    unknown_extra_slot_a72d5c = extra;
}


// bool __fastcall Unknown_005cbdb0(HWND dialog, int count, bool unknown_1, bool unknown_2) (0x005CBDB0)

bool __fastcall Unknown_005cbdb0(HWND dialog, int count, bool unknown_1, bool unknown_2)
{
    if (unknown_lobby_flag_7e6d90 == 0) {
        count++;
    }
    for (int i = 0; i < 8; i++) {
        int id = Slot_Control_Id_005c97a0(i);
        int index = SendDlgItemMessageA(dialog, id, 0x147, 0, 0);
        int data = SendDlgItemMessageA(dialog, id, 0x150, index, 0);
        NodeNameType *entry;
        if (data == -1) {
            entry = 0;
        } else {
            entry = (NodeNameType *)data;
        }
        if (entry != 0 && (entry == unknown_easy_ai_entry_a72d34 || entry == unknown_medium_ai_entry_a72d38
                           || entry == unknown_hard_ai_entry_a72d3c)
            && unknown_session_a3d29c != 0 && !unknown_1) {
            return true;
        }
        if (i >= count) {
            bool player = entry != 0 && entry != unknown_open_entry_a72d2c && entry != unknown_closed_entry_a72d30
                && entry != unknown_player_a72d44 && entry != unknown_waiting_entry_a72d40;
            if (player && entry->unknown_66e290() != -3) {
                return true;
            }
            if (unknown_session_a3d29c != 0 && !unknown_2 && player && entry->unknown_66e290() == -3) {
                return true;
            }
        }
    }
    return false;
}


// Not exact (RA2_NON_MATCHING): Unknown_005cbf20 at 0x005CBF20, best score 53.74, 2026-11-13; differs: 619 instructions in the original, 624 in the draft (shape 85.92)
#ifdef RA2_NON_MATCHING
// void __fastcall Unknown_005cbf20(HWND dialog, int id, NodeNameType *entry, int update) (0x005CBF20)
// Puts an entry into the slot of a slot combo box: a computer is refused (and the combo box reset) while an
// observer is seated or the observer mode is on, an observer boots every computer; then the slot's side and
// colour combo boxes and computer settings follow the change, update 1 and 2 send the slot names and 2 also
// the lobby state.
// NATIVE (Scout1209 r9): the callees under their accepted names and declarations.
void __fastcall unknown_combo_select_by_data_005c9950(HWND dialog, int id, int data);
int Native_Entry_Handle(void const *entry);   // NATIVE: native/link/native_event_data.cpp (32-bit combo data)
void __fastcall Kick_Channel_User_Wide_0075cae0(wchar_t const *name);
void __fastcall Unknown_0073c480();
void __fastcall Update_Channel_Player_Limit_00749f20();

void __fastcall Unknown_005cbf20(HWND dialog, int id, NodeNameType *entry, int update)
{
    int slot = Control_Slot_005c9800(id);
    NodeNameType *previous = unknown_player_slots_a3fa98[slot];
    if (entry != 0 && (entry == unknown_easy_ai_entry_a72d34 || entry == unknown_medium_ai_entry_a72d38
            || entry == unknown_hard_ai_entry_a72d3c) && slot != unknown_lobby_flag_7e6d90) {
        if (unknown_a72c68 != 0 || (unknown_lobby_flag_7e6d90 > 0
                && (unknown_player_slots_a3fa98[unknown_lobby_flag_7e6d90]->Team == -1
                    || unknown_player_slots_a3fa98[unknown_lobby_flag_7e6d90] == unknown_player_a72d44))) {
            Unknown_005C2C80(-1, StringTable::Load_String("TXT_NO_AI_WITH_OBSERVER", 0, "D:\\Ra2\\netshare.cpp", 0x15A2));
            NodeNameType *kept = previous;
            if (previous != 0 && (previous == unknown_easy_ai_entry_a72d34 || previous == unknown_medium_ai_entry_a72d38
                    || previous == unknown_hard_ai_entry_a72d3c)) {
                kept = unknown_closed_entry_a72d30;
            }
            previous = entry;
            unknown_combo_select_by_data_005c9950(dialog, id, Native_Entry_Handle(kept));   // NATIVE: handle
        }
    } else if (entry == unknown_player_a72d44) {
        bool booted = false;
        for (int i = 0; i < 8; i++) {
            NodeNameType *other = unknown_player_slots_a3fa98[i];
            if (i != slot && other != 0 && (other == unknown_easy_ai_entry_a72d34 || other == unknown_medium_ai_entry_a72d38
                    || other == unknown_hard_ai_entry_a72d3c)) {
                int other_id = Slot_Control_Id_005c97a0(i);
                unknown_combo_select_by_data_005c9950(dialog, other_id, Native_Entry_Handle(unknown_closed_entry_a72d30));   // NATIVE: handle
                Unknown_005cbf20(dialog, other_id, unknown_closed_entry_a72d30, 0);
                booted = true;
            }
        }
        if (booted) {
            Unknown_005C2C80(-1, StringTable::Load_String("TXT_OBSERVER_BOOTED_AI", 0, "D:\\Ra2\\netshare.cpp", 0x15BF));
        }
    }
    if (entry == previous) {
        return;
    }
    unknown_player_slots_a3fa98[slot] = entry;
    if (update == 1 || update == 2) {
        if ((entry == 0 || entry == unknown_open_entry_a72d2c || entry == unknown_closed_entry_a72d30
                || entry == unknown_waiting_entry_a72d40 || entry == unknown_player_a72d44
                || entry == unknown_easy_ai_entry_a72d34 || entry == unknown_medium_ai_entry_a72d38
                || entry == unknown_hard_ai_entry_a72d3c)
            && previous != 0 && previous != unknown_open_entry_a72d2c && previous != unknown_closed_entry_a72d30
            && previous != unknown_waiting_entry_a72d40 && previous != unknown_player_a72d44
            && previous != unknown_easy_ai_entry_a72d34 && previous != unknown_medium_ai_entry_a72d38
            && previous != unknown_hard_ai_entry_a72d3c) {
            if (unknown_chat_a72cfc != 0) {
                Kick_Channel_User_Wide_0075cae0(previous->Name);
            } else {
                Reject_Join_005bc3e0(previous);
            }
        }
        if (unknown_chat_a72cfc != 0) {
            Unknown_0073c480();
        } else {
            Send_Slot_Names_005bbd20();
        }
    }
    bool computer = entry != 0 && (entry == unknown_easy_ai_entry_a72d34 || entry == unknown_medium_ai_entry_a72d38
        || entry == unknown_hard_ai_entry_a72d3c);
    bool was_computer = previous != 0 && (previous == unknown_easy_ai_entry_a72d34
        || previous == unknown_medium_ai_entry_a72d38 || previous == unknown_hard_ai_entry_a72d3c);
    bool observer = entry == unknown_player_a72d44 || (entry != 0 && entry != unknown_open_entry_a72d2c
        && entry != unknown_closed_entry_a72d30 && entry != unknown_waiting_entry_a72d40
        && entry != unknown_easy_ai_entry_a72d34 && entry != unknown_medium_ai_entry_a72d38
        && entry != unknown_hard_ai_entry_a72d3c && entry->Team == -1);
    bool was_observer = previous == unknown_player_a72d44 || (previous != 0 && previous != unknown_open_entry_a72d2c
        && previous != unknown_closed_entry_a72d30 && previous != unknown_waiting_entry_a72d40
        && previous != unknown_easy_ai_entry_a72d34 && previous != unknown_medium_ai_entry_a72d38
        && previous != unknown_hard_ai_entry_a72d3c && previous->Team == -1);
    if (observer) {
        Fill_Observer_Combo_4d1640(dialog, Player_Side_Combo_ID_4d1350(slot));
        SendMessageA(GetDlgItem(dialog, Slot_Control_Id_005c97a0(slot)), 0x4F1, 0, 1);
        int color = Player_Color_Combo_ID_4d1cb0(slot);
        Fill_Random_Color_Combo_4d2250(dialog, color);
        Select_Color_Combo_4d2480(dialog, color, 8);
        Unknown_005c9b90(dialog, slot, false);
        unknown_lobby_a3d2dc[slot] = -1;
        unknown_lobby_a3d2fc[slot] = -1;
        unknown_lobby_a3d31c[slot] = -1;
    } else if (was_observer) {
        Fill_Side_Combo_4d1560(dialog, Player_Side_Combo_ID_4d1350(slot));
        SendMessageA(GetDlgItem(dialog, Slot_Control_Id_005c97a0(slot)), 0x4F1, 0, 0);
        int color = Player_Color_Combo_ID_4d1cb0(slot);
        Fill_Random_Color_Combo_4d2250(dialog, color);
        Select_Color_Combo_4d2480(dialog, color, -2);
        Unknown_005c9b90(dialog, slot, false);
        unknown_lobby_a3d2dc[slot] = -1;
        unknown_lobby_a3d2fc[slot] = -1;
        unknown_lobby_a3d31c[slot] = -1;
    }
    if (computer) {
        Unknown_005c9b90(dialog, slot, true);
        int difficulty = -1;
        if (entry == unknown_easy_ai_entry_a72d34) {
            difficulty = 2;
        } else if (entry == unknown_medium_ai_entry_a72d38) {
            difficulty = 1;
        } else if (entry == unknown_hard_ai_entry_a72d3c) {
            difficulty = 0;
        }
        unknown_lobby_a3d2dc[slot] = difficulty;
        unknown_lobby_a3d2fc[slot] = -2;
        unknown_lobby_a3d31c[slot] = -2;
    } else if (was_computer) {   // NATIVE (Scout1274): else-if as in the original (0x005CC608 jmp 0x005CC632); computer to computer keeps the new difficulty
        Unknown_005c9b90(dialog, slot, false);
        unknown_lobby_a3d2dc[slot] = -1;
        unknown_lobby_a3d2fc[slot] = -1;
        unknown_lobby_a3d31c[slot] = -1;
    }
    if (unknown_chat_a72cfc != 0) {
        Update_Channel_Player_Limit_00749f20();
    }
    bool computers_changed = computer || was_computer;
    bool observers_changed = observer || was_observer;
    if (computers_changed || observers_changed) {
        int count = 0;
        for (NodeNameType **pointer = unknown_player_slots_a3fa98; pointer < &unknown_player_slots_a3fa98[8]; pointer++) {
            NodeNameType *other = *pointer;
            if (other != 0 && (other == unknown_easy_ai_entry_a72d34 || other == unknown_medium_ai_entry_a72d38
                    || other == unknown_hard_ai_entry_a72d3c)) {
                count++;
            }
        }
        unknown_session_a3d298.AICount = count;
        if (update == 2) {
            if (unknown_chat_a72cfc != 0) {
                unknown_call_73fee0_default();
            } else {
                Unknown_005bbdc0();
            }
            Unknown_005c3810(true, false);
        }
    }
}
#endif

// void __fastcall Select_Entry_Kind_005cc700(HWND dialog, int id, int kind) (0x005CC700)
// Selects the fixed lobby entry of a kind (2 open .. 7 observer, as Entry_Kind_005c9650 numbers them) in a
// slot combo box and refreshes the slot with it.

void __fastcall Select_Entry_Kind_005cc700(HWND dialog, int id, int kind)
{
    NodeNameType *entry;
    if (kind == 2) {
        entry = unknown_open_entry_a72d2c;
    } else if (kind == 3) {
        entry = unknown_closed_entry_a72d30;
    } else if (kind == 4) {
        entry = unknown_easy_ai_entry_a72d34;
    } else if (kind == 5) {
        entry = unknown_medium_ai_entry_a72d38;
    } else if (kind == 6) {
        entry = unknown_hard_ai_entry_a72d3c;
    } else {
        entry = kind == 7 ? unknown_player_a72d44 : 0;
    }
    unknown_combo_select_by_data_005c9950(dialog, id, (int)entry);
    Unknown_005cbf20(dialog, id, entry, 0);
}


// void __fastcall Load_Random_Map_Preview_005cc800(HWND dialog) (0x005CC800)

void __fastcall Load_Random_Map_Preview_005cc800(HWND dialog)
{
    if (unknown_multi_missions_a3d8d8[unknown_session_a3d2b4]->Unknown_006720C0()) {
        delete unknown_dialog_preview_a72ce8;
        unknown_dialog_preview_a72ce8 = new MapPreviewClass;
        unknown_dialog_preview_a72ce8->Read_PCX_Preview("RandMap.img");
        if (unknown_dialog_preview_a72ce8->unknown_00 == 0) {
            Unknown_005c7570(dialog);
        }
        InvalidateRect(dialog, 0, 0);
    } else {
        Unknown_005c7570(dialog);
    }
}


// void __fastcall Save_Lobby_Options_005cc890(HWND dialog, SessionOptionsClass *options) (0x005CC890)
// Copies the session's game settings and each slot's entry kind, side and colour (read from the lobby
// dialog's combo boxes) into a per-game-type settings block.

void __fastcall Save_Lobby_Options_005cc890(HWND dialog, SessionOptionsClass *options)
{
    options->GameMode = unknown_session_a3d298.GameModeIndex;
    options->ScenIndex = unknown_session_a3d298.ScenarioIndex;
    for (int slot = 1; slot < 8; slot++) {
        int id = Slot_Control_Id_005c97a0(slot);
        int index = SendDlgItemMessageA(dialog, id, 0x147, 0, 0);
        int data = SendDlgItemMessageA(dialog, id, 0x150, index, 0);
        options->Slots[slot][0] = Entry_Kind_005c9650((NodeNameType *)(data == -1 ? 0 : data));
        int side_id = Player_Side_Combo_ID_4d1350(slot);
        options->Slots[slot][1] = Side_Combo_Data_4d1c70(dialog, side_id, -1);
        int color_id = Player_Color_Combo_ID_4d1cb0(slot);
        options->Slots[slot][2] = Color_Combo_Data_4d2900(dialog, color_id, -1);
    }
    options->GameSpeed = unknown_session_a3d298.GameSpeed_30;
    options->Credits = unknown_session_a3d298.unknown_24;
    options->UnitCount = unknown_session_a3d298.UnitCount;
    options->ShortGame = unknown_session_a3d298.unknown_2a;
    options->SuperWeaponsAllowed = unknown_session_a3d298.unknown_2b;
    options->BuildOffAlly = unknown_session_a3d298.unknown_2c;
    options->MCVRepacks = unknown_session_a3d298.unknown_a8;
    options->CratesAppear = unknown_session_a3d298.unknown_crates_29;
}


// void __fastcall Load_Lobby_Options_005cca30(HWND dialog, SessionOptionsClass *options) (0x005CCA30)
// Restores the lobby from a per-game-type settings block (the inverse of Save_Lobby_Options_005cc890):
// the game mode (the first registered mode when the saved one is unknown, or refuses a network session
// test through slot 1), the map (the mode's first playable map when it cannot play the saved one), each
// slot's entry kind (slots past the limit close; observers and computers fall back to open where they
// are not allowed), side and colour, then the session's game settings.

void __fastcall Load_Lobby_Options_005cca30(HWND dialog, SessionOptionsClass *options)
{
    if (options->GameMode < 1 || options->GameMode > MultiplayerGameMode::unknown_mode_count_5b6f80()) {
        options->GameMode = MultiplayerGameMode::unknown_first_mode_5b6ec0()->Get_ID();
    }
    unknown_session_a3d298.GameModeIndex = options->GameMode;
    MultiplayerGameMode *mode = MultiplayerGameMode::unknown_find_mode_5b6fe0(unknown_session_a3d298.GameModeIndex);
    if (mode == 0) {
        mode = MultiplayerGameMode::unknown_first_mode_5b6ec0();
    }
    if ((unknown_73c370() || unknown_73c3a0()) && mode != 0 && mode->unknown_slot1()) {
        options->GameMode = MultiplayerGameMode::unknown_first_mode_5b6ec0()->Get_ID();
        unknown_session_a3d298.GameModeIndex = options->GameMode;
        MultiplayerGameMode *found = MultiplayerGameMode::unknown_find_mode_5b6fe0(unknown_session_a3d298.GameModeIndex);
        if (found == 0) {
            found = MultiplayerGameMode::unknown_first_mode_5b6ec0();
        }
        mode = found;
    }
    options->GameMode = mode->Get_ID();
    unknown_session_a3d298.GameModeIndex = mode->Get_ID();
    if (options->ScenIndex < 0 || options->ScenIndex >= unknown_session_a3d298.Scenarios.Count()) {
        options->ScenIndex = 0;
    }
    unknown_session_a3d298.ScenarioIndex = Select_Scenario_005c7c30(options->ScenIndex) ? options->ScenIndex : 0;
    MultiMission *scen = unknown_session_a3d298.Scenarios[unknown_session_a3d298.ScenarioIndex];
    if (!mode->unknown_5b7470(scen)) {
        int index = mode->FirstValidMapIndex();
        if (index == -1) {
            mode = MultiplayerGameMode::unknown_first_mode_5b6ec0();
            options->GameMode = mode->Get_ID();
            unknown_session_a3d298.GameModeIndex = mode->Get_ID();
            index = mode->FirstValidMapIndex();
        }
        options->ScenIndex = index;
        unknown_session_a3d298.ScenarioIndex = index;
        Select_Scenario_005c7c30(index);
    }
    unknown_session_a3d298.Mode = mode;
    Select_Scenario_005c7c30(unknown_session_a3d298.ScenarioIndex);
    unknown_session_a3d298.Mode->unknown_slot8();
    unknown_session_a3d298.Mode->unknown_slot9();
    mode->AssignTeamsAtStart();
    mode->UnfixAlliances();
    int limit = 8;
    if (unknown_game_mode_a3d298 == 4) {
        limit = (unknown_73c370() && !unknown_73c3a0()) ? 2 : 4;
    }
    if (unknown_lobby_flag_7e6d90 == 0 && limit < 8) {
        limit++;
    }
    for (int slot = 1; slot < 8; slot++) {
        int id = Slot_Control_Id_005c97a0(slot);
        int kind = options->Slots[slot][0];
        if (unknown_73c370() || unknown_lobby_flag_7e6d90 == 0 || unknown_a72c68 != 0) {
            if (kind == 4) {
                kind = 2;
            } else if (kind == 5) {
                kind = 2;
            } else if (kind == 6) {
                kind = 2;
            }
        }
        if (slot >= limit) {
            kind = 3;
        } else if (kind == 7) {
            kind = 2;
        }
        Select_Entry_Kind_005cc700(dialog, id, kind);
        int side_id = Player_Side_Combo_ID_4d1350(slot);
        int color_id = Player_Color_Combo_ID_4d1cb0(slot);
        if (kind != 2 && kind != 3) {
            if (kind == 7) {
                Select_Side_Combo_4d1a90(dialog, side_id, -3);
                Select_Color_Combo_4d2480(dialog, color_id, -2);
                HWND side = GetDlgItem(dialog, side_id);
                HWND colors = GetDlgItem(dialog, color_id);
                EnableWindow(side, FALSE);
                EnableWindow(colors, FALSE);
            } else {
                Select_Side_Combo_4d1a90(dialog, side_id, options->Slots[slot][1]);
                Set_Slot_Country_005c9a10(dialog, side_id);
                Select_Color_Combo_4d2480(dialog, color_id, options->Slots[slot][2]);
                Set_Slot_Color_005c9ab0(dialog, color_id);
            }
        } else {
            Select_Side_Combo_4d1a90(dialog, side_id, unknown_session_a3d298.Mode->unknown_slot10());
            Select_Color_Combo_4d2480(dialog, color_id, -2);
            HWND side = GetDlgItem(dialog, side_id);
            HWND colors = GetDlgItem(dialog, color_id);
            EnableWindow(side, FALSE);
            EnableWindow(colors, FALSE);
        }
    }
    unknown_session_a3d298.GameSpeed_30 = options->GameSpeed;
    unknown_session_a3d298.unknown_24 = options->Credits;
    unknown_session_a3d298.UnitCount = options->UnitCount;
    unknown_session_a3d298.unknown_2a = options->ShortGame;
    unknown_session_a3d298.unknown_2b = options->SuperWeaponsAllowed;
    unknown_session_a3d298.unknown_2c = options->BuildOffAlly;
    unknown_session_a3d298.unknown_a8 = options->MCVRepacks;
    unknown_session_a3d298.unknown_crates_29 = options->CratesAppear;
}


// WideString __fastcall Limit_Name_Text_005ccf50(HWND control) (0x005CCF50)

WideString __fastcall Limit_Name_Text_005ccf50(HWND control)
{
    WideString text;
    unknown_get_window_wide_text(control, text);
    unsigned int limit = 0x78;
    if (!text.Is_Narrow()) {
        unsigned int i;
        for (i = 0; i < text.Length(); i++) {
            if (text.Get_Char(i) > 0x100) {
                break;
            }
        }
        limit = i - 1;
        if (limit <= 0x28) {
            limit = 0x28;
        }
    }
    if (text.Length() > limit) {
        text.Truncate(limit);
        SendMessageA(control, 0x4b2, 0, (LPARAM)text.unknown_text_770430());
    }
    {
        WideString line_breaks(L"\n\r");
        text.Trim_Trailing(line_breaks);
    }
    return text;
}


// void __fastcall Unknown_005cd030(HWND dialog, bool value) (0x005CD030)

void __fastcall Unknown_005cd030(HWND dialog, bool value)
{
    if (dialog != 0) {
        int slot = Find_First_Player_Slot_005c8c70();
        if (slot != unknown_lobby_flag_7e6d90) {
            HWND control = GetDlgItem(dialog, Player_Side_Combo_ID_4d1350(slot));
            if (control != 0) {
                EnableWindow(control, value);
            }
            control = GetDlgItem(dialog, Player_Color_Combo_ID_4d1cb0(slot));
            if (control != 0) {
                EnableWindow(control, value);
            }
        }
    }
}


// Wstring __fastcall Unknown_Lobby_Entry_Name_005cd0e0(NodeNameType *entry) (0x005CD0E0)
// The name of a fixed lobby entry (Open, Closed, EasyAI, MediumAI, HardAI, OpenObserver);
// "Waiting" for the waiting entry and for every other entry.

Wstring __fastcall Unknown_Lobby_Entry_Name_005cd0e0(NodeNameType *entry)
{
    if (entry == unknown_open_entry_a72d2c) {
        return Wstring(unknown_lobby_names_7e6de8[0]);
    }
    if (entry == unknown_closed_entry_a72d30) {
        return Wstring(unknown_lobby_names_7e6de8[1]);
    }
    if (entry == unknown_waiting_entry_a72d40) {
        return Wstring(unknown_lobby_names_7e6de8[2]);
    }
    if (entry == unknown_easy_ai_entry_a72d34) {
        return Wstring(unknown_lobby_names_7e6de8[3]);
    }
    if (entry == unknown_medium_ai_entry_a72d38) {
        return Wstring(unknown_lobby_names_7e6de8[4]);
    }
    if (entry == unknown_hard_ai_entry_a72d3c) {
        return Wstring(unknown_lobby_names_7e6de8[5]);
    }
    if (entry == unknown_player_a72d44) {
        return Wstring(unknown_lobby_names_7e6de8[6]);
    }
    return Wstring(unknown_lobby_names_7e6de8[2]);
}


// NodeNameType *__fastcall Unknown_Find_Lobby_Entry_005cd1b0(WideString const &name) (0x005CD1B0)
// The fixed lobby entry whose name equals the narrowed name; null when name is not narrow or matches none.

NodeNameType *__fastcall Unknown_Find_Lobby_Entry_005cd1b0(WideString const &name)
{
    if (name.Is_Narrow()) {
        Wstring narrow = Narrow_Copy_00771280(name);
        if (narrow == unknown_lobby_names_7e6de8[0]) {
            return unknown_open_entry_a72d2c;
        }
        if (narrow == unknown_lobby_names_7e6de8[1]) {
            return unknown_closed_entry_a72d30;
        }
        if (narrow == unknown_lobby_names_7e6de8[2]) {
            return unknown_waiting_entry_a72d40;
        }
        if (narrow == unknown_lobby_names_7e6de8[3]) {
            return unknown_easy_ai_entry_a72d34;
        }
        if (narrow == unknown_lobby_names_7e6de8[4]) {
            return unknown_medium_ai_entry_a72d38;
        }
        if (narrow == unknown_lobby_names_7e6de8[5]) {
            return unknown_hard_ai_entry_a72d3c;
        }
        if (narrow == unknown_lobby_names_7e6de8[6]) {
            return unknown_player_a72d44;
        }
    }
    return 0;
}


// bool __fastcall Unknown_Is_Lobby_Name_005cd2f0(Wstring const &name) (0x005CD2F0)
// True when name equals one of the seven fixed lobby entry names (Open, Closed,
// Waiting, EasyAI, MediumAI, HardAI, OpenObserver).

bool __fastcall Unknown_Is_Lobby_Name_005cd2f0(Wstring const &name)
{
    return name == unknown_lobby_names_7e6de8[0] || name == unknown_lobby_names_7e6de8[1] ||
           name == unknown_lobby_names_7e6de8[2] || name == unknown_lobby_names_7e6de8[3] ||
           name == unknown_lobby_names_7e6de8[4] || name == unknown_lobby_names_7e6de8[5] ||
           name == unknown_lobby_names_7e6de8[6];
}


extern "C" unsigned char data_5cd380_7e740c[];
extern "C" unsigned char data_5cd380_7dbce4[];
extern "C" unsigned char data_5cd380_839740[];
class Call_5cd380_0 {public: unsigned int target(unsigned int p0, unsigned int p1, unsigned int p2);};
extern "C" unsigned int global_5cd380_a72c68;
class Leaf_5cd380 { public: unsigned int get(); };
unsigned int Leaf_5cd380::get() { unsigned int r0 = ((Call_5cd380_0*)((unsigned int)data_5cd380_839740))->target((unsigned int)((unsigned int)data_5cd380_7dbce4), (unsigned int)((unsigned int)data_5cd380_7e740c), (unsigned int)(0)); global_5cd380_a72c68 = r0; return r0; }


// void __fastcall Unknown_005CD3A0() (0x005CD3A0)
// Writes the observer mode switch to [MultiPlayer] of RA2.INI and saves the INI file.

void __fastcall Unknown_005CD3A0()
{
    RawFileClass file("RA2.INI");
    unknown_ini_839740.Put_Int("MultiPlayer", "ObserverMode", unknown_a72c68);
    unknown_ini_839740.Save(file, false);
}


// void __fastcall Unknown_005cd3f0(HWND dialog) (0x005CD3F0)

void __fastcall Unknown_005cd3f0(HWND dialog)
{
    if (dialog != 0) {
        HWND control = GetDlgItem(dialog, 0x760);
        if (control != 0) {
            SendMessageA(control, 0xF1, unknown_a72c68 != 0, 0);
        }
        Surface *image;
        if (unknown_a72c68) {
            image = unknown_bitmap_writer_a76378.Get_Image_0068ca40("wolob.pcx", 0);
        } else {
            image = unknown_bitmap_writer_a76378.Get_Image_0068ca40("wolnob.pcx", 0);
        }
        HWND picture = GetDlgItem(dialog, 0x75F);
        OwnerDraw::unknown_5e1f30(picture, image);
        control = GetDlgItem(dialog, 0x6CD);
        if (control != 0) {
            wchar_t const *text;
            if (unknown_a72c68) {
                text = StringTable::Load_String("GUI:Observe", 0, "D:\\Ra2\\netshare.cpp", 0x17B8);
            } else {
                text = StringTable::Load_String("GUI:Join", 0, "D:\\Ra2\\netshare.cpp", 0x17BA);
            }
            SendMessageA(control, 0x4B2, 0, (LPARAM)text);
            InvalidateRect(control, 0, 0);
        }
        control = GetDlgItem(dialog, 0x62D);
        if (control != 0) {
            wchar_t const *text;
            if (unknown_a72c68) {
                text = StringTable::Load_String("GUI:Observe", 0, "D:\\Ra2\\netshare.cpp", 0x17C3);
            } else if (WS_Find_Dialog_00735940(0x113) == dialog) {
                text = StringTable::Load_String("GUI:JoinGame", 0, "D:\\Ra2\\netshare.cpp", 0x17C7);
            } else {
                text = StringTable::Load_String("GUI:Join", 0, "D:\\Ra2\\netshare.cpp", 0x17C9);
            }
            SendMessageA(control, 0x4B2, 0, (LPARAM)text);
            InvalidateRect(control, 0, 0);
        }
    }
}


// void __fastcall Unknown_005cd540(HWND dialog) (0x005CD540)

void __fastcall Unknown_005cd540(HWND dialog)
{
    if (dialog != 0) {
        Unknown_005cd3f0(dialog);
    }
    HWND other = WS_Find_Dialog_00735940(0x116);
    if (other != 0) {
        Unknown_005cd3f0(other);
    }
    other = WS_Find_Dialog_00735940(0x113);
    if (other != 0) {
        Unknown_005cd3f0(other);
    }
    other = WS_Find_Dialog_00735940(0xbb);
    if (other != 0) {
        Unknown_005cd3f0(other);
    }
}


// Original region: 0x005cd770 (93 body bytes, 3 independently frozen NOP bytes).
// Exact target SHA256: e6418111c2c36081a1795fb000e0927beea1630170b334c5013d12102602e70f.
// Purpose: initialize an inferred four-byte-element storage wrapper.
// Class/API identity and the three unknown field purposes remain unresolved.

extern "C" void* __cdecl allocate_storage_005cd770(unsigned int byte_count);
extern "C" unsigned char base_dispatch_005cd770[];
extern "C" unsigned char derived_dispatch_005cd770[];

class WordStorage_005cd770 {
    unsigned char* dispatch_table;
    unsigned int* storage;
    unsigned int capacity;
    unsigned char unknown_0x0c;
    unsigned char owns_storage;
    // Bytes 0x0e..0x0f are alignment padding and are not written.
    unsigned int unknown_0x10;
    unsigned int unknown_0x14;

public:
    WordStorage_005cd770* initialize(unsigned int requested_capacity,
                                unsigned int* supplied_storage);
};

typedef char WordStorage_005cd770_size_check[(sizeof(WordStorage_005cd770) == 24) ? 1 : -1];

WordStorage_005cd770* WordStorage_005cd770::initialize(
    unsigned int requested_capacity,
    unsigned int* supplied_storage)
{
    storage = 0;
    capacity = requested_capacity;
    unknown_0x0c = 1;
    owns_storage = 0;
    dispatch_table = base_dispatch_005cd770;

    // A zero capacity ignores even a nonnull supplied pointer.
    if (requested_capacity != 0) {
        if (supplied_storage != 0) {
            storage = supplied_storage;
        } else {
            // Preserve 32-bit wrapping and ownership even on a null result.
            storage = (unsigned int*)allocate_storage_005cd770(requested_capacity << 2);
            owns_storage = 1;
        }
    }

    dispatch_table = derived_dispatch_005cd770;
    unknown_0x14 = 10;
    unknown_0x10 = 0;
    return this;
}


extern "C" unsigned char vtable_5cd900[]; extern "C" void __cdecl release_buffer_5cd900(void*); extern "C" void __cdecl release_object_5cd900(void*);
class Leaf_5cd900 { public: void* get(unsigned int flags); };
void* Leaf_5cd900::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_5cd900; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_5cd900((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_5cd900(this); return this; }


extern "C" unsigned char vtable_5cd950[]; extern "C" void __cdecl release_buffer_5cd950(void*); extern "C" void __cdecl release_object_5cd950(void*);
class Leaf_5cd950 { public: void* get(unsigned int flags); };
void* Leaf_5cd950::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_5cd950; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_5cd950((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_5cd950(this); return this; }


// WolDictionary<WideString, bool>::Shrink_Table (0x005CD9A0): the lobby name set's template member,
// emitted at the end of this unit for the name-set removal in Restore_List_Selection (0x005C6310).
// Layout and behaviour: shared/wol_dictionary.hpp.

template void WolDictionary<WideString, bool>::Shrink_Table();


// Lobby name dictionary expansion, 0x005CDA50.
// Explicit specialization owns this original out-of-line copy; semantics match the shared template.

template <> inline void WolDictionary<WideString, bool>::Grow_Table()
{
    if (ResizeLocked != true) {
        int old_count = BucketCount;
        Node **old_buckets = Buckets;
        BucketCount *= 2;
        ++Bits;
        Buckets = new Node *[BucketCount];
        memset(Buckets, 0, BucketCount * sizeof(Node *));
        for (int index = 0; index < old_count; ++index) {
            Node *node = old_buckets[index];
            while (node) {
                unsigned int hash = Hash(node->Key);
                unsigned int bucket = ((1u << Bits) - 1) & hash;
                Node *head = Buckets[bucket];
                Buckets[bucket] = node;
                Node *next = node->Next;
                node->Next = head;
                node = next;
            }
        }
        delete [] old_buckets;
    }
}


extern "C" unsigned char table_5cdaf0[]; extern "C" void* __cdecl storage_5cdaf0(unsigned int);
class Storage_5cdaf0 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_5cdaf0::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_5cdaf0; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_5cdaf0(requested << 2); owns=1; } return this; }
