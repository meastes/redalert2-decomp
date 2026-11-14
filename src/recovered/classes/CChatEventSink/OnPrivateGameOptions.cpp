#define RA2_WOL_PRIVATE_OPTIONS
// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// CChatEventSink::OnPrivateGameOptions, original RA2 0x00761C50.
#include "shared/wol_sink.hpp"
#include "shared/lobby.hpp"
#include "shared/session_state.hpp"
#include "shared/multiplayer_game_mode.hpp"
#include "shared/gdlg_supp.hpp"
#include "shared/map_preview.hpp"
#include "shared/crt_string.hpp"
extern "C" int __cdecl sscanf(char const *,char const *,...);
extern bool unknown_random_preview_7e6dc0;
extern char port_debug_7fe030[],options_debug_7fdfd0[],preview_debug_7fdf98[],hex_format_7db4b0[],comma_7ce9c0[];

void __fastcall Unknown_0073c480();
inline WOL::User *Unknown_0074DA60()
{
    for (int i = 0; i < unknown_users_b2a728.Count; ++i) {
        unknown_users_b2a728.Get_Copy(unknown_user_b2a658, i);
        if (unknown_user_b2a658.flags & 1) return &unknown_user_b2a658;
    }
    return 0;
}

HRESULT __stdcall CChatEventSink::OnPrivateGameOptions(HRESULT,WOL::User *user,char const *options)
{
    if(unknown_b2abd9) return S_OK;
    if(unknown_b2ac40 && options[0]!='Z') return S_OK;
    if(Is_Online_Session() && options[0]!='Z') return S_OK;
    int existing_user;
    { Wstring name(user->name);existing_user=unknown_users_b2a728.Find_Name(name); }
    if(existing_user==-1) return S_OK;
    switch(options[0]) {
    case 'Z':
        if(options[1]=='4') {
            unsigned int port=atol(options+2);
            Debug_Printf(port_debug_7fe030,user->name,port&0xffff);
            for(int i=0;i<8;++i) {
                if(!_stricmp(Wide_To_Narrow_006fa120(unknown_player_slots_a3fa98[i]->Name),user->name)) {
                    unknown_session_a3d298.unknown_2dc0[i]=(unsigned short)port;
                    break;
                }
            }
        }
        break;
    case 'R': {
        if(unknown_b2ac58!=1) break;
        char *buffer=_strdup(options+1);
        int index=-1;
        int row=0;
        for(;row<8;++row) {
            Wstring name=Narrow_Copy_00771280(unknown_player_slots_a3fa98[row]->Name);
            if(!_stricmp(name.Get(),user->name)) {index=row;break;}
        }
        if(index!=-1) {
            char *token=strtok(buffer,comma_7ce9c0);
            int country=0;
            if(token) country=atol(token);
            token=strtok(0,comma_7ce9c0);
            int color=0;
            if(token) color=atol(token);
            token=strtok(0,comma_7ce9c0);
            unsigned int ip=0;
            sscanf(token,hex_format_7db4b0,&ip);
            IPXAddressClass address;
            if(ip) {
                NetNumType net;NetNodeType node;
                memset(net,0,sizeof(net));memset(node,0,sizeof(node));
                *(unsigned int *)node=ip;
                address.Set_Address(net,node,0);
                unknown_session_a3d298.unknown_293c.Addresses[row]=address;
            }
            token=strtok(0,comma_7ce9c0);
            unsigned int port=0;
            if(token) sscanf(token,hex_format_7db4b0,&port);
            unknown_session_a3d298.unknown_299c.unknown[row]=port;
            token=strtok(0,comma_7ce9c0);
            unsigned int value=0;
            if(token) sscanf(token,hex_format_7db4b0,&value);
            unknown_session_a3d298.unknown_2fa0.unknown[row]=value;
            Debug_Printf(options_debug_7fdfd0,user->name,country,color,address.As_String(),unknown_session_a3d298.unknown_299c.unknown[row],value);
            HWND window=(HWND)WS_Find_Dialog_00735940(0xc2);
            if(!window) window=(HWND)WS_Find_Dialog_00735940(0xc9);
            int old_country=unknown_player_slots_a3fa98[row]->unknown_66e290();
            NodeNameType *player=unknown_player_slots_a3fa98[row];
            bool become_observer=false,become_player=false,changed=false;
            if(old_country==-3) {
                if(country!=old_country) become_player=true;
            } else if(country==-3) become_observer=true;
            WOL::User *host=Unknown_0074DA60();
            if(host && !_stricmp(host->name,unknown_wstring_b26cb8.Peek_Buffer()) && row==0 && become_observer && unknown_session_a3d298.Mode && !unknown_session_a3d298.Mode->unknown_slot40()) {
                become_observer=false;changed=true;
            }
            if(become_player) {
                host=Unknown_0074DA60();
                if(host && !_stricmp(host->name,unknown_wstring_b26cb8.Peek_Buffer())) {
                    if(Slots_Filled_005c9440()) {
                        Unknown_005cbf20(window,Slot_Control_Id_005c97a0(row),unknown_player_a72d44,0);
                        changed=true;Unknown_0073c480();Unknown_005c3810(true,false);Unknown_0073FEE0(0,false);
                    } else {
                        player->unknown_66e2d0(-2);player->Team=0;
                        Unknown_005cbf20(window,Slot_Control_Id_005c97a0(unknown_lobby_flag_7e6d90),unknown_player_a72d44,0);
                        index=Assign_Slot_005c9170(player);
                        Unknown_005cbf20(window,Slot_Control_Id_005c97a0(index),player,0);
                        Unknown_0073c480();
                    }
                }
            }
            if(become_observer) {
                host=Unknown_0074DA60();
                if(host && !_stricmp(host->name,unknown_wstring_b26cb8.Peek_Buffer())) {
                    if(No_Observer_Slot_005c9490()) {
                        Unknown_005cbf20(window,Slot_Control_Id_005c97a0(index),unknown_open_entry_a72d2c,0);
                        Unknown_0073c480();
                    } else {
                        player->unknown_66e2d0(-3);player->Team=-1;
                        Unknown_005cbf20(window,Slot_Control_Id_005c97a0(index),unknown_open_entry_a72d2c,0);
                        Unknown_005cbf20(window,Slot_Control_Id_005c97a0(unknown_lobby_flag_7e6d90),player,0);
                        Unknown_0073c480();
                        Fill_Observer_Combo_4d1640(window,Player_Side_Combo_ID_4d1350(unknown_lobby_flag_7e6d90));
                        int observer_control=Slot_Control_Id_005c97a0(unknown_lobby_flag_7e6d90);
                        SendMessageA(GetDlgItem(window,observer_control),0x4f1,0,1);
                        int control=Player_Color_Combo_ID_4d1cb0(unknown_lobby_flag_7e6d90);
                        Fill_Random_Color_Combo_4d2250(window,control);Select_Color_Combo_4d2480(window,control,8);
                    }
                    goto refresh_options;
                }
            }
            if(changed) goto request_done;
            bool country_changed=old_country!=country;
            if(unknown_session_a3d298.Mode && !unknown_session_a3d298.Mode->unknown_slot7(country)) {
                country=unknown_session_a3d298.Mode->unknown_slot7(old_country)?old_country:-2;
                country_changed=true;
            }
            if(country_changed) unknown_player_slots_a3fa98[index]->unknown_66e2d0(country);
            bool color_changed=unknown_player_slots_a3fa98[index]->unknown_66e2a0()!=color;
            if(color_changed) {
                if(unknown_session_a3d298.Unknown_006728D0(color)) color=-2;
                unknown_player_slots_a3fa98[index]->unknown_66e300(color);
            }
            if(!country_changed && !color_changed) goto request_done;
        } else goto request_done;
refresh_options:
        Unknown_005c3810(true,false);Unknown_0073FEE0(0,false);
request_done:
        free(buffer);
        break;
    }
    case 'P': {
        if(unknown_b2ac58!=1) break;
        Debug_Printf(preview_debug_7fdf98);
        if(unknown_users_b2a728.Count>=2) {
            WOL::User temp;
            WOL::User *host=Unknown_0074DA60();
            if(host && !_stricmp(host->name,unknown_wstring_b26cb8.Peek_Buffer()) && !unknown_b2ac40 && unknown_channel_b299c0.unknown_00 && unknown_dialog_preview_a72ce8 && unknown_dialog_preview_a72ce8->Is_Present() && unknown_random_preview_7e6dc0) {
                int found=unknown_users_b2a728.Find_Name(user->name);
                if(found!=-1) {
                    unknown_users_b2a728.Get_Copy(temp,found);temp.next=0;
                    Unknown_0074d7d0(&temp);
                    int old=unknown_login_state_00b2abe4;unknown_login_state_00b2abe4=1;
                    unknown_dialog_preview_a72ce8->unknown_005c7ef0();unknown_login_state_00b2abe4=old;
                }
            }
        }
        break;
    }
    }
    return S_OK;
}

// @ref ?unknown_random_preview_7e6dc0=007e6dc0
// @ref ?port_debug_7fe030=007fe030
// @ref ?options_debug_7fdfd0=007fdfd0
// @ref ?preview_debug_7fdf98=007fdf98
// @ref ?hex_format_7db4b0=007db4b0
// @ref ?comma_7ce9c0=007ce9c0
