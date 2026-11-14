// modemgst.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/modemgst.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/modem_guest.hpp"
#include "shared/queue.hpp"
#include "shared/serial_progress.hpp"
#include "shared/trigger_type.hpp"
#include "shared/lobby.hpp"
#include "shared/house_type.hpp"
#include "shared/rules.hpp"
#include "shared/sound_play.hpp"
#include "shared/ms_message_box.hpp"
#include "shared/comm_buffer.hpp"
#include "shared/null_dlg.hpp"
#include "shared/string_table.hpp"
#include "shared/keyboard.hpp"
#include "shared/map_preview.hpp"
#include "shared/options_dialog.hpp"
#include "shared/session_state.hpp"
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#include "shared/null_modem.hpp"
#include "shared/unknown_player_entry.hpp"
#include "shared/campaign.hpp"
#include "shared/ww_message_box.hpp"
#include "shared/id_source.hpp"
#include "shared/special.hpp"
#include "shared/multi_mission.hpp"
#define VECTOR_IMPLEMENT_DYNAMIC_ADD
#include "shared/cd_timer.hpp"
#include "shared/raw_file_class.hpp"
#include "shared/buffer_straw.hpp"
#include "shared/lzo_straw.hpp"


// Header-static dynamic initializers of the unit at 0x00595E20: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x00595E20-0x00596001); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x00595E40, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_00595e20.cpp; no source of its own.


// Dynamic initializer (0x00595E70, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_00595e20.cpp; no source of its own.


// Dynamic initializer (0x00595E90, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_00595e20.cpp; no source of its own.


// Dynamic initializer (0x00595EB0, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_00595e20.cpp; no source of its own.


// Dynamic initializer (0x00595ED0, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_00595e20.cpp; no source of its own.


// Dynamic initializer (0x00595EF0, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_00595e20.cpp; no source of its own.


// Dynamic initializer (0x00595F10, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_00595e20.cpp; no source of its own.


// Dynamic initializer (0x00595F50, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_00595e20.cpp; no source of its own.


// Dynamic initializer (0x00595F80, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_00595e20.cpp; no source of its own.


// Dynamic initializer (0x00595FB0, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_00595e20.cpp; no source of its own.


// Dynamic initializer (0x00595FE0, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_00595e20.cpp; no source of its own.


// Dynamic initializer (0x00595FF0, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_00595e20.cpp; no source of its own.


// ModemGuest::Dialog_00596010 (0x00596010)
bool ModemGuest::Dialog_00596010()
{
    bool result = false;
    Debug_Printf("ModemGuest.Dialog(enter)\n");
    Result = -1;
    Window = unknown_create_dialog_5ff610(0xF2, Dialog_Proc_00596170, 0);
    if (Window) {
        SetWindowLongA(Window, 8, (LONG)this);
        Init_Dialog_00596540();
        unknown_dialog_5ff7c0(Window);
        Changed = false;
        OptionsChanged = true;
        OptionsTime = TickCount.Time();
        HostResponseTime = 10000;
        ReceiveTime = TickCount.Time();
        ReceiveTimeout = 1200;
        TimingTime = 0;
        HostName[0] = 0;
        HostColor = -1;
        HostHouse = -1;
        unknown_50 = Unknown_006525F0();
        Read_Art_INI();
        unknown_54 = unknown_art_ini_unique_id();
        unknown_58 = unknown_ini_8397a0_unique_id();
        while (Result < 0) {
            if (unknown_6000d0() == true) {
                break;
            }
            Service_00597550(false);
            if (Process_Messages_00597680() == true) {
                break;
            }
            unknown_517370(false);
        }
        unknown_dialog_5ff6e0(Window);
        if (Changed == true) {
            unknown_session_a3d298.Write_MultiPlayer_Settings();
        }
        if (Result == 1) {
            result = true;
        } else if (Result == 2) {
            result = false;
            Sign_Off_00598590();
        }
    }
    Debug_Printf("ModemGuest.Dialog(exit %s)\n", result == true ? "true" : "false");
    return result;
}


// ModemGuest::Dialog_Proc_00596170 (0x00596170)

extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" int __cdecl swprintf(unsigned short *buffer, unsigned short const *format, ...);
extern unsigned short const unknown_empty_wide_839d4c[];

int __stdcall ModemGuest::Dialog_Proc_00596170(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    int result = unknown_dialog_proc_5ffb00(window, message, wparam, lparam);
    if (result) {
        return result;
    }
    ModemGuest *guest = (ModemGuest *)GetWindowLongA(window, 8);
    switch (message) {
    case 0x111:
        switch ((unsigned short)wparam) {
        case 1:
            if ((unsigned short)(wparam >> 16) == 0) {
                guest->Send_Ready_005974f0();
            }
            break;
        case 2:
            if ((unsigned short)(wparam >> 16) == 0) {
                guest->Read_Dialog_00596d50();
                guest->Result = 2;
            }
            break;
        case 0x405:
            if ((unsigned short)(wparam >> 16) == 1) {
                guest->Read_Color_00597010();
            }
            break;
        case 0x540:
            if ((unsigned short)(wparam >> 16) == 0x501) {
                guest->Send_Chat_00596e30();
            }
            break;
        case 0x5C1:
            if ((unsigned short)(wparam >> 16) == 0x501 || (unsigned short)(wparam >> 16) == 0x300) {
                guest->Read_Name_005970b0((unsigned short)(wparam >> 16) == 0x300);
            }
            break;
        case 0x667:
            if ((unsigned short)(wparam >> 16) == 1) {
                guest->Read_Color_Entry_00596fc0();
            }
            break;
        }
        break;
    case 0xF:
        if (unknown_dialog_preview_a72ce8) {
            unknown_dialog_preview_a72ce8->Blit_Preview(window);
        }
        ValidateRect(window, 0);
        break;
    case 0x2:
        if (unknown_dialog_preview_a72ce8) {
            delete unknown_dialog_preview_a72ce8;
            unknown_dialog_preview_a72ce8 = 0;
        }
        break;
    }
    return 0;
}


// void ModemGuest::Init_Dialog_00596540() -- 0x00596540
// Purpose: fills the modem guest lobby dialog from the session (name, colour, side, unit count, tech level,
// credits, AI players, speed, difficulty and the option check boxes), logging each value, then resets the
// preview, shows any received modem string as chat and clears the player list and map name.
extern char unknown_a3f616;
extern unsigned short const unknown_empty_wide_839d4c[];

// Every SendMessageA call of Init_Dialog goes through this file-local inline (code-free; it sets the
// scratch/callee-saved register choice of `this` versus the GetDlgItem import, as in Fill_Port 0x66D5A0).
static inline LRESULT Send_Message(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    return SendMessageA(window, message, wparam, lparam);
}

void ModemGuest::Init_Dialog_00596540()
{
    unsigned short text[1024];
    Debug_Printf("ModemGuest.InitDialog(enter)\n");
    HWND control = GetDlgItem(Window, 0x5C1);
    if (control) {
        Send_Message(control, 0xC5, 19, 0);
        Send_Message(control, 0x4B2, 0, (LPARAM)Narrow_To_Wide_006fa180(unknown_name_a3d3a0));
    }
    Debug_Printf("Name: %ls\n", unknown_name_a3d3a0);
    control = GetDlgItem(Window, 0x405);
    if (control) {
        Send_Message(control, 0x14B, 0, 0);
        Send_Message(control, 0x4C1, -1, (LPARAM)StringTable::Load_String("TXT_GOLD", 0, "D:\\Ra2\\ModemGst.cpp", 0x118));
        Send_Message(control, 0x4C1, -1, (LPARAM)StringTable::Load_String("TXT_RED", 0, "D:\\Ra2\\ModemGst.cpp", 0x119));
        Send_Message(control, 0x4C1, -1, (LPARAM)StringTable::Load_String("TXT_BLUE", 0, "D:\\Ra2\\ModemGst.cpp", 0x11A));
        Send_Message(control, 0x4C1, -1, (LPARAM)StringTable::Load_String("TXT_GREEN", 0, "D:\\Ra2\\ModemGst.cpp", 0x11B));
        Send_Message(control, 0x4C1, -1, (LPARAM)StringTable::Load_String("TXT_ORANGE", 0, "D:\\Ra2\\ModemGst.cpp", 0x11C));
        Send_Message(control, 0x4C1, -1, (LPARAM)StringTable::Load_String("TXT_SKY_BLUE", 0, "D:\\Ra2\\ModemGst.cpp", 0x11D));
        Send_Message(control, 0x4C1, -1, (LPARAM)StringTable::Load_String("TXT_PURPLE", 0, "D:\\Ra2\\ModemGst.cpp", 0x11E));
        Send_Message(control, 0x4C1, -1, (LPARAM)StringTable::Load_String("TXT_PINK", 0, "D:\\Ra2\\ModemGst.cpp", 0x11F));
        for (int index = 0; index < 8; index++) {
            Send_Message(control, 0x498, index, unknown_color_values_7e6d94[index]);
        }
        unknown_a3d3b4 = unknown_session_a3d298.unknown_12c;
        Send_Message(control, 0x14E, unknown_a3d3b4, 0);
    }
    Debug_Printf("Color: %d\n", unknown_a3d3b4);
    control = GetDlgItem(Window, 0x667);
    if (control) {
        Send_Message(control, 0x14B, 0, 0);
        for (int house = 0; house < HouseTypes.Count(); house++) {
            if (HouseTypes[house]->Multiplay) {
                LPARAM name = (LPARAM)HouseTypes[house]->UIName;
                int row = Send_Message(control, 0x4C2, 0, name);
                Send_Message(control, 0x151, row, house);
            }
        }
        int row = Find_Combo_Item_Data_00736e10(control, -1, unknown_a3d3bc);
        Send_Message(control, 0x14E, row, 0);
    }
    control = GetDlgItem(Window, 0x669);
    if (control) {
        Send_Message(control, 0x406, 1, ((unsigned long)(unsigned short)Rules->MaxUnitCount << 16) | (unsigned short)Rules->MinUnitCount);
        Send_Message(control, 0x405, 1, unknown_session_a3d298.UnitCount);
    }
    Debug_Printf("UnitCount: %d\n", unknown_session_a3d298.UnitCount);
    control = GetDlgItem(Window, 0x550);
    if (control) {
        Send_Message(control, 0x406, 1, 0xA0001);
        Send_Message(control, 0x405, 1, unknown_house_setting_7d88d8);
    }
    Debug_Printf("TechLevel: %d\n", unknown_house_setting_7d88d8);
    control = GetDlgItem(Window, 0x402);
    if (control) {
        Send_Message(control, 0x406, 1, ((unsigned long)(unsigned short)Rules->MaxMoney << 16) | (unsigned short)Rules->MinMoney);
        Send_Message(control, 0x405, 1, unknown_session_a3d298.unknown_24);
    }
    Debug_Printf("Credits: %d\n", unknown_session_a3d298.unknown_24);
    control = GetDlgItem(Window, 0x552);
    if (control) {
        Send_Message(control, 0x406, 1, 0x60000);
        Send_Message(control, 0x405, 1, unknown_session_a3d298.AICount);
    }
    Debug_Printf("AIPlayers: %d\n", unknown_session_a3d298.AICount);
    control = GetDlgItem(Window, 0x529);
    if (control) {
        Send_Message(control, 0x406, 1, 0x60000);
        Send_Message(control, 0x405, 1, 6 - unknown_session_a3d298.GameSpeed_30);
    }
    Debug_Printf("Game Speed: %d\n", unknown_session_a3d298.GameSpeed_30);
    control = GetDlgItem(Window, 0x553);
    if (control) {
        Send_Message(control, 0x406, 1, 0x20000);
        Send_Message(control, 0x405, 1, unknown_session_a3d298.unknown_difficulty_40);
    }
    Debug_Printf("AIDifficulty: %d\n", unknown_session_a3d298.unknown_difficulty_40);
    control = GetDlgItem(Window, 0x66B);
    if (control) {
        Send_Message(control, 0xF1, unknown_session_a3d298.unknown_a8 == true, 0);
    }
    Debug_Printf("MCVRedeploy: %s\n", unknown_session_a3d298.unknown_a8 == true ? "ON" : "OFF");
    control = GetDlgItem(Window, 0x66A);
    if (control) {
        Send_Message(control, 0xF1, unknown_session_a3d298.unknown_bases_20 == true, 0);
    }
    Debug_Printf("Bases: %s\n", unknown_session_a3d298.unknown_bases_20 == true ? "ON" : "OFF");
    control = GetDlgItem(Window, 0x54E);
    if (control) {
        Send_Message(control, 0xF1, unknown_session_a3d298.unknown_2a == true, 0);
    }
    Debug_Printf("Short Game: %s\n", unknown_session_a3d298.unknown_2a == true ? "ON" : "OFF");
    control = GetDlgItem(Window, 0x69A);
    if (control) {
        Send_Message(control, 0xF1, unknown_session_a3d298.unknown_2b == true, 0);
    }
    Debug_Printf("SuperWeapons: %s\n", unknown_session_a3d298.unknown_2b == true ? "ON" : "OFF");
    control = GetDlgItem(Window, 0x69D);
    if (control) {
        Send_Message(control, 0xF1, unknown_session_a3d298.unknown_2c == true, 0);
    }
    Debug_Printf("BuildOffAlly: %s\n", unknown_session_a3d298.unknown_2c == true ? "ON" : "OFF");
    control = GetDlgItem(Window, 0x551);
    if (control) {
        Send_Message(control, 0xF1, unknown_session_a3d298.unknown_34 == true, 0);
    }
    Debug_Printf("Crap Engineers: %s\n", unknown_session_a3d298.unknown_34 == true ? "ON" : "OFF");
    control = GetDlgItem(Window, 0x54C);
    if (control) {
        Send_Message(control, 0xF1, unknown_session_a3d298.unknown_crates_29 == true, 0);
    }
    Debug_Printf("Crates: %s\n", unknown_session_a3d298.unknown_crates_29 == true ? "ON" : "OFF");
    control = GetDlgItem(Window, 0x54D);
    if (control) {
        Send_Message(control, 0xF1, unknown_session_a3d298.unknown_fog_a7 == true, 0);
    }
    Debug_Printf("FogOfWar: %s\n", unknown_session_a3d298.unknown_fog_a7 == true ? "ON" : "OFF");
    control = GetDlgItem(Window, 0x624);
    if (control) {
        Send_Message(control, 0xF1, unknown_session_a3d298.unknown_28 == true, 0);
    }
    Debug_Printf("BridgeDestroy: %s\n", unknown_session_a3d298.unknown_28 == true ? "ON" : "OFF");
    control = GetDlgItem(Window, 0x596);
    if (control) {
        Send_Message(control, 0xF1, unknown_session_a3d298.unknown_a5 == true, 0);
    }
    Debug_Printf("Harvester Truce: %s\n", unknown_session_a3d298.unknown_a5 == true ? "ON" : "OFF");
    VerNum.Init_Clipping();
    if (unknown_dialog_preview_a72ce8) {
        delete unknown_dialog_preview_a72ce8;
        unknown_dialog_preview_a72ce8 = 0;
    }
    if (strlen(ModemRXString) > 78) {
        ModemRXString[79] = 0;
    }
    if (strlen(ModemRXString)) {
        Char_To_Wide_006fa0c0(text, ModemRXString, -1);
        Show_Chat_00598510(text, -1, unknown_empty_wide_839d4c);
    }
    ModemRXString[0] = 0;
    serial_manager_00a40a28.Reset_Response_Time(true);
    unknown_a3f616 = 1;
    Free_Pointers_005a7480(&unknown_players_a3fa7c);
    unknown_map_name_a3d342[0] = 0;
    Debug_Printf("ModemGuest.InitDialog(exit)\n");
}


// ModemGuest::Read_Dialog_00596d50 (0x00596D50)
void ModemGuest::Read_Dialog_00596d50()
{
    unsigned short name[20];
    Debug_Printf("ModemGuest.ReadDialog(enter)\n");
    HWND control = GetDlgItem(Window, 0x5C1);
    if (control) {
        SendMessageA(control, 0x4B3, 20, (LPARAM)name);
        WideToCharString_006fa0f0(unknown_name_a3d3a0, name, -1);
    }
    Debug_Printf("Name: %s\n", unknown_name_a3d3a0);
    control = GetDlgItem(Window, 0x405);
    if (control) {
        unknown_a3d3b4 = SendMessageA(control, 0x147, 0, 0);
    }
    Debug_Printf("Color: %d\n", unknown_a3d3b4);
    control = GetDlgItem(Window, 0x667);
    if (control) {
        int selection = SendMessageA(control, 0x147, 0, 0);
        unknown_a3d3bc = SendMessageA(control, 0x150, selection, 0);
    }
    Debug_Printf("ModemGuest.ReadDialog(exit)\n");
}


// ModemGuest::Send_Chat_00596e30 (0x00596E30)
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" int __cdecl swprintf(unsigned short *buffer, unsigned short const *format, ...);
extern unsigned short const unknown_empty_wide_839d4c[];

void ModemGuest::Send_Chat_00596e30()
{
    unsigned short text[256];
    HWND control = GetDlgItem(Window, 0x540);
    if (control) {
        SendMessageA(control, 0x4B3, 256, (LPARAM)text);
        SendMessageA(control, 0x4B2, 0, (LPARAM)unknown_empty_wide_839d4c);
        if (wcslen(text) && text[0] != 0x0D) {
            wcstrim(text);
            Send_Chat_00598400(text);
        }
    }
}


// ModemGuest::Read_Color_Entry_00596fc0 (0x00596FC0)
void ModemGuest::Read_Color_Entry_00596fc0()
{
    HWND control = GetDlgItem(Window, 0x667);
    if (control) {
        int row = SendMessageA(control, 0x147, 0, 0);
        if (row != -1) {
            unknown_a3d3bc = SendMessageA(control, 0x150, row, 0);
            OptionsChanged = true;
            Changed = true;
        }
    }
}


// ModemGuest::Read_Color_00597010 (0x00597010)
void ModemGuest::Read_Color_00597010()
{
    HWND control = GetDlgItem(Window, 0x405);
    if (control) {
        int color = SendMessageA(control, 0x147, 0, 0);
        if (color != -1) {
            if (color == HostColor) {
                color = HostColor + 1;
                if (color > 7) {
                    color = 0;
                }
                SendMessageA(control, 0x14E, color, 0);
            }
            unknown_a3d3b4 = color;
            OptionsChanged = true;
            Changed = true;
            int rgb;
            if (color >= 0 && color < 8) {
                rgb = unknown_color_values_7e6d94[color];
            } else {
                rgb = unknown_color_7a737c;
            }
            SendMessageA(control, 0x498, 0, rgb);
        }
    }
}


// ModemGuest::Read_Name_005970b0 (0x005970B0)
void ModemGuest::Read_Name_005970b0(bool keep)
{
    HWND control = GetDlgItem(Window, 0x5C1);
    if (control) {
        unsigned short name[21];
        SendMessageA(control, 0x4B3, 21, (LPARAM)name);
        wcstrim(name);
        if (!keep) {
            SendMessageA(control, 0x4B2, 0, (LPARAM)name);
        }
        WideToCharString_006fa0f0(unknown_name_a3d3a0, name, -1);
        OptionsChanged = true;
        Changed = true;
    }
}


// ModemGuest::Set_Host_00597130 (0x00597130)
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" int __cdecl swprintf(unsigned short *buffer, unsigned short const *format, ...);

void ModemGuest::Set_Host_00597130(unsigned short const *name, int color, int house)
{
    wcscpy(HostName, name);
    HostColor = color;
    HWND control = GetDlgItem(Window, 0x405);
    if (control) {
        int selection = SendMessageA(control, 0x147, 0, 0);
        if (selection != -1 && selection == HostColor) {
            Read_Color_00597010();
        }
    }
    HostHouse = house;
    control = GetDlgItem(Window, 0x668);
    if (control) {
        unsigned short text[80];
        swprintf(text, L"%s [%s]", name, HouseTypes[house]->UIName);
        SendMessageA(control, 0x4B2, 0, (LPARAM)text);
        SendMessageA(control, 0x498, 0, Player_Color_RGB(color));
    }
}


// ModemGuest::Show_Options_00597220 (0x00597220)
void ModemGuest::Show_Options_00597220()
{
    Debug_Printf("Settings change\n");
    HWND control = GetDlgItem(Window, 0x669);
    if (control) {
        SendMessageA(control, 0x405, 1, unknown_session_a3d298.UnitCount);
    }
    control = GetDlgItem(Window, 0x550);
    if (control) {
        SendMessageA(control, 0x405, 1, unknown_house_setting_7d88d8);
    }
    control = GetDlgItem(Window, 0x402);
    if (control) {
        SendMessageA(control, 0x405, 1, unknown_session_a3d298.unknown_24);
    }
    control = GetDlgItem(Window, 0x552);
    if (control) {
        SendMessageA(control, 0x405, 1, unknown_session_a3d298.AICount);
    }
    control = GetDlgItem(Window, 0x529);
    if (control) {
        SendMessageA(control, 0x405, 1, 6 - unknown_session_a3d298.GameSpeed_30);
    }
    control = GetDlgItem(Window, 0x553);
    if (control) {
        SendMessageA(control, 0x405, 1, unknown_session_a3d298.unknown_difficulty_40);
    }
    control = GetDlgItem(Window, 0x66B);
    if (control) {
        SendMessageA(control, 0xF1, unknown_session_a3d298.unknown_a8 == true, 0);
    }
    control = GetDlgItem(Window, 0x66A);
    if (control) {
        SendMessageA(control, 0xF1, unknown_session_a3d298.unknown_bases_20 == true, 0);
    }
    control = GetDlgItem(Window, 0x551);
    if (control) {
        SendMessageA(control, 0xF1, unknown_session_a3d298.unknown_34 == true, 0);
    }
    control = GetDlgItem(Window, 0x54E);
    if (control) {
        SendMessageA(control, 0xF1, unknown_session_a3d298.unknown_2a == true, 0);
    }
    control = GetDlgItem(Window, 0x69A);
    if (control) {
        SendMessageA(control, 0xF1, unknown_session_a3d298.unknown_2b == true, 0);
    }
    control = GetDlgItem(Window, 0x69D);
    if (control) {
        SendMessageA(control, 0xF1, unknown_session_a3d298.unknown_2c == true, 0);
    }
    control = GetDlgItem(Window, 0x54C);
    if (control) {
        SendMessageA(control, 0xF1, unknown_session_a3d298.unknown_crates_29 == true, 0);
    }
    control = GetDlgItem(Window, 0x54D);
    if (control) {
        SendMessageA(control, 0xF1, unknown_session_a3d298.unknown_fog_a7 == true, 0);
    }
    control = GetDlgItem(Window, 0x624);
    if (control) {
        SendMessageA(control, 0xF1, unknown_session_a3d298.unknown_28 == true, 0);
    }
    control = GetDlgItem(Window, 0x596);
    if (control) {
        SendMessageA(control, 0xF1, unknown_session_a3d298.unknown_a5 == true, 0);
    }
    control = GetDlgItem(Window, 0x5A8);
    if (control) {
        SendMessageA(control, 0x4B2, 0, (LPARAM)unknown_map_name_a3d342);
    }
    if (!unknown_dialog_preview_a72ce8 || !unknown_dialog_preview_a72ce8->Is_Present()) {
        Unknown_005c7570(Window);
    }
    control = GetDlgItem(Window, 1);
    if (control) {
        EnableWindow(control, 1);
    }
    Voc_Play(Rules->OptionsChanged, 1.0f, 0x2000, 0);
}


// ModemGuest::Send_Ready_005974f0 (0x005974F0)
void ModemGuest::Send_Ready_005974f0()
{
    HWND button = GetDlgItem(Window, 1);
    if (button) {
        EnableWindow(button, 0);
    }
    UnknownSerialPacket packet;
    memset(&packet, 0, sizeof(packet));
    packet.Command = 0x74;
    serial_manager_00a40a28.Send_Message(&packet, sizeof(packet), 1);
}


// ModemGuest::Service_00597550 (0x00597550)
void ModemGuest::Service_00597550(bool force)
{
    serial_manager_00a40a28.Service();
    if ((OptionsChanged == true && TickCount.Time() - OptionsTime > 30) || force == true) {
        Send_Options_00598330();
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


// ModemGuest::Process_Messages_00597680 (0x00597680)

bool ModemGuest::Process_Messages_00597680()
{
    UnknownSerialPacket packet;
    int length;
    serial_manager_00a40a28.Service();
    if (serial_manager_00a40a28.Get_Message(&packet, &length) > 0) {
        ReceiveTime = TickCount.Time();
        ReceiveTimeout = 1200;
        if (packet.Command >= 0x64 && packet.Command < 0x6D && packet.Command != 0x68
            && packet.ID == serial_local_player_00a3fb38) {
            Debug_Printf("Received own packet back\n");
            WWMessageBox(L" ").Process(StringTable::Load_String("TXT_SYSTEM_NOT_RESPONDING", 0, "D:\\Ra2\\ModemGst.cpp", 0x499),
                StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\ModemGst.cpp", 0x499), 0, 0, 0);
            return true;
        }
        switch (packet.Command) {
        case 0x66: {
            Debug_Printf("Received SignOff\n");
            unsigned long start = TickCount.Time();
            while (TickCount.Time() - start < 60) {
                Service_Game_IO_00481fb0();
                serial_manager_00a40a28.Service();
            }
            WWMessageBox(L" ").Process(StringTable::Load_String("TXT_USER_SIGNED_OFF", 0, "D:\\Ra2\\ModemGst.cpp", 0x4A9),
                StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\ModemGst.cpp", 0x4A9), 0, 0, 0);
            ReceiveTime = TickCount.Time();
            Shutdown_Modem();
            return true;
        }
        case 0x65: {
            Debug_Printf("Received game options\n");
            int version = VerNum.Clip_Version(packet.Options.MinVersion, packet.Options.MaxVersion);
            if (version == 0) {
                WWMessageBox(L" ").Process(StringTable::Load_String("TXT_DESTGAME_OUTDATED", 0, "D:\\Ra2\\ModemGst.cpp", 0x4B9),
                    StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\ModemGst.cpp", 0x4B9), 0, 0, 0);
                ReceiveTime = TickCount.Time();
                return true;
            }
            if (version == -1) {
                WWMessageBox(L" ").Process(StringTable::Load_String("TXT_YOURGAME_OUTDATED", 0, "D:\\Ra2\\ModemGst.cpp", 0x4BF),
                    StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\ModemGst.cpp", 0x4BF), 0, 0, 0);
                ReceiveTime = TickCount.Time();
                return true;
            }
            bool match = true;
            if (packet.Options.Checksum != unknown_50) {
                match = false;
            }
            if (packet.Options.INIID != unknown_58) {
                match = false;
            }
            if (packet.Options.ArtID != unknown_54) {
                match = false;
            }
            if (!match) {
                WWMessageBox(L" ").Process(StringTable::Load_String("TXT_MISMATCH", 0, "D:\\Ra2\\ModemGst.cpp", 0x4EE),
                    StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\ModemGst.cpp", 0x4EE), 0, 0, 0);
                ReceiveTime = TickCount.Time();
                return true;
            }
            unknown_session_a3d2ac = VerNum.Version_Protocol(version);
            unsigned short *host = Narrow_To_Wide_006fa180(packet.Name);
            Set_Host_00597130(host, packet.Options.Color, packet.Options.House);
            if (Receive_Options_00597e50(&packet) == true) {
                Show_Options_00597220();
            }
            break;
        }
        case 0x67:
        case 0x6B:
            Debug_Printf("Received go\n");
            if (Find_Local_Scenario_0066c5b0(unknown_session_a3d298.ScenarioFileName_658, unknown_session_a3d298.ScenarioFileLength_87c, unknown_session_a3d298.ScenarioDigest_85a, unknown_session_a3d298.ScenarioIsOfficial_880, true) == true) {
                UnknownSerialPacket reply;
                memset(&reply, 0, sizeof(reply));
                reply.Command = 0x72;
                serial_manager_00a40a28.Send_Message(&reply, sizeof(reply), 1);
            } else if (unknown_session_a3d298.ScenarioIsOfficial_880 && Compare_Narrow_00781f20(unknown_session_a3d298.ScenarioFileName_658, "RandMap.Sed")) {
                UnknownSerialPacket reply;
                memset(&reply, 0, sizeof(reply));
                reply.Command = 0x73;
                serial_manager_00a40a28.Send_Message(&reply, sizeof(reply), 1);
                Service_Game_IO_00481fb0();
                serial_manager_00a40a28.Service();
                WWMessageBox(L" ").Process(StringTable::Load_String("TXT_UNABLE_PLAY_WAAUGH", 0, "D:\\Ra2\\ModemGst.cpp", 0x51C),
                    StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\ModemGst.cpp", 0x51C), 0, 0, 0);
                ReceiveTime = TickCount.Time();
                return true;
            } else {
                if (!Get_File_From_Host_0066aef0(unknown_session_a3d298.ScenarioFileName_658, 1, false)) {
                    return true;
                }
                ReceiveTime = TickCount.Time();
            }
            strcpy(unknown_id_source_a3d290->FileName, unknown_session_a3d298.ScenarioFileName_658);
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
            if (Start_Game_005986f0(packet.Command == 0x6B) == true) {
                Result = 1;
            } else {
                Result = 2;
            }
            break;
        case 0x68: {
            Debug_Printf("Received serial message\n");
            unsigned short *name = Narrow_To_Wide_006fa180(packet.Name);
            Show_Chat_00598510(name, packet.ID, packet.Message);
            Voc_Play(Rules->IncomingMessage, 1.0f, 0x2000, 0);
            break;
        }
        case 0x69:
            Debug_Printf("Received timing packet\n");
            break;
        case 0x6A:
            Debug_Printf("Received score screen packet\n");
            OptionsChanged = true;
            OptionsTime = TickCount.Time();
            break;
        case 0x75:
            Debug_Printf("Received SERIAL_PREVIEW_MODE packet\n");
            Receive_Preview_005989b0();
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
        WWMessageBox(L" ").Process(StringTable::Load_String("TXT_SYSTEM_NOT_RESPONDING", 0, "D:\\Ra2\\ModemGst.cpp", 0x56F),
            StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\ModemGst.cpp", 0x56F), 0, 0, 0);
        return true;
    }
    return false;
}


// ModemGuest::Receive_Options_00597e50 (0x00597E50)
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);

bool ModemGuest::Receive_Options_00597e50(UnknownSerialPacket const *packet)
{
    if (unknown_session_a3d298.unknown_bases_20 == (bool)packet->Options.Bases
        && unknown_session_a3d298.unknown_a8 == (bool)packet->Options.unknown_a8
        && unknown_session_a3d298.unknown_a5 == (bool)packet->Options.unknown_a5
        && unknown_session_a3d298.unknown_28 == (bool)packet->Options.unknown_28
        && unknown_session_a3d298.unknown_crates_29 == (bool)packet->Options.Crates
        && unknown_session_a3d298.unknown_fog_a7 == (bool)packet->Options.FogOfWar
        && unknown_session_a3d298.AICount == packet->Options.AICount
        && unknown_session_a3d298.unknown_difficulty_40 == packet->Options.AIDifficulty
        && unknown_house_setting_7d88d8 == packet->Options.TechLevel
        && unknown_session_a3d298.unknown_24 == packet->Options.Credits
        && unknown_session_a3d298.UnitCount == packet->Options.UnitCount
        && Seed == packet->Options.Seed
        && unknown_special_a40920.Equals_Options(packet->Options.Special)
        && unknown_session_a3d298.GameSpeed_30 == packet->Options.GameSpeed
        && unknown_session_a3d298.unknown_2a == (bool)packet->Options.unknown_2a
        && unknown_session_a3d298.unknown_2b == (bool)packet->Options.unknown_2b
        && unknown_session_a3d298.unknown_2c == (bool)packet->Options.unknown_2c
        && unknown_session_a3d298.unknown_34 == (bool)packet->Options.unknown_34
        && !Compare_Narrow_00781f20(unknown_session_a3d298.ScenarioFileName_658, packet->Options.FileName)
        && !strcmp(unknown_session_a3d298.ScenarioDigest_85a, packet->Options.Digest)
        && unknown_session_a3d298.ScenarioFileLength_87c == packet->Options.FileLength) {
        return false;
    }
    Debug_Printf("Decoding game options\n");
    unknown_session_a3d298.unknown_bases_20 = packet->Options.Bases;
    unknown_session_a3d298.unknown_a8 = packet->Options.unknown_a8;
    unknown_session_a3d298.unknown_28 = packet->Options.unknown_28;
    unknown_special_a40920.DestroyableBridges = packet->Options.unknown_28;
    unknown_session_a3d298.unknown_crates_29 = packet->Options.Crates;
    unknown_session_a3d298.unknown_fog_a7 = packet->Options.FogOfWar;
    unknown_session_a3d298.unknown_a5 = packet->Options.unknown_a5;
    unknown_session_a3d298.AICount = packet->Options.AICount;
    unknown_session_a3d298.unknown_difficulty_40 = packet->Options.AIDifficulty;
    unknown_house_setting_7d88d8 = packet->Options.TechLevel;
    unknown_session_a3d298.unknown_24 = packet->Options.Credits;
    unknown_session_a3d298.UnitCount = packet->Options.UnitCount;
    Seed = packet->Options.Seed;
    unknown_special_a40920 = packet->Options.Special;
    unknown_a40b18 = unknown_session_a3d298.GameSpeed_30 = packet->Options.GameSpeed;
    unknown_session_a3d298.unknown_2a = packet->Options.unknown_2a;
    unknown_session_a3d298.unknown_2b = packet->Options.unknown_2b;
    unknown_session_a3d298.unknown_2c = packet->Options.unknown_2c;
    unknown_session_a3d298.unknown_34 = packet->Options.unknown_34;
    unknown_special_a40920.TiberiumGrows = true;
    unknown_special_a40920.TiberiumSpreads = true;
    unknown_id_source_a3d290->Special().FixedAlliance = false;
    unknown_session_a3d298.unknown_a4 = true;
    unknown_special_a40920.Unknown_0068B410();
    if (Compare_Narrow_00781f20(unknown_session_a3d298.ScenarioFileName_658, packet->Options.FileName)
        || Compare_Narrow_00781f20(unknown_session_a3d298.ScenarioDigest_85a, packet->Options.Digest)
        || unknown_session_a3d298.ScenarioIsOfficial_880 != (bool)packet->Options.IsOfficial
        || unknown_session_a3d298.ScenarioFileLength_87c != packet->Options.FileLength
        || !unknown_map_name_a3d342[0]) {
        Debug_Printf("Scenario has changed\n");
        strcpy(unknown_session_a3d298.ScenarioFileName_658, packet->Options.FileName);
        strcpy(unknown_session_a3d298.ScenarioDigest_85a, packet->Options.Digest);
        unknown_session_a3d298.ScenarioIsOfficial_880 = packet->Options.IsOfficial;
        unknown_session_a3d298.ScenarioFileLength_87c = packet->Options.FileLength;
        bool found = false;
        if (strcmp(unknown_session_a3d298.ScenarioFileName_658, "RandMap.Sed") || !unknown_session_a3d298.ScenarioIsOfficial_880) {
            for (int index = 0; index < unknown_multi_missions_a3d8d8.Count(); index++) {
                MultiMission *mission = unknown_multi_missions_a3d8d8[index];
                if (!Compare_Narrow_00781f20(unknown_session_a3d298.ScenarioFileName_658, mission->unknown_58)) {
                    wcscpy(unknown_map_name_a3d342, mission->Description);
                    found = true;
                    break;
                }
            }
        }
        if (!found) {
            wcscpy(unknown_map_name_a3d342, packet->Options.MapName);
        }
        if (unknown_dialog_preview_a72ce8) {
            delete unknown_dialog_preview_a72ce8;
            unknown_dialog_preview_a72ce8 = 0;
        }
    }
    return true;
}


// ModemGuest::Send_Options_00598330 (0x00598330)
void ModemGuest::Send_Options_00598330()
{
    UnknownSerialPacket packet;
    memset(&packet, 0, sizeof(packet));
    packet.Command = 0x65;
    *(int *)&packet.unknown_19[0x86 - 0x19] = unknown_50;
    *(int *)&packet.unknown_19[0xD4 - 0x19] = unknown_54;
    *(int *)&packet.unknown_19[0xD0 - 0x19] = unknown_58;
    *(unsigned long *)&packet.unknown_19[0xD8 - 0x19] = Unknown_00737af0();
    *(unsigned long *)&packet.unknown_19[0x1E - 0x19] = VerNum.Min_Version();
    *(unsigned long *)&packet.unknown_19[0x22 - 0x19] = VerNum.Max_Version();
    packet.ID = serial_local_player_00a3fb38;
    strcpy(packet.Name, unknown_name_a3d3a0);
    *(int *)&packet.unknown_19[0] = unknown_a3d3bc;
    packet.unknown_19[0x1D - 0x19] = unknown_a3d3b4;
    serial_manager_00a40a28.Send_Message(&packet, sizeof(packet), 1);
}


// ModemGuest::Send_Chat_00598400 (0x00598400)
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" int __cdecl swprintf(unsigned short *buffer, unsigned short const *format, ...);

void ModemGuest::Send_Chat_00598400(unsigned short const *text)
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
        Show_Chat_00598510(name, unknown_a3d3b4, text);
    }
}


// ModemGuest::Show_Chat_00598510 (0x00598510)
extern "C" int __cdecl swprintf(unsigned short *buffer, unsigned short const *format, ...);

void ModemGuest::Show_Chat_00598510(unsigned short const *name, int color, unsigned short const *text)
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


// ModemGuest::Sign_Off_00598590 (0x00598590)
void ModemGuest::Sign_Off_00598590()
{
    UnknownSerialPacket packet;
    memset(&packet, 0, sizeof(packet));
    packet.Command = 0x66;
    packet.ScenarioInfo.Color = unknown_a3d3b4;
    packet.ID = serial_local_player_00a3fb38;
    serial_manager_00a40a28.Send_Message(&packet, sizeof(packet), 1);
    HWND dialog = unknown_cancel_dialog_6001e0(
        StringTable::Load_String("TXT_SIGNING_OFF", 0, "D:\\Ra2\\ModemGst.cpp", 0x693), 0, 0);
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


// Complete guest start handshake and install both player entries (0x005986F0).
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);

bool ModemGuest::Start_Game_005986f0(bool unknown)
{
    if (unknown == true) {
        return false;
    }
    Read_Dialog_00596d50();
    unknown_session_a3d55c = 0;
    NodeNameType *node = new NodeNameType;
    if (node) {
        Char_To_Wide_006fa0c0(node->Name, unknown_name_a3d3a0, -1);
        node->unknown_4b = unknown_a3d3bc;
        node->unknown_53 = unknown_a3d3b4;
        node->unknown_63 = -1;
        unknown_players_a3fa7c.Add(node);
        unknown_session_a3d55c++;
    }
    node = new NodeNameType;
    if (node) {
        wcscpy(node->Name, HostName);
        node->unknown_4b = HostHouse;
        node->unknown_53 = HostColor;
        node->unknown_63 = -1;
        unknown_players_a3fa7c.Add(node);
        unknown_session_a3d55c++;
    }
    if (unknown_session_a3d55c != 2) {
        return false;
    }
    HWND dialog = unknown_cancel_dialog_6001e0(
        StringTable::Load_String("TXT_SENDING_GO_ACK", 0, "D:\\Ra2\\ModemGst.cpp", 0x6F3), 0, 0);
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
        unknown_dialog_5ff6e0(dialog);
    }
    serial_manager_00a40a28.Init_Send_Queue();
    unknown_session_a3f616 = 0;
    int techLevel = unknown_session_a3d298.ScenarioIndex;
    unknown_id_source_a3d290->TechLevel = techLevel;
    unknown_session_a3d55c = unknown_players_a3fa7c.Count();
    Frame = 0;
    unknown_session_a3d2ac = 2;
    unknown_special_a40920.DestroyableBridges = unknown_session_a3d298.unknown_28;
    unknown_special_a40920.HarvesterImmune = unknown_session_a3d298.unknown_a5;
    unknown_special_a40920.TiberiumGrows = true;
    unknown_special_a40920.TiberiumSpreads = true;
    unknown_special_a40920.Unknown_0068B410();
    return true;
}


// ModemGuest::Receive_Preview_005989b0 (0x005989B0)

void ModemGuest::Receive_Preview_005989b0()
{
    CDTimerClass<SystemTimerClass> timer;
    Debug_Printf("Starting map preview download\n");
    UnknownSerialPacket packet;
    memset(&packet, 0, sizeof(packet));
    packet.Command = 0x76;
    Debug_Printf("Sending preview mode ack\n");
    serial_manager_00a40a28.Send_Message(&packet, sizeof(packet), 1);
    timer = 600;
    while (serial_manager_00a40a28.Num_Send() > 0 && timer) {
        Service_Game_IO_00481fb0();
    }
    Debug_Printf("Preview mode ack sent\n");
    Debug_Printf("Calling Get_File_From_Host to receive the file download\n");
    char filename[256];
    if (!Get_File_From_Host_0066aef0(filename, 1, false)) {
        Debug_Printf("got_file is false. Download failed\n");
        return;
    }
    Debug_Printf("Loading the compressed preview image\n");
    RawFileClass file(filename);
    int size = file.Size();
    char *data = new char[size];
    file.Read(data, size);
    int length = *(int *)data;
    Debug_Printf("Decompressing the preview image\n");
    BufferStraw straw(data + 4, size);
    LZOStraw lzo(LZOStraw::DECOMPRESS, 0x2000);
    lzo.Get_From(&straw);
    unsigned short *image = new unsigned short[length];
    lzo.Get(image, length);
    Debug_Printf("Creating the new preview surface\n");
    if (unknown_dialog_preview_a72ce8) {
        delete unknown_dialog_preview_a72ce8;
    }
    unknown_dialog_preview_a72ce8 = new MapPreviewClass;
    unknown_dialog_preview_a72ce8->Create_Preview_Surface(image);
    InvalidateRect(Window, 0, 0);
    Debug_Printf("Cleaning up the temporary decompression buffers\n");
    delete image;
    delete data;
}
