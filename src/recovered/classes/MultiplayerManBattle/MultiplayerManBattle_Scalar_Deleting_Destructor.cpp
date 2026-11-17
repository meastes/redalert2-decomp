// 0x005a7500: function not yet identified
//
// Owner: MultiplayerManBattle, virtual slot 0 (vtable +0x0) (from RTTI/vtables).
// Body: 36 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 * __thiscall MultiplayerManBattle_Scalar_Deleting_Destructor_005a7500(undefined4 *param_1,byte param_2)
// Callees with accepted source:
//   operator_delete_00781d3b -> src/recovered/libcmt/operator_delete.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_005a7500 {

extern void __fastcall MultiplayerGameMode_Destructor_005b6e20(void);
extern void  operator_delete_00781d3b(unsigned int allocation_address);
extern "C" void* MultiplayerManBattle_VTable_007a69dc;

struct Owner_005a7500 { unsigned int * MultiplayerManBattle_Scalar_Deleting_Destructor_005a7500(unsigned char deletion_flags); };

unsigned int * Owner_005a7500::MultiplayerManBattle_Scalar_Deleting_Destructor_005a7500(unsigned char deletion_flags)
{
    unsigned int * object_words = (unsigned int *)this;
    *object_words = (unsigned int)(&MultiplayerManBattle_VTable_007a69dc);
    MultiplayerGameMode_Destructor_005b6e20();
    if ((deletion_flags & 1) != 0) {
        operator_delete_00781d3b((unsigned int)(object_words));
    }
    return object_words;
}

}  // namespace seed_005a7500
