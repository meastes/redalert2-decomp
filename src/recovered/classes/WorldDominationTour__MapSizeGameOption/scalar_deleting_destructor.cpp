// 0x00728c30: function not yet identified
//
// Owner: WorldDominationTour::MapSizeGameOption, virtual slot 0 (vtable +0x0) (from RTTI/vtables).
// Body: 36 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 * __thiscall scalar_deleting_destructor_00728c30(undefined4 *self,byte flags)
// Callees with accepted source:
//   ReferenceCounted_destructor_00639710 -> src/recovered/classes/ReferenceCounted/00639710.cpp
//   operator_delete_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00728c30 {

extern void __fastcall ReferenceCounted_destructor_00639710(void);
extern void  operator_delete_00781d3b(unsigned int block);
extern "C" void* MapSizeGameOption_vtable_007af118;

struct Owner_00728c30 { unsigned int * scalar_deleting_destructor_00728c30(unsigned char flags); };

unsigned int * Owner_00728c30::scalar_deleting_destructor_00728c30(unsigned char flags)
{
    unsigned int * self = (unsigned int *)this;
    *self = (unsigned int)(&MapSizeGameOption_vtable_007af118);
    ReferenceCounted_destructor_00639710();
    if ((flags & 1) != 0) {
        operator_delete_00781d3b((unsigned int)(self));
    }
    return self;
}

}  // namespace seed_00728c30
