//  (0x0056B9B0)
#include "shared/ghidra_types.hpp"

extern "C" void* DynamicVectorClass_SubzoneConnectionStruct_VTable_007a5ad8;
extern "C" void* DynamicVectorClass_SubzoneConnectionStruct_Clear_VTableSlot_007a5ae4;

undefined4 * __fastcall SubzoneTrackingStruct_Constructor_0056b9b0(undefined4 *vector_words)
{
    vector_words[1] = 0;
    vector_words[2] = 0;
    *(undefined1 *)(vector_words + 3) = 1;
    *(undefined1 *)((int)vector_words + 0xd) = 0;
    *vector_words = (undefined4)(&DynamicVectorClass_SubzoneConnectionStruct_VTable_007a5ad8);
    vector_words[5] = 10;
    vector_words[4] = 0;
    (*(code *)DynamicVectorClass_SubzoneConnectionStruct_Clear_VTableSlot_007a5ae4)();
    *(undefined2 *)(vector_words + 6) = 0;
    vector_words[7] = 0;
    vector_words[8] = 0;
    return vector_words;
}

