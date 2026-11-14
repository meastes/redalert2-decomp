// netdlg.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/netdlg.json carries the addresses and the rows that
// stay in their own files.


#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_DELETE_OBJECT
#define VECTOR_IMPLEMENT_DYNAMIC_DELETE_INDEX
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/lobby.hpp"
#include "shared/netdlg.hpp"
#include "shared/ipx_manager.hpp"
#include "shared/winsock_interface.hpp"
#include "shared/queue.hpp"
#include "shared/event.hpp"
#include "shared/house.hpp"
#include "shared/house_type.hpp"
#include "shared/rules.hpp"
#include "shared/map_view.hpp"
#include "shared/string_table.hpp"
#include "shared/unknown_player_entry.hpp"
#include "shared/gadget.hpp"
#include "shared/vector_class.hpp"
#include "shared/session_state.hpp"
#include "shared/owner_talk.hpp"
#include "shared/ipx_conn.hpp"
#include "shared/connection.hpp"
#include "shared/debug_printf.hpp"
#include "shared/crt_string.hpp"


// Header-static dynamic initializers of the unit at 0x005BB350: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x005BB350-0x005BB531); the other
// drafts of this plan are emitted by these includes.
// The unit expands the vector accessors and DynamicVectorClass::Delete inline
// (Destroy_Connection 0x005BB5D0).


// Dynamic initializer (0x005BB370, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_005bb350.cpp; no source of its own.


// Dynamic initializer (0x005BB3A0, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_005bb350.cpp; no source of its own.


// Dynamic initializer (0x005BB3C0, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_005bb350.cpp; no source of its own.


// Dynamic initializer (0x005BB3E0, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_005bb350.cpp; no source of its own.


// Dynamic initializer (0x005BB400, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_005bb350.cpp; no source of its own.


// Dynamic initializer (0x005BB420, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_005bb350.cpp; no source of its own.


// Dynamic initializer (0x005BB440, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_005bb350.cpp; no source of its own.


// Dynamic initializer (0x005BB480, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_005bb350.cpp; no source of its own.


// Dynamic initializer (0x005BB4B0, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_005bb350.cpp; no source of its own.


// Dynamic initializer (0x005BB4E0, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_005bb350.cpp; no source of its own.


// Dynamic initializer (0x005BB510, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_005bb350.cpp; no source of its own.


// Dynamic initializer (0x005BB520, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_005bb350.cpp; no source of its own.


// Red Alert's Init_Network as NetDlg.CPP has it (0x005BB540; netdlg2.cpp's copy 0x005BC460 also
// resets the lobby lists).
bool Init_Network_005bb540()
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
    return true;
}


// Red Alert's Shutdown_Network as NetDlg.CPP has it (0x005BB5A0; netdlg2.cpp's copy 0x005BC4D0 also
// frees the lobby entries).
void Shutdown_Network_005bb5a0()
{
    unknown_host_name_a3f618[0] = 0;
    if (PacketTransport != 0) {
        delete PacketTransport;
    }
    PacketTransport = 0;
}


// void __fastcall Destroy_Connection_005bb5d0(int id, int error) (0x005BB5D0)
// TibSun/Red Alert netdlg.cpp Destroy_Connection: a network player has gone.
// Shows "connection lost" (error 1) or "left the game" (error 0), deletes the
// IPX connection, sends REMOVEPLAYER (or, before the first frame, hands the
// house to the computer) or, for a player who left, tells the packet router;
// when this player is the game master behind the packet router, hands the
// router to the next host; then removes the player from the session list.

extern bool unknown_flag_a40d1c;    // 0x00A40D1C (session +0x3A84: trace the connections)
extern bool unknown_a402d8;

void __fastcall Destroy_Connection_005bb5d0(int id, int error)
{
    int i;
    HouseClass *housep;
    wchar_t txt[80];

    housep = Houses[id];

    if (unknown_flag_a40d1c) {
        Debug_Printf("Destroying connection for house %d (%ls)\n", id, housep->UIName);
    }

    if (!housep || !housep->unknown_134) {
        return;
    }

    txt[0] = 0;
    if (error == 1) {
        swprintf(txt, StringTable::Load_String("TXT_CONNECTION_LOST", 0, "D:\\Ra2\\NetDlg.CPP", 0x1CF), housep->UIName);
    } else if (error == 0) {
        swprintf(txt, StringTable::Load_String("TXT_LEFT_GAME", 0, "D:\\Ra2\\NetDlg.CPP", 0x1D1), housep->UIName);
    }

    if (wcslen(txt)) {
        unknown_session_a3d298.Messages.Add_Message(0, 0, txt, housep->Type->Color, 0x4046, Rules->MessageDelay * 900, false);
        Map.Flag_To_Redraw(false);
    }

    unknown_ipx_manager_a40978.Delete_Connection(id);

    if (error) {
        if (Frame == 0) {
            housep->unknown_func_4e7e80();
        } else {
            OutList.Add(EventClass(unknown_player_a35db4->ArrayIndex, 0x23, id));
        }
    } else {
        if (unknown_game_mode_a3d298 == 4 && unknown_b2a7ec) {
            housep->unknown_func_4e7e80();
        }
        if (unknown_game_mode_a3d298 == 4 && unknown_flag_a3fba8) {
            unknown_OwnerTalkClass_00ab6d18.Unknown_0063FE20(id);
        }
    }

    if (unknown_game_mode_a3d298 == 4 && unknown_flag_a3fba8 && unknown_session_a3d298.Am_I_Master(housep)) {
        unknown_a402d8 = true;
        int host = unknown_OwnerTalkClass_00ab6d18.Unknown_0063FBA0();
        if (host != -1) {
            HouseClass *hostp = Houses[host];
            if (hostp) {
                if (unknown_session_a3d298.Players.Count() > 2) {
                    IPXAddressClass *address = 0;
                    for (i = 0; i < unknown_session_a3d298.Players.Count(); i++) {
                        if (unknown_session_a3d298.Players[i]->ID == host) {
                            address = &unknown_session_a3d298.Players[i]->Address;
                            unknown_session_a3d298.unknown_292c = unknown_node_ids_a402ba[i];
                            break;
                        }
                    }
                    unsigned char network[4];
                    unsigned char node[6];
                    address->Get_Address(network, node);
                    unknown_forward_address_a3fbac.Set_Address(network, node, 0);
                    Debug_Printf("Session.PacketRouterAddress = %s\n", unknown_forward_address_a3fbac.As_String(0));
                    Debug_Printf("Session.PacketRouterPort = %d\n", unknown_session_a3d298.unknown_292c);
                    unknown_ipx_manager_a40978.unknown_5277c0(&unknown_forward_address_a3fbac);
                }
                wcscpy(unknown_session_a3d298.MasterPlayerName, hostp->UIName);
                Debug_Printf("New game host is %ls\n", unknown_session_a3d298.MasterPlayerName);
                unknown_session_a3d298.MasterPlayerID = host;
                if (unknown_session_a3d298.Am_I_Master(0)) {
                    unknown_OwnerTalkClass_00ab6d18.Unknown_0063F130();
                }
            }
        }
    }

    for (i = 0; i < unknown_session_a3d298.Players.Count(); i++) {
        if (unknown_session_a3d298.Players[i]->ID == id) {
            delete unknown_session_a3d298.Players[i];
            unknown_session_a3d298.Players.Delete(unknown_session_a3d298.Players[i]);
            unknown_session_a3d298.Unknown_00672F20(i);
            if (unknown_game_mode_a3d298 == 4 && unknown_flag_a3fba8) {
                unknown_OwnerTalkClass_00ab6d18.Unknown_0063FE20(id);
            }
            break;
        }
    }

    unknown_session_a3d55c--;
}


// unsigned long __fastcall Compute_Name_CRC(char const *name) (0x005BB9D0)
// TibSun netdlg.cpp shape: the CRC of the upper-cased name.

unsigned long __fastcall Compute_Name_CRC(char const *name)
{
    char buf[80];
    unsigned long crc = 0;
    int i;

    strcpy(buf, name);
    _strupr(buf);

    for (i = 0; (unsigned)i < strlen(buf); i++) {
        Add_CRC(&crc, (unsigned long)buf[i]);
    }

    return crc;
}


// unsigned long __fastcall Compute_Name_CRC(wchar_t const *name) (0x005BBA50)
// The wide-character Compute_Name_CRC: the CRC of the upper-cased name.

unsigned long __fastcall Compute_Name_CRC(wchar_t const *name)
{
    wchar_t buf[80];
    unsigned long crc = 0;
    int i;

    wcscpy(buf, name);
    _wcsupr(buf);

    for (i = 0; (unsigned)i < wcslen(buf); i++) {
        Add_CRC(&crc, (unsigned long)buf[i]);
    }

    return crc;
}


// void __fastcall Clear_Listbox(ListClass *list) (0x005BBAD0)
// TibSun netdlg.cpp shape: deletes every text item, then flags the list for redraw.

void __fastcall Clear_Listbox(ListClass *list)
{
    wchar_t *item;

    while (list->Count()) {
        item = list->Get_Item(0);
        list->Remove_Item(item);
        delete[] item;
    }
    list->Flag_To_Redraw();
}
