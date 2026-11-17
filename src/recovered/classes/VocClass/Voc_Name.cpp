// char const *__fastcall Voc_Name(int voc) -- 0x00714A00
// Purpose: the name of a sound effect: "<none>" for an index outside the
// VocClass list (0x00AD0818), "Invalid Voc" when the entry has no sound
// object, otherwise that object's name (0x00405150, a tail call).
// Assumptions: VocClass's first member is the sound object whose name
// 0x00405150 returns; that class and its name are unidentified.
#define VECTOR_IMPLEMENT_ALL
#include "shared/vector_class.hpp"

class UnknownSound_405150 {
public:
    char const *Name();
};

class VocClass {
public:
    UnknownSound_405150 *Sound;     // +0x00
};
extern DynamicVectorClass<VocClass *> Vocs;

char const *__fastcall Voc_Name(int voc)
{
    if (voc >= 0 && voc < Vocs.Count()) {
        UnknownSound_405150 *sound = Vocs[voc]->Sound;
        if (sound != 0) {
            return sound->Name();
        }
        return "Invalid Voc";
    }
    return "<none>";
}
