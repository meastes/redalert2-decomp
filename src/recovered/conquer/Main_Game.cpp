// void __fastcall Main_Game(int argc, char *argv[]) (0x00481BF0, Conquer.CPP): the game-session loop.
// One-time initialisation (Init_Game; a failure shows a message box), then the exception-return
// capture, then Select_Game / Main_Loop until the player quits.
// Shape from ra2-decomp Main_Game 0x0048CCC0 (YR layout twin, src/conquer.cpp) and TibSun conquer.cpp.
// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
#include "shared/win32_api.hpp"
#include "shared/win32_window.hpp"
#include "shared/string_table.hpp"
#include "shared/debug_printf.hpp"
#include "shared/session_state.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/keyboard.hpp"
#include "shared/mouse.hpp"
#include "shared/tool_tip.hpp"
#include "shared/options.hpp"
#include "shared/id_source.hpp"
#include "shared/main_loop.hpp"
#include "shared/queue.hpp"
#include "shared/win_util.hpp"

struct MSGBOXPARAMSA {
    UINT cbSize;
    HWND hwndOwner;
    HINSTANCE hInstance;
    char const *lpszText;
    char const *lpszCaption;
    DWORD dwStyle;
    char const *lpszIcon;
    DWORD dwContextHelpId;
    void (__stdcall *lpfnMsgBoxCallback)(void *help);
    DWORD dwLanguageId;
};
WIN32_IMPORT int WINAPI MessageBoxIndirectA(MSGBOXPARAMSA const *params);

// The exception handler's resume point (Startup.CPP): the stack, frame and code address
// Main_Game records before its game loop.
extern unsigned long ExceptionReturnStack;      // 0x00865C14
extern unsigned long ExceptionReturnAddress;    // 0x00865C18
extern unsigned long ExceptionReturnFrame;      // 0x00865C1C

int __fastcall Init_Game(int argc, char *argv[]);       // 0x005116A0
bool __fastcall Select_Game(bool fade);                 // 0x005135F0
void __fastcall Register_Game_Start_Time();             // 0x0069A130 (internet games)
void __fastcall Modem_Signoff();                        // 0x005CEA90
// Saves the session's chat settings (0x00670730; ra2-decomp Session.Write_Chat_Settings).
void __fastcall Write_Chat_Settings_00670730(SessionClass *session);
void __fastcall Special_Dialog_00481850();
void __cdecl Write_Multiplayer_Statistics_00482fe0();
extern int unknown_abd530;                              // 0x00ABD530 (cleared with unknown_flag_abd500)
extern bool unknown_flag_007d88e0;
extern unsigned accumulated_fps_00a6eb20;               // 0x00A6EB20 frames counted over the game
extern unsigned fps_sample_count_00a6eb24;              // 0x00A6EB24 seconds counted
extern bool unknown_flag_7ce008;                        // 0x007CE008
extern PaletteClass unknown_palette_837510;             // 0x00837510
// Also declared in shared/gscreen.hpp, which cannot follow map_view.hpp in this unit.
extern CCToolTip *ToolTips;                             // 0x008399B0
extern bool TacticalActive;                        // 0x00A40380 (also in gscreen.hpp and load_dlg.hpp)

void __fastcall Main_Game(int argc, char *argv[])
{
    static bool fade = true;

    int ret = Init_Game(argc, argv);
    if (ret) {
        if (ret < 0) {
            char text[0x1400];
            char caption[0x800];
            MSGBOXPARAMSA params;
            WideCharToMultiByte(0, 0, StringTable::Load_String("TXT_INITGAME_FAILED", 0, "D:\\Ra2\\Conquer.CPP", 0x232), -1, text, sizeof(text), 0, 0);
            WideCharToMultiByte(0, 0, StringTable::Load_String("TXT_SHORT_TITLE", 0, "D:\\Ra2\\Conquer.CPP", 0x233), -1, caption, sizeof(caption), 0, 0);
            params.cbSize = sizeof(MSGBOXPARAMSA);
            params.hwndOwner = MainWindow;
            params.hInstance = ProgramInstance;
            params.lpszText = text;
            params.lpszCaption = caption;
            params.dwStyle = 0x50010;   // MB_OK | MB_ICONSTOP | MB_SETFOREGROUND | MB_TOPMOST
            params.lpszIcon = 0;
            params.dwContextHelpId = 0;
            params.lpfnMsgBoxCallback = 0;
            params.dwLanguageId = 0x400;    // MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)
            MessageBoxIndirectA(&params);
        }
        return;
    }

    // The author's exception-return capture, TibSun conquer.cpp:346 (ra2-decomp src/conquer.cpp).
    __asm {
        mov ExceptionReturnStack, esp
        mov ExceptionReturnFrame, ebp
        lea eax, ret_addr
        mov ExceptionReturnAddress, eax
    }

ret_addr:
    Debug_Printf("ExceptionReturnStack = %08X\n", ExceptionReturnStack);
    Debug_Printf("ExceptionReturnAddress = %08X\n", ExceptionReturnAddress);

    while (Select_Game(fade)) {
        ScenarioInit = 0;
        fade = true;

        Set_Palette(&GamePalette, 15, 0);
        WWKeyboard->Clear();
        if (unknown_session_a3d298.Play) {
            WWMouse->Hide_Mouse();
            TeamEvent = 0;
            unknown_team_number_a6ebc8 = 0;
        }

        if (unknown_session_a3d298.Type == 4) {
            Register_Game_Start_Time();
            GameStatisticsPacketSent = false;
            unknown_abd530 = 0;
            unknown_flag_abd500 = false;
        }

        if (ToolTips != 0) {
            ToolTips->Enable(Options.ToolTips);
        }

        IgnoreInput = Scen->IsInputLocked;
        SpecialDialog = 0;
        unknown_flag_007d88e0 = true;
        unknown_session_a3d298.Unknown_00672DE0();

        for (;;) {
            if (Main_Loop()) {
                if (Main_Loop_End_0053f840()) {
                    break;
                }
            }
            Special_Dialog_00481850();
            if (Main_Loop_End_0053f840()) {
                break;
            }
        }
        Reload_Art_Set();
        unknown_session_a3d298.Leave_Game();
        ScenarioActive = false;
        TacticalActive = false;

        Debug_Printf("Game loop finished. Average FPS = %d\n", fps_sample_count_00a6eb24 ? accumulated_fps_00a6eb20 / fps_sample_count_00a6eb24 : 0);

        Write_Multiplayer_Statistics_00482fe0();

        if (!unknown_flag_7ce008) {
            Set_Palette(&unknown_palette_837510, 30, 0);
        }
        Title_Screen_Restore(true);

        if (unknown_session_a3d298.Record || unknown_session_a3d298.Play) {
            unknown_session_a3d298.RecordFile.Close();
        }

        if (unknown_session_a3d298.Type == 2 || unknown_session_a3d298.Type == 1) {
            if (!unknown_session_a3d298.Play) {
                Modem_Signoff();
            }
        } else {
            if (unknown_session_a3d298.Type == 3) {
                if (!unknown_session_a3d298.Play) {
                    Shutdown_Network();
                }
            }
        }

        if (unknown_session_a3d298.Type == 3 || unknown_session_a3d298.Type == 4 || unknown_session_a3d298.Type == 2 || unknown_session_a3d298.Type == 1) {
            if (!unknown_session_a3d298.Play) {
                Write_Chat_Settings_00670730(&unknown_session_a3d298);
            }
        }

        if (unknown_session_a3d298.Play) {
            unknown_session_a3d298.Type = 0;
            unknown_session_a3d298.Play = 0;
            WWMouse->Show_Mouse();
        }
    }

    if (WWMouse != 0) {
        WWMouse->Release_Mouse();
    }
}
