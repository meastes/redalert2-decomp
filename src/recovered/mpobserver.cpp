// mpobserver.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/mpobserver.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"


// Header-static dynamic initializers of the unit at 0x005AA530: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x005AA530-0x005AA711); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x005AA550, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_005aa530.cpp; no source of its own.


// Dynamic initializer (0x005AA580, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_005aa530.cpp; no source of its own.


// Dynamic initializer (0x005AA5A0, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_005aa530.cpp; no source of its own.


// Dynamic initializer (0x005AA5C0, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_005aa530.cpp; no source of its own.


// Dynamic initializer (0x005AA5E0, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_005aa530.cpp; no source of its own.


// Dynamic initializer (0x005AA600, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_005aa530.cpp; no source of its own.


// Dynamic initializer (0x005AA620, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_005aa530.cpp; no source of its own.


// Dynamic initializer (0x005AA660, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_005aa530.cpp; no source of its own.


// Dynamic initializer (0x005AA690, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_005aa530.cpp; no source of its own.


// Dynamic initializer (0x005AA6C0, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_005aa530.cpp; no source of its own.


// Dynamic initializer (0x005AA6F0, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_005aa530.cpp; no source of its own.


// Dynamic initializer (0x005AA700, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_005aa530.cpp; no source of its own.


// 0x005aa720: function not yet identified
//
// Owner: Megawealth, stores the Megawealth vtable (constructor or destructor) (from RTTI/vtables).
// Body: 52 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 * __thiscall Megawealth_ctor_005aa720(undefined4 *param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4, undefined4 param_5,undefined4 param_6,undefined4 param_7)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_005aa720 {

struct Callee_005b6cd0 { void MultiplayerGameMode_ctor_005b6cd0(unsigned int, unsigned int, unsigned int, unsigned int, unsigned char, unsigned int); };
extern "C" void* Megawealth_vtable_007a6aa4;

struct Owner_005aa720 { unsigned int * Megawealth_ctor_005aa720(unsigned int a, unsigned int b, unsigned int c, unsigned int d, unsigned int flag, unsigned int e); };

unsigned int * Owner_005aa720::Megawealth_ctor_005aa720(unsigned int a, unsigned int b, unsigned int c, unsigned int d, unsigned int flag, unsigned int e)
{
    unsigned int * self = (unsigned int *)this;
    ((Callee_005b6cd0*)(self))->MultiplayerGameMode_ctor_005b6cd0((unsigned int)(a), (unsigned int)(b), (unsigned int)(c), (unsigned int)(d), (unsigned char)(flag), (unsigned int)(e));
    *self = (unsigned int)(&Megawealth_vtable_007a6aa4);
    return self;
}

}  // namespace seed_005aa720


// 0x005aa760: function not yet identified
//
// Owner: Megawealth, stores the Megawealth vtable (constructor or destructor) (from RTTI/vtables).
// Body: 11 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall Megawealth_dtor_005aa760(undefined4 *param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_005aa760 {

extern void __fastcall MultiplayerGameMode_dtor_005b6e20(void);
extern "C" void* Megawealth_vtable_007a6aa4;

void __fastcall Megawealth_dtor_005aa760(unsigned int *self)
{
    *self = (unsigned int)(&Megawealth_vtable_007a6aa4);
    MultiplayerGameMode_dtor_005b6e20();
    return;
}

}  // namespace seed_005aa760


// 0x005aa770: function not yet identified
//
// Owner: Megawealth, virtual slot 29 (vtable +0x74) (from RTTI/vtables).
// Body: 13 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined1 unknown_slot_005aa770(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_005aa770 {

struct Callee_0055a1f0 { void Map_Unknown_0055a1f0(); };
extern "C" unsigned int Map_008324e0;

unsigned char  unknown_slot_005aa770(void)
{
    ((Callee_0055a1f0*)(&Map_008324e0))->Map_Unknown_0055a1f0();
    return 1;
}

}  // namespace seed_005aa770


// 0x005aa780: function not yet identified
//
// Owner: Megawealth, virtual slot 0 (vtable +0x0) (from RTTI/vtables).
// Body: 36 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 * __thiscall scalar_deleting_destructor_005aa780(undefined4 *param_1,byte param_2)
// Callees with accepted source:
//   operator_delete_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_005aa780 {

extern void __fastcall MultiplayerGameMode_dtor_005b6e20(void);
extern void  operator_delete_00781d3b(unsigned int);
extern "C" void* Megawealth_vtable_007a6aa4;

struct Owner_005aa780 { unsigned int * scalar_deleting_destructor_005aa780(unsigned char flags); };

unsigned int * Owner_005aa780::scalar_deleting_destructor_005aa780(unsigned char flags)
{
    unsigned int * self = (unsigned int *)this;
    *self = (unsigned int)(&Megawealth_vtable_007a6aa4);
    MultiplayerGameMode_dtor_005b6e20();
    if ((flags & 1) != 0) {
        operator_delete_00781d3b((unsigned int)(self));
    }
    return self;
}

}  // namespace seed_005aa780


class Target_5aa800 { public: unsigned int target(unsigned int p0); };
class Leaf_5aa800 { public: unsigned int get(unsigned int a1); };
unsigned int Leaf_5aa800::get(unsigned int a1) { return ((Target_5aa800*)(this))->target((unsigned int)(a1)); }


// 0x005aa810: function not yet identified
//
// Owner: MultiplayerObserverTeam, virtual slot 0 (vtable +0x0) (from RTTI/vtables).
// Body: 30 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 __thiscall scalar_deleting_destructor_005aa810(undefined4 param_1,byte param_2)
// Callees with accepted source:
//   operator_delete_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_005aa810 {

extern void __fastcall MultiplayerTeam_dtor_005b9b50(void);
extern void  operator_delete_00781d3b(unsigned int);

struct Owner_005aa810 { unsigned int scalar_deleting_destructor_005aa810(unsigned char flags); };

unsigned int Owner_005aa810::scalar_deleting_destructor_005aa810(unsigned char flags)
{
    unsigned int self = (unsigned int)this;
    MultiplayerTeam_dtor_005b9b50();
    if ((flags & 1) != 0) {
        operator_delete_00781d3b((unsigned int)(self));
    }
    return self;
}

}  // namespace seed_005aa810
