// alphalighting.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/alphalighting.json carries the addresses and the rows that
// stay in their own files.


// Dynamic initializer (0x0041E320) and atexit destructor (0x0041E360) of the
// shared remap list AlphaLightingRemapClass::Array (0x0083C5D8); the exit
// thunk tests and sets a guard bit (VC6 guards the destruction of a static
// data member). The functions of the unit follow in their own drafts.
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/alpha_lighting_remap.hpp"

DynamicVectorClass<AlphaLightingRemapClass *> AlphaLightingRemapClass::Array;


// atexit destructor (0x0041E360, _$E3) emitted by the draft of 0x0041E320; no source of its own.


// AlphaLightingRemapInitClass::AlphaLightingRemapInitClass() (0x0041E3C0)
#include "shared/alpha_table_cache.hpp"

AlphaLightingRemapInitClass::AlphaLightingRemapInitClass()
{
}


// AlphaLightingRemapInitClass::~AlphaLightingRemapInitClass() (0x0041E3D0)

AlphaLightingRemapInitClass::~AlphaLightingRemapInitClass()
{
}


// AlphaTableRows AlphaLightingRemapInitClass::Get(int count) (0x0041E3E0)
// Returns the shared remap table for a light level, building and registering
// it on first use; each call takes a reference.

AlphaTableRows AlphaLightingRemapInitClass::Get(int count)
{
    for (int index = 0; index < AlphaLightingRemapClass::Array.Count(); index++) {
        if (AlphaLightingRemapClass::Array[index]->Level == count) {
            AlphaLightingRemapClass::Array[index]->RefCount++;
            return (AlphaTableRows)AlphaLightingRemapClass::Array[index]->Table;
        }
    }

    AlphaLightingRemapClass *remap = new AlphaLightingRemapClass(count);
    AlphaLightingRemapClass::Array.Add(remap);
    remap->RefCount++;
    return (AlphaTableRows)remap->Table;
}


// void AlphaLightingRemapInitClass::Release(AlphaTableRows table) (0x0041E510)
// Drops a reference to a shared remap table; the last one unregisters and
// frees it.

void AlphaLightingRemapInitClass::Release(AlphaTableRows table)
{
    if (table != 0) {
        AlphaLightingRemapClass *remap = (AlphaLightingRemapClass *)table;
        remap->RefCount--;
        if (remap->RefCount == 0) {
            AlphaLightingRemapClass::Array.Delete(remap);
            delete remap;
        }
    }
}


// AlphaLightingRemapClass::AlphaLightingRemapClass(int level) (0x0041E590)

AlphaLightingRemapClass::AlphaLightingRemapClass(int level)
    : Level(level), RefCount(0)
{
    for (int index = 0; index <= 0xFFFF; index++) {
        int value = (index & 0xFF) * (index >> 8) * (level - 1) / 0x7E02;
        if (value > level - 1) {
            value = level - 1;
        }
        Table[index] = (unsigned short)((unsigned char)value << 8);
    }
}


// 0x0041e600: function not yet identified
//
// Body: 1 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void unknown_empty_0041e600(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0041e600 {

void  unknown_empty_0041e600(void)
{
    return;
}

}  // namespace seed_0041e600


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<AlphaLightingRemapClass *>::Clear (0x0041E610; DynamicVectorClass<AlphaLightingRemapClass *> vtable 0x0079b800 slot 3).
// DynamicVectorClass<AlphaLightingRemapClass *>::ID_pointer (0x0041E640; DynamicVectorClass<AlphaLightingRemapClass *> vtable 0x0079b800 slot 5).
// VectorClass<AlphaLightingRemapClass *>::Fetch (0x0041E660; VectorClass<AlphaLightingRemapClass *> vtable 0x0079b820 slot 6).
// VectorClass<AlphaLightingRemapClass *>::operator equal (0x0041E670; VectorClass<AlphaLightingRemapClass *> vtable 0x0079b820 slot 1).
// VectorClass<AlphaLightingRemapClass *>::Resize (0x0041E6B0; VectorClass<AlphaLightingRemapClass *> vtable 0x0079b820 slot 2).
// VectorClass<AlphaLightingRemapClass *>::Clear (0x0041E760; VectorClass<AlphaLightingRemapClass *> vtable 0x0079b820 slot 3).
// VectorClass<AlphaLightingRemapClass *>::ID_object (0x0041E790; VectorClass<AlphaLightingRemapClass *> vtable 0x0079b820 slot 4).
// VectorClass<AlphaLightingRemapClass *>::ID_pointer (0x0041E7D0; VectorClass<AlphaLightingRemapClass *> vtable 0x0079b820 slot 5).
// DynamicVectorClass<AlphaLightingRemapClass *>::Resize (0x0041E7F0; DynamicVectorClass<AlphaLightingRemapClass *> vtable 0x0079b800 slot 2).
// DynamicVectorClass<AlphaLightingRemapClass *>::ID_object (0x0041E8A0; DynamicVectorClass<AlphaLightingRemapClass *> vtable 0x0079b800 slot 4).


extern "C" unsigned char vtable_41e8d0[]; extern "C" void __cdecl release_buffer_41e8d0(void*); extern "C" void __cdecl release_object_41e8d0(void*);
class Leaf_41e8d0 { public: void* get(unsigned int flags); };
void* Leaf_41e8d0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_41e8d0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_41e8d0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_41e8d0(this); return this; }


extern "C" unsigned char vtable_41e920[]; extern "C" void __cdecl release_buffer_41e920(void*); extern "C" void __cdecl release_object_41e920(void*);
class Leaf_41e920 { public: void* get(unsigned int flags); };
void* Leaf_41e920::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_41e920; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_41e920((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_41e920(this); return this; }
