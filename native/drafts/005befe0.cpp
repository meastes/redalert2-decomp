// long __stdcall Host_Dialog_Proc_005befe0(HWND window, unsigned message, unsigned wparam, long lparam) (0x005BEFE0)
// The LAN host dialog's procedure (from Red Alert's Net_New_Dialog).
#include "shared/session_state.hpp"
#include "shared/multiplayer_game_mode.hpp"
void *Native_Entry_From_Handle(int handle);   // NATIVE: native/link/native_event_data.cpp
#include "shared/lobby.hpp"
#include "shared/gdlg_supp.hpp"
#include "shared/modem_host.hpp"
#include "shared/wol_sink.hpp"
#include "shared/skirmish.hpp"
#include "shared/map_preview.hpp"
#include "shared/progress_screen.hpp"
#include "shared/campaign_score.hpp"
#include "shared/random2.hpp"
#include "shared/options_dialog.hpp"
#include "shared/string_table.hpp"
#include "shared/win32_window.hpp"
// @ref ??_C@_0BD@ELLL@D?3?2Ra2?2netdlg2?4cpp?$AA@=007e69c8
// @ref ??_C@_11LOCGONAA@?$AA?$AA@=00839d4c
// @ref ?Unknown_005e22b0@@YIPBDPAUHWND__@@0@Z=005e22b0
// @ref ?Unknown_005e8710@@YIXPAUHWND__@@@Z=005e8710
// @ref ?Unknown_005e5fc0@@YIXPAUHWND__@@@Z=005e5fc0
// @ref ?Unknown_005c7570@@YIXPAUHWND__@@@Z=005c7570
// @ref ?Change_Scenario_Slots_005cb8d0@@YIXPAUHWND__@@HH@Z=005cb8d0
// @ref ?Unknown_005cb560@@YIXPAUHWND__@@@Z=005cb560
// @ref ?Unknown_005cbf20@@YIXPAUHWND__@@HHH@Z=005cbf20
// @ref ?unknown_center_on_owner_737060@@YIXPAUHWND__@@@Z=00737060
// @ref ?Load_Lobby_Options_005cca30@@YIXPAUHWND__@@PAUSessionOptionsClass@@@Z=005cca30
// @ref ?Init_Clipping@UnknownVersionObject@@QAEXXZ=00713160
// @ref ?Select_Scenario_005c7c30@@YI_NH@Z=005c7c30
// @ref ?Open_Lobby_Dialog_005c6960@@YIHPAUHWND__@@@Z=005c6960
// @ref ?Is_Host_005c86a0@@YA_NXZ=005c86a0
// @ref ?unknown_host_shown_7e6dc0@@3_NA=007e6dc0
// @ref ?unknown_lobby_result_a71e8c@@3HA=00a71e8c
// @ref ?ProgressScreen@@3VProgressScreenClass@@A=00a769e8
// @ref ??3@YAXPAX@Z=00781d3b
// @ref ??1MapPreviewClass@@QAE@XZ=0061d440
// @ref __imp__ChildWindowFromPointEx@16=0079a3a4
// @ref _srand=0078469d
// @ref _rand=007846aa

char const *__fastcall Unknown_005e22b0(HWND window, HWND control);
void __fastcall Unknown_005e8710(HWND window);
void __fastcall Unknown_005e5fc0(HWND window);
void __fastcall unknown_center_on_owner_737060(HWND window);
void __fastcall Change_Scenario_Slots_005cb8d0(HWND dialog, int previous, int scenario);
void __fastcall Load_Lobby_Options_005cca30(HWND dialog, SessionOptionsClass *options);
void __fastcall Save_Lobby_Options_005cc890(HWND dialog, SessionOptionsClass *options);
bool __fastcall Unknown_Clear_Closed_Slots_005cacb0(HWND dialog);
void __fastcall unknown_set_dialog_item_text_005c34b0(HWND dialog);
struct UnknownTextOwner_005c3450;
void __fastcall unknown_set_dialog_wide_text_005c3450(HWND dialog, UnknownTextOwner_005c3450 *owner);
extern bool unknown_host_shown_7e6dc0;
// @ref ?Save_Lobby_Options_005cc890@@YIXPAUHWND__@@PAUSessionOptionsClass@@@Z=005cc890
// @ref ?Unknown_Clear_Closed_Slots_005cacb0@@YI_NPAUHWND__@@@Z=005cacb0
// @ref ?unknown_set_dialog_item_text_005c34b0@@YIXPAUHWND__@@@Z=005c34b0
// @ref ?unknown_set_dialog_wide_text_005c3450@@YIXPAUHWND__@@PAUUnknownTextOwner_005c3450@@@Z=005c3450

long __stdcall Host_Dialog_Proc_005befe0(HWND window, unsigned message, unsigned wparam, long lparam)
{
    if (message != 0x497) {
        Options_Dialog_Proc_005bf9b0(window, message, wparam, lparam);
    }
    switch (message) {
    case 0x84: {                // WM_NCHITTEST: the tool tip of the control under the cursor
        HWND tip = GetDlgItem(window, 0x695);
        if (tip != 0) {
            POINT point;
            point.x = (unsigned short)(lparam & 0xFFFF);
            point.y = (unsigned short)(((unsigned long)lparam >> 16) & 0xFFFF);
            RECT rect;
            GetWindowRect(window, &rect);
            point.y -= rect.top;
            point.x -= rect.left;
            wchar_t const *text = L"";
            HWND control = ChildWindowFromPointEx(window, point, 1);
            if (control != 0) {
                char const *label = Unknown_005e22b0(window, control);
                if (label != 0) {
                    text = StringTable::Load_String(label, 0, "D:\\Ra2\\netdlg2.cpp", 2704);
                }
            }
            SendMessageA(tip, 0x4B2, 0, (LPARAM)text);
            return 0;
        }
        break;
    }
    case 0x2B:                  // WM_DRAWITEM
        Unknown_005FE380(lparam);
        return 1;
    case 0x0F:                  // WM_PAINT
        Unknown_005FEE70(window);
        if (unknown_dialog_preview_a72ce8 != 0) {
            unknown_dialog_preview_a72ce8->Blit_Preview(window);
        }
        if (ProgressScreen.Has_Window()) {
            ProgressScreen.Draw_It(Point2D(-1, -1));
        }
        unknown_dialog_5e5f20(window);
        ValidateRect(window, 0);
        return 0;
    case 0x02:                  // WM_DESTROY
        if (ProgressScreen.Has_Window()) {
            ProgressScreen.Clear_Window();
            return 0;
        }
        break;
    case 0x111: {               // WM_COMMAND
        int id = (unsigned short)wparam;
        switch (id) {
        case 0x411:
        case 0x412:
        case 0x413:
        case 0x414:
        case 0x415:
        case 0x416:
        case 0x417:
        case 0x418:             // the player type combo boxes
            if ((unsigned short)(wparam >> 16) == 1 && !unknown_lock_a71e98 && Is_Host_005c86a0()) {
                int data = unknown_combo_selected_data_005c99d0(window, id, -1);
                Unknown_005cbf20(window, id, (NodeNameType *)Native_Entry_From_Handle(data), 2);   // NATIVE: handle (native/link/native_event_data.cpp)
                return 0;
            }
            break;
        case 2:
            if (!unknown_lock_a71e98) {
                if (unknown_dialog_preview_a72ce8 != 0) {
                    delete unknown_dialog_preview_a72ce8;
                    unknown_dialog_preview_a72ce8 = 0;
                }
                Save_Lobby_Options_005cc890(window, &unknown_session_a3d298.LANOptions);
                unknown_lobby_result_a71e8c = 2;
                return 0;
            }
            break;
        case 0x588:
            EnableWindow(GetDlgItem(window, 0x588), FALSE);
            unknown_lock_a71e98 = true;
            Save_Lobby_Options_005cc890(window, &unknown_session_a3d298.LANOptions);
            unknown_lobby_result_a71e8c = 0x588;
            return 0;
        case 0x540:             // the chat line
            Limit_Name_Text_005ccf50((HWND)lparam);
            if ((unsigned short)(wparam >> 16) == 0x501) {
                Send_Chat_Message_005bee20(window, unknown_color_7a737c, unknown_color_7a7364);
                return 0;
            }
            break;
        case 0x5AA:             // the map choice
            if (!unknown_lock_a71e98) {
                int old_scenario = unknown_session_a3d2b4;
                bool old_unlimited = unknown_session_a3d298.Mode == 0 || unknown_session_a3d298.Mode->unknown_slot12();
                bool old_flag = unknown_session_a3d298.Mode == 0 || unknown_session_a3d298.Mode->unknown_slot40();
                Unknown_005e5fc0(window);
                ShowWindow(window, SW_HIDE);
                unknown_host_shown_7e6dc0 = false;
                if (Open_Lobby_Dialog_005c6960(MainWindow) == 2) {
                    Select_Scenario_005c7c30(unknown_session_a3d2b4 = old_scenario);
                    Unknown_005c7570(window);
                    unknown_host_shown_7e6dc0 = true;
                    ShowWindow(window, SW_SHOW);
                    Load_Random_Map_Preview_005cc800(window);
                    Unknown_005c3810(true, false);
                    InvalidateRect(window, 0, FALSE);
                    return 0;
                }
                if (!Select_Scenario_005c7c30(unknown_session_a3d2b4)) {
                    unknown_session_a3d2b4 = old_scenario;
                } else {
                    Change_Scenario_Slots_005cb8d0(window, old_scenario, unknown_session_a3d2b4);
                }
                unknown_host_shown_7e6dc0 = true;
                ShowWindow(window, SW_SHOW);
                unknown_set_dialog_item_text_005c34b0(window);
                Load_Random_Map_Preview_005cc800(window);
                bool unlimited = unknown_session_a3d298.Mode == 0 || unknown_session_a3d298.Mode->unknown_slot12();
                bool flag = unknown_session_a3d298.Mode == 0 || unknown_session_a3d298.Mode->unknown_slot40();
                if (old_flag && !flag) {
                    Unknown_005cb560(window);
                }
                if (old_unlimited && !unlimited && Unknown_Clear_Closed_Slots_005cacb0(window)) {
                    unknown_session_a3d298.AICount = 0;
                    Send_Slot_Names_005bbd20();
                    Unknown_005bbdd0(false);
                    Unknown_005c3810(true, false);
                    return 0;
                }
                Unknown_005bbdd0(true);
                return 0;
            }
            break;
        case 0x510:
        case 0x513:
        case 0x514:
        case 0x51E:
        case 0x51F:
        case 0x520:
        case 0x521:
        case 0x6A1:             // the country combo boxes
            if ((unsigned short)(wparam >> 16) == 1 && !unknown_lock_a71e98) {
                Set_Slot_Country_005c9a10(window, id);
                Unknown_005c3810(true, false);
                Unknown_005bbdd0(false);
                return 0;
            }
            break;
        case 0x522:
        case 0x523:
        case 0x524:
        case 0x525:
        case 0x526:
        case 0x527:
        case 0x528:
        case 0x6A2:             // the colour combo boxes
            if ((unsigned short)(wparam >> 16) == 1 && !unknown_lock_a71e98) {
                Set_Slot_Color_005c9ab0(window, id);
                Unknown_005c3810(true, false);
                Unknown_005bbdd0(false);
                return 0;
            }
            break;
        default:
            return 0;
        }
        break;
    }
    case 0x497:
        VerNum.Init_Clipping();
        srand(unknown_random_839218(1, 0x7FFF));
        Seed = rand();
        unknown_center_on_owner_737060(window);
        Fill_Side_Combos_4d16d0(window);
        Init_Color_Entries_4d1ea0();
        Fill_Color_Combos_4d2300(window);
        Select_Player_Side_4d1820(window, unknown_lobby_a3fa94);
        Select_Player_Color_4d23c0(window, unknown_lobby_a3fa94);
        Unknown_005bbdd0(false);
        Load_Lobby_Options_005cca30(window, &unknown_session_a3d298.LANOptions);
        if (unknown_session_a3d298.Mode != 0 && !unknown_session_a3d298.Mode->unknown_slot40()) {
            unknown_lobby_flag_7e6d90 = -1;
            unknown_a72c68 = 0;
            Unknown_005cd540(0);
            unknown_player_slots_a3fa98[0]->Team = 0;
            unknown_player_slots_a3fa98[0]->unknown_66e2d0(unknown_session_a3d298.Mode->unknown_slot10());
            unknown_player_slots_a3fa98[0]->unknown_66e300(-2);
            Fill_Color_Combos_4d2300(window);
        }
        Unknown_005c9b90(window, 0, true);
        Change_Scenario_Slots_005cb8d0(window, -1, unknown_session_a3d2b4);
        if (unknown_lobby_flag_7e6d90 == 0 && unknown_session_a3d298.Mode != 0 && unknown_session_a3d298.Mode->unknown_slot40()) {
            unknown_player_slots_a3fa98[0]->unknown_66e2d0(-3);
            unknown_player_slots_a3fa98[0]->Team = -1;
        }
        Refresh_Slot_Images_005c9ec0(window);
        // NATIVE (codex4 r565): every mode read goes through Session.Mode (0x00A3D29C); the old view unknown_session_a3d29c is the same object only through the r36 member alias.
        unknown_set_dialog_wide_text_005c3450(window, (UnknownTextOwner_005c3450 *)unknown_session_a3d298.Mode);
        unknown_set_dialog_item_text_005c34b0(window);
        Load_Random_Map_Preview_005cc800(window);
        Options_Dialog_Proc_005bf9b0(window, 0x497, wparam, lparam);
        Refresh_Game_List_005bc790();
        return 0;
    case 0x114:                 // WM_HSCROLL
    case 0x115:                 // WM_VSCROLL: the sliders
        if (!unknown_lock_a71e98) {
            unknown_session_a3d2c8 = 6 - SendDlgItemMessageA(window, 0x529, 0x400, 0, 0);
            unknown_session_a3d2bc = SendDlgItemMessageA(window, 0x402, 0x400, 0, 0);
            unknown_session_a3d2d0 = SendDlgItemMessageA(window, 0x54F, 0x400, 0, 0);
            return 0;
        }
        break;
    case 0x49B:
        {
            HWND item = GetDlgItem(window, wparam);
            SendMessageA(item, 0x4B3, 0x7F, lparam);
        }
        return 0;
    case 0x14:                  // WM_ERASEBKGND
        return 1;
    case 0x4E9: {               // the tool tip text of a combo box item
        ToolTipTextRequest *request = (ToolTipTextRequest *)lparam;
        request->Text = 0;
        if (request->Window != 0 && request->Item != -1) {
            int control = GetDlgCtrlID(request->Window);
            if (Control_Slot_005c9800(control) == -1) {
                if (Side_Combo_Player_4d13b0(control) == -1) {
                    if (Color_Combo_Player_4d1d10(control) != -1) {
                        request->Text = Color_Name_4d1d80(Color_Combo_Data_4d2900(window, control, request->Item));
                    }
                } else {
                    int side = Side_Combo_Data_4d1c70(window, control, request->Item);
                    if (side != -1) {
                        request->Text = Side_Name_4d1420(side);
                    }
                }
            } else {
                request->Text = Entry_Type_Name_005c9870((NodeNameType *)Native_Entry_From_Handle(unknown_combo_selected_data_005c99d0(window, control, request->Item)));   // NATIVE: slot-entry handle (Scout1242)
            }
        }
        return 1;
    }
    case 0x4EC:
        Unknown_005e8710(window);
        break;
    default:
        return 0;
    }
    return 0;
}
