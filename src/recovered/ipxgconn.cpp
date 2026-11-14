// ipxgconn.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/ipxgconn.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/ipx_global_conn.hpp"
#include "shared/owner_talk.hpp"
#include "shared/unknown_player_entry.hpp"


// Header-static dynamic initializers of the unit at 0x00523FC0: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x00523FC0-0x005241A1); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x00523FE0, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_00523fc0.cpp; no source of its own.


// Dynamic initializer (0x00524010, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_00523fc0.cpp; no source of its own.


// Dynamic initializer (0x00524030, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_00523fc0.cpp; no source of its own.


// Dynamic initializer (0x00524050, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_00523fc0.cpp; no source of its own.


// Dynamic initializer (0x00524070, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_00523fc0.cpp; no source of its own.


// Dynamic initializer (0x00524090, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_00523fc0.cpp; no source of its own.


// Dynamic initializer (0x005240B0, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_00523fc0.cpp; no source of its own.


// Dynamic initializer (0x005240F0, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_00523fc0.cpp; no source of its own.


// Dynamic initializer (0x00524120, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_00523fc0.cpp; no source of its own.


// Dynamic initializer (0x00524150, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_00523fc0.cpp; no source of its own.


// Dynamic initializer (0x00524180, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_00523fc0.cpp; no source of its own.


// Dynamic initializer (0x00524190, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_00523fc0.cpp; no source of its own.


// IPXGlobalConnClass::IPXGlobalConnClass(int numsend, int numreceive, int maxlen, unsigned short product_id, unsigned short magicnum) (0x005241B0)
// An IPX connection with no address (two extra bytes per packet, 12 bytes
// of extra data for the sender's address) and per-receive-entry address and
// ID tables.

IPXGlobalConnClass::IPXGlobalConnClass(int numsend, int numreceive, int maxlen, unsigned short product_id, unsigned short magicnum) :
    IPXConnClass(numsend, numreceive, maxlen + 2, magicnum, 0, 0, unknown_empty_wide_839d4c, sizeof(IPXAddressClass))
{
    ProductID = product_id;
    IsBridge = 0;
    unknown_c8 = true;
    unknown_d4 = numreceive;
    unknown_d0 = new int[unknown_d4];
    unknown_cc = new IPXAddressClass[unknown_d4];
    for (int i = 0; i < unknown_d4; i++) {
        unknown_d0[i] = -1;
    }
    unknown_d8 = 0;
}


// IPXGlobalConnClass::~IPXGlobalConnClass() (0x00524290)

IPXGlobalConnClass::~IPXGlobalConnClass()
{
    if (unknown_d0) {
        delete[] unknown_d0;
        unknown_d0 = 0;
    }
    if (unknown_cc) {
        delete[] unknown_cc;
        unknown_cc = 0;
    }
}


// int IPXGlobalConnClass::Send_Packet(void *buf, int buflen, IPXAddressClass *address, int ack_req, unsigned short id, unsigned long packet_id) (0x005242F0, slot 11)
// Wraps the data in a global header (sequenced only with an ACK and an
// address; numbered by the send total unless a number is given) and queues
// it with the destination address as extra data.

int IPXGlobalConnClass::Send_Packet(void *buf, int buflen, IPXAddressClass *address, int ack_req, unsigned short id, unsigned long packet_id)
{
    IPXAddressClass dest_addr;

    ((GlobalHeaderType *)PacketBuf)->unknown_08 = 0;
    ((GlobalHeaderType *)PacketBuf)->unknown_0c = 0;
    ((GlobalHeaderType *)PacketBuf)->MagicNumber = MagicNum;
    if (ack_req && address) {
        ((GlobalHeaderType *)PacketBuf)->Code = PACKET_DATA_ACK;
    } else {
        ((GlobalHeaderType *)PacketBuf)->Code = PACKET_DATA_NOACK;
    }
    ((GlobalHeaderType *)PacketBuf)->PacketID = Queue->SendTotal;
    if (packet_id) {
        ((GlobalHeaderType *)PacketBuf)->PacketID = packet_id;
    }
    ((GlobalHeaderType *)PacketBuf)->ProductID = ProductID;
    ((GlobalHeaderType *)PacketBuf)->unknown_03 = 0;
    if (address) {
        dest_addr = *address;
    }
    memcpy(PacketBuf + sizeof(GlobalHeaderType), buf, buflen);
    return Queue->Queue_Send(PacketBuf, buflen + sizeof(GlobalHeaderType), &dest_addr, sizeof(IPXAddressClass), id);
}


// Receive global IPX/UDP packets, reject duplicates, and acknowledge reliable data (0x005243D0).

int IPXGlobalConnClass::Receive_Packet(void *buf, int buflen, IPXAddressClass *address, unsigned short port)
{
    GlobalHeaderType *packet = (GlobalHeaderType *)buf;
    if (packet->MagicNumber != MagicNum) return 0;
    switch (packet->Code) {
    case PACKET_DATA_ACK: {
        IPXAddressClass received = *address;
        if (port != 0) received.Set_Port(port);
        int resend = 0;
        for (int index = 0; index < unknown_d4; index++) {
            if ((unsigned int)index >= Queue->ReceiveTotal) break;
            if (received == unknown_cc[index] && packet->PacketID == (unsigned long)unknown_d0[index]) {
                resend = 1;
                break;
            }
        }
        Debug_Printf("Queueing new global PACKET_DATA_ACK packet\n");
        if (!resend) {
            if (Queue->Queue_Receive(buf, buflen, &received, sizeof(received))) {
                unknown_cc[unknown_d8] = received;
                unknown_d0[unknown_d8] = packet->PacketID;
                unknown_d8++;
                if (unknown_d8 >= unknown_d4) unknown_d8 = 0;
            } else {
                Debug_Printf("Failed to queue incoming packet. Ack not sent\n");
                break;
            }
        }
        GlobalHeaderType acknowledgement;
        acknowledgement.MagicNumber = MagicNum;
        acknowledgement.Code = PACKET_ACK;
        acknowledgement.PacketID = packet->PacketID;
        acknowledgement.ProductID = ProductID;
        acknowledgement.unknown_03 = 0;
        Debug_Printf("Sending global ack packet to port %d\n", port);
        ConnectionClass *global_channel = unknown_connection_a409c4;
        if (!Send((char *)&acknowledgement, sizeof(acknowledgement), address, sizeof(IPXAddressClass), this == global_channel, port)) {
            Debug_Printf("Failed to ack global packet\n");
        }
        break;
    }
    case PACKET_DATA_NOACK: {
        Debug_Printf("Queueing new global PACKET_DATA_NOACK packet\n");
        IPXAddressClass received = *address;
        if (port != 0) received.Set_Port(port);
        Queue->Queue_Receive(buf, buflen, &received, sizeof(received));
        break;
    }
    case PACKET_ACK: {
        Debug_Printf("Queueing new global PACKET_ACK packet\n");
        for (int index = 0; index < Queue->SendCount; index++) {
            SendQueueType *entry = Queue->Get_Send(index);
            GlobalHeaderType *sent = (GlobalHeaderType *)entry->Buffer;
            if (packet->PacketID == sent->PacketID && sent->Code == PACKET_DATA_ACK) {
                entry->IsACK = 1;
                Debug_Printf("Received global ack for packet %d\n", packet->PacketID);
                break;
            }
        }
        break;
    }
    default:
        break;
    }
    return 1;
}


// int IPXGlobalConnClass::Get_Packet(void *buf, int *buflen, IPXAddressClass *address, unsigned short *product_id) (0x00524670, slot 13)
// Reads the first received packet if unread: its data, product ID and
// sender address.

int IPXGlobalConnClass::Get_Packet(void *buf, int *buflen, IPXAddressClass *address, unsigned short *product_id)
{
    if (Queue->ReceiveCount == 0) {
        return 0;
    }
    ReceiveQueueType *rec = Queue->Get_Receive(0);
    if (rec == 0 || rec->IsRead) {
        return 0;
    }
    GlobalHeaderType *packet = (GlobalHeaderType *)rec->Buffer;
    rec->IsRead = 1;
    int packetlen = rec->BufLen - sizeof(GlobalHeaderType);
    if (packetlen > 0) {
        memcpy(buf, rec->Buffer + sizeof(GlobalHeaderType), packetlen);
    }
    *buflen = packetlen;
    *product_id = packet->ProductID;
    *address = *((IPXAddressClass *)rec->ExtraBuffer);
    return 1;
}


// int IPXGlobalConnClass::Peek_Packet(int index, void *buf, int *buflen, IPXAddressClass *address, unsigned short *product_id, unsigned long *packet_id) (0x00524700, slot 14)
// Reads the index'th received packet if unread, without marking it read.

int IPXGlobalConnClass::Peek_Packet(int index, void *buf, int *buflen, IPXAddressClass *address, unsigned short *product_id, unsigned long *packet_id)
{
    if (Queue->ReceiveCount < index) {
        return 0;
    }
    ReceiveQueueType *rec = Queue->Get_Receive(index);
    if (rec == 0 || rec->IsRead) {
        return 0;
    }
    GlobalHeaderType *packet = (GlobalHeaderType *)rec->Buffer;
    int packetlen = rec->BufLen - sizeof(GlobalHeaderType);
    if (packetlen > 0) {
        memcpy(buf, rec->Buffer + sizeof(GlobalHeaderType), packetlen);
    }
    *buflen = packetlen;
    *product_id = packet->ProductID;
    *address = *((IPXAddressClass *)rec->ExtraBuffer);
    if (packet_id) {
        *packet_id = packet->PacketID;
    }
    return 1;
}


// void IPXGlobalConnClass::Mark_Packet_Read(int index) (0x00524790, slot 15)
// Marks the index'th received packet read.

void IPXGlobalConnClass::Mark_Packet_Read(int index)
{
    if (index < Queue->ReceiveCount) {
        Queue->Get_Receive(index)->IsRead = 1;
    }
}


// int IPXGlobalConnClass::Send(char *buf, int buflen, void *extrabuf, int extralen, bool flagged, unsigned short id) (0x005247B0, slot 9)
// Sends to the address carried as the packet's extra data: broadcast, to
// the bridge node when it is on the bridged network, or directly.

int IPXGlobalConnClass::Send(char *buf, int buflen, void *extrabuf, int extralen, bool flagged, unsigned short id)
{
    IPXAddressClass *address = (IPXAddressClass *)extrabuf;
    if (address->Is_Broadcast()) {
        return Broadcast(buf, buflen);
    }
    if (unknown_game_mode_a3d298 == 3 && IsBridge && !memcmp(address, BridgeNet, 4)) {
        return Send_To(buf, buflen, address, BridgeNode, flagged, 0);
    }
    return Send_To(buf, buflen, address, 0, flagged, id);
}


// int IPXGlobalConnClass::Service_Receive_Queue() (0x00524830, slot 8)
// Drops every packet already read.

int IPXGlobalConnClass::Service_Receive_Queue()
{
    for (int i = 0; i < Queue->ReceiveCount; i++) {
        if (Queue->Get_Receive(i)->IsRead) {
            Queue->UnQueue_Receive(0, 0, i, 0, 0);
            i--;
        }
    }
    return 1;
}


// void IPXGlobalConnClass::Discard_Old_Packets(unsigned long age) (0x00524870, slot 17)
// Strips received global packets older than AGE ticks from the receive queue
// and forgets their packet IDs in the duplicate ring (unknown_d0).

void IPXGlobalConnClass::Discard_Old_Packets(unsigned long age)
{
    Debug_Printf("Stripping queue entries older than %d seconds. Num receive = %d\n", age / 60, Queue->ReceiveCount);
    for (int i = 0; i < Queue->ReceiveCount; i++) {
        ReceiveQueueType *rec = Queue->Get_Receive(i);
        Debug_Printf("i = %d\n", i);
        if (rec) {
            Debug_Printf("Checking packet %d\n", i);
            unsigned long const &received = rec->ReceiveTime;
            if ((long)(received + age) < TickCount.Time()) {
                Debug_Printf("Stripping global packet due to age of %d seconds\n", (int)(TickCount.Time() - rec->ReceiveTime) / 60);
                unsigned long id = ((CommHeaderType *)rec->Buffer)->PacketID;
                for (int j = 0; j < unknown_d4; j++) {
                    if (unknown_d0[j] == id) {
                        unknown_d0[j] = -1;
                        break;
                    }
                }
                Queue->UnQueue_Receive(0, 0, i, 0, 0);
                i--;
            } else {
                Debug_Printf("Leaving global packet due to age of %d seconds\n", (int)(TickCount.Time() - rec->ReceiveTime) / 60);
            }
        }
    }
    Debug_Printf("Done stripping. Num receive = %d\n", Queue->ReceiveCount);
}


// void IPXGlobalConnClass::Set_Bridge(NetNumType bridge) (0x00524A10; Red Alert's Set_Bridge).
// Clears IsBridge once the IPX class is configured (the bridge network is unused under Winsock).

void IPXGlobalConnClass::Set_Bridge(NetNumType bridge)
{
    if (Configured) {
        IsBridge = 0;
    }
}


// int IPXGlobalConnClass::Discard_Undeliverable_Packets() (0x00524A30, slot 6)
// Removes the sends that failed (retry or time limit) without an ACK and
// returns how many.

int IPXGlobalConnClass::Discard_Undeliverable_Packets()
{
    int count = 0;
    for (int i = 0; i < Queue->SendCount; i++) {
        SendQueueType *send = Queue->Get_Send(i);
        if (send->unknown_bit2 && !send->IsACK) {
            char buffer[1024];
            int buflen;
            IPXAddressClass address;
            int addresslen = sizeof(address);
            Queue->UnQueue_Send(buffer, &buflen, i, &address, &addresslen, 0);
            i--;
            count++;
        }
    }
    return count;
}


// int IPXGlobalConnClass::Discard_Packets_To(IPXAddressClass *address, unsigned short id) (0x00524AA0, slot 16)
// Removes the unacknowledged sends to an address (same node) with the given ID and
// returns how many.

int IPXGlobalConnClass::Discard_Packets_To(IPXAddressClass *address, unsigned short id)
{
    int count = 0;
    for (int i = 0; i < Queue->SendCount; i++) {
        SendQueueType *send = Queue->Get_Send(i);
        if (!send->IsACK && send->ExtraLen
            && ((IPXAddressClass *)send->ExtraBuffer)->Is_Same_Node(*address) && send->unknown_20 == id) {
            char buffer[1024];
            int buflen;
            IPXAddressClass dest;
            int destlen = sizeof(dest);
            Queue->UnQueue_Send(buffer, &buflen, i, &dest, &destlen, 0);
            i--;
            count++;
        }
    }
    return count;
}


// int IPXGlobalConnClass::Send_To(char *buf, int buflen, IPXAddressClass *address, unsigned char *immed, bool flagged, unsigned short id) (0x00524B50, slot 10)
// Sends a packet to an address (through a bridge network when given). In the
// forwarding session (game mode 4) an untargeted header gets the target of the
// node whose address matches (1 when none), and the packet goes to the
// forwarding address; the header carries the forwarding port unless that
// address is the first node's.

int IPXGlobalConnClass::Send_To(char *buf, int buflen, IPXAddressClass *address, unsigned char *immed, bool flagged, unsigned short id)
{
    IPXAddressClass dest;
    NetNumType net;
    NetNodeType node;
    dest = *address;
    if (immed) {
        address->Get_Address(net, node);
        memcpy(net, immed, 4);
        dest.Set_Address(net, node, 0);
    }

    unsigned short port;
    CommHeaderType *header = (CommHeaderType *)buf;
    if (unknown_game_mode_a3d298 == 4 && !flagged && unknown_flag_a3fba8 && unknown_c8) {
        if (header->unknown_03 == 0) {
            IPXAddressClass sender = dest;
            sender.Get_Address(net, node);
            sender.Set_Address(net, node, id);
            int target_id = -1;
            for (int i = 0; i < unknown_players_a3fa7c.Count(); i++) {
                if (unknown_players_a3fa7c[i]->Address == sender) {
                    target_id = unknown_players_a3fa7c[i]->ID;
                    break;
                }
            }
            unsigned char target = unknown_OwnerTalkClass_00ab6d18.Unknown_0063F4C0(target_id);
            if (!target) {
                target = 1;
            }
            header->unknown_03 = target;
        }
        dest = unknown_forward_address_a3fbac;
        port = unknown_port_7f60dc;
        if (unknown_forward_address_a3fbb8 == unknown_players_a3fa7c[0]->Address
            && dest.Is_Same_Node(unknown_players_a3fa7c[0]->Address)) {
            header->unknown_0c = 0;
            memset(&header->unknown_08, 0, sizeof(header->unknown_08));
        } else {
            port = unknown_forward_port_a3fbc4;
            header->unknown_0c = unknown_port_7f60dc;
            memset(&header->unknown_08, 0, sizeof(header->unknown_08));
        }
    } else {
        header->unknown_0c = 0;
        memset(&header->unknown_08, 0, sizeof(header->unknown_08));
        port = id;
    }

    if (PacketTransport) {
        PacketTransport->WriteTo(buf, buflen, &dest, sizeof(dest), flagged, !unknown_c8, port);
    }
    return 1;
}


// int IPXGlobalConnClass::Send_Packet(void *buf, int buflen, int ack_req, char target) (0x00524D70, slot 2)
// Inline in shared/ipx_global_conn.hpp (ConnectionClass's); emitted with the vtable. No source of its own.


// int IPXGlobalConnClass::Receive_Packet(void *buf, int buflen) (0x00524D90, slot 3)
// Inline in shared/ipx_global_conn.hpp (ConnectionClass's); emitted with the vtable. No source of its own.


// int IPXGlobalConnClass::Get_Packet(void *buf, int *buflen) (0x00524DB0, slot 4)
// Inline in shared/ipx_global_conn.hpp (ConnectionClass's); emitted with the vtable. No source of its own.


// void *IPXGlobalConnClass::`scalar deleting destructor'(unsigned int flags) (0x00524DD0)
// Compiler-generated with the vtable (the destructor inlined). No source of its own.
