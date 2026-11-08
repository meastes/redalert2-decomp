// ColorScheme *__fastcall unknown_color_scheme_663d30(char const *name, int kind) -- 0x00663D30
// Purpose: the scheme named `name` (case insensitive) whose +0x310 field
// equals kind, through the index lookup (0x00663D90) expanded inline; null
// when there is none.
// Assumptions: as for 0x00663D90.
#ifndef VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#endif
#ifndef VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#endif
#include "shared/vector_class.hpp"
#include "shared/color_scheme.hpp"
// @ref ?ColorSchemes@@3V?$DynamicVectorClass@PAVColorScheme@@@@A=00ab6ef8
// @ref __stricmp=00781f20

extern "C" int __cdecl _stricmp(char const *left, char const *right);

static inline int Index(char const *name, int kind)
{
    for (int index = 0; index < ColorSchemes.Count(); index++) {
        if (_stricmp(name, ColorSchemes[index]->Name) == 0 && ColorSchemes[index]->unknown_0x310 == kind) {
            return index;
        }
    }
    return -1;
}

ColorScheme *__fastcall unknown_color_scheme_663d30(char const *name, int kind)
{
    int index = Index(name, kind);
    if (index >= 0) {
        return ColorSchemes[index];
    }
    return 0;
}
