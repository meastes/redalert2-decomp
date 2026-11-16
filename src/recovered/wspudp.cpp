/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/
// Function comment blocks from mischa85/RedAlert2 392e904 src/wspudp.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.
// Parameter names from mischa85/RedAlert2 392e904 src/wspudp.cpp where both define the function (names only).
// Local variable names from mischa85/RedAlert2 392e904 src/wspudp.cpp where both functions declare the same local types (names only).

// wspudp.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/wspudp.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/udp_interface.hpp"
#include "shared/crt_string.hpp"
#include "shared/crt_memory.hpp"
#include "shared/session_state.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/random2.hpp"
#include "shared/unknown_player_entry.hpp"


// Header-static dynamic initializers of the unit at 0x0076BE60:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x0076BE60-0x0076C046); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x0076BE90, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_0076be60.cpp; no source of its own.


// Dynamic initializer (0x0076BEB0, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_0076be60.cpp; no source of its own.


// Dynamic initializer (0x0076BED0, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_0076be60.cpp; no source of its own.


// Dynamic initializer (0x0076BEF0, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_0076be60.cpp; no source of its own.


// Dynamic initializer (0x0076BF10, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_0076be60.cpp; no source of its own.


// Dynamic initializer (0x0076BF30, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_0076be60.cpp; no source of its own.


// Dynamic initializer (0x0076BF70, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_0076be60.cpp; no source of its own.


// Dynamic initializer (0x0076BFA0, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_0076be60.cpp; no source of its own.


// Dynamic initializer (0x0076BFD0, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_0076be60.cpp; no source of its own.


// Dynamic initializer (0x0076C000, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_0076be60.cpp; no source of its own.


// Dynamic initializer (0x0076C010, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_0076be60.cpp; no source of its own.


// Dynamic initializer (0x0076C030, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_0076be60.cpp; no source of its own.


/***********************************************************************************************
 * UDPInterfaceClass::UDPInterfaceClass -- Class constructor.                                  *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    8/5/97 12:11PM ST : Created                                                              *
 *=============================================================================================*/
// (0x0076C050)

UDPInterfaceClass::UDPInterfaceClass()
{
    unknown_3f2fc = 0;
    NumSockets = 0;
    NumUnreachable = 0;
    unknown_3f334 = INVALID_SOCKET;
}


/***********************************************************************************************
 * UDPIC::~UDPInterfaceClass -- UDPInterface class destructor                                  *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    10/9/97 12:17PM ST : Created                                                             *
 *=============================================================================================*/
// (0x0076C0F0)
// Frees the broadcast and local addresses, then shuts Winsock down.

UDPInterfaceClass::~UDPInterfaceClass()
{
    Clear_Broadcast_Addresses();
    while (LocalAddresses.Count()) {
        delete LocalAddresses[0];
        LocalAddresses.Delete(0);
    }
    Close();
}


/// <summary>
/// Discards every broadcast address the interface knows about.
/// This routine is used to forget the addresses handed over by Set_Broadcast_Address,
/// so that a later broadcast will not reach a stale set of destinations.
/// </summary>
// (0x0076C230, slot 12)
// Frees the broadcast addresses.

void UDPInterfaceClass::Clear_Broadcast_Addresses()
{
    while (BroadcastAddresses.Count()) {
        delete BroadcastAddresses[0];
        BroadcastAddresses.Delete(0);
    }
}


/***********************************************************************************************
 * UDPInterfaceClass::Set_Broadcast_Address -- Sets the address to send broadcast packets to   *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    ptr to address in decimal dot format. i.e. xxx.xxx.xxx.xxx                        *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    8/5/97 12:12PM ST : Created                                                              *
 *=============================================================================================*/
// (0x0076C290, slot 11)
// Adds a broadcast address given as "a.b.c.d" (the default port when 0).

void UDPInterfaceClass::Set_Broadcast_Address(char *address, unsigned short port)
{
    unsigned char *ip = new unsigned char[4];
    int a, b, c, d;
    sscanf(address, "%d.%d.%d.%d", &a, &b, &c, &d);
    ip[0] = a;
    ip[1] = b;
    ip[2] = c;
    ip[3] = d;
    if (port == 0) {
        port = unknown_default_port_7f60d0;
    }
    BroadcastPorts[BroadcastAddresses.Count()] = port;
    BroadcastAddresses.Add(ip);
}


/// <summary>
/// RA2
/// </summary>
// (0x0076C350, slot 23)
// (Re)opens the primary datagram socket and binds it to the port (the
// default game port when 0), with lingering off.

bool UDPInterfaceClass::OpenSocket(unsigned short port)
{
    if (Socket != INVALID_SOCKET) {
        closesocket(Socket);
        Socket = INVALID_SOCKET;
    }
    Debug_Printf("About to open primary UDP socket\n");
    Socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (Socket == INVALID_SOCKET) {
        return false;
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    if (port) {
        unknown_3f2fc = port;
        addr.sin_port = htons(port);
    } else {
        unknown_3f2fc = 0;
        addr.sin_port = htons(unknown_default_port_7f60d0);
    }
    addr.sin_addr = htonl(INADDR_ANY);
    Debug_Printf("About to bind the UDP socket to port %d\n", port);
    if (bind(Socket, &addr, sizeof(addr)) == SOCKET_ERROR) {
        Debug_Printf("Bind failed with error code %d\n", WSAGetLastError());
        Close_Socket();
        return false;
    }

    linger nolinger;
    nolinger.l_onoff = 0;
    nolinger.l_linger = 0;
    setsockopt(Socket, SOL_SOCKET, SO_LINGER, (char *)&nolinger, sizeof(nolinger));
    WinsockInterfaceClass::Set_Socket_Options(INVALID_SOCKET);
    Debug_Printf("Primary UDP Socket init complete\n");
    return true;
}


// bool UDPInterfaceClass::Open_Spare_Socket(unsigned short port) (0x0076C480, slot 26)
// Opens another datagram socket bound to the port, unless the port is 0 or
// already has one.

bool UDPInterfaceClass::Open_Spare_Socket(unsigned short port)
{
    if (!port) {
        return false;
    }
    for (int i = 0; i < NumSockets; i++) {
        if (SocketPorts[i] == port) {
            return false;
        }
    }

    Debug_Printf("About to open spare UDP socket\n");
    SOCKET spare = socket(AF_INET, SOCK_DGRAM, 0);
    if (spare == INVALID_SOCKET) {
        return false;
    }
    Debug_Printf("Got spare socket %d\n", spare);

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr = htonl(INADDR_ANY);
    Debug_Printf("About to bind the UDP socket to port %d\n", port);
    if (bind(spare, &addr, sizeof(addr)) == SOCKET_ERROR) {
        closesocket(spare);
        Debug_Printf("Failed to bind spare socket to port %d\n", port);
        return false;
    }

    SocketPorts[NumSockets] = port;
    Sockets[NumSockets] = spare;
    NumSockets++;
    WinsockInterfaceClass::Set_Socket_Options(spare);
    Debug_Printf("Spare UDP Socket init complete\n");
    return true;
}


// void UDPInterfaceClass::Close_Spare_Socket(unsigned short port) (0x0076C5B0, slot 27)
// Closes the spare socket bound to the port and closes up the list.

void UDPInterfaceClass::Close_Spare_Socket(unsigned short port)
{
    if (port) {
        for (int i = 0; i < NumSockets; i++) {
            if (SocketPorts[i] == port) {
                Debug_Printf("Closing spare socket %d\n", Sockets[i]);
                closesocket(Sockets[i]);
                if (Sockets[i] == unknown_3f334) {
                    unknown_3f334 = INVALID_SOCKET;
                }
                for (int j = i; j < NumSockets - 1; j++) {
                    Sockets[j] = Sockets[j + 1];
                    SocketPorts[j] = SocketPorts[j + 1];
                }
                NumSockets--;
                return;
            }
        }
    }
}


/// <summary>
/// RA2
/// </summary>
// (0x0076C670, slot 29)
// Selects the socket bound to a non-default port (else none).

void UDPInterfaceClass::Select_Port_Socket(unsigned short port)
{
    if (port && port != unknown_default_port_7f60d0) {
        for (int i = 0; i < NumSockets; i++) {
            if (SocketPorts[i] == port) {
                unknown_3f334 = Sockets[i];
                return;
            }
        }
    }
    unknown_3f334 = INVALID_SOCKET;
}


/// <summary>
/// RA2
/// </summary>
// (0x0076C6D0, slot 25)

bool UDPInterfaceClass::Init_Ports()
{
    if (Init()) {
        NumSockets = 0;
        return true;
    }
    return false;
}


/***********************************************************************************************
 * UDPInterfaceClass::Open_Socket -- Opens a socket for communications via the UDP protocol    *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Socket number to use. Not required for this protocol.                             *
 *                                                                                             *
 * OUTPUT:   True if socket was opened OK                                                      *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    8/5/97 12:13PM ST : Created                                                              *
 *=============================================================================================*/
// (0x0076C6F0, slot 15)
// Starts Winsock if needed, opens the primary socket on the default port,
// records this host's addresses as the local addresses and opens the
// alternate socket on the alternate port.

bool UDPInterfaceClass::Open_Socket(int)
{
    if (!WinsockInitialised && !Init_Ports()) {
        return false;
    }
    OpenSocket(unknown_default_port_7f60d0);

    char name[128];
    Debug_Printf("About to query the host name\n");
    if (gethostname(name, sizeof(name))) {
        Debug_Printf("Host name is '%s'\n", name);
    }
    Debug_Printf("About to call gethostbyname\n");
    hostent *host = gethostbyname(name);
    if (!host) {
        Debug_Printf("gethostbyname failed! Error code %d\n", WSAGetLastError());
    }

    while (LocalAddresses.Count()) {
        delete LocalAddresses[0];
        LocalAddresses.Delete(0);
    }
    unsigned long **list = (unsigned long **)host->h_addr_list;
    while (*list) {
        unsigned long address = **list++;
        Debug_Printf("Found local address: %d.%d.%d.%d\n", address & 0xFF, (address >> 8) & 0xFF,
            (address >> 16) & 0xFF, address >> 24);
        unsigned char *local = new unsigned char[4];
        *(unsigned long *)local = address;
        LocalAddresses.Add(local);
    }

    Debug_Printf("About to open alternate UDP socket\n");
    Socket2 = socket(AF_INET, SOCK_DGRAM, 0);
    if (Socket2 == INVALID_SOCKET) {
        return false;
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(AlternateListenPort);
    addr.sin_addr = htonl(INADDR_ANY);
    Debug_Printf("About to bind the UDP socket\n");
    if (bind(Socket2, &addr, sizeof(addr)) == SOCKET_ERROR) {
        Close_Socket();
        return false;
    }

    linger nolinger;
    nolinger.l_onoff = 0;
    nolinger.l_linger = 0;
    setsockopt(Socket2, SOL_SOCKET, SO_LINGER, (char *)&nolinger, sizeof(nolinger));
    WinsockInterfaceClass::Set_Socket_Options(Socket2);
    Debug_Printf("Alternate UDP Socket init complete\n");
    return true;
}


/// <summary>
/// RA2
/// </summary>
// (0x0076C960, slot 3)
// A write to a broadcast address becomes a broadcast.

void UDPInterfaceClass::WriteTo(void *buffer, int buffer_len, void *address, int address_len, bool alternate, unsigned char flag, unsigned short port)
{
    if (address && ((IPXAddressClass *)address)->Is_Broadcast()) {
        Broadcast(buffer, buffer_len, alternate);
        return;
    }
    WinsockInterfaceClass::WriteTo(buffer, buffer_len, address, address_len, alternate, flag, port);
}


/***********************************************************************************************
 * UDPIC::Broadcast -- Send data via the Winsock socket                                        *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    ptr to buffer containing data to send                                             *
 *           length of data to send                                                            *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    3/20/96 3:00PM ST : Created                                                              *
 *=============================================================================================*/
// (0x0076C9C0, slot 4)
// A broadcast goes to every broadcast address as its own checksummed
// packet, each followed by a write notification.

void UDPInterfaceClass::Broadcast(void *buffer, int buffer_len, bool alternate)
{
    for (int i = 0; i < BroadcastAddresses.Count(); i++) {
        WinsockBufferType *packet = Get_New_Out_Buffer(alternate);
        memcpy(packet->Buffer, buffer, buffer_len);
        packet->BufferLen = buffer_len;
        packet->IsBroadcast = true;
        memset(packet->Address, 0, sizeof(packet->Address));
        *(unsigned long *)&packet->Address[4] = *(unsigned long *)BroadcastAddresses[i];
        packet->SrcPort = BroadcastPorts[i];
        Build_Packet_CRC(packet);
        SOCKET send_socket;
        if (alternate) {
            AltBuffers.Add(packet);
            send_socket = Socket2;
        } else {
            OutBuffers.Add(packet);
            send_socket = Socket;
        }
        SendMessageA((HWND)unknown_window_b263d8, Protocol_Event_Message(send_socket), 0, 2);
        Windows_Message_Handler();
    }
}


/// <summary>
/// RA2
/// </summary>
// (0x0076CB30, slot 7)
// The base sockets, then read/write notifications on every spare socket.

bool UDPInterfaceClass::Start_Listening()
{
    if (WinsockInterfaceClass::Start_Listening()) {
        for (int i = 0; i < NumSockets; i++) {
            if (WSAAsyncSelect(Sockets[i], unknown_window_b263d8, Protocol_Event_Message(-1), 3) == SOCKET_ERROR) {
                Debug_Printf("Async select failed on spare socket.\n");
                return false;
            }
        }
    }
    Windows_Message_Handler();
    return true;
}


/// <summary>
/// RA2
/// </summary>
// (0x0076CBA0, slot 8)

void UDPInterfaceClass::Stop_Listening()
{
    WinsockInterfaceClass::Stop_Listening();
    Close_All_Port_Sockets();
}


/// <summary>
/// RA2
/// </summary>
// (0x0076CBC0, slot 30)
// Remembers an address and port as unreachable (up to 16).

void UDPInterfaceClass::Add_Peer(unsigned long *address, unsigned short port)
{
    if (NumUnreachable >= 16) {
        return;
    }
    for (int i = 0; i < NumUnreachable; i++) {
        if (port == UnreachablePorts[i] && *address == UnreachableAddresses[i]) {
            return;
        }
    }
    Debug_Printf("Marking address %d.%d.%d.%d ; %d as unreachable\n",
        ((unsigned char *)address)[0], ((unsigned char *)address)[1],
        ((unsigned char *)address)[2], ((unsigned char *)address)[3], port);
    UnreachableAddresses[NumUnreachable] = *address;
    UnreachablePorts[NumUnreachable] = port;
    NumUnreachable++;
}


/// <summary>
/// RA2
/// </summary>
// (0x0076CC70)
// False when the address and port are marked unreachable.

bool UDPInterfaceClass::Is_Reachable(unsigned long *address, unsigned short port)
{
    for (int i = 0; i < NumUnreachable; i++) {
        if (port == UnreachablePorts[i] && *address == UnreachableAddresses[i]) {
            return false;
        }
    }
    return true;
}


/// <summary>
/// RA2
/// </summary>
// (0x0076CCC0, slot 28)
// Closes the spare sockets and forgets them.

extern HWND MainWindow;                     // 0x00B263D8

void UDPInterfaceClass::Close_All_Port_Sockets()
{
    for (int index = 0; index < NumSockets; index++) {
        Debug_Printf("Closing spare socket %d\n", Sockets[index]);
        WSAAsyncSelect(Sockets[index], MainWindow, 0, 0);
        closesocket(Sockets[index]);
    }
    NumSockets = 0;
    unknown_3f334 = -1;
}


/***********************************************************************************************
 * TMC::Message_Handler -- Message handler function for Winsock related messages               *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Windows message handler stuff                                                     *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    3/20/96 3:05PM ST : Created                                                              *
 *=============================================================================================*/
// (0x0076CD40, slot 16)
// The Winsock message handler (TibSun wspudp.cpp Message_Handler with RA2's spare sockets,
// alternate socket, unreachable list and packet-router address translation).


int UDPInterfaceClass::Message_Handler(int window, int message, int wparam, int lparam)
{
    sockaddr_in addr;
    int addr_len;
    union {
        unsigned long Address;
        unsigned char Bytes[4];
    } ip;
    bool alternate = false;
    SOCKET socket;
    int rc;

    if (message == 0x474) {
        socket = Socket;
        if ((SOCKET)wparam != socket) {
            for (int i = 0; i < NumSockets; i++) {
                if (Sockets[i] == (SOCKET)wparam) {
                    socket = Sockets[i];
                    break;
                }
            }
        }
    } else if (message == 0x475) {
        socket = Socket2;
        alternate = true;
    } else {
        return 1;
    }

    switch (WSAGETSELECTEVENT(lparam)) {
    case FD_READ:
    {
        rc = WSAGETSELECTERROR(lparam);
        if (rc != 0) {
            Debug_Printf("FD_READ - Got error code %d\n", rc);
            Clear_Socket_Error(socket);
            return 0;
        }
        addr_len = sizeof(addr);
        rc = recvfrom(socket, ReceiveBuffer, sizeof(ReceiveBuffer), 0, &addr, &addr_len);
        if (rc == SOCKET_ERROR) {
            Clear_Socket_Error(socket);
            return 0;
        }
        if (rc != 0) {
            ip.Address = addr.sin_addr;
            Debug_Printf("recvfrom %d.%d.%d.%d ; %d\n", ip.Bytes[0], ip.Bytes[1], ip.Bytes[2], ip.Bytes[3], ntohs(addr.sin_port));
            if (!alternate && !unknown_session_a3d298.ViaPacketRouter) {
                for (int i = 0; i < LocalAddresses.Count(); i++) {
                    if (!memcmp(LocalAddresses[i], &addr.sin_addr, 4)) {
                        return 0;
                    }
                }
            }
            int loss = Get_Packet_Loss_Percent();
            if (loss > 0 && NonCriticalRandomNumber(0, 99) <= loss) {
                return 0;
            }
            WinsockBufferType *packet = Get_New_In_Buffer();
            packet->BufferLen = rc - sizeof(packet->CRC);
            packet->CRC = *(unsigned long *)&ReceiveBuffer[0];
            packet->IsAlternate = alternate;
            packet->unknown_18 = 0;
            memcpy(packet->Buffer, ReceiveBuffer + sizeof(packet->CRC), packet->BufferLen);
            if (!Passes_CRC_Check(packet)) {
                *(unsigned long *)&packet->Address[4] = addr.sin_addr;
                unsigned long from = addr.sin_addr;
                Debug_Printf("Throwing away malformed packet from %d . %d . %d . %d ; %d\n", from & 0xFF, (from >> 8) & 0xFF, (from >> 16) & 0xFF, from >> 24, ntohs(addr.sin_port));
                if (!IsMalformedPacketReported) {
                    IsMalformedPacketReported = true;
                }
                if (packet->IsAllocated) {
                    delete packet;
                } else {
                    packet->InUse = false;
                    StaticOutBuffersInUse--;
                }
                return 0;
            }
            memset(packet->Address, 0, sizeof(packet->Address));
            if (addr.sin_addr == htonl(0x7F000001)) {
                packet->SrcPort = 0;
                *(IPXAddressClass *)packet->Address = unknown_forward_address_a3fbb8;
            } else {
                packet->SrcPort = ntohs(addr.sin_port);
                *(unsigned long *)&packet->Address[4] = addr.sin_addr;
            }
            InBuffers.Add(packet);
        }
        return 0;
    }

    case FD_WRITE:
    {
        if (unknown_3f334 != -1) {
            for (int i = 0; i < NumSockets; i++) {
                if (Sockets[i] == unknown_3f334) {
                    socket = unknown_3f334;
                    Debug_Printf("FD_WRITE - Using socket %d\n", socket);
                }
            }
        }
        rc = WSAGETSELECTERROR(lparam);
        if (rc != 0) {
            Debug_Printf("FD_WRITE - Got error code %d\n", rc);
            Clear_Socket_Error(socket);
            return 0;
        }
        WinsockBufferType *packet;
        if (alternate) {
            if (AltBuffers.Count() == 0) {
                Debug_Printf("FD_WRITE - No AltOutBuffers left - nothing to do\n");
                return 0;
            }
            packet = AltBuffers[0];
        } else {
            if (OutBuffers.Count() == 0) {
                Debug_Printf("FD_WRITE - No OutBuffers left - nothing to do\n");
                return 0;
            }
            packet = OutBuffers[0];
        }
        addr.sin_addr = *(unsigned long *)&packet->Address[4];
        addr.sin_port = htons(packet->SrcPort);
        addr.sin_family = AF_INET;
        if (alternate) {
            if (addr.sin_addr != htonl(0x7F000001)) {
                addr.sin_port = htons((unsigned short)AlternateSendPort);
                addr.sin_addr = htonl(0x7F000001);
            }
        } else {
            if (packet->SrcPort) {
                addr.sin_port = htons(packet->SrcPort);
            } else {
                addr.sin_port = htons(unknown_default_port_7f60d0);
            }
            if (!memcmp(&packet->Address[4], unknown_forward_address_a3fbb8.NodeAddress, 4)
                && (unknown_session_a3d298.Players.Count() == 0 || unknown_forward_address_a3fbb8 == unknown_session_a3d298.Players[0]->Address)) {
                addr.sin_addr = htonl(0x7F000001);
                addr.sin_port = htons((unsigned short)LoopbackPort);
            }
        }
        ip.Address = addr.sin_addr;
        unsigned short port = ntohs(addr.sin_port);
        bool unreachable = false;
        for (int i = 0; i < NumUnreachable; i++) {
            if (port == UnreachablePorts[i] && !memcmp(&ip, &UnreachableAddresses[i], 4)) {
                unreachable = true;
                break;
            }
        }
        if (!unreachable) {
            Debug_Printf("sendto %d.%d.%d.%d ; %d\n", ip.Bytes[0], ip.Bytes[1], ip.Bytes[2], ip.Bytes[3], ntohs(addr.sin_port));
            rc = sendto(socket, (char const *)&packet->CRC, packet->BufferLen + sizeof(packet->CRC), 0, &addr, sizeof(addr));
            if (rc == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK) {
                Debug_Printf("sendto returned error %d\n", WSAGetLastError());
                Clear_Socket_Error(socket);
                return 0;
            }
        } else {
            Debug_Printf("FD_WRITE - %d.%d.%d.%d ; %d is unreachable\n", ip.Bytes[0], ip.Bytes[1], ip.Bytes[2], ip.Bytes[3], ntohs(addr.sin_port));
        }
        if (alternate) {
            AltBuffers.Delete(0);
            if (packet->IsAllocated) {
                delete packet;
            } else {
                packet->InUse = false;
                StaticAltBuffersInUse--;
            }
            return 0;
        }
        OutBuffers.Delete(0);
        if (packet->IsAllocated) {
            delete packet;
        } else {
            packet->InUse = false;
            StaticOutBuffersInUse--;
        }
        return 0;
    }
    }
    return 0;
}


// UDPInterfaceClass::Get_Protocol (0x0076D3E0)
// Inline in shared/udp_interface.hpp; emitted with the vtable. No source of its own.


/// <summary>
/// RA2
/// </summary>
// (0x0076D3F0, slot 14)
// The notification message: 0x474 for the primary socket (or -1), 0x475
// for the alternate, else 0.

unsigned int UDPInterfaceClass::Protocol_Event_Message(int socketnum)
{
    if (socketnum == (int)Socket || socketnum == -1) {
        return 0x474;
    }
    return socketnum == (int)Socket2 ? 0x475 : 0;
}


// UDPInterfaceClass::Get_Num_Local_Addresses (0x0076D430)
// Inline in shared/udp_interface.hpp; emitted with the vtable. No source of its own.


// UDPInterfaceClass::Get_Local_Address (0x0076D440)
// Inline in shared/udp_interface.hpp; emitted with the vtable. No source of its own.


// UDPInterfaceClass::Get_Port (0x0076D450)
// Inline in shared/udp_interface.hpp; emitted with the vtable. No source of its own.


// void *UDPInterfaceClass::`scalar deleting destructor'(unsigned int flags) (0x0076D460)
// Compiler-generated with the vtable (the destructor inlined). No source of its own.


extern "C" void __cdecl release_76d5b0(void*);
class Cleanup_76d5b0 { unsigned char pad0[4]; unsigned int* items; int count; unsigned char pad12[1]; unsigned char owns; unsigned char pad14[2]; int capacity; public: void get(); };
void Cleanup_76d5b0::get() { unsigned int* pointer=items; capacity=0; if(pointer && owns) { release_76d5b0(pointer); items=0; } owns=0; count=0; }


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<unsigned char *>::Fetch (0x0076D600; VectorClass<unsigned char *> vtable 0x007afbcc slot 6).
// VectorClass<unsigned char *>::operator equal (0x0076D610; VectorClass<unsigned char *> vtable 0x007afbcc slot 1).
// VectorClass<unsigned char *>::Resize (0x0076D650; VectorClass<unsigned char *> vtable 0x007afbcc slot 2).
// VectorClass<unsigned char *>::Clear (0x0076D700; VectorClass<unsigned char *> vtable 0x007afbcc slot 3).
// VectorClass<unsigned char *>::ID_object (0x0076D730; VectorClass<unsigned char *> vtable 0x007afbcc slot 4).
// VectorClass<unsigned char *>::ID_pointer (0x0076D770; VectorClass<unsigned char *> vtable 0x007afbcc slot 5).


// DynamicVectorClass<unsigned char *>::Resize (0x0076D790)
// Template member instantiated from shared/vector_class.hpp.
// No source of its own: emitted by this unit (population-2641).


extern "C" unsigned char vtable_76d870[]; extern "C" void __cdecl release_buffer_76d870(void*); extern "C" void __cdecl release_object_76d870(void*);
class Leaf_76d870 { public: void* get(unsigned int flags); };
void* Leaf_76d870::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_76d870; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_76d870((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_76d870(this); return this; }


extern "C" unsigned char vtable_76d8c0[]; extern "C" void __cdecl release_buffer_76d8c0(void*); extern "C" void __cdecl release_object_76d8c0(void*);
class Leaf_76d8c0 { public: void* get(unsigned int flags); };
void* Leaf_76d8c0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_76d8c0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_76d8c0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_76d8c0(this); return this; }
