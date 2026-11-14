#ifndef RA2_PACKET_FIELD_HPP
#define RA2_PACKET_FIELD_HPP
// Nonpolymorphic typed packet field;original developer class name unresolved.
// Layout from constructors004BAD50..004BB000 and endian methods004BB080/F0.
class FieldClass {
public:
 FieldClass(char const *name,char value);
 FieldClass(char const *name,unsigned char value);
 FieldClass(char const *name,short value);
 FieldClass(char const *name,unsigned short value);
 FieldClass(char const *name,int value);
 FieldClass(char const *name,unsigned int value);
 FieldClass(char const *name,char const *value);
 FieldClass(char const *name,void const *value,unsigned short length);
 ~FieldClass();
 void Host_To_Network_004BB080();
 void Network_To_Host_004BB0F0();
 char Name[4];
 unsigned short Type; // +04
 unsigned short Length; // +06
 char *Data; // +08,owned byte storage
 FieldClass *Next; // +0c
};
typedef char PacketField_size_check[(sizeof(FieldClass)==16)?1:-1];
#endif
