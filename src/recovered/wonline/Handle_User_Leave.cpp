// Updates WOL and lobby state after a user leaves a channel.
#define VECTOR_IMPLEMENT_ALL
#define RA2_WOL_USER_INLINE_SHRINK
#include "shared/wol_sink.hpp"
#include "shared/lobby.hpp"
#include "shared/session_state.hpp"
#include "shared/rules.hpp"
#include "shared/sound_play.hpp"
#include "shared/gdlg_supp.hpp"
#include "shared/win_util.hpp"
static inline HWND Get_Item(HWND window, int id) { return GetDlgItem(window,id); }
extern wchar_t wol_scenario_description_a3d342[];
extern int wol_player_pings_b292ac[8];
extern int wol_lan_leave_b2abf0;
extern char leave_debug_local_7fe45c[],leave_debug_started_7fe42c[],leave_debug_clear_7fe410[];
extern char leave_debug_lan_guest_7fe3f0[],leave_debug_lan_game_7fe3d0[],leave_debug_running_7fe39c[];
extern char lobby_prefix_7fd358[];
extern wchar_t lobby_name_format_7d9c94[];
extern wchar_t lobby_number_format_7fd91c[];
extern char leave_text_label_7fe420[],wol_source_path_7fd62c[];
HWND __fastcall WS_Top_Window();
void __stdcall unknown_dialog_5ecf40(HWND window,int result);
void __fastcall Unknown_005cd030(HWND window,bool update);
void __fastcall Unknown_Release_Slot_by_Name_005c9270(char const *name);
bool __fastcall Unknown_005c9320(NodeNameType *entry);
void __fastcall Store_User_Ping_0075b110(char const *name,int ping);
void __fastcall Refresh_Game_List_0073f8f0();
void __fastcall Unknown_0073c480();
int __fastcall Join_Selected_Lobby_00744930(bool unknown);
void __fastcall Unknown_005C2C80(int color,wchar_t const *format,...);
void __fastcall Handle_User_Leave_00741750(WOL::User *user)
{
    _stricmp(unknown_wstring_b26cb8.Get(),user->name);
    if (unknown_channel_b299c0.unknown_00 != 0) Voc_Play(Rules->PlayerLeft,1.0f,0x2000,0);
    if (user->flags & 0x8000) {
        Debug_Printf(leave_debug_local_7fe45c);
        if (unknown_b2ac40 != 0) Debug_Printf(leave_debug_started_7fe42c);
        wchar_t name[64];
        Char_To_Wide_006fa0c0(name,unknown_channel_b299c0.name,-1);
        if (strncmp(unknown_channel_b299c0.name,lobby_prefix_7fd358,7) == 0) {
            int number = atol(unknown_channel_b299c0.name+strlen(lobby_prefix_7fd358));
            int page = number/8;
            int lobby = number%8;
            if (page == 0) swprintf(name,lobby_name_format_7d9c94,unknown_lobby_names_b26c94[lobby]);
            else swprintf(name,lobby_number_format_7fd91c,unknown_lobby_names_b26c94[lobby],page+1);
        }
        if (!Is_Online_Session())
            Unknown_005C2C80(unknown_color_7a7360,StringTable::Load_String(leave_text_label_7fe420,0,wol_source_path_7fd62c,0x1f39),name);
        unknown_b2abe8=0;
        Debug_Printf(leave_debug_clear_7fe410);
        memset(&unknown_channel_b299c0,0,sizeof(unknown_channel_b299c0));
        memset(unknown_player_b29cf0,0,sizeof(unknown_player_b29cf0));
        unknown_users_b2a728.Count=0;
        unknown_b2ac58=0;
        HWND wait=WS_Find_Dialog_00735940(0xc4);
        if (wait) {
            unknown_b2abe0 &= ~0x40;
            if (unknown_b2abe0 == 0) End_Dialog_007357e0(wait,0);
        }
        HWND active=WS_Top_Window();
        HWND focus=Get_Item(active,0x540);
        if (focus) SetFocus(focus);
        wol_scenario_description_a3d342[0]=0;
        ++unknown_counter_a3fabc;
        unknown_session_a3d298.ScenarioFileLength_87c=0;
        unknown_session_a3d298.ScenarioDigest_85a[0]=0;
        HWND main=WS_Find_Dialog_00735940(0x113);
        if (main && g_PlayingNetGame) {
            ShowWindow(main,0);
            unknown_dialog_5ecf40(main,1);
        }
        HWND guest=WS_Find_Dialog_00735940(0xc9);
        if (guest) {
            End_Dialog_007357e0(guest,2);
            ShowWindow(guest,0);
            unknown_dialog_5ecf40(guest,1);
            if (unknown_a3d2a4 == 3) {
                if (g_PlayingNetGame) {
                    if (main) {
                        ShowWindow(main,0);
                        unknown_dialog_5ecf40(main,1);
                    }
                } else {
                    Join_Selected_Lobby_00744930(false);
                    if (main) PostMessageA(main,0x593,0,0);
                }
            } else if (unknown_a3d2a4 == 4) {
                Debug_Printf(leave_debug_lan_guest_7fe3f0);
                wol_lan_leave_b2abf0=1;
            }
        }
        HWND game=WS_Find_Dialog_00735940(0xc2);
        if (game) {
            End_Dialog_007357e0(game,2);
            ShowWindow(game,0);
            unknown_dialog_5ecf40(game,1);
            if (unknown_a3d2a4 == 3) {
                if (g_PlayingNetGame) {
                    if (main) {
                        ShowWindow(main,0);
                        unknown_dialog_5ecf40(main,1);
                    }
                } else {
                    Join_Selected_Lobby_00744930(false);
                    if (main) PostMessageA(main,0x593,0,0);
                }
            } else if (unknown_a3d2a4 == 4) Debug_Printf(leave_debug_lan_game_7fe3d0);
        }
        if (main) {
            if (!g_PlayingNetGame) {
                ShowWindow(main,5);
                unknown_dialog_5ecf40(main,0);
            }
            Refresh_Game_List_0073f8f0();
        }
        --unknown_counter_a3fabc;
        return;
    }
    if (unknown_b2ac40 == 2) {
        Debug_Printf(leave_debug_running_7fe39c);
        return;
    }
    if (user) {
        for (int slot=0;slot<8;++slot) {
            if (unknown_player_slots_a3fa98[slot] && !strcmp(Wide_To_Narrow_006fa120(unknown_player_slots_a3fa98[slot]->Name),user->name)) {
                HWND lobby=Find_Lobby_Window_005c2c40();
                if (lobby) Set_Player_Ping_4d1060(lobby,slot,-1);
                break;
            }
        }
    }
    WOL::User other;
    for (int index=0;index<unknown_users_b2a728.Count;++index) {
        unknown_users_b2a728.Get_Copy(other,index);
        if (!_stricmp(other.name,user->name)) {
            unknown_users_b2a728.Remove(index);
            if (unknown_channel_b299c0.unknown_00 != 0) {
                memmove(unknown_player_b29cf0+index,unknown_player_b29cf0+index+1,sizeof(WolPlayerData)*(15-index));
                if (unknown_b2ac40 != 2) {
                    unsigned accepted_end=(unsigned)(unknown_player_b29cf0+16)+0x20;
                    for (unsigned accepted=(unsigned)&unknown_player_b29cf0[1].unknown_20; accepted<accepted_end; accepted+=sizeof(WolPlayerData)) {
                        *(int *)accepted=0;
                        EnableWindow(GetDlgItem(Find_Lobby_Window_005c2c40(),0x59f),1);
                    }
                    if (!unknown_b2ac58) {
                        HWND dialog=WS_Find_Dialog_00735940(0xc9);
                        Unknown_005cd030(dialog,true);
                    }
                    Unknown_005c9700();
                }
            }
            if (index == 0 && unknown_channel_b299c0.unknown_00 != 0 && !unknown_b2ac58)
                unknown_chat_a72cfc->unknown_slot09();
            break;
        }
    }
    if (unknown_channel_b299c0.unknown_00 != 0) {
        Unknown_Release_Slot_by_Name_005c9270(user->name);
        for (int player=0;player<unknown_players_a3fa7c.Count();++player) {
            if (!_stricmp(Wide_To_Narrow_006fa120(unknown_players_a3fa7c[player]->Name),user->name)) {
                NodeNameType *entry=unknown_players_a3fa7c[player];
                unknown_players_a3fa7c.Delete(player);
                unknown_session_a3d298.Unknown_00672F20(player);
                delete entry;
                break;
            }
        }
        for (int slot=0;slot<8;++slot) {
            if (!unknown_player_slots_a3fa98[slot] || !Unknown_005c9320(unknown_player_slots_a3fa98[slot])) wol_player_pings_b292ac[slot]=-1;
            char *name=Wide_To_Narrow_006fa120(unknown_player_slots_a3fa98[slot]->Name);
            Store_User_Ping_0075b110(name,wol_player_pings_b292ac[slot]);
        }
        if (unknown_b2ac58) {
            Unknown_005c9700();
            Unknown_0073c480();
            PumpGameopts(true,true);
            if (Current_Dialog_Id_00735b20() == 0x6b) {
                HWND window=WS_Top_Window();
                int index=SendDlgItemMessageA(window,0x6eb,0x188,0,0);
                if (index != -1) {
                    UnknownSessionA3D29C *mode=(UnknownSessionA3D29C *)SendDlgItemMessageA(window,0x6eb,0x199,index,0);
                    if (mode) {
                        mode->unknown_slot30();
                        Unknown_0073FEE0(0,false);
                        return;
                    }
                }
            } else if (unknown_session_a3d29c) unknown_session_a3d29c->unknown_slot30();
        }
    }
    Unknown_0073FEE0(0,false);
}
