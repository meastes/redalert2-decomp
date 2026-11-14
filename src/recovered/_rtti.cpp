// _rtti.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/_rtti.json carries the addresses and the rows that
// stay in their own files.


#include "shared/rtti_name_table.hpp"
#include "shared/crt_string.hpp"
#include "shared/techno.hpp"


// Dynamic initializer (0x0040D970, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_0040d950.cpp; no source of its own.


// Dynamic initializer (0x0040D9A0, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_0040d950.cpp; no source of its own.


// Dynamic initializer (0x0040D9C0, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_0040d950.cpp; no source of its own.


// Dynamic initializer (0x0040D9E0, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_0040d950.cpp; no source of its own.


// Dynamic initializer (0x0040DA00, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_0040d950.cpp; no source of its own.


// Dynamic initializer (0x0040DA20, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_0040d950.cpp; no source of its own.


// Dynamic initializer (0x0040DA40, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_0040d950.cpp; no source of its own.


// Dynamic initializer (0x0040DA80, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_0040d950.cpp; no source of its own.


// Dynamic initializer (0x0040DAB0, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_0040d950.cpp; no source of its own.


// Dynamic initializer (0x0040DAE0, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_0040d950.cpp; no source of its own.


// Dynamic initializer (0x0040DB10, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_0040d950.cpp; no source of its own.


// Dynamic initializer (0x0040DB20, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_0040d950.cpp; no source of its own.


// Dynamic initializer (0x0040DB40, _$E) of static_rect from the header(s) included
// by HeaderStatics_empty_rect_init_0040d950.cpp; no source of its own.


// char const *__fastcall Name_From_RTTI(RTTIType rtti) (0x0040DB60)
// The name of an AbstractClass kind, "Unknown" when it is not in the table.

char const *__fastcall Name_From_RTTI(RTTIType rtti)
{
    for (int index = 0; index < (int)(sizeof(RTTINames) / sizeof(RTTINames[0])); index++) {
        if (RTTINames[index].Type == rtti) {
            return RTTINames[index].Name;
        }
    }
    return "Unknown";
}


// RTTIType __fastcall RTTI_From_Name(char const *name) (0x0040DB90)
// The AbstractClass kind of a name (case-insensitive), RTTI_NONE when it is
// not in the table.

RTTIType __fastcall RTTI_From_Name(char const *name)
{
    for (int index = 0; index < (int)(sizeof(RTTINames) / sizeof(RTTINames[0])); index++) {
        if (_stricmp(RTTINames[index].Name, name) == 0) {
            return RTTINames[index].Type;
        }
    }
    return RTTI_NONE;
}


// TechnoClass *__fastcall As_Techno_0040dbd0(AbstractClass *object) -- 0x0040DBD0
// TibSun _rtti.cpp Dynamic_Cast<TechnoClass const *>: the object as a techno when its kind is
// one (1, 2, 6, 15), else null; AbstractClass::As_Techno 0x0040DC20 is the non-const twin.

TechnoClass *__fastcall As_Techno_0040dbd0(AbstractClass *object)
{
    if (object != 0) {
        switch (object->Kind_Of()) {
        case 1:
        case 2:
        case 6:
        case 15:
            return (TechnoClass *)object;
        }
    }
    return 0;
}


// Actual direct dependency of canonical Aircraft setter, original0040DC20.
// Original guarded Kind_Of classifier: 41code+3alignment+8jump+15index=67body,13NOP.
// Existing AbstractClass::As_Techno spelling/Techno subtype remain source hypotheses.

TechnoClass *AbstractClass::As_Techno()
{
    if (this) {
        switch (Kind_Of()) {
        case 1:
        case 2:
        case 6:
        case 15:
            return static_cast<TechnoClass *>(this);
        }
    }
    return 0;
}
