// _rules.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/_rules.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unknown_geometry.hpp"
#include "shared/ccini_class.hpp"


// Header-static dynamic initializers of the unit at 0x0040DC70:  the
// shared/unknown_geometry.hpp constants (0x0040DC70-0x0040DE31); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x0040DCA0, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_0040dc70.cpp; no source of its own.


// Dynamic initializer (0x0040DCC0, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_0040dc70.cpp; no source of its own.


// Dynamic initializer (0x0040DCE0, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_0040dc70.cpp; no source of its own.


// Dynamic initializer (0x0040DD00, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_0040dc70.cpp; no source of its own.


// Dynamic initializer (0x0040DD20, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_0040dc70.cpp; no source of its own.


// Dynamic initializer (0x0040DD40, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_0040dc70.cpp; no source of its own.


// Dynamic initializer (0x0040DD80, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_0040dc70.cpp; no source of its own.


// Dynamic initializer (0x0040DDB0, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_0040dc70.cpp; no source of its own.


// Dynamic initializer (0x0040DDE0, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_0040dc70.cpp; no source of its own.


// Dynamic initializer (0x0040DE10, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_0040dc70.cpp; no source of its own.


// Dynamic initializer (0x0040DE20, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_0040dc70.cpp; no source of its own.


// The INI globals unit (0x0040DC70): after its header statics, the five
// CCINIClass objects it defines. Each gets a dynamic initializer (the inline
// CCINIClass constructor, then atexit) and an atexit destructor that calls
// ~INIClass (0x0050B8D0); the other drafts of this plan at 0x0040DED0-
// 0x0040E150 are emitted by these definitions, as are the GenericNode and
// INI list helpers at 0x0040E160-0x0040E300 (kept as their own regions).

CCINIClass ArtINI;              // 0x008397F0
CCINIClass unknown_ini_839860;  // 0x00839860
CCINIClass unknown_ini_8397a0;  // 0x008397A0
CCINIClass unknown_ini_8396e0;  // 0x008396E0
CCINIClass unknown_ini_839740;  // 0x00839740


// atexit destructor (0x0040DED0, _$E) of ArtINI: emitted by the definition in
// ArtINI_init_0040de40.cpp; no source of its own.


// Dynamic initializer (0x0040DEE0, _$E) of unknown_ini_839860: emitted by the definition in
// ArtINI_init_0040de40.cpp; no source of its own.


// atexit destructor (0x0040DF70, _$E) of unknown_ini_839860: emitted by the definition in
// ArtINI_init_0040de40.cpp; no source of its own.


// Dynamic initializer (0x0040DF80, _$E) of unknown_ini_8397a0: emitted by the definition in
// ArtINI_init_0040de40.cpp; no source of its own.


// atexit destructor (0x0040E010, _$E) of unknown_ini_8397a0: emitted by the definition in
// ArtINI_init_0040de40.cpp; no source of its own.


// Dynamic initializer (0x0040E020, _$E) of unknown_ini_8396e0: emitted by the definition in
// ArtINI_init_0040de40.cpp; no source of its own.


// atexit destructor (0x0040E0B0, _$E) of unknown_ini_8396e0: emitted by the definition in
// ArtINI_init_0040de40.cpp; no source of its own.


// Dynamic initializer (0x0040E0C0, _$E) of unknown_ini_839740: emitted by the definition in
// ArtINI_init_0040de40.cpp; no source of its own.


// atexit destructor (0x0040E150, _$E) of unknown_ini_839740: emitted by the definition in
// ArtINI_init_0040de40.cpp; no source of its own.


class Leaf_40e160 { public: unsigned int get(); };
unsigned int Leaf_40e160::get() { unsigned int initialization_value = (unsigned int)this; unsigned int object_address; unsigned int unused_value; object_address = initialization_value; initialization_value = 0; *(unsigned int*)(object_address + 4) = initialization_value; *(unsigned int*)(object_address + 8) = initialization_value; *(unsigned int*)(object_address) = 0x79aafc; return object_address; }


// 0x0040e180: function not yet identified
//
// Body: 75 bytes (frozen inventory bound).
// Referenced from 25 sites (unconditional call).
// Ghidra prototype: void __thiscall FUN_0040e180(int param_1,int param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0040e180 {

struct Owner_0040e180 { void FUN_0040e180(int node_address); };

void Owner_0040e180::FUN_0040e180(int node_address)
{
    int anchor_address = (int)this;
    unsigned int neighbor_address;

    if (((node_address != 0) && (*(int *)(node_address + 4) != 0)) && (*(int *)(node_address + 8) != 0)) {
        *(int *)(*(int *)(node_address + 8) + 4) = *(int *)(node_address + 4);
        *(unsigned int *)(*(int *)(node_address + 4) + 8) = *(unsigned int *)(node_address + 8);
        *(unsigned int *)(node_address + 8) = 0;
        *(unsigned int *)(node_address + 4) = 0;
    }
    neighbor_address = (unsigned int)(*(unsigned int *)(anchor_address + 4));
    *(int *)(node_address + 8) = anchor_address;
    *(unsigned int *)(node_address + 4) = neighbor_address;
    if (*(int *)(anchor_address + 4) != 0) {
        *(int *)(*(int *)(anchor_address + 4) + 8) = node_address;
    }
    *(int *)(anchor_address + 4) = node_address;
    return;
}

}  // namespace seed_0040e180


// 0x0040e1d0: function not yet identified
//
// Body: 69 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 * __thiscall FUN_0040e1d0(undefined4 *param_1,byte param_2)
// Callees with accepted source:
//   FUN_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0040e1d0 {

extern void  FUN_00781d3b(unsigned int allocation_address);
extern "C" void* PTR_FUN_0079aafc;

struct Owner_0040e1d0 { unsigned int * FUN_0040e1d0(unsigned char deletion_flags); };

unsigned int * Owner_0040e1d0::FUN_0040e1d0(unsigned char deletion_flags)
{
    unsigned int * object_words = (unsigned int *)this;
    *object_words = (unsigned int)(&PTR_FUN_0079aafc);
    if (((object_words != (unsigned int *)0x0) && (object_words[1] != 0)) && (object_words[2] != 0)) {
        *(unsigned int *)(object_words[2] + 4) = object_words[1];
        *(unsigned int *)(object_words[1] + 8) = object_words[2];
        object_words[2] = 0;
        object_words[1] = 0;
    }
    if ((deletion_flags & 1) != 0) {
        FUN_00781d3b((unsigned int)(object_words));
    }
    return object_words;
}

}  // namespace seed_0040e1d0


extern "C" void __cdecl release_40e2e0(void*);
class Leaf_40e2e0 { public: void cleanup();void* get(unsigned int flags); };
void* Leaf_40e2e0::get(unsigned int flags) { cleanup(); if (flags & 1) release_40e2e0(this); return this; }
