// sounddlg.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/sounddlg.json carries the addresses and the rows that
// stay in their own files.


// Header-static dynamic initializer of the Sound Controls unit (0x00688CB0, _$E): the shape_file.hpp
// empty rect; emitted by this include.
#include "shared/shape_file.hpp"


// void SoundControlsDialog::Run_00688CD0() (0x00688CD0)
// TS SoundControlsClass::Dialog: shows the sound controls (a cut down dialog outside a game) and services
// the owner draw dialog handler until the player dismisses them.
#include "shared/sound_controls.hpp"
#include "shared/dialog_create.hpp"
#include "shared/ms_message_box.hpp"
#include "shared/debug_printf.hpp"
#include "shared/options.hpp"
#include "shared/unknown_globals.hpp"

void SoundControlsDialog::Run_00688CD0()
{
    int rc = -1;
    Debug_Printf("SoundControls: GameSpeed = %d, ScrollRate = %d, Detail = %d\n", Options.GameSpeed,
                 Options.ScrollRate, Options.DetailLevel);
    DialogInitialized = false;

    HWND dialog;
    if (!unknown_flag_a40958) {
        dialog = unknown_create_dialog_5ff610(0xD6, Sound_Option_Dialog_Func, 0);
    } else {
        dialog = unknown_create_dialog_5ff610(0xB8, Sound_Option_Dialog_Func, 0);
    }

    if (dialog) {
        SetWindowLongA(dialog, 8, (LONG)&rc);
        unknown_dialog_5ff7c0(dialog);
        while (rc == -1) {
            if (unknown_6000d0() == true) {
                rc = 2;
            }
            if (!unknown_flag_a40958) {
                unknown_517370(false);
            }
        }
        unknown_dialog_5ff6e0(dialog);
    }

    Debug_Printf("SoundControls: GameSpeed = %d, ScrollRate = %d, Detail = %d\n", Options.GameSpeed,
                 Options.ScrollRate, Options.DetailLevel);
}


// int SoundControlsDialog::Sound_Option_Dialog_Func(HWND, UINT, WPARAM, LPARAM) (0x00688DA0)
// TS SoundControlsClass::Sound_Option_Dialog_Func: the common owner draw handler first; then the
// init message (0x497) sets up the three volume sliders and, in a game, the shuffle/repeat buttons
// and the wide track list; WM_COMMAND toggles shuffle/repeat, plays/stops a theme and on 0x686
// stores the slider volumes and closes; WM_HSCROLL previews a slider volume.
#define THEME_SOUND_CONTROLS
#ifndef VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#endif
#include "shared/vector_class.hpp"
#include "shared/sound_available.hpp"
#include "shared/string_table.hpp"
#include "shared/theme.hpp"
#include "shared/main_loop.hpp"

int __stdcall SoundControlsDialog::Sound_Option_Dialog_Func(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    int rc = unknown_dialog_proc_5ffb00(window, message, wparam, lparam);

    if (rc == 0) {
        switch (message) {
            case 0x497: {
                DialogInitialized = false;
                int enabled = unknown_check_406f30();

                HWND track = GetDlgItem(window, 0x52F);
                if (track) {
                    SendMessageA(track, 0x4AE, 0, 0);
                    SendMessageA(track, 0x406, 1, 0xA0000);
                    SendMessageA(track, 0x405, 1, (int)(Options.ScoreVolume * 10.0 + 0.5));
                    EnableWindow(track, enabled);
                }

                track = GetDlgItem(window, 0x532);
                if (track) {
                    SendMessageA(track, 0x4AE, 0, 0);
                    SendMessageA(track, 0x406, 1, 0xA0000);
                    SendMessageA(track, 0x405, 1, (int)(Options.SoundVolume * 10.0 + 0.5));
                    EnableWindow(track, enabled);
                }

                track = GetDlgItem(window, 0x536);
                if (track) {
                    SendMessageA(track, 0x4AE, 0, 0);
                    SendMessageA(track, 0x406, 1, 0xA0000);
                    SendMessageA(track, 0x405, 1, (int)(Options.VoiceVolume * 10.0 + 0.5));
                    EnableWindow(track, enabled);
                }

                if (unknown_flag_a40958) {
                    HWND button = GetDlgItem(window, 0x533);
                    if (button) {
                        SendMessageA(button, 0xF1, Options.IsScoreShuffle ? 1 : 0, 0);
                        EnableWindow(button, enabled);
                    }

                    button = GetDlgItem(window, 0x534);
                    if (button) {
                        SendMessageA(button, 0xF1, Options.IsScoreRepeat ? 1 : 0, 0);
                        EnableWindow(button, enabled);
                    }

                    HWND list = GetDlgItem(window, 0x530);
                    if (list) {
                        int active_theme = 0;
                        int visible_num = 1;

                        SendMessageA(list, 0x184, 0, 0);

                        for (ThemeType index = 0; index < Theme.Max_Themes(); index++) {
                            if (Theme.unknown_6e8440(index)) {
                                wchar_t buffer[100];
                                int length = Theme.unknown_theme_length(index);
                                wchar_t const *fullname = Theme.Full_Name(index);

                                swprintf(buffer, L"%02d - %s [%d:%02d]", visible_num, fullname, length / 60, length % 60);
                                visible_num++;

                                int row = SendMessageA(list, 0x4CD, 0, (LPARAM)buffer);
                                if (row != -1) {
                                    SendMessageA(list, 0x19A, row, index);
                                    if (Theme.What_Is_Playing() == index) {
                                        active_theme = row;
                                    }
                                }
                            }
                        }

                        SendMessageA(list, 0x186, active_theme, 0);
                        SendMessageA(list, 0x197, active_theme, 0);
                        EnableWindow(list, enabled);
                    }
                }

                DialogInitialized = true;
            }
                break;

            case 0x111:
                switch ((unsigned short)wparam) {
                    case 0x533:
                        Options.Set_Shuffle(SendMessageA((HWND)lparam, 0xF0, 0, 0) == 1);
                        if (SendMessageA((HWND)lparam, 0xF0, 0, 0) == 1) {
                            SendDlgItemMessageA(window, 0x534, 0xF1, 0, 0);
                            Options.Set_Repeat(false);
                        }
                        break;

                    case 0x534:
                        Options.Set_Repeat(SendMessageA((HWND)lparam, 0xF0, 0, 0) == 1);
                        if (SendMessageA((HWND)lparam, 0xF0, 0, 0) == 1) {
                            SendDlgItemMessageA(window, 0x533, 0xF1, 0, 0);
                            Options.Set_Shuffle(false);
                        }
                        break;

                    case 0x535:
                        if ((unsigned short)(wparam >> 16) == 0) {
                            Theme.Stop(true);
                            Theme.Queue_Song(-3);
                            unknown_flag_a40b51 = false;
                        }
                        break;

                    case 0x686:
                        if ((unsigned short)(wparam >> 16) == 0) {
                            HWND button = GetDlgItem(window, 0x52F);
                            if (button) {
                                Options.Set_Score_Volume(SendMessageA(button, 0x400, 0, 0) * 0.1, false);
                                if (Options.ScoreVolume == 0.0f) {
                                    Theme.Queue_Song(Theme.What_Is_Playing());
                                    Theme.Stop(false);
                                }
                            }
                            button = GetDlgItem(window, 0x532);
                            if (button) {
                                Options.Set_Sound_Volume(SendMessageA(button, 0x400, 0, 0) * 0.1, false);
                            }
                            button = GetDlgItem(window, 0x536);
                            if (button) {
                                Options.Set_Voice_Volume(SendMessageA(button, 0x400, 0, 0) * 0.1, false);
                            }
                            int *res = (int *)GetWindowLongA(window, 8);
                            *res = 1;
                        }
                        break;

                    case 0x531:
                        if ((unsigned short)(wparam >> 16) == 0) {
                            HWND list = GetDlgItem(window, 0x530);
                            if (list) {
                                int row = SendMessageA(list, 0x188, 0, 0);
                                if (row != -1) {
                                    ThemeType theme = SendMessageA(list, 0x199, row, 0);
                                    unknown_flag_a40b51 = true;
                                    if (Theme.What_Is_Playing() != theme) {
                                        Theme.Stop(true);
                                        Theme.Queue_Song(theme);
                                    }
                                }
                            }
                        }
                        break;
                }
                break;

            case 0x114:
                if (DialogInitialized) {
                    HWND track = (HWND)lparam;
                    if (track == GetDlgItem(window, 0x52F)) {
                        Options.Set_Score_Volume(SendMessageA(track, 0x400, 0, 0) * 0.1, true);
                    } else if (track == GetDlgItem(window, 0x532)) {
                        Options.Set_Sound_Volume(SendMessageA(track, 0x400, 0, 0) * 0.1, true);
                    } else if (track == GetDlgItem(window, 0x536)) {
                        Options.Set_Voice_Volume(SendMessageA(track, 0x400, 0, 0) * 0.1, true);
                    }
                }
                break;
        }
        return 0;
    }

    return rc;
}
