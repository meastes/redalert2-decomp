// effects.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/effects.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/shape_file.hpp"
#include "shared/convert.hpp"
#include "shared/palette_lookup.hpp"
#include "shared/crt_memory.hpp"
#include "shared/crt_string.hpp"


// Header-static dynamic initializers of the unit at 0x004B2F50:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x004B2F50-0x004B3136); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x004B2F80, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_004b2f50.cpp; no source of its own.


// Dynamic initializer (0x004B2FA0, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_004b2f50.cpp; no source of its own.


// Dynamic initializer (0x004B2FC0, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_004b2f50.cpp; no source of its own.


// Dynamic initializer (0x004B2FE0, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_004b2f50.cpp; no source of its own.


// Dynamic initializer (0x004B3000, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_004b2f50.cpp; no source of its own.


// Dynamic initializer (0x004B3020, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_004b2f50.cpp; no source of its own.


// Dynamic initializer (0x004B3060, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_004b2f50.cpp; no source of its own.


// Dynamic initializer (0x004B3090, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_004b2f50.cpp; no source of its own.


// Dynamic initializer (0x004B30C0, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_004b2f50.cpp; no source of its own.


// Dynamic initializer (0x004B30F0, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_004b2f50.cpp; no source of its own.


// Dynamic initializer (0x004B3100, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_004b2f50.cpp; no source of its own.


// Dynamic initializer (0x004B3120, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_004b2f50.cpp; no source of its own.


// Original unit header-static zero rectangle at 0x004B3140.


unsigned char *__fastcall Build_Remap_Adjusted_Lookup_004b3160(PaletteClass const &palette,
    unsigned char *lookup, int target_index, int ratio)
{
    if (lookup) {
        unsigned char *output = lookup;
        for (int index = 0; index < 256; ++index) {
            if (index <= 240 && index != 0) {
                RGBClass color = palette[index];
                color.Adjust(ratio, palette[target_index]);
                int best = -1;
                int distance = 0;
                for (int candidate = 240; candidate < 255; ++candidate) {
                    int current = palette[candidate].Difference(color);
                    if (best == -1 || current < distance) {
                        best = candidate;
                        distance = current;
                    }
                }
                *output++ = (unsigned char)best;
            } else {
                *output++ = (unsigned char)index;
            }
        }
    }
    return lookup;
}


unsigned int __fastcall Palette_Lookup_Table_Size_004b3260(unsigned int count) { return (count + 1) * 256; }


unsigned char *__fastcall Build_Adjusted_Tables_004b3270(PaletteClass const &palette,
    PaletteLookupRule const *rules, int count, unsigned char *tables)
{
    if (count && rules) {
        if (!tables) tables = new unsigned char[(count + 1) * 256];
        if (tables) {
            memset(tables, 255, 256);
            unsigned char *output = tables + 256;
            for (int i = 0; i < count; ++i) {
                tables[rules[i].Key] = (unsigned char)i;
                int ratio = rules[i].Ratio;
                RGBClass target = palette[rules[i].ColorIndex];
                output[0] = 0;
                for (int index = 1; index < 256; ++index) {
                    RGBClass color = palette[index];
                    color.Adjust(ratio, target);
                    output[index] = (unsigned char)palette.Closest_Color(color);
                }
                output += 256;
            }
        }
    }
    return tables;
}


unsigned char *__fastcall Build_Remap_Adjusted_Tables_004b33b0(PaletteClass const &palette,
    PaletteLookupRule const *rules, int count, unsigned char *tables)
{
    if (count && rules && (tables || (tables = new unsigned char[(count + 1) * 256]))) {
        int i = 0;
        memset(tables, 255, 256);
        unsigned char *output = tables + 256;
        for (; i < count; ++i) {
            tables[rules[i].Key] = (unsigned char)i;
            Build_Remap_Adjusted_Lookup_004b3160(palette, output,
                rules[i].ColorIndex, rules[i].Ratio);
            output += 256;
        }
    }
    return tables;
}


unsigned char *__fastcall Build_Adjusted_Lookup_004b3460(PaletteClass const &palette,
    unsigned char *lookup, int target_index, int ratio)
{
    if (lookup) {
        int index = 0;
        for (; index < 256; ++index) {
            RGBClass color = palette[index];
            color.Adjust(ratio, palette[target_index]);
            lookup[index] = (unsigned char)palette.Closest_Color(color);
        }
    }
    return lookup;
}


unsigned char *__fastcall Build_Transparent_Adjusted_Lookup_004b34f0(PaletteClass const &palette,
    unsigned char *lookup, int target_index, int ratio)
{
    RGBClass target = palette[target_index];
    lookup[0] = 0;
    for (int index = 1; index < 256; ++index) {
        RGBClass color = palette[index];
        color.Adjust(ratio, target);
        lookup[index] = (unsigned char)palette.Closest_Color(color);
    }
    return lookup;
}
