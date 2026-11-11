// Behaviour rows (Scout1209 r8; NOT EXACT, native/behaviour_rewrites.json): the LAN game's
// connection manager service (runs only with RA2_NATIVE_LAN=1; port/src/wsock32.cpp carries
// IPX over UDP).
#include "shared/ipx_manager.hpp"
#include "shared/winsock_interface.hpp"
#include "shared/connection.hpp"
#include "shared/comm_buffer.hpp"
#include "shared/debug_printf.hpp"



// 0x00525F60 IPXManagerClass::Service (slot 1; TibSun ipxmgr.cpp IPXManagerClass::Service, the
// RA2 differences from inventory/decompiled/00525f60.c): reads every packet the transport holds
// (slot 2 Read, 1,024-byte buffer, 128-byte address) and gives it, by the magic number at its
// start, to the global channel (+0x48: slot 12 Receive_Packet with the sender and port) or, for
// the product's magic (+0x20), to the private connection (+0x28..) whose address is the
// sender's (slot 3); unknown_525dc0 0x00525DC0 decides first whether a packet is forwarded
// through the local router instead. A full queue counts a receive overflow (+0xA4) and stops
// the reading. Then the global channel is serviced (slot 5; on failure slot 6 drops the
// undeliverable sends) and each connection (slot 5; a failing one is the bad connection,
// +0xA8, -1 when all are well). Returns 0 when anything failed.
// NOT CARRIED (Internet games only; flagged in behaviour_rewrites.json): the router channel
// (+0x4C, magic 0xCDEF, and its relay when 0x00A3FBA8 is set), the multicast connection (+0x50),
// and the NAT port tracking that rewrites players' addresses (type 4).
int IPXManagerClass::Service()
{
    int rc = 1;
    char buffer[0x400];
    char address_data[0x80];
    while (PacketTransport != 0) {
        int buflen = sizeof buffer, addrlen = sizeof address_data;
        unsigned short port = 0;
        int packetlen = PacketTransport->Read(buffer, &buflen, address_data, &addrlen, &port);
        if (packetlen == 0) break;
        IPXAddressClass address = *(IPXAddressClass *)address_data;
        char *current = buffer;
        memcpy(unknown_84 + 8, &current, sizeof current);   // CurDataBuf (+0x8C), as the original keeps it
        CommHeaderType *packet = (CommHeaderType *)buffer;
        if (GlobalChannel != 0 && packet->MagicNumber == GlobalChannel->MagicNum) {
            Debug_Printf("Adding packet from %s (%d) to global queue\n", address.As_String_00523840(), (int)port);
            if (!unknown_525dc0(packet, buffer, packetlen, &address, port)) {
                if (!GlobalChannel->Receive_Packet(buffer, packetlen, &address, port)) {
                    ++unknown_a4;
                    Debug_Printf("GlobalChannel recive buffer overflow %d\n", unknown_a4);
                    break;
                }
            }
        } else if (packet->MagicNumber == ProductID && !unknown_525dc0(packet, buffer, packetlen, &address, port)) {
            for (int i = 0; i < NumConnections; ++i) {
                if (Connection[i]->Address == address) {
                    if (!Connection[i]->Receive_Packet(buffer, packetlen)) {
                        ++unknown_a4;
                        Debug_Printf("Reecive buffer overflow %d\n", unknown_a4);
                    }
                    break;
                }
            }
        }
    }
    if (GlobalChannel != 0 && PacketTransport != 0 && !GlobalChannel->Service()) {
        GlobalChannel->Discard_Undeliverable_Packets();
        rc = 0;
    }
    if (PacketTransport != 0) {
        for (int i = 0; i < NumConnections; ++i) {
            if (!Connection[i]->Service()) {
                unknown_a8 = Connection[i]->ID;
                rc = 0;
                Debug_Printf("Error - Connection %d has gone bad\n", unknown_a8);
            }
        }
    }
    if (rc) unknown_a8 = -1;
    return rc;
}
