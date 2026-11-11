// NON-EXACT DRAFT 0x525F60 IPXManagerClass::Service
// entry 0x525F60; exact-match score 97.99 (misses_index best: the raw seed); this text 73.15 (codex4 r545); plan "draft as-is"
// origin: codex4 r545 typed repair of the Scout915 raw seed (build/class-work/Scout915/sweep/1/00525f60.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1257 (native/drafts.json). Not original bytes.
// Needs IPXManagerClass::CurDataBuf (+0x8C): RA2_IPX_MANAGER_CUR_DATA_BUF, defined by its host unit.
// NON-EXACT behaviour repair of restored Scout915 raw seed, IPXManagerClass::Service.
// Derived from the exact target full body00525F60-005269E2 and pass6a;
// TibSun ipxmgr.cpp Service supplies family names, not acceptance evidence.
#include "shared/ipx_manager.hpp"
#include "shared/session_state.hpp"
#include "shared/owner_talk.hpp"
#include "shared/house_screen.hpp"
#include "shared/debug_printf.hpp"
#include "shared/crt_string.hpp"
extern bool use_router_a402d0;
extern bool unknown_a402d8;
extern bool unknown_session_a3f616;
extern "C" char s_Adding_ACK_packet__d_from__s_____007de1b0[];
extern "C" char s_Adding_DATA_ACK_packet__d_from___007de134[];
extern "C" char s_Adding_DATA_NOACK_packet__d_from_007de170[];
extern "C" char s_Adding_____packet__d_from__s_____007de1e8[];
extern "C" char s_Adding_packet_from__s____d_to_gl_007de2e0[];
extern "C" char s_Error___Connection__d_has_gone_b_007de060[];
extern "C" char s_Frame__d__Router_apparently_chan_007de220[];
extern "C" char s_GlobalChannel_recive_buffer_over_007de0d0[];
extern "C" char s_Multicast_channel_has_gone_bad_007de084[];
extern "C" char s_NAT_Alert___port_number_fiddling_007de2b4[];
extern "C" char s_Player__s_is_having_his_port_tra_007de284[];
extern "C" char s_Receive_buffer_overflow__d_007de118[];
extern "C" char s_Reecive_buffer_overflow__d_007de0fc[];
extern "C" char s_RouterChannel_recive_buffer_over_007de0a4[];
extern "C" char s_Set_address_for_connection__d_to_007de25c[];

int IPXManagerClass::Service()
{
    int result=1;
    IPXAddressClass address;
    unsigned short port;
    int packet_length;
    int receive_length;
    int address_length;
    char receive_buffer[1024];
    char address_buffer[128];
    if (PacketTransport) {
        do {
            receive_length=sizeof(receive_buffer);
            address_length=sizeof(address_buffer);
            port=0;
            packet_length=PacketTransport->Read(receive_buffer,&receive_length,address_buffer,&address_length,&port);
            if (!packet_length) break;
            address=*(IPXAddressClass *)address_buffer;
            CurDataBuf=receive_buffer;
            CommHeaderType *packet=(CommHeaderType *)receive_buffer;
            if (packet->MagicNumber==GlobalChannel->MagicNum) {
                Debug_Printf(s_Adding_packet_from__s____d_to_gl_007de2e0,address.As_String_00523840(),(unsigned int)port);
                if (!unknown_525dc0(packet,receive_buffer,packet_length,(IPXAddressClass *)address_buffer,port)) {
                    if (!GlobalChannel->Receive_Packet(receive_buffer,packet_length,&address,port)) {
                        ++unknown_a4;
                        Debug_Printf(s_GlobalChannel_recive_buffer_over_007de0d0,unknown_a4);
                        break;
                    }
                    if (unknown_game_mode_a3d298==4 && use_router_a402d0) {
                        int player;
                        for (player=0;player<unknown_players_a3fa7c.Count();++player) {
                            if (unknown_players_a3fa7c[player]->Address.unknown_523690(address) && unknown_node_ids_a402ba[player]==port)
                                goto next_packet;
                        }
                        if (unknown_players_a3fa7c.Count()!=0) {
                            Debug_Printf(s_NAT_Alert___port_number_fiddling_007de2b4);
                            if ((unsigned int)packet_length>15 && packet->Code!=PACKET_ACK && (unsigned int)packet_length>62) {
                                int command=*(int *)(CurDataBuf+0x10);
                                if (command==0x27 || command==0x29 || command==0x0d) {
                                    char player_name[22]={0};
                                    strncpy(player_name,CurDataBuf+0x14,20);
                                    for (player=0;player<unknown_players_a3fa7c.Count();++player) {
                                        if (_stricmp(Unknown_006FA120(unknown_players_a3fa7c[player]->Name),player_name)==0) {
                                            if (!unknown_players_a3fa7c[player]->Address.unknown_523690(address) ||
                                                *(unsigned short *)unknown_players_a3fa7c[player]->Address.NetworkNumber!=port) {
                                                Debug_Printf(s_Player__s_is_having_his_port_tra_007de284,player_name,(unsigned int)port);
                                                unsigned short old_port=unknown_node_ids_a402ba[player];
                                                unknown_node_ids_a402ba[player]=port;
                                                unknown_OwnerTalkClass_00ab6d18.Unknown_0063FB60(unknown_players_a3fa7c[player]->ID,port);
                                                GlobalChannel->Discard_Packets_To(&address,old_port);
                                                GlobalChannel->Discard_Packets_To(&address,*(unsigned short *)unknown_players_a3fa7c[player]->Address.NetworkNumber);
                                                unknown_players_a3fa7c[player]->Address=address;
                                                unknown_players_a3fa7c[player]->Address.unknown_523960(port);
                                                unknown_OwnerTalkClass_00ab6d18.Unknown_0063F9A0(unknown_players_a3fa7c[player]->ID,&unknown_players_a3fa7c[player]->Address);
                                                int slot=unknown_session_a3d298.Unknown_00672F90(player);
                                                unknown_player_slots_a3fa98[slot]->Address=address;
                                                slot=unknown_session_a3d298.Unknown_00672F90(player);
                                                unknown_player_slots_a3fa98[slot]->Address.unknown_523960(port);
                                                for (int connection=0;connection<NumConnections;++connection) {
                                                    if (_stricmp(Unknown_006FA120(Connection[connection]->Name),player_name)==0) {
                                                        Connection[connection]->Address=address;
                                                        Connection[connection]->Address.unknown_523960(port);
                                                        Debug_Printf(s_Set_address_for_connection__d_to_007de25c,connection,Connection[connection]->Address.As_String_00523840());
                                                        break;
                                                    }
                                                }
                                                if (!unknown_session_a3f616 && NumConnections!=0 && unknown_players_a3fa7c[player]->ID<Houses.Count() &&
                                                    unknown_session_a3d298.Am_I_Master(Houses[unknown_players_a3fa7c[player]->ID])) {
                                                    char *new_address=address.As_String_00523840();
                                                    char *old_address=unknown_50->Address.As_String_00523840();
                                                    Debug_Printf(s_Frame__d__Router_apparently_chan_007de220,Frame,old_address,new_address);
                                                    unknown_50->Address=address;
                                                    unknown_50->Address.unknown_523960(port);
                                                    unknown_address_a3db2c=address;
                                                    unknown_address_a3db2c.unknown_523960(port);
                                                    unknown_forward_address_a3fbac=address;
                                                    unknown_forward_address_a3fbac.unknown_523960(port);
                                                    *(unsigned short *)&unknown_forward_port_a3fbc4=port;
                                                }
                                            }
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            } else if (packet->MagicNumber==unknown_4c->MagicNum) {
                if (unknown_flag_a3fba8 && !unknown_4c->Receive_Packet(receive_buffer,packet_length,&address,0)) {
                    ++unknown_a4;
                    Debug_Printf(s_RouterChannel_recive_buffer_over_007de0a4,unknown_a4);
                    break;
                }
            } else if (packet->MagicNumber==ProductID &&
                       !unknown_525dc0(packet,receive_buffer,packet_length,(IPXAddressClass *)address_buffer,port)) {
                if (!unknown_flag_a3fba8 || packet->unknown_03!=0xff) {
                    int connection;
                    for (connection=0;connection<NumConnections;++connection) {
                        bool matches;
                        if (!unknown_flag_a3fba8) {
check_address:
                            matches=Connection[connection]->Address==address;
                        } else {
                            matches=unknown_OwnerTalkClass_00ab6d18.Unknown_0063F4C0(Connection[connection]->ID)==packet->unknown_03;
                            if (!matches && !unknown_flag_a3fba8) goto check_address;
                        }
                        if (matches) {
                            if (!Connection[connection]->Receive_Packet(receive_buffer,packet_length)) {
                                ++unknown_a4;
                                packet_length=0;
                                Debug_Printf(s_Reecive_buffer_overflow__d_007de0fc,unknown_a4);
                            }
                            goto check_router_address;
                        }
                    }
                    if (unknown_game_mode_a3d298==4 && unknown_a40774==0 && Frame>8 && !unknown_flag_a3fba8 &&
                        (unsigned int)packet_length>15 && packet->Code==PACKET_DATA_NOACK && (signed char)receive_buffer[14]==0x19) {
                        int id=(signed char)receive_buffer[16];
                        for (int player=1;player<unknown_players_a3fa7c.Count();++player) {
                            if (unknown_players_a3fa7c[player]->ID==id) {
                                unknown_players_a3fa7c[player]->Address=address;
                                if (Connection_Index(id)!=-1) Connection[Connection_Index(id)]->Address=address;
                                break;
                            }
                        }
                    }
                } else {
                    if (packet->Code==PACKET_ACK) {
                        unknown_50->Receive_Packet(receive_buffer,packet_length);
                    } else if (NumConnections>0) {
                        char const *format;
                        if (packet->Code==PACKET_DATA_ACK) format=s_Adding_DATA_ACK_packet__d_from___007de134;
                        else if (packet->Code==PACKET_DATA_NOACK) format=s_Adding_DATA_NOACK_packet__d_from_007de170;
                        else if (packet->Code==PACKET_ACK) format=s_Adding_ACK_packet__d_from__s_____007de1b0;
                        else format=s_Adding_____packet__d_from__s_____007de1e8;
                        char *text=address.As_String_00523840();
                        Debug_Printf(format,packet->PacketID,text,(unsigned int)port);
                        if (!Connection[0]->Receive_Packet(receive_buffer,packet_length)) {
                            ++unknown_a4;
                            packet_length=0;
                            Debug_Printf(s_Receive_buffer_overflow__d_007de118,unknown_a4);
                        }
                    }
check_router_address:
                    if (unknown_game_mode_a3d298==4 && unknown_flag_a3fba8 && address!=unknown_50->Address) {
                        for (int player=1;player<unknown_players_a3fa7c.Count();++player) {
                            if (unknown_players_a3fa7c[player]->Address.unknown_523690(unknown_50->Address) &&
                                unknown_session_a3d298.Am_I_Master(Houses[unknown_players_a3fa7c[player]->ID])) {
                                char *new_address=address.As_String_00523840();
                                char *old_address=unknown_50->Address.As_String_00523840();
                                Debug_Printf(s_Frame__d__Router_apparently_chan_007de220,Frame,old_address,new_address);
                                unknown_players_a3fa7c[player]->Address=address;
                                unknown_50->Address=address;
                                unknown_address_a3db2c=address;
                                unknown_forward_address_a3fbac=address;
                                unknown_OwnerTalkClass_00ab6d18.Unknown_0063F9A0(unknown_players_a3fa7c[player]->ID,&address);
                            }
                        }
                    }
                }
            }
next_packet:;
        } while (packet_length);
    }
    if (GlobalChannel && PacketTransport && !GlobalChannel->Service()) {
        GlobalChannel->Discard_Undeliverable_Packets();
        result=0;
    }
    if (unknown_flag_a3fba8) {
        if (unknown_4c) {
            if (!unknown_4c->Service()) {
                unknown_4c->Discard_Undeliverable_Packets();
                result=0;
            }
            if (!unknown_a402d8) unknown_OwnerTalkClass_00ab6d18.Unknown_0063F840();
        }
        if (unknown_flag_a3fba8 && unknown_50 && !unknown_50->Service()) {
            Debug_Printf(s_Multicast_channel_has_gone_bad_007de084);
            result=0;
        }
    }
    if (PacketTransport) {
        for (int connection=0;connection<NumConnections;++connection) {
            if (!Connection[connection]->Service()) {
                result=0;
                unknown_a8=Connection[connection]->ID;
                Debug_Printf(s_Error___Connection__d_has_gone_b_007de060,unknown_a8);
            }
        }
    }
    if (result) unknown_a8=-1;
    return result;
}
