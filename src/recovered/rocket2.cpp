// rocket2.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/rocket2.json carries the addresses and the rows that
// stay in their own files.


// Header-static dynamic initializers of the unit at 0x0063ED70 and its
// list globals (OwnerTalkClass_ConnectionListStruct); the other drafts of this plan are emitted by this file.
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/vector_class.hpp"

#include "shared/owner_talk.hpp"

// The unit's OwnerTalkClass object (0x00AB6D18; shared/owner_talk.hpp).
OwnerTalkClass unknown_OwnerTalkClass_00ab6d18;


// Dynamic initializer (0x0063EDA0, _$E5) emitted by HeaderStatics_diagonal_init_0063ed70.cpp; no source of its own.


// Dynamic initializer (0x0063EDC0, _$E8) emitted by HeaderStatics_diagonal_init_0063ed70.cpp; no source of its own.


// Dynamic initializer (0x0063EDE0, _$E11) emitted by HeaderStatics_diagonal_init_0063ed70.cpp; no source of its own.


// Dynamic initializer (0x0063EE00, _$E14) emitted by HeaderStatics_diagonal_init_0063ed70.cpp; no source of its own.


// Dynamic initializer (0x0063EE20, _$E17) emitted by HeaderStatics_diagonal_init_0063ed70.cpp; no source of its own.


// Dynamic initializer (0x0063EE40, _$E20) emitted by HeaderStatics_diagonal_init_0063ed70.cpp; no source of its own.


// Dynamic initializer (0x0063EE80, _$E23) emitted by HeaderStatics_diagonal_init_0063ed70.cpp; no source of its own.


// Dynamic initializer (0x0063EEB0, _$E26) emitted by HeaderStatics_diagonal_init_0063ed70.cpp; no source of its own.


// Dynamic initializer (0x0063EEE0, _$E29) emitted by HeaderStatics_diagonal_init_0063ed70.cpp; no source of its own.


// Dynamic initializer (0x0063EF10, _$E32) emitted by HeaderStatics_diagonal_init_0063ed70.cpp; no source of its own.


// Dynamic initializer (0x0063EF20, _$E35) emitted by HeaderStatics_diagonal_init_0063ed70.cpp; no source of its own.


// Dynamic initializer (0x0063EF40, _$E38) emitted by HeaderStatics_diagonal_init_0063ed70.cpp; no source of its own.


// Dynamic initializer (0x0063EF60, _$E43) emitted by HeaderStatics_diagonal_init_0063ed70.cpp; no source of its own.


// atexit destructor (0x0063EFB0, _$E41) emitted by HeaderStatics_diagonal_init_0063ed70.cpp; no source of its own.


// OwnerTalkClass (shared/owner_talk.hpp): the packet-router talk unit at 0x0063ED70.
#define IPX_ADDRESS_DESTRUCTOR
#include "shared/ipx_address.hpp"
#include "shared/ipx_manager.hpp"
#include "shared/debug_printf.hpp"
#include "shared/crt_string.hpp"
#include "shared/cd_timer.hpp"
#include "shared/system_timer.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/session_state.hpp"
#include "shared/house.hpp"
#include "shared/unknown_player_entry.hpp"
#include "shared/event.hpp"
#include "shared/win32_api.hpp"
#include "shared/win32_window.hpp"
#include "shared/winsock_interface.hpp"
#include "shared/ipx_conn.hpp"
#include "shared/crt_stdio.hpp"

// 0x0063F040
OwnerTalkClass::OwnerTalkClass() : unknown_18(false), RouterProcess(0)
{
}


// 0x0063F0C0
OwnerTalkClass::~OwnerTalkClass()
{
    while (Connections.Count()) {
        delete Connections[0];
        Connections.Delete(0);
    }
}


// 0x0063F130: sends the router the master player list (TALK type 1; players the IPX manager no longer
// knows, other than the local one, are sent inactive) and waits up to 900 ticks for the queue to empty.
bool OwnerTalkClass::Unknown_0063F130()
{
    TalkPacketStruct packet = {0};
    CDTimerClass<SystemTimerClass> timer;
    if (!unknown_18) {
        return false;
    }
    Debug_Printf("About to send master player list to router\n");
    packet.Type = 1;
    packet.Data = Connections.Count();
    for (int index = 0; index < Connections.Count(); index++) {
        packet.Players[index] = *Connections[index];
        if (unknown_ipx_manager_a40978.Connection_Index(Connections[index]->ID) == -1
            && unknown_player_a35db4->ArrayIndex != Connections[index]->ID) {
            Debug_Printf("Marking player %s as inactive\n", Connections[index]->Name);
            packet.Players[index].IsActive = false;
            packet.Players[index].IsHost = false;
        }
    }
    unknown_ipx_manager_a40978.unknown_525b50(&packet, sizeof(packet), true);
    Debug_Printf("Player list sent to router\n");
    timer = 900;
    while (unknown_ipx_manager_a40978.unknown_slot20() > 0 && timer) {
        Service_Game_IO_00481fb0();
    }
    return true;
}


// 0x0063F2A0: rebuilds the connection list from the human houses still playing (packet-router games).
void OwnerTalkClass::Unknown_0063F2A0()
{
    while (Connections.Count()) {
        delete Connections[0];
        Connections.Delete(0);
    }
    for (int house = 0; house < Houses.Count(); house++) {
        HouseClass *hptr = Houses[house];
        if (hptr->Unknown_004F52F0() && !hptr->unknown_13e) {
            ConnectionListStruct *connection = new ConnectionListStruct;
            connection->ID = house;
            strncpy(connection->Name, Wide_To_Narrow_006fa120(hptr->UIName), 20);
            for (int index = 0; index < unknown_session_a3d298.Players.Count(); index++) {
                if (unknown_session_a3d298.Players[index]->ID == house) {
                    NetNumType net;
                    NetNodeType node;
                    unknown_session_a3d298.Players[index]->Address.Get_Address(net, node);
                    connection->Address = *(unsigned long *)node;
                    connection->Port = unknown_node_ids_a402ba[index];
                    break;
                }
            }
            connection->IsActive = true;
            connection->IsHost = (unknown_session_a3d298.MasterPlayerID == hptr->ArrayIndex);
            connection->Mask = 1 << house;
            Connections.Add(connection);
        }
    }
}


// 0x0063F430: the mask of the connection at this address (0xFF: none or broadcast).
unsigned char OwnerTalkClass::Unknown_0063F430(IPXAddressClass *address)
{
    if (address == 0) {
        return 0xFF;
    }
    if (address->Is_Broadcast()) {
        return 0xFF;
    }
    unsigned char mask = 0;
    NetNumType net;
    NetNodeType node;
    address->Get_Address(net, node);
    for (int index = 0; index < Connections.Count(); index++) {
        if (*(unsigned long *)node == Connections[index]->Address) {
            mask = Connections[index]->Mask;
            break;
        }
    }
    return mask;
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// unsigned char OwnerTalkClass::Unknown_0063F4C0(int id) (0x0063F4C0)
// Adopted from ra2-decomp OwnerTalkClass::Mask_From_ID (YR 0x00663D60).
// The mask of the connection with this ID; all bits (0xFF) for -1 or no match.

unsigned char OwnerTalkClass::Unknown_0063F4C0(int id)
{
    unsigned char mask = 0;

    if (id == -1) {
        return 0xFF;
    }
    for (int i = 0; i < Connections.Count(); i++) {
        if (id == Connections[i]->ID) {
            mask = Connections[i]->Mask;
            break;
        }
    }
    if (mask == 0) {
        mask = 0xFF;
    }
    return mask;
}


struct _SECURITY_ATTRIBUTES;
struct _STARTUPINFOA {
    DWORD cb;
    char *lpReserved, *lpDesktop, *lpTitle;
    DWORD dwX, dwY, dwXSize, dwYSize, dwXCountChars, dwYCountChars, dwFillAttribute, dwFlags;
    unsigned short wShowWindow, cbReserved2;
    unsigned char *lpReserved2;
    HANDLE hStdInput, hStdOutput, hStdError;
};
struct _PROCESS_INFORMATION {
    HANDLE hProcess, hThread;
    DWORD dwProcessId, dwThreadId;
};
WIN32_IMPORT void WINAPI GetStartupInfoA(_STARTUPINFOA *info);
WIN32_IMPORT BOOL WINAPI CreateProcessA(char const *, char *, _SECURITY_ATTRIBUTES *, _SECURITY_ATTRIBUTES *, BOOL, DWORD,
                                        void *, char const *, _STARTUPINFOA *, _PROCESS_INFORMATION *);

// 0x0063F510: starts the packet router (mph.exe) for an IP (null: the first player's address, or the
// transport's own when there are no players), sets the forward addresses and waits up to 1800 ticks
// for the router's TALK_HELLO (type 4).
bool OwnerTalkClass::Unknown_0063F510(unsigned char *address)
{
    CDTimerClass<SystemTimerClass> timer;
    IPXAddressClass from;
    _STARTUPINFOA startup;
    _PROCESS_INFORMATION process;
    char command[256];
    char buffer[1024];
    int length;
    unsigned short product;

    Debug_Printf("Starting packet router process\n");
    GetStartupInfoA(&startup);
    if (!PacketTransport->Get_Num_Local_Addresses()) {
        return false;
    }
    if (address == 0) {
        if (unknown_session_a3d298.Players.Count() == 0) {
            unsigned char *ip = (unsigned char *)PacketTransport->Get_Local_Address(0);
            sprintf(command, "mph.exe %d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
            goto create;
        }
        IPXAddressClass *first = &unknown_session_a3d298.Players[0]->Address;
        if (first == 0) {
            goto create;
        }
        NetNumType net;
        NetNodeType node;
        first->Get_Address(net, node);
        sprintf(command, "mph.exe %d.%d.%d.%d", node[0], node[1], node[2], node[3]);
        unknown_forward_address_a3fbb8 = *first;
        unknown_session_a3d298.unknown_292c = 0;
    } else {
        sprintf(command, "mph.exe %d.%d.%d.%d", address[0], address[1], address[2], address[3]);
        IPXAddressClass *first = &unknown_session_a3d298.Players[0]->Address;
        if (first == 0) {
            goto create;
        }
        NetNumType net;
        NetNodeType node;
        first->Get_Address(net, node);
        node[0] = address[0];
        node[1] = address[1];
        node[2] = address[2];
        node[3] = address[3];
        unknown_forward_address_a3fbb8.Set_Address(net, node, 0);
        unknown_forward_address_a3fbb8.Set_Port(*(unsigned short *)&unknown_session_a3d298.Players[0]->Address);
        unknown_session_a3d298.unknown_292c = *(unsigned short *)&unknown_session_a3d298.Players[0]->Address;
        unknown_forward_address_a3fbac = unknown_session_a3d298.Players[0]->Address;
    }
    Debug_Printf("Session.PacketRouterAddressTranslate = %s\n", unknown_forward_address_a3fbb8.As_String());

create:
    BOOL result = CreateProcessA("mph.exe", command, 0, 0, false, 0x200, 0, 0, &startup, &process);
    if (!result) {
        Debug_Printf("CreateProcess failed with error %d, %s\n", GetLastError(), result);
    } else {
        RouterProcess = process.hProcess;
        Debug_Printf("hRouterProcess = %d\n", process.hProcess);
        SetFocus(MainWindow);
        timer = 1800;
        result = false;
        while (timer) {
            Service_Game_IO_00481fb0();
            if (unknown_ipx_manager_a40978.unknown_525bc0(buffer, &length, &from, &product) && buffer[0] == 4) {
                result = true;
                Debug_Printf("Received TALK_HELLO from router\n");
                unknown_18 = true;
                break;
            }
        }
    }
    return result;
}


// The router's messages (0x00AB68E4).
static unsigned char RouterBuffer[1024];

// 0x0063F840: reads the router's messages; a change of address (type 5: id, IP) becomes an ADDRESSCHANGE event.
void OwnerTalkClass::Unknown_0063F840()
{
    unsigned short product;
    int length;
    IPXAddressClass address;
    if (Frame > 0) {
        while (unknown_ipx_manager_a40978.unknown_525bc0(RouterBuffer, &length, &address, &product)) {
            if (RouterBuffer[0] != 5) {
                Debug_Printf("Got spurious message %d from the router\n", RouterBuffer[0]);
            } else {
                Debug_Printf("Received change of address notification.\n");
                int id = (char)RouterBuffer[1];
                NetNumType net;
                NetNodeType node;
                memset(node, 0, sizeof(node));
                *(unsigned long *)node = *(unsigned long *)&RouterBuffer[2];
                IPXAddressClass newaddress;
                newaddress.Set_Address(net, node, 0);
                OutList.Add(EventClass(unknown_player_a35db4->ArrayIndex, &newaddress, id));
            }
        }
    }
}


// 0x0063F9A0: a player's new address.
void OwnerTalkClass::Unknown_0063F9A0(int id, IPXAddressClass *address)
{
    NetNumType net;
    NetNodeType node;
    address->Get_Address(net, node);
    for (int index = 0; index < Connections.Count(); index++) {
        if (Connections[index]->ID == id) {
            Connections[index]->Address = *(unsigned long *)node;
            break;
        }
    }
}


// 0x0063FA00: the address of the named connection (a function-local static; null node if none).
IPXAddressClass *OwnerTalkClass::Unknown_0063FA00(char *name)
{
    static IPXAddressClass address;
    NetNodeType node;
    memset(node, 0, sizeof(node));
    address.Set_Node(node);
    address.Set_Port(0);
    for (int index = 0; index < Connections.Count(); index++) {
        if (_stricmp(Connections[index]->Name, name) == 0) {
            NetNodeType ip;
            *(unsigned long *)ip = Connections[index]->Address;
            address.Set_Node(ip);
            address.Set_Port(Connections[index]->Port);
            break;
        }
    }
    return &address;
}


// 0x0063FAE0: the atexit destructor of the function-local static in 0x0063FA00 (emitted there).


// 0x0063FAF0: the port of the named connection (0: none).
unsigned short OwnerTalkClass::Unknown_0063FAF0(char *name)
{
    for (int index = 0; index < Connections.Count(); index++) {
        if (_stricmp(Connections[index]->Name, name) == 0) {
            return Connections[index]->Port;
        }
    }
    return 0;
}


// 0x0063FB40: the name of the connection at an index (null past the end).
char *OwnerTalkClass::Name_From_Index(int index)
{
    if (index < Connections.Count()) {
        return Connections[index]->Name;
    }
    return 0;
}


// 0x0063FB60: a player's port.
void OwnerTalkClass::Unknown_0063FB60(int id, unsigned short port)
{
    for (int index = 0; index < Connections.Count(); index++) {
        if (Connections[index]->ID == id) {
            Connections[index]->Port = port;
            break;
        }
    }
}


// 0x0063FBA0: the next game host (the session's choice, else the first active non-host);
// when the local player becomes the host of a game of three or more, it restarts the router.
int OwnerTalkClass::Unknown_0063FBA0()
{
    int host = unknown_session_a3d298.unknown_303c;
    int index;
    if (host == -1) {
        for (index = 0; index < Connections.Count(); index++) {
            ConnectionListStruct *connection = Connections[index];
            if (connection->IsActive && !connection->IsHost) {
                host = connection->ID;
                connection->IsHost = true;
            }
        }
    } else {
        for (index = 0; index < Connections.Count(); index++) {
            ConnectionListStruct *connection = Connections[index];
            if (connection->ID == host) {
                connection->IsHost = true;
            } else {
                connection->IsHost = false;
            }
        }
    }
    if (!unknown_session_a3d298.Am_I_Master(0) && host != -1 && host == unknown_player_a35db4->ArrayIndex
        && unknown_session_a3d298.Players.Count() > 2) {
        IPXAddressClass *address = 0;
        for (index = 0; index < unknown_session_a3d298.Players.Count(); index++) {
            if (unknown_session_a3d298.Players[index]->ID == host) {
                address = &unknown_session_a3d298.Players[index]->Address;
                break;
            }
        }
        NetNumType net;
        NetNodeType node;
        address->Get_Address(net, node);
        Unknown_0063F510(node);
    }
    return host;
}


// 0x0063FC90: tells the router to stop (type 3) and waits up to 900 ticks for the send queue to empty.
void OwnerTalkClass::Unknown_0063FC90()
{
    TalkPacketStruct packet = {0};
    CDTimerClass<SystemTimerClass> timer;
    if (unknown_18 && RouterProcess != 0) {
        packet.Type = 3;
        unknown_ipx_manager_a40978.unknown_525b50(&packet, sizeof(packet), true);
        timer = 900;
        while (unknown_ipx_manager_a40978.unknown_slot20() > 0 && timer) {
            Service_Game_IO_00481fb0();
        }
    }
}


// 0x0063FE20: a player left: marks its connection inactive and tells the router.
void OwnerTalkClass::Unknown_0063FE20(int id)
{
    TalkPacketStruct packet = {0};
    for (int index = 0; index < Connections.Count(); index++) {
        ConnectionListStruct *connection = Connections[index];
        if (connection->ID == id) {
            connection->IsActive = false;
            connection->IsHost = false;
        }
    }
    if (unknown_18) {
        packet.Type = 2;
        packet.Data = id;
        Debug_Printf("Sending TALK_REMOVE_PLAYER to router\n");
        unknown_ipx_manager_a40978.unknown_525b50(&packet, sizeof(packet), true);
    }
}


// 0x0063FEB0: TALK_PORT_BLITZ.
void OwnerTalkClass::Unknown_0063FEB0(unsigned char flag)
{
    TalkPacketStruct packet = {0};
    if (unknown_18) {
        packet.Type = 6;
        packet.Data = flag;
        Debug_Printf("Sending TALK_PORT_BLITZ to router\n");
        unknown_ipx_manager_a40978.unknown_525b50(&packet, sizeof(packet), true);
    }
}


extern "C" unsigned char vtable_6401d0[]; extern "C" void __cdecl release_buffer_6401d0(void*); extern "C" void __cdecl release_object_6401d0(void*);
class Leaf_6401d0 { public: void* get(unsigned int flags); };
void* Leaf_6401d0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_6401d0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_6401d0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_6401d0(this); return this; }


extern "C" unsigned char vtable_640220[]; extern "C" void __cdecl release_buffer_640220(void*); extern "C" void __cdecl release_object_640220(void*);
class Leaf_640220 { public: void* get(unsigned int flags); };
void* Leaf_640220::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_640220; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_640220((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_640220(this); return this; }
