// scheme.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/scheme.json carries the addresses and the rows that
// stay in their own files.


// The colour scheme unit's global list, ColorSchemes (0x00AB6EF8): its
// initializer (0x00663610) and atexit destructor (0x00663650). The list's
// vtables and template members are the copies at 0x007A7B98/0x007A7BB8 and
// 0x00604CD0-0x006055F0 (another unit's, kept by the linker).
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/color_scheme.hpp"

DynamicVectorClass<ColorScheme *> ColorSchemes;


// atexit destructor (0x00663650, _$E) of ColorSchemes: emitted by the definition in
// ColorSchemes_init_00663610.cpp; no source of its own.


// Build_Light_Converter (0x00663690; Tiberian Sun scheme.cpp): copies the base palette into
// remapped, shades its sixteen-entry remap ramp (indices 16-31) from the colour and returns a
// light converter for it. Declared as Unknown_00663690 (shared/color_scheme.hpp): base is the
// base palette, unknown_b the screen palette, table the remap flags (hypotheses).
#include "shared/convert.hpp"
#include "shared/light_convert.hpp"

ConvertClass *__fastcall Unknown_00663690(HSVClass const *hsv, void *base, int unknown_b, RGBClass *remapped,
    Surface *surface, int intensity_levels, int r, int g, int b, void const *table)
{
    int hue = hsv->Hue;
    int saturation = hsv->Saturation;
    int value = hsv->Value;

    *(PaletteClass *)remapped = *(PaletteClass const *)base;

    double cos_step = 0.08144869842640204;      // 14/3 degrees
    double sin_step = 0.046542113386515455;     // 8/3 degrees

    for (int i = 0; i < 16; i++) {
        double cosval = 0.3490658503988659 + (i * cos_step);    // 20 degrees
        double sinval = 0.8726646259971648 + (i * sin_step);    // 50 degrees
        if (i == 0) {
            cosval = 0.19634954084936207;                       // 11.25 degrees
        }

        int sat = int(unknown_sin_4ba480(sinval) * saturation);
        int val = int(unknown_cos_4ba4d0(cosval) * value);

        RGBClass rgb = HSVClass(hue, sat, val).To_RGB();
        (*(PaletteClass *)remapped)[i + 16] = rgb;
    }

    return new LightConvertClass(*(PaletteClass *)remapped, *(PaletteClass const *)unknown_b, surface, r, g, b,
        false, (unsigned char const *)table, intensity_levels);
}


// Build_Converter (0x006637E0; Tiberian Sun scheme.cpp): shades the sixteen-entry remap ramp
// from the colour into a copy of the base palette and returns a plain converter for it.

ConvertClass *__fastcall Build_Converter_006637e0(HSVClass const &hsv, PaletteClass const &base,
    PaletteClass const &screenpalette, Surface *surface, PaletteClass *remapped)
{
    int hue = hsv.Hue;
    int saturation = hsv.Saturation;
    int value = hsv.Value;

    PaletteClass pal = base;

    double cos_step = 0.08144869842640204;      // 14/3 degrees
    double sin_step = 0.046542113386515455;     // 8/3 degrees

    for (int i = 0; i < 16; i++) {
        double cosval = 0.3490658503988659 + (i * cos_step);
        double sinval = 0.8726646259971648 + (i * sin_step);
        if (i == 0) {
            cosval = 0.19634954084936207;
        }

        int sat = int(unknown_sin_4ba480(sinval) * saturation);
        int val = int(unknown_cos_4ba4d0(cosval) * value);

        RGBClass rgb = HSVClass(hue, sat, val).To_RGB();
        pal[i + 16] = rgb;
    }

    if (remapped != 0) {
        *remapped = pal;
    }

    return new ConvertClass(pal, screenpalette, surface, 1, false);
}


// ColorScheme::ColorScheme() (0x00663930; Tiberian Sun scheme.cpp): a blank scheme with no name
// or converter yet that takes its place in ColorSchemes.
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ADD

ColorScheme::ColorScheme() :
    Name(0),
    BaseColor(0, 0, 0),
    Converter(0),
    unknown_0x310(1)
{
    ColorSchemes.Add(this);
    Index = ColorSchemes.Count();
}


// ColorScheme::ColorScheme(...) -- 0x006639F0
// Builds a colour scheme from a base colour and, when asked, adds it to ColorSchemes.
#include "shared/unknown_globals.hpp"
extern "C" char *__cdecl strdup(char const *string);

ColorScheme::ColorScheme(char const *name, HSVClass const &base, void *unknown_a, PaletteClass *palette, int unknown_c, bool add) :
    Name(strdup(name)),
    BaseColor(base)
{
    Converter = 0;
    unknown_0x310 = unknown_c;
    unknown_0x314 = 0x10;
    unknown_0x318 = 0xF;
    unknown_color_0x31c = 0x19;
    unknown_color_0x320 = 0x18;
    unknown_color_0x324 = 0x16;
    unknown_color_0x328 = 0x10;
    unknown_color_0x32c = 0x13;
    unknown_color_0x330 = 0x10;
    unknown_0x334 = 0x10;
    unknown_0x338 = 0x15;
    Converter = Unknown_00663690(&BaseColor, unknown_a, (int)palette, Palette, unknown_surface_839950, unknown_c, 1000, 1000, 1000, unknown_table_7f264c);
    if (add) {
        ColorSchemes.Add(this);
        Index = ColorSchemes.Count();
    }
}


// ColorScheme::Unknown_00663B40 (0x00663B40; Tiberian Sun's Build_Light_Converters): throws away
// the converter and rebuilds it from the palettes with the tint.

void ColorScheme::Unknown_00663B40(PaletteClass *unknown_a, PaletteClass *palette, int rscale, int gscale, int bscale)
{
    if (Converter != 0) {
        delete Converter;
        Converter = 0;
    }

    Converter = Unknown_00663690(&BaseColor, unknown_a, (int)palette, Palette, unknown_surface_839950, unknown_0x310,
        rscale, gscale, bscale, unknown_table_7f264c);
}


// 0x00663c40: function not yet identified
//
// Body: 34 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: bool __thiscall FUN_00663c40(int param_1,int param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00663c40 {

extern unsigned int  FUN_00781f20(unsigned char *first_buffer, unsigned char *second_buffer);

struct Owner_00663c40 { bool FUN_00663c40(int other_scheme_address); };

bool Owner_00663c40::FUN_00663c40(int other_scheme_address)
{
    int scheme_address = (int)this;
    int comparison_result;

    comparison_result = (int)(FUN_00781f20((unsigned char *)(*(unsigned int *)(scheme_address + 0x304)), (unsigned char *)(*(unsigned int *)(other_scheme_address + 0x304))));
    return comparison_result == 0;
}

}  // namespace seed_00663c40


// 0x00663c70: function not yet identified
//
// Body: 35 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: bool __thiscall FUN_00663c70(int param_1,int param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00663c70 {

extern unsigned int  FUN_00781f20(unsigned char *first_buffer, unsigned char *second_buffer);

struct Owner_00663c70 { bool FUN_00663c70(int other_scheme_address); };

bool Owner_00663c70::FUN_00663c70(int other_scheme_address)
{
    int scheme_address = (int)this;
    int comparison_result;

    comparison_result = (int)(FUN_00781f20((unsigned char *)(*(unsigned int *)(scheme_address + 0x304)), (unsigned char *)(*(unsigned int *)(other_scheme_address + 0x304))));
    return comparison_result != 0;
}

}  // namespace seed_00663c70
