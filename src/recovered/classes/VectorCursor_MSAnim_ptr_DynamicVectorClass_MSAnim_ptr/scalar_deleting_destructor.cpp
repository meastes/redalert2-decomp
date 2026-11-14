// 0x007303f0: function not yet identified
//
// Owner: VectorCursor<MSAnim *, DynamicVectorClass<MSAnim *>>, virtual slot 0 (vtable +0x0) (from RTTI/vtables).
// Body: 32 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 * __thiscall scalar_deleting_destructor_007303f0(undefined4 *param_1,byte param_2)
// Callees with accepted source:
//   operator_delete_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_007303f0 {

extern void  operator_delete_00781d3b(unsigned int);
extern "C" void* VectorCursor_vtable_007af3a4;

struct Owner_007303f0 { unsigned int * scalar_deleting_destructor_007303f0(unsigned char flags); };

unsigned int * Owner_007303f0::scalar_deleting_destructor_007303f0(unsigned char flags)
{
    unsigned int * self = (unsigned int *)this;
    *self = (unsigned int)(&VectorCursor_vtable_007af3a4);
    if ((flags & 1) != 0) {
        operator_delete_00781d3b((unsigned int)(self));
    }
    return self;
}

}  // namespace seed_007303f0
