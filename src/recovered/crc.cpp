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
// Function comment blocks from mischa85/RedAlert2 392e904 src/crc.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// crc.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/crc.json carries the addresses and the rows that
// stay in their own files.


#include "shared/ini_class.hpp"
#include "shared/wwcrc_engine.hpp"
#include "shared/crc_memory.hpp"


/***********************************************************************************************
 * CRCEngine::operator() -- Submits one byte of data to the CRC engine.                        *
 *                                                                                             *
 *    This routine will take the specified byte of data and submit it to the CRC engine        *
 *    for processing. This routine is designed to be as fast as possible since the typical     *
 *    use of this routine is to feed one of presumably many byte sized chunks of data to the   *
 *    CRC engine.                                                                              *
 *                                                                                             *
 * INPUT:   datum -- One byte of data to submit to the CRC engine.                             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   If possible, use the buffer/size operator to submit data rather than repeated   *
 *             calls to this routine.                                                          *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/02/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void CRCEngine::operator()(char value)
{
    if(Index==0) StagingBuffer.Composite=0;
    StagingBuffer.Buffer[Index++]=value;
    if(Index==4) {
        CRC=Value();
        Index=0;
    }
}


/// <summary>
/// Submits one boolean datum to the CRC engine.
/// This is the boolean flavor of the byte sized submission operator.
/// </summary>
void CRCEngine::operator()(bool value)
{
    if(Index==0) StagingBuffer.Composite=0;
    StagingBuffer.Buffer[Index++]=value ? 1 : 0;
    if(Index==4) {
        CRC=Value();
        Index=0;
    }
}


void WWCRCEngine::operator()(short datum) { short value[1]; value[0] = datum; (*this)(value, sizeof(value)); }


void WWCRCEngine::operator()(long datum) { long value = datum; (*this)(&value, sizeof(value)); }


void WWCRCEngine::operator()(float datum) { float value = datum; (*this)(&value, sizeof(value)); }


void WWCRCEngine::operator()(double datum) { double value = datum; (*this)(&value, sizeof(value)); }


extern "C" unsigned int __cdecl strlen(char const *string);
/// <summary>
/// Submits one word sized datum to the CRC engine.
/// </summary>
void WWCRCEngine::operator()(char const *string)
{
    if (string) (*this)(string, strlen(string));
}


// CRCEngine memory append, original 00493FC0: full 457-byte body plus 7 frozen padding bytes.
inline void Consume_Partial_CRC_Input(CRCEngine &engine,void const *&cursor,int &remaining)
{
    engine.Append_Partial_Byte(*(char const *)cursor);
    cursor=(char const *)cursor+1;
    --remaining;
}
/// <summary>
/// Submits one long word sized datum to the CRC engine.
/// </summary>
long CRCEngine::operator()(void const *buffer, int length)
{
    if (buffer && length>0) {
        int remaining=length;
        while(remaining>0 && Index && (unsigned int)Index<4) {
            Consume_Partial_CRC_Input(*this,buffer,remaining);
        }
        unsigned int words = (unsigned int)remaining / 4;
        buffer=Consume_Whole_Words(buffer,words,remaining);
        while (remaining > 0) {
            (*this)(*(char const *)buffer);
            buffer = (char const *)buffer + 1;
            --remaining;
        }
    }
    return Value();
}


extern unsigned long const CRC_Table[256];
unsigned long __fastcall CRC_Memory_00494190(void const *buffer, unsigned int length, unsigned long initial)
{
    unsigned char const *data = (unsigned char const *)buffer;
    unsigned long crc = ~initial;
    while (length-- != 0) {
        crc = (crc >> 8) ^ CRC_Table[(crc & 255) ^ *data++];
    }
    return ~crc;
}


extern unsigned long const CRC_Table[256];
unsigned long __fastcall CRC_String_004941d0(char const *string, unsigned long initial)
{
    unsigned long crc = ~initial;
    while (*string != 0) {
        crc = (crc >> 8) ^ CRC_Table[(unsigned char)*string++ ^ (unsigned char)crc];
    }
    return ~crc;
}
