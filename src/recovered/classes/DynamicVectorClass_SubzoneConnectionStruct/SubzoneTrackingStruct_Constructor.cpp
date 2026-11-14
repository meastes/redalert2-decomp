// 0x0056b9b0: function not yet identified
//
// Owner: DynamicVectorClass<SubzoneConnectionStruct>, stores the DynamicVectorClass<SubzoneConnectionStruct> vtable (constructor or destructor) (from RTTI/vtables).
// Body: 56 bytes (frozen inventory bound).
// Referenced from 2 sites (unconditional call).
// Ghidra prototype: undefined4 * __fastcall SubzoneTrackingStruct_Constructor_0056b9b0(undefined4 *param_1)
//
// Types and names are Ghidra placeholders (see shared/ghidra_types.hpp);
// rename and retype as the purpose becomes known.
//
#include "shared/ghidra_types.hpp"

namespace seed_0056b9b0 {

extern "C" void* PTR_FUN_007a5ad8;
extern "C" void* PTR_FUN_007a5ae4;

undefined4 * __fastcall SubzoneTrackingStruct_Constructor_0056b9b0(undefined4 *vector_words)
{
    vector_words[1] = 0;
    vector_words[2] = 0;
    *(undefined1 *)(vector_words + 3) = 1;
    *(undefined1 *)((int)vector_words + 0xd) = 0;
    *vector_words = (undefined4)(&PTR_FUN_007a5ad8);
    vector_words[5] = 10;
    vector_words[4] = 0;
    (*(code *)PTR_FUN_007a5ae4)();
    *(undefined2 *)(vector_words + 6) = 0;
    vector_words[7] = 0;
    vector_words[8] = 0;
    return vector_words;
}

}  // namespace seed_0056b9b0
