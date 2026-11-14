// ColorScheme::~ColorScheme() -- 0x00663BB0
// Purpose: frees the name (+0x304, from _strdup), deletes the converter
// (+0x30C) and removes the scheme from ColorSchemes (0x00AB6EF8).
// Assumptions: ColorScheme as shared/color_scheme.hpp declares it (no
// vtable); the destructor is a hypothesis for this
// clean-up (no direct callers).
#define VECTOR_IMPLEMENT_ALL
#include "shared/vector_class.hpp"
#include "shared/color_scheme.hpp"
#include "shared/convert.hpp"
// @ref ?ColorSchemes@@3V?$DynamicVectorClass@PAVColorScheme@@@@A=00ab6ef8
// @ref _free=007825e8

extern "C" void __cdecl free(void *block);

ColorScheme::~ColorScheme()
{
    if (Name != 0) {
        free((void *)Name);
    }
    Name = 0;
    delete Converter;
    Converter = 0;
    ColorSchemes.Delete(this);
}
