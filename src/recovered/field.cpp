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
FieldClass::FieldClass(char const *name,char value)
{
 strncpy(Name,name,4);
 Type=1;
 Length=1;
 Data=new char[Length];
 memcpy(Data,&value,Length);
 Next=0;
}


FieldClass::FieldClass(char const *name,unsigned char value)
{
 strncpy(Name,name,4);
 Type=2;
 Length=1;
 Data=new char[Length];
 memcpy(Data,&value,Length);
 Next=0;
}


FieldClass::FieldClass(char const *name,short value)
{
 strncpy(Name,name,4);
 Type=3;
 Length=2;
 Data=new char[Length];
 memcpy(Data,&value,Length);
 Next=0;
}


FieldClass::FieldClass(char const *name,unsigned short value)
{
 strncpy(Name,name,4);
 Type=4;
 Length=2;
 Data=new char[Length];
 memcpy(Data,&value,Length);
 Next=0;
}


FieldClass::FieldClass(char const *name,int value)
{
 strncpy(Name,name,4);
 Type=5;
 Length=4;
 Data=new char[Length];
 memcpy(Data,&value,Length);
 Next=0;
}


FieldClass::FieldClass(char const *name,unsigned int value)
{
 strncpy(Name,name,4);
 Type=6;
 Length=4;
 Data=new char[Length];
 memcpy(Data,&value,Length);
 Next=0;
}


FieldClass::FieldClass(char const *name,char const *value)
{
 strncpy(Name,name,4);
 Type=7;
 Length=(unsigned short)(strlen(value)+1);
 Data=new char[Length];
 memcpy(Data,value,Length);
 Next=0;
}


FieldClass::FieldClass(char const *name,void const *value,unsigned short length)
{
 strncpy(Name,name,4);
 Type=20;
 Length=length;
 Data=new char[Length];
 memcpy(Data,value,Length);
 Next=0;
}


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
