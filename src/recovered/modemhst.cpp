// modemhst.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/modemhst.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/modem_host.hpp"
#include "shared/ms_message_box.hpp"
#include "shared/comm_buffer.hpp"
#include "shared/debug_printf.hpp"
#include "shared/session_state.hpp"
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ADD
#include "shared/queue.hpp"
#include "shared/serial_progress.hpp"
#include "shared/lobby.hpp"
#include "shared/trigger_type.hpp"
#include "shared/house_type.hpp"
#include "shared/rules.hpp"
#include "shared/sound_play.hpp"
#include "shared/null_dlg.hpp"
#include "shared/string_table.hpp"
#include "shared/keyboard.hpp"
#include "shared/map_preview.hpp"
#include "shared/options_dialog.hpp"
#include "shared/ww_message_box.hpp"
#include "shared/id_source.hpp"
#include "shared/modem_guest.hpp"
#include "shared/cd_timer.hpp"
#include "shared/raw_file_class.hpp"
#include "shared/buffer_pipe.hpp"
#include "shared/lzo_pipe.hpp"
#include "shared/null_modem.hpp"
#include "shared/unknown_player_entry.hpp"
#include "shared/campaign.hpp"
#include "shared/special.hpp"
#include "shared/random2.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/multi_mission.hpp"
#include "shared/win32_window.hpp"
#include "shared/crt_string.hpp"
#include "shared/unknown_map_generator.hpp"


// Header-static dynamic initializers of the unit at 0x00598C20: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x00598C20-0x00598E01); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x00598C40, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_00598c20.cpp; no source of its own.


// Dynamic initializer (0x00598C70, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_00598c20.cpp; no source of its own.


// Dynamic initializer (0x00598C90, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_00598c20.cpp; no source of its own.


// Dynamic initializer (0x00598CB0, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_00598c20.cpp; no source of its own.


// Dynamic initializer (0x00598CD0, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_00598c20.cpp; no source of its own.


// Dynamic initializer (0x00598CF0, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_00598c20.cpp; no source of its own.


// Dynamic initializer (0x00598D10, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_00598c20.cpp; no source of its own.


// Dynamic initializer (0x00598D50, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_00598c20.cpp; no source of its own.


// Dynamic initializer (0x00598D80, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_00598c20.cpp; no source of its own.


// Dynamic initializer (0x00598DB0, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_00598c20.cpp; no source of its own.


// Dynamic initializer (0x00598DE0, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_00598c20.cpp; no source of its own.


// Dynamic initializer (0x00598DF0, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_00598c20.cpp; no source of its own.


// ModemHost::Dialog_00598e10 (0x00598E10)

bool ModemHost::Dialog_00598e10()
{
    bool result = false;
    Debug_Printf("ModemHost.Dialog(enter)\n");
    Result = -1;
    Window = unknown_create_dialog_5ff610(0xF1, Dialog_Proc_00598fa0, 0);
    if (Window) {
        SetWindowLongA(Window, 8, (LONG)this);
        Init_Dialog_00599920();
        unknown_dialog_5ff7c0(Window);
        Changed = false;
        OptionsChanged = true;
        OptionsTime = TickCount.Time();
        GuestResponseTime = 10000;
        ReceiveTime = TickCount.Time();
        ReceiveTimeout = 1200;
        TimingTime = 0;
        unknown_20 = false;
        GuestName[0] = 0;
        GuestColor = -1;
        GuestHouse = -1;
        unknown_modem_host_a70de4 = this;
        unknown_54 = Unknown_006525F0();
        Read_Art_INI();
        unknown_58 = unknown_art_ini_unique_id();
        unknown_5c = unknown_ini_8397a0_unique_id();
        for (;;) {
            Result = -1;
            while (Result < 0) {
                if (unknown_6000d0() == true) {
                    break;
                }
                Service_0059b560(false);
                if (Process_Messages_0059b690(false) == true) {
                    break;
                }
                unknown_517370(false);
            }
            if (Result != 1) {
                break;
            }
            ShowWindow(Window, 0);
            if (Start_Game_0059c290(false) == true) {
                result = true;
                break;
            }
            ShowWindow(Window, 5);
        }
        unknown_dialog_5ff6e0(Window);
        unknown_modem_host_a70de4 = 0;
        if (Changed == true) {
            unknown_session_a3d298.Write_MultiPlayer_Settings();
        }
    }
    if (Result == 2) {
        Sign_Off_0059c130();
    }
    Debug_Printf("ModemHost.Dialog(exit %s)\n", result == true ? "true" : "false");
    return result;
}


// ModemHost::Dialog_Proc_00598fa0 (0x00598FA0): TibSun modemhst.cpp Modem_Host_Dialog_Proc (outer case order
// WM_COMMAND, WM_HSCROLL/VSCROLL, WM_DESTROY, WM_PAINT). The /Ob2 budget matches the original only with the
// chat chain (Send_Chat 0x59A880 -> 0x59BFA0 -> Show_Chat 0x59C0B0 calls) and the checkbox readers' named
// state/checked locals: Read_Unit_Count inlines, Read_Tech_Level and ??_G 0x59CB80 stay calls.
// @ref? ??_C@_13ECNF@?$AA?5?$AA?$AA@ hits 007a0190 007a01f0 007a0250 007a02b0 007a0310
// Read_AI_Difficulty_0059ad80 (0x0059AD80)
// Read_AI_Players_0059ad10 (0x0059AD10)
// Read_Bases_0059adf0 (0x0059ADF0)
// Read_Bridge_Destroy_0059aee0 (0x0059AEE0)
// Read_Color_0059aa80 (0x0059AA80)
// Read_Color_Entry_0059aa20 (0x0059AA20)
// Read_Crap_Engineers_0059b050 (0x0059B050)
// Read_Crates_0059ae60 (0x0059AE60)
// Read_Credits_0059ace0 (0x0059ACE0)
extern "C" int __cdecl swprintf(unsigned short *buffer, unsigned short const *format, ...);
// Read_Fog_Of_War_0059aea0 (0x0059AEA0)
// Read_Game_Speed_0059ad40 (0x0059AD40)
// Read_Harvester_Truce_0059af20 (0x0059AF20)
// Read_MCV_Redeploy_0059adb0 (0x0059ADB0)
// Read_Name_0059aaf0 (0x0059AAF0)
// Read_Short_Game_0059af60 (0x0059AF60)
// Read_Short_Game_0059b010 (0x0059B010)
// Read_Tech_Level_0059acb0 (0x0059ACB0)
// Read_Unit_Count_0059ac80 (0x0059AC80)
// Read_unknown_2b_0059afd0 (0x0059AFD0)
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern unsigned short const unknown_empty_wide_839d4c[];
// Set_Guest_0059ab80 (0x0059AB80)
extern "C" int __cdecl _wcsicmp(unsigned short const *left, unsigned short const *right);
extern "C" void __cdecl srand(unsigned int seed);
extern "C" int __cdecl rand();


int __stdcall ModemHost::Dialog_Proc_00598fa0(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    int result = unknown_dialog_proc_5ffb00(window, message, wparam, lparam);
    if (result) {
        return result;
    }
    ModemHost *host = (ModemHost *)GetWindowLongA(window, 8);
    switch (message) {
    case 0x111:
        switch ((unsigned short)wparam) {
        case 1:
            if ((unsigned short)(wparam >> 16) == 0 && host->Read_Dialog_0059a310() == true) {
                host->Send_Options_0059bc90();
                host->Result = 1;
            }
            break;
        case 2:
            if ((unsigned short)(wparam >> 16) == 0) {
                host->Read_Dialog_0059a310();
                host->Result = 2;
            }
            break;
        case 0x405:
            if ((unsigned short)(wparam >> 16) == 1) {
                host->Read_Color_0059aa80();
            }
            break;
        case 0x540:
            if ((unsigned short)(wparam >> 16) == 0x501) {
                host->Send_Chat_0059a880();
            }
            break;
        case 0x54C:
            host->Read_Crates_0059ae60();
            break;
        case 0x54D:
            host->Read_Fog_Of_War_0059aea0();
            break;
        case 0x54E:
            host->Read_Short_Game_0059af60();
            break;
        case 0x551:
            host->Read_Crap_Engineers_0059b050();
            break;
        case 0x596:
            host->Read_Harvester_Truce_0059af20();
            break;
        case 0x667:
            if ((unsigned short)(wparam >> 16) == 1) {
                host->Read_Color_Entry_0059aa20();
            }
            break;
        case 0x5C1:
            if ((unsigned short)(wparam >> 16) == 0x501 || (unsigned short)(wparam >> 16) == 0x300) {
                host->Read_Name_0059aaf0((unsigned short)(wparam >> 16) == 0x300);
            }
            break;
        case 0x66B:
            host->Read_MCV_Redeploy_0059adb0();
            break;
        case 0x66A:
            host->Read_Bases_0059adf0();
            break;
        case 0x624:
            host->Read_Bridge_Destroy_0059aee0();
            break;
        case 0x69A:
            host->Read_unknown_2b_0059afd0();
            break;
        case 0x69D:
            host->Read_Short_Game_0059b010();
            break;
        case 0x5AA:
            if ((unsigned short)(wparam >> 16) == 0) {
                host->Select_Multi_Map_0059b090();
            }
            break;
        }
        break;
    case 0x114:
    case 0x115:
        if (GetDlgItem(window, 0x669) == (HWND)lparam) {
            host->Read_Unit_Count_0059ac80();
        } else if (GetDlgItem(window, 0x550) == (HWND)lparam) {
            host->Read_Tech_Level_0059acb0();
        } else if (GetDlgItem(window, 0x402) == (HWND)lparam) {
            host->Read_Credits_0059ace0();
        } else if (GetDlgItem(window, 0x552) == (HWND)lparam) {
            host->Read_AI_Players_0059ad10();
        } else if (GetDlgItem(window, 0x553) == (HWND)lparam) {
            host->Read_AI_Difficulty_0059ad80();
        } else if (GetDlgItem(window, 0x529) == (HWND)lparam) {
            host->Read_Game_Speed_0059ad40();
        }
        break;
    case 0x2:
        if (unknown_dialog_preview_a72ce8) {
            delete unknown_dialog_preview_a72ce8;
            unknown_dialog_preview_a72ce8 = 0;
        }
        break;
    case 0xF:
        if (unknown_dialog_preview_a72ce8) {
            unknown_dialog_preview_a72ce8->Blit_Preview(window);
        }
        ValidateRect(window, 0);
        break;
    }
    return 0;
}


// ModemHost::Init_Dialog_00599920 (0x00599920)
extern "C" void __cdecl srand(unsigned int seed);
extern "C" int __cdecl rand();
extern unsigned short const unknown_empty_wide_839d4c[];

void ModemHost::Init_Dialog_00599920()
{
    unsigned short text[1024];
    Debug_Printf("ModemHost.InitDialog(enter)\n");
    if (unknown_modem_host_first_7e2410 == true) {
        unknown_modem_host_first_7e2410 = false;
        unknown_special_a40920.Unknown_0068B420();
        unknown_session_a3d298.unknown_24 = Rules->Money;
        unknown_session_a3d298.unknown_bases_20 = Rules->Bases;
        unknown_session_a3d298.unknown_28 = Rules->BridgeDestruction;
        unknown_session_a3d298.unknown_crates_29 = Rules->Crates;
        unknown_session_a3d298.AICount = 0;
        unknown_session_a3d298.UnitCount = Rules->UnitCount;
        unknown_session_a3d298.unknown_2a = false;
        unknown_session_a3d298.unknown_2b = Rules->SuperWeaponsAllowed;
        unknown_session_a3d298.unknown_2c = Rules->BuildOffAlly;
        unknown_session_a3d298.GameSpeed_30 = unknown_a40b18;
        unknown_session_a3d298.unknown_34 = false;
    }
    HWND control = GetDlgItem(Window, 0x5C1);
    if (control) {
        SendMessageA(control, 0xC5, 19, 0);
        SendMessageA(control, 0x4B2, 0, (LPARAM)Narrow_To_Wide_006fa180(unknown_name_a3d3a0));
    }
    Debug_Printf("Name: %ls\n", unknown_name_a3d3a0);
    control = GetDlgItem(Window, 0x405);
    if (control) {
        SendMessageA(control, 0x14B, 0, 0);
        SendMessageA(control, 0x4C1, -1, (LPARAM)StringTable::Load_String("TXT_GOLD", 0, "D:\\Ra2\\ModemHst.cpp", 0x197));
        SendMessageA(control, 0x4C1, -1, (LPARAM)StringTable::Load_String("TXT_RED", 0, "D:\\Ra2\\ModemHst.cpp", 0x198));
        SendMessageA(control, 0x4C1, -1, (LPARAM)StringTable::Load_String("TXT_BLUE", 0, "D:\\Ra2\\ModemHst.cpp", 0x199));
        SendMessageA(control, 0x4C1, -1, (LPARAM)StringTable::Load_String("TXT_GREEN", 0, "D:\\Ra2\\ModemHst.cpp", 0x19A));
        SendMessageA(control, 0x4C1, -1, (LPARAM)StringTable::Load_String("TXT_ORANGE", 0, "D:\\Ra2\\ModemHst.cpp", 0x19B));
        SendMessageA(control, 0x4C1, -1, (LPARAM)StringTable::Load_String("TXT_SKY_BLUE", 0, "D:\\Ra2\\ModemHst.cpp", 0x19C));
        SendMessageA(control, 0x4C1, -1, (LPARAM)StringTable::Load_String("TXT_PURPLE", 0, "D:\\Ra2\\ModemHst.cpp", 0x19D));
        SendMessageA(control, 0x4C1, -1, (LPARAM)StringTable::Load_String("TXT_PINK", 0, "D:\\Ra2\\ModemHst.cpp", 0x19E));
        for (int index = 0; index < 8; index++) {
            SendMessageA(control, 0x498, index, unknown_color_values_7e6d94[index]);
        }
        unknown_a3d3b4 = unknown_session_a3d298.unknown_12c;
        SendMessageA(control, 0x14E, unknown_a3d3b4, 0);
    }
    Debug_Printf("Color: %d\n", unknown_a3d3b4);
    control = GetDlgItem(Window, 0x667);
    if (control) {
        SendMessageA(control, 0x14B, 0, 0);
        for (int house = 0; house < HouseTypes.Count(); house++) {
            if (HouseTypes[house]->Multiplay) {
                wchar_t const *name = HouseTypes[house]->UIName;
                int row = SendMessageA(control, 0x4C2, 0, (LPARAM)name);
                SendMessageA(control, 0x151, row, house);
            }
        }
        int row = Find_Combo_Item_Data_00736e10(control, -1, unknown_a3d3bc);
        SendMessageA(control, 0x14E, row, 0);
    }
    control = GetDlgItem(Window, 0x669);
    if (control) {
        int value = unknown_session_a3d298.UnitCount;
        SendMessageA(control, 0x406, 1, ((unsigned long)(unsigned short)Rules->MaxUnitCount << 16) | (unsigned short)Rules->MinUnitCount);
        SendMessageA(control, 0x405, 1, value);
    }
    Debug_Printf("UnitCount: %d\n", unknown_session_a3d298.UnitCount);
    control = GetDlgItem(Window, 0x550);
    if (control) {
        int value = unknown_house_setting_7d88d8;
        SendMessageA(control, 0x406, 1, 0xA0001);
        SendMessageA(control, 0x405, 1, value);
    }
    Debug_Printf("TechLevel: %d\n", unknown_house_setting_7d88d8);
    control = GetDlgItem(Window, 0x402);
    if (control) {
        int value = unknown_session_a3d298.unknown_24;
        SendMessageA(control, 0x406, 1, ((unsigned long)(unsigned short)Rules->MaxMoney << 16) | (unsigned short)Rules->MinMoney);
        SendMessageA(control, 0x405, 1, value);
        SendMessageA(control, 0x4AB, 0, 250);
    }
    Debug_Printf("Credits: %d\n", unknown_session_a3d298.unknown_24);
    control = GetDlgItem(Window, 0x552);
    if (control) {
        int value = unknown_session_a3d298.AICount;
        SendMessageA(control, 0x406, 1, 0x60000);
        SendMessageA(control, 0x405, 1, value);
    }
    Debug_Printf("AIPlayers: %d\n", unknown_session_a3d298.AICount);
    control = GetDlgItem(Window, 0x529);
    if (control) {
        SendMessageA(control, 0x406, 1, 0x60000);
        SendMessageA(control, 0x405, 1, 6 - unknown_a40b18);
    }
    Debug_Printf("Game Speed: %d\n", unknown_session_a3d298.GameSpeed_30);
    control = GetDlgItem(Window, 0x553);
    if (control) {
        int value = unknown_session_a3d298.unknown_difficulty_40;
        SendMessageA(control, 0x406, 1, 0x20000);
        SendMessageA(control, 0x405, 1, value);
    }
    Debug_Printf("AIDifficulty: %d\n", unknown_session_a3d298.unknown_difficulty_40);
    control = GetDlgItem(Window, 0x66B);
    if (control) {
        SendMessageA(control, 0xF1, unknown_session_a3d298.unknown_a8 == true, 0);
    }
    Debug_Printf("MCV Redeploy: %s\n", unknown_session_a3d298.unknown_a8 == true ? "ON" : "OFF");
    control = GetDlgItem(Window, 0x66A);
    if (control) {
        SendMessageA(control, 0xF1, unknown_session_a3d298.unknown_bases_20 == true, 0);
    }
    Debug_Printf("Bases: %s\n", unknown_session_a3d298.unknown_bases_20 == true ? "ON" : "OFF");
    control = GetDlgItem(Window, 0x54C);
    if (control) {
        SendMessageA(control, 0xF1, unknown_session_a3d298.unknown_crates_29 == true, 0);
    }
    Debug_Printf("Crates: %s\n", unknown_session_a3d298.unknown_crates_29 == true ? "ON" : "OFF");
    control = GetDlgItem(Window, 0x54D);
    if (control) {
        SendMessageA(control, 0xF1, unknown_session_a3d298.unknown_fog_a7 == true, 0);
    }
    Debug_Printf("FogOfWar: %s\n", unknown_session_a3d298.unknown_fog_a7 == true ? "ON" : "OFF");
    control = GetDlgItem(Window, 0x624);
    if (control) {
        SendMessageA(control, 0xF1, unknown_session_a3d298.unknown_28 == true, 0);
    }
    Debug_Printf("BridgeDestroy: %s\n", unknown_session_a3d298.unknown_28 == true ? "ON" : "OFF");
    control = GetDlgItem(Window, 0x596);
    if (control) {
        SendMessageA(control, 0xF1, unknown_session_a3d298.unknown_a5 == true, 0);
    }
    Debug_Printf("Harvester Truce: %s\n", unknown_session_a3d298.unknown_a5 == true ? "ON" : "OFF");
    control = GetDlgItem(Window, 0x54E);
    if (control) {
        SendMessageA(control, 0xF1, unknown_session_a3d298.unknown_2a == true, 0);
    }
    Debug_Printf("Short Game: %s\n", unknown_session_a3d298.unknown_2a == true ? "ON" : "OFF");
    control = GetDlgItem(Window, 0x69A);
    if (control) {
        SendMessageA(control, 0xF1, unknown_session_a3d298.unknown_2b == true, 0);
    }
    Debug_Printf("SuperWeapons: %s\n", unknown_session_a3d298.unknown_2b == true ? "ON" : "OFF");
    control = GetDlgItem(Window, 0x69D);
    if (control) {
        SendMessageA(control, 0xF1, unknown_session_a3d298.unknown_2c == true, 0);
    }
    Debug_Printf("BuildOffAlly: %s\n", unknown_session_a3d298.unknown_2c == true ? "ON" : "OFF");
    control = GetDlgItem(Window, 0x551);
    if (control) {
        SendMessageA(control, 0xF1, unknown_session_a3d298.unknown_34 == true, 0);
    }
    Debug_Printf("Crap Engineers: %s\n", unknown_session_a3d298.unknown_34 == true ? "ON" : "OFF");
    VerNum.Init_Clipping();
    srand(unknown_random_839218(1, 0x7FFF));
    Seed = rand();
    unknown_special_a40920.unknown_bit_4 = Rules->CaptureTheFlag;
    if (unknown_dialog_preview_a72ce8) {
        delete unknown_dialog_preview_a72ce8;
        unknown_dialog_preview_a72ce8 = 0;
    }
    Free_Pointers_005a7480(&unknown_players_a3fa7c);
    Select_Scenario_005c7c30(0);
    unknown_session_a3d298.ScenarioIndex = 0;
    HWND window = Window;
    control = GetDlgItem(window, 0x5A8);
    SendMessageA(control, 0x4B2, 0, (LPARAM)unknown_map_name_a3d342);
    unknown_session_a3f616 = true;
    if (CCFileClass(unknown_session_a3d298.ScenarioFileName_658).Is_Available()) {
        unknown_dialog_preview_a72ce8 = new MapPreviewClass;
        unknown_dialog_preview_a72ce8->Read_INI_Preview(unknown_session_a3d298.ScenarioFileName_658);
        PostMessageA(Window, 0xF, 0, 0);
    }
    if (strlen(ModemRXString) > 78) {
        ModemRXString[79] = 0;
    }
    if (strlen(ModemRXString)) {
        Char_To_Wide_006fa0c0(text, ModemRXString, -1);
        Show_Chat_0059c0b0(text, -1, unknown_empty_wide_839d4c);
    }
    ModemRXString[0] = 0;
    control = GetDlgItem(Window, 1);
    if (control) {
        EnableWindow(control, 0);
    }
    serial_manager_00a40a28.Reset_Response_Time(true);
    Debug_Printf("ModemHost.InitDialog(exit)\n");
}


// ModemHost::Read_Dialog_0059a310 (0x0059A310)
extern "C" int __cdecl swprintf(unsigned short *buffer, unsigned short const *format, ...);

bool ModemHost::Read_Dialog_0059a310()
{
    Debug_Printf("ModemHost.ReadDialog(enter)\n");
    int max_players = Unknown_005c6600(unknown_session_a3d298.ScenarioIndex);
    int players = 2;
    HWND control = GetDlgItem(Window, 0x552);
    if (control) {
        players = SendMessageA(control, 0x400, 0, 0) + 2;
    }
    if (max_players < players) {
        unsigned short text[256];
        swprintf(text, StringTable::Load_String("TXT_SCENARIO_TOO_SMALL", 0, "D:\\Ra2\\ModemHst.cpp", 0x2F7), max_players);
        WWMessageBox(L" ").Process(text, StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\ModemHst.cpp", 0x2F8), 0, 0, false);
        return false;
    }

    control = GetDlgItem(Window, 0x5C1);
    if (control) {
        unsigned short name[20];
        SendMessageA(control, 0x4B3, 20, (LPARAM)name);
        WideToCharString_006fa0f0(unknown_name_a3d3a0, name, -1);
    }
    Debug_Printf("Name: %ls\n", unknown_name_a3d3a0);

    control = GetDlgItem(Window, 0x405);
    if (control) {
        unknown_a3d3b4 = SendMessageA(control, 0x147, 0, 0);
    }
    Debug_Printf("Color: %d\n", unknown_a3d3b4);

    control = GetDlgItem(Window, 0x667);
    if (control) {
        int row = SendMessageA(control, 0x147, 0, 0);
        unknown_a3d3bc = SendMessageA(control, 0x150, row, 0);
    }

    control = GetDlgItem(Window, 0x669);
    if (control) {
        unknown_session_a3d298.UnitCount = SendMessageA(control, 0x400, 0, 0);
    }
    Debug_Printf("UnitCount: %d\n", unknown_session_a3d298.UnitCount);

    control = GetDlgItem(Window, 0x550);
    if (control) {
        unknown_house_setting_7d88d8 = SendMessageA(control, 0x400, 0, 0);
    }
    Debug_Printf("TechLevel: %d\n", unknown_house_setting_7d88d8);

    control = GetDlgItem(Window, 0x402);
    if (control) {
        unknown_session_a3d298.unknown_24 = SendMessageA(control, 0x400, 0, 0);
    }
    Debug_Printf("Credits: %d\n", unknown_session_a3d298.unknown_24);

    control = GetDlgItem(Window, 0x552);
    if (control) {
        unknown_session_a3d298.AICount = SendMessageA(control, 0x400, 0, 0);
    }
    Debug_Printf("AIPlayers: %d\n", unknown_session_a3d298.AICount);

    control = GetDlgItem(Window, 0x529);
    if (control) {
        unknown_session_a3d298.GameSpeed_30 = 6 - SendMessageA(control, 0x400, 0, 0);
    }
    Debug_Printf("Game Speed: %d\n", unknown_session_a3d298.GameSpeed_30);

    control = GetDlgItem(Window, 0x553);
    if (control) {
        unknown_session_a3d298.unknown_difficulty_40 = SendMessageA(control, 0x400, 0, 0);
    }
    Debug_Printf("AIDifficulty: %d\n", unknown_session_a3d298.unknown_difficulty_40);

    control = GetDlgItem(Window, 0x66B);
    if (control) {
        unknown_session_a3d298.unknown_a8 = SendMessageA(control, 0xF0, 0, 0) == 1;
    }
    Debug_Printf("MCVRedeploy: %s\n", unknown_session_a3d298.unknown_a8 == true ? "ON" : "OFF");

    control = GetDlgItem(Window, 0x66A);
    if (control) {
        unknown_session_a3d298.unknown_bases_20 = SendMessageA(control, 0xF0, 0, 0) == 1;
    }
    Debug_Printf("Bases: %s\n", unknown_session_a3d298.unknown_bases_20 == true ? "ON" : "OFF");

    control = GetDlgItem(Window, 0x54C);
    if (control) {
        unknown_session_a3d298.unknown_crates_29 = SendMessageA(control, 0xF0, 0, 0) == 1;
    }
    Debug_Printf("Crates: %s\n", unknown_session_a3d298.unknown_crates_29 == true ? "ON" : "OFF");

    control = GetDlgItem(Window, 0x54D);
    if (control) {
        unknown_session_a3d298.unknown_fog_a7 = SendMessageA(control, 0xF0, 0, 0) == 1;
    }
    Debug_Printf("FogOfWar: %s\n", unknown_session_a3d298.unknown_fog_a7 == true ? "ON" : "OFF");

    control = GetDlgItem(Window, 0x624);
    if (control) {
        unknown_session_a3d298.unknown_28 = SendMessageA(control, 0xF0, 0, 0) == 1;
    }
    Debug_Printf("BridgeDestroy: %s\n", unknown_session_a3d298.unknown_28 == true ? "ON" : "OFF");

    control = GetDlgItem(Window, 0x596);
    if (control) {
        unknown_session_a3d298.unknown_a5 = SendMessageA(control, 0xF0, 0, 0) == 1;
    }
    Debug_Printf("Harvester Truce: %s\n", unknown_session_a3d298.unknown_a5 == true ? "ON" : "OFF");

    control = GetDlgItem(Window, 0x54E);
    if (control) {
        unknown_session_a3d298.unknown_2a = SendMessageA(control, 0xF0, 0, 0) == 1;
    }
    Debug_Printf("Short Game: %s\n", unknown_session_a3d298.unknown_2a == true ? "ON" : "OFF");

    control = GetDlgItem(Window, 0x69A);
    if (control) {
        unknown_session_a3d298.unknown_2b = SendMessageA(control, 0xF0, 0, 0) == 1;
    }
    Debug_Printf("Short Game: %s\n", unknown_session_a3d298.unknown_2a == true ? "ON" : "OFF");

    control = GetDlgItem(Window, 0x69D);
    if (control) {
        unknown_session_a3d298.unknown_2c = SendMessageA(control, 0xF0, 0, 0) == 1;
    }
    Debug_Printf("Short Game: %s\n", unknown_session_a3d298.unknown_2a == true ? "ON" : "OFF");

    control = GetDlgItem(Window, 0x551);
    if (control) {
        unknown_session_a3d298.unknown_34 = SendMessageA(control, 0xF0, 0, 0) == 1;
    }
    Debug_Printf("Crap Engineers: %s\n", unknown_session_a3d298.unknown_34 == true ? "ON" : "OFF");
    Debug_Printf("ModemHost.ReadDialog(exit)\n");
    return true;
}


// Send_Chat_0059a880 (0x0059A880)
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" int __cdecl swprintf(unsigned short *buffer, unsigned short const *format, ...);
extern unsigned short const unknown_empty_wide_839d4c[];


void ModemHost::Send_Chat_0059a880()
{
    unsigned short text[256];
    memset(text, 0, sizeof(text));
    HWND control = GetDlgItem(Window, 0x540);
    if (control) {
        SendMessageA(control, 0x4B3, 256, (LPARAM)text);
        SendMessageA(control, 0x4B2, 0, (LPARAM)unknown_empty_wide_839d4c);
        if (wcslen(text) && text[0] != 0x0D) {
            wcstrim(text);
            Send_Chat_0059bfa0(text);
        }
    }
}


// Read_Color_Entry_0059aa20 (0x0059AA20)

void ModemHost::Read_Color_Entry_0059aa20()
{
    HWND control = GetDlgItem(Window, 0x667);
    if (control) {
        int row = SendMessageA(control, 0x147, 0, 0);
        if (row != -1) {
            int entry = SendMessageA(control, 0x150, row, 0);
            if (entry != -1) {
                unknown_a3d3bc = entry;
                OptionsChanged = true;
                Changed = true;
            }
        }
    }
}


// Read_Color_0059aa80 (0x0059AA80)

void ModemHost::Read_Color_0059aa80()
{
    HWND control = GetDlgItem(Window, 0x405);
    if (control) {
        int color = SendMessageA(control, 0x147, 0, 0);
        if (color != -1) {
            unknown_a3d3b4 = color;
            OptionsChanged = true;
            Changed = true;
            SendMessageA(control, 0x498, 0, Player_Color_RGB(color));
        }
    }
}


// Read_Name_0059aaf0 (0x0059AAF0)

void ModemHost::Read_Name_0059aaf0(bool keep)
{
    HWND control = GetDlgItem(Window, 0x5C1);
    if (control) {
        unsigned short name[20];
        SendMessageA(control, 0x4B3, 20, (LPARAM)name);
        wcstrim(name);
        if (!keep) {
            SendMessageA(control, 0x4B2, 0, (LPARAM)name);
        }
        strcpy(unknown_name_a3d3a0, Wide_To_Narrow_006fa120(name));
        OptionsChanged = true;
        Changed = true;
    }
}


// Set_Guest_0059ab80 (0x0059AB80)
extern "C" int __cdecl _wcsicmp(unsigned short const *left, unsigned short const *right);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" int __cdecl swprintf(unsigned short *buffer, unsigned short const *format, ...);

void ModemHost::Set_Guest_0059ab80(unsigned short const *name, int color, int house)
{
    if (_wcsicmp(GuestName, name) || GuestColor != color || GuestHouse != house) {
        wcscpy(GuestName, name);
        GuestColor = color;
        GuestHouse = house;
        HWND control = GetDlgItem(Window, 0x668);
        if (control) {
            unsigned short text[80];
            swprintf(text, L"%s [%s]", name, HouseTypes[house]->UIName);
            SendMessageA(control, 0x4B2, 0, (LPARAM)text);
            SendMessageA(control, 0x498, 0, Player_Color_RGB(color));
        }
        Voc_Play(Rules->OptionsChanged, 1.0f, 0x2000, 0);
    }
    OptionsChanged = true;
}


// Read_Unit_Count_0059ac80 (0x0059AC80)

void ModemHost::Read_Unit_Count_0059ac80()
{
    HWND control = GetDlgItem(Window, 0x669);
    if (control) {
        unknown_session_a3d298.UnitCount = SendMessageA(control, 0x400, 0, 0);
        OptionsChanged = true;
    }
}


// Read_Tech_Level_0059acb0 (0x0059ACB0)

void ModemHost::Read_Tech_Level_0059acb0()
{
    HWND control = GetDlgItem(Window, 0x550);
    if (control) {
        unknown_house_setting_7d88d8 = SendMessageA(control, 0x400, 0, 0);
        OptionsChanged = true;
    }
}


// Read_Credits_0059ace0 (0x0059ACE0)

void ModemHost::Read_Credits_0059ace0()
{
    HWND control = GetDlgItem(Window, 0x402);
    if (control) {
        unknown_session_a3d298.unknown_24 = SendMessageA(control, 0x400, 0, 0);
        OptionsChanged = true;
    }
}


// Read_AI_Players_0059ad10 (0x0059AD10)

void ModemHost::Read_AI_Players_0059ad10()
{
    HWND control = GetDlgItem(Window, 0x552);
    if (control) {
        unknown_session_a3d298.AICount = SendMessageA(control, 0x400, 0, 0);
        OptionsChanged = true;
    }
}


// Read_Game_Speed_0059ad40 (0x0059AD40)

void ModemHost::Read_Game_Speed_0059ad40()
{
    HWND control = GetDlgItem(Window, 0x529);
    if (control) {
        unknown_session_a3d298.GameSpeed_30 = 6 - SendMessageA(control, 0x400, 0, 0);
        OptionsChanged = true;
    }
}


// Read_AI_Difficulty_0059ad80 (0x0059AD80)

void ModemHost::Read_AI_Difficulty_0059ad80()
{
    HWND control = GetDlgItem(Window, 0x553);
    if (control) {
        unknown_session_a3d298.unknown_difficulty_40 = SendMessageA(control, 0x400, 0, 0);
        OptionsChanged = true;
    }
}


// Read_MCV_Redeploy_0059adb0 (0x0059ADB0)

void ModemHost::Read_MCV_Redeploy_0059adb0()
{
    HWND control = GetDlgItem(Window, 0x66B);
    if (control) {
        int state = SendMessageA(control, 0xF0, 0, 0);
        bool checked = state == 1;
        unknown_session_a3d298.unknown_a8 = checked;
        OptionsChanged = true;
    }
}


// Read_Bases_0059adf0 (0x0059ADF0)

void ModemHost::Read_Bases_0059adf0()
{
    HWND control = GetDlgItem(Window, 0x66A);
    if (control) {
        int state = SendMessageA(control, 0xF0, 0, 0);
        bool checked = state == 1;
        unknown_session_a3d298.unknown_bases_20 = checked;
        if (!unknown_session_a3d298.unknown_bases_20) {
            if (GetDlgItem(Window, 0x54E)) {
                unknown_session_a3d298.unknown_2a = false;
                SendDlgItemMessageA(Window, 0x54E, 0xF1, 0, 0);
            }
        }
        OptionsChanged = true;
    }
}


// Read_Crates_0059ae60 (0x0059AE60)

void ModemHost::Read_Crates_0059ae60()
{
    HWND control = GetDlgItem(Window, 0x54C);
    if (control) {
        int state = SendMessageA(control, 0xF0, 0, 0);
        bool checked = state == 1;
        unknown_session_a3d298.unknown_crates_29 = checked;
        OptionsChanged = true;
    }
}


// Read_Fog_Of_War_0059aea0 (0x0059AEA0)

void ModemHost::Read_Fog_Of_War_0059aea0()
{
    HWND control = GetDlgItem(Window, 0x54D);
    if (control) {
        int state = SendMessageA(control, 0xF0, 0, 0);
        bool checked = state == 1;
        unknown_session_a3d298.unknown_fog_a7 = checked;
        OptionsChanged = true;
    }
}


// Read_Bridge_Destroy_0059aee0 (0x0059AEE0)

void ModemHost::Read_Bridge_Destroy_0059aee0()
{
    HWND control = GetDlgItem(Window, 0x624);
    if (control) {
        int state = SendMessageA(control, 0xF0, 0, 0);
        bool checked = state == 1;
        unknown_session_a3d298.unknown_28 = checked;
        OptionsChanged = true;
    }
}


// Read_Harvester_Truce_0059af20 (0x0059AF20)

void ModemHost::Read_Harvester_Truce_0059af20()
{
    HWND control = GetDlgItem(Window, 0x596);
    if (control) {
        int state = SendMessageA(control, 0xF0, 0, 0);
        bool checked = state == 1;
        unknown_session_a3d298.unknown_a5 = checked;
        OptionsChanged = true;
    }
}


// Read_Short_Game_0059af60 (0x0059AF60)

void ModemHost::Read_Short_Game_0059af60()
{
    HWND control = GetDlgItem(Window, 0x54E);
    if (control) {
        int state = SendMessageA(control, 0xF0, 0, 0);
        bool checked = state == 1;
        unknown_session_a3d298.unknown_2a = checked;
        OptionsChanged = true;
        if (unknown_session_a3d298.unknown_2a) {
            if (GetDlgItem(Window, 0x66A)) {
                unknown_session_a3d298.unknown_bases_20 = true;
                SendDlgItemMessageA(Window, 0x66A, 0xF1, 1, 0);
            }
        }
    }
}


// Read_unknown_2b_0059afd0 (0x0059AFD0)

void ModemHost::Read_unknown_2b_0059afd0()
{
    HWND control = GetDlgItem(Window, 0x69A);
    if (control) {
        int state = SendMessageA(control, 0xF0, 0, 0);
        bool checked = state == 1;
        unknown_session_a3d298.unknown_2b = checked;
        OptionsChanged = true;
    }
}


// Read_Short_Game_0059b010 (0x0059B010)

void ModemHost::Read_Short_Game_0059b010()
{
    HWND control = GetDlgItem(Window, 0x69D);
    if (control) {
        int state = SendMessageA(control, 0xF0, 0, 0);
        bool checked = state == 1;
        unknown_session_a3d298.unknown_2a = checked;
        OptionsChanged = true;
    }
}


// Read_Crap_Engineers_0059b050 (0x0059B050)

void ModemHost::Read_Crap_Engineers_0059b050()
{
    HWND control = GetDlgItem(Window, 0x551);
    if (control) {
        int state = SendMessageA(control, 0xF0, 0, 0);
        bool checked = state == 1;
        unknown_session_a3d298.unknown_34 = checked;
        OptionsChanged = true;
    }
}


// void ModemHost::Select_Multi_Map_0059b090() (0x0059B090)
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);   // 0x00783689

void ModemHost::Select_Multi_Map_0059b090()
{
    Debug_Printf("ModemHost.SelectMultiMap(enter)\n");
    int scenario = unknown_session_a3d298.ScenarioIndex;
    char file_name[132];
    strcpy(file_name, unknown_session_a3d298.ScenarioFileName_658);
    unsigned short map_name[130];
    wcscpy(map_name, unknown_map_name_a3d342);
    ShowWindow(Window, 0);
    if (Open_Lobby_Dialog_005c6960(MainWindow) == 2) {
        unknown_session_a3d298.ScenarioIndex = scenario;
        Select_Scenario_005c7c30(scenario);
        Unknown_005c7570(Window);
        ShowWindow(Window, 5);
        if (_stricmp(unknown_session_a3d298.Scenarios[unknown_session_a3d298.ScenarioIndex]->unknown_58, "RandMap.Sed") == 0) {
            if (unknown_dialog_preview_a72ce8 != 0) {
                delete unknown_dialog_preview_a72ce8;
            }
            unknown_dialog_preview_a72ce8 = new MapPreviewClass;
            unknown_dialog_preview_a72ce8->Read_PCX_Preview("RandMap.img");
            if (unknown_dialog_preview_a72ce8->unknown_00 == 0) {
                Unknown_005c7570(Window);
            }
            InvalidateRect(Window, 0, false);
        } else {
            Unknown_005c7570(Window);
        }
        InvalidateRect(Window, 0, false);
    } else {
        ShowWindow(Window, 5);
        if (Select_Scenario_005c7c30(unknown_session_a3d298.ScenarioIndex) == true) {
            Send_Dlg_Item_Message(Window, 0x5A8, 0x4B2, 0, (LPARAM)unknown_map_name_a3d342);
            if (_stricmp(unknown_session_a3d298.Scenarios[unknown_session_a3d298.ScenarioIndex]->unknown_58, "RandMap.Sed") == 0) {
                if (unknown_dialog_preview_a72ce8 != 0) {
                    delete unknown_dialog_preview_a72ce8;
                    unknown_dialog_preview_a72ce8 = new MapPreviewClass;
                    unknown_dialog_preview_a72ce8->Read_PCX_Preview("RandMap.img");
                }
                if (unknown_dialog_preview_a72ce8->unknown_00 == 0) {
                    Unknown_005c7570(Window);
                }
                InvalidateRect(Window, 0, false);
            } else {
                Unknown_005c7570(Window);
            }
        } else {
            unknown_session_a3d298.ScenarioIndex = scenario;
        }
    }
    OptionsChanged = true;
    Debug_Printf("ModemHost.SelectMultiMap(exit)\n");
}


// void ModemHost::Select_Random_Map_0059b2c0() (0x0059B2C0)
// The random map button: runs the random map dialog with the modem idle callback; after OK saves the seed
// options as RandMap.Sed, drops the lobby preview, shows the new description, adds a random map entry (or
// gives the existing one the new digest and description), selects it, and reloads the preview from
// RandMap.img when the scenario file exists. Its exit trace reuses SelectMultiMap's text.

int __fastcall Do_Random_Map_Dialog_00577490(bool (*callback)());
extern bool unknown_host_shown_7e6dc0;

void ModemHost::Select_Random_Map_0059b2c0()
{
    Debug_Printf("ModemHost.SelectRandomMap(enter)\n");
    unknown_host_shown_7e6dc0 = false;
    if (Do_Random_Map_Dialog_00577490(Unknown_0059c880) == 1) {
        unknown_host_shown_7e6dc0 = true;
        RandomMapGen.SeedData.Save_Or_Prompt_00578ed0("RandMap.Sed");
        if (unknown_dialog_preview_a72ce8 != 0) {
            delete unknown_dialog_preview_a72ce8;
            unknown_dialog_preview_a72ce8 = 0;
        }
        Send_Dlg_Item_Message(Window, 0x5A8, 0x4B2, 0, (LPARAM)RandomMapGen.SeedData.DescriptionBuffer);
        bool found = false;
        int index;
        for (index = 0; index < unknown_session_a3d298.Scenarios.Count(); index++) {
            if (_stricmp(unknown_session_a3d298.Scenarios[index]->unknown_58, "RandMap.Sed") == 0) {
                found = true;
                break;
            }
        }
        char *crc = Random_Map_CRC_005c8420();
        if (!found) {
            MultiMission *mission = new MultiMission("RandMap.Sed", RandomMapGen.SeedData.DescriptionBuffer, crc, 1, 0, 2, 4);
            unknown_session_a3d298.Scenarios.Add(mission);
        } else {
            unknown_session_a3d298.Scenarios[index]->Set_Digest(crc);
            unknown_session_a3d298.Scenarios[index]->Set_Description(RandomMapGen.SeedData.DescriptionBuffer);
        }
        delete crc;
        if (Select_Scenario_005c7c30(index) == true) {
            unknown_session_a3d298.ScenarioIndex = index;
            strcpy(unknown_session_a3d298.ScenarioFileName_658, "RandMap.Sed");
            strcpy(unknown_id_source_a3d290->FileName, unknown_session_a3d298.ScenarioFileName_658);
            if (CCFileClass(unknown_session_a3d298.ScenarioFileName_658).Is_Available()) {
                unknown_dialog_preview_a72ce8 = new MapPreviewClass;
                unknown_dialog_preview_a72ce8->Read_PCX_Preview("RandMap.img");
                PostMessageA(Window, 0xF, 0, 0);
            }
        }
        OptionsChanged = true;
    }
    unknown_host_shown_7e6dc0 = true;
    Debug_Printf("ModemHost.SelectMultiMap(exit)\n");
}


// ModemHost::Service_0059b560 (0x0059B560)

void ModemHost::Service_0059b560(bool force)
{
    serial_manager_00a40a28.Service();
    if ((OptionsChanged == true && TickCount.Time() - OptionsTime > 30) || force == true) {
        Send_Options_0059bc90();
        OptionsTime = TickCount.Time();
        OptionsChanged = false;
    }
    if (TickCount.Time() - TimingTime > 40) {
        UnknownSerialPacket packet;
        memset(&packet, 0, sizeof(packet));
        packet.Command = 0x69;
        packet.Timing.ResponseTime = serial_manager_00a40a28.Response_Time();
        packet.ID = serial_local_player_00a3fb38;
        serial_manager_00a40a28.Send_Message(&packet, sizeof(packet), 0);
        TimingTime = TickCount.Time();
    }
}


// ModemHost::Process_Messages_0059b690 (0x0059B690)
// @ref? ??_C@_13ECNF@?$AA?5?$AA?$AA@ hits 007a0190 007a01f0 007a0250 007a02b0 007a0310

bool ModemHost::Process_Messages_0059b690(bool quiet)
{
    UnknownSerialPacket packet;
    int length;
    if (serial_manager_00a40a28.Get_Message(&packet, &length) > 0) {
        ReceiveTime = TickCount.Time();
        ReceiveTimeout = 1200;
        if (packet.Command >= 0x64 && packet.Command < 0x6D && packet.Command != 0x68
            && packet.ID == serial_local_player_00a3fb38) {
            Debug_Printf("Received our own packet back\n");
            if (!quiet) {
                WWMessageBox(L" ").Process(StringTable::Load_String("TXT_SYSTEM_NOT_RESPONDING", 0, "D:\\Ra2\\ModemHst.cpp", 0x7E2),
                    StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\ModemHst.cpp", 0x7E2), 0, 0, 0);
            }
            return true;
        }
        switch (packet.Command) {
        case 0x66: {
            Debug_Printf("Received SignOff!\n");
            unsigned long start = TickCount.Time();
            while (TickCount.Time() - start < 60) {
                Service_Game_IO_00481fb0();
                serial_manager_00a40a28.Service();
            }
            if (!quiet) {
                WWMessageBox(L" ").Process(StringTable::Load_String("TXT_USER_SIGNED_OFF", 0, "D:\\Ra2\\ModemHst.cpp", 0x7F6),
                    StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\ModemHst.cpp", 0x7F6), 0, 0, 0);
            }
            ReceiveTime = TickCount.Time();
            Shutdown_Modem();
            return true;
        }
        case 0x65: {
            Debug_Printf("Received game options\n");
            int version = VerNum.Clip_Version(packet.Options.MinVersion, packet.Options.MaxVersion);
            if (version == 0) {
                if (!quiet) {
                    WWMessageBox(L" ").Process(StringTable::Load_String("TXT_DESTGAME_OUTDATED", 0, "D:\\Ra2\\ModemHst.cpp", 0x80B),
                        StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\ModemHst.cpp", 0x80B), 0, 0, 0);
                }
                ReceiveTime = TickCount.Time();
                return true;
            }
            if (version == -1) {
                if (!quiet) {
                    WWMessageBox(L" ").Process(StringTable::Load_String("TXT_YOURGAME_OUTDATED", 0, "D:\\Ra2\\ModemHst.cpp", 0x814),
                        StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\ModemHst.cpp", 0x814), 0, 0, 0);
                }
                ReceiveTime = TickCount.Time();
                return true;
            }
            bool match = true;
            if (packet.Options.Checksum != unknown_54) {
                match = false;
            }
            if (packet.Options.INIID != unknown_5c) {
                match = false;
            }
            if (packet.Options.ArtID != unknown_58) {
                match = false;
            }
            if (!match) {
                if (!quiet) {
                    WWMessageBox(L" ").Process(StringTable::Load_String("TXT_MISMATCH", 0, "D:\\Ra2\\ModemHst.cpp", 0x851),
                        StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\ModemHst.cpp", 0x851), 0, 0, 0);
                }
                ReceiveTime = TickCount.Time();
                return true;
            }
            unknown_session_a3d2ac = VerNum.Version_Protocol(version);
            unsigned short *guest = Narrow_To_Wide_006fa180(packet.Name);
            Set_Guest_0059ab80(guest, packet.Options.Color, packet.Options.House);
            unknown_20 = true;
            break;
        }
        case 0x74:
            Debug_Printf("Received Accept Options\n");
            EnableWindow(GetDlgItem(Window, 1), 1);
            break;
        case 0x68: {
            Debug_Printf("Received serial message\n");
            unsigned short *name = Narrow_To_Wide_006fa180(packet.Name);
            Show_Chat_0059c0b0(name, packet.ID, packet.Message);
            if (!quiet) {
                Voc_Play(Rules->IncomingMessage, 1.0f, 0x2000, 0);
            }
            break;
        }
        case 0x69:
            Debug_Printf("Received timing packet\n");
            GuestResponseTime = packet.Timing.ResponseTime;
            if (!unknown_20) {
                OptionsChanged = true;
            }
            break;
        case 0x6A:
            Debug_Printf("Received score screen packet\n");
            break;
        case 0x77:
            Send_Preview_0059c8b0();
            ReceiveTime = TickCount.Time();
            break;
        default:
            Debug_Printf("Received packet\n");
            break;
        }
    }
    unsigned long elapsed = TickCount.Time() - ReceiveTime;
    if (elapsed > ReceiveTimeout) {
        Debug_Printf("Timeout! %d ticks since last incoming message\n", elapsed);
        if (!quiet) {
            WWMessageBox(L" ").Process(StringTable::Load_String("TXT_SYSTEM_NOT_RESPONDING", 0, "D:\\Ra2\\ModemHst.cpp", 0x895),
                StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\ModemHst.cpp", 0x895), 0, 0, 0);
        }
        return true;
    }
    return false;
}


extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" char *__cdecl strncpy(char *dest, char const *source, unsigned int count);

void ModemHost::Send_Options_0059bc90()
{
    Debug_Printf("Sending game options packet\n");
    EnableWindow(GetDlgItem(Window, 1), 0);
    UnknownSerialPacket packet;
    memset(&packet, 0, sizeof(packet));
    packet.Command = 0x65;
    packet.Options.Checksum = unknown_54;
    packet.Options.ArtID = unknown_58;
    packet.Options.INIID = unknown_5c;
    packet.Options.Version = Unknown_00737af0();
    packet.Options.MinVersion = VerNum.Min_Version();
    packet.Options.MaxVersion = VerNum.Max_Version();
    packet.Options.Seed = Seed;
    packet.Options.Special = unknown_special_a40920;
    packet.Options.GameSpeed = unknown_session_a3d298.GameSpeed_30;
    packet.Options.unknown_2a = unknown_session_a3d298.unknown_2a;
    packet.Options.unknown_2b = unknown_session_a3d298.unknown_2b;
    packet.Options.unknown_2c = unknown_session_a3d298.unknown_2c;
    packet.Options.unknown_34 = unknown_session_a3d298.unknown_34;
    packet.ID = serial_local_player_00a3fb38;
    strcpy(packet.Name, unknown_name_a3d3a0);
    packet.Options.House = unknown_a3d3bc;
    packet.Options.Color = unknown_a3d3b4;
    packet.Options.Credits = unknown_session_a3d298.unknown_24;
    packet.Options.Bases = unknown_session_a3d298.unknown_bases_20;
    packet.Options.unknown_a8 = unknown_session_a3d298.unknown_a8;
    packet.Options.unknown_28 = unknown_session_a3d298.unknown_28;
    packet.Options.Crates = unknown_session_a3d298.unknown_crates_29;
    packet.Options.unknown_a5 = unknown_session_a3d298.unknown_a5;
    packet.Options.FogOfWar = unknown_session_a3d298.unknown_fog_a7;
    packet.Options.AICount = unknown_session_a3d298.AICount;
    packet.Options.AIDifficulty = unknown_session_a3d298.unknown_difficulty_40;
    packet.Options.TechLevel = unknown_house_setting_7d88d8;
    packet.Options.UnitCount = unknown_session_a3d298.UnitCount;
    wcscpy(packet.Options.MapName, unknown_session_a3d298.Scenarios[unknown_session_a3d298.ScenarioIndex]->Description);
    CCFileClass file(unknown_session_a3d298.Scenarios[unknown_session_a3d298.ScenarioIndex]->Get_Filename());
    packet.Options.FileLength = file.Size();
    strncpy(packet.Options.FileName, unknown_session_a3d298.Scenarios[unknown_session_a3d298.ScenarioIndex]->Get_Filename(), 13);
    strncpy(packet.Options.Digest, unknown_session_a3d298.Scenarios[unknown_session_a3d298.ScenarioIndex]->Get_Digest(), 0x20);
    packet.Options.IsOfficial = unknown_session_a3d298.Scenarios[unknown_session_a3d298.ScenarioIndex]->Get_Official();
    serial_manager_00a40a28.Send_Message(&packet, sizeof(packet), 1);
}


// ModemHost::Send_Chat_0059bfa0 (0x0059BFA0)
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" int __cdecl swprintf(unsigned short *buffer, unsigned short const *format, ...);


void ModemHost::Send_Chat_0059bfa0(unsigned short const *text)
{
    if (text) {
        UnknownSerialPacket packet;
        memset(&packet, 0, sizeof(packet));
        packet.Command = 0x68;
        strcpy(packet.Name, unknown_name_a3d3a0);
        packet.ID = unknown_a3d3b4;
        wcscpy(packet.Message, text);
        serial_manager_00a40a28.Send_Message(&packet, sizeof(packet), 1);
        serial_manager_00a40a28.Service();
        unsigned short *name = Narrow_To_Wide_006fa180(unknown_name_a3d3a0);
        Show_Chat_0059c0b0(name, unknown_a3d3b4, text);
    }
}


// ModemHost::Show_Chat_0059c0b0 (0x0059C0B0)
extern "C" int __cdecl swprintf(unsigned short *buffer, unsigned short const *format, ...);

void ModemHost::Show_Chat_0059c0b0(unsigned short const *name, int color, unsigned short const *text)
{
    if (text) {
        HWND list = GetDlgItem(Window, 0x53F);
        if (list) {
            int rgb = Player_Color_RGB(color);
            unsigned short line[256];
            swprintf(line, L"[%s] %s", name, text);
            _DrawMessage_005c2da0(rgb, line, list);
        }
    }
}


// ModemHost::Sign_Off_0059c130 (0x0059C130)

void ModemHost::Sign_Off_0059c130()
{
    UnknownSerialPacket packet;
    memset(&packet, 0, sizeof(packet));
    packet.Command = 0x66;
    packet.ScenarioInfo.Color = unknown_a3d3b4;
    packet.ID = serial_local_player_00a3fb38;
    serial_manager_00a40a28.Send_Message(&packet, sizeof(packet), 1);
    HWND dialog = unknown_cancel_dialog_6001e0(
        StringTable::Load_String("TXT_SIGNING_OFF", 0, "D:\\Ra2\\ModemHst.cpp", 0x95E), 0, 0);
    if (dialog) {
        unknown_dialog_5ff7c0(dialog);
    }
    unsigned long start = TickCount.Time();
    while (serial_manager_00a40a28.Num_Send() && TickCount.Time() - start < 300) {
        UnknownSerialPacket reply;
        int length;
        if (serial_manager_00a40a28.Get_Message(&reply, &length) > 0 && reply.Command == 0x66
            && reply.ID == serial_local_player_00a3fb38) {
            break;
        }
        Service_Game_IO_00481fb0();
        Unknown_005B5EE0();
        serial_manager_00a40a28.Service();
    }
    Shutdown_Modem();
    if (dialog) {
        unknown_dialog_5ff6e0(dialog);
    }
}


extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);

bool ModemHost::Start_Game_0059c290(bool load_game)
{
    Debug_Printf("ModemHost.StartGame(enter)\n");
    Debug_Printf("LoadGame = %s\n", load_game == true ? "true" : "false");
    if (load_game == true) {
        return false;
    }
    unknown_id_source_a3d290->TechLevel = unknown_session_a3d298.ScenarioIndex;
    strcpy(unknown_id_source_a3d290->FileName, unknown_session_a3d298.Scenarios[unknown_session_a3d298.ScenarioIndex]->unknown_58);
    unknown_id_source_a3d290->unknown_610 = 1;
    unknown_id_source_a3d290->Difficulty = 1;
    unknown_session_a3d55c = 0;
    NodeNameType *node = new NodeNameType;
    if (node) {
        wcscpy(node->Name, Narrow_To_Wide_006fa180(unknown_name_a3d3a0));
        node->unknown_4b = unknown_a3d3bc;
        node->unknown_53 = unknown_a3d3b4;
        node->unknown_63 = -1;
        unknown_players_a3fa7c.Add(node);
    }
    node = new NodeNameType;
    if (node) {
        wcscpy(node->Name, GuestName);
        node->unknown_4b = GuestHouse;
        node->unknown_53 = GuestColor;
        node->unknown_63 = -1;
        unknown_players_a3fa7c.Add(node);
    }
    if (unknown_players_a3fa7c.Count() != 2) {
        return false;
    }
    UnknownSerialPacket packet;
    memset(&packet, 0, sizeof(packet));
    packet.Command = 0x67;
    packet.Timing.ResponseTime = serial_manager_00a40a28.Response_Time();
    if (GuestResponseTime != 10000 && packet.Timing.ResponseTime < GuestResponseTime) {
        packet.Timing.ResponseTime = GuestResponseTime;
    }
    unknown_a40b18 = unknown_session_a3d298.GameSpeed_30;
    unknown_session_a3d298.unknown_2cc = 5;
    unknown_a3fba4 = 0;
    unknown_session_a3d298.unknown_2e4 = 0;
    unknown_session_a3d298.unknown_2e8 = 0;
    if (unknown_session_a3d2ac == 2) {
        unsigned int ahead = ((packet.Timing.ResponseTime >> 3) + 4) / 5 * 5;
        if (ahead > 10) {
            unknown_session_a3d298.MaxAhead_2c8 = ahead;
        } else {
            unknown_session_a3d298.MaxAhead_2c8 = 10;
        }
    } else {
        unknown_session_a3d298.MaxAhead_2c8 = packet.Timing.ResponseTime >> 3;
        if ((int)unknown_session_a3d298.MaxAhead_2c8 <= 5) {
            unknown_session_a3d298.MaxAhead_2c8 = 5;
        }
    }
    packet.ID = serial_local_player_00a3fb38;
    serial_manager_00a40a28.Send_Message(&packet, sizeof(packet), 1);
    HWND dialog = unknown_cancel_dialog_6001e0(
        StringTable::Load_String("TXT_SENDING_GO", 0, "D:\\Ra2\\ModemHst.cpp", 0x9EC), 0, 0);
    if (dialog) {
        unknown_dialog_5ff7c0(dialog);
    }
    unsigned long start = TickCount.Time();
    while ((serial_manager_00a40a28.Num_Send() && TickCount.Time() - start < 1800) || TickCount.Time() - start < 60) {
        Service_Game_IO_00481fb0();
        Unknown_005B5EE0();
        serial_manager_00a40a28.Service();
    }
    if (dialog) {
        unknown_set_dialog_item_5b0(dialog, (LPARAM)StringTable::Load_String("TXT_WAITING_GO", 0, "D:\\Ra2\\ModemHst.cpp", 0x9FD));
    }
    Debug_Printf("Waiting for 'GO' response.\n");
    start = TickCount.Time();
    do {
        Service_Game_IO_00481fb0();
        Unknown_005B5EE0();
        serial_manager_00a40a28.Service();
        UnknownSerialPacket reply;
        int length;
        if (serial_manager_00a40a28.Get_Message(&reply, &length) > 0) {
            Debug_Printf("Received message\n");
            if (reply.Command == 0x72) {
                Debug_Printf("Received GO message\n");
                break;
            }
            if (reply.Command == 0x73) {
                WWMessageBox(L" ").Process(StringTable::Load_String("TXT_NO_EXPANSION_SCENARIO", 0, "D:\\Ra2\\ModemHst.cpp", 0xA16),
                    StringTable::Load_String("TXT_CANCEL", 0, "D:\\Ra2\\ModemHst.cpp", 0xA16), 0, 0, 0);
                ReceiveTime = TickCount.Time();
                Debug_Printf("***Guest doesn't have scenario!\n");
                return false;
            }
            if (reply.Command == 0x6E) {
                Debug_Printf("About to call 'Send_Remote_File'.\n");
                Send_Remote_File_0066bb20(unknown_id_source_a3d290->FileName, 1, 0, 0);
                break;
            }
        }
    } while (TickCount.Time() - start < 1200);
    if (dialog) {
        unknown_dialog_5ff6e0(dialog);
    }
    serial_manager_00a40a28.Init_Send_Queue();
    unknown_session_a3f616 = 0;
    unknown_id_source_a3d290->TechLevel = unknown_session_a3d298.ScenarioIndex;
    unknown_session_a3d55c = unknown_players_a3fa7c.Count();
    Frame = 0;
    unknown_session_a3d2ac = 2;
    unknown_special_a40920.DestroyableBridges = unknown_session_a3d298.unknown_28;
    unknown_special_a40920.HarvesterImmune = unknown_session_a3d298.unknown_a5;
    unknown_special_a40920.TiberiumGrows = true;
    unknown_special_a40920.TiberiumSpreads = true;
    unknown_id_source_a3d290->Special().FixedAlliance = false;
    unknown_session_a3d298.unknown_a4 = true;
    unknown_special_a40920.Unknown_0068B410();
    Debug_Printf("ModemHost.StartGame(exit)\n");
    return true;
}


// ModemHost::Idle_0059c880 (0x0059C880)

bool ModemHost::Idle_0059c880()
{
    if (unknown_modem_host_a70de4) {
        unknown_modem_host_a70de4->Service_0059b560(false);
        unknown_modem_host_a70de4->Process_Messages_0059b690(true);
    }
    Service_Game_IO_00481fb0();
    return false;
}


// ModemHost::Send_Preview_0059c8b0 (0x0059C8B0)
void ModemHost::Send_Preview_0059c8b0()
{
    CDTimerClass<SystemTimerClass> timer;
    if (Compare_Narrow_00781f20(unknown_session_a3d298.ScenarioFileName_658, "RandMap.Sed")) {
        return;
    }
    Debug_Printf("Starting map preview upload\n");
    UnknownSerialPacket packet;
    memset(&packet, 0, sizeof(packet));
    packet.Command = 0x75;
    serial_manager_00a40a28.Send_Message(&packet, sizeof(packet), 1);
    timer = 600;
    while (serial_manager_00a40a28.Num_Send() > 0 && timer) {
        Service_Game_IO_00481fb0();
    }
    timer = 1800;
    bool acked = false;
    do {
        Service_Game_IO_00481fb0();
        UnknownSerialPacket reply;
        int length;
        if (serial_manager_00a40a28.Get_Message(&reply, &length) > 0 && reply.Command == 0x76) {
            acked = true;
            break;
        }
    } while (timer);
    if (!acked) {
        Debug_Printf("Failed to get SERIAL_PREVIEW_ACK\n");
        return;
    }
    int size = 0;
    unsigned short *image = unknown_dialog_preview_a72ce8->Create_Paletted_Preview(0x40, &size);
    Debug_Printf("Preview size is %d bytes\n", size);
    char *data = new char[size * 4];
    BufferPipe pipe(data, size * 4);
    LZOPipe lzo(LZOPipe::COMPRESS, 0x2000);
    lzo.Put_To(&pipe);
    int length = lzo.Put(image, size);
    length += lzo.End();
    Debug_Printf("Compressed preview image is %d bytes\n", length);
    RawFileClass file("Preview.bin");
    if (file.Is_Available()) {
        file.Delete();
    }
    file.Open(2);
    file.Write(&size, 4);
    file.Write(data, length);
    file.Close();
    delete data;
    Debug_Printf("Calling Send_Remote_File to send the preview\n");
    Send_Remote_File_0066bb20("Preview.bin", 1, 0, 0);
}


extern "C" void __cdecl release_59cb80(void*);
class Leaf_59cb80 { public: void cleanup();void* get(unsigned int flags); };
void* Leaf_59cb80::get(unsigned int flags) { cleanup(); if (flags & 1) release_59cb80(this); return this; }
