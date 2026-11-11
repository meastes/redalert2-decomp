// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// IPX_Call_Back (0x00482110), from ra2-decomp 0x0048D1E0 (YR twin, src/conquer.cpp).
#include "shared/session_state.hpp"
#include "shared/ipx_manager.hpp"
#include "shared/msgloop.hpp"
#include "shared/queue.hpp"
#include "shared/unknown_timed_slots.hpp"
#include "shared/comm_buffer.hpp"
#include "shared/owner_talk.hpp"
#include "shared/name_crc.hpp"
#include "shared/progress_screen.hpp"
#include "shared/sound_play.hpp"
#include "shared/map_view.hpp"
#include "shared/rules.hpp"
#include "shared/udp_interface.hpp"
#include "shared/debug_printf.hpp"
#include "shared/crt_string.hpp"
#include "shared/modem_guest.hpp"

extern int unknown_probe_delay_7d333c;
extern "C" void *__cdecl memset(void *dest, int value, unsigned int count);
#pragma intrinsic(memset)
// @ref ?unknown_probe_delay_7d333c@@3HA=007d333c
// @ref _$E66=00482aa0
// @ref _?$S65@?1??IPX_Call_Back@@YIXXZ@4EA=00850db4
// @ref _?_probe_timer@?1??IPX_Call_Back@@YIXXZ@4V?$CDTimerClass@VSystemTimerClass@@@@A=00850d90
// @ref ??_C@_0CN@LJHO@Sending?5NET_PORT_PROBE?5packets?5i@=007d3560
// @ref ??_C@_0P@PHPO@Sending?5to?5?$CFs?6?$AA@=007d3550
// @ref ??_C@_0CO@ILD@No?5PORT_PROBE?5from?5?$CFs?5in?5the?5las@=007d3520
// @ref ??_C@_0CM@NEF@Sending?5to?5?$CFs?5on?5mangled?5port?5fr@=007d34f4
// @ref ??_C@_0CH@FHLG@Got?5NET_IM_STILL_HERE?5packet?5fro@=007d34cc
// @ref ??_C@_0CP@OFJP@Got?5NET_PORT_PROBE?5packet?5from?5?$CF@=007d349c
// @ref ??_C@_0CK@EONK@Strange?5port?5number?5in?5PORT_PROB@=007d3470
// @ref ??_C@_0DD@OKDA@Adjusting?5port?5number?5for?5player@=007d343c
// @ref ??_C@_0CK@BLDP@Got?5NET_PORT_UNREACHABLE?5message@=007d3410
// @ref ??_C@_0CA@JKCI@Processing?5NET_PROGRESS_REPORT?6?$AA@=007d33f0
// @ref __real@8@4008e100000000000000=0079b608


static inline bool Same_Name(NodeNameType *a, NodeNameType *b)
{
    char *name = Unknown_006FA120(a->Name);
    return _stricmp(name, Unknown_006FA120(b->Name)) == 0;
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
                Unknown_006FA0F0(packet.Name, unknown_session_a3d298.Players[0]->Name, -1);
                Unknown_00623280();
                Debug_Printf("Sending NET_PORT_PROBE packets in Call_Back\n");

                for (int i = 1; i < unknown_session_a3d298.Players.Count(); i++) {
                    unsigned short port = unknown_session_a3d298.unknown_3022.unknown[i];
                    if (port) {
                        Debug_Printf("Sending to %s\n", Unknown_006FA120(unknown_session_a3d298.Players[i]->Name));
                        unknown_ipx_manager_a40978.Send_Global_Message(&packet, 0x5b, 0, &unknown_session_a3d298.Players[i]->Address, port, 0);
                        Windows_Message_Handler();
                        unknown_ipx_manager_a40978.Service();

                        for (int index = 0; index < 8; index++) {
                            if (Same_Name(unknown_session_a3d298.PlayerSlots[index], unknown_session_a3d298.Players[i])) {
                                if (TickCount.Time() - unknown_session_a3d298.unknown_2ff4[index] > 2400) {
                                    Debug_Printf("No PORT_PROBE from %s in the last 40 seconds\n", Unknown_006FA120(unknown_session_a3d298.Players[i]->Name));
                                    if (unknown_session_a3d298.Players[i]->Port != port && unknown_session_a3d298.Players[i]->Port >= 0x400) {
                                        Debug_Printf("Sending to %s on mangled port from pserver\n", Unknown_006FA120(unknown_session_a3d298.Players[i]->Name));
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
                                    Debug_Printf("Got NET_IM_STILL_HERE packet from %s.\n", Unknown_006FA120(unknown_ipx_manager_a40978.Connection_Name(unknown_session_a3d298.GPacket.Chat.ID)));
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
                            if (unknown_session_a3d298.Players[i]->Address.Is_Same_Node(unknown_session_a3d298.GAddress) && _stricmp(Unknown_006FA120(unknown_session_a3d298.Players[i]->Name), unknown_session_a3d298.GPacket.Name) == 0) {
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
                        unknown_object_84e8d0.Unknown_0042EC20(unknown_session_a3d298.GPacket.BeaconText.Text, unknown_session_a3d298.GPacket.BeaconText.House, unknown_session_a3d298.GPacket.BeaconText.Slot, false);
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
                            wchar_t *name = Unknown_006FA180(unknown_session_a3d298.GPacket.Name);
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
                            if (unknown_session_a3d298.Players[i]->Address.Is_Same_Node(unknown_session_a3d298.GAddress) && _stricmp(Unknown_006FA120(unknown_session_a3d298.Players[i]->Name), unknown_session_a3d298.GPacket.Name) == 0) {
                                ProgressScreen.Set_Progress_006201c0(i, unknown_session_a3d298.GPacket.Progress.Percent, Point2D(-1, -1));
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
