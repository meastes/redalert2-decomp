#ifndef RA2_UNKNOWN_DESCRIPTION_LIST_HPP
#define RA2_UNKNOWN_DESCRIPTION_LIST_HPP
#include "shared/vector_class.hpp"
#include "shared/ccini_class.hpp"
#ifndef _WCHAR_T_DEFINED
#define _WCHAR_T_DEFINED
typedef unsigned short wchar_t;
#endif
// Cooperative campaign record, 0x44 bytes. The original class name is unresolved.
struct CoopCampEntry {
 int Index;
 wchar_t *Name; // +04
 signed char MapCount;
 char unknown_09[3];
 char *Maps; // MapCount records of 0x30C bytes, each three 260-byte strings.
 DynamicVectorClass<char *> Players;
 DynamicVectorClass<char *> Enemies;
 char *AIName;
};
class CoopCampListClass {
public:
 CoopCampListClass();
 ~CoopCampListClass();
#ifdef DESCRIPTION_LIST_INLINE_GET_RECORD
 // The actual recovered00490C50 operation, shared by the campaign sender.
 bool Get_Record(int index, CoopCampEntry **record)
 {
     if(index>=0 && index<Count) {
         *record=&Entries[index];
         return true;
     }
     *record=0;
     return false;
 }
#else
 bool Get_Record(int index, CoopCampEntry **record);
#endif
 bool Get_Count(int *count) const;
 bool Copy_Names(DynamicVectorClass<wchar_t *> &names);
 CCINIClass Ini; // +0, composition hypothesis: canonical sizeof(CCINIClass)==0x50
 CoopCampEntry *Entries; // +50
 int Count; // +54
 bool Initialized; // +58
};
#endif


typedef char DescriptionRecord_size_check[(sizeof(CoopCampEntry)==0x44)?1:-1];

typedef char Description_INI_prefix_size_check[(sizeof(CCINIClass)==0x50)?1:-1];
