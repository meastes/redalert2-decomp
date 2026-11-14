// lightcon.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/lightcon.json carries the addresses and the rows that
// stay in their own files.


#include "shared/light_convert.hpp"
#include "shared/id_source.hpp"
#include "shared/hicolor.hpp"


// LightConvertClass::LightConvertClass(...) (0x00538860)
// A tinted converter: ConvertClass, the palettes, the light colour (current and stored), then the tint for a
// scenario that has shadow levels.
bool Unknown_0051FC50();
bool Unknown_0051FC60();
extern "C" unsigned char DAT_007df768[];
extern "C" int DAT_007df868;

LightConvertClass::LightConvertClass(PaletteClass const &artpalette, PaletteClass const &screenpalette, Surface *surface,
                                     int red, int green, int blue, bool skip_blitters, unsigned char const *remap,
                                     int unknown_shadow_count)
    : ConvertClass(artpalette, screenpalette, surface, unknown_shadow_count, true),
      ArtPalette(&artpalette), ScreenPalette(&screenpalette), unknown_190(remap), unknown_194(0),
      Red(red), Green(green), Blue(blue), StoredRed(red), StoredGreen(green), StoredBlue(blue), unknown_1b0(false)
{
    if (this->unknown_shadow_count >= 1) {
        if (remap == 0) {
            unknown_190 = DAT_007df768;
        }
        if (DAT_007df868 == -1) {
            DAT_007df868 = Get_Unknown_7d6290();
        }
        if (Unknown_0051FC50()) {
            if (red == -1) {
                Red = 1000;
                Green = 1000;
                Blue = 1000;
            } else {
                Red = red;
                Green = green;
                Blue = blue;
            }
            Set_Tint(unknown_id_source_a3d290->IonRed * 1000 / 100, unknown_id_source_a3d290->IonGreen * 1000 / 100,
                     unknown_id_source_a3d290->IonBlue * 1000 / 100, true);
        } else if (Unknown_0051FC60()) {
            if (red == -1) {
                Red = 1000;
                Green = 1000;
                Blue = 1000;
            } else {
                Red = red;
                Green = green;
                Blue = blue;
            }
            Set_Tint(unknown_id_source_a3d290->unknown_22e4 * 1000 / 100, unknown_id_source_a3d290->unknown_22e8 * 1000 / 100,
                     unknown_id_source_a3d290->unknown_22ec * 1000 / 100, true);
        } else {
            Set_Tint(red, green, blue, false);
        }
        if (!skip_blitters) {
            Initialize_Blitters_00483B00();
        }
    }
}


// LightConvertClass::Set_Tint(int, int, int, bool) (0x00538AA0, vtable slot 1)
// Clamps the tint to 0..2000 (1000 = neutral), records it as the current or stored colour, then fills one
// 256-entry pixel table per shadow level: the remapped colours scale by the tint, the others by the level
// brightness, both rising from black to double over the levels.  The four table builders (0x0079777A..)
// are selected by the pixel format code 0x007DF868; any other format builds the table inline.
extern "C" unsigned char DAT_007df768[];
extern "C" int DAT_007df868;
void __cdecl FUN_0079777a(PaletteClass const *palette, unsigned short *table, int red, int green, int blue, int level, unsigned char const *remap);
void __cdecl FUN_00797aba(PaletteClass const *palette, unsigned short *table, int red, int green, int blue, int level, unsigned char const *remap);
void __cdecl FUN_00797440(PaletteClass const *palette, unsigned short *table, int red, int green, int blue, int level, unsigned char const *remap);
void __cdecl FUN_00797dfa(PaletteClass const *palette, unsigned short *table, int red, int green, int blue, int level, unsigned char const *remap);

void LightConvertClass::Set_Tint(int red, int green, int blue, bool stored)
{
    if (unknown_shadow_count >= 1) {
        if (unknown_190 == 0) {
            unknown_190 = DAT_007df768;
        }
        if (red != -1) {
            red = (red < 2000) ? red : 2000;
            red = (red > 0) ? red : 0;
            green = (green < 2000) ? green : 2000;
            green = (green > 0) ? green : 0;
            blue = (blue < 2000) ? blue : 2000;
            blue = (blue > 0) ? blue : 0;
        }
        if (stored) {
            if (red == -1) {
                red = StoredRed;
                blue = StoredBlue;
                green = StoredGreen;
            } else {
                StoredRed = red;
                StoredGreen = green;
                StoredBlue = blue;
            }
            unknown_1b0 = true;
        } else {
            if (red == -1) {
                red = Red;
                green = Green;
                blue = Blue;
            } else {
                Red = red;
                Green = green;
                Blue = blue;
            }
            unknown_1b0 = false;
        }
        int red_step = (int)(red * 1000 * 0.065536);
        int green_step = (int)(green * 1000 * 0.065536);
        int blue_step = (int)(blue * 1000 * 0.065536);
        int last = unknown_shadow_count - 1;
        unsigned short *table = (unsigned short *)AlphaPalette;
        int limit = unknown_shadow_count * 30 / 200 - 1;
        limit = (limit > 0) ? limit : 0;
        if (limit >= (last >> 1)) {
            limit = last >> 1;
        }
        for (int i = 0; i <= unknown_shadow_count - 1; i++) {
            int r;
            int g;
            int b;
            int level;
            if (unknown_shadow_count > 1) {
                r = red_step * 2 * i / last;
                g = green_step * 2 * i / last;
                b = blue_step * 2 * i / last;
                if (i > limit) {
                    level = 0x10000;
                } else {
                    level = i * 0x10000 / limit;
                }
            } else {
                r = red_step;
                g = green_step;
                b = blue_step;
                level = 0x10000;
            }
            switch (DAT_007df868) {
            case 0:
                FUN_0079777a(ArtPalette, table, r, g, b, level, unknown_190);
                table += 256;
                break;
            case 1:
                FUN_00797aba(ArtPalette, table, r, g, b, level, unknown_190);
                table += 256;
                break;
            case 2:
                FUN_00797440(ArtPalette, table, r, g, b, level, unknown_190);
                table += 256;
                break;
            case 3:
                FUN_00797dfa(ArtPalette, table, r, g, b, level, unknown_190);
                table += 256;
                break;
            default:
                *table++ = 0;
                for (int c = 1; c < 256; c++) {
                    int pr;
                    int pg;
                    int pb;
                    if (unknown_190[c] != 0) {
                        pr = (*ArtPalette)[c].Red * r >> 16;
                        pg = (*ArtPalette)[c].Green * g >> 16;
                        pb = (*ArtPalette)[c].Blue * b >> 16;
                    } else {
                        pr = (*ArtPalette)[c].Red * level >> 16;
                        pg = (*ArtPalette)[c].Green * level >> 16;
                        pb = (*ArtPalette)[c].Blue * level >> 16;
                    }
                    if (pr >= 255) pr = 255;
                    if (pg >= 255) pg = 255;
                    if (pb >= 255) pb = 255;
                    *table++ = (unsigned short)Build_Hicolor_Pixel(pr, pg, pb);
                }
                break;
            }
        }
    }
}


// LightConvertClass::~LightConvertClass() (0x00538f20)

LightConvertClass::~LightConvertClass()
{
}


// 0x00538f30: function not yet identified
//
// Owner: LightConvertClass, virtual slot 0 (vtable +0x0) (from RTTI/vtables).
// Body: 36 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 * __thiscall FUN_00538f30(undefined4 *param_1,byte param_2)
// Callees with accepted source:
//   FUN_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00538f30 {

extern void __fastcall FUN_00485310(void);
extern void  FUN_00781d3b(unsigned int allocation_address);
extern "C" void* PTR_FUN_007a55dc;

struct Owner_00538f30 { unsigned int * FUN_00538f30(unsigned char deletion_flags); };

unsigned int * Owner_00538f30::FUN_00538f30(unsigned char deletion_flags)
{
    unsigned int * object_words = (unsigned int *)this;
    *object_words = (unsigned int)(&PTR_FUN_007a55dc);
    FUN_00485310();
    if ((deletion_flags & 1) != 0) {
        FUN_00781d3b((unsigned int)(object_words));
    }
    return object_words;
}

}  // namespace seed_00538f30
