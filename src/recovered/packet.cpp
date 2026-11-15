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
// Function comment blocks from mischa85/RedAlert2 392e904 src/packet.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.
// Local variable names from mischa85/RedAlert2 392e904 src/packet.cpp where both functions declare the same local types (names only).

// packet.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/packet.json carries the addresses and the rows that
// stay in their own files.


#define RA2_PACKET_UNIT
#include "shared/packet.hpp"
#include "shared/winsock_interface.hpp"
#include "shared/crt_memory.hpp"


/**************************************************************************
 * PACKETCLASS::~PACKETCLASS -- destroys a packet class be freeing list   *
 *                                                                        *
 * INPUT:      none                                                       *
 *                                                                        *
 * OUTPUT:     none                                                       *
 *                                                                        *
 * HISTORY:                                                               *
 *   04/24/1996 PWG : Created.                                            *
 *========================================================================*/
// (0x00602A60)
// Packet.cpp (0x00602A60-0x00602FD0; Tiberian Sun packet.cpp bodies). The field type is
// shared/packet_field.hpp's FieldClass; Find_Field and five Get_Field overloads are declared for
// this unit only (RA2_PACKET_UNIT).

PacketClass::~PacketClass()
{
    FieldClass *current = Head;
    while (current != 0) {
        FieldClass *next = current->Next;
        delete current;
        current = next;
    }
}


/**************************************************************************
 * PACKETCLASS::ADD_FIELD -- Adds a FieldClass entry to head of packet li *
 *                                                                        *
 * INPUT:      FieldClass * - a properly constructed field class entry.   *
 *                                                                        *
 * OUTPUT:     none                                                       *
 *                                                                        *
 * HISTORY:                                                               *
 *   04/24/1996 PWG : Created.                                            *
 *========================================================================*/
// (0x00602A90)

void PacketClass::Add_Field(FieldClass *field)
{
    field->Next = Head;
    Head = field;
}


/**************************************************************************
 * PACKETCLASS::PACKETCLASS -- Creates a Packet object from a COMMS packe *
 *                                                                        *
 * INPUT:                                                                 *
 *                                                                        *
 * OUTPUT:                                                                *
 *                                                                        *
 * WARNINGS:                                                              *
 *                                                                        *
 * HISTORY:                                                               *
 *   04/22/1996 PWG : Created.                                            *
 *========================================================================*/
// (0x00602AA0)

PacketClass::PacketClass(char *curbuf)
{
    Size = *(unsigned short *)curbuf;
    curbuf += sizeof(unsigned short);
    Size = ntohs(Size);
    ID = *(short *)curbuf;
    curbuf += sizeof(short);
    ID = ntohs(ID);
    Head = 0;

    int remaining_size = Size - 4;
    while (remaining_size > 0) {
        FieldClass *field = (FieldClass *)operator new(sizeof(FieldClass));
        memcpy(field, curbuf, 8);
        curbuf += 8;
        remaining_size -= 8;

        int size = ntohs(field->Length);
        field->Data = new char[size];
        memcpy(field->Data, curbuf, size);
        curbuf += size;
        remaining_size -= size;

        int pad = (4 - (ntohs(field->Length) & 3)) & 3;
        curbuf += pad;
        remaining_size -= pad;

        field->Network_To_Host_004BB0F0();
        Add_Field(field);
    }
}


/**************************************************************************
 * CREATE_COMMS_PACKET -- Walks field list creating a packet              *
 *                                                                        *
 * INPUT:      short - the id of the packet so the server can identify it *
 *               unsigned short & - the size of the packet returned here  *
 *                                                                        *
 * OUTPUT:     void * pointer to the linear packet data                   *
 *                                                                        *
 * WARNINGS:    This routine allocates memory that the user is responsible*
 *              for freeing.                                              *
 *                                                                        *
 * HISTORY:                                                               *
 *   04/22/1996 PWG : Created.                                            *
 *========================================================================*/
// (0x00602BB0)

char *PacketClass::Create_Comms_Packet(int &size)
{
    FieldClass *current;
    size = 4;
    for (current = Head; current; current = current->Next) {
        size += (unsigned short)8;
        size += current->Length;
        size += (4 - (size & 3)) & 3;
    }

    char *retval = new char[size];
    char *curbuf = retval;

    *(unsigned short *)curbuf = htons((unsigned short)size);
    curbuf += sizeof(unsigned short);
    *(short *)curbuf = htons(ID);
    curbuf += sizeof(short);

    for (current = Head; current; current = current->Next) {
        current->Host_To_Network_004BB080();
        memcpy(curbuf, current, 8);
        curbuf += 8;
        memcpy(curbuf, current->Data, ntohs(current->Length));
        curbuf += ntohs(current->Length);
        int pad = (4 - (ntohs(current->Length) & 3)) & 3;
        if (pad) {
            memset(curbuf, 0, pad);
            curbuf += pad;
        }
        current->Network_To_Host_004BB0F0();
    }
    return retval;
}


/**************************************************************************
 * PACKETCLASS::FIND_FIELD -- Finds a field if it exists in the packets   *
 *                                                                        *
 * INPUT:      char *  - the id of the field we are looking for.          *
 *                                                                        *
 * OUTPUT:     FieldClass * pointer to the field class                    *
 *                                                                        *
 * HISTORY:                                                               *
 *   04/23/1996 PWG : Created.                                            *
 *========================================================================*/
// (0x00602CD0; was UnknownChunkList_602cd0::Find)
extern "C" int __cdecl strncmp(char const *left, char const *right, unsigned int count);

FieldClass *PacketClass::Find_Field(char const *id)
{
    for (FieldClass *current = Head; current; current = current->Next) {
        if (strncmp(id, current->Name, 4) == 0) {
            return current;
        }
    }
    return 0;
}


/**************************************************************************
 * GET_FIELD -- Find specified name and returns data                      *
 *                                                                        *
 * INPUT:      char *   - the id of the field that holds the data.        *
 *               char &   - the reference to store the data into          *
 *                                                                        *
 * OUTPUT:      true if the field was found, false if it was not.         *
 *                                                                        *
 * WARNINGS:   The data reference is not changed if the field is not      *
 *               found.                                                   *
 *                                                                        *
 * HISTORY:                                                               *
 *   04/23/1996 PWG : Created.                                            *
 *========================================================================*/
// (0x00602D10)

bool PacketClass::Get_Field(char const *id, char &data)
{
    FieldClass *field = Find_Field(id);
    if (field) {
        data = *((char *)field->Data);
    }
    return (field) ? true : false;
}


/**************************************************************************
 * GET_FIELD -- Find specified name and returns data                      *
 *                                                                        *
 * INPUT:      char *   - the id of the field that holds the data.        *
 *               unsigned char &   - the reference to store the data into *
 *                                                                        *
 * OUTPUT:      true if the field was found, false if it was not.         *
 *                                                                        *
 * WARNINGS:   The data reference is not changed if the field is not      *
 *               found.                                                   *
 *                                                                        *
 * HISTORY:                                                               *
 *   04/23/1996 PWG : Created.                                            *
 *========================================================================*/
// (0x00602D60)

bool PacketClass::Get_Field(char const *id, unsigned char &data)
{
    FieldClass *field = Find_Field(id);
    if (field) {
        data = *((unsigned char *)field->Data);
    }
    return (field) ? true : false;
}


/**************************************************************************
 * GET_FIELD -- Find specified name and returns data                      *
 *                                                                        *
 * INPUT:      char *   - the id of the field that holds the data.        *
 *               short &   - the reference to store the data into         *
 *                                                                        *
 * OUTPUT:      true if the field was found, false if it was not.         *
 *                                                                        *
 * WARNINGS:   The data reference is not changed if the field is not      *
 *               found.                                                   *
 *                                                                        *
 * HISTORY:                                                               *
 *   04/23/1996 PWG : Created.                                            *
 *========================================================================*/
// (0x00602DB0; the inventory row starts at 0x00602DB2)

bool PacketClass::Get_Field(char const *id, short &data)
{
    FieldClass *field = Find_Field(id);
    if (field) {
        data = *((short *)field->Data);
    }
    return (field) ? true : false;
}


/**************************************************************************
 * GET_FIELD -- Find specified name and returns data                      *
 *                                                                        *
 * INPUT:      char *   - the id of the field that holds the data.        *
 *               unsigned short &   - the reference to store the data into*
 *                                                                        *
 * OUTPUT:      true if the field was found, false if it was not.         *
 *                                                                        *
 * WARNINGS:   The data reference is not changed if the field is not      *
 *               found.                                                   *
 *                                                                        *
 * HISTORY:                                                               *
 *   04/23/1996 PWG : Created.                                            *
 *========================================================================*/
// (0x00602E00)

bool PacketClass::Get_Field(char const *id, unsigned short &data)
{
    FieldClass *field = Find_Field(id);
    if (field) {
        data = *((unsigned short *)field->Data);
    }
    return (field) ? true : false;
}


/**************************************************************************
 * GET_FIELD -- Find specified name and returns data                      *
 *                                                                        *
 * INPUT:      char *   - the id of the field that holds the data.        *
 *               long &   - the reference to store the data into          *
 *                                                                        *
 * OUTPUT:      true if the field was found, false if it was not.         *
 *                                                                        *
 * WARNINGS:   The data reference is not changed if the field is not      *
 *               found.                                                   *
 *                                                                        *
 * HISTORY:                                                               *
 *   04/23/1996 PWG : Created.                                            *
 *========================================================================*/
// (0x00602E50)

bool PacketClass::Get_Field(char const *id, int &data)
{
    FieldClass *field = Find_Field(id);
    if (field) {
        data = *((int *)field->Data);
    }
    return (field) ? true : false;
}


/**************************************************************************
 * GET_FIELD -- Find specified name and returns data as a string          *
 *                                                                        *
 * INPUT:      char *   - the id of the field that holds the data.        *
 *               char *   - the string to store the data into             *
 *                                                                        *
 * OUTPUT:      true if the field was found, false if it was not.         *
 *                                                                        *
 * WARNINGS:   The string is not changed if the field is not found.  It   *
 *               is assumed that the string variabled specified by the    *
 *               pointer is large enough to hold the data.                *
 *                                                                        *
 * HISTORY:                                                               *
 *   04/23/1996 PWG : Created.                                            *
 *========================================================================*/
// (0x00602EA0; strcpy)

bool PacketClass::Get_Field(char const *id, char *data)
{
    FieldClass *field = Find_Field(id);
    if (field) {
        strcpy(data, (char *)field->Data);
    }
    return (field) ? true : false;
}


/**************************************************************************
 * GET_FIELD -- Find specified name and returns data                      *
 *                                                                        *
 * INPUT:      char *   - the id of the field that holds the data.        *
 *               unsigned long &   - the reference to store the data into *
 *                                                                        *
 * OUTPUT:      true if the field was found, false if it was not.         *
 *                                                                        *
 * WARNINGS:   The data reference is not changed if the field is not      *
 *               found.                                                   *
 *                                                                        *
 * HISTORY:                                                               *
 *   04/23/1996 PWG : Created.                                            *
 *========================================================================*/
// (0x00602F10)

bool PacketClass::Get_Field(char const *id, unsigned int &data)
{
    FieldClass *field = Find_Field(id);
    if (field) {
        data = *((unsigned int *)field->Data);
    }
    return (field) ? true : false;
}


/**************************************************************************
 * GET_FIELD -- Find specified name and returns data                      *
 *                                                                        *
 * INPUT:      char *   - the id of the field that holds the data.        *
 *            void * - the reference to store the data into               *
 *           int    - the length of the buffer passed in                  *
 *                                                                        *
 * OUTPUT:      true if the field was found, false if it was not.         *
 *                                                                        *
 * WARNINGS:   The data reference is not changed if the field is not      *
 *               found.                                                   *
 *                                                                        *
 * HISTORY:                                                               *
 *   6/4/96 4:46PM ST : Created                                           *
 *========================================================================*/
// (0x00602F60; copies at most length bytes, returns the field size in length)

bool PacketClass::Get_Field(char const *id, void *data, int &length)
{
    FieldClass *field = Find_Field(id);
    if (field) {
        memcpy(data, field->Data, field->Length < length ? field->Length : length);
        length = (int)field->Length;
    }
    return (field) ? true : false;
}
