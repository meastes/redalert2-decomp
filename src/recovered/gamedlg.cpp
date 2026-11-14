// gamedlg.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/gamedlg.json carries the addresses and the rows that
// stay in their own files.


// Container members stay undefined here: this text does not yet reproduce the original
// unit's list uses (shared/vector_class.hpp).
#define VECTOR_DECLARATIONS_ONLY
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/game_controls_dialog.hpp"
#include "shared/options.hpp"
#include "shared/debug_printf.hpp"
#include "shared/ms_message_box.hpp"
#include "shared/event.hpp"
#include "shared/house.hpp"
#include "shared/techno.hpp"
#include "shared/display.hpp"
#include "shared/map_cell.hpp"
#include "shared/map_view.hpp"
#include "shared/tool_tip.hpp"
#include "shared/sound_available.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/string_table.hpp"


// Header-static dynamic initializers of the unit at 0x004CF730: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x004CF730-0x004CF911); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x004CF750, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_004cf730.cpp; no source of its own.


// Dynamic initializer (0x004CF780, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_004cf730.cpp; no source of its own.


// Dynamic initializer (0x004CF7A0, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_004cf730.cpp; no source of its own.


// Dynamic initializer (0x004CF7C0, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_004cf730.cpp; no source of its own.


// Dynamic initializer (0x004CF7E0, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_004cf730.cpp; no source of its own.


// Dynamic initializer (0x004CF800, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_004cf730.cpp; no source of its own.


// Dynamic initializer (0x004CF820, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_004cf730.cpp; no source of its own.


// Dynamic initializer (0x004CF860, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_004cf730.cpp; no source of its own.


// Dynamic initializer (0x004CF890, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_004cf730.cpp; no source of its own.


// Dynamic initializer (0x004CF8C0, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_004cf730.cpp; no source of its own.


// Dynamic initializer (0x004CF8F0, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_004cf730.cpp; no source of its own.


// Dynamic initializer (0x004CF900, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_004cf730.cpp; no source of its own.


// void GameControlsDialog::Run() (0x004CF920)
// Logs the speed, scroll rate and detail level, shows the Game Controls dialog
// (0xBBB in a game, 0xF5 otherwise), waits until a button sets the result held
// in DWL_USER (-1 while waiting; the quit check ends it with 2), on OK (1) applies
// the settings and saves the options, and logs the values again.

void GameControlsDialog::Run()
{
    int result = -1;
    Debug_Printf("GameControls: GameSpeed = %d, ScrollRate = %d, Detail = %d\n", Options.GameSpeed, Options.ScrollRate, Options.DetailLevel);
    if (unknown_flag_a40958 == 1) {
        Window = unknown_create_dialog_5ff610(0xBBB, Game_Controls_Proc, 0);
    } else {
        Window = unknown_create_dialog_5ff610(0xF5, Game_Controls_Proc, 0);
    }
    if (Window) {
        SetWindowLongA(Window, 8, (LONG)&result);
        unknown_dialog_5ff7c0(Window);
        while (result == -1) {
            if (unknown_6000d0() == true) {
                result = 2;
            }
            if (!unknown_flag_a40958) {
                unknown_517370(false);
            }
        }
        if (result == 1) {
            Apply_Settings();
            Options.Save_Settings();
        }
        unknown_dialog_5ff6e0(Window);
    }
    Debug_Printf("GameControls: GameSpeed = %d, ScrollRate = %d, Detail = %d\n", Options.GameSpeed, Options.ScrollRate, Options.DetailLevel);
}


// void GameControlsDialog::Apply_Settings() (0x004CFA00)
// Reads the dialog's sliders and check boxes into Options. A changed game speed
// is queued as event 0xD (kept for later when the queue is full) while playing
// a session game (otherwise it is applied directly); a changed detail level
// resets the map's cells; the tool tip box enables the tool tips. Outside a game
// (unknown_flag_a40958 clear) the difficulty slider 0x50F is read as well.

void GameControlsDialog::Apply_Settings()
{
    HWND control = GetDlgItem(Window, 0x529);
    if (control) {
        int speed = 6 - SendMessageA(control, 0x400, 0, 0);
        if (Options.GameSpeed != speed) {
            if (unknown_flag_a40958 == 1 && unknown_game_mode_a3d298 != 0 && unknown_game_mode_a3d298 != 5) {
                OutList.Add(EventClass(unknown_player_a35db4->ArrayIndex, 0xD, speed));
            } else {
                Options.GameSpeed = speed;
            }
        }
    }
    control = GetDlgItem(Window, 0x52A);
    if (control) {
        Options.ScrollRate = 6 - SendMessageA(control, 0x400, 0, 0);
    }
    control = GetDlgItem(Window, 0x52B);
    if (control) {
        int detail = SendMessageA(control, 0x400, 0, 0);
        if (Options.DetailLevel != detail) {
            Options.DetailLevel = detail;
            Map.Reinit_Cell_Drawers();
        }
    }
    control = GetDlgItem(Window, 0x601);
    if (control) {
        Options.UnitActionLines = SendMessageA(control, 0xF0, 0, 0) == 1;
        TechnoClass::unknown_6d79c0(Options.UnitActionLines);
    }
    control = GetDlgItem(Window, 0x604);
    if (control) {
        Options.ShowHidden = SendMessageA(control, 0xF0, 0, 0) == 1;
    }
    control = GetDlgItem(Window, 0x602);
    if (control) {
        Options.ToolTips = SendMessageA(control, 0xF0, 0, 0) == 1;
        if (ToolTips && unknown_flag_a40958 == 1) {
            ToolTips->Enable(Options.ToolTips);
        }
    }
    if (!unknown_flag_a40958) {
        control = GetDlgItem(Window, 0x50F);
        if (control) {
            Options.Difficulty = SendMessageA(control, 0x400, 0, 0);
        }
    }
}


// int __stdcall Game_Controls_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) (0x004CFC00)
// Dialog procedure of the Game Controls dialog (the WM_COMMAND handler Game_Controls_Command, 0x004CFFB0, is defined here and inlined here): the shared handler first, then
// 0x497 (initialisation: the speed, scroll rate, detail and difficulty sliders
// get their range 0-6 / 0-2 and position, the check boxes their state, the
// hotkey button is enabled when sound is available), WM_COMMAND
// (Game_Controls_Command, inlined) and WM_HSCROLL (the label next to
// the moved slider shows the text for its position).

// void __fastcall Game_Controls_Command(HWND window, int id, HWND control, unsigned code) (0x004CFFB0)
// WM_COMMAND of the dialog: stores the result through the DWL_USER pointer; 0x52C and 0x52D
// (notification 0, in a game) also set the radar mode 4 / 6; 0x686: 1. Proc inlines it; the
// out-of-line copy at 0x004CFFB0 has no callers and its final `ret 8` is the former region 0x004D0020.
void __fastcall Game_Controls_Command(HWND window, int id, HWND control, unsigned code)
{
    int *result = (int *)GetWindowLongA(window, 8);
    switch (id) {
        case 0x52C:
            if (code == 0 && unknown_flag_a40958 == 1) {
                unknown_int_a40d48 = 4;
                *result = 1;
            }
            break;
        case 0x52D:
            if (code == 0 && unknown_flag_a40958 == 1) {
                unknown_int_a40d48 = 6;
                *result = 1;
            }
            return;
        case 0x686:
            *result = 1;
            return;
    }
}

int __stdcall Game_Controls_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    int handled = unknown_dialog_proc_5ffb00(window, message, wparam, lparam);
    if (handled != 0) {
        return handled;
    }
    switch (message) {
        case 0x497: {
            HWND slider = GetDlgItem(window, 0x529);
            if (unknown_game_mode_a3d298 == 0 && !unknown_flag_a40d84) {
                ShowWindow(slider, SW_HIDE);
                HWND hidden = GetDlgItem(window, 0x714);
                ShowWindow(hidden, SW_HIDE);
                hidden = GetDlgItem(window, 0x671);
                ShowWindow(hidden, SW_HIDE);
            } else if (slider) {
                SendMessageA(slider, 0x4AC, 0, 0);
                SendMessageA(slider, 0x406, 1, 0x60000);
                SendMessageA(slider, 0x405, 1, 6 - Options.GameSpeed);
            }
            if (unknown_int_a3d548) {
                HWND hidden = GetDlgItem(window, 0x529);
                ShowWindow(hidden, SW_HIDE);
                hidden = GetDlgItem(window, 0x714);
                ShowWindow(hidden, SW_HIDE);
                hidden = GetDlgItem(window, 0x671);
                ShowWindow(hidden, SW_HIDE);
            }
            slider = GetDlgItem(window, 0x52A);
            if (slider) {
                SendMessageA(slider, 0x4AC, 0, 0);
                SendMessageA(slider, 0x406, 1, 0x60000);
                SendMessageA(slider, 0x405, 1, 6 - Options.ScrollRate);
            }
            slider = GetDlgItem(window, 0x52B);
            if (slider) {
                SendMessageA(slider, 0x4AC, 0, 0);
                SendMessageA(slider, 0x406, 1, 0x20000);
                SendMessageA(slider, 0x405, 1, Options.DetailLevel);
            }
            HWND box = GetDlgItem(window, 0x601);
            if (box) {
                SendMessageA(box, 0xF1, Options.UnitActionLines != 0, 0);
            }
            box = GetDlgItem(window, 0x604);
            if (box) {
                SendMessageA(box, 0xF1, Options.ShowHidden != 0, 0);
            }
            box = GetDlgItem(window, 0x602);
            if (box) {
                SendMessageA(box, 0xF1, Options.ToolTips != 0, 0);
            }
            if (unknown_flag_a40958 == 1) {
                HWND button = GetDlgItem(window, 0x52D);
                if (button) {
                    EnableWindow(button, unknown_check_406f30());
                }
                return 0;
            }
            slider = GetDlgItem(window, 0x50F);
            if (slider) {
                SendMessageA(slider, 0x4AC, 0, 0);
                SendMessageA(slider, 0x406, 1, 0x20000);
                SendMessageA(slider, 0x405, 1, Options.Difficulty);
            }
            return 0;
        }
        case 0x111:
            Game_Controls_Command(window, wparam & 0xFFFF, (HWND)lparam, wparam >> 16);
            break;
        case 0x114:
            if ((unsigned short)wparam == 5) {
                unsigned position = wparam >> 16;
                if ((HWND)lparam == GetDlgItem(window, 0x529)) {
                    char const *label = Game_Speed_Labels[position];
                    HWND text = GetDlgItem(window, 0x671);
                    if (text) {
                        SendMessageA(text, 0x4B2, 0, (LPARAM)StringTable::Load_String(label, 0, "D:\\Ra2\\GameDlg.CPP", 0x1A4));
                    }
                    break;
                }
                if ((HWND)lparam == GetDlgItem(window, 0x52A)) {
                    char const *label = Scroll_Rate_Labels[position];
                    HWND text = GetDlgItem(window, 0x672);
                    if (text) {
                        SendMessageA(text, 0x4B2, 0, (LPARAM)StringTable::Load_String(label, 0, "D:\\Ra2\\GameDlg.CPP", 0x1A4));
                    }
                    break;
                }
                if ((HWND)lparam == GetDlgItem(window, 0x52B)) {
                    char const *label = Detail_Level_Labels[position];
                    HWND text = GetDlgItem(window, 0x673);
                    if (text) {
                        SendMessageA(text, 0x4B2, 0, (LPARAM)StringTable::Load_String(label, 0, "D:\\Ra2\\GameDlg.CPP", 0x1A4));
                    }
                    break;
                }
                if (!unknown_flag_a40958 && (HWND)lparam == GetDlgItem(window, 0x50F)) {
                    char const *label = Difficulty_Labels[position];
                    HWND text = GetDlgItem(window, 0x670);
                    if (text) {
                        SendMessageA(text, 0x4B2, 0, (LPARAM)StringTable::Load_String(label, 0, "D:\\Ra2\\GameDlg.CPP", 0x1A4));
                    }
                    break;
                }
            }
            break;
    }
    return 0;
}


// void __fastcall Game_Controls_Command(HWND window, int id, HWND control, unsigned code) (0x004CFFB0)
// The out-of-line copy of the WM_COMMAND handler of the Game Controls dialog. It is defined in
// Game_Controls_Proc_004cfc00.cpp (one definition emits both this copy and the inlined one in Proc);
// this comment-only source holds its region. Its final `ret 8` was catalogued as region 0x004D0020.
