// conquer.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/conquer.json carries the addresses and the rows that
// stay in their own files.


#define RA2_CONQUER_IPX_CALL_BACK    // Conquer.CPP: IPX_Call_Back 0x00482110 members (session_state.hpp, ipx_address.hpp, unknown_globals.hpp, unknown_player_entry.hpp)
#define UNKNOWN_BEACONS_DECLARE_TEXT     // IPX_Call_Back 0x00482110: the beacon text and removal messages
#define UNKNOWN_BEACONS_DECLARE_MEMBERS
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/wol_sink.hpp"    // unknown_a72c68; ahead of house.hpp (after it, rc_ptr<WDTTerritory> COMDATs are emitted)
#include "shared/house.hpp"
#include "shared/mouse.hpp"
#include "shared/event.hpp"
#include "shared/string_table.hpp"
#include "shared/win32_window.hpp"
#include "shared/game_controls_dialog.hpp"
#include "shared/game_options_dialog.hpp"
#include "shared/queue.hpp"
#include "shared/options.hpp"
#include "shared/id_source.hpp"
#include "shared/map_view.hpp"
#include "shared/serial_progress.hpp"
#include "shared/movie.hpp"
#include "shared/session_state.hpp"
#include "shared/ipx_manager.hpp"
#include "shared/msgloop.hpp"
#include "shared/unknown_timed_slots.hpp"
#include "shared/comm_buffer.hpp"
#include "shared/owner_talk.hpp"
#include "shared/name_crc.hpp"
#include "shared/progress_screen.hpp"
#include "shared/sound_play.hpp"
#include "shared/rules.hpp"
#include "shared/udp_interface.hpp"
#include "shared/debug_printf.hpp"
#include "shared/crt_string.hpp"
#include "shared/modem_guest.hpp"
#include "shared/ccini_class.hpp"
#include "shared/anim_type.hpp"
#include "shared/object.hpp"
#include "shared/techno_type.hpp"
#include "shared/win32_api.hpp"
#include "shared/house_type.hpp"
#include "shared/coord.hpp"
#include "shared/winsock_interface.hpp"
#include "shared/ipx_address.hpp"
#include "shared/network_statistics.hpp"


// Header-static dynamic initializers of the unit at 0x00481650: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x00481650-0x00481831); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x00481670, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_00481650.cpp; no source of its own.


// Dynamic initializer (0x004816A0, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_00481650.cpp; no source of its own.


// Dynamic initializer (0x004816C0, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_00481650.cpp; no source of its own.


// Dynamic initializer (0x004816E0, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_00481650.cpp; no source of its own.


// Dynamic initializer (0x00481700, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_00481650.cpp; no source of its own.


// Dynamic initializer (0x00481720, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_00481650.cpp; no source of its own.


// Dynamic initializer (0x00481740, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_00481650.cpp; no source of its own.


// Dynamic initializer (0x00481780, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_00481650.cpp; no source of its own.


// Dynamic initializer (0x004817B0, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_00481650.cpp; no source of its own.


// Dynamic initializer (0x004817E0, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_00481650.cpp; no source of its own.


// Dynamic initializer (0x00481810, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_00481650.cpp; no source of its own.


// Dynamic initializer (0x00481820, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_00481650.cpp; no source of its own.


// 0x00481840: function not yet identified
//
// Body: 1 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void FUN_00481840(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00481840 {

void  FUN_00481840(void)
{
    return;
}

}  // namespace seed_00481840


// void __fastcall Special_Dialog_00481850() (0x00481850): runs the dialog the player asked for
// (unknown_int_a40d48, Red Alert's SpecialDialog) until none is pending, with the game focus,
// mouse and session timing released around it. Called from the Conquer.CPP game loop.

// The Sound Controls dialog: Run (0x00688CD0) logs "SoundControls: GameSpeed = %d, ..." and shows
// dialog 0xD6 (0xB8 in one mode); it does not use its receiver.
class SoundControlsDialog {
public:
    void Run_00688CD0();
};
extern bool unknown_flag_007d88e0;      // 0x007D88E0 set when the dialog may open while the player is out
extern bool unknown_flag_00866250;      // 0x00866250 set by the game options dialog's result 5
void Unknown_004DD7D0();                                        // 0x004DD7D0 (a dialog)
void __fastcall Unknown_00634A00(MouseClass *map);              // 0x00634A00 (a dialog)
void __fastcall Unknown_0063B090(ScenarioClass *scenario);    // 0x0063B090
void __fastcall Unknown_00672DE0(SessionClass *session); // 0x00672DE0
void Unknown_0073C230();                                        // 0x0073C230 (a dialog)
// An OK/Cancel message box (MPLayer.CPP); nonzero on OK (0x005A7430).
int __fastcall OK_Cancel_Message_005A7430(wchar_t const *message);

void __fastcall Special_Dialog_00481850()
{
    if (unknown_int_a40d48 == 0) {
        return;
    }
    if (unknown_game_mode_a3d298 != 0) {
        if (unknown_player_a35db4->unknown_140 || unknown_player_a35db4->unknown_13f || unknown_player_a35db4->unknown_13e
            || (!unknown_flag_007d88e0 && !unknown_player_a35db4->unknown_13d)) {
            unknown_int_a40d48 = 0;
            return;
        }
        if (!unknown_flag_007d88e0) {
            unknown_flag_007d88e0 = true;
        }
    }
    Focus_Loss_0065c040();
    WWMouse->Release_Mouse();
    while (WWMouse->Get_Mouse_State() != 0) {
        WWMouse->Show_Mouse();
    }
    while (unknown_int_a40d48 != 0) {
        switch (unknown_int_a40d48) {
        case 9:
            Unknown_0063B090(unknown_id_source_a3d290);
            if (unknown_int_a40d48 == 9) {
                unknown_int_a40d48 = 0;
            }
            break;
        case 8:
            Unknown_00634A00(&Map);
            if (unknown_int_a40d48 == 8) {
                unknown_int_a40d48 = 0;
            }
            break;
        case 1:
            Unknown_004DD7D0();
            if (unknown_int_a40d48 == 1) {
                unknown_int_a40d48 = 0;
            }
            break;
        case 5:
            GameControlsDialog().Run();
            if (unknown_int_a40d48 == 5) {
                unknown_int_a40d48 = 1;
            }
            break;
        case 6:
            SoundControlsDialog().Run_00688CD0();
            unknown_int_a40d48 = 5;
            break;
        case 4:
            Options.Unknown_005DA360();
            unknown_int_a40d48 = 5;
            break;
        case 3:
            switch (Run_Game_Options_Dialog()) {
            case 3:
                OutList.Add(EventClass(unknown_player_a35db4->ArrayIndex, 8));
                unknown_flag_007d88e0 = false;
            case 2:
                Queue_Exit();
                break;
            case 4:
                OutList.Add(EventClass(unknown_player_a35db4->ArrayIndex, 8));
                unknown_flag_007d88e0 = false;
                break;
            case 5:
                unknown_flag_00866250 = true;
                break;
            }
            unknown_int_a40d48 = 0;
            break;
        case 2:
            if (unknown_a72c68 != 0) {
                Queue_Exit();
            } else if (!unknown_player_a35db4->unknown_13d && !unknown_player_a35db4->unknown_13f
                       && !unknown_player_a35db4->unknown_140 && !unknown_player_a35db4->unknown_13e) {
                if (OK_Cancel_Message_005A7430(StringTable::Load_String("TXT_SURRENDER", 0, "D:\\Ra2\\Conquer.CPP", 487))) {
                    OutList.Add(EventClass(unknown_player_a35db4->ArrayIndex, 8));
                    unknown_flag_007d88e0 = false;
                }
            }
            unknown_int_a40d48 = 0;
            break;
        case 7:
            Unknown_0073C230();
            unknown_int_a40d48 = 0;
            break;
        }
    }
    UpdateWindow(MainWindow);
    if (unknown_id_source_a3d290->unknown_2306) {
        WWMouse->Hide_Mouse();
    }
    WWMouse->Capture_Mouse();
    Unknown_00672DE0(&unknown_session_a3d298);
    Focus_Restore_0065c140();
}


void __cdecl Process_Serial_Progress_Packets_00481f00()
{
    unsigned char packet[252];
    int length;
    if (unknown_game_mode_a3d298 == 2 || (serial_session_flags_00a3fac0 & 1)) {
        serial_manager_00a40a28.Service();
        if (unknown_id_source_a3d290->unknown_22fc) {
            while (serial_manager_00a40a28.Get_Message(packet, &length) > 0) {
                // Preserve the original DWORD sender read and overlapping unaligned
                // signed progress read. Complete packet format remains unknown.
                if (*(int *)packet == 108 &&
                    (*(unsigned int *)(packet + 24) & 255) != serial_local_player_00a3fb38) {
                    ProgressScreen.Set_Progress_Percent(
                        1, (double)*(int *)(packet + 25), Point2D(-1, -1));
                }
            }
        }
    }
}


extern DynamicVectorClass<MovieHandle *> queued_movies_00af8fd8;
void __cdecl Audio_Update();
void __fastcall IPX_Call_Back();
extern int unknown_login_state_00b2abe4;
extern long last_online_poll_00850e10;
extern "C" long __cdecl time(long *result);

void __fastcall Service_Game_IO_00481fb0()
{
    Audio_Update();
    if (queued_movies_00af8fd8.Count()>0) {
        MovieHandle *movie=queued_movies_00af8fd8[0];
        if (movie) movie->Waiting();
    }
    if (unknown_game_mode_a3d298==3 || unknown_game_mode_a3d298==4)
        IPX_Call_Back();
    if (unknown_game_mode_a3d298!=1) {
        if (unknown_game_mode_a3d298!=2) goto online;
    } else if (!(serial_session_flags_00a3fac0&1)) return;
        {
            unsigned char packet[252];
            int length;
            serial_manager_00a40a28.Service();
            if (unknown_id_source_a3d290->unknown_22fc) {
                while (serial_manager_00a40a28.Get_Message(packet,&length)>0) {
                    if (*(int *)packet==108 &&
                        (*(unsigned int *)(packet+24)&255)!=serial_local_player_00a3fb38) {
                        ProgressScreen.Set_Progress_Percent(
                            1,(double)*(int *)(packet+25),Point2D(-1,-1));
                    }
                }
            }
        }
online:
    if (unknown_game_mode_a3d298==4 && !unknown_b2abda && time(0)!=last_online_poll_00850e10) {
        if (unknown_chat_a72cfc && !unknown_login_state_00b2abe4)
            unknown_chat_a72cfc->unknown_slot03();
        if (unknown_ladder_client_a72d00)
            unknown_ladder_client_a72d00->unknown_slot07_004820dd();
        last_online_poll_00850e10=time(0);
    }
}


// 0x00482100: function not yet identified
//
// Body: 8 bytes (frozen inventory bound).
// Referenced from 48 sites (computed call, data).
// Ghidra prototype: undefined1 FUN_00482100(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00482100 {

extern void  Service_Game_IO_00481fb0(void);

unsigned char  FUN_00482100(void)
{
    Service_Game_IO_00481fb0();
    return 0;
}

}  // namespace seed_00482100


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// IPX_Call_Back (0x00482110), from ra2-decomp 0x0048D1E0 (YR twin, src/conquer.cpp).

extern int unknown_probe_delay_7d333c;
extern "C" void *__cdecl memset(void *dest, int value, unsigned int count);
#pragma intrinsic(memset)


static inline bool Same_Name(NodeNameType *a, NodeNameType *b)
{
    char *name = Wide_To_Narrow_006fa120(a->Name);
    return _stricmp(name, Wide_To_Narrow_006fa120(b->Name)) == 0;
}

void __fastcall IPX_Call_Back()
{
    static CDTimerClass<SystemTimerClass> _probe_timer(unknown_probe_delay_7d333c);

    Windows_Message_Handler();
    unknown_ipx_manager_a40978.Service();

    if (unknown_session_a3d298.Type == 4 && !unknown_session_a3d298.NetOpen) {
        if ((_probe_timer == 0 || unknown_session_a3d298.unknown_3014) && unknown_session_a3d298.Players.Count()) {
            Windows_Message_Handler();
            unknown_ipx_manager_a40978.Service();
            if (unknown_ipx_manager_a40978.Global_Num_Send() == 0) {
                UnknownGlobalPacket packet;
                memset(&packet, 0, 0x5b);
                packet.Command = 0x29;
                WideToCharString_006fa0f0(packet.Name, unknown_session_a3d298.Players[0]->Name, -1);
                Unknown_00623280();
                Debug_Printf("Sending NET_PORT_PROBE packets in Call_Back\n");

                for (int i = 1; i < unknown_session_a3d298.Players.Count(); i++) {
                    unsigned short port = unknown_session_a3d298.unknown_3022.unknown[i];
                    if (port) {
                        Debug_Printf("Sending to %s\n", Wide_To_Narrow_006fa120(unknown_session_a3d298.Players[i]->Name));
                        unknown_ipx_manager_a40978.Send_Global_Message(&packet, 0x5b, 0, &unknown_session_a3d298.Players[i]->Address, port, 0);
                        Windows_Message_Handler();
                        unknown_ipx_manager_a40978.Service();

                        for (int index = 0; index < 8; index++) {
                            if (Same_Name(unknown_session_a3d298.PlayerSlots[index], unknown_session_a3d298.Players[i])) {
                                if (TickCount.Time() - unknown_session_a3d298.unknown_2ff4[index] > 2400) {
                                    Debug_Printf("No PORT_PROBE from %s in the last 40 seconds\n", Wide_To_Narrow_006fa120(unknown_session_a3d298.Players[i]->Name));
                                    if (unknown_session_a3d298.Players[i]->Port != port && unknown_session_a3d298.Players[i]->Port >= 0x400) {
                                        Debug_Printf("Sending to %s on mangled port from pserver\n", Wide_To_Narrow_006fa120(unknown_session_a3d298.Players[i]->Name));
                                        unknown_ipx_manager_a40978.Send_Global_Message(&packet, 0x5b, 0, &unknown_session_a3d298.Players[i]->Address, unknown_session_a3d298.Players[i]->Port, 0);
                                    }
                                }
                                break;
                            }
                        }
                    }
                }

                CDTimerClass<SystemTimerClass> timer(120);
                while (unknown_ipx_manager_a40978.Global_Num_Send() > 0 && timer > 0) {
                    Windows_Message_Handler();
                    unknown_ipx_manager_a40978.Service();
                }

                _probe_timer = unknown_probe_delay_7d333c;
                unknown_session_a3d298.unknown_3014 = false;
                unknown_ipx_manager_a40978.unknown_527800();
            }
        }
    }

    if (!unknown_session_a3d298.NetOpen) {
        while (unknown_ipx_manager_a40978.Get_Global_Message(&unknown_session_a3d298.GPacket, &unknown_session_a3d298.GPacketlen, &unknown_session_a3d298.GAddress, &unknown_session_a3d298.GProductID)) {

            if (unknown_session_a3d298.GProductID == 0xAA03) {

                switch (unknown_session_a3d298.GPacket.Command) {

                    case 0x1C:
                    {
                        Unknown_00627190(&unknown_session_a3d298.GPacket, &unknown_session_a3d298.GAddress);
                        break;
                    }

                    case 0x27:
                    {
                        if (unknown_session_a3d298.Type == 4 && unknown_session_a3d298.ViaPacketRouter) {
                            for (int i = 0; i < unknown_ipx_manager_a40978.Num_Connections(); i++) {
                                if (unknown_ipx_manager_a40978.Connection_ID(i) == unknown_session_a3d298.GPacket.Chat.ID) {
                                    Debug_Printf("Got NET_IM_STILL_HERE packet from %s.\n", Wide_To_Narrow_006fa120(unknown_ipx_manager_a40978.Connection_Name(unknown_session_a3d298.GPacket.Chat.ID)));
                                    unknown_stamp_timed_slot_00623300(i);
                                    break;
                                }
                            }
                        }
                        break;
                    }

                    case 0x29:
                    {
                        Debug_Printf("Got NET_PORT_PROBE packet from %s - port = %d\n", unknown_session_a3d298.GPacket.Name, unknown_session_a3d298.GAddress.Get_Port());
                        for (int i = 0; i < unknown_session_a3d298.Players.Count(); i++) {
                            if (unknown_session_a3d298.Players[i]->Address.Is_Same_Node(unknown_session_a3d298.GAddress) && _stricmp(Wide_To_Narrow_006fa120(unknown_session_a3d298.Players[i]->Name), unknown_session_a3d298.GPacket.Name) == 0) {
                                for (int index = 0; index < 8; index++) {
                                    if (Same_Name(unknown_session_a3d298.PlayerSlots[index], unknown_session_a3d298.Players[i])) {
                                        unknown_session_a3d298.unknown_2ff4[index] = TickCount.Time();
                                        break;
                                    }
                                }
                                unsigned short port = unknown_session_a3d298.GAddress.Get_Port();
                                if (port != unknown_session_a3d298.Players[i]->Address.Get_Port() || port != unknown_session_a3d298.unknown_3022.unknown[i]) {
                                    Debug_Printf("Strange port number in PORT_PROBE packet\n");
                                    int old = unknown_session_a3d298.unknown_3022.unknown[i];
                                    if (port == 0) {
                                        port = 1234;
                                    }
                                    unknown_session_a3d298.unknown_3022.unknown[i] = port;
                                    unknown_session_a3d298.Players[i]->Address.Set_Port(port);
                                    Debug_Printf("Adjusting port number for player %s from %d to %d\n", unknown_session_a3d298.GPacket.Name, old, port);
                                }
                            }
                        }
                        break;
                    }

                    case 0x20:
                    {
                        unknown_object_84e8d0.Unknown_0042E370(unknown_session_a3d298.GPacket.Beacon.House, CoordStruct(unknown_session_a3d298.GPacket.Beacon.X, unknown_session_a3d298.GPacket.Beacon.Y, unknown_session_a3d298.GPacket.Beacon.Z), unknown_session_a3d298.GPacket.Beacon.Slot);
                        break;
                    }

                    case 0x21:
                    {
                        unknown_object_84e8d0.Unknown_0042E990(unknown_session_a3d298.GPacket.Beacon.House, unknown_session_a3d298.GPacket.Beacon.Slot);
                        break;
                    }

                    case 0x22:
                    {
                        unknown_object_84e8d0.EditBeaconMessage_0042ec20(unknown_session_a3d298.GPacket.BeaconText.Text, unknown_session_a3d298.GPacket.BeaconText.House, unknown_session_a3d298.GPacket.BeaconText.Slot, false);
                        break;
                    }

                    case 0x0A:
                    {
                        for (int i = 0; i < unknown_ipx_manager_a40978.Num_Connections(); i++) {
                            int id = unknown_ipx_manager_a40978.Connection_ID(i);
                            if (unknown_session_a3d298.GAddress == (*unknown_ipx_manager_a40978.Connection_Address(id))) {
                                Destroy_Connection_005bb5d0(id, 0);
                            }
                        }
                        break;
                    }

                    case 0x2F:
                    {
                        Debug_Printf("Got NET_PORT_UNREACHABLE message from %s\n", unknown_session_a3d298.GPacket.Name);
                        if (unknown_session_a3d298.Type == 4 && PacketTransport != 0) {
                            NetNumType net;
                            NetNodeType node;
                            unsigned short port;
                            IPXAddressClass *address = unknown_OwnerTalkClass_00ab6d18.Unknown_0063FA00(unknown_session_a3d298.GPacket.Name);
                            address->Get_Address(net, node, &port);
                            ((UDPInterfaceClass *)PacketTransport)->Add_Peer((unsigned long *)node, port);
                        }
                        break;
                    }

                    case 0x0C:
                    {
                        bool msg_ok = false;
                        if (!unknown_session_a3d298.NetProtect) {
                            msg_ok = true;
                        } else {
                            if (unknown_session_a3d298.GPacket.Message.unknown_114 == Unknown_005bba50(unknown_session_a3d298.HostName_2380)) {
                                msg_ok = true;
                            } else {
                                msg_ok = false;
                            }
                        }
                        if (msg_ok && unknown_session_a3d298.ViaPacketRouter) {
                            msg_ok = (unknown_session_a3d298.GPacket.Message.unknown_2f & unknown_OwnerTalkClass_00ab6d18.Unknown_0063F4C0(unknown_session_a3d298.Players[0]->ID)) != 0;
                        }

                        if (msg_ok) {
                            wchar_t *name = Narrow_To_Wide_006fa180(unknown_session_a3d298.GPacket.Name);
                            unknown_session_a3d298.Messages.Add_Message(name,
                                unknown_session_a3d298.GPacket.Message.unknown_110,
                                unknown_session_a3d298.GPacket.Message.Buf,
                                unknown_session_a3d298.Unknown_006715D0(unknown_session_a3d298.GPacket.Message.unknown_110),
                                0x4046,
                                int(Rules->MessageDelay * 900), false);

                            Voc_Play(Rules->IncomingMessage, 1.0f, 0x2000, 0);

                            Map.Flag_To_Redraw(2);

                            wcscpy(unknown_session_a3d298.LastMessage, unknown_session_a3d298.GPacket.Message.Buf);
                        }
                        break;
                    }

                    case 0x1D:
                    {
                        bool msg_ok = false;
                        if (!unknown_session_a3d298.NetProtect) {
                            msg_ok = true;
                        } else {
                            if (unknown_session_a3d298.GPacket.Taunt.NameCRC == Unknown_005bba50(unknown_session_a3d298.HostName_2380)) {
                                msg_ok = true;
                            } else {
                                msg_ok = false;
                            }
                        }
                        if (msg_ok && unknown_session_a3d298.ViaPacketRouter) {
                            msg_ok = (unknown_session_a3d298.GPacket.Taunt.Recipients & unknown_OwnerTalkClass_00ab6d18.Unknown_0063F4C0(unknown_session_a3d298.Players[0]->ID)) != 0;
                        }
                        if (msg_ok) {
                            if ((unknown_session_a3d298.LANTaunts && unknown_session_a3d298.Type == 3) || (unknown_session_a3d298.WOLTaunts && unknown_session_a3d298.Type == 4)) {
                                Unknown_Play_Taunt_00715fb0(unknown_session_a3d298.GPacket.Taunt.ID);
                            }
                        }
                        break;
                    }

                    case 0x0F:
                    {
                        Debug_Printf("Processing NET_PROGRESS_REPORT\n");
                        for (int i = 0; i < unknown_session_a3d298.Players.Count(); i++) {
                            if (unknown_session_a3d298.Players[i]->Address.Is_Same_Node(unknown_session_a3d298.GAddress) && _stricmp(Wide_To_Narrow_006fa120(unknown_session_a3d298.Players[i]->Name), unknown_session_a3d298.GPacket.Name) == 0) {
                                ProgressScreen.Set_Progress_Percent(i, unknown_session_a3d298.GPacket.Progress.Percent, Point2D(-1, -1));
                                break;
                            }
                        }
                        break;
                    }

                    case 0x10:
                    {
                        break;
                    }

                    case 0x13:
                    {
                        break;
                    }

                    default:
                        Answer_Query_005c05f0(&unknown_session_a3d298.GPacket, &unknown_session_a3d298.GAddress);
                        break;
                }
            }

            Windows_Message_Handler();
            unknown_ipx_manager_a40978.Service();
        }
    }
}


// The exit-time destructor of IPX_Call_Back 0x00482110's function-local static `_probe_timer` (0x00482AA0, `_$E66`):
// CDTimerClass<SystemTimerClass> has an empty destructor, so the body is a lone ret. The guarded static's
// initialisation registers it with atexit.
// No source of its own: emitted by this unit (Conquer.CPP) for the static in IPX_Call_Back.


extern "C" int __cdecl _stricmp(char const *left,char const *right);
extern char const *SourceNames[5];
SourceType __fastcall Source_From_Name(char const *name)
{
    if (name) {
        for (int index=0;index<5;++index) {
            if (_stricmp(SourceNames[index],name)==0) return (SourceType)index;
        }
    }
    return (SourceType)-1;
}


extern "C" int __cdecl _stricmp(char const *left,char const *right);
TheaterType __fastcall Theater_From_Name(char const *name)
{
    for (int index=0;index<3;++index)
        if (_stricmp(name,Theaters[index].Name)==0) return (TheaterType)index;
    return (TheaterType)-1;
}


// Converts navigation keys to eight-way facing indices, ignoring modifier bits.
int __fastcall Facing_From_Key_00482b50(unsigned int key)
{
    switch (key & 0xfffff8ff) {
    case 0x25: return 6; // left: west
    case 0x27: return 2; // right: east
    case 0x26: return 0; // up: north
    case 0x28: return 4; // down: south
    case 0x24: return 7; // home: northwest
    case 0x21: return 1; // page up: northeast
    case 0x23: return 5; // end: southwest
    case 0x22: return 3; // page down: southeast
    }
    return -1;
}


// Existing shared/display.hpp receiver type, incomplete here: no new owner class.
class BeaconManagerClass;
extern BeaconManagerClass unknown_object_84e8d0;
// Observed one-DWORD ECX argument, no stack arguments; original source form unknown.
void __fastcall Unknown_006f6780(unsigned int value);
// Observed one-pointer ECX receiver, no stack arguments; member/free source form unknown.
void __fastcall Unknown_0042e830(BeaconManagerClass *receiver);
void __cdecl Clear_Selected_Objects_00482bc0()
{
    while (unknown_objects_a40c60.Count()!=0)
        unknown_objects_a40c60[0]->Unselect();
    Unknown_006f6780(0);
    Unknown_0042e830(&unknown_object_84e8d0);
}


class UnitTypeClass;
class AircraftTypeClass;
class BuildingTypeClass;
class InfantryTypeClass;
extern DynamicVectorClass<UnitTypeClass *> UnitTypes;
extern DynamicVectorClass<AircraftTypeClass *> AircraftTypes;
extern DynamicVectorClass<BuildingTypeClass *> BuildingTypes;
extern DynamicVectorClass<InfantryTypeClass *> InfantryTypes;
// Every listed concrete type has its ObjectTypeClass base at offset zero,
// as established by the actual shared class layouts; no concrete members are read.
// Lookup accepts either an object's kind or the corresponding type's kind.
ObjectTypeClass *__fastcall Unknown_00482C00(int kind,int index)
{
    switch (kind) {
    case 1: case 40: return (ObjectTypeClass *)UnitTypes[index];
    case 6: case 7: return (ObjectTypeClass *)BuildingTypes[index];
    case 15: case 16: return (ObjectTypeClass *)InfantryTypes[index];
    case 2: case 3: return (ObjectTypeClass *)AircraftTypes[index];
    }
    return 0;
}


typedef void *HMODULE;
typedef int (WINAPI *FARPROC)();
WIN32_IMPORT HMODULE WINAPI GetModuleHandleA(char const *name);
WIN32_IMPORT FARPROC WINAPI GetProcAddress(HMODULE module,char const *name);
WIN32_IMPORT BOOL WINAPI GetDiskFreeSpaceA(char const *root,DWORD *sectors,DWORD *bytes,DWORD *free_clusters,DWORD *total_clusters);
typedef BOOL (WINAPI *QueryDiskSpaceFn)(char const *,LARGE_INTEGER *,LARGE_INTEGER *,LARGE_INTEGER *);
extern double const disk_high_word_scale_0079de10;
extern double const disk_bytes_to_kb_0079de08;
extern char const disk_check_log_007d3680[];
extern char const disk_kernel_module_007d3670[];
extern char const disk_extended_name_007d365c[];
extern char const disk_extended_log_007d3640[];
extern char const disk_query_error_007d360c[];
extern char const disk_free_log_007d35f0[];
extern char const disk_proc_error_007d35c0[];
extern char const disk_module_error_007d3590[];
// Preserves the original signed interpretation of both 32-bit output halves.
unsigned int Query_Free_Disk_Kilobytes_00482c80()
{
    Debug_Printf(disk_check_log_007d3680);
    HMODULE module=GetModuleHandleA(disk_kernel_module_007d3670);
    if (module) {
        QueryDiskSpaceFn query=(QueryDiskSpaceFn)GetProcAddress(module,disk_extended_name_007d365c);
        if (query) {
            Debug_Printf(disk_extended_log_007d3640);
            LARGE_INTEGER available;
            LARGE_INTEGER total_free;
            LARGE_INTEGER total;
            if (!(unsigned char)query(0,&available,&total,&total_free)) {
                Debug_Printf(disk_query_error_007d360c,GetLastError(),0);
            } else {
                unsigned int kilobytes=(unsigned int)(((double)available.HighPart*disk_high_word_scale_0079de10+(LONG)available.LowPart)*disk_bytes_to_kb_0079de08);
                Debug_Printf(disk_free_log_007d35f0,kilobytes>>10);
                return kilobytes;
            }
        } else Debug_Printf(disk_proc_error_007d35c0,GetLastError(),0);
    } else Debug_Printf(disk_module_error_007d3590);
    DWORD sectors;
    DWORD bytes;
    DWORD free_clusters;
    DWORD total_clusters;
    if (GetDiskFreeSpaceA(0,&sectors,&bytes,&free_clusters,&total_clusters)) {
        unsigned int kilobytes=((sectors*bytes)>>10)*free_clusters;
        Debug_Printf(disk_free_log_007d35f0,kilobytes>>10);
        return kilobytes;
    }
    return 0;
}


extern "C" int __cdecl _stricmp(char const *left,char const *right);
extern char const *CrateNames[19];
CrateType __fastcall Crate_From_Name(char const *name)
{
    if (name) {
        for (int index=0;index<19;++index) {
            if (_stricmp(name,CrateNames[index])==0) return (CrateType)index;
        }
    }
    return (CrateType)0;
}


int __fastcall Owner_From_Name(char const *name)
{
    int result=0;
    int index=HouseTypeClass::Find_Index(name);
    if (index!=-1) result=(int)(1U << index);
    return result;
}


// Copies a sentinel-terminated list of cells, bounded by the requested capacity.
// The original also writes output[-1] when capacity is nonpositive.
void __fastcall Copy_Cell_List_00482e10(Cell const *input,int capacity,Cell *output)
{
    if (input && output) {
        for (;capacity>0;--capacity) {
            *output=*input;
            if (output->X==0x7fff && output->Y==0x7fff) return;
            ++output;
            ++input;
        }
        output[-1]=Cell(0x7fff,0x7fff);
    }
}


extern "C" int __cdecl _stricmp(char const *left,char const *right);
extern DynamicVectorClass<char const *> unknown_names_a71050;
extern char const none_name_007cdecc[];
int __fastcall unknown_name_index_482e60(char const *name)
{
    if (name && _stricmp(none_name_007cdecc,name)!=0) {
        for (int index=0;index<unknown_names_a71050.Count();++index)
            if (_stricmp(name,unknown_names_a71050[index])==0) return index;
    }
    return -1;
}


extern "C" int __cdecl _stricmp(char const *left,char const *right);
extern char const *LandNames[12];
extern char const none_name_007cdecc[];
LandType __fastcall Land_From_Name(char const *name)
{
    // Original compares the sentinel before checking for a null input.
    if (_stricmp(none_name_007cdecc,name)!=0 && name) {
        for (int index=0;index<12;++index) {
            if (_stricmp(LandNames[index],name)==0) return (LandType)index;
        }
    }
    return (LandType)-1;
}


// 0x00482af0: function not yet identified
//
// Body: 19 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined * __fastcall FUN_00482af0(uint param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00482af0 {

extern "C" unsigned int DAT_007ce9b4;
extern "C" void* PTR_s_North_0079ab50;

unsigned char * __fastcall FUN_00482af0(unsigned int index)
{
    if (index < 5) {
        return (unsigned char *)((&PTR_s_North_0079ab50)[index]);
    }
    return (unsigned char *)(&DAT_007ce9b4);
}

}  // namespace seed_00482af0


class Leaf_482e00 { public: void get(); };
void Leaf_482e00::get() {  }


// char const *Name_From_Land(LandType value) -- 0x00482F00
// The INI name of the value, "<none>" out of range.
extern char const *LandNames[12];
extern char const none_name_007cdecc[];

char const *__fastcall Name_From_Land(LandType value)
{
    if ((unsigned)value < 12) {
        return LandNames[value];
    }
    return none_name_007cdecc;
}


extern "C" int __cdecl _stricmp(char const *left,char const *right);
extern char const *SpeedNames[8];
SpeedType __fastcall Speed_From_Name(char const *name)
{
    if (name) {
        for (int index=0;index<8;++index) {
            if (_stricmp(SpeedNames[index],name)==0) return (SpeedType)index;
        }
    }
    return (SpeedType)-1;
}


// char const *Name_From_Speed(SpeedType value) -- 0x00482F60
// The INI name of the value, "<none>" out of range.
extern char const *SpeedNames[8];
extern char const none_name_007cdecc[];

char const *__fastcall Name_From_Speed(SpeedType value)
{
    if ((unsigned)value < 8) {
        return SpeedNames[value];
    }
    return none_name_007cdecc;
}


extern "C" int __cdecl _stricmp(char const *left,char const *right);
extern char const *LayerNames[5];
LayerType __fastcall Layer_From_Name(char const *name)
{
    if (name) {
        for (int index=0;index<5;++index) {
            if (_stricmp(LayerNames[index],name)==0) return (LayerType)index;
        }
    }
    return (LayerType)-1;
}


// char const *Name_From_Layer(LayerType value) -- 0x00482FC0
// The INI name of the value, "<none>" out of range.
extern char const *LayerNames[5];
extern char const none_name_007cdecc[];

char const *__fastcall Name_From_Layer(LayerType value)
{
    if ((unsigned)value < 5) {
        return LayerNames[value];
    }
    return none_name_007cdecc;
}


struct _iobuf;
extern "C" _iobuf *__cdecl fopen(char const *name, char const *mode);
extern "C" int __cdecl fprintf(_iobuf *file, char const *format, ...);
extern "C" int __cdecl fclose(_iobuf *file);
extern int unknown_game_mode_a3d298;
extern unsigned int accumulated_fps_00a6eb20;
extern unsigned int fps_sample_count_00a6eb24;
extern int max_max_ahead_00a3d578;
extern int latency_setting_00a3fba4;

extern char const stats_filename[];
extern char const stats_mode[];
extern char const stats_frames[];
extern char const stats_average_fps[];
extern char const stats_max_ahead[];
extern char const stats_latency[];
extern char const stats_speed[];
extern char const stats_local_address[];
extern char const stats_name[];
extern char const stats_address[];
extern char const stats_max_average_round_trip[];
extern char const stats_max_round_trip[];
extern char const stats_resends[];
extern char const stats_frame_sync_stalls[];
extern char const stats_command_count_stalls[];
extern char const stats_lost[];
extern char const stats_percent_lost[];

void __cdecl Write_Multiplayer_Statistics_00482fe0()
{
    if (unknown_game_mode_a3d298 == 4) {
        _iobuf *file = fopen(stats_filename, stats_mode);
        if (file) {
            fprintf(file, stats_frames, Frame);
            unsigned int average = fps_sample_count_00a6eb24 ? accumulated_fps_00a6eb20 / fps_sample_count_00a6eb24 : 0;
            fprintf(file, stats_average_fps, average);
            fprintf(file, stats_max_ahead, max_max_ahead_00a3d578);
            fprintf(file, stats_latency, latency_setting_00a3fba4);
            fprintf(file, stats_speed, Options.GameSpeed);
            if (PacketTransport) {
                for (int index=0; index<PacketTransport->Get_Num_Local_Addresses(); ++index) {
                    unsigned int *address=(unsigned int *)PacketTransport->Get_Local_Address(index);
                    if (address) {
                        unsigned int ip=*address;
                        fprintf(file,stats_local_address,ip&255,(ip>>8)&255,(ip>>16)&255,ip>>24);
                    }
                }
            }
            for (int player=0; player<8; ++player) {
                UnknownNetworkPlayerStatistics &statistics=player_statistics_00a3d584[player];
                if (statistics.unknown_00[0]) {
                    fprintf(file,stats_name,statistics.unknown_00);
                    fprintf(file,stats_address,statistics.Address.As_String(0));
                    fprintf(file,stats_max_average_round_trip,statistics.MaxAverageRoundTrip);
                    fprintf(file,stats_max_round_trip,statistics.MaxRoundTrip);
                    fprintf(file,stats_resends,statistics.Resends);
                    fprintf(file,stats_frame_sync_stalls,statistics.FrameSyncStalls);
                    fprintf(file,stats_command_count_stalls,statistics.CommandCountStalls);
                    fprintf(file,stats_lost,statistics.Lost);
                    fprintf(file,stats_percent_lost,statistics.PercentLost);
                }
            }
            fclose(file);
        }
    }
}
