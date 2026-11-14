// Behaviour-written bodies for native M22 (Scout1177). NOT EXACT: each follows
// the original's disassembly or its pass-6a decompile against the shared headers.
#include "shared/main_opt.hpp"
#include "shared/dialog_create.hpp"
#include "shared/dialog_default_proc.hpp"
#include "shared/sound_available.hpp"
#include "shared/win_util.hpp"
#include "shared/winsock_interface.hpp"
#include <stdio.h>

// The native Enhancements page (link/native_enhancements.cpp; C3).
void Native_Run_Enhancements(HWND options);
void Native_Enhancements_Button_Text(HWND options);

// The Options dialog's state (main_opt.hpp; BSS in the original, -1 for the
// combo selection at the start). Native storage: nothing else defines them.
VideoModeSize *unknown_modes_a6ed10;
bool unknown_options_ready_a6ed0c;
// 0x00A6EC58/0x00A6EC5C, the mode chosen, are not storage of their own: they are the
// ScreenWidth/ScreenHeight (+0x20/+0x24) of unknown_options_a6ec38, the copy of Options
// Main_Options_Dialog 0x00542460 makes before it opens the dialog (Scout1281; the native
// tree had defined them here, so they stayed 0, no mode matched and the resolution box
// showed blank). native/data/member_aliases.tsv binds the names to those members.

// 0x00542590 Main_Options_Proc (shell Options dialog 0xD5; written from the
// disassembly 0x00542590-0x00542D5C). Reached by the main menu's Options button.
int __stdcall Main_Options_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    int handled = unknown_dialog_proc_5ffb00(window, message, wparam, lparam);
    if (handled) {
        return handled;
    }
    int *result = (int *)ra2port_get_dialog_user(window);
    HWND control = (HWND)lparam;
    switch (message) {
    case 0x111: {                                   // WM_COMMAND
        int id = wparam & 0xFFFF;
        if (id == 0x686) {                          // OK
            if ((wparam >> 16) == 0) {
                Free_Memory_00781d3b(unknown_modes_a6ed10);
                unknown_modes_a6ed10 = 0;
                *(int *)ra2port_get_dialog_user(window) = 0x5CB;
            }
        } else if (id == 0x7F80) {                  // NATIVE (C3): ENHANCEMENTS (port/src/native_dialogs.cpp)
            if ((wparam >> 16) == 0) Native_Run_Enhancements(window);
        } else if (id == 0x5CD || id == 0x5CE) {    // the second page, the keyboard dialog
            Free_Memory_00781d3b(unknown_modes_a6ed10);
            unknown_modes_a6ed10 = 0;
            *result = id;
        } else if (id == 0x6ED && (wparam >> 16) == 1) {   // resolution combo: CBN_SELCHANGE
            Unknown_00737080(window, (HWND)unknown_window_b263d8);
            HWND combo = GetDlgItem(window, 0x6ED);
            if (combo == 0) {
                return 0;
            }
            int item = SendMessageA(combo, 0x147, 0, 0);            // CB_GETCURSEL
            int mode = SendMessageA(combo, 0x150, item, 0);         // CB_GETITEMDATA
            unknown_video_width_a6ec58 = unknown_modes_a6ed10[mode].Width;
            unknown_video_height_a6ec5c = unknown_modes_a6ed10[mode].Height;
            if (unknown_video_width_a6ec58 == Options.ScreenWidth && unknown_video_height_a6ec5c == Options.ScreenHeight) {
                return 0;
            }
            if (Try_Mode_Dialog()) {
                if (unknown_allow_mode_toggle_a40d0b) {
                    Options.ScreenWidth = unknown_video_width_a6ec58;
                    Options.ScreenHeight = unknown_video_height_a6ec5c;
                    return 0;
                }
                if (Test_Display_Mode(unknown_video_width_a6ec58, unknown_video_height_a6ec5c)) {
                    Options.ScreenWidth = unknown_video_width_a6ec58;
                    Options.ScreenHeight = unknown_video_height_a6ec5c;
                    Unknown_Reset_Art_006F33B0(unknown_video_width_a6ec58, unknown_video_height_a6ec5c);
                    unknown_5159b0(0, true);
                    return 0;
                }
            }
            SendMessageA(combo, 0x14E, unknown_mode_initial_7dfdb4, 0);   // CB_SETCURSEL back
        }
        return 0;
    }
    case 2:                                         // WM_DESTROY
        if (unknown_modes_a6ed10) {
            Free_Memory_00781d3b(unknown_modes_a6ed10);
            unknown_modes_a6ed10 = 0;
        }
        return 0;
    case 0x114: {                                   // WM_HSCROLL
        if ((short)wparam != 5) {                   // TB_THUMBTRACK
            return 0;
        }
        int position = (int)(wparam >> 16);
        char const *name;
        int label;
        if (control == GetDlgItem(window, 0x52B)) {
            name = unknown_detail_names_7dfd80[position ? 2 : 0];
            label = 0x673;
        } else if (control == GetDlgItem(window, 0x50F)) {
            name = unknown_difficulty_names_7dfd8c[position];
            label = 0x670;
        } else if (control == GetDlgItem(window, 0x52A)) {
            name = unknown_scroll_rate_names_7dfd98[position];
            label = 0x672;
        } else {
            if (!unknown_options_ready_a6ed0c) {
                return 0;
            }
            float volume;
            if (control == GetDlgItem(window, 0x52F)) {
                volume = (float)(SendMessageA(control, 0x400, 0, 0) * 0.1);
                Options.Set_Score_Volume(volume, true);
            } else if (control == GetDlgItem(window, 0x532)) {
                volume = (float)(SendMessageA(control, 0x400, 0, 0) * 0.1);
                Options.Set_Sound_Volume(volume, true);
            } else if (control == GetDlgItem(window, 0x536)) {
                volume = (float)(SendMessageA(control, 0x400, 0, 0) * 0.1);
                Options.Set_Voice_Volume(volume, true);
            }
            return 0;
        }
        HWND text = GetDlgItem(window, label);
        if (text) {
            SendMessageA(text, 0x4B2, 0, (LPARAM)StringTable::Load_String(name, 0, "D:\\Ra2\\MainOpt.cpp", 0x226));
        }
        return 0;
    }
    case 0x497: {                                   // the dialog's controls are set up
        Native_Enhancements_Button_Text(window);    // NATIVE (C3): the ENHANCEMENTS button
        unknown_options_ready_a6ed0c = false;
        HWND slider = GetDlgItem(window, 0x52B);
        if (slider) {
            SendMessageA(slider, 0x4AC, 0, 0);
            SendMessageA(slider, 0x406, 1, 0x10000);                          // TBM_SETRANGE 0..1
            SendMessageA(slider, 0x405, 1, Options.DetailLevel != 0);       // TBM_SETPOS
        }
        HWND combo = GetDlgItem(window, 0x6ED);
        unknown_modes_a6ed10 = Options.AllowHiResModes ? Get_Display_Mode_Sizes_004962a0(640, 480, 0x1000, 0x1000, 16)
                                                       : Get_Display_Mode_Sizes_004962a0(640, 480, 0x400, 0x300, 16);
        int selected = -1;
        int count = 0;
        for (int index = 0; unknown_modes_a6ed10 && unknown_modes_a6ed10[index].Width; ++index) {
            int width = unknown_modes_a6ed10[index].Width;
            int height = unknown_modes_a6ed10[index].Height;
            if (Options.AllowHiResModes || width == 640 || width == 800 || width == 1024) {
                if (width == unknown_video_width_a6ec58 && height == unknown_video_height_a6ec5c) {
                    selected = count;
                }
                char buffer[64];
                sprintf(buffer, "%d x %d x 16", width, height);
                int item = SendMessageA(combo, 0x4BC, 0, (LPARAM)buffer);
                SendMessageA(combo, 0x151, item, index);                     // CB_SETITEMDATA
                ++count;
            }
        }
        SendMessageA(combo, 0x14E, selected, 0);                              // CB_SETCURSEL
        unknown_mode_initial_7dfdb4 = selected;
        slider = GetDlgItem(window, 0x50F);
        if (slider) {
            SendMessageA(slider, 0x4AC, 0, 0);
            SendMessageA(slider, 0x406, 1, 0x20000);
            SendMessageA(slider, 0x405, 1, Options.Difficulty);
        }
        HWND box = GetDlgItem(window, 0x601);
        if (box) SendMessageA(box, 0xF1, Options.UnitActionLines != 0, 0);   // BM_SETCHECK
        box = GetDlgItem(window, 0x604);
        if (box) SendMessageA(box, 0xF1, Options.ShowHidden != 0, 0);
        box = GetDlgItem(window, 0x602);
        if (box) SendMessageA(box, 0xF1, Options.ToolTips != 0, 0);
        slider = GetDlgItem(window, 0x52A);
        if (slider) {
            SendMessageA(slider, 0x4AC, 0, 0);
            SendMessageA(slider, 0x406, 1, 0x60000);
            SendMessageA(slider, 0x405, 1, 6 - Options.ScrollRate);
        }
        int sound = unknown_check_406f30();
        static int const volume_ids[3] = {0x52F, 0x532, 0x536};
        float const *volumes[3] = {&Options.ScoreVolume, &Options.SoundVolume, &Options.VoiceVolume};
        for (int i = 0; i < 3; ++i) {
            slider = GetDlgItem(window, volume_ids[i]);
            if (slider) {
                SendMessageA(slider, 0x4AE, 0, 0);
                SendMessageA(slider, 0x406, 1, 0xA0000);
                SendMessageA(slider, 0x405, 1, (int)(*volumes[i] * 10.0 + 0.5));
                EnableWindow(slider, sound);
            }
        }
        unknown_options_ready_a6ed0c = true;
        return 0;
    }
    }
    return 0;
}

// 0x00781D3B: the CRT's free (the mode lists come from operator new[], which is
// malloc natively and in the VC6 CRT). Reached by the Options dialog's close.
#include <stdlib.h>
void Free_Memory_00781d3b(void *block)
{
    free(block);
}
