// netdlg2.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/netdlg2.json carries the addresses and the rows that
// stay in their own files.


#include "shared/lobby.hpp"
#include "shared/ipx_manager.hpp"
#include "shared/string_table.hpp"
#include "shared/lobby_name_set.hpp"
#include "shared/wol_ladder_list_cell.hpp"
#include "shared/pcx_cache.hpp"
#include "shared/gdlg_supp.hpp"
#include "shared/progress_screen.hpp"
#include "shared/winsock_interface.hpp"
#include "shared/comm_buffer.hpp"
#include "shared/debug_printf.hpp"
#include "shared/wol_sink.hpp"
#include "shared/ipx_address.hpp"
#include "shared/unknown_player_entry.hpp"
#include "shared/wide_string.hpp"
#include "shared/wstring.hpp"
#include "shared/session_state.hpp"
#include "shared/crt_stdio.hpp"
#include "shared/crt_memory.hpp"
#include "shared/win_util.hpp"
#include "shared/id_source.hpp"
#include "shared/multiplayer_game_mode.hpp"
#include "shared/map_preview.hpp"
#include "shared/cd_timer.hpp"
#include "shared/crt_string.hpp"
#include "shared/modem_host.hpp"
#include "shared/win32_window.hpp"
#include "shared/ww_message_box.hpp"
#include "shared/campaign_score.hpp"
#include "shared/wol_settings.hpp"
#include "shared/taunt_packet.hpp"
#include "shared/options_dialog.hpp"
#include "shared/rules.hpp"
#include "shared/sound_play.hpp"
#include "shared/system_timer.hpp"
#include "shared/netdlg.hpp"
#include "shared/skirmish.hpp"


// 0x005bbd10: function not yet identified
//
// Body: 13 bytes (frozen inventory bound).
// Referenced from 5 sites (data, unconditional call).
// Ghidra prototype: undefined1 FUN_005bbd10(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_005bbd10 {

extern void  Service_Game_IO_00481fb0(void);
extern void  Get_Join_Responses_005c08a0(void);

unsigned char  FUN_005bbd10(void)
{
    Service_Game_IO_00481fb0();
    Get_Join_Responses_005c08a0();
    return 0;
}

}  // namespace seed_005bbd10


// void Send_Slot_Names_005bbd20() (0x005BBD20)

void Send_Slot_Names_005bbd20()
{
    UnknownGlobalPacket packet;
    memset(&packet, 0, sizeof(packet));
    packet.Command = 0x16;
    strcpy(packet.Name, unknown_name_a3d3a0);
    Unknown_Slot_Names_005c8cf0(packet.Slots.Names);
    int count = unknown_players_a3fa7c.Count();
    for (int i = 1; i < count; i++) {
        unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 1, &unknown_players_a3fa7c[i]->Address, 0, 0);
        Service_Game_IO_00481fb0();
    }
}


// void __fastcall Unknown_005bbdc0() (0x005BBDC0)

void __fastcall Unknown_005bbdc0()
{
    Unknown_005bbdd0(false);
}


namespace OwnerDraw { int __fastcall unknown_5eab00(HWND control); }
namespace seed_005c8680 { bool __fastcall unknown_is_host_player_005c8680(int player); }
void __fastcall Save_List_Selection_005c5ff0(HWND list,LobbyNameSet *names);
void __fastcall Restore_List_Selection_005c6310(HWND list,LobbyNameSet *names);
extern int unknown_column_a71de8;
extern int unknown_column_a71e18;
extern int unknown_column_a71e28;
extern wchar_t const unknown_empty_wide_839d4c[];
static bool Find_Active_Game_Dialog(HWND &active)
{
    active=Unknown_00735B10();
    int dialog_id=Current_Dialog_Id_00735b20();
    if(dialog_id==0xbc || dialog_id==0xbd) {
        return true;
    }
    active=(HWND)unknown_previous_key((int)active);
    while(active) {
        int kind=OwnerDraw::unknown_5eab00(active);
        if(kind==0xbc || kind==0xbd) {
            return true;
        }
        active=(HWND)unknown_previous_key((int)active);
    }
    return false;
}

// Rebuild the LAN chat or game roster while preserving selection and scroll.
void __fastcall Unknown_005bbdd0(bool full)
{
    HWND active;
    if(Find_Active_Game_Dialog(active)) {
        Refresh_Slot_Settings_005ca000(active,full);
        return;
    }
    {
    HWND dialog=Unknown_00735B10();
    HWND list=GetDlgItem(dialog,0x53e);
    if(dialog && list) {
        HWND users=GetDlgItem(dialog,0x53e);
        WolLadderListCell cell;
        int top=SendDlgItemMessageA(dialog,0x53e,0x18e,0,0);
        SendDlgItemMessageA(dialog,0x53e,0x49d,0,1);
        LobbyNameSet selections(Hash_Wide_Name_005c2bf0);
        Save_List_Selection_005c5ff0(list,&selections);
        SendDlgItemMessageA(dialog,0x53e,0x184,0,0);
        if(unknown_game_index_a71e90==0) {
            SendMessageA(users,0x4cc,0,(LPARAM)Narrow_To_Wide_006fa180(unknown_name_a3d3a0));
            for(int chat=1;chat<unknown_chat_a3fa4c.Count();++chat)
                SendMessageA(users,0x4cc,chat,(LPARAM)unknown_chat_a3fa4c[chat]);
        } else {
            for(int player=0;player<unknown_players_a3fa7c.Count();++player) {
                int state=0;
                if(seed_005c8680::unknown_is_host_player_005c8680(player)) {
                    state=2;
                    unknown_players_a3fa7c[player]->unknown_67=1;
                } else if(unknown_players_a3fa7c[player]->unknown_67!=0) state=1;
                wchar_t info[128];
                wchar_t country_name[128];
                swprintf(info,L"%s",unknown_players_a3fa7c[player]);
                int country=unknown_players_a3fa7c[player]->unknown_66e290();
                Surface *flag=Country_Flag(country);
                switch(country) {
                case 0: swprintf(country_name,L"%s",StringTable::Load_String("TXT_GDI",0,"D:\\Ra2\\netdlg2.cpp",383)); break;
                case 1: swprintf(country_name,L"%s",StringTable::Load_String("TXT_GDI",0,"D:\\Ra2\\netdlg2.cpp",386)); break;
                case 2: swprintf(country_name,L"%s",StringTable::Load_String("TXT_GDI",0,"D:\\Ra2\\netdlg2.cpp",389)); break;
                case 3: swprintf(country_name,L"%s",StringTable::Load_String("TXT_GDI",0,"D:\\Ra2\\netdlg2.cpp",392)); break;
                case 4: swprintf(country_name,L"%s",StringTable::Load_String("TXT_GDI",0,"D:\\Ra2\\netdlg2.cpp",395)); break;
                case 5: swprintf(country_name,L"%s",StringTable::Load_String("TXT_NOD",0,"D:\\Ra2\\netdlg2.cpp",398)); break;
                case 6: swprintf(country_name,L"%s",StringTable::Load_String("TXT_NOD",0,"D:\\Ra2\\netdlg2.cpp",401)); break;
                case 7: swprintf(country_name,L"%s",StringTable::Load_String("TXT_NOD",0,"D:\\Ra2\\netdlg2.cpp",404)); break;
                case 8: swprintf(country_name,L"%s",StringTable::Load_String("TXT_NOD",0,"D:\\Ra2\\netdlg2.cpp",407)); break;
                }
                SendMessageA(users,0x4cc,-1,(LPARAM)info);
                int color=unknown_color_values_7e6d94[unknown_players_a3fa7c[player]->unknown_53];
                cell.Flags=4;
                cell.unknown_0c=color;
                cell.Secondary.Set(unknown_empty_wide_839d4c);
                SendDlgItemMessageA(dialog,0x53e,0x4a8,(unsigned short)unknown_column_a71de8|((unsigned long)(unsigned short)player<<16),(LPARAM)&cell);
                cell.Flags=2;
                cell.Secondary.Set(country_name);
                cell.unknown_10=(int)flag;
                SendDlgItemMessageA(dialog,0x53e,0x4a8,(unsigned short)unknown_column_a71e18|((unsigned long)(unsigned short)player<<16),(LPARAM)&cell);
                cell.Secondary.Set(unknown_empty_wide_839d4c);
                cell.Flags=2;
                if(state==2) cell.unknown_10=(int)unknown_bitmap_writer_a76378.Get_Image_0068ca40("wolhost.pcx",0);
                else if(state) cell.unknown_10=(int)unknown_bitmap_writer_a76378.Get_Image_0068ca40("wolacpt.pcx",0);
                else cell.Flags=0;
                SendDlgItemMessageA(dialog,0x53e,0x4a8,(unsigned short)unknown_column_a71e28|((unsigned long)(unsigned short)player<<16),(LPARAM)&cell);
            }
        }
        Restore_List_Selection_005c6310(list,&selections);
        SendDlgItemMessageA(dialog,0x53e,0x197,top,0);
        SendDlgItemMessageA(dialog,0x53e,0x49d,0,0);
        InvalidateRect(list,0,0);
        UpdateWindow(list);
    }
    }
    return;

}


// void __fastcall Reject_Join_005bc3e0(NodeNameType *node) (0x005BC3E0)

void __fastcall Reject_Join_005bc3e0(NodeNameType *node)
{
    memset(&unknown_gpacket_a3f640, 0, sizeof(unknown_gpacket_a3f640));
    unknown_gpacket_a3f640.Command = 8;
    unknown_gpacket_a3f640.Reject.Why = 4;
    unknown_ipx_manager_a40978.Send_Global_Message(&unknown_gpacket_a3f640, sizeof(unknown_gpacket_a3f640), 1, &node->Address, 0, 0);
}


// bool __fastcall Unknown_005bc430(int count, bool unknown_1, bool unknown_2) (0x005BC430)

bool __fastcall Unknown_005bc430(int count, bool unknown_1, bool unknown_2)
{
    HWND dialog = WS_Find_Dialog_00735940(0xBC);
    return Unknown_005cbdb0(dialog, count, unknown_1, unknown_2);
}


// bool Init_Network_005bc460() (0x005BC460)

bool Init_Network_005bc460()
{
    NetNumType net;
    NetNodeType node;

    if (!unknown_ipx_manager_a40978.Init()) {
        return false;
    }
    if (unknown_game_mode_a3d298 != 4 && unknown_dest_net_set_a3f604) {
        unknown_dest_net_a3f608.Get_Address(net, node);
        unknown_ipx_manager_a40978.Set_Bridge(net);
    }
    Init_Lobby_Entries_005c86e0();
    Clear_Slot_Lists_005c8a70();
    Unknown_Reset_Slots_005c8a90();
    return true;
}


// void Shutdown_Network_005bc4d0() (0x005BC4D0)

void Shutdown_Network_005bc4d0()
{
    Unknown_Free_Lobby_Entries_005c8950();
    unknown_host_name_a3f618[0] = 0;
    if (PacketTransport != 0) {
        delete PacketTransport;
    }
    PacketTransport = 0;
}


// void __fastcall Process_Timeouts_005bc500() (0x005BC500)

void __fastcall Process_Timeouts_005bc500()
{
    int i;

    for (i = 1; i < unknown_games_a3fa64.Count(); i++) {
        if (Compare_Wide_007837d3(unknown_games_a3fa64[i]->Name, unknown_host_name_a3f618) != 0) {
            if (TickCount.Time() - unknown_games_a3fa64[i]->Game.LastTime > 300) {
                Debug_Printf("Game '%ls' has timed out\n", unknown_games_a3fa64[i]->Name);
                delete unknown_games_a3fa64[i];
                unknown_games_a3fa64.Delete(unknown_games_a3fa64[i]);
                Refresh_Game_List_005bc790();
                if (i <= unknown_game_index_a71e90) {
                    Unknown_005bbdd0(false);
                    Unknown_005c01d0(0, 1, 0, 0);
                }
            }
        }
    }
    for (i = 1; i < unknown_chat_a3fa4c.Count(); i++) {
        if (unknown_chat_a3fa4c[i]->Chat.LastTime == 0) {
            unknown_chat_a3fa4c[i]->Chat.LastTime = TickCount.Time();
        }
        if (TickCount.Time() - unknown_chat_a3fa4c[i]->Chat.LastTime > 360) {
            delete unknown_chat_a3fa4c[i];
            unknown_chat_a3fa4c.Delete(unknown_chat_a3fa4c[i]);
            Unknown_005bbdd0(false);
        } else if (TickCount.Time() - unknown_chat_a3fa4c[i]->Chat.LastTime > 300 && unknown_chat_a3fa4c[i]->Chat.LastChance == 0) {
            UnknownGlobalPacket packet;
            memset(&packet, 0, sizeof(packet));
            strcpy(packet.Name, unknown_name_a3d3a0);
            packet.Command = 5;
            unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 0, &unknown_chat_a3fa4c[i]->Address, 0, 0);
            Service_Game_IO_00481fb0();
            unknown_chat_a3fa4c[i]->Chat.LastChance = 1;
        }
    }
}


// void __fastcall Refresh_Game_List_005bc790() (0x005BC790)

void __fastcall Refresh_Game_List_005bc790()
{
    wchar_t text[80];

    if (Current_Dialog_Id_00735b20() == 0xBB) {
        HWND dialog = Unknown_00735B10();
        HWND list = GetDlgItem(dialog, 0x549);
        if (unknown_game_index_a71e90 >= unknown_games_a3fa64.Count()) {
            unknown_game_index_a71e90 = unknown_games_a3fa64.Count() - 1;
            Unknown_005c01d0(0, 1, 0, 0);
        }
        if (unknown_game_index_a71e90 < 0) {
            unknown_game_index_a71e90 = 0;
        }
        int top = SendDlgItemMessageA(dialog, 0x549, 0x18E, 0, 0);
        SendDlgItemMessageA(dialog, 0x549, 0x49D, 0, 1);
        SendMessageA(list, 0x184, 0, 0);
        SendMessageA(list, 0x4CD, 0, (LPARAM)StringTable::Load_String("TXT_LOBBY", 0, "D:\\Ra2\\netdlg2.cpp", 490));
        for (int i = 1; i < unknown_games_a3fa64.Count(); i++) {
            if (unknown_games_a3fa64[i]->Game.IsOpen) {
                swprintf(text, StringTable::Load_String("TXT_THATGUYS_GAME", 0, "D:\\Ra2\\netdlg2.cpp", 496), unknown_games_a3fa64[i]->Name);
            } else {
                swprintf(text, StringTable::Load_String("TXT_THATGUYS_GAME_BRACKET", 0, "D:\\Ra2\\netdlg2.cpp", 498), unknown_games_a3fa64[i]->Name);
            }
            SendMessageA(list, 0x4CD, 0, (LPARAM)text);
        }
        SendDlgItemMessageA(dialog, 0x549, 0x186, unknown_game_index_a71e90, 0);
        SendDlgItemMessageA(dialog, 0x549, 0x197, top, 0);
        SendDlgItemMessageA(dialog, 0x549, 0x49D, 0, 0);
        list = GetDlgItem(dialog, 0x549);
        InvalidateRect(list, 0, 0);
        UpdateWindow(list);
    }
}


// Encodes the LAN scenario settings, players, and computer-player options.
extern char const net2_debug_length_7e6a44[],net2_debug_options_7e6a34[];
extern char net2_useroptions_a71bdc[512];
Wstring __fastcall Wide_To_7_Bit(WideString const &wide);

void __fastcall Build_Game_Options_005bc940(char *out)
{
    Wstring name = unknown_map_name_a3d342 ? Wide_To_7_Bit(unknown_map_name_a3d342) : "";
    int value=unknown_session_a3d298.unknown_884;
    sprintf(out,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s,%d,%d,%d,%s,%s:",unknown_a40d3c,unknown_a40b18,unknown_session_a3d298.GameSpeed_30,unknown_session_a3d298.unknown_24,unknown_session_a3d298.UnitCount,
        unknown_session_a3d298.unknown_2a,unknown_session_a3d298.unknown_2b,unknown_session_a3d298.unknown_2c,unknown_session_a3d298.unknown_a8,unknown_session_a3d298.unknown_crates_29,unknown_session_a3d298.GameModeIndex,
        name.Get(),value,unknown_session_a3d298.ScenarioIsOfficial_880,unknown_session_a3d298.ScenarioFileLength_87c,unknown_session_a3d298.ScenarioFileName_658,unknown_session_a3d298.ScenarioDigest_85a);
    memset(net2_useroptions_a71bdc,0,sizeof(net2_useroptions_a71bdc));
    for(int i=0,count=unknown_players_a3fa7c.Count();i<count;i++) {
        NodeNameType *entry=unknown_players_a3fa7c[i];
        int country=entry->unknown_66e290();
        int color=entry->unknown_66e2a0();
        unsigned short const *player_name=unknown_players_a3fa7c[i]->Name;
        sprintf(net2_useroptions_a71bdc+strlen(net2_useroptions_a71bdc),"%s,%d,%d,%x,%x,%x,",Wide_To_Narrow_006fa120(player_name),country,color,0,0,0);
    }
    net2_useroptions_a71bdc[strlen(net2_useroptions_a71bdc)-1]=':';
    strcat(net2_useroptions_a71bdc,"@:");
    for(int k=0;k<8;k++) {
        sprintf(net2_useroptions_a71bdc+strlen(net2_useroptions_a71bdc),"%d,%d,%d,",unknown_session_a3d298.AIDifficulty[k],unknown_session_a3d298.AICountry[k]==-3 ? -1 : unknown_session_a3d298.AICountry[k],unknown_session_a3d298.AIColor[k]);
    }
    Debug_Printf(net2_debug_length_7e6a44,strlen(net2_useroptions_a71bdc));
    Debug_Printf(net2_debug_options_7e6a34,net2_useroptions_a71bdc);
    strcat(out,net2_useroptions_a71bdc);
}


// void __fastcall Set_Player_Value_005bcbc0(char const *name, int value) (0x005BCBC0)

void __fastcall Set_Player_Value_005bcbc0(char const *name, int value)
{
    if (name == 0) {
        name = unknown_name_a3d3a0;
        if (value == 0) {
            HWND dialog = WS_Find_Dialog_00735940(0xBD);
            Unknown_005cd030(dialog, true);
        }
    }
    if (Compare_Wide_007837d3(Narrow_To_Wide_006fa180(name), unknown_host_name_a3f618) == 0) {
        value = 1;
    }
    int found = -1;
    for (int i = 0; i < unknown_players_a3fa7c.Count(); i++) {
        if (Compare_Narrow_00781f20(Wide_To_Narrow_006fa120(unknown_players_a3fa7c[i]->Name), name) == 0) {
            found = i;
            break;
        }
    }
    if (found != -1) {
        unknown_players_a3fa7c[found]->unknown_67 = value;
    }
    Unknown_005bbdd0(false);
}


// int __fastcall Unknown_Find_Player_By_Name_005bcc60(char const *name) (0x005BCC60)
// Looks a nick name up: no name means the local one (0x00A3D3A0); the host's name (0x00A3F618) answers 1;
// otherwise the last session player whose name matches (case-insensitively) answers its +0x67 field, -1 when none does.

// The original never inlines it (Unknown_005c28e0 calls it out of line).
#pragma auto_inline(off)
int __fastcall Unknown_Find_Player_By_Name_005bcc60(char const *name)
{
    if (name == 0) {
        name = unknown_name_a3d3a0;
    }
    if (Compare_Wide_007837d3(Narrow_To_Wide_006fa180(name), unknown_host_name_a3f618) == 0) {
        return 1;
    }
    int found = -1;
    for (int i = 0; i < unknown_players_a3fa7c.Count(); i++) {
        if (Compare_Narrow_00781f20(Wide_To_Narrow_006fa120(unknown_players_a3fa7c[i]->Name), name) == 0) {
            found = i;
        }
    }
    if (found != -1) {
        return unknown_players_a3fa7c[found]->unknown_67;
    }
    return -1;
}
#pragma auto_inline(on)


// Not exact (RA2_NON_MATCHING): Set_Slot_Country_Color_005bccf0 at 0x005BCCF0, best score 86.25, 2026-11-13; differs: 134 instructions in the original, 135 in the draft (shape 98.88)
#ifdef RA2_NON_MATCHING
// int __fastcall Set_Slot_Country_Color_005bccf0(char const *name, int country, int color) (0x005BCCF0)

int __fastcall Set_Slot_Country_Color_005bccf0(char const *name, int country, int color)
{
    int slot = -1;
    for (int i = 0; i < 8; i++) {
        char *slot_name = Wide_To_Narrow_006fa120(unknown_player_slots_a3fa98[i]->Name);
        if (Compare_Narrow_00781f20(slot_name, name) == 0) {
            slot = i;
            break;
        }
    }
    int changed = 0;
    if (slot == -1) {
        return 0;
    }
    HWND dialog = WS_Find_Dialog_00735940(0xBC);
    if (dialog == 0) {
        dialog = WS_Find_Dialog_00735940(0xBD);
    }
    NodeNameType *entry = unknown_player_slots_a3fa98[slot];
    if (entry->unknown_66e290() != country) {
        changed = 1;
        entry->unknown_66e2d0(country);
    }
    int id = Player_Side_Combo_ID_4d1350(slot);
    bool observer = Side_Combo_Data_4d1c70(dialog, id, -1) == -3;
    if (country == -3) {
        if (!observer) {
            Fill_Observer_Combo_4d1640(dialog, id);
            int control = Slot_Control_Id_005c97a0(slot);
            SendMessageA(GetDlgItem(dialog, control), 0x4F1, 0, 1);
            int color_id = Player_Color_Combo_ID_4d1cb0(slot);
            Fill_Random_Color_Combo_4d2250(dialog, color_id);
            Select_Color_Combo_4d2480(dialog, color_id, 8);
        }
    } else if (observer) {
        Fill_Side_Combo_4d1560(dialog, id);
        int control = Slot_Control_Id_005c97a0(slot);
        SendMessageA(GetDlgItem(dialog, control), 0x4F1, 0, 0);
        int color_id = Player_Color_Combo_ID_4d1cb0(slot);
        Fill_Color_Combo_4d2080(dialog, color_id);
        Select_Color_Combo_4d2480(dialog, color_id, -2);
    }
    if (unknown_player_slots_a3fa98[slot]->unknown_66e2a0() != color) {
        entry->unknown_66e300(color);
    }
    if (unknown_player_slots_a3fa98[slot] == unknown_players_a3fa7c[0]) {
        unknown_a3d3b4 = color;
    }
    Unknown_005bbdd0(false);
    return changed;
}
#endif

// bool __fastcall Get_Serial_005bce90(char *serial) (0x005BCE90)

bool __fastcall Get_Serial_005bce90(char *serial)
{
    HKEY key;
    DWORD size;
    DWORD type;
    char buffer[128];

    memset(buffer, '0', 22);
    buffer[22] = 0;
    strcpy(serial, buffer);
    memset(buffer, 0, sizeof(buffer));
    char const *path = "SOFTWARE\\Westwood\\Red Alert 2";
    if (path != 0 && strlen(path) != 0) {
        char name[256];
        strcpy(name, path);
        if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, name, 0, KEY_READ, &key) == 0) {
            size = sizeof(buffer);
            RegQueryValueExA(key, "Serial", 0, &type, (BYTE *)buffer, &size);
            RegCloseKey(key);
        }
    }
    buffer[22] = 0;
    strcpy(serial, buffer);
    int i = 0;
    _iobuf *file = fopen("woldata.key", "r");
    if (file == 0) {
        return false;
    }
    int c;
    while ((c = fgetc(file)) != -1) {
        buffer[i] = (((buffer[i] - '0') % 10) - c + 1000) % 10 + '0';
        i++;
        if (i == (int)strlen(buffer)) {
            i = 0;
        }
    }
    fclose(file);
    strcpy(serial, buffer);
    return true;
}


// bool Net_Join_Dialog_005bd070() (0x005BD070): the LAN join dialog's driver loop (Red Alert's
// Net_Join_Dialog, TibSun's Net2Remote_Connect): opens the join dialog 0xBB, pumps messages and the
// join queries until a button result arrives, then handles cancel/back (0x5C0, 2), join (0x6CD,
// 0x6CE), new game (0x6CC: the host dialog 0xBC), the guest's start (1 from 0xBD) and the host's
// GO (0x588). true when a game starts.

void __fastcall unknown_5eac30();
// The join dialog's packet pump (0x005C08A0), defined later in this unit.
void __fastcall Get_Join_Responses_005c08a0();
long __stdcall Host_Dialog_Proc_005befe0(HWND window, unsigned message, unsigned wparam, long lparam);
// A joiner's start of the game: the scenario is found or fetched (NET_READY_TO_GO 0x13 to the host),
// the IPX timing is set from the response time; otherwise TXT_UNABLE_PLAY_WAAUGH and a sign-off
// (0x005BE3B0; from Red Alert's Net_Join_Dialog). The argument is unused (0 for join state 3, 1 for
// state 4). Declared here, its only caller: a new parameter in shared/lobby.hpp changes the code of
// Pick_Lobby 0x00744090 and Print_CRCs 0x0062A360.
bool __fastcall Start_Joined_Game_005be3b0(int unknown);
// Seats the first session player (the host) in a slot and makes it the current slot (0x005C8CD0).
void __fastcall Seat_First_Player_005c8cd0(int slot);
// The version the session plays against (session +0x63C; Red Alert's Session.PlayingAgainstVersion).
extern unsigned long unknown_version_a3d8d4;
extern int unknown_global_length_a3f808;
extern unsigned short unknown_global_product_a3f818;

bool __fastcall Net_Join_Dialog_005bd070()
{
    unknown_a71e9c = Unknown_006525F0();
    Read_Art_INI();
    unknown_a71ea0 = unknown_art_ini_unique_id();
    unknown_a71ea4 = unknown_ini_8397a0_unique_id();
    Get_Serial_005bce90(unknown_a71bc4);
    unknown_ipx_manager_a40978.Set_Timing(30, -1, 600, true);
    unknown_session_a3f614 = false;
    unknown_host_name_a3f618[0] = 0;
    unknown_session_a3f616 = true;
    unknown_lock_a71e98 = false;
    unknown_5eac30();
    HWND dialog = (HWND)unknown_create_dialog_735700(ProgramInstance, 0xBB, MainWindow, Join_Dialog_Proc_005be6b0, 0);
    unknown_show_dialog_5ff7e0(dialog, 0xBB);
    ShowWindow(dialog, 1);
    Unknown_005bbdd0(false);
    unknown_game_index_a71e90 = 0;
    unknown_lobby_result_a71e8c = 0;
    unknown_join_state_a71e94 = 0;
    unknown_join_list_7e699c = true;

    while (true) {
        while (unknown_lobby_result_a71e8c == 0) {
            unknown_ipx_manager_a40978.Service();
            Sleep(0);
            Service_Game_IO_00481fb0();
            unknown_ipx_manager_a40978.Service();
            unknown_517370(false);
            unknown_message_count_a52298++;
            MSG msg;
            while (unknown_peek_message_522ed0(&msg, 0, 0, 0, 1)) {
                TranslateMessage(&msg);
                DispatchMessageA(&msg);
            }
            Service_Game_IO_00481fb0();
            if (unknown_lobby_result_a71e8c != 0) {
                break;
            }
            if (Unknown_00735B10()) {
                Unknown_005c01d0(0, 0, 0, 0);
                Get_Join_Responses_005c08a0();
                if (Current_Dialog_Id_00735b20() == 0xBC) {
                    Unknown_005c3810(false, false);
                }
                Process_Timeouts_005bc500();
            }
        }

        if (unknown_lobby_result_a71e8c == 0x5C0 || unknown_lobby_result_a71e8c == 2) {
            unknown_session_a3d298.Write_MultiPlayer_Settings();
            if (Current_Dialog_Id_00735b20() == 0xBB) {
                if (unknown_join_state_a71e94 > 0) {
                    Unjoin_Game_005c00b0(unknown_game_index_a71e90);
                    unknown_ipx_manager_a40978.Service();
                }
                End_Dialog_007357e0(0, 0);
                Clear_Slot_Lists_005c8a70();
                Fill_Slots_005c8a10();
                Free_Pointers_005a7480(&unknown_players_a3fa7c);
                Free_Pointers_005a7480(&unknown_games_a3fa64);
                Free_Pointers_005a7480(&unknown_chat_a3fa4c);
                unknown_session_a3f616 = false;
                unknown_ipx_manager_a40978.Service();
                return false;
            }
            if (Current_Dialog_Id_00735b20() == 0xBC) {
                Unjoin_Game_005c00b0(unknown_game_index_a71e90);
                unknown_join_state_a71e94 = 0;
                End_Dialog_007357e0(Unknown_00735B10(), 0);
                Unknown_Reset_Slots_005c8a90();
                dialog = (HWND)unknown_create_dialog_735700(ProgramInstance, 0xBB, MainWindow, Join_Dialog_Proc_005be6b0, 0);
                unknown_show_dialog_5ff7e0(dialog, 0xBB);
                ShowWindow(dialog, 1);
                Unknown_005c01d0(0, 0, 1, 0);
            } else if (Current_Dialog_Id_00735b20() == 0xBD) {
                if (unknown_join_state_a71e94 == 2) {
                    Unjoin_Game_005c00b0(unknown_game_index_a71e90);
                    unknown_join_state_a71e94 = 0;
                } else {
                    UnknownGlobalPacket packet;
                    memset(&packet, 0, sizeof(packet));
                    packet.Command = 10;
                    strcpy(packet.Name, unknown_name_a3d3a0);
                    for (int i = 1; i < unknown_chat_a3fa4c.Count(); i++) {
                        unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 1, &unknown_chat_a3fa4c[i]->Address, 0, 0);
                        Service_Game_IO_00481fb0();
                    }
                    unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 0, 0, 0, 0);
                    if (unknown_dest_net_set_a3f604) {
                        unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 0, &unknown_dest_net_a3f608, 0, 0);
                    }
                }
                CDTimerClass<SystemTimerClass> timeout = 600;
                while (unknown_ipx_manager_a40978.Global_Num_Send() > 0) {
                    if (unknown_ipx_manager_a40978.Service() == 0) {
                        break;
                    }
                    if (timeout == 0) {
                        break;
                    }
                    Service_Game_IO_00481fb0();
                }
                unknown_host_name_a3f618[0] = 0;
                unknown_join_state_a71e94 = 0;
                End_Dialog_007357e0(0, 0);
                unknown_lobby_result_a71e8c = 0;
                unknown_game_index_a71e90 = 0;
                Free_Pointers_005a7480(&unknown_players_a3fa7c);
                Clear_Slot_Lists_005c8a70();
                Unknown_Reset_Slots_005c8a90();
                dialog = (HWND)unknown_create_dialog_735700(ProgramInstance, 0xBB, MainWindow, Join_Dialog_Proc_005be6b0, 0);
                unknown_show_dialog_5ff7e0(dialog, 0xBB);
                ShowWindow(dialog, 1);
            } else {
                if (unknown_join_state_a71e94 == 2) {
                    Unjoin_Game_005c00b0(unknown_game_index_a71e90);
                    unknown_join_state_a71e94 = 0;
                } else {
                    UnknownGlobalPacket packet;
                    memset(&packet, 0, sizeof(packet));
                    packet.Command = 10;
                    strcpy(packet.Name, unknown_name_a3d3a0);
                    for (int i = 1; i < unknown_chat_a3fa4c.Count(); i++) {
                        unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 1, &unknown_chat_a3fa4c[i]->Address, 0, 0);
                        Service_Game_IO_00481fb0();
                    }
                    unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 0, 0, 0, 0);
                    if (unknown_dest_net_set_a3f604) {
                        unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 0, &unknown_dest_net_a3f608, 0, 0);
                    }
                }
                CDTimerClass<SystemTimerClass> timeout = 600;
                while (unknown_ipx_manager_a40978.Global_Num_Send() > 0) {
                    if (unknown_ipx_manager_a40978.Service() == 0) {
                        break;
                    }
                    if (timeout == 0) {
                        break;
                    }
                    Service_Game_IO_00481fb0();
                }
                unknown_host_name_a3f618[0] = 0;
                unknown_join_state_a71e94 = 0;
                unknown_lobby_result_a71e8c = 0;
                unknown_game_index_a71e90 = 0;
                Free_Pointers_005a7480(&unknown_players_a3fa7c);
                Clear_Slot_Lists_005c8a70();
                Unknown_Reset_Slots_005c8a90();
                dialog = (HWND)unknown_create_dialog_735700(ProgramInstance, 0xBB, MainWindow, Join_Dialog_Proc_005be6b0, 0);
                unknown_show_dialog_5ff7e0(dialog, 0xBB);
                ShowWindow(dialog, 1);
            }
        }

        if (unknown_lobby_result_a71e8c == 0x6CD) {
            unknown_session_a3f614 = false;
            unknown_session_a3d298.Write_MultiPlayer_Settings();
            if (Request_To_Join_005bfc40(unknown_game_index_a71e90, unknown_a72c68 != 0)) {
                unknown_join_state_a71e94 = 1;
            } else {
                HWND top = Unknown_00735B10();
                if (top) {
                    EnableWindow(top, TRUE);
                    HWND button = GetDlgItem(top, 0x541);
                    if (button) {
                        EnableWindow(button, TRUE);
                    }
                }
            }
            if (unknown_session_a3d298.Mode) {
                unknown_session_a3d298.Mode->unknown_slot9();
            }
        }

        if (unknown_lobby_result_a71e8c == 0x6CE) {
            unknown_session_a3f614 = false;
            unknown_session_a3d298.Write_MultiPlayer_Settings();
            if (Request_To_Join_005bfc40(unknown_game_index_a71e90, true)) {
                unknown_join_state_a71e94 = 1;
            } else {
                HWND top = Unknown_00735B10();
                if (top) {
                    EnableWindow(top, TRUE);
                    HWND button = GetDlgItem(top, 0x541);
                    if (button) {
                        EnableWindow(button, TRUE);
                    }
                }
            }
            if (unknown_session_a3d298.Mode) {
                unknown_session_a3d298.Mode->unknown_slot9();
            }
        }

        if (unknown_lobby_result_a71e8c == 0x6CC) {
            bool ok = true;
            if (strlen(unknown_name_a3d3a0) < 1) {
                Message_Box_005c3230(StringTable::Load_String("TXT_NAME_BLANK", 0, "D:\\Ra2\\netdlg2.cpp", 0x623), 0, Unknown_005bbd10, false);
                ok = false;
            }
            for (int i = 0; i < unknown_games_a3fa64.Count(); i++) {
                if (ok && !_stricmp(Wide_To_Narrow_006fa120(unknown_games_a3fa64[i]->Name), unknown_name_a3d3a0)) {
                    Message_Box_005c3230(StringTable::Load_String("TXT_GAMENAME_MUSTBE_UNIQUE", 0, "D:\\Ra2\\netdlg2.cpp", 0x62D), 0, Unknown_005bbd10, false);
                    ok = false;
                    break;
                }
            }
            if (ok) {
                wcscpy(unknown_host_name_a3f618, Narrow_To_Wide_006fa180(unknown_name_a3d3a0));
                unknown_session_a3f616 = true;
                unknown_session_a3f614 = false;
                unknown_session_a3d2b4 = 0;
                unknown_version_a3d8d4 = VerNum.Version_Number();
                Select_Scenario_005c7c30(unknown_session_a3d2b4);
                End_Dialog_007357e0(0, 0);
                unknown_lobby_result_a71e8c = 0;
                Free_Pointers_005a7480(&unknown_players_a3fa7c);

                NodeNameType *who = new NodeNameType;
                memset(who, 0, sizeof(*who));
                wcscpy(who->Name, Narrow_To_Wide_006fa180(unknown_name_a3d3a0));
                strcpy(who->unknown_34, unknown_a71bc4);
                who->unknown_66e2d0(unknown_session_a3d298.Value_0124_006729f0());
                who->unknown_66e300(unknown_session_a3d298.Value_011c_00672a10());
                who->unknown_67 = 1;
                who->Team = 0;
                unknown_lobby_flag_7e6d90 = 7;
                if (unknown_a71e99) {
                    unknown_lobby_flag_7e6d90 = 0;
                    who->Team = -1;
                } else if (who->unknown_66e290() == -3) {
                    who->unknown_66e2d0(-2);
                }
                unknown_session_a3d298.Players.Add(who);
                if (unknown_session_a3d298.Mode) {
                    unknown_session_a3d298.Mode->unknown_slot9();
                }
                Clear_Slot_Lists_005c8a70();
                Unknown_Reset_Slots_005c8a90();
                Seat_First_Player_005c8cd0(0);
                if (unknown_lobby_flag_7e6d90 == 0) {
                    unknown_player_slots_a3fa98[0]->unknown_66e2d0(-3);
                    unknown_player_slots_a3fa98[0]->Team = -1;
                }
                unknown_join_state_a71e94 = 2;

                NodeNameType *game = new NodeNameType;
                wcscpy(game->Name, Narrow_To_Wide_006fa180(unknown_name_a3d3a0));
                game->Address = unknown_address_a3f80c;
                game->Game.IsOpen = true;
                game->Game.LastTime = TickCount.Time();
                unknown_session_a3d298.NodeList_27cc.Add(game);
                unknown_game_index_a71e90 = unknown_games_a3fa64.Count() - 1;

                HWND host = (HWND)unknown_create_dialog_735700(ProgramInstance, 0xBC, MainWindow, Host_Dialog_Proc_005befe0, 0);
                unknown_show_dialog_5ff7e0(host, 0xBC);
                SendMessageA(host, 0x4A9, 0, 1);
                ShowWindow(host, 1);
            } else {
                HWND top = Unknown_00735B10();
                if (top) {
                    EnableWindow(top, TRUE);
                    HWND button = GetDlgItem(top, 0x541);
                    if (button) {
                        EnableWindow(button, TRUE);
                    }
                }
            }
        }

        if (unknown_lobby_result_a71e8c == 1 && Current_Dialog_Id_00735b20() == 0xBD) {
            while (End_Dialog_007357e0(0, 0) == true) {
            }
            if (unknown_join_state_a71e94 == 3) {
                if (!Start_Joined_Game_005be3b0(0)) {
                    unknown_lobby_result_a71e8c = 2;
                    unknown_lock_a71e98 = false;
                    continue;
                }
                unknown_lock_a71e98 = true;
            } else if (unknown_join_state_a71e94 == 4) {
                Start_Joined_Game_005be3b0(1);
            }
            unknown_lobby_result_a71e8c = 0;
            Unknown_Pregame_Setup_005c74f0();
            unknown_session_a3d298.unknown_2cc = 5;
            unknown_a3fba4 = 0;
            unknown_session_a3d298.unknown_2e4 = 0;
            unknown_a3d580 = 0;
            if (unknown_session_a3d2ac == 2) {
                unknown_max_ahead_a3d560 = ((unknown_ipx_manager_a40978.unknown_526c30() / 8 + unknown_session_a3d298.unknown_2cc - 1) / unknown_session_a3d298.unknown_2cc) * unknown_session_a3d298.unknown_2cc > unknown_session_a3d298.unknown_2cc * 2 ? ((unknown_ipx_manager_a40978.unknown_526c30() / 8 + unknown_session_a3d298.unknown_2cc - 1) / unknown_session_a3d298.unknown_2cc) * unknown_session_a3d298.unknown_2cc : unknown_session_a3d298.unknown_2cc * 2;
            } else {
                unknown_max_ahead_a3d560 = (int)unknown_ipx_manager_a40978.unknown_526c30() / 8 > 2 ? (int)unknown_ipx_manager_a40978.unknown_526c30() / 8 : 2;
            }
            unknown_session_a3f616 = false;
            unknown_session_a3d298.Write_MultiPlayer_Settings();
            return true;
        }

        if (unknown_lobby_result_a71e8c == 0x588) {
            unknown_lock_a71e98 = false;
            unknown_session_a3d298.Write_MultiPlayer_Settings();
        }
        int count = unknown_players_a3fa7c.Count();
        int observers = 0;
        for (int i = 0; i < count; i++) {
            if (unknown_players_a3fa7c[i]->Team == -1) {
                observers++;
            }
        }
        if (unknown_lobby_result_a71e8c == 0x588) {
            if (count - observers <= 1) {
                Unknown_005C2C80(-1, StringTable::Load_String("TXT_ONLY_ONE", 0, "D:\\Ra2\\netdlg2.cpp", 0x6E0));
                unknown_lobby_result_a71e8c = 0;
                EnableWindow(GetDlgItem(Unknown_00735B10(), 0x588), TRUE);
            }
            if (unknown_lobby_result_a71e8c == 0x588) {
                bool nomap = false;
                for (int i = 1; i < count; i++) {
                    NodeNameType *player = unknown_players_a3fa7c[i];
                    if (player->unknown_map_state_6f == 0) {
                        Unknown_005C2C80(-1, StringTable::Load_String("GUI:HostNoMap", 0, "D:\\Ra2\\netdlg2.cpp", 0x6EC), player->Name, unknown_map_name_a3d342);
                        nomap = true;
                    }
                }
                if (nomap) {
                    unknown_lobby_result_a71e8c = 0;
                    EnableWindow(GetDlgItem(Unknown_00735B10(), 0x588), TRUE);
                }
            }
        }
        MultiplayerGameMode *mode = unknown_session_a3d298.Mode;
        if (unknown_lobby_result_a71e8c == 0x588) {
            int map_players = Unknown_005c6600(unknown_session_a3d2b4);
            if (map_players < unknown_session_a3d298.AICount - observers + count) {
                Unknown_005C2C80(-1, StringTable::Load_String("TXT_SCENARIO_TOO_SMALL", 0, "D:\\Ra2\\netdlg2.cpp", 0x6FF), map_players);
                EnableWindow(GetDlgItem(Unknown_00735B10(), 0x588), TRUE);
                unknown_lobby_result_a71e8c = 0;
            }
            {
                WideString message;
                if (!mode->CheckStartConditions(&message)) {
                    Unknown_005C2C80(-1, message.Get());
                    EnableWindow(GetDlgItem(Unknown_00735B10(), 0x588), TRUE);
                    unknown_lobby_result_a71e8c = 0;
                }
            }
            if (unknown_lobby_result_a71e8c == 0x588) {
                for (int i = 0; i < count; i++) {
                    if (unknown_players_a3fa7c[i]->unknown_67 == 0) {
                        Unknown_005c3b70();
                        Unknown_005C2C80(-1, StringTable::Load_String("GUI:HostGameStartHost", 0, "D:\\Ra2\\netdlg2.cpp", 0x719));
                        unknown_lobby_result_a71e8c = 0;
                        EnableWindow(GetDlgItem(Unknown_00735B10(), 0x588), TRUE);
                        break;
                    }
                }
                if (unknown_lobby_result_a71e8c == 0x588) {
                    unknown_lock_a71e98 = true;
                    int i;
                    for (i = 0; i < unknown_players_a3fa7c.Count(); i++) {
                        if (unknown_players_a3fa7c[i]->Team == -1) {
                            unknown_players_a3fa7c[i]->unknown_66e300(-2);
                        }
                    }
                    mode->unknown_slot22();
                    unknown_session_a3d298.Unknown_00672BB0();
                    Unknown_005c3810(true, true);
                    {
                        UnknownGlobalPacket packet;
                        memset(&packet, 0, sizeof(packet));
                        packet.Command = 0x16;
                        strcpy(packet.Name, unknown_name_a3d3a0);
                        Unknown_Slot_Names_005c8cf0(packet.Slots.Names);
                        int players = unknown_players_a3fa7c.Count();
                        for (i = 1; i < players; i++) {
                            unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 1, &unknown_players_a3fa7c[i]->Address, 0, 0);
                            Service_Game_IO_00481fb0();
                        }
                    }
                    mode->unknown_slot21();
                    if (unknown_dialog_preview_a72ce8 != 0) {
                        delete unknown_dialog_preview_a72ce8;
                        unknown_dialog_preview_a72ce8 = 0;
                    }
                    Unknown_Pregame_Setup_005c74f0();
                    unknown_session_a3d298.unknown_2cc = 5;
                    unknown_a3fba4 = 0;
                    unknown_session_a3d298.unknown_2e4 = 0;
                    unknown_a3d580 = 0;
                    if (unknown_session_a3d2ac == 2) {
                        unknown_max_ahead_a3d560 = ((unknown_ipx_manager_a40978.unknown_526c30() / 8 + unknown_session_a3d298.unknown_2cc - 1) / unknown_session_a3d298.unknown_2cc) * unknown_session_a3d298.unknown_2cc > unknown_session_a3d298.unknown_2cc * 2 ? ((unknown_ipx_manager_a40978.unknown_526c30() / 8 + unknown_session_a3d298.unknown_2cc - 1) / unknown_session_a3d298.unknown_2cc) * unknown_session_a3d298.unknown_2cc : unknown_session_a3d298.unknown_2cc * 2;
                    } else {
                        unknown_max_ahead_a3d560 = (int)unknown_ipx_manager_a40978.unknown_526c30() / 8 > 2 ? (int)unknown_ipx_manager_a40978.unknown_526c30() / 8 : 2;
                    }
                    unknown_ipx_manager_a40978.Set_Timing(30 > unknown_ipx_manager_a40978.unknown_526c30() + 2 ? 30 : unknown_ipx_manager_a40978.unknown_526c30() + 2, -1, 600, true);

                    UnknownGlobalPacket packet;
                    memset(&packet, 0, sizeof(packet));
                    packet.Command = 0x0B;
                    packet.Progress.Percent = unknown_max_ahead_a3d560;   // NET_GO carries MaxAhead at +0x2F
                    for (i = 1; i < unknown_players_a3fa7c.Count(); i++) {
                        unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 1, &unknown_players_a3fa7c[i]->Address, 0, 0);
                    }

                    CDTimerClass<SystemTimerClass> timeout = 1200;
                    while (unknown_ipx_manager_a40978.Global_Num_Send() > 0 && !timeout) {
                        Service_Game_IO_00481fb0();
                    }

                    int responses[8];
                    memset(responses, 0, sizeof(responses));
                    int num_responses = 0;
                    bool send_scenario = false;
                    Debug_Printf("About to wait for 'GO' response.\n");
                    CDTimerClass<SystemTimerClass> response_timer;
                    response_timer = 600;
                    do {
                        Service_Game_IO_00481fb0();
                        if (unknown_ipx_manager_a40978.Get_Global_Message(&unknown_gpacket_a3f640, &unknown_global_length_a3f808, &unknown_address_a3f80c, &unknown_global_product_a3f818)
                            && unknown_global_product_a3f818 == 0xAA03) {
                            for (i = 1; i < unknown_players_a3fa7c.Count(); i++) {
                                if (unknown_players_a3fa7c[i]->Address == unknown_address_a3f80c) {
                                    if (!responses[i]) {
                                        if (unknown_gpacket_a3f640.Command == 0x10) {
                                            Debug_Printf("Received REQ_SCENARIO packet.\n");
                                            responses[i] = unknown_gpacket_a3f640.Command;
                                            send_scenario = true;
                                            num_responses++;
                                        }
                                        if (unknown_gpacket_a3f640.Command == 0x13) {
                                            Debug_Printf("Received READY_TO_GO packet.\n");
                                            responses[i] = unknown_gpacket_a3f640.Command;
                                            num_responses++;
                                        }
                                    }
                                }
                            }
                        }
                    } while (num_responses < unknown_players_a3fa7c.Count() - 1 && response_timer);

                    End_Dialog_007357e0(0, 0);
                    unknown_5159b0(0, true);
                    if (send_scenario) {
                        memset(unknown_session_a3d298.ScenarioRequests, 0, sizeof(unknown_session_a3d298.ScenarioRequests));
                        unknown_session_a3d298.RequestCount = 0;
                        for (i = 1; i < unknown_session_a3d298.Players.Count(); i++) {
                            if (responses[i] == 0x10) {
                                unknown_session_a3d298.ScenarioRequests[unknown_session_a3d298.RequestCount] = i;
                                unknown_session_a3d298.RequestCount++;
                            }
                        }
                        Send_Remote_File_0066bb20(unknown_id_source_a3d290->FileName, 3, false, true);
                    }
                    unknown_ipx_manager_a40978.Set_Timing(unknown_ipx_manager_a40978.unknown_526c30() + 2 > 30 ? unknown_ipx_manager_a40978.unknown_526c30() + 2 : 30,
                        -1, 120 > unknown_ipx_manager_a40978.unknown_526c30() * 8 ? 120 : unknown_ipx_manager_a40978.unknown_526c30() * 8, true);
                    break;
                }
            }
        }
        unknown_lobby_result_a71e8c = 0;
    }

    unknown_session_a3f616 = false;
    unknown_session_a3d298.Write_MultiPlayer_Settings();
    return true;
}


// bool __fastcall Start_Joined_Game_005be3b0(int unknown) (0x005BE3B0; the argument is unused: Net_Join_Dialog 0x005BD070 passes 0 for join state 3 and 1 for state 4 and declares it)

bool __fastcall Start_Joined_Game_005be3b0(int unknown)
{
    unknown_ipx_manager_a40978.Set_Timing(unknown_ipx_manager_a40978.unknown_526c30() + 2 > 30 ? unknown_ipx_manager_a40978.unknown_526c30() + 2 : 30,
        -1, 1000, true);
    if (Find_Local_Scenario_0066c5b0(unknown_session_a3d298.ScenarioFileName_658, unknown_session_a3d298.ScenarioFileLength_87c, unknown_session_a3d298.ScenarioDigest_85a,
            unknown_session_a3d298.ScenarioIsOfficial_880, true) == true) {
        Debug_Printf("Found local scenario, file name is %d\n", unknown_session_a3d298.ScenarioFileName_658);
        UnknownGlobalPacket packet;
        memset(&packet, 0, sizeof(packet));
        packet.Command = 0x13;
        unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 1, &unknown_address_a3db2c, 0, 0);
        while (unknown_ipx_manager_a40978.Global_Num_Send() > 0 && unknown_ipx_manager_a40978.Service() != 0) {
            Service_Game_IO_00481fb0();
        }
    } else {
        Debug_Printf("Failed to find local scenario, file name is %d\n", unknown_session_a3d298.ScenarioFileName_658);
        if ((unknown_session_a3d298.ScenarioIsOfficial_880 && !unknown_session_a3d298.Unknown_00672140())
            || !Get_File_From_Host_0066aef0(unknown_session_a3d298.ScenarioFileName_658, 3, true)) {
            unknown_session_a3d2b4 = -1;
            WWMessageBox(L" ").Process(StringTable::Load_String("TXT_UNABLE_PLAY_WAAUGH", 0, "D:\\Ra2\\netdlg2.cpp", 678),
                StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\netdlg2.cpp", 678), 0, 0, false);
            UnknownGlobalPacket packet;
            memset(&packet, 0, sizeof(packet));
            packet.Command = 10;
            strcpy(packet.Name, unknown_name_a3d3a0);
            for (int i = 1; i < unknown_chat_a3fa4c.Count(); i++) {
                unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 1, &unknown_chat_a3fa4c[i]->Address, 0, 0);
                Service_Game_IO_00481fb0();
            }
            unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 0, 0, 0, 0);
            if (unknown_dest_net_set_a3f604) {
                unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 0, &unknown_dest_net_a3f608, 0, 0);
            }
            while (unknown_ipx_manager_a40978.Global_Num_Send() > 0 && unknown_ipx_manager_a40978.Service() != 0) {
                Service_Game_IO_00481fb0();
            }
            return false;
        }
    }
    strcpy(unknown_id_source_a3d290->FileName, unknown_session_a3d298.ScenarioFileName_658);
    int response = unknown_ipx_manager_a40978.unknown_526c30();
    if (response < 20) {
        Debug_Printf("IPX.Global_Response_Time() == %d. Adjusting to 20\n", response);
        response = 20;
    }
    unknown_ipx_manager_a40978.Set_Timing(response, -1, response * 8 < 120 ? 120 : response * 8, true);
    unknown_ipx_manager_a40978.unknown_slot14(60, -1, 600);
    return true;
}


// long __stdcall Join_Dialog_Proc_005be6b0(HWND window, unsigned message, unsigned wparam, long lparam) (0x005BE6B0)
// The LAN join dialog's procedure (from Red Alert's Net_Join_Dialog).

long __stdcall Join_Dialog_Proc_005be6b0(HWND window, unsigned message, unsigned wparam, long lparam)
{
    switch (message) {
    case 0x14:                  // WM_ERASEBKGND
        return 1;
    case 0x0F:                  // WM_PAINT
        Unknown_005FEE70(window);
        unknown_dialog_5e5f20(window);
        ValidateRect(window, 0);
        return 0;
    case 0x2B:                  // WM_DRAWITEM
        Unknown_005FE380(lparam);
        return 1;
    case 0x111:                 // WM_COMMAND
        switch ((unsigned short)wparam) {
        case 0x549:             // the game list
            if (unknown_join_state_a71e94 <= 0) {
                int old_index = unknown_game_index_a71e90;
                int index = SendDlgItemMessageA(window, 0x549, 0x188, 0, 0);
                if (index >= 0 && unknown_join_list_7e699c) {
                    unknown_game_index_a71e90 = index;
                    wcscpy(unknown_host_name_a3f618, unknown_games_a3fa64[index]->Name);
                }
                if ((unsigned short)(wparam >> 16) == 1) {
                    Free_Pointers_005a7480(&unknown_players_a3fa7c);
                    if (old_index != unknown_game_index_a71e90) {
                        Unknown_005c01d0(1, 1, 1, 0);
                    }
                    Unknown_005bbdd0(false);
                    return 0;
                }
                if ((unsigned short)(wparam >> 16) == 2) {
                    unknown_lobby_result_a71e8c = unknown_a72c68 == 1 ? 0x6CE : 0x6CD;
                    return 0;
                }
            }
            break;
        case 0x541:             // the name edit
            if (IsWindowEnabled(window) && unknown_lobby_result_a71e8c == 0) {
                wchar_t name[20];
                name[0] = 0;
                HWND edit = GetDlgItem(window, 0x541);
                SendMessageA(edit, 0x4B3, 20, (LPARAM)name);
                wchar_t *text = name;
                while (*text != 0 && *text == ' ') {
                    text++;
                }
                if (*text == 0 && name[0] != 0) {
                    name[0] = 0;
                    edit = GetDlgItem(window, 0x541);
                    SendMessageA(edit, 0x4B2, 0, (LPARAM)name);
                }
                if (strcmp(Wide_To_Narrow_006fa120(name), unknown_name_a3d3a0) != 0) {
                    strcpy(unknown_name_a3d3a0, Wide_To_Narrow_006fa120(name));
                    Unknown_005c01d0(0, 0, 1, 0);
                    Unknown_005bbdd0(false);
                    return 0;
                }
            }
            break;
        case 0x540:             // the chat line
            Limit_Name_Text_005ccf50((HWND)lparam);
            if ((unsigned short)(wparam >> 16) == 0x501) {
                Send_Chat_Message_005bee20(window, unknown_color_7a737c, unknown_color_7a7364);
                return 0;
            }
            break;
        case 0x5C0:
            unknown_lobby_result_a71e8c = 0x5C0;
            Unknown_005CD3A0();
            return 0;
        case 0x6CC:
            unknown_lobby_result_a71e8c = (unsigned short)wparam;
            unknown_a71e99 = unknown_a72c68 == 1;
            return 0;
        case 0x6CD:
        case 0x6CE: {
            EnableWindow(window, FALSE);
            HWND edit = GetDlgItem(window, 0x541);
            if (edit != 0) {
                EnableWindow(edit, FALSE);
            }
            unknown_lobby_result_a71e8c = (unsigned short)wparam;
            return 0;
        }
        case 0x75F:
        case 0x760: {           // the observer radio buttons
            int checked = 0;
            HWND button = GetDlgItem(window, 0x760);
            if (button != 0) {
                checked = SendMessageA(button, 0xF0, 0, 0);
            }
            if ((unsigned short)wparam == 0x75F) {
                checked = checked == 0;
            }
            unknown_a72c68 = checked;
            Unknown_005cd540(0);
            return 0;
        }
        }
        break;
    case 0x200: {               // WM_MOUSEMOVE: the tool tip of the control under the cursor
        HWND tip = GetDlgItem(window, 0x695);
        if (tip != 0) {
            POINT point;
            point.x = lparam & 0xFFFF;
            point.y = (unsigned long)lparam >> 16;
            RECT rect;
            GetWindowRect(window, &rect);
            point.x -= rect.left;
            point.y -= rect.top;
            wchar_t const *text = L"";
            HWND control = ChildWindowFromPointEx(window, point, 1);
            if (control != 0) {
                char const *label = Unknown_005e22b0(window, control);
                if (label != 0) {
                    text = StringTable::Load_String(label, 0, "D:\\Ra2\\netdlg2.cpp", 2285);
                }
            }
            SendMessageA(tip, 0x4B2, 0, (LPARAM)text);
            return 0;
        }
        break;
    }
    case 0x497: {
        unknown_game_index_a71e90 = 0;
        unknown_join_list_7e699c = true;
        Unknown_005cd540(window);
        SendDlgItemMessageA(window, 0x541, 0xC5, 16, 0);
        SendDlgItemMessageA(window, 0x541, 0x4EB, 0, (LPARAM)L",");
        SendDlgItemMessageA(window, 0x541, 0x4D1, 1, 0);
        wchar_t *name = Narrow_To_Wide_006fa180(unknown_name_a3d3a0);
        HWND edit = GetDlgItem(window, 0x541);
        SendMessageA(edit, 0x4B2, 0, (LPARAM)name);
        unknown_map_name_a3d342[0] = 0;
        unknown_a3d3b4 = unknown_session_a3d298.unknown_12c;
        unknown_session_a3d298.unknown_120 = unknown_session_a3d298.unknown_130;
        Free_Pointers_005a7480(&unknown_games_a3fa64);
        Free_Pointers_005a7480(&unknown_players_a3fa7c);
        Free_Pointers_005a7480(&unknown_chat_a3fa4c);
        NodeNameType *node = new NodeNameType;
        wcscpy(node->Name, Narrow_To_Wide_006fa180(unknown_name_a3d3a0));
        node->Chat.LastTime = 0;
        node->Chat.LastChance = 0;
        node->Chat.Color = unknown_gpacket_a3f640.Chat.Color;
        unknown_chat_a3fa4c.Add(node);
        node = new NodeNameType;
        wcscpy(node->Name, L"");
        node->Game.IsOpen = 0;
        node->Game.LastTime = 0;
        unknown_games_a3fa64.Add(node);
        Unknown_005c01d0(1, 0, 1, 1);
        Refresh_Game_List_005bc790();
        Unknown_005bbdd0(false);
        Unknown_005FEE70(window);
        SetFocus(GetDlgItem(window, 0x540));
        break;
    }
    case 0x4DB:
        Unknown_005def80(lparam, wparam != 0);
        return 0;
    case 0x4EC:
        Unknown_005e8710(window);
        return 0;
    }
    return 0;
}


// void __fastcall Send_Chat_Message_005bee20(HWND dialog, int color, int unknown) (0x005BEE20)

void __fastcall Send_Chat_Message_005bee20(HWND dialog, int color, int unknown)
{
    wchar_t text[112];

    HWND edit = GetDlgItem(dialog, 0x540);
    SendMessageA(edit, 0x4B3, 0x6F, (LPARAM)text);
    text[0x6F] = 0;
    int length = wcslen(text);
    edit = GetDlgItem(dialog, 0x540);
    SendMessageA(edit, 0x4B2, 0, (LPARAM)L"");
    InvalidateRect(dialog, 0, 1);
    if (length > 2) {
        Unknown_005C2C80(color, L"[%s] %s", Narrow_To_Wide_006fa180(unknown_name_a3d3a0), text);
        UnknownGlobalPacket packet;
        memset(&packet, 0, sizeof(packet));
        packet.Command = 12;
        strcpy(packet.Name, unknown_name_a3d3a0);
        wcscpy(packet.Message.Buf, text);
        packet.Message.unknown_110 = unknown;
        packet.Message.unknown_114 = Unknown_005bba50(unknown_host_name_a3f618);
        if (unknown_join_state_a71e94 == 2) {
            int count = unknown_players_a3fa7c.Count();
            for (int i = 1; i < count; i++) {
                unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 1, &unknown_players_a3fa7c[i]->Address, 0, 0);
                Service_Game_IO_00481fb0();
            }
        } else {
            int count = unknown_chat_a3fa4c.Count();
            for (int i = 1; i < count; i++) {
                unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 1, &unknown_chat_a3fa4c[i]->Address, 0, 0);
                Service_Game_IO_00481fb0();
            }
        }
    }
}


// Not exact (RA2_NON_MATCHING): Host_Dialog_Proc_005befe0 at 0x005BEFE0, best score 85.37, 2026-11-13; differs: 756 instructions in the original, 755 in the draft (shape 96.23)
#ifdef RA2_NON_MATCHING
// long __stdcall Host_Dialog_Proc_005befe0(HWND window, unsigned message, unsigned wparam, long lparam) (0x005BEFE0)
// The LAN host dialog's procedure (from Red Alert's Net_New_Dialog).
void *Native_Entry_From_Handle(int handle);   // NATIVE: native/link/native_event_data.cpp
#include "shared/random2.hpp"

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
#endif

// long __stdcall Options_Dialog_Proc_005bf9b0(HWND window, unsigned message, unsigned wparam, long lparam) (0x005BF9B0)

long __stdcall Options_Dialog_Proc_005bf9b0(HWND window, unsigned message, unsigned wparam, long lparam)
{
    if (!unknown_lock_a71e98) {
        switch (message) {
        case 0x497:
            Unknown_Fill_Options_Dialog_005c3550(window, 1);
            return 0;
        case 0x114:
            if (GetDlgItem(window, 0x529) == (HWND)lparam) {
                unknown_session_a3d298.GameSpeed_30 = 6 - SendDlgItemMessageA(window, 0x529, 0x400, 0, 0);
            }
            if (GetDlgItem(window, 0x402) == (HWND)lparam) {
                unknown_session_a3d298.unknown_24 = SendDlgItemMessageA(window, 0x402, 0x400, 0, 0);
            }
            if (GetDlgItem(window, 0x54F) == (HWND)lparam) {
                unknown_session_a3d298.UnitCount = SendDlgItemMessageA(window, 0x54F, 0x400, 0, 0);
            }
            return 0;
        case 0x111:
            switch ((unsigned short)wparam) {
            case 0x54E:
                unknown_session_a3d298.unknown_2a = false;
                if (SendDlgItemMessageA(window, 0x54E, 0xF0, 0, 0) == 1) {
                    unknown_session_a3d298.unknown_2a = true;
                    if (!unknown_session_a3d298.unknown_bases_20) {
                        unknown_session_a3d298.unknown_bases_20 = true;
                        SendDlgItemMessageA(window, 0x54B, 0xF1, 1, 0);
                    }
                }
                return 0;
            case 0x54D:
                unknown_session_a3d298.unknown_crates_29 = false;
                if (SendDlgItemMessageA(window, 0x54D, 0xF0, 0, 0) == 1) {
                    unknown_session_a3d298.unknown_crates_29 = true;
                }
                return 0;
            case 0x3E8:
                unknown_session_a3d298.unknown_a8 = false;
                if (SendDlgItemMessageA(window, 0x3E8, 0xF0, 0, 0) == 1) {
                    unknown_session_a3d298.unknown_a8 = true;
                }
                return 0;
            case 0x624:
                unknown_session_a3d298.unknown_28 = false;
                if (SendDlgItemMessageA(window, 0x624, 0xF0, 0, 0) == 1) {
                    unknown_session_a3d298.unknown_28 = true;
                }
                return 0;
            case 0x69D:
                unknown_session_a3d298.unknown_2c = false;
                if (SendDlgItemMessageA(window, 0x69D, 0xF0, 0, 0) == 1) {
                    unknown_session_a3d298.unknown_2c = true;
                }
                return 0;
            case 0x69A:
                unknown_session_a3d298.unknown_2b = false;
                if (SendDlgItemMessageA(window, 0x69A, 0xF0, 0, 0) == 1) {
                    unknown_session_a3d298.unknown_2b = true;
                }
                return 0;
            }
        }
    }
    return 0;
}


// int __fastcall Request_To_Join_005bfc40(int game, bool unknown) (0x005BFC40)

int __fastcall Request_To_Join_005bfc40(int game, bool unknown)
{
    if (game < 1) {
        Unknown_005C2C80(unknown_color_7a7360, StringTable::Load_String("TXT_MUST_SELECT_GAME", 0, "D:\\Ra2\\netdlg2.cpp", 3423));
        Voc_Play(Rules->SystemError, 1.0f, 0x2000, 0);
        return 0;
    }
    if (unknown_games_a3fa64.Count() <= 1 || game > unknown_games_a3fa64.Count()) {
        Unknown_005C2C80(unknown_color_7a7360, StringTable::Load_String("TXT_NOTHING_TO_JOIN", 0, "D:\\Ra2\\netdlg2.cpp", 3430));
        Voc_Play(Rules->SystemError, 1.0f, 0x2000, 0);
        return 0;
    }
    if (strlen(unknown_name_a3d3a0) == 0) {
        Unknown_005C2C80(unknown_color_7a7360, StringTable::Load_String("TXT_NAME_ERROR", 0, "D:\\Ra2\\netdlg2.cpp", 3438));
        Voc_Play(Rules->SystemError, 1.0f, 0x2000, 0);
        return 0;
    }
    if (strcmp(unknown_name_a3d3a0, Wide_To_Narrow_006fa120(unknown_lobby_texts_a72d0c[0])) == 0
        || strcmp(unknown_name_a3d3a0, Wide_To_Narrow_006fa120(unknown_lobby_texts_a72d0c[1])) == 0
        || strcmp(unknown_name_a3d3a0, Wide_To_Narrow_006fa120(unknown_lobby_texts_a72d0c[2])) == 0
        || strcmp(unknown_name_a3d3a0, Wide_To_Narrow_006fa120(unknown_lobby_texts_a72d0c[3])) == 0
        || strcmp(unknown_name_a3d3a0, Wide_To_Narrow_006fa120(unknown_lobby_texts_a72d0c[4])) == 0
        || strcmp(unknown_name_a3d3a0, Wide_To_Narrow_006fa120(unknown_lobby_texts_a72d0c[5])) == 0
        || strcmp(unknown_name_a3d3a0, Wide_To_Narrow_006fa120(unknown_lobby_texts_a72d0c[6])) == 0
        || Unknown_Is_Lobby_Name_005cd2f0(unknown_name_a3d3a0)) {
        Unknown_005C2C80(unknown_color_7a7360, StringTable::Load_String("ERROR:NameIsReservedWord", 0, "D:\\Ra2\\netdlg2.cpp", 3453));
        Voc_Play(Rules->SystemError, 1.0f, 0x2000, 0);
        return 0;
    }
    if (!unknown_games_a3fa64[game]->Game.IsOpen) {
        Unknown_005C2C80(unknown_color_7a7360, StringTable::Load_String("TXT_GAME_IS_CLOSED", 0, "D:\\Ra2\\netdlg2.cpp", 3461));
        Voc_Play(Rules->SystemError, 1.0f, 0x2000, 0);
        return 0;
    }
    UnknownGlobalPacket packet;
    memset(&packet, 0, sizeof(packet));
    packet.Command = 6;
    strcpy(packet.Name, unknown_name_a3d3a0);
    strcpy(packet.unknown_18, unknown_a71bc4);
    packet.Join.unknown_2f = unknown_session_a3d298.Value_0124_006729f0();
    packet.Join.unknown_33 = unknown_session_a3d298.Value_011c_00672a10();
    packet.Join.unknown_3b = VerNum.Min_Version();
    packet.Join.unknown_3f = VerNum.Max_Version();
    packet.Join.unknown_43 = unknown_a71e9c;
    packet.Join.unknown_4b = unknown_a71ea0;
    packet.Join.unknown_47 = unknown_a71ea4;
    packet.Join.unknown_4f = Unknown_00737af0();
    packet.Join.unknown_57 = unknown;
    unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 1, &unknown_games_a3fa64[game]->Address, 0, 0);
    return 1;
}


// void __fastcall Unjoin_Game_005c00b0(int game) (0x005C00B0)

void __fastcall Unjoin_Game_005c00b0(int game)
{
    UnknownGlobalPacket packet;
    memset(&packet, 0, sizeof(packet));
    packet.Command = 10;
    strcpy(packet.Name, unknown_name_a3d3a0);
    for (int i = 1; i < unknown_players_a3fa7c.Count(); i++) {
        unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 1, &unknown_players_a3fa7c[i]->Address, 0, 0);
        Service_Game_IO_00481fb0();
    }
    if (unknown_join_state_a71e94 == 1 || unknown_join_state_a71e94 == 2) {
        if (!unknown_games_a3fa64[game]->Address.Is_Broadcast()) {
            unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 1, &unknown_games_a3fa64[game]->Address, 0, 0);
        }
    }
    unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 0, 0, 0, 0);
    Clear_Slot_Lists_005c8a70();
    Fill_Slots_005c8a10();
    Free_Pointers_005a7480(&unknown_players_a3fa7c);
    unknown_host_name_a3f618[0] = 0;
}


// void __fastcall Unknown_005c01d0(int gamenow, int playernow, int chatnow, int init) (0x005C01D0)
// Red Alert's Send_Join_Queries: the game query every 120 ticks, the player query of the selected game every 35
// and the chat announcement every 83 (function-local timers; their atexit destructors are 0x5C05E0, 0x5C05D0
// and 0x5C05C0; the $E/$S numbers are this unit's). Session.Handle and Session.Games are read
// through the session object (Games is NodeList_27cc): the alias forms swap the Games[CurGame] load order.

void __fastcall Unknown_005c01d0(int gamenow, int playernow, int chatnow, int init)
{
    static CDTimerClass<SystemTimerClass> game_timer;
    static CDTimerClass<SystemTimerClass> player_timer;
    static CDTimerClass<SystemTimerClass> chat_timer;
    UnknownGlobalPacket packet;

    if (init) {
        game_timer = 120;
        player_timer = 35;
        chat_timer = 83;
    }

    if (!game_timer || gamenow) {
        game_timer = 120;
        if (Current_Dialog_Id_00735b20() != 0xBC || gamenow) {
            memset(&packet, 0, sizeof(packet));
            packet.Command = 0;
            strcpy(packet.Name, unknown_session_a3d298.Handle);
            unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 0, 0, 0, 0);
            if (unknown_dest_net_set_a3f604) {
                unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 0, &unknown_dest_net_a3f608, 0, 0);
            }
        }
    }

    if ((unknown_game_index_a71e90 > 0 && unknown_game_index_a71e90 < unknown_games_a3fa64.Count() && !player_timer) || playernow) {
        player_timer = 35;
        memset(&packet, 0, sizeof(packet));
        packet.Command = 2;
        strcpy(packet.Name, Wide_To_Narrow_006fa120(unknown_session_a3d298.NodeList_27cc[unknown_game_index_a71e90]->Name));
        unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 0, 0, 0, 0);
        if (unknown_dest_net_set_a3f604) {
            unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 0, &unknown_dest_net_a3f608, 0, 0);
        }
    }

    if ((!chat_timer && unknown_join_state_a71e94 < 2) || chatnow) {
        chat_timer = 83;
        memset(&packet, 0, sizeof(packet));
        packet.Command = 4;
        strcpy(packet.Name, unknown_session_a3d298.Handle);
        packet.Chat.ID = unknown_session_a3d298.UniqueID;
        packet.Chat.Color = unknown_a3d3b4;
        unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 0, 0, 0, 0);
        if (unknown_dest_net_set_a3f604) {
            unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 0, &unknown_dest_net_a3f608, 0, 0);
        }
    }
}


// Send_Join_Queries chat_timer exit thunk (0x005C05C0)
// Compiler-emitted atexit destructor of the function-local CDTimerClass<SystemTimerClass> chat_timer in
// netdlg2/Unknown_005c01d0.cpp; no source of its own.
// @symbol _$E31


// Send_Join_Queries player_timer exit thunk (0x005C05D0)
// Compiler-emitted atexit destructor of the function-local CDTimerClass<SystemTimerClass> player_timer in
// netdlg2/Unknown_005c01d0.cpp; no source of its own.
// @symbol _$E30


// Send_Join_Queries game_timer exit thunk (0x005C05E0)
// Compiler-emitted atexit destructor of the function-local CDTimerClass<SystemTimerClass> game_timer in
// netdlg2/Unknown_005c01d0.cpp; no source of its own.
// @symbol _$E29


// bool __fastcall Answer_Query_005c05f0(UnknownGlobalPacket *packet, IPXAddressClass *address) (0x005C05F0)

bool __fastcall Answer_Query_005c05f0(UnknownGlobalPacket *packet, IPXAddressClass *address)
{
    if (packet->Command == 0 && !unknown_session_a3f614) {
        if (wcslen(unknown_host_name_a3f618) > 0) {
            if (!unknown_session_a3f616 || Compare_Narrow_00781f20(unknown_name_a3d3a0, Wide_To_Narrow_006fa120(unknown_host_name_a3f618)) == 0) {
                if (Compare_Narrow_00781f20(unknown_name_a3d3a0, Wide_To_Narrow_006fa120(unknown_host_name_a3f618)) != 0 && !unknown_session_a40d04) {
                    return true;
                }
                UnknownGlobalPacket answer;
                memset(&answer, 0, sizeof(answer));
                answer.Command = 1;
                strcpy(answer.Name, Wide_To_Narrow_006fa120(unknown_host_name_a3f618));
                answer.GameInfo.IsOpen = unknown_session_a3f616;
                unknown_ipx_manager_a40978.Send_Global_Message(&answer, sizeof(answer), 1, address, 0, 0);
            }
        }
        return true;
    }
    if (packet->Command == 2) {
        if (Compare_Narrow_00781f20(packet->Name, Wide_To_Narrow_006fa120(unknown_host_name_a3f618)) == 0
            && wcslen(unknown_host_name_a3f618) > 0 && !unknown_session_a3f614 && unknown_join_state_a71e94 >= 2) {
            UnknownGlobalPacket answer;
            memset(&answer, 0, sizeof(answer));
            answer.Command = 3;
            strcpy(answer.Name, unknown_name_a3d3a0);
            strcpy(answer.unknown_18, unknown_a71bc4);
            answer.PlayerInfo.unknown_2f = unknown_session_a3d298.Value_0124_006729f0();
            answer.PlayerInfo.unknown_33 = unknown_session_a3d298.Value_011c_00672a10();
            answer.PlayerInfo.unknown_37 = Unknown_005bba50(unknown_host_name_a3f618);
            answer.PlayerInfo.Slot = unknown_lobby_a3fa94;
            answer.PlayerInfo.Observer = unknown_lobby_a3fa94 == unknown_lobby_flag_7e6d90;
            if (unknown_players_a3fa7c.Count() == 0) {
                return true;
            }
            if (unknown_players_a3fa7c[0]->Team != -1) {
                answer.PlayerInfo.Observer = 0;
            }
            unknown_ipx_manager_a40978.Send_Global_Message(&answer, sizeof(answer), 1, address, 0, 0);
            if (Compare_Narrow_00781f20(packet->Name, Wide_To_Narrow_006fa120(unknown_players_a3fa7c[0]->Name)) == 0) {
                Unknown_005c3810(true, true);
                if (unknown_session_a3d29c) {
                    unknown_session_a3d29c->unknown_slot20();
                }
            }
            return true;
        }
    }
    return false;
}


// void Get_Join_Responses_005c08a0() (0x005C08A0): the join dialog's packet pump (TibSun netdlg2.cpp
// Get_Join_Responses): reads every global packet while no dialog result is pending, answers queries,
// tracks games, players and chat users, follows the join handshake and the game options, and
// refreshes the user and game lists at the end. __fastcall, as Net_Join_Dialog 0x005BD070 declares it
// (cdecl scores lower). Matching levers: the commands form one if/else-if chain (separate ifs ending in
// continue change the constant registers: 2 in edi, 8 in ebx); the inlined DynamicVectorClass Adds go
// through the session members (NodeList_27cc, Players, NodeList_27b4); the NET_ANSWER_GAME IsOpen store
// reads the packet through unknown_session_a3d298.GPacket, so it is not shared with the compare's load;
// the serial loop keeps TibSun's dups count (now unused); NET_MESSAGE reads the colour once.

extern int unknown_global_length_a3f808;
extern unsigned short unknown_global_product_a3f818;
// Seats a player entry in a lobby slot (0x005C8CC0).
void __fastcall Set_Player_Slot_005c8cc0(int slot, NodeNameType *player);
// Seats the first session player (the host) in a slot and makes it the current slot (0x005C8CD0).
void __fastcall Seat_First_Player_005c8cd0(int slot);
// Applies the host's public game options (TibSun DecodePubGameopt; 0x005C3FF0).
bool __fastcall Decode_Pub_Gameopt_005c3ff0(char const *options, char const *name);
// Enables the start control and clears the guests' ready flags (0x005C9700).
void Unknown_Enable_Start_005c9700();
// Frees the slot of a player that left (0x005C91C0).
void __fastcall Unknown_Release_Slot_005c91c0(IPXAddressClass &address);
extern unsigned long unknown_version_a3d8d4;
extern "C" char *__cdecl _strdup(char const *string);
extern "C" char *__cdecl strtok(char *string, char const *delimiters);
extern "C" long __cdecl atol(char const *string);
extern "C" void __cdecl free(void *block);

void __fastcall Get_Join_Responses_005c08a0()
{
    NodeNameType *who;
    int i;
    int found;
    int why;
    wchar_t txt[80];
    bool display_users = false;
    bool display_games = false;

    while (unknown_lobby_result_a71e8c == 0) {
        Service_Game_IO_00481fb0();
        if (!unknown_ipx_manager_a40978.Get_Global_Message(&unknown_gpacket_a3f640, &unknown_global_length_a3f808, &unknown_address_a3f80c, &unknown_global_product_a3f818)) {
            break;
        }
        if (unknown_global_product_a3f818 != 0xAA03) {
            continue;
        }
        if (Answer_Query_005c05f0(&unknown_gpacket_a3f640, &unknown_address_a3f80c)) {
            continue;
        }

        // The commands of TibSun's Get_Join_Responses, in its order (RA2 adds the map preview,
        // slot and observer packets).
        if (unknown_gpacket_a3f640.Command == 0x19) {
            if (Current_Dialog_Id_00735b20() == 0xBD) {
                Download_Map_Preview_005c7970();
            }
        } else if (unknown_gpacket_a3f640.Command == 0x1B) {
            if (Current_Dialog_Id_00735b20() == 0xBC) {
                Unknown_Enable_Start_005c9700();
                display_users = true;
                unknown_dialog_preview_a72ce8->unknown_005c7ef0();
            }
        } else if (unknown_gpacket_a3f640.Command == 1) {
            // NET_ANSWER_GAME
            found = 0;
            for (i = 1; i < unknown_games_a3fa64.Count(); i++) {
                if (!_stricmp(Wide_To_Narrow_006fa120(unknown_games_a3fa64[i]->Name), unknown_gpacket_a3f640.Name)) {
                    found = 1;
                    unknown_games_a3fa64[i]->Game.LastTime = TickCount.Time();
                    if (unknown_games_a3fa64[i]->Game.IsOpen != unknown_gpacket_a3f640.GameInfo.IsOpen) {
                        display_games = true;
                        unknown_games_a3fa64[i]->Game.IsOpen = unknown_session_a3d298.GPacket.GameInfo.IsOpen;
                        if (unknown_games_a3fa64[i]->Game.IsOpen) {
                            unknown_games_a3fa64[i]->Address = unknown_address_a3f80c;
                        }
                        if (unknown_join_state_a71e94 < 2) {
                            if (unknown_games_a3fa64[i]->Game.IsOpen) {
                                swprintf(txt, StringTable::Load_String("TXT_S_FORMED_NEW_GAME", 0, "D:\\Ra2\\netdlg2.cpp", 0xF64), Narrow_To_Wide_006fa180(unknown_gpacket_a3f640.Name));
                                Voc_Play(Rules->GameForming, 1.0f, 0x2000, 0);
                            } else {
                                swprintf(txt, StringTable::Load_String("TXT_GAME_NOW_IN_PROGRESS", 0, "D:\\Ra2\\netdlg2.cpp", 0xF69), Narrow_To_Wide_006fa180(unknown_gpacket_a3f640.Name));
                                Voc_Play(Rules->GameClosed, 1.0f, 0x2000, 0);
                            }
                            Unknown_005C2C80(unknown_color_7a7360, txt);
                        }
                    }
                    break;
                }
            }
            if (found == 0) {
                who = new NodeNameType;
                wcscpy(who->Name, Narrow_To_Wide_006fa180(unknown_gpacket_a3f640.Name));
                who->Address = unknown_address_a3f80c;
                who->Game.IsOpen = unknown_gpacket_a3f640.GameInfo.IsOpen;
                who->Game.LastTime = TickCount.Time();
                unknown_session_a3d298.NodeList_27cc.Add(who);
                for (i = 1; i < unknown_chat_a3fa4c.Count(); i++) {
                    if (unknown_chat_a3fa4c[i]->Address == unknown_address_a3f80c) {
                        delete unknown_chat_a3fa4c[i];
                        unknown_chat_a3fa4c.Delete(unknown_chat_a3fa4c[i]);
                        break;
                    }
                }
                if (unknown_gpacket_a3f640.GameInfo.IsOpen && unknown_join_state_a71e94 < 2) {
                    swprintf(txt, StringTable::Load_String("TXT_S_FORMED_NEW_GAME", 0, "D:\\Ra2\\netdlg2.cpp", 0xF92), Narrow_To_Wide_006fa180(unknown_gpacket_a3f640.Name));
                    Unknown_005C2C80(unknown_color_7a7360, txt);
                    Voc_Play(Rules->GameForming, 1.0f, 0x2000, 0);
                }
                display_users = true;
                display_games = true;
            }
        } else if (unknown_gpacket_a3f640.Command == 3) {
            // NET_ANSWER_PLAYER
            found = 0;
            for (i = 0; i < unknown_players_a3fa7c.Count(); i++) {
                if (unknown_players_a3fa7c[i]->Address == unknown_address_a3f80c) {
                    found = 1;
                    break;
                }
            }
            if (unknown_games_a3fa64.Count() && unknown_gpacket_a3f640.PlayerInfo.unknown_37 != (int)Compute_Name_CRC(unknown_host_name_a3f618)) {
                found = 1;
            }
            if (!_stricmp(unknown_name_a3d3a0, unknown_gpacket_a3f640.Name)) {
                found = 1;
            }
            if (found == 0) {
                who = new NodeNameType;
                memset(who, 0, sizeof(*who));
                wcscpy(who->Name, Narrow_To_Wide_006fa180(unknown_gpacket_a3f640.Name));
                strcpy(who->unknown_34, unknown_gpacket_a3f640.unknown_18);
                who->Address = unknown_address_a3f80c;
                who->unknown_66e2d0(unknown_gpacket_a3f640.PlayerInfo.unknown_2f);
                who->unknown_66e300(unknown_gpacket_a3f640.PlayerInfo.unknown_33);
                if (unknown_gpacket_a3f640.PlayerInfo.Observer) {
                    who->Team = -1;
                }
                unknown_session_a3d298.Players.Add(who);
                Set_Player_Slot_005c8cc0(unknown_gpacket_a3f640.PlayerInfo.Slot, who);
                for (i = 1; i < unknown_chat_a3fa4c.Count(); i++) {
                    if (unknown_chat_a3fa4c[i]->Address == unknown_address_a3f80c) {
                        delete unknown_chat_a3fa4c[i];
                        unknown_chat_a3fa4c.Delete(unknown_chat_a3fa4c[i]);
                        break;
                    }
                }
                Unknown_Enable_Start_005c9700();
                HWND dialog = WS_Find_Dialog_00735940(0xBD);
                Unknown_005cd030(dialog, true);
                Debug_Printf("New Player");
                display_users = true;
                if (unknown_join_state_a71e94 >= 2) {
                    Voc_Play(Rules->PlayerJoined, 1.0f, 0x2000, 0);
                }
            }
        } else if (unknown_gpacket_a3f640.Command == 0x16) {
            if (!Is_Host_005c86a0()) {
                Set_Slot_Names_005c8dd0(unknown_gpacket_a3f640.Slots.Names);
                display_users = true;
            }
        } else if (unknown_gpacket_a3f640.Command == 0x28) {
            if (!Is_Host_005c86a0()) {
                Unknown_005c3c20();
            }
        } else if (unknown_gpacket_a3f640.Command == 7) {
            // NET_CONFIRM_JOIN
            if (unknown_join_state_a71e94 != 2) {
                wcscpy(unknown_host_name_a3f618, Narrow_To_Wide_006fa180(unknown_gpacket_a3f640.Name));
                unknown_session_a3d298.Set_Value_00672a30(unknown_gpacket_a3f640.PlayerInfo.unknown_2f, false);
                unknown_session_a3d298.Set_Value_00672ae0(unknown_gpacket_a3f640.PlayerInfo.unknown_33, false);
                unknown_join_state_a71e94 = 2;
                unknown_address_a3db2c = unknown_address_a3f80c;
                Free_Pointers_005a7480(&unknown_players_a3fa7c);
                who = new NodeNameType;
                memset(who, 0, sizeof(*who));
                wcscpy(who->Name, Narrow_To_Wide_006fa180(unknown_name_a3d3a0));
                who->unknown_66e2d0(unknown_session_a3d298.Value_0124_006729f0());
                who->unknown_66e300(unknown_session_a3d298.Value_011c_00672a10());
                unknown_lobby_flag_7e6d90 = unknown_gpacket_a3f640.PlayerInfo.Observer;
                if (unknown_gpacket_a3f640.PlayerInfo.Slot == unknown_gpacket_a3f640.PlayerInfo.Observer && unknown_gpacket_a3f640.PlayerInfo.unknown_2f == -3) {
                    who->Team = -1;
                }
                unknown_session_a3d298.Players.Add(who);
                unknown_join_list_7e699c = false;
                End_Dialog_007357e0(0, 0);
                unknown_lobby_result_a71e8c = 0;
                Clear_Slot_Lists_005c8a70();
                Unknown_Reset_Slots_005c8a90();
                Seat_First_Player_005c8cd0(unknown_gpacket_a3f640.PlayerInfo.Slot);
                HWND dialog = (HWND)unknown_create_dialog_735700(ProgramInstance, 0xBD, MainWindow, Join_Lobby_Dialog_Proc_005c2330, 0);
                unknown_show_dialog_5ff7e0(dialog, 0xBD);
                ShowWindow(dialog, 1);
                display_users = true;
                Unknown_005c01d0(1, 1, 1, 0);
            }
        } else if (unknown_gpacket_a3f640.Command == 8) {
            // NET_REJECT_JOIN
            why = 0;
            if (unknown_join_state_a71e94 == 2) {
                UnknownGlobalPacket packet;
                memset(&packet, 0, sizeof(packet));
                packet.Command = 10;
                strcpy(packet.Name, unknown_name_a3d3a0);
                for (i = 1; i < unknown_players_a3fa7c.Count(); i++) {
                    unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 1, &unknown_players_a3fa7c[i]->Address, 0, 0);
                    Service_Game_IO_00481fb0();
                }
                unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 0, 0, 0, 0);
                if (unknown_dest_net_set_a3f604) {
                    unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 0, &unknown_dest_net_a3f608, 0, 0);
                }
                while (unknown_ipx_manager_a40978.Global_Num_Send() > 0 && unknown_ipx_manager_a40978.Service() != 0) {
                    Service_Game_IO_00481fb0();
                }
                unknown_host_name_a3f618[0] = 0;
                Clear_Slot_Lists_005c8a70();
                Fill_Slots_005c8a10();
                Free_Pointers_005a7480(&unknown_players_a3fa7c);
                Free_Pointers_005a7480(&unknown_chat_a3fa4c);
                unknown_game_index_a71e90 = 0;
                unknown_join_list_7e699c = true;
                unknown_join_state_a71e94 = -1;
                why = 4;
                display_users = true;
            } else if (unknown_join_state_a71e94 == 1) {
                why = unknown_gpacket_a3f640.Reject.Why;
                unknown_join_state_a71e94 = -1;
            }
            if (unknown_join_state_a71e94 == -1) {
                Unknown_005C2C80(unknown_color_7a7360, StringTable::Load_String("TXT_REQUEST_DENIED", 0, "D:\\Ra2\\netdlg2.cpp", 0x108F));
                Voc_Play(Rules->SystemError, 1.0f, 0x2000, 0);
                wchar_t const *item = 0;
                if (why == 0) {
                    item = StringTable::Load_String("TXT_NAME_MUSTBE_UNIQUE", 0, "D:\\Ra2\\netdlg2.cpp", 0x1095);
                } else if (why == 1) {
                    if (unknown_a72c68 == 0) {
                        item = StringTable::Load_String("TXT_GAME_FULL", 0, "D:\\Ra2\\netdlg2.cpp", 0x1097);
                    } else {
                        item = StringTable::Load_String("TXT_OBSERVER_FULL", 0, "D:\\Ra2\\netdlg2.cpp", 0x1099);
                    }
                } else if (why == 2) {
                    item = StringTable::Load_String("TXT_YOURGAME_OUTDATED", 0, "D:\\Ra2\\netdlg2.cpp", 0x109B);
                } else if (why == 3) {
                    item = StringTable::Load_String("TXT_DESTGAME_OUTDATED", 0, "D:\\Ra2\\netdlg2.cpp", 0x109D);
                } else if (why == 6) {
                    item = StringTable::Load_String("TXT_MISMATCH", 0, "D:\\Ra2\\netdlg2.cpp", 0x109F);
                } else if (why == 5) {
                    item = StringTable::Load_String("TXT_GAME_CANCELLED", 0, "D:\\Ra2\\netdlg2.cpp", 0x10A1);
                } else if (why == 7) {
                    item = StringTable::Load_String("TXT_SERIAL_DUP", 0, "D:\\Ra2\\netdlg2.cpp", 0x10A3);
                }
                if (item) {
                    Message_Box_005c3230(item, 0, Unknown_005bbd10, false);
                }
                if (Current_Dialog_Id_00735b20() == 0xBB) {
                    HWND window = Unknown_00735B10();
                    if (window) {
                        EnableWindow(window, TRUE);
                        HWND button = GetDlgItem(window, 0x541);
                        if (button) {
                            EnableWindow(button, TRUE);
                        }
                    }
                } else {
                    unknown_lobby_result_a71e8c = 2;
                }
                Unknown_005c01d0(0, 0, 1, 0);
            }
        } else if (unknown_gpacket_a3f640.Command == 0x17) {
            // NET_PUB_GAMEOPT
            for (i = 0; i < unknown_players_a3fa7c.Count(); i++) {
                if (!_stricmp(Wide_To_Narrow_006fa120(unknown_players_a3fa7c[i]->Name), unknown_gpacket_a3f640.Name)) {
                    if (i != -1 && !unknown_lock_a71e98) {
                        Decode_Pub_Gameopt_005c3ff0(&unknown_gpacket_a3f640.unknown_2f[0], unknown_gpacket_a3f640.Name);
                    }
                    break;
                }
            }
        } else if (unknown_gpacket_a3f640.Command == 0x18) {
            // NET_PRIV_GAMEOPT
            char *opts = _strdup(&unknown_gpacket_a3f640.unknown_2f[1]);
            for (i = 0; i < 8; i++) {
                char *name = Wide_To_Narrow_006fa120(unknown_player_slots_a3fa98[i]->Name);
                if (!_stricmp(name, unknown_gpacket_a3f640.Name)) {
                    if (i != -1 && !unknown_lock_a71e98) {
                        char *tok = strtok(opts, ",");
                        int newhouse = 0;
                        if (tok) {
                            newhouse = atol(tok);
                        }
                        tok = strtok(0, ",");
                        int newcolor = 0;
                        if (tok) {
                            newcolor = atol(tok);
                        }
                        HWND dialog = WS_Find_Dialog_00735940(0xBC);
                        if (!dialog) {
                            dialog = WS_Find_Dialog_00735940(0xBD);
                        }
                        bool house_changed = unknown_player_slots_a3fa98[i]->unknown_66e290() != newhouse;
                        if (house_changed) {
                            unknown_player_slots_a3fa98[i]->unknown_66e2d0(newhouse);
                        }
                        bool color_changed = unknown_player_slots_a3fa98[i]->Get_Color_0066e2a0() != newcolor;
                        if (color_changed) {
                            if (unknown_session_a3d298.Unknown_006728D0(newcolor)) {
                                newcolor = -2;
                            }
                            unknown_player_slots_a3fa98[i]->unknown_66e300(newcolor);
                        }
                        if (house_changed || color_changed) {
                            display_users = true;
                            Unknown_005c3810(true, false);
                        }
                    }
                    break;
                }
            }
            free(opts);
        } else if (unknown_gpacket_a3f640.Command == 10) {
            // NET_SIGN_OFF
            for (i = 1; i < unknown_games_a3fa64.Count(); i++) {
                if (!_stricmp(Wide_To_Narrow_006fa120(unknown_games_a3fa64[i]->Name), unknown_gpacket_a3f640.Name) && unknown_games_a3fa64[i]->Address == unknown_address_a3f80c) {
                    if (i == unknown_game_index_a71e90) {
                        Clear_Slot_Lists_005c8a70();
                        Fill_Slots_005c8a10();
                        Free_Pointers_005a7480(&unknown_players_a3fa7c);
                        if (Current_Dialog_Id_00735b20() != 0xBB && Current_Dialog_Id_00735b20() == 0xBD) {
                            unknown_lobby_result_a71e8c = 2;
                        }
                    }
                    if (unknown_join_state_a71e94 > 0 && i == unknown_game_index_a71e90) {
                        unknown_join_state_a71e94 = -1;
                        why = 5;
                    }
                    delete unknown_games_a3fa64[i];
                    unknown_games_a3fa64.Delete(unknown_games_a3fa64[i]);
                    unknown_game_index_a71e90 = i - 1;
                    display_games = true;
                    Unknown_005c01d0(0, 1, 1, 0);
                }
            }
            bool player_removed = false;
            Unknown_Release_Slot_005c91c0(unknown_address_a3f80c);
            for (i = 0; i < unknown_players_a3fa7c.Count(); i++) {
                if (unknown_players_a3fa7c[i]->Address == unknown_address_a3f80c) {
                    delete unknown_players_a3fa7c[i];
                    unknown_players_a3fa7c.Delete(unknown_players_a3fa7c[i]);
                    player_removed = true;
                    if (unknown_join_state_a71e94 >= 2) {
                        Voc_Play(Rules->PlayerLeft, 1.0f, 0x2000, 0);
                    }
                }
            }
            for (i = 1; i < unknown_chat_a3fa4c.Count(); i++) {
                if (unknown_chat_a3fa4c[i]->Address == unknown_address_a3f80c) {
                    delete unknown_chat_a3fa4c[i];
                    unknown_chat_a3fa4c.Delete(unknown_chat_a3fa4c[i]);
                }
            }
            if (player_removed) {
                Unknown_Enable_Start_005c9700();
                HWND dialog = WS_Find_Dialog_00735940(0xBD);
                Unknown_005cd030(dialog, true);
                if (Current_Dialog_Id_00735b20() == 0x6B) {
                    HWND window = Unknown_00735B10();
                    int index = SendDlgItemMessageA(window, 0x6EB, 0x188, 0, 0);
                    if (index != -1) {
                        MultiplayerGameMode *row = (MultiplayerGameMode *)SendDlgItemMessageA(window, 0x6EB, 0x199, index, 0);
                        row->unknown_slot30();
                    }
                } else {
                    unknown_session_a3d298.Mode->unknown_slot30();
                }
            }
            display_users = true;
        } else if (unknown_gpacket_a3f640.Command == 0xB || unknown_gpacket_a3f640.Command == 0xE) {
            // NET_GO, NET_LOADGAME
            if (unknown_join_state_a71e94 == 2) {
                unknown_max_ahead_a3d560 = unknown_gpacket_a3f640.Progress.Percent;
                unknown_address_a3db2c = unknown_address_a3f80c;
                unknown_session_a3d55c = unknown_players_a3fa7c.Count();
                unknown_lobby_result_a71e8c = 1;
                if (unknown_gpacket_a3f640.Command == 0xB) {
                    unknown_join_state_a71e94 = 3;
                } else if (unknown_gpacket_a3f640.Command == 0xE) {
                    unknown_join_state_a71e94 = 4;
                }
            }
        } else if (unknown_gpacket_a3f640.Command == 4) {
            // NET_CHAT_ANNOUNCE
            if (unknown_gpacket_a3f640.Chat.ID == unknown_session_a3d298.UniqueID) {
                continue;
            }
            found = 0;
            for (i = 0; i < unknown_chat_a3fa4c.Count(); i++) {
                if (unknown_chat_a3fa4c[i]->Address == unknown_address_a3f80c) {
                    wcscpy(unknown_chat_a3fa4c[i]->Name, Narrow_To_Wide_006fa180(unknown_gpacket_a3f640.Name));
                    unknown_chat_a3fa4c[i]->Chat.LastTime = TickCount.Time();
                    unknown_chat_a3fa4c[i]->Chat.LastChance = 0;
                    unknown_chat_a3fa4c[i]->Chat.Color = unknown_gpacket_a3f640.Chat.Color;
                    found = 1;
                    if (unknown_game_index_a71e90 == 0) {
                        display_users = true;
                    }
                    break;
                }
            }
            if (!found) {
                who = new NodeNameType;
                wcscpy(who->Name, Narrow_To_Wide_006fa180(unknown_gpacket_a3f640.Name));
                who->Address = unknown_address_a3f80c;
                who->Chat.LastTime = TickCount.Time();
                who->Chat.LastChance = 0;
                who->Chat.Color = unknown_gpacket_a3f640.Chat.Color;
                unknown_session_a3d298.NodeList_27b4.Add(who);
                if (unknown_game_index_a71e90 == 0) {
                    display_users = true;
                }
            }
        } else if (unknown_gpacket_a3f640.Command == 5) {
            // NET_CHAT_REQUEST
            if (unknown_join_state_a71e94 != 1 && unknown_join_state_a71e94 != 2) {
                UnknownGlobalPacket packet;
                memset(&packet, 0, sizeof(packet));
                packet.Command = 4;
                strcpy(packet.Name, unknown_name_a3d3a0);
                packet.Chat.ID = unknown_session_a3d298.UniqueID;
                packet.Chat.Color = unknown_a3d3b4;
                unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 1, &unknown_address_a3f80c, 0, 0);
                Service_Game_IO_00481fb0();
            }
        } else if (unknown_gpacket_a3f640.Command == 12) {
            // NET_MESSAGE
            int color = unknown_gpacket_a3f640.Message.unknown_110;
            if (unknown_join_state_a71e94 == 2) {
                if (unknown_gpacket_a3f640.Message.unknown_114 == (int)Compute_Name_CRC(unknown_host_name_a3f618)) {
                    Unknown_005C2C80(color, L"[%s] %s", Narrow_To_Wide_006fa180(unknown_gpacket_a3f640.Name), unknown_gpacket_a3f640.Message.Buf);
                    Voc_Play(Rules->IncomingMessage, 1.0f, 0x2000, 0);
                }
            } else {
                Unknown_005C2C80(color, L"[%s] %s", Narrow_To_Wide_006fa180(unknown_gpacket_a3f640.Name), unknown_gpacket_a3f640.Message.Buf);
                Voc_Play(Rules->IncomingMessage, 1.0f, 0x2000, 0);
            }
        } else if (unknown_gpacket_a3f640.Command == 6) {
            // NET_QUERY_JOIN
            UnknownGlobalPacket packet;
            if (!unknown_players_a3fa7c.Count()) {
                memset(&packet, 0, sizeof(packet));
                packet.Command = 8;
                packet.Reject.Why = 5;
                unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 1, &unknown_address_a3f80c, 0, 0);
            } else {
            found = 0;
            int resend = 0;
            for (i = 1; i < unknown_players_a3fa7c.Count(); i++) {
                if (!_stricmp(Wide_To_Narrow_006fa120(unknown_players_a3fa7c[i]->Name), unknown_gpacket_a3f640.Name)) {
                    if (unknown_players_a3fa7c[i]->Address != unknown_address_a3f80c) {
                        found = 1;
                    } else {
                        resend = 1;
                    }
                    break;
                }
            }
            if (!_stricmp(Wide_To_Narrow_006fa120(unknown_players_a3fa7c[0]->Name), unknown_gpacket_a3f640.Name)) {
                found = 1;
            }
            if (found) {
                memset(&packet, 0, sizeof(packet));
                packet.Command = 8;
                packet.Reject.Why = 0;
                unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 1, &unknown_address_a3f80c, 0, 0);
                continue;
            }
            if (!resend) {
                if (unknown_gpacket_a3f640.Join.unknown_57 != 1 && Slots_Filled_005c9440()) {
                    memset(&packet, 0, sizeof(packet));
                    packet.Command = 8;
                    packet.Reject.Why = 1;
                    unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 1, &unknown_address_a3f80c, 0, 0);
                    continue;
                }
                if (unknown_gpacket_a3f640.Join.unknown_57 == 1 && No_Observer_Slot_005c9490()) {
                    memset(&packet, 0, sizeof(packet));
                    packet.Command = 8;
                    packet.Reject.Why = 1;
                    unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 1, &unknown_address_a3f80c, 0, 0);
                    continue;
                }
            }
            bool match = true;
            if (!resend) {
                if (unknown_gpacket_a3f640.Join.unknown_43 != unknown_a71e9c) {
                    match = false;
                }
                if (unknown_gpacket_a3f640.Join.unknown_47 != unknown_ini_8397a0_unique_id()) {
                    match = false;
                }
                if (unknown_gpacket_a3f640.Join.unknown_4b != unknown_a71ea0) {
                    match = false;
                }
                if (unknown_gpacket_a3f640.Join.unknown_4f != Unknown_00737af0()) {
                    match = false;
                }
            }
            int dups = 0;
            for (i = 0; i < unknown_players_a3fa7c.Count(); i++) {
                if (!strcmp(unknown_players_a3fa7c[i]->unknown_34, unknown_gpacket_a3f640.unknown_18)) {
                    dups++;
                }
            }
            if (!match) {
                memset(&packet, 0, sizeof(packet));
                packet.Command = 8;
                packet.Reject.Why = 6;
                unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 1, &unknown_address_a3f80c, 0, 0);
                continue;
            }
            if (!resend) {
                unsigned long version = VerNum.Clip_Version(unknown_gpacket_a3f640.Join.unknown_3b, unknown_gpacket_a3f640.Join.unknown_3f);
                unknown_version_a3d8d4 = version;
                if (version == 0) {
                    memset(&packet, 0, sizeof(packet));
                    packet.Command = 8;
                    packet.Reject.Why = 2;
                    unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 1, &unknown_address_a3f80c, 0, 0);
                    continue;
                }
                if (version == 0xFFFFFFFF) {
                    memset(&packet, 0, sizeof(packet));
                    packet.Command = 8;
                    packet.Reject.Why = 3;
                    unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 1, &unknown_address_a3f80c, 0, 0);
                    continue;
                }
                unknown_session_a3d2ac = VerNum.Version_Protocol(version);
                who = new NodeNameType;
                memset(who, 0, sizeof(*who));
                wcscpy(who->Name, Narrow_To_Wide_006fa180(unknown_gpacket_a3f640.Name));
                who->Address = unknown_address_a3f80c;
                if (unknown_session_a3d298.Mode) {
                    who->unknown_66e2d0(!unknown_session_a3d298.Mode->unknown_slot7(unknown_gpacket_a3f640.Join.unknown_2f) ? unknown_session_a3d298.Mode->unknown_slot10() : unknown_gpacket_a3f640.Join.unknown_2f);
                }
                strcpy(who->unknown_34, unknown_gpacket_a3f640.unknown_18);
                int color = unknown_gpacket_a3f640.Join.unknown_33;
                if (Unknown_Is_Color_Taken_005c95d0(color)) {
                    color = -2;
                }
                if (unknown_gpacket_a3f640.Join.unknown_57 == 1) {
                    color = -2;
                    who->unknown_66e2d0(-3);
                    who->Team = -1;
                } else {
                    who->Team = 0;
                    if (unknown_gpacket_a3f640.Join.unknown_2f == -3) {
                        if (unknown_session_a3d298.Mode) {
                            who->unknown_66e2d0(unknown_session_a3d298.Mode->unknown_slot10());
                        } else {
                            who->unknown_66e2d0(-2);
                        }
                    }
                }
                who->unknown_66e300(color);
                unknown_session_a3d298.Players.Add(who);
                int slot = Assign_Slot_005c9170(who);
                Unknown_Enable_Start_005c9700();
                memset(&packet, 0, sizeof(packet));
                packet.Command = 7;
                strcpy(packet.Name, unknown_name_a3d3a0);
                display_users = true;
                packet.PlayerInfo.unknown_2f = who->unknown_66e290();
                packet.PlayerInfo.unknown_33 = who->Get_Color_0066e2a0();
                packet.PlayerInfo.Observer = unknown_lobby_flag_7e6d90;
                packet.PlayerInfo.Slot = slot;
                unknown_ipx_manager_a40978.Send_Global_Message(&packet, sizeof(packet), 1, &unknown_address_a3f80c, 0, 0);
                Voc_Play(Rules->PlayerJoined, 1.0f, 0x2000, 0);
                if (unknown_session_a3d298.Mode) {
                    int rows = unknown_session_a3d298.Mode->unknown_slot32();
                    if (rows != -1) {
                        unknown_int_a3db1c = rows;
                    }
                }
                Unknown_005c3810(true, false);
                UnknownGlobalPacket slots;
                memset(&slots, 0, sizeof(slots));
                slots.Command = 0x16;
                strcpy(slots.Name, unknown_name_a3d3a0);
                Unknown_Slot_Names_005c8cf0(slots.Slots.Names);
                int count = unknown_players_a3fa7c.Count();
                for (i = 1; i < count; i++) {
                    unknown_ipx_manager_a40978.Send_Global_Message(&slots, sizeof(slots), 1, &unknown_players_a3fa7c[i]->Address, 0, 0);
                    Service_Game_IO_00481fb0();
                }
                Unknown_005c34f0();
            }
            }
        }
    }
    if (display_users) {
        Unknown_005bbdd0(false);
    }
    if (display_games) {
        Refresh_Game_List_005bc790();
    }
}


// long __stdcall Join_Lobby_Dialog_Proc_005c2330(HWND window, unsigned message, unsigned wparam, long lparam) (0x005C2330)
// The joined player's game lobby dialog procedure.

long __stdcall Join_Lobby_Dialog_Proc_005c2330(HWND window, unsigned message, unsigned wparam, long lparam)
{
    switch (message) {
    case 0x02:                  // WM_DESTROY
        if (ProgressScreen.Has_Window()) {
            ProgressScreen.Clear_Window();
        }
        if (unknown_dialog_preview_a72ce8 != 0) {
            delete unknown_dialog_preview_a72ce8;
            unknown_dialog_preview_a72ce8 = 0;
            return 0;
        }
        break;
    case 0x2B:                  // WM_DRAWITEM
        Unknown_005FE380(lparam);
        // fall through
    case 0x14:                  // WM_ERASEBKGND
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
    case 0x84: {                // WM_NCHITTEST: the tool tip of the control under the cursor
        HWND tip = GetDlgItem(window, 0x695);
        if (tip != 0) {
            POINT point;
            point.x = lparam & 0xFFFF;
            point.y = (unsigned long)lparam >> 16;
            RECT rect;
            GetWindowRect(window, &rect);
            point.x -= rect.left;
            point.y -= rect.top;
            wchar_t const *text = L"";
            HWND control = ChildWindowFromPointEx(window, point, 1);
            if (control != 0) {
                char const *label = Unknown_005e22b0(window, control);
                if (label != 0) {
                    text = StringTable::Load_String(label, 0, "D:\\Ra2\\netdlg2.cpp", 3143);
                }
            }
            SendMessageA(tip, 0x4B2, 0, (LPARAM)text);
            return 0;
        }
        break;
    }
    case 0x111:                 // WM_COMMAND
        switch ((unsigned short)wparam) {
        case 2:
            if (!unknown_lock_a71e98) {
                unknown_lobby_result_a71e8c = 2;
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
        case 0x522:
        case 0x523:
        case 0x524:
        case 0x525:
        case 0x526:
        case 0x527:
        case 0x528:
        case 0x6A1:
        case 0x6A2:
            if ((unsigned short)(wparam >> 16) == 1) {
                int id = (unsigned short)wparam;
                int side_player = Side_Combo_Player_4d13b0(id);
                int color_player = Color_Combo_Player_4d1d10(id);
                int player = side_player > color_player ? side_player : color_player;
                int side_id = Player_Side_Combo_ID_4d1350(player);
                int color_id = Player_Color_Combo_ID_4d1cb0(player);
                if (player == side_player) {
                    Set_Slot_Country_005c9a10(window, side_id);
                } else {
                    Set_Slot_Color_005c9ab0(window, color_id);
                }
                char text[64];
                int side = Side_Combo_Data_4d1c70(window, side_id, -1);
                int color = Color_Combo_Data_4d2900(window, color_id, -1);
                sprintf(text, "R%d,%d", side, color);
                Unknown_005c3d90(Wide_To_Narrow_006fa120(unknown_host_name_a3f618), text);
                return 0;
            }
            break;
        case 0x540:
            Limit_Name_Text_005ccf50((HWND)lparam);
            if ((unsigned short)(wparam >> 16) == 0x501) {
                Send_Chat_Message_005bee20(window, unknown_color_7a737c, unknown_color_7a7364);
                return 0;
            }
            break;
        case 0x59F:
            if (unknown_players_a3fa7c.Count() != 0) {
                unknown_players_a3fa7c[0]->unknown_67 = 1;
                char text[64];
                sprintf(text, "A1");
                Send_Game_Options_005c3c60(text);
                Unknown_005cd030(window, false);
                HWND button = GetDlgItem(window, 0x59F);
                EnableWindow(button, FALSE);
                InvalidateRect(button, 0, FALSE);
                SendMessageA(button, 0x4DC, 0, 0);
                Unknown_005bbdd0(false);
                return 0;
            }
            break;
        }
        break;
    case 0x497:
        Fill_Side_Combos_4d16d0(window);
        Init_Color_Entries_4d1ea0();
        Fill_Color_Combos_4d2300(window);
        Lock_Slots_005c9da0(window, unknown_lobby_a3fa94);
        Select_Player_Side_4d1820(window, unknown_lobby_a3fa94);
        Select_Player_Color_4d23c0(window, unknown_lobby_a3fa94);
        EnableWindow(GetDlgItem(window, 0x59F), FALSE);
        unknown_a72d52 = false;
        Set_Player_Value_005bcbc0(0, 0);
        Unknown_005bbdd0(false);
        Refresh_Game_List_005bc790();
        Unknown_Fill_Options_Dialog_005c3550(window, 1);
        Select_Scenario_005c7c30(-1);
        unknown_int_a3db1c = 8;
        return 0;
    case 0x4E9: {               // the tool tip text of a combo box item
        ToolTipTextRequest *request = (ToolTipTextRequest *)lparam;
        request->Text = 0;
        if (request->Window != 0 && request->Item != -1) {
            int id = GetDlgCtrlID(request->Window);
            if (Control_Slot_005c9800(id) == -1) {
                if (Side_Combo_Player_4d13b0(id) == -1) {
                    if (Color_Combo_Player_4d1d10(id) != -1) {
                        request->Text = Color_Name_4d1d80(Color_Combo_Data_4d2900(window, id, request->Item));
                        return 1;
                    }
                } else {
                    int side = Side_Combo_Data_4d1c70(window, id, request->Item);
                    if (side != -1) {
                        request->Text = Side_Name_4d1420(side);
                        return 1;
                    }
                }
            } else {
                request->Text = Entry_Type_Name_005c9870((NodeNameType *)unknown_combo_selected_data_005c99d0(window, id, request->Item));
            }
        }
        return 1;
    }
    case 0x4EC:
        Unknown_005e8710(window);
        break;
    }
    return 0;
}


// void __fastcall Unknown_005c28e0() (0x005C28E0)

void __fastcall Unknown_005c28e0()
{
    HWND dialog = Unknown_00735B10();
    HWND item = GetDlgItem(dialog, 0x59F);
    if (Unknown_Find_Player_By_Name_005bcc60(0) != 1) {
        SendMessageA(item, 0x4DC, 0, 1);
        Unknown_005C2C80(-1, StringTable::Load_String("GUI:HostGameStartJoiner", 0, "D:\\Ra2\\netdlg2.cpp", 4958));
    }
}


// bool DynamicVectorClass<NodeNameType *>::Delete(int index) (0x005C2970).
// Instantiated from shared/vector_class.hpp by the out-of-line call in Process_Timeouts_005bc500
// (the netdlg2.cpp unit; its first draft enables VECTOR_IMPLEMENT_DYNAMIC_DELETE_INDEX).
