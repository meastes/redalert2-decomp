// mpsiegeteam.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/mpsiegeteam.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"


// Header-static dynamic initializers of the unit at 0x005ABBD0: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x005ABBD0-0x005ABDB1); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x005ABBF0, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_005abbd0.cpp; no source of its own.


// Dynamic initializer (0x005ABC20, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_005abbd0.cpp; no source of its own.


// Dynamic initializer (0x005ABC40, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_005abbd0.cpp; no source of its own.


// Dynamic initializer (0x005ABC60, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_005abbd0.cpp; no source of its own.


// Dynamic initializer (0x005ABC80, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_005abbd0.cpp; no source of its own.


// Dynamic initializer (0x005ABCA0, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_005abbd0.cpp; no source of its own.


// Dynamic initializer (0x005ABCC0, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_005abbd0.cpp; no source of its own.


// Dynamic initializer (0x005ABD00, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_005abbd0.cpp; no source of its own.


// Dynamic initializer (0x005ABD30, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_005abbd0.cpp; no source of its own.


// Dynamic initializer (0x005ABD60, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_005abbd0.cpp; no source of its own.


// Dynamic initializer (0x005ABD90, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_005abbd0.cpp; no source of its own.


// Dynamic initializer (0x005ABDA0, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_005abbd0.cpp; no source of its own.


// 0x005abe10: function not yet identified
//
// Owner: MultiplayerSiegeDefenderTeam, stores the MultiplayerSiegeDefenderTeam vtable (constructor or destructor) (from RTTI/vtables).
// Body: 11 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall FUN_005abe10(undefined4 *param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_005abe10 {

extern void __fastcall FUN_005b9b50(void);
extern "C" void* PTR_FUN_007a6c54;

void __fastcall FUN_005abe10(unsigned int *object_words)
{
    *object_words = (unsigned int)(&PTR_FUN_007a6c54);
    FUN_005b9b50();
    return;
}

}  // namespace seed_005abe10


// 0x005abeb0: function not yet identified
//
// Owner: MultiplayerSiegeAttackerTeam, stores the MultiplayerSiegeAttackerTeam vtable (constructor or destructor) (from RTTI/vtables).
// Body: 11 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall FUN_005abeb0(undefined4 *param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_005abeb0 {

extern void __fastcall FUN_005b9b50(void);
extern "C" void* PTR_FUN_007a6c64;

void __fastcall FUN_005abeb0(unsigned int *object_words)
{
    *object_words = (unsigned int)(&PTR_FUN_007a6c64);
    FUN_005b9b50();
    return;
}

}  // namespace seed_005abeb0


// 0x005abec0: function not yet identified
//
// Owner: MultiplayerSiegeDefenderTeam, virtual slot 0 (vtable +0x0) (from RTTI/vtables).
// Body: 36 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 * __thiscall FUN_005abec0(undefined4 *param_1,byte param_2)
// Callees with accepted source:
//   FUN_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_005abec0 {

extern void __fastcall FUN_005b9b50(void);
extern void  FUN_00781d3b(unsigned int allocation_address);
extern "C" void* PTR_FUN_007a6c54;

struct Owner_005abec0 { unsigned int * FUN_005abec0(unsigned char deletion_flags); };

unsigned int * Owner_005abec0::FUN_005abec0(unsigned char deletion_flags)
{
    unsigned int * object_words = (unsigned int *)this;
    *object_words = (unsigned int)(&PTR_FUN_007a6c54);
    FUN_005b9b50();
    if ((deletion_flags & 1) != 0) {
        FUN_00781d3b((unsigned int)(object_words));
    }
    return object_words;
}

}  // namespace seed_005abec0


// 0x005abef0: function not yet identified
//
// Owner: MultiplayerSiegeAttackerTeam, virtual slot 0 (vtable +0x0) (from RTTI/vtables).
// Body: 36 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 * __thiscall FUN_005abef0(undefined4 *param_1,byte param_2)
// Callees with accepted source:
//   FUN_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_005abef0 {

extern void __fastcall FUN_005b9b50(void);
extern void  FUN_00781d3b(unsigned int allocation_address);
extern "C" void* PTR_FUN_007a6c64;

struct Owner_005abef0 { unsigned int * FUN_005abef0(unsigned char deletion_flags); };

unsigned int * Owner_005abef0::FUN_005abef0(unsigned char deletion_flags)
{
    unsigned int * object_words = (unsigned int *)this;
    *object_words = (unsigned int)(&PTR_FUN_007a6c64);
    FUN_005b9b50();
    if ((deletion_flags & 1) != 0) {
        FUN_00781d3b((unsigned int)(object_words));
    }
    return object_words;
}

}  // namespace seed_005abef0
