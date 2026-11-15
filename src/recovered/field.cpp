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
// Function comment blocks from mischa85/RedAlert2 392e904 src/field.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// field.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/field.json carries the addresses and the rows that
// stay in their own files.


#include "shared/packet_field.hpp"
#include "shared/crt_memory.hpp"


extern "C" char *__cdecl strncpy(char *,char const *,unsigned int);
extern "C" unsigned int __cdecl strlen(char const *);
#pragma intrinsic(strlen)
extern "C" unsigned long __stdcall htonl(unsigned long);
extern "C" unsigned short __stdcall htons(unsigned short);
extern "C" unsigned long __stdcall ntohl(unsigned long);
extern "C" unsigned short __stdcall ntohs(unsigned short);
/// <summary>
/// Creates a field that carries a character value.
/// This routine, and its siblings below, are used by PacketClass as it builds up the
/// field list for a packet. The value is copied into a buffer the field owns.
/// </summary>
/// <param name="id">The four character identifier to tag this field with.</param>
FieldClass::FieldClass(char const *name,char value)
{
 strncpy(Name,name,4);
 Type=1;
 Length=1;
 Data=new char[Length];
 memcpy(Data,&value,Length);
 Next=0;
}


/// <summary>
/// Creates a field that carries an unsigned character value.
/// The value is copied into a buffer the field owns. Character sized data needs no
/// swapping, so it survives the trip across the network untouched.
/// </summary>
/// <param name="id">The four character identifier to tag this field with.</param>
FieldClass::FieldClass(char const *name,unsigned char value)
{
 strncpy(Name,name,4);
 Type=2;
 Length=1;
 Data=new char[Length];
 memcpy(Data,&value,Length);
 Next=0;
}


/// <summary>
/// Creates a field that carries a short value.
/// The value is copied into a buffer the field owns, and the recorded data type lets
/// the packet byte swap it when it travels to another machine.
/// </summary>
/// <param name="id">The four character identifier to tag this field with.</param>
FieldClass::FieldClass(char const *name,short value)
{
 strncpy(Name,name,4);
 Type=3;
 Length=2;
 Data=new char[Length];
 memcpy(Data,&value,Length);
 Next=0;
}


/// <summary>
/// Creates a field that carries an unsigned short value.
/// The value is copied into a buffer the field owns, and the recorded data type lets
/// the packet byte swap it when it travels to another machine.
/// </summary>
/// <param name="id">The four character identifier to tag this field with.</param>
FieldClass::FieldClass(char const *name,unsigned short value)
{
 strncpy(Name,name,4);
 Type=4;
 Length=2;
 Data=new char[Length];
 memcpy(Data,&value,Length);
 Next=0;
}


/// <summary>
/// Creates a field that carries a long value.
/// The value is copied into a buffer the field owns, and the recorded data type lets
/// the packet byte swap it when it travels to another machine.
/// </summary>
/// <param name="id">The four character identifier to tag this field with.</param>
FieldClass::FieldClass(char const *name,int value)
{
 strncpy(Name,name,4);
 Type=5;
 Length=4;
 Data=new char[Length];
 memcpy(Data,&value,Length);
 Next=0;
}


/// <summary>
/// Creates a field that carries an unsigned long value.
/// The value is copied into a buffer the field owns, and the recorded data type lets
/// the packet byte swap it when it travels to another machine.
/// </summary>
/// <param name="id">The four character identifier to tag this field with.</param>
FieldClass::FieldClass(char const *name,unsigned int value)
{
 strncpy(Name,name,4);
 Type=6;
 Length=4;
 Data=new char[Length];
 memcpy(Data,&value,Length);
 Next=0;
}


/// <summary>
/// Creates a field that carries a text string.
/// The string is copied into the field along with its terminating null, so the caller
/// is free to discard the original.
/// </summary>
/// <param name="id">The four character identifier to tag this field with.</param>
FieldClass::FieldClass(char const *name,char const *value)
{
 strncpy(Name,name,4);
 Type=7;
 Length=(unsigned short)(strlen(value)+1);
 Data=new char[Length];
 memcpy(Data,value,Length);
 Next=0;
}


/// <summary>
/// Creates a field that carries an arbitrary block of data.
/// Use this routine when the payload is not one of the simple types -- the block is
/// copied verbatim and is not byte swapped when the packet crosses the network.
/// </summary>
/// <param name="id">The four character identifier to tag this field with.</param>
/// <param name="data">Pointer to the block of data to copy into the field.</param>
/// <param name="length">The length of the data block in bytes.</param>
FieldClass::FieldClass(char const *name,void const *value,unsigned short length)
{
 strncpy(Name,name,4);
 Type=20;
 Length=length;
 Data=new char[Length];
 memcpy(Data,value,Length);
 Next=0;
}


/// <summary>
/// Destroys the field and releases its data buffer.
/// The field does not unlink itself, so the packet that owns the field list must detach
/// it before deleting it.
/// </summary>
FieldClass::~FieldClass()
{
 delete [] Data;
 Data=0;
}


void FieldClass::Host_To_Network_004BB080()
{
 switch(Type) {
 case 3:case 4:*(unsigned short *)Data=htons(*(unsigned short *)Data);break;
 case 5:case 6:*(unsigned long *)Data=htonl(*(unsigned long *)Data);break;
 }
 Type=htons(Type);
 Length=htons(Length);
}


void FieldClass::Network_To_Host_004BB0F0()
{
 Length=ntohs(Length);
 Type=ntohs(Type);
 switch(Type) {
 case 3:case 4:*(unsigned short *)Data=ntohs(*(unsigned short *)Data);break;
 case 5:case 6:*(unsigned long *)Data=ntohl(*(unsigned long *)Data);break;
 }
}
