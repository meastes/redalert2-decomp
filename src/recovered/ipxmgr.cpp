// ipxmgr.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/ipxmgr.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#define RA2_IPX_MANAGER_CUR_DATA_BUF
#include "shared/ipx_manager.hpp"
#include "shared/owner_talk.hpp"
#include "shared/debug_printf.hpp"
#include "shared/house.hpp"
#include "shared/network_statistics.hpp"
#include "shared/modem_guest.hpp"
#include "shared/text_print.hpp"
#include "shared/string_table.hpp"
#include "shared/progress_screen.hpp"
#include "shared/queue.hpp"
#include "shared/system_timer.hpp"
#include "shared/cd_timer.hpp"
#include "shared/connection.hpp"
#include "shared/session_state.hpp"
#include "shared/unknown_globals.hpp"


// Header-static dynamic initializers of the unit at 0x00524E40:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x00524E40-0x00525026); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x00524E70, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_00524e40.cpp; no source of its own.


// Dynamic initializer (0x00524E90, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_00524e40.cpp; no source of its own.


// Dynamic initializer (0x00524EB0, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_00524e40.cpp; no source of its own.


// Dynamic initializer (0x00524ED0, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_00524e40.cpp; no source of its own.


// Dynamic initializer (0x00524EF0, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_00524e40.cpp; no source of its own.


// Dynamic initializer (0x00524F10, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_00524e40.cpp; no source of its own.


// Dynamic initializer (0x00524F50, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_00524e40.cpp; no source of its own.


// Dynamic initializer (0x00524F80, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_00524e40.cpp; no source of its own.


// Dynamic initializer (0x00524FB0, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_00524e40.cpp; no source of its own.


// Dynamic initializer (0x00524FE0, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_00524e40.cpp; no source of its own.


// Dynamic initializer (0x00524FF0, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_00524e40.cpp; no source of its own.


// Dynamic initializer (0x00525010, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_00524e40.cpp; no source of its own.


// IPXManagerClass::IPXManagerClass(int glb_maxlen, int pvt_maxlen, int glb_num_packets, int pvt_num_packets, unsigned short socket, unsigned short product_id) (0x00525030)
// Probes for a transport (creating and dropping an IPX interface), then
// keeps the channel sizes, the byte-swapped socket and default timing.
// IPXManagerClass::CurDataBuf (+0x8C, shared/ipx_manager.hpp) for this unit, whose Service keeps it.

IPXManagerClass::IPXManagerClass(int glb_maxlen, int pvt_maxlen, int glb_num_packets, int pvt_num_packets,
    unsigned short socket, unsigned short product_id)
{
    if (PacketTransport) {
        delete PacketTransport;
        PacketTransport = 0;
    }
    PacketTransport = new IPXInterfaceClass;
    if (PacketTransport->Init()) {
        IPXStatus = true;
    } else {
        IPXStatus = false;
    }
    if (PacketTransport) {
        delete PacketTransport;
    }
    PacketTransport = 0;

    Listening = false;
    Glb_MaxPacketLen = glb_maxlen;
    Glb_NumPackets = glb_num_packets;
    Pvt_MaxPacketLen = pvt_maxlen;
    Pvt_NumPackets = pvt_num_packets;
    unknown_18 = glb_maxlen;
    unknown_1c = pvt_num_packets;
    ProductID = product_id;
    Socket = ((socket & 0x00FF) << 8) | ((socket & 0xFF00) >> 8);
    ConnectionNum = 0;
    NumConnections = 0;
    CurConnection = 0;
    for (int i = 0; i < CONNECT_MAX; i++) {
        Connection[i] = 0;
    }
    GlobalChannel = 0;
    unknown_a8 = -1;
    MaxRetries = 0xFFFFFFFF;
    unknown_4c = 0;
    unknown_50 = 0;
    SendOverflows = 0;
    unknown_a4 = 0;
    RetryDelta = 2;
    Timeout = 60;
}


// IPXManagerClass::~IPXManagerClass() (0x00525130)
// Stops listening and deletes the channels and every connection.

IPXManagerClass::~IPXManagerClass()
{
    if (Listening) {
        IPXConnClass::Stop_Listening();
        Listening = false;
    }
    if (GlobalChannel) {
        delete GlobalChannel;
        GlobalChannel = 0;
    }
    if (unknown_4c) {
        delete unknown_4c;
        unknown_4c = 0;
    }
    if (unknown_50) {
        delete unknown_50;
        unknown_50 = 0;
    }
    for (int i = 0; i < NumConnections; i++) {
        delete Connection[i];
        Connection[i] = 0;
    }
    NumConnections = 0;
}


// int IPXManagerClass::Init() (0x005251C0)
// Resets the transport, recreates both global channels with the current
// timing, configures the IPX socket and starts listening.

int IPXManagerClass::Init()
{
    Debug_Printf("**** IPXManagerClass::Init ***\n");
    if (PacketTransport) {
        PacketTransport->Discard_In_Buffers();
        PacketTransport->Discard_Out_Buffers();
    }
    if (unknown_game_mode_a3d298 != 4) {
        if (!IPXStatus) {
            return 0;
        }
        if (Listening) {
            IPXConnClass::Stop_Listening();
            Listening = false;
        }
    } else {
        IPXStatus = true;
    }
    if (GlobalChannel) {
        delete GlobalChannel;
        GlobalChannel = 0;
    }
    if (unknown_4c) {
        delete unknown_4c;
        unknown_4c = 0;
    }
    if (unknown_50) {
        delete unknown_50;
        unknown_50 = 0;
    }
    for (int i = 0; i < NumConnections; i++) {
        delete Connection[i];
        Connection[i] = 0;
    }
    NumConnections = 0;

    GlobalChannel = new IPXGlobalConnClass(Glb_NumPackets, Glb_NumPackets * 2, Glb_MaxPacketLen, ProductID, 0x1236);
    if (!GlobalChannel) {
        return 0;
    }
    GlobalChannel->Init();
    GlobalChannel->RetryDelta = RetryDelta;
    GlobalChannel->MaxRetries = MaxRetries;
    GlobalChannel->Timeout = Timeout;

    unknown_4c = new IPXGlobalConnClass(unknown_1c, unknown_1c * 2, unknown_18, ProductID, 0xCDEF);
    if (!unknown_4c) {
        return 0;
    }
    unknown_4c->Init();
    unknown_4c->RetryDelta = RetryDelta;
    unknown_4c->MaxRetries = MaxRetries;
    unknown_4c->Timeout = Timeout;

    IPXConnClass::Configure(Socket, ConnectionNum, unknown_7c, unknown_94, unknown_80, unknown_98, unknown_74);
    if (!PacketTransport) {
        return 0;
    }
    if (unknown_game_mode_a3d298 != 4) {
        if (!IPXConnClass::Start_Listening()) {
            return 0;
        }
    }
    Listening = true;
    return 1;
}


// int IPXManagerClass::Is_IPX() (0x00525390)

int IPXManagerClass::Is_IPX()
{
    return IPXStatus;
}


// void IPXManagerClass::Set_Timing(unsigned long retrydelta, unsigned long maxretries, unsigned long timeout, bool global) (0x005253A0, slot 13)
// Keeps the timing and applies it to every connection and the multicast
// connection (and the global channel when asked).

void IPXManagerClass::Set_Timing(unsigned long retrydelta, unsigned long maxretries, unsigned long timeout, bool global)
{
    RetryDelta = retrydelta;
    MaxRetries = maxretries;
    Timeout = timeout;
    Debug_Printf("IPX Manager: RetryDelta = %d\n", retrydelta);
    Debug_Printf("MaxAhead is %d\n", unknown_max_ahead_a3d560);
    if (global) {
        unknown_slot14(RetryDelta, MaxRetries, Timeout);
    }
    for (int i = 0; i < NumConnections; i++) {
        Connection[i]->RetryDelta = RetryDelta;
        Connection[i]->MaxRetries = MaxRetries;
        Connection[i]->Timeout = Timeout;
    }
    if (unknown_50) {
        unknown_50->RetryDelta = RetryDelta;
        unknown_50->MaxRetries = MaxRetries;
        unknown_50->Timeout = Timeout;
    }
}


// void IPXManagerClass::unknown_slot14(unsigned long retrydelta, unsigned long maxretries, unsigned long timeout) (0x00525450, slot 14)
// The global channel's timing.

void IPXManagerClass::unknown_slot14(unsigned long retrydelta, unsigned long maxretries, unsigned long timeout)
{
    if (GlobalChannel) {
        GlobalChannel->RetryDelta = retrydelta;
        GlobalChannel->MaxRetries = maxretries;
        GlobalChannel->Timeout = timeout;
    }
}


// void IPXManagerClass::unknown_slot16() (0x00525480, slot 16)
// Saves the global channel's timing.

void IPXManagerClass::unknown_slot16()
{
    if (GlobalChannel) {
        unknown_64 = GlobalChannel->RetryDelta;
        unknown_68 = GlobalChannel->MaxRetries;
        unknown_6c = GlobalChannel->Timeout;
    }
}


// void IPXManagerClass::unknown_slot17() (0x005254A0, slot 17)
// Restores the global channel's saved timing.

void IPXManagerClass::unknown_slot17()
{
    if (GlobalChannel) {
        GlobalChannel->RetryDelta = unknown_64;
        GlobalChannel->MaxRetries = unknown_68;
        GlobalChannel->Timeout = unknown_6c;
    }
}


// void IPXManagerClass::unknown_slot18(unsigned long retrydelta, unsigned long maxretries, unsigned long timeout) (0x005254C0, slot 18)
// The second global channel's and the multicast connection's timing.

void IPXManagerClass::unknown_slot18(unsigned long retrydelta, unsigned long maxretries, unsigned long timeout)
{
    if (unknown_4c) {
        unknown_4c->RetryDelta = retrydelta;
        unknown_4c->MaxRetries = maxretries;
        unknown_4c->Timeout = timeout;
    }
    if (unknown_50) {
        unknown_50->RetryDelta = retrydelta;
        unknown_50->MaxRetries = maxretries;
        unknown_50->Timeout = timeout;
    }
}


// int IPXManagerClass::Create_Connection(int id, wchar_t const *name, IPXAddressClass *address) (0x00525500)
// Adds a private connection with the current timing.

int IPXManagerClass::Create_Connection(int id, wchar_t const *name, IPXAddressClass *address)
{
    if (!IPXStatus) {
        return 0;
    }
    if (NumConnections == CONNECT_MAX) {
        return 0;
    }
    Connection[NumConnections] = new IPXConnClass(Pvt_NumPackets, Pvt_NumPackets, Pvt_MaxPacketLen, ProductID, address, id, name);
    if (Connection[NumConnections] == 0) {
        return 0;
    }
    Connection[NumConnections]->Init();
    Connection[NumConnections]->RetryDelta = RetryDelta;
    Connection[NumConnections]->MaxRetries = MaxRetries;
    Connection[NumConnections]->Timeout = Timeout;
    NumConnections++;
    return 1;
}


// int IPXManagerClass::unknown_5255b0() (0x005255B0)
// Creates the multicast connection to the first node's address.

int IPXManagerClass::unknown_5255b0()
{
    if (!IPXStatus) {
        return 0;
    }
    unknown_50 = new IPXConnClass(Pvt_NumPackets, Pvt_NumPackets, Pvt_MaxPacketLen, ProductID,
        &unknown_players_a3fa7c[0]->Address, -1, Narrow_To_Wide_006fa180("Multicast"));
    if (!unknown_50) {
        return 0;
    }
    unknown_50->Init();
    unknown_50->RetryDelta = RetryDelta;
    unknown_50->MaxRetries = MaxRetries;
    unknown_50->Timeout = Timeout;
    return 1;
}


// int IPXManagerClass::Delete_Connection(int id) (0x00525640)
// Deletes a private connection and closes the gap.

int IPXManagerClass::Delete_Connection(int id)
{
    if (!IPXStatus || NumConnections == 0) {
        return 0;
    }
    for (int i = 0; i < NumConnections; i++) {
        if (Connection[i]->ID == id) {
            delete Connection[i];
            for (int j = i; j < NumConnections - 1; j++) {
                Connection[j] = Connection[j + 1];
            }
            NumConnections--;
            if (CurConnection >= NumConnections) {
                CurConnection = 0;
            }
            return 1;
        }
    }
    return 0;
}


// int IPXManagerClass::Num_Connections() (0x005256D0, slot 4)

int IPXManagerClass::Num_Connections()
{
    return NumConnections;
}


// int IPXManagerClass::Connection_ID(int index) (0x005256E0, slot 5)

int IPXManagerClass::Connection_ID(int index)
{
    if (index >= 0 && index < NumConnections) {
        return Connection[index]->ID;
    }
    return -1;
}


// wchar_t *IPXManagerClass::Connection_Name(int id) (0x00525700)

wchar_t *IPXManagerClass::Connection_Name(int id)
{
    for (int i = 0; i < NumConnections; i++) {
        if (Connection[i]->ID == id) {
            return Connection[i]->Name;
        }
    }
    return 0;
}


// IPXAddressClass *IPXManagerClass::Connection_Address(int id) (0x00525740)

IPXAddressClass *IPXManagerClass::Connection_Address(int id)
{
    for (int i = 0; i < NumConnections; i++) {
        if (Connection[i]->ID == id) {
            return &Connection[i]->Address;
        }
    }
    return 0;
}


// int IPXManagerClass::Connection_Index(int id) (0x00525780, slot 6)

int IPXManagerClass::Connection_Index(int id)
{
    for (int i = 0; i < NumConnections; i++) {
        if (Connection[i]->ID == id) {
            return i;
        }
    }
    return -1;
}


// int IPXManagerClass::unknown_slot19(ConnectionClass *connection) (0x005257B0, slot 19)
// The index of a private connection, or -4 (multicast), -3 (second global
// channel), -2 (global channel), -1 (unknown).

int IPXManagerClass::unknown_slot19(ConnectionClass *connection)
{
    for (int i = 0; i < NumConnections; i++) {
        if (Connection[i] == connection) {
            return i;
        }
    }
    if (connection == unknown_50) {
        return -4;
    }
    if (connection == unknown_4c) {
        return -3;
    }
    if (connection == GlobalChannel) {
        return -2;
    }
    return -1;
}


// void IPXManagerClass::unknown_525800(int index, int id, wchar_t const *name) (0x00525800)
// Gives the index'th connection a new ID and name.

void IPXManagerClass::unknown_525800(int index, int id, wchar_t const *name)
{
    if (index < NumConnections) {
        Connection[index]->ID = id;
        wcscpy(Connection[index]->Name, name);
    }
}


// int IPXManagerClass::Send_Global_Message(void *buf, int buflen, int ack_req, IPXAddressClass *address, unsigned short id, unsigned long packet_id) (0x00525830)
// Sends on the global channel (never acknowledged when broadcast or
// unaddressed); counts and reports overflows.

int IPXManagerClass::Send_Global_Message(void *buf, int buflen, int ack_req, IPXAddressClass *address, unsigned short id, unsigned long packet_id)
{
    if (!IPXStatus || !Listening) {
        return 0;
    }
    if (ack_req && (!address || address->Is_Broadcast())) {
        ack_req = 0;
    }
    int rc = GlobalChannel->Send_Packet(buf, buflen, address, ack_req, id, packet_id);
    if (!rc) {
        SendOverflows++;
        Debug_Printf("Send overflow %d\n", SendOverflows);
    }
    return rc;
}


// int IPXManagerClass::unknown_5258b0(void *buf, int buflen, int ack_req, int node_id) (0x005258B0)
// Sends a global message to another session node chosen by ID (never the
// first node, this player).

int IPXManagerClass::unknown_5258b0(void *buf, int buflen, int ack_req, int node_id)
{
    if (unknown_players_a3fa7c.Count() && unknown_players_a3fa7c[0]->ID == node_id) {
        return 0;
    }
    for (int i = 1; i < unknown_players_a3fa7c.Count(); i++) {
        if (unknown_players_a3fa7c[i]->ID == node_id) {
            IPXAddressClass address = unknown_players_a3fa7c[i]->Address;
            return Send_Global_Message(buf, buflen, ack_req, &address, unknown_node_ids_a402ba[i], 0);
        }
    }
    return 0;
}


// int IPXManagerClass::unknown_5259a0(void *buf, int buflen, int ack_req, wchar_t const *name) (0x005259A0)
// Sends a global message to another session node chosen by name (never the
// first node, this player).

int IPXManagerClass::unknown_5259a0(void *buf, int buflen, int ack_req, wchar_t const *name)
{
    if (unknown_players_a3fa7c.Count() && !_wcsicmp(unknown_players_a3fa7c[0]->Name, name)) {
        return 0;
    }
    for (int i = 1; i < unknown_players_a3fa7c.Count(); i++) {
        if (!_wcsicmp(unknown_players_a3fa7c[i]->Name, name)) {
            IPXAddressClass address = unknown_players_a3fa7c[i]->Address;
            return Send_Global_Message(buf, buflen, ack_req, &address, unknown_node_ids_a402ba[i], 0);
        }
    }
    return 0;
}


// int IPXManagerClass::Get_Global_Message(void *buf, int *buflen, IPXAddressClass *address, unsigned short *product_id) (0x00525AB0)

int IPXManagerClass::Get_Global_Message(void *buf, int *buflen, IPXAddressClass *address, unsigned short *product_id)
{
    if (!IPXStatus || !Listening) {
        return 0;
    }
    return GlobalChannel->Get_Packet(buf, buflen, address, product_id);
}


// int IPXManagerClass::unknown_525af0(int index, void *buf, int *buflen, IPXAddressClass *address, unsigned short *product_id, unsigned long *packet_id) (0x00525AF0)
// Reads a global packet without marking it (IPXGlobalConnClass slot 14).

int IPXManagerClass::unknown_525af0(int index, void *buf, int *buflen, IPXAddressClass *address, unsigned short *product_id, unsigned long *packet_id)
{
    if (!IPXStatus || !Listening) {
        return 0;
    }
    return GlobalChannel->Peek_Packet(index, buf, buflen, address, product_id, packet_id);
}


// void IPXManagerClass::unknown_525b30(int index) (0x00525B30)
// Marks a global packet read (IPXGlobalConnClass slot 15).

void IPXManagerClass::unknown_525b30(int index)
{
    if (IPXStatus && Listening) {
        GlobalChannel->Mark_Packet_Read(index);
    }
}


// int IPXManagerClass::unknown_525b50(void *buf, int buflen, bool ack_req) (0x00525B50)
// Sends on the second global channel to its own address; counts overflows.

int IPXManagerClass::unknown_525b50(void *buf, int buflen, bool ack_req)
{
    if (!IPXStatus || !Listening || !unknown_4c) {
        return 0;
    }
    int rc = unknown_4c->Send_Packet(buf, buflen, &unknown_4c->Address, ack_req, 0, 0);
    if (!rc) {
        SendOverflows++;
        Debug_Printf("Send overflow %d\n", SendOverflows);
    }
    return rc;
}


// int IPXManagerClass::unknown_525bc0(void *buf, int *buflen, IPXAddressClass *address, unsigned short *product_id) (0x00525BC0)
// Reads from the second global channel.

int IPXManagerClass::unknown_525bc0(void *buf, int *buflen, IPXAddressClass *address, unsigned short *product_id)
{
    if (!IPXStatus || !Listening) {
        return 0;
    }
    return unknown_4c->Get_Packet(buf, buflen, address, product_id);
}


// int IPXManagerClass::Send_Private_Message(void *buf, int buflen, int ack_req, int conn_id) (0x00525C00, slot 2)
// Sends to one connection, or to all (through the multicast connection
// when forwarding); refuses, counting an overflow, when a queue is full.

int IPXManagerClass::Send_Private_Message(void *buf, int buflen, int ack_req, int conn_id)
{
    int i;
    if (!IPXStatus || !Listening || NumConnections == 0) {
        return 0;
    }
    if (conn_id == -1) {
        if (unknown_game_mode_a3d298 == 4 && unknown_flag_a3fba8) {
            if (unknown_50->Queue->SendCount >= unknown_50->Queue->MaxSend) {
                SendOverflows++;
                return 0;
            }
            return unknown_50->Send_Packet(buf, buflen, ack_req, 0xFF);
        }
        for (i = 0; i < NumConnections; i++) {
            if (Connection[i]->Queue->SendCount == Connection[i]->Queue->MaxSend) {
                SendOverflows++;
                return 0;
            }
        }
        for (i = 0; i < NumConnections; i++) {
            Connection[i]->Send_Packet(buf, buflen, ack_req, 0);
        }
        return 1;
    }
    int index = Connection_Index(conn_id);
    if (index == -1 || Connection[index]->Queue->SendCount == Connection[index]->Queue->MaxSend) {
        SendOverflows++;
        return 0;
    }
    Connection[index]->Send_Packet(buf, buflen, ack_req, 0);
    return 1;
}


// int IPXManagerClass::Get_Private_Message(void *buf, int *buflen, int *conn_id) (0x00525D30, slot 3)
// Reads the next packet, polling the connections round-robin.

int IPXManagerClass::Get_Private_Message(void *buf, int *buflen, int *conn_id)
{
    if (!IPXStatus || !Listening || NumConnections == 0) {
        return 0;
    }
    if (CurConnection >= NumConnections) {
        CurConnection = 0;
    }
    for (int i = 0; i < NumConnections; i++) {
        int rc = Connection[CurConnection]->Get_Packet(buf, buflen);
        int id = Connection[CurConnection]->ID;
        CurConnection++;
        if (CurConnection >= NumConnections) {
            CurConnection = 0;
        }
        if (rc) {
            *conn_id = id;
            return 1;
        }
    }
    return 0;
}


// bool IPXManagerClass::unknown_525dc0(CommHeaderType *header, char *buf, int buflen, IPXAddressClass *address, unsigned short port) (0x00525DC0)
// In the forwarding session with a router process: a packet that carries the
// forwarder port goes to the local router with the sender's node and port in
// its header; a packet with a forwarding target that is not this node goes on
// to that target. Returns whether the packet was forwarded.

// The local router's address (the network number, then the IP 127.0.0.1;
// passed as a 12-byte address).
struct UnknownLoopbackAddress {
    NetNumType NetworkNumber;
    unsigned long NodeAddress;
};

bool IPXManagerClass::unknown_525dc0(CommHeaderType *header, char *buf, int buflen, IPXAddressClass *address, unsigned short port)
{
    static UnknownLoopbackAddress loopback = { {0}, htonl(0x7F000001) };

    if (unknown_game_mode_a3d298 == 4 && unknown_flag_a3fba8 && unknown_OwnerTalkClass_00ab6d18.RouterProcess) {
        if (header->unknown_0c == unknown_port_7f60dc) {
            header->unknown_0c = port;
            memcpy(&header->unknown_08, address->NodeAddress, 4);
            Debug_Printf("Forwarding packet from %s ; %d to forwarder\n", address->As_String(), port);
            PacketTransport->WriteTo(buf, buflen, &loopback, sizeof(IPXAddressClass), false, 1, unknown_port_7f60dc);
            return true;
        }
        if (header->unknown_0c != 0) {
            NetNodeType node;
            unknown_players_a3fa7c[0]->Address.Get_Node(node);
            if (memcmp(node, &header->unknown_08, 4) == 0 && unknown_node_ids_a402ba[0] == header->unknown_0c) {
                Debug_Printf("Packet is supposed to be forwarded to us - just adding it to our queue\n");
            } else if (header->unknown_08 != 0) {
                unsigned short target_port = header->unknown_0c;
                IPXAddressClass target;
                NetNodeType ip;
                memcpy(ip, &header->unknown_08, 4);
                target.Set_Node(ip);
                target.Set_Port(target_port);
                header->unknown_0c = 0;
                memset(&header->unknown_08, 0, sizeof(header->unknown_08));
                Debug_Printf("Forwarding packet from router to %s\n", target.As_String());
                PacketTransport->WriteTo(buf, buflen, &target, sizeof(target), false, 1, target_port);
                return true;
            }
        }
    }
    return false;
}


// Not exact (RA2_NON_MATCHING): FUN_00525f60 at 0x00525F60, best score 97.99, 2026-11-13; differs: 770 instructions in the original, 771 in the draft (shape 99.94)
#ifdef RA2_NON_MATCHING
#include "shared/event.hpp"
#include "shared/crt_string.hpp"
extern bool unknown_flag_a402d8;
extern bool unknown_session_a3f616;
int IPXManagerClass::Service()
{
    int result=1;
    int i;
    CommHeaderType *packet;
    int packetlen;
    IPXAddressClass address;
    unsigned char receive_buffer[1024];
    char address_buffer[128];
    unsigned short port;
    if (PacketTransport) {
    do {
        int receive_length=sizeof(receive_buffer);
        int address_length=sizeof(address_buffer);
        port=0;
        packetlen=PacketTransport->Read(receive_buffer,&receive_length,address_buffer,&address_length,&port);
        if (packetlen) {
        CurDataBuf=(char *)receive_buffer;
        address=*(IPXAddressClass *)address_buffer;
        packet=(CommHeaderType *)CurDataBuf;
        if (packet->MagicNumber==GlobalChannel->MagicNum) {
            Debug_Printf("Adding packet from %s ; %d to global queue\n",address.As_String(),port);
            if (!unknown_525dc0(packet,(char *)receive_buffer,packetlen,(IPXAddressClass *)address_buffer,port)) {
                if (!GlobalChannel->Receive_Packet(packet,packetlen,&address,port)) {
                    ++unknown_a4;
                    Debug_Printf("GlobalChannel recive buffer overflow %d\n",unknown_a4);
                    break;
                }
                if (unknown_game_mode_a3d298==4 && unknown_session_a3d298.unknown_3038) {
                    for (i=0;i<unknown_players_a3fa7c.Count();++i) {
                        if (unknown_players_a3fa7c[i]->Address.Is_Same_Node(address) && unknown_node_ids_a402ba[i]==port) goto next_packet;
                    }
                    if (unknown_players_a3fa7c.Count()!=0) {
                        Debug_Printf("NAT Alert - port number fiddling detected!\n");
                        if ((unsigned)packetlen>=16 && packet->Code!=PACKET_ACK && (unsigned)packetlen>=63) {
                            int *message=(int *)(CurDataBuf+16);
                            if (*message==0x27 || *message==0x29 || *message==0x0d) {
                                char name[22];
                                memset(name,0,sizeof(name));
                                strncpy(name,(char *)(message+1),20);
                                for (i=0;i<unknown_players_a3fa7c.Count();++i) {
                                    wchar_t *wide_name=unknown_players_a3fa7c[i]->Name;
                                    if (!_stricmp(Wide_To_Narrow_006fa120(wide_name),name)) {
                                        if (!unknown_players_a3fa7c[i]->Address.Is_Same_Node(address) || *(unsigned short *)unknown_players_a3fa7c[i]->Address.NetworkNumber!=port) {
                                            Debug_Printf("Player %s is having his port translated to %d\n",name,port);
                                            unsigned short old_port=unknown_node_ids_a402ba[i];
                                            unknown_node_ids_a402ba[i]=port;
                                            unknown_OwnerTalkClass_00ab6d18.Unknown_0063FB60(unknown_players_a3fa7c[i]->ID,port);
                                            GlobalChannel->Discard_Packets_To(&address,old_port);
                                            GlobalChannel->Discard_Packets_To(&address,*(unsigned short *)unknown_players_a3fa7c[i]->Address.NetworkNumber);
                                            unknown_players_a3fa7c[i]->Address=address;
                                            unknown_players_a3fa7c[i]->Address.Set_Port(port);
                                            unknown_OwnerTalkClass_00ab6d18.Unknown_0063F9A0(unknown_players_a3fa7c[i]->ID,&unknown_players_a3fa7c[i]->Address);
                                            unknown_player_slots_a3fa98[unknown_session_a3d298.Unknown_00672F90(i)]->Address=address;
                                            unknown_player_slots_a3fa98[unknown_session_a3d298.Unknown_00672F90(i)]->Address.Set_Port(port);
                                            for (int connection=0;connection<NumConnections;++connection) {
                                                wchar_t *connection_name=Connection[connection]->Name;
                                                if (!_stricmp(Wide_To_Narrow_006fa120(connection_name),name)) {
                                                    Connection[connection]->Address=address;
                                                    Connection[connection]->Address.Set_Port(port);
                                                    Debug_Printf("Set address for connection %d to %s\n",connection,Connection[connection]->Address.As_String());
                                                    break;
                                                }
                                            }
                                            if (!unknown_session_a3f616 && NumConnections!=0 && Houses.Count()>unknown_players_a3fa7c[i]->ID && unknown_session_a3d298.Am_I_Master(Houses[unknown_players_a3fa7c[i]->ID])) {
                                                Debug_Printf("Frame %d, Router apparently changed address from %s to %s\n",Frame,unknown_50->Address.As_String(),address.As_String());
                                                unknown_50->Address=address;
                                                unknown_50->Address.Set_Port(port);
                                                unknown_address_a3db2c=address;
                                                unknown_address_a3db2c.Set_Port(port);
                                                unknown_forward_address_a3fbac=address;
                                                unknown_forward_address_a3fbac.Set_Port(port);
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
            if (unknown_flag_a3fba8 && !unknown_4c->Receive_Packet(packet,packetlen,&address,0)) {
                ++unknown_a4;
                Debug_Printf("RouterChannel recive buffer overflow %d\n",unknown_a4);
                break;
            }
        } else if (packet->MagicNumber==ProductID && !unknown_525dc0(packet,(char *)receive_buffer,packetlen,(IPXAddressClass *)address_buffer,port)) {
            if (unknown_flag_a3fba8 && packet->unknown_03==0xff) {
                if (packet->Code==PACKET_ACK) unknown_50->Receive_Packet(packet,packetlen);
                else if (NumConnections>0) {
                    switch(packet->Code) {
                    case PACKET_DATA_ACK: Debug_Printf("Adding DATA_ACK packet %d from %s ; %d to multicast queue\n",packet->PacketID,address.As_String(),port); break;
                    case PACKET_DATA_NOACK: Debug_Printf("Adding DATA_NOACK packet %d from %s ; %d to multicast queue\n",packet->PacketID,address.As_String(),port); break;
                    case PACKET_ACK: Debug_Printf("Adding ACK packet %d from %s ; %d to multicast queue\n",packet->PacketID,address.As_String(),port); break;
                    default: Debug_Printf("Adding ??? packet %d from %s ; %d to multicast queue\n",packet->PacketID,address.As_String(),port); break;
                    }
                    if (!Connection[0]->Receive_Packet(packet,packetlen)) {
                        packetlen=0;
                        ++unknown_a4;
                        Debug_Printf("Receive buffer overflow %d\n",unknown_a4);
                    }
                }

            } else {
                bool found=false;
                for (i=0;i<NumConnections;++i) {
                    if ((unknown_flag_a3fba8 && unknown_OwnerTalkClass_00ab6d18.Unknown_0063F4C0(Connection[i]->ID)==packet->unknown_03) || (!unknown_flag_a3fba8 && Connection[i]->Address==address)) {
                        found=true;
                        break;
                    }
                }
                if (found) {
                    if (!Connection[i]->Receive_Packet(packet,packetlen)) {
                        packetlen=0;
                        ++unknown_a4;
                        Debug_Printf("Reecive buffer overflow %d\n",unknown_a4);
                    }
                } else {
                if (unknown_game_mode_a3d298==4 && !unknown_a40774 && Frame>8 && !unknown_flag_a3fba8 && (unsigned)packetlen>15 && packet->Code==PACKET_DATA_NOACK) {
                    EventClass *event=(EventClass *)((char *)packet+sizeof(CommHeaderType));
                    if (event->Type==0x19) {
                        int id=event->House;
                        for (int player=1;player<unknown_players_a3fa7c.Count();++player) {
                            if (unknown_players_a3fa7c[player]->ID==id) {
                                unknown_players_a3fa7c[player]->Address=address;
                                if (Connection_Index(id)!=-1) Connection[Connection_Index(id)]->Address=address;
                                break;
                            }
                        }
                    }
                }
                                    goto next_packet;
                }

            }
                if (unknown_game_mode_a3d298==4 && unknown_flag_a3fba8 && address!=unknown_50->Address) {
                    for (int player=1;player<unknown_players_a3fa7c.Count();++player) {
                        if (unknown_players_a3fa7c[player]->Address.Is_Same_Node(unknown_50->Address) && unknown_session_a3d298.Am_I_Master(Houses[unknown_players_a3fa7c[player]->ID])) {
                            Debug_Printf("Frame %d, Router apparently changed address from %s to %s\n",Frame,unknown_50->Address.As_String(),address.As_String());
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
next_packet:
        ;
    } while (packetlen);
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
            if (!unknown_flag_a402d8) unknown_OwnerTalkClass_00ab6d18.Unknown_0063F840();
        }
        if (unknown_flag_a3fba8 && unknown_50 && !unknown_50->Service()) {
            Debug_Printf("Multicast channel has gone bad\n");
            result=0;
        }
    }
    if (PacketTransport && NumConnections>0) {
        i=0;
        do {
            if (!Connection[i]->Service()) {
                result=0;
                unknown_a8=Connection[i]->ID;
                Debug_Printf("Error - Connection %d has gone bad\n",unknown_a8);
            }
            ++i;
        } while (i<NumConnections);
    }
    if (result) unknown_a8=-1;
    return result;
}
#endif

// void IPXManagerClass::Set_Connection_Address(int id, IPXAddressClass *address) (0x005269F0)

void IPXManagerClass::Set_Connection_Address(int id, IPXAddressClass *address)
{
    int index = Connection_Index(id);
    if (index != -1 && index < NumConnections) {
        Connection[index]->Address = *address;
    }
}


// int IPXManagerClass::unknown_526a30() (0x00526A30)

int IPXManagerClass::unknown_526a30()
{
    return unknown_a8;
}


// int IPXManagerClass::Global_Num_Send() (0x00526A40, slot 7)

int IPXManagerClass::Global_Num_Send()
{
    if (!IPXStatus || !Listening) {
        return 0;
    }
    return GlobalChannel->Queue->SendCount;
}


// int IPXManagerClass::Global_Num_Receive() (0x00526A60, slot 8)

int IPXManagerClass::Global_Num_Receive()
{
    if (!IPXStatus || !Listening) {
        return 0;
    }
    return GlobalChannel->Queue->ReceiveCount;
}


// int IPXManagerClass::unknown_slot20() (0x00526A80, slot 20)
// The second global channel's queued sends.

int IPXManagerClass::unknown_slot20()
{
    if (!IPXStatus || !Listening || !unknown_4c) {
        return 0;
    }
    return unknown_4c->Queue->SendCount;
}


// int IPXManagerClass::unknown_slot21() (0x00526AA0, slot 21)
// The second global channel's queued receives.

int IPXManagerClass::unknown_slot21()
{
    if (!IPXStatus || !Listening || !unknown_4c) {
        return 0;
    }
    return unknown_4c->Queue->ReceiveCount;
}


// int IPXManagerClass::Private_Num_Send(int id) (0x00526AC0, slot 9)
// A connection's queued sends, or the most of any connection (-1).

int IPXManagerClass::Private_Num_Send(int id)
{
    if (!IPXStatus || !Listening || NumConnections == 0) {
        return 0;
    }
    if (id != -1) {
        int index = Connection_Index(id);
        if (index == -1) {
            return 0;
        }
        return Connection[index]->Queue->SendCount;
    }
    int maxnum = 0;
    for (int i = 0; i < NumConnections; i++) {
        if (Connection[i]->Queue->SendCount > maxnum) {
            maxnum = Connection[i]->Queue->SendCount;
        }
    }
    return maxnum;
}


// int IPXManagerClass::Private_Num_Receive(int id) (0x00526B30, slot 10)
// A connection's queued receives, or the most of any connection (-1).

int IPXManagerClass::Private_Num_Receive(int id)
{
    if (!IPXStatus || !Listening || NumConnections == 0) {
        return 0;
    }
    if (id != -1) {
        int index = Connection_Index(id);
        if (index == -1) {
            return 0;
        }
        return Connection[index]->Queue->ReceiveCount;
    }
    int maxnum = 0;
    for (int i = 0; i < NumConnections; i++) {
        if (Connection[i]->Queue->ReceiveCount > maxnum) {
            maxnum = Connection[i]->Queue->ReceiveCount;
        }
    }
    return maxnum;
}


// void IPXManagerClass::Set_Socket(unsigned short socket) (0x00526BA0)

void IPXManagerClass::Set_Socket(unsigned short socket)
{
    Socket = ((socket & 0x00FF) << 8) | ((socket & 0xFF00) >> 8);
}


// unsigned long IPXManagerClass::Response_Time() (0x00526BC0, slot 12)
// The largest mean round-trip delay of the connections and the multicast
// connection.

unsigned long IPXManagerClass::Response_Time()
{
    unsigned long resp = 0;
    unsigned long time;
    for (int i = 0; i < NumConnections; i++) {
        time = Connection[i]->Queue->Avg_Delay();
        if (time > resp) {
            resp = time;
        }
    }
    if (unknown_50) {
        time = unknown_50->Queue->Avg_Delay();
        if (time > resp) {
            resp = time;
        }
    }
    return resp;
}


// unsigned long IPXManagerClass::unknown_slot22(int index) (0x00526C10, slot 22)

unsigned long IPXManagerClass::unknown_slot22(int index)
{
    return Connection[index]->Queue->Avg_Delay();
}


// unsigned long IPXManagerClass::unknown_526c30() (0x00526C30)
// The global channel's mean delay (0 without one).

unsigned long IPXManagerClass::unknown_526c30()
{
    if (GlobalChannel) {
        return GlobalChannel->Queue->Avg_Delay();
    }
    return 0;
}


// unsigned long IPXManagerClass::unknown_526c50() (0x00526C50)
// The second global channel's mean delay (0 without one).

unsigned long IPXManagerClass::unknown_526c50()
{
    if (unknown_4c) {
        return unknown_4c->Queue->Avg_Delay();
    }
    return 0;
}


// unsigned long IPXManagerClass::unknown_526c70() (0x00526C70)
// The multicast channel's mean delay (0 without one).

unsigned long IPXManagerClass::unknown_526c70()
{
    if (unknown_50) {
        return unknown_50->Queue->Avg_Delay();
    }
    return 0;
}


// void IPXManagerClass::Store_Stats() (0x00526C90)
// Records each remote player's connection statistics: name and address
// once, the worst round trip, the worst average round trip, resends and
// losses.

void IPXManagerClass::Store_Stats()
{
    for (int i = 0; i < NumConnections; i++) {
        HouseClass *house = Houses[Connection[i]->ID];

        if (house != 0 && house != unknown_player_a35db4) {
            UnknownNetworkPlayerStatistics &stats = player_statistics_00a3d584[i];
            if (stats.unknown_00[0] == '\0') {
                WideToCharString_006fa0f0(stats.unknown_00, Connection[i]->Name, -1);
                stats.Address = Connection[i]->Address;
            }

            int maxtime = Connection[i]->Queue->Max_Delay();
            int maxround = 1000 * maxtime / 60;
            stats.MaxRoundTrip = ((int)stats.MaxRoundTrip > maxround) ? stats.MaxRoundTrip : maxround;

            unsigned int avgtime = Connection[i]->Queue->Avg_Delay();
            int avground = (int)(1000 * avgtime / 60);
            stats.MaxAverageRoundTrip = (avground > (int)stats.MaxAverageRoundTrip) ? avground : stats.MaxAverageRoundTrip;

            stats.Resends = Connection[i]->unknown_08;
            stats.Lost = Connection[i]->unknown_0c;
            stats.PercentLost = Connection[i]->unknown_10;
        }
    }
}


// void IPXManagerClass::Multiplayer_Debug_Print_00526d90() (0x00526D90; TibSun's Multiplayer_Debug_Print)
// The network statistics overlay: retry delta/timeout, latency fudge, frame syncs per second, the multicast
// response time, then per connection its name, queue delays, resends, losses, process time, frames behind
// (the session's per-player array), queue counts, missed packets and the router response time.

template <class T>
class BasicTimerClass {
public:
    BasicTimerClass(int set = 0) : Started(Timer - set) {}
    ~BasicTimerClass() {}
    int Value() const { return Timer - Started; }
    operator int() const { return Value(); }
    int Started;
    T Timer;
};
extern BasicTimerClass<SystemTimerClass> unknown_frame_sync_timer_a780c0;

void IPXManagerClass::Multiplayer_Debug_Print_00526d90()
{
    wchar_t buffer[256];
    wchar_t name[200];
    Surface *surface = unknown_surface_839964;

    swprintf(buffer, L"Rtry delta: %d", 1000 * RetryDelta / 60);
    Fancy_Text_Printf(buffer, surface, &surface->Get_Rect(), &Point2D(0, 402), Fetch_Scheme_By_Name("Grey", 1), 0, 0x19);

    swprintf(buffer, L"Rtry timeout: %d", 1000 * Timeout / 60);
    Fancy_Text_Printf(buffer, surface, &surface->Get_Rect(), &Point2D(0, 412), Fetch_Scheme_By_Name("Grey", 1), 0, 0x19);

    swprintf(buffer, L"Lat Fudge : %d", unknown_a3fba4);
    Fancy_Text_Printf(buffer, surface, &surface->Get_Rect(), &Point2D(0, 422), Fetch_Scheme_By_Name("Grey", 1), 0, 0x19);

    if (unknown_frame_sync_timer_a780c0 / 60) {
        swprintf(buffer, L"FSPS : %d", unknown_frame_syncs_ab62e4 / (unknown_frame_sync_timer_a780c0 / 60));
        Fancy_Text_Printf(buffer, surface, &surface->Get_Rect(), &Point2D(0, 432), Fetch_Scheme_By_Name("Grey", 1), 0, 0x19);
        if ((Frame & 0x7F) == 0x7F) {
            unknown_frame_sync_timer_a780c0 = 0;
            unknown_frame_syncs_ab62e4 = 0;
        }
    }

    if (unknown_game_mode_a3d298 == 4 && unknown_flag_a3fba8) {
        swprintf(buffer, L"MC resp: %d", 1000 * unknown_ipx_manager_a40978.unknown_526c70() / 60);
        Fancy_Text_Printf(buffer, surface, &surface->Get_Rect(), &Point2D(0, 442), Fetch_Scheme_By_Name("Grey", 1), 0, 0x19);
    }

    for (int i = 0; i < NumConnections; i++) {
        HouseClass *house = Houses[Connection[i]->ID];
        if (house != 0 && house != unknown_player_a35db4) {
            int scheme = house->ColorSchemeIndex;
            int x = (i + 1) * 100;

            swprintf(name, L"%s", Connection[i]->Name);
            Fancy_Text_Printf(name, surface, &surface->Get_Rect(), &Point2D(x, 302), ColorSchemes[scheme], 0, 0x19);

            int avg = Connection[i]->Queue->Avg_Delay();
            swprintf(buffer, L"Average  : %d", 1000 * avg / 60);
            Fancy_Text_Printf(buffer, surface, &surface->Get_Rect(), &Point2D(x, 311), ColorSchemes[scheme], 0, 0x19);

            int max = Connection[i]->Queue->Max_Delay();
            swprintf(buffer, L"Max      : %d", 1000 * max / 60);
            Fancy_Text_Printf(buffer, surface, &surface->Get_Rect(), &Point2D(x, 319), ColorSchemes[scheme], 0, 0x19);

            int resends = Connection[i]->unknown_08;
            swprintf(buffer, L"Resends  : %d", resends);
            Fancy_Text_Printf(buffer, surface, &surface->Get_Rect(), &Point2D(x, 327), ColorSchemes[scheme], 0, 0x19);

            int numlost = (0 > (int)Connection[i]->unknown_0c) ? 0 : (int)Connection[i]->unknown_0c;
            swprintf(buffer, L"Num lost : %d", numlost);
            Fancy_Text_Printf(buffer, surface, &surface->Get_Rect(), &Point2D(x, 335), ColorSchemes[scheme], 0, 0x19);

            int pcnt_lost = Connection[i]->unknown_10;
            swprintf(buffer, L"Pcnt lost: %d", pcnt_lost);
            Fancy_Text_Printf(buffer, surface, &surface->Get_Rect(), &Point2D(x, 343), ColorSchemes[scheme], 0, 0x19);

            int process_time = 0;
            for (int j = 0; j < unknown_players_a3fa7c.Count(); ++j) {
                if (Connection[i]->ID == unknown_players_a3fa7c[j]->ID) {
                    process_time = unknown_players_a3fa7c[j]->unknown_63;
                    break;
                }
            }
            swprintf(buffer, L"Process : %d", process_time);
            Fancy_Text_Printf(buffer, surface, &surface->Get_Rect(), &Point2D(x, 351), ColorSchemes[scheme], 0, 0x19);

            swprintf(buffer, L"Frame   : %d", -unknown_session_a3d298.unknown_frames_behind_28ec[i]);
            Fancy_Text_Printf(buffer, surface, &surface->Get_Rect(), &Point2D(x, 359), ColorSchemes[scheme], 0, 0x19);

            swprintf(buffer, L"Queue s/r: %d/%d", Connection[i]->Queue->SendCount, Connection[i]->Queue->ReceiveCount);
            Fancy_Text_Printf(buffer, surface, &surface->Get_Rect(), &Point2D(x, 367), ColorSchemes[scheme], 0, 0x19);

            swprintf(buffer, L"Missed o/m: %d/%d", Connection[i]->unknown_14, Connection[i]->unknown_18);
            Fancy_Text_Printf(buffer, surface, &surface->Get_Rect(), &Point2D(x, 375), ColorSchemes[scheme], 0, 0x19);

            if (unknown_game_mode_a3d298 == 4 && unknown_flag_a3fba8) {
                swprintf(buffer, L"Rout resp: %d", 1000 * unknown_response_times_a76c88[i] / 60);
                Fancy_Text_Printf(buffer, surface, &surface->Get_Rect(), &Point2D(x, 383), ColorSchemes[scheme], 0, 0x19);
            }
        }
    }
}


// void IPXManagerClass::Reset_Response_Time(bool clear) (0x00527630, slot 11)
// Resets the connections' delay statistics (the channels' are cleared).

void IPXManagerClass::Reset_Response_Time(bool clear)
{
    for (int i = 0; i < NumConnections; i++) {
        Connection[i]->Queue->Reset_Delay(clear);
    }
    if (GlobalChannel) {
        GlobalChannel->Queue->Reset_Delay(true);
    }
    if (unknown_4c) {
        unknown_4c->Queue->Reset_Delay(true);
    }
    if (unknown_50) {
        unknown_50->Queue->Reset_Delay(true);
    }
}


// char *IPXManagerClass::unknown_5276a0() (0x005276A0)
// The packet of the oldest unacknowledged sequenced send across the
// connections (the first such entry of each), or null.

char *IPXManagerClass::unknown_5276a0()
{
    unsigned long oldest = 0xFFFFFFFF;
    char *packet = 0;
    for (int i = 0; i < NumConnections; i++) {
        SendQueueType *send = 0;
        for (int j = 0; j < Connection[i]->Queue->SendCount; j++) {
            send = Connection[i]->Queue->Get_Send(j);
            if (send != 0) {
                if (((CommHeaderType *)send->Buffer)->Code == PACKET_DATA_ACK && !send->IsACK) {
                    break;
                }
                send = 0;
            }
        }
        if (send != 0 && send->FirstTime < oldest) {
            oldest = send->FirstTime;
            packet = send->Buffer;
        }
    }
    return packet;
}


// void IPXManagerClass::Set_Bridge(NetNumType bridge) (0x00527730; Red Alert's Set_Bridge).

void IPXManagerClass::Set_Bridge(NetNumType bridge)
{
    if (GlobalChannel) {
        GlobalChannel->Set_Bridge(bridge);
    }
}


// void IPXManagerClass::Configure_Debug(int index, int offset, int size, int names, int namestart, int namecount) (0x00527750, slot 15)
// Debug configuration for the global channel (-1) or a connection's queue.

void IPXManagerClass::Configure_Debug(int index, int offset, int size, int names, int namestart, int namecount)
{
    if (index == -1) {
        GlobalChannel->Queue->Configure_Debug(offset, size, names, namestart, namecount);
    } else if (Connection[index]) {
        Connection[index]->Queue->Configure_Debug(offset, size, names, namestart, namecount);
    }
}


// void IPXManagerClass::Mono_Debug_Print(int index, int refresh) (0x005277B0, slot 24)
// Inline (empty) in shared/ipx_manager.hpp; emitted with the vtable. No source of its own.


// void IPXManagerClass::unknown_5277c0(IPXAddressClass *address) (0x005277C0)
// The second global channel's address.

void IPXManagerClass::unknown_5277c0(IPXAddressClass *address)
{
    if (unknown_4c) {
        unknown_4c->Address = *address;
    }
}


// void IPXManagerClass::unknown_5277f0() (0x005277F0)

void IPXManagerClass::unknown_5277f0()
{
    if (GlobalChannel) {
        GlobalChannel->unknown_c8 = false;
    }
}


// void IPXManagerClass::unknown_527800() (0x00527800)

void IPXManagerClass::unknown_527800()
{
    if (GlobalChannel) {
        GlobalChannel->unknown_c8 = true;
    }
}


// void IPXManagerClass::unknown_527810(IPXAddressClass *address, unsigned short id) (0x00527810)
// Drops the global channel's unacknowledged sends to an address.

void IPXManagerClass::unknown_527810(IPXAddressClass *address, unsigned short id)
{
    if (GlobalChannel) {
        GlobalChannel->Discard_Packets_To(address, id);
    }
}


// void IPXManagerClass::unknown_527830(int index) (0x00527830)

void IPXManagerClass::unknown_527830(int index)
{
    if (GlobalChannel) {
        GlobalChannel->Discard_Old_Packets(index);
    }
}


// void *IPXManagerClass::`scalar deleting destructor'(unsigned int flags) (0x00527850)
// Compiler-generated with the vtable (calls the destructor). No source of its own.


// void *ConnManClass::`scalar deleting destructor'(unsigned int flags) (0x00527870)
// Compiler-generated with ConnManClass's vtable (its inline empty destructor). No source of its own.
