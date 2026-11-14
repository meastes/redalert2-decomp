// vector.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/vector.json carries the addresses and the rows that
// stay in their own files.


#include "shared/boolean_vector.hpp"
#include "shared/crt_memory.hpp"


// BooleanVectorClass::BooleanVectorClass(unsigned size, unsigned char *array) (0x0070EFC0).

BooleanVectorClass::BooleanVectorClass(unsigned size, unsigned char *array) :
    BitCount(size),
    Copy(false),
    LastIndex(-1),
    BitArray(0, 0)
{
    BitArray.Resize((size + (8 - 1)) / 8, array);
}


// BooleanVectorClass::BooleanVectorClass(BooleanVectorClass const &) (0x0070F010): assigns through operator=.

BooleanVectorClass::BooleanVectorClass(BooleanVectorClass const &vector)
{
    LastIndex = -1;
    *this = vector;
}


// BooleanVectorClass::operator= (0x0070F0C0): stores the fetched copy back, then copies member-wise.

BooleanVectorClass &BooleanVectorClass::operator=(BooleanVectorClass const &vector)
{
    Fixup();
    Copy = vector.Copy;
    LastIndex = vector.LastIndex;
    BitArray = vector.BitArray;
    BitCount = vector.BitCount;
    return *this;
}


// BooleanVectorClass::operator== (0x0070F170): same count and same packed bytes.

int BooleanVectorClass::operator==(BooleanVectorClass const &vector) const
{
    Fixup(LastIndex);
    return BitCount == vector.BitCount && BitArray == vector.BitArray;
}


// BooleanVectorClass::Resize (0x0070F1E0): new elements start false; zero clears the vector.

int BooleanVectorClass::Resize(unsigned size)
{
    Fixup();

    if (size > 0) {
        unsigned oldsize = BitCount;
        int success = BitArray.Resize((size + (8 - 1)) / 8);
        BitCount = size;
        if (success && oldsize < size) {
            for (unsigned index = oldsize; index < size; index++) {
                (*this)[index] = 0;
            }
        }
        return success;
    }

    Clear();
    return true;
}


// BooleanVectorClass::Clear (0x0070F2C0): empty vector, storage freed.

void BooleanVectorClass::Clear()
{
    Fixup();
    BitCount = 0;
    BitArray.Clear();
}


// BooleanVectorClass::Reset (0x0070F300): every element false.

void BooleanVectorClass::Reset()
{
    LastIndex = -1;
    if (BitArray.Length() > 0) {
        memset(&BitArray[0], '\0', BitArray.Length());
    }
}


// BooleanVectorClass::Set (0x0070F330): every element true.

void BooleanVectorClass::Set()
{
    LastIndex = -1;
    if (BitArray.Length() > 0) {
        memset(&BitArray[0], '\xFF', BitArray.Length());
    }
}


// BooleanVectorClass::Fixup (0x0070F360): writes Copy back to bit LastIndex and fetches bit index
// (an out-of-range index only flushes).

void BooleanVectorClass::Fixup(int index) const
{
    if ((unsigned)index >= BitCount) {
        index = -1;
    }

    if (index != LastIndex) {
        if (LastIndex != -1) {
            Set_Bit((void *)&BitArray[0], LastIndex, Copy);
        }
        if (index != -1) {
            ((unsigned char &)Copy) = Get_Bit(&BitArray[0], index);
        }
        ((int &)LastIndex) = index;
    }
}


extern "C" unsigned char vtable_70f3c0[]; extern "C" void __cdecl release_buffer_70f3c0(void*); extern "C" void __cdecl release_object_70f3c0(void*);
class Leaf_70f3c0 { public: void* get(unsigned int flags); };
void* Leaf_70f3c0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_70f3c0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_70f3c0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_70f3c0(this); return this; }


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<unsigned char>::Fetch (0x0070F3B0; VectorClass<unsigned char> vtable 0x007ae6ec slot 6).
// VectorClass<unsigned char>::operator equal (0x0070F410; VectorClass<unsigned char> vtable 0x007ae6ec slot 1).
// VectorClass<unsigned char>::Resize (0x0070F450; VectorClass<unsigned char> vtable 0x007ae6ec slot 2).
// VectorClass<unsigned char>::Clear (0x0070F500; VectorClass<unsigned char> vtable 0x007ae6ec slot 3).
// VectorClass<unsigned char>::ID_object (0x0070F530; VectorClass<unsigned char> vtable 0x007ae6ec slot 4).
// VectorClass<unsigned char>::ID_pointer (0x0070F570; VectorClass<unsigned char> vtable 0x007ae6ec slot 5).
