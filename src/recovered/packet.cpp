// packet.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/packet.json carries the addresses and the rows that
// stay in their own files.


#define RA2_PACKET_UNIT
#include "shared/packet.hpp"
#include "shared/winsock_interface.hpp"
#include "shared/crt_memory.hpp"


// PacketClass::~PacketClass() (0x00602A60): deletes every field.
// Packet.cpp (0x00602A60-0x00602FD0; Tiberian Sun packet.cpp bodies). The field type is
// shared/packet_field.hpp's FieldClass; Find_Field and five Get_Field overloads are declared for
// this unit only (RA2_PACKET_UNIT).

PacketClass::~PacketClass()
{
    FieldClass *node = Head;
    while (node != 0) {
        FieldClass *next = node->Next;
        delete node;
        node = next;
    }
}


// void PacketClass::Add_Field(PacketField *field) (0x00602A90): pushes the field on the list head.

void PacketClass::Add_Field(FieldClass *field)
{
    field->Next = Head;
    Head = field;
}


// PacketClass::PacketClass(char *curbuf) (0x00602AA0): parses a received packet (size and ID, then each field header, data and pad; Tiberian Sun packet.cpp).

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


// char *PacketClass::Create_Comms_Packet(int &size) (0x00602BB0): builds the network form of the packet (Tiberian Sun packet.cpp, with the size and ID header written).

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


// PacketField *PacketClass::Find_Field(char const *id) (0x00602CD0; was UnknownChunkList_602cd0::Find): the first field whose four-character ID matches.
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


// bool PacketClass::Get_Field(char const *id, char &data) (0x00602D10).

bool PacketClass::Get_Field(char const *id, char &data)
{
    FieldClass *field = Find_Field(id);
    if (field) {
        data = *((char *)field->Data);
    }
    return (field) ? true : false;
}


// bool PacketClass::Get_Field(char const *id, unsigned char &data) (0x00602D60).

bool PacketClass::Get_Field(char const *id, unsigned char &data)
{
    FieldClass *field = Find_Field(id);
    if (field) {
        data = *((unsigned char *)field->Data);
    }
    return (field) ? true : false;
}


// bool PacketClass::Get_Field(char const *id, short &data) (0x00602DB0; the inventory row starts at 0x00602DB2).

bool PacketClass::Get_Field(char const *id, short &data)
{
    FieldClass *field = Find_Field(id);
    if (field) {
        data = *((short *)field->Data);
    }
    return (field) ? true : false;
}


// bool PacketClass::Get_Field(char const *id, unsigned short &data) (0x00602E00).

bool PacketClass::Get_Field(char const *id, unsigned short &data)
{
    FieldClass *field = Find_Field(id);
    if (field) {
        data = *((unsigned short *)field->Data);
    }
    return (field) ? true : false;
}


// bool PacketClass::Get_Field(char const *id, int &data) (0x00602E50).

bool PacketClass::Get_Field(char const *id, int &data)
{
    FieldClass *field = Find_Field(id);
    if (field) {
        data = *((int *)field->Data);
    }
    return (field) ? true : false;
}


// bool PacketClass::Get_Field(char const *id, char *data) (0x00602EA0; strcpy).

bool PacketClass::Get_Field(char const *id, char *data)
{
    FieldClass *field = Find_Field(id);
    if (field) {
        strcpy(data, (char *)field->Data);
    }
    return (field) ? true : false;
}


// bool PacketClass::Get_Field(char const *id, unsigned int &data) (0x00602F10).

bool PacketClass::Get_Field(char const *id, unsigned int &data)
{
    FieldClass *field = Find_Field(id);
    if (field) {
        data = *((unsigned int *)field->Data);
    }
    return (field) ? true : false;
}


// bool PacketClass::Get_Field(char const *id, void *data, int &length) (0x00602F60; copies at most length bytes, returns the field size in length).

bool PacketClass::Get_Field(char const *id, void *data, int &length)
{
    FieldClass *field = Find_Field(id);
    if (field) {
        memcpy(data, field->Data, field->Length < length ? field->Length : length);
        length = (int)field->Length;
    }
    return (field) ? true : false;
}
