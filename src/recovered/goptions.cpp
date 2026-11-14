// goptions.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/goptions.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/house.hpp"         // ahead of map_view.hpp: after it, Houses.Count() and operator[] stay out of line in Game_Options_Proc
#include "shared/game_options_dialog.hpp"
#include "shared/ms_message_box.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/id_source.hpp"
#include "shared/map_view.hpp"
#include "shared/game_controls_dialog.hpp"
#include "shared/map_seed.hpp"
#include "shared/event.hpp"
#include "shared/options.hpp"
#include "shared/string_table.hpp"
#include "shared/debug_printf.hpp"
#include "shared/system_timer.hpp"


// Header-static dynamic initializers of the unit at 0x004DD5E0: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x004DD5E0-0x004DD7C1); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x004DD600, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_004dd5e0.cpp; no source of its own.


// Dynamic initializer (0x004DD630, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_004dd5e0.cpp; no source of its own.


// Dynamic initializer (0x004DD650, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_004dd5e0.cpp; no source of its own.


// Dynamic initializer (0x004DD670, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_004dd5e0.cpp; no source of its own.


// Dynamic initializer (0x004DD690, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_004dd5e0.cpp; no source of its own.


// Dynamic initializer (0x004DD6B0, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_004dd5e0.cpp; no source of its own.


// Dynamic initializer (0x004DD6D0, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_004dd5e0.cpp; no source of its own.


// Dynamic initializer (0x004DD710, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_004dd5e0.cpp; no source of its own.


// Dynamic initializer (0x004DD740, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_004dd5e0.cpp; no source of its own.


// Dynamic initializer (0x004DD770, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_004dd5e0.cpp; no source of its own.


// Dynamic initializer (0x004DD7A0, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_004dd5e0.cpp; no source of its own.


// Dynamic initializer (0x004DD7B0, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_004dd5e0.cpp; no source of its own.


// void Unknown_004DD7D0() (0x004DD7D0): the in-game options menu (Red Alert's GameOptionsClass::Process).

void __fastcall Unknown_0063B090(ScenarioClass *scenario);    // 0x0063B090

void Unknown_004DD7D0()
{
    int result = 0;
    HWND window;
    if (unknown_game_mode_a3d298 == 0 || unknown_game_mode_a3d298 == 5) {
        window = unknown_create_dialog_5ff610(0xB5, Options_Menu_Proc, 0);
    } else if (unknown_game_mode_a3d298 == 4) {
        if (unknown_int_a3d548) {
            window = unknown_create_dialog_5ff610(0xBBA, Options_Menu_Proc, 0);
        } else {
            window = unknown_create_dialog_5ff610(0xFF, Options_Menu_Proc, 0);
        }
    } else {
        window = unknown_create_dialog_5ff610(0xBBA, Options_Menu_Proc, 0);
    }
    if (window) {
        SetWindowLongA(window, 8, (LONG)&result);
        unknown_dialog_5ff7c0(window);
        while (!result) {
            if (unknown_6000d0() == true) {
                result = 1;
            }
        }
        unknown_dialog_5ff6e0(window);
    }
    if (result == 0x3EC) {
        Unknown_0063B090(unknown_id_source_a3d290);
    }
    Map.Flag_To_Redraw(2);
}


// int __stdcall Options_Menu_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) (0x004DD8A0)
// The in-game options menu's dialog procedure (GOptions.CPP; TibSun Game_Options_Dialog_Proc): the default
// owner-draw handling first, 0x497 initialises the menu, WM_COMMAND runs Options_Menu_Command (inlined; it
// owns the two anti-cheat timers whose atexit thunks are 0x004DDDF0 speed and 0x004DDDE0 latency) and
// WM_HSCROLL updates the speed or latency label of the slider being dragged.

extern char const *Latency_Labels[];            // 0x007D9BB4
extern "C" int __cdecl swprintf(wchar_t *buffer, wchar_t const *format, ...);

static void __fastcall Options_Menu_Command(HWND window, int id, HWND control, unsigned code)
{
    int *result = (int *)GetWindowLongA(window, 8);
    static CDTimerClass<SystemTimerClass> speed_timer;
    static CDTimerClass<SystemTimerClass> latency_timer;
    switch (id) {
        case 0x51E:
            if (code == 0) {
                ShowWindow(window, SW_HIDE);
                if (LoadOptionsClass().Load_Dialog_0053b190()) {
                    *result = 0x51E;
                } else {
                    ShowWindow(window, SW_SHOW);
                    UpdateWindow(window);
                }
            }
            break;
        case 0x3EC:
            if (code == 0) {
                *result = 0x3EC;
            }
            break;
        case 0x51F:
            if (code == 0) {
                if (unknown_game_mode_a3d298 == 0 || unknown_game_mode_a3d298 == 5) {
                    ShowWindow(window, SW_HIDE);
                    wchar_t description[512];
                    wchar_t const *name = (wchar_t const *)unknown_id_source_a3d290->Name;
                    swprintf(description, L"%s", name);
                    LoadOptionsClass().Save_Dialog_0053b1b0(description);
                    Options_Menu_Init(window);
                    ShowWindow(window, SW_SHOW);
                    UpdateWindow(window);
                } else {
                    OutList.Add(EventClass(unknown_player_a35db4->ArrayIndex, 0x1D));
                    *result = 0x51F;
                }
            }
            break;
        case 0x520:
            if (code == 0) {
                ShowWindow(window, SW_HIDE);
                LoadOptionsClass().Delete_Dialog_0053b1e0();
                Options_Menu_Init(window);
                ShowWindow(window, SW_SHOW);
                UpdateWindow(window);
            }
            break;
        case 0x521:
            if (code == 0) {
                unknown_int_a40d48 = 5;
                *result = 1;
            }
            break;
        case 0x522:
            if (code == 0) {
                if (unknown_game_mode_a3d298 == 4) {
                    unknown_int_a40d48 = 2;
                    if (unknown_b2a7ec == 0) {
                        unknown_int_a40d48 = 3;
                    }
                } else {
                    unknown_int_a40d48 = 3;
                }
                *result = 2;
            }
            break;
        case 0x675:
        case 0x686:
            if (code == 0) {
                if (unknown_game_mode_a3d298 == 4) {
                    if (unknown_int_a3d548 == 0) {
                        HWND slider = GetDlgItem(window, 0x3EF);
                        if (slider != 0) {
                            int latency = 3 - SendMessageA(slider, 0x400, 0, 0);
                            if (latency != unknown_a3fba4) {
                                if (latency_timer == 0) {
                                    latency_timer = 900;
                                    OutList.Add(EventClass(unknown_player_a35db4->ArrayIndex, 0x24, latency));
                                    Debug_Printf("LATENCYFUDGE event created - %d\n", latency);
                                } else {
                                    Debug_Printf("Ignoring latency change to avoid cheating\n");
                                }
                            }
                        }
                        slider = GetDlgItem(window, 0x529);
                        if (slider != 0) {
                            int speed = 6 - SendMessageA(slider, 0x400, 0, 0);
                            if (Options.GameSpeed != speed) {
                                if (speed_timer == 0) {
                                    speed_timer = 900;
                                    OutList.Add(EventClass(unknown_player_a35db4->ArrayIndex, 0xD, speed));
                                } else {
                                    Debug_Printf("Ignoring speed change to avoid cheating\n");
                                }
                            }
                        }
                    }
                }
                *result = 1;
            }
            break;
    }
}

int __stdcall Options_Menu_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    int handled = unknown_dialog_proc_5ffb00(window, message, wparam, lparam);
    if (handled != 0) {
        return handled;
    }
    switch (message) {
        case 0x497:
            Options_Menu_Init(window);
            return 0;
        case 0x111:
            Options_Menu_Command(window, wparam & 0xFFFF, (HWND)lparam, wparam >> 16);
            break;
        case 0x114:
            if ((unsigned short)wparam == 5) {
                unsigned position = wparam >> 16;
                if ((HWND)lparam == GetDlgItem(window, 0x529)) {
                    char const *label = Game_Speed_Labels[position];
                    HWND text = GetDlgItem(window, 0x671);
                    if (text) {
                        SendMessageA(text, 0x4B2, 0, (LPARAM)StringTable::Load_String(label, 0, "D:\\Ra2\\GOptions.CPP", 0xC4));
                    }
                    break;
                }
                if ((HWND)lparam == GetDlgItem(window, 0x3EF)) {
                    char const *label = Latency_Labels[position];
                    HWND text = GetDlgItem(window, 0x672);
                    if (text) {
                        SendMessageA(text, 0x4B2, 0, (LPARAM)StringTable::Load_String(label, 0, "D:\\Ra2\\GOptions.CPP", 0xC4));
                    }
                    break;
                }
            }
            break;
    }
    return 0;
}


// atexit destructor of Options_Menu_Command's latency_timer (0x004DDDE0, _$E) emitted by Options_Menu_Proc_004dd8a0.cpp; no source of its own.


// atexit destructor of Options_Menu_Command's speed_timer (0x004DDDF0, _$E) emitted by Options_Menu_Proc_004dd8a0.cpp; no source of its own.


// void __fastcall Options_Menu_Init(HWND window) (0x004DDE00): sets up the in-game options menu (dialog
// 0xB5/0xBBA/0xFF of Unknown_004DD7D0): in single-player games the load (0x51E) and delete (0x520)
// buttons are enabled while saved games exist; in game mode 4 (without unknown_int_a3d548) the latency
// (0x3EF, 0-2) and game speed (0x529, 0-6) sliders get their range and position.


void __fastcall Options_Menu_Init(HWND window)
{
    if (unknown_game_mode_a3d298 == 0 || unknown_game_mode_a3d298 == 5) {
        bool saved = LoadOptionsClass().Has_Saved_Games_0053c5c0();
        HWND button = GetDlgItem(window, 0x51E);
        if (button) {
            EnableWindow(button, saved);
        }
        button = GetDlgItem(window, 0x520);
        if (button) {
            EnableWindow(button, saved);
        }
    } else if (unknown_game_mode_a3d298 == 4 && !unknown_int_a3d548) {
        HWND slider = GetDlgItem(window, 0x3EF);
        if (slider) {
            int latency = 3 - unknown_a3fba4;
            SendMessageA(slider, 0x406, 1, 0x30000);
            SendMessageA(slider, 0x405, 1, latency);
        }
        slider = GetDlgItem(window, 0x529);
        if (slider) {
            SendMessageA(slider, 0x406, 1, 0x60000);
            SendMessageA(slider, 0x405, 1, 6 - Options.GameSpeed);
        }
    }
}


// int __stdcall Game_Options_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) (0x004DDF90)
// Dialog 0xB6 (the leave-game choice): the leave button (0x6C9) returns Leave_Result (2, or 3 when the
// player may stay to observe), the second button (0x712) restarts (5) in the campaign, observes (4) in
// game modes 3 and 4 while at least two human houses are still in the game, and is hidden otherwise.

void __fastcall unknown_5e2040(HWND window, int value);   // 0x005E2040 (an owner-draw button's value)

static int Leave_Result;    // 0x00A40E1C
static int Other_Result;    // 0x00A40E20

int __stdcall Game_Options_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    int handled = unknown_dialog_proc_5ffb00(window, message, wparam, lparam);
    if (handled != 0) {
        return handled;
    }
    switch (message) {
        case 0x497: {
            HWND leave = GetDlgItem(window, 0x6C9);
            SendMessageA(leave, 0x4B2, 0, (LPARAM)StringTable::Load_String("GUI:Leave", 0, "D:\\Ra2\\GOptions.CPP", 0x21F));
            Leave_Result = 2;
            HWND other = GetDlgItem(window, 0x712);
            Other_Result = 0;
            if (unknown_game_mode_a3d298 == 0) {
                SendMessageA(other, 0x4B2, 0, (LPARAM)StringTable::Load_String("GUI:Restart", 0, "D:\\Ra2\\GOptions.CPP", 0x22A));
                Other_Result = 5;
            } else if (unknown_game_mode_a3d298 == 5) {
                ShowWindow(other, SW_HIDE);
            } else if (unknown_game_mode_a3d298 == 3 || unknown_game_mode_a3d298 == 4) {
                int humans = 0;
                for (int index = 0; index < Houses.Count(); index++) {
                    HouseClass *house = Houses[index];
                    if (house->unknown_134 && !house->unknown_13d) {
                        humans++;
                    }
                }
                if (humans >= 2 && !unknown_player_a35db4->unknown_13d) {
                    Leave_Result = 3;
                    SendMessageA(other, 0x4B2, 0, (LPARAM)StringTable::Load_String("GUI:Observe", 0, "D:\\Ra2\\GOptions.CPP", 0x246));
                    Other_Result = 4;
                } else {
                    ShowWindow(other, SW_HIDE);
                }
            }
            unknown_5e2040(leave, Leave_Result);
            unknown_5e2040(other, Other_Result);
            break;
        }
        case 0x111: {
            int *result = (int *)GetWindowLongA(window, 8);
            unsigned short code = wparam >> 16;
            int id = wparam & 0xFFFF;
            if (code == 0) {
                switch (id) {
                    case 1:
                    case 0x686:
                        *result = 1;
                        return 0;
                    case 0x6C9:
                        *result = Leave_Result;
                        return 0;
                    case 0x712:
                        *result = Other_Result;
                        break;
                }
            }
            break;
        }
    }
    return 0;
}


// QueueClass<EventClass,128>::Add physical copy,004DE180.
// Instantiate the existing shared queue algorithm; native RA2 adds the
// timeGetTime stamp absent from the older Red Alert/TibSun Queue.H.
template int QueueClass<EventClass, 128>::Add(EventClass const &event);


class Call_4de1e0_0 {public: unsigned int target();};
class Leaf_4de1e0 { public: unsigned int get(unsigned int a1); };
unsigned int Leaf_4de1e0::get(unsigned int a1) { unsigned int r0 = ((Call_4de1e0_0*)(((unsigned int)this) + 4))->target(); *(unsigned int*)(((unsigned int)this)) = r0; *(unsigned int*)(((unsigned int)this) + 8) = a1; return (unsigned int)this; }
