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
// Function comment blocks from mischa85/RedAlert2 392e904 src/combuf.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.
// Parameter names from mischa85/RedAlert2 392e904 src/combuf.cpp where both define the function (names only).
// Local variable names from mischa85/RedAlert2 392e904 src/combuf.cpp where both functions declare the same local types (names only).

// combuf.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/combuf.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/comm_buffer.hpp"


// Header-static dynamic initializer of Combuf.cpp (0x004800A0): the
// shape_file.hpp empty rect.


/***************************************************************************
 * CommBufferClass::CommBufferClass -- class constructor                   *
 *                                                                         *
 * INPUT:                                                                  *
 *      numsend      # queue entries for sending                           *
 *      numreceive   # queue entries for receiving                         *
 *      maxlen      maximum desired packet length, in bytes                *
 *      extralen      max size of app-specific extra bytes (optional)      *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/19/1994 BR : Created.                                              *
 *=========================================================================*/
// (0x004800C0)
// Allocates both queues, their index tables and a packet (and extra) buffer
// per entry, then clears everything.

CommBufferClass::CommBufferClass(int numsend, int numreceive, int maxlen, int extralen)
{
    MaxSend = numsend;
    MaxReceive = numreceive;
    MaxPacketSize = maxlen;
    MaxExtraSize = extralen;
    SendQueue = new SendQueueType[numsend];
    ReceiveQueue = new ReceiveQueueType[numreceive];
    SendIndex = new int[numsend];
    ReceiveIndex = new int[numreceive];
    int index;
    for (index = 0; index < MaxSend; index++) {
        SendQueue[index].Buffer = new char[maxlen];
        if (MaxExtraSize > 0) {
            SendQueue[index].ExtraBuffer = new char[MaxExtraSize];
        } else {
            SendQueue[index].ExtraBuffer = 0;
        }
    }
    for (index = 0; index < MaxReceive; index++) {
        ReceiveQueue[index].Buffer = new char[maxlen];
        if (MaxExtraSize > 0) {
            ReceiveQueue[index].ExtraBuffer = new char[MaxExtraSize];
        } else {
            ReceiveQueue[index].ExtraBuffer = 0;
        }
    }
    Init();
}


/***************************************************************************
 * CommBufferClass::~CommBufferClass -- class destructor                   *
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
 *   12/19/1994 BR : Created.                                              *
 *=========================================================================*/
// (0x004801D0)
// Frees every entry's buffers, both queues and their index tables.

CommBufferClass::~CommBufferClass()
{
    int index;
    for (index = 0; index < MaxSend; index++) {
        delete[] SendQueue[index].Buffer;
        if (SendQueue[index].ExtraBuffer) {
            delete[] SendQueue[index].ExtraBuffer;
        }
    }
    for (index = 0; index < MaxReceive; index++) {
        delete[] ReceiveQueue[index].Buffer;
        if (ReceiveQueue[index].ExtraBuffer) {
            delete[] ReceiveQueue[index].ExtraBuffer;
        }
    }
    delete[] SendQueue;
    delete[] ReceiveQueue;
    delete[] SendIndex;
    delete[] ReceiveIndex;
}


/***************************************************************************
 * CommBufferClass::Init -- initializes this queue                         *
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
 *   01/20/1995 BR : Created.                                              *
 *=========================================================================*/
// (0x00480280)
// Clears the counters, the delay statistics, every queue entry and index,
// and the debug configuration.

void CommBufferClass::Init()
{
    SendTotal = 0;
    ReceiveTotal = 0;
    DelaySum = 0;
    NumDelay = 0;
    MeanDelay = 0;
    MaxDelay = 0;
    SendCount = 0;
    ReceiveCount = 0;
    int index;
    for (index = 0; index < MaxSend; index++) {
        SendQueue[index].IsActive = 0;
        SendQueue[index].IsACK = 0;
        SendQueue[index].unknown_bit2 = 0;
        SendQueue[index].FirstTime = 0;
        SendQueue[index].LastTime = 0;
        SendQueue[index].SendCount = 0;
        SendQueue[index].BufLen = 0;
        SendQueue[index].ExtraLen = 0;
        SendQueue[index].unknown_20 = 0;
        SendIndex[index] = 0;
    }
    for (index = 0; index < MaxReceive; index++) {
        ReceiveQueue[index].IsActive = 0;
        ReceiveQueue[index].IsRead = 0;
        ReceiveQueue[index].IsACK = 0;
        ReceiveQueue[index].ReceiveTime = 0;
        ReceiveQueue[index].BufLen = 0;
        ReceiveQueue[index].ExtraLen = 0;
        ReceiveIndex[index] = 0;
    }
    DebugOffset = 0;
    DebugSize = 0;
    DebugNames = 0;
    DebugNameCount = 0;
}


/***************************************************************************
 * CommBufferClass::Init_Send_Queue -- Clears the send queue               *
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
 *   10/23/1995 BRR : Created.                                             *
 *=========================================================================*/
// (0x00480370)
// Empties the send queue.

void CommBufferClass::Init_Send_Queue()
{
    SendCount = 0;
    for (int index = 0; index < MaxSend; index++) {
        SendQueue[index].IsActive = 0;
        SendQueue[index].IsACK = 0;
        SendQueue[index].unknown_bit2 = 0;
        SendQueue[index].FirstTime = 0;
        SendQueue[index].LastTime = 0;
        SendQueue[index].SendCount = 0;
        SendQueue[index].BufLen = 0;
        SendQueue[index].ExtraLen = 0;
        SendQueue[index].unknown_20 = 0;
        SendIndex[index] = 0;
    }
}


/***************************************************************************
 * CommBufferClass::Queue_Send -- queues a message for sending             *
 *                                                                         *
 * INPUT:                                                                  *
 *      buf         buffer containing the message                          *
 *      buflen      length of 'buf'                                        *
 *      extrabuf      buffer containing extra data (optional)              *
 *      extralen      length of extra data (optional)                      *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      1 = OK, 0 = no room in the queue                                   *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
// (0x004803F0)
// Copies a packet (and its extra data) into a free send entry and appends
// it to the send order; 0 when full or too long.

int CommBufferClass::Queue_Send(void *buf, int buflen, void *extrabuf, int extralen, unsigned short id)
{
    if (SendCount == MaxSend || buflen > MaxPacketSize) {
        if (SendCount == MaxSend) {
            Debug_Printf("Queue_Send - No room in send queue\n");
        } else {
            Debug_Printf("buflen > MaxPacketSize by %d bytes\n", MaxPacketSize - buflen);
        }
        return 0;
    }
    int index = -1;
    for (int i = 0; i < MaxSend; i++) {
        if (SendQueue[i].IsActive == 0) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        return 0;
    }
    SendQueue[index].IsActive = 1;
    SendQueue[index].IsACK = 0;
    SendQueue[index].unknown_bit2 = 0;
    SendQueue[index].FirstTime = 0;
    SendQueue[index].LastTime = 0;
    SendQueue[index].SendCount = 0;
    SendQueue[index].BufLen = buflen;
    SendQueue[index].unknown_20 = id;
    memcpy(SendQueue[index].Buffer, buf, buflen);
    if (extrabuf != 0 && extralen > 0 && extralen <= MaxExtraSize) {
        memcpy(SendQueue[index].ExtraBuffer, extrabuf, extralen);
        SendQueue[index].ExtraLen = extralen;
    } else {
        SendQueue[index].ExtraLen = 0;
    }
    SendIndex[SendCount] = index;
    SendCount++;
    SendTotal++;
    return 1;
}


/***************************************************************************
 * CommBufferClass::UnQueue_Send -- removes next entry from send queue     *
 *                                                                         *
 * Frees the given entry; the index given by the caller is the "active"    *
 * index value (ie the "nth" active entry), not the actual index in the    *
 * array.                                                                  *
 *                                                                         *
 * INPUT:                                                                  *
 *      buf         buffer to store entry's data in; if NULL, it's discarded*
 *      buflen      filled in with length of entry retrieved               *
 *      index         "index" of entry to un-queue                         *
 *      extrabuf      buffer for extra data (optional)                     *
 *      extralen      ptr to length of extra data (optional)               *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      1 = OK, 0 = no entry to retrieve                                   *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
// (0x00480550)
// Copies out (when asked) and frees the index'th packet in send order,
// closing the gap in the order; 0 when there is none.

int CommBufferClass::UnQueue_Send(void *buf, int *buflen, int index, void *extrabuf, int *extralen, unsigned short *id)
{
    if (SendCount == 0 || SendQueue[SendIndex[index]].IsActive == 0) {
        return 0;
    }
    if (buf != 0) {
        memcpy(buf, SendQueue[SendIndex[index]].Buffer, SendQueue[SendIndex[index]].BufLen);
        *buflen = SendQueue[SendIndex[index]].BufLen;
    }
    if (extrabuf != 0 && extralen != 0) {
        memcpy(extrabuf, SendQueue[SendIndex[index]].ExtraBuffer, SendQueue[SendIndex[index]].ExtraLen);
        *extralen = SendQueue[SendIndex[index]].ExtraLen;
    }
    if (id != 0) {
        *id = SendQueue[SendIndex[index]].unknown_20;
    }
    SendQueue[SendIndex[index]].IsActive = 0;
    SendQueue[SendIndex[index]].IsACK = 0;
    SendQueue[SendIndex[index]].unknown_bit2 = 0;
    SendQueue[SendIndex[index]].FirstTime = 0;
    SendQueue[SendIndex[index]].LastTime = 0;
    SendQueue[SendIndex[index]].SendCount = 0;
    SendQueue[SendIndex[index]].BufLen = 0;
    SendQueue[SendIndex[index]].ExtraLen = 0;
    SendQueue[SendIndex[index]].unknown_20 = 0;
    for (int i = index; i < SendCount - 1; i++) {
        SendIndex[i] = SendIndex[i + 1];
    }
    SendIndex[SendCount - 1] = 0;
    SendCount--;
    return 1;
}


/***************************************************************************
 * CommBufferClass::Get_Send -- gets ptr to queue entry                    *
 *                                                                         *
 * This routine gets a pointer to the indicated queue entry.  The index    *
 * value is relative to the next-accessible queue entry; 0 = get the       *
 * next available queue entry, 1 = get the one behind that, etc.           *
 *                                                                         *
 * INPUT:                                                                  *
 *      index      index of entry to get (0 = 1st available)               *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      ptr to entry                                                       *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/21/1994 BR : Created.                                              *
 *=========================================================================*/
// (0x00480700)
// The index'th packet in send order, or null when that entry is free.

SendQueueType *CommBufferClass::Get_Send(int index)
{
    if (SendQueue[SendIndex[index]].IsActive == 0) {
        return 0;
    }
    return &SendQueue[SendIndex[index]];
}


/***************************************************************************
 * CommBufferClass::Queue_Receive -- queues a received message             *
 *                                                                         *
 * INPUT:                                                                  *
 *      buf         buffer containing the message                          *
 *      buflen      length of 'buf'                                        *
 *      extrabuf      buffer containing extra data (optional)              *
 *      extralen      length of extra data (optional)                      *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      1 = OK, 0 = no room in the queue                                   *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
// (0x00480730)
// Copies an incoming packet (and its extra data) into a free receive entry
// stamped with the tick time and appends it to the receive order.

int CommBufferClass::Queue_Receive(void *buf, int buflen, void *extrabuf, int extralen)
{
    if (ReceiveCount == MaxReceive || buflen > MaxPacketSize) {
        return 0;
    }
    int index = -1;
    for (int i = 0; i < MaxReceive; i++) {
        if (ReceiveQueue[i].IsActive == 0) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        Debug_Printf("Can't queue incoming packet - Unable to find free slot in array.\n");
        return 0;
    }
    ReceiveQueue[index].IsActive = 1;
    ReceiveQueue[index].IsRead = 0;
    ReceiveQueue[index].IsACK = 0;
    ReceiveQueue[index].ReceiveTime = TickCount.Time();
    ReceiveQueue[index].BufLen = buflen;
    memcpy(ReceiveQueue[index].Buffer, buf, buflen);
    if (extrabuf != 0 && extralen > 0 && extralen <= MaxExtraSize) {
        memcpy(ReceiveQueue[index].ExtraBuffer, extrabuf, extralen);
        ReceiveQueue[index].ExtraLen = extralen;
    } else {
        ReceiveQueue[index].ExtraLen = 0;
    }
    ReceiveIndex[ReceiveCount] = index;
    ReceiveCount++;
    ReceiveTotal++;
    return 1;
}


/***************************************************************************
 * CommBufferClass::UnQueue_Receive -- removes next entry from send queue  *
 *                                                                         *
 * Frees the given entry; the index given by the caller is the "active"    *
 * index value (ie the "nth" active entry), not the actual index in the    *
 * array.                                                                  *
 *                                                                         *
 * INPUT:                                                                  *
 *      buf         buffer to store entry's data in; if NULL, it's discarded*
 *      buflen      filled in with length of entry retrieved               *
 *      index         index of entry to un-queue                           *
 *      extrabuf      buffer for extra data (optional)                     *
 *      extralen      ptr to length of extra data (optional)               *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      1 = OK, 0 = no entry to retrieve                                   *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
// (0x00480870)
// Copies out (when asked) and frees the index'th packet in receive order,
// closing the gap in the order; 0 when there is none.

int CommBufferClass::UnQueue_Receive(void *buf, int *buflen, int index, void *extrabuf, int *extralen)
{
    if (ReceiveCount == 0 || ReceiveQueue[ReceiveIndex[index]].IsActive == 0) {
        return 0;
    }
    if (buf != 0) {
        memcpy(buf, ReceiveQueue[ReceiveIndex[index]].Buffer, ReceiveQueue[ReceiveIndex[index]].BufLen);
        *buflen = ReceiveQueue[ReceiveIndex[index]].BufLen;
    }
    if (extrabuf != 0 && extralen != 0) {
        memcpy(extrabuf, ReceiveQueue[ReceiveIndex[index]].ExtraBuffer, ReceiveQueue[ReceiveIndex[index]].ExtraLen);
        *extralen = ReceiveQueue[ReceiveIndex[index]].ExtraLen;
    }
    ReceiveQueue[ReceiveIndex[index]].IsActive = 0;
    ReceiveQueue[ReceiveIndex[index]].IsRead = 0;
    ReceiveQueue[ReceiveIndex[index]].IsACK = 0;
    ReceiveQueue[ReceiveIndex[index]].BufLen = 0;
    ReceiveQueue[ReceiveIndex[index]].ExtraLen = 0;
    for (int i = index; i < ReceiveCount - 1; i++) {
        ReceiveIndex[i] = ReceiveIndex[i + 1];
    }
    ReceiveIndex[ReceiveCount - 1] = 0;
    ReceiveCount--;
    return 1;
}


/***************************************************************************
 * CommBufferClass::Get_Receive -- gets ptr to queue entry                 *
 *                                                                         *
 * This routine gets a pointer to the indicated queue entry.  The index    *
 * value is relative to the next-accessible queue entry; 0 = get the       *
 * next available queue entry, 1 = get the one behind that, etc.           *
 *                                                                         *
 * INPUT:                                                                  *
 *      index      index of entry to get (0 = 1st available)               *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      ptr to entry                                                       *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/21/1994 BR : Created.                                              *
 *=========================================================================*/
// (0x004809C0)
// The index'th packet in receive order, or null when that entry is free.

ReceiveQueueType *CommBufferClass::Get_Receive(int index)
{
    if (ReceiveQueue[ReceiveIndex[index]].IsActive == 0) {
        return 0;
    }
    return &ReceiveQueue[ReceiveIndex[index]];
}


/***************************************************************************
 * CommBufferClass::Add_Delay -- adds a new delay value for response time  *
 *                                                                         *
 * This routine updates the average response time for this queue.  The     *
 * computation is based on the average of the last 'n' delay values given, *
 * It computes a running total of the last n delay values, then divides    *
 * that by n to compute the average.                                       *
 *                                                                         *
 * When the number of values given exceeds the max, the mean is subtracted *
 * off the total, then the new value is added in.  Thus, any single delay  *
 * value will have an effect on the total that approaches 0 over time, and *
 * the new delay value contributes to 1/n of the mean.                     *
 *                                                                         *
 * INPUT:                                                                  *
 *      delay         value to add into the response time computation      *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/19/1995 BR : Created.                                              *
 *=========================================================================*/
// (0x004809F0)
// Folds a round-trip delay into the running mean (a 256-sample window once
// full, rounded) and the maximum.

void CommBufferClass::Add_Delay(unsigned long delay)
{
    int roundoff = 0;
    if (NumDelay == 256) {
        DelaySum += delay - MeanDelay;
        if ((DelaySum & 0xff) > 127) {
            roundoff = 1;
        }
        MeanDelay = (DelaySum >> 8) + roundoff;
    } else {
        NumDelay++;
        DelaySum += delay;
        MeanDelay = DelaySum / NumDelay;
    }
    if (delay > MaxDelay) {
        MaxDelay = delay;
    }
}


// unsigned long CommBufferClass::Avg_Delay() (0x00480A60)

unsigned long CommBufferClass::Avg_Delay()
{
    return MeanDelay;
}


// unsigned long CommBufferClass::Max_Delay() (0x00480A70)

unsigned long CommBufferClass::Max_Delay()
{
    return MaxDelay;
}


// void CommBufferClass::Reset_Delay(bool clear) (0x00480A80)
// Clears the delay statistics, or restarts them from the current mean.

void CommBufferClass::Reset_Delay(bool clear)
{
    if (clear) {
        DelaySum = 0;
        NumDelay = 0;
        MeanDelay = 0;
        MaxDelay = 0;
    } else {
        NumDelay = 1;
        DelaySum = MeanDelay;
        MaxDelay = MeanDelay;
    }
}


/***************************************************************************
 * CommBufferClass::Configure_Debug -- sets up special debug values        *
 *                                                                         *
 * Mono_Debug_Print2() can look into a packet to pull out a particular     *
 * ID, and can print both that ID and a string corresponding to            *
 * that ID.  This routine configures these values so it can find           *
 * and decode the ID.  This ID is used in addition to the normal           *
 * CommHeaderType values.                                                  *
 *                                                                         *
 * INPUT:                                                                  *
 *      type_offset      ID's byte offset into packet                      *
 *      type_size      size of ID, in bytes; 0 if none                     *
 *      names            ptr to array of names; use ID as an index into this*
 *      maxnames         max # in the names array; 0 if none.              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      Names shouldn't be longer than 12 characters.                      *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/31/1995 BRR : Created.                                             *
 *=========================================================================*/
// (0x00480AB0)

void CommBufferClass::Configure_Debug(int type_offset, int type_size, int names, int namestart, int namecount)
{
    DebugOffset = type_offset;
    DebugSize = type_size;
    DebugNames = names;
    DebugNameStart = namestart;
    DebugNameCount = namecount;
}


// void *CommBufferClass::`scalar deleting destructor'(unsigned int flags) (0x00480AE0)
// Compiler-generated with the vtable (calls the destructor). No source of its own.
