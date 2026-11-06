// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// HRESULT CChatEventSink::OnUserList(HRESULT, WOL::Channel *, WOL::User *) (0x00760C00)
// Rebuilds the WOL channel user list and queues missing profile lookups.
// From ra2-decomp 0x007A7110 (YR twin FUN_007A7110, src/wonline.cpp): the session player list is read through
// the session object (Session.Players) and a user's address comes from Unknown_0073c130 (YR FUN_0077D720).
// Built on codex4's pass7_760c00 draft (Scout696 keep/ul_c4_9972.cpp).
#include "shared/wol_sink.hpp"
#include "shared/session_state.hpp"
#include "shared/unknown_player_entry.hpp"
#include "shared/lobby.hpp"
#include "shared/win_util.hpp"
extern char ladder_names_separator_7e6f8c[], user_reply_format_7fdec4[],user_reply_debug_7fdea0[];
extern unsigned int local_socket_a71b18;
HRESULT __stdcall CChatEventSink::OnUserList(HRESULT result,WOL::Channel *channel,WOL::User *user)
{
    if (unknown_b2abd9) return S_OK;
    memset(unknown_query_b2b420,0,sizeof(char)*512);
    Wstring ladder_name;
    if (FAILED(result)) return S_OK;
    SendDlgItemMessageA(Unknown_00735B10(),0x53e,0x184,0,0);
    unknown_users_b2a728.Count=0;
    int batch_count=0;
    int user_count=0;
    bool saw_owner=false;
    while (user) {
        char name_buffer[128];
        ++user_count;
        if (user->unknown_14 != 0 && strlen(user->unknown_57) == 0) {
            ++unknown_query_count_b2abf4;
            unknown_chat_a72cfc->unknown_slot44(user->unknown_14);
        } else if (unknown_ladder_client_a72d00 && user->unknown_14 != 0 && strlen(user->unknown_57) != 0) {
            Wstring squad(user->unknown_57);
            squad.To_Lower();
            if (!unknown_ladder_b296f8.Contains(squad))
                unknown_ladder_client_a72d00->Request(unknown_ladder_address_b29018,unknown_ladder_port_b2a8ac,user->unknown_57,0x802100,-1,0,0);
        }
        unsigned flags=user->flags;
        strcpy(name_buffer,user->name);
        WOL::User entry=*user;
        entry.next=0;
        unknown_users_b2a728.Insert_00765e90(entry,unknown_users_b2a728.Count);
        if (flags & 1) saw_owner=true;
        ladder_name.Set(user->name);
        ladder_name.To_Lower();
        if (unknown_game_mode_a3d298 == 4 && unknown_a3d2a4 == 2 && !unknown_known_names_b294b8.Contains(ladder_name))
            unknown_game_names_b2a648.Insert(ladder_name,0);
        if (!unknown_channel_names_b293a0.Contains(ladder_name)) unknown_pending_names_b29468.Insert(ladder_name,0);
        if (!unknown_ladder_cache_b2ac24->Contains(ladder_name) && user_count < 100) {
            ladder_name.Set(user->name);
            ladder_name.To_Lower();
            strcat(unknown_query_b2b420,ladder_name.Get());
            strcat(unknown_query_b2b420,ladder_names_separator_7e6f8c);
            ++batch_count;
        }
        if (batch_count > 20) {
            unknown_ladder_client_a72d00->Request(unknown_ladder_address_b29018,unknown_ladder_port_b2a8ac,unknown_query_b2b420,unknown_protocol_7fd370 & 0xffff00,-1,0,0);
            batch_count=0;
            memset(unknown_query_b2b420,0,512);
        }
        if (unknown_channel_b299c0.unknown_00 != 0 && !unknown_b2ac58) {
            Wstring name(user->name);
            int index=unknown_users_b2a728.Find_Name(name);
            memset(unknown_player_b29cf0+index,0,sizeof(WolPlayerData));
            strcpy(unknown_player_b29cf0[index].name,user->name);
            if (_stricmp(user->name,unknown_wstring_b26cb8.Get()) != 0) {
                if (unknown_session_a3d298.Players.Count() == 1) wcscpy(unknown_session_a3d298.HostName_2380,Unknown_006FA180(user->name));
                NodeNameType *node=new NodeNameType;
                memset(node,0,sizeof(NodeNameType));
                WideString wide_name=Wstring_To_Wide(user->name);
                wcscpy(node->Name,wide_name.Get());
                node->Address=Unknown_0073c130(user);
                node->unknown_66e2d0(-2);
                node->unknown_66e300(-2);
                unknown_session_a3d298.Players.Add(node);
            }
        }
        user=user->next;
    }
    if (strlen(unknown_query_b2b420))
        unknown_ladder_client_a72d00->Request(unknown_ladder_address_b29018,unknown_ladder_port_b2a8ac,unknown_query_b2b420,unknown_protocol_7fd370 & 0xffff00,-1,0,0);
    if (unknown_channel_b299c0.unknown_00 != 0) {
        if (!saw_owner) {
            WOL::User entry;
            for (int index=0;index<unknown_users_b2a728.Count;++index) {
                if (unknown_users_b2a728.Get_Copy(entry,index) && !_stricmp(entry.name,unknown_wstring_b26cb8.Get())) {
                    Unknown_00741750(&entry);
                    break;
                }
            }
        }
        if (unknown_channel_b299c0.unknown_00 != 0) {
            if (!unknown_b2ac58 && unknown_users_b2a728.Count == 1 && !_stricmp(user->name,unknown_wstring_b26cb8.Get())) Unknown_00741750(user);
            if (unknown_channel_b299c0.unknown_00 != 0) {
                unknown_a72d08=1;
                if (!unknown_b2ac58) {
                    HWND window=Find_Dialog_00735940(0xc9);
                    Unknown_005cd030(window,true);
                }
                Unknown_005c9700();
                WolRegistryOptions options;
                options.Read_004ff200();
                rc_ptr<PlayerProfile> profile;
                if (options.unknown_004ff2c0()) profile=PlayerProfile::Load_00738880(unknown_wstring_b26cb8);
                else profile.Set(unknown_profile_b28cc4);
                unknown_session_a3d298.Set_Value_00672a30(profile->Country,false);
                unknown_session_a3d298.Set_Value_00672ae0(profile->Color,false);
                if (unknown_a72c68) {
                    if (!Is_Online_Session()) {
                        unknown_session_a3d298.Set_Value_00672a30(-3,false);
                        unknown_session_a3d298.Set_Value_00672ae0(-2,false);
                    }
                }
                if (!unknown_a72c68 && !Is_Online_Session() && unknown_session_a3d298.Value_0124_006729f0() == -3) {
                    if (unknown_session_a3d298.Players.Count() != 0) {
                        unknown_session_a3d298.Set_Value_00672a30(unknown_session_a3d298.Players[0]->unknown_66e290(),false);
                        unknown_session_a3d298.Set_Value_00672ae0(unknown_session_a3d298.Players[0]->unknown_66e2a0(),false);
                    } else {
                        unknown_session_a3d298.Set_Value_00672a30(-2,false);
                        unknown_session_a3d298.Set_Value_00672ae0(-2,false);
                    }
                }
                char buffer[128];
                {
                NetNumType network;
                NetNodeType address;
                unsigned short socket;
                unknown_address_a3fbc8.Get_Address(network,address,&socket);
                sprintf(buffer,user_reply_format_7fdec4,unknown_session_a3d298.Value_0124_006729f0(),unknown_session_a3d298.Value_011c_00672a10(),*(unsigned *)address,local_socket_a71b18&0xffff,unknown_a40230);
                Debug_Printf(user_reply_debug_7fdea0,buffer);
                }
                WOL::User host;
                if (unknown_users_b2a728.Get_Copy(host,0)) unknown_chat_a72cfc->unknown_slot14(&host,buffer);
            }
        }
    }
    if (Is_Online_Session() && unknown_channel_b299c0.unknown_00 == 0 && !Unknown_0075a920()) SetEvent(unknown_b2650c);
    Unknown_0073FEE0(0,false);
    return S_OK;
}
// @ref ??0IPXAddressClass@@QAE@QAE0G@Z=00523440
// @ref ??0IPXAddressClass@@QAE@XZ=00523410
// @ref ??0Wstring@@QAE@ABV0@@Z=0076d980
// @ref ??0Wstring@@QAE@PBD@Z=0076d920
// @ref ??0Wstring@@QAE@XZ=0076d910
// @ref ??0rc_ptr_base@@QAE@XZ=006394e0
// @ref ??1WideString@@QAE@XZ=0076fa50
// @ref ??1Wstring@@QAE@XZ=0076d9e0
// @ref ??1rc_ptr_base@@UAE@XZ=00639510
// @ref ??2@YAPAXI@Z=00782017
// @ref ??8Wstring@@QBE_NABV0@@Z=0076da70
// @ref ?Debug_Printf@@YAXPBDZZ=004068f0
// @ref ?Find_Dialog_00735940@@YIPAUHWND__@@H@Z=00735940
// @ref ?Get@WideString@@QBEPBGXZ=007703f0
// @ref ?Get@Wstring@@QBEPBDXZ=0076e6f0
// @ref ?Get_Address@IPXAddressClass@@QAEXQAE0PAG@Z=005234c0
// @ref ?Grow_00768320@?$WolArrayList@VWstring@@@@QAE_NXZ=00768320
// @ref ?Insert_00765e90@WolUserArray@@QAE_NABUUser@WOL@@H@Z=00765e90
// @ref ?Load_00738880@PlayerProfile@@SI?AV?$rc_ptr@VPlayerProfile@@@@ABVWstring@@@Z=00738880
// @ref ?Peek_Buffer@Wstring@@QBEPBDXZ=0076e730
// @ref ?Read_004ff200@WolRegistryOptions@@QAE_NXZ=004ff200
// @ref ?Set@Wstring@@QAE_NPBD@Z=0076ed30
// @ref ?Set@rc_ptr_base@@QAEXPAVReferenceCounted@@@Z=00639530
// @ref ?Set_Value_00672a30@UnknownSessionState@@QAEXH_N@Z=00672a30
// @ref ?Set_Value_00672ae0@UnknownSessionState@@QAEXH_N@Z=00672ae0
// @ref ?To_Lower@Wstring@@QAEXXZ=0076ee20
// @ref ?Unknown_005c9700@@YAXXZ=005c9700
// @ref ?Unknown_005cd030@@YIXPAUHWND__@@_N@Z=005cd030
// @ref ?Unknown_006FA180@@YIPAGPBD@Z=006fa180
// @ref ?Unknown_00735B10@@YIPAUHWND__@@XZ=00735b10
// @ref ?Unknown_0073FEE0@@YIXH_N@Z=0073fee0
// @ref ?Unknown_00741750@@YIXPAUUser@WOL@@@Z=00741750
// @ref ?Unknown_0075a920@@YI_NXZ=0075a920
// @ref ?Value_011c_00672a10@UnknownSessionState@@QAEHXZ=00672a10
// @ref ?Value_0124_006729f0@UnknownSessionState@@QAEHXZ=006729f0
// @ref ?Wstring_To_Wide@@YI?AVWideString@@ABVWstring@@@Z=007711c0
// @ref ?ladder_names_separator_7e6f8c@@3PADA=007e6f8c
// @ref ?local_socket_a71b18@@3IA=00a71b18
// @ref ?unknown_004ff2c0@WolRegistryOptions@@QBE_NXZ=004ff2c0
// @ref ?unknown_66e290@NodeNameType@@QAEHXZ=0066e290
// @ref ?unknown_66e2a0@NodeNameType@@QAEHXZ=0066e2a0
// @ref ?unknown_66e2d0@NodeNameType@@QAEXH@Z=0066e2d0
// @ref ?unknown_66e300@NodeNameType@@QAEXH@Z=0066e300
// @ref ?unknown_a3d2a4@@3HA=00a3d2a4
// @ref ?unknown_a40230@@3HA=00a40230
// @ref ?unknown_a72c68@@3HA=00a72c68
// @ref ?unknown_a72d08@@3HA=00a72d08
// @ref ?unknown_address_a3fbc8@@3VIPXAddressClass@@A=00a3fbc8
// @ref ?unknown_b2650c@@3PAXA=00b2650c
// @ref ?unknown_b2abd9@@3_NA=00b2abd9
// @ref ?unknown_b2ac58@@3HA=00b2ac58
// @ref ?unknown_channel_b299c0@@3UChannel@WOL@@A=00b299c0
// @ref ?unknown_channel_names_b293a0@@3VWolChannelNameSet@@A=00b293a0
// @ref ?unknown_chat_a72cfc@@3PAUIChat@WOL@@A=00a72cfc
// @ref ?unknown_game_mode_a3d298@@3HA=00a3d298
// @ref ?unknown_game_names_b2a648@@3V?$WolArrayList@VWstring@@@@A=00b2a648
// @ref ?unknown_known_names_b294b8@@3VWolChannelNameSet@@A=00b294b8
// @ref ?unknown_ladder_address_b29018@@3PADA=00b29018
// @ref ?unknown_ladder_b296f8@@3VWolLadderCache@@A=00b296f8
// @ref ?unknown_ladder_cache_b2ac24@@3PAVWolLadderCache@@A=00b2ac24
// @ref ?unknown_ladder_client_a72d00@@3PAUWolLadderClient@@A=00a72d00
// @ref ?unknown_ladder_port_b2a8ac@@3HA=00b2a8ac
// @ref ?unknown_pending_names_b29468@@3V?$WolArrayList@VWstring@@@@A=00b29468
// @ref ?unknown_player_b29cf0@@3PAUWolPlayerData@@A=00b29cf0
// @ref ?unknown_players_a3fa7c@@3V?$DynamicVectorClass@PAUNodeNameType@@@@A=00a3fa7c
// @ref ?unknown_profile_b28cc4@@3PAVPlayerProfile@@A=00b28cc4
// @ref ?unknown_protocol_7fd370@@3IA=007fd370
// @ref ?unknown_query_b2b420@@3PADA=00b2b420
// @ref ?unknown_query_count_b2abf4@@3HA=00b2abf4
// @ref ?unknown_session_a3d298@@3UUnknownSessionState@@A=00a3d298
// @ref ?unknown_users_b2a728@@3VWolUserArray@@A=00b2a728
// @ref ?unknown_wstring_b26cb8@@3VWstring@@A=00b26cb8
// @ref ?user_reply_debug_7fdea0@@3PADA=007fdea0
// @ref ?user_reply_format_7fdec4@@3PADA=007fdec4
// @ref __imp__SendDlgItemMessageA@20=0079a484
// @ref __imp__SetEvent@4=0079a234
// @ref __stricmp=00781f20
// @ref _memmove=00783290
// @ref _ntohl@4=00781b72
// @ref _sprintf=007820f4
// @ref _wcscpy=00783689
