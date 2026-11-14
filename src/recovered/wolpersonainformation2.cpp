// wolpersonainformation2.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/wolpersonainformation2.json carries the addresses and the rows that
// stay in their own files.

// Header-static dynamic initializers of the unit at 0x0073A0F0: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x0073A0F0-0x0073A2D1); the other
// drafts of this plan are emitted by these includes.
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"




#include "shared/wol_main.hpp"
#include "shared/mouse.hpp"
#include "shared/theme.hpp"
#include "shared/wol_settings.hpp"
#include "shared/wol_registry_options.hpp"
#include "shared/wol_sink.hpp"


// Dynamic initializer (0x0073A110, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_0073a0f0.cpp; no source of its own.


// Dynamic initializer (0x0073A140, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_0073a0f0.cpp; no source of its own.


// Dynamic initializer (0x0073A160, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_0073a0f0.cpp; no source of its own.


// Dynamic initializer (0x0073A180, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_0073a0f0.cpp; no source of its own.


// Dynamic initializer (0x0073A1A0, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_0073a0f0.cpp; no source of its own.


// Dynamic initializer (0x0073A1C0, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_0073a0f0.cpp; no source of its own.


// Dynamic initializer (0x0073A1E0, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_0073a0f0.cpp; no source of its own.


// Dynamic initializer (0x0073A220, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_0073a0f0.cpp; no source of its own.


// Dynamic initializer (0x0073A250, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_0073a0f0.cpp; no source of its own.


// Dynamic initializer (0x0073A280, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_0073a0f0.cpp; no source of its own.


// Dynamic initializer (0x0073A2B0, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_0073a0f0.cpp; no source of its own.


// Dynamic initializer (0x0073A2C0, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_0073a0f0.cpp; no source of its own.


// int unknown_session_kind() -- 0x0073A2E0
// Purpose: the session kind the online state at 0x00A3D2A4 needs: 0 when the
// state is 0 or 5 (no login), 2 for state 2 and 1 for any other state. Inline
// helper of Wonline_Main 0x0073A300 (same unit), also emitted out of line.
// Assumptions: the state's meaning is a hypothesis (shared/wol_main.hpp).

int unknown_session_kind()
{
    if (unknown_a3d2a4 != 0 && unknown_a3d2a4 != 5) {
        return (unknown_a3d2a4 == 2) + 1;
    }
    return 0;
}


// int __fastcall Wonline_Main(bool first_time) (0x0073A300)
// Runs the online screens until a game starts (1) or the player leaves
// online (0, or the lobby's -1/-2): logs in for the session kind a state
// needs, logs out when the kind changes, and shuts the online layer down on
// the way out. first_time opens the buddy list over the first screen.

int __fastcall Wonline_Main(bool first_time)
{
    while (WWMouse->Get_Mouse_State() < 0) {
        WWMouse->Show_Mouse();
    }
    Unknown_00753F10();
    int theme = Theme.Score;
    if (theme == -1) {
        theme = Theme.Pending;
    }
    if (theme == Theme.From_Name("INTRO")) {
        Theme.Queue_Song(-3);
    }
    Unknown_WOL_Load_Settings_0073C7C0();
    bool shuffle = Theme.IsShuffle;
    Theme.IsShuffle = true;
    Unknown_0073C990();
    if (unknown_wstring_b26cb8.Length() == 0) {
        unknown_wstring_b26cb8 = WOL_Auto_Login_Persona();
        if (unknown_wstring_b26cb8.Length() == 0) {
            WolRegistryOptions options;
            options.Read_004ff200();
            if (options.unknown_004ff2c0()) {
                unknown_wstring_b26cb8 = WOL_Last_Valid_Persona();
            }
        }
    }
    int result = 0;
    bool done = false;
    for (;;) {
        int kind = unknown_session_kind();
        if (unknown_b264f0 == 0 && kind != 0) {
            result = Login_Wonline_00742fd0();
            switch (result) {
            case 0:
                if (unknown_a3d2a4 == 0) {
                    unknown_game_mode_a3d298 = 0;
                    Shutdown_Wonline_00742080();
                    Theme.IsShuffle = shuffle;
                    unknown_close_all_dialogs();
                    return result;
                }
                unknown_a3d2a4 = 0;
                unknown_a3d2a8 = 0;
                break;
            case 1:
                unknown_b264f0 = kind;
                break;
            default:
                unknown_game_mode_a3d298 = 0;
                unknown_a3d2a4 = 0;
                unknown_a3d2a8 = 0;
                Shutdown_Wonline_00742080();
                Theme.IsShuffle = shuffle;
                unknown_close_all_dialogs();
                return result;
            }
        }
        Unknown_0073C990();
        switch (unknown_a3d2a4) {
        case 0:
            first_time = false;
            result = Open_Wonline_Selection_00753110();
            done = result != 1;
            break;
        case 1:
            if (first_time) {
                Open_Buddy_Dialog_0075cba0();
                unknown_a3d2a4 = 0;
                break;
            }
            result = Unknown_00759E20();
            first_time = false;
            if (result == 0) {
                unknown_a3d2a4 = 0;
            } else {
                done = true;
            }
            break;
        case 3:
            result = Unknown_00743720(0, first_time);
            first_time = false;
            if (result == 0) {
                if (unknown_a3d2a4 == 3) {
                    unknown_a3d2a4 = 0;
                }
            } else {
                done = true;
            }
            break;
        case 2:
            if (first_time) {
                Open_Buddy_Dialog_0075cba0();
            }
            result = WDT_Setup_Game_00768e90(first_time);
            first_time = false;
            if (result == 0) {
                unknown_a3d2a4 = 0;
            } else {
                done = true;
            }
            break;
        case 4:
            result = Unknown_00759370(first_time);
            first_time = false;
            if (result == 0) {
                if (unknown_a3d2a4 == 4) {
                    unknown_a3d2a4 = unknown_a3d2a8;
                    unknown_a3d2a8 = 0;
                }
            } else {
                done = true;
            }
            break;
        case 5:
            result = Open_Wonline_Lobby_00753f70();
            first_time = false;
            if (result == -2 || result == -1) {
                unknown_game_mode_a3d298 = 0;
                unknown_a3d2a4 = 0;
                unknown_a3d2a8 = 0;
                Shutdown_Wonline_00742080();
                Theme.IsShuffle = shuffle;
                unknown_close_all_dialogs();
                return result;
            }
            if (result == 0) {
                unknown_a3d2a4 = 0;
            }
            break;
        }
        if (result != 1 && unknown_b264f0 != 0) {
            bool force = false;
            if (done) {
                force = true;
            }
            int kind = unknown_session_kind();
            if (unknown_b264f0 != kind || force) {
                Logout_Wonline_00741ea0();
                unknown_b264f0 = 0;
            }
        }
        if (done) {
            if (result == 1) {
                unknown_close_all_dialogs();
                return result;
            }
            Shutdown_Wonline_00742080();
            unknown_game_mode_a3d298 = 0;
            Theme.IsShuffle = shuffle;
            unknown_a3d2a4 = 0;
            unknown_a3d2a8 = 0;
            unknown_close_all_dialogs();
            return 0;
        }
    }
}
