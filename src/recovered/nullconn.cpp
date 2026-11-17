/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 * Copyright 2026 ra2-decomp contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/
// Function comment blocks from mischa85/RedAlert2 392e904 src/nullconn.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// nullconn.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/nullconn.json carries the addresses and the rows that
// stay in their own files.


#include "shared/null_modem.hpp"
#include "shared/crt_memory.hpp"


/***************************************************************************
 * NullModemConnClass::NullModemConnClass -- class constructor             *
 *                                                                         *
 * INPUT:                                                                  *
 *      numsend         desired # send queue entries                       *
 *      numreceive      desired # send receive entries                     *
 *      maxlen         max length of application's packets                 *
 *      magicnum         application-defined magic # for the packets       *
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
// (0x005CDB40)
// A serial (null-modem) connection: ConnectionClass with a 60-tick retry
// delta, unlimited retries and a 1200-tick timeout; no port yet (-1) and a
// send buffer of MaxPacketLen + 11 bytes (header and checksum). Class name
// from RTTI (vtable 0x007A73C8); member names follow Westwood's
// NullModemConnClass (hypotheses). Bound by linear sweep to the ret 0x10.

NullModemConnClass::NullModemConnClass(int numsend, int numreceive, int maxlen, unsigned short magicnum) :
    ConnectionClass(numsend, numreceive, maxlen, magicnum, 60, (unsigned long)-1, 1200)
{
    PortHandle = -1;
    SendBuf = new char[MaxPacketLen + 11];
}


/***************************************************************************
 * NullModemConnClass::~NullModemConnClass -- class destructor             *
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
// (0x005CDB90)
// Frees the send buffer (the base destructor follows).

NullModemConnClass::~NullModemConnClass()
{
    delete SendBuf;
}


/***************************************************************************
 * NullModemConnClass::Init -- hardware-dependent initialization           *
 *                                                                         *
 * INPUT:                                                                  *
 *      port      GreenLeaf port handle                                    *
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
// (0x005CDBB0)
// The base initialization, then the serial port handle (+0x4C).

void NullModemConnClass::Init(int port)
{
    ConnectionClass::Init();
    PortHandle = port;
}


/***************************************************************************
 * NullModemConnClass::Send -- hardware-dependent packet sending           *
 *                                                                         *
 * INPUT:                                                                  *
 *      buf         buffer to send                                         *
 *      buflen      length of buffer to send                               *
 *      extrabuf      (not used by this class)                             *
 *      extralen      (not used by this class)                             *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      1 = OK, 0 = error                                                  *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
// (0x005CDBD0)
// Slot 9: frames the packet for the serial line (magic 0xDABD, length,
// magic 0xDEAF, the data, a rotating checksum of the data and a carriage
// return) in SendBuf and writes it through the serial port object (SerialPort,
// 0x00B25BB0; Write_To_Serial_Port 0x00734C70). Nothing is sent without an
// open port. The extra buffer and flags are unused here.

int NullModemConnClass::Send(char *buf, int buflen, void *extrabuf, int extralen, bool flagged, unsigned short id)
{
    if (PortHandle == -1) {
        return 0;
    }
    SerialHeaderType *header = (SerialHeaderType *)SendBuf;
    header->MagicNumber = 0xdabd;
    header->Length = (unsigned short)buflen;
    header->MagicNumber2 = 0xdeaf;
    unsigned long sendlen = sizeof(SerialHeaderType);
    memcpy(SendBuf + sendlen, buf, buflen);
    sendlen += buflen;
    int *ibuf = (int *)(SendBuf + sendlen);
    *ibuf = Compute_CRC(buf, buflen);
    sendlen += sizeof(int);
    *(SendBuf + sendlen) = '\r';
    sendlen += 1;
    SerialPort->Write_To_Serial_Port((unsigned char *)SendBuf, sendlen);
    return 1;
}


/***************************************************************************
 * NullModemConnClass::Compute_CRC -- computes CRC for given buffer        *
 *                                                                         *
 * INPUT:                                                                  *
 *      buf      buffer to compute CRC for                                 *
 *      buflen   length of buffer in bytes                                 *
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
// (0x005CDC80)
// The rotating checksum of the data: each step doubles the sum, adds back
// its top bit and adds the next byte. Send expands it inline.

int __fastcall NullModemConnClass::Compute_CRC(char *buf, int buflen)
{
    unsigned int sum = 0;
    unsigned int hibit;
    for (int index = 0; index < buflen; index++) {
        if (sum & 0x80000000) {
            hibit = 1;
        } else {
            hibit = 0;
        }
        sum <<= 1;
        sum += hibit;
        sum += (unsigned char)buf[index];
    }
    return (int)sum;
}


/***************************************************************************
 * NullModemConnClass::Packet_Overhead_Size -- number of extra bytes       *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      number of bytes used for communications only.                      *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   04/20/1995 DRD : Created.                                             *
 *=========================================================================*/
// (0x005CDCB0)
// The bytes a packet carries for the serial link only: the frame header and CRC
// (PACKET_SERIAL_OVERHEAD_SIZE) and the queue header (CommHeaderType).

int NullModemConnClass::Packet_Overhead_Size()
{
    return PACKET_SERIAL_OVERHEAD_SIZE + sizeof(CommHeaderType);
}


// void NullModemConnClass::Init() -- 0x005CDCC0
// Slot 1: the base initialization only.

void NullModemConnClass::Init()
{
    ConnectionClass::Init();
}


// NullModemConnClass scalar deleting destructor (0x005CDCD0, ??_G): emitted
// with the vtable by the constructor; the destructor (inline in the class)
// frees the send buffer, then calls ConnectionClass::~ConnectionClass.
