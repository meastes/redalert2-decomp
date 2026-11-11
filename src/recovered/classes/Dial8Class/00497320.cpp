// 0x00497320: function not yet identified
//
// Owner: Dial8Class, virtual slot 0 (vtable +0x0) (from RTTI/vtables).
// Body: 30 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 __thiscall FUN_00497320(undefined4 param_1,byte param_2)
// Calls GadgetClass::~GadgetClass, then operator delete when flags bit 0 is set
// (a scalar deleting destructor of a GadgetClass-derived class with a trivial destructor).
//
// Types and names are Ghidra placeholders (see shared/ghidra_types.hpp);
// rename and retype as the purpose becomes known.
//
#include "shared/gadget.hpp"

namespace seed_00497320 {

struct Owner_00497320 { unsigned int FUN_00497320(unsigned char flags); };

unsigned int Owner_00497320::FUN_00497320(unsigned char flags)
{
    ((GadgetClass *)this)->GadgetClass::~GadgetClass();
    if ((flags & 1) != 0) {
        operator delete(this);
    }
    return (unsigned int)this;
}

}  // namespace seed_00497320
