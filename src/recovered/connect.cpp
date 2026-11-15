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
// Function comment blocks from mischa85/RedAlert2 392e904 src/connect.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// connect.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/connect.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/connection.hpp"


// Header-static dynamic initializers of the unit at 0x00480C30: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x00480C30-0x00480E11); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x00480C50, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_00480c30.cpp; no source of its own.


// Dynamic initializer (0x00480C80, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_00480c30.cpp; no source of its own.


// Dynamic initializer (0x00480CA0, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_00480c30.cpp; no source of its own.


// Dynamic initializer (0x00480CC0, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_00480c30.cpp; no source of its own.


// Dynamic initializer (0x00480CE0, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_00480c30.cpp; no source of its own.


// Dynamic initializer (0x00480D00, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_00480c30.cpp; no source of its own.


// Dynamic initializer (0x00480D20, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_00480c30.cpp; no source of its own.


// Dynamic initializer (0x00480D60, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_00480c30.cpp; no source of its own.


// Dynamic initializer (0x00480D90, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_00480c30.cpp; no source of its own.


// Dynamic initializer (0x00480DC0, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_00480c30.cpp; no source of its own.


// Dynamic initializer (0x00480DF0, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_00480c30.cpp; no source of its own.


// Dynamic initializer (0x00480E00, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_00480c30.cpp; no source of its own.


/***************************************************************************
 * ConnectionClass::ConnectionClass -- class constructor                   *
 *                                                                         *
 * INPUT:                                                                  *
 *      numsend         desired # of entries for the send queue            *
 *      numreceive      desired # of entries for the receive queue         *
 *      maxlen         max length of an application packet                 *
 *      magicnum         the packet "magic number" for this connection     *
 *      retry_delta      the time to wait between sends                    *
 *      max_retries      the max # of retries allowed for a packet         *
 *                     (-1 means retry forever, based on this parameter)   *
 *      timeout         the max amount of time before we give up on a packet*
 *                     (-1 means retry forever, based on this parameter)   *
 *      extralen         max size of app-specific extra bytes (optional)   *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
// (0x00480E20)
// Keeps the protocol settings and creates the packet buffer and queue.

ConnectionClass::ConnectionClass(int numsend, int numreceive, int maxlen, unsigned short magicnum,
    unsigned long retry_delta, unsigned long max_retries, unsigned long timeout, int extralen)
{
    MaxPacketLen = maxlen + sizeof(CommHeaderType);
    MagicNum = magicnum;
    RetryDelta = retry_delta;
    MaxRetries = max_retries;
    Timeout = timeout;
    PacketBuf = new char[MaxPacketLen];
    Queue = new CommBufferClass(numsend, numreceive, MaxPacketLen, extralen);
}


/***************************************************************************
 * ConnectionClass::~ConnectionClass -- class destructor                   *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
// (0x00480EA0)

ConnectionClass::~ConnectionClass()
{
    delete[] PacketBuf;
    if (Queue) {
        delete Queue;
    }
}


/***************************************************************************
 * ConnectionClass::Init -- Initializes connection queue to empty          *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
// (0x00480ED0, slot 1)
// Clears the counters and statistics and empties the queue.

void ConnectionClass::Init()
{
    NumRecNoAck = 0;
    NumRecAck = 0;
    NumSendNoAck = 0;
    NumSendAck = 0;
    unknown_08 = 0;
    unknown_0c = 0;
    unknown_10 = 0;
    unknown_14 = 0;
    unknown_18 = 0;
    LastSeqID = 0xFFFFFFFF;
    LastReadID = 0xFFFFFFFF;
    Queue->Init();
}


/***************************************************************************
 * ConnectionClass::Send_Packet -- adds a packet to the send queue         *
 *                                                                         *
 * This routine prefixes the given buffer with a CommHeaderType and        *
 * queues the resulting packet into the Send Queue.  (It's actually the    *
 * Service() routine that handles the hardware-dependent Send of the data).*
 * The packet's MagicNumber, Code, and PacketID are set here.              *
 *                                                                         *
 * INPUT:                                                                  *
 *      buf         buffer to send                                         *
 *      buflen      length of buffer                                       *
 *      ack_req      1 = ACK is required for this packet; 0 = isn't        *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      1 = packet was queue'd OK, 0 = wasn't                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
// (0x00480F00, slot 2)
// Wraps the data in a header (sequenced when an ACK is wanted) and queues
// it for sending.

int ConnectionClass::Send_Packet(void *buf, int buflen, int ack_req, unsigned char target)
{
    ((CommHeaderType *)PacketBuf)->MagicNumber = MagicNum;
    if (ack_req) {
        ((CommHeaderType *)PacketBuf)->Code = PACKET_DATA_ACK;
        ((CommHeaderType *)PacketBuf)->PacketID = NumSendAck;
        Debug_Printf("NumSendAck = %d\n", NumSendAck);
    } else {
        ((CommHeaderType *)PacketBuf)->Code = PACKET_DATA_NOACK;
        ((CommHeaderType *)PacketBuf)->PacketID = NumSendNoAck;
    }
    if (unknown_game_mode_a3d298 == 4 && unknown_flag_a3fba8) {
        ((CommHeaderType *)PacketBuf)->unknown_03 = target;
    }
    if (MaxPacketLen < buflen) {
        Debug_Printf("MaxPacketLen = %d\n", MaxPacketLen);
        Debug_Printf("buflen = %d\n", buflen);
        return 0;
    }
    memcpy(PacketBuf + sizeof(CommHeaderType), buf, buflen);
    if (Queue->Queue_Send(PacketBuf, buflen + sizeof(CommHeaderType), 0, 0, 0)) {
        if (ack_req) {
            NumSendAck++;
        } else {
            NumSendNoAck++;
        }
        return 1;
    }
    return 0;
}


/***************************************************************************
 * ConnectionClass::Receive_Packet -- adds packet to receive queue         *
 *                                                                         *
 * INPUT:                                                                  *
 *      buf      buffer to process (already includes CommHeaderType)       *
 *      buflen   length of buffer to process                               *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      1 = packet was processed OK, 0 = error                             *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
int ConnectionClass::Receive_Packet(void *buf, int buflen)
{
    CommHeaderType *packet = (CommHeaderType *)buf;
    if (packet->MagicNumber != MagicNum) { ++unknown_18; return 0; }
    if (packet->Code == PACKET_ACK) {
        Debug_Printf("Received ACK for packet %d\n", packet->PacketID);
        for (int index = 0; index < Queue->SendCount; ++index) {
            SendQueueType *entry = Queue->Get_Send(index);
            if (entry && packet->PacketID == ((CommHeaderType *)entry->Buffer)->PacketID
                && ((CommHeaderType *)entry->Buffer)->Code == PACKET_DATA_ACK) {
                entry->IsACK = 1;
                break;
            }
        }
        return 1;
    }
    if (packet->Code == PACKET_DATA_NOACK) {
        if (Queue->MaxReceive - Queue->ReceiveCount <= 1) {
            ++unknown_14;
            Debug_Printf("Error - ConnectClass::Receive_Packet Queue full on PACKET_DATA_NOACK\n");
            return 0;
        }
        if (!Queue->Queue_Receive(buf, buflen, 0, 0)) {
            Debug_Printf("Error - unable to queue incoming packet. %s %s\n", "D:\\Ra2\\Connect.CPP", 0x14c);
            ++unknown_14;
            return 0;
        }
        ++NumRecNoAck;
        unknown_0c = packet->PacketID - NumRecNoAck + 1;
        if (packet->PacketID != 0)
            unknown_10 = 100 - NumRecNoAck * 100 / (packet->PacketID + 1);
        return 1;
    }
    if (packet->Code == PACKET_DATA_ACK) {
        if (packet->PacketID > LastSeqID || LastSeqID == 0xffffffff) {
            for (int index = 0; index < Queue->ReceiveCount; ++index) {
                ReceiveQueueType *entry = Queue->Get_Receive(index);
                if (entry && ((CommHeaderType *)entry->Buffer)->Code == PACKET_DATA_ACK
                    && ((CommHeaderType *)entry->Buffer)->PacketID == packet->PacketID)
                    goto send_ack;
            }
            if (Queue->MaxReceive - Queue->ReceiveCount <= 1 && packet->PacketID != LastSeqID + 1) {
                Debug_Printf("Error - ConnectClass::Receive_Packet Queue full on PACKET_DATA_ACK\n");
                return 0;
            }
            if (!Queue->Queue_Receive(buf, buflen, 0, 0)) {
                Debug_Printf("Error - ConnectClass::Receive_Packet Queue->Queue_Receive error\n");
                return 0;
            }
            ++NumRecAck;
            unsigned long next = packet->PacketID;
            if (next == LastSeqID + 1) {
                while (true) {
                    LastSeqID = next;
                    int index = 0;
                    if (Queue->ReceiveCount <= 0) break;
                    CommHeaderType *queued;
                    do {
                        ReceiveQueueType *entry = Queue->Get_Receive(index);
                        if (entry) {
                            queued = (CommHeaderType *)entry->Buffer;
                            if (queued->Code == PACKET_DATA_ACK && queued->PacketID == LastSeqID + 1) break;
                        }
                        ++index;
                        if (index >= Queue->ReceiveCount) goto send_ack;
                    } while (true);
                    next = queued->PacketID;
                }
            }
        }
send_ack:
        unsigned long receivedID = packet->PacketID;
        CommHeaderType ack;
        ack.MagicNumber = MagicNum;
        ack.Code = PACKET_ACK;
        ack.PacketID = receivedID;
        if (unknown_flag_a3fba8) ack.unknown_03 = packet->unknown_03;
        else ack.unknown_03 = 0;
        Debug_Printf("Sending ACK for packet %d to be forwarded to %02x\n", receivedID, ack.unknown_03);
        Send((char *)&ack, sizeof(ack), 0, 0, this == PacketRouterConnection, 0);
        return 1;
    }
    Debug_Printf("Error - ConnectClass::Receive_Packet did not understand packet->Code\n");
    return 0;
}


/***************************************************************************
 * ConnectionClass::Get_Packet -- gets a packet from receive queue         *
 *                                                                         *
 * INPUT:                                                                  *
 *      buf      location to store buffer                                  *
 *      buflen   filled in with length of 'buf'                            *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      1 = packet was read, 0 = wasn't                                    *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
// (0x004812E0, slot 4)
// Reads the next unread packet: the next sequenced one in order, or any
// unsequenced one; copies its data (without the header) out.

int ConnectionClass::Get_Packet(void *buf, int *buflen)
{
    for (int i = 0; i < Queue->ReceiveCount; i++) {
        ReceiveQueueType *rec = Queue->Get_Receive(i);
        if (rec != 0 && !rec->IsRead) {
            CommHeaderType *packet = (CommHeaderType *)rec->Buffer;
            if (packet->Code == PACKET_DATA_ACK && packet->PacketID == LastReadID + 1) {
                LastReadID = packet->PacketID;
            } else if (packet->Code != PACKET_DATA_NOACK) {
                continue;
            }
            rec->IsRead = 1;
            int packetlen = rec->BufLen - sizeof(CommHeaderType);
            if (packetlen > 0) {
                memcpy(buf, rec->Buffer + sizeof(CommHeaderType), packetlen);
            }
            *buflen = packetlen;
            return 1;
        }
    }
    return 0;
}


/***************************************************************************
 * ConnectionClass::Service -- main polling routine; services packets      *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      1 = OK, 0 = error (connection is broken!)                          *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
// (0x00481370, slot 5)

int ConnectionClass::Service()
{
    if (Service_Send_Queue() && Service_Receive_Queue()) {
        return 1;
    }
    return 0;
}


/***************************************************************************
 * ConnectionClass::Service_Send_Queue -- services the send queue          *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      1 = OK, 0 = error                                                  *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
// (0x004813A0, slot 7)
// Drops acknowledged packets (timing the round trip of sequenced ones),
// then resends whatever is due, flagging entries past the retry or time
// limits; 0 when the connection has gone bad.

int ConnectionClass::Service_Send_Queue()
{
    int i;
    int bad_conn = 0;
    SendQueueType *send;

    for (i = 0; i < Queue->SendCount; i++) {
        send = Queue->Get_Send(i);
        if (send->IsACK) {
            if (((CommHeaderType *)send->Buffer)->Code == PACKET_DATA_ACK) {
                Queue->Add_Delay(Time() - send->FirstTime);
            }
            Queue->UnQueue_Send(0, 0, i, 0, 0, 0);
            i--;
        }
    }

    int count = Queue->SendCount;
    for (i = 0; i < count; i++) {
        send = Queue->Get_Send(i);
        if (send->IsACK) {
            continue;
        }
        unsigned long time = Time();
        if (time - send->LastTime > RetryDelta) {
            Send(send->Buffer, send->BufLen, send->ExtraBuffer, send->ExtraLen,
                this == PacketRouterConnection, send->unknown_20);
            send->LastTime = time;
            if (send->SendCount == 0) {
                send->FirstTime = time;
                if (((CommHeaderType *)send->Buffer)->Code == PACKET_DATA_NOACK) {
                    send->IsACK = 1;
                }
            } else {
                unknown_08++;
            }
            send->SendCount++;
            if (MaxRetries != 0xFFFFFFFF && send->SendCount > MaxRetries) {
                bad_conn = 1;
                send->unknown_bit2 = 1;
            }
            if (Timeout != 0xFFFFFFFF && send->LastTime - send->FirstTime > Timeout) {
                bad_conn = 1;
                send->unknown_bit2 = 1;
            }
        }
    }

    if (bad_conn) {
        Debug_Printf("ConnectionClass::Service_Send_Queue bad connection\n");
        return 0;
    }
    return 1;
}


/// <summary>
/// Handles a connection that has failed its service pass.
/// This routine is the do-nothing default for connections that cannot clog. The
/// connection manager calls it when Service reports failure, giving a derived
/// connection the chance to throw away whatever is blocking its queues.
/// </summary>
/// <returns>Returns with the number of entries discarded, none at this level.</returns>
// (0x00481550, slot 6)
// Always 0 here (IPXGlobalConnClass overrides it).

int ConnectionClass::Discard_Undeliverable_Packets()
{
    return 0;
}


/***************************************************************************
 * ConnectionClass::Service_Receive_Queue -- services receive queue        *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      1 = OK, 0 = error                                                  *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
// (0x00481560, slot 8)
// Drops packets already read that are unsequenced or older than the
// in-order sequence.

int ConnectionClass::Service_Receive_Queue()
{
    for (int i = 0; i < Queue->ReceiveCount; i++) {
        ReceiveQueueType *rec = Queue->Get_Receive(i);
        if (rec->IsRead) {
            if (((CommHeaderType *)rec->Buffer)->Code == PACKET_DATA_NOACK
                || ((CommHeaderType *)rec->Buffer)->PacketID < LastSeqID) {
                Queue->UnQueue_Receive(0, 0, i, 0, 0);
                i--;
            }
        }
    }
    return 1;
}


/***************************************************************************
 * ConnectionClass::Time -- gets current time                              *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
// (0x004815C0)
// The network tick time.

unsigned long ConnectionClass::Time()
{
    return TickCount.Time();
}


/***************************************************************************
 * ConnectionClass::Command_Name -- returns name for given packet command  *
 *                                                                         *
 * INPUT:                                                                  *
 *      command      packet Command value to get name for                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      ptr to command name, NULL if invalid                               *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/31/1995 BRR : Created.                                             *
 *=========================================================================*/
// (0x004815F0)
// The name of a packet code, or null.

char *__fastcall ConnectionClass::Command_Name(int command)
{
    if (command >= 0 && command < PACKET_COUNT) {
        return Commands[command];
    }
    return 0;
}


// void *ConnectionClass::`scalar deleting destructor'(unsigned int flags) (0x00481610)
// Compiler-generated with the vtable (the destructor inlined). No source of its own.
