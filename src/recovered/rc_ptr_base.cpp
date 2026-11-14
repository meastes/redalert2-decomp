// rc_ptr_base.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/rc_ptr_base.json carries the addresses and the rows that
// stay in their own files.


#define RA2_RC_PTR_UNIT
#include "shared/reference_counted.hpp"


// rc_ptr_base::rc_ptr_base() (0x006394E0): a null pointer.
// The rc_ptr/readline unit 0x006394E0-0x0063976F: rc_ptr_base members, Tiberian Sun readline.cpp, then
// ReferenceCounted (its scalar deleting destructor 0x00639750 is emitted with the vtable). The rc_ptr_base copy
// constructor is declared for this unit only (RA2_RC_PTR_UNIT).

rc_ptr_base::rc_ptr_base() : Pointer(0)
{
}


// rc_ptr_base::rc_ptr_base(ReferenceCounted *pointer) (0x006394F0): takes a reference.

rc_ptr_base::rc_ptr_base(ReferenceCounted *pointer) : Pointer(pointer)
{
    if (pointer != 0) {
        pointer->RefCount++;
    }
}


// rc_ptr_base::~rc_ptr_base() (0x00639510): releases the pointer (tail call).

rc_ptr_base::~rc_ptr_base()
{
    if (Pointer != 0) {
        ReferenceCounted *old = Pointer;
        Pointer = 0;
        old->Release();
    }
}


// void rc_ptr_base::Set(ReferenceCounted *pointer) (0x00639530; was UnknownRefHolder_00639530::Set): takes a reference to pointer, releases the old one.

void rc_ptr_base::Set(ReferenceCounted *pointer)
{
    if (pointer != Pointer) {
        if (pointer != 0) {
            pointer->RefCount++;
        }
        if (Pointer != 0) {
            ReferenceCounted *old = Pointer;
            Pointer = 0;
            old->Release();
        }
        Pointer = pointer;
    }
}


// rc_ptr_base::rc_ptr_base(rc_ptr_base const &other) (0x00639560): shares other's pointer (its scalar deleting destructor 0x00639580 follows).

rc_ptr_base::rc_ptr_base(rc_ptr_base const &other) : Pointer(other.Pointer)
{
    if (Pointer != 0) {
        Pointer->RefCount++;
    }
}


// rc_ptr_base's scalar deleting destructor (0x00639580), emitted with its vtable.
