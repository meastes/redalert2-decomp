// wspipx.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/wspipx.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unknown_geometry.hpp"
#include "shared/ipx_interface.hpp"
#include "shared/ipx_address.hpp"
#include "shared/random2.hpp"


// Header-static dynamic initializers of the unit at 0x00769EA0:  the
// shared/unknown_geometry.hpp constants (0x00769EA0-0x0076A061); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x00769ED0, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_00769ea0.cpp; no source of its own.


// Dynamic initializer (0x00769EF0, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_00769ea0.cpp; no source of its own.


// Dynamic initializer (0x00769F10, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_00769ea0.cpp; no source of its own.


// Dynamic initializer (0x00769F30, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_00769ea0.cpp; no source of its own.


// Dynamic initializer (0x00769F50, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_00769ea0.cpp; no source of its own.


// Dynamic initializer (0x00769F70, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_00769ea0.cpp; no source of its own.


// Dynamic initializer (0x00769FB0, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_00769ea0.cpp; no source of its own.


// Dynamic initializer (0x00769FE0, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_00769ea0.cpp; no source of its own.


// Dynamic initializer (0x0076A010, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_00769ea0.cpp; no source of its own.


// Dynamic initializer (0x0076A040, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_00769ea0.cpp; no source of its own.


// Dynamic initializer (0x0076A050, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_00769ea0.cpp; no source of its own.


// IPXInterfaceClass::IPXInterfaceClass() (0x0076A070)
// The network and node addresses start unknown (all 0xFF).

IPXInterfaceClass::IPXInterfaceClass()
{
    memset(Network, 0xFF, sizeof(Network));
    memset(Node, 0xFF, sizeof(Node));
    memset(unknown_3f306, 0xFF, sizeof(unknown_3f306));
}


// int IPXInterfaceClass::Get_Network_Card_Address(int card_number, UnknownIPXSocketAddress *address) (0x0076A0B0)
// The network and node of an adapter, read through a temporary bound IPX
// socket; the network is also kept in Network. 0 on failure.

int IPXInterfaceClass::Get_Network_Card_Address(int card_number, UnknownIPXSocketAddress *address)
{
    SOCKET s = socket(6, 2, 1000);
    if (s == INVALID_SOCKET) {
        return 0;
    }

    UnknownIPXSocketAddress ipx;
    memset(&ipx, 0, sizeof(ipx));
    ipx.sa_family = 6;
    if (bind(s, &ipx, sizeof(ipx)) == SOCKET_ERROR) {
        closesocket(s);
        return 0;
    }

    int adapters = 1;
    int length = sizeof(adapters);
    bool win95 = unknown_flag_839c4c && unknown_839bc0 == 4 && unknown_839bc4 == 0;
    if (!win95) {
        if (getsockopt(s, 1000, 0x400D, (char *)&adapters, &length) == 0) {
            if (card_number >= adapters) {
                return 0;
            }
        } else {
            Debug_Printf("getsockopt with IPX_MAX_ADAPTER_NUM failed with error code %d\n", WSAGetLastError());
        }
    } else {
        Debug_Printf("Skipping IPX_MAX_ADAPTER_NUM call due to bug in Win95\n");
    }

    UnknownIPXAddressData data;
    memset(&data, 0, sizeof(data));
    data.adapternum = card_number;
    int data_length = sizeof(data);
    if (getsockopt(s, 1000, 0x4007, (char *)&data, &data_length) == SOCKET_ERROR) {
        Debug_Printf("Failed to get address of net card %d - error code %d.\n", card_number, WSAGetLastError());
        closesocket(s);
        return 0;
    }
    memcpy(address->sa_netnum, data.netnum, 4);
    memcpy(Network, data.netnum, 4);
    memcpy(address->sa_nodenum, data.nodenum, 6);
    closesocket(s);
    return 1;
}


// int IPXInterfaceClass::unknown_76a230(int card_number, char *buffer, int length) (0x0076A230)
// A network card's address as text ("net : node" in hex).

int IPXInterfaceClass::unknown_76a230(int card_number, char *buffer, int length)
{
    if (!WinsockInitialised) {
        return 0;
    }
    UnknownIPXSocketAddress address;
    char text[256];
    int result = Get_Network_Card_Address(card_number, &address);
    if (result) {
        if (result == 2) {
            sprintf(text, "?? ?? ?? ?? : %02x %02x %02x %02x %02x %02x",
                address.sa_nodenum[0], address.sa_nodenum[1], address.sa_nodenum[2],
                address.sa_nodenum[3], address.sa_nodenum[4], address.sa_nodenum[5]);
        } else {
            sprintf(text, "%02x %02x %02x %02x : %02x %02x %02x %02x %02x %02x",
                address.sa_netnum[0], address.sa_netnum[1], address.sa_netnum[2], address.sa_netnum[3],
                address.sa_nodenum[0], address.sa_nodenum[1], address.sa_nodenum[2],
                address.sa_nodenum[3], address.sa_nodenum[4], address.sa_nodenum[5]);
        }
        if ((int)strlen(text) >= length) {
            return 0;
        }
        strcpy(buffer, text);
    }
    return result;
}


// bool IPXInterfaceClass::Open_Socket(int socket_number) (0x0076A3A0, slot 15)
// Opens the IPX datagram socket on the socket number at the address of net
// card unknown_3f2f8 and sets its broadcast, packet type and filter options.

bool IPXInterfaceClass::Open_Socket(int socket_number)
{
    if (!WinsockInitialised) {
        if (!Init()) {
            return false;
        }
    } else if (Socket != INVALID_SOCKET) {
        return false;
    }

    SocketNumber = socket_number;
    Debug_Printf("IPX Socket number is %04x\n", socket_number);
    UnknownIPXSocketAddress address;
    address.sa_family = 6;
    memset(address.sa_netnum, 0, sizeof(address.sa_netnum));
    memset(address.sa_nodenum, 0xFF, sizeof(address.sa_nodenum));
    address.sa_socket = htons(socket_number);

    Debug_Printf("About to create an IPX socket\n");
    Socket = socket(6, 2, 1000);
    if (Socket == INVALID_SOCKET) {
        Debug_Printf("Failed to create IPX socket - error code %d.\n", WSAGetLastError());
        closesocket(Socket);
        return false;
    }
    Debug_Printf("IPX Socket created OK\n");

    if (!Get_Network_Card_Address(unknown_3f2f8, &address)) {
        Debug_Printf("IPX Error - failed to get the net card address\n");
        closesocket(Socket);
        return false;
    }
    Debug_Printf("IPX net card address is %02x %02x %02x %02x : %02x %02x %02x %02x %02x %02x\n",
        address.sa_netnum[0], address.sa_netnum[1], address.sa_netnum[2], address.sa_netnum[3],
        address.sa_nodenum[0], address.sa_nodenum[1], address.sa_nodenum[2],
        address.sa_nodenum[3], address.sa_nodenum[4], address.sa_nodenum[5]);
    memcpy(unknown_3f306, address.sa_nodenum, sizeof(unknown_3f306));

    Debug_Printf("About to bind IPX Socket\n");
    if (bind(Socket, &address, 16) == SOCKET_ERROR) {
        Debug_Printf("IPX socket bind failed with error code %d.\n", WSAGetLastError());
        closesocket(Socket);
        return false;
    }
    Debug_Printf("IPX Socket bound OK\n");

    int broadcast = 1;
    int packet_type = 4;
    Debug_Printf("About to set IPX socket option SO_BROADCAST\n");
    if (setsockopt(Socket, 0xFFFF, 0x20, (char *)&broadcast, sizeof(broadcast)) == SOCKET_ERROR) {
        Debug_Printf("Failed to set IPX socket option SO_BROADCAST - error code %d.\n", WSAGetLastError());
    } else {
        Debug_Printf("IPX socket option SO_BROADCAST set OK\n");
    }
    Debug_Printf("About to set IPX socket option IPX_PTYPE\n");
    if (setsockopt(Socket, 1000, 0x4000, (char *)&packet_type, sizeof(packet_type)) == SOCKET_ERROR) {
        Debug_Printf("Failed to set IPX protocol option IPX_PTYPE - error code %d.\n", WSAGetLastError());
    } else {
        Debug_Printf("IPX socket option IPX_PTYPE set OK\n");
    }
    Debug_Printf("About to set IPX socket option IPX_FILTERTYPE\n");
    if (setsockopt(Socket, 1000, 0x4001, (char *)&packet_type, sizeof(packet_type)) == SOCKET_ERROR) {
        Debug_Printf("Failed to set IPX protocol option IPX_FILTERTYPE - error code %d.\n", WSAGetLastError());
    } else {
        Debug_Printf("IPX socket option IPX_FILTERTYPE set OK\n");
    }
    WinsockInterfaceClass::Set_Socket_Options(INVALID_SOCKET);
    Debug_Printf("IPX Socket init complete\n");
    return true;
}


// int IPXInterfaceClass::Message_Handler(int window, int message, int wparam, int lparam) (0x0076A6B0, slot 16)
// The socket notification handler (message 0x473): FD_READ queues an
// incoming packet that is not our own and passes its CRC check; FD_WRITE
// sends every queued outgoing packet.

int IPXInterfaceClass::Message_Handler(int window, int message, int wparam, int lparam)
{
    UnknownIPXSocketAddress address;
    NetNumType network;
    NetNodeType node;
    WinsockBufferType *packet;

    if (message != 0x473) {
        return 1;
    }
    switch (WSAGETSELECTEVENT(lparam)) {
        case FD_READ: {
            int error = WSAGETSELECTERROR(lparam);
            if (error) {
                Clear_Socket_Error(Socket);
                return 0;
            }
            int length = sizeof(address);
            int received = recvfrom(Socket, ReceiveBuffer, sizeof(ReceiveBuffer), 0, &address, &length);
            if (received == SOCKET_ERROR) {
                if (WSAGetLastError() != WSAEWOULDBLOCK) {
                    Clear_Socket_Error(Socket);
                }
                return 0;
            }
            if (received) {
                memcpy(network, address.sa_netnum, sizeof(network));
                memcpy(node, address.sa_nodenum, sizeof(node));
                if (!memcmp(network, Network, sizeof(network)) && !memcmp(node, unknown_3f306, sizeof(node))) {
                    return 0;
                }
                int loss = WinsockInterfaceClass::unknown_76b9f0();
                if (loss > 0 && unknown_random_839218(0, 99) <= loss) {
                    return 0;
                }
                packet = Get_New_In_Buffer();
                packet->BufferLen = received - 4;
                packet->CRC = *(unsigned long *)ReceiveBuffer;
                memcpy(packet->Buffer, &ReceiveBuffer[4], received - 4);
                if (!Passes_CRC_Check(packet)) {
                    Debug_Printf("Throwing away malformed packet\n");
                    if (!unknown_malformed_seen_b2ae24) {
                        unknown_malformed_seen_b2ae24 = true;
                    }
                    if (packet->IsAllocated) {
                        delete packet;
                        return 0;
                    }
                    packet->InUse = false;
                    StaticOutBuffersInUse--;
                    return 0;
                }
                ((IPXAddressClass *)packet->Address)->Set_Address(network, node, 0);
                InBuffers.Add(packet);
            }
            return 0;
        }

        case FD_WRITE: {
            int error = WSAGETSELECTERROR(lparam);
            if (error) {
                Clear_Socket_Error(Socket);
                return 0;
            }
            while (OutBuffers.Count()) {
                packet = OutBuffers[0];
                address.sa_family = 6;
                address.sa_socket = htons(SocketNumber);
                if (packet->IsBroadcast) {
                    memcpy(address.sa_netnum, Network, sizeof(address.sa_netnum));
                    memcpy(address.sa_nodenum, Node, sizeof(address.sa_nodenum));
                } else {
                    ((IPXAddressClass *)packet->Address)->Get_Address(network, node);
                    memcpy(address.sa_netnum, network, sizeof(address.sa_netnum));
                    memcpy(address.sa_nodenum, node, sizeof(address.sa_nodenum));
                }
                if (sendto(Socket, (char *)&packet->CRC, packet->BufferLen + 4, 0, &address, sizeof(address)) == SOCKET_ERROR
                    && WSAGetLastError() != WSAEWOULDBLOCK) {
                    Clear_Socket_Error(Socket);
                    return 0;
                }
                OutBuffers.Delete(0);
                if (packet->IsAllocated) {
                    delete packet;
                } else {
                    packet->InUse = false;
                    StaticOutBuffersInUse--;
                }
            }
            return 0;
        }
    }
    return 0;
}


// WinsockInterfaceClass::Set_Broadcast_Address (0x0076AA00)
// Inline in the class header; emitted with IPXInterfaceClass's vtable. No source of its own.


// WinsockInterfaceClass::Clear_Broadcast_Addresses (0x0076AA10)
// Inline in the class header; emitted with IPXInterfaceClass's vtable. No source of its own.


// WinsockInterfaceClass::Get_Num_Local_Addresses (0x0076AA20)
// Inline in the class header; emitted with IPXInterfaceClass's vtable. No source of its own.


// WinsockInterfaceClass::Get_Local_Address (0x0076AA30)
// Inline in the class header; emitted with IPXInterfaceClass's vtable. No source of its own.


// WinsockInterfaceClass::Set_NetCard (0x0076AA40)
// Inline in the class header; emitted with IPXInterfaceClass's vtable. No source of its own.


// IPXInterfaceClass::Get_Protocol (0x0076AA50)
// Inline in the class header; emitted with IPXInterfaceClass's vtable. No source of its own.


// IPXInterfaceClass::Protocol_Event_Message (0x0076AA60)
// Inline in the class header; emitted with IPXInterfaceClass's vtable. No source of its own.


// void *IPXInterfaceClass::`scalar deleting destructor'(unsigned int flags) (0x0076AA70)
// Compiler-generated with the vtable (calls the base destructor). No source of its own.
