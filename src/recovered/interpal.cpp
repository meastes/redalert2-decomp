// interpal.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/interpal.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unknown_static_rect.hpp"
#define RGB_DECLARE_TO_HSV
#include "shared/hsv_class.hpp"
#include "shared/rgb_class.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/surface.hpp"
#include "shared/main_loop.hpp"
#include "shared/convert.hpp"


// Dynamic initializer of this unit's copy of unknown_static_rect (0x0051E800, _$E;
// 0x00A417D0, all zero): emitted by shared/unknown_static_rect.hpp.


// Tiberian Sun's interpal.cpp: the 2x interpolation blend table, its palette and the
// 2x scaling copy. PaletteInterpolationTable (0x00A417E4) holds, for every pair of
// palette entries, the entry closest to their average; InterpolationPalette (0x00A417E0)
// is the palette it is built from; InterpolationPaletteChanged (0x00A517E4) asks for a
// rebuild and CopyType (0x00A517E8) picks the assembly copy routine.

extern "C" {
void __cdecl Asm_Interpolate(unsigned char *src_ptr, unsigned char *dest_ptr, int lines, int src_width,
    int dest_width);                                                    // 0x00799241
void __cdecl Asm_Interpolate_Line_Double(unsigned char *src_ptr, unsigned char *dest_ptr, int lines,
    int src_width, int dest_width);                                     // 0x007992E6
void __cdecl Asm_Interpolate_Line_Interpolate(unsigned char *src_ptr, unsigned char *dest_ptr, int lines,
    int src_width, int dest_width);                                     // 0x0079943B
}

bool InterpolationPaletteChanged;

extern "C" {
unsigned char PaletteInterpolationTable[256][256];
unsigned char *InterpolationPalette;
}

// Reads the blend table from palette_file_name when it exists (0x0051E820).
void __fastcall Read_Interpolation_Palette(char const *palette_file_name)
{
    CCFileClass palette_file(palette_file_name);

    if (palette_file.Is_Available()) {
        palette_file.Open(FILE_RIGHTS_READ);
        palette_file.Read(&PaletteInterpolationTable[0][0], 256 * 256);
        palette_file.Close();
        InterpolationPaletteChanged = false;
    }
}


// Writes the blend table to palette_file_name unless that file exists (0x0051E8A0).
void __fastcall Write_Interpolation_Palette(char const *palette_file_name)
{
    CCFileClass palette_file(palette_file_name);

    if (!palette_file.Is_Available()) {
        palette_file.Open(FILE_RIGHTS_WRITE);
        palette_file.Write(&PaletteInterpolationTable[0][0], 256 * 256);
        palette_file.Close();
    }
}


// Builds the blend table from InterpolationPalette: for each pair of entries the entry
// nearest (squared RGB distance) to their average (0x0051E920).
void __fastcall Create_Palette_Interpolation_Table()
{
    int i;
    int j;
    int p;
    unsigned char *first_palette_ptr;
    unsigned char *second_palette_ptr;
    unsigned char *match_pal_ptr;
    int first_r;
    int first_g;
    int first_b;
    int second_r;
    int second_g;
    int second_b;
    int diff_r;
    int diff_g;
    int diff_b;
    int dest_r;
    int dest_g;
    int dest_b;
    int distance;
    int closest_distance;
    int index_of_closest_color;

    first_palette_ptr = InterpolationPalette;
    for (i = 0; i < 256; i++) {
        first_r = *first_palette_ptr;
        first_palette_ptr++;
        first_g = *first_palette_ptr;
        first_palette_ptr++;
        first_b = *first_palette_ptr;
        first_palette_ptr++;

        second_palette_ptr = InterpolationPalette;
        for (j = 0; j < 256; j++) {
            second_r = *second_palette_ptr;
            second_palette_ptr++;
            second_g = *second_palette_ptr;
            second_palette_ptr++;
            second_b = *second_palette_ptr;
            second_palette_ptr++;

            dest_r = (first_r + second_r) >> 1;
            dest_g = (first_g + second_g) >> 1;
            dest_b = (first_b + second_b) >> 1;

            index_of_closest_color = 0;
            closest_distance = 500000;
            match_pal_ptr = InterpolationPalette;
            for (p = 0; p < 256; p++) {
                diff_r = (int)*match_pal_ptr - dest_r;
                match_pal_ptr++;
                diff_g = (int)*match_pal_ptr - dest_g;
                match_pal_ptr++;
                diff_b = (int)*match_pal_ptr - dest_b;
                match_pal_ptr++;

                distance = diff_r * diff_r + diff_g * diff_g + diff_b * diff_b;
                if (distance < closest_distance) {
                    closest_distance = distance;
                    index_of_closest_color = p;
                }
            }

            PaletteInterpolationTable[i][j] = (unsigned char)index_of_closest_color;
        }
    }

    InterpolationPaletteChanged = false;
}


// Scales the value (HSV) of every palette entry by percentage, clamped to 0..255 (0x0051EA50).
void __fastcall Increase_Palette_Luminance(PaletteClass &palette, double percentage)
{
    for (int i = 0; i < 256; i++) {
        HSVClass hsv = palette[i];

        int value = (int)(hsv.Value * percentage);
        value = value < 255 ? value : 255;
        value = value > 0 ? value : 0;
        hsv.Value = (unsigned char)value;
        RGBClass rgb = hsv.To_RGB();
        palette[i] = rgb;
    }
}


// Tiberian Sun interpal.cpp (continued).
int CopyType = 0;

// Copies source to dest at twice the size with one of the assembly routines; the blend
// table is read from palette_file_name or rebuilt first when the palette changed, then
// written back (0x0051EB00).
void __fastcall Interpolate_2X_Scale(Surface *source, Surface *dest, char const *palette_file_name)
{
    unsigned char *src_ptr;
    unsigned char *dest_ptr;
    int src_width;
    int dest_width;
    bool source_locked = false;
    bool dest_locked = false;

    if (InterpolationPaletteChanged) {
        if (palette_file_name) {
            Read_Interpolation_Palette(palette_file_name);
        }
        if (InterpolationPaletteChanged) {
            Create_Palette_Interpolation_Table();
        }
    }

    if (palette_file_name) {
        Write_Interpolation_Palette(palette_file_name);
    }

    Wait_For_Surface_Blit_004961d0();

    src_ptr = (unsigned char *)source->Lock();
    if (src_ptr != 0) {
        source_locked = true;
    }

    dest_ptr = (unsigned char *)dest->Lock();
    if (dest_ptr != 0) {
        dest_locked = true;
    }

    if (dest_locked && source_locked) {
        src_width = source->Get_Width();
        dest_width = 2 * dest->Stride();

        switch (CopyType) {
            case 0:
                Asm_Interpolate(src_ptr, dest_ptr, source->Get_Height(), src_width, dest_width);
                break;

            case 1:
                Asm_Interpolate_Line_Double(src_ptr, dest_ptr, source->Get_Height(), src_width, dest_width);
                break;

            case 2:
                Asm_Interpolate_Line_Interpolate(src_ptr, dest_ptr, source->Get_Height(), src_width, dest_width);
                break;
        }
    }

    if (source_locked) {
        source->Unlock();
    }
    if (dest_locked) {
        dest->Unlock();
    }
}
