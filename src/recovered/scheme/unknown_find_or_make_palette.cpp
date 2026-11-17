// int __fastcall unknown_find_or_make_palette(char const *name, HSVClass const &color, void *arg3, void *arg4, int key) -- 0x00663CA0
// Purpose: the index in ColorSchemes (0x00AB6EF8) of the scheme named `name`
// (case-insensitive, +0x304) with `key` (+0x310); when there is none a new
// scheme is made (ColorScheme::ColorScheme 0x006639F0 adds it) and the
// search repeats. The arguments are RulesClass::Colors's (0x00647E60).
// Assumptions: arg3/arg4 are passed through untyped (the constructor takes arg4 as a PaletteClass).
#define VECTOR_IMPLEMENT_ALL
#include "shared/vector_class.hpp"
#include "shared/color_scheme.hpp"
// @ref ?ColorSchemes@@3V?$DynamicVectorClass@PAVColorScheme@@@@A=00ab6ef8
// @ref ??0ColorScheme@@QAE@PBDABVHSVClass@@PAXPAVPaletteClass@@H_N@Z=006639f0
// @ref _stricmp=00781f20
// @ref ??2@YAPAXI@Z=00782017

extern "C" int __cdecl _stricmp(char const *left, char const *right);

int __fastcall unknown_find_or_make_palette(char const *name, HSVClass const &color, void *arg3, void *arg4, int key)
{
    for (;;) {
        for (int index = 0; index < ColorSchemes.Count(); index++) {
            if (_stricmp(name, ColorSchemes[index]->Name) == 0 && ColorSchemes[index]->unknown_0x310 == key) {
                return index;
            }
        }
        new ColorScheme(name, color, arg3, (PaletteClass *)arg4, key, true);
    }
}
